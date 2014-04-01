#pragma once

#include <windowsx.h>
#include "..\MainWindow\MainWindow.h"

class BitmapApp : public MainWindow<BitmapApp>
{
	typedef BitmapApp Me;
	typedef MainWindow<BitmapApp> Base;
public :
	BitmapApp()
		: bMouseClipped(false), ptMouse(300,300)
	{
	}
protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		//AddEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
		//AddEventHandler(WM_KEYDOWN, &Me::OnKeyDown);
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, _T("bg_small.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);


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

		::BitBlt(hdc, 0, 0,	cBitmap.cx, cBitmap.cy,
			hMemDC, 0, 0, SRCCOPY);


		::DeleteDC(hMemDC);

		::EndPaint(hWnd, &ps);

		return 0;
	}
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (bMouseClipped)
		{
			ptMouse.x = GET_X_LPARAM(lParam);
			ptMouse.y = GET_Y_LPARAM(lParam);

			Invalidate();
		}

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
			::ShowCursor(TRUE);
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
			::ShowCursor(FALSE);
		}

		bMouseClipped = !bMouseClipped;

		return bMouseClipped;
	}

private :
	HBITMAP hBitmap;
	Point ptMouse;
	SIZE cBitmap;
	bool bMouseClipped;
};