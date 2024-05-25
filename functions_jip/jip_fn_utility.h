#pragma once

DEFINE_COMMAND_PLUGIN(RefToString, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(StringToRef, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetMinOf, 0, kParams_TwoDoubles_ThreeOptionalDoubles);
DEFINE_COMMAND_PLUGIN(GetMaxOf, 0, kParams_TwoDoubles_ThreeOptionalDoubles);
DEFINE_COMMAND_PLUGIN(ReadArrayFromFile, 0, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(WriteArrayToFile, 0, kParams_OneString_TwoInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ReadStringFromFile, 0, kParams_OneString_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(WriteStringToFile, 0, kParams_OneString_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetLoadOrderChanged, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ValidateModIndex, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ClearJIPSavedData, 0, kParams_FourInts);
DEFINE_COMMAND_PLUGIN(ModLogPrint, 0, kParams_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetOptionalPatch, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetOptionalPatch, 0, kParams_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(GetPluginHeaderVersion, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetIsLAA, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetArrayValue, 0, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetRandomInRange, 0, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetSessionTime, 0, nullptr);

bool Cmd_RefToString_Execute(COMMAND_ARGS)
{
	TESForm *form = nullptr;
	if (!NUM_ARGS)
		form = thisObj;
	else if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IS_REFERENCE(form))
		form = ((TESObjectREFR*)form)->baseForm;
	AssignString(PASS_COMMAND_ARGS, form ? form->RefToString() : nullptr);
	return true;
}

bool Cmd_StringToRef_Execute(COMMAND_ARGS)
{
	char refStr[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refStr))
		REFR_RES = StringToRef(refStr);
	return true;
}

bool Cmd_GetMinOf_Execute(COMMAND_ARGS)
{
	UInt8 numArgs = NUM_ARGS;
	double values[5];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &values, &values[1], &values[2], &values[3], &values[4]))
	{
		__asm
		{
			mov		eax, 1
			movq	xmm0, values
			ALIGN 16
		iterHead:
			minsd	xmm0, values[eax*8]
			inc		eax
			cmp		al, numArgs
			jb		iterHead
			mov		eax, result
			movlpd	[eax], xmm0
		}
	}
	return true;
}

bool Cmd_GetMaxOf_Execute(COMMAND_ARGS)
{
	UInt8 numArgs = NUM_ARGS;
	double values[5];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &values, &values[1], &values[2], &values[3], &values[4]))
	{
		__asm
		{
			mov		eax, 1
			movq	xmm0, values
			ALIGN 16
		iterHead:
			maxsd	xmm0, values[eax*8]
			inc		eax
			cmp		al, numArgs
			jb		iterHead
			mov		eax, result
			movlpd	[eax], xmm0
		}
	}
	return true;
}

bool Cmd_ReadArrayFromFile_Execute(COMMAND_ARGS)
{
	char filePath[0x100];
	UInt32 transpose = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &filePath, &transpose))
		return true;
	ReplaceChr(filePath, '/', '\\');
	LineIterator lineIter(filePath, GetStrArgBuffer());
	if (!lineIter) return true;
	TempElements *tempElems = GetTempElements();
	char *dataPtr = *lineIter, *pos;
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
		dataPtr = *lineIter;
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
	if ((numLines == 1) && !transpose)
	{
		*result = (int)CreateArray(elemPtr, numColumns, scriptObj);
		return true;
	}
	NVSEArrayVar *mainArr = CreateArray(NULL, 0, scriptObj);
	if (!transpose)
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
	else
	{
		count = numLines;
		do
		{
			AppendElement(mainArr, ArrayElementL(CreateArray(elemPtr, numColumns, scriptObj)));
			elemPtr += numColumns;
		}
		while (--count);
	}
	*result = (int)mainArr;
	return true;
}

void __fastcall WriteElemToFile(TempArrayElements *colElements, UInt32 idx, FILE *theFile, bool edid)
{
	if (colElements->size > idx)
	{
		ArrayElementR *elem = &(*colElements)[idx];
		switch (elem->GetType())
		{
			case 1:
			{
				char valStr[0x20];
				FltToStr(valStr, elem->Number());
				fputs(valStr, theFile);
				return;
			}
			case 2:
				if (elem->Form())
				{
					fputc('@', theFile);
					if (edid)
						if (const char *edidStr = elem->Form()->GetEditorID(); edidStr && *edidStr)
						{
							fputs(edidStr, theFile);
							return;
						}
					fputs(elem->Form()->RefToString(), theFile);
					return;
				}
				break;
			case 3:
				fputc('$', theFile);
				fputs(elem->String(), theFile);
				return;
		}
	}
	fputc('0', theFile);
}

