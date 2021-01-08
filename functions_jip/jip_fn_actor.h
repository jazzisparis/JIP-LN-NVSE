#pragma once

DEFINE_COMMAND_PLUGIN(GetActorTemplate, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetLeveledActorBase, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCreatureDamage, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetCreatureDamage, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_CMD_ALT_COND_PLUGIN(GetIsPoisoned, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(GetFollowers, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActorLevelingData, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorLevelingData, , 0, 3, kParams_JIP_OneInt_OneFloat_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorVoiceType, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorVoiceType, , 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCreatureReach, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetIsImmobile, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(PickFromList, , 1, 3, kParams_JIP_OneList_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(ToggleCreatureModel, , 0, 3, kParams_JIP_OneString_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(CreatureHasModel, , 0, 2, kParams_JIP_OneString_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCreatureModels, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_ALT_PLUGIN(ModBaseActorValue, ModBaseAV, , 0, 3, kParams_JIP_OneActorValue_OneFloat_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorProcessingLevel, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActorsByProcessingLevel, , 0, 1, kParams_OneInt);
DEFINE_CMD_ALT_COND_PLUGIN(GetActorLightAmount, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(GetActorAlpha, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActorDiveBreath, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetActorDiveBreath, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCombatTargets, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCombatAllies, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetDetectedActors, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetDetectingActors, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCombatDisabled, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetCombatDisabled, , 1, 2, kParams_JIP_OneInt_OneOptionalActor);
DEFINE_COMMAND_PLUGIN(ToggleNoHealthReset, , 0, 2, kParams_JIP_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCurrentStablePackage, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetTeammateUsingAmmo, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetTeammateUsingAmmo, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ToggleDetectionFix, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ToggleIgnoreLockedDoors, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(IsInCombatWith, , 1, 1, kParams_OneActorRef);
DEFINE_CMD_ALT_COND_PLUGIN(IsAttacking, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(StopIdle, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(HolsterWeapon, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetFallTimeElapsed, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetFallTimeRemaining, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(ResetFallTime, , 1, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetRadiationLevelAlt, , , 1, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(IsInWater, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(GetDroppedRefs, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(MoveAwayFromPlayer, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(FaceObject, , 1, 1, kParams_JIP_OneObjectRef);
DEFINE_COMMAND_PLUGIN(Turn, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(GetAshPileSource, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetOnAnimActionEventHandler, , 0, 4, kParams_JIP_OneForm_OneInt_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnPlayGroupEventHandler, , 0, 4, kParams_JIP_OneForm_OneInt_OneForm_OneAnimGroup);
DEFINE_COMMAND_PLUGIN(SetOnHealthDamageEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetOnCrippledLimbEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetOnFireWeaponEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetCurrentAmmo, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCurrentAmmoRounds, , 1, 0, NULL);
DEFINE_COMMAND_ALT_PLUGIN(SetFullNameAlt, SetActorFullNameAlt, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetDetectionValue, , 1, 1, kParams_OneActorRef);
DEFINE_COMMAND_ALT_PLUGIN(GetBaseActorValueAlt, GetBaseAVAlt, , 0, 2, kParams_JIP_OneActorValue_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetSpeedMult, , 1, 1, kParams_OneOptionalInt);
DEFINE_CMD_ALT_COND_PLUGIN(GetIsRagdolled, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(ForceActorDetectionValue, , 1, 2, kParams_JIP_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetActorVelocity, , 1, 1, kParams_JIP_OneOptionalAxis);
DEFINE_CMD_ALT_COND_PLUGIN(IsInAir, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(GetObjectUnderFeet, , 1, 0, NULL);
DEFINE_COMMAND_ALT_PLUGIN(GetActorValueModifier, GetAVMod, , 1, 2, kParams_JIP_OneActorValue_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPerkModifier, , 1, 4, kParams_JIP_OneInt_OneFloat_TwoOptionalForms);
DEFINE_COMMAND_PLUGIN(GetWheelDisabled, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetWheelDisabled, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetHitHealthDamage, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetHitLimbDamage, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(CrippleLimb, , 1, 2, kParams_JIP_OneInt_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(PlayIdleEx, , 1, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetKillXP, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetKiller, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(KillActorAlt, , 1, 3, kParams_JIP_OneOptionalObjectRef_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(ReloadEquippedModels, ReloadModels, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetPlayedIdle, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(IsIdlePlayingEx, , 1, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetWeaponOut, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(AddBaseEffectListEffect, , 0, 2, kParams_JIP_OneSpellItem_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(RemoveBaseEffectListEffect, , 0, 2, kParams_JIP_OneSpellItem_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetHitAttacker, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetHitProjectile, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetHitWeapon, , 1, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetWaterImmersionPerc, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(DismemberLimb, , 1, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetInterruptPackage, , 1, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(IsFleeing, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(SetCurrentAmmoRounds, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(AttachAshPileEx, , 1, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetActorVelocity, , 1, 2, kParams_JIP_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(GetSkeletonModel, , 0, 1, kParams_JIP_OneActorBase);
DEFINE_COMMAND_PLUGIN(SetNPCSkeletonModel, , 0, 2, kParams_JIP_OneActorBase_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetCRESkeletonModel, , 0, 2, kParams_JIP_OneActorBase_OneString);
DEFINE_CMD_ALT_COND_PLUGIN(GetTargetUnreachable, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(SetVATSTargetable, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetCreatureWeaponList, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetDeathItem, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetDeathItem, , 0, 2, kParams_JIP_OneObjectID_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetActorLeveledList, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetOnHitEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetArmourPenetrated, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetImpactMaterialType, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetImpactMaterialType, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(PushActorAwayAlt, , 1, 5, kParams_JIP_OneFloat_ThreeOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(MoveAwayFrom, , 1, 3, kParams_JIP_OneObjectRef_OneFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(TravelToRef, , 1, 2, kParams_JIP_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(DonnerReedKuruParty, TheOtherOtherWhiteMeat, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEquippedEx, , 1, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(TestEquippedSlots, , 1, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(GetFactions, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetHit3DData, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetCreatureSoundsTemplate, , 0, 1, kParams_JIP_OneActorBase);
DEFINE_COMMAND_PLUGIN(SetCreatureSoundsTemplate, , 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetOnReloadWeaponEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(UpdatePlayer3D, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetOnRagdollEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(PushActorNoRagdoll, , 1, 4, kParams_JIP_TwoFloats_OneOptionalObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetBodyPartVATSTarget, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_CMD_ALT_COND_PLUGIN(GetInFactionList, , , 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetActorTiltAngle, , 1, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(SetActorTiltAngle, , 1, 2, kParams_JIP_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(ReloadCharController, , 1, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetGroundMaterial, , , 1, NULL);
DEFINE_COMMAND_PLUGIN(FireWeaponEx, , 1, 2, kParams_JIP_OneObjectID_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetActorGravityMult, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetActorGravityMult, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(SetTeammateKillable, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetNoGunWobble, , 1, 1, kParams_OneInt);

bool Cmd_GetActorTemplate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (actorBase->baseData.templateActor)
		REFR_RES = actorBase->baseData.templateActor->refID;
	return true;
}

bool Cmd_GetLeveledActorBase_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor())
		REFR_RES = ((Actor*)thisObj)->GetActorBase()->refID;
	return true;
}

bool Cmd_GetCreatureDamage_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if IS_TYPE(creature, TESCreature)
		*result = creature->attackDmg.damage;
	return true;
}

bool Cmd_SetCreatureDamage_Execute(COMMAND_ARGS)
{
	int dmg;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &dmg, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if IS_TYPE(creature, TESCreature)
		creature->attackDmg.damage = GetMin(GetMax(dmg, 0), 0xFFFF);
	return true;
}

bool __fastcall GetIsPoisoned(Actor *actor)
{
	if (!actor->IsActor()) return false;
	ActiveEffectList *effList = actor->magicTarget.GetEffectList();
	if (!effList) return false;
	ListNode<ActiveEffect> *iter = effList->Head();
	ActiveEffect *activeEff;
	do
	{
		activeEff = iter->data;
		if (activeEff && (activeEff->spellType == 5))
			return true;
	}
	while (iter = iter->next);
	return false;
}

bool Cmd_GetIsPoisoned_Execute(COMMAND_ARGS)
{
	*result = GetIsPoisoned((Actor*)thisObj);
	return true;
}

bool Cmd_GetIsPoisoned_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetIsPoisoned((Actor*)thisObj);
	return true;
}

bool Cmd_GetFollowers_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!thisObj->IsActor()) return true;
	ExtraFollower *xFollower = GetExtraType(&thisObj->extraDataList, Follower);
	if (!xFollower || !xFollower->followers) return true;
	s_tempElements.Clear();
	ListNode<Actor> *iter = xFollower->followers->Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetActorLevelingData_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 valID;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &valID, &actorBase) || (valID > 2)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (valID == 1) *result = actorBase->baseData.calcMin;
	else if (valID == 2) *result = actorBase->baseData.calcMax;
	else if (actorBase->baseData.flags & 0x80) *result = actorBase->baseData.level / 1000.0;
	else *result = actorBase->baseData.level;
	return true;
}

bool Cmd_SetActorLevelingData_Execute(COMMAND_ARGS)
{
	UInt32 valID;
	float val;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &valID, &val, &actorBase) || (valID > 2)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	switch (valID)
	{
		case 0:
		{
			if (actorBase->baseData.flags & 128)
			{
				if (val < 0.01F) val = 10;
				else val *= 1000;
			}
			else if (val < 1) val = 1;
			actorBase->baseData.level = val;
			break;
		}
		case 1:
		{
			if (val < 1) val = 1;
			actorBase->baseData.calcMin = val;
			if (actorBase->baseData.calcMax < val) actorBase->baseData.calcMax = val;
			break;
		}
		default:
		{
			if (val < 1) val = 1;
			actorBase->baseData.calcMax = val;
			if (actorBase->baseData.calcMin > val) actorBase->baseData.calcMin = val;
		}
	}
	return true;
}

bool Cmd_GetActorVoiceType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (actorBase->baseData.voiceType)
		REFR_RES = actorBase->baseData.voiceType->refID;
	return true;
}

bool Cmd_SetActorVoiceType_Execute(COMMAND_ARGS)
{
	BGSVoiceType *form;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &form, &actorBase) || NOT_TYPE(form, BGSVoiceType)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->baseData.voiceType = form;
	return true;
}

bool Cmd_GetCreatureReach_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if IS_TYPE(creature, TESCreature)
		*result = creature->attackReach;
	return true;
}

bool Cmd_GetIsImmobile_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if (IS_TYPE(creature, TESCreature) && (creature->baseData.flags & 0x800000))
		*result = 1;
	return true;
}

bool Cmd_PickFromList_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSListForm *listForm;
	SInt32 start = 0, len = -1;
	if (!ExtractArgs(EXTRACT_ARGS, &listForm, &start, &len) || !thisObj->GetContainer() || (start < 0)) return true;
	ListNode<TESForm> *iter = listForm->list.Head();
	TESForm *item;
	do
	{
		if (start-- > 0) continue;
		if (!len-- || !(item = iter->data)) break;
		if (thisObj->GetItemCount(item) < 1) continue;
		REFR_RES = item->refID;
		break;
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_ToggleCreatureModel_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 enable;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &enable, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if (IS_TYPE(creature, TESCreature) && creature->modelList.ModelListAction(s_strArgBuffer, enable ? 1 : -1))
		*result = 1;
	return true;
}

bool Cmd_CreatureHasModel_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if (IS_TYPE(creature, TESCreature) && creature->modelList.ModelListAction(s_strArgBuffer, 0))
		*result = 1;
	return true;
}

bool Cmd_GetCreatureModels_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if NOT_TYPE(creature, TESCreature) return true;
	s_tempElements.Clear();
	ListNode<char> *iter = creature->modelList.modelList.Head();
	do
	{
		if (iter->data)
			s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_ModBaseActorValue_Execute(COMMAND_ARGS)
{
	UInt32 actorVal;
	float value;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorVal, &value, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = (TESActorBase*)thisObj->baseForm;
	}
	actorBase->ModActorValue(actorVal, value);
	return true;
}

bool Cmd_GetActorProcessingLevel_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
		*result = (int)((Actor*)thisObj)->baseProcess->processLevel;
	else *result = -1;
	return true;
}

bool Cmd_GetActorsByProcessingLevel_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 procLevel;
	if (!ExtractArgs(EXTRACT_ARGS, &procLevel) || (procLevel > 3)) return true;
	s_tempElements.Clear();
	MobileObject **objArray = g_processManager->objects.data, **arrEnd = objArray;
	objArray += g_processManager->beginOffsets[procLevel];
	arrEnd += g_processManager->endOffsets[procLevel];
	Actor *actor;
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && actor->IsActor())
			s_tempElements.Append(actor);
	}
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

float __fastcall GetActorLightAmount(Actor *actor)
{
	return (actor->IsActor() && actor->baseProcess) ? actor->baseProcess->GetLightAmount() : 0;
}

bool Cmd_GetActorLightAmount_Execute(COMMAND_ARGS)
{
	*result = GetActorLightAmount((Actor*)thisObj);
	return true;
}

bool Cmd_GetActorLightAmount_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetActorLightAmount((Actor*)thisObj);
	return true;
}

bool Cmd_GetActorAlpha_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
		*result = ((Actor*)thisObj)->baseProcess->GetActorAlpha();
	else *result = 0;
	return true;
}

bool Cmd_GetActorDiveBreath_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
		*result = ((Actor*)thisObj)->baseProcess->GetDiveBreath();
	else *result = 0;
	return true;
}

bool Cmd_SetActorDiveBreath_Execute(COMMAND_ARGS)
{
	float breath;
	if (ExtractArgs(EXTRACT_ARGS, &breath) && thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
		((Actor*)thisObj)->baseProcess->SetDiveBreath(GetMax(breath, 0));
	return true;
}

void __fastcall GetCombatActors(TESObjectREFR *thisObj, Script *scriptObj, bool getAllies, double *result)
{
	*result = 0;
	s_tempElements.Clear();
	UInt32 count;
	Actor *actor;
	if (thisObj == g_thePlayer)
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
					s_tempElements.Append(actor);
			}
		}
		else
		{
			CombatTarget *targets = cmbActors->targets.data;
			for (count = cmbActors->targets.size; count; count--, targets++)
			{
				actor = targets->target;
				if (actor) s_tempElements.Append(actor);
			}
		}
	}
	else
	{
		if (!thisObj->IsActor()) return;
		actor = (Actor*)thisObj;
		Actor **actorsArr = NULL;
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
				s_tempElements.Append(actor);
		}
	}
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
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
	*result = 0;
	if (!thisObj->IsActor()) return;
	s_tempElements.Clear();
	Actor *actor = (Actor*)thisObj;
	if (actor == g_thePlayer)
	{
		MobileObject **objArray = g_processManager->objects.data, **arrEnd = objArray;
		objArray += g_processManager->beginOffsets[0];
		arrEnd += g_processManager->endOffsets[0];
		Actor *target;
		for (; objArray != arrEnd; objArray++)
		{
			target = (Actor*)*objArray;
			if (!target || !target->IsActor() || target->GetDead())
				continue;
			if (detecting)
			{
				if (target->GetDetectionValue(actor) <= 0)
					continue;
			}
			else if (!actor->GetLOS(target))
				continue;
			s_tempElements.Append(target);
		}
	}
	else
	{
		if (!actor->baseProcess || actor->baseProcess->processLevel) return;
		HighProcess *hiProcess = (HighProcess*)actor->baseProcess;
		ListNode<DetectionData> *iter = detecting ? hiProcess->detectingActors->Head() : hiProcess->detectedActors->Head();
		DetectionData *data;
		do
		{
			if (!(data = iter->data)) continue;
			if ((data->detectionValue > 0) && !data->actor->GetDead())
				s_tempElements.Append(data->actor);
		}
		while (iter = iter->next);
	}
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
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
	*result = (thisObj->IsActor() && (((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_CombatAIModified)) ? 1 : 0;
	return true;
}

bool Cmd_SetCombatDisabled_Execute(COMMAND_ARGS)
{
	SInt32 disable;
	Actor *target = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &disable, &target) || !thisObj->IsActor()) return true;
	Actor *actor = (Actor*)thisObj;
	if (disable <= 0)
	{
		if (!(actor->jipActorFlags1 & kHookActorFlag1_CombatAIModified)) return true;
		if (actor->jipActorFlags1 & kHookActorFlag1_CombatDisabled)
			HOOK_MOD(StartCombat, false);
		else
		{
			s_forceCombatTargetMap.Erase(actor);
			HOOK_MOD(SetCombatTarget, false);
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
		if (!target) s_forceCombatTargetMap.Erase(actor);
		HOOK_MOD(StartCombat, !target);
		HOOK_MOD(SetCombatTarget, target != NULL);
		actor->jipActorFlags1 ^= kHookActorFlag1_CombatAIModified;
	}
	if (target) s_forceCombatTargetMap[actor] = target;
	else actor->StopCombat();
	return true;
}

bool Cmd_ToggleNoHealthReset_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgs(EXTRACT_ARGS, &enable, &s_healthRestoreRatio))
	{
		HOOK_SET(ResetHPCommand, enable != 0);
		HOOK_SET(ResetHPWakeUp, enable != 0);
	}
	return true;
}

bool Cmd_GetCurrentStablePackage_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESPackage *package = thisObj->IsActor() ? ((Actor*)thisObj)->GetStablePackage() : NULL;
	if (package) REFR_RES = package->refID;
	DoConsolePrint(package);
	return true;
}

bool Cmd_GetTeammateUsingAmmo_Execute(COMMAND_ARGS)
{
	*result = (thisObj->IsActor() && !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_InfiniteAmmo)) ? 1 : 0;
	return true;
}

bool Cmd_SetTeammateUsingAmmo_Execute(COMMAND_ARGS)
{
	UInt32 useAmmo;
	if (ExtractArgs(EXTRACT_ARGS, &useAmmo) && thisObj->IsActor() && (!useAmmo == !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_InfiniteAmmo)))
	{
		((Actor*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_InfiniteAmmo;
		HOOK_MOD(RemoveAmmo, !useAmmo);
	}
	return true;
}

bool Cmd_ToggleDetectionFix_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgs(EXTRACT_ARGS, &enable) && thisObj->IsActor() && (!enable != !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_DetectionFix)))
		((Actor*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_DetectionFix;
	return true;
}

bool Cmd_ToggleIgnoreLockedDoors_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgs(EXTRACT_ARGS, &enable))
		HOOK_SET(ActivateDoor, enable != 0);
	return true;
}

bool Cmd_IsInCombatWith_Execute(COMMAND_ARGS)
{
	*result = 0;
	Actor *target;
	if (ExtractArgs(EXTRACT_ARGS, &target))
	{
		if (thisObj == g_thePlayer)
		{
			CombatActors *cmbActors = g_thePlayer->combatActors;
			if (!cmbActors) return true;
			CombatTarget *targets = cmbActors->targets.data;
			for (UInt32 count = cmbActors->targets.size; count; count--, targets++)
			{
				if (targets->target == target)
				{
					*result = 1;
					break;
				}
			}
		}
		else if (thisObj->IsActor())
			*result = ((Actor*)thisObj)->IsInCombatWith(target);
	}
	return true;
}

bool __fastcall IsAttacking(Actor *actor)
{
	if (!actor->IsActor() || !actor->baseProcess) return false;
	SInt16 currentAction = actor->baseProcess->GetCurrentAction();
	return (currentAction > 1) && (currentAction < 6);
}

bool Cmd_IsAttacking_Execute(COMMAND_ARGS)
{
	*result = IsAttacking((Actor*)thisObj);
	return true;
}

bool Cmd_IsAttacking_Eval(COMMAND_ARGS_EVAL)
{
	*result = IsAttacking((Actor*)thisObj);
	return true;
}

bool Cmd_StopIdle_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
		((Actor*)thisObj)->baseProcess->SetQueuedIdleFlags(0x800);
	return true;
}

bool Cmd_HolsterWeapon_Execute(COMMAND_ARGS)
{
	if (!thisObj->IsActor()) return true;
	Actor *actor = (Actor*)thisObj;
	if (actor->baseProcess && actor->baseProcess->IsWeaponOut())
		actor->baseProcess->SetWeaponOut(actor, false);
	return true;
}

bool Cmd_GetFallTimeElapsed_Execute(COMMAND_ARGS)
{
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	*result = charCtrl ? charCtrl->fallTimeElapsed : 0;
	return true;
}

bool Cmd_GetFallTimeRemaining_Execute(COMMAND_ARGS)
{
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	*result = charCtrl ? charCtrl->fallTimeRemaining : 0;
	return true;
}

bool Cmd_ResetFallTime_Execute(COMMAND_ARGS)
{
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	if (charCtrl)
	{
		charCtrl->startingHeight = thisObj->posZ;
		charCtrl->fallTimeElapsed = 0;
	}
	return true;
}

float __fastcall GetRadiationLevelAlt(Actor *actor)
{
	if (!actor->IsActor() || !actor->baseProcess || actor->baseProcess->processLevel) return 0;
	if (actor == g_thePlayer)
	{
		HighProcess *hiProc = (HighProcess*)actor->baseProcess;
		return hiProc->waterRadsSec + hiProc->rads238 + hiProc->GetRadsSec();
	}
	return actor->GetRadiationLevel();
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
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->inWater : 0;
	return true;
}

bool Cmd_IsInWater_Execute(COMMAND_ARGS)
{
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->inWater : 0;
	return true;
}

bool Cmd_GetDroppedRefs_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!thisObj->IsActor()) return true;
	ExtraDroppedItemList *xDropped = GetExtraType(&thisObj->extraDataList, DroppedItemList);
	if (!xDropped) return true;
	s_tempElements.Clear();
	ListNode<TESObjectREFR> *iter = xDropped->itemRefs.Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_MoveAwayFromPlayer_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor())
		((Actor*)thisObj)->AddBackUpPackage(g_thePlayer, thisObj->parentCell, kPackageFlag_ContinueDuringCombat);
	return true;
}

bool Cmd_FaceObject_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	if (ExtractArgs(EXTRACT_ARGS, &target) && thisObj->IsActor())
		((Actor*)thisObj)->TurnToFaceObject(target);
	return true;
}

bool Cmd_Turn_Execute(COMMAND_ARGS)
{
	float turnAngle;
	if (ExtractArgs(EXTRACT_ARGS, &turnAngle) && thisObj->IsActor())
		((Actor*)thisObj)->TurnAngle(turnAngle);
	return true;
}

bool Cmd_GetAshPileSource_Execute(COMMAND_ARGS)
{
	*result = 0;
	if IS_TYPE(thisObj->baseForm, TESObjectACTI)
	{
		ExtraAshPileRef *xAshPileRef = GetExtraType(&thisObj->extraDataList, AshPileRef);
		if (xAshPileRef && xAshPileRef->sourceRef)
			REFR_RES = xAshPileRef->sourceRef->refID;
	}
	return true;
}

UInt8 s_onAnimEventFlag = 0;

bool SetOnAnimationEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt, animID;
	TESForm *actorOrList;
	if (!ExtractArgs(EXTRACT_ARGS, &script, &addEvnt, &actorOrList, &animID) || NOT_TYPE(script, Script))
		return true;
	UInt8 flag = s_onAnimEventFlag;
	ActorAnimEventCallbacks &eventMap = (flag == kHookActorFlag3_OnAnimAction) ? s_animActionEventMap : s_playGroupEventMap;
	UInt32 hookIdx = (flag == kHookActorFlag3_OnAnimAction) ? kHook_SetAnimAction : kHook_SetAnimGroup;
	ListNode<TESForm> *iter;
	if IS_TYPE(actorOrList, BGSListForm)
		iter = ((BGSListForm*)actorOrList)->list.Head();
	else
	{
		ListNode<TESForm> tempList(actorOrList);
		iter = &tempList;
	}
	Actor *actor;
	AnimEventCallbacks *scriptsMap;
	do
	{
		if (!(actor = (Actor*)iter->data)) continue;
		if (addEvnt)
		{
			if (!actor->IsActor()) continue;
			if (eventMap.Insert(actor, &scriptsMap))
				s_hookInfos[hookIdx].ModUsers(true);
			if ((*scriptsMap)[animID].Insert(script))
				actor->jipActorFlags3 |= flag;
		}
		else
		{
			auto findActor = eventMap.Find(actor);
			if (!findActor) continue;
			auto findAnim = findActor().Find(animID);
			if (!findAnim || !findAnim().Erase(script)) continue;
			if (actor->IsActor())
				actor->jipActorFlags3 &= ~flag;
			if (!findAnim().Empty()) continue;
			findAnim.Remove();
			if (!findActor().Empty()) continue;
			findActor.Remove();
			s_hookInfos[hookIdx].ModUsers(false);
		}
	}
	while (iter = iter->next);
	return true;
}

__declspec(naked) bool Cmd_SetOnAnimActionEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onAnimEventFlag, kHookActorFlag3_OnAnimAction
		jmp		SetOnAnimationEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnPlayGroupEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onAnimEventFlag, kHookActorFlag3_OnPlayGroup
		jmp		SetOnAnimationEventHandler_Execute
	}
}

UInt8 s_actorEventType = 0;

bool SetActorEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	TESForm *actorOrList = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &script, &addEvnt, &actorOrList) || NOT_TYPE(script, Script))
		return true;
	if (!actorOrList)
	{
		if (addEvnt)
		{
			if (s_onHitEventScripts.Insert(script))
				HOOK_MOD(OnHitEvent, true);
		}
		else if (s_onHitEventScripts.Erase(script))
			HOOK_MOD(OnHitEvent, false);
		return true;
	}
	ActorEventCallbacks *eventMap;
	UInt8 flag, hookID;
	switch (s_actorEventType)
	{
		case 0:
			eventMap = &s_healthDamageEventMap;
			flag = kHookActorFlag3_OnHealthDamage;
			hookID = kHook_DamageActorValue;
			break;
		case 1:
			eventMap = &s_crippledLimbEventMap;
			flag = kHookActorFlag3_OnCrippledLimb;
			hookID = kHook_DamageActorValue;
			break;
		case 2:
			eventMap = &s_fireWeaponEventMap;
			flag = kHookActorFlag3_OnFireWeapon;
			hookID = kHook_RemoveAmmo;
			break;
		case 3:
			eventMap = &s_onHitEventMap;
			flag = kHookActorFlag3_OnHit;
			hookID = kHook_OnHitEvent;
			break;
		default:
			return true;
	}
	ListNode<TESForm> *iter;
	Actor *actor;
	EventCallbackScripts *callbacks;
	if IS_TYPE(actorOrList, BGSListForm)
		iter = ((BGSListForm*)actorOrList)->list.Head();
	else
	{
		ListNode<TESForm> tempList(actorOrList);
		iter = &tempList;
	}
	do
	{
		if (!(actor = (Actor*)iter->data)) continue;
		if (addEvnt)
		{
			if (!actor->IsActor()) continue;
			if (eventMap->Insert(actor, &callbacks))
				s_hookInfos[hookID].ModUsers(true);
			callbacks->Insert(script);
			actor->jipActorFlags3 |= flag;
		}
		else
		{
			auto findActor = eventMap->Find(actor);
			if (!findActor || !findActor().Erase(script)) continue;
			if (actor->IsActor())
				actor->jipActorFlags3 &= ~flag;
			if (!findActor().Empty()) continue;
			findActor.Remove();
			s_hookInfos[hookID].ModUsers(false);
		}
	}
	while (iter = iter->next);
	return true;
}

__declspec(naked) bool Cmd_SetOnHealthDamageEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_actorEventType, 0
		jmp		SetActorEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnCrippledLimbEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_actorEventType, 1
		jmp		SetActorEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnFireWeaponEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_actorEventType, 2
		jmp		SetActorEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnHitEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_actorEventType, 3
		jmp		SetActorEventHandler_Execute
	}
}

bool Cmd_GetCurrentAmmo_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
	{
		ContChangesEntry *ammoInfo = ((Actor*)thisObj)->baseProcess->GetAmmoInfo();
		if (ammoInfo && ammoInfo->type) REFR_RES = ammoInfo->type->refID;
	}
	return true;
}

bool Cmd_GetCurrentAmmoRounds_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
	{
		ContChangesEntry *ammoInfo = ((Actor*)thisObj)->baseProcess->GetAmmoInfo();
		if (ammoInfo) *result = ammoInfo->countDelta;
	}
	return true;
}

