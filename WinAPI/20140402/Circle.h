#pragma once

#include <windows.h>
#include "..\MainWindow\sga.hpp"

#include <list>

class Object
{
protected :
	typedef unsigned long id_type;
	Object():id(gen_id++){}
	id_type getid() const {return id;}

private :
	id_type id;
	static id_type gen_id;
};

unsigned long Object::gen_id = 0;

template<typename T>
class ObjectManager : public singleton<ObjectManager<T>>
{
	friend class singleton<ObjectManager<T>>;

	typedef T element;
	typedef element* pointer;
	typedef element& reference;
	typedef std::list<pointer> DepotType;
	typedef typename DepotType::iterator DepotIter;
	typedef typename element::id_type id_type;
private :
	ObjectManager(){}
	~ObjectManager(){}
public :
	bool insert(pointer pObj)
	{
		depot.push_back(pObj);
		return true;
	}
	bool remove(pointer pObj)
	{
		for (DepotIter it = depot.begin(); it != depot.end(); it++)
		{
			if (*it == pObj)
			{
				SafeDelete(*it);
				depot.erase(it);
				return true;
			}
		}
		return false;
	}
	bool remove(const id_type& _id)
	{
		for (DepotIter it = depot.begin(); it != depot.end(); it++)
		{
			if ((*it)->getid() == _id)
			{
				SafeDelete(*it);
				depot.erase(it);
				return true;
			}
		}
		return false;
	}

private :
	DepotType depot;
};

class Shape : public Object
{
};

class Circle : public Shape
{
public :
	Circle(const Point& c, const LONG& r, COLORREF clr = RGB(0xFF,0xFF,0xFF))
		: center(c), radius(r), speed(0), color(clr)
	{}
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
	void SetColor(COLORREF clr)
	{
		color = clr;
	}
	void Update(Circle* pObj)
	{
		speed += 1;
		center.y += speed;

		if (center.y >= 500)
		{
			speed = -speed - 1;
		}

		if (IsCollide(pObj))
		{
			if (center.x > pObj->center.x)
				center.x += pObj->radius;
			else
				center.x -= pObj->radius;
		}
	}
	bool IsCollide(Circle* pObj)
	{
		if ((center ^ pObj->center) <= float(radius + pObj->radius))
			return true;

		return false;
	}
	void Draw(HDC hdc)
	{
		HBRUSH brush = ::CreateSolidBrush(color);
		HBRUSH oldbrush = Select(hdc, brush);
		HPEN pen = ::CreatePen(PS_SOLID, 1, color);
		HPEN oldpen = Select(hdc, pen);

		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);

		Select(hdc, oldpen);
		::DeleteObject(pen);

		Select(hdc, oldbrush);
		::DeleteObject(brush);
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
};

