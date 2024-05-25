#include "internal/dinput.h"

__declspec(naked) bool __fastcall DIHookControl::IsKeyPressed(UInt32 keycode, UInt32 flags)
{
	__asm
	{
		cmp		edx, kMaxMacros
		jnb		retnFalse
		lea		ecx, [ecx+edx*8+4]
		sub		ecx, edx
		mov		eax, [esp+4]
		and		eax, 7
		mov		edx, kFlagMask[eax*4]
		test	[ecx], edx
		setnz	al
		retn	4
	retnFalse:
		xor		al, al
		retn	4
		ALIGN 4
	kFlagMask:
		EMIT_DW_4(0x00000100, 0x00000100, 0x00000001, 0x00000101)
		EMIT_DW_4(0x00010000, 0x00010100, 0x00010001, 0x00010101)
	}
}

bool DIHookControl::IsKeyPressedRaw(UInt32 keycode)
{
	return (keycode < kMaxMacros) && m_keys[keycode].rawState;
}

bool DIHookControl::IsLMBPressed()
{
	return m_keys[0x100].rawState;
}

bool DIHookControl::IsKeyDisabled(UInt32 keycode)
{
	return (keycode < kMaxMacros) && (*(UInt16*)&m_keys[keycode].userDisable & 0x101);
}

bool DIHookControl::IsKeyHeld(UInt32 keycode)
{
	return (keycode < kMaxMacros) && m_keys[keycode].hold;
}

bool DIHookControl::IsKeyTapped(UInt32 keycode)
{
	return (keycode < kMaxMacros) && m_keys[keycode].tap;
}

void DIHookControl::SetKeyDisableState(UInt32 keycode, bool bDisable)
{
	if (keycode < kMaxMacros)
		*(UInt16*)&m_keys[keycode].userDisable = bDisable ? 0x101 : 0;
}

void DIHookControl::SetLMBDisabled(bool bDisable)
{
	m_keys[0x100].userDisable = bDisable;
}

void DIHookControl::SetKeyHeldState(UInt32 keycode, bool bHold)
{
	if (keycode < kMaxMacros)
		m_keys[keycode].hold = bHold;
}

void DIHookControl::TapKey(UInt32 keycode)
{
	if (keycode < kMaxMacros)
		m_keys[keycode].tap = true;
}