bool Cmd_SetFullNameAlt_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	TESFullName *fullName;
	UInt32 modFlag = 4;
	if (IS_TYPE(form, TESNPC) || IS_TYPE(form, TESCreature))
	{
		fullName = &((TESActorBase*)form)->fullName;
		modFlag = 0x20;
	}
	else if IS_TYPE(form, BGSTalkingActivator)
		fullName = &((BGSTalkingActivator*)form)->fullName;
	else if IS_TYPE(form, TESObjectCELL)
		fullName = &((TESObjectCELL*)form)->fullName;
	else return true;
	fullName->name.Set(s_strArgBuffer);
	form->MarkAsModified(modFlag);
	return true;
}

bool Cmd_GetDetectionValue_Execute(COMMAND_ARGS)
{
	Actor *target;
	if (ExtractArgs(EXTRACT_ARGS, &target) && thisObj->IsActor())
		*result = ((Actor*)thisObj)->GetDetectionValue(target);
	else *result = 0;
	return true;
}

bool Cmd_GetBaseActorValueAlt_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 actorVal;
	TESActorBase *actorBase = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &actorVal, &actorBase))
	{
		if (actorBase)
			*result = actorBase->avOwner.GetActorValue(actorVal);
		else if (thisObj && thisObj->IsActor())
			*result = (int)((Actor*)thisObj)->avOwner.GetBaseActorValue(actorVal);
	}
	return true;
}

