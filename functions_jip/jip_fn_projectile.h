#pragma once

DEFINE_COMMAND_PLUGIN(GetProjectileTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetProjectileTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetProjectileFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetProjectileFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetProjectileExplosion, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetProjectileExplosion, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetProjectileRefSource, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefSource, , 1, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetProjectileRefWeapon, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefWeapon, , 1, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(GetProjectileRefLifeTime, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetProjectileRefDistanceTraveled, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetProjectileRefDamage, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefDamage, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetProjectileRefSpeedMult, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetProjectileRefSpeedMult, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetDetonationTimer, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetDetonationTimer, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetMineArmed, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetProjectileMuzzleFlash, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetProjectileMuzzleFlash, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetProjectileRefImpactRef, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetOnProjectileImpactEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetProjectileRefImpactMaterial, , 1, 0, NULL);

bool Cmd_GetProjectileTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSProjectile *projectile;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &projectile, &traitID) || NOT_ID(projectile, BGSProjectile)) return true;
	switch (traitID)
	{
	case 0:
		*result = projectile->type;
		break;
	case 1:
	case 2:
	case 3:
		*result = ((float*)projectile)[24 + traitID];
		break;
	case 4:
	case 5:
	case 6:
		*result = ((float*)projectile)[26 + traitID];
		break;
	case 7:
	case 8:
	case 9:
		*result = ((float*)projectile)[28 + traitID];
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		*result = ((float*)projectile)[31 + traitID];
		break;
	case 14:
		*result = projectile->soundLevel;
	}
	return true;
}

bool Cmd_SetProjectileTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSProjectile *projectile;
	UInt32 traitID;
	float fVal;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &projectile, &traitID, &fVal) || NOT_ID(projectile, BGSProjectile)) return true;
	UInt32 iVal = (int)fVal;
	switch (traitID)
	{
	case 0:
		if ((iVal == 1) || (iVal == 2) || (iVal == 4) || (iVal == 8) || (iVal == 16))
			projectile->type = iVal;
		break;
	case 1:
	case 2:
	case 3:
		((float*)projectile)[24 + traitID] = fVal;
		break;
	case 4:
	case 5:
	case 6:
		((float*)projectile)[26 + traitID] = fVal;
		break;
	case 7:
	case 8:
	case 9:
		((float*)projectile)[28 + traitID] = fVal;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
		((float*)projectile)[31 + traitID] = fVal;
		break;
	case 14:
		if (iVal && (iVal < 3)) projectile->soundLevel = iVal;
	}
	return true;
}

bool Cmd_GetProjectileFlag_Execute(COMMAND_ARGS)
{
	BGSProjectile *projectile;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &projectile, &flagID) && IS_ID(projectile, BGSProjectile) && (flagID <= 11))
		*result = (projectile->projFlags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetProjectileFlag_Execute(COMMAND_ARGS)
{
	BGSProjectile *projectile;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &projectile, &flagID, &val) && IS_ID(projectile, BGSProjectile) && (flagID <= 11))
		projectile->SetFlag(1 << flagID, val != 0);
	return true;
}

bool Cmd_GetProjectileExplosion_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSProjectile *projectile;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &projectile) && IS_ID(projectile, BGSProjectile) && projectile->explosion)
		REFR_RES = projectile->explosion->refID;
	return true;
}

bool Cmd_SetProjectileExplosion_Execute(COMMAND_ARGS)
{
	BGSProjectile *projectile;
	BGSExplosion *explosion = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &projectile, &explosion) && IS_ID(projectile, BGSProjectile))
		projectile->explosion = (explosion && IS_ID(explosion, BGSExplosion)) ? explosion : NULL;
	return true;
}

bool Cmd_GetProjectileRefSource_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(0, result);
	return true;
}

bool Cmd_GetProjectileRefWeapon_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(1, result);
	return true;
}

bool Cmd_GetProjectileRefLifeTime_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(2, result);
	return true;
}

bool Cmd_GetProjectileRefDistanceTraveled_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(3, result);
	return true;
}

bool Cmd_GetProjectileRefDamage_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(4, result);
	return true;
}

bool Cmd_GetProjectileRefSpeedMult_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(5, result);
	return true;
}

