#pragma once

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
double s_deadZoneLSg = 7849, s_deadZoneRSg = 8689;
double s_deadZoneLSd = 24918, s_deadZoneRSd = 24078;

struct XInputStateMods
{
	UInt16		buttonSkip;		// 00
	UInt16		buttonHold;		// 02
	UInt16		buttonTap;		// 04
	UInt8		triggerMods;	// 06
	UInt8		stickSkip;		// 07
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
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		jmp		done
	updateState:
		push	ecx
		push	0
		call	XInputGetStateEx
		mov		result, eax
		or		al, ds:[0x11D8C50]
		test	eax, eax
		setz	al
		mov		s_controllerReady, al
		jnz		done
		push	esi
		mov		esi, [esp+0xC]
		movups	xmm0, [esi]
		movaps	s_gamePad, xmm0
		mov		ecx, offset s_XIStateMods
		mov		ax, [esi+4]
		or		ax, [ecx+2]
		and		ax, [ecx]
		or		ax, [ecx+4]
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
		xor		edx, edx
		mov		al, [ecx+7]
		test	al, 0xF
		jz		rightStick
		cmp		[esi+0xA], dx
		jz		lStickX
		jl		lStickDn
		test	al, 1
		jz		lStickX
		jmp		lStickY0
	lStickDn:
		test	al, 2
		jz		lStickX
	lStickY0:
		mov		[esi+0xA], dx
	lStickX:
		cmp		[esi+8], dx
		jz		rightStick
		jl		lStickLt
		test	al, 4
		jz		rightStick
		jmp		lStickX0
	lStickLt:
		test	al, 8
		jz		rightStick
	lStickX0:
		mov		[esi+8], dx
	rightStick:
		test	al, 0xF0
		jz		doneSticks
		mov		[esi+0xC], edx
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

bool __fastcall GetXIControlPressed(XINPUT_GAMEPAD_EX *xiState, UInt32 ctrlID)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			return (xiState->wButtons & kXboxMask_DPAD_UP) != 0;
		case kXboxCtrl_DPAD_DOWN:
			return (xiState->wButtons & kXboxMask_DPAD_DOWN) != 0;
		case kXboxCtrl_DPAD_RIGHT:
			return (xiState->wButtons & kXboxMask_DPAD_RIGHT) != 0;
		case kXboxCtrl_DPAD_LEFT:
			return (xiState->wButtons & kXboxMask_DPAD_LEFT) != 0;
		case kXboxCtrl_START:
			return (xiState->wButtons & kXboxMask_START) != 0;
		case kXboxCtrl_BACK:
			return (xiState->wButtons & kXboxMask_BACK) != 0;
		case kXboxCtrl_LS_BUTTON:
			return (xiState->wButtons & kXboxMask_LS_BUTTON) != 0;
		case kXboxCtrl_RS_BUTTON:
			return (xiState->wButtons & kXboxMask_RS_BUTTON) != 0;
		case kXboxCtrl_BUTTON_A:
			return (xiState->wButtons & kXboxMask_BUTTON_A) != 0;
		case kXboxCtrl_BUTTON_B:
			return (xiState->wButtons & kXboxMask_BUTTON_B) != 0;
		case kXboxCtrl_BUTTON_X:
			return (xiState->wButtons & kXboxMask_BUTTON_X) != 0;
		case kXboxCtrl_BUTTON_Y:
			return (xiState->wButtons & kXboxMask_BUTTON_Y) != 0;
		case kXboxCtrl_RB:
			return (xiState->wButtons & kXboxMask_RB) != 0;
		case kXboxCtrl_LB:
			return (xiState->wButtons & kXboxMask_LB) != 0;
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

bool __fastcall GetXIControlDisabled(UInt32 ctrlID)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			return (s_XIStateMods.buttonSkip & kXboxMask_DPAD_UP) != 0;
		case kXboxCtrl_DPAD_DOWN:
			return (s_XIStateMods.buttonSkip & kXboxMask_DPAD_DOWN) != 0;
		case kXboxCtrl_DPAD_RIGHT:
			return (s_XIStateMods.buttonSkip & kXboxMask_DPAD_RIGHT) != 0;
		case kXboxCtrl_DPAD_LEFT:
			return (s_XIStateMods.buttonSkip & kXboxMask_DPAD_LEFT) != 0;
		case kXboxCtrl_START:
			return (s_XIStateMods.buttonSkip & kXboxMask_START) != 0;
		case kXboxCtrl_BACK:
			return (s_XIStateMods.buttonSkip & kXboxMask_BACK) != 0;
		case kXboxCtrl_LS_BUTTON:
			return (s_XIStateMods.buttonSkip & kXboxMask_LS_BUTTON) != 0;
		case kXboxCtrl_RS_BUTTON:
			return (s_XIStateMods.buttonSkip & kXboxMask_RS_BUTTON) != 0;
		case kXboxCtrl_BUTTON_A:
			return (s_XIStateMods.buttonSkip & kXboxMask_BUTTON_A) != 0;
		case kXboxCtrl_BUTTON_B:
			return (s_XIStateMods.buttonSkip & kXboxMask_BUTTON_B) != 0;
		case kXboxCtrl_BUTTON_X:
			return (s_XIStateMods.buttonSkip & kXboxMask_BUTTON_X) != 0;
		case kXboxCtrl_BUTTON_Y:
			return (s_XIStateMods.buttonSkip & kXboxMask_BUTTON_Y) != 0;
		case kXboxCtrl_RB:
			return (s_XIStateMods.buttonSkip & kXboxMask_RB) != 0;
		case kXboxCtrl_LB:
			return (s_XIStateMods.buttonSkip & kXboxMask_LB) != 0;
		case kXboxCtrl_LT:
			return (s_XIStateMods.triggerMods & 1) != 0;
		case kXboxCtrl_RT:
			return (s_XIStateMods.triggerMods & 2) != 0;
		case kXboxCtrl_LS_UP:
			return (s_XIStateMods.stickSkip & 1) != 0;
		case kXboxCtrl_LS_DOWN:
			return (s_XIStateMods.stickSkip & 2) != 0;
		case kXboxCtrl_LS_RIGHT:
			return (s_XIStateMods.stickSkip & 4) != 0;
		case kXboxCtrl_LS_LEFT:
			return (s_XIStateMods.stickSkip & 8) != 0;
		default:
			return false;
	}
}

