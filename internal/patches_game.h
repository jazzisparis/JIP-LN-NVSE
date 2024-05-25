#pragma once

__declspec(naked) void InitTopicInfoHook()
{
	__asm
	{
		xor		edx, edx
		mov		[ecx], edx
		mov		[ecx+4], edx
		mov		[ecx+0x38], edx
		retn
	}
}

__declspec(naked) void LoadTopicInfoHook()
{
	__asm
	{
		mov		eax, [esp+8]
		mov		[eax+0x50], ecx
		JMP_EAX(0x619820)
	}
}

__declspec(naked) void InvalidChargenQuestFixHook()
{
	__asm
	{
		cmp		dword ptr [ebp-0x10], 0
		jnz		done
		push	0x102037
		call	LookupFormByRefID
		mov		[ebp-0x10], eax
	done:
		JMP_EAX(0x7D3345)
	}
}

__declspec(naked) void GeneratePlayerNodeHook()
{
	__asm
	{
		push	0
		push	esp
		push	dword ptr ds:0x11CDD7C
		mov		eax, g_modelLoader
		mov		ecx, [eax]
		CALL_EAX(0x4493D0)
		pop		eax
		cmp		dword ptr [eax+8], 0
		jg		skipRefCount
		lock inc dword ptr [eax+8]
	skipRefCount:
		mov		ecx, [eax+0xC]
		call	NiAVObject::CreateCopy
		mov		[ebp-0x18], eax
		or		byte ptr [eax+0x32], 0x20
		mov		s_pc1stPersonNode, eax
		JMP_EAX(0x94E4D9)
	}
}

__declspec(naked) void __fastcall DoQueuedPlayerHook(QueuedPlayer *queuedPlayer)
{
	__asm
	{
		mov		eax, [ecx+0x28]
		test	byte ptr [eax+0x5F], kHookRefFlag5F_Update3D
		jnz		proceed
		jmp		DoQueuedReferenceHook
	proceed:
		push	ebx
		push	ecx
		mov		ebx, eax
		push	1
		push	0
		mov		ecx, ebx
		CALL_EAX(0x5702E0)
		mov		ecx, [ebx+0x68C]
		test	ecx, ecx
		jz		done68C
		push	1
		CALL_EAX(0x418E00)
		and		dword ptr [ebx+0x68C], 0
	done68C:
		mov		ecx, [ebx+0x690]
		test	ecx, ecx
		jz		done690
		push	1
		CALL_EAX(0x418D20)
		and		dword ptr [ebx+0x690], 0
	done690:
		mov		edx, [ebx+0x694]
		test	edx, edx
		jz		done694
		mov		ecx, [edx+0x18]
		test	ecx, ecx
		jz		done694
		push	edx
		mov		eax, [ecx]
		call	dword ptr [eax+0xE8]
	done694:
		xorps	xmm0, xmm0
		movups	ds:0x11E07CC, xmm0
		lea		ecx, [ebx+0xD74]
		movups	[ecx], xmm0
		movups	[ecx+0x10], xmm0
		movups	[ecx+0x20], xmm0
		movups	[ecx+0x30], xmm0
		movups	[ecx+0x40], xmm0
		movups	[ecx+0x50], xmm0
		and		byte ptr [ebx+0x5F], ~kHookRefFlag5F_Update3D
		pop		ecx
		call	DoQueuedReferenceHook
		mov		eax, [ebx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		mov		ecx, [ebx+0x694]
		test	ecx, ecx
		jz		done
		mov		edx, ecx
		cmp		byte ptr [ebx+0x64A], 0
		cmovz	ecx, eax
		cmovz	eax, edx
		or		byte ptr [ecx+0x30], 1
		and		byte ptr [eax+0x30], 0xFE
		mov		ecx, [ebx+0x68]
		test	ecx, ecx
		jz		done
		cmp		byte ptr [ecx+0x28], 0
		jnz		done
		cmp		dword ptr [ecx+0x114], 0
		setnz	al
		movzx	edx, byte ptr [ecx+0x135]
		or		al, dl
		jz		done
		push	ebx
		push	dword ptr [ebx+0x690]
		push	dword ptr [ebx+0x68C]
		push	edx
		push	ebx
		push	dword ptr [ecx+0x1C0]
		push	dword ptr [ebx+0x1B4]
		push	edx
		mov		ebx, ecx
		CALL_EAX(0x923960)
		mov		ecx, ebx
		CALL_EAX(0x923960)
	done:
		pop		ebx
		retn
	}
}

typedef UnorderedMap<const SInt8*, const char*, 0x800, false> NiFixedStringsMap;
TempObject<NiFixedStringsMap> s_NiFixedStringsMap;
PrimitiveCS s_NiFixedStringsCS;

bool __stdcall InsertNiFixedString(const SInt8 *inStr, const char ***outStr)
{
	return s_NiFixedStringsMap->InsertKey(inStr, outStr);
}

__declspec(naked) const char* __cdecl GetNiFixedString(const char *inStr)
{
	__asm
	{
		//JMP_EAX(0xA5B690)

		mov		eax, [esp+4]
		test	eax, eax
		jz		done
		push	esi
		mov		esi, eax
		mov		ecx, offset s_NiFixedStringsCS
		call	PrimitiveCS::Enter
		push	ecx
		push	esp
		push	esi
		call	InsertNiFixedString
		test	al, al
		jnz		addNew
		pop		ecx
		mov		eax, [ecx]
		lock inc dword ptr [eax-8]
		and		s_NiFixedStringsCS.selfPtr, 0
		pop		esi
	done:
		retn
		ALIGN 16
	addNew:
		mov		ecx, esi
		call	StrLen
		push	edi
		mov		edi, eax
		lea		ecx, [eax+0x19]
		and		cl, 0xF0
		call	MemoryPool::Alloc
		mov		dword ptr [eax], 1
		mov		[eax+4], edi
		add		eax, 8
		lea		ecx, [edi+1]
		mov		edi, eax
		rep movsb
		pop		edi
		pop		ecx
		mov		[ecx], eax
		and		s_NiFixedStringsCS.selfPtr, 0
		pop		esi
		retn
	}
}

__declspec(naked) void FreeUnusedNiFixedStrings()
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ecx, offset s_NiFixedStringsCS
		call	PrimitiveCS::Enter
		mov		ebp, s_NiFixedStringsMap
		lea		ebx, [ebp+0x2000]
		ALIGN 16
	bucketIter:
		cmp		ebp, ebx
		jz		done
		mov		esi, ebp
		mov		edi, [esi]
		add		ebp, 4
		ALIGN 16
	entryIter:
		test	edi, edi
		jz		bucketIter
		mov		eax, edi
		mov		edi, [edi]
		mov		ecx, [eax+8]
		cmp		dword ptr [ecx-8], 0
		cmovnz	esi, eax
		jnz		entryIter
		dec		dword ptr s_NiFixedStringsMap+8
		mov		[esi], edi
		push	eax
		and		cl, 0xF0
		mov		edx, [ecx+4]
		add		edx, 0x19
		and		dl, 0xF0
		call	MemoryPool::Free
		mov		edx, 0x10
		pop		ecx
		call	MemoryPool::Free
		jmp		entryIter
		ALIGN 16
	done:
		and		s_NiFixedStringsCS.selfPtr, 0
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
	}
}

__declspec(naked) void __fastcall SetPickedCountHook(ExtraDataList *xDataList)
{
	__asm
	{
		push	ecx
		push	kXData_ExtraCount
		call	BaseExtraList::GetByType
		pop		ecx
		test	eax, eax
		jz		done
		movsx	edx, word ptr [eax+0xC]
		mov		[ebp+0xC], edx
		push	kXData_ExtraCount
		CALL_EAX(ADDR_RemoveExtraType)
	done:
		retn
	}
}

__declspec(naked) void __fastcall CopyExtraListHook(ExtraDataList *xNewList, int, ExtraDataList *xSrcList, bool arg2)
{
	__asm
	{
		mov		eax, [ebp+0xC]
		cmp		eax, 1
		jle		done
		push	ecx
		push	eax
		call	ExtraDataList::AddExtraCount
		pop		ecx
	done:
		JMP_EAX(0x4121E0)
	}
}

void __fastcall ExtraJIPDestroyHook(ExtraJIP *xJIP, int, bool doFree)
{
	if (xJIP->key)
		if (auto findEntry = s_extraDataKeysMap->Find(xJIP->key))
			if (!findEntry().refID || LookupFormByRefID(findEntry().refID) || !HasChangeData(findEntry().refID))
			{
				findEntry.Remove();
				s_dataChangedFlags |= kChangedFlag_ExtraData;
			}
	if (doFree)
		Game_HeapFree(xJIP);
}

bool __fastcall ExtraJIPDiffersHook(ExtraJIP *xJIP, int, BSExtraData *compareTo)
{
	if (xJIP->type == compareTo->type)
		if (ExtraJIPEntry *entry1 = s_extraDataKeysMap->GetPtr(xJIP->key))
			if (ExtraJIPEntry *entry2 = s_extraDataKeysMap->GetPtr(((ExtraJIP*)compareTo)->key))
				return !(*entry1 == *entry2);
	return true;
}

void __fastcall CopyExtraJIPEntry(ExtraJIPEntry *inEntry, UINT key)
{
	s_extraDataKeysMap()[key] = *inEntry;
}

__declspec(naked) void ExtraJIPCreateCopyHook()
{
	__asm
	{
		mov		edx, [ebp+8]
		push	dword ptr [edx+0xC]
		mov		ecx, offset s_extraDataKeysMap
		call	ExtraJIPEntryMap::GetPtr
		test	eax, eax
		jz		done
		mov		esi, eax
		or		s_dataChangedFlags, kChangedFlag_ExtraData
		push	kXData_ExtraJIP
		mov		ecx, [ebp-0x170]
		call	BaseExtraList::GetByType
		test	eax, eax
		jnz		hasXJIP
		push	0
		call	ExtraDataList::AddExtraJIP
	hasXJIP:
		mov		ecx, [ebp+4]
		mov		edx, [ebp]
		mov		dl, [edx+0xC]
		cmp		ecx, 0x412250
		jnz		notCont
		test	dl, dl
		jz		doSwap
	doCopy:
		mov		edx, [eax+0xC]
		test	edx, edx
		jnz		hasKey
		push	eax
		call	ExtraJIP::MakeKey
		pop		ecx
		mov		[ecx+0xC], eax
		mov		edx, eax
	hasKey:
		mov		ecx, esi
		call	CopyExtraJIPEntry
		jmp		done
	notCont:
		cmp		ecx, 0x412523
		jnz		doCopy
		test	dl, dl
		jz		doCopy
	doSwap:
		and		dword ptr [esi], 0
		mov		esi, [ebp+8]
		mov		ecx, [esi+0xC]
		mov		edx, [eax+0xC]
		mov		[esi+0xC], edx
		mov		[eax+0xC], ecx
	done:
		mov		ecx, [ebp-0xC]
		mov		fs:0, ecx
		pop		ecx
		pop		esi
		leave
		retn	4
	}
}

__declspec(naked) void ExtraJIPLoadGameHook()
{
	__asm
	{
		mov		eax, [ebp+8]
		mov		edx, [eax+4]
		add		edx, [eax+0xC]
		add		dword ptr [eax+0xC], 5
		mov		edx, [edx]
		test	edx, edx
		jz		done
		push	edx
		push	kXData_ExtraJIP
		mov		ecx, [ebp-0x5D4]
		call	BaseExtraList::GetByType
		test	eax, eax
		jnz		hasXJIP
		push	0x42C1C3
		jmp		ExtraDataList::AddExtraJIP
	hasXJIP:
		pop		ecx
		mov		edx, [eax+0xC]
		cmp		ecx, edx
		jz		done
		mov		[eax+0xC], ecx
		push	edx
		mov		ecx, offset s_extraDataKeysMap
		call	ExtraJIPEntryMap::Erase
	done:
		JMP_EAX(0x42C1C3)
	}
}

__declspec(naked) void ExtraJIPSaveGameHook()
{
	__asm
	{
		mov		edx, [ebp+8]
		mov		ecx, [ebp-0x20]
		push	0x427EB6
		jmp		ExtraJIP::SaveGame
	}
}

__declspec(naked) bool __fastcall GetShouldSaveUnloadedFormHook(BGSSaveLoadGame *slGame, int, TESForm *form)
{
	__asm
	{
		mov		ecx, [esp+4]
		test	byte ptr [ecx+9], 0x40
		jnz		retn0
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xF0], ADDR_ReturnTrue
		jnz		retn1
		cmp		dword ptr [eax+0x224], ADDR_ReturnTrue
		jz		isProjectile
		cmp		byte ptr [ecx+0xF], 0xFF
		jnz		retn1
		test	byte ptr [ecx+0xA], 0x40
		jnz		retn0
	retn1:
		mov		al, 1
		retn	4
	retn0:
		xor		al, al
		retn	4
	isProjectile:
		call	dword ptr [eax+0x324]
		retn	4
	}
}

typedef Map<TESForm*, SInt32, 0x80> InventoryIterMap;

struct InventoryIterator
{
	TESContainer::FormCountList	*frmCountList;	// 00
	TESContainer::FormCountList	*frmCountIter;	// 04
	TESContainer::FormCountList	*baseFormList;	// 08
	ContChangesEntry			*outEntry;		// 0C
	TESContainer				*container;		// 10
	ContChangesEntryList		*entryListIter;	// 14
	UInt32						stage;			// 18
	UInt32						unk1C;			// 1C
	ContChangesExtraList		*extraList;		// 20
	UInt32						unk24;			// 24
	ContChangesEntryList		*baseEntryList;	// 28
	InventoryIterMap			deltasMap;		// 2C

	InventoryIterator() {}
};
static_assert(sizeof(InventoryIterator) == 0x38);

TempObject<InventoryIterator> s_inventoryIterator;

__declspec(naked) InventoryIterator* __fastcall CreateInventoryIteratorHook(ExtraContainerChanges::Data *xChangesData)
{
	__asm
	{
		push	esi
		mov		esi, offset s_inventoryIterator
		push	ecx
		mov		ecx, [esi]
		test	ecx, ecx
		jz		doneList
		push	ecx
		call	TESContainer::FormCountList::RemoveAll
		call	Game_HeapFree
	doneList:
		xorps	xmm0, xmm0
		movups	[esi], xmm0
		movups	[esi+0x10], xmm0
		movlps	[esi+0x20], xmm0
		pop		ecx
		mov		eax, [ecx]
		mov		[esi+0x28], eax
		and		dword ptr [esi+0x30], 0
		mov		eax, [ecx+4]
		test	eax, eax
		jz		noOwner
		mov		ecx, [eax+0x20]
		call	TESForm::GetContainer
		mov		[esi+0x10], eax
		mov		ecx, eax
		add		eax, 4
		mov		[esi+8], eax
		CALL_EAX(0x482B00)
		mov		[esi], eax
		mov		[esi+4], eax
	noOwner:
		mov		eax, esi
		pop		esi
		retn
	}
}

__declspec(naked) void InventoryIterGetNextHook()
{
	__asm
	{
		mov		esi, [ebp+8]
		mov		eax, [esi+0xC]
		test	eax, eax
		jz		noEntry
		push	ecx
		push	esp
		push	dword ptr [eax+8]
		lea		ecx, [esi+0x2C]
		call	InventoryIterMap::InsertKey
		pop		ecx
		and		eax, 1
		dec		eax
		and		[ecx], eax
		mov		eax, [esi+0xC]
		mov		edx, [eax+4]
		add		[ecx], edx
		jmp		done
		NOP_0x4
	noEntry:
		mov		byte ptr [esi+0x18], 2
	iterHead:
		dec		dword ptr [esi+0x30]
		js		retnNull
		mov		eax, [esi+0x2C]
		mov		edx, [esi+0x30]
		lea		ecx, [eax+edx*8]
		push	ecx
		push	dword ptr [ecx]
		mov		edx, [esi+0x28]
		mov		ecx, [esi+8]
		call	GetFormCount
		pop		ecx
		sub		eax, [ecx+4]
		jle		iterHead
		push	dword ptr [ecx]
		push	eax
		push	0xC
		call	Game_DoHeapAlloc
		and		dword ptr [eax], 0
		pop		dword ptr [eax+4]
		pop		dword ptr [eax+8]
		jmp		done
	retnNull:
		xor		eax, eax
	done:
		mov		ecx, fs:0x2C
		mov		edx, g_TLSIndex
		mov		ecx, [ecx+edx*4]
		mov		edx, [ebp-0x10]
		mov		[ecx+0x2B4], edx
		mov		ecx, [ebp-0xC]
		mov		fs:0, ecx
		pop		ecx
		pop		esi
		leave
		retn	4
	}
}

__declspec(naked) void InitModInfoNameHook()
{
	__asm
	{
		mov		ecx, [ebp+0xC]
		call	StrHashCI
		mov		edx, [ebp+0xC]
		mov		ecx, [ebp-0x3B8]
		mov		[ecx+0x120], eax
		add		ecx, 0x20
		push	0x470D71
		jmp		StrCopy
	}
}

__declspec(naked) void __fastcall SetContainerItemsHealthHook(TESContainer *container, int, float healthPerc)
{
	__asm
	{
		push	esi
		push	edi
		movss	xmm0, [esp+0xC]
		minss	xmm0, kMaxHealth
		xorps	xmm1, xmm1
		maxss	xmm0, xmm1
		movss	[esp+0xC], xmm0
		lea		esi, [ecx+4]
		ALIGN 16
	iterHead:
		test	esi, esi
		jz		done
		mov		edi, [esi]
		mov		esi, [esi+4]
		test	edi, edi
		jz		iterHead
		mov		eax, [edi+8]
		mov		ecx, [edi+4]
		mov		dl, [ecx+4]
		cmp		dl, kFormType_TESObjectARMO
		jz		hasHealth
		cmp		dl, kFormType_TESObjectWEAP
		jnz		clrExtra
		cmp		byte ptr [ecx+0xF4], 0xA
		jb		hasHealth
	clrExtra:
		test	eax, eax
		jz		iterHead
		mov		dword ptr [eax+8], 0x3F800000
		mov		edx, [eax]
		test	edx, edx
		jnz		iterHead
		mov		[edi+8], edx
		push	eax
		call	Game_HeapFree
		jmp		iterHead
		ALIGN 16
	hasHealth:
		test	eax, eax
		jnz		hasExtra
		push	0xC
		call	Game_DoHeapAlloc
		mov		[edi+8], eax
		xor		edx, edx
		mov		[eax], edx
		mov		[eax+4], edx
		mov		edx, [esp+0xC]
		mov		[eax+8], edx
		jmp		iterHead
		ALIGN 16
	hasExtra:
		movss	xmm0, [esp+0xC]
		minss	xmm0, [eax+8]
		movss	[eax+8], xmm0
		jmp		iterHead
	done:
		pop		edi
		pop		esi
		retn	4
		ALIGN 4
	kMaxHealth:
		EMIT_DW(0x3F7FBE77)
	}
}

__declspec(naked) void TESGlobalLoadFormHook()
{
	__asm
	{
		mov		ecx, [ebp-0x10]
		test	byte ptr [ecx+8], 0x40
		jnz		skipLoad
		push	dword ptr [ebp+8]
		JMP_EAX(0x5A64DE)
	skipLoad:
		mov		al, 1
		leave
		retn	4
	}
}

__declspec(naked) void MarkCreatureNoFallHook()
{
	__asm
	{
		mov		edx, [ecx+0x20]
		test	edx, edx
		jz		done
		cmp		byte ptr [edx+4], kFormType_TESCreature
		jnz		skipFlag
		test	byte ptr [edx+0x36], 0x80
		jz		skipFlag
		mov		edx, [ebp-0x64]
		test	edx, edx
		jz		skipFlag
		or		byte ptr [edx+0x417], 0x40
	skipFlag:
		cmp		[ecx+0x1B0], 0
		jnz		done
		mov		edx, [eax+0x68]
		cmp		dword ptr [edx+0x434], 0
		jnz		done
		push	ecx
		mov		ecx, edx
		CALL_EAX(0x8E4E50)
	done:
		JMP_EAX(0x931431)
	}
}

__declspec(naked) bool DetectionTeammateHook()
{
	__asm
	{
		test	word ptr [ecx+0x105], kHookActorFlags_DetectionModified
		jz		getTeammate
		xor		al, al
		retn
	getTeammate:
		mov		al, [ecx+0x18D]
		retn
	}
}

__declspec(naked) UInt16 DetectionGetLevelHook()
{
	__asm
	{
		test	byte ptr [edx+0x105], kHookActorFlag1_DetectionFix
		jz		getLevel
		mov		ecx, g_thePlayer
		add		ecx, 0xA4
	getLevel:
		jmp		dword ptr [eax+0x28]
	}
}

__declspec(naked) bool WheelTeammateHook()
{
	__asm
	{
		mov		al, [ecx+0x18D]
		test	al, al
		jz		done
		test	byte ptr [ecx+0x105], kHookActorFlag1_DisableWheel
		setz	al
	done:
		retn
	}
}

FixedTypeArray<Script*, 0xF> s_messageBoxScripts;

__declspec(naked) float MaxMessageWidthHook()
{
	__asm
	{
		cmp		s_messageBoxScripts, 0
		jnz		overrideWidth
		JMP_EAX(ADDR_TileGetFloat)
	overrideWidth:
		fld		kMaxWidth
		retn	4
		ALIGN 4
	kMaxWidth:
		EMIT_DW(0x44960000)
	}
}

__declspec(naked) void InitBarterMenuHook()
{
	__asm
	{
		push	ecx
		push	dword ptr [ecx+0x48]
		CALL_EAX(0x7301F0)
		pop		ecx
		JMP_EAX(0x72DC30)
	}
}

__declspec(naked) void InitArmorFormHook()
{
	__asm
	{
		push	0xFFFFFFFF
		mov		ecx, [ebp-0xC]
		CALL_EAX(0x484E60)
		push	eax
		lea		eax, [esp+8]
		push	eax
		CALL_EAX(0x485D50)
		add		esp, 8
		push	dword ptr [esp+4]
		call	LookupFormByRefID
		retn
	}
}

__declspec(naked) UInt32 ToggleConsoleHook()
{
	__asm
	{
		xor		eax, eax
		test	byte ptr [ecx+0x1A21], 0x80
		jnz		done
		test	byte ptr [ecx+0x1921], 0x80
		setnz	al
	done:
		retn	8
	}
}

__declspec(naked) UInt32 GamePadRumbleHook()
{
	__asm
	{
		cmp		byte ptr ds:0x11E0854, 0
		jz		noRumble
		jmp		dword ptr ds:0xFDF398
	noRumble:
		xor		eax, eax
		retn	8
	}
}

bool s_doRefreshItemListBox = false;

__declspec(naked) void RemoveMeHook()
{
	__asm
	{
		mov		s_doRefreshItemListBox, 1
		xor		al, al
		leave
		retn
	}
}

__declspec(naked) bool __fastcall IsPersistentHook(TESForm *form)
{
	__asm
	{
		cmp		byte ptr [ecx+0xF], 0xFF
		jz		done
		JMP_EAX(0x5653D0)
	done:
		xor		al, al
		retn
	}
}

