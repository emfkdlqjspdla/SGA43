#pragma once

#include "..\MainWindow\MainWindow.h"
#include "DoubleBuffer.h"
#include "Circle.h"
#include <list>
#include <vector>
#include <cstdlib>
#include <windowsx.h>

class TransparentApp : public MainWindow<TransparentApp>
{
	typedef TransparentApp Me;
	typedef MainWindow<TransparentApp> Base;
	typedef std::list<Circle*> marble;
public :
	TransparentApp()
		: update_dt(0), red(0), index(0)
	{
		SetWindowTitle(_T("Transparent Bitmap Sample"));
	}

protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		AddEventHandler(WM_PAINT, &Me::OnPaint);
		AddEventHandler(WM_TIMER, &Me::OnTimer);
		AddEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
	}
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		return 0;
	}

	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::GetClientRect(hWnd, &rcClient);

		//////////////////////////////////////
		// for double buffering...
		backbuffer.Attach(hWnd);
		// for double buffering...
		//////////////////////////////////////

		const int size = 64;
		for (int i = 0; i < 1; i++)
		{
			Circle* pCircle = new Circle(Point(50 + rand()%400, 50 + rand()%300), 50);
			pCircle->SetImage(_T("explosion.bmp"), Rect((i%7)*size, 0, size + (i%7)*size, size));
			pCircle->SetTransparentColor(RGB(255,0,255));
			depot.push_back(pCircle);
		}

		for (int i = 0; i < 25; i++)
		{
			animation.push_back(Rect((i%5)*size, (i/5)*size, size+(i%5)*size, size+(i/5)*size));
		}

		MouseCircle.SetTransparentColor(RGB(255,255,255));
		MouseCircle.SetData(Point(0,0), 10);
		MouseCircle.SetImage(_T("circle.bmp"), Rect(0,0,100,100));

		::SetTimer(hWnd, 0, 50, NULL);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		marble::iterator it;
		for (it = depot.begin(); it != depot.end(); it++)
		{
			delete (*it);
		}

		//////////////////////////////////////
		// for double buffering...
		backbuffer.Detach();
		// for double buffering...
		//////////////////////////////////////

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	//void Render(DWORD tick)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		backbuffer << RGB(255,255,255);

		marble::iterator it;
		for (it = depot.begin(); it != depot.end(); it++)
		{
			(*it)->Draw(backbuffer);
		}

		MouseCircle.Draw(backbuffer);

		backbuffer.Draw();

		::EndPaint(hWnd, &ps);
		return 0;
	}
	LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	//void Update(DWORD tick)
	{
		MouseCircle << ptMouse;

		marble::iterator it;
		for (it = depot.begin(); it != depot.end(); it++)
		{
			(*it)->SetRect(animation[index]);
			(*it)->Update(&MouseCircle);
		}

		index++;
		if (index >= 25)
			index = 0;

		Invalidate();
		return 0;
	}

private :
	Rect rcClient;

	// for double buffering...
	DoubleBuffer backbuffer;

	marble depot;
	Circle MouseCircle;
	Point ptMouse;

	DWORD update_dt;

	BYTE red;
	std::vector<Rect> animation;
	int index;
};