void __fastcall SetXIControlDisabled(UInt32 ctrlID, bool bDisable)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_DPAD_UP;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_DPAD_UP;
			break;
		case kXboxCtrl_DPAD_DOWN:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_DPAD_DOWN;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_DPAD_DOWN;
			break;
		case kXboxCtrl_DPAD_RIGHT:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_DPAD_RIGHT;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_DPAD_RIGHT;
			break;
		case kXboxCtrl_DPAD_LEFT:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_DPAD_LEFT;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_DPAD_LEFT;
			break;
		case kXboxCtrl_START:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_START;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_START;
			break;
		case kXboxCtrl_BACK:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_BACK;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_BACK;
			break;
		case kXboxCtrl_LS_BUTTON:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_LS_BUTTON;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_LS_BUTTON;
			break;
		case kXboxCtrl_RS_BUTTON:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_RS_BUTTON;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_RS_BUTTON;
			break;
		case kXboxCtrl_BUTTON_A:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_A;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_BUTTON_A;
			break;
		case kXboxCtrl_BUTTON_B:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_B;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_BUTTON_B;
			break;
		case kXboxCtrl_BUTTON_X:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_X;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_BUTTON_X;
			break;
		case kXboxCtrl_BUTTON_Y:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_Y;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_BUTTON_Y;
			break;
		case kXboxCtrl_RB:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_RB;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_RB;
			break;
		case kXboxCtrl_LB:
			if (!bDisable) s_XIStateMods.buttonSkip |= kXboxMask_LB;
			else s_XIStateMods.buttonSkip &= ~kXboxMask_LB;
			break;
		case kXboxCtrl_LT:
			if (bDisable) s_XIStateMods.triggerMods |= 1;
			else s_XIStateMods.triggerMods &= ~1;
			break;
		case kXboxCtrl_RT:
			if (bDisable) s_XIStateMods.triggerMods |= 2;
			else s_XIStateMods.triggerMods &= ~2;
			break;
		case kXboxCtrl_LS_UP:
			if (bDisable) s_XIStateMods.stickSkip |= 1;
			else s_XIStateMods.stickSkip &= ~1;
			break;
		case kXboxCtrl_LS_DOWN:
			if (bDisable) s_XIStateMods.stickSkip |= 2;
			else s_XIStateMods.stickSkip &= ~2;
			break;
		case kXboxCtrl_LS_RIGHT:
			if (bDisable) s_XIStateMods.stickSkip |= 4;
			else s_XIStateMods.stickSkip &= ~4;
			break;
		case kXboxCtrl_LS_LEFT:
			if (bDisable) s_XIStateMods.stickSkip |= 8;
			else s_XIStateMods.stickSkip &= ~8;
			break;
		default:
			break;
	}
}

