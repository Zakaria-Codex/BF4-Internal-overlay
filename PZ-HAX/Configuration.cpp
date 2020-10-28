#include "Configuration.hpp"
#include "Settings.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
#include "TEXT.h"
#include "CSX.h"
#include "direct3d.h"

bool ScanColorFromCvar(const char* str, float* colors)
{
	int len = strlen(str);
	if (len > 11)return false;
	int clrnum = 0;
	int clrsum = 0;
	for (int i = 0; i < len; i++)
	{
		if (clrnum >= 3)return false;
		if (str[i] >= '0' && str[i] <= '9')
		{
			clrsum = clrsum * 10 + str[i] - (int)'0';
		}
		else
		{
			colors[clrnum++] = (float)clrsum / 255.f;
			clrsum = 0;
		}
	}
	colors[clrnum] = (float)clrsum / 255.f;
	return true;
}

void LoadSettings(std::string szIniFile)
{
	CSX::Cvar::InitPath(szIniFile.c_str());

#pragma region LoadAimbot
	g_Options.AIMBOT.aimbot = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_aimbot, g_Options.AIMBOT.aimbot) != false;
	g_Options.AIMBOT.aimbotauto = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_autoaimbot, g_Options.AIMBOT.aimbotauto) != false;
	g_Options.AIMBOT.aimbotenemy = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_aimbotenemy, g_Options.AIMBOT.aimbotenemy) != false;
	g_Options.AIMBOT.aimbotteam = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_aimbotteam, g_Options.AIMBOT.aimbotteam) != false;
	g_Options.AIMBOT.aimkey = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_aimkey, g_Options.AIMBOT.aimkey);
	g_Options.AIMBOT.FOV = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_fov, g_Options.AIMBOT.FOV);
	g_Options.AIMBOT.Lock = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_lock, g_Options.AIMBOT.Lock);
	g_Options.AIMBOT.aimsmooth = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_aimsmooth, g_Options.AIMBOT.aimsmooth);
	g_Options.AIMBOT.drawfov = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_drawfov, g_Options.AIMBOT.drawfov);
	g_Options.AIMBOT.hitboxbone = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_hitboxbone, g_Options.AIMBOT.hitboxbone);
	g_Options.AIMBOT.bPredict = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_pointscalex, g_Options.AIMBOT.bPredict);
	g_Options.AIMBOT.bYAxis = CSX::Cvar::LoadCvar(AIMBOT_TEXT, A_pointscaley, g_Options.AIMBOT.bYAxis);

