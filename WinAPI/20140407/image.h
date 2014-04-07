#pragma once

#include <windows.h>

class Image
{
public :
	Image()
		: hBitmap(NULL)
		, clrTransparent(RGB(0,0,0))
		, bUseTransparent(false)
	{
	}
	~Image()
	{
		if (hBitmap)
			::DeleteObject(hBitmap);
	}

public :
	BOOL load(LPCTSTR szFileName, const Rect& rc)
	{
		hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
		if (hBitmap == NULL)
			return FALSE;

		rcBitmap = rc;

		return TRUE;
	}
	void Draw(HDC hdc, const Rect& rcDest, DWORD rop = SRCCOPY)
	{
		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = Select(hBitmapDC, hBitmap);

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
		Select(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);
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
	HBITMAP hBitmap;
	Rect rcBitmap;

	COLORREF clrTransparent;
	bool bUseTransparent;
};