#pragma once

DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetSize, RefMapSize, 0, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetType, RefMapGetType, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetFloat, RefMapGetFlt, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetRef, RefMapGetRef, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetString, RefMapGetStr, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetValue, RefMapGetVal, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetFirst, RefMapFirst, 0, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetNext, RefMapNext, 0, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetKeys, RefMapKeys, 0, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayGetAll, RefMapGetAll, 0, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetFloat, RefMapSetFlt, 0, kParams_OneString_OneDouble_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetRef, RefMapSetRef, 0, kParams_OneString_OneForm_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetString, RefMapSetStr, 0, kParams_TwoStrings_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArraySetValue, RefMapSetVal, 0, kParams_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayErase, RefMapErase, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayValidate, RefMapValidate, 0, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(RefMapArrayDestroy, RefMapDestroy, 0, kParams_OneString);

#define REF_MAP_CS	ScopedPrimitiveCS cs(&s_refMapCS);

RefMapIDsMap *RMFind(Script *scriptObj, char *varName)
{
	if (*varName)
	{
		RefMapInfo varInfo(scriptObj, varName);
		if (RefMapVarsMap *findMod = varInfo().GetPtr(varInfo.modIndex))
			return findMod->GetPtr(varName);
	}
	return nullptr;
}

bool Cmd_RefMapArrayGetSize_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
			*result = (int)idsMap->Size();
	}
	return true;
}

bool Cmd_RefMapArrayGetType_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
			if (AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj)))
				*result = value->GetType();
	}
	return true;
}

bool Cmd_RefMapArrayGetFloat_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
			if (AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj)))
				*result = value->GetFlt();
	}
	return true;
}

bool Cmd_RefMapArrayGetRef_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
			if (AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj)))
				REFR_RES = value->GetRef();
	}
	return true;
}

bool Cmd_RefMapArrayGetString_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
			if (AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj)))
				resStr = value->GetStr();
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_RefMapArrayGetValue_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
			if (AuxVariableValue *value = idsMap->GetPtr(GetSubjectID(form, thisObj)))
			{
				ArrayElementL element(value->GetAsElement());
				*result = (int)CreateArray(&element, 1, scriptObj);
			}
	}
	return true;
}

RefMapIDsMap::Iterator s_refMapIterator;

NVSEArrayVar* __fastcall RefMapArrayIterator(Script *scriptObj, char *varName, bool getFirst)
{
	REF_MAP_CS
	RefMapIDsMap *idsMap = RMFind(scriptObj, varName);
	if (!idsMap) return nullptr;
	if (getFirst || (s_refMapIterator.Table() != idsMap))
	{
		s_refMapIterator.Init(*idsMap);
		if (!s_refMapIterator)
			return nullptr;
	}
	else if (!s_refMapIterator.IsValid())
		return nullptr;
	ArrayElementL elements[2] = {LookupFormByRefID(s_refMapIterator.Key()), s_refMapIterator().GetAsElement()};
	++s_refMapIterator;
	return CreateArray(elements, 2, scriptObj);
}

bool Cmd_RefMapArrayGetFirst_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
		*result = (int)RefMapArrayIterator(scriptObj, varName, true);
	return true;
}

bool Cmd_RefMapArrayGetNext_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
		*result = (int)RefMapArrayIterator(scriptObj, varName, false);
	return true;
}

bool Cmd_RefMapArrayGetKeys_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName))
	{
		REF_MAP_CS
		if (RefMapIDsMap *idsMap = RMFind(scriptObj, varName))
		{
			TempElements *tmpElements = GetTempElements();
			for (auto idIter = idsMap->Begin(); idIter; ++idIter)
				tmpElements->Append(LookupFormByRefID(idIter.Key()));
			if (!tmpElements->Empty())
				*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
		}
	}
	return true;
}

