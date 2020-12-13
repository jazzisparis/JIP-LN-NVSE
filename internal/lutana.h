#pragma once

enum
{
	kLNEventID_OnCellEnter,
	kLNEventID_OnCellExit,
	kLNEventID_OnPlayerGrab,
	kLNEventID_OnPlayerRelease,
	kLNEventID_OnCrosshairOn,
	kLNEventID_OnCrosshairOff,
	kLNEventID_OnButtonDown,
	kLNEventID_OnButtonUp,
	kLNEventID_OnKeyDown,
	kLNEventID_OnKeyUp,
	kLNEventID_OnControlDown,
	kLNEventID_OnControlUp,
	kLNEventID_OnTriggerDown,
	kLNEventID_OnTriggerUp,
	kLNEventID_Max =					kLNEventID_OnKeyDown,

	kLNEventMask_OnCellEnter =			1 << kLNEventID_OnCellEnter,
	kLNEventMask_OnCellExit =			1 << kLNEventID_OnCellExit,
	kLNEventMask_OnCell =				kLNEventMask_OnCellEnter | kLNEventMask_OnCellExit,
	kLNEventMask_OnPlayerGrab =			1 << kLNEventID_OnPlayerGrab,
	kLNEventMask_OnPlayerRelease =		1 << kLNEventID_OnPlayerRelease,
	kLNEventMask_OnPlayerGR =			kLNEventMask_OnPlayerGrab | kLNEventMask_OnPlayerRelease,
	kLNEventMask_OnCrosshairOn =		1 << kLNEventID_OnCrosshairOn,
	kLNEventMask_OnCrosshairOff =		1 << kLNEventID_OnCrosshairOff,
	kLNEventMask_OnCrosshair =			kLNEventMask_OnCrosshairOn | kLNEventMask_OnCrosshairOff,
	kLNEventMask_OnButtonDown =			1 << kLNEventID_OnButtonDown,
	kLNEventMask_OnButtonUp =			1 << kLNEventID_OnButtonUp,
	kLNEventMask_OnButton =				kLNEventMask_OnButtonDown | kLNEventMask_OnButtonUp,
	kLNEventMask_OnKeyDown =			1 << kLNEventID_OnKeyDown,
	kLNEventMask_OnKeyUp =				1 << kLNEventID_OnKeyUp,
	kLNEventMask_OnKey =				kLNEventMask_OnKeyDown | kLNEventMask_OnKeyUp,
	kLNEventMask_OnControlDown =		1 << kLNEventID_OnControlDown,
	kLNEventMask_OnControlUp =			1 << kLNEventID_OnControlUp,
	kLNEventMask_OnControl =			kLNEventMask_OnControlDown | kLNEventMask_OnControlUp,
	kLNEventMask_OnTriggerDown =		1 << kLNEventID_OnTriggerDown,
	kLNEventMask_OnTriggerUp =			1 << kLNEventID_OnTriggerUp,
	kLNEventMask_OnTrigger =			kLNEventMask_OnTriggerDown | kLNEventMask_OnTriggerUp,
};

struct alignas(16) XINPUT_GAMEPAD_EX
{
	UInt32		eventCount;		// 00
	UInt16		wButtons;		// 04
	UInt8		bLeftTrigger;	// 06
	UInt8		bRightTrigger;	// 07
	SInt16		sThumbLX;		// 08
	SInt16		sThumbLY;		// 0A
	SInt16		sThumbRX;		// 0C
	SInt16		sThumbRY;		// 0E
}
s_gamePad = {};

bool s_controllerReady = false;

UInt32 (__stdcall *XInputGetStateEx)(UInt32, XINPUT_GAMEPAD_EX*);

UInt16 s_deadZoneLS = 0x1EA9, s_deadZoneRS = 0x21F1;
double s_deadZoneLSd = 24918.0, s_deadZoneRSd = 24078.0;

struct XInputStateMods
{
	UInt16		buttonSkip;		// 00
	UInt16		buttonHold;		// 02
	UInt16		buttonTap;		// 04
	UInt8		triggerMods;	// 06
	UInt8		stickMods;		// 07
}
s_XIStateMods = {0xFFFF, 0, 0, 0, 0};

