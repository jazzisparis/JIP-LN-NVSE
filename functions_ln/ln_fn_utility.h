#pragma once

DEFINE_COMMAND_ALT_PLUGIN(GetLNVersion, GetJIPLNVersion, 0, nullptr);
DEFINE_COMMAND_PLUGIN(FileExists, 0, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ListToArray, 0, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetTimeStamp, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetINIFloat, 0, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetINIFloat, 0, kParams_OneString_OneDouble_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINIString, 0, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetINIString, 0, kParams_TwoString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINISection, 0, kParams_OneString_OneOptionalString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetINISection, 0, kParams_OneString_OneInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINISectionNames, 0, kParams_OneOptionalString);
DEFINE_COMMAND_PLUGIN(RemoveINIKey, 0, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(RemoveINISection, 0, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_ALT_PLUGIN(GetFilesInFolder, GetFiles, 0, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_ALT_PLUGIN(GetFoldersInFolder, GetFolders, 0, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SortFormsByType, 0, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetFormCountType, 0, kParams_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(GetDefaultMessageTime, GetDMT, 0, nullptr);
DEFINE_COMMAND_ALT_PLUGIN(SetDefaultMessageTime, SetDMT, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(Console, 0, kParams_FormatString);

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
	DoConsolePrint(result);
	return true;
}

bool Cmd_ListToArray_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm))
	{
		TempElements *tmpElements = GetTempElements();
		auto iter = listForm->list.Head();
		do
		{
			if (iter->data)
				tmpElements->Append(iter->data);
		}
		while (iter = iter->next);
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
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

__declspec(naked) bool __fastcall GetINIPath(char *iniPath, Script *scriptObj)
{
	__asm
	{
		mov		dword ptr [ecx], 'atad'
		mov		dword ptr [ecx+4], 'noc\\'
		mov		dword ptr [ecx+8], '\\gif'
		add		ecx, 0xC
		push	esi
		mov		esi, ecx
		cmp		[ecx], 0
		jnz		hasPath
		mov		al, [edx+0xF]
		cmp		al, 0xFF
		jz		retnFalse
		movzx	edx, al
		mov		ecx, g_dataHandler
		mov		eax, [ecx+edx*4+0x21C]
		test	eax, eax
		jz		retnFalse
		lea		edx, [eax+0x20]
		mov		ecx, esi
		call	StrCopy
		jmp		buildPath
	retnFalse:
		xor		al, al
		pop		esi
		retn
	hasPath:
		push	'\\'
		mov		dl, '/'
		call	ReplaceChr
	buildPath:
		mov		dl, '.'
		mov		ecx, esi
		call	FindChrR
		test	eax, eax
		jz		noDot
		mov		dword ptr [eax+1], 'ini'
		mov		al, 1
		pop		esi
		retn
	noDot:
		mov		ecx, esi
		call	StrLen
		mov		dword ptr [edx], 'ini.'
		mov		[edx+4], 0
		mov		al, 1
		pop		esi
		retn
	}
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
		EMIT_DW(0x3A3A3A3A) EMIT_DW(0x2F2F2F2F) EMIT_DW(0x5C5C5C5C) EMIT_DW_0
	}
}

bool Cmd_GetINIFloat_Execute(COMMAND_ARGS)
{
	char configPath[0x80], valueName[0x80];
	configPath[12] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, configPath + 12) && GetINIPath(configPath, scriptObj))
		if (char *delim = GetValueDelim(valueName); *delim)
		{
			char valStr[0x20];
			valStr[0] = 0;
			if (GetPrivateProfileString(valueName, delim, nullptr, valStr, 0x20, configPath) && valStr[0])
				*result = StrToDbl(valStr);
		}
	return true;
}

bool Cmd_SetINIFloat_Execute(COMMAND_ARGS)
{
	char configPath[0x80], valueName[0x80];
	double value;
	configPath[12] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, &value, configPath + 12) && GetINIPath(configPath, scriptObj))
		if (char *delim = GetValueDelim(valueName); *delim)
		{
			if (!FileExists(configPath))
				FileStream::MakeAllDirs(configPath);
			char valStr[0x20];
			FltToStr(valStr, value);
			if (WritePrivateProfileString(valueName, delim, valStr, configPath))
				*result = 1;
		}
	return true;
}

bool Cmd_GetINIString_Execute(COMMAND_ARGS)
{
	char configPath[0x80], valueName[0x80], *buffer = GetStrArgBuffer();
	buffer[0] = 0;
	configPath[12] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, configPath + 12) && GetINIPath(configPath, scriptObj))
		if (char *delim = GetValueDelim(valueName); *delim)
			GetPrivateProfileString(valueName, delim, nullptr, buffer, kMaxMessageLength, configPath);
	AssignString(PASS_COMMAND_ARGS, buffer);
	return true;
}

