#pragma once

DEFINE_COMMAND_PLUGIN(HasPerkRank, 1, kParams_OneForm_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetPerkRank, 1, kParams_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPerkEntryCount, 0, kParams_OnePerk);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryType, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryFunction, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryFunction, 0, kParams_OnePerk_TwoInts);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryForm, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryForm, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryValue1, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryValue1, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryValue2, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryValue2, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetNthPerkEntryString, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthPerkEntryString, 0, kParams_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(IsTrait, 0, kParams_OnePerk);
DEFINE_COMMAND_PLUGIN(GetPerkLevel, 0, kParams_OnePerk);
DEFINE_COMMAND_PLUGIN(SetPerkLevel, 0, kParams_OnePerk_OneInt);

bool Cmd_HasPerkRank_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 useAlt = 0;
	if (IS_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &useAlt) && IS_ID(perk, BGSPerk))
		*result = ((Actor*)thisObj)->GetPerkRank(perk, useAlt);
	DoConsolePrint(result);
	return true;
}

__declspec(naked) bool Cmd_SetPerkRank_Execute(COMMAND_ARGS)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x1C
		mov		ecx, [ebp+0x10]
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		done
		lea		edx, [ebp-4]
		and		dword ptr [edx], 0
		push	edx
		lea		edx, [ebp-0x18]
		push	edx
		lea		edx, [ebp-0xC]
		push	edx
		push	dword ptr [ebp+0x1C]
		push	dword ptr [ebp+0x18]
		push	dword ptr [ebp+0x24]
		push	dword ptr [ebp+0xC]
		push	dword ptr [ebp+8]
		call	ExtractArgsEx
		add		esp, 0x20
		test	al, al
		jz		done
		mov		ecx, [ebp-0xC]
		cmp		byte ptr [ecx+4], kFormType_BGSPerk
		jnz		done
		mov		eax, [ebp-0x18]
		test	eax, eax
		jz		done
		movzx	edx, byte ptr [ecx+0x3A]
		cmp		eax, edx
		cmova	eax, edx
		mov		[ebp-0x18], eax
		dec		al
		mov		[ebp-0xD], al
		mov		edx, [ebp-4]
		mov		[ebp-5], dl
		push	edx
		push	ecx
		mov		ecx, [ebp+0x10]
		mov		[ebp-0x14], ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x4A0]
		mov		edx, 0x5D4EBC
		mov		ecx, 0x5D4E42
		cmp		[ebp-0x18], al
		cmovz	ecx, edx
		jmp		ecx		//	Jump to Cmd_AddPerk_Execute for compatibility with JG OnAddPerk event.
	done:
		mov		al, 1
		leave
		retn
	}
}

bool Cmd_GetPerkEntryCount_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk))
		*result = (int)perk->entries.Count();
	return true;
}

bool Cmd_GetNthPerkEntryType_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index))
			if IS_TYPE(entry, BGSEntryPointPerkEntry)
			{
				switch (VTBL(((BGSEntryPointPerkEntry*)entry)->data))
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
						break;
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index))
		if (BGSEntryPointPerkEntry *entry = (BGSEntryPointPerkEntry*)perk->entries.GetNthItem(index); entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			*result = entry->entryPoint;
	return true;
}

bool Cmd_SetNthPerkEntryFunction_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index, func;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index, &func))
		if (BGSEntryPointPerkEntry *entry = (BGSEntryPointPerkEntry*)perk->entries.GetNthItem(index); entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			entry->entryPoint = func;
	return true;
}

bool Cmd_GetNthPerkEntryForm_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index))
			switch (VTBL(entry))
			{
				case kVtbl_BGSEntryPointPerkEntry:
				{
					if (BGSEntryPointPerkEntry *perkEntry = (BGSEntryPointPerkEntry*)entry; perkEntry->data && IS_TYPE(perkEntry->data, BGSEntryPointFunctionDataLeveledList))
						if (BGSEntryPointFunctionDataLeveledList *dataLvlList = (BGSEntryPointFunctionDataLeveledList*)perkEntry->data; dataLvlList->leveledList)
							REFR_RES = ((BGSEntryPointFunctionDataLeveledList*)perkEntry->data)->leveledList->refID;
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index, &value) && IS_ID(perk, BGSPerk))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index))
			if IS_TYPE(entry, BGSQuestPerkEntry)
			{
				if IS_ID(value, TESQuest)
					((BGSQuestPerkEntry*)entry)->quest = (TESQuest*)value;
			}
			else if (IS_TYPE(entry, BGSAbilityPerkEntry) && IS_ID(value, SpellItem))
				((BGSAbilityPerkEntry*)entry)->ability = (SpellItem*)value;
	return true;
}

bool Cmd_GetNthPerkEntryValue1_Execute(COMMAND_ARGS)
{
	*result = -1;
	BGSPerk *perk;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index))
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index, &value) && IS_ID(perk, BGSPerk))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index))
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index); entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			if (BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data; IS_TYPE(data, BGSEntryPointFunctionDataTwoValue))
				*result = ((BGSEntryPointFunctionDataTwoValue*)data)->value[1];
	return true;
}

bool Cmd_SetNthPerkEntryValue2_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index, &value) && IS_ID(perk, BGSPerk))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index); entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			if (BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data; IS_TYPE(data, BGSEntryPointFunctionDataTwoValue))
				((BGSEntryPointFunctionDataTwoValue*)data)->value[1] = value;
	return true;
}

bool Cmd_GetNthPerkEntryString_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	BGSPerk *perk;
	UInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index); entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			if (BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data; IS_TYPE(data, BGSEntryPointFunctionDataActivateChoice))
				resStr = ((BGSEntryPointFunctionDataActivateChoice*)data)->label.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetNthPerkEntryString_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 index;
	char labelStr[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &index, &labelStr) && IS_ID(perk, BGSPerk))
		if (BGSPerkEntry *entry = perk->entries.GetNthItem(index); entry && IS_TYPE(entry, BGSEntryPointPerkEntry))
			if (BGSEntryPointFunctionData *data = ((BGSEntryPointPerkEntry*)entry)->data; IS_TYPE(data, BGSEntryPointFunctionDataActivateChoice))
				((BGSEntryPointFunctionDataActivateChoice*)data)->label.Set(labelStr);
	return true;
}

bool Cmd_IsTrait_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk) && perk->data.isTrait)
		*result = 1;
	return true;
}

bool Cmd_GetPerkLevel_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk))
		*result = perk->data.minLevel;
	return true;
}

bool Cmd_SetPerkLevel_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 level;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &level))
		perk->data.minLevel = level;
	return true;
}