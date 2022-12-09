#pragma once

DEFINE_COMMAND_PLUGIN(IsMapMarker, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetMapMarkerName, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerName, 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetMapMarkerVisible, 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerTravel, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerTravel, 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerHidden, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerHidden, 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerType, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerType, 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMapMarkerRep, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetMapMarkerRep, 1, 1, kParams_OneOptionalForm);

bool Cmd_IsMapMarker_Execute(COMMAND_ARGS)
{
	*result = (thisObj->baseForm->refID == 0x10) ? 1 : 0;
	return true;
}

bool Cmd_GetMapMarkerName_Execute(COMMAND_ARGS)
{
	MapMarkerData *markerData = thisObj->GetMapMarkerData();
	AssignString(PASS_COMMAND_ARGS, markerData ? markerData->fullName.name.m_data : nullptr);
	return true;
}

bool Cmd_SetMapMarkerName_Execute(COMMAND_ARGS)
{
	char nameStr[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nameStr))
	{
		MapMarkerData *markerData = thisObj->GetMapMarkerData();
		if (markerData) markerData->fullName.name.Set(nameStr);
	}
	return true;
}

bool Cmd_SetMapMarkerVisible_Execute(COMMAND_ARGS)
{
	UInt32 visible;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &visible))
	{
		MapMarkerData *markerData = thisObj->GetMapMarkerData();
		if (markerData)
		{
			if (visible)
				markerData->flags |= 1;
			else
				markerData->flags &= ~1;
		}
	}
	return true;
}

bool Cmd_GetMapMarkerTravel_Execute(COMMAND_ARGS)
{
	MapMarkerData *markerData = thisObj->GetMapMarkerData();
	*result = (markerData && (markerData->flags & 2)) ? 1 : 0;
	return true;
}

bool Cmd_SetMapMarkerTravel_Execute(COMMAND_ARGS)
{
	UInt32 travel;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &travel))
	{
		MapMarkerData *markerData = thisObj->GetMapMarkerData();
		if (markerData)
		{
			if (travel)
				markerData->flags |= 2;
			else
				markerData->flags &= ~2;
		}
	}
	return true;
}

bool Cmd_GetMapMarkerHidden_Execute(COMMAND_ARGS)
{
	MapMarkerData *markerData = thisObj->GetMapMarkerData();
	*result = (markerData && (markerData->flags & 4)) ? 1 : 0;
	return true;
}

bool Cmd_SetMapMarkerHidden_Execute(COMMAND_ARGS)
{
	UInt32 hidden;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hidden))
	{
		MapMarkerData *markerData = thisObj->GetMapMarkerData();
		if (markerData)
		{
			if (hidden)
				markerData->flags |= 4;
			else
				markerData->flags &= ~4;
		}
	}
	return true;
}

bool Cmd_GetMapMarkerType_Execute(COMMAND_ARGS)
{
	MapMarkerData *markerData = thisObj->GetMapMarkerData();
	*result = markerData ? markerData->type : -1;
	return true;
}

bool Cmd_SetMapMarkerType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type) && (type <= 14))
	{
		MapMarkerData *markerData = thisObj->GetMapMarkerData();
		if (markerData) markerData->type = type;
	}
	return true;
}

bool Cmd_GetMapMarkerRep_Execute(COMMAND_ARGS)
{
	MapMarkerData *markerData = thisObj->GetMapMarkerData();
	TESForm *reputation = markerData ? markerData->reputation : nullptr;
	REFR_RES = reputation ? reputation->refID : 0;
	DoConsolePrint(reputation);
	return true;
}

bool Cmd_SetMapMarkerRep_Execute(COMMAND_ARGS)
{
	TESReputation *reputation = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &reputation) && (!reputation || IS_ID(reputation, TESReputation)))
	{
		MapMarkerData *markerData = thisObj->GetMapMarkerData();
		if (markerData) markerData->reputation = reputation;
	}
	return true;
}