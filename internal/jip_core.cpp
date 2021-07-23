#include "internal/jip_core.h"

ExpressionEvaluatorUtils s_expEvalUtils;

_WriteRecord WriteRecord;
_WriteRecordData WriteRecordData;
_GetNextRecordInfo GetNextRecordInfo;
_ReadRecordData ReadRecordData;
_ResolveRefID ResolveRefID;
_GetSavePath GetSavePath;
_WriteRecord8 WriteRecord8;
_WriteRecord16 WriteRecord16;
_WriteRecord32 WriteRecord32;
_WriteRecord64 WriteRecord64;
_ReadRecord8 ReadRecord8;
_ReadRecord16 ReadRecord16;
_ReadRecord32 ReadRecord32;
_ReadRecord64 ReadRecord64;
_GetCmdByOpcode GetCmdByOpcode;
_GetStringVar GetStringVar;
_AssignString AssignString;
_CreateArray CreateArray;
_CreateStringMap CreateStringMap;
_AssignCommandResult AssignCommandResult;
_SetElement SetElement;
_AppendElement AppendElement;
_GetArraySize GetArraySize;
_LookupArrayByID LookupArrayByID;
_GetElement GetElement;
_GetElements GetElements;
_ExtractArgsEx ExtractArgsEx;
_ExtractFormatStringArgs ExtractFormatStringArgs;
_CallFunction CallFunction;
_CaptureLambdaVars CaptureLambdaVars;
_UncaptureLambdaVars UncaptureLambdaVars;
_InventoryRefCreate InventoryRefCreate;
_InventoryRefGetForID InventoryRefGetForID;

DIHookControl *g_DIHookCtrl;
UInt8 *g_numPreloadMods;

SpellItem *g_pipBoyLight;
ModelLoader *g_modelLoader;
DataHandler *g_dataHandler;
LoadedReferenceCollection *g_loadedReferences;
InterfaceManager *g_interfaceManager;
OSGlobals *g_OSGlobals;
TES *g_TES;
PlayerCharacter *g_thePlayer;
SceneGraph *g_sceneGraph;
void *g_scrapHeapQueue;
FontManager *g_fontManager;
OSInputGlobals *g_inputGlobals;
TileMenu **g_tileMenuArray;
HUDMainMenu *g_HUDMainMenu;
ConsoleManager *g_consoleManager;
SystemColorManager *g_sysColorManager;
NiNode *g_cursorNode;
ShadowSceneNode *g_shadowSceneNode;
TESObjectREFR *s_tempPosMarker;
float g_screenResConvert, g_screenWidth, g_screenHeight;
const char *g_terminalModelDefault;
TESObjectWEAP *g_fistsWeapon;
TESObjectACTI *g_ashPileACTI, *g_gooPileACTI;
TESGlobal *g_gameYear, *g_gameMonth, *g_gameDay, *g_gameHour;
TESObjectMISC *g_capsItem;
TESImageSpaceModifier *g_getHitIMOD, *g_explosionInFaceIMOD;
double *g_condDmgPenalty;
double s_condDmgPenalty = 0.67;
UInt32 s_mainThreadID;
UInt32 s_initialTickCount;

Cmd_Execute SayTo, KillActor, AddNote, AttachAshPile, MoveToFade, GetRefs;

const bool kInventoryType[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
};

alignas(16) const UInt8 kMaterialConvert[] =
{
	kMaterial_Stone,
	kMaterial_Cloth,
	kMaterial_Dirt,
	kMaterial_Glass,
	kMaterial_Grass,
	kMaterial_Metal,
	kMaterial_Organic,
	kMaterial_Organic,
	kMaterial_Water,
	kMaterial_Wood,
	kMaterial_Stone,
	kMaterial_Metal,
	kMaterial_Wood,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_HollowMetal,
	kMaterial_HollowMetal,
	kMaterial_Dirt,
	kMaterial_Stone,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_HollowMetal,
	kMaterial_HollowMetal,
	kMaterial_Glass,
	kMaterial_HollowMetal,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_HollowMetal,
	kMaterial_HollowMetal,
	kMaterial_Wood,
	kMaterial_Wood
};

const char kDaysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

const char kMenuIDJumpTable[] =
{
	0, 1, 2, 3, -1, -1, 4, 5, 6, -1, -1, 7, 8, 9, -1, 10, -1, -1, -1, -1, -1, -1, 11, -1, -1, 12, 13, -1, -1, -1, 
	-1, -1, -1, -1, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, 17, -1, 18, 19, 20, 21, 22, 
	23, 24, 25, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, 28, 29, 30, -1, -1, 31, 32, 33, 34, 35
};

UInt32 s_serializedFlags = 0;

__declspec(noinline) TempFormList *GetTempFormList()
{
	thread_local TempFormList s_tempFormList(0x40);
	return &s_tempFormList;
}

__declspec(noinline) TempElements *GetTempElements()
{
	thread_local TempElements s_tempElements(0x100);
	return &s_tempElements;
}

UnorderedMap<const char*, UInt32> s_strRefs;

