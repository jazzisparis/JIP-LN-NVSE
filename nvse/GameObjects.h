#pragma once

SInt32 __fastcall GetFormCount(TESContainer::FormCountList *formCountList, ContChangesEntryList *objList, TESForm *form);

// 68
class TESObjectREFR : public TESForm
{
public:
	/*138*/virtual bool		GetStartingPosition(NiVector3 *outPos, NiVector3 *outRot, TESForm **outWrldOrCell, TESForm *defaultWrldOrCell);
	/*13C*/virtual void		SayTopic(Sound *sound, TESTopic *topic, TESObjectREFR *target, bool dontUseNiNode, bool notVoice, bool useLipFile, UInt8 unused, bool subtitles);
	/*140*/virtual void		Unk_50(void);
	/*144*/virtual void		DamageObject(float damage, bool allowDestroyed);
	/*148*/virtual bool		GetCastsShadows();
	/*14C*/virtual void		SetCastsShadows(bool doSet);
	/*150*/virtual void		Unk_54(void);
	/*154*/virtual void		Unk_55(void);
	/*158*/virtual void		Unk_56(void);
	/*15C*/virtual bool		IsObstacle();
	/*160*/virtual bool		BaseIsQuestItem();
	/*164*/virtual void		Unk_59(void);
	/*168*/virtual void		Unk_5A(void);
	/*16C*/virtual void		Unk_5B(void);
	/*170*/virtual void		Unk_5C(void);
	/*174*/virtual void		Unk_5D(void);
	/*178*/virtual void		RefreshDynamicLight();
	/*17C*/virtual TESObjectREFR	*RemoveItem(TESForm *toRemove, BaseExtraList *extraList, UInt32 quantity, bool keepOwner, bool drop, TESObjectREFR *destRef, UInt32 unk6, UInt32 unk7, bool unk8, bool unk9);
	/*180*/virtual void		Unk_60(void);
	/*184*/virtual bool		LoadEquipedItem3D(TESForm *item, UInt32 count, ExtraDataList *xData, bool lockEquip);
	/*188*/virtual void		Unk_62(void);
	/*18C*/virtual void		Unk_63(void);
	/*190*/virtual void		AddItem(TESForm *item, ExtraDataList *xDataList, UInt32 quantity);
	/*194*/virtual void		Unk_65(void);
	/*198*/virtual MagicCaster	*GetMagicCaster();
	/*19C*/virtual MagicTarget	*GetMagicTarget();
	/*1A0*/virtual bool		GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	/*1A4*/virtual UInt32	GetActorUnk0148();			// result can be interchanged with baseForm, so TESForm* ?
	/*1A8*/virtual void		SetActorUnk0148(UInt32 arg1);
	/*1AC*/virtual BSFaceGenNiNode	*GetFaceGenNodeBiped(UInt32 arg1);
	/*1B0*/virtual BSFaceGenNiNode	*GetFaceGenNodeSkinned(UInt32 arg1);
	/*1B4*/virtual BSFaceGenNiNode	*CallGetFaceGenNodeSkinned(UInt32 arg1);	// REFR: calls 006C
	/*1B8*/virtual BSFaceGenAnimationData	*CreateFaceAnimationData(UInt32 arg1);
	/*1BC*/virtual void		Unk_6F(void);
	/*1C0*/virtual bool		Unload3D();
	/*1C4*/virtual void		AnimateNiNode();
	/*1C8*/virtual NiNode	*GenerateNiNode(bool arg1);
	/*1CC*/virtual void		Set3D(NiNode* niNode, bool unloadArt);
	/*1D0*/virtual NiNode	*GetNiNode_v();
	/*1D4*/virtual void		Unk_75(void);
	/*1D8*/virtual NiVector3	*GetMinBounds(NiVector3 *outBounds);
	/*1DC*/virtual NiVector3	*GetMaxBounds(NiVector3 *outBounds);
	/*1E0*/virtual void		UpdateAnimation();
	/*1E4*/virtual AnimData	*GetAnimData();			// 0079
	/*1E8*/virtual BipedAnim	*GetBipedAnim();	// 007A	Character only
	/*1EC*/virtual BipedAnim	*CallGetBipedAnim();
	/*1F0*/virtual void		SetBipedAnim(BipedAnim *bipedAnims);
	/*1F4*/virtual NiVector3	*GetPos();
	/*1F8*/virtual void		Unk_7E(UInt32 arg0);
	/*1FC*/virtual void		Unk_7F(void);
	/*200*/virtual void		Unk_80(UInt32 arg0);
	/*204*/virtual void		Unk_81(UInt32 arg0);
	/*208*/virtual void		Unk_82(void);
	/*20C*/virtual NiNode	*GetProjectileNode();
	/*210*/virtual void		SetProjectileNode(NiNode *node);
	/*214*/virtual UInt32	GetSitSleepState();			
	/*218*/virtual bool		IsCharacter();			// return false for Actor and Creature, true for character and PlayerCharacter
	/*21C*/virtual bool		IsCreature();
	/*220*/virtual bool		IsExplosion();
	/*224*/virtual bool		IsProjectile() const;
	/*228*/virtual void		SetParentCell(TESObjectCELL *cell);			// SetParentCell (Interior only ?)
	/*22C*/virtual bool		HasHealth(bool arg0);	// HasHealth (baseForm health > 0 or Flags bit23 set)
	/*230*/virtual bool		GetHasKnockedState();
	/*234*/virtual bool		GetIsParalyzed();
	/*238*/virtual void		Unk_8E(void);
	/*23C*/virtual void		Unk_8F(void);
	/*240*/virtual void		MoveToHighProcess();

	enum
	{
		kFlags_Unk00000002			= 0x00000002,
		kFlags_Deleted				= 0x00000020,		// refr removed from .esp or savegame
		kFlags_Taken				= kFlags_Deleted | kFlags_Unk00000002,
		kFlags_Persistent			= 0x00000400,		//shared bit with kFormFlags_QuestItem
		kFlags_Temporary			= 0x00004000,
		kFlags_IgnoreFriendlyHits	= 0x00100000,
		kFlags_Destroyed			= 0x00800000,
		
		kChanged_Inventory			= 0x08000000,
	};

	struct RenderState
	{
		TESObjectREFR	*currWaterRef;		// 00
		UInt32			underwaterCount;	// 04	0-0x13 when fully-underwater; exterior only
		float			waterLevel;			// 08
		float			revealDistance;		// 0C
		UInt32			flags;				// 10
		NiNode			*rootNode;			// 14
		bhkPhantom		*phantom;			// 18	Used with trigger volume
	};

	TESChildCell	childCell;		// 18

	TESSound		*loopSound;		// 1C
	TESForm			*baseForm;		// 20
	NiVector3		rotation;		// 24
	NiVector3		position;		// 30
	float			scale;			// 3C 
	TESObjectCELL	*parentCell;	// 40
	ExtraDataList	extraDataList;	// 44
	RenderState		*renderState;	// 64

	bool IsTaken() const {return (flags & kFlags_Taken) != 0;}
	bool IsPersistent() const {return (flags & kFlags_Persistent) != 0;}
	bool IsTemporary() const {return (flags & kFlags_Temporary) != 0;}
	bool IsDeleted() const {return (flags & kFlags_Deleted) != 0;}
	bool IsDestroyed() const {return (flags & kFlags_Destroyed) != 0;}

	__forceinline NiNode *GetRefNiNode() const {return renderState ? renderState->rootNode : nullptr;}

	TESForm *GetBaseForm() const;
	TESForm *GetBaseForm2() const;

	ScriptLocals *GetEventList() const;

	UInt8& JIPRefFlags() {return extraDataList.jipRefFlags5F;}

