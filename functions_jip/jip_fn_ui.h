#pragma once

DEFINE_COMMAND_PLUGIN(IsComponentLoaded, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(InjectUIXML, , 0, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(InjectUIComponent, , 0, 22, kParams_JIP_OneString_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetCursorPos, , 0, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(GetUIString, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetActiveMenuMode, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentName, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentFullName, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentID, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetMenuTargetRef, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMenuItemFilter, , 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ClickMenuButton, , 0, 2, kParams_JIP_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetSelectedItemRef, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetBarterItems, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetBarterGoldAlt, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuSelection, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuCategory, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(UnlockRecipeMenuQuantity, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuNumSubcategories, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(MessageExAlt, , 0, 22, kParams_JIP_OneFloat_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetFontFile, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetFontFile, , 0, 2, kParams_JIP_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(ShowTextInputMenu, , 0, 24, kParams_JIP_OneForm_TwoFloats_OneFormatString);
DEFINE_COMMAND_PLUGIN(SetTextInputExtendedProps, , 0, 5, kParams_JIP_TwoFloats_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(SetTextInputString, , 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(SetMessageDisabled, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetMessageDisabled, , 0, 1, kParams_JIP_OneMessage);
DEFINE_COMMAND_PLUGIN(GetMessageFlags, , 0, 1, kParams_JIP_OneMessage);
DEFINE_COMMAND_PLUGIN(SetMessageFlags, , 0, 2, kParams_JIP_OneMessage_OneInt);
DEFINE_COMMAND_PLUGIN(SetMessageDisplayTime, , 0, 2, kParams_JIP_OneMessage_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnMenuClickEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(SetOnMenuOpenEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnMenuCloseEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnMouseoverChangeEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RefreshItemsList, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetBarterPriceMult, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetBarterPriceMult, , 0, 2, kParams_JIP_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(SetTerminalUIModel, , 0, 2, kParams_JIP_OneForm_OneOptionalString);
DEFINE_COMMAND_PLUGIN(ShowQuantityMenu, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(MessageBoxExAlt, , 0, 22, kParams_JIP_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetVATSTargets, , 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(IsInCharGen, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(ToggleCraftingMessages, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetCursorPos, , 0, 2, kParams_TwoFloats);
DEFINE_COMMAND_PLUGIN(UnloadUIComponent, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ClearMessageQueue, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetSystemColor, , 0, 4, kParams_JIP_FourInts);
DEFINE_COMMAND_PLUGIN(SuppressQuestMessages, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(EnableImprovedRecipeMenu, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(ClickMenuTile, , 0, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(TogglePipBoyLight, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InitItemFilter, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetItemFilter, , 0, 1, kParams_JIP_OneOptionalString)
DEFINE_COMMAND_PLUGIN(ToggleHUDCursor, , 0, 1, kParams_OneOptionalInt)
DEFINE_COMMAND_PLUGIN(AddTileFromTemplate, , 0, 21, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(SetUIFloatGradual, , 0, 5, kParams_JIP_OneString_ThreeOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CloseActiveMenu, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ShowLevelUpMenuEx, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(AttachUIXML, , 1, 2, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(AttachUIComponent, , 1, 22, kParams_JIP_OneString_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetWorldMapPosMults, , 1, 2, kParams_JIP_TwoScriptVars);

bool Cmd_IsComponentLoaded_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath) && GetTargetComponent(tilePath))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_InjectUIXML_Execute(COMMAND_ARGS)
{
	*result = 0;
	char tilePath[0x100], xmlPath[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &xmlPath))
	{
		Tile *component = GetTargetComponent(tilePath);
		if (component) *result = component->ReadXML(xmlPath) ? 1 : 0;
	}
	return true;
}

Tile* __stdcall InjectUIComponent(Tile *parentTile, char *dataStr)
{
	FILE *tempXML = fopen("jip_temp.xml", "wb");
	if (!tempXML) return NULL;
	fputs(dataStr, tempXML);
	fclose(tempXML);
	return parentTile->ReadXML("jip_temp.xml");
}

bool Cmd_InjectUIComponent_Execute(COMMAND_ARGS)
{
	*result = 0;
	char tilePath[0x100], *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_InjectUIComponent.numParams, &tilePath))
	{
		Tile *component = GetTargetComponent(tilePath);
		*result = (component && InjectUIComponent(component, buffer)) ? 1 : 0;
	}
	return true;
}

bool Cmd_GetCursorPos_Execute(COMMAND_ARGS)
{
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
		*result = (axis == 'X') ? g_interfaceManager->cursorX : g_interfaceManager->cursorY;
	else *result = 0;
	return true;
}

bool Cmd_GetUIString_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath))
	{
		Tile::Value *value = NULL;
		if (GetTargetComponent(tilePath, &value) && value)
			resStr = value->str;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_GetActiveMenuMode_Execute(COMMAND_ARGS)
{
	*result = (int)g_interfaceManager->GetTopVisibleMenuID();
	return true;
}

bool Cmd_GetActiveUIComponentName_Execute(COMMAND_ARGS)
{
	const char *tileName = NULL;
	Tile *activeTile = g_interfaceManager->GetActiveTile();
	if (activeTile)
	{
		char tilePath[0x100];
		activeTile->GetComponentFullName(tilePath);
		tileName = SlashPosR(tilePath);
		if (tileName) tileName++;
	}
	AssignString(PASS_COMMAND_ARGS, tileName);
	return true;
}

bool Cmd_GetActiveUIComponentFullName_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	tilePath[0] = 0;
	Tile *activeTile = g_interfaceManager->GetActiveTile();
	if (activeTile) activeTile->GetComponentFullName(tilePath);
	AssignString(PASS_COMMAND_ARGS, tilePath);
	return true;
}

SInt32 GetActiveTileID()
{
	Tile *activeTile = g_interfaceManager->GetActiveTile();
	if (activeTile)
	{
		Tile::Value *val = activeTile->GetValue(kTileValue_id);
		if (val) return val->num;
	}
	return -1;
}

bool Cmd_GetActiveUIComponentID_Execute(COMMAND_ARGS)
{
	*result = GetActiveTileID();
	return true;
}

bool Cmd_GetMenuTargetRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 menuID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &menuID) || (menuID > kMenuType_Max)) return true;
	TileMenu *tileMenu = g_tileMenuArray[menuID - kMenuType_Min];
	Menu *menu = tileMenu ? tileMenu->menu : NULL;
	if (!menu) return true;
	TESForm *menuRef = NULL;
	switch (menuID)
	{
		case kMenuType_Container:
			menuRef = ((ContainerMenu*)menu)->containerRef;
			break;
		case kMenuType_Dialog:
		{
			menuRef = ((DialogMenu*)menu)->partnerRef;
			if (menuRef && IS_ID(((TESObjectREFR*)menuRef)->baseForm, BGSTalkingActivator))
			{
				ExtraTalkingActor *xTalker = GetExtraType(&((TESObjectREFR*)menuRef)->extraDataList, TalkingActor);
				if (xTalker) menuRef = xTalker->actor;
			}
			break;
		}
		case kMenuType_LockPick:
			menuRef = ((LockPickMenu*)menu)->targetRef;
			break;
		case kMenuType_Quantity:
		{
			if (MENU_VISIBILITY[kMenuType_Inventory]) menuRef = CreateRefForStack(g_thePlayer, InventoryMenu::Selection());
			else if (ContainerMenu::Get())
			{
				ContainerMenu *cntMenu = ContainerMenu::Get();
				menuRef = CreateRefForStack((cntMenu->currentItems == &cntMenu->leftItems) ? g_thePlayer : cntMenu->containerRef, ContainerMenu::Selection());
			}
			else if (BarterMenu::Get())
			{
				BarterMenu *brtMenu = BarterMenu::Get();
				menuRef = CreateRefForStack((brtMenu->currentItems == &brtMenu->leftItems) ? g_thePlayer : brtMenu->merchantRef->GetMerchantContainer(), BarterMenu::Selection());
			}
			break;
		}
		case kMenuType_Map:
		{
			MapMenu *mapMenu = (MapMenu*)menu;
			if (mapMenu->mapMarker)
			{
				static UInt32 valueID = 0;
				if (!valueID) valueID = TraitNameToID("_MarkerIndex");
				Tile::Value *markerIdx = mapMenu->mapMarker->GetValue(valueID);
				if (markerIdx) menuRef = mapMenu->mapMarkerList.GetNthItem(markerIdx->num);
			}
			break;
		}
		case kMenuType_Repair:
			menuRef = CreateRefForStack(g_thePlayer, RepairMenu::Target());
			break;
		case kMenuType_Barter:
			menuRef = ((BarterMenu*)menu)->merchantRef;
			break;
		case kMenuType_Hacking:
			menuRef = GetHackingMenuRef((HackingMenu*)menu);
			break;
		case kMenuType_VATS:
			menuRef = VATSMenu::Target();
			break;
		case kMenuType_Computers:
			menuRef = ((ComputersMenu*)menu)->targetRef;
			break;
		case kMenuType_ItemMod:
			menuRef = CreateRefForStack(g_thePlayer, ItemModMenu::Target());
			break;
		case kMenuType_CompanionWheel:
			menuRef = ((CompanionWheelMenu*)menu)->companionRef;
			break;
		case kMenuType_Recipe:
			menuRef = ((RecipeMenu*)menu)->sourceRef;
			break;
		case kMenuType_Caravan:
			menuRef = ((CaravanMenu*)menu)->opponentRef;
			break;
		case kMenuType_Trait:
		{
			TraitMenu *traitMenu = (TraitMenu*)menu;
			if (traitMenu->perkListBox.selected)
				menuRef = traitMenu->perkListBox.GetSelected();
		}
	}		
	if (menuRef) REFR_RES = menuRef->refID;
	return true;
}

bool Cmd_GetMenuItemFilter_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 menuID, useRef = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &menuID, &useRef)) return true;
	switch (menuID)
	{
		case kMenuType_Inventory:
			if (InventoryMenu::Get()) *result = (int)InventoryMenu::Get()->filter + 1;
			break;
		case kMenuType_Container:
			if (ContainerMenu::Get()) *result = useRef ? (int)ContainerMenu::Get()->rightFilter : (int)ContainerMenu::Get()->leftFilter;
			break;
		case kMenuType_Barter:
			if (BarterMenu::Get()) *result = useRef ? (int)BarterMenu::Get()->rightFilter : (int)BarterMenu::Get()->leftFilter;
	}
	return true;
}

bool Cmd_ClickMenuButton_Execute(COMMAND_ARGS)
{
	*result = 0;
	char tilePath[0x100];
	UInt32 times = 1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &times) || !GetMenuMode() || !times) return true;
	Tile *component = NULL;
	Menu *parentMenu = NULL;
	SInt32 tileID = -1;
	char *hashPos = FindChr(tilePath, '#');
	if (hashPos)
	{
		tileID = StrToInt(hashPos + 1);
		if (tileID >= 0)
		{
			*hashPos = 0;
			TileMenu *tileMenu = GetMenuTile(tilePath);
			if (tileMenu) parentMenu = tileMenu->menu;
		}
	}
	else
	{
		component = GetTargetComponent(tilePath);
		if (!component) return true;
		parentMenu = component->GetParentMenu();
		Tile::Value *tileVal = component->GetValue(kTileValue_id);
		if (tileVal) tileID = tileVal->num;
	}
	if (parentMenu)
	{
		while (times--)
			parentMenu->HandleClick(tileID, component);
		*result = 1;
	}
	return true;
}

bool Cmd_GetSelectedItemRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *itemRef = NULL;
	switch (g_interfaceManager->GetTopVisibleMenuID())
	{
		case kMenuType_Inventory:
		{
			InventoryMenu *invMenu = InventoryMenu::Get();
			if (invMenu->itemList.selected)
				itemRef = CreateRefForStack(g_thePlayer, InventoryMenu::Selection());
			break;
		}
		case kMenuType_Stats:
		{
			StatsMenu *statsMenu = StatsMenu::Get();
			if (statsMenu->perkRankList.selected)
				itemRef = statsMenu->perkRankList.GetSelected()->perk;
			break;
		}
		case kMenuType_Container:
		{
			ContainerMenu *cntMenu = ContainerMenu::Get();
			if (cntMenu->leftItems.selected || cntMenu->rightItems.selected)
				itemRef = CreateRefForStack(cntMenu->leftItems.selected ? g_thePlayer : cntMenu->containerRef, ContainerMenu::Selection());
			break;
		}
		case kMenuType_Map:
		{
			MapMenu *mapMenu = MapMenu::Get();
			if (mapMenu->questList.selected)
				itemRef = mapMenu->questList.GetSelected();
			else if (mapMenu->noteList.selected)
				itemRef = mapMenu->noteList.GetSelected();
			else if (mapMenu->challengeList.selected)
				itemRef = mapMenu->challengeList.GetSelected();
			break;
		}
		case kMenuType_Repair:
		{
			RepairMenu *rprMenu = RepairMenu::Get();
			if (rprMenu->repairItems.selected)
				itemRef = CreateRefForStack(g_thePlayer, rprMenu->repairItems.GetSelected());
			break;
		}
		case kMenuType_Barter:
		{
			BarterMenu *brtMenu = BarterMenu::Get();
			if (brtMenu->leftItems.selected || brtMenu->rightItems.selected)
				itemRef = CreateRefForStack(brtMenu->leftItems.selected ? g_thePlayer : brtMenu->merchantRef->GetMerchantContainer(), BarterMenu::Selection());
			break;
		}
		case kMenuType_RepairServices:
		{
			RepairServicesMenu *rpsMenu = RepairServicesMenu::Get();
			if (rpsMenu->itemList.selected)
				itemRef = CreateRefForStack(g_thePlayer, rpsMenu->itemList.GetSelected());
			break;
		}
		case kMenuType_ItemMod:
		{
			ItemModMenu *modMenu = ItemModMenu::Get();
			if (modMenu->itemModList.selected)
				itemRef = CreateRefForStack(g_thePlayer, modMenu->itemModList.GetSelected());
		}
	}
	if (itemRef) REFR_RES = itemRef->refID;
	return true;
}

bool Cmd_GetBarterItems_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 sold;
	BarterMenu *brtMenu = BarterMenu::Get();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &sold) || !brtMenu || !brtMenu->merchantRef) return true;
	TESObjectREFR *target = sold ? brtMenu->merchantRef->GetMerchantContainer() : g_thePlayer, *itemRef;
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	ListNode<ContChangesEntry> *iter = sold ? brtMenu->rightBarter.Head() : brtMenu->leftBarter.Head();
	do
	{
		if (itemRef = CreateRefForStack(target, iter->data))
			tmpElements->Append(itemRef);
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool Cmd_GetBarterGoldAlt_Execute(COMMAND_ARGS)
{
	*result = BarterMenu::Get() ? (int)BarterMenu::Get()->merchantGold : 0;
	return true;
}

bool Cmd_GetRecipeMenuSelection_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (RecipeMenu::Get() && RecipeMenu::Selection()) REFR_RES = RecipeMenu::Selection()->refID;
	return true;
}

bool Cmd_GetRecipeMenuCategory_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (RecipeMenu::Get()) REFR_RES = RecipeMenu::Get()->category->refID;
	return true;
}

