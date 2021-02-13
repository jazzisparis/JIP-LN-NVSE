#pragma once

DEFINE_COMMAND_PLUGIN(GetAmmoTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetAmmoTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetAmmoProjectile, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetAmmoProjectile, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);

bool Cmd_GetAmmoTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESAmmo *ammo;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &traitID) || NOT_ID(ammo, TESAmmo)) return true;
	switch (traitID)
	{
	case 0:
		*result = ammo->speed;
		break;
	case 1:
		*result = (int)ammo->projPerShot;
		break;
	case 2:
		*result = ammo->ammoPercentConsumed;
		break;
	case 3:
		*result = ammo->clipRounds.clipRounds;
	}
	return true;
}

bool Cmd_SetAmmoTraitNumeric_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	UInt32 traitID;
	float val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &traitID, &val) || NOT_ID(ammo, TESAmmo)) return true;
	switch (traitID)
	{
	case 0:
		ammo->speed = val;
		break;
	case 1:
		ammo->projPerShot = (int)val;
		break;
	case 2:
		ammo->ammoPercentConsumed = val;
		break;
	case 3:
		ammo->clipRounds.clipRounds = val;
	}
	return true;
}

bool Cmd_GetAmmoProjectile_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESAmmo *ammo;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo) && IS_ID(ammo, TESAmmo) && ammo->projectile)
		REFR_RES = ammo->projectile->refID;
	return true;
}

bool Cmd_SetAmmoProjectile_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	BGSProjectile *proj;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &proj) && IS_ID(ammo, TESAmmo) && IS_ID(proj, BGSProjectile))
		ammo->projectile = proj;
	return true;
}