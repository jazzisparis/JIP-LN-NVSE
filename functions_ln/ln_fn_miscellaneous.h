#pragma once

DEFINE_CMD_ALT_COND_PLUGIN(GetArmorClass, GetAC, , 0, kParams_OneOptionalObjectID);
DEFINE_COMMAND_ALT_PLUGIN(SetGlobalValue, SGV, , 0, 2, kParams_OneString_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(GetWorldspaceParentWorldspace, GetWorldParentWorld, , 0, 1, kParams_JIP_OneWorldspace);
DEFINE_COMMAND_PLUGIN(GetCellCoords, , 0, 1, kParams_JIP_OneOptionalCell);
DEFINE_COMMAND_ALT_PLUGIN(GetBipedModelList, GetBMList, , 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_ALT_PLUGIN(SetBipedModelList, SetBMList, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetPCCanUsePowerArmor, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetIdleAnimPath, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetIdleAnimPath, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(LNGetName, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(LNSetName, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(GetGlobalTimeMultiplier, ggtm, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetWaterFormEffect, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(ar_Cat, pinto_Cat, , 0, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetFormFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetFormFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetIngestibleFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetIngestibleFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(IsSkillMagazine, , 0, 1, kParams_OneForm);
DEFINE_CMD_ALT_COND_PLUGIN(PlayerHasKey, , , 1, NULL);
DEFINE_COMMAND_ALT_PLUGIN(GetAnimObjectIdle, GetANIOIdle, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(SetAnimObjectIdle, SetANIOIdle, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetPickupSound, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetPickupSound, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetPutdownSound, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetPutdownSound, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetContainerOpenSound, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetContainerOpenSound, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetContainerCloseSound, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetContainerCloseSound, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetPlayerRegions, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetZoneFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetZoneFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetImpactDataSet, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetImpactDataSet, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(LNIsPlayable, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(LNSetIsPlayable, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetZone, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(AddNoteNS, , 0, 1, kParams_JIP_OneNote);
DEFINE_COMMAND_PLUGIN(CrosshairRefInList, , 0, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetShowQuestItems, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetWaterTrait, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetWaterTrait, , 0, 3, kParams_JIP_OneForm_OneInt_OneDouble);
DEFINE_COMMAND_PLUGIN(GetActiveQuest, , 0, 0, NULL);

UInt8 __fastcall GetArmorClass(Character *character)
{
	if (!character->IsCharacter()) return 0;
	ValidBip01Names *equipment = character->GetValidBip01Names();
	if (!equipment) return true;
	ValidBip01Names::Data *slotData = equipment->slotData;
	TESObjectARMO *armor;
	TESBipedModelForm *biped;
	for (UInt8 count = 20; count; count--, slotData++)
	{
		armor = slotData->armor;
		if (!armor || NOT_TYPE(armor, TESObjectARMO)) continue;
		biped = &armor->bipedModel;
		if (biped->partMask & 4)
			return (biped->bipedFlags & 0x80) ? 3 : ((biped->bipedFlags & 8) ? 2 : 1);
	}
	return 0;
}

bool Cmd_GetArmorClass_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectARMO *armor = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &armor)) return true;
	if (armor)
	{
		if NOT_TYPE(armor, TESObjectARMO) return true;
		TESBipedModelForm *biped = &armor->bipedModel;
		*result = (biped->bipedFlags & 0x80) ? 3 : ((biped->bipedFlags & 8) ? 2 : 1);
	}
	else if (thisObj) *result = GetArmorClass((Character*)thisObj);
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetArmorClass_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetArmorClass((Character*)thisObj);
	return true;
}

bool Cmd_SetGlobalValue_Execute(COMMAND_ARGS)
{
	char varName[0x40];
	float value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &value)) return true;
	ListNode<TESGlobal> *iter = g_dataHandler->globalList.Head();
	TESGlobal *global;
	do
	{
		if ((global = iter->data) && !StrCompare(global->name.m_data, varName))
		{
			global->data = value;
			break;
		}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_GetWorldspaceParentWorldspace_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wspc;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc) && wspc->parent)
		REFR_RES = wspc->parent->refID;
	return true;
}

bool Cmd_GetCellCoords_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &cell)) return true;
	NVSEArrayVar *outArray = CreateStringMap(NULL, NULL, 0, scriptObj);
	if (!cell) cell = thisObj ? thisObj->parentCell : g_thePlayer->parentCell;
	SetElement(outArray, ArrayElementL("x"), (cell && !cell->IsInterior()) ? ArrayElementL(cell->coords.exterior->x) : ArrayElementL(0.0));
	SetElement(outArray, ArrayElementL("y"), (cell && !cell->IsInterior()) ? ArrayElementL(cell->coords.exterior->y) : ArrayElementL(0.0));
	AssignCommandResult(outArray, result);
	if (IsConsoleOpen() && cell && !cell->IsInterior()) Console_Print("GetCellCoords >> %d, %d", cell->coords.exterior->x, cell->coords.exterior->y);
	return true;
}

bool Cmd_GetBipedModelList_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	BGSListForm *models = NULL;
	if IS_TYPE(form, TESObjectARMO) models = ((TESObjectARMO*)form)->bipedModelList.models;
	else if IS_ID(form, TESObjectWEAP) models = ((TESObjectWEAP*)form)->bipedModelList.models;
	if (models) REFR_RES = models->refID;
	return true;
}

bool Cmd_SetBipedModelList_Execute(COMMAND_ARGS)
{
	TESForm *form;
	BGSListForm *list = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &list) && (!list || IS_ID(list, BGSListForm)))
	{
		if IS_TYPE(form, TESObjectARMO) ((TESObjectARMO*)form)->bipedModelList.models = list;
		else if IS_ID(form, TESObjectWEAP) ((TESObjectWEAP*)form)->bipedModelList.models = list;
	}
	return true;
}

