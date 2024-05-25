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
	UInt32/*NiTPtrMap<TESFile*>*/		*pointerMap;		// 008
	UInt32								unk00C;				// 00C
	BSFile*								unkFile;			// 010
	UInt32								unk014;				// 014 
	void								* unk018;			// 018 seen all zeroes. size unknown, seen not valid pointer in FalloutNV.esm
	void								* unk01C;			// 01C as above
	char								name[0x100];		// 020
	UInt32								nameHash;			// 120
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
	bool								hasModLog;			// 29B	JIP Only!
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
	//MEMBER_FN_PREFIX(ModInfo);
	//DEFINE_MEMBER_FN(GetNextChunk, UInt32, 0x004726B0);	// returns chunk type
	//DEFINE_MEMBER_FN(GetChunkData, bool, 0x00472890, UInt8* buf, UInt32 bufSize); // max size, not num to read
	//DEFINE_MEMBER_FN(Read32, void, 0x004727F0, void* out);
	//DEFINE_MEMBER_FN(HasMoreSubrecords, bool, 0x004726F0);
};
static_assert(sizeof(WIN32_FIND_DATA) == 0x140);
static_assert(sizeof(ModInfo) == 0x42C);

struct Timer
{
	UInt8		disableCounter;			// 00
	UInt8		pad01[3];				// 01
	float		fpsClamp;				// 04
	float		fpsClampRemainder;		// 08
	float		secondsPassed;			// 0C
	float		lastSecondsPassed;		// 10
	UInt32		msPassed;				// 14
	UInt32		tickCount;				// 18
	UInt8		isChangeTimeMultSlowly;	// 1C
	UInt8		byte1D;					// 1D
	UInt8		pad1E[2];				// 1E
};

struct TimeGlobal : Timer
{
	float		flt20;
	float		flt24;
	float		flt28;

	static TimeGlobal *Get() {return (TimeGlobal*)0x11F6394;}
};

struct ModList
{
	tList<ModInfo>		modInfoList;		// 00
	UInt32				loadedModCount;		// 08
	ModInfo*			loadedMods[0xFF];	// 0C
};
static_assert(sizeof(ModList) == 0x408);

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
	UInt8							byte618;				// 618
	UInt8							byte619;				// 619
	UInt8							byte61A;				// 61A	referenced during LoadForm (ie TESSpellList). bit 1 might mean refID to pointer conversion not done. For GECK means save in progress
	UInt8							byte61B;				// 61B
	UInt8							byte61C;				// 61C
	UInt8							byte61D;				// 61D
	UInt8							byte61E;				// 61E
	UInt8							byte61F;				// 61F
	UInt8							byte620;				// 620
	UInt8							loading;				// 621	Init'd to 0 after loadForms
	UInt8							byte622;				// 622	referenced during loading of modules
	UInt8							byte623;				// 623
	TESRegionManager				*regionManager;			// 624
	void							*vendorContainer;		// 628	ExtraContainerChanges::Data
	UInt32							unk62C;					// 62C	
	UInt32							unk630;					// 630
	UInt32							unk634;					// 634
	UInt32							unk638;					// 638

	__forceinline static DataHandler *GetSingleton() {return *(DataHandler**)0x11C3F2C;}
	ModInfo* __fastcall LookupModByName(const char *modName) const;

	UInt8 __fastcall GetModIndex(const char *modName) const
	{
		ModInfo *modInfo = LookupModByName(modName);
		return modInfo ? modInfo->modIndex : 0xFF;
	}

	UInt8 GetActiveModCount() const {return modList.loadedModCount;}
	inline ModInfo *GetNthModInfo(UInt32 modIndex) const
	{
		return modList.loadedMods[modIndex];
	}
	inline const char *GetNthModName(UInt32 modIndex) const
	{
		auto modInfo = (modIndex < 0xFF) ? modList.loadedMods[modIndex] : nullptr;
		return modInfo ? modInfo->name : "";
	}

	__forceinline bool DoAddForm(TESForm *pForm)
	{
		return ThisCall<bool>(0x4603B0, this, pForm);
	}

	void DecompileModScripts(UInt8 modIdx, UInt8 typeMask = 0x1F);
};
static_assert(sizeof(DataHandler) == 0x63C);

