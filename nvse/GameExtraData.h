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
	kExtraData_Unknown00,
	kExtraData_Havok,
	kExtraData_Cell3D,
	kExtraData_CellWaterType,
	kExtraData_RegionList,
	kExtraData_SeenData,
	kExtraData_EditorID,
	kExtraData_CellMusicType,
	kExtraData_CellClimate,
	kExtraData_ProcessMiddleLow,
	kExtraData_CellCanopyShadowMask,
	kExtraData_DetachTime,
	kExtraData_PersistentCell,
	kExtraData_Script,
	kExtraData_Action,
	kExtraData_StartingPosition,
	kExtraData_Anim,
	kExtraData_Unknown11,
	kExtraData_UsedMarkers,
	kExtraData_DistantData,
	kExtraData_RagdollData,
	kExtraData_ContainerChanges,
	kExtraData_Worn,
	kExtraData_WornLeft,
	kExtraData_PackageStartLocation,
	kExtraData_Package,
	kExtraData_TrespassPackage,
	kExtraData_RunOncePacks,
	kExtraData_ReferencePointer,
	kExtraData_Follower,
	kExtraData_LevCreaModifier,
	kExtraData_Ghost,
	kExtraData_OriginalReference,
	kExtraData_Ownership,
	kExtraData_Global,
	kExtraData_Rank,
	kExtraData_Count,
	kExtraData_Health,
	kExtraData_Uses,
	kExtraData_TimeLeft,
	kExtraData_Charge,
	kExtraData_Light,
	kExtraData_Lock,
	kExtraData_Teleport,
	kExtraData_MapMarker,
	kExtraData_Unknown2D,
	kExtraData_LeveledCreature,
	kExtraData_LeveledItem,
	kExtraData_Scale,
	kExtraData_Seed,
	kExtraData_NonActorMagicCaster,
	kExtraData_NonActorMagicTarget,
	kExtraData_Unknown34,
	kExtraData_PlayerCrimeList,
	kExtraData_Unknown36,
	kExtraData_EnableStateParent,
	kExtraData_EnableStateChildren,
	kExtraData_ItemDropper,
	kExtraData_DroppedItemList,
	kExtraData_RandomTeleportMarker,
	kExtraData_MerchantContainer,
	kExtraData_SavedHavokData,
	kExtraData_CannotWear,
	kExtraData_Poison,
	kExtraData_Unknown40,
	kExtraData_LastFinishedSequence,
	kExtraData_SavedAnimation,
	kExtraData_NorthRotation,
	kExtraData_XTarget,
	kExtraData_FriendHits,
	kExtraData_HeadingTarget,
	kExtraData_Unknown47,
	kExtraData_RefractionProperty,
	kExtraData_StartingWorldOrCell,
	kExtraData_Hotkey,
	kExtraData_Unknown4B,
	kExtraData_EditorRefMovedData,
	kExtraData_InfoGeneralTopic,
	kExtraData_HasNoRumors,
	kExtraData_Sound,
	kExtraData_TerminalState,
	kExtraData_LinkedRef,
	kExtraData_LinkedRefChildren,
	kExtraData_ActivateRef,
	kExtraData_ActivateRefChildren,
	kExtraData_TalkingActor,
	kExtraData_ObjectHealth,
	kExtraData_DecalRefs,
	kExtraData_Unknown58,
	kExtraData_CellImageSpace,
	kExtraData_NavMeshPortal,
	kExtraData_ModelSwap,
	kExtraData_Radius,
	kExtraData_Radiation,
	kExtraData_FactionChanges,
	kExtraData_DismemberedLimbs,
	kExtraData_ActorCause,
	kExtraData_MultiBound,
	kExtraData_MultiBoundData,
	kExtraData_MultiBoundRef,
	kExtraData_Unknown64,
	kExtraData_ReflectedRefs,
	kExtraData_ReflectorRefs,
	kExtraData_EmittanceSource,
	kExtraData_RadioData,
	kExtraData_CombatStyle,
	kExtraData_Unknown6A,
	kExtraData_Primitive,
	kExtraData_OpenCloseActivateRef,
	kExtraData_AnimNoteReciever,
	kExtraData_Ammo,
	kExtraData_PatrolRefData,
	kExtraData_PackageData,
	kExtraData_OcclusionPlane,
	kExtraData_CollisionData,
	kExtraData_SayTopicInfoOnceADay,
	kExtraData_EncounterZone,
	kExtraData_SayToTopicInfo,
	kExtraData_OcclusionPlaneRefData,
	kExtraData_PortalRefData,
	kExtraData_Portal,
	kExtraData_Room,
	kExtraData_HealthPerc,
	kExtraData_RoomRefData,
	kExtraData_GuardedRefData,
	kExtraData_CreatureAwakeSound,
	kExtraData_WaterZoneMap,
	kExtraData_Unknown7F,
	kExtraData_IgnoredBySandbox,
	kExtraData_CellAcousticSpace,
	kExtraData_ReservedMarkers,
	kExtraData_WeaponIdleSound,
	kExtraData_WaterLightRefs,
	kExtraData_LitWaterRefs,
	kExtraData_WeaponAttackSound,
	kExtraData_ActivateLoopSound,
	kExtraData_PatrolRefInUseData,
	kExtraData_AshPileRef,
	kExtraData_CreatureMovementSound,
	kExtraData_FollowerSwimBreadcrumbs,
	kExtraData_CellImpactSwap,
	kExtraData_WeaponModFlags,
	kExtraData_ModdingItem,
	kExtraData_SecuritronFace,
	kExtraData_AudioMarker,
	kExtraData_AudioBuoyMarker,
	kExtraData_SpecialRenderFlags,
	kExtraData_Max
};