#pragma endregion
#pragma region LoadESP
	g_Options.ESP.EnableESP = CSX::Cvar::LoadCvar(ESP_TEXT, E_enableesp, g_Options.ESP.EnableESP) != false;
	g_Options.ESP.EspBoxType = CSX::Cvar::LoadCvar(ESP_TEXT, E_espboxtype, g_Options.ESP.EspBoxType);
	g_Options.ESP.EspHealthBarType = CSX::Cvar::LoadCvar(ESP_TEXT, E_esphealthbartype, g_Options.ESP.EspHealthBarType);
	g_Options.ESP.ESPVehicleMode = CSX::Cvar::LoadCvar(ESP_TEXT, E_espvehiclemode, g_Options.ESP.ESPVehicleMode);
	g_Options.ESP.distanceToPlayer = CSX::Cvar::LoadCvar(ESP_TEXT, E_distancetoplayer, g_Options.ESP.distanceToPlayer);
	g_Options.ESP.ESP_ENEMY.enemyBoxes = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyboxes, g_Options.ESP.ESP_ENEMY.enemyBoxes) != false;
	g_Options.ESP.ESP_ENEMY.enemyDistance = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemydistance, g_Options.ESP.ESP_ENEMY.enemyDistance) != false;
	g_Options.ESP.ESP_ENEMY.enemyfilledbox = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyfilledbox, g_Options.ESP.ESP_ENEMY.enemyfilledbox) != false;
	g_Options.ESP.ESP_ENEMY.enemyHealth = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyhealth, g_Options.ESP.ESP_ENEMY.enemyHealth) != false;
	g_Options.ESP.ESP_ENEMY.enemyNames = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyNames, g_Options.ESP.ESP_ENEMY.enemyNames) != false;
	g_Options.ESP.ESP_ENEMY.enemySkeleton = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyskeleton, g_Options.ESP.ESP_ENEMY.enemySkeleton) != false;
	g_Options.ESP.ESP_ENEMY.enemySnaplines = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemysnaplines, g_Options.ESP.ESP_ENEMY.enemySnaplines) != false;
	g_Options.ESP.ESP_TEAM.teamBoxes = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamboxes, g_Options.ESP.ESP_TEAM.teamBoxes) != false;
	g_Options.ESP.ESP_TEAM.teamDistance = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamdistance, g_Options.ESP.ESP_TEAM.teamDistance) != false;
	g_Options.ESP.ESP_TEAM.teamfilledbox = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamfilledbox, g_Options.ESP.ESP_TEAM.teamfilledbox) != false;
	g_Options.ESP.ESP_TEAM.teamHealth = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamhealth, g_Options.ESP.ESP_TEAM.teamHealth) != false;
	g_Options.ESP.ESP_TEAM.teamNames = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamnames, g_Options.ESP.ESP_TEAM.teamNames) != false;
	g_Options.ESP.ESP_TEAM.teamSkeleton = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamskeleton, g_Options.ESP.ESP_TEAM.teamSkeleton) != false;
	g_Options.ESP.ESP_TEAM.teamSnaplines = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamsnaplines, g_Options.ESP.ESP_TEAM.teamSnaplines) != false;
	g_Options.ESP.VEHICLE_ENEMY.enemyVehicle = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyvehicle, g_Options.ESP.VEHICLE_ENEMY.enemyVehicle) != false;
	g_Options.ESP.VEHICLE_ENEMY.enemyVehicledmg = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyvehicledmg, g_Options.ESP.VEHICLE_ENEMY.enemyVehicledmg) != false;
	g_Options.ESP.VEHICLE_ENEMY.enemyVehicleName = CSX::Cvar::LoadCvar(ESP_TEXT, E_enemyvehicleName, g_Options.ESP.VEHICLE_ENEMY.enemyVehicleName) != false;
	g_Options.ESP.VEHICLE_TEAM.teamVehicle = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamvehicle, g_Options.ESP.VEHICLE_TEAM.teamVehicle) != false;
	g_Options.ESP.VEHICLE_TEAM.teamVehicledmg = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamvehicledmg, g_Options.ESP.VEHICLE_TEAM.teamVehicledmg) != false;
	g_Options.ESP.VEHICLE_TEAM.teamVehicleName = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamvehicleName, g_Options.ESP.VEHICLE_TEAM.teamVehicleName) != false;
	g_Options.ESP.ESPVehicleMode = CSX::Cvar::LoadCvar(ESP_TEXT, E_espvehiclemode, g_Options.ESP.ESPVehicleMode);
	g_Options.ESP.filledboxAlpha = CSX::Cvar::LoadCvar(ESP_TEXT, E_filledboxAlpha, g_Options.ESP.filledboxAlpha);
