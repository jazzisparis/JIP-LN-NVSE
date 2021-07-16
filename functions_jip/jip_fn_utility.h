#pragma once

DEFINE_COMMAND_PLUGIN(RefToString, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(StringToRef, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetMinOf, , 0, 5, kParams_JIP_TwoDoubles_ThreeOptionalDoubles);
DEFINE_COMMAND_PLUGIN(GetMaxOf, , 0, 5, kParams_JIP_TwoDoubles_ThreeOptionalDoubles);
DEFINE_COMMAND_PLUGIN(ReadArrayFromFile, , 0, 2, kParams_JIP_OneString_OneOptionalInt);
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
DEFINE_COMMAND_PLUGIN(GetRandomInRange, , 0, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetSessionTime, , 0, 0, NULL);

bool Cmd_RefToString_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (!NUM_ARGS)
		form = thisObj;
	else if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IS_REFERENCE(form))
		form = ((TESObjectREFR*)form)->baseForm;
	AssignString(PASS_COMMAND_ARGS, form ? form->RefToString() : NULL);
	return true;
}

bool Cmd_StringToRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	char refStr[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refStr))
		REFR_RES = StringToRef(refStr);
	return true;
}

bool Cmd_GetMinOf_Execute(COMMAND_ARGS)
{
	UInt8 numArgs = NUM_ARGS;
	double values[5];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &values[0], &values[1], &values[2], &values[3], &values[4]))
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
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &values[0], &values[1], &values[2], &values[3], &values[4]))
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

bool Cmd_ReadArrayFromFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char filePath[0x80];
	UInt32 transpose = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &filePath, &transpose))
		return true;
	ReplaceChr(filePath, '/', '\\');
	LineIterator lineIter(filePath, GetStrArgBuffer());
	if (!lineIter) return true;
	TempElements *tempElems = GetTempElements();
	tempElems->Clear();
	char *dataPtr = lineIter.Get(), *pos;
	++lineIter;
	while (true)
	{
		pos = GetNextToken(dataPtr, '\t');
		if (!*dataPtr) break;
		if (*dataPtr == '@')
			tempElems->Append(LookupFormByRefID(StringToRef(dataPtr + 1)));
		else if (*dataPtr == '$')
			tempElems->Append(dataPtr + 1);
		else
			tempElems->Append(StrToDbl(dataPtr));
		dataPtr = pos;
	}
	UInt32 numColumns = tempElems->Size();
	if (!numColumns) return true;
	UInt32 numLines = 1, count;
	while (lineIter)
	{
		dataPtr = lineIter.Get();
		++lineIter;
		numLines++;
		count = numColumns;
		do
		{
			if (*dataPtr)
			{
				pos = GetNextToken(dataPtr, '\t');
				if (*dataPtr == '@')
					tempElems->Append(LookupFormByRefID(StringToRef(dataPtr + 1)));
				else if (*dataPtr == '$')
					tempElems->Append(dataPtr + 1);
				else
					tempElems->Append(StrToDbl(dataPtr));
				dataPtr = pos;
			}
			else tempElems->Append(0.0);
		}
		while (--count);
	}
	ArrayElementL *elemPtr = tempElems->Data();
	if (numLines == 1)
	{
		AssignCommandResult(CreateArray(elemPtr, numColumns, scriptObj), result);
		return true;
	}
	NVSEArrayVar *mainArr = CreateArray(NULL, 0, scriptObj);
	if (transpose)
	{
		count = numLines;
		do
		{
			AppendElement(mainArr, ArrayElementL(CreateArray(elemPtr, numColumns, scriptObj)));
			elemPtr += numColumns;
		}
		while (--count);
	}
	else
	{
		TempElements transElems(numLines);
		count = numColumns;
		do
		{
			for (UInt32 lineIdx = 0; lineIdx < numLines; lineIdx++)
				transElems.Append(elemPtr[lineIdx * numColumns]);
			AppendElement(mainArr, ArrayElementL(CreateArray(transElems.Data(), numLines, scriptObj)));
			transElems.Clear();
			elemPtr++;
		}
		while (--count);
	}
	AssignCommandResult(mainArr, result);
	return true;
}

