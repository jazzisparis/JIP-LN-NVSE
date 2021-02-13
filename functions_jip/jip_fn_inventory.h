#pragma once

DEFINE_COMMAND_PLUGIN(AddItemAlt, , 1, 4, kParams_JIP_OneItemOrList_OneInt_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetValueAlt, , 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetValueAlt, , 0, 2, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(RemoveItemTarget, , 1, 4, kParams_JIP_OneItemOrList_OneContainer_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetWeaponRefModFlags, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetWeaponRefModFlags, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetItemRefCurrentHealth, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetItemRefCurrentHealth, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(SetHotkeyItemRef, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(EquipItemAlt, , 1, 3, kParams_EquipItem);
DEFINE_COMMAND_PLUGIN(UnequipItemAlt, , 1, 3, kParams_EquipItem);
DEFINE_COMMAND_PLUGIN(DropAlt, , 1, 3, kParams_JIP_OneItemOrList_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(DropMeAlt, , 1, 2, kParams_JIP_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetAllItems, , 1, 5, kParams_JIP_FourOptionalInts_OneOptionalList);
DEFINE_COMMAND_PLUGIN(GetAllItemRefs, , 1, 5, kParams_JIP_FourOptionalInts_OneOptionalList);
DEFINE_COMMAND_PLUGIN(RemoveMeIRAlt, , 1, 3, kParams_JIP_TwoOptionalInts_OneOptionalContainer);
DEFINE_COMMAND_PLUGIN(GetEquippedItemRef, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetNoUnequip, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetNoUnequip, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEquippedWeaponPoison, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(ToggleItemUnique, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetBaseItems, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetOnUseAidItemEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetEquippedArmorRefs, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetArmorEffectiveDT, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetArmorEffectiveDR, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetHotkeyItemRef, , 0, 1, kParams_OneInt);

bool Cmd_AddItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 count, doEquip = 0;
	float condition = 100;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &condition, &doEquip) && count && thisObj->GetContainer())
	{
		condition = GetMin(GetMax(condition, 0.0F), 100.0F) / 100;
		thisObj->AddItemAlt(form, count, condition, doEquip != 0);
	}
	return true;
}

bool Cmd_GetValueAlt_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (form || (thisObj && (form = thisObj->baseForm) && kInventoryType[form->typeID])))
		*result = (int)form->GetItemValue();
	else *result = 0;
	return true;
}

bool Cmd_SetValueAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 newVal;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &newVal))
		return true;
	TESValueForm *valForm = DYNAMIC_CAST(form, TESForm, TESValueForm);
	if (valForm) valForm->value = newVal;
	else if IS_ID(form, AlchemyItem)
		((AlchemyItem*)form)->value = newVal;
	return true;
}

bool Cmd_RemoveItemTarget_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESObjectREFR *target;
	SInt32 quantity = 0, clrOwner = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &target, &quantity, &clrOwner) && thisObj->GetContainer())
		thisObj->RemoveItemTarget(form, target, quantity, !clrOwner);
	return true;
}

bool Cmd_GetWeaponRefModFlags_Execute(COMMAND_ARGS)
{
	*result = 0;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	ExtraDataList *xData = invRef ? invRef->xData : &thisObj->extraDataList;
	if (xData)
	{
		ExtraWeaponModFlags *xModFlags = GetExtraType(xData, WeaponModFlags);
		if (xModFlags) *result = xModFlags->flags;
	}
	return true;
}

bool Cmd_SetWeaponRefModFlags_Execute(COMMAND_ARGS)
{
	*result = 0;
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
	ExtraDataList *xData = invRef->xData;
	if (xData)
	{
		ContChangesEntry *entry = invRef->containerRef->GetContainerChangesEntry(invRef->type);
		if (!entry || !entry->extendData) return true;

		ExtraWeaponModFlags *xModFlags = GetExtraType(xData, WeaponModFlags);
		if (xModFlags)
		{
			if (xModFlags->flags == flags)
				return true;
			if (flags) xModFlags->flags = flags;
			else
			{
				RemoveExtraData(xData, xModFlags, true);
				if (!xData->m_data)
				{
					entry->extendData->Remove(xData);
					GameHeapFree(xData);
				}
			}
		}
		else if (flags)
		{
			xData = SplitFromStack(entry, xData);
			AddExtraData(xData, ExtraWeaponModFlags::Create(flags));
		}
	}
	else if (flags)
	{
		xData = invRef->CreateExtraData();
		if (!xData) return true;
		AddExtraData(xData, ExtraWeaponModFlags::Create(flags));
	}
	*result = 1;
	return true;
}

