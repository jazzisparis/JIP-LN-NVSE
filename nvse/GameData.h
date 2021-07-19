#pragma once

class TESRegionManager;

struct ChunkAndFormType {
	UInt32		chunkType;	// ie 
	UInt32		formType;	// ie 0x2A
	const char	* formName;	// ie 'NPC_'
};

// 10
class BoundObjectListHead
{
public:
	UInt32			boundObjectCount;	// 0
	TESBoundObject	* first;			// 4
	TESBoundObject	* last;				// 8
	UInt32			unkC;				// C
};

struct FormRecordData
{
	UInt8		typeID;		// corresponds to kFormType_XXX
	UInt32		typeCode;	// i.e. 'GMST', 'FACT'
	UInt32		unk08;		// only seen zero
};

struct ChunkHeader
{
	UInt32	type : 4;	// i.e. 'XGRD', 'DATA'
	UInt16	size : 2;
};

struct ModInfo		// referred to by game as TESFile
{
	// 18 info about currently loading form
	struct FormInfo  // Record Header in FNVEdit
	{
		UInt32		recordType;			// 00 i.e. 'FACT', 'GMST'						Signature
		UInt32		dataSize;			// 04 looks like size of entire record			Data Size
		UInt32		formFlags;			// 08 copied to TESForm->flags					Record Flags
		UInt32		formID;				// 0C											FormID
		UInt32		unk10;				// 10											Version Control Info 1
		UInt16		formVersion;		// 14 always initialized to 0F on SaveForm.		Form Version
		UInt16		unk16;				// 16                                           Version Control Info 2
	};

	// 18 info about current group of form
	struct GroupInfo  // Record Header in FNVEdit
	{
		UInt32		recordType;			// 00 'GRUP'									Signature
		UInt32		groupSize;			// 04 Size of entire record						Size
		UInt32		groupLabel;			// 08 copied to TESForm->flags					Label
		UInt32		groupType;			// 0C forms, dialog, cell...					Type
		UInt32		unk10;				// 10											Stamp
		UInt16		unk14;				// 14											Part of Unknown
		UInt16		unk16;				// 16                                           Part of Unknown
	};

	struct FileHeader	// File header in FNVEdit Signature 'HEDR'
	{
		float version;			//	00
		UInt32	recordCount;	//	04
		UInt32	nextObectID;	//	08
	};

	struct	MasterSize	// Data member of the master list in WIN32_FIND_DATA format
	{
		UInt32	low;
		UInt32	high;
	};

	tList<UInt32>						unkList;			// 000 treated as ModInfo during InitializeForm, looks to be a linked list of modInfo
	UInt32 /*NiTPointerMap<TESFile*>*/	* pointerMap;		// 008
	UInt32								unk00C;				// 00C
	BSFile*								unkFile;			// 010
	UInt32								unk014;				// 014 
	void								* unk018;			// 018 seen all zeroes. size unknown, seen not valid pointer in FalloutNV.esm
	void								* unk01C;			// 01C as above
	char								name[0x104];		// 020
	char								filepath[0x104];	// 124
	UInt32								unk228;				// 228
	UInt32								unk22C;				// Masters are init'd to dword_1186740 (0x2800) same val as BSFile+10? Buffer size ?
	UInt32								unk230;				// 230
	UInt32								unk234;				// 234
	UInt32								unk238;				// 238
	UInt32								unk23C;				// 23C
	FormInfo							formInfo;			// 240
	ChunkHeader							subRecordHeader;	// 258
	UInt32								unk260;				// 260 could be file size, has it is compared with fileOffset during load module. But filesize would be an Int64 !
	UInt32								fileOffset;			// 264
	UInt32								dataOffset;			// 268 index into dataBuf
	UInt32								subrecordBytesRead;	// 26C generates error on Read if != expected length
	FormInfo							writeInfo;			// 270 "used" to add record to the plugin.
	UInt32								writeOffset;		// 288
	UInt32								subrecordBytesToWrite;	// 28C
	tList<UInt32>						tList290;			// 290 looks to be a list of form or a list of formInfo. referenced from TESForm::WriteForm
	UInt8								unk298;				// 298
	UInt8								bIsBigEndian;		// 299
	UInt8								unk29A;				// 29A
	UInt8								pad29B;
	WIN32_FIND_DATA						fileData;			// 29C
	FileHeader							header;				// 3DC
	UInt8								flags;				// 3E8	Bit 0 is ESM . Runtime: Bit 2 is Valid, Bit 3 is Unselected Editor: 2 is selected, 3 is active, 4 may be invalid, 6 is endian, 14 controls VCI.
	UInt8								pad3E9[3];
	tList<char*>						* refModNames;		// 3EC	paired with 3F0
	UInt32								unk3F0;				// 3F0
	tList<MasterSize*>					* refModData;		// 3F4 most likely full of 0
	UInt32								unk3F8;				// 3F8
	UInt32								numRefMods;			// 3FC related to modindex; see 4472D0
																// formIDs in mod are as saved in GECK, must fix up at runtime
	ModInfo								** refModInfo;		// 400 used to look up modInfo based on fixed mod index, double-check
	UInt32								unk404;				// 404
	UInt32								unk408;				// 408
	UInt8								modIndex;			// 40C init to 0xFF
	UInt8								pad40D[3];
	String								author;				// 410
	String								description;		// 418
	void								* dataBuf;			// 420 
	UInt32								dataBufSize;		// 424 looks like size of entire record
	UInt8								unk428;				// 428 decide if forms needs to be reloaded on LoadFiles
	UInt8								pad429[3];

