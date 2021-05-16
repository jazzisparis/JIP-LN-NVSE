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

#define RM_Map (s_avIsPerm ? s_refMapArraysPerm : s_refMapArraysTemp)

RefMapIDsMap *RMFind()
{
	RefMapVarsMap *findMod = RM_Map.GetPtr(s_avModIdx);
	if (!findMod) return NULL;
	s_strArgBuffer[255] = 0;
	return findMod->GetPtr(s_strArgBuffer);
}

bool Cmd_RefMapArrayGetSize_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		GetBaseParams(scriptObj);
		RefMapIDsMap *idsMap = RMFind();
		if (idsMap) *result = (int)idsMap->Size();
	}
	return true;
}

AuxVariableValue *RefMapGetValue(Script *scriptObj, TESForm *form, TESObjectREFR *thisObj)
{
	GetBaseParams(scriptObj);
	RefMapIDsMap *idsMap = RMFind();
	return idsMap ? idsMap->GetPtr(GetSubjectID(form, thisObj)) : NULL;
}

bool Cmd_RefMapArrayGetType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form))
	{
		AuxVariableValue *value = RefMapGetValue(scriptObj, form, thisObj);
		if (value)
			*result = value->GetType();
	}
	return true;
}

bool Cmd_RefMapArrayGetFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form))
	{
		AuxVariableValue *value = RefMapGetValue(scriptObj, form, thisObj);
		if (value)
			*result = value->GetFlt();
	}
	return true;
}

bool Cmd_RefMapArrayGetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form))
	{
		AuxVariableValue *value = RefMapGetValue(scriptObj, form, thisObj);
		if (value)
			REFR_RES = value->GetRef();
	}
	return true;
}

bool Cmd_RefMapArrayGetString_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form))
	{
		AuxVariableValue *value = RefMapGetValue(scriptObj, form, thisObj);
		if (value)
			resStr = value->GetStr();
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_RefMapArrayGetValue_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form))
	{
		AuxVariableValue *value = RefMapGetValue(scriptObj, form, thisObj);
		if (value)
		{
			ArrayElementL element(value->GetAsElement());
			AssignCommandResult(CreateArray(&element, 1, scriptObj), result);
		}
	}
	return true;
}

RefMapIDsMap::Iterator s_refMapIterator;

NVSEArrayVar* __fastcall RefMapArrayIterator(Script *scriptObj, bool getFirst)
{
	GetBaseParams(scriptObj);
	RefMapIDsMap *idsMap = RMFind();
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		NVSEArrayVar *pairArr = RefMapArrayIterator(scriptObj, true);
		if (pairArr) AssignCommandResult(pairArr, result);
	}
	return true;
}

bool Cmd_RefMapArrayGetNext_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		NVSEArrayVar *pairArr = RefMapArrayIterator(scriptObj, false);
		if (pairArr) AssignCommandResult(pairArr, result);
	}
	return true;
}

bool Cmd_RefMapArrayGetKeys_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer)) return true;
	GetBaseParams(scriptObj);
	RefMapIDsMap *idsMap = RMFind();
	if (!idsMap) return true;
	s_tempElements.Clear();
	for (auto idIter = idsMap->Begin(); idIter; ++idIter)
		s_tempElements.Append(LookupFormByRefID(idIter.Key()));
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_RefMapArrayGetAll_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type)) return true;
	GetBaseParams(scriptObj, type);
	RefMapVarsMap *findMod = RM_Map.GetPtr(s_avModIdx);
	if (!findMod || findMod->Empty()) return true;
	NVSEArrayVar *varsMap = CreateStringMap(NULL, NULL, 0, scriptObj);
	for (auto varIter = findMod->Begin(); varIter; ++varIter)
	{
		s_tempElements.Clear();
		for (auto idIter = varIter().Begin(); idIter; ++idIter)
			s_tempElements.Append(LookupFormByRefID(idIter.Key()));
		SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj)));
	}
	AssignCommandResult(varsMap, result);
	return true;
}

AuxVariableValue* __fastcall RefMapAddValue(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj)
{
	if (s_strArgBuffer[0])
	{
		UInt32 keyID = GetSubjectID(form, thisObj);
		if (keyID)
		{
			GetBaseParams(scriptObj);
			s_strArgBuffer[255] = 0;
			if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_RefMaps;
			return &RM_Map[s_avModIdx][s_strArgBuffer][keyID];
		}
	}
	return NULL;
}

bool Cmd_RefMapArraySetFloat_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	double fltVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &fltVal, &form))
	{
		AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj);
		if (value)
			value->SetFlt(fltVal);
	}
	return true;
}

bool Cmd_RefMapArraySetRef_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL, *refVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &refVal, &form))
	{
		AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj);
		if (value)
			value->SetRef(refVal);
	}
	return true;
}

bool Cmd_RefMapArraySetString_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &s_strValBuffer, &form))
	{
		AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj);
		if (value)
			value->SetStr(s_strValBuffer);
	}
	return true;
}

bool Cmd_RefMapArraySetValue_Execute(COMMAND_ARGS)
{
	UInt32 arrID;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &arrID, &form))
	{
		NVSEArrayVar *srcArr = LookupArrayByID(arrID);
		if (srcArr && (GetArraySize(srcArr) == 1))
		{
			AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj);
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
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form)) return true;
	GetBaseParams(scriptObj);
	auto findMod = RM_Map.Find(s_avModIdx);
	if (!findMod) return true;
	auto findVar = findMod().Find(s_strArgBuffer);
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
	if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}

bool Cmd_RefMapArrayValidate_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer)) return true;
	GetBaseParams(scriptObj);
	auto findMod = RM_Map.Find(s_avModIdx);
	if (!findMod) return true;
	auto findVar = findMod().Find(s_strArgBuffer);
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
	if (cleaned && s_avIsPerm) s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}

bool Cmd_RefMapArrayDestroy_Execute(COMMAND_ARGS)
{
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer)) return true;
	GetBaseParams(scriptObj);
	auto findMod = RM_Map.Find(s_avModIdx);
	if (!findMod) return true;
	auto findVar = findMod().Find(s_strArgBuffer);
	if (!findVar) return true;
	findVar.Remove();
	if (findMod().Empty()) findMod.Remove();
	if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_RefMaps;
	return true;
}