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
		kFlag_IsSneakAttack =			0x400,
		kFlag_ArmorPenetrated =			0x80000000	// JIP only
	};

	Actor				*source;		// 00
	Actor				*target;		// 04
	union								// 08
	{
		Projectile		*projectile;
		Explosion		*explosion;
	};
	UInt32				weaponAV;		// 0C
	SInt32				hitLocation;	// 10
	float				healthDmg;		// 14
	float				wpnBaseDmg;		// 18	Skill and weapon condition modifiers included
	float				fatigueDmg;		// 1C
	float				limbDmg;		// 20
	float				blockDTMod;		// 24
	float				armorDmg;		// 28
	float				flt2C;			// 2C
	TESObjectWEAP		*weapon;		// 30
	float				healthPerc;		// 34
	NiVector3			impactPos;		// 38
	NiVector3			impactAngle;	// 44
	UInt32				unk50;			// 50
	void				*ptr54;			// 54
	UInt32				flags;			// 58
	float				dmgMult;		// 5C
	SInt32				unk60;			// 60	Unused; rigged by CopyHitDataHook to store hitLocation
};

struct ProjectileData;

// 30
class BaseProcess
{
public:
	struct Data2C
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
		SInt32	unk30;					// 30
		SInt32	unk34;					// 34
		float	walkSpeed;				// 38
		float	runSpeed;				// 3C
		UInt8	hasNoCrippledLegs;		// 40
		UInt8	pad41[3];				// 41
		UInt32	flags;					// 44
	};

	virtual void	Destroy(bool deFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);
	virtual void	Unk_15(void);
	virtual void	Unk_16(void);
	virtual void	Unk_17(void);
	virtual void	Unk_18(void);
	virtual void	Unk_19(void);
	virtual void	Unk_1A(void);
	virtual void	Unk_1B(void);
	virtual void	Unk_1C(void);
	virtual void	Unk_1D(void);
	virtual void	Unk_1E(void);
	virtual void	Unk_1F(void);
	virtual void	Unk_20(void);
	virtual void	Unk_21(void);
	virtual void	Unk_22(void);
	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual DetectionEvent	*GetDetectionEvent(Actor *actor);	// arg not used
	virtual void	CreateDetectionEvent(Actor *actor, float posX, float posY, float posZ, UInt32 soundLevel, UInt32 eventType, TESObjectREFR *locationRef);	// actor & eventType unused
	virtual void	RemoveDetectionEvent();
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual bool	GetHasCaughtPlayerPickpocketting();
	virtual void	SetHasCaughtPlayerPickpocketting(bool doSet);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual TESForm *GetLowProcess40();
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);
	virtual ExtraContainerChanges::EntryData *GetWeaponInfo();
	virtual ExtraContainerChanges::EntryData *GetAmmoInfo();
	virtual void	Unk_54(void);
	virtual void	Unk_55(void);
	virtual void	Unk_56(void);
	virtual void	Unk_57(void);
	virtual void	Unk_58(void);
	virtual void	Unk_59(void);
	virtual void	SetAmmoInfo(ExtraContainerChanges::EntryData *_ammoInfo);
	virtual void	Unk_5B(void);
	virtual void	HandleQueuedEquipItems(Actor *actor);
	virtual void	Unk_5D(void);	// Called by 5E with count itemExtraList item
	virtual void	QueueEquipItem(Actor *actor, bool doEquip, TESForm *item, UInt32 count, ExtraDataList *xDataList, bool applyEnchantment, bool noUnequip, UInt8 arg8, UInt8 arg9, bool playSound);
	virtual void	Unk_5F(void);
	virtual void	Unk_60(void);
	virtual NiNode	*GetProjectileNode();
	virtual void	SetProjectileNode(NiNode *node);
	virtual void	Unk_63(void);
	virtual NiNode	*GetWeaponNode(ValidBip01Names *vbp01Names);
	virtual void	Unk_65(void);
	virtual void	Unk_66(void);
	virtual void	Unk_67(void);
	virtual NiNode	*GetWeaponNode2(UInt32 arg);
	virtual void	Unk_69(void);
	virtual bool	IsUsingOneHandGrenade();
	virtual bool	IsUsingOneHandMine();
	virtual bool	IsUsingOneHandThrownMineOrGrenade();
	virtual bool	IsUsingOneHandThrown();
	virtual AnimData	*GetAnimData();
	virtual void	Unk_6F(void);
	virtual void	Unk_70(void);
	virtual void	Unk_71(void);
	virtual void	Unk_72(void);
	virtual void	Unk_73(bool weaponOut, ValidBip01Names *validBip01Names, AnimData *pAnimData, Actor *actor);
	virtual void	Unk_74(void);
	virtual void	Unk_75(void);
	virtual void	Unk_76(Actor *actor);
	virtual void	Unk_77(void);
	virtual void	Unk_78(void);
	virtual void	Unk_79(void);
	virtual void	Unk_7A(void);
	virtual void	Unk_7B(void);
	virtual void	Unk_7C(void);
	virtual void	Unk_7D(void);
	virtual void	Unk_7E(void);
	virtual void	Unk_7F(void);
	virtual void	Unk_80(void);
	virtual void	Unk_81(void);
	virtual void	Unk_82(void);
	virtual TESPackage	*GetInterruptPackage();
	virtual void	SetInterruptPackage(TESPackage *package, Actor *onActor);
	virtual void	StopInterruptPackage();
	virtual void	Unk_86(void);	// 086 - SetInterruptPackageTargetRef
	virtual void	Unk_87(void);	// 087 - SetInterruptPackageTargetRef
	virtual void	Unk_88(void);	// 088 - IncreaseInterruptPackageUnk00C
	virtual void	Unk_89(void);
	virtual void	Unk_8A(void);
	virtual TESPackage	*GetStablePackage();
	virtual void	SetStablePackage(TESPackage *package, Actor *onActor);
	virtual void	StopStablePackage();
	virtual void	Unk_8E(void);
	virtual void	Unk_8F(void);
	virtual void	Unk_90(void);
	virtual void	Unk_91(void);
	virtual void	Unk_92(void);	// Only HighProcess, get Unk0454
	virtual void	Unk_93(void);
	virtual void	Unk_94(void);
	virtual void	Unk_95(void);
	virtual void	Unk_96(void);
	virtual void	Unk_97(void);
	virtual void	Unk_98(void);
	virtual void	Unk_99(void);
	virtual void	Unk_9A(void);
	virtual void	Unk_9B(void);
	virtual void	Unk_9C(void);
	virtual TESPackageData	*GetPackageData();
	virtual void	Unk_9E(void);
	virtual TESPackage	*GetCurrentPackage();
	virtual UInt32	GetPackageInfo0C();
	virtual void	Unk_A1();
	virtual void	Unk_A2();
	virtual bhkCharacterController	*GetCharacterController();
	virtual void	SetCharacterController(bhkCharacterController *charCtrl);
	virtual void	Unk_A5();
	virtual void	Unk_A6();
	virtual void	Unk_A7();
	virtual void	Unk_A8();
	virtual void	Unk_A9();
	virtual void	Unk_AA();
	virtual void	Unk_AB();
	virtual void	Unk_AC();
	virtual void	Unk_AD();
	virtual void	Unk_AE();
	virtual void	Unk_AF();
	virtual void	Unk_B0();
	virtual void	Unk_B1();
	virtual void	Unk_B2();
	virtual void	Unk_B3();
	virtual void	Unk_B4();
	virtual void	Unk_B5();
	virtual void	Unk_B6();
	virtual void	Unk_B7();
	virtual void	Unk_B8();
	virtual void	Unk_B9();
	virtual void	Unk_BA();
	virtual void	Unk_BB();
	virtual void	Unk_BC();
	virtual void	Unk_BD();
	virtual void	Unk_BE();
	virtual void	SetDiveBreath(float breath);
	virtual float	GetDiveBreath();
	virtual void	Unk_C1();
	virtual void	Unk_C2();
	virtual void	Unk_C3();
	virtual void	Unk_C4();
	virtual void	Unk_C5();
	virtual void	Unk_C6();
	virtual bool	GetAlerted();
	virtual void	SetAlert(bool alert);
	virtual void	Unk_C9();
	virtual void	Unk_CA();
	virtual void	Unk_CB();
	virtual void	Unk_CC();
	virtual void	Unk_CD();
	virtual void	Unk_CE();
	virtual void	Unk_CF();
	virtual void	Unk_D0();
	virtual void	Unk_D1();
	virtual void	Unk_D2();
	virtual void	Unk_D3();
	virtual void	Unk_D4();
	virtual void	Unk_D5();
	virtual void	Unk_D6();
	virtual void	Unk_D7();
	virtual void	Unk_D8();
	virtual void	Unk_D9();
	virtual void	Unk_DA();
	virtual void	Unk_DB();
	virtual void	Unk_DC();
	virtual void	Unk_DD();
	virtual void	Unk_DE();
	virtual void	Unk_DF();
	virtual void	Unk_E0();
	virtual void	Unk_E1();
	virtual UInt8	GetPlantedExplosive();
	virtual TESIdleForm	*GetLastPlayedIdle();
	virtual void	SetLastPlayedIdle(TESIdleForm *idleForm);
	virtual void	StopIdle();
	virtual void	Unk_E6();
	virtual void	Unk_E7();	// float GetActorValue
	virtual void	Unk_E8();
	virtual void	Unk_E9();
	virtual void	Unk_EA();
	virtual void	Unk_EB();
	virtual void	Unk_EC();
	virtual void	Unk_ED();
	virtual void	Unk_EE();
	virtual void	Unk_EF();
	virtual void	Unk_F0();
	virtual void	Unk_F1();
	virtual void	Unk_F2();
	virtual void	Unk_F3();
	virtual void	Unk_F4();
	virtual void	Unk_F5();
	virtual void	Unk_F6();
	virtual void	Unk_F7();
	virtual void	Unk_F8();
	virtual SInt16	GetCurrentAction();
	virtual BSAnimGroupSequence	*GetCurrentSequence();
	virtual void	SetCurrentActionAndSequence(SInt16 action, BSAnimGroupSequence *sequence);
	virtual void	Unk_FC();
	virtual void	Unk_FD();
	virtual bool	IsReadyForAnim();
	virtual void	Unk_FF();
	virtual void	SetIsAiming(bool aiming);
	virtual bool	GetIsAiming();
	virtual void	Unk_102();
	virtual SInt32	GetKnockedState();
	virtual void	SetKnockedState(char state);
	virtual void	Unk_105();
	virtual void	PushActorAway(Actor *pushed, float posX, float posY, float posZ, float force);
	virtual void	Unk_107(Actor *actor);
	virtual void	Unk_108();
	virtual void	SetNthAnimSequenceWeight(UInt32 index, BSAnimGroupSequence *animSeq);
	virtual void	Unk_10A();
	virtual void	Unk_10B();
	virtual void	Unk_10C();
	virtual void	Unk_10D();
	virtual void	Unk_10E();
	virtual void	Unk_10F();
	virtual void	Unk_110();
	virtual void	Unk_111();
	virtual void	Unk_112();
	virtual void	Unk_113();
	virtual void	Unk_114();
	virtual bool	IsWeaponOut();
	virtual void	SetWeaponOut(Actor *actor, bool weaponOut);
	virtual void	Unk_117();
	virtual void	Unk_118();
	virtual void	Unk_119(Actor *actor);
	virtual void	Unk_11A(UInt32 unk);
	virtual void	Unk_11B();
	virtual void	Unk_11C();
	virtual bool	Unk_11D(UInt32 arg);
	virtual void	Unk_11E();
	virtual void	Unk_11F();
	virtual void	Unk_120();
	virtual void	Unk_121();
	virtual void	Unk_122();
	virtual void	Unk_123();
	virtual void	Unk_124();
	virtual void	Unk_125();
	virtual void	Unk_126();
	virtual void	Unk_127();
	virtual void	Unk_128();
	virtual void	Unk_129();
	virtual void	Unk_12A();
	virtual void	Unk_12B();
	virtual void	Unk_12C();
	virtual void	Unk_12D();
	virtual bool	IsOnDialoguePackage(Actor *actor);
	virtual UInt32	GetSitSleepState();
	virtual void	Unk_130();
	virtual void	Unk_131();
	virtual TESObjectREFR	*GetCurrentFurnitureRef();
	virtual void	Unk_133();
	virtual void	Unk_134();
	virtual void	Unk_135();
	virtual void	Unk_136();
	virtual void	Unk_137();
	virtual void	Unk_138();
	virtual void	Unk_139();
	virtual void	Unk_13A();
	virtual void	Unk_13B();
	virtual void	Unk_13C();
	virtual void	Unk_13D();
	virtual void	Unk_13E();
	virtual void	Unk_13F(UInt32 unk);
	virtual void	Unk_140();
	virtual DetectionData *GetDetectionData(Actor *target, UInt32 detecting);
	virtual void	Unk_142();
	virtual void	Unk_143();
	virtual void	Unk_144();
	virtual void	Unk_145();
	virtual void	Unk_146();
	virtual void	Unk_147();
	virtual void	Unk_148();
	virtual void	Unk_149();
	virtual void	Unk_14A();
	virtual void	Unk_14B();
	virtual void	Unk_14C();
	virtual void	Unk_14D();
	virtual void	Unk_14E();
	virtual void	Unk_14F();
	virtual void	Unk_150();
	virtual void	Unk_151();
	virtual void	Unk_152();
	virtual void	Unk_153();
	virtual void	Unk_154();
	virtual void	Unk_155();
	virtual void	Unk_156();
	virtual void	Unk_157();
	virtual void	Unk_158();
	virtual void	Unk_159();
	virtual void	Unk_15A();
	virtual void	Unk_15B();
	virtual void	Unk_15C();
	virtual void	Unk_15D();
	virtual void	Unk_15E();
	virtual void	Unk_15F();
	virtual void	Unk_160();
	virtual void	Unk_161();
	virtual void	Unk_162();
	virtual void	Unk_163();
	virtual void	Unk_164();
	virtual void	Unk_165();
	virtual void	Unk_166();
	virtual void	Unk_167();
	virtual void	Unk_168();
	virtual void	Unk_169();
	virtual void	Unk_16A();
	virtual float	GetActorAlpha();
	virtual void	SetActorAlpha(float alpha);
	virtual void	Unk_16D();
	virtual void	Unk_16E();
	virtual void	Unk_16F();
	virtual void	Unk_170();
	virtual void	Unk_171();
	virtual void	Unk_172();
	virtual void	Unk_173();
	virtual void	Unk_174();
	virtual void	Unk_175();
	virtual void	Unk_176();
	virtual void	Unk_177();
	virtual void	Unk_178();
	virtual void	Unk_179();
	virtual void	Unk_17A();
	virtual void	Unk_17B();
	virtual BSBound	*GetBoundingBox();
	virtual void	SetBoundingBox(BSBound *bounds);
	virtual void	Unk_17E();
	virtual void	Unk_17F();
	virtual void	Unk_180();
	virtual void	Unk_181();
	virtual void	Unk_182();
	virtual void	Unk_183();
	virtual void	Unk_184();
	virtual void	SetQueuedIdleFlags(UInt32 flags);
	virtual UInt32	GetQueuedIdleFlags();
	virtual void	Unk_187();
	virtual void	Unk_188();
	virtual void	Unk_189();
	virtual void	Unk_18A(Actor *actor);
	virtual void	Unk_18B();
	virtual void	Unk_18C();
	virtual void	Unk_18D();
	virtual void	Unk_18E();
	virtual void	Unk_18F();
	virtual void	Unk_190();
	virtual void	Unk_191();
	virtual void	Unk_192(UInt8 unk);
	virtual void	Unk_193();
	virtual void	Unk_194();
	virtual void	Unk_195();
	virtual void	Unk_196();
	virtual void	Unk_197();
	virtual void	Unk_198();
	virtual void	Unk_199();
	virtual void	Unk_19A();
	virtual void	Unk_19B();
	virtual void	Unk_19C();
	virtual void	Unk_19D();
	virtual void	Unk_19E();
	virtual void	Unk_19F();
	virtual void	Unk_1A0();
	virtual void	Unk_1A1();
	virtual void	Unk_1A2();
	virtual void	Unk_1A3();
	virtual void	Unk_1A4();
	virtual void	Unk_1A5();
	virtual void	Unk_1A6();
	virtual void	Unk_1A7();
	virtual void	Unk_1A8();
	virtual void	Unk_1A9();
	virtual void	Unk_1AA();
	virtual void	Unk_1AB();
	virtual void	Unk_1AC();
	virtual void	Unk_1AD();
	virtual ProjectileData	*GetProjectileData();
	virtual void	Unk_1AF();
	virtual void	Unk_1B0();
	virtual void	Unk_1B1();
	virtual void	Unk_1B2();
	virtual void	Unk_1B3();
	virtual void	Unk_1B4();
	virtual void	Unk_1B5();
	virtual void	Unk_1B6();
	virtual void	Unk_1B7();
	virtual void	Unk_1B8();
	virtual void	Unk_1B9();
	virtual void	Unk_1BA();
	virtual void	Unk_1BB();
	virtual void	Unk_1BC();
	virtual void	Unk_1BD();
	virtual void	Unk_1BE();
	virtual void	Unk_1BF();
	virtual void	Unk_1C0();
	virtual void	Unk_1C1();
	virtual void	Unk_1C2();
	virtual void	Unk_1C3();
	virtual void	Unk_1C4();
	virtual void	Unk_1C5();
	virtual TESIdleForm	*GetIdleForm350();
	virtual void	SetIdleForm350(TESIdleForm *idleForm);
	virtual void	Unk_1C8();
	virtual void	Unk_1C9();
	virtual void	Unk_1CA();
	virtual void	Unk_1CB();
	virtual void	Unk_1CC();
	virtual float	GetLightAmount();
	virtual void	SetLightAmount(float lightAmount);
	virtual void	Unk_1CF();
	virtual void	Unk_1D0();
	virtual void	Unk_1D1();
	virtual void	Unk_1D2();
	virtual void	Unk_1D3();
	virtual void	Unk_1D4();
	virtual void	Unk_1D5();
	virtual void	Unk_1D6();
	virtual void	Unk_1D7();
	virtual void	SetWaterRadsSec(float radsSec);
	virtual float	GetWaterRadsSec();
	virtual void	Unk_1DA();
	virtual float	GetRadsSec();
	virtual ActorHitData *GetHitData();
	virtual void	CopyHitData(ActorHitData *hitData);
	virtual void	ResetHitData();
	virtual ActorHitData *GetHitData254();
	virtual void	CopyHitData254(ActorHitData *hitData);
	virtual void	ResetHitData254();
	virtual void	Unk_1E2();
	virtual void	Unk_1E3();
	virtual void	Unk_1E4();
	virtual void	Unk_1E5();
	virtual void	Unk_1E6();
	virtual void	Unk_1E7();
	virtual void	Unk_1E8();
	virtual void	Unk_1E9();
	virtual void	Unk_1EA();
	virtual void	Unk_1EB();
	virtual void	Unk_1EC();
	virtual void	Unk_1ED();	// Leads to Last Target

	PackageInfo		currentPackage;	// 04
	float			unk1C;			// 1C	not initialized, only by descendant!
	float			unk20;			// 20	not initialized, only by descendant to -1.0! flt020 gets set to GameHour minus one on package evaluation
	UInt32			unk24;			// 24	not initialized, only by descendant!
	UInt32			processLevel;	// 28	not initialized, only by descendant to 3 for Low, 2 for MidlleLow, 1 MiddleHighProcess and 0 for HigProcess
	Data2C			*unk2C;			// 2C
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

	virtual void	Unk_1EE();
	virtual void	Unk_1EF();
	virtual void	Unk_1F0();
	virtual void	Unk_1F1();
	virtual void	Unk_1F2();
	virtual void	Unk_1F3();
	virtual void	Unk_1F4();
	virtual void	Unk_1F5();
	virtual void	Unk_1F6();
	virtual void	Unk_1F7();
	virtual void	Unk_1F8();
	virtual void	Unk_1F9();
	virtual void	Unk_1FA();
	virtual void	Unk_1FB();
	virtual void	Unk_1FC();
	virtual void	Unk_1FD();
	virtual void	Unk_1FE();
	virtual void	Unk_1FF();
	virtual void	Unk_200();
	virtual void	Unk_201();
	virtual void	Unk_202();
	virtual void	Unk_203();
	virtual void	Unk_204();
	virtual void	Unk_205();
	virtual void	Unk_206();

	UInt8				byte30;		// 8 = IsAlerted
	UInt8				pad31[3];
	UInt32				unk34;
	FloatPair			unk38;
	TESForm				*unk40;		// Used when picking idle anims.
	UInt32				unk44;		// not initialized!	refr, expected actor, might be CombatTarget
	UInt32				unk48;
	UInt32				unk4C;
	UInt32				unk50;
	UInt32				unk54;		// not initialized!
	UInt32				unk58;
	UInt32				unk5C;
	tList<UInt32>		unk60;		// List
	UInt32				unk68;
	UInt32				unk6C;
	tList<TESForm>		unk70;
	tList<UInt32>		unk78;
	tList<UInt32>		unk80;
	UInt32				unk88;
	UInt32				unk8C;
	UInt32				unk90;
	UInt32				unk94;
	ActorValueModifiers	damageModifiers;
	float				gameDayDied;
	UInt32				unkAC;		// not initialized!
	UInt32				unkB0;		// not initialized!
};

