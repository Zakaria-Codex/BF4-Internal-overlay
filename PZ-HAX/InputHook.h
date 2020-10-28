#include <Windows.h>

class InputHook
{
public:
	void Init(HWND hWindow);
	void Remove(HWND hWindow);
	static bool menuEnabled;
};
static LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
