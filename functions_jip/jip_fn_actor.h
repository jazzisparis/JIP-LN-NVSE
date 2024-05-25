#pragma once

DEFINE_COMMAND_PLUGIN(GetActorTemplate, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetLeveledActorBase, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetCreatureDamage, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetCreatureDamage, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_CMD_COND_PLUGIN(GetIsPoisoned, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetFollowers, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetActorLevelingData, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorLevelingData, 0, kParams_OneInt_OneFloat_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorVoiceType, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorVoiceType, 0, kParams_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCreatureReach, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetIsImmobile, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(PickFromList, 1, kParams_OneList_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(ToggleCreatureModel, 0, kParams_OneString_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(CreatureHasModel, 0, kParams_OneString_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCreatureModels, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_ALT_PLUGIN(ModBaseActorValue, ModBaseAV, 0, kParams_OneActorValue_OneFloat_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorProcessingLevel, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetActorsByProcessingLevel, 0, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(GetActorLightAmount, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetActorAlpha, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetActorDiveBreath, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetActorDiveBreath, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCombatTargets, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetCombatAllies, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetDetectedActors, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetDetectingActors, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetCombatDisabled, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetCombatDisabled, 1, kParams_OneInt_OneOptionalActor);
DEFINE_COMMAND_PLUGIN(ToggleNoHealthReset, 0, kParams_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCurrentStablePackage, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetTeammateUsingAmmo, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetTeammateUsingAmmo, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ToggleDetectionFix, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ToggleIgnoreLockedDoors, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(IsInCombatWith, 1, kParams_OneActor);
DEFINE_CMD_COND_PLUGIN(IsAttacking, 1, nullptr);
DEFINE_COMMAND_PLUGIN(StopIdle, 1, nullptr);
DEFINE_COMMAND_PLUGIN(HolsterWeapon, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetFallTimeElapsed, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetFallTimeRemaining, 1, nullptr);
DEFINE_COMMAND_PLUGIN(ResetFallTime, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(GetRadiationLevelAlt, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(IsInWater, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetDroppedRefs, 1, nullptr);
DEFINE_COMMAND_PLUGIN(MoveAwayFromPlayer, 1, nullptr);
DEFINE_COMMAND_PLUGIN(FaceObject, 1, kParams_OneObjectRef);
DEFINE_COMMAND_PLUGIN(Turn, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetAshPileSource, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetOnAnimActionEventHandler, 0, kParams_OneForm_OneInt_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnPlayGroupEventHandler, 0, kParams_OneForm_OneInt_OneForm_OneAnimGroup);
DEFINE_COMMAND_PLUGIN(SetOnHealthDamageEventHandler, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetOnCrippledLimbEventHandler, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetOnFireWeaponEventHandler, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCurrentAmmo, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetCurrentAmmoRounds, 1, nullptr);
DEFINE_COMMAND_ALT_PLUGIN(SetFullNameAlt, SetActorFullNameAlt, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetDetectionValue, 1, kParams_OneActor);
DEFINE_COMMAND_ALT_PLUGIN(GetBaseActorValueAlt, GetBaseAVAlt, 0, kParams_OneActorValue_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetSpeedMult, 1, kParams_OneOptionalInt);
DEFINE_CMD_COND_PLUGIN(GetIsRagdolled, 1, nullptr);
DEFINE_COMMAND_PLUGIN(ForceActorDetectionValue, 1, kParams_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetActorVelocity, 1, kParams_OneOptionalAxis);
DEFINE_CMD_COND_PLUGIN(IsInAir, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetObjectUnderFeet, 1, nullptr);
DEFINE_COMMAND_ALT_PLUGIN(GetActorValueModifier, GetAVMod, 1, kParams_OneActorValue_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPerkModifier, 1, kParams_OneInt_OneFloat_TwoOptionalForms);
DEFINE_COMMAND_PLUGIN(GetWheelDisabled, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetWheelDisabled, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetHitHealthDamage, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHitLimbDamage, 1, nullptr);
DEFINE_COMMAND_PLUGIN(CrippleLimb, 1, kParams_OneInt_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(PlayIdleEx, 1, kParams_OneOptionalIdleForm);
DEFINE_COMMAND_PLUGIN(GetKillXP, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetKiller, 1, nullptr);
DEFINE_COMMAND_PLUGIN(KillActorAlt, 1, kParams_OneOptionalObjectRef_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(ReloadEquippedModels, ReloadModels, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPlayedIdle, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(IsIdlePlayingEx, 1, kParams_OneIdleForm);
DEFINE_COMMAND_PLUGIN(SetWeaponOut, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(AddBaseEffectListEffect, 0, kParams_OneSpellItem_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(RemoveBaseEffectListEffect, 0, kParams_OneSpellItem_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetHitAttacker, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHitProjectile, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHitWeapon, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(GetWaterImmersionPerc, 1, nullptr);
DEFINE_COMMAND_PLUGIN(DismemberLimb, 1, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetInterruptPackage, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(IsFleeing, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetCurrentAmmoRounds, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(AttachAshPileEx, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetActorVelocity, 1, kParams_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(GetSkeletonModel, 0, kParams_OneActorBase);
DEFINE_COMMAND_PLUGIN(SetNPCSkeletonModel, 0, kParams_OneActorBase_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetCRESkeletonModel, 0, kParams_OneActorBase_OneString);
DEFINE_CMD_COND_PLUGIN(GetTargetUnreachable, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetVATSTargetable, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetCreatureWeaponList, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetDeathItem, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetDeathItem, 0, kParams_OneObjectID_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorLeveledList, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetOnHitEventHandler, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetArmourPenetrated, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetImpactMaterialType, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetImpactMaterialType, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(PushActorAwayAlt, 1, kParams_OneFloat_ThreeOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(MoveAwayFrom, 1, kParams_OneObjectRef_OneFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(TravelToRef, 1, kParams_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(DonnerReedKuruParty, TheOtherOtherWhiteMeat, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEquippedEx, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(TestEquippedSlots, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(GetFactions, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetHit3DData, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetCreatureSoundsTemplate, 0, kParams_OneActorBase);
DEFINE_COMMAND_PLUGIN(SetCreatureSoundsTemplate, 0, kParams_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetOnReloadWeaponEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(UpdatePlayer3D, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetOnRagdollEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(PushActorNoRagdoll, 1, kParams_TwoFloats_OneOptionalObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetBodyPartVATSTarget, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_CMD_COND_PLUGIN(GetInFactionList, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetActorTiltAngle, 1, kParams_OneAxis);
DEFINE_COMMAND_PLUGIN(SetActorTiltAngle, 1, kParams_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(ReloadCharController, 1, nullptr);
DEFINE_CMD_COND_PLUGIN(GetGroundMaterial, 1, nullptr);
DEFINE_COMMAND_PLUGIN(FireWeaponEx, 1, kParams_OneObjectID_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetActorGravityMult, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetActorGravityMult, 1, kParams_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(ToggleTeammateKillable, SetTeammateKillable, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(ToggleNoGunWobble, SetNoGunWobble, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetHitNode, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHitExtendedFlag, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveAllPerks, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetActorMovementFlags, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHitBaseWeaponDamage, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHitFatigueDamage, 1, nullptr);
DEFINE_COMMAND_PLUGIN(RefreshAnimData, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetActorVelocityAlt, 1, kParams_ThreeScriptVars_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetExcludedCombatActions, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetExcludedCombatActions, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetAllPerks, 1, kParams_TwoOptionalInts);

bool Cmd_GetActorTemplate_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			if (actorBase->baseData.templateActor)
				REFR_RES = actorBase->baseData.templateActor->refID;
	return true;
}

bool Cmd_GetLeveledActorBase_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		REFR_RES = ((Actor*)thisObj)->GetActorBase()->refID;
	return true;
}

bool Cmd_GetCreatureDamage_Execute(COMMAND_ARGS)
{
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if IS_ID(creature, TESCreature)
				*result = creature->attackDmg.damage;
	return true;
}

bool Cmd_SetCreatureDamage_Execute(COMMAND_ARGS)
{
	int dmg;
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &dmg, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if IS_ID(creature, TESCreature)
				creature->attackDmg.damage = GetMin(GetMax(dmg, 0), 0xFFFF);
	return true;
}

__declspec(noinline) bool __fastcall GetIsPoisoned(Actor *actor)
{
	if IS_ACTOR(actor)
		if (ActiveEffectList *effList = actor->magicTarget.GetEffectList())
		{
			auto iter = effList->Head();
			do
			{
				if (ActiveEffect *activeEff = iter->data; activeEff && (activeEff->spellType == 5))
					return true;
			}
			while (iter = iter->next);
		}
	return false;
}

bool Cmd_GetIsPoisoned_Execute(COMMAND_ARGS)
{
	if (GetIsPoisoned((Actor*)thisObj))
		*result = 1;
	return true;
}

bool Cmd_GetIsPoisoned_Eval(COMMAND_ARGS_EVAL)
{
	if (GetIsPoisoned((Actor*)thisObj))
		*result = 1;
	return true;
}

bool Cmd_GetFollowers_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		if (auto xFollower = GetExtraType(&thisObj->extraDataList, ExtraFollower); xFollower && xFollower->followers)
		{
			TempElements *tmpElements = GetTempElements();
			auto iter = xFollower->followers->Head();
			do
			{
				if (iter->data)
					tmpElements->Append(iter->data);
			}
			while (iter = iter->next);
			if (!tmpElements->Empty())
				*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
		}
	return true;
}

bool Cmd_GetActorLevelingData_Execute(COMMAND_ARGS)
{
	UInt32 valID;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valID, &actorBase) && (valID <= 2))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			if (valID == 1)
				*result = actorBase->baseData.calcMin;
			else if (valID == 2)
				*result = actorBase->baseData.calcMax;
			else if (actorBase->baseData.flags & 0x80)
				*result = actorBase->baseData.level * 0.001;
			else
				*result = actorBase->baseData.level;
	return true;
}

bool Cmd_SetActorLevelingData_Execute(COMMAND_ARGS)
{
	UInt32 valID;
	float val;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valID, &val, &actorBase) && (valID <= 2))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			switch (valID)
			{
				case 0:
				{
					if (actorBase->baseData.flags & 128)
					{
						if (val < 0.01F) val = 10;
						else val *= 1000;
					}
					else val = GetMax(val, 1.0F);
					actorBase->baseData.level = val;
					break;
				}
				case 1:
				{
					val = GetMax(val, 1.0F);
					actorBase->baseData.calcMin = val;
					if (actorBase->baseData.calcMax < val)
						actorBase->baseData.calcMax = val;
					break;
				}
				default:
				{
					val = GetMax(val, 1.0F);
					actorBase->baseData.calcMax = val;
					if (actorBase->baseData.calcMin > val)
						actorBase->baseData.calcMin = val;
				}
			}
	return true;
}

bool Cmd_GetActorVoiceType_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			if (actorBase->baseData.voiceType)
				REFR_RES = actorBase->baseData.voiceType->refID;
	return true;
}

bool Cmd_SetActorVoiceType_Execute(COMMAND_ARGS)
{
	BGSVoiceType *form;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &actorBase) && IS_ID(form, BGSVoiceType))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			actorBase->baseData.voiceType = form;
	return true;
}

bool Cmd_GetCreatureReach_Execute(COMMAND_ARGS)
{
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if IS_ID(creature, TESCreature)
				*result = creature->attackReach;
	return true;
}

bool Cmd_GetIsImmobile_Execute(COMMAND_ARGS)
{
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature))
		if (!creature)
		{
			if (thisObj && IS_ACTOR(thisObj) && ((Actor*)thisObj)->isImmobileCreature)
				*result = 1;
		}
		else if (IS_ID(creature, TESCreature) && (creature->baseData.flags & 0x800000))
			*result = 1;
	return true;
}

bool Cmd_PickFromList_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	SInt32 start = 0, len = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm, &start, &len) && (start >= 0))
		if (TESContainer *container = thisObj->baseForm->GetContainer())
			if (ContChangesEntryList *entryList = thisObj->GetContainerChangesList())
			{
				auto iter = listForm->list.Head();
				do
				{
					if (start-- > 0) continue;
					if (!len-- || !iter->data)
						break;
					if (GetFormCount(&container->formCountList, entryList, iter->data) > 0)
					{
						REFR_RES = iter->data->refID;
						break;
					}
				}
				while (iter = iter->next);
			}
	return true;
}

bool Cmd_ToggleCreatureModel_Execute(COMMAND_ARGS)
{
	char path[0x100];
	UInt32 enable;
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &path, &enable, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if (IS_ID(creature, TESCreature) && creature->modelList.ModelListAction(path, enable ? 1 : -1))
				*result = 1;
	return true;
}

bool Cmd_CreatureHasModel_Execute(COMMAND_ARGS)
{
	char path[0x100];
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &path, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if (IS_ID(creature, TESCreature) && creature->modelList.ModelListAction(path, 0))
				*result = 1;
	return true;
}

bool Cmd_GetCreatureModels_Execute(COMMAND_ARGS)
{
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if IS_ID(creature, TESCreature)
			{
				TempElements *tmpElements = GetTempElements();
				auto iter = creature->modelList.modelList.Head();
				do
				{
					if (iter->data)
						tmpElements->Append(iter->data);
				}
				while (iter = iter->next);
				if (!tmpElements->Empty())
					*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
			}	
	return true;
}

bool Cmd_ModBaseActorValue_Execute(COMMAND_ARGS)
{
	UInt32 actorVal;
	float value;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorVal, &value, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = (TESActorBase*)thisObj->baseForm)))
			actorBase->ModActorValue(actorVal, value);
	return true;
}

bool Cmd_GetActorProcessingLevel_Execute(COMMAND_ARGS)
{
	int procLvl = -1;
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->baseProcess)
		procLvl = (int)((Actor*)thisObj)->baseProcess->processLevel;
	*result = procLvl;
	return true;
}

bool Cmd_GetActorsByProcessingLevel_Execute(COMMAND_ARGS)
{
	UInt32 procLevel;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &procLevel) || (procLevel > 3)) return true;
	TempElements *tmpElements = GetTempElements();
	ProcessManager *procMngr = ProcessManager::Get();
	MobileObject **objArray = procMngr->objects.data, **arrEnd = objArray;
	objArray += procMngr->beginOffsets[procLevel];
	arrEnd += procMngr->endOffsets[procLevel];
	Actor *actor;
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && IS_ACTOR(actor))
			tmpElements->Append(actor);
	}
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetActorLightAmount_Execute(COMMAND_ARGS)
{
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor) && actor->baseProcess && !actor->baseProcess->processLevel)
		*result = ((HighProcess*)actor->baseProcess)->lightAmount;
	return true;
}

bool Cmd_GetActorLightAmount_Eval(COMMAND_ARGS_EVAL)
{
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor) && actor->baseProcess && !actor->baseProcess->processLevel)
		*result = ((HighProcess*)actor->baseProcess)->lightAmount;
	return true;
}

bool Cmd_GetActorAlpha_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->baseProcess)
		*result = ((Actor*)thisObj)->baseProcess->GetActorAlpha();
	return true;
}

bool Cmd_GetActorDiveBreath_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->baseProcess)
		*result = ((Actor*)thisObj)->baseProcess->GetDiveBreath();
	return true;
}