bool Cmd_GetPCCanUsePowerArmor_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->canUsePA;
	return true;
}

bool Cmd_GetIdleAnimPath_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESIdleForm *idle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idle) && IS_ID(idle, TESIdleForm))
		resStr = idle->anim.GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetIdleAnimPath_Execute(COMMAND_ARGS)
{
	TESIdleForm *idle;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idle, &path) && IS_ID(idle, TESIdleForm))
		idle->anim.SetModelPath(path);
	return true;
}

bool Cmd_LNGetName_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (form || (thisObj && (form = thisObj->baseForm))))
	{
		if (thisObj) resStr = s_refNamesMap.Get(thisObj);
		if (!resStr) resStr = form->GetTheName();
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_LNSetName_Execute(COMMAND_ARGS)
{
	char nameStr[0x80];
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &nameStr, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	TESFullName *fullName = DYNAMIC_CAST(form, TESForm, TESFullName);
	if (fullName) fullName->name.Set(nameStr);
	return true;
}

bool Cmd_GetGlobalTimeMultiplier_Execute(COMMAND_ARGS)
{
	*result = *(float*)0x11AC3A0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetWaterFormEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWaterForm *water;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &water) && IS_ID(water, TESWaterForm) && water->drinkEffect)
		REFR_RES = water->drinkEffect->refID;
	return true;
}

bool Cmd_ar_Cat_Execute(COMMAND_ARGS)
{
	UInt32 arr1ID, arr2ID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arr1ID, &arr2ID)) return true;
	NVSEArrayVar *inArray = LookupArrayByID(arr1ID);
	NVSEArrayVar *catArray = LookupArrayByID(arr2ID);
	if (!inArray || !catArray) return true;
	UInt32 size;
	ArrayElementR *vals = GetArrayData(catArray, &size);
	if (!vals) return true;
	do
	{
		AppendElement(inArray, *vals);
		vals->~ElementR();
		vals++;
	}
	while (--size);
	return true;
}

