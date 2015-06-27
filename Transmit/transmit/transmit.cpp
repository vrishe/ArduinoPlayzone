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

int _tmain(int argc, _TCHAR* argv[]) 
{
	if (argc == 3) {
		HANDLE hComm = CreateFile(
			argv[1],
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
			argv[2],
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
	}
	else {
		std::_tcout << _T("transmit[.exe] <port: e.g. COM3> <file binary source path>") << std::endl;

		return -1;
	}
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