// C8
class MiddleLowProcess : public LowProcess
{
public:
	virtual void		Unk_207();

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
	NiControllerManager				*unk0D8;			// 0D8
	NiTPointerMap<AnimSequenceBase>	*sequenceBaseMap;	// 0DC
	BSAnimGroupSequence				*animSequence[8];	// 0E0
	BSAnimGroupSequence				*animSeq100;		// 100
	tList<KFModel>					loadingAnims;		// 104
	float							movementSpeedMult;	// 10C
	float							rateOfFire;			// 110
	float							turboSpeedMult;		// 114
	float							weaponReloadSpeed;	// 118
	float							equipSpeed;			// 11C
	UInt8							byte120;			// 120
	UInt8							byte121;			// 121
	UInt16							word122;			// 122
	AnimIdle						*idleAnim;			// 124
	AnimIdle						*queuedIdleAnim;	// 128
	NiObject						*object12C;			// 12C
	NiObject						*object130;			// 130
	tList<void>						list134;			// 134
};
STATIC_ASSERT(sizeof(AnimData) == 0x13C);

class QueuedFile;
class BSFaceGenAnimationData;

// 25C
class MiddleHighProcess : public MiddleLowProcess
{
public:
	virtual void	SetAnimation(UInt32 newAnimation);
	virtual void	Unk_209();
	virtual void	Unk_20A();
	virtual void	Unk_20B();
	virtual void	Unk_20C();
	virtual void	Unk_20D();
	virtual void	Unk_20E();
	virtual void	Unk_20F();
	virtual void	Unk_210();
	virtual void	Unk_211();
	virtual void	Unk_212();
	virtual void	Unk_213();
	virtual void	Unk_214();
	virtual void	Unk_215();
	virtual void	Unk_216();
	virtual void	Unk_217();
	virtual void	Unk_218();
	virtual void	Unk_219();
	virtual void	Unk_21A();
	virtual void	Unk_21B();

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

