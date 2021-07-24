#include "nvse/GameExtraData.h"

void ExtraContainerChanges::EntryData::Cleanup()
{
	if (!extendData) return;
	ListNode<ExtraDataList> *xdlIter = extendData->Head(), *prev = NULL;;
	ExtraDataList *xData;
	ExtraCount *xCount;
	do
	{
		xData = xdlIter->data;
		if (xData)
		{
			xCount = GetExtraType(xData, Count);
			if (xCount && (xCount->count <= 1))
				xData->RemoveByType(kExtraData_Count);
			if (xData->m_data)
			{
				prev = xdlIter;
				xdlIter = xdlIter->next;
				continue;
			}
		}
		GameHeapFree(xData);
		xdlIter = prev ? prev->RemoveNext() : xdlIter->RemoveMe();
	}
	while (xdlIter);
}

ExtraContainerChanges *ExtraContainerChanges::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraContainerChanges));
	dataPtr[0] = kVtbl_ExtraContainerChanges;
	dataPtr[1] = kExtraData_ContainerChanges;
	dataPtr[2] = 0;
	dataPtr[3] = 0;
	return (ExtraContainerChanges*)dataPtr;
}

ExtraContainerChanges::Data *ExtraContainerChanges::Data::Create(TESObjectREFR *owner)
{
	Data *data = (Data*)GameHeapAlloc(sizeof(Data));
	data->objList = (EntryDataList*)GameHeapAlloc(sizeof(EntryDataList));
	data->objList->Init();
	data->owner = owner;
	data->totalWgCurrent = -1.0F;
	data->totalWgLast = -1.0F;
	data->byte10 = 0;
	return data;
}

void ExtraContainerChanges::Cleanup()
{
	if (data && data->objList)
	{
		ListNode<EntryData> *entryIter = data->objList->Head();
		do
		{
			if (entryIter->data)
				entryIter->data->Cleanup();
		}
		while (entryIter = entryIter->next);
	}
}

BSExtraData *BSExtraData::Create(UInt8 xType, UInt32 size, UInt32 vtbl)
{
	BSExtraData *xData = (BSExtraData*)GameHeapAlloc(size);
	MemZero(xData, size);
	*(UInt32*)xData = vtbl;
	xData->type = xType;
	return xData;
}

ExtraHealth *ExtraHealth::Create(float _health)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraHealth));
	dataPtr[0] = kVtbl_ExtraHealth;
	dataPtr[1] = kExtraData_Health;
	dataPtr[2] = 0;
	ExtraHealth *xHealth = (ExtraHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraWorn *ExtraWorn::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraWorn));
	dataPtr[0] = kVtbl_ExtraWorn;
	dataPtr[1] = kExtraData_Worn;
	dataPtr[2] = 0;
	return (ExtraWorn*)dataPtr;
}

ExtraCannotWear *ExtraCannotWear::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCannotWear));
	dataPtr[0] = kVtbl_ExtraCannotWear;
	dataPtr[1] = kExtraData_CannotWear;
	dataPtr[2] = 0;
	return (ExtraCannotWear*)dataPtr;
}

ExtraLock *ExtraLock::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraLock));
	dataPtr[0] = kVtbl_ExtraLock;
	dataPtr[1] = kExtraData_Lock;
	dataPtr[2] = 0;
	UInt32 *lockData = (UInt32*)GameHeapAlloc(sizeof(Data));
	MemZero(lockData, sizeof(Data));
	dataPtr[3] = (UInt32)lockData;
	return (ExtraLock*)dataPtr;
}

ExtraCount *ExtraCount::Create(UInt32 count)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCount));
	dataPtr[0] = kVtbl_ExtraCount;
	dataPtr[1] = kExtraData_Count;
	dataPtr[2] = 0;
	dataPtr[3] = count;
	return (ExtraCount*)dataPtr;
}

ExtraTeleport *ExtraTeleport::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraTeleport));
	dataPtr[0] = kVtbl_ExtraTeleport;
	dataPtr[1] = kExtraData_Teleport;
	dataPtr[2] = 0;
	UInt32 *teleData = (UInt32*)GameHeapAlloc(sizeof(Data));
	MemZero(teleData, sizeof(Data));
	dataPtr[3] = (UInt32)teleData;
	return (ExtraTeleport*)dataPtr;
}

