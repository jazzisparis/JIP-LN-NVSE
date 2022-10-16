#include "internal/containers.h"

_NAKED char* __fastcall CopyStringKey(const char *key)
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

_NAKED UInt32 __fastcall AlignBucketCount(UInt32 count)
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
		inc		al
		shl		eax, cl
		retn
	}
}

_NAKED UInt32 __fastcall StrHashCS(const char *inKey)
{
	__asm
	{
		push	esi
		mov		eax, 0x1505
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		ecx, ecx
		ALIGN 16
	iterHead:
		mov		cl, [esi]
		test	cl, cl
		jz		done
		lea		edx, [eax+ecx]
		shl		eax, 5
		add		eax, edx
		mov		cl, [esi+1]
		test	cl, cl
		jz		done
		lea		edx, [eax+ecx]
		shl		eax, 5
		add		eax, edx
		mov		cl, [esi+2]
		test	cl, cl
		jz		done
		lea		edx, [eax+ecx]
		shl		eax, 5
		add		eax, edx
		mov		cl, [esi+3]
		test	cl, cl
		jz		done
		lea		edx, [eax+ecx]
		shl		eax, 5
		add		eax, edx
		add		esi, 4
		jmp		iterHead
		ALIGN 16
	done:
		pop		esi
		retn
	}
}

_NAKED UInt32 __fastcall StrHashCI(const char *inKey)
{
	__asm
	{
		push	esi
		mov		eax, 0x1505
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
		add		edx, eax
		shl		eax, 5
		add		eax, edx
		mov		cl, [esi+1]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		add		edx, eax
		shl		eax, 5
		add		eax, edx
		mov		cl, [esi+2]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		add		edx, eax
		shl		eax, 5
		add		eax, edx
		mov		cl, [esi+3]
		test	cl, cl
		jz		done
		movzx	edx, kLwrCaseConverter[ecx]
		add		edx, eax
		shl		eax, 5
		add		eax, edx
		add		esi, 4
		jmp		iterHead
		ALIGN 16
	done:
		pop		esi
		retn
	}
}