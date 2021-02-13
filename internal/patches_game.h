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

__declspec(naked) void DoQueuedPlayerHook()
{
	__asm
	{
		cmp		s_queuedPlayerLock, 0
		jnz		proceed
		JMP_EAX(0x440BA0)
	proceed:
		push	esi
		push	ecx
		mov		ecx, [ecx+0x28]
		mov		esi, ecx
		push	1
		push	0
		CALL_EAX(0x5702E0)
		mov		ecx, [esi+0x68C]
		test	ecx, ecx
		jz		done68C
		push	1
		CALL_EAX(0x418E00)
	done68C:
		mov		ecx, [esi+0x690]
		test	ecx, ecx
		jz		done690
		push	1
		CALL_EAX(0x418D20)
	done690:
		mov		ecx, [esi+0x694]
		test	ecx, ecx
		jz		doneNode
		call	NiReleaseObject
	doneNode:
		xor		edx, edx
		mov		[esi+0x68C], edx
		mov		[esi+0x690], edx
		mov		[esi+0x694], edx
		mov		eax, 0x11E07D0
		mov		[eax], edx
		mov		[eax+4], edx
		mov		[eax+8], edx
		lea		ecx, [esi+0xD74]
		pxor	xmm0, xmm0
		movups	[ecx], xmm0
		movups	[ecx+0x10], xmm0
		movups	[ecx+0x20], xmm0
		movups	[ecx+0x30], xmm0
		movups	[ecx+0x40], xmm0
		movups	[ecx+0x50], xmm0
		pop		ecx
		CALL_EAX(0x440BA0)
		mov		eax, [esi+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		mov		ecx, [esi+0x694]
		test	ecx, ecx
		jz		done
		mov		edx, ecx
		cmp		byte ptr [esi+0x64A], 0
		cmovz	ecx, eax
		cmovz	eax, edx
		or		dword ptr [ecx+0x30], 1
		and		dword ptr [eax+0x30], 0xFFFFFFFE
		mov		ecx, [esi+0x68]
		test	ecx, ecx
		jz		done
		cmp		dword ptr [ecx+0x28], 0
		jnz		done
		cmp		byte ptr [ecx+0x135], 0
		jz		reEquip
		push	0
		push	esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x458]
		jmp		done
	reEquip:
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		done
		push	0
		mov		edx, [eax]
		push	dword ptr [edx]
		push	dword ptr [eax+4]
		push	dword ptr [eax+8]
		mov		ecx, esi
		CALL_EAX(0x88DB20)
	done:
		mov		s_queuedPlayerLock, 0
		pop		esi
		retn
	}
}

__declspec(naked) void TESGlobalLoadFormHook()
{
	__asm
	{
		test	dword ptr [ecx+8], 0x40
		jz		contRetn
		JMP_EAX(0x5A65BD)
	contRetn:
		mov		[ebp-0x10], ecx
		mov		ecx, [ebp+8]
		JMP_EAX(0x5A64C6)
	}
}

__declspec(naked) bool SetNPCModelHook()
{
	__asm
	{
		mov		al, 1
		retn	4
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
		call	dword ptr [eax+0x28]
		retn
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

Script *s_messageBoxScript = NULL;

__declspec(naked) float MaxMessageWidthHook()
{
	static const float maxWidth = 1200.0F;
	__asm
	{
		cmp		s_messageBoxScript, 0
		jnz		overrideWidth
		JMP_EAX(kAddr_TileGetFloat)
	overrideWidth:
		fld		maxWidth
		retn	4
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
		push	ebp
		mov		ebp, esp
		push	0xFFFFFFFF
		mov		ecx, [ebp]
		mov		ecx, [ecx-0xC]
		CALL_EAX(0x484E60)
		push	eax
		lea		eax, [ebp+8]
		push	eax
		CALL_EAX(0x485D50)
		add		esp, 8
		push	dword ptr [ebp+8]
		call	LookupFormByRefID
		leave
		retn
	}
}

__declspec(naked) UInt32 ToggleConsoleHook()
{
	__asm
	{
		add		ecx, 0x29
		test	byte ptr [ecx+0x19F8], 0x80
		jnz		retnFalse
		test	byte ptr [ecx+0x18F8], 0x80
		setnz	al
		movzx	eax, al
		retn	8
	retnFalse:
		xor		eax, eax
		retn	8
	}
}

__declspec(naked) UInt32 GamePadRumbleHook()
{
	__asm
	{
		cmp		byte ptr ds:[0x11E0854], 0
		jz		noRumble
		jmp		dword ptr ds:[0xFDF398]
	noRumble:
		xor		eax, eax
		retn	8
	}
}

__declspec(naked) void GetMouseWheelHook()
{
	__asm
	{
		mov		eax, [ecx+0x1B2C]
		test	eax, eax
		jz		done
		mov		ecx, g_DIHookCtrl
		jns		wheelUp
		cmp		word ptr [ecx+0x748], 0
		jnz		retnZero
		jmp		done
	wheelUp:
		cmp		word ptr [ecx+0x741], 0
		jz		done
	retnZero:
		xor		eax, eax
	done:
		JMP_EDX(0xA23A26)
	}
}

void RemoveMeHook()
{
	if (!FindMainLoopCallback(RefreshItemListBox))
		MainLoopAddCallback(RefreshItemListBox);
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
		jmp		done
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
	done:
		leave
		retn	0x18
	}
}

__declspec(naked) void ResetActorFlagsHook()
{
	__asm
	{
		mov		ecx, [ebp-0xAC]
		mov		word ptr [ecx+0x105], 0
		and		dword ptr [ecx+0x140], 0xBFFFFFFF
		retn
	}
}

__declspec(naked) void __fastcall FixVendorCaps(ExtraContainerChanges::Data *changesData)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 8
		mov		ecx, g_dataHandler
		mov		ecx, [ecx+0x628]
		mov		ecx, [ecx]
		mov		edx, g_capsItem
		call	ExtraContainerChanges::EntryDataList::FindForItem
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		mov		[ebp-8], eax
		mov		ecx, [ebp-4]
		mov		eax, [ecx+4]
		mov		[ebp-0xC], eax
		mov		ecx, [ecx]
		mov		edx, g_capsItem
		call	ExtraContainerChanges::EntryDataList::FindForItem
		test	eax, eax
		jz		done
		mov		edx, [eax+4]
		cmp		edx, 0x7FFF
		jle		done
		push	esi
		push	edi
		mov		esi, [ebp-8]
		mov		edi, edx
	countHead:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		countNext
		push	kExtraData_OriginalReference
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		countNext
		mov		eax, [eax+0xC]
		cmp		[ebp-0xC], eax
		jnz		countNext
		push	kExtraData_Count
		mov		ecx, [esi]
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		addHead
		mov		ecx, 0x7FFF
		cmp		edi, ecx
		cmovl	ecx, edi
		mov		word ptr [eax+0xC], cx
		sub		edi, ecx
		jmp		addHead
	countNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		countHead
	addHead:
		cmp		edi, 1
		jle		doPop
		push	dword ptr [ebp-0xC]
		call	ExtraOriginalReference::Create
		push	eax
		call	ExtraDataList::Create
		mov		esi, eax
		mov		eax, 0x7FFF
		cmp		edi, eax
		cmovl	eax, edi
		sub		edi, eax
		push	eax
		call	ExtraCount::Create
		push	eax
		mov		ecx, esi
		CALL_EAX(kAddr_AddExtraData)
		push	esi
		mov		ecx, [ebp-8]
		call	ExtraContainerChanges::ExtendDataList::Append
		jmp		addHead
	doPop:
		pop		edi
		pop		esi
	done:
		leave
		retn
	}
}

__declspec(naked) void BarterSellFixHook()
{
	__asm
	{
		mov		ecx, [esp-4]
		mov		ecx, [ecx-0x1C]
		call	FixVendorCaps
		mov		ecx, g_dataHandler
		JMP_EAX(0x46F640)
	}
}

__declspec(naked) void JumpHeightFixHook()
{
	static const UInt32 kFlt7 = 0x40E00000;
	__asm
	{
		add		esp, 8
		mov		dword ptr [esi+0x3F0], 2
		mov		dword ptr [esi+0x520], 0xB
		movss	xmm0, [esi+0x540]
		mulss	xmm0, kFlt7
		addss	xmm0, [esp+0x28]
		movss	[esi+0x544], xmm0
		JMP_EAX(0xCD4486)
	}
}

