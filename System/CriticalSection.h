
#ifndef _CRITICAL_SECTION_H_
#define _CRITICAL_SECTION_H_

#include "SyncObject.h"

namespace MTSp {
	class CriticalSection : public SyncObject {
	public:
		explicit CriticalSection () {::InitializeCriticalSection (&c_sect);}
		virtual ~CriticalSection () {::DeleteCriticalSection (&c_sect);}
		
		operator CRITICAL_SECTION* () {return &c_sect;}
		
		bool lock () {::EnterCriticalSection (&c_sect); return true;}
		bool lock (long ignoredTimeoutInMSec) {return lock ();}
		bool unlock () {::LeaveCriticalSection (&c_sect); return true;}

		CRITICAL_SECTION c_sect;
	};
}

#endif // _CRITICAL_SECTION_H_