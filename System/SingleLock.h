
#ifndef _SINGLE_LOCK_H_
#define _SINGLE_LOCK_H_

#include "SyncObject.h"

namespace MTSp {

	class SingleLock {
	public:
		explicit SingleLock (SyncObject *sync_obj, bool initialLock = false);
		~SingleLock () {unlock ();}

		bool lock (long timeoutInMSec = INFINITE);
		bool unlock ();
		bool unlock (long count, long *prevCount = 0);
		bool isLocked () {return acquired;}

	protected:
		SyncObject *syncObj;
		bool acquired;
	};
}

#endif // _SINGLE_LOCK_H_