#include "direct3d.h"
#include "KeyMan.h"
#include "InputHook.h"
#include "Gui/Gui.h"
#include "globals.h"

#define DELETE_MOD(name) delete name; name = nullptr

//Direct3D Resources
 D3DPRESENT_PARAMETERS pParams;
time_t preTime = clock();

static std::once_flag           g_isInitialized;
CGui* g_pGui = nullptr;
std::string BaseDir = "";
std::string LogFile = "";
std::string GuiFile = "";
std::string IniFile = "";
std::vector<std::string> ConfigList;
InputHook inputHook = InputHook();

void ToggleMenu() {
	Globals::Opened = !Globals::Opened;

}

DWORD D3DInitialize(HWND hWnd) {
	ZeroMemory(&pParams, sizeof(pParams));
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &pInstance)))
	{
		MessageBox(Globals::tWnd, "Failed to create D3D", "Fatal Error", MB_ICONERROR);
		return E_FAIL;
	}
	pParams.Windowed = TRUE;
	pParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pParams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	pParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	pParams.hDeviceWindow = hWnd;
	pParams.EnableAutoDepthStencil = TRUE;
	pParams.AutoDepthStencilFormat = D3DFMT_D16;
	pParams.BackBufferCount = 1;
	pParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	pParams.BackBufferWidth = Globals::rWidth;
	pParams.BackBufferHeight = Globals::rHeight;
	pParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // Must be 0 for windowed.
	if (FAILED(pInstance->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pParams, 0, &pDevice))) {
		MessageBox(Globals::tWnd, "Failed to create device", "Fatal Error", MB_ICONERROR);
		return E_FAIL;
	}
	g_pGui = new CGui();
	std::call_once(g_isInitialized, [&]() {
		g_pGui->GUI_Init(Globals::hWnd, pDevice);
		inputHook.Init(Globals::tWnd);
		});

	return S_OK;
}
void hookkeys()
{
	if (KeyMan::ReadKeyOnce(Keys::showMenuKey.keyCode)) {
		ToggleMenu();
	}
	InputHook::menuEnabled = Globals::Opened;
}
void theme()
{
	switch (g_Options.MENU.Theme)
	{
	case 0:
		g_pGui->RedSheme();
		break;
	case 1:
		g_pGui->purple();
		break;
	case 2:
		g_pGui->DefaultSheme1();
		break;
	case 3:
		g_pGui->RedSheme();
		break;
	case 4:
		g_pGui->darkblue();
		break;
	case 5:
		g_pGui->MidNightSheme();
		break;
	case 6:
		g_pGui->NightSheme();
		break;
	case 7:
		g_pGui->DunnoSheme();
		break;
	case 8:
		g_pGui->BlueSheme();
		break;
	case 9:
		g_pGui->MidNight2();
		break;
	case 10:
		g_pGui->BlackSheme2();
		break;
	case 11:
		g_pGui->green();
		break;
	case 12:
		g_pGui->pink();
		break;
	case 13:
		g_pGui->blue();
		break;
	case 14:
		g_pGui->yellow();
		break;
	case 15:
		g_pGui->BlackSheme();
		break;
	default:
		break;
	}
}
void D3DRender() {
	int vsyncA = clock();
	pParams.BackBufferHeight = Globals::rHeight;
	pParams.BackBufferWidth = Globals::rWidth;
	pDevice->Reset(&pParams);
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	pDevice->BeginScene();
	if (Globals::tWnd == GetForegroundWindow() || GetActiveWindow() == GetForegroundWindow()) {
		if (g_pGui)
			g_pGui->GUI_Draw_Elements();
		g_pGui->MenuColor();
		theme();
	}
	pDevice->EndScene();
	pDevice->PresentEx(0, 0, 0, 0, 0);
	if (g_Options.MENU.o_VSync) {
		int pausetime = 15 - (clock() - vsyncA);
		if (pausetime > 0 && pausetime < 30) {
			Sleep(pausetime);
		}
	}
}
Renderer* Renderer::m_pInstance;
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}
float Renderer::RenderText(ImFont* m_pFont, const std::string& text, const ImVec2& position, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = m_pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->DrawList->AddText(m_pFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}
		else
		{
			window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());
			window->DrawList->AddText(m_pFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, a / 255.0f }), line.c_str());

			window->DrawList->AddText(m_pFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), line.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void Renderer::RenderLine(const ImVec2& from, const ImVec2& to,uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), thickness);
}

void Renderer::RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircle(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), radius, thickness);
}

void Renderer::RenderCircleFilled(const ImVec2& position, float radius, uint32_t color)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xff;
	float r = (color >> 16) & 0xff;
	float g = (color >> 8) & 0xff;
	float b = (color) & 0xff;

	window->DrawList->AddCircleFilled(position, radius, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), radius);
}