bool Cmd_UnlockRecipeMenuQuantity_Execute(COMMAND_ARGS)
{
	UInt32 limit = 0x7FFFFFFF;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &limit)) SafeWrite32(0x727975, limit);
	return true;
}

bool Cmd_GetRecipeMenuNumSubcategories_Execute(COMMAND_ARGS)
{
	*result = (int)GameGlobals::RecipeMenuCategories()->size;
	return true;
}

const UInt32 kMsgIconsPathAddr[] = {0x10208A0, 0x10208E0, 0x1025CDC, 0x1030E78, 0x103A830, 0x1049638, 0x104BFE8};

bool Cmd_MessageExAlt_Execute(COMMAND_ARGS)
{
	float displayTime;
	char *buffer = GetStrArgBuffer();
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_MessageExAlt.numParams, &displayTime))
		return true;
	char *msgPtr = GetNextToken(buffer, '|');
	msgPtr[0x203] = 0;
	if (*msgPtr)
	{
		if ((buffer[0] == '#') && (buffer[1] >= '0') && (buffer[1] <= '6') && !buffer[2])
			QueueUIMessage(msgPtr, 0, (const char*)kMsgIconsPathAddr[buffer[1] - '0'], NULL, displayTime, 0);
		else QueueUIMessage(msgPtr, 0, buffer, NULL, displayTime, 0);
	}
	else QueueUIMessage(buffer, 0, NULL, NULL, displayTime, 0);
	return true;
}