bool Cmd_WriteArrayToFile_Execute(COMMAND_ARGS)
{
	char filePath[0x100];
	UInt32 apnd, arrID, flags = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &filePath, &apnd, &arrID, &flags))
		return true;
	NVSEArrayVar *mainArray = LookupArrayByID(arrID);
	if (!mainArray) return true;
	UInt32 numLines = 1;
	TempArrayElements topLine(mainArray);
	if (!topLine.size) return true;
	Vector<TempArrayElements> columnBuffer(topLine.size);
	for (UInt32 idx = 0; idx < topLine.size; idx++)
	{
		if (NVSEArrayVar *column = topLine[idx].GetArray())
		{
			TempArrayElements *colElements = columnBuffer.Append(column);
			if (numLines < colElements->size)
				numLines = colElements->size;
		}
		else columnBuffer.Append(&topLine[idx]);
	}
	ReplaceChr(filePath, '/', '\\');
	if (FileStream outputFile; outputFile.OpenWrite(filePath, apnd != 0))
	{
		bool edid = (flags & 2) != 0;
		if (!(flags & 1))
		{
			for (UInt32 idx = 0; idx < numLines; idx++)
			{
				for (UInt32 cnt = 0; cnt < topLine.size; cnt++)
				{
					WriteElemToFile(&columnBuffer[cnt], idx, outputFile, edid);
					if ((topLine.size - cnt) > 1)
						fputc('\t', outputFile);
				}
				if ((numLines - idx) > 1)
					fputc('\n', outputFile);
			}
		}
		else
		{
			for (UInt32 cnt = 0; cnt < topLine.size; cnt++)
			{
				TempArrayElements *colElements = &columnBuffer[cnt];
				for (UInt32 idx = 0; idx < numLines; idx++)
				{
					WriteElemToFile(colElements, idx, outputFile, edid);
					if ((numLines - idx) > 1)
						fputc('\t', outputFile);
				}
				if ((topLine.size - cnt) > 1)
					fputc('\n', outputFile);
			}
		}
		*result = 1;
	}
	return true;
}

bool Cmd_ReadStringFromFile_Execute(COMMAND_ARGS)
{
	char *buffer = GetStrArgBuffer(), *startPtr = buffer;
	SInt32 startAt = 0, lineCount = -1, lenRead = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, buffer, &startAt, &lineCount) && lineCount)
	{
		ReplaceChr(buffer, '/', '\\');
		if ((lenRead = FileToBuffer(buffer, buffer, kMaxMessageLength - 1)) && ((--startAt > 0) || (lineCount > 0)))
			while (buffer = FindChr(buffer, '\n'))
			{
				if (startAt > 0)
				{
					startAt--;
					startPtr = buffer + 1;
				}
				else if (lineCount <= 0)
					break;
				else if (!--lineCount)
				{
					if (buffer[-1] == '\r')
						buffer[-1] = 0;
					else *buffer = 0;
					break;
				}
				buffer++;
			}
	}
	if (!lenRead)
		*startPtr = 0;
	AssignString(PASS_COMMAND_ARGS, startPtr);
	return true;
}

bool Cmd_WriteStringToFile_Execute(COMMAND_ARGS)
{
	char *buffer = GetStrArgBuffer();
	UInt32 apnd;
	if (ExtractFormatStringArgs(2, buffer + 0x100, EXTRACT_ARGS_EX, kCommandInfo_WriteStringToFile.numParams, buffer, &apnd))
	{
		ReplaceChr(buffer, '/', '\\');
		if (FileStream outputFile; outputFile.OpenWrite(buffer, apnd != 0))
		{
			outputFile.WriteStr(buffer + 0x100);
			*result = 1;
		}
	}
	return true;
}

bool Cmd_GetLoadOrderChanged_Execute(COMMAND_ARGS)
{
	UInt8 *idxArray = g_BGSSaveLoadGame->saveMods;
	for (UInt32 idx = 254; idx > 0; idx--)
	{
		UInt8 modIdx = idxArray[idx];
		if ((modIdx == 0xFF) || (modIdx == idxArray[idx + 0xFF]))
			continue;
		*result = 1;
		break;
	}
	return true;
}

bool Cmd_ValidateModIndex_Execute(COMMAND_ARGS)
{
	UInt32 modIdx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modIdx) && (modIdx <= 0xFF) && GetResolvedModIndex((UInt8*)&modIdx))
		*result = (int)modIdx;
	else *result = -1;
	return true;
}