#pragma endregion
#pragma region LoadMISC
	g_Options.MISC.RADAR.radarwindow = CSX::Cvar::LoadCvar(MISC_TEXT, M_radarwindow, g_Options.MISC.RADAR.radarwindow) != false;
	g_Options.MISC.RADAR.radralpha = CSX::Cvar::LoadCvar(MISC_TEXT, M_radralpha, g_Options.MISC.RADAR.radralpha);
	g_Options.MISC.RADAR.radrzoom = CSX::Cvar::LoadCvar(MISC_TEXT, M_radarzoom, g_Options.MISC.RADAR.radrzoom);
	g_Options.MISC.RADAR.distanceToPlayer = CSX::Cvar::LoadCvar(MISC_TEXT, M_distancetoplayer, g_Options.MISC.RADAR.distanceToPlayer);
	g_Options.MISC.RADAR.PLAYERS.Team = CSX::Cvar::LoadCvar(MISC_TEXT, M_radarteam, g_Options.MISC.RADAR.PLAYERS.Team) != false;
	g_Options.MISC.RADAR.PLAYERS.Enemy = CSX::Cvar::LoadCvar(MISC_TEXT, M_radarenemy, g_Options.MISC.RADAR.PLAYERS.Enemy) != false;
	g_Options.MISC.RADAR.VEHICLE.TeamVehicle = CSX::Cvar::LoadCvar(MISC_TEXT, M_teamvehicle, g_Options.MISC.RADAR.VEHICLE.TeamVehicle) != false;
	g_Options.MISC.RADAR.VEHICLE.EnemyVehicle = CSX::Cvar::LoadCvar(MISC_TEXT, M_enemyvehicle, g_Options.MISC.RADAR.VEHICLE.EnemyVehicle) != false;

	g_Options.MISC.crosshair.crosshair = CSX::Cvar::LoadCvar(MISC_TEXT, M_crosshairtype, g_Options.MISC.crosshair.crosshair);
	g_Options.MISC.crosshair.size = CSX::Cvar::LoadCvar(MISC_TEXT, M_size, g_Options.MISC.crosshair.size);
	g_Options.MISC.crosshair.stroke = CSX::Cvar::LoadCvar(MISC_TEXT, M_stroke, g_Options.MISC.crosshair.stroke);

#pragma endregion
#pragma region LoadMEMU
	g_Options.MENU.iVisualsSubTab = CSX::Cvar::LoadCvar(MENU_TEXT, M_iVisualssubTab, g_Options.MENU.iVisualsSubTab);
	g_Options.MENU.Theme = CSX::Cvar::LoadCvar(MENU_TEXT, M_thame, g_Options.MENU.Theme);

#pragma endregion
#pragma region LoadCOLORS
	string teamtolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_teamtolor, "0,0,0");
	string occludedenemycolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_occludedenemycolor, "0,0,0");
	string visibleenemycolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_visibleenemycolor, "0,0,0");
	string teamlinecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_teamlinecolor, "0,0,0");
	string vislinecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_vislinecolor, "0,0,0");
	string unvislinecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_unvislinecolor, "0,0,0");
	string teamskeleton = CSX::Cvar::LoadCvar(COLORS_TEXT, C_teamskeleton, "0,0,0");
	string visskeleton = CSX::Cvar::LoadCvar(COLORS_TEXT, C_visskeleton, "0,0,0");
	string unvisskeleton = CSX::Cvar::LoadCvar(COLORS_TEXT, C_unvisskeleton, "0,0,0");

	string teamfiledboxcolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_teamfiledboxcolor, "0,0,0");
	string visfiledboxcolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_visfiledboxcolor, "0,0,0");
	string unvisfiledboxcolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_unvisfiledboxcolor, "0,0,0");
	string teamdistancecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_teamdistancecolor, "0,0,0");
	string visdistancecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_visdistancecolor, "0,0,0");
	string unvisdistancecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_unvisdistancecolor, "0,0,0");
	string teamnamecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_teamnamecolor, "0,0,0");
	string visnamecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_visnamecolor, "0,0,0");
	string unvisnamecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_unvisnamecolor, "0,0,0");
	string vehiclenamecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_vehiclenamecolor, "0,0,0");
	string vehiclecolor = CSX::Cvar::LoadCvar(COLORS_TEXT, C_vehiclecolor, "0,0,0");

	string radarteam = CSX::Cvar::LoadCvar(COLORS_TEXT, C_radarteam, "0,0,0");
	string visradarenemy = CSX::Cvar::LoadCvar(COLORS_TEXT, C_visradarenemy, "0,0,0");
	string unvisradarenemy = CSX::Cvar::LoadCvar(COLORS_TEXT, C_unvisradarenemy, "0,0,0");
	string vehicleradar = CSX::Cvar::LoadCvar(COLORS_TEXT, C_vehicleradar, "0,0,0");
	string crosshair = CSX::Cvar::LoadCvar(COLORS_TEXT, C_crosshair, "0,0,0");

	ScanColorFromCvar(teamtolor.c_str(), g_Options.COLOR.TeamColor);
	ScanColorFromCvar(occludedenemycolor.c_str(), g_Options.COLOR.OccludedEnemyColor);
	ScanColorFromCvar(visibleenemycolor.c_str(), g_Options.COLOR.VisibleEnemyColor);
	ScanColorFromCvar(teamlinecolor.c_str(), g_Options.COLOR.TeamLineColor);
	ScanColorFromCvar(vislinecolor.c_str(), g_Options.COLOR.VisLineColor);
	ScanColorFromCvar(unvislinecolor.c_str(), g_Options.COLOR.UnVisLineColor);
	ScanColorFromCvar(teamskeleton.c_str(), g_Options.COLOR.TeamSkeleton);
	ScanColorFromCvar(visskeleton.c_str(), g_Options.COLOR.VisSkeleton);
	ScanColorFromCvar(unvisskeleton.c_str(), g_Options.COLOR.UnVisSkeleton);
	ScanColorFromCvar(teamfiledboxcolor.c_str(), g_Options.COLOR.TeamFiledboxColor);
	ScanColorFromCvar(visfiledboxcolor.c_str(), g_Options.COLOR.VisFiledboxColor);
	ScanColorFromCvar(unvisfiledboxcolor.c_str(), g_Options.COLOR.UnVisFiledboxColor);
	ScanColorFromCvar(teamdistancecolor.c_str(), g_Options.COLOR.TeamDistanceColor);
	ScanColorFromCvar(visdistancecolor.c_str(), g_Options.COLOR.VisDistanceColor);
	ScanColorFromCvar(unvisdistancecolor.c_str(), g_Options.COLOR.UnVisDistanceColor);
	ScanColorFromCvar(teamnamecolor.c_str(), g_Options.COLOR.TeamNameColor);
	ScanColorFromCvar(visnamecolor.c_str(), g_Options.COLOR.VisNameColor);
	ScanColorFromCvar(unvisnamecolor.c_str(), g_Options.COLOR.UnVisNameColor);
	ScanColorFromCvar(vehiclenamecolor.c_str(), g_Options.COLOR.VehicleNameColor);
	ScanColorFromCvar(vehiclecolor.c_str(), g_Options.COLOR.VehicleColor);
	ScanColorFromCvar(radarteam.c_str(), g_Options.COLOR.RadarTeam);
	ScanColorFromCvar(visradarenemy.c_str(), g_Options.COLOR.VisRadarEnemy);
	ScanColorFromCvar(unvisradarenemy.c_str(), g_Options.COLOR.UnVisRadarEnemy);
	ScanColorFromCvar(vehicleradar.c_str(), g_Options.COLOR.VehicleRadar);
	ScanColorFromCvar(crosshair.c_str(), g_Options.COLOR.CrossHair);
