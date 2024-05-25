#pragma once

DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetSize, AuxVarSize, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetType, AuxVarType, 0, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetFloat, AuxVarGetFlt, 0, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetRef, AuxVarGetRef, 0, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetString, AuxVarGetStr, 0, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAsArray, AuxVarGetAsArr, 0, kParams_OneString_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableGetAll, AuxVarGetAll, 0, kParams_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFloat, AuxVarSetFlt, 0, kParams_OneString_OneDouble_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetRef, AuxVarSetRef, 0, kParams_OneString_OneForm_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetString, AuxVarSetStr, 0, kParams_TwoStrings_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableSetFromArray, AuxVarSetFromArr, 0, kParams_OneString_OneInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableErase, AuxVarErase, 0, kParams_OneString_OneOptionalInt_OneOptionalForm);
DEFINE_COMMAND_ALT_PLUGIN(AuxiliaryVariableEraseAll, AuxVarEraseAll, 0, kParams_OneInt_OneOptionalForm);
DEFINE_CMD_COND_ONLY(AuxVarGetFltCond, kParams_OneQuest_OneInt);

#define AUX_VAR_CS	ScopedPrimitiveCS cs(&s_auxVarCS);

__declspec(noinline) AuxVarValsArr* __fastcall AuxVarInfo::GetArray(bool addArr)
{
	if (addArr) 
		return &(*this)()[modIndex][ownerID][varName];
	if (AuxVarOwnersMap *ownersMap = (*this)().GetPtr(modIndex))
		if (AuxVarVarsMap *varsMap = ownersMap->GetPtr(ownerID))
			return varsMap->GetPtr(varName);
	return nullptr;
}

__declspec(noinline) AuxVariableValue* __fastcall AuxVarInfo::GetValue(SInt32 idx, bool addVal)
{
	if (AuxVarValsArr *valsArr = GetArray(addVal && (idx <= 0)))
	{
		SInt32 size = valsArr->Size();
		if (addVal && ((idx < 0) || (idx == size)))
			return valsArr->Append();
		if (size && (idx < size))
		{
			if (idx < 0)
				idx = size - 1;
			return &(*valsArr)[idx];
		}
	}
	return nullptr;
}

void __fastcall MarkVarModified(TESObjectREFR *thisObj)
{
	s_dataChangedFlags |= kChangedFlag_AuxVars;
	if (thisObj)
		thisObj->MarkModified(0);
}

