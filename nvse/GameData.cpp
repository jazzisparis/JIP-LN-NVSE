#include "nvse/GameData.h"

const ModInfo *DataHandler::LookupModByName(const char *modName)
{
	ModList *pModList = &modList;
	for (UInt32 idx = 0; idx < pModList->loadedModCount; idx++)
	{
		ModInfo *pInfo = pModList->loadedMods[idx];
		if (!StrCompareCI(pInfo->name, modName))
			return pInfo;
	}
	return nullptr;
}

__declspec(naked) UInt8 __fastcall DataHandler::GetModIndex(const char *modName)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		lea		ebx, [ecx+0x21C]
		mov		esi, edx
		mov		edi, [ebx-4]
		ALIGN 16
	iterHead:
		mov		edx, esi
		mov		ecx, [ebx]
		add		ecx, 0x20
		call	StrCompareCI
		test	al, al
		jz		found
		add		ebx, 4
		dec		edi
		jnz		iterHead
		mov		al, 0xFF
		pop		edi
		pop		esi
		pop		ebx
		retn
	found:
		mov		ecx, [ebx]
		mov		al, [ecx+0x40C]
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

const char *DataHandler::GetNthModName(UInt32 modIndex)
{
	auto modInfo = modList.loadedMods[modIndex];
	return modInfo ? modInfo->name : "";
}

void Sky::RefreshMoon()
{
	if (masserMoon) masserMoon->Destroy(true);
	masserMoon = ThisCall<Moon*>(0x634A70, GameHeapAlloc(sizeof(Moon)), (const char*)0x104EEB0, *(UInt32*)0x11CCCBC, *(UInt32*)0x11CCC98, *(UInt32*)0x11CCBA8, *(UInt32*)0x11CCC00, *(UInt32*)0x11CCC58, *(UInt32*)0x11CCC1C);
	masserMoon->Refresh(niNode008, (const char*)0x104EEB0);
}

__declspec(naked) bool Sky::GetIsRaining()
{
	__asm
	{
		mov		eax, [ecx+0x10]
		test	eax, eax
		jz		checkSecond
		cmp		byte ptr [eax+0xEB], 4
		jz		weatherPerc
	checkSecond:
		mov		eax, [ecx+0x14]
		test	eax, eax
		jz		retnFalse
		cmp		byte ptr [eax+0xEB], 4
		jnz		retnFalse
	weatherPerc:
		movss	xmm0, PS_V3_One
		comiss	xmm0, [ecx+0xF4]
		setbe	al
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

__declspec(naked) TESObjectCELL *GridCellArray::GetCell(Coordinate cellXY) const
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		mov		ecx, [ebx+0xC]
		shr		ecx, 1
		movsx	eax, word ptr [esp+0xA]
		sub		eax, [ebx+4]
		add		eax, ecx
		cmp		eax, [ebx+0xC]
		jnb		retnNull
		movsx	edx, word ptr [esp+8]
		sub		edx, [ebx+8]
		add		edx, ecx
		cmp		edx, [ebx+0xC]
		jnb		retnNull
		add		edx, eax
		shl		eax, cl
		add		edx, eax
		mov		ecx, [ebx+0x10]
		mov		eax, [ecx+edx*4]
		pop		ebx
		retn	4
	retnNull:
		xor		eax, eax
		pop		ebx
		retn	4
	}
}