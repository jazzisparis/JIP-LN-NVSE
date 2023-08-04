#include "nvse/GameBSExtraData.h"
#include "nvse/GameExtraData.h"

bool BaseExtraList::HasType(UInt32 type) const
{
	return (m_presenceBitfield[type >> 3] & (1 << (type & 7))) != 0;
}

__declspec(naked) void __fastcall BaseExtraList::SetTypePresent(UInt32 type, bool present)
{
	__asm
	{
		mov		eax, edx
		shr		eax, 5
		lea		ecx, [ecx+eax*4+8]
		mov		eax, [ecx]
		cmp		[esp+4], 0
		jz		unset
		bts		eax, edx
		mov		[ecx], eax
		retn	4
	unset:
		btr		eax, edx
		mov		[ecx], eax
		retn	4
	}
}

__declspec(naked) BSExtraData *BaseExtraList::GetByType(UInt32 xType) const
{
	__asm
	{
		cmp		dword ptr [ecx+4], 0
		jz		retnNULL
		mov		edx, [esp+4]
		shr		edx, 5
		mov		eax, [ecx+edx*4+8]
		mov		edx, [esp+4]
		bt		eax, edx
		jnc		retnNULL
		push	ecx
		mov		ecx, EXTRA_DATA_CS
		call	LightCS::Enter
		pop		ecx
		mov		eax, [ecx+4]
		mov		edx, [esp+4]
		ALIGN 16
	iterHead:
		cmp		[eax+4], dl
		jz		lockLeave
		mov		eax, [eax+8]
		test	eax, eax
		jnz		iterHead
	lockLeave:
		mov		edx, EXTRA_DATA_CS
		dec		dword ptr [edx+4]
		jnz		done
		and		dword ptr [edx], 0
	done:
		retn	4
	retnNULL:
		xor		eax, eax
		retn	4
	}
}

__declspec(naked) ExtraDataList *ExtraDataList::Create()
{
	__asm
	{
		push	0x20
		call	Game_DoHeapAlloc
		xorps	xmm0, xmm0
		movups	[eax], xmm0
		movups	[eax+0x10], xmm0
		mov		dword ptr [eax], kVtbl_ExtraDataList
		retn
	}
}

__declspec(naked) char __fastcall BaseExtraList::GetExtraFactionRank(TESFaction *faction) const
{
	__asm
	{
		push	edx
		push	kXData_ExtraFactionChanges
		call	BaseExtraList::GetByType
		pop		edx
		test	eax, eax
		jz		noRank
		mov		eax, [eax+0xC]
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		noRank
		mov		ecx, [eax]
		mov		eax, [eax+4]
		test	ecx, ecx
		jz		iterHead
		cmp		[ecx], edx
		jnz		iterHead
		mov		al, [ecx+4]
		retn
	noRank:
		dec		al
		retn
	}
}

SInt32 BaseExtraList::GetCount() const
{
	ExtraCount *xCount = GetExtraType(this, ExtraCount);
	return xCount ? xCount->count : 1;
}

void __fastcall ExtraValueStr(BSExtraData *xData, char *buffer)
{
	switch (xData->type)
	{
		case kXData_ExtraOwnership:
		{
			ExtraOwnership *xOwnership = (ExtraOwnership*)xData;
			sprintf_s(buffer, 0x20, "%08X", xOwnership->owner ? xOwnership->owner->refID : 0);
			break;
		}
		case kXData_ExtraCount:
		{
			ExtraCount *xCount = (ExtraCount*)xData;
			sprintf_s(buffer, 0x20, "%d", xCount->count);
			break;
		}
		default:
			sprintf_s(buffer, 0x20, "%08X", ((UInt32*)xData)[3]);
			break;
	}
}

void BaseExtraList::DebugDump() const
{
	PrintDebug("\nBaseExtraList Dump:");
	Console_Print("BaseExtraList Dump:");
	s_debug().Indent();
	if (m_data)
	{
		char dataStr[0x20];
		for (BSExtraData *traverse = m_data; traverse; traverse = traverse->next)
		{
			ExtraValueStr(traverse, dataStr);
			PrintDebug("%08X\t%02X\t%s\t%s", traverse, traverse->type, GetExtraDataName(traverse->type), dataStr);
			Console_Print("%08X  %02X  %s  %s", traverse, traverse->type, GetExtraDataName(traverse->type), dataStr);
		}
		Console_Print(" ");
	}
	else
	{
		PrintDebug("No data in list");
		Console_Print("No data in list");
	}
	s_debug().Outdent();
}
