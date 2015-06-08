
#ifndef _LOG_WCHAR_FILE_RECEIVER_H_
#define _LOG_WCHAR_FILE_RECEIVER_H_

#include "LogAbstractReceiver.h"
#include "LogMsg.h"
#include "LogFormat.h"

namespace LoggerSp {
	class LogWCharFileReceiver : public LogAbstractReceiver {
	
		public:
			LogWCharFileReceiver (std::wstring _connectionData) : LogAbstractReceiver (_connectionData) {pOFS = NULL;}
			~LogWCharFileReceiver () {}

			virtual int transmit (const LogMsg &msg, const LogFormat &fmt);
			virtual int connect ();
			virtual int disconnect ();

		private:
			std::wstring createTimeWStr (const LogMsg &msg) const;
			std::wstring createPriorityWStr (const LogMsg &msg) const;

			std::wstring createMsg (const LogMsg &msg, const LogFormat &logFormat) const;

			std::string LogWCharFileReceiver::utf8_encode(const std::wstring &wstr);
			std::ofstream *pOFS;
	};
}

#endif // _LOG_WCHAR_FILE_RECEIVER_H_