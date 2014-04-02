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
		AddEventHandler(WM_TIMER, &Me::OnTimer);
		AddEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
		AddEventHandler(WM_KEYDOWN, &Me::OnKeyDown);
		AddEventHandler(WM_ERASEBKGND, &Me::OnEraseBkgnd);
	}
	LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Invalidate(FALSE);
		return 0;
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, _T("bg_small.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);


		BITMAP bm;
		::GetObject(hBitmap, sizeof(BITMAP), &bm);
		cBitmap.cx = bm.bmWidth;
		cBitmap.cy = bm.bmHeight;

		bgColor = ::CreateSolidBrush(RGB(255,0,255));
		
		::SetTimer(hWnd, 0, 10, NULL);

		Rect rc;
		::GetClientRect(hWnd, &rc);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
		::SelectObject(hMemDC, hMemBitmap);

		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		// delete loaded bitmap handle.
		::DeleteObject(bgColor);
		::DeleteObject(hBitmap);
		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);


		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		::SelectObject(hBitmapDC, hBitmap);

		::FillRect(hMemDC, &rc, bgColor);

		Rect imgRect(0, 0, cBitmap.cx, cBitmap.cy);

		//for (int i = 0; i < rc.bottom; i+= cBitmap.cy)
		//{
		//	for (int j = 0; j < rc.right; j+= cBitmap.cx)
		//	{
		////for (int i = 0; i < 10; i++)
		////{
		////	for (int j = 0; j < 5; j++)
		////	{
		//		//Rect dstRect = imgRect >> Size(cBitmap.cx*j, cBitmap.cy*i);
		//		Rect dstRect = imgRect;
		//		::OffsetRect(&dstRect, cBitmap.cx*j, cBitmap.cy*i);

		//		::BitBlt(hdc, j, i,
		//		//::BitBlt(hdc, dstRect.left, dstRect.top,
		//			cBitmap.cx, cBitmap.cy,
		//			hMemDC, 0, 0, SRCCOPY);
		//	}
		//}

		//::BitBlt(hdc, j, i,
		Rect dstRect = imgRect >> Size(0,0);
		::BitBlt(hMemDC, dstRect.left, dstRect.top,
			cBitmap.cx, cBitmap.cy,
			hBitmapDC, 0, 0, SRCCOPY);
		dstRect = imgRect;
		::OffsetRect(&dstRect, cBitmap.cx, 0);
		::BitBlt(hMemDC, dstRect.left, dstRect.top,
			cBitmap.cx, cBitmap.cy,
			hBitmapDC, 0, 0, SRCPAINT);
		dstRect = imgRect;
		::OffsetRect(&dstRect, cBitmap.cx*2, 0);
		::BitBlt(hMemDC, dstRect.left, dstRect.top,
			cBitmap.cx, cBitmap.cy,
			hBitmapDC, 0, 0, SRCAND);
		dstRect = imgRect;
		::OffsetRect(&dstRect, 0, cBitmap.cy);
		::BitBlt(hMemDC, dstRect.left, dstRect.top,
			cBitmap.cx, cBitmap.cy,
			hBitmapDC, 0, 0, SRCINVERT);
		dstRect = imgRect;
		::OffsetRect(&dstRect, cBitmap.cx, cBitmap.cy);
		::BitBlt(hMemDC, dstRect.left, dstRect.top,
			cBitmap.cx, cBitmap.cy,
			hBitmapDC, 0, 0, SRCERASE);

		::DeleteDC(hBitmapDC);

		::BitBlt(hMainDC, 0, 0, rc.width(), rc.height(),
			hMemDC, 0, 0, SRCCOPY);

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
	HBITMAP hMemBitmap;
	HDC hMemDC;
	HDC hMainDC;
	Point ptMouse;
	Size cBitmap;
	bool bMouseClipped;
	HBRUSH bgColor;
};