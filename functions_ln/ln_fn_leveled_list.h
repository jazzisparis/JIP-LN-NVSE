#pragma once

DEFINE_COMMAND_ALT_PLUGIN(LeveledListAddForm, LListAddForm, 0, kParams_TwoForms_TwoInts_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListRemoveForm, LListRemoveForm, 0, kParams_TwoForms);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListReplaceForm, LListReplaceForm, 0, kParams_ThreeForms);
DEFINE_COMMAND_PLUGIN(GetChanceNone, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChanceNone, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChanceNoneGlobal, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChanceNoneGlobal, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(GetNumLevItems, LListGetCount, 0, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetNthLevItem, LListGetNthForm, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNthLevItem, LListsetNthForm, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetNthLevItemLevel, LListGetNthLevel, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNthLevItemLevel, LListSetNthLevel, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(GetNthLevItemCount, LListGetNthCount, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNthLevItemCount, LListSetNthCount, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(RemoveNthLevItem, LListRemoveNthForm, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListClear, LListClear, 0, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetLevItemIndexByLevel, LListGetLevelIndex, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(GetLevItemIndexByForm, LListGetFormIndex, 0, kParams_TwoForms);
DEFINE_COMMAND_ALT_PLUGIN(DumpLevList, LListDump, 0, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetLeveledListFlags, LListGetFlags, 0, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(SetLeveledListFlags, LListSetFlags, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListHasFormDeep, LListHasFormDeep, 0, kParams_TwoForms);

bool Cmd_LeveledListAddForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	UInt32 level, count;
	float health;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form, &level, &count, &health))
		if (auto lvlList = list->GetLvlList())
		{
			if (health < 0)
				health = 0;
			else if (health > 1.0F)
				health *= 0.01F;
			lvlList->AddItem(form, level, count, health);
		}
	return true;
}

bool Cmd_LeveledListRemoveForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form))
		if (auto lvlList = list->GetLvlList())
			*result = (int)lvlList->RemoveItem(form);
	return true;
}

bool Cmd_LeveledListReplaceForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *oldform, *newform;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &oldform, &newform))
		if (auto lvlList = list->GetLvlList())
		{
			auto iter = lvlList->list.Head();
			do
			{
				if (TESLeveledList::ListData *data = iter->data; data && (data->form == oldform))
					data->form = newform;
			}
			while (iter = iter->next);
		}
	return true;
}

bool Cmd_GetChanceNone_Execute(COMMAND_ARGS)
{
	TESForm *form;
	int chanceNone = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		if (auto lvlList = form->GetLvlList())
			chanceNone = lvlList->chanceNone;
	*result = chanceNone;
	return true;
}

bool Cmd_SetChanceNone_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 newChance;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &newChance) && (newChance <= 100))
		if (auto lvlList = form->GetLvlList())
			lvlList->chanceNone = newChance;
	return true;
}

bool Cmd_GetChanceNoneGlobal_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		if (auto lvlList = form->GetLvlList(); lvlList && lvlList->global)
			REFR_RES = lvlList->global->refID;
	return true;
}

bool Cmd_SetChanceNoneGlobal_Execute(COMMAND_ARGS)
{
	TESForm *list;
	TESGlobal *global = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &global) && (!global || IS_ID(global, TESGlobal)))
		if (auto lvlList = list->GetLvlList())
			lvlList->global = global;
	return true;
}

bool Cmd_GetNumLevItems_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		if (auto lvlList = form->GetLvlList())
			*result = (int)lvlList->list.Count();
	return true;
}

bool Cmd_GetNthLevItem_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index))
		if (auto lvlList = form->GetLvlList())
			if (TESLeveledList::ListData *data = lvlList->list.GetNthItem(index); data && data->form)
				REFR_RES = data->form->refID;
	return true;
}

bool Cmd_SetNthLevItem_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &index, &form))
		if (auto lvlList = list->GetLvlList())
			if (TESLeveledList::ListData *data = lvlList->list.GetNthItem(index))
				data->form = form;
	return true;
}

bool Cmd_GetNthLevItemLevel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index))
		if (auto lvlList = form->GetLvlList())
			if (TESLeveledList::ListData* data = lvlList->list.GetNthItem(index))
				*result = data->level;
	return true;
}

bool Cmd_SetNthLevItemLevel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	UInt32 level;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index, &level))
		if (auto lvlList = form->GetLvlList())
			if (TESLeveledList::ListData *data = lvlList->list.GetNthItem(index))
				data->level = level;
	return true;
}

bool Cmd_GetNthLevItemCount_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index))
		if (auto lvlList = form->GetLvlList())
			if (TESLeveledList::ListData *data = lvlList->list.GetNthItem(index))
				*result = data->count;
	return true;
}

bool Cmd_SetNthLevItemCount_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	UInt32 count;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index, &count))
		if (auto lvlList = form->GetLvlList())
			if (TESLeveledList::ListData *data = lvlList->list.GetNthItem(index))
				data->count = count;
	return true;
}

bool Cmd_RemoveNthLevItem_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index))
		if (auto lvlList = form->GetLvlList())
			lvlList->list.RemoveNth(index);
	return true;
}

bool Cmd_LeveledListClear_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		if (auto lvlList = form->GetLvlList())
			lvlList->list.RemoveAll();
	return true;
}

bool Cmd_GetLevItemIndexByLevel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 level;
	SInt32 index = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &level))
		if (auto lvlList = form->GetLvlList())
			index = lvlList->GetItemIndexByLevel(level);
	*result = index;
	return true;
}

bool Cmd_GetLevItemIndexByForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	SInt32 index = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form))
		if (auto lvlList = list->GetLvlList())
			index = lvlList->GetItemIndexByForm(form);
	*result = index;
	return true;
}

bool Cmd_DumpLevList_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IsConsoleOpen())
		if (auto lvlList = form->GetLvlList())
		{
			Console_Print("Dumping LeveledList [%08X]", form->refID);
			s_dumpLvlListIndent = 60;
			lvlList->Dump();
		}
	return true;
}

bool Cmd_GetLeveledListFlags_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		if (auto lvlList = form->GetLvlList())
			*result = lvlList->flags;
	return true;
}

bool Cmd_SetLeveledListFlags_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 flags;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &flags))
		if (auto lvlList = form->GetLvlList())
			lvlList->flags = flags;
	return true;
}

bool Cmd_LeveledListHasFormDeep_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form))
		if (auto lvlList = list->GetLvlList(); lvlList && LeveledListHasFormDeep(lvlList, form, GetTempFormList()))
			*result = 1;
	return true;
}