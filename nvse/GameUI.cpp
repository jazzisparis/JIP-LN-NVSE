#include "nvse/GameUI.h"

InterfaceManager *g_interfaceManager = NULL;
bool *g_menuVisibility = (bool*)0x11F308F;
TileMenu **g_tileMenuArray = NULL;

HUDMainMenu *g_HUDMainMenu = NULL;
RecipeMenu **g_recipeMenu = (RecipeMenu**)0x11D8E90;
BarterMenu **g_barterMenu = (BarterMenu**)0x11D8FA4;
CaravanMenu **g_caravanMenu = (CaravanMenu**)0x11D917C;
CompanionWheelMenu **g_companionWheelMenu = (CompanionWheelMenu**)0x11D92B8;
ComputersMenu **g_computersMenu = (ComputersMenu**)0x11D9334;
ContainerMenu **g_containerMenu = (ContainerMenu**)0x11D93F8;
DialogMenu **g_dialogMenu = (DialogMenu**)0x11D9510;
HackingMenu **g_hackingMenu = (HackingMenu**)0x11D95B8;
InventoryMenu **g_inventoryMenu = (InventoryMenu**)0x11D9EA4;
ItemModMenu **g_itemModMenu = (ItemModMenu**)0x11D9F54;
LockPickMenu **g_lockPickMenu = (LockPickMenu**)0x11DA204;
MapMenu **g_mapMenu = (MapMenu**)0x11DA368;
MessageMenu **g_messageMenu = (MessageMenu**)0x11DA4F0;
QuantityMenu **g_quantityMenu = (QuantityMenu**)0x11DA618;
RaceSexMenu **g_raceSexMenu = (RaceSexMenu**)0x11DA634;
RepairMenu **g_repairMenu = (RepairMenu**)0x11DA75C;
RepairServicesMenu **g_repairServicesMenu = (RepairServicesMenu**)0x11DA7F0;
SleepWaitMenu **g_sleepWaitMenu = (SleepWaitMenu**)0x11DA920;
StartMenu **g_startMenu = (StartMenu**)0x11DAAC0;
StatsMenu **g_statsMenu = (StatsMenu**)0x11DACE0;
TextEditMenu **g_textEditMenu = (TextEditMenu**)0x11DAEC4;
VATSMenu **g_VATSMenu = (VATSMenu**)0x11DB0D4;
TraitMenu **g_traitMenu = (TraitMenu**)0x11DAF74;
TraitSelectMenu **g_traitSelectMenu = (TraitSelectMenu**)0x11DB00C;

ContChangesEntry **g_barterMenuSelection = (ContChangesEntry**)0x11D8FA8;
ContChangesEntry **g_containerMenuSelection = (ContChangesEntry**)0x11D93FC;
ContChangesEntry **g_inventoryMenuSelection = (ContChangesEntry**)0x11D9EA8;
ContChangesEntry **g_modMenuTarget = (ContChangesEntry**)0x11D9F58;
ContChangesEntry **g_repairMenuTarget = (ContChangesEntry**)0x11DA760;
TESObjectREFR **g_VATSTargetRef = (TESObjectREFR**)0x11F21CC;
VATSCameraData *g_VATSCameraData = (VATSCameraData*)0x11F2250;
TESRecipe **g_recipeMenuSelection = (TESRecipe**)0x11D8E94;

FontManager *g_fontManager = NULL;
SystemColorManager *g_sysColorManager = NULL;

