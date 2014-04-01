#pragma once

#include <windows.h>

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
};

