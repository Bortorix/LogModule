
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <map>
#include <queue>
#include "LogAbstractWriter.h"
#include "..\System\CriticalSection.h"
#include "..\System\Event.h"

namespace LoggerSp {

	#define LOG_WTRACE(msg, priority) Logger::instance ()->logMsg (msg, priority, __FILE__, __FUNCSIG__, __LINE__);


	class Logger {
		public:

			/* Writer's identifiers */
			enum WId {
				FILEWriter  = 1,
				MSSQLWriter = 2, // В настоящий момент не реализован !!!
				UDPWriter   = 3  // В настоящий момент не реализован !!!
			};

			static Logger *instance ();
			static void remove ();

			static const char * moduleName () {return "Logger";}
        
			void freezeWriteOperations () {isFreezeWriteOperations = true;}
			void unfreezeWriteOperations () {isFreezeWriteOperations = false;}

			bool setWriterConnectionData (const WId &wId, const std::wstring &connectionData) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				i->second->setConnectionData (connectionData);
				return true;
			}

			bool getWriterConnectionData (const WId &wId, std::wstring &_connectionData) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				_connectionData = i->second->connectionData ();
				return true;
			}

			bool setWriterState (const WId &wId, const LogAbstractWriter::LogWriterState &state) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				i->second->setState (state);
				return true;
			}

			bool getWriterState (const WId &wId, LogAbstractWriter::LogWriterState &state) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				state = i->second->state ();
				return true;
			}

			bool setPriority (const WId &wId, const LogMsg::LogPriority &priority) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				i->second->setMinPriority (priority);
				return true;
			}

			bool getPriority (const WId &wId, LogMsg::LogPriority &priority) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				priority = i->second->minPriority ();
				return true;
			}

			bool setFormat (const WId &wId, const LoggerSp::LogFormat &format) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				i->second->setLogFormat (format);
				return true;
			}

			bool getPriority (const WId &wId, LoggerSp::LogFormat &format) {
				std::map<WId, LogAbstractWriter *>::iterator i = writers.find (wId);
				if (i == writers.end ()) return false; // Writer with the key wId not found
				format = i->second->logFormat ();
				return true;
			}

			void logMsg (const std::wstring &message, const LogMsg::LogPriority &priority,
				         const std::string &srcFilename, const std::string &funName, const unsigned int &rowNumber);

		private:
			Logger () : isExistNewMsgEvent (false, true), finishThreadEvent (false, true), finishedThreadEvent (false, true) {}
			Logger (const Logger &) : isExistNewMsgEvent (false, true), finishThreadEvent (false, true), finishedThreadEvent (false, true) {}
			static Logger *_instance;

			//Запрещаем копирование модуля логов
			Logger & operator= (const Logger &) {return *this;}

			std::map<WId, LogAbstractWriter *> writers;

			bool isFreezeWriteOperations;

			bool addWriter (const WId &wId, LogAbstractWriter *writer, const LogAbstractWriter::LogWriterState &state = LogAbstractWriter::Close) {
				if (writers.find (wId) != writers.end ()) return false; // Already added Writer with key wId
				writer->setState (state);
				writers.insert (std::map<WId, LogAbstractWriter *>::value_type (wId, writer));
				return true;
			}

			/*************************************************************/
			/* Запускает поток, сохраняющий сообщения в log файлы        */
			/*************************************************************/
			static void startLoggerThreads (Logger *pLogger);

			/*************************************************************/
			/* Останавливает поток, сохраняющий сообщения в log файлы    */
			/*************************************************************/
			static void finishLoggerThread (Logger *pLogger);

			static DWORD __stdcall executeLoggerThread (LPVOID pParam);
			static HANDLE loggerThreadHandle;

			MTSp::Event isExistNewMsgEvent;
			MTSp::Event finishThreadEvent;
			MTSp::Event finishedThreadEvent;

			std::queue<LogMsg> queueOfLogMsg;
			MTSp::CriticalSection defQueueOfLogMsg_CS;

			bool getMsg (LogMsg &msg);
	};
}

#endif // _LOGGER_H_