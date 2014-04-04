#include <windows.h>
#include <tchar.h> // for unicode
#include <stdio.h>
#include <math.h>
#include "utility.h"

LRESULT CALLBACK MyWndProc(HWND,UINT,WPARAM,LPARAM);

INT
APIENTRY
_tWinMain(HINSTANCE hInst,
		  HINSTANCE hPrevInst,
		  LPTSTR lpCmdLine,
		  INT nCmdShow)
{
	LPCTSTR lpClassName = _T("MyMainWindow");

	WNDCLASSEX wcex;

	::ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInst;
	wcex.lpfnWndProc = &MyWndProc;
	wcex.lpszClassName = lpClassName;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!::RegisterClassEx(&wcex))
	{
		DWORD dwError = ::GetLastError();

		//ErrorMessageBox(dwError);
		return 2;
	}

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);

	cx = (cx-500)/2;
	cy = (cy-500)/2;

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		cx,
		cy,
		500,
		500,
		0,
		0,
		hInst,
		0);

	if (hMainWnd == NULL)
	{
		DWORD dwError = ::GetLastError();

		//ErrorMessageBox(dwError);
		return 1;
	}


	::ShowWindow(hMainWnd, nCmdShow);
	::UpdateWindow(hMainWnd);

	MSG msg;

	while (true)
	{
		//if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		if (::GetMessage(&msg, 0, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			break;
		}
	}


	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static SYSTEMTIME st;
	static float theta = 0.f;

	if (uMsg == WM_CREATE)
	{
		// WM_TIMER
		// typedef VOID (CALLBACK* TIMERPROC)(HWND, UINT, UINT_PTR, DWORD);
		::SetTimer(hWnd,	// handle window
					0,			// timer id
					1000,		// millisecond time
					NULL);		// TIMEPROC
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);
		// SendMessage
		// PostMessage

		::PostQuitMessage(0);
		return 0;
	}
	//else if (uMsg == WM_CLOSE)
	//{
	//	if (IDOK == ::MessageBox(NULL, _T("Really?"), _T("Confirm"), MB_OKCANCEL))
	//	{
	//		::DestroyWindow(hWnd);
	//	}
	//	return 0;
	//}
	else if (uMsg == WM_PAINT)
	{
		LPCTSTR lpszHello = _T("Welcome to Hell World");
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		// �⺻������ �׸���� WM_PAINT ó���ÿ� �Ѵ�.

		::InflateRect(&rc, -50, -50);

		// �� �����, DC �� �����ϵ��� ����.
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);

		// �� �����, DC �� �����ϵ���...
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush);
		
		// ���� �׸���...
		//::FillRect(hdc, &rc, RedBrush);

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);

		int cx = (rc.right + rc.left)/2;
		int cy = (rc.right + rc.left)/2;

		const int length = 100;
		POINT pt;
		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, cx, cy-length);

		float pi = 2.f*asin(1.f);
		float D2R = pi/180.f;

		int x = cx + int(length*sin(theta*D2R));
		int y = cy - int(length*cos(theta*D2R));
		
		// 12�ð� : 360 = 1�ð� : x, x = 360/12 = 30
		// 60�� : 30 = 1�� : x, x = 30/60 = 1/2
		// 60�� : 1/2 = 1�� : x, x = 1/120

		// 60�� : 360 = 1�� : x, x = 6
		// 60�� : 6 = 1�� : x, x = 1/10

		::MoveToEx(hdc, cx, cy, &pt);
		::LineTo(hdc, x, y);




		TCHAR lpszTime[10];

		_stprintf_s(lpszTime, _countof(lpszTime), _T("%02d:%02d:%02d"), 
			st.wHour, st.wMinute, st.wSecond);

		::DrawText(hdc, lpszTime, -1, &rc, DT_CENTER);


		// ����
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_TIMER)
	{
		::GetLocalTime(&st);
		//::GetSystemTime(&st);

		RECT rc;
		::GetClientRect(hWnd, &rc);

		theta += 10.f;
		if (theta > 360.f)
			theta = 0.f;

		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
