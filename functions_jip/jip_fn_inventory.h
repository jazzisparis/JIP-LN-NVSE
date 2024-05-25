#pragma once

DEFINE_COMMAND_PLUGIN(AddItemAlt, 1, kParams_OneItemOrList_OneOptInt_OneOptFloat_TwoOptInts);
DEFINE_COMMAND_PLUGIN(GetValueAlt, 0, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetValueAlt, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveItemTarget, 1, kParams_OneItemOrList_OneContainer_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetWeaponRefModFlags, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetWeaponRefModFlags, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetItemRefCurrentHealth, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetItemRefCurrentHealth, 1, kParams_OneFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetHotkeyItemRef, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(EquipItemAlt, 1, kParams_OneObjectID_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(UnequipItemAlt, 1, kParams_OneObjectID_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(DropAlt, 1, kParams_OneItemOrList_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(DropMeAlt, 1, kParams_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetAllItems, 1, kParams_FourOptionalInts_OneOptionalList);
DEFINE_COMMAND_PLUGIN(GetAllItemRefs, 1, kParams_FourOptionalInts_OneOptionalList);
DEFINE_COMMAND_PLUGIN(RemoveMeIRAlt, 1, kParams_TwoOptionalInts_OneOptionalContainer);
DEFINE_COMMAND_PLUGIN(GetEquippedItemRef, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetNoUnequip, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetNoUnequip, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEquippedWeaponPoison, 1, nullptr);
DEFINE_COMMAND_PLUGIN(ToggleItemUnique, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetBaseItems, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetOnUseAidItemEventHandler, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetEquippedArmorRefs, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetArmorEffectiveDT, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetArmorEffectiveDR, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHotkeyItemRef, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(IsItemUnique, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(GetSelfAsInventoryRef, 1, nullptr);

bool Cmd_AddItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 count = 1, doEquip = 0, noMessage = 1;
	float condition = 100.0F;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &condition, &doEquip, &noMessage) && count && thisObj->baseForm->GetContainer())
		thisObj->AddItemAlt(form, count, condition * 0.01F, doEquip, noMessage);
	return true;
}

bool Cmd_GetValueAlt_Execute(COMMAND_ARGS)
{
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (form || (thisObj && (form = thisObj->baseForm) && kInventoryType[form->typeID])))
		*result = (int)form->GetItemValue();
	return true;
}

bool Cmd_SetValueAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 newVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &newVal))
		if (TESValueForm *valForm = DYNAMIC_CAST(form, TESForm, TESValueForm))
		{
			valForm->value = newVal;
			//form->MarkModified(2);
		}
		else if IS_ID(form, AlchemyItem)
			((AlchemyItem*)form)->value = newVal;
	return true;
}

bool Cmd_RemoveItemTarget_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESObjectREFR *target;
	SInt32 quantity = 0, clrOwner = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &target, &quantity, &clrOwner) && thisObj->baseForm->GetContainer())
		thisObj->RemoveItemTarget(form, target, quantity, !clrOwner);
	return true;
}

bool Cmd_GetWeaponRefModFlags_Execute(COMMAND_ARGS)
{
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (ExtraDataList *xData = invRef ? invRef->xData : &thisObj->extraDataList)
		if (auto xModFlags = GetExtraType(xData, ExtraWeaponModFlags))
			*result = xModFlags->flags;
	return true;
}