bool Cmd_GetFontFile_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	UInt32 fontID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fontID) && fontID && (fontID <= 89) && (fontID != 9))
	{
		FontInfo *fontInfo = g_fontManager->fontInfos[fontID - 1];
		if (fontInfo) resStr = fontInfo->filePath;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetFontFile_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 fontID;
	char dataPathFull[0x100], *dataPath = dataPathFull + 5;
	*(UInt32*)dataPathFull = 'atad';
	dataPathFull[4] = '\\';
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &fontID, dataPath) || !fontID || (fontID > 89) || (fontID == 9) || !*dataPath) return true;
	FontInfo *fontInfo = s_fontInfosMap.Get(dataPath);
	if (!fontInfo)
	{
		if (!FileExists(dataPathFull)) return true;
		fontInfo = (FontInfo*)GameHeapAlloc(sizeof(FontInfo));
		fontInfo->Init(fontID, dataPath, true);
		if (!fontInfo->bufferData)
		{
			GameHeapFree(fontInfo);
			return true;
		}
		s_fontInfosMap[dataPath] = fontInfo;
	}
	g_fontManager->fontInfos[fontID - 1] = fontInfo;
	*result = 1;
	return true;
}

__declspec(naked) TextEditMenu* __stdcall ShowTextEditMenu(float width, float height, char *msgTitle, Script *callback)
{
	static const char altXMLPath[] = "Data\\NVSE\\plugins\\textinput\\texteditmenu.xml";
	__asm
	{
		push	esi
		push	edi
		CALL_EAX(0x703DA0)
		mov		esi, g_interfaceManager
		push	offset altXMLPath
		mov		ecx, [esi+0x9C]
		CALL_EAX(0xA01B00)
		mov		edi, [eax+0x3C]
		test	edi, edi
		jz		done
		mov		[edi+4], eax
		mov		dword ptr [edi+0x14], 0x41B
		push	0x41B
		mov		ecx, esi
		CALL_EAX(0x714D90)
		cmp		[esi+0xD0], edi
		jz		isActive
		mov		ecx, [esi+0xCC]
		test	ecx, ecx
		jz		resetActive
		push	1
		push	0
		push	kTileValue_mouseover
		CALL_EAX(kAddr_TileSetFloat)
	resetActive:
		mov		dword ptr [esi+0xCC], 0
		mov		dword ptr [esi+0xD0], 0
	isActive:
		mov		ds:[0x11DAEC4], edi
		CALL_EAX(0xA1DFB0)
		push	1
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_depth
		mov		ecx, [edi+4]
		CALL_EAX(kAddr_TileSetFloat)
		mov		eax, [esp+0xC]
		push	1
		push	eax
		push	kTileValue_user0
		mov		ecx, [edi+4]
		CALL_EAX(kAddr_TileSetFloat)
		mov		eax, [esp+0x10]
		push	1
		push	eax
		push	kTileValue_user1
		mov		ecx, [edi+4]
		CALL_EAX(kAddr_TileSetFloat)
		mov		esi, [esp+0x14]
		mov		[esi+0x50], 0
		cmp		[esi], 0
		jnz		hasTitle
		push	1
		push	0
		push	kTileValue_visible
		mov		ecx, [edi+0x30]
		CALL_EAX(kAddr_TileSetFloat)
	hasTitle:
		push	1
		push	esi
		push	kTileValue_string
		mov		ecx, [edi+0x30]
		CALL_EAX(kAddr_TileSetString)
		push	kTileValue_font
		mov		ecx, [edi+0x28]
		CALL_EAX(kAddr_TileGetFloat)
		fistp	dword ptr [edi+0x4C]
		mov		ecx, [edi+0x4C]
		mov		esi, offset s_fontHeightDatas
		lea		esi, [esi+ecx*8]
		push	1
		push	dword ptr [esi]
		push	kTileValue_user0
		mov		ecx, [edi+0x28]
		CALL_EAX(kAddr_TileSetFloat)
		push	1
		push	dword ptr [esi+4]
		push	kTileValue_user1
		mov		ecx, [edi+0x28]
		CALL_EAX(kAddr_TileSetFloat)
		push	0x400
		lea		ecx, [edi+0x34]
		call	String::Init
		push	0x400
		lea		ecx, [edi+0x3C]
		call	String::Init
		mov		ecx, [edi+0x3C]
		mov		word ptr [ecx], '|'
		inc		word ptr [edi+0x40]
		push	1
		push	dword ptr ds:[0x11D38BC]
		push	kTileValue_string
		mov		ecx, [edi+0x2C]
		CALL_EAX(kAddr_TileSetString)
		mov		dword ptr [edi+0x44], 0
		mov		dword ptr [edi+0x48], 0x7FFF0001
		mov		eax, [edi+0x28]
		mov		eax, [eax+0x28]
		mov		[edi+0x4C], eax
		mov		dword ptr [edi+0x54], 0x00000100
		mov		eax, [esp+0x18]
		mov		[edi+0x58], eax
		mov		ecx, edi
		call	TextInputRefreshHook
		push	0
		mov		ecx, edi
		CALL_EAX(0xA1DC20)
		push	1
		mov		ecx, offset s_hookInfos+kHook_TextInputClose*kHookInfoSize
		call	HookInfo::Set
		push	TextInputRefreshHook
		push	0x1070060
		call	SafeWrite32
		push	TextInputKeyPressHook
		push	0x1070064
		call	SafeWrite32
	done:
		mov		eax, edi
		pop		edi
		pop		esi
		retn	0x10
	}
}

