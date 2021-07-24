#pragma once

/*    Class							     vtbl	  Type  Size
 *   ----------------------------		------		--  --
 *	ExtraAction                        ????????		0E	14
 *	ExtraActivateLoopSound             ????????		87	18
 *	ExtraActivateRef                   ????????		53	18
 *	ExtraActivateRefChildren           ????????		54	18
 *	ExtraAmmo                          ????????		6E	14
 *	ExtraAnim                          ????????		10	10
 *	ExtraAshPileRef                    ????????		89	10
 *	ExtraCannotWear                    ????????		3E	0C	// no data
 *	ExtraCell3D                        ????????		2	10
 *	ExtraCellAcousticSpace             ????????		81	10
 *	ExtraCellCanopyShadowMask          ????????		0A	1C
 *	ExtraCellClimate                   ????????		8	10
 *	ExtraCellImageSpace                ????????		59	10
 *	ExtraCellMusicType                 ????????		7	10
 *	ExtraCellWaterType                 ????????		3	10
 *	ExtraCharge                        ????????		28	10
 *	ExtraCollisionData                 ????????		72	10
 *	ExtraCombatStyle                   ????????		69	10
 *	ExtraContainerChanges              ????????		15	10
 *	ExtraCount                         ????????		24	10
 *	ExtraCreatureAwakeSound            ????????		7D	18
 *	ExtraCreatureMovementSound         ????????		8A	18
 *	ExtraDecalRefs                     ????????		57	14
 *	ExtraDetachTime                    ????????		0B	10
 *	ExtraDismemberedLimbs              ????????		5F	30
 *	ExtraDistantData                   ????????		13	18
 *	ExtraDroppedItemList               ????????		3A	14
 *	ExtraEditorRefMovedData            ????????		4C	30
 *	ExtraEmittanceSource               ????????		67	10
 *	ExtraEnableStateChildren           ????????		38	14
 *	ExtraEnableStateParent             ????????		37	14
 *	ExtraEncounterZone                 ????????		74	10
 *	ExtraFactionChanges                ????????		5E	10
 *	ExtraFollower                      ????????		1D	10
 *	ExtraFollowerSwimBreadcrumbs       ????????		8B	28
 *	ExtraFriendHits                    ????????		45	1C
 *	ExtraGhost                         ????????		1F	0C	// no data
 *	ExtraGlobal                        ????????		22	10
 *	ExtraGuardedRefData                ????????		7C	1C
 *	ExtraHasNoRumors                   ????????		4E	10
 *	ExtraHavok                         ????????		1	14
 *	ExtraHeadingTarget                 ????????		46	10
 *	ExtraHealth                        ????????		25	10
 *	ExtraHealthPerc                    ????????		7A	10
 *	ExtraHotkey                        ????????		4A	10
 *	ExtraIgnoredBySandbox              ????????		80	0C	// no data
 *	ExtraInfoGeneralTopic              ????????		4D	10
 *	ExtraItemDropper                   ????????		39	10
 *	ExtraLastFinishedSequence          ????????		41	10
 *	ExtraLevCreaModifier               ????????		1E	10
 *	ExtraLeveledCreature               ????????		2E	14
 *	ExtraLeveledItem                   ????????		2F	14
 *	ExtraLight                         ????????		29	10
 *	ExtraLinkedRef                     ????????		51	10
 *	ExtraLinkedRefChildren             ????????		52	14
 *	ExtraLitWaterRefs                  ????????		85	14
 *	ExtraLock                          ????????		2A	10
 *	ExtraMapMarker                     ????????		2C	10
 *	ExtraMerchantContainer             ????????		3C	10
 *	ExtraModelSwap                     ????????		5B	14
 *	ExtraMultiBound                    ????????		61	10
 *	ExtraMultiBoundData                ????????		62	10
 *	ExtraMultiBoundRef                 ????????		63	10
 *	ExtraNavMeshPortal                 ????????		5A	14
 *	ExtraNorthRotation                 ????????		43	10
 *	ExtraObjectHealth                  ????????		56	10
 *	ExtraOcclusionPlane                ????????		71	10
 *	ExtraOcclusionPlaneRefData         ????????		76	10
 *	ExtraOpenCloseActivateRef          ????????		6C	10
 *	ExtraOriginalReference             ????????		20	10
 *	ExtraOwnership                     ????????		21	10
 *	ExtraPackage                       ????????		19	1C
 *	ExtraPackageData                   ????????		70	10
 *	ExtraPackageStartLocation          ????????		18	1C
 *	ExtraPatrolRefData                 ????????		6F	10
 *	ExtraPatrolRefInUseData            ????????		88	10
 *	ExtraPersistentCell                ????????		0C	10
 *	ExtraPlayerCrimeList               ????????		35	10
 *	ExtraPoison                        ????????		3F	10
 *	ExtraPortal                        ????????		78	10
 *	ExtraPortalRefData                 ????????		77	10
 *	ExtraPrimitive                     ????????		6B	10
 *	ExtraProcessMiddleLow              ????????		9	10
 *	ExtraRadiation                     ????????		5D	10
 *	ExtraRadioData                     ????????		68	1C
 *	ExtraRadius                        ????????		5C	10
 *	ExtraRagdollData                   ????????		14	10
 *	ExtraRandomTeleportMarker          ????????		3B	10
 *	ExtraRank                          ????????		23	10
 *	ExtraReferencePointer              ????????		1C	10
 *	ExtraReflectedRefs                 ????????		65	14
 *	ExtraReflectorRefs                 ????????		66	14
 *	ExtraRefractionProperty            ????????		48	10
 *	ExtraRegionList                    ????????		4	10
 *	ExtraReservedMarkers               ????????		82	10
 *	ExtraRoom                          ????????		79	10
 *	ExtraRoomRefData                   ????????		7B	10
 *	ExtraRunOncePacks                  ????????		1B	10
 *	ExtraSavedAnimation                ????????		42	10
 *	ExtraSavedHavokData                ????????		3D	10
 *	ExtraSayToTopicInfo                ????????		75	18
 *	ExtraSayTopicInfoOnceADay          ????????		73	10
 *	ExtraScale                         ????????		30	10
 *	ExtraScript                        ????????		0D	14
 *	ExtraSeed                          ????????		31	10
 *	ExtraSeenData                      ????????		5	10
 *	ExtraSound                         ????????		4F	18
 *	ExtraStartingPosition              ????????		0F	24
 *	ExtraStartingWorldOrCell           ????????		49	10
 *	ExtraTalkingActor                  ????????		55	10
 *	ExtraTeleport                      ????????		2B	10
 *	ExtraTerminalState                 ????????		50	10
 *	ExtraTimeLeft                      ????????		27	10
 *	ExtraTrespassPackage               ????????		1A	10
 *	ExtraUsedMarkers                   ????????		12	10
 *	ExtraUses                          ????????		26	10
 *	ExtraWaterLightRefs                ????????		84	14
 *	ExtraWaterZoneMap                  ????????		7E	20
 *	ExtraWeaponAttackSound             ????????		86	18
 *	ExtraWeaponIdleSound               ????????		83	18
 *	ExtraWeaponModFlags                ????????		8D	10
 *	ExtraWorn                          ????????		16	0C	// no data
 *	ExtraWornLeft                      ????????		17	0C	// no data
 *	ExtraXTarget                       ????????		44	10
 */