bool Cmd_GetItemRefCurrentHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	TESForm *item = invRef ? invRef->type : thisObj->baseForm;
	if (!item) return true;
	TESHealthForm *healthForm = DYNAMIC_CAST(item, TESForm, TESHealthForm);
	if (!healthForm) return true;
	ExtraDataList *xData = invRef ? invRef->xData : &thisObj->extraDataList;
	if (xData)
	{
		ExtraHealth *xHealth = GetExtraType(xData, Health);
		if (xHealth)
		{
			*result = xHealth->health;
			return true;
		}
	}
	*result = (int)healthForm->health;
	return true;
}

bool Cmd_SetItemRefCurrentHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	float health;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &health)) return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (!invRef) return true;
	TESHealthForm *healthForm = DYNAMIC_CAST(invRef->type, TESForm, TESHealthForm);
	if (!healthForm) return true;
	float maxHealth = (int)healthForm->health;
	ExtraDataList *xData = invRef->xData;
	if (xData)
	{
		maxHealth += GetModBonuses(invRef, 10);
		if (health > maxHealth) health = maxHealth;
		ExtraHealth *xHealth = GetExtraType(xData, Health);
		if (xHealth) xHealth->health = health;
		else AddExtraData(xData, ExtraHealth::Create(health));
	}
	else
	{
		xData = invRef->CreateExtraData();
		if (!xData) return true;
		AddExtraData(xData, ExtraHealth::Create(GetMin(health, maxHealth)));
	}
	*result = 1;
	return true;
}

bool Cmd_SetHotkeyItemRef_Execute(COMMAND_ARGS)
{
	*result = 0;
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
		if (invRef->xData) RemoveExtraType(invRef->xData, kExtraData_Hotkey);
		return true;
	}
	keyNum--;
	ExtraDataList *xData = invRef->xData;
	if (xData)
	{
		ExtraHotkey *xHotkey = GetExtraType(xData, Hotkey);
		if (!xHotkey)
		{
			if (!ClearHotkey(keyNum))
				return true;
			AddExtraData(xData, ExtraHotkey::Create(keyNum));
		}
		else if ((xHotkey->index == keyNum) || !ClearHotkey(keyNum))
			return true;
		else xHotkey->index = keyNum;
	}
	else
	{
		if (!ClearHotkey(keyNum) || !(xData = invRef->CreateExtraData()))
			return true;
		AddExtraData(xData, ExtraHotkey::Create(keyNum));
	}
	*result = 1;
	return true;
}

bool Cmd_EquipItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *item;
	UInt32 noUnequip = 0, noMessage = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &item, &noUnequip, &noMessage) && thisObj->IsActor())
	{
		ExtraContainerChanges::EntryDataList *entryList = thisObj->GetContainerChangesList();
		if (entryList)
			((Actor*)thisObj)->EquipItemAlt(item, entryList->FindForItem(item), noUnequip != 0, noMessage != 0);
	}
	return true;
}

bool Cmd_UnequipItemAlt_Execute(COMMAND_ARGS)
{
	TESForm *item;
	UInt32 noEquip = 0, noMessage = 1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &item, &noEquip, &noMessage) || !thisObj->IsActor() || 
		(NOT_ID(item, TESObjectWEAP) && NOT_TYPE(item, TESObjectARMO)))
		return true;
	ContChangesEntry *entry = thisObj->GetContainerChangesEntry(item);
	if (entry && entry->extendData)
	{
		Actor *actor = (Actor*)thisObj;
		ListNode<ExtraDataList> *xdlIter = entry->extendData->Head();
		ExtraDataList *xData;
		do
		{
			xData = xdlIter->data;
			if (xData && xData->HasType(kExtraData_Worn))
				actor->UnequipItem(item, 1, xData, 1, noEquip != 0, noMessage != 0);
		}
		while (xdlIter = xdlIter->next);
	}
	return true;
}

