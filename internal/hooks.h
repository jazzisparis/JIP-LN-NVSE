#pragma once

enum
{
	kHook_GetDescription,
	kHook_PCFastTravel,
	kHook_PCCellChange,
	kHook_TextInputClose,
	kHook_StartCombat,
	kHook_SetCombatTarget,
	kHook_ResetHPCommand,
	kHook_ResetHPWakeUp,
	kHook_RemoveAmmo,
	kHook_ActivateDoor,
	kHook_TeleportWithPC,
	kHook_EquipItem,
	kHook_ReEquipAll,
	kHook_WeaponSwitchSelect,
	kHook_WeaponSwitchUnequip,
	kHook_GetPreferedWeapon,
	kHook_ShowMessage,
	kHook_ShowTutorial,
	kHook_MenuStateOpen,
	kHook_MenuStateClose,
	kHook_SetAnimAction,
	kHook_SetAnimGroup,
	kHook_DamageActorValue,
	kHook_InitMoon,
	kHook_UpdateWeather,
	kHook_MenuHandleMouseover,
	kHook_GetDetectionValue,
	kHook_SetPCTarget,
	kHook_PCActivateRef,
	kHook_SetRolloverText,
	kHook_MergeEventMask,
	kHook_MarkScriptEvent,
	kHook_DoActivate,
	kHook_CreateMapMarkers,
	kHook_GetRefName,
	kHook_SetQuestStage,
	kHook_RunResultScript,
	kHook_ScriptRunner,
	kHook_EvalEventBlock,
	kHook_SetTerminalModel,
	kHook_AddVATSTarget,
	kHook_LocationDiscover,
	kHook_ItemCrafted,
	kHook_MakeObjLODPath,
	kHook_OnHitEvent,
	kHook_CheckUniqueItem,
	kHook_ProjectileImpact,
	kHook_AddNote,
	kHook_EquipAidItem,
	kHook_ReloadWeapon,
	kHook_OnRagdoll,
	kHook_PlayerMinHealth,
	kHook_ApplyActorVelocity,
	kHook_InsertObject,
	kHook_SynchronizePosition,
	kHook_CalculateHitDamage,
	kHook_EnableRepairButton,
	kHook_PopulateRepairList,
	kHook_SetRepairListValues,
	kHook_DoRepairItem,
	kHook_RepairMenuClick,
	kHook_DamageToWeapon,
	kHook_InitMissileFlags,
	kHook_QttSelectInventory,
	kHook_QttSelectContainer,
	kHook_QttSelectBarter,
	kHook_RefreshHPBarDelay,
	kHook_VoiceModulationFix,
	kHook_SneakBoundingBoxFix,
	kHook_InitControllerShape,
	kHook_GetSuitableLoadScreens,
	kHook_PickLoadScreen,
	kHook_UpdateTimeGlobals,
	kHook_DoOperator,
#if JIP_DEBUG
	kHook_JIPTest,
#endif
	kHook_Max,

	kHookFormFlag6_MessageDisabled =		1 << 0,
	kHookFormFlag6_SetStageHandlers =		kHookFormFlag6_MessageDisabled,
	kHookFormFlag6_ProjectileImpact =		kHookFormFlag6_MessageDisabled,
	kHookFormFlag6_OnEquipHandlers =		kHookFormFlag6_MessageDisabled,
	kHookFormFlag6_NoPCActivation =			1 << 1,
	kHookFormFlag6_EventDisabled =			1 << 2,
	kHookFormFlag6_ActivateDisabled =		1 << 3,
	kHookFormFlag6_AltRefName =				1 << 4,
	kHookFormFlag6_IsAshPile =				1 << 5,
	kHookFormFlag6_UniqueItem =				1 << 6,
	kHookFormFlag6_InsertNode =				1 << 7,
	kHookFormFlag6_AttachModel =			1 << 8,
	kHookFormFlag6_InsertObject =			kHookFormFlag6_InsertNode | kHookFormFlag6_AttachModel,

	kHookFormFlag5_FastTravelInformed =		1 << 0,
	kHookFormFlag5_CellChangeInformed =		1 << 1,
	kHookFormFlag5_MenuInputInformed =		1 << 2,
	kHookFormFlag5_ScriptOnWait =			1 << 3,

	kHookActorFlag1_CombatDisabled =		1 << 0,
	kHookActorFlag1_ForceCombatTarget =		1 << 1,
	kHookActorFlag1_CombatAIModified =		kHookActorFlag1_CombatDisabled | kHookActorFlag1_ForceCombatTarget,
	kHookActorFlag1_InfiniteAmmo =			1 << 2,
	kHookActorFlag1_LockedEquipment =		1 << 3,
	kHookActorFlag1_DetectionFix =			1 << 4,
	kHookActorFlag1_PCTeleportWait =		1 << 5,
	kHookActorFlag1_PCTeleportFollow =		1 << 6,
	kHookActorFlag1_PCTeleportAI =			kHookActorFlag1_PCTeleportWait | kHookActorFlag1_PCTeleportFollow,
	kHookActorFlag1_DisableWheel =			1 << 7,

	kHookActorFlag2_ForceDetectionVal =		1 << 0,
	kHookActorFlag2_NonTargetable =			1 << 1,
	kHookActorFlag2_TeammateKillable =		1 << 2,
	kHookActorFlag2_NoGunWobble =			1 << 3,

	kHookActorFlag3_OnAnimAction =			1 << 0,
	kHookActorFlag3_OnPlayGroup =			1 << 1,
	kHookActorFlag3_OnHealthDamage =		1 << 2,
	kHookActorFlag3_OnCrippledLimb =		1 << 3,
	kHookActorFlag3_OnFireWeapon =			1 << 4,
	kHookActorFlag3_OnHit =					1 << 5,

	kHookActorFlags_DetectionModified =		kHookActorFlag1_DetectionFix | (kHookActorFlag2_ForceDetectionVal << 8),
};

struct HookInfo
{
	UInt32		patchAddr;
	UInt8		*instructions;
	UInt32		length;
	UInt32		userCount;

	void Init(UInt32 _patchAddr, void *hookPtr, UInt8 type, UInt32 retAddr = 0);
	bool Set(bool install);
	void ModUsers(bool add);
}
s_hookInfos[kHook_Max];

__declspec(naked) void HookInfo::Init(UInt32 _patchAddr, void *hookPtr, UInt8 type, UInt32 retAddr)
{
	__asm
	{
		push	ebx
		push	esi
		mov		bl, [esp+0x14]
		mov		esi, ecx
		mov		eax, 5
		cmp		bl, 0x68
		jnz		doneLen
		mov		eax, 0xA
	doneLen:
		mov		[ecx+8], eax
		push	eax
		shl		eax, 1
		push	eax
		call	malloc
		pop		ecx
		mov		[esi+4], eax
		push	dword ptr [esp+0x10]
		push	eax
		call	MemCopy
		add		esp, 0xC
		add		eax, [esi+8]
		mov		ecx, [esp+0xC]
		mov		[esi], ecx
		mov		edx, [esp+0x10]
		sub		edx, ecx
		cmp		bl, 0x68
		jnz		notRET
		mov		ecx, [esp+0x18]
		mov		[eax+1], ecx
		mov		byte ptr [eax+5], 0xE9
		sub		edx, 0xA
		mov		[eax+6], edx
		jmp		done
	notRET:
		sub		edx, 5
		mov		[eax+1], edx
	done:
		mov		[eax], bl
		mov		dword ptr [esi+0xC], 0
#if LOG_HOOKS
		push	dword ptr [esi+8]
		push	dword ptr [esp+0x10]
		call	StoreOriginalData
#endif
		pop		esi
		pop		ebx
		retn	0x10
	}
}

__declspec(naked) bool HookInfo::Set(bool install)
{
	__asm
	{
		movzx	eax, [esp+4]
		cmp		dword ptr [ecx+0xC], 0
		setnz	dl
		cmp		al, dl
		jnz		proceed
		xor		al, al
		retn	4
	proceed:
		mov		[ecx+0xC], eax
		push	esi
		mov		esi, ecx
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	dword ptr [esi+8]
		push	dword ptr [esi]
		call	VirtualProtect
		mov		eax, [esi+8]
		push	eax
		imul	eax, [esi+0xC]
		add		eax, [esi+4]
		push	eax
		push	dword ptr [esi]
		call	MemCopy
		add		esp, 0xC
		mov		edx, [esp]
		push	esp
		push	edx
		push	dword ptr [esi+8]
		push	dword ptr [esi]
		call	VirtualProtect
		mov		al, 1
		pop		ecx
		pop		esi
		retn	4
	}
}

__declspec(naked) void HookInfo::ModUsers(bool add)
{
	__asm
	{
		mov		edx, [ecx+0xC]
		cmp		byte ptr [esp+4], 0
		jz		doDecr
		test	edx, edx
		jz		doSet
		inc		edx
		jmp		modCount
	doDecr:
		test	edx, edx
		jz		done
		cmp		edx, 1
		jz		doSet
		dec		edx
	modCount:
		mov		[ecx+0xC], edx
	done:
		retn	4
	doSet:
		jmp		HookInfo::Set
	}
}

#define HOOK_INIT_CALL(name, baseAddr) s_hookInfos[kHook_##name].Init(baseAddr, ##name##Hook, 0xE8, 0)
#define HOOK_INIT_JUMP(name, baseAddr) s_hookInfos[kHook_##name].Init(baseAddr, ##name##Hook, 0xE9, 0)
#define HOOK_INIT_JPRT(name, baseAddr, retAddr) s_hookInfos[kHook_##name].Init(baseAddr, ##name##Hook, 0x68, retAddr)
#define HOOK_INSTALLED(name) s_hookInfos[kHook_##name].userCount != 0
#define HOOK_MOD(name, add) s_hookInfos[kHook_##name].ModUsers(add)
#define HOOK_SET(name, install) s_hookInfos[kHook_##name].Set(install)

UnorderedMap<UInt32, UInt32> s_jipFormFlagsMap;

__declspec(noinline) void TESForm::SetJIPFlag(UInt16 jipFlag, bool bSet)
{
	if (bSet) jipFormFlags6 |= jipFlag;
	else jipFormFlags6 &= ~jipFlag;
	s_jipFormFlagsMap[refID] = jipFormFlags6;
}

UnorderedSet<UInt32> s_eventInformedObjects;

struct MainLoopCallback
{
	void			*cmdPtr;		// 00
	void			*thisObj;		// 04
	UInt8			numArgs;		// 08
	bool			bRemove;		// 09
	bool			isScript;		// 0A
	UInt8			flags;			// 0B
	UInt32			callCount;		// 0C
	UInt32			callDelay;		// 10
	UInt32			cycleCount;		// 14
	union							// 18
	{
		FunctionArg	arg;
		FunctionArg	*pArgs;
	};

	static MainLoopCallback *Create(void *_cmdPtr, void *_thisObj, UInt32 _callCount = 1, UInt32 _callDelay = 1, UInt8 _numArgs = 0);

	void Execute();

	void Destroy()
	{
		if (numArgs > 1)
			Pool_Free(pArgs, numArgs * sizeof(FunctionArg));
		Pool_Free(this, sizeof(MainLoopCallback));
	}
};

Vector<MainLoopCallback*> s_mainLoopCallbacks(0x50);

MainLoopCallback *MainLoopCallback::Create(void *_cmdPtr, void *_thisObj, UInt32 _callCount, UInt32 _callDelay, UInt8 _numArgs)
{
	MainLoopCallback *callback = (MainLoopCallback*)Pool_Alloc(sizeof(MainLoopCallback));
	callback->cmdPtr = _cmdPtr;
	callback->thisObj = _thisObj;
	*(UInt32*)&callback->numArgs = _numArgs;
	callback->callCount = _callCount;
	callback->callDelay = _callDelay;
	callback->cycleCount = _callDelay;
	if (_numArgs > 1)
		callback->pArgs = (FunctionArg*)Pool_Alloc(_numArgs * sizeof(FunctionArg));
	s_mainLoopCallbacks.Append(callback);
	return callback;
}

__declspec(naked) void MainLoopCallback::Execute()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		movzx	edx, byte ptr [ecx+8]
		dec		edx
		js		doExecute
		mov		eax, [ecx+0x18]
		jnz		pushArgs
		push	eax
		jmp		doExecute
		ALIGN 16
	pushArgs:
		push	dword ptr [eax+edx*4]
		dec		edx
		jns		pushArgs
		ALIGN 16
	doExecute:
		mov		eax, [ecx]
		mov		ecx, [ecx+4]
		call	eax
		leave
		retn
	}
}

MainLoopCallback* __fastcall FindMainLoopCallback(void *cmdPtr, void *thisObj = NULL)
{
	for (auto iter = s_mainLoopCallbacks.Begin(); iter; ++iter)
		if ((iter->cmdPtr == cmdPtr) && (iter->thisObj == thisObj))
			return *iter;
	return NULL;
}