bool Cmd_SetWeaponRefModFlags_Execute(COMMAND_ARGS)
{
	UInt32 flags;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &flags) || (flags > 7))
		return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (!invRef || NOT_ID(invRef->type, TESObjectWEAP))
		return true;
	TESObjectIMOD **mods = ((TESObjectWEAP*)invRef->type)->itemMod;
	if (!mods[0]) flags &= ~1;
	if (!mods[1]) flags &= ~2;
	if (!mods[2]) flags &= ~4;
	if (ExtraDataList *xData = invRef->xData)
	{
		if (auto xModFlags = GetExtraType(xData, ExtraWeaponModFlags))
		{
			if (xModFlags->flags == flags)
				return true;
			if (flags) xModFlags->flags = flags;
			else
			{
				xData->RemoveByType(kXData_ExtraWeaponModFlags);
				if (!xData->m_data)
				{
					auto entry = invRef->containerRef->GetContainerChangesEntry(invRef->type);
					if (!entry || !entry->extendData) return true;
					entry->extendData->Remove(xData);
					Game_HeapFree(xData);
				}
			}
		}
		else if (flags)
		{
			if (invRef->GetCount() > 1)
				xData = invRef->SplitFromStack();
			xData->AddExtra(ExtraWeaponModFlags::Create(flags));
		}
	}
	else if (flags)
	{
		xData = invRef->CreateExtraData();
		if (!xData) return true;
		xData->AddExtra(ExtraWeaponModFlags::Create(flags));
	}
	*result = 1;
	return true;
}

bool Cmd_GetItemRefCurrentHealth_Execute(COMMAND_ARGS)
{
	UInt32 percOrBase = 0;
	if (NUM_ARGS_EX)
		ExtractArgsEx(EXTRACT_ARGS_EX, &percOrBase);
	ExtraDataList *xData;
	float baseHealth;
	if (InventoryRef *invRef = InventoryRefGetForID(thisObj->refID))
	{
		xData = invRef->xData;
		baseHealth = invRef->entry->GetBaseHealth();
	}
	else
	{
		xData = &thisObj->extraDataList;
		ContChangesExtraList extendData(xData);
		ContChangesEntry entry(&extendData, 1, thisObj->baseForm);
		baseHealth = entry.GetBaseHealth();
	}
	if (percOrBase != 2)
	{
		ExtraHealth *xHealth;
		if (xData && (xHealth = GetExtraType(xData, ExtraHealth)))
			*result = !percOrBase ? xHealth->health : (xHealth->health / baseHealth);
		else
			*result = !percOrBase ? baseHealth : 1.0;
	}
	else *result = baseHealth;
	return true;
}

bool Cmd_SetItemRefCurrentHealth_Execute(COMMAND_ARGS)
{
	float health;
	UInt32 setPercent = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &health, &setPercent))
		return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (!invRef) return true;
	float baseHealth = invRef->entry->GetBaseHealth();
	if (baseHealth == 0) return true;
	if (setPercent)
		health *= baseHealth;
	if (health > baseHealth)
		health = baseHealth;
	if (ExtraDataList *xData = invRef->xData)
		if (auto xHealth = GetExtraType(xData, ExtraHealth))
			xHealth->health = health;
		else
			xData->AddExtra(ExtraHealth::Create(health));
	else if (xData = invRef->CreateExtraData())
		xData->AddExtra(ExtraHealth::Create(health));
	else return true;
	*result = 1;
	return true;
}

bool Cmd_SetHotkeyItemRef_Execute(COMMAND_ARGS)
{
	UInt32 keyNum;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &keyNum) || !keyNum || (keyNum > 8))
		return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (!invRef || (invRef->containerRef != g_thePlayer))
		return true;
	UInt8 type = invRef->type->typeID;
	if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem))
		return true;
	if (!keyNum)
	{
		if (invRef->xData) invRef->xData->RemoveByType(kXData_ExtraHotkey);
		return true;
	}
	keyNum--;
	if (ExtraDataList *xData = invRef->xData)
	{
		auto xHotkey = GetExtraType(xData, ExtraHotkey);
		if (!xHotkey)
		{
			if (!ClearHotkey(keyNum))
				return true;
			xData->AddExtra(ExtraHotkey::Create(keyNum));
		}
		else if ((xHotkey->index == keyNum) || !ClearHotkey(keyNum))
			return true;
		else xHotkey->index = keyNum;
	}
	else
	{
		if (!ClearHotkey(keyNum) || !(xData = invRef->CreateExtraData()))
			return true;
		xData->AddExtra(ExtraHotkey::Create(keyNum));
	}
	*result = 1;
	return true;
}