	void Update3D();
	bool IsMapMarker() const {return baseForm->refID == 0x10;}

	bool GetDisabled() const;
	ContChangesEntryList *GetContainerChangesList() const;
	ContChangesEntry *GetContainerChangesEntry(TESForm *itemForm) const;
	SInt32 GetItemCount(TESForm *form) const;
	void AddItemAlt(TESForm *item, UInt32 count, float condition, UInt32 doEquip = 0, UInt32 noMessage = 1);
	void RemoveItemTarget(TESForm *itemForm, TESObjectREFR *target, SInt32 quantity, bool keepOwner);
	TESObjectREFR *CreateInventoryRefForScriptedObj(TESForm *item, ScriptLocals *eventList);
	TESObjectCELL *GetParentCell() const;
	TESWorldSpace *GetParentWorld() const;
	bool __fastcall GetInSameCellOrWorld(TESObjectREFR *target) const;
	float __vectorcall GetDistance(TESObjectREFR *target) const;
	void SetPos(const NiVector3 &posVector);
	void __vectorcall SetAngle(__m128 pry, UInt8 setMode);
	void __fastcall MoveToCell(TESObjectCELL *cell, const NiVector3 &posVector);
	__m128 __vectorcall GetTranslatedPos(const NiVector3 &posMods) const;
	void __vectorcall Rotate(__m128 rotVector);
	void __vectorcall RotateAroundPoint(__m128 pry, const NiVector3 &origin, UInt32 skipAngles = 0);
	float __vectorcall GetHeadingAngle(TESObjectREFR *to) const;
	bool Disable();
	void DeleteReference();
	bhkCharacterController *GetCharacterController() const;
	TESObjectREFR *GetMerchantContainer() const;
	double GetWaterImmersionPerc() const;
	bool IsMobile() const;
	bool IsGrabbable() const;
	NiTexture** __fastcall GetTexturePtr(const char *blockName) const;
	void SwapTexture(const char *blockName, const char *filePath, UInt32 texIdx);
	bool SetLinkedRef(TESObjectREFR *linkObj, UInt8 modIdx = 0xFF);
	bool ValidForHooks() const;
	NiNode *GetNiNode() const;
	NiAVObject* __fastcall GetNiBlock(const char *blockName) const;
	NiAVObject* __fastcall GetNiBlock2(const char *blockName) const;
	NiNode* __fastcall GetNode(const char *nodeName) const;
	NiNode* __fastcall GetNode2(const char *nodeName) const;
	hkpRigidBody* __fastcall GetRigidBody(const char *blockName) const;
	void __fastcall ToggleCollision(bool toggle);
	BSBound *GetBoundingBox() const;
	MapMarkerData *GetMapMarkerData() const;

	__forceinline float GetScale() {return ThisCall<float>(0x567400, this);}

	static TESObjectREFR* __stdcall Create(bool bTemp = false);

	__forceinline bool Activate(TESObjectREFR *actionRef, UInt32 arg2 = 0, UInt32 arg3 = 0, UInt32 arg4 = 1)
	{
		return ThisCall<bool>(0x573170, this, actionRef, arg2, arg3, arg4);
	}
};
static_assert(sizeof(TESObjectREFR) == 0x068);

extern TESObjectREFR *s_tempPosMarker;

// 88
class MobileObject : public TESObjectREFR
{
public:
	/*244*/virtual void		MoveToLowProcess();
	/*248*/virtual void		MoveToMidLowProcess();
	/*24C*/virtual void		MoveToMidHighProcess();
	/*250*/virtual void		Move(float scale, NiVector3 *vec, int movementFlags);
	/*254*/virtual void		Jump();
	/*258*/virtual void		Unk_96(void);
	/*25C*/virtual void		RunProcess(float deltaTime);
	/*260*/virtual void		UpdateProcessLevel();
	/*264*/virtual void		Unk_99(void);
	/*268*/virtual void		DoUpdateAnimation(float deltaTime);
	/*26C*/virtual void		Unk_9B(void);
	/*270*/virtual void		Unk_9C(void);
	/*274*/virtual bool		IsInCombat(bool notSearching);
	/*278*/virtual void		Unk_9E(void);
	/*27C*/virtual void		Unk_9F(void);
	/*280*/virtual void		StartConversation(void);	// 9 args
	/*284*/virtual void		DoSpeechLoadLipFiles(void);	// 13 args!
	/*288*/virtual void		StopDialogue();
	/*28C*/virtual void		SetExtraRunOncePacks(TESPackage *package, UInt8 arg2);
	/*290*/virtual bool		HasStartingWorldOrCell();
	/*294*/virtual TESWorldSpace	*GetStartingWorld();
	/*298*/virtual TESObjectCELL	*GetStartingCell();
	/*29C*/virtual NiVector3	*GetStartingPos(NiVector3 *outPos);
	/*2A0*/virtual void		Unk_A8(void);
	/*2A4*/virtual void		Unk_A9(void);
	/*2A8*/virtual void		SetPos(NiVector3 *newPos);
	/*2AC*/virtual void		Unk_AB(void);
	/*2B0*/virtual void		HandleRunDetection(float arg1);
	/*2B4*/virtual void		Unk_AD(void);
	/*2B8*/virtual void		Unk_AE(void);
	/*2BC*/virtual float	AdjustRot(UInt32 arg1);
	/*2C0*/virtual void		Unk_B0(void);
	/*2C4*/virtual void		SetRotation(float angle);
	/*2C8*/virtual Actor	*GetCurrentTarget();
	/*2CC*/virtual void		Unk_B3(void);
	/*2D0*/virtual UInt32	GetSpeechExpression();
	/*2D4*/virtual void		SetSpeechExpression(UInt32 newExpr);
	/*2D8*/virtual UInt32	GetEmotionValue();
	/*2DC*/virtual void		SetEmotionValue(UInt32 newVal);
	/*2E0*/virtual void		Unk_B8(void);
	/*2E4*/virtual void		Unk_B9(void);
	/*2E8*/virtual bool		GetIsDying();
	/*2EC*/virtual void		Unk_BB(void);
	/*2F0*/virtual void		Unk_BC(void);
	/*2F4*/virtual void		Unk_BD(void);
	/*2F8*/virtual void		Update(float deltaTime);
	/*2FC*/virtual void		Unk_BF(void);
	/*300*/virtual void		Unk_C0(void);
	
	BaseProcess		*baseProcess;	// 68
	UInt32			unk6C;			// 6C
	Actor			*talkedToActor;	// 70
	UInt32			unk74;			// 74
	float			voiceTimer;		// 78
	UInt8			unk7C;			// 7C
	UInt8			isTalking;		// 7D
	UInt8			unk7E;			// 7E
	UInt8			unk7F;			// 7F
	UInt8			unk80;			// 80
	UInt8			unk81;			// 81
	UInt8			unk82;			// 82
	UInt8			unk83;			// 83
	UInt8			unk84;			// 84
	UInt8			unk85;			// 85
	UInt8			unk86;			// 86
	UInt8			unk87;			// 87	Init'd to the inverse of NoLowLevelProcessing
};
static_assert(sizeof(MobileObject) == 0x88);

typedef tList<ActiveEffect> ActiveEffectList;

