#pragma once

DEFINE_COMMAND_PLUGIN(GetWeatherImageSpaceMod, 0, kParams_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherImageSpaceMod, 0, kParams_OneWeatherID_OneInt_OneOptionalImageSpaceMod);
DEFINE_COMMAND_PLUGIN(GetWeatherTexture, 0, kParams_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherTexture, 0, kParams_OneWeatherID_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetWeatherPrecipitationModel, 0, kParams_OneWeatherID);
DEFINE_COMMAND_PLUGIN(SetWeatherPrecipitationModel, 0, kParams_OneWeatherID_OneString);
DEFINE_COMMAND_PLUGIN(GetWeatherTraitNumeric, 0, kParams_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherTraitNumeric, 0, kParams_OneWeatherID_OneInt_OneDouble);
DEFINE_COMMAND_PLUGIN(GetWeatherRGBColor, 0, kParams_OneWeatherID_TwoInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetWeatherRGBColor, 0, kParams_OneWeatherID_ThreeInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetCurrentWeather, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetWeatherTransitionTimeOverride, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetWindDirection, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetWindDirection, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(SetWindSpeedMult, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(TriggerLightningFX, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ResetClouds, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ReloadCloudTextures, 0, nullptr);

bool Cmd_GetWeatherImageSpaceMod_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 time;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &time) && (time <= 5) && weather->imageSpaceMods[time])
		REFR_RES = weather->imageSpaceMods[time]->refID;
	return true;
}

bool Cmd_SetWeatherImageSpaceMod_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 time;
	TESImageSpaceModifier *imgSpcMod = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &time, &imgSpcMod) && (time <= 5))
		weather->imageSpaceMods[time] = imgSpcMod;
	return true;
}

bool Cmd_GetWeatherTexture_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESWeather *weather;
	UInt32 layer;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &layer) && (layer <= 3))
		resStr = weather->layerTextures[layer].ddsPath.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWeatherTexture_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 layer;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &layer, &path) && (layer <= 3))
		weather->layerTextures[layer].ddsPath.Set(path);
	return true;
}

bool Cmd_GetWeatherPrecipitationModel_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESWeather *weather;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather))
		resStr = weather->model.GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWeatherPrecipitationModel_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &path))
		weather->model.SetModelPath(path);
	return true;
}

bool Cmd_GetWeatherTraitNumeric_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &traitID) && (traitID <= 20))
		switch (traitID)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				*result = weather->cloudSpeed[traitID] * (1 / 2550.0);
				break;
			case 4:
				*result = weather->windSpeed * (1 / 255.0);
				break;
			case 5:
				*result = weather->transDelta * 0.001;
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				*result = ((UInt8*)weather)[0xDE + traitID] * (1 / 255.0);
				break;
			case 13:
				*result = weather->weatherClassification;
				break;
			case 14:
				cvtul2d(RGBHexToDec(weather->lightningColor), result);
				break;
			default:
				*result = weather->fogDistance[traitID - 15];
		}
	return true;
}

bool Cmd_SetWeatherTraitNumeric_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 traitID;
	double value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &traitID, &value) && (traitID <= 20))
		switch (traitID)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				weather->cloudSpeed[traitID] = (value > 0.1) ? 255 : (value * 2550);
				break;
			case 4:
				weather->windSpeed = (value > 1) ? 255 : (value * 255);
				break;
			case 5:
				weather->transDelta = (value > 0.25) ? 255 : (value * 1000);
				break;
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				((UInt8*)weather)[0xDE + traitID] = (value > 1) ? 255 : (value * 255);
				break;
			case 13:
				if (UInt32 intVal = (int)value; !(intVal & (intVal - 1)))
					weather->weatherClassification = intVal;
				break;
			case 14:
				if (UInt32 decRGB = cvtd2ul(value); decRGB <= 255255255)
					weather->lightningColor = RGBDecToHex(decRGB);
				break;
			default:
				weather->fogDistance[traitID - 15] = value;
		}
	return true;
}

bool Cmd_GetWeatherRGBColor_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 type, time, layer = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &type, &time, &layer) && (type <= 9) && (time <= 5))
		if (type != 2)
			cvtul2d(RGBHexToDec(weather->colors[type][time]), result);
		else if (layer <= 3)
			cvtul2d(RGBHexToDec(weather->cloudColor[layer][time]), result);
	return true;
}

bool Cmd_SetWeatherRGBColor_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 type, time, rgb, layer = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &type, &time, &rgb, &layer) && (type <= 9) && (time <= 5) && (rgb <= 255255255))
		if (type != 2)
			weather->colors[type][time] = RGBDecToHex(rgb);
		else if (layer <= 3)
			weather->cloudColor[layer][time] = RGBDecToHex(rgb);
	return true;
}

bool Cmd_GetCurrentWeather_Execute(COMMAND_ARGS)
{
	TESWeather *weather = g_currentSky->currWeather;
	if (weather)
		REFR_RES = weather->refID;
	DoConsolePrint(weather);
	return true;
}

bool Cmd_SetWeatherTransitionTimeOverride_Execute(COMMAND_ARGS)
{
	UInt32 transTime = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &transTime))
	{
		if (transTime != 0) s_weatherTransitionRateOverride = 120.0F / (int)transTime;
		HOOK_SET(UpdateWeather, transTime != 0);
	}
	return true;
}

bool Cmd_GetWindDirection_Execute(COMMAND_ARGS)
{
	*result = g_currentSky->windDirection * -Dbl180dPI;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetWindDirection_Execute(COMMAND_ARGS)
{
	float windDirection;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &windDirection))
		g_currentSky->windDirection = windDirection * -FltPId180;
	return true;
}

bool Cmd_SetWindSpeedMult_Execute(COMMAND_ARGS)
{
	float speedMult;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &speedMult))
		s_windSpeedMult = speedMult * (1 / 255.0F);
	return true;
}

bool Cmd_TriggerLightningFX_Execute(COMMAND_ARGS)
{
	if (g_currentSky->GetIsRaining())
		g_currentSky->lightningFxPerc = 1;
	return true;
}

bool Cmd_ResetClouds_Execute(COMMAND_ARGS)
{
	_mm_storeu_ps((float*)0x11FF8B4, _mm_setzero_ps());
	return true;
}

bool Cmd_ReloadCloudTextures_Execute(COMMAND_ARGS)
{
	if (Clouds *clouds = g_currentSky->clouds)
	{
		clouds->bForceUpdate = 1;
		clouds->Update(g_currentSky, 0);
	}
	return true;
}