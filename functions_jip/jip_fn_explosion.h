#pragma once

DEFINE_COMMAND_PLUGIN(GetExplosionTraitNumeric, 0, kParams_OneBoundObject_OneInt);
DEFINE_COMMAND_PLUGIN(SetExplosionTraitNumeric, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetExplosionTraitForm, 0, kParams_OneBoundObject_OneInt);
DEFINE_COMMAND_PLUGIN(SetExplosionTraitForm, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetExplosionFlag, 0, kParams_OneBoundObject_OneInt);
DEFINE_COMMAND_PLUGIN(SetExplosionFlag, 0, kParams_OneForm_TwoInts);

bool Cmd_GetExplosionTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSExplosion *explosion;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &explosion, &traitID) && IS_ID(explosion, BGSExplosion))
		switch (traitID)
		{
			case 0:
				*result = explosion->force;
				break;
			case 1:
				*result = explosion->damage;
				break;
			case 2:
				*result = explosion->radius;
				break;
			case 3:
				*result = explosion->ISradius;
				break;
			case 4:
				*result = explosion->RADlevel;
				break;
			case 5:
				*result = explosion->dissipationTime;
				break;
			case 6:
				*result = explosion->RADradius;
				break;
			case 7:
				*result = explosion->soundLevel;
		}
	return true;
}

bool Cmd_SetExplosionTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSExplosion *explosion;
	UInt32 traitID;
	float val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &explosion, &traitID, &val) && IS_ID(explosion, BGSExplosion))
		switch (traitID)
		{
			case 0:
				explosion->force = val;
				break;
			case 1:
				explosion->damage = val;
				break;
			case 2:
				explosion->radius = val;
				break;
			case 3:
				explosion->ISradius = val;
				break;
			case 4:
				explosion->RADlevel = val;
				break;
			case 5:
				explosion->dissipationTime = val;
				break;
			case 6:
				explosion->RADradius = val;
				break;
			case 7:
				explosion->soundLevel = val;
		}
	return true;
}

bool Cmd_GetExplosionTraitForm_Execute(COMMAND_ARGS)
{
	BGSExplosion *explosion;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &explosion, &traitID) && IS_ID(explosion, BGSExplosion))
		switch (traitID)
		{
			case 0:
				if (explosion->light)
					REFR_RES = explosion->light->refID;
				break;
			case 1:
				if (explosion->sound1)
					REFR_RES = explosion->sound1->refID;
				break;
			case 2:
				if (explosion->sound2)
					REFR_RES = explosion->sound2->refID;
				break;
			case 3:
				if (explosion->impactDataSet)
					REFR_RES = explosion->impactDataSet->refID;
				break;
			case 4:
				if (explosion->placedObj)
					REFR_RES = explosion->placedObj->refID;
				break;
			case 5:
				if (explosion->enchantable.enchantItem)
					REFR_RES = explosion->enchantable.enchantItem->refID;
				break;
			case 6:
				if (explosion->imageSpaceModForm.imod)
					REFR_RES = explosion->imageSpaceModForm.imod->refID;
				break;
		}
	return true;
}

bool Cmd_SetExplosionTraitForm_Execute(COMMAND_ARGS)
{
	BGSExplosion *explosion;
	UInt32 traitID;
	TESForm *object = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &explosion, &traitID, &object) && IS_ID(explosion, BGSExplosion))
		switch (traitID)
		{
			case 0:
				explosion->light = (object && IS_ID(object, TESObjectLIGH)) ? (TESObjectLIGH*)object : NULL;
				break;
			case 1:
				explosion->sound1 = (object && IS_ID(object, TESSound)) ? (TESSound*)object : NULL;
				break;
			case 2:
				explosion->sound2 = (object && IS_ID(object, TESSound)) ? (TESSound*)object : NULL;
				break;
			case 3:
				explosion->impactDataSet = (object && IS_ID(object, BGSImpactDataSet)) ? (BGSImpactDataSet*)object : NULL;
				break;
			case 4:
				explosion->placedObj = object;
				break;
			case 5:
				explosion->enchantable.enchantItem = (object && IS_ID(object, EnchantmentItem)) ? (EnchantmentItem*)object : NULL;
				break;
			case 6:
				explosion->imageSpaceModForm.imod = (object && IS_ID(object, TESImageSpaceModifier)) ? (TESImageSpaceModifier*)object : NULL;
				break;
		}
	return true;
}

bool Cmd_GetExplosionFlag_Execute(COMMAND_ARGS)
{
	BGSExplosion *explosion;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &explosion, &flagID) && IS_ID(explosion, BGSExplosion) && flagID && (flagID <= 31) && (explosion->explFlags & (1 << flagID)))
		*result = 1;
	return true;
}

bool Cmd_SetExplosionFlag_Execute(COMMAND_ARGS)
{
	BGSExplosion *explosion;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &explosion, &flagID, &val) && IS_ID(explosion, BGSExplosion) && flagID && (flagID <= 31))
		explosion->SetFlag(1 << flagID, val != 0);
	return true;
}