#pragma once

class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;
struct CombatTarget;
struct BGSSaveLoadFileEntry;
class Sky;
class BSTempNodeManager;
class CombatGoal;
struct CombatSearchLocation;
struct CombatSearchDoor;
struct CombatGroupCluster;
class PathingLocation;
class PathingCoverLocation;
struct UnreachableLocation;
struct UnreachableCoverLocation;
class BSAudioManagerThread;
class ImageSpaceModifierInstanceRB;
struct NavMeshClosedDoorInfo;
class BSWin32Audio;

// 34
class BGSPrimitive
{
public:
	virtual void	Destructor(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);

	UInt32			type;		// 04
	float			unk08[4];	// 08
	float			bounds[3];	// 18
	NiRefObject		*unk24;		// 24
	NiRefObject		*unk28;		// 28
	NiRefObject		*unk2C;		// 2C
	UInt32			unk30;		// 30
};

// 34
class BGSPrimitivePlane : public BGSPrimitive
{
public:
};

// 4C
class BGSPrimitiveBox : public BGSPrimitive
{
public:
	UInt32		unk34[6];	// 34
};

// 34
class BGSPrimitiveSphere : public BGSPrimitive
{
public:
};

// 18
class BSTempEffect : public NiObject
{
public:
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

	float			duration;	// 08
	TESObjectCELL	*cell;		// 0C
	float			unk10;		// 10
	UInt8			unk14;		// 14
	UInt8			pad15[3];	// 15
};

// 28
class MagicHitEffect : public BSTempEffect
{
public:
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);

	ActiveEffect	*activeEffect;	// 18
	TESObjectREFR	*target;		// 1C
	float			unk20;			// 20
	UInt8			flags;			// 24	1 - Stop
	UInt8			pad25[3];		// 25
};

// 6C
class MagicShaderHitEffect : public MagicHitEffect
{
public:
	UInt32									unk28[2];		// 28
	TESEffectShader							*effectShader;	// 30
	float									flt34;			// 34
	BSSimpleArray<ParticleShaderProperty>	shaderProps;	// 38
	NiNode									*shaderNode;	// 48
	UInt32									unk4C;			// 4C
	BSSimpleArray<NiAVObject>				objects;		// 50	Seen BSFadeNode
	float									flt60;			// 60
	float									flt64;			// 64
	NiProperty								*prop68;		// 68	Seen 0x10AE0C8
};
STATIC_ASSERT(sizeof(MagicShaderHitEffect) == 0x6C);

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

	virtual UInt32	GetProjectileType();
	virtual void	Unk_C2(void);
	virtual void	Unk_C3(void);
	virtual void	Unk_C4(void);
	virtual bool	ProcessImpact();
	virtual bool	IsProximityTriggered();
	virtual void	Unk_C7(void);
	virtual bool	DisarmPlacedExplosives(TESObjectREFR *refr, bool unk);
	virtual void	Unk_C9(void);
	virtual void	Unk_CA(void);
	virtual void	Unk_CB(void);

	enum
	{
		kProjFlag_Bit00Unk =			0x1,
		kProjFlag_Bit01Unk =			0x2,
		kProjFlag_Bit02Unk =			0x4,
		kProjFlag_Bit03Unk =			0x8,
		kProjFlag_Bit04Unk =			0x10,
		kProjFlag_Bit05Unk =			0x20,
		kProjFlag_Bit06Unk =			0x40,
		kProjFlag_Bit07Unk =			0x80,
		kProjFlag_Bit08Unk =			0x100,
		kProjFlag_MineDisarmed =		0x200,
		kProjFlag_Bit0AUnk =			0x400,
		kProjFlag_Bit0BUnk =			0x800,
		kProjFlag_Bit0CUnk =			0x1000,
		kProjFlag_Bit0DUnk =			0x2000,
		kProjFlag_Bit0EUnk =			0x4000,
		kProjFlag_Bit0FUnk =			0x8000,		// Don't apply source-weapon's damage upon impact
		kProjFlag_Bit10Unk =			0x10000,
		kProjFlag_Bit11Unk =			0x20000,
		kProjFlag_Bit12Unk =			0x40000,
		kProjFlag_Bit13Unk =			0x80000,
		kProjFlag_Bit14Unk =			0x100000,
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

	struct Struct128
	{
		UInt32			unk00;
		UInt8			byte04;
		UInt8			pad05[3];
		UInt32			status;		//	0 - Not triggered, 1 - Triggered, 2 - Disarmed
	};

	tList<ImpactData>	impactDataList;	// 088
	UInt8				hasImpacted;	// 090
	UInt8				pad091[3];		// 091
	float				unk094[13];		// 094
	UInt32				projFlags;		// 0C8
	float				speedMult1;		// 0CC
	float				speedMult2;		// 0D0
	float				flt0D4;			// 0D4
	float				elapsedTime;	// 0D8
	float				hitDamage;		// 0DC
	float				flt0E0;			// 0E0
	float				detonationTime;	// 0E4
	float				flt0E8;			// 0E8
	float				flt0EC;			// 0EC
	float				flt0F0;			// 0F0
	float				wpnHealthPerc;	// 0F4
	TESObjectWEAP		*sourceWeap;	// 0F8
	TESObjectREFR		*sourceRef;		// 0FC
	UInt32				unk100;			// 100
	float				flt104;			// 104
	float				flt108;			// 108
	float				flt10C;			// 10C
	float				distTravelled;	// 110
	NiPointLight		*projLight;		// 114
	UInt8				byte118;		// 118
	UInt8				pad119[3];		// 119
	NiNode				*node11C;		// 11C
	UInt32				unk120;			// 120
	float				flt124;			// 124
	Struct128			unk128;			// 128
	Struct128			unk134;			// 134
	UInt32				unk140;			// 140
	ContChangesEntry	*rockItEntry;	// 144
	UInt8				byte148;		// 148
	UInt8				pad149[3];		// 149
	float				range;			// 14C

	void GetData(UInt32 dataType, double *result);
};
STATIC_ASSERT(sizeof(Projectile) == 0x150);

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

// 104
class Explosion : public MobileObject
{
public:
	virtual void	Unk_C1(void);

	float			unk088;			// 088
	float			unk08C;			// 08C
	float			unk090;			// 090
	float			unk094;			// 094
	float			unk098;			// 098
	float			unk09C;			// 09C
	NiRefObject		*object0A0;		// 0A0
	tList<void>		list0A4;		// 0A4
	UInt32			unk0AC[6];		// 0AC
	NiPointLight	*pointLight;	// 0C4
	UInt32			unk0C8[2];		// 0C8
	NiRefObject		*object0D0;		// 0D0
	UInt32			unk0D4[11];		// 0D4
	float			unk100;			// 100
};
STATIC_ASSERT(sizeof(Explosion) == 0x104);

// 0C (from Stewie)
struct Sound
{
	UInt32		unk00;		// 00
	UInt8		byte04;		// 04
	UInt8		pad05[3];	// 05
	UInt32		unk08;		// 08

	Sound() : unk00(0xFFFFFFFF), byte04(0), unk08(0) {}

	Sound(const char *soundEDID, UInt32 flags)
	{
		ThisCall(0xAD7550, *(BSWin32Audio**)0x11F6D98, this, soundEDID, flags);
	}

	void Play()
	{
		ThisCall(0xAD8830, this, 0);
	}
};

template <typename Item> struct ListBoxItem
{
	Tile	*tile;
	Item	*object;
};

// 30
template <typename Item> class ListBox : public BSSimpleList<ListBoxItem<Item>>
{
public:
	Tile			*parentTile;	// 0C
	Tile			*selected;		// 10
	Tile			*scrollBar;		// 14
	const char		*templateName;	// 18
	UInt16			itemCount;		// 1C
	UInt16			pad1E;			// 1E
	float			flt20;			// 20
	float			listIndex;		// 24
	float			currValue;		// 28
	UInt16			word2C;			// 2C
	UInt16			pad2E;			// 2E

	Item *GetSelected()
	{
		ListNode<ListBoxItem<Item>> *iter = list.Head();
		ListBoxItem<Item> *item;
		do
		{
			item = iter->data;
			if (item && (item->tile == selected))
				return item->object;
		}
		while (iter = iter->next);
		return NULL;
	}

	void Clear()
	{
		ListNode<ListBoxItem<Item>> *iter = list.Head();
		ListBoxItem<Item> *item;
		do
		{
			item = iter->data;
			if (!item) continue;
			if (item->tile)
				item->tile->Destroy(true);
			GameHeapFree(item);
		}
		while (iter = iter->next);
		list.RemoveAll();
		selected = NULL;
		itemCount = 0;
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
};

typedef ListBox<ContChangesEntry> MenuItemEntryList;

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
STATIC_ASSERT(sizeof(HotKeyWheel) == 0x34);

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
};
STATIC_ASSERT(sizeof(InventoryMenu) == 0x124);

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
	ListBox<PerkRank>				perkRankList;		// 1E0
	ListBox<UInt32>					miscStatIDList;		// 210
	ListBox<StatusEffect>			statusEffListBox;	// 240
	ListBox<TESReputation>			reputationList;		// 270
	UInt8							isInHealLimbSelect;	// 2A0
	UInt8							pad2A0[3];			// 2A1
};