bool Cmd_GetFormFlag_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &flag))
		*result = (form->flags & flag) ? 1 : 0;
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetFormFlag_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 flag, inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &flag, &inval))
	{
		if (inval) form->flags |= flag;
		else form->flags &= ~flag;
	}
	return true;
}

bool Cmd_GetIngestibleFlag_Execute(COMMAND_ARGS)
{
	AlchemyItem *alch;
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &alch, &flag) && IS_ID(alch, AlchemyItem))
		*result = (alch->alchFlags & flag) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetIngestibleFlag_Execute(COMMAND_ARGS)
{
	AlchemyItem *alch;
	UInt32 flag, inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &alch, &flag, &inval) && IS_ID(alch, AlchemyItem))
	{
		if (inval) alch->alchFlags |= flag;
		else alch->alchFlags &= ~flag;
	}
	return true;
}

bool Cmd_IsSkillMagazine_Execute(COMMAND_ARGS)
{
	*result = 0;
	AlchemyItem *ingestible;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &ingestible) || NOT_ID(ingestible, AlchemyItem)) return true;
	ListNode<EffectItem> *effIter = ingestible->magicItem.list.list.Head();
	EffectItem *effItem;
	ListNode<Condition> *condIter;
	Condition *condition;
	TESForm *param1;
	do
	{
		if (!(effItem = effIter->data)) continue;
		condIter = effItem->conditions.Head();
		do
		{
			if (!(condition = condIter->data)) continue;
			param1 = condition->parameter1.form;
			if ((condition->opcode == 0x1C1) && ((param1->refID == 0x31DE1) || (param1->refID == 0x31D6A)))	// HasPerk Comprehension || HasPerk Retention
			{
				*result = 1;
				return true;
			}
		}
		while (condIter = condIter->next);
	}
	while (effIter = effIter->next);
	return true;
}

bool __fastcall PlayerHasKey(TESObjectREFR *thisObj)
{
	if IS_ID(thisObj->baseForm, BGSTerminal)
	{
		BGSNote *password = ((BGSTerminal*)thisObj->baseForm)->password;
		if (password && g_thePlayer->notes.IsInList(password))
			return true;
	}
	else
	{
		ExtraLock *xLock = GetExtraType(&thisObj->extraDataList, Lock);
		TESKey *key = (xLock && xLock->data) ? xLock->data->key : NULL;
		if (key && g_thePlayer->GetItemCount(key))
			return true;
	}
	return false;
}

bool Cmd_PlayerHasKey_Execute(COMMAND_ARGS)
{
	*result = PlayerHasKey(thisObj);
	return true;
}

bool Cmd_PlayerHasKey_Eval(COMMAND_ARGS_EVAL)
{
	*result = PlayerHasKey(thisObj);
	return true;
}

bool Cmd_GetAnimObjectIdle_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectANIO *anio;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &anio) && IS_ID(anio, TESObjectANIO) && anio->idleForm)
		REFR_RES = anio->idleForm->refID;
	return true;
}

bool Cmd_SetAnimObjectIdle_Execute(COMMAND_ARGS)
{
	TESObjectANIO *anio;
	TESIdleForm *idle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &anio, &idle) && IS_ID(anio, TESObjectANIO) && IS_ID(idle, TESIdleForm))
		anio->idleForm = idle;
	return true;
}

bool Cmd_GetPickupSound_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		BGSPickupPutdownSounds *sounds = DYNAMIC_CAST(form, TESForm, BGSPickupPutdownSounds);
		if (sounds && sounds->pickupSound) REFR_RES = sounds->pickupSound->refID;
	}
	return true;
}

bool Cmd_SetPickupSound_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESSound *sound;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &sound) && IS_ID(sound, TESSound))
	{
		BGSPickupPutdownSounds *sounds = DYNAMIC_CAST(form, TESForm, BGSPickupPutdownSounds);
		if (sounds) sounds->pickupSound = sound;
	}
	return true;
}

