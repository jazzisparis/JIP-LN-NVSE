#pragma once

DEFINE_COMMAND_PLUGIN(GetWaterNoiseTexture, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWaterNoiseTexture, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetWaterFormEffect, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWaterSound, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWaterSound, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);

bool Cmd_GetWaterNoiseTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESWaterForm *water;
	if (ExtractArgs(EXTRACT_ARGS, &water) && IS_TYPE(water, TESWaterForm))
		resStr = water->noiseMap.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWaterNoiseTexture_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	if (ExtractArgs(EXTRACT_ARGS, &water, &s_strArgBuffer) && IS_TYPE(water, TESWaterForm))
		water->noiseMap.ddsPath.Set(s_strArgBuffer);
	return true;
}

bool Cmd_SetWaterFormEffect_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	SpellItem *effect = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &water, &effect) && IS_TYPE(water, TESWaterForm) && (!effect || IS_TYPE(effect, SpellItem)))
		water->drinkEffect = effect;
	return true;
}

bool Cmd_GetWaterSound_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWaterForm *water;
	if (ExtractArgs(EXTRACT_ARGS, &water) && IS_TYPE(water, TESWaterForm) && water->sound)
		REFR_RES = water->sound->refID;
	return true;
}

bool Cmd_SetWaterSound_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	TESSound *sound = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &water, &sound) && IS_TYPE(water, TESWaterForm) && (!sound || IS_TYPE(sound, TESSound)))
		water->sound = sound;
	return true;
}