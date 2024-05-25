#pragma once

DEFINE_COMMAND_PLUGIN(GetMerchantContainer, 1, nullptr);
DEFINE_COMMAND_PLUGIN(LNGetAggroRadius, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(LNSetAggroRadius, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_CMD_COND_PLUGIN(IsRaceInList, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetCreatureType, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetCombatStyle, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(CopyFaceGenFrom, 1, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetServiceFlag, 0, kParams_OneActorBase_OneInt);
DEFINE_COMMAND_PLUGIN(SetServiceFlag, 0, kParams_OneActorBase_TwoInts);
DEFINE_COMMAND_PLUGIN(GetActorTemplateFlag, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetActorTemplateFlag, 0, kParams_TwoInts_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(LNSetActorTemplate, 0, kParams_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(GetBaseFactionRank, 0, kParams_OneForm_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetBaseFactionRank, 0, kParams_OneFaction_OneInt_OneOptionalActorBase);
DEFINE_COMMAND_ALT_PLUGIN(GetEquippedData, GetEqData, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetEquippedData, SetEqData, 1, kParams_OneInt_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(EquipItemData, 1, kParams_OneObjectID_OneOptionalFloat_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(GetBaseKarma, 0, kParams_OneOptionalActorBase);
DEFINE_COMMAND_PLUGIN(SetBaseKarma, 0, kParams_OneInt_OneOptionalActorBase);
DEFINE_CMD_COND_PLUGIN(ActorHasEffect, 1, kParams_OneEffect);
DEFINE_COMMAND_PLUGIN(GetBodyPartData, 0, kParams_OneActorBase);
DEFINE_COMMAND_PLUGIN(SetBodyPartData, 0, kParams_OneActorBase_OneForm);

bool Cmd_GetMerchantContainer_Execute(COMMAND_ARGS)
{
	if (TESObjectREFR *containerRef = thisObj->GetMerchantContainer())
		REFR_RES = containerRef->refID;
	return true;
}

bool Cmd_LNGetAggroRadius_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			if (actorBase->ai.aggroRadiusBehavior)
				*result = actorBase->ai.aggroRadius;
	return true;
}

bool Cmd_LNSetAggroRadius_Execute(COMMAND_ARGS)
{
	int value;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
		{
			value = GetMax(value, 0);
			actorBase->ai.aggroRadiusBehavior = value != 0;
			actorBase->ai.aggroRadius = value;
		}
	return true;
}

bool __fastcall IsRaceInList(TESObjectREFR *thisObj, BGSListForm *listForm)
{
	if (thisObj->IsCharacter())
		if (TESRace *race = ((TESNPC*)thisObj->baseForm)->race.race)
		{
			auto iter = listForm->list.Head();
			do
			{
				if (iter->data == race)
					return true;
			}
			while (iter = iter->next);
		}
	return false;
}

bool Cmd_IsRaceInList_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm) && IsRaceInList(thisObj, listForm))
		*result = 1;
	return true;
}

bool Cmd_IsRaceInList_Eval(COMMAND_ARGS_EVAL)
{
	if (IsRaceInList(thisObj, (BGSListForm*)arg1))
		*result = 1;
	return true;
}

bool Cmd_GetCreatureType_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESCreature *creature = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &creature))
		if (creature || (thisObj && IS_ACTOR(thisObj) && (creature = (TESCreature*)((Actor*)thisObj)->GetActorBase())))
			if IS_ID(creature, TESCreature)
			{
				*result = creature->type;
				DoConsolePrint(result);
			}
	return true;
}

bool Cmd_GetCombatStyle_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
	{
		TESCombatStyle *combatStyle = nullptr;
		if (actorBase)
			combatStyle = actorBase->GetCombatStyle();
		else if (thisObj && IS_ACTOR(thisObj))
			combatStyle = ((Actor*)thisObj)->GetCombatStyle();
		if (combatStyle)
			REFR_RES = combatStyle->refID;
	}
	return true;
}

bool Cmd_CopyFaceGenFrom_Execute(COMMAND_ARGS)
{
	TESNPC *srcNPC = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &srcNPC) && thisObj->IsCharacter() && (!srcNPC || IS_ID(srcNPC, TESNPC)))
	{
		TESNPC *destNPC = (TESNPC*)thisObj->baseForm;
		if (srcNPC)
		{
			if (AppearanceUndo **pAprUndo; s_appearanceUndoMap().InsertKey(destNPC, &pAprUndo))
				*pAprUndo = new (malloc(sizeof(AppearanceUndo))) AppearanceUndo(destNPC);
			destNPC->SetSex(srcNPC->baseData.flags);
			destNPC->SetRace(srcNPC->race.race);
			destNPC->CopyAppearance(srcNPC);
		}
		else if (AppearanceUndo *aprUndo = s_appearanceUndoMap().GetErase(destNPC))
		{
			aprUndo->Undo(destNPC);
			aprUndo->Destroy();
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase, &flag) && (actorBase->ai.buySellsAndServices & flag))
		*result = 1;
	return true;
}

bool Cmd_SetServiceFlag_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase;
	UInt32 flag, inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase, &flag, &inval))
		if (inval) actorBase->ai.buySellsAndServices |= flag;
		else actorBase->ai.buySellsAndServices &= ~flag;
	return true;
}

bool Cmd_GetActorTemplateFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &flag, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			if (actorBase->baseData.templateFlags & flag)
				*result = 1;
	return true;
}

