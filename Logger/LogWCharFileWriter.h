
#ifndef _FILE_WCHAR_WRITER_H_
#define _FILE_WCHAR_WRITER_H_

#include "LogAbstractWriter.h"
#include "LogMsg.h"
#include "LogFormat.h"

namespace LoggerSp {
	class LogWCharFileWriter : public LogAbstractWriter {
	
		public:
			LogWCharFileWriter (std::wstring _connectionData) : LogAbstractWriter (_connectionData) {pOFS = NULL;}
			~LogWCharFileWriter () {}

			virtual int write (const LogMsg &msg, const LogFormat &fmt);
			virtual int connect ();
			virtual int disconnect ();

		private:
			std::wstring createTimeWStr (const LogMsg &msg) const;
			std::wstring createPriorityWStr (const LogMsg &msg) const;

			std::wstring createMsg (const LogMsg &msg, const LogFormat &logFormat) const;

			std::string LogWCharFileWriter::utf8_encode(const std::wstring &wstr);
			std::ofstream *pOFS;
	};
}

#endif // _FILE_WCHAR_WRITER_H_