UInt32 __fastcall StringToRef(char *refStr)
{
	UInt32 *findStr;
	if (!s_strRefs.Insert(refStr, &findStr)) return *findStr;
	*findStr = 0;
	char *colon = FindChr(refStr, ':');
	if (colon)
	{
		UInt8 modIdx;
		if (colon != refStr)
		{
			*colon = 0;
			modIdx = g_dataHandler->GetModIndex(refStr);
			*colon = ':';
			if (modIdx == 0xFF) return 0;
		}
		else modIdx = 0xFF;
		*findStr = (modIdx << 24) | HexToUInt(colon + 1);
		return *findStr;
	}
	return ResolveRefID(HexToUInt(refStr), findStr) ? *findStr : 0;
}

__declspec(noinline) InventoryItemsMap *GetInventoryItemsMap()
{
	thread_local InventoryItemsMap s_inventoryItemsMap(0x40);
	return &s_inventoryItemsMap;
}

bool GetInventoryItems(TESObjectREFR *refr, UInt8 typeID, InventoryItemsMap *invItemsMap)
{
	TESContainer *container = refr->GetContainer();
	if (!container) return false;
	ExtraContainerChanges::EntryDataList *entryList = refr->GetContainerChangesList();
	if (!entryList) return false;
	invItemsMap->Clear();

	TESContainer::FormCount *formCount;
	TESForm *item;
	SInt32 contCount, countDelta;
	ContChangesEntry *entry;

	ListNode<TESContainer::FormCount> *contIter = container->formCountList.Head();
	do
	{
		if (!(formCount = contIter->data)) continue;
		item = formCount->form;
		if ((typeID && (item->typeID != typeID)) || (item->typeID == kFormType_TESLevItem) || invItemsMap->HasKey(item))
			continue;
		contCount = container->GetCountForForm(item);
		if (entry = entryList->FindForItem(item))
		{
			countDelta = entry->countDelta;
			if (entry->HasExtraLeveledItem())
				contCount = countDelta;
			else contCount += countDelta;
		}
		if (contCount > 0)
			invItemsMap->Emplace(item, contCount, entry);
	}
	while (contIter = contIter->next);

	ListNode<ContChangesEntry> *xtraIter = entryList->Head();
	do
	{
		if (!(entry = xtraIter->data)) continue;
		item = entry->type;
		if ((typeID && (item->typeID != typeID)) || invItemsMap->HasKey(item))
			continue;
		countDelta = entry->countDelta;
		if (countDelta > 0)
			invItemsMap->Emplace(item, countDelta, entry);
	}
	while (xtraIter = xtraIter->next);

	return !invItemsMap->Empty();
}

__declspec(naked) NiAVObject* __fastcall _GetRayCastObject(void *rcData, void *args)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0xC
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		mov		eax, [edi+0xC]
		add		eax, [edi+0x14]
		mov		ecx, [edi+8]
		lea		edx, [ebp-0xC]
		movss	xmm0, [edi+0x10]
		movss	xmm1, [eax+0x18]
		mulss	xmm1, xmm0
		addss	xmm1, [ecx+8]
		movss	[edx+8], xmm1
		movss	xmm1, [eax+0xC]
		mulss	xmm1, xmm0
		addss	xmm1, [ecx+4]
		movss	[edx+4], xmm1
		mulss	xmm0, [eax]
		addss	xmm0, [ecx]
		movss	[edx], xmm0
		mov		ecx, esi
		CALL_EAX(0x4A3C20)
		lea		eax, [esi+0x24]
		push	eax
		mov		ecx, g_thePlayer
		CALL_EAX(0x931ED0)
		mov		edx, [eax]
		mov		dx, [edi+0x18]
		mov		[eax], edx
		push	dword ptr [edi+8]
		mov		ecx, esi
		CALL_EAX(0x4A3DA0)
		lea		eax, [ebp-0xC]
		push	eax
		mov		ecx, esi
		CALL_EAX(0x4A3EB0)
		push	esi
		mov		ecx, g_TES
		CALL_EAX(0x458420)
		pop		edi
		pop		esi
		leave
		retn
	}
}

__declspec(naked) NiAVObject* __stdcall GetRayCastObject(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xB0
		mov		edx, ebp
		mov		ecx, esp
		call	_GetRayCastObject
		leave
		retn	0x14
	}
}

__declspec(naked) bool NiVector3::RayCastCoords(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xC0
		mov		[esp], ecx
		mov		edx, ebp
		lea		ecx, [esp+0x10]
		call	_GetRayCastObject
		push	dword ptr [esp]
		lea		ecx, [esp+0x14]
		CALL_EAX(0x5DBE60)
		push	dword ptr [esp]
		mov		ecx, g_TES
		CALL_EAX(0x451110)
		leave
		retn	0x14
	}
}

__declspec(naked) int __stdcall GetRayCastMaterial(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xC0
		mov		edx, ebp
		lea		ecx, [esp+0x10]
		call	_GetRayCastObject
		test	eax, eax
		jz		invalid
		mov		[esp+0xC], eax
		push	eax
		CALL_EAX(0x56F930)
		pop		ecx
		test	eax, eax
		jz		isTerrain
		mov		ecx, eax
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], kAddr_ReturnTrue
		jnz		notActor
		mov		eax, [ecx]
		call	dword ptr [eax+0x218]
		mov		ecx, [ecx+0x20]
		test	al, al
		jz		notNPC
		mov		eax, [ecx+0x1E4]
		jmp		done
	notNPC:
		mov		eax, [ecx+0x148]
		jmp		done
	notActor:
		mov		ecx, [esp+0xC]
		mov		ecx, [ecx+0x1C]
		test	ecx, ecx
		jz		invalid
		mov		ecx, [ecx+0x10]
		test	ecx, ecx
		jz		invalid
		mov		ecx, [ecx+8]
		mov		ecx, [ecx+0x10]
		test	ecx, ecx
		jz		invalid
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		invalid
		push	0
		CALL_EAX(0xC84F10)
		jmp		convert
	isTerrain:
		push	esp
		lea		ecx, [esp+0x14]
		CALL_EAX(0x5DBE60)
		push	esp
		mov		ecx, g_TES
		CALL_EAX(0x451110)
		push	esp
		mov		ecx, g_TES
		CALL_EAX(0x457720)
		test	eax, eax
		jz		invalid
		movzx	eax, byte ptr [eax+0x1C]
	convert:
		cmp		eax, 0x1F
		ja		invalid
		mov		edx, eax
		movzx	eax, kMaterialConvert[edx]
		jmp		done
	invalid:
		mov		eax, 0xFFFFFFFF
	done:
		leave
		retn	0x14
	}
}

