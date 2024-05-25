#pragma once

DEFINE_COMMAND_PLUGIN(GetWeaponDetectionSoundLevel, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponDetectionSoundLevel, 0, kParams_OneObjectID_OneInt);
DEFINE_CMD_COND_PLUGIN(IsEquippedWeaponSilenced, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(IsEquippedWeaponScoped, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetWeaponSound, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponSound, 0, kParams_OneForm_OneInt_OneOptionalSound);
DEFINE_COMMAND_PLUGIN(SetWeaponItemMod, 0, kParams_OneForm_OneInt_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponItemModEffect, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWeaponItemModValue, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponModReloadAnim, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponModReloadAnim, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(GetWeaponShellCasingModel, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponShellCasingModel, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetEmbeddedWeaponNode, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetEmbeddedWeaponAV, 0, kParams_OneObjectID_OneActorValue);
DEFINE_COMMAND_PLUGIN(GetCalculatedWeaponDamage, 0, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(GetWeaponOnHitSetting, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetWeaponOnHitSetting, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(GetWeaponSemiAutoFireDelay, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponSemiAutoFireDelay, 0, kParams_OneObjectID_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetWeaponModel, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(SetWeaponModel, 0, kParams_OneForm_OneInt_OneString);
DEFINE_CMD_COND_PLUGIN(EquippedWeaponHasModType, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(WeaponHasModType, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(CalculateShotsPerSec, 1, kParams_OneForm);
DEFINE_CMD_COND_PLUGIN(IsAttackQueued, 0, nullptr);

bool Cmd_GetWeaponDetectionSoundLevel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		*result = (int)weapon->soundLevel;
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
	if IS_ACTOR(thisObj)
		*result = ((Actor*)thisObj)->EquippedWeaponSilenced();
	return true;
}

bool Cmd_IsEquippedWeaponSilenced_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		*result = ((Actor*)thisObj)->EquippedWeaponSilenced();
	return true;
}

bool Cmd_IsEquippedWeaponScoped_Eval(COMMAND_ARGS_EVAL)
{
	if IS_ACTOR(thisObj)
		*result = ((Actor*)thisObj)->EquippedWeaponHasScope();
	return true;
}

bool Cmd_IsEquippedWeaponScoped_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		*result = ((Actor*)thisObj)->EquippedWeaponHasScope();
	return true;
}

bool Cmd_GetWeaponSound_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &type) && IS_ID(weapon, TESObjectWEAP) && (type <= 13))
		if (TESSound *sound = (type < 12) ? weapon->sounds[type] : ((type == 12) ? weapon->pickupPutdownSounds.pickupSound : weapon->pickupPutdownSounds.putdownSound))
			REFR_RES = sound->refID;
	return true;
}

bool Cmd_SetWeaponSound_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 type;
	TESSound *newSound = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &type, &newSound) && IS_ID(weapon, TESObjectWEAP) && (type <= 13))
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
	TESObjectIMOD *wmod = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &idx, &wmod) && IS_ID(weapon, TESObjectWEAP) && idx && (idx <= 3))
		weapon->itemMod[idx - 1] = (wmod && IS_ID(wmod, TESObjectIMOD)) ? wmod : nullptr;
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
	const char *resStr = nullptr;
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP))
		resStr = weapon->shellCasingModel.GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWeaponShellCasingModel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &path) && IS_ID(weapon, TESObjectWEAP))
		weapon->shellCasingModel.SetModelPath(path);
	return true;
}

