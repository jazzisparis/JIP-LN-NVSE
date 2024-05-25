#pragma once

#define MENU_VISIBILITY ((bool*)0x11F308F)
extern TileMenu **g_tileMenuArray;
extern TempObject<UnorderedMap<const char*, UInt32, 0x20, false>> s_menuNameToID;

// 580
class InterfaceManager
{
public:
	struct HighlightedRef
	{
		TESObjectREFR	*refr;
		NiNode			*node;

		void Set(TESObjectREFR *_refr, NiNode *_node)
		{
			refr = _refr;
			NiReplaceObject(&node, _node);
		}

		void Set(HighlightedRef *hRef)
		{
			refr = hRef->refr;
			NiReplaceObject(&node, hRef->node);
		}

		void Clear()
		{
			refr = nullptr;
			if (node)
			{
				NiReleaseObject(node);
				node = nullptr;
			}
		}

		void Replace(HighlightedRef *hRef)
		{
			Clear();
			if (hRef)
			{
				refr = hRef->refr;
				node = hRef->node;
				hRef->refr = nullptr;
				hRef->node = nullptr;
			}
		}
	};

	struct Unk13C
	{
		bhkSimpleShapePhantom	*shape;
		bool					initialized;
		UInt8					pad05[3];
		UInt32					unk08;
	};

	struct Unk164
	{
		UInt32		unk00;
		float		flt04;
		float		flt08;
		UInt32		unk0C;
		UInt32		unk10;
	};

	struct HUDEffects
	{
		UInt32				unk00;			// 00
		NiTArray<UInt32>	array04;		// 04	NiTPrimitiveArray<BGSMenuPacker::PackedMenu*>
		UInt32				unk14[5];		// 14
		UInt8				byte28;			// 28
		UInt8				pad29[3];
		float				flt2C[8];		// 2C
		UInt32				unk4C;			// 4C
		void				*ptr50;			// 50	vtbl: 0x10AFF8C; size: 0x88; c'tor: 0xB8A9E0
		UInt8				byte54;			// 54
		UInt8				pad55[3];
		float				blurRadius;		// 58
		float				scanlineFreq;	// 5C
		float				blurIntensity;	// 60
	};

	enum KeyModifier
	{
		kHeld_Alt =		1,
		kHeld_Ctrl =	2,
		kHeld_Shift =	4
	};

	UInt32					flags;				// 000
	SceneGraph				*sceneGraph004;		// 004
	SceneGraph				*sceneGraph008;		// 008
	UInt32					currentMode;		// 00C	1 = GameMode; 2 = MenuMode
	// checked for 1 at 0x70BA97
	// set to 2 at 0x70BA8D
	// set to 3 at 0x714E96, 0x714E20 - checked at 0x70B94E
	// set to 4 at 0x714D5D, 0x715177 (CloseAllMenus)
	// set to 5 at 0x70B972 - checked at 0x70BA84, 0x70CA14, 0x70CC7A
	UInt8					byte010;			// 010
	UInt8					byte011;			// 011
	UInt8					byte012;			// 012
	UInt8					byte013;			// 013
	UInt32					unk014;				// 014
	UInt32					pickLength;			// 018
	UInt32					unk01C;				// 01C
	UInt8					byte020;			// 020
	UInt8					byte021;			// 021
	UInt8					byte022;			// 022
	UInt8					byte023;			// 023
	UInt32					unk024;				// 024
	TileImage				*cursor;			// 028
	float					flt02C;				// 02C
	float					flt030;				// 030
	float					flt034;				// 034
	float					cursorX;			// 038
	float					flt03C;				// 03C
	float					cursorY;			// 040
	float					mouseWheel;			// 044	-120.0 = down; 120.0 = up
	float					flt048;				// 048
	Tile					*draggedTile;		// 04C
	int						unk050;				// 050
	float					flt054;				// 054
	float					flt058;				// 058
	int						unk05C;				// 05C
	int						unk060;				// 060
	int						unk064;				// 064
	UInt32					unk068[2];			// 068
	tList<TESObjectREFR>	selectableRefs;		// 070
	UInt32					selectedRefIndex;	// 078
	bool					debugText;			// 07C
	UInt8					byte07D;			// 07D
	UInt8					byte07E;			// 07E
	UInt8					byte07F;			// 07F
	NiNode					*uiRootNode;		// 080
	NiNode					*cursorRootNode;	// 084
	UInt32					unk088;				// 088
	BSShaderAccumulator		*shaderAccum08C;	// 08C
	BSShaderAccumulator		*shaderAccum090;	// 090
	ShadowSceneNode			*shadowScene1;		// 094
	ShadowSceneNode			*shadowScene3;		// 098
	TileRect				*menuRoot;			// 09C
	TileRect				*globalsTile;		// 0A0	globals.xml
	NiNode					*debugTextRoot;		// 0A4
	UInt32					unk0A8;				// 0A8
	NiObject				*unk0AC;			// 0AC seen NiAlphaProperty
	UInt32					unk0B0[3];			// 0B0
	Tile					*activeTileAlt;		// 0BC
	UInt32					unk0C0;				// 0C0
	UInt32					unk0C4;				// 0C4
	UInt8					byte0C8;			// 0C8
	UInt8					byte0C9;			// 0C9
	UInt8					byte0CA;			// 0CA
	UInt8					byte0CB;			// 0CB
	Tile					*activeTile;		// 0CC
	Menu					*activeMenu;		// 0D0
	Tile					*tile0D4;			// 0D4
	Menu					*menu0D8;			// 0D8
	UInt32					unk0DC[2];			// 0DC
	UInt8					msgBoxButton;		// 0E4 -1 if no button pressed
	UInt8					byte0E5;			// 0E5
	UInt8					byte0E6;			// 0E6
	UInt8					byte0E7;			// 0E7
	UInt32					unk0E8;				// 0E8
	UInt8					byte0EC;			// 0EC
	UInt8					hasMouseMoved;		// 0ED
	UInt8					byte0EE;			// 0EE
	UInt8					byte0EF;			// 0EF
	TESObjectREFR			*debugSelection;	// 0F0	compared to activated object during Activate
	UInt32					unk0F4;				// 0F4
	UInt32					unk0F8;				// 0F8
	TESObjectREFR			*crosshairRef;		// 0FC
	TESObjectREFR			*refr100;			// 100
	UInt32					unk104[3];			// 104
	UInt8					byte110;			// 110
	UInt8					pad111[3];			// 111
	UInt32					menuStack[10];		// 114
	Unk13C					*ptr13C;			// 13C
	UInt32					unk140;				// 140
	UInt32					unk144;				// 144
	UInt8					byte148;			// 148
	UInt8					isShift;			// 149
	UInt8					byte14A;			// 14A
	UInt8					byte14B;			// 14B
	UInt32					keyModifiers;		// 14C
	UInt32					currentKey;			// 150
	UInt32					keyRepeatStartTime;	// 154
	UInt32					lastKeyRepeatTime;	// 158
	UInt32					unk15C;				// 15C
	UInt32					unk160;				// 160
	Unk164					*ptr164;			// 164
	UInt8					byte168;			// 168
	UInt8					pad169[3];			// 169
	FORenderedMenu			*renderedMenu;		// 16C
	UInt8					byte170;			// 170
	UInt8					byte171;			// 171
	UInt8					byte172;			// 172
	UInt8					byte173;			// 173
	FOPipboyManager			*pipboyManager;		// 174
	HUDEffects				hudEffects;			// 178
	UInt32					highlightState;		// 1DC
	HighlightedRef			highlightMain;		// 1E0
	UInt32					numHighlighted;		// 1E8
	SInt32					flashingRefIndex;	// 1EC
	HighlightedRef			highlightedRefs[32];// 1F0
	UInt32					unk2F0[41];			// 2F0
	NiObject				*unk394;			// 394 seen NiSourceTexture
	UInt32					unk398[47];			// 398
	NiTArray<UInt32>		array454;			// 454 NiTPrimitiveArray@TextureType@BSTextureManager
	NiTArray<UInt32>		array464;			// 464 NiTPrimitiveArray@FilterMode@NiTexturingProperty
	UInt32					unk474[7];			// 474
	UInt8					byte490;			// 490
	UInt8					byte491;			// 491
	UInt8					pad492[2];			// 492
	UInt32					unk494[6];			// 494
	float					scale4AC;			// 4AC
	float					unk4B0;				// 4B0
	UInt8					isRenderedMenuOrPipboyManager;	// 4B4
	UInt8					byte4B5;			// 4B5
	UInt8					byte4B6;			// 4B6
	UInt8					byte4B7;			// 4B7
	UInt32					queuedPipboyTabToSwitchTo;	// 4B8
	UInt32					pipBoyMode;			// 4BC
	void					*pipBoyModeCallback;// 4C0
	UInt32					unk4C4[47];			// 4C4

	__forceinline static InterfaceManager *GetSingleton() {return *(InterfaceManager**)0x11D8A80;}

	UInt32 GetTopVisibleMenuID() const;
	NiAVObject *GetCursorPick() const;
	Tile *GetActiveTile() const;
	void ClearHighlights();
	void AddHighlightedRef(TESObjectREFR *refr);
	void RemoveHighlightedRef(TESObjectREFR *refr);
	bool IsRefHighlighted(TESObjectREFR *refr) const;
};
static_assert(sizeof(InterfaceManager) == 0x580);

extern InterfaceManager *g_interfaceManager;

enum
{
	kMenuType_Min =				0x3E9,
	kMenuType_Message =			kMenuType_Min,
	kMenuType_Inventory,
	kMenuType_Stats,
	kMenuType_HUDMain,
	kMenuType_Loading =			0x3EF,
	kMenuType_Container,
	kMenuType_Dialog,
	kMenuType_SleepWait =		0x3F4,
	kMenuType_Start,
	kMenuType_LockPick,
	kMenuType_Quantity =		0x3F8,
	kMenuType_Map =				0x3FF,
	kMenuType_Book =			0x402,
	kMenuType_LevelUp,
	kMenuType_Repair =			0x40B,
	kMenuType_RaceSex,
	kMenuType_Credits =			0x417,
	kMenuType_CharGen,
	kMenuType_TextEdit =		0x41B,
	kMenuType_Barter =			0x41D,
	kMenuType_Surgery,
	kMenuType_Hacking,
	kMenuType_VATS,
	kMenuType_Computers,
	kMenuType_RepairServices,
	kMenuType_Tutorial,
	kMenuType_SpecialBook,
	kMenuType_ItemMod,
	kMenuType_LoveTester =		0x432,
	kMenuType_CompanionWheel,
	kMenuType_TraitSelect,
	kMenuType_Recipe,
	kMenuType_SlotMachine =		0x438,
	kMenuType_Blackjack,
	kMenuType_Roulette,
	kMenuType_Caravan,
	kMenuType_Trait =			0x43C,
	kMenuType_Max =				kMenuType_Trait,
};

