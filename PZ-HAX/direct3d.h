#pragma once

#include "Includes.h"
//Resources
static IDirect3DDevice9Ex* pDevice;
static IDirect3D9Ex* pInstance;

//DX Stuff
void D3DRender();
DWORD D3DInitialize(HWND hWnd);
void hookkeys();
static HMODULE					g_hModule = nullptr;

class Renderer
{
public:
	static Renderer* GetInstance();
private:
	Renderer();
	~Renderer();
public:
	enum CrosshairStyle
	{
		Dot,
		Plus,
		Cross,
		Gap,
		Diagonal,
		Swastika
	};

	float RenderText(ImFont* m_pFont,const std::string& text, const ImVec2& position, float size, uint32_t color, bool center = false);
	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness = 1.0f);
	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness = 1.0f);
	void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color);
	void RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = 0xF, float thickness = 1.0f);
	void RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding = 0.0f, uint32_t roundingCornersFlags = 0xF);
	void HorizontalDrawHealth(int x, int y, int w, int h, float Health, float MaxHealth, D3DXCOLOR color1, D3DXCOLOR color2);
	void VerticalHealthBar(int x, int y, int w, int h, float Health, float MaxHealth, D3DXCOLOR color1, D3DXCOLOR color2);
	void DrawBox(float X, float Y, float W, float H, D3DCOLOR Color, float thickness = 1.0f);
	void DrawBoxFilled(float X, float Y, float W, float H, D3DCOLOR Color);
	void DrawOutlineBox(int x, int y, int w, int h, D3DCOLOR color, float thickness = 1.0f);
	void DrawCoalBox(int x, int y, int w, int h, D3DCOLOR color, float thickness = 1.0f);
	void DrawOutlineCoalBox(int x, int y, int w, int h, D3DCOLOR color, float thickness = 1.0f);
	void DrawWindow(int x, int y, int w, int h, D3DCOLOR Border, D3DCOLOR Background);

	void DrawCrosshair(CrosshairStyle style, float x, float y, float size, float stroke, D3DCOLOR color);
private:
	static Renderer* m_pInstance;
};