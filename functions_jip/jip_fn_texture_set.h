#pragma once

DEFINE_COMMAND_PLUGIN(GetTextureSetTexture, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetTexture, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetTextureSetTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneDouble);
DEFINE_COMMAND_PLUGIN(GetTextureSetFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);

bool Cmd_GetTextureSetTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	BGSTextureSet *texSet;
	UInt32 mapID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &mapID) && IS_ID(texSet, BGSTextureSet) && (mapID <= 5))
		resStr = texSet->textures[mapID].ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetTextureSetTexture_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 mapID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &mapID, &s_strArgBuffer) && IS_ID(texSet, BGSTextureSet) && (mapID <= 5))
		texSet->textures[mapID].ddsPath.Set(s_strArgBuffer);
	return true;
}

bool Cmd_GetTextureSetTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSTextureSet *texSet;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &traitID) || NOT_ID(texSet, BGSTextureSet) || !texSet->decalInfo) return true;
	switch (traitID)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			*result = ((float*)texSet->decalInfo)[traitID];
			break;
		case 7:
			*result = texSet->decalInfo->parallaxPasses;
			break;
		case 8:
			*result = RGBHexToDec(texSet->decalInfo->color);
	}
	return true;
}

bool Cmd_SetTextureSetTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 traitID;
	double value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &traitID, &value) || NOT_ID(texSet, BGSTextureSet) || (value < 0) || !texSet->decalInfo) return true;
	UInt32 intVal = (int)value;
	switch (traitID)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			((float*)texSet->decalInfo)[traitID] = value;
			break;
		case 7:
			texSet->decalInfo->parallaxPasses = intVal;
			break;
		case 8:
			if (intVal <= 255255255) texSet->decalInfo->color = RGBDecToHex(intVal);
	}
	return true;
}

bool Cmd_GetTextureSetFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSTextureSet *texSet;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &flagID) && IS_ID(texSet, BGSTextureSet) && (flagID <= 3))
	{
		if (!flagID) *result = texSet->texFlags ? 1 : 0;
		else if (texSet->decalInfo) *result = (texSet->decalInfo->flags & (1 << (flagID - 1))) ? 1 : 0;
	}
	return true;
}

bool Cmd_SetTextureSetFlag_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 flagID, val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &flagID, &val) || NOT_ID(texSet, BGSTextureSet) || (flagID > 3)) return true;
	if (!flagID) texSet->texFlags = val ? 1 : 0;
	else if (texSet->decalInfo)
	{
		if (val) texSet->decalInfo->flags |= (1 << (flagID - 1));
		else texSet->decalInfo->flags &= ~(1 << (flagID - 1));
	}
	return true;
}