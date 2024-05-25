#pragma once

enum FormType
{
	/*00*/kFormType_None,
	/*01*/kFormType_TES4,
	/*02*/kFormType_Group,
	/*03*/kFormType_GMST,
	/*04*/kFormType_BGSTextureSet,
	/*05*/kFormType_BGSMenuIcon,
	/*06*/kFormType_TESGlobal,
	/*07*/kFormType_TESClass,
	/*08*/kFormType_TESFaction,
	/*09*/kFormType_BGSHeadPart,
	/*0A*/kFormType_TESHair,
	/*0B*/kFormType_TESEyes,
	/*0C*/kFormType_TESRace,
	/*0D*/kFormType_TESSound,
	/*0E*/kFormType_BGSAcousticSpace,
	/*0F*/kFormType_TESSkill,
	/*10*/kFormType_EffectSetting,
	/*11*/kFormType_Script,
	/*12*/kFormType_TESLandTexture,
	/*13*/kFormType_EnchantmentItem,
	/*14*/kFormType_SpellItem,
	/*15*/kFormType_TESObjectACTI,
	/*16*/kFormType_BGSTalkingActivator,
	/*17*/kFormType_BGSTerminal,
	/*18*/kFormType_TESObjectARMO,
	/*19*/kFormType_TESObjectBOOK,
	/*1A*/kFormType_TESObjectCLOT,
	/*1B*/kFormType_TESObjectCONT,
	/*1C*/kFormType_TESObjectDOOR,
	/*1D*/kFormType_IngredientItem,
	/*1E*/kFormType_TESObjectLIGH,
	/*1F*/kFormType_TESObjectMISC,
	/*20*/kFormType_TESObjectSTAT,
	/*21*/kFormType_BGSStaticCollection,
	/*22*/kFormType_BGSMovableStatic,
	/*23*/kFormType_BGSPlaceableWater,
	/*24*/kFormType_TESGrass,
	/*25*/kFormType_TESObjectTREE,
	/*26*/kFormType_TESFlora,
	/*27*/kFormType_TESFurniture,
	/*28*/kFormType_TESObjectWEAP,
	/*29*/kFormType_TESAmmo,
	/*2A*/kFormType_TESNPC,
	/*2B*/kFormType_TESCreature,
	/*2C*/kFormType_TESLevCreature,
	/*2D*/kFormType_TESLevCharacter,
	/*2E*/kFormType_TESKey,
	/*2F*/kFormType_AlchemyItem,
	/*30*/kFormType_BGSIdleMarker,
	/*31*/kFormType_BGSNote,
	/*32*/kFormType_BGSConstructibleObject,
	/*33*/kFormType_BGSProjectile,
	/*34*/kFormType_TESLevItem,
	/*35*/kFormType_TESWeather,
	/*36*/kFormType_TESClimate,
	/*37*/kFormType_TESRegion,
	/*38*/kFormType_NavMeshInfoMap,
	/*39*/kFormType_TESObjectCELL,
	/*3A*/kFormType_TESObjectREFR,
	/*3B*/kFormType_Character,
	/*3C*/kFormType_Creature,
	/*3D*/kFormType_MissileProjectile,
	/*3E*/kFormType_GrenadeProjectile,
	/*3F*/kFormType_BeamProjectile,
	/*40*/kFormType_FlameProjectile,
	/*41*/kFormType_TESWorldSpace,
	/*42*/kFormType_TESObjectLAND,
	/*43*/kFormType_NavMesh,
	/*44*/kFormType_TLOD,
	/*45*/kFormType_TESTopic,
	/*46*/kFormType_TESTopicInfo,
	/*47*/kFormType_TESQuest,
	/*48*/kFormType_TESIdleForm,
	/*49*/kFormType_TESPackage,
	/*4A*/kFormType_TESCombatStyle,
	/*4B*/kFormType_TESLoadScreen,
	/*4C*/kFormType_TESLevSpell,
	/*4D*/kFormType_TESObjectANIO,
	/*4E*/kFormType_TESWaterForm,
	/*4F*/kFormType_TESEffectShader,
	/*50*/kFormType_TOFT,
	/*51*/kFormType_BGSExplosion,
	/*52*/kFormType_BGSDebris,
	/*53*/kFormType_TESImageSpace,
	/*54*/kFormType_TESImageSpaceModifier,
	/*55*/kFormType_BGSListForm,
	/*56*/kFormType_BGSPerk,
	/*57*/kFormType_BGSBodyPartData,
	/*58*/kFormType_BGSAddonNode,
	/*59*/kFormType_ActorValueInfo,
	/*5A*/kFormType_BGSRadiationStage,
	/*5B*/kFormType_BGSCameraShot,
	/*5C*/kFormType_BGSCameraPath,
	/*5D*/kFormType_BGSVoiceType,
	/*5E*/kFormType_BGSImpactData,
	/*5F*/kFormType_BGSImpactDataSet,
	/*60*/kFormType_TESObjectARMA,
	/*61*/kFormType_BGSEncounterZone,
	/*62*/kFormType_BGSMessage,
	/*63*/kFormType_BGSRagdoll,
	/*64*/kFormType_DOBJ,
	/*65*/kFormType_BGSLightingTemplate,
	/*66*/kFormType_BGSMusicType,
	/*67*/kFormType_TESObjectIMOD,
	/*68*/kFormType_TESReputation,
	/*69*/kFormType_ContinuousBeamProjectile,
	/*6A*/kFormType_TESRecipe,
	/*6B*/kFormType_TESRecipeCategory,
	/*6C*/kFormType_TESCasinoChips,
	/*6D*/kFormType_TESCasino,
	/*6E*/kFormType_TESLoadScreenType,
	/*6F*/kFormType_MediaSet,
	/*70*/kFormType_MediaLocationController,
	/*71*/kFormType_TESChallenge,
	/*72*/kFormType_TESAmmoEffect,
	/*73*/kFormType_TESCaravanCard,
	/*74*/kFormType_TESCaravanMoney,
	/*75*/kFormType_TESCaravanDeck,
	/*76*/kFormType_BGSDehydrationStage,
	/*77*/kFormType_BGSHungerStage,
	/*78*/kFormType_BGSSleepDeprevationStage,
	kFormType_Max,
};

#define IS_ID(form, type) (form->typeID == kFormType_##type)
#define NOT_ID(form, type) (form->typeID != kFormType_##type)

extern const bool kInventoryType[];
extern const UInt8 kMaterialConvert[];

/**** bases ****/

class BaseFormComponent
{
public:
	/*000*/virtual void	Init(void);
	/*004*/virtual void	Free(void);
	/*008*/virtual void	CopyFromBase(BaseFormComponent * component);
	/*00C*/virtual bool	CompareWithBase(BaseFormComponent * src);

//	void		** _vtbl;	// 000
};

struct PermanentClonedForm
{
	UInt32 orgRefID;
	UInt32 cloneRefID;
};

extern BGSSaveLoadGame *g_BGSSaveLoadGame;

// 018
class TESForm : public BaseFormComponent
{
public:
	/*010*/virtual void		Destroy(bool doFree);				// func_00C in GECK ?? I think ??
	/*014*/virtual void		SetToDefault();						// Might be set default value (called from constructor)
	/*018*/virtual UInt32	Unk_06(void);
	/*01C*/virtual bool		Unk_07(void);
	/*020*/virtual bool		LoadForm(ModInfo * modInfo);		// func_010 in GECK
	/*024*/virtual bool		Unk_09(void * arg);					// points to LoadForm on TESForm
	/*028*/virtual bool		AppendForm(ModInfo* modInfo);		// (ie SaveForm + append to modInfo)
	/*02C*/virtual void		SaveForm(void);						// saves in same format as in .esp	//	func_013 in GECK
															// data buffer and buffer size stored in globals when done, doesn't return anything
	/*030*/virtual bool		LoadForm2(ModInfo * modInfo);		// just calls LoadForm
	/*034*/virtual void		WriteFormInfo(ModInfo* modInfo);	// does some saving stuff, then calls Fn0A
	/*038*/virtual bool		Unk_0E(void * arg);					// prapares a GRUP formInfo
	/*03C*/virtual bool		Sort(TESForm * form);				// returns if the argument is "greater or equal" to this form
	/*040*/virtual TESForm *CreateForm(void * arg0, void * mapToAddTo);	// makes a new form, 
	/*044*/virtual void		Unk_11(void * arg);
	/*048*/virtual void		MarkAsModified(UInt32 changedFlags);		// enable changed flag?
	/*04C*/virtual void		MarkAsUnmodified(UInt32 changedFlags);		// disable changed flag?
	/*050*/virtual UInt32	GetSaveSize(UInt32 changedFlags);	// bytes taken by the delta flags for this form, UNRELIABLE, not (always) overriden
	/*054*/virtual void		AppendSaveLoadFormBuffer(void *arg);					// collect referenced forms?
	/*058*/virtual void		SaveGame(UInt32 changedFlags);		// Used as part of CopyFromBase with LoadGame.
	/*05C*/virtual void		LoadGame(void * arg);				// load from BGSLoadFormBuffer arg
	/*060*/virtual void		LoadGame2(UInt32 changedFlags);		// load from TESSaveLoadGame
	/*064*/virtual void		InitLoadGame(void * arg);
	/*068*/virtual void		Unk_1A(void * arg0, void * arg1);
	/*06C*/virtual void		Unk_1B(void * arg0, void * arg1);
	/*070*/virtual void		Revert(UInt32 changedFlags);		// reset changes in form
	/*074*/virtual void		Unk_1D(void * arg);
	/*078*/virtual void		Unk_1E(void * arg);
	/*07C*/virtual bool		Unk_1F(void * arg);
	/*080*/virtual void		Unk_20(void * arg);
	/*084*/virtual void		Unk_21(void * arg);
	/*088*/virtual void		InitItem();
	/*08C*/virtual UInt32	GetTypeID();
	/*090*/virtual void		GetDebugName(String * dst);
	/*094*/virtual bool		IsPersistentOrQuestItem();
										// Unk_26 though Unk_36 get or set flag bits
	/*098*/virtual bool		HasTalkedToPC();		// 00000040
	/*09C*/virtual bool		GetHavokDeath();		// 00010000
	/*0A0*/virtual bool		Unk_28(void);		// 00010000
	/*0A4*/virtual bool		GetNeedToChangeProcess();		// 00020000
	/*0A8*/virtual bool		Unk_2A(void);		// 00020000
	/*0AC*/virtual bool		Unk_2B(void);		// 00080000
	/*0B0*/virtual bool		Unk_2C(void);		// 02000000
	/*0B4*/virtual bool		Unk_2D(void);		// 40000000
	/*0B8*/virtual bool		Unk_2E(void);		// 00000200
	/*0BC*/virtual void		Unk_2F(bool set);	// 00000200
	/*0C0*/virtual bool		Unk_30(void);		// returns false
	/*0C4*/virtual void		UpdateDeleted(bool set);	// 00000020 then calls Fn12 MarkAsModified
	/*0C8*/virtual void		SetAltered(bool set);	// 00000002 with a lot of housekeeping
	/*0CC*/virtual void		SetQuestItem(bool set);	// 00000400 then calls Fn12 MarkAsModified
	/*0D0*/virtual void		SetTalkedToPC(bool set);	// 00000040 then calls Fn12 MarkAsModified
	/*0D4*/virtual void		SetHavokDeath(bool set);	// 00010000 then calls Fn12 MarkAsModified
	/*0D8*/virtual void		SetNeedToChangeProcess(bool set);	// 00020000
	/*0DC*/virtual void		Unk_37(void);		// write esp format
	/*0E0*/virtual void		readOBNDSubRecord(ModInfo * modInfo);	// read esp format
	/*0E4*/virtual bool		Unk_39(void);		// Identical to IsBoundObject (3A)
	/*0E8*/virtual bool		IsBoundObject();
	/*0EC*/virtual bool		IsMagicItem();		// EnchantmentItem, SpellItem, IngredientItem, AlchemyItem
	/*0F0*/virtual bool		GetIsReference();
	/*0F4*/virtual bool		IsArmorAddon();		// TESObjectARMA
	/*0F8*/virtual bool		IsActorBase();		// TESNPC or TESCreature
	/*0FC*/virtual bool		IsMobileObject();	// Actor, Projectile or Explosion
	/*100*/virtual bool		IsActor(void);
	/*104*/virtual UInt32	Unk_41(void);
	/*108*/virtual void		CopyFrom(const TESForm * form);
	/*10C*/virtual bool		Compare(TESForm * form);
	/*110*/virtual bool		CheckFormGRUP(void * arg);	// Checks the group is valid for the form
	/*114*/virtual void		InitFormGRUP(void * dst, void * arg1);	// Fills the groupInfo with info valid for the form
	/*118*/virtual bool		Unk_46(void);
	/*11C*/virtual bool		Unk_47(void);
	/*120*/virtual bool		Unk_48(UInt32 formType);	// returns if the same FormType is passed in
	/*124*/virtual bool		ActivateRef(TESObjectREFR *activatedRef, TESObjectREFR *activatingRef, UInt8 arg3, TESForm *form, UInt32 count);	// looks to be func33 in Oblivion
	/*128*/virtual void		SetRefID(UInt32 refID, bool generateID);
	/*12C*/virtual const char	*GetName(void);
	/*130*/virtual const char	*GetEditorID(void);
	/*134*/virtual bool		SetEditorID(const char * edid);		// simply returns true at run-time

	enum
	{
		kFormFlag_IsMaster =			1,
		kFormFlag_IsAltered =			2,
		kFormFlag_Initialized =			8,
		kFormFlag_DroppedRef =			0x100,
		kFormFlag_CastShadows =			0x200,
		kFormFlag_QuestItem =			0x400,
		kFormFlag_IsRefPersistent =		0x400,
		kFormFlag_IsPermanent =			0x800,
		kFormFlag_DontSaveForm =		0x4000,
		kFormFlag_VisibleDistant =		0x8000,
		kFormFlag_HavokDeath =			0x10000,
		kFormFlag_NeedToChangeProcess =	0x20000,
		kFormFlag_Compressed =			0x40000,
		kFormFlag_CenterRefOnCreation =	0x100000,
		kFormFlag_StillLoading =		0x200000,
		kFormFlag_Destructible =		0x1000000,
		kFormFlag_IsVATSTargettable =	0x4000000,
		kFormFlag_DisableFade =			0x8000000,
		kFormFlag_TalkingActivator = 	0x40000000
	};

	enum
	{
		kModified_Flags =	0x00000001
		//	UInt32	flags;
	};

	UInt8		typeID;				// 04
	UInt8		jipFormFlags5;		// 05
	UInt16		jipFormFlags6;		// 06
	UInt32		flags;				// 08
	union
	{
		UInt32		refID;			// 0C
		struct
		{
			UInt8	id[3];
			UInt8	modIndex;
		};
	};

	tList<ModInfo>	mods;			// 10

	TESForm *GetBaseIfRef() const;

	bool IsCreated() const {return modIndex == 0xFF;}

	bool IsPlayer() const {return refID == 0x14;}

	TESFullName *GetFullName() const;
	const char *GetTheName() const;
	TESScriptableForm *GetScriptableForm() const;
	BGSDestructibleObjectForm *GetDestructibleForm() const;
	TESContainer *GetContainer() const;

	// adds a new form to the game (from CloneForm or LoadForm)
	void DoAddForm(TESForm* newForm, bool bPersist = true, bool record = true) const;
	// return a new base form which is the clone of this form
	TESForm *CloneForm(bool bPersist = true) const;

	bool HasScript() const;
	bool GetScriptAndEventList(Script **script, ScriptLocals **eventList) const;
	bool IsItemPlayable() const;
	UInt32 GetItemValue() const;
	UInt8 GetOverridingModIdx() const;
	const char *GetDescriptionText();
	const char *RefToString();
	const char *GetModelPath() const;
	void UnloadModel();
	TESLeveledList *GetLvlList() const;
	MagicItem *GetMagicItem() const;
	void SetJIPFlag(UInt16 jipFlag, bool bSet);

	__forceinline void MarkModified(UInt32 modFlag)
	{
		ThisCall(0x84A690, g_BGSSaveLoadGame, this, modFlag, 0);
	}

	__forceinline void MarkAsTemporary()
	{
		ThisCall(0x484490, this);
	}

	__forceinline static TESForm *CreateFormByType(FormType eType)
	{
		return CdeclCall<TESForm*>(0x465110, eType);
	}
};
static_assert(sizeof(TESForm) == 0x18);

union ColorRGBA
{
	struct
	{
		UInt8	r, g, b, a;
	};
	UInt32		rgba;
	
	inline void operator=(UInt32 _rgba) {rgba = _rgba;}
	inline operator UInt32() const {return rgba;}
	inline UInt8 operator[](UInt32 index) const {return (&r)[index];}
};

struct Condition
{
	enum ComparisonType
	{
		kComp_OR =			1,
		kComp_UseGlobal =	4,
		kComp_NEQ =			0x20,
		kComp_GT =			0x40,
		kComp_GE =			0x60,
		kComp_LT =			0x80,
		kComp_LE =			0xA0
	};

	enum RunOnType
	{
		kRunOn_Subject,
		kRunOn_Target,
		kRunOn_Reference,
		kRunOn_CombatTarget,
		kRunOn_LinkedReference
	};

	union Parameter
	{
		float		fltVal;
		SInt32		intVal;
		TESForm		*form;
	};

	UInt8			type;				// 00
	UInt8			pad01[3];			// 01
	union
	{
		float		value;
		TESGlobal	*global;
	}				comparisonValue;	// 04
	UInt32			opcode;				// 08
	Parameter		parameter1;			// 0C
	Parameter		parameter2;			// 10
	UInt32			runOnType;			// 14
	TESObjectREFR	*reference;			// 18

	__forceinline bool Evaluate(TESObjectREFR *runOnRef, TESForm *arg2, bool *result)
	{
		return ThisCall<bool>(0x681600, this, runOnRef, arg2, result);
	}
};

struct ConditionList : tList<Condition>
{
	__forceinline bool Evaluate(TESObjectREFR *runOnRef, TESForm *arg2, bool *result, bool arg4)
	{
		return ThisCall<bool>(0x680C60, this, runOnRef, arg2, result, arg4);
	}
};

class TESObject : public TESForm
{
public:
	/*138*/virtual UInt32	Unk_4E(void);
	/*13C*/virtual bool		Unk_4F(void);
	/*140*/virtual UInt32	Unk_50(void);
	/*144*/virtual bool		Unk_51(void);
	/*148*/virtual void		Unk_52(void * arg);
	/*14C*/virtual NiNode	*_CreateNiNode(TESObjectREFR *refr, bool arg1);
	/*150*/virtual void		Unk_54(void * arg);
	/*154*/virtual bool		IsInternal(void);
	/*158*/virtual bool		IsInternalMarker(void);
	/*15C*/virtual void		Unk_57(void);
	/*160*/virtual bool		Unk_58(void);	// BoundObject: Calls Unk_5F on the object model
	/*164*/virtual bool		Unk_59(void * arg);
	/*168*/virtual void		Unk_5A(void * arg0, void * arg1);
	/*16C*/virtual UInt32	Unk_5B(void);
	/*170*/virtual UInt32	Unk_5C(void);
	/*174*/virtual NiNode	*CreateNiNode(TESObjectREFR *refr);
};

// 30
class TESBoundObject : public TESObject
{
public:
	/*178*/virtual NiNode	*Create3DModel(TESObjectREFR *refr);	// calls Fn53, for NPC calls ReadBones, for LevelledActor level them if necessary
	/*17C*/virtual bool		Unk_5F(void);

	BoundObjectListHead		*head;		// 018
	TESBoundObject			*prev;		// 01C
	TESBoundObject			*next;		// 020
	SInt16					bounds[6];	// 024
};

// C
class TESFullName : public BaseFormComponent
{
public:
	String	name;		// 004
};

// 0C
class TESTexture : public BaseFormComponent
{
public:
	virtual UInt32	Unk_04(void);
	virtual void	GetNormalMap(String *str);
	virtual char	*GetPathRoot(void);

	String		ddsPath;	// 04
};

// 0C
class TESTexture1024 : public TESTexture
{
public:
};

// 00C
class TESIcon : public TESTexture
{
public:
	void SetPath(const char* newPath)	{ ddsPath.Set(newPath); }
};

// C
class TESScriptableForm : public BaseFormComponent
{
public:
	Script	* script;	// 004
	bool	resolved;	// 008	called during LoadForm, so scripts do not wait for TESForm_InitItem to be resolved
	UInt8	pad[3];		// 009
};

// 010
class BGSMessageIcon : public BaseFormComponent
{
public:
	TESIcon	icon;		// 004
};

// 008
class TESValueForm : public BaseFormComponent
{
public:
	enum
	{
		kModified_GoldValue =	0x00000008,
		// UInt32	value
	};

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	UInt32	value;
	// 008
};

// 10
class TESEnchantableForm : public BaseFormComponent
{
public:
	virtual UInt32	Unk_04(void);	// returns unk2

	EnchantmentItem* enchantItem;	// 04
	UInt16	enchantment;			// 08
	UInt16	unk1;					// 0A
	UInt32	unk2;					// 0C
	// 010
};

// 08
class TESImageSpaceModifiableForm : public BaseFormComponent
{
public:
	TESImageSpaceModifier	*imod;	// 04
};

// 008
class TESWeightForm : public BaseFormComponent
{
public:
	float	weight;		// 004
	// 008
};

// 008
class TESHealthForm : public BaseFormComponent
{
public:
	virtual UInt32	GetHealth(void);	// 0004

	UInt32	health;		// 004
};

// 008
class TESAttackDamageForm : public BaseFormComponent
{
public:
	virtual UInt16	GetDamage(void);

	UInt16	damage;	// 04
	UInt16	unk0;	// 06 - bitmask? perhaps 2 UInt8s?
	// 008
};

// 18
class TESModel : public BaseFormComponent
{
public:
	enum
	{
		kFacegenFlag_Head =			0x01,
		kFacegenFlag_Torso =		0x02,
		kFacegenFlag_RightHand =	0x04,
		kFacegenFlag_LeftHand =		0x08,
	};

	virtual void	*Destroy(bool doFree);	// 04
	virtual const char	*GetModelPath();
	virtual void	SetModelPath(const char *path);	// 06

	String	nifPath;		// 04
	UInt32	unk0C;			// 0C	referenced when saving Texture Hashes, init'd as a byte or is it a pointer to a structure starting with a byte followed by a pointer to some allocated data ?
	void	* unk10;		// 10
	UInt8	facegenFlags;	// 14
	UInt8	pad15[3];		// 15

	void SetPath(const char* newPath)	{ nifPath.Set(newPath); }
};

// 18
class BGSTextureModel : public TESModel
{
public:
};

// 020
class TESModelTextureSwap : public TESModel
{
public:
	struct Texture 
	{
		UInt32	textureID;			// 00
		UInt32	index3D;			// 04
		char	textureName[0x80];	// 08
	};	// there seem to be an array (length 6) post 0x88

	virtual void	*Unk_07(void);

	tList<Texture> textureList;	// 018
};

// 008
class BGSClipRoundsForm : public BaseFormComponent
{
public:
	UInt8	clipRounds;
	UInt8	padding[3];
	// 008
};

// 18
struct DestructionStage
{
	enum
	{
		kFlags_CapDamage =		1,
		kFlags_DisableObject =	2,
		kFlags_DestroyObject =	4,
	};

	UInt8				dmgStage;		// 00
	UInt8				healthPrc;		// 01
	UInt16				flags;			// 02
	UInt32				selfDmgSec;		// 04
	BGSExplosion		*explosion;		// 08
	BGSDebris			*debris;		// 0C
	UInt32				debrisCount;	// 10
	TESModelTextureSwap	*replacement;	// 14
};

// 14
struct DestructibleData
{
	UInt32				health;		// 00
	UInt8				stageCount;	// 04
	bool				targetable;	// 05
	UInt8				unk06[2];	// 06
	DestructionStage	**stages;	// 08
	UInt32				unk0C;		// 0C
	UInt32				unk10;		// 10
};

// 08
class BGSDestructibleObjectForm : public BaseFormComponent
{
public:
	DestructibleData	*data;			// 04
};

static_assert(sizeof(BGSDestructibleObjectForm) == 0x8);

// 00C
class BGSPickupPutdownSounds : public BaseFormComponent
{
public:
	TESSound	* pickupSound;		// 004
	TESSound	* putdownSound;		// 008
};

// 008
class BGSAmmoForm : public BaseFormComponent
{
public:
	TESForm* ammo; // 04	either TESAmmo or BGSListForm
};

// 008
class BGSRepairItemList : public BaseFormComponent
{
public:
	BGSListForm	* listForm;	// 04
};

// 008
class BGSEquipType : public BaseFormComponent
{
public:
	enum EquipTypes
	{
		kEqpType_BigGuns,
		kEqpType_EnergyWeapons,
		kEqpType_SmallGuns,
		kEqpType_MeleeWeapons,
		kEqpType_UnarmedWeapons,
		kEqpType_ThrowWeapons,
		kEqpType_Mine,
		kEqpType_BodyWear,
		kEqpType_HeadWear,
		kEqpType_HandWear,
		kEqpType_Chems,
		kEqpType_Stimpack,
		kEqpType_Food,
		kEqpType_Alcohol
	};

	UInt32	equipType;	// 08
};

// 004
class BGSPreloadable : public BaseFormComponent
{
public:
	virtual void	Fn_04(void); // pure virtual
};

// 008
class BGSBipedModelList : public BaseFormComponent
{
public:
	BGSListForm*    models;		// 004
	// 008
};

// 018
class TESModelRDT : public TESModel
{
public:
	virtual UInt32	Fn_07(void);
};

// DC
class TESBipedModelForm : public BaseFormComponent
{
public:
	// bit indices starting from lsb
	enum EPartBit
	{
		ePart_Head,
		ePart_Hair,
		ePart_UpperBody,
		ePart_LeftHand,
		ePart_RightHand,
		ePart_Weapon,
		ePart_PipBoy,
		ePart_Backpack,
		ePart_Necklace,
		ePart_Headband,
		ePart_Hat,
		ePart_Eyeglasses,
		ePart_Nosering,
		ePart_Earrings,
		ePart_Mask,
		ePart_Choker,
		ePart_MouthObject,
		ePart_BodyAddon1,
		ePart_BodyAddon2,
		ePart_BodyAddon3
	};

	enum EPartBitMask
	{
		ePartBitMask_Full = 0x7FFFF
	};

