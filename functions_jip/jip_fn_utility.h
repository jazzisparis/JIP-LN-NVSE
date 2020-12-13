#pragma once

DEFINE_COMMAND_PLUGIN(RefToString, , 0, 1, kParams_JIP_OptionalForm);
DEFINE_COMMAND_PLUGIN(StringToRef, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetMinOf, , 0, 5, kParams_JIP_TwoDoubles_ThreeOptionalDoubles);
DEFINE_COMMAND_PLUGIN(GetMaxOf, , 0, 5, kParams_JIP_TwoDoubles_ThreeOptionalDoubles);
DEFINE_COMMAND_PLUGIN(ReadArrayFromFile, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(WriteArrayToFile, , 0, 3, kParams_JIP_OneString_TwoInts);
DEFINE_COMMAND_PLUGIN(ReadStringFromFile, , 0, 3, kParams_JIP_OneString_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(WriteStringToFile, , 0, 23, kParams_JIP_OneString_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetLoadOrderChanged, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ValidateModIndex, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ClearJIPSavedData, , 0, 4, kParams_JIP_FourInts);
DEFINE_COMMAND_PLUGIN(ClearModNVSEVars, , 0, 2, kParams_JIP_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(ModLogPrint, , 0, 22, kParams_JIP_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetOptionalPatch, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetOptionalPatch, , 0, 2, kParams_JIP_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(GetPluginHeaderVersion, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetIsLAA, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(Sleep, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetArrayValue, , 0, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(TF_DebugDump, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRandomInRange, , 0, 2, kParams_TwoInts);

bool Cmd_RefToString_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &form))
	{
		if (form)
		{
			if (form->GetIsReference())
				form = ((TESObjectREFR*)form)->baseForm;
		}
		else form = thisObj;
	}
	AssignString(PASS_COMMAND_ARGS, form ? form->RefToString() : NULL);
	return true;
}

bool Cmd_StringToRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer))
		REFR_RES = StringToRef(s_strArgBuffer);
	return true;
}

bool Cmd_GetMinOf_Execute(COMMAND_ARGS)
{
	UInt8 numArgs = NUM_ARGS;
	double values[5];
	if (!ExtractArgs(EXTRACT_ARGS, &values[0], &values[1], &values[2], &values[3], &values[4]))
		return true;
	double *current = values, *minVal = values;
	do
	{
		current++;
		if (*minVal > *current) minVal = current;
	}
	while (--numArgs > 1);
	*result = *minVal;
	return true;
}

bool Cmd_GetMaxOf_Execute(COMMAND_ARGS)
{
	UInt8 numArgs = NUM_ARGS;
	double values[5];
	if (!ExtractArgs(EXTRACT_ARGS, &values[0], &values[1], &values[2], &values[3], &values[4]))
		return true;
	double *current = values, *maxVal = values;
	do
	{
		current++;
		if (*maxVal < *current) maxVal = current;
	}
	while (--numArgs > 1);
	*result = *maxVal;
	return true;
}

void __fastcall CreateForType(NVSEArrayVar *arr, char *dataStr)
{
	if (*dataStr == '@')
		AppendElement(arr, ArrayElementL(LookupFormByRefID(StringToRef(dataStr + 1))));
	else if (*dataStr == '$')
		AppendElement(arr, ArrayElementL(dataStr + 1));
	else
		AppendElement(arr, ArrayElementL(StrToDbl(dataStr)));
}

