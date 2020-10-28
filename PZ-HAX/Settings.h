#pragma once
#include <set>
#include <map>
#include <unordered_map>
#pragma region Settings
struct Variables
{
	Variables()
	{

	}
	struct
	{
		bool screen = false;
		bool totalenemy = false;
		bool enemyclose = false;
		int discloselayer = 500;
	}SCREEN;
	struct 
	{
		bool aimbot = false;
		bool aimbotauto = false;
		bool aimbotteam = false;
		bool aimbotenemy = false;
		bool norecoil = false;
		bool nospread = false;
		float recoilControler = 0;
		float spreadControler = 0;
		int aimkey = 1;
		float bPredict = 0.50f;
		bool drawfov = false;
		float FOV = 0;
		float aimsmooth = 0;
		float bYAxis = 0.50f;
		int hitboxbone = 0;
		bool drawhitbox = true;
		float Lock = 0.0f;
	}AIMBOT;
	struct 
	{
		bool EnableESP = false;
		int EspBoxType = 0;
		bool esp_Outline = false;
		int EspHealthBarType = 0;
		int ESPVehicleMode = 0;
		int lineWidth = 2;
		int distanceToPlayer = 700;
		int visSkeletontype = 0;
		float filledboxAlpha = 0;
		struct 
		{
			bool teamBoxes = false;
			bool teamNames = false;
			bool teamDistance = false;
			bool teamSnaplines = false;
			bool teamHealth = false;
			bool teamSkeleton = false;
			bool teamfilledbox = false;
		}ESP_TEAM;
		struct 
		{
			bool enemyBoxes = false;
			bool enemyNames = false;
			bool enemyDistance = false;
			bool enemySnaplines = false;
			bool enemyHealth = false;
			bool enemySkeleton = false;
			bool enemyfilledbox = false;

		}ESP_ENEMY;
		struct 
		{
			bool teamVehicle = false;
			bool teamVehicleName = false;
			bool teamVehicledmg = false;
		}VEHICLE_TEAM;

		struct 
		{
			bool enemyVehicle = false;
			bool enemyVehicleName = false;
			bool enemyVehicledmg = false;
		}VEHICLE_ENEMY;
		struct
		{
			float espbox = 1;
			float espline = 1;
			float espski = 1;
			float espvinbox = 1;
			int espname = 11;
			int espdis = 11;
			int espvinname = 11;
		}ESP_SETTINGS;
	}ESP;
	struct
	{
		bool SpecList = true;
		struct
		{
			bool radarwindow = false;
			float radralpha = 1.f;
			float radrzoom = 2.f;
			int distanceToPlayer = 700;
			struct
			{
				bool Team = false;
				bool Enemy = false;
			}PLAYERS;
			struct
			{
				bool TeamVehicle = false;
				bool EnemyVehicle = false;
			}VEHICLE;
		}RADAR;
		struct
		{
			int crosshair = 6;
			float size = 0;
			float stroke = 0;
		}crosshair;
	}MISC;
	struct
	{
		int		 currencolorid;
		int		iVisualsSubTab;
		int		iColorsSubTab;
		int		Theme;
		bool o_DrawFPS = false;
		bool o_VSync = false;

	}MENU;
	struct 
	{
		float TeamColor[3] = { 0.f, 0.2f, 1.f };
		float OccludedEnemyColor[3] = { 0.f, 0.2f, 1.f };
		float VisibleEnemyColor[3] = { 0.f, 0.2f, 1.f };

		float TeamLineColor[3] = { 0.f, 0.2f, 1.f };
		float VisLineColor[3] = { 0.f, 0.2f, 1.f };
		float UnVisLineColor[3] = { 0.f, 0.2f, 1.f };

		float TeamSkeleton[3] = { 0.f, 0.2f, 1.f };
		float VisSkeleton[3] = { 0.f, 0.2f, 1.f };
		float UnVisSkeleton[3] = { 0.f, 0.2f, 1.f };

		float TeamFiledboxColor[3] = { 0.f, 0.2f, 1.f };
		float VisFiledboxColor[3] = { 0.f, 0.2f, 1.f };
		float UnVisFiledboxColor[3] = { 0.f, 0.2f, 1.f };

		float TeamDistanceColor[3] = { 0.f, 0.2f, 1.f };
		float VisDistanceColor[3] = { 0.f, 0.2f, 1.f };
		float UnVisDistanceColor[3] = { 0.f, 0.2f, 1.f };

		float TeamNameColor[3] = { 0.f, 0.2f, 1.f };
		float VisNameColor[3] = { 0.f, 0.2f, 1.f };
		float UnVisNameColor[3] = { 0.f, 0.2f, 1.f };

		float VehicleNameColor[3] = { 0.f, 0.2f, 1.f };
		float VehicleColor[3] = { 0.f, 0.2f, 1.f };
		
		float RadarTeam[3] = { 0.f, 0.2f, 1.f };
		float VisRadarEnemy[3] = { 0.f, 0.2f, 1.f };
		float UnVisRadarEnemy[3] = { 0.f, 0.2f, 1.f };
		float VehicleRadar[3] = { 0.f, 0.2f, 1.f };
		float CrossHair[3] = { 0.f, 0.2f, 1.f };
	}COLOR;
};
struct ColorP
{
public:
	const char* Name;
	float* Ccolor;

	ColorP(const char* name, float* color)
	{
		this->Name = name;
		this->Ccolor = color;
	}
};
extern Variables g_Options;
#pragma endregion