bool Cmd_ClearJIPSavedData_Execute(COMMAND_ARGS)
{
	UInt32 scrVars, lnkRefs, auxVars, refMaps/*, xData = 0*/;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &scrVars, &lnkRefs, &auxVars, &refMaps/*, &xData*/))
		return true;
	UInt32 modIdx = scriptObj->GetOverridingModIdx();
	if (scrVars)
	{
		for (auto ownerIter = s_scriptVariablesBuffer->Begin(); ownerIter; ++ownerIter)
		{
			for (auto varIter = ownerIter().Begin(); varIter; ++varIter)
				if (varIter().modIdx == modIdx) varIter.Remove();
			if (ownerIter().Empty()) ownerIter.Remove();
		}
	}
	if (lnkRefs && !s_linkedRefModified->Empty())
	{
		for (auto refIter = s_linkedRefModified->Begin(); refIter; ++refIter)
			if (refIter().modIdx == modIdx) refIter.Remove();
		s_dataChangedFlags |= kChangedFlag_LinkedRefs;
	}
	if (auxVars && s_auxVariables[0]->Erase((auxVars == 2) ? 0xFF : modIdx))
		s_dataChangedFlags |= kChangedFlag_AuxVars;
	if (refMaps && s_refMapArrays[0]->Erase((refMaps == 2) ? 0xFF : modIdx))
		s_dataChangedFlags |= kChangedFlag_RefMaps;
	/*if (xData && !s_extraDataKeysMap->Empty() && (modIdx < 0xFF))
	{
		for (auto jedIter = s_extraDataKeysMap->Begin(); jedIter; ++jedIter)
			if (jedIter().dataMap.Erase(modIdx) && jedIter().dataMap.Empty())
				jedIter.Remove();
		s_dataChangedFlags |= kChangedFlag_ExtraData;
	}*/
	return true;
}

bool Cmd_ModLogPrint_Execute(COMMAND_ARGS)
{
	UInt32 modIdx = scriptObj->GetOverridingModIdx();
	if (modIdx == 0xFF) return true;
	char *buffer = GetStrArgBuffer();
	UInt32 indentLevel;
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_ModLogPrint.numParams, &indentLevel))
		return true;
	ModInfo *modInfo = g_dataHandler->GetNthModInfo(modIdx);
	char modLogPath[0x80];
	*(UInt32*)modLogPath = ' doM';
	*(UInt32*)(modLogPath + 4) = 'sgoL';
	modLogPath[8] = '\\';
	char *endPtr = StrCopy(modLogPath + 9, modInfo->name);
	*(UInt32*)endPtr = 'gol.';
	endPtr[4] = 0;
	if (FileStream outputFile; outputFile.OpenWrite(modLogPath, modInfo->hasModLog))
	{
		modInfo->hasModLog = true;
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
	bool enabled = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &patchName))
		switch (s_optionalHacks->Get(patchName))
		{
			case 1:
				enabled = HOOK_INSTALLED(CalculateHitDamage);
				break;
			case 2:
				enabled = HOOK_INSTALLED(FO3Repair);
				break;
			case 3:
				enabled = s_patchInstallState.bigGunsSkill;
				break;
			case 4:
				enabled = s_patchInstallState.impactDmgFix;
				break;
			case 5:
				enabled = HOOK_INSTALLED(UpdateTimeGlobals);
				break;
			case 6:
				enabled = s_patchInstallState.hardcoreNeedsFix;
				break;
			case 7:
				enabled = s_patchInstallState.failedScriptLocks;
				break;
			case 8:
				enabled = s_patchInstallState.dblPrecision;
				break;
			case 9:
				enabled = HOOK_INSTALLED(QuantitySelect);
				break;
			case 11:
				enabled = s_patchInstallState.FO3WpnDegrade;
				break;
			case 12:
				enabled = s_patchInstallState.localizedDTDR;
				break;
			case 13:
				enabled = HOOK_INSTALLED(VoiceModulationFix);
				break;
			case 14:
				enabled = s_patchInstallState.sneakBoundsFix;
				break;
			case 15:
				enabled = s_patchInstallState.NVACAlerts;
				break;
			case 16:
				enabled = HOOK_INSTALLED(LoadScreenFix);
				break;
			case 17:
				enabled = s_patchInstallState.NPCWeaponMods;
				break;
			case 18:
				enabled = s_patchInstallState.NPCPerks + s_NPCPerksAutoAdd;
				break;
			case 19:
				enabled = s_patchInstallState.creSpreadFix;
				break;
		}
	if (enabled)
		*result = 1;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetOptionalPatch_Execute(COMMAND_ARGS)
{
	char patchName[0x40];
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &patchName, &enable))
		SetOptionalPatch(s_optionalHacks->Get(patchName), enable != 0);
	return true;
}

bool Cmd_GetPluginHeaderVersion_Execute(COMMAND_ARGS)
{
	char modName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modName))
		if (auto modInfo = g_dataHandler->LookupModByName(modName))
			*result = modInfo->header.version;
	return true;
}

bool Cmd_GetIsLAA_Execute(COMMAND_ARGS)
{
	*result = GetIsLAA();
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetArrayValue_Execute(COMMAND_ARGS)
{
	UInt32 arrID;
	SInt32 index;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &index) && arrID)
		if (ArrayElementR resElement; GetElement((NVSEArrayVar*)arrID, ArrayElementL(index), resElement))
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
	SInt32 minVal, maxVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &minVal, &maxVal))
		*result = GetRandomIntInRange(minVal, maxVal);
	return true;
}

bool Cmd_GetSessionTime_Execute(COMMAND_ARGS)
{
	*result = int(CdeclCall<UInt32>(0x457FE0) - s_initialTickCount) * 0.001;
	return true;
}