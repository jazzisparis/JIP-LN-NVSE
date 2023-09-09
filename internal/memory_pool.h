#pragma once

#define MAX_BLOCK_SIZE		0x800UL
#define POOL_SECTION_SIZE	0x1000UL
#define MEMORY_POOL_SIZE	0x40000UL

struct MemoryPool
{
	struct BlockNode
	{
		BlockNode	*m_next;
		//	Data
	};

	PrimitiveCS		m_cs;
	BlockNode		*m_freeSections = nullptr;
	BlockNode		*m_sections[MAX_BLOCK_SIZE >> 4] = {nullptr};
	size_t			m_allocPoolCount = 0;

	static void* __fastcall Alloc(size_t size);
	static void __fastcall Free(void *pBlock, size_t size);
	static void* __fastcall Realloc(void *pBlock, size_t curSize, size_t reqSize);

	static size_t GetTotalAllocSize();
};

template <typename T> consteval size_t AlignAlloc()
{
	return (sizeof(T) & 0xF) ? ((sizeof(T) & 0xFFFFFFF0) + 0x10) : sizeof(T);
}

template <typename T> __forceinline size_t AlignNumAlloc(size_t numAlloc)
{
	switch (sizeof(T) & 0xF)
	{
		case 0:
			return numAlloc;
		case 2:
		case 6:
		case 0xA:
		case 0xE:
			return (numAlloc & 7) ? ((numAlloc & 0xFFFFFFF8) + 8) : numAlloc;
		case 4:
		case 0xC:
			return (numAlloc & 3) ? ((numAlloc & 0xFFFFFFFC) + 4) : numAlloc;
		case 8:
			return (numAlloc & 1) ? (numAlloc + 1) : numAlloc;
		default:
			return (numAlloc & 0xF) ? ((numAlloc & 0xFFFFFFF0) + 0x10) : numAlloc;
	}
}

template <typename T> __forceinline T* Pool_Alloc()
{
	return (T*)MemoryPool::Alloc(AlignAlloc<T>());
}

template <typename T> __forceinline void Pool_Free(void *block)
{
	MemoryPool::Free(block, AlignAlloc<T>());
}

template <typename T = char> __forceinline T* Pool_CAlloc(size_t count = 1)
{
	return (T*)MemoryPool::Alloc(count * sizeof(T));
}

template <typename T = char> __forceinline void Pool_CFree(void *block, size_t count = 1)
{
	MemoryPool::Free(block, count * sizeof(T));
}

template <typename T = char> __forceinline T* Pool_CRealloc(void *block, size_t curCount, size_t newCount)
{
	return (T*)MemoryPool::Realloc(block, curCount * sizeof(T), newCount * sizeof(T));
}