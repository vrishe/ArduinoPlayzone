// ASR-1388_Driver.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ASR-1388_Driver.h"

static BYTE screen[8] = { 0, 36, 36, 36, 0, 102, 60, 0 };

// TODO: place screen interface code here.


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


#define AA_RANK 3

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

	switch (message)
	{
	case WM_CREATE:
		hDc = CreateCompatibleDC(GetDC(NULL));
		break;
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

			LONG width  = (client.right - client.left) / (sizeof(*screen) << 3);
			LONG height = (client.bottom - client.top) / _countof(screen);

			RECT cell;
			for (int i = 0; i < _countof(screen); ++i) {
				cell.top = i * height;
				cell.bottom = cell.top + height;

				BYTE line = screen[i];
				for (int j = 0; j < (sizeof(*screen) << 3); ++j) {
					cell.left = j * width;
					cell.right = cell.left + width;

					SetDCBrushColor(hDc, !!((line << j) & 0x80) ? 
						COLOR_LED_ON : COLOR_LED_OFF);

					Ellipse(hDc, cell.left + 40, cell.top + 40,
						cell.right - 40, cell.bottom - 40);
				}
			}
			CommitOutputBitmap(hDc, hDcWindow, 
				client.right - client.left, 
				client.bottom - client.top,
				AA_RANK
			);
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		DeleteOutputBitmap(hDc);
		DeleteDC(hDc);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