	tList<TESForm>						unk0C8;				// 0C8
	tList<UInt32>						unk0D0;				// 0D0
	UInt32								unk0D8[3];			// 0D8
	PackageInfo							interruptPackage;	// 0E4
	UInt8								unk0FC[12];			// 0FC	Saved as one, might be Pos/Rot given size
	UInt32								unk108;				// 108
	TESIdleForm							*LastPlayedIdle;	// 10C
	UInt32								unk110;				// 110  EntryData, also handled as part of weapon code. AmmoInfo.
	ExtraContainerChanges::EntryData	*weaponInfo;		// 114
	ExtraContainerChanges::EntryData	*ammoInfo;			// 118
	QueuedFile							*unk11C;			// 11C
	UInt8								byt120;				// 120
	UInt8								byt121;				// 121
	UInt8								byt122;				// 122
	UInt8								fil123;				// 123
	UInt8								usingOneHandGrenade;// 124
	UInt8								usingOneHandMine;	// 125
	UInt8								usingOneHandThrown;	// 126
	UInt8								byte127;			// 127
	UInt32								unk128;				// 128 Gets copied over during TESNPC.CopyFromBase
	NiNode								*weaponNode;		// 12C
	NiNode								*projectileNode;	// 130
	UInt8								byt134;				// 134
	bool								isWeaponOut;		// 135
	UInt8								byt136;				// 136
	UInt8								byt137;				// 137
	bhkCharacterController				*charCtrl;			// 138
	UInt8								knockedState;		// 13C
	UInt8								sitSleepState;		// 13D
	UInt8								unk13E[2];			// 13E
	TESObjectREFR						*usedFurniture;		// 140
	UInt8								byte144;			// 144
	UInt8								unk145[3];			// 145
	UInt32								unk148[6];			// 148
	MagicItem							*magicItem160;		// 160
	UInt32								unk164[3];			// 164
	float								actorAlpha;			// 170
	UInt32								unk174;				// 174
	BSFaceGenAnimationData				*unk178;			// 178
	UInt8								byte17C;			// 17C
	UInt8								byte17D;			// 17D
	UInt8								byte17E;			// 17E
	UInt8								byte17F;			// 17F
	UInt32								unk180;				// 180
	UInt32								unk184;				// 184
	UInt8								hasCaughtPCPickpocketting;	// 188
	UInt8								byte189;			// 189
	UInt8								byte18A;			// 18A
	UInt8								byte18B;			// 18B
	UInt8								byte18C;			// 18C
	UInt8								byte18D[3];			// 18D
	UInt32								unk190[4];			// 190
	void								*ptr1A0;			// 1A0
	UInt32								unk1A4[2];			// 1A4
	tList<void>							*list1AC;			// 1AC
	tList<void>							list1B0;			// 1B0
	tList<void>							*list1B8;			// 1B8
	MagicTarget							*magicTarget1BC;	// 1BC
	AnimData							*animData;			// 1C0
	BSAnimGroupSequence					*animSequence[3];	// 1C4
	UInt32								unk1D0[3];			// 1D0
	NiNode								*limbNodes[15];		// 1DC
	NiNode								*unk218;			// 218
	NiNode								*unk21C;			// 21C
	BSShaderProperty					*shaderProp220;		// 220
	BSBound								*boundingBox;		// 224
	bool								isAiming;			// 228
	UInt8								pad229[3];			// 229
	UInt32								unk22C[2];			// 22C
	float								radsSec234;			// 234
	float								rads238;			// 238
	float								waterRadsSec;		// 23C
	ActorHitData						*hitData240;		// 240
	UInt32								unk244;				// 244
	BSFaceGenNiNode						*unk248;			// 248
	BSFaceGenNiNode						*unk24C;			// 24C
	NiTriShape							*unk250;			// 250
	ActorHitData						*hitData254;		// 254
	UInt32								unk258;				// 258
};
STATIC_ASSERT(sizeof(MiddleHighProcess) == 0x25C);