bool Cmd_WriteArrayToFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char filePath[0x80];
	UInt32 apnd, arrID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &filePath, &apnd, &arrID))
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
	ReplaceChr(filePath, '/', '\\');
	FileStream outputFile;
	if (outputFile.OpenWrite(filePath, apnd != 0))
	{
		char valStr[0x20];
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
							FltToStr(valStr, elem->Number());
							outputFile.WriteStr(valStr);
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
	char filePath[0x80], *buffer = GetStrArgBuffer(), *startPtr = buffer;
	UInt32 startAt = 0, lineCount = 0;
	*startPtr = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &filePath, &startAt, &lineCount))
	{
		ReplaceChr(filePath, '/', '\\');
		if (FileToBuffer(filePath, buffer) && (startAt || lineCount))
		{
			if (startAt) startAt--;
			char data;
			while (data = *buffer)
			{
				if (data == '\n')
				{
					if (startAt)
					{
						startAt--;
						startPtr = buffer + 1;
					}
					else if (!--lineCount)
					{
						if (buffer[-1] == '\r')
							buffer[-1] = 0;
						else *buffer = 0;
						break;
					}
				}
				buffer++;
			}
		}
	}
	AssignString(PASS_COMMAND_ARGS, startPtr);
	return true;
}

bool Cmd_WriteStringToFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char filePath[0x80], *buffer = GetStrArgBuffer();
	UInt32 apnd;
	if (!ExtractFormatStringArgs(2, buffer, EXTRACT_ARGS_EX, kCommandInfo_WriteStringToFile.numParams, &filePath, &apnd))
		return true;
	ReplaceChr(filePath, '/', '\\');
	FileStream outputFile;
	if (outputFile.OpenWrite(filePath, apnd != 0))
	{
		outputFile.WriteStr(buffer);
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modIdx) && (modIdx <= 0xFF) && ResolveRefID(modIdx << 24, &modIdx))
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
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &scrVars, &lnkRefs, &auxVars, &refMaps))
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
	/*UInt32 keepArrs = 0, keepStrs = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &keepArrs, &keepStrs) || s_releaseFast)
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
	}*/
	return true;
}

UnorderedSet<UInt32> s_openLogs;

bool Cmd_ModLogPrint_Execute(COMMAND_ARGS)
{
	UInt32 modIdx = scriptObj ? scriptObj->modIndex : 0xFF;
	if (modIdx == 0xFF) return true;
	char *buffer = GetStrArgBuffer();
	UInt32 indentLevel;
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_ModLogPrint.numParams, &indentLevel))
		return true;
	char modLogPath[0x80];
	*(UInt32*)modLogPath = ' doM';
	*(UInt32*)(modLogPath + 4) = 'sgoL';
	modLogPath[8] = '\\';
	char *endPtr = StrCopy(modLogPath + 9, g_dataHandler->GetNthModName(modIdx));
	*(UInt32*)endPtr = 'gol.';
	endPtr[4] = 0;
	FileStream outputFile;
	if (outputFile.OpenWrite(modLogPath, !s_openLogs.Insert(modIdx)))
	{
		if (indentLevel)
		{
			if (indentLevel >= 40)
				indentLevel = 0;
			else indentLevel = 40 - indentLevel;
			outputFile.WriteStr(kIndentLevelStr + indentLevel);
		}
		outputFile.WriteStr(buffer);
	}
	return true;
}

bool Cmd_GetOptionalPatch_Execute(COMMAND_ARGS)
{
	char patchName[0x40];
	int enabled = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &patchName))
	{
		switch (s_optionalHacks.Get(patchName))
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
			case 18:
				enabled = s_NPCPerks + s_NPCPerksAutoAdd;
				break;
		}
	}
	*result = enabled;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetOptionalPatch_Execute(COMMAND_ARGS)
{
	char patchName[0x40];
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &patchName, &enable))
		SetOptionalPatch(s_optionalHacks.Get(patchName), enable != 0);
	return true;
}

bool Cmd_GetPluginHeaderVersion_Execute(COMMAND_ARGS)
{
	*result = 0;
	char dataPath[0x80];
	*(UInt32*)dataPath = 'atad';
	dataPath[4] = '\\';
	if (ExtractArgsEx(EXTRACT_ARGS_EX, dataPath + 5))
	{
		FileStream sourceFile(dataPath, 0x1E);
		if (sourceFile)
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &milliseconds))
		Sleep(milliseconds);
	return true;
}

bool Cmd_GetArrayValue_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 arrID;
	SInt32 index;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &index) || !arrID)
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

bool Cmd_GetRandomInRange_Execute(COMMAND_ARGS)
{
	int minVal, maxVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &minVal, &maxVal))
		*result = GetRandomIntInRange(minVal, maxVal);
	else *result = 0;
	return true;
}

bool Cmd_GetSessionTime_Execute(COMMAND_ARGS)
{
	*result = int(CdeclCall<UInt32>(0x457FE0) - s_initialTickCount) / 1000.0;
	return true;
}