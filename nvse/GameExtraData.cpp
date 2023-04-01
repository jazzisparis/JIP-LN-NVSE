#include "nvse/GameExtraData.h"
#include "internal/jip_core.h"

ExtraContainerChanges *ExtraContainerChanges::Create()
{
	CreatetraType(ExtraContainerChanges)
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

ExtraHealth *ExtraHealth::Create(float _health)
{
	CreatetraType(ExtraHealth)
	ExtraHealth *xHealth = (ExtraHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraWorn *ExtraWorn::Create()
{
	CreatetraType(ExtraWorn)
	return (ExtraWorn*)dataPtr;
}

ExtraWornLeft *ExtraWornLeft::Create()
{
	CreatetraType(ExtraWornLeft)
	return (ExtraWornLeft*)dataPtr;
}

ExtraCannotWear *ExtraCannotWear::Create()
{
	CreatetraType(ExtraCannotWear)
	return (ExtraCannotWear*)dataPtr;
}

ExtraLock *ExtraLock::Create()
{
	CreatetraType(ExtraLock)
	UInt32 *lockData = (UInt32*)GameHeapAlloc(sizeof(Data));
	ZeroMemory(lockData, sizeof(Data));
	dataPtr[3] = (UInt32)lockData;
	return (ExtraLock*)dataPtr;
}

ExtraCount *ExtraCount::Create(UInt32 count)
{
	CreatetraType(ExtraCount)
	dataPtr[3] = count;
	return (ExtraCount*)dataPtr;
}

ExtraTeleport *ExtraTeleport::Create()
{
	CreatetraType(ExtraTeleport)
	UInt32 *teleData = (UInt32*)GameHeapAlloc(sizeof(Data));
	ZeroMemory(teleData, sizeof(Data));
	dataPtr[3] = (UInt32)teleData;
	return (ExtraTeleport*)dataPtr;
}

ExtraWeaponModFlags *ExtraWeaponModFlags::Create(UInt32 _flags)
{
	CreatetraType(ExtraWeaponModFlags)
	dataPtr[3] = _flags;
	return (ExtraWeaponModFlags*)dataPtr;
}

ExtraOwnership *ExtraOwnership::Create(TESForm *_owner)
{
	CreatetraType(ExtraOwnership)
	dataPtr[3] = (UInt32)_owner;
	return (ExtraOwnership*)dataPtr;
}

ExtraRank *ExtraRank::Create(UInt32 _rank)
{
	CreatetraType(ExtraRank)
	dataPtr[3] = _rank;
	return (ExtraRank*)dataPtr;
}

ExtraAction *ExtraAction::Create(TESObjectREFR *_actionRef)
{
	CreatetraType(ExtraAction)
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
	"Ownership", "Global", "Rank", "Count", "Health", "Uses", "JIP", "Charge", "Light", "Lock", "Teleport", "MapMarker",
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
	return (extraDataType < kXData_ExtraMax) ? kExtraDataNames[extraDataType] : "INVALID";
}

ExtraScript *ExtraScript::Create(Script *pScript)
{
	CreatetraType(ExtraScript)
	ExtraScript *xScript = (ExtraScript*)dataPtr;
	xScript->script = pScript;
	xScript->eventList = pScript ? pScript->CreateEventList() : NULL;
	return xScript;
}

ExtraFactionChanges *ExtraFactionChanges::Create()
{
	CreatetraType(ExtraFactionChanges)
	FactionListEntry *listData = (FactionListEntry*)GameHeapAlloc(sizeof(FactionListEntry));
	listData->Init();
	dataPtr[3] = (UInt32)listData;
	return (ExtraFactionChanges*)dataPtr;
}

ExtraHotkey *ExtraHotkey::Create(UInt8 _index)
{
	CreatetraType(ExtraHotkey)
	dataPtr[3] = _index;
	return (ExtraHotkey*)dataPtr;
}

ExtraTerminalState *ExtraTerminalState::Create()
{
	CreatetraType(ExtraTerminalState)
	dataPtr[3] = 0;
	return (ExtraTerminalState*)dataPtr;
}

ExtraCellWaterType *ExtraCellWaterType::Create(TESWaterForm *_waterForm)
{
	CreatetraType(ExtraCellWaterType)
	ExtraCellWaterType *xCellWater = (ExtraCellWaterType*)dataPtr;
	xCellWater->waterForm = _waterForm;
	return xCellWater;
}

ExtraCellMusicType *ExtraCellMusicType::Create(BGSMusicType *_musicType)
{
	CreatetraType(ExtraCellMusicType)
	ExtraCellMusicType *xMusicType = (ExtraCellMusicType*)dataPtr;
	xMusicType->musicType = _musicType;
	return xMusicType;
}

ExtraCellClimate *ExtraCellClimate::Create(TESClimate *_climate)
{
	CreatetraType(ExtraCellClimate)
	ExtraCellClimate *xCellClimate = (ExtraCellClimate*)dataPtr;
	xCellClimate->climate = _climate;
	return xCellClimate;
}

ExtraLinkedRef *ExtraLinkedRef::Create(TESObjectREFR *_linkedRef)
{
	CreatetraType(ExtraLinkedRef)
	ExtraLinkedRef *xLinkedRef = (ExtraLinkedRef*)dataPtr;
	xLinkedRef->linkedRef = _linkedRef;
	return xLinkedRef;
}

ExtraObjectHealth *ExtraObjectHealth::Create(float _health)
{
	CreatetraType(ExtraObjectHealth)
	ExtraObjectHealth *xHealth = (ExtraObjectHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraCellImageSpace *ExtraCellImageSpace::Create(TESImageSpace *_imgSpace)
{
	CreatetraType(ExtraCellImageSpace)
	ExtraCellImageSpace *xCellIS = (ExtraCellImageSpace*)dataPtr;
	xCellIS->imageSpace = _imgSpace;
	return xCellIS;
}

ExtraRadius *ExtraRadius::Create(float _radius)
{
	CreatetraType(ExtraRadius)
	ExtraRadius *xRadius = (ExtraRadius*)dataPtr;
	xRadius->radius = _radius;
	return xRadius;
}

ExtraPrimitive *ExtraPrimitive::Create(BGSPrimitive *_primitive)
{
	CreatetraType(ExtraPrimitive)
	ExtraPrimitive *xPrimitive = (ExtraPrimitive*)dataPtr;
	xPrimitive->primitive = _primitive;
	return xPrimitive;
}

ExtraCellAcousticSpace *ExtraCellAcousticSpace::Create(BGSAcousticSpace *_acousticSpace)
{
	CreatetraType(ExtraCellAcousticSpace)
	ExtraCellAcousticSpace *xCellAcoustic = (ExtraCellAcousticSpace*)dataPtr;
	xCellAcoustic->acousticSpace = _acousticSpace;
	return xCellAcoustic;
}

ExtraSpecialRenderFlags *ExtraSpecialRenderFlags::Create(UInt32 _flags)
{
	CreatetraType(ExtraSpecialRenderFlags)
	ExtraSpecialRenderFlags *xRenderFlags = (ExtraSpecialRenderFlags*)dataPtr;
	xRenderFlags->flags = _flags;
	xRenderFlags->unk10 = 0;
	return xRenderFlags;
}

ExtraOriginalReference *ExtraOriginalReference::Create(TESObjectREFR *_originalRef)
{
	CreatetraType(ExtraOriginalReference)
	ExtraOriginalReference *xOriginalRef = (ExtraOriginalReference*)dataPtr;
	xOriginalRef->originalRef = _originalRef;
	return xOriginalRef;
}

ExtraUses *ExtraUses::Create(UInt8 _uses)
{
	CreatetraType(ExtraUses)
	ExtraUses *xUses = (ExtraUses*)dataPtr;
	xUses->uses = _uses;
	return xUses;
}

ExtraTimeLeft *ExtraTimeLeft::Create(float _timeLeft)
{
	CreatetraType(ExtraTimeLeft)
	ExtraTimeLeft *xTimeLeft = (ExtraTimeLeft*)dataPtr;
	xTimeLeft->timeLeft = _timeLeft;
	return xTimeLeft;
}

ExtraCharge *ExtraCharge::Create(float _charge)
{
	CreatetraType(ExtraCharge)
	ExtraCharge *xCharge = (ExtraCharge*)dataPtr;
	xCharge->charge = _charge;
	return xCharge;
}

__declspec(naked) ContChangesEntry* __fastcall ContChangesEntryList::FindForItem(TESForm *item) const
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

ExtraDataList *ContChangesEntry::CreateExtraData()
{
	ExtraDataList *newList = ExtraDataList::Create();
	if (extendData)
		extendData->Prepend(newList);
	else
	{
		extendData = (ContChangesExtraList*)GameHeapAlloc(8);
		extendData->Init(newList);
	}
	return newList;
}

__declspec(naked) ExtraDataList *ExtraDataList::CreateCopy(bool bCopyAndRemove)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		push	0x20
		GAME_HEAP_ALLOC
		xorps	xmm0, xmm0
		movups	[eax], xmm0
		movups	[eax+0x10], xmm0
		mov		dword ptr [eax], kVtbl_ExtraDataList
		movzx	edx, byte ptr [esp+8]
		push	edx
		push	esi
		mov		esi, eax
		mov		ecx, eax
		CALL_EAX(0x412490)
		mov		eax, esi
		pop		esi
		retn	4
	}
}

__declspec(naked) double ExtraContainerChanges::Data::GetInventoryWeight()
{
	__asm
	{
		mov		dword ptr [ecx+8], 0xBF800000
		mov		eax, g_thePlayer
		movzx	eax, byte ptr [eax+0x7BC]
		push	eax
		mov		byte ptr ds:0x11E0898, 1
		CALL_EAX(0x4D0900)
		mov		byte ptr ds:0x11E0898, 0
		retn
	}
}

__declspec(naked) bool __fastcall ContChangesEntry::HasExtraType(UInt32 xType) const
{
	__asm
	{
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		push	esi
		mov		esi, eax
		mov		ecx, edx
		shr		ecx, 5
		lea		eax, [ecx*4+8]
		ALIGN 16
	iterHead:
		test	esi, esi
		jz		retnFalse
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		iterHead
		mov		ecx, [eax+ecx]
		bt		ecx, edx
		jnc		iterHead
		mov		al, 1
		pop		esi
		retn
	retnFalse:
		xor		al, al
		pop		esi
	done:
		retn
	}
}

__declspec(naked) bool ContChangesEntry::HasExtraLeveledItem() const
{
	__asm
	{
		mov		eax, [ecx]
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		mov		eax, [eax+4]
		test	ecx, ecx
		jz		iterHead
		test	byte ptr [ecx+0xD], 0x80
		jz		iterHead
		mov		al, 1
	done:
		retn
	}
}

__declspec(naked) ExtraDataList *ContChangesEntry::GetEquippedExtra() const
{
	__asm
	{
		mov		eax, [ecx]
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		mov		eax, [eax+4]
		test	ecx, ecx
		jz		iterHead
		test	byte ptr [ecx+0xA], 0x40
		jz		iterHead
		mov		eax, ecx
	done:
		retn
	}
}

__declspec(naked) float __vectorcall ContChangesEntry::GetWeaponModEffectValue(UInt32 effectType) const
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
		push	kXData_ExtraWeaponModFlags
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
		xorps	xmm0, xmm0
		pop		ecx
		retn
	}
}

__declspec(naked) float __vectorcall ContChangesEntry::GetBaseHealth() const
{
	__asm
	{
		mov		eax, [ecx+8]
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESObjectWEAP
		jnz		notWeapon
		cvtsi2ss	xmm1, [eax+0x98]
		mov		edx, 0xA
		call	ContChangesEntry::GetWeaponModEffectValue
		addss	xmm0, xmm1
		retn
	notWeapon:
		cmp		dl, kFormType_TESObjectARMO
		jnz		done
		cvtsi2ss	xmm0, [eax+0x6C]
		retn
	done:
		xorps	xmm0, xmm0
		retn
	}
}

__declspec(naked) float __vectorcall ContChangesEntry::GetHealthPercent() const
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
		movss	xmm0, SS_100
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		test	ecx, ecx
		jz		done
		push	kXData_ExtraHealth
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		movaps	xmm2, xmm0
		movss	xmm3, [eax+0xC]
		mov		ecx, esi
		call	ContChangesEntry::GetBaseHealth
		divss	xmm3, xmm0
		mulss	xmm3, xmm2
		minss	xmm2, xmm3
		movaps	xmm0, xmm2
	done:
		pop		esi
		retn
	invalid:
		movss	xmm0, kFltMin1
		retn
		ALIGN 4
	kFltMin1:
		EMIT_DW(BF, 80, 00, 00)
	}
}