bool Cmd_ReadArrayFromFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer))
		return true;
	ReplaceChr(s_strArgBuffer, '/', '\\');
	LineIterator lineIter(s_strArgBuffer, s_strValBuffer);
	if (lineIter.End()) return true;
	NVSEArrayVar *mainArr = CreateArray(NULL, 0, scriptObj);
	char *dataPtr = lineIter.Get(), *pos;
	lineIter.Next();
	if (lineIter.End())
	{
		do
		{
			pos = GetNextToken(dataPtr, '\t');
			CreateForType(mainArr, dataPtr);
			dataPtr = pos;
		}
		while (*dataPtr);
	}
	else
	{
		Vector<NVSEArrayVar*> columnList(0x10);
		NVSEArrayVar *columnArr;
		do
		{
			pos = GetNextToken(dataPtr, '\t');
			columnArr = CreateArray(NULL, 0, scriptObj);
			columnList.Append(columnArr);
			CreateForType(columnArr, dataPtr);
			AppendElement(mainArr, ArrayElementL(columnArr));
			dataPtr = pos;
		}
		while (*dataPtr);
		UInt32 numColumns = columnList.Size();
		do
		{
			dataPtr = lineIter.Get();
			lineIter.Next();
			for (UInt32 column = 0; column < numColumns; column++)
			{
				if (*dataPtr) pos = GetNextToken(dataPtr, '\t');
				CreateForType(columnList[column], dataPtr);
				dataPtr = pos;
			}
		}
		while (!lineIter.End());
	}
	if (GetArraySize(mainArr)) AssignCommandResult(mainArr, result);
	return true;
}

bool Cmd_WriteArrayToFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 apnd, arrID;
	if (!ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &apnd, &arrID))
		return true;
	NVSEArrayVar *mainArray = LookupArrayByID(arrID), *column;
	if (!mainArray) return true;
	UInt32 numLines = 1, idx, cnt;
	TempArrayElements topLine(mainArray);
	if (!topLine.size) return true;
	Vector<TempArrayElements> columnBuffer(topLine.size);
	ArrayElementR *elem;
	TempArrayElements *colElements;
	for (idx = 0; idx < topLine.size; idx++)
	{
		elem = &topLine.elements[idx];
		if (column = elem->Array())
		{
			colElements = columnBuffer.Append(column);
			if (numLines < colElements->size)
				numLines = colElements->size;
		}
		else columnBuffer.Append(elem);
	}
	FileStream outputFile;
	if (outputFile.OpenWrite(s_strArgBuffer, apnd != 0))
	{
		for (idx = 0; idx < numLines; idx++)
		{
			for (cnt = 0; cnt < topLine.size; cnt++)
			{
				if (columnBuffer[cnt].size > idx)
				{
					elem = &columnBuffer[cnt].elements[idx];
					switch (elem->GetType())
					{
						case 1:
							FltToStr(s_strValBuffer, elem->Number());
							outputFile.WriteStr(s_strValBuffer);
							break;
						case 2:
							if (elem->Form())
							{
								outputFile.WriteChar('@');
								outputFile.WriteStr(elem->Form()->RefToString());
							}
							else outputFile.WriteChar('0');
							break;
						case 3:
							outputFile.WriteChar('$');
							outputFile.WriteStr(elem->String());
							break;
						default:
							outputFile.WriteChar('0');
					}
				}
				else outputFile.WriteChar('0');
				if ((topLine.size - cnt) > 1)
					outputFile.WriteChar('\t');
			}
			if ((numLines - idx) > 1)
				outputFile.WriteChar('\n');
		}
		*result = 1;
	}
	return true;
}

bool Cmd_ReadStringFromFile_Execute(COMMAND_ARGS)
{
	UInt32 startAt = 0, lineCount = 0;
	char *resStr = s_strValBuffer;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &startAt, &lineCount))
	{
		ReplaceChr(s_strArgBuffer, '/', '\\');
		FileStream sourceFile;
		if (sourceFile.Open(s_strArgBuffer))
		{
			if (startAt) startAt--;
			char data;
			UInt32 length = 0;
			do
			{
				data = sourceFile.ReadChar();
				if (data <= 0) break;
				if (data == '\r') continue;
				if (startAt)
				{
					if (data == '\n') startAt--;
					continue;
				}
				if ((++length == kMaxMessageLength) || ((data == '\n') && !--lineCount))
					break;
				*resStr++ = data;
			}
			while (true);
		}
	}
	*resStr = 0;
	AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
	return true;
}

bool Cmd_WriteStringToFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 apnd;
	if (!ExtractFormatStringArgs(2, s_strValBuffer, EXTRACT_ARGS_EX, kCommandInfo_WriteStringToFile.numParams, &s_strArgBuffer, &apnd))
		return true;
	FileStream outputFile;
	if (outputFile.OpenWrite(s_strArgBuffer, apnd != 0))
	{
		outputFile.WriteStr(s_strValBuffer);
		*result = 1;
	}
	return true;
}