bool Cmd_ShowTextInputMenu_Execute(COMMAND_ARGS)
{
	Script *callback;
	float width, height;
	char *msgTitle = GetStrArgBuffer();;
	if (!TextEditMenu::Get() && ExtractFormatStringArgs(3, msgTitle, EXTRACT_ARGS_EX, kCommandInfo_ShowTextInputMenu.numParams,
		&callback, &width, &height) && IS_ID(callback, Script) && ShowTextEditMenu(width, height, msgTitle, callback))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_SetTextInputExtendedProps_Execute(COMMAND_ARGS)
{
	float posX, posY;
	UInt32 minLength = 1, maxLength = 0, miscFlags = 0;
	TextEditMenu *textMenu = TextEditMenu::Get();
	if (textMenu && HOOK_INSTALLED(TextInputClose) && ExtractArgsEx(EXTRACT_ARGS_EX, &posX, &posY, &minLength, &maxLength, &miscFlags))
	{
		if (posX > 0)
			textMenu->tile->SetFloat(kTileValue_user2, posX);
		if (posY > 0)
			textMenu->tile->SetFloat(kTileValue_user3, posY);
		textMenu->minLength = minLength;
		if (maxLength)
			textMenu->maxLength = maxLength & 0x7FFF;
		else
			textMenu->maxLength = 0x7FFF;
		textMenu->miscFlags = miscFlags & 3;
	}
	return true;
}

bool Cmd_SetTextInputString_Execute(COMMAND_ARGS)
{
	TextEditMenu *textMenu = TextEditMenu::Get();
	if (textMenu && HOOK_INSTALLED(TextInputClose))
	{
		char *buffer = GetStrArgBuffer();
		if (ExtractFormatStringArgs(0, buffer, EXTRACT_ARGS_EX, kCommandInfo_SetTextInputString.numParams))
		{
			textMenu->currentText.Set(buffer);
			textMenu->cursorIndex = 0;
			textMenu->HandleKeyboardInput(0x80000006);
		}
	}
	return true;
}

bool Cmd_SetMessageDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 disable;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &disable)) return true;
	tList<TESForm> tempList(form);
	if IS_ID(form, BGSListForm)
		tempList = ((BGSListForm*)form)->list;
	ListNode<TESForm> *iter = tempList.Head();
	do
	{
		if (!(form = iter->data) || NOT_ID(form, BGSMessage)) continue;
		form->SetJIPFlag(kHookFormFlag6_MessageDisabled, disable != 0);
	}
	while (iter = iter->next);
	HOOK_SET(ShowMessage, true);
	HOOK_SET(ShowTutorial, true);
	return true;
}

bool Cmd_GetMessageDisabled_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &message) && (message->jipFormFlags6 & kHookFormFlag6_MessageDisabled)) *result = 1;
	else *result = 0;
	return true;
}

bool Cmd_GetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &message)) *result = message->msgFlags;
	else *result = 0;
	return true;
}

bool Cmd_SetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	UInt32 flags;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &message, &flags) && (flags <= 3))
		message->msgFlags = flags;
	return true;
}

bool Cmd_SetMessageDisplayTime_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	UInt32 displayTime;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &message, &displayTime))
		message->displayTime = displayTime;
	return true;
}

bool Cmd_SetOnMenuClickEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	char tilePath[0x100];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &tilePath) || NOT_ID(script, Script)) return true;
	char *slashPos = SlashPos(tilePath), *hashPos = NULL;
	if (slashPos) *slashPos = 0;
	else
	{
		hashPos = FindChr(tilePath, '#');
		if (hashPos) *hashPos = 0;
	}
	UInt32 menuID = s_menuNameToID.Get(tilePath);
	if (!menuID) return true;
	MenuClickEvent &clickEvent = s_menuClickEventMap[kMenuIDJumpTable[menuID - kMenuType_Min]];
	if (slashPos) *slashPos = '/';
	else if (hashPos)
	{
		int tileID = StrToInt(hashPos + 1);
		if (tileID < 0) return true;
		if (addEvnt)
		{
			if (clickEvent.Empty())
				clickEvent.SetHook(true);
			clickEvent.idsMap[tileID].Insert(script);
		}
		else if (!clickEvent.idsMap.Empty())
		{
			auto findID = clickEvent.idsMap.Find(tileID);
			if (findID)
			{
				findID().Erase(script);
				if (findID().Empty()) findID.Remove();
				if (clickEvent.Empty())
					clickEvent.SetHook(false);
			}
		}
		return true;
	}
	FixPath(tilePath);
	if (addEvnt)
	{
		bool match = false;
		if (!clickEvent.filtersMap.Empty())
		{
			for (auto filterb = clickEvent.filtersMap.FindOpDir(tilePath, false); filterb; --filterb)
			{
				if (!StrBeginsCS(tilePath, filterb.Key())) break;
				if (filterb().HasKey(script))
					return true;
			}
			char cmpr;
			for (auto filterf = clickEvent.filtersMap.FindOpDir(tilePath, true); filterf; ++filterf)
			{
				cmpr = StrBeginsCS(filterf.Key(), tilePath);
				if (!cmpr) break;
				if (cmpr == 2)
				{
					filterf().Insert(script);
					match = true;
				}
				else if (filterf().Erase(script) && filterf().Empty())
					filterf.Remove(clickEvent.filtersMap);
			}
		}
		if (!match)
		{
			if (clickEvent.Empty())
				clickEvent.SetHook(true);
			clickEvent.filtersMap[tilePath].Insert(script);
		}
	}
	else if (!clickEvent.filtersMap.Empty())
	{
		for (auto filterf = clickEvent.filtersMap.FindOpDir(tilePath, true); filterf; ++filterf)
		{
			if (!StrBeginsCS(filterf.Key(), tilePath)) break;
			filterf().Erase(script);
			if (filterf().Empty()) filterf.Remove(clickEvent.filtersMap);
		}
		if (clickEvent.Empty())
			clickEvent.SetHook(false);
	}
	return true;
}

