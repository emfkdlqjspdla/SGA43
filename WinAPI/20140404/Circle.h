#pragma once

#include <windows.h>
#include "..\MainWindow\sga.hpp"

#include <list>

class Circle
{
public :
	Circle(const Point& c, const LONG& r, COLORREF clr = RGB(0xFF,0xFF,0xFF))
		: center(c), radius(r), speed(0), color(clr), bUseTransparent(FALSE)
	{}
	Circle()
		: radius(0), speed(0), bUseTransparent(FALSE)
	{
	}
	~Circle()
	{
	}
	void SetImage(LPCTSTR lpszFileName, const Rect& rc)
	{
		hImage = (HBITMAP)::LoadImage(NULL, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
		rcImage = rc;

		//BITMAP bm;
		//::GetObject(hImage, sizeof(BITMAP), &bm);
		//radius = bm.bmWidth/2;

		radius = rc.width()/2;
	}
	void SetRect(const Rect& rc)
	{
		rcImage = rc;
	}
	void SetRect(const std::vector<Rect>& rclist)
	{
		animation = rclist;
	}
	void SetTransparentColor(COLORREF clr)
	{
		bUseTransparent = TRUE;
		clrTransparent = clr;
	}
	void SetData(const Point& c, const LONG& r)
	{
		center = c;
		radius = r;
	}
	void SetColor(COLORREF clr)
	{
		color = clr;
	}
	//void Update(Circle* pObj)
	//{
	//	speed += 1;
	//	center.y += speed;

	//	if (center.y >= 500)
	//	{
	//		speed = -speed - 1;
	//	}

	//	if (IsCollide(pObj))
	//	{
	//		if (center.x > pObj->center.x)
	//			center.x += pObj->radius;
	//		else
	//			center.x -= pObj->radius;
	//	}
	//}
	bool IsCollide(Circle* pObj)
	{
		if ((center ^ pObj->center) <= float(radius + pObj->radius))
			return true;

		return false;
	}
	void Update(DWORD tick)
	{
		// delay = 100;
		if (update_dt > delay)
		{
			index++;
			if (index>= 25)
				index = 0;

			update_dt -= delay;
		}

		update_dt += tick;
	}
	void Draw(HDC hdc)
	{
		HDC hBitmap = ::CreateCompatibleDC(hdc);
		::SelectObject(hBitmap, hImage);

		if (bUseTransparent)
			//::GdiTransparentBlt(hdc,
			//center.x - radius, center.y - radius, 
			//radius*2, radius*2,
			//hBitmap, rcImage.left, rcImage.top, 
			//rcImage.width(), rcImage.height(), 
			//clrTransparent);
			::GdiTransparentBlt(hdc,
			center.x - radius, center.y - radius, 
			radius*2, radius*2,
			hBitmap, animation[index].left, animation[index].top, 
			rcImage.width(), rcImage.height(), 
			clrTransparent);
		else
			::BitBlt(hdc, center.x - radius, center.y - radius, radius*2, radius*2,
				hBitmap, rcImage.left, rcImage.top, SRCCOPY);


		::DeleteDC(hBitmap);

		//HBRUSH brush = ::CreateSolidBrush(color);
		//HBRUSH oldbrush = Select(hdc, brush);
		//HPEN pen = ::CreatePen(PS_SOLID, 1, color);
		//HPEN oldpen = Select(hdc, pen);

		//::Ellipse(hdc, center.x - radius, center.y - radius,
		//	center.x + radius, center.y + radius);

		//Select(hdc, oldpen);
		//::DeleteObject(pen);

		//Select(hdc, oldbrush);
		//::DeleteObject(brush);
	}

	Circle& operator << (const Point& pt)
	{
		center = pt;
		return *this;
	}

private :
	Point center;
	LONG radius;
	LONG speed;
	COLORREF color;
	HBITMAP hImage;
	Rect rcImage;
	COLORREF clrTransparent;
	BOOL bUseTransparent;

	std::vector<Rect> animation;
};

