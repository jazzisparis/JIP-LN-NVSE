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
	/*00*/kXData_ExtraUnknown00,
	/*01*/kXData_ExtraHavok,
	/*02*/kXData_ExtraCell3D,
	/*03*/kXData_ExtraCellWaterType,
	/*04*/kXData_ExtraRegionList,
	/*05*/kXData_ExtraSeenData,
	/*06*/kXData_ExtraEditorID,
	/*07*/kXData_ExtraCellMusicType,
	/*08*/kXData_ExtraCellClimate,
	/*09*/kXData_ExtraProcessMiddleLow,
	/*0A*/kXData_ExtraCellCanopyShadowMask,
	/*0B*/kXData_ExtraDetachTime,
	/*0C*/kXData_ExtraPersistentCell,
	/*0D*/kXData_ExtraScript,
	/*0E*/kXData_ExtraAction,
	/*0F*/kXData_ExtraStartingPosition,
	/*10*/kXData_ExtraAnim,
	/*11*/kXData_ExtraUnknown11,
	/*12*/kXData_ExtraUsedMarkers,
	/*13*/kXData_ExtraDistantData,
	/*14*/kXData_ExtraRagdollData,
	/*15*/kXData_ExtraContainerChanges,
	/*16*/kXData_ExtraWorn,
	/*17*/kXData_ExtraWornLeft,
	/*18*/kXData_ExtraPackageStartLocation,
	/*19*/kXData_ExtraPackage,
	/*1A*/kXData_ExtraTrespassPackage,
	/*1B*/kXData_ExtraRunOncePacks,
	/*1C*/kXData_ExtraReferencePointer,
	/*1D*/kXData_ExtraFollower,
	/*1E*/kXData_ExtraLevCreaModifier,
	/*1F*/kXData_ExtraGhost,
	/*20*/kXData_ExtraOriginalReference,
	/*21*/kXData_ExtraOwnership,
	/*22*/kXData_ExtraGlobal,
	/*23*/kXData_ExtraRank,
	/*24*/kXData_ExtraCount,
	/*25*/kXData_ExtraHealth,
	/*26*/kXData_ExtraUses,
	/*27*/kXData_ExtraTimeLeft,
	/*27*/kXData_ExtraJIP = kXData_ExtraTimeLeft,
	/*28*/kXData_ExtraCharge,
	/*29*/kXData_ExtraLight,
	/*2A*/kXData_ExtraLock,
	/*2B*/kXData_ExtraTeleport,
	/*2C*/kXData_ExtraMapMarker,
	/*2D*/kXData_ExtraUnknown2D,
	/*2E*/kXData_ExtraLeveledCreature,
	/*2F*/kXData_ExtraLeveledItem,
	/*30*/kXData_ExtraScale,
	/*31*/kXData_ExtraSeed,
	/*32*/kXData_ExtraNonActorMagicCaster,
	/*33*/kXData_ExtraNonActorMagicTarget,
	/*34*/kXData_ExtraUnknown34,
	/*35*/kXData_ExtraPlayerCrimeList,
	/*36*/kXData_ExtraUnknown36,
	/*37*/kXData_ExtraEnableStateParent,
	/*38*/kXData_ExtraEnableStateChildren,
	/*39*/kXData_ExtraItemDropper,
	/*3A*/kXData_ExtraDroppedItemList,
	/*3B*/kXData_ExtraRandomTeleportMarker,
	/*3C*/kXData_ExtraMerchantContainer,
	/*3D*/kXData_ExtraSavedHavokData,
	/*3E*/kXData_ExtraCannotWear,
	/*3F*/kXData_ExtraPoison,
	/*40*/kXData_ExtraUnknown40,
	/*41*/kXData_ExtraLastFinishedSequence,
	/*42*/kXData_ExtraSavedAnimation,
	/*43*/kXData_ExtraNorthRotation,
	/*44*/kXData_ExtraXTarget,
	/*45*/kXData_ExtraFriendHits,
	/*46*/kXData_ExtraHeadingTarget,
	/*47*/kXData_ExtraUnknown47,
	/*48*/kXData_ExtraRefractionProperty,
	/*49*/kXData_ExtraStartingWorldOrCell,
	/*4A*/kXData_ExtraHotkey,
	/*4B*/kXData_ExtraUnknown4B,
	/*4C*/kXData_ExtraEditorRefMovedData,
	/*4D*/kXData_ExtraInfoGeneralTopic,
	/*4E*/kXData_ExtraHasNoRumors,
	/*4F*/kXData_ExtraSound,
	/*50*/kXData_ExtraTerminalState,
	/*51*/kXData_ExtraLinkedRef,
	/*52*/kXData_ExtraLinkedRefChildren,
	/*53*/kXData_ExtraActivateRef,
	/*54*/kXData_ExtraActivateRefChildren,
	/*55*/kXData_ExtraTalkingActor,
	/*56*/kXData_ExtraObjectHealth,
	/*57*/kXData_ExtraDecalRefs,
	/*58*/kXData_ExtraUnknown58,
	/*59*/kXData_ExtraCellImageSpace,
	/*5A*/kXData_ExtraNavMeshPortal,
	/*5B*/kXData_ExtraModelSwap,
	/*5C*/kXData_ExtraRadius,
	/*5D*/kXData_ExtraRadiation,
	/*5E*/kXData_ExtraFactionChanges,
	/*5F*/kXData_ExtraDismemberedLimbs,
	/*60*/kXData_ExtraActorCause,
	/*61*/kXData_ExtraMultiBound,
	/*62*/kXData_ExtraMultiBoundData,
	/*63*/kXData_ExtraMultiBoundRef,
	/*64*/kXData_ExtraUnknown64,
	/*65*/kXData_ExtraReflectedRefs,
	/*66*/kXData_ExtraReflectorRefs,
	/*67*/kXData_ExtraEmittanceSource,
	/*68*/kXData_ExtraRadioData,
	/*69*/kXData_ExtraCombatStyle,
	/*6A*/kXData_ExtraUnknown6A,
	/*6B*/kXData_ExtraPrimitive,
	/*6C*/kXData_ExtraOpenCloseActivateRef,
	/*6D*/kXData_ExtraAnimNoteReciever,
	/*6E*/kXData_ExtraAmmo,
	/*6F*/kXData_ExtraPatrolRefData,
	/*70*/kXData_ExtraPackageData,
	/*71*/kXData_ExtraOcclusionPlane,
	/*72*/kXData_ExtraCollisionData,
	/*73*/kXData_ExtraSayTopicInfoOnceADay,
	/*74*/kXData_ExtraEncounterZone,
	/*75*/kXData_ExtraSayToTopicInfo,
	/*76*/kXData_ExtraOcclusionPlaneRefData,
	/*77*/kXData_ExtraPortalRefData,
	/*78*/kXData_ExtraPortal,
	/*79*/kXData_ExtraRoom,
	/*7A*/kXData_ExtraHealthPerc,
	/*7B*/kXData_ExtraRoomRefData,
	/*7C*/kXData_ExtraGuardedRefData,
	/*7D*/kXData_ExtraCreatureAwakeSound,
	/*7E*/kXData_ExtraWaterZoneMap,
	/*7F*/kXData_ExtraUnknown7F,
	/*80*/kXData_ExtraIgnoredBySandbox,
	/*81*/kXData_ExtraCellAcousticSpace,
	/*82*/kXData_ExtraReservedMarkers,
	/*83*/kXData_ExtraWeaponIdleSound,
	/*84*/kXData_ExtraWaterLightRefs,
	/*85*/kXData_ExtraLitWaterRefs,
	/*86*/kXData_ExtraWeaponAttackSound,
	/*87*/kXData_ExtraActivateLoopSound,
	/*88*/kXData_ExtraPatrolRefInUseData,
	/*89*/kXData_ExtraAshPileRef,
	/*8A*/kXData_ExtraCreatureMovementSound,
	/*8B*/kXData_ExtraFollowerSwimBreadcrumbs,
	/*8C*/kXData_ExtraCellImpactSwap,
	/*8D*/kXData_ExtraWeaponModFlags,
	/*8E*/kXData_ExtraModdingItem,
	/*8F*/kXData_ExtraSecuritronFace,
	/*90*/kXData_ExtraAudioMarker,
	/*91*/kXData_ExtraAudioBuoyMarker,
	/*92*/kXData_ExtraSpecialRenderFlags,
	kXData_ExtraMax
};

