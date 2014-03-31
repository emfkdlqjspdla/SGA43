#pragma once

#include "..\MainWindow\MainWindow.h"

class CannonApp : public MainWindow<CannonApp>
{
	typedef CannonApp Me;
	typedef MainWindow<CannonApp> Base;
public :
protected :
	void SetEventMap()
	{
		Base::SetEventMap();

		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, _T("circle.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

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
private :
	HBITMAP hBitmap;
};