#pragma once

DEFINE_COMMAND_PLUGIN(IsComponentLoaded, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(InjectUIXML, 0, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(InjectUIComponent, 0, kParams_OneString_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetCursorPos, 0, kParams_OneAxis);
DEFINE_COMMAND_PLUGIN(GetUIString, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetActiveMenuMode, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentName, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentFullName, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetActiveUIComponentID, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetMenuTargetRef, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMenuItemFilter, 0, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ClickMenuButton, 0, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetSelectedItemRef, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetBarterItems, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetBarterGoldAlt, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuSelection, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuCategory, 0, nullptr);
DEFINE_COMMAND_PLUGIN(UnlockRecipeMenuQuantity, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetRecipeMenuNumSubcategories, 0, nullptr);
DEFINE_COMMAND_PLUGIN(MessageExAlt, 0, kParams_OneFloat_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetFontFile, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetFontFile, 0, kParams_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(ShowTextInputMenu, 0, kParams_OneForm_TwoFloats_OneFormatString);
DEFINE_COMMAND_PLUGIN(SetTextInputExtendedProps, 0, kParams_TwoFloats_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(SetTextInputString, 0, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(SetMessageDisabled, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetMessageDisabled, 0, kParams_OneMessage);
DEFINE_COMMAND_PLUGIN(GetMessageFlags, 0, kParams_OneMessage);
DEFINE_COMMAND_PLUGIN(SetMessageFlags, 0, kParams_OneMessage_OneInt);
DEFINE_COMMAND_PLUGIN(SetMessageDisplayTime, 0, kParams_OneMessage_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnMenuClickEventHandler, 0, kParams_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(SetOnMenuOpenEventHandler, 0, kParams_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnMenuCloseEventHandler, 0, kParams_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnMouseoverChangeEventHandler, 0, kParams_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RefreshItemsList, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetBarterPriceMult, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetBarterPriceMult, 0, kParams_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(SetTerminalUIModel, 0, kParams_OneForm_OneOptionalString);
DEFINE_COMMAND_PLUGIN(ShowQuantityMenu, 0, kParams_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(MessageBoxExAlt, 0, kParams_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetVATSTargets, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(IsInCharGen, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ToggleCraftingMessages, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetCursorPos, 0, kParams_TwoFloats);
DEFINE_COMMAND_PLUGIN(UnloadUIComponent, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(ClearMessageQueue, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetSystemColor, 0, kParams_FourInts);
DEFINE_COMMAND_PLUGIN(SuppressQuestMessages, 0, nullptr);
DEFINE_COMMAND_PLUGIN(EnableImprovedRecipeMenu, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ClickMenuTile, 0, kParams_OneString);
DEFINE_COMMAND_PLUGIN(TogglePipBoyLight, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InitItemFilter, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetItemFilter, 0, kParams_OneOptionalString)
DEFINE_COMMAND_PLUGIN(ToggleHUDCursor, 0, kParams_OneOptionalInt)
DEFINE_COMMAND_PLUGIN(AddTileFromTemplate, 0, kParams_FormatString);
DEFINE_COMMAND_PLUGIN(SetUIFloatGradual, 0, kParams_OneString_ThreeOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(CloseActiveMenu, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ShowLevelUpMenuEx, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(AttachUIXML, 1, kParams_TwoStrings);
DEFINE_COMMAND_PLUGIN(AttachUIComponent, 1, kParams_OneString_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetWorldMapPosMults, 1, kParams_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(ProjectUITile, 1, kParams_TwoStrings_FourFloats);
DEFINE_COMMAND_PLUGIN(GetStringUIDimensions, 0, kParams_OneString_OneInt_OneFloat_ThreeScriptVars);
DEFINE_COMMAND_PLUGIN(GetMenuItemListRefs, 0, kParams_TwoOptionalInts);

bool Cmd_IsComponentLoaded_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath) && GetTargetComponent(tilePath))
		*result = 1;
	return true;
}

bool Cmd_InjectUIXML_Execute(COMMAND_ARGS)
{
	char tilePath[0x100], xmlPath[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &xmlPath))
		if (Tile *component = GetTargetComponent(tilePath); component && component->ReadXML(xmlPath))
			*result = 1;
	return true;
}

__declspec(naked) Tile* __stdcall InjectUIComponent(Tile *parentTile, char *dataStr)
{
	static const char kTempXMLFile[] = "jip_temp.xml";
	__asm
	{
		mov		eax, UIOInjectComponent
		test	eax, eax
		jz		doStream
		jmp		eax
		ALIGN 16
	doStream:
		push	'bw'
		push	esp
		push	offset kTempXMLFile
		call	fopen
		add		esp, 0xC
		test	eax, eax
		jz		done
		push	eax
		push	dword ptr [esp+0xC]
		call	fputs
		pop		ecx
		call	fclose
		pop		ecx
		push	offset kTempXMLFile
		mov		ecx, [esp+8]
		CALL_EAX(0xA01B00)
	done:
		retn	8
	}
}

bool Cmd_InjectUIComponent_Execute(COMMAND_ARGS)
{
	char tilePath[0x100], *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_InjectUIComponent.numParams, &tilePath))
		if (Tile *component = GetTargetComponent(tilePath); component && InjectUIComponent(component, buffer))
			*result = 1;
	return true;
}

bool Cmd_GetCursorPos_Execute(COMMAND_ARGS)
{
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
		*result = (axis == 'X') ? g_interfaceManager->cursorX : g_interfaceManager->cursorY;
	return true;
}

bool Cmd_GetUIString_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath))
	{
		TileValue *value = nullptr;
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
	const char *tileName = nullptr;
	if (Tile *activeTile = g_interfaceManager->GetActiveTile())
	{
		char tilePath[0x100];
		activeTile->GetComponentFullName(tilePath);
		tileName = FindChrR(tilePath, '/');
		if (tileName) tileName++;
	}
	AssignString(PASS_COMMAND_ARGS, tileName);
	return true;
}

bool Cmd_GetActiveUIComponentFullName_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	tilePath[0] = 0;
	if (Tile *activeTile = g_interfaceManager->GetActiveTile())
		activeTile->GetComponentFullName(tilePath);
	AssignString(PASS_COMMAND_ARGS, tilePath);
	return true;
}

SInt32 GetActiveTileID()
{
	if (Tile *activeTile = g_interfaceManager->GetActiveTile())
		if (TileValue *val = activeTile->GetValue(kTileValue_id))
			return val->num;
	return -1;
}

bool Cmd_GetActiveUIComponentID_Execute(COMMAND_ARGS)
{
	*result = GetActiveTileID();
	return true;
}

bool Cmd_GetMenuTargetRef_Execute(COMMAND_ARGS)
{
	UInt32 menuID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &menuID) || (menuID < kMenuType_Min) || (menuID > kMenuType_Max))
		return true;
	TileMenu *tileMenu = g_tileMenuArray[menuID];
	Menu *menu = tileMenu ? tileMenu->menu : nullptr;
	if (!menu) return true;
	TESForm *menuRef = nullptr;
	TESObjectREFR *container = g_thePlayer;
	ContChangesEntry *entry = nullptr;
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
				ExtraTalkingActor *xTalker = GetExtraType(&((TESObjectREFR*)menuRef)->extraDataList, ExtraTalkingActor);
				if (xTalker) menuRef = xTalker->actor;
			}
			break;
		}
		case kMenuType_LockPick:
			menuRef = ((LockPickMenu*)menu)->targetRef;
			break;
		case kMenuType_Quantity:
		{
			if (MENU_VISIBILITY[kMenuType_Inventory])
				entry = InventoryMenu::Selection();
			else if (ContainerMenu::Get())
			{
				entry = ContainerMenu::Selection();
				ContainerMenu *cntMenu = ContainerMenu::Get();
				if (cntMenu->currentItems == &cntMenu->rightItems)
					container = cntMenu->containerRef;
			}
			else if (BarterMenu::Get())
			{
				entry = BarterMenu::Selection();
				BarterMenu *brtMenu = BarterMenu::Get();
				if (brtMenu->currentItems == &brtMenu->rightItems)
					container = brtMenu->merchantRef->GetMerchantContainer();
			}
			break;
		}
		case kMenuType_Map:
		{
			MapMenu *mapMenu = (MapMenu*)menu;
			if (mapMenu->mapMarker)
			{
				static UInt32 valueID = 0;
				if (!valueID) valueID = Tile::TraitNameToID("_MarkerIndex");
				TileValue *markerIdx = mapMenu->mapMarker->GetValue(valueID);
				if (markerIdx) menuRef = mapMenu->mapMarkerList.GetNthItem(markerIdx->num);
			}
			break;
		}
		case kMenuType_LevelUp:
		{
			LevelUpMenu *levelUpMenu = (LevelUpMenu*)menu;
			if ((levelUpMenu->currentPage == 1) && levelUpMenu->perkListBox.selected)
				menuRef = levelUpMenu->perkListBox.GetSelected();
			break;
		}
		case kMenuType_Repair:
			entry = RepairMenu::Target();
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
		case kMenuType_RepairServices:
			menuRef = RepairServicesMenu::Target();
			break;
		case kMenuType_ItemMod:
			entry = ItemModMenu::Target();
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
	if (container && entry)
		menuRef = CreateRefForStack(container, entry);
	if (menuRef)
		REFR_RES = menuRef->refID;
	return true;
}

