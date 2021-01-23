#include "internal/containers.h"

__declspec(naked) void* __fastcall Pool_Alloc_Buckets(UInt32 numBuckets)
{
	__asm
	{
		push	ecx
		shl		ecx, 2
		call	Pool_Alloc
		pop		ecx
		push	eax
		push	edi
		mov		edi, eax
		xor		eax, eax
		rep stosd
		pop		edi
		pop		eax
		retn
	}
}

__declspec(naked) UInt32 __fastcall AlignBucketCount(UInt32 count)
{
	__asm
	{
		cmp		ecx, MAP_DEFAULT_BUCKET_COUNT
		ja		gtMin
		mov		eax, MAP_DEFAULT_BUCKET_COUNT
		retn
	gtMin:
		cmp		ecx, MAP_MAX_BUCKET_COUNT
		jb		ltMax
		mov		eax, MAP_MAX_BUCKET_COUNT
		retn
	ltMax:
		mov		eax, ecx
		bsr		ecx, eax
		bsf		edx, eax
		cmp		cl, dl
		jz		done
		mov		eax, 2
		shl		eax, cl
	done:
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrHashCS(const char *inKey)
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
		mov		edx, eax
		add		eax, ecx
		shl		edx, 5
		add		eax, edx
		inc		esi
		jmp		iterHead
	done:
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrHashCI(const char *inKey)
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
		mov		edx, eax
		shl		edx, 5
		add		eax, edx
		movzx	edx, kLwrCaseConverter[ecx]
		add		eax, edx
		inc		esi
		jmp		iterHead
	done:
		pop		esi
		retn
	}
}