	enum ESlot
	{
		eSlot_Head =		1 << ePart_Head,
		eSlot_Hair =		1 << ePart_Hair,
		eSlot_UpperBody =	1 << ePart_UpperBody,
		eSlot_LeftHand =	1 << ePart_LeftHand,
		eSlot_RightHand =	1 << ePart_RightHand,
		eSlot_Weapon =		1 << ePart_Weapon,
		eSlot_PipBoy =		1 << ePart_PipBoy,
		eSlot_Backpack =	1 << ePart_Backpack,
		eSlot_Necklace =	1 << ePart_Necklace,
		eSlot_Headband =	1 << ePart_Headband,
		eSlot_Hat =			1 << ePart_Hat,
		eSlot_Eyeglasses =	1 << ePart_Eyeglasses,
		eSlot_Nosering =	1 << ePart_Nosering,
		eSlot_Earrings =	1 << ePart_Earrings,
		eSlot_Mask =		1 << ePart_Mask,
		eSlot_Choker =		1 << ePart_Choker,
		eSlot_MouthObject=	1 << ePart_MouthObject,
		eSlot_BodyAddon1 =	1 << ePart_BodyAddon1,
		eSlot_BodyAddon2 =	1 << ePart_BodyAddon2,
		eSlot_BodyAddon3 =	1 << ePart_BodyAddon3
	};

	enum EBipedFlags
	{
		eBipedFlag_HasBackPack	= 0x4,
		eBipedFlag_MediumArmor	= 0x8,
		eBipedFlag_PowerArmor	= 0x20,
		eBipedFlag_NonPlayable	= 0x40,
		eBipedFlag_HeavyArmor	= 0x80,
	};

	enum EBipedPath
	{
		ePath_Biped,
		ePath_Ground,
		ePath_Icon,
		ePath_Max
	};

	// missing part mask and flags
	UInt32					partMask;			// 004
	UInt32					bipedFlags;			// 008
	TESModelTextureSwap		bipedModel[2];		// 00C
	TESModelTextureSwap		groundModel[2];		// 04C
	TESIcon					icon[2];			// 08C
	BGSMessageIcon			messageIcon[2];		// 0A4
	TESModelRDT				modelRDT;			// 0C4

	static UInt32 MaskForSlot(UInt32 mask);

	bool IsPlayable() const {return !(bipedFlags & eBipedFlag_NonPlayable);}
	bool IsPowerArmor() const {return (bipedFlags & eBipedFlag_PowerArmor) != 0;}
	void SetPlayable(bool doset) {if (doset) bipedFlags &= ~eBipedFlag_NonPlayable; else bipedFlags |= eBipedFlag_NonPlayable;}
	void SetPowerArmor(bool bPA) {if (bPA) bipedFlags |= eBipedFlag_PowerArmor; else bipedFlags &= ~eBipedFlag_PowerArmor;}
};
static_assert(sizeof(TESBipedModelForm) == 0xDC);

// 30
class TESBoundAnimObject : public TESBoundObject
{
public:
};

// 0C
struct ContainerExtra
{
	union						// 00
	{
		TESFaction	*ownerFaction;
		TESNPC		*ownerNPC;
	};
	union						// 04
	{
		UInt32		requiredRank;
		TESGlobal	*globalVar;
	};
	float			health;		// 08
};

// 0C
class TESContainer : public BaseFormComponent
{
public:
	struct FormCount
	{
		SInt32				count;			//	00
		TESForm				*form;			//	04
		ContainerExtra		*contExtraData;	//	08
	};
	typedef tList<FormCount> FormCountList;

	FormCountList	formCountList;	// 04

	bool __fastcall ContainsForm(TESForm *form) const;
	SInt32 __fastcall GetCountForForm(TESForm *form) const;
};

// 00C
class BGSTouchSpellForm : public BaseFormComponent
{
public:
	TESForm	*unarmedEffect;	// 04
	UInt16	unarmedAnim;	// 08
	UInt16	pad0A;			// 0A
};

struct FactionListData
{
	TESFaction	*faction;
	SInt8		rank;
	UInt8		pad[3];
};

// 034
class TESActorBaseData : public BaseFormComponent
{
public:
	virtual void			Fn_04(TESForm * selectedForm);	// Called during form initialization after LoadForm and InitForm
	// flags access
	virtual bool			Fn_05(void);	// 00100000
	virtual bool			Fn_06(void);	// 00200000
	virtual bool			Fn_07(void);	// 10000000
	virtual bool			Fn_08(void);	// 20000000
	virtual bool			Fn_09(void);	// 80000000
	virtual bool			Fn_0A(void);	// 00400000
	virtual bool			Fn_0B(void);	// 00400000
	virtual bool			Fn_0C(void);	// 00800000
	virtual bool			Fn_0D(void);
	virtual bool			Fn_0E(void);
	virtual bool			Fn_0F(void);
	virtual bool			Fn_10(void);
	virtual bool			Fn_11(void);
	virtual bool			Fn_12(void);
	virtual void			Fn_13(void * arg);
	virtual bool			Fn_14(void);
	virtual void			Fn_15(void * arg);
	virtual UInt32			Fn_16(void);
	virtual void			Fn_17(void * arg);
	virtual UInt32			Fn_18(void);	// return unk08
	virtual float			Fn_19(void);	// return unk14
	virtual BGSVoiceType *	GetVoiceType(void);

	enum
	{
		kFlags_Female =					1 << 0,
		kFlags_Essential =				1 << 1,
		kFlags_HasCharGenFace =			1 << 2,
		kFlags_Respawn =				1 << 3,
		kFlags_AutoCalcStats =			1 << 4,
		//								1 << 5,
		//								1 << 6,
		kFlags_PCLevelMult =			1 << 7,
		kFlags_UseTemplate =			1 << 8,
		kFlags_NoLowLevelProcessing =	1 << 9,
		//								1 << 10,
		kFlags_NoBloodSpray =			1 << 11,
		kFlags_NoBloodDecal =			1 << 12,
		//								1 << 13,
		//								1 << 14,
		//								1 << 15,
		//								1 << 16,
		//								1 << 17,
		//								1 << 18,
		//								1 << 19,
		kFlags_NoVATSMelee =			1 << 20,
		//								1 << 21,
		kFlags_CanBeAllRaces =			1 << 22,
		kFlags_IsImmobile =				1 << 23,
		//								1 << 24,
		//								1 << 25,
		kFlags_NoKnockdowns =			1 << 26,
		kFlags_NotPushable =			1 << 27,
		//								1 << 28,
		//								1 << 29,
		kFlags_NoRotateToHeadTrack =	1 << 30,
		//								1 << 31,
	};

	enum TemplateFlags
	{
		kTemplFlag_UseTraits =		1 << 0,
		kTemplFlag_UseStats =		1 << 1,
		kTemplFlag_UseFactions =	1 << 2,
		kTemplFlag_UseEffectList =	1 << 3,
		kTemplFlag_UseAIData =		1 << 4,
		kTemplFlag_UseAIPackages =	1 << 5,
		kTemplFlag_UseModel =		1 << 6,
		kTemplFlag_UseBaseData =	1 << 7,
		kTemplFlag_UseInventory =	1 << 8,
		kTemplFlag_UseScript =		1 << 9,
	};

	UInt32			flags;				// 04	Comparing with LoadForm and FNVEdit
	UInt16			fatigue;			// 08	Fatique
	UInt16			barterGold;			// 0A	Barter Gold
	SInt16			level;				// 0C	Level/ Level Mult
	UInt16			calcMin;			// 0E	Calc min
	UInt16			calcMax;			// 10	Calc max
	UInt16			speedMultiplier;	// 12	Speed Multiplier (confirmed)
	float			karma;				// 14	Karma
	UInt16			dispositionBase;	// 18	Disposition Base
	UInt16			templateFlags;		// 1A	Template Flags
	TESForm			*deathItem;			// 1C	Death Item: object or FormList
	BGSVoiceType	*voiceType;			// 20
	TESForm			*templateActor;		// 24	Points toward Template
	UInt32			changedFlags;		// 28
	tList<FactionListData>	factionList;	// 2C

	char GetFactionRank(TESFaction* faction) const;
	void SetFactionRank(TESFaction* faction, char rank);

	bool IsFemale() {return (flags & kFlags_Female) != 0;}	// place holder until GECK 
};

// 14
class TESSpellList : public BaseFormComponent
{
public:
	enum
	{
		kModified_BaseSpellList =	0x00000020,
		// CHANGE_ACTOR_BASE_SPELLLIST
		//	UInt16	numSpells;
		//	UInt32	spells[numSpells];
	};

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	tList<SpellItem>	spellList;			// 004
	tList<SpellItem>	leveledSpellList;	// 00C

	UInt32	GetSpellCount() const {
		return spellList.Count();
	}

	// return the nth spell
	SpellItem* GetNthSpell(SInt32 whichSpell) const
	{
		return spellList.GetNthItem(whichSpell);
	}

	// removes all spells and returns how many spells were removed
	//UInt32 RemoveAllSpells();
};

// 020
class TESAIForm : public BaseFormComponent
{
public:
	typedef tList<TESPackage> PackageList;

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	UInt8	agression;				// 04
	UInt8	confidence;				// 05 
	UInt8	energyLevel;			// 06
	UInt8	responsibility;			// 07
	UInt8	mood;					// 08
	UInt8	pad09[3];				// 09

	UInt32	buySellsAndServices;	// 0C
	UInt8	teaches;				// 10
	UInt8	maximumTrainingLevel;	// 11
	UInt8	assistance;				// 12
	UInt8	aggroRadiusBehavior;	// 13
	SInt32	aggroRadius;			// 14

	PackageList	packageList;	// 18

	UInt32	GetPackageCount() const {
		return packageList.Count();
	}

	// return the nth package
	TESPackage* GetNthPackage(SInt32 anIndex) const
	{
		return packageList.GetNthItem(anIndex);
	}

	// replace the nth package
	TESPackage* SetNthPackage(TESPackage* pPackage, SInt32 anIndex)
	{
		return packageList.ReplaceNth(anIndex, pPackage);
	}

	// return the nth package
	SInt32 AddPackageAt(TESPackage* pPackage, SInt32 anIndex)
	{
		return packageList.Insert(pPackage, anIndex);
	}

	TESPackage* RemovePackageAt(SInt32 anIndex)
	{
		return packageList.RemoveNth(anIndex);
	}

	// removes all packages and returns how many were removed
	UInt32 RemoveAllPackages()
	{
		UInt32 cCount = GetPackageCount();
		packageList.RemoveAll();
		return cCount - GetPackageCount();
	}
};

// 00C
class TESAttributes : public BaseFormComponent
{
public:
	enum
	{
		kStrength = 0,
		kPerception,
		kEndurance,
		kCharisma,
		kIntelligence,
		kAgility,
		kLuck,
	};

	UInt8	attributes[7];	// 4
	UInt8	padB;			// B
};

// 00C
class TESAnimation : public BaseFormComponent
{
public:
	//UInt32	unk004;	// constructor and Fn_01 sugest this is a tList of char string.
	//UInt32	unk008;
	tList<char>	animNames;
	// 00C
};

class ActorValueOwner
{
public:
	virtual int		GetBaseActorValueInt(UInt32 avCode) const;
	virtual float	GetBaseActorValue(UInt32 avCode) const;
	virtual int		GetActorValueInt(UInt32 avCode) const;
	virtual float	GetActorValue(UInt32 avCode) const;
	virtual float	GetBaseSubAV(UInt32 avCode) const;
	virtual float	GetSubAVDamage(UInt32 avCode) const;
	virtual float	GetSubAVMod(UInt32 avCode) const;
	virtual int		GetPermanentActorValueInt(UInt32 avCode) const;
	virtual float	GetPermanentActorValue(UInt32 avCode) const;
	virtual Actor	*GetActor() const;
	virtual UInt16	GetLevel() const;

	__forceinline float GetThresholdedAV(UInt32 avCode)
	{
		return ThisCall<float>(0x66EF50, this, avCode);
	}
};

class CachedValuesOwner
{
public:
	virtual float	GetRadius(void); // computed from the BSBounds
	virtual float	GetWidthX(void);
	virtual float	GetWidthY(void);
	virtual float	GetHeight(void);
	virtual float	GetDPS(void);
	virtual float	GetMedicineSkillMult(void);
	virtual float	GetSurvivalSkillMult(void);
	virtual float	GetParalysis(void);
	virtual float	GetHealRate(void);
	virtual float	GetFatigueReturnRate(void); // calculated the same as HealRate
	virtual float	GetPerceptionCondition(void);
	virtual UInt32	GetEyeHeight(void);
	virtual UInt32	GetUnkShouldAttack(void);
	virtual float	GetAssistance(void);
	virtual float	GetWalkSpeedMult(void);
	virtual float	GetRunSpeedMult(void);
	virtual bool	GetHasNoCrippledLimbs(void);
};

// 10C
class TESActorBase : public TESBoundAnimObject
{
public:
	virtual BGSBodyPartData *	GetBodyPartData(void);
	virtual void				Fn_61(void * arg);
	virtual TESCombatStyle *	GetCombatStyle(void);	// Result saved as ZNAM GetCombatStyle
	virtual void				SetCombatStyle(TESCombatStyle * combatStyle);
	virtual void				SetAttr(UInt32 idx, float value);	// calls Fn65
	virtual void				SetAttr(UInt32 idx, UInt32 value);
	virtual void				ModActorValue(UInt32 actorValueCode, float value);
	virtual void				Fn_67(UInt32 arg0, UInt32 arg1);	// mod actor value?

	TESActorBaseData			baseData;		// 030
	TESContainer				container;		// 064
	BGSTouchSpellForm			touchSpell;		// 070
	TESSpellList				spellList;		// 07C
	TESAIForm					ai;				// 090
	TESHealthForm				health;			// 0B0
	TESAttributes				attributes;		// 0B8
	TESAnimation				animation;		// 0C4
	TESFullName					fullName;		// 0D0
	TESModel					model;			// 0DC
	TESScriptableForm			scriptable;		// 0F4
	ActorValueOwner				avOwner;		// 100
	BGSDestructibleObjectForm	destructible;	// 104
};

static_assert(sizeof(TESActorBase) == 0x10C);

// 14
class TESModelList : public BaseFormComponent
{
public:
	tList<char>		modelList;	// 04
	UInt32			count;		// 0C
	UInt32			unk10;		// 10

	bool ModelListAction(char *path, char action);
	void CopyFrom(TESModelList *source);
};

// 008
class TESDescription : public BaseFormComponent
{
public:
	virtual const char *Get(TESForm * overrideForm, UInt32 chunkID);

	UInt32	formDiskOffset;	// 4 - how does this work for descriptions in mods?
	// maybe extracts the mod ID then uses that to find the src file?
};

// 10
class TESReactionForm : public BaseFormComponent
{
public:
	struct Reaction
	{
		enum
		{
			kNeutral = 0,
			kEnemy,
			kAlly,
			kFriend
		};

		TESFaction	* faction;
		SInt32		modifier;
		UInt32		reaction;
	};

	tList <Reaction>	reactions;	// 4
	UInt8	unkC;		// C
	UInt8	padD[3];	// D
};

// 08
class TESRaceForm : public BaseFormComponent
{
public:
	TESRace	* race;	// 04
};

// 0C
class TESSoundFile : public BaseFormComponent
{
public:
	virtual void	Set(const char * str);

	String			path;	// 04
};

// 24
class BGSQuestObjective
{
public:
	virtual void *Destroy(bool noDealloc);

	enum
	{
		eQObjStatus_displayed = 1,
		eQObjStatus_completed = 2,
	};

	struct TargetData 
	{
		TESObjectREFR*	target;
		UInt8			flags;
		UInt8			filler[3];
	};

	struct ParentSpaceNode {};

	struct TeleportLink
	{
		TESObjectREFR*	door;
		UInt32			unk04[3];
	};

	struct Target
	{
		struct Data
		{
			BSSimpleArray<ParentSpaceNode>	parentSpaceNodes;	// 00
			BSSimpleArray<TeleportLink>		teleportLinks;		// 10
			UInt32							unk20[6];			// 20
		};

		UInt8							byte00;			// 00
		UInt8							pad01[3];		// 01
		ConditionList					conditions;		// 04
		TESObjectREFR					*target;		// 0C
		Data							data;			// 10
	};

	UInt32			objectiveId;	// 004 Objective Index in the GECK
	String			displayText;	// 008
	TESQuest		*quest;			// 010
	tList<Target>	targets;		// 014
	UInt32			unk01C;			// 01C
	UInt32			status;			// 020	bit0 = displayed, bit 1 = completed. 1 and 3 significant. If setting it to 3, quest flags bit1 will be set also.
};

typedef BGSQuestObjective::Target ObjectiveTarget;
typedef BGSQuestObjective::Target::Data ObjectiveTargetData;

class ObjTargetFinder
{
	TESObjectREFR	*m_target;

public:
	ObjTargetFinder(TESObjectREFR *_target) : m_target(_target) {}

	bool operator==(ObjectiveTarget *objTarget) const {return objTarget->target == m_target;}
};

class BGSOpenCloseForm
{
public:
	virtual void	Unk_00(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_01(UInt32 arg0, UInt32 arg1);
	virtual bool	Unk_02(void);
};

/**** forms ****/

class TESModelAnim : public TESModel
{
public:
};	// 018

// 54
class TESIdleForm : public TESForm
{
public:
	enum {
		eIFgf_groupIdle = 0,
		eIFgf_groupMovement = 1,
		eIFgf_groupLeftArm = 2,
		eIFgf_groupLeftHand = 3,
		eIFgf_groupLeftWeapon = 4,
		eIFgf_groupLeftWeaponUp = 5,
		eIFgf_groupLeftWeaponDown = 6,
		eIFgf_groupSpecialIdle = 7,
		eIFgf_groupWholeBody = 20,
		eIFgf_groupUpperBody = 20,

		eIFgf_flagOptionallyReturnsAFile = 128,
		eIFgf_flagUnknown = 64,
	};

	struct Data
	{
		UInt8			groupFlags;		// 000	animation group and other flags
		UInt8			loopMin;		// 001
		UInt8			loopMax;		// 002
		UInt8			fil03B;			// 003
		UInt16			replayDelay;	// 004
		UInt8			flags;			// 006	bit0 is No attacking
		UInt8			fil03F;			// 007
	};

	TESModelAnim				anim;			// 018
	ConditionList				conditions;		// 030
	Data						data;			// 038
	BSSimpleArray<TESIdleForm*>	*children;		// 040	NiFormArray, contains all idle anims in path if eIFgf_flagUnknown is set
	TESIdleForm					*parent;		// 044
	TESIdleForm					*previous;		// 048
	String						editorID;		// 04C

	TESIdleForm *FindIdle(Actor *animActor);
};

struct TESTopicInfoResponse
{
	struct Data
	{
		UInt32	emotionType;	//	00
		UInt32	emotionValue;	//	04	Init'd to 0x32
		UInt32	unused;			//	08
		UInt8	responseNumber;	//	0C
		UInt8	pad00D[3];		
		UInt32	sound;			//	10
		UInt8	flags;			//	14	Init'd to 1
		UInt8	pad015[3];
	};

	Data			data;					//	000
	String			responseText;			//	018
	TESIdleForm	*	spkeakerAnimation;		//	020
	TESIdleForm	*	listenerAnimation;		//	024
	TESTopicInfoResponse	* next;			//	028
};

// 50
class TESTopicInfo : public TESForm
{
public:
	struct RelatedTopics
	{
		tList<TESTopic>		linkFrom;
		tList<TESTopic>		choices;
		tList<TESTopic>		unknown;
	};

	ConditionList		conditions;			// 18
	UInt16				unk20;				// 20
	UInt8				saidOnce;			// 22
	UInt8				type;				// 23
	UInt8				nextSpeaker;		// 24
	UInt8				flags1;				// 25
	UInt8				flags2;				// 26
	UInt8				pad27;				// 27
	String				prompt;				// 28
	tList<TESTopic>		addTopics;			// 30
	RelatedTopics		*relatedTopics;		// 38
	UInt32				speaker;			// 3C
	UInt32				actorValueOrPerk;	// 40
	UInt32				speechChallenge;	// 44
	TESQuest			*quest;				// 48
	UInt32				modInfoFileOffset;	// 4C	during LoadForm
	TESTopic			*parentTopic;		// 50	JIP only!

	__forceinline Script *GetResultScript(UInt32 onEnd)
	{
		return ThisCall<Script*>(0x61EB60, this, onEnd);
	}
	__forceinline void RunResultScript(UInt32 onEnd, Actor *actor)
	{
		ThisCall(0x61F170, this, onEnd, actor);
	}

	size_t DecompileResultScripts(FILE *pStream, char *pBuffer);
};

typedef NiTLargeArray<TESTopicInfo*> TopicInfoArray;
typedef void* INFO_LINK_ELEMENT;

// 48
class TESTopic : public TESForm
{
public:
	enum TopicType
	{
		kTopicType_Topic,
		kTopicType_Conversation,
		kTopicType_Combat,
		kTopicType_Persuasion,
		kTopicType_Detection,
		kTopicType_Service,
		kTopicType_Miscellaneous,
		kTopicType_Radio
	};

	struct Info	//	34
	{
		TESQuest		*quest;		//	00
		TopicInfoArray	infoArray;	//	04
		BSSimpleArray<INFO_LINK_ELEMENT>		unk01C;
		TESQuest		*quest2;	//	2C
		UInt8			unk030;
		UInt8			pad031[3];
	};

	TESFullName		fullName;		// 18
	UInt8			topicType;		// 24
	UInt8			topicFlags;		// 25
	UInt8			pad26[2];		// 26
	float			priority;		// 28
	tList<Info>		infos;			// 2C
	String			lowIntDialog;	// 34
	UInt16			unk3C;			// 3C
	UInt16			unk3E;			// 3E
	String			editorIDstr;	// 40
};

// 24
struct DecalInfo
{
	enum
	{
		kFlag_Parallax =	1,
		kFlag_AlphaBlend =	2,
		kFlag_AlphaTest =	4
	};

	float		minWidth;		// 00
	float		maxWidth;		// 04
	float		minHeight;		// 08
	float		maxHeight;		// 0C
	float		depth;			// 10
	float		shininess;		// 14
	float		parallaxScale;	// 18
	UInt8		parallaxPasses;	// 1C
	UInt8		flags;			// 1D
	UInt8		pad1E[2];		// 1E
	ColorRGBA	color;			// 20
};

// A0
class BGSTextureSet : public TESBoundObject
{
public:
	enum	// texture types
	{
		kDiffuse = 0,
		kNormal,
		kEnvMask,
		kGlow,
		kParallax,
		kEnv
	};

	enum
	{
		kTexFlag_NoSpecMap = 0x0001,
	};

	BSTextureSet	bsTexSet;		// 30

	TESTexture		textures[6];	// 38
	DecalInfo		*decalInfo;		// 80
	UInt16			texFlags;		// 84
	UInt8			pad86[2];		// 86
	UInt32			unk88;			// 88
	UInt32			unk8C;			// 8C
	UInt32			unk90;			// 90
	UInt32			unk94;			// 94
	UInt32			unk98;			// 98
	UInt32			unk9C;			// 9C
};

static_assert(sizeof(BGSTextureSet) == 0xA0);

// 24
class BGSMenuIcon : public TESForm
{
public:
	TESIcon	icon;	// 18
};

static_assert(sizeof(BGSMenuIcon) == 0x24);

// 28
class TESGlobal : public TESForm
{
public:
	enum
	{
		kType_Float =		'f',
		kType_Long =		'l',
		kType_RefID =		'r',
		kType_Short =		's'
	};

	String			name;		// 18	(EDID)
	UInt8			type;		// 20
	UInt8			pad21[3];	// 21
	union
	{
		float		data;
		UInt32		uRefID;
	};
};

static_assert(sizeof(TESGlobal) == 0x28);

// 60
class TESClass : public TESForm
{
public:
	enum
	{
		kFlag_Playable =	0x00000001,
		kFlag_Guard =		0x00000002,
	};

	enum
	{
		kService_Weapons =		0x00000001,
		kService_Armor =		0x00000002,
		kService_Clothing =		0x00000004,
		kService_Books =		0x00000008,
		kService_Food =			0x00000010,
		kService_Chems =		0x00000020,
		kService_Stimpacks =	0x00000040,
		kService_Lights =		0x00000080,	// ??
		kService_Misc =			0x00000400,
		kService_Potions =		0x00002000,	// probably deprecated
		kService_Training =		0x00004000,
		kService_Recharge =		0x00010000,
		kService_Repair =		0x00020000,
	};

	TESFullName		fullName;		// 18
	TESDescription	description;	// 24
	TESTexture		texture;		// 2C
	TESAttributes	attributes;		// 38

	// corresponds to DATA chunk
	UInt32			tagSkills[4];	// 44
	UInt32			classFlags;		// 54
	UInt32			services;		// 58
	UInt8			teaches;		// 5C
	UInt8			trainingLevel;	// 5D
	UInt8			pad5E[2];		// 5E
};

static_assert(sizeof(TESClass) == 0x60);

// 50
class TESReputation : public TESForm
{
public:
	TESFullName		fullName;			// 18
	TESIcon			icon;				// 24
	BGSMessageIcon	msgIcon;			// 30

	float			maxRep;				// 40
	float			positiveRep;		// 44
	float			negativeRep;		// 48
	UInt32			lastChangePositive;	// 4C
};
static_assert(sizeof(TESReputation) == 0x50);

// 4C
class TESFaction : public TESForm
{
public:
	enum
	{
		// TESForm flags

		// TESReactionForm flags

		kModified_FactionFlags =	0x00000004
		// CHANGE_FACTION_FLAGS
		// UInt8	flags;
	};

	enum
	{
		kFlag_HiddenFromPC =	0x00000001,
		kFlag_Evil =			0x00000002,
		kFlag_SpecialCombat =	0x00000004,

		kFlag_TrackCrime =		0x00000100,
		kFlag_AllowSell =		0x00000200,
	};

	// 1C
	struct Rank
	{
		String		name;		// 00
		String		femaleName;	// 08
		TESTexture	insignia;	// 10 - effectively unused, can be set but there is no faction UI
	};

	TESFullName		fullName;	// 18
	TESReactionForm	reaction;	// 24

	UInt32			factionFlags;	// 34
	TESReputation	*reputation;	// 38
	tList<Rank>		ranks;			// 3C
	UInt32			numMajorCrimes;	// 44
	UInt32			numMinorCrimes;	// 48

