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
			push	esi
			mov		esi, ecx
			mov		ecx, offset s_memoryPool.m_cs
			call	PrimitiveCS::Enter
			mov		edx, esi
			shr		edx, 2
			lea		edx, [eax+edx+4]
			mov		eax, [edx]
			test	eax, eax
			jz		newSection
			mov		ecx, [eax]
			mov		[edx], ecx
			mov		s_memoryPool.m_cs.selfPtr, 0
			pop		esi
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
			push	edi
			push	edx
			mov		edi, s_memoryPool.m_freeSections
			test	edi, edi
			jz		newPool
			mov		eax, [edi]
			mov		s_memoryPool.m_freeSections, eax
			jmp		gotSection
			NOP_0x8
		newPool:
			push	MEMORY_POOL_SIZE+0x40
			call	malloc
			add		eax, 0x20
			and		eax, 0xFFFFFFF0
			mov		edi, eax
			lea		edx, [eax+MEMORY_POOL_SIZE-POOL_SECTION_SIZE]
			add		eax, POOL_SECTION_SIZE
			mov		s_memoryPool.m_freeSections, eax
		sectLinker:
			mov		ecx, eax
			add		eax, POOL_SECTION_SIZE
			mov		[ecx], eax
			cmp		eax, edx
			jb		sectLinker
			pop		ecx
			xor		ecx, ecx
			mov		[eax], ecx
		gotSection:
			pop		ecx
			lea		eax, [esi+edi]
			mov		[ecx], eax
			lea		edx, [edi+POOL_SECTION_SIZE]
			sub		edx, esi
		blockLinker:
			mov		ecx, eax
			add		eax, esi
			mov		[ecx], eax
			cmp		eax, edx
			jbe		blockLinker
			mov		dword ptr [ecx], 0
			add		edx, esi
			sub		edx, eax
			jbe		done
			mov		esi, edx
			shr		esi, 8
			setnz	cl
			add		cl, 4
			shr		edx, cl
			mov		esi, 1
			shl		esi, cl
			push	eax
			ALIGN 16
		surplIter:
			mov		ecx, eax
			add		eax, esi
			mov		[ecx], eax
			dec		edx
			ja		surplIter
			shr		esi, 2
			lea		eax, s_memoryPool.m_freeSections[esi]
			mov		edx, [eax]
			mov		[ecx], edx
			pop		dword ptr [eax]
		done:
			mov		s_memoryPool.m_cs.selfPtr, 0
			mov		eax, edi
			pop		edi
			pop		esi
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