/* BaseExtraList methods:
	AddExtra		0x0040A180
	GetByType		0x0040A320, pass typeID
	ItemsInList		0x0040A130
	RemoveExtra		0x0040A250

  ExtraDataList methods:
	DuplicateExtraListForContainer	0x0041B090
*/

enum
{
	/*00*/kExtraData_Unknown00,
	/*01*/kExtraData_Havok,
	/*02*/kExtraData_Cell3D,
	/*03*/kExtraData_CellWaterType,
	/*04*/kExtraData_RegionList,
	/*05*/kExtraData_SeenData,
	/*06*/kExtraData_EditorID,
	/*07*/kExtraData_CellMusicType,
	/*08*/kExtraData_CellClimate,
	/*09*/kExtraData_ProcessMiddleLow,
	/*0A*/kExtraData_CellCanopyShadowMask,
	/*0B*/kExtraData_DetachTime,
	/*0C*/kExtraData_PersistentCell,
	/*0D*/kExtraData_Script,
	/*0E*/kExtraData_Action,
	/*0F*/kExtraData_StartingPosition,
	/*10*/kExtraData_Anim,
	/*11*/kExtraData_Unknown11,
	/*12*/kExtraData_UsedMarkers,
	/*13*/kExtraData_DistantData,
	/*14*/kExtraData_RagdollData,
	/*15*/kExtraData_ContainerChanges,
	/*16*/kExtraData_Worn,
	/*17*/kExtraData_WornLeft,
	/*18*/kExtraData_PackageStartLocation,
	/*19*/kExtraData_Package,
	/*1A*/kExtraData_TrespassPackage,
	/*1B*/kExtraData_RunOncePacks,
	/*1C*/kExtraData_ReferencePointer,
	/*1D*/kExtraData_Follower,
	/*1E*/kExtraData_LevCreaModifier,
	/*1F*/kExtraData_Ghost,
	/*20*/kExtraData_OriginalReference,
	/*21*/kExtraData_Ownership,
	/*22*/kExtraData_Global,
	/*23*/kExtraData_Rank,
	/*24*/kExtraData_Count,
	/*25*/kExtraData_Health,
	/*26*/kExtraData_Uses,
	/*27*/kExtraData_TimeLeft,
	/*28*/kExtraData_Charge,
	/*29*/kExtraData_Light,
	/*2A*/kExtraData_Lock,
	/*2B*/kExtraData_Teleport,
	/*2C*/kExtraData_MapMarker,
	/*2D*/kExtraData_Unknown2D,
	/*2E*/kExtraData_LeveledCreature,
	/*2F*/kExtraData_LeveledItem,
	/*30*/kExtraData_Scale,
	/*31*/kExtraData_Seed,
	/*32*/kExtraData_NonActorMagicCaster,
	/*33*/kExtraData_NonActorMagicTarget,
	/*34*/kExtraData_Unknown34,
	/*35*/kExtraData_PlayerCrimeList,
	/*36*/kExtraData_Unknown36,
	/*37*/kExtraData_EnableStateParent,
	/*38*/kExtraData_EnableStateChildren,
	/*39*/kExtraData_ItemDropper,
	/*3A*/kExtraData_DroppedItemList,
	/*3B*/kExtraData_RandomTeleportMarker,
	/*3C*/kExtraData_MerchantContainer,
	/*3D*/kExtraData_SavedHavokData,
	/*3E*/kExtraData_CannotWear,
	/*3F*/kExtraData_Poison,
	/*40*/kExtraData_Unknown40,
	/*41*/kExtraData_LastFinishedSequence,
	/*42*/kExtraData_SavedAnimation,
	/*43*/kExtraData_NorthRotation,
	/*44*/kExtraData_XTarget,
	/*45*/kExtraData_FriendHits,
	/*46*/kExtraData_HeadingTarget,
	/*47*/kExtraData_Unknown47,
	/*48*/kExtraData_RefractionProperty,
	/*49*/kExtraData_StartingWorldOrCell,
	/*4A*/kExtraData_Hotkey,
	/*4B*/kExtraData_Unknown4B,
	/*4C*/kExtraData_EditorRefMovedData,
	/*4D*/kExtraData_InfoGeneralTopic,
	/*4E*/kExtraData_HasNoRumors,
	/*4F*/kExtraData_Sound,
	/*50*/kExtraData_TerminalState,
	/*51*/kExtraData_LinkedRef,
	/*52*/kExtraData_LinkedRefChildren,
	/*53*/kExtraData_ActivateRef,
	/*54*/kExtraData_ActivateRefChildren,
	/*55*/kExtraData_TalkingActor,
	/*56*/kExtraData_ObjectHealth,
	/*57*/kExtraData_DecalRefs,
	/*58*/kExtraData_Unknown58,
	/*59*/kExtraData_CellImageSpace,
	/*5A*/kExtraData_NavMeshPortal,
	/*5B*/kExtraData_ModelSwap,
	/*5C*/kExtraData_Radius,
	/*5D*/kExtraData_Radiation,
	/*5E*/kExtraData_FactionChanges,
	/*5F*/kExtraData_DismemberedLimbs,
	/*60*/kExtraData_ActorCause,
	/*61*/kExtraData_MultiBound,
	/*62*/kExtraData_MultiBoundData,
	/*63*/kExtraData_MultiBoundRef,
	/*64*/kExtraData_Unknown64,
	/*65*/kExtraData_ReflectedRefs,
	/*66*/kExtraData_ReflectorRefs,
	/*67*/kExtraData_EmittanceSource,
	/*68*/kExtraData_RadioData,
	/*69*/kExtraData_CombatStyle,
	/*6A*/kExtraData_Unknown6A,
	/*6B*/kExtraData_Primitive,
	/*6C*/kExtraData_OpenCloseActivateRef,
	/*6D*/kExtraData_AnimNoteReciever,
	/*6E*/kExtraData_Ammo,
	/*6F*/kExtraData_PatrolRefData,
	/*70*/kExtraData_PackageData,
	/*71*/kExtraData_OcclusionPlane,
	/*72*/kExtraData_CollisionData,
	/*73*/kExtraData_SayTopicInfoOnceADay,
	/*74*/kExtraData_EncounterZone,
	/*75*/kExtraData_SayToTopicInfo,
	/*76*/kExtraData_OcclusionPlaneRefData,
	/*77*/kExtraData_PortalRefData,
	/*78*/kExtraData_Portal,
	/*79*/kExtraData_Room,
	/*7A*/kExtraData_HealthPerc,
	/*7B*/kExtraData_RoomRefData,
	/*7C*/kExtraData_GuardedRefData,
	/*7D*/kExtraData_CreatureAwakeSound,
	/*7E*/kExtraData_WaterZoneMap,
	/*7F*/kExtraData_Unknown7F,
	/*80*/kExtraData_IgnoredBySandbox,
	/*81*/kExtraData_CellAcousticSpace,
	/*82*/kExtraData_ReservedMarkers,
	/*83*/kExtraData_WeaponIdleSound,
	/*84*/kExtraData_WaterLightRefs,
	/*85*/kExtraData_LitWaterRefs,
	/*86*/kExtraData_WeaponAttackSound,
	/*87*/kExtraData_ActivateLoopSound,
	/*88*/kExtraData_PatrolRefInUseData,
	/*89*/kExtraData_AshPileRef,
	/*8A*/kExtraData_CreatureMovementSound,
	/*8B*/kExtraData_FollowerSwimBreadcrumbs,
	/*8C*/kExtraData_CellImpactSwap,
	/*8D*/kExtraData_WeaponModFlags,
	/*8E*/kExtraData_ModdingItem,
	/*8F*/kExtraData_SecuritronFace,
	/*90*/kExtraData_AudioMarker,
	/*91*/kExtraData_AudioBuoyMarker,
	/*92*/kExtraData_SpecialRenderFlags,
	kExtraData_Max
};