UnorderedMap<const char*, UInt32> s_menuNameToID({{"MessageMenu", kMenuType_Message}, {"InventoryMenu", kMenuType_Inventory}, {"StatsMenu", kMenuType_Stats},
	{"HUDMainMenu", kMenuType_HUDMain}, {"LoadingMenu", kMenuType_Loading}, {"ContainerMenu", kMenuType_Container}, {"DialogMenu", kMenuType_Dialog},
	{"SleepWaitMenu", kMenuType_SleepWait}, {"StartMenu", kMenuType_Start}, {"LockpickMenu", kMenuType_LockPick}, {"QuantityMenu", kMenuType_Quantity},
	{"MapMenu", kMenuType_Map}, {"BookMenu", kMenuType_Book}, {"LevelUpMenu", kMenuType_LevelUp}, {"RepairMenu", kMenuType_Repair},
	{"RaceSexMenu", kMenuType_RaceSex}, {"CharGenMenu", kMenuType_CharGen}, {"TextEditMenu", kMenuType_TextEdit}, {"BarterMenu", kMenuType_Barter},
	{"SurgeryMenu", kMenuType_Surgery}, {"HackingMenu", kMenuType_Hacking}, {"VATSMenu", kMenuType_VATS}, {"ComputersMenu", kMenuType_Computers},
	{"RepairServicesMenu", kMenuType_RepairServices}, {"TutorialMenu", kMenuType_Tutorial}, {"SpecialBookMenu", kMenuType_SpecialBook},
	{"ItemModMenu", kMenuType_ItemMod}, {"LoveTesterMenu", kMenuType_LoveTester}, {"CompanionWheelMenu", kMenuType_CompanionWheel},
	{"TraitSelectMenu", kMenuType_TraitSelect}, {"RecipeMenu", kMenuType_Recipe}, {"SlotMachineMenu", kMenuType_SlotMachine},
	{"BlackjackMenu", kMenuType_Blackjack}, {"RouletteMenu", kMenuType_Roulette}, {"CaravanMenu", kMenuType_Caravan}, {"TraitMenu", kMenuType_Trait}});


TileMenu* __fastcall GetMenuTile(const char *componentPath)
{
	UInt32 menuID = s_menuNameToID.Get(componentPath);
	return menuID ? g_tileMenuArray[menuID - kMenuType_Min] : NULL;
}

Menu* __fastcall GetMenuByType(UInt32 menuID)
{
	menuID -= kMenuType_Min;
	if (menuID > 83) return NULL;
	TileMenu *tileMenu = g_tileMenuArray[menuID];
	return tileMenu ? tileMenu->menu : NULL;
}

UInt32 InterfaceManager::GetTopVisibleMenuID()
{
	if (currentMode < 2) return 0;
	if (activeMenu) return activeMenu->id;
	UInt32 *mnStack = &menuStack[1];
	while (*mnStack) mnStack++;
	UInt32 menuID = *(mnStack - 1);
	if (menuID != 1) return menuID;
	if (g_menuVisibility[kMenuType_Inventory])
		return kMenuType_Inventory;
	if (g_menuVisibility[kMenuType_Map])
		return kMenuType_Map;
	if (g_menuVisibility[kMenuType_Stats])
		return kMenuType_Stats;
	if (g_menuVisibility[kMenuType_Repair])
		return kMenuType_Repair;
	if (g_menuVisibility[kMenuType_ItemMod])
		return kMenuType_ItemMod;
	return 0;
}

Tile *InterfaceManager::GetActiveTile()
{
	return activeTile ? activeTile : activeTileAlt;
}

void InterfaceManager::ClearHighlights()
{
	highlightState = 0;
	for (UInt32 index = numHighlighted; index;)
		highlightedRefs[--index].Clear();
	numHighlighted = 0;
	flashingRefIndex = -1;
	highlightMain.Clear();
}

void InterfaceManager::AddHighlightedRef(TESObjectREFR *refr)
{
	for (UInt32 index = numHighlighted; index;)
		if (highlightedRefs[--index].refr == refr) return;
	if (numHighlighted < 32)
	{
		NiNode *refrNode = refr->GetNiNode();
		if (refrNode)
		{
			highlightedRefs[numHighlighted].Set(refr, refrNode);
			if (!numHighlighted)
				highlightMain.Set(&highlightedRefs[0]);
			highlightState = 1;
			numHighlighted++;
			byte490 = 0;
		}
	}
}

void InterfaceManager::RemoveHighlightedRef(TESObjectREFR *refr)
{
	for (UInt32 index = numHighlighted; index;)
	{
		if (highlightedRefs[--index].refr != refr)
			continue;
		if (--numHighlighted)
		{
			if (index == numHighlighted)
				highlightedRefs[index].Clear();
			else highlightedRefs[index].Replace(&highlightedRefs[numHighlighted]);
			if (highlightMain.refr == refr)
				highlightMain.Set(&highlightedRefs[0]);
		}
		else
		{
			highlightedRefs[0].Clear();
			highlightMain.Clear();
			highlightState = 0;
		}
		break;
	}
}

