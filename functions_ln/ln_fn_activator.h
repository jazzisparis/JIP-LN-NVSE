#pragma once

DEFINE_COMMAND_PLUGIN(GetActivatorRadioStation, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorRadioStation, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorPrompt, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorPrompt, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorWaterType, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorWaterType, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorSoundLooping, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorSoundLooping, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorSoundActivate, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorSoundActivate, 0, kParams_OneForm_OneOptionalForm);

bool Cmd_GetActivatorRadioStation_Execute(COMMAND_ARGS)
{
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &activator))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if (IS_ID(activator, TESObjectACTI) && activator->radioStation)
				REFR_RES = activator->radioStation->refID;
	return true;
}

bool Cmd_SetActivatorRadioStation_Execute(COMMAND_ARGS)
{
	BGSTalkingActivator *radio;
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &radio, &activator) && IS_ID(radio, BGSTalkingActivator))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if IS_ID(activator, TESObjectACTI)
				activator->radioStation = radio;
	return true;
}

bool Cmd_GetActivatorPrompt_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &activator))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if IS_ID(activator, TESObjectACTI)
				resStr = activator->activationPrompt.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetActivatorPrompt_Execute(COMMAND_ARGS)
{
	char prompt[0x80];
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &prompt, &activator))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if IS_ID(activator, TESObjectACTI)
				activator->activationPrompt.Set(prompt);
	return true;
}

bool Cmd_GetActivatorWaterType_Execute(COMMAND_ARGS)
{
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		if (form || (thisObj && (form = thisObj->baseForm)))
		{
			TESWaterForm *waterType = nullptr;
			if IS_ID(form, TESObjectACTI)
				waterType = ((TESObjectACTI*)form)->waterType;
			else if IS_ID(form, BGSPlaceableWater)
				waterType = ((BGSPlaceableWater*)form)->water;
			if (waterType)
				REFR_RES = waterType->refID;
		}
	return true;
}

bool Cmd_SetActivatorWaterType_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water, &form) && IS_ID(water, TESWaterForm))
		if (form || (thisObj && (form = thisObj->baseForm)))
			if IS_ID(form, TESObjectACTI)
				((TESObjectACTI*)form)->waterType = water;
			else if IS_ID(form, BGSPlaceableWater)
				((BGSPlaceableWater*)form)->water = water;
	return true;
}

bool Cmd_GetActivatorSoundLooping_Execute(COMMAND_ARGS)
{
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &activator))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if (IS_ID(activator, TESObjectACTI) && activator->loopingSound)
				REFR_RES = activator->loopingSound->refID;
	return true;
}

bool Cmd_SetActivatorSoundLooping_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &activator) && IS_ID(sound, TESSound))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if IS_ID(activator, TESObjectACTI)
				activator->loopingSound = sound;
	return true;
}

bool Cmd_GetActivatorSoundActivate_Execute(COMMAND_ARGS)
{
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &activator))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if (IS_ID(activator, TESObjectACTI) && activator->activationSound)
				REFR_RES = activator->activationSound->refID;
	return true;
}

bool Cmd_SetActivatorSoundActivate_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	TESObjectACTI *activator = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &activator) && IS_ID(sound, TESSound))
		if (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm)))
			if IS_ID(activator, TESObjectACTI)
				activator->activationSound = sound;
	return true;
}