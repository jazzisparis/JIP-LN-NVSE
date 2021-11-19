#include "internal/dinput.h"

__declspec(naked) bool __fastcall DIHookControl::IsKeyPressed(UInt32 keycode, UInt32 flags)
{
	static const UInt32 kFlagMask[] = {0x100, 0x100, 1, 0x101, 0x10000, 0x10100, 0x10001, 0x10101};
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
	return (keycode < kMaxMacros) && (m_keys[keycode].userDisable || m_keys[keycode].scriptDisable);
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
	{
		KeyInfo *info = &m_keys[keycode];
		info->userDisable = bDisable;
		info->scriptDisable = bDisable;
	}
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