	bool IsFlagSet(UInt32 flag)
	{
		return (factionFlags & flag) != 0;
	}
	void SetFlag(UInt32 pFlag, bool bEnable)
	{
		if (bEnable) factionFlags |= pFlag;
		else factionFlags &= ~pFlag;
		MarkModified(kModified_FactionFlags);
	}
	bool IsHidden()
	{	return IsFlagSet(kFlag_HiddenFromPC);	}
	bool IsEvil()
	{	return IsFlagSet(kFlag_Evil);	}
	bool HasSpecialCombat()
	{	return IsFlagSet(kFlag_SpecialCombat);	}
	void SetHidden(bool bHidden)
	{	SetFlag(kFlag_HiddenFromPC, bHidden);	}
	void SetEvil(bool bEvil)
	{	SetFlag(kFlag_Evil, bEvil);	}
	void SetSpecialCombat(bool bSpec)
	{	SetFlag(kFlag_SpecialCombat, bSpec);	}
};

static_assert(sizeof(TESFaction) == 0x4C);

// 50
class BGSHeadPart : public TESForm
{
public:
	enum
	{
		kFlag_Playable =	0x01,
	};

	TESFullName			fullName;	// 18
	TESModelTextureSwap	texSwap;	// 24

	UInt8				headFlags;	// 44
	UInt8				pad45[3];	// 45
	UInt32				unk48;		// 48
	UInt32				unk4C;		// 4C
};

static_assert(sizeof(BGSHeadPart) == 0x50);

// 4C
class TESHair : public TESForm
{
public:
	enum
	{
		kFlag_Playable =	0x01,
		kFlag_NotMale =		0x02,
		kFlag_NotFemale =	0x04,
		kFlag_Fixed =		0x08,
	};

	TESFullName		fullName;	// 18
	TESModel		model;		// 24
	TESTexture		texture;	// 3C

	UInt8			hairFlags;	// 48	Playable, not Male, not Female, Fixed
	UInt8			pad49[3];	// 49

	bool IsPlayable() {return (hairFlags & kFlag_Playable) == kFlag_Playable;}
	void SetPlayable(bool doset) {if (doset) hairFlags |= kFlag_Playable; else hairFlags &= ~kFlag_Playable;}
};

static_assert(sizeof(TESHair) == 0x4C);

// 34
class TESEyes : public TESForm
{
public:
	enum
	{
		kFlag_Playable =	0x01,
		kFlag_NotMale =		0x02,
		kFlag_NotFemale =	0x04,
	};

	TESFullName		fullName;	// 18
	TESTexture		texture;	// 24

	UInt8			eyeFlags;	// 30
	UInt8			pad31[3];	// 31

	bool IsPlayable() {return (eyeFlags & kFlag_Playable) == kFlag_Playable;}
	void SetPlayable(bool doset) {if (doset) eyeFlags |= kFlag_Playable; else eyeFlags &= ~kFlag_Playable;}
};

static_assert(sizeof(TESEyes) == 0x34);

// 524
class TESRace : public TESForm
{
public:
	// 18
	struct FaceGenData
	{
		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
		UInt32	unk0C;
		UInt32	unk10;
		UInt32	unk14;
	};

	// 2
	struct SkillMod
	{
		UInt8	actorValue;
		char	mod;
	};

	enum
	{
		kFlag_Playable =	0x00000001,
		kFlag_Child =		0x00000004,
	};

	TESFullName		fullName;				// 018
	TESDescription	desc;					// 024
	TESSpellList	spells;					// 02C
	TESReactionForm	reaction;				// 040

	SkillMod		skillMods[7];			// 050
	UInt8			pad05E[2];				// 05E
	float			height[2];				// 060 male/female
	float			weight[2];				// 068 male/female
	UInt32			raceFlags;				// 070

	TESAttributes	baseAttributes[2];		// 074 male/female
	tList<TESHair>	hairs;					// 08C
	TESHair *		defaultHair[2];			// 094 male/female
	UInt8			defaultHairColor[2];	// 09C male/female
	UInt8			fill09E[2];				// 09E

	UInt32			unk0A0[(0xA8 - 0xA0) >> 2];	// 0A0

	tList<TESEyes>	eyes;					// 0A8

	TESModel		faceModels[2][8];			// 0B0	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture		faceTextures[2][8];			// 230	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture		bodyPartsTextures[2][3];	// 2F0	male/female	UpperBody, LeftHand, RightHand
	TESModel		bodyModels[2][3];			// 338	male/female	UpperBody, LeftHand, RightHand
	BGSTextureModel	bodyTextures[2];			// 3C8	male/female	EGT file, not DDS.
	FaceGenData		unk3F8[2][4];				// 3F8  male/female

	UInt32			unk4B8[(0x4CC - 0x4B8) >> 2]; // 4B8

	String				name;				// 4CC
	NiTArray <void *>	faceGenUndo;		// 4D4 - NiTPrimitiveArray<FaceGenUndo *>
	UInt32				unk4E4[6];			// 4E4
	BGSVoiceType*		voiceTypes[2];		// 4FC // VTCK male/female
	TESRace*			ageRace[2];			// 504 // ONAM/YNAM
	String				editorID;			// 50C
	UInt32				unk514[4];			// 514

	bool IsPlayable() const {return (raceFlags & kFlag_Playable) == kFlag_Playable;}
	void SetPlayable(bool doset) {if (doset) raceFlags |= kFlag_Playable; else raceFlags &= ~kFlag_Playable;}
};
static_assert(sizeof(TESRace) == 0x524);

// 68
class TESSound : public TESBoundAnimObject
{
public:
	enum
	{
		kFlag_RandomFrequencyShift =	1,
		kFlag_PlayAtRandom =			2,
		kFlag_EnvironmentIgnored =		4,
		kFlag_RandomLocation =			8,
		kFlag_Loop =					0x10,
		kFlag_MenuSound =				0x20,
		kFlag_2D =						0x40,
		kFlag_360LFE =					0x80,
		kFlag_DialogueSound =			0x100,
		kFlag_EnvelopeFast =			0x200,
		kFlag_EnvelopeSlow =			0x400,
		kFlag_2DRadius =				0x800,
		kFlag_MuteWhenSubmerged =		0x1000,
		kFlag_StartAtRandomPosition =	0x2000,
	};

	TESSoundFile	soundFile;				// 30

	String			editorID;				// 3C
	UInt8			minAttenuationDist;		// 44
	UInt8			maxAttenuationDist;		// 45
	SInt8			frequencyAdj;			// 46
	UInt8			byte47;					// 47
	UInt32			soundFlags;				// 48
	UInt16			staticAttenuation;		// 4C
	UInt8			endsAt;					// 4E
	UInt8			startsAt;				// 4F
	UInt16			attenuationCurve[5];	// 50
	UInt16			reverbAttenuation;		// 5A
	UInt32			priority;				// 5C
	UInt32			unk60;					// 60
	UInt32			unk64;					// 64

	void SetFlag(UInt32 pFlag, bool bEnable)
	{
		if (bEnable) soundFlags |= pFlag;
		else soundFlags &= ~pFlag;
	}
};
static_assert(sizeof(TESSound) == 0x68);

// 54
class BGSAcousticSpace : public TESBoundObject
{
public:
	enum EnvironmentTypes
	{
		kNone,
		kDefault,
		kGeneric,
		kPaddedCell,
		kRoom,
		kBathroom,
		kLivingroom,
		kStoneRoom,
		kAuditorium,
		kConcerthall,
		kCave,
		kArena,
		kHangar,
		kCarpetedHallway,
		kHallway,
		kStoneCorridor,
		kAlley,
		kForest,
		kCity,
		kMountains,
		kQuarry,
		kPlain,
		kParkinglot,
		kSewerpipe,
		kUnderwater,
		kSmallRoom,
		kMediumRoom,
		kLargeRoom,
		kMediumHall,
		kLargeHall,
		kPlate
	};

	UInt8		isInterior;			// 30
	UInt8		pad31[3];			// 31
	TESSound	*dawnSound;			// 34
	TESSound	*noonSound;			// 38
	TESSound	*duskSound;			// 3C
	TESSound	*nightSound;		// 40
	TESSound	*wallaSound;		// 44
	TESRegion	*region;			// 48
	UInt32		environmentType;	// 4C
	UInt32		wallaTriggerCount;	// 50
};
static_assert(sizeof(BGSAcousticSpace) == 0x54);

// 60
class TESSkill : public TESForm
{
public:
	TESDescription	description;	// 18
	TESTexture		texture;		// 20

	UInt32			unk2C;			// 2C
	UInt32			unk30;			// 30
	UInt32			unk34;			// 34
	float			unk38;			// 38
	float			unk3C;			// 3C
	TESDescription	desc2[3];		// 40
	UInt32			unk58[(0x60 - 0x58) >> 2];	// 58
};
static_assert(sizeof(TESSkill) == 0x60);

// B0
class EffectSetting : public TESForm
{
public:
	enum
	{
		kArchType_ValueModifier = 0,
		kArchType_Script,
		kArchType_Dispel,
		kArchType_CureDisease,
		kArchType_Absorb,
		kArchType_Shield,
		kArchType_Calm,
		kArchType_Demoralize,
		kArchType_Frenzy,
		kArchType_CommandCreature,
		kArchType_CommandHumanoid,
		kArchType_Invisibility,
		kArchType_Chameleon,
		kArchType_Light,
		kArchType_Darkness,
		kArchType_NightEye,
		kArchType_Lock,
		kArchType_Open,
		kArchType_BoundItem,
		kArchType_SummonCreature,
		kArchType_DetectLife,
		kArchType_Telekinesis,
		kArchType_DisintigrateArmor,
		kArchType_DisinitgrateWeapon,
		kArchType_Paralysis,
		kArchType_Reanimate,
		kArchType_SoulTrap,
		kArchType_TurnUndead,
		kArchType_SunDamage,
		kArchType_Vampirism,
		kArchType_CureParalysis,
		kArchType_CureAddiction,
		kArchType_CurePoison,
		kArchType_Concussion,
		kArchType_ValueAndParts,
		kArchType_LimbCondition,
		kArchType_Turbo,
	};

	enum EffectSettingFlags
	{
		kEffFlag_Hostile =			1,
		kEffFlag_Recover =			2,
		kEffFlag_Detrimental =		4,
		kEffFlag_Hidden =			8,
		kEffFlag_Self =				0x10,
		kEffFlag_Touch =			0x20,
		kEffFlag_Target =			0x40,
		kEffFlag_NoDuration =		0x80,
		kEffFlag_NoMagnitude =		0x100,
		kEffFlag_NoArea =			0x200,
		kEffFlag_FXPersist =		0x400,
		kEffFlag_GoryVisuals =		0x1000,
		kEffFlag_DisplayNameOnly =	0x2000,
		kEffFlag_UseSkill =			0x80000,
		kEffFlag_UseAttribute =		0x100000,
		kEffFlag_Painless =			0x1000000,
		kEffFlag_NoHitEffect =		0x8000000,
		kEffFlag_NoDeathDispel =	0x10000000
	};

	TESModel		model;				// 18
	TESDescription	description;		// 30
	TESFullName		fullName;			// 38
	TESIcon			icon;				// 44
	UInt32			unk50;				// 50
	UInt32			unk54;				// 54
	UInt32			effectFlags;		// 58
	float			baseCost;			// 5C	Unused
	TESForm			*associatedItem;	// 60	Script* for ScriptEffects
	UInt32			magicSchool;		// 64	Unused
	UInt32			resistType;			// 68	actor value for resistance
	UInt16			counterEffectCount;	// 6C
	UInt8			pad6E[2];			// 6E
	TESObjectLIGH	*light;				// 70
	float			projectileSpeed;	// 74
	TESEffectShader	*effectShader;		// 78
	TESEffectShader	*enchantShader;		// 7C
	TESSound		*castSound;			// 80
	TESSound		*boltSound;			// 84
	TESSound		*hitSound;			// 88
	TESSound		*areaSound;			// 8C
	float			flt90;				// 90	Unused; fMagicDefaultCEEnchantFactor
	float			flt94;				// 94	Unused; fMagicDefaultCEBarterFactor
	UInt8			archtype;			// 98
	UInt8			pad99[3];			// 99
	UInt8			actorVal;			// 9C
	UInt8			pad9D[3];			// 9D
	UInt32			unkA0;				// A0
	UInt32			unkA4;				// A4
	UInt32			unkA8;				// A8
	UInt32			unkAC;				// AC
};
static_assert(sizeof(EffectSetting) == 0xB0);

// 24
class EffectItem
{
public:
	enum
	{
		kRange_Self = 0,
		kRange_Touch,
		kRange_Target,
	};

	UInt32				magnitude;			// 00	used as a float
	UInt32				area;				// 04
	UInt32				duration;			// 08
	UInt32				range;				// 0C
	UInt32				actorValueOrOther;	// 10
	EffectSetting		*setting;			// 14
	float				cost;				// 18 on autocalc items this seems to be the cost
	ConditionList		conditions;			// 1C
};

// 10
class EffectItemList : public BSSimpleList<EffectItem>
{
public:
	UInt32		unk0C;			// 0C

	bool RemoveNthEffect(UInt32 index);
};
static_assert(sizeof(EffectItemList) == 0x10);

// 1C
class MagicItem : public TESFullName
{
public:
	/*010*/virtual bool		Unk_04();
	/*014*/virtual void		Unk_05(bool doSet);
	/*018*/virtual UInt32	GetType();
	/*01C*/virtual bool		Unk_07();
	/*020*/virtual bool		Unk_08();
	/*024*/virtual UInt32	GetTypeSignature();
	/*028*/virtual void		*Unk_0A();
	/*02C*/virtual UInt32	Unk_0B();
	/*030*/virtual bool		GetNotEqual(TESForm *magicForm);
	/*034*/virtual void		CopyFrom(TESForm *magicForm);
	/*038*/virtual void		Unk_0E();
	/*03C*/virtual void		Unk_0F();
	/*040*/virtual void		Unk_10(UInt32 arg1, UInt32 arg2);

	EffectItemList	list;	// 0C

	enum MagicItemType
	{
		//	SpellItem
		kActorEffect =	0,
		kDisease =		1,
		kPower =		2,
		kLesserPower =	3,
		kAbility =		4,
		kPoison =		5,
		kAddiction =	10,

		//	EnchantmentItem
		kObjectEffect =	6,

		//	AlchemyItem
		kIngestible =	7,

		//	IngredientItem
		kIngredient =	8
	};

	void __fastcall UpdateEffectsAllActors(EffectItem *effItem, bool addNew = false);
};
static_assert(sizeof(MagicItem) == 0x1C);

// 34
class MagicItemForm : public TESForm
{
public:
	virtual void	ByteSwap();

	MagicItem	magicItem;	// 18
};
static_assert(sizeof(MagicItemForm) == 0x34);

// 44
class EnchantmentItem : public MagicItemForm
{
public:
	enum
	{
		kType_Weapon = 2,
		kType_Apparel,
	};

	UInt32		type;		// 34
	UInt32		unk38;		// 38
	UInt32		unk3C;		// 3C
	UInt8		enchFlags;	// 40
	UInt8		pad41[3];	// 41
};
static_assert(sizeof(EnchantmentItem) == 0x44);

// 44
class SpellItem : public MagicItemForm
{
public:
	enum Types
	{
		kType_ActorEffect	= 0,
		kType_Disease,
		kType_Power,
		kType_LesserPower,
		kType_Ability,
		kType_Poison,
		kType_Addiction		= 10,
	};

	enum Flags
	{
		kFlag_NoAutoCalc =					1,		// see 0x40E600, 0x40E577, 0x40E1EF, 0x4094B1
		kFlag_Unk2 =						2,		// see 0x823C2C
		kFlag_PCStartEffect =				4,		// see 0x8C2E3C, 0x7294A3, 0x7293D8, 0x708B94
		kFlag_AreaEffectIgnoresLOS =		0x10,	// see 0x818DD0
		kFlag_ScriptEffectAlwaysApplies =	0x20,	// see 0x40E65F (AlwaysApplyNotSet?)
		kFlag_DisallowAbsorbOrReflect =		0x40,	// see 0x40E67F
		kFlag_ForceTouchExplode =			0x80	// see 0x8169A8, 0x816173, 0x816849, 0x8169A8
	};

	UInt32		type;		// 34
	UInt32		unk38;		// 38
	UInt32		unk3C;		// 3C
	UInt8		spellFlags;	// 40
	UInt8		pad41[3];	// 41
};
static_assert(sizeof(SpellItem) == 0x44);

// 4C
class MagicItemObject : public TESBoundObject
{
public:
	MagicItem		magicItem;		// 30
};

// D8
class AlchemyItem : public MagicItemObject
{
public:
	TESModelTextureSwap			model;					// 4C
	TESIcon						icon;					// 6C
	BGSMessageIcon				messageIcon;			// 78
	TESScriptableForm			scriptable;				// 88
	TESWeightForm				weight;					// 94
	BGSEquipType				equipType;				// 9C
	BGSDestructibleObjectForm	destructible;			// A4
	BGSPickupPutdownSounds		pickupPutdownsounds;	// AC

	UInt32						value;					// B8
	UInt8						alchFlags;				// BC
	UInt8						padBD[3];				// BD
	SpellItem					*withdrawalEffect;		// C0
	float						addictionChance;		// C4
	TESSound					*consumeSound;			// C8
	TESIcon						iconCC;					// CC

	bool IsPoison() const;
};
static_assert(sizeof(AlchemyItem) == 0xD8);

// 68
class TESGrass : public TESBoundObject
{
public:
	TESModel		model;					// 30

	UInt8			density;				// 48
	UInt8			minSlope;				// 49
	UInt8			maxSlope;				// 4A
	UInt8			pad4B;					// 4B
	UInt16			unitFromWaterAmount;	// 4C
	UInt8			pad4E[2];				// 4E
	UInt8			unitFromWaterType;		// 50
	UInt8			pad51[3];				// 51
	float			positionRange;			// 54
	float			heightRange;			// 58
	float			colorRange;				// 5C
	float			wavePeriod;				// 60
	UInt8			grassFlags;				// 64
	UInt8			pad65[3];				// 65
};

static_assert(sizeof(TESGrass) == 0x68);

// 28
class TESLandTexture : public TESForm
{
public:
	BGSTextureSet	*textureSet;		// 18
	UInt8			materialType;		// 1C
	UInt8			friction;			// 1D
	UInt8			restitution;		// 1E
	UInt8			specularExponent;	// 1F
	tList<TESGrass>	grasses;			// 20
};
static_assert(sizeof(TESLandTexture) == 0x28);

// 90
class TESObjectACTI : public TESBoundAnimObject
{
public:
	TESFullName					fullName;			// 30
	TESModelTextureSwap			modelTextureSwap;	// 3C
	TESScriptableForm			scriptable;			// 5C
	BGSDestructibleObjectForm	destructible;		// 68
	BGSOpenCloseForm			openClose;			// 70

	TESSound					*loopingSound;		// 74
	TESSound					*activationSound;	// 78
	TESSound					*radioTemplate;		// 7C
	TESWaterForm				*waterType;			// 80
	BGSTalkingActivator			*radioStation;		// 84
	String						activationPrompt;	// 88
};

static_assert(sizeof(TESObjectACTI) == 0x90);

// 98
class BGSTalkingActivator : public TESObjectACTI
{
public:
	Actor				*talkingActor;	// 90
	BGSVoiceType		*voiceType;		// 94
};
static_assert(sizeof(BGSTalkingActivator) == 0x98);

struct TerminalEntry;

// BGSTerminal (9C)
class BGSTerminal : public TESObjectACTI
{
public:
	enum
	{
		kTerminalFlagLeveled = 1 << 0,
		kTerminalFlagUnlocked = 1 << 1,
		kTerminalFlagAltColors = 1 << 2,
		kTerminalFlagHideWelcome = 1 << 3,
	};

	enum
	{
		kEntryFlagAddNote = 1 << 0,
		kEntryFlagForceRedraw = 1 << 1,
	};

	struct TermData
	{
		UInt8 difficulty;       // 0: very easy, 1: easy, 2: average, 3: hard, 4: very hard, 5: requires key
		UInt8 terminalFlags;
		UInt8 type;             // 0-9, corresponds to GECK types 1-10
	};

	String					desc;			// 90	DESC
	tList<TerminalEntry>	menuEntries;	// 98
	BGSNote					*password;		// A0	PNAM
	TermData				data;			// A4	DNAM

	size_t DecompileResultScripts(FILE *pStream, char *pBuffer);
};

// 98
class TESFurniture : public TESObjectACTI
{
public:
	UInt32			unk90[2];	// 90
};
static_assert(sizeof(TESFurniture) == 0x98);

// 190
class TESObjectARMO : public TESBoundObject
{
public:
	struct MovementSound
	{
		TESSound		*sound;
		UInt8			unk04[3];
		UInt8			chance;
		UInt32			type;
		//				0x11	Walk
		//				0x12	Sneak
		//				0x13	Run
		//				0x14	Sneak (Armor)
		//				0x15	Run (Armor)
		//				0x16	Walk (Armor)
	};

	TESFullName					fullName;				// 030
	TESScriptableForm			scriptable;				// 03C
	TESEnchantableForm			enchantable;			// 048
	TESValueForm				value;					// 058
	TESWeightForm				weight;					// 060
	TESHealthForm				health;					// 068
	TESBipedModelForm			bipedModel;				// 070
	BGSDestructibleObjectForm	destuctible;			// 14C
	BGSEquipType				equipType;				// 154
	BGSRepairItemList			repairItemList;			// 15C
	BGSBipedModelList			bipedModelList;			// 164
	BGSPickupPutdownSounds		pickupPutdownSounds;	// 16C

	UInt16						armorRating;			// 178
	UInt16						modifiesVoice;			// 17A
	float						damageThreshold;		// 17C
	UInt8						armorFlags;				// 180
	UInt8						pad181[3];				// 181
	float						armorHealthDmgMult;		// 184	JIP only!
	union												// 188
	{
		TESObjectARMO			*audioTemplate;
		tList<MovementSound>	*movementSounds;
	};
	UInt8						overrideSounds;			// 18C
	UInt8						pad18D[3];				// 18D
};
static_assert(sizeof(TESObjectARMO) == 0x190);

// C4
class TESObjectBOOK : public TESBoundObject
{
public:
	TESFullName					fullName;		// 30
	TESModelTextureSwap			model;			// 3C
	TESIcon						icon;			// 5C
	TESScriptableForm			scriptable;		// 68
	TESEnchantableForm			enchantable;	// 74
	TESValueForm				value;			// 84
	TESWeightForm				weight;			// 8C
	TESDescription				description;	// 94
	BGSDestructibleObjectForm	destuctible;	// 9C
	BGSMessageIcon				messageIcon;	// A4
	BGSPickupPutdownSounds		sounds;			// B4

	UInt32						unkC0;			// C0
};
static_assert(sizeof(TESObjectBOOK) == 0xC4);

// 154
class TESObjectCLOT : public TESBoundObject
{
public:
	TESFullName					fullName;		// 030
	TESScriptableForm			scriptable;		// 03C
	TESEnchantableForm			enchantable;	// 048
	TESValueForm				value;			// 058
	TESWeightForm				weight;			// 060
	TESBipedModelForm			bipedModel;		// 068
	BGSDestructibleObjectForm	destuctible;	// 144
	BGSEquipType				equipType;		// 14C
	// unk data
};

// 9C
class TESObjectCONT : public TESBoundAnimObject
{
public:
	TESContainer                container;				// 30
	TESFullName					name;					// 3C
	TESModelTextureSwap			model;					// 48
	TESScriptableForm			scriptForm;				// 68
	TESWeightForm				weightForm;				// 74
	BGSDestructibleObjectForm	destructForm;			// 7C
	BGSOpenCloseForm			openCloseForm;			// 84

	UInt32						unk88;					// 88
	TESSound					*openSound;				// 8C
	TESSound					*closeSound;			// 90
	TESSound					*randomLoopingSound;	// 94
	UInt8						flags;					// 98
	UInt8						pad99[3];				// 99
};

// 90
class TESObjectDOOR : public TESBoundAnimObject
{
public:
	TESFullName					name;					// 30
	TESModelTextureSwap			model;					// 3C
	TESScriptableForm			scriptForm;				// 5C
	BGSDestructibleObjectForm	destructForm;			// 68
	BGSOpenCloseForm			openCloseForm;			// 70

	UInt32						unk74;					// 74
	TESSound					*openSound;				// 78
	TESSound					*closeSound;			// 7C
	TESSound					*randomLoopingSound;	// 80
	UInt32						unk84;					// 84
	tList<void>					list88;					// 88
};

// IngredientItem (A4)
class IngredientItem;

// TESObjectLIGH (C8)
class TESObjectLIGH : public TESBoundAnimObject
{
public:
	enum
	{
		kFlag_Dynamic =			1,
		kFlag_CanBeCarried =	2,
		kFlag_Negative =		4,
		kFlag_Flicker =			8,
		kFlag_Unused =			0x10,
		kFlag_OffByDefault =	0x20,
		kFlag_FlickerSlow =		0x40,
		kFlag_Pulse =			0x80,
		kFlag_PulseSlow =		0x100,
		kFlag_SpotLight =		0x200,
		kFlag_SpotShadow =		0x400,
		//	JIP Only
		kFlag_ColorShift =		0x800,
		kFlag_ColorShiftSlow =	0x1000
	};

	TESFullName					fullName;		// 030
	TESModelTextureSwap			modelSwap;		// 03C
	TESIcon						icon;			// 05C
	BGSMessageIcon				messageIcon;	// 068
	TESScriptableForm			scriptable;		// 078
	TESWeightForm				weight;			// 084
	TESValueForm				value;			// 08C
	BGSDestructibleObjectForm	destructible;	// 094

	SInt32						time;			// 09C
	UInt32						radius;			// 0A0
	UInt8						red;			// 0A4
	UInt8						green;			// 0A5
	UInt8						blue;			// 0A6
	UInt8						padA7;			// 0A7
	UInt32						lightFlags;		// 0A8
	float						falloffExp;		// 0AC
	float						FOV;			// 0B0
	float						fadeValue;		// 0B4
	TESSound					*sound;			// 0B8
	NiVector3					vectorBC;		// 0BC

	/*__forceinline NiPointLight *CreatePointLight(TESObjectREFR *targetRef, NiNode *targetNode, bool forceDynamic)
	{
		return ThisCall<NiPointLight*>(0x50D810, this, targetRef, targetNode, forceDynamic);
	}*/
};
static_assert(sizeof(TESObjectLIGH) == 0x0C8);

// AC
class TESObjectMISC : public TESBoundObject
{
public:
	TESFullName					fullName;		// 30
	TESModelTextureSwap			modelSwap;		// 3C
	TESIcon						icon;			// 5C
	TESScriptableForm			scriptable;		// 68
	TESValueForm				value;			// 74
	TESWeightForm				weight;			// 7C
	BGSDestructibleObjectForm	destructible;	// 84
	BGSMessageIcon				messageIcon;	// 8C
	BGSPickupPutdownSounds		pickupPutdown;	// 9C

