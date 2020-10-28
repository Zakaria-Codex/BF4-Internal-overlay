#include "render.h"
#include "SDK.h"
#include "CSX.h"
#include "Configuration.hpp"
#include "direct3d.h"
#include "Color.h"
#include "Gui/Gui.h"

#define checkSetting(teamval, enemyval) (((gm.LocalPlayer.Team == player.Team) && teamval) || ((gm.LocalPlayer.Team != player.Team) && enemyval))
#define pasteColor(x) x.r, x.g, x.b
#define pasteColorA(x) x.r, x.g, x.b, x.a
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);

static class Text
{
public:
	std::string text;
	int side;
	int size;
	D3DXCOLOR color;
	Text(std::string text, int side, int size, D3DXCOLOR color) : text(text), side(side),  size(size), color(color) {}
};
namespace Render {


#pragma region utils draw

	void DrawAABB(Manager gm, AxisAlignedBox aabb, D3DXVECTOR3 Position, float Yaw, D3DXCOLOR color, float thickness)
	{
		
		float cosY = (float)cos(Yaw);
		float sinY = (float)sin(Yaw);

		D3DXVECTOR3 fld = D3DXVECTOR3(aabb.Min.z * cosY - aabb.Min.x * sinY, aabb.Min.y, aabb.Min.x * cosY + aabb.Min.z * sinY) + Position; // 0
		D3DXVECTOR3 brt = D3DXVECTOR3(aabb.Min.z * cosY - aabb.Max.x * sinY, aabb.Min.y, aabb.Max.x * cosY + aabb.Min.z * sinY) + Position; // 1
		D3DXVECTOR3 bld = D3DXVECTOR3(aabb.Max.z * cosY - aabb.Max.x * sinY, aabb.Min.y, aabb.Max.x * cosY + aabb.Max.z * sinY) + Position; // 2
		D3DXVECTOR3 frt = D3DXVECTOR3(aabb.Max.z * cosY - aabb.Min.x * sinY, aabb.Min.y, aabb.Min.x * cosY + aabb.Max.z * sinY) + Position; // 3
		D3DXVECTOR3 frd = D3DXVECTOR3(aabb.Max.z * cosY - aabb.Min.x * sinY, aabb.Max.y, aabb.Min.x * cosY + aabb.Max.z * sinY) + Position; // 4
		D3DXVECTOR3 brb = D3DXVECTOR3(aabb.Min.z * cosY - aabb.Min.x * sinY, aabb.Max.y, aabb.Min.x * cosY + aabb.Min.z * sinY) + Position; // 5
		D3DXVECTOR3 blt = D3DXVECTOR3(aabb.Min.z * cosY - aabb.Max.x * sinY, aabb.Max.y, aabb.Max.x * cosY + aabb.Min.z * sinY) + Position; // 6
		D3DXVECTOR3 flt = D3DXVECTOR3(aabb.Max.z * cosY - aabb.Max.x * sinY, aabb.Max.y, aabb.Max.x * cosY + aabb.Max.z * sinY) + Position; // 7

		if (!gm.WorldToScreen(fld) || !gm.WorldToScreen(brt)
			|| !gm.WorldToScreen(bld) || !gm.WorldToScreen(frt)
			|| !gm.WorldToScreen(frd) || !gm.WorldToScreen(brb)
			|| !gm.WorldToScreen(blt) || !gm.WorldToScreen(flt))
			return;

		Renderer::GetInstance()->RenderLine(ImVec2(fld.x, fld.y), ImVec2(brt.x, brt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(brb.x, brb.y), ImVec2(blt.x, blt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(fld.x, fld.y), ImVec2(brb.x, brb.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(brt.x, brt.y), ImVec2(blt.x, blt.y),color, thickness);

		Renderer::GetInstance()->RenderLine(ImVec2(frt.x, frt.y), ImVec2(bld.x, bld.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frd.x, frd.y), ImVec2(flt.x, flt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frt.x, frt.y), ImVec2(frd.x, frd.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(bld.x, bld.y), ImVec2(flt.x, flt.y),color, thickness);

		Renderer::GetInstance()->RenderLine(ImVec2(frt.x, frt.y), ImVec2(fld.x, fld.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frd.x, frd.y), ImVec2(brb.x, brb.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(brt.x, brt.y), ImVec2(bld.x, bld.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(blt.x, blt.y), ImVec2(flt.x, flt.y),color, thickness);
		
	}
	void VinDrawAABB(Manager gm, AxisAlignedBox aabb, D3DXMATRIXA16 tranform, D3DXCOLOR color, float thickness)
	{
	
		D3DXVECTOR3 Position = D3DXVECTOR3(tranform._41, tranform._42, tranform._43);
		D3DXVECTOR3 fld = Multiply(D3DXVECTOR3(aabb.Min.x, aabb.Min.y, aabb.Min.z), tranform) + Position;
		D3DXVECTOR3 brt = Multiply(D3DXVECTOR3(aabb.Max.x, aabb.Max.y, aabb.Max.z), tranform) + Position;
		D3DXVECTOR3 bld = Multiply(D3DXVECTOR3(aabb.Min.x, aabb.Min.y, aabb.Max.z), tranform) + Position;
		D3DXVECTOR3 frt = Multiply(D3DXVECTOR3(aabb.Max.x, aabb.Max.y, aabb.Min.z), tranform) + Position;
		D3DXVECTOR3 frd = Multiply(D3DXVECTOR3(aabb.Max.x, aabb.Min.y, aabb.Min.z), tranform) + Position;
		D3DXVECTOR3 brb = Multiply(D3DXVECTOR3(aabb.Max.x, aabb.Min.y, aabb.Max.z), tranform) + Position;
		D3DXVECTOR3 blt = Multiply(D3DXVECTOR3(aabb.Min.x, aabb.Max.y, aabb.Max.z), tranform) + Position;
		D3DXVECTOR3 flt = Multiply(D3DXVECTOR3(aabb.Min.x, aabb.Max.y, aabb.Min.z), tranform) + Position;

		if (!gm.WorldToScreen(fld) || !gm.WorldToScreen(brt)
			|| !gm.WorldToScreen(bld) || !gm.WorldToScreen(frt)
			|| !gm.WorldToScreen(frd) || !gm.WorldToScreen(brb)
			|| !gm.WorldToScreen(blt) || !gm.WorldToScreen(flt))
			return;

		Renderer::GetInstance()->RenderLine(ImVec2(fld.x, fld.y), ImVec2(flt.x, flt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(flt.x, flt.y), ImVec2(frt.x, frt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frt.x, frt.y), ImVec2(frd.x, frd.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frd.x, frd.y), ImVec2(fld.x, fld.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(bld.x, bld.y), ImVec2(blt.x, blt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(blt.x, blt.y), ImVec2(brt.x, brt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(brt.x, brt.y), ImVec2(brb.x, brb.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(brb.x, brb.y), ImVec2(bld.x, bld.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(fld.x, fld.y), ImVec2(bld.x, bld.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frd.x, frd.y), ImVec2(brb.x, brb.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(flt.x, flt.y), ImVec2(blt.x, blt.y),color, thickness);
		Renderer::GetInstance()->RenderLine(ImVec2(frt.x, frt.y), ImVec2(brt.x, brt.y),color, thickness);
		
	}

	void DrawBoneHead(Manager gm, D3DXVECTOR3 from, D3DXCOLOR color)
	{
		D3DXVECTOR3 W2S_from = from;
		if (!gm.WorldToScreen(W2S_from)) return;
		Renderer::GetInstance()->RenderCircle(ImVec2(W2S_from.x, W2S_from.y), 4, color, g_Options.ESP.ESP_SETTINGS.espski);
	}
	void DrawBone(Manager gm, D3DXVECTOR3 from, D3DXVECTOR3 to, D3DXCOLOR color)
	{
		D3DXVECTOR3 W2S_from = from;
		if (!gm.WorldToScreen(W2S_from)) return;
		D3DXVECTOR3  W2S_to = to;
		if (!gm.WorldToScreen(W2S_to)) return;
		Renderer::GetInstance()->RenderLine(ImVec2(W2S_from.x, W2S_from.y), ImVec2(W2S_to.x, W2S_to.y), color, g_Options.ESP.ESP_SETTINGS.espski);
		Renderer::GetInstance()->RenderCircleFilled(ImVec2(W2S_from.x, W2S_from.y), 2, color);
	}
	void DrawSkeletonESP(Manager gm, Player pEnemySoldier, D3DXCOLOR Color)
	{
		D3DXVECTOR3 BonePosition[12];
		int BoneID[12] = {
						 BONE_HEAD,				//00
						 BONE_LEFTHAND,			//01
						 BONE_RIGHTHAND,		//02
						 BONE_RIGHTFOOT,		//03
						 BONE_RIGHTKNEEROLL,	//04
						 BONE_LEFTKNEEROLL,		//05
						 BONE_LEFTFOOT,			//06
						 BONE_RIGHTSHOULDER,	//07
						 BONE_LEFTSHOULDER,		//08
						 BONE_SPINE,			//09
						 BONE_RIGHTELBOWROLL,	//10
						 BONE_LEFTELBOWROLL		//11
		};
		for (int i = 0; i < 12; i++)
		{
			if (!pEnemySoldier.GetBoneById(BoneID[i], &BonePosition[i])) return;
		}
		DrawBoneHead(gm,BonePosition[0], Color);
		DrawBone(gm, BonePosition[0], BonePosition[9], Color);
		DrawBone(gm, BonePosition[9], BonePosition[5], Color);
		DrawBone(gm, BonePosition[5], BonePosition[6], Color);
		DrawBone(gm, BonePosition[9], BonePosition[4], Color);
		DrawBone(gm, BonePosition[4], BonePosition[3], Color);
		DrawBone(gm, BonePosition[7], BonePosition[8], Color);
		DrawBone(gm, BonePosition[7], BonePosition[10], Color);
		DrawBone(gm, BonePosition[10], BonePosition[2], Color);
		DrawBone(gm, BonePosition[8], BonePosition[11], Color);
		DrawBone(gm, BonePosition[11], BonePosition[1], Color);
	}

#pragma endregion
#pragma region utils
	inline BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = FALSE)
	{
		LPTSTR part;
		char tmp[MAX_PATH];
		char name[MAX_PATH];

		HANDLE hSearch = NULL;
		WIN32_FIND_DATA wfd;
		memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

		if (bInnerFolders)
		{
			if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
			strcpy(name, part);
			strcpy(part, "*.*");
			wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
			if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
				do
				{
					if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
						continue;

					if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						char next[MAX_PATH];
						if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
						strcpy(part, wfd.cFileName);
						strcat(next, "\\");
						strcat(next, name);

						SearchFiles(next, lpSearchFunc, TRUE);
					}
				} while (FindNextFile(hSearch, &wfd));
				FindClose(hSearch);
		}

		if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
			return TRUE;
		do
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char file[MAX_PATH];
				if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
				strcpy(part, wfd.cFileName);

				lpSearchFunc(wfd.cFileName);
			}
		while (FindNextFile(hSearch, &wfd));
		FindClose(hSearch);
		return TRUE;
	}
	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.ini"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}
	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = "C:\\BF4_HAX\\*.ini";
		CreateDirectoryW(L"C:\\BF4_HAX", NULL);
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}
	static D3DXVECTOR3 RotatePoint(D3DXVECTOR3 EntityPos, D3DXVECTOR3 LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck)
	{
		float zs = LocalPlayerPos.z - EntityPos.z;
		float xs = LocalPlayerPos.x - EntityPos.x;
		double Yaw = -(double)angle;
		float single = xs * (float)cos(Yaw) - zs * (float)sin(Yaw);
		float ypisilum1 = xs * (float)sin(Yaw) + zs * (float)cos(Yaw);
		*viewCheck = ypisilum1 < 0;
		single *= zoom;
		ypisilum1 *= zoom;
		single = single * 2.f;
		single = single + (float)(posX + sizeX / 2);
		ypisilum1 = ypisilum1 * 2.f;
		ypisilum1 = ypisilum1 + (float)(posY + sizeY / 2);
		if (single < (float)posX)
			single = (float)posX;
		if (ypisilum1 < (float)posY)
			ypisilum1 = (float)posY;
		if (single > (float)(posX + sizeX - 3))
			single = (float)(posX + sizeX - 3);
		if (ypisilum1 > (float)(posY + sizeY - 3))
			ypisilum1 = (float)(posY + sizeY - 3);


		return D3DXVECTOR3(single, ypisilum1, 0);
	}
#pragma endregion
#pragma region menu utils
	static const char* sidebar_tabs[] = {
	"Screen",
	"Aimbot",
	"Visual",
	"Misc",
	"Colors",
	"Settings",
	"",
	};
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float& flRainbow)
	{
		ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

		Color::Color colColor(0, 0, 0, 255);
		flRainbow += flSpeed;
		if (flRainbow > 1.f) flRainbow = 0.f;
		for (int i = 0; i < width; i++)
		{
			float hue = (1.f / (float)width) * i;
			hue -= flRainbow;
			if (hue < 0.f) hue += 1.f;

			Color::Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
			windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
		}
	}

	template<size_t N>
	void render_tabs(const char* (&names)[N], int& activetab, float w, float h, bool sameline)
	{
		bool values[N] = { false };

		values[activetab] = true;

		for (auto i = 0u; i < N; ++i) {
			if (ImGui::Button(names[i], ImVec2{ w, h })) {
				activetab = i;
			}
			if (sameline && i < N - 1)
				ImGui::SameLine();
		}
	}
	int get_fps()
	{
		using namespace std::chrono;
		static int count = 0;
		static auto last = high_resolution_clock::now();
		auto now = high_resolution_clock::now();
		static int fps = 0;

		count++;

		if (duration_cast<milliseconds>(now - last).count() > 1000) {
			fps = count;
			count = 0;
			last = now;
		}

		return fps;
	}
	constexpr static float get_sidebar_item_width() { return 150.0f; }
	constexpr static float get_sidebar_item_height() { return  50.0f; }
	ImVec2 get_sidebar_size()
	{
		constexpr float padding = 10.0f;
		constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
		constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();

		return ImVec2{ size_w, size_h };
	}
	const char* HitBoxesToScan[] =
	{
		"Head",
		"Neck",
		"Spine",
	};
	const char* EspBoxTtpe[] =
	{
		"3D",
		"Box",
		"Corners",
	};
	const char* HealthBarType[] =
	{
		"Horizontal Bar",
		"Vertical Bar",
	};
	const char* SkeletonType[] =
	{
		"Only Visible",
		"None",
	};
	const char* EspvehicleType[] =
	{
		"Esp DMG Color",
		"Esp Only",
	};
	const char* CrosshairType[] =
	{
		"Dot",
		"Plus",
		"Cross",
		"Gap",
		"Diagonal",
		"Swastika",
		"Off"
	};
	void screen()
	{
		ImGui::Checkbox(XorStr("Active"), &g_Options.SCREEN.screen);
		ImGui::Separator();
		ImGui::BeginChild(XorStr("#Screen"), ImVec2(200, 100), true);
		{
			ImGui::Text(XorStr("Screen"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Players close"), &g_Options.SCREEN.enemyclose);
			ImGui::Checkbox(XorStr("Total Enemy"), &g_Options.SCREEN.totalenemy);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("#ScreenOptions"), ImVec2(200, 100), true);
		{
			ImGui::Text(XorStr("Options"));
			ImGui::Separator();
			ImGui::SliderInt(XorStr("Distance"), &g_Options.SCREEN.discloselayer, 1, 180, "%1");

		}ImGui::EndChild();
	}
	void aimbot()
	{
		ImGui::Checkbox(XorStr("Aimbot Micro"), &g_Options.AIMBOT.aimbot);
		ImGui::Separator();
		ImGui::BeginChild(XorStr("#Genral"), ImVec2(270, 175), true);
		{
			ImGui::Text(XorStr("Genral"));
			ImGui::Separator();
			ImGui::Hotkey(XorStr("AimKey"), &g_Options.AIMBOT.aimkey);
			ImGui::Checkbox(XorStr("AutoLock"), &g_Options.AIMBOT.aimbotauto);
			ImGui::SameLine();
			ImGui::Checkbox(XorStr("Draw Fov"), &g_Options.AIMBOT.drawfov);
			ImGui::SliderFloat(XorStr("FOV##1"), &g_Options.AIMBOT.FOV, 1.f, 180.0f, "%1.0");
			ImGui::SliderFloat(XorStr("Smooth"), &g_Options.AIMBOT.aimsmooth, 1.0f, 50.0f, "%1.0");
			ImGui::SliderFloat(XorStr("Lock"), &g_Options.AIMBOT.Lock, 0.0f, 3.0f, "%0.1");

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("#Accuracy"), ImVec2(270, 175), true);
		{
			ImGui::Text(XorStr("Accuracy"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("AimBot Team"), &g_Options.AIMBOT.aimbotteam);
			ImGui::Checkbox(XorStr("AimBot Enemy"), &g_Options.AIMBOT.aimbotenemy);
			ImGui::Checkbox(XorStr("No Recoil"), &g_Options.AIMBOT.norecoil);
			ImGui::Checkbox(XorStr("No Spread"), &g_Options.AIMBOT.nospread);
			ImGui::SliderFloat(XorStr("Control Recoil"), &g_Options.AIMBOT.recoilControler, 0.1f, 0.9f, "%0.1");
			ImGui::SliderFloat(XorStr("Control Spread"), &g_Options.AIMBOT.spreadControler, 0.1f, 0.9f, "%0.1");


		}ImGui::EndChild();
		ImGui::BeginChild(XorStr("#HitBox"), ImVec2(270, 150), true);
		{
			ImGui::Text(XorStr("HitBox"));
			ImGui::Separator();
			ImGui::Combo(XorStr("HitBox"), &g_Options.AIMBOT.hitboxbone, HitBoxesToScan, ARRAYSIZE(HitBoxesToScan));
			ImGui::Checkbox(XorStr("Draw Hitbox##1"), &g_Options.AIMBOT.drawhitbox);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("#angles"), ImVec2(270, 150), true);
		{
			ImGui::Text(XorStr("Angles"));
			ImGui::Separator();
			ImGui::SliderFloat(XorStr("XAxis"), &g_Options.AIMBOT.bPredict, 0.0f, 1.0f, "%0.1");
			ImGui::SliderFloat(XorStr("YAxis"), &g_Options.AIMBOT.bYAxis, 0.0f, 1.0f, "%0.1");

		}ImGui::EndChild();

	}
	void Visual()
	{
		ImGui::Checkbox(XorStr("Active"), &g_Options.ESP.EnableESP);
		ImGui::Separator();
		if (ImGui::Button(XorStr("ESP PLAYERS"), ImVec2(150, 0)))
		{
			g_Options.MENU.iVisualsSubTab = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorStr("ESP VEHICLE"), ImVec2(150, 0)))
		{
			g_Options.MENU.iVisualsSubTab = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button(XorStr("FONT & SIZE"), ImVec2(150, 0)))
		{
			g_Options.MENU.iVisualsSubTab = 2;
		}
		ImGui::Separator();
		switch (g_Options.MENU.iVisualsSubTab)
		{
		case 0:
		{
			ImGui::BeginChild(XorStr("#ESP_TEAM"), ImVec2(270, 120), true);
			{
				ImGui::Text(XorStr("ESP TEAM"));
				ImGui::Separator();

				ImGui::Checkbox(XorStr("Box"), &g_Options.ESP.ESP_TEAM.teamBoxes);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Name"), &g_Options.ESP.ESP_TEAM.teamNames);
				ImGui::Checkbox(XorStr("HP"), &g_Options.ESP.ESP_TEAM.teamHealth);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Aimlines"), &g_Options.ESP.ESP_TEAM.teamSnaplines);
				ImGui::Checkbox(XorStr("Skeleton"), &g_Options.ESP.ESP_TEAM.teamSkeleton);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Fill-Box"), &g_Options.ESP.ESP_TEAM.teamfilledbox);
				ImGui::Checkbox(XorStr("Distance"), &g_Options.ESP.ESP_TEAM.teamDistance);
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("#ESP_ENEMY"), ImVec2(270, 120), true);
			{
				ImGui::Text(XorStr("ESP ENEMY"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Box"), &g_Options.ESP.ESP_ENEMY.enemyBoxes);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Name"), &g_Options.ESP.ESP_ENEMY.enemyNames);
				ImGui::Checkbox(XorStr("HP"), &g_Options.ESP.ESP_ENEMY.enemyHealth);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Aimlines"), &g_Options.ESP.ESP_ENEMY.enemySnaplines);
				ImGui::Checkbox(XorStr("Skeleton"), &g_Options.ESP.ESP_ENEMY.enemySkeleton);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Fill-Box"), &g_Options.ESP.ESP_ENEMY.enemyfilledbox);
				ImGui::Checkbox(XorStr("Distance"), &g_Options.ESP.ESP_ENEMY.enemyDistance);
			}ImGui::EndChild();
			ImGui::BeginChild(XorStr("#ESP_OPTIONS"), ImVec2(270, 180), true);
			{
				ImGui::Text(XorStr("ESP SETTINGS"));
				ImGui::Separator();
				ImGui::Combo(XorStr("Box"), &g_Options.ESP.EspBoxType, EspBoxTtpe, ARRAYSIZE(EspBoxTtpe));
				ImGui::Combo(XorStr("Health "), &g_Options.ESP.EspHealthBarType, HealthBarType, ARRAYSIZE(HealthBarType));
				ImGui::Combo(XorStr("Skeleton"), &g_Options.ESP.visSkeletontype, SkeletonType, ARRAYSIZE(SkeletonType));
				ImGui::SliderInt(XorStr("Distance##1"), &g_Options.ESP.distanceToPlayer, 1, 700, "%0");
				ImGui::SliderFloat(XorStr("Filled Box Alpha"), &g_Options.ESP.filledboxAlpha, 0.0f, 1.f, "%0");
				ImGui::Checkbox(XorStr("OutLine"), &g_Options.ESP.esp_Outline);

			}ImGui::EndChild();			
			break;
		}
		case 1:
		{
			ImGui::BeginChild(XorStr("#VEHICLE_TEAM"), ImVec2(270, 140), true);
			{
				ImGui::Text(XorStr("VEHICLE TEAM"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Box"), &g_Options.ESP.VEHICLE_TEAM.teamVehicle);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Name"), &g_Options.ESP.VEHICLE_TEAM.teamVehicleName);
				ImGui::Checkbox(XorStr("DMG"), &g_Options.ESP.VEHICLE_TEAM.teamVehicledmg);
			}ImGui::EndChild();
			ImGui::SameLine();
			ImGui::BeginChild(XorStr("#VEHICLE_ENEMY"), ImVec2(270, 140), true);
			{
				ImGui::Text(XorStr("VEHICLE ENEMY"));
				ImGui::Separator();
				ImGui::Checkbox(XorStr("Box"), &g_Options.ESP.VEHICLE_ENEMY.enemyVehicle);
				ImGui::SameLine(150);
				ImGui::Checkbox(XorStr("Name"), &g_Options.ESP.VEHICLE_ENEMY.enemyVehicleName);
				ImGui::Checkbox(XorStr("DMG"), &g_Options.ESP.VEHICLE_ENEMY.enemyVehicledmg);
			}ImGui::EndChild();
			ImGui::BeginChild(XorStr("#ESPVEHICLE_OPTIONS"), ImVec2(270, 140), true);
			{
				ImGui::Combo(XorStr("Esp Type"), &g_Options.ESP.ESPVehicleMode, EspvehicleType, ARRAYSIZE(EspvehicleType));
			}ImGui::EndChild();
			break;
		}
		case 2:
		{
			ImGui::Text(XorStr("ESP BOX Size"));
			ImGui::SliderFloat(XorStr("##BOX"), &g_Options.ESP.ESP_SETTINGS.espbox, 1, 20, "%0.1");
			ImGui::Separator();
			ImGui::Text(XorStr("ESP Line Size"));
			ImGui::SliderFloat(XorStr("##Lin"), &g_Options.ESP.ESP_SETTINGS.espline, 1, 20, "%0.1");
			ImGui::Separator();
			ImGui::Text(XorStr("ESP Skeleton Size"));
			ImGui::SliderFloat(XorStr("##Skeleton"), &g_Options.ESP.ESP_SETTINGS.espski, 1, 20, "%0.1");
			ImGui::Separator();
			ImGui::Text(XorStr("ESP Name Size"));
			ImGui::SliderInt(XorStr("##Name"), &g_Options.ESP.ESP_SETTINGS.espname, 1, 20, "%1");
			ImGui::Separator();
			ImGui::Text(XorStr("ESP Distance Size"));
			ImGui::SliderInt(XorStr("##Distance"), &g_Options.ESP.ESP_SETTINGS.espdis, 1, 20, "%1");
			ImGui::Separator();
			ImGui::Text(XorStr("ESP Vehicle Name Size"));
			ImGui::SliderInt(XorStr("##VehicleN"), &g_Options.ESP.ESP_SETTINGS.espvinname, 1, 20, "%0.1");
			ImGui::Separator();
			ImGui::Text(XorStr("ESP Vehicle Box Size"));
			ImGui::SliderFloat(XorStr("##VehicleB"), &g_Options.ESP.ESP_SETTINGS.espvinbox, 1, 20, "%0.1");
			break;
		}
		default:
			break;
		}
	}
	void Misc()
	{
		ImGui::BeginChild(XorStr("#RADAR"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("Radar Options"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Radar Window"), &g_Options.MISC.RADAR.radarwindow);
			ImGui::SliderFloat(XorStr("Radar Zoom"), &g_Options.MISC.RADAR.radrzoom, 0.f, 4.f, "Zoom: %.2f");
			ImGui::SliderFloat(XorStr("Radar Alpha"), &g_Options.MISC.RADAR.radralpha, 0.f, 1.f, "Alpha: %.2f");
			ImGui::SliderInt(XorStr("Distance ##1"), &g_Options.MISC.RADAR.distanceToPlayer, 0, 700, "%0");
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Radar Team"), &g_Options.MISC.RADAR.PLAYERS.Team);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Radar Enemy"), &g_Options.MISC.RADAR.PLAYERS.Enemy);
			ImGui::Checkbox(XorStr("Team Vehicle"), &g_Options.MISC.RADAR.VEHICLE.TeamVehicle);
			ImGui::SameLine(150);
			ImGui::Checkbox(XorStr("Enemy Vehicle"), &g_Options.MISC.RADAR.VEHICLE.EnemyVehicle);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("#Crosshair"), ImVec2(270, 250), true);
		{
			ImGui::Text(XorStr("Crosshair Options"));
			ImGui::Separator();
			ImGui::Combo(XorStr("Type"), &g_Options.MISC.crosshair.crosshair, CrosshairType, ARRAYSIZE(CrosshairType));
			ImGui::SliderFloat(XorStr("Size"), &g_Options.MISC.crosshair.size, 0.f, 10.f);
			ImGui::SliderFloat(XorStr("Stroke"), &g_Options.MISC.crosshair.stroke, 0.f, 10.f);

		}ImGui::EndChild();

		ImGui::BeginChild(XorStr("#Options"), ImVec2(270, 100), true);
		{
			ImGui::Text(XorStr("Exstra Options"));
			ImGui::Separator();
			ImGui::Checkbox(XorStr("Spectator List"), &g_Options.MISC.SpecList);

		}ImGui::EndChild();
	}
	void Colors()
	{
		auto& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);
		static int selectedItem = 0;
		ImVec2 lastCursor = ImGui::GetCursorPos();
		ImGui::ListBoxHeader("##0", ImVec2(240, 330));
		for (int i = 0; i < Globals::ColorsForPicker1.size(); i++)
		{
			bool selected = i == selectedItem;
			if (ImGui::Selectable(Globals::ColorsForPicker1[i].Name, selected))
				selectedItem = i;
		}
		ImGui::ListBoxFooter();
		ImGui::SameLine(0.0f, 15.0f);
		ImGui::BeginChild(XorStr("#generalcolors"), ImVec2(285, 330), false, ImGuiWindowFlags_NoScrollbar);
		{
			style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);
			ImGui::BeginChild(XorStr("#ColorsChild"), ImVec2(285, 125), true);
			{
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);
				ImGui::Spacing();
				ImGui::Spacing();
				ColorP col = Globals::ColorsForPicker1[selectedItem];
				int r = (col.Ccolor[0] * 255.f);
				int g = (col.Ccolor[1] * 255.f);
				int b = (col.Ccolor[2] * 255.f);
				ImGui::PushItemWidth(250);
				ImGui::SliderInt(XorStr("##red"), &r, 0, 255, "%.0f"); ImGui::SameLine(); ImGui::Text("R");
				ImGui::Spacing();
				ImGui::SliderInt(XorStr("##green"), &g, 0, 255, "%.0f"); ImGui::SameLine(); ImGui::Text("G");
				ImGui::Spacing();
				ImGui::SliderInt(XorStr("##blue"), &b, 0, 255, "%.0f"); ImGui::SameLine(); ImGui::Text("B");
				ImGui::PopItemWidth();
				col.Ccolor[0] = r / 255.0f;
				col.Ccolor[1] = g / 255.0f;
				col.Ccolor[2] = b / 255.0f;
			}ImGui::EndChild();
			ImGui::BeginChild(XorStr("#colorpreview"), ImVec2(285, 201), true);
			{
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);
				ColorP col = Globals::ColorsForPicker1[selectedItem];
				ImGui::ColorPicker(XorStr("##COLOR_PICKER"), 250, 185, col.Ccolor);
			}ImGui::EndChild();
		}ImGui::EndChild();

	}
	
	void Settings()
	{
		static int iConfigSelect = 0;
		static int iMenuSheme = 1;
		static char ConfigName[64] = { 0 };
		ImGui::BeginChild(XorStr("first child"), ImVec2(547, 120), true);
		{
			ImGui::ComboBoxArray(XorStr("Select Config"), &iConfigSelect, ConfigList);
			ImGui::Separator();
			if (ImGui::Button(XorStr("Refresh Config")))
			{
				RefreshConfigs();
			}	
			ImGui::SameLine();
			if (ImGui::Button(XorStr("Save Config")))
			{
				SaveSettings("C:\\BF4_HAX\\" + ConfigList[iConfigSelect]);
			}
			ImGui::SameLine();
			if (ImGui::Button(XorStr("Load Config")))
			{
				LoadSettings("C:\\BF4_HAX\\" + ConfigList[iConfigSelect]);
			}
			ImGui::Separator();
			ImGui::InputText(XorStr("Config Name"), ConfigName, 64);
			if (ImGui::Button(XorStr("Create  Config")))
			{
				string ConfigFileName = ConfigName;
						
				if (ConfigFileName.size() < 1)
				{
					ConfigFileName = "";
				}
				if (ConfigFileName != "") {
					SaveSettings("C:\\BF4_HAX\\" + ConfigFileName + ".ini");
					RefreshConfigs();
				}
			}

			ImGui::Separator();
		}ImGui::EndChild();

		ImGui::BeginChild(XorStr("#VSync"), ImVec2(180, 50), true);
		{
			ImGui::Checkbox(XorStr("VSync"), &g_Options.MENU.o_VSync);
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild(XorStr("#Style"), ImVec2(180, 50), true);
		{
			const char* ThemesList[] = { "None","Purple" , "DefaultSheme1" , "RedSheme" , "Darkblue" , "MidNightSheme" , "NightSheme" , "DunnoSheme" , "BlueSheme"  , "MidNight2" , "BlackSheme2" , "Green" , "Pink" , "Blue" , "Yellow" , "BlackSheme" };

			ImGui::Combo(XorStr("Style"), &g_Options.MENU.Theme, ThemesList, IM_ARRAYSIZE(ThemesList));
		}ImGui::EndChild();
	}
#pragma endregion

	void Crosshair()
	{
		switch (g_Options.MISC.crosshair.crosshair)
		{
		case Renderer::CrosshairStyle::Dot:
			Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Dot, Globals::rWidth / 2, Globals::rHeight / 2, g_Options.MISC.crosshair.size, g_Options.MISC.crosshair.stroke, D3DXCOLOR(g_Options.COLOR.CrossHair[0], g_Options.COLOR.CrossHair[1], g_Options.COLOR.CrossHair[2], 255.f));
			break;
		case Renderer::CrosshairStyle::Plus:
			Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Plus, Globals::rWidth / 2, Globals::rHeight / 2, g_Options.MISC.crosshair.size, g_Options.MISC.crosshair.stroke, D3DXCOLOR(g_Options.COLOR.CrossHair[0], g_Options.COLOR.CrossHair[1], g_Options.COLOR.CrossHair[2], 255.f));
			break;
		case Renderer::CrosshairStyle::Cross:
			Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Cross, Globals::rWidth / 2, Globals::rHeight / 2, g_Options.MISC.crosshair.size, g_Options.MISC.crosshair.stroke, D3DXCOLOR(g_Options.COLOR.CrossHair[0], g_Options.COLOR.CrossHair[1], g_Options.COLOR.CrossHair[2], 255.f));
			break;
		case Renderer::CrosshairStyle::Gap:
			Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Gap, Globals::rWidth / 2, Globals::rHeight / 2, g_Options.MISC.crosshair.size, g_Options.MISC.crosshair.stroke, D3DXCOLOR(g_Options.COLOR.CrossHair[0], g_Options.COLOR.CrossHair[1], g_Options.COLOR.CrossHair[2], 255.f));
			break;
		case Renderer::CrosshairStyle::Diagonal:
			Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Diagonal, Globals::rWidth / 2, Globals::rHeight / 2, g_Options.MISC.crosshair.size, g_Options.MISC.crosshair.stroke, D3DXCOLOR(g_Options.COLOR.CrossHair[0], g_Options.COLOR.CrossHair[1], g_Options.COLOR.CrossHair[2], 255.f));
			break;			
		case Renderer::CrosshairStyle::Swastika:
			Renderer::GetInstance()->DrawCrosshair(Renderer::CrosshairStyle::Swastika, Globals::rWidth / 2, Globals::rHeight / 2, g_Options.MISC.crosshair.size, g_Options.MISC.crosshair.stroke, D3DXCOLOR(g_Options.COLOR.CrossHair[0], g_Options.COLOR.CrossHair[1], g_Options.COLOR.CrossHair[2], 255.f));
			break;
		case 6:
			break;
		default:
			break;
		}
	}
	void memory()
	{
		if (Mem::IsValid((void*)Mem::ReadPtr<DWORD_PTR>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 }, true)))
		{
			float recoil = Mem::ReadPtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x430 }, true);
			float recoilHip = Mem::ReadPtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x438 }, true);
			if (g_Options.AIMBOT.norecoil)
			{
				if (recoil != g_Options.AIMBOT.recoilControler && recoilHip != g_Options.AIMBOT.recoilControler)
				{
					
					Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x430 }, g_Options.AIMBOT.recoilControler, true);
					Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x438 }, g_Options.AIMBOT.recoilControler, true);
				}
			}
			else
			{
				
					if (recoil != 1 && recoilHip != 1)
					{
						Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x430 }, 1, true);
						Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x438 }, 1, true);
					}
				
			}

			float spread = Mem::ReadPtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x434 }, true);
			float spreadHip = Mem::ReadPtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x43C }, true);
			if (g_Options.AIMBOT.nospread)
			{
				if (spread != g_Options.AIMBOT.spreadControler && spreadHip != g_Options.AIMBOT.spreadControler)
				{
					Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x434 }, g_Options.AIMBOT.spreadControler, true);
					Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x43C }, g_Options.AIMBOT.spreadControler, true);
				}
			}
			else
			{
				
					if (spread != 1 && spreadHip != 1)
					{
						Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x434 }, 1, true);
						Mem::WritePtr<float>({ OFFSET_LOCALWEAPONS ,0x128, 0x30 ,0x43C }, 1, true);
					}
				
			}
			
		}
	}
	void Runaimbot()
	{
		Manager gm(Globals::rWidth, Globals::rHeight);

			if ((Inputs::keydown(g_Options.AIMBOT.aimkey) || g_Options.AIMBOT.aimbotauto)) {
				D3DXVECTOR3 BONE = D3DXVECTOR3();
				float YAxis = g_Options.AIMBOT.bYAxis - 0.5f;
				float XAxis = g_Options.AIMBOT.bPredict - 0.5f;
				std::vector<int> HitBoxesToScan{};
				switch (g_Options.AIMBOT.hitboxbone)
				{
				case 0:
					HitBoxesToScan.push_back(BONE_HEAD);
					break;
				case 1:
					HitBoxesToScan.push_back(BONE_NECK);
					break;
				case 2:
					HitBoxesToScan.push_back(BONE_SPINE);
					break;
				default:
					break;
				}
				for (auto hitbox : HitBoxesToScan)
				{
					if (hitbox != -1)
					{

						if (gm.ClosestCrosshairEnemy.GetBoneById(hitbox, &BONE)) {

							D3DXVECTOR3 Hitbox = gm.CalcSoldierFuturePos(D3DXVECTOR3(BONE.x, BONE.y - YAxis, BONE.z - XAxis));
							if (GetCrossDistance(Hitbox.x, Hitbox.y, Globals::rWidth / 2, Globals::rHeight / 2) <= g_Options.AIMBOT.FOV * 8)
							{
								auto roundPos = D3DXVECTOR2((float)round(Hitbox.x), (float)round(Hitbox.y));
								AimAtPosV2(roundPos.x, roundPos.y, g_Options.AIMBOT.aimsmooth, g_Options.AIMBOT.Lock, false);
								if (g_Options.AIMBOT.drawhitbox) {
									Renderer::GetInstance()->RenderCircleFilled(ImVec2(roundPos.x, roundPos.y), 5, D3DXCOLOR(255, 0, 0, 255));
									Renderer::GetInstance()->RenderCircle(ImVec2(roundPos.x, roundPos.y), 5, D3DXCOLOR(255, 255, 0, 255), 3);
								}
							}

						}

					}
				}
			}

			if (g_Options.AIMBOT.drawfov)
				Renderer::GetInstance()->RenderCircle(ImVec2(Globals::rWidth / 2, Globals::rHeight / 2), g_Options.AIMBOT.FOV * 8, D3DXCOLOR(255, 255, 255, 255));
	}

	void DrawVisuals() {
		Manager gm(Globals::rWidth, Globals::rHeight);
		for (Player& player : gm.Players) {
			D3DXCOLOR Playerbox;
			D3DXCOLOR Playerline;
			D3DXCOLOR PlayerSkeleton;
			D3DXCOLOR PlayerFiledBox;
			D3DXCOLOR PlayerDistance;
			D3DXCOLOR PlayerName;
			//
			D3DXCOLOR teamColor = D3DXCOLOR(g_Options.COLOR.TeamColor[0], g_Options.COLOR.TeamColor[1], g_Options.COLOR.TeamColor[2], 255.f);
			D3DXCOLOR occludedEnemyColor = D3DXCOLOR(g_Options.COLOR.OccludedEnemyColor[0], g_Options.COLOR.OccludedEnemyColor[1], g_Options.COLOR.OccludedEnemyColor[2], 255.f);
			D3DXCOLOR UnVisibleEnemyColor = D3DXCOLOR(g_Options.COLOR.VisibleEnemyColor[0], g_Options.COLOR.VisibleEnemyColor[1], g_Options.COLOR.VisibleEnemyColor[2], 255.f);

			D3DXCOLOR TeamSkeleton = D3DXCOLOR(g_Options.COLOR.TeamSkeleton[0], g_Options.COLOR.TeamSkeleton[1], g_Options.COLOR.TeamSkeleton[2], 255.f);
			D3DXCOLOR VisibleSkeleton = D3DXCOLOR(g_Options.COLOR.VisSkeleton[0], g_Options.COLOR.VisSkeleton[1], g_Options.COLOR.VisSkeleton[2], 255.f);
			D3DXCOLOR UnVisibleSkeleton = D3DXCOLOR(g_Options.COLOR.UnVisSkeleton[0], g_Options.COLOR.UnVisSkeleton[1], g_Options.COLOR.UnVisSkeleton[2], 255.f);

			D3DXCOLOR LineTeam = D3DXCOLOR(g_Options.COLOR.TeamLineColor[0], g_Options.COLOR.TeamLineColor[1], g_Options.COLOR.TeamLineColor[2], 255.f);
			D3DXCOLOR VisibleLineColor = D3DXCOLOR(g_Options.COLOR.VisLineColor[0], g_Options.COLOR.VisLineColor[1], g_Options.COLOR.VisLineColor[2], 255.f);
			D3DXCOLOR UnVisLineColor = D3DXCOLOR(g_Options.COLOR.UnVisLineColor[0], g_Options.COLOR.UnVisLineColor[1], g_Options.COLOR.UnVisLineColor[2], 255.f);

			D3DXCOLOR FiledBoxTeam = D3DXCOLOR(g_Options.COLOR.TeamFiledboxColor[0], g_Options.COLOR.TeamFiledboxColor[1], g_Options.COLOR.TeamFiledboxColor[2], g_Options.ESP.filledboxAlpha);
			D3DXCOLOR VisibleFiledBoxColor = D3DXCOLOR(g_Options.COLOR.VisFiledboxColor[0], g_Options.COLOR.VisFiledboxColor[1], g_Options.COLOR.VisFiledboxColor[2], g_Options.ESP.filledboxAlpha);
			D3DXCOLOR UnVisibleFiledBoxColor = D3DXCOLOR(g_Options.COLOR.UnVisFiledboxColor[0], g_Options.COLOR.UnVisFiledboxColor[1], g_Options.COLOR.UnVisFiledboxColor[2], g_Options.ESP.filledboxAlpha);

			D3DXCOLOR TeamDistanceColor = D3DXCOLOR(g_Options.COLOR.TeamDistanceColor[0], g_Options.COLOR.TeamDistanceColor[1], g_Options.COLOR.TeamDistanceColor[2], 255.f);
			D3DXCOLOR VisibleDistanceColor = D3DXCOLOR(g_Options.COLOR.VisDistanceColor[0], g_Options.COLOR.VisDistanceColor[1], g_Options.COLOR.VisDistanceColor[2], 255.f);
			D3DXCOLOR UnVisibleDistanceColor = D3DXCOLOR(g_Options.COLOR.UnVisDistanceColor[0], g_Options.COLOR.UnVisDistanceColor[1], g_Options.COLOR.UnVisDistanceColor[2], 255.f);

			D3DXCOLOR TeamNameColor = D3DXCOLOR(g_Options.COLOR.TeamNameColor[0], g_Options.COLOR.TeamNameColor[1], g_Options.COLOR.TeamNameColor[2], 255.f);
			D3DXCOLOR VisibleNameColor = D3DXCOLOR(g_Options.COLOR.VisNameColor[0], g_Options.COLOR.VisNameColor[1], g_Options.COLOR.VisNameColor[2], 255.f);
			D3DXCOLOR UnVisibleNameColor = D3DXCOLOR(g_Options.COLOR.UnVisNameColor[0], g_Options.COLOR.UnVisNameColor[1], g_Options.COLOR.UnVisNameColor[2], 255.f);
			//////
			if (player.isSpectator == true)
			{
				Renderer::GetInstance()->DrawBox((S_width / 2) - 30, 170, 220, 50, D3DXCOLOR(1, 0, 0, 0.5));
				Renderer::GetInstance()->RenderText(font, "Spectator Watch You ", ImVec2(S_width / 2, 180),20, D3DXCOLOR( 1.f, 0.f, 0.f, 1.f));
			}
				if (player.Health > NULL) {

					if (player.Team == gm.LocalPlayer.Team) {
						Playerbox = teamColor;
						Playerline = LineTeam;
						PlayerSkeleton = TeamSkeleton;
						PlayerFiledBox = FiledBoxTeam;
						PlayerDistance = TeamDistanceColor;
						PlayerName = TeamNameColor;
					}
					else {
						if (player.Visible) {
							Playerbox = occludedEnemyColor;
							Playerline = VisibleLineColor;
							PlayerSkeleton = VisibleSkeleton;
							PlayerFiledBox = VisibleFiledBoxColor;
							PlayerDistance = VisibleDistanceColor;
							PlayerName = VisibleNameColor;
						}
						else {
							Playerbox = UnVisibleEnemyColor;
							Playerline = UnVisLineColor;
							PlayerSkeleton = UnVisibleSkeleton;
							PlayerFiledBox = UnVisibleFiledBoxColor;
							PlayerDistance = UnVisibleDistanceColor;
							PlayerName = UnVisibleNameColor;
						}
					}

					D3DXVECTOR3 textPos = D3DXVECTOR3(player.Location.x, player.Location.y, player.Location.z);
					D3DXVECTOR3 footLocation = textPos;
					D3DXVECTOR3 headLocation = footLocation;
					if (player.PoseType == 0) headLocation.y += 1.6;
					if (player.PoseType == 1) headLocation.y += 1;
					if (player.PoseType == 2) headLocation.y += .5;
					if (gm.WorldToScreen(footLocation) && gm.WorldToScreen(headLocation)) {
						float w2sHeight = Distance3D(footLocation, headLocation);
						float w2sWidth = w2sHeight;
						float heightoffset = Distance3D(footLocation, headLocation);
						float H = w2sHeight;
						float W = H / 2;
						float X = headLocation.x - W / 2;
						std::vector< Text > texts;

						if (player.PoseType == 0) w2sWidth /= 2;
						if (player.PoseType == 1) w2sWidth /= 1.5;
						float distanceToPlayer = Distance3D(gm.LocalPlayer.Location, player.Location);

						if (distanceToPlayer > g_Options.ESP.distanceToPlayer)
							continue;

						int iHpAmY = 1;
						if checkSetting(g_Options.ESP.ESP_TEAM.teamBoxes, g_Options.ESP.ESP_ENEMY.enemyBoxes) {

							if (g_Options.ESP.EspBoxType == 0)
							{
								DrawAABB(gm, player.GetAABB(), textPos, player.Yaw, Playerbox, g_Options.ESP.ESP_SETTINGS.espbox);
							}
							else if (g_Options.ESP.EspBoxType == 1)
							{
								if (!g_Options.ESP.esp_Outline)
									Renderer::GetInstance()->DrawBox(X, headLocation.y, W, H, Playerbox, g_Options.ESP.ESP_SETTINGS.espbox);
								else if (g_Options.ESP.esp_Outline)
									Renderer::GetInstance()->DrawOutlineBox(X, headLocation.y, W, H, Playerbox, g_Options.ESP.ESP_SETTINGS.espbox);

							}
							else
							{
								if (!g_Options.ESP.esp_Outline)
									Renderer::GetInstance()->DrawCoalBox(X, headLocation.y, W, H, Playerbox, g_Options.ESP.ESP_SETTINGS.espbox);
								else if (g_Options.ESP.esp_Outline)
									Renderer::GetInstance()->DrawOutlineCoalBox(X, headLocation.y, W, H, Playerbox, g_Options.ESP.ESP_SETTINGS.espbox);

							}
						}
						if (checkSetting(g_Options.ESP.ESP_TEAM.teamSnaplines, g_Options.ESP.ESP_ENEMY.enemySnaplines)) {
							Renderer::GetInstance()->RenderLine(ImVec2(Globals::rWidth / 2, 0), ImVec2(headLocation.x, headLocation.y), Playerline);
						}
						if (checkSetting(g_Options.ESP.ESP_TEAM.teamNames, g_Options.ESP.ESP_ENEMY.enemyNames)) {
							char txt[1024];
							sprintf(txt, "%s", player.Name);
							texts.push_back(Text(txt, 0, g_Options.ESP.ESP_SETTINGS.espname, PlayerName));
						}
						if (checkSetting(g_Options.ESP.ESP_TEAM.teamDistance, g_Options.ESP.ESP_ENEMY.enemyDistance)) {
							char txt[1024];
							sprintf(txt, "%0.f m", distanceToPlayer);
							texts.push_back(Text(txt, 1, g_Options.ESP.ESP_SETTINGS.espdis, PlayerDistance));
						}
						if (checkSetting(g_Options.ESP.ESP_TEAM.teamHealth, g_Options.ESP.ESP_ENEMY.enemyHealth)) {
							if (g_Options.ESP.EspHealthBarType == 0)
								Renderer::GetInstance()->HorizontalDrawHealth(X, headLocation.y, W, 3, (int)player.Health, (int)player.MaxHealth, D3DXCOLOR(.48f, .98f, 0.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
							else
								Renderer::GetInstance()->VerticalHealthBar(X - 6, headLocation.y, 3, H, (int)player.Health, (int)player.MaxHealth, D3DXCOLOR(.48f, .98f, 0.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
						}
						if (checkSetting(g_Options.ESP.ESP_TEAM.teamSkeleton, g_Options.ESP.ESP_ENEMY.enemySkeleton)) {
							if (g_Options.ESP.visSkeletontype == 0)
							{
								if (player.Visible)
									DrawSkeletonESP(gm, player, PlayerSkeleton);
							}
							else
							{
								DrawSkeletonESP(gm, player, PlayerSkeleton);
							}
						}
						if (checkSetting(g_Options.ESP.ESP_TEAM.teamfilledbox, g_Options.ESP.ESP_ENEMY.enemyfilledbox)) {
							Renderer::GetInstance()->DrawBoxFilled((int)X, (int)headLocation.y, (int)W, (int)H, PlayerFiledBox);
						}

						int middle[3] = { footLocation.y,footLocation.y, footLocation.x - footLocation.y };
						int Top[3] = { headLocation.y,headLocation.y, headLocation.x - headLocation.y };
						for (auto text : texts)
						{
							int fontsize = text.size;
							float offset = (text.text.length() * fontsize) / 5;
							switch (text.side)
							{
							case 0:
								Renderer::GetInstance()->RenderText(font, (char*)text.text.c_str(), ImVec2(headLocation.x - offset, Top[1]- (heightoffset / 4) - fontsize), text.size, text.color);
								Top[1] -= fontsize;
								break;
							case 1:
								Renderer::GetInstance()->RenderText(font, (char*)text.text.c_str(), ImVec2(footLocation.x - offset, middle[1]), text.size, text.color);
								middle[1] += fontsize;
								break;
							case 2:
								Renderer::GetInstance()->RenderText(font, (char*)text.text.c_str(), ImVec2(headLocation.x + (heightoffset / 2), Top[1]), text.size, text.color);
								Top[1] += fontsize;
								break;
							}
						}


					}
				}
				if (player.InVehicle)
				{
					if (player.VehicleHealth > NULL)
					{
						D3DXCOLOR VehicleColor = D3DXCOLOR(g_Options.COLOR.VehicleColor[0], g_Options.COLOR.VehicleColor[1], g_Options.COLOR.VehicleColor[2], 255.f);
						D3DXCOLOR VehicleNameColor = D3DXCOLOR(g_Options.COLOR.VehicleNameColor[0], g_Options.COLOR.VehicleNameColor[1], g_Options.COLOR.VehicleNameColor[2], 255.f);

						D3DXVECTOR3 Position = D3DXVECTOR3(player.VehicleTranfsorm._41, player.VehicleTranfsorm._42, player.VehicleTranfsorm._43);
						D3DXVECTOR3 PositionEnd = Position;
						PositionEnd.y += 3;
						float distanceToPlayer = Distance3D(gm.LocalPlayer.Location, player.Location);
						if (checkSetting(g_Options.ESP.VEHICLE_TEAM.teamVehicle, g_Options.ESP.VEHICLE_ENEMY.enemyVehicle)) {
							if (g_Options.ESP.ESPVehicleMode == 0) {
								float perc = (player.VehicleHealth / player.VehicleMaxHealth);
								D3DXCOLOR HealthColor = D3DXCOLOR(1 - perc, perc, 0, 1);
								VinDrawAABB(gm, player.VehicleAABB, player.VehicleTranfsorm, HealthColor, g_Options.ESP.ESP_SETTINGS.espvinbox);
							}
							else
							{
								VinDrawAABB(gm, player.VehicleAABB, player.VehicleTranfsorm, VehicleColor, g_Options.ESP.ESP_SETTINGS.espvinbox);
							}
						}
						if (gm.WorldToScreen(Position) && gm.WorldToScreen(PositionEnd))
						{
							float w2sHeight = Distance3D(Position, PositionEnd);
							float w2sWidth = w2sHeight;
							float heightoffset = Distance3D(Position, PositionEnd);
							float H = w2sHeight;
							float W = H / 2;
							float X = PositionEnd.x - W / 2;
							string vname = player.VehicleName;
							if (checkSetting(g_Options.ESP.VEHICLE_TEAM.teamVehicleName, g_Options.ESP.VEHICLE_ENEMY.enemyVehicleName)) {
								int fontsize = g_Options.ESP.ESP_SETTINGS.espvinname;
								float offset = (vname.length() * fontsize) / 5;
								char txt[1024];
								sprintf(txt, "%s", vname.c_str());
								Renderer::GetInstance()->RenderText(font, txt, ImVec2(PositionEnd.x - offset, PositionEnd.y - (heightoffset / 4) - fontsize), fontsize, VehicleNameColor);
							}
							if (checkSetting(g_Options.ESP.VEHICLE_TEAM.teamVehicledmg, g_Options.ESP.VEHICLE_ENEMY.enemyVehicledmg)) {
								Renderer::GetInstance()->HorizontalDrawHealth(X, PositionEnd.y - 10, W, 3, player.VehicleHealth, player.VehicleMaxHealth, D3DXCOLOR(0, 1, 0, 1), D3DXCOLOR(1, 0, 0, 1));
							}
						}
					}
				}
		}
	}
	void DrawRadars()
	{
		auto& style = ImGui::GetStyle();
		style.Alpha = 1.f;
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1, 0.1f, 1.f);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Appearing);
		
		if (ImGui::Begin(XorStr("Radar"), &g_Options.MISC.RADAR.radarwindow, ImGuiWindowFlags_NoCollapse))
		{
			ImVec2 siz = ImGui::GetContentRegionAvail();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			float RadarCenterX = pos.x + (siz.x / 2);
			float RadarCenterY = pos.y + (siz.y / 2);
			Renderer::GetInstance()->RenderRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + siz.x, pos.y + siz.y), ImGui::GetColorU32(ImVec4(100, 100, 100, 255)), 0.0F, -1, 1.5f);
			Renderer::GetInstance()->RenderLine(ImVec2(RadarCenterX, RadarCenterY), ImVec2(pos.x, pos.y), ImGui::GetColorU32(ImVec4(100, 100, 100, 100)), 1.f);
			Renderer::GetInstance()->RenderLine(ImVec2(RadarCenterX, RadarCenterY), ImVec2(pos.x + siz.x, pos.y), ImGui::GetColorU32(ImVec4(100, 100, 100, 100)), 1.f);
			Renderer::GetInstance()->RenderLine(ImVec2(pos.x, RadarCenterY), ImVec2(pos.x + siz.x, RadarCenterY), ImGui::GetColorU32(ImVec4(100, 100, 100, 100)), 1.f);
			Renderer::GetInstance()->RenderLine(ImVec2(RadarCenterX, RadarCenterY), ImVec2(RadarCenterX, pos.y + siz.y), ImGui::GetColorU32(ImVec4(100, 100, 100, 100)), 1.f);
			Renderer::GetInstance()->RenderCircleFilled(ImVec2(RadarCenterX, RadarCenterY), 2, ImGui::GetColorU32(ImVec4(100, 100, 100, 100)));
			Manager gm(Globals::rWidth, Globals::rHeight);
			for (Player& player : gm.Players) {
				D3DXCOLOR RadarPlayer;
				D3DXCOLOR RadarTeam = D3DXCOLOR(g_Options.COLOR.RadarTeam[0], g_Options.COLOR.RadarTeam[1], g_Options.COLOR.RadarTeam[2], 255);
				D3DXCOLOR VisRadarEnemy = D3DXCOLOR(g_Options.COLOR.VisRadarEnemy[0], g_Options.COLOR.VisRadarEnemy[1], g_Options.COLOR.VisRadarEnemy[2], 255);
				D3DXCOLOR UnVisRadarEnemy = D3DXCOLOR(g_Options.COLOR.UnVisRadarEnemy[0], g_Options.COLOR.UnVisRadarEnemy[1], g_Options.COLOR.UnVisRadarEnemy[2], 255);
				
					if (player.Health > NULL && player.Location != NULL) {
						if (player.Team == gm.LocalPlayer.Team) {
							RadarPlayer = RadarTeam;
						}
						else {
							if (player.Visible) {
								RadarPlayer = VisRadarEnemy;
							}
							else {
								RadarPlayer = UnVisRadarEnemy;
							}
						}
						float distanceToPlayer = Distance3D(gm.LocalPlayer.Location, player.Location);
						if (distanceToPlayer > g_Options.MISC.RADAR.distanceToPlayer)
							continue;
						if (checkSetting(g_Options.MISC.RADAR.PLAYERS.Team, g_Options.MISC.RADAR.PLAYERS.Enemy)) {

							bool viewCheck = false;
							D3DXVECTOR3 EntityPos = RotatePoint(player.Location, gm.LocalPlayer.Location, pos.x, pos.y, siz.x, siz.y, gm.LocalPlayer.Yaw, g_Options.MISC.RADAR.radrzoom, &viewCheck);
							int s = 5;
							Renderer::GetInstance()->RenderCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, D3DXCOLOR(RadarPlayer.r, RadarPlayer.g, RadarPlayer.b, RadarPlayer.a));
						}

					}
					if (player.InVehicle)
					{
						if (player.VehicleHealth > NULL && player.VehicleTranfsorm != NULL)
						{
							D3DXVECTOR3 Position = D3DXVECTOR3(player.VehicleTranfsorm._41, player.VehicleTranfsorm._42, player.VehicleTranfsorm._43);
							if (checkSetting(g_Options.MISC.RADAR.VEHICLE.TeamVehicle, g_Options.MISC.RADAR.VEHICLE.EnemyVehicle)) {
								bool viewCheck = false;
								D3DXVECTOR3 EntityPos = RotatePoint(Position, gm.LocalPlayer.Location, pos.x, pos.y, siz.x, siz.y, gm.LocalPlayer.Yaw, g_Options.MISC.RADAR.radrzoom, &viewCheck);
								int s = 5;
								Renderer::GetInstance()->RenderCircleFilled(ImVec2(EntityPos.x, EntityPos.y), s, D3DXCOLOR(g_Options.COLOR.VehicleRadar[0], g_Options.COLOR.VehicleRadar[1], g_Options.COLOR.VehicleRadar[2], 255));
							}
						}
					}
			}
			
		}ImGui::End();
		
	}

	void DrawingSpectatorList()
	{
		Manager gm(Globals::rWidth, Globals::rHeight);
		int specs = 0;
		int modes = 0;
		std::string spect = "";
		std::string mode = "";
			for (Player& player : gm.Players) {
				if (gm.SpectatorList.Name !="") {
					spect += gm.SpectatorList.Name;
					spect += "\n";
					specs++;

					mode += "Spectator";
					mode += "\n";
					modes++;
				}
			}
			if (ImGui::Begin(XorStr("Spectator List"), &g_Options.MISC.SpecList, ImVec2(0, 0), 0.4F, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
			{
				if (specs > 0) spect += "\n";
				if (modes > 0) mode += "\n";
				ImVec2 size = ImGui::CalcTextSize(spect.c_str());
				ImGui::SetWindowSize(ImVec2(200, 50 + size.y));
				ImGui::Columns(2);
				ImGui::Separator();

				ImGui::Text("Name");
				ImGui::NextColumn();

				ImGui::Text("Mode");
				ImGui::NextColumn();
				ImGui::Separator();

				ImGui::Text(spect.c_str());
				ImGui::NextColumn();

				ImGui::Text(mode.c_str());
				ImGui::Columns(1);
				ImGui::Separator();
			}
			ImGui::End();
	}
	void DrawClosedPlayer()
	{
		Manager gm(Globals::rWidth, Globals::rHeight);
		int closed = gm.ClosestVisibleEnemy.Distance;
		if (closed < g_Options.SCREEN.discloselayer) {
			if (ImGui::Begin(XorStr("Closed Player"), &g_Options.SCREEN.enemyclose, ImVec2(220, 50), 0.4F, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
			{
				ImVec2 siz = ImGui::GetContentRegionAvail();
				ImVec2 pos = ImGui::GetCursorScreenPos();
				Renderer::GetInstance()->RenderRect(ImVec2(pos.x - 4, pos.y - 5), ImVec2((pos.x + siz.x) + 4, (pos.y + siz.y) + 5), ImGui::GetColorU32(ImVec4(255, 0, 0, 255)));
				Renderer::GetInstance()->RenderText(font, "Players Closed " + std::to_string(closed) + " m", ImVec2(pos.x + 38, pos.y + 5), 20, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));


			}
			ImGui::End();
		}
	}
	void DrawTotalPlayer()
	{
		Manager gm(Globals::rWidth, Globals::rHeight);
		int Total = gm.Total;
		if (Total > 0) {
			if (ImGui::Begin(XorStr("Total Player"), &g_Options.SCREEN.totalenemy, ImVec2(220, 50), 0.4F, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
			{
				ImVec2 siz = ImGui::GetContentRegionAvail();
				ImVec2 pos = ImGui::GetCursorScreenPos();
				Renderer::GetInstance()->RenderRect(ImVec2(pos.x - 4, pos.y - 5), ImVec2((pos.x + siz.x) + 4, (pos.y + siz.y) + 5), ImGui::GetColorU32(ImVec4(255, 0, 0, 255)));
				Renderer::GetInstance()->RenderText(font, "Total Players " + std::to_string(Total - 1), ImVec2(pos.x + 38, pos.y + 5), 20, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));


			}
			ImGui::End();
		}
	}
	void DrawMenu() {
		//ImGui Frame
		auto& style = ImGui::GetStyle();
		style.Alpha = 1.f;
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1, 0.1f, 1.f);
		ImGui::SetNextWindowPos(ImVec2(500, 500), ImGuiSetCond_Appearing);
		const auto sidebar_size = get_sidebar_size();
		static int active_sidebar_tab = 0;
		Renderer::GetInstance()->DrawBoxFilled(0, 0, Globals::rWidth, Globals::rHeight, D3DXCOLOR(0.f, 0.f, 0.f, 0.70f));
		if (ImGui::Begin(XorStr("Sadar-Hax"), &Globals::Opened, ImVec2(760, 430), 0.98f, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			static float flRainbow;
			float flSpeed = 0.0003f;
			int curWidth = 1;
			ImVec2 curPos = ImGui::GetCursorPos();
			ImVec2 curWindowPos = ImGui::GetWindowPos();
			curPos.x += curWindowPos.x;
			curPos.y += curWindowPos.y;
			DrawRectRainbow(curPos.x, curPos.y * -1, ImGui::GetWindowSize().x + Globals::rWidth, curPos.y * 1.0, flSpeed, flRainbow);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
			{
				ImGui::BeginGroupBox(XorStr("##sidebar"), sidebar_size);
				{
					ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

					render_tabs(sidebar_tabs, active_sidebar_tab, get_sidebar_item_width(), get_sidebar_item_height(), false);
				}
				ImGui::EndGroupBox();
			}
			ImGui::PopStyleVar();
			ImGui::SameLine();
			auto size = ImVec2{ 0.0f, sidebar_size.y };
			ImGui::BeginGroupBox(XorStr("##body"), size);
			if (active_sidebar_tab == 0) {
				screen();
			}
			if (active_sidebar_tab == 1) {
				aimbot();
			}
			else if (active_sidebar_tab == 2) {
				Visual();
			}
			else if (active_sidebar_tab == 3) {
				Misc();
			}
			else if (active_sidebar_tab == 4) {
				Colors();
			}
			else if (active_sidebar_tab == 5) {
				Settings();
			}
			ImGui::EndGroupBox();
			ImGui::TextColored(ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f }, "FPS: %03d", get_fps());
			ImGui::End();
		}

	}
}

