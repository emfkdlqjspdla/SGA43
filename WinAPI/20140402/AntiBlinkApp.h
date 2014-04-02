#pragma once

#include "..\MainWindow\MainWindow.h"
#include "Circle.h"
#include <list>
#include <cstdlib>
#include <windowsx.h>

class AntiBlinkApp : public MainWindow<AntiBlinkApp>
{
	typedef AntiBlinkApp Me;
	typedef MainWindow<AntiBlinkApp> Base;
	typedef std::list<Circle*> marble;
public :
	AntiBlinkApp()
		: update_dt(0)
	{
		//SetWindowTitle(_T("Anti Blink Sample"));
	}

protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		//AddEventHandler(WM_PAINT, &Me::OnPaint);
		//AddEventHandler(WM_TIMER, &Me::OnTimer);
		AddEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
		AddEventHandler(WM_ERASEBKGND, &Me::OnEraseBkgnd);
	}
	LRESULT OnEraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}

	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::GetClientRect(hWnd, &rcClient);

		hMainDC = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hMainDC);
		hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rcClient.width(), rcClient.height());
		::SelectObject(hMemDC, hMemBitmap);

		hBgBrush = ::CreateSolidBrush(RGB(0xE6,0xE6,0xFA));

		// 글자 출력시 바탕 투명 처리.
		::SetBkMode(hMemDC, TRANSPARENT);

		////////////////////////////////////
		// circle init
		for (int i = 0; i < 50; i++)
		{
			depot.push_back(new Circle(Point(50 + rand()%400, 50 + rand()%300),
									   15 + rand()%10,
									   RGB(255, 50 + rand()%200, 100 + rand()%150)));
		}

		MouseCircle.SetData(Point(0,0), 50);
		MouseCircle.SetColor(RGB(255,255,255));

		::SetTimer(hWnd, 1, 100, NULL);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		marble::iterator it;
		for (it = depot.begin(); it != depot.end();)
		{
			delete (*it);
			it = depot.erase(it);
		}

		::DeleteObject(hBgBrush);

		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(hWnd, hMainDC);

		::KillTimer(hWnd, 1);
		::PostQuitMessage(0);
		return 0;
	}
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);
		return 0;
	}
public :
//	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	void Render(DWORD tick)
	{
		// background
		::FillRect(hMemDC, &rcClient, hBgBrush);

		::DrawText(hMemDC, _T("Hello World"), -1, &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		marble::iterator it;
		for (it = depot.begin(); it != depot.end(); it++)
		{
			(*it)->Draw(hMemDC);
		}

		MouseCircle.Draw(hMemDC);

		::BitBlt(hMainDC, 0, 0, rcClient.width(), rcClient.height(), hMemDC, 0, 0, SRCCOPY);

		//return 0;
	}

//	LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	void Update(DWORD tick)
	{
		MouseCircle << ptMouse;

		if (update_dt > 50)
		{
			marble::iterator it;
			for (it = depot.begin(); it != depot.end(); it++)
			{
				(*it)->Update(&MouseCircle);
			}

			update_dt -= 50;
		}
		update_dt += tick;
		//Invalidate();


		//return 0;
	}

private :
	////////////////////////////////
	// for anti blink
	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	Rect rcClient;
	HBRUSH hBgBrush;

	DWORD update_dt;
	marble depot;

	Point ptMouse;
	Circle MouseCircle;
};
