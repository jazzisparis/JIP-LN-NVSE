#pragma once

// 68
class TESObjectREFR : public TESForm
{
public:
	MEMBER_FN_PREFIX(TESObjectREFR);

	virtual void		Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void		Unk_4F(void);
	virtual void		Unk_50(void);
	virtual void		Unk_51(void);
	virtual bool		CastShadows();
	virtual void		Unk_53(void);
	virtual void		Unk_54(void);
	virtual void		Unk_55(void);
	virtual void		Unk_56(void);
	virtual bool		IsObstacle();
	virtual void		Unk_58(void);
	virtual void		Unk_59(void);
	virtual void		Unk_5A(void);
	virtual void		Unk_5B(void);
	virtual void		Unk_5C(void);
	virtual void		Unk_5D(void);
	virtual void		Unk_5E(void);
	virtual TESObjectREFR	*RemoveItem(TESForm *toRemove, BaseExtraList *extraList, UInt32 quantity, bool keepOwner, bool drop, TESObjectREFR *destRef,
		UInt32 unk6, UInt32 unk7, bool unk8, bool unk9);
	virtual void		Unk_60(void);
	virtual bool		EquipObject(TESForm *item, UInt32 count, ExtraDataList *xData, bool lockEquip);
	virtual void		Unk_62(void);
	virtual void		Unk_63(void);
	virtual void		AddItem(TESForm *item, ExtraDataList *xDataList, UInt32 quantity);
	virtual void		Unk_65(void);
	virtual void		Unk_66(void);
	virtual void		Unk_67(void);					// Actor: GetMagicEffectList
	virtual bool		GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	virtual UInt32		GetActorUnk0148();			// result can be interchanged with baseForm, so TESForm* ?
	virtual void		SetActorUnk0148(UInt32 arg0);
	virtual void		Unk_6B(void);
	virtual void		Unk_6C(void);	// REFR: GetBSFaceGenNiNodeSkinned
	virtual void		Unk_6D(void);	// REFR: calls 006C
	virtual void		Unk_6E(void);	// MobileActor: calls 006D then NiNode::Func0040
	virtual void		Unk_6F(void);
	virtual bool		Unload3D();
	virtual void		AnimateNiNode();
	virtual NiNode		*GenerateNiNode(bool arg1);
	virtual void		Set3D(NiNode* niNode, bool unloadArt);
	virtual NiNode		*GetNiNode();
	virtual void		Unk_75(void);
	virtual void		Unk_76(void);
	virtual void		Unk_77(void);
	virtual void		Unk_78(void);
	virtual AnimData	*GetAnimData();			// 0079
	virtual ValidBip01Names * GetValidBip01Names(void);	// 007A	Character only
	virtual ValidBip01Names * CallGetValidBip01Names(void);
	virtual void		SetValidBip01Names(ValidBip01Names *validBip01Names);
	virtual NiVector3	*GetPos();
	virtual void		Unk_7E(UInt32 arg0);
	virtual void		Unk_7F(void);
	virtual void		Unk_80(UInt32 arg0);
	virtual void		Unk_81(UInt32 arg0);
	virtual void		Unk_82(void);
	virtual NiNode		*GetProjectileNode();
	virtual void		Unk_84(UInt32 arg0);
	virtual UInt32		Unk_85(void);			
	virtual bool		IsCharacter();			// return false for Actor and Creature, true for character and PlayerCharacter
	virtual bool		IsCreature();
	virtual bool		IsExplosion();
	virtual bool		IsProjectile();
	virtual void		Unk_8A(void);			// SetParentCell (Interior only ?)
	virtual bool		HasHealth(bool arg0);	// HasHealth (baseForm health > 0 or Flags bit23 set)
	virtual bool		GetHasKnockedState();
	virtual bool		Unk_8D(void);
	virtual void		Unk_8E(void);
	virtual void		Unk_8F(void);
	virtual void		Unk_90(void);

