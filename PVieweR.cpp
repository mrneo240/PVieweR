/*
Copyright 2018 NeoDC/HaydenK.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stdafx.h"
#include "PVieweR.h"
extern "C" {
#include "pvrspi.h"
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//LRESULT CALLBACK PanelProc(HWND, UINT, WPARAM, LPARAM);
//void RegisterRedPanelClass(void);

BOOL ResizeWindow(int cx, int cy);

HWND hWnd;
LPCWSTR imagePath;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) 
{
	MSG  msg;
	WNDCLASSW wc = { 0 };
	wc.lpszClassName = L"PVieweR";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL || argCount < 2)
	{
		MessageBoxA(NULL,"Unable to parse command line\nor No .PVR supplied\n\nDrag and Drop a PVR onto this EXE", "Error", MB_OK);
		PostQuitMessage(-1);
	} else {
		imagePath = szArgList[1];
	}

	RegisterClassW(&wc);
#if NO_FRAME
	hWnd = CreateWindowExW(WS_EX_LAYERED, wc.lpszClassName, NULL, WS_DLGFRAME | WS_VISIBLE,
		100, 100, 512, 512, NULL, NULL, hInstance, NULL);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)&~0xC40000);
	//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
#else
	hWnd = CreateWindowExW(WS_EX_LAYERED,wc.lpszClassName, L"PVieweR", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 512, 512, NULL, NULL, hInstance, NULL);
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE)&~WS_SIZEBOX);
#endif

	SetLayeredWindowAttributes(hWnd, RGB(255, 0, 255), 0, LWA_COLORKEY);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

BOOL isBitmapJustLoaded = false;

void showAbout() {
	MessageBoxA(hWnd, "Copyright NeoDC aka Hayden K. 2018", "About", MB_OK);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {

	static HBITMAP hBitmap;
	HDC hdc;
	PAINTSTRUCT ps;
	BITMAP bitmap;
	HDC hdcMem;
	HGDIOBJ oldBitmap;
	char *buffer = NULL;
	LRESULT hit = NULL;

	switch (msg) {

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
				SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		else if (wParam == VK_SPACE) {
			showAbout();
		}

	case WM_CREATE:
		pvr2bmpfile(imagePath, L"out.bmp");
		hBitmap = (HBITMAP)LoadImageW(NULL, L"out.bmp"/*imagePath*/, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap == NULL) {
			MessageBoxA(hwnd, "Failed to load image", "Error", MB_OK);
			PostQuitMessage(-1);
		}
		//fake and gay
		/*RegisterRedPanelClass();

		CreateWindowW(L"RedPanelClass", NULL,WS_CHILD | WS_VISIBLE,
			20, 20, 80, 80,	hwnd, (HMENU)1, NULL, NULL);
		break;*/

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, hBitmap);

		GetObject(hBitmap, sizeof(bitmap), &bitmap);
		if (!isBitmapJustLoaded) {
			if (ResizeWindow(bitmap.bmWidth, bitmap.bmHeight)) {
				isBitmapJustLoaded = true;
			}
		}
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
			hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
		break;

	case WM_NCHITTEST:
		hit = DefWindowProc(hWnd, msg, wParam, lParam);
		if (hit == HTCLIENT) hit = HTCAPTION;
		return hit;
		break;

	case WM_RBUTTONUP:
		showAbout();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

BOOL ResizeWindow(int cx, int cy)
{
	if (hWnd == NULL)
		return FALSE;

	RECT wrect;
	wrect.left = 0;
	wrect.top = 0;
#if NO_FRAME
	wrect.right = cx;
	wrect.bottom = cy;
#else 
	wrect.right = cx + 16;
	wrect.bottom = cy + 39;
#endif
	SetWindowPos(hWnd, NULL, 0, 0, wrect.right, wrect.bottom, 
		SWP_NOMOVE | SWP_NOREPOSITION);
	return TRUE;
}

/*LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_LBUTTONUP:
		MessageBeep(MB_OK);
		break;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}


void RegisterRedPanelClass(void) {
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
	WNDCLASSW rwc = { 0 };

	rwc.lpszClassName = L"RedPanelClass";
	rwc.hbrBackground = hbrush;
	rwc.lpfnWndProc = PanelProc;
	rwc.hCursor = LoadCursor(0, IDC_ARROW);
	RegisterClassW(&rwc);
}*/
