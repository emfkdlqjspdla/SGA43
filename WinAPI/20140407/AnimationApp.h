#pragma once

#include "..\MainWindow\sga.hpp"
#include "image.h"
#include "animation.h"
#include <windowsx.h>
#include <list>

class AnimationApp : public MainWindow<AnimationApp>
{
	typedef AnimationApp Me;
	typedef MainWindow<AnimationApp> Base;

	typedef std::list<Animation*> Effect;
	typedef Effect::iterator EffectIter;
public :
	AnimationApp()
		: maxEffect(50)
	{
	}
	~AnimationApp()
	{
	}
	void Input(DWORD tick)
	{
		if ((::GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0x8000)
		{
			//::GetCursorPos(&ptMouse);
			DWORD dwPoint = ::GetMessagePos();
			ptMouse.x = GET_X_LPARAM(dwPoint);
			ptMouse.y = GET_Y_LPARAM(dwPoint);

			::ScreenToClient(hMainWnd, &ptMouse);

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

	}
	void Render(DWORD tick)
	{
		buffer << RGB(200,200,100);

		test.Draw(buffer, Rect(100,100,200,200));

		explosion.Draw(buffer, Rect(200,200,264,264));

		for (EffectIter it = effect.begin();
			it != effect.end();
			it++)
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
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		hMainWnd = hWnd;
		test.load(_T("circle.bmp"), Rect(0,0,100,100));
		test.SetTransparent(RGB(255,0,255));

		const int size = 64;
		for (int i = 0; i < 25; i++)
		{
			Image* pImage = new Image();
			pImage->load(_T("explosion.bmp"), Rect(size*(i%5), size*(i/5), size + size*(i%5), size + size*(i/5)));
			explosion.AddImage(pImage);
		}

		explosion.SetTransparent(RGB(255,0,255));
		explosion.SetDelay(50);

		buffer.Attach(hWnd);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::PostQuitMessage(0);
		return 0;
	}

private :
	HWND hMainWnd;
	Image test;
	Animation explosion;
	DoubleBuffer buffer;

	int maxEffect;
	Effect effect;
	Point ptMouse;
};