	UInt32						unkA8;			// A8
};
static_assert(sizeof(TESObjectMISC) == 0xAC);

// 9C
class TESCasinoChips : public TESBoundObject
{
public:
	TESFullName					fullName;		// 30
	TESModelTextureSwap			modelSwap;		// 3C
	TESIcon						icon;			// 5C
	BGSMessageIcon				messageIcon;	// 68
	TESValueForm				value;			// 78
	BGSDestructibleObjectForm	destructible;	// 80
	BGSPickupPutdownSounds		pickupPutdown;	// 88

	UInt32						unk94[2];		// 94
};
static_assert(sizeof(TESCasinoChips) == 0x9C);

// CC
class TESCaravanMoney : public TESBoundObject
{
public:
	TESFullName					fullName;		// 30
	TESModelTextureSwap			modelSwap;		// 3C
	TESIcon						icon;			// 5C
	BGSMessageIcon				messageIcon;	// 68
	TESValueForm				value;			// 78
	BGSPickupPutdownSounds		pickupPutdown;	// 80

	UInt32						unk8C[16];		// 8C
};
static_assert(sizeof(TESCaravanMoney) == 0xCC);

// 58
class TESObjectSTAT : public TESBoundObject
{
public:
	TESModelTextureSwap		model;		// 30
	UInt32					unk50[2];	// 50
};

/*// 74
class BGSMovableStatic : public TESFullName
{
public:
	BGSMovableStatic();
	~BGSMovableStatic();

	BGSDestructibleObjectForm	destructible;	// 0C
	TESObjectSTAT				objectSTAT;		// 14
	UInt32						unk6C[2];		// 6C
};
static_assert(sizeof(BGSMovableStatic) == 0x74);*/

// Note: The above (commented-out) is the "actual" definition; The one below is the "effective" definition, used at runtime.
// Access to TESFullName and BGSDestructibleObjectForm via DYNAMIC_CAST.
class BGSMovableStatic : public TESObjectSTAT
{
	UInt32			unk58[2];	// 58
};
static_assert(sizeof(BGSMovableStatic) == 0x60);

// 50
class BGSStaticCollection : public TESBoundObject
{
public:
	TESModelTextureSwap		model;		// 30
};
static_assert(sizeof(BGSStaticCollection) == 0x50);

// 50
class BGSPlaceableWater : public TESBoundObject
{
public:
	enum WaterFlags
	{
		kWtrFlag_Reflects =					1,
		kWtrFlag_ReflectsActors =			2,
		kWtrFlag_ReflectsLand =				4,
		kWtrFlag_ReflectsLODLand =			8,
		kWtrFlag_ReflectsLODBuildings =		0x10,
		kWtrFlag_ReflectsLODTrees =			0x20,
		kWtrFlag_ReflectsSky =				0x40,
		kWtrFlag_ReflectsDynamicObjects =	0x80,
		kWtrFlag_ReflectsDeadBodies =		0x100,
	
		kWtrFlag_Refracts =					0x200,
		kWtrFlag_RefractsActors =			0x400,
		kWtrFlag_RefractsLand =				0x800,
		kWtrFlag_RefractsDeadBodies =		0x20000,
		kWtrFlag_RefractsDynamicObjects =	0x10000,
	
		kWtrFlag_SilhouetteReflections =	0x40000,
		kWtrFlag_Depth =					0x10000000,
		kWtrFlag_ObjectTextureCoords =		0x20000000,
		kWtrFlag_AutoGenerated =			0x40000000,
		kWtrFlag_NoUnderwaterFog =			0x80000000
	};

	TESModel			model;		// 30
	UInt32				waterFlags;	// 48
	TESWaterForm		*water;		// 4C
};

// TESObjectTREE (94)
class TESObjectTREE;

// TESFlora (90)
class TESFlora;

class TESObjectIMOD : public TESBoundObject
{
public:
	TESFullName					name;				// 030
	TESModelTextureSwap			model;				// 03C
	TESIcon						icon;				// 05C
	TESScriptableForm			scriptForm;			// 068
	TESDescription				description;		// 074
	TESValueForm				value;				// 07C
	TESWeightForm				weight;				// 084
	BGSDestructibleObjectForm	destructible;		// 08C
	BGSMessageIcon				messageIcon;		// 094
	BGSPickupPutdownSounds		pickupPutdownSounds;// 0A4
};

// 388
class TESObjectWEAP : public TESBoundObject
{
public:
	enum EWeaponType {
		kWeapType_HandToHandMelee = 0,
		kWeapType_OneHandMelee,
		kWeapType_TwoHandMelee,
		kWeapType_OneHandPistol,
		kWeapType_OneHandPistolEnergy,
		kWeapType_TwoHandRifle,
		kWeapType_TwoHandAutomatic,
		kWeapType_TwoHandRifleEnergy,
		kWeapType_TwoHandHandle,
		kWeapType_TwoHandLauncher,
		kWeapType_OneHandGrenade,
		kWeapType_OneHandMine,
		kWeapType_OneHandLunchboxMine,
		kWeapType_OneHandThrown,
		kWeapType_Last	// During animation analysis, player weapon can be classified as 0x0C = last
	};

	enum EWeaponSounds {
		kWeapSound_Shoot3D = 0,
		kWeapSound_Shoot2D,
		kWeapSound_Shoot3DLooping,
		kWeapSound_NoAmmo,
		kWeapSound_Swing = kWeapSound_NoAmmo,
		kWeapSound_Block,
		kWeapSound_Idle,
		kWeapSound_Equip,
		kWeapSound_Unequip
	};

	enum EHandGrip {
		eHandGrip_Default	= 0xFF,
		eHandGrip_1			= 0xE6,
		eHandGrip_2			= 0xE7,
		eHandGrip_3			= 0xE8,
		eHandGrip_4			= 0xE9,
		eHandGrip_5			= 0xEA,
		eHandGrip_6			= 0xEB,
		eHandGrip_Count		= 7,
	};

	enum EAttackAnimations {
		eAttackAnim_Default =		255,
		eAttackAnim_Attack3 =		38,
		eAttackAnim_Attack4 =		44,
		eAttackAnim_Attack5 =		50,
		eAttackAnim_Attack6 =		56,
		eAttackAnim_Attack7 =		62,
		eAttackAnim_Attack8 =		68,
		eAttackAnim_Attack9 =		144,
		eAttackAnim_AttackLeft =	26,
		eAttackAnim_AttackLoop =	74,
		eAttackAnim_AttackRight =	32,
		eAttackAnim_AttackSpin =	80,
		eAttackAnim_AttackSpin2 =	86,
		eAttackAnim_AttackThrow =	114,
		eAttackAnim_AttackThrow2 =	120,
		eAttackAnim_AttackThrow3 =	126,
		eAttackAnim_AttackThrow4 =	132,
		eAttackAnim_AttackThrow5 =	138,
		eAttackAnim_AttackThrow6 =	150,
		eAttackAnim_AttackThrow7 =	156,
		eAttackAnim_AttackThrow8 =	162,
		eAttackAnim_PlaceMine =		102,
		eAttackAnim_PlaceMine2 =	108,
		eAttackAnim_Count =			23,
	};

	enum ReloadAnim {
		eReload_A = 0,
		eReload_B,
		eReload_C,
		eReload_D,
		eReload_E,
		eReload_F,
		eReload_G,
		eReload_H,
		eReload_I,
		eReload_J,
		eReload_K,
		eReload_L,
		eReload_M,
		eReload_N,
		eReload_O,
		eReload_P,
		eReload_Q,
		eReload_R,
		eReload_S,
		eReload_W,
		eReload_X,
		eReload_Y,
		eReload_Z,
		eReload_Count,
	};
	static_assert(eReload_Count == 23);

	enum EWeaponFlags1 {
		eFlag_IgnoresNormalWeapResist	= 0x1,
		eFlag_IsAutomatic				= 0x2,
		eFlag_HasScope					= 0x4,
		eFlag_CantDrop					= 0x8,
		eFlag_HideBackpack				= 0x10,
		eFlag_EmbeddedWeapon			= 0x20,
		eFlag_No1stPersonISAnims		= 0x40,
		eflag_NonPlayable				= 0x80
	};

	enum EWeaponFlags2 {
		eFlag_PlayerOnly				= 0x1,
		eFlag_NPCsUseAmmo				= 0x2,
		eFlag_NoJamAfterReload			= 0x4,
		eFlag_ActionPointOverride		= 0x8,
		eFlag_MinorCrime				= 0x10,
		eFlag_FixedRange				= 0x20,
		eFlag_NotUsedNormalCombat		= 0x40,
		eFlag_DamageToWeaponOverride	= 0x80,
		eFlag_No3rdPersonISAnims		= 0x100,
		eFlag_BurstShot					= 0x200,
		eFlag_RumbleAlternate			= 0x400,
		eFlag_LongBurst					= 0x800,
		eFlag_ScopeNightVision			= 0x1000,
		eFlag_ScopeFromMod				= 0x2000
	};

	enum EEmbedWeapAV {
		eEmbedAV_Perception				= 0,
		eEmbedAV_Endurance,
		eEmbedAV_LeftAttack,
		eEmbedAV_RightAttack,
		eEmbedAV_LeftMobility,
		eEmbedAV_RightMobility,
		eEmbedAV_Brain,
	};

	enum EOnHit {
		eOnHit_Normal					= 0,
		eOnHit_DismemberOnly,
		eOnHit_ExplodeOnly,
		eOnHit_NoDismemberOrExplode,
	};

	enum ERumblePattern {
		eRumblePattern_Constant			= 0,
		eRumblePattern_Square,
		eRumblePattern_Triangle,
		eRumblePattern_Sawtooth
	};

	enum ECritDamageFlags {
		eCritDamage_OnDeath				= 0x1
	};

	enum
	{
		kWeaponModEffect_None = 0,
		kWeaponModEffect_IncreaseDamage,
		kWeaponModEffect_IncreaseClipCapacity,
		kWeaponModEffect_DecreaseSpread,
		kWeaponModEffect_DecreaseWeight,
		kWeaponModEffect_RegenerateAmmo_Shots,
		kWeaponModEffect_RegenerateAmmo_Seconds,
		kWeaponModEffect_DecreaseEquipTime,
		kWeaponModEffect_IncreaseRateOfFire,
		kWeaponModEffect_IncreaseProjectileSpeed,
		kWeaponModEffect_IncreaseMaxCondition,
		kWeaponModEffect_Silence,
		kWeaponModEffect_SplitBeam,
		kWeaponModEffect_VATSBonus,
		kWeaponModEffect_IncreaseZoom,
		kWeaponModEffect_DecreaseEquipTime2,
		kWeaponModEffect_Suppressor
	};

	// bases
	TESFullName					fullName;			// 030
	TESModelTextureSwap			textureSwap;		// 03C
	TESIcon						icon;				// 05C
	TESScriptableForm			scritpable;			// 068
	TESEnchantableForm			enchantable;		// 074
	TESValueForm				value;				// 084
	TESWeightForm				weight;				// 08C
	TESHealthForm				health;				// 094
	TESAttackDamageForm			attackDmg;			// 09C
	BGSAmmoForm					ammo;				// 0A4
	BGSClipRoundsForm			clipRounds;			// 0AC
	BGSDestructibleObjectForm	destructible;		// 0B4
	BGSRepairItemList			repairItemList;		// 0BC
	BGSEquipType				equipType;			// 0C4
	BGSPreloadable				preloadable;		// 0CC
	BGSMessageIcon				messageIcon;		// 0D0
	BGSBipedModelList			bipedModelList;		// 0E0
	BGSPickupPutdownSounds		pickupPutdownSounds;// 0E8

	UInt8				eWeaponType;		// 0F4
	UInt8				pad[3];
	float				animMult;			// 0F8
	float				reach;				// 0FC
	UInt8				weaponFlags1;		// 100
	UInt8				handGrip;			// 101
	UInt8				ammoUse;			// 102
	UInt8				reloadAnim;			// 103
	float				minSpread;			// 104
	float				spread;				// 108
	UInt32				unk10C;				// 10C
	float				sightFOV;			// 110
	UInt32				unk114;				// 114
	BGSProjectile		*projectile;		// 118
	UInt8				baseVATSChance;		// 11C
	UInt8				attackAnim;			// 11D
	UInt8				numProjectiles;		// 11E
	UInt8				embedWeaponAV;		// 11F
	float				minRange;			// 120
	float				maxRange;			// 124
	UInt32				onHit;				// 128
	UInt32				weaponFlags2;		// 12C
	float				animAttackMult;		// 130
	float				fireRate;			// 134
	float				AP;					// 138
	float				rumbleLeftMotor;	// 13C
	float				rumbleRightMotor;	// 140
	float				rumbleDuration;		// 144
	float				damageToWeaponMult;	// 148
	float				animShotsPerSec;	// 14C
	float				animReloadTime;		// 150
	float				animJamTime;		// 154		
	float				aimArc;				// 158
	UInt32				weaponSkill;		// 15C - actor value
	UInt32				rumblePattern;		// 160 - reload anim?
	float				rumbleWavelength;	// 164
	float				limbDamageMult;		// 168
	SInt32				resistType;			// 16c - actor value
	float				sightUsage;			// 170
	float				semiAutoFireDelay[2];	// 174
	UInt32				unk17C;				// 17C - 0-0x10: 0x8:str req 0x10: - skill req  - 0xb:kill impulse B158 - mod 1 val B15C - Mod 2 val Effects: 0x1: e(zoom) 0x2: a 0x3:0 0x4-6: Values c-e Mod Effects Val2:1-3 
	UInt32				effectMods[3];		// 180
	float				value1Mod[3];		// 18C
	UInt32				powerAttackAnimOverride;	// 198
	UInt32				strRequired;		// 19C
	UInt8				pad1A0;				// 1A0
	UInt8				modReloadAnim;		// 1A1
	UInt8				pad1A2[2];			// 1A2
	float				regenRate;			// 1A4
	float				killImpulse;		// 1A8
	float				value2Mod[3];		// 1AC
	float				impulseDist;		// 1B8
	UInt32				skillRequirement;	// 1BC
	UInt16				criticalDamage;		// 1C0
	UInt8				unk1C2[2];			// 1C2
	float				criticalPercent;	// 1C4
	UInt8				critDamageFlags;	// 1C8
	UInt8				pad1C9[3];			// 1C9
	SpellItem			*criticalEffect;	// 1CC
	TESModel			shellCasingModel;	// 1DO
	TESModel			targetNIF;			// 1E8 - target NIF
	TESModel			model200;			// 200 - could be a texture swap
	TESEffectShader		*scopeEffect;		// 218
	TESSound			*sounds[12];		// 21C
	BGSImpactDataSet	*impactDataSet;		// 24C
	TESObjectSTAT		*worldStatic;		// 250
	TESObjectSTAT		*modStatics[7];		// 254
	TESModelTextureSwap	modModels[7];		// 270
	TESObjectIMOD		*itemMod[3];		// 350
	String				embeddedNodeName;	// 35C
	UInt32				soundLevel;			// 364
	String				VATSAttackName;		// 368
	SpellItem			*VATSEffect;		// 370
	float				vatsSkill;			// 374
	float				vatsDmgMult;		// 378
	float				vatsAP;				// 37C
	UInt32				recharge;			// 380 maybe recharge
	UInt32				unk384;				// 384

	bool IsAutomatic() const {return (weaponFlags1 & eFlag_IsAutomatic) != 0;}
	bool IsPlayable() const {return !(weaponFlags1 & eflag_NonPlayable);}
	void SetPlayable(bool doset)
	{
		if (doset) weaponFlags1 &= ~eflag_NonPlayable;
		else weaponFlags1 |= eflag_NonPlayable;
	}
	UInt8 HandGrip() const;
	void SetHandGrip(UInt8 handGrip);
	UInt8 AttackAnimation() const;
	void SetAttackAnimation(UInt32 attackAnim);
	TESAmmo *GetAmmo() const;
	TESAmmo *GetEquippedAmmo(Actor *actor) const;
	float GetModBonuses(UInt8 modFlags, UInt32 effectID) const;
	TESModelTextureSwap *GetWeaponModel(UInt32 modFlags, Actor *actor) const;
};
static_assert(sizeof(TESObjectWEAP) == 0x388);

extern TESObjectWEAP *g_rockItLauncher;

enum AmmoEffectID : UInt32
{
	kAmmoEffect_DamageMod =		0,
	kAmmoEffect_DRMod =			1,
	kAmmoEffect_DTMod =			2,
	kAmmoEffect_SpreadMod =		3,
	kAmmoEffect_ConditionMod =	4,
	kAmmoEffect_FatigueMod =	5
};

// 30
class TESAmmoEffect : public TESForm
{
public:
	enum
	{
		kOperation_Add =		0,
		kOperation_Multiply =	1,
		kOperation_Subtract =	2,
	};

	TESFullName		fullName;		// 18
	AmmoEffectID	type;			// 24
	UInt32			operation;		// 28
	float			value;			// 2C
};
static_assert(sizeof(TESAmmoEffect) == 0x30);

class AmmoEffectList : public tList<TESAmmoEffect>
{
public:
	bool HasEffect(UInt32 type) const
	{
		auto iter = Head();
		do
		{
			if (iter->data && (iter->data->type == type))
				return true;
		}
		while (iter = iter->next);
		return false;
	}
};

// DC
class TESAmmo : public TESBoundObject
{
public:
	enum eAmmoFlags
	{
		kFlags_IgnoreWeapResistance =	1,
		kFlags_NonPlayable =			2,
	};

	// bases
	TESFullName					fullName;				// 30
	TESModelTextureSwap			model;					// 3C
	TESIcon						icon;					// 5C
	BGSMessageIcon				messageIcon;			// 68	
	TESValueForm				value;					// 78
	BGSClipRoundsForm			clipRounds;				// 80
	BGSDestructibleObjectForm	destructible;			// 88
	BGSPickupPutdownSounds		pickupPutdownsounds;	// 90
	TESScriptableForm			scriptable;				// 9C

	float						speed;					// A8
	UInt8						ammoFlags;				// AC
	UInt8						padAD[3];				// AD
	UInt32						projPerShot;			// B0
	BGSProjectile				*projectile;			// B4
	float						weight;					// B8
	TESBoundObject				*casingItem;			// BC
	float						casingChance;			// C0
	String						shortName;				// C4
	String						abbreviation;			// CC
	AmmoEffectList				effectList;				// D4

	bool IsPlayable() {return !(ammoFlags & kFlags_NonPlayable);}
	void SetPlayable(bool doset) {if (doset) ammoFlags &= ~kFlags_NonPlayable; else ammoFlags |= kFlags_NonPlayable;}
};
static_assert(sizeof(TESAmmo) == 0xDC);

class TESCaravanCard : public TESBoundObject
{
public:
};

// 2B4
struct BipedAnim
{
	enum eOptionalBoneType
	{
		kOptionalBone_Bip01Head			= 0,
		kOptionalBone_Weapon			= 1,
		kOptionalBone_Bip01LForeTwist	= 2,
		kOptionalBone_Bip01Spine2		= 3,
		kOptionalBone_Bip01Neck1		= 4,
	};

	enum SlotIDs
	{
		kSlot_Head = 0,
		kSlot_Hair,
		kSlot_UpperBody,
		kSlot_LeftHand,
		kSlot_RightHand,
		kSlot_Weapon,
		kSlot_PipBoy,
		kSlot_Backpack,
		kSlot_Necklace,
		kSlot_Headband,
		kSlot_Hat,
		kSlot_Eyeglasses,
		kSlot_Nosering,
		kSlot_Earrings,
		kSlot_Mask,
		kSlot_Choker,
		kSlot_MouthObject,
		kSlot_BodyAddon1,
		kSlot_BodyAddon2,
		kSlot_BodyAddon3
	};

	// 08
	struct OptionalBone	
	{
		bool		exists;
		NiNode		*bone;
	};

	// 10
	struct Data
	{
		union									// 00 can be a modelled form (Armor or Weapon) or a Race if not equipped
		{
			TESForm				*item;
			TESObjectARMO		*armor;
			TESObjectWEAP		*weapon;
			TESRace				*race;
		};
		TESModelTextureSwap		*modelTexture;	// 04 texture or model for said form
		NiAVObject				*object;		// 08 NiNode for the modelled form
		UInt8					hasSkinnedGeom;	// 0C
		UInt8					pad0D[3];		// 0D
	};

	NiNode				*bip01;			// 000 receive Bip01 node, then optionally Bip01Head, Weapon, Bip01LForeTwist, Bip01Spine2, Bip01Neck1
	OptionalBone		bones[5];		// 004
	Data				slotData[20];	// 02C indexed by the EquipSlot
	Data				unk016C[20];	// 16C indexed by the EquipSlot
	UInt32				unk2AC;			// 2AC
	Character			*character;		// 2B0
};
static_assert(sizeof(BipedAnim) == 0x2B4);

// 20
struct FaceGenData
{
	UInt32		unk00;		// 00
	void		*unk04;		// 04
	UInt32		unk08;		// 08
	float		**values;	// 0C
	UInt32		useOffset;	// 10
	UInt32		maxOffset;	// 14
	UInt32		count;		// 18
	UInt32		size;		// 1C
};

// 1EC
class TESNPC : public TESActorBase
{
public:
	TESRaceForm				race;				// 10C
	UInt8					skillValues[14];	// 114
	UInt8					skillOffsets[14];	// 122
	TESClass				*classID;			// 130
	FaceGenData				faceGenData[3];		// 134
	UInt32					unk194[8];			// 194
	FaceGenData				*faceGenDataPtr;	// 1B4
	TESHair					*hair;				// 1B8
	float					hairLength;			// 1BC
	TESEyes					*eyes;				// 1C0
	BSFaceGenNiNode			*unk1C4;			// 1C4
	BSFaceGenNiNode			*unk1C8;			// 1C8
	UInt32					unk1CC;				// 1CC
	UInt16					unk1D0;				// 1D0
	UInt16					unk1D2;				// 1D2
	TESCombatStyle			*combatStyle;		// 1D4
	ColorRGBA				hairColor;			// 1D8
	tList<BGSHeadPart>		headPart;			// 1DC
	UInt32					impactMaterialType;	// 1E4
	UInt32					unk01E8;			// 1E8
	TESRace					*race1EC;			// 1EC
	TESNPC					*copyFrom;			// 1F0	Not set once PlayerRef exists and the target is the Player
	float					height;				// 1F4
	float					weight;				// 1F8	Aparently, getWeight purposly returns height except for the player.
	NiTArray<FaceGenUndo*>	faceGenUndo;		// 1FC

	void __fastcall SetSex(UInt32 flags);
	void __fastcall SetRace(TESRace *pRace);
	__forceinline void CopyAppearance(TESNPC *srcNPC)
	{
		ThisCall(0x603790, this, srcNPC);
	}
};

static_assert(sizeof(TESNPC) == 0x20C);

// 160
class TESCreature : public TESActorBase
{
public:
	TESAttackDamageForm			attackDmg;			// 10C
	TESModelList				modelList;			// 114

	TESCreature					*audioTemplate;		// 128
	UInt8						type;				// 12C
	UInt8						combatSkill;		// 12D
	UInt8						magicSkill;			// 12E
	UInt8						stealthSkill;		// 12F
	UInt8						attackReach;		// 130
	UInt8						pad0131[3];			// 131
	float						turningSpeed;		// 134
	float						footWeight;			// 138
	float						baseScale;			// 13C
	TESCombatStyle				*combatStyle;		// 140
	BGSBodyPartData				*bodyPartData;		// 144
	UInt32						materialType;		// 148
	BGSImpactDataSet			*impactDataSet;		// 14C
	UInt32						unk0150;			// 150
	UInt32						soundLevel;			// 154
	BGSListForm					*weaponList;		// 158
	UInt8						byt015C;			// 15C
	UInt8						pad015D[3];			// 15D
};

// 1C
class TESLeveledList : public BaseFormComponent
{
public:
	virtual UInt8	GetChanceNone();		// 10
	virtual bool	GetCalcEachInCount();	// 14
	virtual UInt32	GetLevelDifferenceMax();// 18

	struct LoadBaseData	// as used by LoadForm
	{
		SInt16			level;		// 00
		UInt16			fill002;	// 02
		TESObjectREFR	*refr;		// 04
		SInt16			count;		// 08
		UInt16			fill00A;	// 0A
	};	// 0C

	struct ListData
	{
		TESForm			*form;		// 00
		SInt16			count;		// 04
		SInt16			level;		// 06
		ContainerExtra	*extra;		// 08
	}; // 0C

	enum
	{
		kFlags_CalcAllLevels =		1 << 0,
		kFlags_CalcEachInCount =	1 << 1,
		kFlags_UseAll =				1 << 2,
	};

	tList<ListData>		list;			// 04
	UInt8				chanceNone;		// 0C
	UInt8				flags;			// 0D
	UInt16				pad00E;			// 0E
	TESGlobal			*global;		// 10 use global value for chance none?
	tList<void>			list14;			// 14

	void AddItem(TESForm *form, UInt16 level, UInt16 count, float health);
	UInt32 RemoveItem(TESForm *form);
	void Dump();
	bool RemoveNthItem(UInt32 itemIndex);
	SInt32 GetItemIndexByForm(TESForm *form);
	SInt32 GetItemIndexByLevel(UInt32 level);
};

extern UInt32 s_dumpLvlListIndent;

// TESLevCreature (68)
class TESLevCreature : public TESBoundObject
{
public:
	TESLeveledList		list;		// 030
	TESModelTextureSwap	texture;	// 04C
};

// TESLevCharacter (68)
class TESLevCharacter : public TESBoundObject
{
public:
	TESLeveledList		list;		// 030
	TESModelTextureSwap	texture;	// 04C
};

// TESKey (A8)
class TESKey : public TESObjectMISC
{
public:
};

// BGSIdleMarker (40)
class BGSIdleMarker;

// BGSNote (80)
class BGSNote : public TESBoundObject
{
public:
	TESModel					model;					// 30
	TESFullName					fullName;				// 48
	TESIcon						icon;					// 54
	BGSPickupPutdownSounds		pickupPutdownSounds;	// 60
	union												// 6C
	{
		TESDescription			*noteText;
		TESTexture				*picture;
		TESTopic				*voice;
	};
	UInt32						unk70;					// 70
	UInt32						unk74;					// 74
	UInt32						unk78;					// 78
	UInt8                       noteType;				// 7C
	UInt8                       read;					// 7D
	UInt8                       byte7E;					// 7E
	UInt8                       byte7F;					// 7F
};
static_assert(sizeof(BGSNote) == 0x80);

// BGSConstructibleObject (B0)
class BGSConstructibleObject;

// C0
class BGSProjectile : public TESBoundObject
{
public:
	enum
	{
		kFlags_Hitscan =				0x1,
		kFlags_Explosion =				0x2,
		kFlags_AltTrigger =				0x4,
		kFlags_MuzzleFlash =			0x8,
		//								0x10,
		kFlags_CanBeDisabled =			0x20,
		kFlags_CanBePicked =			0x40,
		kFlags_Supersonic =				0x80,
		kFlags_PinsLimbs =				0x100,
		kFlags_PassSmallTransparent =	0x200,
		kFlags_Detonates =				0x400,
		kFlags_Rotation =				0x800,
	};