UInt8 s_menuStateEventType = 0;

void SetOnMenuStateEvent(Script *script, bool doAdd, char idx)
{
	MenuStateCallbacks *callbacks = s_menuStateEventMap[idx];
	if (!callbacks)
	{
		if (!doAdd) return;
		callbacks = (MenuStateCallbacks*)malloc(sizeof(MenuStateCallbacks));
		new (callbacks) MenuStateCallbacks();
		s_menuStateEventMap[idx] = callbacks;
	}
	EventCallbackScripts *scripts = (s_menuStateEventType == 2) ? &callbacks->onMouseover : (s_menuStateEventType ? &callbacks->onClose : &callbacks->onOpen);
	if (doAdd)
	{
		if (scripts->HasKey(script)) return;
		if (s_menuStateEventType == 2)
		{
			if (scripts->Empty())
				HOOK_MOD(MenuHandleMouseover, true);
		}
		else if (callbacks->onOpen.Empty() && callbacks->onClose.Empty())
		{
			HOOK_MOD(MenuStateOpen, true);
			HOOK_MOD(MenuStateClose, true);
		}
		scripts->Insert(script);
	}
	else
	{
		if (!scripts->Erase(script)) return;
		if (s_menuStateEventType == 2)
		{
			if (scripts->Empty())
				HOOK_MOD(MenuHandleMouseover, false);
		}
		else if (callbacks->onOpen.Empty() && callbacks->onClose.Empty())
		{
			HOOK_MOD(MenuStateOpen, false);
			HOOK_MOD(MenuStateClose, false);
		}
	}
}

bool SetOnMenuStateEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt, menuID = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &menuID) && IS_ID(script, Script))
	{
		char idx;
		bool doAdd = (addEvnt != 0);
		if (!menuID)
		{
			for (idx = 0; idx < 36; idx++)
				SetOnMenuStateEvent(script, doAdd, idx);
		}
		else if ((menuID >= kMenuType_Min) && (menuID <= kMenuType_Max))
		{
			idx = kMenuIDJumpTable[menuID - kMenuType_Min];
			if (idx != -1) SetOnMenuStateEvent(script, doAdd, idx);
		}
	}
	return true;
}

__declspec(naked) bool Cmd_SetOnMenuOpenEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_menuStateEventType, 0
		jmp		SetOnMenuStateEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnMenuCloseEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_menuStateEventType, 1
		jmp		SetOnMenuStateEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnMouseoverChangeEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_menuStateEventType, 2
		jmp		SetOnMenuStateEventHandler_Execute
	}
}

__declspec(naked) void __fastcall RefreshRecipeMenu(RecipeMenu *menu)
{
	_asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		add		ecx, 0x6C
		mov		eax, [ecx]
		call	dword ptr [eax+0x1C]
		mov		ecx, esi
		push	dword ptr [ecx+0x64]
		push	0
		CALL_EAX(0x727680)
		add		esi, 0x6C
		mov		ecx, esi
		mov		edi, [ecx]
		push	0
		push	0
		call	dword ptr [edi+0x14]
		push	eax
		mov		ecx, esi
		call	dword ptr [edi]
		mov		ecx, esi
		call	dword ptr [edi+0x10]
		mov		ecx, esi
		CALL_EAX(0x7312E0)
		mov		eax, 0x727637
		push	dword ptr [eax]
		mov		ecx, esi
		CALL_EAX(0x729FE0)
		push	1
		mov		ecx, esi
		CALL_EAX(0x72A660)
		pop		edi
		pop		esi
		retn
	}
}

bool Cmd_RefreshItemsList_Execute(COMMAND_ARGS)
{
	if (MENU_VISIBILITY[kMenuType_Inventory])
		CdeclCall(0x782A90);
	else if (MENU_VISIBILITY[kMenuType_Stats])
		ThisCall(0x7DF230, StatsMenu::Get(), 4);
	else if (MENU_VISIBILITY[kMenuType_Container])
		ContainerMenu::Get()->Refresh(NULL);
	else if (MENU_VISIBILITY[kMenuType_Map])
	{
		MapMenu *mapMenu = MapMenu::Get();
		if (mapMenu->currentTab == MapMenu::kTab_WorldMap)
		{
			WriteRelJump(0x79DF73, 0x79E053);
			ThisCall(0x79DBB0, mapMenu);
			SAFE_WRITE_BUF(0x79DF73, "\x6A\x00\x68\xB1\x0F");
		}
	}
	else if (MENU_VISIBILITY[kMenuType_Barter])
		CdeclCall(0x730690, 1);
	else if (MENU_VISIBILITY[kMenuType_Recipe])
		RefreshRecipeMenu(RecipeMenu::Get());
	return true;
}

bool Cmd_GetBarterPriceMult_Execute(COMMAND_ARGS)
{
	UInt32 sellMult;
	BarterMenu *brtMenu = BarterMenu::Get();
	if (brtMenu && ExtractArgsEx(EXTRACT_ARGS_EX, &sellMult))
		*result = sellMult ? brtMenu->sellValueMult : brtMenu->buyValueMult;
	else *result = 0;
	return true;
}

bool Cmd_SetBarterPriceMult_Execute(COMMAND_ARGS)
{
	UInt32 sellMult;
	float valueMult;
	BarterMenu *brtMenu = BarterMenu::Get();
	if (brtMenu && ExtractArgsEx(EXTRACT_ARGS_EX, &sellMult, &valueMult))
	{
		if (sellMult) brtMenu->sellValueMult = valueMult;
		else brtMenu->buyValueMult = valueMult;
		CdeclCall(0x730690, 1);
	}
	return true;
}

void DoPurgePath(char *path)
{
	if (!path) return;
	if (*GameGlobals::TerminalModelPtr() == path)
	{
		PurgeTerminalModel();
		*GameGlobals::TerminalModelPtr() = g_terminalModelDefault;
	}
	free(path);
}

bool Cmd_SetTerminalUIModel_Execute(COMMAND_ARGS)
{
	TESForm *form;
	char modelPath[0x80];
	modelPath[0] = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form, &modelPath)) return true;
	bool bRemove = !modelPath[0];
	tList<TESForm> tempList(form);
	if IS_ID(form, BGSListForm)
		tempList = ((BGSListForm*)form)->list;
	ListNode<TESForm> *lstIter = tempList.Head();
	BGSTerminal *terminal;
	char **pathPtr;
	do
	{
		terminal = (BGSTerminal*)lstIter->data;
		if (!terminal || NOT_ID(terminal, BGSTerminal)) continue;
		if (bRemove) DoPurgePath(s_terminalAltModelsMap.GetErase(terminal));
		else
		{
			if (!s_terminalAltModelsMap.Insert(terminal, &pathPtr)) DoPurgePath(*pathPtr);
			*pathPtr = CopyString(modelPath);
		}
	}
	while (lstIter = lstIter->next);
	HOOK_SET(SetTerminalModel, !s_terminalAltModelsMap.Empty());
	return true;
}

