#pragma once

DEFINE_COMMAND_PLUGIN(GetWeaponKillImpulse, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponKillImpulse, , 0, 2, kParams_OneForm_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponImpulseDistance, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponImpulseDistance, , 0, 2, kParams_OneForm_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponVATSEffect, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponVATSEffect, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetWeaponCritFlags, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponCritFlags, , 0, 2, kParams_OneObjectID_OneInt);

bool Cmd_GetWeaponKillImpulse_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgs(EXTRACT_ARGS, &weapon) && IS_TYPE(weapon, TESObjectWEAP))
		*result = weapon->killImpulse;
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponKillImpulse_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	float impulse;
	if (ExtractArgs(EXTRACT_ARGS, &weapon, &impulse) && IS_TYPE(weapon, TESObjectWEAP))
		weapon->killImpulse = impulse;
	return true;
}

bool Cmd_GetWeaponImpulseDistance_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgs(EXTRACT_ARGS, &weapon) && IS_TYPE(weapon, TESObjectWEAP))
		*result = weapon->impulseDist;
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponImpulseDistance_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	float impDist;
	if (ExtractArgs(EXTRACT_ARGS, &weapon, &impDist) && IS_TYPE(weapon, TESObjectWEAP))
		weapon->impulseDist = impDist;
	return true;
}

bool Cmd_GetWeaponVATSEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectWEAP *weapon;
	if (ExtractArgs(EXTRACT_ARGS, &weapon) && IS_TYPE(weapon, TESObjectWEAP) && weapon->VATSEffect)
		REFR_RES = weapon->VATSEffect->refID;
	return true;
}

bool Cmd_SetWeaponVATSEffect_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	SpellItem *effect;
	if (ExtractArgs(EXTRACT_ARGS, &weapon, &effect) && IS_TYPE(weapon, TESObjectWEAP) && IS_TYPE(effect, SpellItem))
		weapon->VATSEffect = effect;
	return true;
}

bool Cmd_GetWeaponCritFlags_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgs(EXTRACT_ARGS, &weapon) && IS_TYPE(weapon, TESObjectWEAP))
		*result = weapon->critDamageFlags;
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponCritFlags_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 flags;
	if (ExtractArgs(EXTRACT_ARGS, &weapon, &flags) && IS_TYPE(weapon, TESObjectWEAP))
		weapon->critDamageFlags = flags;
	return true;
}