bool Cmd_GetMenuItemFilter_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 menuID, useRef = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &menuID, &useRef))
		return true;
	switch (menuID)
	{
		case kMenuType_Inventory:
			if (InventoryMenu::Get())
				*result = (int)InventoryMenu::Get()->filter + 1;
			break;
		case kMenuType_Container:
			if (ContainerMenu::Get())
				*result = useRef ? (int)ContainerMenu::Get()->rightFilter : (int)ContainerMenu::Get()->leftFilter;
			break;
		case kMenuType_Barter:
			if (BarterMenu::Get())
				*result = useRef ? (int)BarterMenu::Get()->rightFilter : (int)BarterMenu::Get()->leftFilter;
	}
	return true;
}

bool Cmd_ClickMenuButton_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	UInt32 times = 1;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &times) || !GetMenuMode() || !times) return true;
	Tile *component = nullptr;
	Menu *parentMenu = nullptr;
	SInt32 tileID = -1;
	if (char *hashPos = FindChr(tilePath, '#'))
	{
		tileID = StrToInt(hashPos + 1);
		if (tileID >= 0)
		{
			*hashPos = 0;
			if (TileMenu *tileMenu = GetMenuTile(tilePath))
				parentMenu = tileMenu->menu;
		}
	}
	else
	{
		component = GetTargetComponent(tilePath);
		if (!component) return true;
		parentMenu = component->GetParentMenu();
		if (TileValue *tileVal = component->GetValue(kTileValue_id))
			tileID = tileVal->num;
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
	TESForm *itemRef = nullptr;
	TESObjectREFR *container = g_thePlayer;
	ContChangesEntry *entry = nullptr;
	switch (g_interfaceManager->GetTopVisibleMenuID())
	{
		case kMenuType_Inventory:
		{
			if (InventoryMenu::Get()->itemList.selected)
				entry = InventoryMenu::Selection();
			break;
		}
		case kMenuType_Stats:
		{
			if (StatsMenu *statsMenu = StatsMenu::Get(); statsMenu->perkRankList.selected)
				itemRef = statsMenu->perkRankList.GetSelected()->perk;
			break;
		}
		case kMenuType_Container:
		{
			if (ContainerMenu *cntMenu = ContainerMenu::Get(); cntMenu->leftItems.selected || cntMenu->rightItems.selected)
			{
				entry = ContainerMenu::Selection();
				if (cntMenu->rightItems.selected)
					container = cntMenu->containerRef;
			}
			break;
		}
		case kMenuType_Map:
		{
			MapMenu *mapMenu = MapMenu::Get();
			if (mapMenu->questList.selected && mapMenu->questList.IsEnabled())
				itemRef = mapMenu->questList.GetSelected();
			else if (mapMenu->noteList.selected && mapMenu->noteList.IsEnabled())
				itemRef = mapMenu->noteList.GetSelected();
			else if (mapMenu->challengeList.selected && mapMenu->challengeList.IsEnabled())
				itemRef = mapMenu->challengeList.GetSelected();
			break;
		}
		case kMenuType_Repair:
		{
			if (RepairMenu *rprMenu = RepairMenu::Get(); rprMenu->repairItems.selected)
				entry = rprMenu->repairItems.GetSelected();
			break;
		}
		case kMenuType_Barter:
		{
			if (BarterMenu *brtMenu = BarterMenu::Get(); brtMenu->leftItems.selected || brtMenu->rightItems.selected)
			{
				entry = BarterMenu::Selection();
				if (brtMenu->rightItems.selected)
					container = brtMenu->merchantRef->GetMerchantContainer();
			}
			break;
		}
		case kMenuType_RepairServices:
		{
			if (RepairServicesMenu *rpsMenu = RepairServicesMenu::Get(); rpsMenu->itemList.selected)
				entry = rpsMenu->itemList.GetSelected();
			break;
		}
		case kMenuType_ItemMod:
		{
			if (ItemModMenu *modMenu = ItemModMenu::Get(); modMenu->itemModList.selected)
				entry = modMenu->itemModList.GetSelected();
		}
	}
	if (container && entry)
		itemRef = CreateRefForStack(container, entry);
	if (itemRef)
		REFR_RES = itemRef->refID;
	return true;
}

