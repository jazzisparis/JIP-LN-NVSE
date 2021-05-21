#pragma once

DEFINE_COMMAND_ALT_PLUGIN(GetLNVersion, GetJIPLNVersion, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(FileExists, , 0, 2, kParams_JIP_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ListToArray, , 0, 1, kParams_FormList);
DEFINE_COMMAND_PLUGIN(GetTimeStamp, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetINIFloat, , 0, 2, kParams_JIP_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetINIFloat, , 0, 3, kParams_JIP_OneString_OneDouble_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINIString, , 0, 2, kParams_JIP_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetINIString, , 0, 3, kParams_JIP_TwoString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINISection, , 0, 3, kParams_JIP_OneString_OneOptionalString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetINISection, , 0, 3, kParams_JIP_OneString_OneInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetINISectionNames, , 0, 1, kParams_JIP_OneOptionalString);
DEFINE_COMMAND_PLUGIN(RemoveINIKey, , 0, 2, kParams_JIP_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(RemoveINISection, , 0, 2, kParams_JIP_OneString_OneOptionalString);
DEFINE_COMMAND_ALT_PLUGIN(GetFilesInFolder, GetFiles, , 0, 2, kParams_JIP_OneString_OneOptionalString);
DEFINE_COMMAND_ALT_PLUGIN(GetFoldersInFolder, GetFolders, , 0, 2, kParams_JIP_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SortFormsByType, , 0, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(GetFormCountType, , 0, 2, kParams_TwoInts);
DEFINE_COMMAND_ALT_PLUGIN(GetDefaultMessageTime, GetDMT, , 0, 0, NULL);
DEFINE_COMMAND_ALT_PLUGIN(SetDefaultMessageTime, SetDMT, , 0, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(Console, , 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(GetENBFloat, , 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(SetENBFloat, , 0, 3, kParams_JIP_TwoStrings_OneFloat);
DEFINE_COMMAND_PLUGIN(ReloadENB, , 0, 0, NULL);

bool Cmd_GetLNVersion_Execute(COMMAND_ARGS)
{
	*result = JIP_LN_VERSION;
	DoConsolePrint(result);
	return true;
}

bool Cmd_FileExists_Execute(COMMAND_ARGS)
{
	UInt32 checkDir = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, s_dataPath, &checkDir))
		*result = FileExistsEx(s_dataPathFull, checkDir != 0);
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_ListToArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSListForm *listForm;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &listForm)) return true;
	s_tempElements.Clear();
	ListNode<TESForm> *iter = listForm->list.Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
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

bool __fastcall GetINIPath(Script *scriptObj)
{
	if (!*s_configPath)
	{
		UInt8 modIdx = scriptObj->GetOverridingModIdx();
		if (modIdx == 0xFF) return false;
		StrCopy(s_configPath, g_dataHandler->GetNthModName(modIdx));
	}
	else ReplaceChr(s_configPath, '/', '\\');
	char *dotPos = FindChrR(s_configPath, StrLen(s_configPath), '.');
	if (dotPos) memcpy(dotPos + 1, "ini", 4);
	else StrCat(s_configPath, ".ini");
	return true;
}

bool Cmd_GetINIFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	*s_configPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, s_configPath) || !GetINIPath(scriptObj)) return true;
	char *delim = GetNextToken(s_strArgBuffer, ":\\/");
	if (!*delim) return true;
	s_strValBuffer[0] = 0;
	if (GetPrivateProfileString(s_strArgBuffer, delim, NULL, s_strValBuffer, 0x100, s_configPathFull) && s_strValBuffer[0])
		*result = StrToDbl(s_strValBuffer);
	return true;
}

bool Cmd_SetINIFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	double value;
	*s_configPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &value, s_configPath) || !GetINIPath(scriptObj)) return true;
	char *delim = GetNextToken(s_strArgBuffer, ":\\/");
	if (!*delim) return true;
	if (!FileExists(s_configPathFull)) FileStream::MakeAllDirs(s_configPathFull);
	FltToStr(s_strValBuffer, value);
	if (WritePrivateProfileString(s_strArgBuffer, delim, s_strValBuffer, s_configPathFull))
		*result = 1;
	return true;
}

