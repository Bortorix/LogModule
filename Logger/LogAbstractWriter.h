
#ifndef _ABSTRACT_WRITER_H_
#define _ABSTRACT_WRITER_H_

#include <string>
#include "LogMsg.h"
#include "LogFormat.h"

namespace LoggerSp {

	class LogAbstractWriter {
		public:

			enum LogWriterState {
				Open      = 1,
				Close     = 2
			};

			LogAbstractWriter (std::wstring connectionData, const LogMsg::LogPriority &minPriority = LogMsg::DebugPriority,
				               const LogFormat &format = LogFormat::Time | LogFormat::Prior) {
			    
				setConnectionData (connectionData);
				setState (LogAbstractWriter::Close);
				setMinPriority (minPriority);
				setLogFormat (format);
			}
			virtual ~LogAbstractWriter () {}

			virtual int write (const LogMsg &msg, const LogFormat &fmt) = 0;
			virtual int connect () = 0;
			virtual int disconnect () = 0;

			void setConnectionData (const std::wstring &connectionData) {_connectionData.assign (connectionData.begin (), connectionData.end ());}
			std::wstring connectionData () const {return _connectionData;}

			void setState (const LogWriterState &st) {_state = st;}
			LogWriterState state () const {return _state;}

			void setMinPriority (const LogMsg::LogPriority &_minPrior) {_minPriority = _minPrior;}
			LogMsg::LogPriority minPriority () const {return _minPriority;}

			void setLogFormat (const LogFormat &format) {_format = format;}
			LogFormat logFormat () const {return _format;}

			bool isAcceptWrite (const LogMsg::LogPriority &priority) {return minPriority () <= priority ? true : false;}

		protected:
			std::wstring _connectionData;
			LogWriterState _state;
			LogMsg::LogPriority _minPriority;
			LogFormat _format;
	};
}
#endif // _ABSTRACT_WRITER_H_