// 50
class Tile3D : public Tile
{
public:
	UInt32			unk38;		// 38
	UInt32			unk3C;		// 3C
	String			string40;	// 40
	String			fileName;	// 48
};

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

	struct SubtitleData;

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
	UInt32							currMsgKey;			// 194
	BSSimpleArray<SubtitleData>		subtitlesArr;		// 198
	UInt32							unk1A8[4];			// 1A8
	TESObjectREFR					*crosshairRef;		// 1B8
	UInt32							unk1BC;				// 1BC
	UInt32							visibilityOverrides;	// 1C0
	UInt32							stage;				// 1C4
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
	UInt32							unk244;				// 244
	UInt32							unk248[4];			// 248
	tList<QueuedQuestText>			queuedQuestTextList;// 258
	UInt8							byte260;			// 260
	UInt8							byte261;			// 261
	UInt8							pad262[2];			// 262
	tList<UInt32>					xpMessages;			// 264
	tList<UInt32>					list26C;			// 26C
	float							hudShake;			// 274
};
STATIC_ASSERT(sizeof(HUDMainMenu) == 0x278);

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
STATIC_ASSERT(sizeof(LoadingMenu) == 0x5C0);

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
	UInt32				unk0FC[4];			// 0FC
};
STATIC_ASSERT(sizeof(ContainerMenu) == 0x10C);

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
};
STATIC_ASSERT(sizeof(DialogMenu) == 0x13C);

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
};
STATIC_ASSERT(sizeof(SleepWaitMenu) == 0x4C);

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
	ListBox<Option>					options084;		// 084
	ListBox<Option>					options0B4;		// 0B4
	ListBox<Option>					options0E4;		// 0E4
	ListBox<Option>					options114;		// 114
	ListBox<int>					listBox144;		// 144
	ListBox<BGSSaveLoadFileEntry>	listBox174;		// 174
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
};
STATIC_ASSERT(sizeof(StartMenu) == 0x1D4);

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
};
STATIC_ASSERT(sizeof(LockPickMenu) == 0xE4);

// 44
class QuantityMenu : public Menu		// 1016
{
public:
	virtual void		Unk_12(void);

	TileRect			*tile28;		// 28
	TileImage			*tile2C;		// 2C	QM_DecreaseArrow
	TileImage			*tile30;		// 30	QM_IncreaseArrow
	TileText			*tile34;		// 34
	TileImage			*tile38;		// 38
	TileImage			*tile3C;		// 3C
	float				currentQtt;		// 40
};

// 230
class MapMenu : public Menu				// 1023
{
public:
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
	UInt32							unk0F8[4];		// 0F8
	TESObjectCELL					*cell108;		// 108
	TESWorldSpace					*wspc10C;		// 10C
	UInt32							unk110;			// 110
	TESObjectREFR					*lastExtDoor;	// 114
	TESObjectREFR					*selectedMarker;// 118
	TESObjectCELL					*cell11C;		// 11C
	float							clickStartPosX;	// 120
	float							clickStartPosY;	// 124
	UInt32							unk128;			// 128
	bool							fogOfWar;		// 12C
	UInt8							pad12D[3];		// 12D
	ListBox<TESQuest>				questList;		// 130
	ListBox<BGSNote>				noteList;		// 160
	ListBox<TESObjectREFR>			radioRefList;	// 190
	ListBox<BGSQuestObjective>		objectiveList;	// 1C0
	ListBox<TESChallenge>			challengeList;	// 1F0
	BSSimpleArray<Tile>				arr220;			// 220
};
STATIC_ASSERT(sizeof(MapMenu) == 0x230);

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
	ListBox<BGSPerk>	perkListBox;			// 94
	tList<BGSPerk>		availablePerks;			// C4	Perks to show in the perk listBox
};
STATIC_ASSERT(sizeof(LevelUpMenu) == 0xCC);

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
};

// 1A4
class RaceSexMenu : public Menu			// 1036
{
public:
	UInt32				unk028[44];		// 028
	TESNPC				*npc;			// 0D8
	UInt32				unk0DC[50];		// 0DC
};
STATIC_ASSERT(sizeof(RaceSexMenu) == 0x1A4);

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
};
STATIC_ASSERT(sizeof(TextEditMenu) == 0x5C);

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
};
STATIC_ASSERT(sizeof(HackingMenu) == 0x1DC);

struct VATSTargetInfo
{
	TESObjectREFR	*targetRef;
	UInt32			unk04;
};

// 144
class VATSMenu : public Menu			// 1056
{
public:
	virtual void		Unk_12(void);

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
		kAPCost_MAX = 0x16,
	};

	UInt32				unk028;			// 028
	TileImage			*tile02C;		// 02C
	TileImage			*tile030;		// 030
	TileImage			*tile034;		// 034
	TileImage			*tile038;		// 038
	TileImage			*tile03C;		// 03C
	TileImage			*tile040;		// 040
	TileImage			*tile044;		// 044
	TileImage			*tile048;		// 048
	TileImage			*tile04C;		// 04C
	TileImage			*tile050;		// 050
	TileText			*tile054;		// 054
	TileText			*tile058;		// 058
	TileText			*tile05C;		// 05C
	TileText			*tile060;		// 060
	TileImage			*tile064;		// 064
	TileImage			*tile068;		// 068
	TileImage			*tile06C;		// 06C
	TileImage			*tile070;		// 070
	TileText			*tile074;		// 074
	TileRect			*tile078;		// 078
	TileRect			*tile07C;		// 07C
	TileRect			*tile080;		// 080
	TileImage			*tile084;		// 084
	TileRect			*tile088;		// 088
	TileImage			*tile08C;		// 08C
	TileImage			*tile090;		// 090
	TileImage			*tile094;		// 094
	TileImage			*tile098;		// 098
	TileText			*tile09C;		// 09C
	TileImage			*tile0A0;		// 0A0
	TileImage			*tile0A4;		// 0A4
	UInt32				unk0A8[2];		// 0A8
	ListBox<UInt32>		queuedActions;	// 0B0
	float				actionPoints;	// 0E0
	float				maxActionPoints;// 0E4
	float				flt0E8;			// 0E8
	float				flt0EC;			// 0EC
	float				clipAmmo;		// 0F0
	float				reserveAmmo;	// 0F4
	UInt32				unk0F8;			// 0F8
	float				flt0FC;			// 0FC
	UInt32				unk100[7];		// 100
	int					actionType;		// 11C
	UInt8				isSuccess;		// 120
	UInt8				byte121;		// 121
	UInt8				isMysteriousStrangerVisit;	// 122
	UInt8				byte123;		// 123
	UInt8				remainingShotsToFire;	// 124
	UInt8				count125;		// 125
	UInt8				pad126[2];		// 126
	TESObjectREFR		*targetRef;		// 128
	UInt32				avCode;			// 12C
	ActorHitData		*hitData;		// 130
	float				unk134;			// 134
	float				unk138;			// 138
	float				apCost;			// 13C
	UInt8				isMissFortuneVisit;	// 140
	UInt8				pad141[3];		// 141
};
STATIC_ASSERT(sizeof(VATSMenu) == 0x144);

// FC
class ComputersMenu : public Menu		// 1057
{
public:
	UInt32				unk28[32];		// 28
	BGSTerminal			*baseTerminal;	// A8
	TESObjectREFR		*targetRef;		// AC
	UInt32				unkB0[19];		// B0
};
STATIC_ASSERT(sizeof(ComputersMenu) == 0xFC);

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
	UInt32				unk98;			// 98
	UInt8				skill;			// 9C
	UInt8				pad9D[3];		// 9D
};
STATIC_ASSERT(sizeof(RepairServicesMenu) == 0xA0);

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
};
STATIC_ASSERT(sizeof(ItemModMenu) == 0x90);

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
};
STATIC_ASSERT(sizeof(CompanionWheelMenu) == 0x88);