	enum ProjectileType
	{
		eProjType_Missile =			1,
		eProjType_Lobber =			2,
		eProjType_Beam =			4,
		eProjType_Flame =			8,
		eProjType_ContinuousBeam =	0x10
	};

	TESFullName						fullName;			// 30
	TESModel						model;				// 3C
	BGSPreloadable					preloadable;		// 54
	BGSDestructibleObjectForm		destructible;		// 58

	UInt16							projFlags;			// 60
	UInt16							type;				// 62
	float							gravity;			// 64
	float							speed;				// 68
	float							range;				// 6C
	TESObjectLIGH					*lightProjectile;	// 70
	TESObjectLIGH					*lightMuzzleFlash;	// 74
	float							tracerChance;		// 78
	float							altProximity;		// 7C
	float							altTimer;			// 80
	BGSExplosion					*explosion;			// 84
	TESSound						*soundProjectile;	// 88
	float							flashDuration;		// 8C
	float							fadeDuration;		// 90
	float							impactForce;		// 94
	TESSound						*soundCountDown;	// 98
	TESSound						*soundDisable;		// 9C
	TESObjectWEAP					*defaultWeapSrc;	// A0
	float							rotationX;			// A4
	float							rotationY;			// A8
	float							rotationZ;			// AC
	float							bouncyMult;			// B0
	TESModel						muzzleFlash;		// B4
	UInt8							soundLevel;			// CC

	void SetFlag(UInt32 pFlag, bool bEnable)
	{
		if (bEnable) projFlags |= pFlag;
		else projFlags &= ~pFlag;
	}
};

// TESLevItem (44)
class TESLevItem : public TESBoundObject
{
public:
	TESLeveledList		list;
};

// 36C
class TESWeather : public TESForm
{
public:
	struct WeatherSound
	{
		UInt32		soundID;	// refID of TESSound
		UInt32		type;		// 0 - Default; 1 - Precip; 2 - Wind; 3 - Thunder
	};

	UInt32					unk018;						// 018	TESImageSpaceModifiableCountForm<6>
	TESImageSpaceModifier	*imageSpaceMods[6];			// 01C
	TESTexture1024			layerTextures[4];			// 034
	UInt8					cloudSpeed[4];				// 064
	ColorRGBA				cloudColor[4][6];			// 068
	TESModel				model;						// 0C8
	UInt8					windSpeed;					// 0E0
	UInt8					cloudSpeedLower;			// 0E1
	UInt8					cloudSpeedUpper;			// 0E2
	UInt8					transDelta;					// 0E3
	UInt8					sunGlare;					// 0E4
	UInt8					sunDamage;					// 0E5
	UInt8					precipitationBeginFadeIn;	// 0E6
	UInt8					precipitationEndFadeOut;	// 0E7
	UInt8					lightningBeginFadeIn;		// 0E8
	UInt8					lightningEndFadeOut;		// 0E9
	UInt8					lightningFrequency;			// 0EA
	UInt8					weatherClassification;		// 0EB
	ColorRGBA				lightningColor;				// 0EC
	float					fogDistance[6];				// 0F0
	ColorRGBA				colors[10][6];				// 108
	tList<WeatherSound>		sounds;						// 1F8
	UInt32					unk200[91];					// 200
};
static_assert(sizeof(TESWeather) == 0x36C);

struct WeatherEntry
{
	TESWeather		*weather;
	UInt32			chance;
	TESGlobal		*global;
};
typedef tList<WeatherEntry> WeatherTypes;

// 58
class TESClimate : public TESForm
{
public:
	TESModel			nightSkyModel;		// 18
	WeatherTypes		weatherTypes;		// 30
	TESTexture			sunTexture;			// 38
	TESTexture			sunGlareTexture;	// 44
	UInt8				sunriseBegin;		// 50
	UInt8				sunriseEnd;			// 51
	UInt8				sunsetBegin;		// 52
	UInt8				sunsetEnd;			// 53
	UInt8				volatility;			// 54
	UInt8				phaseLength;		// 55
	UInt8				pad56[2];			// 56

	WeatherEntry *GetWeatherEntry(TESWeather *weather, bool remove = false);
};

static_assert(sizeof(TESClimate) == 0x58);

// 08
class TESRegionData
{
public:
	enum
	{
		kRegionData_Weather = 3,
		kRegionData_Map,
		kRegionData_Landscape,
		kRegionData_Grass,
		kRegionData_Sound,
		kRegionData_Imposter
	};

	/*00*/virtual void	Destroy(bool doFree);
	/*04*/virtual void	Save();
	/*08*/virtual void	Unk_02(void);
	/*0C*/virtual void	Unk_03(void);
	/*10*/virtual UInt32	GetType();
	/*14*/virtual TESRegionData	*CreateCopy();
	/*18*/virtual void	Unk_06(void);
	/*1C*/virtual void	Unk_07(void);
	/*20*/virtual void	Unk_08(void);
	/*24*/virtual void	Load(ModInfo *modInfo);

	bool				bOverride;	// 04
	UInt8				byte05;		// 05
	UInt8				priority;	// 06
	UInt8				byte07;		// 07
};
typedef tList<TESRegionData> RegionDataEntryList;

class TESRegionDataGrass : public TESRegionData
{
public:
	/*28*/virtual void	Unk_0A(void);
};

// 10
class TESRegionDataImposter : public TESRegionData
{
public:
	tList<TESObjectREFR>	imposters;	// 08
};

class TESRegionDataLandscape : public TESRegionData
{
public:
	/*28*/virtual void	Unk_0A(void);
	/*2C*/virtual void	Unk_0B(void);
};

// 10
class TESRegionDataMap : public TESRegionData
{
public:
	/*28*/virtual void	Unk_0A(void);
	/*2C*/virtual void	Unk_0B(void);
	/*30*/virtual void	Unk_0C(void);
	/*34*/virtual void	Unk_0D(void);

	String		mapName;	// 08
};

struct SoundType
{
	TESSound		*sound;
	UInt32			flags;
	UInt32			chance;
};
typedef tList<SoundType> SoundTypeList;

class TESRegionDataSound : public TESRegionData
{
public:
	/*28*/virtual BGSMusicType	*GetMusicType();
	/*2C*/virtual void	SetMusicType(BGSMusicType *_musicType);
	/*30*/virtual MediaSet	*GetIncidentalSet();
	/*34*/virtual MediaSet	*GetBattleSet();
	/*38*/virtual void	SetIncidentalSet(MediaSet *incidentalSet);

	BGSMusicType	*musicType;			// 08
	SoundTypeList	soundTypes;			// 0C
	UInt32			incidentalMediaSet;	// 14	RefID
	tList<UInt32>	battleMediaSets;	// 18	RefIDs
};

// 10
class TESRegionDataWeather : public TESRegionData
{
public:
	WeatherTypes	weatherTypes;	// 08
};

struct AreaPointEntry
{
	float	x;
	float	y;
};
typedef tList<AreaPointEntry> AreaPointEntryList;

struct RegionAreaEntry
{
	AreaPointEntryList	points;
	UInt32				unk08[2];
	float				unk10[4];
	UInt32				edgeFallOff;
	UInt32				pointCount;
};
typedef tList<RegionAreaEntry> RegionAreaEntryList;

// 38
class TESRegion : public TESForm
{
public:
	RegionDataEntryList	*dataEntries;	// 18
	RegionAreaEntryList	*areaEntries;	// 1C
	TESWorldSpace		*worldSpace;	// 20
	TESWeather			*weather;		// 24
	UInt32				unk28;			// 28
	float				flt2C;			// 2C
	float				flt30;			// 30
	float				flt34;			// 34
};
static_assert(sizeof(TESRegion) == 0x38);

// 10
class TESRegionList : public BSSimpleList<TESRegion>
{
public:
	UInt8			byte0C;		// 0C
	UInt8			pad0D[3];	// 0D
};

// E0
class TESObjectCELL : public TESForm
{
public:
	typedef tList<TESObjectREFR> RefList;
	
	struct LightingData
	{
		ColorRGBA	ambient;			// 00
		ColorRGBA	directional;		// 04
		ColorRGBA	fog;				// 08
		float		fogNear;			// 0C
		float		fogFar;				// 10
		int			directionalRotXY;	// 14
		int			directionalRotZ;	// 18
		float		directionalFade;	// 1C
		float		fogClipDist;		// 20
		float		fogPower;			// 24
		void		*getValuesFrom;		// 28
	};

	// 64
	struct CellRenderData
	{
		enum CellSubNodes
		{
			kNodeIdx_Actors,
			kNodeIdx_Markers,
			kNodeIdx_Land,
			kNodeIdx_StaticObj,
			kNodeIdx_DynamicObj,
			kNodeIdx_OcclusionPlanes,
			kNodeIdx_Portals,
			kNodeIdx_Multibounds,
			kNodeIdx_Collision
		};

		NiNode										*masterNode;			// 00
		tList<TESObjectREFR>						largeRefs;				// 04	refs with bound size > 3000
		NiTMap<TESObjectREFR*, NiNode*>				animatedRefs;			// 0C
		NiTMap<TESForm*, TESObjectREFR*>			emittanceSourceRefs;	// 1C
		NiTMap<TESObjectREFR*, NiNode*>				emittanceLightRefs;		// 2C
		NiTMap<TESObjectREFR*, BSMultiBoundNode*>	multiboundRefs;			// 3C
		tList<TESObjectREFR>						scriptedNonActors;		// 4C
		tList<TESObjectREFR>						activatingParentRefs;	// 54	Has ExtraActivateRefChildren
		tList<TESObjectREFR>						placeableWaterList;		// 5C
	};

	enum
	{
		kCellFlag_IsInterior =					1 << 0,
		kCellFlag_HasWater =					1 << 1,
		kCellFlag_InvertFastTravelBehavior =	1 << 2,
		kCellFlag_ForceHideLand =				1 << 3,
		kCellFlag_PublicPlace =					1 << 5,
		kCellFlag_HandChanged =					1 << 6,
		kCellFlag_BehaveLikeExterior =			1 << 7,
	};

	enum LoadingState
	{
		kState_NotLoaded =	0,
		kState_Unloading =	1,
		kState_Loading =	2,
		kState_Loaded =		3,
		kState_Detaching =	4,
		kState_Attaching =	5,
		kState_Attached =	6
	};

	TESFullName				fullName;				// 18
	UInt8					cellFlags;				// 24
	UInt8					fullySeen;				// 25	Fully visible on local-map
	UInt8					loadingStage;			// 26
	UInt8					byte27;					// 27
	ExtraDataList			extraDataList;			// 28
	union											// 48
	{
		CellCoord			*exteriorCoords;
		LightingData		*interiorLighting;
	};
	TESObjectLAND			*land;					// 4C
	float					waterHeight;			// 50
	bool					autoWaterLoaded;		// 54
	UInt8					pad55[3];				// 55
	TESTexture				noiseTexture;			// 58
	BSSimpleArray<NavMesh*>	*navMeshArray;			// 64
	UInt32					unk68[6];				// 68
	LightCS					refLock;				// 80
	UInt32					unk88[6];				// 88
	SInt32					criticalQueuedRefCount;	// A0
	SInt32					queuedRefCount;			// A4
	UInt16					visibleDistantCount;	// A8
	UInt16					visibleDistantLoadedCount;	// AA
	RefList					objectList;				// AC
	NiNode					*lightMarkerNode;		// B4
	NiNode					*soundMarkerNode;		// B8
	UInt32					unkBC;					// BC
	TESWorldSpace			*worldSpace;			// C0
	CellRenderData			*renderData;			// C4
	float					LODFadeOut;				// C8
	UInt8					byteCC;					// CC
	UInt8					byteCD;					// CD
	bool					triggeredLODHide;		// CE
	UInt8					byteCF;					// CF
	bool					canHideLOD;				// D0
	bool					cellDetached;			// D1
	bool					skippedFade;			// D2
	UInt8					byteD3;					// D3
	BSPortalGraph			*portalGraph;			// D4
	BGSLightingTemplate		*lightingTemplate;		// D8
	UInt32					inheritFlags;			// DC

	bool IsInterior() const {return (cellFlags & kCellFlag_IsInterior) != 0;}
	NiNode **Get3DNodes() const {return (NiNode**)renderData->masterNode->m_children.data;}
	NiNode *Get3DNode(UInt32 index) const {return Get3DNodes()[index];}
	void __fastcall ToggleNodes(UInt32 nodeBits);

	void RefLockEnter()
	{
		refLock.Enter();
	}
	void RefLockLeave()
	{
		refLock.Leave();
	}
};
static_assert(sizeof(TESObjectCELL) == 0xE0);

typedef BSSimpleArray<TESObjectREFR*> ObjectRefrArray;

// 3C	C'tor @ 0x6FC490
struct BGSTerrainManager
{
	struct BGSTerrainNode;

	// 30
	struct TerrainChunk
	{
		BGSTerrainNode				*parent;		// 00
		NiAVObject					*landMesh;		// 04
		NiAVObject					*waterMesh;		// 08
		NiAVObject					*object0C;		// 0C
		NiObject					*object10;		// 10
		BSMultiBoundNode			*multiBound14;	// 14
		BSMultiBoundNode			*multiBound18;	// 18
		UInt32						unk1C;			// 1C
		UInt32						unk20;			// 20
		BGSTerrainChunkLoadTask		*loadTask;		// 24
		UInt8						byte28;			// 28
		UInt8						isInitialized;	// 29
		UInt8						shaderInitDone;	// 2A
		UInt8						pad2B;			// 2B
		NiObject					*object2C;		// 2C
	};

	// 2C	C'tor @ 0x6F5160
	struct BGSDistantObjectBlock
	{
		BGSTerrainNode				*node;			// 00
		BSSegmentedTriShape			*shape;			// 04
		BSMultiBoundNode			*multiBound;	// 08
		NiAVObject					*object0C;		// 0C
		NiObject					*object10;		// 10
		NiObject					*object14;		// 14
		BGSDistantObjectBlockLoadTask	*loadTask;	// 18
		NiObject					*object1C;		// 1C
		UInt8						byte20;			// 20
		UInt8						byte21;			// 21
		UInt8						byte22;			// 22
		UInt8						isHigh;			// 23
		UInt8						byte24;			// 24
		UInt8						pad25[3];		// 25
		UInt32						unk28;			// 28
	};

	// 4C
	struct DistantTreeBlock
	{
		struct TreeGroup;
		struct InstanceData;

		BGSDistantTreeBlockLoadTask	*loadTask;		// 00
		BSSimpleArray<TreeGroup>	treeGroups;		// 04
		NiTPtrMap<InstanceData>		instanceMap;	// 14
		NiTPtrMap<TreeGroup>		groupMap;		// 24
		BSSimpleArray<UInt32>		array34;		// 34
		BGSTerrainNode				*node;			// 44
		UInt8						byte48;			// 48
		UInt8						byte49;			// 49
		UInt8						pad4A[2];		// 4A
	};

	// 60	C'tor @ 0x6FD210
	struct BGSTerrainNode
	{
		BGSTerrainManager		*parent;		// 00
		UInt32					lodLevel;		// 04
		Coordinate				cellXY;			// 08
		UInt8					byte0C;			// 0C
		UInt8					byte0D;			// 0D
		UInt8					byte0E;			// 0E
		UInt8					byte0F;			// 0F
		TerrainChunk			*trrChunk;		// 10
		BGSDistantObjectBlock	*object;		// 14
		DistantTreeBlock		*distTree;		// 18
		UInt32					ukn1C;			// 1C
		BGSTerrainNode			*linked[4];		// 20
		void					*ptr30;			// 30
		NiVector3				pos34;			// 34
		float					flt40;			// 40
		float					splitDist;		// 44
		float					morphStartDist;	// 48
		float					morphEndDist;	// 4C
		UInt32					terrainFadeTimer;	// 50
		UInt32					ukn54;			// 54
		UInt32					linkID;			// 58
		UInt8					shouldShow;		// 5C
		UInt8					byte5D;			// 5D
		UInt8					byte5E;			// 5E
		UInt8					byte5F;			// 5F

		BGSTerrainNode *GetNodeByCoord(Coordinate coord) const;
	};
	static_assert(sizeof(BGSTerrainNode) == 0x60);

	TESWorldSpace			*world;			// 00
	BGSTerrainNode			*lodNode;		// 04
	NiNode					*node08;		// 08
	NiNode					*waterLODNode;	// 0C
	Coordinate				coordNW;		// 10
	Coordinate				coordSE;		// 14
	UInt32					lodLevelMax;	// 18
	UInt32					lodLevelMin;	// 1C
	UInt32					stride;			// 20
	UInt32					lodLevel;		// 24
	UInt8					byte28;			// 28
	UInt8					byte29;			// 29
	UInt8					byte2A;			// 2A
	UInt8					byte2B;			// 2B
	ObjectRefrArray			array2C;		// 2C
};
static_assert(sizeof(BGSTerrainManager) == 0x3C);

typedef NiTPtrMap<TESObjectCELL> CellPointerMap;

struct ImpactDataSwap
{
	NiTMap<BGSImpactData*, BGSImpactData*>	*impactDataSwapMaps[12];		// 000
	char									footstepMaterialNames[10][30];	// 030
};
static_assert(sizeof(ImpactDataSwap) == 0x15C);

// EC
class TESWorldSpace : public TESForm
{
public:
	virtual bool	GetNameForLocation(String *result, float x, float y, float z);
	virtual void	Unk_4F(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);

	struct DCoordXY
	{
		SInt32	X;
		SInt32	Y;
	};

	struct WCoordXY
	{
		SInt16	X;
		SInt16	Y;
	};

	struct Offset_Data
	{
		UInt32		**unk00;	// 00 array of UInt32 stored in OFST record.
		NiPoint2	min;		// 04 NAM0
		NiPoint2	max;		// 0C NAM9
		UInt32		fileOffset;	// 14 TESWorldspace file offset in modInfo
	};	// 014

	struct MapData
	{
		DCoordXY	usableDimensions;	// 00
		WCoordXY	cellNWCoordinates;	// 08
		WCoordXY	cellSECoordinates;	// 0C
	};	// 010

	typedef NiTPtrMap<tList<TESObjectREFR>> RefListPointerMap;
	typedef NiTMap<ModInfo*, TESWorldSpace::Offset_Data*> OffsetDataMap;

	enum
	{
		kWorldFlag_SmallWorld =			1 << 0,
		kWorldFlag_NoFastTravel =		1 << 1,
		kWorldFlag_NoLODWater =			1 << 4,
		kWorldFlag_NoLODNoise =			1 << 5,
		kWorldFlag_NoFallDamage =		1 << 6,
		kWorldFlag_WaterAdjustment =	1 << 7,

		kParentFlag_UseLandData =		1 << 0,
		kParentFlag_UseLODData =		1 << 1,
		kParentFlag_UseMapData =		1 << 2,
		kParentFlag_UseWaterData =		1 << 3,
		kParentFlag_UseClimateData =	1 << 4,
		kParentFlag_UseISData =			1 << 5,
	};

	TESFullName				fullName;			// 18
	TESTexture				worldMap;			// 24
	CellPointerMap			*cellMap;			// 30
	TESObjectCELL			*cell;				// 34
	UInt32					unk38;				// 38
	BGSTerrainManager		*lodData;			// 3C
	TESClimate				*climate;			// 40
	TESImageSpace			*imageSpace;		// 44
	ImpactDataSwap			*impactSwap;		// 48
	UInt8					worldFlags;			// 4C
	UInt8					unk4D;				// 4D
	UInt16					parentFlags;		// 4E
	RefListPointerMap		pointerMap;			// 50
	tList<TESObjectREFR>	lst60;				// 60
	tList<void>				lst68;				// 68
	TESWorldSpace			*parent;			// 70
	TESWaterForm			*waterFormFirst;	// 74
	TESWaterForm			*waterFormLast;		// 78
	float					waterLODHeight;		// 7C
	MapData					mapData;			// 80
	float					worldMapScale;		// 90
	float					worldMapCellX;		// 94
	float					worldMapCellY;		// 98
	BGSMusicType			*musicType;			// 9C
	NiPoint2				min;				// A0
	NiPoint2				max;				// A8
	OffsetDataMap			offsetMap;			// B0
	String					editorID;			// C0
	float					defaultLandHeight;	// C8
	float					defaultWaterHeight;	// CC
	BGSEncounterZone		*encounterZone;		// D0
	TESTexture				canopyShadow;		// D4
	TESTexture				waterNoiseTexture;	// E0

	TESObjectCELL* __vectorcall GetCellAtPos(__m128 pos) const;
	TESWorldSpace *GetRootMapWorld() const;
};
static_assert(sizeof(TESWorldSpace) == 0xEC);

// 04
class TESChildCell
{
public:
	virtual TESObjectCELL	*GetPersistentCell();
};

// 38
struct NavMeshBounds
{
	struct BoundsTriangle;

	NiPoint3						point00;		// 00
	NiPoint3						point0C;		// 0C
	BSSimpleArray<BoundsTriangle>	boundsTrigArr;	// 18
	BSSimpleArray<NiPoint3>			pointArr;		// 28
};
static_assert(sizeof(NavMeshBounds) == 0x38);

struct NavMeshInfo;
typedef BSSimpleArray<NavMeshInfo*> NavMeshInfoArray;

// 5C	c'tor @ 0x6B46A0; d'tor @ 0x6B6390
struct NavMeshInfo
{
	enum
	{
		kFlag_Disabled =	0x10,
		kFlag_HasBounds =	0x20
	};

	UInt32				navMeshRefID;		// 00
	UInt32				worldOrCellRefID;	// 04
	UInt32				flags;				// 08
	Coordinate			exteriorCoord;		// 0C
	NiPoint3			approxLocation;		// 10
	union									// 1C
	{
		TESWorldSpace	*parentWorld;
		TESObjectCELL	*parentCell;
	};
	float				preferredPercent;	// 20
	NavMeshInfoArray	array24;			// 24
	NavMeshInfoArray	array34;			// 34
	ObjectRefrArray		connectedDoors;		// 44
	void				*ptr54;				// 54
	NavMeshBounds		*bounds;			// 58
};
static_assert(sizeof(NavMeshInfo) == 0x5C);

// 40
class NavMeshInfoMap : public TESForm
{
public:
	typedef NiTPtrMap<NavMeshInfoArray> WorldInfoArraysMap;

	UInt8							byte18;		// 18
	UInt8							pad19[3];	// 19
	NiTPtrMap<NavMeshInfo>			infoMap;	// 1C	Keys are RefIDs of NavMesh
	NiTPtrMap<WorldInfoArraysMap>	intInfoMap;	// 2C	Keys of main map are RefIDs of TESWorldSpace; keys of the sub-map are Coordinates of cells
													//		One exception is main map key 0 - keys of the sub-map are TESObjectCELL*
	UInt8							byte3C;		// 3C
	UInt8							pad3D[3];	// 3D
};
static_assert(sizeof(NavMeshInfoMap) == 0x40);

// 8C
class ObstacleData : public NiRefObject
{
public:
	UInt32				unk08;			// 08
	NiRefObject			*object0C;		// 0C
	UInt32				unk10[25];		// 10
	UInt8				byte74;			// 74
	UInt8				byte75[3];		// 75
	NavMeshInfoArray	navMeshInfos;	// 78
	NiRefObject			*object88;		// 88
};

struct NavMeshVertex;
struct NavMeshTriangle;
struct EdgeExtraInfo;
struct NavMeshTriangleDoorPortal;
struct NavMeshClosedDoorInfo;
struct NavMeshPOVData;
struct ObstacleUndoData;
struct NavMeshStaticAvoidNode;

// 108
class NavMesh : public TESForm
{
public:
	virtual void		Unk_4E(void);

	TESChildCell								childCell;			// 018
	NiRefObject									refObject;			// 01C
	TESObjectCELL								*parentCell;		// 024
	BSSimpleArray<NavMeshVertex>				vertexArr;			// 028
	BSSimpleArray<NavMeshTriangle>				triangleArr;		// 038
	BSSimpleArray<EdgeExtraInfo>				edgeInfoArr;		// 048
	BSSimpleArray<NavMeshTriangleDoorPortal>	doorPortalArr;		// 058
	BSSimpleArray<NavMeshClosedDoorInfo>		closedDorrArr;		// 068
	BSSimpleArray<UInt16>						unk078Arr;			// 078
	NiTMap<UInt16, NavMeshPOVData*>				povDataMap;			// 088
	BSSimpleArray<UInt16>						unk098Arr;			// 098
	UInt32										unk0A8;				// 0A8
	float										unk0AC[8];			// 0AC
	BSSimpleArray<UInt16>						*arrPtr0CC;			// 0CC
	BSSimpleArray<ObstacleUndoData>				obstacleUndoArr;	// 0D0
	NiTMap<UInt16, ObstacleData*>				*obstacleDataMap;	// 0E0
	BSSimpleArray<UInt16>						unk0E4Arr;			// 0E4
	BSSimpleArray<NavMeshStaticAvoidNode>		avoidNodeArr;		// 0F4
	UInt32										*ptr104;			// 104
};
static_assert(sizeof(NavMesh) == 0x108);

// 2C
class TESObjectLAND : public TESForm
{
public:
	// A4
	struct LandData
	{
		//	Note: All arrays in the structs are of 289 elements.
		struct Geometry
		{
			NiVector3		*quad0Vertices;
			NiVector3		*quad1Vertices;
			NiVector3		*quad2Vertices;
			NiVector3		*quad3Vertices;
		};