bool Cmd_SetSpeedMult_Execute(COMMAND_ARGS)
{
	UInt32 speedMult = 0;
	if (ExtractArgs(EXTRACT_ARGS, &speedMult) && thisObj->IsActor())
	{
		if (speedMult) ((Actor*)thisObj)->SetActorValueInt(0x15, speedMult);
		BaseProcess *baseProc = ((Actor*)thisObj)->baseProcess;
		if (baseProc && baseProc->unk2C)
			baseProc->unk2C->flags &= ~0x3000;
	}
	return true;
}

bool __fastcall GetIsRagdolled(Actor *actor)
{
	return (actor->IsActor() && actor->baseProcess) ? (actor->baseProcess->GetKnockedState() == 1) : false;
}

bool Cmd_GetIsRagdolled_Execute(COMMAND_ARGS)
{
	*result = GetIsRagdolled((Actor*)thisObj);
	return true;
}

bool Cmd_GetIsRagdolled_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetIsRagdolled((Actor*)thisObj);
	return true;
}

bool Cmd_ForceActorDetectionValue_Execute(COMMAND_ARGS)
{
	SInt32 detectionValue = 0, oprType = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &detectionValue, &oprType) || !thisObj->IsActor()) return true;
	Actor *actor = (Actor*)thisObj;
	if (detectionValue)
	{
		UInt32 *valPtr;
		if (s_forceDetectionValueMap.Insert(actor, &valPtr))
		{
			actor->jipActorFlags2 |= kHookActorFlag2_ForceDetectionVal;
			HOOK_MOD(GetDetectionValue, true);
		}
		Coordinate packedVal(oprType != 0, detectionValue);
		*valPtr = packedVal.xy;
	}
	else if (s_forceDetectionValueMap.Erase(actor))
	{
		actor->jipActorFlags2 ^= kHookActorFlag2_ForceDetectionVal;
		HOOK_MOD(GetDetectionValue, false);
	}
	return true;
}

