#include "internal/xinput.h"

XINPUT_GAMEPAD_EX s_gamePad = {};

bool s_controllerReady = false;

_XInputGetStateEx XInputGetStateEx;

int s_deadZoneLS = 7849, s_deadZoneRS = 8689;
double s_deadZoneLSg = 7849, s_deadZoneRSg = 8689;
double s_deadZoneLSd = 1.0 / 24918, s_deadZoneRSd = 1.0 / 24078;

XInputStateMods s_XIStateMods = {0xFBFF, 0, 0, 0, 0};

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
	done:
		mov		eax, result
		retn	8
		ALIGN 16
	updateState:
		push	ecx
		push	0
		call	XInputGetStateEx
		mov		result, eax
		or		al, ds:0x11D8C50
		test	eax, eax
		setz	s_controllerReady
		jnz		done
		push	esi
		mov		esi, [esp+0xC]
		movups	xmm0, [esi]
		movaps	s_gamePad, xmm0
		mov		ecx, offset s_XIStateMods
		mov		ax, [esi+4]
		xor		edx, edx
		test	ax, kXboxMask_GUIDE
		setz	dl
		neg		edx
		and		eax, edx
		or		ax, [ecx+2]
		and		ax, [ecx]
		or		ax, [ecx+4]
		mov		[esi+4], ax
		mov		word ptr [ecx+4], 0
		mov		al, [ecx+6]
		test	al, al
		jz		doneTRmods
		mov		dx, [esi+6]
		mov		ah, al
		and		ah, 0x11
		cmp		ah, 1
		jnz		testLThold
		xor		dl, dl
		jmp		testRTskip
		ALIGN 16
	testLThold:
		test	al, 0x14
		jz		testRTskip
		mov		dl, 0xFF
	testRTskip:
		mov		ah, al
		and		ah, 0x22
		cmp		ah, 2
		jnz		testRThold
		xor		dh, dh
		jmp		clrTRtap
		ALIGN 16
	testRThold:
		test	al, 0x28
		jz		clrTRtap
		mov		dh, 0xFF
	clrTRtap:
		mov		[esi+6], dx
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
		ALIGN 16
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
		ALIGN 16
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
		mov		eax, result
		pop		esi
		retn	8
	}
}

bool HookXInput()
{
	if (HMODULE xinput = GetModuleHandle("XINPUT1_3"))
		if (XInputGetStateEx = (_XInputGetStateEx)GetProcAddress(xinput, (LPCSTR)0x64))
		{
			WriteRelJump(0x9F996E, (UInt32)Hook_XInputGetState);
			return true;
		}
	return false;
}

__declspec(noinline) bool __fastcall GetXIControlPressed(XINPUT_GAMEPAD_EX *xiState, UInt32 ctrlID)
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

__declspec(noinline) bool __fastcall GetXIControlDisabled(UInt32 ctrlID)
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