bool Cmd_SetActorDiveBreath_Execute(COMMAND_ARGS)
{
	float breath;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &breath) && IS_ACTOR(thisObj) && ((Actor*)thisObj)->baseProcess)
		((Actor*)thisObj)->baseProcess->SetDiveBreath(GetMax(breath, 0.0F));
	return true;
}

void __fastcall GetCombatActors(TESObjectREFR *thisObj, Script *scriptObj, bool getAllies, double *result)
{
	UInt32 count;
	Actor *actor;
	TempElements *tmpElements = GetTempElements();
	if (thisObj->IsPlayer())
	{
		CombatActors *cmbActors = g_thePlayer->combatActors;
		if (!cmbActors) return;
		if (getAllies)
		{
			CombatAlly *allies = cmbActors->allies.data;
			for (count = cmbActors->allies.size; count; count--, allies++)
			{
				actor = allies->ally;
				if (actor && (actor != thisObj))
					tmpElements->Append(actor);
			}
		}
		else
		{
			CombatTarget *targets = cmbActors->targets.data;
			for (count = cmbActors->targets.size; count; count--, targets++)
			{
				actor = targets->target;
				if (actor) tmpElements->Append(actor);
			}
		}
	}
	else
	{
		if (NOT_ACTOR(thisObj)) return;
		actor = (Actor*)thisObj;
		Actor **actorsArr = nullptr;
		if (getAllies)
		{
			if (!actor->combatAllies) return;
			actorsArr = actor->combatAllies->data;
			count = actor->combatAllies->size;
		}
		else if (actor->combatTargets)
		{
			actorsArr = actor->combatTargets->data;
			count = actor->combatTargets->size;
		}
		if (!actorsArr) return;
		for (; count; count--, actorsArr++)
		{
			actor = *actorsArr;
			if (actor && (actor != thisObj))
				tmpElements->Append(actor);
		}
	}
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
}

bool Cmd_GetCombatTargets_Execute(COMMAND_ARGS)
{
	GetCombatActors(thisObj, scriptObj, false, result);
	return true;
}

bool Cmd_GetCombatAllies_Execute(COMMAND_ARGS)
{
	GetCombatActors(thisObj, scriptObj, true, result);
	return true;
}

void __fastcall GetDetectionData(TESObjectREFR *thisObj, Script *scriptObj, bool detecting, double *result)
{
	if (NOT_ACTOR(thisObj)) return;
	TempElements *tmpElements = GetTempElements();
	Actor *actor = (Actor*)thisObj;
	if (actor->IsPlayer())
	{
		ProcessManager *procMngr = ProcessManager::Get();
		MobileObject **objArray = procMngr->objects.data, **arrEnd = objArray;
		objArray += procMngr->beginOffsets[0];
		arrEnd += procMngr->endOffsets[0];
		Actor *target;
		for (; objArray != arrEnd; objArray++)
		{
			target = (Actor*)*objArray;
			if (!target || NOT_ACTOR(target) || target->GetDead())
				continue;
			if (detecting)
			{
				if (target->GetDetectionValue(actor) <= 0)
					continue;
			}
			else if (!actor->GetLOS(target))
				continue;
			tmpElements->Append(target);
		}
	}
	else
	{
		if (!actor->baseProcess || actor->baseProcess->processLevel) return;
		HighProcess *hiProcess = (HighProcess*)actor->baseProcess;
		auto iter = detecting ? hiProcess->detectingActors->Head() : hiProcess->detectedActors->Head();
		DetectionData *data;
		do
		{
			if (!(data = iter->data)) continue;
			if ((data->detectionValue > 0) && !data->actor->GetDead())
				tmpElements->Append(data->actor);
		}
		while (iter = iter->next);
	}
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
}

