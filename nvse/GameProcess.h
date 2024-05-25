#pragma once

#include "nvse/GameTasks.h"

struct PackageInfo
{
	TESPackage		*package;		// 00
	union
	{
		TESPackageData		*packageData;	// 04
		ActorPackageData	*actorPkgData;
	};
	TESObjectREFR	*targetRef;		// 08
	UInt32			unk0C;			// 0C	Initialized to 0FFFFFFFFh, set to 0 on start
	float			unk10;			// 10	Initialized to -1.0	. Set to GameHour on start so some time
	UInt32			flags;			// 14	Flags, bit0 would be not created and initialized
};

// 64
struct ActorHitData
{
	enum HitFlags
	{
		kFlag_TargetIsBlocking =		1,
		kFlag_TargetWeaponOut =			2,
		kFlag_IsCritical =				4,
		kFlag_OnDeathCritEffect =		8,
		kFlag_IsFatal =					0x10,
		kFlag_DismemberLimb =			0x20,
		kFlag_ExplodeLimb =				0x40,
		kFlag_CrippleLimb =				0x80,
		kFlag_BreakWeaponNonEmbedded =	0x100,
		kFlag_BreakWeaponEmbedded =		0x200,
		kFlag_BreakWeapon =				kFlag_BreakWeaponNonEmbedded | kFlag_BreakWeaponEmbedded,
		kFlag_IsSneakAttack =			0x400,
		kFlag_Unk800 =					0x800,
		kFlag_Unk1000 =					0x1000,
		kFlag_IsExplosionHit =			0x2000,
		kFlag_ArmorPenetrated =			0x80000000	// JIP only
	};

	Actor				*source;		// 00
	Actor				*target;		// 04
	union								// 08
	{
		Projectile		*projectile;
		Explosion		*explosion;
		//	Can in some cases be the source actor (seen w/ weapon == Fists) 
	};
	UInt32				weaponAV;		// 0C
	SInt32				hitLocation;	// 10
	float				healthDmg;		// 14
	float				wpnBaseDmg;		// 18	Skill and weapon condition modifiers included
	float				fatigueDmg;		// 1C
	float				limbDmg;		// 20
	float				blockDTMod;		// 24
	float				armorDmg;		// 28
	float				weaponDmg;		// 2C
	TESObjectWEAP		*weapon;		// 30
	float				weapHealthPerc;	// 34
	NiVector3			impactPos;		// 38
	NiVector3			impactAngle;	// 44
	SpellItem			*critHitEffect;	// 50
	void				*ptr54;			// 54
	UInt32				flags;			// 58
	float				dmgMult;		// 5C
	SInt32				unk60;			// 60	Unused; rigged by CopyHitDataHook to store hitLocation
};

struct DetectionData
{
	Actor		*actor;				// 00
	UInt8		detectionLevel;		// 04
	UInt8		byte05;				// 05
	UInt8		byte06;				// 06
	UInt8		byte07;				// 07
	SInt32		detectionValue;		// 08
	NiPoint3	detectedLocation;	// 0C
	float		fTimeStamp;			// 18
	UInt8		forceResetCombatLOSBuffer;	// 1C
	UInt8		byte1D;				// 1D
	bool		inLOS;				// 1E
	UInt8		byte1F;				// 1F
	UInt32		unk20;				// 20
};

// 68
struct CombatTarget
{
	Actor		*target;
	UInt32		unk04[25];
};

// 1C
struct DetectionEvent
{
	UInt32			soundLevel;		// 00
	float			posX;			// 04
	float			posY;			// 08
	float			posZ;			// 0C
	float			timeStamp;		// 10
	UInt32			unk14;			// 14
	TESObjectREFR	*locationRef;	// 18
};

// 20
struct MuzzleFlash
{
	UInt8				bEnabled;		// 00
	UInt8				bMPSEnabled;	// 01
	UInt8				bUpdateLight;	// 02
	UInt8				byte03;			// 03
	float				enableTimer;	// 04
	float				flashDuration;	// 08
	NiNode				*muzzleFlash;	// 0C
	NiPointLight		*flashLight;	// 10
	BGSProjectile		*baseProj;		// 14
	TESObjectWEAP		*sourceWeap;	// 18
	TESObjectREFR		*sourceRefr;	// 1C
};

class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;

struct StoredActorValues
{
	struct StoredValue
	{
		UInt8		byte00;
		UInt8		pad01[3];
		float		value;
	};

	StoredValue		values[kAVCode_Max];
};

// 30
class BaseProcess
{
public:
	struct CachedValues
	{
		enum
		{
			kCached_Radius =				0x1,
			kCached_WidthX =				0x2,
			kCached_WidthY =				0x4,
			kCached_DPS =					0x8,
			kCached_MedicineSkillMult =		0x10,
			kCached_Paralysis =				0x20,
			kCached_HealRate =				0x40,
			kCached_FatigueReturnRate =		0x80,
			kCached_PerceptionCondition =	0x100,
			kCached_EyeHeight =				0x200,
			kCached_SomethingShouldAttack =	0x400,
			kCached_WalkSpeed =				0x1000,
			kCached_RunSpeed =				0x2000,
			kCached_NoCrippledLegs =		0x4000,
			kCached_Height =				0x8000,
			kCached_IsGhost =				0x10000000,
			kCached_Health =				0x20000000,
			kCached_Fatigue =				0x40000000,
			kCached_SurvivalSkillMult =		0x80000000
		};

		float	radius;					// 00
		float	widthX;					// 04
		float	widthY;					// 08
		float	height;					// 0C
		float	DPS;					// 10
		float	medicineSkillMult;		// 14
		float	survivalSkillMult;		// 18
		float	paralysis;				// 1C
		float	healRate;				// 20
		float	fatigueReturnRate;		// 24
		float	perceptionCondition;	// 28
		float	eyeHeight;				// 2C
		SInt32	aggression;				// 30
		SInt32	assistance;				// 34
		float	walkSpeed;				// 38
		float	runSpeed;				// 3C
		UInt8	hasNoCrippledLegs;		// 40
		UInt8	pad41[3];				// 41
		UInt32	flags;					// 44
	};

