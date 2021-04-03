#pragma once

NiRefObject** __stdcall NiReleaseAddRef(NiRefObject **toRelease, NiRefObject *toAdd);

// 584
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
			NiReleaseAddRef((NiRefObject**)&node, _node);
		}

		void Set(HighlightedRef *hRef)
		{
			refr = hRef->refr;
			NiReleaseAddRef((NiRefObject**)&node, hRef->node);
		}

		void Clear()
		{
			refr = NULL;
			if (node) NiReleaseAddRef((NiRefObject**)&node, nullptr);
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
	NiNode					*niNode080;			// 080
	NiNode					*niNode084;			// 084
	UInt32					unk088;				// 088
	BSShaderAccumulator		*shaderAccum08C;	// 08C
	BSShaderAccumulator		*shaderAccum090;	// 090
	ShadowSceneNode			*shadowScene094;	// 094
	ShadowSceneNode			*shadowScene098;	// 098
	Tile					*menuRoot;			// 09C
	Tile					*globalsTile;		// 0A0	globals.xml
	NiNode					*niNode0A4;			// 0A4
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
	UInt32					unk140[5];			// 140
	UInt32					unk154;				// 154
	UInt32					unk158;				// 158
	UInt32					unk15C;				// 15C
	UInt32					unk160;				// 160
	Unk164					*ptr164;			// 164
	UInt8					byte168;			// 168
	UInt8					pad169[3];			// 169
	void					*ptr16C;			// 16C
	UInt8					byte170;			// 170
	UInt8					byte171;			// 171
	UInt8					byte172;			// 172
	UInt8					byte173;			// 173
	FOPipboyManager			*pipboyManager;		// 174
	UInt32					unk178;				// 178
	NiTArray<UInt32>		array17C;			// 17C NiTPrimitiveArray@PackedMenu@BGSMenuPacker
	UInt32					unk18C[20];			// 18C
	UInt32					highlightState;		// 1DC
	HighlightedRef			highlightMain;		// 1E0
	UInt32					numHighlighted;		// 1E8
	SInt32					flashingRefIndex;	// 1EC
	HighlightedRef			highlightedRefs[32];	// 1F0
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

	UInt32 GetTopVisibleMenuID();
	TESObjectREFR *GetCursorPick();
	Tile *GetActiveTile();
	void ClearHighlights();
	void AddHighlightedRef(TESObjectREFR *refr);
	void RemoveHighlightedRef(TESObjectREFR *refr);
	bool IsRefHighlighted(TESObjectREFR *refr);
};
STATIC_ASSERT(sizeof(InterfaceManager) == 0x580);

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
	virtual void	Destructor(bool doFree);
	virtual void	SetTile(UInt32 idx, Tile *value);
	virtual void	HandleLeftClickPress(UInt32 tileID, Tile *activeTile);	// called when the mouse has moved and left click is pressed
	virtual void	HandleClick(SInt32 tileID, Tile *clickedTile);
	virtual void	HandleMouseover(UInt32 arg0, Tile *activeTile);	//    Called on mouseover, activeTile is moused-over Tile
	virtual void	HandleMouseoverTileAlt(UInt32 arg0, Tile *tile);
	virtual void	Unk_06(UInt32 arg0, UInt32 arg1, UInt32 arg2);	// unused?
	virtual void	Unk_07(UInt32 arg0, UInt32 arg1, UInt32 arg2);	// unused?
	virtual void	HandleActiveMenuClick(UInt32 tileID, Tile *activeTile); // StartMenu, RaceSexMenu, VATSMenu, BookMenu
	virtual void	Unk_09(UInt32 arg0, UInt32 arg1); // unused?
	virtual void	HandleMousewheel(UInt32 tileID, Tile *tile);
	virtual void	Update();	// Called every frame while the menu is active
	virtual bool	HandleKeyboardInput(UInt32 inputChar);	// Return false for handling keyboard shortcuts
	virtual UInt32	GetID();
	virtual bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, UInt32 arg1);
	virtual bool	HandleControllerInput(int arg0, Tile *tile);
	virtual void	Unk_10();	// unused?
	virtual void	HandleControllerConnectOrDisconnect(bool isControllerConnected);

	// 14
	struct TemplateInstance
	{
		UInt32		unk00;		// 00
		float		flt04;		// 04
		String		tileName;	// 08
		Tile		*tile;		// 10
	};

	// 14
	struct TemplateData
	{
		const char				*templateName;	// 00
		TileExtra				*tileExtra;		// 04
		DList<TemplateInstance>	instances;		// 08
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