#define GetExtraType(xDataList, Type) (Extra ## Type*)(xDataList)->GetByType(kExtraData_ ## Type)
const char *GetExtraDataName(UInt8 extraDataType);

// 14
class ExtraAction : public BSExtraData
{
public:
	UInt8			byte0C;		// 0C	some kind of status or flags
	UInt8			pad0D[3];	// 0D
	TESObjectREFR	*actionRef;	// 10

	static ExtraAction* __stdcall Create(TESObjectREFR *_actionRef = NULL);
};

// 14
class ExtraScript : public BSExtraData
{
public:
	Script			*script;	// 0C
	ScriptEventList	*eventList;	// 10

	static ExtraScript* __stdcall Create(Script *pScript);
};

// 10
class ExtraContainerChanges : public BSExtraData
{
public:
	class ExtendDataList : public tList<ExtraDataList>
	{
	public:
		void Clear();
		void CleanEmpty();
	};

	// 0C
	struct EntryData
	{
		ExtendDataList	*extendData;	// 00
		SInt32			countDelta;		// 04
		TESForm			*type;			// 08

		EntryData(ExtendDataList *extend, SInt32 count, TESForm *item) :
			extendData(extend), countDelta(count), type(item) {}

		__forceinline void Destroy()
		{
			ThisCall(0x4459E0, this, 1);
		}
		void Cleanup();
		bool __fastcall HasExtraType(UInt8 xType);
		bool HasExtraLeveledItem();
		ExtraDataList *GetEquippedExtra();
		float __vectorcall GetWeaponModEffectValue(UInt32 effectType);
		float __vectorcall GetBaseHealth();
		float GetHealthPercent();
		float CalculateWeaponDamage(Actor *owner, float condition, TESForm *ammo);
	};

