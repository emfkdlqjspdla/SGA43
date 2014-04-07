#pragma once

#include "..\MainWindow\sga.hpp"

class AnimationApp : public MainWindow<AnimationApp>
{
	typedef AnimationApp Me;
	typedef MainWindow<AnimationApp> Base;
public :
	AnimationApp()
	{
	}
	~AnimationApp()
	{
	}
	void Update(DWORD tick)
	{
	}
	void Render(DWORD tick)
	{
	}
protected :
	void SetEventMap()
	{
		AddEventHandler(WM_CREATE, &Me::OnCreate);
		AddEventHandler(WM_DESTROY, &Me::OnDestroy);
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::PostQuitMessage(0);
		return 0;
	}
};
