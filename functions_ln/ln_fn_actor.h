#pragma once

DEFINE_COMMAND_PLUGIN(GetMerchantContainer, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(LNGetAggroRadius, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(LNSetAggroRadius, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_CMD_ALT_COND_PLUGIN(IsRaceInList, , , 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetCreatureType, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCombatStyle, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(CopyFaceGenFrom, , 1, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetServiceFlag, , 0, 2, kParams_JIP_OneActorBase_OneInt);
DEFINE_COMMAND_PLUGIN(SetServiceFlag, , 0, 3, kParams_JIP_OneActorBase_TwoInts);
DEFINE_COMMAND_PLUGIN(GetActorTemplateFlag, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorTemplateFlag, , 0, 3, kParams_JIP_TwoInts_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(LNSetActorTemplate, , 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetBaseFactionRank, , 0, 2, kParams_JIP_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetBaseFactionRank, , 0, 3, kParams_JIP_OneFaction_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_ALT_PLUGIN(GetEquippedData, GetEqData, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetEquippedData, SetEqData, , 1, 3, kParams_JIP_OneInt_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(EquipItemData, , 1, 5, kParams_EquipData);
DEFINE_COMMAND_PLUGIN(GetBaseKarma, , 0, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetBaseKarma, , 0, 2, kParams_JIP_OneInt_OneOptionalActorBase);
DEFINE_CMD_ALT_COND_PLUGIN(ActorHasEffect, , , 1, kParams_JIP_OneEffect);
DEFINE_COMMAND_PLUGIN(GetBodyPartData, , 0, 1, kParams_JIP_OneActorBase);
DEFINE_COMMAND_PLUGIN(SetBodyPartData, , 0, 2, kParams_JIP_OneActorBase_OneForm);

bool Cmd_GetMerchantContainer_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *containerRef = thisObj->GetMerchantContainer();
	if (containerRef) REFR_RES = containerRef->refID;
	return true;
}

bool Cmd_LNGetAggroRadius_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	*result = actorBase->ai.aggroRadiusBehavior ? actorBase->ai.aggroRadius : 0;
	return true;
}

bool Cmd_LNSetAggroRadius_Execute(COMMAND_ARGS)
{
	UInt32 value;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &value, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->ai.aggroRadiusBehavior = value ? 1 : 0;
	actorBase->ai.aggroRadius = value;
	return true;
}

bool __fastcall IsRaceInList(TESObjectREFR *thisObj, BGSListForm *listForm)
{
	if (thisObj->IsCharacter())
	{
		TESRace *race = ((TESNPC*)thisObj->baseForm)->race.race;
		if (race)
		{
			ListNode<TESForm> *iter = listForm->list.Head();
			do
			{
				if (iter->data == race)
					return true;
			}
			while (iter = iter->next);
		}
	}
	return false;
}

bool Cmd_IsRaceInList_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm))
		*result = IsRaceInList(thisObj, listForm);
	else *result = 0;
	return true;
}

bool Cmd_IsRaceInList_Eval(COMMAND_ARGS_EVAL)
{
	*result = IsRaceInList(thisObj, (BGSListForm*)arg1);
	return true;
}

bool Cmd_GetCreatureType_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESCreature *creature = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &creature)) return true;
	if (!creature)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		creature = (TESCreature*)((Actor*)thisObj)->GetActorBase();
	}
	if IS_ID(creature, TESCreature)
		*result = creature->type;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetCombatStyle_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase)) return true;
	TESCombatStyle *combatStyle = NULL;
	if (actorBase)
		combatStyle = actorBase->GetCombatStyle();
	else if (thisObj && thisObj->IsActor())
		combatStyle = ((Actor*)thisObj)->GetCombatStyle();
	if (combatStyle) REFR_RES = combatStyle->refID;
	return true;
}

