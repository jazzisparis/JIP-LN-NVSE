#pragma once

DEFINE_COMMAND_PLUGIN(HasVariableAdded, , 0, 2, kParams_JIP_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(AddScriptVariable, , 0, 2, kParams_JIP_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(RemoveScriptVariable, , 0, 2, kParams_JIP_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(RemoveAllAddedVariables, , 0, 1, kParams_JIP_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(GetDelayElapsed, , 0, 1, kParams_OneQuest);
DEFINE_COMMAND_PLUGIN(SetDelayElapsed, , 0, 2, kParams_JIP_OneQuest_OneFloat);
DEFINE_COMMAND_PLUGIN(SetGameMainLoopCallback, , 0, 4, kParams_JIP_OneForm_OneInt_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetScriptDisabled, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetScriptDisabled, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetScriptEventDisabled, , 0, 2, kParams_JIP_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetScriptEventDisabled, , 0, 3, kParams_JIP_OneForm_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(FakeScriptEvent, , 1, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetOnQuestStageEventHandler, , 0, 5, kParams_JIP_OneForm_OneInt_OneQuest_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RunScriptSnippet, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(ScriptWait, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(IsScriptWaiting, , 0, 1, kParams_JIP_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(StopScriptWaiting, , 0, 1, kParams_JIP_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(GetScriptBlockDisabled, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetScriptBlockDisabled, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(HasScriptBlock, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(DisableScriptedActivate, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(RunBatchScript, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ExecuteScript, , 1, 0, NULL);

UInt8 s_scriptVarActionType = 0;

bool ScriptVariableAction_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form) || !varName[0]) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj;
	}
	if (s_scriptVarActionType == 2)
	{
		auto findOwner = s_scriptVariablesBuffer.Find(form->refID);
		if (findOwner)
		{
			auto findVar = findOwner().Find(varName);
			if (findVar)
			{
				findVar.Remove();
				if (findOwner().Empty()) findOwner.Remove();
				*result = 1;
			}
		}
		return true;
	}
	Script *pScript;
	ScriptEventList *pEventList;
	if (!form->GetScriptAndEventList(&pScript, &pEventList)) return true;
	if (s_scriptVarActionType)
	{
		if (pScript->AddVariable(varName, pEventList, form->refID, scriptObj->GetOverridingModIdx()))
			*result = 1;
	}
	else if (GetVariableAdded(pScript->refID, varName))
	{
		auto findOwner = s_scriptVariablesBuffer.Find(form->refID);
		if (findOwner && findOwner().HasKey(varName))
		{
			VariableInfo *varInfo = pScript->GetVariableByName(varName);
			if (varInfo && pEventList->GetVariable(varInfo->idx)) *result = 1;
		}
	}
	return true;
}

__declspec(naked) bool Cmd_HasVariableAdded_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_scriptVarActionType, 0
		jmp		ScriptVariableAction_Execute
	}
}

__declspec(naked) bool Cmd_AddScriptVariable_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_scriptVarActionType, 1
		jmp		ScriptVariableAction_Execute
	}
}

__declspec(naked) bool Cmd_RemoveScriptVariable_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_scriptVarActionType, 2
		jmp		ScriptVariableAction_Execute
	}
}

bool Cmd_RemoveAllAddedVariables_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj;
	}
	auto findOwner = s_scriptVariablesBuffer.Find(form->refID);
	if (!findOwner) return true;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	for (auto varIter = findOwner().Begin(); varIter; ++varIter)
		if (varIter().modIdx == modIdx) varIter.Remove();
	if (findOwner().Empty()) findOwner.Remove();
	return true;
}

bool Cmd_GetDelayElapsed_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESQuest *quest;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest) && quest->scriptable.script)
		*result = quest->scriptable.script->questDelayTimeCounter;
	return true;
}

bool Cmd_SetDelayElapsed_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	float delay;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &delay) && quest->scriptable.script)
		quest->scriptable.script->questDelayTimeCounter = delay;
	return true;
}

bool Cmd_SetGameMainLoopCallback_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 doAdd, callDelay = 1, modeFlag = 3;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &doAdd, &callDelay, &modeFlag) && IS_ID(script, Script))
	{
		TESObjectREFR *callingRef = thisObj ? thisObj : g_thePlayer;
		MainLoopCallback *callback = FindMainLoopCallback(script, callingRef);
		if (doAdd)
		{
			if (!callDelay) callDelay = 1;
			if (callback)
			{
				callback->bRemove = false;
				callback->callDelay = callDelay;
			}
			else
			{
				callback = MainLoopCallback::Create(script, callingRef, 0, callDelay);
				callback->isScript = true;
				callback->arg.uVal = callingRef->refID;
			}
			callback->flags = (modeFlag & 0xB);
		}
		else if (callback)
			callback->bRemove = true;
	}
	return true;
}

