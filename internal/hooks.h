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
	kHookFormFlag6_InsertNode =				1 << 6,
	kHookFormFlag6_AttachModel =			1 << 7,
	kHookFormFlag6_InsertObject =			kHookFormFlag6_InsertNode | kHookFormFlag6_AttachModel,
	kHookFormFlag6_UniqueItem =				1 << 8,

	kHookFormFlag5_FastTravelInformed =		1 << 0,
	kHookFormFlag5_CellChangeInformed =		1 << 1,
	kHookFormFlag5_MenuInputInformed =		1 << 2,
	kHookFormFlag5_ScriptOnWait =			1 << 3,

	kHookRefFlag61_Update3D =				1 << 0,
	kHookRefFlag61_DisableCollision =		1 << 1,

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
	UInt32		patchAddr;			// 00
	UInt8		length;				// 04
	UInt8		pad05[3];			// 05
	UInt8		instructions[20];	// 08
	UInt32		userCount;			// 1C

	void Init(UInt32 _patchAddr, void *hookPtr, UInt8 type, UInt32 retAddr = 0);
	bool Set(bool install);
	void ModUsers(bool add);
}
s_hookInfos[kHook_Max];

enum
{
	kHookInfoSize =	sizeof(HookInfo)
};

__declspec(naked) void HookInfo::Init(UInt32 _patchAddr, void *hookPtr, UInt8 type, UInt32 retAddr)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		dword ptr [esi+0x1C], 0
		mov		edx, [esp+0x14]
		mov		eax, 5
		mov		ecx, 0xA
		test	edx, edx
		cmovnz	eax, ecx
		mov		[esi+4], al
		mov		ecx, [esp+8]
		mov		[esi], ecx
		movups	xmm0, [ecx]
		movups	[esi+8], xmm0
		add		ecx, eax
		mov		al, [esp+0x10]
		mov		[esi+0x12], al
		mov		eax, [esp+0xC]
		sub		eax, ecx
		test	edx, edx
		jnz		isRET
		mov		[esi+0x13], eax
		jmp		done
	isRET:
		mov		[esi+0x13], edx
		mov		byte ptr [esi+0x17], 0xE9
		mov		[esi+0x18], eax
	done:
#if LOG_HOOKS
		push	dword ptr [esi+4]
		push	dword ptr [esp+0xC]
		call	StoreOriginalData
#endif
		pop		esi
		retn	0x10
	}
}

__declspec(naked) bool HookInfo::Set(bool install)
{
	__asm
	{
		mov		eax, [esp+4]
		cmp		dword ptr [ecx+0x1C], 0
		setnz	dl
		cmp		al, dl
		jnz		proceed
		xor		al, al
		retn	4
	proceed:
		mov		[ecx+0x1C], eax
		push	esi
		mov		esi, ecx
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	0xA
		push	dword ptr [esi]
		call	VirtualProtect
		mov		eax, 8
		mov		ecx, 0x12
		cmp		dword ptr [esi+0x1C], 0
		cmovnz	eax, ecx
		add		eax, esi
		mov		ecx, [esi]
		mov		edx, [eax]
		mov		[ecx], edx
		cmp		byte ptr [esi+4], 5
		jnz		copy10
		mov		dl, [eax+4]
		mov		[ecx+4], dl
		jmp		doneCopy
	copy10:
		mov		edx, [eax+4]
		mov		[ecx+4], edx
		mov		dx, [eax+8]
		mov		[ecx+8], dx
	doneCopy:
		mov		edx, [esp]
		push	esp
		push	edx
		push	0xA
		push	dword ptr [esi]
		call	VirtualProtect
		pop		ecx
		mov		al, 1
		pop		esi
		retn	4
	}
}

