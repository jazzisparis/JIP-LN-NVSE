#include "nvse/GameUI.h"

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

__declspec(naked) UInt32 InterfaceManager::GetTopVisibleMenuID()
{
	__asm
	{
		cmp		byte ptr [ecx+0xC], 2
		jb		retn0
		mov		eax, [ecx+0xD0]
		add		ecx, 0x114
		test	eax, eax
		jz		stackIter
		mov		eax, [eax+0x20]
		retn
		ALIGN 16
	stackIter:
		add		ecx, 4
		cmp		[ecx], 0
		jnz		stackIter
		mov		eax, [ecx-4]
		cmp		eax, 1
		jnz		done
		mov		ecx, 0x11F3479
		mov		eax, 0x3EA
		cmp		[ecx], 0
		jnz		done
		mov		al, 0xFF
		cmp		[ecx+0x15], 0
		jnz		done
		mov		al, 0xEB
		cmp		[ecx+1], 0
		jnz		done
		mov		eax, 0x40B
		cmp		[ecx+0x21], 0
		jnz		done
		mov		al, 0x25
		cmp		[ecx+0x3B], 0
		jnz		done
	retn0:
		xor		eax, eax
	done:
		retn
	}
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
				return MENU_VISIBILITY[kMenuType_Inventory] || MENU_VISIBILITY[kMenuType_Stats] || MENU_VISIBILITY[kMenuType_Map];
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
			return MENU_VISIBILITY[kMenuType_Loading] && MENU_VISIBILITY[kMenuType_Start] && MENU_VISIBILITY[kMenuType_HUDMain];
		default:
			if ((menuID >= kMenuType_Min) && (menuID <= kMenuType_Max))
				return MENU_VISIBILITY[menuID];
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