UnorderedMap<TESNPC*, AppearanceUndo*> s_appearanceUndoMap;

UnorderedSet<TESGlobal*> s_resolvedGlobals;

__declspec(naked) UInt32 TESGlobal::ResolveRefValue()
{
	__asm
	{
		mov		word ptr [ecx+6], 1
		push	esi
		lea		esi, [ecx+0x24]
		cmp		byte ptr [esi], 0xFF
		jz		doLookup
		push	esi
		push	dword ptr [esi]
		call	ResolveRefID
		add		esp, 8
		movzx	eax, al
		test	eax, eax
		jz		invalid
	doLookup:
		push	dword ptr [esi]
		call	LookupFormByRefID
		test	eax, eax
		jz		invalid
		mov		eax, [esi]
		pop		esi
		retn
	invalid:
		mov		[esi], eax
		pop		esi
		retn
	}
}

hkpWorld *GethkpWorld()
{
	if (!g_thePlayer) return NULL;
	bhkWorld *hkWorld = NULL;
	TESObjectCELL *cell = g_thePlayer->parentCell;
	if (cell)
	{
		ExtraHavok *xHavok = GetExtraType(&cell->extraDataList, Havok);
		if (xHavok) hkWorld = xHavok->world;
	}
	if (!hkWorld) hkWorld = *g_bhkWorldM;
	return hkWorld ? hkWorld->GetWorld() : NULL;
}

void *TESRecipe::ComponentList::GetComponents(Script *scriptObj)
{
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	Node *iter = Head();
	RecipeComponent *component;
	do
	{
		if (component = iter->data)
			tmpElements->Append(component->item);
	}
	while (iter = iter->next);
	NVSEArrayVar *outArray = CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return outArray;
}

bool LeveledListHasFormDeep(TESLeveledList *pLvlList, TESForm *form, TempFormList *tmpFormLst)
{
	ListNode<TESLeveledList::ListData> *iter = pLvlList->list.Head();
	TESLeveledList::ListData *data;
	TESLeveledList *lvlList;
	do
	{
		if (!(data = iter->data)) continue;
		if (data->form == form)
			return true;
		lvlList = data->form->GetLvlList();
		if (lvlList && tmpFormLst->Insert(data->form) && LeveledListHasFormDeep(lvlList, form, tmpFormLst))
			return true;
	}
	while (iter = iter->next);
	return false;
}

float GetDaysPassed(int bgnYear, int bgnMonth, int bgnDay)
{
	int totalDays = 0, iter;
	int iYear = g_gameYear->data, iMonth = g_gameMonth->data, iDay = g_gameDay->data;
	iYear -= bgnYear;
	if (iYear > 0)
	{
		totalDays = kDaysPerMonth[bgnMonth] - bgnDay;
		for (iter = bgnMonth + 1; iter < 12; iter++)
			totalDays += kDaysPerMonth[iter];
		iYear--;
		if (iYear) totalDays += (365 * iYear);
		for (iter = 0; iter < iMonth; iter++)
			totalDays += kDaysPerMonth[iter];
		totalDays += iDay - 1;
	}
	else if (bgnMonth < iMonth)
	{
		totalDays = kDaysPerMonth[bgnMonth] - bgnDay;
		for (iter = bgnMonth + 1; iter < iMonth; iter++)
			totalDays += kDaysPerMonth[iter];
		totalDays += iDay - 1;
	}
	else totalDays = iDay - bgnDay;
	return (float)totalDays + (g_gameHour->data / 24.0F);
}

void PlayGameSound(const char *soundEDID)
{
	Sound sound(soundEDID, 0x121);
	sound.Play();
}

UnorderedMap<UInt32, ScriptVariablesMap> s_scriptVariablesBuffer;
UnorderedMap<UInt32, VariableNames> s_addedVariables;

bool __fastcall GetVariableAdded(UInt32 ownerID, char *varName)
{
	VariableNames *findOwner = s_addedVariables.GetPtr(ownerID);
	if (!findOwner) return false;
	return findOwner->HasKey(varName);
}