bool Cmd_SetEmbeddedWeaponNode_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	char nodeName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &nodeName) && IS_ID(weapon, TESObjectWEAP))
		weapon->embeddedNodeName.Set(nodeName);
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
	TESObjectWEAP *weapon = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &weapon)) return true;
	float condition = 1.0F;
	InventoryRef *invRef = nullptr;
	Actor *owner = g_thePlayer;
	ContChangesEntry tempEntry(nullptr, 1, weapon);
	ContChangesExtraList extendData;
	if (!weapon)
	{
		if (!thisObj) return true;
		weapon = (TESObjectWEAP*)thisObj->baseForm;
		if NOT_ID(weapon, TESObjectWEAP) return true;
		tempEntry.type = weapon;
		if (invRef = InventoryRefGetForID(thisObj->refID))
		{
			if (invRef->containerRef && IS_ACTOR(invRef->containerRef))
				owner = (Actor*)invRef->containerRef;
			tempEntry.extendData = invRef->entry->extendData;
		}
		else
		{
			extendData.Init(&thisObj->extraDataList);
			tempEntry.extendData = &extendData;
		}
		condition = tempEntry.GetHealthPercent() * 0.01F;
	}
	else if NOT_ID(weapon, TESObjectWEAP)
		return true;
	MiddleHighProcess *midHiProc = (owner->baseProcess && (owner->baseProcess->processLevel <= 1)) ? (MiddleHighProcess*)owner->baseProcess : nullptr;
	ContChangesEntry *weaponInfo = nullptr;
	TESForm *ammo = nullptr;
	if (midHiProc)
	{
		weaponInfo = midHiProc->weaponInfo;
		midHiProc->weaponInfo = nullptr;
		if (invRef && weaponInfo && midHiProc->ammoInfo && (weaponInfo->type == weapon))
			ammo = midHiProc->ammoInfo->type;
	}
	if (!ammo) ammo = weapon->GetAmmo();
	if (ammo && NOT_TYPE(ammo, TESAmmo))
		ammo = nullptr;
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
	const char *resStr = nullptr;
	TESObjectWEAP *weapon;
	UInt32 modFlags;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &modFlags) && IS_ID(weapon, TESObjectWEAP) && (modFlags <= 7))
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
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetWeaponModel_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	UInt32 modFlags;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &modFlags, &path) && IS_ID(weapon, TESObjectWEAP) && (modFlags <= 7))
		switch (modFlags)
		{
			case 0:
				weapon->textureSwap.SetModelPath(path);
				break;
			case 3:
				weapon->modModels[3].SetModelPath(path);
				break;
			case 4:
				weapon->modModels[2].SetModelPath(path);
				break;
			default:
				weapon->modModels[modFlags - 1].SetModelPath(path);
		}
	return true;
}

bool Cmd_EquippedWeaponHasModType_Execute(COMMAND_ARGS)
{
	UInt32 modType;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &modType) && ((Actor*)thisObj)->EquippedWeaponHasMod(modType))
		*result = 1;
	return true;
}

bool Cmd_EquippedWeaponHasModType_Eval(COMMAND_ARGS_EVAL)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->EquippedWeaponHasMod((UInt32)arg1))
		*result = 1;
	return true;
}

bool Cmd_WeaponHasModType_Execute(COMMAND_ARGS)
{
	TESForm *inForm;
	UInt32 modType;
	int slotIdx = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &inForm, &modType))
	{
		if IS_REFERENCE(inForm)
			inForm = ((TESObjectREFR*)inForm)->baseForm;
		if IS_ID(inForm, TESObjectWEAP)
		{
			TESObjectWEAP *inWeapon = (TESObjectWEAP*)inForm;
			int idx = 0;
			do
			{
				if (!inWeapon->itemMod[idx] || (inWeapon->effectMods[idx] != modType))
					continue;
				slotIdx = idx + 1;
				break;
			}
			while (++idx < 3);
		}
	}
	*result = slotIdx;
	return true;
}

bool Cmd_CalculateShotsPerSec_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		ContChangesEntry *weapInfo = nullptr;
		if IS_REFERENCE(form)
			if (InventoryRef *invRef = InventoryRefGetForID(form->refID))
			{
				weapInfo = invRef->entry;
				form = invRef->type;
			}
		if IS_ID(form, TESObjectWEAP)
		{
			TESObjectWEAP *weapon = (TESObjectWEAP*)form;
			auto animData = thisObj->GetAnimData();
			float animMult = weapon->animAttackMult;
			if (weapInfo)
				animMult += weapInfo->GetWeaponModEffectValue(8);
			if (animData && !weapon->IsAutomatic())
			{
				UInt32 attackAnim = weapon->attackAnim;
				if (attackAnim == 0xFF)
					attackAnim = 0x20;
				UInt32 animID = ThisCall<UInt16>(0x495740, animData, (((UInt32*)0x118A838)[weapon->eWeaponType] << 8) | attackAnim, 0);
				if ((animID & 0xFF) == attackAnim)
					if (TESAnimGroup *animGroup = ThisCall<TESAnimGroup*>(0x496500, animData, animID); animGroup && (animGroup->numKeys >= 2))
						*result = (animMult * weapon->animMult) / animGroup->keyTimes[animGroup->numKeys - 2];
			}
			else *result = animMult * animMult * weapon->fireRate * weapon->animMult;
		}
	}
	return true;
}

bool Cmd_IsAttackQueued_Execute(COMMAND_ARGS)
{
	if (*(UInt8*)0x11E07AC)
		*result = 1;
	return true;
}

bool Cmd_IsAttackQueued_Eval(COMMAND_ARGS_EVAL)
{
	if (*(UInt8*)0x11E07AC)
		*result = 1;
	return true;
}