	struct EntryDataList : tList<EntryData>
	{
		EntryData* __fastcall FindForItem(TESForm *item);
	};

	// 14
	struct Data
	{
		EntryDataList	*objList;		// 00
		TESObjectREFR	*owner;			// 04
		float			totalWgCurrent;	// 08
		float			totalWgLast;	// 0C
		UInt8			byte10;			// 10	referenced in relation to scripts in container
		UInt8			pad11[3];		// 11

		static Data *Create(TESObjectREFR *owner);
		float GetInventoryWeight();
	};

	Data	*data;	// 0C

	void Cleanup();	// clean up unneeded extra data from each EntryData

	static ExtraContainerChanges* Create();

	EntryDataList *GetEntryDataList() const {return data ? data->objList : NULL;}
};
typedef ExtraContainerChanges::EntryData ContChangesEntry;

// 10
class ExtraHealth : public BSExtraData
{
public:
	float		health;		// 0C

	static ExtraHealth* __stdcall Create(float _health = 0);
};

// 0C
class ExtraWorn : public BSExtraData	// Item is equipped
{
public:
	static ExtraWorn* Create();
};

// 0C
class ExtraWornLeft : public BSExtraData	// haven't seen used yet
{
public:
};

// 0C
class ExtraCannotWear : public BSExtraData	//	Seen used as ForceEquip ! Unused as forceUnequip (bug?)
{
public:
	static ExtraCannotWear* Create();
};

