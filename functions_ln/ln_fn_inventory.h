#pragma once

DEFINE_COMMAND_PLUGIN(SetHotkey, 0, 4, kParams_OneInt_OneForm_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ClearAllHotkeys, 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SaveHotkeys, 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(RestoreHotkeys, 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(BaseGetItemCount, 0, 2, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(BaseAddItem, 0, 3, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(BaseAddItemHealth, 0, 4, kParams_OneForm_OneInt_OneFloat_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(BaseRemoveItem, 0, 3, kParams_OneForm_OneInt_OneOptionalForm);

struct HotkeyInfo
{
	TESForm		*form;
	float		health;
	int			modFlags;

	HotkeyInfo() : form(nullptr), health(-1.0F), modFlags(0) {}
	HotkeyInfo(TESForm *_form, float _health, int _modFlags) : form(_form), health(_health), modFlags(_modFlags) {}
}
s_savedHotkeys[8] = {{}, {}, {}, {}, {}, {}, {}, {}};

void SetHotkey(UInt8 index, const HotkeyInfo &hotkey)
{
	if (!hotkey.form)
	{
		ClearHotkey(index);
		return;
	}
	ExtraContainerChanges *xChanges = GetExtraType(&g_thePlayer->extraDataList, ExtraContainerChanges);
	if (!xChanges || !xChanges->data) return;
	auto entryIter = xChanges->data->objList->Head();
	ContChangesEntry *entry;
	UInt8 type;
	ExtraDataList *xData;
	tList<ExtraDataList>::Node *xdlIter;
	ExtraHotkey *xHotkey;
	ExtraHealth *xHealth;
	ExtraWeaponModFlags *xModFlags;
	bool found, matching;
	do
	{
		if (!(entry = entryIter->data)) continue;
		type = entry->type->typeID;
		if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem)) continue;
		if (entry->type == hotkey.form)
		{
			if (!entry->extendData)
			{
				if ((hotkey.health >= 0) || (hotkey.modFlags > 0)) continue;
				xData = ExtraDataList::Create();
				xData->AddExtra(ExtraHotkey::Create(index));
				entry->extendData = Game_HeapAlloc<ContChangesExtraList>();
				entry->extendData->Init(xData);
				continue;
			}
			if (entry->extendData->Empty())
			{
				if ((hotkey.health >= 0) || (hotkey.modFlags > 0)) continue;
				xData = ExtraDataList::Create();
				xData->AddExtra(ExtraHotkey::Create(index));
				entry->extendData->Prepend(xData);
				continue;
			}
			xdlIter = entry->extendData->Head();
			found = false;
			do
			{
				if (!(xData = xdlIter->data)) continue;
				xHotkey = GetExtraType(xData, ExtraHotkey);
				if (type != kFormType_AlchemyItem)
				{
					if (hotkey.health != -2.0F)
					{
						xHealth = GetExtraType(xData, ExtraHealth);
						matching = xHealth ? (ULNG(xHealth->health) == ULNG(hotkey.health)) : (hotkey.health < 0);
					}
					else matching = true;
					if (matching && (hotkey.modFlags >= 0))
					{
						xModFlags = GetExtraType(xData, ExtraWeaponModFlags);
						matching = xModFlags ? (xModFlags->flags == hotkey.modFlags) : !hotkey.modFlags;
					}
				}
				else matching = true;
				if (xHotkey)
				{
					if (matching && !found)
					{
						found = true;
						xHotkey->index = index;
					}
					else if (xHotkey->index == index)
						xData->RemoveByType(kXData_ExtraHotkey);
				}
				else if (matching && !found)
				{
					found = true;
					xData->AddExtra(ExtraHotkey::Create(index));
				}
			}
			while (xdlIter = xdlIter->next);
			entry->extendData->CleanEmpty();
		}
		else if (entry->extendData)
		{
			xdlIter = entry->extendData->Head();
			do
			{
				if (!(xData = xdlIter->data)) continue;
				xHotkey = GetExtraType(xData, ExtraHotkey);
				if (!xHotkey || (xHotkey->index != index)) continue;
				xData->RemoveByType(kXData_ExtraHotkey);
				if (!xData->m_data)
				{
					entry->extendData->Remove(xData);
					Game_HeapFree(xData);
				}
				break;
			}
			while (xdlIter = xdlIter->next);
		}
	}
	while (entryIter = entryIter->next);
}

bool Cmd_SetHotkey_Execute(COMMAND_ARGS)
{
	UInt32 index;
	TESForm *form;
	float health = -2.0F;
	int flags = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &index, &form, &health, &flags) && index && (index <= 8))
		SetHotkey(index - 1, HotkeyInfo(form, health, flags));
	return true;
}

bool Cmd_ClearAllHotkeys_Execute(COMMAND_ARGS)
{
	ContChangesEntryList *entryList = g_thePlayer->GetContainerChangesList();
	if (!entryList) return true;
	auto entryIter = entryList->Head();
	ContChangesEntry *entry;
	UInt8 type;
	tList<ExtraDataList>::Node *xdlIter;
	ExtraDataList *xData;
	ExtraHotkey *xHotkey;
	do
	{
		if (!(entry = entryIter->data) || !entry->extendData) continue;
		type = entry->type->typeID;
		if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem)) continue;
		xdlIter = entry->extendData->Head();
		do
		{
			if (!(xData = xdlIter->data)) continue;
			xHotkey = GetExtraType(xData, ExtraHotkey);
			if (!xHotkey) continue;
			xData->RemoveByType(kXData_ExtraHotkey);
			if (!xData->m_data)
			{
				entry->extendData->Remove(xData);
				Game_HeapFree(xData);
			}
			break;
		}
		while (xdlIter = xdlIter->next);
	}
	while (entryIter = entryIter->next);
	return true;
}

