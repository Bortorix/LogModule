
#ifndef _SYNC_OBJECT_H_
#define _SYNC_OBJECT_H_

#include <Windows.h>

namespace MTSp {
	class SyncObject {
	public:
		SyncObject () {handle = 0;}
		virtual ~SyncObject () {if (handle != 0) {::CloseHandle (handle); handle = 0;}}
		operator HANDLE () const {return handle;}
		HANDLE handle;

		virtual bool lock (long timeoutInMSec = INFINITE);
		virtual bool unlock () = 0;
		virtual bool unlock (long count, long *prevCount = 0) {return true;}
	};
}
#endif // _SYNC_OBJECT_H_