// 10
class ExtraHotkey : public BSExtraData
{
public:
	UInt8		index;		// 0C (is 0-7)
	UInt8		pad0D[3];	// 0D

	static ExtraHotkey* __stdcall Create(UInt8 _index = 0);
};

// 10
class ExtraCount : public BSExtraData
{
public:
	SInt16		count;		// 0C
	UInt8		pad0E[2];	// 0E

	static ExtraCount* __stdcall Create(UInt32 count = 0);
};

// 10
class ExtraLock : public BSExtraData
{
public:
	struct Data
	{
		UInt32	lockLevel;	// 00
		TESKey	*key;		// 04
		UInt8	flags;		// 08
		UInt8	pad09[3];	// 09
		UInt32  unk0C;		// 0C introduced since form version 0x10
		UInt32	unk10;		// 10
	};

	Data	*data;	// 0C

	static ExtraLock* Create();
};

// 10
class ExtraTeleport : public BSExtraData
{
public:
	struct Data
	{
		TESObjectREFR	*linkedDoor;	// 00
		float			x;				// 04 x, y, z, zRot refer to teleport marker's position and rotation
		float			y;				// 08
		float			z;				// 0C
		float			xRot;			// 10 angles in radians. x generally 0
		float			yRot;			// 14 y generally -0.0, no reason to modify
		float			zRot;			// 18
		UInt8			unk1C;			// 1C
		UInt8			pad1D[3];		// 1D
	};

	Data		*data;	// 0C

	static ExtraTeleport* Create();
};

// 10
class ExtraRandomTeleportMarker : public BSExtraData
{
public:
	TESObjectREFR	*teleportRef;	// 0C
};

// 14
class ExtraAmmo : public BSExtraData
{
public:
	TESAmmo		*ammo;	// 0C
	UInt32		count;	// 10
};

