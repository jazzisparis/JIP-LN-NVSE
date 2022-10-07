#pragma once

DEFINE_COMMAND_ALT_PLUGIN(GetLNVersion, GetJIPLNVersion, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(FileExists, 0, 2, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ListToArray, 0, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetTimeStamp, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetINIFloat, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetINIFloat, 0, 3, kParams_OneString_OneDouble_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINIString, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetINIString, 0, 3, kParams_TwoString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINISection, 0, 3, kParams_OneString_OneOptionalString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetINISection, 0, 3, kParams_OneString_OneInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINISectionNames, 0, 1, kParams_OneOptionalString);
DEFINE_COMMAND_PLUGIN(RemoveINIKey, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(RemoveINISection, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_ALT_PLUGIN(GetFilesInFolder, GetFiles, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_ALT_PLUGIN(GetFoldersInFolder, GetFolders, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SortFormsByType, 0, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetFormCountType, 0, 2, kParams_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(GetDefaultMessageTime, GetDMT, 0, 0, nullptr);
DEFINE_COMMAND_ALT_PLUGIN(SetDefaultMessageTime, SetDMT, 0, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(Console, 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(GetENBFloat, 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(SetENBFloat, 0, 3, kParams_TwoStrings_OneDouble);
DEFINE_COMMAND_PLUGIN(ReloadENB, 0, 0, nullptr);

bool Cmd_GetLNVersion_Execute(COMMAND_ARGS)
{
	*result = JIP_LN_VERSION;
	DoConsolePrint(result);
	return true;
}

bool Cmd_FileExists_Execute(COMMAND_ARGS)
{
	char dataPath[0x80];
	*(UInt32*)dataPath = 'atad';
	dataPath[4] = '\\';
	UInt32 checkDir = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, dataPath + 5, &checkDir))
		*result = FileExistsEx(dataPath, checkDir != 0);
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_ListToArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSListForm *listForm;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &listForm)) return true;
	TempElements *tmpElements = GetTempElements();
	ListNode<TESForm> *iter = listForm->list.Head();
	do
	{
		if (iter->data) tmpElements->Append(iter->data);
	}
	while (iter = iter->next);
	AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_GetTimeStamp_Execute(COMMAND_ARGS)
{
	char buffer[0x20];
	GetTimeStamp(buffer);
	if (IsConsoleOpen()) Console_Print(buffer);
	AssignString(PASS_COMMAND_ARGS, buffer);
	return true;
}

bool __fastcall GetINIPath(char *iniPath, Script *scriptObj)
{
	if (!*iniPath)
	{
		UInt8 modIdx = scriptObj->GetOverridingModIdx();
		if (modIdx == 0xFF) return false;
		StrCopy(iniPath, g_dataHandler->GetNthModName(modIdx));
	}
	else ReplaceChr(iniPath, '/', '\\');
	char *dotPos = FindChrR(iniPath, '.');
	if (dotPos) *(UInt32*)(dotPos + 1) = 'ini';
	else
	{
		UInt32 length = StrLen(iniPath);
		*(UInt32*)(iniPath + length) = 'ini.';
		iniPath[length + 4] = 0;
	}
	*(UInt32*)(iniPath - 12) = 'atad';
	*(UInt32*)(iniPath - 8) = 'noc\\';
	*(UInt32*)(iniPath - 4) = '\\gif';
	return true;
}

__declspec(naked) char* __fastcall GetValueDelim(char *valueName)
{
	__asm
	{
		movaps	xmm7, kINIDelims
		pshufd	xmm6, xmm7, 0x55
		pshufd	xmm5, xmm7, 0xAA
		shufps	xmm7, xmm7, 0
		ALIGN 16
	iterHead:
		movups	xmm4, [ecx]
		add		ecx, 0x10
		xorps	xmm3, xmm3
		movaps	xmm2, xmm4
		pcmpeqb	xmm3, xmm4
		pcmpeqb	xmm2, xmm7
		por		xmm3, xmm2
		movaps	xmm2, xmm4
		pcmpeqb	xmm2, xmm6
		por		xmm3, xmm2
		pcmpeqb	xmm4, xmm5
		por		xmm3, xmm4
		pmovmskb	edx, xmm3
		bsf		edx, edx
		jz		iterHead
		lea		eax, [ecx+edx-0x10]
		cmp		[eax], 0
		jz		done
		mov		[eax], 0
		inc		eax
	done:
		retn
		ALIGN 16
	kINIDelims:
		EMIT_DW(3A, 3A, 3A, 3A) EMIT_DW(2F, 2F, 2F, 2F)
		EMIT_DW(5C, 5C, 5C, 5C) EMIT_DW(00, 00, 00, 00)
	}
}

bool Cmd_GetINIFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], valueName[0x80], *iniPath = configPath + 12;
	*iniPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, iniPath) || !GetINIPath(iniPath, scriptObj))
		return true;
	char *delim = GetValueDelim(valueName);
	if (!*delim) return true;
	char valStr[0x20];
	valStr[0] = 0;
	if (GetPrivateProfileString(valueName, delim, nullptr, valStr, 0x20, configPath) && valStr[0])
		*result = StrToDbl(valStr);
	return true;
}

bool Cmd_SetINIFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], valueName[0x80], *iniPath = configPath + 12;
	double value;
	*iniPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, &value, iniPath) || !GetINIPath(iniPath, scriptObj))
		return true;
	char *delim = GetValueDelim(valueName);
	if (!*delim) return true;
	if (!FileExists(configPath)) FileStream::MakeAllDirs(configPath);
	char valStr[0x20];
	FltToStr(valStr, value);
	if (WritePrivateProfileString(valueName, delim, valStr, configPath))
		*result = 1;
	return true;
}