bool Cmd_AuxiliaryVariableGetSize_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVarValsArr *valsArr = varInfo.GetArray())
				*result = (int)valsArr->Size();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetType_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx))
				*result = value->GetType();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetFloat_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx))
				*result = value->GetFlt();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetRef_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx))
				REFR_RES = value->GetRef();
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetString_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx))
				resStr = value->GetStr();
		}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_AuxiliaryVariableGetAsArray_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVarValsArr *valsArr = varInfo.GetArray())
			{
				TempElements *tmpElements = GetTempElements();
				for (auto value = valsArr->Begin(); value; ++value)
					tmpElements->Append(value().GetAsElement());
				if (!tmpElements->Empty())
					*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableGetAll_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, type})
		{
			AUX_VAR_CS
			if (AuxVarOwnersMap *findMod = varInfo().GetPtr(varInfo.modIndex))
				if (AuxVarVarsMap *findOwner = findMod->GetPtr(varInfo.ownerID))
				{
					NVSEArrayVar *varsMap = CreateStringMap(nullptr, nullptr, 0, scriptObj);
					TempElements *tmpElements = GetTempElements();
					for (auto varIter = findOwner->Begin(); varIter; ++varIter)
					{
						for (auto value = varIter().Begin(); value; ++value)
							tmpElements->Append(value().GetAsElement());
						SetElement(varsMap, ArrayElementL(varIter.Key()), ArrayElementL(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj)));
						tmpElements->Clear();
					}
					*result = (int)varsMap;
				}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetFloat_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	double fltVal;
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &fltVal, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx, true))
			{
				*value = fltVal;
				if (!varInfo.isTemp)
					MarkVarModified(thisObj);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetRef_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	SInt32 idx = 0;
	TESForm *refVal, *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &refVal, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx, true))
			{
				*value = refVal;
				if (!varInfo.isTemp)
					MarkVarModified(thisObj);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetString_Execute(COMMAND_ARGS)
{
	char varName[0x50], *buffer = GetStrArgBuffer();
	SInt32 idx = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, buffer, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (AuxVariableValue *value = varInfo.GetValue(idx, true))
			{
				*value = buffer;
				if (!varInfo.isTemp)
					MarkVarModified(thisObj);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_AuxiliaryVariableSetFromArray_Execute(COMMAND_ARGS)
{
	char varName[0x50];
	UInt32 arrID;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &arrID, &form) && arrID)
		if (NVSEArrayVar *srcArr = LookupArrayByID(arrID))
			if (ArrayData arrData(srcArr, true); arrData.size)
				if (AuxVarInfo varInfo(form, thisObj, scriptObj, varName); varInfo.ownerID)
				{
					AUX_VAR_CS
					AuxVarValsArr *valsArr = varInfo.GetArray(true);
					if (!valsArr->Empty())
						valsArr->Clear();
					for (UInt32 idx = 0; idx < arrData.size; idx++)
						valsArr->Append(arrData.vals[idx]);
					if (!varInfo.isTemp)
						MarkVarModified(thisObj);
					*result = (int)arrData.size;
				}
	return true;
}

bool Cmd_AuxiliaryVariableErase_Execute(COMMAND_ARGS)
{
	*result = -1;
	char varName[0x50];
	SInt32 idx = -1;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &varName, &idx, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, varName})
		{
			AUX_VAR_CS
			if (auto findMod = varInfo().Find(varInfo.modIndex))
				if (auto findOwner = findMod().Find(varInfo.ownerID))
					if (auto findVar = findOwner().Find(varInfo.varName))
					{
						if (idx >= 0)
						{
							if (!findVar().RemoveNth(idx))
								return true;
							*result = (int)findVar().Size();
							if (findVar().Empty()) findVar.Remove();
						}
						else findVar.Remove();
						if (findOwner().Empty())
						{
							findOwner.Remove();
							if (findMod().Empty()) findMod.Remove();
						}
						if (!varInfo.isTemp)
							s_dataChangedFlags |= kChangedFlag_AuxVars;
					}
		}
	return true;
}

bool Cmd_AuxiliaryVariableEraseAll_Execute(COMMAND_ARGS)
{
	UInt32 type;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &form))
		if (AuxVarInfo varInfo = {form, thisObj, scriptObj, type})
		{
			AUX_VAR_CS
			if (auto findMod = varInfo().Find(varInfo.modIndex))
				if (auto findOwner = findMod().Find(varInfo.ownerID))
				{
					findOwner.Remove();
					if (findMod().Empty()) findMod.Remove();
					if (!varInfo.isTemp)
						s_dataChangedFlags |= kChangedFlag_AuxVars;
				}
		}
	return true;
}

bool Cmd_AuxVarGetFltCond_Eval(COMMAND_ARGS_EVAL)
{
	if (TESQuest *quest = (TESQuest*)arg1; quest->scriptEventList)
		if (ScriptVar *scriptVar = quest->scriptEventList->GetVariable((UInt32)arg2))
			if (const char *varName = GetStringVar((int)scriptVar->data.num); varName && *varName)
			{
				AuxVarInfo varInfo(nullptr, thisObj, quest->scriptable.script, (char*)varName);
				AUX_VAR_CS
				if (AuxVariableValue *value = varInfo.GetValue(0))
					*result = value->GetFlt();
			}
	return true;
}