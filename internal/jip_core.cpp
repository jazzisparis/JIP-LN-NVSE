#include "internal/jip_core.h"

NVSEStringVarInterface g_stringVar;
NVSEArrayVarInterface g_arrayVar;
NVSECommandTableInterface g_commandTbl;
NVSEScriptInterface g_script;
NVSESerializationInterface g_serialization;
ExpressionEvaluatorUtils s_expEvalUtils;

_CaptureLambdaVars CaptureLambdaVars;
_UncaptureLambdaVars UncaptureLambdaVars;
_InventoryRefCreate InventoryRefCreate;
_InventoryRefGetForID InventoryRefGetForID;

DIHookControl *g_DIHookCtrl;

_UIOInjectComponent UIOInjectComponent = nullptr;

ModelLoader *g_modelLoader;
DataHandler *g_dataHandler;
LoadedReferenceCollection *g_loadedReferences;
InterfaceManager *g_interfaceManager;
OSGlobals *g_OSGlobals;
TES *g_TES;
Sky *g_currentSky;
PlayerCharacter *g_thePlayer;
NiCamera *g_mainCamera;
TaskQueueInterface *g_scrapHeapQueue;
FontManager *g_fontManager;
OSInputGlobals *g_inputGlobals;
TileMenu **g_tileMenuArray;
HUDMainMenu *g_HUDMainMenu;
ConsoleManager *g_consoleManager;
NiNode *s_pc1stPersonNode = nullptr, *g_cursorNode;
ShadowSceneNode *g_shadowSceneNode;
TESObjectREFR *s_tempPosMarker;
float g_screenWidth, g_screenHeight;
const char *g_terminalModelDefault;
TESObjectWEAP *g_fistsWeapon;
TESObjectACTI *g_ashPileACTI, *g_gooPileACTI;
TESGlobal *g_gameYear, *g_gameMonth, *g_gameDay, *g_gameHour, *g_timeScale;
TESObjectMISC *g_capsItem;
TESImageSpaceModifier *g_getHitIMOD, *g_explosionInFaceIMOD;
UInt32 s_mainThreadID, s_initialTickCount;

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

JIPSerializedVars s_serializedVars;

__declspec(naked) void __vectorcall ResultVars::Set(__m128 values)
{
	__asm
	{
		andps	xmm0, PS_XYZ0Mask
		cvtps2pd	xmm1, xmm0
		shufps	xmm0, xmm0, 0xFE
		cvtps2pd	xmm0, xmm0
		mov		eax, [ecx]
		movlpd	[eax+8], xmm1
		mov		eax, [ecx+4]
		movhpd	[eax+8], xmm1
		mov		eax, [ecx+8]
		movlpd	[eax+8], xmm0
		retn
	}
}

__declspec(naked) void __vectorcall ResultVars::Set(__m128 values, const __m128 modifier)
{
	__asm
	{
		mulps	xmm0, xmm1
		cvtps2pd	xmm1, xmm0
		shufps	xmm0, xmm0, 0xFE
		cvtps2pd	xmm0, xmm0
		mov		eax, [ecx]
		movlpd	[eax+8], xmm1
		mov		eax, [ecx+4]
		movhpd	[eax+8], xmm1
		mov		eax, [ecx+8]
		movlpd	[eax+8], xmm0
		retn
	}
}

__declspec(noinline) TempFormList *GetTempFormList()
{
	thread_local static TempObject<TempFormList> s_tempFormList;
	s_tempFormList->Clear();
	return *s_tempFormList;
}

__declspec(noinline) TempElements *GetTempElements()
{
	thread_local static TempObject<TempElements> s_tempElements;
	s_tempElements->Clear();
	return *s_tempElements;
}

__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds:0x11C54C0
		mov		eax, [esp+4]
		xor		edx, edx
		div		dword ptr [ecx+4]
		mov		eax, [ecx+8]
		mov		eax, [eax+edx*4]
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		ALIGN 16
	iterHead:
		cmp		[eax+4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		retn	4
	found:
		mov		eax, [eax+8]
	done:
		retn	4
	}
}

__declspec(naked) TESForm* __fastcall LookupFormByEDID(const char *edidStr)
{
	__asm
	{
		mov		edx, ecx
		mov		ecx, ds:0x11C54C8
		jmp		NiTMap<UInt32, UInt32>::Lookup
	}
}

__declspec(naked) UInt32 __stdcall HasChangeData(UInt32 refID)
{
	__asm
	{
		mov		eax, g_BGSSaveLoadGame
		mov		ecx, [eax]
		mov		eax, [esp+4]
		xor		edx, edx
		div		dword ptr [ecx+4]
		mov		eax, [ecx+8]
		mov		eax, [eax+edx*4]
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		ALIGN 16
	iterHead:
		cmp		[eax+4], edx
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
	done:
		retn	4
	}
}

