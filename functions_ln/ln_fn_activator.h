#pragma once

DEFINE_COMMAND_PLUGIN(GetActivatorRadioStation, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorRadioStation, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorPrompt, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorPrompt, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorWaterType, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorWaterType, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorSoundLooping, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorSoundLooping, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetActivatorSoundActivate, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetActivatorSoundActivate, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);

bool Cmd_GetActivatorRadioStation_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &activator)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if (IS_TYPE(activator, TESObjectACTI) && activator->radioStation)
		REFR_RES = activator->radioStation->refID;
	return true;
}

bool Cmd_SetActivatorRadioStation_Execute(COMMAND_ARGS)
{
	BGSTalkingActivator *radio;
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &radio, &activator) || NOT_TYPE(radio, BGSTalkingActivator)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if IS_TYPE(activator, TESObjectACTI)
		activator->radioStation = radio;
	return true;
}

bool Cmd_GetActivatorPrompt_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESObjectACTI *activator = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &activator) && (activator || (thisObj && (activator = (TESObjectACTI*)thisObj->baseForm))) && IS_TYPE(activator, TESObjectACTI))
		resStr = activator->activationPrompt.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetActivatorPrompt_Execute(COMMAND_ARGS)
{
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &activator)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if IS_TYPE(activator, TESObjectACTI)
		activator->activationPrompt.Set(s_strArgBuffer);
	return true;
}

bool Cmd_GetActivatorWaterType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	TESWaterForm *waterType = NULL;
	if IS_TYPE(form, TESObjectACTI)
		waterType = ((TESObjectACTI*)form)->waterType;
	else if IS_TYPE(form, BGSPlaceableWater)
		waterType = ((BGSPlaceableWater*)form)->water;
	if (waterType) REFR_RES = waterType->refID;
	return true;
}

bool Cmd_SetActivatorWaterType_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	TESForm *form = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &water, &form) || NOT_TYPE(water, TESWaterForm)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	if IS_TYPE(form, TESObjectACTI)
		((TESObjectACTI*)form)->waterType = water;
	else if IS_TYPE(form, BGSPlaceableWater)
		((BGSPlaceableWater*)form)->water = water;
	return true;
}

bool Cmd_GetActivatorSoundLooping_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &activator)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if (IS_TYPE(activator, TESObjectACTI) && activator->loopingSound)
		REFR_RES = activator->loopingSound->refID;
	return true;
}

bool Cmd_SetActivatorSoundLooping_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &sound, &activator) || NOT_TYPE(sound, TESSound)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if IS_TYPE(activator, TESObjectACTI)
		activator->loopingSound = sound;
	return true;
}

bool Cmd_GetActivatorSoundActivate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &activator)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if (IS_TYPE(activator, TESObjectACTI) && activator->activationSound)
		REFR_RES = activator->activationSound->refID;
	return true;
}

bool Cmd_SetActivatorSoundActivate_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	TESObjectACTI *activator = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &sound, &activator) || NOT_TYPE(sound, TESSound)) return true;
	if (!activator)
	{
		if (!thisObj) return true;
		activator = (TESObjectACTI*)thisObj->baseForm;
	}
	if IS_TYPE(activator, TESObjectACTI)
		activator->activationSound = sound;
	return true;
}