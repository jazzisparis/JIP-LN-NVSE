#pragma once

DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetSize, AuxVarSize, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetType, AuxVarType, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetFloat, AuxVarGetFlt, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetRef, AuxVarGetRef, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetString, AuxVarGetStr, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAsArray, AuxVarGetAsArr, , 0, 2, kParams_JIP_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAll, AuxVarGetAll, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFloat, AuxVarSetFlt, , 0, 4, kParams_JIP_OneString_OneFloat_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetRef, AuxVarSetRef, , 0, 4, kParams_JIP_OneString_OneForm_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetString, AuxVarSetStr, , 0, 4, kParams_JIP_TwoStrings_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFromArray, AuxVarSetFromArr, , 0, 3, kParams_JIP_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableErase, AuxVarErase, , 0, 3, kParams_JIP_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableEraseAll, AuxVarEraseAll, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_CMD_ALT_COND_PLUGIN(AuxVarGetFltCond, , , 1, kParams_JIP_OneQuest_OneInt);

#define AV_Map (s_avIsPerm ? s_auxVariablesPerm : s_auxVariablesTemp)

AuxVarValsArr* __fastcall AVGetArray(UInt32 ownerID, bool addArr = false)
{
	s_strArgBuffer[255] = 0;
	if (addArr) return &AV_Map[s_avModIdx][ownerID][s_strArgBuffer];
	AuxVarOwnersMap *ownersMap = AV_Map.GetPtr(s_avModIdx);
	if (!ownersMap) return NULL;
	AuxVarVarsMap *varsMap = ownersMap->GetPtr(ownerID);
	if (!varsMap) return NULL;
	return varsMap->GetPtr(s_strArgBuffer);
}

AuxVariableValue* __fastcall AVGetValue(UInt32 ownerID, SInt32 idx, bool addVal = false)
{
	AuxVarValsArr *valsArr = AVGetArray(ownerID, addVal && (idx <= 0));
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
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form)) return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	GetBaseParams(scriptObj);
	AuxVarValsArr *valsArr = AVGetArray(ownerID);
	if (valsArr) *result = (int)valsArr->Size();
	return true;
}

UInt8 s_auxVarOperationType = 0;

bool AuxiliaryVariableGet_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	SInt32 idx = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &idx, &form) && (idx >= 0))
	{
		UInt32 ownerID = GetSubjectID(form, thisObj);
		if (ownerID)
		{
			GetBaseParams(scriptObj);
			AuxVariableValue *value = AVGetValue(ownerID, idx);
			if (value)
			{
				switch (s_auxVarOperationType)
				{
				case 0:
					*result = value->GetType();
					return true;
				case 1:
					*result = value->GetFlt();
					return true;
				case 2:
					*(UInt64*)result = value->GetRef();
					return true;
				default:
					resStr = value->GetStr();
				}
			}
		}
	}
	if (s_auxVarOperationType == 3) AssignString(PASS_COMMAND_ARGS, resStr);
	else *result = 0;
	return true;
}

__declspec(naked) bool Cmd_AuxiliaryVariableGetType_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 0
		jmp		AuxiliaryVariableGet_Execute
	}
}

__declspec(naked) bool Cmd_AuxiliaryVariableGetFloat_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 1
		jmp		AuxiliaryVariableGet_Execute
	}
}

__declspec(naked) bool Cmd_AuxiliaryVariableGetRef_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 2
		jmp		AuxiliaryVariableGet_Execute
	}
}

__declspec(naked) bool Cmd_AuxiliaryVariableGetString_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 3
		jmp		AuxiliaryVariableGet_Execute
	}
}

bool Cmd_AuxiliaryVariableGetAsArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &form)) return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	GetBaseParams(scriptObj);
	AuxVarValsArr *valsArr = AVGetArray(ownerID);
	if (!valsArr) return true;
	s_tempElements.Clear();
	for (auto value = valsArr->Begin(); value; ++value)
		s_tempElements.Append(value().GetAsElement());
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_AuxiliaryVariableGetAll_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form)) return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	GetBaseParams(scriptObj, type);
	AuxVarOwnersMap *findMod = AV_Map.GetPtr(s_avModIdx);
	if (!findMod) return true;
	AuxVarVarsMap *findOwner = findMod->GetPtr(ownerID);
	if (!findOwner) return true;
	NVSEArrayVar *varsMap = CreateStringMap(NULL, NULL, 0, scriptObj);
	for (auto varIter = findOwner->Begin(); varIter; ++varIter)
	{
		s_tempElements.Clear();
		for (auto value = varIter().Begin(); value; ++value)
			s_tempElements.Append(value().GetAsElement());
		SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj)));
	}
	if (GetArraySize(varsMap)) AssignCommandResult(varsMap, result);
	return true;
}