bool Cmd_SaveHotkeys_Execute(COMMAND_ARGS)
{
	*result = 0;
	for (HotkeyInfo &hotkey : s_savedHotkeys) hotkey = HotkeyInfo();
	ContChangesEntryList *entryList = g_thePlayer->GetContainerChangesList();
	if (entryList)
	{
		auto entryIter = entryList->Head();
		ContChangesEntry *entry;
		UInt8 type, index;
		tList<ExtraDataList>::Node *xdlIter;
		ExtraDataList *xData;
		ExtraHotkey *xHotkey;
		ExtraHealth *xHealth;
		ExtraWeaponModFlags *xModFlags;
		do
		{
			if (!(entry = entryIter->data) || !entry->extendData) continue;
			type = entry->type->typeID;
			if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem)) continue;
			xdlIter = entry->extendData->Head();
			do
			{
				if (!(xData = xdlIter->data)) continue;
				xHotkey = GetExtraType(xData, ExtraHotkey);
				if (!xHotkey) continue;
				index = xHotkey->index;
				s_savedHotkeys[index].form = entry->type;
				if (type == kFormType_AlchemyItem) continue;
				xHealth = GetExtraType(xData, ExtraHealth);
				if (xHealth) s_savedHotkeys[index].health = xHealth->health;
				xModFlags = GetExtraType(xData, ExtraWeaponModFlags);
				if (xModFlags) s_savedHotkeys[index].modFlags = xModFlags->flags;
			}
			while (xdlIter = xdlIter->next);
		}
		while (entryIter = entryIter->next);
	}
	TempElements *tmpElements = GetTempElements();
	for (HotkeyInfo &hotkey : s_savedHotkeys)
	{
		ArrayElementL elements[3] = {hotkey.form, hotkey.health, hotkey.modFlags};
		tmpElements->Append(CreateArray(elements, 3, scriptObj));
	}
	*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_RestoreHotkeys_Execute(COMMAND_ARGS)
{
	UInt32 arrID = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID)) return true;
	if (arrID)
	{
		NVSEArrayVar *inArray = LookupArrayByID(arrID), *dataArray;
		if (!inArray || (GetArraySize(inArray) != 8)) return true;
		ArrayElementL vals[8], data[3];
		GetElements(inArray, vals, NULL);
		for (UInt8 idx = 0; idx < 8; idx++)
		{
			dataArray = vals[idx].Array();
			if (!dataArray || (GetArraySize(dataArray) != 3)) continue;
			GetElements(dataArray, data, NULL);
			SetHotkey(idx, HotkeyInfo(data[0].Form(), data[1].Number(), data[2].Number()));
		}
	}
	else for (UInt8 idx = 0; idx < 8; idx++) SetHotkey(idx, s_savedHotkeys[idx]);
	return true;
}