bool Cmd_GetBarterItems_Execute(COMMAND_ARGS)
{
	UInt32 sold;
	BarterMenu *brtMenu = BarterMenu::Get();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &sold) || !brtMenu || !brtMenu->merchantRef) return true;
	TESObjectREFR *target = sold ? brtMenu->merchantRef->GetMerchantContainer() : g_thePlayer;
	TempElements *tmpElements = GetTempElements();
	auto iter = sold ? brtMenu->rightBarter.Head() : brtMenu->leftBarter.Head();
	do
	{
		if (iter->data)
			tmpElements->Append(CreateRefForStack(target, iter->data));
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetBarterGoldAlt_Execute(COMMAND_ARGS)
{
	if (BarterMenu::Get())
		*result = (int)BarterMenu::Get()->merchantGold;
	return true;
}

bool Cmd_GetRecipeMenuSelection_Execute(COMMAND_ARGS)
{
	if (RecipeMenu::Get() && RecipeMenu::Selection())
		REFR_RES = RecipeMenu::Selection()->refID;
	return true;
}

bool Cmd_GetRecipeMenuCategory_Execute(COMMAND_ARGS)
{
	if (RecipeMenu::Get())
		REFR_RES = RecipeMenu::Get()->category->refID;
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
	const char *msgIcon = nullptr, *msgSound = nullptr;
	while (true)
	{
		char *barPtr = GetNextToken(buffer, '|');
		if (!*barPtr) break;
		if (*buffer == '$')
			msgSound = buffer + 1;
		else if (*buffer == '#')
		{
			char iconIdx = buffer[1] - '0';
			if ((iconIdx >= 0) && (iconIdx <= 6))
				msgIcon = (const char*)kMsgIconsPathAddr[iconIdx];
		}
		else msgIcon = buffer;
		buffer = barPtr;
	}
	buffer[0x203] = 0;
	QueueUIMessage(buffer, 0, msgIcon, msgSound, displayTime, 0);
	return true;
}

bool Cmd_GetFontFile_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	UInt32 fontID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fontID) && fontID && (fontID <= 89) && (fontID != 9))
		if (FontInfo *fontInfo = g_fontManager->fontInfos[fontID - 1])
			resStr = fontInfo->filePath;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetFontFile_Execute(COMMAND_ARGS)
{
	UInt32 fontID;
	char dataPathFull[0x100], *dataPath = dataPathFull + 5;
	*(UInt32*)dataPathFull = 'atad';
	dataPathFull[4] = '\\';
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &fontID, dataPath) || !fontID || (fontID > 89) || (fontID == 9) || !*dataPath) return true;
	FontInfo *fontInfo = s_fontInfosMap->Get(dataPath);
	if (!fontInfo)
	{
		if (!FileExists(dataPathFull)) return true;
		fontInfo = Game_HeapAlloc<FontInfo>();
		fontInfo->Init(fontID, dataPath, true);
		if (!fontInfo->bufferData)
		{
			Game_HeapFree(fontInfo);
			return true;
		}
		s_fontInfosMap()[dataPath] = fontInfo;
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
		push	0
		mov		edx, kTileValue_mouseover
		call	Tile::SetBool
	resetActive:
		and		dword ptr [esi+0xCC], 0
		and		dword ptr [esi+0xD0], 0
	isActive:
		mov		ds:0x11DAEC4, edi
		CALL_EAX(0xA1DFB0)
		push	0
		push	ecx
		fstp	dword ptr [esp]
		push	kTileValue_depth
		mov		ecx, [edi+4]
		CALL_EAX(ADDR_TileSetFloat)
		movss	xmm0, [esp+0xC]
		mov		edx, kTileValue_user0
		mov		ecx, [edi+4]
		call	Tile::SetFloat
		movss	xmm0, [esp+0x10]
		mov		edx, kTileValue_user1
		mov		ecx, [edi+4]
		call	Tile::SetFloat
		mov		esi, [esp+0x14]
		mov		[esi+0x50], 0
		cmp		[esi], 0
		jnz		hasTitle
		push	0
		mov		edx, kTileValue_visible
		mov		ecx, [edi+0x30]
		call	Tile::SetBool
	hasTitle:
		push	esi
		mov		edx, kTileValue_string
		mov		ecx, [edi+0x30]
		call	Tile::SetString
		mov		edx, kTileValue_font
		mov		ecx, [edi+0x28]
		call	Tile::GetValueFloat
		cvtss2si	ecx, xmm0
		lea		esi, s_fontHeightDatas[ecx*8]
		movss	xmm0, [esi]
		mov		edx, kTileValue_user0
		mov		ecx, [edi+0x28]
		call	Tile::SetFloat
		movss	xmm0, [esi+4]
		mov		edx, kTileValue_user1
		mov		ecx, [edi+0x28]
		call	Tile::SetFloat
		push	0x400
		lea		ecx, [edi+0x34]
		call	String::Init
		push	0x400
		lea		ecx, [edi+0x3C]
		call	String::Init
		mov		ecx, [edi+0x3C]
		mov		word ptr [ecx], '|'
		inc		word ptr [edi+0x40]
		push	dword ptr ds:0x11D38BC
		mov		edx, kTileValue_string
		mov		ecx, [edi+0x2C]
		call	Tile::SetString
		and		dword ptr [edi+0x44], 0
		mov		dword ptr [edi+0x48], 0x7FFF0001
		mov		eax, [edi+0x28]
		mov		eax, [eax+0x28]
		mov		[edi+0x4C], eax
		mov		dword ptr [edi+0x54], 0x00000100
		mov		eax, [esp+0x18]
		mov		[edi+0x58], eax
		push	eax
		call	CaptureLambdaVars
		pop		ecx
		mov		ecx, edi
		call	TextInputRefreshHook
		push	0
		mov		ecx, edi
		CALL_EAX(0xA1DC20)
		mov		dl, 1
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
	auto iter = tempList.Head();
	do
	{
		if (!(form = iter->data) || NOT_ID(form, BGSMessage)) continue;
		form->SetJIPFlag(kHookFormFlag6_MessageDisabled, disable != 0);
	}
	while (iter = iter->next);
	HOOK_SET(MessageDisabled, true);
	return true;
}

