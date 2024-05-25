#pragma once

DEFINE_COMMAND_PLUGIN(HasVariableAdded, 0, kParams_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(AddScriptVariable, 0, kParams_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(RemoveScriptVariable, 0, kParams_OneString_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(RemoveAllAddedVariables, 0, kParams_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(GetDelayElapsed, 0, kParams_OneQuest);
DEFINE_COMMAND_PLUGIN(SetDelayElapsed, 0, kParams_OneQuest_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(SetGameMainLoopCallback, SGMLC, 0, kParams_OneForm_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(GetScriptDisabled, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetScriptDisabled, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetScriptEventDisabled, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetScriptEventDisabled, 0, kParams_OneForm_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(FakeScriptEvent, 1, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetOnQuestStageEventHandler, 0, kParams_OneForm_OneInt_OneQuest_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RunScriptSnippet, 0, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(ScriptWait, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(IsScriptWaiting, 0, kParams_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(StopScriptWaiting, 0, kParams_OneOptionalQuest);
DEFINE_COMMAND_PLUGIN(GetScriptBlockDisabled, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetScriptBlockDisabled, 0, kParams_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(HasScriptBlock, 0, kParams_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(DisableScriptedActivate, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(RunBatchScript, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ExecuteScript, 1, nullptr);
DEFINE_COMMAND_PLUGIN(RegisterSRScript, 0, kParams_OneString);

bool ScriptVariableAction_Execute(COMMAND_ARGS)
{
	CAPTURE_CL(actionType)
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form) && varName[0])
	{
		if (!form)
		{
			if (!thisObj) return true;
			form = thisObj;
		}
		if (actionType == 2)
		{
			if (auto findOwner = s_scriptVariablesBuffer->Find(form->refID))
				if (auto findVar = findOwner().Find(varName))
				{
					findVar.Remove();
					if (findOwner().Empty()) findOwner.Remove();
					*result = 1;
				}
			return true;
		}
		Script *pScript;
		ScriptLocals *pEventList;
		if (form->GetScriptAndEventList(&pScript, &pEventList))
			if (actionType)
			{
				if (pScript->AddVariable(varName, pEventList, form->refID, scriptObj->GetOverridingModIdx()))
					*result = 1;
			}
			else if (GetVariableAdded(pScript->refID, varName))
				if (auto findOwner = s_scriptVariablesBuffer->Find(form->refID); findOwner && findOwner().HasKey(varName))
					if (VariableInfo *varInfo = pScript->GetVariableByName(varName); varInfo && pEventList->GetVariable(varInfo->idx))
						*result = 1;
	}
	return true;
}

__declspec(naked) bool Cmd_HasVariableAdded_Execute(COMMAND_ARGS)
{
	__asm
	{
		xor		cl, cl
		jmp		ScriptVariableAction_Execute
	}
}

__declspec(naked) bool Cmd_AddScriptVariable_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		cl, 1
		jmp		ScriptVariableAction_Execute
	}
}

__declspec(naked) bool Cmd_RemoveScriptVariable_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		cl, 2
		jmp		ScriptVariableAction_Execute
	}
}

bool Cmd_RemoveAllAddedVariables_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		if (!form)
		{
			if (!thisObj) return true;
			form = thisObj;
		}
		if (auto findOwner = s_scriptVariablesBuffer->Find(form->refID))
		{
			UInt8 modIdx = scriptObj->GetOverridingModIdx();
			for (auto varIter = findOwner().Begin(); varIter; ++varIter)
				if (varIter().modIdx == modIdx) varIter.Remove();
			if (findOwner().Empty()) findOwner.Remove();
		}
	}
	return true;
}

bool Cmd_GetDelayElapsed_Execute(COMMAND_ARGS)
{
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
	UInt8 numArgs = NUM_ARGS;
	Script *script;
	UInt32 doAdd, callDelay = 1, modeFlag = 3;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &doAdd, &callDelay, &modeFlag) && IS_ID(script, Script))
	{
		TESObjectREFR *callingRef = thisObj ? thisObj : g_thePlayer;
		MainLoopCallback *callback = FindMainLoopCallback(script, callingRef);
		if (numArgs < 2)
		{
			if (callback)
				*result = 1;
		}
		else if (doAdd)
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
				callback->args[0] = callingRef->refID;
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
	return true;
}

void __fastcall SetScriptDisabled(Script *script, bool disable)
{
	if (disable != !script->info.dataLength)
	{
		script->info.dataLength = disable ? 0 : script->GetDataLength();
		if (!script->IsQuestScript()) HOOK_MOD(MergeEventMask, disable);
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
		auto iter = tempList.Head();
		do
		{
			if (form = iter->data; form && IS_ID(form, Script))
				SetScriptDisabled((Script*)form, bDisable);
		}
		while (iter = iter->next);
	}
	return true;
}

bool Cmd_GetScriptEventDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form;
	char evtName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &evtName))
		if (UInt32 *inMask = s_eventMasks->GetPtr(evtName))
			if (*inMask)
			{
				if ((form->jipFormFlags6 & kHookFormFlag6_EventDisabled) && (s_disabledEventsMap->Get(form) & *inMask))
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
			if (s_disabledEventsMap->InsertKey(form, &evntMask))
			{
				form->SetJIPFlag(kHookFormFlag6_EventDisabled, true);
				HOOK_INC(MarkScriptEvent);
			}
			*evntMask |= inMask;
		}
		else if (form->jipFormFlags6 & kHookFormFlag6_EventDisabled)
		{
			evntMask = s_disabledEventsMap->GetPtr(form);
			if (evntMask && !(*evntMask &= ~inMask))
			{
				s_disabledEventsMap->Erase(form);
				form->SetJIPFlag(kHookFormFlag6_EventDisabled, false);
				HOOK_DEC(MarkScriptEvent);
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
		evntMask = s_eventMasks->GetPtr(posPtr);
		posPtr = delim;
		if (!evntMask) continue;
		inMask |= *evntMask;
		if (!*evntMask) onActivate = true;
	}
	while (*posPtr);
	if (!inMask && !onActivate)
		return true;
	tList<TESForm> tempList(form);
	if IS_ID(form, BGSListForm)
		tempList = ((BGSListForm*)form)->list;
	auto iter = tempList.Head();
	do
	{
		if (form = iter->data; form && NOT_ID(form, TESQuest))
		{
			if IS_REFERENCE(form)
			{
				TESForm *refBase = ((TESObjectREFR*)form)->baseForm;
				if (kInventoryType[refBase->typeID] || !refBase->HasScript())
					continue;
			}
			else if (!form->HasScript()) continue;
			SetScriptEventDisabled(form, inMask, onActivate, bDisable);
		}
	}
	while (iter = iter->next);
	return true;
}

bool Cmd_FakeScriptEvent_Execute(COMMAND_ARGS)
{
	char evtName[0x40];
	TESForm *filterForm = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &evtName, &filterForm))
		if (UInt32 inMask = s_eventMasks->Get(evtName))
		{
			InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
			if (ExtraDataList *xData = invRef ? invRef->xData : &thisObj->extraDataList)
				if (auto xScript = GetExtraType(xData, ExtraScript); xScript && xScript->eventList && ThisCall<bool>(0x5A8E20, xScript->eventList, filterForm, inMask))
					*result = 1;
		}
	return true;
}

