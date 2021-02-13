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