__declspec(naked) void PackageSetRunHook()
{
	__asm
	{
		mov		eax, [ebp-4]
		test	eax, eax
		jz		noPackage
		mov		eax, [eax+0x1C]
		test	eax, kPackageFlag_AlwaysWalk
		jz		allowRun
		mov		eax, 0x101
		leave
		retn	0x18
	allowRun:
		test	eax, kPackageFlag_AlwaysRun
		jnz		skipRetn
	noPackage:
		cmp		byte ptr [ecx+0x124], 0
		jnz		skipRetn
		cmp		byte ptr [ecx+0x104], 0
		jnz		skipRetn
		JMP_EAX(0x8DAAA9)
	skipRetn:
		mov		eax, 0x201
		leave
		retn	0x18
	}
}

__declspec(naked) void __fastcall FixVendorCaps(DataHandler *dataHandler, ExtraContainerChanges::Data *changesData)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	edx
		mov		ecx, [ecx+0x628]
		mov		ecx, [ecx]
		mov		edx, g_capsItem
		call	ContChangesEntryList::FindForItem
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		push	eax
		mov		ecx, [ebp-8]
		push	dword ptr [ecx+4]
		mov		ecx, [ecx]
		mov		edx, g_capsItem
		call	ContChangesEntryList::FindForItem
		test	eax, eax
		jz		done
		mov		edx, [eax+4]
		cmp		edx, 0x7FFF
		jle		done
		push	esi
		push	edi
		mov		esi, [ebp-0xC]
		mov		edi, edx
		ALIGN 16
	countHead:
		test	esi, esi
		jz		addHead
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		countHead
		push	kXData_ExtraOriginalReference
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		countHead
		mov		edx, [eax+0xC]
		cmp		[ebp-0x10], edx
		jnz		countHead
		push	kXData_ExtraCount
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		addHead
		mov		ecx, 0x7FFF
		cmp		edi, ecx
		cmovl	ecx, edi
		mov		[eax+0xC], ecx
		sub		edi, ecx
		ALIGN 16
	addHead:
		cmp		edi, 1
		jle		doPop
		call	ExtraDataList::Create
		mov		esi, eax
		push	dword ptr [ebp-0x10]
		call	ExtraOriginalReference::Create
		push	eax
		mov		ecx, esi
		CALL_EAX(ADDR_AddExtraData)
		mov		eax, 0x7FFF
		cmp		edi, eax
		cmovl	eax, edi
		sub		edi, eax
		push	eax
		mov		ecx, esi
		call	ExtraDataList::AddExtraCount
		push	esi
		mov		ecx, [ebp-0xC]
		call	ContChangesExtraList::Prepend
		jmp		addHead
	doPop:
		pop		edi
		pop		esi
	done:
		mov		ecx, [ebp-4]
		leave
		retn
	}
}

__declspec(naked) void BarterSellFixHook()
{
	__asm
	{
		mov		edx, [ebp-0x1C]
		push	0x46F640
		jmp		FixVendorCaps
	}
}

__declspec(naked) void JumpHeightFixHook()
{
	__asm
	{
		add		esp, 8
		mov		dword ptr [esi+0x3F0], 2
		mov		dword ptr [esi+0x520], 0xB
		movss	xmm0, [esi+0x540]
		mulss	xmm0, PS_HKUnitCnvrt
		addss	xmm0, [esp+0x28]
		movss	[esi+0x544], xmm0
		JMP_EAX(0xCD4486)
	}
}

__declspec(naked) void EffectSkillModifiers1Hook()
{
	__asm
	{
		mov		al, [ebp-0xA1]
		shl		al, 1
		add		al, [ebp-0xA3]
		test	al, al
		jz		done
		mov		ecx, [ebp-0xAC]
		mov		ecx, [ecx]
		cmp		dword ptr [ecx], 0
		jl		done
		mov		[ebp-0xDC], al
		test	al, 1
		jz		survival
		mov		ecx, g_thePlayer
		CALL_EAX(0x8BEBB0)
		jmp		doPerkMod
	survival:
		test	byte ptr [ebp-0xDC], 2
		jz		done
		cmp		dword ptr [ebp-0xBC], kAVCode_RadiationRads
		jz		done
		mov		ecx, g_thePlayer
		CALL_EAX(0x8BEC80)
	doPerkMod:
		fimul	dword ptr [ebp-0xB8]
		fstp	dword ptr [ebp-0xC0]
		cmp		dword ptr [ebp-0xBC], kAVCode_Health
		jnz		skipPerkMod
		lea		eax, [ebp-0xC0]
		push	eax
		push	g_thePlayer
		push	kPerkEntry_ModifyRecoveredHealth
		CALL_EAX(ADDR_ApplyPerkModifiers)
		add		esp, 0xC
	skipPerkMod:
		cvttss2si	eax, dword ptr [ebp-0xC0]
		mov		[ebp-0xB8], eax
	done:
		JMP_EAX(0x406A9D)
	}
}

__declspec(naked) void EffectSkillModifiers2Hook()
{
	__asm
	{
		mov		ecx, [ebp-0x60]
		test	ecx, ecx
		jz		done
		mov		al, [ecx+0xBC]
		test	al, 6
		jz		done
		mov		ecx, [ebp-0x74]
		mov		ecx, [ecx+0xC]
		mov		edx, [ecx+0x14]
		test	byte ptr [edx+0x58], 4
		jnz		done
		mov		edx, [ecx+0x10]
		mov		[ebp-0x38], edx
		mov		[ebp-0x40], al
		test	al, 4
		jz		survival
		mov		ecx, [ebp-0x28]
		CALL_EAX(0x8BEBB0)
		jmp		doPerkMod
	survival:
		test	byte ptr [ebp-0x40], 2
		jz		done
		cmp		dword ptr [ebp-0x38], kAVCode_RadiationRads
		jz		done
		mov		ecx, [ebp-0x28]
		CALL_EAX(0x8BEC80)
	doPerkMod:
		fmul	dword ptr [ebp+0x14]
		fstp	dword ptr [ebp-0x50]
		cmp		dword ptr [ebp-0x38], kAVCode_Health
		jnz		skipPerkMod
		mov		ecx, [ebp-0x28]
		cmp		dword ptr [ecx+0xC], 0x14
		jnz		skipPerkMod
		lea		eax, [ebp-0x50]
		push	eax
		push	ecx
		push	kPerkEntry_ModifyRecoveredHealth
		CALL_EAX(ADDR_ApplyPerkModifiers)
		add		esp, 0xC
	skipPerkMod:
		push	dword ptr [ebp-0x50]
		mov		ecx, [ebp-0x74]
		CALL_EAX(0x804BC0)
	done:
		JMP_EAX(0x816627)
	}
}

__declspec(naked) void RecipeMenuAmmoEffectsHook()
{
	__asm
	{
		mov		ecx, [ebp+8]
		cmp		byte ptr [ecx+4], kFormType_TESAmmo
		jnz		notAmmo
		push	0x100
		lea		eax, [ebp-0x338]
		push	eax
		push	0x729506
		JMP_EAX(0x503A70)
	notAmmo:
		mov		ecx, [ebp-0x6C]
		test	ecx, ecx
		jz		done
		cmp		dword ptr [ecx+4], 0
	done:
		JMP_EAX(0x7294F0)
	}
}

__declspec(naked) void QttMenuEnableWheelHook()
{
	__asm
	{
		mov		eax, g_interfaceManager
		cmp		dword ptr [eax+0x44], 0
		jz		done
		setg	dl
		inc		dl
		movzx	eax, dl
		push	0
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
	done:
		push	0xFC4
		JMP_EAX(0x7AC17E)
	}
}

__declspec(naked) bool __stdcall TerminalGetLockedHook(TESObjectREFR *terminalRef)
{
	__asm
	{
		push	ebx
		test	byte ptr [ecx+0xA5], 2
		setnz	bl
		mov		ecx, [esp+8]
		test	ecx, ecx
		jz		setRes
		push	kXData_ExtraTerminalState
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		setRes
		mov		cl, [eax+0xD]
		cmp		byte ptr [eax+0xC], 0x80
		setz	al
		jz		done
		cmp		cl, 0xFE
		setz	al
		jz		done
		cmp		cl, 0xFF
		jnz		done
	setRes:
		mov		al, bl
	done:
		pop		ebx
		retn	4
	}
}

TESWorldSpace *s_audioMarkerLastWorld = nullptr;
TempObject<Vector<MusicMarker>> s_musicMarkerList(0x100);

void __fastcall BuildMusicMarkerList(TESWorldSpace *worldspace, tList<TESObjectREFR>::Node *markerRefList)
{
	if (s_audioMarkerLastWorld != worldspace)
	{
		s_audioMarkerLastWorld = worldspace;
		s_musicMarkerList->Clear();
		for (; markerRefList; markerRefList = markerRefList->next)
			if (TESObjectREFR *markerRef = markerRefList->data)
				if (ExtraAudioMarker *xAudioMarker = GetExtraType(&markerRef->extraDataList, ExtraAudioMarker); xAudioMarker && xAudioMarker->data)
					ArrayUtils<Vector<MusicMarker>>::InsertSorted(s_musicMarkerList(), MusicMarker(markerRef, xAudioMarker->data));
	}
	for (auto mkIter = s_musicMarkerList->Begin(); mkIter; ++mkIter)
		g_thePlayer->musicMarkers.Prepend(new (Game_HeapAlloc<MusicMarker>()) MusicMarker(mkIter()));
}

__declspec(naked) void BuildMusicMarkerListHook()
{
	__asm
	{
		mov		edx, eax
		mov		ecx, [ebp-0x10]
		push	0x947E79
		jmp		BuildMusicMarkerList
	}
}

TESForm *s_altAshPile = nullptr;

__declspec(naked) void AttachAshPileHook()
{
	__asm
	{
		push	0x5DBD23
		mov		eax, s_altAshPile
		test	eax, eax
		jz		useDefault
		and		s_altAshPile, 0
		retn
	useDefault:
		mov		eax, g_gooPileACTI
		mov		ecx, g_ashPileACTI
		cmp     dword ptr [ebp-0x18], 2
		cmovnz	eax, ecx
		retn
	}
}

float s_windSpeedMult = 1 / 255.0F;

__declspec(naked) double __fastcall GetWindSpeedHook(TESWeather *weather, int, UInt32 speedType, float maxSpeed, float minSpeed)
{
	__asm
	{
		movzx	eax, byte ptr [ecx+0xE0]
		mov		[esp+4], eax
		fild	dword ptr [esp+4]
		fmul	s_windSpeedMult
		retn	0xC
	}
}

__declspec(naked) void __fastcall ClearExtraHotkeyIfUsed(ExtraDataList *inXDataList)
{
	__asm
	{
		push	kXData_ExtraHotkey
		call	BaseExtraList::GetByType
		test	eax, eax
		jnz		isBound
		retn
		ALIGN 16
	isBound:
		push	ebx
		push	esi
		push	edi
		push	ecx
		mov		bl, [eax+0xC]
		mov		ecx, g_thePlayer
		call	TESObjectREFR::GetContainerChangesList
		mov		esi, eax
		ALIGN 16
	entryIter:
		test	esi, esi
		jz		done
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		entryIter
		cmp		dword ptr [ecx], 0
		jz		entryIter
		mov		eax, [ecx+8]
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESAmmo
		jz		entryIter
		cmp		dl, kFormType_TESObjectMISC
		jz		entryIter
		cmp		dl, kFormType_AlchemyItem
		ja		entryIter
		mov		edi, [ecx]
		ALIGN 16
	xdlIter:
		test	edi, edi
		jz		entryIter
		mov		ecx, [edi]
		mov		edi, [edi+4]
		test	ecx, ecx
		jz		xdlIter
		push	kXData_ExtraHotkey
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		xdlIter
		cmp		[eax+0xC], bl
		jnz		xdlIter
		pop		ecx
		push	kXData_ExtraHotkey
		CALL_EAX(ADDR_RemoveExtraType)
		pop		edi
		pop		esi
		pop		ebx
		retn
		ALIGN 16
	done:
		pop		ecx
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) void HotkeyFixRemoveItemHook()
{
	__asm
	{
		cmp		byte ptr [ebp+0x1C], 0
		jnz		done
		mov		ecx, [ebp+0x18]
		test	ecx, ecx
		jz		done
		mov		eax, [ebp+0x20]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0xC], 0x14
		jnz		done
		call	ClearExtraHotkeyIfUsed
	done:
		JMP_EAX(0x4C39B0)
	}
}

__declspec(naked) void HotkeyFixPlayerActivateHook()
{
	__asm
	{
		mov		eax, [ecx+0x20]
		movzx	edx, byte ptr [eax+4]
		mov		[ebp-0x38], edx
		add		ecx, 0x44
		call	ClearExtraHotkeyIfUsed
		JMP_EAX(0x95401E)
	}
}

__declspec(naked) void SetScaleHook()
{
	__asm
	{
		movss	xmm0, [ebp+8]
		maxss	xmm0, kMinScale
		minss	xmm0, SS_100
		movss	[ebp+8], xmm0
		JMP_EAX(0x567554)
		ALIGN 4
	kMinScale:
		EMIT_DW(0x38D1B717)
	}
}

__declspec(naked) UInt8 __fastcall GetEntryDataModFlagsHook(ContChangesEntry *entry)
{
	__asm
	{
		test	ecx, ecx
		jz		retn0
		mov		eax, [ecx+8]
		cmp		byte ptr [eax+4], kFormType_TESObjectWEAP
		jnz		retn0
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		push	kXData_ExtraItemDropper
		mov		ecx, eax
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		noDropper
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		noDropper
		lea		ecx, [eax+0x44]
	noDropper:
		push	kXData_ExtraWeaponModFlags
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		al, [eax+0xC]
	done:
		retn
	retn0:
		xor		al, al
		retn
	}
}

__declspec(naked) bool __fastcall GetEntryDataHasModHook(ContChangesEntry *entry, int, UInt8 modType)
{
	__asm
	{
		push	ecx
		call	GetEntryDataModFlagsHook
		pop		ecx
		test	al, al
		jz		done
		mov		ecx, [ecx+8]
		mov		dl, [esp+4]
		test	al, 1
		jz		check2nd
		cmp		[ecx+0x180], dl
		jnz		check2nd
		mov		al, 1
		retn	4
		ALIGN 16
	check2nd:
		test	al, 2
		jz		check3rd
		cmp		[ecx+0x184], dl
		jnz		check3rd
		mov		al, 1
		retn	4
		ALIGN 16
	check3rd:
		test	al, 4
		jz		retn0
		cmp		[ecx+0x188], dl
		setz	al
		retn	4
		ALIGN 16
	retn0:
		xor		al, al
	done:
		retn	4
	}
}

__declspec(naked) void RolloverWeaponHook()
{
	__asm
	{
		mov		eax, [ebp-0x9C]
		test	byte ptr [eax+0x100], 0x80
		setz	al
		jnz		done
		push	kXData_ExtraWeaponModFlags
		mov		ecx, [ebp+8]
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		retn1
		cmp		byte ptr [eax+0xC], 0
		jz		retn1
		mov		ecx, 0x11D9C48
		call	StrLen
		mov		word ptr [eax+ecx], '+'
	retn1:
		mov		al, 1
	done:
		JMP_EDX(0x77712E)
	}
}

__declspec(naked) void __fastcall ClearWeaponNodeHook(NiNode *wpnNode)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, ecx
		mov		esi, [ecx+0xA0]
		movzx	edi, word ptr [ecx+0xA6]
		ALIGN 16
	iterHead:
		dec		edi
		js		done
		mov		ecx, [esi+edi*4]
		test	ecx, ecx
		jz		iterHead
		test	byte ptr [ecx+0x33], 0x80
		jnz		iterHead
		push	ecx
		mov		ecx, ebx
		mov		eax, [ecx]
		call	dword ptr [eax+0xE8]
		jmp		iterHead
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) void ConstructItemEntryNameHook()
{
	__asm
	{
		push	ebx
		xor		ebx, ebx
		mov		[ebp-8], eax
		mov		ecx, [ebp+0xC]
		call	GetEntryDataModFlagsHook
		test	al, al
		jz		noModFlags
		mov		bl, '+'
		jmp		getName
	noModFlags:
		cmp		dword ptr [ebp-8], 1
		jle		done
	getName:
		mov		eax, [ebp+0xC]
		mov		eax, [eax+8]
		movzx	ecx, word ptr [eax+0x38]
		test	ecx, ecx
		jz		done
		push	esi
		push	edi
		mov		esi, [eax+0x34]
		lea		edi, [ebp-0x88]
		rep movsb
		mov		eax, edi
		pop		edi
		pop		esi
		mov		[eax], ebx
		and		bl, 1
		add		eax, ebx
		mov		edx, [ebp-8]
		cmp		edx, 1
		jle		setUIStr
		mov		word ptr [eax], '( '
		lea		ecx, [eax+2]
		call	IntToStr
		mov		word ptr [eax], ')'
	setUIStr:
		lea		edx, [ebp-0x88]
		push	edx
		mov		edx, kTileValue_string
		mov		ecx, [ebp+8]
		call	Tile::SetString
	done:
		pop		ebx
		retn
	}
}

__declspec(naked) const char* __cdecl GetIconPathForItemHook(TESForm *item, TESObjectREFR *owner)
{
	__asm
	{
		xor		eax, eax
		mov		ecx, [esp+4]
		cmp		byte ptr [ecx+4], kFormType_TESObjectARMO
		jz		isARMO
		retn
	isARMO:
		add		ecx, 0xFC
		mov		edx, g_thePlayer
		mov		edx, [edx+0x20]
		test	byte ptr [edx+0x34], 1
		jz		notFem
		cmp		word ptr [ecx+0x14], 0
		jz		notFem
		mov		eax, [ecx+0x10]
		retn
	notFem:
		mov		eax, [ecx+4]
		retn
	}
}

float s_condDmgPenalty = 0.67F;

__declspec(naked) float __cdecl GetConditionDamagePenaltyHook(float healthPercent)
{
	__asm
	{
		movss	xmm0, PS_V3_One
		movss	xmm1, [esp+4]
		movss	xmm2, kPenaltyThreshold
		comiss	xmm1, xmm2
		jnb		done
		subss	xmm2, xmm1
		mulss	xmm2, s_condDmgPenalty
		movq	xmm1, xmm0
		subss	xmm0, xmm2
		xorps	xmm2, xmm2
		minss	xmm0, xmm1
		maxss	xmm0, xmm2
	done:
		movss	[esp+4], xmm0
		fld		dword ptr [esp+4]
		retn
		ALIGN 4
	kPenaltyThreshold:
		EMIT_DW(0x3F400000)
	}
}

float s_condDRDTPenalty = 1.0F;

__declspec(naked) float __cdecl GetArmorConditionPenaltyHook(UInt16 baseDRDT, float healthPercent)
{
	__asm
	{
		movzx	eax, word ptr [esp+4]
		cvtsi2ss	xmm0, eax
		movss	xmm1, [esp+8]
		movss	xmm2, PS_V3_Half
		comiss	xmm1, xmm2
		jnb		done
		subss	xmm2, xmm1
		mulss	xmm2, s_condDRDTPenalty
		movss	xmm1, PS_V3_One
		movq	xmm3, xmm1
		subss	xmm1, xmm2
		xorps	xmm2, xmm2
		minss	xmm1, xmm3
		maxss	xmm1, xmm2
		mulss	xmm0, xmm1
	done:
		movss	[esp+8], xmm0
		fld		dword ptr [esp+8]
		retn
	}
}

__declspec(naked) bool __fastcall GetEffectHiddenHook(EffectItem *effItem)
{
	__asm
	{
		mov		ecx, [ecx+0x14]
		test	ecx, ecx
		jz		retnTrue
		cmp		ecx, ds:0x11C33F4
		jz		retnTrue
		test	byte ptr [ecx+0x58], 8
		setnz	al
		retn
	retnTrue:
		mov		al, 1
		retn
	}
}

bool s_mapMenuSkipSetXY = false;

__declspec(naked) void RefreshMapMenuHook()
{
	__asm
	{
		cmp		s_mapMenuSkipSetXY, 0
		jnz		skipRetn
		push	0
		push	kTileValue_width
		mov		ecx, [ebp-0xD4]
		mov		eax, [ecx+0x84]
		mov		ecx, [eax+0x28]
		CALL_EAX(ADDR_TileGetFloat)
		fmul	dword ptr PS_V3_Half
		JMP_EAX(0x79DF98)
	skipRetn:
		mov		s_mapMenuSkipSetXY, 0
		JMP_EAX(0x79E053)
	}
}

__declspec(naked) void ProcessGradualSetFloatHook()
{
	__asm
	{
		call	GetTickCount
		mov		ecx, [ebp-0x20]
		mov		ecx, [ecx]
		mov		edx, eax
		sub		eax, [ecx+8]
		cvtsi2ss	xmm0, eax
		divss	xmm0, [ecx+0xC]
		comiss	xmm0, PS_V3_One
		jnb		finished
		movss	xmm1, [ecx+4]
		subss	xmm1, [ecx]
		mulss	xmm0, xmm1
		addss	xmm0, [ecx]
		mov		edx, [ecx+0x10]
		mov		ecx, [ecx+0x14]
		push	0xA08155
		jmp		Tile::SetFloat
	finished:
		mov		[ecx+8], edx
		movss	xmm0, [ecx]
		mov		edx, [ecx+0x10]
		mov		ecx, [ecx+0x14]
		call	Tile::SetFloat
		cmp		dword ptr [ebp-0xD8], 5
		jz		done
		push	dword ptr [ebp-0x20]
		lea		ecx, [ebp-0x18]
		CALL_EAX(0xA0C7C0)
	done:
		JMP_EAX(0xA08155)
	}
}

__declspec(naked) void __fastcall EndWeatherTransition(Sky *sky)
{
	__asm
	{
		and		dword ptr [ecx+0x14], 0
		retn
	}
}

__declspec(naked) void CloudsFixHook()
{
	__asm
	{
		mov		ecx, [ebp-0x44]
		and		dword ptr [ecx+0x34], 0
		push	dword ptr [ecx+0x68]
		push	EndWeatherTransition
		call	MainLoopAddCallback
		add		esp, 8
		JMP_EAX(0x454580)
	}
}

__declspec(naked) bool ReactionCooldownCheckHook()
{
	static UInt32 lastTickCount = 0;
	__asm
	{
		CALL_EAX(0x457FE0)
		mov		edx, eax
		sub		eax, lastTickCount
		cvtsi2ss	xmm0, eax
		mulss	xmm0, SS_1d1K
		comiss	xmm0, ds:0x11CE9B8
		seta	al
		jbe		done
		mov		lastTickCount, edx
	done:
		retn
	}
}

__declspec(naked) void IsValidAITargetHook()
{
	__asm
	{
		push	0x9F50E7
		test	dword ptr [ecx+8], 0x4820
		setnz	al
		jnz		done
		jmp		TESObjectREFR::GetDisabled
	done:
		retn
	}
}

