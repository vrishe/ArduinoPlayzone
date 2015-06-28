// transmit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#if defined(UNICODE) || defined(_UNICODE)
#	define _tcout			wcout
#else
#	define _tcout			cout
#endif


BOOL WaitIO(BOOL result, HANDLE hFile, OVERLAPPED &overlapped, LPDWORD bytesTransferred) {
	if (!result) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			result = FALSE;
		}
		else {
			// Write is pending.
			DWORD dwRes = WaitForSingleObject(overlapped.hEvent, INFINITE);

			switch (dwRes)
			{
				// OVERLAPPED structure's event has been signaled. 
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(hFile, &overlapped, bytesTransferred, FALSE)) {
					result = FALSE;
				}
				else {
					// Write operation completed successfully.
					result = TRUE;
				}
				break;

			default:
				// An error has occurred in WaitForSingleObject.
				// This usually indicates a problem with the
				// OVERLAPPED structure's event handle.
				result = FALSE;
				break;
			}
		}
	}
	else {
		// WriteFile completed immediately.
		result = TRUE;
	}
	if (overlapped.hEvent != NULL && overlapped.hEvent != INVALID_HANDLE_VALUE) {
		CloseHandle(overlapped.hEvent);
	}
	CloseHandle(hFile);

	return result;
}


// Routine Description:
// Retrieve and output the system error message for the last-error code
DWORD DisplayError(LPTSTR lpszFunction);

std::map<std::_tstring, std::_tstring> readArgs(int argc, _TCHAR *argv[], std::vector<std::_tstring> *outFreeParams, 
	size_t paramsCount, const _TCHAR *params, const _TCHAR *const paramsExt[]) {

	std::vector<std::_tstring> paramsVector;

	for (const _TCHAR *paramsStart = params, *paramsEnd = params + paramsCount; paramsStart < paramsEnd; ++paramsStart) {
		size_t length = 1;

		const _TCHAR *param = paramsStart;
		if (*(paramsStart + 1) == ':') {
			++paramsStart;

			length = 2;
		}
		paramsVector.push_back(std::_tstring(param, length));
	}
	std::vector<std::_tstring> paramsExtVector(paramsCount);

	if (paramsExt != NULL) {
		paramsExtVector = std::vector<std::_tstring>(paramsExt, paramsExt + paramsCount);
	}
	std::map<std::_tstring, std::_tstring> result;

	std::vector<_TCHAR *> removal;
	for (_TCHAR **argvStart = argv + 1, **argvEnd = argv + argc; argvStart < argvEnd; ++argvStart) {
		bool match = false;
		for (size_t j = 0; j < paramsCount; ++j) {
			std::_tstring &param    = paramsVector.at(j);
			std::_tstring &paramExt = paramsExtVector.at(j);

			_TCHAR *argument = *argvStart;

			bool extended = false;
			if (_tcsncmp(++argument, &param[0], param.length()) == 0
				|| (extended = !paramExt.empty() && _tcsncmp(++argument, &paramExt[0], paramExt.length()) == 0)) {

				if (extended) {
					param = paramExt;
				}
				result.insert(std::pair<std::_tstring, std::_tstring>(param, std::_tstring(argument + param.length())));
				match = true;

				break;
			}
		}
		if (!match && outFreeParams != NULL) {
			outFreeParams->push_back(std::_tstring(*argvStart));
		}
	}
	return result;
}

static const _TCHAR *paramsExtended[] = { _T("help"), _T("scan") };

