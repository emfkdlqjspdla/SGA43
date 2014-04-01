#pragma once

#include <windowsx.h>
#include "..\MainWindow\MainWindow.h"

class CannonApp : public MainWindow<CannonApp>
{
	typedef CannonApp Me;
	typedef MainWindow<CannonApp> Base;
public :
	CannonApp()
		: bMouseClipped(false)
	{
	}
protected :
	void SetEventMap()
	{
		// for WM_DESTROY event handle
		// if you want to redefine WM_DESTROY event handler,
		// do not call Base::SetEventMap(), and then call AddEventHandler(WM_DESTROY, &Me::SomeHandler);
		// also, SomeHandler member function must be declared.
		//Base::SetEventMap();

		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		AddEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
		AddEventHandler(WM_KEYDOWN, &Me::OnKeyDown);
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, _T("circle.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		// get information of loaded bitmap file.
		//typedef struct tagBITMAP
		//{
		//	LONG        bmType;			// bitmap must be 0.
		//	LONG        bmWidth;		// width of bitmap file in pixel unit.
		//	LONG        bmHeight;		// height of bitmap file in pixel unit.
		//	LONG        bmWidthBytes;	// amount of read which 1 scan. must be divided by 2. (bmBitsPixel/8)*bmWidth
		//	WORD        bmPlanes;		// color plane.
		//	WORD        bmBitsPixel;	// color bit per 1 pixel.
		//	LPVOID      bmBits;			// real bit map data pointer.
		//} BITMAP, *PBITMAP, NEAR *NPBITMAP, FAR *LPBITMAP;

		BITMAP bm;
		::GetObject(hBitmap, sizeof(BITMAP), &bm);
		cBitmap.cx = bm.bmWidth;
		cBitmap.cy = bm.bmHeight;

		
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
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		HDC hMemDC = ::CreateCompatibleDC(hdc);
		::SelectObject(hMemDC, hBitmap);

		::BitBlt(hdc, ptMouse.x - cBitmap.cx/2, ptMouse.y - cBitmap.cy/2, cBitmap.cx, cBitmap.cy,
			hMemDC, 0, 0, SRCCOPY);


		::DeleteDC(hMemDC);

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
	LRESULT OnKeyDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if ((::GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0x8000)
		{
			ClipCursor(hWnd);
		}

		return 0;
	}
	bool ClipCursor(HWND hWnd)
	{
		if (bMouseClipped)
		{
			// free mouse.
			::ClipCursor(NULL);
		}
		else
		{
			Rect rc;
			::GetClientRect(hWnd, &rc);
			Point lt = rc.lefttop();
			Point rb = rc.rightbottom();
			::ClientToScreen(hWnd, &lt);
			::ClientToScreen(hWnd, &rb);

			rc = Rect(lt, rb);

			// confine cursor in client area.
			::ClipCursor(&rc);
		}

		bMouseClipped = !bMouseClipped;

		return bMouseClipped;
	}

private :
	HBITMAP hBitmap;
	POINT ptMouse;
	SIZE cBitmap;
	bool bMouseClipped;
};