bool Cmd_GetLoadOrderChanged_Execute(COMMAND_ARGS)
{
	static char changed = -1;
	if (changed == -1)
	{
		UInt8 preloadMods = *g_numPreloadMods;
		if (preloadMods == g_dataHandler->modList.modInfoList.Count())
		{
			changed = 0;
			UInt32 modIdx, resolved;
			for (UInt8 idx = 1; idx < preloadMods; idx++)
			{
				modIdx = idx << 24;
				if (!ResolveRefID(modIdx, &resolved) || (modIdx != resolved))
				{
					changed = 1;
					break;
				}
			}
		}
		else changed = 1;
	}
	*result = changed;
	return true;
}

bool Cmd_ValidateModIndex_Execute(COMMAND_ARGS)
{
	UInt32 modIdx;
	if (ExtractArgs(EXTRACT_ARGS, &modIdx) && (modIdx <= 0xFF) && ResolveRefID(modIdx << 24, &modIdx))
	{
		modIdx >>= 24;
		*result = (int)modIdx;
	}
	else *result = -1;
	return true;
}

bool Cmd_ClearJIPSavedData_Execute(COMMAND_ARGS)
{
	UInt32 scrVars, lnkRefs, auxVars, refMaps;
	if (!ExtractArgs(EXTRACT_ARGS, &scrVars, &lnkRefs, &auxVars, &refMaps))
		return true;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	if (scrVars)
	{
		for (auto ownerIter = s_scriptVariablesBuffer.Begin(); ownerIter; ++ownerIter)
		{
			for (auto varIter = ownerIter().Begin(); varIter; ++varIter)
				if (varIter().modIdx == modIdx) varIter.Remove();
			if (ownerIter().Empty()) ownerIter.Remove();
		}
	}
	if (lnkRefs && !s_linkedRefModified.Empty())
	{
		for (auto refIter = s_linkedRefModified.Begin(); refIter; ++refIter)
			if (refIter().modIdx == modIdx) refIter.Remove();
		s_dataChangedFlags |= kChangedFlag_LinkedRefs;
	}
	if (auxVars && s_auxVariablesPerm.Erase((auxVars == 2) ? 0xFF : modIdx))
		s_dataChangedFlags |= kChangedFlag_AuxVars;
	if (refMaps && s_refMapArraysPerm.Erase((refMaps == 2) ? 0xFF : modIdx))
		s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}

bool Cmd_ClearModNVSEVars_Execute(COMMAND_ARGS)
{
	UInt32 keepArrs = 0, keepStrs = 0;
	if (!ExtractArgs(EXTRACT_ARGS, &keepArrs, &keepStrs) || s_releaseFast)
		return true;
	UInt8 modIdx = scriptObj->GetOverridingModIdx();
	NVSEVarsCollector vars(false, modIdx);
	if (!vars.GetVars()->Empty())
		vars.ClearVars();
	vars.Init(true, modIdx);
	if (!vars.GetVars()->Empty())
	{
		vars.RemoveToKeep(keepArrs);
		vars.ClearVars();
	}
	return true;
}

UnorderedSet<UInt32> s_openLogs;

bool Cmd_ModLogPrint_Execute(COMMAND_ARGS)
{
	UInt32 modIdx = scriptObj ? scriptObj->modIndex : 0xFF;
	if (modIdx == 0xFF) return true;
	UInt32 indentLevel;
	if (!ExtractFormatStringArgs(1, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_ModLogPrint.numParams, &indentLevel))
		return true;
	StrCopy(StrCopy(s_modLogPath, g_dataHandler->GetNthModName(modIdx)), ".log");
	FileStream outputFile;
	if (outputFile.OpenWrite(s_modLogPathFull, !s_openLogs.Insert(modIdx)))
	{
		if (indentLevel)
		{
			if (indentLevel >= 40)
				indentLevel = 0;
			else indentLevel = 40 - indentLevel;
			outputFile.WriteStr(kIndentLevelStr + indentLevel);
		}
		outputFile.WriteStr(s_strArgBuffer);
	}
	return true;
}