int _tmain(int argc, _TCHAR* argv[]) 
{
	std::vector<std::_tstring> argsFree;
	std::map<std::_tstring, std::_tstring> argsBound 
		= readArgs(argc, argv, &argsFree, 2, _T("hs:"), paramsExtended);

	if (argsBound.find(_T("h")) == argsBound.end()
		&& argsBound.find(_T("help")) == argsBound.end()) {

		std::map<std::_tstring, std::_tstring>::iterator scanArguments;

		if ((scanArguments = argsBound.find(_T("s:"))) != argsBound.end()
			|| (scanArguments = argsBound.find(_T("scan"))) != argsBound.end()) {

			size_t countScanMax = 10;

			if (!scanArguments->second.empty()) {
				countScanMax = _ttol(&scanArguments->second[0]);
			}
			std::vector<std::_tstring> portsFound;

			while (countScanMax-- > 0) {
				std::_tstring port = string::format(_T("\\.\COM%d", countScanMax - 1));

				HANDLE hComm = CreateFile(
					&port[0],
					GENERIC_WRITE,
					0,
					0,
					OPEN_EXISTING,
					FILE_FLAG_OVERLAPPED,
					0
				);
				if (hComm != INVALID_HANDLE_VALUE) {
					portsFound.push_back(port);

					CloseHandle(hComm);
				}
			}
			if (!portsFound.empty()) {
				for (std::vector<std::_tstring>::iterator portsStart = portsFound.begin(), portsEnd = portsFound.end();
					portsStart != portsEnd; ++portsStart) {

					std::_tcout << *portsStart << std::endl;
				}
			}
			else {
				std::_tcout << _T("No active communication ports found.") << std::endl;
			}
			return 0;
		}
		if (argsFree.size() == 2) {
			std::_tstring port = argsFree[0];

			if (_tcsncmp(&port[0], _T("\\\\.\\COM"), 7) != 0) {
				port = string::format(_T("\\\\.\\%s"), &port[0]);
			}
			HANDLE hComm = CreateFile(
				&port[0],
				GENERIC_WRITE,
				0,
				0,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				0
			);
			if (hComm == INVALID_HANDLE_VALUE) {
				return DisplayError(_T("CreateFile"));
			}
			HANDLE hFile = CreateFile(
				&argsFree[1][0],
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				NULL
			);
			LARGE_INTEGER sizeFile;

			if (!GetFileSizeEx(hFile, &sizeFile)) {
				return DisplayError(_T("GetFileSizeEx"));
			}
			if (sizeFile.QuadPart > 100LL * 1024 * 1024) {
				std::_tcout << string::format(
					_T("It is possible to transmit files that NOT exceed 100Mb (%s is %dMb).",

					argv[2],
					sizeFile.QuadPart / 1024 / 1024)
					) << std::endl;
				return -2;
			}
			std::auto_ptr<BYTE> data(new BYTE[sizeFile.QuadPart]);
			{
				OVERLAPPED osRead = {};
				{
					osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

					if (osRead.hEvent == NULL) {
						return DisplayError(_T("CreateEvent"));
					}
				}
				DWORD bytesTransferred;

				if (!WaitIO(ReadFile(hFile, data.get(), sizeFile.LowPart, NULL, &osRead), hFile, osRead, &bytesTransferred)) {
					return DisplayError(_T("ReadFile"));
				}
				std::_tcout << string::format(_T("Bytes read: %d", bytesTransferred)) << std::endl;

				OVERLAPPED osTransmit = {};
				{
					osTransmit.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

					if (osTransmit.hEvent == NULL) {
						return DisplayError(_T("CreateEvent"));
					}
				}
				if (!WaitIO(WriteFile(hComm, data.get(), bytesTransferred, NULL, &osTransmit), hComm, osTransmit, &bytesTransferred)) {
					return DisplayError(_T("WriteFile"));
				}
				std::_tcout << string::format(_T("Bytes written: %d", bytesTransferred)) << std::endl;
				std::_tcout << string::format(_T("Done!", bytesTransferred)) << std::endl;
			}
			data.release();

			return 0;
		}
	}
	std::_tcout << _T("transmit[.exe] <port: e.g. COM3> <file binary source path>") << std::endl;
	std::_tcout << _T(" this call perfroms data transmission from the source file right to the port given.") << std::endl << std::endl;
	
	std::_tcout << _T("transmit[.exe] [-s:<port max index> | --scan]") << std::endl;
	std::_tcout << _T(" this call will return a list of currently present communication devices up to the") << std::endl;
	std::_tcout << _T(" maximum index specified, if '--scan' is given it will perfrom scan on first 10 (0..9) ports.") << std::endl << std::endl;
	
	std::_tcout << _T("transmit[.exe] [-h | --help]") << std::endl;
	std::_tcout << _T(" shows this help message.") << std::endl << std::endl;

	return 0;
}


DWORD DisplayError(LPTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	lpDisplayBuf =
		(LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf)
		+ lstrlen((LPCTSTR)lpszFunction)
		+ 40) // account for format string
		* sizeof(TCHAR));

	if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error code %d as follows:\n%s"),
		lpszFunction,
		dw,
		lpMsgBuf)))
	{
		printf("FATAL ERROR: Unable to output error code.\n");
	}

	_tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	return dw;
}
