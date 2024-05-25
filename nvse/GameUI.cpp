#include "nvse/GameUI.h"

TempObject<UnorderedMap<const char*, UInt32, 0x20, false>> s_menuNameToID;

UInt32 g_valueID_enabled = 0, g_valueID_numVisible = 0;

TileMenu* __fastcall GetMenuTile(const char *componentPath)
{
	if (UInt32 menuID = s_menuNameToID->Get(componentPath))
		return g_tileMenuArray[menuID];
	return nullptr;
}

Menu* __fastcall GetMenuByType(UInt32 menuID)
{
	if ((menuID >= kMenuType_Min) && (menuID <= kMenuType_Max))
		if (TileMenu *tileMenu = g_tileMenuArray[menuID])
			return tileMenu->menu;
	return nullptr;
}

__declspec(naked) UInt32 InterfaceManager::GetTopVisibleMenuID() const
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

Tile *InterfaceManager::GetActiveTile() const
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
		NiNode *refrNode = refr->GetRefNiNode();
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

bool InterfaceManager::IsRefHighlighted(TESObjectREFR *refr) const
{
	for (UInt32 index = numHighlighted; index;)
		if (highlightedRefs[--index].refr == refr)
			return true;
	return false;
}

__declspec(naked) NiAVObject *InterfaceManager::GetCursorPick() const
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
		mov		dword ptr [edi], 1
		mov		byte ptr [edi+0x11], 1
		mov		eax, g_TES
		mov		ecx, [eax+0xC]
		lock inc dword ptr [ecx+4]
		mov		[edi+0x14], ecx
		push	0
		push	0
		push	0
		mov		eax, g_thePlayer
		push	dword ptr [eax+0x670]
		mov		ecx, ds:0x11DEB7C
		CALL_EAX(0xC52020)
		lea     edx, [ebp-0x4C]
		lea     eax, [ebp-0x40]
		push	0
		push	edx
		push	eax
		push	0
		push	edx
		push	eax
		cvttss2si	edx, [esi+0x40]
		push	edx
		cvttss2si	edx, [esi+0x38]
		push	edx
		mov		ecx, g_mainCamera
		CALL_EAX(0xA71080)
		mov     ecx, edi
		CALL_EAX(0xE98E20)
		xor		esi, esi
		test	al, al
		jz		done
		cmp		dword ptr [edi+0x28], 0
		jz		done
		mov		esi, [edi+0x1C]
		mov		eax, [esi]
		mov		esi, [eax]
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

__declspec(noinline) bool __fastcall IsMenuMode(UInt32 menuID)
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
		if (!linesPtr->text.m_data) break;
		if (result->offsetY < linesPtr->offsetY)
			result = linesPtr;
	}
	while (--counter);
	return result;
}

FontHeightData s_fontHeightDatas[90];