bool __fastcall GetEntryDataHasModHook(ContChangesEntry *entry, int EDX, UInt8 modType);

__declspec(naked) float ContChangesEntry::CalculateWeaponDamage(Actor *owner, float condition, TESForm *ammo) const
{
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
		CALL_EAX(ADDR_ApplyAmmoEffects)
		add		esp, 0xC
	noAmmo:
		push	0xC
		mov		ecx, esi
		call	GetEntryDataHasModHook
		test	al, al
		jz		noMod
		fmul	dword ptr kSplitBeamMult
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
		CALL_EAX(ADDR_ApplyPerkModifiers)
		add		esp, 0x14
		fld		dword ptr [esp]
		pop		ecx
	done:
		pop		esi
		retn	0xC
		ALIGN 4
	kSplitBeamMult:
		EMIT_DW(3F, A6, 66, 66)
	}
}

ContChangesEntry *ContChangesEntry::CreateCopy(ExtraDataList *xData)
{
	ContChangesEntry *pCopy = (ContChangesEntry*)GameHeapAlloc(sizeof(ContChangesEntry));
	pCopy->type = type;
	pCopy->countDelta = countDelta;
	if (xData)
	{
		pCopy->extendData = (ExtendDataList*)GameHeapAlloc(sizeof(ExtendDataList));
		pCopy->extendData->Init(xData);
	}
	else pCopy->extendData = nullptr;
	return pCopy;
}

