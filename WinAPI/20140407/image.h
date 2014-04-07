#pragma once

#include <windows.h>

class Image
{
public :
	Image()
		: hMainDC(NULL)
		, hBitmapDC(NULL)
		, hBitmap(NULL)
		, clrTransparent(RGB(0,0,0))
		, bUseTransparent(false)
	{
	}
	~Image()
	{
		Detach();
	}

public :
	BOOL Attach(HWND hWnd)
	{
		if (!hWnd) return FALSE;

		hOwner = hWnd;
		hMainDC = ::GetDC(hWnd);
		hBitmapDC = ::CreateCompatibleDC(hMainDC);
		hOldBitmap = Select(hBitmapDC, hBitmap);

		return TRUE;
	}
	void Detach()
	{
		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);
		::ReleaseDC(hOwner, hMainDC);
	}
	BOOL load(LPCTSTR szFileName, const Rect& rc)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
		if (hBitmap == NULL)
			return FALSE;

		rcBitmap = rc;

		return TRUE;
	}
	void Draw(const Rect& rcDest, DWORD rop = SRCCOPY)
	{
		if (bUseTransparent)
		{
			::GdiTransparentBlt(hMainDC, rcDest.left, rcDest.top,
				rcDest.width(), rcDest.height(), 
				hBitmapDC, rcBitmap.left, rcBitmap.top,
				rcBitmap.width(), rcBitmap.height(),
				clrTransparent);
		}
		else
		{
			::BitBlt(hMainDC, rcDest.left, rcDest.top,
				rcDest.width(), rcDest.height(), 
				hBitmapDC, rcBitmap.left, rcBitmap.top, rop);
		}
	}
	void Draw(HDC hdc, const Rect& rcDest, DWORD rop = SRCCOPY)
	{
		if (bUseTransparent)
		{
			::GdiTransparentBlt(hdc, rcDest.left, rcDest.top,
				rcDest.width(), rcDest.height(), 
				hBitmapDC, rcBitmap.left, rcBitmap.top,
				rcBitmap.width(), rcBitmap.height(),
				clrTransparent);
		}
		else
		{
			::BitBlt(hdc, rcDest.left, rcDest.top,
				rcDest.width(), rcDest.height(), 
				hBitmapDC, rcBitmap.left, rcBitmap.top, rop);
		}
	}
	void SetTransparent(COLORREF clr)
	{
		clrTransparent = clr;
		bUseTransparent = true;
	}
	Rect getRect()
	{
		return rcBitmap;
	}



private :
	HWND hOwner;
	HDC hMainDC;
	HDC hBitmapDC;
	HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	Rect rcBitmap;

	COLORREF clrTransparent;
	bool bUseTransparent;
};