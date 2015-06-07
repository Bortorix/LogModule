
#ifndef _LOG_MSG_H_
#define _LOG_MSG_H_

#include <string>
#include <windows.h>
#include <time.h>
#include <tchar.h>

namespace LoggerSp {

	class LogMsg {
	public:

		LogMsg () {fill (_T(""), 0, LogMsg::DebugPriority, "", "", 0);}
		LogMsg (const LogMsg &logMsg) {this->operator= (logMsg);}
		
		LogMsg & operator= (const LogMsg &logMsg) {
			fill (logMsg.msg, logMsg.ltime, logMsg.logPriority, logMsg.srcFilename, logMsg.funName, logMsg.rowNumber);
			return *this;
		}

		enum LogPriority {
			DebugPriority = 1,
			InfoPriority  = 2,
			WarnPriority  = 3,
			ErrorPriority = 4,
			FatalPriority = 5
		};
		
		void swap (LogMsg &msg) {
			this->msg.swap (msg.msg);

			__time64_t lt = msg.ltime;
			msg.ltime     = ltime;
			ltime         = lt;
			
			this->srcFilename.swap (msg.srcFilename);
			this->funName.swap (msg.funName);

			LogPriority lp  = msg.logPriority;
			msg.logPriority = logPriority;
			logPriority     = lp;
			
			unsigned int rn = msg.rowNumber;
			msg.rowNumber   = rowNumber;
			rowNumber       = rn;
		}

		static std::wstring logPriorityAsStr (const LogPriority &lLev);

		std::wstring msg;
		__time64_t ltime;
		std::string srcFilename;
		std::string funName;
		LogPriority logPriority;
		unsigned int rowNumber;

	private:
		void fill (const std::wstring &msg, const __time64_t &time,
			       const LogPriority &logPriority,
				   const std::string srcFilename,
				   const std::string &funName,
				   const unsigned int &rowNumber) {

			this->msg.assign (msg.begin (), msg.end ());
			this->ltime       = time;
			this->srcFilename.assign (srcFilename.begin (), srcFilename.end ());
			this->funName.assign (funName.begin (), funName.end ());
			this->logPriority = logPriority;
			this->rowNumber   = rowNumber;
		}
	};
}

#endif // LOG_MSG_H_