bool Cmd_GetScriptDisabled_Execute(COMMAND_ARGS)
{
	Script *script;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script) && IS_ID(script, Script) && !script->info.dataLength)
		*result = 1;
	else *result = 0;
	return true;
}

void __fastcall SetScriptDisabled(Script *script, bool disable)
{
	if (disable != !script->info.dataLength)
	{
		script->info.dataLength = disable ? 0 : script->GetDataLength();
		if (script->info.type != 1) HOOK_MOD(MergeEventMask, disable);
	}
}

bool Cmd_SetScriptDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 disable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &disable))
	{
		bool bDisable = (disable != 0);
		tList<TESForm> tempList(form);
		if IS_ID(form, BGSListForm)
			tempList = ((BGSListForm*)form)->list;
		ListNode<TESForm> *iter = tempList.Head();
		do
		{
			form = iter->data;
			if (form && IS_ID(form, Script))
				SetScriptDisabled((Script*)form, bDisable);
		}
		while (iter = iter->next);
	}
	return true;
}

bool Cmd_GetScriptEventDisabled_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	char evtName[0x40];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &evtName)) return true;
	UInt32 *inMask = s_eventMasks.GetPtr(evtName);
	if (!inMask) return true;
	if (*inMask)
	{
		if ((form->jipFormFlags6 & kHookFormFlag6_EventDisabled) && (s_disabledEventsMap.Get(form) & *inMask))
			*result = 1;
	}
	else if (form->jipFormFlags6 & kHookFormFlag6_ActivateDisabled)
		*result = 1;
	return true;
}

void __fastcall SetScriptEventDisabled(TESForm *form, UInt32 inMask, bool onActivate, bool bDisable)
{
	if (inMask)
	{
		UInt32 *evntMask;
		if (bDisable)
		{
			if (s_disabledEventsMap.Insert(form, &evntMask))
			{
				form->SetJIPFlag(kHookFormFlag6_EventDisabled, true);
				HOOK_MOD(MarkScriptEvent, true);
			}
			*evntMask |= inMask;
		}
		else if (form->jipFormFlags6 & kHookFormFlag6_EventDisabled)
		{
			evntMask = s_disabledEventsMap.GetPtr(form);
			if (evntMask && !(*evntMask &= ~inMask))
			{
				s_disabledEventsMap.Erase(form);
				form->SetJIPFlag(kHookFormFlag6_EventDisabled, false);
				HOOK_MOD(MarkScriptEvent, false);
			}
		}
	}
	if (onActivate && (bDisable == !(form->jipFormFlags6 & kHookFormFlag6_ActivateDisabled)))
	{
		form->SetJIPFlag(kHookFormFlag6_ActivateDisabled, bDisable);
		HOOK_MOD(DoActivate, bDisable);
	}
}

bool Cmd_SetScriptEventDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form;
	char evtName[0x40];
	UInt32 disable;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &evtName, &disable)) return true;
	bool bDisable = (disable != 0), onActivate = false;
	char *posPtr = evtName, *delim;
	UInt32 *evntMask, inMask = 0;
	do
	{
		delim = GetNextToken(posPtr, ' ');
		evntMask = s_eventMasks.GetPtr(posPtr);
		posPtr = delim;
		if (!evntMask) continue;
		inMask |= *evntMask;
		if (!*evntMask) onActivate = true;
	}
	while (*posPtr);
	if (!inMask && !onActivate) return true;
	tList<TESForm> tempList(form);
	if IS_ID(form, BGSListForm)
		tempList = ((BGSListForm*)form)->list;
	ListNode<TESForm> *iter = tempList.Head();
	TESForm *refBase;
	do
	{
		form = iter->data;
		if (!form || IS_ID(form, TESQuest)) continue;
		if IS_REFERENCE(form)
		{
			refBase = ((TESObjectREFR*)form)->baseForm;
			if (kInventoryType[refBase->typeID] || !refBase->HasScript()) continue;
		}
		else if (!form->HasScript()) continue;
		SetScriptEventDisabled(form, inMask, onActivate, bDisable);
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_FakeScriptEvent_Execute(COMMAND_ARGS)
{
	*result = 0;
	char evtName[0x40];
	TESForm *filterForm = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &evtName, &filterForm)) return true;
	UInt32 inMask = s_eventMasks.Get(evtName);
	if (!inMask) return true;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	ExtraDataList *xData = invRef ? invRef->xData : &thisObj->extraDataList;
	if (!xData) return true;
	ExtraScript *xScript = GetExtraType(xData, Script);
	if (xScript && xScript->eventList)
		*result = ThisCall<bool>(0x5A8E20, xScript->eventList, filterForm, inMask);
	return true;
}