enum MenuSpecialKeyboardInputCode
{
	kEnter =		-2,
	kUpArrow =		1,
	kDownArrow,
	kRightArrow,
	kLeftArrow,
	kSpace =		9,
	kTab,
	kShiftEnter,
	kAltEnter,
	kShiftLeft,
	kShiftRight,
	kPageUp,
	kPageDown
};

class Menu
{
public:
	/*000*/virtual void		Destructor(bool doFree);
	/*004*/virtual void		SetTile(UInt32 idx, Tile *value);
	/*008*/virtual void		HandleLeftClickPress(UInt32 tileID, Tile *activeTile);	// called when the mouse has moved and left click is pressed
	/*00C*/virtual void		HandleClick(SInt32 tileID, Tile *clickedTile);
	/*010*/virtual void		HandleMouseover(UInt32 arg0, Tile *activeTile);	//    Called on mouseover, activeTile is moused-over Tile
	/*014*/virtual void		HandleMouseoverTileAlt(UInt32 arg0, Tile *tile);
	/*018*/virtual void		PostDragTileChange(UInt32 tileID, Tile* newTile, Tile* activeTile); // unused in vanilla
	/*01C*/virtual void		PreDragTileChange(UInt32 tileID, Tile* oldTile, Tile* activeTile); // unused in vanilla
	/*020*/virtual void		HandleActiveMenuClick(UInt32 tileID, Tile *activeTile); // StartMenu, RaceSexMenu, VATSMenu, BookMenu
	/*024*/virtual void		OnClickHeld(UInt32 tileID, Tile* activeTile); // unused by vanilla menus
	/*028*/virtual void		HandleMousewheel(UInt32 tileID, Tile *tile);
	/*02C*/virtual void		Update();	// Called every frame while the menu is active
	/*030*/virtual bool		HandleKeyboardInput(UInt32 inputChar);	// Return false for handling keyboard shortcuts
	/*034*/virtual UInt32	GetID();
	/*038*/virtual bool		HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState);
	/*03C*/virtual bool		HandleControllerInput(int arg0, Tile *tile);
	/*040*/virtual void		OnUpdateUserTrait(int tileVal);	// unimplemented by any vanilla menus
	/*044*/virtual void		HandleControllerConnectOrDisconnect(bool isControllerConnected);

	// 14
	struct ParsedXMLTag
	{
		UInt32		tagType;	// 00
		float		value;		// 04
		String		nameOrSrc;	// 08
		union					// 10
		{
			UInt32	valueID;
			Tile	*tmplTile;
		};
	};

	// 14
	struct TemplateData
	{
		NiFixedString			templateName;	// 00
		TileExtra				*tileExtra;		// 04
		DList<ParsedXMLTag>		parsedTags;		// 08
	};

	TileMenu			*tile;			// 04
	tList<TemplateData>	menuTemplates;	// 08
	UInt32				unk10;			// 10
	UInt32				unk14;			// 14	Same as id (20)
	UInt32				unk18;			// 18
	UInt8				byte1C;			// 1C
	UInt8				byte1D;			// 1D
	UInt8				byte1E;			// 1E
	UInt8				byte1F;			// 1F
	UInt32				id;				// 20
	UInt32				visibilityState;// 24 :: Initialised to 4
	// check 1 at 0xA0B174, 0x70D529, 0x70D592 :: set at 0x712224
	// check 2 at 0x711FF1 :: set 2 at 0xA1D987 (when closes menu), 0xA1DA41
	// check 4 at 0xA1D9EC (when closing menu) :: set at 0x7036A4, 0x71204D
	// check 8 at 0x712194 :: set 8 at 0xA1DB8F (when opening menu), 0x720B39

	__forceinline Tile *AddTileFromTemplate(Tile *destTile, const char *templateName)
	{
		return ThisCall<Tile*>(0xA1DDB0, this, destTile, templateName, 0);
	}
};

struct BGSSaveLoadFileEntry;

extern UInt32 g_valueID_enabled, g_valueID_numVisible;

// 30
template <typename Item> class ListBox
{
public:
	enum
	{
		kFlag_RecalculateHeightsOnInsert = 1,
		kFlag_FreeContChangeOnListItemDestruction = 2, // assumes the object is a ContChangesEntry - do not set this if the object isn't one...
	};

	struct ListItem
	{
		Tile	*tile;
		Item	item;
		bool	isFiltered;
		UInt8	pad09[3];

		ListItem(Tile *_tile, Item _item, bool _isFiltered) : tile(_tile), item(_item), isFiltered(_isFiltered) {}
	};

	typedef int (*CompareItems)(ListItem *item1, ListItem *item2);
	typedef bool (*ShouldFilter)(Item item);

	/*00*/virtual bool	SetSelectedTile(Tile *tile);
	/*04*/virtual Tile	*GetSelectedTile();
	/*08*/virtual Tile	*HandleKeyboardInput(int inputCode);
	/*0C*/virtual bool	IsParentMenu(Menu *menu);
	/*10*/virtual void	ScrollToHighlight();
	/*14*/virtual Tile	*GetItemTile(int index, bool countFromTop);	//	If countFromTop is 0, looks for a tile with kTileValue_listindex == index
	/*18*/virtual void	Destroy(bool doFree);
	/*1C*/virtual void	ClearList();
	/*20*/virtual void	Sort(CompareItems cmpFunc);

	tList<ListItem>	list;			// 04
	Tile			*parentTile;	// 0C
	Tile			*selected;		// 10
	Tile			*scrollBar;		// 14
	const char		*templateName;	// 18
	UInt16			itemCount;		// 1C
	UInt16			pad1E;			// 1E
	float			totalHeight;	// 20
	float			listIndex;		// 24
	float			scrollPos;		// 28
	UInt16			flags;			// 2C
	UInt16			pad2E;			// 2E

	Item GetSelected() const
	{
		if (selected)
		{
			auto iter = list.Head();
			ListItem *listItem;
			do
			{
				listItem = iter->data;
				if (listItem && (listItem->tile == selected))
					return listItem->item;
			}
			while (iter = iter->next);
		}
		return NULL;
	}

	bool IsEnabled() const
	{
		if (!g_valueID_enabled)
			g_valueID_enabled = Tile::TraitNameToID("_enabled");
		return parentTile && parentTile->GetValueFloat(g_valueID_enabled);
	}

	int GetNumVisibleItems()
	{
		return ThisCall<int>(0x71AE60, this);
	}

	void Filter(ShouldFilter callback)
	{
		ThisCall(0x729FE0, this, callback);
	}

	Tile *Insert(Item item, const char *text, CompareItems sortingFunction = nullptr, const char *_templateName = nullptr)
	{
		if (!parentTile) return nullptr;
		auto _template = _templateName ? _templateName : templateName;
		if (!_template) return nullptr;

		Tile *newTile = parentTile->GetParentMenu()->AddTileFromTemplate(parentTile, _template);
		if (!newTile->GetValue(kTileValue_id))
			newTile->SetFloat(kTileValue_id, -1.0F);
		if (text)
			newTile->SetString(kTileValue_string, text);

		ListItem *listItem = new (Game_HeapAlloc<ListItem>()) ListItem(newTile, item, false);
		if (sortingFunction)
		{
			ThisCall(0x7A7EB0, &list, listItem, sortingFunction); // InsertSorted
			if (flags & kFlag_RecalculateHeightsOnInsert)
				ThisCall(0x71A670, this);
		}
		else
		{
			list.Append(listItem);
			if (flags & kFlag_RecalculateHeightsOnInsert)
			{
				ThisCall(0x7269D0, this, newTile);
				ThisCall(0x71AD30, this);
			}
			newTile->SetFloat(kTileValue_listindex, itemCount++);
		}

		if (itemCount == 1)
		{
			if (!g_valueID_numVisible)
				g_valueID_numVisible = Tile::TraitNameToID("_number_of_visible_items");
			if (parentTile->GetValueFloat(g_valueID_numVisible) > 0)
			{
				TileValue *valPtr = parentTile->GetValue(kTileValue_height);
				valPtr->RemoveFromReactionMap();
				valPtr->AddRefValueAction(Tile::kAction_copy, newTile, kTileValue_height);

				float numVisible = parentTile->GetValueFloat(g_valueID_numVisible);
				valPtr->AddFloatAction(Tile::kAction_mul, numVisible);
				valPtr->Refresh();
			}
		}

		return newTile;
	}
};

// 94
class MessageMenu : public Menu			// 1001
{
public:
	TileRect			*tile28;		// 28
	TileText			*tile2C;		// 2C
	TileImage			*tile30;		// 30
	TileText			*tile34;		// 34
	TileImage			*tile38;		// 38
	TileImage			*tile3C;		// 3C
	ListBox<int>		buttonList;		// 40
	UInt8				unk70;			// 70
	UInt8				pad71[3];		// 71
	float				unk74[3];		// 74
	UInt32				unk80[2];		// 80
	UInt32				tickCount;		// 88
	UInt8				unk8C;			// 8C
	UInt8				unk8D;			// 8D
	UInt8				pad8E[2];		// 8E
	UInt32				unk90;			// 90

	__forceinline static MessageMenu *Get() {return *(MessageMenu**)0x11DA4F0;}
};

typedef ListBox<ContChangesEntry*> MenuItemEntryList;

struct HotKeyWheel
{
	TileRect	*parent;		// 00
	TileRect	*hotkeys[8];	// 04
	UInt8		byte24;			// 24
	UInt8		pad25[3];		// 25
	UInt32		unk28;			// 28
	UInt32		selectedHotkeyTrait;// 2C
	UInt32		selectedTextTrait;	// 30
};
static_assert(sizeof(HotKeyWheel) == 0x34);

// 124
class InventoryMenu : public Menu		// 1002
{
public:
	struct ScrollPos
	{
		SInt32		listIndex;
		SInt32		currentValue;
	};

	TileRect				*tile028;		// 028	IM_Headline_PlayerCapsInfo
	TileRect				*tile02C;		// 02C	IM_Headline_PlayerHPInfo
	TileRect				*tile030;		// 030	IM_Headline_PlayerDRInfo
	TileRect				*tile034;		// 034	IM_Headline_PlayerWGInfo
	TileImage				*tile038;		// 038	IM_InventoryList
	TileRect				*tile03C;		// 03C	IM_HotKeyWheel
	TileImage				*tile040;		// 040	IM_EquipButton
	TileImage				*tile044;		// 044	IM_DropButton
	TileImage				*tile048;		// 048	IM_RepairButton
	TileImage				*tile04C;		// 04C	IM_HotkeyButton
	TileImage				*tile050;		// 050	IM_CancelButton
	TileImage				*tile054;		// 054	IM_ItemIcon
	TileRect				*tile058;		// 058	IM_ItemInfoRect
	TileRect				*tile05C;		// 05C	IM_Tabline
	TileRect				*tile060;		// 060	DAMInfo
	TileRect				*tile064;		// 064	DPSInfo
	TileRect				*tile068;		// 068	StrengthReqInfo
	TileRect				*tile06C;		// 06C	DamageResistInfo
	TileRect				*tile070;		// 070	DamageThresholdInfo
	TileImage				*tile074;		// 074	IM_ModButton
	TileImage				*tile078;		// 078	IM_ItemIconBadge
	TileRect				*tile07C;		// 07C	IM_Headline_PlayerDTInfo
	TileText				*tile080;		// 080	IM_StrReq
	UInt32					filter;			// 084
	ScrollPos				tabScrollPos[6];// 088	The scroll index for Weapons, Apparel, Aid, Misc, Ammo and the Keyring
	MenuItemEntryList		itemList;		// 0B8
	HotKeyWheel				hotKeyWheel;	// 0E8
	tList<ContChangesEntry>	changedItemsList;	// 11C