__declspec(naked) void HookInfo::ModUsers(bool add)
{
	__asm
	{
		mov		edx, [ecx+0x1C]
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
		mov		[ecx+0x1C], edx
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
	LambdaVariableContext lambdaVariableContext;
	
	static MainLoopCallback *Create(void *_cmdPtr, void *_thisObj, UInt32 _callCount = 1, UInt32 _callDelay = 1, UInt8 _numArgs = 0);

	void Execute();

	void Destroy()
	{
		if (numArgs > 1)
			POOL_FREE(pArgs, numArgs, FunctionArg);
		POOL_FREE(this, 1, MainLoopCallback);
	}
};

Vector<MainLoopCallback*> s_mainLoopCallbacks(0x50);

MainLoopCallback *MainLoopCallback::Create(void *_cmdPtr, void *_thisObj, UInt32 _callCount, UInt32 _callDelay, UInt8 _numArgs)
{
	MainLoopCallback *callback = POOL_ALLOC(1, MainLoopCallback);
	callback->cmdPtr = _cmdPtr;
	callback->thisObj = _thisObj;
	*(UInt32*)&callback->numArgs = _numArgs;
	callback->callCount = _callCount;
	callback->callDelay = _callDelay;
	callback->cycleCount = _callDelay;
	if (_numArgs > 1)
		callback->pArgs = POOL_ALLOC(_numArgs, FunctionArg);
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

__declspec(naked) void __fastcall CycleMainLoopCallbacks(Vector<MainLoopCallback*> *mlCallbacks)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, ecx
		mov		esi, [ebx]
		mov		edi, [ebx+4]
		ALIGN 16
	cycleHead:
		dec		edi
		js		cycleEnd
		mov		ecx, [esi]
		add		esi, 4
		cmp		[ecx+9], 0
		jnz		cycleHead
		cmp		[ecx+0xA], 0
		jnz		isScript
		dec		dword ptr [ecx+0x14]
		jnz		cycleHead
		dec		dword ptr [ecx+0xC]
		jnz		resetCount
		mov		[ecx+9], 1
		jmp		doExecute
		ALIGN 16
	resetCount:
		mov		eax, [ecx+0x10]
		mov		[ecx+0x14], eax
	doExecute:
		call	MainLoopCallback::Execute
		jmp		cycleHead
		ALIGN 16
	isScript:
		mov		eax, g_interfaceManager
		cmp		[eax+0xC], 1
		seta	al
		inc		al
		test	al, [ecx+0xB]
		jz		cycleHead
		dec		dword ptr [ecx+0x14]
		jnz		cycleHead
		mov		eax, [ecx+0x18]
		cmp		eax, 0x14
		jz		doCallFunc
		push	eax
		call	LookupFormByRefID
		mov		ecx, [esi-4]
		cmp		[ecx+4], eax
		jz		doCallFunc
		mov		[ecx+9], 1
		jmp		cycleHead
		ALIGN 16
	doCallFunc:
		mov		edx, [ecx+0x10]
		mov		[ecx+0x14], edx
		push	0
		push	dword ptr [ecx+4]
		push	dword ptr [ecx]
		call	CallFunction
		add		esp, 0xC
		jmp		cycleHead
		ALIGN 16
	cycleEnd:
		mov		eax, [ebx]
		mov		edi, [ebx+4]
		lea		esi, [eax+edi*4]
		ALIGN 16
	removeHead:
		dec		edi
		js		done
		sub		esi, 4
		mov		ecx, [esi]
		cmp		[ecx+9], 0
		jz		removeHead
		call	MainLoopCallback::Destroy
		mov		eax, [ebx+4]
		dec		eax
		mov		[ebx+4], eax
		sub		eax, edi
		jz		removeHead
		shl		eax, 2
		push	eax
		lea		edx, [esi+4]
		push	edx
		push	esi
		call	MemCopy
		add		esp, 0xC
		jmp		removeHead
		ALIGN 16
	done:
		pop		edi
		pop		esi
		pop		ebx
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
		cmp		dword ptr s_fastTravelEventScripts+4, 0
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
	char *currText = menu->currentText.m_data;
	SInt32 currIdx = menu->cursorIndex, newIdx;
	UInt16 length = menu->currentText.m_dataLen;
	if (inputKey < kInputCode_Backspace)
	{
		if (length >= menu->maxLength)
			return false;
		if (menu->miscFlags & 1)
		{
			if (inputKey == '-')
			{
				if (currIdx || (*currText == '-'))
					return false;
			}
			else if (inputKey == '.')
			{
				if (FindChr(currText, '.'))
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
			chrPtr = FindChrR(currText, currIdx, '\n');
			newIdx = chrPtr ? (chrPtr - currText + 1) : 0;
			if (newIdx == currIdx) break;
			menu->cursorIndex = newIdx;
			return true;
		}
		case kInputCode_End:
		{
			chrPtr = FindChr(currText + currIdx, '\n');
			newIdx = chrPtr ? (chrPtr - currText) : length;
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
		mov		ecx, offset s_hookInfos+kHook_TextInputClose*kHookInfoSize
		call	HookInfo::Set
		mov		eax, [esi+0x34]
		mov		edx, 0x1011584
		test	eax, eax
		cmovz	eax, edx
		push	eax
		push	1
		push	0
		push	dword ptr [esi+0x58]
		call	CallFunction
		add		esp, 0x10
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
		char lastClickedTilePath[0x80];
		clickedTile->GetComponentFullName(lastClickedTilePath);
		StrToLower(lastClickedTilePath);
		for (auto filter = clickEvent.filtersMap.FindOpDir(lastClickedTilePath, false); filter; --filter)
		{
			if (!StrBeginsCS(lastClickedTilePath, filter.Key())) break;
			for (auto script = filter().BeginCp(); script; ++script)
				CallFunction(*script, NULL, 3, menu->id, tileID, clickedTile->name.m_data);
		}
	}
	if (!clickEvent.idsMap.Empty())
	{
		EventCallbackScripts *callbacks = clickEvent.idsMap.GetPtr(tileID);
		if (callbacks)
		{
			const char *tileName = clickedTile ? clickedTile->name.m_data : "";
			for (auto script = callbacks->BeginCp(); script; ++script)
				CallFunction(*script, NULL, 3, menu->id, tileID, tileName);
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
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		contRetn
		mov		eax, [eax+0x10]
		cmp		[ebp+0x1C], eax
		jnz		contRetn
		mov		eax, [eax+8]
		test	eax, eax
		jz		contRetn
		mov		edx, [ebp+0x10]
		ALIGN 16
	loopHead:
		mov		ecx, [eax]
		test	ecx, ecx
		jz		contRetn
		mov		eax, [eax+4]
		test	byte ptr [ecx+4], 0x40
		jz		loopHead
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		skipRetn
		cmp		ecx, edx
		jz		skipRetn
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
		cmp		dword ptr [esi+0xC], 0x14
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
		call	BaseExtraList::GetByType
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

bool s_forceEquipCall = false;

__declspec(naked) bool __fastcall GetCannotWearHook(ExtraDataList *xData)
{
	__asm
	{
		test	byte ptr [ecx+0xF], 0x40
		jz		done
		cmp		s_forceEquipCall, 1
	done:
		setnz	al
		retn
	}
}

__declspec(naked) void EquipItemHook()
{
	__asm
	{
		movzx	eax, byte ptr [ecx+4]
		cmp		al, kFormType_TESObjectWEAP
		jnz		contRetn
		cmp		s_forceEquipCall, 0
		jnz		contRetn
		cmp		dword ptr [ebp+4], 0x9155A2
		jz		contRetn
		mov		ecx, [ebp-0x80]
		test	byte ptr [ecx+0x105], kHookActorFlag1_LockedEquipment
		jz		contRetn
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
		cmp		dword ptr [eax+4], 0x5CD73B	//	Cmd_SetCombatStyle_Execute
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
		call	BaseExtraList::GetByType
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
		cmova	edx, [eax+4]
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
		test	byte ptr [ecx+6], kHookFormFlag6_MessageDisabled
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
		test	byte ptr [ecx+6], kHookFormFlag6_MessageDisabled
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
		lea		ecx, [eax+0x11F308F]
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
		lea		ecx, [edx+0x11F308F]
		cmp		byte ptr [ecx], 0
		jz		done
		mov		byte ptr [ecx], 0
		movzx	eax, kMenuIDJumpTable[edx-kMenuType_Min]
		cmp		al, 0xFF
		jz		done
		mov		ecx, s_menuStateEventMap[eax*4]
		test	ecx, ecx
		jz		done
		cmp		dword ptr [ecx+0x10], 0
		jz		done
		mov		eax, g_thePlayer
		cmp		dword ptr [eax+0x40], 0
		jz		done
		push	dword ptr [ebp-0x28]
		add		ecx, 0xC
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
		mov		ecx, [eax+0xD0]
		test	ecx, ecx
		cmovz	ecx, lastMenuID
		jz		gotID
		mov		ecx, [ecx+0x20]
		mov		lastMenuID, ecx
	gotID:
		movzx	edx, kMenuIDJumpTable[ecx-kMenuType_Min]
		cmp		dl, 0xFF
		jz		skipEvt
		mov		ecx, s_menuStateEventMap[edx*4]
		test	ecx, ecx
		jz		skipEvt
		cmp		dword ptr [ecx+0x1C], 0
		jz		skipEvt
		mov		edx, g_thePlayer
		cmp		dword ptr [edx+0x40], 0
		jz		skipEvt
		push	lastMenuID
		add		ecx, 0x18
		call	EventCallbackScripts::InvokeEvents
		mov		eax, [ebp-0xCC]
	skipEvt:
		cmp		byte ptr [eax+0xED], 0
		jz		done
		mov		dword ptr [eax+0xBC], 0
	done:
		retn
	}
}

typedef UnorderedMap<Actor*, AnimEventCallbacks> ActorAnimEventCallbacks;
AnimEventCallbacks s_animActionEventMap, s_playGroupEventMap;
ActorAnimEventCallbacks s_animActionEventMapFl, s_playGroupEventMapFl;

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
		mov		ecx, offset s_animActionEventMap
		cmp		dword ptr [ecx+8], 0
		jz		doFiltered
		push	dword ptr [ebp+8]
		call	AnimEventCallbacks::GetPtr
		test	eax, eax
		jz		doFiltered
		push	dword ptr [ebp+8]
		push	dword ptr [ebp-0x10]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	doFiltered:
		mov		ecx, [ebp-0x10]
		test	byte ptr [ecx+0x107], kHookActorFlag3_OnAnimAction
		jz		done
		push	ecx
		mov		ecx, offset s_animActionEventMapFl
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
		cmp		dword ptr [ecx+0xC], 0x14
		jnz		notPlayer
		cmp		[ecx+0x690], eax
		jz		done
	notPlayer:
		mov		ecx, offset s_playGroupEventMap
		cmp		dword ptr [ecx+8], 0
		jz		doFiltered
		push	dword ptr [ebp-0x28]
		call	AnimEventCallbacks::GetPtr
		test	eax, eax
		jz		doFiltered
		push	dword ptr [ebp-0x28]
		push	dword ptr [ebp-0x18]
		mov		ecx, eax
		call	EventCallbackScripts::InvokeEventsThis1
	doFiltered:
		mov		ecx, [ebp-0x18]
		test	byte ptr [ecx+0x107], kHookActorFlag3_OnPlayGroup
		jz		done
		push	ecx
		mov		ecx, offset s_playGroupEventMapFl
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
		pxor	xmm1, xmm1
		comiss	xmm0, xmm1
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
		pxor	xmm0, xmm0
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
		push	esi
		mov		esi, [ebp-0x18]
		mov		eax, offset s_moonTexturesSet
		push	dword ptr [eax+0x1C]
		push	dword ptr [eax+0x18]
		push	dword ptr [eax+0x14]
		push	dword ptr [eax+0x10]
		push	dword ptr [eax+0xC]
		push	dword ptr [eax+8]
		push	dword ptr [eax+4]
		push	dword ptr [eax]
		lea		ecx, [esi+0x18]
		call	String::Set
		lea		ecx, [esi+0x20]
		call	String::Set
		lea		ecx, [esi+0x28]
		call	String::Set
		lea		ecx, [esi+0x30]
		call	String::Set
		lea		ecx, [esi+0x38]
		call	String::Set
		lea		ecx, [esi+0x40]
		call	String::Set
		lea		ecx, [esi+0x48]
		call	String::Set
		lea		ecx, [esi+0x50]
		call	String::Set
		pop		esi
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
		test	byte ptr [ecx+6], kHookFormFlag6_NoPCActivation
		jnz		skipRetn
		mov		eax, [ecx+0x20]
		test	byte ptr [eax+6], kHookFormFlag6_NoPCActivation
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
		test	byte ptr [edx+6], kHookFormFlag6_NoPCActivation
		jnz		nullRef
		mov		eax, [edx+0x20]
		test	byte ptr [eax+6], kHookFormFlag6_NoPCActivation
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
		call	BaseExtraList::GetByType
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
		test	byte ptr [ecx+6], kHookFormFlag6_EventDisabled
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
		test	byte ptr [ecx+6], kHookFormFlag6_EventDisabled
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
		test	byte ptr [ecx+6], kHookFormFlag6_ActivateDisabled
		jnz		retnTrue
		mov		edx, [ecx+0x20]
		test	byte ptr [edx+6], kHookFormFlag6_ActivateDisabled
		jnz		retnTrue
		push	kExtraData_Action
		add		ecx, 0x44
		call	BaseExtraList::GetByType
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
		test	byte ptr [ecx+6], kHookFormFlag6_AltRefName
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
		dec		edi
		js		iterEnd
		mov		ecx, esi
		add		esi, 8
		mov		edx, [ebp-8]
		cmp		[ecx], dl
		jnz		iterHead
		push	edx
		push	dword ptr [ebp-4]
		push	2
		push	0
		push	dword ptr [ecx+4]
		call	CallFunction
		add		esp, 0x14
		or		bl, [esi-7]
		jmp		iterHead
		ALIGN 16
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
		test	byte ptr [eax+6], kHookFormFlag6_SetStageHandlers
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
	for (auto iter = s_criticalHitEvents.BeginCp(); iter; ++iter)
		if ((!iter().target || (iter().target == hitData->target)) && (!iter().source || (iter().source == hitData->source)) && (!iter().weapon || (iter().weapon == hitData->weapon)))
			CallFunction(iter().callback, hitData->target, 2, hitData->source, hitData->weapon);
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
		movups	xmm0, [edx]
		movups	[eax], xmm0
		movups	xmm0, [edx+0x10]
		movups	[eax+0x10], xmm0
		movups	xmm0, [edx+0x20]
		movups	[eax+0x20], xmm0
		movups	xmm0, [edx+0x30]
		movups	[eax+0x30], xmm0
		movups	xmm0, [edx+0x40]
		movups	[eax+0x40], xmm0
		movups	xmm0, [edx+0x50]
		movups	[eax+0x50], xmm0
		mov		ecx, [edx+0x10]
		mov		[eax+0x60], ecx
		test	byte ptr [eax+0x58], 4
		jz		done
		cmp		s_criticalHitEvents.numItems, 0
		jz		done
		mov		ecx, eax
		call	InvokeActorHitEvents
	done:
		retn	4
	}
}

struct ScriptWaitInfo
{
	TESForm		*owner;				// 00
	UInt32		refID;				// 04
	UInt32		iterNum;			// 08
	UInt32		blockOffset;		// 0C
	UInt32		savedOffset;		// 10
	UInt32		savedIfStack[12];	// 14

	ScriptWaitInfo() {}

	void Init(TESForm *_owner, UInt32 _iterNum, UInt32 _blockOffset, UInt32 *opcodeOffsetPtr)
	{
		owner = _owner;
		refID = owner->refID;
		iterNum = _iterNum;
		blockOffset = _blockOffset;
		savedOffset = *opcodeOffsetPtr - blockOffset;
		ScriptRunner *scrRunner = *(ScriptRunner**)(opcodeOffsetPtr - 0x3B0);
		memcpy(savedIfStack, &scrRunner->ifStackDepth, 0x30);
	}
}
*s_scriptWaitInfo = NULL;

typedef UnorderedMap<TESForm*, ScriptWaitInfo> ScriptWaitInfoMap;
ScriptWaitInfoMap s_scriptWaitInfoMap;

__declspec(naked) Script::ScriptInfo* __fastcall ScriptRunnerHook(Script *script)
{
	__asm
	{
		xor		eax, eax
		cmp		word ptr [ecx+0x28], 1
		ja		done
		mov		ecx, [ecx+0x40]
		cmovnz	ecx, [ebp+0xC]
		test	ecx, ecx
		jz		done
		test	byte ptr [ecx+5], kHookFormFlag5_ScriptOnWait
		jz		done
		push	ecx
		mov		ecx, offset s_scriptWaitInfoMap
		call	ScriptWaitInfoMap::GetPtr
	done:
		mov		s_scriptWaitInfo, eax
		mov		eax, [ebp+8]
		add		eax, 0x18
		retn
	}
}

__declspec(naked) void EvalEventBlockHook()
{
	__asm
	{
		cmp		dword ptr [ebp-0x28], 0
		setnz	al
		jz		retnFalse
		cmp		byte ptr [ebp+0x28], 0
		setz	al
	retnFalse:
		mov		ecx, s_scriptWaitInfo
		test	ecx, ecx
		jz		notWaiting
		mov		edx, [ecx+0xC]
		cmp		[ebp-0x10], edx
		jnz		notWaiting
		cmp		dword ptr [ebp-0x18], 1
		ja		evntBlock
		test	al, al
		jz		skipBlock
	evntBlock:
		dec		dword ptr [ecx+8]
		jnz		skipBlock
		mov		eax, [ecx+0x10]
		mov		edx, [ebp+0x14]
		add		[edx], eax
		mov		edx, [ebp-0xED0]
		movups	xmm0, [ecx+0x14]
		movups	[edx+0x20], xmm0
		movups	xmm0, [ecx+0x24]
		movups	[edx+0x30], xmm0
		movups	xmm0, [ecx+0x34]
		movups	[edx+0x40], xmm0
		mov		s_scriptWaitInfo, 0
		mov		eax, [ecx]
		and		byte ptr [eax+5], ~kHookFormFlag5_ScriptOnWait
		retn
	notWaiting:
		test	al, al
		jnz		done
	skipBlock:
		mov		eax, [ebp+0x14]
		mov		edx, [ebp-0x20]
		add		[eax], edx
	done:
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
		cmp		dword ptr [eax+0x100], kAddr_ReturnTrue
		setnz	al
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
		test	byte ptr [ebp+0x10], 0x80
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
		cmp		dword ptr s_onHitEventScripts+4, 0
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
		test	byte ptr [eax+6], kHookFormFlag6_ProjectileImpact
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
		test	byte ptr [eax+6], kHookFormFlag6_OnEquipHandlers
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
		mov		eax, 0x93ACB0
		call	eax
		fst		dword ptr [ebp-0x40]
		push	kAVCode_Health
		mov		ecx, [ebp-0x10]
		mov		eax, 0x93AD60
		call	eax
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
		test	byte ptr [ecx+0x417], 0x80
		jz		contRetn
		pxor	xmm0, xmm0
		comiss	xmm0, [ecx+0x524]
		jnb		rmvFlag
		JMP_EDX(0xC6D5A5)
	rmvFlag:
		and		byte ptr [ecx+0x417], 0x7F
	contRetn:
		JMP_EDX(0xC6D4E9)
	}
}

typedef bool (__thiscall *_SetFormEDID)(TESForm *form, const char *EDID);
_SetFormEDID SetFormEDID = nullptr;

UnorderedMap<const char*, TESForm*> s_lightFormEDIDMap(0x400);

__declspec(naked) bool __fastcall TESObjectLIGHSetEDIDHook(TESObjectLIGH *lightForm, int EDX, const char *EDID)
{
	__asm
	{
		push	ecx
		push	ecx
		push	esp
		push	dword ptr [esp+0x10]
		mov		ecx, offset s_lightFormEDIDMap
		call	UnorderedMap<const char*, TESForm*>::Insert
		pop		eax
		pop		ecx
		mov		[eax], ecx
		jmp		SetFormEDID
	}
}

NiString s_LIGH_EDID;

__declspec(naked) void __fastcall InitPointLights(NiNode *niNode)
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	blockIter:
		dec		edi
		js		blockEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		blockIter
		mov		eax, [ecx]
		cmp		eax, kVtbl_NiPointLight
		jz		isLight
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		blockIter
		call	InitPointLights
		jmp		blockIter
		ALIGN 16
	isLight:
		push	ebx
		mov		ebx, [ecx+0x10]
		movzx	eax, word ptr [ecx+0x14]
		mov		edx, s_LIGH_EDID.str
		ALIGN 16
	xtraIter:
		dec		eax
		js		xtraEnd
		mov		ecx, [ebx+eax*4]
		test	ecx, ecx
		jz		xtraIter
		cmp		[ecx+8], edx
		jnz		xtraIter
		push	eax
		mov		ebx, [esi-4]
		cmp		dword ptr [ecx], kVtbl_NiStringExtraData
		jnz		removeExtra
		mov		eax, [ecx+0xC]
		test	eax, eax
		jz		removeExtra
		push	eax
		mov		ecx, offset s_lightFormEDIDMap
		call	UnorderedMap<const char*, TESForm*>::Get
		test	eax, eax
		jz		removeExtra
		mov		ecx, ebx
		or		byte ptr [ecx+0x33], 0x20
		mov		[ecx+0xE8], eax
		ALIGN 16
	parentIter:
		mov		ecx, [ecx+0x18]
		test	byte ptr [ecx+0x33], 0x20
		jnz		removeExtra
		or		byte ptr [ecx+0x33], 0x20
		cmp		ecx, ebp
		jnz		parentIter
	removeExtra:
		mov		ecx, ebx
		CALL_EAX(0xA5B990)
	xtraEnd:
		pop		ebx
		jmp		blockIter
		ALIGN 16
	blockEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void LoadNifRetnNodeHook()
{
	__asm
	{
		mov		ecx, [ebp-0x10]
		cmp		byte ptr [ebp+0x1C], 0
		jnz		skipInc
		lock inc dword ptr [ecx+8]
	skipInc:
		mov		eax, [ecx+0xC]
		test	eax, eax
		jz		done
		test	byte ptr [eax+0x33], 0x40
		jnz		done
		or		byte ptr [eax+0x33], 0x40
		push	ebp
		mov		ebp, eax
		mov		ecx, eax
		call	InitPointLights
		mov		eax, ebp
		pop		ebp
	done:
		retn
	}
}

Vector<NiPointLight*> s_activePtLights(0x40);

__declspec(naked) NiPointLight* __fastcall DestroyNiPointLightHook(NiPointLight *ptLight, int EDX, bool doFree)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		CALL_EAX(0xA72010)
		test	byte ptr [esi+0x33], 0x20
		jnz		done
		test	byte ptr [esp+8], 1
		jz		done
		push	0xFC
		push	esi
		CALL_EAX(0xAA1460)
		add		esp, 8
	done:
		mov		eax, esi
		pop		esi
		retn	4
	}
}

__declspec(naked) NiPointLight* __fastcall CreatePointLight(TESObjectLIGH *lightForm, NiNode *destParent)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	edx
		sub		esp, 0x14
		push	esi
		push	edi
		mov		esi, ecx
		movzx	eax, word ptr [edx+0xA6]
		mov		ecx, [edx+0xA0]
		ALIGN 16
	iterHead:
		dec		eax
		js		iterEnd
		mov		edx, [ecx]
		add		ecx, 4
		test	edx, edx
		jz		iterHead
		cmp		dword ptr [edx], kVtbl_NiPointLight
		jnz		iterHead
		mov		edi, edx
		jmp		hasLight
		ALIGN 16
	iterEnd:
		CALL_EAX(0xA7D6E0)
		mov		edi, eax
		mov		edx, [esi+0xC]
		lea		ecx, [ebp-0x10]
		mov		dword ptr [ecx-8], 'iLtP'
		mov		dword ptr [ecx-4], ' thg'
		call	UIntToHex
		lea		edx, [ebp-0x18]
		mov		ecx, edi
		call	NiObjectNET::SetName
		push	1
		push	edi
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
	hasLight:
		mov		[edi+0xE8], esi
		test	byte ptr [esi+0xA8], 0x20
		setnz	al
		or		[edi+0x30], al
		mov		[edi+0x9E], 1
		test	byte ptr [edi+0x33], 0x40
		jnz		done
		or		byte ptr [edi+0x33], 0x60
		push	edi
		mov		ecx, offset s_activePtLights
		call	Vector<NiPointLight*>::Append
	done:
		mov		eax, edi
		pop		edi
		pop		esi
		leave
		retn
	}
}