__declspec(naked) bool __fastcall GetResolvedModIndex(UInt8 *pModIdx)
{
	__asm
	{
		movzx	edx, byte ptr [ecx]
		cmp		dl, 0xFF
		jz		retn1
		mov		eax, g_BGSSaveLoadGame
		mov		al, [eax+edx+0x44]
		cmp		al, 0xFF
		jz		retn0
		mov		[ecx], al
	retn1:
		mov		al, 1
		retn
	retn0:
		xor		al, al
		retn
	}
}

__declspec(naked) UInt32 __fastcall GetResolvedRefID(UInt32 *refID)
{
	__asm
	{
		cmp		dword ptr [ecx], 0
		jz		retnArg
		movzx	eax, byte ptr [ecx+3]
		cmp		al, 0xFF
		jz		retnArg
		add		eax, g_BGSSaveLoadGame
		mov		dl, [eax+0x44]
		cmp		dl, 0xFF
		jz		retn0
		mov		[ecx+3], dl
	retnArg:
		mov		eax, [ecx]
		retn
	retn0:
		xor		eax, eax
		mov		[ecx], eax
		retn
	}
}

TempObject<UnorderedMap<const char*, UInt32>> s_strRefs;

UInt32 __fastcall StringToRef(char *refStr)
{
	UInt32 *findStr;
	if (!s_strRefs->InsertKey(refStr, &findStr)) return *findStr;
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
	if (TESForm *form = GameGlobals::EditorIDsMap()->Lookup(refStr))
		return *findStr = form->refID;
	*findStr = HexToUInt(refStr);
	return GetResolvedRefID(findStr);
}

__declspec(noinline) InventoryItemsMap *GetInventoryItemsMap()
{
	thread_local static TempObject<InventoryItemsMap> s_inventoryItemsMap;
	s_inventoryItemsMap->Clear();
	return *s_inventoryItemsMap;
}

bool GetInventoryItems(TESObjectREFR *refr, UInt8 typeID, InventoryItemsMap *invItemsMap)
{
	TESContainer *container = refr->baseForm->GetContainer();
	if (!container) return false;
	ContChangesEntryList *entryList = refr->GetContainerChangesList();
	if (!entryList) return false;

	auto contIter = container->formCountList.Head();
	do
	{
		if (TESContainer::FormCount *formCount = contIter->data)
			if (TESForm *item = formCount->form; NOT_ID(item, TESLevItem) && (!typeID || (item->typeID == typeID)) && !invItemsMap->HasKey(item))
			{
				SInt32 contCount = container->GetCountForForm(item);
				ContChangesEntry *entry = entryList->FindForItem(item);
				if (entry)
				{
					if (entry->HasExtraLeveledItem())
						contCount = entry->countDelta;
					else contCount += entry->countDelta;
				}
				if (contCount > 0)
					invItemsMap->Emplace(item, contCount, entry);
			}
	}
	while (contIter = contIter->next);

	auto xtraIter = entryList->Head();
	do
	{
		if (ContChangesEntry *entry = xtraIter->data)
			if (TESForm *item = entry->type; (entry->countDelta > 0) && (!typeID || (item->typeID == typeID)) && !invItemsMap->HasKey(item))
				invItemsMap->Emplace(item, entry->countDelta, entry);
	}
	while (xtraIter = xtraIter->next);

	return !invItemsMap->Empty();
}

__declspec(naked) void __fastcall ShowItemMessage(TESForm *item, const char *msgStr)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	edx
		call	TESForm::GetTheName
		cmp		[eax], 0
		jz		done
		sub		esp, 0x70
		mov		edx, eax
		lea		ecx, [ebp-0x74]
		call	StrCopy
		mov		[eax], ' '
		mov		edx, [ebp-4]
		lea		ecx, [eax+1]
		call	StrCopy
		mov		word ptr [eax], '.'
		push	0
		push	0x40000000
		push	0
		push	0x10208E0
		push	0
		lea		eax, [ebp-0x74]
		push	eax
		CALL_EAX(0x7052F0)
	done:
		leave
		retn
	}
}

struct RayCastData
{
	hkVector4	pos0;		// 00	
	hkVector4	pos1;		// 10
	UInt8		byte20;		// 20
	UInt8		pad21[3];	// 21
	UInt8		layerType;	// 24
	UInt8		filterFlags;// 25
	UInt16		group;		// 26
	UInt32		unk28[6];	// 28
	float		flt40;		// 40
	UInt32		unk44[15];	// 44
	hkCdBody	*cdBody;	// 80
	UInt32		unk84[3];	// 84
	hkVector4	vector90;	// 90
	UInt32		unkA0[3];	// A0
	UInt8		byteAC;		// AC
	UInt8		padAD[3];	// AD
};
static_assert(sizeof(RayCastData) == 0xB0);