bool Cmd_EquipItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *item;
	UInt32 noUnequip = 0, noMessage = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &item, &noUnequip, &noMessage) && IS_ACTOR(thisObj))
		if (ContChangesEntry *entry = thisObj->GetContainerChangesEntry(item))
			((Actor*)thisObj)->EquipItemAlt(entry, noUnequip, noMessage);
	return true;
}

bool Cmd_UnequipItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *item;
	UInt32 noEquip = 0, noMessage = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &item, &noEquip, &noMessage) && IS_ACTOR(thisObj) && (IS_ID(item, TESObjectWEAP) || IS_TYPE(item, TESObjectARMO)))
		if (ContChangesEntry *entry = thisObj->GetContainerChangesEntry(item))
			if (ExtraDataList *xData = entry->GetEquippedExtra())
			{
				((Actor*)thisObj)->UnequipItem(item, 1, xData, 1, noEquip, noMessage);
				if (!noMessage)
					ShowItemMessage(item, *(const char**)0x11D4250);
			}
	return true;
}

bool Cmd_DropAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	SInt32 dropCount = 0, clrOwner = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &dropCount, &clrOwner))
		return true;
	TESContainer *container = thisObj->baseForm->GetContainer();
	if (!container) return true;
	ContChangesEntryList *entryList = thisObj->GetContainerChangesList();
	if (!entryList) return true;
	tList<TESForm> tempList(form);
	if IS_ID(form, BGSListForm)
		tempList = ((BGSListForm*)form)->list;
	auto iter = tempList.Head();
	TESForm *item;
	SInt32 total, subCount;
	bool keepOwner = !clrOwner, hasScript, stacked;
	ContChangesEntry *entry;
	ExtraDataList *xData;
	do
	{
		item = iter->data;
		if (!item || !kInventoryType[item->typeID])
			continue;
		total = GetFormCount(&container->formCountList, entryList, item);
		if (total < 1) continue;
		if ((dropCount > 0) && (dropCount < total))
			total = dropCount;
		if ((entry = entryList->FindForItem(item)) && entry->extendData)
		{
			hasScript = item->HasScript();
			stacked = IS_ID(item, TESObjectWEAP) ? (((TESObjectWEAP*)item)->eWeaponType > 9) : NOT_TYPE(item, TESObjectARMO);
			while ((total > 0) && entry->extendData && (xData = entry->extendData->GetFirstItem()))
			{
				subCount = xData->GetCount();
				if (subCount < 1)
					subCount = 1;
				else if (subCount > 1)
				{
					if (hasScript && xData->HasType(kXData_ExtraScript))
					{
						xData->RemoveByType(kXData_ExtraCount);
						subCount = 1;
					}
					else if (subCount > total)
						subCount = total;
				}
				total -= subCount;
				if (stacked)
					thisObj->RemoveItem(item, xData, subCount, keepOwner, 1, nullptr, 0, 0, 1, 0);
				else while (subCount-- > 0)
					thisObj->RemoveItem(item, xData, 1, keepOwner, 1, nullptr, 0, 0, 1, 0);
			}
		}
		while (total > 0)
		{
			subCount = (total < 0x7FFF) ? total : 0x7FFF;
			thisObj->RemoveItem(item, nullptr, subCount, keepOwner, 1, nullptr, 0, 0, 1, 0);
			total -= subCount;
		}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_DropMeAlt_Execute(COMMAND_ARGS)
{
	SInt32 dropCount = 0, clrOwner = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &dropCount, &clrOwner))
		return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (!invRef) return true;
	ExtraDataList *xData = invRef->xData;
	SInt32 countExtra = invRef->GetCount();
	if (countExtra < 1)
		return true;
	if ((dropCount < 1) || (dropCount > countExtra))
		dropCount = countExtra;
	TESForm *item = invRef->type;
	bool stacked = IS_ID(item, TESObjectWEAP) ? (((TESObjectWEAP*)item)->eWeaponType > 9) : NOT_TYPE(item, TESObjectARMO);
	if (stacked) invRef->containerRef->RemoveItem(item, xData, dropCount, !clrOwner, 1, nullptr, 0, 0, 1, 0);
	else while (dropCount-- > 0) invRef->containerRef->RemoveItem(item, xData, 1, !clrOwner, 1, nullptr, 0, 0, 1, 0);
	invRef->removed = true;
	return true;
}

