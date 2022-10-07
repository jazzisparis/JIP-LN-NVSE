#include "internal/memory_pool.h"

namespace MemoryPool
{
	struct M_Pool
	{
		struct BlockNode
		{
			BlockNode	*m_next;
			//	Data
		};

		PrimitiveCS		m_cs;
		BlockNode		*m_freeSections = nullptr;
		BlockNode		*m_sections[MAX_BLOCK_SIZE >> 4] = {nullptr};
	}
	s_memoryPool;

	__declspec(naked) void* __fastcall Alloc(size_t size)
	{
		__asm
		{
			cmp		ecx, MAX_BLOCK_SIZE
			ja		doMalloc
			push	ebx
			mov		ebx, ecx
			mov		ecx, offset s_memoryPool.m_cs
			call	PrimitiveCS::Enter
			mov		edx, ebx
			shr		edx, 2
			lea		edx, [eax+edx+4]
			mov		eax, [edx]
			test	eax, eax
			jz		newSection
			mov		ecx, [eax]
			mov		[edx], ecx
			mov		s_memoryPool.m_cs.selfPtr, 0
			pop		ebx
			retn
			ALIGN 16
		doMalloc:
			push	0x10
			push	ecx
			call	_aligned_malloc
			add		esp, 8
			retn
			ALIGN 16
		newSection:
			push	esi
			push	edx
			mov		esi, s_memoryPool.m_freeSections
			test	esi, esi
			jz		newPool
			mov		eax, [esi]
			mov		s_memoryPool.m_freeSections, eax
			jmp		gotSection
			NOP_0x3
		newPool:
			push	PAGE_READWRITE
			push	MEM_RESERVE | MEM_COMMIT
			push	MEMORY_POOL_SIZE
			push	0
			call	VirtualAlloc
			mov		esi, eax
			lea		edx, [eax+MEMORY_POOL_SIZE-POOL_SECTION_SIZE]
			add		eax, POOL_SECTION_SIZE
			mov		s_memoryPool.m_freeSections, eax
			mov		ecx, eax
		sectLinker:
			add		eax, POOL_SECTION_SIZE
			cmp		eax, edx
			ja		gotSection
			mov		[ecx], eax
			mov		ecx, eax
			jmp		sectLinker
			ALIGN 16
		gotSection:
			pop		edx
			lea		eax, [esi+ebx]
			mov		[edx], eax
			lea		edx, [esi+POOL_SECTION_SIZE]
			sub		edx, ebx
			mov		ecx, eax
		blockLinker:
			add		eax, ebx
			cmp		eax, edx
			ja		done
			mov		[ecx], eax
			mov		ecx, eax
			jmp		blockLinker
			ALIGN 16
		done:
			mov		eax, esi
			mov		s_memoryPool.m_cs.selfPtr, 0
			pop		esi
			pop		ebx
			retn
		}
	}

	__declspec(naked) void __fastcall Free(void *pBlock, size_t size)
	{
		__asm
		{
			test	ecx, ecx
			jz		nullPtr
			cmp		edx, MAX_BLOCK_SIZE
			ja		doFree
			push	edx
			push	ecx
			mov		ecx, offset s_memoryPool.m_cs
			call	PrimitiveCS::Enter
			pop		ecx
			pop		edx
			shr		edx, 2
			lea		edx, [eax+edx+4]
			mov		eax, [edx]
			mov		[ecx], eax
			mov		[edx], ecx
			mov		s_memoryPool.m_cs.selfPtr, 0
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

	__declspec(naked) void* __fastcall Realloc(void *pBlock, size_t curSize, size_t reqSize)
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
			call	Alloc
			push	eax
			call	MemCopy
			pop		eax
			pop		ecx
			pop		edx
			push	eax
			call	Free
			pop		eax
			retn	4
			ALIGN 16
		doRealloc:
			push	0x10
			push	ecx
			push	eax
			call	_aligned_realloc
			add		esp, 0xC
			retn	4
			ALIGN 16
		nullPtr:
			call	Alloc
		done:
			retn	4
		}
	}
};