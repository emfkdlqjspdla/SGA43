#pragma once

#include "..\MainWindow\MainWindow.h"

class CannonApp : public MainWindow<CannonApp>
{
	typedef CannonApp Me;
	typedef MainWindow<CannonApp> Base;
public :
protected :
	void SetEventMap()
	{
		Base::SetEventMap();

		AddEventHandler(WM_PAINT, &Me::OnPaint);
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RECT rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::DrawText(hdc, _T("Draw Bitmap"), -1, &rc, DT_TOP);


		::EndPaint(hWnd, &ps);

		return 0;
	}
private :

};