void ContChangesExtraList::Clear()
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

void ContChangesExtraList::CleanEmpty()
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

TempObject<ExtraJIPEntryMap> s_extraDataKeysMap;
PrimitiveCS s_JIPExtraDataCS;

ExtraJIP *ExtraJIP::Create(UInt32 _key)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraJIP));
	dataPtr[0] = kVtbl_ExtraTimeLeft;
	dataPtr[1] = kXData_ExtraJIP;
	dataPtr[2] = 0;
	ExtraJIP *xJIP = (ExtraJIP*)dataPtr;
	xJIP->key = _key;
	return xJIP;
}

__declspec(noinline) UInt32 ExtraJIP::MakeKey()
{
	while (true)
	{
		UInt32 key = ThisCall<UInt32, UInt32>(0xAA5230, (void*)0x11C4180, 0xFFFFFFFE) + 1;
		if (!s_extraDataKeysMap->HasKey(key))
			return key;
	}
}

__declspec(noinline) ExtraJIP *ExtraDataList::AddExtraJIP(UInt32 _key)
{
	return (ExtraJIP*)AddExtra(ExtraJIP::Create(_key));
}

void __fastcall ExtraJIP::SaveGame(BGSSaveFormBuffer *sgBuffer)
{
	UInt8 *buffer = sgBuffer->Reserve(5);
	buffer[4] = '|';
	XDATA_CS
	if (ExtraJIPEntry *entry = s_extraDataKeysMap->GetPtr(key))
	{
		entry->refID = sgBuffer->GetRefID();
		*(UInt32*)buffer = key;
	}
	else *(UInt32*)buffer = 0;
}