__declspec(naked) TESModelTextureSwap *TESObjectWEAP::GetWeaponModel(UInt32 modFlags, Actor *actor) const
{
	__asm
	{
		mov		eax, [esp+4]
		and		eax, 7
		mov		edx, kModelByMod[eax*4]
		cmp		word ptr [ecx+edx+8], 0
		setnz	dl
		neg		dl
		and		al, dl
		mov		edx, [esp+8]
		cmp		dword ptr [edx+0xC], 0x14
		jz		get1stP
		test	al, al
		jz		get3rdP
		cmp		byte ptr [ecx+0xF4], 8
		jnz		get3rdP
	get1stP:
		mov		edx, kModelByMod[eax*4+0x20]
		mov		edx, [ecx+edx]
		test	edx, edx
		jz		get3rdP
		lea		eax, [edx+0x30]
		retn	8
	get3rdP:
		mov		edx, kModelByMod[eax*4]
		lea		eax, [ecx+edx]
		retn	8
		ALIGN 16
	kModelByMod:
		EMIT_DW_4(0x0000003C, 0x00000270, 0x00000290, 0x000002D0)
		EMIT_DW_4(0x000002B0, 0x000002F0, 0x00000310, 0x00000330)
		EMIT_DW_4(0x00000250, 0x00000254, 0x00000258, 0x00000260)
		EMIT_DW_4(0x0000025C, 0x00000268, 0x00000264, 0x0000026C)
	}
}

__declspec(naked) void __fastcall SetWeaponSlotHook(BipedAnim *vbp01Names, int, TESObjectWEAP *weapon, UInt8 modFlags)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		eax, [esp+8]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+4], kFormType_TESObjectWEAP
		jnz		done
		cmp		[esi+0x7C], eax
		jnz		proceed
		cmp		dword ptr [esi+0x84], 0
		jnz		done
	proceed:
		push	0
		push	1
		push	5
		CALL_EAX(0x4AAFF0)
		mov		ecx, [esp+8]
		mov		[esi+0x7C], ecx
		push	dword ptr [esi+0x2B0]
		push	dword ptr [esp+0x10]
		call	TESObjectWEAP::GetWeaponModel
		mov		[esi+0x80], eax
	done:
		pop		esi
		retn	8
	}
}

// Credits to carxt for tracing the weapon backpack bug
__declspec(naked) void InitWornObjectHook()
{
	__asm
	{
		xor		al, al
		mov		ecx, [ebp+8]
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		done
		cmp		byte ptr [ecx+0x28], 1
		ja		done
		mov		ecx, [ecx+0x114]
		test	ecx, ecx
		jz		done
		call	GetEntryDataModFlagsHook
	done:
		push	eax
		push	dword ptr [ebp+0x10]
		mov		ecx, [ebp+0xC]
		call	SetWeaponSlotHook
		JMP_EAX(0x60622F)
	}
}

__declspec(naked) void PickWeaponModelHook()
{
	__asm
	{
		and		dword ptr [ebp-0x1C], 0
		mov		al, [ebp-9]
		test	al, al
		jnz		doCall
		mov		ecx, [ebp-4]
		test	ecx, ecx
		jz		doCall
		cmp		dword ptr [ecx+0xC], 0x14
		jz		doCall
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		doCall
		cmp		byte ptr [ecx+0x28], 1
		ja		doCall
		mov		ecx, [ecx+0x114]
		test	ecx, ecx
		jz		doCall
		call	GetEntryDataModFlagsHook
	doCall:
		push	0x571A27
		mov		ecx, [ebp-8]
		test	ecx, ecx
		jz		noBip01
		push	eax
		push	dword ptr [ebp+8]
		CALL_EAX(0x4AB750)
		retn
	noBip01:
		push	dword ptr [ebp-0x30]
		push	eax
		mov		ecx, [ebp+8]
		call	TESObjectWEAP::GetWeaponModel
		mov		[ebp-0x1C], eax
		retn
	}
}

__declspec(naked) void PlayAttackSoundHook()
{
	__asm
	{
		mov		ecx, [ebp+0xC]
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		done
		mov		dword ptr [ebp-0x7C], 0x200000
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		done
		cmp		byte ptr [ecx+0x28], 1
		ja		done
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		done
		mov		edx, [eax+8]
		push	edx
		push	kXData_ExtraWeaponModFlags
		mov		ecx, [eax]
		mov		ecx, [ecx]
		call	BaseExtraList::GetByType
		pop		edx
		test	eax, eax
		jz		done
		mov		al, [eax+0xC]
		test	al, 1
		jz		check2nd
		mov		cl, [edx+0x180]
		cmp		cl, 0xB
		jz		hasMod
		cmp		cl, 0x10
		jz		hasMod
	check2nd:
		test	al, 2
		jz		check3rd
		mov		cl, [edx+0x184]
		cmp		cl, 0xB
		jz		hasMod
		cmp		cl, 0x10
		jz		hasMod
	check3rd:
		test	al, 4
		jz		done
		mov		cl, [edx+0x188]
		cmp		cl, 0xB
		jz		hasMod
		cmp		cl, 0x10
		jnz		done
	hasMod:
		mov		[ebp-0x7D], 1
	done:
		JMP_EAX(0x83AE4F)
	}
}

const char *g_attachLightString = nullptr;

__declspec(naked) void __fastcall AddProjectileLightHook(Projectile *projRef)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		test	dword ptr [ecx+8], 0x820
		jnz		done
		mov		edx, [ecx+0x20]
		mov		edx, [edx+0x70]
		test	edx, edx
		jz		done
		mov		ecx, [ecx+0x64]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+0x14]
		test	ecx, ecx
		jz		done
		push	edx
		push	ecx
		mov		edx, g_attachLightString
		call	NiNode::GetBlockByName
		pop		ecx
		test	eax, eax
		cmovz	eax, ecx
		jz		gotNode
		mov		edx, [eax]
		cmp		dword ptr [edx+0xC], ADDR_ReturnThis
		cmovnz	eax, ecx
	gotNode:
		mov		edx, eax
		pop		ecx
		call	CreatePointLight
		mov		[esi+0x114], eax
	done:
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall RemoveProjectileLightHook(Projectile *projRef)
{
	__asm
	{
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		done
		and		dword ptr [ecx+0x114], 0
		mov		ecx, [eax+0x18]
		test	ecx, ecx
		jz		done
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xE8]
	done:
		retn
	}
}

__declspec(naked) void __fastcall AddExplosionLightHook(Explosion *explosion)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		eax, [ecx+0x20]
		mov		eax, [eax+0x80]
		test	eax, eax
		jz		done
		push	eax
		//call	GetAttachParentNode
		pop		ecx
		test	eax, eax
		jz		done
		mov		edx, eax
		call	CreatePointLight
		mov		[esi+0xC4], eax
	done:
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall RemoveExplosionLightHook(Explosion *explosion)
{
	__asm
	{
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		and		dword ptr [ecx+0xC4], 0
		mov		ecx, [eax+0x18]
		test	ecx, ecx
		jz		done
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xE8]
	done:
		retn
	}
}

__declspec(naked) void SetMuzzleFlashFadeHook()
{
	__asm
	{
		mov		ecx, [ebp-8]
		mov		edx, [ecx+0x14]
		mov		eax, [edx+0x74]
		mov		edx, [eax+0xB4]
		movzx	eax, byte ptr [ecx]
		neg		eax
		and		edx, eax
		mov		eax, [ecx+0x10]
		mov		[eax+0xC4], edx
		leave
		retn
	}
}

__declspec(naked) void __fastcall SetExplosionLightFadeHook(Explosion *explRef)
{
	__asm
	{
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		movss	xmm1, [ecx+0x88]
		addss	xmm1, xmm1
		divss	xmm1, [ecx+0x8C]
		movss	xmm2, PS_V3_One
		movq	xmm0, xmm2
		subss	xmm0, xmm1
		minss	xmm0, xmm2
		xorps	xmm2, xmm2
		maxss	xmm0, xmm2
		mov		edx, [ecx+0x20]
		mov		ecx, [edx+0x80]
		mulss	xmm0, [ecx+0xB4]
		movss	[eax+0xC4], xmm0
	done:
		retn
	}
}

__declspec(naked) bool __fastcall KillChallengeFixHook(Actor *killed)
{
	__asm
	{
		cmp		byte ptr [ecx+0x18D], 0
		jnz		done
		test	byte ptr [ecx+0x143], 0x80
		jnz		done
		call	Actor::GetActorBase
		test	byte ptr [eax+0x34], 2
	done:
		setnz   al
		retn
	}
}

__declspec(naked) void DoHitArmorDamageHook()
{
	__asm
	{
		push	0
		mov		ecx, [eax+8]
		cvtsi2ss	xmm0, [ecx+0x6C]
		mulss	xmm0, s_serializedVars.dmgArmorMaxPercent
		mov		ecx, [ebx+8]
		minss	xmm0, [ecx+0x28]
		push	ecx
		movss	[esp], xmm0
		push	eax
		mov		ecx, [ebp-0x18]
		CALL_EAX(0x9508B0)
		push	1
		mov		ecx, [ebp-0x598]
		push	0x89D362
		JMP_EAX(0x4459E0)
	}
}

__declspec(naked) bool __fastcall IsDisposableWeaponHook(TESObjectWEAP *weapon)
{
	__asm
	{
		cmp		byte ptr [ecx+0xF4], 9
		setg	al
		retn
	}
}

__declspec(naked) Actor* __fastcall GetNearestLivingAlly(Actor *actor)
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebp, ecx
		xor		ebx, ebx
		mov		esi, [ecx+0x130]
		test	esi, esi
		jz		done
		mov		edi, [esi+8]
		mov		esi, [esi+4]
		movss	xmm4, kMaxDist
		ALIGN 16
	iterHead:
		dec		edi
		js		done
		mov		eax, [esi]
		add		esi, 4
		test	eax, eax
		jz		iterHead
		cmp		eax, ebp
		jz		iterHead
		mov		ecx, [eax+0x68]
		test	ecx, ecx
		jz		iterHead
		mov		dl, [eax+0x108]
		cmp		dl, 1
		jz		iterHead
		cmp		dl, 2
		jz		iterHead
		cmp		byte ptr [eax+0x7D], 0
		jnz		iterHead
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x4B8]
		test	al, al
		jnz		iterHead
		mov		eax, [esi-4]
		lea		edx, [eax+0x30]
		lea		ecx, [ebp+0x30]
		call	Point3Distance
		comiss	xmm0, xmm4
		jnb		iterHead
		movq	xmm4, xmm0
		mov		ebx, eax
		jmp		iterHead
	done:
		mov		eax, ebx
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
		ALIGN 4
	kMaxDist:
		EMIT_DW(0x44800000)
	}
}

__declspec(naked) void DeathResponseFixHook()
{
	__asm
	{
		mov		dl, 1
		cmp		dword ptr [ebp+0x1C], 0
		jnz		done
		cmp		dword ptr [ebp+0x10], 2
		jnz		done
		cmp		dword ptr [ebp+0x14], 6
		jnz		done
		cmp		byte ptr [ebp+0x18], 1
		jnz		done
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		done
		call	GetNearestLivingAlly
		test	eax, eax
		setz	dl
		jz		done
		mov		[ebp-0x14], eax
	done:
		movzx	edx, dl
		push	edx
		push	dword ptr [ebp-0x1C]
		push	edx
		push	edx
		push	0
		push	0
		push	0
		push	dword ptr [ebp-0x14]
		push	0x9841C1
		mov		ecx, [ebp-0x10]
		JMP_EAX(0x8B2170)
	}
}

__declspec(naked) void ClearAshPilesHook()
{
	__asm
	{
		test	byte ptr [ecx+6], kHookFormFlag6_IsAshPile
		jz		done
		push	kXData_ExtraAshPileRef
		mov		ecx, [ebp-0x2C]
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		doUnload
		mov		[ebp-0x20], eax
		mov		ecx, [eax+0xC]
		test	ecx, ecx
		jz		noRefr
		mov		edx, [ecx+0x20]
		test	byte ptr [edx+9], 4
		jnz		done
		push	kXData_ExtraAshPileRef
		add		ecx, 0x44
		CALL_EAX(ADDR_RemoveExtraType)
	noRefr:
		push	kXData_ExtraAshPileRef
		mov		ecx, [ebp-0x2C]
		add		ecx, 0x44
		CALL_EAX(ADDR_RemoveExtraType)
	doUnload:
		mov		ecx, [ebp-0x2C]
		CALL_EAX(0x572270)
	done:
		JMP_EAX(0x54E5E6)
	}
}

__declspec(naked) void SetAcousticSpaceFixHook()
{
	__asm
	{
		mov		byte ptr ds:0x11DCFB1, 0
		test	eax, eax
		jz		done
		and		dword ptr ds:0x11DCFB8, 0
		cmp		eax, ds:0x11DCFB4
		jz		done
		push	eax
		CALL_EAX(0x82EAE0)
		pop		ecx
	done:
		JMP_EAX(0x82D8F4)
	}
}

__declspec(naked) void DamagePCFatigueMinFixHook()
{
	__asm
	{
		push	dword ptr [ebp+8]
		mov		ecx, [ebp-0x38]
		add		ecx, 0xA4
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		fstp	dword ptr [ebp-0x18]
		cmp		dword ptr [ebp+8], kAVCode_Fatigue
		jnz		done
		movss	xmm0, [ebp+0xC]
		movss	xmm1, [ebp-0x18]
		addss	xmm1, xmm0
		subss	xmm1, ds:0x11D2668	//	fMinFatigue
		xorps	xmm2, xmm2
		minss	xmm1, xmm2
		subss	xmm0, xmm1
		movss	[ebp+0xC], xmm0
	done:
		JMP_EAX(0x93B8AA)
	}
}

__declspec(naked) void UpdateWaterTexturesHook()
{
	__asm
	{
		mov		al, ds:0x11F91C5
		test	al, al
		jnz		done
		jmp		NiTPtrMap<UInt32>::Lookup
	done:
		retn	4
	}
}

__declspec(naked) UInt32 __fastcall EffectItemGetModifiedDuration(EffectItem *effItem)
{
	__asm
	{
		xor		eax, eax
		mov		edx, [ecx+0x14]
		test	byte ptr [edx+0x58], 0x80
		jnz		done
		mov		eax, [ecx+8]
		test	byte ptr [edx+0x58], 1
		jnz		done
		mov		ecx, [ebp]
		mov		ecx, [ecx-0xD4]
		cmp		dword ptr [ecx], 0x1011900
		jnz		done
		mov		dl, [ecx+0x64]
		sub		dl, 0xA
		cmp		dl, 1
		ja		done
		cvtsi2ss	xmm0, eax
		push	ecx
		movss	[esp], xmm0
		push	esp
		push	g_thePlayer
		push	kPerkEntry_ModifyPositiveChemDuration
		CALL_EAX(0x5E58F0)
		add		esp, 0xC
		cvttss2si	eax, [esp]
		pop		ecx
	done:
		retn
	}
}

__declspec(naked) void ModPositiveChemDurationHook()
{
	__asm
	{
		mov		al, [eax+0xA0]
		sub		al, 0xA
		cmp		al, 1
		seta	al
		ja		done
		mov		ecx, [ebp+0x10]
		mov		eax, [ecx+0xC]
		mov		ecx, [eax+0x14]
		mov		al, [ecx+0x58]
		and		al, 1
	done:
		JMP_EDX(0x82370C)
	}
}

__declspec(naked) bool __cdecl PickSoundFileFromFolderHook(char *outFilePath)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x100
		push	esi
		push	edi
		mov		edx, [ebp+8]
		lea		ecx, [ebp-0x100]
		call	StrCopy
		mov		edi, eax
		mov		word ptr [eax], '.*'
		mov		dword ptr [eax+2], '\0vaw'
		cmp		byte ptr ds:0x11ACDF9, 0
		jnz		invalidate
		push	8
		push	dword ptr [ebp+8]
		lea		ecx, [ebp-0x100]
		push	ecx
		CALL_EAX(0xAF7400)
		add		esp, 0xC
		jmp		done
	invalidate:
		push	0
		push	8
		push	dword ptr [ebp+8]
		lea		ecx, [ebp-0x100]
		push	ecx
		CALL_EAX(0xAFE420)
		add		esp, 0x10
		mov		esi, eax
		mov		dword ptr [edi+1], 'ggo.'
		push	0
		push	8
		push	dword ptr [ebp+8]
		lea		ecx, [ebp-0x100]
		push	ecx
		CALL_EAX(0xAFE420)
		add		esp, 0x10
		test	esi, esi
		jnz		hasWAVs
		test	eax, eax
		jz		done
		mov		esi, eax
		jmp		doneLists
	hasWAVs:
		test	eax, eax
		jz		doneLists
		mov		ecx, esi
		ALIGN 16
	appendIter:
		mov		edx, [ecx+4]
		test	edx, edx
		jz		appendEnd
		mov		ecx, edx
		jmp		appendIter
	appendEnd:
		mov		[ecx+4], eax
	doneLists:
		xor		edi, edi
		mov		ecx, esi
		ALIGN 16
	countIter:
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jz		countEnd
		inc		edi
		jmp		countIter
	countEnd:
		test	edi, edi
		jz		clearIter
		inc		edi
		push	edi
		mov		ecx, GAME_RNG
		CALL_EAX(ADDR_GetRandomInt)
		mov		edi, eax
		ALIGN 16
	clearIter:
		test	edi, edi
		jnz		skipCopy
		mov		edx, [esi]
		mov		ecx, [ebp+8]
		call	StrCopy
	skipCopy:
		dec		edi
		push	esi
		push	dword ptr [esi]
		mov		esi, [esi+4]
		call	Game_HeapFree
		call	Game_HeapFree
		test	esi, esi
		jnz		clearIter
		mov		al, 1
	done:
		pop		edi
		pop		esi
		leave
		retn
	}
}

__declspec(naked) float __vectorcall GetFrequencyModifier(TESSound *soundForm)
{
	__asm
	{
		movss	xmm0, PS_V3_One
		test	byte ptr [ecx+0x48], 1
		jnz		done
		movsx	edx, byte ptr [ecx+0x46]
		test	edx, edx
		jz		done
		cvtsi2ss	xmm1, edx
		shr		edx, 0x1F
		mulss	xmm1, kFreqMult[edx*4]
		addss	xmm0, xmm1
	done:
		retn
		ALIGN 4
	kFreqMult:
		EMIT_DW(0x3C23D70A) EMIT_DW(0x3BA3D70A)
	}
}

__declspec(naked) void SetSourceSoundRequestHook()
{
	__asm
	{
		mov		ecx, [ebp-0xCF0]
		mov		eax, [ebp+8]
		mov		eax, [eax+4]
		xor		edx, edx
		div		dword ptr [ecx+0x58]
		mov		eax, [ecx+0x5C]
		mov		eax, [eax+edx*4]
		test	eax, eax
		jz		done
		mov		ecx, [ebp+8]
		mov		edx, [ecx+4]
		ALIGN 16
	iterHead:
		cmp		[eax+4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		JMP_EAX(0xAE5527)
	found:
		mov		eax, [eax+8]
		mov		ecx, [ecx+8]
		mov		[eax+0x134], ecx
		call	GetFrequencyModifier
		movss	[eax+0x138], xmm0
	done:
		JMP_EAX(0xAE5527)
	}
}

__declspec(naked) void FillGameSoundPropsHook()
{
	__asm
	{
		and		dword ptr [ebp-0x10], 0
		mov		ecx, [ebp+0xC]
		mov		[ebp-0xC], ecx
		mov		eax, [ebp+8]
		mov		eax, [eax]
		test	eax, eax
		js		done
		push	0
		push	0
		push	0
		push	0
		push	0
		push	ecx
		push	eax
		push	0x36
		mov		ecx, 0x11F6EF0
		CALL_EAX(0xADB1A0)
		mov		ecx, [ebp-0xC]
	done:
		JMP_EAX(0x82D426)
	}
}

__declspec(naked) UInt32 __fastcall AdjustSoundFrequencyHook(BSGameSound *gameSound, int, float freq)
{
	__asm
	{
		cmp		dword ptr [ecx+0x19C], 0
		jz		done
		cmp		dword ptr [ecx+0x198], 0
		jz		done
		movzx	edx, word ptr [ecx+0x15A]
		cvtsi2ss	xmm0, edx
		mulss	xmm0, [ecx+0x138]
		mulss	xmm0, [esp+4]
		maxss	xmm0, SS_100
		minss	xmm0, kFlt200K
		cvtss2si	edx, xmm0
		push	edx
		mov		ecx, [ecx+0x19C]
		push	ecx
		mov		ecx, [ecx]
		call	dword ptr [ecx+0x44]
	done:
		xor		eax, eax
		retn	4
		ALIGN 4
	kFlt200K:
		EMIT_DW(0x48435000)
	}
}

__declspec(naked) float __fastcall GetSoundFrequencyPercHook(BSGameSound *gameSound)
{
	__asm
	{
		push	ecx
		cmp		dword ptr [ecx+0x19C], 0
		jz		retn1
		cmp		dword ptr [ecx+0x198], 0
		jz		retn1
		push	0
		push	esp
		mov		ecx, [ecx+0x19C]
		push	ecx
		mov		ecx, [ecx]
		call	dword ptr [ecx+0x20]
		pop		edx
		test	eax, eax
		js		retn1
		cvtsi2ss	xmm0, edx
		mov		ecx, [esp]
		movzx	edx, word ptr [ecx+0x15A]
		cvtsi2ss	xmm1, edx
		mulss	xmm1, [ecx+0x138]
		divss	xmm0, xmm1
		movss	[esp], xmm0
		fld		dword ptr [esp]
		pop		ecx
		retn
	retn1:
		fld1
		pop		ecx
		retn
	}
}

__declspec(naked) BGSImpactData* __fastcall GetImpactDataHook(BGSImpactDataSet *impDataSet, int, UInt32 materialType)
{
	__asm
	{
		mov		edx, [esp+4]
		mov		eax, [ecx+edx*4+0x1C]
		test	eax, eax
		jz		done
		mov		ecx, ds:0x11CA834
		test	ecx, ecx
		jz		done
		push	eax
		push	eax
		push	edx
		CALL_EAX(0x58F6F0)
		pop		ecx
		test	eax, eax
		cmovz	eax, ecx
	done:
		retn	4
	}
}

__declspec(naked) UInt32 __fastcall GetFactionReactionHook(TESFaction *faction, int, TESFaction *toFaction)
{
	__asm
	{
		add		ecx, 0x24
		JMP_EAX(0x48C1B0)
	}
}

__declspec(naked) void __fastcall MarkRefAsModifiedHook(TESObjectREFR *refr, int, UInt32 flag)
{
	__asm
	{
		mov		eax, [esp+4]
		cmp		eax, 4
		jnz		proceed
		mov		edx, [ecx+0x40]
		test	edx, edx
		jz		proceed
		test	byte ptr [edx+0xA], 0x40
		jz		proceed
		retn	4
	proceed:
		MARK_MODIFIED(ecx, eax)
		retn	4
	}
}

PatchInstallState s_patchInstallState;

__declspec(naked) float __cdecl GetDamageToWeaponHook(Actor *actor)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		ecx, [eax+0x68]
		mov		edx, 0x11CF154
		test	ecx, ecx
		jz		noWeapon
		cmp		byte ptr [ecx+0x28], 1
		ja		noWeapon
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		noWeapon
		mov		ecx, [eax+8]
		lea		eax, [ecx+0x148]
		test	byte ptr [ecx+0x12C], 0x80
		cmovnz	edx, eax
		movss	xmm0, [edx]
		cmp		s_patchInstallState.FO3WpnDegrade, 0
		jz		skipFO3
		xor		edx, edx
		mov		dl, [ecx+0x15C]
		sub		dl, kAVCode_BigGuns
		js		skipFO3
		cmp		dl, 0xC
		ja		skipFO3
		mov		dl, byte ptr kSkillMatcher[edx]
		test	dl, dl
		js		skipFO3
		mulss	xmm0, kDegrMults[edx*4]
		mov		dx, [ecx+0xA0]
		cvtsi2ss	xmm1, edx
		mulss	xmm0, xmm1
	skipFO3:
		push	ecx
		movss	[esp], xmm0
		cmp		dword ptr [ecx+0xA8], 0
		jz		noAmmo
		mov		eax, [esp+8]
		mov		ecx, [eax+0x68]
		mov		eax, [ecx+0x118]
		test	eax, eax
		jz		noAmmo
		mov		ecx, [eax+8]
		test	ecx, ecx
		jz		noAmmo
		cmp		dword ptr [ecx], kVtbl_TESAmmo
		jnz		noAmmo
		add		ecx, 0xD4
		cmp		dword ptr [ecx], 0
		jz		noAmmo
		push	ecx
		push	4
		CALL_EAX(ADDR_ApplyAmmoEffects)
		add		esp, 0xC
		retn
	noAmmo:
		fld		dword ptr [esp]
		pop		ecx
		retn
	noWeapon:
		fld		dword ptr [edx]
		retn
		ALIGN 8
	kSkillMatcher:
		EMIT_B_8(3, 1, 3, -1, -1, 2, -1, -1)
		EMIT_B_8(0, -1, -1, -1, 2, 0, 0, 0)
	kDegrMults:
		EMIT_DW_4(0x3E19999A, 0x3E4CCCCD, 0x3E800000, 0x3E99999A)
	}
}

