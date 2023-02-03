#pragma once

DEFINE_COMMAND_ALT_PLUGIN(GetLoadedType, GLT, 0, 4, kParams_OneList_OneInt_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(GetLoadedTypeArray, GLTA, 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(Search, 0, 2, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetModName, 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetFormMods, 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetFormRefs, 0, 2, kParams_OneForm_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetSelfModIndex, 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(IsFormOverridden, 0, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetFormFromMod, GFFM, 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(GetStringSetting, 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetStringSetting, 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(GetGameSettings, 0, 1, kParams_OneOptionalInt);

const UInt8 kTypeListJmpTbl[] =
{
	255, 255, 255, 255, 128, 50, 28, 15, 16, 8, 9, 10, 11, 25, 26, 255, 255, 24, 13, 6, 7, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 
	128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 48, 128, 39, 128, 5, 2, 58, 255, 255, 255, 255, 
	255, 255, 255, 255, 255, 1, 255, 255, 255, 32, 33, 34, 255, 0, 35, 36, 128, 51, 37, 38, 255, 40, 45, 3, 4, 49, 46, 47, 128, 255, 41, 14, 
	255, 29, 30, 31, 128, 12, 52, 27, 255, 53, 54, 128, 17, 255, 19, 20, 128, 22, 55, 56, 57, 18, 21, 128, 128, 23, 42, 43, 44
};

tList<TESForm> *GetSourceList(UInt8 formType)
{
	if (formType == kFormType_TESRegion)
		return (tList<TESForm>*)&g_dataHandler->regionList->list;
	UInt8 listIdx = kTypeListJmpTbl[formType];
	if (listIdx >= 0x80) return NULL;
	return &((tList<TESForm>*)&g_dataHandler->packageList)[listIdx];
}

void GetLoadedType(UInt32 formType, int index, tList<TESForm> *outList, TempElements *tmpElements)
{
	if (formType == kFormType_TESObjectCELL)
	{
		TESObjectCELL **cells = g_dataHandler->cellArray.data;
		for (UInt32 count = g_dataHandler->cellArray.Length(); count; count--, cells++)
		{
			if ((index != -1) && (index != (*cells)->modIndex)) continue;
			if (outList) outList->Prepend(*cells);
			else tmpElements->Append(*cells);
		}
	}
	else if (formType == 301)
	{
		ListNode<TESWorldSpace> *wspcIter = g_dataHandler->worldSpaceList.Head();
		TESWorldSpace *wspc;
		ListNode<TESObjectREFR> *refrIter;
		TESObjectREFR *refr;
		do
		{
			if (!(wspc = wspcIter->data)) continue;
			refrIter = wspc->cell->objectList.Head();
			do
			{
				refr = refrIter->data;
				if (!refr || !refr->extraDataList.HasType(kXData_ExtraMapMarker) || ((index != -1) && (index != refr->modIndex))) continue;
				if (outList) outList->Prepend(refr);
				else tmpElements->Append(refr);
			}
			while (refrIter = refrIter->next);
		}
		while (wspcIter = wspcIter->next);
	}
	else if (formType == 302)
	{
		TESObjectCELL **cells = g_dataHandler->cellArray.data;
		ListNode<TESObjectREFR> *refrIter;
		TESObjectREFR *refr;
		for (UInt32 count = g_dataHandler->cellArray.Length(); count; count--, cells++)
		{
			refrIter = (*cells)->objectList.Head();
			do
			{
				refr = refrIter->data;
				if (!refr || !refr->extraDataList.HasType(kXData_ExtraRadioData) || ((index != -1) && (index != refr->modIndex))) continue;
				if (outList) outList->Prepend(refr);
				else tmpElements->Append(refr);
			}
			while (refrIter = refrIter->next);
		}
	}
	else if (formType < kFormType_Max)
	{
		tList<TESForm> *sourceList = GetSourceList(formType);
		TESForm *form;
		if (sourceList)
		{
			ListNode<TESForm> *iter = sourceList->Head();
			do
			{
				form = iter->data;
				if (!form || ((index != -1) && (index != form->modIndex))) continue;
				if (outList) outList->Prepend(form);
				else tmpElements->Append(form);
			}
			while (iter = iter->next);
		}
		else if (kTypeListJmpTbl[formType] == 0x80)
		{
			for (TESBoundObject *object = g_dataHandler->boundObjectList->first; object; object = object->next)
			{
				if ((object->typeID != formType) || ((index != -1) && (index != object->modIndex))) continue;
				if (outList) outList->Prepend(object);
				else tmpElements->Append(object);
			}
		}
		else
		{
			for (auto mIter = GameGlobals::AllFormsMap()->Begin(); mIter; ++mIter)
			{
				form = mIter.Get();
				if (!form || (form->typeID != formType) || ((index != -1) && (index != form->modIndex))) continue;
				if (outList) outList->Prepend(form);
				else tmpElements->Append(form);
			}
		}
	}
}

bool Cmd_GetLoadedType_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	UInt32 formType, noClear = 0;
	int index = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm, &formType, &index, &noClear))
	{
		if (!noClear) listForm->list.RemoveAll();
		GetLoadedType(formType, index, &listForm->list, NULL);
	}
	return true;
}

bool Cmd_GetLoadedTypeArray_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 formType;
	int index = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &formType, &index))
	{
		TempElements *tmpElements = GetTempElements();
		GetLoadedType(formType, index, NULL, tmpElements);
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

const bool kValidSearchTypes[] =
{
	0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0
};

bool Cmd_Search_Execute(COMMAND_ARGS)
{
	char toFind[0x40], typeStr[0x10];
	typeStr[0] = 0;
	if (!IsConsoleOpen() || !ExtractArgsEx(EXTRACT_ARGS_EX, &toFind, &typeStr))
		return true;
	UInt32 filter = 0;
	char first = typeStr[0];
	if (first)
	{
		if ((first >= '0') && (first <= '9'))
		{
			filter = StrToInt(typeStr);
			if ((filter > 120) || !kValidSearchTypes[filter])
				return true;
		}
		else
		{
			UInt32 uType = *(UInt32*)typeStr & 0xDFDFDFDF;
			for (UInt32 idx = 3; idx <= 120; idx++)
			{
				if (uType != *(UInt32*)TypeSignature::Array()[idx].signature)
					continue;
				filter = 120 - idx;
				break;
			}
			if (!kValidSearchTypes[filter])
				return true;
		}
	}
	bool validTypes[124], *pValidTypes;
	if (!filter)
		pValidTypes = (bool*)kValidSearchTypes;
	else
	{
		ZERO_BYTES(validTypes, sizeof(validTypes));
		validTypes[filter] = true;
		pValidTypes = validTypes;
	}
	TESForm *form;
	const char *pName, *pEDID;
	for (auto frmIter = GameGlobals::AllFormsMap()->Begin(); frmIter; ++frmIter)
	{
		form = frmIter.Get();
		if (!form || !pValidTypes[form->typeID])
			continue;
		pName = form->GetTheName();
		pEDID = form->GetEditorID();
		if ((*pName && SubStrCI(pName, toFind)) || (*pEDID && SubStrCI(pEDID, toFind)))
			Console_Print("[%08X]  %s  %s  %s", form->refID, TypeSignature::Array()[120 - form->typeID].signature, pEDID, pName);
	}
	return true;
}

bool Cmd_GetModName_Execute(COMMAND_ARGS)
{
	char modName[0x80];
	modName[0] = 0;
	UInt32 index, keepExt = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &index, &keepExt))
	{
		StrCopy(modName, g_dataHandler->GetNthModName(index));
		if (modName[0] && !keepExt) GetNextToken(modName, '.');
	}
	AssignString(PASS_COMMAND_ARGS, modName);
	return true;
}