bool Cmd_GetOptionalPatch_Execute(COMMAND_ARGS)
{
	bool enabled = false;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer))
	{
		switch (s_optionalHacks.Get(s_strArgBuffer))
		{
			case 1:
				enabled = HOOK_INSTALLED(CalculateHitDamage);
				break;
			case 2:
				enabled = HOOK_INSTALLED(EnableRepairButton);
				break;
			case 3:
				enabled = s_bigGunsSkill;
				break;
			case 4:
				enabled = HOOK_INSTALLED(InitMissileFlags);
				break;
			case 5:
				enabled = HOOK_INSTALLED(UpdateTimeGlobals);
				break;
			case 6:
				enabled = s_hardcoreNeedsFix;
				break;
			case 7:
				enabled = s_failedScriptLocks;
				break;
			case 8:
				enabled = HOOK_INSTALLED(DoOperator);
				break;
			case 9:
				enabled = HOOK_INSTALLED(QttSelectInventory);
				break;
			case 10:
				enabled = HOOK_INSTALLED(RefreshHPBarDelay);
				break;
			case 11:
				enabled = HOOK_INSTALLED(DamageToWeapon);
				break;
			case 12:
				enabled = s_localizedDTDR;
				break;
			case 13:
				enabled = HOOK_INSTALLED(VoiceModulationFix);
				break;
			case 14:
				enabled = HOOK_INSTALLED(SneakBoundingBoxFix);
				break;
			case 15:
				enabled = s_NVACAlerts;
				break;
			case 16:
				enabled = HOOK_INSTALLED(GetSuitableLoadScreens);
				break;
			case 17:
				enabled = s_NPCWeaponMods;
				break;
		}
	}
	*result = enabled;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetOptionalPatch_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgs(EXTRACT_ARGS, &s_strArgBuffer, &enable))
		SetOptionalPatch(s_optionalHacks.Get(s_strArgBuffer), enable != 0);
	return true;
}

bool Cmd_GetPluginHeaderVersion_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgs(EXTRACT_ARGS, s_dataPath))
	{
		FileStream sourceFile;
		if (sourceFile.OpenAt(s_dataPathFull, 0x1E))
		{
			float version;
			sourceFile.ReadBuf(&version, 4);
			*result = version;
		}
	}
	return true;
}

bool Cmd_GetIsLAA_Execute(COMMAND_ARGS)
{
	*result = GetIsLAA();
	DoConsolePrint(result);
	return true;
}

bool Cmd_Sleep_Execute(COMMAND_ARGS)
{
	UInt32 milliseconds;
	if (ExtractArgs(EXTRACT_ARGS, &milliseconds))
		Sleep(milliseconds);
	return true;
}

bool Cmd_GetArrayValue_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 arrID;
	SInt32 index;
	if (!ExtractArgs(EXTRACT_ARGS, &arrID, &index) || !arrID)
		return true;
	ArrayElementR resElement;
	if (!GetElement((NVSEArrayVar*)arrID, ArrayElementL(index), resElement))
		return true;
	switch (resElement.GetType())
	{
		case 0:
			break;
		case 1:
			*result = resElement.num;
			break;
		case 2:
			if (resElement.form)
				REFR_RES = resElement.form->refID;
			break;
		case 3:
			break;
		case 4:
			*result = (int)resElement.raw;
			break;
	}
	return true;
}