		struct Struct08
		{
			NiVector3		*quad0Unk;
			NiVector3		*quad1Unk;
			NiVector3		*quad2Unk;
			NiVector3		*quad3Unk;
		};

		struct Struct0C
		{
			NiVector4		*quad0Unk;
			NiVector4		*quad1Unk;
			NiVector4		*quad2Unk;
			NiVector4		*quad3Unk;
		};

		struct Struct10
		{
			UInt8			*quad0Unk;
			UInt8			*quad1Unk;
			UInt8			*quad2Unk;
			UInt8			*quad3Unk;
		};

		struct Struct30
		{
			UInt32			unk00;
			UInt32			unk04;
			UInt32			unk08;
			UInt32			unk0C;
			UInt32			unk10;
			UInt32			unk14;
		};

		struct GrassAreaParam;
		typedef NiTPtrMap<GrassAreaParam*> GrassAreaParamMap;

		void					*ptr00;			// 00
		Geometry				*geometry;		// 04
		Struct08				*ptr08;			// 08
		Struct0C				*ptr0C;			// 0C
		void					*ptr10;			// 10
		NiObject				*object14;		// 14
		float					minHeight;		// 18
		float					maxHeight;		// 1C
		TESLandTexture			*textures20[4];	// 20
		Struct30				*ptrs30[4];		// 30
		void					**ptrs40[4];	// 40
		UInt32					unk50;			// 50
		GrassAreaParamMap		grassParams54;	// 54
		GrassAreaParamMap		grassParams64;	// 64
		GrassAreaParamMap		grassParams74;	// 74
		GrassAreaParamMap		grassParams84;	// 84
		NiObject				*object94;		// 94
		SInt32					cellCoordX;		// 98
		SInt32					cellCoordY;		// 9C
		float					meanHeight;		// A0
	};

	TESChildCell		childCell;		// 18
	UInt32				landFlags;		// 1C
	TESObjectCELL		*cell;			// 20
	QueuedFile			*queuedFile;	// 24
	LandData			*landData;		// 28
};
static_assert(sizeof(TESObjectLAND) == 0x2C);

struct VariableInfo
{
	UInt32			idx;		// 00
	UInt32			pad04;		// 04
	double			data;		// 08
	UInt8			type;		// 10
	UInt8			pad11[3];	// 11
	UInt32			unk14;		// 14
	String			name;		// 18
};

struct QuestLogEntry;

// TESQuest (6C)
class TESQuest : public TESForm
{
public:
	virtual char *GetEditorName() const;

	TESScriptableForm		scriptable;			// 18
	TESIcon					icon;				// 24
	TESFullName				fullName;			// 30

	struct StageInfo
	{
		UInt8					stage;		// 00 stageID
		UInt8					isDone;		// 01 status ?
		UInt8					pad02[2];	// 02
		tList<QuestLogEntry>	logEntries;	// 04
	};

	enum QuestFlag
	{
		kFlag_IsRunning =			1,
		kFlag_Completed =			2,
		kFlag_AllowRepeatedTopics =	4,
		kFlag_AllowRepeatedStages =	8,
		kFlag_RunAfterReset =		0x10,
		kFlag_StartGameEnabled =	kFlag_IsRunning | kFlag_RunAfterReset,
		kFlag_IsShownInPipboy =		0x20,
		kFlag_Failed =				0x40
	};

	UInt8					questFlags;			// 3C
	UInt8					priority;			// 3D
	UInt8					pad3E[2];			// 3E
	float					questDelayTime;		// 40
	tList<StageInfo>		stages;				// 44
	tList<void>				lVarOrObjectives;	// 4C
		// So: this list would contain both Objectives and LocalVariables !
		// That seems very strange but still, looking at Get/SetObjective... and ShowQuestVars there's no doubt.
	ConditionList			conditions;			// 54
	ScriptLocals			*scriptEventList;	// 5C
	UInt8					currentStage;		// 60
	UInt8					pad61[3];			// 61
	String					editorName;			// 64

	__forceinline bool SetStage(UInt8 stageID)
	{
		return ThisCall<bool>(0x60D510, this, stageID);
	}
	BGSQuestObjective *GetObjective(UInt32 objectiveID) const;

	size_t DecompileResultScripts(FILE *pStream, char *pBuffer);
};
static_assert(sizeof(TESQuest) == 0x6C);

class TESPackageData
{
public:
	virtual void	Destroy(bool free);
	virtual void	CopyFrom(TESPackageData * packageData);
	virtual void	Unk_02(void);
	virtual void	Save(ModInfo* modInfo);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
};

class TESPatrolPackageData : public TESPackageData
{
public:
	UInt8	patrolFlags;
};

enum
{
	kPackageFlag_OffersServices =			1 << 0,
	kPackageFlag_MustReachLocation =		1 << 1,
	kPackageFlag_MustComplete =				1 << 2,
	kPackageFlag_LockDoorsAtStart =			1 << 3,
	kPackageFlag_LockDoorsAtEnd =			1 << 4,
	kPackageFlag_LockDoorsAtLocation =		1 << 5,
	kPackageFlag_UnlockDoorsAtStart =		1 << 6,
	kPackageFlag_UnlockDoorsAtEnd =			1 << 7,
	kPackageFlag_UnlockDoorsAtLocation =	1 << 8,
	kPackageFlag_ContinueIfPCNear =			1 << 9,
	kPackageFlag_OncePerDay =				1 << 10,
	kPackageFlag_Unk11 =					1 << 11,
	kPackageFlag_SkipFalloutBehavior =		1 << 12,
	kPackageFlag_AlwaysRun =				1 << 13,
	kPackageFlag_Unk14 =					1 << 14,
	kPackageFlag_NeverRun =					1 << 15,
	kPackageFlag_Unk16 =					1 << 16,
	kPackageFlag_AlwaysSneak =				1 << 17,
	kPackageFlag_AllowSwimming =			1 << 18,
	kPackageFlag_AllowFalls =				1 << 19,
	kPackageFlag_ArmorUnequipped =			1 << 20,
	kPackageFlag_WeaponsUnequipped =		1 << 21,
	kPackageFlag_DefensiveCombat =			1 << 22,
	kPackageFlag_WeaponsDrawn =				1 << 23,
	kPackageFlag_NoIdleAnims =				1 << 24,
	kPackageFlag_PretendInCombat =			1 << 25,
	kPackageFlag_ContinueDuringCombat =		1 << 26,
	kPackageFlag_NoCombatAlert =			1 << 27,
	kPackageFlag_NoWarnAttackBehavior =		1 << 28,
	kPackageFlag_AlwaysWalk =				1 << 29,	//	JIP LN
	kPackageFlag_Unk30 =					1 << 30,
	kPackageFlag_Unk31 =					1 << 31
};

// 04
class ActorPackageData
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(TESObjectREFR *refr);
	virtual UInt32	GetPackageType();
	virtual void	SaveGame(BGSSaveLoadGame *saveLoadGame);
	virtual void	LoadGame(BGSSaveLoadGame *saveLoadGame);
	virtual void	Unk_05(UInt32 arg);
};

// 64
class SandBoxActorPackageData : public ActorPackageData
{
public:
	struct SandboxChoice
	{
		TESObjectREFR	*markerRef;		// 00
		UInt32			unk04;			// 04
		UInt32			unk08;			// 08
		UInt32			unk0C;			// 0C
	};

	UInt32							unk04;		// 04
	UInt32							unk08;		// 08
	float							flt0C;		// 0C
	UInt32							unk10;		// 10
	float							flt14;		// 14
	float							flt18;		// 18
	UInt32							unk1C;		// 1C
	BSSimpleArray<SandboxChoice>	choices;	// 20
	UInt32							unk30;		// 30
	UInt8							byte34;		// 34
	UInt8							byte35;		// 35
	UInt8							byte36;		// 36
	UInt8							byte37;		// 37
	UInt32							unk38;		// 38
	UInt32							unk3C;		// 3C
	UInt8							byte40;		// 40
	UInt8							pad41[3];	// 41
	UInt32							unk44;		// 44
	UInt32							unk48;		// 48
	UInt32							unk4C;		// 4C
	float							flt50;		// 50
	float							flt54;		// 54
	UInt32							unk58;		// 58
	UInt8							byte5C[6];	// 5C
	UInt8							pad62[2];	// 62
};
static_assert(sizeof(SandBoxActorPackageData) == 0x64);

// 18
class EscortActorPackageData : public ActorPackageData
{
public:
	UInt32				unk04;		// 04
	UInt32				unk08;		// 08
	UInt32				unk0C;		// 0C
	tList<void>			list10;		// 10
};

// 14
class GuardActorPackageData : public ActorPackageData
{
public:
	UInt32				unk04;		// 04
	UInt32				unk08;		// 08
	UInt32				unk0C;		// 0C
	float				flt10;		// 10
};

// 38
class PatrolActorPackageData : public ActorPackageData
{
public:
	UInt32				unk04;		// 04
	UInt32				unk08;		// 08
	UInt32				unk0C;		// 0C
	UInt32				unk10;		// 10
	UInt32				unk14;		// 14
	UInt32				unk18;		// 18
	ObjectRefrArray		pointRefs;	// 1C
	UInt32				unk2C[3];	// 2C
};
static_assert(sizeof(PatrolActorPackageData) == 0x38);

// 08
class UseWeaponActorPackageData : public ActorPackageData
{
public:
	UInt32				unk04;		// 04
};

// TESPackage (80)
class TESPackage : public TESForm
{
public:
	virtual void	Unk_4E();
	virtual void	Unk_4F();
	virtual void	Unk_50();
	virtual void	Unk_51();
	virtual void	Unk_52();
	virtual void	Unk_53();
	virtual void	Unk_54();
	virtual void	Unk_55();
	virtual void	Unk_56();
	virtual void	Unk_57();

	enum
	{
		kPackageType_Explore,
		kPackageType_Follow,
		kPackageType_Escort,
		kPackageType_Eat,
		kPackageType_Sleep,
		kPackageType_Wander,
		kPackageType_Travel,
		kPackageType_Accompany,
		kPackageType_UseItemAt,
		kPackageType_Ambush,
		kPackageType_FleeNotCombat,
		kPackageType_CastMagic,
		kPackageType_Sandbox,
		kPackageType_Patrol,
		kPackageType_Guard,
		kPackageType_Dialogue,
		kPackageType_UseWeapon,
		kPackageType_Find,
		kPackageType_Combat,
		kPackageType_CombatLow,
		kPackageType_Activate,
		kPackageType_Alarm,
		kPackageType_Flee,
		kPackageType_Trespass,
		kPackageType_Spectator,
		kPackageType_ReactToDead,
		kPackageType_GetUp,
		kPackageType_DoNothing,
		kPackageType_InGameDialogue,
		kPackageType_Surface,
		kPackageType_SearchForAttacker,
		kPackageType_AvoidRadiation,
		kPackageType_ReactToDestroyedObject,
		kPackageType_ReactToGrenadeOrMine,
		kPackageType_StealWarning,
		kPackageType_PickpocketWarning,
		kPackageType_MovementBlocked,
		kPackageType_Sandman,
		kPackageType_Cannibal,
		kPackType_MAX
	};

	// 8
	struct PackageTime
	{
		enum
		{
			kDay_Any = 0,
			kTime_Any = 0xFF,
		};

		enum
		{
			kMonth_January = 0,
			kMonth_February,
			kMonth_March,
			kMonth_April,
			kMonth_May,
			kMonth_June,
			kMonth_July,
			kMonth_August,
			kMonth_September,
			kMonth_October,
			kMonth_November,
			kMonth_December,
			kMonth_Spring,	// march, april, may
			kMonth_Summer,	// june, july, august
			kMonth_Autumn,	// september, august, november (in Geck)
			kMonth_Winter,	// december, january, february

			kMonth_Any = 0xFF,
		};

		enum
		{
			kWeekday_Sundays = 0,
			kWeekday_Morndays,
			kWeekday_Tuesdays,
			kWeekday_Wednesdays,
			kWeekday_Thursdays,
			kWeekday_Frydays,
			kWeekday_Saturdays,
			kWeekday_Weekdays,
			kWeekday_Weekends,
			kWeekday_MWF,
			kWeekday_TT,

			kWeekday_Any = 0xFF
		};

		UInt8	month;
		UInt8	weekDay;
		UInt8	date;
		UInt8	time;
		UInt32	duration;

		static bool IsValidMonth(UInt8 m) { return (m+1) <= kMonth_Winter; }
		static bool IsValidTime(UInt8 t) { return (t+1) <= 24; }
		static bool IsValidDay(UInt8 d) { return (d+1) <= kWeekday_TT; }
		static bool IsValidDate(UInt8 d) { return d <= 31; }
	};

	union ObjectType
	{
		TESForm			* form;
		TESObjectREFR	* refr;
		UInt32			objectCode;
	};

	// order only somewhat related to kFormType_XXX (values off by 17, 20, or 21)
	enum	// From OBSE and FNVEdit
	{	
		kObjectType_None	=	0,
		kObjectType_Activators,
		kObjectType_Armor,
		kObjectType_Books,
		kObjectType_Clothing,
		kObjectType_Containers,
		kObjectType_Doors,
		kObjectType_Ingredients,
		kObjectType_Lights,
		kObjectType_Misc,
		kObjectType_Flora,
		kObjectType_Furniture,
		kObjectType_WeaponsAny,
		kObjectType_Ammo,
		kObjectType_NPCs,
		kObjectType_Creatures,
		kObjectType_Keys,				//	10
		kObjectType_Alchemy,
		kObjectType_Food,
		kObjectType_AllCombatWearable,
		kObjectType_AllWearable,
		kObjectType_WeaponsRanged,
		kObjectType_WeaponsMelee,
		kObjectType_WeaponsNone,
		kObjectType_ActorEffectAny,
		kObjectType_ActorEffectRangeTarget,
		kObjectType_ActorEffectRangeTouch,
		kObjectType_ActorEffectRangeSelf,
		kObjectType_ActorsAny,

		kObjectType_Max,						//	1E
	};

	struct LocationData
	{
		enum {
			kPackLocation_NearReference		= 0,
			kPackLocation_InCell			= 1,
			kPackLocation_CurrentLocation	= 2,
			kPackLocation_EditorLocation	= 3,
			kPackLocation_ObjectID			= 4,
			kPackLocation_ObjectType		= 5,
			kPackLocation_LinkedReference	= 6,

			kPackLocation_Max,
		};

		UInt8		locationType;	
		UInt8		pad[3];
		UInt32		radius;
		ObjectType  object;

		static bool IsValidLocationType(UInt8 locCode) { return locCode < kPackLocation_Max; }
	};

	enum
	{
		kTargetType_Refr		= 0,
		kTargetType_BaseObject	= 1,
		kTargetType_TypeCode	= 2,
		
		kTargetType_Max	= 3,
	};

	struct TargetData 
	{
		UInt8		targetType;	// 00
		UInt8		pad[3];		// 01
		ObjectType	target;		// 04
		UInt32		count;		// 08 can be distance too
		float		unk0C;		// 0C
	};


	enum eProcedure {			// UInt32	// Checked the Geck Wiki. Not consistent with s_procNames (which has a diffferent order and 0x37 procedures)
		kProcedure_TRAVEL = 0,
		kProcedure_ACTIVATE,
		kProcedure_ACQUIRE,
		kProcedure_WAIT,
		kProcedure_DIALOGUE,
		kProcedure_GREET,
		kProcedure_GREET_DEAD,
		kProcedure_WANDER,
		kProcedure_SLEEP,
		kProcedure_OBSERVE_COMBAT,
		kProcedure_EAT,
		kProcedure_FOLLOW,
		kProcedure_ESCORT,
		kProcedure_COMBAT,
		kProcedure_ALARM,
		kProcedure_PURSUE,
		kProcedure_FLEE,					// 0x10
		kProcedure_DONE,
		kProcedure_YELD,
		kProcedure_TRAVEL_TARGET,
		kProcedure_CREATE_FOLLOW,
		kProcedure_GET_UP,
		kProcedure_MOUNT_HORSE,
		kProcedure_DISMOUNT_HORSE,
		kProcedure_DO_NOTHING,
		kProcedure_UNK019,
		kProcedure_UNK01A,
		kProcedure_ACCOMPANY,
		kProcedure_USE_ITEM_AT,
		kProcedure_SANDMAN,
		kProcedure_WAIT_AMBUSH,
		kProcedure_SURFACE,					// 0x20
		kProcedure_WAIT_FOR_SPELL,
		kProcedure_CHOOSE_CAST,
		kProcedure_FLEE_NON_COMBAT,
		kProcedure_REMOVE_WORN_ITEMS,
		kProcedure_SEARCH,
		kProcedure_CLEAR_MOUNT_POSITION,
		kProcedure_SUMMON_CREATURE_DEFEND,
		kProcedure_AVOID_AREA,
		kProcedure_UNEQUIP_ARMOR,
		kProcedure_PATROL,
		kProcedure_USE_WEAPON,
		kProcedure_DIALOGUE_ACTIVATE,
		kProcedure_GUARD,
		kProcedure_SANDBOX,
		kProcedure_USE_IDLE_MARKER,
		kProcedure_TAKE_BACK_ITEM,
		kProcedure_SITTING,					// 0x30
		kProcedure_MOVEMENT_BLOCKED,
		kProcedure_CANIBAL_FEED,			// 0x32

		kProcedure_MAX						// 0x33
	};

	// 10
	struct PackageEvent
	{
		TESIdleForm		*idleForm;	// 00
		Script			*resScript;	// 04
		TESTopic		*topic;		// 08
		UInt32			unk0C;		// 0C
	};

	// In DialoguePackage, there are 0x58 virtual functions (including 0x4E from TESForm)

	UInt32			procedureArrayIndex;	// 18 index into array of array of eProcedure terminated by 0x2C. -1 if no procedure array exists for package type.
	UInt32			packageFlags;			// 1C
	char			type;					// 20
	UInt8			byte21;					// 21
	UInt16			behaviorFlags;			// 22
	UInt32			specificFlags;			// 24
	TESPackageData	*packageData;			// 28
	LocationData	*location;				// 2C
	TargetData		*target;				// 30
	UInt32			unk34;					// 34	idles
	PackageTime		time;					// 38
	ConditionList	conditions;				// 40
	TESCombatStyle	*combatStyle;			// 48
	PackageEvent	onBegin;				// 4C
	PackageEvent	onEnd;					// 5C
	PackageEvent	onChange;				// 6C
	UInt32			unk7C;					// 7C

	__forceinline bool GetFleeCombat()
	{
		return ThisCall<bool>(0x981990, this);
	}

	static bool IsValidObjectCode(UInt8 o) { return o < kObjectType_Max; }

	size_t DecompileResultScripts(FILE *pStream, char *pBuffer);
};
static_assert(sizeof(TESPackage) == 0x80);

// DialoguePackage : Only package tested and verified effectivly
class DialoguePackage : public TESPackage
{
public:
	UInt32 unk080[(0x8C - 0x80) >> 2];	// 080
	TESTopic	* topic;		// 08C
	UInt32		unk090;			// 090
	Character	* speaker;		// 094
	UInt8		unk098;			// 098
	UInt8		unk099;			// 098
	UInt8		unk09A;			// 098
	UInt8		unk09B;			// 098
	TESForm		* unk09C;		// 09C
	UInt32		unk0A0;			// 0A0
	void *		unk0A4;			// 0A4	list of Dialogue Item and Dialogue Response, plus current item and current response
	UInt32		unk0A8;			// 0A8
	UInt32		unk0AC;			// 0AC
	Character	* subject;		// 0B0
	Character	* target;		// 0B4
	TESForm		* unk0B8;		// 0B8
	UInt8		unk0BC;			// 0BC
	UInt8		unk0BD;			// 0BD
	UInt8		unk0BE;			// 0BE
	UInt8		unk0BF;			// 0BF
	UInt8		unk0C0;			// 0C0
	UInt8		unk0C1;			// 0C1
	UInt8		unk0C2;			// 0C2
	UInt8		unk0C3;			// 0C3
	UInt32		unk0C4;			// 0C4
	UInt32		unk0C8;			// 0C8
	UInt8		unk0CC;			// OCC
	UInt8		unk0CD[3];		// OCD
};	// 0D0

class FleePackage : public TESPackage
{
public:
	UInt8					bEvaluatePoint;	// 80
	UInt8					bCombatMode;	// 81
	UInt8					pad82[2];		// 82
	NiVector3				fleePoint;		// 84
	float					fleeTimer;		// 90
	UInt8					bFleeSucceeded;	// 94
	UInt8					pad95[3];		// 95
	tList<TESObjectREFR>	avoidRefs;		// 98
	TESObjectREFR			*teleportDoor;	// A0
	TESObjectREFR			*refFleeTo;		// A4
	UInt8					bKnowsTarget;	// A8
	UInt8					byteA9;			// A9
	UInt8					padAA[2];		// AA
};	// 0AC

// 8C
class BackUpPackage : public TESPackage
{
public:
	virtual void	Unk_58(void);

	NiVector3		destPos;	// 80
};

class TressPassPackage : public TESPackage
{
public:
	float		unk080;		// 080
	UInt32		unk084;		// 084
	TESForm		* unk088;	// 088
	TESForm		* unk08C;	// 08C
	UInt32		unk090;		// 090
	UInt32		unk094;		// 094
	UInt32		unk098;		// 098
};	// 09C

struct SpectatorThreatInfo
{
	TESForm			* unk000;	// 000
	TESForm			* unk004;	// 004
	UInt32			unk008;		// 008
	UInt32			unk00C;		// 00C	elapsed tick count
	UInt32			unk010;		// 010
	NiVector3		unk014;		// 014	is array of 3 floats, should be Pos
	NiVector3		unk020;		// 020	is array of 3 floats, should be Rot
	UInt8			unk02C;		// 02C
	UInt8			unk02D;		// 02D
	UInt8			pad[2];		// 02E
};	// 030

class SpectatorPackage : public TESPackage
{
public:
	UInt32			unk080;		// 080
	UInt32			unk084;		// 084
	UInt32			unk088;		// 088
	UInt32			unk08C;		// 08C
	UInt8			unk090;		// 090
	UInt8			pad091[3];	// 091
	NiVector3		unk094;		// 094	is array of 3 floats, should be Pos
	BSSimpleArray<SpectatorThreatInfo>	arr0A0;	// 0A0
	// There is an object containing a semaphore at B0/B4
};	// 0B4

class TESFollowPackageData : public TESPackageData
{
public:
	TESPackage::LocationData* endLocation;
	float	flt008;
};

// 24
class TESUseWeaponPackageData : public TESPackageData
{
public:
	TESPackage::TargetData	*targetData;	// 04
	UInt32					unk08;			// 08
	bool					autoHit;		// 0C
	bool					noDamage;		// 0D
	bool					crouchReload;	// 0E
	bool					holdWhenBlocked;// 0F
	UInt8					byte10;			// 10	1
	UInt8					byte11;			// 11	0
	UInt16					word12;			// 12
	UInt16					useTimes0;		// 14
	UInt16					useTimes1;		// 16
	float					flt18;			// 18
	float					flt1C;			// 1C
	UInt32					unk20;			// 20
};

// 108
class TESCombatStyle : public TESForm
{
public:
	enum
	{
		kFlag_ChooseAttackUsingChance		= 1,
		kFlag_MeleeAlertOK					= 2,
		kFlag_FleeBasedOnPersonalSurvival	= 4,
		kFlag_IgnoreThreats					= 16,
		kFlag_IgnoreDamagingSelf			= 32,
		kFlag_IgnoreDamagingGroup			= 64,
		kFlag_IgnoreDamagingSpectators		= 128,
		kFlag_CannotUseStealthboy			= 256,
	};

	float	coverSearchRadius;				// 018
	float	takeCoverChance;				// 01C
	float	waitTimeMin;					// 020
	float	waitTimeMax;					// 024
	float	waitToFireTimerMin;				// 028
	float	waitToFireTimerMax;				// 02C
	float	fireTimerMin;					// 030
	float	fireTimerMax;					// 034
	float	rangedWeapRangeMultMin;			// 038
	UInt8	pad3C[4];						// 03C
	UInt8	weaponRestrictions;				// 040
	UInt8	pad41[3];						// 041
	float	rangedWeapRangeMultMax;			// 044
	float	maxTargetingFOV;				// 048
	float	combatRadius;					// 04C
	float	semiAutoFiringDelayMultMin;		// 050
	float	semiAutoFiringDelayMultMax;		// 054
	UInt8	dodgeChance;					// 058
	UInt8	LRChance;						// 059
	UInt8	pad5A[2];						// 05A
	float	dodgeLRTimerMin;				// 05C
	float	dodgeLRTimerMax;				// 060
	float	dodgeFWTimerMin;				// 064
	float	dodgeFWTimerMax;				// 068
	float	dodgeBKTimerMin;				// 06C
	float	dodgeBKTimerMax;				// 070
	float	idleTimerMin;					// 074
	float	idleTimerMax;					// 078
	UInt8	blockChance;					// 07C
	UInt8	attackChance;					// 07D
	UInt8	pad7E[2];						// 07E
	float	staggerBonusToAttack;			// 080
	float	KOBonusToAttack;				// 084
	float	H2HBonusToAttack;				// 088
	UInt8	powerAttackChance;				// 08C
	UInt8	pad8D[3];						// 08D
	float	staggerBonusToPower;			// 090
	float	KOBonusToPower;					// 094
	UInt8	powerAttackN;					// 098
	UInt8	powerAttackF;					// 099
	UInt8	powerAttackB;					// 09A
	UInt8	powerAttackL;					// 09B
	UInt8	powerAttackR;					// 09C
	UInt8	pad9D[3];						// 09D
	float	holdTimerMin;					// 0A0
	float	holdTimerMax;					// 0A4
	UInt16	csFlags;						// 0A8
	UInt8	pad0AA[2];						// 0AA
	UInt8	acrobaticDodgeChance;			// 0AC
	UInt8	rushAttackChance;				// 0AD
	UInt8	pad0AE[2];						// 0AE
	float	rushAttackDistMult;				// 0B0
	float	dodgeFatigueModMult;			// 0B4
	float	dodgeFatigueModBase;			// 0B8
	float	encumSpeedModBase;				// 0BC
	float	encumSpeedModMult;				// 0C0
	float	dodgeUnderAttackMult;			// 0C4
	float	dodgeNotUnderAttackMult;		// 0C8
	float	dodgeBackUnderAttackMult;		// 0CC
	float	dodgeBackNotUnderAttackMult;	// 0D0
	float	dodgeFWAttackingMult;			// 0D4
	float	dodgeFWNotAttackingMult;		// 0D8
	float	blockSkillModMult;				// 0DC
	float	blockSkillModBase;				// 0E0
	float	blockUnderAttackMult;			// 0E4
	float	blockNotUnderAttackMult;		// 0E8
	float	attackSkillModMult;				// 0EC
	float	attackSkillModBase;				// 0F0
	float	attackUnderAttackMult;			// 0F4
	float	attackNotUnderAttackMult;		// 0F8
	float	attackDuringBlockMult;			// 0FC
	float	powerAttackFatigueModBase;		// 100
	float	powerAttackFatigueModMult;		// 104

