#pragma once

DEFINE_COMMAND_PLUGIN(IsRadioRef, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetRadioBroadcastType, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetRadioBroadcastType, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetRadioRadius, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetRadioRadius, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetRadioStatic, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetRadioStatic, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetRadioPosRef, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetRadioPosRef, 1, kParams_OneObjectRef);

bool Cmd_IsRadioRef_Execute(COMMAND_ARGS)
{
	if (thisObj->extraDataList.HasType(kXData_ExtraRadioData))
		*result = 1;
	return true;
}

bool Cmd_GetRadioBroadcastType_Execute(COMMAND_ARGS)
{
	if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData))
		*result = (int)xRadio->rangeType;
	return true;
}

bool Cmd_SetRadioBroadcastType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type) && (type <= 4))
		if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData))
			if (xRadio->rangeType = type)
			{
				xRadio->radius = 0;
				xRadio->staticPerc = 0;
			}
	return true;
}

bool Cmd_GetRadioRadius_Execute(COMMAND_ARGS)
{
	if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData); xRadio && !xRadio->rangeType)
		*result = xRadio->radius;
	return true;
}

bool Cmd_SetRadioRadius_Execute(COMMAND_ARGS)
{
	float radius;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &radius) && (radius >= 0))
		if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData); xRadio && !xRadio->rangeType)
			xRadio->radius = radius;
	return true;
}

bool Cmd_GetRadioStatic_Execute(COMMAND_ARGS)
{
	if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData); xRadio && !xRadio->rangeType)
		*result = xRadio->staticPerc;
	return true;
}

bool Cmd_SetRadioStatic_Execute(COMMAND_ARGS)
{
	float radStatic;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &radStatic) && (radStatic >= 0))
		if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData); xRadio && !xRadio->rangeType)
			xRadio->staticPerc = radStatic;
	return true;
}

bool Cmd_GetRadioPosRef_Execute(COMMAND_ARGS)
{
	if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData); xRadio && !xRadio->rangeType && xRadio->positionRef)
		REFR_RES = xRadio->positionRef->refID;
	return true;
}

bool Cmd_SetRadioPosRef_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr))
		if (auto xRadio = GetExtraType(&thisObj->extraDataList, ExtraRadioData); xRadio && !xRadio->rangeType)
			xRadio->positionRef = refr;
	return true;
}