// 88
class TraitSelectMenu : public Menu		// 1076
{
public:
	UInt32				unk28[24];		// 28
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
	UInt32						unk068;			// 068
	ListBox<TESRecipe>			recipeList;		// 06C
	ListBox<TESRecipe>			*unk09C;		// 09C
	ListBox<RecipeComponent>	componentList;	// 0A0
	ListBox<Condition>			conditionList;	// 0D0
	UInt32						unk100;			// 100
};

// E88
class CaravanMenu : public Menu			// 1083
{
public:
	UInt32					unk028[54];		// 028
	TESObjectREFR			*opponentRef;	// 100
	UInt32					unk104[865];	// 104
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
	ListBox<BGSPerk>	perkListBox;	// 54
	tList<BGSPerk>		perkList;		// 84
};

// D8
class FORenderedMenu
{
public:
	virtual void	Destructor(bool doFree);
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

	NiAVObject			*unk04;			// 04
	BSFadeNode			*node08;		// 08
	UInt32				unk0C;			// 0C
	NiSourceTexture		*srcTexture;	// 10
	NiNode				*node14;		// 14
	TileMenu			*tileMenu;		// 18
	UInt32				unk1C[2];		// 1C
	NiCamera			*camera;		// 24
	UInt32				unk28[44];		// 28
};

// 170 (from Stewie)
class FOPipboyManager : public FORenderedMenu
{
public:
	NiNode			*pipBoyScreenNode;		// 0D8
	NiNode			*node0DC;				// 0DC
	NiTriStrips		*triStrips0E0;			// 0E0
	UInt32			unk0E4;					// 0E4
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
	float			time164;				// 164
	float			lightEffectFadeDuration;// 168
	UInt32			unk16C;					// 16C
};
STATIC_ASSERT(sizeof(FOPipboyManager) == 0x170);

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

// 68
struct CombatTarget
{
	Actor		*target;
	UInt32		unk04[25];
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

struct FontHeightData
{
	float		heightBase;
	float		heightwGap;
}
s_fontHeightDatas[90];

// 54
struct FontInfo
{
	struct BufferData
	{
		float			lineHeight;		// 0000
		UInt32			unk0004[73];	// 0004
		UInt32			unk0128[458];	// 0128
		float			baseHeight;		// 0850
		float			flt0854;		// 0854
		float			flt0858;		// 0858
	};

	struct ButtonIcon;

	UInt8						isLoaded;	// 00
	UInt8						pad01[3];	// 01
	char						*filePath;	// 04
	UInt8						fontID;		// 08
	UInt8						pad09[3];	// 09
	NiTexturingProperty			*texProp;	// 0C
	UInt32						unk10[7];	// 10
	float						flt2C;		// 2C
	float						flt30;		// 30
	UInt32						unk34;		// 34
	BufferData					*bufferData;// 38
	UInt32						unk3C[2];	// 3C
	BSSimpleArray<ButtonIcon>	arr44;		// 44
};
STATIC_ASSERT(sizeof(FontInfo) == 0x54);

// 164 (24)
class FontManager
{
public:
	FontInfo		*fontInfos[8];		// 00
	UInt8			byte20;				// 20
	UInt8			pad21[3];			// 21
	FontInfo		*extraFonts[80];	// 24

	//	outDims.x := width (pxl); outDims.y := height (pxl); outDims.z := numLines
	__forceinline NiVector3 *GetStringDimensions(NiVector3 *outDims, const char *srcString, UInt32 fontID, UInt32 maxFlt = 0x7F7FFFFF, UInt32 startIdx = 0)
	{
		return ThisCall<NiVector3*>(0xA1B020, this, outDims, srcString, fontID, maxFlt, startIdx);
	}
};

// 24
struct RadioEntry
{
	TESObjectREFR	*radioRef;
	void			*ptr04;
	UInt32			unk08[7];
};

// 18
class LoadedReferenceMap : public NiTPointerMap<TESObjectREFR>
{
public:
	UInt32			unk10;		// 10
	UInt32			unk14;		// 14
};

// 8C
class BackUpPackage : public TESPackage
{
public:
	virtual void	Unk_58(void);

	NiVector3		destPos;	// 80
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
		UInt32			isVisible;	// 08
		NiNode			*node;		// 0C
		String			text;		// 10
		float			flt18;		// 18	Always -1.0
		NiColorAlpha	color;		// 1C
	};

	DebugLine					lines[200];		// 0004
	DList<String>				textList;		// 2264
	BSSimpleArray<DebugLine>	lines2;			// 2270
	UInt32						unk2280[7];		// 2280

	static DebugText *GetSingleton();
	DebugLine *GetDebugInput();
};
STATIC_ASSERT(sizeof(DebugText) == 0x229C);

DebugText *DebugText::GetSingleton()
{
	return CdeclCall<DebugText*>(0xA0D9E0, true);
}

DebugText::DebugLine *DebugText::GetDebugInput()
{
	DebugLine *linesPtr = lines, *result = lines;
	UInt32 counter = 200;
	do
	{
		linesPtr++;
		if (!linesPtr->isVisible) break;
		if (result->offsetY < linesPtr->offsetY)
			result = linesPtr;
	}
	while (--counter);
	return result;
}

// 254
class BSSoundInfo
{
public:
	UInt32			unk000[72];		// 000
	const char		*filePath;		// 120
	UInt32			unk124[76];		// 124
};

// 198
class BSGameSound
{
public:
	virtual BSGameSound	*Destroy(bool doFree);
	virtual bool	CheckStateFlagsBit5();
	virtual bool	CheckStateFlagsBit19();
	virtual bool	CheckStateFlagsBit6();
	virtual float	GetVolume();
	virtual void	SetIsLooping(bool doSet);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	CopyFrom(BSGameSound *source, bool arg2);
	virtual void	Unk_0C(void);
	virtual bool	Unk_0D(void);
	virtual bool	Unk_0E(void);
	virtual bool	SetVolume(float inVol);
	virtual void	Unk_10(void);
	virtual bool	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(float arg1, float arg2, float arg3);
	virtual void	Unk_14(float arg1, float arg2, float arg3);
	virtual void	Unk_15(NiVector3 &arg1);
	virtual void	Unk_16(void);
	virtual void	Unk_17(float arg1, float arg2);
	virtual void	Unk_18(UInt16 arg1, UInt16 arg2, UInt16 arg3, UInt16 arg4, UInt16 arg5);
	virtual bool	SetFrameFrequencyPerc(float freqPerc);
	virtual float	GetFrameFrequencyPerc();
	virtual void	Seek(UInt32 timePoint);

	enum SoundFlags
	{
		kFlag_2D =						1,
		kFlag_3D =						2,
		kFlag_IsVoice =					4,
		kFlag_IsFootsteps =				8,
		kFlag_Loop =					0x10,
		kFlag_NotDialogue =				0x20,
		kFlag_RandomFrequencyShift =	0x40,
		kFlag_UnkBit7 =					0x80,
		kFlag_UnkBit8 =					0x100,
		kFlag_UnkBit9 =					0x200,
		kFlag_UnkBit10 =				0x400,
		kFlag_IsMusic =					0x800,
		kFlag_MuteWhenSubmerged1 =		0x1000,
		kFlag_MaybeUnderwater =			0x2000,
		kFlag_DonotCache =				0x10000,
		kFlag_FirstPerson =				0x40000,
		kFlag_Modulated =				0x80000,
		kFlag_IsRadio =					0x100000,
		kFlag_UnkBit22 =				0x400000,
		kFlag_EnvelopeFast =			0x2000000,
		kFlag_EnvelopeSlow =			0x4000000,
		kFlag_2DRadius =				0x8000000
	};

	enum Flags00C
	{
		kFlag_MuteWhenSubmerged =		1,
		kFlag_StartAtRandomPosition =	2,
	};

	enum StateFlags
	{
		kState_Stopped =	0x10,
		kState_UnkBit5 =	0x20,
		kState_UnkBit6 =	0x40,
		kState_UnkBit9 =	0x200,
		kState_Muted =		0x400,
		kState_UnkBit15 =	0x8000,
		kState_UnkBit16 =	0x10000,
		kState_IsLooping =	0x20000,
		kState_UnkBit19 =	0x80000,
		kState_UnkBit20 =	0x100000,
	};

