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
		lea		ecx, [edi+4]
		test	cl, 0xF
		jz		isAligned
		and		cl, 0xF0
		add		ecx, 0x10
	isAligned:
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
		cmp		ecx, MAP_MIN_BUCKET_COUNT
		ja		gtMin
		mov		eax, MAP_MIN_BUCKET_COUNT
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
		mov		cl, [esi+1]
		test	cl, cl
		jz		done
		mov		edx, eax
		add		eax, ecx
		shl		edx, 5
		add		eax, edx
		add		esi, 2
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
		mov		cl, [esi+1]
		test	cl, cl
		jz		done
		mov		edx, eax
		shl		edx, 5
		add		eax, edx
		movzx	edx, kLwrCaseConverter[ecx]
		add		eax, edx
		add		esi, 2
		jmp		iterHead
	done:
		pop		esi
		retn
	}
}