bool Cmd_GetFormMods_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	TempElements *tmpElements = GetTempElements();
	ListNode<ModInfo> *iter = form->mods.Head();
	do
	{
		if (iter->data) tmpElements->Append(iter->data->name);
	}
	while (iter = iter->next);
	*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetFormRefs_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 scanGrid = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &scanGrid)) return true;
	TempElements *tmpElements = GetTempElements();
	ListNode<TESObjectREFR> *refrIter;
	TESObjectREFR *refr;
	for (auto intrIter = g_dataHandler->cellArray.Begin(); intrIter; ++intrIter)
	{
		if (!*intrIter) continue;
		refrIter = intrIter->objectList.Head();
		do
		{
			refr = refrIter->data;
			if (refr && (refr->baseForm == form))
				tmpElements->Append(refr);
		}
		while (refrIter = refrIter->next);
	}
	if (scanGrid)
	{
		for (auto gridIter = g_TES->gridCellArray->Begin(); gridIter; ++gridIter)
		{
			if (!*gridIter) continue;
			refrIter = gridIter->objectList.Head();
			do
			{
				refr = refrIter->data;
				if (refr && (refr->baseForm == form) && !(refr->flags & TESObjectREFR::kFlags_Persistent))
					tmpElements->Append(refr);
			}
			while (refrIter = refrIter->next);
		}
	}
	ListNode<TESWorldSpace> *wspcIter = g_dataHandler->worldSpaceList.Head();
	TESWorldSpace *wspc;
	do
	{
		if (!(wspc = wspcIter->data) || !wspc->cell) continue;
		refrIter = wspc->cell->objectList.Head();
		do
		{
			refr = refrIter->data;
			if (refr && (refr->baseForm == form))
				tmpElements->Append(refr);
		}
		while (refrIter = refrIter->next);
	}
	while (wspcIter = wspcIter->next);
	*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetSelfModIndex_Execute(COMMAND_ARGS)
{
	*result = scriptObj ? scriptObj->modIndex : 0xFF;
	return true;
}

