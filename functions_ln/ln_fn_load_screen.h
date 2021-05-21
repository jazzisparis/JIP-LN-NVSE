#pragma once

DEFINE_COMMAND_PLUGIN(GetLoadScreenTexture, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLoadScreenTexture, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetLoadScreenType, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLoadScreenType, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetLoadScreenTypeTextRGB, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLoadScreenTypeTextRGB, , 0, 4, kParams_JIP_OneForm_ThreeFloats);

bool Cmd_GetLoadScreenTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESLoadScreen *loadScreen;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScreen) && IS_ID(loadScreen, TESLoadScreen))
		resStr = loadScreen->texture.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetLoadScreenTexture_Execute(COMMAND_ARGS)
{
	TESLoadScreen *loadScreen;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScreen, &path) && IS_ID(loadScreen, TESLoadScreen))
		loadScreen->texture.ddsPath.Set(path);
	return true;
}

bool Cmd_GetLoadScreenType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESLoadScreen *loadScreen;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScreen) && IS_ID(loadScreen, TESLoadScreen) && loadScreen->type)
		REFR_RES = loadScreen->type->refID;
	return true;
}

bool Cmd_SetLoadScreenType_Execute(COMMAND_ARGS)
{
	TESLoadScreen *loadScreen;
	TESLoadScreenType *loadScrType;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScreen, &loadScrType) && IS_ID(loadScreen, TESLoadScreen) && IS_ID(loadScrType, TESLoadScreenType))
		loadScreen->type = loadScrType;
	return true;
}

bool Cmd_GetLoadScreenTypeTextRGB_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESLoadScreenType *loadScrType;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &loadScrType) || NOT_ID(loadScrType, TESLoadScreenType)) return true;
	NVSEArrayVar *outArray = CreateStringMap(NULL, NULL, 0, scriptObj);
	SetElement(outArray, ArrayElementL("r"), ArrayElementL(loadScrType->fontcolor1.R));
	SetElement(outArray, ArrayElementL("g"), ArrayElementL(loadScrType->fontcolor1.G));
	SetElement(outArray, ArrayElementL("b"), ArrayElementL(loadScrType->fontcolor1.B));
	AssignCommandResult(outArray, result);
	return true;
}

bool Cmd_SetLoadScreenTypeTextRGB_Execute(COMMAND_ARGS)
{
	TESLoadScreenType *loadScrType;
	float inR, inG, inB;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &loadScrType, &inR, &inG, &inB) || NOT_ID(loadScrType, TESLoadScreenType)) return true;
	loadScrType->fontcolor1.R = inR;
	loadScrType->fontcolor1.G = inG;
	loadScrType->fontcolor1.B = inB;
	return true;
}