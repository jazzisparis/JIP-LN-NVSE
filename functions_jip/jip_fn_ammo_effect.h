#pragma once

DEFINE_COMMAND_PLUGIN(GetNumAmmoEffects, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthAmmoEffect, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(AddAmmoEffect, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(RemoveAmmoEffect, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetAmmoEffectTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetAmmoEffectTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);

bool Cmd_GetNumAmmoEffects_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo) && IS_ID(ammo, TESAmmo))
		*result = (int)ammo->effectList.Count();
	else *result = 0;
	return true;
}

bool Cmd_GetNthAmmoEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESAmmo *ammo;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &idx) && IS_ID(ammo, TESAmmo))
	{
		TESAmmoEffect *effect = ammo->effectList.GetNthItem(idx);
		if (effect) REFR_RES = effect->refID;
	}
	return true;
}

bool Cmd_AddAmmoEffect_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	TESAmmoEffect *effect;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &effect) && IS_ID(ammo, TESAmmo) && IS_ID(effect, TESAmmoEffect) && !ammo->effectList.IsInList(effect))
	{
		ammo->effectList.Prepend(effect);
		*result = 1;
	}
	else *result = 0;
	return true;
}

bool Cmd_RemoveAmmoEffect_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	TESAmmoEffect *effect;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &effect) && IS_ID(ammo, TESAmmo) && IS_ID(effect, TESAmmoEffect) && ammo->effectList.Remove(effect))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetAmmoEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESAmmoEffect *effect;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &effect, &traitID) || NOT_ID(effect, TESAmmoEffect)) return true;
	switch (traitID)
	{
	case 0:
		*result = (int)effect->type;
		break;
	case 1:
		*result = (int)effect->operation;
		break;
	case 2:
		*result = effect->value;
	}
	return true;
}

bool Cmd_SetAmmoEffectTraitNumeric_Execute(COMMAND_ARGS)
{
	TESAmmoEffect *effect;
	UInt32 traitID;
	float fVal;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &effect, &traitID, &fVal) || NOT_ID(effect, TESAmmoEffect) || (fVal < 0)) return true;
	UInt32 iVal = (int)fVal;
	switch (traitID)
	{
	case 0:
		if (iVal < 6) effect->type = iVal;
		break;
	case 1:
		if (iVal < 3) effect->operation = iVal;
		break;
	case 2:
		effect->value = fVal;
	}
	return true;
}