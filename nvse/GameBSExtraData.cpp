#include "nvse/GameBSExtraData.h"
#include "nvse/GameExtraData.h"

const _GetExtraData GetExtraData = (_GetExtraData)0x410220;
const _AddExtraData AddExtraData = (_AddExtraData)0x40FF60;
const _RemoveExtraData RemoveExtraData = (_RemoveExtraData)0x410020;
const _RemoveExtraType RemoveExtraType = (_RemoveExtraType)0x410140;
const _ClearExtraDataList ClearExtraDataList = (_ClearExtraDataList)0x40FAE0;
const _CopyExtraDataList CopyExtraDataList = (_CopyExtraDataList)0x411EC0;

bool BaseExtraList::HasType(UInt32 type) const
{
	return (m_presenceBitfield[type >> 3] & (1 << (type & 7))) != 0;
}

void BaseExtraList::MarkType(UInt32 type, bool bCleared)
{
	UInt8 bitMask = 1 << (type & 7);
	UInt8 &flag = m_presenceBitfield[type >> 3];
	if (bCleared) flag &= ~bitMask;
	else flag |= bitMask;
}

ExtraDataList *ExtraDataList::Create(BSExtraData *xBSData)
{
	ExtraDataList *xData = (ExtraDataList*)GameHeapAlloc(sizeof(ExtraDataList));
	MemZero(xData, sizeof(ExtraDataList));
	*(UInt32*)xData = kVtbl_ExtraDataList;
	if (xBSData) AddExtraData(xData, xBSData);
	return xData;
}

bool BaseExtraList::IsWorn() const
{
	return HasType(kExtraData_Worn);
}

char BaseExtraList::GetExtraFactionRank(TESFaction *faction) const
{
	ExtraFactionChanges *xFactionChanges = GetExtraType(this, FactionChanges);
	if (xFactionChanges && xFactionChanges->data)
	{
		ListNode<FactionListData> *traverse = xFactionChanges->data->Head();
		FactionListData *pData;
		do
		{
			pData = traverse->data;
			if (pData && (pData->faction == faction))
				return pData->rank;
		}
		while (traverse = traverse->next);
	}
	return -1;
}

SInt32 BaseExtraList::GetCount() const
{
	ExtraCount *xCount = GetExtraType(this, Count);
	return xCount ? xCount->count : 1;
}

void __fastcall ExtraValueStr(BSExtraData *xData, char *buffer)
{
	switch (xData->type)
	{
		case kExtraData_Ownership:
		{
			ExtraOwnership *xOwnership = (ExtraOwnership*)xData;
			sprintf_s(buffer, 0x20, "%08X", xOwnership->owner ? xOwnership->owner->refID : 0);
			break;
		}
		case kExtraData_Count:
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
	s_debug.Indent();
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
	s_debug.Outdent();
}

bool BaseExtraList::MarkScriptEvent(UInt32 eventMask, TESForm* eventTarget)
{
	return MarkBaseExtraListScriptEvent(eventTarget, this, eventMask);
}