	UInt32			mapKey;					// 004
	UInt32			soundFlags;				// 008
	UInt32			flags00C;				// 00C
	UInt32			stateFlags;				// 010
	UInt32			duration;				// 014
	UInt16			staticAttenuation;		// 018	dB * -1000
	UInt16			unk01A;					// 01A
	UInt16			unk01C;					// 01C
	UInt16			unk01E;					// 01E
	UInt16			unk020;					// 020
	UInt16			unk022;					// 022
	float			volume;					// 024	0.0 to 1.0
	float			flt028;					// 028
	float			flt02C;					// 02C
	UInt32			unk030;					// 030
	UInt16			baseSamplingFreq;		// 034
	char			filePath[254];			// 036	Originally: filePath[260]
	TESSound		*sourceSound;			// 134	"Stolen" from filePath
	float			frequencyMod;			// 138	^
	float			maxAttenuationDist;		// 13C
	float			minAttenuationDist;		// 140
	UInt32			pathHashFile;			// 144
	UInt32			pathHashFolder;			// 148
	UInt32			unk14C;					// 14C
	float			flt150;					// 150
	UInt32			unk154;					// 154
	SInt8			randFrequencyShift;		// 158
	UInt8			byte159;				// 159
	UInt16			samplingFreq;			// 15A
	UInt32			unk15C;					// 15C
	UInt32			unk160;					// 160
	UInt32			unk164;					// 164
	UInt32			unk168;					// 168
	UInt32			unk16C;					// 16C
	UInt32			unk170;					// 170
	UInt32			unk174[5];				// 174
	UInt32			priority;				// 188
	UInt32			unk18C[3];				// 18C
};
STATIC_ASSERT(sizeof(BSGameSound) == 0x198);

// 230
class BSWin32GameSound : public BSGameSound
{
public:
	UInt32			unk198;					// 198
	UInt32			*ptr19C;				// 19C	DirectSound func table
	UInt32			unk1A0[5];				// 1A0
	UInt32			*ptr1B4;				// 1B4	DirectSound func table
	UInt32			unk1B8[5];				// 1B8
	UInt32			*ptr1CC;				// 1CC	DirectSound func table
	UInt32			unk1D0;					// 1D0
	UInt32			*ptr1D4;				// 1D4	DirectSound func table
	UInt32			unk1D8[13];				// 1D8
	UInt16			word20C;				// 20C
	UInt16			word20E;				// 20E
	UInt16			word210;				// 210
	UInt16			word212;				// 212
	UInt16			word214;				// 214
	UInt16			word216;				// 216
	NiVector3		originWorldPos;			// 218
	UInt32			unk224[3];				// 224
};
STATIC_ASSERT(sizeof(BSWin32GameSound) == 0x230);

enum AudioRequestTypes
{
	kRequestType_Stop =											3,
	kRequestType_Stop2 =										4,
	kRequestType_Delay =										5,
	kRequestType_StopSoundFadeOutTime =							9,
	kRequestType_DialogueFadeInOrOut =							0xD,
	kRequestType_RecalculateVolumesForChangesToMasterVolume =	0xE,
	kRequestType_Volume =										0x10,
	kRequestType_FrequencyAdjustment =							0x14,
	kRequestType_FadeAllSFX =									0x18,
	kRequestType_SetState400ForSomeSounds =						0x1A,
	kRequestType_ClearState400ForSomeSounds =					0x1B,
	kRequestType_SetState400ForSomeSounds2 =					0x1C,
	kRequestType_ClearState400ForSomeSounds2 =					0x1D,
	kRequestType_SoundAtPos =									0x1E,
	kRequestType_Attenuation =									0x21,
	kRequestType_ReverbAttenuation =							0x22,
	kRequestType_PlayWithMinMaxDistance =						0x23,
	kRequestType_SetsPlayingSoundClearStateBit200 =				0x26,
	kRequestType_PlayAtNode =									0x27,
	kRequestType_NiNodeSound =									0x29,
	kRequestType_StopAllSoundsWithFlags =						0x2A,
	kRequestType_SetAudioListenerPosition =						0x38,
	kRequestType_HasCompletionCallback =						0x3A,
	kRequestType_HasSomeCallback =								0x3B,
	kRequestType_StartsAtEndsAt =								0x3E,
	kRequestType_MultiThreaded =								0x35,
	kRequestType_ToggleFirstPersonForPlayingSound =				0x40,
	kRequestType_Priority =										0x41,
	kRequestType_Speed =										0x42
};

union FunctionArg
{
	void		*pVal;
	float		fVal;
	UInt32		uVal;
	SInt32		iVal;

	FunctionArg& operator=(void *other)
	{
		pVal = other;
		return *this;
	}
	FunctionArg& operator=(float other)
	{
		fVal = other;
		return *this;
	}
	FunctionArg& operator=(UInt32 other)
	{
		uVal = other;
		return *this;
	}
	FunctionArg& operator=(SInt32 other)
	{
		iVal = other;
		return *this;
	}
};

// 20
struct AudioRequestData
{
	UInt32				type;		// 00
	UInt32				soundKey;	// 04
	FunctionArg			value1;		// 08
	FunctionArg			value2;		// 0C
	NiNode				*niNode;	// 10
	NiVector3			pos;		// 14
};

// 188
class BSAudioManager
{
public:
	virtual void				Destroy(bool doFree);

	// 10
	struct AudioRequest
	{
		UInt32				count;			// 00
		UInt8				byte04;			// 04
		UInt8				pad05[3];		// 05
		AudioRequestData	*requestData;	// 08
		UInt32				tickCount;		// 0C
	};

	UInt32						unk004[8];			// 004
	AudioRequest				request024;			// 024
	AudioRequest				request034;			// 034
	AudioRequest				request044;			// 044
	NiTPointerMap<BSGameSound>	playingSounds;		// 054
	NiTPointerMap<BSSoundInfo>	playingSoundInfos1;	// 064
	NiTPointerMap<BSSoundInfo>	playingSoundInfos2;	// 074
	NiTPointerMap<NiAVObject>	soundPlayingObjects;// 084
	DList<BSGameSound>			cachedSounds;		// 094
	UInt32						unk0A0;				// 0A0
	UInt32						unk0A4;				// 0A4
	float						flt0A8;				// 0A8
	LPCRITICAL_SECTION			cs0AC;				// 0AC
	UInt32						unk0B0[5];			// 0B0
	LPCRITICAL_SECTION			cs0C4;				// 0C4
	UInt32						unk0C8[5];			// 0C8
	LPCRITICAL_SECTION			cs0DC;				// 0DC
	UInt32						unk0E0[5];			// 0E0
	LPCRITICAL_SECTION			cs0F4;				// 0F4
	UInt32						unk0F8[5];			// 0F8
	LPCRITICAL_SECTION			cs10C;				// 10C
	UInt32						unk110[5];			// 110
	DList<void>					list124;			// 124
	UInt32						lastTickCount;		// 130
	UInt8						byte134;			// 134
	UInt8						byte135;			// 135
	UInt8						pad136[2];			// 136
	UInt32						unk138;				// 138
	BSAudioManagerThread		*audioMgrThread;	// 13C
	float						volumes[12];		// 140
	//	0	Master
	//	1	Foot
	//	2	Voice
	//	3	Effects
	//	4	Music
	//	5	Radio
	UInt32						unk170;				// 170
	UInt8						byte174;			// 174
	UInt8						byte175;			// 175
	UInt8						pad176[2];			// 176
	UInt32						unk178;				// 178
	UInt32						unk17C;				// 17C
	UInt32						nextMapKey;			// 180
	UInt8						byte184;			// 184
	UInt8						pad185[3];			// 185
};
STATIC_ASSERT(sizeof(BSAudioManager) == 0x188);

class BSAudioListener
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	SetOriginWorldPos(NiVector3 *pos);
	virtual void	Unk_03(void);
	virtual void	UpdatePositionAndOrientation();
	virtual void	SetFrontAndTopOrientation(NiVector3 *front, NiVector3 *top);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	SetVelocity(NiVector3 *pVelocity);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	SetRolloffFactor(float factor);
};

class IDirectSound3DListener;

// 64
class BSWin32AudioListener : public BSAudioListener
{
public:
	NiVector3				originWorldPos;		// 04
	UInt32					unk10[6];			// 10
	NiVector3				velocity;			// 28
	UInt32					sysTime;			// 34
	float					flt38;				// 38
	float					flt3C;				// 3C
	float					flt40;				// 40
	UInt32					unk44;				// 44
	IDirectSound3DListener	**ptr48;			// 48
	NiVector3				topOrientation;		// 4C
	NiVector3				frontOrientation;	// 58
};
STATIC_ASSERT(sizeof(BSWin32AudioListener) == 0x64);

class IDirectSound8;
class IDirectSoundBuffer;