	// In Editor: 430 = ONAM array and 434 ONAM array count. Allocated at 0438
	
	/*** used by TESForm::LoadForm() among others ***/
	MEMBER_FN_PREFIX(ModInfo);
	DEFINE_MEMBER_FN(GetNextChunk, UInt32, 0x004726B0);	// returns chunk type
	DEFINE_MEMBER_FN(GetChunkData, bool, 0x00472890, UInt8* buf, UInt32 bufSize); // max size, not num to read
	DEFINE_MEMBER_FN(Read32, void, 0x004727F0, void* out);
	DEFINE_MEMBER_FN(HasMoreSubrecords, bool, 0x004726F0);
};
STATIC_ASSERT(sizeof(WIN32_FIND_DATA) == 0x140);
STATIC_ASSERT(sizeof(ModInfo) == 0x42C);

struct ModList
{
	tList<ModInfo>		modInfoList;		// 00
	UInt32				loadedModCount;		// 08
	ModInfo*			loadedMods[0xFF];	// 0C
};
STATIC_ASSERT(sizeof(ModList) == 0x408);

// 5B8
class DataHandler
{
public:
	UInt32							unk00;					// 000
	BoundObjectListHead				*boundObjectList;		// 004
	tList<TESPackage>				packageList;			// 008
	tList<TESWorldSpace>			worldSpaceList;			// 010
	tList<TESClimate>				climateList;			// 018
	tList<TESImageSpace>			imageSpaceList;			// 020
	tList<TESImageSpaceModifier>	imageSpaceModList;		// 028
	tList<TESWeather>				weatherList;			// 030
	tList<EnchantmentItem>			enchantmentItemList;	// 038
	tList<SpellItem>				spellItemList;			// 040
	tList<BGSHeadPart>				headPartList;			// 048
	tList<TESHair>					hairList;				// 050
	tList<TESEyes>					eyeList;				// 058
	tList<TESRace>					raceList;				// 060
	tList<BGSEncounterZone>			encounterZoneList;		// 068
	tList<TESLandTexture>			landTextureList;		// 070
	tList<BGSCameraShot>			cameraShotList;			// 078
	tList<TESClass>					classList;				// 080
	tList<TESFaction>				factionList;			// 088
	tList<TESReputation>			reputationList;			// 090
	tList<TESChallenge>				challengeList;			// 098
	tList<TESRecipe>				recipeList;				// 0A0
	tList<TESRecipeCategory>		recipeCategoryList;		// 0A8
	tList<TESAmmoEffect>			ammoEffectList;			// 0B0
	tList<TESCasino>				casinoList;				// 0B8
	tList<TESCaravanDeck>			caravanDeckList;		// 0C0
	tList<Script>					scriptList;				// 0C8
	tList<TESSound>					soundList;				// 0D0
	tList<BGSAcousticSpace>			acousticSpaceList;		// 0D8
	tList<BGSRagdoll>				ragdollList;			// 0E0
	tList<TESGlobal>				globalList;				// 0E8
	tList<BGSVoiceType>				voiceTypeList;			// 0F0
	tList<BGSImpactData>			impactDataList;			// 0F8
	tList<BGSImpactDataSet>			impactDataSetList;		// 100
	tList<TESTopic>					topicList;				// 108
	tList<TESTopicInfo>				topicInfoList;			// 110
	tList<TESQuest>					questList;				// 118
	tList<TESCombatStyle>			combatStyleList;		// 120
	tList<TESLoadScreen>			loadScreenList;			// 128
	tList<TESWaterForm>				waterFormList;			// 130
	tList<TESEffectShader>			effectShaderList;		// 138
	tList<BGSProjectile>			projectileList;			// 140
	tList<BGSExplosion>				explosionList;			// 148
	tList<BGSRadiationStage>		radiationStageList;		// 150
	tList<BGSDehydrationStage>		dehydrationStageList;	// 158
	tList<BGSHungerStage>			hungerStageList;		// 160
	tList<BGSSleepDeprevationStage>	sleepDepriveStageList;	// 168
	tList<BGSDebris>				debrisList;				// 170
	tList<BGSPerk>					perkList;				// 178
	tList<BGSBodyPartData>			bodyPartDataList;		// 180
	tList<BGSNote>					noteList;				// 188
	tList<BGSListForm>				listFormList;			// 190
	tList<BGSMenuIcon>				menuIconList;			// 198
	tList<TESObjectANIO>			anioList;				// 1A0
	tList<BGSMessage>				messageList;			// 1A8
	tList<BGSLightingTemplate>		lightingTemplateList;	// 1B0
	tList<BGSMusicType>				musicTypeList;			// 1B8
	tList<TESLoadScreenType>		loadScreenTypeList;		// 1C0
	tList<MediaSet>					mediaSetList;			// 1C8
	tList<MediaLocationController>	mediaLocControllerList;	// 1D0
	TESRegionList					*regionList;			// 1D8
	NiTArray<TESObjectCELL*>		cellArray;				// 1DC
	NiTArray<BGSAddonNode*>			addonArray;				// 1EC