bool Cmd_CopyFaceGenFrom_Execute(COMMAND_ARGS)
{
	TESNPC *srcNPC = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &srcNPC) && thisObj->IsCharacter() && (!srcNPC || IS_ID(srcNPC, TESNPC)))
	{
		TESNPC *destNPC = (TESNPC*)thisObj->baseForm;
		if (srcNPC)
		{
			if (!s_appearanceUndoMap.HasKey(destNPC))
			{
				AppearanceUndo *aprUndo = (AppearanceUndo*)malloc(sizeof(AppearanceUndo));
				new (aprUndo) AppearanceUndo(destNPC);
				s_appearanceUndoMap[destNPC] = aprUndo;
			}
			destNPC->SetSex(srcNPC->baseData.flags);
			destNPC->SetRace(srcNPC->race.race);
			destNPC->CopyAppearance(srcNPC);
		}
		else
		{
			auto aprUndo = s_appearanceUndoMap.Find(destNPC);
			if (!aprUndo) return true;
			aprUndo->Undo(destNPC);
			aprUndo->Destroy();
			aprUndo.Remove();
		}
		*(bool*)0x11D5AE0 = true;
		ThisCall(0x8D3FA0, thisObj);
	}
	return true;
}

bool Cmd_GetServiceFlag_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase;
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase, &flag))
		*result = (actorBase->ai.buySellsAndServices & flag) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetServiceFlag_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase;
	UInt32 flag, inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase, &flag, &inval))
	{
		if (inval) actorBase->ai.buySellsAndServices |= flag;
		else actorBase->ai.buySellsAndServices &= ~flag;
	}
	return true;
}

bool Cmd_GetActorTemplateFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 flag;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &flag, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	*result = (actorBase->baseData.templateFlags & flag) ? 1 : 0;
	return true;
}

bool Cmd_SetActorTemplateFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag, inval;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &flag, &inval, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (inval) actorBase->baseData.templateFlags |= flag;
	else actorBase->baseData.templateFlags &= ~flag;
	return true;
}

bool Cmd_LNSetActorTemplate_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->baseData.templateActor = form;
	return true;
}

bool Cmd_GetBaseFactionRank_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESFaction *faction;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &faction, &actorBase) || NOT_ID(faction, TESFaction)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	*result = actorBase->baseData.GetFactionRank(faction);
	return true;
}

bool Cmd_SetBaseFactionRank_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	SInt32 rank;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &faction, &rank, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	if (rank < -1) rank = -1;
	actorBase->baseData.SetFactionRank(faction, rank);
	return true;
}

bool Cmd_GetEquippedData_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 slot;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &slot) || !thisObj->IsActor())
		return true;
	ExtraContainerChanges *xChanges = GetExtraType(&thisObj->extraDataList, ContainerChanges);
	if (!xChanges || !xChanges->data || !xChanges->data->objList)
		return true;
	slot = TESBipedModelForm::MaskForSlot(slot);
	ListNode<ContChangesEntry> *traverse = xChanges->data->objList->Head();
	ContChangesEntry *entry;
	TESForm *item;
	ExtraDataList *xDataList;
	do
	{
		if (!(entry = traverse->data) || !entry->extendData) continue;
		item = entry->type;
		if IS_ID(item, TESObjectWEAP)
		{
			if (!(slot & TESBipedModelForm::eSlot_Weapon))
				continue;
		}
		else if IS_TYPE(item, TESObjectARMO)
		{
			if (!(slot & ((TESObjectARMO*)item)->bipedModel.partMask))
				continue;
		}
		else continue;
		if (xDataList = entry->GetEquippedExtra())
		{
			ExtraHealth *xHealth = GetExtraType(xDataList, Health);
			ExtraWeaponModFlags *xModFlags = GetExtraType(xDataList, WeaponModFlags);
			ArrayElementL elements[3] = { item, xHealth ? xHealth->health : -1, xModFlags ? xModFlags->flags : 0 };
			AssignCommandResult(CreateArray(elements, 3, scriptObj), result);
			break;
		}
	}
	while (traverse = traverse->next);
	return true;
}

