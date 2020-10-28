
#include "SetupColor.h"
#include "KeyMan.h"
#include "direct3d.h"
HWND enumWindow = NULL;
void mainLoop() {

	SetupColors();
	for (;;) {
		MSG uMessage;
		uMessage.message = WM_NULL;
		ShowWindow(Globals::hWnd, 1);
		UpdateWindow(Globals::hWnd);
		SetLayeredWindowAttributes(Globals::hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
		if (uMessage.message != WM_QUIT)
		{
			if (PeekMessage(&uMessage, Globals::hWnd, 0, 0, PM_REMOVE)) {
				DispatchMessage(&uMessage);
				TranslateMessage(&uMessage);
			}
			UpdateWindow(Globals::hWnd);
			WINDOWINFO info;
			ZeroMemory(&info, sizeof(info));
			info.cbSize = sizeof(info);
			GetWindowInfo(Globals::tWnd, &info);	
			RECT rekt;
			GetClientRect(Globals::hWnd, &rekt);
			Globals::rWidth = ((rekt.right) - (rekt.left));
			Globals::rHeight = ((rekt.bottom) - (rekt.top));
			if (!IsIconic(Globals::hWnd)) {
				SetWindowPos(Globals::hWnd, HWND_TOPMOST, info.rcClient.left, info.rcClient.top, ((info.rcClient.right) - (info.rcClient.left)), ((info.rcClient.bottom) - (info.rcClient.top)), SWP_SHOWWINDOW);
			}
			hookkeys();		
		}
		
		Sleep(1);
	}
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uiMessage)
	{
	case WM_PAINT:
		D3DRender();
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
	return 0;
}

void d2oSetup(HWND tWindow) {
	Globals::tWnd = tWindow;
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = "d3do";
	if (!RegisterClass(&wc))
	{
		MessageBox(Globals::tWnd, "Failed Register Class", "Fatal Error", MB_ICONERROR);
		return;
	}
	Globals::hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, wc.lpszClassName, "D3D Overlay", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);
	MARGINS mar = { -1 };
	DwmExtendFrameIntoClientArea(Globals::hWnd, &mar);
	D3DInitialize(Globals::hWnd);
}

DWORD WINAPI OverlayThread(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	printf("[BFV]Hooked By Zakaria!\n");
	printf("[github]www.github.com/Zakaria-Codex\n");
	printf("[Discord]Zakaria#1077\n");
	printf("[unknowncheats]www.unknowncheats.me/forum/members/1930085.html\n");
	printf("fix aimbot disable Raw mouse input in game \n");
	printf("inject before join any server \n");
	enumWindow = FindWindow(0, "Battlefield 4");
	d2oSetup(enumWindow);
		mainLoop();
		return true;
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {

	g_hModule = hinstDLL;
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OverlayThread, lpReserved, NULL, NULL);
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		UpdateWindow(Globals::hWnd);
	}
	return TRUE;
}