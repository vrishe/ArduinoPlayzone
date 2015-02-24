// ASR-1388_Driver.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ASR-1388_Driver.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.

	// Register window class
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.lpfnWndProc	= WndProc;
	wcex.hInstance		= hInstance;
	wcex.lpszClassName	= TEXT_WINDOW_CLASS;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = CreateSolidBrush(COLOR_WINDOW_BACKGROUND);

	return RegisterClassEx(&wcex);
}

#define WS_MYWINDOW (WS_CAPTION | WS_SYSMENU | WS_DLGFRAME)
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   RECT windowRect;
   windowRect.left = 0;
   windowRect.top = 0;
   windowRect.right = 400;
   windowRect.bottom = 400;
   if (AdjustWindowRectEx(&windowRect, WS_MYWINDOW, FALSE, WS_EX_DLGMODALFRAME))
   {
	   windowRect.right -= windowRect.left;
	   windowRect.bottom -= windowRect.top;
   }
   windowRect.left = CW_USEDEFAULT;
   windowRect.top = 0;
 
   hWnd = CreateWindowEx(WS_EX_DLGMODALFRAME, TEXT_WINDOW_CLASS, TEXT_WINDOW_CLASS, WS_MYWINDOW,
      windowRect.left, windowRect.top, windowRect.right, windowRect.bottom, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


#define AA_RANK 2			// A power of 2.
#define AA_(dimen) \
	((dimen) * antialias)

static HBITMAP hBmpOut;
static HGDIOBJ hBmpRestore;

static RECT rcBmpOut = { 0 };

VOID DeleteOutputBitmap(HDC hDc) {
	if (!!hBmpRestore) {
		SelectObject(hDc, hBmpRestore);
	}
	if (!!hBmpOut && DeleteObject(hBmpOut)) {
		hBmpOut = NULL;
	}
}

VOID CreateOutputBitmap(HDC hDcSrc, HDC hDcDst, LONG width, LONG height, BYTE antialias) {
	DeleteOutputBitmap(hDcDst);

	rcBmpOut.right = width << antialias;
	rcBmpOut.bottom = height << antialias;
	hBmpOut = CreateCompatibleBitmap(hDcSrc, rcBmpOut.right, rcBmpOut.bottom);

	if (!!hBmpOut) {
		SelectObject(hDcDst, hBmpOut);
	}
}

VOID CommitOutputBitmap(HDC hDcSrc, HDC hDcDst, LONG dstWidth, LONG dstHeight, BYTE antialias) {
	SetStretchBltMode(hDcDst, HALFTONE);
	StretchBlt(hDcDst, 0, 0, 

		dstWidth >> antialias, 
		dstHeight >> antialias, 

		hDcSrc, 
		rcBmpOut.left, 
		rcBmpOut.top, 
		rcBmpOut.right - rcBmpOut.left, 
		rcBmpOut.bottom - rcBmpOut.top, 
		SRCCOPY
	);
}

BOOL GetClientRectAntialias(HWND hWnd, LPRECT rcClient, BYTE antialias) {
	if (GetClientRect(hWnd, rcClient)) {

		LONG *values = reinterpret_cast<LONG *>(rcClient);
		values[0] <<= antialias;
		values[1] <<= antialias;
		values[2] <<= antialias;
		values[3] <<= antialias;

		return TRUE;
	}
	return FALSE;
}


#include "Graphics/Scene.h"
#include "Graphics/Screen.h"
#include "ASRSprite.h"
#include "ASRTextScroller.h"

#include <stdint.h>

// TODO: place screen interface code here.
class TestScreen : public _2d::Screen<uint8_t, uint8_t, TestScreen, 8, 8> {

	uint8_t data[8];

public:
	TestScreen::uunit_t *TestScreen::getLine(uunit_t lineIndex) {
		return this->data + lineIndex;
	}

	virtual TestScreen TestScreen::getViewport(unit_t x, unit_t y, uunit_t w, uunit_t h) const {
		return TestScreen(*this);
	}

	virtual void flush() {
		memset(data, 0x00, sizeof(data));
	}

	void display(HDC &hDc, const RECT &client, BYTE antialias) const {
		antialias = 1 << antialias;

		LONG width = (client.right - client.left)  / getWidth();
		LONG height = (client.bottom - client.top) / getHeight();

		RECT cell;
		for (int i = 0, imax = getHeight(); i < imax; ++i) {
			cell.top = i * height;
			cell.bottom = cell.top + height;

			uint8_t line = *const_cast<TestScreen *>(this)->getLine(i);
			for (int j = 0, jmax = getWidth(); j < jmax; ++j) {
				cell.left = j * width;
				cell.right = cell.left + width;

				SetDCBrushColor(hDc, !!((line << j) & 0x80) ?
					COLOR_LED_ON : COLOR_LED_OFF);

				Ellipse(hDc, cell.left + AA_(5), cell.top + AA_(5),
					cell.right - AA_(5), cell.bottom - AA_(5));
			}
		}
	}
};

typedef _2d::Scene<uint8_t, uint8_t> TestScene;


static TestScreen screen;
static TestScene  scene;

//static asr::Sprite smiley(8, 8);
//
//static void initScene() {
//
//	uint8_t *spriteData;
//
//	spriteData = smiley.getValueAt(0);
//	spriteData[0] = 0x00;
//	spriteData[1] = 0x24;
//	spriteData[2] = 0x24;
//	spriteData[3] = 0x24;
//	spriteData[4] = 0x00;
//	spriteData[5] = 0x66;
//	spriteData[6] = 0x3c;
//	spriteData[7] = 0x00;
//
//	scene.add(smiley);
//}

static unsigned long ticks;
static asr::TextScroller textScroller(&screen);
static asr::Sprite *font[20] = { /* zero */ };
static char fontIndex[10];
static size_t currentCodePoint = 0;

static void initTextScroller() {
	uint8_t *spriteData;

	if (!!(font[0] = new (std::nothrow) asr::Sprite(5, 5))) { // D
		spriteData = font[0]->getValueAt(0);

		spriteData[0] = 0xe0;
		spriteData[1] = 0x90;
		spriteData[2] = 0x90;
		spriteData[3] = 0x90;
		spriteData[4] = 0xe0;
	}
	if (!!(font[1] = new (std::nothrow) asr::Sprite(5, 5))) { // E
		spriteData = font[1]->getValueAt(0);

		spriteData[0] = 0xf0;
		spriteData[1] = 0x80;
		spriteData[2] = 0xe0;
		spriteData[3] = 0x80;
		spriteData[4] = 0xf0;
	}
	if (!!(font[4] = new (std::nothrow) asr::Sprite(5, 5))) { // H
		spriteData = font[4]->getValueAt(0);

		spriteData[0] = 0x90;
		spriteData[1] = 0x90;
		spriteData[2] = 0xf0;
		spriteData[3] = 0x90;
		spriteData[4] = 0x90;
	}
	if (!!(font[8] = new (std::nothrow) asr::Sprite(5, 5))) { // L
		spriteData = font[8]->getValueAt(0);

		spriteData[0] = 0x80;
		spriteData[1] = 0x80;
		spriteData[2] = 0x80;
		spriteData[3] = 0x80;
		spriteData[4] = 0xf0;
	}
	if (!!(font[11] = new (std::nothrow) asr::Sprite(5, 5))) { // O
		spriteData = font[11]->getValueAt(0);

		spriteData[0] = 0x60;
		spriteData[1] = 0x90;
		spriteData[2] = 0x90;
		spriteData[3] = 0x90;
		spriteData[4] = 0x60;
	}
	if (!!(font[14] = new (std::nothrow) asr::Sprite(5, 5))) { // R
		spriteData = font[14]->getValueAt(0);

		spriteData[0] = 0xe0;
		spriteData[1] = 0x90;
		spriteData[2] = 0xe0;
		spriteData[3] = 0x90;
		spriteData[4] = 0x90;
	}
	if (!!(font[19] = new (std::nothrow) asr::Sprite(6, 5))) { // W
		spriteData = font[19]->getValueAt(0);
		
		spriteData[0] = 0x88;
		spriteData[1] = 0x88;
		spriteData[2] = 0xa8;
		spriteData[3] = 0xa8;
		spriteData[4] = 0x50;
	}
	strcpy(fontIndex, "HELLOWORLD");

	const char text[] = "HELLO WORLD";
	textScroller.setText(text, _countof(text), 1, 5, font, _countof(font), 'D', asr::LeftToRight);
	textScroller.placeAtRow(2);
}


static void RenderWorld(HWND hWnd) {
	screen.flush();
	scene.renderTo(screen);

	if (!!hWnd) {
		InvalidateRect(hWnd, NULL, FALSE);
	}
}


BOOL WriteABuffer(HANDLE hComm, char * lpBuf, DWORD dwToWrite)
{
	OVERLAPPED osWrite = { 0 };
	DWORD dwWritten;
	DWORD dwRes;
	BOOL fRes;

	// Create this write operation's OVERLAPPED structure's hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (osWrite.hEvent == NULL)
		// error creating overlapped event handle
		return FALSE;

	// Issue write.
	if (!WriteFile(hComm, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			fRes = FALSE;
		} else {
			// Write is pending.
			dwRes = WaitForSingleObject(osWrite.hEvent, INFINITE);
		}
		switch (dwRes)
		{
			// OVERLAPPED structure's event has been signaled. 
		case WAIT_OBJECT_0:
			if (!GetOverlappedResult(hComm, &osWrite, &dwWritten, FALSE))
				fRes = FALSE;
			else
				// Write operation completed successfully.
				fRes = TRUE;
			break;

		default:
			// An error has occurred in WaitForSingleObject.
			// This usually indicates a problem with the
			// OVERLAPPED structure's event handle.
			fRes = FALSE;
			break;
		}
	} else {
		// WriteFile completed immediately.
		fRes = TRUE;
	}
	CloseHandle(osWrite.hEvent);
	return fRes;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hDc;
	static HANDLE hCom;

	switch (message)
	{
	case WM_CREATE:
	{
		hDc = CreateCompatibleDC(GetDC(NULL));
		hCom = CreateFile(_T("COM4"),
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			0
		);
		//if (hCom == INVALID_HANDLE_VALUE) {
		//	DestroyWindow(hWnd);

		//	return -1;
		//}
		//uint8_t *spriteData;

		//spriteData = smiley.getValueAt(0); // 0, 36, 36, 36, 0, 102, 60, 0
		//spriteData[0] = 0x00;
		//spriteData[1] = 0x24;
		//spriteData[2] = 0x24;
		//spriteData[3] = 0x24;
		//spriteData[4] = 0x00;
		//spriteData[5] = 0x66;
		//spriteData[6] = 0x3c;
		//spriteData[7] = 0x00;

		initTextScroller();

		RenderWorld(NULL);
		break;
	}
	case WM_SIZE:
		CreateOutputBitmap(GetDC(NULL), hDc, LOWORD(lParam), HIWORD(lParam), AA_RANK);
		break;
	case WM_PAINT:
	{
		RECT client;
		PAINTSTRUCT ps;
	
		HDC hDcWindow = BeginPaint(hWnd, &ps);

		HBRUSH dcBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		if (!!dcBrush && GetClientRectAntialias(hWnd, &client, AA_RANK)) {
			SelectObject(hDc, dcBrush);

			screen.display(hDc, client, AA_RANK);
			CommitOutputBitmap(hDc, hDcWindow, 
				client.right - client.left, 
				client.bottom - client.top,
				AA_RANK
			);
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
	//	case VK_LEFT:
	//		WriteABuffer(hCom, "l", 1);
	//		smiley.moveBy(-1, 0);
	//		RenderWorld(hWnd);
	//		break;

	//	case VK_RIGHT:
	//		WriteABuffer(hCom, "r", 1);
	//		smiley.moveBy(1, 0);
	//		RenderWorld(hWnd);
	//		break;

	//	case VK_UP:
	//		WriteABuffer(hCom, "u", 1);
	//		smiley.moveBy(0, -1);
	//		RenderWorld(hWnd);
	//		break;

	//	case VK_DOWN:
	//		WriteABuffer(hCom, "d", 1);
	//		smiley.moveBy(0, 1);
	//		RenderWorld(hWnd);
	//		break;
		case VK_SPACE:
			//scene.clear();
			//scene.add(*font[fontIndex[currentCodePoint] - 'D']);
			//currentCodePoint = (currentCodePoint + 1) % _countof(fontIndex);
			textScroller.updateText(ticks += 127);

			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		break;
	case WM_DESTROY:
		DeleteOutputBitmap(hDc);
		DeleteDC(hDc);

		if (hCom != INVALID_HANDLE_VALUE) {
			CloseHandle(hCom);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