bool Cmd_GetINIString_Execute(COMMAND_ARGS)
{
	s_strValBuffer[0] = 0;
	*s_configPath = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, s_configPath) && GetINIPath(scriptObj))
	{
		char *delim = GetNextToken(s_strArgBuffer, ":\\/");
		if (*delim) GetPrivateProfileString(s_strArgBuffer, delim, NULL, s_strValBuffer, kMaxMessageLength, s_configPathFull);
	}
	AssignString(PASS_COMMAND_ARGS, s_strValBuffer);
	return true;
}

bool Cmd_SetINIString_Execute(COMMAND_ARGS)
{
	*result = 0;
	*s_configPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &s_strValBuffer, s_configPath) || !GetINIPath(scriptObj)) return true;
	char *delim = GetNextToken(s_strArgBuffer, ":\\/");
	if (!*delim) return true;
	if (!FileExists(s_configPathFull)) FileStream::MakeAllDirs(s_configPathFull);
	if (WritePrivateProfileString(s_strArgBuffer, delim, s_strValBuffer, s_configPathFull))
		*result = 1;
	return true;
}

bool Cmd_GetINISection_Execute(COMMAND_ARGS)
{
	*result = 0;
	*s_configPath = 0;
	UInt32 getNumeric = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, s_configPath, &getNumeric) || !GetINIPath(scriptObj)) return true;
	NVSEArrayVar *outArray = CreateStringMap(NULL, NULL, 0, scriptObj);
	char *buffer = s_strValBuffer, *delim;
	SInt32 length = GetPrivateProfileSection(s_strArgBuffer, s_strValBuffer, 0x10000, s_configPathFull), size;
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
	UInt32 arrID;
	*s_configPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &arrID, s_configPath) || !GetINIPath(scriptObj)) return true;
	NVSEArrayVar *srcArray = LookupArrayByID(arrID);
	if (!srcArray) return true;
	UInt32 size = GetArraySize(srcArray);
	if (!size) return true;
	if (!FileExists(s_configPathFull)) FileStream::MakeAllDirs(s_configPathFull);
	ArrayElementR *vals = (ArrayElementR*)GetAuxBuffer(s_auxBuffers[2], size * 2 * sizeof(ArrayElementR)), *keys = vals + size;
	MemZero(vals, size * 2 * sizeof(ArrayElementR));
	GetElements(srcArray, vals, keys);
	char *posPtr = s_strValBuffer;
	for (UInt32 idx = 0; idx < size; idx++)
	{
		posPtr = StrCopy(posPtr, keys[idx].String());
		*posPtr++ = '=';
		if (vals[idx].GetType() == 3) posPtr = StrCopy(posPtr, vals[idx].String());
		else posPtr = FltToStr(posPtr, vals[idx].Number());
		posPtr++;
	}
	*posPtr = 0;
	size *= 2;
	do
	{
		vals->~ElementR();
		vals++;
	}
	while (--size);
	if (WritePrivateProfileSection(s_strArgBuffer, s_strValBuffer, s_configPathFull))
		*result = 1;
	return true;
}

bool Cmd_GetINISectionNames_Execute(COMMAND_ARGS)
{
	*result = 0;
	*s_configPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, s_configPath) || !GetINIPath(scriptObj)) return true;
	s_tempElements.Clear();
	SInt32 length = GetPrivateProfileSectionNames(s_strValBuffer, kMaxMessageLength, s_configPathFull), size;
	char *name = s_strValBuffer;
	while (length > 0)
	{
		s_tempElements.Append(name);
		length -= size = StrLen(name) + 1;
		name += size;
	}
	AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_RemoveINIKey_Execute(COMMAND_ARGS)
{
	*result = 0;
	*s_configPath = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, s_configPath) || !GetINIPath(scriptObj) || !FileExists(s_configPathFull)) return true;
	char *key = GetNextToken(s_strArgBuffer, ":\\/");
	if (*key && WritePrivateProfileString(s_strArgBuffer, key, NULL, s_configPathFull))
		*result = 1;
	return true;
}

