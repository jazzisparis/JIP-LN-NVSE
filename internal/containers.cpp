#include "internal/containers.h"

__declspec(naked) char* __fastcall CopyStringKey(const char *key)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		call	StrLen
		lea		edi, [eax+1]
		lea		ecx, [edi+0x14]
		and		cl, 0xF0
		push	ecx
		call	MemoryPool::Alloc
		pop		dword ptr [eax]
		add		eax, 4
		mov		ecx, edi
		mov		edi, eax
		rep movsb
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall AlignBucketCount(UInt32 count)
{
	__asm
	{
		mov		eax, MAP_MIN_BUCKET_COUNT
		cmp		ecx, eax
		cmovb	ecx, eax
		mov		eax, MAP_MAX_BUCKET_COUNT
		cmp		ecx, eax
		cmova	ecx, eax
		bsf		eax, ecx
		bsr		ecx, ecx
		cmp		al, cl
		setnz	al
		inc		eax
		shl		eax, cl
		retn
	}
}

#define STR_HASH_SEED 0xD39CA509UL

__declspec(naked) UInt32 __fastcall StrHashCS(const char *inKey)
{
	__asm
	{
		test	ecx, ecx
		jz		nullStr
		push	ebx
		mov		eax, STR_HASH_SEED
		ALIGN 16
	iterHead:
		mov		ebx, [ecx]
		test	bl, bl
		jz		done
		movzx	edx, bl
		shl		edx, 4
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		test	bh, bh
		jz		done
		movzx	edx, bh
		shl		edx, 0xC
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		shr		ebx, 0x10
		test	bl, bl
		jz		done
		movzx	edx, bl
		shl		edx, 0x14
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		test	bh, bh
		jz		done
		movzx	edx, bh
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		add		ecx, 4
		jmp		iterHead
	nullStr:
		xor		eax, eax
		retn
		ALIGN 16
	done:
		pop		ebx
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrHashCI(const char *inKey)
{
	__asm
	{
		test	ecx, ecx
		jz		nullStr
		push	ebx
		push	esi
		mov		eax, STR_HASH_SEED
		mov		esi, ecx
		xor		ecx, ecx
		ALIGN 16
	iterHead:
		mov		ebx, [esi]
		test	bl, bl
		jz		done
		mov		cl, bl
		movzx	edx, kLwrCaseConverter[ecx]
		shl		edx, 4
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		test	bh, bh
		jz		done
		mov		cl, bh
		movzx	edx, kLwrCaseConverter[ecx]
		shl		edx, 0xC
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		shr		ebx, 0x10
		test	bl, bl
		jz		done
		mov		cl, bl
		movzx	edx, kLwrCaseConverter[ecx]
		shl		edx, 0x14
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		test	bh, bh
		jz		done
		mov		cl, bh
		movzx	edx, kLwrCaseConverter[ecx]
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		add		esi, 4
		jmp		iterHead
	nullStr:
		xor		eax, eax
		retn
		ALIGN 16
	done:
		pop		esi
		pop		ebx
		retn
	}
}