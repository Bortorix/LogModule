﻿
#include <time.h>
#include "Logger.h"
#include "..\System\SingleLock.h"
#include "LogWCharFileWriter.h"

#pragma warning(disable:4996)

using namespace LoggerSp;

Logger * Logger::_instance = 0;
HANDLE Logger::loggerThreadHandle = NULL;

Logger * Logger::instance () {
    if (_instance == 0) {
		_instance = new Logger ();
		_instance->writers.clear ();
		
		_instance->isFreezeWriteOperations = false;

		_instance->addWriter (Logger::FILEWriter,  new LoggerSp::LogWCharFileWriter (_T("")));
		//_instance->addWriter (Logger::MSSQLWriter, new ); // To do later
		//_instance->addWriter (Logger::UDPWriter,   new ); // To do later

		_instance->startLoggerThreads (_instance);
    }
    return _instance;
}

void Logger::remove () {

	if (_instance != 0) {
		finishLoggerThread (_instance);
		for (std::map<WId, LoggerSp::LogAbstractWriter *>::iterator i  = _instance->writers.begin () ;
																	i != _instance->writers.end () ; ++i) delete (i->second);
		delete _instance;
		_instance = 0;
	}
}

void Logger::logMsg (const std::wstring &message, const LogMsg::LogPriority &priority,
					 const std::string &srcFilename, const std::string &funName, const unsigned int &rowNumber) {
	LogMsg msg;
	
	_time64 (&msg.ltime); // Fixed current time
	msg.msg.assign (message.begin (), message.end ());
	msg.logPriority = priority;
	msg.srcFilename.assign (srcFilename.begin (), srcFilename.end ());
	msg.funName.assign (funName.begin (), funName.end ());
	msg.rowNumber = rowNumber;

	MTSp::SingleLock lock (&defQueueOfLogMsg_CS, true);
	queueOfLogMsg.push (msg);
	isExistNewMsgEvent.setEvent ();
	lock.unlock ();
}

void Logger::startLoggerThreads (Logger *pLogger) {

	pLogger->isExistNewMsgEvent.resetEvent ();
	pLogger->finishThreadEvent.resetEvent ();
	pLogger->finishedThreadEvent.resetEvent ();

	DWORD threadId;
	Logger::loggerThreadHandle = CreateThread (NULL, 0, Logger::executeLoggerThread, (LPVOID) pLogger, 0, &threadId);

	if (Logger::loggerThreadHandle != INVALID_HANDLE_VALUE) {
		SetThreadPriority (Logger::loggerThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
		//SetThreadAffinityMask (Logger::loggerThreadHandle, SystemSp::System::getAffinityMaskForNotCriticalTimeThreads ());
	}
}

void Logger::finishLoggerThread (Logger *pLogger) {
	pLogger->finishThreadEvent.setEvent ();

	if (WaitForSingleObject ((HANDLE) pLogger->finishedThreadEvent, 5000) != WAIT_OBJECT_0) { //Поток не закончил своё существование
		TerminateThread (Logger::loggerThreadHandle, 0);
		pLogger->finishedThreadEvent.setEvent ();
	}
	CloseHandle (Logger::loggerThreadHandle);
	Logger::loggerThreadHandle = NULL;
}

bool Logger::getMsg (LogMsg &msg) {

	MTSp::SingleLock lock (&defQueueOfLogMsg_CS, true);

	if (queueOfLogMsg.empty ()) {
		isExistNewMsgEvent.resetEvent ();
		lock.unlock ();
		return false;
	}

	msg.swap (queueOfLogMsg.front ());
	queueOfLogMsg.pop ();

	if (queueOfLogMsg.empty ()) {
		isExistNewMsgEvent.resetEvent ();
	} else {
		isExistNewMsgEvent.setEvent ();
	}
	lock.unlock ();

	return true;
}

DWORD __stdcall Logger::executeLoggerThread (LPVOID pParam) {

	static const short countOfEvents = 2;
	std::vector<HANDLE> handles (countOfEvents);
	Logger *lgr = (Logger *)pParam;
	handles [0] = (HANDLE) lgr->isExistNewMsgEvent;
	handles [1] = (HANDLE) lgr->finishThreadEvent;
	
	LogMsg msg;
	bool isContinue = true;

	while (isContinue) {

		//Функция WaitForMultipleObjects возвращает индекс первого события со статусом signaled,
		//поэтому пока очередь с сообщениями не будет сохранена в файлы, поток будет продолжать работать
		DWORD index = WaitForMultipleObjects (countOfEvents, &handles [0], FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (index) {
			case 0: {
				
				try {
					if (lgr->isFreezeWriteOperations) {::Sleep (10); break;}
					if (lgr->getMsg (msg) == false) break;
					for (std::map<Logger::WId, LogAbstractWriter *>::const_iterator i = lgr->writers.begin () ; i != lgr->writers.end () ; ++i) {
						LogAbstractWriter *writer = (*i).second;

						if (writer->state () == LogAbstractWriter::Open) {
							if (writer->isAcceptWrite (msg.logPriority)) {
								try {
									if (writer->connect () == 0) { // Не самая эффективная реализация сохранения данных
										writer->write (msg, writer->logFormat ());
									}
									writer->disconnect ();
								} catch (...) { // input/output operation
									try {writer->disconnect ();} catch (...) {}
								}
							}
						}
					}
				} catch (...) {} 
				break;	
			}
			case 1: {
				isContinue = false;
				break;
			}
		}
	}
	
	lgr->finishedThreadEvent.setEvent ();
	return 0;
}
