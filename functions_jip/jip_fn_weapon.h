#pragma once

DEFINE_COMMAND_PLUGIN(GetWeaponDetectionSoundLevel, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponDetectionSoundLevel, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_CMD_ALT_COND_PLUGIN(IsEquippedWeaponSilenced, , , 1, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(IsEquippedWeaponScoped, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(GetWeaponSound, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponSound, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalSound);
DEFINE_COMMAND_PLUGIN(SetWeaponItemMod, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponItemModEffect, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWeaponItemModValue, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponModReloadAnim, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponModReloadAnim, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(GetWeaponShellCasingModel, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponShellCasingModel, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetEmbeddedWeaponNode, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetEmbeddedWeaponAV, , 0, 2, kParams_JIP_OneObjectID_OneActorValue);
DEFINE_COMMAND_PLUGIN(GetCalculatedWeaponDamage, , 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(GetWeaponOnHitSetting, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponOnHitSetting, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(GetWeaponSemiAutoFireDelay, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponSemiAutoFireDelay, , 0, 3, kParams_JIP_OneObjectID_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponModel, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponModel, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_CMD_ALT_COND_PLUGIN(EquippedWeaponHasModType, , , 1, kParams_OneInt);

bool Cmd_GetWeaponDetectionSoundLevel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = (int)weapon->soundLevel;
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponDetectionSoundLevel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 level;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &level) && IS_ID(weapon, TESObjectWEAP) && (level <= 2))
		weapon->soundLevel = level;
	return true;
}

bool Cmd_IsEquippedWeaponSilenced_Eval(COMMAND_ARGS_EVAL)
{
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->EquippedWeaponHasMod(11) : 0;
	return true;
}

bool Cmd_IsEquippedWeaponSilenced_Execute(COMMAND_ARGS)
{
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->EquippedWeaponHasMod(11) : 0;
	return true;
}

bool Cmd_IsEquippedWeaponScoped_Eval(COMMAND_ARGS_EVAL)
{
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->EquippedWeaponHasMod(14) : 0;
	return true;
}

bool Cmd_IsEquippedWeaponScoped_Execute(COMMAND_ARGS)
{
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->EquippedWeaponHasMod(14) : 0;
	return true;
}

bool Cmd_GetWeaponSound_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectWEAP *weapon;
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &type) && IS_ID(weapon, TESObjectWEAP) && (type <= 13))
	{
		TESSound *sound = (type < 12) ? weapon->sounds[type] : ((type == 12) ? weapon->pickupPutdownSounds.pickupSound : weapon->pickupPutdownSounds.putdownSound);
		if (sound) REFR_RES = sound->refID;
	}
	return true;
}

bool Cmd_SetWeaponSound_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 type;
	TESSound *newSound = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &type, &newSound) || NOT_ID(weapon, TESObjectWEAP) || (type > 13)) return true;
	if (type < 12)
		weapon->sounds[type] = newSound;
	else if (type == 12)
		weapon->pickupPutdownSounds.pickupSound = newSound;
	else
		weapon->pickupPutdownSounds.putdownSound = newSound;
	return true;
}

bool Cmd_SetWeaponItemMod_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 idx;
	TESObjectIMOD *wmod = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &idx, &wmod) && IS_ID(weapon, TESObjectWEAP) && idx && (idx <= 3))
		weapon->itemMod[idx - 1] = (wmod && IS_ID(wmod, TESObjectIMOD)) ? wmod : NULL;
	return true;
}

bool Cmd_SetWeaponItemModEffect_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 idx, type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &idx, &type) && IS_ID(weapon, TESObjectWEAP) && idx && (idx <= 3) && type && (type <= 14) && (type != 5))
		weapon->effectMods[idx - 1] = type;
	return true;
}

bool Cmd_SetWeaponItemModValue_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 idx;
	float val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &idx, &val) && IS_ID(weapon, TESObjectWEAP) && idx && (idx <= 3))
		weapon->value1Mod[idx - 1] = val;
	return true;
}

bool Cmd_GetWeaponModReloadAnim_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = weapon->modReloadAnim;
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponModReloadAnim_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 anim;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &anim) && IS_ID(weapon, TESObjectWEAP) && (anim <= 22))
		weapon->modReloadAnim = anim;
	return true;
}

bool Cmd_GetWeaponShellCasingModel_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		resStr = weapon->shellCasingModel.GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWeaponShellCasingModel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &s_strArgBuffer) && IS_ID(weapon, TESObjectWEAP))
		weapon->shellCasingModel.SetModelPath(s_strArgBuffer);
	return true;
}

bool Cmd_SetEmbeddedWeaponNode_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &s_strArgBuffer) && IS_ID(weapon, TESObjectWEAP))
		weapon->embeddedNodeName.Set(s_strArgBuffer);
	return true;
}