bool Cmd_GetMessageDisabled_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &message) && (message->jipFormFlags6 & kHookFormFlag6_MessageDisabled))
		*result = 1;
	return true;
}

bool Cmd_GetMessageFlags_Execute(COMMAND_ARGS)
{
	BGSMessage *message;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &message))
		*result = message->msgFlags;
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
	char *slashPos = SlashPos(tilePath), *hashPos = nullptr;
	if (slashPos) *slashPos = 0;
	else
	{
		hashPos = FindChr(tilePath, '#');
		if (hashPos) *hashPos = 0;
	}
	UInt32 menuID = s_menuNameToID->Get(tilePath);
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
			clickEvent.idsMap()[tileID].Insert(script);
		}
		else if (!clickEvent.idsMap().Empty())
		{
			auto findID = clickEvent.idsMap().Find(tileID);
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
	ReplaceChr(tilePath, '\\', '/');
	StrToLower(tilePath);
	if (addEvnt)
	{
		bool match = false;
		if (!clickEvent.filtersMap().Empty())
		{
			for (auto filterb = clickEvent.filtersMap().FindOpDir(tilePath, false); filterb; --filterb)
			{
				if (!StrBeginsCS(tilePath, filterb.Key())) break;
				if (filterb().HasKey(script))
					return true;
			}
			char cmpr;
			for (auto filterf = clickEvent.filtersMap().FindOpDir(tilePath, true); filterf; ++filterf)
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
			clickEvent.filtersMap()[tilePath].Insert(script);
		}
	}
	else if (!clickEvent.filtersMap().Empty())
	{
		for (auto filterf = clickEvent.filtersMap().FindOpDir(tilePath, true); filterf; ++filterf)
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

void SetOnMenuStateEvent(Script *script, bool doAdd, char idx, UInt8 eventType)
{
	MenuStateCallbacks *callbacks = s_menuStateEventMap[idx];
	if (!callbacks)
	{
		if (!doAdd) return;
		callbacks = new (Pool_Alloc<MenuStateCallbacks>()) MenuStateCallbacks();
		s_menuStateEventMap[idx] = callbacks;
	}
	EventCallbackScripts *scripts = (eventType == 2) ? &callbacks->onMouseover : (eventType ? &callbacks->onClose : &callbacks->onOpen);
	if (doAdd)
	{
		if (scripts->HasKey(script)) return;
		if (eventType == 2)
		{
			if (scripts->Empty())
				HOOK_INC(MenuHandleMouseover);
		}
		else if (callbacks->onOpen.Empty() && callbacks->onClose.Empty())
			HOOK_INC(MenuState);
		scripts->Insert(script);
	}
	else
	{
		if (!scripts->Erase(script)) return;
		if (eventType == 2)
		{
			if (scripts->Empty())
				HOOK_DEC(MenuHandleMouseover);
		}
		else if (callbacks->onOpen.Empty() && callbacks->onClose.Empty())
			HOOK_DEC(MenuState);
	}
}

bool SetOnMenuStateEventHandler_Execute(COMMAND_ARGS)
{
	CAPTURE_CL(eventType)
	Script *script;
	UInt32 addEvnt, menuID = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &menuID) && IS_ID(script, Script))
	{
		char idx;
		bool doAdd = (addEvnt != 0);
		if (!menuID)
		{
			for (idx = 0; idx < 36; idx++)
				SetOnMenuStateEvent(script, doAdd, idx, eventType);
		}
		else if ((menuID >= kMenuType_Min) && (menuID <= kMenuType_Max))
		{
			idx = kMenuIDJumpTable[menuID - kMenuType_Min];
			if (idx != -1) SetOnMenuStateEvent(script, doAdd, idx, eventType);
		}
	}
	return true;
}

