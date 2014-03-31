#pragma once

#include <windowsx.h>
#include "..\MainWindow\MainWindow.h"

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
		hBitmap = (HBITMAP)::LoadImage(NULL, _T("circle.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
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

		::BitBlt(hdc, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
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

private :
	HBITMAP hBitmap;
	POINT ptMouse;
};