Script *s_quantityMenuScript = NULL;

__declspec(naked) void QuantityMenuCallback(int count)
{
	__asm
	{
		mov		eax, s_quantityMenuScript
		test	eax, eax
		jz		done
		mov		s_quantityMenuScript, 0
		mov		ecx, [esp+4]
		push	ecx
		push	1
		push	0
		push	eax
		call	CallFunction
		add		esp, 0x10
	done:
		retn
	}
}

bool Cmd_ShowQuantityMenu_Execute(COMMAND_ARGS)
{
	Script *callback;
	int maxCount;
	if (!QuantityMenu::Get() && !s_quantityMenuScript && ExtractArgsEx(EXTRACT_ARGS_EX, &callback, &maxCount) && 
		IS_ID(callback, Script) && ShowQuantityMenu(maxCount, QuantityMenuCallback, maxCount))
		s_quantityMenuScript = callback;
	return true;
}

__declspec(naked) void MessageBoxCallback()
{
	__asm
	{
		mov		eax, s_messageBoxScript
		mov		s_messageBoxScript, 0
		mov		ecx, g_interfaceManager
		movzx	edx, byte ptr [ecx+0xE4]
		cmp		dl, 0xFF
		jz		done
		mov		byte ptr [ecx+0xE4], 0xFF
		test	eax, eax
		jz		done
		cmp		dword ptr [eax], kVtbl_Script
		jnz		done
		push	edx
		push	1
		push	0
		push	eax
		call	CallFunction
		add		esp, 0x10
	done:
		retn
	}
}

__declspec(naked) void __fastcall MessageBoxExAlt(char **msgStrings)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	0
	addButtons:
		mov		eax, [ecx]
		sub		ecx, 4
		test	eax, eax
		jz		doneButtons
		push	eax
		jmp		addButtons
	doneButtons:
		push	0
		push	0
		push	0x17
		push	0
		push	MessageBoxCallback
		push	dword ptr [ecx-4]
		push	0
		push	dword ptr [ecx]
		call	ShowMessageBox
		leave
		retn
	}
}

bool Cmd_MessageBoxExAlt_Execute(COMMAND_ARGS)
{
	Script *callback;
	char *buffer = GetStrArgBuffer();
	if (!s_messageBoxScript && ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_MessageBoxExAlt.numParams, &callback))
	{
		s_messageBoxScript = callback;
		char *msgStrings[0x102], **buttonPtr = msgStrings + 2, *delim = buffer;
		*buttonPtr = NULL;
		for (UInt32 count = 0xFF; count; count--)
		{
			delim = GetNextToken(delim, '|');
			if (!*delim) break;
			*++buttonPtr = delim;
		}
		if (!*buttonPtr) *++buttonPtr = "OK";
		if ((buffer[0] == '^') && (delim = FindChr(buffer + 1, '^')))
		{
			*delim = 0;
			*msgStrings = buffer + 1;
			msgStrings[1] = delim + 1;
		}
		else
		{
			*msgStrings = NULL;
			msgStrings[1] = buffer;
		}
		MessageBoxExAlt(buttonPtr);
	}
	return true;
}

bool Cmd_GetVATSTargets_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!VATSMenu::Get()) return true;
	TempElements *tmpElements = GetTempElements();
	tmpElements->Clear();
	ListNode<VATSTargetInfo> *iter = GameGlobals::VATSTargetList()->Head();
	VATSTargetInfo *targetInfo;
	do
	{
		targetInfo = iter->data;
		if (targetInfo && targetInfo->targetRef)
			tmpElements->Append(targetInfo->targetRef);
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	return true;
}

bool InCharGen()
{
	return g_tileMenuArray[kMenuType_CharGen - kMenuType_Min] || g_tileMenuArray[kMenuType_SpecialBook - kMenuType_Min] || 
		g_tileMenuArray[kMenuType_LoveTester - kMenuType_Min] || g_tileMenuArray[kMenuType_Trait - kMenuType_Min];
}

bool Cmd_IsInCharGen_Execute(COMMAND_ARGS)
{
	*result = InCharGen();
	return true;
}

bool Cmd_IsInCharGen_Eval(COMMAND_ARGS_EVAL)
{
	*result = InCharGen();
	return true;
}

bool s_craftingMessages = true;

bool Cmd_ToggleCraftingMessages_Execute(COMMAND_ARGS)
{
	*result = s_craftingMessages;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (s_craftingMessages == !toggle))
	{
		s_craftingMessages = !s_craftingMessages;
		SafeWriteBuf(0x728933, s_craftingMessages ? "\x8D\x4D\xCC\xE8\x75" : "\xE9\xC4\x00\x00\x00", 5);
	}
	return true;
}

bool Cmd_SetCursorPos_Execute(COMMAND_ARGS)
{
	float posX, posY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posX, &posY) && GetMenuMode())
	{
		g_interfaceManager->cursorX = posX;
		g_interfaceManager->cursorY = posY;
		g_cursorNode->m_localTranslate.x = (posX * g_screenResConvert) - g_screenWidth;
		g_cursorNode->m_localTranslate.z = g_screenHeight - (posY * g_screenResConvert);
	}
	return true;
}

bool Cmd_UnloadUIComponent_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath))
	{
		Tile *component = GetTargetComponent(tilePath);
		if (component) component->Destroy(true);
	}
	return true;
}

bool Cmd_ClearMessageQueue_Execute(COMMAND_ARGS)
{
	if (!g_HUDMainMenu->queuedMessages.Empty())
	{
		ListNode<HUDMainMenu::QueuedMessage> *headNode = g_HUDMainMenu->queuedMessages.Head(), *msgIter = headNode->next;
		while (msgIter)
		{
			GameHeapFree(msgIter->data);
			msgIter = msgIter->RemoveMe();
		}
		if (!g_HUDMainMenu->currMsgKey)
		{
			GameHeapFree(headNode->data);
			headNode->RemoveMe();
		}
		else if (headNode->next)
			headNode->RemoveNext();
	}
	return true;
}

bool Cmd_SetSystemColor_Execute(COMMAND_ARGS)
{
	UInt32 type, red, green, blue;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &red, &green, &blue) && type && (type <= 5))
	{
		DListNode<SystemColorManager::SystemColor> *colorNode = g_sysColorManager->sysColors.Head()->Advance(type - 1);
		if (colorNode && colorNode->data)
		{
			colorNode->data->SetColorRGB(red, green, blue);
			ThisCall(0xA0B350, g_interfaceManager->menuRoot, type, 0);
		}
	}
	return true;
}