bool Cmd_GetDetectedActors_Execute(COMMAND_ARGS)
{
	GetDetectionData(thisObj, scriptObj, false, result);
	return true;
}

bool Cmd_GetDetectingActors_Execute(COMMAND_ARGS)
{
	GetDetectionData(thisObj, scriptObj, true, result);
	return true;
}

bool Cmd_GetCombatDisabled_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && (((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_CombatAIModified))
		*result = 1;
	return true;
}

bool Cmd_SetCombatDisabled_Execute(COMMAND_ARGS)
{
	SInt32 disable;
	Actor *target = nullptr;
	if (NOT_ACTOR(thisObj) || !ExtractArgsEx(EXTRACT_ARGS_EX, &disable, &target)) return true;
	Actor *actor = (Actor*)thisObj;
	if (disable <= 0)
	{
		if (!(actor->jipActorFlags1 & kHookActorFlag1_CombatAIModified)) return true;
		if (actor->jipActorFlags1 & kHookActorFlag1_CombatDisabled)
			HOOK_DEC(StartCombat);
		else
		{
			s_forceCombatTargetMap->Erase(actor);
			HOOK_DEC(SetCombatTarget);
		}
		actor->jipActorFlags1 &= ~kHookActorFlag1_CombatAIModified;
		return true;
	}
	UInt8 flag = target ? kHookActorFlag1_ForceCombatTarget : kHookActorFlag1_CombatDisabled;
	if (!(actor->jipActorFlags1 & kHookActorFlag1_CombatAIModified))
	{
		actor->jipActorFlags1 |= flag;
		s_hookInfos[target ? kHook_SetCombatTarget : kHook_StartCombat].ModUsers(true);
	}
	else if (!(actor->jipActorFlags1 & flag))
	{
		if (!target) s_forceCombatTargetMap->Erase(actor);
		HOOK_MOD(StartCombat, !target);
		HOOK_MOD(SetCombatTarget, target != nullptr);
		actor->jipActorFlags1 ^= kHookActorFlag1_CombatAIModified;
	}
	if (target) s_forceCombatTargetMap()[actor] = target;
	else actor->StopCombat();
	return true;
}

bool Cmd_ToggleNoHealthReset_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &enable, &s_healthRestoreRatio))
		HOOK_SET(NoHealthReset, enable != 0);
	return true;
}

bool Cmd_GetCurrentStablePackage_Execute(COMMAND_ARGS)
{
	TESPackage *package = IS_ACTOR(thisObj) ? ((Actor*)thisObj)->GetStablePackage() : nullptr;
	if (package) REFR_RES = package->refID;
	DoConsolePrint(package);
	return true;
}

bool Cmd_GetTeammateUsingAmmo_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_InfiniteAmmo))
		*result = 1;
	return true;
}

bool Cmd_SetTeammateUsingAmmo_Execute(COMMAND_ARGS)
{
	UInt32 useAmmo;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &useAmmo) && (!useAmmo == !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_InfiniteAmmo)))
	{
		((Actor*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_InfiniteAmmo;
		HOOK_MOD(RemoveAmmo, !useAmmo);
	}
	return true;
}

bool Cmd_ToggleDetectionFix_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &enable) && (!enable != !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_DetectionFix)))
		((Actor*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_DetectionFix;
	return true;
}

bool Cmd_ToggleIgnoreLockedDoors_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &enable))
		HOOK_SET(ActivateDoor, enable != 0);
	return true;
}

bool Cmd_IsInCombatWith_Execute(COMMAND_ARGS)
{
	Actor *target;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &target))
		if (!thisObj->IsPlayer())
		{
			if (((Actor*)thisObj)->IsInCombatWith(target))
				*result = 1;
		}
		else if (CombatActors *cmbActors = g_thePlayer->combatActors)
			for (auto iter = cmbActors->targets.Begin(); iter; ++iter)
				if (iter.Get().target == target)
				{
					*result = 1;
					break;
				}
	return true;
}

bool __fastcall IsAttacking(Actor *actor)
{
	if (IS_ACTOR(actor) && actor->baseProcess)
		if (SInt16 currentAction = actor->baseProcess->GetCurrentAction(); (currentAction > 1) && (currentAction < 6))
			return true;
	return false;
}

bool Cmd_IsAttacking_Execute(COMMAND_ARGS)
{
	if (IsAttacking((Actor*)thisObj))
		*result = 1;
	return true;
}

bool Cmd_IsAttacking_Eval(COMMAND_ARGS_EVAL)
{
	if (IsAttacking((Actor*)thisObj))
		*result = 1;
	return true;
}

bool Cmd_StopIdle_Execute(COMMAND_ARGS)
{
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor) && actor->baseProcess)
		actor->baseProcess->StopIdle(actor);
	return true;
}

bool Cmd_HolsterWeapon_Execute(COMMAND_ARGS)
{
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor))
		if (BaseProcess *process = actor->baseProcess; process && process->IsWeaponOut())
			process->SetWeaponOut(actor, false);
	return true;
}

bool Cmd_GetFallTimeElapsed_Execute(COMMAND_ARGS)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
		*result = charCtrl->fallTimeElapsed;
	return true;
}

bool Cmd_GetFallTimeRemaining_Execute(COMMAND_ARGS)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
		*result = charCtrl->calculatePitchTimer;
	return true;
}

bool Cmd_ResetFallTime_Execute(COMMAND_ARGS)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
	{
		charCtrl->startingHeight = thisObj->position.z;
		charCtrl->fallTimeElapsed = 0;
	}
	return true;
}

__declspec(noinline) float __fastcall GetRadiationLevelAlt(Actor *actor)
{
	if IS_ACTOR(actor)
		if (HighProcess *hiProc = (HighProcess*)actor->baseProcess; hiProc && !hiProc->processLevel)
			if (actor->IsPlayer())
				return hiProc->waterRadsSec + hiProc->rads238 + hiProc->GetRadsSec();
			else
				return actor->GetRadiationLevel();
	return 0;
}

bool Cmd_GetRadiationLevelAlt_Execute(COMMAND_ARGS)
{
	*result = GetRadiationLevelAlt((Actor*)thisObj);
	return true;
}

bool Cmd_GetRadiationLevelAlt_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetRadiationLevelAlt((Actor*)thisObj);
	return true;
}

bool Cmd_IsInWater_Eval(COMMAND_ARGS_EVAL)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->inWater)
		*result = 1;
	return true;
}

bool Cmd_IsInWater_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->inWater)
		*result = 1;
	return true;
}

bool Cmd_GetDroppedRefs_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		if (auto xDropped = GetExtraType(&thisObj->extraDataList, ExtraDroppedItemList))
		{
			TempElements *tmpElements = GetTempElements();
			auto iter = xDropped->itemRefs.Head();
			do
			{
				if (iter->data)
					tmpElements->Append(iter->data);
			}
			while (iter = iter->next);
			if (!tmpElements->Empty())
				*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
		}
	return true;
}

bool Cmd_MoveAwayFromPlayer_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		((Actor*)thisObj)->AddBackUpPackage(g_thePlayer, thisObj->parentCell, kPackageFlag_ContinueDuringCombat);
	return true;
}

bool Cmd_FaceObject_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &target))
		((Actor*)thisObj)->TurnToFaceObject(target);
	return true;
}

bool Cmd_Turn_Execute(COMMAND_ARGS)
{
	float turnAngle;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &turnAngle))
		((Actor*)thisObj)->TurnAngle(turnAngle);
	return true;
}

bool Cmd_GetAshPileSource_Execute(COMMAND_ARGS)
{
	if IS_ID(thisObj->baseForm, TESObjectACTI)
		if (auto xAshPileRef = GetExtraType(&thisObj->extraDataList, ExtraAshPileRef); xAshPileRef && xAshPileRef->sourceRef)
			REFR_RES = xAshPileRef->sourceRef->refID;
	return true;
}