	/*000*/virtual void		Destroy(bool deFree);
	/*004*/virtual void		Copy(BaseProcess *from);
	/*008*/virtual void		Unk_02(void);
	/*00C*/virtual void		RunProcess(MobileObject *refr, float arg2);
	/*010*/virtual void		RunProcess2(MobileObject *refr);
	/*014*/virtual void		Unk_05(void);
	/*018*/virtual void		Unk_06(void);
	/*01C*/virtual void		Unk_07(void);
	/*020*/virtual void		Unk_08(void);
	/*024*/virtual bool		CheckForNewPackage(Actor *actor, UInt8 flag);
	/*028*/virtual void		SetLastTimeProcessed();
	/*02C*/virtual void		Unk_0B(void);
	/*030*/virtual void		Unk_0C(void);
	/*034*/virtual void		Unk_0D(void);
	/*038*/virtual void		Unk_0E(void);
	/*03C*/virtual void		Unk_0F(void);
	/*040*/virtual void		Unk_10(void);
	/*044*/virtual void		Unk_11(void);
	/*048*/virtual void		Unk_12(void);
	/*04C*/virtual void		Unk_13(void);
	/*050*/virtual void		Unk_14(void);
	/*054*/virtual void		Unk_15(void);
	/*058*/virtual void		Unk_16(void);
	/*05C*/virtual void		Unk_17(void);
	/*060*/virtual UInt8	GetIdleDoneOnce();
	/*064*/virtual void		Unk_19(void);
	/*068*/virtual void		Unk_1A(void);
	/*06C*/virtual void		Unk_1B(void);
	/*070*/virtual void		Unk_1C(void);
	/*074*/virtual void		Unk_1D(void);
	/*078*/virtual void		Unk_1E(void);
	/*07C*/virtual void		ProcessSandman(Actor *actor);
	/*080*/virtual void		ProcessCannibal(Actor *actor);
	/*084*/virtual void		Unk_21(void);
	/*088*/virtual void		Unk_22(void);
	/*08C*/virtual void		Unk_23(void);
	/*090*/virtual void		Unk_24(void);
	/*094*/virtual void		Unk_25(void);
	/*098*/virtual void		Unk_26(void);
	/*09C*/virtual void		Unk_27(void);
	/*0A0*/virtual void		Unk_28(void);
	/*0A4*/virtual void		Unk_29(void);
	/*0A8*/virtual void		Unk_2A(void);
	/*0AC*/virtual void		Unk_2B(void);
	/*0B0*/virtual void		Unk_2C(void);
	/*0B4*/virtual void		Unk_2D(void);
	/*0B8*/virtual void		Unk_2E(void);
	/*0BC*/virtual void		Unk_2F(void);
	/*0C0*/virtual void		Unk_30(void);
	/*0C4*/virtual void		Unk_31(void);
	/*0C8*/virtual void		Unk_32(void);
	/*0CC*/virtual void		Unk_33(void);
	/*0D0*/virtual void		Unk_34(void);
	/*0D4*/virtual void		Unk_35(void);
	/*0D8*/virtual void		StopIdle(Actor *actor);
	/*0DC*/virtual void		Unk_37(void);
	/*0E0*/virtual void		UpdateEssentialDownTimer();
	/*0E4*/virtual float	GetEssentialDownTimer();
	/*0E8*/virtual void		SetEssentialDownTimer(float _downTimer);
	/*0EC*/virtual void		Unk_3B(void);
	/*0F0*/virtual void		Unk_3C(void);
	/*0F4*/virtual void		Unk_3D(void);
	/*0F8*/virtual DetectionEvent	*GetDetectionEvent(Actor *actor);	// arg not used
	/*0FC*/virtual void		CreateDetectionEvent(Actor *actor, float posX, float posY, float posZ, UInt32 soundLevel, UInt32 eventType, TESObjectREFR *locationRef);	// actor & eventType unused
	/*100*/virtual void		RemoveDetectionEvent();
	/*104*/virtual void		Unk_41(void);
	/*108*/virtual void		Unk_42(void);
	/*10C*/virtual void		Unk_43(void);
	/*110*/virtual bool		GetHasCaughtPlayerPickpocketting();
	/*114*/virtual void		SetHasCaughtPlayerPickpocketting(bool doSet);
	/*118*/virtual void		Unk_46(void);
	/*11C*/virtual void		Unk_47(void);
	/*120*/virtual void		Unk_48(void);
	/*124*/virtual void		Unk_49(void);
	/*128*/virtual TESForm *GetLowProcess40();
	/*12C*/virtual void		Unk_4B(void);
	/*130*/virtual void		Unk_4C(void);
	/*134*/virtual void		Unk_4D(void);
	/*138*/virtual void		Unk_4E(void);
	/*13C*/virtual void		Unk_4F(void);
	/*140*/virtual void		Unk_50(void);
	/*144*/virtual BSFaceGenAnimationData	*GetFaceGenAnimData(Actor *actor);
	/*148*/virtual ContChangesEntry *GetWeaponInfo();
	/*14C*/virtual ContChangesEntry *GetAmmoInfo();
	/*150*/virtual void		Unk_54(void);
	/*154*/virtual void		Unk_55(void);
	/*158*/virtual void		Unk_56(void);
	/*15C*/virtual void		Unk_57(void);
	/*160*/virtual void		Unk_58(ContChangesEntry *_weapInfo, NiNode *rootNode, bool arg3);
	/*164*/virtual void		Unk_59(void);
	/*168*/virtual void		SetAmmoInfo(ContChangesEntry *_ammoInfo);
	/*16C*/virtual void		Unk_5B(void);
	/*170*/virtual void		HandleQueuedEquipItems(Actor *actor);
	/*174*/virtual bool		IsItemQueued(TESForm *item, ExtraDataList *xDataList, bool doEquip);
	/*178*/virtual void		QueueEquipItem(Actor *actor, bool doEquip, TESForm *item, UInt32 count, ExtraDataList *xDataList, bool applyEnchantment, bool noUnequip, bool removeEnchantment, UInt8 arg9, bool playSound);
	/*17C*/virtual void		RemoveItemFromQueue(TESForm *item, ExtraDataList *xDataList);
	/*180*/virtual void		RemoveAllItemsFromQueue();
	/*184*/virtual NiNode	*GetProjectileNode();
	/*188*/virtual void		SetProjectileNode(NiNode *node);
	/*18C*/virtual void		SetWeaponNode(NiControllerManager *ctrlMgr, NiNode *node);
	/*190*/virtual NiNode	*GetWeaponNode(BipedAnim *vbp01Names);
	/*194*/virtual void		Unk_65(void);
	/*198*/virtual void		Unk_66(void);
	/*19C*/virtual void		Unk_67(void);
	/*1A0*/virtual NiNode	*GetWeaponNode2(UInt32 arg);
	/*1A4*/virtual void		Unk_69(void);
	/*1A8*/virtual bool		IsUsingOneHandGrenade();
	/*1AC*/virtual bool		IsUsingOneHandMine();
	/*1B0*/virtual bool		IsUsingOneHandThrownMineOrGrenade();
	/*1B4*/virtual bool		IsUsingOneHandThrown();
	/*1B8*/virtual AnimData	*GetAnimData();
	/*1BC*/virtual void		Unk_6F(void);
	/*1C0*/virtual void		Unk_70(void);
	/*1C4*/virtual void		Unk_71(void);
	/*1C8*/virtual void		Unk_72(void);
	/*1CC*/virtual void		Unk_73(bool weaponOut, BipedAnim *bipedAnims, AnimData *pAnimData, Actor *actor);
	/*1D0*/virtual void		Unk_74(void);
	/*1D4*/virtual void		Unk_75(void);
	/*1D8*/virtual void		Unk_76(Actor *actor);
	/*1DC*/virtual void		Unk_77(void);
	/*1E0*/virtual void		Unk_78(void);
	/*1E4*/virtual void		Unk_79(void);
	/*1E8*/virtual void		Unk_7A(void);
	/*1EC*/virtual void		Unk_7B(void);
	/*1F0*/virtual void		Unk_7C(void);
	/*1F4*/virtual void		Unk_7D(void);
	/*1F8*/virtual void		Unk_7E(void);
	/*1FC*/virtual void		Unk_7F(void);
	/*200*/virtual void		Unk_80(void);
	/*204*/virtual void		Unk_81(void);
	/*208*/virtual void		Unk_82(void);
	/*20C*/virtual TESPackage	*GetInterruptPackage();
	/*210*/virtual void		SetInterruptPackage(TESPackage *package, Actor *onActor);
	/*214*/virtual void		StopInterruptPackage();
	/*218*/virtual void		Unk_86(void);	// 086 - SetInterruptPackageTargetRef
	/*21C*/virtual void		Unk_87(void);	// 087 - SetInterruptPackageTargetRef
	/*220*/virtual void		Unk_88(void);	// 088 - IncreaseInterruptPackageUnk00C
	/*224*/virtual void		Unk_89(void);
	/*228*/virtual void		Unk_8A(void);
	/*22C*/virtual TESPackage	*GetStablePackage();
	/*230*/virtual void		SetStablePackage(TESPackage *package, Actor *onActor);
	/*234*/virtual void		StopStablePackage();
	/*238*/virtual void		Unk_8E(void);
	/*23C*/virtual void		Unk_8F(void);
	/*240*/virtual void		Unk_90(void);
	/*244*/virtual void		Unk_91(void);
	/*248*/virtual void		Unk_92(void);	// Only HighProcess, get Unk0454
	/*24C*/virtual void		Unk_93(void);
	/*250*/virtual void		Unk_94(void);
	/*254*/virtual void		Unk_95(void);
	/*258*/virtual void		Unk_96(void);
	/*25C*/virtual void		Unk_97(void);
	/*260*/virtual void		Unk_98(void);
	/*264*/virtual void		Unk_99(void);
	/*268*/virtual void		Unk_9A(void);
	/*26C*/virtual void		Unk_9B(void);
	/*270*/virtual void		Unk_9C(void);
	/*274*/virtual TESPackageData	*GetPackageData();
	/*278*/virtual void		Unk_9E(void);
	/*27C*/virtual TESPackage	*GetCurrentPackage();
	/*280*/virtual UInt32	GetPackageInfo0C();
	/*284*/virtual void		Unk_A1();
	/*288*/virtual void		Unk_A2();
	/*28C*/virtual bhkCharacterController	*GetCharacterController();
	/*290*/virtual void		SetCharacterController(bhkCharacterController *charCtrl);
	/*294*/virtual void		StopMoving(Actor *actor);
	/*298*/virtual void		Unk_A6();
	/*29C*/virtual void		Unk_A7();
	/*2A0*/virtual void		ProcessFlee(Actor *packageUser);
	/*2A4*/virtual void		Unk_A9();
	/*2A8*/virtual void		Unk_AA();
	/*2AC*/virtual void		Unk_AB();
	/*2B0*/virtual void		Unk_AC();
	/*2B4*/virtual void		Unk_AD();
	/*2B8*/virtual void		Unk_AE();
	/*2BC*/virtual void		Unk_AF();
	/*2C0*/virtual void		Unk_B0();
	/*2C4*/virtual void		Unk_B1();
	/*2C8*/virtual void		SetDetectionTimer(float timer);
	/*2CC*/virtual void		Unk_B3();
	/*2D0*/virtual void		Unk_B4();
	/*2D4*/virtual void		Unk_B5();
	/*2D8*/virtual void		Unk_B6();
	/*2DC*/virtual void		Unk_B7();
	/*2E0*/virtual void		Unk_B8();
	/*2E4*/virtual void		Unk_B9();
	/*2E8*/virtual void		Unk_BA();
	/*2EC*/virtual void		Unk_BB();
	/*2F0*/virtual void		Unk_BC();
	/*2F4*/virtual void		Unk_BD();
	/*2F8*/virtual void		Unk_BE();
	/*2FC*/virtual void		SetDiveBreath(float breath);
	/*300*/virtual float	GetDiveBreath();
	/*304*/virtual void		Unk_C1();
	/*308*/virtual void		Unk_C2();
	/*30C*/virtual void		Unk_C3();
	/*310*/virtual void		Unk_C4();
	/*314*/virtual void		Unk_C5();
	/*318*/virtual void		Unk_C6();
	/*31C*/virtual bool		GetAlerted();
	/*320*/virtual void		SetAlert(bool alert);
	/*324*/virtual void		Unk_C9();
	/*328*/virtual void		Unk_CA();
	/*32C*/virtual void		Unk_CB();
	/*330*/virtual void		Unk_CC();
	/*334*/virtual void		Unk_CD();
	/*338*/virtual void		Unk_CE();
	/*33C*/virtual void		Unk_CF();
	/*340*/virtual void		Unk_D0();
	/*344*/virtual void		Unk_D1();
	/*348*/virtual void		Unk_D2();
	/*34C*/virtual void		Unk_D3();
	/*350*/virtual void		Unk_D4();
	/*354*/virtual void		FinishDying(Actor *actor);
	/*358*/virtual void		Unk_D6();
	/*35C*/virtual void		Unk_D7();
	/*360*/virtual void		Unk_D8();
	/*364*/virtual void		Unk_D9();
	/*368*/virtual void		Unk_DA();
	/*36C*/virtual void		Unk_DB();
	/*370*/virtual void		Unk_DC();
	/*374*/virtual void		Unk_DD();
	/*378*/virtual void		Unk_DE();
	/*37C*/virtual void		Unk_DF();
	/*380*/virtual void		Unk_E0();
	/*384*/virtual void		Unk_E1();
	/*388*/virtual UInt8	GetPlantedExplosive();
	/*38C*/virtual TESIdleForm	*GetLastPlayedIdle();
	/*390*/virtual void		SetLastPlayedIdle(TESIdleForm *idleForm);
	/*394*/virtual void		StopIdleFlags(UInt32 arg1);
	/*398*/virtual void		Unk_E6();
	/*39C*/virtual float	GetActorValue(TESActorBase *actorBase, UInt32 avID, Actor *actor);
	/*3A0*/virtual void		Unk_E8();
	/*3A4*/virtual void		Unk_E9();
	/*3A8*/virtual void		Unk_EA();
	/*3AC*/virtual void		Unk_EB();
	/*3B0*/virtual void		Unk_EC();
	/*3B4*/virtual void		Unk_ED();
	/*3B8*/virtual void		Unk_EE();
	/*3BC*/virtual void		Unk_EF();
	/*3C0*/virtual void		Unk_F0();
	/*3C4*/virtual void		Unk_F1();
	/*3C8*/virtual MagicItem	*Unk_F2();
	/*3CC*/virtual void		Unk_F3();
	/*3D0*/virtual MagicTarget	*Unk_F4();
	/*3D4*/virtual void		Unk_F5();
	/*3D8*/virtual void		Unk_F6();
	/*3DC*/virtual void		Unk_F7();
	/*3E0*/virtual void		Unk_F8();
	/*3E4*/virtual SInt16	GetCurrentAction();
	/*3E8*/virtual BSAnimGroupSequence	*GetCurrentSequence();
	/*3EC*/virtual void		SetCurrentActionAndSequence(SInt16 action, BSAnimGroupSequence *sequence);
	/*3F0*/virtual bool		GetForceFireWeapon();
	/*3F4*/virtual void		SetForceFireWeapon(bool doSet);
	/*3F8*/virtual bool		IsReadyForAnim();
	/*3FC*/virtual void		Unk_FF();
	/*400*/virtual void		SetIsAiming(bool aiming);
	/*404*/virtual bool		GetIsAiming();
	/*408*/virtual void		Unk_102();
	/*40C*/virtual SInt32	GetKnockedState();
	/*410*/virtual void		SetKnockedState(char state);
	/*414*/virtual void		Unk_105();
	/*418*/virtual void		PushActorAway(Actor *pushed, float posX, float posY, float posZ, float force);
	/*41C*/virtual void		Unk_107(Actor *actor);
	/*420*/virtual void		Unk_108();
	/*424*/virtual void		SetNthAnimSequenceWeight(UInt32 index, BSAnimGroupSequence *animSeq);
	/*428*/virtual void		Unk_10A();
	/*42C*/virtual float	GetFlt1D0();
	/*430*/virtual void		Unk_10C();
	/*434*/virtual void		ResetAttackLoopTimer(bool setToFull);
	/*438*/virtual void		DecreaseAttackLoopShootTime(Actor *actor);
	/*43C*/virtual void		Unk_10F();
	/*440*/virtual void		Unk_110();
	/*444*/virtual void		Unk_111();
	/*448*/virtual void		Unk_112();
	/*44C*/virtual bool		GetWantsWeaponOut();
	/*450*/virtual void		Unk_114();
	/*454*/virtual bool		IsWeaponOut();
	/*458*/virtual void		SetWeaponOut(Actor *actor, bool weaponOut);
	/*45C*/virtual void		Unk_117();
	/*460*/virtual void		Unk_118();
	/*464*/virtual void		Unk_119(Actor *actor);
	/*468*/virtual void		Flags18CFlagSet(UInt32 flagMask);
	/*46C*/virtual void		Flags18CFlagUnset(UInt32 flagMask);
	/*470*/virtual void		ResetFlags18C();
	/*474*/virtual bool		IsFlags18CFlagSet(UInt32 flagMask);
	/*478*/virtual UInt8	GetFlags18C();
	/*47C*/virtual void		Unk_11F();
	/*480*/virtual void		Unk_120();
	/*484*/virtual UInt32	GetUnk30C();
	/*488*/virtual void		SetUnk30C(UInt32 setTo);
	/*48C*/virtual void		Unk_123();
	/*490*/virtual void		Unk_124();
	/*494*/virtual void		Unk_125();
	/*498*/virtual void		Unk_126();
	/*49C*/virtual UInt8	GetByte3A0();
	/*4A0*/virtual void		ResetByte3A0();
	/*4A4*/virtual void		Unk_129();
	/*4A8*/virtual void		ResetDetectionActionTimer();
	/*4AC*/virtual float	GetFlt3BC();
	/*4B0*/virtual float	GetFlt330();
	/*4B4*/virtual void		SetFlt330(float setTo);
	/*4B8*/virtual bool		IsOnDialoguePackage(Actor *actor);
	/*4BC*/virtual UInt32	GetSitSleepState();
	/*4C0*/virtual void		Unk_130();
	/*4C4*/virtual void		Unk_131();
	/*4C8*/virtual TESObjectREFR	*GetCurrentFurnitureRef();
	/*4CC*/virtual void		Unk_133();
	/*4D0*/virtual UInt8	GetByte144();
	/*4D4*/virtual void		Unk_135();
	/*4D8*/virtual void		Unk_136();
	/*4DC*/virtual void		Unk_137();
	/*4E0*/virtual UInt8	GetByte374();
	/*4E4*/virtual void		SetByte374(UInt8 setTo);
	/*4E8*/virtual void		Unk_13A();
	/*4EC*/virtual void		Unk_13B();
	/*4F0*/virtual UInt8	GetByte1DA();
	/*4F4*/virtual void		SetByte1DA(UInt8 setTo);
	/*4F8*/virtual BSShaderProperty	*GetShaderProp220();
	/*4FC*/virtual void		SetShaderProp220(BSShaderProperty *inProp);
	/*500*/virtual void		Unk_140();
	/*504*/virtual DetectionData *GetDetectionData(Actor *target, UInt32 detecting);
	/*508*/virtual void		Unk_142();
	/*50C*/virtual float	GetFlt3C8();
	/*510*/virtual void		SetFlt3C8(float setTo);
	/*514*/virtual UInt32	GetUnk044();
	/*518*/virtual void		SetUnk044(UInt32 setTo);
	/*51C*/virtual UInt32	GetUnk048();
	/*520*/virtual void		SetUnk048(UInt32 setTo);
	/*524*/virtual void		Unk_149();
	/*528*/virtual void		SetUnk158(UInt32 setTo);
	/*52C*/virtual UInt32	GetUnk158();
	/*530*/virtual void		SetUnk15C(UInt32 setTo);
	/*534*/virtual UInt32	GetUnk15C();
	/*538*/virtual void		SetFlt378(float setTo);
	/*53C*/virtual float	GetFlt378();
	/*540*/virtual void		Unk_150();
	/*544*/virtual void		Unk_151();
	/*548*/virtual void		Unk_152();
	/*54C*/virtual void		Unk_153();
	/*550*/virtual void		Unk_154();
	/*554*/virtual void		Unk_155();
	/*558*/virtual void		Unk_156();
	/*55C*/virtual void		Unk_157();
	/*560*/virtual void		Unk_158();
	/*564*/virtual void		Unk_159();
	/*568*/virtual void		Unk_15A();
	/*56C*/virtual void		Unk_15B();
	/*570*/virtual void		Unk_15C();
	/*574*/virtual UInt32	GetUnk16C();
	/*578*/virtual void		SetUnk16C(UInt32 setTo);
	/*57C*/virtual UInt8	GetByte17C();
	/*580*/virtual void		Unk_160(UInt8 setE, UInt8 setD, UInt8 setF);
	/*584*/virtual void		Unk_161(Actor *actor);
	/*588*/virtual void		SetByte19C(UInt8 setTo);
	/*58C*/virtual UInt8	GetByte19C();
	/*590*/virtual void		SetByte19D(UInt8 setTo);
	/*594*/virtual UInt8	GetByte19D();
	/*598*/virtual void		HandleOnPackageBegin(Actor *actor, TESPackage *package);
	/*59C*/virtual void		HandleOnPackageChange(Actor *actor, TESPackage *package);
	/*5A0*/virtual void		HandleOnPackageEnd(Actor *actor, TESPackage *package);
	/*5A4*/virtual void		Unk_169();
	/*5A8*/virtual void		Unk_16A();
	/*5AC*/virtual float	GetActorAlpha();
	/*5B0*/virtual void		SetActorAlpha(float alpha);
	/*5B4*/virtual float	GetFlt174();
	/*5B8*/virtual void		SetFlt174(float setTo);
	/*5BC*/virtual void		Unk_16F();
	/*5C0*/virtual UInt32	GetUnk39C();
	/*5C4*/virtual void		IncUnk39C(UInt32 incBy);
	/*5C8*/virtual void		Unk_172();
	/*5CC*/virtual void		Unk_173();
	/*5D0*/virtual void		Unk_174();
	/*5D4*/virtual void		SetUnk3AC(UInt32 setTo);
	/*5D8*/virtual UInt32	GetUnk3AC();
	/*5DC*/virtual void		SetUnk3B0(UInt32 setTo);
	/*5E0*/virtual UInt32	GetUnk3B0();
	/*5E4*/virtual void		Unk_179();
	/*5E8*/virtual void		Unk_17A();
	/*5EC*/virtual void		Unk_17B();
	/*5F0*/virtual BSBound	*GetBoundingBox();
	/*5F4*/virtual void		SetBoundingBox(BSBound *bounds);
	/*5F8*/virtual void		SetByte18D(UInt8 setTo);
	/*5FC*/virtual UInt8	GetByte18D();
	/*600*/virtual void		SetByte445(UInt8 setTo);
	/*604*/virtual UInt8	GetByte445();
	/*608*/virtual tList<void>	*GetList38C();
	/*60C*/virtual void		Unk_183();
	/*610*/virtual UInt32	GetFadeType();
	/*614*/virtual void		SetQueuedIdleFlags(UInt32 flags);
	/*618*/virtual UInt32	GetQueuedIdleFlags();
	/*61C*/virtual void		ResetQueuedIdleFlags();
	/*620*/virtual void		UnsetQueuedIdleFlag(UInt32 flag);
	/*624*/virtual void		Unk_189();
	/*628*/virtual void		Unk_18A(Actor *actor);
	/*62C*/virtual void		Unk_18B(TESObjectREFR *target);
	/*630*/virtual void		Unk_18C();
	/*634*/virtual void		Unk_18D();
	/*638*/virtual void		Unk_18E();
	/*63C*/virtual void		Unk_18F();
	/*640*/virtual void		Unk_190();
	/*644*/virtual void		Unk_191();
	/*648*/virtual void		Unk_192(UInt8 unk);
	/*64C*/virtual void		Unk_193();
	/*650*/virtual void		Unk_194();
	/*654*/virtual void		Unk_195();
	/*658*/virtual void		UpdateFlt3C0();
	/*65C*/virtual float	GetFlt3C0();
	/*660*/virtual void		Unk_198();
	/*664*/virtual void		Unk_199();
	/*668*/virtual void		Unk_19A();
	/*66C*/virtual void		Unk_19B();
	/*670*/virtual bool		GetNotByte414();
	/*674*/virtual void		Unk_19D();
	/*678*/virtual void		Unk_19E();
	/*67C*/virtual void		Unk_19F();
	/*680*/virtual void		Unk_1A0();
	/*684*/virtual void		SetByte420(UInt8 setTo);
	/*688*/virtual UInt8	GetByte420();
	/*68C*/virtual void		Unk_1A3();
	/*690*/virtual void		Unk_1A4();
	/*694*/virtual void		SetByte18B(UInt8 setTo);
	/*698*/virtual UInt8	GetByte18B();
	/*69C*/virtual float	GetGameDayDied();
	/*6A0*/virtual void		SetGameDayDied(float dayDied);
	/*6A4*/virtual void		Unk_1A9();
	/*6A8*/virtual void		Unk_1AA();
	/*6AC*/virtual void		Unk_1AB();
	/*6B0*/virtual void		Unk_1AC();
	/*6B4*/virtual void		SetMuzzleFlash(Projectile *projRef, TESObjectREFR *refr);
	/*6B8*/virtual MuzzleFlash	*GetMuzzleFlash();
	/*6BC*/virtual void		Unk_1AF();
	/*6C0*/virtual void		Unk_1B0();
	/*6C4*/virtual void		ClearMuzzleFlash();
	/*6C8*/virtual void		Unk_1B2();
	/*6CC*/virtual void		SetLimbNode(UInt32 limbIdx, NiNode *node);
	/*6D0*/virtual NiNode	*GetLimbNode(UInt32 limbIdx);
	/*6D4*/virtual NiNode	*GetNode218();
	/*6D8*/virtual NiNode	*GetNode21C();
	/*6DC*/virtual UInt8	GetByte1D9();
	/*6E0*/virtual void		SetByte1D9(UInt8 setTo);
	/*6E4*/virtual void		RefreshWearArmorFlags(Actor *actor);
	/*6E8*/virtual bool		IsWearingHeavyArmor();
	/*6EC*/virtual bool		IsWearingPowerArmorTorso();
	/*6F0*/virtual bool		IsWearingPowerArmorHelmet();
	/*6F4*/virtual bool		IsWearingBackpack();
	/*6F8*/virtual void		SetByte3B9(UInt8 setTo);
	/*6FC*/virtual UInt8	GetByte3B9();
	/*700*/virtual void		Unk_1C0();
	/*704*/virtual void		Unk_1C1();
	/*708*/virtual void		SetWord22A(UInt16 setTo);
	/*70C*/virtual void		PlayCrippledLimbAnim(Actor *actor);
	/*710*/virtual void		Unk_1C4();
	/*714*/virtual void		Unk_1C5();
	/*718*/virtual TESIdleForm	*GetQueuedIdleForm();
	/*71C*/virtual void		SetQueuedIdleForm(TESIdleForm *idleForm);
	/*720*/virtual void		Unk_1C8();
	/*724*/virtual NiRefObject	*GetObject354();
	/*728*/virtual NiRefObject	*GetObject35C();
	/*72C*/virtual void		IncFltAC(Actor *actor, float incBy);
	/*730*/virtual float	GetFltAC();
	/*734*/virtual float	GetLightAmount();
	/*738*/virtual void		SetLightAmount(float lightAmount);
	/*73C*/virtual void		Unk_1CF();
	/*740*/virtual void		Unk_1D0();
	/*744*/virtual void		SetUnk1A0(void *setTo);
	/*748*/virtual void		*GetUnk1A0();
	/*74C*/virtual void		SetUnk1A4(UInt32 setTo);
	/*750*/virtual UInt32	GetUnk1A4();
	/*754*/virtual void		IncRads238(float incBy);
	/*758*/virtual void		DecRads238(float decBy);
	/*75C*/virtual float	GetRads238();
	/*760*/virtual void		SetWaterRadsSec(float radsSec);
	/*764*/virtual float	GetWaterRadsSec();
	/*768*/virtual void		SetRadsSec234(float value);
	/*76C*/virtual float	GetRadsSec();
	/*770*/virtual ActorHitData *GetHitData();
	/*774*/virtual void		CopyHitData(ActorHitData *hitData);
	/*778*/virtual void		ResetHitData();
	/*77C*/virtual ActorHitData *GetHitData254();
	/*780*/virtual void		CopyHitData254(ActorHitData *hitData);
	/*784*/virtual void		ResetHitData254();
	/*788*/virtual void		*GetUnk244();
	/*78C*/virtual void		SetUnk244(void *setTo);
	/*790*/virtual void		*GetUnk248();
	/*794*/virtual void		SetUnk248(void *setTo);
	/*798*/virtual void		*GetUnk24C();
	/*79C*/virtual void		SetUnk24C(void *setTo);
	/*7A0*/virtual void		*GetUnk250();
	/*7A4*/virtual void		SetUnk250(void *setTo);
	/*7A8*/virtual void		Unk_1EA();
	/*7AC*/virtual void		Unk_1EB();
	/*7B0*/virtual void		Unk_1EC();
	/*7B4*/virtual void		ProcessEscort(Actor *packageUser);