bool MainLoopHasCallback(void *cmdPtr, void *thisObj = NULL)
{
	return FindMainLoopCallback(cmdPtr, thisObj) != NULL;
}

bool MainLoopRemoveCallback(void *cmdPtr, void *thisObj = NULL)
{
	MainLoopCallback *callback = FindMainLoopCallback(cmdPtr, thisObj);
	if (!callback) return false;
	callback->bRemove = true;
	return true;
}

void MainLoopAddCallback(void *cmdPtr, void *thisObj = NULL)
{
	MainLoopCallback::Create(cmdPtr, thisObj);
}

void MainLoopAddCallbackArgs(void *cmdPtr, void *thisObj, UInt8 numArgs, ...)
{
	MainLoopCallback *callback = MainLoopCallback::Create(cmdPtr, thisObj, 1, 1, numArgs);
	if (!numArgs) return;
	UInt32 *pArgs = (numArgs > 1) ? (UInt32*)callback->pArgs : (UInt32*)&callback->arg;
	va_list args;
	va_start(args, numArgs);
	do
	{
		*pArgs = va_arg(args, UInt32);
		pArgs++;
	}
	while (--numArgs);
	va_end(args);
}

void MainLoopAddCallbackEx(void *cmdPtr, void *thisObj, UInt32 callCount, UInt32 callDelay = 1)
{
	MainLoopCallback::Create(cmdPtr, thisObj, callCount, callDelay ? callDelay : 1);
}

void MainLoopAddCallbackArgsEx(void *cmdPtr, void *thisObj, UInt32 callCount, UInt32 callDelay, UInt8 numArgs, ...)
{
	MainLoopCallback *callback = MainLoopCallback::Create(cmdPtr, thisObj, callCount, callDelay ? callDelay : 1, numArgs);
	if (!numArgs) return;
	UInt32 *pArgs = (numArgs > 1) ? (UInt32*)callback->pArgs : (UInt32*)&callback->arg;
	va_list args;
	va_start(args, numArgs);
	do
	{
		*pArgs = va_arg(args, UInt32);
		pArgs++;
	}
	while (--numArgs);
	va_end(args);
}

__declspec(naked) void CycleMainLoopCallbacks()
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, offset s_mainLoopCallbacks
		mov		esi, [ebx]
		mov		edi, [ebx+4]
		ALIGN 16
	cycleHead:
		mov		ecx, [esi]
		cmp		[ecx+9], 0
		jnz		cycleNext
		cmp		[ecx+0xA], 0
		jnz		isScript
		dec		dword ptr [ecx+0x14]
		jnz		cycleNext
		dec		dword ptr [ecx+0xC]
		jnz		resetCount
		mov		[ecx+9], 1
		jmp		doExecute
	resetCount:
		mov		eax, [ecx+0x10]
		mov		[ecx+0x14], eax
	doExecute:
		call	MainLoopCallback::Execute
		jmp		cycleNext
	isScript:
		mov		eax, g_interfaceManager
		cmp		[eax+0xC], 1
		seta	al
		inc		al
		test	al, [ecx+0xB]
		jz		cycleNext
		dec		dword ptr [ecx+0x14]
		jnz		cycleNext
		mov		eax, [ecx+0x18]
		cmp		eax, 0x14
		jz		doCallFunc
		push	eax
		call	LookupFormByRefID
		mov		ecx, [esi]
		cmp		[ecx+4], eax
		jz		doCallFunc
		mov		[ecx+9], 1
		jmp		cycleNext
	doCallFunc:
		mov		edx, [ecx+0x10]
		mov		[ecx+0x14], edx
		push	0
		push	offset s_callRes
		push	0
		push	dword ptr [ecx+4]
		push	dword ptr [ecx]
		call	CallFunction
		add		esp, 0x14
		ALIGN 16
	cycleNext:
		add		esi, 4
		dec		edi
		jnz		cycleHead
		mov		eax, [ebx]
		mov		edi, [ebx+4]
		lea		esi, [eax+edi*4]
		ALIGN 16
	removeHead:
		sub		esi, 4
		mov		ecx, [esi]
		cmp		[ecx+9], 0
		jz		removeNext
		call	MainLoopCallback::Destroy
		mov		eax, [ebx+4]
		dec		dword ptr [ebx+4]
		sub		eax, edi
		jz		removeNext
		shl		eax, 2
		push	eax
		lea		edx, [esi+4]
		push	edx
		push	esi
		call	MemMove
		add		esp, 0xC
		ALIGN 16
	removeNext:
		dec		edi
		jnz		removeHead
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

bool s_sleepSemaphore = false;

__declspec(naked) void GameMainLoopHook()
{
	__asm
	{
		mov		ecx, [eax+0x288]
		push	ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		mov		[ebp-0x2EC], eax
		cmp		dword ptr ds:[s_mainLoopCallbacks.numItems], 0
		jz		doneCallbacks
		call	CycleMainLoopCallbacks
	doneCallbacks:
		cmp		s_LNEventFlags, 0
		jz		doneEvents
		call	LN_ProcessEvents
	doneEvents:
		cmp		dword ptr ds:[s_tempContChangesEntries.numEntries], 0
		jz		doneEntries
		call	DoDeferredFreeEntries
	doneEntries:
		cmp		s_HUDCursorMode, 0
		jz		doneCursor
		mov		ecx, g_interfaceManager
		cmp		dword ptr [ecx+0xC], 1
		jz		doneCursor
		mov		s_HUDCursorMode, 0
		mov		ecx, g_DIHookCtrl
		mov		byte ptr [ecx+0x709], 0
	doneCursor:
		mov		s_sleepSemaphore, 0
		retn
	}
}

__declspec(naked) void DoQueuedCmdCallHook()
{
	__asm
	{
		push	dword ptr [eax+8]
		call	LookupFormByRefID
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [ebp+8]
		mov		edx, [eax+0xC]
		test	edx, edx
		jz		doExecute
		ALIGN 16
	pushArgs:
		dec		edx
		push	dword ptr [eax+edx*4+0x10]
		jnz		pushArgs
	doExecute:
		call	dword ptr [eax+4]
	done:
		mov		ecx, [ebp-0xC]
		mov		fs:0, ecx
		pop		ecx
		leave
		retn
	}
}

UnorderedMap<TESDescription*, char*> s_descriptionChanges;

__declspec(naked) void GetDescriptionHook()
{
	__asm
	{
		mov		ds:[0x11C5490], ecx
		push	ecx
		mov		ecx, offset s_descriptionChanges
		call	UnorderedMap<TESDescription*, char*>::Get
		test	eax, eax
		jz		contRetn
		push	eax
		mov		ecx, 0x11C5498
		call	String::Set
		JMP_EAX(0x483063)
	contRetn:
		JMP_EAX(0x482F85)
	}
}

EventCallbackScripts s_fastTravelEventScripts;
typedef UnorderedSet<TESForm*> InformedObjectsMap;
InformedObjectsMap s_pcFastTravelInformed;

__declspec(naked) InterfaceManager *PCFastTravelHook()
{
	__asm
	{
		cmp		dword ptr ds:[s_fastTravelEventScripts+4], 0
		jz		noEvents
		mov		eax, ds:[0x11DA368]
		push	dword ptr [eax+0x118]
		mov		ecx, offset s_fastTravelEventScripts
		call	EventCallbackScripts::InvokeEvents
	noEvents:
		mov		ecx, offset s_pcFastTravelInformed
		call	InformedObjectsMap::Clear
		mov		eax, g_interfaceManager
		retn
	}
}

InformedObjectsMap s_pcCellChangeInformed;

__declspec(naked) UInt8 PCCellChangeHook()
{
	static TESObjectCELL *pcLastCell = NULL;
	__asm
	{
		test	eax, eax
		jz		done
		cmp		eax, pcLastCell
		jz		done
		mov		pcLastCell, eax
		mov		ecx, offset s_pcCellChangeInformed
		call	InformedObjectsMap::Clear
	done:
		mov		al, byte ptr ds:[0x11C7A5A]
		retn
	}
}

__declspec(naked) void __fastcall TextInputRefreshHook(TextEditMenu *menu)
{
	__asm
	{
		push	esi
		cmp		byte ptr [ecx+0x55], 0
		jz		done
		mov		esi, ecx
		mov		eax, ds:[0x11F63A8]
		mov		edx, eax
		sub		edx, [ecx+0x50]
		cmp		edx, 0x1F4
		jbe		doneCursor
		mov		[ecx+0x50], eax
		cmp		byte ptr [ecx+0x54], 0
		setz	al
		mov		[ecx+0x54], al
		mov		eax, 0x7C
		mov		edx, 0x7F
		cmovz	eax, edx
		mov		edx, [ecx+0x3C]
		add		edx, [ecx+0x44]
		mov		[edx], al
		push	1
		push	dword ptr [ecx+0x3C]
		push	kTileValue_string
		mov		ecx, [ecx+0x28]
		CALL_EAX(kAddr_TileSetString)
	doneCursor:
		mov		ax, [esi+0x38]
		mov		ecx, 0x3F800000
		xor		edx, edx
		cmp		ax, [esi+0x48]
		cmovb	ecx, edx
		push	1
		push	ecx
		push	kTileValue_target
		mov		ecx, [esi+0x2C]
		CALL_EAX(kAddr_TileSetFloat)
		push	kTileValue_user1
		mov		ecx, [esi+0x4C]
		CALL_EAX(kAddr_TileGetFloat)
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_user2
		mov		ecx, [esi+0x4C]
		CALL_EAX(kAddr_TileSetFloat)
	done:
		pop		esi
		retn
	}
}

bool __fastcall HandleInputKey(TextEditMenu *menu, UInt32 inputKey)
{
	if (!menu->isActive || (inputKey < 0x20))
		return false;
	SInt32 currIdx = menu->cursorIndex, newIdx;
	UInt16 length = menu->currentText.m_dataLen;
	if (inputKey < kInputCode_Backspace)
	{
		if (length >= menu->maxLength)
			return false;
		if (menu->miscFlags & 1)
		{
			if (inputKey == '.')
			{
				if (strchr(menu->currentText.m_data, '.'))
					return false;
			}
			else if ((inputKey < '0') || (inputKey > '9'))
				return false;
		}
		menu->currentText.InsertChar(inputKey, currIdx);
		menu->cursorIndex++;
		return true;
	}
	char *chrPtr;
	switch (inputKey)
	{
		case kInputCode_Backspace:
		{
			if (!currIdx) break;
			currIdx--;
			menu->cursorIndex = currIdx;
			menu->currentText.EraseAt(currIdx);
			return true;
		}
		case kInputCode_ArrowLeft:
		{
			if (!currIdx) break;
			menu->cursorIndex--;
			return true;
		}
		case kInputCode_ArrowRight:
		{
			if (currIdx >= length) break;
			menu->cursorIndex++;
			return true;
		}
		case kInputCode_ArrowUp:
		case kInputCode_ArrowDown:
			break;
		case kInputCode_Home:
		{
			chrPtr = FindChrR(menu->currentText.m_data, currIdx, '\n');
			newIdx = chrPtr ? (chrPtr - menu->currentText.m_data + 1) : 0;
			if (newIdx == currIdx) break;
			menu->cursorIndex = newIdx;
			return true;
		}
		case kInputCode_End:
		{
			chrPtr = strchr(menu->currentText.m_data + currIdx, '\n');
			newIdx = chrPtr ? (chrPtr - menu->currentText.m_data) : length;
			if (newIdx == currIdx) break;
			menu->cursorIndex = newIdx;
			return true;
		}
		case kInputCode_Delete:
		{
			if (currIdx >= length) break;
			menu->currentText.EraseAt(currIdx);
			return true;
		}
		case kInputCode_Enter:
		{
			if (length >= menu->maxLength) break;
			menu->currentText.InsertChar('\n', currIdx);
			menu->cursorIndex++;
			return true;
		}
		case kInputCode_PageUp:
		{
			if (!currIdx) break;
			currIdx -= 5;
			menu->cursorIndex = (currIdx > 0) ? currIdx : 0;
			return true;
		}
		case kInputCode_PageDown:
		{
			if (currIdx >= length) break;
			currIdx += 5;
			menu->cursorIndex = (currIdx <= length) ? currIdx : length;
			return true;
		}
		default: break;
	}
	return false;
}

__declspec(naked) bool __stdcall TextInputKeyPressHook(UInt32 inputKey)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		edx, [esp+8]
		cmp		edx, kInputCode_Enter
		jnz		passInput
		test	byte ptr [ecx+0x57], 2
		jz		passInput
		push	0xFAF
		mov		ecx, [ecx+0x2C]
		CALL_EAX(0xA01230)
		test	al, al
		jz		done
		mov		ecx, esi
		push	dword ptr [ecx+0x2C]
		push	1
		call	dword ptr ds:[0x1070040]
		jmp		done
	passInput:
		call	HandleInputKey
		test	al, al
		jz		done
		push	dword ptr [esi+0x34]
		lea		ecx, [esi+0x3C]
		call	String::Set
		push	dword ptr [esi+0x44]
		mov		eax, 0x7C
		mov		edx, 0x7F
		cmp		byte ptr [esi+0x54], 0
		cmovz	eax, edx
		push	eax
		lea		ecx, [esi+0x3C]
		call	String::InsertChar
		push	1
		push	dword ptr [esi+0x3C]
		push	kTileValue_string
		mov		ecx, [esi+0x28]
		CALL_EAX(kAddr_TileSetString)
		mov		ecx, esi
		call	TextInputRefreshHook
	done:
		mov		al, 1
		pop		esi
		retn	4
	}
}