	enum {
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
		TESObjectREFR	*waterRef;		// 00
		UInt32			unk04;			// 04	0-0x13 when fully-underwater; exterior only
		float			waterLevel;		// 08
		float			unk0C;			// 0C
		UInt32			unk10;			// 10
		NiNode			*niNode14;		// 14
		NiNode			*niNode18;		// 18
	};

	struct EditorData {
		UInt32	unk00;	// 00
	};
	// 0C

#ifdef EDITOR
	EditorData	editorData;			// +04
#endif

	TESChildCell	childCell;				// 018

	TESSound		*loopSound;				// 01C

	TESForm			*baseForm;				// 020
	
	float			rotX, rotY, rotZ;		// 024 - either public or accessed via simple inline accessor common to all child classes
	float			posX, posY, posZ;		// 030 - seems to be private
	float			scale;					// 03C 

	TESObjectCELL	*parentCell;			// 040
	ExtraDataList	extraDataList;			// 044
	RenderState		*renderState;			// 064

	ScriptEventList *GetEventList() const;

	bool IsTaken() const {return (flags & kFlags_Taken) != 0;}
	bool IsPersistent() const {return (flags & kFlags_Persistent) != 0;}
	bool IsTemporary() const {return (flags & kFlags_Temporary) != 0;}
	bool IsDeleted() const {return (flags & kFlags_Deleted) != 0;}
	bool IsDestroyed() const {return (flags & kFlags_Destroyed) != 0;}

	NiVector3 *PosVector() {return (NiVector3*)&posX;}
	CoordXY *PosXY() {return (CoordXY*)&posX;}

	void Update3D();
	TESContainer *GetContainer();
	bool IsMapMarker();

	TESForm *GetBaseForm();
	bool GetDisabled();
	ExtraContainerChanges *GetOrCreateContainerChanges();
	ExtraContainerChanges::EntryDataList *GetContainerChangesList();
	ContChangesEntry *GetContainerChangesEntry(TESForm *itemForm);
	SInt32 GetItemCount(TESForm *form);
	void AddItemAlt(TESForm *item, UInt32 count, float condition, bool doEquip);
	void RemoveItemTarget(TESForm *itemForm, TESObjectREFR *target, SInt32 quantity, bool keepOwner);
	bool GetInventoryItems(UInt8 typeID);
	TESObjectCELL *GetParentCell();
	TESWorldSpace *GetParentWorld();
	bool __fastcall GetInSameCellOrWorld(TESObjectREFR *target);
	float __vectorcall GetDistance(TESObjectREFR *target);
	void SetPos(NiVector3 *posVector);
	void SetAngle(NiVector3 *rotVector);
	bool MoveToCell(TESForm *worldOrCell, NiVector3 *posVector);
	bool Disable();
	void DeleteReference();
	bhkCharacterController *GetCharacterController();
	TESObjectREFR *GetMerchantContainer();
	float GetWaterImmersionPerc();
	bool IsMobile();
	bool IsGrabbable();
	void SwapTexture(const char *blockName, const char *filePath, UInt32 texIdx);
	bool SetLinkedRef(TESObjectREFR *linkObj, UInt8 modIdx);
	bool ValidForHooks();
	NiAVObject* __fastcall GetNiBlock(const char *blockName);
	NiNode* __fastcall GetNode(const char *nodeName);
	NiNode *GetNiNodeCopyIfTemplate();
	hkpRigidBody *GetRigidBody(const char *nodeName);
	bool RunScriptSource(const char *sourceStr);

	static TESObjectREFR* __stdcall Create(bool bTemp = false);

	MEMBER_FN_PREFIX(TESObjectREFR);
	DEFINE_MEMBER_FN(Activate, bool, 0x00573170, TESObjectREFR*, UInt32, UInt32, UInt32);	// Usage Activate(actionRef, 0, 0, 1); found inside Cmd_Activate_Execute as the last call (190 bytes)
};
STATIC_ASSERT(sizeof(TESObjectREFR) == 0x068);