void __fastcall ExtraDataList::ExtraJIPLoadGame(BGSLoadFormBuffer *lgBuffer)
{
	UInt8 *buffer = lgBuffer->chunk + lgBuffer->chunkConsumed;
	lgBuffer->chunkConsumed += 5;
	UInt32 key = *(UInt32*)buffer;
	if (!key) return;
	if (ExtraJIP *xJIP = GetExtraType(this, ExtraJIP))
	{
		if (xJIP->key != key)	//	Should never happen!
		{
			s_extraDataKeysMap->Erase(xJIP->key);
			xJIP->key = key;
		}
	}
	else AddExtraJIP(key);
}

void __fastcall ExtraDataList::ExtraJIPCopy(ExtraJIP *source)
{
	XDATA_CS
	ExtraJIPEntry *inEntry = s_extraDataKeysMap->GetPtr(source->key);
	if (!inEntry) return;
	ExtraJIP *xJIP = GetExtraType(this, ExtraJIP);
	if (!xJIP)
		xJIP = AddExtraJIP(ExtraJIP::MakeKey());
	ExtraJIPEntry *newEntry = &s_extraDataKeysMap()[xJIP->key];
	newEntry->dataMap.Clear();
	*newEntry = *inEntry;
	s_dataChangedFlags |= kChangedFlag_ExtraData;
}