//	Credits to lStewieAl and TrueCourierSix
__declspec(naked) void __fastcall LoadMediaSetIDHook(ModInfo *modInfo, int, UInt32 *pRefID)
{
	__asm
	{
		mov		edx, [esp+4]
		push	ecx
		push	edx
		push	edx
		CALL_EAX(0x4727F0)
		CALL_EAX(0x485D50)
		add		esp, 8
		retn	4
	}
}

typedef Vector<MediaSet*, 4> MediaSetArray;
TempObject<UnorderedMap<tList<MediaSet>::Node*, MediaSetArray>> s_pickMediaSetMap(0x40);

MediaSet* __stdcall PickMediaSetHook(tList<MediaSet>::Node *listHead)
{
	if (!listHead->next)
		return listHead->data;
	MediaSetArray &msArr = s_pickMediaSetMap()[listHead];
	if (msArr.Empty())
	{
		do {
			if (listHead->data)
				msArr.Append(listHead->data);
		} while (listHead = listHead->next);
		ArrayUtils<MediaSetArray>::Shuffle(msArr);
	}
	return msArr.Pop();
}

__declspec(naked) MediaSet* __fastcall PickBattleMediaSetHook(TESRegionDataSound *dataSound)
{
	__asm
	{
		add		ecx, 0x18
		cmp		dword ptr [ecx], 0
		jz		retnNull
		mov		edx, ecx
		or		eax, 0xFFFFFFFF
		ALIGN 16
	countIter:
		inc		eax
		mov		edx, [edx+4]
		test	edx, edx
		jnz		countIter
		test	eax, eax
		jz		getForm
		mov		edx, [ecx+4]
		cmp		eax, 1
		jz		doSwap
		push	ecx
		push	eax
		mov		ecx, GAME_RNG
		CALL_EAX(ADDR_GetRandomInt)
		pop		ecx
		mov		edx, ecx
	getNth:
		mov		edx, [edx+4]
		dec		eax
		jns		getNth
	doSwap:
		push	dword ptr [ecx]
		mov		eax, [edx]
		mov		[ecx], eax
		pop		dword ptr [edx]
	getForm:
		push	dword ptr [ecx]
		call	LookupFormByRefID
		test	eax, eax
		jz		retnNull
		cmp		byte ptr [eax+4], kFormType_MediaSet
		jnz		retnNull
		retn
	retnNull:
		xor		eax, eax
		retn
	}
}

TempObject<Vector<TESLoadScreen*>> s_locationLoadScreens, s_genericLoadScreens;

__declspec(naked) void __fastcall GetSuitableLoadScreensHook(LoadingMenu *loadingMenu, int, BSSimpleArray<TESLoadScreen*> *results)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		test	byte ptr [ecx+0x222], 0x20
		setnz	bl
		mov		bh, byte ptr ds:0x11D8907
		mov		eax, g_OSGlobals
		or		bh, byte ptr [eax+2]
		mov		eax, [esp+0x10]
		and		dword ptr [eax+8], 0
		mov		eax, [ecx+0x1E8]
		push	eax
		test	bl, bl
		jnz		doGeneric
		test	eax, eax
		jz		doGeneric
		mov		esi, s_locationLoadScreens
		mov		edi, s_locationLoadScreens+4
		ALIGN 16
	iter1Head:
		push	dword ptr [esp]
		mov		ecx, [esi]
		CALL_EAX(0x5A6EC0)
		test	al, al
		jz		iter1Next
		test	bh, bh
		jz		doAdd1
		mov		eax, [esi]
		mov		eax, [eax+0x34]
		test	eax, eax
		jz		doAdd1
		mov		eax, [eax+0x18]
		test	eax, eax
		jz		doAdd1
		cmp		eax, 3
		jnz		iter1Next
	doAdd1:
		push	esi
		mov		ecx, [esp+0x18]
		CALL_EAX(0x7CB2E0)
	iter1Next:
		add		esi, 4
		dec		edi
		jnz		iter1Head
		mov		eax, [esp+0x14]
		cmp		dword ptr [eax+8], 0
		jnz		done
	doGeneric:
		mov		edi, s_genericLoadScreens+4
		test	edi, edi
		jz		done
		mov		esi, s_genericLoadScreens
		ALIGN 16
	iter2Head:
		mov		eax, [esi]
		test	bl, bl
		jz		doneCheck
		test	byte ptr [eax+9], 4
		jz		iter2Next
	doneCheck:
		test	bh, bh
		jz		doAdd2
		mov		eax, [eax+0x34]
		test	eax, eax
		jz		doAdd2
		mov		eax, [eax+0x18]
		test	eax, eax
		jz		doAdd2
		cmp		eax, 3
		jnz		iter2Next
	doAdd2:
		push	esi
		mov		ecx, [esp+0x18]
		CALL_EAX(0x7CB2E0)
	iter2Next:
		add		esi, 4
		dec		edi
		jnz		iter2Head
	done:
		pop		ecx
		pop		edi
		pop		esi
		pop		ebx
		retn	4
	}
}

__declspec(naked) void PickLoadScreenHook()
{
	__asm
	{
		mov		ecx, [ebp-0x44]
		cmp		dword ptr [ecx+0x1D0], 0
		JMP_EAX(0x78A7A5)
	}
}

UInt32 s_uWMChancePerLevel = 0, s_uWMChanceMin = 0, s_uWMChanceMax = 0;

void __fastcall DistributeWeaponMods(Actor *actor, ContChangesEntry *weaponInfo)
{
	static const UInt8 kWeaponModShuffle[][4] =
	{
		{1, 2, 4, 0}, {1, 4, 2, 0}, {2, 1, 4, 0},
		{2, 4, 1, 0}, {4, 1, 2, 0}, {4, 2, 1, 0}
	};
	TESObjectWEAP *weapon = weaponInfo->weapon;
	if ((weapon->eWeaponType > 9) || (weapon->weaponFlags1 & 0xA0))
		return;
	ExtraDataList *xDataList = weaponInfo->extendData->GetFirstItem();
	if (xDataList->HasType(kXData_ExtraWeaponModFlags))
		return;
	UInt8 modFlags;
	__asm
	{
		mov		eax, weapon
		movups	xmm0, [eax+0x350]
		xorps	xmm1, xmm1
		pcmpeqd	xmm1, xmm0
		movmskps	eax, xmm1
		not		al
		and		al, 7
		mov		modFlags, al
	}
	if (!modFlags)
		return;
	UInt32 chance = actor->GetLevel() * s_uWMChancePerLevel;
	if (chance < s_uWMChanceMin)
		chance = s_uWMChanceMin;
	else if (chance > s_uWMChanceMax)
		chance = s_uWMChanceMax;
	UInt32 rand = GetRandomUInt(100);
	if (chance <= rand)
		return;
	const UInt8 *shuffle = kWeaponModShuffle[GetRandomUInt(6)];
	if (modFlags & shuffle[0])
		chance >>= 1;
	if (modFlags & shuffle[1])
	{
		if (chance <= rand)
		{
			modFlags = shuffle[0];
			goto doneFlags;
		}
		chance >>= 1;
	}
	if ((modFlags & shuffle[2]) && (chance <= rand))
		modFlags ^= shuffle[2];
doneFlags:
	xDataList->AddExtra(ExtraWeaponModFlags::Create(modFlags));
	ThisCall(0x88DB20, actor, weapon, 1, xDataList, false);
}

const SInt8 kValidEntryPoints[] =
{
	0, 1, 2, -1, 3, -1, 4, -1, -1, -1, -1, -1, 5, -1, 6, -1, -1, -1, -1, -1, -1, -1, 7, 8, -1, 9, -1, -1, -1, -1, -1, -1, -1, -1, 10, -1, 11, 12,
	13, -1, -1, -1, 14, 15, -1, -1, 16, -1, -1, -1, -1, -1, 17, -1, -1, 18, 19, -1, 20, 21, -1, -1, -1, -1, -1, -1, -1, -1, 22, -1, -1, -1, 23, -1
};

TempObject<Vector<BGSPerk*>> s_validNPCPerks(0x40), s_validNPCTraits(0x20), s_NPCPerksPick(0x40);

void BuildValidNPCPerks()
{
	auto perkIter = g_dataHandler->perkList.Head();
	do
	{
		if (BGSPerk *perk = perkIter->data; perk && !perk->entries.Empty() && perk->data.isPlayable && !perk->data.isHidden)
		{
			auto entryIter = perk->entries.Head();
			bool isValid = 0;
			do
			{
				BGSPerkEntry *entry = entryIter->data;
				if (!entry) goto isInvalid;
				switch (entry->GetType())
				{
					default:
					case 0:
						goto isInvalid;
					case 1:
					{
						SpellItem *ability = ((BGSAbilityPerkEntry*)entry)->ability;
						if (!ability) goto isInvalid;
						auto effIter = ability->magicItem.list.list.Head();
						do
						{
							if (EffectItem *effItem = effIter->data; effItem && effItem->setting && (effItem->setting->archtype != EffectSetting::kArchType_Script))
								continue;
							goto isInvalid;
						}
						while (effIter = effIter->next);
						break;
					}
					case 2:
						isValid |= kValidEntryPoints[((BGSEntryPointPerkEntry*)entry)->entryPoint] >= 0;
						break;
				}
			}
			while (entryIter = entryIter->next);
			if (isValid)
			{
				if (perk->data.isTrait)
					s_validNPCTraits->Append(perk);
				else s_validNPCPerks->Append(perk);
			}
		}
	isInvalid:
		continue;
	}
	while (perkIter = perkIter->next);

	/*for (auto trtIter = s_validNPCTraits->Begin(); trtIter; ++trtIter)
		PrintLog("%08X\t%s", trtIter->refID, trtIter->GetEditorID());
	PrintLog("\n================\n");
	for (auto prkIter = s_validNPCPerks->Begin(); prkIter; ++prkIter)
		PrintLog("%08X\t%s", prkIter->refID, prkIter->GetEditorID());
	PrintLog("\n================\n");*/
}

typedef Map<BGSPerk*, UInt8, 4> PerkRanksMap;

class NPCPerkEntryPoints
{
	PerkEntryPointList		perkEntries[24];

public:
	static NPCPerkEntryPoints *Create()
	{
		NPCPerkEntryPoints *lists = Pool_Alloc<NPCPerkEntryPoints>();
		ZERO_BYTES(lists, sizeof(NPCPerkEntryPoints));
		return lists;
	}

	PerkEntryPointList* operator[](UInt8 entryPt)
	{
		int index = kValidEntryPoints[entryPt];
		return (index >= 0) ? &perkEntries[index] : nullptr;
	}

	void Destroy()
	{
		for (auto &iter : perkEntries)
			iter.RemoveAll();
		Pool_Free<NPCPerkEntryPoints>(this);
	}
};

struct NPCPerksInfo
{
	PerkRanksMap			perkRanks;
	NPCPerkEntryPoints		*entryPoints = nullptr;

	~NPCPerksInfo() {if (entryPoints) entryPoints->Destroy();}

	void Reset()
	{
		perkRanks.Clear();
		if (entryPoints)
		{
			entryPoints->Destroy();
			entryPoints = nullptr;
		}
	}
};

TempObject<UnorderedMap<UInt32, NPCPerksInfo>> s_NPCPerksInfoMap(0x80);
bool s_NPCPerksAutoAdd = false;

__declspec(naked) bool __fastcall DestroyActorHook(Actor *actor)
{
	__asm
	{
		cmp		dword ptr [ecx+0x60], 0
		jz		done
		and		dword ptr [ecx+0x60], 0
		or		s_dataChangedFlags, kChangedFlag_NPCPerks
		push	ecx
		push	dword ptr [ecx+0xC]
		mov		ecx, offset s_NPCPerksInfoMap
		call	UnorderedMap<UInt32, NPCPerksInfo>::Erase
		pop		ecx
	done:
		test	byte ptr [ecx+9], 0x40
		setnz	al
		retn
	}
}

NPCPerksInfo* __fastcall AddStartingPerks(Actor *actor, NPCPerksInfo *perksInfo)
{
	if (s_NPCPerksAutoAdd && IS_ID(actor, Character))
	{
		if (!perksInfo && !s_NPCPerksInfoMap->Insert(actor->refID, &perksInfo))
			perksInfo->Reset();
		perksInfo->perkRanks.Emplace(s_validNPCTraits()[GetRandomUInt(s_validNPCTraits->Size())], 1);
		UInt32 level = actor->GetLevel();
		if (level >= 3)
		{
			s_NPCPerksPick->Clear();
			BGSPerk *perk;
			UInt8 minLevel = (level * 3) >> 1;
			for (auto rstIter = s_validNPCPerks->Begin(); rstIter; ++rstIter)
			{
				perk = *rstIter;
				if (perk->data.minLevel > minLevel) continue;
				for (UInt8 ranks = perk->data.numRanks; ranks; ranks--)
					s_NPCPerksPick->Append(perk);
			}
			UInt32 size = s_NPCPerksPick->Size();
			if (size)
			{
				if (level >= 30) level = 10;
				else level /= 3;
				if (level < size)
					ArrayUtils<Vector<BGSPerk*>>::Shuffle(s_NPCPerksPick());
				else level = size;
				do
				{
					perksInfo->perkRanks[s_NPCPerksPick()[--level]]++;
				}
				while (level);
			}
		}
	}
	if (perksInfo)
	{
		actor->extraDataList.perksInfo = perksInfo;
		s_dataChangedFlags |= kChangedFlag_NPCPerks;
	}
	return perksInfo;
}

void __fastcall AddPerkEntriesNPC(Actor *actor, BGSPerk *perk, UInt8 newRank, NPCPerkEntryPoints *entryPoints)
{
	newRank--;
	auto entryIter = perk->entries.Head();
	bool bUpdReload = false;
	do
	{
		if (BGSPerkEntry *entry = entryIter->data)
			if (UInt32 eType = entry->GetType())
				if (eType == 2)
				{
					BGSEntryPointPerkEntry *entryPt = (BGSEntryPointPerkEntry*)entry;
					if (PerkEntryPointList *entryList = (*entryPoints)[entryPt->entryPoint])
					{
						if (entryPt->rank != newRank)
						{
							if (!entryList->Remove(entryPt))
								continue;
						}
						else if (!entryList->AppendNotIn(entryPt))
							continue;
						bUpdReload |= (entryPt->entryPoint == kPerkEntry_ReloadSpeed);
					}
				}
				else if (entry->rank != newRank)
					entry->RemoveEntry(actor, 0);
				else entry->AddEntry(actor, 0);
	}
	while (entryIter = entryIter->next);
	if (bUpdReload)
		ThisCall(0x8C17C0, actor);
}

void __fastcall SetPerkRankNPCHook(Actor *actor, int, BGSPerk *perk, UInt8 newRank, bool forTeammates)
{
	if (actor->lifeState) return;
	NPCPerksInfo *perksInfo = actor->extraDataList.perksInfo;
	if (!perksInfo)
	{
		if (!s_NPCPerksInfoMap->Insert(actor->refID, &perksInfo))
			perksInfo->Reset();
		AddStartingPerks(actor, perksInfo);
	}
	if (UInt8 *rankPtr; perksInfo->perkRanks.InsertKey(perk, &rankPtr) || (*rankPtr < newRank))
	{
		*rankPtr = newRank;
		s_dataChangedFlags |= kChangedFlag_NPCPerks;
		NPCPerkEntryPoints *entryPoints = perksInfo->entryPoints;
		if (!entryPoints)
		{
			if (!actor->GetRefNiNode())
				return;
			perksInfo->entryPoints = entryPoints = NPCPerkEntryPoints::Create();
		}
		AddPerkEntriesNPC(actor, perk, newRank, entryPoints);
	}
}

void __fastcall AddTeammatePerkEntries(Actor *teammate, BGSPerk *perk, UInt8 newRank)
{
	if (teammate->extraDataList.perksInfo && teammate->extraDataList.perksInfo->perkRanks.HasKey(perk))
		return;
	auto entryIter = perk->entries.Head();
	do
	{
		if (BGSPerkEntry *entry = entryIter->data; entry && IS_TYPE(entry, BGSAbilityPerkEntry))
			if (entry->rank != newRank)
				entry->RemoveEntry(teammate, 0);
			else entry->AddEntry(teammate, 0);
	}
	while (entryIter = entryIter->next);
}

void __fastcall RemoveTeammatePerkEntries(Actor *teammate, BGSPerk *perk)
{
	if (teammate->extraDataList.perksInfo && teammate->extraDataList.perksInfo->perkRanks.HasKey(perk))
		return;
	auto entryIter = perk->entries.Head();
	do
	{
		if (BGSPerkEntry *entry = entryIter->data; entry && IS_TYPE(entry, BGSAbilityPerkEntry))
			entry->RemoveEntry(teammate, 0);
	}
	while (entryIter = entryIter->next);
}

void __fastcall SetPerkRankPlayerHook(PlayerCharacter *thePlayer, int, BGSPerk *perk, UInt8 newRank, bool forTeammates)
{
	auto perkList = forTeammates ? &thePlayer->perkRanksTM : &thePlayer->perkRanksPC;
	PerkRank *perkRank = perkList->Find(PerkRankFinder(perk));
	if (!perkRank)
	{
		perkRank = Game_HeapAlloc<PerkRank>();
		perkRank->perk = perk;
		perkList->Prepend(perkRank);
	}
	else if (perkRank->rank == newRank)
		return;
	perkRank->rank = newRank--;
	if (!forTeammates)
	{
		auto entryIter = perk->entries.Head();
		do
		{
			if (BGSPerkEntry *entry = entryIter->data)
				if (entry->rank != newRank)
					entry->RemoveEntry(thePlayer, 0);
				else entry->AddEntry(thePlayer, 0);
		}
		while (entryIter = entryIter->next);
		CdeclCall(0x7DD710);
		ThisCall(0x8C17C0, thePlayer);
	}
	else
	{
		auto tmmIter = thePlayer->teammates.Head();
		do
		{
			if (Actor *teammate = (Actor*)tmmIter->data)
				AddTeammatePerkEntries(teammate, perk, newRank);
		}
		while (tmmIter = tmmIter->next);
	}
}

void __fastcall RemovePerkNPCHook(Actor *actor, int, BGSPerk *perk, bool forTeammates)
{
	NPCPerksInfo *perksInfo = actor->extraDataList.perksInfo;
	if (!perksInfo || !perksInfo->perkRanks.Erase(perk))
		return;
	s_dataChangedFlags |= kChangedFlag_NPCPerks;
	NPCPerkEntryPoints *entryPoints = perksInfo->entryPoints;
	auto entryIter = perk->entries.Head();
	PerkEntryPointList *entryList;
	bool bUpdReload = false;
	do
	{
		if (BGSPerkEntry *entry = entryIter->data)
			if (UInt32 eType = entry->GetType())
				if (eType == 2)
				{
					BGSEntryPointPerkEntry *entryPt = (BGSEntryPointPerkEntry*)entry;
					if (entryPoints && (entryList = (*entryPoints)[entryPt->entryPoint]) && entryList->Remove(entryPt) && (entryPt->entryPoint == kPerkEntry_ReloadSpeed))
						bUpdReload = true;
				}
				else entry->RemoveEntry(actor, 0);
	}
	while (entryIter = entryIter->next);
	if (bUpdReload)
		ThisCall(0x8C17C0, actor);
	if (perksInfo->perkRanks.Empty())
	{
		actor->extraDataList.perksInfo = nullptr;
		s_NPCPerksInfoMap->Erase(actor->refID);
	}
}

void __fastcall RemovePerkPlayerHook(PlayerCharacter *thePlayer, int, BGSPerk *perk, bool forTeammates)
{
	auto ranksList = !forTeammates ? &thePlayer->perkRanksPC : &thePlayer->perkRanksTM;
	PerkRank *perkRank = ranksList->RemoveIf(PerkRankFinder(perk));
	if (!perkRank) return;
	Game_HeapFree(perkRank);
	if (!forTeammates)
	{
		auto entryIter = perk->entries.Head();
		do
		{
			if (BGSPerkEntry *entry = entryIter->data)
				entry->RemoveEntry(thePlayer, 0);
		}
		while (entryIter = entryIter->next);
		CdeclCall(0x7DD710);
		ThisCall(0x8C17C0, thePlayer);
	}
	else
	{
		auto tmmIter = thePlayer->teammates.Head();
		do
		{
			if (Actor *teammate = (Actor*)tmmIter->data)
				RemoveTeammatePerkEntries(teammate, perk);
		}
		while (tmmIter = tmmIter->next);
	}
}