	PackageInfo		currentPackage;	// 04
	float			unk1C;			// 1C	not initialized, only by descendant!
	float			hourLastProcessed;	// 20	not initialized, only by descendant to -1.0! flt020 gets set to GameHour minus one on package evaluation
	UInt32			dateLastProcessed;	// 24	not initialized, only by descendant!
	UInt32			processLevel;	// 28	not initialized, only by descendant to 3 for Low, 2 for MidlleLow, 1 MiddleHighProcess and 0 for HigProcess
	CachedValues	*cachedValues;	// 2C
};

// B4
class LowProcess : public BaseProcess
{
public:
	struct FloatPair
	{
		float	flt000;
		float	flt004;
	};

	struct ActorValueModifier
	{
		UInt8	actorValue;	// 00 Might allow for other values
		UInt8	pad[3];		// 01
		float	damage;		// 04
	};

	struct ActorValueModifiers
	{
		tList<ActorValueModifier>	avModifierList;	// 00
		UInt8						unk008;			// 08
		UInt8						pad009[3];		// 09
		void						**modifiedAV;	// 0C	array of damaged actorValue
	};	// 10

	/*7B8*/virtual void		ProcessSleep(Actor *packageUser);
	/*7BC*/virtual void		ProcessEat(Actor *packageUser, TESForm *arg2);
	/*7C0*/virtual void		Unk_1F0();
	/*7C4*/virtual void		ProcessAlarm(Actor *packageUser);
	/*7C8*/virtual bool		ProcessActivate(Actor *packageUser, bool arg2);
	/*7CC*/virtual void		ProcessAccompany(Actor *packageUser);
	/*7D0*/virtual void		ProcessAcquire(Actor *packageUser);
	/*7D4*/virtual void		ProcessAmbushWait(Actor *packageUser);
	/*7D8*/virtual void		ProcessUseItemAt(Actor *packageUser);
	/*7DC*/virtual bool		ProcessUseWeapon(Actor *packageUser);
	/*7E0*/virtual void		Unk_1F8();
	/*7E4*/virtual void		ProcessFleeNonCombat(Actor *packageUser);
	/*7E8*/virtual void		ProcessAvoidRadiation(Actor *packageUser);
	/*7EC*/virtual void		ProcessSandbox(Actor *packageUser);
	/*7F0*/virtual void		ProcessMovementBlocked(Actor *packageUser);
	/*7F4*/virtual void		ProcessGuard(Actor *packageUser);
	/*7F8*/virtual void		Unk_1FE();
	/*7FC*/virtual void		ProcessPatrol(Actor *packageUser);
	/*800*/virtual void		Unk_200();
	/*804*/virtual void		Unk_201();
	/*808*/virtual void		ProcessNotify(Actor *packageUser);
	/*80C*/virtual void		Unk_203();
	/*810*/virtual void		Unk_204();
	/*814*/virtual void		Unk_205();
	/*818*/virtual void		Unk_206();