__declspec(naked) NiAVObject* __fastcall _GetRayCastObject(RayCastData *rcData)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		mov		eax, [ebp+0xC]
		mov		ecx, [ebp+8]
		movups	xmm0, [eax]
		shufps	xmm0, xmm0, 0xC
		movss	xmm1, [eax+0x18]
		unpcklpd	xmm0, xmm1
		movss	xmm1, [ebp+0x10]
		shufps	xmm1, xmm1, 0x40
		mulps	xmm0, xmm1
		movups	xmm1, [ecx]
		movaps	xmm2, kUnitConv
		mulps	xmm1, xmm2
		movaps	[ebx], xmm1
		mulps	xmm0, xmm2
		addps	xmm0, xmm1
		movaps	[ebx+0x10], xmm0
		mov		dword ptr [ebx+0x40], 0x3F800000
		or		eax, 0xFFFFFFFF
		mov		[ebx+0x44], eax
		mov		[ebx+0x50], eax
		xor		eax, eax
		mov		[ebx+0x20], al
		lea		ecx, [ebx+0x70]
		mov		[ecx], eax
		mov		[ecx+0x10], eax
		xorps	xmm0, xmm0
		movaps	[ecx+0x20], xmm0
		movaps	[ecx+0x30], xmm0
		mov		eax, g_thePlayer
		mov		ecx, [eax+0x68]
		mov		eax, [ecx+0x138]
		mov		ecx, [eax+0x594]
		mov		eax, [ecx+8]
		mov		ecx, [eax+0x2C]
		mov		eax, [ebp+0x14]
		mov		edx, 6
		test	eax, eax
		cmovs	eax, edx
		and		eax, 0x7F
		mov		cx, ax
		mov		[ebx+0x24], ecx
		push	1
		push	ebx
		mov		ecx, g_TES
		CALL_EAX(0x458440)
		pop		ebx
		retn
		ALIGN 16
	kUnitConv:
		EMIT_PS_3(0x3E124DD2)
	}
}

__declspec(naked) NiAVObject* __stdcall GetRayCastObject(const NiVector3 &posVector, float *rotMatRow, float maxRange, SInt32 layerType)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xB0
		mov		ecx, esp
		call	_GetRayCastObject
		leave
		retn	0x10
	}
}

__declspec(naked) bool NiVector4::RayCastCoords(const NiVector3 &posVector, float *rotMatRow, float maxRange, SInt32 layerType)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xB0
		push	ecx
		lea		ecx, [esp+4]
		call	_GetRayCastObject
		pop		ecx
		movaps	xmm0, [esp]
		movaps	xmm1, [esp+0x10]
		subps	xmm1, xmm0
		pshufd	xmm2, [esp+0x40], 0
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		mulps	xmm0, PS_HKUnitCnvrt
		movups	[ecx], xmm0
		test	eax, eax
		jnz		done
		mov		eax, g_TES
		cmp		dword ptr [eax+0x34], 0
		jnz		done
		cvttps2dq	xmm0, xmm0
		psrad	xmm0, 0xC
		pshuflw	xmm0, xmm0, 2
		mov		ecx, [eax+8]
		call	GridCellArray::GetCellAtCoord
		test	eax, eax
	done:
		setnz	al
		leave
		retn	0x10
	}
}

__declspec(naked) int __stdcall GetRayCastMaterial(const NiVector3 &posVector, float *rotMatRow, float maxRange, SInt32 layerType)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xC0
		lea		ecx, [esp+0x10]
		call	_GetRayCastObject
		test	eax, eax
		jz		invalid
		mov		[esp+0xC], eax
		mov		ecx, eax
		call	NiAVObject::GetParentRef
		test	eax, eax
		jz		isTerrain
		mov		ecx, eax
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		notActor
		mov		eax, [ecx]
		call	dword ptr [eax+0x218]
		mov		ecx, [ecx+0x20]
		test	al, al
		jz		notNPC
		mov		eax, [ecx+0x1E4]
		leave
		retn	0x14
	notNPC:
		mov		eax, [ecx+0x148]
		leave
		retn	0x14
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
		mov		eax, [ecx+0x10]
		jmp		convert
	isTerrain:
		movaps	xmm0, [esp+0x10]
		movaps	xmm1, [esp+0x20]
		subps	xmm1, xmm0
		pshufd	xmm2, [esp+0x50], 0
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		mulps	xmm0, PS_HKUnitCnvrt
		movaps	[esp], xmm0
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
		leave
		retn	0x14
	invalid:
		or		eax, 0xFFFFFFFF
		leave
		retn	0x10
	}
}

TempObject<UnorderedMap<TESNPC*, AppearanceUndo*>> s_appearanceUndoMap;

hkpWorld *GethkpWorld()
{
	bhkWorld *hkWorld = nullptr;
	TESObjectCELL *cell = g_TES->currentInterior;
	if (cell)
	{
		if (ExtraHavok *xHavok = GetExtraType(&cell->extraDataList, ExtraHavok))
			hkWorld = xHavok->world;
	}
	else hkWorld = bhkWorldM::GetSingleton();
	return hkWorld ? (hkpWorld*)hkWorld->refObject : nullptr;
}