ExtraWeaponModFlags *ExtraWeaponModFlags::Create(UInt32 _flags)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraWeaponModFlags));
	dataPtr[0] = kVtbl_ExtraWeaponModFlags;
	dataPtr[1] = kExtraData_WeaponModFlags;
	dataPtr[2] = 0;
	dataPtr[3] = _flags;
	return (ExtraWeaponModFlags*)dataPtr;
}

ExtraOwnership *ExtraOwnership::Create(TESForm *_owner)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraOwnership));
	dataPtr[0] = kVtbl_ExtraOwnership;
	dataPtr[1] = kExtraData_Ownership;
	dataPtr[2] = 0;
	dataPtr[3] = (UInt32)_owner;
	return (ExtraOwnership*)dataPtr;
}

ExtraRank *ExtraRank::Create(UInt32 _rank)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraRank));
	dataPtr[0] = kVtbl_ExtraRank;
	dataPtr[1] = kExtraData_Rank;
	dataPtr[2] = 0;
	dataPtr[3] = _rank;
	return (ExtraRank*)dataPtr;
}

ExtraAction *ExtraAction::Create(TESObjectREFR *_actionRef)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraAction));
	dataPtr[0] = kVtbl_ExtraAction;
	dataPtr[1] = kExtraData_Action;
	dataPtr[2] = 0;
	dataPtr[3] = 0;
	dataPtr[4] = (UInt32)_actionRef;
	return (ExtraAction*)dataPtr;
}

const char *kExtraDataNames[] =
{
	"Unknown00", "Havok", "Cell3D", "CellWaterType", "RegionList", "SeenData", "EditorID", "CellMusicType", "CellClimate",
	"ProcessMiddleLow", "CellCanopyShadowMask", "DetachTime", "PersistentCell", "Script", "Action", "StartingPosition",
	"Anim", "Unknown11", "UsedMarkers", "DistantData", "RagdollData", "ContainerChanges", "Worn", "WornLeft", "PackageStartLocation",
	"Package", "TrespassPackage", "RunOncePacks", "ReferencePointer", "Follower", "LevCreaModifier", "Ghost", "OriginalReference",
	"Ownership", "Global", "Rank", "Count", "Health", "Uses", "TimeLeft", "Charge", "Light", "Lock", "Teleport", "MapMarker",
	"Unknown2D", "LeveledCreature", "LeveledItem", "Scale", "Seed", "NonActorMagicCaster", "NonActorMagicTarget", "Unknown34",
	"PlayerCrimeList", "Unknown36", "EnableStateParent", "EnableStateChildren", "ItemDropper", "DroppedItemList", "RandomTeleportMarker",
	"MerchantContainer", "SavedHavokData", "CannotWear", "Poison", "Unknown40", "LastFinishedSequence", "SavedAnimation",
	"NorthRotation", "XTarget", "FriendHits", "HeadingTarget", "Unknown47", "RefractionProperty", "StartingWorldOrCell", "Hotkey",
	"Unknown4B", "EditorRefMovedData", "InfoGeneralTopic", "HasNoRumors", "Sound", "TerminalState", "LinkedRef", "LinkedRefChildren",
	"ActivateRef", "ActivateRefChildren", "TalkingActor", "ObjectHealth", "DecalRefs", "Unknown58", "CellImageSpace", "NavMeshPortal",
	"ModelSwap", "Radius", "Radiation", "FactionChanges", "DismemberedLimbs", "ActorCause", "MultiBound", "MultiBoundData",
	"MultiBoundRef", "Unknown64", "ReflectedRefs", "ReflectorRefs", "EmittanceSource", "RadioData", "CombatStyle", "Unknown6A",
	"Primitive", "OpenCloseActivateRef", "AnimNoteReciever", "Ammo", "PatrolRefData", "PackageData", "OcclusionPlane", "CollisionData",
	"SayTopicInfoOnceADay", "EncounterZone", "SayToTopicInfo", "OcclusionPlaneRefData", "PortalRefData", "Portal", "Room", "HealthPerc",
	"RoomRefData", "GuardedRefData", "CreatureAwakeSound", "WaterZoneMap", "Unknown7F", "IgnoredBySandbox", "CellAcousticSpace",
	"ReservedMarkers", "WeaponIdleSound", "WaterLightRefs", "LitWaterRefs", "WeaponAttackSound", "ActivateLoopSound",
	"PatrolRefInUseData", "AshPileRef", "CreatureMovementSound", "FollowerSwimBreadcrumbs", "CellImpactSwap", "WeaponModFlags",
	"ModdingItem", "SecuritronFace", "AudioMarker", "AudioBuoyMarker", "SpecialRenderFlags"
};