ScriptVar *Script::AddVariable(char *varName, ScriptEventList *eventList, UInt32 ownerID, UInt8 modIdx)
{
	VariableInfo *varInfo = GetVariableByName(varName);
	if (!varInfo)
	{
		varInfo = (VariableInfo*)GameHeapAlloc(sizeof(VariableInfo));
		MemZero(varInfo, sizeof(VariableInfo));
		varInfo->idx = ++info.varCount;
		varInfo->name.Set(varName);
		varList.Append(varInfo);
		s_addedVariables[refID].Insert(varName);
	}
	else if (!GetVariableAdded(refID, varName)) return NULL;

	ScriptVar *var = eventList->GetVariable(varInfo->idx);
	if (!var)
	{
		var = (ScriptVar*)GameHeapAlloc(sizeof(ScriptVar));
		var->id = varInfo->idx;
		var->next = NULL;
		var->data.num = 0;
		eventList->m_vars->Append(var);
	}

	if (varName[0] != '*')
		s_scriptVariablesBuffer[ownerID][varName].Set(var, modIdx);
	return var;
}

UnorderedMap<UInt32, LinkedRefEntry> s_linkedRefModified;
UnorderedMap<UInt32, UInt32> s_linkedRefDefault, s_linkedRefsTemp;

bool TESObjectREFR::SetLinkedRef(TESObjectREFR *linkObj = NULL, UInt8 modIdx)
{
	ExtraLinkedRef *xLinkedRef = GetExtraType(&extraDataList, LinkedRef);
	if (!linkObj)
	{
		auto findDefID = s_linkedRefDefault.Find(refID);
		if (findDefID)
		{
			if (xLinkedRef)
			{
				if (*findDefID)
				{
					TESForm *form = LookupFormByRefID(*findDefID);
					if (form && IS_REFERENCE(form)) xLinkedRef->linkedRef = (TESObjectREFR*)form;
				}
				else extraDataList.RemoveExtra(xLinkedRef, true);
			}
			findDefID.Remove();
		}
		s_linkedRefModified.Erase(refID);
		return true;
	}
	if (!xLinkedRef)
	{
		extraDataList.AddExtra(ExtraLinkedRef::Create(linkObj));
		s_linkedRefDefault[refID] = 0;
	}
	else
	{
		if (!xLinkedRef->linkedRef) return false;
		s_linkedRefDefault[refID] = xLinkedRef->linkedRef->refID;
		xLinkedRef->linkedRef = linkObj;
	}
	s_linkedRefModified[refID].Set(linkObj->refID, modIdx);
	return true;
}

bool SetLinkedRefID(UInt32 thisID, UInt32 linkID, UInt8 modIdx)
{
	TESObjectREFR *thisObj = (TESObjectREFR*)LookupFormByRefID(thisID);
	if (!thisObj || NOT_REFERENCE(thisObj)) return false;
	if (!linkID) return thisObj->SetLinkedRef();
	TESObjectREFR *linkObj = (TESObjectREFR*)LookupFormByRefID(linkID);
	return linkObj && IS_REFERENCE(linkObj) && thisObj->SetLinkedRef(linkObj, modIdx);
}

UInt32 s_serializedVersion = 9;

AuxVarModsMap s_auxVariablesPerm, s_auxVariablesTemp;

RefMapModsMap s_refMapArraysPerm, s_refMapArraysTemp;

UInt32 __fastcall GetSubjectID(TESForm *form, TESObjectREFR *thisObj)
{
	if (form) return IS_REFERENCE(form) ? ((TESObjectREFR*)form)->baseForm->refID : form->refID;
	if (thisObj) return thisObj->refID;
	return 0;
}

UInt8 s_dataChangedFlags = 0;

void EventCallbackScripts::InvokeEvents(UInt32 arg)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, NULL, 1, arg);
}

void EventCallbackScripts::InvokeEvents2(UInt32 arg1, UInt32 arg2)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, NULL, 2, arg1, arg2);
}

void EventCallbackScripts::InvokeEventsThis(TESObjectREFR *thisObj)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, thisObj, 0);
}

void EventCallbackScripts::InvokeEventsThis1(TESObjectREFR *thisObj, UInt32 arg)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, thisObj, 1, arg);
}

void EventCallbackScripts::InvokeEventsThis2(TESObjectREFR *thisObj, UInt32 arg1, UInt32 arg2)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, thisObj, 2, arg1, arg2);
}

CriticalHitEventCallbacks s_criticalHitEvents;

UnorderedMap<Script*, DisabledScriptBlocks> s_disabledScriptBlocksMap;

ArrayElementR* __fastcall GetArrayData(NVSEArrayVar *srcArr, UInt32 *size)
{
	*size = GetArraySize(srcArr);
	if (!*size) return NULL;
	ArrayElementR *data = (ArrayElementR*)AuxBuffer::Get(2, *size * sizeof(ArrayElementR));
	MemZero(data, *size * sizeof(ArrayElementR));
	GetElements(srcArr, data, NULL);
	return data;
}

ExtraDataList *InventoryRef::CreateExtraData()
{
	ContChangesEntry *pEntry = containerRef->GetContainerChangesEntry(type);
	if (!pEntry) return NULL;
	xData = ExtraDataList::Create();
	if (pEntry->extendData)
		pEntry->extendData->Prepend(xData);
	else
	{
		pEntry->extendData = (ExtraContainerChanges::ExtendDataList*)GameHeapAlloc(8);
		pEntry->extendData->Init(xData);
	}
	containerRef->MarkAsModified(0x20);
	return xData;
}

TESObjectREFR* __fastcall CreateRefForStack(TESObjectREFR *container, ContChangesEntry *menuEntry)
{
	return (container && menuEntry) ? InventoryRefCreate(container, menuEntry->type, menuEntry->countDelta, menuEntry->extendData ? menuEntry->extendData->GetFirstItem() : NULL) : NULL;
}