__declspec(naked) void TextInputCloseHook()
{
	__asm
	{
		cmp		dword ptr [esp+4], 1
		jnz		done
		push	esi
		mov		esi, ecx
		mov		byte ptr [ecx+0x55], 0
		push	0x7E65F0
		push	0x1070060
		call	SafeWrite32
		push	0x7E6620
		push	0x1070064
		call	SafeWrite32
		push	0
		mov		ecx, offset s_hookInfos+kHook_TextInputClose*0x10
		call	HookInfo::Set
		mov		eax, [esi+0x34]
		mov		edx, 0x1011584
		test	eax, eax
		cmovz	eax, edx
		push	eax
		push	1
		push	offset s_callRes
		push	0
		push	0
		push	dword ptr [esi+0x58]
		call	CallFunction
		add		esp, 0x18
		pop		esi
		CALL_EAX(0x7E65B0)
	done:
		retn	8
	}
}

void __fastcall MenuHandleClickHook(Menu *menu, int EDX, int tileID, Tile *clickedTile);

struct MenuClickEvent
{
	typedef void (__thiscall *MenuHandleClick)(Menu*, int, Tile*);

	UInt32				patchAddr;
	MenuHandleClick		funcPtr;
	MenuClickFiltersMap filtersMap;
	MenuClickIDsMap		idsMap;

	MenuClickEvent(UInt32 menuVtbl)
	{
		patchAddr = menuVtbl + 0xC;
		funcPtr = (MenuHandleClick)*(UInt32*)patchAddr;
	}

	bool Empty() const {return filtersMap.Empty() && idsMap.Empty();}

	void SetHook(bool doSet) {SafeWrite32(patchAddr, doSet ? (UInt32)MenuHandleClickHook : (UInt32)funcPtr);}
}
s_menuClickEventMap[] =
{
	kVtbl_MessageMenu, kVtbl_InventoryMenu, kVtbl_StatsMenu, kVtbl_HUDMainMenu, kVtbl_LoadingMenu, 
	kVtbl_ContainerMenu, kVtbl_DialogMenu, kVtbl_SleepWaitMenu, kVtbl_StartMenu, kVtbl_LockpickMenu, 
	kVtbl_QuantityMenu, kVtbl_MapMenu, kVtbl_BookMenu, kVtbl_LevelUpMenu, kVtbl_RepairMenu, kVtbl_RaceSexMenu, 
	kVtbl_CharGenMenu, kVtbl_TextEditMenu, kVtbl_BarterMenu, kVtbl_SurgeryMenu, kVtbl_HackingMenu, 
	kVtbl_VATSMenu, kVtbl_ComputersMenu, kVtbl_RepairServicesMenu, kVtbl_TutorialMenu, kVtbl_SpecialBookMenu, 
	kVtbl_ItemModMenu, kVtbl_LoveTesterMenu, kVtbl_CompanionWheelMenu, kVtbl_TraitSelectMenu, kVtbl_RecipeMenu, 
	kVtbl_SlotMachineMenu, kVtbl_BlackjackMenu, kVtbl_RouletteMenu, kVtbl_CaravanMenu, kVtbl_TraitMenu
};

void __fastcall MenuHandleClickHook(Menu *menu, int EDX, int tileID, Tile *clickedTile)
{
	MenuClickEvent &clickEvent = s_menuClickEventMap[kMenuIDJumpTable[menu->id - kMenuType_Min]];
	if (clickedTile && !clickEvent.filtersMap.Empty())
	{
		char lastClickedTilePath[0x200];
		clickedTile->GetComponentFullName(lastClickedTilePath);
		StrToLower(lastClickedTilePath);
		for (auto filter = clickEvent.filtersMap.FindOpDir(lastClickedTilePath, false); filter; --filter)
		{
			if (!StrBeginsCS(lastClickedTilePath, filter.Key())) break;
			for (auto script = filter().BeginCp(); script; ++script)
				CallFunction(*script, NULL, NULL, &s_callRes, 3, menu->id, tileID, clickedTile->name.m_data);
		}
	}
	if (!clickEvent.idsMap.Empty())
	{
		EventCallbackScripts *callbacks = clickEvent.idsMap.GetPtr(tileID);
		if (callbacks)
		{
			const char *tileName = clickedTile ? clickedTile->name.m_data : "";
			for (auto script = callbacks->BeginCp(); script; ++script)
				CallFunction(*script, NULL, NULL, &s_callRes, 3, menu->id, tileID, tileName);
		}
	}
	return clickEvent.funcPtr(menu, tileID, clickedTile);
}

__declspec(naked) void StartCombatHook()
{
	__asm
	{
		mov		eax, [ebp+8]
		test	byte ptr [eax+0x105], kHookActorFlag1_CombatDisabled
		jnz		skipRetn
		mov		[ebp-0xC], ecx
		mov		eax, ecx
		JMP_EDX(0x9001CC)
	skipRetn:
		xor		al, al
		leave
		retn	0x34
	}
}

UnorderedMap<Actor*, Actor*> s_forceCombatTargetMap;

__declspec(naked) void SetCombatTargetHook()
{
	__asm
	{
		mov		eax, [ecx+8]
		test	eax, eax
		jz		skipRetn
		mov		ecx, [ebp+8]
		test	byte ptr [ecx+0x105], kHookActorFlag1_ForceCombatTarget
		jz		contRetn
		push	ecx
		mov		ecx, offset s_forceCombatTargetMap
		call	UnorderedMap<Actor*, Actor*>::Get
		test	eax, eax
		jz		contRetn
		mov		[ebp-0x14], eax
		mov		edx, eax
		mov		ecx, [ebp+8]
		call	Actor::IsInCombatWith
		test	al, al
		jz		contRetn
		mov		eax, [ebp-0x14]
	skipRetn:
		pop		esi
		leave
		retn	4
	contRetn:
		JMP_EAX(0x986C8C)
	}
}

__declspec(naked) void ResetHPCommandHook()
{
	__asm
	{
		mov		ecx, [ebp+0x10]
		mov		[ebp-0xC], ecx
		cmp		byte ptr [ecx+0x18D], 0
		jz		contRetn
		cmp		dword ptr [ebp+0x1C], 0
		jz		contRetn
		cmp		byte ptr ds:[0x11DEA2E], 0
		jnz		contRetn
		push	kExtraData_Script
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		contRetn
		mov		eax, [eax+0x10]
		cmp		[ebp+0x1C], eax
		jnz		contRetn
		mov		eax, [eax+8]
		test	eax, eax
		jz		contRetn
		mov		edx, [ebp+0x10]
	loopHead:
		mov		ecx, [eax]
		test	ecx, ecx
		jz		contRetn
		test	dword ptr [ecx+4], 0x40
		jz		loopNext
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		skipRetn
		cmp		ecx, edx
		jz		skipRetn
	loopNext:
		mov		eax, [eax+4]
		jmp		loopHead
	contRetn:
		JMP_EAX(0x5C6B91)
	skipRetn:
		mov		al, 1
		leave
		retn
	}
}

float s_healthRestoreRatio = 0;

__declspec(naked) void ResetHPWakeUpHook()
{
	__asm
	{
		sub		esp, 0x10
		mov		[ebp-8], ecx
		cmp		byte ptr [ecx+0x18D], 0
		jnz		doRestore
		JMP_EAX(0x8A0969)
	doRestore:
		push	kAVCode_Health
		add		ecx, 0xA4
		mov		eax, [ecx]
		call	dword ptr [eax+4]
		fmul	s_healthRestoreRatio
		push	kAVCode_Health
		mov		ecx, [ebp-8]
		add		ecx, 0xA4
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		fsubp	st(1), st
		fstp	dword ptr [ebp-0x10]
		push	kAVCode_Health
		mov		ecx, [ebp-8]
		CALL_EAX(0x88B740)
		leave
		retn
	}
}

typedef UnorderedMap<Actor*, EventCallbackScripts> ActorEventCallbacks;
ActorEventCallbacks s_fireWeaponEventMap;

__declspec(naked) bool __fastcall RemoveAmmoHook(Actor *actor, int EDX, TESObjectWEAP *weapon)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		test	byte ptr [esi+0x107], kHookActorFlag3_OnFireWeapon
		jz		skipHandler
		push	esi
		mov		ecx, offset s_fireWeaponEventMap
		call	ActorEventCallbacks::GetPtr
		test	eax, eax
		jz		skipHandler
		push	dword ptr [esp+8]
		push	esi
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	skipHandler:
		cmp		esi, g_thePlayer
		jz		retnTrue
		test	byte ptr [esi+0x105], kHookActorFlag1_InfiniteAmmo
		jnz		retnFalse
		mov		edx, [esp+8]
		test	edx, edx
		jz		retnFalse
		test	byte ptr [edx+0x12C], 2
		jnz		retnTrue
		test	byte ptr [edx+0x100], 0x80
		jnz		retnFalse
		cmp		byte ptr [esi+0x18D], 0
		jz		retnFalse
	retnTrue:
		mov		al, 1
		jmp		done
	retnFalse:
		xor		al, al
	done:
		pop		esi
		retn	4
	}
}

__declspec(naked) void ActivateDoorHook()
{
	__asm
	{
		mov		[ebp-0x120], eax
		test	eax, eax
		jnz		contRetn
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		contRetn
		cmp		byte ptr [ecx+0x18D], 0
		jz		contRetn
		push	kExtraData_Lock
		mov		ecx, [ebp-0x12C]
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		contRetn
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		contRetn
		cmp		byte ptr [eax+8], 0
		jz		contRetn
		JMP_EAX(0x573661)
	contRetn:
		JMP_EAX(0x573679)
	}
}

__declspec(naked) void TeleportWithPCHook()
{
	__asm
	{
		cmp		byte ptr [ecx+0x18D], 0
		jz		contRetn2
		mov		al, [ecx+0x105]
		test	al, kHookActorFlag1_PCTeleportFollow
		jnz		contRetn2
		test	al, kHookActorFlag1_PCTeleportWait
		jz		contRetn1
		JMP_EAX(0x8AD38B)
	contRetn1:
		JMP_EAX(0x8AD47D)
	contRetn2:
		JMP_EAX(0x8AD49C)
	}
}

__declspec(naked) void EquipItemHook()
{
	__asm
	{
		movzx	eax, byte ptr [ecx+4]
		mov		ecx, [ebp-0x80]
		test	byte ptr [ecx+0x105], kHookActorFlag1_LockedEquipment
		jz		contRetn
		cmp		dword ptr [ebp+4], 0x9155A2
		jz		contRetn
		cmp		eax, kFormType_TESObjectWEAP
		jnz		contRetn
		JMP_EDX(0x88D27A)
	contRetn:
		JMP_EDX(0x88C87F)
	}
}

__declspec(naked) void ReEquipAllHook()
{
	__asm
	{
		mov		[ebp-0x50], ecx
		mov		ecx, [ebp+8]
		test	byte ptr [ecx+0x105], kHookActorFlag1_LockedEquipment
		jnz		proceed
		mov		byte ptr [ebp-0x11], 1
		JMP_EAX(0x6047CE)
	proceed:
		mov		eax, [ebp]
		cmp		dword ptr [eax+4], 0x5CD73B
		jnz		skipRetn
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		skipRetn
		cmp		dword ptr [ecx+0x28], 1
		ja		getPrefWpn
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		getPrefWpn
		mov		[ebp-0x28], eax
		push	kExtraData_CombatStyle
		mov		ecx, [ebp+8]
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		skipRetn
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		skipRetn
		mov		al, [eax+0x40]
		test	al, al
		jz		skipRetn
		and		al, 1
		mov		ecx, [ebp-0x28]
		mov		ecx, [ecx+8]
		cmp		byte ptr [ecx+0xF4], 2
		setbe	cl
		cmp		al, cl
		jz		skipRetn
	getPrefWpn:
		push	6
		mov		ecx, [ebp+8]
		mov		eax, [ecx]
		call	dword ptr [eax+0x3BC]
		test	eax, eax
		jz		skipRetn
		mov		[ebp-0x28], eax
		mov		ecx, [eax+8]
		mov		edx, 1
		cmp		byte ptr [ecx+0xF4], 9
		jbe		xData
		cmp		dword ptr [eax+4], 1
		jle		xData
		mov		edx, [eax+4]
	xData:
		mov		eax, [eax]
		test	eax, eax
		jz		doEquip
		mov		eax, [eax]
	doEquip:
		push	1
		push	0
		push	1
		push	eax
		push	edx
		push	ecx
		mov		ecx, [ebp+8]
		CALL_EAX(kAddr_EquipItem)
		push	1
		mov		ecx, [ebp-0x28]
		CALL_EAX(0x4459E0)
	skipRetn:
		pop		esi
		leave
		retn	0x14
	}
}