	void SetFlag(UInt32 pFlag, bool bEnable)
	{
		if (bEnable) csFlags |= pFlag;
		else csFlags &= ~pFlag;
	}
};
static_assert(sizeof(TESCombatStyle) == 0x108);

// 28
class TESRecipeCategory : public TESForm
{
public:
	TESFullName			fullName;		// 18

	UInt32				categoryFlags;	// 24
};
static_assert(sizeof(TESRecipeCategory) == 0x28);

struct RecipeComponent
{
	UInt32		quantity;
	TESForm		*item;
};

struct NVSEArrayVarInterface::Array;

// 5C
class TESRecipe : public TESForm
{
public:
	struct ComponentList : tList<RecipeComponent>
	{
		NVSEArrayVarInterface::Array *GetComponents(Script *scriptObj);
		void AddComponent(TESForm *form, UInt32 quantity);
		UInt32 RemoveComponent(TESForm *form);
		void ReplaceComponent(TESForm *form, TESForm *replace);
		UInt32 GetQuantity(TESForm *form);
		void SetQuantity(TESForm *form, UInt32 quantity);
	};

	TESFullName				fullName;		// 18

	UInt32					reqSkill;		// 24
	UInt32					reqSkillLevel;	// 28
	UInt32					categoryID;		// 2C
	UInt32					subCategoryID;	// 30
	ConditionList			conditions;		// 34
	ComponentList			inputs;			// 3C
	ComponentList			outputs;		// 44
	UInt32					unk4C;			// 4C
	UInt32					unk50;			// 50
	TESRecipeCategory		*category;		// 54
	TESRecipeCategory		*subCategory;	// 58
};

static_assert(sizeof(TESRecipe) == 0x5C);

class TESLoadScreenType : public TESForm
{
public:
	enum ScreenType
	{
		kScrType_None,
		kScrType_XP_Progress,
		kScrType_Objective,
		kScrType_Tip,
		kScrType_Stats
	};

	UInt32			type;			// 18
	// Data 1
	UInt32			x;				// 1C
	UInt32			y;				// 20
	UInt32			width;			// 24
	UInt32			height;			// 28
	UInt32			orientation;	// 2C
	UInt32			font1;			// 30
	NiColor			fontColor1;		// 34
	UInt32			justification;	// 40
	UInt32			unk044[5];		// 44
	// Data 2
	UInt32			font2;			// 58
	NiColor			fontColor2;		// 5C
	UInt32			unk068;			// 68
	UInt32			stats;			// 6C
};

// 40
class TESLoadScreen : public TESForm
{
public:
	struct Location
	{
		UInt32		interiorRefID;	// 00	Used if cell is an interior.
		UInt32		worldRefID;		// 04	Used if cell is an exterior, with grid coords
		Coordinate	gridCoords;		// 08
	};

	TESTexture			texture;		// 18
	TESDescription		description;	// 24	Appears to be unused
	tList<Location>		locations;		// 2C
	TESLoadScreenType	*type;			// 34
	String				screenText;		// 38
};
static_assert(sizeof(TESLoadScreen) == 0x40);

// TESLevSpell (44)
class TESLevSpell;

// TESObjectANIO (3C)
class TESObjectANIO : public TESForm
{
public:
	TESModelTextureSwap	modelSwap;		// 18
	TESIdleForm			*idleForm;		// 38
};

// 194
class TESWaterForm : public TESForm
{
public:
	union WaterValue
	{
		float	f;
		UInt32	u;
	};

	TESFullName				fullName;			// 018
	TESAttackDamageForm		attackDamage;		// 024
	bool					needUpdate;			// 02C
	UInt8					pad02D[3];			// 02D
	BSRenderedTexture		*normalNoiseTexture;// 030
	NiColorAlpha			texScroll[3];		// 034
	TESTexture				noiseMap;			// 064
	UInt8					opacity;			// 070 ANAM
	UInt8					waterFlags;			// 071 FNAM (0x01: causes damage, 0x02: reflective)
	UInt8					unk072[2];			// 072
	UInt32					unk074[2];			// 074
	TESSound				*sound;				// 07C
	TESWaterForm			*waterForm;			// 080
	WaterValue				visData[49];		// 084
	TESWaterForm			*wtrWeatherCtrl[3];	// 148
	SInt32					unk154[2];			// 154
	UInt32					frequencyX;			// 15C
	UInt32					frequencyY;			// 160
	int						octaves;			// 164
	float					amplitude;			// 168
	float					lacunarity;			// 16C
	float					bias;				// 170
	float					gain;				// 174
	SpellItem				*drinkEffect;		// 178
	NiSourceTexture			*noiseTexture;		// 17C
	UInt32					unk180[2];			// 180
	UInt8					radiation;			// 188
	UInt8					pad189[3];			// 189
	UInt32					unk18C;				// 18C
	bool					resetNoiseTexture;	// 190
	UInt8					pad191[3];			// 191
};
static_assert(sizeof(TESWaterForm) == 0x194);

// 134
struct EffectShaderData
{
	UInt8		flags;				// 000
	UInt8		pad01[3];			// 001
	UInt32		membraneSourceBlendMode;	// 004
	UInt32		membraneBlendOp;	// 008
	UInt32		membraneZTestFunc;	// 00C
	UInt32		fillTextureRGB;		// 010
	float		fillTextureAlphaFadeInTime;	// 014
	float		fillTextureFullAlphaTime;	// 018
	float		fillTextureAlphaFadeOutTime;// 01C
	float		fillTexturePersistentAlphaRatio;// 020
	float		fillTextureAlphaPulseAmpl;	// 024
	float		fillTextureAlphaPulseFreq;	// 028
	float		fillTextureAnimSpeedU;	// 02C
	float		fillTextureAnimSpeedV;	// 030
	float		edgeFallOff;		// 034
	ColorRGBA	edgeColor1;			// 038
	float		edgeAlphaFadeInTime;// 03C
	float		edgeFullAlphaTime;	// 040
	float		edgeAlphaFadeOutTime;	// 044
	float		edgePersistentAlphaRatio;	// 048
	float		edgeAlphaPulseAmpl;	// 04C
	float		edgeAlphaPulseFreq;	// 050
	float		fillTextureFullAlphaRatio;	// 054
	float		edgeFullAlphaRatio;	// 058
	UInt32		membraneDestBlendMode;	// 05C
	UInt32		particleSourceBlendMode;// 060
	UInt32		particleBlendOp;	// 064
	UInt32		particleZTestFunc;	// 068
	UInt32		particleDestBlendMode;	// 06C
	float		particleBirthRampUpTime;// 070
	float		particleBirthFullTime;	// 074
	float		particleBirthRampDownTime;	// 078
	float		particleBirthFullRatio;	// 07C
	float		particleBirthPersistRatio;	// 080
	float		particleLifetime;	// 084
	float		particleLifetimeVar;// 088
	float		particleInitSpeedAlongNormal;	// 08C
	float		particleAccelAlongNormal;	// 090
	NiVector3	initialVelocity;	// 094
	NiVector3	acceleration;		// 0A0
	float		scaleKey1;			// 0AC
	float		scaleKey2;			// 0B0
	float		scaleKey1Time;		// 0B4
	float		scaleKey2Time;		// 0B8
	ColorRGBA	colorKey1;			// 0BC
	ColorRGBA	colorKey2;			// 0C0
	ColorRGBA	colorKey3;			// 0C4
	float		colorKey1Alpha;		// 0C8
	float		colorKey2Alpha;		// 0CC
	float		colorKey3Alpha;		// 0D0
	float		colorKey1Time;		// 0D4
	float		colorKey2Time;		// 0D8
	float		colorKey3Time;		// 0DC
	float		particleInitSpeedAlongNormalVar;// 0E0
	float		particleInitRotDeg;	// 0E4
	float		particleInitRotDegVar;	// 0E8
	float		particleRotSpeedDegPerSec;	// 0EC
	float		particleRotSpeedDegPerSecVar;	// 0F0
	BGSDebris	*addonModels;		// 0F4
	float		holesStartTime;		// 0F8
	float		holesEndTime;		// 0FC
	float		holesStartVal;		// 100
	float		holesEndVal;		// 104
	float		edgeWidthAlphaUnits;// 108
	ColorRGBA	edgeColor2;			// 10C
	float		explosionWindSpeed;	// 110
	UInt32		textureCountU;		// 114
	UInt32		textureCountV;		// 118
	float		addonFadeInTime;	// 11C
	float		addonFadeOutTime;	// 120
	float		addonScaleStart;	// 124
	float		addonScaleEnd;		// 128
	float		addonScaleInTime;	// 12C
	float		addonScaleOutTime;	// 130
};

// 170
class TESEffectShader : public TESForm
{
public:
	EffectShaderData	shaderData;				// 018
	TESTexture			fillTexture;			// 14C
	TESTexture			particleShaderTexture;	// 158
	TESTexture			holesTexture;			// 164
};
static_assert(sizeof(TESEffectShader) == 0x170);

// A8
class BGSExplosion : public TESBoundObject
{
public:
	enum
	{
		kFlags_Unknown =					1,
		kFlags_AlwaysUseWorldOrientation =	2,
		kFlags_KnockDownAlways =			4,
		kFlags_KnockDownByFormula =			8,
		kFlags_IgnoreLOSCheck =				0x10,
		kFlags_PushSourceRefOnly =			0x20,
		kFlags_IgnoreImageSpaceSwap =		0x40
	};

	TESFullName					fullName;			// 30
	TESModel					model;				// 3C
	TESEnchantableForm			enchantable;		// 54
	BGSPreloadable				preloadable;		// 64
	TESImageSpaceModifiableForm	imageSpaceModForm;	// 68

	TESForm						*placedObj;			// 70
	float						force;				// 74
	float						damage;				// 78
	float						radius;				// 7C
	TESObjectLIGH				*light;				// 80
	TESSound					*sound1;			// 84
	UInt32						explFlags;			// 88
	float						ISradius;			// 8C
	BGSImpactDataSet			*impactDataSet;		// 90
	TESSound					*sound2;			// 94
	float						RADlevel;			// 98
	float						dissipationTime;	// 9C
	float						RADradius;			// A0
	UInt8						soundLevel;			// A4	0 - Loud, 1 - Normal, 2 - Silent
	UInt8						padA5[3];			// A5

	void SetFlag(UInt32 pFlag, bool bEnable)
	{
		if (bEnable) explFlags |= pFlag;
		else explFlags &= ~pFlag;
	}
};
static_assert(sizeof(BGSExplosion) == 0xA8);

// BGSDebris (24)
class BGSDebris : public TESForm
{
public:
	BGSPreloadable				preloadable;	// 018
	UInt32	unk01C;
	UInt32	unk020;
};

// B0
class TESImageSpace : public TESForm
{
public:
	float		traitValues[33];	// 18
	// 00:	HDR: Eye Adapt Speed
	// 01:	HDR: Blur Radius
	// 02:	HDR: Blur Passes
	// 03:	HDR: Emissive Mult
	// 04:	HDR: Target LUM
	// 05:	HDR: Upper LUM Clamp
	// 06:	HDR: Bright Scale
	// 07:	HDR: Bright Clamp
	// 08:	HDR: LUM Ramp No Tex
	// 09:	HDR: LUM Ramp Min
	// 10:	HDR: LUM Ramp Max
	// 11:	HDR: Sunlight Dimmer
	// 12:	HDR: Grass Dimmer
	// 13:	HDR: Tree Dimmer
	// 14:	HDR: Skin Dimmer
	// 15:	Bloom: Blur Radius
	// 16:	Bloom: Alpha Mult Interior
	// 17:	Bloom: Alpha Mult Exterior
	// 18:	Get Hit: Blur Radius
	// 19:	Get Hit: Blur Damping Contrast
	// 20:	Get Hit: Damping Contrast
	// 21:	Night Eye: Tint: Red
	// 22:	Night Eye: Tint: Green
	// 23:	Night Eye: Tint: Blue
	// 24:	Night Eye: Brightness
	// 25:	Cinematic: Saturation: Value
	// 26:	Cinematic: Contrast: Avg Lum Value
	// 27:	Cinematic: Contrast: Value
	// 28:	Cinematic: Brightness: Value
	// 29:	Cinematic: Tint: Red
	// 30:	Cinematic: Tint: Green
	// 31:	Cinematic: Tint: Blue
	// 32:	Cinematic: Tint: Value
	UInt32		unk9C[5];			// 9C
};
static_assert(sizeof(TESImageSpace) == 0xB0);

// 730
class TESImageSpaceModifier : public TESForm
{
public:
	enum DoFModes
	{
		kMode_Either,
		kMode_Front,
		kMode_Back
	};

	TESSound				*outroSound;		// 018
	TESSound				*introSound;		// 01C
	UInt8					animable;			// 020
	UInt8					pad021[3];			// 021
	float					duration;			// 024
	UInt32					unk028[48];			// 028
	UInt8					radBlurUseTarget;	// 0E8
	UInt8					pad0E9[3];			// 0E9
	float					radialBlurCentreX;	// 0EC
	float					radialBlurCentreY;	// 0F0
	UInt32					unk0F4[3];			// 0F4
	UInt8					DoFUseTarget;		// 100
	UInt8					DoFMode;			// 101
	UInt8					pad102[2];			// 102
	UInt32					unk104[4];			// 104
	NiFloatInterpolator		fltIntrpl1[44];		// 114
	NiColorInterpolator		clrIntrpl[2];		// 534
	NiFloatInterpolator		fltIntrpl2[9];		// 57C
	InterpKey<float>		*data654[44];		// 654
	// 00:	HDR: Eye Adapt Speed (Multiply)
	// 01:	HDR: Eye Adapt Speed (Add)
	// 02:	HDR: Blur Radius (Multiply)
	// 03:	HDR: Blur Radius (Add)
	// 04:	HDR: Skin Dimmer (Multiply)
	// 05:	HDR: Skin Dimmer (Add)
	// 06:	HDR: Emissive Mult (Multiply)
	// 07:	HDR: Emissive Mult (Add)
	// 08:	HDR: Target LUM (Multiply)
	// 09:	HDR: Target LUM (Add)
	// 10:	HDR: Upper LUM Clamp (Multiply)
	// 11:	HDR: Upper LUM Clamp (Add)
	// 12:	HDR: Bright Scale (Multiply)
	// 13:	HDR: Bright Scale (Add)
	// 14:	HDR: Bright Clamp (Multiply)
	// 15:	HDR: Bright Clamp (Add)
	// 16:	HDR: LUM Ramp No Tex (Multiply)
	// 17:	HDR: LUM Ramp No Tex (Add)
	// 18:	HDR: LUM Ramp Min (Multiply)
	// 19:	HDR: LUM Ramp Min (Add)
	// 20:	HDR: LUM Ramp Max (Multiply)
	// 21:	HDR: LUM Ramp Max (Add)
	// 22:	HDR: Sunlight Dimmer (Multiply)
	// 23:	HDR: Sunlight Dimmer (Add)
	// 24:	HDR: Grass Dimmer (Multiply)
	// 25:	HDR: Grass Dimmer (Add)
	// 26:	HDR: Tree Dimmer (Multiply)
	// 27:	HDR: Tree Dimmer (Add)
	// 28:	Bloom: Blur Radius (Multiply)
	// 29:	Bloom: Blur Radius (Add)
	// 30:	Bloom: Alpha Mult Interior (Multiply)
	// 31:	Bloom: Alpha Mult Interior (Add)
	// 32:	Bloom: Alpha Mult Exterior (Multiply)
	// 33:	Bloom: Alpha Mult Exterior (Add)
	// 34:	Cinematic: Saturation (Multiply)
	// 35:	Cinematic: Saturation (Add)
	// 36:	Cinematic: Contrast (Multiply)
	// 37:	Cinematic: Contrast (Add)
	// 38:	Cinematic: Contrast Avg Lum (Multiply)
	// 39:	Cinematic: Contrast Avg Lum (Add)
	// 40:	Cinematic: Brightness (Multiply)
	// 41:	Cinematic: Brightness (Add)
	// 42:	Blur: Blur Radius
	// 43:	Double Vision: Strength
	InterpKey<NiColorAlpha>	*data704[2];		// 704
	InterpKey<float>		*data70C[9];		// 70C
	// 00:	Radial Blur: Strength
	// 01:	Radial Blur: Rampup
	// 02:	Radial Blur: Up Start
	// 03:	Radial Blur: Rampdown
	// 04:	Radial Blur: Down Start
	// 05:	Depth of Field: Strength
	// 06:	Depth of Field: Distance
	// 07:	Depth of Field: Range
	// 08:	Full-Screen Motion Blur: Strength
};
static_assert(sizeof(TESImageSpaceModifier) == 0x730);

// 24
class BGSListForm : public TESForm
{
public:
	tList<TESForm> list;			// 018

	UInt32	numAddedObjects;	// number of objects added via script - assumed to be at the start of the list

	__forceinline void Prepend(TESForm *toAdd) {ThisCall(0x590010, this, toAdd);}

	UInt32 Count() const {
		return list.Count();
	}

	TESForm* GetNthForm(SInt32 n) const {
		return list.GetNthItem(n);
	}

	UInt32 AddAt(TESForm* pForm, SInt32 n)
	{
		SInt32 result = list.Insert(pForm, n);
		if ((result >= 0) && IsAddedObject(n))
			numAddedObjects++;
		return result;
	}

	SInt32 GetIndexOf(TESForm* pForm);

	TESForm *RemoveNthForm(SInt32 n)
	{
		TESForm	*form = list.RemoveNth(n);

		if (form && IsAddedObject(n) && numAddedObjects)
			numAddedObjects--;

		return form;
	}

	TESForm* ReplaceNthForm(SInt32 n, TESForm* pReplaceWith) {
		return list.ReplaceNth(n, pReplaceWith);
	}

	SInt32 RemoveForm(TESForm* pForm);
	SInt32 ReplaceForm(TESForm* pForm, TESForm* pReplaceWith);

	bool IsAddedObject(SInt32 idx)
	{
		return (idx >= 0) && (idx < numAddedObjects);
	}

	void RemoveAll()
	{
		list.RemoveAll();
		numAddedObjects = 0;
	}
};
static_assert(sizeof(BGSListForm) == 0x024);

// 08
class BGSPerkEntry
{
public:
	virtual bool	CheckConditionFilters(UInt32 numFilters, void *argsPtr);
	virtual UInt8	GetFunctionType();
	virtual BGSEntryPointFunctionData	*GetFunctionData();
	virtual void	Fn_03(void);
	virtual UInt32	GetType();		//	0 - Quest; 1 - Ability; 2 - Entry Point
	virtual void	Fn_05(void);
	virtual void	Fn_06(void);
	virtual void	Fn_07(void);
	virtual void	CopyRankAndPriority(BGSPerkEntry *source);
	virtual void	Fn_09(void);
	virtual void	Fn_0A(void);
	virtual void	Fn_0B(void);
	virtual void	AddEntry(Actor *actor, bool forTeammates);
	virtual void	RemoveEntry(Actor *actor, bool forTeammates);

	UInt8				rank;				// 04 +1 for value shown in GECK
	UInt8				priority;			// 05
	UInt16				type;				// 06 (Quest: 0xC24, Ability: 0xB27, Entry Point: 0xD16)
};

// 10
class BGSQuestPerkEntry : public BGSPerkEntry
{
public:
	TESQuest			*quest;				// 08
	UInt8				stage;				// 0C
	UInt8				pad[3];				// 0D
};

// 0C
class BGSAbilityPerkEntry : public BGSPerkEntry
{
public:
	SpellItem			*ability;			// 08
};

class BGSEntryPointFunctionData
{
public:
	virtual void	Fn_00(void);
	virtual void	Fn_01(void);
	virtual void	Fn_02(void);
	virtual void	Fn_03(void);
	virtual void	Fn_04(void);
	virtual void	Fn_05(void);
	virtual void	Fn_06(void);
};

// 08
class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
{
public:
	float		value;		// 04
};

// 0C
class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData
{
public:
	float		value[2];	// 04
};

class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData
{
public:
	TESLevItem		*leveledList;	// 04
};

class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
{
public:
	virtual void		Fn_07(void);

	String				label;				// 04
	Script				*script;			// 0C
	UInt32				flags;				// 10
};

struct EntryPointConditions
{
	ConditionList		tab1;
	ConditionList		tab2;
	ConditionList		tab3;
};

// 14
class BGSEntryPointPerkEntry : public BGSPerkEntry
{
public:
	UInt8						entryPoint;		// 08
	UInt8						function;		// 09
	UInt8						conditionTabs;	// 0A
	UInt8						pad0B;			// 0B
	BGSEntryPointFunctionData	*data;			// 0C
	EntryPointConditions		*conditions;	// 10
};

// 50
class BGSPerk : public TESForm
{
public:
	struct PerkData
	{
		bool				isTrait;	// 00 (38)
		UInt8				minLevel;	// 01 (39)
		UInt8				numRanks;	// 02 (3A)
		bool				isPlayable;	// 03 (3B)
		bool				isHidden;	// 04 (3C)
		UInt8				unk05;		// 05 (3D)
		UInt8				unk06;		// 06
		UInt8				unk07;		// 07
	};

	TESFullName				fullName;			// 18
	TESDescription			description;		// 24
	TESIcon					icon;				// 2C
	PerkData				data;				// 38
	ConditionList			conditions;			// 40
	tList<BGSPerkEntry>		entries;			// 48
};

class TESCasino : public TESForm
{
public:
	TESFullName				fullName;
	TESModelTextureSwap		chip1;
	TESModelTextureSwap		chip5;
	TESModelTextureSwap		chip10;
	TESModelTextureSwap		chip25;
	TESModelTextureSwap		chip100;
	TESModelTextureSwap		chip500;
	TESModelTextureSwap		chipRoulette;
	TESModelTextureSwap		slotMachine;
	TESModelTextureSwap		blackjackTable;
	TESModelTextureSwap		rouletteTable;
	TESIcon					slotReel[7];
	TESTexture				blackjackDeck[4];
	float					shufflePercent;
	float					blackjackPayout;
	UInt32					reelStops[7];			// the values here must total 14
	UInt32					numDecks;
	UInt32					maxWinnings;
	UInt32					currencyRefID;			// ID, not form pointer
	UInt32					winningsQuestRefID;		// ID, not form pointer
	UInt32					casinoFlags;			// 1: dealer stand on soft 17 (no other flags)
	UInt32					unk220[2];
};

// 7C
class TESChallenge : public TESForm
{
public:
	enum
	{
		kFlag_StartDisabled = 1 << 0,
		kFlag_Recurring = 1 << 1,
		kFlag_ShowZeroProgress = 1 << 2
	};

	enum ChallengeTypes
	{
		kChallengeType_KillInFormList,
		kChallengeType_KillFormID,
		kChallengeType_KillInCategory,
		kChallengeType_HitEnemy,
		kChallengeType_LocationDiscovered,
		kChallengeType_UseAnIngestible,
		kChallengeType_AcquireAnItem,
		kChallengeType_UseASkill,
		kChallengeType_DoDamage,
		kChallengeType_UseAnIngestibleInFormList,
		kChallengeType_AcquireItemInFormList,
		kChallengeType_MiscStat,
		kChallengeType_Crafting,
		kChallengeType_Scripted,
	};

	struct ChallengeData	// 018
	{
		UInt32		type;			// needs enumeration
		UInt32		threshold;
		UInt32		flags;
		UInt32		interval;
		UInt16		value1;			// these fields change based on challenge type
		UInt16		value2;			// might need unions...
		UInt32		value3;
	};

	TESFullName				fullName;		// 18
	TESDescription			description;	// 24
	TESScriptableForm		scriptable;		// 2C
	TESIcon					icon;			// 38
	BGSMessageIcon			msgIcon;		// 44
	
	ChallengeData			data;			// 54
	UInt32					amount;			// 6C
	UInt32					challengeflags;	// 70
	TESForm					*SNAM;			// 74
	TESForm					*XNAM;			// 78
};
static_assert(sizeof(TESChallenge) == 0x7C);

// B0
class BGSBodyPart : public BaseFormComponent
{
public:
	enum
	{
		kFlags_Severable =		1,
		kFlags_IKData =			2,
		kFlags_BipedData =		4,
		kFlags_Explodable =		8,
		kFlags_IsHead =			0x10,
		kFlags_Headtracking =	0x20,
		kFlags_Absolute =		0x40,
	};

	String				partNode;				// 04
	String				VATSTarget;				// 0C
	String				startNode;				// 14
	String				partName;				// 1C
	String				targetBone;				// 24
	TESModel			limbReplacement;		// 2C
	UInt32				unk44[6];				// 44
	float				damageMult;				// 5C
	UInt8				flags;					// 60
	UInt8				pad61;					// 61
	UInt8				healthPercent;			// 62
	UInt8				actorValue;				// 63
	UInt8				toHitChance;			// 64
	UInt8				explChance;				// 65
	UInt8				explDebrisCount;		// 66
	UInt8				pad67;					// 67
	BGSDebris			*explDebris;			// 68
	BGSExplosion		*explExplosion;			// 6C
	float				trackingMaxAngle;		// 70
	float				explDebrisScale;		// 74
	UInt8				sevrDebrisCount;		// 78
	UInt8				pad79[3];				// 79
	BGSDebris			*sevrDebris;			// 7C
	BGSExplosion		*sevrExplosion;			// 80
	float				sevrDebrisScale;		// 84
	float				goreEffTranslate[3];	// 88
	float				goreEffRotation[3];		// 94
	BGSImpactDataSet	*sevrImpactDS;			// A0
	BGSImpactDataSet	*explImpactDS;			// A4
	UInt8				sevrDecalCount;			// A8
	UInt8				explDecalCount;			// A9
	UInt8				padAA[2];				// AA
	float				limbRepScale;			// AC

	void SetFlag(UInt32 pFlag, bool bEnable)
	{
		if (bEnable) flags |= pFlag;
		else flags &= ~pFlag;
	}
};

static_assert(sizeof(BGSBodyPart) == 0xB0);

