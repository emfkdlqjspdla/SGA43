#include <windows.h>
#include <tchar.h> // for unicode
#include <stdio.h>
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

	HWND hMainWnd = ::CreateWindowEx(0, lpClassName,
		_T("Hello World"),
		WS_OVERLAPPEDWINDOW,
		500,
		100,
		800,
		600,
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

	// WM_TIMER
	::SetTimer(hMainWnd, 0, 1000, NULL);

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

	::KillTimer(hMainWnd, 0);

	return msg.wParam;
}

LRESULT CALLBACK MyWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static SYSTEMTIME st;

	if (uMsg == WM_DESTROY)
	{
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

		// 기본적으로 그리기는 WM_PAINT 처리시에 한다.

		::InflateRect(&rc, -50, -50);

		// 펜 만들고, DC 가 선택하도록 하자.
		HPEN GreenPen = ::CreatePen(PS_SOLID, 5, RGB(100,200,100));
		HPEN oldPen = (HPEN)::SelectObject(hdc, GreenPen);

		// 붓 만들고, DC 가 선택하도록...
		HBRUSH RedBrush = ::CreateSolidBrush(RGB(200,100,100));
		HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, RedBrush);
		
		// 무언가 그리고...
		//::FillRect(hdc, &rc, RedBrush);

		::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

		::Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);

		POINT pt;
		::MoveToEx(hdc, 100, 100, &pt);
		::LineTo(hdc, 200, 300);
		::LineTo(hdc, 400, 400);



		TCHAR lpszTime[10];

		_stprintf_s(lpszTime, _countof(lpszTime), _T("%02d:%02d:%02d"), 
			st.wHour, st.wMinute, st.wSecond);

		::DrawText(hdc, lpszTime, -1, &rc, DT_CENTER);


		// 해제
		::SelectObject(hdc, oldBrush);
		::DeleteObject(RedBrush);

		::SelectObject(hdc, oldPen);
		::DeleteObject(GreenPen);

		::EndPaint(hWnd, &ps);
	}
	else if (uMsg == WM_TIMER)
	{
		::GetLocalTime(&st);

		RECT rc;
		::GetClientRect(hWnd, &rc);

		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
