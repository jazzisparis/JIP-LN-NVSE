#pragma once

DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetSize, RefMapSize, , 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetType, RefMapGetType, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetFloat, RefMapGetFlt, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetRef, RefMapGetRef, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetString, RefMapGetStr, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetValue, RefMapGetVal, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetFirst, RefMapFirst, , 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetNext, RefMapNext, , 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetKeys, RefMapKeys, , 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetAll, RefMapGetAll, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetFloat, RefMapSetFlt, , 0, 3, kParams_JIP_OneString_OneDouble_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetRef, RefMapSetRef, , 0, 3, kParams_JIP_OneString_OneForm_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetString, RefMapSetStr, , 0, 3, kParams_JIP_TwoStrings_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetValue, RefMapSetVal, , 0, 3, kParams_JIP_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayErase, RefMapErase, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayValidate, RefMapValidate, , 0, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayDestroy, RefMapDestroy, , 0, 1, kParams_OneString);

RefMapIDsMap *RMFind(Script *scriptObj, char *varName)
{
	if (!varName[0]) return NULL;
	RefMapInfo varInfo(scriptObj, varName);
	RefMapVarsMap *findMod = varInfo.ModsMap().GetPtr(varInfo.modIndex);
	if (!findMod) return NULL;
	return findMod->GetPtr(varName);
}

bool Cmd_RefMapArrayGetSize_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
	{
		RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
		if (idsMap) *result = (int)idsMap->Size();
	}
	return true;
}

bool Cmd_RefMapArrayGetType_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
		if (idsMap)
		{
			AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj));
			if (value)
				*result = value->GetType();
		}
	}
	return true;
}

bool Cmd_RefMapArrayGetFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
		if (idsMap)
		{
			AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj));
			if (value)
				*result = value->GetFlt();
		}
	}
	return true;
}

bool Cmd_RefMapArrayGetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
		if (idsMap)
		{
			AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj));
			if (value)
				REFR_RES = value->GetRef();
		}
	}
	return true;
}

bool Cmd_RefMapArrayGetString_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
		if (idsMap)
		{
			AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj));
			if (value)
				resStr = value->GetStr();
		}
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_RefMapArrayGetValue_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
		if (idsMap)
		{
			AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj));
			if (value)
			{
				ArrayElementL element(value->GetAsElement());
				AssignCommandResult(CreateArray(&element, 1, scriptObj), result);
			}
		}
	}
	return true;
}

RefMapIDsMap::Iterator s_refMapIterator;

NVSEArrayVar* __fastcall RefMapArrayIterator(Script *scriptObj, char *varName, bool getFirst)
{
	RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
	if (!idsMap) return NULL;
	if (getFirst || (s_refMapIterator.Table() != idsMap))
	{
		s_refMapIterator.Init(*idsMap);
		if (!s_refMapIterator)
			return NULL;
	}
	else if (!s_refMapIterator.IsValid())
		return NULL;
	ArrayElementL elements[2] = {LookupFormByRefID(s_refMapIterator.Key()), s_refMapIterator().GetAsElement()};
	++s_refMapIterator;
	return CreateArray(elements, 2, scriptObj);
}

bool Cmd_RefMapArrayGetFirst_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
	{
		NVSEArrayVar *pairArr = RefMapArrayIterator(scriptObj, varName, true);
		if (pairArr) AssignCommandResult(pairArr, result);
	}
	return true;
}

bool Cmd_RefMapArrayGetNext_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
	{
		NVSEArrayVar *pairArr = RefMapArrayIterator(scriptObj, varName, false);
		if (pairArr) AssignCommandResult(pairArr, result);
	}
	return true;
}