__declspec(naked) UInt32 __stdcall Hook_XInputGetState(UInt32 index, XINPUT_GAMEPAD_EX *currState)
{
	static UInt32 result = ERROR_DEVICE_NOT_CONNECTED;
	__asm
	{
		mov		eax, [esp+8]
		mov		ecx, 0x11F35A8
		cmp		eax, ecx
		jz		updateState
		movdqu	xmm0, xmmword ptr [ecx]
		movdqu	xmmword ptr [eax], xmm0
		jmp		done
	updateState:
		push	ecx
		push	dword ptr [esp+8]
		call	XInputGetStateEx
		mov		result, eax
		movzx	edx, byte ptr ds:[0x11D8C50]
		or		eax, edx
		setz	al
		mov		s_controllerReady, al
		jnz		done
		push	esi
		mov		esi, 0x11F35A8
		movdqu	xmm0, xmmword ptr [esi]
		movdqa	xmmword ptr ds:[s_gamePad], xmm0
		mov		ecx, offset s_XIStateMods
		mov		ax, [esi+4]
		or		ax, word ptr [ecx+2]
		and		ax, word ptr [ecx]
		or		ax, word ptr [ecx+4]
		mov		word ptr [ecx+4], 0
		mov		[esi+4], ax
		mov		al, [ecx+6]
		test	al, al
		jz		doneTRmods
		mov		dl, al
		and		dl, 0x11
		cmp		dl, 1
		jnz		testLThold
		mov		[esi+6], 0
		jmp		testRTskip
	testLThold:
		test	al, 0x14
		jz		testRTskip
		mov		byte ptr [esi+6], 0xFF
	testRTskip:
		mov		dl, al
		and		dl, 0x22
		cmp		dl, 2
		jnz		testRThold
		mov		[esi+7], 0
		jmp		clrTRtap
	testRThold:
		test	al, 0x28
		jz		clrTRtap
		mov		byte ptr [esi+7], 0xFF
	clrTRtap:
		and		al, 0xF
		mov		[ecx+6], al
	doneTRmods:
		mov		al, [ecx+7]
		test	al, 1
		jz		rightStick
		mov		dword ptr [esi+8], 0
	rightStick:
		test	al, 2
		jz		doneSticks
		mov		dword ptr [esi+0xC], 0
	doneSticks:
		pop		esi
	done:
		mov		eax, result
		retn	8
	}
}

bool HookXInput()
{
	HMODULE xinput = GetModuleHandle("XINPUT1_3");
	if (xinput)
	{
		XInputGetStateEx = (UInt32 (__stdcall *)(UInt32, XINPUT_GAMEPAD_EX*))GetProcAddress(xinput, (LPCSTR)0x64);
		if (XInputGetStateEx)
		{
			WriteRelJump(0x9F996E, (UInt32)Hook_XInputGetState);
			return true;
		}
		PrintLog("ERROR: XInputGetStateEx not found.");
	}
	else PrintLog("ERROR: XInput module not found.");
	return false;
}

struct LNEventData
{
	UInt8				eventID;
	UInt8				filterType;
	bool				remove;
	UInt8				byte03;
	Script				*script;
	union
	{
		TESForm			*form;
		tList<TESForm>	*list;
		UInt32			typeID;
	};

	LNEventData(UInt8 _eventID, Script *_script, bool _remove, UInt8 _filterType = 0, UInt32 _typeID = 0) :
		eventID(_eventID), filterType(_filterType), remove(_remove), script(_script), typeID(_typeID) {}
	LNEventData(const LNEventData &copyFrom) {*this = copyFrom;}

	bool EvalFilter();
};

class LNEventFinder
{
	LNEventData		*data;
	
public:
	LNEventFinder(LNEventData &evntData) : data(&evntData) {}

	bool operator()(const LNEventData &rhs)
	{
		if (data->script != rhs.script)
			return false;
		if (data->remove && !rhs.filterType)
			return true;
		if (data->filterType != rhs.filterType)
			return false;
		if ((data->eventID < kLNEventID_OnButtonDown) || data->remove)
			return data->typeID == rhs.typeID;
		return (data->typeID & rhs.typeID) != 0;
	}
};

TESObjectREFR *s_evalRefr;
TESForm *s_evalBase;