bool Cmd_SetActorTemplateFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag, inval;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &flag, &inval, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			if (inval) actorBase->baseData.templateFlags |= flag;
			else actorBase->baseData.templateFlags &= ~flag;
	return true;
}

bool Cmd_LNSetActorTemplate_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			actorBase->baseData.templateActor = form;
	return true;
}

bool Cmd_GetBaseFactionRank_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESFaction *faction;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &faction, &actorBase) && IS_ID(faction, TESFaction))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = (TESActorBase*)thisObj->baseForm)))
			*result = actorBase->baseData.GetFactionRank(faction);
	return true;
}

bool Cmd_SetBaseFactionRank_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	int rank;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &faction, &rank, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = (TESActorBase*)thisObj->baseForm)))
			actorBase->baseData.SetFactionRank(faction, GetMax(rank, -1));
	return true;
}

bool Cmd_GetEquippedData_Execute(COMMAND_ARGS)
{
	UInt32 slot;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &slot) && IS_ACTOR(thisObj))
		if (auto xChanges = GetExtraType(&thisObj->extraDataList, ExtraContainerChanges); xChanges && xChanges->data && xChanges->data->objList)
		{
			slot = TESBipedModelForm::MaskForSlot(slot);
			auto traverse = xChanges->data->objList->Head();
			do
			{
				if (auto entry = traverse->data; entry && entry->extendData)
				{
					TESForm *item = entry->type;
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
					if (auto xDataList = entry->GetEquippedExtra())
					{
						auto xHealth = GetExtraType(xDataList, ExtraHealth);
						auto xModFlags = GetExtraType(xDataList, ExtraWeaponModFlags);
						ArrayElementL elements[3] = {item, xHealth ? xHealth->health : -1, xModFlags ? xModFlags->flags : 0};
						*result = (int)CreateArray(elements, 3, scriptObj);
						break;
					}
				}
			}
			while (traverse = traverse->next);
		}
	return true;
}

void SetEquippedData(Actor *actor, TESForm *form, float health, UInt8 flags, bool lock, bool silent)
{
	if (auto entry = actor->GetContainerChangesEntry(form); entry && entry->extendData)
	{
		auto xdlIter = entry->extendData->Head();
		do
		{
			if (auto xData = xdlIter->data)
			{
				if (health > 0)
				{
					if (auto xHealth = GetExtraType(xData, ExtraHealth); !xHealth || (ULNG(health) != ULNG(xHealth->health)))
						continue;
				}
				if (flags)
				{
					if (auto xModFlags = GetExtraType(xData, ExtraWeaponModFlags); !xModFlags || (xModFlags->flags != flags))
						continue;
				}
				else if (xData->HasType(kXData_ExtraWeaponModFlags))
					continue;
				actor->EquipItem(entry->type, 1, xData, 1, lock, silent);
				break;
			}
		}
		while (xdlIter = xdlIter->next);
	}
}

bool Cmd_SetEquippedData_Execute(COMMAND_ARGS)
{
	UInt32 arrID, lock = 0, silent = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &lock, &silent) && IS_ACTOR(thisObj))
		if (NVSEArrayVar *inArray = LookupArrayByID(arrID); inArray && (GetArraySize(inArray) == 3))
		{
			ArrayElementL vals[3];
			GetElements(inArray, vals, NULL);
			SetEquippedData((Actor*)thisObj, vals[0].Form(), (float)vals[1].Number(), (UInt8)vals[2].Number(), lock != 0, silent != 0);
		}
	return true;
}

bool Cmd_EquipItemData_Execute(COMMAND_ARGS)
{
	TESForm *form;
	float health = 0;
	UInt32 flags = 0, lock = 0, silent = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &health, &flags, &lock, &silent) && IS_ACTOR(thisObj))
		SetEquippedData((Actor*)thisObj, form, health, flags, lock != 0, silent != 0);
	return true;
}

bool Cmd_GetBaseKarma_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			*result = actorBase->baseData.karma;
	return true;
}

bool Cmd_SetBaseKarma_Execute(COMMAND_ARGS)
{
	SInt32 karma;
	TESActorBase *actorBase = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &karma, &actorBase))
		if (actorBase || (thisObj && IS_ACTOR(thisObj) && (actorBase = ((Actor*)thisObj)->GetActorBase())))
			actorBase->baseData.karma = karma;
	return true;
}

__declspec(noinline) bool __fastcall ActorHasEffect(Actor *actor, EffectSetting *effSetting)
{
	if (IS_ACTOR(actor))
		if (ActiveEffectList *effList = actor->magicTarget.GetEffectList())
		{
			auto iter = effList->Head();
			do
			{
				if (ActiveEffect *effect = iter->data; effect && (effect->effectItem->setting == effSetting) && effect->bActive && !effect->bTerminated && effect->magicItem)
					return true;
			}
			while (iter = iter->next);
		}
	return false;
}

bool Cmd_ActorHasEffect_Execute(COMMAND_ARGS)
{
	EffectSetting *effSetting;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &effSetting) && ActorHasEffect((Actor*)thisObj, effSetting))
		*result = 1;
	DoConsolePrint(result);
	return true;
}

bool Cmd_ActorHasEffect_Eval(COMMAND_ARGS_EVAL)
{
	if (ActorHasEffect((Actor*)thisObj, (EffectSetting*)arg1))
		*result = 1;
	return true;
}

bool Cmd_GetBodyPartData_Execute(COMMAND_ARGS)
{
	TESActorBase *actorBase;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &actorBase))
		if (BGSBodyPartData *bpData = actorBase->GetBodyPartData())
			REFR_RES = bpData->refID;
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