bool AuxiliaryVariableSet_Execute(COMMAND_ARGS)
{
	*result = 0;
	float fltVal;
	SInt32 idx = 0;
	TESForm *refVal, *form = NULL;
	switch (s_auxVarOperationType)
	{
	case 0:
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &fltVal, &idx, &form)) return true;
		break;
	case 1:
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &refVal, &idx, &form)) return true;
		break;
	default:
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &s_strValBuffer, &idx, &form)) return true;
	}
	if (!s_strArgBuffer[0]) return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	GetBaseParams(scriptObj);
	AuxVariableValue *value = AVGetValue(ownerID, idx, true);
	if (!value) return true;
	switch (s_auxVarOperationType)
	{
	case 0:
		value->SetFlt(fltVal);
		break;
	case 1:
		value->SetRef(refVal);
		break;
	default:
		value->SetStr(s_strValBuffer);
	}
	*result = 1;
	if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

__declspec(naked) bool Cmd_AuxiliaryVariableSetFloat_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 0
		jmp		AuxiliaryVariableSet_Execute
	}
}

__declspec(naked) bool Cmd_AuxiliaryVariableSetRef_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 1
		jmp		AuxiliaryVariableSet_Execute
	}
}

__declspec(naked) bool Cmd_AuxiliaryVariableSetString_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_auxVarOperationType, 2
		jmp		AuxiliaryVariableSet_Execute
	}
}

bool Cmd_AuxiliaryVariableSetFromArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 arrID;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &arrID, &form) || !s_strArgBuffer[0] || !arrID)
		return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	NVSEArrayVar *srcArr = LookupArrayByID(arrID);
	if (!srcArr) return true;
	UInt32 srcSize;
	ArrayElementR *arrElems = GetArrayData(srcArr, &srcSize);
	if (!arrElems) return true;
	GetBaseParams(scriptObj);
	AuxVarValsArr *valsArr = AVGetArray(ownerID, true);
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
	if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

bool Cmd_AuxiliaryVariableErase_Execute(COMMAND_ARGS)
{
	*result = -1;
	SInt32 idx = -1;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &idx, &form)) return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	GetBaseParams(scriptObj);
	auto findMod = AV_Map.Find(s_avModIdx);
	if (!findMod) return true;
	auto findOwner = findMod().Find(ownerID);
	if (!findOwner) return true;
	s_strArgBuffer[255] = 0;
	auto findVar = findOwner().Find(s_strArgBuffer);
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
	if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

bool Cmd_AuxiliaryVariableEraseAll_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form)) return true;
	UInt32 ownerID = GetSubjectID(form, thisObj);
	if (!ownerID) return true;
	GetBaseParams(scriptObj, type);
	auto findMod = AV_Map.Find(s_avModIdx);
	if (!findMod) return true;
	auto findOwner = findMod().Find(ownerID);
	if (!findOwner) return true;
	findOwner.Remove();
	if (findMod().Empty()) findMod.Remove();
	if (s_avIsPerm) s_dataChangedFlags |= kChangedFlag_AuxVars;
	return true;
}

float __fastcall AuxVarGetFltCond(TESObjectREFR *thisObj, TESQuest *quest, UInt32 varIndex)
{
	if (quest->scriptEventList)
	{
		ScriptVar *scriptVar = quest->scriptEventList->GetVariable(varIndex);
		if (scriptVar)
		{
			const char *varName = GetStringVar((int)scriptVar->data.num);
			if (varName && *varName)
			{
				StrCopy(s_strArgBuffer, varName);
				GetBaseParams(quest->scriptable.script);
				AuxVariableValue *value = AVGetValue(thisObj->refID, 0);
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