bool LNEventData::EvalFilter()
{
	switch (filterType)
	{
		case 0: return true;
		case 1: return form == s_evalRefr;
		case 2: return form == s_evalBase;
		case 3:
		{
			TESForm *form;
			ListNode<TESForm> *iter = list->Head();
			do
			{
				form = iter->data;
				if ((form == s_evalRefr) || (form == s_evalBase))
					return true;
			}
			while (iter = iter->next);
			return false;
		}
		case 4: return typeID == s_evalBase->typeID;
		default: return false;
	}
}

struct LNDInpuCallbacks
{
	EventCallbackScripts	onDown;
	EventCallbackScripts	onUp;

	bool Empty() {return onDown.Empty() && onUp.Empty();}
};

typedef Vector<LNEventData> LNEventCallbacks;
LNEventCallbacks s_LNEvents[kLNEventID_Max];

typedef Map<UInt32, LNDInpuCallbacks> LNDInputEventsMap;
LNDInputEventsMap s_LNOnKeyEvents, s_LNOnControlEvents;

EventCallbackScripts s_LNOnTriggerEvents[4];

UInt32 s_LNEventFlags = 0;

bool __fastcall GetXIControlPressed(UInt32 ctrlID, UInt32 flags)
{
	XINPUT_GAMEPAD_EX *xiState = (flags & 2) ? &s_gamePad : (XINPUT_GAMEPAD_EX*)0x11F35A8;
	switch (ctrlID)
	{
		case kXboxCtrl_DPAD_UP:
			return (xiState->wButtons & 1) != 0;
		case kXboxCtrl_DPAD_DOWN:
			return (xiState->wButtons & 2) != 0;
		case kXboxCtrl_DPAD_RIGHT:
			return (xiState->wButtons & 8) != 0;
		case kXboxCtrl_DPAD_LEFT:
			return (xiState->wButtons & 4) != 0;
		case kXboxCtrl_START:
			return (xiState->wButtons & 0x10) != 0;
		case kXboxCtrl_BACK:
			return (xiState->wButtons & 0x20) != 0;
		case kXboxCtrl_LS_BUTTON:
			return (xiState->wButtons & 0x40) != 0;
		case kXboxCtrl_RS_BUTTON:
			return (xiState->wButtons & 0x80) != 0;
		case kXboxCtrl_BUTTON_A:
			return (xiState->wButtons & 0x1000) != 0;
		case kXboxCtrl_BUTTON_B:
			return (xiState->wButtons & 0x2000) != 0;
		case kXboxCtrl_BUTTON_X:
			return (xiState->wButtons & 0x4000) != 0;
		case kXboxCtrl_BUTTON_Y:
			return (xiState->wButtons & 0x8000) != 0;
		case kXboxCtrl_RB:
			return (xiState->wButtons & 0x200) != 0;
		case kXboxCtrl_LB:
			return (xiState->wButtons & 0x100) != 0;
		case kXboxCtrl_LT:
			return xiState->bLeftTrigger != 0;
		case kXboxCtrl_RT:
			return xiState->bRightTrigger != 0;
		case kXboxCtrl_LS_UP:
			return xiState->sThumbLY > s_deadZoneLS;
		case kXboxCtrl_LS_DOWN:
			return xiState->sThumbLY < -s_deadZoneLS;
		case kXboxCtrl_LS_RIGHT:
			return xiState->sThumbLX > s_deadZoneLS;
		case kXboxCtrl_LS_LEFT:
			return xiState->sThumbLX < -s_deadZoneLS;
		default:
			return false;
	}
}

bool Hook_IsKeyPressed_Execute(COMMAND_ARGS)
{
	UInt32 keyID, flags = 1;
	if (ExtractArgs(EXTRACT_ARGS, &keyID, &flags))
		*result = g_DIHookCtrl->IsKeyPressed(keyID, flags);
	else *result = 0;
	return true;
}

bool Hook_IsKeyPressed_Eval(TESObjectREFR *thisObj, UInt32 keyID, UInt32 flags, double *result)
{
	*result = g_DIHookCtrl->IsKeyPressed(keyID, flags);
	return true;
}