// 74
class BGSBodyPartData : public TESForm
{
public:
	enum
	{
		eBodyPart_Torso = 0,
		eBodyPart_Head1,
		eBodyPart_Head2,
		eBodyPart_LeftArm1,
		eBodyPart_LeftArm2,
		eBodyPart_RightArm1,
		eBodyPart_RightArm2,
		eBodyPart_LeftLeg1,
		eBodyPart_LeftLeg2,
		eBodyPart_LeftLeg3,
		eBodyPart_RightLeg1,
		eBodyPart_RightLeg2,
		eBodyPart_RightLeg3,
		eBodyPart_Brain,
		eBodyPart_Weapon,
	};

	TESModel		model;				// 018
	BGSPreloadable	preloadable;		// 030
	BGSBodyPart		*bodyParts[15];		// 034
	BGSRagdoll		*ragDoll;			// 070
};
static_assert(sizeof(BGSBodyPartData) == 0x74);

// C4
class MediaSet : public TESForm
{
public:
	enum SetType
	{
		kSetType_Battle,
		kSetType_Location,
		kSetType_Dungeon,
		kSetType_Incidental,
		kSetType_None
	};

	// 10
	struct NameAndDB
	{
		String	name;
		float	decibels;
		float	percent;
	};

	TESFullName		fullName;		// 18
	UInt32			unk24;			// 24
	UInt32			unk28;			// 28
	UInt32			unk2C;			// 2C
	UInt8			byte30;			// 30
	UInt8			byte31;			// 31
	UInt8			byte32;			// 32
	UInt8			byte33;			// 33
	UInt8			byte34;			// 34
	UInt8			pad35[3];		// 35
	TESFullName		name38;			// 38
	UInt32			type;			// 44
	NameAndDB		dayOuter;		// 48
	NameAndDB		dayMiddle;		// 58
	NameAndDB		dayInner;		// 68
	NameAndDB		nightOuter;		// 78
	NameAndDB		nightMiddle;	// 88
	NameAndDB		nightInner;		// 98
	UInt32			enableFlags;	// A8
	float			waitTime;		// AC
	float			loopFadeOut;	// B0
	float			recoveryTime;	// B4
	float			nightTimeMax;	// B8
	TESSound		*sound1;		// BC
	TESSound		*sound2;		// C0
};
static_assert(sizeof(MediaSet) == 0xC4);

// B8
class MediaLocationController : public TESForm
{
public:
	TESFullName			fullName;		// 18
	UInt32				unk24[4];		// 24
	UInt8				byte34;			// 34
	UInt8				byte35;			// 35
	UInt8				byte36;			// 36
	UInt8				byte37;			// 37
	UInt8				byte38;			// 38
	UInt8				pad39[3];		// 39
	float				flt3C;			// 3C
	UInt32				hostileTargets;	// 40
	UInt32				inFactionTargets;	// 44
	UInt32				unk48;			// 48
	UInt8				byte4C;			// 4C
	UInt8				pad4D[3];		// 4D
	MediaSet			*mediaSet;		// 50
	UInt32				unk54[3];		// 54
	TESFaction			*reputation;	// 60
	UInt32				unk64;			// 64
	UInt32				unk68;			// 68
	UInt32				mlcFlags;		// 6C
	float				flt70;			// 70
	float				flt74;			// 74
	float				retriggerDelay;	// 78
	float				locationDelay;	// 7C
	UInt32				dayStart;		// 80
	UInt32				nightStart;		// 84
	tList<MediaSet>		neutralSets;	// 88
	tList<MediaSet>		allySets;		// 90
	tList<MediaSet>		friendSets;		// 98
	tList<MediaSet>		enemySets;		// A0
	tList<MediaSet>		locationSets;	// A8
	tList<MediaSet>		battleSets;		// B0
};
static_assert(sizeof(MediaLocationController) == 0xB8);

// BGSAddonNode (60)
class BGSAddonNode : public TESBoundObject
{
public:
	TESModel	model;				// 030
	UInt32 unk48[(0x60-0x48) >> 2]; // 048
};
static_assert(sizeof(BGSAddonNode) == 0x60);

enum ActorValueCode : UInt32
{
	/*00*/kAVCode_Aggression,
	/*01*/kAVCode_Confidence,
	/*02*/kAVCode_Energy,
	/*03*/kAVCode_Responsibility,
	/*04*/kAVCode_Mood,
	/*05*/kAVCode_Strength,
	/*06*/kAVCode_Perception,
	/*07*/kAVCode_Endurance,
	/*08*/kAVCode_Charisma,
	/*09*/kAVCode_Intelligence,
	/*0A*/kAVCode_Agility,
	/*0B*/kAVCode_Luck,
	/*0C*/kAVCode_ActionPoints,
	/*0D*/kAVCode_CarryWeight,
	/*0E*/kAVCode_CritChance,
	/*0F*/kAVCode_HealRate,
	/*10*/kAVCode_Health,
	/*11*/kAVCode_MeleeDamage,
	/*12*/kAVCode_DamageResist,
	/*13*/kAVCode_PoisonResist,
	/*14*/kAVCode_RadResist,
	/*15*/kAVCode_SpeedMult,
	/*16*/kAVCode_Fatigue,
	/*17*/kAVCode_Karma,
	/*18*/kAVCode_XP,
	/*19*/kAVCode_PerceptionCondition,
	/*1A*/kAVCode_EnduranceCondition,
	/*1B*/kAVCode_LeftAttackCondition,
	/*1C*/kAVCode_RightAttackCondition,
	/*1D*/kAVCode_LeftMobilityCondition,
	/*1E*/kAVCode_RightMobilityCondition,
	/*1F*/kAVCode_BrainCondition,
	/*20*/kAVCode_Barter,
	/*21*/kAVCode_BigGuns,
	/*22*/kAVCode_EnergyWeapons,
	/*23*/kAVCode_Explosives,
	/*24*/kAVCode_Lockpick,
	/*25*/kAVCode_Medicine,
	/*26*/kAVCode_MeleeWeapons,
	/*27*/kAVCode_Repair,
	/*28*/kAVCode_Science,
	/*29*/kAVCode_Guns,
	/*2A*/kAVCode_Sneak,
	/*2B*/kAVCode_Speech,
	/*2C*/kAVCode_Survival,
	/*2D*/kAVCode_Unarmed,
	/*2E*/kAVCode_InventoryWeight,
	/*2F*/kAVCode_Paralysis,
	/*30*/kAVCode_Invisibility,
	/*31*/kAVCode_Chameleon,
	/*32*/kAVCode_NightEye,
	/*33*/kAVCode_Turbo,
	/*34*/kAVCode_FireResist,
	/*35*/kAVCode_WaterBreathing,
	/*36*/kAVCode_RadiationRads,
	/*37*/kAVCode_BloodyMess,
	/*38*/kAVCode_UnarmedDamage,
	/*39*/kAVCode_Assistance,
	/*3A*/kAVCode_ElectricResist,
	/*3B*/kAVCode_FrostResist,
	/*3C*/kAVCode_EnergyResist,
	/*3D*/kAVCode_EmpResist,
	/*3E*/kAVCode_Variable01,
	/*3F*/kAVCode_Variable02,
	/*40*/kAVCode_Variable03,
	/*41*/kAVCode_Variable04,
	/*42*/kAVCode_Variable05,
	/*43*/kAVCode_Variable06,
	/*44*/kAVCode_Variable07,
	/*45*/kAVCode_Variable08,
	/*46*/kAVCode_Variable09,
	/*47*/kAVCode_Variable10,
	/*48*/kAVCode_IgnoreCrippledLimbs,
	/*49*/kAVCode_Dehydration,
	/*4A*/kAVCode_Hunger,
	/*4B*/kAVCode_SleepDeprivation,
	/*4C*/kAVCode_DamageThreshold,
	kAVCode_Max
};

// C4
class ActorValueInfo : public TESForm
{
public:
	enum ActorValueFlags
	{
		kAVFlag_Unk0 =					1,
		kAVFlag_Unused1 =				2,
		kAVFlag_Unused2 =				4,
		kAVFlag_Max10 =					8,
		kAVFlag_Skill =					0x10,
		kAVFlag_Condition =				0x20,
		kAVFlag_Regenerates =			0x40,
		kAVFlag_IgnoreDerivedValue =	0x80,
		kAVFlag_NonDamageable =			0x100,
		kAVFlag_NegateEffectsString =	0x200,
		kAVFlag_CreatureCombatSkill =	0x400,
		kAVFlag_UnkB =					0x800,
		kAVFlag_UnkC =					0x1000,
		kAVFlag_UnusedD =				0x2000,
		kAVFlag_NonModifiableByScript =	0x4000,
		kAVFlag_Min1 =					0x8000
	};

	enum ActorValueGroups
	{
		kAVGroup_SPECIAL =		0,
		kAVGroup_Misc =			1,	//	Incl. Health, Fatigue, AP, etc.
		kAVGroup_Skills =		2,
		kAVGroup_Behavior =		3,
		kAVGroup_LimbCond =		4,
		kAVGroup_Effects =		5,	//	Incl. RADS and HC needs.
		kAVGroup_Resistance =	6,
		kAVGroup_Variables =	7
	};

	TESFullName					fullName;		// 18
	TESDescription				description;	// 24
	TESIcon						icon;			// 2C

	char								*infoName;		// 38
	String								avName;			// 3C
	UInt32								avFlags;		// 44
	UInt32								avGroup;		// 48
	void								*callback4C;	// 4C
	UInt32								unk50;			// 50
	void								(*OnChangeCallback)(ActorValueOwner *avOwner, UInt32 avCode, float prevVal, float modifier, ActorValueOwner *attacker);	// 54
	FixedTypeArray<ActorValueCode, 15>	derivedStatIDs;	// 58
	FixedTypeArray<const char*, 10>		levelNames;		// 98

	__forceinline static ActorValueInfo **Array() {return (ActorValueInfo**)0x11D61C8;}
};
static_assert(sizeof(ActorValueInfo) == 0xC4);

bool __fastcall ToggleDerivedActorValue(ActorValueCode specialID, ActorValueCode avID, bool toggle);

// 20
class BGSRadiationStage : public TESForm
{
public:
	UInt32		threshold;	// 18
	SpellItem	*effect;	// 1C
};

// 20
class BGSDehydrationStage : public TESForm
{
public:
	UInt32		threshold;	// 18
	SpellItem	*effect;	// 1C
};

// 20
class BGSHungerStage : public TESForm
{
public:
	UInt32		threshold;	// 18
	SpellItem	*effect;	// 1C
};

// 20
class BGSSleepDeprevationStage : public TESForm
{
public:
	UInt32		threshold;	// 18
	SpellItem	*effect;	// 1C
};

// BGSCameraShot (78)
class BGSCameraShot : public TESForm
{
public:
	TESModel	model;								// 018
	//	TESImageSpaceModifiableForm imageSpaceModForm;	// 024

	UInt32 unk024[(0x78-0x24) >> 2];	// 024
};

// BGSCameraPath (38)
class BGSCameraPath;

// BGSVoiceType (24)
class BGSVoiceType : public TESForm
{
public:
	UInt32		unk18;		// 18
	String		editorID;	// 1C
};

// 78
class BGSImpactData : public TESForm
{
public:
	TESModel		model;				// 18
	float			effectDuration;		// 30
	UInt8			effectOrientation;	// 34	0 - Surface Normal, 1 - Projectile Vector, 2 - Projectile Reflection
	UInt8			pad35[3];			// 35
	float			angleThreshold;		// 38
	float			placementRadius;	// 3C
	UInt8			soundLevel;			// 40
	UInt8			pad41[3];			// 41
	UInt8			noDecalData;		// 44
	UInt8			pad45[3];			// 45
	BGSTextureSet	*textureSet;		// 48
	TESSound		*sound1;			// 4C
	TESSound		*sound2;			// 50
	DecalInfo		decalInfo;			// 54
};
static_assert(sizeof(BGSImpactData) == 0x78);

// 4C
class BGSImpactDataSet : public TESForm
{
public:
	BGSPreloadable	preloadable;		// 18
	BGSImpactData	*impactDatas[12];	// 1C
};

static_assert(sizeof(BGSImpactDataSet) == 0x4C);

// 190
class TESObjectARMA : public TESObjectARMO
{
public:
};

static_assert(sizeof(TESObjectARMA) == 0x190);

// BGSEncounterZone (30)
class BGSEncounterZone : public TESForm
{
public:
	TESForm		*owner;			// 18
	UInt8		rank;			// 1C
	UInt8		minLevel;		// 1D
	UInt8		zoneFlags;		// 1E
	UInt8		byte1F;			// 1F
	UInt32		detachTime;		// 20
	UInt32		attachTime;		// 24
	UInt32		resetTime;		// 28
	UInt16		zoneLevel;		// 2C
	UInt8		pad2E[2];		// 2E
};

// 40
class BGSMessage : public TESForm
{
public:
	struct Button
	{
		String			label;
		ConditionList	conditions;
	};

	TESFullName		fullName;		// 18
	TESDescription	description;	// 24

	BGSMenuIcon		*menuIcon;		// 2C
	tList<Button>	buttons;		// 30
	UInt8			msgFlags;		// 38	1 - Message Box, 2 - Auto-display
	UInt8			pad39[3];		// 39
	UInt32			displayTime;	// 3C
};

static_assert(sizeof(BGSMessage) == 0x40);

// BGSRagdoll (148)
class BGSRagdoll : public TESForm
{
public:
	TESModel	model;					// 018
	UInt32	unk030[(0x148-0x30) >> 2];	// 030
};

static_assert(sizeof(BGSRagdoll) == 0x148);

// 44
class BGSLightingTemplate : public TESForm
{
public:
	UInt32			ambientRGB;			// 18
	UInt32			directionalRGB;		// 1C
	UInt32			fogRGB;				// 20
	float			fogNear;			// 24
	float			fogFar;				// 28
	UInt32			directionalXY;		// 2C
	UInt32			directionalZ;		// 30
	float			directionalFade;	// 34
	float			fogClipDist;		// 38
	float			fogPower;			// 3C
	TESObjectCELL	*getValuesFrom;		// 40
};

static_assert(sizeof(BGSLightingTemplate) == 0x44);

// BGSMusicType (30)
class BGSMusicType : public TESForm
{
public:
	TESSoundFile	soundFile;	// 18
	float			unk24;		// 24
	UInt32			unk28;		// 28
	UInt32			unk2C;		// 2C
};

// BGSDefaultObjectManager, with help from "Luthien Anarion"

static_assert(sizeof(BGSMusicType) == 0x30);

const char kDefaultObjectNames[34][28] = {	// 0x0118C360 is an array of struct: { char * Name, UInt8 kFormType , UInt8 pad[3] }
      "Stimpack",
      "SuperStimpack",
      "RadX",
      "RadAway",
      "Morphine",
      "Perk Paralysis",
      "Player Faction",
      "Mysterious Stranger NPC",
      "Mysterious Stranger Faction",
      "Default Music",
      "Battle Music",
      "Death Music",
      "Success Music",
      "Level Up Music",
      "Player Voice (Male)",
      "Player Voice (Male Child)",
      "Player Voice (Female)",
      "Player Voice (Female Child)",
      "Eat Package Default Food",
      "Every Actor Ability",
      "Drug Wears Off Image Space",
      "Doctor's Bag",
      "Miss Fortune NPC",
      "Miss Fortune Faction",
      "Meltdown Explosion",
      "Unarmed Forward PA",
      "Unarmed Backward PA",
      "Unarmed Left PA",
      "Unarmed Right PA",
      "Unarmed Crouch PA",
      "Unarmed Counter PA",
      "Spotter Effect",
      "Item Detected Effect",
      "Cateye Mobile Effect (NYI)"
};

// BGSDefaultObjectManager (A0)
class BGSDefaultObjectManager : public TESForm
{
public:
	__forceinline static BGSDefaultObjectManager *GetSingleton() {return *(BGSDefaultObjectManager**)0x11CA80C;}

	enum {
		kDefaultObject_Max = 34,
	};

	typedef TESForm *	FormArray[kDefaultObject_Max];

	struct FormStruct {
		TESForm*    Stimpak;
		TESForm*    SuperStimpak;
		TESForm*    RadX;
		TESForm*    RadAway;
		TESForm*    Morphine;
		TESForm*    PerkParalysis;
		TESForm*    PlayerFaction;
		TESForm*    MysteriousStranger;
		TESForm*    MysteriousStrangerFaction;
		TESForm*    DefaultMusic;
		TESForm*    BattleMusic;
		TESForm*    DefaultDeath;
		TESForm*    SuccessMusic;
		TESForm*    LevelUpMusic;
		TESForm*    PlayerVoiceMale;
		TESForm*    PlayerVoiceMaleChild;
		TESForm*    PlayerVoiceFemale;
		TESForm*    PlayerVoiceFemaleChild;
		TESForm*    EatPackageDefaultFood;
		TESForm*    EveryActorAbility;
		TESForm*    DrugWearOffImageSpace;
		// FNV
		TESForm*    DoctorsBag;
		TESForm*    MissFortuneNPC;
		TESForm*    MissFortuneFaction;
		TESForm*    MeltdownExplosion;
		TESForm*    UnarmedForwardPA;
		TESForm*    UnarmedBackwardPA;
		TESForm*    UnarmedLeftPA;
		TESForm*    UnarmedRightPA;
		TESForm*    UnarmedCrouchPA;
		TESForm*    UnarmedCounterPA;
		TESForm*    SpotterEffect;
		TESForm*    ItemDetectedEffect;
		TESForm*    CateyeMobileEffectNYI;
	};

	union DefaultObjects {
		FormStruct	asStruct;
		FormArray	asArray;
	};

	DefaultObjects	defaultObjects;	// 018
};
static_assert(sizeof(BGSDefaultObjectManager) == 0xA0);

enum MiscStatCode : UInt32
{
	/*00*/kMiscStat_QuestsCompleted,
	/*01*/kMiscStat_LocationsDiscovered,
	/*02*/kMiscStat_PeopleKilled,
	/*03*/kMiscStat_CreaturesKilled,
	/*04*/kMiscStat_LocksPicked,
	/*05*/kMiscStat_ComputersHacked,
	/*06*/kMiscStat_StimpaksTaken,
	/*07*/kMiscStat_RadXTaken,
	/*08*/kMiscStat_RadAwayTaken,
	/*09*/kMiscStat_ChemsTaken,
	/*0A*/kMiscStat_TimesAddicted,
	/*0B*/kMiscStat_MinesDisarmed,
	/*0C*/kMiscStat_SpeechSuccesses,
	/*0D*/kMiscStat_PocketsPicked,
	/*0E*/kMiscStat_PantsExploded,
	/*0F*/kMiscStat_BooksRead,
	/*10*/kMiscStat_HealthFromStimpaks,
	/*11*/kMiscStat_WeaponsCreated,
	/*12*/kMiscStat_HealthFromFood,
	/*13*/kMiscStat_WaterConsumed,
	/*14*/kMiscStat_SandmanKills,
	/*15*/kMiscStat_ParalyzingPunches,
	/*16*/kMiscStat_RobotsDisabled,
	/*17*/kMiscStat_TimesSlept,
	/*18*/kMiscStat_CorpsesEaten,
	/*19*/kMiscStat_MysteriousStrangerVisits,
	/*1A*/kMiscStat_DoctorBagsUsed,
	/*1B*/kMiscStat_ChallengesCompleted,
	/*1C*/kMiscStat_MissFortunateOccurrences,
	/*1D*/kMiscStat_Disintegrations,
	/*1E*/kMiscStat_HaveLimbsCrippled,
	/*1F*/kMiscStat_SpeechFailures,
	/*20*/kMiscStat_ItemsCrafted,
	/*21*/kMiscStat_WeaponModifications,
	/*22*/kMiscStat_ItemsRepaired,
	/*23*/kMiscStat_TotalThingsKilled,
	/*24*/kMiscStat_DismemberedLimbs,
	/*25*/kMiscStat_CaravanGamesWon,
	/*26*/kMiscStat_CaravanGamesLost,
	/*27*/kMiscStat_BarterAmountTraded,
	/*28*/kMiscStat_RouletteGamesPlayed,
	/*29*/kMiscStat_BlackjackGamesPlayed,
	/*2A*/kMiscStat_SlotsGamesPlayed,
	kMiscStat_Max
};

enum PerkEntryPointID : UInt32
{
	/*00*/kPerkEntry_CalculateWeaponDamage,
	/*01*/kPerkEntry_CalculateMyCriticalHitChance,
	/*02*/kPerkEntry_CalculateMyCriticalHitDamage,
	/*03*/kPerkEntry_CalculateWeaponAttackAPCost,
	/*04*/kPerkEntry_CalculateMineExplodeChance,
	/*05*/kPerkEntry_AdjustRangePenalty,
	/*06*/kPerkEntry_AdjustLimbDamage,
	/*07*/kPerkEntry_CalculateWeaponRange,
	/*08*/kPerkEntry_CalculateToHitChance,
	/*09*/kPerkEntry_AdjustExperiencePoints,
	/*0A*/kPerkEntry_AdjustGainedSkillPoints,
	/*0B*/kPerkEntry_AdjustBookSkillPoints,
	/*0C*/kPerkEntry_ModifyRecoveredHealth,
	/*0D*/kPerkEntry_CalculateInventoryAPCost,
	/*0E*/kPerkEntry_GetDisposition,
	/*0F*/kPerkEntry_GetShouldAttack,
	/*10*/kPerkEntry_GetShouldAssist,
	/*11*/kPerkEntry_CalculateBuyPrice,
	/*12*/kPerkEntry_GetBadKarma,
	/*13*/kPerkEntry_GetGoodKarma,
	/*14*/kPerkEntry_IgnoreLockedTerminal,
	/*15*/kPerkEntry_AddLeveledListOnDeath,
	/*16*/kPerkEntry_GetMaxCarryWeight,
	/*17*/kPerkEntry_ModifyAddictionChance,
	/*18*/kPerkEntry_ModifyAddictionDuration,
	/*19*/kPerkEntry_ModifyPositiveChemDuration,
	/*1A*/kPerkEntry_AdjustDrinkingRadiation,
	/*1B*/kPerkEntry_Activate,
	/*1C*/kPerkEntry_MysteriousStranger,
	/*1D*/kPerkEntry_HasParalyzingPalm,
	/*1E*/kPerkEntry_HackingScienceBonus,
	/*1F*/kPerkEntry_IgnoreRunningDuringDetection,
	/*20*/kPerkEntry_IgnoreBrokenLock,
	/*21*/kPerkEntry_HasConcentratedFire,
	/*22*/kPerkEntry_CalculateGunSpread,
	/*23*/kPerkEntry_PlayerKillAPReward,
	/*24*/kPerkEntry_ModifyEnemyCriticalHitChance,
	/*25*/kPerkEntry_ReloadSpeed,
	/*26*/kPerkEntry_EquipSpeed,
	/*27*/kPerkEntry_ActionPointRegen,
	/*28*/kPerkEntry_ActionPointCost,
	/*29*/kPerkEntry_MissFortune,
	/*2A*/kPerkEntry_ModifyRunSpeed,
	/*2B*/kPerkEntry_ModifyAttackSpeed,
	/*2C*/kPerkEntry_ModifyRadiationConsumed,
	/*2D*/kPerkEntry_HasPipHacker,
	/*2E*/kPerkEntry_HasMeltdown,
	/*2F*/kPerkEntry_SeeEnemyHealth,
	/*30*/kPerkEntry_HasJuryRigging,
	/*31*/kPerkEntry_ModifyThreatRange,
	/*32*/kPerkEntry_ModifyThreat,
	/*33*/kPerkEntry_HasFastTravelAlways,
	/*34*/kPerkEntry_KnockdownChance,
	/*35*/kPerkEntry_ModifyWeaponStrengthReq,
	/*36*/kPerkEntry_ModifyAimingMoveSpeed,
	/*37*/kPerkEntry_ModifyLightItems,
	/*38*/kPerkEntry_ModifyDamageThresholdDefender,
	/*39*/kPerkEntry_ModifyChanceforAmmoItem,
	/*3A*/kPerkEntry_ModifyDamageThresholdAttacker,
	/*3B*/kPerkEntry_ModifyThrowingVelocity,
	/*3C*/kPerkEntry_ChanceForItemOnFire,
	/*3D*/kPerkEntry_HasUnarmedForwardPowerAttack,
	/*3E*/kPerkEntry_HasUnarmedBackPowerAttack,
	/*3F*/kPerkEntry_HasUnarmedCrouchedPowerAttack,
	/*40*/kPerkEntry_HasUnarmedCounterAttack,
	/*41*/kPerkEntry_HasUnarmedLeftPowerAttack,
	/*42*/kPerkEntry_HasUnarmedRightPowerAttack,
	/*43*/kPerkEntry_VATSHelperChance,
	/*44*/kPerkEntry_ModifyItemDamage,
	/*45*/kPerkEntry_HasImprovedDetection,
	/*46*/kPerkEntry_HasImprovedSpotting,
	/*47*/kPerkEntry_HasImprovedItemDetection,
	/*48*/kPerkEntry_AdjustExplosionRadius,
	/*49*/kPerkEntry_AdjustHeavyWeaponWeight,
	kPerkEntry_Max
};

enum MaterialType
{
	kMaterial_Stone,
	kMaterial_Dirt,
	kMaterial_Grass,
	kMaterial_Glass,
	kMaterial_Metal,
	kMaterial_Wood,
	kMaterial_Organic,
	kMaterial_Cloth,
	kMaterial_Water,
	kMaterial_HollowMetal,
	kMaterial_OrganicBug,
	kMaterial_OrganicGlow
};

enum EActionListForm
{
	eActionListForm_AddAt	= 00,
	eActionListForm_DelAt,
	eActionListForm_ChgAt,
	eActionListForm_GetAt,
	eActionListForm_Max,
};

enum EWhichListForm
{
	eWhichListForm_RaceHair					= 00,
	eWhichListForm_RaceEyes,
	eWhichListForm_RaceHeadPart,			// ? //
	eWhichListForm_BaseFaction,
	eWhichListForm_BaseRank,
	eWhichListForm_BasePackage,
	eWhichListForm_BaseSpellListSpell,
	eWhichListForm_BaseSpellListLevSpell,
	eWhichListForm_FactionRankName,
	eWhichListForm_FactionRankFemaleName,
	eWhichListForm_HeadParts,
	eWhichListForm_LevCreatureRef,
	eWhichListForm_LevCharacterRef,
	eWhichListForm_FormList,
	eWhichListForm_Max,
};

