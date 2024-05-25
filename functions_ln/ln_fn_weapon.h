#pragma once

DEFINE_COMMAND_PLUGIN(GetWeaponKillImpulse, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponKillImpulse, 0, kParams_OneForm_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponImpulseDistance, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponImpulseDistance, 0, kParams_OneForm_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponVATSEffect, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponVATSEffect, 0, kParams_TwoForms);
DEFINE_COMMAND_PLUGIN(GetWeaponCritFlags, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponCritFlags, 0, kParams_OneObjectID_OneInt);

bool Cmd_GetWeaponKillImpulse_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = weapon->killImpulse;
	return true;
}

bool Cmd_SetWeaponKillImpulse_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	float impulse;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &impulse) && IS_ID(weapon, TESObjectWEAP))
		weapon->killImpulse = impulse;
	return true;
}

bool Cmd_GetWeaponImpulseDistance_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = weapon->impulseDist;
	return true;
}

bool Cmd_SetWeaponImpulseDistance_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	float impDist;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &impDist) && IS_ID(weapon, TESObjectWEAP))
		weapon->impulseDist = impDist;
	return true;
}

bool Cmd_GetWeaponVATSEffect_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP) && weapon->VATSEffect)
		REFR_RES = weapon->VATSEffect->refID;
	return true;
}

bool Cmd_SetWeaponVATSEffect_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	SpellItem *effect;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &effect) && IS_ID(weapon, TESObjectWEAP) && IS_ID(effect, SpellItem))
		weapon->VATSEffect = effect;
	return true;
}

bool Cmd_GetWeaponCritFlags_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = weapon->critDamageFlags;
	return true;
}

bool Cmd_SetWeaponCritFlags_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 flags;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &flags) && IS_ID(weapon, TESObjectWEAP))
		weapon->critDamageFlags = flags;
	return true;
}