// 10
class ExtraOwnership : public BSExtraData
{
public:
	TESForm		*owner;	// maybe this should be a union {TESFaction*; TESNPC*} but it would be more unwieldy to access and modify

	static ExtraOwnership* __stdcall Create(TESForm *_owner);
};

// 10
class ExtraRank : public BSExtraData
{
public:
	SInt32		rank; // 0C

	static ExtraRank* __stdcall Create(UInt32 _rank);
};

// 10
class ExtraGlobal : public BSExtraData
{								//ownership data, stored separately from ExtraOwnership
public:
	TESGlobal	*globalVar;	// 0C
};

// 10
class ExtraWeaponModFlags : public BSExtraData
{
public:
	UInt8		flags;		// 0C
	UInt8		pad0D[3];	// 0D

	static ExtraWeaponModFlags* __stdcall Create(UInt32 _flags);
};

// 10
class ExtraFactionChanges : public BSExtraData
{
public:
	typedef tList<FactionListData> FactionListEntry;
	FactionListEntry	*data;		// 0C

	static ExtraFactionChanges* Create();
};

class ExtraFactionChangesMatcher
{
	TESFaction* pFaction;
	ExtraFactionChanges* xFactionChanges;
public:
	ExtraFactionChangesMatcher(TESFaction* faction, ExtraFactionChanges* FactionChanges) : pFaction(faction), xFactionChanges(FactionChanges) {}
	bool Accept(FactionListData *data) {
		return (data->faction == pFaction) ? true : false;
	}
};

// 14
class ExtraLeveledCreature : public BSExtraData
{
public:
	TESActorBase	*baseForm;	// 0C
	TESActorBase	*actorBase;	// 10
};

// PackageStartLocation = Worldspace or Cell / PosX / PosY / PosZ / and 4 bytes

// 10
class ExtraCombatStyle : public BSExtraData
{
public:
	TESCombatStyle	*combatStyle;	// 0C
};

// 10
class ExtraReferencePointer : public BSExtraData
{
public:
	TESObjectREFR	*refr;	// 0C
};

// 10
class ExtraMapMarker : public BSExtraData
{
public:
	enum
	{
		kFlag_Visible =		1,        // shown on the world map
		kFlag_CanTravel =	2,        // visited, can fast-travel to it
		kFlag_Hidden =		4,        // does not appear with Explorer perk
	};
    enum
    {
		kType_None,                // this determines the icon on the world map
		kType_City,
		kType_Settlement,
		kType_Encampment,
		kType_NaturalLandmark,
		kType_Cave,
		kType_Factory,
		kType_Memorial,
		kType_Military,
		kType_Office,
		kType_TownRuins,
		kType_UrbanRuins,
		kType_SewerRuins,
		kType_Metro,
		kType_Vault
	};

	struct MarkerData
	{
		TESFullName		fullName;		// 00
		UInt16			flags;			// 0C
		UInt16			type;			// 0E
		TESForm			*reputation;	// 10
	};
	MarkerData		*data;	// 0C

	bool IsVisible() {return (data->flags & kFlag_Visible) != 0;}
	bool CanTravel() {return (data->flags & kFlag_CanTravel) != 0;}
	bool IsHidden() {return (data->flags & kFlag_Hidden) != 0;}
	void SetVisible(bool visible)
	{
		if (visible) data->flags |= kFlag_Visible;
		else data->flags &= ~kFlag_Visible;
	}
	void SetCanTravel(bool travel)
	{
		if (travel) data->flags |= kFlag_CanTravel;
		else data->flags &= ~kFlag_CanTravel;
	}
	void SetHidden(bool hidden)
	{
		if (hidden) data->flags |= kFlag_Hidden;
		else data->flags &= ~kFlag_Hidden;
	}
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

// 14
class ExtraSpecialRenderFlags : public BSExtraData
{
public:
	UInt32		flags;		// 0C
	UInt32		unk10;		// 10

	static ExtraSpecialRenderFlags* __stdcall Create(UInt32 _flags = 0);
};