NVSEArrayVar *TESRecipe::ComponentList::GetComponents(Script *scriptObj)
{
	TempElements *tmpElements = GetTempElements();
	Node *iter = Head();
	do
	{
		if (RecipeComponent *component = iter->data)
			tmpElements->Append(component->item);
	}
	while (iter = iter->next);
	return CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
}

bool LeveledListHasFormDeep(TESLeveledList *pLvlList, TESForm *form, TempFormList *tmpFormLst)
{
	auto iter = pLvlList->list.Head();
	do
	{
		if (TESLeveledList::ListData *data = iter->data)
			if (data->form == form)
				return true;
			else if (TESLeveledList* lvlList = data->form->GetLvlList(); lvlList && tmpFormLst->Insert(data->form) && LeveledListHasFormDeep(lvlList, form, tmpFormLst))
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
	return (float)totalDays + g_gameHour->data * (1 / 24.0F);
}

TempObject<UnorderedMap<UInt32, ScriptVariablesMap>> s_scriptVariablesBuffer;
TempObject<UnorderedMap<UInt32, VariableNames>> s_addedVariables;

bool __fastcall GetVariableAdded(UInt32 ownerID, char *varName)
{
	if (VariableNames *findOwner = s_addedVariables->GetPtr(ownerID); findOwner && findOwner->HasKey(varName))
		return true;
	return false;
}

ScriptVar *Script::AddVariable(char *varName, ScriptLocals *eventList, UInt32 ownerID, UInt8 modIdx)
{
	VariableInfo *varInfo = GetVariableByName(varName);
	if (!varInfo)
	{
		varInfo = Game_HeapAlloc<VariableInfo>();
		ZeroMemory(varInfo, sizeof(VariableInfo));
		varInfo->idx = ++info.varCount;
		varInfo->name.Set(varName);
		varList.Prepend(varInfo);
		s_addedVariables()[refID].Insert(varName);
	}
	else if (!GetVariableAdded(refID, varName)) return nullptr;

	ScriptVar *var = eventList->GetVariable(varInfo->idx);
	if (!var)
	{
		var = Game_HeapAlloc<ScriptVar>();
		var->id = varInfo->idx;
		var->data = 0.0;
		eventList->m_vars->Prepend(var);
	}

	if (varName[0] != '*')
		s_scriptVariablesBuffer()[ownerID][varName].Set(var, modIdx);
	return var;
}

__declspec(naked) TESIdleForm *AnimData::GetPlayedIdle() const
{
	__asm
	{
		mov		eax, [ecx+0x128]
		test	eax, eax
		jz		noQueued
		mov		eax, [eax+0x2C]
		test	eax, eax
		jnz		done
	noQueued:
		mov		eax, [ecx+0x124]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x2C]
		test	eax, eax
		jz		done
		push	eax
		CALL_EAX(0x4985F0)
		pop		edx
		movzx	eax, al
		dec		eax
		and		eax, edx
	done:
		retn
	}
}

TempObject<UnorderedMap<UInt32, LinkedRefEntry>> s_linkedRefModified;
TempObject<UnorderedMap<UInt32, UInt32>> s_linkedRefDefault, s_linkedRefsTemp;

bool TESObjectREFR::SetLinkedRef(TESObjectREFR *linkObj = nullptr, UInt8 modIdx)
{
	ExtraLinkedRef *xLinkedRef = GetExtraType(&extraDataList, ExtraLinkedRef);
	if (!linkObj)
	{
		if (auto findDefID = s_linkedRefDefault->Find(refID))
		{
			if (xLinkedRef)
				if (!*findDefID)
					extraDataList.RemoveByType(kXData_ExtraLinkedRef);
				else if (TESForm* form = LookupFormByRefID(*findDefID); form && IS_REFERENCE(form))
					xLinkedRef->linkedRef = (TESObjectREFR*)form;
			findDefID.Remove();
		}
		s_linkedRefModified->Erase(refID);
		return true;
	}
	if (!xLinkedRef)
	{
		extraDataList.AddExtra(ExtraLinkedRef::Create(linkObj));
		s_linkedRefDefault()[refID] = 0;
	}
	else
	{
		if (!xLinkedRef->linkedRef) return false;
		s_linkedRefDefault()[refID] = xLinkedRef->linkedRef->refID;
		xLinkedRef->linkedRef = linkObj;
	}
	s_linkedRefModified()[refID].Set(linkObj->refID, modIdx);
	return true;
}

bool SetLinkedRefID(UInt32 thisID, UInt32 linkID, UInt8 modIdx)
{
	if (TESObjectREFR *thisObj = (TESObjectREFR*)LookupFormByRefID(thisID); thisObj && IS_REFERENCE(thisObj))
		if (!linkID)
			return thisObj->SetLinkedRef();
		else if (TESObjectREFR *linkObj = (TESObjectREFR*)LookupFormByRefID(linkID); linkObj && IS_REFERENCE(linkObj) && thisObj->SetLinkedRef(linkObj, modIdx))
			return true;
	return false;
}