bool Cmd_GetActorVelocity_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis = 0;
	if (ExtractArgs(EXTRACT_ARGS, &axis))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (charCtrl)
		{
			if (axis) *result = charCtrl->velocity[axis - 'X'];
			else *result = Vector3Length(&charCtrl->velocity);
		}
	}
	return true;
}

bool Cmd_IsInAir_Execute(COMMAND_ARGS)
{
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	*result = (charCtrl && (charCtrl->chrContext.hkState & 2)) ? 1 : 0;
	return true;
}

bool Cmd_IsInAir_Eval(COMMAND_ARGS_EVAL)
{
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	*result = (charCtrl && (charCtrl->chrContext.hkState & 2)) ? 1 : 0;
	return true;
}

bool Cmd_GetObjectUnderFeet_Execute(COMMAND_ARGS)
{
	*result = 0;
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	if (charCtrl && charCtrl->bodyUnderFeet)
	{
		TESObjectREFR *refr = GetCdBodyRef(&charCtrl->bodyUnderFeet->cdBody);
		if (refr) REFR_RES = refr->refID;
	}
	return true;
}

bool Cmd_GetActorValueModifier_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 actorVal, duration = 3;
	if (!ExtractArgs(EXTRACT_ARGS, &actorVal, &duration) || !thisObj->IsActor()) return true;
	ActiveEffectList *effList = ((Actor*)thisObj)->magicTarget.GetEffectList();
	if (!effList) return true;
	float modifier = 0;
	ActiveEffect *activeEff;
	EffectSetting *effSetting;
	ListNode<ActiveEffect> *iter = effList->Head();
	do
	{
		activeEff = iter->data;
		if (!activeEff || !activeEff->bApplied || !activeEff->effectItem) continue;
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
	*result = 0;
	UInt32 entryPointID;
	float baseValue;
	TESForm *filterForm1 = NULL, *filterForm2 = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &entryPointID, &baseValue, &filterForm1, &filterForm2) || (entryPointID > 73) || !thisObj->IsActor()) return true;
	switch (g_entryPointConditionInfo[entryPointID].numTabs)
	{
		case 1:
			ApplyPerkModifiers(entryPointID, thisObj, &baseValue);
			break;
		case 2:
			ApplyPerkModifiers(entryPointID, thisObj, filterForm1, &baseValue);
			break;
		case 3:
			ApplyPerkModifiers(entryPointID, thisObj, filterForm1, filterForm2, &baseValue);
	}
	*result = baseValue;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetWheelDisabled_Execute(COMMAND_ARGS)
{
	*result = (thisObj->IsActor() && (((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_DisableWheel)) ? 1 : 0;
	return true;
}

bool Cmd_SetWheelDisabled_Execute(COMMAND_ARGS)
{
	UInt32 disable;
	if (ExtractArgs(EXTRACT_ARGS, &disable) && thisObj->IsActor() && (!disable != !(((Actor*)thisObj)->jipActorFlags1 & kHookActorFlag1_DisableWheel)))
		((Actor*)thisObj)->jipActorFlags1 ^= kHookActorFlag1_DisableWheel;
	return true;
}

__declspec(naked) float __fastcall AdjustDmgByDifficulty(ActorHitData *hitData)
{
	__asm
	{
		mov		edx, g_thePlayer
		mov		eax, 0x119B310
		cmp		dword ptr [ecx+4], edx
		jz		isPlayer
		add		eax, 0x14
	isPlayer:
		mov		edx, [edx+0x7B8]
		mov		eax, [eax+edx*4]
		fld		dword ptr [ecx+0x14]
		fmul	dword ptr [eax+4]
		retn
	}
}

void __fastcall GetHitData(Actor *target, UInt8 dataType, double *result)
{
	*result = 0;
	if (!target->IsActor() || !target->baseProcess) return;
	ActorHitData *hitData = target->baseProcess->GetHitData();
	if (!hitData) return;
	switch (dataType)
	{
		case 0:
			*result = AdjustDmgByDifficulty(hitData);
			break;
		case 1:
			*result = hitData->limbDmg;
			break;
		case 2:
			if (hitData->source)
				REFR_RES = hitData->source->refID;
			break;
		case 3:
			if (hitData->projectile)
				REFR_RES = hitData->projectile->refID;
			break;
		case 4:
			if (hitData->weapon)
				REFR_RES = hitData->weapon->refID;
			break;
		case 5:
			if (hitData->flags & 0x80000000)
				*result = 1;
			break;
		default: break;
	}
}

bool Cmd_GetHitHealthDamage_Execute(COMMAND_ARGS)
{
	GetHitData((Actor*)thisObj, 0, result);
	return true;
}

bool Cmd_GetHitLimbDamage_Execute(COMMAND_ARGS)
{
	GetHitData((Actor*)thisObj, 1, result);
	return true;
}

bool Cmd_CrippleLimb_Execute(COMMAND_ARGS)
{
	UInt32 limbID;
	Actor *attacker = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &limbID, &attacker) && thisObj->IsActor() && (limbID <= 6))
		((Actor*)thisObj)->DamageActorValue(kAVCode_PerceptionCondition + limbID, -100, attacker);
	return true;
}

