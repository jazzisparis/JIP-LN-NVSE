#pragma once

DEFINE_COMMAND_PLUGIN(GetWeatherImageSpaceMod, , 0, 2, kParams_JIP_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherImageSpaceMod, , 0, 3, kParams_JIP_OneWeatherID_OneInt_OneOptionalImageSpaceMod);
DEFINE_COMMAND_PLUGIN(GetWeatherTexture, , 0, 2, kParams_JIP_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherTexture, , 0, 3, kParams_JIP_OneWeatherID_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetWeatherPrecipitationModel, , 0, 1, kParams_JIP_OneWeatherID);
DEFINE_COMMAND_PLUGIN(SetWeatherPrecipitationModel, , 0, 2, kParams_JIP_OneWeatherID_OneString);
DEFINE_COMMAND_PLUGIN(GetWeatherTraitNumeric, , 0, 2, kParams_JIP_OneWeatherID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeatherTraitNumeric, , 0, 3, kParams_JIP_OneWeatherID_OneInt_OneDouble);
DEFINE_COMMAND_PLUGIN(GetWeatherRGBColor, , 0, 4, kParams_JIP_OneWeatherID_TwoInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetWeatherRGBColor, , 0, 5, kParams_JIP_OneWeatherID_ThreeInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetCurrentWeather, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetWeatherTransitionTimeOverride, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetWindDirection, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetWindDirection, , 0, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(SetWindSpeedMult, , 0, 1, kParams_JIP_OneDouble);
DEFINE_COMMAND_PLUGIN(TriggerLightningFX, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ResetClouds, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ReloadCloudTextures, , 0, 0, NULL);

bool Cmd_GetWeatherImageSpaceMod_Execute(COMMAND_ARGS)
{
	*result = 0;
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
	const char *resStr;
	TESWeather *weather;
	UInt32 layer;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &layer) && (layer <= 3))
		resStr = weather->layerTextures[layer].ddsPath.m_data;
	else resStr = NULL;
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
	const char *resStr;
	TESWeather *weather;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather))
		resStr = weather->model.GetModelPath();
	else resStr = NULL;
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
	*result = 0;
	TESWeather *weather;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &traitID) || (traitID > 20)) return true;
	switch (traitID)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			*result = weather->cloudSpeed[traitID] / 2550.0;
			break;
		case 4:
			*result = weather->windSpeed / 255.0;
			break;
		case 5:
			*result = weather->transDelta / 1000.0;
			break;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			*result = ((UInt8*)weather)[0xDE + traitID] / 255.0;
			break;
		case 13:
			*result = weather->weatherClassification;
			break;
		case 14:
			*result = RGBHexToDec(weather->lightningColor);
			break;
		default:
			*result = weather->fogDistance[traitID - 15];
	}
	return true;
}

bool Cmd_SetWeatherTraitNumeric_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 traitID, intVal;
	double value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &traitID, &value) || (traitID > 20)) return true;
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
			intVal = (int)value;
			if (!intVal || (intVal == 1) || (intVal == 2) || (intVal == 4) || (intVal == 8))
				weather->weatherClassification = intVal;
			break;
		case 14:
			intVal = (int)value;
			if (intVal <= 255255255)
				weather->lightningColor = RGBDecToHex(intVal);
			break;
		default:
			weather->fogDistance[traitID - 15] = value;
	}
	return true;
}

bool Cmd_GetWeatherRGBColor_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWeather *weather;
	UInt32 type, time, layer = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &type, &time, &layer) && (type <= 9) && (time <= 5))
	{
		if (type != 2)
			*result = RGBHexToDec(weather->colors[type][time]);
		else if (layer <= 3)
			*result = RGBHexToDec(weather->cloudColor[layer][time]);
	}
	return true;
}

bool Cmd_SetWeatherRGBColor_Execute(COMMAND_ARGS)
{
	TESWeather *weather;
	UInt32 type, time, rgb, layer = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weather, &type, &time, &rgb, &layer) && (type <= 9) && (time <= 5) && (rgb <= 255255255))
	{
		if (type != 2)
			weather->colors[type][time] = RGBDecToHex(rgb);
		else if (layer <= 3)
			weather->cloudColor[layer][time] = RGBDecToHex(rgb);
	}
	return true;
}

bool Cmd_GetCurrentWeather_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWeather *weather = g_TES->sky ? g_TES->sky->currWeather : NULL;
	if (weather) REFR_RES = weather->refID;
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
	if (g_TES->sky)
	{
		*result = g_TES->sky->windDirection * -kDbl180dPI;
		DoConsolePrint(result);
	}
	else *result = 0;
	return true;
}

bool Cmd_SetWindDirection_Execute(COMMAND_ARGS)
{
	float windDirection;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &windDirection) && g_TES->sky)
		g_TES->sky->windDirection = windDirection * -kFltPId180;
	return true;
}

bool Cmd_SetWindSpeedMult_Execute(COMMAND_ARGS)
{
	double speedMult;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &speedMult))
		s_windSpeedMult = speedMult / 255.0;
	return true;
}

bool Cmd_TriggerLightningFX_Execute(COMMAND_ARGS)
{
	Sky *sky = g_TES->sky;
	if (sky && sky->GetIsRaining())
		sky->lightningFxPerc = 1;
	return true;
}

bool Cmd_ResetClouds_Execute(COMMAND_ARGS)
{
	__asm
	{
		pxor	xmm0, xmm0
		movups	ds:[0x11FF8B4], xmm0
	}
	return true;
}

bool Cmd_ReloadCloudTextures_Execute(COMMAND_ARGS)
{
	Sky *sky = g_TES->sky;
	if (sky)
	{
		Clouds *clouds = sky->clouds;
		if (clouds)
		{
			clouds->byte5A = 1;
			clouds->Update(sky, 0);
		}
	}
	return true;
}