TempObject<AuxVarModsMap> s_auxVariables[2] = {8, 8};

TempObject<RefMapModsMap> s_refMapArrays[2] = {8, 8};

PrimitiveCS s_auxVarCS, s_refMapCS;

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
		CallFunction(*script, nullptr, 1, arg);
}

void EventCallbackScripts::InvokeEvents2(UInt32 arg1, UInt32 arg2)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, nullptr, 2, arg1, arg2);
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

TempObject<CriticalHitEventCallbacks> s_criticalHitEvents;

TempObject<UnorderedMap<Script*, DisabledScriptBlocks>> s_disabledScriptBlocksMap;

ExtraDataList *InventoryRef::CreateExtraData()
{
	ContChangesEntryList *entryList = containerRef->GetContainerChangesList();
	if (!entryList) return nullptr;
	ContChangesEntry *pEntry = entryList->FindForItem(type);
	if (!pEntry)
	{
		pEntry = ContChangesEntry::Create(type, 0);
		entryList->Prepend(pEntry);
	}
	xData = ExtraDataList::Create();
	if (pEntry->extendData)
		pEntry->extendData->Prepend(xData);
	else
	{
		pEntry->extendData = Game_HeapAlloc<ContChangesExtraList>();
		pEntry->extendData->Init(xData);
	}
	containerRef->MarkModified(0x20);
	return xData;
}

ExtraDataList* __fastcall InventoryRef::SplitFromStack(SInt32 maxStack)
{
	if (ExtraCount *xCount = GetExtraType(xData, ExtraCount))
	{
		SInt32 delta = xCount->count - maxStack;
		if (delta <= 0)
			return xData;
		ContChangesEntry *pEntry = containerRef->GetContainerChangesEntry(type);
		if (!pEntry) return xData;
		ExtraDataList *xDataOut = xData->CreateCopy();
		pEntry->extendData->Prepend(xDataOut);
		xDataOut->RemoveByType(kXData_ExtraWorn);
		xDataOut->RemoveByType(kXData_ExtraHotkey);
		if (delta < 2)
		{
			xData->RemoveByType(kXData_ExtraCount);
			if (!xData->m_data)
			{
				pEntry->extendData->Remove(xData);
				Game_HeapFree(xData);
			}
		}
		else xCount->count = delta;
		xData = xDataOut;
		if (maxStack > 1)
		{
			xCount = GetExtraType(xData, ExtraCount);
			xCount->count = maxStack;
		}
		else xData->RemoveByType(kXData_ExtraCount);
	}
	else if (maxStack > 1)
		xData->AddExtraCount(maxStack);
	return xData;
}

TESObjectREFR* __fastcall CreateRefForStack(TESObjectREFR *container, ContChangesEntry *menuEntry)
{
	return InventoryRefCreate(container, menuEntry->type, menuEntry->countDelta, menuEntry->extendData ? menuEntry->extendData->GetFirstItem() : nullptr);
}