// 88
class MobileObject : public TESObjectREFR
{
public:
	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Unk_94(void);
	virtual void		Unk_95(void);
	virtual void		Unk_96(void);
	virtual void		Unk_97(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		Unk_A0(void);
	virtual void		Unk_A1(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		Unk_AA(void);
	virtual void		Unk_AB(void);
	virtual void		Unk_AC(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual float		Unk_AF(UInt32 arg1);
	virtual void		Unk_B0(void);
	virtual void		Unk_B1(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		Unk_B4(void);
	virtual void		Unk_B5(void);
	virtual void		Unk_B6(void);
	virtual void		Unk_B7(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		Unk_BA(void);
	virtual void		Unk_BB(void);
	virtual void		Unk_BC(void);
	virtual void		Unk_BD(void);
	virtual void		Unk_BE(void);
	virtual void		Unk_BF(void);
	virtual void		Unk_C0(void);
	
	BaseProcess		*baseProcess;	// 68
	UInt32			unk6C;			// 6C - loaded
	TESObjectREFR	*unk70;			// 70 - loaded
	UInt32			unk74;			// 74 - loaded
	UInt32			unk78;			// 78 - loaded
	UInt8			unk7C;			// 7C - loaded
	UInt8			unk7D;			// 7D - loaded
	UInt8			unk7E;			// 7E - loaded
	UInt8			unk7F;			// 7F - loaded
	UInt8			unk80;			// 80 - loaded
	UInt8			unk81;			// 81 - loaded
	UInt8			unk82;			// 82
	UInt8			unk83;			// 83 - loaded
	UInt8			unk84;			// 84 - loaded
	UInt8			unk85;			// 85 - loaded
	UInt8			unk86;			// 86 - loaded
	UInt8			unk87;			// 87	Init'd to the inverse of NoLowLevelProcessing
};
STATIC_ASSERT(sizeof(MobileObject) == 0x88);

typedef tList<ActiveEffect> ActiveEffectList;

// 0C
class MagicCaster
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	CastSpell(MagicItem *spell, bool arg2, MagicTarget *target, float arg4, bool arg5);
	virtual void	Unk_04(void);
	virtual void	AddEffect(MagicItem *magItem, TESForm *itemForm, bool arg3);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual Actor	*GetActor(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(MagicItem *spell);
	virtual void	Unk_11(void);
	virtual void	Unk_12(MagicTarget *magicTarget);
	virtual void	Unk_13(void);

	UInt32	unk04[2];	// 04
};
STATIC_ASSERT(sizeof(MagicCaster) == 0xC);

// 10
class MagicTarget
{
public:
	virtual bool	ApplyEffect(MagicCaster *magicCaster, MagicItem *magicItem, ActiveEffect *activeEffect, bool arg4);
	virtual Actor	*GetActor(void);
	virtual ActiveEffectList	*GetEffectList(void);
	virtual void	Unk_03(void);
	virtual bool	CannotBeHit(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual float	Unk_09(MagicCaster *magicCaster, MagicItem *magicItem, ActiveEffect *activeEffect);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);

	UInt32	unk04[3];	// 04

	void RemoveEffect(EffectItem *effItem);
};
STATIC_ASSERT(sizeof(MagicTarget) == 0x10);

class PathingRequest;
class PathingSolution;
class DetailedActorPathHandler;
class ActorPathingMessageQueue;

// 28
class PathingLocation
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);

	UInt32			unk04[9];	// 04
};

// 88
class ActorMover
{
public:
	virtual void		Unk_00(void);
	virtual void		Unk_01(void);
	virtual void		ClearMovementFlag(void);
	virtual void		SetMovementFlag(void);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		Unk_06(void);
	virtual void		Unk_07(void);
	virtual UInt32		GetMovementFlags();
		//	Running		0x200
		//	Sneaking	0x400

		// bit 11 = swimming 
		// bit 9 = sneaking
		// bit 8 = run
		// bit 7 = walk
		// bit 0 = keep moving (Q)
	virtual void		Unk_09(void);
	virtual void		Unk_0A(void);
	virtual void		Unk_0B(void);
	virtual void		Unk_0C(void);
	virtual void		Unk_0D(void);
	virtual void		Unk_0E(void);

	UInt32						unk04[6];			// 04
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
	UInt8						unk70[4];			// 70
	UInt32						unk74;				// 74
	UInt32						unk78;				// 78
	UInt32						unk7C;				// 7C
	UInt32						unk80;				// 80
	UInt32						unk84;				// 84
};

// A0
class PlayerMover : public ActorMover
{
public:
	float			flt88;				// 88
	float			flt8C;				// 8C
	float			flt90;				// 90
	UInt32			pcMovementFlags;	// 94
	UInt32			unk98;				// 98
	UInt32			unk9C;				// 9C
};

typedef ActiveEffect *(*ActiveEffectCreate)(MagicCaster *magCaster, MagicItem *magItem, EffectItem *effItem);

class Actor : public MobileObject
{
public:
	virtual void		Unk_C1(void);
	virtual void		Unk_C2(void);
	virtual void		Unk_C3(void);
	virtual void		Unk_C4(void);
	virtual void		Unk_C5(void);
	virtual void		Unk_C6(void);
	virtual void		SetIgnoreCrime(bool ignoreCrime);
	virtual bool		GetIgnoreCrime();
	virtual void		Unk_C9(void);
	virtual void		Unk_CA(void);
	virtual void		Unk_CB(void);
	virtual void		Unk_CC(void);
	virtual void		Unk_CD(void);
	virtual void		Unk_CE(void);
	virtual void		DamageActionPoints(float amount); // checks GetIsGodMode before decreasing
	virtual void		Unk_D0(void);
	virtual void		Unk_D1(void);
	virtual void		Unk_D2(void);
	virtual void		Unk_D3(void);
	virtual void		Unk_D4(void);
	virtual void		Unk_D5(void);
	virtual bool		IsOverencumbered();
	virtual void		Unk_D7(void);
	virtual bool		IsPlayerRef();
	virtual void		Unk_D9(void);
	virtual void		Unk_DA(void);
	virtual void		Unk_DB(void);
	virtual void		Unk_DC(void);
	virtual void		Unk_DD(void);
	virtual void		Unk_DE(void);
	virtual void		Unk_DF(void);
	virtual void		Unk_E0(void);
	virtual void		Unk_E1(void);
	virtual void		Unk_E2(void);
	virtual bool		IsPushable();
	virtual UInt32		GetActorType(void);	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		SetActorValue(UInt32 avCode, float value);
	virtual void		SetActorValueInt(UInt32 avCode, UInt32 value);
	virtual void		Unk_E7(void);
	virtual void		Unk_E8(void);
	virtual void		Unk_E9(void);
	virtual void		ModActorValue(UInt32 avCode, int modifier, UInt32 arg3);
	virtual void		DamageActorValue(UInt32 avCode, float damage, Actor *attacker);
	virtual void		Unk_EC(void);
	virtual void		Unk_ED(void);
	virtual void		Unk_EE(void);
	virtual ExtraContainerChanges::EntryData *GetPreferedWeapon(UInt32 unk);
	virtual void		Unk_F0(void);
	virtual void		ResetArmorDRDT();
	virtual void		Unk_F2(void);
	virtual void		Unk_F3(void);
	virtual void		Unk_F4(void);
	virtual void		Unk_F5(void);
	virtual void		Unk_F6(void);
	virtual void		Unk_F7(void);
	virtual void		Unk_F8(void);
	virtual void		Unk_F9(void);
	virtual void		Unk_FA(void);
	virtual void		Unk_FB(TESForm *form, UInt32 arg2, UInt8 arg3, UInt8 arg4);
	virtual void		DecreaseAmmo(void);
	virtual void		Unk_FD(void);
	virtual void		Unk_FE(void);
	virtual void		Unk_FF(void);
	virtual void		Unk_100(void);
	virtual void		Unk_101(void);
	virtual void		Unk_102(void);
	virtual void		Unk_103(void);
	virtual void		Unk_104(void);
	virtual void		Unk_105(void);
	virtual void		Unk_106(void);
	virtual void		Unk_107(void);
	virtual void		Unk_108(void);
	virtual void		Unk_109(void);
	virtual CombatController	*GetCombatController(void);
	virtual Actor		*GetCombatTarget(void);
	virtual void		Unk_10C(void);
	virtual void		Unk_10D(void);
	virtual void		Unk_10E(void);
	virtual float		GetTotalArmorDR(void);
	virtual float		GetTotalArmorDT(void);
	virtual UInt32		Unk_111(void);
	virtual void		Unk_112(void);
	virtual void		Unk_113(void);
	virtual void		Unk_114(void);
	virtual void		Unk_115(void);
	virtual float		CalculateMoveSpeed();
	virtual float		CalculateRunSpeed();
	virtual void		Unk_118(void);
	virtual void		Unk_119(void);
	virtual void		Unk_11A(void);
	virtual void		Unk_11B(void);
	virtual void		Unk_11C(void);
	virtual void		Unk_11D(void);
	virtual void		Unk_11E(void);
	virtual void		Unk_11F(void);
	virtual void		Unk_120(void);
	virtual void		Unk_121(void);
	virtual void		RewardXP(UInt32 amount);
	virtual void		Unk_123(void);
	virtual void		Unk_124(void);
	virtual void		Unk_125(void);
	virtual void		SetPerkRank(BGSPerk *perk, UInt8 rank, bool alt);
	virtual void		RemovePerk(BGSPerk *perk, bool alt);
	virtual UInt8		GetPerkRank(BGSPerk *perk, bool alt);
	virtual void		Unk_129(void);
	virtual void		Unk_12A(void);
	virtual void		Unk_12B(void);
	virtual void		Unk_12C(void);
	virtual void		Unk_12D(void);
	virtual void		DoHealthDamage(Actor *attacker, float damage);
	virtual void		Unk_12F(void);
	virtual void		Unk_130(void);
	virtual float		Unk_131(void);
	virtual void		Unk_132(void);
	virtual void		Unk_133(void);
	virtual void		Unk_134(void);
	virtual void		Unk_135(void);
	virtual void		Unk_136(void);

	enum LifeStates
	{
		kLifeState_Alive =			0,
		kLifeState_Dying =			1,
		kLifeState_Dead =			2,
		kLifeState_Unconscious =	3,
		kLifeState_Reanimate =		4,
		kLifeState_Restrained =		5
	};
	
	MagicCaster			magicCaster;			// 088
	MagicTarget			magicTarget;			// 094
	ActorValueOwner		avOwner;				// 0A4
	CachedValuesOwner	cvOwner;				// 0A8

	bhkRagdollController				*ragDollController;			// 0AC
	bhkRagdollPenetrationUtil			*ragDollPentrationUtil;		// 0B0
	UInt32								unk0B4;						// 0B4-
	float								flt0B8;						// 0B8
	UInt8								byte0BC;					// 0BC-
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
	tList<void>							list0D0;					// 0D0
	UInt8								byte0D8;					// 0D8
	UInt8								byte0D9;					// 0D9
	UInt8								byte0DA;					// 0DA
	UInt8								byte0DB;					// 0DB
	UInt32								unk0DC;						// 0DC
	tList<void>							list0E0;					// 0E0
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
	tList<void>							list0FC;					// 0FC
	bool								isInCombat;					// 104
	UInt8								jipActorFlags1;				// 105
	UInt8								jipActorFlags2;				// 106
	UInt8								jipActorFlags3;				// 107
	UInt32								lifeState;					// 108	saved as byte HasHealth
	UInt32								criticalStage;				// 10C
	UInt32								unk110;						// 110-
	float								flt114;						// 114
	UInt8								byte118;					// 118-
	UInt8								byte119;					// 119+
	UInt8								byte11A;					// 11A
	UInt8								byte11B;					// 11B
	UInt32								unk11C;						// 11C-
	UInt32								resetTime;					// 120-
	bool								forceRun;					// 124
	bool								forceSneak;					// 125
	UInt8								byte126;					// 126-
	UInt8								byte127;					// 127-
	Actor								*combatTarget;				// 128
	BSSimpleArray<Actor*>				*combatTargets;				// 12C
	BSSimpleArray<Actor*>				*combatAllies;				// 130
	UInt8								byte134;					// 134-
	UInt8								byte135;					// 135+
	UInt8								byte136;					// 136
	UInt8								byte137;					// 137
	UInt32								unk138;						// 138-
	UInt32								unk13C;						// 13C-
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
	float								flt16C;						// 16C
	TESForm								*startingWorldOrCell;		// 170
	UInt8								byte174;					// 174-
	UInt8								byte175;					// 175-
	UInt8								byte176;					// 176
	UInt8								byte177;					// 177
	float								flt178;						// 178
	float								flt17C;						// 17C
	float								flt180;						// 180
	float								flt184;						// 184
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
	UInt32								unk1A4;						// 1A4-
	UInt32								unk1A8;						// 1A8-
	UInt32								unk1AC;						// 1AC-
	UInt8								byte1B0;					// 1B0-
	bool								forceHit;					// 1B1-
	UInt8								byte1B2;					// 1B2
	UInt8								byte1B3;					// 1B3

	// OBSE: unk1 looks like quantity, usu. 1; ignored for ammo (equips entire stack). In NVSE, pretty much always forced internally to 1 
	// OBSE: itemExtraList is NULL as the container changes entry is not resolved before the call
	// NVSE: Default values are those used by the vanilla script functions.
	__forceinline void EquipItem(TESForm *objType, UInt32 equipCount = 1, ExtraDataList *itemExtraList = NULL, bool arg4 = 1, bool lockEquip = 0, bool noMessage = 1)
	{
		ThisCall(0x88C650, this, objType, equipCount, itemExtraList, arg4, lockEquip, noMessage);
	}
	__forceinline void UnequipItem(TESForm *objType, UInt32 unequipCount = 1, ExtraDataList *itemExtraList = NULL, bool arg4 = 1, bool lockUnequip = 0, bool noMessage = 1)
	{
		ThisCall(0x88C790, this, objType, unequipCount, itemExtraList, arg4, lockUnequip, noMessage);
	}

	//EquippedItemsList GetEquippedItems();
	//ExtraContainerDataArray GetEquippedEntryDataList();
	//ExtraContainerExtendDataArray GetEquippedExtendDataList();

	bool GetDead() {return (lifeState == 1) || (lifeState == 2);}
	bool GetRestrained() {return lifeState == 5;}

	TESActorBase *GetActorBase();
	bool GetLOS(Actor *target);
	char GetCurrentAIPackage();
	char GetCurrentAIProcedure();
	bool IsFleeing();
	TESObjectWEAP *GetEquippedWeapon();
	bool IsItemEquipped(TESForm *item);
	UInt8 EquippedWeaponHasMod(UInt8 modID);
	bool IsSneaking();
	void StopCombat();
	bool __fastcall IsInCombatWith(Actor *target);
	int __fastcall GetDetectionValue(Actor *detected);
	TESPackage *GetStablePackage();
	PackageInfo *GetPackageInfo();
	TESObjectREFR *GetPackageTarget();
	TESCombatStyle *GetCombatStyle();
	bool GetKnockedState();
	bool IsWeaponOut();
	void UpdateActiveEffects(MagicItem *magicItem, EffectItem *effItem, ActiveEffectCreate callback, bool addNew);
	bool GetIsGhost();
	float GetRadiationLevel();
	BackUpPackage *AddBackUpPackage(TESObjectREFR *targetRef, TESObjectCELL *targetCell, UInt32 flags);
	void __fastcall TurnToFaceObject(TESObjectREFR *target);
	void TurnAngle(float angle);
	void PlayIdle(TESIdleForm *idleAnim);
	UInt32 GetLevel();
	float GetKillXP();
	void DismemberLimb(UInt32 bodyPartID, bool explode);
	void EquipItemAlt(TESForm *itemForm, ContChangesEntry *entry, UInt32 noUnequip, UInt32 noMessage);
	bool HasNoPath();
	bool CanBePushed();
	float AdjustPushForce(float baseForce);
	void PushActor(float force, float angle, TESObjectREFR *originRef, bool adjustForce);
	int GetGroundMaterial();
};

// 1C0
class Creature : public Actor
{
public:
	virtual void	Unk_137(void);

	UInt8			byte1B4;			// 1B4
	UInt8			pad1B5[3];			// 1B5
	UInt32			unk1B8[2];			// 1B8
};

// 1C8
class Character : public Actor
{
public:
	virtual void	Unk_137(void);
	virtual void	Unk_138(void);

	ValidBip01Names	*validBip01Names;	// 1B4
	float			totalArmorDR;		// 1B8
	float			totalArmorDT;		// 1BC
	UInt8			isTrespassing;		// 1C0
	UInt8			byt1C1;				// 1C1
	UInt16			unk1C2;				// 1C2
	float			unk1C4;				// 1C4
};

struct ParentSpaceNode;
struct TeleportLink;
struct ItemChange;
struct MusicMarker;

struct PerkRank
{
	BGSPerk		*perk;
	UInt8		rank;
	UInt8		pad05[3];
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

	virtual void		Unk_139(void);
	virtual void		Unk_13A(void);

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

	UInt32								unk1C8[17];				// 1C8	208 could be a DialogPackage
	void								*unk20C;				// 20C
	tList<ActiveEffect>					*activeEffects;			// 210
	UInt32								unk214[2];				// 214
	void								*unk21C;				// 21C
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
	void								*unk608;				// 608
	void								*unk60C;				// 60C
	void								*unk610;				// 610
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
	bool								byte64A;				// 64A	= not FirstPerson
	bool								is3rdPerson;			// 64B
	bool								bThirdPerson;			// 64C
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
	UInt32								unk65C[4];				// 65C
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
	ValidBip01Names						*VB01N1stPerson;		// 68C
	AnimData							*animData1stPerson;		// 690
	NiNode								*playerNode;			// 694 used as node if unk64A is true
	float								flt698;					// 698
	UInt32								unk69C[3];				// 69C
	tList<TESTopic>						topicList;				// 6A8
	UInt32								unk6B0[2];				// 6B0
	TESQuest							*activeQuest;			// 6B8
	tList<BGSQuestObjective>			questObjectiveList;		// 6BC
	tList<BGSQuestObjective::Target>	questTargetList;		// 6C4
	UInt32								unk6CC[5];				// 6CC
	float								sortActorDistanceListTimer;	// 6E0
	float								seatedRotation;			// 6E4
	UInt32								unk6E8[2];				// 6E8
	UInt32								playerSpell;			// 6F0
	TESObjectREFR						*placedMarker;			// 6F4
	BSSimpleArray<ParentSpaceNode>		parentSpaceNodes;		// 6F8
	BSSimpleArray<TeleportLink>			teleportLinks;			// 708
	UInt32								unk718[6];				// 718
	float 								timeGrenadeHeld;		// 730
	UInt32								unk734[10];				// 734
	bool								inCharGen;				// 75C
	UInt8								byte75D;				// 75D
	UInt8								byte75E;				// 75E
	UInt8								byte75F;				// 75F
	TESRegion							*currentRegion;			// 760
	TESRegionList						regionsList;			// 764
	UInt32								unk774[14];				// 774
	TESForm								*pcWorldOrCell;			// 7AC
	UInt32								unk7B0;					// 7B0
	BGSMusicType						*musicType;				// 7B4
	UInt8								gameDifficulty;			// 7B8
	UInt8								byte7B9;				// 7B9
	UInt8								byte7BA;				// 7BA
	UInt8								byte7BB;				// 7BB
	bool								isHardcore;				// 7BC
	UInt8								byte7BD;				// 7BD
	UInt8								byte7BE;				// 7BE
	UInt8								byte7BF;				// 7BF
	UInt32								killCamMode;			// 7C0
	UInt8								byte7C4;				// 7C4
	UInt8								byte7C5;				// 7C5
	bool								isToddler;				// 7C6
	bool								canUsePA;				// 7C7
	tList<MapMarkerInfo>				mapMarkers;				// 7C8
	TESWorldSpace						*worldSpc7D0;			// 7D0
	tList<MusicMarker>					musicMarkers;			// 7D4
	MusicMarker							*currMusicMarker;		// 7DC
	float								flycamZRot;				// 7E0
	float								flycamXRot;				// 7E4
	float								flycamPosX;				// 7E8
	float								flycamPosY;				// 7EC
	float								flycamPosZ;				// 7F0
	UInt32								unk7F4[33];				// 7F4
	PCLevelData							*pcLevelData;			// 878
	tList<PerkRank>						perkRanksPC;			// 87C
	tList<BGSEntryPointPerkEntry>		perkEntriesPC[74];		// 884
	tList<PerkRank>						perkRanksTM;			// AD4
	tList<BGSEntryPointPerkEntry>		perkEntriesTM[74];		// ADC
	UInt32								unkD2C[4];				// D2C
	NiObject							*unkD3C;				// D3C
	UInt32								unkD40;					// D40
	Actor								*reticleActor;			// D44
	tList<CompassTarget>				*compassTargets;		// D48
	UInt32								unkD4C;					// D4C
	float								lastAmmoSwapTime;		// D50
	UInt32								unkD54;					// D54
	NiVector3							vectorD58;				// D58
	CombatActors						*combatActors;			// D64
	UInt32								teammateCount;			// D68
	UInt32								unkD6C[5];				// D6C
	NiNode								*niNodeD80;				// D80
	UInt32								unkD84[12];				// D84
	NiNode								*niNodeDB4;				// DB4
	UInt32								unkDB8[7];				// DB8
	NiVector3							vectorDD4;				// DD4
	NiVector3							cameraPos;				// DE0
	bhkRigidBody						*rigidBody;				// DEC
	bool								pcInCombat;				// DF0
	bool								pcUnseen;				// DF1
	UInt8								byteDF2;				// DF2
	UInt8								byteDF3;				// DF3
	BSSimpleArray<ItemChange>			itemChanges;			// DF4
	UInt32								unkE04;					// E04
	UInt8								byteE08;				// E08
	UInt8								padE09[3];				// E09
	UInt32								unkE0C[3];				// E0C
	float								killCamTimer;			// E18
	float								killCamCooldown;		// E1C
	UInt8								byteE20;				// E20
	UInt8								byteE21;				// E21
	UInt8								byteE22;				// E22
	UInt8								byteE23;				// E23
	float								fltE24;					// E24
	float								counterAttackTimer;		// E28
	UInt32								unkE2C;					// E2C
	UInt32								itemDetectionTimer;		// E30
	NiNode								*ironSightNode;			// E34
	bool								noHardcoreTracking;		// E38	Appears to be unused
	bool								skipHCNeedsUpdate;		// E39
	UInt8								byteE3A;				// E3A
	UInt8								byteE3B;				// E3B
	BSSimpleArray<TESAmmo*>				hotkeyedWeaponAmmos;	// E3C
	UInt32								unkE4C;					// E4C
		// 7C6 is a boolean meaning toddler, 
		// 7C7 byte bool PCCanUsePowerArmor, Byt0E39 referenced during LoadGame
		// Used by TFC : 7E8/EC/F0 stores Pos, 7F0 adjusted by scaledHeight , 7E0 stores RotZ, 7E4 RotX
		// Quest Stage LogEntry at 6B0.
		// tList at 6C4 is cleared when there is no current quest. There is another NiNode at 069C
		// 086C is cleared after equipement change.

	bool IsThirdPerson() { return bThirdPerson ? true : false; }
	UInt32 GetMovementFlags() { return actorMover->GetMovementFlags(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool IsPlayerSwimming() { return (actorMover->GetMovementFlags() & 0x800) ? true : false; }

	static PlayerCharacter*	GetSingleton();
	bool SetSkeletonPath(const char* newPath);
	static void UpdateHead(void);

	__forceinline bool ToggleFirstPerson(bool toggleON)
	{
		return ThisCall<bool>(0x950110, this, toggleON);
	}
	char GetDetectionState();
	void UpdatePlayer3D();
};
STATIC_ASSERT(sizeof(PlayerCharacter) == 0xE50);