ExtraDataList* __fastcall SplitFromStack(ContChangesEntry *entry, ExtraDataList *xDataIn)
{
	ExtraCount *xCount = GetExtraType(xDataIn, Count);
	if (!xCount) return xDataIn;
	ExtraDataList *xDataOut = xDataIn->CreateCopy();
	xDataOut->RemoveByType(kExtraData_Count);
	xDataOut->RemoveByType(kExtraData_Hotkey);
	if (--xCount->count < 2)
	{
		xDataIn->RemoveExtra(xCount, true);
		if (!xDataIn->m_data)
		{
			entry->extendData->Remove(xDataIn);
			GameHeapFree(xDataIn);
		}
	}
	entry->extendData->Prepend(xDataOut);
	return xDataOut;
}

TESObjectREFR* __fastcall GetEquippedItemRef(Actor *actor, UInt32 slotIndex)
{
	ExtraContainerChanges::EntryDataList *entryList = actor->GetContainerChangesList();
	if (!entryList) return NULL;
	UInt32 partMask = 1 << slotIndex;
	TESForm *item;
	ContChangesEntry *entry;
	ExtraDataList *xData;
	if (actor->GetNiNode())
	{
		if (slotIndex == 5)
		{
			entry = actor->GetWeaponInfo();
			if (entry && entry->extendData)
				return InventoryRefCreate(actor, entry->type, entry->countDelta, entry->extendData->GetFirstItem());
		}
		else
		{
			ValidBip01Names *equipment = actor->GetValidBip01Names();
			if (equipment)
			{
				ValidBip01Names::Data *slotData = equipment->slotData;
				for (UInt8 count = 20; count; count--, slotData++)
				{
					item = slotData->item;
					if (!item || NOT_TYPE(item, TESObjectARMO) || !(((TESObjectARMO*)item)->bipedModel.partMask & partMask))
						continue;
					if (!(entry = entryList->FindForItem(item)) || !(xData = entry->GetEquippedExtra()))
						break;
					return InventoryRefCreate(actor, item, entry->countDelta, xData);
				}
			}
		}
	}
	else
	{
		ListNode<ContChangesEntry> *listIter = entryList->Head();
		do
		{
			entry = listIter->data;
			if (!entry || !entry->extendData)
				continue;
			item = entry->type;
			if (slotIndex == 5)
			{
				if NOT_ID(item, TESObjectWEAP)
					continue;
			}
			else if (NOT_TYPE(item, TESObjectARMO) || !(((TESObjectARMO*)item)->bipedModel.partMask & partMask))
				continue;
			if (xData = entry->GetEquippedExtra())
				return InventoryRefCreate(actor, item, entry->countDelta, xData);
		}
		while (listIter = listIter->next);
	}
	return NULL;
}

ContChangesEntry* __fastcall GetHotkeyItemEntry(UInt8 index, ExtraDataList **outXData)
{
	ExtraContainerChanges::EntryDataList *entryList = g_thePlayer->GetContainerChangesList();
	if (!entryList) return NULL;
	ListNode<ContChangesEntry> *entryIter = entryList->Head();
	ContChangesEntry *entry;
	UInt8 type;
	ListNode<ExtraDataList> *xdlIter;
	ExtraDataList *xData;
	ExtraHotkey *xHotkey;
	do
	{
		if (!(entry = entryIter->data) || !entry->extendData)
			continue;
		type = entry->type->typeID;
		if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem))
			continue;
		xdlIter = entry->extendData->Head();
		do
		{
			if (!(xData = xdlIter->data)) continue;
			xHotkey = GetExtraType(xData, Hotkey);
			if (!xHotkey || (xHotkey->index != index))
				continue;
			*outXData = xData;
			return entry;
		}
		while (xdlIter = xdlIter->next);
	}
	while (entryIter = entryIter->next);
	return NULL;
}

bool __fastcall ClearHotkey(UInt8 index)
{
	ExtraDataList *xData;
	ContChangesEntry *entry = GetHotkeyItemEntry(index, &xData);
	if (entry)
	{
		xData->RemoveByType(kExtraData_Hotkey);
		if (!xData->m_data)
		{
			entry->extendData->Remove(xData);
			GameHeapFree(xData);
		}
		return true;
	}
	return false;
}

float __fastcall GetModBonuses(TESObjectREFR *wpnRef, UInt32 effectID)
{
	TESObjectWEAP *weapon = (TESObjectWEAP*)wpnRef->baseForm;
	if NOT_ID(weapon, TESObjectWEAP) return 0;
	ExtraWeaponModFlags *xModFlags = GetExtraType(&wpnRef->extraDataList, WeaponModFlags);
	if (!xModFlags) return 0;
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if ((xModFlags->flags & (1 << idx)) && (weapon->effectMods[idx] == effectID))
			result += weapon->value1Mod[idx];
	return result;
}