	__forceinline static InventoryMenu *Get() {return *(InventoryMenu**)0x11D9EA4;}
	__forceinline static ContChangesEntry *Selection() {return *(ContChangesEntry**)0x11D9EA8;}
};
static_assert(sizeof(InventoryMenu) == 0x124);

// 2A4
class StatsMenu : public Menu			// 1003
{
public:
	struct AlchItemData
	{
		AlchemyItem		*alchItem;
		TileImage		*tileImg;
		void			*dataPtr;
		bool			(*callback)(void *arg);
	};

	struct StatusEffect;

	AlchItemData					alchItemData[4];	// 028	0: Stimpak, 1: Rad-X, 2: RadAway, 3: Doctor's Bag
	BSSimpleList<EffectSetting>		effectList;			// 068
	UInt32							unk074[4];			// 074
	BSSimpleList<StatusEffect>		statusEffList;		// 084
	TileImage						*tile090;			// 090
	TileImage						*tile094;			// 094
	TileImage						*tile098;			// 098
	TileImage						*tile09C;			// 09C
	TileImage						*tile0A0;			// 0A0
	TileImage						*tile0A4;			// 0A4
	TileImage						*tile0A8;			// 0A8
	TileImage						*tile0AC;			// 0AC
	TileImage						*tile0B0;			// 0B0
	TileImage						*tile0B4;			// 0B4
	TileImage						*tile0B8;			// 0B8
	TileImage						*tile0BC;			// 0BC
	TileImage						*tile0C0;			// 0C0
	TileRect						*tile0C4;			// 0C4
	TileImage						*tile0C8;			// 0C8
	TileRect						*tile0CC;			// 0CC
	TileImage						*tile0D0;			// 0D0
	TileRect						*tile0D4;			// 0D4
	TileImage						*tile0D8;			// 0D8
	TileRect						*tile0DC;			// 0DC
	TileImage						*tile0E0;			// 0E0
	TileRect						*tile0E4;			// 0E4
	TileImage						*tile0E8;			// 0E8
	TileRect						*tile0EC;			// 0EC
	TileImage						*tile0F0;			// 0F0
	TileImage						*tile0F4;			// 0F4
	TileImage						*tile0F8;			// 0F8
	TileText						*tile0FC;			// 0FC
	TileImage						*tile100;			// 100
	TileImage						*tile104;			// 104
	TileImage						*tile108;			// 108
	TileImage						*tile10C;			// 10C
	TileImage						*tile110;			// 110
	TileImage						*tile114;			// 114
	TileImage						*tile118;			// 118
	TileText						*tile11C;			// 11C
	TileImage						*tile120;			// 120
	TileText						*tile124;			// 124
	TileText						*tile128;			// 128
	TileText						*tile12C;			// 12C
	TileRect						*tile130;			// 130
	TileRect						*tile134;			// 134
	TileRect						*tile138;			// 138
	TileRect						*tile13C;			// 13C
	TileText						*tile140;			// 140
	TileText						*tile144;			// 144
	TileImage						*tile148;			// 148
	TileImage						*tile14C;			// 14C
	TileText						*tile150;			// 150
	TileImage						*tile154;			// 154
	TileImage						*tile158;			// 158
	TileText						*tile15C;			// 15C
	TileImage						*tile160;			// 160
	TileImage						*tile164;			// 164
	TileImage						*tile168;			// 168
	TileImage						*tile16C;			// 16C
	TileImage						*tile170;			// 170
	TileImage						*tile174;			// 174
	TileImage						*tile178;			// 178
	TileImage						*tile17C;			// 17C
	ListBox<UInt32>					SPECIALList;		// 180
	ListBox<UInt32>					skillList;			// 1B0
	ListBox<PerkRank*>				perkRankList;		// 1E0
	ListBox<UInt32>					miscStatIDList;		// 210
	ListBox<StatusEffect*>			statusEffListBox;	// 240
	ListBox<TESReputation*>			reputationList;		// 270
	UInt8							isInHealLimbSelect;	// 2A0
	UInt8							pad2A0[3];			// 2A1

	__forceinline static StatsMenu *Get() {return *(StatsMenu**)0x11DACE0;}
};

// 278
class HUDMainMenu : public Menu			// 1004
{
public:
	struct QueuedMessage
	{
		char	msgText[0x204];			// 000
		char	iconPate[MAX_PATH];		// 204
		char	soundPath[MAX_PATH];	// 308
		float	displayTime;			// 40C
	};

	struct SubtitleData
	{
		char		text[0x104];
		UInt32		startTime;
		float		displayTime;
		NiPoint3	pos;
		SInt32		priority;
	};

	struct QueuedQuestText
	{
		char		msg[0x104];
		bool		byte104;
		bool		byte105;
	};

	enum VisibilityFlags
	{
		kActionPoints =			0x1,
		kHitPoints =			0x2,
		kRadiationMeter =		0x4,
		kEnemyHealth =			0x8,
		kQuestReminder =		0x10,
		kRegionLocation =		0x20,
		kReticleCenter =		0x40,
		kSneakMeter =			0x80,
		kMessages =				0x100,
		kInfo =					0x200,
		kSubtibles =			0x400,
		kHotkeys =				0x800,
		kXpMeter =				0x1000,
		kBreathMeter =			0x2000,
		kExplosivePositioning = 0x4000,
		kCrippledLimbIndicator = 0x8000,
		kHardcoreMode =			0x10000,
	};

	enum HUDStates
	{
		kRECALCULATE = 0x1,
		kNormal,
		kPipBoy,
		kPause,
		kLoading,
		kDialog,
		kVATSMenu,
		kVATSPlayback,
		kContainer,
		kBeginSit,
		kSleepWait,
		kPlayerDisabledControls,
		kUnusedD,
		kLevelUpMenu,
		kHacking,
		kComputers,
		kMessage,
		kSpecialBook,
		kLoveTester,
		kVanityCam,
		kUnused15,
		kAiming,
		kAimingScope,
		kIntroMovie,
		kGambling,
	};

	UInt32							unk028;				// 028
	TileImage						*tile02C;			// 02C	HitPoints\meter
	TileText						*tile030;			// 030	HitPoints\justify_right_text
	TileRect						*tile034;			// 034	HitPoints\compass_window\compass_icon_group
	TileRect						*tile038;			// 038	HitPoints\compass_window\compass_icon_group
	TileRect						*tile03C;			// 03C	HitPoints\compass_window\compass_icon_group
	TileImage						*tile040;			// 040	HitPoints\compass_window
	TileImage						*tile044;			// 044	ActionPoints\meter
	TileText						*tile048;			// 048	ActionPoints\justify_right_text
	TileText						*tile04C;			// 04C	ActionPoints\justify_right_text
	TileImage						*tile050;			// 050	ActionPoints\meter
	TileImage						*tile054;			// 054	ActionPoints\MeterBackground
	TileText						*tile058;			// 058	ActionPoints\justify_right_text
	TileRect						*tile05C;			// 05C	QuestReminder\QuestStages
	TileRect						*tile060;			// 060	QuestReminder\QuestAdded
	TileText						*tile064;			// 064	Region_Location\justify_left_text
	TileText						*tile068;			// 068	Region_Location\justify_left_text
	TileImage						*tile06C;			// 06C	RadiationMeter\radiation_bracket
	TileImage						*tile070;			// 070	RadiationMeter\radiation_pointer
	TileText						*tile074;			// 074	RadiationMeter\radiation_text_value
	TileText						*tile078;			// 078	RadiationMeter\radiation_text
	TileImage						*tile07C;			// 07C	EnemyHealth\enemy_health_bracket
	TileImage						*tile080;			// 080	EnemyHealth\meter
	TileText						*tile084;			// 084	EnemyHealth\justify_center_text
	TileText						*sneakLabel;		// 088	SneakMeter\sneak_nif
	TileImage						*tile08C;			// 08C	Messages\message_icon
	TileText						*tile090;			// 090	Messages\justify_left_text
	TileImage						*tile094;			// 094	Messages\message_bracket
	TileText						*tile098;			// 098	Subtitles\justify_center_text
	TileRect						*tile09C;			// 09C	Info\justify_center_hotrect
	TileText						*tile0A0;			// 0A0	Info\justify_center_hotrect\PCShortcutLabel
	TileImage						*tile0A4;			// 0A4	Info\justify_center_hotrect\xbox_button
	TileText						*tile0A8;			// 0A8	Info\justify_center_text
	TileText						*tile0AC;			// 0AC	Info\justify_center_text
	TileText						*tile0B0;			// 0B0	Info\justify_center_text
	TileText						*tile0B4;			// 0B4	Info\justify_right_text
	TileText						*tile0B8;			// 0B8	Info\justify_left_text
	TileText						*tile0BC;			// 0BC	Info\justify_right_text
	TileText						*tile0C0;			// 0C0	Info\justify_left_text
	TileImage						*tile0C4;			// 0C4	Info\info_seperator
	TileRect						*tile0C8;			// 0C8	Hokeys\hotkey_selector
	TileText						*tile0CC;			// 0CC	Hokeys\justify_center_text
	TileImage						*tile0D0;			// 0D0	HitPoints\left_bracket
	TileImage						*tile0D4;			// 0D4	ActionPoints\right_bracket
	TileImage						*tile0D8;			// 0D8	XPMeter\XPBracket
	TileText						*tile0DC;			// 0DC	XPMeter\XPAmount
	TileText						*tile0E0;			// 0E0	XPMeter\XPLabel
	TileImage						*tile0E4;			// 0E4	XPMeter\XPPointer
	TileText						*tile0E8;			// 0E8	XPMeter\XPLastLevel
	TileText						*tile0EC;			// 0EC	XPMeter\XPNextLevel
	TileText						*tile0F0;			// 0F0	XPMeter\XPLevelUp
	TileImage						*tile0F4;			// 0F4	ReticleCenter\reticle_center
	TileImage						*tile0F8;			// 0F8	crippled_limb_indicator\Face
	TileImage						*tile0FC;			// 0FC	crippled_limb_indicator\Head
	TileImage						*tile100;			// 100	crippled_limb_indicator\Torso
	TileImage						*tile104;			// 104	crippled_limb_indicator\Left_Arm
	TileImage						*tile108;			// 108	crippled_limb_indicator\Right_Arm
	TileImage						*tile10C;			// 10C	crippled_limb_indicator\Left_Leg
	TileImage						*tile110;			// 110	crippled_limb_indicator\Right_Leg
	TileRect						*tile114;			// 114	ActionPoints
	TileRect						*tile118;			// 118	HitPoints
	TileRect						*tile11C;			// 11C	RadiationMeter
	TileRect						*tile120;			// 120	EnemyHealth
	TileRect						*tile124;			// 124	QuestReminder
	TileRect						*tile128;			// 128	Region_Location
	TileRect						*tile12C;			// 12C	ReticleCenter
	TileRect						*tile130;			// 130	SneakMeter
	TileRect						*tile134;			// 134	Messages
	TileRect						*tile138;			// 138	Info
	TileRect						*tile13C;			// 13C	Subtitles
	TileRect						*tile140;			// 140	Hokeys
	TileRect						*tile144;			// 144	XPMeter
	Tile3D							*tile148;			// 148	BreathMeter
	TileRect						*tile14C;			// 14C	Explosive_positioning_rect
	TileRect						*tile150;			// 150	crippled_limb_indicator
	TileImage						*tile154;			// 154	DDTIcon
	TileImage						*tile158;			// 158	DDTIconEnemy
	TileText						*tile15C;			// 15C	AmmoTypeLabel
	TileRect						*tile160;			// 160	HardcoreMode
	TileText						*tile164;			// 164	HardcoreMode\Dehydration
	TileText						*tile168;			// 168	HardcoreMode\Sleep
	TileText						*tile16C;			// 16C	HardcoreMode\Hunger
	TileImage						*tile170;			// 170	DDTIcon
	TileImage						*tile174;			// 174	DDTIconEnemyAP
	TileText						*tile178;			// 178	HardcoreMode\Rads
	TileText						*tile17C;			// 17C	HardcoreMode\LMBs
	TileImage						*tile180;			// 180	CNDArrows
	UInt32							unk184;				// 184
	float							flt188;				// 188
	tList<QueuedMessage>			queuedMessages;		// 18C
	UInt32							currMsgStartTime;	// 194
	BSSimpleArray<SubtitleData>		subtitlesArr;		// 198
	SubtitleData					*currentSubtitle;	// 1A8 
	Sound							radiationSound;		// 1AC
	TESObjectREFR					*crosshairRef;		// 1B8
	UInt32							visibilityFlags;	// 1BC
	UInt32							visibilityOverrides;	// 1C0
	UInt32							state;				// 1C4
	HotKeyWheel						hotKeyWheel;		// 1C8
	UInt8							isUsingScope;		// 1FC
	UInt8							byte1FD[3];			// 1FD
	NiControllerSequence			*niContSeq;			// 200
	UInt8							isLevelUpQueued;	// 204
	UInt8							autoDisplayObjectives;	// 205
	UInt8							pad206[2];			// 206
	UInt32							unk208;				// 208
	UInt32							questTargets;		// 20C
	UInt32							unk210;				// 210
	UInt32							compassWidth;		// 214
	UInt32							maxCompassAngle;	// 218
	Actor							*healthTarget;		// 21C
	UInt32							unk220;				// 220
	Timer							timer;				// 224
	UInt32							enemyHealthBarTime;	// 244
	UInt32							lastQuestUpdReminderTime;	// 248
	UInt32							timeArmorIcon;		// 24C
	UInt32							timeDTIcon;			// 250
	UInt8							doClearQuestUpdates;// 254
	UInt8							byte255;			// 255
	UInt8							byte256;			// 256
	UInt8							byte257;			// 257
	tList<QueuedQuestText>			queuedQuestTextList;// 258
	UInt8							isQuestAddedTextVisible;// 260
	UInt8							isQuestStagesTextVisible;	// 261
	UInt8							pad262[2];			// 262
	tList<UInt32>					xpMessages;			// 264
	tList<UInt32>					list26C;			// 26C
	float							hudShake;			// 274