bool Cmd_GetPutdownSound_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		BGSPickupPutdownSounds *sounds = DYNAMIC_CAST(form, TESForm, BGSPickupPutdownSounds);
		if (sounds && sounds->putdownSound) REFR_RES = sounds->putdownSound->refID;
	}
	return true;
}

bool Cmd_SetPutdownSound_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESSound *sound;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &sound) && IS_ID(sound, TESSound))
	{
		BGSPickupPutdownSounds *sounds = DYNAMIC_CAST(form, TESForm, BGSPickupPutdownSounds);
		if (sounds) sounds->putdownSound = sound;
	}
	return true;
}

bool Cmd_GetContainerOpenSound_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCONT *container = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &container)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if (IS_ID(container, TESObjectCONT) && container->openSound)
		REFR_RES = container->openSound->refID;
	return true;
}

bool Cmd_SetContainerOpenSound_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	TESObjectCONT *container = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &container) || NOT_ID(sound, TESSound)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if IS_ID(container, TESObjectCONT)
		container->openSound = sound;
	return true;
}

bool Cmd_GetContainerCloseSound_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCONT *container = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &container)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if (IS_ID(container, TESObjectCONT) && container->closeSound)
		REFR_RES = container->closeSound->refID;
	return true;
}

bool Cmd_SetContainerCloseSound_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	TESObjectCONT *container = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &container) || NOT_ID(sound, TESSound)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if IS_ID(container, TESObjectCONT)
		container->closeSound = sound;
	return true;
}

bool Cmd_GetPlayerRegions_Execute(COMMAND_ARGS)
{
	s_tempElements.Clear();
	ListNode<TESRegion> *iter = g_thePlayer->regionsList.list.Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetZoneFlag_Execute(COMMAND_ARGS)
{
	BGSEncounterZone *encZone;
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &encZone, &flag) && IS_ID(encZone, BGSEncounterZone))
		*result = (encZone->flags & flag) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetZoneFlag_Execute(COMMAND_ARGS)
{
	BGSEncounterZone *encZone;
	UInt32 flag, inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &encZone, &flag, &inval) && IS_ID(encZone, BGSEncounterZone))
	{
		if (inval) encZone->zoneFlags |= flag;
		else encZone->zoneFlags &= ~flag;
	}
	return true;
}

bool Cmd_GetImpactDataSet_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectWEAP *weapon;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon) && IS_ID(weapon, TESObjectWEAP) && weapon->impactDataSet)
		REFR_RES = weapon->impactDataSet->refID;
	return true;
}

bool Cmd_SetImpactDataSet_Execute(COMMAND_ARGS)
{
	TESObjectWEAP *weapon;
	BGSImpactDataSet *idSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &idSet) && IS_ID(weapon, TESObjectWEAP) && IS_ID(idSet, BGSImpactDataSet))
		weapon->impactDataSet = idSet;
	return true;
}

bool Cmd_LNIsPlayable_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	switch (form->typeID)
	{
	case kFormType_TESObjectARMO:
		*result = ((TESObjectARMO*)form)->bipedModel.IsPlayable() ? 1 : 0;
		break;
	case kFormType_TESObjectWEAP:
		*result = ((TESObjectWEAP*)form)->IsPlayable() ? 1 : 0;
		break;
	case kFormType_TESAmmo:
		*result = ((TESAmmo*)form)->IsPlayable() ? 1 : 0;
		break;
	case kFormType_BGSPerk:
		*result = ((BGSPerk*)form)->data.isPlayable ? 1 : 0;
		break;
	case kFormType_TESRace:
		*result = ((TESRace*)form)->IsPlayable() ? 1 : 0;
		break;
	case kFormType_TESEyes:
		*result = ((TESEyes*)form)->IsPlayable() ? 1 : 0;
		break;
	case kFormType_TESHair:
		*result = ((TESHair*)form)->IsPlayable() ? 1 : 0;
	}
	return true;
}

