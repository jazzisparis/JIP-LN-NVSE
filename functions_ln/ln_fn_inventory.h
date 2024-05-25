#pragma once

DEFINE_COMMAND_PLUGIN(SetHotkey, 0, kParams_OneInt_OneForm_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ClearAllHotkeys, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SaveHotkeys, 0, nullptr);
DEFINE_COMMAND_PLUGIN(RestoreHotkeys, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(BaseGetItemCount, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(BaseAddItem, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(BaseAddItemHealth, 0, kParams_OneForm_OneInt_OneFloat_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(BaseRemoveItem, 0, kParams_OneForm_OneInt_OneOptionalForm);

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
	if (auto xChanges = GetExtraType(&g_thePlayer->extraDataList, ExtraContainerChanges); xChanges && xChanges->data)
	{
		auto entryIter = xChanges->data->objList->Head();
		do
		{
			auto entry = entryIter->data;
			if (!entry) continue;
			UInt8 type = entry->type->typeID;
			if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem))
				continue;
			if (entry->type == hotkey.form)
			{
				if (!entry->extendData)
				{
					if ((hotkey.health < 0) && (hotkey.modFlags <= 0))
					{
						auto xData = ExtraDataList::Create();
						xData->AddExtra(ExtraHotkey::Create(index));
						entry->extendData = Game_HeapAlloc<ContChangesExtraList>();
						entry->extendData->Init(xData);
					}
					continue;
				}
				if (entry->extendData->Empty())
				{
					if ((hotkey.health < 0) && (hotkey.modFlags <= 0))
					{
						auto xData = ExtraDataList::Create();
						xData->AddExtra(ExtraHotkey::Create(index));
						entry->extendData->Prepend(xData);
					}
					continue;
				}
				auto xdlIter = entry->extendData->Head();
				bool found = false;
				do
				{
					auto xData = xdlIter->data;
					if (!xData) continue;
					auto xHotkey = GetExtraType(xData, ExtraHotkey);
					bool matching = true;
					if (type != kFormType_AlchemyItem)
					{
						if (hotkey.health != -2.0F)
						{
							auto xHealth = GetExtraType(xData, ExtraHealth);
							matching = xHealth ? (ULNG(xHealth->health) == ULNG(hotkey.health)) : (hotkey.health < 0);
						}
						if (matching && (hotkey.modFlags >= 0))
						{
							auto xModFlags = GetExtraType(xData, ExtraWeaponModFlags);
							matching = xModFlags ? (xModFlags->flags == hotkey.modFlags) : !hotkey.modFlags;
						}
					}
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
				auto xdlIter = entry->extendData->Head();
				do
				{
					if (auto xData = xdlIter->data)
						if (auto xHotkey = GetExtraType(xData, ExtraHotkey); xHotkey && (xHotkey->index == index))
						{
							xData->RemoveByType(kXData_ExtraHotkey);
							if (!xData->m_data)
							{
								entry->extendData->Remove(xData);
								Game_HeapFree(xData);
							}
						}
					break;
				}
				while (xdlIter = xdlIter->next);
			}
		}
		while (entryIter = entryIter->next);
	}
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
	if (auto entryList = g_thePlayer->GetContainerChangesList())
	{
		auto entryIter = entryList->Head();
		do
		{
			if (auto entry = entryIter->data; entry && entry->extendData)
				if (UInt8 type = entry->type->typeID; (type == kFormType_TESObjectARMO) || (type == kFormType_TESObjectWEAP) || (type == kFormType_AlchemyItem))
				{
					auto xdlIter = entry->extendData->Head();
					do
					{
						if (auto xData = xdlIter->data; xData && xData->HasType(kXData_ExtraHotkey))
						{
							xData->RemoveByType(kXData_ExtraHotkey);
							if (!xData->m_data)
							{
								entry->extendData->Remove(xData);
								Game_HeapFree(xData);
							}
							break;
						}
					}
					while (xdlIter = xdlIter->next);
				}
		}
		while (entryIter = entryIter->next);
	}
	return true;
}

bool Cmd_SaveHotkeys_Execute(COMMAND_ARGS)
{
	for (HotkeyInfo &hotkey : s_savedHotkeys)
		hotkey = HotkeyInfo();
	if (auto entryList = g_thePlayer->GetContainerChangesList())
	{
		auto entryIter = entryList->Head();
		do
		{
			if (auto entry = entryIter->data; entry && entry->extendData)
				if (UInt8 type = entry->type->typeID; (type == kFormType_TESObjectARMO) || (type == kFormType_TESObjectWEAP) || (type == kFormType_AlchemyItem))
				{
					auto xdlIter = entry->extendData->Head();
					do
					{
						if (auto xData = xdlIter->data)
							if (auto xHotkey = GetExtraType(xData, ExtraHotkey))
							{
								UInt32 index = xHotkey->index;
								s_savedHotkeys[index].form = entry->type;
								if (type != kFormType_AlchemyItem)
								{
									if (auto xHealth = GetExtraType(xData, ExtraHealth))
										s_savedHotkeys[index].health = xHealth->health;
									if (auto xModFlags = GetExtraType(xData, ExtraWeaponModFlags))
										s_savedHotkeys[index].modFlags = xModFlags->flags;
								}
							}
					}
					while (xdlIter = xdlIter->next);
				}
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &arrID))
		if (arrID)
		{
			if (NVSEArrayVar *inArray = LookupArrayByID(arrID); inArray && (GetArraySize(inArray) == 8))
			{
				ArrayElementL vals[8], data[3];
				GetElements(inArray, vals, nullptr);
				for (UInt32 idx = 0; idx < 8; idx++)
					if (auto dataArray = vals[idx].GetArray(); dataArray && (GetArraySize(dataArray) == 3))
					{
						GetElements(dataArray, data, nullptr);
						SetHotkey(idx, HotkeyInfo(data[0].Form(), data[1].Number(), data[2].Number()));
					}
			}
		}
		else for (UInt8 idx = 0; idx < 8; idx++)
			SetHotkey(idx, s_savedHotkeys[idx]);
	return true;
}

bool Cmd_BaseGetItemCount_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &base))
		if (base || (thisObj && (base = thisObj->baseForm)))
			if (TESContainer *container = base->GetContainer())
			{
				SInt32 count = 0;
				auto iter = container->formCountList.Head();
				do
				{
					if (auto formCount = iter->data; formCount && (formCount->form == form))
						count += formCount->count;
				}
				while (iter = iter->next);
				*result = count;
			}
	return true;
}