bool InterfaceManager::IsRefHighlighted(TESObjectREFR *refr)
{
	for (UInt32 index = numHighlighted; index;)
		if (highlightedRefs[--index].refr == refr)
			return true;
	return false;
}

__declspec(naked) TESObjectREFR *InterfaceManager::GetCursorPick()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x4C
		push	esi
		push	edi
		mov		esi, ecx
		lea     edi, [ebp-0x34]
		push	8
		push	0
		mov     ecx, edi
		CALL_EAX(0xE98F20)
		mov     ecx, edi
		mov		dword ptr [ecx], 1
		mov		byte ptr [ecx+0x11], 1
		mov		eax, g_TES
		push	dword ptr [eax+0xC]
		CALL_EAX(0x705FC0)
		push	0
		push	0
		push	0
		mov		eax, g_thePlayer
		push	dword ptr [eax+0x670]
		mov		ecx, ds:[0x11DEB7C]
		CALL_EAX(0xC52020)
		push	0
		lea     edx, [ebp-0x4C]
		push	edx
		lea     edx, [ebp-0x40]
		push	edx
		cvttss2si	edx, [esi+0x40]
		push	edx
		cvttss2si	edx, [esi+0x38]
		push	edx
		mov		ecx, ds:[0x11DEB7C]
		mov		ecx, [ecx+0xAC]
		CALL_EAX(0xA71080)
		push	0
		lea     edx, [ebp-0x4C]
		push	edx
		lea     edx, [ebp-0x40]
		push	edx
		mov     ecx, edi
		CALL_EAX(0xE98E20)
		xor		esi, esi
		test	al, al
		jz		done
		cmp		dword ptr [edi+0x28], 0
		jz		done
		mov		eax, [edi+0x1C]
		mov		eax, [eax]
		push	dword ptr [eax]
		CALL_EAX(0x56F930)
		pop		ecx
		mov		esi, eax
	done:
		mov     ecx, edi
		CALL_EAX(0xE98FA0)
		mov		eax, esi
		pop		edi
		pop		esi
		leave
		retn
	}
}

bool __fastcall IsMenuMode(UInt32 menuID)
{
	switch (menuID)
	{
		case 0:
			return g_interfaceManager->currentMode != 1;
		case 1:
			if (*(bool*)0x11CAB24)
				return g_menuVisibility[kMenuType_Inventory] || g_menuVisibility[kMenuType_Stats] || g_menuVisibility[kMenuType_Map];
			return g_interfaceManager->pipBoyMode == 3;
		case 2:
		{
			for (UInt32 pMenuID : g_interfaceManager->menuStack)
			{
				if (pMenuID >= kMenuType_Min) return true;
				if (!pMenuID) break;
			}
			return false;
		}
		case 3:
			return *(bool*)0x11DEA2E;
		case 4:
			return g_menuVisibility[kMenuType_Loading] && g_menuVisibility[kMenuType_Start] && g_menuVisibility[kMenuType_HUDMain];
		default:
			if ((menuID >= kMenuType_Min) && (menuID <= kMenuType_Max))
				return g_menuVisibility[menuID];
			return false;
	}
}

__declspec(naked) void SystemColorManager::SystemColor::SetColorRGB(UInt32 r, UInt32 g, UInt32 b)
{
	__asm
	{
		xor		eax, eax
		mov		ah, [esp+4]
		mov		al, [esp+8]
		shl		eax, 0x10
		mov		ah, [esp+0xC]
		mov		al, 0xFF
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+8]
		retn	0xC
	}
}

DebugText *DebugText::GetSingleton()
{
	return CdeclCall<DebugText*>(0xA0D9E0, true);
}

DebugText::DebugLine *DebugText::GetDebugInput()
{
	DebugLine *linesPtr = lines, *result = lines;
	UInt32 counter = 200;
	do
	{
		linesPtr++;
		if (!linesPtr->isVisible) break;
		if (result->offsetY < linesPtr->offsetY)
			result = linesPtr;
	}
	while (--counter);
	return result;
}

FontHeightData s_fontHeightDatas[90];