bool Cmd_PlayIdleEx_Execute(COMMAND_ARGS)
{
	TESIdleForm *idleAnim = NULL;
	Actor *actor = (Actor*)thisObj;
	if (!ExtractArgs(EXTRACT_ARGS, &idleAnim) || !actor->IsActor() || !actor->baseProcess || (idleAnim && NOT_TYPE(idleAnim, TESIdleForm))) return true;
	if (!idleAnim) idleAnim = ThisCall<TESIdleForm*>(0x600950, *g_idleAnimsDirectoryMap, actor, actor->baseProcess->GetLowProcess40());
	else if (idleAnim->children) idleAnim = idleAnim->FindIdle(actor);
	if (idleAnim) actor->PlayIdle(idleAnim);
	return true;
}

bool Cmd_GetKillXP_Execute(COMMAND_ARGS)
{
	*result = thisObj->IsActor() ? ((Actor*)thisObj)->GetKillXP() : 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetKiller_Execute(COMMAND_ARGS)
{
	*result = 0;
	Actor *actor = (Actor*)thisObj;
	actor = (actor->IsActor() && actor->HasHealth(0)) ? actor->killer : NULL;
	if (actor) REFR_RES = actor->refID;
	DoConsolePrint(actor);
	return true;
}

__declspec(naked) bool Cmd_KillActorAlt_Execute(COMMAND_ARGS)
{
	__asm	jmp		KillActor
}

bool Cmd_ReloadEquippedModels_Execute(COMMAND_ARGS)
{
	Character *character = (Character*)thisObj;
	if (character->IsCharacter() && character->GetNiNode() && character->validBip01Names)
	{
		bool isPlayer = character == g_thePlayer;
		ValidBip01Names::Data *slotData = character->validBip01Names->slotData;
		UInt32 slotIdx;
	Do1stPerson:
		slotIdx = 20;
		do
		{
			if ((slotIdx != 14) && slotData->armor && IS_ID(slotData->armor, TESObjectARMO) && slotData->boneNode)
			{
				((NiNode*)slotData->boneNode->m_parent)->RemoveObject(slotData->boneNode);
				slotData->boneNode = NULL;
			}
			slotData++;
		}
		while (--slotIdx);
		if (isPlayer)
		{
			isPlayer = false;
			slotData = g_thePlayer->VB01N1stPerson->slotData;
			goto Do1stPerson;
		}
		ThisCall(0x605D70, character->baseForm, character);
	}
	return true;
}

bool Cmd_GetPlayedIdle_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
	{
		TESIdleForm *idleAnim = ((Actor*)thisObj)->baseProcess->GetIdleForm350();
		if (idleAnim) REFR_RES = idleAnim->refID;
	}
	return true;
}

