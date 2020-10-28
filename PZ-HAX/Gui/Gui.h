#pragma once

#include "../Includes.h"

#define GUI_KEY_DOWN( KeyNum ) ( GetAsyncKeyState(KeyNum) & 0x1 )
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
extern	std::string BaseDir;
extern std::string LogFile;
extern	std::string GuiFile;
extern	std::string IniFile;
extern	std::vector<std::string> ConfigList;
extern ImFont* tabfont;
extern ImFont* font;
class CGui
{
public:

	CGui();
	~CGui();

	void	GUI_Init(HWND hWnd, IDirect3DDevice9* pDevice );
	void	GUI_Begin_Render();
	void	GUI_End_Render();
	void	GUI_Draw_Elements();

	void	BlackSheme();
	void	RedSheme();
	void	MenuColor();
	void	purple();
	void	DefaultSheme1();
	void    darkblue();
	void    MidNightSheme();
	void    NightSheme();
	void    DunnoSheme();
	void    BlueSheme();
	void    MidNight2();
	void    BlackSheme2();
	void    green();
	void    pink();
	void    blue();
	void    yellow();
};
extern CGui* g_pGui;

namespace ImGui
{
	IMGUI_API bool ComboBoxArray( const char* label , int* currIndex , std::vector<std::string>& values );
	IMGUI_API bool TabLabels( const char **tabLabels , int tabSize , int &tabIndex , int *tabOrder );
}