// 0C
class MagicCaster
{
public:
	/*000*/virtual void		AddAbility(SpellItem *splItem, bool arg2);
	/*004*/virtual void		AddAddiction(SpellItem *splItem, bool arg2);
	/*008*/virtual void		AddEffect(SpellItem *splItem, bool arg2);
	/*00C*/virtual void		CastSpell(MagicItem *spell, bool arg2, MagicTarget *target, float arg4, bool arg5);
	/*010*/virtual void		AddDisease(SpellItem *splItem, MagicTarget *target, bool arg3);
	/*014*/virtual void		AddFormEffects(MagicItem *magItem, MagicItemForm *itemForm, bool arg3);
	/*018*/virtual MagicTarget	*PickMagicTarget();
	/*01C*/virtual void		Unk_07();
	/*020*/virtual void		Unk_08();
	/*024*/virtual void		Unk_09(UInt32 arg1, UInt32 arg2);
	/*028*/virtual bool		Unk_0A(UInt32 arg1, float *arg2, UInt32 *arg3, UInt32 arg4);
	/*02C*/virtual Actor	*GetActor();
	/*030*/virtual NiNode	*GetMagicNode();
	/*034*/virtual MagicItem	*GetMagicItem();
	/*038*/virtual bool		Unk_0E(ActiveEffect *activeEffect);
	/*03C*/virtual float	Unk_0F(UInt8 arg1, float arg2);
	/*040*/virtual void		SetMagicItem(MagicItem *spell);
	/*044*/virtual MagicTarget	*GetMagicTarget();
	/*048*/virtual void		SetMagicTarget(MagicTarget *magicTarget);
	/*04C*/virtual ActiveEffect	*CreateActiveEffect(MagicItem *magicItem, EffectItem *effItem, TESForm *itemForm);

	UInt32	unk04[2];	// 04
};
static_assert(sizeof(MagicCaster) == 0xC);

// 10
class MagicTarget
{
public:
	/*000*/virtual bool		ApplyEffect(MagicCaster *magicCaster, MagicItem *magicItem, ActiveEffect *activeEffect, bool arg4);
	/*004*/virtual Actor	*GetActor();
	/*008*/virtual ActiveEffectList	*GetEffectList();
	/*00C*/virtual bool		Unk_03();
	/*010*/virtual bool		CannotBeHit();
	/*014*/virtual void		Unk_05(ActiveEffect *activeEffect);
	/*018*/virtual void		Unk_06(ActiveEffect *activeEffect);
	/*01C*/virtual void		Unk_07(MagicCaster *magicCaster, ActiveEffect *activeEffect);
	/*020*/virtual void		Unk_08(MagicCaster *magicCaster, ActiveEffect *activeEffect);
	/*024*/virtual float	GetEffectMagnitudeModifier(MagicCaster *magicCaster, MagicItem *magicItem, ActiveEffect *activeEffect);
	/*028*/virtual void		Unk_0A(MagicCaster *magicCaster, MagicItem *magicItem, ActiveEffect *activeEffect, bool arg4);
	/*02C*/virtual bool		Unk_0B(MagicCaster *magicCaster, MagicItem *magicItem, ActiveEffect *activeEffect);

	struct SpellInfo
	{
		MagicItem		*magItem;
		MagicCaster		*magCaster;
		ActiveEffect	*activeEff;
	};

	UInt8				byte04;		// 04
	UInt8				byte05;		// 05
	UInt8				pad06[2];	// 06
	tList<SpellInfo>	spellInfos;	// 08

	void RemoveEffect(EffectItem *effItem);
};
static_assert(sizeof(MagicTarget) == 0x10);

class PathingSolution;
class DetailedActorPathHandler;
class ActorPathingMessageQueue;
class PathingAvoidNodeArray;

// 28
class PathingLocation
{
public:
	/*virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);*/

	void			**vtbl;		// 00
	NiPoint3		pos;		// 04
	UInt32			unk10[6];	// 10

	PathingLocation(TESObjectREFR *refr) {ThisCall(0x6DCD70, this, refr);}
};

// B0
class PathingRequest : public NiRefObject
{
public:
	/*08*/virtual void	Unk_02(void);
	/*0C*/virtual void	Unk_03(void);
	/*10*/virtual void	Unk_04(void);
	/*14*/virtual void	Unk_05(void);
	/*18*/virtual void	Unk_06(void);
	/*1C*/virtual void	Unk_07(void);
	/*20*/virtual void	Unk_08(void);

	struct ActorData
	{
		TESForm		*baseForm;
		void		*inventoryChanges;
		bool		isAlarmed;

		ActorData(Actor *actor) {ThisCall(0x502670, this, actor);}
	};

	UInt32					unk08;					// 08
	PathingLocation			start;					// 0C
	PathingLocation			dest;					// 34
	ActorData				actorData;				// 5C
	float					actorRadius;			// 68
	float					flt6C;					// 6C
	float					goalZDelta;				// 70
	float					targetRadius;			// 74
	float					centerRadius;			// 78
	NiVector3				targetPt;				// 7C
	float					unk88;					// 88
	float					goalAngle;				// 8C
	float					initialPathHeading;		// 90
	PathingAvoidNodeArray	*avoidNodeArray;		// 94
	UInt8					bCantOpenDoors;			// 98
	UInt8					bFaceTargetAtGoal;		// 99
	UInt8					byte9A;					// 9A
	UInt8					bAllowIncompletePath;	// 9B
	UInt8					byte9C;					// 9C
	UInt8					bCanSwim;				// 9D
	UInt8					bCanFly;				// 9E
	UInt8					byte9F;					// 9F
	UInt8					bInitialPathHeading;	// A0
	UInt8					byteA1;					// A1
	UInt8					bCurvedPath;			// A2
	UInt8					byteA3;					// A3
	UInt8					bIgnoreLocks;			// A4
	UInt8					padA5[3];				// A5
	UInt32					iSmoothingRetryCount;	// A8
	UInt8					byteAC;					// AC
	UInt8					padAD[3];				// AD
};

// 88
class ActorMover
{
public:
	virtual void		Destroy(bool doFree);
	virtual void		Unk_01(void);
	virtual void		ClearMovementFlag(UInt32 flag);
	virtual void		SetMovementFlags(UInt32 movementFlags);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		HandleTurnAnimationTimer(float timePassed);
	virtual void		Unk_07(void);
	virtual UInt32		GetMovementFlags();
	virtual void		Unk_09(void);
	virtual void		Unk_0A(void);
	virtual void		Unk_0B(void);
	virtual void		Unk_0C(void);
	virtual void		Unk_0D(void);
	virtual void		Unk_0E(void);

	enum MovementFlags
	{
		kMoveFlag_Forward =		1,
		kMoveFlag_Backward =	2,
		kMoveFlag_Left =		4,
		kMoveFlag_Right =		8,
		kMoveFlag_TurnLeft =	0x10,
		kMoveFlag_TurnRight =	0x20,
		kMoveFlag_IsKeyboard =	0x40,	// (returns that the movement is for non-controller)
		kMoveFlag_Walking =		0x100,
		kMoveFlag_Running =		0x200,
		kMoveFlag_Sneaking =	0x400,
		kMoveFlag_Swimming =	0x800,
		kMoveFlag_Jump =		0x1000,
		kMoveFlag_Flying =		0x2000,
		kMoveFlag_Fall =		0x4000,
		kMoveFlag_Slide =		0x8000
	};

	NiVector3					point04;			// 04
	NiVector3					overrideMovement;	// 10
	PathingRequest				*pathingRequest;	// 1C
	PathingSolution				*pathingSolution;	// 20
	DetailedActorPathHandler	*pathHandler;		// 24
	Actor						*actor;				// 28
	UInt32						unk2C;				// 2C
	ActorPathingMessageQueue	*pathingMsgQueue;	// 30
	UInt32						movementFlags1;		// 34
	UInt32						unk38;				// 38
	UInt32						movementFlags2;		// 3C
	UInt32						unk40;				// 40
	PathingLocation				pathingLocation;	// 44
	UInt32						unk6C;				// 6C
	UInt8						bPathingFailed;		// 70
	UInt8						byte71;				// 71
	UInt8						byte72;				// 72
	UInt8						byte73;				// 73
	UInt8						bWaitingOnPath;		// 74
	UInt8						byte75;				// 75
	UInt8						byte76;				// 76
	UInt8						bOverrideMovement;	// 77
	UInt32						unk78;				// 78
	UInt32						unk7C;				// 7C
	UInt32						unk80;				// 80
	UInt32						unk84;				// 84
};
static_assert(sizeof(ActorMover) == 0x88);