// A4
class BSWin32Audio
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	CreateAudioListener(HWND _window);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);	// Does nothing
	virtual void	Unk_04(void);	// Does nothing
	virtual BSGameSound	*CreateGameSound(const char *filePath);
	virtual void	InsertPathPrefix(char *filePath);	// Prefixes path with data\\sound\\ if fx\\ or song\\.
	virtual void	Unk_07(void);	// Does nothing

	UInt8					byte04;			// 04
	UInt8					byte05;			// 05
	UInt8					byte06;			// 06
	UInt8					pad07;			// 07
	UInt32					unk08;			// 08
	UInt8					byte0C;			// 0C
	UInt8					pad0D[3];		// 0D
	BSWin32AudioListener	*listener;		// 10
	UInt32					unk14[3];		// 14
	bool					(*GetSoundDataFromRefID)(UInt32 refID, char *outFilePath, UInt32 *outFlags, TESSound **outSound);	// 20	0x82D150
	bool					(*GetSoundDataFromEDID)(const char *EDIDstr, char *outFilePath, UInt32 *outFlags, TESSound **outSound);	// 24	0x82D280
	bool					(*PickSoundFileFromFolder)(char *outFilePath);	// 28	0x5E3630
	UInt32					(*FillGameSoundProps)(UInt32 *mapKey, TESSound *soundForm, UInt32 *outFlags0C);	// 2C	0x82D400
	void					(*sub_832C40)(void);	// 30
	void					(*sub_832C80)(void);	// 34
	IDirectSound8 			**ptr38;				// 38
	IDirectSoundBuffer		*ptr3C;					// 3C
	UInt32					unk40[24];				// 40
	HWND					window;					// A0
};
STATIC_ASSERT(sizeof(BSWin32Audio) == 0xA4);

// 30
class BSThread
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);

	LPCRITICAL_SECTION	cs;			// 04
	UInt32				unk08[10];	// 08
};

// 9C
class FAMThread : public BSThread
{
public:
	UInt32				unk30[27];	// 30
};

enum
{
	kMusicState_Pause =		1 << 2,
	kMusicState_Stop =		1 << 3,
	kMusicState_Play =		1 << 4,
	kMusicState_Loop =		1 << 5,
};

// 288
struct PlayingMusic
{
	char					track1Path[MAX_PATH];	// 000
	FAMThread				*famThread;				// 104
	char					track2Path[MAX_PATH];	// 108
	UInt32					unk20C;					// 20C
	float					flt210;					// 210
	float					flt214;					// 214
	float					flt218;					// 218
	float					flt21C;					// 21C
	UInt8					track1Flags;			// 220
	UInt8					track2Flags;			// 221
	UInt8					pad222[2];				// 222
	UInt32					unk224;					// 224
	UInt32					unk228[8];				// 228
	float					flt248;					// 248
	float					flt24C;					// 24C
	float					flt250;					// 250
	float					flt254;					// 254
	UInt32					unk258;					// 258
	UInt32					unk25C[8];				// 25C
	UInt32					track1Active;			// 27C
	UInt32					unk280;					// 280
	MediaLocationController	*medLocCtrl;			// 284
};
STATIC_ASSERT(sizeof(PlayingMusic) == 0x288);

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
};

// 10
struct EffectArchTypeEntry
{
	const char				*name;
	ActiveEffectCreate		callback;
	UInt32					unk08[2];
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
};

// 24
struct AnimGroupInfo
{
	const char	*name;			// 00
	UInt32		unk04;			// 04
	UInt32		sequenceType;	// 08
	UInt32		unk0C;			// 0C
	UInt32		unk10;			// 10
	UInt32		unk14[4];		// 14
};

struct PCMiscStat
{
	const char	*name;
	UInt32		level;
};

// 08
struct TypeSignature
{
	char	signature[8];
};

// 34
class NiPick
{
public:
	NiPick(UInt32 capacity = 0, UInt32 growSize = 8) {ThisCall(0xE98F20, this, capacity, growSize);}
	~NiPick() {ThisCall(0xE98FA0, this);}

	// 10
	struct Results
	{
		struct Result
		{
			NiAVObject	*object;
			UInt32		unk04;
			NiVector3	pos;
		};

		UInt32		**_vtbl;		// 00
		Result		**data;			// 04
		UInt16		capacity;		// 08
		UInt16		firstFreeEntry;	// 0A
		UInt16		numObjs;		// 0C
		UInt16		growSize;		// 0E
	};

	UInt32			unk00;		// 00	If non-zero, returns only one result
	UInt32			unk04;		// 04
	UInt32			unk08;		// 08
	UInt32			unk0C;		// 0C
	UInt8			byte10;		// 10
	UInt8			byte11;		// 11
	UInt8			pad12[2];	// 12
	NiRefObject		*object14;	// 14
	Results			results;	// 18
	UInt32			numResults;	// 28
	UInt32			unk2C;		// 2C
	UInt8			byte30;		// 30
	UInt8			byte31;		// 31
	UInt8			byte32;		// 32
	UInt8			byte33;		// 33
};

// 14
class ExtraHavok : public BSExtraData
{
public:
	bhkWorld		*world;			// 0C
	UInt32			unk10;			// 10
};

// 10
class ExtraRegionList : public BSExtraData
{
public:
	TESRegionList	*regionList;	// 0C
};

// 24
class SeenData
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual void	Unk_02(UInt32 arg1);
	virtual void	SaveGame(UInt32 arg1);
	virtual void	DoSave(UInt32 arg1);
	virtual void	LoadGame(UInt32 arg1);
	virtual void	DoLoad(UInt32 arg1);

	UInt8			seenBits[32];	// 04	256 bitfield
};

// 2C
class IntSeenData : public SeenData
{
public:
	SInt8			segmentX;		// 24
	SInt8			segmentY;		// 25
	bool			fullySeen;		// 26	MiniMap only!
	UInt8			pad27;			// 27
	IntSeenData		*nextSegment;	// 28
};

// 10
class ExtraSeenData : public BSExtraData
{
public:
	SeenData		*data;		// 0C
};

// 10
class ExtraDetachTime : public BSExtraData
{
public:
	int		detachTime;		// 0C
};

// 10
class ExtraCellWaterType : public BSExtraData
{
public:
	TESWaterForm	*waterForm;		// 0C

	static ExtraCellWaterType* __stdcall Create(TESWaterForm *_waterForm);
};

// 10
class ExtraCellMusicType : public BSExtraData
{
public:
	BGSMusicType	*musicType;

	static ExtraCellMusicType* __stdcall Create(BGSMusicType *_musicType);
};

// 10
class ExtraCellClimate : public BSExtraData
{
public:
	TESClimate		*climate;		// 0C

	static ExtraCellClimate* __stdcall Create(TESClimate *_climate);
};

// 10
class ExtraPersistentCell : public BSExtraData
{
public:
	TESObjectCELL	*persistentCell;	// 0C
};

// 24
class ExtraStartingPosition : public BSExtraData
{
public:
	NiVector3	posVector;	// 0C
	NiVector3	rotVector;	// 18
};

// 10
class ExtraAnim : public BSExtraData
{
public:
	AnimData		*animData;	// 0C
};

// 1C
class ExtraPackage : public BSExtraData
{
public:
	TESPackage		*package;	// 0C
	UInt32			unk10[3];	// 10
};

// 10
class ExtraFollower : public BSExtraData
{
public:
	tList<Actor>	*followers;		// 0C
};

// 10
class ExtraOriginalReference : public BSExtraData
{
public:
	TESObjectREFR	*originalRef;		// 0C

	static ExtraOriginalReference* __stdcall Create(TESObjectREFR *_originalRef);
};

// 10
class ExtraUses : public BSExtraData
{
public:
	UInt8		uses;		// 0C

	static ExtraUses *Create();
};

// 10
class ExtraTimeLeft : public BSExtraData
{
public:
	float		timeLeft;		// 0C

	static ExtraTimeLeft *Create();
};

// 10
class ExtraCharge : public BSExtraData
{
public:
	float		charge;		// 0C

	static ExtraCharge *Create(float _charge);
};

// 10
class ExtraLight : public BSExtraData
{
public:
	struct Data
	{
		NiPointLight	*pointLight;
		float			fadeValue;
	};

	Data		*data;		// 0C
};

// 14
class ExtraLeveledItem : public BSExtraData
{
public:
	UInt32		unk0C;		// 0C
	UInt8		byte10;		// 10
	UInt8		pad11[3];	// 11
};

// 14
class ExtraEnableStateChildren : public BSExtraData
{
public:
	tList<TESObjectREFR>	children;	// 0C
};

// 10
class ExtraItemDropper : public BSExtraData
{
public:
	TESObjectREFR		*dropper;	// 0C
};

// 14
class ExtraDroppedItemList : public BSExtraData
{
public:
	tList<TESObjectREFR>	itemRefs;	// 0C
};

// 10
class ExtraMerchantContainer : public BSExtraData
{
public:
	TESObjectREFR	*containerRef;	// 0C
};