	__forceinline static HUDMainMenu *Get() {return *(HUDMainMenu**)0x11D96C0;}

	void ClearMessageQueue()
	{
		if (!queuedMessages.Empty())
		{
			queuedMessages.DeleteAll();
			currMsgStartTime = 0;
			tile08C->SetFloat(kTileValue_alpha, 0);
			tile090->SetFloat(kTileValue_alpha, 0);
			tile094->SetFloat(kTileValue_alpha, 0);
		}
	}
};
static_assert(sizeof(HUDMainMenu) == 0x278);

// 5C0
class LoadingMenu : public Menu			// 1007
{
public:
	Tile3D					*tile028;		// 028
	TileText				*tile02C;		// 02C
	TileText				*tile030;		// 030
	TileText				*tile034;		// 034
	TileText				*tile038;		// 038
	TileText				*tile03C;		// 03C
	TileText				*tile040;		// 040
	TileText				*tile044;		// 044
	TileText				*tile048;		// 048
	TileText				*tile04C;		// 04C
	TileImage				*tile050;		// 050
	Tile3D					*tile054;		// 054
	TileImage				*tile058;		// 058
	TileText				*tile05C;		// 05C
	TileText				*tile060;		// 060
	TileText				*tile064;		// 064
	TileRect				*tile068;		// 068
	TileText				*tile06C;		// 06C
	TileRect				*tile070;		// 070
	TileText				*tile074;		// 074
	TileText				*tile078;		// 078
	TileText				*tile07C;		// 07C
	TileText				*tile080;		// 080
	TileText				*tile084;		// 084
	TileText				*tile088;		// 088
	TileText				*tile08C;		// 08C
	TileText				*tile090;		// 090
	TileRect				*tile094;		// 094
	TileText				*tile098;		// 098
	TileText				*tile09C;		// 09C
	TileText				*tile0A0;		// 0A0
	TileRect				*tile0A4;		// 0A4
	TileImage				*tile0A8;		// 0A8
	TileImage				*tile0AC;		// 0AC
	TileText				*tile0B0;		// 0B0
	TileText				*tile0B4;		// 0B4
	TileImage				*tile0B8;		// 0B8
	TileImage				*tile0BC;		// 0BC
	UInt32					unk0C0;			// 0C0
	UInt32					unk0C4;			// 0C4
	UInt32					unk0C8;			// 0C8
	UInt32					unk0CC;			// 0CC
	DList<Tile>				list0D0;		// 0D0
	DList<Tile>				list0DC;		// 0DC
	DList<Tile>				list0E8;		// 0E8
	DList<Tile>				list0F4;		// 0F4
	DList<Tile>				list100;		// 100
	UInt32					unk10C[43];		// 10C
	String					lsPath1B8;		// 1B8
	UInt32					unk1C0[4];		// 1C0
	tList<TESLoadScreen>	loadScrList;	// 1D0
	TESLoadScreen			**ptr1D8;		// 1D8
	TESLoadScreen			*loadScr1DC;	// 1DC
	UInt32					unk1E0;			// 1E0
	UInt32					unk1E4;			// 1E4
	TESForm					*worldOrCell;	// 1E8	TESWorldSpace or TESObjectCELL
	UInt32					unk1EC[8];		// 1EC
	NiSourceTexture			*srcTexture[4];	// 20C
	UInt32					unk21C;			// 21C
	UInt8					byte220[2];		// 220
	UInt16					flags;			// 222
	UInt32					unk224[231];	// 224
};
static_assert(sizeof(LoadingMenu) == 0x5C0);

// 10C
class ContainerMenu : public Menu		// 1008
{
public:
	TileImage			*tile028;			// 028
	TileText			*tile02C;			// 02C
	TileImage			*tile030;			// 030
	TileText			*tile034;			// 034
	TileImage			*tile038;			// 038
	TileImage			*tile03C;			// 03C
	TileText			*tile040;			// 040
	TileImage			*tile044;			// 044
	TileImage			*tile048;			// 048
	TileImage			*tile04C;			// 04C
	TileImage			*tile050;			// 050
	TileImage			*tile054;			// 054
	TileImage			*tile058;			// 058
	TileRect			*tile05C;			// 05C
	TileRect			*tile060;			// 060
	TileRect			*tile064;			// 064
	TileRect			*tile068;			// 068
	TileRect			*tile06C;			// 06C
	TileRect			*tile070;			// 070
	TESObjectREFR		*containerRef;		// 074
	tList<void>			list078;			// 078
	UInt32				unk080;				// 080
	UInt32				valueTransfer;		// 084
	UInt8				hasPickedPocket;	// 088
	UInt8				hasFailedPickpocket;// 089
	UInt8				byte08A;			// 08A
	UInt8				byte08B;			// 08B
	UInt32				leftFilter;			// 08C
	UInt32				rightFilter;		// 090
	UInt32				menuSoundID;		// 094
	MenuItemEntryList	leftItems;			// 098
	MenuItemEntryList	rightItems;			// 0C8
	MenuItemEntryList	*currentItems;		// 0F8
	UInt32				unk0FC;				// 0FC
	Sound				menuSound;			// 100

	__forceinline static ContainerMenu *Get() {return *(ContainerMenu**)0x11D93F8;}
	__forceinline static ContChangesEntry *Selection() {return *(ContChangesEntry**)0x11D93FC;}
	__forceinline void Refresh(TESForm *itemForm)
	{
		ThisCall(0x75C280, this, itemForm);
	}
};
static_assert(sizeof(ContainerMenu) == 0x10C);

// 13C
class DialogMenu : public Menu			// 1009
{
public:
	UInt32				unk028[2];			// 028
	TileImage			*tile030;			// 030
	TileText			*tile034;			// 034
	TileText			*tile038;			// 038
	TileImage			*tile03C;			// 03C
	ListBox<int>		topicList;			// 040
	void				*unk070;			// 070
	UInt32				unk074;				// 074
	TESTopicInfo		*info078;			// 078
	UInt32				unk07C;				// 07C
	TESObjectREFR		*partnerRef;		// 080
	float				unk084[2];			// 084
	void				*unk08C;			// 08C
	void				*unk090;			// 090
	TESTopicInfo		*infos094[16];		// 094
	void				*unk0D4;			// 0D4
	UInt32				unk0D8[3];			// 0D8
	TESTopicInfo		*infos0E4[8];		// 0E4
	UInt32				unk104[2];			// 104
	int					lastClickTime;		// 10C
	TESTopicInfo		*infos110[6];		// 110
	float				unk128;				// 128
	void				*unk12C;			// 12C	ImageSpaceModifierInstanceDOF
	UInt32				unk130[2];			// 130
	UInt8				isBarterMenuShown;	// 138
	UInt8				byte139[3];			// 139

	__forceinline static DialogMenu *Get() {return *(DialogMenu**)0x11D9510;}
};
static_assert(sizeof(DialogMenu) == 0x13C);

// 4C
class SleepWaitMenu : public Menu		// 1012
{
public:
	TileText			*tile28;		// 28
	TileImage			*tile2C;		// 2C
	TileText			*tile30;		// 30
	TileText			*tile34;		// 34
	TileImage			*tile38;		// 38
	TileImage			*tile3C;		// 3C
	UInt32				unk40;			// 40
	float				flt44;			// 44
	float				selectedHours;	// 48

