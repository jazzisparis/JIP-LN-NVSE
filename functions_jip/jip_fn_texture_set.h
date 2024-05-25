#pragma once

DEFINE_COMMAND_PLUGIN(GetTextureSetTexture, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetTexture, 0, kParams_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetTextureSetTraitNumeric, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetTraitNumeric, 0, kParams_OneForm_OneInt_OneDouble);
DEFINE_COMMAND_PLUGIN(GetTextureSetFlag, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetTextureSetFlag, 0, kParams_OneForm_TwoInts);

bool Cmd_GetTextureSetTexture_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	BGSTextureSet *texSet;
	UInt32 mapID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &mapID) && IS_ID(texSet, BGSTextureSet) && (mapID <= 5))
		resStr = texSet->textures[mapID].ddsPath.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetTextureSetTexture_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 mapID;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &mapID, &path) && IS_ID(texSet, BGSTextureSet) && (mapID <= 5))
		texSet->textures[mapID].ddsPath.Set(path);
	return true;
}

bool Cmd_GetTextureSetTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &traitID) && IS_ID(texSet, BGSTextureSet) && texSet->decalInfo)
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
				cvtul2d(RGBHexToDec(texSet->decalInfo->color), result);
		}
	return true;
}

bool Cmd_SetTextureSetTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 traitID;
	double value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &traitID, &value) && IS_ID(texSet, BGSTextureSet) && (value >= 0) && texSet->decalInfo)
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
				texSet->decalInfo->parallaxPasses = (int)value;
				break;
			case 8:
				if (UInt32 decRGB = cvtd2ul(value); decRGB <= 255255255)
					texSet->decalInfo->color = RGBDecToHex(decRGB);
		}
	return true;
}

bool Cmd_GetTextureSetFlag_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &flagID) && IS_ID(texSet, BGSTextureSet) && (flagID <= 3))
		if (!flagID)
		{
			if (texSet->texFlags)
				*result = 1;
		}
		else if (texSet->decalInfo && (texSet->decalInfo->flags & (1 << (flagID - 1))))
			*result = 1;
	return true;
}

bool Cmd_SetTextureSetFlag_Execute(COMMAND_ARGS)
{
	BGSTextureSet *texSet;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &texSet, &flagID, &val) && IS_ID(texSet, BGSTextureSet) && (flagID <= 3))
		if (!flagID)
			texSet->texFlags = val != 0;
		else if (texSet->decalInfo)
			if (val)
				texSet->decalInfo->flags |= (1 << (flagID - 1));
			else
				texSet->decalInfo->flags &= ~(1 << (flagID - 1));
	return true;
}