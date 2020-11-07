#pragma once

DEFINE_COMMAND_PLUGIN(HasPerkRank, , 1, 2, kParams_JIP_OneForm_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetPerkRank, , 1, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPerkEntryCount, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryType, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryFunction, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryFunction, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryForm, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryForm, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryValue1, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryValue1, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryValue2, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryValue2, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryString, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryString, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(IsTrait, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetPerkLevel, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetPerkLevel, , 0, 2, kParams_OneForm_OneInt);

bool Cmd_HasPerkRank_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 useAlt = 0;
	if (ExtractArgs(EXTRACT_ARGS, &perk, &useAlt) && (thisObj->refID == 0x14) && IS_TYPE(perk, BGSPerk))
		*result = g_thePlayer->GetPerkRank(perk, useAlt != 0);
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetPerkRank_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 rank, useAlt = 0;
	if (ExtractArgs(EXTRACT_ARGS, &perk, &rank, &useAlt) && (thisObj->refID == 0x14) && IS_TYPE(perk, BGSPerk) && rank)
		g_thePlayer->SetPerkRank(perk, GetMin(rank, perk->data.numRanks), useAlt != 0);
	return true;
}

bool Cmd_GetPerkEntryCount_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	if (ExtractArgs(EXTRACT_ARGS, &perk) && IS_TYPE(perk, BGSPerk))
		*result = (int)perk->entries.Count();
	else *result = 0;
	return true;
}

bool Cmd_GetNthPerkEntryType_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSPerk *perk;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (!entry) return true;
	if IS_TYPE(entry, BGSEntryPointPerkEntry)
	{
		switch (*(UInt32*)(((BGSEntryPointPerkEntry*)entry)->data))
		{
		case kVtbl_BGSEntryPointFunctionDataOneValue:
			*result = 1;
			break;
		case kVtbl_BGSEntryPointFunctionDataTwoValue:
			*result = 2;
			break;
		case kVtbl_BGSEntryPointFunctionDataLeveledList:
			*result = 3;
			break;
		case kVtbl_BGSEntryPointFunctionDataActivateChoice:
			*result = 4;
		}
	}
	else *result = IS_TYPE(entry, BGSQuestPerkEntry) ? 6 : 5;
	return true;
}

bool Cmd_GetNthPerkEntryFunction_Execute(COMMAND_ARGS)
{
	*result = -1;
	BGSPerk *perk;
	UInt32 index;
	if (ExtractArgs(EXTRACT_ARGS, &perk, &index) && IS_TYPE(perk, BGSPerk))
	{
		BGSEntryPointPerkEntry *entry = (BGSEntryPointPerkEntry*)perk->entries.GetNthItem(index);
		if (entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			*result = entry->entryPoint;
	}
	return true;
}

bool Cmd_SetNthPerkEntryFunction_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index, func;
	if (ExtractArgs(EXTRACT_ARGS, &perk, &index, &func) && IS_TYPE(perk, BGSPerk))
	{
		BGSEntryPointPerkEntry *entry = (BGSEntryPointPerkEntry*)perk->entries.GetNthItem(index);
		if (entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			entry->entryPoint = func;
	}
	return true;
}

bool Cmd_GetNthPerkEntryForm_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSPerk *perk;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (!entry) return true;
	switch (*(UInt32*)entry)
	{
		case kVtbl_BGSEntryPointPerkEntry:
		{
			BGSEntryPointPerkEntry *perkEntry = (BGSEntryPointPerkEntry*)entry;
			if (perkEntry->data && IS_TYPE(perkEntry->data, BGSEntryPointFunctionDataLeveledList))
			{
				BGSEntryPointFunctionDataLeveledList *dataLvlList = (BGSEntryPointFunctionDataLeveledList*)perkEntry->data;
				if (dataLvlList->leveledList)
					REFR_RES = ((BGSEntryPointFunctionDataLeveledList*)perkEntry->data)->leveledList->refID;
			}
			break;
		}
		case kVtbl_BGSQuestPerkEntry:
			if (((BGSQuestPerkEntry*)entry)->quest)
				REFR_RES = ((BGSQuestPerkEntry*)entry)->quest->refID;
			break;
		case kVtbl_BGSAbilityPerkEntry:
			if (((BGSAbilityPerkEntry*)entry)->ability)
				REFR_RES = ((BGSAbilityPerkEntry*)entry)->ability->refID;
	}
	return true;
}