bool Cmd_GetINIString_Execute(COMMAND_ARGS)
{
	char configPath[0x80], valueName[0x80], *iniPath = configPath + 12, *buffer = GetStrArgBuffer();
	buffer[0] = 0;
	*iniPath = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, iniPath) && GetINIPath(iniPath, scriptObj))
	{
		char *delim = GetValueDelim(valueName);
		if (*delim) GetPrivateProfileString(valueName, delim, nullptr, buffer, kMaxMessageLength, configPath);
	}
	AssignString(PASS_COMMAND_ARGS, buffer);
	return true;
}

bool Cmd_SetINIString_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], valueName[0x80], *iniPath = configPath + 12, *buffer = GetStrArgBuffer();
	*iniPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, buffer, iniPath) || !GetINIPath(iniPath, scriptObj))
		return true;
	char *delim = GetValueDelim(valueName);
	if (!*delim) return true;
	if (!FileExists(configPath)) FileStream::MakeAllDirs(configPath);
	if (WritePrivateProfileString(valueName, delim, buffer, configPath))
		*result = 1;
	return true;
}

bool Cmd_GetINISection_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], secName[0x40], *iniPath = configPath + 12;
	*iniPath = 0;
	UInt32 getNumeric = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &secName, iniPath, &getNumeric) || !GetINIPath(iniPath, scriptObj))
		return true;
	NVSEArrayVar *outArray = CreateStringMap(nullptr, nullptr, 0, scriptObj);
	char *buffer = GetStrArgBuffer(), *delim;
	SInt32 length = GetPrivateProfileSection(secName, buffer, 0x10000, configPath), size;
	while (length > 0)
	{
		length -= size = StrLen(buffer) + 1;
		delim = GetNextToken(buffer, '=');
		if (*delim)
			SetElement(outArray, ArrayElementL(buffer), getNumeric ? ArrayElementL(StrToDbl(delim)) : ArrayElementL(delim));
		buffer += size;
	}
	AssignCommandResult(outArray, result);
	return true;
}

