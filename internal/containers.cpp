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

__declspec(naked) UInt32 __fastcall StrHashCS(const char *inKey)
{
	__asm
	{
		mov		eax, 0x6B49D20B
		test	ecx, ecx
		jz		done
		ALIGN 16
	iterHead:
		movzx	edx, byte ptr [ecx]
		test	dl, dl
		jz		done
		shl		edx, 4
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		movzx	edx, byte ptr [ecx+1]
		test	dl, dl
		jz		done
		shl		edx, 0xC
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		movzx	edx, byte ptr [ecx+2]
		test	dl, dl
		jz		done
		shl		edx, 0x14
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		movzx	edx, byte ptr [ecx+3]
		test	dl, dl
		jz		done
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		add		ecx, 4
		jmp		iterHead
	done:
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrHashCI(const char *inKey)
{
	__asm
	{
		push	esi
		mov		eax, 0x6B49D20B
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		ecx, ecx
		ALIGN 16
	iterHead:
		mov		cl, [esi]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		shl		edx, 4
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		mov		cl, [esi+1]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		shl		edx, 0xC
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		mov		cl, [esi+2]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		shl		edx, 0x14
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		mov		cl, [esi+3]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		sub		eax, edx
		mov		edx, eax
		shl		eax, 5
		sub		eax, edx
		add		esi, 4
		jmp		iterHead
	done:
		pop		esi
		retn
	}
}