bool Cmd_DropAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	SInt32 dropCount = 0, clrOwner = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &dropCount, &clrOwner) || !thisObj->GetContainer())
		return true;
	ExtraContainerChanges::EntryDataList *entryList = thisObj->GetContainerChangesList();
	if (!entryList) return true;
	ListNode<TESForm> *iter;
	if IS_ID(form, BGSListForm)
		iter = ((BGSListForm*)form)->list.Head();
	else
	{
		ListNode<TESForm> tempList(form);
		iter = &tempList;
	}
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
		total = thisObj->GetItemCount(item);
		if (total < 1) continue;
		if ((dropCount > 0) && (dropCount < total))
			total = dropCount;
		if ((entry = entryList->FindForItem(item)) && entry->extendData)
		{
			hasScript = item->HasScript();
			stacked = IS_ID(item, TESObjectWEAP) ? (((TESObjectWEAP*)item)->eWeaponType > 9) : NOT_TYPE(item, TESObjectARMO);
			while ((total > 0) && (xData = entry->extendData->GetFirstItem()))
			{
				subCount = xData->GetCount();
				if (subCount < 1)
					continue;
				if (subCount > 1)
				{
					if (hasScript && xData->HasType(kExtraData_Script))
					{
						RemoveExtraType(xData, kExtraData_Count);
						subCount = 1;
					}
					else if (subCount > total)
						subCount = total;
				}
				total -= subCount;
				if (stacked)
					thisObj->RemoveItem(item, xData, subCount, keepOwner, 1, NULL, 0, 0, 1, 0);
				else while (subCount-- > 0)
					thisObj->RemoveItem(item, xData, 1, keepOwner, 1, NULL, 0, 0, 1, 0);
			}
		}
		while (total > 0)
		{
			subCount = (total < 0x7FFF) ? total : 0x7FFF;
			thisObj->RemoveItem(item, NULL, subCount, keepOwner, 1, NULL, 0, 0, 1, 0);
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
	if (stacked) invRef->containerRef->RemoveItem(item, xData, dropCount, !clrOwner, 1, NULL, 0, 0, 1, 0);
	else while (dropCount-- > 0) invRef->containerRef->RemoveItem(item, xData, 1, !clrOwner, 1, NULL, 0, 0, 1, 0);
	invRef->removed = true;
	return true;
}

