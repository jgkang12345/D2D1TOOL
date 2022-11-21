#include "pch.h"
#include "CorePch.h"
#pragma comment(lib, "winmm.lib")
#include "App.h"

int APIENTRY _tWinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstace, LPTSTR _lpCmdLine, int _nCmdShow)
{
	App app;
	app.Init(_hInstance, _T("JGMapToolApp"), _T("JGMapToolApp"), 1000, 800, _nCmdShow);

	return app.Dispatch();
}