__declspec(naked) void __fastcall SetLightProperties(NiPointLight *ptLight, TESObjectLIGH *lightForm)
{
	static const __m128 kColourMults = {1.0 / 255, 1.0 / 255, 1.0 / 255, 1};
	__asm
	{
		add		ecx, 0xC4
		add		edx, 0xA0
		mov		eax, [edx+0x14]
		mov		[ecx], eax
		pxor	xmm0, xmm0
		movups	[ecx+4], xmm0
		movups	[ecx+0x28], xmm0
		xor		eax, eax
		mov		al, [edx+4]
		mov		[ecx+0x10], eax
		mov		al, [edx+5]
		mov		[ecx+0x14], eax
		mov		al, [edx+6]
		mov		[ecx+0x18], eax
		mov		eax, [edx]
		mov		[ecx+0x1C], eax
		movups	xmm0, [ecx+0x10]
		cvtdq2ps	xmm0, xmm0
		mulps	xmm0, kColourMults
		movups	[ecx+0x10], xmm0
		sub		ecx, 0xC4
		sub		edx, 0xA0
		retn
	}
}

__declspec(naked) void __fastcall AddPointLights(NiNode *objNode)
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		test	byte ptr [ecx+0x33], 0x20
		jz		iterHead
		mov		eax, [ecx]
		cmp		eax, kVtbl_NiPointLight
		jz		isLight
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		iterHead
		call	AddPointLights
		jmp		iterHead
		ALIGN 16
	isLight:
		test	byte ptr [ecx+0x33], 0x40
		jnz		iterHead
		or		byte ptr [ecx+0x33], 0x40
		mov		edx, [ecx+0xE8]
		test	byte ptr [edx+0xA8], 0x20
		setnz	al
		or		[ecx+0x30], al
		call	SetLightProperties
		push	ecx
		mov		ecx, offset s_activePtLights
		call	Vector<NiPointLight*>::Append
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void __fastcall HidePointLights(NiNode *objNode)
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		test	byte ptr [ecx+0x33], 0x20
		jz		iterHead
		mov		eax, [ecx]
		cmp		eax, kVtbl_NiPointLight
		jz		isLight
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		iterHead
		call	HidePointLights
		jmp		iterHead
		ALIGN 16
	isLight:
		or		byte ptr [ecx+0x30], 1
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