	UInt32							unk1FC[3];				// 1FC	208 looks like next created refID
	UInt32							nextCreatedRefID;		// 208	Init'd to FF000800
	UInt32							unk20C;					// 20C	last unselected mod in modList. GECK: active ESM
	ModList							modList;				// 210
	UInt8							unk618;					// 618
	UInt8							unk619;					// 619
	UInt8							unk61A;					// 61A	referenced during LoadForm (ie TESSpellList). bit 1 might mean refID to pointer conversion not done. For GECK means save in progress
	UInt8							unk61B;					// 61B
	UInt32							unk61C;					// 61C
	UInt8							unk620;					// 620
	UInt8							loading;				// 621	Init'd to 0 after loadForms
	UInt8							unk622;					// 622	referenced during loading of modules
	UInt8							unk623;					// 623
	TESRegionManager				*regionManager;			// 624
	void							*vendorContainer;		// 628	ExtraContainerChanges::Data
	UInt32							unk62C;					// 62C	
	UInt32							unk630;					// 630
	UInt32							unk634;					// 634
	UInt32							unk638;					// 638

	__forceinline static DataHandler *Get() {return *(DataHandler**)0x11C3F2C;}
	const ModInfo ** GetActiveModList();		// returns array of modEntry* corresponding to loaded mods sorted by mod index
	const ModInfo* LookupModByName(const char* modName);
	UInt8 GetModIndex(const char* modName);
	UInt8 GetActiveModCount() const {return modList.modInfoList.Count();}
	const char* GetNthModName(UInt32 modIndex);

	MEMBER_FN_PREFIX(DataHandler);
	DEFINE_MEMBER_FN(DoAddForm, UInt32, 0x004603B0, TESForm * pForm);	// stupid name is because AddForm is redefined in windows header files
};
STATIC_ASSERT(sizeof(DataHandler) == 0x63C);

extern DataHandler *g_dataHandler;

// 1C
class ImageSpaceModifierInstance : public NiObject
{
public:
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual ImageSpaceModifierInstanceForm	*GetInstanceForm();
	virtual void	Unk_26(char *buffer);

