#pragma once

DEFINE_COMMAND_PLUGIN(GetWaterNoiseTexture, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWaterNoiseTexture, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetWaterFormEffect, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetWaterSound, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWaterSound, 0, kParams_OneForm_OneOptionalForm);

bool Cmd_GetWaterNoiseTexture_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESWaterForm *water;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water) && IS_ID(water, TESWaterForm))
		resStr = water->noiseMap.ddsPath.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWaterNoiseTexture_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water, &path) && IS_ID(water, TESWaterForm))
		water->noiseMap.ddsPath.Set(path);
	return true;
}

bool Cmd_SetWaterFormEffect_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	SpellItem *effect = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water, &effect) && IS_ID(water, TESWaterForm) && (!effect || IS_ID(effect, SpellItem)))
		water->drinkEffect = effect;
	return true;
}

bool Cmd_GetWaterSound_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water) && IS_ID(water, TESWaterForm) && water->sound)
		REFR_RES = water->sound->refID;
	return true;
}

bool Cmd_SetWaterSound_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	TESSound *sound = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water, &sound) && IS_ID(water, TESWaterForm) && (!sound || IS_ID(sound, TESSound)))
		water->sound = sound;
	return true;
}