__declspec(naked) bool Cmd_SetOnMenuOpenEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		xor		cl, cl
		jmp		SetOnMenuStateEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnMenuCloseEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		cl, 1
		jmp		SetOnMenuStateEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnMouseoverChangeEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		cl, 2
		jmp		SetOnMenuStateEventHandler_Execute
	}
}

bool Cmd_RefreshItemsList_Execute(COMMAND_ARGS)
{
	RefreshItemListBox();
	return true;
}

bool Cmd_GetBarterPriceMult_Execute(COMMAND_ARGS)
{
	UInt32 sellMult;
	BarterMenu *brtMenu = BarterMenu::Get();
	if (brtMenu && ExtractArgsEx(EXTRACT_ARGS_EX, &sellMult))
		*result = sellMult ? brtMenu->sellValueMult : brtMenu->buyValueMult;
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
	auto lstIter = tempList.Head();
	BGSTerminal *terminal;
	char **pathPtr;
	do
	{
		terminal = (BGSTerminal*)lstIter->data;
		if (!terminal || NOT_ID(terminal, BGSTerminal)) continue;
		if (bRemove) DoPurgePath(s_terminalAltModelsMap->GetErase(terminal));
		else
		{
			if (!s_terminalAltModelsMap->InsertKey(terminal, &pathPtr))
				DoPurgePath(*pathPtr);
			*pathPtr = CopyString(modelPath);
		}
	}
	while (lstIter = lstIter->next);
	HOOK_SET(SetTerminalModel, !s_terminalAltModelsMap->Empty());
	return true;
}

Script *s_quantityMenuScript = nullptr;

__declspec(naked) void QuantityMenuCallback(int count)
{
	__asm
	{
		mov		ecx, s_quantityMenuScript
		test	ecx, ecx
		jz		done
		xor		eax, eax
		mov		s_quantityMenuScript, eax
		cmp		dword ptr [ebp+8], 5
		setz	al
		neg		eax
		add		eax, [esp+4]
		push	eax
		push	1
		push	0
		push	ecx
		call	CallFunction
		call	UncaptureLambdaVars
		add		esp, 0x10
	done:
		retn
	}
}

bool Cmd_ShowQuantityMenu_Execute(COMMAND_ARGS)
{
	Script *callback;
	int maxCount, defaultCount = -1;
	if (!QuantityMenu::Get() && ExtractArgsEx(EXTRACT_ARGS_EX, &callback, &maxCount, &defaultCount) && (maxCount > 0) && IS_ID(callback, Script) &&
		QuantityMenu::ShowMenu(maxCount, QuantityMenuCallback, ((defaultCount < 0) || (defaultCount > maxCount)) ? maxCount : defaultCount))
	{
		s_quantityMenuScript = callback;
		CaptureLambdaVars(callback);
		*result = 1;
	}
	return true;
}

__declspec(naked) void MessageBoxCallback()
{
	__asm
	{
		mov		ecx, g_interfaceManager
		add		ecx, 0xE4
		movzx	edx, byte ptr [ecx]
		mov		byte ptr [ecx], 0xFF
		mov		ecx, offset s_messageBoxScripts
		mov		eax, [ecx]
		mov		eax, [ecx+eax*4]
		dec		dword ptr [ecx]
		test	eax, eax
		jz		done
		push	edx
		push	1
		push	0
		push	eax
		call	CallFunction
		call	UncaptureLambdaVars
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
		mov		edx, 0x703E80
		push	0
		ALIGN 16
	addButtons:
		mov		eax, [ecx]
		push	eax
		sub		ecx, 4
		test	eax, eax
		jnz		addButtons
		push	0
		push	0x17
		push	0
		push	MessageBoxCallback
		push	dword ptr [ecx-4]
		push	0
		push	dword ptr [ecx]
		call	edx
		leave
		retn
	}
}