	__forceinline static SleepWaitMenu *Get() {return *(SleepWaitMenu**)0x11DA920;}
};
static_assert(sizeof(SleepWaitMenu) == 0x4C);

// 1D4
class StartMenu : public Menu			// 1013
{
public:
	// 10
	class Option
	{
	public:
		virtual void	Destructor(bool doFree);

		enum OptionFlags
		{
			kOptFlag_MainMenu =			0x1,
			kOptFlag_PauseMenu =		0x2,
			kOptFlag_SettingsMenu =		0x4,
			kOptFlag_GameplayMenu =		0x8,
			kOptFlag_DisplayMenu =		0x10,
			kOptFlag_AudioMenu =		0x20,
			kOptFlag_ControlsMenu =		0x40,
			kOptFlag_NewGameMenu =		0x80,
			kOptFlag_ContinueGameMenu =	0x100,
			kOptFlag_QuitGameMenu =		0x2000
		};

		const char		*optionName;				// 04
		void			(*followupOption)(void);	// 08
		UInt32			flags;						// 0C
	};

	// 30
	class UserOption : public Option
	{
	public:
		enum UserOptionType
		{
			// kToggleCycles will show a right arrow at max value which will cycle it back to the lowest value
			kToggleCycles =	0,
			kToggleNoCycle,
			kSlider
		};

		const char		*templateName;					// 10
		UInt32			currValue;						// 14
		UInt32			unk18;							// 18
		UInt32			type;							// 1C
		UInt32			numOptions;						// 20
		UInt32			buttonWidth;					// 24
		const char		**currValueName;				// 28
		void			(*onSelection)(UserOption*);	// 2C
	};

	TileImage						*tile028;		// 028
	TileImage						*tile02C;		// 02C
	TileImage						*tile030;		// 030
	TileImage						*tile034;		// 034
	TileImage						*tile038;		// 038
	TileImage						*tile03C;		// 03C
	TileImage						*tile040;		// 040
	TileImage						*tile044;		// 044
	TileText						*tile048;		// 048
	TileText						*tile04C;		// 04C
	TileText						*tile050;		// 050
	TileImage						*tile054;		// 054
	TileText						*tile058;		// 058
	Tile3D							*tile05C;		// 05C
	TileImage						*tile060;		// 060
	TileImage						*tile064;		// 064
	TileText						*tile068;		// 068
	TileImage						*tile06C;		// 06C
	TileText						*tile070;		// 070
	TileText						*tile074;		// 074
	TileText						*tile078;		// 078
	TileImage						*tile07C;		// 07C
	TileText						*tile080;		// 080
	ListBox<Option*>				options084;		// 084
	ListBox<Option*>				options0B4;		// 0B4
	ListBox<Option*>				options0E4;		// 0E4
	ListBox<Option*>				options114;		// 114
	ListBox<int>					listBox144;		// 144
	ListBox<BGSSaveLoadFileEntry*>	listBox174;		// 174
	UInt32							unk1A4;			// 1A4
	UInt32							flags;			// 1A8
	UInt32							unk1AC;			// 1AC
	UInt32							unk1B0;			// 1B0
	Option							*option1B4;		// 1B4
	NiSourceTexture					*texture1B8;	// 1B8
	UInt32							unk1BC;			// 1BC
	TileImage						*tile1C0;		// 1C0
	TileImage						*tile1C4;		// 1C4
	UInt32							controlsDevice;	// 1C8
	UInt32							unk1CC;			// 1CC
	UInt32							unk1D0;			// 1D0

	__forceinline static StartMenu *Get() {return *(StartMenu**)0x11DAAC0;}
};
static_assert(sizeof(StartMenu) == 0x1D4);

// E4
class LockPickMenu : public Menu		// 1014
{
public:
	UInt32					stage;			// 28
	TileRect				*tile2C;		// 2C
	TileRect				*tile30;		// 30
	TileRect				*tile34;		// 34
	TileImage				*tile38;		// 38
	TileText				*tile3C;		// 3C
	TileImage				*tile40;		// 40
	TileImage				*tile44;		// 44
	TileText				*tile48;		// 48
	TileImage				*tile4C;		// 4C
	TileImage				*tile50;		// 50
	TileImage				*tile54;		// 54
	TileText				*tile58;		// 58
	TileText				*tile5C;		// 5C
	TileImage				*tile60;		// 60
	TileImage				*tile64;		// 64
	TileImage				*tile68;		// 68
	TESObjectREFR			*targetRef;		// 6C
	UInt32					lockLevel;		// 70
	UInt32					skillLevel;		// 74
	float					fSkillLevel;	// 78
	UInt32					unk7C;			// 7C
	UInt32					unk80;			// 80
	float					flt84;			// 84
	UInt32					sweetSpotLen;	// 88
	float					cylinderAngle;	// 8C
	float					pickAngle;		// 90
	float					pickHealth;		// 94
	UInt8					isForceRotate;	// 98
	UInt8					byte99;			// 99
	UInt8					pad9A[2];		// 99
	NiControllerSequence	*ctrlSeq9C;		// 9C
	NiControllerSequence	*ctrlSeqA0;		// A0
	NiControllerManager		*ctrlManager;	// A4
	NiControllerSequence	*ctrlSeqA8;		// A8
	NiControllerSequence	*ctrlSeqAC;		// AC
	NiControllerSequence	*ctrlSeqB0;		// B0
	NiQuaternion			quaternionB4;	// B4
	NiQuaternion			quaternionC4;	// C4
	NiQuaternion			quaternionD4;	// D4

	__forceinline static LockPickMenu *Get() {return *(LockPickMenu**)0x11DA204;}
};
static_assert(sizeof(LockPickMenu) == 0xE4);

// 44
class QuantityMenu : public Menu		// 1016
{
public:
	virtual void		Unk_12(void);

	TileRect			*tile28;		// 28	QM_AmountMeter
	TileImage			*tile2C;		// 2C	QM_DecreaseArrow
	TileImage			*tile30;		// 30	QM_IncreaseArrow
	TileText			*tile34;		// 34	QM_AmountChosen
	TileImage			*tile38;		// 38	QM_OKButton
	TileImage			*tile3C;		// 3C	QM_CancelButton
	float				currentQtt;		// 40

	__forceinline static QuantityMenu *Get() {return *(QuantityMenu**)0x11DA618;}

	static TileMenu *ShowMenu(int maxCount, void (*callback)(int), int defaultCount)
	{
		return CdeclCall<TileMenu*>(0x7ABA00, maxCount, callback, defaultCount);
	}
};

// 230
class MapMenu : public Menu				// 1023
{
public:
	enum MapMenuTabs
	{
		kTab_LocalMap = 0x20,
		kTab_WorldMap,
		kTab_Quests,
		kTab_Misc,
		kTab_Radio
	};

	TileText						*tile028;		// 028	MM_MainRect\MM_HeadlineRect\MM_Headline_LocationInfo
	TileText						*tile02C;		// 02C	MM_MainRect\MM_HeadlineRect\MM_Headline_TimeDateInfo
	TileImage						*tile030;		// 030	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMap_ParentImage
	TileImage						*tile034;		// 034	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMapCursor
	TileImage						*tile038;		// 038	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMap_ParentImage
	TileImage						*tile03C;		// 03C	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMapCursor
	TileRect						*tile040;		// 040	MM_MainRect\MM_Highlight_ClipWindow\MM_MapHighlightBox
	TileImage						*tile044;		// 044	MM_MainRect\MM_QuestsList
	TileImage						*tile048;		// 048	MM_MainRect\MM_NotesList
	TileImage						*tile04C;		// 04C	MM_MainRect\MM_NotesList\MM_Notes_SelectedNoteHighlight
	TileImage						*tile050;		// 050	MM_MainRect\MM_RadioStationList
	TileImage						*tile054;		// 054	MM_MainRect\MM_ButtonRect\MM_ButtonA
	TileImage						*tile058;		// 058	MM_MainRect\MM_ButtonRect\MM_ButtonX
	TileRect						*tile05C;		// 05C	MM_MainRect\MM_DataRect
	TileImage						*tile060;		// 060	MM_MainRect\MM_DataRect\MM_DataTextRect
	TileImage						*tile064;		// 064	MM_MainRect\MM_DataRect\MM_QuestObjectivesList
	TileImage						*tile068;		// 068	MM_MainRect\MM_WaveformRect\MM_Waveform
	TileRect						*tile06C;		// 06C	MM_Tabline
	TileImage						*tile070;		// 070	MM_MainRect\MM_ButtonRect\MM_ButtonY
	TileImage						*tile074;		// 074	MM_MainRect\MM_ChallengeList
	TileImage						*tile078;		// 078	MM_MainRect\MM_ChallengeList\MM_Challenge_SelectedHighlight
	TileText						*tile07C;		// 07C	MM_MainRect\MM_Headline_ChallengeType
	UInt8							currentTab;		// 080
	UInt8							pad081[3];		// 081
	TileImage						*placedMarker;	// 084
	UInt32							unk088;			// 088
	UInt8							isShowAllNotes;	// 08C
	UInt8							byte08D;		// 08D
	UInt8							byte08E;		// 08E
	UInt8							byte08F;		// 08F
	BGSNote							*currentNote;	// 090
	UInt32							timeNoteViewed;	// 094
	UInt32							someSoundsList[4];	// 098
	BSSimpleArray<String>			arr0A8;			// 0A8
	Sound							*holotape;		// 0B8
	UInt8							isHolotapePlaying;	// 0BC
	UInt8							pad0BD;			// 0BD
	UInt8							pad0BE;			// 0BE
	UInt8							pad0BF;			// 0BF
	float							holotapeTotalTime;	// 0C0
	UInt32							time0C4;		// 0C4
	float							radioVolume;	// 0C8
	TESQuest						*selectedQuest;	// 0CC
	BGSQuestObjective				*currObjective;	// 0D0
	TileImage						*mapMarker;		// 0D4
	tList<TESObjectREFR>			mapMarkerList;	// 0D8
	tList<TESObjectREFR>			doorList;		// 0E0
	float							flt0E8;			// 0E8
	float							flt0EC;			// 0EC
	float							flt0F0;			// 0F0
	float							flt0F4;			// 0F4
	TESForm							*markerForm;	// 0F8
	NiVector3						markerPos;		// 0FC
	TESObjectCELL					*cell108;		// 108
	TESWorldSpace					*mapWorldSpace;	// 10C
	UInt32							unk110;			// 110
	TESObjectREFR					*lastExtDoor;	// 114
	TESObjectREFR					*selectedMarker;// 118
	TESObjectCELL					*cell11C;		// 11C
	float							clickStartPosX;	// 120
	float							clickStartPosY;	// 124
	UInt32							unk128;			// 128
	bool							fogOfWar;		// 12C
	UInt8							pad12D[3];		// 12D
	ListBox<TESQuest*>				questList;		// 130
	ListBox<BGSNote*>				noteList;		// 160
	ListBox<TESObjectREFR*>			radioRefList;	// 190
	ListBox<BGSQuestObjective*>		objectiveList;	// 1C0
	ListBox<TESChallenge*>			challengeList;	// 1F0
	BSSimpleArray<Tile*>			arr220;			// 220