__declspec(naked) UInt8 __fastcall GetPerkRankNPCHook(Actor *actor, int, BGSPerk *perk, bool forTeammates)
{
	__asm
	{
		mov		eax, [ecx+0x60]
		test	eax, eax
		jz		checkTM
		push	ecx
		push	dword ptr [esp+8]
		mov		ecx, eax
		call	PerkRanksMap::Get
		pop		ecx
		test	al, al
		jz		checkTM
		retn	8
	checkTM:
		cmp		byte ptr [ecx+0x18D], 0
		jz		done
		mov		edx, [esp+4]
		mov		eax, g_thePlayer
		add		eax, 0xAD4
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		mov		eax, [eax+4]
		test	ecx, ecx
		jz		iterHead
		cmp		[ecx], edx
		jnz		iterHead
		mov		al, [ecx+4]
	done:
		retn	8
	}
}

__declspec(naked) PerkEntryPointList* __fastcall GetPerkEntryPointListNPCHook(Actor *actor, int, UInt8 entryPointID, bool forTeammates)
{
	__asm
	{
		push	ecx
		mov		eax, [ecx+0x60]
		test	eax, eax
		jz		checkTM
		mov		ecx, [eax+0xC]
		test	ecx, ecx
		jz		checkTM
		mov		eax, [esp+8]
		movsx	edx, byte ptr kValidEntryPoints[eax]
		test	edx, edx
		js		checkTM
		lea		eax, [ecx+edx*8]
		cmp		dword ptr [eax], 0
		jz		checkTM
		pop		ecx
		retn	8
	checkTM:
		pop		ecx
		xor		eax, eax
		cmp		byte ptr [ecx+0x18D], 0
		jz		done
		mov		ecx, g_thePlayer
		mov		edx, [esp+4]
		lea		edx, [ecx+edx*8+0xADC]
		cmp		[edx], eax
		cmovnz	eax, edx
	done:
		retn	8
	}
}

void __fastcall SetPlayerTeammateHook(Actor *actor, int, bool doSet)
{
	PlayerCharacter *thePlayer = g_thePlayer;
	if ((actor == thePlayer) || (actor->isTeammate == doSet))
		return;
	actor->isTeammate = doSet;
	if (doSet)
	{
		thePlayer->teammateCount++;
		thePlayer->teammates.Prepend(actor);
		if (!thePlayer->perkRanksTM.Empty())
		{
			auto perkIter = thePlayer->perkRanksTM.Head();
			do
			{
				if (PerkRank *perkRank = perkIter->data)
					AddTeammatePerkEntries(actor, perkRank->perk, perkRank->rank - 1);
			}
			while (perkIter = perkIter->next);
		}
	}
	else
	{
		thePlayer->teammateCount--;
		thePlayer->teammates.Remove(actor);
		if (!thePlayer->perkRanksTM.Empty() && actor->extraDataList.perksInfo)
		{
			auto perkIter = thePlayer->perkRanksTM.Head();
			do
			{
				if (PerkRank *perkRank = perkIter->data)
					RemoveTeammatePerkEntries(actor, perkRank->perk);
			}
			while (perkIter = perkIter->next);
		}
	}
}

void __fastcall InitNPCPerks(Actor *actor)
{
	NPCPerksInfo *perksInfo = actor->extraDataList.perksInfo;
	if (!perksInfo)
		perksInfo = AddStartingPerks(actor, nullptr);
	if (perksInfo && !perksInfo->entryPoints)
	{
		perksInfo->entryPoints = NPCPerkEntryPoints::Create();
		for (auto perkIter = perksInfo->perkRanks.Begin(); perkIter; ++perkIter)
		{
			AddPerkEntriesNPC(actor, perkIter.Key(), perkIter(), perksInfo->entryPoints);
			//PrintLog("%08X\t%d\t%s", actor->refID, perkIter(), perkIter.Key()->GetEditorID());
		}
	}
}

__declspec(naked) void DoOnLoadActorHook()
{
	__asm
	{
		push	dword ptr [ecx+0xC]
		push	0
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		mov		[esp], eax
		mov		eax, g_BGSSaveLoadGame
		test	byte ptr [eax+0x244], 2
		jnz		done
		cmp		dword ptr [ecx+0x108], 0
		jnz		done
		cmp		s_patchInstallState.NPCPerks, 0
		jz		donePerks
		call	InitNPCPerks
		mov		ecx, [ebp+8]
	donePerks:
		cmp		s_patchInstallState.NPCWeaponMods, 0
		jz		done
		test	byte ptr [ecx+0x143], 0x10
		jnz		done
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x28], 1
		ja		done
		or		byte ptr [ecx+0x143], 0x10
		cmp		byte ptr [ecx+0x18D], 0
		jnz		done
		mov		edx, [eax+0x114]
		test	edx, edx
		jz		done
		cmp		dword ptr [edx], 0
		jz		done
		call	DistributeWeaponMods
	done:
		CALL_EAX(0x4B6DC0)
		leave
		retn	0x10
	}
}

__declspec(naked) void ResetActorFlagsRespawnHook()
{
	__asm
	{
		and		byte ptr [ecx+0x143], 0xEF
		lea		eax, [ecx+0x44]
		retn
	}
}

UInt32 s_doCalcDamage = 0;

float s_VATSHitDT = -FLT_MAX;

void __fastcall CalculateHitDamageHook(ActorHitData *hitData, int, UInt32 noBlock)
{
	if (s_doCalcDamage)
	{
		noBlock = s_doCalcDamage - 1;
		s_doCalcDamage = 0;
	}
	else if (UInt32 retnAddr = (UInt32)_ReturnAddress(); (retnAddr == 0x9B5628) || (retnAddr == 0x9B5707))
	{
		s_doCalcDamage = noBlock + 1;
		return;
	}
	Actor *target = hitData->target;
	if (!target || NOT_ACTOR(target)) return;
	if (target->magicTarget.CannotBeHit())
	{
		hitData->healthDmg = 0;
		hitData->wpnBaseDmg = 0;
		hitData->fatigueDmg = 0;
		return;
	}
	Actor *source = hitData->source;
	TESObjectWEAP *hitWeapon = hitData->weapon;
	hitData->wpnBaseDmg = hitData->healthDmg;
	Projectile *projectile = (hitData->projectile && IS_PROJECTILE(hitData->projectile)) ? hitData->projectile : nullptr;
	TESAmmo *ammo = projectile ? projectile->GetAmmo() : nullptr;
	AmmoEffectList *ammoEffects = nullptr;
	if (ammo && !ammo->effectList.Empty())
	{
		ammoEffects = &ammo->effectList;
		hitData->fatigueDmg = ApplyAmmoEffects(kAmmoEffect_FatigueMod, ammoEffects, hitData->fatigueDmg);
	}
	UInt8 flagPCTM = 0;
	if (source)
	{
		if (source->IsPlayer())
			flagPCTM = 1;
		else if (source->isTeammate)
			flagPCTM = 2;
		else if (s_patchInstallState.NPCPerks)
			flagPCTM = 4;
	}
	if (target->IsPlayer())
		flagPCTM |= 8;
	else if (target->isTeammate)
		flagPCTM |= 0x10;
	else if (s_patchInstallState.NPCPerks)
		flagPCTM |= 0x20;
	float dmgResist, dmgThreshold, cpyThreshold, hitLocDT = 0, hitLocDR = 0, valueMod;
	if (hitWeapon && (hitWeapon->weaponFlags1 & 1))
	{
		dmgResist = 1.0F;
		dmgThreshold = 0;
		cpyThreshold = 0;
	}
	else
	{
		if (Character *character = (Character*)target; s_patchInstallState.localizedDTDR && NOT_ID(character, Creature) &&
			(hitData->hitLocation >= 0) && (hitData->hitLocation <= 12) && character->bipedAnims)
		{
			if (TESObjectARMO *armor = character->bipedAnims->slotData[2].armor; armor && IS_TYPE(armor, TESObjectARMO))
				if (ContChangesEntry *entry = character->GetContainerChangesEntry(armor))
					if (ExtraDataList *xDataList = entry->GetEquippedExtra())
					{
						hitLocDT = armor->damageThreshold;
						hitLocDR = armor->armorRating * 0.01F;
						if (ExtraHealth *xHealth = GetExtraType(xDataList, ExtraHealth))
						{
							valueMod = xHealth->health / (int)armor->health.health;
							if (valueMod < 0.5F)
							{
								valueMod = 1.0F - (0.5F - valueMod) * s_condDRDTPenalty;
								hitLocDT *= valueMod;
								hitLocDR *= valueMod;
							}
						}
					}
			if (hitData->hitLocation != 1)
			{
				hitLocDT = GetMax(ThisCall<float>(0x8D2110, character) - hitLocDT, 0.0F);
				hitLocDR = GetMax(ThisCall<float>(0x8D22B0, character) - hitLocDR, 0.0F);
			}
		}
		valueMod = 1.0F;
		if (flagPCTM & 0x10)
			valueMod += g_thePlayer->avOwner.GetThresholdedAV(kAVCode_Charisma) * 0.05F;
		dmgResist = (target->avOwner.GetActorValue(kAVCode_DamageResist) - hitLocDR) * valueMod;
		dmgResist = GetMin(dmgResist, 100.0F);
		if (ammoEffects)
			dmgResist = ApplyAmmoEffects(kAmmoEffect_DRMod, ammoEffects, dmgResist);
		if (dmgResist <= 0)
			dmgResist = 1.0F;
		else
			dmgResist = 1.0F - (GetMin(dmgResist, GMST_FLT(fMaxArmorRating)) * 0.01F);
		cpyThreshold = dmgThreshold = (target->avOwner.GetActorValue(kAVCode_DamageThreshold) - hitLocDT) * valueMod;
		if (ammoEffects)
			dmgThreshold = ApplyAmmoEffects(kAmmoEffect_DTMod, ammoEffects, dmgThreshold);
	}
	HighProcess *hiProcess = (HighProcess*)target->baseProcess;
	if (!noBlock && hiProcess && !hiProcess->processLevel && (hiProcess->currentAction == kAnimAction_Block) && !target->GetIsParalyzed() &&
		CdeclCall<bool>(0x9A6AE0, target, projectile ? projectile : (TESObjectREFR*)source, 0))
	{
		ContChangesEntry *weaponInfo = hiProcess->weaponInfo;
		bool noInfo = !weaponInfo;
		TESObjectWEAP *pWeapon = weaponInfo ? weaponInfo->weapon : nullptr;
		hitData->flags |= 1;
		if (flagPCTM & 8)
			g_thePlayer->counterAttackTimer = GMST_FLT(fCounterAttackTimer);
		if (noInfo)
		{
			if ((!hitWeapon || (hitWeapon->eWeaponType <= 2)) && !projectile)
				noInfo = false;
			else hitData->blockDTMod = 0;
		}
		if (!noInfo)
		{
			UInt32 skillModType = 0;
			if (!pWeapon)
				skillModType = kAVCode_Unarmed;
			else if (!pWeapon->eWeaponType)
				skillModType = kAVCode_Unarmed;
			else if (pWeapon->eWeaponType <= 2)
				skillModType = kAVCode_MeleeWeapons;
			hitData->blockDTMod = GMST_FLT(fBlockSkillBase);
			if (skillModType)
				hitData->blockDTMod += GMST_FLT(fBlockSkillMult) * target->avOwner.GetActorValue(skillModType);
			dmgThreshold += hitData->blockDTMod;
		}
		if (pWeapon && hiProcess->isWeaponOut)
			hitData->flags |= 2;
		else hitData->flags &= ~2;
	}
	if (hitWeapon)
	{
		if (ammoEffects)
			hitData->healthDmg = ApplyAmmoEffects(kAmmoEffect_DamageMod, ammoEffects, hitData->healthDmg);
		if (flagPCTM & 7)
			ApplyPerkModifiers(kPerkEntry_CalculateWeaponDamage, source, hitWeapon, target, &hitData->healthDmg);
	}
	else if (source)
	{
		hitData->weapon = g_fistsWeapon;
		hitWeapon = g_fistsWeapon;
	}
	hitData->healthDmg *= dmgResist;
	if (flagPCTM & 0x38)
	{
		valueMod = 0;
		ApplyPerkModifiers(kPerkEntry_ModifyDamageThresholdDefender, target, source, hitWeapon ? hitWeapon : g_fistsWeapon, &valueMod);
		dmgThreshold += valueMod;
	}
	if (flagPCTM & 7)
	{
		valueMod = 0;
		ApplyPerkModifiers(kPerkEntry_ModifyDamageThresholdAttacker, source, hitWeapon, target, &valueMod);
		dmgThreshold -= valueMod;
	}
	s_VATSHitDT = dmgThreshold;
	bool flagArg;
	if (dmgThreshold > 0)
	{
		if (projectile && projectile->hasSplitBeams && (projectile->numProjectiles > 1))
			dmgThreshold /= (float)projectile->numProjectiles;
		dmgThreshold *= dmgResist;
		flagArg = false;
	}
	else
	{
		dmgThreshold = 0;
		flagArg = cpyThreshold > 0;
	}
	if (hitWeapon && hitWeapon->IsAutomatic() && (hitWeapon->eWeaponType <= 2))
	{
		int attackDmg = hitWeapon->attackDmg.damage;
		if (!attackDmg) attackDmg = 1;
		valueMod = 1.0F - (dmgThreshold / attackDmg);
		if (valueMod > 0)
			hitData->healthDmg *= valueMod;
	}
	else hitData->healthDmg -= dmgThreshold;
	valueMod = GMST_FLT(fMinDamMultiplier) * hitData->wpnBaseDmg;
	if (hitData->healthDmg > valueMod)
	{
		hitData->flags |= 0x80000000;
		if (flagArg)
			if (flagPCTM & 8)
				CdeclCall(0x77F420, true);
			else if ((flagPCTM & 1) && !target->GetDead())
				CdeclCall(0x77F490, true);
	}
	else
	{
		hitData->healthDmg = valueMod;
		if (flagPCTM & 8)
			CdeclCall(0x77F420, flagArg);
		else if ((flagPCTM & 1) && !target->GetDead())
			CdeclCall(0x77F490, flagArg);
		if (flagPCTM & 2)
			CdeclCall(0x8D5CB0, source, 9);
	}
	if ((flagPCTM & 8) && (hitData->fatigueDmg <= 0))
		hitData->armorDmg = (valueMod * (1.0F - dmgResist) + GetMin(hitData->healthDmg, dmgThreshold)) * GMST_FLT(fDamageToArmorPercentage);
	if (hitWeapon && (hitWeapon->resistType != -1) && (!ammo || !(ammo->ammoFlags & 1)))
	{
		if (valueMod = target->avOwner.GetActorValue(hitWeapon->resistType); valueMod > 0)
			if (valueMod < 100.0F)
				hitData->healthDmg *= 1.0F - valueMod * 0.01F;
			else hitData->healthDmg = 0;
	}
	if ((hitData->fatigueDmg > 0) && (hitData->wpnBaseDmg > 0))
		hitData->fatigueDmg *= hitData->healthDmg / hitData->wpnBaseDmg;
	if ((flagPCTM & 8) && VATSCameraData::Get()->mode)
	{
		valueMod = GMST_FLT(fVATSPlayerDamageMult);
		hitData->healthDmg *= valueMod;
		hitData->wpnBaseDmg *= valueMod;
		hitData->fatigueDmg *= valueMod;
		hitData->limbDmg *= valueMod;
		hitData->armorDmg *= valueMod;
		hitData->weaponDmg *= valueMod;
	}
}

__declspec(naked) void SetHitLocationHook()
{
	__asm
	{
		cmp		s_doCalcDamage, 0
		jz		skipCalc
		push	0
		mov		ecx, [ebp-0x18]
		call	CalculateHitDamageHook
	skipCalc:
		mov		eax, [ebp-0x18]
		cmp		dword ptr [eax+0x10], 0xE
		jnz		done
		and		dword ptr [ebp-0x24], 0
		movss	xmm0, [eax+0x14]
		and		dword ptr [eax+0x14], 0
		cmp		dword ptr [eax+0x54], 0
		jz		skipMod
		mulss	xmm0, ds:0x11CE5D4
	skipMod:
		movss	[eax+0x20], xmm0
	done:
		JMP_EAX(0x9B6EC4)
	}
}

__declspec(naked) void InitProjectileSetAmmoHook()
{
	__asm
	{
		push	ecx
		mov		ecx, [ebp+0x10]
		call	TESObjectWEAP::GetEquippedAmmo
		mov		ecx, [ebp-0x28]
		mov		[ecx+0x60], eax
		movss	xmm0, [ecx+0xDC]
		mov		ecx, [ebp+0x10]
		movzx	edx, byte ptr [ecx+0x11E]
		test	eax, eax
		jz		noAmmo
		movsx	eax, byte ptr [eax+0xB0]
		test	eax, eax
		jle		noAmmo
		movzx	edx, byte ptr [ecx+0x102]
		imul	edx, eax
	noAmmo:
		mov		eax, [ebp-0x28]
		cmp		[ebp-0x15], 0
		setnz	[eax+0x149]
		jz		noSplBeam
		mulss	xmm0, ds:0x1022004
		mov		eax, 0xC
		add		ecx, 0x180
		cmp		[ecx], eax
		jz		foundMod
		add		ecx, 4
		cmp		[ecx], eax
		jz		foundMod
		add		ecx, 4
		cmp		[ecx], eax
		jnz		restoreProj
	foundMod:
		cvttss2si	eax, [ecx+0xC]
		add		edx, eax
	restoreProj:
		mov		eax, [ebp-0x28]
	noSplBeam:
		mov		[eax+0x14A], dl
		cmp		dl, 1
		jbe		done
		cvtsi2ss	xmm1, edx
		divss	xmm0, xmm1
	done:
		movss	[eax+0xDC], xmm0
		JMP_EDX(0x9BC293)
	}
}

__declspec(naked) void RunAmmoImpactScriptHook()
{
	__asm
	{
		mov		eax, [ebx+8]
		mov		ecx, [eax+8]
		test	ecx, ecx
		jz		done
		mov		eax, [ecx+0x60]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax], kVtbl_TESAmmo
		jnz		done
		mov		ecx, [eax+0xA0]
		test	ecx, ecx
		jz		done
		push	0
		push	dword ptr [ebp-0x18]
		CALL_EAX(0x5AC340)
	done:
		JMP_EAX(0x89AC5B)
	}
}

__declspec(naked) void CreateHitBloodHook()
{
	__asm
	{
		xor		edx, edx
		mov		ecx, [ebx+0xC]
		mov		eax, [ecx+0x30]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x24C]
		test	eax, eax
		jz		done
		push	dword ptr [ebp-0x78]
		mov		ecx, eax
		call	GetImpactDataHook
		mov		edx, eax
		test	eax, eax
		jz		done
		mov		eax, [eax+0x48]
	done:
		mov		[ebp-0x8C], edx
		mov		[ebp-0x94], eax
		JMP_EAX(0x88EC51)
	}
}

__declspec(naked) float __cdecl GetVATSTargetDTHook(PlayerCharacter *thePlayer, Actor *target)
{
	__asm
	{
		mov		eax, s_VATSHitDT
		cmp		eax, 0xFF7FFFFF
		jnz		skipCall
		JMP_EAX(0x7700F0)
	skipCall:
		xor		edx, edx
		test	eax, eax
		cmovs	eax, edx
		mov		[esp+4], eax
		fld		dword ptr [esp+4]
		retn
	}
}

UInt8 s_playerMinHPMode = 0;

__declspec(naked) bool PlayerCannotBeHitHook()
{
	__asm
	{
		mov		al, ds:0x11E0898
		test	al, al
		jnz		done
		cmp		s_playerMinHPMode, 2
		setz	al
		jz		done
		mov		al, ds:0x11E07BA
	done:
		retn
	}
}

float s_msgDisplayTime = 2.0F;

__declspec(naked) void QueueUIMessageHook()
{
	__asm
	{
		mov		eax, [ebp+0x18]
		mov		edx, s_msgDisplayTime
		cmp		dword ptr [ebp+4], 0x96A262
		cmovbe	eax, edx
		push	eax
		JMP_EDX(0x705363)
	}
}

UInt8 s_disableHitEffects = 0;

