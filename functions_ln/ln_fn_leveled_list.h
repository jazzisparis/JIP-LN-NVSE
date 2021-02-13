#pragma once

DEFINE_COMMAND_ALT_PLUGIN(LeveledListAddForm, LListAddForm, , 0, 5, kParams_JIP_TwoForms_TwoInts_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListRemoveForm, LListRemoveForm, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListReplaceForm, LListReplaceForm, , 0, 3, kParams_JIP_ThreeForms);
DEFINE_COMMAND_PLUGIN(GetChanceNone, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChanceNone, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChanceNoneGlobal, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChanceNoneGlobal, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(GetNumLevItems, LListGetCount, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetNthLevItem, LListGetNthForm, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNthLevItem, LListsetNthForm, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetNthLevItemLevel, LListGetNthLevel, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNthLevItemLevel, LListSetNthLevel, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(GetNthLevItemCount, LListGetNthCount, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNthLevItemCount, LListSetNthCount, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(RemoveNthLevItem, LListRemoveNthForm, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListClear, LListClear, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetLevItemIndexByLevel, LListGetLevelIndex, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(GetLevItemIndexByForm, LListGetFormIndex, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_ALT_PLUGIN(DumpLevList, LListDump, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetLeveledListFlags, LListGetFlags, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(SetLeveledListFlags, LListSetFlags, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(LeveledListHasFormDeep, LListHasFormDeep, , 0, 2, kParams_JIP_TwoForms);

bool Cmd_LeveledListAddForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	UInt32 level, count;
	float health;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form, &level, &count, &health) && (lvlList = list->GetLvlList()))
		lvlList->AddItem(form, level, count, health ? (health <= 1 ? health : health / 100) : 0);
	return true;
}

bool Cmd_LeveledListRemoveForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form) && (lvlList = list->GetLvlList()))
		*result = (int)lvlList->RemoveItem(form);
	else *result = 0;
	return true;
}

bool Cmd_LeveledListReplaceForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *oldform, *newform;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &oldform, &newform) && (lvlList = list->GetLvlList()))
	{
		ListNode<TESLeveledList::ListData> *iter = lvlList->list.Head();
		TESLeveledList::ListData *data;
		do
		{
			if ((data = iter->data) && (data->form == oldform))
				data->form = newform;
		}
		while (iter = iter->next);
	}
	return true;
}

bool Cmd_GetChanceNone_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (lvlList = form->GetLvlList()))
		*result = lvlList->chanceNone;
	else *result = -1;
	return true;
}

bool Cmd_SetChanceNone_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 newChance;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &newChance) && (lvlList = form->GetLvlList()) && (newChance <= 100))
		lvlList->chanceNone = newChance;
	return true;
}

bool Cmd_GetChanceNoneGlobal_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (lvlList = form->GetLvlList()) && lvlList->global)
		REFR_RES = lvlList->global->refID;
	return true;
}

bool Cmd_SetChanceNoneGlobal_Execute(COMMAND_ARGS)
{
	TESForm *list;
	TESGlobal *global = NULL;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &global) && (lvlList = list->GetLvlList()) && (!global || IS_ID(global, TESGlobal)))
		lvlList->global = global;
	return true;
}

bool Cmd_GetNumLevItems_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (lvlList = form->GetLvlList()))
		*result = (int)lvlList->list.Count();
	else *result = 0;
	return true;
}

bool Cmd_GetNthLevItem_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 index;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index) && (lvlList = form->GetLvlList()))
	{
		TESLeveledList::ListData *data = lvlList->list.GetNthItem(index);
		if (data && data->form) REFR_RES = data->form->refID;
	}
	return true;
}

bool Cmd_SetNthLevItem_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	UInt32 index;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &index, &form) && (lvlList = list->GetLvlList()))
	{
		TESLeveledList::ListData *data = lvlList->list.GetNthItem(index);
		if (data && data->form) data->form = form;
	}
	return true;
}

bool Cmd_GetNthLevItemLevel_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 index;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index) && (lvlList = form->GetLvlList()))
	{
		TESLeveledList::ListData *data = lvlList->list.GetNthItem(index);
		if (data) *result = data->level;
	}
	return true;
}

bool Cmd_SetNthLevItemLevel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	UInt32 level;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index, &level) && (lvlList = form->GetLvlList()))
	{
		TESLeveledList::ListData *data = lvlList->list.GetNthItem(index);
		if (data) data->level = level;
	}
	return true;
}

bool Cmd_GetNthLevItemCount_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 index;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index) && (lvlList = form->GetLvlList()))
	{
		TESLeveledList::ListData *data = lvlList->list.GetNthItem(index);
		if (data) *result = data->count;
	}
	return true;
}

bool Cmd_SetNthLevItemCount_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	UInt32 count;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index, &count) && (lvlList = form->GetLvlList()))
	{
		TESLeveledList::ListData *data = lvlList->list.GetNthItem(index);
		if (data) data->count = count;
	}
	return true;
}

bool Cmd_RemoveNthLevItem_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 index;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &index) && (lvlList = form->GetLvlList()))
		lvlList->list.RemoveNth(index);
	return true;
}

bool Cmd_LeveledListClear_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (lvlList = form->GetLvlList()))
		lvlList->list.RemoveAll();
	return true;
}

bool Cmd_GetLevItemIndexByLevel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 level;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &level) && (lvlList = form->GetLvlList()))
		*result = lvlList->GetItemIndexByLevel(level);
	else *result = -1;
	return true;
}

bool Cmd_GetLevItemIndexByForm_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form) && (lvlList = list->GetLvlList()))
		*result = lvlList->GetItemIndexByForm(form);
	else *result = -1;
	return true;
}

bool Cmd_DumpLevList_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IsConsoleOpen() && (lvlList = form->GetLvlList()))
	{
		Console_Print("Dumping LeveledList [%08X]", form->refID);
		s_dumpLvlListIndent = 50;
		lvlList->Dump();
	}
	return true;
}

bool Cmd_GetLeveledListFlags_Execute(COMMAND_ARGS)
{
	TESForm *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (lvlList = form->GetLvlList()))
		*result = lvlList->flags;
	else *result = 0;
	return true;
}

bool Cmd_SetLeveledListFlags_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 flags;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &flags) && (lvlList = form->GetLvlList()))
		lvlList->flags = flags;
	return true;
}

bool Cmd_LeveledListHasFormDeep_Execute(COMMAND_ARGS)
{
	TESForm *list, *form;
	TESLeveledList *lvlList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &list, &form) && (lvlList = list->GetLvlList()))
	{
		s_tempFormList.Clear();
		*result = lvlList->HasFormDeep(form);
	}
	else *result = 0;
	return true;
}