// 10
class ExtraRefractionProperty : public BSExtraData
{
public:
	float			refraction;		// 0C
};

// 10
class ExtraPoison : public BSExtraData
{
public:
	AlchemyItem		*poisonEffect;	// 0C
};

// 10
class ExtraNorthRotation : public BSExtraData
{
public:
	float		rotation;		// 0C
};

// 10
class ExtraStartingWorldOrCell : public BSExtraData
{
public:
	TESForm			*worldOrCell;	// 0C
};

// 10
class ExtraTerminalState : public BSExtraData
{
public:
	UInt8			lockedOut;	// 0C
	UInt8			lockLevel;	// 0D unlocked: 0xFE
	UInt16			pad;

	static ExtraTerminalState *Create();
};

// 10
class ExtraLinkedRef : public BSExtraData
{
public:
	TESObjectREFR	*linkedRef;		// 0C

	static ExtraLinkedRef* __stdcall Create(TESObjectREFR *_linkedRef = NULL);
};

// 14
class ExtraLinkedRefChildren : public BSExtraData
{
public:
	tList<TESObjectREFR>	children;	// 0C
};

// 18
class ExtraActivateRef : public BSExtraData
{
public:
	struct parentRef
	{
		TESObjectREFR*	ref;
		float			delay;
	};
	tList<parentRef>	parentRefs;
	UInt32				flags;
};

// 10
class ExtraTalkingActor : public BSExtraData
{
public:
	Actor			*actor;		// 0C
};

// 10
class ExtraObjectHealth : public BSExtraData
{
public:
	float			health;		// 0C

	static ExtraObjectHealth* __stdcall Create(float _health);
};

// 10
class ExtraCellImageSpace : public BSExtraData
{
public:
	TESImageSpace	*imageSpace;	// 0C

	static ExtraCellImageSpace* __stdcall Create(TESImageSpace *_imgSpace);
};

// 10
class ExtraRadius : public BSExtraData
{
public:
	float			radius;		// 0C

	static ExtraRadius* __stdcall Create(float _radius);
};

// 10
class ExtraRadiation : public BSExtraData
{
public:
	float			radiation;	// 0C
};

// 30
class ExtraDismemberedLimbs : public BSExtraData
{
public:
	struct DismemberedLimb
	{
		UInt8				bodyPartID;
		UInt8				explodeChance;
		bool				explodeOnly;
		UInt8				byte03;
		BSSimpleArray<void>	*array04;
	};

	UInt16							dismemberedMask;	// 0C
	UInt8							pad0E[2];			// 0E
	int								unk10;				// 10
	TESObjectWEAP					*weapon;			// 14
	int								unk18;				// 18
	bool							wasEaten;			// 1C
	UInt8							pad1D[3];			// 1D
	BSSimpleArray<DismemberedLimb>	dismemberedLimbs;	// 20
};

// 1C
class ExtraRadioData : public BSExtraData
{
public:
	float				radius;			// 0C
	UInt32				rangeType;		// 10
	float				staticPerc;		// 14
	TESObjectREFR		*positionRef;	// 18
};

// 10
class ExtraPrimitive : public BSExtraData
{
public:
	BGSPrimitive	*primitive;		// 0C

	static ExtraPrimitive* __stdcall Create(BGSPrimitive *_primitive = NULL);
};

// 10
class ExtraCollisionData : public BSExtraData
{
public:
	UInt32			unk0C;			// 0C
};

// 10
class ExtraEncounterZone : public BSExtraData
{
public:
	BGSEncounterZone	*zone;		// 0C
};

// 1C
class ExtraSayToTopicInfo : public BSExtraData
{
public:
	TESTopicInfo	*info;		// 0C
	TESTopic		*topic;		// 10
	TESQuest		*quest;		// 14
	Actor			*speaker;	// 18
};

// 10
class ExtraCellAcousticSpace : public BSExtraData
{
public:
	BGSAcousticSpace	*acousticSpace;		// 0C

	static ExtraCellAcousticSpace* __stdcall Create(BGSAcousticSpace *_acousticSpace = NULL);
};

// 10
class ExtraAshPileRef : public BSExtraData
{
public:
	TESObjectREFR		*sourceRef;		// 0C
};

// 28
class ExtraFollowerSwimBreadcrumbs : public BSExtraData
{
public:
	UInt32			unk0C;
	float			unk10;
	float			unk14;
	float			unk18;
	UInt32			unk1C;
	tList<void>		list20;
};

// 10
class ExtraCellImpactSwap : public BSExtraData
{
public:
	ImpactDataSwap		*impactSwap;	// 0C
};

// 10
class ExtraAudioMarker : public BSExtraData
{
public:
	// 34
	struct Data
	{
		TESFullName		fullName;
		float			layer2TriggerPerc;
		float			layer3TriggerPerc;
		UInt32			unk14[4];
		UInt32			mediaLocCtrlID;
		UInt32			flags;
		UInt32			unk2C[2];
	};
	
	Data		*data;		// 0C
};

struct MusicMarker
{
	TESObjectREFR			*markerRef;
	ExtraAudioMarker::Data	*markerData;

	MusicMarker(TESObjectREFR *_markerRef, ExtraAudioMarker::Data *data) : markerRef(_markerRef), markerData(data) {}

	bool operator<(const MusicMarker &rhs) const {return markerData->mediaLocCtrlID < rhs.markerData->mediaLocCtrlID;}
};

// 14
class ExtraSpecialRenderFlags : public BSExtraData
{
public:
	UInt32		flags;		// 0C
	UInt32		unk10;		// 10

	static ExtraSpecialRenderFlags* __stdcall Create(UInt32 _flags = 0);
};

// 48
struct VATSCameraData
{
	tList<void>						targetsList;	// 00
	UInt32							mode;			// 08
	UInt32							unk0C;			// 0C
	BGSCameraShot					*camShot;		// 10
	float							flt14;			// 14
	float							flt18;			// 18
	UInt32							unk1C;			// 1C
	UInt32							unk20;			// 20
	UInt32							unk24;			// 24
	ImageSpaceModifierInstanceForm	*isModInstForm;	// 28
	ImageSpaceModifierInstanceRB	*isModInstRB;	// 2C
	UInt32							unk30;			// 30
	NiObject						*object34;		// 34
	UInt8							byte38;			// 38
	UInt8							pad39[3];		// 39
	UInt32							unk3C;			// 3C
	UInt32							unk40;			// 40
	UInt32							unk44;			// 44
};
STATIC_ASSERT(sizeof(VATSCameraData) == 0x48);

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
};

__declspec(naked) void SystemColorManager::SystemColor::SetColorRGB(UInt32 r, UInt32 g, UInt32 b)
{
	__asm
	{
		xor		eax, eax
		mov		ah, [esp+4]
		mov		al, [esp+8]
		shl		eax, 0x10
		mov		ah, [esp+0xC]
		mov		al, 0xFF
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+8]
		retn	0xC
	}
}

// 8C
class ObstacleData : public NiRefObject
{
public:
	UInt32						unk08;			// 08
	NiRefObject					*object0C;		// 0C
	UInt32						unk10[25];		// 10
	UInt8						byte74;			// 74
	UInt8						byte75[3];		// 75
	BSSimpleArray<NavMeshInfo>	navMeshInfos;	// 78
	NiRefObject					*object88;		// 88
};

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
	NiTMapBase<UInt16, NavMeshPOVData*>			povDataMap;			// 088
	BSSimpleArray<UInt16>						unk098Arr;			// 098
	UInt32										unk0A8;				// 0A8
	float										unk0AC[8];			// 0AC
	BSSimpleArray<UInt16>						*arrPtr0CC;			// 0CC
	BSSimpleArray<ObstacleUndoData>				obstacleUndoArr;	// 0D0
	NiTMapBase<UInt16, ObstacleData*>			*obstacleDataMap;	// 0E0
	BSSimpleArray<UInt16>						unk0E4Arr;			// 0E4
	BSSimpleArray<NavMeshStaticAvoidNode>		avoidNodeArr;		// 0F4
	UInt32										*ptr104;			// 104
};
STATIC_ASSERT(sizeof(NavMesh) == 0x108);

class BSArchiveHeader
{
public:
};

// 70
class BSArchive : public BSArchiveHeader
{
public:
	UInt32		unk00;			// 00	160
	UInt32		unk04;			// 04	164
	UInt32		unk08;			// 08	168
	UInt32		unk0C;			// 0C	16C
	UInt32		unk10;			// 10	170
	UInt32		unk14;			// 14	174
	UInt32		unk18;			// 18	178
	UInt32		unk1C;			// 1C	17C
	UInt16		fileTypesMask;	// 20	180
	UInt16		word22;			// 22	182
	UInt32		unk24[19];		// 24	184
};
STATIC_ASSERT(sizeof(BSArchive) == 0x70);