__declspec(naked) TESImageSpaceModifier *GetHitIMODHook()
{
	__asm
	{
		cmp		byte ptr ds:0x118ABB1, 0
		jz		retnNULL
		cmp		s_disableHitEffects, 0
		jnz		retnNULL
		mov		eax, g_getHitIMOD
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) TESImageSpaceModifier *GetExplosionInFaceIMODHook()
{
	__asm
	{
		cmp		s_disableHitEffects, 0
		jnz		retnNULL
		mov		eax, g_explosionInFaceIMOD
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) void ExplosionScreenShakeHook()
{
	__asm
	{
		cmp		s_disableHitEffects, 0
		jnz		skipShake
		mov		edx, [eax+0x1F4]
		JMP_EAX(0x9AD2CB)
	skipShake:
		JMP_EAX(0x9AD2FE)
	}
}

bool s_noMovementCombat = false;

__declspec(naked) bool __fastcall GetControlFlagHook(PlayerCharacter *thePlayer, int, UInt32 flag)
{
	__asm
	{
		cmp		s_noMovementCombat, 1
		jz		done
		test	byte ptr [ecx+0x680], 1
	done:
		setnz	al
		retn	4
	}
}

__declspec(naked) void MineExplodeChanceHook()
{
	__asm
	{
		test	al, al
		jz		retnLoop
		lea		edx, [ebp-0x34]
		mov		dword ptr [edx], 0x42C80000
		push	edx
		mov		edx, [ebp-0x50]
		push	dword ptr [edx+0x20]
		push	dword ptr [ebp-0x28]
		push	4
		CALL_EAX(ADDR_ApplyPerkModifiers)
		add		esp, 0x10
		push	0x64
		mov		ecx, GAME_RNG
		CALL_EAX(ADDR_GetRandomInt)
		cvtss2si	edx, [ebp-0x34]
		cmp		eax, edx
		jnb		retnLoop
		mov		[ebp-1], 1
		JMP_EAX(0x9C3CFA)
	retnLoop:
		JMP_EAX(0x9C3AC0)
	}
}

UInt32 s_clearDepthBuffer = 1;

__declspec(naked) void RendererClearBufferHook()
{
	__asm
	{
		cmp		s_clearDepthBuffer, 0
		jz		skipCall
		JMP_EAX(0x7148C0)
	skipCall:
		retn	8
	}
}

float s_pcRepairSkill = 0, s_repairedItemHealth = 0;
bool s_repairedItemModded = false;

__declspec(naked) void EnableRepairButtonHook()
{
	__asm
	{
		mov		ecx, [ebp+8]
		call	ContChangesEntry::GetHealthPercent
		push	0x5FA0
		ldmxcsr	[esp]
		cvtps2dq	xmm0, xmm0
		cvtdq2ps	xmm0, xmm0
		mov		byte ptr [esp+1], 0x1F
		ldmxcsr	[esp]
		pop		ecx
		movss	s_repairedItemHealth, xmm0
		mov		ecx, [ebp+8]
		mov		eax, [ecx]
		test	eax, eax
		jz		doneMods
		mov		eax, [eax]
		test	eax, eax
		jz		doneMods
		test	byte ptr [eax+0x19], 0x20
		setnz	al
	doneMods:
		mov		s_repairedItemModded, al
		push	kAVCode_Repair
		mov		ecx, g_thePlayer
		add		ecx, 0xA4
		CALL_EAX(0x93ACB0)
		fstp	dword ptr [ebp-0x44]
		movss	xmm0, [ebp-0x44]
		xorps	xmm1, xmm1
		maxss	xmm0, xmm1
		movss	xmm1, ds:0x11D0FF4	// fRepairSkillBase
		movss	xmm2, SS_10
		mulss	xmm1, xmm2
		mulss	xmm2, ds:0x11D0190	// fRepairSkillMax
		subss	xmm2, xmm1
		mulss	xmm0, xmm2
		mulss	xmm0, SS_1d100
		addss	xmm0, xmm1
		minss	xmm0, SS_100
		movss	s_pcRepairSkill, xmm0
		comiss	xmm0, s_repairedItemHealth
		JMP_EAX(0x7818D3)
	}
}

__declspec(naked) void PopulateRepairListHook()
{
	__asm
	{
		push	dword ptr [ebp+8]
		push	dword ptr [eax+8]
		CALL_EAX(0x47BB50)
		add		esp, 8
		test	al, al
		jz		done
		mov		ecx, [ebp-8]
		mov		edx, [ecx+8]
		cmp		[ebp+8], edx
		jnz		done
		cmp		s_repairedItemModded, 0
		jnz		done
		call	ContChangesEntry::GetHealthPercent
		comiss	xmm0, s_repairedItemHealth
		setbe	al
	done:
		JMP_EDX(0x4D4C26)
	}
}

__declspec(naked) float __vectorcall GetRepairAmount(float itemToRepairHealth, float itemUsedHealth)
{
	__asm
	{
		movss	xmm5, SS_1d10
		mulss	xmm0, xmm5
		mulss	xmm1, xmm5
		movss	xmm2, s_pcRepairSkill
		mulss	xmm2, SS_1d100
		movq	xmm3, xmm0
		minss	xmm3, xmm1
		mulss	xmm3, ds:0x11D074C	// fRepairScavengeMult
		maxss	xmm0, xmm1
		movss	xmm1, ds:0x11D0884	// fRepairMin
		movss	xmm4, ds:0x11CFFA4	// fRepairMax
		subss	xmm4, xmm1
		mulss	xmm4, xmm2
		addss	xmm0, xmm1
		addss	xmm0, xmm3
		addss	xmm0, xmm4
		mulss	xmm0, xmm5
		minss	xmm0, xmm2
		retn
	}
}

__declspec(naked) void SetRepairListValuesHook()
{
	__asm
	{
		movss	xmm1, [ebp-0xC]
		movss	xmm0, [ebp-4]
		call	GetRepairAmount
		movss	[ebp-8], xmm0
		JMP_EAX(0x7B592A)
	}
}

__declspec(naked) void DoRepairItemHook()
{
	__asm
	{
		mov		ecx, [ebp+8]
		call	ContChangesEntry::GetHealthPercent
		movq	xmm5, xmm0
		mov		ecx, ds:0x11DA760
		call	ContChangesEntry::GetHealthPercent
		movq	xmm1, xmm5
		call	GetRepairAmount
		movss	[ebp-0x14], xmm0
		JMP_EAX(0x7B5E0B)
	}
}

__declspec(naked) void RepairMenuClickHook()
{
	__asm
	{
		mov		ecx, ds:0x11DA760
		call	ContChangesEntry::GetHealthPercent
		addss	xmm0, PS_V3_One
		comiss	xmm0, s_pcRepairSkill
		JMP_EAX(0x7B5D05)
	}
}

__declspec(naked) void InitMissileFlagsHook()
{
	__asm
	{
		cmp		byte ptr [esp+8], 0
		jz		done
		mov		eax, [ecx+0x20]
		test	byte ptr [eax+0x61], 1
		jz		done
		or		byte ptr [ecx+0xC9], 0x80
	done:
		retn	8
	}
}

__declspec(naked) void QttSelectInventoryHook()
{
	__asm
	{
		mov		ecx, g_DIHookCtrl
		mov		edx, 0x780B39
		cmp		byte ptr [ecx+0xCF], 0
		jnz		dropOne
		cmp		byte ptr [ecx+0x44F], 0
		jnz		dropOne
		cmp		byte ptr [ecx+0x12A], 0
		jnz		dropAll
		cmp		byte ptr [ecx+0x17E], 0
		jnz		dropAll
		mov		eax, 0x11D241C
		JMP_EDX(0x780B13)
	dropOne:
		push	1
		jmp		edx
	dropAll:
		push	esi
		jmp		edx
	}
}

__declspec(naked) void QttSelectContainerHook()
{
	__asm
	{
		mov		ecx, g_DIHookCtrl
		mov		edx, 0x75C038
		cmp		byte ptr [ecx+0xCF], 0
		jnz		dropOne
		cmp		byte ptr [ecx+0x44F], 0
		jnz		dropOne
		cmp		byte ptr [ecx+0x12A], 0
		jnz		dropAll
		cmp		byte ptr [ecx+0x17E], 0
		jnz		dropAll
		mov		eax, 0x11D241C
		JMP_EDX(0x75BFA1)
	dropOne:
		push	1
		jmp		edx
	dropAll:
		push	esi
		jmp		edx
	}
}

__declspec(naked) void QttSelectBarterHook()
{
	__asm
	{
		mov		ecx, g_DIHookCtrl
		mov		edx, 0x72D909
		cmp		byte ptr [ecx+0xCF], 0
		jnz		dropOne
		cmp		byte ptr [ecx+0x44F], 0
		jnz		dropOne
		cmp		byte ptr [ecx+0x12A], 0
		jnz		dropAll
		cmp		byte ptr [ecx+0x17E], 0
		jnz		dropAll
		mov		eax, 0x11D241C
		JMP_EDX(0x72D8BE)
	dropOne:
		push	1
		jmp		edx
	dropAll:
		push	dword ptr [ebp-4]
		jmp		edx
	}
}

__declspec(naked) void VoiceModulationFixHook()
{
	__asm
	{
		mov		eax, 0x104
		mov		ecx, [ebp-0x29C]
		mov		ecx, [ecx+0x20]
		test	byte ptr [ecx+9], 0x20
		jnz		done
		mov		eax, 0x80104
	done:
		mov		[ebp-0x248], eax
		JMP_EAX(0x934AD2)
	}
}

__m128 s_shapeVerticesZ[10];
bhkCharControllerShape *s_pcControllerShape = nullptr;

__declspec(naked) void InitControllerShapeHook()
{
	__asm
	{
		push	0xC72EB3
		mov		eax, 0x1267BE0
		mov		ecx, [esi+0x80]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+0xCC]
		cmp		dword ptr [ecx+0xC], 0x14
		jz		proceed
		cmp		byte ptr [ecx+0x18D], 0
		jz		done
	proceed:
		mov		eax, offset s_pcControllerShape
		cmp		dword ptr [eax], 0
		jnz		done
		mov		eax, offset s_shapeVerticesZ
		mov		ecx, ds:0x1267BE0
		mov		ecx, [ecx+8]
		mov		ecx, [ecx+0x40]
		add		ecx, 0x20
		mov		dl, 5
		xorps	xmm1, xmm1
		movss	xmm2, kFltSix
	iter1Head:
		movups	xmm0, [ecx]
		movaps	[eax], xmm0
		mov		dh, 4
	iter2Head:
		movss	xmm0, [eax]
		comiss	xmm0, xmm1
		jb		iter2Next
		subss	xmm0, xmm2
	iter2Next:
		movss	[eax+0x50], xmm0
		add		eax, 4
		dec		dh
		jnz		iter2Head
		add		ecx, 0x30
		dec		dl
		jnz		iter1Head
		lea		eax, [esp+0x28]
		push	dword ptr [ebx+0x470]
		push	dword ptr [eax]
		add		eax, 0x1C
		push	eax
		sub		eax, 0x14
		push	eax
		CALL_EAX(0xC70DE0)
		add		esp, 0x10
		push	eax
		push	offset s_pcControllerShape
		call	NiReplaceObject
	done:
		push	eax
		lea		ecx, [ebx+0x5A8]
		CALL_EAX(0xC663D0)
		retn
		ALIGN 4
	kFltSix:
		EMIT_DW(0x40C00000)
	}
}

__declspec(naked) bool __fastcall SneakBoundingBoxFixHook(PlayerCharacter *thePlayer)
{
	__asm
	{
		mov		eax, [ecx+0x190]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x94]
		and		eax, 0xC00
		cmp		eax, 0x400
		setz	al
		lea		edx, [ecx+0xD57]
		cmp		[edx], al
		jz		done
		mov		ecx, s_pcControllerShape
		test	ecx, ecx
		jz		done
		mov		[edx], al
		mov		ecx, [ecx+8]
		mov		ecx, [ecx+0x40]
		movzx	edx, al
		neg		edx
		and		edx, 0x50
		add		edx, offset s_shapeVerticesZ
		movaps	xmm0, [edx]
		movups	[ecx+0x20], xmm0
		movaps	xmm0, [edx+0x10]
		movups	[ecx+0x50], xmm0
		movaps	xmm0, [edx+0x20]
		movups	[ecx+0x80], xmm0
		movaps	xmm0, [edx+0x30]
		movups	[ecx+0xB0], xmm0
		movaps	xmm0, [edx+0x40]
		movups	[ecx+0xE0], xmm0
	done:
		retn
	}
}

bool s_NVACLogUpdated = false;
UInt32 s_lastNVACLogSize = 0;

void CheckNVACLog()
{
	if (!s_NVACLogUpdated) return;
	s_NVACLogUpdated = false;
	FileStream srcFile("nvac.log");
	if (!srcFile) return;
	UInt32 length = srcFile.GetLength();
	if (s_lastNVACLogSize >= length) return;
	UInt32 startAt = s_lastNVACLogSize;
	s_lastNVACLogSize = length;
	if (!startAt) return;
	srcFile.SetOffset(startAt);
	length -= startAt + 2;
	char *buffer = GetStrArgBuffer(), *endPtr = CPY_RET_END(buffer, "\n>>>>>>>>>>> BEGIN NVAC REPORT >>>>>>>>>>>\n", 43);
	srcFile.ReadBuf(endPtr, length);
	memcpy(endPtr + length, "\n<<<<<<<<<<<< END NVAC REPORT <<<<<<<<<<<<\n", 44);
	Console_Print(buffer);
	QueueUIMessage("NVAC log updated (see console print).", 0, (const char*)0x1049638, nullptr, 2.5F, 0);
}

__declspec(naked) bool __fastcall CreatureSpreadFixHook(Actor *actor)
{
    __asm
    {
        cmp     byte ptr [ecx+4], kFormType_Creature
        jz      retn1
        mov     ecx, [ecx+0x68]
        test    ecx, ecx
        jz      retn0
		cmp		byte ptr [ecx+0x28], 0
		jnz		retn0
		mov		al, [ecx+0x228]
        retn
    retn1:
        mov     al, 1
        retn
    retn0:
        xor     al, al
        retn
    }
}

bool s_forceHCNeedsUpdate = false;

__declspec(naked) void __fastcall UpdateTimeGlobalsHook(GameTimeGlobals *timeGlobals, int, float secPassed)
{
	alignas(16) static double hourAndDays[] = {0, 0};
	__asm
	{
		movq	xmm4, qword ptr kMults
		cvtss2sd	xmm2, [esp+4]
		mov		eax, [ecx+0x14]
		cvtss2sd	xmm0, [eax+0x24]
		mulsd	xmm2, xmm0
		mulsd	xmm2, kMults+8
		mov		eax, [ecx+0xC]
		cvtss2sd	xmm0, [eax+0x24]
		cmp		byte ptr [ecx+0x1C], 0
		jnz		doRecalc
		movq	xmm1, xmm0
		movaps	xmm3, hourAndDays
		subsd	xmm1, xmm3
		comisd	xmm1, kMults+0x10
		ja		doRecalc
		comisd	xmm1, kMults+0x18
		jb		backWrds
		movaps	xmm0, xmm3
		pshufd	xmm1, xmm0, 0xFE
		jmp		proceed
	backWrds:
		addsd	xmm0, xmm4
	doRecalc:
		mov		[ecx+0x1C], 0
		mov		al, s_forceHCNeedsUpdate
		xor		al, 1
		mov		s_forceHCNeedsUpdate, 0
		mov		edx, g_thePlayer
		mov		[edx+0xE39], al
		movq	xmm1, xmm0
		divsd	xmm1, xmm4
		mov		eax, [ecx+0x10]
		movss	xmm3, [eax+0x24]
		cvttps2dq	xmm3, xmm3
		cvtdq2pd	xmm3, xmm3
		addsd	xmm1, xmm3
	proceed:
		addsd	xmm0, xmm2
		comisd	xmm0, xmm4
		jbe		done
		push	esi
		push	edi
		mov		eax, [ecx+4]
		cvttss2si	esi, [eax+0x24]
		mov		eax, [ecx+8]
		cvttss2si	edi, [eax+0x24]
	getDPM:
		movzx	edx, kDaysPerMonth[esi]
		ALIGN 16
	iterHead:
		comisd	xmm0, xmm4
		jbe		iterEnd
		subsd	xmm0, xmm4
		cmp		edi, edx
		jb		incDay
		mov		edi, 1
		cmp		esi, 0xB
		jb		incMonth
		xor		esi, esi
		mov		edx, 0x1F
		mov		eax, [ecx]
		movss	xmm3, [eax+0x24]
		addss	xmm3, PS_V3_One
		movss	[eax+0x24], xmm3
		jmp		iterHead
	incDay:
		inc		edi
		jmp		iterHead
	incMonth:
		inc		esi
		jmp		getDPM
	iterEnd:
		cvtsi2ss	xmm3, esi
		mov		eax, [ecx+4]
		movss	[eax+0x24], xmm3
		cvtsi2ss	xmm3, edi
		mov		eax, [ecx+8]
		movss	[eax+0x24], xmm3
		inc		dword ptr [ecx+0x18]
		pop		edi
		pop		esi
	done:
		cvtsd2ss	xmm3, xmm0
		mov		eax, [ecx+0xC]
		movss	[eax+0x24], xmm3
		divsd	xmm2, xmm4
		addsd	xmm1, xmm2
		cvtsd2ss	xmm3, xmm1
		mov		eax, [ecx+0x10]
		movss	[eax+0x24], xmm3
		unpcklpd	xmm0, xmm1
		movaps	hourAndDays, xmm0
		retn	4
		ALIGN 8
	kMults:
		EMIT_DW_4(0x00000000, 0x40380000, 0x789ABCDF, 0x3F323456)
		EMIT_DW_4(0x9999999A, 0x3FA99999, 0x9999999A, 0xBFA99999)
	}
}

__declspec(naked) bool __fastcall ModHardcoreNeedsHook(PlayerCharacter *thePlayer, int, UInt32 flag)
{
	__asm
	{
		lea		eax, [ecx+0x5D4]
		cmp		s_patchInstallState.hardcoreNeedsFix, 0
		jz		tracking
		add		ecx, 0x49C
		movups	xmm0, [eax]
		movups	xmm1, [ecx]
		addps	xmm0, xmm1
		xorps	xmm1, xmm1
		maxps	xmm0, xmm1
		movlps	[eax], xmm0
		unpckhpd	xmm0, xmm0
		movss	[eax+8], xmm0
		movlps	[ecx], xmm1
		movss	[ecx+8], xmm1
	tracking:
		test	s_serializedVars.serializedFlags, kSerializedFlag_NoHardcoreTracking
		setnz	dl
		or		[eax+0x865], dl
		test	byte ptr [eax+0xAC], 4
		setnz	al
		retn	4
	}
}

#define OPERATOR_RES_FLT __asm movq qword ptr [ebp-8], xmm0 __asm mov [ebp-0x10], 1 __asm retn __asm ALIGN 16
#define OPERATOR_RES_EAX __asm mov [ebp-8], eax __asm retn __asm ALIGN 16
#define OPERATOR_RES_EDX __asm mov [ebp-8], edx __asm retn __asm ALIGN 16
#define OPERATOR_CMP_FLT(cmpr) __asm xor edx, edx __asm comisd xmm0, xmm1 __asm set##cmpr dl OPERATOR_RES_EDX
#define OPERATOR_CMP_INT(cmpr) __asm xor edx, edx __asm cmp eax, ecx __asm set##cmpr dl OPERATOR_RES_EDX

__declspec(naked) void DoOperatorHook()
{
	__asm
	{
		push	0x5948BB
		mov		[ebp-0x10], 0
		movzx	eax, byte ptr [ebp-0x40]
		shl		al, 1
		or		al, byte ptr [ebp-0x28]
		lea		ecx, kDoOperatorJumpTable[edx*8]
		cmp		edx, 1
		mov		edx, ecx
		jbe		doJump
		jmp		kConvertJumpTable[eax*4]
		ALIGN 16
//	Both int
		mov		eax, [ebp-0x20]
		mov		ecx, [ebp-0x38]
		jmp		dword ptr [edx+4]
		ALIGN 16
//	rvalue int
		movq	xmm0, qword ptr [ebp-0x20]
		cvtsi2sd	xmm1, [ebp-0x38]
		jmp		dword ptr [edx]
		ALIGN 16
//	lvalue int
		cvtsi2sd	xmm0, [ebp-0x20]
		movq	xmm1, qword ptr [ebp-0x38]
		jmp		dword ptr [edx]
		ALIGN 16
//	Both float
		movq	xmm0, qword ptr [ebp-0x20]
		movq	xmm1, qword ptr [ebp-0x38]
	doJump:
		jmp		dword ptr [edx]
		ALIGN 16
//	Operator &&
		xor		edx, edx
		mov		ecx, [ebp-0x20]
		test	al, 1
		jz		lIntAND
		or		ecx, [ebp-0x1C]
	lIntAND:
		test	ecx, ecx
		jz		doneAND
		mov		ecx, [ebp-0x38]
		test	al, 2
		jz		rIntAND
		or		ecx, [ebp-0x34]
	rIntAND:
		test	ecx, ecx
	doneAND:
		setnz	dl
		OPERATOR_RES_EDX
//	Operator ||
		xor		edx, edx
		mov		ecx, [ebp-0x20]
		test	al, 1
		jz		lIntOR
		or		ecx, [ebp-0x1C]
	lIntOR:
		test	ecx, ecx
		jnz		doneOR
		mov		ecx, [ebp-0x38]
		test	al, 2
		jz		rIntOR
		or		ecx, [ebp-0x34]
	rIntOR:
		test	ecx, ecx
	doneOR:
		setnz	dl
		OPERATOR_RES_EDX
//	Operator <= (flt)
		OPERATOR_CMP_FLT(be)
//	Operator <= (int)
		OPERATOR_CMP_INT(le)
//	Operator < (flt)
		OPERATOR_CMP_FLT(b)
//	Operator < (int)
		OPERATOR_CMP_INT(l)
//	Operator >= (flt)
		OPERATOR_CMP_FLT(nb)
//	Operator >= (int)
		OPERATOR_CMP_INT(nl)
//	Operator > (flt)
		OPERATOR_CMP_FLT(nbe)
//	Operator > (int)
		OPERATOR_CMP_INT(nle)
//	Operator == (flt)
		OPERATOR_CMP_FLT(z)
//	Operator == (int)
		OPERATOR_CMP_INT(z)
//	Operator != (flt)
		OPERATOR_CMP_FLT(nz)
//	Operator != (int)
		OPERATOR_CMP_INT(nz)
//	Operator - (flt)
		subsd	xmm0, xmm1
		OPERATOR_RES_FLT
//	Operator - (int)
		sub		eax, ecx
		OPERATOR_RES_EAX
//	Operator + (flt)
		addsd	xmm0, xmm1
		OPERATOR_RES_FLT
//	Operator + (int)
		add		eax, ecx
		OPERATOR_RES_EAX
//	Operator * (flt)
		mulsd	xmm0, xmm1
		OPERATOR_RES_FLT
//	Operator * (int)
		imul	eax, ecx
		OPERATOR_RES_EAX
//	Operator / (flt)
		cmp		dword ptr [ebp-0x34], 0
		jz		div0Flt
		divsd	xmm0, xmm1
	div0Flt:
		OPERATOR_RES_FLT
//	Operator / (int)
		test	ecx, ecx
		jz		div0Int
		cdq
		idiv	ecx
	div0Int:
		OPERATOR_RES_EAX
//	Operator % (flt)
		cvttsd2si	eax, xmm0
		cvttsd2si	ecx, xmm1
		test	ecx, ecx
		cmovz	edx, eax
		jz		mod0Flt
		cdq
		idiv	ecx
	mod0Flt:
		OPERATOR_RES_EDX
//	Operator % (int)
		test	ecx, ecx
		cmovz	edx, eax
		jz		mod0Int
		cdq
		idiv	ecx
	mod0Int:
		OPERATOR_RES_EDX
	kConvertJumpTable:
		EMIT_DW_4(0x30, 0x40, 0x50, 0x60)
	kDoOperatorJumpTable:
		EMIT_DW_4(0x70, 0x70, 0xA0, 0xA0) EMIT_DW_4(0xD0, 0xE0, 0xF0, 0x100) EMIT_DW_4(0x110, 0x120, 0x130, 0x140)
		EMIT_DW_4(0x150, 0x160, 0x170, 0x180) EMIT_DW_4(0x190, 0x1A0, 0x1B0, 0x1C0) EMIT_DW_4(0x1D0, 0x1E0, 0x1F0, 0x210)
		EMIT_DW(0x220) EMIT_DW(0x240)
	}
}

TempObject<UnorderedMap<const char*, UInt32, 0x20, false>> s_optionalHacks;

const char kBigGunsDescription[] = "The Big Guns skill determines your combat effectiveness with all oversized weapons such as the Fat Man, Missile Launcher, Flamer, Minigun, Gatling Laser, etc.";