bool Cmd_SetEmbeddedWeaponAV_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 avCode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &avCode) && IS_ID(weapon, TESObjectWEAP))
		weapon->embedWeaponAV = avCode;
	return true;
}

bool Cmd_GetCalculatedWeaponDamage_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectWEAP *weapon = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &weapon)) return true;
	float condition = 1.0F;
	InventoryRef *invRef = NULL;
	Actor *owner = g_thePlayer;
	ContChangesEntry tempEntry(NULL, 1, weapon);
	ExtraContainerChanges::ExtendDataList extendData;
	if (!weapon)
	{
		if (!thisObj) return true;
		weapon = (TESObjectWEAP*)thisObj->baseForm;
		if NOT_ID(weapon, TESObjectWEAP) return true;
		tempEntry.type = weapon;
		if (invRef = InventoryRefGetForID(thisObj->refID))
		{
			if (invRef->containerRef && invRef->containerRef->IsActor())
				owner = (Actor*)invRef->containerRef;
			tempEntry.extendData = invRef->entry->extendData;
		}
		else
		{
			extendData.Init(&thisObj->extraDataList);
			tempEntry.extendData = &extendData;
		}
		condition = GetItemHealthPerc(&tempEntry, true) / 100.0F;
	}
	else if NOT_ID(weapon, TESObjectWEAP)
		return true;
	MiddleHighProcess *midHiProc = (owner->baseProcess && (owner->baseProcess->processLevel <= 1)) ? (MiddleHighProcess*)owner->baseProcess : NULL;
	ContChangesEntry *weaponInfo = NULL;
	TESForm *ammo = NULL;
	if (midHiProc)
	{
		weaponInfo = midHiProc->weaponInfo;
		midHiProc->weaponInfo = NULL;
		if (invRef && weaponInfo && midHiProc->ammoInfo && (weaponInfo->type == weapon))
			ammo = midHiProc->ammoInfo->type;
	}
	if (!ammo) ammo = weapon->GetAmmo();
	*result = tempEntry.CalculateWeaponDamage(owner, condition, ammo);
	if (weaponInfo)
		midHiProc->weaponInfo = weaponInfo;
	return true;
}

bool Cmd_GetWeaponOnHitSetting_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = (int)weapon->onHit;
	else *result = -1;
	return true;
}

bool Cmd_SetWeaponOnHitSetting_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 setting;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &setting) && IS_ID(weapon, TESObjectWEAP) && (setting <= 3))
		weapon->onHit = setting;
	return true;
}

bool Cmd_GetWeaponSemiAutoFireDelay_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 getMax;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &getMax) && IS_ID(weapon, TESObjectWEAP))
		*result = weapon->semiAutoFireDelay[getMax != 0];
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponSemiAutoFireDelay_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 getMax;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &getMax, &value) && IS_ID(weapon, TESObjectWEAP))
		weapon->semiAutoFireDelay[getMax != 0] = value;
	return true;
}

bool Cmd_GetWeaponModel_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESObjectWEAP *weapon;
	UInt32 modFlags;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &modFlags) && IS_ID(weapon, TESObjectWEAP) && (modFlags <= 7))
	{
		switch (modFlags)
		{
		case 0:
			resStr = weapon->textureSwap.GetModelPath();
			break;
		case 3:
			resStr = weapon->modModels[3].GetModelPath();
			break;
		case 4:
			resStr = weapon->modModels[2].GetModelPath();
			break;
		default:
			resStr = weapon->modModels[modFlags - 1].GetModelPath();
		}
	}
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWeaponModel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 modFlags;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &modFlags, &s_strArgBuffer) && IS_ID(weapon, TESObjectWEAP) && (modFlags <= 7))
	{
		switch (modFlags)
		{
		case 0:
			weapon->textureSwap.SetModelPath(s_strArgBuffer);
			break;
		case 3:
			weapon->modModels[3].SetModelPath(s_strArgBuffer);
			break;
		case 4:
			weapon->modModels[2].SetModelPath(s_strArgBuffer);
			break;
		default:
			weapon->modModels[modFlags - 1].SetModelPath(s_strArgBuffer);
		}
	}
	return true;
}

bool Cmd_EquippedWeaponHasModType_Execute(COMMAND_ARGS)
{
	UInt32 modType;
	if (thisObj->IsActor() && ExtractArgsEx(EXTRACT_ARGS_EX, &modType) && ((Actor*)thisObj)->EquippedWeaponHasMod(modType))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_EquippedWeaponHasModType_Eval(COMMAND_ARGS_EVAL)
{
	*result = (thisObj->IsActor() && ((Actor*)thisObj)->EquippedWeaponHasMod((UInt32)arg1)) ? 1 : 0;
	return true;
}