#define GetExtraType(xDataList, Type) (Type*)(xDataList)->GetByType(kXData_ ## Type)
#define CreateExtraType(Type) \
	UInt32 *dataPtr = (UInt32*)Game_HeapAlloc<Type>(); \
	dataPtr[0] = kVtbl_ ## Type; \
	dataPtr[1] = kXData_ ## Type; \
	dataPtr[2] = 0;

#define XDATA_CS ScopedLightCS cs((LightCS*)EXTRA_DATA_CS);

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
	ScriptLocals	*eventList;	// 10

	static ExtraScript* __stdcall Create(Script *pScript);
};

// 10
class ExtraContainerChanges : public BSExtraData
{
public:
	class ExtendDataList : public tList<ExtraDataList>
	{
	public:
		ExtendDataList(ExtraDataList *_xData = nullptr) : tList<ExtraDataList>(_xData) {}

		void Clear();
		void CleanEmpty();
	};

	// 0C
	struct EntryData
	{
		ExtendDataList	*extendData;	// 00
		SInt32			countDelta;		// 04
		union							// 08
		{
			TESForm			*type;
			TESObjectWEAP	*weapon;
			TESAmmo			*ammo;
			TESObjectARMO	*armor;
			AlchemyItem		*ingestible;
			TESObjectMISC	*misc;
			BGSNote			*note;

		};