bool Cmd_GetAllItems_Execute(COMMAND_ARGS)
{
	UInt32 typeID = 0, noNonPlayable = 0, noQuestItem = 0, noEquipped = 0;
	BGSListForm *listForm = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &typeID, &noNonPlayable, &noQuestItem, &noEquipped, &listForm))
		return true;
	if (typeID && !kInventoryType[typeID])
		return true;
	InventoryItemsMap *invItemsMap = GetInventoryItemsMap();
	if (!GetInventoryItems(thisObj, typeID, invItemsMap))
		return true;
	if (NOT_ACTOR(thisObj)) noEquipped = 0;
	TESForm *item;
	TempElements *tmpElements;
	if (listForm) listForm->RemoveAll();
	else tmpElements = GetTempElements();
	int count = 0;
	for (auto itemIter = invItemsMap->Begin(); itemIter; ++itemIter)
	{
		item = itemIter.Key();
		if ((noNonPlayable && !item->IsItemPlayable()) || (noQuestItem && (item->flags & 0x400)) || (noEquipped && ((Actor*)thisObj)->IsItemEquipped(item)))
			continue;
		if (listForm) listForm->list.Prepend(item);
		else tmpElements->Append(item);
		count++;
	}
	if (count)
	{
		if (listForm) *result = count;
		else *result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_GetAllItemRefs_Execute(COMMAND_ARGS)
{
	UInt32 typeID = 0, noNonPlayable = 0, noQuestItem = 0, noEquipped = 0;
	BGSListForm *listForm = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &typeID, &noNonPlayable, &noQuestItem, &noEquipped, &listForm))
		return true;
	if (typeID && !kInventoryType[typeID])
		return true;
	InventoryItemsMap *invItemsMap = GetInventoryItemsMap();
	if (!GetInventoryItems(thisObj, typeID, invItemsMap))
		return true;
	if (NOT_ACTOR(thisObj)) noEquipped = 0;
	TESForm *item;
	SInt32 baseCount, xCount;
	ContChangesEntry *entry;
	tList<ExtraDataList>::Node *xdlIter;
	ExtraDataList *xData;
	TESObjectREFR *invRef;
	TempElements *tmpElements;
	if (listForm) listForm->RemoveAll();
	else tmpElements = GetTempElements();
	int count = 0;
	for (auto dataIter = invItemsMap->Begin(); dataIter; ++dataIter)
	{
		item = dataIter.Key();
		if ((noNonPlayable && !item->IsItemPlayable()) || (noQuestItem && (item->flags & 0x400)))
			continue;
		baseCount = dataIter().count;
		entry = dataIter().entry;
		if (entry && entry->extendData)
		{
			xdlIter = entry->extendData->Head();
			do
			{
				if (!(xData = xdlIter->data))
					continue;
				xCount = xData->GetCount();
				if (xCount < 1) continue;
				if (xCount > baseCount)
					xCount = baseCount;
				baseCount -= xCount;
				if (noEquipped && xData->HasType(kXData_ExtraWorn))
					continue;
				invRef = InventoryRefCreate(thisObj, item, xCount, xData);
				if (listForm) listForm->list.Prepend(invRef);
				else tmpElements->Append(invRef);
				count++;
			}
			while (baseCount && (xdlIter = xdlIter->next));
		}
		if (baseCount > 0)
		{
			invRef = InventoryRefCreate(thisObj, item, baseCount, nullptr);
			if (listForm) listForm->list.Prepend(invRef);
			else tmpElements->Append(invRef);
			count++;
		}
	}
	if (count)
	{
		if (listForm) *result = count;
		else *result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_RemoveMeIRAlt_Execute(COMMAND_ARGS)
{
	SInt32 quantity = 0, clrOwner = 0;
	TESObjectREFR *destRef = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &quantity, &clrOwner, &destRef))
		return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (!invRef) return true;
	SInt32 countExtra = invRef->GetCount();
	if (countExtra < 1)
		return true;
	if ((quantity < 1) || (quantity > countExtra))
		quantity = countExtra;
	invRef->containerRef->RemoveItem(invRef->type, invRef->xData, quantity, !clrOwner, 0, destRef, 0, 0, 1, 0);
	invRef->removed = true;
	return true;
}