#pragma endregion

}
void SaveSettings(std::string szIniFile)
{
	CSX::Cvar::InitPath(szIniFile.c_str());
#pragma region SaveAimbot
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_aimbot, g_Options.AIMBOT.aimbot);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_autoaimbot, g_Options.AIMBOT.aimbotauto);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_aimbotenemy, g_Options.AIMBOT.aimbotenemy);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_aimbotteam, g_Options.AIMBOT.aimbotteam);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_aimkey, g_Options.AIMBOT.aimkey);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_fov, g_Options.AIMBOT.FOV);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_lock, g_Options.AIMBOT.Lock);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_aimsmooth, g_Options.AIMBOT.aimsmooth);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_drawfov, g_Options.AIMBOT.drawfov);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_hitboxbone, g_Options.AIMBOT.hitboxbone);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_pointscalex, g_Options.AIMBOT.bPredict);
	CSX::Cvar::SaveCvar(AIMBOT_TEXT, A_pointscaley, g_Options.AIMBOT.bYAxis);

#pragma endregion
#pragma region SaveESP
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enableesp, g_Options.ESP.EnableESP);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_espboxtype, g_Options.ESP.EspBoxType);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esphealthbartype, g_Options.ESP.EspHealthBarType);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_espvehiclemode, g_Options.ESP.ESPVehicleMode);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_distancetoplayer, g_Options.ESP.distanceToPlayer);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyboxes, g_Options.ESP.ESP_ENEMY.enemyBoxes);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemydistance, g_Options.ESP.ESP_ENEMY.enemyDistance);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyfilledbox, g_Options.ESP.ESP_ENEMY.enemyfilledbox);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyhealth, g_Options.ESP.ESP_ENEMY.enemyHealth);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyNames, g_Options.ESP.ESP_ENEMY.enemyNames);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyskeleton, g_Options.ESP.ESP_ENEMY.enemySkeleton);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemysnaplines, g_Options.ESP.ESP_ENEMY.enemySnaplines);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamboxes, g_Options.ESP.ESP_TEAM.teamBoxes);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamdistance, g_Options.ESP.ESP_TEAM.teamDistance);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamfilledbox, g_Options.ESP.ESP_TEAM.teamfilledbox);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamhealth, g_Options.ESP.ESP_TEAM.teamHealth);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamnames, g_Options.ESP.ESP_TEAM.teamNames);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamskeleton, g_Options.ESP.ESP_TEAM.teamSkeleton);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamsnaplines, g_Options.ESP.ESP_TEAM.teamSnaplines);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyvehicle, g_Options.ESP.VEHICLE_ENEMY.enemyVehicle);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyvehicledmg, g_Options.ESP.VEHICLE_ENEMY.enemyVehicledmg);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_enemyvehicleName, g_Options.ESP.VEHICLE_ENEMY.enemyVehicleName);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamvehicle, g_Options.ESP.VEHICLE_TEAM.teamVehicle);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamvehicledmg, g_Options.ESP.VEHICLE_TEAM.teamVehicledmg);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamvehicleName, g_Options.ESP.VEHICLE_TEAM.teamVehicleName);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_espvehiclemode, g_Options.ESP.ESPVehicleMode);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_filledboxAlpha, g_Options.ESP.filledboxAlpha);