extern DataHandler *g_dataHandler;

// 1C
class ImageSpaceModifierInstance : public NiObject
{
public:
	/*8C*/virtual bool	IsExpired();
	/*90*/virtual void	Apply();
	/*94*/virtual ImageSpaceModifierInstanceForm* GetInstanceForm();
	/*98*/virtual void	PrintInfo(char *buffer);

	UInt8					hidden;			// 08
	UInt8					pad09[3];		// 09
	float					percent;		// 0C
	NiObject				*target;		// 10
	float					age;			// 14
	UInt32					flags;			// 18
};

// 20
class ImageSpaceModifierInstanceTemp : public ImageSpaceModifierInstance
{
public:
	float					duration;		// 1C
};

// 30
class ImageSpaceModifierInstanceForm : public ImageSpaceModifierInstance
{
public:
	TESImageSpaceModifier	*imageSpaceMod;		// 1C
	TESImageSpaceModifier	*lastImageSpaceMod;	// 20
	float					lastStrength;		// 24
	NiObject				*lastTarget;		// 28
	float					transitionTime;		// 2C
};

// 30
class ImageSpaceModifierInstanceDOF : public ImageSpaceModifierInstanceTemp
{
public:
	float					distance;		// 20
	float					focalDepthRange;// 24
	float					flt28;			// 28
	UInt32					unk2C;			// 2C
};

// 38
class ImageSpaceModifierInstanceRB : public ImageSpaceModifierInstanceTemp
{
public:
	float					rampUp;			// 20
	float					start;			// 24
	float					rampDown;		// 28
	float					downStart;		// 2C
	NiPoint2				centre;			// 30
};

// 44
class ImageSpaceModifierInstanceDRB : public ImageSpaceModifierInstanceTemp
{
public:
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

	NiNode				*node08;		// 08
	BSFogProperty		*fogProp;		// 0C	Same as *0x11DEB00
	NiRefObject			*skyQuadNode;	// 10
	NiRefObject			*skyQuad;		// 14
	UInt8				updateFogDist;	// 18
	UInt8				pad19[3];		// 19
};

// 10
class Stars : public SkyObject
{
public:
	NiNode			*starsNode;	// 08
	float			alpha;		// 0C
};

// 2C
class Sun : public SkyObject
{
public:
	NiBillboardNode		*node08;		// 08
	NiBillboardNode		*sunGlare;		// 0C
	NiTriShape			*sunBase;		// 10
	NiTriShape			*shape14;		// 14
	UInt32				unk18;			// 18
	NiDirectionalLight	*sunLight;		// 1C	Same as g_TES->directionalLight
	float				glareScale;		// 20
	UInt8				bDoOcclusionTests;	// 24
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
	UInt8				bForceUpdate;	// 5A
	UInt8				byte5B;			// 5B
};

// 7C
class Moon : public SkyObject
{
public:
	virtual void	Refresh(NiNode *niNode, const char *moonStr);

	NiNode			*moonNode;			// 08
	NiNode			*shadowNode;		// 0C
	NiTriShape		*moonGeom;			// 10
	NiTriShape		*shadowGeom;		// 14
	String			moonTexture[8];		// 18
					//	0	Full Moon
					//	1	Three Wan
					//	2	Half Wan
					//	3	One Wan
					//	4	No Moon
					//	5	One Wax
					//	6	Half Wax
					//	7	Three Wax
	float			angleFadeStart;		// 58
	float			angleFadeEnd;		// 5C
	float			shadowEarlyFade;	// 60
	float			speed;				// 64
	float			offsetZ;			// 68
	UInt32			size;				// 6C
	UInt32			unk70;				// 70
	float			flt74;				// 74
	float			lastUpdateHour;		// 78
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

	enum SkyMode
	{
		kSkyMode_None,
		kSkyMode_Interior,
		kSkyMode_SkyDomeOnly,
		kSkyMode_Full
	};