bool Cmd_MessageBoxExAlt_Execute(COMMAND_ARGS)
{
	Script *callback;
	char *buffer = GetStrArgBuffer();
	if (!s_messageBoxScripts.Full() && ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_MessageBoxExAlt.numParams, &callback))
	{
		if IS_ID(callback, Script)
			CaptureLambdaVars(callback);
		else callback = nullptr;
		s_messageBoxScripts.Append(callback);
		
		char *msgStrings[0x102], **buttonPtr = msgStrings + 2, *delim = buffer;
		*buttonPtr = nullptr;
		for (UInt32 count = 0xFF; count; count--)
		{
			delim = GetNextToken(delim, '|');
			if (!*delim) break;
			*++buttonPtr = delim;
		}
		if (!*buttonPtr) *++buttonPtr = const_cast<char*>("OK");
		if ((buffer[0] == '^') && (delim = FindChr(buffer + 1, '^')))
		{
			*delim = 0;
			*msgStrings = buffer + 1;
			msgStrings[1] = delim + 1;
		}
		else
		{
			*msgStrings = nullptr;
			msgStrings[1] = buffer;
		}
		MessageBoxExAlt(buttonPtr);
	}
	return true;
}

bool Cmd_GetVATSTargets_Execute(COMMAND_ARGS)
{
	if (!VATSMenu::Get()) return true;
	TempElements *tmpElements = GetTempElements();
	auto iter = GameGlobals::VATSTargetList()->Head();
	do
	{
		if (VATSTarget *targetInfo = iter->data; targetInfo && targetInfo->targetRef)
			tmpElements->Append(targetInfo->targetRef);
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

__declspec(noinline) bool InCharGen()
{
	return g_tileMenuArray[kMenuType_CharGen] || g_tileMenuArray[kMenuType_SpecialBook] || 
		g_tileMenuArray[kMenuType_LoveTester] || g_tileMenuArray[kMenuType_Trait];
}

bool Cmd_IsInCharGen_Execute(COMMAND_ARGS)
{
	if (InCharGen())
		*result = 1;
	return true;
}

bool Cmd_IsInCharGen_Eval(COMMAND_ARGS_EVAL)
{
	if (InCharGen())
		*result = 1;
	return true;
}

bool Cmd_ToggleCraftingMessages_Execute(COMMAND_ARGS)
{
	int state = *(UInt8*)0x728933 == 0x8D, toggle;
	*result = state;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (state != toggle))
		SafeWriteBuf(0x728933, toggle ? "\x8D\x4D\xCC\xE8\x75" : "\xE9\xC4\x00\x00\x00", 5);
	return true;
}

bool Cmd_SetCursorPos_Execute(COMMAND_ARGS)
{
	float posX, posY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posX, &posY) && GetMenuMode())
	{
		g_interfaceManager->cursorX = posX;
		g_interfaceManager->cursorY = posY;
		float converter = *(float*)0x11D8A48;
		g_cursorNode->LocalTranslate().x = (posX * converter) - g_screenWidth;
		g_cursorNode->LocalTranslate().z = g_screenHeight - (posY * converter);
	}
	return true;
}

bool Cmd_UnloadUIComponent_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath))
		if (Tile *component = GetTargetComponent(tilePath))
			component->Destroy(true);
	return true;
}

bool Cmd_ClearMessageQueue_Execute(COMMAND_ARGS)
{
	g_HUDMainMenu->ClearMessageQueue();
	return true;
}

bool Cmd_SetSystemColor_Execute(COMMAND_ARGS)
{
	UInt32 type, red, green, blue;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &red, &green, &blue) && type && (type <= 5))
		if (auto sysColor = SystemColorManager::GetSingleton()->sysColors.GetNthItem(type - 1))
		{
			sysColor->SetColorRGB(red, green, blue);
			ThisCall(0xA0B350, g_interfaceManager->menuRoot, type, 0);
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

bool s_showQuestMessages = true;

void ToggleQuestMessages()
{
	s_showQuestMessages = !s_showQuestMessages;
	INIS_BOOL(bShowChallengeUpdates_GamePlay) = s_showQuestMessages;
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
		push	dword ptr ds:0x11D8EA8
		CALL_EAX(0x7ABA00)
	done:
		leave
		retn	8
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

__declspec(naked) void RecipeMenuCloseHook()
{
	__asm
	{
		cmp		byte ptr ds:0x728933, 0x8D
		jz		doneSound
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
		mov		esi, ds:0x11D8E90
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
	if (*(UInt8*)0x7274E1 != 0xF)
	{
		SafeWrite8(0x7274E1, 0xF);
		SafeWrite32(0x727660, (UInt32)RecipeMenuAcceptHook);
		SAFE_WRITE_BUF(0x727675, "\x03\x03\x03\x03\x03\x03\x01");
		SafeWrite32(0x727975, 0x7FFFFFFF);
		WriteRelJump(0x728A0C, (UInt32)RecipeMenuCloseHook);
	}
	return true;
}

bool Cmd_ClickMenuTile_Execute(COMMAND_ARGS)
{
	char tilePath[0x100];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath) && GetMenuMode())
		if (Tile *component = GetTargetComponent(tilePath); component && (component->GetValueFloat(kTileValue_target) > 0))
		{
			component->PokeValue(kTileValue_clicked);
			*result = 1;
		}
	return true;
}

__declspec(naked) void __fastcall TogglePipBoyLight(PlayerCharacter *thePlayer, SpellItem *pipBoyLight, UInt32 turnON)
{
	__asm
	{
		push	0
		cmp		dword ptr [esp+8], 0
		jz		turnOFF
		push	edx
		add		ecx, 0x88
		mov		eax, [ecx]
		call	dword ptr [eax]
		jmp		finish
	turnOFF:
		push	0
		add		edx, 0x18
		push	edx
		add		ecx, 0x94
		CALL_EAX(0x824400)
	finish:
		mov		ecx, g_interfaceManager
		mov		ecx, [ecx+0x174]
		mov		edx, [esp+4]
		push	1
		push	edx
		push	0
		push	ecx
		push	1
		push	edx
		push	1
		CALL_EAX(0x7FA310)
		pop		ecx
		CALL_EAX(0x7FA310)
		retn	4
	}
}

bool Cmd_TogglePipBoyLight_Execute(COMMAND_ARGS)
{
	SpellItem *pipBoyLight = GameGlobals::PipBoyLight();
	UInt32 turnON, currState = ThisCall<bool>(0x822B90, &g_thePlayer->magicTarget, &pipBoyLight->magicItem, 1);
	*result = (int)currState;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &turnON) && (turnON != currState))
		TogglePipBoyLight(g_thePlayer, pipBoyLight, turnON);
	return true;
}

