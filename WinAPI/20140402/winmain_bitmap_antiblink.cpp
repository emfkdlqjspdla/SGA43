#include "AntiBlinkApp.h"

INT
WINAPI
_tWinMain(HINSTANCE hInst,
		  HINSTANCE,
		  LPTSTR,
		  INT)
{
	AntiBlinkApp app;

	app.Setup(hInst);

	MSG msg;

	DWORD st = ::GetTickCount();
	DWORD dt = 1;

	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

		app.Update(dt);
		app.Render(dt);

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();
	}

	return msg.wParam;
}