bool Cmd_SetNthPerkEntryForm_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	TESForm *value;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index, &value) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (!entry) return true;
	if IS_TYPE(entry, BGSQuestPerkEntry)
	{
		if IS_TYPE(value, TESQuest)
			((BGSQuestPerkEntry*)entry)->quest = (TESQuest*)value;
	}
	else if IS_TYPE(entry, BGSAbilityPerkEntry)
	{
		if IS_TYPE(value, SpellItem)
			((BGSAbilityPerkEntry*)entry)->ability = (SpellItem*)value;
	}
	return true;
}

bool Cmd_GetNthPerkEntryValue1_Execute(COMMAND_ARGS)
{
	*result = -1;
	BGSPerk *perk;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (!entry) return true;
	if IS_TYPE(entry, BGSEntryPointPerkEntry)
	{
		BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data;
		if IS_TYPE(data, BGSEntryPointFunctionDataOneValue)
			*result = ((BGSEntryPointFunctionDataOneValue*)data)->value;
		else if IS_TYPE(data, BGSEntryPointFunctionDataTwoValue)
			*result = ((BGSEntryPointFunctionDataTwoValue*)data)->value[0];
	}
	else if IS_TYPE(entry, BGSQuestPerkEntry)
		*result = ((BGSQuestPerkEntry*)entry)->stage;
	return true;
}

bool Cmd_SetNthPerkEntryValue1_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	float value;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index, &value) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (!entry) return true;
	if IS_TYPE(entry, BGSEntryPointPerkEntry)
	{
		BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data;
		if IS_TYPE(data, BGSEntryPointFunctionDataOneValue)
			((BGSEntryPointFunctionDataOneValue*)data)->value = value;
		else if IS_TYPE(data, BGSEntryPointFunctionDataTwoValue)
			((BGSEntryPointFunctionDataTwoValue*)data)->value[0] = value;
	}
	else if IS_TYPE(entry, BGSQuestPerkEntry)
		((BGSQuestPerkEntry*)entry)->stage = value;
	return true;
}

bool Cmd_GetNthPerkEntryValue2_Execute(COMMAND_ARGS)
{
	*result = -1;
	BGSPerk *perk;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
	{
		BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data;
		if IS_TYPE(data, BGSEntryPointFunctionDataTwoValue)
			*result = ((BGSEntryPointFunctionDataTwoValue*)data)->value[1];
	}
	return true;
}

bool Cmd_SetNthPerkEntryValue2_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	float value;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index, &value) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
	{
		BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data;
		if IS_TYPE(data, BGSEntryPointFunctionDataTwoValue)
			((BGSEntryPointFunctionDataTwoValue*)data)->value[1] = value;
	}
	return true;
}

bool Cmd_GetNthPerkEntryString_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	BGSPerk *perk;
	UInt32 index;
	if (ExtractArgs(EXTRACT_ARGS, &perk, &index) && IS_TYPE(perk, BGSPerk))
	{
		BGSPerkEntry *entry = perk->entries.GetNthItem(index);
		if (entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
		{
			BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data;
			if IS_TYPE(data, BGSEntryPointFunctionDataActivateChoice)
				resStr = ((BGSEntryPointFunctionDataActivateChoice*)data)->label.m_data;
		}
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetNthPerkEntryString_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &perk, &index, &s_strArgBuffer) || NOT_TYPE(perk, BGSPerk)) return true;
	BGSPerkEntry *entry = perk->entries.GetNthItem(index);
	if (!entry) return true;
	if (entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
	{
		BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data;
		if IS_TYPE(data, BGSEntryPointFunctionDataActivateChoice)
			((BGSEntryPointFunctionDataActivateChoice*)data)->label.Set(s_strArgBuffer);
	}
	return true;
}

bool Cmd_IsTrait_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	if (ExtractArgs(EXTRACT_ARGS, &perk) && IS_TYPE(perk, BGSPerk))
		*result = perk->data.isTrait;
	else *result = 0;
	return true;
}

bool Cmd_GetPerkLevel_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	if (ExtractArgs(EXTRACT_ARGS, &perk) && IS_TYPE(perk, BGSPerk))
		*result = perk->data.minLevel;
	else *result = 0;
	return true;
}

bool Cmd_SetPerkLevel_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 level;
	if (ExtractArgs(EXTRACT_ARGS, &perk, &level) && IS_TYPE(perk, BGSPerk))
		perk->data.minLevel = level;
	return true;
}