bool Cmd_SetProjectileRefSource_Execute(COMMAND_ARGS)
{
	TESObjectREFR *newSource = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &newSource) && thisObj->IsProjectile())
		((Projectile*)thisObj)->sourceRef = newSource;
	return true;
}

bool Cmd_SetProjectileRefWeapon_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && thisObj->IsProjectile() && IS_ID(weapon, TESObjectWEAP))
		((Projectile*)thisObj)->sourceWeap = weapon;
	return true;
}

bool Cmd_SetProjectileRefDamage_Execute(COMMAND_ARGS)
{
	float damage;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &damage) && thisObj->IsProjectile())
		((Projectile*)thisObj)->hitDamage = damage;
	return true;
}

bool Cmd_SetProjectileRefSpeedMult_Execute(COMMAND_ARGS)
{
	float speedMult;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &speedMult) && thisObj->IsProjectile())
		((Projectile*)thisObj)->speedMult2 = speedMult;
	return true;
}

bool Cmd_GetDetonationTimer_Execute(COMMAND_ARGS)
{
	if (IS_ID(thisObj, GrenadeProjectile) && (((BGSProjectile*)thisObj->baseForm)->projFlags & 4))
		*result = ((Projectile*)thisObj)->detonationTime;
	else *result = -1;
	return true;
}

bool Cmd_SetDetonationTimer_Execute(COMMAND_ARGS)
{
	float detnTime;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &detnTime) && IS_ID(thisObj, GrenadeProjectile) && (((BGSProjectile*)thisObj->baseForm)->projFlags & 4))
		((Projectile*)thisObj)->detonationTime = detnTime;
	return true;
}

bool Cmd_GetMineArmed_Execute(COMMAND_ARGS)
{
	*result = 0;
	if NOT_ID(thisObj, GrenadeProjectile) return true;
	GrenadeProjectile *projectile = (GrenadeProjectile*)thisObj;
	if (!(projectile->projFlags & 0x200) && (projectile->sourceRef != g_thePlayer) && ((((BGSProjectile*)thisObj->baseForm)->projFlags & 0x426) == 0x26))
		*result = 1;
	return true;
}

bool Cmd_GetProjectileMuzzleFlash_Execute(COMMAND_ARGS)
{
	const char *resStr;
	BGSProjectile *projectile;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &projectile) && IS_ID(projectile, BGSProjectile))
		resStr = projectile->muzzleFlash.GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetProjectileMuzzleFlash_Execute(COMMAND_ARGS)
{
	BGSProjectile *projectile;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &projectile, &s_strArgBuffer) && IS_ID(projectile, BGSProjectile))
		projectile->muzzleFlash.SetModelPath(s_strArgBuffer);
	return true;
}

bool Cmd_GetProjectileRefImpactRef_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(6, result);
	return true;
}

bool Cmd_SetOnProjectileImpactEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	TESForm *projectileOrList;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &projectileOrList) || NOT_ID(script, Script)) return true;
	ListNode<TESForm> *iter;
	BGSProjectile *projectile;
	EventCallbackScripts *callbacks;
	if IS_ID(projectileOrList, BGSListForm)
		iter = ((BGSListForm*)projectileOrList)->list.Head();
	else
	{
		ListNode<TESForm> tempList(projectileOrList);
		iter = &tempList;
	}
	do
	{
		projectile = (BGSProjectile*)iter->data;
		if (!projectile || NOT_ID(projectile, BGSProjectile)) continue;
		if (addEvnt)
		{
			if (s_projectileImpactEventMap.Insert(projectile, &callbacks))
				HOOK_MOD(ProjectileImpact, true);
			callbacks->Insert(script);
			projectile->SetJIPFlag(kHookFormFlag6_ProjectileImpact, true);
		}
		else
		{
			auto findProj = s_projectileImpactEventMap.Find(projectile);
			if (!findProj || !findProj().Erase(script) || !findProj().Empty()) continue;
			findProj.Remove();
			HOOK_MOD(ProjectileImpact, false);
			projectile->SetJIPFlag(kHookFormFlag6_ProjectileImpact, false);
		}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_GetProjectileRefImpactMaterial_Execute(COMMAND_ARGS)
{
	((Projectile*)thisObj)->GetData(7, result);
	return true;
}