	__forceinline static MapMenu *Get() {return *(MapMenu**)0x11DA368;}
};
static_assert(sizeof(MapMenu) == 0x230);

// CC (from Stewie)
class LevelUpMenu : public Menu		// 1027
{
public:
	UInt32				currentPage;			// 28	0 for skills, 1 for perks
	TileText			*tileTitle;				// 2C
	TileImage			*tileSkills;			// 30
	TileImage			*tilePerks;				// 34
	TileImage			*tileSelectionIcon;		// 38
	TileText			*tileSelectionText;		// 3C
	TileText			*tilePointCounter;		// 40
	TileImage			*tileBtnReset;			// 44
	TileImage			*tileBtnContinue;		// 48
	TileImage			*tileBtnBack;			// 4C
	TileImage			*tileStatsIconBadge;	// 50
	UInt32				numAssignedSkillPoints;	// 54
	UInt32				numAssignedPerks;		// 58
	UInt32				numSkillPointsToAssign;	// 5C
	UInt32				numPerksToAssign;		// 60
	ListBox<int>		skillListBox;			// 64
	ListBox<BGSPerk*>	perkListBox;			// 94
	tList<BGSPerk>		availablePerks;			// C4	Perks to show in the perk listBox

	__forceinline static LevelUpMenu *Get() {return *(LevelUpMenu**)0x11D9FDC;}
};
static_assert(sizeof(LevelUpMenu) == 0xCC);

// 8C
class RepairMenu : public Menu			// 1035
{
public:
	TileRect				*tile28;		// 28
	TileImage				*tile2C;		// 2C
	TileRect				*tile30;		// 30
	TileImage				*tile34;		// 34
	TileRect				*tile38;		// 38
	TileRect				*tile3C;		// 3C
	TileText				*tile40;		// 40
	TileText				*tile44;		// 44
	TileText				*tile48;		// 48
	TileRect				*tile4C;		// 4C
	TileRect				*tile50;		// 50
	TileImage				*tile54;		// 54
	TileImage				*tile58;		// 58
	MenuItemEntryList		repairItems;	// 5C

	__forceinline static RepairMenu *Get() {return *(RepairMenu**)0x11DA75C;}
	__forceinline static ContChangesEntry *Target() {return *(ContChangesEntry**)0x11DA760;}
};

// 1A4
class RaceSexMenu : public Menu			// 1036
{
public:
	UInt32				unk028[44];		// 028
	TESNPC				*npc;			// 0D8
	UInt32				unk0DC[50];		// 0DC

	__forceinline static RaceSexMenu *Get() {return *(RaceSexMenu**)0x11DA634;}
};
static_assert(sizeof(RaceSexMenu) == 0x1A4);

// 5C
class TextEditMenu : public Menu		// 1051
{
public:
	virtual void		Unk_12(void);

	TileText			*currTextTile;			// 28
	TileImage			*okButton;				// 2C
	TileText			*messageTitle;			// 30
	String				currentText;			// 34
	String				displayedText;			// 3C	Copy of currentText + cursor
	UInt32				cursorIndex;			// 44
	union
	{
		UInt32			maxPixelLength;			// 48
		struct
		{
			UInt16		minLength;				// 48
			UInt16		maxLength;				// 4A
		};
	};
	union
	{
		UInt32			fontID;					// 4C
		TileRect		*inputRect;				// 4C
	};
	UInt32				cursorBlink;			// 50	Value used for the cursor blink cycles (every 500ms)
	UInt8				cursorVisible;			// 54
	UInt8				isActive;				// 55
	UInt8				byte56;					// 56
	UInt8				miscFlags;				// 57
	union
	{
		bool			(*menuCallback)(char*);	// 58
		Script			*scriptCallback;		// 58
	};

	__forceinline static TextEditMenu *Get() {return *(TextEditMenu**)0x11DAEC4;}
};
static_assert(sizeof(TextEditMenu) == 0x5C);

typedef tList<ContChangesEntry> BarterItemList;

// 120
class BarterMenu : public Menu			// 1053
{
public:
	TileImage			*tile028;		// 028
	TileImage			*tile02C;		// 02C
	TileImage			*tile030;		// 030
	TileText			*tile034;		// 034
	TileImage			*tile038;		// 038
	TileText			*tile03C;		// 03C
	TileImage			*tile040;		// 040
	TileImage			*tile044;		// 044
	TileText			*tile048;		// 048
	TileImage			*tile04C;		// 04C
	TileText			*tile050;		// 050
	TileImage			*tile054;		// 054
	TileRect			*tile058;		// 058
	TileImage			*tile05C;		// 05C
	TileRect			*tile060;		// 060
	TileRect			*tile064;		// 064
	TileRect			*tile068;		// 068
	TileRect			*tile06C;		// 06C
	TileRect			*tile070;		// 070
	TileImage			*tile074;		// 074
	TileImage			*tile078;		// 078
	TileRect			*tile07C;		// 07C
	TESObjectREFR		*merchantRef;	// 080
	float				barterTotalSum;	// 084
	UInt32				unk088;			// 088
	UInt32				playerGold;		// 08C
	UInt32				merchantGold;	// 090
	float				buyValueMult;	// 094
	float				sellValueMult;	// 098
	UInt32				leftFilter;		// 09C
	UInt32				rightFilter;	// 0A0
	UInt32				unk0A4;			// 0A4
	MenuItemEntryList	leftItems;		// 0A8
	MenuItemEntryList	rightItems;		// 0D8
	MenuItemEntryList	*currentItems;	// 108
	BarterItemList		leftBarter;		// 10C
	BarterItemList		rightBarter;	// 114
	UInt32				unk11C;			// 11C

	__forceinline static BarterMenu *Get() {return *(BarterMenu**)0x11D8FA4;}
	__forceinline static ContChangesEntry *Selection() {return *(ContChangesEntry**)0x11D8FA8;}
};

// 1DC (from Stewie)
class HackingMenu : public Menu			// 1055
{
public:
	// 2C
	struct HackingText
	{
		String		textStr;				// 00
		Tile		*tileText;				// 08
		void		*ptr0C;					// 0C
		UInt32		displayRate;			// 10
		UInt32		flashOnDuration;		// 14
		UInt32		flashOffDuration;		// 18
		UInt32		nextDisplayTime;		// 1C
		UInt32		nextBlinkTime;			// 20
		UInt32		currentlyDisplayedText;	// 24
		UInt8		flags;					// 28
		UInt8		allOfLineIsDisplayed;	// 29
		UInt8		pad2A[2];				// 2A
	};

	HackingText			*lines[39];			// 028
	UInt32				unk0C4[7];			// 0C4
	HackingText			bottomLine;			// 0E0
	Tile				*tile10C;			// 10C
	Tile				*tile110;			// 110
	Tile				*tile114;			// 114
	Tile				*tile118;			// 118
	Tile				*tile11C;			// 11C
	Tile				*tile120;			// 120
	Tile				*tile124;			// 124
	Tile				*tile128;			// 128
	Sound				fanHum;				// 12C	UIHackingFanHumLP
	Sound				passGood;			// 138	UIHackingPassGood
	Sound				passBad;			// 144	UIHackingPassBad
	Sound				charScroll;			// 150	UIHackingCharScroll
	Sound				charEnter;			// 15C	UIHackingCharEnter
	DList<Tile>			childList168;		// 168
	DList<Tile>			childList174;		// 174
	tList<char>			wordList;			// 180
	DList<Tile>			childList188;		// 188
	TESObjectREFR		*targetBaseRef;		// 194
	TESObjectREFR		*targetRef;			// 198
	UInt32				correctAnswer;		// 19C
	UInt32				attemptsRemaining;	// 1A0
	UInt32				maxAttemptsRemaining;	// 1A4
	UInt32				wordsRemaining;		// 1A8
	UInt32				correctAnswerLength;	// 1AC
	void				*ptr1B0;			// 1B0
	UInt32				topTextHeight;		// 1B4
	UInt32				topTextLineHeight;	// 1B8
	UInt32				highlightedCharacterXIndex;	// 1BC
	UInt32				activeTile;			// 1C0
	UInt32				stage;				// 1C4
	TileImage			*tile1C8;			// 1C8
	UInt32				unk1CC;				// 1CC
	UInt32				transitionToComputersMenuTime;	// 1D0
	void				*ptr1D4;			// 1D4
	UInt8				noHighlightOnHover;	// 1D8
	UInt8				hasAllowanceReplenished;	// 1D9
	UInt8				byte1DA;			// 1DA
	UInt8				byte1DB;			// 1DB

	__forceinline static HackingMenu *Get() {return *(HackingMenu**)0x11D95B8;}
};
static_assert(sizeof(HackingMenu) == 0x1DC);

struct VATSTargetInfo
{
	enum ActionTypes
	{
		kAPCost_AttackUnarmed,
		kAPCost_AttackOneHandMelee,
		kAPCost_AttackTwoHandMelee,
		kAPCost_AttackPistol,
		kAPCost_AttackRifle,
		kAPCost_AttackHandle,
		kAPCost_AttackLauncher,
		kAPCost_AttackGrenade,
		kAPCost_AttackMine,
		kAPCost_Reload,
		kAPCost_Crouch,
		kAPCost_Stand,
		kAPCost_SwitchWeapon,
		kAPCost_ToggleWeaponDrawn,
		kAPCost_Heal,
		kAPCost_OneHandThrown = 0x13,
		kAPCost_AttackThrown,
		kAPCost_UnarmedAttackGround,
		kAPCost_MAX = 0x16
	};

	int				actionType;			// 00
	UInt8			isSuccess;			// 04
	UInt8			byte05;				// 05
	UInt8			mysteriousStrangerVisit;// 06
	UInt8			byte07;				// 07
	UInt8			remainingShotsToFire;	// 08
	UInt8			count09;			// 09
	UInt8			pad0A[2];			// 0A
	TESObjectREFR	*refr;				// 0C
	UInt32			avCode;				// 10
	ActorHitData	*hitData;			// 14
	float			flt18;				// 18
	float			flt1C;				// 1C
	float			apCost;				// 20
	UInt8			missFortuneVisit;	// 24
	UInt8			pad25[3];			// 25

	static VATSTargetInfo *Create()
	{
		return ThisCall<VATSTargetInfo*>(0x9CA500, Game_HeapAlloc<VATSTargetInfo>());
	}
};
static_assert(sizeof(VATSTargetInfo) == 0x28);

struct VATSTargetBodyPartData
{
	NiPoint2	screenPos;			// 00
	NiVector3	relativePos;		// 08
	NiVector3	pos;				// 14
	UInt32		bodyPartID;			// 20
	float		percentVisible;		// 24
	float		hitChance;			// 28
	UInt8		isOnScreen;			// 2C
	UInt8		byte2D;				// 2D
	UInt8		shouldCalculateBodyPartVisibilityPercents;	// 2E
	UInt8		byte2F;				// 2F
	Tile		*bodyPartPercent;	// 30
	float		flt34;				// 34
	UInt8		byte38;				// 38
	UInt8		pad39[3];			// 39
};
static_assert(sizeof(VATSTargetBodyPartData) == 0x3C);