bool Cmd_RemoveINISection_Execute(COMMAND_ARGS)
{
	*s_configPath = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, s_configPath) && GetINIPath(scriptObj) && FileExists(s_configPathFull) && 
		WritePrivateProfileStruct(s_strArgBuffer, NULL, NULL, 0, s_configPathFull)) *result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetFilesInFolder_Execute(COMMAND_ARGS)
{
	*result = 0;
	s_strArgBuffer[0] = '*';
	s_strArgBuffer[1] = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, s_dataPath, &s_strArgBuffer)) return true;
	ReplaceChr(s_dataPath, '/', '\\');
	char *pathEnd = s_dataPath + StrLen(s_dataPath);
	if (pathEnd[-1] != '\\') *pathEnd++ = '\\';
	StrCopy(pathEnd, s_strArgBuffer);
	NVSEArrayVar *outArray = CreateArray(NULL, 0, scriptObj);
	for (DirectoryIterator iter(s_dataPathFull); iter; ++iter)
		if (iter.IsFile()) AppendElement(outArray, ArrayElementL(iter.Get()));
	AssignCommandResult(outArray, result);
	return true;
}

bool Cmd_GetFoldersInFolder_Execute(COMMAND_ARGS)
{
	*result = 0;
	s_strArgBuffer[0] = '*';
	s_strArgBuffer[1] = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, s_dataPath, &s_strArgBuffer)) return true;
	ReplaceChr(s_dataPath, '/', '\\');
	char *pathEnd = s_dataPath + StrLen(s_dataPath);
	if (pathEnd[-1] != '\\') *pathEnd++ = '\\';
	StrCopy(pathEnd, s_strArgBuffer);
	NVSEArrayVar *outArray = CreateArray(NULL, 0, scriptObj);
	for (DirectoryIterator iter(s_dataPathFull); iter; ++iter)
		if (iter.IsFolder()) AppendElement(outArray, ArrayElementL(iter.Get()));
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
	s_tempElements.Clear();
	Vector<TESForm*> tempForms(nForms);
	UInt32 size = GetMax(nForms, nTypes) * sizeof(ArrayElementL);
	ArrayElementL *elements = (ArrayElementL*)GetAuxBuffer(s_auxBuffers[2], size);
	MemZero(elements, size);
	GetElements(formArray, elements, NULL);
	TESForm *form;
	for (int idx = 0; idx < nForms; idx++)
	{
		form = elements[idx].Form();
		if (form) tempForms.Append(form);
	}
	GetElements(typeArray, elements, NULL);
	UInt8 typeID;
	for (UInt32 idx = 0; idx < nTypes; idx++)
	{
		typeID = elements[idx].Number();
		for (auto iter = tempForms.BeginRv(); iter; --iter)
		{
			form = *iter;
			if ((form->typeID != typeID) && (NOT_REFERENCE(form) || (((TESObjectREFR*)form)->baseForm->typeID != typeID)))
				continue;
			s_tempElements.Append(form);
			iter.Remove(tempForms);
		}
		if (tempForms.Empty()) break;
	}
	if (!tempForms.Empty())
		for (auto iter = tempForms.Begin(); iter; ++iter)
			s_tempElements.Append(*iter);
	AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetFormCountType_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 arrID, typeID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &arrID, &typeID)) return true;
	NVSEArrayVar *formArray = LookupArrayByID(arrID);
	if (!formArray) return true;
	UInt32 size;
	ArrayElementR *elements = GetArrayData(formArray, &size);
	if (!elements) return true;
	int count = 0;
	TESForm *form;
	for (UInt32 idx = 0; idx < size; idx++)
	{
		form = elements[idx].Form();
		if (form && (form->typeID == typeID)) count++;
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
	if (ExtractFormatStringArgs(0, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_Console.numParams))
		*result = JIPScriptRunner::RunScript(s_strArgBuffer);
	else *result = 0;
	return true;
}

bool Cmd_GetENBFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &s_strValBuffer) || !FileExists(s_strArgBuffer)) return true;
	char *delim = GetNextToken(s_strValBuffer, ":\\/");
	if (!*delim) return true;
	char strVal[0x20];
	if (GetPrivateProfileString(s_strValBuffer, delim, NULL, strVal, 0x20, s_strArgBuffer) && *strVal)
		*result = StrToDbl(strVal);
	return true;
}

bool Cmd_SetENBFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	float value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &s_strValBuffer, &value) || !FileExists(s_strArgBuffer)) return true;
	char *delim = GetNextToken(s_strValBuffer, ":\\/");
	if (!*delim) return true;
	char strVal[0x20];
	FltToStr(strVal, value);
	if (WritePrivateProfileString(s_strValBuffer, delim, strVal, s_strArgBuffer))
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