	UInt8				byte30;				// 30	8 = IsAlerted
	UInt8				pad31[3];			// 31
	UInt32				unk34;				// 34
	FloatPair			unk38;				// 38
	TESObjectREFR		*interactedRef;		// 40	Used when picking idle anims.
	UInt32				unk44;				// 44
	UInt32				unk48;				// 48
	UInt32				unk4C;				// 4C
	UInt32				unk50;				// 50
	UInt32				unk54;				// 54
	UInt32				unk58;				// 58
	tList<void>			list5C;				// 5C
	UInt32				unk64;				// 64
	UInt32				unk68;				// 68
	tList<void>			list6C;				// 6C
	tList<void>			list74;				// 74
	tList<void>			list7C;				// 7C
	UInt32				unk84;				// 84
	UInt32				unk88;				// 88
	UInt32				unk8C;				// 8C
	UInt32				unk90;				// 90
	ActorValueModifiers	damageModifiers;	// 94
	float				essentialDownTimer;	// A4
	float				gameDayDied;		// A8
	float				fltAC;				// AC
	UInt32				unkB0;				// B0
};

// C8
class MiddleLowProcess : public LowProcess
{
public:
	/*81C*/virtual void		Unk_207();

	UInt32				unk0B4;			// B4
	ActorValueModifiers	tempModifiers;	// B8
};