typedef Map<char*, NiString, 2> DataStrings;
typedef Map<NiString, DataStrings, 2> NodeNamesMap;
UnorderedMap<TESForm*, NodeNamesMap> s_insertNodeMap, s_attachModelMap;

__declspec(naked) void __fastcall DoInsertNode(NiAVObject *targetObj, const char *nodeName, const char *nameStr, NiNode *rootNode)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		mov		ecx, [esp+0xC]
		cmp		[edx], '^'
		mov		edx, [esp+8]
		jz		asParent
		mov		eax, [ebx]
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		done
		call	NiNode::GetBlockByName
		test	eax, eax
		jnz		done
		push	edx
		call	NiNode::Create
		or		byte ptr [eax+0x33], 0x80
		push	1
		push	eax
		mov		ecx, ebx
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
		pop		ebx
		retn	8
		ALIGN 16
	asParent:
		cmp		ecx, ebx
		jz		done
		call	NiNode::GetBlockByName
		test	eax, eax
		jz		doCreate
		mov		edx, [eax]
		cmp		dword ptr [edx+0xC], kAddr_ReturnThis
		jnz		done
		push	1
		push	ebx
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
		pop		ebx
		retn	8
		ALIGN 16
	doCreate:
		push	edx
		call	NiNode::Create
		or		byte ptr [eax+0x33], 0x80
		push	eax
		push	eax
		mov		ecx, ebx
		call	NiAVObject::GetIndex
		push	eax
		mov		ecx, [ebx+0x18]
		mov		eax, [ecx]
		call	dword ptr [eax+0xE0]
		pop		ecx
		push	1
		push	ebx
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
	done:
		pop		ebx
		retn	8
	}
}

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
		sub		esp, 0xC
		push	esi
		push	edi
		mov		esi, [eax]
		mov		edx, [eax+4]
		lea		eax, [esi+edx*8]
		mov		[ebp-4], eax
		ALIGN 16
	nodeHead:
		cmp		[ebp-4], esi
		jz		nodeEnd
		mov		edi, esi
		add		esi, 8
		mov		eax, [ebp+8]
		mov		edx, [edi]
		test	edx, edx
		jz		gotDest
		cmp		[eax+8], edx
		jz		gotDest
		mov		ecx, eax
		call	NiNode::GetBlockByName
		test	eax, eax
		jz		nodeHead
	gotDest:
		mov		[ebp-0xC], eax
		mov		eax, [edi+4]
		mov		edi, [eax]
		mov		edx, [eax+4]
		lea		eax, [edi+edx*8]
		mov		[ebp-8], eax
		ALIGN 16
	insHead:
		cmp		[ebp-8], edi
		jz		nodeHead
		push	dword ptr [ebp+8]
		push	dword ptr [edi+4]
		mov		edx, [edi]
		mov		ecx, [ebp-0xC]
		call	DoInsertNode
		add		edi, 8
		jmp		insHead
		ALIGN 16
	nodeEnd:
		pop		edi
		pop		esi
		leave
	done:
		retn	4
	}
}