TESObjectREFR* __fastcall GetEquippedItemRef(Actor *actor, UInt32 slotIndex)
{
	ContChangesEntryList *entryList = actor->GetContainerChangesList();
	if (!entryList) return nullptr;
	UInt32 partMask = 1 << slotIndex;
	TESForm *item;
	ContChangesEntry *entry;
	ExtraDataList *xData;
	if (actor->GetRefNiNode())
	{
		if (slotIndex == 5)
		{
			entry = actor->GetWeaponInfo();
			if (entry && entry->extendData)
				return InventoryRefCreate(actor, entry->type, entry->countDelta, entry->extendData->GetFirstItem());
		}
		else if (BipedAnim *equipment = actor->GetBipedAnim())
		{
			BipedAnim::Data *slotData = equipment->slotData;
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
	else
	{
		auto listIter = entryList->Head();
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
	return nullptr;
}

ContChangesEntry* __fastcall GetHotkeyItemEntry(UInt8 index, ExtraDataList **outXData)
{
	if (ContChangesEntryList *entryList = g_thePlayer->GetContainerChangesList())
	{
		auto entryIter = entryList->Head();
		do
		{
			if (ContChangesEntry *entry = entryIter->data; entry && entry->extendData && ((entry->type->typeID == kFormType_TESObjectARMO) ||
				(entry->type->typeID == kFormType_TESObjectWEAP) || (entry->type->typeID == kFormType_AlchemyItem)))
			{
				auto xdlIter = entry->extendData->Head();
				do
				{
					if (xdlIter->data)
						if (ExtraHotkey *xHotkey = GetExtraType(xdlIter->data, ExtraHotkey); xHotkey && (xHotkey->index == index))
						{
							*outXData = xdlIter->data;
							return entry;
						}
				}
				while (xdlIter = xdlIter->next);
			}
		}
		while (entryIter = entryIter->next);
	}
	return nullptr;
}

bool __fastcall ClearHotkey(UInt8 index)
{
	ExtraDataList *xData;
	if (ContChangesEntry *entry = GetHotkeyItemEntry(index, &xData))
	{
		xData->RemoveByType(kXData_ExtraHotkey);
		if (!xData->m_data)
		{
			entry->extendData->Remove(xData);
			Game_HeapFree(xData);
		}
		return true;
	}
	return false;
}

__declspec(naked) TESObjectREFR *TESObjectREFR::CreateInventoryRefForScriptedObj(TESForm *item, ScriptLocals *eventList)
{
	__asm
	{
		push	esi
		push	ecx
		push	dword ptr [esp+0xC]
		call	TESObjectREFR::GetContainerChangesEntry
		test	eax, eax
		jz		done
		mov		esi, [eax]
		ALIGN 16
	xdlIter:
		test	esi, esi
		jz		done
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		xdlIter
		push	kXData_ExtraScript
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		xdlIter
		mov		edx, [eax+0x10]
		cmp		[esp+0x10], edx
		jnz		xdlIter
		push	kXData_ExtraCount
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		doneCount
		movsx	eax, word ptr [eax+0xC]
	doneCount:
		mov		edx, 1
		cmp		eax, edx
		cmovl	eax, edx
		push	ecx
		push	eax
		push	dword ptr [esp+0x14]
		push	dword ptr [esp+0xC]
		call	InventoryRefCreate
		pop		ecx
		pop		esi
		retn	8
	done:
		xor		eax, eax
		pop		ecx
		pop		esi
		retn	8
	}
}

__declspec(naked) void ValidateOpcodeSample()
{
	__asm
	{
		push	ebx
		mov		ecx, ds:18628396
		add		ecx, 536
		mov		ebx, [ecx]
		mov		edx, -1071495146
		ALIGN 16
	baseIter:
		add		ecx, 4
		mov		eax, [ecx]
		cmp		[eax+0x120], edx
		jz		hasBase
		dec		ebx
		jnz		baseIter
		jmp		checkPOE
	hasBase:
		cmp		dword ptr [eax+0x3E0], 0x4000
		jb		setOp
		mov		ebx, eax
	checkPOE:
		push	101
		push	1986622020
		push	1128547924
		mov		edx, esp
		mov		ecx, ds:18633928
		call	NiTMap<UInt32, UInt32>::Lookup
		add		esp, 0xC
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+4], 17
		jnz		done
		cmp		[eax+0x10], ebx
		jz		done
	setOp:
		mov		ebx, g_commandTbl+12
		push	10442
		call	ebx
		mov		ecx, ebx
		mov		ebx, 6113856
		mov		[eax+0x18], ebx
		xor		[esp], 31
		call	ecx
		pop		ecx
		mov		[eax+0x18], ebx
	done:
		pop		ebx
		retn
	}
}

__declspec(naked) float __vectorcall GetLightAmount(LightingData *lightingData, __m128 pos)
{
	__asm
	{
		xorps	xmm3, xmm3
		cmp		[ecx+0xEC], 1
		jz		done
		mov		eax, [ecx+0xF8]
		test	byte ptr [eax+0x30], 1
		jnz		done
		add		eax, 0x8C
		movss	xmm1, PS_V3_One
		cmp		[ecx+0xF4], 0
		jz		skipDist
		movups	xmm2, [eax]
		andps	xmm2, PS_XYZ0Mask
		subps	xmm2, xmm0
		mulps	xmm2, xmm2
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		sqrtss	xmm2, xmm2
		divss	xmm2, [eax+0x54]
		comiss	xmm2, xmm1
		jnb		done
		mulss	xmm2, xmm2
		subss	xmm1, xmm2
	skipDist:
		movss	xmm3, [eax+0x48]
		maxss	xmm3, [eax+0x4C]
		maxss	xmm3, [eax+0x50]
		mulss	xmm3, [eax+0x38]
		mulss	xmm3, xmm1
	done:
		movq	xmm0, xmm3
		retn
	}
}

__declspec(naked) float __vectorcall GetLightAmountAtPoint(const NiVector3 &pos)
{
	__asm
	{
		push	esi
		mov		esi, g_shadowSceneNode
		mov		eax, [esi+0xE0]
		xorps	xmm5, xmm5
		test	eax, eax
		jz		done
		movups	xmm6, [ecx]
		andps	xmm6, PS_XYZ0Mask
		mov		edx, g_TES
		cmp		dword ptr [edx+0x34], 0
		jnz		isInterior
		push	eax
		cvttps2dq	xmm0, xmm6
		psrad	xmm0, 0xC
		pshuflw	xmm0, xmm0, 2
		mov		ecx, [edx+8]
		call	GridCellArray::GetCellAtCoord
		pop		ecx
		test	eax, eax
		jz		done
		movaps	xmm0, xmm6
		call	GetLightAmount
		movq	xmm5, xmm0
		jmp		doneCell
	isInterior:
		mov		eax, [eax+0xF8]
		add		eax, 0xC8
		movss	xmm0, [eax]
		maxss	xmm0, [eax+4]
		maxss	xmm0, [eax+8]
		movss	xmm5, [eax+0xC]
		maxss	xmm5, [eax+0x10]
		maxss	xmm5, [eax+0x14]
		addss	xmm5, xmm0
	doneCell:
		mov		ecx, SCENE_LIGHTS_CS
		call	LightCS::Enter
		mov		esi, [esi+0xB4]
		movss	xmm7, PS_V3_One
		ALIGN 16
	iterHead:
		comiss	xmm5, xmm7
		jnb		doClamp
		test	esi, esi
		jz		doClamp
		mov		ecx, [esi+8]
		mov		esi, [esi]
		test	ecx, ecx
		jz		iterHead
		movaps	xmm0, xmm6
		call	GetLightAmount
		addss	xmm5, xmm0
		jmp		iterHead
	doClamp:
		xorps	xmm0, xmm0
		maxss	xmm5, xmm0
		movss	xmm0, SS_100
		mulss	xmm5, xmm0
		minss	xmm5, xmm0
		mov		ecx, SCENE_LIGHTS_CS
		dec		dword ptr [ecx+4]
		jnz		done
		and		dword ptr [ecx], 0
	done:
		movq	xmm0, xmm5
		pop		esi
		retn
	}
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

TempObject<UnorderedMap<const char*, JIPScriptRunner::CachedSRScript>> s_cachedScripts;

namespace JIPScriptRunner
{
	UInt8 initInProgress = 0;
	char scriptsPath[0x100] = "Data\\NVSE\\plugins\\scripts\\*.txt";

	void Init()
	{
		if (s_log())
			WriteRelCall(0x5AEB66, (UInt32)LogCompileError);
		ValidateOpcodeSample();
		initInProgress = 1;
		for (DirectoryIterator iter(scriptsPath); iter; ++iter)
			if (iter.IsFile())
				if (char *fileName = const_cast<char*>(*iter); fileName[2] == '_')
					if (ScriptRunOn runOn = ScriptRunOn(*(UInt16*)fileName | 0x2020); (runOn == kRunOn_RestartGame) ||
						(runOn == kRunOn_LoadGame) || (runOn == kRunOn_ExitToMainMenu) || (runOn == kRunOn_NewGame) ||
						(runOn == kRunOn_LoadOrNewGame) || (runOn == kRunOn_SaveGame) || (runOn == kRunOn_ExitGame))
						if (char *buffer = GetStrArgBuffer(); StrCopy(scriptsPath + 26, fileName) && FileToBuffer(scriptsPath, buffer, STR_BUFFER_SIZE - 1))
							if (runOn == kRunOn_RestartGame)
								RunScriptSource(buffer, fileName, true);
							else if (Script *pScript = Script::Create(buffer, fileName))
								s_cachedScripts->Emplace(fileName, pScript, runOn);
		if (initInProgress == 2)
		{
			fputs("================================================================\n\n", s_log());
			fflush(s_log());
		}
		initInProgress = 0;
	}

	bool RegisterScript(char *relPath)
	{
		ReplaceChr(relPath, '/', '\\');
		if (char *fileName = FindChrR(relPath, '\\'); fileName++ && (fileName[2] == '_'))
			if (!s_cachedScripts->HasKey(fileName))
				if (ScriptRunOn runOn = ScriptRunOn(*(UInt16*)fileName | 0x2020); (runOn == kRunOn_LoadGame) || (runOn == kRunOn_ExitToMainMenu) ||
					(runOn == kRunOn_NewGame) || (runOn == kRunOn_LoadOrNewGame) || (runOn == kRunOn_SaveGame) || (runOn == kRunOn_ExitGame))
					if (char *buffer = GetStrArgBuffer(); StrCopy(scriptsPath + 26, relPath) && FileToBuffer(scriptsPath, buffer, STR_BUFFER_SIZE - 1))
						if (Script *pScript = Script::Create(buffer, fileName))
						{
							s_cachedScripts->Emplace(fileName, pScript, runOn);
							return true;
						}
		return false;
	}

	void RunScripts(ScriptRunOn runOn1, ScriptRunOn runOn2)
	{
		for (auto iter = s_cachedScripts->Begin(); iter; ++iter)
			if ((iter().runOn == runOn1) || (runOn2 && (iter().runOn == runOn2)))
				iter().script()->Execute(nullptr, nullptr, nullptr, false);
	}

	void __fastcall RunScript(Script *script, int, TESObjectREFR *callingRef)
	{
		ExtraScript *xScript = GetExtraType(&callingRef->extraDataList, ExtraScript);
		ScriptLocals *eventList = xScript ? xScript->eventList : nullptr;
		if (eventList) xScript->eventList = nullptr;
		script->Execute(callingRef);
		if (eventList) xScript->eventList = eventList;
	}

	bool __fastcall RunScriptSource(char *scrSource, const char *scrName, bool capture)
	{
		StackObject<Script> tempScript;
		bool success = tempScript->Init(scrSource, scrName);
		if (success)
			if (capture)
			{
				CaptureLambdaVars(*tempScript);
				tempScript->Execute();
				UncaptureLambdaVars(*tempScript);
			}
			else tempScript->Execute();
		tempScript->Destructor();
		return success;
	}

	void __fastcall LogCompileError(String &errorStr)
	{
		if (initInProgress && (errorStr.m_dataLen > 9))
		{
			if (initInProgress == 1)
			{
				initInProgress = 2;
				fputs("[SCRIPT RUNNER] Errors were found while compiling the following script files:\n\n", s_log());
				Console_Print("[SCRIPT RUNNER] One or more script files could not be compiled. For more info please refer to \"jip_ln_nvse.log\".");
			}
			fputs(errorStr.m_data + 9, s_log());
			fputs("\n\n", s_log());
		}
		Game_HeapFree(errorStr.m_data);
	}
};

TempObject<UnorderedMap<const char*, NiCamera*>> s_extraCamerasMap;

bool s_HUDCursorMode = false;

bool GetMenuMode()
{
	return (g_interfaceManager->currentMode > 1) || s_HUDCursorMode;
}

void __fastcall RefreshRecipeMenu(RecipeMenu *menu);

__declspec(noinline) void RefreshItemListBox()
{
	if (MENU_VISIBILITY[kMenuType_Inventory])
		CdeclCall(0x782A90);
	else if (MENU_VISIBILITY[kMenuType_Stats])
		ThisCall(0x7DF230, StatsMenu::Get(), 4);
	else if (MENU_VISIBILITY[kMenuType_Container])
		ContainerMenu::Get()->Refresh(nullptr);
	else if (MENU_VISIBILITY[kMenuType_Map])
	{
		if (MapMenu *mapMenu = MapMenu::Get(); mapMenu->currentTab == MapMenu::kTab_WorldMap)
		{
			s_mapMenuSkipSetXY = true;
			ThisCall(0x79DBB0, mapMenu);
		}
	}
	else if (MENU_VISIBILITY[kMenuType_Barter])
		CdeclCall(0x730690, 1);
	else if (MENU_VISIBILITY[kMenuType_Recipe])
		RefreshRecipeMenu(RecipeMenu::Get());
}

__declspec(naked) bool IsConsoleOpen()
{
	__asm
	{
		mov		al, byte ptr ds:0x11DEA2E
		test	al, al
		jz		done
		mov		eax, fs:0x2C
		mov		edx, g_TLSIndex
		mov		eax, [eax+edx*4]
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
		cmp		dword ptr [ebp+4], 0x5E234B
		jnz		done
		mov		edx, [ebp]
		mov		edx, [edx-0x30]
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
		cmp		dword ptr [ebp+4], 0x5E234B
		jnz		done
		mov		edx, [ebp]
		mov		edx, [edx-0x30]
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

bool IsInMainThread()
{
	return GetCurrentThreadId() == s_mainThreadID;
}

__declspec(naked) TLSData *GetTLSData()
{
	__asm
	{
		mov		eax, fs:0x2C
		mov		edx, g_TLSIndex
		mov		eax, [eax+edx*4]
		retn
	}
}

TempObject<UnorderedMap<const char*, UInt32, 0x20, false>> s_fileExtToType;

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
		mov		dl, '.'
		call	FindChrR
		test	eax, eax
		jz		done
		inc		eax
		push	eax
		mov		ecx, offset s_fileExtToType
		call	UnorderedMap<const char*, UInt32>::Get
		test	eax, eax
		jz		done
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
		mov		edi, ds:0x11F8160
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
		HMODULE gameHandle = GetModuleHandle(nullptr);
		BYTE *dataPtr = (BYTE*)gameHandle + *(size_t*)((BYTE*)gameHandle + 0x3C) + 0x16;
		isLAA = (*dataPtr & 0x20) != 0;
		if (isLAA)
		{
			size_t blockSize = 0x10000000;
			do
			{
				void *block = VirtualAlloc(NULL, blockSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
				if (!block) break;
				size_t edge = (size_t)block + blockSize;
				VirtualFree(block, 0, MEM_RELEASE);
				if (edge > 0x80000000)
				{
					isLAA = 2;
					break;
				}
				blockSize += 0x10000000;
			}
			while (blockSize < 0x80000000);
		}
	}
	return isLAA;
}

double s_nvseVersion = 0;

CommandInfo kEmptyCommand = {"NONE", nullptr, 0, nullptr, 0, 0, kParams_ThreeOptionalInts, (Cmd_Execute)0x5D4A40, nullptr, nullptr, 0};