bool SetOnAnimationEventHandler_Execute(COMMAND_ARGS)
{
	CAPTURE_CL(flag)
	Script *script;
	UInt32 addEvnt, animID;
	TESForm *actorOrList;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &actorOrList, &animID) || NOT_ID(script, Script))
		return true;
	bool animAction = flag == kHookActorFlag3_OnAnimAction;
	HookInfo &hookInfo = animAction ? s_hookInfos[kHook_SetAnimAction] : s_hookInfos[kHook_SetAnimGroup];
	tList<TESForm> tempList(actorOrList);
	if IS_ID(actorOrList, BGSListForm)
		tempList = ((BGSListForm*)actorOrList)->list;
	AnimEventCallbacks *scriptsMap;
	if (tempList.Empty())
	{
		scriptsMap = animAction ? *s_animActionEventMap : *s_playGroupEventMap;
		EventCallbackScripts *callbacks;
		if (addEvnt)
		{
			if (scriptsMap->Insert(animID, &callbacks))
				hookInfo.ModUsers(true);
			callbacks->Insert(script);
		}
		else if (auto findAnim = scriptsMap->Find(animID); findAnim && findAnim().Erase(script) && findAnim().Empty())
		{
			findAnim.Remove();
			hookInfo.ModUsers(false);
		}
		return true;
	}
	auto iter = tempList.Head();
	ActorAnimEventCallbacks *eventMap = animAction ? *s_animActionEventMapFl : *s_playGroupEventMapFl;
	do
	{
		if (Actor *actor = (Actor*)iter->data)
			if (addEvnt)
			{
				if (NOT_ACTOR(actor)) continue;
				if (eventMap->Insert(actor, &scriptsMap))
				{
					actor->jipActorFlags3 |= flag;
					hookInfo.ModUsers(true);
				}
				(*scriptsMap)[animID].Insert(script);
			}
			else if (auto findActor = eventMap->Find(actor))
				if (auto findAnim = findActor().Find(animID); findAnim && findAnim().Erase(script) && findAnim().Empty())
				{
					findAnim.Remove();
					if (findActor().Empty())
					{
						findActor.Remove();
						actor->jipActorFlags3 &= ~flag;
						hookInfo.ModUsers(false);
					}
				}
	}
	while (iter = iter->next);
	return true;
}

__declspec(naked) bool Cmd_SetOnAnimActionEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		cl, kHookActorFlag3_OnAnimAction
		jmp		SetOnAnimationEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnPlayGroupEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		cl, kHookActorFlag3_OnPlayGroup
		jmp		SetOnAnimationEventHandler_Execute
	}
}

struct ActorEventInfo
{
	ActorEventCallbacks		*filtered;
	EventCallbackScripts	*unfiltered;
	HookInfo				*hookInfo;
	UInt8					flag;
}
s_actorEventInfos[] =
{
	{*s_healthDamageEventMap, nullptr, &s_hookInfos[kHook_DamageActorValue], kHookActorFlag3_OnHealthDamage},
	{*s_crippledLimbEventMap, nullptr, &s_hookInfos[kHook_DamageActorValue], kHookActorFlag3_OnCrippledLimb},
	{*s_fireWeaponEventMap, *s_fireWeaponEventScripts, &s_hookInfos[kHook_RemoveAmmo], kHookActorFlag3_OnFireWeapon},
	{*s_onHitEventMap, *s_onHitEventScripts, &s_hookInfos[kHook_OnHitEvent], kHookActorFlag3_OnHit}
};

bool SetActorEventHandler_Execute(COMMAND_ARGS)
{
	CAPTURE_ECX(eventType)
	Script *script;
	UInt32 addEvnt;
	TESForm *actorOrList = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &actorOrList) || NOT_ID(script, Script))
		return true;
	ActorEventInfo &eventInfo = s_actorEventInfos[eventType];
	EventCallbackScripts *callbacks = eventInfo.unfiltered;
	HookInfo *hookInfo = eventInfo.hookInfo;
	if (!actorOrList)
	{
		if (addEvnt)
		{
			if (callbacks->Insert(script))
				hookInfo->ModUsers(true);
		}
		else if (callbacks->Erase(script))
			hookInfo->ModUsers(false);
		return true;
	}
	ActorEventCallbacks *eventMap = eventInfo.filtered;
	UInt8 flag = eventInfo.flag;
	tList<TESForm> tempList(actorOrList);
	if IS_ID(actorOrList, BGSListForm)
		tempList = ((BGSListForm*)actorOrList)->list;
	auto iter = tempList.Head();
	do
	{
		if (Actor *actor = (Actor*)iter->data)
			if (addEvnt)
			{
				if (NOT_ACTOR(actor)) continue;
				if (eventMap->Insert(actor, &callbacks))
					hookInfo->ModUsers(true);
				callbacks->Insert(script);
				actor->jipActorFlags3 |= flag;
			}
			else if (auto findActor = eventMap->Find(actor); findActor && findActor().Erase(script))
			{
				if (IS_ACTOR(actor))
					actor->jipActorFlags3 &= ~flag;
				if (findActor().Empty())
				{
					findActor.Remove();
					hookInfo->ModUsers(false);
				}
			}
	}
	while (iter = iter->next);
	return true;
}

__declspec(naked) bool Cmd_SetOnHealthDamageEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		xor		ecx, ecx
		jmp		SetActorEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnCrippledLimbEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		ecx, 1
		jmp		SetActorEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnFireWeaponEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		ecx, 2
		jmp		SetActorEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnHitEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		ecx, 3
		jmp		SetActorEventHandler_Execute
	}
}

bool Cmd_GetCurrentAmmo_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		if (ContChangesEntry *ammoInfo = ((Actor*)thisObj)->GetAmmoInfo(); ammoInfo && ammoInfo->type)
			REFR_RES = ammoInfo->type->refID;
	return true;
}

bool Cmd_GetCurrentAmmoRounds_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		if (ContChangesEntry *ammoInfo = ((Actor*)thisObj)->GetAmmoInfo())
			*result = ammoInfo->countDelta;
	return true;
}

bool Cmd_SetFullNameAlt_Execute(COMMAND_ARGS)
{
	char name[0x100];
	TESForm *form = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &name, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	TESFullName *fullName;
	UInt32 modFlag = 4;
	if (IS_ID(form, TESNPC) || IS_ID(form, TESCreature))
	{
		fullName = &((TESActorBase*)form)->fullName;
		modFlag = 0x20;
	}
	else if IS_ID(form, BGSTalkingActivator)
		fullName = &((BGSTalkingActivator*)form)->fullName;
	else if IS_ID(form, TESObjectCELL)
		fullName = &((TESObjectCELL*)form)->fullName;
	else return true;
	fullName->name.Set(name);
	form->MarkModified(modFlag);
	return true;
}

bool Cmd_GetDetectionValue_Execute(COMMAND_ARGS)
{
	Actor *target;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &target))
		*result = ((Actor*)thisObj)->GetDetectionValue(target);
	return true;
}

bool Cmd_GetBaseActorValueAlt_Execute(COMMAND_ARGS)
{
	UInt32 actorVal;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorVal, &actorBase))
		if (actorBase)
			*result = actorBase->avOwner.GetActorValue(actorVal);
		else if (thisObj && IS_ACTOR(thisObj))
			*result = (int)((Actor*)thisObj)->avOwner.GetBaseActorValue(actorVal);
	return true;
}

bool Cmd_SetSpeedMult_Execute(COMMAND_ARGS)
{
	UInt32 speedMult = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &speedMult))
	{
		if (speedMult)
			((Actor*)thisObj)->SetActorValueInt(0x15, speedMult);
		if (BaseProcess *baseProc = ((Actor*)thisObj)->baseProcess; baseProc && baseProc->cachedValues)
			baseProc->cachedValues->flags &= ~0x3000;
	}
	return true;
}

bool Cmd_GetIsRagdolled_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && (((Actor*)thisObj)->GetKnockedState() == 1))
		*result = 1;
	return true;
}

bool Cmd_GetIsRagdolled_Eval(COMMAND_ARGS_EVAL)
{
	if (IS_ACTOR(thisObj) && (((Actor*)thisObj)->GetKnockedState() == 1))
		*result = 1;
	return true;
}

bool Cmd_ForceActorDetectionValue_Execute(COMMAND_ARGS)
{
	SInt32 detectionValue = 0, oprType = 0;
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor) && ExtractArgsEx(EXTRACT_ARGS_EX, &detectionValue, &oprType))
		if (detectionValue)
		{
			UInt32 *valPtr;
			if (s_forceDetectionValueMap->InsertKey(actor, &valPtr))
			{
				actor->jipActorFlags2 |= kHookActorFlag2_ForceDetectionVal;
				HOOK_INC(GetDetectionValue);
			}
			Coordinate packedVal(oprType != 0, detectionValue);
			*valPtr = packedVal.xy;
		}
		else if (s_forceDetectionValueMap->Erase(actor))
		{
			actor->jipActorFlags2 &= ~kHookActorFlag2_ForceDetectionVal;
			HOOK_DEC(GetDetectionValue);
		}
	return true;
}

bool Cmd_GetActorVelocity_Execute(COMMAND_ARGS)
{
	char axis = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
		if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
			if (axis) *result = charCtrl->velocity[axis - 'X'];
			else *result = Length_V4(charCtrl->velocity.PS());
	return true;
}

bool Cmd_IsInAir_Execute(COMMAND_ARGS)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController(); charCtrl && (charCtrl->chrContext.hkState & 2))
		*result = 1;
	return true;
}

bool Cmd_IsInAir_Eval(COMMAND_ARGS_EVAL)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController(); charCtrl && (charCtrl->chrContext.hkState & 2))
		*result = 1;
	return true;
}

bool Cmd_GetObjectUnderFeet_Execute(COMMAND_ARGS)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController(); charCtrl && charCtrl->bodyUnderFeet)
		if (TESObjectREFR *refr = charCtrl->bodyUnderFeet->GetParentRef())
			REFR_RES = refr->refID;
	return true;
}