__declspec(naked) void WeaponSwitchSelectHook()
{
	__asm
	{
		mov		dword ptr [ebp-0xB0], 0
		mov		ecx, [ebp-0x28]
		test	byte ptr [ecx+0x105], kHookActorFlag1_LockedEquipment
		jz		done
		mov		eax, [ebp-0x2C]
		test	eax, eax
		jz		done
		mov		eax, [eax+8]
		mov		eax, [eax+0xA8]
		test	eax, eax
		jz		hasAmmo
		push	eax
		call	TESObjectREFR::GetItemCount
		test	eax, eax
		jle		done
	hasAmmo:
		lea     edx, [ebp-0x68]
		mov		ecx, [edx+8]
		test	ecx, ecx
		jz		done
		mov		eax, [ebp-0x2C]
		mov		eax, [eax+8]
		mov		edx, [edx+4]
		ALIGN 16
	loopHead:
		cmp		[edx], eax
		jz		found
		add		edx, 0x14
		dec		ecx
		jnz		loopHead
	done:
		mov		dword ptr [ebp-0xBC], 0
		retn
	found:
		lea     eax, [ebp-0x68]
		mov		edx, [eax+8]
		sub		edx, ecx
		mov		[ebp-0xBC], edx
		inc		edx
		mov		[eax+8], edx
		retn
	}
}

__declspec(naked) void WeaponSwitchUnequipHook()
{
	__asm
	{
		mov		ecx, [ebp-4]
		test	byte ptr [ecx+0x105], kHookActorFlag1_LockedEquipment
		jnz		done
		push	1
		push	0
		push	0
		push	0
		mov		eax, [ebp-0xC]
		push	dword ptr [eax+4]
		push	dword ptr [ebp-8]
		CALL_EAX(kAddr_UnequipItem)
	done:
		retn
	}
}

__declspec(naked) void GetPreferedWeaponHook()
{
	__asm
	{
		mov		[ebp-0x10], ecx
		mov		dword ptr [ebp-8], 0
		cmp		byte ptr [ebp+7], 0
		jnz		contRetn
		test	byte ptr [ecx+0x105], kHookActorFlag1_LockedEquipment
		jz		contRetn
		mov		eax, [ebp-8]
		leave
		retn	4
	contRetn:
		JMP_EAX(0x891C90)
	}
}

__declspec(naked) void ShowMessageHook()
{
	__asm
	{
		mov		ecx, [ebp-0x44]
		test	word ptr [ecx+6], kHookFormFlag6_MessageDisabled
		jz		contRetn
		mov		al, 1
		JMP_EDX(0x5B4930)
	contRetn:
		lea		ecx, [ebp-0x14]
		mov		dword ptr [ecx], 0
		mov		dword ptr [ecx+4], 0
		JMP_EDX(0x5B46B2)
	}
}

__declspec(naked) void ShowTutorialHook()
{
	__asm
	{
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		contRetn
		test	word ptr [ecx+6], kHookFormFlag6_MessageDisabled
		jz		contRetn
		mov		al, 1
		JMP_EDX(0x7E8D48)
	contRetn:
		mov		eax, 0x423
		JMP_EDX(0x7E88BB)
	}
}

struct MenuStateCallbacks
{
	EventCallbackScripts	onOpen;
	EventCallbackScripts	onClose;
	EventCallbackScripts	onMouseover;
}
*s_menuStateEventMap[36] = {NULL};

__declspec(naked) void MenuStateOpenHook()
{
	__asm
	{
		mov		dl, [ebp-0x11]
		mov		ecx, g_menuVisibility
		add		ecx, eax
		cmp		[ecx], dl
		jz		done
		mov		[ecx], dl
		lea		ecx, [eax-kMenuType_Min]
		movzx	eax, kMenuIDJumpTable[ecx]
		cmp		al, 0xFF
		jz		done
		mov		ecx, s_menuStateEventMap[eax*4]
		test	ecx, ecx
		jz		done
		test	dl, dl
		jnz		onOpen
		add		ecx, 0xC
	onOpen:
		cmp		dword ptr [ecx+4], 0
		jz		done
		mov		eax, g_thePlayer
		cmp		dword ptr [eax+0x40], 0
		jz		done
		push	dword ptr [ebp-0x10]
		call	EventCallbackScripts::InvokeEvents
	done:
		xor		eax, eax
		leave
		retn	0xC
	}
}

__declspec(naked) void MenuStateCloseHook()
{
	__asm
	{
		mov		ecx, g_menuVisibility
		add		ecx, edx
		cmp		byte ptr [ecx], 0
		jz		done
		mov		byte ptr [ecx], 0
		movzx	eax, kMenuIDJumpTable[edx-kMenuType_Min]
		cmp		al, 0xFF
		jz		done
		mov		ecx, s_menuStateEventMap[eax*4]
		test	ecx, ecx
		jz		done
		add		ecx, 0xC
		cmp		dword ptr [ecx+4], 0
		jz		done
		mov		eax, g_thePlayer
		cmp		dword ptr [eax+0x40], 0
		jz		done
		push	dword ptr [ebp-0x28]
		call	EventCallbackScripts::InvokeEvents
	done:
		retn
	}
}

__declspec(naked) void MenuHandleMouseoverHook()
{
	static UInt32 lastMenuID = 0x3ED;
	__asm
	{
		mov		eax, [ebp-0xCC]
		mov		ecx, [eax+0xD0]
		test	ecx, ecx
		cmovz	ecx, lastMenuID
		jz		gotID
		mov		ecx, [ecx+0x20]
		mov		lastMenuID, ecx
	gotID:
		movzx	edx, kMenuIDJumpTable[ecx-kMenuType_Min]
		cmp		dl, 0xFF
		jz		skipRetn
		mov		ecx, s_menuStateEventMap[edx*4]
		test	ecx, ecx
		jz		skipRetn
		add		ecx, 0x18
		cmp		dword ptr [ecx+4], 0
		jz		skipRetn
		mov		edx, g_thePlayer
		cmp		dword ptr [edx+0x40], 0
		jz		skipRetn
		push	lastMenuID
		push	1
		push	ecx
		push	EventCallbackScripts::InvokeEvents
		call	MainLoopAddCallbackArgs
		add		esp, 0x10
		mov		eax, [ebp-0xCC]
	skipRetn:
		JMP_EDX(0x70D574)
	}
}

typedef UnorderedMap<Actor*, AnimEventCallbacks> ActorAnimEventCallbacks;
ActorAnimEventCallbacks s_animActionEventMap, s_playGroupEventMap;

__declspec(naked) void SetAnimActionHook()
{
	__asm
	{
		pop		edx
		mov		ecx, [ebp-0x10]
		mov		eax, [ecx+0x68]
		pop		dword ptr [eax+0x2F0]
		mov		edx, [ebp+8]
		cmp		[eax+0x2EC], dx
		jz		done
		mov		[eax+0x2EC], dx
		test	edx, edx
		js		done
		test	byte ptr [ecx+0x107], kHookActorFlag3_OnAnimAction
		jz		done
		push	ecx
		mov		ecx, offset s_animActionEventMap
		call	ActorAnimEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		push	dword ptr [ebp+8]
		mov		ecx, eax
		call	AnimEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		push	dword ptr [ebp+8]
		push	dword ptr [ebp-0x10]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	done:
		leave
		retn	8
	}
}

__declspec(naked) void SetAnimGroupHook()
{
	__asm
	{
		mov		ecx, [ebp+8]
		mov		[eax+edx*4+0xE0], ecx
		mov		cx, [ebp+0xC]
		mov		[eax+edx*2+0x4C], cx
		cmp		cx, 0xFF
		jz		done
		mov		ecx, [eax+4]
		test	byte ptr [ecx+0x107], kHookActorFlag3_OnPlayGroup
		jz		done
		cmp		ecx, g_thePlayer
		jnz		notPlayer
		cmp		[ecx+0x690], eax
		jz		done
	notPlayer:
		push	ecx
		mov		ecx, offset s_playGroupEventMap
		call	ActorAnimEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		push	dword ptr [ebp-0x28]
		mov		ecx, eax
		call	AnimEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		push	dword ptr [ebp-0x28]
		push	dword ptr [ebp-0x18]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	done:
		retn
	}
}

ActorEventCallbacks s_healthDamageEventMap, s_crippledLimbEventMap;

__declspec(naked) void DamageActorValueHook()
{
	__asm
	{
		push	esi
		mov		esi, [ebp+8]
		test	esi, esi
		jz		done
		sub		esi, 0xA4
		mov		edx, [ebp+0xC]
		cmp		dl, kAVCode_Health
		jz		healthDmg
		test	byte ptr [esi+0x107], kHookActorFlag3_OnCrippledLimb
		jz		done
		cmp		dl, kAVCode_PerceptionCondition
		jl		done
		cmp		dl, kAVCode_BrainCondition
		jg		done
		cmp		dword ptr [ebp+0x10], 0
		jz		done
		movss	xmm0, [ebp+0x10]
		addss	xmm0, [ebp+0x14]
		comiss	xmm0, kFltZero
		ja		done
		push	esi
		mov		ecx, offset s_crippledLimbEventMap
		call	ActorEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		mov		edx, [ebp+0xC]
		sub		dl, kAVCode_PerceptionCondition
		push	edx
		push	esi
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
		jmp		done
	healthDmg:
		test	byte ptr [esi+0x107], kHookActorFlag3_OnHealthDamage
		jz		done
		xorps	xmm0, xmm0
		comiss	xmm0, [ebp+0x14]
		jbe		done
		push	esi
		mov		ecx, offset s_healthDamageEventMap
		call	ActorEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		mov		edx, [ebp+0x18]
		test	edx, edx
		jz		noSource
		sub		edx, 0xA4
	noSource:
		push	edx
		push	dword ptr [ebp+0x14]
		push	esi
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis2
	done:
		pop		esi
		push	dword ptr [ebp+0x18]
		push	dword ptr [ebp+0x14]
		push	dword ptr [ebp+0x10]
		push	dword ptr [ebp+0xC]
		push	dword ptr [ebp+8]
		mov		eax, [ebp-4]
		call	dword ptr [eax+0x54]
		leave
		retn
	}
}

const char *kMoonTexturesDefault[] =
{
	"textures\\sky\\masser_full.dds",
	"textures\\sky\\masser_three_wan.dds",
	"textures\\sky\\masser_half_wan.dds",
	"textures\\sky\\masser_one_wan.dds",
	NULL,
	"textures\\sky\\masser_one_wax.dds",
	"textures\\sky\\masser_half_wax.dds",
	"textures\\sky\\masser_three_wax.dds"
};
const char *s_moonTexturesOverride[8] = {NULL};
const char *s_moonTexturesSet[8] = {NULL};

__declspec(naked) void InitMoonHook()
{
	__asm
	{
		push	s_moonTexturesSet[0]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x18
		call	String::Set
		push	s_moonTexturesSet[4]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x20
		call	String::Set
		push	s_moonTexturesSet[8]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x28
		call	String::Set
		push	s_moonTexturesSet[0xC]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x30
		call	String::Set
		push	s_moonTexturesSet[0x10]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x38
		call	String::Set
		push	s_moonTexturesSet[0x14]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x40
		call	String::Set
		push	s_moonTexturesSet[0x18]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x48
		call	String::Set
		push	s_moonTexturesSet[0x1C]
		mov		ecx, [ebp-0x18]
		add		ecx, 0x50
		call	String::Set
		retn
	}
}

float s_weatherTransitionRateOverride = 4;

__declspec(naked) void UpdateWeatherHook()
{
	__asm
	{
		fmul	s_weatherTransitionRateOverride
		retn
	}
}

UnorderedMap<Actor*, UInt32> s_forceDetectionValueMap;

__declspec(naked) void GetDetectionValueHook()
{
	__asm
	{
		mov		[ebp-8], eax
		mov		ecx, [ebp+0xC]
		test	byte ptr [ecx+0x106], kHookActorFlag2_ForceDetectionVal
		jz		done
		push	ecx
		mov		ecx, offset s_forceDetectionValueMap
		call	UnorderedMap<Actor*, UInt32>::Get
		movsx	edx, ax
		test	eax, 0x10000
		jnz		doAdd
		mov		[ebp-8], edx
		jmp		done
	doAdd:
		add		[ebp-8], edx
	done:
		movzx	eax, [ebp+8]
		JMP_EDX(0x8A153C)
	}
}

EventCallbackScripts s_targetChangeEventScripts;

