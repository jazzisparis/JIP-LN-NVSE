#pragma once

DEFINE_COMMAND_PLUGIN(IsRadioRef, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadioBroadcastType, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetRadioBroadcastType, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetRadioRadius, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetRadioRadius, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetRadioStatic, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetRadioStatic, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetRadioPosRef, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetRadioPosRef, , 1, 1, kParams_JIP_OneObjectRef);

bool Cmd_IsRadioRef_Execute(COMMAND_ARGS)
{
	*result = thisObj->extraDataList.HasType(kExtraData_RadioData);
	return true;
}

bool Cmd_GetRadioBroadcastType_Execute(COMMAND_ARGS)
{
	ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
	*result = xRadio ? (int)xRadio->rangeType : 0;
	return true;
}

bool Cmd_SetRadioBroadcastType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type) && (type <= 4))
	{
		ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
		if (xRadio)
		{
			xRadio->rangeType = type;
			if (type) xRadio->radius = xRadio->staticPerc = 0;
		}
	}
	return true;
}

bool Cmd_GetRadioRadius_Execute(COMMAND_ARGS)
{
	ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
	*result = (xRadio && !xRadio->rangeType) ? xRadio->radius : 0;
	return true;
}

bool Cmd_SetRadioRadius_Execute(COMMAND_ARGS)
{
	float radius;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &radius) && (radius >= 0))
	{
		ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
		if (xRadio && !xRadio->rangeType) xRadio->radius = radius;
	}
	return true;
}

bool Cmd_GetRadioStatic_Execute(COMMAND_ARGS)
{
	ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
	*result = (xRadio && !xRadio->rangeType) ? xRadio->staticPerc : 0;
	return true;
}

bool Cmd_SetRadioStatic_Execute(COMMAND_ARGS)
{
	float radStatic;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &radStatic) && (radStatic >= 0))
	{
		ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
		if (xRadio && !xRadio->rangeType) xRadio->staticPerc = radStatic;
	}
	return true;
}

bool Cmd_GetRadioPosRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
	if (xRadio && !xRadio->rangeType && xRadio->positionRef) REFR_RES = xRadio->positionRef->refID;
	return true;
}

bool Cmd_SetRadioPosRef_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr))
	{
		ExtraRadioData *xRadio = GetExtraType(&thisObj->extraDataList, RadioData);
		if (xRadio && !xRadio->rangeType) xRadio->positionRef = refr;
	}
	return true;
}