	enum SkyFlags
	{
		kSkyFlag_IsInitWeather =				1,
		kSkyFlag_IsUpdateWeather =				3,
		kSkyFlag_IsUnderwater =					4,
		kSkyFlag_SetAcceleration =				8,
		kSkyFlag_FastTravel =					0x10,
		kSkyFlag_UpdateMoonPhase =				0x20,
		kSkyFlag_IsClimateChange =				0x40,
		kSkyFlag_NoCachedSunriseBegin =			0x100,
		kSkyFlag_NoCachedSunriseEnd =			0x200,
		kSkyFlag_NoCachedSunsetBegin =			0x400,
		kSkyFlag_NoCachedSunsetEnd =			0x800,
		kSkyFlag_NoCachedBeginSunriseColors =	0x1000,
		kSkyFlag_NoCachedEndSunsetColors =		0x2000
	};

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
	NiColor							skyUpper;			// 03C
	NiColor							waterReflection;	// 048
	NiColor							color054;			// 054
	NiColor							sunAmbient;			// 060
	NiColor							sunDirectional;		// 06C
	NiColor							color078;			// 078
	NiColor							color084;			// 084
	NiColor							skyLower;			// 090
	NiColor							horizon;			// 09C
	NiColor							color0A8;			// 0A8
	NiVector3						vector0B4;			// 0B4
	NiColor							sunFog;				// 0C0
	float							windSpeed;			// 0CC
	float							windDirection;		// 0D0
	UInt32							unk0D4[5];			// 0D4
	float							fogPower;			// 0E8
	float							gameHour;			// 0EC
	float							lastUpdateHour;		// 0F0
	float							weatherPercent;		// 0F4
	UInt32							skyMode;			// 0F8
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
	bool GetIsRaining() const;
};
static_assert(sizeof(Sky) == 0x138);
extern Sky *g_currentSky;

// 04
class GridArray
{
public:
	/*00*/virtual void	*Destroy(bool doFree);
	/*04*/virtual void	Fn_01(void);
	/*08*/virtual void	Fn_02(void);
	/*0C*/virtual void	Fn_03(void);
	/*10*/virtual bool	SetPosXY(SInt32 posX, SInt32 posY);
	/*14*/virtual void	Fn_05(UInt32 arg1, UInt32 arg2);
};

struct CellCoord
{
	SInt32		x;
	SInt32		y;
};

// 28
class GridCellArray : public GridArray
{
public:
	/*18*/virtual void	UnloadCellAtGridXY(UInt32 gridX, UInt32 gridY);
	/*1C*/virtual void	SetGridAtXYToNull(UInt32 gridX, UInt32 gridY);
	/*20*/virtual void	CopyCellAtGridXYTo(UInt32 gridX1, UInt32 gridY1, UInt32 gridX2, UInt32 gridY2);
	/*24*/virtual void	SwapCellsAtGridXYs(UInt32 gridX1, UInt32 gridY1, UInt32 gridX2, UInt32 gridY2);

	CellCoord		worldXY;		// 04	Coord of current cell within worldspace
	UInt32			gridSize;		// 0C	Init'd to uGridsToLoad
	TESObjectCELL	**gridCells;	// 10	Size is gridSize^2
	NiVector3		posXY;			// 14	worldXY * 4096
	UInt8			byte20;			// 20
	UInt8			pad21[3];		// 21
	NiObject		*object24;		// 24

	class Iterator
	{
		TESObjectCELL	**pCells;
		UInt32			count;

	public:
		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pCells++;
			count--;
		}

		TESObjectCELL* operator*() const {return *pCells;}
		TESObjectCELL* operator->() const {return *pCells;}

		Iterator(GridCellArray &source) : pCells(source.gridCells), count(source.gridSize * source.gridSize) {}
	};

	Iterator Begin() {return Iterator(*this);}

	TESObjectCELL* __vectorcall GetCellAtPos(__m128 pos) const;
	TESObjectCELL* __vectorcall GetCellAtCoord(__m128i cellXY) const;
};
extern GridCellArray *g_gridCellArray;;

// 44
class LoadedAreaBound : public NiRefObject
{
public:
	bhkPhantom						*phantoms[6];	// 08	Seen bhkAabbPhantom
	TESObjectCELL					*cell;			// 20
	NiTMap<bhkRigidBody*, UInt16>	boundsMap;		// 24
	float							flt34;			// 34	Init'd to 20.0
	float							flt38;			// 38	Init'd to 600.0
	float							flt3C;			// 3C	Init'd to 1000.0
	float							flt40;			// 40	Init'd to 0.3