#pragma endregion
#pragma region SaveMISC
	CSX::Cvar::SaveCvar(MISC_TEXT, M_radarwindow, g_Options.MISC.RADAR.radarwindow);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_radralpha, g_Options.MISC.RADAR.radralpha);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_radarzoom, g_Options.MISC.RADAR.radrzoom);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_distancetoplayer, g_Options.MISC.RADAR.distanceToPlayer);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_radarteam, g_Options.MISC.RADAR.PLAYERS.Team);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_radarenemy, g_Options.MISC.RADAR.PLAYERS.Enemy);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_teamvehicle, g_Options.MISC.RADAR.VEHICLE.TeamVehicle);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_enemyvehicle, g_Options.MISC.RADAR.VEHICLE.EnemyVehicle);

	CSX::Cvar::SaveCvar(MISC_TEXT, M_crosshairtype, g_Options.MISC.crosshair.crosshair);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_size, g_Options.MISC.crosshair.size);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_stroke, g_Options.MISC.crosshair.stroke);

#pragma endregion
#pragma region SaveMEMU
	CSX::Cvar::SaveCvar(MENU_TEXT, M_iVisualssubTab, g_Options.MENU.iVisualsSubTab);
	CSX::Cvar::SaveCvar(MENU_TEXT, M_thame, g_Options.MENU.Theme);
