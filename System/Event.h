
#ifndef _EVENT_H_
#define _EVENT_H_

#include "SyncObject.h"

namespace MTSp {
	class Event : public SyncObject {
	public:
		explicit Event (bool initiallyOwn = false, bool manualReset = false, LPTSTR name = 0, LPSECURITY_ATTRIBUTES attributes = 0) {
			handle = ::CreateEvent (attributes, manualReset, initiallyOwn, name);
		}

		virtual ~Event () {}

		bool setEvent ()   {return (::SetEvent   (handle) != 0);}
		bool pulseEvent () {return (::PulseEvent (handle) != 0);}
		bool resetEvent () {return (::ResetEvent (handle) != 0);}
		virtual bool unlock () {return true;}
	};
}

#endif // _EVENT_H_