bool Cmd_RefMapArrayGetAll_Execute(COMMAND_ARGS)
{
	UInt32 type;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type))
	{
		RefMapInfo varInfo(scriptObj, type);
		REF_MAP_CS
		if (RefMapVarsMap *findMod = varInfo().GetPtr(varInfo.modIndex); findMod && !findMod->Empty())
		{
			NVSEArrayVar *varsMap = CreateStringMap(nullptr, nullptr, 0, scriptObj);
			TempElements *tmpElements = GetTempElements();
			for (auto varIter = findMod->Begin(); varIter; ++varIter)
			{
				for (auto idIter = varIter().Begin(); idIter; ++idIter)
					tmpElements->Append(LookupFormByRefID(idIter.Key()));
				SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj)));
				tmpElements->Clear();
			}
			*result = (int)varsMap;
		}
	}
	return true;
}

AuxVariableValue* __fastcall RefMapAddValue(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj, char *varName)
{
	if (varName[0])
		if (UInt32 keyID = GetSubjectID(form, thisObj))
		{
			RefMapInfo varInfo(scriptObj, varName);
			if (!varInfo.isTemp)
			{
				s_dataChangedFlags |= kChangedFlag_RefMaps;
				if (thisObj)
					thisObj->MarkModified(0);
			}
			return &varInfo()[varInfo.modIndex][varName][keyID];
		}
	return nullptr;
}

bool Cmd_RefMapArraySetFloat_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	double fltVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &fltVal, &form))
	{
		REF_MAP_CS
		if (AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName))
			*value = fltVal;
	}
	return true;
}

bool Cmd_RefMapArraySetRef_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr, *refVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &refVal, &form))
	{
		REF_MAP_CS
		if (AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName))
			*value = refVal;
	}
	return true;
}

bool Cmd_RefMapArraySetString_Execute(COMMAND_ARGS)
{
	char varName[0x50], *buffer = GetStrArgBuffer();
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, buffer, &form))
	{
		REF_MAP_CS
		if (AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName))
			*value = buffer;
	}
	return true;
}

bool Cmd_RefMapArraySetValue_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	UInt32 arrID;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &arrID, &form))
		if (NVSEArrayVar *srcArr = LookupArrayByID(arrID); srcArr && (GetArraySize(srcArr) == 1))
		{
			REF_MAP_CS
			if (AuxVariableValue *value = RefMapAddValue(form, thisObj, scriptObj, varName))
			{
				ArrayElementR element;
				GetElements(srcArr, &element, nullptr);
				*value = element;
			}
		}
	return true;
}

bool Cmd_RefMapArrayErase_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form) && varName[0])
	{
		RefMapInfo varInfo(scriptObj, varName);
		REF_MAP_CS
		if (auto findMod = varInfo().Find(varInfo.modIndex))
			if (auto findVar = findMod().Find(varName))
				if (auto findID = findVar().Find(GetSubjectID(form, thisObj)))
				{
					findID.Remove();
					if (findVar().Empty())
					{
						findVar.Remove();
						if (findMod().Empty()) findMod.Remove();
					}
					else *result = (int)findVar().Size();
					if (!varInfo.isTemp)
						s_dataChangedFlags |= kChangedFlag_RefMaps;
				}
	}
	return true;
}

bool Cmd_RefMapArrayValidate_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName) && varName[0])
	{
		RefMapInfo varInfo(scriptObj, varName);
		REF_MAP_CS
		if (auto findMod = varInfo().Find(varInfo.modIndex))
			if (auto findVar = findMod().Find(varName))
			{
				bool cleaned = false;
				for (auto idIter = findVar().Begin(); idIter; ++idIter)
					if (!LookupFormByRefID(idIter.Key()))
					{
						idIter.Remove();
						cleaned = true;
					}
				if (findVar().Empty())
				{
					findVar.Remove();
					if (findMod().Empty()) findMod.Remove();
				}
				else *result = (int)findVar().Size();
				if (cleaned && !varInfo.isTemp)
					s_dataChangedFlags |= kChangedFlag_RefMaps;
			}
	}
	return true;
}

bool Cmd_RefMapArrayDestroy_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName) && varName[0])
	{
		RefMapInfo varInfo(scriptObj, varName);
		REF_MAP_CS
		if (auto findMod = varInfo().Find(varInfo.modIndex))
			if (auto findVar = findMod().Find(varName))
			{
				findVar.Remove();
				if (findMod().Empty()) findMod.Remove();
				if (!varInfo.isTemp)
					s_dataChangedFlags |= kChangedFlag_RefMaps;
			}
	}
	return true;
}