bool Cmd_SetOnQuestStageEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt, stageID, skipRes = 0;
	TESQuest *quest;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &quest, &stageID, &skipRes) || NOT_ID(script, Script))
		return true;
	TESQuest::StageInfo *info = NULL;
	auto iter = quest->stages.Head();
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
		if (s_questStageEventMap->Insert(quest, &callbacks))
		{
			quest->SetJIPFlag(kHookFormFlag6_SetStageHandlers, true);
			HOOK_INC(SetQuestStage);
		}
		else if (callbacks->Find(QuestStageEventFinder(pCallback)))
			return true;
		*callbacks->Append() = pCallback;
		*result = 1;
	}
	else if (auto findQuest = s_questStageEventMap->Find(quest); findQuest && findQuest().Remove(QuestStageEventFinder(pCallback)))
	{
		if (findQuest().Empty())
		{
			findQuest.Remove();
			quest->SetJIPFlag(kHookFormFlag6_SetStageHandlers, false);
			HOOK_DEC(SetQuestStage);
		}
		*result = 1;
	}
	return true;
}

bool Cmd_RunScriptSnippet_Execute(COMMAND_ARGS)
{
	UInt32 delayTime;
	char *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_RunScriptSnippet.numParams, &delayTime))
	{
		ReplaceChr(buffer, '\n', '\r');
		if (Script *pScript = Script::Create(buffer, "RunScriptSnippet"))
		{
			TESObjectREFR *callingRef = thisObj ? thisObj : g_thePlayer;
			if (delayTime)
				MainLoopAddCallbackArgsEx(JIPScriptRunner::RunScript, pScript, 1, delayTime, 1, callingRef);
			else
			{
				JIPScriptRunner::RunScript(pScript, 0, callingRef);
				pScript->Destroy(1);
			}
		}
	}
	return true;
}