// A0
class PlayerMover : public ActorMover
{
public:
	float			flt88;				// 88
	float			flt8C;				// 8C
	float			flt90;				// 90
	UInt32			pcMovementFlags;	// 94
	UInt32			turnAnimFlags;		// 98
	float			turnAnimMinTime;	// 9C
};

typedef tList<BGSEntryPointPerkEntry> PerkEntryPointList;

struct ActorValueMod
{
	UInt8		avCode;
	UInt8		pad01[3];
	float		mod;
};

class Actor : public MobileObject
{
public:
	/*304*/virtual UInt8	IsGuard();
	/*308*/virtual void		SetGuard(UInt8 setTo);
	/*30C*/virtual void		Unk_C3(void);
	/*310*/virtual void		Unk_C4(void);
	/*314*/virtual void		Unk_C5(void);
	/*318*/virtual void		Unk_C6(void);
	/*31C*/virtual void		SetIgnoreCrime(bool ignoreCrime);
	/*320*/virtual bool		GetIgnoreCrime();
	/*324*/virtual void		Resurrect(UInt8 arg1, UInt8 arg2, UInt8 arg3);
	/*328*/virtual void		Unk_CA(void);
	/*32C*/virtual void		Unk_CB(void);
	/*330*/virtual void		Unk_CC(void);
	/*334*/virtual void		Unk_CD(void);
	/*338*/virtual void		DamageHealthAndFatigue(float healthDmg, float fatigueDmg, Actor *source);
	/*33C*/virtual void		DamageActionPoints(float amount); // checks GetIsGodMode before decreasing
	/*340*/virtual void		Unk_D0(void);
	/*344*/virtual int		CalculateDisposition(Actor *target, void *arg2);
	/*348*/virtual void		UpdateMovement(float arg1, UInt32 arg2);
	/*34C*/virtual void		Unk_D3(void);
	/*350*/virtual void		Unk_D4(void);
	/*354*/virtual float	GetDefaultTurningSpeed();
	/*358*/virtual bool		IsOverencumbered();
	/*35C*/virtual void		Unk_D7(void);
	/*360*/virtual bool		IsPlayerRef();
	/*364*/virtual void		Unk_D9(void);
	/*368*/virtual void		Unk_DA(void);
	/*36C*/virtual void		Unk_DB(void);
	/*370*/virtual void		Unk_DC(void);
	/*374*/virtual void		Unk_DD(void);
	/*378*/virtual void		Unk_DE(void);
	/*37C*/virtual TESRace	*GetRace();
	/*380*/virtual float	GetHandReachTimesCombatDistance();
	/*384*/virtual void		Unk_E1(void);
	/*388*/virtual void		Unk_E2(void);
	/*38C*/virtual bool		IsPushable();
	/*390*/virtual UInt32	GetActorType();	// Creature = 0, Character = 1, PlayerCharacter = 2
	/*394*/virtual void		SetActorValue(UInt32 avCode, float value);
	/*398*/virtual void		SetActorValueInt(UInt32 avCode, UInt32 value);
	/*39C*/virtual void		ModActorValue(UInt32 avCode, float modifier, Actor *attacker);
	/*3A0*/virtual void		Unk_E8(void);
	/*3A4*/virtual void		ForceActorValue(UInt32 avCode, float modifier, UInt32 arg3);
	/*3A8*/virtual void		ModActorValueInt(UInt32 avCode, int modifier, UInt32 arg3);
	/*3AC*/virtual void		DamageActorValue(UInt32 avCode, float damage, Actor *attacker);
	/*3B0*/virtual void		Unk_EC(void);
	/*3B4*/virtual void		Unk_ED(void);
	/*3B8*/virtual void		Unk_EE(void);
	/*3BC*/virtual ContChangesEntry *GetPreferedWeapon(UInt32 unk);
	/*3C0*/virtual void		Unk_F0(void);
	/*3C4*/virtual void		ResetArmorDRDT();
	/*3C8*/virtual bool		DamageItem(ContChangesEntry *itemEntry, float damage, int unused);
	/*3CC*/virtual void		DropItem(TESForm *itemForm, ExtraDataList *xDataList, SInt32 count, NiVector3 *pos, int arg5);
	/*3D0*/virtual void		DoActivate(TESObjectREFR *activatedRef, UInt32 count, bool arg3);
	/*3D4*/virtual void		Unk_F5(void);
	/*3D8*/virtual void		Unk_F6(void);
	/*3DC*/virtual void		Unk_F7(void);
	/*3E0*/virtual bool		AddActorEffect(SpellItem *actorEffect);
	/*3E4*/virtual bool		RemoveActorEffect(SpellItem *actorEffect);
	/*3E8*/virtual void		Reload(TESObjectWEAP *weapon, UInt32 animType, UInt8 hasExtendedClip);
	/*3EC*/virtual void		Reload2(TESObjectWEAP *weapon, UInt32 animType, UInt8 hasExtendedClip, UInt8 isInstantSwapHotkey);
	/*3F0*/virtual void		DecreaseAmmo(int amount);
	/*3F4*/virtual void		Unk_FD(void);
	/*3F8*/virtual CombatActors	*GetCombatGroup();
	/*3FC*/virtual void		SetCombatGroup(CombatActors *combatActors);
	/*400*/virtual void		Unk_100(void);
	/*404*/virtual void		Unk_101(void);
	/*408*/virtual void		Unk_102(void);
	/*40C*/virtual void		Unk_103(void);
	/*410*/virtual void		Unk_104(void);
	/*414*/virtual void		Unk_105(void);
	/*418*/virtual void		InitGetUpPackage();
	/*41C*/virtual void		SetAlpha(float alpha);
	/*420*/virtual float	GetAlpha();
	/*424*/virtual void		ForceAttackActor(Actor *target, CombatActors *combatGroup, UInt8 arg3, UInt32 arg4, UInt8 arg5, UInt32 arg6, UInt32 arg7, UInt32 arg8);
	/*428*/virtual CombatController	*GetCombatController();
	/*42C*/virtual Actor	*GetCombatTarget();
	/*430*/virtual void		UpdateCombat();
	/*434*/virtual void		StopCombat(Actor *target);
	/*438*/virtual void		Unk_10E(void);
	/*43C*/virtual float	GetTotalArmorDR();
	/*440*/virtual float	GetTotalArmorDT();
	/*444*/virtual UInt32	Unk_111(void);
	/*448*/virtual bool		IsTrespassing();
	/*44C*/virtual void		Unk_113(void);
	/*450*/virtual void		Unk_114(void);
	/*454*/virtual void		Unk_115(void);
	/*458*/virtual float	CalculateWalkSpeed();
	/*45C*/virtual float	CalculateRunSpeed();
	/*460*/virtual void		ModDisposition(Actor *target, float value);
	/*464*/virtual float	GetDisposition(Actor *target);
	/*468*/virtual void		ClearDisposition(Actor *target);
	/*46C*/virtual void		SetStartingPosition();
	/*470*/virtual bool		GetAttacked();
	/*474*/virtual void		Unk_11D(void);
	/*478*/virtual void		Unk_11E(void);
	/*47C*/virtual void		Unk_11F(void);
	/*480*/virtual void		Unk_120(void);
	/*484*/virtual void		Unk_121(void);
	/*488*/virtual void		RewardXP(UInt32 amount);
	/*48C*/virtual void		Unk_123(void);
	/*490*/virtual void		Unk_124(void);
	/*494*/virtual void		Unk_125(void);
	/*498*/virtual void		SetPerkRank(BGSPerk *perk, UInt8 rank, bool alt);
	/*49C*/virtual void		RemovePerk(BGSPerk *perk, bool alt);
	/*4A0*/virtual UInt8	GetPerkRank(BGSPerk *perk, bool alt);
	/*4A4*/virtual void		AddPerkEntryPoint(BGSEntryPointPerkEntry *perkEntry, bool alt);
	/*4A8*/virtual void		RemovePerkEntryPoint(BGSEntryPointPerkEntry *perkEntry, bool alt);
	/*4AC*/virtual PerkEntryPointList	*GetPerkEntryPointList(UInt8 entryPointID, bool alt);
	/*4B0*/virtual void		Unk_12C(void);
	/*4B4*/virtual bool		GetIsImmobileCreature();
	/*4B8*/virtual void		DoHealthDamage(Actor *attacker, float damage);
	/*4BC*/virtual void		Unk_12F(void);
	/*4C0*/virtual void		Unk_130(void);
	/*4C4*/virtual float	Unk_131(void);
	/*4C8*/virtual void		HandleHeadTracking(float arg1);
	/*4CC*/virtual void		UpdateHeadTrackingEmotions(UInt32 arg1);
	/*4D0*/virtual void		Unk_134(void);
	/*4D4*/virtual void		Unk_135(void);
	/*4D8*/virtual NiVector3	*GetAnticipatedLocation(NiVector3 *resPos, float time);