	__forceinline void GetGroundPos(TESObjectREFR *refr, NiVector3 *currPos, NiVector3 *outPos)
	{
		ThisCall(0x6296D0, this, refr, currPos, outPos);
	}
};
static_assert(sizeof(LoadedAreaBound) == 0x44);

// A0
struct WaterSurfaceManager
{
	struct WadingWaterData;

	// B0 c'tor @ 0x4ED5F0
	struct WaterGroup
	{
		TESWaterForm			*waterForm;			// 00
		NiPlane					plane04;			// 04
		NiPlane					plane14;			// 14
		DList<TESObjectREFR>	waterPlanes;		// 24
		DList<void>				list30;				// 30
		DList<void>				list3C;				// 3C
		DList<void>				list48;				// 48
		BSRenderedTexture		*waterRenderTarget;	// 54
		NiAVObject				*object58;			// 58
		bool					usesWaterLevel;		// 5C
		UInt8					byte5D;				// 5D
		UInt8					byte5E;				// 5E
		bool					isInterior;			// 5F
		bool					allowLowDetailReflections;	// 60
		UInt8					pad61[3];			// 61
		DList<NiNode>			geometryGroup;		// 64
		DList<NiNode>			list70;				// 70
		DList<NiNode>			depthCellGeometry;	// 7C
		DList<NiNode>			list88;				// 88
		BSShaderAccumulator		*shaderAccum94;		// 94
		BSShaderAccumulator		*shaderAccum98;		// 98
		UInt32					reflectionGroupCount;	// 9C
		UInt32					depthGroupCount;	// A0
		NiCamera				*cameraA4;			// A4
		NiCamera				*depthCamera;		// A8
		UInt16					stencilMask;		// AC
		UInt16					wordAE;				// AE
	};

	UInt32										unk00;			// 00
	UInt32										unk04;			// 04
	NiObject									*object08;		// 08
	NiObject									*object0C;		// 0C
	NiObject									*object10;		// 10
	NiObject									*object14;		// 14
	NiObject									*object18;		// 18
	NiObject									*noiseTexture;	// 1C	Seen NiSourceTexture
	NiObject									*object20;		// 20
	UInt32										unk24;			// 24
	UInt32										unk28;			// 28
	UInt32										unk2C;			// 2C
	UInt32										unk30;			// 30
	UInt32										unk34;			// 34
	UInt32										unk38;			// 38
	DList<WaterGroup>							waterGroups;	// 3C
	WaterGroup									*waterLOD;		// 48	(Assumed)
	NiTMap<TESObjectREFR*, TESObjectREFR*>		map4C;			// 4C
	NiTMap<TESObjectREFR*, TESObjectREFR*>		map5C;			// 5C
	NiTMap<TESWaterForm*, bool>					map6C;			// 6C
	NiTMap<TESObjectREFR*, WadingWaterData*>	wadingWaterMap;	// 7C
	Sound										sound8C;		// 8C
	float										flt98;			// 98
	UInt32										unk9C;			// 9C

	__forceinline static NiNode *GetWaterLOD() {return *(NiNode**)0x11DEA1C;}
	__forceinline static NiNode *GetWaterWade() {return *(NiNode**)0x11C7C28;}

	void __fastcall Update(NiCamera *camera);
	void __fastcall UpdateEx(NiCamera *camera);
};
static_assert(sizeof(WaterSurfaceManager) == 0xA0);

// C4
class TES
{
public:
	virtual void Fn_00(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);

	struct DeathCount
	{
		TESActorBase	*actorBase;
		UInt16			count;
	};

	struct ParticleNode
	{
		NiNode			*particleParent;
		void			*ptr;
		ParticleNode	*next;
	};