bool Cmd_GetActorValueModifier_Execute(COMMAND_ARGS)
{
	UInt32 actorVal, duration = 3;
	if (NOT_ACTOR(thisObj) || !ExtractArgsEx(EXTRACT_ARGS_EX, &actorVal, &duration)) return true;
	ActiveEffectList *effList = ((Actor*)thisObj)->magicTarget.GetEffectList();
	if (!effList) return true;
	float modifier = 0;
	ActiveEffect *activeEff;
	EffectSetting *effSetting;
	auto iter = effList->Head();
	do
	{
		activeEff = iter->data;
		if (!activeEff || !activeEff->bActive || activeEff->bTerminated || !activeEff->effectItem) continue;
		effSetting = activeEff->effectItem->setting;
		if (!effSetting || effSetting->archtype || (effSetting->actorVal != actorVal) || 
			!(effSetting->effectFlags & 2) || !((activeEff->duration ? 2 : 1) & duration)) continue;
		modifier += activeEff->magnitude;
	}
	while (iter = iter->next);
	*result = modifier;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetPerkModifier_Execute(COMMAND_ARGS)
{
	PerkEntryPointID entryPointID;
	float baseValue;
	TESForm *filterForm1 = nullptr, *filterForm2 = nullptr;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &entryPointID, &baseValue, &filterForm1, &filterForm2) && (entryPointID < kPerkEntry_Max))
	{
		switch (EntryPointConditionInfo::Array()[entryPointID].numTabs)
		{
			case 1:
				ApplyPerkModifiers(entryPointID, thisObj, &baseValue);
				break;
			case 2:
				ApplyPerkModifiers(entryPointID, thisObj, filterForm1, &baseValue);
				break;
			case 3:
				ApplyPerkModifiers(entryPointID, thisObj, filterForm1, filterForm2, &baseValue);
				break;
		}
		*result = baseValue;
		DoConsolePrint(result);
	}
	return true;
}

bool Cmd_GetWheelDisabled_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && (((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_DisableWheel))
		*result = 1;
	return true;
}

bool Cmd_SetWheelDisabled_Execute(COMMAND_ARGS)
{
	UInt32 disable;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &disable) && (!disable != !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_DisableWheel)))
		((Actor*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_DisableWheel;
	return true;
}

bool Cmd_GetHitHealthDamage_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(0, result);
	return true;
}

bool Cmd_GetHitLimbDamage_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(1, result);
	return true;
}

bool Cmd_CrippleLimb_Execute(COMMAND_ARGS)
{
	UInt32 limbID;
	Actor *attacker = nullptr;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &limbID, &attacker) && (limbID <= 6))
		((Actor*)thisObj)->DamageActorValue(kAVCode_PerceptionCondition + limbID, -100, attacker);
	return true;
}

bool Cmd_PlayIdleEx_Execute(COMMAND_ARGS)
{
	TESIdleForm *idleAnim = nullptr;
	Actor *actor = (Actor*)thisObj;
	if (IS_ACTOR(actor) && actor->baseProcess && !actor->baseProcess->processLevel && ExtractArgsEx(EXTRACT_ARGS_EX, &idleAnim))
		if (AnimData *animData = thisObj->GetAnimData())
		{
			if (!idleAnim)
				idleAnim = ThisCall<TESIdleForm*>(0x600950, GameGlobals::IdleAnimsDirectoryMap(), actor, ((HighProcess*)actor->baseProcess)->interactedRef);
			else if (idleAnim->children)
				idleAnim = idleAnim->FindIdle(actor);
			if (idleAnim && (animData->GetPlayedIdle() != idleAnim))
				animData->PlayIdle(idleAnim);
		}
	return true;
}

bool Cmd_GetKillXP_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		*result = ((Actor*)thisObj)->GetKillXP();
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetKiller_Execute(COMMAND_ARGS)
{
	Actor *actor = (Actor*)thisObj;
	TESObjectREFR *killer = nullptr;
	if (IS_ACTOR(actor) && actor->GetDead() && (killer = actor->killer))
		REFR_RES = killer->refID;
	DoConsolePrint(killer);
	return true;
}

__declspec(naked) bool Cmd_KillActorAlt_Execute(COMMAND_ARGS)
{
	JMP_EAX(0x5BE2A0)
}

__declspec(naked) void __fastcall ReloadBipedAnim(BipedAnim *bipAnim, NiNode *rootNode, UInt32 reloadMask)
{
	__asm
	{
		push	esi
		push	edi
		push	edx
		push	ecx
		lea		esi, [ecx+0x24]
		mov		edi, [esp+0x14]
		ALIGN 16
	iterHead:
		test	edi, edi
		jz		doneClear
		add		esi, 0x10
		shr		edi, 1
		jnc		iterHead
		mov		eax, [esi]
		test	eax, eax
		jz		iterHead
		and		dword ptr [esi], 0
		mov		ecx, [eax+0x18]
		test	ecx, ecx
		jz		iterHead
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xE8]
		jmp		iterHead
		ALIGN 16
	doneClear:
		pop		esi
		push	0
		push	esi
		mov		eax, [esi+0x2B0]
		push	eax
		mov		ecx, [eax+0x20]
		CALL_EAX(0x606540)
		push	0
		mov		ecx, esi
		CALL_EAX(0x4AC1E0)
		pop		ecx
		push	0
		push	offset kNiUpdateData
		mov		eax, [ecx]
		call	dword ptr [eax+0xA4]
		pop		edi
		pop		esi
		retn	4
	}
}

bool Cmd_ReloadEquippedModels_Execute(COMMAND_ARGS)
{
	if (g_OSGlobals->tfcState && g_OSGlobals->freezeTime)
		return true;
	SInt32 targetSlot = -1;
	Character *character = (Character*)thisObj;
	if (!character->IsCharacter() || !character->GetRefNiNode() || !character->bipedAnims || !character->baseProcess || character->baseProcess->processLevel ||
		(NUM_ARGS_EX && (!ExtractArgsEx(EXTRACT_ARGS_EX, &targetSlot) || (targetSlot > 19) || (targetSlot == 6))))
		return true;
	TESObjectWEAP *weapon = ((targetSlot < 0) || (targetSlot == 5)) ? character->bipedAnims->slotData[5].weapon : nullptr;
	bool doReEquip = weapon && (weapon->weaponFlags1 & 0x10);
	if (doReEquip)
		if (NiNode *backPack = character->bipedAnims->bip01->GetNode("Backpack"))
			backPack->m_parent->RemoveObject(backPack);
	UInt32 reloadMask = 0xFFFBF;
	if (targetSlot >= 0)
		reloadMask &= (1 << targetSlot);
	ReloadBipedAnim(character->bipedAnims, character->GetRefNiNode(), reloadMask);
	if (character->IsPlayer())
	{
		PlayerCharacter *thePC = (PlayerCharacter*)character;
		ReloadBipedAnim(thePC->bipedAnims1stPerson, thePC->node1stPerson, reloadMask);
		if (weapon)
		{
			CdeclCall(0x77F270);
			if (character->EquippedWeaponHasScope())
				CdeclCall(0x77F2F0, &weapon->targetNIF);
		}
	}
	if (doReEquip)
	{
		HighProcess *hiProcess = (HighProcess*)character->baseProcess;
		if (ContChangesEntry *weapInfo = hiProcess->weaponInfo; weapInfo && (weapInfo->type == weapon))
			hiProcess->QueueEquipItem(character, 1, weapon, 1, weapInfo->extendData ? weapInfo->extendData->GetFirstItem() : nullptr, 1, 0, 1, 0, 0);
	}
	character->RefreshAnimData();
	return true;
}

bool Cmd_GetPlayedIdle_Execute(COMMAND_ARGS)
{
	if (AnimData *animData = thisObj->GetAnimData())
		if (TESIdleForm *idleAnim = animData->GetPlayedIdle())
			REFR_RES = idleAnim->refID;
	return true;
}

__declspec(noinline) bool __fastcall IsIdlePlaying(TESObjectREFR *thisObj, TESIdleForm *idleAnim)
{
	AnimData *animData = thisObj->GetAnimData();
	return animData && (animData->GetPlayedIdle() == idleAnim);
}

bool Cmd_IsIdlePlayingEx_Execute(COMMAND_ARGS)
{
	TESIdleForm *idleAnim;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idleAnim) && IsIdlePlaying(thisObj, idleAnim))
		*result = 1;
	return true;
}

bool Cmd_IsIdlePlayingEx_Eval(COMMAND_ARGS_EVAL)
{
	if (IsIdlePlaying(thisObj, (TESIdleForm*)arg1))
		*result = 1;
	return true;
}

bool Cmd_SetWeaponOut_Execute(COMMAND_ARGS)
{
	UInt32 setOut;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &setOut))
		ThisCall(0x8A6840, thisObj, setOut != 0);
	return true;
}

bool Cmd_AddBaseEffectListEffect_Execute(COMMAND_ARGS)
{
	SpellItem *spell;
	TESActorBase *actorBase = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &spell, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	if (!actorBase->spellList.spellList.IsInList(spell))
		actorBase->spellList.spellList.Prepend(spell);
	return true;
}