const char *GetExtraDataName(UInt8 extraDataType)
{
	return (extraDataType < kExtraData_Max) ? kExtraDataNames[extraDataType] : "INVALID";
}

ExtraScript *ExtraScript::Create(Script *pScript)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraScript));
	dataPtr[0] = kVtbl_ExtraScript;
	dataPtr[1] = kExtraData_Script;
	dataPtr[2] = 0;
	ExtraScript *xScript = (ExtraScript*)dataPtr;
	xScript->script = pScript;
	xScript->eventList = pScript ? pScript->CreateEventList() : NULL;
	return xScript;
}

ExtraFactionChanges *ExtraFactionChanges::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraFactionChanges));
	dataPtr[0] = kVtbl_ExtraFactionChanges;
	dataPtr[1] = kExtraData_FactionChanges;
	dataPtr[2] = 0;
	FactionListEntry *listData = (FactionListEntry*)GameHeapAlloc(sizeof(FactionListEntry));
	listData->Init();
	dataPtr[3] = (UInt32)listData;
	return (ExtraFactionChanges*)dataPtr;
}

ExtraHotkey *ExtraHotkey::Create(UInt8 _index)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraHotkey));
	dataPtr[0] = kVtbl_ExtraHotkey;
	dataPtr[1] = kExtraData_Hotkey;
	dataPtr[2] = 0;
	dataPtr[3] = _index;
	return (ExtraHotkey*)dataPtr;
}

ExtraTerminalState *ExtraTerminalState::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraTerminalState));
	dataPtr[0] = kVtbl_ExtraTerminalState;
	dataPtr[1] = kExtraData_TerminalState;
	dataPtr[2] = 0;
	dataPtr[3] = 0;
	return (ExtraTerminalState*)dataPtr;
}

ExtraCellWaterType *ExtraCellWaterType::Create(TESWaterForm *_waterForm)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellWaterType));
	dataPtr[0] = kVtbl_ExtraCellWaterType;
	dataPtr[1] = kExtraData_CellWaterType;
	dataPtr[2] = 0;
	ExtraCellWaterType *xCellWater = (ExtraCellWaterType*)dataPtr;
	xCellWater->waterForm = _waterForm;
	return xCellWater;
}

ExtraCellMusicType *ExtraCellMusicType::Create(BGSMusicType *_musicType)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellMusicType));
	dataPtr[0] = kVtbl_ExtraCellMusicType;
	dataPtr[1] = kExtraData_CellMusicType;
	dataPtr[2] = 0;
	ExtraCellMusicType *xMusicType = (ExtraCellMusicType*)dataPtr;
	xMusicType->musicType = _musicType;
	return xMusicType;
}

ExtraCellClimate *ExtraCellClimate::Create(TESClimate *_climate)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellClimate));
	dataPtr[0] = kVtbl_ExtraCellClimate;
	dataPtr[1] = kExtraData_CellClimate;
	dataPtr[2] = 0;
	ExtraCellClimate *xCellClimate = (ExtraCellClimate*)dataPtr;
	xCellClimate->climate = _climate;
	return xCellClimate;
}

ExtraLinkedRef *ExtraLinkedRef::Create(TESObjectREFR *_linkedRef)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraLinkedRef));
	dataPtr[0] = kVtbl_ExtraLinkedRef;
	dataPtr[1] = kExtraData_LinkedRef;
	dataPtr[2] = 0;
	ExtraLinkedRef *xLinkedRef = (ExtraLinkedRef*)dataPtr;
	xLinkedRef->linkedRef = _linkedRef;
	return xLinkedRef;
}