bool __fastcall SetOptionalPatch(UInt32 patchID, bool bEnable)
{
	switch (patchID)
	{
		default:
		case 0:
			return false;
		case 1:
			if (HOOK_SET(CalculateHitDamage, bEnable))
			{
				if (!bEnable)
				{
					s_patchInstallState.localizedDTDR = false;
					s_VATSHitDT = -FLT_MAX;
				}
				return true;
			}
			return false;
		case 2:
			return HOOK_SET(FO3Repair, bEnable);
		case 3:
		{
			if (s_patchInstallState.bigGunsSkill == bEnable)
				return false;
			s_patchInstallState.bigGunsSkill = bEnable;
			ActorValueInfo *avInfo = ActorValueInfo::Array()[kAVCode_BigGuns];
			avInfo->avFlags = bEnable ? 0x410 : 0x2000;
			SetDescriptionAltText(&avInfo->description, bEnable ? kBigGunsDescription : nullptr);
			return true;
		}
		case 4:
		{
			if (!bEnable || s_patchInstallState.impactDmgFix)
				return false;
			s_patchInstallState.impactDmgFix = true;
			WriteRelCall(0x9B7E14, (UInt32)InitMissileFlagsHook);
			return true;
		}
		case 5:
			return HOOK_SET(UpdateTimeGlobals, bEnable);
		case 6:
		{
			if (s_patchInstallState.hardcoreNeedsFix == bEnable)
				return false;
			s_patchInstallState.hardcoreNeedsFix = bEnable;
			return true;
		}
		case 7:
		{
			if (!bEnable || s_patchInstallState.failedScriptLocks)
				return false;
			s_patchInstallState.failedScriptLocks = true;
			SafeWrite16(0x5E0FBE, 0x7AEB);
			SafeWrite16(0x5E1A23, 0x0DEB);
			SafeWrite16(0x5E1F0B, 0x0DEB);
			return true;
		}
		case 8:
		{
			if (!bEnable || s_patchInstallState.dblPrecision)
				return false;
			s_patchInstallState.dblPrecision = true;
			UInt32 doOperAddr = (UInt32)DoOperatorHook, *jmpTblAddr = (UInt32*)(doOperAddr + 0x250);
			UInt32 oldProtect;
			VirtualProtect(jmpTblAddr, 0x78, PAGE_EXECUTE_READWRITE, &oldProtect);
			for (UInt32 i = 0; i < 30; i++)
				jmpTblAddr[i] += doOperAddr;
			VirtualProtect(jmpTblAddr, 0x78, oldProtect, &oldProtect);
			WriteRelJump(0x593FBC, (UInt32)DoOperatorHook);
			return true;
		}
		case 9:
			return HOOK_SET(QuantitySelect, bEnable);
		case 11:
			if (s_patchInstallState.FO3WpnDegrade == bEnable)
				return false;
			s_patchInstallState.FO3WpnDegrade = bEnable;
			return true;
		case 12:
			if ((s_patchInstallState.localizedDTDR == bEnable) || !HOOK_INSTALLED(CalculateHitDamage))
				return false;
			s_patchInstallState.localizedDTDR = bEnable;
			return true;
		case 13:
			return HOOK_SET(VoiceModulationFix, bEnable);
		case 14:
		{
			if (!bEnable || s_patchInstallState.sneakBoundsFix)
				return false;
			s_patchInstallState.sneakBoundsFix = true;
			WriteRelJump(0xC72EA3, (UInt32)InitControllerShapeHook);
			WriteRelCall(0x770B0E, (UInt32)SneakBoundingBoxFixHook);
			return true;
		}
		case 15:
		{
			if (!bEnable || s_patchInstallState.NVACAlerts)
				return false;
			UInt32 patchAddr = (UInt32)GetModuleHandle("nvac");
			if (!patchAddr || (*(UInt32*)(patchAddr + 0x14B0) != 0xFF102474))
				return false;
			s_patchInstallState.NVACAlerts = true;
			UInt8 buffer[10];
			*(UInt16*)buffer = 0x5C6;
			*(UInt32*)(buffer + 2) = (UInt32)&s_NVACLogUpdated;
			*(UInt32*)(buffer + 6) = 0x8C201;
			SafeWriteBuf(patchAddr + 0x14C0, buffer, sizeof(buffer));
			MainLoopAddCallbackEx(CheckNVACLog, nullptr, 0, 60);
			s_NVACLogUpdated = true;
			return true;
		}
		case 16:
		{
			if (!HOOK_SET(LoadScreenFix, bEnable))
				return false;
			if (bEnable && s_locationLoadScreens->Empty())
			{
				auto lscrIter = g_dataHandler->loadScreenList.Head();
				do
				{
					if (TESLoadScreen *loadScreen = lscrIter->data)
						if (loadScreen->locations.Empty())
							s_genericLoadScreens->Append(loadScreen);
						else s_locationLoadScreens->Append(loadScreen);
				}
				while (lscrIter = lscrIter->next);
			}
			return true;
		}
		case 17:
		{
			if (!bEnable || s_patchInstallState.NPCWeaponMods || !s_uWMChancePerLevel || !s_uWMChanceMin || !s_uWMChanceMax)
				return false;
			s_patchInstallState.NPCWeaponMods = true;
			WriteRelJump(0x45234E, (UInt32)DoOnLoadActorHook);
			WriteRelCall(0x54E2A4, (UInt32)ResetActorFlagsRespawnHook);
			return true;
		}
		case 18:
		{
			if (!bEnable || s_patchInstallState.NPCPerks)
				return false;
			s_patchInstallState.NPCPerks = true;
			BuildValidNPCPerks();
			WriteRelJump(0x45234E, (UInt32)DoOnLoadActorHook);
			WriteRelCall(0x87DE3F, (UInt32)DestroyActorHook);
			WriteRelJump(0x8BCA90, (UInt32)SetPlayerTeammateHook);
			SafeWrite32(0x1086F04, (UInt32)SetPerkRankNPCHook);
			SafeWrite32(0x1087544, (UInt32)SetPerkRankNPCHook);
			SafeWrite32(0x1086F08, (UInt32)RemovePerkNPCHook);
			SafeWrite32(0x1087548, (UInt32)RemovePerkNPCHook);
			SafeWrite32(0x1086F0C, (UInt32)GetPerkRankNPCHook);
			SafeWrite32(0x108754C, (UInt32)GetPerkRankNPCHook);
			SafeWrite32(0x1086F18, (UInt32)GetPerkEntryPointListNPCHook);
			SafeWrite32(0x1087558, (UInt32)GetPerkEntryPointListNPCHook);
			SafeWrite32(0x108AED4, (UInt32)SetPerkRankPlayerHook);
			SafeWrite32(0x108AED8, (UInt32)RemovePerkPlayerHook);
			SAFE_WRITE_BUF(0x5E592F, "\x0F\x1F\x84\x00\x00\x00\x00\x00");
			SafeWrite32(0x64570F, 0x401F0F);
			SAFE_WRITE_BUF(0x89B455, "\x8B\x02\x8B\x48\x68\x85\xC9\x74\x6E\x8B\x01\xFF\x90\x48\x01\x00\x00\x85\xC0\x74\x62\x8B\x48\x08\x89\x8D\x34\xFD\xFF\xFF\xEB\x57");
			SAFE_WRITE_BUF(0x89B50B, "\x8B\x43\x08\x8B\x08");
			return true;
		}
		case 19:
		{
			if (!bEnable || s_patchInstallState.creSpreadFix)
				return false;
			s_patchInstallState.creSpreadFix = true;
			WriteRelCall(0x8B0FBF, (UInt32)CreatureSpreadFixHook);
			return true;
		}
	}
}

TempObject<UnorderedMap<const char*, FontInfo*>> s_fontInfosMap;
char *s_extraFontsPaths[80] = {nullptr};

__declspec(naked) void InitFontManagerHook()
{
	__asm
	{
		cmp		dword ptr ds:0x11F33F8, 0
		jnz		exists
		push	ebp
		push	ebx
		push	esi
		push	edi
		push	0x164
		call	Game_DoHeapAlloc
		mov		g_fontManager, eax
		mov		ebp, eax
		and		dword ptr [eax+0x20], 0
		mov		esi, eax
		mov		edi, 0x11F3418
		mov		ebx, 1
		ALIGN 16
	defFontHead:
		push	0x54
		call	Game_DoHeapAlloc
		push	1
		push	dword ptr [edi]
		push	ebx
		mov		ecx, eax
		CALL_EAX(ADDR_InitFontInfo)
		push	eax
		push	ecx
		push	esp
		push	dword ptr [edi]
		mov		ecx, offset s_fontInfosMap
		call	UnorderedMap<const char*, FontInfo*>::InsertKey
		test	al, al
		pop		ecx
		pop		eax
		jz		defFontNext
		mov		[ecx], eax
	defFontNext:
		mov		[esi], eax
		add		esi, 4
		add		edi, 0xC
		inc		ebx
		cmp		bl, 9
		jb		defFontHead
		mov		ebp, [ebp+8]
		mov		edi, offset s_extraFontsPaths
		ALIGN 16
	extFontHead:
		inc		ebx
		cmp		bl, 0x5A
		jnb		done
		cmp		dword ptr [edi], 0
		jz		useDefault
		push	dword ptr [edi]
		mov		ecx, offset s_fontInfosMap
		call	UnorderedMap<const char*, FontInfo*>::Get
		test	eax, eax
		jnz		extFontNext
		push	0x54
		call	Game_DoHeapAlloc
		push	1
		push	dword ptr [edi]
		push	ebx
		mov		ecx, eax
		CALL_EAX(ADDR_InitFontInfo)
		cmp		dword ptr [eax+0x38], 0
		jz		doFree
		push	eax
		push	ecx
		push	esp
		push	dword ptr [edi]
		mov		ecx, offset s_fontInfosMap
		call	UnorderedMap<const char*, FontInfo*>::InsertKey
		pop		ecx
		pop		eax
		mov		[ecx], eax
		jmp		extFontNext
		ALIGN 16
	doFree:
		push	eax
		call	Game_HeapFree
		ALIGN 16
	useDefault:
		mov		eax, ebp
	extFontNext:
		add		esi, 4
		mov		[esi], eax
		add		edi, 4
		jmp		extFontHead
	done:
		mov		eax, g_fontManager
		mov		ds:0x11F33F8, eax
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
	exists:
		retn
	}
}

bool ProcessCustomINI()
{
	if (s_gameSettingsMap->Empty()) InitSettingMaps();
	char customINIPath[MAX_PATH];
	memcpy(StrCopy(customINIPath, GameGlobals::GetConfigDirectory()), "FalloutCustom.ini", 18);
	if (FileExists(customINIPath))
	{
		char *buffer = GetStrArgBuffer();
		SInt32 namesLen = GetPrivateProfileSectionNames(buffer, kMaxMessageLength, customINIPath);
		char *currName = buffer, *section = buffer + namesLen, settingName[0x100];
		while (namesLen > 0)
		{
			SInt32 sectionLen = GetPrivateProfileSection(currName, section, kMaxMessageLength, customINIPath);
			char *currPair = section;
			while (sectionLen > 0)
			{
				SInt32 pairSize = StrLen(currPair) + 1;
				sectionLen -= pairSize;
				char *delim = GetNextToken(currPair, '='), *endPtr = StrCopy(settingName, currPair);
				*endPtr = ':';
				StrCopy(endPtr + 1, currName);
				currPair += pairSize;
				if (Setting *setting = s_gameSettingsMap->Get(settingName))
					switch (*settingName | 0x20)
					{
						case 'b':
							setting->data.uint = (*delim == '1');
							break;
						case 'i':
							setting->data.i = StrToInt(delim);
							break;
						case 'u':
							setting->data.uint = StrToUInt(delim);
							break;
						case 'f':
							setting->data.f = StrToDbl(delim);
							break;
						case 's':
							setting->Set(delim, false);
							break;
						default:
							break;
					}
			}
			SInt32 nameSize = StrLen(currName) + 1;
			namesLen -= nameSize;
			currName += nameSize;
		}
	}
	return CdeclCall<bool>(0x4DA570);
}

TempObject<UnorderedSet<const char*>> s_overrideBSAFiles;

__declspec(naked) BSArchive* __cdecl LoadBSAFileHook(const char *filename, short arg2, bool isOverride)
{
	__asm
	{
		push	dword ptr [esp+4]
		mov		ecx, offset s_overrideBSAFiles
		call	UnorderedSet<const char*>::HasKey
		mov		[esp+0xC], al
		JMP_EAX(0xAF4BE0)
	}
}

UInt32 s_deferrSetOptional = 0;

