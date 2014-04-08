#pragma once

#include "..\MainWindow\sga.hpp"

class AlphaApp : public MainWindow<AlphaApp>
{
	typedef AlphaApp Me;
	typedef MainWindow<AlphaApp> Base;
public :
	void Input(DWORD tick)
	{
	}
	void Update(DWORD tick)
	{
	}
	void Render(DWORD tick)
	{
		buffer << RGB(255,255,255);

		test.Draw(buffer, Rect(100,100,200,200));

		BLENDFUNCTION bf;
		bf.BlendOp =  AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = 200;

		::GdiAlphaBlend(HDC(buffer), 0, 0, rcClient.width(), rcClient.height(),
			hBitmapDC, 0, 0, 10, 10, bf);

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

		test.load(_T("circle.bmp"), Rect(0, 0, 100,100));

		hMainDC = ::GetDC(hWnd);
		hBitmapDC = ::CreateCompatibleDC(hMainDC);
		gray.load(_T("gray.bmp"), Rect(0,0,10,10));
		Select(hBitmapDC, HBITMAP(gray));


		::GetClientRect(hWnd, &rcClient);



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
};
