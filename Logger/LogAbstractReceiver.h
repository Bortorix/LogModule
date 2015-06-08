
#ifndef _LOG_ABSTRACT_RECEIVER_H_
#define _LOG_ABSTRACT_RECEIVER_H_

#include <string>
#include "LogMsg.h"
#include "LogFormat.h"

namespace LoggerSp {

	class LogAbstractReceiver {
		public:

			enum LogReceiverState {
				Open      = 1,
				Close     = 2
			};

			LogAbstractReceiver (std::wstring connectionData, const LogMsg::LogPriority &minPriority = LogMsg::DebugPriority,
				               const LogFormat &format = LogFormat::Time | LogFormat::Prior) {
			    
				setConnectionData (connectionData);
				setState (LogAbstractReceiver::Close);
				setMinPriority (minPriority);
				setLogFormat (format);
			}
			virtual ~LogAbstractReceiver () {}

			virtual int transmit (const LogMsg &msg, const LogFormat &fmt) = 0;
			virtual int connect () = 0;
			virtual int disconnect () = 0;

			void setConnectionData (const std::wstring &connectionData) {_connectionData.assign (connectionData.begin (), connectionData.end ());}
			std::wstring connectionData () const {return _connectionData;}

			void setState (const LogReceiverState &st) {_state = st;}
			LogReceiverState state () const {return _state;}

			void setMinPriority (const LogMsg::LogPriority &_minPrior) {_minPriority = _minPrior;}
			LogMsg::LogPriority minPriority () const {return _minPriority;}

			void setLogFormat (const LogFormat &format) {_format = format;}
			LogFormat logFormat () const {return _format;}

			bool isAcceptReceive (const LogMsg::LogPriority &priority) {return minPriority () <= priority ? true : false;}

		protected:
			std::wstring _connectionData;
			LogReceiverState _state;
			LogMsg::LogPriority _minPriority;
			LogFormat _format;
	};
}
#endif // _LOG_ABSTRACT_RECEIVER_H_