ExtraObjectHealth *ExtraObjectHealth::Create(float _health)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraObjectHealth));
	dataPtr[0] = kVtbl_ExtraObjectHealth;
	dataPtr[1] = kExtraData_ObjectHealth;
	dataPtr[2] = 0;
	ExtraObjectHealth *xHealth = (ExtraObjectHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraCellImageSpace *ExtraCellImageSpace::Create(TESImageSpace *_imgSpace)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellImageSpace));
	dataPtr[0] = kVtbl_ExtraCellImageSpace;
	dataPtr[1] = kExtraData_CellImageSpace;
	dataPtr[2] = 0;
	ExtraCellImageSpace *xCellIS = (ExtraCellImageSpace*)dataPtr;
	xCellIS->imageSpace = _imgSpace;
	return xCellIS;
}

ExtraRadius *ExtraRadius::Create(float _radius)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraRadius));
	dataPtr[0] = kVtbl_ExtraRadius;
	dataPtr[1] = kExtraData_Radius;
	dataPtr[2] = 0;
	ExtraRadius *xRadius = (ExtraRadius*)dataPtr;
	xRadius->radius = _radius;
	return xRadius;
}

ExtraPrimitive *ExtraPrimitive::Create(BGSPrimitive *_primitive)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraPrimitive));
	dataPtr[0] = kVtbl_ExtraPrimitive;
	dataPtr[1] = kExtraData_Primitive;
	dataPtr[2] = 0;
	ExtraPrimitive *xPrimitive = (ExtraPrimitive*)dataPtr;
	xPrimitive->primitive = _primitive;
	return xPrimitive;
}

ExtraCellAcousticSpace *ExtraCellAcousticSpace::Create(BGSAcousticSpace *_acousticSpace)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellAcousticSpace));
	dataPtr[0] = kVtbl_ExtraCellAcousticSpace;
	dataPtr[1] = kExtraData_CellAcousticSpace;
	dataPtr[2] = 0;
	ExtraCellAcousticSpace *xCellAcoustic = (ExtraCellAcousticSpace*)dataPtr;
	xCellAcoustic->acousticSpace = _acousticSpace;
	return xCellAcoustic;
}

ExtraSpecialRenderFlags *ExtraSpecialRenderFlags::Create(UInt32 _flags)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraSpecialRenderFlags));
	dataPtr[0] = kVtbl_ExtraSpecialRenderFlags;
	dataPtr[1] = kExtraData_SpecialRenderFlags;
	dataPtr[2] = 0;
	ExtraSpecialRenderFlags *xRenderFlags = (ExtraSpecialRenderFlags*)dataPtr;
	xRenderFlags->flags = _flags;
	xRenderFlags->unk10 = 0;
	return xRenderFlags;
}

ExtraOriginalReference *ExtraOriginalReference::Create(TESObjectREFR *_originalRef)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraOriginalReference));
	dataPtr[0] = kVtbl_ExtraOriginalReference;
	dataPtr[1] = kExtraData_OriginalReference;
	dataPtr[2] = 0;
	ExtraOriginalReference *xOriginalRef = (ExtraOriginalReference*)dataPtr;
	xOriginalRef->originalRef = _originalRef;
	return xOriginalRef;
}

ExtraUses *ExtraUses::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraUses));
	dataPtr[0] = kVtbl_ExtraUses;
	dataPtr[1] = kExtraData_Uses;
	dataPtr[2] = 0;
	return (ExtraUses*)dataPtr;
}

ExtraTimeLeft *ExtraTimeLeft::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraTimeLeft));
	dataPtr[0] = kVtbl_ExtraTimeLeft;
	dataPtr[1] = kExtraData_TimeLeft;
	dataPtr[2] = 0;
	return (ExtraTimeLeft*)dataPtr;
}

ExtraCharge *ExtraCharge::Create(float _charge)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCharge));
	dataPtr[0] = kVtbl_ExtraCharge;
	dataPtr[1] = kExtraData_Charge;
	dataPtr[2] = 0;
	ExtraCharge *xCharge = (ExtraCharge*)dataPtr;
	xCharge->charge = _charge;
	return xCharge;
}