#define GetExtraType(xDataList, Type) (Extra ## Type*)(xDataList)->GetByType(kExtraData_ ## Type)
extern const char *GetExtraDataName(UInt8 extraDataType);

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

	void DebugDump();
	void Cleanup();	// clean up unneeded extra data from each EntryData

	static ExtraContainerChanges* Create();

	EntryDataList *GetEntryDataList() const {return data ? data->objList : NULL;}
};
typedef ExtraContainerChanges::EntryData ContChangesEntry;

// Finds an ExtraDataList in an ExtendDataList
class ExtraDataListInExtendDataListMatcher
{
	ExtraDataList* m_toMatch;
public:
	ExtraDataListInExtendDataListMatcher(ExtraDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraDataList* match)
	{
		return (m_toMatch == match);
	}
};

// Finds an ExtraDataList in an ExtendDataList embedded in one of the EntryData from an EntryDataList
class ExtraDataListInEntryDataListMatcher
{
	ExtraDataList* m_toMatch;
public:
	ExtraDataListInEntryDataListMatcher(ExtraDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		if (match && match->extendData)
			return match->extendData->GetIndexOf(ExtraDataListInExtendDataListMatcher(m_toMatch))>=0;
		else
			return false;
	}
};

// Finds an ExtendDataList in an EntryDataList
class ExtendDataListInEntryDataListMatcher
{
	ExtraContainerChanges::ExtendDataList* m_toMatch;
public:
	ExtendDataListInEntryDataListMatcher(ExtraContainerChanges::ExtendDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		if (match && match->extendData)
			return (match->extendData == m_toMatch);
		else
			return false;
	}
};

// Finds an EntryData in an EntryDataList
class EntryDataInEntryDataListMatcher
{
	ExtraContainerChanges::EntryData* m_toMatch;
public:
	EntryDataInEntryDataListMatcher(ExtraContainerChanges::EntryData* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (m_toMatch == match);
	}
};

// Finds an Item (type) in an EntryDataList
class ItemInEntryDataListMatcher
{
	TESForm* m_toMatch;
public:
	ItemInEntryDataListMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && m_toMatch == match->type);
	}
};

// Finds an Item from its base form in an EntryDataList
class BaseInEntryDataLastMatcher
{
	TESForm* m_toMatch;
public:
	BaseInEntryDataLastMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && match->type && m_toMatch == match->type->TryGetREFRParent());
	}
};

// Finds an item by refID in an EntryDataList
class RefIDInEntryDataListMatcher
{
	UInt32 m_toMatch;
public:
	RefIDInEntryDataListMatcher(UInt32 match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && match->type && m_toMatch == match->type->refID);
	}
};

// Finds an item by the refID of its base form in an EntryDataList
class BaseIDInEntryDataListMatcher
{
	UInt32 m_toMatch;
public:
	BaseIDInEntryDataListMatcher(UInt32 match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && match->type && match->type->TryGetREFRParent() && m_toMatch == match->type->TryGetREFRParent()->refID);
	}
};

extern ExtraContainerChanges::ExtendDataList* ExtraContainerChangesExtendDataListCreate(ExtraDataList* pExtraDataList = NULL);
extern void ExtraContainerChangesExtendDataListFree(ExtraContainerChanges::ExtendDataList* xData, bool bFreeList = false);

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

	void DebugDump();

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

ExtraFactionChanges::FactionListEntry* GetExtraFactionList(BaseExtraList& xDataList);
void SetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction, char rank);

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