bool __fastcall IsControlPressed(UInt32 ctrlID, UInt32 flags = 1)
{
	if (s_controllerReady)
		return GetXIControlPressed(g_inputGlobals->controllerBinds[ctrlID], flags);
	UInt32 keyID = g_inputGlobals->keyBinds[ctrlID];
	if ((keyID != 0xFF) && g_DIHookCtrl->IsKeyPressed(keyID, flags))
		return true;
	else
	{
		keyID = g_inputGlobals->mouseBinds[ctrlID];
		return (keyID != 0xFF) && g_DIHookCtrl->IsKeyPressed(keyID + 0x100, flags);
	}
}

bool __fastcall IsControlPressedRaw(UInt32 ctrlID)
{
	if (s_controllerReady)
		return GetXIControlPressed(g_inputGlobals->controllerBinds[ctrlID], 2);
	UInt32 keyID = g_inputGlobals->keyBinds[ctrlID];
	if ((keyID != 0xFF) && g_DIHookCtrl->IsKeyPressedRaw(keyID))
		return true;
	else
	{
		keyID = g_inputGlobals->mouseBinds[ctrlID];
		return (keyID != 0xFF) && g_DIHookCtrl->IsKeyPressedRaw(keyID + 0x100);
	}
}

bool Hook_IsControlPressed_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID, flags = 1;
	if (ExtractArgs(EXTRACT_ARGS, &ctrlID, &flags) && (ctrlID < kMaxControlBinds))
		*result = IsControlPressed(ctrlID, flags);
	else *result = 0;
	return true;
}

bool Hook_IsControlPressed_Eval(TESObjectREFR *thisObj, UInt32 ctrlID, UInt32 flags, double *result)
{
	if (ctrlID < kMaxControlBinds)
		*result = IsControlPressed(ctrlID, flags);
	else *result = 0;
	return true;
}

UnorderedMap<const char*, UInt32> s_LNEventNames(0x10);

Cmd_Execute SetEventHandler, RemoveEventHandler;