__declspec(naked) NiNode* __fastcall LoadModelCopy(const char *filePath)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	0
		push	esp
		push	ecx
		mov		eax, g_modelLoader
		mov		ecx, [eax]
		CALL_EAX(0x4493D0)
		test	al, al
		pop		eax
		jnz		hasModel
		sub		esp, 0x44
		push	0
		push	1
		push	0
		push	0
		push	dword ptr [ebp-4]
		lea		ecx, [ebp-0x48]
		CALL_EAX(0x43C6E0)
		or		byte ptr [eax+0x3C], 0x10
		mov		ecx, eax
		CALL_EAX(0x43CCF0)
		lea		ecx, [ebp-0x48]
		CALL_EAX(0x43D180)
		lea		ecx, [ebp-0x48]
		CALL_EAX(0x43C830)
		mov		eax, [ebp-0x18]
		test	eax, eax
		jz		done
	hasModel:
		cmp		dword ptr [eax+8], 0
		jnz		skipInc
		lock inc dword ptr [eax+8]
	skipInc:
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
		mov		ecx, eax
		test	byte ptr [ecx+0x33], 0x40
		jnz		doCopy
		or		byte ptr [ecx+0x33], 0x40
		push	ebp
		mov		ebp, ecx
		call	InitPointLights
		mov		ecx, ebp
		pop		ebp
	doCopy:
		call	NiNode::CreateCopy
	done:
		leave
		retn
	}
}

__declspec(naked) void __fastcall AppendNameSuffix(NiAVObject *object, const char *suffix)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x50
		push	ecx
		push	edx
		push	ebx
		lea		ebx, [ebp-0x50]
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		cpyIter2
		ALIGN 16
	cpyIter1:
		mov		al, [ecx]
		test	al, al
		jz		cpyIter2
		mov		[ebx], al
		inc		ecx
		inc		ebx
		jmp		cpyIter1
		ALIGN 16
	cpyIter2:
		mov		al, [edx]
		cmp		al, '*'
		jz		done
		mov		[ebx], al
		inc		edx
		inc		ebx
		jmp		cpyIter2
		ALIGN 16
	done:
		mov		[ebx], 0
		lea		edx, [ebp-0x50]
		mov		ecx, [ebp-0x54]
		call	NiObjectNET::SetName
		pop		ebx
		pop		edx
		pop		ecx
		leave
		retn
	}
}

__declspec(naked) void __fastcall AppendBlockNameSuffixes(NiNode *node, const char *suffix)
{
	__asm
	{
		call	AppendNameSuffix
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		notNode
		call	AppendBlockNameSuffixes
		jmp		iterHead
		ALIGN 16
	notNode:
		call	AppendNameSuffix
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) NiNode* __fastcall DoAttachModel(NiAVObject *targetObj, const char *modelPath, NiString *nameStr, NiNode *rootNode)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	edx
		mov		ecx, edx
		cmp		[ecx], '*'
		setz	al
		push	eax
		jnz		noSuffix
		inc		ecx
		mov		[ebp-8], ecx
		mov		dl, '*'
		call	FindChr
		lea		ecx, [eax+1]
	noSuffix:
		call	LoadModelCopy
		test	eax, eax
		jz		done
		push	eax
		push	1
		push	eax
		mov		ecx, eax
		cmp		byte ptr [ebp-0xC], 0
		jz		doneSuffix
		mov		edx, [ebp-8]
		call	AppendBlockNameSuffixes
		mov		ecx, [ebp-0x10]
	doneSuffix:
		mov		eax, [ebp+8]
		cmp		dword ptr [eax], 0
		jnz		doneName
		mov		edx, [ecx+8]
		lock inc dword ptr [edx-8]
		mov		[eax], edx
	doneName:
		call	NiNode::RemoveCollision
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
		mov		ecx, [ebp-0x10]
		CALL_EAX(0xA5A040)
		push	0
		push	0
		push	esp
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0xC0]
		mov		eax, [ebp-0x10]
		or		byte ptr [eax+0x33], 0x80
		test	byte ptr [eax+0x33], 0x20
		jz		done
		mov		ecx, eax
		mov		edx, [ebp+0xC]
		ALIGN 16
	parentIter:
		mov		ecx, [ecx+0x18]
		test	byte ptr [ecx+0x33], 0x20
		jnz		done
		or		byte ptr [ecx+0x33], 0x20
		cmp		ecx, edx
		jnz		parentIter
	done:
		leave
		retn	8
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
		sub		esp, 0xC
		push	esi
		push	edi
		mov		esi, [eax]
		mov		edx, [eax+4]
		lea		eax, [esi+edx*8]
		mov		[ebp-4], eax
		ALIGN 16
	nodeHead:
		cmp		[ebp-4], esi
		jz		nodeEnd
		mov		edi, esi
		add		esi, 8
		mov		eax, [ebp+8]
		mov		edx, [edi]
		test	edx, edx
		jz		gotDest
		cmp		[eax+8], edx
		jz		gotDest
		mov		ecx, eax
		call	NiNode::GetBlockByName
		test	eax, eax
		jz		nodeHead
		mov		edx, [eax]
		cmp		dword ptr [edx+0xC], kAddr_ReturnThis
		jnz		nodeHead
	gotDest:
		mov		[ebp-0xC], eax
		mov		eax, [edi+4]
		mov		edi, [eax]
		mov		edx, [eax+4]
		lea		eax, [edi+edx*8]
		mov		[ebp-8], eax
		ALIGN 16
	insHead:
		cmp		[ebp-8], edi
		jz		nodeHead
		push	dword ptr [ebp+8]
		mov		edx, [edi]
		add		edi, 4
		push	edi
		mov		ecx, [ebp-0xC]
		call	DoAttachModel
		add		edi, 4
		jmp		insHead
		ALIGN 16
	nodeEnd:
		pop		edi
		pop		esi
		leave
	done:
		retn	4
	}
}