	UInt32								unk04;				// 04
	GridCellArray						*gridCellArray;		// 08
	NiNode								*objRoot;			// 0C
	NiNode								*LODroot;			// 10
	NiNode								*waterLODRoot;		// 14
	BSTempNodeManager					*tempNodeMgr;		// 18
	NiDirectionalLight					*directionalLight;	// 1C
	BSFogProperty						*fogProperty;		// 20
	CellCoord							exteriorGrid;		// 24
	CellCoord							exteriorCoord;		// 2C
	TESObjectCELL						*currentInterior;	// 34
	TESObjectCELL						**interiorsBuffer;	// 38
	TESObjectCELL						**exteriorsBuffer;	// 3C
	UInt32								interiorBufferSize;	// 40
	UInt32								exteriorBufferSize;	// 44
	CellCoord							savedGrid;			// 48
	UInt8								byte50;				// 50
	bool								isTestAllCells;		// 51
	UInt8								byte52;				// 52
	UInt8								pad53;				// 53
	void								*renderTestCellsCallback;	// 54
	UInt32								unk58;				// 58
	UInt32								unk5C;				// 5C
	bool								showCellBorders;	// 60
	UInt8								pad61[3];			// 61
	WaterSurfaceManager					*waterManager;		// 64
	Sky									*sky;				// 68
	tList<ImageSpaceModifierInstance>	activeIMODs;		// 6C
	UInt32								unk74[3];			// 74
	float								flt80;				// 80	Abs X distance from centre of grid.
	float								flt84;				// 84	Abs Y distance from centre of grid.
	TESWorldSpace						*currentWrldspc;	// 88
	tList<UInt32>						list8C;				// 8C
	tList<TESObjectREFR>				cellFurnitureRefs;	// 94
	tList<DeathCount>					deathCounts;		// 9C
	QueuedFile							*unkA4;				// A4
	NiSourceTexture						*bloodTexture;		// A8
	QueuedFile							*unkAC;				// AC
	ParticleNode						*particleCacheHead;	// B0
	bool								fadeWhenLoading;	// B4
	bool								allowUnusedPurge;	// B5
	UInt8								byteB6;				// B6
	UInt8								byteB7;				// B7
	UInt32								placeableWaterCount;// B8
	NavMeshInfoMap						*navMeshInfoMap;	// BC
	LoadedAreaBound						*areaBound;			// C0

	__forceinline static TES *GetSingleton() {return *(TES**)0x11DEA10;}

	__forceinline bool GetTerrainHeight(NiPoint2 *posXY, float *result)
	{
		return ThisCall<bool>(0x4572E0, this, posXY, result);
	}

	__forceinline void AddTempNode(NiAVObject *object, float lifetime)
	{
		ThisCall(0x458E20, this, object, lifetime);
	}

	void UnloadBufferedExterior(TESObjectCELL *cell);

	__forceinline void CreateTextureImage(const char *filePath, NiSourceTexture **outTexture, bool mustFindFile = 1, bool archiveOnly = 0)
	{
		//	filePath is relative to FalloutNV root, or optionally to Data.
		ThisCall(0x4568C0, this, filePath, outTexture, mustFindFile, archiveOnly);
	}

	static NiLines *DrawAxesLines(float size)
	{
		return CdeclCall<NiLines*>(0xC59200, size);
	}

	static NiLines *DrawArrow(NiVector3 *direction, const NiColorAlpha &color)
	{
		auto shape = CdeclCall<NiLines*>(0xC59A50, direction, &color);
		shape->AddProperty(BSShaderNoLightingProperty::Create());
		shape->AddProperty(s_alphaProperty);
		shape->AssignGeometryProps();
		return shape;
	}

	static NiTriShape *DrawOctahedron(float diagonal, const NiColorAlpha &color)
	{
		auto shape = CdeclCall<NiTriShape*>(0x4B29B0, diagonal, &color, 0);
		shape->AddProperty(BSShaderNoLightingProperty::Create());
		shape->AddProperty(s_alphaProperty);
		shape->AssignGeometryProps();
		return shape;
	}

	static NiNode *DrawSphere(float diameter, UInt32 numEdges, const NiColorAlpha &color)
	{
		auto sphere = CdeclCall<NiNode*>(0xC56680, diameter, numEdges, numEdges, &color);
		sphere->AddNoLightingPropertyRecurse();
		sphere->AddPropertyRecurse(s_alphaProperty);
		sphere->AssignGeometryProps();
		return sphere;
	}

	static NiTriStrips *DrawCylinder(float radius, float height, UInt32 numEdges, const NiColorAlpha &color)
	{
		auto shape = NiTriStrips::Create(NiTriStripsData::DrawCylinder(radius, height, numEdges, color));
		/*auto stencilProp = NiStencilProperty::Create();
		stencilProp->flags = 0x4D80;
		shape->AddProperty(stencilProp);*/
		shape->AddProperty(BSShaderNoLightingProperty::Create());
		shape->AddProperty(s_alphaProperty);
		shape->AssignGeometryProps();
		return shape;
	}