const bool kValidKeyCode[] =
{
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

const bool kValidFilterForm[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
};

UInt32 s_DInputEventClear = 0;

bool SetDInputEventHandler(UInt32 eventMask, Script *script, SInt32 keyID, bool doAdd)
{
	bool onKey = (eventMask & kLNEventMask_OnKey) ? true : false;
	bool onUp = (eventMask == kLNEventMask_OnKeyUp) || (eventMask == kLNEventMask_OnControlUp);
	eventMask = onKey ? kLNEventMask_OnKey : kLNEventMask_OnControl;
	LNDInputEventsMap &events = onKey ? s_LNOnKeyEvents : s_LNOnControlEvents;
	bool result = false;
	if (keyID < 0)
	{
		if (doAdd) return false;
		for (auto iter = events.Begin(); iter; ++iter)
		{
			result |= onUp ? iter().onUp.Erase(script) : iter().onDown.Erase(script);
			if (iter().Empty())
				s_DInputEventClear |= eventMask;
		}
		return result;
	}
	else if (onKey)
	{
		if ((keyID >= sizeof(kValidKeyCode)) || !kValidKeyCode[keyID])
			return false;
	}
	else if (keyID > 27)
		return false;
	LNDInpuCallbacks *callbacks;
	if (doAdd)
	{
		if (events.Insert(keyID, &callbacks)) s_LNEventFlags |= eventMask;
		result = onUp ? callbacks->onUp.Insert(script) : callbacks->onDown.Insert(script);
	}
	else
	{
		callbacks = events.GetPtr(keyID);
		if (callbacks)
		{
			result = onUp ? callbacks->onUp.Erase(script) : callbacks->onDown.Erase(script);
			if (callbacks->Empty())
				s_DInputEventClear |= eventMask;
		}
	}
	return result;
}

bool s_processEventAdd = false;

bool ProcessEventHandler(COMMAND_ARGS)
{
	*result = 0;
	if (!scriptObj) return true;
	UInt8 *data = (UInt8*)scriptData + *opcodeOffsetPtr;
	UInt8 numArgs = *(UInt8*)data;
	data += 4;
	UInt16 argLen = *(UInt16*)data;
	data += 2;
	char evnName[0x40];
	evnName[argLen] = 0;
	memcpy(evnName, data, argLen);
	data += argLen + 3;
	Script::RefVariable *scrVar = scriptObj->GetVariable(*(UInt16*)data);
	if (!scrVar) return true;
	Script *script = (Script*)scrVar->form;
	if (!script || NOT_TYPE(script, Script)) return true;

	TESForm *formFilter = NULL;
	if (numArgs > 2)
	{
		data += 5;
		argLen = *(UInt16*)data;
		data += argLen + 3;
		scrVar = scriptObj->GetVariable(*(UInt16*)data);
		if (scrVar) formFilter = scrVar->form;
	}

	char *delim = GetNextToken(evnName, ':');
	UInt32 eventMask = s_LNEventNames.Get(evnName);

	bool doAdd = s_processEventAdd;

	if (!eventMask) return doAdd ? SetEventHandler(PASS_COMMAND_ARGS) : RemoveEventHandler(PASS_COMMAND_ARGS);

	if (eventMask >= kLNEventMask_OnKeyDown)
	{
		if (SetDInputEventHandler(eventMask, script, *delim ? StrToInt(delim) : -1, doAdd))
			*result = 1;
		return true;
	}

	UInt8 eventID = 0;
	while (!((eventMask >> eventID) & 1)) eventID++;

	LNEventData evntData(eventID, script, !doAdd);

	if (formFilter)
	{
		if IS_TYPE(formFilter, BGSListForm)
		{
			evntData.list = &((BGSListForm*)formFilter)->list;
			if (evntData.list->Empty()) return true;
			evntData.filterType = 3;
		}
		else if (eventID <= kLNEventID_OnCellExit)
		{
			if NOT_TYPE(formFilter, TESObjectCELL) return true;
			evntData.filterType = 2;
			evntData.form = formFilter;
		}
		else if (formFilter->GetIsReference())
		{
			evntData.filterType = 1;
			evntData.form = formFilter;
		}
		else
		{
			if (!kValidFilterForm[formFilter->typeID]) return true;
			evntData.filterType = 2;
			evntData.form = formFilter;
		}
	}
	else if (eventID >= kLNEventID_OnCrosshairOn)
	{
		if (*delim)
		{
			evntData.typeID = StrToInt(delim);
			if (eventID <= kLNEventID_OnCrosshairOff)
			{
				if ((evntData.typeID > kFormType_TESCaravanMoney) || !kValidFilterForm[evntData.typeID]) return true;
			}
			else if (!s_controllerReady || (evntData.typeID > 0xFFFF)) return true;
			evntData.filterType = 4;
		}
		else if ((eventID >= kLNEventID_OnButtonDown) && doAdd)
			return true;
	}

	LNEventCallbacks &callbacks = s_LNEvents[eventID];
	if (doAdd)
	{
		if (callbacks.Find(LNEventFinder(evntData))) return true;
		callbacks.Append(evntData);
		s_LNEventFlags |= eventMask;
		*result = 1;
	}
	else if (callbacks.Remove(LNEventFinder(evntData)))
	{
		if (callbacks.Empty()) s_LNEventFlags ^= eventMask;
		*result = 1;
	}
	return true;
}

__declspec(naked) bool Hook_SetEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_processEventAdd, 1
		jmp		ProcessEventHandler
	}
}

__declspec(naked) bool Hook_RemoveEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_processEventAdd, 0
		jmp		ProcessEventHandler
	}
}

bool s_gameLoadFlagLN = true;