struct VATSTarget
{
	TESObjectREFR					*targetRef;		// 00
	UInt32							unk04;			// 04
	tList<VATSTargetBodyPartData>	bodyPartlist;	// 08
	UInt8							byte10;			// 10
	UInt8							pad11[3];		// 11
	UInt32							unk14[3];		// 14
	UInt8							byte20;			// 20
	UInt8							pad21[3];		// 21
};
static_assert(sizeof(VATSTarget) == 0x24);

// 144
class VATSMenu : public Menu			// 1056
{
public:
	virtual void		Unk_12(void);

	UInt32					unk028;			// 028
	TileImage				*tile02C;		// 02C
	TileImage				*tile030;		// 030
	TileImage				*tile034;		// 034
	TileImage				*tile038;		// 038
	TileImage				*tile03C;		// 03C
	TileImage				*tile040;		// 040
	TileImage				*tile044;		// 044
	TileImage				*tile048;		// 048
	TileImage				*tile04C;		// 04C
	TileImage				*tile050;		// 050
	TileText				*tile054;		// 054
	TileText				*tile058;		// 058
	TileText				*tile05C;		// 05C
	TileText				*tile060;		// 060
	TileImage				*tile064;		// 064
	TileImage				*tile068;		// 068
	TileImage				*tile06C;		// 06C
	TileImage				*tile070;		// 070
	TileText				*tile074;		// 074
	TileRect				*tile078;		// 078
	TileRect				*tile07C;		// 07C
	TileRect				*tile080;		// 080
	TileImage				*tile084;		// 084
	TileRect				*tile088;		// 088
	TileImage				*tile08C;		// 08C
	TileImage				*tile090;		// 090
	TileImage				*tile094;		// 094
	TileImage				*tile098;		// 098
	TileText				*tile09C;		// 09C
	TileImage				*tile0A0;		// 0A0
	TileImage				*tile0A4;		// 0A4
	UInt32					unk0A8[2];		// 0A8
	ListBox<UInt32>			queuedActions;	// 0B0
	float					actionPoints;	// 0E0
	float					maxActionPoints;// 0E4
	float					flt0E8;			// 0E8
	float					flt0EC;			// 0EC
	float					clipAmmo;		// 0F0
	float					reserveAmmo;	// 0F4
	UInt8					fullyZoomedIn;	// 0F8
	UInt8					byte0F9;		// 0F9
	UInt8					byte0FA;		// 0FA
	UInt8					byte0FB;		// 0FB
	float					flt0FC;			// 0FC
	VATSTargetBodyPartData	*currBodyPartData;	// 100
	UInt8					hasConcentratedFire;// 104
	UInt8					pad105[3];		// 105
	UInt32					unk100[5];		// 108
	VATSTargetInfo			targetInfo;		// 11C

	__forceinline static VATSMenu *Get() {return *(VATSMenu**)0x11DB0D4;}
	__forceinline static TESObjectREFR *Target() {return *(TESObjectREFR**)0x11F21CC;}
};
static_assert(sizeof(VATSMenu) == 0x144);

class ImageSpaceModifierInstanceRB;

// 48
struct VATSCameraData
{
	tList<VATSTargetInfo>			targetsList;	// 00
	UInt32							mode;			// 08
	UInt32							unk0C;			// 0C
	BGSCameraShot					*camShot;		// 10
	float							flt14;			// 14
	float							flt18;			// 18
	Projectile						*projectile1C;	// 1C
	Projectile						*projectile20;	// 20
	TESIdleForm						*attackAnim;	// 24
	ImageSpaceModifierInstanceForm	*isModInstForm;	// 28
	ImageSpaceModifierInstanceRB	*isModInstRB;	// 2C
	UInt32							unk30;			// 30
	NiPointLight					*ptLight;		// 34
	UInt8							byte38;			// 38
	UInt8							pad39[3];		// 39
	UInt32							numKills;		// 3C
	UInt32							unk40;			// 40
	UInt32							unk44;			// 44

	__forceinline static VATSCameraData *Get() {return (VATSCameraData*)0x11F2250;}
};
static_assert(sizeof(VATSCameraData) == 0x48);

// FC
class ComputersMenu : public Menu		// 1057
{
public:
	UInt32				unk28[32];		// 28
	BGSTerminal			*baseTerminal;	// A8
	TESObjectREFR		*targetRef;		// AC
	UInt32				unkB0[19];		// B0

	__forceinline static ComputersMenu *Get() {return *(ComputersMenu**)0x11D9334;}
};
static_assert(sizeof(ComputersMenu) == 0xFC);

// A0
class RepairServicesMenu : public Menu	// 1058
{
public:
	TileText			*tile28;		// 28
	TileText			*tile2C;		// 2C
	TileImage			*tile30;		// 30
	TileImage			*tile34;		// 34
	TileRect			*tile38;		// 38
	TileRect			*tile3C;		// 3C
	TileRect			*tile40;		// 40
	TileText			*tile44;		// 44
	TileText			*tile48;		// 48
	TileText			*tile4C;		// 4C
	TileRect			*tile50;		// 50
	TileRect			*tile54;		// 54
	TileText			*tile58;		// 58
	TileImage			*tile5C;		// 5C
	TileImage			*tile60;		// 60
	TileImage			*tile64;		// 64
	MenuItemEntryList	itemList;		// 68
	UInt32				repairAllCost;	// 98
	UInt8				skill;			// 9C
	UInt8				pad9D[3];		// 9D

	__forceinline static RepairServicesMenu *Get() {return *(RepairServicesMenu**)0x11DA7F0;}
	__forceinline static Actor *Target() {return *(Actor**)0x11DA7F4;}
};
static_assert(sizeof(RepairServicesMenu) == 0xA0);

// 90
class ItemModMenu : public Menu			// 1061
{
public:
	TileRect			*tile28;		// 28
	TileImage			*tile2C;		// 2C
	TileRect			*tile30;		// 30
	TileImage			*tile34;		// 34
	TileRect			*tile38;		// 38
	TileRect			*tile3C;		// 3C
	TileText			*tile40;		// 40
	TileText			*tile44;		// 44
	TileText			*tile48;		// 48
	TileRect			*tile4C;		// 4C
	TileRect			*tile50;		// 50
	TileImage			*tile54;		// 54
	TileImage			*tile58;		// 58
	TileText			*tile5C;		// 5C
	MenuItemEntryList	itemModList;	// 60

	__forceinline static ItemModMenu *Get() {return *(ItemModMenu**)0x11D9F54;}
	__forceinline static ContChangesEntry *Target() {return *(ContChangesEntry**)0x11D9F58;}
};
static_assert(sizeof(ItemModMenu) == 0x90);

// 88
class CompanionWheelMenu : public Menu	// 1075
{
public:
	virtual void		Unk_12(void);

	Tile				*tiles[16];					// 28
	Actor				*companionRef;				// 68
	UInt8				isIsFollowingLong;			// 6C
	UInt8				isFollowerSwitchAggressive;	// 6D
	UInt8				isCombatStyleRanged;		// 6E
	UInt8				isWaiting;					// 6F
	UInt32				lastSelectedTileID;			// 70
	UInt8				talkToActorWhenClosingMenu;	// 74
	UInt8				pad75[3];					// 75
	UInt32				time78;						// 78
	Sound				sound7C;					// 7C

	__forceinline static CompanionWheelMenu *Get() {return *(CompanionWheelMenu**)0x11D92B8;}
};
static_assert(sizeof(CompanionWheelMenu) == 0x88);

// 88
class TraitSelectMenu : public Menu		// 1076
{
public:
	UInt32				unk28[24];		// 28

	__forceinline static TraitSelectMenu *Get() {return *(TraitSelectMenu**)0x11DB00C;}
};

// 104
class RecipeMenu : public Menu			// 1077
{
public:
	TileImage					*tile028;		// 028	RM_Items_LeftFilterArrow
	TileText					*tile02C;		// 02C	RM_ItemsTitle
	TileImage					*tile030;		// 030	RM_Items_RightFilterArrow
	TileImage					*tile034;		// 034	RM_Items_InventoryList
	TileText					*tile038;		// 038	RM_MadeAtVariable
	TileText					*tile03C;		// 03C	RM_SkillRequirement
	TileImage					*tile040;		// 040	RM_Items_IngredientList
	TileImage					*tile044;		// 044	RM_ButtonX
	TileImage					*tile048;		// 048	RM_ButtonB
	TileImage					*tile04C;		// 04C	RM_ItemIcon
	TileRect					*tile050;		// 050	RM_ItemData
	TileText					*tile054;		// 054	RM_Items_IngredientList
	TileText					*tile058;		// 058	RM_ContainerTitle
	TileText					*tile05C;		// 05C	RM_SkillRequirementHeader
	TESObjectREFR				*sourceRef;		// 060
	TESRecipeCategory			*category;		// 064
	UInt32						menuSoundID;	// 068
	ListBox<TESRecipe*>			recipeList;		// 06C
	ListBox<TESRecipe*>			*currentItems;	// 09C
	ListBox<RecipeComponent*>	componentList;	// 0A0
	ListBox<Condition*>			conditionList;	// 0D0
	UInt32						unk100;			// 100

	__forceinline static RecipeMenu *Get() {return *(RecipeMenu**)0x11D8E90;}
	__forceinline static TESRecipe *Selection() {return *(TESRecipe**)0x11D8E94;}
};

// E88
class CaravanMenu : public Menu			// 1083
{
public:
	UInt32					unk028[54];		// 028
	TESObjectREFR			*opponentRef;	// 100
	UInt32					unk104[865];	// 104

	__forceinline static CaravanMenu *Get() {return *(CaravanMenu**)0x11D917C;}
};

// 8C
class TraitMenu : public Menu			// 1084
{
public:
	virtual void		Unk_12(void);

	TileText			*tile28;		// 28	LUM_Headline_Title
	TileImage			*tile2C;		// 2C	LUM_PerkList
	TileImage			*tile30;		// 30	LUM_SelectionIcon
	TileText			*tile34;		// 34	TM_DescriptionText
	TileText			*tile38;		// 38	LUM_PointCounter
	TileImage			*tile3C;		// 3C	LUM_ResetButton
	TileImage			*tile40;		// 40	LUM_ContinueButton
	TileImage			*tile44;		// 44	stats_icon_badge
	TileImage			*tile48;		// 48	TM_DescriptionScrollbar
	UInt32				numSelected;	// 4C
	UInt32				maxSelect;		// 50
	ListBox<BGSPerk*>	perkListBox;	// 54
	tList<BGSPerk>		perkList;		// 84

	__forceinline static TraitMenu *Get() {return *(TraitMenu**)0x11DAF74;}
};

extern HUDMainMenu *g_HUDMainMenu;