	UInt8					hidden;			// 08
	UInt8					pad09[3];		// 09
	float					percent;		// 0C
	NiObject				*obj10;			// 10
	float					flt14;			// 14
	UInt32					unk18;			// 18
};

// 30
class ImageSpaceModifierInstanceForm : public ImageSpaceModifierInstance
{
public:
	TESImageSpaceModifier	*imageSpace;	// 1C
	void					*ptr20;			// 20
	float					flt24;			// 24
	NiObject				*obj28;			// 28
	float					flt2C;			// 2C
};

// 30
class ImageSpaceModifierInstanceDOF : public ImageSpaceModifierInstance
{
public:
	float					flt1C;			// 1C
	float					flt20;			// 20
	float					flt24;			// 24
	float					flt28;			// 28
	UInt32					unk2C;			// 2C
};

// 44
class ImageSpaceModifierInstanceDRB : public ImageSpaceModifierInstance
{
public:
	float					flt1C;			// 1C
	float					flt20;			// 20
	float					flt24;			// 24
	float					flt28;			// 28
	float					flt2C;			// 2C
	UInt32					unk30;			// 30
	UInt32					unk34;			// 34
	float					flt38;			// 38
	float					flt3C;			// 3C
	UInt32					unk40;			// 40
};

// 08
class SkyObject
{
public:
	virtual SkyObject	*Destroy(bool doFree);
	virtual NiNode		*GetNiNode(void);
	virtual void		InitNiNode(NiNode *skyNode04);
	virtual void		Update(Sky *sky, float value);

	NiNode				*node04;	// 04
};

// 1C
class Atmosphere : public SkyObject
{
public:
	virtual void		Init(NiNode *niNode, BSFogProperty *_fogProp);

	NiNode				*node08;	// 08
	BSFogProperty		*fogProp;	// 0C	Same as *0x11DEB00
	NiRefObject			*object10;	// 10
	NiRefObject			*object14;	// 14
	UInt8				byte18;		// 18
	UInt8				pad19[3];	// 19
};

// 10
class Stars : public SkyObject
{
public:
	NiNode			*node08;	// 08
	float			flt0C;		// 0C
};

// 2C
class Sun : public SkyObject
{
public:
	NiBillboardNode		*node08;		// 08
	NiBillboardNode		*node0C;		// 0C
	NiTriShape			*shape10;		// 10
	NiTriShape			*shape14;		// 14
	UInt32				unk18;			// 18
	NiDirectionalLight	*sunLight;		// 1C	Same as g_TES->directionalLight
	float				flt20;			// 20
	UInt8				byte24;			// 24
	UInt8				byte25;			// 25
	UInt8				byte26;			// 26
	UInt8				byte27;			// 27
	BSShaderAccumulator	*shaderAccum;	// 28
};

// 5C
class Clouds : public SkyObject
{
public:
	NiTriStrips			*layers[4];		// 08
	NiSourceTexture		*textures[4];	// 18
	NiVector3			layerPos[4];	// 28
	UInt16				numLayers;		// 58
	UInt8				byte5A;			// 5A
	UInt8				byte5B;			// 5B
};

// 7C
class Moon : public SkyObject
{
public:
	virtual void	Refresh(NiNode *niNode, const char *moonStr);

	NiNode			*node08;			// 08
	NiNode			*node0C;			// 0C
	NiTriShape		*shape10;			// 10
	NiTriShape		*shape14;			// 14
	String			moonTexture[8];		// 18
					//	0	Full Moon
					//	1	Three Wan
					//	2	Half Wan
					//	3	One Wan
					//	4	No Moon
					//	5	One Wax
					//	6	Half Wax
					//	7	Three Wax
	float			flt58;				// 58
	float			flt5C;				// 5C
	float			flt60;				// 60
	float			flt64;				// 64
	float			flt68;				// 68
	UInt32			unk6C;				// 6C
	UInt32			unk70;				// 70
	float			flt74;				// 74
	float			flt78;				// 78
};

// 18
class Precipitation
{
public:
	virtual Precipitation	*Destroy(bool doFree);

	NiNode		*node04;	// 04
	NiNode		*node08;	// 08
	UInt32		unk0C;		// 0C
	float		unk10;		// 10
	UInt32		unk14;		// 14
};

