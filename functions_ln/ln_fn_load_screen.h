#pragma once

DEFINE_COMMAND_PLUGIN(GetLoadScreenTexture, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLoadScreenTexture, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetLoadScreenType, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLoadScreenType, 0, kParams_TwoForms);
DEFINE_COMMAND_PLUGIN(GetLoadScreenTypeTextRGB, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLoadScreenTypeTextRGB, 0, kParams_OneForm_ThreeFloats);

bool Cmd_GetLoadScreenTexture_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESLoadScreen *loadScreen;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScreen) && IS_ID(loadScreen, TESLoadScreen))
		resStr = loadScreen->texture.ddsPath.m_data;
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

const char *kRGBPrefixes[] = {"r", "g", "b"};

bool Cmd_GetLoadScreenTypeTextRGB_Execute(COMMAND_ARGS)
{
	TESLoadScreenType *loadScrType;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScrType) && IS_ID(loadScrType, TESLoadScreenType))
	{
		ArrayElementL values[3] = {loadScrType->fontColor1.r, loadScrType->fontColor1.g, loadScrType->fontColor1.b};
		*result = (int)CreateStringMap(kRGBPrefixes, values, 3, scriptObj);
	}
	return true;
}

bool Cmd_SetLoadScreenTypeTextRGB_Execute(COMMAND_ARGS)
{
	TESLoadScreenType *loadScrType;
	NiColor inRGB;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &loadScrType, &inRGB.r, &inRGB.g, &inRGB.b) && IS_ID(loadScrType, TESLoadScreenType))
		loadScrType->fontColor1 = inRGB;
	return true;
}