void __fastcall SetXIControlHeld(UInt32 ctrlID, bool bHold)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_DPAD_UP;
			else s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_UP;
			break;
		case kXboxCtrl_DPAD_DOWN:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_DPAD_DOWN;
			else s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_DOWN;
			break;
		case kXboxCtrl_DPAD_RIGHT:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_DPAD_RIGHT;
			else s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_RIGHT;
			break;
		case kXboxCtrl_DPAD_LEFT:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_DPAD_LEFT;
			else s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_LEFT;
			break;
		case kXboxCtrl_START:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_START;
			else s_XIStateMods.buttonHold &= ~kXboxMask_START;
			break;
		case kXboxCtrl_BACK:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_BACK;
			else s_XIStateMods.buttonHold &= ~kXboxMask_BACK;
			break;
		case kXboxCtrl_LS_BUTTON:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_LS_BUTTON;
			else s_XIStateMods.buttonHold &= ~kXboxMask_LS_BUTTON;
			break;
		case kXboxCtrl_RS_BUTTON:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_RS_BUTTON;
			else s_XIStateMods.buttonHold &= ~kXboxMask_RS_BUTTON;
			break;
		case kXboxCtrl_BUTTON_A:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_BUTTON_A;
			else s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_A;
			break;
		case kXboxCtrl_BUTTON_B:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_BUTTON_B;
			else s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_B;
			break;
		case kXboxCtrl_BUTTON_X:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_BUTTON_X;
			else s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_X;
			break;
		case kXboxCtrl_BUTTON_Y:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_BUTTON_Y;
			else s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_Y;
			break;
		case kXboxCtrl_RB:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_RB;
			else s_XIStateMods.buttonHold &= ~kXboxMask_RB;
			break;
		case kXboxCtrl_LB:
			if (bHold) s_XIStateMods.buttonHold |= kXboxMask_LB;
			else s_XIStateMods.buttonHold &= ~kXboxMask_LB;
			break;
		case kXboxCtrl_LT:
			if (bHold) s_XIStateMods.triggerMods |= 4;
			else s_XIStateMods.triggerMods &= ~4;
			break;
		case kXboxCtrl_RT:
			if (bHold) s_XIStateMods.triggerMods |= 8;
			else s_XIStateMods.triggerMods &= ~8;
			break;
		case kXboxCtrl_LS_UP:
		case kXboxCtrl_LS_DOWN:
		case kXboxCtrl_LS_RIGHT:
		case kXboxCtrl_LS_LEFT:
		default:
			break;
	}
}

bool __fastcall TapXIControl(UInt32 ctrlID)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			s_XIStateMods.buttonTap |= kXboxMask_DPAD_UP;
			return true;
		case kXboxCtrl_DPAD_DOWN:
			s_XIStateMods.buttonTap |= kXboxMask_DPAD_DOWN;
			return true;
		case kXboxCtrl_DPAD_RIGHT:
			s_XIStateMods.buttonTap |= kXboxMask_DPAD_RIGHT;
			return true;
		case kXboxCtrl_DPAD_LEFT:
			s_XIStateMods.buttonTap |= kXboxMask_DPAD_LEFT;
			return true;
		case kXboxCtrl_START:
			s_XIStateMods.buttonTap |= kXboxMask_START;
			return true;
		case kXboxCtrl_BACK:
			s_XIStateMods.buttonTap |= kXboxMask_BACK;
			return true;
		case kXboxCtrl_LS_BUTTON:
			s_XIStateMods.buttonTap |= kXboxMask_LS_BUTTON;
			return true;
		case kXboxCtrl_RS_BUTTON:
			s_XIStateMods.buttonTap |= kXboxMask_RS_BUTTON;
			return true;
		case kXboxCtrl_BUTTON_A:
			s_XIStateMods.buttonTap |= kXboxMask_BUTTON_A;
			return true;
		case kXboxCtrl_BUTTON_B:
			s_XIStateMods.buttonTap |= kXboxMask_BUTTON_B;
			return true;
		case kXboxCtrl_BUTTON_X:
			s_XIStateMods.buttonTap |= kXboxMask_BUTTON_X;
			return true;
		case kXboxCtrl_BUTTON_Y:
			s_XIStateMods.buttonTap |= kXboxMask_BUTTON_Y;
			return true;
		case kXboxCtrl_RB:
			s_XIStateMods.buttonTap |= kXboxMask_RB;
			return true;
		case kXboxCtrl_LB:
			s_XIStateMods.buttonTap |= kXboxMask_LB;
			return true;
		case kXboxCtrl_LT:
			s_XIStateMods.triggerMods |= 0x10;
			return true;
		case kXboxCtrl_RT:
			s_XIStateMods.triggerMods |= 0x20;
			return true;
		case kXboxCtrl_LS_UP:
		case kXboxCtrl_LS_DOWN:
		case kXboxCtrl_LS_RIGHT:
		case kXboxCtrl_LS_LEFT:
		default:
			return false;
	}
}