// 04
class AnimSequenceBase
{
public:
	virtual void	Destroy(bool deFree);
	virtual void	AddAnimGroupSequence(BSAnimGroupSequence *sequence, UInt32 arg2);
	virtual bool	RemoveAnimGroupSequence(BSAnimGroupSequence *sequence, UInt32 arg2);
	virtual bool	IsSingle();
	virtual BSAnimGroupSequence	*GetSequenceByIndex(UInt8 index);
	virtual BSAnimGroupSequence	*GetSequenceByGroup(TESAnimGroup *animGroup);
	virtual char	GetSequenceIndexByName(const char *seqName);
};

// 08
class AnimSequenceSingle : public AnimSequenceBase
{
public:
	BSAnimGroupSequence		*animSequence;	// 04
};

// 08
class AnimSequenceMultiple : public AnimSequenceBase
{
public:
	DList<BSAnimGroupSequence>	*sequenceList;	// 04
};

enum AnimAction
{
	kAnimAction_None = -1,
	kAnimAction_Equip_Weapon = 0,
	kAnimAction_Unequip_Weapon,
	kAnimAction_Attack,
	kAnimAction_Attack_Eject,
	kAnimAction_Attack_Follow_Through,
	kAnimAction_Attack_Throw,
	kAnimAction_Attack_Throw_Attach,
	kAnimAction_Block,
	kAnimAction_Recoil,
	kAnimAction_Reload,
	kAnimAction_Stagger,
	kAnimAction_Dodge,
	kAnimAction_Wait_For_Lower_Body_Anim,
	kAnimAction_Wait_For_Special_Idle,
	kAnimAction_Force_Script_Anim,
	kAnimAction_ReloadLoopStart,
	kAnimAction_ReloadLoopEnd,
	kAnimAction_ReloadLoop
};

struct KFModel;
class AnimIdle;

// 13C
struct AnimData
{
	enum SequenceTypes
	{
		kSequence_None =		-1,
		kSequence_Idle =		0,
		kSequence_Movement =	1,
		kSequence_LeftArm =		2,
		kSequence_LeftHand =	3,
		kSequence_Weapon =		4,
		kSequence_WeaponUp =	5,
		kSequence_WeaponDown =	6,
		kSequence_SpecialIdle =	7,
		kSequence_Death =		0x14
	};

	enum AnimStates
	{

	};

	struct PlayingIdle
	{
		TESIdleForm		*idleForm;
		float			replayDelay;
	};

	UInt32							unk000;				// 000
	Actor							*actor;				// 004
	NiNode							*nSceneRoot;		// 008
	NiNode							*nBip01;			// 00C
	UInt32							unk010;				// 010
	float							flt014;				// 014
	float							flt018;				// 018
	NiVector3						pos01C;				// 01C
	NiNode							*nPelvis;			// 028
	NiNode							*nBip01Copy;		// 02C
	NiNode							*nLForearm;			// 030
	NiNode							*nHead;				// 034
	NiNode							*nWeapon;			// 038
	UInt32							unk03C[2];			// 03C
	NiNode							*nNeck1;			// 044
	float							flt048;				// 048
	UInt16							animGroupIDs[8];	// 04C
	SInt32							sequenceState1[8];	// 05C
	SInt32							sequenceState2[8];	// 07C
	UInt16							word09C[8];			// 09C
	UInt32							unk0AC[8];			// 0AC
	UInt8							byte0CC;			// 0CC
	UInt8							byte0CD;			// 0CD
	UInt8							byte0CE;			// 0CE
	UInt8							byte0CF;			// 0CF
	float							timePassed;			// 0D0
	UInt32							unk0D4;				// 0D4
	NiControllerManager				*controllerMngr;	// 0D8
	NiTPtrMap<AnimSequenceBase>		*sequenceBaseMap;	// 0DC
	BSAnimGroupSequence				*animSequence[8];	// 0E0
	BSAnimGroupSequence				*animSeq100;		// 100
	tList<KFModel>					loadingAnims;		// 104
	float							movementSpeedMult;	// 10C
	float							rateOfFire;			// 110
	float							turboSpeedMult;		// 114
	float							weaponReloadSpeed;	// 118
	float							equipSpeed;			// 11C
	UInt8							noBlend;			// 120
	UInt8							byte121;			// 121
	UInt16							word122;			// 122
	AnimIdle						*idleAnim;			// 124
	AnimIdle						*queuedIdleAnim;	// 128
	NiObject						*object12C;			// 12C
	NiObject						*object130;			// 130
	tList<PlayingIdle>				playingIdleAnims;	// 134

	TESIdleForm *GetPlayedIdle() const;

	__forceinline void PlayIdle(TESIdleForm *idleAnim)
	{
		ThisCall(0x497F20, this, idleAnim, actor, idleAnim->data.groupFlags & 0x3F, 3);
	}

	__forceinline void StopIdle()
	{
		ThisCall(0x498910, this, true, false);
	}

	__forceinline void Refresh()
	{
		ThisCall(0x499240, this, 0);
	}

	void BlendSequence(UInt32 sequenceIdx)
	{
		if (animSequence[sequenceIdx])
			ThisCall(0x4994F0, this, sequenceIdx, 0);
	}
};
static_assert(sizeof(AnimData) == 0x13C);

class QueuedFile;

// 25C
class MiddleHighProcess : public MiddleLowProcess
{
public:
	/*820*/virtual void		ReplaceAnimData(AnimData *newData);
	/*824*/virtual void		Unk_209();
	/*828*/virtual void		SetByte375(UInt8 setTo);
	/*82C*/virtual void		Unk_20B();
	/*830*/virtual void		Unk_20C();
	/*834*/virtual void		Unk_20D();
	/*838*/virtual void		Unk_20E();
	/*83C*/virtual void		Unk_20F();
	/*840*/virtual void		Unk_210();
	/*844*/virtual void		Unk_211();
	/*848*/virtual void		Unk_212();
	/*84C*/virtual void		Unk_213();
	/*850*/virtual void		Unk_214();
	/*854*/virtual void		Unk_215();
	/*858*/virtual void		Unk_216();
	/*85C*/virtual void		Unk_217();
	/*860*/virtual void		Unk_218();
	/*864*/virtual void		Unk_219();
	/*868*/virtual void		Unk_21A();
	/*86C*/virtual void		Unk_21B();

	enum KnockedState
	{
		kState_None,
		kState_KnockedDown,
		kState_Ragdolled,
		kState_Unconscious,
		kState_Unknown4,
		kState_Unknown5,
		kState_GettingUp
	};

	// 10
	struct FurnitureMark
	{
		NiVector3	pos;
		UInt16		rotation;
		UInt8		type;
		UInt8		byte0F;
	};

	// 18
	struct QueuedEquipItem
	{
		TESForm			*itemForm;			// 00
		ExtraDataList	*xDataList;			// 04
		UInt32			count;				// 08
		bool			doEquip;			// 0C
		bool			applyEnchantment;	// 0D
		bool			lockEquip;			// 0E
		bool			playSound;			// 0F
		bool			removeEnchantment;	// 10
		bool			unkArg9;			// 11
		UInt8			pad12[2];			// 12
		QueuedFile		*queuedFile;		// 14
	};

	tList<TESForm>				list0C8;			// 0C8
	tList<TESObjectREFR>		furnitureRefList;	// 0D0
	float						pursueTimer;		// 0D8
	float						flt0DC;				// 0DC
	UInt8						idleDoneOnce;		// 0E0
	UInt8						byte0E1;			// 0E1
	UInt8						byte0E2;			// 0E2
	UInt8						byte0E3;			// 0E3
	PackageInfo					interruptPackage;	// 0E4
	NiVector3					lastSeenPosition;	// 0FC
	UInt32						packageObjType;		// 108
	TESIdleForm					*lastPlayedIdle;	// 10C
	UInt8						byte110;			// 110
	UInt8						byte111;			// 111
	UInt8						byte112;			// 112
	UInt8						byte113;			// 113
	ContChangesEntry			*weaponInfo;		// 114
	ContChangesEntry			*ammoInfo;			// 118
	QueuedFile					*unk11C;			// 11C
	NiRefObject					*object120;			// 120
	UInt8						usingOneHandGrenade;// 124
	UInt8						usingOneHandMine;	// 125
	UInt8						usingOneHandThrown;	// 126
	UInt8						wearingHeavyArmor;	// 127
	UInt8						wearingPowerArmorTorso;	// 128
	UInt8						wearingPowerArmorHelmet;// 129
	UInt8						wearingBackpack;	// 12A
	UInt8						byte12B;			// 12B
	NiNode						*weaponNode;		// 12C
	NiNode						*projectileNode;	// 130
	bool						wantWeaponOut;		// 134
	bool						isWeaponOut;		// 135
	UInt8						byt136;				// 136
	UInt8						byt137;				// 137
	bhkCharacterController		*charCtrl;			// 138
	SInt8						knockedState;		// 13C
	UInt8						sitSleepState;		// 13D
	UInt8						unk13E[2];			// 13E
	TESObjectREFR				*usedFurniture;		// 140
	UInt8						markerIndex;		// 144
	UInt8						unk145[3];			// 145
	FurnitureMark				furnitureData;		// 148
	Actor						*commandingActor;	// 158
	UInt32						unk15C;				// 15C
	MagicItem					*magicItem160;		// 160
	UInt32						unk164;				// 164
	UInt8						byte168;			// 168
	UInt8						byte169;			// 169
	UInt8						byte16A;			// 16A
	UInt8						byte16B;			// 16B
	UInt32						unk16C;				// 16C
	float						actorAlpha;			// 170
	float						flt174;				// 174
	BSFaceGenAnimationData		*faceGenAnimData;	// 178
	UInt8						byte17C;			// 17C
	UInt8						byte17D;			// 17D
	UInt8						byte17E;			// 17E
	UInt8						byte17F;			// 17F
	UInt8						byte180;			// 180
	UInt8						byte181;			// 181
	UInt8						byte182;			// 182
	UInt8						byte183;			// 183
	UInt32						unk184;				// 184
	UInt8						hasCaughtPCPickpocketting;	// 188
	UInt8						byte189;			// 189
	UInt8						byte18A;			// 18A
	UInt8						byte18B;			// 18B
	UInt8						byte18C;			// 18C
	UInt8						forceNextUpdate;	// 18D
	UInt8						pad18E[2];			// 18E
	UInt32						unk190[2];			// 190
	float						flt198;				// 198
	UInt8						byte19C;			// 19C
	UInt8						byte19D;			// 19D
	UInt8						byte19E;			// 19E
	UInt8						byte19F;			// 19F
	void						*ptr1A0;			// 1A0
	UInt32						unk1A4;				// 1A4
	UInt32						unk1A8;				// 1A8
	tList<Projectile>			*arrowProjectiles;	// 1AC
	tList<void>					list1B0;			// 1B0
	tList<ActiveEffect>			*activeEffects;		// 1B8
	MagicTarget					*magicTarget1BC;	// 1BC
	AnimData					*animData;			// 1C0
	BSAnimGroupSequence			*animSequence[3];	// 1C4
	float						flt1D0;				// 1D0
	float						flt1D4;				// 1D4
	UInt8						byte1D8;			// 1D8
	UInt8						usingAutoWeapon;	// 1D9
	UInt8						isAttacked;			// 1DA
	UInt8						byte1DB;			// 1DB
	NiNode						*limbNodes[15];		// 1DC
	NiNode						*node218;			// 218
	NiNode						*node21C;			// 21C
	BSShaderLightingProperty	*shaderProp220;		// 220
	BSBound						*boundingBox;		// 224
	bool						isAiming;			// 228
	UInt8						byte229;			// 229
	UInt16						word22A;			// 22A
	UInt32						unk22C;				// 22C
	tList<QueuedEquipItem>		*queuedEquipList;	// 230
	float						radsSec234;			// 234
	float						rads238;			// 238
	float						waterRadsSec;		// 23C
	ActorHitData				*lastHitData;		// 240
	UInt32						unk244;				// 244
	BSFaceGenNiNode				*unk248;			// 248
	BSFaceGenNiNode				*unk24C;			// 24C
	NiTriShape					*unk250;			// 250
	ActorHitData				*lastTargetHitData;	// 254
	UInt32						unk258;				// 258
};
static_assert(sizeof(MiddleHighProcess) == 0x25C);