bool Cmd_RemoveBaseEffectListEffect_Execute(COMMAND_ARGS)
{
	SpellItem *spell;
	TESActorBase *actorBase = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &spell, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	actorBase->spellList.spellList.Remove(spell);
	return true;
}

bool Cmd_GetHitAttacker_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(2, result);
	return true;
}

bool Cmd_GetHitProjectile_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(3, result);
	return true;
}

bool Cmd_GetHitWeapon_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(4, result);
	return true;
}

bool Cmd_GetWaterImmersionPerc_Execute(COMMAND_ARGS)
{
	*result = thisObj->GetWaterImmersionPerc();
	return true;
}

bool Cmd_GetWaterImmersionPerc_Eval(COMMAND_ARGS_EVAL)
{
	*result = thisObj->GetWaterImmersionPerc();
	return true;
}

bool Cmd_DismemberLimb_Execute(COMMAND_ARGS)
{
	UInt32 bodyPartID, explode = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &bodyPartID, &explode))
		((Actor*)thisObj)->DismemberLimb(bodyPartID, explode != 0);
	return true;
}

bool Cmd_GetInterruptPackage_Execute(COMMAND_ARGS)
{
	*result = -1;
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->baseProcess)
		if (TESPackage *package = ((Actor*)thisObj)->baseProcess->GetInterruptPackage())
			*result = package->type;
	return true;
}

bool Cmd_IsFleeing_Execute(COMMAND_ARGS)
{
	if (((Actor*)thisObj)->IsFleeing())
		*result = 1;
	return true;
}

bool Cmd_IsFleeing_Eval(COMMAND_ARGS_EVAL)
{
	if (((Actor*)thisObj)->IsFleeing())
		*result = 1;
	return true;
}

bool Cmd_SetCurrentAmmoRounds_Execute(COMMAND_ARGS)
{
	SInt32 ammoRounds;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &ammoRounds) && (ammoRounds >= 0))
		if (ContChangesEntry *ammoInfo = ((Actor*)thisObj)->GetAmmoInfo())
			ammoInfo->countDelta = ammoRounds;
	return true;
}

__declspec(naked) bool __cdecl Cmd_AttachAshPileEx_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		ecx, [esp+0xC]
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		done
		push	eax
		mov		eax, esp
		push	eax
		push	dword ptr [eax+0x1C]
		push	dword ptr [eax+0x18]
		push	dword ptr [eax+0x24]
		push	dword ptr [eax+0xC]
		push	dword ptr [eax+8]
		call	ExtractArgsEx
		add		esp, 0x18
		pop		ecx
		test	al, al
		jz		done
		test	byte ptr [ecx+6], kHookFormFlag6_IsAshPile
		jz		done
		mov		s_altAshPile, ecx
		JMP_EAX(0x5DB870)
	done:
		mov		al, 1
		retn
	}
}

bool Cmd_SetActorVelocity_Execute(COMMAND_ARGS)
{
	char axis;
	float velocity;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &velocity))
		if (bhkCharacterController *charCtrl = thisObj->GetCharacterController(); charCtrl && charCtrl->refObject)
			((hkpCharacterProxy*)charCtrl->refObject)->velocity[axis - 'X'] = velocity;
	return true;
}

__declspec(naked) TESModel* __fastcall GetActorModel(TESActorBase *actorBase)
{
	__asm
	{
	iterHead:
		mov		eax, [ecx+0x54]
		test	eax, eax
		jz		done
		test	byte ptr [ecx+0x4A], 0x40
		jz		done
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESLevCreature
		jz		done
		cmp		dl, kFormType_TESLevCharacter
		jz		done
		mov		ecx, eax
		jmp		iterHead
	done:
		lea		eax, [ecx+0xDC]
		retn
	}
}

bool Cmd_GetSkeletonModel_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESActorBase *actorBase;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
		resStr = GetActorModel(actorBase)->GetModelPath();
	else resStr = nullptr;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetNPCSkeletonModel_Execute(COMMAND_ARGS)
{
	char path[0x100];
	TESNPC *npc;
	path[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &npc, &path) && IS_ID(npc, TESNPC))
		GetActorModel(npc)->nifPath.Set(path);
	return true;
}

bool Cmd_SetCRESkeletonModel_Execute(COMMAND_ARGS)
{
	char path[0x100];
	TESCreature *creature;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature, &path) && IS_ID(creature, TESCreature))
		GetActorModel(creature)->nifPath.Set(path);
	return true;
}

bool Cmd_GetTargetUnreachable_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->HasNoPath())
		*result = 1;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetTargetUnreachable_Eval(COMMAND_ARGS_EVAL)
{
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->HasNoPath())
		*result = 1;
	return true;
}

bool Cmd_SetVATSTargetable_Execute(COMMAND_ARGS)
{
	UInt32 targetable;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &targetable) && (!targetable == !(((Actor*)thisObj)->jipActorFlags2 & kHookActorFlag2_NonTargetable)))
	{
		((Actor*)thisObj)->jipActorFlags2 ^= kHookActorFlag2_NonTargetable;
		HOOK_MOD(AddVATSTarget, !targetable);
	}
	return true;
}

bool Cmd_GetCreatureWeaponList_Execute(COMMAND_ARGS)
{
	TESCreature *creature = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if (IS_ID(creature, TESCreature) && creature->weaponList)
		REFR_RES = creature->weaponList->refID;
	return true;
}

bool Cmd_GetDeathItem_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (actorBase->baseData.deathItem)
		REFR_RES = actorBase->baseData.deathItem->refID;
	return true;
}

bool Cmd_SetDeathItem_Execute(COMMAND_ARGS)
{
	TESForm *item;
	TESActorBase *actorBase = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &item, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->baseData.deathItem = item;
	return true;
}

bool Cmd_GetActorLeveledList_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		if (auto xLvlCre = GetExtraType(&thisObj->extraDataList, ExtraLeveledCreature); xLvlCre && xLvlCre->baseForm)
			if (TESForm *templateActor = xLvlCre->baseForm->baseData.templateActor; templateActor && (IS_ID(templateActor, TESLevCreature) || IS_ID(templateActor, TESLevCharacter)))
				REFR_RES = templateActor->refID;
	return true;
}

bool Cmd_GetArmourPenetrated_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(5, result);
	return true;
}

bool Cmd_GetImpactMaterialType_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if IS_ID(actorBase, TESNPC)
		*result = (int)((TESNPC*)actorBase)->impactMaterialType;
	else
		*result = (int)((TESCreature*)actorBase)->materialType;
	return true;
}

bool Cmd_SetImpactMaterialType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESActorBase *actorBase = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type, &actorBase) || (type > 11)) return true;
	if (!actorBase)
	{
		if (!thisObj || NOT_ACTOR(thisObj)) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if IS_ID(actorBase, TESNPC)
		((TESNPC*)actorBase)->impactMaterialType = type;
	else
		((TESCreature*)actorBase)->materialType = type;
	return true;
}

bool Cmd_PushActorAwayAlt_Execute(COMMAND_ARGS)
{
	float force;
	int absPos = 0;
	NiVector3 posVector(0, 0, 0);
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor) && ExtractArgsEx(EXTRACT_ARGS_EX, &force, &posVector.x, &posVector.y, &posVector.z, &absPos))
		if (BaseProcess *process = actor->baseProcess; process && !process->processLevel)
		{
			if (!absPos) posVector += actor->position.PS();
			process->PushActorAway(actor, posVector.x, posVector.y, posVector.z, actor->AdjustPushForce(force));
		}
	return true;
}

bool Cmd_MoveAwayFrom_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	float distance;
	UInt32 flags = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &target, &distance, &flags))
	{
		s_moveAwayDistance = distance;
		UInt32 flagsMask = kPackageFlag_ContinueDuringCombat;
		if (flags & 1)
			flagsMask |= kPackageFlag_AlwaysRun;
		if (flags & 2)
			flagsMask |= kPackageFlag_AlwaysSneak;
		((Actor*)thisObj)->AddBackUpPackage(target, thisObj->parentCell, flagsMask);
	}
	return true;
}

bool Cmd_TravelToRef_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	UInt32 flags = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &target, &flags))
	{
		TESObjectCELL *actorCell = thisObj->parentCell, *targetCell = target->parentCell;
		if (!actorCell || !targetCell) return true;
		if (actorCell->IsInterior())
		{
			if (actorCell != targetCell)
				return true;
		}
		else if (actorCell->worldSpace != targetCell->worldSpace)
			return true;
		UInt32 flagsMask = kPackageFlag_AllowSwimming | kPackageFlag_AllowFalls | kPackageFlag_ContinueDuringCombat;
		if (flags & 1)
			flagsMask |= kPackageFlag_AlwaysRun;
		if (flags & 2)
			flagsMask |= kPackageFlag_AlwaysSneak;
		((Actor*)thisObj)->AddBackUpPackage(target, nullptr, flagsMask);
	}
	return true;
}