__declspec(naked) void SetPCTargetHook()
{
	static Actor *lastTarget = NULL;
	__asm
	{
		mov		edx, [ebp-0x1C]
		mov     [ebp-0xA8], edx
		mov		ecx, [ebp-0x18]
		mov		ecx, [ecx+0xD44]
		cmp		ecx, lastTarget
		jz		done
		mov		lastTarget, ecx
		push	ecx
		mov		ecx, offset s_targetChangeEventScripts
		call	EventCallbackScripts::InvokeEvents
	done:
		lea		ecx, [ebp-0x160]
		retn
	}
}

bool s_activationDisabledTypes[kFormType_Max] = {0};

__declspec(naked) void PCActivateRefHook()
{
	__asm
	{
		mov		ecx, [ebp-0x34C]
		test	word ptr [ecx+6], kHookFormFlag6_NoPCActivation
		jnz		skipRetn
		mov		eax, [ecx+0x20]
		test	word ptr [eax+6], kHookFormFlag6_NoPCActivation
		jnz		skipRetn
		movzx	edx, byte ptr [eax+4]
		cmp		s_activationDisabledTypes[edx], 0
		jnz		skipRetn
		mov		edx, [ebp-0x378]
		JMP_EAX(0x94327F)
	skipRetn:
		JMP_EAX(0x9432A3)
	}
}

__declspec(naked) void SetRolloverTextHook()
{
	__asm
	{
		test	edx, edx
		jz		done
		test	word ptr [edx+6], kHookFormFlag6_NoPCActivation
		jnz		nullRef
		mov		eax, [edx+0x20]
		test	word ptr [eax+6], kHookFormFlag6_NoPCActivation
		jnz		nullRef
		movzx	eax, byte ptr [eax+4]
		cmp		s_activationDisabledTypes[eax], 0
		jz		done
	nullRef:
		mov		dword ptr [ebp+8], 0
		xor		edx, edx
	done:
		mov		[ebp-0x84], edx
		JMP_EAX(0x775BA1)
	}
}

__declspec(naked) void MergeEventMaskHook()
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x20], 0
		jz		skipRetn
		JMP_EAX(0x5A8E20)
	skipRetn:
		mov		al, 1
		retn	8
	}
}

UnorderedMap<TESForm*, UInt32> s_disabledEventsMap;

__declspec(naked) bool __fastcall IsValidReference(void *object)
{
	__asm
	{
		test	byte ptr [ecx], 4
		jz		retnFalse
		cmp		word ptr [ecx+2], 0x108
		jz		retnTrue
		cmp		dword ptr [ecx], kVtbl_TESObjectREFR
		jnz		retnFalse
	retnTrue:
		mov		al, 1
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

__declspec(naked) void MarkScriptEventHook()
{
	__asm
	{
		push	kExtraData_Script
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x10], 0
		jz		done
		mov		edx, [eax+0xC]
		cmp		dword ptr [edx+0x20], 0
		jz		skipRetn
		mov		edx, [eax+0x10]
		mov		[ebp-8], edx
		mov		edx, [ebp+0x10]
		test	edx, 0x40400F
		jz		notItem
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		contRetn
		test	edx, 0x400000
		jnz		checkBase
		call	IsValidReference
		test	al, al
		jz		notItem
		mov		ecx, [ebp]
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		contRetn
		test	edx, 0xA
		jnz		checkBase
		call	IsValidReference
		test	al, al
		jz		checkBase
		test	edx, 1
		jnz		getBase
		jmp		skipRetn
	notItem:
		mov		ecx, [ebp+0xC]
		sub		ecx, 0x44
		call	IsValidReference
		test	al, al
		jz		contRetn
		test	word ptr [ecx+6], kHookFormFlag6_EventDisabled
		jz		getBase
		push	ecx
		mov		ecx, offset s_disabledEventsMap
		call	UnorderedMap<TESForm*, UInt32>::Get
		test	[ebp+0x10], eax
		jnz		skipRetn
		mov		ecx, [ebp+0xC]
		sub		ecx, 0x44
	getBase:
		mov		ecx, [ecx+0x20]
	checkBase:
		test	word ptr [ecx+6], kHookFormFlag6_EventDisabled
		jz		contRetn
		push	ecx
		mov		ecx, offset s_disabledEventsMap
		call	UnorderedMap<TESForm*, UInt32>::Get
		test	[ebp+0x10], eax
		jnz		skipRetn
	contRetn:
		JMP_EAX(0x5AC777)
	skipRetn:
		mov		[ebp-1], 1
	done:
		JMP_EAX(0x5AC78A)
	}
}

__declspec(naked) void DoActivateHook()
{
	__asm
	{
		mov		ecx, [ebp-0x12C]
		test	word ptr [ecx+6], kHookFormFlag6_ActivateDisabled
		jnz		retnTrue
		mov		edx, [ecx+0x20]
		test	word ptr [edx+6], kHookFormFlag6_ActivateDisabled
		jnz		retnTrue
		push	kExtraData_Action
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		retnTrue
		test	byte ptr [eax+0xC], 1
		setnz	al
		retn
	retnTrue:
		mov		al, 1
		retn
	}
}

SInt32 s_refMapMarkersCount = 0;
tList<TESObjectREFR> s_refMapMarkersList;

__declspec(naked) void *CreateMapMarkersHook()
{
	__asm
	{
		mov		eax, offset s_refMapMarkersList
		retn
	}
}

UnorderedMap<TESObjectREFR*, char*> s_refNamesMap;

__declspec(naked) const char* __fastcall GetRefNameHook(TESObjectREFR *refr)
{
	__asm
	{
		test	word ptr [ecx+6], kHookFormFlag6_AltRefName
		jz		baseName
		push	ecx
		push	ecx
		mov		ecx, offset s_refNamesMap
		call	UnorderedMap<TESObjectREFR*, char*>::Get
		pop		ecx
		test	eax, eax
		jz		baseName
		retn
	baseName:
		push	dword ptr [ecx+0x20]
		CALL_EAX(0x482720)
		pop		ecx
		retn
	}
}

UnorderedMap<TESQuest*, QuestStageCallbacks> s_questStageEventMap;

__declspec(naked) bool __fastcall HandleSetQuestStage(TESQuest *quest, UInt8 stageID)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	edx
		push	ebx
		xor		ebx, ebx
		push	ecx
		mov		ecx, offset s_questStageEventMap
		call	UnorderedMap<TESQuest*, QuestStageCallbacks>::GetPtr
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [eax]
		mov		edi, [eax+4]
		ALIGN 16
	iterHead:
		test	edi, edi
		jz		iterEnd
		mov		edx, [ebp-8]
		cmp		byte ptr [esi], dl
		jnz		iterNext
		push	edx
		push	dword ptr [ebp-4]
		push	2
		push	offset s_callRes
		push	0
		push	0
		push	dword ptr [esi+4]
		call	CallFunction
		add		esp, 0x1C
		or		bl, byte ptr [esi+1]
		ALIGN 16
	iterNext:
		add		esi, 8
		dec		edi
		jmp		iterHead
	iterEnd:
		pop		edi
		pop		esi
	done:
		cmp		bl, 2
		setnz	al
		pop		ebx
		leave
		retn
	}
}

__declspec(naked) void SetQuestStageHook()
{
	__asm
	{
		mov		eax, [ebp+8]
		test	byte ptr [ecx+1], 1
		jnz		checkRepeated
		mov		byte ptr [ecx+1], 1
		jmp		checkOverride
	checkRepeated:
		test	byte ptr [eax+0x3C], 8
		jz		retn0
	checkOverride:
		test	word ptr [eax+6], kHookFormFlag6_SetStageHandlers
		jz		retn1
		movzx	edx, [ecx]
		mov		ecx, eax
		call	HandleSetQuestStage
		retn
	retn0:
		xor		al, al
		retn
	retn1:
		mov		al, 1
		retn
	}
}

UnorderedMap<TESForm*, EventCallbackScripts> s_dialogTopicEventMap;

__declspec(naked) void RunResultScriptHook()
{
	__asm
	{
		cmp		dword ptr [ebp+8], 0
		jnz		done
		push	dword ptr [ebp-0xC]
		mov		ecx, offset s_dialogTopicEventMap
		call	UnorderedMap<TESForm*, EventCallbackScripts>::GetPtr
		test	eax, eax
		jz		doneInfo
		push	dword ptr [ebp-0xC]
		push	dword ptr [ebp+0xC]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	doneInfo:
		mov		ecx, [ebp-0xC]
		mov		eax, [ecx+0x50]
		push	eax
		push	eax
		mov		ecx, offset s_dialogTopicEventMap
		call	UnorderedMap<TESForm*, EventCallbackScripts>::GetPtr
		pop		ecx
		test	eax, eax
		jz		done
		push	ecx
		push	dword ptr [ebp+0xC]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	done:
		push	dword ptr [ebp+8]
		mov		ecx, [ebp-0xC]
		CALL_EAX(0x61EB60)
		retn
	}
}

void __fastcall InvokeActorHitEvents(ActorHitData *hitData)
{
	CriticalHitEventData *eventData;
	for (auto iter = s_criticalHitEvents.BeginCp(); iter; ++iter)
	{
		eventData = &iter();
		if (eventData->target && (eventData->target != hitData->target))
			continue;
		if (eventData->source && (eventData->source != hitData->source))
			continue;
		if (eventData->weapon && (eventData->weapon != hitData->weapon))
			continue;
		CallFunction(eventData->callback, hitData->target, NULL, &s_callRes, 2, hitData->source, hitData->weapon);
	}
}

__declspec(naked) void __fastcall CopyHitDataHook(MiddleHighProcess *process, int EDX, ActorHitData *copyFrom)
{
	__asm
	{
		mov		eax, [ecx+0x240]
		test	eax, eax
		jnz		allocated
		push	ecx
		push	0x64
		GAME_HEAP_ALLOC
		pop		ecx
		mov		[ecx+0x240], eax
	allocated:
		mov		edx, [esp+4]
		mov		ecx, [edx+0x10]
		mov		[eax+0x60], ecx
		push	0x60
		push	edx
		push	eax
		call	MemCopy
		add		esp, 0xC
		cmp		dword ptr ds:[s_criticalHitEvents.numItems], 0
		jz		done
		test	byte ptr [eax+0x58], 4
		jz		done
		mov		ecx, eax
		call	InvokeActorHitEvents
	done:
		retn	4
	}
}

__declspec(naked) void __fastcall ResetHitDataHook(MiddleHighProcess *process)
{
	__asm
	{
		mov		eax, [ecx+0x240]
		test	eax, eax
		jz		done
		mov		dword ptr [eax+0x10], 0xFFFFFFFF
	done:
		retn
	}
}

struct ScriptWaitInfo
{
	TESForm		*owner;				// 00
	UInt32		iterNum;			// 04
	UInt32		blockOffset;		// 08
	UInt32		savedOffset;		// 0C
	UInt32		ifStackDepth;		// 10
	UInt32		ifStackFlags[10];	// 14

	ScriptWaitInfo() {}

	void Init(TESForm *_owner, UInt32 _iterNum, UInt32 _blockOffset, UInt32 *opcodeOffsetPtr)
	{
		owner = _owner;
		iterNum = _iterNum;
		blockOffset = _blockOffset;
		savedOffset = *opcodeOffsetPtr - blockOffset;
		ScriptRunner *scrRunner = *(ScriptRunner**)(opcodeOffsetPtr - 0x3B0);
		memcpy(&ifStackDepth, &scrRunner->ifStackDepth, 0x2C);
	}
}
*s_scriptWaitInfo = NULL;

typedef UnorderedMap<UInt32, ScriptWaitInfo> ScriptWaitInfoMap;
ScriptWaitInfoMap s_scriptWaitInfoMap;

__declspec(naked) void *ScriptRunnerHook()
{
	__asm
	{
		mov		s_scriptWaitInfo, 0
		mov		dx, [ecx+0x28]
		cmp		dx, 1
		ja		done
		test	dl, dl
		jz		objScript
		mov		eax, [ecx+0x40]
		jmp		checkFlag
	objScript:
		mov		eax, [ebp+0xC]
	checkFlag:
		test	eax, eax
		jz		done
		test	byte ptr [eax+5], kHookFormFlag5_ScriptOnWait
		jz		done
		push	dword ptr [eax+0xC]
		mov		ecx, offset s_scriptWaitInfoMap
		call	ScriptWaitInfoMap::GetPtr
		mov		s_scriptWaitInfo, eax
	done:
		mov		eax, [ebp+8]
		add		eax, 0x18
		retn
	}
}