// D8
class FORenderedMenu
{
public:
	/*00*/virtual void		Destructor(bool doFree);
	/*04*/virtual void		Unk_01(void);
	/*08*/virtual void		Unk_02(void);
	/*0C*/virtual void		Render(void);
	/*10*/virtual void		Unk_04(void);
	/*14*/virtual void		Update(void);
	/*18*/virtual void		Unk_06(void);
	/*1C*/virtual void		Unk_07(void);
	/*20*/virtual void		Unk_08(void);
	/*24*/virtual void		Init(void);
	/*28*/virtual void		ResetNifs(void);
	/*2C*/virtual void		Unk_0B(void);
	/*30*/virtual void		OnMenuOpen(void);
	/*34*/virtual void		OnMenuClose(void);
	/*38*/virtual void		HandleStaticEffect(float msPassed);
	/*3C*/virtual void		HandleVerticalHoldEffect(float msPassed);
	/*40*/virtual void		HandleShudderEffect(float msPassed);
	/*44*/virtual void		HandlePulseEffect(float msPassed);
	/*48*/virtual void		HandleScreenLightEffect(float msPassed);
	/*4C*/virtual void		HandleScanlines(float msPassed);
	/*50*/virtual void		Unk_14(void);

	NiTriStrips			*screenTriShape;		// 04
	NiNode				*parentNode;			// 08
	NiScreenElements	*screenElements;		// 0C
	NiSourceTexture		*srcTexture;			// 10
	SceneGraph			*sceneGraph;			// 14
	TileMenu			*tileMenu;				// 18
	NiPointLight		*light1C;				// 1C
	ShadowSceneNode		*shadowScene0;			// 20
	NiCamera			*camera;				// 24
	UInt8				isInitialised;			// 28
	UInt8				pad29[3];				// 29
	NiVector3			point2C;				// 2C
	float				flt38;					// 38
	float				flt3C;					// 3C
	float				blurRadius;				// 40
	float				blurIntensity;			// 44
	float				scanlineFrequency;		// 48
	UInt8				byte4C;					// 4C
	UInt8				showStaticEffect;		// 4D
	UInt8				pad4E[2];				// 4E
	float				staticEffectStartTime;	// 50
	float				burstDuration;			// 54
	float				burstIntensity2;		// 58
	float				burstIntensity;			// 5C
	UInt8				showVerticalHoldEffect;	// 60
	UInt8				pad61[3];				// 61
	float				verticalHoldStartTime;	// 64
	float				verticalHoldDuration;	// 68
	float				verticalHoldSpeed;		// 6C
	float				flt70;					// 70
	UInt8				showShudderHoldEffect;	// 74
	UInt8				pad75[3];				// 75
	float				shudderHoldStartTime;	// 78
	float				shudderHoldDuration;	// 7C
	float				shudderHoldIntensity;	// 80
	float				shudderHoldFrequency;	// 84
	float				flt88;					// 88
	float				pulseBrightenIntensity;	// 8C
	float				pulseRadiusIntensity;	// 90
	UInt8				byte94;					// 94
	UInt8				scanlineEffect;			// 95
	UInt8				pad96[2];				// 96
	float				scanLineStartTime;		// 98
	float				interval9C;				// 9C
	UInt32				rateA0;					// A0
	float				nextScanlineTime;		// A4
	NiColorAlpha		systemColor1;			// A8
	UInt8				renderedMenuOrPipboyOpen;// B8
	UInt8				byteB9;					// B9
	UInt8				padBA[2];				// BA
	NiSourceTexture		*scanlineTexture;		// BC
	NiColorAlpha		systemColor2;			// C0
	UInt32				unkD0;					// D0
	BSShaderAccumulator	*shaderAccum;			// D4

	__forceinline void Close() {ThisCall(0x7FFD50, this);}
};
static_assert(sizeof(FORenderedMenu) == 0xD8);

// 170 (from Stewie)
class FOPipboyManager : public FORenderedMenu
{
public:
	NiNode			*pipBoyScreenNode;		// 0D8
	NiNode			*node0DC;				// 0DC
	NiTriStrips		*triStrips0E0;			// 0E0
	NiObject		*object0E4;				// 0E4
	NiTriStrips		*pipboyLightButton[3];	// 0E8
	NiNode			*pipboyLightGlow[3];	// 0F4	Stats, Items, Data
	NiTriStrips		*scrollKnobs[3];		// 100
	float			flt10C[3];				// 10C
	float			knobScrollPos[3];		// 118
	float			knobScrollRates[3];		// 124
	float			tabKnobMinPosition;		// 130
	UInt32			unk134;					// 134
	UInt32			unk138;					// 138
	UInt32			unk13C;					// 13C
	float			tabKnobMaxPosition;		// 140
	UInt8			byte144;				// 144
	UInt8			pad145[3];				// 145
	UInt32			unk148;					// 148
	UInt32			unk14C;					// 14C
	UInt8			byte150;				// 150
	UInt8			pad151[3];				// 151
	Sound			sound154;				// 154
	UInt8			byte160;				// 160
	UInt8			byte161;				// 161
	UInt8			byte162;				// 162
	UInt8			byte163;				// 163
	float			lightFadeStartTime;		// 164
	float			lightEffectFadeDuration;// 168
	UInt8			resetPipboyManager;		// 16C
	UInt8			pad16D[3];				// 16D
};
static_assert(sizeof(FOPipboyManager) == 0x170);

// F0
class FORenderedTerminal : public FORenderedMenu
{
public:
	NiVector3		localTranslate;	// D8
	NiTriStrips		*powerButton;	// E4
	float			fltE8;			// E8
	UInt8			bytEC;			// EC
	UInt8			padED[3];		// ED
};
static_assert(sizeof(FORenderedTerminal) == 0xF0);

struct FontHeightData
{
	float		heightBase;
	float		heightwGap;
};
extern FontHeightData s_fontHeightDatas[90];

// 54
struct FontInfo
{
	// 38
	struct CharDimensions
	{
		float			flt00[9];
		float			width;		// 24
		float			height;		// 28
		float			flt2C;		// 2C
		float			widthMod;	// 30
		float			flt34;		// 34
		//	totalWidth = width + widthMod
	};

	// 24
	struct TexFileName
	{
		UInt32			unk00;
		char			fileName[0x20];
	};

	// 3928
	struct BufferData
	{
		float			lineHeight;				// 0000
		UInt32			numTextures;			// 0004
		TexFileName		textures[8];			// 0008
		CharDimensions	charDimensions[256];	// 0128
	};

	struct ButtonIcon;

	UInt8						isLoaded;	// 00
	UInt8						pad01[3];	// 01
	char						*filePath;	// 04
	UInt32						fontID;		// 08
	NiTexturingProperty			*texProp;	// 0C
	UInt32						unk10[7];	// 10
	float						flt2C;		// 2C
	float						flt30;		// 30
	UInt32						unk34;		// 34
	BufferData					*bufferData;// 38
	UInt32						unk3C[2];	// 3C
	BSSimpleArray<ButtonIcon>	arr44;		// 44

	__forceinline FontInfo *Init(UInt32 fontID, const char *filePath, bool arg3)
	{
		return ThisCall<FontInfo*>(0xA12020, this, fontID, filePath, arg3);
	}
};
static_assert(sizeof(FontInfo) == 0x54);

// 164 (24)
class FontManager
{
public:
	FontInfo		*fontInfos[8];		// 00
	UInt8			byte20;				// 20
	UInt8			pad21[3];			// 21
	FontInfo		*extraFonts[80];	// 24

										//	outDims.x := width (pxl); outDims.y := height (pxl); outDims.z := numLines
	__forceinline NiVector3 *GetStringDimensions(NiVector3 *outDims, const char *srcString, UInt32 fontID, float wrapWidth = FLT_MAX, UInt32 startIdx = 0)
	{
		return ThisCall<NiVector3*>(0xA1B020, this, outDims, srcString, fontID, wrapWidth, startIdx);
	}

	__forceinline static FontManager *GetSingleton() {return *(FontManager**)0x11F33F8;}
};

extern FontManager *g_fontManager;

// 10
struct SystemColorManager
{
	// 0C
	class SystemColor
	{
	public:
		virtual void	Destructor(bool doFree);
		virtual UInt32	GetColor();
		virtual void	SetColor(UInt32 newColor);
		virtual bool	IsHard();
		virtual bool	IsSoft();

		String		traitName;

		void SetColorRGB(UInt32 r, UInt32 g, UInt32 b);
	};

	// 10
	class HardSystemColor : public SystemColor
	{
	public:
		UInt32		color;		// 0C
	};

	// 10
	class SoftSystemColor : public SystemColor
	{
	public:
		Setting		*setting;	// 0C
	};

	DList<SystemColor>	sysColors;
	UInt32				unk0C;

	__forceinline static SystemColorManager *GetSingleton() {return *(SystemColorManager**)0x11D8A88;}
};

// 229C
class DebugText
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(UInt32 arg1, UInt32 arg2);
	virtual UInt32	Unk_02(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual UInt32	Unk_03(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	virtual void	Unk_04(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual UInt32	Unk_05(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);
	virtual void	Unk_06(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);
	virtual UInt32	Unk_07(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6, UInt32 arg7);
	virtual UInt32	Unk_08(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);
	virtual UInt32	Unk_09(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual UInt32	Unk_0A(UInt32 arg1);
	virtual void	Unk_0B(UInt32 arg1, UInt32 arg2);

	struct DebugLine
	{
		float			offsetX;	// 00
		float			offsetY;	// 04
		UInt32			alignment;	// 08
		NiNode			*node;		// 0C
		String			text;		// 10
		float			flt18;		// 18	Always -1.0
		NiColorAlpha	color;		// 1C
	};

	struct PrintData
	{
		DebugLine	*debugLine;
		UInt32		unk04;
		UInt32		unk08;
		UInt32		unk0C;
		float		duration;
		UInt32		fontNumber;
	};

	enum TextAlign
	{
		kAlign_Left =	1,
		kAlign_Center =	2,
		kAlign_Right =	4
	};

	DebugLine					lines[200];		// 0004
	DList<String>				textList;		// 2264
	BSSimpleArray<PrintData*>	lines2;			// 2270
	SInt32						unk2280;		// 2280
	SInt32						yMods[3];		// 2284
	SInt32						xMods[3];		// 2290

	static DebugText *GetSingleton();
	DebugLine *GetDebugInput();
	void CreateLine(const char *text, float xPos, float yPos, TextAlign alignment, int arg5 = -1, float duration = -1.0F, int fontNumber = 0, NiColorAlpha *color = nullptr)
	{
		ThisCall(0xA0F8B0, this, text, xPos, yPos, alignment, arg5, duration, fontNumber, color);
	}
};
static_assert(sizeof(DebugText) == 0x229C);

TileMenu* __fastcall GetMenuTile(const char *componentPath);

Menu* __fastcall GetMenuByType(UInt32 menuID);

bool __fastcall IsMenuMode(UInt32 menuID);