void InitGamePatches()
{
	CdeclCall(0x476C00);

	//	Adds NULL pointer checks
	SAFE_WRITE_BUF(0x559450, "\x8B\xC1\x85\xC0\x74\x02\x8B\x00\xC3");
	SAFE_WRITE_BUF(0x5A8ECF, "\x8B\x45\xFC\x8B\x40\x04\x89\x45\xFC\x85\xD2\x74\x0D\xC7\x42\x04\x00\x00\x00\x00\xEB\xCD");
	SAFE_WRITE_BUF(0xB707E0, "\x8B\x4A\x08\x8B\x12\x85\xC9\x74\x26\x66\x81\xB9\x10\x01\x00\x00\xFF\x00\x74\x1B\x8B\xB1\xF8\x00\x00\x00\x85\xF6\x74\x11\x90");

	//	Cmd_Execute >> *result = 0 by default
	SAFE_WRITE_BUF(0x5ACB9D, "\xD9\xEE\xDD\x19\xFF\x75\x20\xFF\x75\x1C\xFF\x75\x18\xFF\x75\xD4");
	SAFE_WRITE_BUF(0x5E16C3, "\x0F\x57\xC0\x66\x0F\xD6\x01\xFF\x75\x08\xFF\x32\x90");
	SAFE_WRITE_BUF(0x5E2329, "\x0F\x57\xC0\x66\x0F\xD6\x02\xFF\x75\x08\xFF\x30\x90");

	//	Earn Steam achievements even if the console has been used
	SafeWrite16(0x5D3B69, 0x9066);

	//	Don't reset Actor::actorFlags for creatures, upon LoadGame
	SafeWrite32(0x8D5003, 0x0004C2C9);

	//	Increase expression-parser buffer size to 0x400
	SafeWrite8(0x593DB6, 7);
	SafeWrite16(0x593E5A, 0xF8B4);

	//	Recognize custom ashpile activators
	for (UInt32 patchAddr : {0x56A30A, 0x573FE8, 0x57445B, 0x576AF0, 0x624958, 0x77765A, 0x95F7FE})
		SAFE_WRITE_BUF(patchAddr, "\x8B\x41\x20\xF6\x40\x06\x20\x0F\x95\xC0\x34\x01\xEB\x0D");
	SAFE_WRITE_BUF(0x573194, "\x8B\x41\x20\xF6\x40\x06\x20\x0F\x95\xC0\x34\x01\xEB\x10");

	//	Increase grass fade distance
	SafeWrite16(0x57D3B9, 0x1AEB);

	//	Increase buffer size for GetPrivateProfileString
	SafeWrite8(0xC44405, 4);

	//	Override display time of default UI messages.
	WriteRelJump(0x70535C, (UInt32)QueueUIMessageHook);

	//	Increase corner message max text length to 0x203 characters
	SafeWrite8(0x7753B8, 4);
	SafeWrite8(0x7753D2, 2);
	SafeWrite8(0x7753EA, 4);
	SafeWrite8(0x77540C, 2);
	SafeWrite8(0x775443, 2);
	SafeWrite8(0x775461, 2);
	SafeWrite8(0x77547E, 2);
	SafeWrite8(0x77549C, 2);
	SafeWrite8(0x7754BC, 3);
	SafeWrite8(0x7754D0, 3);
	SafeWrite8(0x77553C, 2);
	SafeWrite8(0x775600, 4);
	SafeWrite8(0x7714DA, 4);
	SafeWrite8(0x7757B1, 3);
	SafeWrite8(0x7757D3, 3);
	SafeWrite8(0x77582B, 2);

	//	Increase max trait description length to 0x400 characters
	SafeWrite8(0x7E6EA6, 6);
	SafeWrite8(0x7E6F04, 4);
	SafeWrite16(0x7E6F09, 0xE8F9);
	SafeWrite16(0x7E6F2B, 0xE8F9);

	//	Prevent duplicate perk entry points for teammates
	SafeWrite8(0x5EB79B, 0x23);

	//	Fix item value/weight display in the crafting menu
	SAFE_WRITE_BUF(0x728E10, "\x0F\x1F\x84\x00\x00\x00\x00\x00");
	SafeWrite32(0x729616, 0x1030018);

	//	Don't play equip sound if item is already equipped
	SafeWrite8(0x88DCDC, 0);

	//	Fix sun glare texture
	SafeWrite8(0x63E705, 1);

	//	1st person shell casing lifetime fix
	WriteRelJump(0x5251A3, 0x52528C);

	//	TileMenu::DoDestroy fix for OnMenuClose
	SafeWrite16(0xA1F035, 0x30EB);

	//	Dead actors "T-Pose" fix
	SafeWrite8(0x8A19CF, 4);

	//	High-noon fix
	SafeWrite8(0x63F385, 4);
	SafeWrite8(0x63F38C, 1);
	SafeWrite8(0x63F3F7, 1);
	SafeWrite8(0x63F3FE, 4);

	//	FireWeapon crash fix
	SafeWrite8(0x5248EF, 0x77);

	//	NULL check for perk activation text
	SAFE_WRITE_BUF(0x7AA80F, "\x8B\x01\x8B\x40\x04\x85\xC0\x75\x03\xEB\x68");

	//	AI Z fix (credits to Stewie)
	SAFE_WRITE_BUF(0x6B10A3, "\x0F\x1F\x44\x00\x00");
	SAFE_WRITE_BUF(0x881A9C, "\x0F\x1F\x44\x00\x00");
	SAFE_WRITE_BUF(0x9E6A82, "\x0F\x1F\x44\x00\x00");

	//	Fix SetLevel XP
	SafeWrite32(0x8D535F, 0x001F0F50);

	//	ScriptRunner::Run skip Script::GetName calls
	SafeWrite16(0x5E0DD1, 0x39EB);

	//	Store parent TESTopic for every TESTopicInfo
	SafeWrite8(0x468CE7, 0x54);
	WriteRelCall(0x61CD16, (UInt32)InitTopicInfoHook);
	WriteRelCall(0x61E086, (UInt32)LoadTopicInfoHook);
	SAFE_WRITE_BUF(0x61A310, "\x8B\x44\x24\x04\x8B\x40\x50\xC3");
	SAFE_WRITE_BUF(0x61D041, "\x8B\x42\x50\x66\x0F\x1F\x44\x00\x00");

	//	Ignore disabled MusicMarkers
	SAFE_WRITE_BUF(0x9699C5, "\x8B\x01\xF7\x40\x08\x00\x08\x00\x00\x0F\x85\xC0\x00\x00\x00\x83\xC0\x30\x66\x90");

	//	Fix source weapon for kPerkEntry_AdjustExplosionRadius conditions
	SAFE_WRITE_BUF(0x47667D, "\x89\xC8\x0F\x1F\x00");
	SAFE_WRITE_BUF(0x9C35F7, "\x89\xC8\x0F\x1F\x00");

	//	Cap Agility for calculating reload & equip speeds
	SAFE_WRITE_BUF(0x8C17D9, "\xB8\x50\xEF\x66\x00\xFF\xD0\x0F\x1F\x80\x00\x00\x00\x00");
	SAFE_WRITE_BUF(0x8C1959, "\xB8\x50\xEF\x66\x00\xFF\xD0\x0F\x1F\x80\x00\x00\x00\x00");

	//	Fix immobile creatures free-falling to the ground
	WriteRelJump(0x9313EC, (UInt32)MarkCreatureNoFallHook);
	SAFE_WRITE_BUF(0xCD3FDD, "\xA9\x01\x00\x40\x00\x74\x0D\x89\xBE\x20\x05\x00\x00\x80\x8E\x14\x04\x00\x00\x80");

	//	AdjustExplosionRadius perk entry point affects NPCs
	SAFE_WRITE_BUF(0x9ACA23, "\x8B\x8A\xFC\x00\x00\x00\x89\x4D\xE8\x85\xC9\x74\x4B\x8B\x01\x81\xB8\x00\x01\x00\x00\x60\x03\x8D\x00\x75\x3D\x8D\x45\xF0\x50\xFF\xB2\xF8\x00\x00\x00\x51\x6A\x48\xE8\xA0\x8E\xC3\xFF\x83\xC4\x10\xE9\xB5\x00\x00\x00");

	//	Make TESObjectREFR::GetActivationIsCrime return 0 for unlocked teleport doors linked to public cells
	WriteRelJump(0x579942, 0x579AB7);

	//	Update fixed-collision when changing position/rotation
	SafeWrite32(0xC65B47, 0x38F);
	SAFE_WRITE_BUF(0xC65B5B, "\x8B\x4C\x24\x28\xF6\x41\x0C\x40\x0F\x84\x71\x03\x00\x00\x90");

	//	Save the player's killer (reset on game-load)
	SafeWrite8(0x89E790, 8);
	SafeWrite16(0x89F351, 0x0BEB);

	//	PlaySound3D
	SAFE_WRITE_BUF(0x5C240D, "\x8B\x41\x64\x8B\x40\x14\x66\x90");

	//	Enable console prints in the loading menu
	SafeWrite16(0x71D0D2, 0x1BEB);

	//	Inlines
	SafeWrite32(0x47C850, 0x90C3C030);
	SafeWrite32(0x6815C0, 0x90C3C889);
	SafeWrite32(0x8D0360, 0x90C301B0);
	SafeWrite32(0xACBB70, 0x90C3C031);

	//	Fall back to the vanilla CharGenQuest if the INI one is invalid
	WriteRelJump(0x7D333B, (UInt32)InvalidChargenQuestFixHook);

	SafeWrite16(0x94E5F6, 0x18EB);
	WriteRelJump(0x94E4A1, (UInt32)GeneratePlayerNodeHook);
	SafeWrite32(0x1016DB8, (UInt32)DoQueuedPlayerHook);
	SAFE_WRITE_BUF(0x601C30, "\x8B\x41\x04\x85\xC0\x74\x01\xC3\x81\xE9\xD8\x00\x00\x00\xF7\x01\x01\x00\x00\x00\xB8\x48\x3E\x1D\x01\xB9\x1C\x27\x1D\x01\x0F\x45\xC1\x8B\x00\xC3");
	SafeWrite32(0x104A1BC, 0x401290);
	SafeWrite8(0x40F75B, 0x18);

	SafeWrite32(0x9ED3F8, (UInt32)&s_moveAwayDistance);
	SafeWrite32(0x9ED528, (UInt32)&s_moveAwayDistance);

	SafeWrite8(0xA5B630, 0xC3);
	WriteRelJump(0xA5B690, (UInt32)GetNiFixedString);
	WriteRelCall(0x878203, (UInt32)FreeUnusedNiFixedStrings);

	//	For ExtraJIP framework
	SafeWrite8(0x40F9D7, 0);
	SafeWrite8(0x41D0F6, 3);
	SafeWrite8(0x41D212, 1);
	SafeWrite8(0x41D25A, 1);
	WriteRelCall(0x4C21EB, (UInt32)SetPickedCountHook);
	WriteRelCall(0x4C2291, (UInt32)CopyExtraListHook);
	SafeWrite32(0x10158FC, (UInt32)ExtraJIPDestroyHook);
	SafeWrite32(0x1015900, (UInt32)ExtraJIPDiffersHook);
	SafeWrite32(0x411978, (UInt32)ExtraJIPCreateCopyHook);
	SafeWrite32(0x42C328, (UInt32)ExtraJIPLoadGameHook);
	SafeWrite32(0x427F30, (UInt32)ExtraJIPSaveGameHook);

	SafeWrite8(0x484E03, 0xD1);
	WriteRelCall(0x8497CD, (UInt32)GetShouldSaveUnloadedFormHook);
	SafeWrite16(0x845558, 0x7D63);
	SafeWrite16(0x846FFB, 0x7D63);

	//	Inventory invisible stack fix
	SafeWrite8(0x41271E, 0xEB);
	SafeWrite32(0x4BF630, 0x4C2);
	WriteRelJump(0x4CA200, (UInt32)CreateInventoryIteratorHook);
	WriteRelJump(0x4CAFA3, (UInt32)InventoryIterGetNextHook);

	WriteRelJump(0x470D56, (UInt32)InitModInfoNameHook);
	SAFE_WRITE_BUF(0x47079D, "\x66\xC7\x80\x9A\x02\x00\x00\x00\x00\x90");
	SAFE_WRITE_BUF(0x4F15A0, "\x0F\xB6\x44\x24\x04\x89\x41\x04\x3C\x3B\x74\x07\x3C\x3C\x74\x03\xC2\x04\x00\x31\xD2\x89\x91\x04\x01\x00\x00\xC2\x04\x00");
	SafeWrite32(0x939258, 0xD54A183);
	//SafeWrite32(0x9394AE, 0xE08A083);
	WriteRelJump(0x482090, (UInt32)SetContainerItemsHealthHook);
	SafeWrite16(0x5B4DBC, 0x19EB);
	WriteRelJump(0x5A64D7, (UInt32)TESGlobalLoadFormHook);
	WriteRelCall(0x8A0DAB, (UInt32)DetectionTeammateHook);
	WriteRelCall(0x8A1291, (UInt32)DetectionTeammateHook);
	WriteRelCall(0x8A131D, (UInt32)DetectionTeammateHook);
	WriteRelCall(0x8A147C, (UInt32)DetectionGetLevelHook);
	WriteRelCall(0x5FA977, (UInt32)WheelTeammateHook);
	WriteRelCall(0x5FB122, (UInt32)WheelTeammateHook);
	WriteRelCall(0x607C8E, (UInt32)WheelTeammateHook);
	WriteRelCall(0x7A9AEE, (UInt32)MaxMessageWidthHook);
	WriteRelCall(0x7A9C45, (UInt32)MaxMessageWidthHook);
	WriteRelCall(0x72D661, (UInt32)InitBarterMenuHook);
	WriteRelCall(0x512F3D, (UInt32)InitArmorFormHook);
	WriteRelCall(0x512FAF, (UInt32)InitArmorFormHook);
	WriteRelCall(0x70E5FE, (UInt32)ToggleConsoleHook);
	WriteRelJump(0xEE820A, (UInt32)GamePadRumbleHook);
	WriteRelJump(0x5B54E7, (UInt32)RemoveMeHook);
	WriteRelCall(0x41C833, (UInt32)IsPersistentHook);
	WriteRelCall(0x4C232C, (UInt32)IsPersistentHook);
	WriteRelCall(0x4C273A, (UInt32)IsPersistentHook);
	WriteRelJump(0x8DAA6F, (UInt32)PackageSetRunHook);
	SAFE_WRITE_BUF(0x8AB6D9, "\x8B\x8D\x54\xFF\xFF\xFF\x66\xC7\x81\x05\x01\x00\x00\x00\x00\x80\xA1\x43\x01\x00\x00\xBF\x0F\x1F\x40\x00");
	WriteRelCall(0x73005C, (UInt32)BarterSellFixHook);
	WriteRelJump(0xCD4468, (UInt32)JumpHeightFixHook);
	SAFE_WRITE_BUF(0x80ECA8, "\x85\xC0\x74\x26\x89\xC1\x6A\x01\x8B\x45\xA0\x83\xC0\x48\xFF\x30\x8B\x01\xFF\x90\xDC\x00\x00\x00\xEB\x10");
	WriteRelJump(0x4069D4, (UInt32)EffectSkillModifiers1Hook);
	WriteRelJump(0x816519, (UInt32)EffectSkillModifiers2Hook);
	WriteRelJump(0x7294DC, (UInt32)RecipeMenuAmmoEffectsHook);
	WriteRelJump(0x7AC179, (UInt32)QttMenuEnableWheelHook);
	WriteRelCall(0x59D0B3, (UInt32)TerminalGetLockedHook);
	SAFE_WRITE_BUF(0x59E034, "\x8B\x41\x68\x85\xC0\x74\x11\x89\xC1\x8B\x01\xFF\x90\xC8\x04\x00\x00\x0F\x1F\x80\x00\x00\x00\x00");
	WriteRelJump(0x947DD2, (UInt32)BuildMusicMarkerListHook);
	WriteRelJump(0x5DBCFD, (UInt32)AttachAshPileHook);
	WriteRelCall(0x63C4D6, (UInt32)GetWindSpeedHook);
	WriteRelCall(0x63C52C, (UInt32)GetWindSpeedHook);
	WriteRelJump(0x4C3976, (UInt32)HotkeyFixRemoveItemHook);
	WriteRelJump(0x95400F, (UInt32)HotkeyFixPlayerActivateHook);
	SafeWrite8(0x412675, 1);
	SAFE_WRITE_BUF(0x9B6335, "\xF3\x0F\x10\x45\xF4\x0F\x2F\x41\x14\x0F\x86\x8E\x00\x00\x00\x80\x49\x5B\x80\xF3\x0F\x11\x41\x14\x0F\x1F\x40\x00");
	WriteRelJump(0x5674D5, (UInt32)SetScaleHook);
	SafeWrite16(0x567709, 0x15EB);
	WriteRelJump(0x4BD820, (UInt32)GetEntryDataModFlagsHook);
	WriteRelJump(0x4BDA70, (UInt32)GetEntryDataHasModHook);
	WriteRelJump(0x777123, (UInt32)RolloverWeaponHook);
	WriteRelCall(0x571F65, (UInt32)ClearWeaponNodeHook);
	WriteRelCall(0x571FDF, (UInt32)ClearWeaponNodeHook);
	WritePushRetRelJump(0x782876, 0x7828F8, (UInt32)ConstructItemEntryNameHook);
	WritePushRetRelJump(0x75D1A5, 0x75D1EB, (UInt32)ConstructItemEntryNameHook);
	SAFE_WRITE_BUF(0x72F337, "\x8B\x85\x70\xFF\xFF\xFF");
	WritePushRetRelJump(0x72F33D, 0x72F37F, (UInt32)ConstructItemEntryNameHook);
	WriteRelCall(0x48E761, (UInt32)GetIconPathForItemHook);
	WriteRelJump(0x646D00, (UInt32)GetConditionDamagePenaltyHook);
	WriteRelJump(0x646360, (UInt32)GetArmorConditionPenaltyHook);
	WriteRelCall(0x406720, (UInt32)GetEffectHiddenHook);
	WriteRelJump(0x79DF73, (UInt32)RefreshMapMenuHook);
	SafeWrite8(0xA081A8, 6);
	SafeWrite32(0xA08718, (UInt32)ProcessGradualSetFloatHook);
	SafeWrite32(0xA0871C, (UInt32)ProcessGradualSetFloatHook);
	SAFE_WRITE_BUF(0x87A12A, "\x31\xD2\x66\x89\x50\x26\x89\x50\x28\xC9\xC2\x08\x00");
	WriteRelJump(0x454576, (UInt32)CloudsFixHook);
	WriteRelCall(0x9D10F3, (UInt32)ReactionCooldownCheckHook);
	WriteRelJump(0x9F50C4, (UInt32)IsValidAITargetHook);
	WriteRelJump(0x4AB400, (UInt32)SetWeaponSlotHook);
	WriteRelJump(0x6061D9, (UInt32)InitWornObjectHook);
	WriteRelJump(0x5719ED, (UInt32)PickWeaponModelHook);
	WriteRelJump(0x83AD97, (UInt32)PlayAttackSoundHook);
	WriteRelCall(0x9BDC92, (UInt32)AddProjectileLightHook);
	WriteRelCall(0x9BC6D9, (UInt32)RemoveProjectileLightHook);
	/*WriteRelCall(0x9AD024, (UInt32)AddExplosionLightHook);
	WriteRelCall(0x9AC6A5, (UInt32)RemoveExplosionLightHook);*/
	WriteRelJump(0x9BB906, (UInt32)SetMuzzleFlashFadeHook);
	WriteRelCall(0x9AE68C, (UInt32)SetExplosionLightFadeHook);
	WriteRelCall(0x89DC0E, (UInt32)KillChallengeFixHook);
	WriteRelJump(0x89D2FF, (UInt32)DoHitArmorDamageHook);
	WriteRelCall(0x72880D, (UInt32)IsDisposableWeaponHook);
	//SafeWrite16(0x8EC59B, 0x53EB);
	WriteRelJump(0x984156, (UInt32)DeathResponseFixHook);
	SAFE_WRITE_BUF(0x9EE367, "\x8B\x8A\xB0\x00\x00\x00\x83\xB9\x08\x01\x00\x00\x02\x0F\x84\x82\x00\x00\x00\xEB\x41");
	SafeWrite16(0x54E421, 0x1EEB);
	WriteRelJump(0x54E4E1, (UInt32)ClearAshPilesHook);
	WriteRelJump(0x82D8CA, (UInt32)SetAcousticSpaceFixHook);
	WriteRelJump(0x93B87F, (UInt32)DamagePCFatigueMinFixHook);
	SafeWrite32(0x4E4000, 0x00401F0F);
	WriteRelCall(0x4E400E, (UInt32)UpdateWaterTexturesHook);
	WriteRelCall(0x7E0C1F, (UInt32)EffectItemGetModifiedDuration);
	WriteRelJump(0x8236FD, (UInt32)ModPositiveChemDurationHook);
	SAFE_WRITE_BUF(0xAEACAA, "\x31\xD2\x89\x90\x34\x01\x00\x00\x89\x90\x9C\x01\x00\x00\x89\x90\xA0\x01\x00\x00\x89\x90\xDC\x01\x00\x00\xC7\x80\x38\x01\x00\x00\x00\x00\x80\x3F\x0F\x1F\x00");
	SafeWrite32(0xAE5648, (UInt32)SetSourceSoundRequestHook);
	WriteRelJump(0x82D416, (UInt32)FillGameSoundPropsHook);
	SafeWrite32(0x10A3C58, (UInt32)AdjustSoundFrequencyHook);
	SafeWrite32(0x10A3C5C, (UInt32)GetSoundFrequencyPercHook);
	WriteRelJump(0x58E9D0, (UInt32)GetImpactDataHook);
	WriteRelCall(0x5956BC, (UInt32)GetFactionReactionHook);
	WriteRelJump(0x646260, (UInt32)GetDamageToWeaponHook);
	GMST_INT(fRepairSkillMax) = 0x41200000;
	WriteRelCall(0x4F49AB, (UInt32)LoadMediaSetIDHook);
	WriteRelCall(0x4F49D2, (UInt32)LoadMediaSetIDHook);
	WriteRelJump(0x595560, (UInt32)PickMediaSetHook);
	SafeWrite32(0x1023DD8, (UInt32)PickBattleMediaSetHook);
	WriteRelCall(0x969C41, (UInt32)ModHardcoreNeedsHook);
	WriteRelCall(0x86AC60, (UInt32)ProcessCustomINI);

	WriteRelJump(0x9B6E81, (UInt32)SetHitLocationHook);
	WriteRelJump(0x9BC241, (UInt32)InitProjectileSetAmmoHook);
	SafeWrite16(0x8C227F, 0x9066);
	WriteRelJump(0x89ABDC, (UInt32)RunAmmoImpactScriptHook);
	WriteRelJump(0x88EBAA, (UInt32)CreateHitBloodHook);
	WriteRelCall(0x7ECE32, (UInt32)GetVATSTargetDTHook);
	SafeWrite32(0x108A9B4, (UInt32)PlayerCannotBeHitHook);
	WriteRelJump(0x5D2860, (UInt32)GetHitIMODHook);
	WriteRelCall(0x9ACEC6, (UInt32)GetExplosionInFaceIMODHook);
	WriteRelJump(0x9AD2C5, (UInt32)ExplosionScreenShakeHook);
	WriteRelCall(0x8A04B2, (UInt32)GetControlFlagHook);
	WriteRelJump(0x9C3BD6, (UInt32)MineExplodeChanceHook);
	WriteRelCall(0x8751C6, (UInt32)RendererClearBufferHook);

	HOOK_INIT_JUMP(0x9B5A30, CalculateHitDamage);
	HOOK_INIT_LIST(FO3Repair, {0x7818B2, EnableRepairButtonHook, 5, 0xE9}, {0x4D4C11, PopulateRepairListHook, 5, 0xE9},
		{0x7B58DB, SetRepairListValuesHook, 5, 0xE9}, {0x7B5DAD, DoRepairItemHook, 5, 0xE9}, {0x7B5CED, RepairMenuClickHook, 5, 0xE9},
		{0x7818D3, 0x77UL, 1, 0}, {0x7B5D05, 0x76UL, 1, 0});
	HOOK_INIT_LIST(QuantitySelect, {0x780B09, QttSelectInventoryHook, 5, 0xE9}, {0x75BF97, QttSelectContainerHook, 5, 0xE9},
		{0x72D8B4, QttSelectBarterHook, 5, 0xE9}, {0x75BF18, 0x63EBUL, 2, 0});
	HOOK_INIT_LIST(VoiceModulationFix, {0x934AC8, VoiceModulationFixHook, 5, 0xE9}, {0x578E16, 8UL, 1, 0}, {0x7974CC, 8UL, 1, 0});
	HOOK_INIT_LIST(LoadScreenFix, {0x78AC60, GetSuitableLoadScreensHook, 5, 0xE9}, {0x78A79B, PickLoadScreenHook, 5, 0xE9});
	HOOK_INIT_JUMP(0x867A40, UpdateTimeGlobals);

	//char filePath[0x80] = "Data\\NVSE\\plugins\\xfonts\\*.txt", dataPath[0x80] = "data\\", *buffer = GetStrArgBuffer();

	char filePath[0x80], dataPath[0x80], *buffer = GetStrArgBuffer();
	memcpy(filePath, "Data\\NVSE\\plugins\\xfonts\\*.txt", 31);
	memcpy(dataPath, "data\\", 6);
	for (DirectoryIterator dirIter(filePath); dirIter; ++dirIter)
	{
		if (!dirIter.IsFile()) continue;
		StrCopy(filePath + 25, *dirIter);
		for (LineIterator lineIter(filePath, buffer); lineIter;)
		{
			char *dataPtr = *lineIter;
			++lineIter;
			if (char *delim = GetNextToken(dataPtr, '='); *delim)
				if (UInt32 index = StrToInt(dataPtr) - 10; (index <= 79) && !s_extraFontsPaths[index])
					if (StrCopy(dataPath + 5, delim); FileExists(dataPath))
						s_extraFontsPaths[index] = CopyString(delim);
		}
	}
	WriteRelCall(0x70B285, (UInt32)InitFontManagerHook);
	SafeWrite8(0xA1B032, 0x59);

	for (DirectoryIterator dirIter("Data\\*.override"); dirIter; ++dirIter)
		if (dirIter.IsFile())
		{
			memcpy(StrCopy(dataPath + 5, *dirIter) - 8, "bsa", 4);
			s_overrideBSAFiles->Insert(dataPath);
		}
	if (!s_overrideBSAFiles->Empty())
		WriteRelCall(0x463855, (UInt32)LoadBSAFileHook);

	SInt32 lines = GetPrivateProfileSection("GamePatches", buffer, 0x10000, "Data\\NVSE\\plugins\\jip_nvse.ini");
	if (lines <= 0)
		for (UInt32 index : {1, 4, 5, 6, 7, 8, 9, 13, 14})
			SetOptionalPatch(index, true);
	else while (lines > 0)
	{
		char *delim = GetNextToken(buffer, '=');
		if (UInt32 index = s_optionalHacks->Get(buffer))
			if (index >= 20)
			{
				UInt32 value = StrToInt(delim);
				switch (index)
				{
				case 20:
					s_uWMChancePerLevel = value;
					break;
				case 21:
					s_uWMChanceMin = value;
					break;
				case 22:
					s_uWMChanceMax = value;
					break;
				}
			}
			else if (*delim != '0')
			{
				if ((index == 18) && (*delim == '2'))
					s_NPCPerksAutoAdd = true;
				if ((index == 3) || (index >= 15))
					s_deferrSetOptional |= 1 << index;
				else SetOptionalPatch(index, true);
			}
		SInt32 size = StrLen(buffer) + 1;
		buffer += size;
		lines -= size;
	}

	//	Fix Enhanced Camera ground sinking bug when switching to 3rd person
	if (UInt32 enCamAddr = (UInt32)GetModuleHandle("NVSE_EnhancedCamera"); enCamAddr && (*(UInt32*)(enCamAddr + 0x1815) == 0x5F605ED9))
		SafeWrite32(enCamAddr + 0x1815, 0x5F90D8DD);
}

NiCamera* __fastcall GetSingletonsHook(SceneGraph *sceneGraph)
{
	g_TLSIndex = *(UInt32*)0x126FD98;
	g_modelLoader = ModelLoader::GetSingleton();
	g_dataHandler = DataHandler::GetSingleton();
	g_loadedReferences = LoadedReferenceCollection::Get();
	g_interfaceManager = InterfaceManager::GetSingleton();
	g_OSGlobals = OSGlobals::GetSingleton();
	g_TES = TES::GetSingleton();
	g_gridCellArray = g_TES->gridCellArray;
	g_currentSky = Sky::Get();
	g_BGSSaveLoadGame = BGSSaveLoadGame::GetSingleton();
	g_thePlayer = PlayerCharacter::GetSingleton();
	g_inputGlobals = OSInputGlobals::GetSingleton();
	g_scrapHeapQueue = TaskQueueInterface::GetSingleton();
	g_tileMenuArray = *(TileMenu***)0x11F350C - kMenuType_Min;
	return g_mainCamera = sceneGraph->camera;
}

void DeferredInit()
{
	g_HUDMainMenu = HUDMainMenu::Get();
	g_consoleManager = ConsoleManager::GetSingleton();
	g_cursorNode = g_interfaceManager->cursor->node;
	float converter = *(float*)0x11D8A48 * 0.5F;
	g_screenWidth = INIS_INT(iSize_W_Display) * converter;
	g_screenHeight = INIS_INT(iSize_H_Display) * converter;
	g_shadowSceneNode = *(ShadowSceneNode**)0x11F91C8;
	g_terminalModelDefault = *GameGlobals::TerminalModelPtr();
	g_attachLightString = **(const char***)0x11C620C;
	GameTimeGlobals *timeGlobals = GameTimeGlobals::Get();
	g_gameYear = timeGlobals->year;
	g_gameMonth = timeGlobals->month;
	g_gameDay = timeGlobals->day;
	g_gameHour = timeGlobals->hour;
	g_timeScale = timeGlobals->timeScale;
	g_capsItem = (TESObjectMISC*)LookupFormByRefID(0xF);
	g_getHitIMOD = (TESImageSpaceModifier*)LookupFormByRefID(0x162);
	g_explosionInFaceIMOD = (TESImageSpaceModifier*)LookupFormByRefID(0x166);
	g_fistsWeapon = *(TESObjectWEAP**)0x11CA278;
	g_ashPileACTI = *(TESObjectACTI**)0x11CA27C;
	g_gooPileACTI = *(TESObjectACTI**)0x11CA280;
	g_ashPileACTI->SetJIPFlag(kHookFormFlag6_IsAshPile, true);
	g_gooPileACTI->SetJIPFlag(kHookFormFlag6_IsAshPile, true);

	s_mainThreadID = g_OSGlobals->mainThreadID;
	s_initialTickCount = CdeclCall<UInt32>(0x457FE0);

	SafeWrite32(0x102F5A4, (UInt32)MarkRefAsModifiedHook);

	s_overrideBSAFiles->Destroy();

	s_tempPosMarker = ThisCall<TESObjectREFR*>(0x55A2F0, Game_HeapAlloc<TESObjectREFR>());
	ThisCall(0x484490, s_tempPosMarker);

	ThisCall(0x970D50, ProcessManager::Get());

	CommandInfo *eventCmdInfos = (CommandInfo*)0x118E2F0;
	eventCmdInfos[1].execute = Hook_MenuMode_Execute;
	eventCmdInfos[0xE].execute = (Cmd_Execute)0x5D4A40;

	if (GetModuleHandle("nvac"))
		if (FILE *nvacLog = _fsopen("nvac.log", "ab", _SH_DENYWR))
		{
			fprintf(nvacLog, "NVSE version: %.2f\tJIP LN version: %.2f\tBase address: %08X\n", s_nvseVersion, JIP_LN_VERSION, (UInt32)GetModuleHandle("jip_nvse"));
			fclose(nvacLog);
		}

	for (UInt32 index = 1; index <= 19; index++)
		if (s_deferrSetOptional & (1 << index))
			SetOptionalPatch(index, true);

	if (HookXInput())
	{
		for (UInt32 patchAddr : {0x7416D6, 0x741705, 0x790966, 0x799861, 0x7BD47D, 0x7BD4A0, 0x7BD4DD, 0x94AB59, 0x94AB88})
			SafeWrite32(patchAddr, (UInt32)&s_deadZoneLSg);
		SafeWrite32(0x94A93C, (UInt32)&s_deadZoneRSg);
		SafeWrite32(0x94A95F, (UInt32)&s_deadZoneRSg);
	}

	FontInfo **fontInfos = g_fontManager->fontInfos;
	for (UInt32 length = 1; length < 90; length++, fontInfos++)
	{
		if (length == 9) continue;
		s_fontHeightDatas[length].heightBase = (*fontInfos)->bufferData->charDimensions[' '].height;
		s_fontHeightDatas[length].heightwGap = (*fontInfos)->bufferData->lineHeight;
	}
	s_fontHeightDatas[2].heightwGap += 4.0F;
	s_fontHeightDatas[3].heightwGap += 6.0F;
	s_fontHeightDatas[6].heightwGap += 4.0F;
	s_fontHeightDatas[7].heightwGap += 2.0F;

	BSWin32Audio::Get()->PickSoundFileFromFolder = PickSoundFileFromFolderHook;

	ActorValueInfo *avInfo = ActorValueInfo::Array()[kAVCode_BigGuns];
	avInfo->fullName.name = avInfo->avName;
	avInfo->callback4C = (void*)0x643BD0;

	s_alphaProperty = NiAlphaProperty::Create();
	s_alphaProperty->flags = 0x10ED;

	s_LIGH_EDID() = "LIGH_EDID";

	UInt8 modIdx = g_dataHandler->GetModIndex("JIP Companions Command & Control.esp");
	if (modIdx != 0xFF)
		if (Script *cccMain = (Script*)LookupFormByRefID((modIdx << 24) | 0xADE); cccMain && (*(UInt32*)(cccMain->data + 0x29C) == ' 931'))
			*(UInt32*)(cccMain->data + 0x29C) = ' 552';

	Console_Print("JIP LN version: %.2f", JIP_LN_VERSION);

	JIPScriptRunner::Init();
}

void InitEditorPatches()
{
	//	Make DefaultCmdParse accept exterior cells with kParamType_Cell
	WriteRelJump(0x5C6C1F, 0x5C7A01);

	SafeWriteBuf(0xD61660, "GetNVSEVersionFull", 18);
}