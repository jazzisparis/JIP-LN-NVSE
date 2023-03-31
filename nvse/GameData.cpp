#include "nvse/GameData.h"

ModInfo* __fastcall DataHandler::LookupModByName(const char *modName)
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

__declspec(naked) UInt8 __fastcall DataHandler::GetModIndex(const char *modName) const
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

GridCellArray *g_gridCellArray;

__declspec(naked) TESObjectCELL* __vectorcall GridCellArray::GetCellAtPos(__m128 pos) const
{
	__asm
	{
		pxor	xmm3, xmm3
		unpcklpd	xmm0, xmm3
		cvttps2dq	xmm0, xmm0
		psrad	xmm0, 0xC
		movq	xmm1, qword ptr [ecx+4]
		psubd	xmm0, xmm1
		movd	xmm1, [ecx+0xC]
		unpcklps	xmm1, xmm1
		movaps	xmm2, xmm1
		psrld	xmm2, 1
		paddd	xmm0, xmm2
		pcmpgtd	xmm1, xmm0
		movmskps	eax, xmm1
		cmp		al, 3
		jnz		retnNull
		movaps	xmm1, xmm0
		phaddd	xmm1, xmm3
		psrldq	xmm2, 4
		pslld	xmm0, xmm2
		paddd	xmm0, xmm1
		pextrw	edx, xmm0, 0
		mov		ecx, [ecx+0x10]
		mov		eax, [ecx+edx*4]
		retn
	retnNull:
		xor		eax, eax
		retn
	}
}

__declspec(naked) TESObjectCELL* __vectorcall GridCellArray::GetCellAtCoord(__m128i cellXY) const
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		movd	eax, xmm0
		movsx	edx, ax
		sar		eax, 0x10
		mov		ecx, [ebx+0xC]
		shr		ecx, 1
		sub		eax, [ebx+4]
		add		eax, ecx
		cmp		eax, [ebx+0xC]
		jnb		retnNull
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
		retn
	retnNull:
		xor		eax, eax
		pop		ebx
		retn
	}
}

void TES::UnloadBufferedExterior(TESObjectCELL *cell)
{
	UInt32 bufferSize = *(UInt32*)0x11C3C94;
	for (UInt32 i = 0; i < bufferSize; i++)
		if (!exteriorsBuffer[i])
		{
			bufferSize = i;
			break;
		}			
	for (UInt32 i = 0; i < bufferSize; i++)
	{
		if (exteriorsBuffer[i] != cell)
			continue;
		memmove(exteriorsBuffer + i, exteriorsBuffer + i + 1, (bufferSize - i - 1) << 2);
		ThisCall(0x462290, g_dataHandler, cell);
		break;
	}
}