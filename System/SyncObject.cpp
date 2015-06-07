
#include "SyncObject.h"

using namespace MTSp;

bool SyncObject::lock (long timeoutInMSec /*= INFINITE*/) {
	long answer = static_cast<long> (::WaitForSingleObject (handle, timeoutInMSec));
	
	if (answer == WAIT_OBJECT_0 || answer == WAIT_ABANDONED) {
		return true;
	} else {
		return false;
	}
}