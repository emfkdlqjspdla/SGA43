#pragma once

#include <windows.h>
#include "..\MainWindow\BaseType.h"

class Circle
{
public :
	Circle()
		: radius(0), speed(0)
	{
	}
	~Circle()
	{
	}
	void SetData(const Point& c, const LONG& r)
	{
		center = c;
		radius = r;
	}
	void Update()
	{
		speed += 1;
		center.y += speed;

		if (center.y >= 500)
		{
			speed = -speed;
		}
	}
	void Draw(HDC hdc)
	{
		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);
	}

private :
	Point center;
	LONG radius;
	LONG speed;
};