		EntryData(ExtendDataList *extend, SInt32 count, TESForm *item) :
			extendData(extend), countDelta(count), type(item) {}

		static EntryData *Create(TESForm *item, SInt32 count, ExtraDataList *xData = nullptr);

		__forceinline void Destroy()
		{
			ThisCall(0x4459E0, this, 1);
		}
		ExtraDataList *CreateExtraData();
		bool __fastcall HasExtraType(UInt32 xType) const;
		bool HasExtraLeveledItem() const;
		ExtraDataList *GetEquippedExtra() const;
		SInt32 GetExtendDataCount() const;
		float __vectorcall GetWeaponModEffectValue(UInt32 effectType) const;
		float __vectorcall GetBaseHealth() const;
		float __vectorcall GetHealthPercent() const;
		float CalculateWeaponDamage(Actor *owner, float condition, TESForm *ammo) const;
		EntryData *CreateCopy(ExtraDataList *xData);
	};

	struct EntryDataList : tList<EntryData>
	{
		EntryData* __fastcall FindForItem(TESForm *item) const;
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
		double GetInventoryWeight() const;
	};

	Data	*data;	// 0C

	static ExtraContainerChanges* Create();

	EntryDataList *GetEntryDataList() const {return data ? data->objList : NULL;}
};
typedef ExtraContainerChanges::EntryData ContChangesEntry;
typedef ExtraContainerChanges::EntryDataList ContChangesEntryList;
typedef ExtraContainerChanges::ExtendDataList ContChangesExtraList;


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
	static ExtraWornLeft* Create();
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

	static ExtraCount* __stdcall Create(SInt32 count = 0);
};

// 10
class ExtraLock : public BSExtraData
{
public:
	struct Data
	{
		UInt8	lockLevel;	// 00
		UInt8	pad01[3];	// 01
		TESKey	*key;		// 04
		UInt8	flags;		// 08	1 - IsLocked
		UInt8	pad09[3];	// 09
		UInt32  unk0C;		// 0C	introduced since form version 0x10
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
		NiVector3		position;		// 04
		NiVector3		rotation;		// 10
		UInt8			byte1C;			// 1C
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
	TESFaction			*pFaction;
	ExtraFactionChanges	*xFactionChanges;

public:
	ExtraFactionChangesMatcher(TESFaction *faction, ExtraFactionChanges *FactionChanges) : pFaction(faction), xFactionChanges(FactionChanges) {}
	bool Accept(FactionListData *data) const {return data->faction == pFaction;}
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
		UInt8			flags;			// 0C
		UInt8			pad0D;			// 0D
		UInt16			type;			// 0E
		TESReputation	*reputation;	// 10

		const char *GetTexturePath() const {return (flags & 2) ? ((const char**)0x11A0404)[type] : (const char*)0x1075030;}
	};
	MarkerData		*data;	// 0C
};
typedef ExtraMapMarker::MarkerData MapMarkerData;

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

	BitField<16>	verticalSeenBits[16];	// 04	16x16 grid bitfield

	void Dump()
	{
		for (UInt32 i = 0; i < 16; i++)
			verticalSeenBits[i].Dump();
	}
};
static_assert(sizeof(SeenData) == 0x24);

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

	static ExtraUses *Create(UInt8 _uses);
};

// 10
class ExtraTimeLeft : public BSExtraData
{
public:
	float		timeLeft;		// 0C

	static ExtraTimeLeft *Create(float _timeLeft);
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
class ExtraEnableStateParent : public BSExtraData
{
public:
	enum EnableStateFlags
	{
		kEnableFlag_OppositeOfParent =	1,
		kEnableFlag_PopIn =				2
	};

	TESObjectREFR		*parentRef;		// 0C
	UInt8				flags;			// 10
	UInt8				pad11[3];		// 11
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

// 10
class ExtraLevCreaModifier : public BSExtraData
{
public:
	enum LevelMod
	{
		kLvlMod_Easy,
		kLvlMod_Medium,
		kLvlMod_Hard,
		kLvlMod_Boss,
		kLvlMod_None
	};

	UInt32			levelMod;	// 0C
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

// 20
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
	String              activationPromptOverride;
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

// 14
class ExtraModelSwap : public BSExtraData
{
public:
	TESModelTextureSwap	*model;		// 0C
	TESForm				*baseForm;	// 10
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
		UInt8							bodyPartID;
		UInt8							explodeChance;
		bool							explodeOnly;
		UInt8							byte03;
		BSSimpleArray<TESBoundObject*>	array04;
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

class BSMultiBoundShape;

// 34
class BGSPrimitive
{
public:
	virtual void	Destructor(bool doFree);
	virtual void	CreateFadeNode();
	virtual void	SetColor(NiColorAlpha *_color);
	virtual void	SetBounds(NiVector3 *_bounds);
	virtual bool	TestBounds(NiVector3 *_bounds);
	virtual BSMultiBoundShape	*CreateShape(NiVector3 *arg);