bool Cmd_SetINISection_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], secName[0x40], *iniPath = configPath + 12;
	UInt32 arrID;
	*iniPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &secName, &arrID, iniPath) || !GetINIPath(iniPath, scriptObj))
		return true;
	NVSEArrayVar *srcArray = LookupArrayByID(arrID);
	if (!srcArray || (GetContainerType(srcArray) != NVSEArrayVarInterface::kArrType_StringMap))
		return true;
	ArrayData arrData(srcArray, false);
	if (!arrData.size) return true;
	char *buffer = GetStrArgBuffer(), *posPtr = buffer;
	for (UInt32 idx = 0; idx < arrData.size; idx++)
	{
		posPtr = StrCopy(posPtr, arrData.keys[idx].String());
		*posPtr++ = '=';
		if (arrData.vals[idx].GetType() == 3)
			posPtr = StrCopy(posPtr, arrData.vals[idx].String());
		else posPtr = FltToStr(posPtr, arrData.vals[idx].Number());
		posPtr++;
	}
	*posPtr = 0;
	if (!FileExists(configPath))
		FileStream::MakeAllDirs(configPath);
	if (WritePrivateProfileSection(secName, buffer, configPath))
		*result = 1;
	return true;
}

bool Cmd_GetINISectionNames_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], *iniPath = configPath + 12;
	*iniPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, iniPath) || !GetINIPath(iniPath, scriptObj))
		return true;
	TempElements *tmpElements = GetTempElements();
	char *buffer = GetStrArgBuffer();
	SInt32 length = GetPrivateProfileSectionNames(buffer, kMaxMessageLength, configPath), size;
	while (length > 0)
	{
		tmpElements->Append(buffer);
		length -= size = StrLen(buffer) + 1;
		buffer += size;
	}
	AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_RemoveINIKey_Execute(COMMAND_ARGS)
{
	*result = 0;
	char configPath[0x80], valueName[0x80], *iniPath = configPath + 12;
	*iniPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, iniPath) || !GetINIPath(iniPath, scriptObj) || !FileExists(configPath))
		return true;
	char *key = GetValueDelim(valueName);
	if (*key && WritePrivateProfileString(valueName, key, nullptr, configPath))
		*result = 1;
	return true;
}

bool Cmd_RemoveINISection_Execute(COMMAND_ARGS)
{
	char configPath[0x80], secName[0x40], *iniPath = configPath + 12;
	*iniPath = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &secName, iniPath) && GetINIPath(iniPath, scriptObj) && FileExists(configPath) && 
		WritePrivateProfileStruct(secName, nullptr, nullptr, 0, configPath)) *result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetFilesInFolder_Execute(COMMAND_ARGS)
{
	*result = 0;
	char dataPathFull[0x80], filter[0x40], *dataPath = dataPathFull + 5;
	*(UInt32*)dataPathFull = 'atad';
	dataPathFull[4] = '\\';
	*(UInt16*)filter = '*';
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, dataPath, &filter)) return true;
	ReplaceChr(dataPath, '/', '\\');
	char *pathEnd = dataPath + StrLen(dataPath);
	if (pathEnd[-1] != '\\') *pathEnd++ = '\\';
	StrCopy(pathEnd, filter);
	NVSEArrayVar *outArray = CreateArray(nullptr, 0, scriptObj);
	for (DirectoryIterator iter(dataPathFull); iter; ++iter)
		if (iter.IsFile()) AppendElement(outArray, ArrayElementL(*iter));
	AssignCommandResult(outArray, result);
	return true;
}

bool Cmd_GetFoldersInFolder_Execute(COMMAND_ARGS)
{
	*result = 0;
	char dataPathFull[0x80], filter[0x40], *dataPath = dataPathFull + 5;
	*(UInt32*)dataPathFull = 'atad';
	dataPathFull[4] = '\\';
	*(UInt16*)filter = '*';
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, dataPath, &filter)) return true;
	ReplaceChr(dataPath, '/', '\\');
	char *pathEnd = dataPath + StrLen(dataPath);
	if (pathEnd[-1] != '\\') *pathEnd++ = '\\';
	StrCopy(pathEnd, filter);
	NVSEArrayVar *outArray = CreateArray(nullptr, 0, scriptObj);
	for (DirectoryIterator iter(dataPathFull); iter; ++iter)
		if (iter.IsFolder()) AppendElement(outArray, ArrayElementL(*iter));
	AssignCommandResult(outArray, result);
	return true;
}