// 1D0
class Archive : public BSFile
{
public:
	NiRefObject			refObject;		// 158
	BSArchive			archive;		// 160
};
STATIC_ASSERT(sizeof(Archive) == 0x1D0);

// 160
class ArchiveFile : public BSFile
{
public:
	UInt32			unk158;		// 158
	UInt32			unk15C;		// 15C
};
STATIC_ASSERT(sizeof(ArchiveFile) == 0x160);

// 178
class CompressedArchiveFile : public ArchiveFile
{
public:
	void			*ptr160;		// 160
	void			*ptr164;		// 164
	UInt32			streamLength;	// 168
	UInt32			unk16C;			// 16C
	UInt32			streamOffset;	// 170
	UInt32			unk174;			// 174
};
STATIC_ASSERT(sizeof(CompressedArchiveFile) == 0x178);

// 58
class BGSDistantObjectBlockLoadTask : public QueuedFileEntry
{
public:
	virtual void	Unk_0C(void);

	UInt8			byte30;			// 30
	UInt8			byte31;			// 31
	UInt8			byte32;			// 32
	UInt8			byte33;			// 33
	int				cellX;			// 34
	int				cellY;			// 38
	UInt32			lodLevel;		// 3C
	void			*lodNode14;		// 40
	TESWorldSpace	*worldSpc;		// 44
	NiRefObject		*object48;		// 48
	NiRefObject		*object4C;		// 4C
	UInt8			byte50;			// 50
	UInt8			byte51;			// 51
	UInt8			byte52;			// 52
	UInt8			byte53;			// 53
	UInt32			unk54;			// 54
};
STATIC_ASSERT(sizeof(BGSDistantObjectBlockLoadTask) == 0x58);