	enum LifeStates
	{
		kLifeState_Alive =			0,
		kLifeState_Dying =			1,
		kLifeState_Dead =			2,
		kLifeState_Unconscious =	3,
		kLifeState_Reanimate =		4,
		kLifeState_Restrained =		5
	};

	struct Disposition
	{
		float	value;
		Actor	*target;
	};
	
	MagicCaster			magicCaster;			// 088
	MagicTarget			magicTarget;			// 094
	ActorValueOwner		avOwner;				// 0A4
	CachedValuesOwner	cvOwner;				// 0A8

	bhkRagdollController				*ragDollController;			// 0AC
	bhkRagdollPenetrationUtil			*ragDollPentrationUtil;		// 0B0
	UInt32								unk0B4;						// 0B4-
	float								flt0B8;						// 0B8
	bool								bAIState;					// 0BC-
	UInt8								byte0BD;					// 0BD
	UInt8								byte0BE;					// 0BE
	UInt8								byte0BF;					// 0BF
	Actor								*killer;					// 0C0
	UInt8								byte0C4;					// 0C4-
	UInt8								byte0C5;					// 0C5
	UInt8								byte0C6;					// 0C6
	UInt8								byte0C7;					// 0C7
	float								flt0C8;						// 0C8
	float								flt0CC;						// 0CC
	tList<ActorValueMod>				forceAVList;				// 0D0
	UInt8								byte0D8;					// 0D8
	UInt8								byte0D9;					// 0D9
	UInt8								byte0DA;					// 0DA
	UInt8								byte0DB;					// 0DB
	UInt32								unk0DC;						// 0DC
	tList<ActorValueMod>				setAVList;					// 0E0
	UInt8								byte0E8;					// 0E8	const 1
	UInt8								byte0E9;					// 0E9
	UInt8								byte0EA;					// 0EA
	UInt8								byte0EB;					// 0EB
	UInt32								unk0EC;						// 0EC
	UInt8								byte0F0;					// 0F0-
	UInt8								byte0F1;					// 0F1-
	UInt8								byte0F2;					// 0F2
	UInt8								byte0F3;					// 0F3
	tList<void>							list0F4;					// 0F4
	tList<Disposition>					dispositionList;			// 0FC
	bool								isInCombat;					// 104
	UInt8								jipActorFlags1;				// 105
	UInt8								jipActorFlags2;				// 106
	UInt8								jipActorFlags3;				// 107
	UInt32								lifeState;					// 108	saved as byte HasHealth
	UInt32								criticalStage;				// 10C
	UInt32								unk110;						// 110-
	float								painSoundTimer;				// 114
	UInt8								byte118;					// 118-
	UInt8								byte119;					// 119+
	UInt8								byte11A;					// 11A
	UInt8								byte11B;					// 11B
	UInt32								unk11C;						// 11C-
	UInt32								resetTime;					// 120-
	bool								forceRun;					// 124
	bool								forceSneak;					// 125
	UInt8								byte126;					// 126-
	bool								searchingForEnemies;		// 127
	Actor								*combatTarget;				// 128
	BSSimpleArray<Actor*>				*combatTargets;				// 12C
	BSSimpleArray<Actor*>				*combatAllies;				// 130
	UInt8								byte134;					// 134-
	UInt8								byte135;					// 135+
	UInt8								byte136;					// 136
	UInt8								byte137;					// 137
	UInt32								unk138;						// 138-
	UInt32								minorCrimeCount;			// 13C
	UInt32								actorFlags;					// 140	0x80000000 - IsEssential
	bool								ignoreCrime;				// 144
	UInt8								byte145;					// 145	Has to do with package evaluation
	UInt8								byte146;					// 146	Has to do with package evaluation
	UInt8								byte147;					// 147
	UInt32								unk148;						// 148-
	UInt8								inWater;					// 14C
	UInt8								isSwimming;					// 14D
	UInt8								byte14E;					// 14E
	UInt8								byte14F;					// 14F
	UInt32								unk150;						// 150-
	float								flt154;						// 154
	float								flt158;						// 158
	UInt8								byte15C;					// 15C-
	UInt8								byte15D;					// 15D-
	UInt8								byte15E;					// 15E
	UInt8								byte15F;					// 15F
	NiVector3							startingPos;				// 160
	float								startingZRot;				// 16C
	TESForm								*startingWorldOrCell;		// 170
	UInt8								byte174;					// 174-
	UInt8								byte175;					// 175-
	UInt8								byte176;					// 176
	UInt8								byte177;					// 177
	float								flt178;						// 178
	float								weaponSpread1;				// 17C
	float								weaponSpread2;				// 180
	float								weaponSpread3;				// 184
	float								flt188;						// 188
	UInt8								byte18C;					// 18C-
	bool								isTeammate;					// 18D
	UInt8								byte18E;					// 18E-
	UInt8								byte18F;					// 18F
	ActorMover							*actorMover;				// 190
	UInt32								unk194;						// 194-
	UInt32								unk198;						// 198-
	float								flt19C;						// 19C
	UInt32								unk1A0;						// 1A0-
	UInt32								speechExpression;			// 1A4
	UInt32								emotionValue;				// 1A8
	UInt32								sitSleepState;				// 1AC-
	UInt8								isImmobileCreature;			// 1B0-
	bool								forceHit;					// 1B1-
	UInt8								byte1B2;					// 1B2
	UInt8								byte1B3;					// 1B3

	__forceinline void EquipItem(TESForm *objType, UInt32 equipCount = 1, ExtraDataList *itemExtraList = NULL, bool applyEnchantment = 1, bool lockEquip = 0, bool noMessage = 1)
	{
		ThisCall(0x88C650, this, objType, equipCount, itemExtraList, applyEnchantment, lockEquip, noMessage);
	}
	__forceinline void UnequipItem(TESForm *objType, UInt32 unequipCount = 1, ExtraDataList *itemExtraList = NULL, bool removeEnchantment = 1, bool lockUnequip = 0, bool noMessage = 1)
	{
		ThisCall(0x88C790, this, objType, unequipCount, itemExtraList, removeEnchantment, lockUnequip, noMessage);
	}