void LN_ProcessEvents()
{
	static bool lastKeyState[kMaxMacros] = {0};

	if (s_LNEventFlags & kLNEventMask_OnKey)
	{
		if (s_DInputEventClear & kLNEventMask_OnKey)
		{
			s_DInputEventClear ^= kLNEventMask_OnKey;
			for (auto onKeyClr = s_LNOnKeyEvents.BeginOp(); onKeyClr; ++onKeyClr)
				if (onKeyClr().Empty()) onKeyClr.Remove(s_LNOnKeyEvents);
			if (s_LNOnKeyEvents.Empty())
				s_LNEventFlags ^= kLNEventMask_OnKey;
		}
		UInt32 key;
		bool currKeyState;
		for (auto onKey = s_LNOnKeyEvents.BeginCp(); onKey; ++onKey)
		{
			key = onKey.Key();
			currKeyState = g_DIHookCtrl->IsKeyPressedRaw(key);
			if (lastKeyState[key] == currKeyState) continue;
			if (currKeyState) onKey().onDown.InvokeEvents(key);
			else onKey().onUp.InvokeEvents(key);
			lastKeyState[key] = currKeyState;
		}
	}

	if (s_LNEventFlags & kLNEventMask_OnButton)
	{
		static UInt16 lastButtonState = 0;
		UInt16 currButtonState = s_gamePad.wButtons, changes = currButtonState ^ lastButtonState;
		if (changes)
		{
			UInt16 cmprMask = changes & currButtonState;
			UInt32 outMask;
			if (cmprMask)
			{
				for (auto data = s_LNEvents[kLNEventID_OnButtonDown].BeginCp(); data; ++data)
				{
					outMask = cmprMask & data().typeID;
					if (outMask) CallFunction(data().script, NULL, NULL, &s_callRes, 1, outMask);
				}
			}
			cmprMask = changes & lastButtonState;
			if (cmprMask)
			{
				for (auto data = s_LNEvents[kLNEventID_OnButtonUp].BeginCp(); data; ++data)
				{
					outMask = cmprMask & data().typeID;
					if (outMask) CallFunction(data().script, NULL, NULL, &s_callRes, 1, outMask);
				}
			}
			lastButtonState = currButtonState;
		}
	}

	if (s_LNEventFlags & kLNEventMask_OnTrigger)
	{
		static bool lastLTriggerState = 0, lastRTriggerState = 0;
		bool currTriggerState = s_gamePad.bLeftTrigger != 0;
		if (lastLTriggerState != currTriggerState)
		{
			s_LNOnTriggerEvents[currTriggerState].InvokeEvents(0);
			lastLTriggerState = currTriggerState;
		}
		currTriggerState = s_gamePad.bRightTrigger != 0;
		if (lastRTriggerState != currTriggerState)
		{
			s_LNOnTriggerEvents[currTriggerState + 2].InvokeEvents(1);
			lastRTriggerState = currTriggerState;
		}
	}

	if (g_interfaceManager->currentMode > 1) return;

	static TESObjectCELL *lastCell = NULL;
	TESObjectCELL *currCell = g_thePlayer->parentCell;
	if (!currCell) return;

	if (lastCell != currCell)
	{
		if (!s_gameLoadFlagLN)
		{
			if (s_LNEventFlags & kLNEventMask_OnCellExit)
			{
				s_evalRefr = NULL;
				s_evalBase = lastCell;
				for (auto data = s_LNEvents[kLNEventID_OnCellExit].BeginCp(); data; ++data)
					if (data().EvalFilter()) CallFunction(data().script, NULL, NULL, &s_callRes, 1, lastCell);
			}
			if (s_LNEventFlags & kLNEventMask_OnCellEnter)
			{
				s_evalRefr = NULL;
				s_evalBase = currCell;
				for (auto data = s_LNEvents[kLNEventID_OnCellEnter].BeginCp(); data; ++data)
					if (data().EvalFilter()) CallFunction(data().script, NULL, NULL, &s_callRes, 1, currCell);
			}
		}
		lastCell = currCell;
	}

	static TESObjectREFR *lastGrabbed = NULL;
	static UInt32 lastGrabID = 0;
	TESObjectREFR *currGrabbed = g_thePlayer->grabbedRef;

	if (lastGrabbed != currGrabbed)
	{
		if (!s_gameLoadFlagLN)
		{
			if (lastGrabbed && (s_LNEventFlags & kLNEventMask_OnPlayerRelease))
			{
				if (lastGrabbed == LookupFormByRefID(lastGrabID))
				{
					s_evalRefr = lastGrabbed;
					s_evalBase = lastGrabbed->baseForm;
					for (auto data = s_LNEvents[kLNEventID_OnPlayerRelease].BeginCp(); data; ++data)
						if (data().EvalFilter()) CallFunction(data().script, NULL, NULL, &s_callRes, 1, lastGrabbed);
				}
			}
			if (currGrabbed && (s_LNEventFlags & kLNEventMask_OnPlayerGrab))
			{
				s_evalRefr = currGrabbed;
				s_evalBase = currGrabbed->baseForm;
				for (auto data = s_LNEvents[kLNEventID_OnPlayerGrab].BeginCp(); data; ++data)
					if (data().EvalFilter()) CallFunction(data().script, NULL, NULL, &s_callRes, 1, currGrabbed);
			}
		}
		lastGrabbed = currGrabbed;
		if (currGrabbed) lastGrabID = currGrabbed->refID;
	}

	static TESObjectREFR *lastCrosshair = NULL;
	static UInt32 lastCrshrID = 0;
	TESObjectREFR *currCrosshair = g_interfaceManager->crosshairRef;

	if (lastCrosshair != currCrosshair)
	{
		if (!s_gameLoadFlagLN)
		{
			if (lastCrosshair && (s_LNEventFlags & kLNEventMask_OnCrosshairOff))
			{
				if (lastCrosshair == LookupFormByRefID(lastCrshrID))
				{
					s_evalRefr = lastCrosshair;
					s_evalBase = lastCrosshair->GetBaseForm();
					for (auto data = s_LNEvents[kLNEventID_OnCrosshairOff].BeginCp(); data; ++data)
						if (data().EvalFilter()) CallFunction(data().script, NULL, NULL, &s_callRes, 1, lastCrosshair);
				}
			}
			if (currCrosshair && (s_LNEventFlags & kLNEventMask_OnCrosshairOn))
			{
				s_evalRefr = currCrosshair;
				s_evalBase = currCrosshair->GetBaseForm();
				for (auto data = s_LNEvents[kLNEventID_OnCrosshairOn].BeginCp(); data; ++data)
					if (data().EvalFilter()) CallFunction(data().script, NULL, NULL, &s_callRes, 1, currCrosshair);
			}
		}
		lastCrosshair = currCrosshair;
		if (currCrosshair) lastCrshrID = currCrosshair->refID;
	}
	
	static bool lastCtrlState[kMaxControlBinds] = {0};

	if (s_LNEventFlags & kLNEventMask_OnControl)
	{
		if (s_DInputEventClear & kLNEventMask_OnControl)
		{
			s_DInputEventClear ^= kLNEventMask_OnControl;
			for (auto onCtrlClr = s_LNOnControlEvents.BeginOp(); onCtrlClr; ++onCtrlClr)
				if (onCtrlClr().Empty()) onCtrlClr.Remove(s_LNOnControlEvents);
			if (s_LNOnControlEvents.Empty())
				s_LNEventFlags ^= kLNEventMask_OnControl;
		}
		UInt32 ctrl;
		bool currCtrlState;
		for (auto onCtrl = s_LNOnControlEvents.BeginCp(); onCtrl; ++onCtrl)
		{
			ctrl = onCtrl.Key();
			currCtrlState = IsControlPressedRaw(ctrl);
			if (lastCtrlState[ctrl] == currCtrlState) continue;
			if (currCtrlState) onCtrl().onDown.InvokeEvents(ctrl);
			else onCtrl().onUp.InvokeEvents(ctrl);
			lastCtrlState[ctrl] = currCtrlState;
		}
	}

	s_gameLoadFlagLN = false;
}

