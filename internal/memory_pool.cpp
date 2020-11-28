#include "internal/memory_pool.h"

alignas(16) void *s_availableCachedBlocks[(MAX_CACHED_BLOCK_SIZE >> 4) + 1] = {NULL};

__declspec(naked) void* __fastcall Pool_Alloc(UInt32 size)
{
	__asm
	{
		cmp		ecx, 0x10
		jbe		minSize
		test	cl, 0xF
		jz		isAligned
		and		ecx, 0xFFFFFFF0
		add		ecx, 0x10
	isAligned:
		cmp		ecx, MAX_CACHED_BLOCK_SIZE
		jbe		doCache
		push	ecx
		call	malloc
		pop		ecx
		retn
		NOP_0x5
	minSize:
		mov		ecx, 0x10
	doCache:
		mov		edx, offset s_availableCachedBlocks
	spinHead:
		xor		eax, eax
		lock cmpxchg [edx], edx
		test	eax, eax
		jnz		spinHead
		mov		eax, ecx
		shr		eax, 2
		add		edx, eax
		mov		eax, [edx]
		test	eax, eax
		jz		allocPool
		mov		ecx, [eax]
		mov		[edx], ecx
		xor		edx, edx
		mov		s_availableCachedBlocks, edx
		retn
	allocPool:
		push	esi
		mov		esi, ecx
		mov		ecx, MEMORY_POOL_SIZE
		push	edx
		xor		edx, edx
		mov		eax, ecx
		div		esi
		push	eax
		sub		ecx, edx
		add		ecx, 8
		push	ecx
		call	malloc
		pop		ecx
		pop		ecx
		sub		ecx, 2
		pop		edx
		add		eax, 8
		and		al, 0xF0
		mov		[edx], eax
		lea		edx, [eax+esi]
	linkHead:
		mov		[eax], edx
		mov		eax, edx
		add		edx, esi
		dec		ecx
		jnz		linkHead
		mov		[eax], ecx
		mov		eax, edx
		mov		s_availableCachedBlocks, ecx
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall Pool_Free(void *pBlock, UInt32 size)
{
	__asm
	{
		test	ecx, ecx
		jz		nullPtr
		test	dl, 0xF
		jz		isAligned
		and		edx, 0xFFFFFFF0
		add		edx, 0x10
		ALIGN 16
	isAligned:
		cmp		edx, MAX_CACHED_BLOCK_SIZE
		ja		doFree
		push	edx
		mov		edx, offset s_availableCachedBlocks
		ALIGN 16
	spinHead:
		xor		eax, eax
		lock cmpxchg [edx], edx
		test	eax, eax
		jnz		spinHead
		pop		eax
		shr		eax, 2
		add		edx, eax
		mov		eax, [edx]
		mov		[ecx], eax
		mov		[edx], ecx
		mov		s_availableCachedBlocks, 0
	nullPtr:
		retn
		ALIGN 16
	doFree:
		push	ecx
		call	free
		pop		ecx
		retn
	}
}

__declspec(naked) void* __fastcall Pool_Realloc(void *pBlock, UInt32 curSize, UInt32 reqSize)
{
	__asm
	{
		mov		eax, ecx
		mov		ecx, [esp+4]
		test	eax, eax
		jz		nullPtr
		cmp		ecx, edx
		jbe		done
		cmp		edx, MAX_CACHED_BLOCK_SIZE
		ja		doRealloc
		push	edx
		push	eax
		call	Pool_Alloc
		push	eax
		call	MemCopy
		pop		eax
		pop		ecx
		pop		edx
		push	eax
		call	Pool_Free
		pop		eax
		retn	4
		ALIGN 16
	nullPtr:
		call	Pool_Alloc
	done:
		retn	4
		ALIGN 16
	doRealloc:
		push	ecx
		push	eax
		call	realloc
		add		esp, 8
		retn	4
	}
}