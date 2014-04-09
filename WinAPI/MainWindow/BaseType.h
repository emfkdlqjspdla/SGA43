#pragma once

#include <windows.h>
#include <cmath>
struct Point;
struct Rect;
struct Size;

struct Point : public POINT
{
	Point(const LONG& _x = 0, const LONG& _y = 0)
	{
		x = _x;
		y = _y;
	}
	Point operator - (const Point& o)
	{
		return Point(x-o.x, y-o.y);
	}
	float operator ^ (const Point& o)
	{
		return sqrt(float((x-o.x)*(x-o.x) + (y-o.y)*(y-o.y)));
	}
};

struct Size : public SIZE
{
	Size(const LONG& _cx = 0, const LONG& _cy = 0)
	{
		cx = _cx;
		cy = _cy;
	}
};

struct Rect : public RECT
{
	Rect()
	{
		left = top = right = bottom = 0;
	}
	Rect(const LONG& l, const LONG& t, const LONG& r, const LONG& b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
	Rect(const Point& c, const Size& s)
	{
		left = c.x - s.cx/2;
		top = c.y - s.cy/2;
		right = left + s.cx;
		bottom = top + s.cy;
	}
	Rect(const Point& lt, const Point& rb)
	{
		left = lt.x;
		top = lt.y;
		right = rb.x;
		bottom = rb.y;
	}
	Point lefttop()
	{
		return Point(left, top);
	}
	Point rightbottom()
	{
		return Point(right, bottom);
	}
	const LONG width() const
	{
		return (right - left);
	}
	const LONG height() const
	{
		return (bottom - top);
	}
	// OffsetRect
	Rect operator << (const Size& diff)
	{
		return Rect(left - diff.cx, top - diff.cy,
			right - diff.cx, bottom - diff.cy);
	}
	Rect operator >> (const Size& diff)
	{
		return Rect(left + diff.cx, top + diff.cy,
			right + diff.cx, bottom + diff.cy);
	}
	// InflateRect
	Rect operator + (const Size& diff)
	{
		return Rect(left - diff.cx, top - diff.cy,
			right + diff.cx, bottom + diff.cy);
	}
	Rect operator - (const Size& diff)
	{
		return Rect(left + diff.cx, top + diff.cy,
			right - diff.cx, bottom - diff.cy);
	}
};

