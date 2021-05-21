#pragma once

DEFINE_COMMAND_PLUGIN(IsMapMarker, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetMapMarkerName, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerName, , 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetMapMarkerVisible, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerTravel, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerTravel, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerHidden, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerHidden, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerType, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerType, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerRep, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerRep, , 1, 1, kParams_OneOptionalForm);

bool Cmd_IsMapMarker_Execute(COMMAND_ARGS)
{
	ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
	*result = xMarker ? 1 : 0;
	return true;
}

bool Cmd_GetMapMarkerName_Execute(COMMAND_ARGS)
{
	ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
	AssignString(PASS_COMMAND_ARGS, xMarker ? xMarker->data->fullName.name.m_data : NULL);
	return true;
}

bool Cmd_SetMapMarkerName_Execute(COMMAND_ARGS)
{
	char nameStr[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nameStr))
	{
		ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
		if (xMarker) xMarker->data->fullName.name.Set(nameStr);
	}
	return true;
}

bool Cmd_SetMapMarkerVisible_Execute(COMMAND_ARGS)
{
	UInt32 visible;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &visible))
	{
		ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
		if (xMarker) xMarker->SetVisible(visible != 0);
	}
	return true;
}

bool Cmd_GetMapMarkerTravel_Execute(COMMAND_ARGS)
{
	ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
	*result = (xMarker && xMarker->CanTravel()) ? 1 : 0;
	return true;
}

bool Cmd_SetMapMarkerTravel_Execute(COMMAND_ARGS)
{
	UInt32 travel;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &travel))
	{
		ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
		if (xMarker) xMarker->SetCanTravel(travel != 0);
	}
	return true;
}

bool Cmd_GetMapMarkerHidden_Execute(COMMAND_ARGS)
{
	ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
	*result = (xMarker && xMarker->IsHidden()) ? 1 : 0;
	return true;
}

bool Cmd_SetMapMarkerHidden_Execute(COMMAND_ARGS)
{
	UInt32 hidden;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hidden))
	{
		ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
		if (xMarker) xMarker->SetHidden(hidden != 0);
	}
	return true;
}

bool Cmd_GetMapMarkerType_Execute(COMMAND_ARGS)
{
	ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
	*result = xMarker ? xMarker->data->type : -1;
	return true;
}

bool Cmd_SetMapMarkerType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type))
	{
		ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
		if (xMarker) xMarker->data->type = type;
	}
	return true;
}

bool Cmd_GetMapMarkerRep_Execute(COMMAND_ARGS)
{
	*result = 0;
	ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
	TESForm *reputation = xMarker ? xMarker->data->reputation : NULL;
	if (reputation) REFR_RES = reputation->refID;
	DoConsolePrint(reputation);
	return true;
}

bool Cmd_SetMapMarkerRep_Execute(COMMAND_ARGS)
{
	TESReputation *reputation = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &reputation) && (!reputation || IS_ID(reputation, TESReputation)))
	{
		ExtraMapMarker *xMarker = GetExtraType(&thisObj->extraDataList, MapMarker);
		if (xMarker) xMarker->data->reputation = reputation;
	}
	return true;
}