bool Cmd_ScriptWait_Execute(COMMAND_ARGS)
{
	if (_ReturnAddress() != (void*)0x5E234B) return true;
	UInt32 iterNum;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &iterNum) || !scriptObj->refID || scriptObj->IsMagicScript() || !iterNum)
		return true;
	TESForm *owner = scriptObj->IsQuestScript() ? scriptObj->quest : (TESForm*)thisObj;
	if (!owner) return true;
	*opcodeOffsetPtr += *(UInt16*)(scriptData + *opcodeOffsetPtr - 2);
	ScriptBlockIterator blockIter(scriptData, *opcodeOffsetPtr);
	while (blockIter) ++blockIter;
	UInt8 *blockType = blockIter.TypePtr();
	if (!blockType || (*blockType == 0xD))
		return true;
	s_scriptWaitInfoMap()[owner].Init(owner, iterNum, blockIter.NextOpOffset(), opcodeOffsetPtr);
	owner->jipFormFlags5 |= kHookFormFlag5_ScriptOnWait;
	HOOK_SET(ScriptWait, true);
	UInt32 *callerArgs = opcodeOffsetPtr + 6;
	*(UInt32*)callerArgs[3] = blockIter.NextBlockOffset() - 4 - callerArgs[4];
	return true;
}

bool Cmd_IsScriptWaiting_Execute(COMMAND_ARGS)
{
	TESForm *owner = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &owner))
	{
		if (!owner)
		{
			if (!thisObj) return true;
			owner = thisObj;
		}
		if (owner->jipFormFlags5 & kHookFormFlag5_ScriptOnWait)
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
		if (owner->jipFormFlags5 & kHookFormFlag5_ScriptOnWait)
			if (ScriptWaitInfo *waitInfo = s_scriptWaitInfoMap->GetPtr(owner))
				waitInfo->iterNum = 1;
	}
	return true;
}

bool Cmd_GetScriptBlockDisabled_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 blockType;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &blockType) && IS_ID(script, Script) && (blockType <= 0x25))
		if (auto findScript = s_disabledScriptBlocksMap->Find(script); findScript && findScript().Find(DisabledBlockFinder(blockType)))
			*result = 1;
	return true;
}