bool Cmd_GetEquippedItemRef_Execute(COMMAND_ARGS)
{
	UInt32 slotIdx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &slotIdx) && (slotIdx <= 19) && IS_ACTOR(thisObj))
		if (TESObjectREFR *invRef = GetEquippedItemRef((Actor*)thisObj, slotIdx))
			REFR_RES = invRef->refID;
	return true;
}

bool Cmd_GetNoUnequip_Execute(COMMAND_ARGS)
{
	if (InventoryRef *invRef = InventoryRefGetForID(thisObj->refID); invRef && invRef->xData && invRef->xData->HasType(kXData_ExtraCannotWear))
		*result = 1;
	return true;
}

bool Cmd_SetNoUnequip_Execute(COMMAND_ARGS)
{
	UInt32 noUnequip;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &noUnequip))
	{
		InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
		ExtraDataList *xData = invRef ? invRef->xData : nullptr;
		if (xData)
		{
			if (!noUnequip)
				xData->RemoveByType(kXData_ExtraCannotWear);
			else if (xData->HasType(kXData_ExtraWorn) && !xData->HasType(kXData_ExtraCannotWear))
				xData->AddExtra(ExtraCannotWear::Create());
		}
	}
	return true;
}

bool Cmd_GetEquippedWeaponPoison_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		if (ContChangesEntry *wpnInfo = ((Actor*)thisObj)->GetWeaponInfo(); wpnInfo && wpnInfo->extendData)
			if (ExtraDataList *xDataList = wpnInfo->extendData->GetFirstItem())
				if (ExtraPoison *xPoison = GetExtraType(xDataList, ExtraPoison); xPoison && xPoison->poisonEffect)
					REFR_RES = xPoison->poisonEffect->refID;
	return true;
}

bool Cmd_ToggleItemUnique_Execute(COMMAND_ARGS)
{
	TESForm *itemOrList;
	UInt32 toggle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &itemOrList, &toggle))
	{
		tList<TESForm> tempList(itemOrList);
		if IS_ID(itemOrList, BGSListForm)
			tempList = ((BGSListForm*)itemOrList)->list;
		else if (NOT_ID(itemOrList, TESObjectWEAP) && NOT_TYPE(itemOrList, TESObjectARMO))
			return true;
		auto traverse = tempList.Head();
		bool bToggle = toggle != 0;
		TESForm *item;
		do
		{
			if (!(item = traverse->data) || (NOT_TYPE(item, TESObjectARMO) && NOT_ID(item, TESObjectWEAP)) || (bToggle == ((item->jipFormFlags6 & kHookFormFlag6_UniqueItem) != 0)))
				continue;
			item->SetJIPFlag(kHookFormFlag6_UniqueItem, bToggle);
			HOOK_MOD(CheckUniqueItem, bToggle);
		}
		while (traverse = traverse->next);
	}
	return true;
}