__declspec(naked) void EvalEventBlockHook()
{
	__asm
	{
		pxor	xmm0, xmm0
		comisd	xmm0, qword ptr [ebp-0x2C]
		setnz	al
		jz		retnFalse
		cmp		byte ptr [ebp+0x28], 0
		setz	al
	retnFalse:
		mov		ecx, s_scriptWaitInfo
		test	ecx, ecx
		jz		notWaiting
		mov		edx, [ecx+8]
		cmp		[ebp-0x10], edx
		jnz		notWaiting
		cmp		dword ptr [ebp-0x18], 1
		ja		evntBlock
		test	al, al
		jz		skipBlock
	evntBlock:
		dec		dword ptr [ecx+4]
		jnz		skipBlock
		mov		eax, [ecx+0xC]
		mov		edx, [ebp+0x14]
		add		[edx], eax
		push	0x2C
		lea		edx, [ecx+0x10]
		push	edx
		mov		ecx, [ebp-0xED0]
		add		ecx, 0x20
		push	ecx
		call	MemCopy
		add		esp, 0xC
		mov		eax, s_scriptWaitInfo
		mov		s_scriptWaitInfo, 0
		mov		eax, [eax]
		and		byte ptr [eax+5], ~kHookFormFlag5_ScriptOnWait
		push	dword ptr [eax+0xC]
		mov		ecx, offset s_scriptWaitInfoMap
		call	ScriptWaitInfoMap::Erase
		push	0
		mov		ecx, offset s_hookInfos+kHook_ScriptRunner*0x10
		call	HookInfo::ModUsers
		push	0
		mov		ecx, offset s_hookInfos+kHook_EvalEventBlock*0x10
		call	HookInfo::ModUsers
		retn
	notWaiting:
		test	al, al
		jz		skipBlock
		retn
	skipBlock:
		mov		eax, [ebp+0x14]
		mov		edx, [ebp-0x20]
		add		[eax], edx
		retn
	}
}

UnorderedMap<BGSTerminal*, char*> s_terminalAltModelsMap;

__declspec(naked) TESObjectREFR* __fastcall GetHackingMenuRef(HackingMenu *menu)
{
	__asm
	{
		cmp		byte ptr [ecx+0x17C], 0
		jz		getCrosshair
		mov		eax, [ecx+0x198]
		retn
	getCrosshair:
		mov		eax, g_interfaceManager
		mov		eax, [eax+0xFC]
		retn
	}
}

__declspec(naked) void SetTerminalModelHook()
{
	__asm
	{
		CALL_EAX(kAddr_PurgeTerminalModel)
		mov		eax, [eax]
		test	eax, eax
		jnz		done
		mov		eax, ds:[0x11D9334]
		test	eax, eax
		jz		getHacking
		mov		eax, [eax+0xAC]
		jmp		checkTarget
	getHacking:
		mov		eax, ds:[0x11D95B8]
		test	eax, eax
		jz		done
		mov		ecx, eax
		call	GetHackingMenuRef
	checkTarget:
		test	eax, eax
		jz		done
		push	dword ptr [eax+0x20]
		mov		ecx, offset s_terminalAltModelsMap
		call	UnorderedMap<BGSTerminal*, char*>::Get
		test	eax, eax
		cmovz	eax, g_terminalModelDefault
		mov		ds:[0x11A0BB0], eax
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) void AddVATSTargetHook()
{
	__asm
	{
		CALL_EAX(0x88B880)
		test	al, al
		jz		done
		mov		ecx, [ebp-0x28]
		mov		ecx, [ecx]
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		xor		al, 1
		jnz		done
		test	byte ptr [ecx+0x106], kHookActorFlag2_NonTargetable
		setz	al
	done:
		JMP_EDX(0x7F54B6)
	}
}

EventCallbackScripts s_locationDiscoverEventScripts;

__declspec(naked) void LocationDiscoverHook()
{
	__asm
	{
		push	1
		push	1
		CALL_EAX(0x4D5E10)
		add		esp, 8
		mov		eax, [ebp-0x8C]
		push	dword ptr [eax+4]
		mov		ecx, offset s_locationDiscoverEventScripts
		call	EventCallbackScripts::InvokeEvents
		retn
	}
}

EventCallbackScripts s_itemCraftedEventScripts;

__declspec(naked) void ItemCraftedHook()
{
	__asm
	{
		mov		eax, [ecx+4]
		mov		[ebp-0x1C], eax
		mov		eax, [ebp-0x14]
		imul	eax, [ebp+8]
		push	eax
		push	dword ptr [ebp-0x20]
		mov		ecx, offset s_itemCraftedEventScripts
		call	EventCallbackScripts::InvokeEvents2
		retn
	}
}

typedef UnorderedSet<UInt32> CellCoordsSet;
UnorderedMap<UInt32, CellCoordsSet> s_swapObjLODMap;
const char kObjLODPathFormat[] = "Data\\Meshes\\Landscape\\LOD\\%s\\Blocks\\%s.Level%d.X%d.Y%d.AltLOD.nif";

__declspec(naked) void MakeObjLODPathHook()
{
	__asm
	{
		cmp		byte ptr [ebp+0x20], 0
		jnz		contRetn
		mov		eax, [ebp-0x120]
		push	dword ptr [eax+0x44]
		mov		ecx, offset s_swapObjLODMap
		call	UnorderedMap<UInt32, CellCoordsSet>::GetPtr
		test	eax, eax
		jz		contRetn
		mov		ecx, eax
		movzx	eax, word ptr [ebp+0x10]
		shl		eax, 0x10
		movzx	edx, word ptr [ebp+0x14]
		or		eax, edx
		push	eax
		call	CellCoordsSet::HasKey
		test	al, al
		jz		contRetn
		mov		eax, [ebp-0x120]
		mov		ecx, [eax+0x44]
		mov		eax, [ecx]
		call	dword ptr [eax+0x130]
		push	dword ptr [ebp+0x14]
		push	dword ptr [ebp+0x10]
		push	dword ptr [ebp+0x18]
		push	eax
		push	eax
		push	offset kObjLODPathFormat
		push	0x104
		lea		eax, [ebp-0x11C]
		push	eax
		CALL_EAX(0x406D00)
		add		esp, 0x20
		JMP_EAX(0x6F6EC0)
	contRetn:
		xor		edx, edx
		mov		dl, [ebp+0x1C]
		mov		eax, g_thePlayer
		or		dl, byte ptr [eax+0xDF2]
		JMP_EAX(0x6F6E07)
	}
}

EventCallbackScripts s_onHitEventScripts;
ActorEventCallbacks s_onHitEventMap;

__declspec(naked) void OnHitEventHook()
{
	__asm
	{
		test	dword ptr [ebp+0x10], 0x80
		jz		done
		/*cmp		dword ptr [ebp+8], 0
		jz		done*/
		mov		eax, [ebp+0xC]
		test	eax, eax
		jz		done
		sub		eax, 0x44
		test	byte ptr [eax], 8
		jz		done
		cmp		word ptr [eax+2], 0x108
		jnz		done
		mov		[ebp-8], eax
		cmp		dword ptr ds:[s_onHitEventScripts+4], 0
		jz		doFiltered
		push	eax
		mov		ecx, offset s_onHitEventScripts
		call	EventCallbackScripts::InvokeEventsThis
		mov		eax, [ebp-8]
	doFiltered:
		test	byte ptr [eax+0x107], kHookActorFlag3_OnHit
		jz		done
		push	eax
		mov		ecx, offset s_onHitEventMap
		call	ActorEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		push	dword ptr [ebp-8]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis
	done:
		mov		al, [ebp-1]
		leave
		retn
	}
}

__declspec(naked) void CheckUniqueItemHook()
{
	__asm
	{
		movzx	eax, byte ptr [ecx+4]
		cmp		eax, esi
		jnz		done
		xor		eax, eax
		test	word ptr [ecx+6], kHookFormFlag6_UniqueItem
		jnz		done
		mov		eax, esi
	done:
		retn
	}
}

typedef UnorderedMap<BGSProjectile*, EventCallbackScripts> ProjectileEventCallbacks;
ProjectileEventCallbacks s_projectileImpactEventMap;

__declspec(naked) void ProjectileImpactHook()
{
	__asm
	{
		mov		eax, [ebp-0x58]
		mov     byte ptr [eax+0x90], 1
		mov		eax, [eax+0x20]
		test	word ptr [eax+6], kHookFormFlag6_ProjectileImpact
		jz		done
		push	eax
		mov		ecx, offset s_projectileImpactEventMap
		call	ProjectileEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [ebp-0x58]
		mov		edx, [eax+0x20]
		cmp		word ptr [edx+0x62], 2
		jz		keepPos
		mov		edx, [eax+0x88]
		test	edx, edx
		jz		keepPos
		movq	xmm0, qword ptr [edx+4]
		movq	qword ptr [eax+0x30], xmm0
		mov		edx, [edx+0xC]
		mov		[eax+0x38], edx
	keepPos:
		push	eax
		call	EventCallbackScripts::InvokeEventsThis
	done:
		retn
	}
}

EventCallbackScripts s_noteAddedEventScripts;

__declspec(naked) void AddNoteHook()
{
	__asm
	{
		push	dword ptr [ebp+8]
		mov		ecx, offset s_noteAddedEventScripts
		call	EventCallbackScripts::InvokeEvents
		mov		eax, [ebp+8]
		mov		eax, [eax+0x4C]
		mov		edx, 0x1011584
		test	eax, eax
		cmovz	eax, edx
		retn
	}
}

typedef UnorderedMap<AlchemyItem*, EventCallbackScripts> EquipAidEventCallbacks;
EquipAidEventCallbacks s_useAidItemEventMap;

__declspec(naked) void EquipAidItemHook()
{
	__asm
	{
		CALL_EAX(0x88C830)
		mov		eax, [ebp+8]
		test	word ptr [eax+6], kHookFormFlag6_OnEquipHandlers
		jz		done
		push	eax
		mov		ecx, offset s_useAidItemEventMap
		call	EquipAidEventCallbacks::GetPtr
		test	eax, eax
		jz		done
		push	dword ptr [ebp+8]
		push	dword ptr [ebp-4]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	done:
		leave
		retn	0x18
	}
}

EventCallbackScripts s_reloadWeaponEventScripts;

__declspec(naked) void ReloadWeaponHook()
{
	__asm
	{
		push	dword ptr [ebp+0xC]
		mov		edx, [ebp+8]
		push	edx
		mov		eax, [ecx+0x68]
		cmp		[eax+0x2EC], dx
		jz		done
		cmp		dl, kAnimAction_Reload
		jz		proceed
		cmp		dl, kAnimAction_ReloadLoop
		jnz		done
	proceed:
		mov		eax, [eax+0x114]
		test	eax, eax
		jz		noWeapon
		mov		eax, [eax+8]
	noWeapon:
		push	eax
		push	ecx
		mov		ecx, offset s_reloadWeaponEventScripts
		call	EventCallbackScripts::InvokeEventsThis1
	done:
		JMP_EAX(0x8A7551)
	}
}

EventCallbackScripts s_onRagdollEventScripts;

__declspec(naked) void OnRagdollHook()
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		eax, [esi+0x58]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x18]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax], kVtbl_BSFadeNode
		jnz		done
		mov		eax, [eax+0xCC]
		test	eax, eax
		jz		done
		push	eax
		mov		ecx, offset s_onRagdollEventScripts
		call	EventCallbackScripts::InvokeEventsThis
	done:
		mov		ecx, esi
		xor		ebx, ebx
		JMP_EAX(0xC7C156)
	}
}

__declspec(naked) void PlayerMinHealthHook()
{
	__asm
	{
		push	kAVCode_Health
		mov		ecx, [ebp-0x38]
		add		ecx, 0xA4
		mov		[ebp-0x10], ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		fst		dword ptr [ebp-0x40]
		push	kAVCode_Health
		mov		ecx, [ebp-0x10]
		mov		eax, [ecx]
		call	dword ptr [eax+0x20]
		fdivp	st(1), st
		cmp		dword ptr [ebp+0xC], 0
		jge		done
		movss	xmm0, [ebp-0x40]
		movss	xmm1, xmm0
		addss	xmm1, [ebp+0xC]
		movss	xmm2, kFltOne
		comiss	xmm1, xmm2
		jnb		done
		subss	xmm2, xmm0
		movss	[ebp+0xC], xmm2
	done:
		retn
	}
}

__declspec(naked) void ApplyActorVelocityHook()
{
	__asm
	{
		mov		eax, [ebp+8]
		fld		dword ptr [eax]
		test	dword ptr [ecx+0x414], 0x80000000
		jz		contRetn
		xorps	xmm0, xmm0
		comiss	xmm0, [ecx+0x524]
		jnb		rmvFlag
		JMP_EDX(0xC6D5A5)
	rmvFlag:
		and		dword ptr [ecx+0x414], 0x7FFFFFFF
	contRetn:
		JMP_EDX(0xC6D4E9)
	}
}

struct DataStrings : Set<char*>
{
	DataStrings(UInt32 _alloc = 4) : Set<char*>(_alloc) {}
};
typedef Map<char*, DataStrings> NodeNamesMap;
UnorderedMap<TESForm*, NodeNamesMap> s_insertNodeMap, s_attachModelMap;