#pragma endregion
#pragma region SaveCOLOR
	string TeamColor =
		to_string(int(g_Options.COLOR.TeamColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamColor[2] * 255.f));
	string OccludedEnemyColor =
		to_string(int(g_Options.COLOR.OccludedEnemyColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.OccludedEnemyColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.OccludedEnemyColor[2] * 255.f));
	string VisibleEnemyColor =
		to_string(int(g_Options.COLOR.VisibleEnemyColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisibleEnemyColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisibleEnemyColor[2] * 255.f));
	string TeamLineColor =
		to_string(int(g_Options.COLOR.TeamLineColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamLineColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamLineColor[2] * 255.f));
	string VisLineColor =
		to_string(int(g_Options.COLOR.VisLineColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisLineColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisLineColor[2] * 255.f));
	string UnVisLineColor =
		to_string(int(g_Options.COLOR.UnVisLineColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisLineColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisLineColor[2] * 255.f));
	string TeamSkeleton =
		to_string(int(g_Options.COLOR.TeamSkeleton[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamSkeleton[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamSkeleton[2] * 255.f));
	string VisSkeleton =
		to_string(int(g_Options.COLOR.VisSkeleton[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisSkeleton[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisSkeleton[2] * 255.f));
	string UnVisSkeleton =
		to_string(int(g_Options.COLOR.UnVisSkeleton[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisSkeleton[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisSkeleton[2] * 255.f));
	string TeamFiledboxColor =
		to_string(int(g_Options.COLOR.TeamFiledboxColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamFiledboxColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamFiledboxColor[2] * 255.f));
	string VisFiledboxColor =
		to_string(int(g_Options.COLOR.VisFiledboxColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisFiledboxColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisFiledboxColor[2] * 255.f));
	string UnVisFiledboxColor =
		to_string(int(g_Options.COLOR.UnVisFiledboxColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisFiledboxColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisFiledboxColor[2] * 255.f));
	string TeamDistanceColor =
		to_string(int(g_Options.COLOR.TeamDistanceColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamDistanceColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamDistanceColor[2] * 255.f));
	string VisDistanceColor =
		to_string(int(g_Options.COLOR.VisDistanceColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisDistanceColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisDistanceColor[2] * 255.f));
	string UnVisDistanceColor =
		to_string(int(g_Options.COLOR.UnVisDistanceColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisDistanceColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisDistanceColor[2] * 255.f));
	string TeamNameColor =
		to_string(int(g_Options.COLOR.TeamNameColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamNameColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.TeamNameColor[2] * 255.f));
	string VisNameColor =
		to_string(int(g_Options.COLOR.VisNameColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisNameColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisNameColor[2] * 255.f));
	string UnVisNameColor =
		to_string(int(g_Options.COLOR.UnVisNameColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisNameColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisNameColor[2] * 255.f));
	string VehicleNameColor =
		to_string(int(g_Options.COLOR.VehicleNameColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VehicleNameColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VehicleNameColor[2] * 255.f));
	string VehicleColor =
		to_string(int(g_Options.COLOR.VehicleColor[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VehicleColor[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VehicleColor[2] * 255.f));
	string RadarTeam =
		to_string(int(g_Options.COLOR.RadarTeam[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.RadarTeam[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.RadarTeam[2] * 255.f));
	string VisRadarEnemy =
		to_string(int(g_Options.COLOR.VisRadarEnemy[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisRadarEnemy[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VisRadarEnemy[2] * 255.f));
	string UnVisRadarEnemy =
		to_string(int(g_Options.COLOR.UnVisRadarEnemy[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisRadarEnemy[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.UnVisRadarEnemy[2] * 255.f));
	string VehicleRadar =
		to_string(int(g_Options.COLOR.VehicleRadar[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VehicleRadar[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.VehicleRadar[2] * 255.f));
	string crosshair =
		to_string(int(g_Options.COLOR.CrossHair[0] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.CrossHair[1] * 255.f)) + "," +
		to_string(int(g_Options.COLOR.CrossHair[2] * 255.f));


	CSX::Cvar::SaveCvar(COLORS_TEXT, C_teamtolor, TeamColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_occludedenemycolor, OccludedEnemyColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_visibleenemycolor, VisibleEnemyColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_teamlinecolor, TeamLineColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_vislinecolor, VisLineColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_unvislinecolor, UnVisLineColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_teamskeleton, TeamSkeleton);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_visskeleton, VisSkeleton);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_unvisskeleton, UnVisSkeleton);

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_teamfiledboxcolor, TeamFiledboxColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_visfiledboxcolor, VisFiledboxColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_unvisfiledboxcolor, UnVisFiledboxColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_teamdistancecolor, TeamDistanceColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_visdistancecolor, VisDistanceColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_unvisdistancecolor, UnVisDistanceColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_teamnamecolor, TeamNameColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_visnamecolor, VisNameColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_unvisnamecolor, UnVisNameColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_vehiclenamecolor, VehicleNameColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_vehiclecolor, VehicleColor);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_radarteam, RadarTeam);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_visradarenemy, VisRadarEnemy);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_unvisradarenemy, UnVisRadarEnemy);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_vehicleradar, VehicleRadar);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_crosshair, crosshair);

#pragma endregion

}

Variables g_Options;