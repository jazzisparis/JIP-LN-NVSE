#pragma once

DEFINE_COMMAND_PLUGIN(GetWorldspaceFlag, , 0, 2, kParams_JIP_OneWorldspace_OneInt);
DEFINE_COMMAND_PLUGIN(SetWorldspaceFlag, , 0, 3, kParams_JIP_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetWorldspaceClimate, , 0, 1, kParams_JIP_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceClimate, , 0, 2, kParams_JIP_OneWorldspace_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWorldspaceNoiseTexture, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWorldspaceNoiseTexture, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetWorldspaceWaterType, , 0, 1, kParams_JIP_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceWaterType, , 0, 2, kParams_JIP_OneWorldspace_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWorldspaceImagespace, , 0, 1, kParams_JIP_OneWorldspace);
DEFINE_COMMAND_PLUGIN(SetWorldspaceImagespace, , 0, 2, kParams_JIP_OneWorldspace_OneOptionalForm);

bool Cmd_GetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wspc;
	UInt32 flagID;
	if (ExtractArgs(EXTRACT_ARGS, &wspc, &flagID) && (flagID <= 7))
		*result = (wspc->flags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetWorldspaceFlag_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wspc;
	UInt32 flagID, val;
	if (ExtractArgs(EXTRACT_ARGS, &wspc, &flagID, &val) && (flagID <= 7))
	{
		flagID = 1 << flagID;
		if (val) wspc->flags |= flagID;
		else wspc->flags &= ~flagID;
	}
	return true;
}

bool Cmd_GetWorldspaceClimate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgs(EXTRACT_ARGS, &wldSpc))
	{
		while (wldSpc->parent && (wldSpc->parentFlags & 0x10))
			wldSpc = wldSpc->parent;
		TESClimate *climate = wldSpc->climate;
		if (climate) REFR_RES = climate->refID;
		DoConsolePrint(climate);
	}
	return true;
}

bool Cmd_SetWorldspaceClimate_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wldSpc;
	TESClimate *climate = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &wldSpc, &climate) && (!climate || IS_TYPE(climate, TESClimate)))
	{
		if (wldSpc->parent && (wldSpc->parentFlags & 16)) wldSpc->parentFlags -= 16;
		wldSpc->climate = climate;
		if (!*g_currentSky || !g_thePlayer->parentCell) return true;
		TESWorldSpace *pcWspc = g_thePlayer->parentCell->worldSpace;
		if (!pcWspc) return true;
		while (pcWspc->parent && (pcWspc->parentFlags & 16)) pcWspc = pcWspc->parent;
		if (pcWspc == wldSpc) (*g_currentSky)->currClimate = climate;
	}
	return true;
}

bool Cmd_GetWorldspaceNoiseTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESWorldSpace *wspc;
	if (ExtractArgs(EXTRACT_ARGS, &wspc) && IS_TYPE(wspc, TESWorldSpace))
		resStr = wspc->waterNoiseTexture.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWorldspaceNoiseTexture_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wspc;
	if (ExtractArgs(EXTRACT_ARGS, &wspc, &s_strArgBuffer) && IS_TYPE(wspc, TESWorldSpace))
		wspc->waterNoiseTexture.ddsPath.Set(s_strArgBuffer);
	return true;
}

bool Cmd_GetWorldspaceWaterType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgs(EXTRACT_ARGS, &wldSpc) && wldSpc->waterFormFirst)
		REFR_RES = wldSpc->waterFormFirst->refID;
	return true;
}

bool Cmd_SetWorldspaceWaterType_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wldSpc;
	TESWaterForm *water = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &wldSpc, &water) && (!water || IS_TYPE(water, TESWaterForm)))
		wldSpc->waterFormFirst = water;
	return true;
}

bool Cmd_GetWorldspaceImagespace_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wldSpc;
	if (ExtractArgs(EXTRACT_ARGS, &wldSpc) && wldSpc->imageSpace)
		REFR_RES = wldSpc->imageSpace->refID;
	return true;
}

bool Cmd_SetWorldspaceImagespace_Execute(COMMAND_ARGS)
{
	TESWorldSpace *wldSpc;
	TESImageSpace *imgSpc = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &wldSpc, &imgSpc) && (!imgSpc || IS_TYPE(imgSpc, TESImageSpace)))
		wldSpc->imageSpace = imgSpc;
	return true;
}