__declspec(naked) bool __fastcall IsIdlePlayingEx(Actor *actor, TESIdleForm *idleAnim)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		done
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x1E4]
		test	eax, eax
		jz		done
		mov		ecx, eax
		CALL_EAX(0x4985F0)
		test	al, al
		setz	al
		jnz		done
		mov		eax, esi
		mov		eax, [eax+0x68]
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x718]
		cmp		eax, edi
		setz	al
	done:
		pop		edi
		pop		esi
		retn
	}
}

bool Cmd_IsIdlePlayingEx_Execute(COMMAND_ARGS)
{
	TESIdleForm *idleAnim;
	if (ExtractArgs(EXTRACT_ARGS, &idleAnim))
		*result = IsIdlePlayingEx((Actor*)thisObj, idleAnim);
	else *result = 0;
	return true;
}

bool Cmd_SetWeaponOut_Execute(COMMAND_ARGS)
{
	UInt32 setOut;
	if (ExtractArgs(EXTRACT_ARGS, &setOut) && thisObj->IsActor())
		ThisCall(0x8A6840, thisObj, setOut != 0);
	return true;
}

bool Cmd_AddBaseEffectListEffect_Execute(COMMAND_ARGS)
{
	SpellItem *spell;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &spell, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (actorBase->spellList.spellList.GetIndexOf(spell) == -1)
		actorBase->spellList.spellList.Insert(spell);
	return true;
}

bool Cmd_RemoveBaseEffectListEffect_Execute(COMMAND_ARGS)
{
	SpellItem *spell;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &spell, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->spellList.spellList.Remove(spell);
	return true;
}

bool Cmd_GetHitAttacker_Execute(COMMAND_ARGS)
{
	GetHitData((Actor*)thisObj, 2, result);
	return true;
}

bool Cmd_GetHitProjectile_Execute(COMMAND_ARGS)
{
	GetHitData((Actor*)thisObj, 3, result);
	return true;
}

bool Cmd_GetHitWeapon_Execute(COMMAND_ARGS)
{
	GetHitData((Actor*)thisObj, 4, result);
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
	if (ExtractArgs(EXTRACT_ARGS, &bodyPartID, &explode) && thisObj->IsActor())
		((Actor*)thisObj)->DismemberLimb(bodyPartID, explode != 0);
	return true;
}

bool Cmd_GetInterruptPackage_Execute(COMMAND_ARGS)
{
	*result = -1;
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
	{
		TESPackage *package = ((Actor*)thisObj)->baseProcess->GetInterruptPackage();
		if (package) *result = package->type;
	}
	return true;
}

bool Cmd_IsFleeing_Execute(COMMAND_ARGS)
{
	*result = (thisObj->IsActor() && ((Actor*)thisObj)->IsFleeing()) ? 1 : 0;
	return true;
}

bool Cmd_IsFleeing_Eval(COMMAND_ARGS_EVAL)
{
	*result = (thisObj->IsActor() && ((Actor*)thisObj)->IsFleeing()) ? 1 : 0;
	return true;
}

bool Cmd_SetCurrentAmmoRounds_Execute(COMMAND_ARGS)
{
	SInt32 ammoRounds;
	if (ExtractArgs(EXTRACT_ARGS, &ammoRounds) && thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
	{
		ContChangesEntry *ammoInfo = ((Actor*)thisObj)->baseProcess->GetAmmoInfo();
		if (ammoInfo) ammoInfo->countDelta = ammoRounds;
	}
	return true;
}

bool Cmd_AttachAshPileEx_Execute(COMMAND_ARGS)
{
	UInt32 opcodeOffset = *opcodeOffsetPtr;
	TESForm *form;
	if (ExtractArgs(EXTRACT_ARGS, &form) && thisObj->IsActor() && (form->jipFormFlags6 & kHookFormFlag6_IsAshPile))
	{
		s_altAshPile = form;
		*opcodeOffsetPtr = opcodeOffset;
		AttachAshPile(PASS_COMMAND_ARGS);
	}
	return true;
}

bool Cmd_SetActorVelocity_Execute(COMMAND_ARGS)
{
	char axis;
	float velocity;
	if (ExtractArgs(EXTRACT_ARGS, &axis, &velocity))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (charCtrl && charCtrl->refObject)
			((hkpCharacterProxy*)charCtrl->refObject)->velocity[axis - 'X'] = velocity;
	}
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
		test	word ptr [ecx+0x4A], 0x40
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
	if (ExtractArgs(EXTRACT_ARGS, &actorBase))
		resStr = GetActorModel(actorBase)->GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetNPCSkeletonModel_Execute(COMMAND_ARGS)
{
	TESNPC *npc;
	s_strArgBuffer[0] = 0;
	if (ExtractArgs(EXTRACT_ARGS, &npc, &s_strArgBuffer) && IS_TYPE(npc, TESNPC))
		GetActorModel(npc)->nifPath.Set(s_strArgBuffer);
	return true;
}

bool Cmd_SetCRESkeletonModel_Execute(COMMAND_ARGS)
{
	TESCreature *creature;
	if (ExtractArgs(EXTRACT_ARGS, &creature, &s_strArgBuffer) && IS_TYPE(creature, TESCreature))
		GetActorModel(creature)->nifPath.Set(s_strArgBuffer);
	return true;
}

bool Cmd_GetTargetUnreachable_Execute(COMMAND_ARGS)
{
	*result = (thisObj->IsActor() && ((Actor*)thisObj)->HasNoPath()) ? 1 : 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetTargetUnreachable_Eval(COMMAND_ARGS_EVAL)
{
	*result = (thisObj->IsActor() && ((Actor*)thisObj)->HasNoPath()) ? 1 : 0;
	return true;
}

bool Cmd_SetVATSTargetable_Execute(COMMAND_ARGS)
{
	UInt32 targetable;
	if (ExtractArgs(EXTRACT_ARGS, &targetable) && thisObj->IsActor() && (!targetable == !(((Actor*)thisObj)->jipActorFlags2 & kHookActorFlag2_NonTargetable)))
	{
		((Actor*)thisObj)->jipActorFlags2 ^= kHookActorFlag2_NonTargetable;
		HOOK_MOD(AddVATSTarget, !targetable);
	}
	return true;
}

bool Cmd_GetCreatureWeaponList_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if (IS_TYPE(creature, TESCreature) && creature->weaponList)
		REFR_RES = creature->weaponList->refID;
	return true;
}

bool Cmd_GetDeathItem_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (actorBase->baseData.deathItem)
		REFR_RES = actorBase->baseData.deathItem->refID;
	return true;
}

