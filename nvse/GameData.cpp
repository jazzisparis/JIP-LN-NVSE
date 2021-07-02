#include "nvse/GameData.h"

DataHandler *g_dataHandler = NULL;
Sky **g_tempSky = (Sky**)0x11CCB78, **g_currentSky = (Sky**)0x11DEA20;
TES *g_TES = NULL;
GameTimeGlobals *g_gameTimeGlobals = (GameTimeGlobals*)0x11DE7B8;
LoadedReferenceMap **g_loadedRefrMaps = (LoadedReferenceMap**)0x11C95C8;
RadioEntry **g_pipboyRadio = (RadioEntry**)0x11DD42C;
EffectArchTypeEntry *g_effectArchTypeArray = (EffectArchTypeEntry*)0x1183320;				//	Array size = 0x25
EntryPointConditionInfo *g_entryPointConditionInfo = (EntryPointConditionInfo*)0x1196EE0;	//	Array size = 0x49
AnimGroupInfo *g_animGroupInfoArray = (AnimGroupInfo*)0x11977D8;							//	Array size = 0xF5
PCMiscStat **g_miscStatData = (PCMiscStat**)0x11C6D50;										//	Array size = 0x2B
TypeSignature *g_typeSignatures = (TypeSignature*)0x101C2AC;								//	Array size = 0x79; order is reversed.

DataHandler* DataHandler::Get() {
	DataHandler** g_dataHandler = (DataHandler**)0x011C3F2C;
	return *g_dataHandler;
}

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
	static const ModInfo* activeModList[0x100] = { 0 };

	if (!(*activeModList))
	{
		UInt16 index = 0;
		for (index = 0  ; index < DataHandler::Get()->modList.modInfoList.Count() ; index++)
		{
			ModInfo* entry = DataHandler::Get()->modList.modInfoList.GetNthItem(index);
			if (entry->IsLoaded())
				activeModList[index] = entry;
		}
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

struct IsModLoaded
{
	bool Accept(ModInfo* pModInfo) const {
		return pModInfo->IsLoaded();
	}
};

UInt8 DataHandler::GetActiveModCount() const
{
	return modList.modInfoList.Count();
}

ModInfo::ModInfo() {
	//
};

ModInfo::~ModInfo() {
	//
};

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