bool Cmd_BaseGetItemCount_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form, *base = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &base)) return true;
	if (!base)
	{
		if (!thisObj) return true;
		base = thisObj->baseForm;
	}
	TESContainer *container = base->GetContainer();
	if (container)
	{
		SInt32 count = 0;
		auto iter = container->formCountList.Head();
		TESContainer::FormCount *formCount;
		do
		{
			formCount = iter->data;
			if (formCount && (formCount->form == form))
				count += formCount->count;
		}
		while (iter = iter->next);
		*result = count;
	}
	return true;
}

bool Cmd_BaseAddItem_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = NULL;
	UInt32 count;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &base)) return true;
	if (!base)
	{
		if (!thisObj) return true;
		base = thisObj->baseForm;
	}
	TESContainer *container = base->GetContainer();
	if (!container) return true;
	auto iter = container->formCountList.Head();
	TESContainer::FormCount *formCount;
	do
	{
		formCount = iter->data;
		if (formCount && (formCount->form == form))
		{
			formCount->count += count;
			return true;
		}
	}
	while (iter = iter->next);
	formCount = Game_HeapAlloc<TESContainer::FormCount>();
	formCount->contExtraData = Game_HeapAlloc<ContainerExtra>();
	formCount->contExtraData->ownerFaction = NULL;
	formCount->contExtraData->globalVar = NULL;
	formCount->contExtraData->health = 1;
	formCount->form = form;
	formCount->count = count;
	container->formCountList.Prepend(formCount);
	return true;
}

bool Cmd_BaseAddItemHealth_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = NULL;
	UInt32 count;
	float health;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &health, &base)) return true;
	if (!base)
	{
		if (!thisObj) return true;
		base = thisObj->baseForm;
	}
	TESContainer *container = base->GetContainer();
	if (!container) return true;
	auto iter = container->formCountList.Head();
	TESContainer::FormCount *formCount;
	do
	{
		formCount = iter->data;
		if (formCount && (formCount->form == form))
		{
			formCount->count += count;
			return true;
		}
	}
	while (iter = iter->next);
	formCount = Game_HeapAlloc<TESContainer::FormCount>();
	formCount->contExtraData = Game_HeapAlloc<ContainerExtra>();
	formCount->contExtraData->ownerFaction = NULL;
	formCount->contExtraData->globalVar = NULL;
	formCount->contExtraData->health = health;
	formCount->form = form;
	formCount->count = count;
	container->formCountList.Prepend(formCount);
	return true;
}

bool Cmd_BaseRemoveItem_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = NULL;
	UInt32 count;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &base)) return true;
	if (!base)
	{
		if (!thisObj) return true;
		base = thisObj->baseForm;
	}
	TESContainer *container = base->GetContainer();
	if (!container) return true;
	tList<TESContainer::FormCount>::Node *iter = container->formCountList.Head(), *prev = NULL;
	TESContainer::FormCount *formCount;
	do
	{
		formCount = iter->data;
		if (formCount && (formCount->form == form))
		{
			formCount->count -= count;
			if (formCount->count < 1)
			{
				if (thisObj && IS_ACTOR(thisObj))
					((Actor*)thisObj)->UnequipItem(form);
				if (prev) prev->RemoveNext();
				else iter->RemoveMe();
			}
			break;
		}
		prev = iter;
	}
	while (iter = iter->next);
	return true;
}