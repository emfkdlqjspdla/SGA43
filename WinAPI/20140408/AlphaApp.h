#pragma once

#include "..\MainWindow\sga.hpp"
#include "Input.h"

class AlphaApp : public MainWindow<AlphaApp>
{
	typedef AlphaApp Me;
	typedef MainWindow<AlphaApp> Base;

	enum mode_id
	{
		INGAME_MODE,
		CONTROL_MODE,
	};

public :
	AlphaApp()
		: mode(INGAME_MODE)
		, FPS_dt(0), FPS_frame(0)
	{
		//SetWindowTitle(_T("Alphablend Sample"));
		SetWindowSize(640, 480);
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
	}
	void Render(DWORD tick)
	{
		buffer << RGB(100,255,255);

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
		}

		TCHAR szDebug[200];
		_stprintf_s(szDebug, _T("FPS : %08d"), dwFPS);
		::DrawText(buffer, szDebug, -1, &rcClient, DT_LEFT | DT_TOP);

		buffer.Draw();
	}

protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
	}

	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::GetClientRect(hWnd, &rcClient);

		test.load(_T("circle.bmp"), Rect(0, 0, 100,100));

		hMainDC = ::GetDC(hWnd);
		hBitmapDC = ::CreateCompatibleDC(hMainDC);
		gray.load(_T("gray.bmp"), Rect(0,0,10,10));
		Select(hBitmapDC, HBITMAP(gray));





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

	int mode;

	// FPS 출력용도.
	// FPS : Frame Per Second.
	// FPS_dt(ms) : FPS_frame = 1000ms : x
	// FPS_frame*1000/FPS_dt = x
	DWORD FPS_dt;
	DWORD FPS_frame;
	DWORD dwFPS;

};