__declspec(naked) void __fastcall DoInsertNodes(TESForm *form, int EDX, NiNode *rootNode)
{
	__asm
	{
		push	ecx
		mov		ecx, offset s_insertNodeMap
		call	UnorderedMap<TESForm*, NodeNamesMap>::GetPtr
		test	eax, eax
		jz		done
		push	ebp
		mov		ebp, esp
		sub		esp, 8
		push	ebx
		push	esi
		push	edi
		mov		esi, [eax]
		mov		eax, [eax+4]
		mov		[ebp-4], eax
		ALIGN 16
	nodeHead:
		mov		ebx, [ebp+8]
		mov		edx, [esi]
		cmp		[edx], 0
		jz		useRoot
		mov		ecx, ebx
		call	NiNode::GetNode
		test	eax, eax
		jz		nodeNext
		mov		ebx, eax
	useRoot:
		mov		eax, [esi+4]
		mov		edi, [eax]
		mov		edx, [eax+4]
		mov		[ebp-8], edx
		ALIGN 16
	insHead:
		mov		ecx, [ebp+8]
		mov		edx, [edi]
		cmp		[edx], '^'
		jnz		notParent
		cmp		ecx, ebx
		jz		insNext
		inc		edx
		push	edx
		call	NiNode::GetBlock
		test	eax, eax
		jz		doCreate
		pop		ecx
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		test	eax, eax
		jz		insNext
		mov		ecx, eax
		mov		eax, ebx
		jmp		doAdd
	doCreate:
		call	NiNode::Create
		mov		edx, [ebx+0x30]
		mov		[eax+0x30], edx
		push	eax
		push	eax
		mov		ecx, ebx
		call	NiAVObject::GetIndex
		push	eax
		mov		ecx, [ebx+0x18]
		mov		eax, [ecx]
		call	dword ptr [eax+0xE0]
		pop		ecx
		mov		eax, ebx
		jmp		doAdd
	notParent:
		call	NiNode::GetBlock
		test	eax, eax
		jnz		insNext
		push	dword ptr [edi]
		call	NiNode::Create
		mov		ecx, ebx
	doAdd:
		push	1
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
	insNext:
		add		edi, 4
		dec		dword ptr [ebp-8]
		jnz		insHead
	nodeNext:
		add		esi, 8
		dec		dword ptr [ebp-4]
		jnz		nodeHead
		pop		edi
		pop		esi
		pop		ebx
		leave
	done:
		retn	4
	}
}

__declspec(naked) void __fastcall DoAttachModels(TESForm *form, int EDX, NiNode *rootNode)
{
	__asm
	{
		push	ecx
		mov		ecx, offset s_attachModelMap
		call	UnorderedMap<TESForm*, NodeNamesMap>::GetPtr
		test	eax, eax
		jz		done
		push	ebp
		mov		ebp, esp
		sub		esp, 8
		push	ebx
		push	esi
		push	edi
		mov		esi, [eax]
		mov		eax, [eax+4]
		mov		[ebp-4], eax
		ALIGN 16
	nodeHead:
		mov		ebx, [ebp+8]
		mov		edx, [esi]
		cmp		[edx], 0
		jz		useRoot
		mov		ecx, ebx
		call	NiNode::GetNode
		test	eax, eax
		jz		nodeNext
		mov		ebx, eax
	useRoot:
		mov		eax, [esi+4]
		mov		edi, [eax]
		mov		edx, [eax+4]
		mov		[ebp-8], edx
		ALIGN 16
	insHead:
		push	1
		push	0
		push	0
		push	1
		push	0
		push	dword ptr [edi]
		mov		ecx, g_modelLoader
		CALL_EAX(kAddr_LoadModel)
		test	eax, eax
		jz		insNext
		mov		ecx, eax
		call	NiNode::CreateCopy
		test	eax, eax
		jz		insNext
		push	1
		push	eax
		mov		ecx, eax
		call	NiNode::RemoveCollision
		mov		ecx, ebx
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
	insNext:
		add		edi, 4
		dec		dword ptr [ebp-8]
		jnz		insHead
	nodeNext:
		add		esi, 8
		dec		dword ptr [ebp-4]
		jnz		nodeHead
		pop		edi
		pop		esi
		pop		ebx
		leave
	done:
		retn	4
	}
}

__declspec(naked) void InsertObjectHook()
{
	__asm
	{
		mov		[ebp-0x1C], ecx
		mov		ecx, [ecx+0x40]
		add		ecx, 0x80
		CALL_EAX(0x40FBA0)
		mov		ecx, [ebp-0x1C]
		call	TESObjectREFR::GetBaseForm
		test	eax, eax
		jz		done
		mov		ecx, [ebp-0x1C]
		test	word ptr [ecx+6], kHookFormFlag6_InsertObject
		jnz		hasFlag
		test	word ptr [eax+6], kHookFormFlag6_InsertObject
		jz		done
	hasFlag:
		mov		[ebp-0x10], eax
		mov		edx, [ebp-0x18]
		mov		eax, [edx+0x34]
		test	eax, eax
		jnz		gotNode
		mov		eax, [ecx]
		call	dword ptr [eax+0x1D0]
		test	eax, eax
		jz		done
		mov		edx, [ebp-0x18]
	gotNode:
		mov		[ebp-0x18], eax
		mov		ecx, [edx+0x30]
		test	ecx, ecx
		jz		doneProt
		cmp		[ecx+0xC], eax
		jnz		doneProt
		push	ecx
		mov		ecx, eax
		call	NiNode::CreateCopy
		pop		ecx
		test	eax, eax
		jz		done
		push	eax
		add		ecx, 0xC
		push	ecx
		call	NiReleaseAddRef
	doneProt:
		cmp		dword ptr ds:[s_insertNodeMap.numEntries], 0
		jz		doModels
		mov		ecx, [ebp-0x1C]
		test	word ptr [ecx+6], kHookFormFlag6_InsertNode
		jz		checkBase1
		push	dword ptr [ebp-0x18]
		call	DoInsertNodes
	checkBase1:
		mov		ecx, [ebp-0x10]
		test	word ptr [ecx+6], kHookFormFlag6_InsertNode
		jz		doModels
		push	dword ptr [ebp-0x18]
		call	DoInsertNodes
	doModels:
		cmp		dword ptr ds:[s_attachModelMap.numEntries], 0
		jz		done
		mov		ecx, [ebp-0x1C]
		test	word ptr [ecx+6], kHookFormFlag6_AttachModel
		jz		checkBase2
		push	dword ptr [ebp-0x18]
		call	DoAttachModels
	checkBase2:
		mov		ecx, [ebp-0x10]
		test	word ptr [ecx+6], kHookFormFlag6_AttachModel
		jz		done
		push	dword ptr [ebp-0x18]
		call	DoAttachModels
	done:
		push	dword ptr [ebp-0x14]
		CALL_EAX(0x404F30)
		pop		ecx
		mov		ecx, [ebp-0xC]
		mov		fs:0, ecx
		pop		ecx
		leave
		retn
	}
}

TESObjectREFR *s_syncPositionRef = NULL;
char s_syncPositionNode[0x80];
AlignedVector4 s_syncPositionMods(0, 0, 0, 0), s_syncPositionPos;
UInt8 s_syncPositionFlags = 0;

__declspec(naked) void SynchronizePositionHook()
{
	__asm
	{
		mov		[ebp-0x44], ecx
		mov		[ebp-0x1D], 0
		mov		eax, ds:[0x11F426C]
		mov		[ebp-0x10], eax
		cmp		ecx, g_thePlayer
		jnz		done
		mov		ecx, [ecx+0x40]
		test	ecx, ecx
		jz		done
		mov		eax, s_syncPositionRef
		mov		edx, [eax+0x40]
		test	edx, edx
		jz		done
		cmp		ecx, edx
		jz		sameCell
		mov		ecx, [ecx+0xC0]
		test	ecx, ecx
		jz		done
		cmp		[edx+0xC0], ecx
		jnz		done
	sameCell:
		mov		edx, offset s_syncPositionNode
		mov		ecx, eax
		call	TESObjectREFR::GetNode
		test	eax, eax
		jz		done
		movups	xmm0, [eax+0x8C]
		addps	xmm0, s_syncPositionMods
		mov		ecx, offset s_syncPositionPos
		movaps	[ecx], xmm0
		mov		[ebp+8], ecx
		cmp		s_syncPositionFlags, 0
		jz		done
		push	dword ptr [eax+0x68]
		push	dword ptr [eax+0x6C]
		CALL_EAX(0xA811F0)
		add		esp, 8
		mov		ecx, g_thePlayer
		fstp	dword ptr [ecx+0x2C]
	done:
		retn
	}
}

bool s_lastLMBState = false;

__declspec(naked) void ProcessHUDMainUI()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x14
		push	ebx
		push	esi
		push	edi
		mov		ebx, g_interfaceManager
		mov		eax, g_inputGlobals
		and		dword ptr [eax+4], 0xFFFFFFF7
		mov		ecx, ebx
		CALL_EAX(0x7118D0)
		mov		ecx, g_DIHookCtrl
		mov		al, [ecx+0x704]
		mov		[ebp-4], al
		mov		esi, [ebx+0x4C]
		test	esi, esi
		jz		pickTile
		test	al, al
		jz		notPressed
		CALL_EAX(0x70ECB0)
		fld		st
		fdivr	dword ptr [ebx+0x38]
		fstp	dword ptr [ebp-8]
		fdivr	dword ptr [ebx+0x40]
		fstp	dword ptr [ebp-0xC]
		push	kTileValue_dragstartx
		mov		ecx, esi
		CALL_EAX(0xA011B0)
		fsubr	dword ptr [ebp-8]
		fstp	dword ptr [ebp-0x14]
		cvttss2si	edx, dword ptr [ebp-0x14]
		mov		[ebx+0x60], edx
		push	kTileValue_dragstarty
		mov		ecx, esi
		CALL_EAX(0xA011B0)
		fsubr	dword ptr [ebp-0xC]
		fstp	dword ptr [ebp-0x14]
		cvttss2si	edx, dword ptr [ebp-0x14]
		mov		[ebx+0x64], edx
		mov		ecx, esi
		CALL_EAX(0xA013D0)
		fstp	dword ptr [ebp-0x14]
		push	kTileValue_x
		mov		ecx, esi
		CALL_EAX(0xA011B0)
		fsubr	dword ptr [ebp-0x14]
		fsubr	dword ptr [ebp-8]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragx
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		mov		ecx, esi
		CALL_EAX(0xA01440)
		fstp	dword ptr [ebp-0x14]
		push	kTileValue_y
		mov		ecx, esi
		CALL_EAX(0xA011B0)
		fsubr	dword ptr [ebp-0x14]
		fsubr	dword ptr [ebp-0xC]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragy
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		jmp		done
	notPressed:
		xor		edx, edx
		mov		[ebx+0x4C], edx
		mov		[ebx+0xD4], edx
		mov		[ebx+0xD8], edx
		push	1
		push	0xBF800000
		push	kTileValue_dragstartx
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		push	1
		push	0xBF800000
		push	kTileValue_dragstarty
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		mov		s_lastLMBState, 0
	pickTile:
		push	0
		mov		ecx, ebx
		CALL_EAX(0x7126C0)
		mov		esi, eax
		mov		edi, [ebx+0xCC]
		cmp		esi, edi
		jnz		handleMouseover
		test	esi, esi
		jz		done
		mov		edi, [ebx+0xD0]
		test	edi, edi
		jz		done
		mov		al, [ebp-4]
		cmp		s_lastLMBState, al
		jz		handleDrag
		mov		s_lastLMBState, al
		test	al, al
		jnz		done
		push	kTileValue_clicksound
		mov		ecx, esi
		CALL_EAX(0xA0B110)
		push	1
		push	0x3F800000
		push	kTileValue_clicked
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		push	1
		push	0
		push	kTileValue_clicked
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		push	kTileValue_id
		mov		ecx, esi
		call	Tile::GetValue
		mov		edx, 0xFFFFFFFF
		test	eax, eax
		jz		gotTileID
		cvttss2si	edx, dword ptr [eax+8]
	gotTileID:
		push	esi
		push	edx
		mov		ecx, edi
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		jmp		done
	handleDrag:
		test	al, al
		jz		handleWheel
		push	kTileValue_draggable
		mov		ecx, esi
		CALL_EAX(0xA01230)
		test	al, al
		jz		done
		mov		[ebx+0xD4], esi
		mov		[ebx+0xD8], edi
		mov		[ebx+0x4C], esi
		CALL_EAX(0x70ECB0)
		fld		st
		fdivr	dword ptr [ebx+0x38]
		fstp	dword ptr [ebp-8]
		fdivr	dword ptr [ebx+0x40]
		fstp	dword ptr [ebp-0xC]
		cvttss2si	edx, dword ptr [ebp-8]
		mov		[ebx+0x50], edx
		cvttss2si	edx, dword ptr [ebp-0xC]
		mov		[ebx+0x5C], edx
		mov		ecx, esi
		CALL_EAX(0xA013D0)
		fstp	dword ptr [ebp-0x10]
		mov		ecx, esi
		CALL_EAX(0xA01440)
		fstp	dword ptr [ebp-0x14]
		push	kTileValue_x
		mov		ecx, esi
		CALL_EAX(0xA011B0)
		fsubr	dword ptr [ebp-0x10]
		fsubr	dword ptr [ebp-8]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragx
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		push	kTileValue_y
		mov		ecx, esi
		CALL_EAX(0xA011B0)
		fsubr	dword ptr [ebp-0x14]
		fsubr	dword ptr [ebp-0xC]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragy
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		fild	dword ptr [ebx+0x50]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragstartx
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		fild	dword ptr [ebx+0x5C]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragstarty
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		fld		dword ptr [ebp-8]
		fsub	dword ptr [ebp-0x10]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragoffsetx
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		fld		dword ptr [ebp-0xC]
		fsub	dword ptr [ebp-0x14]
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_dragoffsety
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		xor		edx, edx
		mov		[ebx+0x54], edx
		mov		[ebx+0x58], edx
		mov		[ebx+0x60], edx
		mov		[ebx+0x64], edx
		jmp		done
	handleWheel:
		mov		eax, g_inputGlobals
		mov		eax, [eax+0x1B2C]
		test	eax, eax
		jz		done
		mov		ecx, 0xFFFFFF88
		xor		edx, edx
		idiv	ecx
		test	eax, eax
		jz		done
		mov		[ebp-8], eax
		ALIGN 16
	iterHead:
		push	kTileValue_wheelable
		mov		ecx, esi
		CALL_EAX(0xA01230)
		test	al, al
		jnz		iterEnd
		mov		esi, [esi+0x28]
		test	esi, esi
		jnz		iterHead
		jmp		done
		ALIGN 16
	iterEnd:
		push	1
		fild	dword ptr [ebp-8]
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_wheelmoved
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		push	1
		push	0
		push	kTileValue_wheelmoved
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		jmp		done
	handleMouseover:
		test	edi, edi
		jz		noLast
		push	1
		push	0
		push	kTileValue_mouseover
		mov		ecx, edi
		CALL_EAX(kAddr_TileSetFloat)
	noLast:
		mov		[ebx+0xCC], esi
		test	esi, esi
		jz		noCurr
		push	kTileValue_mouseoversound
		mov		ecx, esi
		CALL_EAX(0xA0B110)
		push	1
		push	0x3F800000
		push	kTileValue_mouseover
		mov		ecx, esi
		CALL_EAX(kAddr_TileSetFloat)
		mov		esi, g_HUDMainMenu
	noCurr:
		mov		[ebx+0xD0], esi
	done:
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn
	}
}