	bool GetDead() const {return (lifeState == 1) || (lifeState == 2);}
	bool GetRestrained() const {return lifeState == 5;}

	TESActorBase *GetActorBase() const;
	bool GetLOS(Actor *target) const;
	char GetCurrentAIPackage() const;
	char GetCurrentAIProcedure() const;
	bool IsFleeing() const;
	ContChangesEntry *GetWeaponInfo() const;
	ContChangesEntry *GetAmmoInfo() const;
	TESObjectWEAP *GetEquippedWeapon() const;
	bool IsItemEquipped(TESForm *item) const;
	bool EquippedWeaponHasMod(UInt32 modType) const;
	UInt8 EquippedWeaponHasScope() const;
	UInt8 EquippedWeaponSilenced() const;
	bool IsSneaking() const;
	void StopCombat();
	bool __fastcall IsInCombatWith(Actor *target) const;
	int __fastcall GetDetectionValue(Actor *detected) const;
	TESPackage *GetStablePackage() const;
	PackageInfo *GetPackageInfo() const;
	TESObjectREFR *GetPackageTarget() const;
	TESCombatStyle *GetCombatStyle() const;
	SInt8 GetKnockedState() const;
	bool IsWeaponOut() const;
	void UpdateActiveEffects(MagicItem *magicItem, EffectItem *effItem, bool addNew);
	bool GetIsGhost() const;
	float GetRadiationLevel() const;
	BackUpPackage *AddBackUpPackage(TESObjectREFR *targetRef, TESObjectCELL *targetCell, UInt32 flags);
	void __fastcall TurnToFaceObject(TESObjectREFR *target);
	void __vectorcall TurnAngle(float angle);
	void PlayAnimGroup(UInt32 animGroupID);
	UInt32 GetLevel() const;
	double GetKillXP() const;
	void __fastcall GetHitDataValue(UInt32 valueType, double *result) const;
	void DismemberLimb(UInt32 bodyPartID, bool explode);
	void EquipItemAlt(ContChangesEntry *entry, UInt32 noUnequip = 0, UInt32 noMessage = 1);
	bool HasNoPath() const;
	bool CanBePushed() const;
	float __vectorcall AdjustPushForce(float baseForce);
	void PushActor(float force, float angle, TESObjectREFR *originRef);
	int GetGroundMaterial() const;
	void RefreshAnimData();
	double GetPathingDistance(TESObjectREFR *target);
};

extern float s_moveAwayDistance;

// 1C0
class Creature : public Actor
{
public:
	/*4DC*/virtual void	Unk_137(void);

	UInt8			byte1B4;			// 1B4
	UInt8			pad1B5[3];			// 1B5
	UInt32			unk1B8[2];			// 1B8
};

// 1C8
class Character : public Actor
{
public:
	/*4DC*/virtual void	Unk_137(void);
	/*4E0*/virtual void	Unk_138(void);

	BipedAnim		*bipedAnims;		// 1B4
	float			totalArmorDR;		// 1B8
	float			totalArmorDT;		// 1BC
	UInt8			isTrespassing;		// 1C0
	UInt8			isGuard;			// 1C1
	UInt16			unk1C2;				// 1C2
	float			unk1C4;				// 1C4

	void RefreshWornDTDR()
	{
		totalArmorDR = -1.0F;
		totalArmorDT = -1.0F;
		ThisCall(0x8D2110, this);
		ThisCall(0x8D22B0, this);
	}
};

struct ParentSpaceNode;
struct TeleportLink;

struct MusicMarker
{
	TESObjectREFR			*markerRef;
	ExtraAudioMarker::Data	*markerData;

	MusicMarker(TESObjectREFR *_markerRef, ExtraAudioMarker::Data *data) : markerRef(_markerRef), markerData(data) {}
	MusicMarker(const MusicMarker &from) : markerRef(from.markerRef), markerData(from.markerData) {}

	bool operator<(const MusicMarker &rhs) const {return markerData->mediaLocCtrlID < rhs.markerData->mediaLocCtrlID;}
};

struct PerkRank
{
	BGSPerk		*perk;
	UInt8		rank;
	UInt8		pad05[3];
};

class PerkRankFinder
{
	BGSPerk		*m_perk;

public:
	PerkRankFinder(BGSPerk *_perk) : m_perk(_perk) {}

	bool operator==(PerkRank *perkRank) const {return perkRank->perk == m_perk;}
};

struct CasinoStats
{
	UInt32		casinoRefID;
	UInt32		earnings;
	UInt16		unk08;
	UInt8		gap0A[2];
};

// E50
class PlayerCharacter : public Character
{
public:
	// used to flag controls as disabled in disabledControlFlags
	enum
	{
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};

	/*4E4*/virtual void		Unk_139(void);
	/*4E8*/virtual tList<PerkRank>	*GetPerkRankList(bool forTeammates);

	struct MapMarkerInfo
	{
		ExtraMapMarker::MarkerData	*markerData;
		TESObjectREFR				*markerRef;
	};

	struct CompassTarget
	{
		Actor		*target;
		UInt8		isHostile;
		UInt8		isDetected;
		UInt8		pad06[2];
	};

	struct PCLevelData
	{
		UInt8		byte00;
		UInt8		pad01[3];
		UInt32		xpToNextLevel;
		UInt8		byte08;
		UInt8		pad09[3];
	};

	struct CameraCollision		// C'tor @ 0x620850
	{
		bhkSimpleShapePhantom	*phantom00;
		bhkSimpleShapePhantom	*phantom04;
	};

