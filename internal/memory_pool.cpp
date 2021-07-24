#include "internal/memory_pool.h"

#define MAX_BLOCK_SIZE		0x400
#define MEMORY_POOL_SIZE	0x1000

struct MemoryPool
{
	struct BlockNode
	{
		BlockNode	*m_next;
		//	Data
	};

	PrimitiveCS		m_cs;
	BlockNode		*m_pools[MAX_BLOCK_SIZE >> 4];

	MemoryPool() {MemZero(m_pools, sizeof(m_pools));}
};

alignas(16) MemoryPool s_memoryPool;

__declspec(naked) void* __fastcall Pool_Alloc(UInt32 size)
{
	__asm
	{
		test	cl, 0xF
		jz		isAligned
		and		cl, 0xF0
		add		ecx, 0x10
	isAligned:
		cmp		ecx, MAX_BLOCK_SIZE
		jbe		doCache
		push	0x10
		push	ecx
		call	_aligned_malloc
		add		esp, 8
		retn
	doCache:
		push	ecx
		mov		ecx, offset s_memoryPool.m_cs
		call	PrimitiveCS::Enter
		pop		ecx
		mov		edx, ecx
		shr		edx, 2
		add		edx, eax
		mov		eax, [edx]
		test	eax, eax
		jz		allocPool
		mov		ecx, [eax]
		mov		[edx], ecx
		xor		edx, edx
		mov		s_memoryPool.m_cs.owningThread, edx
		retn
		NOP_0x2
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
		push	0x10
		push	ecx
		call	_aligned_malloc
		add		esp, 8
		pop		ecx
		sub		ecx, 2
		pop		edx
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
		mov		s_memoryPool.m_cs.owningThread, ecx
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
		and		dl, 0xF0
		add		edx, 0x10
		ALIGN 16
	isAligned:
		cmp		edx, MAX_BLOCK_SIZE
		ja		doFree
		push	edx
		push	ecx
		mov		ecx, offset s_memoryPool.m_cs
		call	PrimitiveCS::Enter
		pop		ecx
		pop		edx
		shr		edx, 2
		add		edx, eax
		mov		eax, [edx]
		mov		[ecx], eax
		mov		[edx], ecx
		mov		s_memoryPool.m_cs.owningThread, 0
	nullPtr:
		retn
		ALIGN 16
	doFree:
		push	ecx
		call	_aligned_free
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
		cmp		edx, MAX_BLOCK_SIZE
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
		push	0x10
		push	ecx
		push	eax
		call	_aligned_realloc
		add		esp, 0xC
		retn	4
	}
}