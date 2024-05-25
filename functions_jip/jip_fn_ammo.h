#pragma once

DEFINE_COMMAND_PLUGIN(GetAmmoTraitNumeric, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetAmmoTraitNumeric, 0, kParams_OneObjectID_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetAmmoProjectile, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetAmmoProjectile, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetAmmoCasing, 0, kParams_OneObjectID_OneOptionalObjectID);

bool Cmd_GetAmmoTraitNumeric_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &traitID) && IS_TYPE(ammo, TESAmmo))
	{
		switch (traitID)
		{
			case 0:
				*result = ammo->speed;
				break;
			case 1:
				*result = (int)ammo->projPerShot;
				break;
			case 2:
				*result = ammo->casingChance;
				break;
			case 3:
				*result = ammo->clipRounds.clipRounds;
		}
	}
	return true;
}

bool Cmd_SetAmmoTraitNumeric_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	UInt32 traitID;
	float val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &traitID, &val) && IS_TYPE(ammo, TESAmmo))
	{
		switch (traitID)
		{
			case 0:
				ammo->speed = val;
				break;
			case 1:
				ammo->projPerShot = (int)val;
				break;
			case 2:
				ammo->casingChance = val;
				break;
			case 3:
				ammo->clipRounds.clipRounds = val;
		}
	}
	return true;
}

bool Cmd_GetAmmoProjectile_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo) && IS_TYPE(ammo, TESAmmo) && ammo->projectile)
		REFR_RES = ammo->projectile->refID;
	return true;
}

bool Cmd_SetAmmoProjectile_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	BGSProjectile *proj = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &proj) && IS_TYPE(ammo, TESAmmo) && (!proj || IS_ID(proj, BGSProjectile)))
		ammo->projectile = proj;
	return true;
}

bool Cmd_SetAmmoCasing_Execute(COMMAND_ARGS)
{
	TESAmmo *ammo;
	TESBoundObject *casing = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ammo, &casing) && IS_TYPE(ammo, TESAmmo) && (!casing || kInventoryType[casing->typeID]))
		ammo->casingItem = casing;
	return true;
}