bool Cmd_SetINIString_Execute(COMMAND_ARGS)
{
	char configPath[0x80], valueName[0x80], *buffer = GetStrArgBuffer();
	configPath[12] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, buffer, configPath + 12) && GetINIPath(configPath, scriptObj))
		if (char *delim = GetValueDelim(valueName); *delim)
		{
			if (!FileExists(configPath))
				FileStream::MakeAllDirs(configPath);
			if (WritePrivateProfileString(valueName, delim, buffer, configPath))
				*result = 1;
		}
	return true;
}

bool Cmd_GetINISection_Execute(COMMAND_ARGS)
{
	char configPath[0x80], secName[0x40];
	configPath[12] = 0;
	UInt32 getNumeric = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &secName, configPath + 12, &getNumeric) || !GetINIPath(configPath, scriptObj))
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
	*result = (int)outArray;
	return true;
}

bool Cmd_SetINISection_Execute(COMMAND_ARGS)
{
	char configPath[0x80], secName[0x40];
	UInt32 arrID;
	configPath[12] = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &secName, &arrID, configPath + 12) || !GetINIPath(configPath, scriptObj))
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
	char configPath[0x80];
	configPath[12] = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, configPath + 12) || !GetINIPath(configPath, scriptObj))
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
	*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_RemoveINIKey_Execute(COMMAND_ARGS)
{
	char configPath[0x80], valueName[0x80];
	configPath[12] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &valueName, configPath + 12) && GetINIPath(configPath, scriptObj) && FileExists(configPath))
		if (char *key = GetValueDelim(valueName); *key && WritePrivateProfileString(valueName, key, nullptr, configPath))
			*result = 1;
	return true;
}

bool Cmd_RemoveINISection_Execute(COMMAND_ARGS)
{
	char configPath[0x80], secName[0x40];
	configPath[12] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &secName, configPath + 12) && GetINIPath(configPath, scriptObj) && FileExists(configPath) &&
		WritePrivateProfileStruct(secName, nullptr, nullptr, 0, configPath)) *result = 1;
	return true;
}

bool Cmd_GetFilesInFolder_Execute(COMMAND_ARGS)
{
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
	*result = (int)outArray;
	return true;
}

bool Cmd_GetFoldersInFolder_Execute(COMMAND_ARGS)
{
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
	*result = (int)outArray;
	return true;
}

bool Cmd_SortFormsByType_Execute(COMMAND_ARGS)
{
	UInt32 formArrID, typeArrID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &formArrID, &typeArrID)) return true;
	NVSEArrayVar *formArray = LookupArrayByID(formArrID);
	NVSEArrayVar *typeArray = LookupArrayByID(typeArrID);
	if (!formArray || !typeArray) return true;
	UInt32 nForms = GetArraySize(formArray), nTypes = GetArraySize(typeArray);
	if (!nForms || !nTypes) return true;
	TempElements *tmpElements = GetTempElements();
	Vector<TESForm*> tempForms(nForms);
	AuxBuffer<ArrayElementL> elements(GetMax(nForms, nTypes));
	GetElements(formArray, elements, nullptr);
	for (int idx = 0; idx < nForms; idx++)
		if (TESForm *form = elements[idx].Form())
			tempForms.Append(form);
	GetElements(typeArray, elements, nullptr);
	for (UInt32 idx = 0; idx < nTypes; idx++)
	{
		UInt8 typeID = elements[idx].Number();
		for (auto iter = tempForms.BeginRv(); iter; --iter)
			if ((iter->typeID == typeID) || (IS_REFERENCE(*iter) && (((TESObjectREFR*)*iter)->baseForm->typeID == typeID)))
			{
				tmpElements->Append(*iter);
				iter.Remove(tempForms);
			}
		if (tempForms.Empty()) break;
	}
	if (!tempForms.Empty())
		for (auto iter = tempForms.Begin(); iter; ++iter)
			tmpElements->Append(*iter);
	*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetFormCountType_Execute(COMMAND_ARGS)
{
	UInt32 arrID, typeID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &typeID)) return true;
	NVSEArrayVar *formArray = LookupArrayByID(arrID);
	if (!formArray || (GetContainerType(formArray) != NVSEArrayVarInterface::kArrType_Array))
		return true;
	UInt32 size = GetArraySize(formArray);
	if (!size) return true;
	AuxBuffer<ArrayElementR> elements(size);
	if (GetElements(formArray, elements, nullptr))
	{
		int count = 0;
		for (UInt32 idx = 0; idx < size; idx++)
			if (TESForm *form = elements[idx].Form(); form && (form->typeID == typeID))
				count++;
		*result = count;
	}
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
	if (ExtractFormatStringArgs(0, buffer, EXTRACT_ARGS_EX, kCommandInfo_Console.numParams) && JIPScriptRunner::RunScriptSource(buffer, "Console"))
		*result = 1;
	return true;
}