__declspec(naked) ContChangesEntry* __fastcall ExtraContainerChanges::EntryDataList::FindForItem(TESForm *item)
{
	__asm
	{
	listIter:
		test	ecx, ecx
		jz		retnNULL
		mov		eax, [ecx]
		mov		ecx, [ecx+4]
		test	eax, eax
		jz		listIter
		cmp		[eax+8], edx
		jnz		listIter
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

ExtraDataList *ExtraDataList::CreateCopy()
{
	ExtraDataList *xDataList = Create();
	xDataList->CopyFrom(this);
	return xDataList;
}

__declspec(naked) float ExtraContainerChanges::Data::GetInventoryWeight()
{
	__asm
	{
		mov		dword ptr [ecx+8], 0xBF800000
		mov		eax, g_thePlayer
		movzx	eax, byte ptr [eax+0x7BC]
		push	eax
		mov		byte ptr ds:[0x11E0898], 1
		CALL_EAX(0x4D0900)
		mov		byte ptr ds:[0x11E0898], 0
		retn
	}
}

__declspec(naked) bool __fastcall ExtraContainerChanges::EntryData::HasExtraType(UInt8 xType)
{
	__asm
	{
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		push	esi
		mov		esi, eax
		mov		cl, dl
		movzx	eax, cl
		shr		al, 3
		add		al, 8
		and		cl, 7
		mov		dl, 1
		shl		dl, cl
		ALIGN 16
	iterHead:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		iterNext
		test	byte ptr [ecx+eax], dl
		jnz		retnTrue
	iterNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		iterHead
		xor		al, al
		pop		esi
		retn
	retnTrue:
		mov		al, 1
		pop		esi
	done:
		retn
	}
}

__declspec(naked) bool ExtraContainerChanges::EntryData::HasExtraLeveledItem()
{
	__asm
	{
		mov		ecx, [ecx]
		ALIGN 16
	iterHead:
		test	ecx, ecx
		jz		retnFalse
		mov		edx, [ecx]
		mov		ecx, [ecx+4]
		test	edx, edx
		jz		iterHead
		test	byte ptr [edx+0xD], 0x80
		jz		iterHead
		mov		al, 1
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

ExtraDataList *ExtraContainerChanges::EntryData::GetEquippedExtra()
{
	if (extendData)
	{
		ExtraDataList *xData;
		ListNode<ExtraDataList> *xdlIter = extendData->Head();
		do
		{
			xData = xdlIter->data;
			if (xData && xData->HasType(kExtraData_Worn))
				return xData;
		}
		while (xdlIter = xdlIter->next);
	}
	return NULL;
}

__declspec(naked) float __vectorcall ExtraContainerChanges::EntryData::GetWeaponModEffectValue(UInt32 effectType)
{
	__asm
	{
		push	edx
		mov		eax, [ecx]
		test	eax, eax
		jz		retn0
		mov		edx, [ecx+8]
		mov		ecx, [eax]
		test	ecx, ecx
		jz		retn0
		push	edx
		push	kExtraData_WeaponModFlags
		call	BaseExtraList::GetByType
		pop		ecx
		test	eax, eax
		jz		retn0
		mov		dl, [eax+0xC]
		mov		eax, [esp]
		test	dl, 1
		jz		check2nd
		cmp		[ecx+0x180], eax
		jnz		check2nd
		movss	xmm0, [ecx+0x18C]
		pop		ecx
		retn
	check2nd:
		test	dl, 2
		jz		check3rd
		cmp		[ecx+0x184], eax
		jnz		check3rd
		movss	xmm0, [ecx+0x190]
		pop		ecx
		retn
	check3rd:
		test	dl, 4
		jz		retn0
		cmp		[ecx+0x188], eax
		jnz		retn0
		movss	xmm0, [ecx+0x194]
		pop		ecx
		retn
	retn0:
		pxor	xmm0, xmm0
		pop		ecx
		retn
	}
}

__declspec(naked) float __vectorcall ExtraContainerChanges::EntryData::GetBaseHealth()
{
	__asm
	{
		mov		eax, [ecx+8]
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESObjectWEAP
		jnz		notWeapon
		movd	xmm1, [eax+0x98]
		cvtdq2ps	xmm1, xmm1
		mov		edx, 0xA
		call	ContChangesEntry::GetWeaponModEffectValue
		addss	xmm0, xmm1
		retn
	notWeapon:
		cmp		dl, kFormType_TESObjectARMO
		jnz		done
		movd	xmm0, [eax+0x6C]
		cvtdq2ps	xmm0, xmm0
		retn
	done:
		pxor	xmm0, xmm0
		retn
	}
}

__declspec(naked) float ExtraContainerChanges::EntryData::GetHealthPercent()
{
	__asm
	{
		mov		eax, [ecx+8]
		mov		dl, [eax+4]
		mov		al, dl
		and		al, 0x38
		cmp		al, dl
		jnz		invalid
		push	esi
		mov		esi, ecx
		movss	xmm3, kFlt100
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		test	ecx, ecx
		jz		done
		push	kExtraData_Health
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		movss	xmm2, [eax+0xC]
		mov		ecx, esi
		call	ContChangesEntry::GetBaseHealth
		divss	xmm2, xmm0
		mulss	xmm2, xmm3
		minss	xmm3, xmm2
	done:
		movss	[esp-4], xmm3
		fld		dword ptr [esp-4]
		pop		esi
		retn
	invalid:
		fld1
		fchs
		retn
	}
}

__declspec(naked) float ExtraContainerChanges::EntryData::CalculateWeaponDamage(Actor *owner, float condition, TESForm *ammo)
{
	static const double kSplitBeamMult = 1.3;
	__asm
	{
		push	esi
		mov		esi, ecx
		cmp		dword ptr [esp+0x10], 0
		setnz	dl
		movzx	edx, dl
		push	edx
		push	ecx
		push	0
		push	0
		push	0x3F800000
		push	dword ptr [esp+0x20]
		push	dword ptr [ecx+8]
		mov		ecx, [esp+0x24]
		add		ecx, 0xA4
		push	ecx
		CALL_EAX(0x644CE0)
		add		esp, 0x20
		mov		ecx, [esp+0x10]
		test	ecx, ecx
		jz		noAmmo
		add		ecx, 0xD4
		cmp		dword ptr [ecx], 0
		jz		noAmmo
		push	ecx
		fstp	dword ptr [esp]
		push	ecx
		push	kAmmoEffect_DamageMod
		CALL_EAX(0x59A030)
		add		esp, 0xC
	noAmmo:
		push	0xC
		mov		ecx, esi
		CALL_EAX(0x4BDA70)
		test	al, al
		jz		noMod
		fmul	kSplitBeamMult
	noMod:
		mov		ecx, [esp+8]
		cmp		s_NPCPerks, 0
		jnz		perkMod
		cmp		dword ptr [ecx+0xC], 0x14
		jz		perkMod
		cmp		byte ptr [ecx+0x18D], 0
		jz		done
	perkMod:
		push	ecx
		fstp	dword ptr [esp]
		push	esp
		push	ecx
		push	dword ptr [esi+8]
		push	ecx
		push	kPerkEntry_CalculateWeaponDamage
		CALL_EAX(kAddr_ApplyPerkModifiers)
		add		esp, 0x14
		fld		dword ptr [esp]
		pop		ecx
	done:
		pop		esi
		retn	0xC
	}
}

void ExtraContainerChanges::ExtendDataList::Clear()
{
	ListNode<ExtraDataList> *xdlIter = Head();
	ExtraDataList *xData;
	do
	{
		xData = xdlIter->data;
		if (xData)
		{
			xData->RemoveAll(true);
			GameHeapFree(xData);
		}
	}
	while (xdlIter = xdlIter->next);
	RemoveAll();
}

void ExtraContainerChanges::ExtendDataList::CleanEmpty()
{
	ListNode<ExtraDataList> *xdlIter = Head(), *prev = NULL;
	ExtraDataList *xDataList;
	do
	{
		xDataList = xdlIter->data;
		if (xDataList && !xDataList->m_data)
		{
			GameHeapFree(xDataList);
			xdlIter = prev ? prev->RemoveNext() : xdlIter->RemoveMe();
		}
		else
		{
			prev = xdlIter;
			xdlIter = xdlIter->next;
		}
	}
	while (xdlIter);
}