__declspec(noinline) void __fastcall SetXIControlDisabled(UInt32 ctrlID, bool bDisable)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			s_XIStateMods.buttonSkip |= kXboxMask_DPAD_UP;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_DPAD_UP);
			break;
		case kXboxCtrl_DPAD_DOWN:
			s_XIStateMods.buttonSkip |= kXboxMask_DPAD_DOWN;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_DPAD_DOWN);
			break;
		case kXboxCtrl_DPAD_RIGHT:
			s_XIStateMods.buttonSkip |= kXboxMask_DPAD_RIGHT;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_DPAD_RIGHT);
			break;
		case kXboxCtrl_DPAD_LEFT:
			s_XIStateMods.buttonSkip |= kXboxMask_DPAD_LEFT;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_DPAD_LEFT);
			break;
		case kXboxCtrl_START:
			s_XIStateMods.buttonSkip |= kXboxMask_START;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_START);
			break;
		case kXboxCtrl_BACK:
			s_XIStateMods.buttonSkip |= kXboxMask_BACK;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_BACK);
			break;
		case kXboxCtrl_LS_BUTTON:
			s_XIStateMods.buttonSkip |= kXboxMask_LS_BUTTON;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_LS_BUTTON);
			break;
		case kXboxCtrl_RS_BUTTON:
			s_XIStateMods.buttonSkip |= kXboxMask_RS_BUTTON;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_RS_BUTTON);
			break;
		case kXboxCtrl_BUTTON_A:
			s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_A;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_BUTTON_A);
			break;
		case kXboxCtrl_BUTTON_B:
			s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_B;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_BUTTON_B);
			break;
		case kXboxCtrl_BUTTON_X:
			s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_X;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_BUTTON_X);
			break;
		case kXboxCtrl_BUTTON_Y:
			s_XIStateMods.buttonSkip |= kXboxMask_BUTTON_Y;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_BUTTON_Y);
			break;
		case kXboxCtrl_RB:
			s_XIStateMods.buttonSkip |= kXboxMask_RB;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_RB);
			break;
		case kXboxCtrl_LB:
			s_XIStateMods.buttonSkip |= kXboxMask_LB;
			s_XIStateMods.buttonSkip ^= (bDisable << kXboxBit_LB);
			break;
		case kXboxCtrl_LT:
			s_XIStateMods.triggerMods &= ~1;
			s_XIStateMods.triggerMods |= (UInt8)bDisable;
			break;
		case kXboxCtrl_RT:
			s_XIStateMods.triggerMods &= ~2;
			s_XIStateMods.triggerMods |= (bDisable << 1);
			break;
		case kXboxCtrl_LS_UP:
			s_XIStateMods.stickSkip &= ~1;
			s_XIStateMods.stickSkip |= (UInt8)bDisable;
			break;
		case kXboxCtrl_LS_DOWN:
			s_XIStateMods.stickSkip &= ~2;
			s_XIStateMods.stickSkip |= (bDisable << 1);
			break;
		case kXboxCtrl_LS_RIGHT:
			s_XIStateMods.stickSkip &= ~4;
			s_XIStateMods.stickSkip |= (bDisable << 2);
			break;
		case kXboxCtrl_LS_LEFT:
			s_XIStateMods.stickSkip &= ~8;
			s_XIStateMods.stickSkip |= (bDisable << 3);
			break;
		default:
			break;
	}
}

__declspec(noinline) void __fastcall SetXIControlHeld(UInt32 ctrlID, bool bHold)
{
	switch (CONTROLLER_BIND(ctrlID))
	{
		case kXboxCtrl_DPAD_UP:
			s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_UP;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_DPAD_UP);
			break;
		case kXboxCtrl_DPAD_DOWN:
			s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_DOWN;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_DPAD_DOWN);
			break;
		case kXboxCtrl_DPAD_RIGHT:
			s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_RIGHT;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_DPAD_RIGHT);
			break;
		case kXboxCtrl_DPAD_LEFT:
			s_XIStateMods.buttonHold &= ~kXboxMask_DPAD_LEFT;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_DPAD_LEFT);
			break;
		case kXboxCtrl_START:
			s_XIStateMods.buttonHold &= ~kXboxMask_START;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_START);
			break;
		case kXboxCtrl_BACK:
			s_XIStateMods.buttonHold &= ~kXboxMask_BACK;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_BACK);
			break;
		case kXboxCtrl_LS_BUTTON:
			s_XIStateMods.buttonHold &= ~kXboxMask_LS_BUTTON;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_LS_BUTTON);
			break;
		case kXboxCtrl_RS_BUTTON:
			s_XIStateMods.buttonHold &= ~kXboxMask_RS_BUTTON;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_RS_BUTTON);
			break;
		case kXboxCtrl_BUTTON_A:
			s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_A;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_BUTTON_A);
			break;
		case kXboxCtrl_BUTTON_B:
			s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_B;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_BUTTON_B);
			break;
		case kXboxCtrl_BUTTON_X:
			s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_X;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_BUTTON_X);
			break;
		case kXboxCtrl_BUTTON_Y:
			s_XIStateMods.buttonHold &= ~kXboxMask_BUTTON_Y;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_BUTTON_Y);
			break;
		case kXboxCtrl_RB:
			s_XIStateMods.buttonHold &= ~kXboxMask_RB;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_RB);
			break;
		case kXboxCtrl_LB:
			s_XIStateMods.buttonHold &= ~kXboxMask_LB;
			s_XIStateMods.buttonHold |= (bHold << kXboxBit_LB);
			break;
		case kXboxCtrl_LT:
			s_XIStateMods.triggerMods &= ~4;
			s_XIStateMods.triggerMods |= (bHold << 2);
			break;
		case kXboxCtrl_RT:
			s_XIStateMods.triggerMods &= ~8;
			s_XIStateMods.triggerMods |= (bHold << 3);
			break;
		case kXboxCtrl_LS_UP:
		case kXboxCtrl_LS_DOWN:
		case kXboxCtrl_LS_RIGHT:
		case kXboxCtrl_LS_LEFT:
		default:
			break;
	}
}

__declspec(noinline) bool __fastcall TapXIControl(UInt32 ctrlID)
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