__declspec(naked) void AddLightObjectHook()
{
	__asm
	{
		test	eax, eax
		jz		done
		mov		ecx, eax
		push	1
		mov		eax, [ebp-0x60]
		add		eax, 0x48
		push	dword ptr [eax]
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
	done:
		retn
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
		CALL_EAX(0x5E58F0)
		add		esp, 0xC
	skipPerkMod:
		cvttss2si	eax, dword ptr [ebp-0xC0]
		mov		[ebp-0xB8], eax
	done:
		retn
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
		test	dword ptr [edx+0x58], 4
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
		CALL_EAX(0x5E58F0)
		add		esp, 0xC
	skipPerkMod:
		push	dword ptr [ebp-0x50]
		mov		ecx, [ebp-0x74]
		CALL_EAX(0x804BC0)
	done:
		retn
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
		CALL_EAX(0x503A70)
		JMP_EAX(0x729506)
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
		pxor	xmm0, xmm0
		comiss	xmm0, [eax+0x44]
		jz		done
		setb	dl
		add		dl, 1
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
		add		ecx, 0x44
		push	kExtraData_TerminalState
		CALL_EAX(kAddr_GetExtraData)
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

__declspec(naked) void IsCurrentFurnitureObjFixHook()
{
	__asm
	{
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x4C8]
	done:
		retn
	}
}

TESWorldSpace *s_audioMarkerLastWorld = NULL;
Vector<MusicMarker> s_musicMarkerList(0x100);

void __fastcall BuildMusicMarkerList(TESWorldSpace *worldspace, tList<TESObjectREFR> *markerRefList)
{
	if (s_audioMarkerLastWorld != worldspace)
	{
		s_audioMarkerLastWorld = worldspace;
		s_musicMarkerList.Clear();
		ListNode<TESObjectREFR> *traverse = markerRefList->Head();
		TESObjectREFR *markerRef;
		ExtraAudioMarker *xAudioMarker;
		do
		{
			markerRef = traverse->data;
			if (!markerRef) continue;
			xAudioMarker = GetExtraType(&markerRef->extraDataList, AudioMarker);
			if (xAudioMarker && xAudioMarker->data)
				s_musicMarkerList.InsertSorted(MusicMarker(markerRef, xAudioMarker->data));
		}
		while (traverse = traverse->next);
	}
	tList<MusicMarker> *musicMarkers = &g_thePlayer->musicMarkers;
	MusicMarker *musicMarker;
	for (auto mkIter = s_musicMarkerList.Begin(); mkIter; ++mkIter)
	{
		musicMarker = (MusicMarker*)GameHeapAlloc(8);
		*musicMarker = mkIter();
		musicMarkers->Prepend(musicMarker);
	}
}

__declspec(naked) void BuildMusicMarkerListHook()
{
	__asm
	{
		mov		edx, eax
		mov		ecx, [ebp-0x10]
		call	BuildMusicMarkerList
		retn
	}
}

TESForm *s_altAshPile = NULL;

__declspec(naked) void AttachAshPileHook()
{
	__asm
	{
		mov		eax, s_altAshPile
		test	eax, eax
		jz		useDefault
		mov		s_altAshPile, 0
		retn
	useDefault:
		mov		eax, g_gooPileACTI
		mov		ecx, g_ashPileACTI
		cmp     dword ptr [ebp-0x18], 2
		cmovnz	eax, ecx
		retn
	}
}

double s_windSpeedMult = 1 / 255.0;

__declspec(naked) float GetWindSpeedHook()
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

__declspec(naked) void RemoveHotkeyFixHook()
{
	__asm
	{
		mov		eax, [ebp+0x20]
		cmp		eax, g_thePlayer
		jz		doRemove
		mov		ecx, [ebp+0x18]
		CALL_EAX(0x418770)
		movsx	eax, ax
		cmp		[ebp+0x14], eax
		jnz		done
	doRemove:
		mov		ecx, [ebp+0x18]
		CALL_EAX(0x42DEC0)
	done:
		retn
	}
}

__declspec(naked) void ArmourPenetratedHook()
{
	__asm
	{
		movss	xmm0, [ebp-0xC]
		comiss	xmm0, [ecx+0x14]
		ja		done
		or		dword ptr [ecx+0x58], 0x80000000
	done:
		retn
	}
}

__declspec(naked) void SetScaleHook()
{
	static const UInt32 kMinScale = 0x38D1B717;
	__asm
	{
		movss	xmm0, [ebp+8]
		maxss	xmm0, kMinScale
		minss	xmm0, kFlt100
		movss	[ebp+8], xmm0
		retn
	}
}

__declspec(naked) UInt8 __fastcall GetEntryDataModFlagsHook(ContChangesEntry *entry)
{
	__asm
	{
		mov		eax, [ecx+8]
		cmp		byte ptr [eax+4], kFormType_TESObjectWEAP
		jnz		retn0
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		mov		ecx, eax
		push	ecx
		push	kExtraData_ItemDropper
		CALL_EAX(kAddr_GetExtraData)
		pop		ecx
		test	eax, eax
		jz		noDropper
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		noDropper
		lea		ecx, [eax+0x44]
	noDropper:
		push	kExtraData_WeaponModFlags
		CALL_EAX(kAddr_GetExtraData)
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

__declspec(naked) bool __fastcall GetEntryDataHasModHook(ContChangesEntry *entry, int EDX, UInt8 modType)
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
		jz		retn1
	check2nd:
		test	al, 2
		jz		check3rd
		cmp		[ecx+0x184], dl
		jz		retn1
	check3rd:
		test	al, 4
		jz		retn0
		cmp		[ecx+0x188], dl
		jz		retn1
	retn0:
		xor		al, al
		retn	4
	retn1:
		mov		al, 1
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
		push	kExtraData_WeaponModFlags
		mov		ecx, [ebp+8]
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
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
		retn
	}
}

__declspec(naked) void ConstructItemEntryNameHook()
{
	static const char kItemWetnessLevel[] = " (Damp)\0 (Wet)\0 (Frozen)";
	__asm
	{
		push	esi
		push	edi
		mov		esi, [ebp+0xC]
		xor		edi, edi
		mov		[ebp-8], eax
		mov		ecx, [esi+8]
		mov		al, [ecx+4]
		cmp		al, kFormType_TESObjectARMO
		jz		getExtra
		cmp		al, kFormType_TESObjectWEAP
		jnz		doneExtra
	getExtra:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		doneExtra
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		doneExtra
		cmp		al, kFormType_TESObjectARMO
		jnz		isWeapon
		push	kExtraData_Charge
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		doneExtra
		cvtss2si	edx, [eax+0xC]
		cmp		edx, 30
		jl		doneExtra
		mov		edi, offset kItemWetnessLevel
		cmp		edx, 60
		jl		doneExtra
		add		edi, 8
		cmp		edx, 100
		jl		doneExtra
		add		edi, 7
		jmp		doneExtra
	isWeapon:
		push	ecx
		push	kExtraData_ItemDropper
		CALL_EAX(kAddr_GetExtraData)
		pop		ecx
		test	eax, eax
		jz		noDropper
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		noDropper
		lea		ecx, [eax+0x44]
	noDropper:
		push	kExtraData_WeaponModFlags
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		doneExtra
		cmp		byte ptr [eax+0xC], 0
		jz		doneExtra
		mov		edi, '+'
	doneExtra:
		test	edi, edi
		jnz		getName
		cmp		dword ptr [ebp-8], 1
		jle		done
	getName:
		mov		eax, [esi+8]
		movzx	edx, word ptr [eax+0x38]
		test	edx, edx
		jz		done
		push	edx
		mov		edx, [eax+0x34]
		lea		ecx, [ebp-0x88]
		call	StrLenCopy
		test	edi, edi
		jz		doneSuffix
		cmp		edi, '+'
		jnz		wetnessLvl
		mov		[eax], di
		inc		eax
		jmp		doneSuffix
	wetnessLvl:
		mov		edx, edi
		mov		ecx, eax
		call	StrCopy
	doneSuffix:
		mov		edx, [ebp-8]
		cmp		edx, 1
		jle		setUIStr
		mov		word ptr [eax], '( '
		lea		ecx, [eax+2]
		call	IntToStr
		mov		word ptr [eax], ')'
	setUIStr:
		push	1
		lea		edx, [ebp-0x88]
		push	edx
		push	kTileValue_string
		mov		ecx, [ebp+8]
		CALL_EAX(kAddr_TileSetString)
	done:
		pop		edi
		pop		esi
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
		test	dword ptr [edx+0x34], 1
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

__declspec(naked) bool __fastcall GetEffectHiddenHook(EffectItem *effItem)
{
	__asm
	{
		mov		ecx, [ecx+0x14]
		test	ecx, ecx
		jz		retnTrue
		cmp		ecx, ds:[0x11C33F4]
		jz		retnTrue
		test	dword ptr [ecx+0x58], 8
		setnz	al
		retn
	retnTrue:
		mov		al, 1
		retn
	}
}

__declspec(naked) void QueuedRefStopFadeInHook()
{
	__asm
	{
		lea		edx, [eax+0xA4]
		lock dec dword ptr [edx]
		jns		doneSmph1
		mov		dword ptr [edx], 0
	doneSmph1:
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jnz		doneSmph2
		sub		edx, 4
		lock dec dword ptr [edx]
		jns		doneSmph2
		mov		dword ptr [edx], 0
	doneSmph2:
		test	byte ptr [ecx+0x61], 1
		jz		done
		xor		byte ptr [ecx+0x61], 1
		mov		eax, [ebp-0x18]
		mov		ecx, [eax+0x34]
		test	ecx, ecx
		jz		done
		mov		eax, [ecx]
		call	dword ptr [eax+0x10]
		test	eax, eax
		jz		done
		mov		edx, 0x3F800000
		mov		[eax+0xB4], edx
		mov		[eax+0xB8], edx
		or		dword ptr [eax+0x30], 0x4000
	done:
		retn
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
		movss	xmm1, kFltOne
		comiss	xmm0, xmm1
		jb		notFinished
		movss	xmm0, xmm1
		mov		[ecx+8], edx
	notFinished:
		movss	xmm1, [ecx+4]
		subss	xmm1, [ecx]
		mulss	xmm0, xmm1
		addss	xmm0, [ecx]
		push	1
		movd	eax, xmm0
		push	eax
		push	dword ptr [ecx+0x10]
		mov		ecx, [ecx+0x14]
		CALL_EAX(kAddr_TileSetFloat)
		JMP_EAX(0xA08155)
	}
}

__declspec(naked) void __fastcall EndWeatherTransition(Sky *sky)
{
	__asm
	{
		mov		dword ptr [ecx+0x14], 0
		retn
	}
}

__declspec(naked) void CloudsFixHook()
{
	__asm
	{
		mov		ecx, [ebp-0x44]
		mov		dword ptr [ecx+0x34], 0
		push	dword ptr [ecx+0x68]
		push	EndWeatherTransition
		call	MainLoopAddCallback
		add		esp, 8
		retn
	}
}

__declspec(naked) bool ReactionCooldownCheckHook()
{
	static UInt32 lastTickCount = 0;
	__asm
	{
		call	GetTickCount
		mov		edx, eax
		sub		eax, lastTickCount
		cvtsi2ss	xmm0, eax
		divss	xmm0, kFlt1000
		comiss	xmm0, ds:[0x11CE9B8]
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
		test	dword ptr [ecx+8], 0x4820
		setnz	al
		jnz		done
		call	TESObjectREFR::GetDisabled
	done:
		retn
	}
}

__declspec(naked) void PickWeaponModelHook()
{
	__asm
	{
		mov		dword ptr [ebp-0x1C], 0
		mov		ecx, [ebp-4]
		test	ecx, ecx
		jz		doCall
		cmp		dword ptr [ecx+0xC], 0x14
		jz		doCall
		mov		eax, [ebp+8]
		cmp		byte ptr [eax+0xF4], 8
		jz		doCall
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		doCall
		cmp		dword ptr [ecx+0x28], 1
		ja		doCall
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		doCall
		mov		ecx, eax
		call	GetEntryDataModFlagsHook
		mov		[ebp-9], al
	doCall:
		movzx	eax, byte ptr [ebp-9]
		mov		ecx, [ebp-8]
		test	ecx, ecx
		jz		noBip01
		push	eax
		push	dword ptr [ebp+8]
		CALL_EAX(0x4AB750)
		retn
	noBip01:
		push	1
		push	eax
		mov		ecx, [ebp+8]
		CALL_EAX(0x522DF0)
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
		call	dword ptr [eax+0x100]
		test	al, al
		jz		done
		mov		dword ptr [ebp-0x7C], 0x200000
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		done
		cmp		dword ptr [ecx+0x28], 1
		ja		done
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		done
		mov		edx, [eax+8]
		push	edx
		mov		ecx, [eax]
		mov		ecx, [ecx]
		push	kExtraData_WeaponModFlags
		CALL_EAX(kAddr_GetExtraData)
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
		retn
	}
}

__declspec(naked) void AddProjectileLightHook()
{
	__asm
	{
		push	dword ptr [ebp-0x14]
		mov		ecx, ds:[0x11F1958]
		mov		ecx, [ecx+0x14]
		CALL_EAX(0x9A52F0)
		mov		eax, [ebp+8]
		mov		eax, [eax+0x70]
		test	eax, eax
		jz		done
		mov		[ebp-0x18], eax
		mov		ecx, [ebp-0x14]
		mov		eax, [ecx]
		call	dword ptr [eax+0x1D0]
		test	eax, eax
		jz		done
		push	0
		push	eax
		push	dword ptr [ebp-0x14]
		mov		ecx, [ebp-0x18]
		CALL_EAX(0x50D810)
		test	eax, eax
		jz		done
		mov		dword ptr [eax+0xA8], 0x12E
		mov		edx, 0x3F800000
		mov		[eax+0xC8], edx
		mov		[eax+0xCC], edx
		mov		[eax+0xD0], edx
		mov		[eax+0xF4], edx
		mov		dword ptr [eax+0xF8], 0
		mov		ecx, [ebp-0x18]
		cvtsi2ss	xmm0, [ecx+0xA0]
		movss	[eax+0xE0], xmm0
		mov		edx, [ecx+0xB4]
		mov		[eax+0xC4], edx
	done:
		JMP_EAX(0x9BD51D)
	}
}

__declspec(naked) bool __fastcall KillChallengeFixHook(Actor *killed)
{
	__asm
	{
		cmp		byte ptr [ecx+0x18D], 0
		jnz		done
		test	dword ptr [ecx+0x140], 0x80000000
		jnz		done
		call	Actor::GetActorBase
		test	dword ptr [eax+0x34], 2
	done:
		setnz   al
		retn
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

Actor* __fastcall GetNearestLivingAlly(Actor *actor)
{
	if (!actor->combatAllies)
		return NULL;
	Actor *result = NULL, *ally;
	float minDistance = 12800.0F, distance;
	for (auto iter = actor->combatAllies->Begin(); !iter.End(); ++iter)
	{
		ally = *iter;
		if (!ally || (ally == actor) || ally->GetDead() || ThisCall<bool>(0x8A67F0, ally))
			continue;
		distance = GetDistance3D(actor, ally);
		if (distance > minDistance)
			continue;
		minDistance = distance;
		result = ally;
	}
	return result;
}

__declspec(naked) void DeathResponseFixHook()
{
	__asm
	{
		cmp		dword ptr [ebp+0x1C], 0
		jnz		notApplicable
		cmp		dword ptr [ebp+0x10], 2
		jnz		notApplicable
		cmp		dword ptr [ebp+0x14], 6
		jnz		notApplicable
		cmp		dword ptr [ebp+0x18], 1
		jnz		notApplicable
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		notApplicable
		call	GetNearestLivingAlly
		test	eax, eax
		jz		notApplicable
		mov		[ebp-0x14], eax
		push	0
		push	dword ptr [ebp-0x1C]
		push	0
		push	0
		jmp		done
	notApplicable:
		push	1
		push	dword ptr [ebp-0x1C]
		push	1
		push	1
	done:
		JMP_EAX(0x984160)
	}
}

__declspec(naked) void ClearAshPilesHook()
{
	__asm
	{
		test	word ptr [ecx+6], kHookFormFlag6_IsAshPile
		jz		done
		push	kExtraData_AshPileRef
		mov		ecx, [ebp-0x2C]
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		doUnload
		mov		edx, [eax+0xC]
		mov		[ebp-0x20], edx
		push	1
		push	eax
		mov		ecx, [ebp-0x2C]
		add		ecx, 0x44
		CALL_EAX(0x410020)
		mov		ecx, [ebp-0x20]
		test	ecx, ecx
		jz		done
		push	kExtraData_AshPileRef
		add		ecx, 0x44
		CALL_EAX(0x410140)
	doUnload:
		mov		ecx, [ebp-0x2C]
		CALL_EAX(0x572270)
	done:
		retn
	}
}

__declspec(naked) void SetAcousticSpaceFixHook()
{
	__asm
	{
		mov		byte ptr ds:[0x11DCFB1], 0
		test	eax, eax
		jz		done
		mov		dword ptr ds:[0x11DCFB8], 0
		cmp		eax, ds:[0x11DCFB4]
		jz		done
		push	eax
		CALL_EAX(0x82EAE0)
		pop		ecx
	done:
		retn
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
		cmp		byte ptr ds:[0x11ACDF9], 0
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
		mov		ecx, 0x11C4180
		CALL_EAX(0xAA5230)
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
		GAME_HEAP_FREE
		GAME_HEAP_FREE
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
		movss	xmm0, kFltOne
		test	dword ptr [ecx+0x48], 1
		jnz		done
		movsx	edx, byte ptr [ecx+0x46]
		test	edx, edx
		jz		done
		cvtsi2ss	xmm1, edx
		js		isNeg
		divss	xmm1, kFlt100
		jmp		doAdd
	isNeg:
		divss	xmm1, kFlt200
	doAdd:
		addss	xmm0, xmm1
	done:
		retn
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
		jmp		done
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
		mov		dword ptr [ebp-0x10], 0
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
		retn
	}
}

__declspec(naked) UInt32 __fastcall AdjustSoundFrequencyHook(BSGameSound *gameSound, int EDX, float freq)
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
		cvtss2si	edx, xmm0
		mov		eax, 0x64
		cmp		edx, eax
		cmovb	edx, eax
		mov		eax, 0x30D40
		cmp		edx, eax
		cmova	edx, eax
		push	edx
		mov		ecx, [ecx+0x19C]
		push	ecx
		mov		ecx, [ecx]
		call	dword ptr [ecx+0x44]
	done:
		xor		eax, eax
		retn	4
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

__declspec(naked) BGSImpactData* __fastcall GetImpactDataHook(BGSImpactDataSet *impDataSet, int EDX, UInt32 materialType)
{
	__asm
	{
		mov		edx, [esp+4]
		mov		eax, [ecx+edx*4+0x1C]
		test	eax, eax
		jz		done
		mov		ecx, ds:[0x11CA834]
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

__declspec(naked) UInt32 __fastcall GetFactionReactionHook(TESFaction *faction, int EDX, TESFaction *toFaction)
{
	__asm
	{
		add		ecx, 0x24
		JMP_EAX(0x48C1B0)
	}
}

__declspec(naked) void GetActorAgilityHook()
{
	__asm
	{
		push	0xA
		add		ecx, 0xA4
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		fstp	dword ptr [ebp-0xC]
		movss	xmm0, [ebp-0xC]
		maxss	xmm0, kFltOne
		minss	xmm0, kFlt10
		movss	[ebp-0xC], xmm0
		retn
	}
}

typedef Vector<MediaSet*> MediaSetArray;
UnorderedMap<ListNode<MediaSet>*, MediaSetArray> s_pickMediaSetMap(0x40);

MediaSet* __stdcall PickMediaSetHook(ListNode<MediaSet> *listHead)
{
	if (!listHead->next)
		return listHead->data;
	MediaSetArray *msArr;
	s_pickMediaSetMap.Insert(listHead, &msArr);
	if (msArr->Empty())
	{
		do {
			if (listHead->data)
				msArr->Append(listHead->data);
		} while (listHead = listHead->next);
		msArr->Shuffle();
	}
	return msArr->Pop();
}

Vector<TESLoadScreen*> s_locationLoadScreens, s_genericLoadScreens;

__declspec(naked) void __fastcall GetSuitableLoadScreensHook(LoadingMenu *loadingMenu, int EDX, BSSimpleArray<TESLoadScreen> *results)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		test	word ptr [ecx+0x222], 0x20
		setnz	bl
		mov		bh, byte ptr ds:[0x11D8907]
		mov		eax, g_OSGlobals
		or		bh, byte ptr [eax+2]
		mov		eax, [esp+0x10]
		mov		dword ptr [eax+8], 0
		mov		eax, [ecx+0x1E8]
		push	eax
		test	bl, bl
		jnz		doGeneric
		test	eax, eax
		jz		doGeneric
		mov		esi, ds:[s_locationLoadScreens.data]
		mov		edi, ds:[s_locationLoadScreens.numItems]
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
		mov		edi, ds:[s_genericLoadScreens.numItems]
		test	edi, edi
		jz		done
		mov		esi, ds:[s_genericLoadScreens.data]
		ALIGN 16
	iter2Head:
		mov		eax, [esi]
		test	bl, bl
		jz		doneCheck
		test	dword ptr [eax+8], 0x400
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
		retn
	}
}

UInt32 s_uWMChancePerLevel = 0, s_uWMChanceMin = 0, s_uWMChanceMax = 0;
const UInt8 kWeaponModShuffle[][4] = {{1, 2, 4, 0}, {1, 4, 2, 0}, {2, 1, 4, 0}, {2, 4, 1, 0}, {4, 1, 2, 0}, {4, 2, 1, 0}};

void __fastcall DistributeWeaponMods(Actor *actor)
{
	ContChangesEntry *weaponInfo = ((MiddleHighProcess*)actor->baseProcess)->weaponInfo;
	if (!weaponInfo || !weaponInfo->extendData)
		return;
	TESObjectWEAP *weapon = (TESObjectWEAP*)weaponInfo->type;
	if ((weapon->eWeaponType > 9) || (weapon->weaponFlags1 & 0xA0))
		return;
	UInt8 modFlags = weapon->itemMod[0] != NULL;
	if (weapon->itemMod[1])
		modFlags |= 2;
	if (weapon->itemMod[2])
		modFlags |= 4;
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
	ExtraDataList *xData = weaponInfo->extendData->GetFirstItem();
	AddExtraData(xData, ExtraWeaponModFlags::Create(modFlags));
	ThisCall(0x88DB20, actor, weapon, 1, xData, false);
}

__declspec(naked) ExtraDataList* __fastcall DoOnLoadActorHook(TESObjectREFR *refr)
{
	__asm
	{
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		done
		test	dword ptr [ecx+0x140], 0x10000000
		jnz		done
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 1
		ja		done
		or		dword ptr [ecx+0x140], 0x10000000
		cmp		dword ptr [ecx+0x108], 0
		jnz		done
		cmp		byte ptr [ecx+0x18D], 0
		jnz		done
		call	DistributeWeaponMods
		mov		ecx, [ebp+8]
	done:
		lea		eax, [ecx+0x44]
		retn
	}
}

__declspec(naked) void ResetActorFlagsRespawnHook()
{
	__asm
	{
		and		dword ptr [ecx+0x140], 0xEFFFFFFF
		lea		eax, [ecx+0x44]
		retn
	}
}

bool s_localizedDTDR = false;
UInt32 s_doCalcDamage = 0;

float s_VATSHitLocDT = 0;

void __fastcall CalculateHitDamageHook(ActorHitData *hitData, UInt32 dummyEDX, UInt32 noBlock)
{
	if (s_doCalcDamage)
	{
		noBlock = s_doCalcDamage - 1;
		s_doCalcDamage = 0;
	}
	else
	{
		UInt32 retnAddr = (UInt32)_ReturnAddress();
		if ((retnAddr == 0x9B5628) || (retnAddr == 0x9B5707))
		{
			s_doCalcDamage = noBlock + 1;
			return;
		}
	}
	Actor *target = hitData->target;
	if (!target || !target->IsActor()) return;
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
	tList<TESAmmoEffect> *ammoEffects = NULL;
	if (hitWeapon && hitWeapon->ammo.ammo)
	{
		TESAmmo *ammo = NULL;
		if (source)
		{
			ContChangesEntry *ammoInfo = source->GetAmmoInfo();
			if (ammoInfo) ammo = (TESAmmo*)ammoInfo->type;
		}
		if (!ammo || NOT_ID(ammo, TESAmmo))
			ammo = hitWeapon->GetAmmo();
		if (ammo && IS_ID(ammo, TESAmmo) && !ammo->effectList.Empty())
		{
			ammoEffects = &ammo->effectList;
			hitData->fatigueDmg = ApplyAmmoEffects(kAmmoEffect_FatigueMod, ammoEffects, hitData->fatigueDmg);
		}
	}
	UInt8 flagPCTM = source == g_thePlayer;
	if (source && source->isTeammate)
		flagPCTM |= 2;
	if (target == g_thePlayer)
		flagPCTM |= 4;
	if (target->isTeammate)
		flagPCTM |= 8;
	float dmgResist, dmgThreshold, cpyThreshold, hitLocDT = 0, hitLocDR = 0, valueMod1, valueMod2;
	if (hitWeapon && (hitWeapon->weaponFlags1 & 1))
	{
		dmgResist = 1.0F;
		dmgThreshold = 0;
		cpyThreshold = 0;
		hitData->armorDmg = 0;
	}
	else
	{
		if (s_localizedDTDR)
		{
			SInt32 hitLocation = hitData->hitLocation;
			if ((hitLocation >= 0) && (hitLocation <= 12) && NOT_ID(target, Creature))
			{
				ValidBip01Names *validBip = ((Character*)target)->validBip01Names;
				if (validBip)
				{
					ValidBip01Names::Data *slotData = validBip->slotData;
					TESObjectARMO *hitLocArmor;
					if (hitLocation == 1)
					{
						hitLocArmor = slotData[2].armor;
						if (hitLocArmor && IS_TYPE(hitLocArmor, TESObjectARMO))
						{
							hitLocDT = hitLocArmor->damageThreshold;
							hitLocDR = (float)hitLocArmor->armorRating;
						}
					}
					else
					{
						for (UInt32 slotIdx : {0, 1, 9, 10, 14})
						{
							hitLocArmor = slotData[slotIdx].armor;
							if (hitLocArmor && IS_TYPE(hitLocArmor, TESObjectARMO) && !(hitLocArmor->bipedModel.partMask & 4))
							{
								hitLocDT += hitLocArmor->damageThreshold;
								hitLocDR += (float)hitLocArmor->armorRating;
							}
						}
					}
				}
			}
			s_VATSHitLocDT = hitLocDT;
		}
		valueMod1 = 1.0F;
		if (flagPCTM & 8)
			valueMod1 += ThisCall<float>(0x66EF50, &g_thePlayer->avOwner, kAVCode_Charisma) / 20.0F;
		dmgResist = (target->avOwner.GetActorValue(kAVCode_DamageResist) - hitLocDR) * valueMod1;
		if (dmgResist > 100.0F)
			dmgResist = 100.0F;
		if (ammoEffects)
			dmgResist = ApplyAmmoEffects(kAmmoEffect_DRMod, ammoEffects, dmgResist);
		if (dmgResist <= 0)
			dmgResist = 1.0F;
		else
		{
			valueMod2 = *(float*)0x11D1024;		// fMaxArmorRating
			if (dmgResist > valueMod2)
				dmgResist = valueMod2;
			dmgResist = 1.0F - (dmgResist / 100.0F);
		}
		cpyThreshold = dmgThreshold = (target->avOwner.GetActorValue(kAVCode_DamageThreshold) - hitLocDT) * valueMod1;
		if (ammoEffects)
			dmgThreshold = ApplyAmmoEffects(kAmmoEffect_DTMod, ammoEffects, dmgThreshold);
	}
	HighProcess *hiProcess = (HighProcess*)target->baseProcess;
	ContChangesEntry *weaponInfo;
	if (!noBlock && (hiProcess->currentAction == kAnimAction_Block) && !target->GetIsParalyzed() &&
		CdeclCall<bool>(0x9A6AE0, target, hitData->projectile ? hitData->projectile : (TESObjectREFR*)source, 0))
	{
		weaponInfo = hiProcess->weaponInfo;
		bool noInfo = !weaponInfo;
		TESObjectWEAP *pWeapon = weaponInfo ? (TESObjectWEAP*)weaponInfo->type : NULL;
		hitData->flags |= 1;
		if (flagPCTM & 4)
			g_thePlayer->counterAttackTimer = *(float*)0x11D0BE8;	// fCounterAttackTimer
		if (noInfo)
		{
			if ((!hitWeapon || (hitWeapon->eWeaponType <= 2)) && !hitData->projectile)
				noInfo = false;
			else hitData->blockDTMod = 0;
		}
		if (!noInfo)
		{
			UInt32 skillModType = 0;
			if (!hitWeapon)
				skillModType = kAVCode_Unarmed;
			else if (pWeapon)
			{
				if (!pWeapon->eWeaponType)
					skillModType = kAVCode_Unarmed;
				else if (pWeapon->eWeaponType <= 2)
					skillModType = kAVCode_MeleeWeapons;
			}
			hitData->blockDTMod = *(float*)0x11CE844;	// fBlockSkillBase
			if (skillModType)
				hitData->blockDTMod += *(float*)0x11CFD90 * target->avOwner.GetActorValue(skillModType);	// fBlockSkillMult
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
		if (flagPCTM & 3)
			ApplyPerkModifiers(kPerkEntry_CalculateWeaponDamage, source, hitWeapon, target, &hitData->healthDmg);
	}
	else if (source)
	{
		hitData->weapon = g_fistsWeapon;
		hitWeapon = g_fistsWeapon;
	}
	valueMod2 = hitData->healthDmg;
	hitData->healthDmg *= dmgResist;
	valueMod1 = 0;
	if (flagPCTM & 3)
	{
		ApplyPerkModifiers(kPerkEntry_ModifyDamageThresholdAttacker, source, hitWeapon, target, &valueMod1);
		dmgThreshold -= valueMod1;
	}
	else if (flagPCTM & 0xC)
	{
		ApplyPerkModifiers(kPerkEntry_ModifyDamageThresholdDefender, target, source, hitWeapon ? hitWeapon : g_fistsWeapon, &valueMod1);
		dmgThreshold += valueMod1;
	}
	bool flagArg;
	if (dmgThreshold > 0)
	{
		if (source)
		{
			weaponInfo = source->GetWeaponInfo();
			if (weaponInfo && ThisCall<bool>(0x4BDA70, weaponInfo, 0xC))
			{
				valueMod1 = (float)ThisCall<UInt8>(0x525B20, hitWeapon, 1, 0, source);
				dmgThreshold /= valueMod1;
			}
		}
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
		UInt32 attackDmg = hitWeapon->attackDmg.damage;
		if (!attackDmg) attackDmg = 1;
		valueMod1 = 1.0F - (dmgThreshold / (int)attackDmg);
		if (valueMod1 > 0)
			hitData->healthDmg *= valueMod1;
	}
	else hitData->healthDmg -= dmgThreshold;
	valueMod1 = *(float*)0x11CED50 * valueMod2;	// fMinDamMultiplier
	valueMod2 = hitData->healthDmg - valueMod1;
	hitData->armorDmg = 0;
	if (valueMod2 > 0)
	{
		if ((flagPCTM & 4) && (hitData->fatigueDmg <= 0))
		{
			hitLocDT *= dmgResist;
			valueMod2 -= hitLocDT;
			if (valueMod2 > 0)
			{
				dmgThreshold += hitLocDT;
				if (valueMod2 > dmgThreshold)
					valueMod2 = dmgThreshold;
				hitData->armorDmg = *(float*)0x11CF100 * valueMod2;	// fDamageToArmorPercentage
			}
		}
		hitData->flags |= 0x80000000;
		if (flagArg)
		{
			if (flagPCTM & 4)
				CdeclCall(0x77F420, true);
			else if ((flagPCTM & 1) && !target->HasHealth(0))
				CdeclCall(0x77F490, true);
		}
	}
	else
	{
		hitData->healthDmg = valueMod1;
		if (flagPCTM & 4)
			CdeclCall(0x77F420, flagArg);
		else if ((flagPCTM & 1) && !target->HasHealth(0))
			CdeclCall(0x77F490, flagArg);
		if (flagPCTM & 2)
			CdeclCall(0x8D5CB0, source, 9);
	}
	if (hitWeapon && (hitWeapon->resistType != -1))
	{
		valueMod1 = target->avOwner.GetActorValue(hitWeapon->resistType);
		if (valueMod1 > 0)
		{
			valueMod1 /= 100.0F;
			if (valueMod1 < 1.0F)
				hitData->healthDmg *= 1.0F - valueMod1;
			else hitData->healthDmg = 0;
		}
	}
	if ((hitData->fatigueDmg > 0) && (hitData->wpnBaseDmg > 0))
		hitData->fatigueDmg *= hitData->healthDmg / hitData->wpnBaseDmg;
	if ((flagPCTM & 4) && g_VATSCameraData->mode)
	{
		valueMod1 = *(float*)0x11CE9F4;		//	fVATSPlayerDamageMult
		hitData->healthDmg *= valueMod1;
		hitData->wpnBaseDmg *= valueMod1;
		hitData->fatigueDmg *= valueMod1;
		hitData->limbDmg *= valueMod1;
		hitData->armorDmg *= valueMod1;
		hitData->flt2C *= valueMod1;
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
		mov		dword ptr [ebp-0x24], 0
		movss	xmm0, [eax+0x14]
		mov		dword ptr [eax+0x14], 0
		cmp		dword ptr [eax+0x54], 0
		jz		skipMod
		mulss	xmm0, ds:[0x11CE5D4]
	skipMod:
		movss	[eax+0x20], xmm0
	done:
		retn
	}
}

__declspec(naked) float __cdecl GetVATSTargetDTHook(PlayerCharacter *thePlayer, Actor *target)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	0
		push	esi
		mov		esi, ds:[0x11D98D4]
		test	esi, esi
		cmovz	esi, g_fistsWeapon
		push	kAVCode_DamageThreshold
		mov		ecx, [ebp+0xC]
		add		ecx, 0xA4
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		cmp		s_localizedDTDR, 0
		jz		noLocalized
		fsub	s_VATSHitLocDT
	noLocalized:
		lea		eax, [ebp-4]
		push	eax
		push	dword ptr [ebp+0xC]
		push	esi
		push	dword ptr [ebp+8]
		push	kPerkEntry_ModifyDamageThresholdAttacker
		CALL_EAX(kAddr_ApplyPerkModifiers)
		add		esp, 0x14
		fsub	dword ptr [ebp-4]
		mov		ecx, [ebp+0xC]
		cmp		byte ptr [ecx+0x18D], 0
		jz		notTeammate
		lea		eax, [ebp-4]
		mov		dword ptr [eax], 0
		push	eax
		push	esi
		push	dword ptr [ebp+8]
		push	ecx
		push	kPerkEntry_ModifyDamageThresholdDefender
		CALL_EAX(kAddr_ApplyPerkModifiers)
		add		esp, 0x14
		fadd	dword ptr [ebp-4]
	notTeammate:
		cmp		dword ptr [esi+0xA8], 0
		jz		done
		mov		ecx, [ebp+8]
		mov		ecx, [ecx+0x68]
		mov		ecx, [ecx+0x118]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		done
		add		ecx, 0xD4
		cmp		dword ptr [ecx], 0
		jz		done
		push	ecx
		fstp	dword ptr [esp]
		push	ecx
		push	kAmmoEffect_DTMod
		CALL_EAX(kAddr_ApplyAmmoEffects)
		add		esp, 0xC
		fldz
		fucomip	st, st(1)
		jbe		done
		fstp	st
		fldz
	done:
		pop		esi
		leave
		retn
	}
}

UInt8 s_playerMinHPMode = 0;

__declspec(naked) bool PlayerCannotBeHitHook()
{
	__asm
	{
		mov		al, ds:[0x11E0898]
		test	al, al
		jnz		done
		cmp		s_playerMinHPMode, 2
		setz	al
		jz		done
		mov		al, ds:[0x11E07BA]
	done:
		retn
	}
}

UInt8 s_disableHitEffects = 0;

__declspec(naked) TESImageSpaceModifier *GetHitIMODHook()
{
	__asm
	{
		cmp		byte ptr ds:[0x118ABB1], 0
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

__declspec(naked) bool __fastcall GetControlFlagHook(PlayerCharacter *thePlayer, int EDX, UInt32 flag)
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

__declspec(naked) float GetPCRepairSkill()
{
	__asm
	{
		push	kAVCode_Repair
		mov		ecx, g_thePlayer
		add		ecx, 0xA4
		CALL_EAX(0x93ACB0)
		push	ecx
		fstp	dword ptr [esp]
		movss	xmm0, [esp]
		pxor	xmm1, xmm1
		maxss	xmm0, xmm1
		movss	xmm1, ds:[0x11D0FF4]	// fRepairSkillBase
		movss	xmm2, kFlt10
		mulss	xmm1, xmm2
		mulss	xmm2, ds:[0x11D0190]	// fRepairSkillMax
		subss	xmm2, xmm1
		mulss	xmm0, xmm2
		movss	xmm2, kFlt100
		divss	xmm0, xmm2
		addss	xmm0, xmm1
		minss	xmm0, xmm2
		movss	[esp], xmm0
		fld		dword ptr [esp]
		pop		ecx
		retn
	}
}

float s_repairedItemHealth = 0;
bool s_repairedItemModded = false;

__declspec(naked) void EnableRepairButtonHook()
{
	__asm
	{
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
		call	GetPCRepairSkill
		fucomip	st, st(1)
		fstp	s_repairedItemHealth
		retn
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
		push	1
		CALL_EAX(kAddr_GetItemHealthPerc)
		fld		s_repairedItemHealth
		fucomip	st, st(1)
		setnb	al
		fstp	st
	done:
		retn
	}
}

__declspec(naked) float __stdcall GetRepairAmount(float itemToRepairHealth, float itemUsedHealth)
{
	__asm
	{
		call	GetPCRepairSkill
		fstp	st
		divss	xmm0, kFlt100
		movss	xmm5, kFlt10
		movss	xmm1, [esp+4]
		divss	xmm1, xmm5
		movss	xmm3, [esp+8]
		divss	xmm3, xmm5
		movss	xmm2, xmm1
		minss	xmm2, xmm3
		mulss	xmm2, ds:[0x11D074C]	// fRepairScavengeMult
		maxss	xmm1, xmm3
		movss	xmm3, ds:[0x11D0884]	// fRepairMin
		movss	xmm4, ds:[0x11CFFA4]	// fRepairMax
		subss	xmm4, xmm3
		mulss	xmm4, xmm0
		addss	xmm4, xmm1
		addss	xmm4, xmm2
		addss	xmm4, xmm3
		divss	xmm4, xmm5
		minss	xmm0, xmm4
		movss	[esp+4], xmm0
		fld		dword ptr [esp+4]
		retn	8
	}
}

__declspec(naked) void SetRepairListValuesHook()
{
	__asm
	{
		push	dword ptr [ebp-0xC]
		push	dword ptr [ebp-4]
		call	GetRepairAmount
		retn
	}
}

__declspec(naked) void DoRepairItemHook()
{
	__asm
	{
		push	1
		mov		ecx, [ebp+8]
		CALL_EAX(kAddr_GetItemHealthPerc)
		push	ecx
		fstp	dword ptr [esp]
		push	1
		mov		ecx, ds:[0x11DA760]
		CALL_EAX(kAddr_GetItemHealthPerc)
		push	ecx
		fstp	dword ptr [esp]
		call	GetRepairAmount
		retn
	}
}

__declspec(naked) void RepairMenuClickHook()
{
	__asm
	{
		call	GetPCRepairSkill
		push	1
		mov		ecx, ds:[0x11DA760]
		CALL_EAX(kAddr_GetItemHealthPerc)
		fucomip	st, st(1)
		fstp	st
		setb	ah
		retn
	}
}

__declspec(naked) void DamageToWeaponHook()
{
	static const char kSkillMatcher[] = {3, 1, 3, -1, -1, 2, -1, -1, 0, -1, -1, -1, 2};
	static const float kDegrMults[] = {0.15F, 0.2F, 0.25F, 0.3F};
	__asm
	{
		mov		eax, ds:[0x11CF154]
		mov		[ebp-4], eax
		xor		edx, edx
		mov		[ebp-8], edx
		mov		ecx, [ebp+8]
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		done
		cmp		dword ptr [ecx+0x28], 1
		ja		done
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		done
		mov		ecx, [eax+8]
		test	ecx, ecx
		jz		done
		mov		[ebp-8], ecx
		mov		dl, [ecx+0x15C]
		sub		dl, kAVCode_BigGuns
		js		done
		cmp		dl, 0xC
		ja		done
		movsx	eax, kSkillMatcher[edx]
		test	eax, eax
		js		done
		movzx	edx, word ptr [ecx+0xA0]
		cvtsi2ss	xmm0, edx
		mulss	xmm0, [ebp-4]
		mulss	xmm0, kDegrMults[eax*4]
		movss	[ebp-4], xmm0
	done:
		retn
	}
}

__declspec(naked) void InitMissileFlagsHook()
{
	__asm
	{
		cmp		byte ptr [esp+8], 0
		jz		done
		mov		eax, [ecx+0x20]
		test	word ptr [eax+0x60], 0x100
		jz		done
		or		word ptr [ecx+0xC8], 0x8000
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

__declspec(naked) void RefreshHPBarDelayHook()
{
	__asm
	{
		mov		eax, [ebp-0xC]
		mov		edx, 0xA
		cmp		eax, edx
		jnb		done
		mov		[ebp-0xC], edx
		cmp		s_sleepSemaphore, 0
		jnz		done
		mov		s_sleepSemaphore, 1
		sub		edx, eax
		push	edx
		call	Sleep
	done:
		retn
	}
}

__declspec(naked) void VoiceModulationFixHook()
{
	__asm
	{
		mov		eax, 0x104
		mov		ecx, [ebp-0x29C]
		mov		ecx, [ecx+0x20]
		test	dword ptr [ecx+8], 0x2000
		jnz		done
		mov		eax, 0x80104
	done:
		mov		[ebp-0x248], eax
		retn
	}
}

__m128 s_shapeVerticesZ[10];
bhkCharControllerShape *s_pcControllerShape = NULL;

__declspec(naked) void InitControllerShapeHook()
{
	static const float kFltSix = 6.0F;
	__asm
	{
		mov		eax, 0x1267BE0
		mov		ecx, [esi+0x80]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+0xCC]
		cmp		ecx, ds:[0x11DEA3C]
		jz		proceed
		cmp		byte ptr [ecx+0x18D], 0
		jz		done
	proceed:
		mov		eax, offset s_pcControllerShape
		cmp		dword ptr [eax], 0
		jnz		done
		mov		eax, offset s_shapeVerticesZ
		mov		ecx, ds:[0x1267BE0]
		mov		ecx, [ecx+8]
		mov		ecx, [ecx+0x40]
		add		ecx, 0x20
		mov		dl, 5
		pxor	xmm1, xmm1
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
		call	NiReleaseAddRef
	done:
		push	eax
		lea		ecx, [ebx+0x5A8]
		CALL_EAX(0xC663D0)
		retn
	}
}

bool s_playerIsSneaking = false;

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
		cmp		s_playerIsSneaking, al
		jz		done
		mov		ecx, s_pcControllerShape
		test	ecx, ecx
		jz		done
		mov		s_playerIsSneaking, al
		mov		ecx, [ecx+8]
		mov		ecx, [ecx+0x40]
		movzx	eax, al
		mov		edx, 0x50
		test	eax, eax
		cmovz	edx, eax
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
	FileStream srcFile;
	if (!srcFile.Open("nvac.log")) return;
	UInt32 length = srcFile.GetLength();
	if (s_lastNVACLogSize >= length) return;
	UInt32 startAt = s_lastNVACLogSize;
	s_lastNVACLogSize = length;
	if (!startAt) return;
	srcFile.SetOffset(startAt);
	length -= startAt + 2;
	char *buffer = StrLenCopy(s_strValBuffer, "\n>>>>>>>>>>> BEGIN NVAC REPORT >>>>>>>>>>>\n", 43);
	srcFile.ReadBuf(buffer, length);
	memcpy(buffer + length, "\n<<<<<<<<<<<< END NVAC REPORT <<<<<<<<<<<<\n", 44);
	Console_Print(s_strValBuffer);
	QueueUIMessage("NVAC log updated (see console print).", 0, (const char*)0x1049638, NULL, 2.5F, 0);
}

__declspec(naked) void __fastcall UpdateTimeGlobalsHook(GameTimeGlobals *timeGlobals, int EDX, float secPassed)
{
	static const double kDbl0dot1 = 0.1, kDbl24dot0 = 24.0, kDbl3600dot0 = 3600.0;
	static double gameHour = 0, daysPassed = 0;
	__asm
	{
		movq	xmm4, kDbl24dot0
		cvtss2sd	xmm2, [esp+4]
		mov		eax, [ecx+0x14]
		cvtss2sd	xmm0, [eax+0x24]
		mulsd	xmm2, xmm0
		divsd	xmm2, kDbl3600dot0
		mov		eax, [ecx+0xC]
		cvtss2sd	xmm0, [eax+0x24]
		cmp		byte ptr [ecx+0x1C], 0
		jnz		doRecalc
		movq	xmm1, xmm0
		movq	xmm3, gameHour
		subsd	xmm1, xmm3
		comisd	xmm1, kDbl0dot1
		ja		doRecalc
		movq	xmm0, xmm3
		movq	xmm1, daysPassed
		jmp		proceed
	doRecalc:
		mov		[ecx+0x1C], 0
		mov		edx, g_thePlayer
		mov		[edx+0xE39], 1
		movq	xmm1, xmm0
		divsd	xmm1, xmm4
		mov		eax, [ecx+0x10]
		cvttss2si	edx, [eax+0x24]
		cvtsi2sd	xmm3, edx
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
		movzx	edx, kDaysPerMonth[esi]
	iterHead:
		cmp		edi, edx
		jb		incDay
		mov		edi, 1
		cmp		esi, 0xB
		jb		incMonth
		xor		esi, esi
		mov		edx, 0x1F
		mov		eax, [ecx]
		movss	xmm3, [eax+0x24]
		addss	xmm3, kFltOne
		movss	[eax+0x24], xmm3
		jmp		iterNext
	incDay:
		inc		edi
		jmp		iterNext
	incMonth:
		inc		esi
		movzx	edx, kDaysPerMonth[esi]
	iterNext:
		subsd	xmm0, xmm4
		comisd	xmm0, xmm4
		ja		iterHead
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
		movq	gameHour, xmm0
		movq	daysPassed, xmm1
		retn	4
	}
}

bool s_hardcoreNeedsFix = false;

__declspec(naked) bool __fastcall ModHardcoreNeedsHook(PlayerCharacter *thePlayer, int EDX, UInt32 flag)
{
	__asm
	{
		lea		eax, [ecx+0x5D4]
		cmp		s_hardcoreNeedsFix, 0
		jz		tracking
		add		ecx, 0x49C
		xor		edx, edx
		pxor	xmm1, xmm1
		cmp		[ecx], edx
		jz		FOD
		movss	xmm0, [ecx]
		mov		[ecx], edx
		addss	xmm0, [eax]
		maxss	xmm0, xmm1
		movss	[eax], xmm0
	FOD:
		cmp		[ecx+4], edx
		jz		SLP
		movss	xmm0, [ecx+4]
		mov		[ecx+4], edx
		addss	xmm0, [eax+4]
		maxss	xmm0, xmm1
		movss	[eax+4], xmm0
	SLP:
		cmp		[ecx+8], edx
		jz		tracking
		movss	xmm0, [ecx+8]
		mov		[ecx+8], edx
		addss	xmm0, [eax+8]
		maxss	xmm0, xmm1
		movss	[eax+8], xmm0
	tracking:
		test	s_serializedFlags, kSerializedFlag_NoHardcoreTracking
		jz		done
		mov		byte ptr [eax+0x865], 1
	done:
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

__declspec(naked) void DoOperator()
{
	__asm
	{
//	Operator &&
		xor		edx, edx
		mov		ecx, [ebp-0x20]
		test	al, al
		jz		lIntAND
		or		ecx, [ebp-0x1C]
	lIntAND:
		test	ecx, ecx
		jz		doneAND
		mov		ecx, [ebp-0x38]
		test	ah, ah
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
		test	al, al
		jz		lIntOR
		or		ecx, [ebp-0x1C]
	lIntOR:
		test	ecx, ecx
		jnz		doneOR
		mov		ecx, [ebp-0x38]
		test	ah, ah
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
		xor		edx, edx
		idiv	ecx
	div0Int:
		OPERATOR_RES_EAX
//	Operator % (flt)
		cvttsd2si	eax, xmm0
		cvttsd2si	ecx, xmm1
		xor		edx, edx
		test	ecx, ecx
		jz		mod0Flt
		idiv	ecx
	mod0Flt:
		OPERATOR_RES_EDX
//	Operator % (int)
		xor		edx, edx
		test	ecx, ecx
		jz		mod0Int
		idiv	ecx
	mod0Int:
		OPERATOR_RES_EDX
	}
}

const UInt32 kDoOperatorJumpTable[] =
{
	(UInt32)DoOperator, (UInt32)DoOperator,
	(UInt32)DoOperator + 0x30, (UInt32)DoOperator + 0x30,
	(UInt32)DoOperator + 0x60, (UInt32)DoOperator + 0x70,
	(UInt32)DoOperator + 0x80, (UInt32)DoOperator + 0x90,
	(UInt32)DoOperator + 0xA0, (UInt32)DoOperator + 0xB0,
	(UInt32)DoOperator + 0xC0, (UInt32)DoOperator + 0xD0,
	(UInt32)DoOperator + 0xE0, (UInt32)DoOperator + 0xF0,
	(UInt32)DoOperator + 0x100, (UInt32)DoOperator + 0x110,
	(UInt32)DoOperator + 0x120, (UInt32)DoOperator + 0x130,
	(UInt32)DoOperator + 0x140, (UInt32)DoOperator + 0x150,
	(UInt32)DoOperator + 0x160, (UInt32)DoOperator + 0x170,
	(UInt32)DoOperator + 0x180, (UInt32)DoOperator + 0x1A0,
	(UInt32)DoOperator + 0x1B0, (UInt32)DoOperator + 0x1D0
};

__declspec(naked) void DoOperatorHook()
{
	__asm
	{
		mov		al, [ebp-0x28]
		mov		ah, [ebp-0x40]
		shl		edx, 3
		cmp		edx, 8
		jbe		doJump
		test	al, al
		jnz		lValFlt
		test	ah, ah
		jnz		lValCvt
		mov		eax, [ebp-0x20]
		mov		ecx, [ebp-0x38]
		add		edx, 4
		jmp		doJump
		ALIGN 16
	lValFlt:
		movq	xmm0, qword ptr [ebp-0x20]
		test	ah, ah
		jnz		rValFlt
		cvtsi2sd	xmm1, [ebp-0x38]
		jmp		doJump
	lValCvt:
		cvtsi2sd	xmm0, [ebp-0x20]
	rValFlt:
		movq	xmm1, qword ptr [ebp-0x38]
	doJump:
		mov		[ebp-0x10], 0
		push	0x5948BB
		jmp		kDoOperatorJumpTable[edx]
	}
}

__declspec(naked) void DefaultTextureHook()
{
	__asm
	{
		CALL_EAX(0xAFE220)
		mov		[ebp-8], eax
		test	eax, eax
		jnz		done
		cmp		dword ptr [ebp+0x10], 0
		jz		done
		lea		ecx, [ebp-0x118]
		mov		edx, [ebp+0x14]
		cmp		edx, 2
		jz		isDDS
		cmp		edx, 0xFFFF
		jnz		done
		call	StrLen
		cmp		dword ptr [ecx+eax-4], 'sdd.'
		jnz		done
	isDDS:
		push	ecx
		mov		ecx, offset s_missingTextures
		call	DebugLog::Message
	done:
		JMP_EAX(0xAFE19E)
	}
}

UnorderedMap<const char*, UInt32> s_optionalHacks({{"bIgnoreDTDRFix", 1}, {"bEnableFO3Repair", 2}, {"bEnableBigGunsSkill", 3}, {"bProjImpactDmgFix", 4},
	{"bGameDaysPassedFix", 5}, {"bHardcoreNeedsFix", 6}, {"bNoFailedScriptLocks", 7}, {"bDoublePrecision", 8}, {"bQttSelectShortKeys", 9},
	{"bMultiProjectileFix", 10}, {"bFO3WpnDegradation", 11}, {"bLocalizedDTDR", 12}, {"bVoiceModulationFix", 13}, {"bSneakBoundingBoxFix", 14},
	{"bEnableNVACAlerts", 15}, {"bLoadScreenFix", 16}, {"bNPCWeaponMods", 17}, {"uWMChancePerLevel", 18}, {"uWMChanceMin", 19}, {"uWMChanceMax", 20}});

bool s_bigGunsSkill = false, s_failedScriptLocks = false, s_NVACAlerts = false, s_NPCWeaponMods = false;
UInt32 s_NVACAddress = 0;
char *s_bigGunsDescription = "The Big Guns skill determines your combat effectiveness with all oversized weapons such as the Fat Man, Missile Launcher, Flamer, Minigun, Gatling Laser, etc.";

char __fastcall SetOptionalPatch(UInt32 patchID, bool bEnable)
{
	switch (patchID)
	{
		default:
		case 0:
			return 0;
		case 1:
			if (HOOK_SET(CalculateHitDamage, bEnable))
			{
				if (!bEnable) s_localizedDTDR = false;
				return 1;
			}
			return 0;
		case 2:
		{
			if (!HOOK_SET(EnableRepairButton, bEnable))
				return 0;
			SafeWrite8(0x7818D3, bEnable ? 0x77 : 0x7A);
			HOOK_SET(PopulateRepairList, bEnable);
			HOOK_SET(SetRepairListValues, bEnable);
			HOOK_SET(DoRepairItem, bEnable);
			HOOK_SET(RepairMenuClick, bEnable);
			return 2;
		}
		case 3:
		{
			if (s_bigGunsSkill == bEnable)
				return 0;
			s_bigGunsSkill = bEnable;
			ActorValueInfo *avInfo = g_actorValueInfoArray[kAVCode_BigGuns];
			if (bEnable)
			{
				avInfo->avFlags = 0x410;
				s_descriptionChanges[&avInfo->description] = s_bigGunsDescription;
			}
			else
			{
				avInfo->avFlags = 0x2000;
				s_descriptionChanges.Erase(&avInfo->description);
			}
			HOOK_SET(GetDescription, !s_descriptionChanges.Empty());
			return 3;
		}
		case 4:
			return HOOK_SET(InitMissileFlags, bEnable) ? 4 : 0;
		case 5:
			return HOOK_SET(UpdateTimeGlobals, bEnable) ? 5 : 0;
		case 6:
		{
			if (s_hardcoreNeedsFix == bEnable)
				return 0;
			s_hardcoreNeedsFix = bEnable;
			return 6;
		}
		case 7:
		{
			if (!bEnable || s_failedScriptLocks)
				return 0;
			s_failedScriptLocks = true;
			SafeWrite16(0x5E0FBE, 0x7AEB);
			SafeWrite16(0x5E1A23, 0x0DEB);
			SafeWrite16(0x5E1F0B, 0x0DEB);
			return 7;
		}
		case 8:
			return HOOK_SET(DoOperator, bEnable) ? 8 : 0;
		case 9:
		{
			if (!HOOK_SET(QttSelectInventory, bEnable))
				return 0;
			HOOK_SET(QttSelectContainer, bEnable);
			HOOK_SET(QttSelectBarter, bEnable);
			SafeWrite16(0x75BF18, bEnable ? 0x63EB : 0x0D8B);
			return 9;
		}
		case 10:
			return HOOK_SET(RefreshHPBarDelay, bEnable) ? 10 : 0;
		case 11:
			return HOOK_SET(DamageToWeapon, bEnable) ? 11 : 0;
		case 12:
			if ((s_localizedDTDR == bEnable) || !HOOK_INSTALLED(CalculateHitDamage))
				return 0;
			s_localizedDTDR = bEnable;
			return 12;
		case 13:
			if (!HOOK_SET(VoiceModulationFix, bEnable))
				return 0;
			SafeWrite8(0x578E16, bEnable ? 8 : 0);
			SafeWrite8(0x7974CC, bEnable ? 8 : 0);
			return 13;
		case 14:
			if (!HOOK_SET(InitControllerShape, bEnable))
				return 0;
			HOOK_SET(SneakBoundingBoxFix, bEnable);
			return 14;
		case 15:
		{
			if (!s_NVACAddress || (s_NVACAlerts == bEnable))
				return 0;
			UInt32 patchAddr = s_NVACAddress + 0x14B0;
			if (*(UInt32*)patchAddr != 0xFF102474)
				return 0;
			s_NVACAlerts = bEnable;
			patchAddr += 0x10;
			if (bEnable)
			{
				UInt8 buffer[10];
				*(UInt16*)buffer = 0x5C6;
				*(UInt32*)(buffer + 2) = (UInt32)&s_NVACLogUpdated;
				*(UInt32*)(buffer + 6) = 0x8C201;
				SafeWriteBuf(patchAddr, buffer, sizeof(buffer));
				MainLoopAddCallbackEx(CheckNVACLog, NULL, 0, 60);
				s_NVACLogUpdated = true;
			}
			else
			{
				SafeWrite32(patchAddr, 0x8C2);
				MainLoopRemoveCallback(CheckNVACLog);
			}
			return 15;
		}
		case 16:
		{
			if (!HOOK_SET(GetSuitableLoadScreens, bEnable))
				return 0;
			HOOK_SET(PickLoadScreen, bEnable);
			if (bEnable && s_locationLoadScreens.Empty())
			{
				ListNode<TESLoadScreen> *lscrIter = g_dataHandler->loadScreenList.Head();
				TESLoadScreen *loadScreen;
				do
				{
					loadScreen = lscrIter->data;
					if (!loadScreen) continue;
					if (loadScreen->locations.Empty())
						s_genericLoadScreens.Append(loadScreen);
					else s_locationLoadScreens.Append(loadScreen);
				}
				while (lscrIter = lscrIter->next);
			}
			return 16;
		}
		case 17:
		{
			if (!bEnable || s_NPCWeaponMods || !s_uWMChancePerLevel || !s_uWMChanceMin || !s_uWMChanceMax)
				return 0;
			s_NPCWeaponMods = true;
			WriteRelCall(0x452220, (UInt32)DoOnLoadActorHook);
			WriteRelCall(0x54E2A4, (UInt32)ResetActorFlagsRespawnHook);
			return 17;
		}
	}
}

UnorderedMap<const char*, FontInfo*> s_fontInfosMap;
char *s_extraFontsPaths[80] = {NULL};

__declspec(naked) void InitFontManagerHook()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	ebx
		push	esi
		push	edi
		mov		esi, 0x11F33F8
		cmp		dword ptr [esi], 0
		jnz		done
		push	0x164
		GAME_HEAP_ALLOC
		mov		[esi], eax
		mov		[ebp-4], eax
		mov		dword ptr [eax+0x20], 0
		mov		esi, eax
		mov		edi, 0x11F3418
		mov		ebx, 1
	defFontHead:
		push	0x54
		GAME_HEAP_ALLOC
		push	1
		push	dword ptr [edi]
		push	ebx
		mov		ecx, eax
		CALL_EAX(kAddr_InitFontInfo)
		mov		[esi], eax
		push	eax
		push	dword ptr [edi]
		mov		ecx, offset s_fontInfosMap
		call	UnorderedMap<const char*, FontInfo*>::InsertNotIn
		add		esi, 4
		add		edi, 0xC
		inc		ebx
		cmp		bl, 9
		jb		defFontHead
		mov		eax, [ebp-4]
		mov		eax, [eax+8]
		mov		[ebp-4], eax
		mov		edi, offset s_extraFontsPaths
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
		GAME_HEAP_ALLOC
		push	1
		push	dword ptr [edi]
		push	ebx
		mov		ecx, eax
		CALL_EAX(kAddr_InitFontInfo)
		cmp		dword ptr [eax+0x38], 0
		jz		doFree
		push	eax
		push	dword ptr [edi]
		mov		ecx, offset s_fontInfosMap
		call	UnorderedMap<const char*, FontInfo*>::InsertNotIn
		jmp		extFontNext
	doFree:
		push	eax
		GAME_HEAP_FREE
	useDefault:
		mov		eax, [ebp-4]
	extFontNext:
		add		esi, 4
		mov		[esi], eax
		add		edi, 4
		jmp		extFontHead
	done:
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn
	}
}

bool ProcessCustomINI()
{
	if (s_INISettingsMap.Empty()) InitSettingMaps();
	char customINIPath[MAX_PATH];
	memcpy(StrCopy(customINIPath, (char*)0x1202FA0), "FalloutCustom.ini", 18);
	if (FileExists(customINIPath))
	{
		SInt32 namesLen = GetPrivateProfileSectionNames(s_strArgBuffer, kMaxMessageLength, customINIPath), sectionLen, nameSize, pairSize;
		char *currName = s_strArgBuffer, *currPair, *delim, settingName[0x100], *endPtr;
		Setting *setting;
		while (namesLen > 0)
		{
			sectionLen = GetPrivateProfileSection(currName, s_strValBuffer, kMaxMessageLength, customINIPath);
			currPair = s_strValBuffer;
			while (sectionLen > 0)
			{
				sectionLen -= pairSize = StrLen(currPair) + 1;
				delim = GetNextToken(currPair, '=');
				endPtr = StrCopy(settingName, currPair);
				*endPtr = ':';
				StrCopy(endPtr + 1, currName);
				currPair += pairSize;
				setting = s_INISettingsMap.Get(settingName);
				if (!setting) continue;
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
			namesLen -= nameSize = StrLen(currName) + 1;
			currName += nameSize;
		}
	}
	return CdeclCall<bool>(0x4DA570);
}

UnorderedSet<const char*> s_overrideBSAFiles;

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

float s_msgDisplayTime = 2;				// Replacement float for 0x010162C0
const UInt32 kDisplayTimePatches[] =	// Instances of "D905 C0620101" (FLD DWORD PTR DS:[010162C0]) related to default UI messages.
{
	0x4825C7, 0x48261B, 0x4C5625, 0x4CB8FE, 0x4CEBE0, 0x4CEC36, 0x4CF150, 0x4CF1A9, 0x4CF57D, 0x4CF5D6, 0x4CF7FD, 0x4CF856, 0x4CFB17, 0x4CFB70,
	0x4FEEEA, 0x5013FF, 0x501494, 0x5016EA, 0x508C81, 0x508D21, 0x509695, 0x5096ED, 0x50973E, 0x50978B, 0x5097C9, 0x50980B, 0x5150C8, 0x5151CF,
	0x516F94, 0x517065, 0x518150, 0x518369, 0x51853D, 0x518601, 0x5732E4, 0x57344B, 0x5B5238, 0x5B5287, 0x5C1E88, 0x5C86FF, 0x5CDD77, 0x5D0268,
	0x5D0475, 0x5F5D36, 0x5F5EE4, 0x5F62E0, 0x5F648E, 0x5FA5D3, 0x5FA689, 0x5FA913, 0x607BAF, 0x607C65, 0x60822F, 0x608D3C, 0x615847, 0x61593E,
	0x615B0B, 0x615BF6, 0x615DF8, 0x615EFD, 0x6160FC, 0x6161F5, 0x678444, 0x70E55A, 0x70E59E, 0x7289BA, 0x733811, 0x7338CD, 0x73398F, 0x733A4A,
	0x7351AF, 0x7352C9, 0x737D34, 0x7410FF, 0x75739E, 0x758297, 0x75DE49, 0x75E1A8, 0x780508, 0x780569, 0x78084B, 0x7809B5, 0x780A03, 0x780A3F,
	0x780A8F, 0x780AD7, 0x780E85, 0x780ED6, 0x781C81, 0x781CEE, 0x78DBED, 0x78EEB5, 0x790162, 0x790489, 0x7981F6, 0x7BBFF1, 0x7BC0AD, 0x7BC16C,
	0x7BC227, 0x7BD716, 0x7BD830, 0x7BEC29, 0x7C0AFE, 0x7C0C7D, 0x7C0D39, 0x7C0DFB, 0x7C0EB6, 0x7C18C4, 0x7C2EE0, 0x7C2FFA, 0x7C3FC7, 0x7EE72F,
	0x7EE85F, 0x7EEA4E, 0x8159F8, 0x823F5A, 0x825167, 0x82BFC9, 0x8332E4, 0x833E7D, 0x834153, 0x85045E, 0x85057F, 0x8505E4, 0x8509AC, 0x8509FC,
	0x856D23, 0x878E48, 0x88C916, 0x88C96B, 0x88CB02, 0x88CBE8, 0x88CDBA, 0x88CF9A, 0x88D1EE, 0x88D842, 0x8929F4, 0x89AD85, 0x89E0AB, 0x8B957B,
	0x8BA3B1, 0x8C1E1B, 0x8C35D0, 0x8C54E5, 0x8C5525, 0x8C5581, 0x8C55C1, 0x8C5773, 0x8C57B3, 0x8C580F, 0x8C584F, 0x8C59F3, 0x8C5A33, 0x8C5A8F,
	0x8C5ACF, 0x8C5C73, 0x8C5CB3, 0x8C5D0F, 0x8C5D4F, 0x93D6C5, 0x93D744, 0x93D864, 0x93D8CE, 0x93D968, 0x93D9F6, 0x93DA52, 0x93DB06, 0x9423C8,
	0x942404, 0x942464, 0x9424B1, 0x94250C, 0x942564, 0x9425A2, 0x94260C, 0x94264E, 0x94DC0A, 0x94DD36, 0x94FDED, 0x94FE1D, 0x94FEEE, 0x94FF1E,
	0x9627C4, 0x966B3A, 0x969FC5, 0x96A001, 0x96A061, 0x96A0AE, 0x96A109, 0x96A161, 0x96A19F, 0x96A200, 0x96A242
};

void PatchDisplayTime(bool enable = true)
{
	UInt32 valueAddr = enable ? (UInt32)&s_msgDisplayTime : 0x10162C0;
	for (UInt32 patchAddr : kDisplayTimePatches)
		SafeWrite32(patchAddr, valueAddr);
}

UInt32 s_deferrSetOptional = 0;

void InitGamePatches()
{
	CdeclCall(0x476C00);

	//	Adds NULL pointer checks
	SafeWriteBuf(0x559450, "\x8B\xC1\x85\xC0\x74\x02\x8B\x00\xC3", 9);
	SafeWriteBuf(0x5A8ECF, "\x8B\x45\xFC\x8B\x40\x04\x89\x45\xFC\x85\xD2\x74\x0D\xC7\x42\x04\x00\x00\x00\x00\xEB\xCD", 22);

	//	Earn Steam achievements even if the console has been used
	SafeWrite16(0x5D3B69, 0x9066);

	//	Don't reset Actor::actorFlags for creatures, upon LoadGame
	SafeWrite16(0x8D5003, 0x0BEB);

	//	Increase expression-parser buffer size to 0x400
	SafeWrite8(0x593DB6, 7);
	SafeWrite16(0x593E5A, 0xF8B4);

	//	Recognize custom ashpile activators
	for (UInt32 patchAddr : {0x56A30A, 0x573FE8, 0x57445B, 0x576AF0, 0x624958, 0x77765A, 0x95F7FE})
		SafeWriteBuf(patchAddr, "\x8B\x41\x20\x66\xF7\x40\x06\x20\x00\x0F\x95\xC0\x34\x01\xEB\x0B", 16);
	SafeWriteBuf(0x573194, "\x8B\x41\x20\x66\xF7\x40\x06\x20\x00\x0F\x95\xC0\x34\x01\xEB\x0E", 16);

	s_NVACAddress = (UInt32)GetModuleHandle("nvac");
	if (!s_NVACAddress)		//	NVAC already patches those
	{
		SafeWrite16(0x57D3B9, 0x1AEB);	//	Increase grass fade distance
		SafeWrite8(0xC44405, 4);		//	Increase buffer size for GetPrivateProfileString
	}

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
	SafeWriteBuf(0x728E10, "\x0F\x1F\x84\x00\x00\x00\x00\x00", 8);
	SafeWrite32(0x729616, 0x1030018);

	//	Don't play equip sound if item is already equipped
	SafeWrite8(0x88DCDC, 0);

	//	Fix sun glare texture
	SafeWrite8(0x63E705, 1);

	//	1st person shell casing lifetime fix
	WriteRelCall(0x5251A9, 0x8D0360);

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
	SafeWriteBuf(0x7AA80F, "\x8B\x01\x8B\x40\x04\x85\xC0\x75\x03\xEB\x68", 11);

	//	AI Z fix (credits to Stewie)
	SafeWriteBuf(0x6B10A3, "\x0F\x1F\x44\x00\x00", 5);
	SafeWriteBuf(0x881A9C, "\x0F\x1F\x44\x00\x00", 5);
	SafeWriteBuf(0x9E6A82, "\x0F\x1F\x44\x00\x00", 5);

	//	Fix SetLevel XP
	SafeWriteBuf(0x8D535F, "\x0F\x1F\x00", 3);

	//	ScriptRunner::Run skip Script::GetName calls
	SafeWrite16(0x5E0DD1, 0x39EB);

	//	Store parent TESTopic for every TESTopicInfo
	SafeWrite8(0x468CE7, 0x54);
	WriteRelCall(0x61CD16, (UInt32)InitTopicInfoHook);
	WriteRelCall(0x61E086, (UInt32)LoadTopicInfoHook);
	SafeWriteBuf(0x61A310, "\x8B\x44\x24\x04\x8B\x40\x50\xC3", 8);
	SafeWriteBuf(0x61D041, "\x8B\x42\x50\x66\x0F\x1F\x44\x00\x00", 9);

	//	Ignore disabled MusicMarkers
	SafeWriteBuf(0x9699C5, "\x8B\x01\xF7\x40\x08\x00\x08\x00\x00\x0F\x85\xC0\x00\x00\x00\x83\xC0\x30\x66\x90", 20);

	//	Fix source weapon for kPerkEntry_AdjustExplosionRadius conditions
	SafeWriteBuf(0x47667D, "\x89\xC8\x0F\x1F\x00", 5);
	SafeWriteBuf(0x9C35F7, "\x89\xC8\x0F\x1F\x00", 5);

	SafeWrite16(0x94E5F6, 0x18EB);
	SafeWrite32(0x1016DB8, (UInt32)DoQueuedPlayerHook);
	SafeWriteBuf(0x601C30, "\x8B\x41\x04\x85\xC0\x74\x01\xC3\x81\xE9\xD8\x00\x00\x00\xF7\x01\x01\x00\x00\x00\xB8\x48\x3E\x1D\x01\xB9\x1C\x27\x1D\x01\x0F\x45\xC1\x8B\x00\xC3", 36);
	SafeWrite32(0x104A1BC, (UInt32)SetNPCModelHook);
	SafeWrite8(0x40F75B, 0x18);

	SafeWrite32(0x9ED3F8, (UInt32)&s_moveAwayDistance);
	SafeWrite32(0x9ED528, (UInt32)&s_moveAwayDistance);

	SafeWriteBuf(0x4F15A0, "\x0F\xB6\x44\x24\x04\x89\x41\x04\x3C\x3B\x74\x07\x3C\x3C\x74\x03\xC2\x04\x00\x31\xD2\x89\x91\x04\x01\x00\x00\xC2\x04\x00", 30);
	WriteRelJump(0x5A64C0, (UInt32)TESGlobalLoadFormHook);
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
	WriteRelJump(0xA23A19, (UInt32)GetMouseWheelHook);
	WriteRelCall(0x5B54E7, (UInt32)RemoveMeHook);
	WriteRelJump(0x8DAA6F, (UInt32)PackageSetRunHook);
	WritePushRetRelJump(0x8AB6D9, 0x8AB6F3, (UInt32)ResetActorFlagsHook);
	WriteRelCall(0x73005C, (UInt32)BarterSellFixHook);
	WriteRelJump(0xCD4468, (UInt32)JumpHeightFixHook);
	WritePushRetRelJump(0x80ECA8, 0x80ECD2, (UInt32)AddLightObjectHook);
	WritePushRetRelJump(0x4069D4, 0x406A9D, (UInt32)EffectSkillModifiers1Hook);
	WritePushRetRelJump(0x816519, 0x816627, (UInt32)EffectSkillModifiers2Hook);
	WriteRelJump(0x7294DC, (UInt32)RecipeMenuAmmoEffectsHook);
	WriteRelJump(0x7AC179, (UInt32)QttMenuEnableWheelHook);
	WriteRelCall(0x59D0B3, (UInt32)TerminalGetLockedHook);
	WritePushRetRelJump(0x59E039, 0x59E04C, (UInt32)IsCurrentFurnitureObjFixHook);
	WritePushRetRelJump(0x947DD2, 0x947E79, (UInt32)BuildMusicMarkerListHook);
	WritePushRetRelJump(0x5DBCFD, 0x5DBD23, (UInt32)AttachAshPileHook);
	WriteRelCall(0x63C4D6, (UInt32)GetWindSpeedHook);
	WriteRelCall(0x63C52C, (UInt32)GetWindSpeedHook);
	WritePushRetRelJump(0x4C3998, 0x4C39B0, (UInt32)RemoveHotkeyFixHook);
	WritePushRetRelJump(0x9B6335, 0x9B6342, (UInt32)ArmourPenetratedHook);
	WritePushRetRelJump(0x5674D5, 0x567554, (UInt32)SetScaleHook);
	SafeWrite16(0x567709, 0x15EB);
	WriteRelJump(0x4BD820, (UInt32)GetEntryDataModFlagsHook);
	WriteRelJump(0x4BDA70, (UInt32)GetEntryDataHasModHook);
	WritePushRetRelJump(0x777123, 0x77712E, (UInt32)RolloverWeaponHook);
	WritePushRetRelJump(0x782876, 0x7828F8, (UInt32)ConstructItemEntryNameHook);
	WritePushRetRelJump(0x75D1A5, 0x75D1EB, (UInt32)ConstructItemEntryNameHook);
	SafeWriteBuf(0x72F337, "\x8B\x85\x70\xFF\xFF\xFF", 6);
	WritePushRetRelJump(0x72F33D, 0x72F37F, (UInt32)ConstructItemEntryNameHook);
	WriteRelCall(0x48E761, (UInt32)GetIconPathForItemHook);
	WriteRelCall(0x406720, (UInt32)GetEffectHiddenHook);
	WritePushRetRelJump(0x440D0D, 0x440D43, (UInt32)QueuedRefStopFadeInHook);
	SafeWrite8(0xA081A8, 5);
	SafeWrite32(0xA08718, (UInt32)ProcessGradualSetFloatHook);
	WritePushRetRelJump(0x454576, 0x454580, (UInt32)CloudsFixHook);
	WriteRelCall(0x9D10F3, (UInt32)ReactionCooldownCheckHook);
	WritePushRetRelJump(0x9F50C4, 0x9F50E7, (UInt32)IsValidAITargetHook);
	WritePushRetRelJump(0x5719ED, 0x571A27, (UInt32)PickWeaponModelHook);
	WritePushRetRelJump(0x83AD97, 0x83AE4F, (UInt32)PlayAttackSoundHook);
	WriteRelJump(0x9BD507, (UInt32)AddProjectileLightHook);
	WriteRelCall(0x89DC0E, (UInt32)KillChallengeFixHook);
	WriteRelCall(0x72880D, (UInt32)IsDisposableWeaponHook);
	WriteRelJump(0x984156, (UInt32)DeathResponseFixHook);
	SafeWriteBuf(0x9EE367, "\x8B\x8A\xB0\x00\x00\x00\x83\xB9\x08\x01\x00\x00\x02\x0F\x84\x82\x00\x00\x00\xEB\x41", 21);
	SafeWrite16(0x54E421, 0x1EEB);
	WritePushRetRelJump(0x54E4E1, 0x54E5E6, (UInt32)ClearAshPilesHook);
	WritePushRetRelJump(0x82D8CA, 0x82D8F4, (UInt32)SetAcousticSpaceFixHook);
	SafeWriteBuf(0xAEACAA, "\x31\xD2\x89\x90\x34\x01\x00\x00\x89\x90\x9C\x01\x00\x00\x89\x90\xA0\x01\x00\x00\x89\x90\xDC\x01\x00\x00\xC7\x80\x38\x01\x00\x00\x00\x00\x80\x3F\x0F\x1F\x00", 39);
	SafeWrite32(0xAE5648, (UInt32)SetSourceSoundRequestHook);
	WritePushRetRelJump(0x82D416, 0x82D426, (UInt32)FillGameSoundPropsHook);
	SafeWrite32(0x10A3C58, (UInt32)AdjustSoundFrequencyHook);
	SafeWrite32(0x10A3C5C, (UInt32)GetSoundFrequencyPercHook);
	WriteRelJump(0x58E9D0, (UInt32)GetImpactDataHook);
	WriteRelCall(0x5956BC, (UInt32)GetFactionReactionHook);
	*(UInt32*)0x11D0190 = 0x41200000;
	WritePushRetRelJump(0x8C17CE, 0x8C17EA, (UInt32)GetActorAgilityHook);
	WritePushRetRelJump(0x8C194E, 0x8C196A, (UInt32)GetActorAgilityHook);
	WriteRelJump(0x595560, (UInt32)PickMediaSetHook);
	WriteRelCall(0x969C41, (UInt32)ModHardcoreNeedsHook);
	WriteRelCall(0x86AC60, (UInt32)ProcessCustomINI);

	WritePushRetRelJump(0x9B6E81, 0x9B6EC4, (UInt32)SetHitLocationHook);
	WriteRelCall(0x7ECE32, (UInt32)GetVATSTargetDTHook);
	SafeWrite32(0x108A9B4, (UInt32)PlayerCannotBeHitHook);
	WriteRelJump(0x5D2860, (UInt32)GetHitIMODHook);
	WriteRelCall(0x9ACEC6, (UInt32)GetExplosionInFaceIMODHook);
	WriteRelJump(0x9AD2C5, (UInt32)ExplosionScreenShakeHook);
	WriteRelCall(0x8A04B2, (UInt32)GetControlFlagHook);

	HOOK_INIT_JUMP(CalculateHitDamage, 0x9B5A30);
	HOOK_INIT_JPRT(EnableRepairButton, 0x7818C8, 0x7818D3);
	HOOK_INIT_JPRT(PopulateRepairList, 0x4D4C11, 0x4D4C26);
	HOOK_INIT_JPRT(SetRepairListValues, 0x7B58DB, 0x7B5927);
	HOOK_INIT_JPRT(DoRepairItem, 0x7B5DAD, 0x7B5E08);
	HOOK_INIT_JPRT(RepairMenuClick, 0x7B5CED, 0x7B5D02);
	HOOK_INIT_JPRT(DamageToWeapon, 0x646266, 0x64629A);
	HOOK_INIT_CALL(InitMissileFlags, 0x9B7E14);
	HOOK_INIT_JUMP(QttSelectInventory, 0x780B09);
	HOOK_INIT_JUMP(QttSelectContainer, 0x75BF97);
	HOOK_INIT_JUMP(QttSelectBarter, 0x72D8B4);
	HOOK_INIT_JPRT(RefreshHPBarDelay, 0xAA4FC7, 0xAA4FE9);
	HOOK_INIT_JPRT(VoiceModulationFix, 0x934AC8, 0x934AD2);
	HOOK_INIT_CALL(SneakBoundingBoxFix, 0x770B0E);
	HOOK_INIT_JPRT(InitControllerShape, 0xC72EA3, 0xC72EB3);
	HOOK_INIT_JUMP(GetSuitableLoadScreens, 0x78AC60);
	HOOK_INIT_JPRT(PickLoadScreen, 0x78A79B, 0x78A7A5);
	HOOK_INIT_JUMP(UpdateTimeGlobals, 0x867A40);
	HOOK_INIT_JUMP(DoOperator, 0x593FBC);

	if (FileExists("data\\textures\\missing_texture.dds"))
	{
		s_missingTextures.Create("missing_textures.log");
		WriteRelJump(0xAFE196, (UInt32)DefaultTextureHook);
	}

	char *namePtr = StrLenCopy(s_strArgBuffer, "Data\\NVSE\\plugins\\xfonts\\", 25), *dataPtr, *delim;
	SInt32 lines;
	UInt32 size, index, value;
	memcpy(namePtr, "*.txt", 6);
	for (DirectoryIterator dirIter(s_strArgBuffer); !dirIter.End(); dirIter.Next())
	{
		if (!dirIter.IsFile()) continue;
		StrCopy(namePtr, dirIter.Get());
		LineIterator lineIter(s_strArgBuffer, s_strValBuffer);
		while (!lineIter.End())
		{
			dataPtr = lineIter.Get();
			lineIter.Next();
			delim = GetNextToken(dataPtr, '=');
			size = StrLen(delim);
			if (!size) continue;
			index = StrToInt(dataPtr) - 10;
			if ((index > 79) || s_extraFontsPaths[index]) continue;
			StrCopy(s_dataPath, delim);
			if (FileExists(s_dataPathFull))
				s_extraFontsPaths[index] = (char*)memcpy(malloc(size + 1), delim, size + 1);
		}
	}
	WriteRelCall(0x70B285, (UInt32)InitFontManagerHook);
	SafeWrite8(0xA1B032, 0x59);

	for (DirectoryIterator dirIter("Data\\*.override"); !dirIter.End(); dirIter.Next())
	{
		if (dirIter.IsFile())
		{
			memcpy(StrCopy(StrLenCopy(s_strArgBuffer, "Data\\", 5), dirIter.Get()) - 8, "bsa", 4);
			s_overrideBSAFiles.Insert(s_strArgBuffer);
		}
	}
	if (!s_overrideBSAFiles.Empty())
		WriteRelCall(0x463855, (UInt32)LoadBSAFileHook);

	lines = GetPrivateProfileSection("GamePatches", s_strValBuffer, 0x10000, "Data\\NVSE\\plugins\\jip_nvse.ini");
	dataPtr = s_strValBuffer;
	while (lines > 0)
	{
		lines -= size = StrLen(dataPtr) + 1;
		delim = GetNextToken(dataPtr, '=');
		index = s_optionalHacks.Get(dataPtr);
		dataPtr += size;
		if (!index) continue;
		if (index >= 18)
		{
			value = StrToInt(delim);
			switch (index)
			{
			case 18:
				s_uWMChancePerLevel = value;
				break;
			case 19:
				s_uWMChanceMin = value;
				break;
			case 20:
				s_uWMChanceMax = value;
				break;
			}
		}
		else if (*delim == '1')
		{
			if ((index == 3) || (index >= 15))
				s_deferrSetOptional |= 1 << index;
			else SetOptionalPatch(index, true);
		}
	}

	PatchDisplayTime();

	PrintLog("> Game patches initialized successfully.");
}

typedef int (*_ReloadENB)(UInt32 type, void *data);
_ReloadENB ReloadENB = NULL;

void DeferredInit()
{
	g_pipBoyLight = *(SpellItem**)0x11C358C;
	g_modelLoader = *(ModelLoader**)0x11C3B3C;
	g_dataHandler = *(DataHandler**)0x11C3F2C;
	g_interfaceManager = *(InterfaceManager**)0x11D8A80;
	g_OSGlobals = *(OSGlobals**)0x11DEA0C;
	g_TES = *(TES**)0x11DEA10;
	g_thePlayer = *(PlayerCharacter**)0x11DEA3C;
	g_sceneGraph = *(SceneGraph**)0x11DEB7C;
	g_scrapHeapQueue = *(void**)0x11DF1A8;
	g_fontManager = *(FontManager**)0x11F33F8;
	g_inputGlobals = *(OSInputGlobals**)0x11F35CC;
	g_tileMenuArray = *(TileMenu***)0x11F350C;
	g_HUDMainMenu = *(HUDMainMenu**)0x11D96C0;
	g_consoleManager = ConsoleManager::GetSingleton();
	g_sysColorManager = *(SystemColorManager**)0x11D8A88;
	g_screenWidth = *(UInt32*)0x11C73E0;
	g_screenHeight = *(UInt32*)0x11C7190;
	g_terminalModelDefault = *g_terminalModelPtr;
	g_capsItem = (TESObjectMISC*)LookupFormByRefID(0xF);
	g_getHitIMOD = (TESImageSpaceModifier*)LookupFormByRefID(0x162);
	g_explosionInFaceIMOD = (TESImageSpaceModifier*)LookupFormByRefID(0x166);
	g_fistsWeapon = *(TESObjectWEAP**)0x11CA278;
	g_ashPileACTI = *(TESObjectACTI**)0x11CA27C;
	g_gooPileACTI = *(TESObjectACTI**)0x11CA280;
	g_ashPileACTI->SetJIPFlag(kHookFormFlag6_IsAshPile, true);
	g_gooPileACTI->SetJIPFlag(kHookFormFlag6_IsAshPile, true);

	s_tempPosMarker = ThisCall<TESObjectREFR*>(0x55A2F0, GameHeapAlloc(sizeof(TESObjectREFR)));
	ThisCall(0x484490, s_tempPosMarker);

	g_eventCmdInfos[1].execute = Hook_MenuMode_Execute;
	g_eventCmdInfos[0xE].execute = Cmd_EmptyCommand_Execute;

	g_condDmgPenalty = *(double**)0x646D25;
	if ((UInt32)g_condDmgPenalty == 0x1051680)
	{
		SafeWrite32(0x646D25, (UInt32)&s_condDmgPenalty);
		g_condDmgPenalty = &s_condDmgPenalty;
	}

	for (UInt32 index = 0; index <= 17; index++)
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
		s_fontHeightDatas[length].heightBase = (*fontInfos)->bufferData->baseHeight;
		s_fontHeightDatas[length].heightwGap = (*fontInfos)->bufferData->lineHeight;
	}
	s_fontHeightDatas[2].heightwGap += 4.0F;
	s_fontHeightDatas[3].heightwGap += 6.0F;
	s_fontHeightDatas[6].heightwGap += 4.0F;
	s_fontHeightDatas[7].heightwGap += 2.0F;

	(*g_BSWin32Audio)->PickSoundFileFromFolder = PickSoundFileFromFolderHook;

	GetModuleFileName(GetModuleHandle(NULL), s_strArgBuffer, MAX_PATH);
	char *delim = SlashPosR(s_strArgBuffer) + 1;
	memcpy(delim, "d3d9.dll", 9);
	HMODULE handle = GetModuleHandle(s_strArgBuffer);
	if (handle) ReloadENB = (_ReloadENB)GetProcAddress(handle, "DirtyHack");
	if (!ReloadENB)
	{
		memcpy(delim, "enbseries.dll", 14);
		handle = GetModuleHandle(s_strArgBuffer);
		if (handle) ReloadENB = (_ReloadENB)GetProcAddress(handle, "DirtyHack");
	}

	ActorValueInfo *avInfo = g_actorValueInfoArray[kAVCode_BigGuns];
	avInfo->fullName.name = avInfo->avName;
	avInfo->callback4C = 0x643BD0;

	JIPScriptRunner::Init();

	ThisCall(0x4AD050, &s_NiObjectCopyInfo, 0x3F800000);

	UInt8 cccIdx = g_dataHandler->GetModIndex("JIP Companions Command & Control.esp");
	if (cccIdx != 0xFF)
	{
		Script *cccMain = (Script*)LookupFormByRefID((cccIdx << 24) | 0xADE);
		if (cccMain && (*(UInt32*)(cccMain->data + 0x29C) == ' 931'))
			*(UInt32*)(cccMain->data + 0x29C) = ' 552';
	}

	if (s_NVACAddress)
	{
		FILE *nvacLog = _fsopen("nvac.log", "ab", 0x20);
		if (nvacLog)
		{
			fprintf(nvacLog, "NVSE version: %.2f\tJIP LN version: %.2f\tBase address: %08X\n", s_nvseVersion, JIP_LN_VERSION, (UInt32)GetModuleHandle("jip_nvse"));
			fclose(nvacLog);
		}
	}

	Console_Print("JIP LN version: %.2f", JIP_LN_VERSION);
}

void InitEditorPatches()
{
	
}