void Renderer::RenderRect(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddRect(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags, thickness);
}

void Renderer::RenderRectFilled(const ImVec2& from, const ImVec2& to, uint32_t color, float rounding, uint32_t roundingCornersFlags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float a = (color >> 24) & 0xFF;
	float r = (color >> 16) & 0xFF;
	float g = (color >> 8) & 0xFF;
	float b = (color) & 0xFF;

	window->DrawList->AddRectFilled(from, to, ImGui::GetColorU32({ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f }), rounding, roundingCornersFlags);
}


void Renderer::HorizontalDrawHealth(int x, int y, int w, int h, float Health, float MaxHealth, D3DXCOLOR color1, D3DXCOLOR color2)
{
	if (Health <= 0)
		Health = 1;
	if (MaxHealth < Health)
		MaxHealth = 100;

	int size = (w * Health / (MaxHealth / 100)) / 100;
	DrawBox(x, y, w, h, D3DXCOLOR(0.f, 0.f, 0.f, 1.f),1.f);
	this->DrawBoxFilled(x + 1, y + 1, w - 2, h - 2, color2);

	if (size == w)
		this->DrawBoxFilled(x + 1, y + 1, size - 2, h - 2, color1);
	else
		this->DrawBoxFilled(x + 1, y + 1, size, h - 2, color1);
}

void Renderer::VerticalHealthBar(int x, int y, int w, int h, float Health, float MaxHealth, D3DXCOLOR color1, D3DXCOLOR color2)
{
	if (Health <= 0)
		Health = 1;

	if (MaxHealth < Health)
		MaxHealth = 100;
	int size = (h * Health / (MaxHealth / 100)) / 100;

	this->DrawBox(x, y, w, h, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), 1.f);
	this->DrawBoxFilled(x + 1, y + 1, w - 2, h - 2, color2);

	if (size == h)
		this->DrawBoxFilled(x + 1, y + 1 + h - size, w - 2, size - 2, color1);
	else
		this->DrawBoxFilled(x + 1, y + 1 + h - size, w - 2, size, color1);
}
void Renderer::DrawBox(float X, float Y, float W, float H, D3DCOLOR Color,  float thickness)
{

		this->RenderRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), Color, 0.0f, 0xF, thickness);
		this->RenderRect(ImVec2(X, Y), ImVec2(X + W, Y + H), Color, 0.0f, 0xF, thickness);
}
void Renderer::DrawBoxFilled(float X, float Y, float W, float H, D3DCOLOR Color)
{

	this->RenderRectFilled(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), Color, 0.0f, 0xF);
	this->RenderRectFilled(ImVec2(X, Y), ImVec2(X + W, Y + H), Color, 0.0f, 0xF);
}
void Renderer::DrawOutlineBox(int x, int y, int w, int h, D3DCOLOR color, float thickness)
{
	this->DrawBox(x, y, w, h, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);
	this->DrawBox(x + 1, y + 1, w - 2, h - 2, color, thickness);
	this->DrawBox(x + 2, y + 2, w - 4, h - 4, D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);
}

void Renderer::DrawCoalBox(int x, int y, int w, int h, D3DCOLOR color, float thickness)
{
	int iw = w / 4;
	int ih = h / 4;
	// top
	this->RenderLine(ImVec2(x, y), ImVec2(x + iw, y), color, thickness);					// left
	this->RenderLine(ImVec2(x + w - iw, y), ImVec2(x + w, y), color, thickness);			// right
	this->RenderLine(ImVec2(x, y), ImVec2(x, y + ih), color, thickness);					// top left
	this->RenderLine(ImVec2(x + w - 1, y), ImVec2( x + w - 1, y + ih), color, thickness);	// top right
														// bottom
	this->RenderLine(ImVec2(x, y + h), ImVec2(x + iw, y + h), color, thickness);			// left
	this->RenderLine(ImVec2(x + w - iw, y + h), ImVec2(x + w, y + h), color, thickness);	// right
	this->RenderLine(ImVec2(x, y + h - ih), ImVec2(x, y + h), color, thickness);			// bottom left
	this->RenderLine(ImVec2(x + w - 1, y + h - ih), ImVec2( x + w - 1, y + h), color, thickness);	// bottom right
}

