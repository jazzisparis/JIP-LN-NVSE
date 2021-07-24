#pragma once

void* __fastcall Pool_Alloc(UInt32 size);
void __fastcall Pool_Free(void *pBlock, UInt32 size);
void* __fastcall Pool_Realloc(void *pBlock, UInt32 curSize, UInt32 reqSize);

#define POOL_ALLOC(count, type) (type*)Pool_Alloc(count * sizeof(type))
#define POOL_FREE(block, count, type) Pool_Free(block, count * sizeof(type))
#define POOL_REALLOC(block, curCount, newCount, type) (type*)Pool_Realloc(block, curCount * sizeof(type), newCount * sizeof(type))