struct DetectionData
{
	Actor		*actor;			// 00
	UInt8		detectionLevel;	// 04
	UInt8		byte05;			// 05
	UInt8		byte06;			// 06
	UInt8		byte07;			// 07
	SInt32		detectionValue;	// 08
	UInt32		unk0C;			// 0C
	UInt32		unk10;			// 10
	UInt32		unk14;			// 14
	UInt32		unk18;			// 18
	UInt8		byte1C;			// 1C
	UInt8		byte1D;			// 1D
	bool		inLOS;			// 1E
	UInt8		byte1F;			// 1F
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
struct ProjectileData
{
	UInt8				byte00;			// 00
	UInt8				byte01;			// 01
	UInt8				byte02;			// 02
	UInt8				byte03;			// 03
	float				flt04;			// 04
	float				flashDuration;	// 08
	NiNode				*muzzleFlash;	// 0C
	NiPointLight		*flashLight;	// 10
	BGSProjectile		*baseProj;		// 14
	TESObjectWEAP		*sourceWeap;	// 18
	TESObjectREFR		*refr;			// 1C
};

class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;

// 46C
class HighProcess : public MiddleHighProcess
{
public:
	tList<DetectionData>				*detectedActors;	// 25C
	tList<DetectionData>				*detectingActors;	// 260
	void								*ptr264;			// 264
	void								*ptr268;			// 268
	void								*ptr26C;			// 26C
	UInt32								unk270;				// 270
	tList<CombatTarget>					list274;			// 274
	tList<void>							list27C;			// 27C
	tList<void>							list284;			// 284
	tList<void>							list28C;			// 28C
	float								flt294;				// 294
	float								flt298;				// 298
	UInt32								unk29C;				// 29C
	float								flt2A0;				// 2A0
	UInt32								unk2A4;				// 2A4
	float								flt2A8;				// 2A8
	UInt32								unk2AC;				// 2AC
	float								actorAlpha2;		// 2B0
	float								flt2B4;				// 2B4
	float								flt2B8;				// 2B8
	float								flt2BC;				// 2BC
	UInt16								word2C0;			// 2C0
	UInt8								byte2C2;			// 2C2
	UInt8								byte2C3;			// 2C3
	UInt8								byte2C4;			// 2C4
	UInt8								byte2C5;			// 2C5
	UInt8								byte2C6;			// 2C6
	UInt8								byte2C7;			// 2C7
	float								flt2C8;				// 2C8
	UInt32								unk2CC;				// 2CC
	float								flt2D0;				// 2D0
	float								flt2D4;				// 2D4
	float								flt2D8;				// 2D8
	UInt32								unk2DC;				// 2DC
	float								flt2E0;				// 2E0
	NiBSBoneLODController				*ptr2E4;			// 2E4
	UInt32								unk2E8;				// 2E8
	SInt16								currentAction;		// 2EC
	UInt8								pad2EE[2];			// 2EE
	BSAnimGroupSequence					*currentSequence;	// 2F0
	UInt8								forceFireWeapon;	// 2F4
	UInt8								pad2F5[3];			// 2F5
	float								flt2F8;				// 2F8
	UInt32								unk2FC[5];			// 2FC
	float								flt310;				// 310
	UInt32								unk314[6];			// 314
	UInt8								byte32C;			// 32C
	UInt8								byte32D;			// 32D
	UInt8								byte32E;			// 32E
	UInt8								byte32F;			// 32F
	float								flt330;				// 330
	float								flt334;				// 334
	float								flt338;				// 338
	float								diveBreath;			// 33C
	UInt32								unk340;				// 340
	float								flt344;				// 344
	UInt32								unk348;				// 348
	float								flt34C;				// 34C
	TESIdleForm							*idleForm350;		// 350
	UInt32								unk354[4];			// 354
	NiBSplineCompTransformInterpolator	**ptr364;			// 364
	UInt32								unk368[4];			// 368
	float								flt378;				// 378
	float								flt37C;				// 37C
	UInt32								unk380;				// 380
	float								flt384;				// 384
	float								flt388;				// 388
	tList<void>							list38C;			// 38C
	tList<void>							list394;			// 394
	UInt32								unk39C;				// 39C
	UInt32								unk3A0;				// 3A0
	float								flt3A4;				// 3A4
	UInt32								unk3A8[5];			// 3A8
	float								flt3BC;				// 3BC
	float								flt3C0;				// 3C0
	float								lightAmount;		// 3C4
	float								flt3C8;				// 3C8
	UInt32								unk3CC;				// 3CC
	UInt32								unk3D0;				// 3D0
	ProjectileData						*projData;			// 3D4
	UInt32								unk3D8;				// 3D8
	DetectionEvent						*detectionEvent;	// 3DC
	UInt32								unk3E0;				// 3E0
	UInt32								unk3E4;				// 3E4
	UInt32								fadeType;			// 3E8
	float								delayTime;			// 3EC
	UInt32								unk3F0;				// 3F0
	UInt32								unk3F4;				// 3F4
	UInt32								unk3F8[3];			// 3F8
	Actor								*combatTarget;		// 404
	UInt32								unk408[4];			// 408
	float								flt418;				// 418
	TESObjectREFR						*packageTarget;		// 41C
	UInt32								unk420;				// 420
	UInt32								queuedIdleFlags;	// 424
	UInt32								unk428;				// 428
	float								flt42C;				// 42C
	UInt32								unk430;				// 430
	bhkShapePhantom						*ptr434;			// 434
	UInt32								unk438;				// 438
	float								unk43C;				// 43C
	float								radsSec440;			// 440
	UInt8								plantedExplosive;	// 444
	UInt8								pad445[3];			// 445
	float								flt448;				// 448
	UInt32								unk44C;				// 44C
	float								flt450;				// 450
	UInt32								unk454[6];			// 454
};
STATIC_ASSERT(sizeof(HighProcess) == 0x46C);

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
	tList<void>								list068;			// 068
	tList<void>								list070;			// 070
	tList<void>								list078;			// 078
	tList<Actor>							highActors;			// 080
	UInt32									unk088[50];			// 088
	UInt32									unk150;				// 150
	float									flt154;				// 154
	float									flt158;				// 158
	UInt8									byte15C;			// 15C
	UInt8									byte15D;			// 15D
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
	UInt32									unk103B0;			// 103B0
	float									flt103B4;			// 103B4
	UInt32									unk103B8;			// 103B8
	float									removeDeadActorsTime;	// 103BC
	UInt32									unk103C0[3];		// 103C0