// 46C
class HighProcess : public MiddleHighProcess
{
public:
	/*870*/virtual tList<void>	*GetList394();

	struct MoveToInfo
	{
		TESObjectREFR	*destRef;
		NiVector3		posMods;
	};

	tList<DetectionData>				*detectedActors;	// 25C
	tList<DetectionData>				*detectingActors;	// 260
	tList<void>							*lstPtr264;			// 264
	tList<void>							*lstPtr268;			// 268
	tList<void>							*lstPtr26C;			// 26C
	UInt8								byte270;			// 270
	UInt8								byte271;			// 271
	UInt8								byte272;			// 272
	UInt8								byte273;			// 273
	tList<CombatTarget>					list274;			// 274
	tList<void>							list27C;			// 27C
	tList<void>							list284;			// 284
	tList<void>							list28C;			// 28C
	float								detectListTimer;	// 294
	float								idleChatterCommentTimer;// 298
	UInt8								byte29C;			// 29C
	UInt8								byte29D;			// 29D
	UInt8								byte29E;			// 29E
	UInt8								byte29F;			// 29F
	float								flt2A0;				// 2A0
	UInt32								unk2A4;				// 2A4
	float								flt2A8;				// 2A8
	TESObjectREFR						*turnToTarget;		// 2AC
	float								actorAlpha2;		// 2B0
	float								packageEvalTimer;	// 2B4
	float								useItemTimer;		// 2B8
	float								flt2BC;				// 2BC
	SInt16								word2C0;			// 2C0
	SInt16								word2C2;			// 2C2
	SInt16								word2C4;			// 2C4
	UInt8								byte2C6;			// 2C6
	UInt8								byte2C7;			// 2C7
	float								flt2C8;				// 2C8
	UInt32								unk2CC;				// 2CC
	float								flt2D0;				// 2D0
	float								flt2D4;				// 2D4
	float								flt2D8;				// 2D8
	UInt8								byte2DC;			// 2DC
	UInt8								byte2DD;			// 2DD
	UInt8								byte2DE;			// 2DE
	UInt8								byte2DF;			// 2DF
	float								flt2E0;				// 2E0
	NiBSBoneLODController				*ptr2E4;			// 2E4
	UInt32								unk2E8;				// 2E8
	SInt16								currentAction;		// 2EC
	UInt8								pad2EE[2];			// 2EE
	BSAnimGroupSequence					*currentSequence;	// 2F0
	UInt8								forceFireWeapon;	// 2F4
	UInt8								pad2F5[3];			// 2F5
	float								detectionTimer;		// 2F8
	UInt32								unk2FC;				// 2FC
	NiVector3							vec300;				// 300
	UInt32								unk30C;				// 30C
	float								flt310;				// 310
	Sound								playedSounds[2];	// 314
	UInt8								greetingFlag;		// 32C
	UInt8								byte32D;			// 32D
	UInt8								byte32E;			// 32E
	UInt8								byte32F;			// 32F
	float								dyingTimer;			// 330
	float								flt334;				// 334
	float								flt338;				// 338
	float								diveBreath;			// 33C
	UInt32								unk340;				// 340
	float								painSoundTimer;		// 344
	UInt8								byte348;			// 348
	UInt8								weaponAlertDrawn;	// 349
	UInt8								byte34A;			// 34A
	UInt8								byte34B;			// 34B
	float								awarePlayerTimer;	// 34C
	TESIdleForm							*queuedIdle;		// 350
	NiRefObject							*object354;			// 354
	NiRefObject							*object358;			// 358
	void								*object35C;			// 35C
	void								*object360;			// 360
	NiBSplineCompTransformInterpolator	**ptr364;			// 364
	void								*ptr368;			// 368
	UInt32								unk36C[2];			// 36C
	UInt8								continueIfPCNear;	// 374
	UInt8								byte375;			// 375
	UInt8								byte376;			// 376
	UInt8								byte377;			// 377
	float								VATSDamageMult;		// 378
	float								flt37C;				// 37C
	NiRefObject							*object380;			// 380
	float								flt384;				// 384
	float								flt388;				// 388
	tList<void>							list38C;			// 38C
	tList<void>							list394;			// 394
	UInt32								numGuardsPursuing;	// 39C
	UInt8								bStop;				// 3A0
	UInt8								byte3A1;			// 3A1
	UInt8								byte3A2;			// 3A2
	UInt8								byte3A3;			// 3A3
	float								flt3A4;				// 3A4
	UInt8								byte3A8;			// 3A8
	UInt8								byte3A9;			// 3A9
	UInt8								byte3AA;			// 3AA
	UInt8								byte3AB;			// 3AB
	UInt32								unk3AC;				// 3AC
	UInt32								unk3B0;				// 3B0
	tList<void>							*list3B4;			// 3B4
	UInt8								byte3B8;			// 3B8
	UInt8								byte3B9;			// 3B9
	UInt8								byte3BA;			// 3BA
	UInt8								byte3BB;			// 3BB
	float								flt3BC;				// 3BC
	float								detectionActionTimer;	// 3C0
	float								lightAmount;		// 3C4
	float								lightUpdateTimer;	// 3C8
	void								*ptr3CC;			// 3CC
	UInt8								byte3D0;			// 3D0
	UInt8								byte3D1;			// 3D1
	UInt8								byte3D2;			// 3D2
	UInt8								byte3D3;			// 3D3
	MuzzleFlash							*muzzleFlash;		// 3D4
	UInt32								unk3D8;				// 3D8
	DetectionEvent						*detectionEvent;	// 3DC
	UInt8								byte3E0;			// 3E0
	UInt8								byte3E1;			// 3E1
	UInt8								byte3E2;			// 3E2
	UInt8								byte3E3;			// 3E3
	UInt32								unk3E4;				// 3E4
	UInt32								fadeType;			// 3E8
	float								delayTime;			// 3EC
	UInt32								unk3F0;				// 3F0
	MoveToInfo							*delayedMoveTo;		// 3F4
	TESObjectREFR						*defaultHeadTrack;	// 3F8
	TESObjectREFR						*actionHeadTrack;	// 3FC
	TESObjectREFR						*scriptHeadTrack;	// 400
	Actor								*combatHeadTrack;	// 404
	Actor								*dialogHeadTrack;	// 408
	TESObjectREFR						*procedureHeadTrack;// 40C
	UInt8								byte410;			// 410
	UInt8								byte411;			// 411
	UInt8								byte412;			// 412
	UInt8								byte413;			// 413
	UInt8								byte414;			// 414
	UInt8								byte415;			// 415
	UInt8								byte416;			// 416
	UInt8								byte417;			// 417
	float								headTrackTimer;		// 418
	TESObjectREFR						*packageTarget;		// 41C
	UInt8								byte420;			// 420
	UInt8								byte421;			// 421
	UInt8								byte422;			// 422
	UInt8								byte423;			// 423
	UInt32								queuedIdleFlags;	// 424
	StoredActorValues					*storedAVs;			// 428
	float								flt42C;				// 42C
	UInt32								unk430;				// 430
	bhkShapePhantom						*ptr434;			// 434
	UInt32								unk438;				// 438
	float								radReevalTimer;		// 43C
	float								radsSec440;			// 440
	UInt8								plantedExplosive;	// 444
	UInt8								forceTalkToPCAfterGettingUp;// 445
	UInt8								byte446;			// 446
	UInt8								byte447;			// 447
	float								takeBackTimer;		// 448
	UInt32								unk44C;				// 44C
	float								flt450;				// 450
	UInt32								unk454;				// 454
	UInt8								isHiding;			// 458
	UInt8								isDoingSayTo;		// 459
	UInt8								byte45A;			// 45A
	UInt8								byte45B;			// 45B
	NiRefObject							*object45C;			// 45C
	NiRefObject							*object460;			// 460
	void								*object464;			// 464
	void								*object468;			// 468
};
static_assert(sizeof(HighProcess) == 0x46C);