bool Cmd_SetDeathItem_Execute(COMMAND_ARGS)
{
	TESForm *item;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &item, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->baseData.deathItem = item;
	return true;
}

bool Cmd_GetActorLeveledList_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor())
	{
		ExtraLeveledCreature *xLvlCre = GetExtraType(&thisObj->extraDataList, LeveledCreature);
		if (xLvlCre && xLvlCre->baseForm)
		{
			TESForm *templateActor = xLvlCre->baseForm->baseData.templateActor;
			if (templateActor && (IS_TYPE(templateActor, TESLevCreature) || IS_TYPE(templateActor, TESLevCharacter)))
				REFR_RES = templateActor->refID;
		}
	}
	return true;
}

bool Cmd_GetArmourPenetrated_Execute(COMMAND_ARGS)
{
	GetHitData((Actor*)thisObj, 5, result);
	return true;
}

bool Cmd_GetImpactMaterialType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if IS_TYPE(actorBase, TESNPC)
		*result = (int)((TESNPC*)actorBase)->impactMaterialType;
	else
		*result = (int)((TESCreature*)actorBase)->materialType;
	return true;
}

bool Cmd_SetImpactMaterialType_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &type, &actorBase) || (type > 11)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if IS_TYPE(actorBase, TESNPC)
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
	if (ExtractArgs(EXTRACT_ARGS, &force, &posVector.x, &posVector.y, &posVector.z, &absPos) && thisObj->IsActor())
	{
		Actor *actor = (Actor*)thisObj;
		BaseProcess *process = actor->baseProcess;
		if (process && !process->processLevel)
		{
			if (!absPos) posVector += *(actor->PosVector());
			process->PushActorAway(actor, posVector.x, posVector.y, posVector.z, actor->AdjustPushForce(force));
		}
	}
	return true;
}

bool Cmd_MoveAwayFrom_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	float distance;
	UInt32 flags = 0;
	if (ExtractArgs(EXTRACT_ARGS, &target, &distance, &flags) && thisObj->IsActor())
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
	if (ExtractArgs(EXTRACT_ARGS, &target, &flags) && thisObj->IsActor())
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
		((Actor*)thisObj)->AddBackUpPackage(target, NULL, flagsMask);
	}
	return true;
}

bool Cmd_DonnerReedKuruParty_Execute(COMMAND_ARGS)
{
	UInt32 doSet;
	if (ExtractArgs(EXTRACT_ARGS, &doSet) && thisObj->IsActor())
	{
		ExtraDismemberedLimbs *xDismembered = GetExtraType(&thisObj->extraDataList, DismemberedLimbs);
		if (xDismembered)
		{
			bool wasEaten = (doSet != 0);
			if (xDismembered->wasEaten != wasEaten)
			{
				if (!wasEaten && !xDismembered->dismemberedMask)
					RemoveExtraData(&thisObj->extraDataList, xDismembered, true);
				else xDismembered->wasEaten = wasEaten;
				thisObj->MarkAsModified(0x20000);
			}
		}
		else if (doSet)
		{
			xDismembered = (ExtraDismemberedLimbs*)GameHeapAlloc(sizeof(ExtraDismemberedLimbs));
			ThisCall(0x430200, xDismembered);
			xDismembered->wasEaten = true;
			AddExtraData(&thisObj->extraDataList, xDismembered);
			thisObj->MarkAsModified(0x20000);
		}
	}
	return true;
}

bool Cmd_GetEquippedEx_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSListForm *listForm;
	if (ExtractArgs(EXTRACT_ARGS, &listForm) && thisObj->IsActor())
	{
		Actor *actor = (Actor*)thisObj;
		if NOT_TYPE(actor, Creature)
		{
			ValidBip01Names *validBip = actor->GetValidBip01Names();
			if (validBip)
			{
				s_tempFormList.Clear();
				TESForm *item;
				for (ValidBip01Names::Data &slotData : validBip->slotData)
				{
					item = slotData.item;
					if (item && !item->IsArmorAddon())
						s_tempFormList.Insert(item);
				}
				if (!s_tempFormList.Empty())
				{
					ListNode<TESForm> *traverse = listForm->list.Head();
					do
					{
						item = traverse->data;
						if (!s_tempFormList.HasKey(item)) continue;
						REFR_RES = item->refID;
						break;
					}
					while (traverse = traverse->next);
				}
			}
		}
		else if (actor->baseProcess)
		{
			ContChangesEntry *weaponInfo = actor->baseProcess->GetWeaponInfo();
			if (weaponInfo && (listForm->list.GetIndexOf(weaponInfo->type) >= 0))
				REFR_RES = weaponInfo->type->refID;
		}
	}
	return true;
}

bool Cmd_TestEquippedSlots_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectARMO *armor;
	if (ExtractArgs(EXTRACT_ARGS, &armor) && thisObj->IsCharacter() && IS_TYPE(armor, TESObjectARMO))
	{
		ValidBip01Names *validBip = ((Character*)thisObj)->GetValidBip01Names();
		if (validBip)
		{
			UInt32 partMask = armor->bipedModel.partMask;
			for (ValidBip01Names::Data &slotData : validBip->slotData)
			{
				armor = slotData.armor;
				if (!armor || NOT_TYPE(armor, TESObjectARMO) || !(armor->bipedModel.partMask & partMask))
					continue;
				*result = 1;
				break;
			}
		}
	}
	return true;
}

bool __fastcall GetFactionList(Actor *actor, TESActorBase *actorBase)
{
	if (actorBase)
		actor = NULL;
	else
	{
		if (!actor || !actor->IsActor())
			return false;
		actorBase = actor->GetActorBase();
	}
	s_tempFormList.Clear();
	ListNode<FactionListData> *traverse = actorBase->baseData.factionList.Head();
	FactionListData	*data;
	do
	{
		data = traverse->data;
		if (data) s_tempFormList.Insert(data->faction);
	}
	while (traverse = traverse->next);
	if (actor)
	{
		ExtraFactionChanges *xFactionChanges = GetExtraType(&actor->extraDataList, FactionChanges);
		if (xFactionChanges && xFactionChanges->data)
		{
			traverse = xFactionChanges->data->Head();
			do
			{
				data = traverse->data;
				if (!data) continue;
				if (data->rank >= 0)
					s_tempFormList.Insert(data->faction);
				else s_tempFormList.Erase(data->faction);
			}
			while (traverse = traverse->next);
		}
	}
	return !s_tempFormList.Empty();
}

bool Cmd_GetFactions_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &actorBase) && GetFactionList((Actor*)thisObj, actorBase))
	{
		s_tempElements.Clear();
		for (auto lstIter = s_tempFormList.Begin(); lstIter; ++lstIter)
			s_tempElements.Append(*lstIter);
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	else *result = 0;
	return true;
}

bool Cmd_GetHit3DData_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	Actor *actor = (Actor*)thisObj;
	if (ExtractArgs(EXTRACT_ARGS, &type) && actor->IsActor() && actor->baseProcess)
	{
		ActorHitData *hitData = actor->baseProcess->GetHitData();
		if (hitData)
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
			AssignCommandResult(CreateArray(elements, (type > 1) ? 6 : 3, scriptObj), result);
		}
	}
	return true;
}

bool Cmd_GetCreatureSoundsTemplate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESCreature *creature;
	if (ExtractArgs(EXTRACT_ARGS, &creature) && IS_ID(creature, TESCreature) && creature->audioTemplate)
		REFR_RES = creature->audioTemplate->refID;
	return true;
}

bool Cmd_SetCreatureSoundsTemplate_Execute(COMMAND_ARGS)
{
	TESCreature *creature, *audioTemplate = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &creature, &audioTemplate) && IS_TYPE(creature, TESCreature) && (!audioTemplate || IS_ID(audioTemplate, TESCreature)))
		creature->audioTemplate = audioTemplate;
	return true;
}