void DumpFormData(FileStream *outputFile, TESForm *form)
{
	char *endPtr = s_strValBuffer;

	endPtr += sprintf_s(s_strValBuffer, 0x10000, "Dumping: %s [%08X]\nType: ", form->GetName(), form->refID);

	if IS_ID(form, TESQuest)
	{
		TESQuest *quest = (TESQuest*)form;

		endPtr += sprintf_s(endPtr, 0x10000, "QUEST\nIs running: %s\nCurrent stage: %d\nStages:\n", (quest->flags & 1) ? "YES" : "NO", quest->currentStage);

		ListNode<TESQuest::StageInfo> *stageIter = quest->stages.Head();
		TESQuest::StageInfo *stageInfo;
		do
		{
			stageInfo = stageIter->data;
			if (stageInfo)
				endPtr += sprintf_s(endPtr, 0x10000, "%d\t%sDONE\n", stageInfo->stage, stageInfo->isDone ? "" : "NOT ");
		}
		while (stageIter = stageIter->next);
	}
	else endPtr = StrLenCopy(endPtr, "REFERENCE", 9);

	Script *pScript;
	ScriptEventList *pEventList;
	if (form->GetScriptAndEventList(&pScript, &pEventList) && pScript->info.varCount)
	{
		endPtr += sprintf_s(endPtr, 0x10000, "\nScript: %s [%08X]\nVariables:\n", pScript->GetName(), pScript->refID);

		ListNode<VariableInfo> *varIter = pScript->varList.Head();
		VariableInfo *varInfo;
		ScriptVar *var;
		UInt32 refID;
		do
		{
			varInfo = varIter->data;
			if (!varInfo) continue;
			var = pEventList->GetVariable(varInfo->idx);
			if (!var) continue;

			endPtr = StrCopy(endPtr, varInfo->name.m_data);
			*(UInt16*)endPtr = '\t\t';
			endPtr += 2;

			if (pScript->GetVariable(varInfo->idx))
			{
				refID = *(UInt32*)&var->data;
				form = LookupFormByRefID(refID);
				if (form)
					endPtr += sprintf_s(endPtr, 0x10000, "%s [%08X]\n", form->GetName(), refID);
				else endPtr = StrLenCopy(endPtr, "0\n", 2);
			}
			else
			{
				endPtr = FltToStr(endPtr, var->data);
				*endPtr++ = '\n';
			}
		}
		while (varIter = varIter->next);
	}

	memcpy(endPtr, "------------------------------------------------\n", 50);
	outputFile->WriteStr(s_strValBuffer);
}

UInt32 (*GetFormIDFromEDID)(const char *edidStr) = NULL;

bool Cmd_TF_DebugDump_Execute(COMMAND_ARGS)
{
	if (!GetFormIDFromEDID)
	{
		HMODULE johnny = GetModuleHandle("johnnyguitar");
		if (!johnny) return true;
		GetFormIDFromEDID = (UInt32 (*)(const char*))GetProcAddress(johnny, "JGNVSE_GetFormIDFromEDID");
		if (!GetFormIDFromEDID) return true;
	}

	GetTimeStamp(StrLenCopy(s_strValBuffer, "TF Debug\\", 9));
	memcpy(s_strValBuffer + 17, ".log", 5);

	FileStream outputFile;
	if (!outputFile.Create(s_strValBuffer))
		return true;

	char *endPtr = StrLenCopy(s_strValBuffer, "Mod list:\n", 10);

	ListNode<ModInfo> *modIter = g_dataHandler->modList.modInfoList.Head();
	ModInfo *modInfo;
	UInt32 index = 0;
	do
	{
		modInfo = modIter->data;
		if (!modInfo) break;
		endPtr += sprintf_s(endPtr, 0x10000, "%02X\t%s\n", index++, modInfo->name);
	}
	while (modIter = modIter->next);

	memcpy(endPtr, "================================================\n\n", 51);
	outputFile.WriteStr(s_strValBuffer);

	LineIterator lineIter("TF Debug\\tf_debug_list.txt", s_strArgBuffer);
	TESForm *form;
	while (!lineIter.End())
	{
		form = LookupFormByRefID(GetFormIDFromEDID(lineIter.Get()));
		if (form && (IS_ID(form, TESQuest) || form->GetIsReference()))
			DumpFormData(&outputFile, form);
		lineIter.Next();
	}
	return true;
}

bool Cmd_GetRandomInRange_Execute(COMMAND_ARGS)
{
	int minVal, maxVal;
	if (ExtractArgs(EXTRACT_ARGS, &minVal, &maxVal))
		*result = GetRandomIntInRange(minVal, maxVal);
	else *result = 0;
	return true;
}