__declspec(naked) bool GetVanityDisabledHook()
{
	__asm
	{
		cmp		s_HUDCursorMode, 0
		jz		getCtrlFlag
		call	ProcessHUDMainUI
		mov		al, 1
		retn	4
	getCtrlFlag:
		test	byte ptr [ecx+0x680], 2
		setnz	al
		retn	4
	}
}

UInt32 s_bipedSlotVisibility = 0x7C;

__declspec(naked) void BipedSlotVisibilityHook()
{
	__asm
	{
		mov		ecx, s_bipedSlotVisibility
		bt		ecx, eax
		mov		eax, 0x4AC666
		mov		ecx, 0x4AC285
		cmovnc	eax, ecx
		jmp		eax
	}
}

TESClimate *s_forcedClimate = NULL;

__declspec(naked) void SkyRefreshClimateHook()
{
	__asm
	{
		CALL_EAX(0x404F00)
		mov		eax, s_forcedClimate
		test	eax, eax
		jz		done
		mov		[ebp+8], eax
	done:
		JMP_EAX(0x63C92E)
	}
}

__declspec(naked) void IsActorEssentialHook()
{
	__asm
	{
		test	byte ptr [ecx+0x106], kHookActorFlag2_TeammateKillable
		jnz		done
		mov		ecx, g_thePlayer
		cmp		byte ptr [ecx+0x7BC], 0
	done:
		setnz	al
		JMP_EDX(0x87F432)
	}
}

__declspec(naked) void FireWeaponWobbleHook()
{
	__asm
	{
		mov		ecx, [ebp-0x2C]
		test	byte ptr [ecx+0x106], kHookActorFlag2_NoGunWobble
		jnz		done
		push	2
		CALL_EAX(0x8B0DD0)
		fmul	dword ptr ds:[0x11CE038]
		fmul	qword ptr ds:[0x1023128]
		fadd	dword ptr [ebp-0x1C]
		fstp	dword ptr [ebp-0x1C]
	done:
		retn
	}
}

UnorderedMap<const char*, UInt32> s_eventMasks(0x20);
const char
kEventNames[] =
"OnActivate\0OnAdd\0OnEquip\0OnActorEquip\0OnDrop\0OnUnequip\0OnActorUnequip\0OnDeath\0OnMurder\0OnCombatEnd\0OnHit\0\
OnHitWith\0OnPackageStart\0OnPackageDone\0OnPackageChange\0OnLoad\0OnMagicEffectHit\0OnSell\0OnStartCombat\0OnOpen\0\
OnClose\0SayToDone\0OnGrab\0OnRelease\0OnDestructionStageChange\0OnFire\0OnTrigger\0OnTriggerEnter\0OnTriggerLeave\0OnReset",
kMenuNames[] =
"MessageMenu\0InventoryMenu\0StatsMenu\0HUDMainMenu\0LoadingMenu\0ContainerMenu\0DialogMenu\0SleepWaitMenu\0StartMenu\0\
LockpickMenu\0QuantityMenu\0MapMenu\0BookMenu\0LevelUpMenu\0RepairMenu\0RaceSexMenu\0CharGenMenu\0TextEditMenu\0BarterMenu\0\
SurgeryMenu\0HackingMenu\0VATSMenu\0ComputersMenu\0RepairServicesMenu\0TutorialMenu\0SpecialBookMenu\0ItemModMenu\0LoveTesterMenu\0\
CompanionWheelMenu\0TraitSelectMenu\0RecipeMenu\0SlotMachineMenu\0BlackjackMenu\0RouletteMenu\0CaravanMenu\0TraitMenu";
const UInt32
kEventMasks[] =
{
	0, 1, 2, 2, 4, 8, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000,
	0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
},
kMenuIDs[] =
{
	kMenuType_Message, kMenuType_Inventory, kMenuType_Stats, kMenuType_HUDMain, kMenuType_Loading, kMenuType_Container,
	kMenuType_Dialog, kMenuType_SleepWait, kMenuType_Start, kMenuType_LockPick, kMenuType_Quantity, kMenuType_Map, kMenuType_Book,
	kMenuType_LevelUp, kMenuType_Repair, kMenuType_RaceSex, kMenuType_CharGen, kMenuType_TextEdit, kMenuType_Barter, kMenuType_Surgery,
	kMenuType_Hacking, kMenuType_VATS, kMenuType_Computers, kMenuType_RepairServices, kMenuType_Tutorial, kMenuType_SpecialBook,
	kMenuType_ItemMod, kMenuType_LoveTester, kMenuType_CompanionWheel, kMenuType_TraitSelect, kMenuType_Recipe, kMenuType_SlotMachine,
	kMenuType_Blackjack, kMenuType_Roulette, kMenuType_Caravan, kMenuType_Trait
};

void InitJIPHooks()
{
	WritePushRetRelJump(0x86B38B, 0x86B3B2, (UInt32)GameMainLoopHook);
	SafeWrite32(0x87CE34, (UInt32)DoQueuedCmdCallHook);
	WriteRelCall(0x9459ED, (UInt32)GetVanityDisabledHook);

	HOOK_INIT_JUMP(GetDescription, 0x482F7F);
	HOOK_INIT_CALL(PCFastTravel, 0x798727);
	HOOK_INIT_CALL(PCCellChange, 0x94CB35);
	HOOK_INIT_JUMP(TextInputClose, 0x7E66A0);
	HOOK_INIT_JUMP(StartCombat, 0x9001C6);
	HOOK_INIT_JUMP(SetCombatTarget, 0x986C79);
	HOOK_INIT_JUMP(ResetHPCommand, 0x5C6B8B);
	HOOK_INIT_JUMP(ResetHPWakeUp, 0x8A0963);
	HOOK_INIT_JUMP(RemoveAmmo, 0x8A8DD0);
	HOOK_INIT_JUMP(ActivateDoor, 0x573673);
	HOOK_INIT_JUMP(TeleportWithPC, 0x8AD471);
	HOOK_INIT_JUMP(EquipItem, 0x88C87A);
	HOOK_INIT_JUMP(ReEquipAll, 0x6047C7);
	HOOK_INIT_JPRT(WeaponSwitchSelect, 0x999857, 0x99987A);
	HOOK_INIT_JPRT(WeaponSwitchUnequip, 0x9DA8E3, 0x9DA900);
	HOOK_INIT_JUMP(GetPreferedWeapon, 0x891C86);
	HOOK_INIT_JUMP(ShowMessage, 0x5B46AA);
	HOOK_INIT_JUMP(ShowTutorial, 0x7E88B6);
	HOOK_INIT_JUMP(MenuStateOpen, 0xA1F2F9);
	HOOK_INIT_JPRT(MenuStateClose, 0xA1C573, 0xA1C57D);
	HOOK_INIT_JUMP(SetAnimAction, 0x8A7551);
	HOOK_INIT_JPRT(SetAnimGroup, 0x494E19, 0x494E32);
	HOOK_INIT_JUMP(DamageActorValue, 0x66EE72);
	HOOK_INIT_JPRT(InitMoon, 0x634B30, 0x634BF4);
	HOOK_INIT_JPRT(UpdateWeather, 0x63D4F5, 0x63D528);
	HOOK_INIT_JUMP(MenuHandleMouseover, 0x70D56E);
	HOOK_INIT_JUMP(GetDetectionValue, 0x8A1535);
	HOOK_INIT_JPRT(SetPCTarget, 0x964675, 0x964684);
	HOOK_INIT_JUMP(PCActivateRef, 0x943279);
	HOOK_INIT_JUMP(SetRolloverText, 0x775B9B);
	HOOK_INIT_CALL(MergeEventMask, 0x5AC782);
	HOOK_INIT_JUMP(MarkScriptEvent, 0x5AC763);
	HOOK_INIT_JPRT(DoActivate, 0x573512, 0x57351F);
	HOOK_INIT_CALL(CreateMapMarkers, 0x79EBA1);
	HOOK_INIT_JUMP(GetRefName, 0x55D520);
	HOOK_INIT_JPRT(SetQuestStage, 0x60F4A9, 0x60F4C6);
	HOOK_INIT_JPRT(RunResultScript, 0x61F184, 0x61F190);
	HOOK_INIT_CALL(ScriptRunner, 0x5E0EC9);
	HOOK_INIT_JPRT(EvalEventBlock, 0x5E1716, 0x5E238A);
	HOOK_INIT_CALL(SetTerminalModel, 0x7FEF0B);
	HOOK_INIT_JUMP(AddVATSTarget, 0x7F54B1);
	HOOK_INIT_JPRT(LocationDiscover, 0x77955D, 0x779567);
	HOOK_INIT_JPRT(ItemCrafted, 0x7289FF, 0x728785);
	HOOK_INIT_JUMP(MakeObjLODPath, 0x6F6DF1);
	HOOK_INIT_JUMP(OnHitEvent, 0x5AC78A);
	HOOK_INIT_CALL(CheckUniqueItem, 0x47BBEC);
	HOOK_INIT_JPRT(ProjectileImpact, 0x9C20BF, 0x9C20C9);
	HOOK_INIT_JPRT(AddNote, 0x966AF9, 0x966B04);
	HOOK_INIT_JUMP(EquipAidItem, 0x88C74E);
	HOOK_INIT_JUMP(ReloadWeapon, 0x8A7549);
	HOOK_INIT_JUMP(OnRagdoll, 0xC7C151);
	HOOK_INIT_JPRT(PlayerMinHealth, 0x93B80A, 0x93B84F);
	HOOK_INIT_JUMP(ApplyActorVelocity, 0xC6D4E4);
	HOOK_INIT_JUMP(InsertObject, 0x440D49);
	HOOK_INIT_JPRT(SynchronizePosition, 0x575836, 0x575845);

	WriteRelJump(0x92C400, (UInt32)CopyHitDataHook);
	WriteRelJump(0x92C4A0, (UInt32)ResetHitDataHook);
	WriteRelJump(0x4AC645, (UInt32)BipedSlotVisibilityHook);
	WriteRelJump(0x63C929, (UInt32)SkyRefreshClimateHook);
	WriteRelJump(0x87F427, (UInt32)IsActorEssentialHook);
	WritePushRetRelJump(0x524014, 0x524042, (UInt32)FireWeaponWobbleHook);

	const char *strPos = kEventNames;
	UInt32 count = 0;
	do
	{
		s_eventMasks[strPos] = kEventMasks[count];
		strPos += StrLen(strPos) + 1;
	}
	while (++count < 30);

	strPos = kMenuNames;
	count = 0;
	do
	{
		s_menuNameToID[strPos] = kMenuIDs[count];
		strPos += StrLen(strPos) + 1;
	}
	while (++count < 36);

	PrintLog("> JIP hooks initialized successfully.");
}