	static NiTriStrips *DrawCone(float diameter, float height, UInt32 numEdges, const NiColorAlpha &color)
	{
		auto shape = CdeclCall<NiTriStrips*>(0xC584A0, diameter, 0, height, numEdges, 1, &color);
		/*auto stencilProp = NiStencilProperty::Create();
		stencilProp->flags = 0x4D80;
		shape->AddProperty(stencilProp);*/
		shape->AddProperty(BSShaderNoLightingProperty::Create());
		shape->AddProperty(s_alphaProperty);
		shape->AssignGeometryProps();
		return shape;
	}

	static NiTriStrips *DrawConvex(float radius, UInt32 numEdges, const NiColorAlpha &color)
	{
		auto convex = NiTriStrips::Create(NiTriStripsData::DrawConvex(radius, numEdges, color));
		convex->AddProperty(BSShaderNoLightingProperty::Create());
		convex->AddProperty(s_alphaProperty);
		convex->AssignGeometryProps();
		return convex;
	}

	static NiTriStrips *DrawPrism(float radius, float height, UInt32 numEdges, const NiColorAlpha &color)
	{
		auto shape = NiTriStrips::Create(NiTriStripsData::DrawPrism(radius, height, numEdges, color));
		shape->AddProperty(BSShaderNoLightingProperty::Create());
		shape->AddProperty(s_alphaProperty);
		shape->AssignGeometryProps();
		return shape;
	}

	static NiTriStrips *DrawTorus(float diamtrTube, float diamtrRing, UInt32 numEdgesTube, UInt32 numEdgesRing, const NiColorAlpha &color)
	{
		auto shape = CdeclCall<NiTriStrips*>(0xC57D70, diamtrTube, diamtrRing, numEdgesRing, numEdgesTube, &color);
		shape->AddProperty(BSShaderNoLightingProperty::Create());
		shape->AddProperty(s_alphaProperty);
		shape->AssignGeometryProps();
		return shape;
	}
};
static_assert(sizeof(TES) == 0xC4);

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

// 34
struct PositionRequest
{
	TESWorldSpace		*worldSpace;	// 00
	TESObjectCELL		*cell;			// 04
	NiVector3			position;		// 08
	NiVector3			rotation;		// 14
	bool				resetWeather;	// 20
	UInt8				pad21[3];		// 21
	void				*callback;		// 24
	UInt32				cbArg;			// 28
	TESObjectREFR		*destRef;		// 2C
	TESObjectREFR		*fastTravelRef;	// 30
};

// 18
class LoadedReferenceMap : public NiTPtrMap<TESObjectREFR>
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

typedef ActiveEffect* (*ActiveEffectCreate)(MagicCaster*, MagicItem*, EffectItem*);

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
	enum AnimKeyType
	{
		kKey_ClampSequence,
		kKey_LoopingSequence,
		kKey_SpecialIdle,
		kKey_Equip,
		kKey_Unequip,
		kKey_Attack,
		kKey_PowerAttackOrPipboy,
		kKey_AttackThrow,
		kKey_PlaceMine,
		kKey_SpinAttack,
		kKey_LoopingReload
	};

	const char	*name;			// 00
	bool		usesVariants;	// 04
	UInt8		pad05[3];		// 05
	UInt32		sequenceType;	// 08
	UInt32		keyType;		// 0C
	UInt32		unk10;			// 10
	UInt32		unk14;			// 14
	UInt32		unk18;			// 18
	UInt32		unk1C;			// 1C
	UInt32		unk20;			// 20

	__forceinline static AnimGroupInfo *Array() {return (AnimGroupInfo*)0x11977D8;}	//	Array size = 0xF5
};

struct PCMiscStat
{
	Setting		*name;
	UInt32		level;

	__forceinline static PCMiscStat **Array() {return (PCMiscStat**)0x11C6D50;}	//	Array size = 0x2B
};

// 08
struct TypeSignature
{
	char	signature[8];

	__forceinline static TypeSignature *Array() {return (TypeSignature*)0x101C2AC;}	//	Array size = 0x79; order is reversed.
};