bool Cmd_LNSetIsPlayable_Execute(COMMAND_ARGS)
{
	UInt32 playable;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &playable, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	switch (form->typeID)
	{
	case kFormType_TESObjectARMO:
		((TESObjectARMO*)form)->bipedModel.SetPlayable(playable != 0);
		break;
	case kFormType_TESObjectWEAP:
		((TESObjectWEAP*)form)->SetPlayable(playable != 0);
		break;
	case kFormType_TESAmmo:
		((TESAmmo*)form)->SetPlayable(playable != 0);
		break;
	case kFormType_BGSPerk:
		((BGSPerk*)form)->data.isPlayable = (playable != 0);
		break;
	case kFormType_TESRace:
		((TESRace*)form)->SetPlayable(playable != 0);
		break;
	case kFormType_TESEyes:
		((TESEyes*)form)->SetPlayable(playable != 0);
		break;
	case kFormType_TESHair:
		((TESHair*)form)->SetPlayable(playable != 0);
	}
	return true;
}

bool Cmd_GetZone_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->parentCell)
	{
		ExtraEncounterZone *xEncZone = GetExtraType(&thisObj->parentCell->extraDataList, EncounterZone);
		if (xEncZone && xEncZone->zone) REFR_RES = xEncZone->zone->refID;
	}
	return true;
}

bool Cmd_AddNoteNS_Execute(COMMAND_ARGS)
{
	SafeWrite8((UInt32)QueueUIMessage, 0xC3);	// RETN
	AddNote(PASS_COMMAND_ARGS);
	SafeWrite8((UInt32)QueueUIMessage, 0x55);	// PUSH EBP
	return true;
}

UInt8 __fastcall CrosshairRefInList(BGSListForm *listForm)
{
	TESObjectREFR *refr = g_interfaceManager->crosshairRef;
	if (!refr) return 0;
	TESForm *base = IS_ID(refr->baseForm, BGSPlaceableWater) ? ((BGSPlaceableWater*)refr->baseForm)->water : refr->baseForm, *form;
	ListNode<TESForm> *iter = listForm->list.Head();
	do
	{
		form = iter->data;
		if (form == refr)
			return 1;
		if (form == base)
			return 2;
	}
	while (iter = iter->next);
	return 0;
}

bool Cmd_CrosshairRefInList_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm))
		*result = CrosshairRefInList(listForm);
	else *result = 0;
	return true;
}

bool Cmd_CrosshairRefInList_Eval(COMMAND_ARGS_EVAL)
{
	*result = CrosshairRefInList((BGSListForm*)arg1);
	return true;
}

bool Cmd_GetShowQuestItems_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->showQuestItems;
	return true;
}

bool Cmd_GetWaterTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWaterForm *water;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &water, &traitID) || NOT_ID(water, TESWaterForm) || (traitID > 50)) return true;
	switch (traitID)
	{
	case 0:
		*result = water->opacity;
		break;
	case 1:
		*result = water->flags;
		break;
	case 12:
	case 13:
	case 14:
		*result = RGBHexToDec(*(UInt32*)&water->visData[traitID - 2]);
		break;
	default:
		*result = water->visData[traitID - 2];
	}
	return true;
}

bool Cmd_SetWaterTrait_Execute(COMMAND_ARGS)
{
	TESWaterForm *water;
	UInt32 traitID;
	double value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &water, &traitID, &value) || NOT_ID(water, TESWaterForm) || (traitID > 50)) return true;
	UInt32 intVal = (int)value;
	switch (traitID)
	{
	case 0:
		if (intVal <= 100) water->opacity = intVal;
		break;
	case 1:
		if (intVal <= 3) water->flags = intVal;
		break;
	case 12:
	case 13:
	case 14:
		if (intVal <= 255255255) *(UInt32*)&water->visData[traitID - 2] = RGBDecToHex(intVal);
		break;
	default:
		water->visData[traitID - 2] = value;
	}
	return true;
}

bool Cmd_GetActiveQuest_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (g_thePlayer->activeQuest) REFR_RES = g_thePlayer->activeQuest->refID;
	return true;
}