bool Cmd_DonnerReedKuruParty_Execute(COMMAND_ARGS)
{
	UInt32 doSet;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &doSet))
		if (auto xDismembered = GetExtraType(&thisObj->extraDataList, ExtraDismemberedLimbs))
		{
			bool wasEaten = (doSet != 0);
			if (xDismembered->wasEaten != wasEaten)
			{
				if (!wasEaten && !xDismembered->dismemberedMask)
					thisObj->extraDataList.RemoveByType(kXData_ExtraDismemberedLimbs);
				else xDismembered->wasEaten = wasEaten;
				thisObj->MarkModified(0x20000);
			}
		}
		else if (doSet)
		{
			xDismembered = ThisCall<ExtraDismemberedLimbs*>(0x430200, Game_HeapAlloc<ExtraDismemberedLimbs>());
			xDismembered->wasEaten = true;
			thisObj->extraDataList.AddExtra(xDismembered);
			thisObj->MarkModified(0x20000);
		}
	return true;
}

bool Cmd_GetEquippedEx_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &listForm))
		if (auto entryList = thisObj->GetContainerChangesList())
		{
			auto listIter = listForm->list.Head();
			do
			{
				if (TESForm *item = listIter->data)
					if (ContChangesEntry *entry = entryList->FindForItem(item); entry && entry->GetEquippedExtra())
					{
						REFR_RES = item->refID;
						break;
					}
			}
			while (listIter = listIter->next);
		}
	return true;
}

bool Cmd_TestEquippedSlots_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor;
	if (thisObj->IsCharacter() && ExtractArgsEx(EXTRACT_ARGS_EX, &armor) && IS_TYPE(armor, TESObjectARMO))
		if (BipedAnim *validBip = ((Character*)thisObj)->GetBipedAnim())
		{
			UInt32 partMask = armor->bipedModel.partMask;
			for (BipedAnim::Data &slotData : validBip->slotData)
				if ((armor = slotData.armor) && IS_TYPE(armor, TESObjectARMO) && (armor->bipedModel.partMask & partMask))
				{
					*result = 1;
					break;
				}
		}
	return true;
}

bool GetFactionList(Actor *actor, TESActorBase *actorBase, TempFormList *tmpFormLst)
{
	if (actorBase)
		actor = nullptr;
	else
	{
		if (!actor || NOT_ACTOR(actor))
			return false;
		actorBase = (TESActorBase*)actor->baseForm;
	}
	auto traverse = actorBase->baseData.factionList.Head();
	do
	{
		if (traverse->data)
			tmpFormLst->Insert(traverse->data->faction);
	}
	while (traverse = traverse->next);
	if (actor)
		if (auto xFactionChanges = GetExtraType(&actor->extraDataList, ExtraFactionChanges); xFactionChanges && xFactionChanges->data)
		{
			traverse = xFactionChanges->data->Head();
			do
			{
				if (FactionListData	*data = traverse->data)
					if (data->rank >= 0)
						tmpFormLst->Insert(data->faction);
					else tmpFormLst->Erase(data->faction);
			}
			while (traverse = traverse->next);
		}
	return !tmpFormLst->Empty();
}

bool Cmd_GetFactions_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	TempFormList *tmpFormLst = GetTempFormList();
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase) && GetFactionList((Actor*)thisObj, actorBase, tmpFormLst))
	{
		TempElements *tmpElements = GetTempElements();
		for (auto lstIter = tmpFormLst->Begin(); lstIter; ++lstIter)
			tmpElements->Append(*lstIter);
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_GetHit3DData_Execute(COMMAND_ARGS)
{
	UInt32 type;
	Actor *actor = (Actor*)thisObj;
	if (IS_ACTOR(actor) && actor->baseProcess && ExtractArgsEx(EXTRACT_ARGS_EX, &type))
		if (ActorHitData *hitData = actor->baseProcess->GetHitData())
		{
			ArrayElementL elements[6], *elemPtr = elements;
			if (type != 1)
			{
				elements[0] = hitData->impactPos.x;
				elements[1] = hitData->impactPos.y;
				elements[2] = hitData->impactPos.z;
				elemPtr += 3;
			}
			if (type)
			{
				elemPtr[0] = hitData->impactAngle.x;
				elemPtr[1] = hitData->impactAngle.y;
				elemPtr[2] = hitData->impactAngle.z;
			}
			*result = (int)CreateArray(elements, (type > 1) ? 6 : 3, scriptObj);
		}
	return true;
}

bool Cmd_GetCreatureSoundsTemplate_Execute(COMMAND_ARGS)
{
	TESCreature *creature;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature) && IS_ID(creature, TESCreature) && creature->audioTemplate)
		REFR_RES = creature->audioTemplate->refID;
	return true;
}

bool Cmd_SetCreatureSoundsTemplate_Execute(COMMAND_ARGS)
{
	TESCreature *creature, *audioTemplate = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature, &audioTemplate) && IS_ID(creature, TESCreature) && (!audioTemplate || IS_ID(audioTemplate, TESCreature)))
		creature->audioTemplate = audioTemplate;
	return true;
}

bool Cmd_SetOnReloadWeaponEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_reloadWeaponEventScripts->Insert(script))
			HOOK_INC(ReloadWeapon);
	}
	else if (s_reloadWeaponEventScripts->Erase(script))
		HOOK_DEC(ReloadWeapon);
	return true;
}

bool Cmd_UpdatePlayer3D_Execute(COMMAND_ARGS)
{
	g_thePlayer->Update3D();
	return true;
}

bool Cmd_SetOnRagdollEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_onRagdollEventScripts->Insert(script))
			HOOK_INC(OnRagdoll);
	}
	else if (s_onRagdollEventScripts->Erase(script))
		HOOK_DEC(OnRagdoll);
	return true;
}

bool Cmd_PushActorNoRagdoll_Execute(COMMAND_ARGS)
{
	float force, angle;
	TESObjectREFR *originRef = nullptr;
	UInt32 doAdjust = 1;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &force, &angle, &originRef, &doAdjust))
	{
		HOOK_SET(ApplyActorVelocity, true);
		Actor *actor = (Actor*)thisObj;
		if (originRef == actor)
			originRef = nullptr;
		if (doAdjust)
			force = actor->AdjustPushForce(force);
		actor->PushActor(force, angle, originRef);
	}
	return true;
}

bool Cmd_GetBodyPartVATSTarget_Execute(COMMAND_ARGS)
{
	const char *resStr = "";
	UInt32 partID;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &partID, &actorBase) && (partID <= 14))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor *)thisObj)->GetActorBase())))
			if (BGSBodyPartData *bpData = actorBase->GetBodyPartData())
				if (BGSBodyPart *bodyPart = bpData->bodyParts[partID]; bodyPart && bodyPart->VATSTarget.m_dataLen)
					resStr = bodyPart->VATSTarget.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

__declspec(noinline) bool __fastcall GetInFactionList(Actor *actor, BGSListForm *facList)
{
	TempFormList *tmpFormLst = GetTempFormList();
	if (GetFactionList(actor, nullptr, tmpFormLst))
	{
		auto listIter = facList->list.Head();
		do
		{
			if (tmpFormLst->HasKey(listIter->data))
				return true;
		}
		while (listIter = listIter->next);
	}
	return false;
}

bool Cmd_GetInFactionList_Execute(COMMAND_ARGS)
{
	BGSListForm *facList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &facList) && GetInFactionList((Actor*)thisObj, facList))
		*result = 1;
	return true;
}

bool Cmd_GetInFactionList_Eval(COMMAND_ARGS_EVAL)
{
	if (arg1 && GetInFactionList((Actor*)thisObj, (BGSListForm*)arg1))
		*result = 1;
	return true;
}

bool Cmd_GetActorTiltAngle_Execute(COMMAND_ARGS)
{
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis) && (axis < 'Z'))
		if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
			*result = charCtrl->tiltAngle[axis - 'X'] * Dbl180dPI;
	return true;
}

bool Cmd_SetActorTiltAngle_Execute(COMMAND_ARGS)
{
	char axis;
	float angle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &angle) && (axis < 'Z'))
		if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
			charCtrl->tiltAngle[axis - 'X'] = angle * FltPId180;
	return true;
}

bool Cmd_ReloadCharController_Execute(COMMAND_ARGS)
{
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor))
		if (MiddleHighProcess *process = (MiddleHighProcess*)actor->baseProcess; process && (process->processLevel <= 1))
		{
			process->SetCharacterController(nullptr);
			actor->AnimateNiNode();
		}
	return true;
}

bool Cmd_GetGroundMaterial_Execute(COMMAND_ARGS)
{
	*result = ((Actor*)thisObj)->GetGroundMaterial();
	return true;
}

bool Cmd_GetGroundMaterial_Eval(COMMAND_ARGS_EVAL)
{
	*result = ((Actor*)thisObj)->GetGroundMaterial();
	return true;
}

__declspec(naked) MuzzleFlash* __fastcall GetMuzzleFlashHook(HighProcess *hiProc)
{
	__asm
	{
		mov		eax, [ecx+0x3D4]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+3], 0
		jz		done
		cmp		word ptr [eax], 0
		jnz		done
		mov		byte ptr [eax+3], 0
		and		dword ptr [ecx+0x130], 0
	done:
		retn
	}
}

