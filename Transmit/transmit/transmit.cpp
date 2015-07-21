// transmit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#if defined(UNICODE) || defined(_UNICODE)
#	define _tcout			wcout
#	define _tstringstream   wstringstream
#	define _tistringstream  wistringstream
#else
#	define _tcout			cout
#	define _tstringstream   stringstream
#	define _tstringstream   istringstream
#endif


BOOL WaitIO(BOOL result, HANDLE hFile, OVERLAPPED &overlapped, LPDWORD bytesTransferred, BOOL cleanUp = TRUE, DWORD timeout = INFINITE);


// Routine Description:
// Retrieve and output the system error message for the last-error code
DWORD DisplayError(LPTSTR lpszFunction);

std::map<std::_tstring, std::_tstring> readArgs(int argc, _TCHAR *argv[], std::vector<std::_tstring> *outFreeParams,
	size_t paramsCount, const _TCHAR *params, const _TCHAR *const paramsExt[]);


static const _TCHAR *paramsExtended[] = { _T("help"), _T("cts"), _T("scan"), _T("baud="), _T("dps=") };

int _tmain(int argc, _TCHAR* argv[]) 
{
	std::vector<std::_tstring> argsFree;
	std::map<std::_tstring, std::_tstring> argsBound 
		= readArgs(argc, argv, &argsFree, 4, _T("hcs:b:d:"), paramsExtended);

	// This is NOT a 'help' query block.
	if (argsBound.find(_T("h")) == argsBound.end()
		&& argsBound.find(_T("help")) == argsBound.end()) {

		std::map<std::_tstring, std::_tstring>::iterator arguments;

		// Check, whether we are told to perform COM port scanning.
		if ((arguments = argsBound.find(_T("s:"))) != argsBound.end()
			|| (arguments = argsBound.find(_T("scan"))) != argsBound.end()) {

			size_t countScanMax = 10;

			if (!arguments->second.empty()) {
				size_t countScanMaxNew = _ttol(&arguments->second[0]);

				if (countScanMaxNew > 0) {
					countScanMax = countScanMaxNew;
				}
			}
			std::vector<std::_tstring> portsFound;

			while (countScanMax-- > 0) {
				std::_tstring port = string::format(_T("\\\\.\\COM%d"), countScanMax - 1);

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
		// We've got COM port and File path specified. Let's do data transfer then.
		if (argsFree.size() == 2) {
			std::_tstring port = argsFree[0];

			if (_tcsncmp(&port[0], _T("\\\\.\\COM"), 7) != 0) {
				port = string::format(_T("\\\\.\\%s"), &port[0]);
			}
			HANDLE hComm = CreateFile(
				&port[0],
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				NULL
			);
			if (hComm == INVALID_HANDLE_VALUE) {
				return DisplayError(_T("CreateFile"));
			}
			DCB portDcb = {};
			{
				portDcb.DCBlength = sizeof(portDcb);

				DWORD baud = 9600;

				if ((arguments = argsBound.find(_T("b:"))) != argsBound.end()
					|| (arguments = argsBound.find(_T("baud="))) != argsBound.end()) {

					size_t baudNew = _ttol(&arguments->second[0]);

					if (baudNew > 0) {
						baud = baudNew;
					}
				}
				BYTE			data	= 8;
				std::_tstring	parity	= _T("N");
				std::_tstring	stop	= _T("1");
				bool			octs = (arguments = argsBound.find(_T("c"))) != argsBound.end()
										|| (arguments = argsBound.find(_T("cts"))) != argsBound.end();
				
				if ((arguments = argsBound.find(_T("d:"))) != argsBound.end()
					|| (arguments = argsBound.find(_T("dps="))) != argsBound.end()) {
					
					std::_tistringstream iss(arguments->second);

					std::_tstring value;
					if (std::getline(iss, value, _T(';'))) {
						int dataNew = _ttoi(&value[0]);

						if (dataNew > 0) {
							data = (BYTE)dataNew;
						}
					}
					if (std::getline(iss, value, _T(';'))) {
						parity = value;
					}
					if (std::getline(iss, value, _T(';'))) {
						stop = value;
					}
				}
				if (!GetCommState(hComm, &portDcb)) {
					return DisplayError(_T("GetCommState"));
				}
				if (!BuildCommDCB(&::string::format(_T("baud=%d data=%d parity=%s stop=%s octs=%s"), 
					baud, data, &parity[0], &stop[0], octs ? _T("on") : _T("off"))[0], &portDcb)) {

					return DisplayError(_T("BuildCommDCB"));
				}
				portDcb.fAbortOnError = TRUE;
			}
			if (!SetCommState(hComm, &portDcb)) {
				return DisplayError(_T("SetCommState"));
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
					_T("It is possible to transmit files that NOT exceed 100Mb (%s is %dMb)."),

					argv[2],
					sizeFile.QuadPart / 1024 / 1024
				) << std::endl;

				return -2;
			}
			std::vector<BYTE> data(static_cast<size_t>(sizeFile.QuadPart));
			{
				OVERLAPPED osRead = {};
				{
					osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

					if (osRead.hEvent == NULL) {
						return DisplayError(_T("CreateEvent"));
					}
				}
				DWORD bytesTransferred;

				if (!WaitIO(ReadFile(hFile, &data[0], sizeFile.LowPart, NULL, &osRead), 
					hFile, osRead, &bytesTransferred)) {

					return DisplayError(_T("ReadFile"));
				}
				std::_tcout << _T("Source file of data found... ");
				std::_tcout << string::format(_T("Bytes read: %d"), bytesTransferred) << std::endl;

				OVERLAPPED osTransmit = {};
				{
					osTransmit.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

					if (osTransmit.hEvent == NULL) {
						return DisplayError(_T("CreateEvent"));
					}
				}
				if (portDcb.fOutxCtsFlow) {
					DWORD commStatus;

					if (!GetCommModemStatus(hComm, &commStatus)) {
						return DisplayError(_T("GetCommModemStatus"));
					}
					if ((commStatus & MS_CTS_ON) == 0) {
						DWORD commMask;
						if (!(GetCommMask(hComm, &commMask) 
							&& SetCommMask(hComm, commMask | EV_CTS))) {
							return DisplayError(_T("SetCommMask"));
						}
						std::_tcout << _T("Waiting for CTS signal... ");

						DWORD ignore;
						if (!WaitIO(WaitCommEvent(hComm, &commMask, &osTransmit), hComm, osTransmit, &ignore, FALSE)) {
							return DisplayError(_T("WaitCommEvent"));
						}
						ResetEvent(osTransmit.hEvent);
					}
				}
				std::_tcout << _T("Transferring data... ");

				if (!WaitIO(WriteFile(hComm, &data[0], bytesTransferred, &bytesTransferred, &osTransmit),
					hComm, osTransmit, &bytesTransferred)) {

					return DisplayError(_T("WriteFile"));
				}
				std::_tcout << string::format(_T("Bytes written: %d"), bytesTransferred) << std::endl;
				std::_tcout << _T("Done!") << std::endl;
			}
			return 0;
		}
	}
	std::_tcout << _T("transmit[.exe] <port: e.g. COM3> <file binary source path> [-b, -d, -c]") << std::endl;
	std::_tcout << _T(" this call perfroms data transmission from the source file to the port given.") << std::endl << std::endl;
	std::_tcout << _T(" Additional parameters are:") << std::endl;
	std::_tcout << _T("  -b:<baudrate> | --baud=<baudrate>  - specify the baudrate to perform data") << std::endl;
	std::_tcout << _T("                                       transfer at (e.g.: 76500 or 115200);") << std::endl << std::endl;
	std::_tcout << _T("  -d:D;P;S | --dps=D;P;S             - specify transmission config in") << std::endl;
	std::_tcout << _T("                                       D/P/S format (e.g.: 8;N;1 or 7;E;1.5).") << std::endl << std::endl;
	std::_tcout << _T("  -d | --cts                         - enable CTS assertion before") << std::endl;
	std::_tcout << _T("                                       transmission.") << std::endl << std::endl;

	std::_tcout << _T("transmit[.exe] [-s:<port max index> | --scan(=10max)]") << std::endl;
	std::_tcout << _T(" this call will return a list of available communication ports up to the") << std::endl;
	std::_tcout << _T(" maximum index specified, if '--scan' is given it will perfrom scan") << std::endl;
	std::_tcout << _T(" on first 10 (0..9) ports.") << std::endl << std::endl;
	
	std::_tcout << _T("transmit[.exe] [-h | --help]") << std::endl;
	std::_tcout << _T(" shows this help message.") << std::endl << std::endl;

	return 0;
}


BOOL WaitIO(BOOL result, HANDLE hFile, OVERLAPPED &overlapped, LPDWORD bytesTransferred, BOOL cleanUp, DWORD timeout) {
	if (!result) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			result = FALSE;
		}
		else {
			// Write is pending.
			DWORD dwRes = WaitForSingleObject(overlapped.hEvent, timeout);

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
	if (cleanUp) {
		if (overlapped.hEvent != NULL && overlapped.hEvent != INVALID_HANDLE_VALUE) {
			CloseHandle(overlapped.hEvent);
		}
		CloseHandle(hFile);
	}
	return result;
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

std::map<std::_tstring, std::_tstring> readArgs(int argc, _TCHAR *argv[], std::vector<std::_tstring> *outFreeParams,
	size_t paramsCount, const _TCHAR *params, const _TCHAR *const paramsExt[]) {

	std::vector<std::_tstring> paramsVector;

	for (const _TCHAR *paramsStart = params, *paramsEnd = params + _tcslen(params); 
			paramsVector.size() < paramsCount && paramsStart < paramsEnd; ++paramsStart) {

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
			std::_tstring &param	= paramsVector.at(j);
			std::_tstring &paramExt = paramsExtVector.at(j);

			_TCHAR *argument = *argvStart;
			_TCHAR trailer = _T(':');

			bool extended = false;
			if (_tcsncmp(++argument, &param[0], param.length()) == 0
				|| (extended = !paramExt.empty() && _tcsncmp(++argument, &paramExt[0], paramExt.length()) == 0)) {

				if (extended) {
					param = paramExt;

					trailer = _T('=');
				}
				match = true;

				if (param[param.length() - 1] == trailer 
						|| _tcslen(argument + param.length()) == 0) {
					result.insert(std::pair<std::_tstring, std::_tstring>(param, std::_tstring(argument + param.length())));
				}
				break;
			}
		}
		if (!match && outFreeParams != NULL) {
			outFreeParams->push_back(std::_tstring(*argvStart));
		}
	}
	return result;
}