	__forceinline int GetTotalDetectionValue(Actor *actor, bool arg2 = false)
	{
		return ThisCall<int>(0x973710, this, actor, arg2);
	}
};
STATIC_ASSERT(sizeof(ProcessManager) == 0x103CC);

extern ProcessManager *g_processManager;

class CombatGoal;
struct CombatSearchLocation;
struct CombatSearchDoor;
struct CombatGroupCluster;
class PathingCoverLocation;
struct UnreachableLocation;
struct UnreachableCoverLocation;

// 0C
struct CombatActionInfo
{
	char		*name;	// 00
	float		cost;	// 04
	UInt32		unk08;	// 08
};

enum CombatActions
{
	COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE,
	COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE_F,
	COMBAT_ACTION_ATTACK_RANGED,
	COMBAT_ACTION_ATTACK_RANGED_FROM_COVER,
	COMBAT_ACTION_ATTACK_GRENADE,
	COMBAT_ACTION_ATTACK_GRENADE_FLUSH_TARG,
	COMBAT_ACTION_ATTACK_GRENADE_FROM_COVER,
	COMBAT_ACTION_ATTACK_MELEE,
	COMBAT_ACTION_ATTACK_HAND_TO_HAND,
	COMBAT_ACTION_MOVE,
	COMBAT_ACTION_MOVE_AND_SWITCH_TO_MELEE,
	COMBAT_ACTION_MOVE_AND_ATTACK_RANGED,
	COMBAT_ACTION_MOVE_AND_ATTACK_RANGED_EX,
	COMBAT_ACTION_MOVE_AND_ATTACK_GRENADE,
	COMBAT_ACTION_DRAW_WEAPON,
	COMBAT_ACTION_SWITCH_WEAPON,
	COMBAT_ACTION_AVOID_THREAT,
	COMBAT_ACTION_SEARCH,
	COMBAT_ACTION_INVESTIGATE,
	COMBAT_ACTION_DODGE,
	COMBAT_ACTION_IGNORE_BLOCKED_TARGET,
	COMBAT_ACTION_FLEE,
	COMBAT_ACTION_ACTIVATE_COMBAT_ITEM,
	COMBAT_ACTION_USE_COMBAT_ITEM,
	COMBAT_ACTION_ACQUIRE_LINE_OF_SIGHT,
	COMBAT_ACTION_HIDE,
	COMBAT_ACTION_APPROACH_TARGET
};