__declspec(naked) void QueueQuestMsgHook(TESQuest *quest)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		ecx, g_thePlayer
		mov		edx, [ecx+0x6B8]
		test	byte ptr [eax+0x3C], 0x42
		jnz		clrCurrent
		test	edx, edx
		jnz		done
		push	eax
		jmp		setCurrent
	clrCurrent:
		cmp		eax, edx
		jnz		done
		push	0
	setCurrent:
		CALL_EAX(0x9529D0)
	done:
		retn
	}
}

bool s_showQuestMessages = true, *g_showChallengeUpdates = (bool*)0x11CB4D8;

void ToggleQuestMessages()
{
	s_showQuestMessages = !s_showQuestMessages;
	*g_showChallengeUpdates = s_showQuestMessages;
	if (s_showQuestMessages)
	{
		SAFE_WRITE_BUF(0x77A480, "\x55\x8B\xEC\x83\xEC");
		SafeWrite8(0x77A5B0, 0x55);
	}
	else
	{
		WriteRelJump(0x77A480, (UInt32)QueueQuestMsgHook);
		SafeWrite8(0x77A5B0, 0xC3);
	}
}

bool Cmd_SuppressQuestMessages_Execute(COMMAND_ARGS)
{
	if (s_showQuestMessages)
	{
		ToggleQuestMessages();
		MainLoopAddCallback(ToggleQuestMessages);
	}
	return true;
}

__declspec(naked) void RecipeMenuAcceptHook()
{
	__asm
	{
		push	kTileValue_target
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x44]
		CALL_EAX(0xA01230)
		test	al, al
		jz		done
		push	0x7FFFFFFF
		push	0x7284F0
		push	dword ptr ds:[0x11D8EA8]
		CALL_EAX(0x7ABA00)
	done:
		leave
		retn	8
	}
}

__declspec(naked) void RecipeMenuCloseHook()
{
	__asm
	{
		cmp		s_craftingMessages, 0
		jnz		doneSound
		push	0
		push	1
		push	dword ptr [ebp-0x20]
		CALL_EAX(0x8ADCF0)
		test	eax, eax
		jz		doneSound
		push	0
		push	0x121
		push	0
		push	eax
		lea		eax, [ebp-0x44]
		push	eax
		mov		ecx, g_thePlayer
		CALL_EAX(0x933270)
	doneSound:
		mov		esi, ds:[0x11D8E90]
		test	esi, esi
		jz		done
		push	1
		mov		ecx, g_thePlayer
		mov		ecx, [ecx+0x68]
		mov		eax, [ecx]
		call	dword ptr [eax+0x474]
		test	al, al
		jz		doRefresh
		mov		ecx, g_thePlayer
		CALL_EAX(0x950030)
	doRefresh:
		mov		ecx, esi
		call	RefreshRecipeMenu
		mov		ecx, g_interfaceManager
		mov		ecx, [ecx+0x28]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+0x2C]
		test	ecx, ecx
		jz		done
		add		ecx, 0x58
		xor		eax, eax
		mov		[ecx], eax
		mov		[ecx+8], eax
	done:
		mov		ecx, [ebp-0xC]
		mov		fs:0, ecx
		pop		ecx
		pop		esi
		leave
		retn
	}
}

bool Cmd_EnableImprovedRecipeMenu_Execute(COMMAND_ARGS)
{
	static bool enabled = false;
	if (!enabled)
	{
		enabled = true;
		SafeWrite32(0x727975, 0x7FFFFFFF);
		SafeWrite8(0x7274E1, 0xF);
		SAFE_WRITE_BUF(0x727675, "\x03\x03\x03\x03\x03\x03\x01");
		SafeWrite32(0x727660, (UInt32)RecipeMenuAcceptHook);
		WriteRelJump(0x728A0C, (UInt32)RecipeMenuCloseHook);
	}
	return true;
}

bool Cmd_ClickMenuTile_Execute(COMMAND_ARGS)
{
	*result = 0;
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath) && GetMenuMode())
	{
		Tile *component = GetTargetComponent(tilePath);
		if (component && (component->GetValueFloat(kTileValue_target) > 0))
		{
			component->FakeClick();
			*result = 1;
		}
	}
	return true;
}

__declspec(naked) void __stdcall TogglePipBoyLight(UInt32 turnON)
{
	__asm
	{
		push	esi
		mov		esi, g_pipBoyLight
		mov		ecx, g_thePlayer
		push	0
		cmp		dword ptr [esp+0xC], 0
		jz		turnOFF
		push	esi
		add		ecx, 0x88
		mov		eax, [ecx]
		call	dword ptr [eax]
		jmp		finish
	turnOFF:
		push	0
		add		esi, 0x18
		push	esi
		add		ecx, 0x94
		CALL_EAX(0x824400)
	finish:
		mov		esi, g_interfaceManager
		mov		esi, [esi+0x174]
		push	1
		push	dword ptr [esp+0xC]
		push	1
		mov		ecx, esi
		CALL_EAX(0x7FA310)
		push	1
		push	dword ptr [esp+0xC]
		push	0
		mov		ecx, esi
		CALL_EAX(0x7FA310)
		pop		esi
		retn	4
	}
}

bool Cmd_TogglePipBoyLight_Execute(COMMAND_ARGS)
{
	UInt32 turnON, currState = ThisCall<bool>(0x822B90, &g_thePlayer->magicTarget, &g_pipBoyLight->magicItem, 1);
	*result = (int)currState;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &turnON) && (turnON != currState))
		TogglePipBoyLight(turnON);
	return true;
}

char *s_itemFilterString = NULL;

__declspec(naked) bool __fastcall ShouldHideItem(TESForm *itemForm)
{
	__asm
	{
		mov		eax, ecx
		test	eax, eax
		jz		done
		mov		edx, s_itemFilterString
		mov		al, [edx]
		test	al, al
		jz		done
		mov		al, [ecx+4]
		cmp		al, kFormType_TESRecipe
		jnz		notRecipe
		mov		ecx, [ecx+0x1C]
		jmp		checkName
	notRecipe:
		cmp		al, kFormType_BGSNote
		jnz		notNote
		mov		ecx, [ecx+0x4C]
		jmp		checkName
	notNote:
		mov		ecx, [ecx+0x34]
	checkName:
		call	SubStrCI
		test	eax, eax
		setz	al
	done:
		retn
	}
}

__declspec(naked) void ItemFilterHook()
{
	__asm
	{
		test	al, al
		jnz		done
		mov		ecx, [ebp+8]
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+8]
		call	ShouldHideItem
	done:
		leave
		retn
	}
}

__declspec(naked) void RecipeFilterHook()
{
	__asm
	{
		test	al, al
		jnz		done
		mov		ecx, [ebp+8]
		call	ShouldHideItem
	done:
		pop		esi
		pop		ebp
		retn
	}
}