bool s_insertObjects = false;

__declspec(naked) void __fastcall DoInsertObjects(TESForm *form1, TESForm *form2, NiNode *rootNode)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		cmp		s_insertNodeMap.numEntries, 0
		jz		doModels
		test	byte ptr [esi+6], kHookFormFlag6_InsertNode
		jz		nodeForm2
		push	dword ptr [esp+0xC]
		call	DoInsertNodes
	nodeForm2:
		test	edi, edi
		jz		doModels
		test	byte ptr [edi+6], kHookFormFlag6_InsertNode
		jz		doModels
		push	dword ptr [esp+0xC]
		mov		ecx, edi
		call	DoInsertNodes
	doModels:
		cmp		s_attachModelMap.numEntries, 0
		jz		done
		test	byte ptr [esi+6], kHookFormFlag6_AttachModel
		jz		modelForm2
		push	dword ptr [esp+0xC]
		mov		ecx, esi
		call	DoAttachModels
	modelForm2:
		test	edi, edi
		jz		done
		test	byte ptr [edi+6], kHookFormFlag6_AttachModel
		jz		done
		push	dword ptr [esp+0xC]
		mov		ecx, edi
		call	DoAttachModels
	done:
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) void CreateObjectNodeHook()
{
	__asm
	{
		test	byte ptr [ecx+0x33], 0x20
		jnz		skipRetn
		mov		ecx, [ebp-0x1A8]
		test	byte ptr [ecx+6], kHookFormFlag6_InsertObject
		jnz		skipRetn
		mov		eax, [ebp+8]
		test	eax, eax
		jz		contRetn
		test	byte ptr [eax+6], kHookFormFlag6_InsertObject
		jnz		skipRetn
		test	byte ptr [eax+0x5F], kHookRefFlag61_DisableCollision
		jnz		skipRetn
	contRetn:
		movzx	eax, byte ptr [ecx+4]
		sub		eax, 0x15
		mov		[ebp-0x1AC], eax
		JMP_EAX(0x50EF66)
	skipRetn:
		JMP_EAX(0x50F0FE)
	}
}

