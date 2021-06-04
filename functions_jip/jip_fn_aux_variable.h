#pragma once

DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetSize, AuxVarSize, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetType, AuxVarType, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetFloat, AuxVarGetFlt, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetRef, AuxVarGetRef, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetString, AuxVarGetStr, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAsArray, AuxVarGetAsArr, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAll, AuxVarGetAll, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFloat, AuxVarSetFlt, , 0, 4, kParams_JIP_OneString_OneDouble_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetRef, AuxVarSetRef, , 0, 4, kParams_JIP_OneString_OneForm_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetString, AuxVarSetStr, , 0, 4, kParams_JIP_TwoStrings_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFromArray, AuxVarSetFromArr, , 0, 3, kParams_JIP_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableErase, AuxVarErase, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableEraseAll, AuxVarEraseAll, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_CMD_ALT_COND_PLUGIN(AuxVarGetFltCond, , , 1, kParams_JIP_OneQuest_OneInt);

AuxVarValsArr* __fastcall AVGetArray(AuxVarInfo *varInfo, bool addArr = false)
{
	if (addArr) return &varInfo->ModsMap()[varInfo->modIndex][varInfo->ownerID][varInfo->varName];
	AuxVarOwnersMap *ownersMap = varInfo->ModsMap().GetPtr(varInfo->modIndex);
	if (!ownersMap) return NULL;
	AuxVarVarsMap *varsMap = ownersMap->GetPtr(varInfo->ownerID);
	if (!varsMap) return NULL;
	return varsMap->GetPtr(varInfo->varName);
}

AuxVariableValue* __fastcall AVGetValue(AuxVarInfo *varInfo, SInt32 idx, bool addVal = false)
{
	AuxVarValsArr *valsArr = AVGetArray(varInfo, addVal && (idx <= 0));
	if (!valsArr) return NULL;
	SInt32 size = valsArr->Size();
	if (idx < 0)
	{
		if (addVal) return valsArr->Append();
		return size ? valsArr->GetPtr(size - 1) : NULL;
	}
	AuxVariableValue *varValue = valsArr->GetPtr(idx);
	if (varValue) return varValue;
	if (!addVal || (idx > size)) return NULL;
	return valsArr->Append();
}

bool Cmd_AuxiliaryVariableGetSize_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form)) return true;
	AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
	if (!varInfo.ownerID) return true;
	AuxVarValsArr *valsArr = AVGetArray(&varInfo);
	if (valsArr) *result = (int)valsArr->Size();
	return true;
}

bool Cmd_AuxiliaryVariableGetType_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx);
			if (value)
				*result = value->GetType();
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableGetFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx);
			if (value)
				*result = value->GetFlt();
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableGetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx);
			if (value)
				REFR_RES = value->GetRef();
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableGetString_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form) && (idx >= 0))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx);
			if (value)
				resStr = value->GetStr();
		}
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_AuxiliaryVariableGetAsArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form)) return true;
	AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
	if (!varInfo.ownerID) return true;
	AuxVarValsArr *valsArr = AVGetArray(&varInfo);
	if (!valsArr) return true;
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	for (auto value = valsArr->Begin(); value; ++value)
		tmpElements->Append(value().GetAsElement());
	if (!tmpElements->Empty())
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_AuxiliaryVariableGetAll_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form)) return true;
	AuxVarInfo varInfo(form, thisObj, scriptObj, type);
	if (!varInfo.ownerID) return true;
	AuxVarOwnersMap *findMod = varInfo.ModsMap().GetPtr(varInfo.modIndex);
	if (!findMod) return true;
	AuxVarVarsMap *findOwner = findMod->GetPtr(varInfo.ownerID);
	if (!findOwner) return true;
	NVSEArrayVar *varsMap = CreateStringMap(NULL, NULL, 0, scriptObj);
	TempElements *tmpElements = GetTempElements();
	for (auto varIter = findOwner->Begin(); varIter; ++varIter)
	{
		tmpElements->Clear();
		for (auto value = varIter().Begin(); value; ++value)
			tmpElements->Append(value().GetAsElement());
		SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj)));
	}
	if (GetArraySize(varsMap)) AssignCommandResult(varsMap, result);
	return true;
}

bool Cmd_AuxiliaryVariableSetFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	double fltVal;
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &fltVal, &idx, &form))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx, true);
			if (value)
			{
				value->SetFlt(fltVal);
				if (varInfo.isPerm)
					s_dataChangedFlags |= kChangedFlag_AuxVars;
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableSetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *refVal, *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &refVal, &idx, &form))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx, true);
			if (value)
			{
				value->SetRef(refVal);
				if (varInfo.isPerm)
					s_dataChangedFlags |= kChangedFlag_AuxVars;
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableSetString_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50], *buffer = GetStrArgBuffer();
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, buffer, &idx, &form))
	{
		AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
		if (varInfo.ownerID)
		{
			AuxVariableValue *value = AVGetValue(&varInfo, idx, true);
			if (value)
			{
				value->SetStr(buffer);
				if (varInfo.isPerm)
					s_dataChangedFlags |= kChangedFlag_AuxVars;
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_AuxiliaryVariableSetFromArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	char varName[0x50];
	UInt32 arrID;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &arrID, &form) || !arrID)
		return true;
	NVSEArrayVar *srcArr = LookupArrayByID(arrID);
	if (!srcArr) return true;
	UInt32 srcSize;
	ArrayElementR *arrElems = GetArrayData(srcArr, &srcSize);
	if (!arrElems) return true;
	AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
	if (!varInfo.ownerID)
	{
		do
		{
			arrElems->~ElementR();
			arrElems++;
		}
		while (--srcSize);
		return true;
	}
	AuxVarValsArr *valsArr = AVGetArray(&varInfo, true);
	if (!valsArr->Empty())
		valsArr->Clear();
	*result = (int)srcSize;
	do
	{
		valsArr->Append(*arrElems);
		arrElems->~ElementR();
		arrElems++;
	}
	while (--srcSize);
	if (varInfo.isPerm)
		s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

bool Cmd_AuxiliaryVariableErase_Execute(COMMAND_ARGS)
{
	*result = -1;
	char varName[0x50];
	SInt32 idx = -1;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form)) return true;
	AuxVarInfo varInfo(form, thisObj, scriptObj, varName);
	if (!varInfo.ownerID) return true;
	auto findMod = varInfo.ModsMap().Find(varInfo.modIndex);
	if (!findMod) return true;
	auto findOwner = findMod().Find(varInfo.ownerID);
	if (!findOwner) return true;
	auto findVar = findOwner().Find(varInfo.varName);
	if (!findVar) return true;
	if (idx >= 0)
	{
		if (!findVar().RemoveNth(idx)) return true;
		*result = (int)findVar().Size();
		if (findVar().Empty()) findVar.Remove();
	}
	else
	{
		findVar.Remove();
		*result = 0;
	}
	if (findOwner().Empty())
	{
		findOwner.Remove();
		if (findMod().Empty()) findMod.Remove();
	}
	if (varInfo.isPerm)
		s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

bool Cmd_AuxiliaryVariableEraseAll_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form)) return true;
	AuxVarInfo varInfo(form, thisObj, scriptObj, type);
	if (!varInfo.ownerID) return true;
	auto findMod = varInfo.ModsMap().Find(varInfo.modIndex);
	if (!findMod) return true;
	auto findOwner = findMod().Find(varInfo.ownerID);
	if (!findOwner) return true;
	findOwner.Remove();
	if (findMod().Empty()) findMod.Remove();
	if (varInfo.isPerm)
		s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

double __fastcall AuxVarGetFltCond(TESObjectREFR *thisObj, TESQuest *quest, UInt32 varIndex)
{
	if (quest->scriptEventList)
	{
		ScriptVar *scriptVar = quest->scriptEventList->GetVariable(varIndex);
		if (scriptVar)
		{
			const char *varName = GetStringVar((int)scriptVar->data.num);
			if (varName && *varName)
			{
				AuxVarInfo varInfo(NULL, thisObj, quest->scriptable.script, (char*)varName);
				AuxVariableValue *value = AVGetValue(&varInfo, 0);
				if (value) return value->GetFlt();
			}
		}
	}
	return 0;
}

bool Cmd_AuxVarGetFltCond_Eval(COMMAND_ARGS_EVAL)
{
	*result = AuxVarGetFltCond(thisObj, (TESQuest*)arg1, (UInt32)arg2);
	return true;
}

bool Cmd_AuxVarGetFltCond_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 varIndex;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &varIndex))
		*result = AuxVarGetFltCond(thisObj, quest, varIndex);
	else *result = 0;
	return true;
}