// Logger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Logger/Logger.h"

using namespace LoggerSp;

void setupLogger (Logger *lgr) {
	lgr->setReceiverConnectionData (Logger::FILEWReceiver, _T(".\\logs\\имя_приложения.log"));
	lgr->setPriority (Logger::FILEWReceiver, LogMsg::InfoPriority);
	lgr->setFormat (Logger::FILEWReceiver, LogFormat::Prior | LogFormat::Time | LogFormat::FunName | LogFormat::Fname | LogFormat::RowNum);
	lgr->setReceiverState (Logger::FILEWReceiver, LogAbstractReceiver::Open);
}

void message1 (int a) {
	a = 0; // Dummy code
	LOG_WTRACE (_T("Собщение передаваемое в лог-файл (приоритет DEBUG)"), LogMsg::DebugPriority);
}

int message2 (long b) {
	b = 0; // Dummy code
	b++;   // Dummy code
	LOG_WTRACE (_T("Собщение передаваемое в лог-файл (приоритет  INFO)"), LogMsg::InfoPriority);
	return 0; // Dummy code
}

char * message3 (Logger *lgr) {
	lgr = 0; // Dummy code
	lgr++;   // Dummy code
	LOG_WTRACE (_T("Собщение передаваемое в лог-файл (приоритет  WARN)"), LogMsg::WarnPriority);
	return ""; // Dummy code
}

int _tmain(int argc, _TCHAR* argv[])
{
	Logger *logger = Logger::instance ();

	setupLogger (logger);
	logger->unfreezeLogMessageBroadcast ();

	message1 (0 /* Dummy value */);
	message2 (2 /* Dummy value */);
	message3 (NULL /* Dummy value */);
	
	LOG_WTRACE (_T("Собщение передаваемое в лог-файл (приоритет ERROR)"), LogMsg::ErrorPriority);
	LOG_WTRACE (_T("Собщение передаваемое в лог-файл (приоритет FATAL)"), LogMsg::FatalPriority);

	Logger::remove ();
	return 0;
}