char *s_itemFilterString = nullptr;

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
			g_interfaceManager->cursor->SetBool(kTileValue_visible, 0);
			g_interfaceManager->cursor->node->Hide();
			if (g_interfaceManager->activeTile)
				g_interfaceManager->activeTile->SetBool(kTileValue_mouseover, 0);
		}
		g_interfaceManager->activeTile = nullptr;
		g_interfaceManager->activeMenu = nullptr;
		g_DIHookCtrl->SetLMBDisabled(s_HUDCursorMode);
		s_lastLMBState = false;
	}
	return true;
}

bool Cmd_AddTileFromTemplate_Execute(COMMAND_ARGS)
{
	char buffer[0x100];
	if (ExtractFormatStringArgs(0, buffer, EXTRACT_ARGS_EX, kCommandInfo_AddTileFromTemplate.numParams))
	{
		char *tempName = GetNextToken(buffer, '|');
		if (!*tempName) return true;
		char *altName = GetNextToken(tempName, '|');
		TileMenu *menu;
		Tile *component = nullptr;
		char *slashPos = SlashPos(buffer);
		if (slashPos)
		{
			*slashPos = 0;
			menu = GetMenuTile(buffer);
			if (!menu) return true;
			const char *trait = nullptr;
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
	char tilePath[0x100];
	float startVal, endVal, timer;
	UInt32 changeMode = 0;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &startVal, &endVal, &timer, &changeMode))
	{
		TileValue *tileVal = nullptr;
		if (Tile *component = GetTargetComponent(tilePath, &tileVal))
			if (numArgs >= 4)
			{
				if (changeMode > 4)
					changeMode = 0;
				else
				{
					UInt8 changeModeMatch[] = {0, 4, 1, 5, 6};
					changeMode = changeModeMatch[changeMode];
				}
				component->StartGradualSetFloat(tileVal->id, startVal, endVal, timer, changeMode);
			}
			else
			{
				component->EndGradualSetFloat(tileVal->id);
				if (numArgs >= 2)
					tileVal->SetFloat(startVal);
			}
	}
	return true;
}

bool Cmd_CloseActiveMenu_Execute(COMMAND_ARGS)
{
	UInt32 closeAll = 0;
	InterfaceManager *intrfcMgr = InterfaceManager::GetSingleton();
	if ((intrfcMgr->currentMode > 1) && ExtractArgsEx(EXTRACT_ARGS_EX, &closeAll))
	{
		int index = 9;
		do
		{
			UInt32 menuID = intrfcMgr->menuStack[index];
			if (!menuID) continue;
			if (menuID >= kMenuType_Min)
			{
				if (TileMenu *tileMenu = g_tileMenuArray[menuID])
				{
					if (FORenderedMenu *renderedMenu = intrfcMgr->renderedMenu; renderedMenu && (renderedMenu->tileMenu == tileMenu))
						renderedMenu->Close();
					else tileMenu->Destroy(true);
				}
				if (menuID == kMenuType_Message)
				{
					CdeclCall(0x7AA480);
					if (!s_messageBoxScripts.Empty())
					{
						for (auto scrIter = s_messageBoxScripts.Begin(); scrIter; ++scrIter)
							if (*scrIter) UncaptureLambdaVars(*scrIter);
						s_messageBoxScripts.Clear();
					}
				}
				else if (menuID == kMenuType_Quantity)
				{
					if (s_quantityMenuScript)
					{
						UncaptureLambdaVars(s_quantityMenuScript);
						s_quantityMenuScript = nullptr;
					}
				}
				else if ((menuID == kMenuType_TextEdit) && HOOK_INSTALLED(TextInputClose))
					UnsetTextInputHooks(TextEditMenu::Get());
			}
			else if (menuID == 1)
			{
				intrfcMgr->pipBoyMode = 4;
				intrfcMgr->pipBoyModeCallback = nullptr;
			}
			intrfcMgr->menuStack[index] = 0;
			if (!closeAll) break;
		}
		while (--index >= 0);
	}
	return true;
}