// 2C
class CombatAction
{
public:
	virtual void	IsApplicable(void);
	virtual void	AddProcedureToController(CombatController *combatCtrl, void *a2);
	virtual void	Unk_02(void);
	virtual void	GetCost(CombatController *combatCtrl, int a2);

	tList<void>		list04;		// 04
	tList<void>		list0C;		// 0C
	tList<void>		list14;		// 14
	tList<void>		list1C;		// 1C
	UInt32			actionID;	// 24
	UInt32			unk28;		// 28
};
STATIC_ASSERT(sizeof(CombatAction) == 0x2C);

class CombatProcedure
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Update(void);
	virtual void	SetCombatController(CombatController *_combatCtrl);
	virtual void	Unk_03(void);
	virtual void	ClearTargettedRefIfEqualTo(TESObjectREFR *targettedRef);
	virtual void	DebugPrint(void);
	virtual void	Unk_06(NiVector3 *out, NiVector3 *in);
	virtual void	SetLastError_Disarmed(int unused);
	virtual void	Unk_08(int a1);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(int a1, int a2, int a3, int a4);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	SaveGame(int a1);
	virtual void	LoadGame(int a1);
	virtual void	LoadGame2(int a1);

	CombatController	*combatCtrl;	// 04
	UInt32				*status;		// 08
	char				*errorText;		// 0C
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

	UInt32									unk000[2];			// 000
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
	float									flt0C0;				// 0C0
	float									flt0C4;				// 0C4
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
STATIC_ASSERT(sizeof(CombatActors) == 0x15C);

