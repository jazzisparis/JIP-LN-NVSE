#pragma once

DEFINE_COMMAND_PLUGIN(IsMapMarker, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetMapMarkerName, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetMapMarkerName, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetMapMarkerVisible, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerTravel, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetMapMarkerTravel, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerHidden, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetMapMarkerHidden, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerType, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetMapMarkerType, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerRep, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetMapMarkerRep, 1, kParams_OneOptionalForm);

bool Cmd_IsMapMarker_Execute(COMMAND_ARGS)
{
	if (thisObj->baseForm->refID == 0x10)
		*result = 1;
	return true;
}

bool Cmd_GetMapMarkerName_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	if (auto markerData = thisObj->GetMapMarkerData())
		resStr = markerData->fullName.name.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetMapMarkerName_Execute(COMMAND_ARGS)
{
	char nameStr[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nameStr))
		if (auto markerData = thisObj->GetMapMarkerData())
			markerData->fullName.name.Set(nameStr);
	return true;
}

bool Cmd_SetMapMarkerVisible_Execute(COMMAND_ARGS)
{
	UInt32 visible;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &visible))
		if (auto markerData = thisObj->GetMapMarkerData())
			if (visible)
				markerData->flags |= 1;
			else
				markerData->flags &= ~1;
	return true;
}

bool Cmd_GetMapMarkerTravel_Execute(COMMAND_ARGS)
{
	if (auto markerData = thisObj->GetMapMarkerData(); markerData && (markerData->flags & 2))
		*result = 1;
	return true;
}

bool Cmd_SetMapMarkerTravel_Execute(COMMAND_ARGS)
{
	UInt32 travel;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &travel))
		if (auto markerData = thisObj->GetMapMarkerData())
			if (travel)
				markerData->flags |= 2;
			else
				markerData->flags &= ~2;
	return true;
}

bool Cmd_GetMapMarkerHidden_Execute(COMMAND_ARGS)
{
	if (auto markerData = thisObj->GetMapMarkerData(); markerData && (markerData->flags & 4))
		*result = 1;
	return true;
}

bool Cmd_SetMapMarkerHidden_Execute(COMMAND_ARGS)
{
	UInt32 hidden;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hidden))
		if (auto markerData = thisObj->GetMapMarkerData())
			if (hidden)
				markerData->flags |= 4;
			else
				markerData->flags &= ~4;
	return true;
}

bool Cmd_GetMapMarkerType_Execute(COMMAND_ARGS)
{
	int type = -1;
	if (auto markerData = thisObj->GetMapMarkerData())
		type = markerData->type;
	*result = type;
	return true;
}

bool Cmd_SetMapMarkerType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type) && (type <= 14))
		if (auto markerData = thisObj->GetMapMarkerData())
			markerData->type = type;
	return true;
}

bool Cmd_GetMapMarkerRep_Execute(COMMAND_ARGS)
{
	if (auto markerData = thisObj->GetMapMarkerData(); markerData && markerData->reputation)
		REFR_RES = markerData->reputation->refID;
	return true;
}

bool Cmd_SetMapMarkerRep_Execute(COMMAND_ARGS)
{
	TESReputation *reputation = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &reputation) && (!reputation || IS_ID(reputation, TESReputation)))
		if (auto markerData = thisObj->GetMapMarkerData())
			markerData->reputation = reputation;
	return true;
}