bool Cmd_SetScriptBlockDisabled_Execute(COMMAND_ARGS)
{
	Script *script;
	SInt32 disable, blockType = -1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &disable, &blockType) || NOT_ID(script, Script) || (blockType > 0x25)) return true;
	auto findScript = s_disabledScriptBlocksMap->Find(script);
	DisabledScriptBlocks *disabledBlocks = findScript ? &findScript() : NULL;
	if (disable)
	{
		if ((blockType < 0) || (disabledBlocks && disabledBlocks->Find(DisabledBlockFinder(blockType)))) return true;
		UInt8 *typePtr;
		for (ScriptBlockIterator blockIter(script->data, script->info.dataLength); blockIter; ++blockIter)
		{
			typePtr = blockIter.TypePtr();
			if (*typePtr != blockType) continue;
			if (!disabledBlocks) disabledBlocks = &s_disabledScriptBlocksMap()[script];
			disabledBlocks->Append(typePtr);
		}
	}
	else if ((blockType < 0) || (disabledBlocks && disabledBlocks->Remove(DisabledBlockFinder(blockType)) && disabledBlocks->Empty()))
		findScript.Remove();
	return true;
}

bool Cmd_HasScriptBlock_Execute(COMMAND_ARGS)
{
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
		TESScriptableForm *scriptable = form->GetScriptableForm();
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
	ExtraScript *xScript = GetExtraType(&thisObj->extraDataList, ExtraScript);
	if (!xScript || !xScript->script || !xScript->eventList) return true;
	Script *script = xScript->script;
	UInt8 *dataPtr = script->data, *endPtr = dataPtr + script->info.dataLength;
	dataPtr += 4;
	UInt16 lookFor = disable ? 0x100D : 0x2215, replace = disable ? 0x2215 : 0x100D, *opcodePtr, length;
	bool onActivate = false;
	while (dataPtr < endPtr)
	{
		opcodePtr = (UInt16*)dataPtr;
		dataPtr += 2;
		length = *(UInt16*)dataPtr;
		dataPtr += 2;
		if (*opcodePtr == 0x11)
		{
			if (onActivate) break;
		}
		else if (*opcodePtr == 0x10)
			onActivate = (*(UInt16*)dataPtr == 2);
		else
		{
			if (*opcodePtr == 0x1C)
			{
				opcodePtr = (UInt16*)dataPtr;
				dataPtr += 2;
				length = *(UInt16*)dataPtr;
				dataPtr += 2;
			}
			if (onActivate && (*opcodePtr == lookFor))
				*opcodePtr = replace;
		}
		dataPtr += length;
	}
	return true;
}

bool Cmd_RunBatchScript_Execute(COMMAND_ARGS)
{
	char filePath[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &filePath))
	{
		ReplaceChr(filePath, '/', '\\');
		char *fileName = FindChrR(filePath, '\\');
		if (fileName) fileName++;
		else fileName = filePath;
		auto cachedScript = s_cachedScripts->GetPtr(fileName);
		if (!cachedScript)
			if (char *buffer = GetStrArgBuffer(); FileToBuffer(filePath, buffer, STR_BUFFER_SIZE - 1))
				if (Script *pScript = Script::Create(buffer, fileName))
				{
					cachedScript = s_cachedScripts->Emplace(fileName, pScript);
					if (UInt32 modIdx = scriptObj->GetOverridingModIdx(); modIdx < 0xFF)
						pScript->mods.Init(g_dataHandler->GetNthModInfo(modIdx));
				}
		if (cachedScript)
		{
			JIPScriptRunner::RunScript(cachedScript->script, 0, thisObj ? thisObj : g_thePlayer);
			*result = 1;
		}
	}
	return true;
}

bool Cmd_ExecuteScript_Execute(COMMAND_ARGS)
{
	if (auto xScript = GetExtraType(&thisObj->extraDataList, ExtraScript); xScript && xScript->script && xScript->eventList)
		xScript->script->Execute(thisObj, xScript->eventList, nullptr, false);
	return true;
}

bool Cmd_RegisterSRScript_Execute(COMMAND_ARGS)
{
	char filePath[0x80];
	if (JIPScriptRunner::initInProgress && ExtractArgsEx(EXTRACT_ARGS_EX, &filePath) && JIPScriptRunner::RegisterScript(filePath))
		*result = 1;
	return true;
}