// 188
class CombatController : public TESPackage
{
public:
	struct Unk09C
	{
		UInt32									unk000[4];	// 000
		TESObjectWEAP							*weapon1;	// 010
		TESObjectWEAP							*weapon2;	// 014
		TESObjectWEAP							*weapon3;	// 018
		TESObjectWEAP							*weapon4;	// 01C
		UInt32									unk020;		// 020
		BSSimpleArray<TESObjectWEAP>			arr024;		// 024
		UInt32									unk034[24];	// 034
		float									flt094;		// 094
		UInt32									unk098[11];	// 098
		void									*ptr0C4;	// 0C4
		UInt32									unk0C8[17];	// 0C8
		BSSimpleArray<PathingCoverLocation>		arr10C;		// 10C
		UInt32									unk11C[11];	// 11C
		BSSimpleArray<PathingCoverLocation>		arr148;		// 148
		UInt32									unk158[3];	// 158
		BSSimpleArray<UnreachableCoverLocation>	arr164;		// 164
		BSSimpleArray<UnreachableLocation>		arr174;		// 174
		UInt32									unk184[15];	// 184
		Actor									*actor1C0;	// 1C0
		CombatController						*cmbtCtrl;	// 1C4
		UInt32									unk1C8[25];	// 1C8
	};
	STATIC_ASSERT(sizeof(Unk09C) == 0x22C);

