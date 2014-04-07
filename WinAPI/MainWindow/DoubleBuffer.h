#pragma once

#include <windows.h>
#include "BaseType.h"

class DoubleBuffer
{
	typedef DoubleBuffer Me;
public :
	DoubleBuffer()
		: hMainWnd(NULL), hMainDC(NULL), hMemDC(NULL), hMemBitmap(NULL)
	{
	}
	~DoubleBuffer()
	{
		Detach();
	}

	void Attach(HWND hWnd)
	{
		if (hWnd == NULL) return;

		if (hMainWnd)
			Detach();

		hMainWnd = hWnd;

		::GetClientRect(hMainWnd, &rcClient);

		hMainDC = ::GetDC(hMainWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rcClient.width(), rcClient.height());
		::SelectObject(hMemDC, hMemBitmap);
	}
	void Detach()
	{
		if (hMemBitmap)
			::DeleteObject(hMemBitmap);
		if (hMemDC)
			::DeleteDC(hMemDC);
		if (hMainDC)
			::ReleaseDC(hMainWnd, hMainDC);
	}

	void Draw()
	{
		::BitBlt(hMainDC, rcClient.left, rcClient.top,
			rcClient.width(), rcClient.height(),
			hMemDC, 0, 0,
			SRCCOPY);
	}

	Me& operator << (const COLORREF& clr)
	{
		::SetDCBrushColor(hMemDC, clr);
		::FillRect(hMemDC, &rcClient, (HBRUSH)::GetStockObject(DC_BRUSH));

		return *this;
	}

	operator HDC()
	{
		return hMemDC;
	}

private :
	HWND hMainWnd;
	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	Rect rcClient;
};