const char kLNEventNames[] =
"OnCellEnter\0OnCellExit\0OnPlayerGrab\0OnPlayerRelease\0OnCrosshairOn\0OnCrosshairOff\0OnButtonDown\0OnButtonUp\0OnKeyDown\0OnKeyUp\0OnControlDown\0OnControlUp";

void InitLNHooks()
{
	CommandInfo *cmdInfo = GetCmdByOpcode(0x1453);
	cmdInfo->execute = Hook_IsKeyPressed_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsKeyPressed_Eval;
	cmdInfo = GetCmdByOpcode(0x146B);
	cmdInfo->execute = Hook_IsControlPressed_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsControlPressed_Eval;
	cmdInfo = GetCmdByOpcode(0x15E9);
	SetEventHandler = cmdInfo->execute;
	cmdInfo->execute = Hook_SetEventHandler_Execute;
	cmdInfo = GetCmdByOpcode(0x15EA);
	RemoveEventHandler = cmdInfo->execute;
	cmdInfo->execute = Hook_RemoveEventHandler_Execute;

	const char *strPos = kLNEventNames;
	UInt32 count = 0;
	do
	{
		s_LNEventNames[strPos] = 1 << count;
		strPos += StrLen(strPos) + 1;
	}
	while (++count < 12);

	PrintLog("> LN hooks initialized successfully.");
}