	CombatActors					*combatActors;		// 080
	CombatProcedure					*combatProcedure1;	// 084
	CombatProcedure					*combatProcedure2;	// 088
	BSSimpleArray<CombatProcedure>	combatProcedures;	// 08C
	Unk09C							*struct09C;			// 09C
	void							*ptr0A0;			// 0A0
	UInt32							unk0A4;				// 0A4
	CombatAction					*combatAction;		// 0A8
	CombatGoal						*combatGoal;		// 0AC
	UInt32							unk0B0;				// 0B0
	float							flt0B4[2];			// 0B4
	Actor							*packageOwner;		// 0BC
	Actor							*packageTarget;		// 0C0
	UInt32							unk0C4[2];			// 0C4
	float							flt0CC;				// 0CC
	float							flt0D0;				// 0D0
	UInt8							byte0D4;			// 0D4
	UInt8							byte0D5;			// 0D5
	UInt8							pad0D6[2];			// 0D6
	float							flt0D8;				// 0D8
	float							flt0DC;				// 0DC
	float							flt0E0;				// 0E0
	UInt32							unk0E4[3];			// 0E4
	TESObjectWEAP					*weapon;			// 0F0
	TESCombatStyle					*combatStyle;		// 0F4
	UInt32							unk0F8[11];			// 0F8
	UInt8							byte124;			// 124
	bool							stopCombat;			// 125
	UInt8							byte126;			// 126
	UInt8							byte127;			// 127
	UInt32							unk128[8];			// 128
	float							flt148;				// 148
	UInt32							unk14C[15];			// 14C
};
STATIC_ASSERT(sizeof(CombatController) == 0x188);