const AnimGroupClassify kAnimGroupClassify[] =
{
	{1, 1, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}, {2, 1, 0, 0}, {2, 2, 0, 0}, {2, 3, 0, 0}, {2, 4, 0, 0}, {2, 1, 1, 0}, {2, 2, 1, 0}, 
	{2, 3, 1, 0}, {2, 4, 1, 0}, {2, 1, 2, 0}, {2, 2, 2, 0}, {2, 3, 2, 0}, {2, 4, 2, 0}, {2, 3, 4, 0}, {2, 4, 4, 0}, {3, 0, 0, 0}, 
	{3, 0, 1, 0}, {3, 0, 2, 0}, {3, 0, 4, 0}, {3, 0, 5, 0}, {3, 0, 6, 0}, {1, 2, 0, 0}, {1, 2, 0, 0}, {1, 2, 0, 0}, {3, 1, 0, 0}, 
	{3, 1, 1, 0}, {3, 1, 2, 0}, {3, 1, 4, 0}, {3, 1, 5, 0}, {3, 1, 6, 0}, {3, 2, 0, 0}, {3, 2, 1, 0}, {3, 2, 2, 0}, {3, 2, 4, 0}, 
	{3, 2, 5, 0}, {3, 2, 6, 0}, {3, 3, 0, 0}, {3, 3, 1, 0}, {3, 3, 2, 0}, {3, 3, 4, 0}, {3, 3, 5, 0}, {3, 3, 6, 0}, {3, 4, 0, 0}, 
	{3, 4, 1, 0}, {3, 4, 2, 0}, {3, 4, 4, 0}, {3, 4, 5, 0}, {3, 4, 6, 0}, {3, 5, 0, 0}, {3, 5, 1, 0}, {3, 5, 2, 0}, {3, 5, 4, 0}, 
	{3, 5, 5, 0}, {3, 5, 6, 0}, {3, 6, 0, 0}, {3, 6, 1, 0}, {3, 6, 2, 0}, {3, 6, 4, 0}, {3, 6, 5, 0}, {3, 6, 6, 0}, {3, 7, 0, 0}, 
	{3, 7, 1, 0}, {3, 7, 2, 0}, {3, 7, 4, 0}, {3, 7, 5, 0}, {3, 7, 6, 0}, {3, 8, 0, 0}, {3, 8, 1, 0}, {3, 8, 2, 0}, {3, 8, 4, 0}, 
	{3, 8, 5, 0}, {3, 8, 6, 0}, {3, 10, 0, 0}, {3, 10, 1, 0}, {3, 10, 2, 0}, {3, 10, 4, 0}, {3, 10, 5, 0}, {3, 10, 6, 0}, {3, 11, 0, 0}, 
	{3, 11, 1, 0}, {3, 11, 2, 0}, {3, 11, 4, 0}, {3, 11, 5, 0}, {3, 11, 6, 0}, {3, 12, 0, 0}, {3, 12, 1, 0}, {3, 12, 2, 0}, {3, 12, 4, 0}, 
	{3, 12, 5, 0}, {3, 12, 6, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, 
	{3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 21, 0, 0}, {3, 21, 1, 0}, {3, 21, 2, 0}, {3, 21, 4, 0}, {3, 21, 5, 0}, {3, 21, 6, 0}, 
	{3, 22, 0, 0}, {3, 22, 1, 0}, {3, 22, 2, 0}, {3, 22, 4, 0}, {3, 22, 5, 0}, {3, 22, 6, 0}, {3, 13, 0, 0}, {3, 13, 1, 0}, {3, 13, 2, 0}, 
	{3, 13, 4, 0}, {3, 13, 5, 0}, {3, 13, 6, 0}, {3, 14, 0, 0}, {3, 14, 1, 0}, {3, 14, 2, 0}, {3, 14, 4, 0}, {3, 14, 5, 0}, {3, 14, 6, 0}, 
	{3, 15, 0, 0}, {3, 15, 1, 0}, {3, 15, 2, 0}, {3, 15, 4, 0}, {3, 15, 5, 0}, {3, 15, 6, 0}, {3, 16, 0, 0}, {3, 16, 1, 0}, {3, 16, 2, 0}, 
	{3, 16, 4, 0}, {3, 16, 5, 0}, {3, 16, 6, 0}, {3, 17, 0, 0}, {3, 17, 1, 0}, {3, 17, 2, 0}, {3, 17, 4, 0}, {3, 17, 5, 0}, {3, 17, 6, 0}, 
	{3, 9, 0, 0}, {3, 9, 1, 0}, {3, 9, 2, 0}, {3, 9, 4, 0}, {3, 9, 5, 0}, {3, 9, 6, 0}, {3, 18, 0, 0}, {3, 18, 1, 0}, {3, 18, 2, 0}, 
	{3, 18, 4, 0}, {3, 18, 5, 0}, {3, 18, 6, 0}, {3, 19, 0, 0}, {3, 19, 1, 0}, {3, 19, 2, 0}, {3, 19, 4, 0}, {3, 19, 5, 0}, {3, 19, 6, 0}, 
	{3, 20, 0, 0}, {3, 20, 1, 0}, {3, 20, 2, 0}, {3, 20, 4, 0}, {3, 20, 5, 0}, {3, 20, 6, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 3, 0, 0}, 
	{1, 3, 0, 0}, {1, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, 
	{4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, 
	{4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {5, 0, 0, 0}, 
	{5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, 
	{5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, 
	{5, 0, 0, 0}, {5, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {1, 4, 0, 0}, 
	{1, 4, 0, 0}, {1, 4, 0, 0}, {1, 4, 0, 0}, {1, 4, 0, 0}, {1, 4, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {1, 0, 0, 0}, 
	{2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}
};

JIPScriptRunner s_jipScriptRunner;

void JIPScriptRunner::Init()
{
	s_jipScriptRunner.script = (Script*)malloc(sizeof(Script));
	MemZero(s_jipScriptRunner.script, sizeof(Script));
	s_jipScriptRunner.scrContext = g_consoleManager->scriptContext;
	char *fileName;
	for (DirectoryIterator iter("Data\\NVSE\\plugins\\scripts\\*.txt"); iter; ++iter)
	{
		if (!iter.IsFile()) continue;
		fileName = (char*)iter.Get();
		if (fileName[2] != '_') continue;
		switch (*(UInt16*)fileName |= 0x2020)
		{
			case 'rg':
				RunFile(fileName);
				break;
			case 'lg':
			case 'sg':
			case 'mx':
			case 'xg':
				s_jipScriptRunner.sourceFiles.Insert(fileName);
				break;
			default:
				break;
		}
	}
}

__declspec(naked) bool __stdcall JIPScriptRunner::RunScript(const char *scriptText, TESObjectREFR *thisObj)
{
	__asm
	{
		push	ebx
		push	esi
		mov		ebx, offset s_jipScriptRunner
		mov		esi, [ebx]
		mov		ecx, esi
		CALL_EAX(0x5AA0F0)
		mov		ecx, esi
		CALL_EAX(0x484490)
		push	dword ptr [esp+0xC]
		mov		ecx, esi
		CALL_EAX(0x5ABE50)
		push	dword ptr [esp+0x10]
		push	1
		push	dword ptr [ebx+4]
		mov		ecx, esi
		CALL_EAX(0x5AC400)
		mov		ecx, esi
		cmp		dword ptr [ecx+0x20], 0
		setnz	bl
		CALL_EAX(0x5AA1A0)
		mov		al, bl
		pop		esi
		pop		ebx
		retn	8
	}
}

void __stdcall JIPScriptRunner::RunFile(const char *fileName)
{
	char scriptsPath[0x80], *buffer = GetStrArgBuffer();
	memcpy(scriptsPath, "Data\\NVSE\\plugins\\scripts\\", 26);
	StrCopy(scriptsPath + 26, fileName);
	bool success = FileToBuffer(scriptsPath, buffer) && RunScript(buffer);
	PrintLog("Run Script : %s >> %s", fileName, success ? "SUCCESS" : "FAILED");
}

void __stdcall JIPScriptRunner::RunScriptFiles(UInt16 type)
{
	bool found = false;
	for (auto iter = s_jipScriptRunner.sourceFiles.Begin(); iter; ++iter)
	{
		if (*(UInt16*)*iter == type)
		{
			found = true;
			RunFile(*iter);
		}
		else if (found) break;
	}
}

bool TESObjectREFR::RunScriptSource(char *sourceStr, bool doFree)
{
	ExtraScript *xScript = GetExtraType(&extraDataList, Script);
	ScriptEventList *eventList = xScript ? xScript->eventList : NULL;
	if (eventList) xScript->eventList = NULL;
	bool success = JIPScriptRunner::RunScript(sourceStr, this);
	if (!success)
	{
		if (StrLen(sourceStr) > 0x80) const_cast<char*>(sourceStr)[0x80] = 0;
		Console_Print("ERROR: Failed to execute script source:\n%s (...)", sourceStr);
	}
	if (doFree) free(sourceStr);
	if (eventList) xScript->eventList = eventList;
	return success;
}

UnorderedMap<const char*, NiCamera*> s_extraCamerasMap;

bool s_HUDCursorMode = false;

bool GetMenuMode()
{
	return (g_interfaceManager->currentMode > 1) || s_HUDCursorMode;
}

__declspec(naked) bool IsConsoleOpen()
{
	__asm
	{
		mov		al, byte ptr ds:[0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, fs:[0x2C]
		mov		edx, ds:[0x126FD98]
		mov		eax, [eax+edx*4]
		test	eax, eax
		jz		done
		mov		al, [eax+0x268]
	done:
		retn
	}
}

__declspec(naked) void __fastcall DoConsolePrint(double *result)
{
	__asm
	{
		call	IsConsoleOpen
		test	al, al
		jz		done
		mov		edx, [ebp]
		mov		edx, [edx-0x30]
		test	edx, edx
		jz		done
		mov		edx, [edx]
		movq	xmm0, qword ptr [ecx]
		push	ebp
		mov		ebp, esp
		sub		esp, 0x50
		lea		ecx, [ebp-0x50]
		call	StrCopy
		mov		ecx, eax
		mov		dword ptr [ecx], ' >> '
		add		ecx, 4
		call	FltToStr
		lea		eax, [ebp-0x50]
		push	eax
		push	eax
		mov		ecx, g_consoleManager
		CALL_EAX(0x71D0A0)
		leave
	done:
		retn
	}
}

__declspec(naked) void __fastcall DoConsolePrint(TESForm *result)
{
	__asm
	{
		call	IsConsoleOpen
		test	al, al
		jz		done
		mov		edx, [ebp]
		mov		edx, [edx-0x30]
		test	edx, edx
		jz		done
		mov		edx, [edx]
		push	ebp
		mov		ebp, esp
		sub		esp, 0x60
		push	esi
		mov		esi, ecx
		lea		ecx, [ebp-0x60]
		call	StrCopy
		mov		ecx, eax
		mov		dword ptr [ecx], ' >> '
		add		ecx, 4
		test	esi, esi
		jnz		haveID
		mov		word ptr [ecx], '0'
		jmp		noEDID
	haveID:
		mov		edx, [esi+0xC]
		call	UIntToHex
		mov		ecx, esi
		mov		esi, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x130]
		test	eax, eax
		jz		noEDID
		cmp		[eax], 0
		jz		noEDID
		mov		ecx, esi
		mov		word ptr [ecx], '\" '
		add		ecx, 2
		mov		edx, eax
		call	StrCopy
		mov		word ptr [eax], '\"'
	noEDID:
		pop		esi
		lea		eax, [ebp-0x60]
		push	eax
		push	eax
		mov		ecx, g_consoleManager
		CALL_EAX(0x71D0A0)
		leave
	done:
		retn
	}
}

__declspec(naked) TLSData *GetTLSData()
{
	__asm
	{
		mov		eax, fs:[0x2C]
		mov		edx, ds:[0x126FD98]
		mov		eax, [eax+edx*4]
		retn
	}
}

UnorderedMap<const char*, UInt32> s_fileExtToType({{"nif", 1}, {"egm", 1}, {"egt", 1}, {"kf", 1}, {"psa", 1}, {"tri", 1}, {"dds", 2}, {"fnt", 2}, {"psd", 2},
	{"tai", 2}, {"tex", 2}, {"wav", 8}, {"lip", 0x10}, {"ogg", 0x10}, {"spt", 0x40}, {"ctl", 0x100}, {"dat", 0x100}, {"dlodsettings", 0x100}, {"xml", 0x100}});

__declspec(naked) bool __fastcall GetFileArchived(const char *filePath)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x1C
		push	esi
		push	edi
		mov		esi, ecx
		call	StrLen
		push	'.'
		mov		edx, eax
		call	FindChrR
		test	eax, eax
		jz		retnFalse
		inc		eax
		push	eax
		mov		ecx, offset s_fileExtToType
		call	UnorderedMap<const char*, UInt32>::Get
		test	eax, eax
		jz		retnFalse
		mov		[ebp-4], ax
		mov		edx, 0x10A4828
		mov		ecx, esi
		call	StrBeginsCI
		test	al, al
		jz		noSubDir
		add		esi, 5
	noSubDir:
		lea		eax, [ebp-0x14]
		push	eax
		lea		eax, [ebp-0xC]
		push	eax
		push	esi
		CALL_EAX(0xAFD270)
		add		esp, 0xC
		mov		edi, ds:[0x11F8160]
		ALIGN 16
	iterHead:
		test	edi, edi
		jz		retnFalse
		mov		ecx, [edi]
		mov		edi, [edi+4]
		test	ecx, ecx
		jz		iterHead
		mov		ax, word ptr [ecx+0x180]
		and		ax, word ptr [ebp-4]
		jz		iterHead
		push	esi
		lea		eax, [ebp-0x1C]
		push	eax
		lea		eax, [ebp-0x18]
		push	eax
		lea		eax, [ebp-0x14]
		push	eax
		lea		eax, [ebp-0xC]
		push	eax
		CALL_EAX(0xAF9BF0)
		test	al, al
		jz		iterHead
		jmp		done
	retnFalse:
		xor		al, al
	done:
		pop		edi
		pop		esi
		leave
		retn
	}
}

__declspec(naked) int __stdcall FileExistsEx(char *filePath, bool isFolder)
{
	__asm
	{
		push	'\\'
		mov		dl, '/'
		mov		ecx, [esp+8]
		call	ReplaceChr
		push	dword ptr [esp+4]
		call	GetFileAttributesA
		test	eax, eax
		js		notFound
		test	al, 0x10
		setnz	dl
		xor		eax, eax
		cmp		dl, [esp+8]
		setz	al
		retn	8
		ALIGN 16
	notFound:
		xor		eax, eax
		cmp		[esp+8], 0
		jnz		done
		mov		ecx, [esp+4]
		call	GetFileArchived
		shl		al, 1
		movzx	eax, al
	done:
		retn	8
	}
}

int GetIsLAA()
{
	static int isLAA = -1;
	if (isLAA == -1)
	{
		HMODULE gameHandle = GetModuleHandle(NULL);
		UInt8 *dataPtr = (UInt8*)gameHandle + *(UInt32*)((UInt8*)gameHandle + 0x3C) + 0x16;
		isLAA = (*dataPtr & 0x20) ? 1 : 0;
		if (isLAA)
		{
			void *blockPtrs[20], *block;
			MemZero(blockPtrs, 0x50);
			int idx = 0;
			do
			{
				blockPtrs[idx++] = block = malloc(0x6400000);
				if (!block) break;
				if ((UInt32)block >= 0x80000000)
				{
					isLAA = 2;
					break;
				}
			}
			while (idx < 20);
			do
			{
				free(blockPtrs[--idx]);
			}
			while (idx);
		}
	}
	return isLAA;
}

double s_nvseVersion = 0;

bool Cmd_EmptyCommand_Execute(COMMAND_ARGS)
{
	*result = 0;
	return true;
}

#if LOG_HOOKS
Map<UInt32, UInt8*> s_hookOriginalData(0x200);

void __stdcall StoreOriginalData(UInt32 addr, UInt8 size)
{
	UInt8 **dataPtr;
	if (s_hookOriginalData.Insert(addr, &dataPtr))
	{
		UInt8 *data = (UInt8*)malloc(0x10);
		*dataPtr = data;
		*data++ = size;
		memcpy(data, (void*)addr, size);
	}
}
#endif