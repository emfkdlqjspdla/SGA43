#pragma once

#include <windowsx.h>
#include "..\MainWindow\MainWindow.h"
#include <string>
#include <sstream> // for ostringstream

#if defined(UNICODE) || defined(_UNICODE)
#define tstring std::wstring
#define tostringstream std::wostringstream
#else
#define tstring std::string
#define tostringstream std::ostringstream
#endif // defined(UNICODE) || defined(_UNICODE)

class CannonApp : public MainWindow<CannonApp>
{
	typedef CannonApp Me;
	typedef MainWindow<CannonApp> Base;
public :
protected :
	void SetEventMap()
	{
		// for WM_DESTROY event handle
		// if you want to redefine WM_DESTROY event handler,
		// do not call Base::SetEventMap(), and then call AddEventHandler(WM_DESTROY, &Me::SomeHandler);
		// also, SomeHandler member function must have declared.
		//Base::SetEventMap();

		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		AddEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, _T("circle1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		::GetObject(hBitmap, sizeof(BITMAP), &bm);
    //LONG        bmType; // must be 0.
    //LONG        bmWidth;
    //LONG        bmHeight;
    //LONG        bmWidthBytes;
    //WORD        bmPlanes;
    //WORD        bmBitsPixel;
    //LPVOID      bmBits;

		tostringstream oss;

		oss << _T("type   : ") << bm.bmType << _T("\r\n");
		oss << _T("width  : ") << bm.bmWidth << _T("\r\n");
		oss << _T("height : ") << bm.bmHeight << _T("\r\n");
		oss << _T("widthbytes : ") << bm.bmWidthBytes << _T("\r\n");
		oss << _T("plane  : ") << bm.bmPlanes << _T("\r\n");
		oss << _T("bitspixel : ") << bm.bmBitsPixel << _T("\r\n");

		bmpinfo = oss.str();

		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// delete loaded bitmap handle.
		::DeleteObject(hBitmap);
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		////////////////////////////////////////
		// draw bitmap information start.
		DrawText(hdc, bmpinfo.c_str(), -1, &rc, DT_TOP);
		// draw bitmap information end.
		///////////////////////////////////////

		////////////////////////////////////////
		// draw bitmap start.
		HDC hMemDC = ::CreateCompatibleDC(hdc);
		::SelectObject(hMemDC, hBitmap);

		::BitBlt(hdc, ptMouse.x, ptMouse.y, rc.right-rc.left, rc.bottom-rc.top,
			hMemDC, 0, 0, SRCCOPY);

		::DeleteDC(hMemDC);
		// draw bitmap end.
		/////////////////////////////////////////

		::EndPaint(hWnd, &ps);

		return 0;
	}
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		Invalidate();

		return 0;
	}

private :
	HBITMAP hBitmap;
	POINT ptMouse;
	BITMAP bm;
	tstring bmpinfo;
};