bool Cmd_SortFormsByType_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 formArrID, typeArrID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &formArrID, &typeArrID)) return true;
	NVSEArrayVar *formArray = LookupArrayByID(formArrID);
	NVSEArrayVar *typeArray = LookupArrayByID(typeArrID);
	if (!formArray || !typeArray) return true;
	UInt32 nForms = GetArraySize(formArray), nTypes = GetArraySize(typeArray);
	if (!nForms || !nTypes) return true;
	TempElements *tmpElements = GetTempElements();
	Vector<TESForm*> tempForms(nForms);
	UInt32 size = GetMax(nForms, nTypes) * sizeof(ArrayElementL);
	ArrayElementL *elements = (ArrayElementL*)AuxBuffer::Get(2, size);
	MEM_ZERO(elements, size);
	GetElements(formArray, elements, nullptr);
	TESForm *form;
	for (int idx = 0; idx < nForms; idx++)
	{
		form = elements[idx].Form();
		if (form) tempForms.Append(form);
	}
	GetElements(typeArray, elements, nullptr);
	UInt8 typeID;
	for (UInt32 idx = 0; idx < nTypes; idx++)
	{
		typeID = elements[idx].Number();
		for (auto iter = tempForms.BeginRv(); iter; --iter)
		{
			form = *iter;
			if ((form->typeID != typeID) && (NOT_REFERENCE(form) || (((TESObjectREFR*)form)->baseForm->typeID != typeID)))
				continue;
			tmpElements->Append(form);
			iter.Remove(tempForms);
		}
		if (tempForms.Empty()) break;
	}
	if (!tempForms.Empty())
		for (auto iter = tempForms.Begin(); iter; ++iter)
			tmpElements->Append(*iter);
	AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_GetFormCountType_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 arrID, typeID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &typeID)) return true;
	NVSEArrayVar *formArray = LookupArrayByID(arrID);
	if (!formArray || (GetContainerType(formArray) != NVSEArrayVarInterface::kArrType_Array))
		return true;
	UInt32 size = GetArraySize(formArray);
	if (!size) return true;
	ArrayElementR *elements = (ArrayElementR*)AuxBuffer::Get(2, size * sizeof(ArrayElementR));
	MEM_ZERO(elements, size * sizeof(ArrayElementR));
	if (!GetElements(formArray, elements, nullptr))
		return true;
	int count = 0;
	TESForm *form;
	for (UInt32 idx = 0; idx < size; idx++)
	{
		form = elements[idx].Form();
		if (form && (form->typeID == typeID))
			count++;
	}
	*result = count;
	return true;
}

bool Cmd_GetDefaultMessageTime_Execute(COMMAND_ARGS)
{
	*result = s_msgDisplayTime;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetDefaultMessageTime_Execute(COMMAND_ARGS)
{
	float inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &inval))
		s_msgDisplayTime = inval;
	return true;
}

bool Cmd_Console_Execute(COMMAND_ARGS)
{
	char *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(0, buffer, EXTRACT_ARGS_EX, kCommandInfo_Console.numParams) && JIPScriptRunner::RunScriptSource(buffer))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetENBFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char fileName[0x40], valueName[0x80];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &fileName, &valueName) || !FileExists(fileName))
		return true;
	char *delim = GetNextToken(valueName, ':');
	if (!*delim) return true;
	char strVal[0x20];
	if (GetPrivateProfileString(valueName, delim, nullptr, strVal, 0x20, fileName) && *strVal)
		*result = StrToDbl(strVal);
	return true;
}

bool Cmd_SetENBFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char fileName[0x40], valueName[0x80];
	double value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &fileName, &valueName, &value) || !FileExists(fileName))
		return true;
	char *delim = GetNextToken(valueName, ':');
	if (!*delim) return true;
	char strVal[0x20];
	FltToStr(strVal, value);
	if (WritePrivateProfileString(valueName, delim, strVal, fileName))
		*result = 1;
	return true;
}

bool Cmd_ReloadENB_Execute(COMMAND_ARGS)
{
	if (ReloadENB)
	{
		ReloadENB(3, result);
		*result = 1;
	}
	else *result = 0;
	return true;
}