void ExtraJIPData::ResolvedRefID(UInt32 idx)
{
	if (IsRef(idx) && !(values[idx].refID = GetResolvedRefID(values[idx].refID)))
		_bittestandreset(&typeBtf, idx);
}

void ExtraJIPData::operator=(const ExtraJIPData &other)
{
	COPY_BYTES(this, &other, sizeof(values) + 8);
	strings[0].InitCopy(other.strings[0]);
	strings[1].InitCopy(other.strings[1]);
}

bool ExtraJIPData::operator==(const ExtraJIPData &other) const
{
	return MemCmp(this, &other, sizeof(values) + 8) && (strings[0] == other.strings[0]) && (strings[1] == other.strings[1]);
}

void ExtraJIPEntry::operator=(ExtraJIPEntry &other)
{
	for (auto iter = other.dataMap.Begin(); iter; ++iter)
		dataMap.Emplace(iter.Key(), iter());
}

bool ExtraJIPEntry::operator==(ExtraJIPEntry &other)
{
	if (dataMap.Size() != other.dataMap.Size())
		return false;
	for (auto iter1 = dataMap.Begin(), iter2 = other.dataMap.Begin(); iter1; ++iter1, ++iter2)
		if ((iter1.Key() != iter2.Key()) || !(iter1() == iter2()))
			return false;
	return true;
}

void ExtraJIPData::Dump() const
{
	UInt32 valuesUsed = ValuesUsed();
	for (UInt32 i = 0; i < valuesUsed; i++)
	{
		if (IsRef(i))
		{
			if (TESForm *form = LookupFormByRefID(values[i].refID))
			{
				PrintDebug("%d\t%08X\t%s", i, form->refID, form->GetEditorID());
				Console_Print("%d  %08X  %s", i, form->refID, form->GetEditorID());
			}
			else
			{
				PrintDebug("%d\t00000000\tNULL", i);
				Console_Print("%d  00000000  NULL", i);
			}
		}
		else
		{
			PrintDebug("%d\t%.4f", i, values[i].flt);
			Console_Print("%d  %.4f", i, values[i].flt);
		}
	}
	PrintDebug("18\t\"%s\"", strings[0].CString());
	Console_Print("18  \"%s\"", strings[0].CString());
	PrintDebug("19\t\"%s\"", strings[1].CString());
	Console_Print("19  \"%s\"", strings[1].CString());
}

void ExtraJIP::Dump() const
{
	if (ExtraJIPEntry *entry = s_extraDataKeysMap->GetPtr(key))
	{
		for (auto iter = entry->dataMap.Begin(); iter; ++iter)
		{
			PrintDebug("ModIdx = %02X", iter.Key());
			Console_Print("ModIdx = %02X", iter.Key());
			iter().Dump();
		}
	}
}