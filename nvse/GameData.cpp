#include "nvse/GameData.h"

class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo)
	{
		return !StrCompare(modInfo->name, m_stringToFind);
	}
};

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}

const ModInfo ** DataHandler::GetActiveModList()
{
	static const ModInfo *activeModList[0x100] = {NULL};

	if (!activeModList[0])
	{
		UInt32 index = 0;
		auto iter = modList.modInfoList.Head();
		do
		{
			activeModList[index++] = iter->data;
		}
		while (iter = iter->next);
	}

	return activeModList;
}

UInt8 DataHandler::GetModIndex(const char *modName)
{
	ListNode<ModInfo> *iter = modList.modInfoList.Head();
	ModInfo *modInfo;
	do
	{
		modInfo = iter->data;
		if (modInfo && !StrCompare(modInfo->name, modName))
			return modInfo->modIndex;
	}
	while (iter = iter->next);
	return 0xFF;
}

const char* DataHandler::GetNthModName(UInt32 modIndex)
{
	const ModInfo** activeModList = GetActiveModList();
	if (modIndex < GetActiveModCount() && activeModList[modIndex])
		return activeModList[modIndex]->name;
	else
		return "";
}

void Sky::RefreshMoon()
{
	if (masserMoon) masserMoon->Destroy(true);
	masserMoon = (Moon*)GameHeapAlloc(sizeof(Moon));
	ThisCall(0x634A70, masserMoon, (const char*)0x104EEB0, *(UInt32*)0x11CCCBC, *(UInt32*)0x11CCC98, *(UInt32*)0x11CCBA8, *(UInt32*)0x11CCC00, *(UInt32*)0x11CCC58, *(UInt32*)0x11CCC1C);
	masserMoon->Refresh(niNode008, (const char*)0x104EEB0);
}

__declspec(naked) bool Sky::GetIsRaining()
{
	__asm
	{
		mov		eax, [ecx+0x10]
		test	eax, eax
		jz		checkSecond
		cmp		byte ptr [eax+0xEB], 4
		jz		weatherPerc
	checkSecond:
		mov		eax, [ecx+0x14]
		test	eax, eax
		jz		retnFalse
		cmp		byte ptr [eax+0xEB], 4
		jnz		retnFalse
	weatherPerc:
		movss	xmm0, kFltOne
		comiss	xmm0, [ecx+0xF4]
		setbe	al
		retn
	retnFalse:
		xor		al, al
		retn
	}
}