// 138
class Sky
{
public:
	virtual Sky *Destructor(bool doFree);

	struct SkySound
	{
		UInt32		unk00;		// 00
		UInt32		unk04;		// 04
		UInt32		unk08;		// 08
		TESWeather	*weather;	// 0C
		UInt32		type;		// 10
		UInt32		soundID;	// 14
	};

	NiNode							*niNode004;			// 004
	NiNode							*niNode008;			// 008
	TESClimate						*currClimate;		// 00C
	TESWeather						*currWeather;		// 010
	TESWeather						*transWeather;		// 014	Previous weather, gradually fading, on weather transition
	TESWeather						*defaultWeather;	// 018	Picked from currClimate weathers list. currClimate is set to this unless there's a regional weather
	TESWeather						*overrideWeather;	// 01C
	Atmosphere						*atmosphere;		// 020
	Stars							*stars;				// 024
	Sun								*sun;				// 028
	Clouds							*clouds;			// 02C
	Moon							*masserMoon;		// 030
	Moon							*secundaMoon;		// 034
	Precipitation					*precipitation;		// 038
	NiVector3						vector03C;			// 03C
	NiColor							waterReflection;	// 048
	NiVector3						vector054;			// 054
	NiColor							sunAmbient;			// 060
	NiColor							sunDirectional;		// 06C
	NiVector3						vector078;			// 078
	NiVector3						vector084;			// 084
	NiVector3						vector090;			// 090
	NiVector3						vector09C;			// 09C
	NiVector3						vector0A8;			// 0A8
	NiVector3						vector0B4;			// 0B4
	NiColor							sunFog;				// 0C0
	float							windSpeed;			// 0CC
	float							windDirection;		// 0D0
	UInt32							unk0D4[6];			// 0D4
	float							gameHour;			// 0EC
	float							lastUpdateHour;		// 0F0
	float							weatherPercent;		// 0F4
	UInt32							unk0F8;				// 0F8
	tList<SkySound>					*skySounds;			// 0FC
	float							lightningFxPerc;	// 100
	UInt32							unk104;				// 104
	float							flt108;				// 108
	float							flt10C;				// 10C
	float							flt110;				// 110
	UInt32							unk114;				// 114
	UInt32							flags;				// 118
	ImageSpaceModifierInstanceForm	*currFadeInIMOD;	// 11C
	ImageSpaceModifierInstanceForm	*currFadeOutIMOD;	// 120
	ImageSpaceModifierInstanceForm	*transFadeInIMOD;	// 124	On weather transition, set to the previuos weather fadeIn/OutIMODs
	ImageSpaceModifierInstanceForm	*transFadeOutIMOD;	// 128		"			"
	float							flt12C;				// 12C	Always 12.0
	float							flt130;				// 130	Always 23.99
	float							flt134;				// 134	Always 0

	__forceinline static Sky *Get() {return *(Sky**)0x11DEA20;}

	void RefreshMoon();
	__forceinline void RefreshClimate(TESClimate *climate, bool immediate = true)
	{
		ThisCall(0x63C8F0, this, climate, immediate);
	}
	bool GetIsRaining();
};
STATIC_ASSERT(sizeof(Sky) == 0x138);

// 04
class GridArray
{
public:
	virtual void	*Destroy(bool doFree);
	virtual void	Fn_01(void);
	virtual void	Fn_02(void);
	virtual void	Fn_03(void);
	virtual bool	Fn_04(UInt32 arg1, UInt32 arg2);
	virtual void	Fn_05(UInt32 arg1, UInt32 arg2);
};

// 28
class GridCellArray : public GridArray
{
public:
	virtual void	UnloadCellAtGridXY(UInt32 gridX, UInt32 gridY);
	virtual void	SetGridAtXYToNull(UInt32 gridX, UInt32 gridY);
	virtual void	CopyCellAtGridXYTo(UInt32 gridX1, UInt32 gridY1, UInt32 gridX2, UInt32 gridY2);
	virtual void	SwapCellsAtGridXYs(UInt32 gridX1, UInt32 gridY1, UInt32 gridX2, UInt32 gridY2);

