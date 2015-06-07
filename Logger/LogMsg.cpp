
#include "LogMsg.h"

using namespace LoggerSp;

std::wstring LogMsg::logPriorityAsStr (const LogPriority &lLev) {
	std::wstring resStr;

	switch (lLev) {
			case LogMsg::DebugPriority:
				resStr = _T("DEBUG");
				break;
			case LogMsg::InfoPriority:
				resStr = _T(" INFO");
				break;
			case LogMsg::WarnPriority:
				resStr = _T(" WARN");
				break;
			case LogMsg::ErrorPriority:
				resStr = _T("ERROR");
				break;
			case LogMsg::FatalPriority:
				resStr = _T("FATAL");
				break;
			default:
				resStr = _T("UNKNOWN");
				break;
	}
	return resStr;
}