struct AnimGroupClassify
{
	UInt8	category;	// 00
	UInt8	subType;	// 01
	UInt8	flags;		// 02
	UInt8	byte03;		// 03
};
const AnimGroupClassify s_animGroupClassify[] =
{
	{1, 1, 0, 0}, {1, 1, 0, 0}, {1, 1, 0, 0}, {2, 1, 0, 0}, {2, 2, 0, 0}, {2, 3, 0, 0}, {2, 4, 0, 0}, {2, 1, 1, 0}, {2, 2, 1, 0}, 
	{2, 3, 1, 0}, {2, 4, 1, 0}, {2, 1, 2, 0}, {2, 2, 2, 0}, {2, 3, 2, 0}, {2, 4, 2, 0}, {2, 3, 4, 0}, {2, 4, 4, 0}, {3, 0, 0, 0}, 
	{3, 0, 1, 0}, {3, 0, 2, 0}, {3, 0, 4, 0}, {3, 0, 5, 0}, {3, 0, 6, 0}, {1, 2, 0, 0}, {1, 2, 0, 0}, {1, 2, 0, 0}, {3, 1, 0, 0}, 
	{3, 1, 1, 0}, {3, 1, 2, 0}, {3, 1, 4, 0}, {3, 1, 5, 0}, {3, 1, 6, 0}, {3, 2, 0, 0}, {3, 2, 1, 0}, {3, 2, 2, 0}, {3, 2, 4, 0}, 
	{3, 2, 5, 0}, {3, 2, 6, 0}, {3, 3, 0, 0}, {3, 3, 1, 0}, {3, 3, 2, 0}, {3, 3, 4, 0}, {3, 3, 5, 0}, {3, 3, 6, 0}, {3, 4, 0, 0}, 
	{3, 4, 1, 0}, {3, 4, 2, 0}, {3, 4, 4, 0}, {3, 4, 5, 0}, {3, 4, 6, 0}, {3, 5, 0, 0}, {3, 5, 1, 0}, {3, 5, 2, 0}, {3, 5, 4, 0}, 
	{3, 5, 5, 0}, {3, 5, 6, 0}, {3, 6, 0, 0}, {3, 6, 1, 0}, {3, 6, 2, 0}, {3, 6, 4, 0}, {3, 6, 5, 0}, {3, 6, 6, 0}, {3, 7, 0, 0}, 
	{3, 7, 1, 0}, {3, 7, 2, 0}, {3, 7, 4, 0}, {3, 7, 5, 0}, {3, 7, 6, 0}, {3, 8, 0, 0}, {3, 8, 1, 0}, {3, 8, 2, 0}, {3, 8, 4, 0}, 
	{3, 8, 5, 0}, {3, 8, 6, 0}, {3, 10, 0, 0}, {3, 10, 1, 0}, {3, 10, 2, 0}, {3, 10, 4, 0}, {3, 10, 5, 0}, {3, 10, 6, 0}, {3, 11, 0, 0}, 
	{3, 11, 1, 0}, {3, 11, 2, 0}, {3, 11, 4, 0}, {3, 11, 5, 0}, {3, 11, 6, 0}, {3, 12, 0, 0}, {3, 12, 1, 0}, {3, 12, 2, 0}, {3, 12, 4, 0}, 
	{3, 12, 5, 0}, {3, 12, 6, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, 
	{3, 23, 0, 0}, {3, 23, 0, 0}, {3, 23, 0, 0}, {3, 21, 0, 0}, {3, 21, 1, 0}, {3, 21, 2, 0}, {3, 21, 4, 0}, {3, 21, 5, 0}, {3, 21, 6, 0}, 
	{3, 22, 0, 0}, {3, 22, 1, 0}, {3, 22, 2, 0}, {3, 22, 4, 0}, {3, 22, 5, 0}, {3, 22, 6, 0}, {3, 13, 0, 0}, {3, 13, 1, 0}, {3, 13, 2, 0}, 
	{3, 13, 4, 0}, {3, 13, 5, 0}, {3, 13, 6, 0}, {3, 14, 0, 0}, {3, 14, 1, 0}, {3, 14, 2, 0}, {3, 14, 4, 0}, {3, 14, 5, 0}, {3, 14, 6, 0}, 
	{3, 15, 0, 0}, {3, 15, 1, 0}, {3, 15, 2, 0}, {3, 15, 4, 0}, {3, 15, 5, 0}, {3, 15, 6, 0}, {3, 16, 0, 0}, {3, 16, 1, 0}, {3, 16, 2, 0}, 
	{3, 16, 4, 0}, {3, 16, 5, 0}, {3, 16, 6, 0}, {3, 17, 0, 0}, {3, 17, 1, 0}, {3, 17, 2, 0}, {3, 17, 4, 0}, {3, 17, 5, 0}, {3, 17, 6, 0}, 
	{3, 9, 0, 0}, {3, 9, 1, 0}, {3, 9, 2, 0}, {3, 9, 4, 0}, {3, 9, 5, 0}, {3, 9, 6, 0}, {3, 18, 0, 0}, {3, 18, 1, 0}, {3, 18, 2, 0}, 
	{3, 18, 4, 0}, {3, 18, 5, 0}, {3, 18, 6, 0}, {3, 19, 0, 0}, {3, 19, 1, 0}, {3, 19, 2, 0}, {3, 19, 4, 0}, {3, 19, 5, 0}, {3, 19, 6, 0}, 
	{3, 20, 0, 0}, {3, 20, 1, 0}, {3, 20, 2, 0}, {3, 20, 4, 0}, {3, 20, 5, 0}, {3, 20, 6, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 3, 0, 0}, 
	{1, 3, 0, 0}, {1, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, 
	{4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, 
	{4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}, {5, 0, 0, 0}, 
	{5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, 
	{5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, 
	{5, 0, 0, 0}, {5, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {1, 4, 0, 0}, 
	{1, 4, 0, 0}, {1, 4, 0, 0}, {1, 4, 0, 0}, {1, 4, 0, 0}, {1, 4, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {1, 0, 0, 0}, 
	{2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}, {2, 5, 0, 0}
};

enum ActorValueCode
{
	kAVCode_Aggression,
	kAVCode_Confidence,
	kAVCode_Energy,
	kAVCode_Responsibility,
	kAVCode_Mood,
	kAVCode_Strength,
	kAVCode_Perception,
	kAVCode_Endurance,
	kAVCode_Charisma,
	kAVCode_Intelligence,
	kAVCode_Agility,
	kAVCode_Luck,
	kAVCode_ActionPoints,
	kAVCode_CarryWeight,
	kAVCode_CritChance,
	kAVCode_HealRate,
	kAVCode_Health,
	kAVCode_MeleeDamage,
	kAVCode_DamageResist,
	kAVCode_PoisonResist,
	kAVCode_RadResist,
	kAVCode_SpeedMult,
	kAVCode_Fatigue,
	kAVCode_Karma,
	kAVCode_XP,
	kAVCode_PerceptionCondition,
	kAVCode_EnduranceCondition,
	kAVCode_LeftAttackCondition,
	kAVCode_RightAttackCondition,
	kAVCode_LeftMobilityCondition,
	kAVCode_RightMobilityCondition,
	kAVCode_BrainCondition,
	kAVCode_Barter,
	kAVCode_BigGuns,
	kAVCode_EnergyWeapons,
	kAVCode_Explosives,
	kAVCode_Lockpick,
	kAVCode_Medicine,
	kAVCode_MeleeWeapons,
	kAVCode_Repair,
	kAVCode_Science,
	kAVCode_Guns,
	kAVCode_Sneak,
	kAVCode_Speech,
	kAVCode_Survival,
	kAVCode_Unarmed,
	kAVCode_InventoryWeight,
	kAVCode_Paralysis,
	kAVCode_Invisibility,
	kAVCode_Chameleon,
	kAVCode_NightEye,
	kAVCode_Turbo,
	kAVCode_FireResist,
	kAVCode_WaterBreathing,
	kAVCode_RadiationRads,
	kAVCode_BloodyMess,
	kAVCode_UnarmedDamage,
	kAVCode_Assistance,
	kAVCode_ElectricResist,
	kAVCode_FrostResist,
	kAVCode_EnergyResist,
	kAVCode_EmpResist,
	kAVCode_Variable01,
	kAVCode_Variable02,
	kAVCode_Variable03,
	kAVCode_Variable04,
	kAVCode_Variable05,
	kAVCode_Variable06,
	kAVCode_Variable07,
	kAVCode_Variable08,
	kAVCode_Variable09,
	kAVCode_Variable10,
	kAVCode_IgnoreCrippledLimbs,
	kAVCode_Dehydration,
	kAVCode_Hunger,
	kAVCode_SleepDeprivation,
	kAVCode_DamageThreshold,
};

enum MiscStatCode
{
	kMiscStat_QuestsCompleted,
	kMiscStat_LocationsDiscovered,
	kMiscStat_PeopleKilled,
	kMiscStat_CreaturesKilled,
	kMiscStat_LocksPicked,
	kMiscStat_ComputersHacked,
	kMiscStat_StimpaksTaken,
	kMiscStat_RadXTaken,
	kMiscStat_RadAwayTaken,
	kMiscStat_ChemsTaken,
	kMiscStat_TimesAddicted,
	kMiscStat_MinesDisarmed,
	kMiscStat_SpeechSuccesses,
	kMiscStat_PocketsPicked,
	kMiscStat_PantsExploded,
	kMiscStat_BooksRead,
	kMiscStat_HealthFromStimpaks,
	kMiscStat_WeaponsCreated,
	kMiscStat_HealthFromFood,
	kMiscStat_WaterConsumed,
	kMiscStat_SandmanKills,
	kMiscStat_ParalyzingPunches,
	kMiscStat_RobotsDisabled,
	kMiscStat_TimesSlept,
	kMiscStat_CorpsesEaten,
	kMiscStat_MysteriousStrangerVisits,
	kMiscStat_DoctorBagsUsed,
	kMiscStat_ChallengesCompleted,
	kMiscStat_MissFortunateOccurrences,
	kMiscStat_Disintegrations,
	kMiscStat_HaveLimbsCrippled,
	kMiscStat_SpeechFailures,
	kMiscStat_ItemsCrafted,
	kMiscStat_WeaponModifications,
	kMiscStat_ItemsRepaired,
	kMiscStat_TotalThingsKilled,
	kMiscStat_DismemberedLimbs,
	kMiscStat_CaravanGamesWon,
	kMiscStat_CaravanGamesLost,
	kMiscStat_BarterAmountTraded,
	kMiscStat_RouletteGamesPlayed,
	kMiscStat_BlackjackGamesPlayed,
	kMiscStat_SlotsGamesPlayed,
};

enum PerkEntryPointID
{
	kPerkEntry_CalculateWeaponDamage,
	kPerkEntry_CalculateMyCriticalHitChance,
	kPerkEntry_CalculateMyCriticalHitDamage,
	kPerkEntry_CalculateWeaponAttackAPCost,
	kPerkEntry_CalculateMineExplodeChance,
	kPerkEntry_AdjustRangePenalty,
	kPerkEntry_AdjustLimbDamage,
	kPerkEntry_CalculateWeaponRange,
	kPerkEntry_CalculateToHitChance,
	kPerkEntry_AdjustExperiencePoints,
	kPerkEntry_AdjustGainedSkillPoints,
	kPerkEntry_AdjustBookSkillPoints,
	kPerkEntry_ModifyRecoveredHealth,
	kPerkEntry_CalculateInventoryAPCost,
	kPerkEntry_GetDisposition,
	kPerkEntry_GetShouldAttack,
	kPerkEntry_GetShouldAssist,
	kPerkEntry_CalculateBuyPrice,
	kPerkEntry_GetBadKarma,
	kPerkEntry_GetGoodKarma,
	kPerkEntry_IgnoreLockedTerminal,
	kPerkEntry_AddLeveledListOnDeath,
	kPerkEntry_GetMaxCarryWeight,
	kPerkEntry_ModifyAddictionChance,
	kPerkEntry_ModifyAddictionDuration,
	kPerkEntry_ModifyPositiveChemDuration,
	kPerkEntry_AdjustDrinkingRadiation,
	kPerkEntry_Activate,
	kPerkEntry_MysteriousStranger,
	kPerkEntry_HasParalyzingPalm,
	kPerkEntry_HackingScienceBonus,
	kPerkEntry_IgnoreRunningDuringDetection,
	kPerkEntry_IgnoreBrokenLock,
	kPerkEntry_HasConcentratedFire,
	kPerkEntry_CalculateGunSpread,
	kPerkEntry_PlayerKillAPReward,
	kPerkEntry_ModifyEnemyCriticalHitChance,
	kPerkEntry_ReloadSpeed,
	kPerkEntry_EquipSpeed,
	kPerkEntry_ActionPointRegen,
	kPerkEntry_ActionPointCost,
	kPerkEntry_MissFortune,
	kPerkEntry_ModifyRunSpeed,
	kPerkEntry_ModifyAttackSpeed,
	kPerkEntry_ModifyRadiationConsumed,
	kPerkEntry_HasPipHacker,
	kPerkEntry_HasMeltdown,
	kPerkEntry_SeeEnemyHealth,
	kPerkEntry_HasJuryRigging,
	kPerkEntry_ModifyThreatRange,
	kPerkEntry_ModifyThread,
	kPerkEntry_HasFastTravelAlways,
	kPerkEntry_KnockdownChance,
	kPerkEntry_ModifyWeaponStrengthReq,
	kPerkEntry_ModifyAimingMoveSpeed,
	kPerkEntry_ModifyLightItems,
	kPerkEntry_ModifyDamageThresholdDefender,
	kPerkEntry_ModifyChanceforAmmoItem,
	kPerkEntry_ModifyDamageThresholdAttacker,
	kPerkEntry_ModifyThrowingVelocity,
	kPerkEntry_ChanceforItemonFire,
	kPerkEntry_HasUnarmedForwardPowerAttack,
	kPerkEntry_HasUnarmedBackPowerAttack,
	kPerkEntry_HasUnarmedCrouchedPowerAttack,
	kPerkEntry_HasUnarmedCounterAttack,
	kPerkEntry_HasUnarmedLeftPowerAttack,
	kPerkEntry_HasUnarmedRightPowerAttack,
	kPerkEntry_VATSHelperChance,
	kPerkEntry_ModifyItemDamage,
	kPerkEntry_HasImprovedDetection,
	kPerkEntry_HasImprovedSpotting,
	kPerkEntry_HasImprovedItemDetection,
	kPerkEntry_AdjustExplosionRadius,
	kPerkEntry_AdjustHeavyWeaponWeight
};

enum SpecialInputCode
{
	kInputCode_Backspace =	0x80000000,
	kInputCode_ArrowLeft =	0x80000001,
	kInputCode_ArrowRight =	0x80000002,
	kInputCode_ArrowUp =	0x80000003,
	kInputCode_ArrowDown =	0x80000004,
	kInputCode_Home =		0x80000005,
	kInputCode_End =		0x80000006,
	kInputCode_Delete =		0x80000007,
	kInputCode_Enter =		0x80000008,
	kInputCode_PageUp =		0x80000009,
	kInputCode_PageDown =	0x8000000A
};