__declspec(noinline) void __fastcall DoBaseAddItem(TESForm *base, TESObjectREFR *thisObj, TESForm *form, UInt32 count, float health)
{
	if (base || (thisObj && (base = thisObj->baseForm)))
		if (TESContainer *container = base->GetContainer())
		{
			auto iter = container->formCountList.Head();
			do
			{
				if (auto formCount = iter->data; formCount && (formCount->form == form))
				{
					formCount->count += count;
					return;
				}
			}
			while (iter = iter->next);
			auto formCount = Game_HeapAlloc<TESContainer::FormCount>();
			formCount->contExtraData = Game_HeapAlloc<ContainerExtra>();
			formCount->contExtraData->ownerFaction = nullptr;
			formCount->contExtraData->globalVar = nullptr;
			formCount->contExtraData->health = health;
			formCount->form = form;
			formCount->count = count;
			container->formCountList.Prepend(formCount);
		}
}

bool Cmd_BaseAddItem_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = nullptr;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &base))
		DoBaseAddItem(base, thisObj, form, count, 1.0F);
	return true;
}

bool Cmd_BaseAddItemHealth_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = nullptr;
	UInt32 count;
	float health;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &health, &base))
		DoBaseAddItem(base, thisObj, form, count, health);
	return true;
}

bool Cmd_BaseRemoveItem_Execute(COMMAND_ARGS)
{
	TESForm *form, *base = nullptr;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &base))
		if (base || (thisObj && (base = thisObj->baseForm)))
			if (TESContainer *container = base->GetContainer())
			{
				tList<TESContainer::FormCount>::Node *iter = container->formCountList.Head(), *prev = nullptr;
				do
				{
					if (auto formCount = iter->data; formCount && (formCount->form == form))
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
			}
	return true;
}