	UInt32								unk1C8[9];				// 1C8	208 could be a DialogPackage
	PositionRequest						*posRequest;			// 1EC
	TESForm								*queuedWeapon;			// 1F0
	UInt32								unk1F4;					// 1F4
	UInt8								byte1F8;				// 1F8
	UInt8								pad1F9[3];				// 1F9
	UInt32								unk1FC[2];				// 1FC
	UInt8								byte204;				// 204
	UInt8								byte205;				// 205
	UInt8								recalcQuestTargets;		// 206
	UInt8								byte207;				// 207
	TESPackage							*package208;			// 208
	void								*unk20C;				// 20C
	tList<ActiveEffect>					*activeEffects;			// 210
	MagicItem							*pcMagicItem;			// 214
	MagicTarget							*pcMagicTarget;			// 218
	CameraCollision						*cameaCollision;		// 21C
	UInt32								unk220[8];				// 220	224 is a package of type 1C
	bool								showQuestItems;			// 240
	UInt8								byte241;				// 241
	UInt8								byte242;				// 242
	UInt8								byte243;				// 243
	float								unk244[77];				// 244	have to be a set of ActorValue
	float								permAVMods[77];			// 378	have to be a set of ActorValue
	float								flt4AC;					// 4AC
	float								actorValues4B0[77];		// 4B0	have to be a set of ActorValue
	tList<BGSNote>						notes;					// 5E4
	ImageSpaceModifierInstanceDOF		*unk5EC;				// 5EC
	ImageSpaceModifierInstanceDOF		*unk5F0;				// 5F0
	ImageSpaceModifierInstanceDRB		*unk5F4;				// 5F4
	UInt8								byte5F8;				// 5F8
	UInt8								byte5F9;				// 5F9
	UInt8								byte5FA;				// 5FA
	UInt8								byte5FB;				// 5FB
	tList<TESObjectREFR>				teammates;				// 5FC
	TESObjectREFR						*lastExteriorDoor;		// 604
	UInt8								isPlayingAttackSound;	// 608
	UInt8								pad609[3];				// 609
	tList<void>							*actionList;			// 60C
	tList<CasinoStats>					*casinoDataList;		// 610
	tList<TESCaravanCard>				*caravanCards1;			// 614
	tList<TESCaravanCard>				*caravanCards2;			// 618
	UInt32								unk61C[6];				// 61C
	void								*ptr634;				// 634	bhkMouseSpringAction when there's a grabbed reference
	TESObjectREFR						*grabbedRef;			// 638
	UInt32								unk63C;					// 63C
	UInt32								unk640;					// 640
	float								grabbedWeight;			// 644
	UInt8								byte648;				// 648
	UInt8								byte649;				// 649
	bool								is3rdPerson;			// 64A
	bool								byte64B;				// 64B
	bool								byte64C;				// 64C
	UInt8								byte64D;				// 64D
	UInt8								byte64E;				// 64E
	bool								isUsingScope;			// 64F
	UInt8								byte650;				// 650
	bool								alwaysRun;				// 651
	bool								autoMove;				// 652
	UInt8								byte653;				// 653
	UInt32								sleepHours;				// 654
	UInt8								isResting;				// 658	Something to do with SleepDeprivation; see 0x969DCF
	UInt8								byte659;				// 659
	UInt8								byte65A;				// 65A
	UInt8								byte65B;				// 65B
	float								flt65C;					// 65C
	UInt32								unk660[3];				// 660
	UInt8								byte66C;				// 66C
	UInt8								byte66D;				// 66D
	UInt8								byte66E;				// 66E
	UInt8								byte66F;				// 66F
	float								worldFOV;				// 670
	float								firstPersonFOV;			// 674
	float								flt678;					// 678
	UInt32								unk67C;					// 67C
	UInt8								pcControlFlags;			// 680
	UInt8								byte681;				// 681
	UInt8								byte682;				// 682
	UInt8								byte683;				// 683
	UInt32								unk684[2];				// 684
	BipedAnim							*bipedAnims1stPerson;	// 68C
	AnimData							*animData1stPerson;		// 690
	NiNode								*node1stPerson;			// 694
	float								flt698;					// 698
	UInt32								unk69C[3];				// 69C
	tList<TESTopic>						topicList;				// 6A8
	UInt32								unk6B0[2];				// 6B0
	TESQuest							*activeQuest;			// 6B8
	tList<BGSQuestObjective>			questObjectiveList;		// 6BC
	tList<ObjectiveTarget>				questTargetList;		// 6C4
	UInt32								unk6CC[5];				// 6CC
	float								sortActorDistanceListTimer;	// 6E0
	float								seatedRotation;			// 6E4
	UInt32								unk6E8;					// 6E8
	UInt32								unk6EC;					// 6EC
	UInt32								playerSpell;			// 6F0
	TESObjectREFR						*placedMarkerRef;		// 6F4
	ObjectiveTargetData					placedMarkerData;		// 6F8
	float 								timeGrenadeHeld;		// 730
	UInt32								unk734[10];				// 734
	bool								inCharGen;				// 75C
	UInt8								byte75D;				// 75D
	UInt8								canUseTelekinesis;		// 75E
	UInt8								byte75F;				// 75F
	TESRegion							*currentRegion;			// 760
	TESRegionList						regionsList;			// 764
	UInt32								unk774[6];				// 774
	UInt32								initialTickCount;		// 78C
	UInt32								timePlayedCurrGame;		// 790	ms
	UInt32								unk794[6];				// 794
	TESForm								*pcWorldOrCell;			// 7AC
	UInt32								unk7B0;					// 7B0
	BGSMusicType						*musicType;				// 7B4
	int									gameDifficulty;			// 7B8
	UInt32								isHardcore;				// 7BC
	UInt32								killCamMode;			// 7C0
	UInt8								inCombatWithGuard;		// 7C4
	bool								isYoung;				// 7C5
	bool								isToddler;				// 7C6
	bool								canUsePA;				// 7C7
	tList<MapMarkerInfo>				mapMarkers;				// 7C8
	TESWorldSpace						*mapMarkersWorldspace;	// 7D0
	tList<MusicMarker>					musicMarkers;			// 7D4
	MusicMarker							*currMusicMarker;		// 7DC
	float								flycamZRot;				// 7E0
	float								flycamXRot;				// 7E4
	NiVector3							flycamPos;				// 7E8
	UInt32								unk7F4[33];				// 7F4
	PCLevelData							*pcLevelData;			// 878
	tList<PerkRank>						perkRanksPC;			// 87C
	PerkEntryPointList					perkEntriesPC[74];		// 884
	tList<PerkRank>						perkRanksTM;			// AD4
	PerkEntryPointList					perkEntriesTM[74];		// ADC
	Actor								*autoAimActor;			// D2C
	NiVector3							vecD30;					// D30
	NiObject							*unkD3C;				// D3C
	UInt32								unkD40;					// D40
	Actor								*reticleActor;			// D44
	tList<CompassTarget>				*compassTargets;		// D48
	UInt32								unkD4C;					// D4C
	float								lastAmmoSwapTime;		// D50
	UInt8								shouldOpenPipboy;		// D54
	UInt8								unusedByteD55;			// D55
	UInt8								unusedByteD56;			// D56
	UInt8								unusedByteD57;			// D57	Used by SneakBoundingBoxFixHook
	NiVector3							cameraPos3rdPerson;		// D58
	CombatActors						*combatActors;			// D64
	UInt32								teammateCount;			// D68
	UInt32								unkD6C[5];				// D6C
	NiNode								*niNodeD80;				// D80
	UInt32								unkD84[12];				// D84
	NiNode								*niNodeDB4;				// DB4
	UInt32								unkDB8[7];				// DB8
	NiVector3							cameraPos1stPerson;		// DD4
	NiVector3							cameraPos;				// DE0
	bhkRigidBody						*rigidBody;				// DEC
	bool								pcInCombat;				// DF0
	bool								pcUnseen;				// DF1
	UInt8								isLODApocalypse;		// DF2
	UInt8								byteDF3;				// DF3
	BSSimpleArray<ContChangesEntry*>	rockItLauncherContainer;// DF4
	float								rockItLauncherWeight;	// E04
	UInt8								nightVisionApplied;		// E08
	UInt8								unusedByteE09;			// E09
	UInt8								unusedByteE0A;			// E0A
	UInt8								unusedByteE0B;			// E0B
	TESReputation						*lastModifiedRep;		// E0C
	UInt32								unkE10[2];				// E10
	float								killCamTimer;			// E18
	float								killCamCooldown;		// E1C
	UInt8								byteE20;				// E20
	UInt8								isUsingTurbo;			// E21
	UInt8								byteE22;				// E22
	UInt8								byteE23;				// E23
	float								fltE24;					// E24
	float								counterAttackTimer;		// E28
	UInt8								byteE2C;				// E2C
	UInt8								cateyeEnabled;			// E2D
	UInt8								spottingEnabled;		// E2E
	UInt8								byteE2F;				// E2F
	UInt32								itemDetectionTimer;		// E30
	NiNode								*ironSightNode;			// E34
	bool								noHardcoreTracking;		// E38	Appears to be unused
	bool								skipHCNeedsUpdate;		// E39
	UInt8								byteE3A;				// E3A
	UInt8								byteE3B;				// E3B
	BSSimpleArray<TESAmmo*>				hotkeyedWeaponAmmos;	// E3C
	UInt32								unkE4C;					// E4C

