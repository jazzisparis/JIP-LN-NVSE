#pragma once

DEFINE_COMMAND_PLUGIN(GetClimateSunTexture, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetClimateSunTexture, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateSunGlareTexture, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetClimateSunGlareTexture, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateNightSkyModel, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetClimateNightSkyModel, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetClimateTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetClimateTraitNumeric, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetClimateNumWeatherTypes, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetClimateNthWeatherType, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetClimateNthWeatherChance, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetClimateNthWeatherGlobal, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(ClimateAddWeatherType, , 0, 4, kParams_JIP_OneForm_OneWeatherID_OneInt_OneOptionalGlobal);
DEFINE_COMMAND_PLUGIN(ClimateRemoveWeatherType, , 0, 2, kParams_JIP_OneForm_OneWeatherID);
DEFINE_COMMAND_PLUGIN(GetCurrentClimate, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetCurrentClimate, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(RefreshCurrentClimate, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ForceClimate, , 0, 1, kParams_OneOptionalForm);

bool Cmd_GetClimateSunTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESClimate *climate;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate) && IS_ID(climate, TESClimate))
		resStr = climate->sunTexture.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetClimateSunTexture_Execute(COMMAND_ARGS)
{
	TESClimate *climate;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &path) && IS_ID(climate, TESClimate))
		climate->sunTexture.ddsPath.Set(path);
	return true;
}

bool Cmd_GetClimateSunGlareTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESClimate *climate;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate) && IS_ID(climate, TESClimate))
		resStr = climate->sunGlareTexture.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetClimateSunGlareTexture_Execute(COMMAND_ARGS)
{
	TESClimate *climate;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &path) && IS_ID(climate, TESClimate))
		climate->sunGlareTexture.ddsPath.Set(path);
	return true;
}

bool Cmd_GetClimateNightSkyModel_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESClimate *climate;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate) && IS_ID(climate, TESClimate))
		resStr = climate->nightSkyModel.GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetClimateNightSkyModel_Execute(COMMAND_ARGS)
{
	TESClimate *climate;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &path) && IS_ID(climate, TESClimate))
		climate->nightSkyModel.SetModelPath(path);
	return true;
}

bool Cmd_GetClimateTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &traitID) || NOT_ID(climate, TESClimate)) return true;
	switch (traitID)
	{
	case 0:
		*result = climate->sunriseBegin * 10;
		break;
	case 1:
		*result = climate->sunriseEnd * 10;
		break;
	case 2:
		*result = climate->sunsetBegin * 10;
		break;
	case 3:
		*result = climate->sunsetEnd * 10;
		break;
	case 4:
		*result = climate->volatility;
		break;
	case 5:
		*result = climate->phaseLength >> 6;
		break;
	case 6:
		*result = climate->phaseLength & 63;
	}
	return true;
}

bool Cmd_SetClimateTraitNumeric_Execute(COMMAND_ARGS)
{
	TESClimate *climate;
	UInt32 traitID, val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &traitID, &val) || NOT_ID(climate, TESClimate)) return true;
	switch (traitID)
	{
	case 0:
		climate->sunriseBegin = (val > 1430) ? 143 : (val / 10);
		break;
	case 1:
		climate->sunriseEnd = (val > 1430) ? 143 : (val / 10);
		break;
	case 2:
		climate->sunsetBegin = (val > 1430) ? 143 : (val / 10);
		break;
	case 3:
		climate->sunsetEnd = (val > 1430) ? 143 : (val / 10);
		break;
	case 4:
		climate->volatility = (val > 255) ? 255 : val;
		break;
	case 5:
		climate->phaseLength = (climate->phaseLength & 63) + ((val > 3) ? 192 : (val << 6));
		break;
	case 6:
		climate->phaseLength = (climate->phaseLength & 192) + ((val > 63) ? 63 : val);
	}
	return true;
}

bool Cmd_GetClimateNumWeatherTypes_Execute(COMMAND_ARGS)
{
	TESClimate *climate;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate) && IS_ID(climate, TESClimate))
		*result = (int)climate->weatherTypes.Count();
	else *result = 0;
	return true;
}

bool Cmd_GetClimateNthWeatherType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &idx) && IS_ID(climate, TESClimate))
	{
		WeatherEntry *entry = climate->weatherTypes.GetNthItem(idx);
		if (entry && entry->weather) REFR_RES = entry->weather->refID;
	}
	return true;
}

bool Cmd_GetClimateNthWeatherChance_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate;
	UInt32 idx;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &idx) || NOT_ID(climate, TESClimate)) return true;
	WeatherEntry *entry = climate->weatherTypes.GetNthItem(idx);
	if (entry) *result = (int)entry->chance;
	return true;
}

bool Cmd_GetClimateNthWeatherGlobal_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &idx) && IS_ID(climate, TESClimate))
	{
		WeatherEntry *entry = climate->weatherTypes.GetNthItem(idx);
		if (entry && entry->global) REFR_RES = entry->global->refID;
	}
	return true;
}

bool Cmd_ClimateAddWeatherType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate;
	TESWeather *weather;
	UInt32 chance;
	TESGlobal *global = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &weather, &chance, &global) || NOT_ID(climate, TESClimate)) return true;
	WeatherEntry *entry = climate->GetWeatherEntry(weather);
	entry->weather = weather;
	entry->chance = chance % 101;
	entry->global = global;
	*result = 1;
	return true;
}

bool Cmd_ClimateRemoveWeatherType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate;
	TESWeather *weather;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &climate, &weather) || NOT_ID(climate, TESClimate)) return true;
	WeatherEntry *entry = climate->GetWeatherEntry(weather, true);
	if (entry)
	{
		GameHeapFree(entry);
		*result = 1;
	}
	return true;
}

bool Cmd_GetCurrentClimate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESClimate *climate = g_TES->sky ? g_TES->sky->currClimate : NULL;
	if (climate) REFR_RES = climate->refID;
	DoConsolePrint(climate);
	return true;
}

bool Cmd_SetCurrentClimate_Execute(COMMAND_ARGS)
{
	TESClimate *climate;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate) && IS_ID(climate, TESClimate))
	{
		if (s_forcedClimate && (s_forcedClimate != climate))
			s_forcedClimate = NULL;
		Sky *currSky = g_TES->sky;
		if (currSky)
			currSky->currClimate = climate;
	}
	return true;
}

bool Cmd_RefreshCurrentClimate_Execute(COMMAND_ARGS)
{
	Sky *currSky = g_TES->sky;
	if (currSky) currSky->RefreshClimate(currSky->currClimate);
	return true;
}

bool Cmd_ForceClimate_Execute(COMMAND_ARGS)
{
	TESClimate *climate = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &climate) && (!climate || IS_ID(climate, TESClimate)))
		s_forcedClimate = climate;
	return true;
}