bool Cmd_GetAllItems_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 typeID = 0, noNonPlayable = 0, noQuestItem = 0, noEquipped = 0;
	BGSListForm *listForm = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &typeID, &noNonPlayable, &noQuestItem, &noEquipped, &listForm))
		return true;
	if ((typeID && !kInventoryType[typeID]) || !thisObj->GetInventoryItems(typeID))
		return true;
	if (!thisObj->IsActor()) noEquipped = 0;
	TESForm *item;
	if (listForm) listForm->RemoveAll();
	else s_tempElements.Clear();
	int count = 0;
	for (auto itemIter = s_inventoryItemsMap.Begin(); itemIter; ++itemIter)
	{
		item = itemIter.Key();
		if ((noNonPlayable && !item->IsItemPlayable()) || (noQuestItem && item->IsQuestItem()) || (noEquipped && ((Actor*)thisObj)->IsItemEquipped(item)))
			continue;
		if (listForm) listForm->list.Prepend(item);
		else s_tempElements.Append(item);
		count++;
	}
	if (count)
	{
		if (listForm) *result = count;
		else AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_GetAllItemRefs_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 typeID = 0, noNonPlayable = 0, noQuestItem = 0, noEquipped = 0;
	BGSListForm *listForm = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &typeID, &noNonPlayable, &noQuestItem, &noEquipped, &listForm))
		return true;
	if ((typeID && !kInventoryType[typeID]) || !thisObj->GetInventoryItems(typeID))
		return true;
	if (!thisObj->IsActor()) noEquipped = 0;
	TESForm *item;
	SInt32 baseCount, xCount;
	ContChangesEntry *entry;
	ListNode<ExtraDataList> *xdlIter;
	ExtraDataList *xData;
	TESObjectREFR *invRef;
	if (listForm) listForm->RemoveAll();
	else s_tempElements.Clear();
	int count = 0;
	for (auto dataIter = s_inventoryItemsMap.Begin(); dataIter; ++dataIter)
	{
		item = dataIter.Key();
		if ((noNonPlayable && !item->IsItemPlayable()) || (noQuestItem && item->IsQuestItem()))
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
				if (noEquipped && xData->HasType(kExtraData_Worn))
					continue;
				invRef = CreateInventoryRef(thisObj, item, xCount, xData);
				if (listForm) listForm->list.Prepend(invRef);
				else s_tempElements.Append(invRef);
				count++;
			}
			while (baseCount && (xdlIter = xdlIter->next));
		}
		if (baseCount > 0)
		{
			invRef = CreateInventoryRef(thisObj, item, baseCount, NULL);
			if (listForm) listForm->list.Prepend(invRef);
			else s_tempElements.Append(invRef);
			count++;
		}
	}
	if (count)
	{
		if (listForm) *result = count;
		else AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_RemoveMeIRAlt_Execute(COMMAND_ARGS)
{
	SInt32 quantity = 0, clrOwner = 0;
	TESObjectREFR *destRef = NULL;
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
	*result = 0;
	UInt32 slotIdx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &slotIdx) && (slotIdx <= 19) && thisObj->IsActor())
	{
		TESObjectREFR *invRef = GetEquippedItemRef((Actor*)thisObj, slotIdx);
		if (invRef) REFR_RES = invRef->refID;
	}
	return true;
}

bool Cmd_GetNoUnequip_Execute(COMMAND_ARGS)
{
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	*result = (invRef && invRef->xData && invRef->xData->HasType(kExtraData_CannotWear)) ? 1 : 0;
	return true;
}

bool Cmd_SetNoUnequip_Execute(COMMAND_ARGS)
{
	UInt32 noUnequip;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &noUnequip))
	{
		InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
		ExtraDataList *xData = invRef ? invRef->xData : NULL;
		if (xData)
		{
			if (!noUnequip)
				RemoveExtraType(xData, kExtraData_CannotWear);
			else if (xData->HasType(kExtraData_Worn) && !xData->HasType(kExtraData_CannotWear))
				AddExtraData(xData, ExtraCannotWear::Create());
		}
	}
	return true;
}

bool Cmd_GetEquippedWeaponPoison_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor())
	{
		ContChangesEntry *wpnInfo = ((Actor*)thisObj)->GetWeaponInfo();
		if (wpnInfo && wpnInfo->extendData)
		{
			ExtraDataList *xDataList = wpnInfo->extendData->GetFirstItem();
			if (xDataList)
			{
				ExtraPoison *xPoison = GetExtraType(xDataList, Poison);
				if (xPoison && xPoison->poisonEffect)
					REFR_RES = xPoison->poisonEffect->refID;
			}
		}
	}
	return true;
}

bool Cmd_ToggleItemUnique_Execute(COMMAND_ARGS)
{
	TESForm *itemOrList;
	UInt32 toggle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &itemOrList, &toggle))
	{
		ListNode<TESForm> *traverse = NULL;
		if IS_ID(itemOrList, BGSListForm)
			traverse = ((BGSListForm*)itemOrList)->list.Head();
		else if (IS_TYPE(itemOrList, TESObjectARMO) || IS_ID(itemOrList, TESObjectWEAP))
		{
			ListNode<TESForm> tempList(itemOrList);
			traverse = &tempList;
		}
		if (traverse)
		{
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
	}
	return true;
}