__declspec(naked) void __fastcall DoFireWeaponEx(Actor *actor, int, TESObjectWEAP *weapon)
{
	__asm
	{
		mov		edx, [ecx+0x68]
		push	dword ptr [edx+0x118]
		and		dword ptr [edx+0x118], 0
		push	dword ptr [edx+0x114]
		and		dword ptr [edx+0x114], 0
		push	edx
		push	ecx
		mov		ecx, [esp+0x14]
		CALL_EAX(0x523150)
		pop		eax
		pop		dword ptr [eax+0x114]
		pop		dword ptr [eax+0x118]
		mov		ecx, [eax+0x3D4]
		test	ecx, ecx
		jz		done
		mov		byte ptr [ecx+3], 1
	done:
		retn	4
	}
}

bool Cmd_FireWeaponEx_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	char nodeName[0x40];
	nodeName[0] = 0;
	Actor *actor = (Actor*)thisObj;
	if (IS_ACTOR(actor) && ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &nodeName) && IS_ID(weapon, TESObjectWEAP) && weapon->projectile)
		if (HighProcess *hiProc = (HighProcess*)actor->baseProcess; hiProc && !hiProc->processLevel)
		{
			if (*(UInt32*)0x9BAD76 == 0x000242C6)
			{
				SafeWrite32(0x1087F1C, (UInt32)GetMuzzleFlashHook);
				SafeWrite32(0x9BAD76, 0x90002083);
			}

			if (NiNode *altProjNode = nodeName[0] ? actor->GetNode(nodeName) : nullptr)
				hiProc->projectileNode = altProjNode;

			QueuedCmdCall qCall(DoFireWeaponEx, actor->refID);
			qCall.PushArg(weapon);
			qCall.QueueCall();
		}
	return true;
}

bool Cmd_GetActorGravityMult_Execute(COMMAND_ARGS)
{
	if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
		*result = charCtrl->gravityMult;
	return true;
}

bool Cmd_SetActorGravityMult_Execute(COMMAND_ARGS)
{
	float gravityMult;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &gravityMult))
		if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
			charCtrl->gravityMult = gravityMult;
	return true;
}

bool Cmd_ToggleTeammateKillable_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
	{
		Actor *actor = (Actor*)thisObj;
		bool state = (actor->jipActorFlags2 & kHookActorFlag2_TeammateKillable) != 0;
		if (state) *result = 1;
		UInt32 doSet;
		if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &doSet) && (!doSet == state))
			actor->jipActorFlags2 ^= kHookActorFlag2_TeammateKillable;
	}
	return true;
}

bool Cmd_ToggleNoGunWobble_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
	{
		Actor *actor = (Actor*)thisObj;
		bool state = (actor->jipActorFlags2 & kHookActorFlag2_NoGunWobble) != 0;
		if (state) *result = 1;
		UInt32 doSet;
		if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &doSet) && (!doSet == state))
			actor->jipActorFlags2 ^= kHookActorFlag2_NoGunWobble;
	}
	return true;
}

bool Cmd_GetHitNode_Execute(COMMAND_ARGS)
{
	const char *nodeName = "NULL";
	if (Actor *actor = (Actor*)thisObj; IS_ACTOR(actor) && actor->baseProcess)
		if (ActorHitData *hitData = actor->baseProcess->GetHitData(); hitData && hitData->projectile && IS_PROJECTILE(hitData->projectile))
		{
			auto traverse = hitData->projectile->impactDataList.Head();
			do
			{
				if (auto impactData = traverse->data; impactData && (impactData->refr == actor))
					if (hkpRigidBody *rigidBody = impactData->rigidBody)
						if (NiNode *hitNode = rigidBody->GetParentNode())
						{
							nodeName = hitNode->GetName();
							break;
						}
			}
			while (traverse = traverse->next);
		}
	AssignString(PASS_COMMAND_ARGS, nodeName);
	return true;
}

bool Cmd_GetHitExtendedFlag_Execute(COMMAND_ARGS)
{
	static const UInt32 kHitFlags[] =
	{
		ActorHitData::kFlag_TargetIsBlocking, ActorHitData::kFlag_IsCritical, ActorHitData::kFlag_IsFatal,
		ActorHitData::kFlag_DismemberLimb, ActorHitData::kFlag_ExplodeLimb, ActorHitData::kFlag_CrippleLimb,
		ActorHitData::kFlag_BreakWeapon, ActorHitData::kFlag_IsSneakAttack, ActorHitData::kFlag_IsExplosionHit
	};
	Actor *target = (Actor*)thisObj;
	UInt32 flagID;
	if (IS_ACTOR(target) && target->baseProcess && ExtractArgsEx(EXTRACT_ARGS_EX, &flagID) && (flagID <= 8))
		if (ActorHitData *hitData = target->baseProcess->GetHitData(); hitData && (hitData->flags & kHitFlags[flagID]))
			*result = 1;
	return true;
}

bool Cmd_RemoveAllPerks_Execute(COMMAND_ARGS)
{
	UInt32 forTeammates = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &forTeammates))
	{
		Actor *actor = (Actor*)thisObj;
		if (actor->IsPlayer())
		{
			auto perkIter = forTeammates ? g_thePlayer->perkRanksTM.Head() : g_thePlayer->perkRanksPC.Head();
			do
			{
				if (PerkRank *perkRank = perkIter->data)
					actor->RemovePerk(perkRank->perk, forTeammates);
			}
			while (perkIter = perkIter->next);
		}
		else if (s_patchInstallState.NPCPerks)
			if (NPCPerksInfo *perksInfo = actor->extraDataList.perksInfo)
			{
				for (auto perkIter = perksInfo->perkRanks.Begin(); perkIter; ++perkIter)
					RemovePerkNPCHook(actor, 0, perkIter.Key(), 0);
				actor->extraDataList.perksInfo = nullptr;
				s_NPCPerksInfoMap->Erase(actor->refID);
			}
	}
	return true;
}

bool Cmd_GetActorMovementFlags_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		if (ActorMover *actorMover = ((Actor*)thisObj)->actorMover)
			*result = (int)actorMover->GetMovementFlags();
	DoConsolePrint(result);
	return true;
}

//	Credits to Demorome
bool Cmd_GetHitBaseWeaponDamage_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(6, result);
	return true;
}

bool Cmd_GetHitFatigueDamage_Execute(COMMAND_ARGS)
{
	((Actor*)thisObj)->GetHitDataValue(7, result);
	return true;
}
//	===================

bool Cmd_RefreshAnimData_Execute(COMMAND_ARGS)
{
	if IS_ACTOR(thisObj)
		((Actor*)thisObj)->RefreshAnimData();
	return true;
}

bool Cmd_GetActorVelocityAlt_Execute(COMMAND_ARGS)
{
	ResultVars outVel;
	UInt32 getLocal = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outVel.x, &outVel.y, &outVel.z, &getLocal))
		if (bhkCharacterController *charCtrl = thisObj->GetCharacterController())
			if (NiNode *rootNode = thisObj->GetRefNiNode())
			{
				if (!getLocal)
					outVel.Set(charCtrl->velocity.PS());
				else
					outVel.Set(rootNode->WorldRotate().MultiplyVectorInv(_mm_load_ps(charCtrl->velocity)));
				*result = 1;
			}
	return true;
}

bool Cmd_GetExcludedCombatActions_Execute(COMMAND_ARGS)
{
	*result = (int)s_excludedCombatActionsMap->Get(thisObj);
	return true;
}

bool Cmd_SetExcludedCombatActions_Execute(COMMAND_ARGS)
{
	UInt32 excludeMask;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &excludeMask))
	{
		if (excludeMask &= 0x7FFFFFF)
			s_excludedCombatActionsMap()[thisObj] = excludeMask;
		else s_excludedCombatActionsMap->Erase(thisObj);
		if (CombatController *combatCtrl = ((Actor*)thisObj)->GetCombatController())
			combatCtrl->excludedActionsMask = excludeMask;
	}
	return true;
}

__declspec(noinline) NVSEArrayVar* __fastcall GetAllPerks(Actor *actor, UInt32 forTeammates, Script *scriptObj, UInt32 inclHidden)
{
	TempElements *tmpElements = GetTempElements();
	bool isPlayer = actor == g_thePlayer;
	if (isPlayer || (forTeammates && actor->isTeammate))
	{
		auto perkIter = forTeammates ? g_thePlayer->perkRanksTM.Head() : g_thePlayer->perkRanksPC.Head();
		PerkRank *perkRank;
		BGSPerk *perk;
		do
		{
			if ((perkRank = perkIter->data) && (perk = perkRank->perk) && (inclHidden || !perk->data.isHidden))
				tmpElements->Append(perk);
		}
		while (perkIter = perkIter->next);
	}
	if (!isPlayer && s_patchInstallState.NPCPerks && actor->extraDataList.perksInfo)
		for (auto perkIter = actor->extraDataList.perksInfo->perkRanks.Begin(); perkIter; ++perkIter)
			if (inclHidden || !perkIter.Key()->data.isHidden)
				tmpElements->Append(perkIter.Key());
	return !tmpElements->Empty() ? CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj) : nullptr;
}

bool Cmd_GetAllPerks_Execute(COMMAND_ARGS)
{
	UInt32 forTeammates = 0, inclHidden = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &forTeammates, &inclHidden))
		*result = (int)GetAllPerks((Actor*)thisObj, forTeammates, scriptObj, inclHidden);
	return true;
}