	bool IsThirdPerson() const {return is3rdPerson;}
	UInt32 GetMovementFlags() const {return actorMover->GetMovementFlags();}	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool IsPlayerSwimming() const {return (actorMover->GetMovementFlags() & 0x800) != 0;}

	__forceinline static PlayerCharacter *GetSingleton() {return *(PlayerCharacter**)0x11DEA3C;}

	__forceinline bool ToggleFirstPerson(bool toggleON)
	{
		return ThisCall<bool>(0x950110, this, toggleON);
	}
	int GetDetectionState();

	void ToggleSneak(bool toggle);
};
static_assert(sizeof(PlayerCharacter) == 0xE50);

extern PlayerCharacter *g_thePlayer;

// 150
class Projectile : public MobileObject
{
public:
	enum
	{
		kProjType_Beam =	1,
		kProjType_Flame,
		kProjType_Grenade,
		kProjType_Missile,
		kProjType_ContinuousBeam
	};

	/*304*/virtual UInt32	GetProjectileType();
	/*308*/virtual void	Do3DLoaded();
	/*30C*/virtual void	HandleTracer();
	/*310*/virtual void	UpdateProjectile(float timePassed);
	/*314*/virtual bool	ProcessImpact();
	/*318*/virtual bool	IsProximityTriggered();
	/*31C*/virtual void	Unk_C7(void);
	/*320*/virtual bool	DisarmPlacedExplosives(TESObjectREFR *refr, bool bSilent);
	/*324*/virtual void	Unk_C9(void);
	/*328*/virtual void	Unk_CA(void);
	/*32C*/virtual void	Unk_CB(void);

	enum
	{
		kProjFlag_IsHitScan =					0x1,
		kProjFlag_HitScanNonTracer =			0x2,
		kProjFlag_IsStuck =						0x4,
		kProjFlag_Bit03Unk =					0x8,
		kProjFlag_IsTracer =					0x10,
		kProjFlag_Fades =						0x20,
		kProjFlag_HasGravity =					0x40,
		kProjFlag_Bit07Unk =					0x80,
		kProjFlag_Bit08Unk =					0x100,
		kProjFlag_MineDisarmed =				0x200,
		kProjFlag_IsPickpocketLiveExplosive =	0x400,
		kProjFlag_Bit0BUnk =					0x800,
		kProjFlag_AlwaysHit =					0x1000,
		kProjFlag_Bit0DUnk =					0x2000,
		kProjFlag_MineIgnoresPlayer =			0x4000,
		kProjFlag_Bit0FUnk =					0x8000,		// Don't apply source-weapon's damage upon impact
		kProjFlag_Bit10Unk =					0x10000,
		kProjFlag_IgnoreGravity =				0x20000,
		kProjFlag_Bit12Unk =					0x40000,
		kProjFlag_SourceActorInCombat =			0x80000,
		kProjFlag_Bit14Unk =					0x100000
	};

	struct ImpactData
	{
		TESObjectREFR	*refr;			// 00
		NiVector3		pos;			// 04
		NiVector3		rot;			// 10
		hkpRigidBody	*rigidBody;		// 1C
		UInt32			materialType;	// 20
		SInt32			hitLocation;	// 24
		UInt8			byte28;			// 28
		UInt8			byte29;			// 29
		UInt8			byte2A;			// 2A
		UInt8			byte2B;			// 2B
		UInt32			unk2C;			// 2C
	};

	tList<ImpactData>	impactDataList;		// 088
	UInt8				hasImpacted;		// 090
	UInt8				pad091[3];			// 091
	NiTransform			transform;			// 094
	UInt32				projFlags;			// 0C8
	float				power;				// 0CC
	float				speedMult;			// 0D0
	float				flt0D4;				// 0D4
	float				lifeTime;			// 0D8
	float				hitDamage;			// 0DC
	float				alpha;				// 0E0
	float				detonationTime;		// 0E4
	float				blinkTimer;			// 0E8
	float				angMomentumZ;		// 0EC
	float				angMomentumX;		// 0F0
	float				wpnHealthPerc;		// 0F4
	TESObjectWEAP		*sourceWeap;		// 0F8
	TESObjectREFR		*sourceRef;			// 0FC
	TESObjectREFR		*liveGrenadeTarget;	// 100
	NiVector3			vector104;			// 104
	float				distTravelled;		// 110
	NiPointLight		*projLight;			// 114
	UInt8				byte118;			// 118
	UInt8				pad119[3];			// 119
	NiNode				*node11C;			// 11C
	UInt32				unk120;				// 120
	float				decalSize;			// 124
	Sound				passPlayerSound;	// 128
	Sound				countDownSound;		// 134
	UInt32				unk140;				// 140
	ContChangesEntry	*rockItEntry;		// 144
	UInt8				byte148;			// 148
	bool				hasSplitBeams;		// 149	JIP only!
	UInt8				numProjectiles;		// 14A	JIP only!
	UInt8				pad14B;				// 14B
	float				range;				// 14C

	void __fastcall GetData(UInt32 dataType, double *result) const;

	TESAmmo *GetAmmo() const {return numProjectiles ? extraDataList.ammo : nullptr;}
};
static_assert(sizeof(Projectile) == 0x150);

// 154
class BeamProjectile : public Projectile
{
public:
	NiRefObject		*object150;		// 150
};

// 158
class ContinuousBeamProjectile : public Projectile
{
public:
	NiRefObject		*object150;		// 150
	UInt32			unk154;			// 154
};

// 158
class FlameProjectile : public Projectile
{
public:
	virtual void	Unk_CC(void);

	float		flt150;		// 150
	float		flt154;		// 154
};

// 154
class GrenadeProjectile : public Projectile
{
public:
	virtual void	Unk_CC(void);

	UInt8		byte150;		// 150
	UInt8		pad151[3];		// 151
};

// 160
class MissileProjectile : public Projectile
{
public:
	virtual void	Unk_CC(void);

	UInt32		unk150;			// 150
	UInt8		byte154;		// 154
	UInt8		pad155[3];		// 155
	float		flt158;			// 158
	float		flt15C;			// 15C
};

// 24
class NonActorMagicCaster : public BSExtraData
{
public:
	MagicCaster		caster;		// 0C
};

// 104
class Explosion : public MobileObject
{
public:
	virtual void	CheckInit3D();

	float				unk088;			// 088	init'd to 0
	float				unk08C;			// 08C	init'd to 3.0
	float				unk090;			// 090	init'd to 0
	float				radius;			// 094
	float				ISradius;		// 098
	float				unk09C;			// 09C	init'd to 1.0
	NiRefObject			*object0A0;		// 0A0
	tList<void>			list0A4;		// 0A4
	Sound				sound0AC;		// 0AC
	Sound				sound0B8;		// 0B8
	NiPointLight		*pointLight;	// 0C4
	TESObjectREFR		*sourceRef;		// 0C8
	UInt32				unk0CC;			// 0CC
	NiRefObject			*object0D0;		// 0D0
	UInt8				byte0D4;		// 0D4
	UInt8				byte0D5;		// 0D5
	UInt8				byte0D6;		// 0D6
	UInt8				byte0D7;		// 0D7
	UInt8				byte0D8;		// 0D8
	UInt8				byte0D9;		// 0D9
	UInt8				pad0DA[2];		// 0DA
	NonActorMagicCaster	*caster;		// 0DC
	UInt32				unk0E0;			// 0E0
	UInt32				unk0E4;			// 0E4
	NiVector3			vec0E8;			// 0E8
	NiVector3			vec0F4;			// 0F4
	float				unk100;			// 100	init'd to -1.0
};
static_assert(sizeof(Explosion) == 0x104);