bool Cmd_SetOnReloadWeaponEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgs(EXTRACT_ARGS, &script, &addEvnt) || NOT_TYPE(script, Script)) return true;
	if (addEvnt)
	{
		if (s_reloadWeaponEventScripts.Insert(script))
			HOOK_MOD(ReloadWeapon, true);
	}
	else if (s_reloadWeaponEventScripts.Erase(script))
		HOOK_MOD(ReloadWeapon, false);
	return true;
}

bool Cmd_UpdatePlayer3D_Execute(COMMAND_ARGS)
{
	g_thePlayer->UpdatePlayer3D();
	return true;
}

bool Cmd_SetOnRagdollEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgs(EXTRACT_ARGS, &script, &addEvnt) || NOT_TYPE(script, Script)) return true;
	if (addEvnt)
	{
		if (s_onRagdollEventScripts.Insert(script))
			HOOK_MOD(OnRagdoll, true);
	}
	else if (s_onRagdollEventScripts.Erase(script))
		HOOK_MOD(OnRagdoll, false);
	return true;
}

bool Cmd_PushActorNoRagdoll_Execute(COMMAND_ARGS)
{
	float force, angle;
	TESObjectREFR *originRef = NULL;
	UInt32 doAdjust = 1;
	if (ExtractArgs(EXTRACT_ARGS, &force, &angle, &originRef, &doAdjust) && thisObj->IsActor())
	{
		if (originRef == thisObj)
			originRef = NULL;
		HOOK_SET(ApplyActorVelocity, true);
		((Actor*)thisObj)->PushActor(force, angle, originRef, doAdjust != 0);
	}
	return true;
}

bool Cmd_GetBodyPartVATSTarget_Execute(COMMAND_ARGS)
{
	const char *resStr = "";
	UInt32 partID;
	TESActorBase *actorBase = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &partID, &actorBase) && (partID <= 14))
	{
		if (!actorBase)
		{
			if (!thisObj || !thisObj->IsActor()) return true;
			actorBase = ((Actor*)thisObj)->GetActorBase();
		}
		if (actorBase)
		{
			BGSBodyPartData *bpData = actorBase->GetBodyPartData();
			if (bpData)
			{
				BGSBodyPart *bodyPart = bpData->bodyParts[partID];
				if (bodyPart && bodyPart->VATSTarget.m_dataLen)
					resStr = bodyPart->VATSTarget.m_data;
			}
		}
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool __fastcall GetInFactionList(Actor *actor, BGSListForm *facList)
{
	if (GetFactionList(actor, NULL))
	{
		ListNode<TESForm> *listIter = facList->list.Head();
		do
		{
			if (s_tempFormList.HasKey(listIter->data))
				return true;
		}
		while (listIter = listIter->next);
	}
	return false;
}

bool Cmd_GetInFactionList_Execute(COMMAND_ARGS)
{
	BGSListForm *facList;
	if (ExtractArgs(EXTRACT_ARGS, &facList))
		*result = GetInFactionList((Actor*)thisObj, facList);
	else *result = 0;
	return true;
}

bool Cmd_GetInFactionList_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetInFactionList((Actor*)thisObj, (BGSListForm*)arg1);
	return true;
}

bool Cmd_GetActorTiltAngle_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis;
	if (ExtractArgs(EXTRACT_ARGS, &axis) && (axis < 'Z'))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (charCtrl)
			*result = ((axis == 'X') ? charCtrl->tiltAngleX : charCtrl->tiltAngleY) / kDblPId180;
	}
	return true;
}

bool Cmd_SetActorTiltAngle_Execute(COMMAND_ARGS)
{
	char axis;
	float angle;
	if (ExtractArgs(EXTRACT_ARGS, &axis, &angle) && (axis < 'Z'))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (charCtrl)
		{
			angle *= kFltPId180;
			if (axis == 'X')
				charCtrl->tiltAngleX = angle;
			else charCtrl->tiltAngleY = angle;
		}
	}
	return true;
}

bool Cmd_ReloadCharController_Execute(COMMAND_ARGS)
{
	Actor *actor = (Actor*)thisObj;
	if (actor->IsActor() && actor->baseProcess)
	{
		MiddleHighProcess *process = (MiddleHighProcess*)actor->baseProcess;
		if (process->processLevel <= 1)
		{
			process->SetCharacterController(NULL);
			actor->AnimateNiNode();
		}
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

__declspec(naked) ProjectileData* __fastcall GetProjectileDataHook(HighProcess *hiProc)
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
		mov		dword ptr [ecx+0x130], 0
	done:
		retn
	}
}

__declspec(naked) void __fastcall DoFireWeaponEx(TESObjectREFR *refr, int EDX, TESObjectWEAP *weapon)
{
	__asm
	{
		mov		edx, [ecx+0x68]
		push	dword ptr [edx+0x118]
		mov		dword ptr [edx+0x118], 0
		push	dword ptr [edx+0x114]
		mov		dword ptr [edx+0x114], 0
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
	static bool hookInstalled = false;
	TESObjectWEAP *weapon;
	s_strArgBuffer[0] = 0;
	if (ExtractArgs(EXTRACT_ARGS, &weapon, &s_strArgBuffer) && IS_TYPE(weapon, TESObjectWEAP) && weapon->projectile && thisObj->IsActor())
	{
		HighProcess *hiProc = (HighProcess*)((Actor*)thisObj)->baseProcess;
		if (!hiProc || hiProc->processLevel)
			return true;

		if (!hookInstalled)
		{
			hookInstalled = true;
			SafeWrite32(0x1087F1C, (UInt32)GetProjectileDataHook);
			SafeWriteBuf(0x9BAD66, "\xC7\x01\x00\x00\x00\x00\xEB\x0C", 8);
		}

		NiNode *altProjNode = s_strArgBuffer[0] ? thisObj->GetNode(s_strArgBuffer) : NULL;
		if (altProjNode)
			hiProc->projectileNode = altProjNode;

		if (CdeclCall<bool>(0x8C7AA0))
		{
			QueuedCmdCall qCall(DoFireWeaponEx, thisObj->refID, 1);
			qCall.args[0] = weapon;
			AddQueuedCmdCall(qCall);
		}
		else DoFireWeaponEx(thisObj, 0, weapon);
	}
	return true;
}

bool Cmd_GetActorGravityMult_Execute(COMMAND_ARGS)
{
	bhkCharacterController *charCtrl = thisObj->GetCharacterController();
	*result = charCtrl ? charCtrl->gravityMult : 0;
	return true;
}

bool Cmd_SetActorGravityMult_Execute(COMMAND_ARGS)
{
	float gravityMult;
	if (ExtractArgs(EXTRACT_ARGS, &gravityMult))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (charCtrl) charCtrl->gravityMult = gravityMult;
	}
	return true;
}

bool Cmd_SetTeammateKillable_Execute(COMMAND_ARGS)
{
	UInt32 doSet;
	if (ExtractArgs(EXTRACT_ARGS, &doSet) && thisObj->IsActor())
	{
		if (doSet) ((Actor*)thisObj)->jipActorFlags2 |= kHookActorFlag2_TeammateKillable;
		else ((Actor*)thisObj)->jipActorFlags2 &= ~kHookActorFlag2_TeammateKillable;
	}
	return true;
}

bool Cmd_SetNoGunWobble_Execute(COMMAND_ARGS)
{
	UInt32 doSet;
	if (ExtractArgs(EXTRACT_ARGS, &doSet) && thisObj->IsActor())
	{
		if (doSet) ((Actor*)thisObj)->jipActorFlags2 |= kHookActorFlag2_NoGunWobble;
		else ((Actor*)thisObj)->jipActorFlags2 &= ~kHookActorFlag2_NoGunWobble;
	}
	return true;
}