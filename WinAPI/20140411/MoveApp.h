﻿#pragma once

#include "..\MainWindow\sga.hpp"
#include "Input.h"
#include "button.h"

class MoveApp : public MainWindow<MoveApp>
{
	typedef MoveApp Me;
	typedef MainWindow<MoveApp> Base;

	enum mode_id
	{
		INGAME_MODE,
		CONTROL_MODE,
	};

public :
	MoveApp()
		: mode(INGAME_MODE)
		, FPS_dt(0), FPS_frame(0)
		, bGrabWindow(false), hMainWnd(NULL)
		, dx(0), bg_update_dt(0), bg_update_delay(5)
	{
		//SetWindowTitle(_T("Alphablend Sample"));
		SetWindowSize(800, 600);
		SetWindowStyle(WS_POPUP | WS_VISIBLE);
	}
	void Input(DWORD tick)
	{
		if (InputDevice.OnClick(VK_ESCAPE))
		{
			if (mode == CONTROL_MODE)
				mode = INGAME_MODE;
			else
				mode = CONTROL_MODE;
		}

		if (mode == CONTROL_MODE)
		{
			btnMini.Update(tick);
			btnClose.Update(tick);

			if (InputDevice[VK_LBUTTON])
			{
				bGrabWindow = true;
			}
			else
			{
				bGrabWindow = false;
			}

			ptPrev = ptMouse;
			ptMouse = InputDevice.getPos();
		}
	}
	void Update(DWORD tick)
	{
		// FPS_dt(ms) : FPS_frame = 1000ms : x
		// FPS_frame*1000/FPS_dt = x
		if (FPS_dt > 1000)
		{
			FPS_dt -= 1000;
			FPS_frame = 0;
		}

		FPS_dt += tick;
		FPS_frame++;
		dwFPS = FPS_frame*1000/FPS_dt;

		if (bg_update_dt > bg_update_delay)
		{
			int count = bg_update_dt/bg_update_delay;

			for (int i = 0; i < count; i++)
			{
				dx--;
				if (dx <= -block.getRect().width())
					dx = 0;
			}

			bg_update_dt %= bg_update_delay;
		}
		bg_update_dt += tick;


		if (mode == CONTROL_MODE)
		{
			Point diff = ptMouse - ptPrev;

			if (bGrabWindow)
			{
				if (hMainWnd)
				{
					Rect rcWindow = rcClient.ToScreen(hMainWnd);
					::MoveWindow(hMainWnd, rcWindow.left + diff.x, rcWindow.top + diff.y, rcWindow.width(), rcWindow.height(), TRUE);
				}
			}
		}
	}
	void Render(DWORD tick)
	{
		buffer << RGB(100,255,255);

		Size cBlock(block.getRect().width(), block.getRect().height());
		for (int y = 0; y < rcClient.height(); y += cBlock.cy)
		{
			for (int x = dx; x < rcClient.width(); x += cBlock.cx)
			{
				block.Draw(buffer, Rect(x, y, x+cBlock.cx, y+cBlock.cy));
			}
		}


		test.Draw(buffer, Rect(100,100,200,200));

		if (mode == CONTROL_MODE)
		{
			BLENDFUNCTION bf;
			bf.BlendOp =  AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.AlphaFormat = 0;
			bf.SourceConstantAlpha = 200;	// 0(완전투명:transparent) ~ 255(완전불투명:opaque)

			::GdiAlphaBlend(HDC(buffer), 
							0, 0, rcClient.width(), rcClient.height(),
							hBitmapDC, 
							0, 0, 10, 10, bf);

			btnMini.Draw(buffer);
			btnClose.Draw(buffer);
		}

		TCHAR szDebug[200];
		_stprintf_s(szDebug, _T("FPS : %08d"), dwFPS);
		::DrawText(buffer, szDebug, -1, &rcClient, DT_LEFT | DT_TOP);

		buffer.Draw();
	}

	static void Proxy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::SendMessage(hWnd, uMsg, wParam, lParam);
	}

protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
		//AddEventHandler(WM_SYSCOMMAND, &Me::OnSysCommand);
	}
	//LRESULT OnSysCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	//{
	//	if (SC_MAXIMIZE == wParam)
	//	{
	//		::DefWindowProc(hWnd, uMsg, wParam, lParam);
	//		buffer.Attach(hWnd);
	//		::GetClientRect(hWnd, &rcClient);
	//	}
	//	else
	//	{
	//		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	//	}
	//	return 0;
	//}

	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::GetClientRect(hWnd, &rcClient);
		hMainWnd = hWnd;

		block.load(_T("block.bmp"));

		test.load(_T("circle.bmp"), Rect(0, 0, 100,100));

		hMainDC = ::GetDC(hWnd);
		hBitmapDC = ::CreateCompatibleDC(hMainDC);
		gray.load(_T("gray.bmp"), Rect(0,0,10,10));
		Select(hBitmapDC, HBITMAP(gray));


		btnMini.Attach(hWnd);
		btnMini.SetImageOn(_T("button2.bmp"), Rect(50,0,100,50));
		btnMini.SetImageOff(_T("button2.bmp"), Rect(0,0,50,50));
		btnMini.SetTransparent(RGB(255,0,255));
		btnMini.SetButtonRect(Rect(800 - 50*2, 0, 800 - 50, 50));
		btnMini.SetAction(&MoveApp::Proxy, hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);


		btnClose.Attach(hWnd);
		btnClose.SetImageOn(_T("button2.bmp"), Rect(150,0,200,50));
		btnClose.SetImageOff(_T("button2.bmp"), Rect(100,0,150,50));
		btnClose.SetTransparent(RGB(255,0,255));
		btnClose.SetButtonRect(Rect(800 - 50, 0, 800, 50));
		btnClose.SetAction(&MoveApp::Proxy, hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);

		buffer.Attach(hWnd);
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		gray.release();
		::DeleteDC(hBitmapDC);
		::ReleaseDC(hWnd, hMainDC);

		::PostQuitMessage(0);
		return 0;
	}

private :
	DoubleBuffer buffer;
	Rect rcClient;

	HDC hMainDC;
	HDC hBitmapDC;
	Image gray;
	Image test;

	Image block;

	int mode;

	// FPS 출력용도.
	// FPS : Frame Per Second.
	// FPS_dt(ms) : FPS_frame = 1000ms : x
	// FPS_frame*1000/FPS_dt = x
	DWORD FPS_dt;
	DWORD FPS_frame;
	DWORD dwFPS;

	Button btnMini;
	Button btnClose;

	HWND hMainWnd;
	Point ptPrev;
	Point ptMouse;
	bool bGrabWindow;

	//////////////////////////
	// moving background...
	LONG dx;
	DWORD bg_update_dt;
	DWORD bg_update_delay;
};
