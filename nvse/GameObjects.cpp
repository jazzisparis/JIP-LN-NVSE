#include "nvse/GameObjects.h"
#include "nvse/GameExtraData.h"
#include "nvse/GameTasks.h"

extern PlayerCharacter *g_thePlayer;

ScriptEventList *TESObjectREFR::GetEventList() const
{
	ExtraScript *xScript = GetExtraType(&extraDataList, Script);
	return xScript ? xScript->eventList : NULL;
}

PlayerCharacter *PlayerCharacter::GetSingleton()
{
	return g_thePlayer;
}

__declspec(naked) TESContainer *TESObjectREFR::GetContainer()
{
	__asm
	{
		mov		eax, [ecx]
		mov		eax, [eax+0x100]
		call	eax
		test	al, al
		mov		eax, [ecx+0x20]
		jz		notActor
		add		eax, 0x64
		retn
	notActor:
		cmp		dword ptr [eax], kVtbl_TESObjectCONT
		jnz		notCONT
		add		eax, 0x30
		retn
	notCONT:
		xor		eax, eax
		retn
	}
}

bool TESObjectREFR::IsMapMarker()
{
	return baseForm->refID == 0x10;
}

extern ModelLoader *g_modelLoader;

__declspec(naked) void TESObjectREFR::Update3D()
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x14], 0
		jz		done
		cmp		dword ptr [ecx+0xC], 0x14
		jz		isPlayer
		push	ecx
		push	1
		push	0
		CALL_EAX(0x5702E0)
		pop		ecx
		jmp		doQueue
	isPlayer:
		test	byte ptr [ecx+0x61], 1
		jnz		done
	doQueue:
		or		byte ptr [ecx+0x61], 1
		push	0
		push	1
		push	ecx
		mov		ecx, g_modelLoader
		CALL_EAX(0x444850)
	done:
		retn
	}
}

TESObjectREFR *TESObjectREFR::Create(bool bTemp)
{
	TESObjectREFR *refr = (TESObjectREFR*)GameHeapAlloc(sizeof(TESObjectREFR));
	ThisCall(0x55A2F0, refr);
	if (bTemp) ThisCall(0x484490, refr);
	return refr;
}