void Renderer::DrawOutlineCoalBox(int x, int y, int w, int h, D3DCOLOR color, float thickness)
{
	int iw = w / 4;
	int ih = h / 4;

	DrawCoalBox(x, y, w, h, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	// top left
	this->RenderLine(ImVec2(x + 1, y + 1), ImVec2( x + iw, y + 1), color, thickness);
	this->RenderLine(ImVec2(x + iw, y), ImVec2( x + iw, y + 2), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);
	this->RenderLine(ImVec2(x + 1, y + 1), ImVec2( x + 1, y + ih), color);
	this->RenderLine(ImVec2(x, y + ih), ImVec2( x + 2, y + ih), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);

	// top right
	this->RenderLine(ImVec2(x + w - iw, y + 1), ImVec2( x + w - 1, y + 1), color, thickness);
	this->RenderLine(ImVec2(x + w - iw - 1, y), ImVec2( x + w - iw, y + 2), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);
	this->RenderLine(ImVec2(x + w - 2, y + 1), ImVec2( x + w - 2, y + ih), color, thickness);
	this->RenderLine(ImVec2(x + w - 2, y + ih), ImVec2( x + w, y + ih), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);

	// bottom left
	this->RenderLine(ImVec2(x + 1, y + h - ih), ImVec2( x + 1, y + h), color, thickness);
	this->RenderLine(ImVec2(x, y + h - ih - 1), ImVec2( x + 2, y + h - ih - 1), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);
	this->RenderLine(ImVec2(x + 1, y + h - 1), ImVec2( x + iw, y + h - 1), color, thickness);
	this->RenderLine(ImVec2(x + iw, y + h - 1), ImVec2( x + iw, y + h + 1), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);

	// bottom right
	this->RenderLine(ImVec2(x + w - iw, y + h - 1), ImVec2( x + w - 1, y + h - 1), color, thickness);
	this->RenderLine(ImVec2(x + w - iw - 1, y + h - 1), ImVec2( x + w - iw, y + h + 1), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);
	this->RenderLine(ImVec2(x + w - 2, y + h - ih), ImVec2( x + w - 2, y + h), color, thickness);
	this->RenderLine(ImVec2(x + w - 2, y + h - ih - 1), ImVec2( x + w, y + h - ih - 1), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), thickness);

	this->DrawCoalBox(x + 2, y + 2, w - 4, h - 4, D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
}

void Renderer::DrawWindow(int x, int y, int w, int h, D3DCOLOR Border, D3DCOLOR Background)
{
	DrawBox(x, y, w, h, Border,1.f);
	DrawBoxFilled(x + 1, y + 1, w - 2, h - 2, Background);
}

 void Renderer::DrawCrosshair(CrosshairStyle style, float x, float y, float size, float stroke, D3DCOLOR color)
{
	if (style == CrosshairStyle::Dot)
	{
		this->RenderCircleFilled(ImVec2(x, y), size, color);
	}
	else if (style == CrosshairStyle::Plus)
	{
		this->RenderLine(ImVec2(x - size, y) , ImVec2(x + size, y), color, stroke);
		this->RenderLine(ImVec2(x, y - size), ImVec2(x, y + size), color, stroke);
	}
	else if (style == CrosshairStyle::Cross)
	{
		this->RenderLine(ImVec2(x - size, y - size), ImVec2(x + size, y + size), color, stroke);
		this->RenderLine(ImVec2(x + size, y - size), ImVec2(x - size, y + size), color, stroke);
	}
	else if (style == CrosshairStyle::Gap)
	{
		this->RenderLine(ImVec2(x - size - stroke, y), ImVec2(x - stroke, y), color, stroke);
		this->RenderLine(ImVec2(x + size + stroke, y), ImVec2(x + stroke, y), color, stroke);
		this->RenderLine(ImVec2(x, y - size - stroke), ImVec2(x, y - stroke), color, stroke);
		this->RenderLine(ImVec2(x, y + size + stroke), ImVec2(x, y + stroke), color, stroke);
	}
	else if (style == CrosshairStyle::Diagonal)
	{
		this->RenderLine(ImVec2(x - size, y - size), ImVec2(x + size, y + size), color, stroke);
		this->RenderLine(ImVec2(x + size, y - size), ImVec2(x - size, y + size), color, stroke);
	}
	else if (style == CrosshairStyle::Swastika)
	{
		ImVec2 first =  ImVec2(x - size, y);
		ImVec2 second =  ImVec2(x + size, y);
		ImVec2 third =  ImVec2(x, y - size);
		ImVec2 fourth =  ImVec2(x, y + size);
		ImVec2 haken_1 =  ImVec2(third.x + size, third.y);
		ImVec2 haken_2 =  ImVec2(second.x, second.y + size);
		ImVec2 haken_3 =  ImVec2(fourth.x - size, fourth.y);
		ImVec2 haken_4 =  ImVec2(first.x, first.y - size);
		this->RenderLine(first, second, color, stroke);
		this->RenderLine(third, fourth, color, stroke);
		this->RenderLine(third, haken_1, color, stroke);
		this->RenderLine(second, haken_2, color, stroke);
		this->RenderLine(fourth, haken_3, color, stroke);
		this->RenderLine(first, haken_4, color, stroke);
	}
}

Renderer* Renderer::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Renderer();

	return m_pInstance;
}