	SInt32			worldX;			// 04	X coord of current cell within worldspace
	SInt32			worldY;			// 08	Y coord "
	UInt32			gridSize;		// 0C	Init'd to uGridsToLoad
	TESObjectCELL	**gridCells;	// 10	Size is gridSize^2
	float			posX;			// 14	worldX * 4096
	float			posY;			// 18	worldY * 4096
	UInt32			unk1C;			// 1C
	UInt8			byte20;			// 20
	UInt8			pad21[3];		// 21
	UInt32			unk24;			// 24
};

// 44
class LoadedAreaBound : public NiRefObject
{
public:
	bhkPhantom							*phantoms[6];	// 08	Seen bhkAabbPhantom
	TESObjectCELL						*cell;			// 20
	NiTMapBase<bhkRigidBody*, UInt32>	boundsMap;		// 24
	float								flt34;			// 34
	float								flt38;			// 38
	float								flt3C;			// 3C
	float								flt40;			// 40
};
STATIC_ASSERT(sizeof(LoadedAreaBound) == 0x44);

// A0
struct WaterSurfaceManager
{
	// 30
	struct WaterGroup
	{
		TESWaterForm			*waterForm;		// 00
		UInt32					unk04;			// 04
		UInt32					unk08;			// 08
		float					flt0C;			// 0C	Always 1.0 ?
		float					waterHeight;	// 10
		UInt32					unk14;			// 14
		UInt32					unk18;			// 18
		float					flt1C;			// 1C	-flt0C
		float					flt20;			// 20	-waterHeight
		DList<TESObjectREFR>	waterPlanes;	// 24
	};

	struct Struct8C
	{
		UInt32		unk00;
		UInt32		unk04;
		UInt32		unk08;
	};

	UInt32								unk00;			// 00
	UInt32								unk04;			// 04
	NiObject							*object08;		// 08
	NiObject							*object0C;		// 0C
	NiObject							*object10;		// 10
	NiObject							*object14;		// 14
	NiObject							*object18;		// 18
	NiObject							*object1C;		// 1C	Seen NiSourceTexture
	NiObject							*object20;		// 20
	UInt32								unk24;			// 24
	UInt32								unk28;			// 28
	UInt32								unk2C;			// 2C
	UInt32								unk30;			// 30
	UInt32								unk34;			// 34
	UInt32								unk38;			// 38
	DList<WaterGroup>					waterGroups;	// 3C
	WaterGroup							*waterLOD;		// 48	(Assumed)
	NiTPointerMap<TESObjectREFR>		map4C;			// 4C
	NiTPointerMap<TESObjectREFR>		map5C;			// 5C
	NiTPointerMap<TESWaterForm>			map6C;			// 6C
	NiTMapBase<TESObjectREFR*, void*>	map7C;			// 7C
	Struct8C							unk8C;			// 8C
	float								flt98;			// 98
	UInt32								unk9C;			// 9C
};
STATIC_ASSERT(sizeof(WaterSurfaceManager) == 0xA0);

class BSTempNodeManager;

// C4
class TES
{
public:
	virtual void Fn_00(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);

	UInt32								unk04;				// 04
	GridCellArray						*gridCellArray;		// 08
	NiNode								*niNode0C;			// 0C
	NiNode								*niNode10;			// 10
	NiNode								*niNode14;			// 14
	BSTempNodeManager					*tempNodeMgr;		// 18
	NiDirectionalLight					*directionalLight;	// 1C
	void								*ptr20;				// 20
	SInt32								extGridX;			// 24
	SInt32								extGridY;			// 28
	SInt32								extCoordX;			// 2C
	SInt32								extCoordY;			// 30
	TESObjectCELL						*currentInterior;	// 34
	TESObjectCELL						**interiorsBuffer;	// 38
	TESObjectCELL						**exteriorsBuffer;	// 3C
	UInt32								unk40[9];			// 40
	WaterSurfaceManager					*waterManager;		// 64
	Sky									*sky;				// 68
	tList<ImageSpaceModifierInstance>	activeIMODs;		// 6C
	UInt32								unk74[3];			// 74
	float								flt80;				// 80	Abs X distance from centre of grid.
	float								flt84;				// 84	Abs Y distance from centre of grid.
	TESWorldSpace						*currentWrldspc;	// 88
	tList<UInt32>						list8C;				// 8C
	tList<UInt32>						list94;				// 94
	tList<UInt32>						list9C;				// 9C
	QueuedFile							*unkA4;				// A4
	NiSourceTexture						*unkA8;				// A8
	QueuedFile							*unkAC;				// AC
	void								*ptrB0;				// B0
	UInt32								unkB4[2];			// B4
	NavMeshInfoMap						*navMeshInfoMap;	// BC
	LoadedAreaBound						*areaBound;			// C0