bool Cmd_IsFormOverridden_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (scriptObj && ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		UInt8 overriding = form->GetOverridingModIdx();
		*result = ((overriding > scriptObj->modIndex) && (overriding != 0xFF)) ? 1 : 0;
	}
	else *result = 0;
	return true;
}

bool Cmd_GetFormFromMod_Execute(COMMAND_ARGS)
{
	REFR_RES = 0;
	char modName[0x80], refIDStr[0x10];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &modName, &refIDStr)) return true;
	UInt8 modIndex;
	if (StrCompareCI(modName, "NONE"))
	{
		modIndex = g_dataHandler->GetModIndex(modName);
		if (modIndex == 0xFF) return true;
	}
	else modIndex = 0xFF;
	UInt32 refID = (HexToUInt(refIDStr) & 0xFFFFFF) | (modIndex << 24);
	if (LookupFormByRefID(refID)) REFR_RES = refID;
	return true;
}

bool Cmd_GetStringSetting_Execute(COMMAND_ARGS)
{
	char settingName[0x80];
	const char *resStr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &settingName) && ((settingName[0] | 0x20) == 's'))
	{
		Setting *setting = s_gameSettingsMap->Get(settingName);
		if (setting)
			resStr = setting->data.str;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetStringSetting_Execute(COMMAND_ARGS)
{
	char settingName[0x80];
	char *buffer = GetStrArgBuffer();
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &settingName, buffer) && ((settingName[0] | 0x20) == 's'))
	{
		Setting *setting = s_gameSettingsMap->Get(settingName);
		if (setting)
			setting->Set(buffer, true);
	}
	return true;
}

bool Cmd_GetGameSettings_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 gmstType = 3;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &gmstType))
	{
		NVSEArrayVar *outArray = CreateStringMap(NULL, NULL, 0, scriptObj);
		char namePrfx;
		for (auto gmstIter = s_gameSettingsMap->Begin(); gmstIter; ++gmstIter)
		{
			namePrfx = *gmstIter->name | 0x20;
			if (namePrfx == 's')
			{
				if (gmstType & 2)
					SetElement(outArray, ArrayElementL(gmstIter->name), ArrayElementL(gmstIter->data.str));
			}
			else if (gmstType & 1)
			{
				if (namePrfx == 'f')
					SetElement(outArray, ArrayElementL(gmstIter->name), ArrayElementL(gmstIter->data.f));
				else
					SetElement(outArray, ArrayElementL(gmstIter->name), ArrayElementL(gmstIter->data.i));
			}
		}
		*result = (int)outArray;
	}
	return true;
}