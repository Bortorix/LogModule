
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>
#include "LogWCharFileReceiver.h"

using namespace LoggerSp;

int LogWCharFileReceiver::connect () {

	if (_connectionData.empty ()) return -1;

	size_t pos = _connectionData.rfind (L'\\') + 1;
	std::wstring filename = _connectionData.substr (pos, _connectionData.length () - pos);
	std::wstring dir      = _connectionData.substr (0, pos);
	
	if (CreateDirectory (dir.c_str (), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		
		pOFS = new std::ofstream (_connectionData.c_str(), std::ios::app);
		if (!pOFS->is_open ()) return -2;
	} else {
		pOFS = NULL;
		return -3;
	}
	return 0;
}

int LogWCharFileReceiver::disconnect () {
	if (pOFS != NULL) {
		pOFS->close ();
		delete pOFS;
	}
	pOFS = NULL;
	return 0;
}

int LogWCharFileReceiver::transmit (const LogMsg &msg, const LogFormat &fmt) {
	
	std::wstring str = createMsg (msg, fmt);
	(*pOFS) << utf8_encode (str) << std::endl << std::flush;
	return 0;
}

std::wstring LogWCharFileReceiver::createMsg (const LogMsg &lmsg, const LogFormat &logFormat) const {
	
	std::wstringstream sstr;
	bool isAppend = false;

	if (logFormat.isContains (LogFormat::Time)) {
		if (isAppend) sstr << _T(" ");
		std::wstring tws = createTimeWStr (lmsg);
		sstr << tws;
		isAppend = true;
	}

	if (logFormat.isContains (LogFormat::Prior)) {
		if (isAppend) sstr << _T(", ");
		std::wstring pws = createPriorityWStr (lmsg);
		sstr << pws;
		isAppend = true;
	}

	sstr << _T(": ") << lmsg.msg;
	isAppend = true;

	if (logFormat.isContains (LogFormat::Fname)) {
		if (isAppend) sstr << _T(", ");
		sstr << lmsg.srcFilename.c_str ();
		isAppend = true;
	}

	if (logFormat.isContains (LogFormat::FunName)) {
		if (isAppend) sstr << _T(", ");
		sstr << lmsg.funName.c_str ();
		isAppend = true;
	}

	if (logFormat.isContains (LogFormat::RowNum)) {
		if (isAppend) sstr << _T(", ");
		sstr << _T("Row: ") << lmsg.rowNumber;
		isAppend = true;
	}
	
	return sstr.str ();
}

std::wstring LogWCharFileReceiver::createTimeWStr (const LogMsg &msg) const {
	wchar_t tws[26];

	_wctime64_s (tws, 26, &msg.ltime);
	tws [24] = _T('\0');

	// Short version of time string
	//memmove ((tws + 0), (tws + 4 * sizeof (wchar_t)), 15 * sizeof (wchar_t));
	//tws [11] = _T('\0');

	return std::wstring (tws);
}

std::wstring LogWCharFileReceiver::createPriorityWStr (const LogMsg &msg) const {
	return LogMsg::logPriorityAsStr (msg.logPriority);
}

// Convert a wide Unicode string to an UTF8 string
std::string LogWCharFileReceiver::utf8_encode(const std::wstring &wstr) {

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &(wstr.c_str ()[0]), (int)wstr.size(), NULL, 0, NULL, NULL);
	if (size_needed == 0) return "";

	std::vector<char> strTo(size_needed, 0);
	WideCharToMultiByte                  (CP_UTF8, 0, &(wstr.c_str ()[0]), (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

	return (std::string (strTo.begin (), strTo.end ()));
}