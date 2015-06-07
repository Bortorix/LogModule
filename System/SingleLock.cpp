
#include "SingleLock.h"

using namespace MTSp;

SingleLock::SingleLock (SyncObject *sync_obj, bool initialLock /*= false*/) {
	syncObj = sync_obj;
	acquired = false;
	if (initialLock) lock ();
}

bool SingleLock::lock (long timeoutInMSec /*= INFINITE*/) {

	if (acquired == false) {
		acquired = syncObj->lock (timeoutInMSec);
		return acquired;
	} else {
		return false;
	}
}

bool SingleLock::unlock () {
	if (acquired) {
		acquired = !syncObj->unlock ();
	}
	return !acquired;
}

bool SingleLock::unlock (long count, long *prevCount /*= 0*/) {
	if (acquired) {
		acquired = !syncObj->unlock (count, prevCount);
	}
	return !acquired;
}