void SetEquippedData(Actor *actor, TESForm *form, float health, UInt8 flags, bool lock, bool silent)
{
	ContChangesEntry *entry = actor->GetContainerChangesEntry(form);
	if (!entry || !entry->extendData) return;
	ListNode<ExtraDataList> *xdlIter = entry->extendData->Head();
	ExtraDataList *xData;
	ExtraHealth *xHealth;
	ExtraWeaponModFlags *xModFlags;
	do
	{
		if (!(xData = xdlIter->data)) continue;
		if (health > 0)
		{
			xHealth = GetExtraType(xData, Health);
			if (!xHealth || (health != xHealth->health)) continue;
		}
		xModFlags = GetExtraType(xData, WeaponModFlags);
		if (flags)
		{
			if (!xModFlags || (xModFlags->flags != flags)) continue;
		}
		else if (xModFlags) continue;
		actor->EquipItem(entry->type, 1, xData, 1, lock, silent);
		break;
	}
	while (xdlIter = xdlIter->next);
}

bool Cmd_SetEquippedData_Execute(COMMAND_ARGS)
{
	UInt32 arrID, lock = 0, silent = 1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &lock, &silent) || !thisObj->IsActor())
		return true;
	NVSEArrayVar *inArray = LookupArrayByID(arrID);
	if (!inArray || (GetArraySize(inArray) != 3))
		return true;
	ArrayElementL vals[3];
	GetElements(inArray, vals, NULL);
	SetEquippedData((Actor*)thisObj, vals[0].Form(), (float)vals[1].Number(), (UInt8)vals[2].Number(), lock != 0, silent != 0);
	return true;
}

bool Cmd_EquipItemData_Execute(COMMAND_ARGS)
{
	TESForm *form;
	float health = 0;
	UInt32 flags = 0, lock = 0, silent = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &health, &flags, &lock, &silent) && thisObj->IsActor())
		SetEquippedData((Actor*)thisObj, form, health, flags, lock != 0, silent != 0);
	return true;
}

bool Cmd_GetBaseKarma_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	*result = actorBase->baseData.karma;
	return true;
}

bool Cmd_SetBaseKarma_Execute(COMMAND_ARGS)
{
	SInt32 karma;
	TESActorBase *actorBase = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &karma, &actorBase)) return true;
	if (!actorBase)
	{
		if (!thisObj || !thisObj->IsActor()) return true;
		actorBase = ((Actor*)thisObj)->GetActorBase();
	}
	actorBase->baseData.karma = karma;
	return true;
}

bool __fastcall ActorHasEffect(Actor *actor, EffectSetting *effSetting)
{
	if (actor->IsActor())
	{
		ActiveEffectList *effList = actor->magicTarget.GetEffectList();
		if (effList)
		{
			ListNode<ActiveEffect> *iter = effList->Head();
			ActiveEffect *effect;
			do
			{
				effect = iter->data;
				if (effect && (effect->effectItem->setting == effSetting) && effect->bApplied && effect->magicItem)
					return true;
			}
			while (iter = iter->next);
		}
	}
	return false;
}

bool Cmd_ActorHasEffect_Execute(COMMAND_ARGS)
{
	EffectSetting *effSetting;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &effSetting))
		*result = ActorHasEffect((Actor*)thisObj, effSetting);
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_ActorHasEffect_Eval(COMMAND_ARGS_EVAL)
{
	*result = ActorHasEffect((Actor*)thisObj, (EffectSetting*)arg1);
	return true;
}

bool Cmd_GetBodyPartData_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESActorBase *actorBase;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
	{
		BGSBodyPartData *bpData = actorBase->GetBodyPartData();
		if (bpData) REFR_RES = bpData->refID;
	}
	return true;
}

bool Cmd_SetBodyPartData_Execute(COMMAND_ARGS)
{
	TESCreature *creature;
	BGSBodyPartData *bpData;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature, &bpData) && IS_ID(creature, TESCreature) && IS_ID(bpData, BGSBodyPartData))
		creature->bodyPartData = bpData;
	return true;
}