bool Cmd_GetBaseItems_Execute(COMMAND_ARGS)
{
	TESForm *baseForm = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &baseForm))
		return true;
	if (!baseForm)
	{
		if (!thisObj) return true;
		baseForm = thisObj->baseForm;
	}
	if (TESContainer *container = baseForm->GetContainer())
	{
		TempElements *tmpElements = GetTempElements();
		auto traverse = container->formCountList.Head();
		do
		{
			if (TESContainer::FormCount *formCount = traverse->data)
				tmpElements->Append(formCount->form);
		}
		while (traverse = traverse->next);
		if (!tmpElements->Empty())
			*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_SetOnUseAidItemEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	TESForm *itemOrList;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &itemOrList) || NOT_ID(script, Script))
		return true;
	tList<TESForm> tempList(itemOrList);
	if IS_ID(itemOrList, BGSListForm)
		tempList = ((BGSListForm*)itemOrList)->list;
	auto iter = tempList.Head();
	do
	{
		if (AlchemyItem *alchItem = (AlchemyItem*)iter->data; alchItem && IS_ID(alchItem, AlchemyItem))
			if (addEvnt)
			{
				EventCallbackScripts *callbacks;
				if (s_useAidItemEventMap->Insert(alchItem, &callbacks))
					HOOK_INC(EquipAidItem);
				callbacks->Insert(script);
				alchItem->SetJIPFlag(kHookFormFlag6_OnEquipHandlers, true);
			}
			else if (auto findItem = s_useAidItemEventMap->Find(alchItem); findItem && findItem().Erase(script) && findItem().Empty())
			{
				findItem.Remove();
				HOOK_DEC(EquipAidItem);
				alchItem->SetJIPFlag(kHookFormFlag6_OnEquipHandlers, false);
			}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_GetEquippedArmorRefs_Execute(COMMAND_ARGS)
{
	if (IS_ACTOR(thisObj))
		if (BipedAnim *equipment = ((Actor*)thisObj)->GetBipedAnim())
			if (ContChangesEntryList *entryList = thisObj->GetContainerChangesList())
			{
				TempElements *tmpElements = GetTempElements();
				BipedAnim::Data *slotData = equipment->slotData;
				TESForm *item;
				ContChangesEntry *entry;
				ExtraDataList *xData;
				TESObjectREFR *invRef;
				for (UInt32 count = 20; count; count--, slotData++)
				{
					item = slotData->item;
					if (!item || NOT_TYPE(item, TESObjectARMO) || !(entry = entryList->FindForItem(item)) || !(xData = entry->GetEquippedExtra()))
						continue;
					invRef = InventoryRefCreate(thisObj, item, entry->countDelta, xData);
					if (invRef) tmpElements->Append(invRef);
				}
				*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
			}
	return true;
}

double __fastcall GetArmorEffectiveDX(TESObjectREFR *thisObj, UInt32 funcAddr)
{
	if NOT_TYPE(thisObj->baseForm, TESObjectARMO)
		return 0;
	if (InventoryRef *invRef = InventoryRefGetForID(thisObj->refID))
		return ThisCall<double>(funcAddr, invRef->entry, 0);
	ContChangesExtraList extendData(&thisObj->extraDataList);
	ContChangesEntry tempEntry(&extendData, 1, thisObj->baseForm);
	return ThisCall<double>(funcAddr, &tempEntry, 0);
}

bool Cmd_GetArmorEffectiveDT_Execute(COMMAND_ARGS)
{
	*result = GetArmorEffectiveDX(thisObj, 0x4BE0B0);
	return true;
}

bool Cmd_GetArmorEffectiveDR_Execute(COMMAND_ARGS)
{
	*result = GetArmorEffectiveDX(thisObj, 0x4BDF90);
	return true;
}

bool Cmd_GetHotkeyItemRef_Execute(COMMAND_ARGS)
{
	UInt32 keyNum;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &keyNum) && keyNum && (keyNum <= 8))
	{
		ExtraDataList *xData;
		ContChangesEntry *entry = GetHotkeyItemEntry(keyNum - 1, &xData);
		if (entry)
		{
			TESObjectREFR *invRef = InventoryRefCreate(g_thePlayer, entry->type, xData->GetCount(), xData);
			REFR_RES = invRef->refID;
		}
	}
	return true;
}

bool Cmd_IsItemUnique_Execute(COMMAND_ARGS)
{
	TESForm *item;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &item) && (item->jipFormFlags6 & kHookFormFlag6_UniqueItem))
		*result = 1;
	return true;
}

bool Cmd_GetSelfAsInventoryRef_Execute(COMMAND_ARGS)
{
	if (containingObj && thisObj->baseForm)
		if (TESObjectREFR *invRef = containingObj->CreateInventoryRefForScriptedObj(thisObj->baseForm, eventList))
			REFR_RES = invRef->refID;
	return true;
}