bool Cmd_SetOnQuestStageEventHandler_Execute(COMMAND_ARGS)
{
	*result = 0;
	Script *script;
	UInt32 addEvnt, stageID, skipRes = 0;
	TESQuest *quest;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &quest, &stageID, &skipRes) || NOT_ID(script, Script))
		return true;
	TESQuest::StageInfo *info = NULL;
	ListNode<TESQuest::StageInfo> *iter = quest->stages.Head();
	do
	{
		if (!iter->data || (iter->data->stage != stageID))
			continue;
		info = iter->data;
		break;
	}
	while (iter = iter->next);
	if (!info) return true;
	QuestStageEventCallback pCallback(info->stage, skipRes ? 2 : 1, script);
	if (addEvnt)
	{
		QuestStageCallbacks *callbacks;
		if (s_questStageEventMap.Insert(quest, &callbacks))
		{
			quest->SetJIPFlag(kHookFormFlag6_SetStageHandlers, true);
			HOOK_MOD(SetQuestStage, true);
		}
		else if (callbacks->Find(QuestStageEventFinder(pCallback)))
			return true;
		callbacks->Append(pCallback);
		*result = 1;
	}
	else
	{
		auto findQuest = s_questStageEventMap.Find(quest);
		if (!findQuest || !findQuest().Remove(QuestStageEventFinder(pCallback)))
			return true;
		*result = 1;
		if (findQuest().Empty())
		{
			findQuest.Remove();
			quest->SetJIPFlag(kHookFormFlag6_SetStageHandlers, false);
			HOOK_MOD(SetQuestStage, false);
		}
	}
	return true;
}

bool Cmd_RunScriptSnippet_Execute(COMMAND_ARGS)
{
	UInt32 delayTime;
	if (ExtractFormatStringArgs(1, s_strValBuffer, EXTRACT_ARGS_EX, kCommandInfo_RunScriptSnippet.numParams, &delayTime))
	{
		TESObjectREFR *callingRef = thisObj ? thisObj : g_thePlayer;
		ReplaceChr(s_strValBuffer, '\n', '\r');
		if (delayTime)
		{
			bool (TESObjectREFR::*FuncPtr)(const char*) = &TESObjectREFR::RunScriptSource;
			MainLoopAddCallbackArgsEx((void*&)FuncPtr, callingRef, 1, delayTime, 1, CopyString(s_strValBuffer));
		}
		else callingRef->RunScriptSource(s_strValBuffer);
	}
	return true;
}

bool Cmd_ScriptWait_Execute(COMMAND_ARGS)
{
	if (_ReturnAddress() != (void*)0x5E234B) return true;
	UInt32 iterNum;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &iterNum) || !scriptObj->refID || (scriptObj->info.type > 1) || !iterNum) return true;
	TESForm *owner = scriptObj->info.type ? scriptObj->quest : (TESForm*)thisObj;
	if (!owner) return true;
	*opcodeOffsetPtr += *(UInt16*)(scriptData + *opcodeOffsetPtr - 2);
	ScriptBlockIterator blockIter(scriptData, *opcodeOffsetPtr);
	while (blockIter) ++blockIter;
	UInt8 *blockType = blockIter.TypePtr();
	if (!blockType || (*blockType == 0xD)) return true;
	ScriptWaitInfo *waitInfo;
	if (s_scriptWaitInfoMap.Insert(owner->refID, &waitInfo))
	{
		owner->jipFormFlags5 |= kHookFormFlag5_ScriptOnWait;
		HOOK_MOD(ScriptRunner, true);
		HOOK_MOD(EvalEventBlock, true);
	}
	waitInfo->Init(owner, iterNum, blockIter.NextOpOffset(), opcodeOffsetPtr);
	UInt32 *callerArgs = opcodeOffsetPtr + 6;
	*(UInt32*)callerArgs[3] = blockIter.NextBlockOffset() - 4 - callerArgs[4];
	return true;
}

bool Cmd_IsScriptWaiting_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *owner = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &owner))
	{
		if (!owner)
		{
			if (!thisObj) return true;
			owner = thisObj;
		}
		if (s_scriptWaitInfoMap.HasKey(owner->refID))
			*result = 1;
	}
	return true;
}

bool Cmd_StopScriptWaiting_Execute(COMMAND_ARGS)
{
	TESForm *owner = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &owner))
	{
		if (!owner)
		{
			if (!thisObj) return true;
			owner = thisObj;
		}
		ScriptWaitInfo *waitInfo = s_scriptWaitInfoMap.GetPtr(owner->refID);
		if (waitInfo) waitInfo->iterNum = 1;
	}
	return true;
}

