#pragma once

#include <windows.h>
#include "..\MainWindow\sga.hpp"
#include "image.h"
#include <list>

enum msg_id
{
	WM_COLLIDE = WM_USER + 1,
};

class Circle : public Image
{
public :
	Circle(const Point& c, const LONG& r, COLORREF clr = RGB(0xFF,0xFF,0xFF))
		: center(c), radius(r), speed(0), color(clr)
		, hOwner(NULL)
	{}
	Circle()
		: radius(0), speed(0)
	{
	}
	~Circle()
	{
	}
	void Attach(HWND hWnd)
	{
		hOwner = hWnd;
	}
	void SetData(const Point& pt)
	{
		Rect rc = Image::getRect();

		center = pt;
		radius = rc.width()/2;
	}
	void Update(DWORD tick)
	{
		speed += 1;
		center.y += speed;

		if (center.y >= 500)
		{
			speed = -speed - 1;
		}
	}
	void Draw(HDC hdc)
	{
		Image::Draw(hdc, Rect(center.x - radius, center.y - radius,
			center.x + radius, center.y + radius));
	}

	bool IsCollide(Circle* pObj)
	{
		if ((center ^ pObj->center) <= float(radius + pObj->radius))
		{
			Point ptCollide;

			// r : (r + R) = (x-a) : (c-a)
			// (x-a)*(r+R) = r*(c-a)
			// x-a = (rc-ra)/(r+R)
			// x = (rc-ra)/(r+R) + a
			// x = (rc - ra + ra + Ra)/(r+R)
			// x = (rc + Ra)/(r+R)
			ptCollide.x = (radius*pObj->center.x + pObj->radius*center.x)/(radius+pObj->radius);
			// r : (r + R) = (y-b) : (d-b)
			// (y-b)*(r+R) = r*(d-b)
			// y-b = (rd-rb)/(r+R)
			// y = (rd-rb)/(r+R) + b
			// y = (rd - rb + rb + Rb)/(r+R)
			// y = (rd + Rb)/(r+R)
			ptCollide.y = (radius*pObj->center.y + pObj->radius*center.y)/(radius+pObj->radius);

			LPARAM lParam = MAKEWPARAM(ptCollide.x, ptCollide.y);

			if (hOwner)
				::PostMessage(hOwner, WM_COLLIDE, 0, lParam);
			else
				::PostMessage(NULL, WM_COLLIDE, 0, lParam);
			//::SendMessage();

			return true;
		}

		return false;
	}
	Circle& operator << (const Point& pt)
	{
		center = pt;
		return *this;
	}
	const Point getCenter() const
	{
		return center;
	}
	void setCenter(const Point& pt)
	{
		center.x += pt.x;
		center.y += pt.y;
	}
	const LONG getRadius() const
	{
		return radius;
	}

private :
	Point center;
	LONG radius;
	LONG speed;
	COLORREF color;
	HWND hOwner;
};

