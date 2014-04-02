#pragma once

#include "..\MainWindow\MainWindow.h"

class AntiBlinkApp : public MainWindow<AntiBlinkApp>
{
	typedef AntiBlinkApp Me;
	typedef MainWindow<AntiBlinkApp> Base;
public :
	AntiBlinkApp()
	{
		SetWindowTitle(_T("Anti Blink Sample"));
	}

protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		AddEventHandler(WM_TIMER, &Me::OnTimer);
		AddEventHandler(WM_ERASEBKGND, &Me::OnEraseBkgnd);
	}
	LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}

	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::GetClientRect(hWnd, &rcClient);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rcClient.width(), rcClient.height());
		::SelectObject(hMemDC, hMemBitmap);

		hBgBrush = ::CreateSolidBrush(RGB(0xE0,0xFF,0xFF));

		::SetTimer(hWnd, 0, 100, NULL);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::DeleteObject(hBgBrush);

		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::FillRect(hMemDC, &rcClient, hBgBrush);

		::DrawText(hMemDC, _T("Hello World"), -1, &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		::BitBlt(hMainDC, 0, 0, rcClient.width(), rcClient.height(), hMemDC, 0, 0, SRCCOPY);

		return 0;
	}
	LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Invalidate();

		return 0;
	}

private :
	////////////////////////////////
	// for anti blink
	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	Rect rcClient;
	HBRUSH hBgBrush;
};