bool Cmd_GetScriptBlockDisabled_Execute(COMMAND_ARGS)
{
	*result = 0;
	Script *script;
	UInt32 blockType;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &blockType) && IS_ID(script, Script) && (blockType <= 0x25))
	{
		auto findScript = s_disabledScriptBlocksMap.Find(script);
		if (findScript && findScript().Find(DisabledBlockFinder(blockType)))
			*result = 1;
	}
	return true;
}

bool Cmd_SetScriptBlockDisabled_Execute(COMMAND_ARGS)
{
	Script *script;
	SInt32 disable, blockType = -1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &disable, &blockType) || NOT_ID(script, Script) || (blockType > 0x25)) return true;
	auto findScript = s_disabledScriptBlocksMap.Find(script);
	DisabledScriptBlocks *disabledBlocks = findScript ? &findScript() : NULL;
	if (disable)
	{
		if ((blockType < 0) || (disabledBlocks && disabledBlocks->Find(DisabledBlockFinder(blockType)))) return true;
		UInt8 *typePtr;
		for (ScriptBlockIterator blockIter(script->data, script->info.dataLength); blockIter; ++blockIter)
		{
			typePtr = blockIter.TypePtr();
			if (*typePtr != blockType) continue;
			if (!disabledBlocks) disabledBlocks = &s_disabledScriptBlocksMap[script];
			disabledBlocks->Append(typePtr);
		}
	}
	else if ((blockType < 0) || (disabledBlocks && disabledBlocks->Remove(DisabledBlockFinder(blockType)) && disabledBlocks->Empty()))
		findScript.Remove();
	return true;
}

bool Cmd_HasScriptBlock_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 blockType;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &blockType, &form) || (blockType > 0x25)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	Script *script;
	if IS_ID(form, Script)
		script = (Script*)form;
	else
	{
		TESScriptableForm *scriptable = DYNAMIC_CAST(form, TESForm, TESScriptableForm);
		script = scriptable ? scriptable->script : NULL;
		if (!script) return true;
	}
	UInt8 *typePtr;
	for (ScriptBlockIterator blockIter(script->data, script->info.dataLength); blockIter; ++blockIter)
	{
		typePtr = blockIter.TypePtr();
		if (*typePtr == blockType)
		{
			*result = 1;
			break;
		}
	}
	return true;
}

bool Cmd_DisableScriptedActivate_Execute(COMMAND_ARGS)
{
	UInt32 disable;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &disable)) return true;
	ExtraScript *xScript = GetExtraType(&thisObj->extraDataList, Script);
	if (!xScript || !xScript->script || !xScript->eventList) return true;
	Script *script = xScript->script;
	UInt8 *dataPtr = script->data, *endPtr = dataPtr + script->info.dataLength;
	dataPtr += 4;
	UInt16 lookFor = disable ? 0x100D : 0x2210, replace = disable ? 0x2210 : 0x100D, *opcode, length;
	while (dataPtr < endPtr)
	{
		opcode = (UInt16*)dataPtr;
		dataPtr += 2;
		length = *(UInt16*)dataPtr;
		dataPtr += 2;
		if (*opcode == 0x10)
		{
			if (*(UInt16*)dataPtr != 2)
			{
				dataPtr += 2;
				length = *(UInt16*)dataPtr + 6;
			}
		}
		else
		{
			if (*opcode == 0x1C)
			{
				opcode = (UInt16*)dataPtr;
				dataPtr += 2;
				length = *(UInt16*)dataPtr;
				dataPtr += 2;
			}
			if (*opcode == lookFor)
				*opcode = replace;
		}
		dataPtr += length;
	}
	return true;
}

bool Cmd_RunBatchScript_Execute(COMMAND_ARGS)
{
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		TESObjectREFR *callingRef = thisObj ? thisObj : g_thePlayer;
		*result = FileToBuffer(s_strArgBuffer, s_strValBuffer) && callingRef->RunScriptSource(s_strValBuffer);
	}
	else *result = 0;
	return true;
}

bool Cmd_ExecuteScript_Execute(COMMAND_ARGS)
{
	ExtraScript *xScript = GetExtraType(&thisObj->extraDataList, Script);
	if (xScript)
	{
		Script *pScript = xScript->script;
		ScriptEventList *pEventList = xScript->eventList;
		if (pScript && pEventList)
			CALL_MEMBER_FN(pScript, Execute)(thisObj, pEventList, NULL, false);
	}
	return true;
}