bool Cmd_ShowLevelUpMenuEx_Execute(COMMAND_ARGS)
{
	UInt32 skillPoints;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &skillPoints))
	{
		CdeclCall(0x784C80);
		if (auto menu = LevelUpMenu::Get())
		{
			menu->numSkillPointsToAssign = skillPoints;
			ThisCall(0x785830, menu, 0);
		}
	}
	return true;
}

bool Cmd_AttachUIXML_Execute(COMMAND_ARGS)
{
	char nodeName[0x40], xmlPath[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName, &xmlPath))
		if (NiNode *targetNode = thisObj->GetNode(nodeName))
			if (Tile *component = g_HUDMainMenu->tile->ReadXML(xmlPath); component && component->node)
			{
				targetNode->AddObject(component->node, 1);
				*result = 1;
			}
	return true;
}

bool Cmd_AttachUIComponent_Execute(COMMAND_ARGS)
{
	char nodeName[0x40], *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_AttachUIComponent.numParams, &nodeName))
		if (NiNode *targetNode = thisObj->GetNode(nodeName))
			if (Tile *component = InjectUIComponent(g_HUDMainMenu->tile, buffer); component && component->node)
			{
				targetNode->AddObject(component->node, 1);
				*result = 1;
			}
	return true;
}

TESWorldSpace *s_currWorldSpaceA = nullptr, *s_mapWorldSpaceA = nullptr;
WorldDimensions s_worldDimensionsA;

bool Cmd_GetWorldMapPosMults_Execute(COMMAND_ARGS)
{
	ScriptVar *outX, *outY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY))
		if (TESWorldSpace *parentWorld = thisObj->GetParentWorld())
		{
			if (s_currWorldSpaceA != parentWorld)
			{
				s_currWorldSpaceA = parentWorld;
				s_worldDimensionsA.InitPosMods(parentWorld);
				TESWorldSpace *rootWorld = parentWorld->GetRootMapWorld();
				if (s_mapWorldSpaceA != rootWorld)
				{
					s_mapWorldSpaceA = rootWorld;
					s_worldDimensionsA.InitDimensions(rootWorld);
				}
			}
			NiPoint2 outPos(GetWorldMapPosMults(thisObj->position.PS2(), s_worldDimensionsA));
			outX->data = outPos.x;
			outY->data = outPos.y;
		}
	return true;
}

bool Cmd_ProjectUITile_Execute(COMMAND_ARGS)
{
	char tilePath[0x100], blockName[0x40];
	NiVector4 scrArea;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &tilePath, &blockName, &scrArea.x, &scrArea.y, &scrArea.z, &scrArea.w))
	{
		if (auto pTexture = thisObj->GetTexturePtr(blockName))
		{
			NiNode *tileNode = nullptr;
			if (tilePath[0])
			{
				if (auto component = GetTargetComponent(tilePath))
					tileNode = component->node;
			}
			else tileNode = g_interfaceManager->uiRootNode;
			if (tileNode)
			{
				BSTextureManager::GenerateRenderedUITexture(tileNode, scrArea, pTexture);
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_GetStringUIDimensions_Execute(COMMAND_ARGS)
{
	char *buffer = GetStrArgBuffer();
	UInt32 fontID;
	float wrapWidth;
	ResultVars outDims;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, buffer, &fontID, &wrapWidth, &outDims.x, &outDims.y, &outDims.z))
	{
		if (wrapWidth <= 0)
			wrapWidth = FLT_MAX;
		NiVector3 resDims;
		g_fontManager->GetStringDimensions(&resDims, buffer, fontID, wrapWidth);
		outDims.Set(resDims.PS());
	}
	return true;
}

bool Cmd_GetMenuItemListRefs_Execute(COMMAND_ARGS)
{
	UInt32 inclFiltered = 0, rightSide = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &inclFiltered, &rightSide))
	{
		MenuItemEntryList *entryList;
		TESObjectREFR *container = g_thePlayer;
		switch (g_interfaceManager->GetTopVisibleMenuID())
		{
			case kMenuType_Inventory:
				entryList = &InventoryMenu::Get()->itemList;
				break;
			case kMenuType_Container:
			{
				ContainerMenu *cntMenu = ContainerMenu::Get();
				if (rightSide)
				{
					entryList = &cntMenu->rightItems;
					container = cntMenu->containerRef;
				}
				else entryList = &cntMenu->leftItems;
				break;
			}
			case kMenuType_Repair:
				entryList = &RepairMenu::Get()->repairItems;
				break;
			case kMenuType_Barter:
			{
				BarterMenu *brtMenu = BarterMenu::Get();
				if (rightSide)
				{
					entryList = &brtMenu->rightItems;
					container = brtMenu->merchantRef->GetMerchantContainer();
				}
				else entryList = &brtMenu->leftItems;
				break;
			}
			case kMenuType_RepairServices:
				entryList = &RepairServicesMenu::Get()->itemList;
				break;
			default:
				return true;
		}
		if (!container)
			return true;
		NVSEArrayVar *resArray = CreateMap(nullptr, nullptr, 0, scriptObj);
		int tileIndex = 0;
		auto listIter = entryList->list.Head();
		do
		{
			auto listItem = listIter->data;
			if (!listItem || !listItem->item)
				continue;
			if (inclFiltered || !listItem->isFiltered)
				SetElement(resArray, ArrayElementL(tileIndex), ArrayElementL(CreateRefForStack(container, listItem->item)));
			tileIndex++;
		}
		while (listIter = listIter->next);
		*result = (int)resArray;
	}
	return true;
}