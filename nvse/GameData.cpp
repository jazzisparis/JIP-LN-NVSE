#include "nvse/GameData.h"

__declspec(naked) ModInfo* __fastcall DataHandler::LookupModByName(const char *modName) const
{
	_asm
	{
		push	ebx
		lea		ebx, [ecx+0x21C]
		mov		ecx, edx
		call	StrHashCI
		mov		ecx, eax
		mov		edx, [ebx-4]
		ALIGN 16
	iterHead:
		mov		eax, [ebx]
		cmp		[eax+0x120], ecx
		jz		done
		add		ebx, 4
		dec		edx
		jnz		iterHead
		xor		eax, eax
	done:
		pop		ebx
		retn
	}
}

void Sky::RefreshMoon()
{
	if (masserMoon) masserMoon->Destroy(true);
	masserMoon = ThisCall<Moon*>(0x634A70, Game_HeapAlloc<Moon>(), (const char*)0x104EEB0, GMST_FLT(fMasserAngleFadeStart), GMST_FLT(fMasserAngleFadeEnd),
		GMST_FLT(fMasserAngleShadowEarlyFade), GMST_FLT(fMasserSpeed), GMST_FLT(fMasserZOffset), GMST_INT(iMasserSize));
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

void __fastcall WaterSurfaceManager::Update(NiCamera *camera)
{
	if (!waterGroups.Empty())
		ThisCall(0x4E21B0, this, camera, 0);
}
__declspec(naked) void __fastcall WaterSurfaceManager::UpdateEx(NiCamera *camera)
{
	__asm
	{
		cmp		dword ptr [ecx+0x3C], 0
		jz		done
		mov		eax, 0x11C7B70
		push	dword ptr [eax]
		and		dword ptr [eax], 0
		push	dword ptr [eax+0x50]
		and		dword ptr [eax+0x50], 0
		push	eax
		push	0
		push	edx
		CALL_EAX(0x4E21B0)
		pop		eax
		pop		dword ptr [eax+0x50]
		pop		dword ptr [eax]
	done:
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
	UInt32 bufferSize = INIS_UINT(uExterior_Cell_Buffer_General);
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