	__forceinline bool GetTerrainHeight(float *posXY, float *result)
	{
		return ThisCall<bool>(0x4572E0, this, posXY, result);
	}
};
STATIC_ASSERT(sizeof(TES) == 0xC4);

extern TES *g_TES;

// 34
struct GameTimeGlobals
{
	TESGlobal		*year;			// 00
	TESGlobal		*month;			// 04
	TESGlobal		*day;			// 08
	TESGlobal		*hour;			// 0C
	TESGlobal		*daysPassed;	// 10
	TESGlobal		*timeScale;		// 14
	UInt32			unk18;			// 18
	bool			gameLoaded;		// 1C
	UInt8			pad1D[3];		// 1D
	UInt32			unk20;			// 20
	UInt32			unk24;			// 24
	UInt32			unk28;			// 28
	float			lastUpdHour;	// 2C
	UInt32			initialized;	// 30

	__forceinline static GameTimeGlobals *Get() {return (GameTimeGlobals*)0x11DE7B8;}
};

// 18
class LoadedReferenceMap : public NiTPointerMap<TESObjectREFR>
{
public:
	UInt32			unk10;		// 10
	UInt32			unk14;		// 14
};

// C0
struct LoadedReferenceCollection
{
	LoadedReferenceMap	map00;					// 00	Unused?
	LoadedReferenceMap	allInventoryObjects;	// 18
	LoadedReferenceMap	weaponObjects;			// 30
	LoadedReferenceMap	ammoObjects;			// 48
	LoadedReferenceMap	containers;				// 60
	LoadedReferenceMap	actors;					// 78
	LoadedReferenceMap	radiationEmitters;		// 90
	LoadedReferenceMap	teleportDoors;			// A8

	__forceinline static LoadedReferenceCollection *Get() {return *(LoadedReferenceCollection**)0x11C95C8;}
};
extern LoadedReferenceCollection *g_loadedReferences;

// 24
struct RadioEntry
{
	TESObjectREFR	*radioRef;
	void			*ptr04;
	UInt32			unk08[7];
};

typedef ActiveEffect *(*ActiveEffectCreate)(MagicCaster *magCaster, MagicItem *magItem, EffectItem *effItem);

// 10
struct EffectArchTypeEntry
{
	const char				*name;
	ActiveEffectCreate		callback;
	UInt32					unk08[2];

	__forceinline static EffectArchTypeEntry *Array() {return (EffectArchTypeEntry*)0x1183320;}	//	Array size = 0x25
};

// 10
struct EntryPointConditionInfo
{
	const char		*entryPoint;	// 00
	UInt8			numTabs;		// 04
	UInt8			pad01[3];		// 05
	const char		**runOn;		// 08
	UInt8			byte0C;			// 0C
	UInt8			pad0D[3];		// 0D

	__forceinline static EntryPointConditionInfo *Array() {return (EntryPointConditionInfo*)0x1196EE0;}	//	Array size = 0x49
};

// 24
struct AnimGroupInfo
{
	const char	*name;			// 00
	UInt8		byte04;			// 04
	UInt8		pad05[3];		// 05
	UInt32		sequenceType;	// 08
	UInt32		unk0C;			// 0C
	UInt32		unk10;			// 10
	UInt32		unk14[4];		// 14

	__forceinline static AnimGroupInfo *Array() {return (AnimGroupInfo*)0x11977D8;}	//	Array size = 0xF5
};

struct PCMiscStat
{
	const char	*name;
	UInt32		level;

	__forceinline static PCMiscStat **Array() {return (PCMiscStat**)0x11C6D50;}	//	Array size = 0x2B
};

// 08
struct TypeSignature
{
	char	signature[8];

	__forceinline static TypeSignature *Array() {return (TypeSignature*)0x101C2AC;}	//	Array size = 0x79; order is reversed.
};