// 0C
struct TaskletInfo
{
	enum TaskletStage
	{
		kStage_EnterCS,
		kStage_Execute,
		kStage_LeaveCS
	};

	UInt32			stage;
	UInt32			unk04;	// Unused
	TESObjectREFR	*execOn;
};

// 08
class BSTaskletData
{
public:
	virtual void	Destroy(bool doFree);
	virtual bool	Unk_01(void);	// Returns true
	virtual void	ExecuteQueuedTasks();
	virtual void	Unk_03(void);	// Does nothing

	UInt8		byte04;		// 04
	UInt8		pad05[3];	// 05
};

// 14
class BSWin32TaskletData : public BSTaskletData
{
public:
	UInt32		unk08;		// 08
	UInt8		byte0C;		// 0C
	UInt8		pad0D[3];	// 0D
	UInt32		unk10;		// 10
};

// 14
class BSTCommonLLMessageQueue
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);

	UInt32		unk04[4];	// 04
};

// 38
class MobileObjectTaskletData : public BSWin32TaskletData
{
public:
	virtual void	EnterCriticalSections(UInt32 unused);
	virtual void	ExecuteTask(TESObjectREFR *execOn);
	virtual void	LeaveCriticalSections(UInt32 unused);

	UInt32					unk14[2];		// 14
	BSTCommonLLMessageQueue	messageQueue;	// 1C
	UInt32					unk30[2];		// 30
};

// 40
class DetectionTaskData : public MobileObjectTaskletData
{
public:
	float			flt38;		// 38
	UInt32			unk3C;		// 3C
};

// 38
class AnimationTaskData : public MobileObjectTaskletData
{
public:
};

// 44
class PackageUpdateTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38[3];	// 38
};

// 3C
class ActorUpdateTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38;		// 38
};

// 40
class ActorsScriptTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38[2];	// 38
};

// 3C
class MovementTaskData : public MobileObjectTaskletData
{
public:
	UInt32			unk38;		// 38
};

struct LipTask;

// 103CC (estimated according to highest init'd field)
// C'tor: 0x96D020
struct ProcessManager
{
	UInt32									unk000;				// 000
	NiTArray<MobileObject*>					objects;			// 004
	UInt32									beginOffsets[4];	// 014	0: High, 1: Mid-High, 2: Mid-Low, 3: Low
	UInt32									endOffsets[4];		// 024
	UInt32									offsets034[4];		// 034	Same as beginOffsets
	UInt32									unk044[5];			// 044
	tList<void>								list058;			// 058
	tList<BSTempEffect>						tempEffects;		// 060
	tList<MuzzleFlash>						muzzFlashList;		// 068
	tList<void>								list070;			// 070
	tList<void>								list078;			// 078
	tList<Actor>							highActors;			// 080
	Actor									*nearestActors[50];	// 088
	UInt32									nearestActorsCount;	// 150
	float									commentPCActionsTimer;	// 154
	float									commentPCKnockingTimer;	// 158
	UInt8									pcInRadiation1;		// 15C
	UInt8									pcInRadiation2;		// 15D
	UInt8									pad15E[2];			// 15E
	DetectionTaskData						detectionTasks;		// 160
	AnimationTaskData						animationTasks;		// 1A0
	PackageUpdateTaskData					packageUpdTasks;	// 1D8
	ActorUpdateTaskData						actorUpdTasks;		// 21C
	ActorsScriptTaskData					actorScriptTasks;	// 258
	MovementTaskData						movementTasks;		// 298
	UInt32									unk2D4[0x4023];		// 2D4
	LockFreeMap<MobileObject*, LipTask*>	lipTasksMap;		// 10360
	UInt8									toggleAI;			// 103A0
	UInt8									toggleDetection;	// 103A1
	UInt8									toggleDetectionStats;	// 103A2
	UInt8									byte103A3;			// 103A3
	UInt32									detectionStats;		// 103A4
	UInt8									toggleHighProcess;	// 103A8
	UInt8									toggleLowProcess;	// 103A9
	UInt8									toggleMidHighProcess;	// 103AA
	UInt8									toggleMidLowProcess;	// 103AB
	UInt8									toggleAISchedules;	// 103AC
	UInt8									showSubtitle;		// 103AD
	UInt8									byte103AE;			// 103AE
	UInt8									byte103AF;			// 103AF
	UInt32									numHighActors;		// 103B0
	float									crimeUpdateTimer;	// 103B4
	UInt32									crimeNumber;		// 103B8
	float									removeDeadActorsTime;	// 103BC
	UInt32									unk103C0[3];		// 103C0

	__forceinline static ProcessManager *Get() {return (ProcessManager*)0x11E0E80;}

	__forceinline int GetTotalDetectionValue(Actor *actor, bool arg2 = false)
	{
		return ThisCall<int>(0x973710, this, actor, arg2);
	}

	void UnCullHighActors()
	{
		auto iter = highActors.Head();
		do
		{
			if (Actor *actor = iter->data)
				if (NiNode *rootNode = actor->GetRefNiNode())
					rootNode->m_flags &= ~NiAVObject::kNiFlag_Culled;
		}
		while (iter = iter->next);
	}
};
static_assert(sizeof(ProcessManager) == 0x103CC);

struct CombatSearchLocation;
struct CombatSearchDoor;
struct CombatGroupCluster;
class PathingCoverLocation;
struct UnreachableLocation;
struct UnreachableCoverLocation;

// 1C
class CombatGoal
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);

	tList<void>		list04;		// 04
	tList<void>		list0C;		// 0C
	UInt32			type;		// 14
	UInt32			unk18;		// 18
};

// 0C
struct CombatActionInfo
{
	const char	*name;	// 00
	float		cost;	// 04
	UInt32		flags;	// 08

	__forceinline static CombatActionInfo *Array() {return (CombatActionInfo*)0x11A4280;}
};

enum CombatActions
{
	/*00*/COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE,
	/*01*/COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE_FROM_COVER,
	/*02*/COMBAT_ACTION_ATTACK_RANGED,
	/*03*/COMBAT_ACTION_ATTACK_RANGED_FROM_COVER,
	/*04*/COMBAT_ACTION_ATTACK_GRENADE,
	/*05*/COMBAT_ACTION_ATTACK_GRENADE_FLUSH_TARGET,
	/*06*/COMBAT_ACTION_ATTACK_GRENADE_FROM_COVER,
	/*07*/COMBAT_ACTION_ATTACK_MELEE,
	/*08*/COMBAT_ACTION_ATTACK_HAND_TO_HAND,
	/*09*/COMBAT_ACTION_MOVE,
	/*0A*/COMBAT_ACTION_MOVE_AND_SWITCH_TO_MELEE,
	/*0B*/COMBAT_ACTION_MOVE_AND_ATTACK_RANGED,
	/*0C*/COMBAT_ACTION_MOVE_AND_ATTACK_RANGED_EXPLOSIVE,
	/*0D*/COMBAT_ACTION_MOVE_AND_ATTACK_GRENADE,
	/*0E*/COMBAT_ACTION_DRAW_WEAPON,
	/*0F*/COMBAT_ACTION_SWITCH_WEAPON,
	/*10*/COMBAT_ACTION_AVOID_THREAT,
	/*11*/COMBAT_ACTION_SEARCH,
	/*12*/COMBAT_ACTION_INVESTIGATE,
	/*13*/COMBAT_ACTION_DODGE,
	/*14*/COMBAT_ACTION_IGNORE_BLOCKED_TARGET,
	/*15*/COMBAT_ACTION_FLEE,
	/*16*/COMBAT_ACTION_ACTIVATE_COMBAT_ITEM,
	/*17*/COMBAT_ACTION_USE_COMBAT_ITEM,
	/*18*/COMBAT_ACTION_ACQUIRE_LINE_OF_SIGHT,
	/*19*/COMBAT_ACTION_HIDE,
	/*1A*/COMBAT_ACTION_APPROACH_TARGET
};

enum CombatProcedures
{
	/*00*/COMBAT_PROCEDURE_ATTACK_RANGED,
	/*01*/COMBAT_PROCEDURE_ATTACK_MELEE,
	/*02*/COMBAT_PROCEDURE_ATTACK_GRENADE,
	/*03*/COMBAT_PROCEDURE_ATTACK_LOW,
	/*04*/COMBAT_PROCEDURE_EVADE,
	/*05*/COMBAT_PROCEDURE_SWITCH_WEAPON,
	/*06*/COMBAT_PROCEDURE_MOVE,
	/*07*/COMBAT_PROCEDURE_BE_IN_COVER,
	/*08*/COMBAT_PROCEDURE_ACTIVATE_OBJECT,
	/*09*/COMBAT_PROCEDURE_HIDE_FROM_TARGET,
	/*0A*/COMBAT_PROCEDURE_SEARCH,
	/*0B*/COMBAT_PROCEDURE_USE_COMBAT_ITEM,
	/*0C*/COMBAT_PROCEDURE_ENGAGE_TARGET
};

// 2C
class CombatAction
{
public:
	/*00*/virtual bool	IsApplicable(CombatController *combatCtrl, void *worldState, UInt32 moveType);
	/*04*/virtual void	AddProcedureToController(CombatController *combatCtrl, void *a2);
	/*08*/virtual bool	Unk_02();
	/*0C*/virtual float	GetCost(CombatController *combatCtrl, int a2);
	/*10*/virtual void	GetName(char *buffer, UInt32 bufferSize, UInt32 subType);

	tList<void>		list04;		// 04
	tList<void>		list0C;		// 0C
	tList<void>		list14;		// 14
	tList<void>		list1C;		// 1C
	UInt32			actionID;	// 24
	UInt32			flags;		// 28
};
static_assert(sizeof(CombatAction) == 0x2C);