__declspec(naked) void __fastcall DoQueuedReferenceHook(QueuedReference *queuedRef)
{
	__asm
	{
		cmp		dword ptr [ecx+0xC], 6
		jz		done
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, [ecx+0x28]
		mov		eax, fs:[0x2C]
		mov		ecx, ds:[0x126FD98]
		mov		edx, [eax+ecx*4]
		add		edx, 0x2B4
		push	dword ptr [edx]
		mov		eax, [esi+0x18]
		mov		[edx], eax
		test	dword ptr [edi+8], 0x820
		jnz		doErase
		mov		ecx, [edi+0x40]
		test	ecx, ecx
		jz		doErase
		add		ecx, 0x80
		call	LightCS::Enter
		mov		ecx, [edi+0x40]
		cmp		byte ptr [ecx+0x26], 2
		jb		doErase
		mov		eax, [esi+0x30]
		test	eax, eax
		jz		noModel
		push	dword ptr [eax+0xC]
		push	edi
		mov		ecx, g_modelLoader
		CALL_EAX(0x4489B0)
	noModel:
		push	0
		push	esi
		push	dword ptr [edi+0x40]
		push	edi
		mov		ecx, g_TES
		CALL_EAX(0x451EF0)
	doErase:
		push	edi
		mov		eax, g_modelLoader
		mov		ecx, [eax+8]
		CALL_EAX(0x9062E0)
		mov		ecx, [edi+0x40]
		test	ecx, ecx
		jz		popTLS
		lea		edx, [ecx+0xA4]
		lock dec dword ptr [edx]
		jns		doneSmph1
		mov		dword ptr [edx], 0
	doneSmph1:
		mov		eax, [edi]
		cmp		dword ptr [eax+0x100], kAddr_ReturnTrue
		jz		doneSmph2
		sub		edx, 4
		lock dec dword ptr [edx]
		jns		doneSmph2
		mov		dword ptr [edx], 0
	doneSmph2:
		mov		eax, [edi+0x64]
		test	eax, eax
		jz		cellUnlock
		mov		ecx, [eax+0x14]
		test	ecx, ecx
		jz		cellUnlock
		mov		esi, ecx
		test	byte ptr [edi+0x5F], kHookRefFlag61_Update3D
		jz		doneFade
		and		byte ptr [edi+0x5F], ~kHookRefFlag61_Update3D
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x10], kAddr_ReturnThis
		jnz		doneFade
		fld1
		fst		dword ptr [ecx+0xB4]
		fstp	dword ptr [ecx+0xB8]
		or		byte ptr [ecx+0x31], 0x40
	doneFade:
		test	byte ptr [edi+0x5F], kHookRefFlag61_DisableCollision
		jz		doneCollision
		call	NiNode::RemoveCollision
	doneCollision:
		cmp		s_insertObjects, 0
		jz		doLights
		mov		ecx, edi
		call	TESObjectREFR::GetBaseForm2
		test	eax, eax
		jz		doLights
		test	byte ptr [edi+6], kHookFormFlag6_InsertObject
		jnz		hasFlag
		test	byte ptr [eax+6], kHookFormFlag6_InsertObject
		jz		doLights
	hasFlag:
		push	esi
		mov		edx, eax
		mov		ecx, edi
		call	DoInsertObjects
		cmp		dword ptr [edi+0xC], 0x14
		jnz		doLights
		mov		eax, [edi+0x694]
		test	eax, eax
		jz		doLights
		push	eax
		xor		edx, edx
		mov		ecx, edi
		call	DoInsertObjects
	doLights:
		test	byte ptr [esi+0x33], 0x20
		jz		cellUnlock
		mov		ecx, esi
		call	AddPointLights
	cellUnlock:
		mov		ecx, [edi+0x40]
		add		ecx, 0x80
		dec		dword ptr [ecx+4]
		jnz		popTLS
		mov		dword ptr [ecx], 0
	popTLS:
		mov		eax, fs:[0x2C]
		mov		ecx, ds:[0x126FD98]
		mov		edx, [eax+ecx*4]
		pop		dword ptr [edx+0x2B4]
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void LoadBip01SlotHook()
{
	static const char kBlockNameFmt[] = "%s %08X";
	__asm
	{
		push	ecx
		push	dword ptr [ecx+0xC]
		mov		edx, [ebp-0x14]
		mov		eax, ds:0x1188B98[edx*4]
		push	eax
		push	offset kBlockNameFmt
		push	0x18
		lea		edx, [ebp-0x190]
		push	edx
		call	sprintf_s
		add		esp, 0x14
		pop		ecx
		mov		eax, [ebp-0x40]
		mov		edx, [eax]
		cmp		dword ptr [edx+0xC], kAddr_ReturnThis
		jnz		done
		test	byte ptr [ecx+6], kHookFormFlag6_InsertObject
		jz		doLights
		mov		edx, [ebp-0x30]
		mov		edx, [edx+8]
		lock inc dword ptr [edx-8]
		mov		[eax+8], edx
		push	eax
		xor		edx, edx
		call	DoInsertObjects
	doLights:
		cmp		[ebp-0xE], 0
		jnz		done
		mov		ecx, [ebp-0x40]
		test	byte ptr [ecx+0x33], 0x20
		jz		done
		call	AddPointLights
	done:
		lea		edx, [ebp-0x190]
		mov		ecx, [ebp-0x40]
		call	NiObjectNET::SetName
		retn
	}
}

NiNode *s_pc1stPersonNode = NULL;

__declspec(naked) void LoadWeaponSlotHook()
{
	__asm
	{
		mov		eax, [ebp-0x14]
		mov		edx, 0x3F800000
		movd	xmm0, edx
		movups	[eax+0x34], xmm0
		movups	[eax+0x44], xmm0
		movups	[eax+0x54], xmm0
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		done
		test	byte ptr [ecx+6], kHookFormFlag6_InsertObject
		jz		doLights
		push	eax
		xor		edx, edx
		call	DoInsertObjects
	doLights:
		mov		ecx, [ebp-0x14]
		test	byte ptr [ecx+0x33], 0x20
		jz		done
		mov		edx, [ebp+0x18]
		cmp		edx, s_pc1stPersonNode
		jz		done
		call	AddPointLights
	done:
		retn
	}
}

void __fastcall DoUpdateAnimatedLight(TESObjectLIGH *lightForm, NiPointLight *ptLight)
{
	if (lightForm->lightFlags & 0x48)
	{
		float v01 = *(float*)0x11F63A0 * 0.001F;
		if (lightForm->lightFlags & 0x40) v01 *= 0.5F;
		float v02 = ptLight->vectorF0.x + GetRandomIntInRange(1100, 13100) * v01;
		float v03 = ptLight->vectorF0.y + GetRandomIntInRange(1200, 13200) * v01;
		float v04 = ptLight->vectorF0.z + GetRandomIntInRange(1300, 19300) * v01;
		if (v02 > 6.28319F) v02 -= 6.28319F;
		if (v03 > 6.28319F) v03 -= 6.28319F;
		if (v04 > 6.28319F) v04 -= 6.28319F;
		ptLight->vectorF0 = {v02, v03, v04};
		v03 += 0.5F;
		v04 += 0.3F;
		float v05 = sin(v02 * 2.0F + 3.4F);
		float v06 = sin(v03 * 1.7F);
		float v07 = sin(v04 * 2.6F);
		v01 = *(float*)0x11C8FD0 * v05 * v06 * 0.5F;
		v05 *= v01;
		v06 *= v01;
		v07 *= v01;
		if (ptLight->m_flags & 0x20000000)
		{
			ptLight->m_localTranslate.x += v05 - ptLight->ambientColor.r;
			ptLight->m_localTranslate.y += v06 - ptLight->ambientColor.g;
			ptLight->m_localTranslate.z += v07 - ptLight->ambientColor.b;
			ptLight->ambientColor = {v05, v06, v07};
		}
		else ptLight->m_localTranslate = {v05, v06, v07};
		v01 = sin((v02 + 1.7F) * 1.1F) + 1.0F;
		v01 *= (sin(v03 * 1.3F) + 1.0F) * 0.0833333F;
		v01 += sin(v04 * 3.0F) * 0.2F;
		if (v01 > 1.0F) v01 = 1.0F;
		else if (v01 < -1.0F) v01 = -1.0F;
		ptLight->fadeValue = (v01 * 0.2F + 0.8F) * lightForm->fadeValue;
	}
	else
	{
		float v08 = lightForm->fadeValue;
		float v09 = ptLight->fadeValue;
		float v10 = (lightForm->lightFlags & 0x100) ? 0.05F : 0.1F;
		v10 *= *(float*)0x11F63A0 * v08;
		if (v09 >= v08) ptLight->extraFlags |= 1;
		else if (v09 <= (v08 * 0.25F)) ptLight->extraFlags &= ~1;
		if (ptLight->extraFlags & 1) v09 -= v10;
		else v09 += v10;
		ptLight->fadeValue = v09;
	}
}

__declspec(naked) void __fastcall UpdateAnimatedLightHook(TESObjectLIGH *lightForm, int EDX, ExtraLight::Data *xLightData, int arg2)
{
	__asm
	{
		test	dword ptr [ecx+0xA8], 0x1C8
		jz		done
		mov		edx, [esp+4]
		mov		edx, [edx]
		call	DoUpdateAnimatedLight
	done:
		retn	8
	}
}

__declspec(naked) void __fastcall UpdateAnimatedLightsHook(TES *pTES)
{
	__asm
	{
		mov		dword ptr ds:[0x11C56E8], 0
		mov		eax, [ecx+0x34]
		test	eax, eax
		jz		isExterior
		mov		ecx, eax
		CALL_EAX(0x553820)
		jmp		doneCells
	isExterior:
		mov		ecx, [ecx+8]
		CALL_EAX(0x4BABA0)
	doneCells:
		mov		eax, offset s_activePtLights
		mov		ecx, [eax+4]
		test	ecx, ecx
		jz		done
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebp, g_shadowSceneNode
		mov		ebx, eax
		mov		eax, [eax]
		lea		esi, [eax+ecx*4]
		mov		edi, ecx
		mov		ecx, 0x11F9EA0
		call	LightCS::Enter
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		sub		esi, 4
		mov		ecx, [esi]
		cmp		dword ptr [ecx+0x18], 0
		jz		doFree
		test	byte ptr [ecx+0x30], 1
		jnz		iterHead
		mov		edx, [ecx+0xE8]
		cmp		[ecx+0x9E], 0
		jz		unmodified
		mov		[ecx+0x9E], 0
		movups	xmm0, [ecx+0x58]
		movups	xmm1, [ecx+0xC8]
		subps	xmm0, xmm1
		movups	[ecx+0x58], xmm0
		call	SetLightProperties
	unmodified:
		cmp		dword ptr [ecx+4], 1
		jz		doAdd
		test	dword ptr [edx+0xA8], 0x1C8
		jz		iterHead
		xchg	ecx, edx
		call	DoUpdateAnimatedLight
		jmp		iterHead
		ALIGN 16
	doFree:
		cmp		dword ptr [ecx+4], 0
		jnz		iterHead
		push	0xFC
		push	ecx
		CALL_EAX(0xAA1460)
		add		esp, 8
		mov		eax, [ebx+4]
		dec		eax
		mov		[ebx+4], eax
		sub		eax, edi
		jz		iterHead
		shl		eax, 2
		push	eax
		lea		edx, [esi+4]
		push	edx
		push	esi
		call	MemCopy
		add		esp, 0xC
		jmp		iterHead
		ALIGN 16
	doAdd:
		push	0x250
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xB9FDA0)
		mov		word ptr [eax+0xF4], 1
		mov		edx, [esi]
		lock inc dword ptr [edx+4]
		mov		[eax+0xF8], edx
		mov		[eax+0xFC], 1
		mov		edx, [ebp+0x1E0]
		mov		[eax+0x240], edx
		push	eax
		mov		ecx, ebp
		CALL_EAX(0xB5ECA0)
		jmp		iterHead
		ALIGN 16
	iterEnd:
		mov		ecx, 0x11F9EA0
		dec		dword ptr [ecx+4]
		jnz		inUse
		mov		dword ptr [ecx], 0
	inUse:
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
	done:
		retn
	}
}

TESObjectREFR *s_syncPositionRef = NULL;
NiString s_syncPositionNode;
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
		cmp		dword ptr [ecx+0xC], 0x14
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
		mov		eax, [eax+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		mov		edx, s_syncPositionNode.str
		test	edx, edx
		jz		useRoot
		cmp		[eax+8], edx
		jz		useRoot
		mov		ecx, eax
		call	NiNode::GetBlockByName
		test	eax, eax
		jz		done
	useRoot:
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
		mov		ecx, [ebp-0x44]
		fstp	dword ptr [ecx+0x2C]
	done:
		retn
	}
}

void HandleFramePreRender()
{
	
}