bool Cmd_GetBaseItems_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *baseForm = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &baseForm))
		return true;
	if (!baseForm)
	{
		if (!thisObj) return true;
		baseForm = thisObj->baseForm;
	}
	TESContainer *container = DYNAMIC_CAST(baseForm, TESForm, TESContainer);
	if (!container) return true;
	s_tempElements.Clear();
	ListNode<TESContainer::FormCount> *traverse = container->formCountList.Head();
	TESContainer::FormCount *formCount;
	do
	{
		formCount = traverse->data;
		if (formCount)
			s_tempElements.Append(formCount->form);
	}
	while (traverse = traverse->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_SetOnUseAidItemEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	TESForm *itemOrList;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &itemOrList) || NOT_ID(script, Script))
		return true;
	ListNode<TESForm> *iter;
	AlchemyItem *alchItem;
	EventCallbackScripts *callbacks;
	if IS_ID(itemOrList, BGSListForm)
		iter = ((BGSListForm*)itemOrList)->list.Head();
	else
	{
		ListNode<TESForm> tempList(itemOrList);
		iter = &tempList;
	}
	do
	{
		alchItem = (AlchemyItem*)iter->data;
		if (!alchItem || NOT_ID(alchItem, AlchemyItem))
			continue;
		if (addEvnt)
		{
			if (s_useAidItemEventMap.Insert(alchItem, &callbacks))
				HOOK_MOD(EquipAidItem, true);
			callbacks->Insert(script);
			alchItem->SetJIPFlag(kHookFormFlag6_OnEquipHandlers, true);
		}
		else
		{
			auto findItem = s_useAidItemEventMap.Find(alchItem);
			if (!findItem || !findItem().Erase(script) || !findItem().Empty())
				continue;
			findItem.Remove();
			HOOK_MOD(EquipAidItem, false);
			alchItem->SetJIPFlag(kHookFormFlag6_OnEquipHandlers, false);
		}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_GetEquippedArmorRefs_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor())
	{
		ValidBip01Names *equipment = ((Actor*)thisObj)->GetValidBip01Names();
		if (equipment)
		{
			ExtraContainerChanges::EntryDataList *entryList = thisObj->GetContainerChangesList();
			if (entryList)
			{
				s_tempElements.Clear();
				s_tempFormList.Clear();
				ValidBip01Names::Data *slotData = equipment->slotData;
				TESForm *item;
				ContChangesEntry *entry;
				ExtraDataList *xData;
				TESObjectREFR *invRef;
				for (UInt32 count = 20; count; count--, slotData++)
				{
					item = slotData->item;
					if (!item || NOT_TYPE(item, TESObjectARMO) || !s_tempFormList.Insert(item) || !(entry = entryList->FindForItem(item)) || !(xData = entry->GetEquippedExtra()))
						continue;
					invRef = CreateInventoryRef(thisObj, item, entry->countDelta, xData);
					if (invRef) s_tempElements.Append(invRef);
				}
				AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
			}
		}
	}
	return true;
}

float __fastcall GetArmorEffectiveDX(TESObjectREFR *thisObj, UInt32 funcAddr)
{
	if IS_TYPE(thisObj->baseForm, TESObjectARMO)
	{
		InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
		if (invRef)
			return ThisCall<float>(funcAddr, invRef->entry, 0);
		ExtraContainerChanges::ExtendDataList extendData;
		extendData.Init(&thisObj->extraDataList);
		ContChangesEntry tempEntry(&extendData, 1, thisObj->baseForm);
		return ThisCall<float>(funcAddr, &tempEntry, 0);
	}
	return 0;
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
	*result = 0;
	UInt32 keyNum;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &keyNum) && keyNum && (keyNum <= 8))
	{
		ExtraDataList *xData;
		ContChangesEntry *entry = GetHotkeyItemEntry(keyNum, &xData);
		if (entry)
		{
			TESObjectREFR *invRef = CreateInventoryRef(g_thePlayer, entry->type, xData->GetCount(), xData);
			REFR_RES = invRef->refID;
		}
	}
	return true;
}