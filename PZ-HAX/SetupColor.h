#pragma once
#include "globals.h"

void SetupColors()
{
	Globals::ColorsForPicker1.push_back(ColorP("Team Box", g_Options.COLOR.TeamColor));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Enemy Box", g_Options.COLOR.OccludedEnemyColor));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Enemy Box", g_Options.COLOR.VisibleEnemyColor));

	Globals::ColorsForPicker1.push_back(ColorP("Team Name", g_Options.COLOR.TeamNameColor));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Enemy Name", g_Options.COLOR.VisNameColor));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Enemy Name", g_Options.COLOR.UnVisNameColor));

	Globals::ColorsForPicker1.push_back(ColorP("Team Distance", g_Options.COLOR.TeamDistanceColor));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Enemy Distance", g_Options.COLOR.VisDistanceColor));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Enemy Distance", g_Options.COLOR.UnVisDistanceColor));

	Globals::ColorsForPicker1.push_back(ColorP("Team Line ", g_Options.COLOR.TeamLineColor));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Enemy Line", g_Options.COLOR.VisLineColor));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Enemy Line", g_Options.COLOR.UnVisLineColor));

	Globals::ColorsForPicker1.push_back(ColorP("Team Filedbox ", g_Options.COLOR.TeamFiledboxColor));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Enemy Filedbox", g_Options.COLOR.VisFiledboxColor));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Enemy Filedbox", g_Options.COLOR.UnVisFiledboxColor));

	Globals::ColorsForPicker1.push_back(ColorP("Team Skeleton", g_Options.COLOR.TeamSkeleton));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Enemy Skeleton", g_Options.COLOR.VisSkeleton));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Enemy Skeleton", g_Options.COLOR.UnVisSkeleton));

	Globals::ColorsForPicker1.push_back(ColorP("Vehicle Name", g_Options.COLOR.VehicleNameColor));
	Globals::ColorsForPicker1.push_back(ColorP("Vehicle 3DBOX", g_Options.COLOR.VehicleColor));

	Globals::ColorsForPicker1.push_back(ColorP("Radar Team", g_Options.COLOR.RadarTeam));
	Globals::ColorsForPicker1.push_back(ColorP("Vis Radar Enemy", g_Options.COLOR.VisRadarEnemy));
	Globals::ColorsForPicker1.push_back(ColorP("UnVis Radar Enemy", g_Options.COLOR.UnVisRadarEnemy));
	Globals::ColorsForPicker1.push_back(ColorP("Radar Vehicle", g_Options.COLOR.VehicleRadar));
	Globals::ColorsForPicker1.push_back(ColorP("Crosshair", g_Options.COLOR.CrossHair));

}