	enum PrimitiveType
	{
		kType_Box =		1,
		kType_Sphere =	2,
		kType_Plane =	3
	};

	UInt32			type;		// 04
	NiColorAlpha	color;		// 08
	NiVector3		bounds;		// 18
	NiRefObject		*unk24;		// 24
	NiRefObject		*unk28;		// 28
	BSFadeNode		*node;		// 2C
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
	UInt8			byte18;		// 18
	UInt8			pad19[3];	// 19
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
class ExtraModdingItem : public BSExtraData
{
public:
	bool		isModdingItem;	// 0C
	UInt8		pad0D[3];		// 0D
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
	enum RenderFlags
	{
		kRenderFlag_Bit0 =			1,
		kRenderFlag_Imposter =		2,
		kRenderFlag_StingerProp =	4,
		kRenderFlag_Disabled =		8
	};

	UInt32		flags;		// 0C
	UInt32		unk10;		// 10

	static ExtraSpecialRenderFlags* __stdcall Create(UInt32 _flags = 0);
};

// 10
class ExtraJIP : public BSExtraData
{
public:
	enum
	{
		kExtraJIP_Verion =	1
	};

	UINT		key;	// 0C

	static ExtraJIP *Create(UINT _key = 0);

	static UINT MakeKey();

	void __fastcall SaveGame(BGSSaveFormBuffer *sgBuffer);

	void Dump() const;
};

// 60
struct ExtraJIPData
{
	union XValue
	{
		float	flt;
		UInt32	refID;
		
		inline void operator=(double val) {flt = val;}
		inline void operator=(UInt32 val) {refID = val;}

		inline operator double() const {return flt;}
		inline operator UInt32() const {return refID;}
	};
	
	SInt32		initBtf;
	SInt32		typeBtf;
	XValue		values[18];
	XString		strings[2];

	inline bool IsUsed(UInt32 idx) const {return _bittest(&initBtf, idx) != 0;}
	inline bool IsRef(UInt32 idx) const {return _bittest(&typeBtf, idx) != 0;}

	inline void Set(UInt32 idx, double val)
	{
		values[idx] = val;
		UInt32 mask = 1 << idx;
		if (val) initBtf |= mask;
		else initBtf &= ~mask;
		typeBtf &= ~mask;
	}
	inline void Set(UInt32 idx, UInt32 val)
	{
		values[idx] = val;
		UInt32 mask = 1 << idx;
		if (val)
		{
			initBtf |= mask;
			typeBtf |= mask;
		}
		else
		{
			initBtf &= ~mask;
			typeBtf &= initBtf;
		}
	}
	inline void Set(UInt32 idx, const char *inStr) {strings[idx] = inStr;}

	void ResolvedRefIDs();
	
	inline UInt32 GetMaxIndex() const
	{
		UInt32 bitIdx;
		if (_BitScanReverse(&bitIdx, initBtf))
			return bitIdx + 1;
		return 0;
	}

	inline UInt32 GetSaveSize() const
	{
		if (UInt32 maxIdx = GetMaxIndex())
			return (maxIdx + 2) << 2;
		return 8;
	}

	void operator=(const ExtraJIPData &other);

	bool operator==(const ExtraJIPData &other) const;
	
	ExtraJIPData() {ZERO_BYTES(this, sizeof(ExtraJIPData));}
	ExtraJIPData(const ExtraJIPData &other) {*this = other;}

	~ExtraJIPData()
	{
		strings[0].Free();
		strings[1].Free();
	}

	void Dump() const;
};
static_assert(sizeof(ExtraJIPData) == 0x60);

typedef Map<UInt32, ExtraJIPData, 2> ExtraJIPDataMap;

struct ExtraJIPEntry
{
	UInt32			refID;
	ExtraJIPDataMap	dataMap;

	ExtraJIPEntry(UInt32 _refID = 0) : refID(_refID) {}

	UInt32 GetSaveSize();

	inline void operator=(const ExtraJIPEntry &other) {dataMap = other.dataMap;}

	inline bool operator==(const ExtraJIPEntry &other) const {return dataMap == other.dataMap;}
};
static_assert(sizeof(ExtraJIPEntry) == 0x10);

typedef UnorderedMap<UINT, ExtraJIPEntry, 0x20> ExtraJIPEntryMap;
extern TempObject<ExtraJIPEntryMap> s_extraDataKeysMap;