#pragma once

#include "..\MainWindow\MainWindow.h"
#include "Circle.h"
#include <list>
#include <cstdlib>
#include <windowsx.h>
#include "DoubleBuffer.h"

class TransparentApp : public MainWindow<TransparentApp>
{
	typedef TransparentApp Me;
	typedef MainWindow<TransparentApp> Base;
	typedef std::list<Circle*> marble;
public :
	TransparentApp()
		: update_dt(0), red(0)
	{
		SetWindowTitle(_T("Transparent Bitmap Sample"));
	}

protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		//AddEventHandler(WM_PAINT, &Me::OnPaint);
		//AddEventHandler(WM_TIMER, &Me::OnTimer);
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
		//hMainDC = ::GetDC(hWnd);
		//hMemDC = ::CreateCompatibleDC(hMainDC);
		//hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rcClient.width(), rcClient.height());
		//::SelectObject(hMemDC, hMemBitmap);
		backbuffer.Attach(hWnd);
		// for double buffering...
		//////////////////////////////////////

		bgColor = ::CreateSolidBrush(RGB(255,200,200));

		const int size = 64;
		for (int i = 0; i < 50; i++)
		{
			Circle* pCircle = new Circle(Point(50 + rand()%400, 50 + rand()%300), 50);
			pCircle->SetImage(_T("explosion.bmp"), Rect((i%7)*size, 0, size + (i%7)*size, size));
			pCircle->SetTransparentColor(RGB(255,0,255));
			depot.push_back(pCircle);
		}

		MouseCircle.SetTransparentColor(RGB(255,255,255));
		MouseCircle.SetData(Point(0,0), 10);
		MouseCircle.SetImage(_T("circle.bmp"), Rect(0,0,100,100));

		::SetTimer(hWnd, 0, 100, NULL);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		marble::iterator it;
		for (it = depot.begin(); it != depot.end(); it++)
		{
			delete (*it);
		}

		::DeleteObject(bgColor);

		//////////////////////////////////////
		// for double buffering...
		backbuffer.Detach();
		//::DeleteObject(hMemBitmap);
		//::DeleteDC(hMemDC);
		//::ReleaseDC(hWnd, hMainDC);
		// for double buffering...
		//////////////////////////////////////

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
public :
	//LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	void Render(DWORD tick)
	{
		//PAINTSTRUCT ps;
		//HDC hdc = ::BeginPaint(hWnd, &ps);

		backbuffer << RGB(red,255,255);

		//::FillRect(backbuffer, &rcClient, bgColor);

		//SYSTEMTIME st;

		//::GetLocalTime(&st);

		//TCHAR szTime[100];

		//_stprintf_s(szTime, _T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);

		//::DrawText(backbuffer, szTime, -1, &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		marble::iterator it;
		for (it = depot.begin(); it != depot.end(); it++)
		{
			(*it)->Draw(backbuffer);
		}

		MouseCircle.Draw(backbuffer);

		backbuffer.Draw();
		//::BitBlt(hMainDC, 0, 0, rcClient.width(), rcClient.height(),
		//	backbuffer, 0, 0, SRCCOPY);

		//::EndPaint(hWnd, &ps);
		//return 0;
	}
	//LRESULT OnTimer(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

			red += 10;
			if (red > 255)
				red = 0;

			update_dt -= 50;
		}

		update_dt += tick;

		//Invalidate();
		//return 0;
	}

private :
	Rect rcClient;

	// for double buffering...
	DoubleBuffer backbuffer;

	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	HBRUSH bgColor;

	marble depot;
	Circle MouseCircle;
	Point ptMouse;

	DWORD update_dt;

	BYTE red;
};