class CombatProcedure
{
public:
	/*00*/virtual void	Destroy(bool doFree);
	/*04*/virtual void	Update();
	/*08*/virtual void	SetCombatController(CombatController *_combatCtrl);
	/*0C*/virtual void	OnStop();
	/*10*/virtual void	ClearTargettedRefIfEqualTo(TESObjectREFR *targettedRef);
	/*14*/virtual void	DebugPrint(void);
	/*18*/virtual void	Unk_06(NiVector3 *out, NiVector3 *in);
	/*1C*/virtual void	SetLastError_Disarmed(int unused);
	/*20*/virtual void	Unk_08(int a1);
	/*24*/virtual void	Unk_09(void);
	/*28*/virtual void	Unk_0A(int a1, int a2, int a3, int a4);
	/*2C*/virtual void	Unk_0B(void);
	/*30*/virtual void	Unk_0C(void);
	/*34*/virtual UInt32	GetType();
	/*38*/virtual void	SaveGame(int a1);
	/*3C*/virtual void	LoadGame(int a1);
	/*40*/virtual void	LoadGame2(int a1);

	CombatController	*combatCtrl;	// 04
	UInt32				state;			// 08
	char				*errorText;		// 0C
};

// 18
struct CombatPlan
{
	struct Action
	{
		CombatAction	*action;
		Action			*next;
		UInt32			unk08;
		UInt32			unk0C;
	};

	Action				**actionStack;
	UInt32				numActions;
	UInt32				counter08;
	CombatGoal			*goal;
	UInt32				unk10;
	CombatController	*combatCtrl;
};

// 14
struct CombatAlly
{
	Actor		*ally;
	UInt32		unk04[4];
};

// 15C
struct CombatActors
{
	struct Unk0F0
	{
		NiVector3		vector00;
		UInt32			unk0C;
	};

	UInt32									unk000;				// 000
	UInt32									groupID;			// 004
	BSSimpleArray<CombatTarget>				targets;			// 008
	BSSimpleArray<CombatAlly>				allies;				// 018
	UInt32									unk028;				// 028
	UInt8									byte02C;			// 02C
	UInt8									pad02D[3];			// 02D
	NiPoint2								unk030;				// 030
	NiPoint2								unk038;				// 038
	UInt32									unk040;				// 040
	float									flt044;				// 044
	NiPoint2								unk048;				// 048
	NiPoint2								unk050;				// 050
	NiPoint2								unk058;				// 058
	NiPoint2								unk060;				// 060
	NiPoint2								unk068;				// 068
	NiPoint2								unk070;				// 070
	NiPoint2								unk078;				// 078
	NiPoint2								unk080;				// 080
	NiPoint2								unk088;				// 088
	NiPoint2								unk090;				// 090
	NiPoint2								unk098;				// 098
	NiPoint2								unk0A0;				// 0A0
	NiPoint2								unk0A8;				// 0A8
	NiPoint2								unk0B0;				// 0B0
	NiPoint2								unk0B8;				// 0B8
	float									sumThreatValue;		// 0C0
	float									averageDPS;			// 0C4
	float									flt0C8;				// 0C8
	float									flt0CC;				// 0CC
	UInt32									targetSearchState;	// 0D0	0 - Not searching; 1-2 - Searching
	NiRefObject								*object0D4;			// 0D4
	NiPoint2								unk0D8;				// 0D8
	NiPoint2								unk0E0;				// 0E0
	float									flt0E8;				// 0E8
	UInt32									unk0EC;				// 0EC
	Unk0F0									unk0F0;				// 0F0
	NiVector3								vector100;			// 100
	float									flt10C;				// 10C
	BSSimpleArray<CombatSearchLocation>		searchLocations;	// 110
	BSSimpleArray<CombatSearchDoor>			searchDoors;		// 120
	NiRefObject								*object130;			// 130
	UInt8									byte134;			// 134
	UInt8									pad135[3];			// 135
	BSSimpleArray<CombatGroupCluster>		groupClusters;		// 138
	UInt32									unk148;				// 148
	UInt32									unk14C;				// 14C
	UInt32									unk150;				// 150
	UInt8									byte154;			// 154
	UInt8									pad155[3];			// 155
	NiRefObject								*object158;			// 158
};
static_assert(sizeof(CombatActors) == 0x15C);

// 22C
struct CombatState
{
	UInt8									byte000;				// 000
	UInt8									pad001[3];				// 001
	UInt32									flags;					// 004
	float									fleeThreshold;			// 008
	TESObjectWEAP							*weaponsByType[6];		// 00C
	BSSimpleArray<TESObjectWEAP*>			arr024;					// 024
	TESObjectWEAP							*weapon034;				// 034
	UInt32									availableWeaponTypes;	// 038
	float									maxDPSPerWeaponType[7];	// 03C
	float									meleeDPS;				// 058
	float									rangedDPS;				// 05C
	UInt32									unk060[13];				// 060
	float									detectionBufferTimer;	// 094
	UInt32									unk098[11];				// 098
	void									*ptr0C4;				// 0C4
	UInt32									unk0C8[17];				// 0C8
	BSSimpleArray<PathingCoverLocation*>	arr10C;					// 10C
	UInt32									unk11C[11];				// 11C
	BSSimpleArray<PathingCoverLocation*>	arr148;					// 148
	UInt32									unk158[3];				// 158
	BSSimpleArray<UnreachableCoverLocation*>	arr164;				// 164
	BSSimpleArray<UnreachableLocation*>		arr174;					// 174
	UInt32									unk184[15];				// 184
	Actor									*actor1C0;				// 1C0
	CombatController						*cmbtCtrl;				// 1C4
	UInt32									unk1C8[25];				// 1C8
};
static_assert(sizeof(CombatState) == 0x22C);

// 188
class CombatController : public TESPackage
{
public:
	CombatActors					*combatGroup;		// 080
	CombatProcedure					*combatProcedure1;	// 084
	CombatProcedure					*combatProcedure2;	// 088
	BSSimpleArray<CombatProcedure*>	combatProcedures;	// 08C
	CombatState						*combatState;		// 09C
	CombatPlan						*combatPlan;		// 0A0
	UInt32							unk0A4;				// 0A4
	CombatAction					*combatAction;		// 0A8
	CombatGoal						*combatGoal;		// 0AC
	UInt32							unk0B0;				// 0B0
	TimePair						combatStyleChangeTimer;	// 0B4
	Actor							*packageOwner;		// 0BC
	Actor							*packageTarget;		// 0C0
	UInt8							byte0C4;			// 0C4
	UInt8							byte0C5;			// 0C5
	UInt8							isInIronSights;		// 0C6
	UInt8							shouldSneak;		// 0C7
	UInt32							movementFlagsToClear;	// 0C8
	float							actorHeight;		// 0CC
	float							eyeHeight;			// 0D0
	UInt8							isCharacter;		// 0D4
	UInt8							isMobile;			// 0D5
	UInt8							pad0D6[2];			// 0D6
	float							walkSpeedMult;		// 0D8
	float							runSpeedMult;		// 0DC
	float							flt0E0;				// 0E0
	UInt32							unk0E4;				// 0E4
	NiObject						*object0E8;			// 0E8
	NiObject						*object0EC;			// 0EC
	TESObjectWEAP					*weapon;			// 0F0
	TESCombatStyle					*combatStyle;		// 0F4
	UInt32							unk0F8[11];			// 0F8
	UInt8							canPowerAttack;		// 124
	bool							stopCombat;			// 125
	UInt8							stoppingCombat;		// 126
	UInt8							byte127;			// 127
	NiVector3						pos128;				// 128
	NiVector3						pos134;				// 134
	UInt32							flags140;			// 140
	UInt32							unk144;				// 144
	float							flt148;				// 148
	UInt32							unk14C[15];			// 14C
	SInt32							excludedActionsMask;// 188	JIP only!
};
static_assert(sizeof(CombatController) == 0x18C);

struct Decal
{
	enum Type
	{
		kDecalType_Simple =		1,
		kDecalType_Skinned =	2,
		kDecalType_Other =		4
	};

	NiPoint3		worldPos;			// 00
	NiPoint3		rotation;			// 0C
	NiPoint3		point18;			// 18
	Actor			*actor;				// 24
	BSFadeNode		*node;				// 28
	UInt32			unk2C;				// 2C
	BGSTextureSet	*textureSet;		// 30
	SInt32			index;				// 34
	float			width;				// 38
	float			height;				// 3C
	float			depth;				// 40
	float			rng44;				// 44
	TESObjectCELL	*parentCell;		// 48
	float			parallaxScale;		// 4C
	NiNode			*skinnedDecal;		// 50
	float			specular;			// 54
	float			epsilon;			// 58
	float			placementRadius;	// 5C
	NiColor			vertexColor;		// 60
	UInt32			hitLocationFlags;	// 6C
	UInt8			whichUVQuadrant;	// 70
	UInt8			byte71;				// 71
	UInt8			byte72;				// 72
	UInt8			isParallax;			// 73
	UInt8			isAlphaTest;		// 74
	UInt8			alphaBlend;			// 75
	UInt8			parallaxPasses;		// 76
	UInt8			modelSpace;			// 77
	UInt8			forceFade;			// 78
	UInt8			twoSided;			// 79
	UInt8			pad7A[2];			// 7A
};
static_assert(sizeof(Decal) == 0x7C);

class BSTempEffectSimpleDecal;
class BSTempEffectParticle;

struct DecalManager
{
	struct List14Item
	{
		UInt32					sevrDecalCount;
		UInt8					byte04[4];
		BGSImpactData			*impactData;
		BSTempEffectParticle	*effectParticle;
	};

	NiObject						*object00;		// 00
	UInt8							byte04;			// 04
	UInt8							pad05[3];		// 05
	DList<BSTempEffectSimpleDecal>	list08;			// 08
	DList<List14Item>				list14;			// 14
	BSShaderAccumulator				*shaderAccum;	// 20
	NiCamera						*camera;		// 24

	__forceinline static DecalManager *GetSingleton() {return *(DecalManager**)0x11C57F8;}

	__forceinline void AddGeometryDecal(Decal *decal, Decal::Type decalType, bool ignoreDistToPlayer) {ThisCall(0x4A10D0, this, decal, decalType, ignoreDistToPlayer);}
};