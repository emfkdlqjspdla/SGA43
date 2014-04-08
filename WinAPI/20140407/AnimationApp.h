#pragma once

#include "..\MainWindow\sga.hpp"
#include "image.h"
#include "animation.h"
#include "Circle.h"
#include <windowsx.h>
#include <list>

class AnimationApp : public MainWindow<AnimationApp>
{
	typedef AnimationApp Me;
	typedef MainWindow<AnimationApp> Base;

	typedef std::list<Animation*> Effect;
	typedef Effect::iterator EffectIter;

	typedef std::list<Circle*> marble;
public :
	AnimationApp()
		: maxEffect(50), marble_dt(0), marble_delay(50)
	{
	}
	~AnimationApp()
	{
	}
	void Input(DWORD tick)
	{
		DWORD dwPoint = ::GetMessagePos();
		ptMouse.x = GET_X_LPARAM(dwPoint);
		ptMouse.y = GET_Y_LPARAM(dwPoint);

		::ScreenToClient(hMainWnd, &ptMouse);

		if ((::GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0x8000)
		{
			AddEffect(ptMouse);
		}
	}
	void Update(DWORD tick)
	{
		explosion.Update(tick);

		for (EffectIter it = effect.begin();
			it != effect.end();
			it++)
		{
			(*it)->Update(tick);
		}

		if (marble_dt > marble_delay)
		{
			marble::iterator it;
			for (it = circles.begin(); it != circles.end(); it++)
			{
				(*it)->Update(tick);
				if ((*it)->IsCollide(&MouseCircle))
				{
					if ((*it)->getCenter().x > MouseCircle.getCenter().x)
						(*it)->setCenter(Point((*it)->getRadius(), 0));
					else
						(*it)->setCenter(Point(-(*it)->getRadius(), 0));
				}
			}

			marble_dt -= marble_delay;
		}

		marble_dt += tick;

		MouseCircle << ptMouse;

	}
	void Render(DWORD tick)
	{
		buffer << RGB(200,200,100);

		MouseCircle.Draw(buffer);

		explosion.Draw(buffer, Rect(200,200,264,264));

		for (EffectIter it = effect.begin();
			it != effect.end();
			it++)
		{
			(*it)->Draw(buffer);
		}

		marble::iterator it;
		for (it = circles.begin(); it != circles.end(); it++)
		{
			(*it)->Draw(buffer);
		}

		buffer.Draw();
	}
protected :
	void AddEffect(const Point& pt)
	{
		if (effect.size() == maxEffect)
		{
			EffectIter it = effect.begin();

			delete (*it);
			effect.pop_front();
		}

		Animation* pAnimation = new Animation;
		const int size = 64;
		for (int i = 0; i < 25; i++)
		{
			Image* pImage = new Image();
			pImage->load(_T("explosion.bmp"), Rect(size*(i%5), size*(i/5), size + size*(i%5), size + size*(i/5)));
			pAnimation->AddImage(pImage);
		}

		pAnimation->SetTransparent(RGB(255,0,255));
		pAnimation->SetDelay(50);
		pAnimation->SetPosition(pt);

		effect.push_back(pAnimation);
	}


	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		AddEventHandler(WM_COLLIDE, &Me::OnCollide);
	}
	LRESULT OnCollide(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Point pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		AddEffect(pt);
		return 0;
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hMainWnd = hWnd;
		MouseCircle.load(_T("circle.bmp"), Rect(0,0,100,100));
		MouseCircle.SetTransparent(RGB(255,255,255));
		MouseCircle.SetData(Point(0,0));

		const int size = 64;
		for (int i = 0; i < 25; i++)
		{
			Image* pImage = new Image();
			pImage->load(_T("explosion.bmp"), Rect(size*(i%5), size*(i/5), size + size*(i%5), size + size*(i/5)));
			explosion.AddImage(pImage);
		}

		explosion.SetTransparent(RGB(255,0,255));
		explosion.SetDelay(50);


		for (int i = 0; i < 5; i++)
		{
			Circle* pImage = new Circle;
			pImage->load(_T("circle_group.bmp"), 
				Rect(100*i, 0, 100 + 100*i, 100));
			pImage->SetTransparent(RGB(255,255,255));
			pImage->SetData(Point(50 + rand()%400, 50 + rand()%200));
			pImage->Attach(hWnd);
			circles.push_back(pImage);
		}


		buffer.Attach(hWnd);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		marble::iterator it;
		for (it = circles.begin(); it != circles.end(); it++)
		{
			delete (*it);
		}

		::PostQuitMessage(0);
		return 0;
	}

private :
	HWND hMainWnd;
	Circle MouseCircle;
	Animation explosion;
	DoubleBuffer buffer;

	int maxEffect;
	Effect effect;
	Point ptMouse;

	marble circles;
	DWORD marble_dt;
	DWORD marble_delay;
};