bool Cmd_RefMapArrayGetKeys_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName)) return true;
	RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
	if (!idsMap) return true;
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	for (auto idIter = idsMap->Begin(); idIter; ++idIter)
		tmpElements->Append(LookupFormByRefID(idIter.Key()));
	if (!tmpElements->Empty())
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_RefMapArrayGetAll_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type)) return true;
	RefMapInfo varInfo(scriptObj, type);
	RefMapVarsMap *findMod = varInfo.ModsMap().GetPtr(varInfo.modIndex);
	if (!findMod || findMod->Empty()) return true;
	NVSEArrayVar *varsMap = CreateStringMap(NULL, NULL, 0, scriptObj);
	TempElements *tmpElements = GetTempElements();
	for (auto varIter = findMod->Begin(); varIter; ++varIter)
	{
		tmpElements->Clear();
		for (auto idIter = varIter().Begin(); idIter; ++idIter)
			tmpElements->Append(LookupFormByRefID(idIter.Key()));
		SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj)));
	}
	AssignCommandResult(varsMap, result);
	return true;
}

AuxVariableValue* __fastcall RefMapAddValue(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj, char *varName)
{
	if (varName[0])
	{
		UInt32 keyID = GetSubjectID(form, thisObj);
		if (keyID)
		{
			RefMapInfo varInfo(scriptObj, varName);
			if (varInfo.isPerm)
				s_dataChangedFlags |= kChangedFlag_RefMaps;
			return &varInfo.ModsMap()[varInfo.modIndex][varName][keyID];
		}
	}
	return NULL;
}

bool Cmd_RefMapArraySetFloat_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = NULL;
	double fltVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &fltVal, &form))
	{
		AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName);
		if (value)
			value->SetFlt(fltVal);
	}
	return true;
}

bool Cmd_RefMapArraySetRef_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = NULL, *refVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &refVal, &form))
	{
		AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName);
		if (value)
			value->SetRef(refVal);
	}
	return true;
}

bool Cmd_RefMapArraySetString_Execute(COMMAND_ARGS)
{
	char varName[0x50], *buffer = GetStrArgBuffer();
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, buffer, &form))
	{
		AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName);
		if (value)
			value->SetStr(buffer);
	}
	return true;
}

bool Cmd_RefMapArraySetValue_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	UInt32 arrID;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &arrID, &form))
	{
		NVSEArrayVar *srcArr = LookupArrayByID(arrID);
		if (srcArr && (GetArraySize(srcArr) == 1))
		{
			AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName);
			if (value)
			{
				ArrayElementR element;
				GetElements(srcArr, &element, NULL);
				value->SetElem(element);
			}
		}
	}
	return true;
}

bool Cmd_RefMapArrayErase_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form) || !varName[0]) return true;
	RefMapInfo varInfo(scriptObj, varName);
	auto findMod = varInfo.ModsMap().Find(varInfo.modIndex);
	if (!findMod) return true;
	auto findVar = findMod().Find(varName);
	if (!findVar) return true;
	auto findID = findVar().Find(GetSubjectID(form, thisObj));
	if (!findID) return true;
	findID.Remove();
	if (findVar().Empty())
	{
		findVar.Remove();
		if (findMod().Empty()) findMod.Remove();
	}
	else *result = (int)findVar().Size();
	if (varInfo.isPerm)
		s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}

bool Cmd_RefMapArrayValidate_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName) || !varName[0]) return true;
	RefMapInfo varInfo(scriptObj, varName);
	auto findMod = varInfo.ModsMap().Find(varInfo.modIndex);
	if (!findMod) return true;
	auto findVar = findMod().Find(varName);
	if (!findVar) return true;
	bool cleaned = false;
	for (auto idIter = findVar().Begin(); idIter; ++idIter)
	{
		if (LookupFormByRefID(idIter.Key())) continue;
		idIter.Remove();
		cleaned = true;
	}
	if (findVar().Empty())
	{
		findVar.Remove();
		if (findMod().Empty()) findMod.Remove();
	}
	else *result = (int)findVar().Size();
	if (cleaned && varInfo.isPerm)
		s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}

bool Cmd_RefMapArrayDestroy_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName) || !varName[0]) return true;
	RefMapInfo varInfo(scriptObj, varName);
	auto findMod = varInfo.ModsMap().Find(varInfo.modIndex);
	if (!findMod) return true;
	auto findVar = findMod().Find(varName);
	if (!findVar) return true;
	findVar.Remove();
	if (findMod().Empty()) findMod.Remove();
	if (varInfo.isPerm)
		s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}