bool Cmd_InitItemFilter_Execute(COMMAND_ARGS)
{
	if (!s_itemFilterString)
	{
		s_itemFilterString = (char*)malloc(0x100);
		*s_itemFilterString = 0;
		WriteRelJump(0x7827F1, (UInt32)ItemFilterHook);
		WriteRelJump(0x75E89F, (UInt32)ItemFilterHook);
		WriteRelJump(0x730671, (UInt32)ItemFilterHook);
		WriteRelJump(0x728D99, (UInt32)RecipeFilterHook);
	}
	return true;
}

bool Cmd_SetItemFilter_Execute(COMMAND_ARGS)
{
	if (s_itemFilterString)
	{
		*s_itemFilterString = 0;
		ExtractArgsEx(EXTRACT_ARGS_EX, s_itemFilterString);
	}
	return true;
}

bool Cmd_ToggleHUDCursor_Execute(COMMAND_ARGS)
{
	*result = s_HUDCursorMode;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (g_interfaceManager->currentMode == 1) && !s_controllerReady && (s_HUDCursorMode == !toggle))
	{
		s_HUDCursorMode = !s_HUDCursorMode;
		if (toggle)
		{
			g_interfaceManager->byte170 = 0;
			g_interfaceManager->isRenderedMenuOrPipboyManager = 0;
		}
		else
		{
			g_interfaceManager->cursor->SetFloat(kTileValue_visible, 0);
			g_interfaceManager->cursor->node->m_flags |= 1;
			if (g_interfaceManager->activeTile)
				g_interfaceManager->activeTile->SetFloat(kTileValue_mouseover, 0);
		}
		g_interfaceManager->activeTile = NULL;
		g_interfaceManager->activeMenu = NULL;
		g_DIHookCtrl->SetLMBDisabled(s_HUDCursorMode);
		s_lastLMBState = false;
	}
	return true;
}

bool Cmd_AddTileFromTemplate_Execute(COMMAND_ARGS)
{
	*result = 0;
	char *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(0, buffer, EXTRACT_ARGS_EX, kCommandInfo_AddTileFromTemplate.numParams))
	{
		char *tempName = GetNextToken(buffer, '|');
		if (!*tempName) return true;
		char *altName = GetNextToken(tempName, '|');
		TileMenu *menu;
		Tile *component = NULL;
		char *slashPos = SlashPos(buffer);
		if (slashPos)
		{
			*slashPos = 0;
			menu = GetMenuTile(buffer);
			if (!menu) return true;
			const char *trait = NULL;
			component = menu->GetComponent(slashPos + 1, &trait);
			if (trait) return true;
		}
		else
		{
			menu = GetMenuTile(buffer);
			component = menu;
		}
		if (component)
		{
			component = menu->menu->AddTileFromTemplate(component, tempName);
			if (component)
			{
				*result = 1;
				if (*altName)
					component->name.Set(altName);
			}
		}
	}
	return true;
}

bool Cmd_SetUIFloatGradual_Execute(COMMAND_ARGS)
{
	static const UInt8 kChangeModeMatch[] = {0, 4, 1, 5};
	char tilePath[0x100];
	float startVal, endVal, timer;
	UInt32 changeMode = 0;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &startVal, &endVal, &timer, &changeMode))
	{
		Tile::Value *tileVal = NULL;
		Tile *component = GetTargetComponent(tilePath, &tileVal);
		if (component)
		{
			if (numArgs >= 4)
			{
				if (changeMode > 3) changeMode = 0;
				else changeMode = kChangeModeMatch[changeMode];
				component->GradualSetFloat(tileVal->id, startVal, endVal, timer, changeMode);
			}
			else
			{
				ThisCall(0xA07DC0, component, tileVal->id);
				if (numArgs >= 2)
					tileVal->SetFloat(startVal);
			}
		}
	}
	return true;
}

bool Cmd_CloseActiveMenu_Execute(COMMAND_ARGS)
{
	UInt32 closeAll = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &closeAll) && (g_interfaceManager->currentMode > 1))
	{
		UInt32 index = 10, *mnStack = &g_interfaceManager->menuStack[9], menuID;
		do
		{
			menuID = *mnStack--;
			if (!menuID) continue;
			if (menuID >= kMenuType_Min)
			{
				TileMenu *tileMenu = g_tileMenuArray[menuID - kMenuType_Min];
				if (tileMenu)
					tileMenu->Destroy(true);
			}
			else if (menuID == 1)
			{
				g_interfaceManager->pipBoyMode = 4;
				g_interfaceManager->pipBoyModeCallback = NULL;
			}
			if (!closeAll) break;
		}
		while (--index);
	}
	return true;
}

bool Cmd_ShowLevelUpMenuEx_Execute(COMMAND_ARGS)
{
	UInt32 skillPoints;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &skillPoints))
	{
		CdeclCall(0x784C80);
		LevelUpMenu *menu = *(LevelUpMenu**)0x11D9FDC;
		if (menu)
		{
			menu->numSkillPointsToAssign = skillPoints;
			ThisCall(0x785830, menu, 0);
		}
	}
	return true;
}

bool Cmd_AttachUIXML_Execute(COMMAND_ARGS)
{
	*result = 0;
	char nodeName[0x40], xmlPath[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName, &xmlPath))
	{
		NiNode *targetNode = thisObj->GetNode(nodeName);
		if (targetNode)
		{
			Tile *component = g_HUDMainMenu->tile->ReadXML(xmlPath);
			if (component && component->node)
			{
				targetNode->AddObject(component->node, 1);
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_AttachUIComponent_Execute(COMMAND_ARGS)
{
	*result = 0;
	char nodeName[0x40], *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_AttachUIComponent.numParams, &nodeName))
	{
		NiNode *targetNode = thisObj->GetNode(nodeName);
		if (targetNode)
		{
			Tile *component = InjectUIComponent(g_HUDMainMenu->tile, buffer);
			if (component && component->node)
			{
				targetNode->AddObject(component->node, 1);
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_GetWorldMapPosMults_Execute(COMMAND_ARGS)
{
	static TESWorldSpace *currWorldSpace = NULL, *mapWorldSpace = NULL;
	static alignas(16) WorldDimensions worldDimensions;
	ScriptVar *outX, *outY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY))
	{
		TESWorldSpace *parentWorld = thisObj->GetParentWorld();
		if (parentWorld)
		{
			if (currWorldSpace != parentWorld)
			{
				currWorldSpace = parentWorld;
				worldDimensions.GetPosMods(parentWorld);
				TESWorldSpace *rootWorld = parentWorld->GetRootMapWorld();
				if (mapWorldSpace != rootWorld)
				{
					mapWorldSpace = rootWorld;
					worldDimensions.GetDimensions(rootWorld);
				}
			}
			NiPoint2 outPos;
			GetWorldMapPosMults(thisObj->PosXY(), &worldDimensions, &outPos);
			outX->data.num = outPos.x;
			outY->data.num = outPos.y;
		}
	}
	return true;
}