void __fastcall DoRenderFrameHook(OSGlobals *osGlobals, int EDX, NiObject *renderer, UInt8 arg2, UInt8 arg3)
{
	__asm
	{
		push	ecx
		cmp		dword ptr [esp+8], 0
		jnz		skipHandle
		call	HandleFramePreRender
	skipHandle:
		mov		eax, ds:[0x11F91C8]
		mov		ecx, ds:[0x11DEB7C]
		mov		ecx, [ecx+0xAC]
		mov		edx, [ecx+0x8C]
		mov		[eax+0x1F0], edx
		mov		edx, [ecx+0x90]
		mov		[eax+0x1F4], edx
		mov		edx, [ecx+0x94]
		mov		[eax+0x1F8], edx
		mov		dl, [eax+0x131]
		cmp		[esp+0xC], 0
		jz		notMenu
		cmp		byte ptr ds:[0x11F3484], 0
		jnz		notMenu
		mov		[esp+0x10], dl
		mov		eax, 0x8707C0
		jmp		doCall
	notMenu:
		mov		eax, 0x870A00
		mov		ecx, 0x870BD0
		test	dl, dl
		cmovz	eax, ecx
	doCall:
		pop		ecx
		push	dword ptr [esp+0xC]
		push	dword ptr [esp+8]
		call	eax
		retn	0xC
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
		and		byte ptr [eax+4], 0xF7
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

UInt32 s_mouseMovementState = 7;

__declspec(naked) int __fastcall GetMouseMovementHook(OSInputGlobals *inputGlobals, int EDX, UInt32 type)
{
	__asm
	{
		mov		edx, [esp+4]
		cmp		edx, 3
		ja		retn0
		jz		getWheel
		mov		eax, s_mouseMovementState
		dec		edx
		bt		eax, edx
		jnc		retn0
		mov		eax, [ecx+edx*4+0x1B24]
		retn	4
	getWheel:
		mov		eax, [ecx+0x1B2C]
		test	eax, eax
		jz		done
		mov		ecx, g_DIHookCtrl
		jns		wheelUp
		cmp		word ptr [ecx+0x748], 0
		jnz		retn0
		retn	4
	wheelUp:
		cmp		word ptr [ecx+0x741], 0
		jnz		retn0
		retn	4
	retn0:
		xor		eax, eax
	done:
		retn	4
	}
}

__declspec(naked) void __fastcall ClearHUDOrphanedTiles(HUDMainMenu *hudMain)
{
	__asm
	{
		push	esi
		push	ecx
		mov		eax, [ecx+4]
		mov		esi, [eax+4]
		ALIGN 16
	iterHead:
		test	esi, esi
		jz		done
		mov		ecx, [esi+8]
		mov		esi, [esi]
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx+0x2C]
		test	eax, eax
		jz		iterHead
		cmp		dword ptr [eax+0x18], 0
		jnz		iterHead
		push	1
		mov		eax, [ecx]
		call	dword ptr [eax]
		jmp		iterHead
		ALIGN 16
	done:
		pop		ecx
		CALL_EAX(0x77DA60)
		pop		esi
		retn
	}
}

Set<UInt32> s_internalMarkerIDs({1, 2, 3, 4, 5, 6, 0x10, 0x12, 0x15, 0x23, 0x24, 0x32, 0x33, 0x34, 0x3B, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77});

__declspec(naked) bool __fastcall GetIsInternalMarkerHook(TESForm *form)
{
	__asm
	{
		mov		al, [ecx+4]
		cmp		al, kFormType_BGSIdleMarker
		ja		retn0
		jz		retn1
		cmp		al, kFormType_BGSTextureSet
		jz		retn1
		push	ecx
		push	dword ptr [ecx+0xC]
		mov		ecx, offset s_internalMarkerIDs
		call	Set<UInt32>::HasKey
		pop		ecx
		test	al, al
		jnz		done
		mov		al, [ecx+4]
		cmp		al, kFormType_TESObjectACTI
		jnz		notACTI
		mov		ecx, [ecx+0x40]
		test	ecx, ecx
		jz		retn0
		mov		edx, 0x1029C94
		call	StrCompare
		test	al, al
		setz	al
		retn
	notACTI:
		cmp		al, kFormType_TESObjectLIGH
		jnz		retn0
		cmp		word ptr [ecx+0x44], 0
		setz	al
		retn
	retn0:
		xor		al, al
		retn
	retn1:
		mov		al, 1
	done:
		retn
	}
}

UnorderedMap<const char*, UInt32> s_eventMasks({{"OnActivate", 0}, {"OnAdd", 1}, {"OnEquip", 2}, {"OnActorEquip", 2}, {"OnDrop", 4}, {"OnUnequip", 8}, {"OnActorUnequip", 8},
	{"OnDeath", 0x10}, {"OnMurder", 0x20}, {"OnCombatEnd", 0x40}, {"OnHit", 0x80}, {"OnHitWith", 0x100}, {"OnPackageStart", 0x200}, {"OnPackageDone", 0x400},
	{"OnPackageChange", 0x800}, {"OnLoad", 0x1000}, {"OnMagicEffectHit", 0x2000}, {"OnSell", 0x4000}, {"OnStartCombat", 0x8000}, {"OnOpen", 0x10000}, {"OnClose", 0x20000},
	{"SayToDone", 0x40000}, {"OnGrab", 0x80000}, {"OnRelease", 0x100000}, {"OnDestructionStageChange", 0x200000}, {"OnFire", 0x400000}, {"OnTrigger", 0x10000000},
	{"OnTriggerEnter", 0x20000000}, {"OnTriggerLeave", 0x40000000}, {"OnReset", 0x80000000}});

void InitJIPHooks()
{
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
	HOOK_INIT_JPRT(MenuHandleMouseover, 0x70D642, 0x70D658);
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
	HOOK_INIT_JPRT(SynchronizePosition, 0x575836, 0x575845);

	SetFormEDID = (_SetFormEDID)*(UInt32*)0x1029018;
	SafeWrite32(0x1029018, (UInt32)TESObjectLIGHSetEDIDHook);
	SAFE_WRITE_BUF(0xA68D6B, "\x8B\x86\x9C\x00\x00\x00\x89\x87\x9C\x00\x00\x00");
	WritePushRetRelJump(0x447155, 0x447171, (UInt32)LoadNifRetnNodeHook);
	SafeWrite32(0x109DD0C, (UInt32)DestroyNiPointLightHook);
	WriteRelJump(0x50EF46, (UInt32)CreateObjectNodeHook);
	SafeWrite32(0x1016BE0, (UInt32)DoQueuedReferenceHook);
	SafeWrite32(0x1016D28, (UInt32)DoQueuedReferenceHook);
	SafeWrite32(0x1016D70, (UInt32)DoQueuedReferenceHook);
	WritePushRetRelJump(0x4ACCF1, 0x4ACD8C, (UInt32)LoadBip01SlotHook);
	WritePushRetRelJump(0x4AF0DE, 0x4AF0F8, (UInt32)LoadWeaponSlotHook);
	//SAFE_WRITE_BUF(0x50DCE7, "\x8B\x4D\xD4\x83\xC1\x7C\x8B\x82\xB4\x00\x00\x00\x89\x41\x48\x89\x51\x6C");
	WriteRelJump(0x50DE20, (UInt32)UpdateAnimatedLightHook);
	WriteRelCall(0x8C7C4E, (UInt32)UpdateAnimatedLightsHook);
	WriteRelCall(0x8C7E18, (UInt32)UpdateAnimatedLightsHook);
	//WriteRelJump(0x8706B0, (UInt32)DoRenderFrameHook);
	SafeWrite32(0x1087FD8, (UInt32)CopyHitDataHook);
	SafeWrite32(0x10897C0, (UInt32)CopyHitDataHook);
	SAFE_WRITE_BUF(0x92C4A0, "\x8B\x81\x40\x02\x00\x00\x85\xC0\x74\x07\xC7\x40\x10\xFF\xFF\xFF\xFF\xC3");
	WriteRelJump(0x418B10, (UInt32)GetCannotWearHook);
	WriteRelJump(0x4AC645, (UInt32)BipedSlotVisibilityHook);
	WriteRelJump(0x63C929, (UInt32)SkyRefreshClimateHook);
	WriteRelJump(0x87F427, (UInt32)IsActorEssentialHook);
	WritePushRetRelJump(0x524014, 0x524042, (UInt32)FireWeaponWobbleHook);
	WriteRelJump(0xA239E0, (UInt32)GetMouseMovementHook);
	WriteRelCall(0x7704AE, (UInt32)ClearHUDOrphanedTiles);
	WriteRelJump(0x50F9E0, (UInt32)GetIsInternalMarkerHook);

	PrintLog("> JIP hooks initialized successfully.");
}