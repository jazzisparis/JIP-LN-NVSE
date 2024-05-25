#include "nvse/GameExtraData.h"
#include "internal/jip_core.h"

ExtraContainerChanges *ExtraContainerChanges::Create()
{
	CreateExtraType(ExtraContainerChanges)
	dataPtr[3] = 0;
	return (ExtraContainerChanges*)dataPtr;
}

ExtraContainerChanges::Data *ExtraContainerChanges::Data::Create(TESObjectREFR *owner)
{
	Data *data = Game_HeapAlloc<Data>();
	data->objList = Game_HeapAlloc<EntryDataList>();
	data->objList->Init();
	data->owner = owner;
	data->totalWgCurrent = -1.0F;
	data->totalWgLast = -1.0F;
	data->byte10 = 0;
	return data;
}

ExtraHealth *ExtraHealth::Create(float _health)
{
	CreateExtraType(ExtraHealth)
	ExtraHealth *xHealth = (ExtraHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraWorn *ExtraWorn::Create()
{
	CreateExtraType(ExtraWorn)
	return (ExtraWorn*)dataPtr;
}

ExtraWornLeft *ExtraWornLeft::Create()
{
	CreateExtraType(ExtraWornLeft)
	return (ExtraWornLeft*)dataPtr;
}

ExtraCannotWear *ExtraCannotWear::Create()
{
	CreateExtraType(ExtraCannotWear)
	return (ExtraCannotWear*)dataPtr;
}

ExtraLock *ExtraLock::Create()
{
	CreateExtraType(ExtraLock)
	ExtraLock *xLock = (ExtraLock*)dataPtr;
	xLock->data = Game_HeapAlloc<Data>();
	ZeroMemory(xLock->data, sizeof(Data));
	return xLock;
}

ExtraCount *ExtraCount::Create(SInt32 count)
{
	CreateExtraType(ExtraCount)
	dataPtr[3] = (count > SHRT_MAX) ? SHRT_MAX : count;
	return (ExtraCount*)dataPtr;
}

ExtraCount *ExtraDataList::AddExtraCount(SInt32 count)
{
	return (ExtraCount*)AddExtra(ExtraCount::Create(count));
}

ExtraTeleport *ExtraTeleport::Create()
{
	CreateExtraType(ExtraTeleport)
	ExtraTeleport *xTeleport = (ExtraTeleport*)dataPtr;
	xTeleport->data = Game_HeapAlloc<Data>();
	ZeroMemory(xTeleport->data, sizeof(Data));
	return xTeleport;
}

ExtraWeaponModFlags *ExtraWeaponModFlags::Create(UInt32 _flags)
{
	CreateExtraType(ExtraWeaponModFlags)
	dataPtr[3] = _flags;
	return (ExtraWeaponModFlags*)dataPtr;
}

ExtraOwnership *ExtraOwnership::Create(TESForm *_owner)
{
	CreateExtraType(ExtraOwnership)
	dataPtr[3] = (UInt32)_owner;
	return (ExtraOwnership*)dataPtr;
}

ExtraRank *ExtraRank::Create(UInt32 _rank)
{
	CreateExtraType(ExtraRank)
	dataPtr[3] = _rank;
	return (ExtraRank*)dataPtr;
}

ExtraAction *ExtraAction::Create(TESObjectREFR *_actionRef)
{
	CreateExtraType(ExtraAction)
	dataPtr[3] = 0;
	dataPtr[4] = (UInt32)_actionRef;
	return (ExtraAction*)dataPtr;
}

const char *kExtraDataNames[] =
{
	"Unknown00", "Havok", "Cell3D", "CellWaterType", "RegionList", "SeenData", "EditorID", "CellMusicType", "CellClimate",
	"ProcessMiddleLow", "CellCanopyShadowMask", "DetachTime", "PersistentCell", "Script", "Action", "StartingPosition",
	"Anim", "NoStack", "UsedMarkers", "DistantData", "RagdollData", "ContainerChanges", "Worn", "WornLeft", "PackageStartLocation",
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
	CreateExtraType(ExtraScript)
	ExtraScript *xScript = (ExtraScript*)dataPtr;
	xScript->script = pScript;
	xScript->eventList = pScript ? pScript->CreateEventList() : NULL;
	return xScript;
}

ExtraFactionChanges *ExtraFactionChanges::Create()
{
	CreateExtraType(ExtraFactionChanges)
	ExtraFactionChanges *xFacChanges = (ExtraFactionChanges*)dataPtr;
	xFacChanges->data = Game_HeapAlloc<FactionListEntry>();
	xFacChanges->data->Init();
	return xFacChanges;
}

ExtraHotkey *ExtraHotkey::Create(UInt8 _index)
{
	CreateExtraType(ExtraHotkey)
	dataPtr[3] = _index;
	return (ExtraHotkey*)dataPtr;
}

ExtraTerminalState *ExtraTerminalState::Create()
{
	CreateExtraType(ExtraTerminalState)
	dataPtr[3] = 0;
	return (ExtraTerminalState*)dataPtr;
}

ExtraCellWaterType *ExtraCellWaterType::Create(TESWaterForm *_waterForm)
{
	CreateExtraType(ExtraCellWaterType)
	ExtraCellWaterType *xCellWater = (ExtraCellWaterType*)dataPtr;
	xCellWater->waterForm = _waterForm;
	return xCellWater;
}

ExtraCellMusicType *ExtraCellMusicType::Create(BGSMusicType *_musicType)
{
	CreateExtraType(ExtraCellMusicType)
	ExtraCellMusicType *xMusicType = (ExtraCellMusicType*)dataPtr;
	xMusicType->musicType = _musicType;
	return xMusicType;
}

ExtraCellClimate *ExtraCellClimate::Create(TESClimate *_climate)
{
	CreateExtraType(ExtraCellClimate)
	ExtraCellClimate *xCellClimate = (ExtraCellClimate*)dataPtr;
	xCellClimate->climate = _climate;
	return xCellClimate;
}

ExtraLinkedRef *ExtraLinkedRef::Create(TESObjectREFR *_linkedRef)
{
	CreateExtraType(ExtraLinkedRef)
	ExtraLinkedRef *xLinkedRef = (ExtraLinkedRef*)dataPtr;
	xLinkedRef->linkedRef = _linkedRef;
	return xLinkedRef;
}

ExtraObjectHealth *ExtraObjectHealth::Create(float _health)
{
	CreateExtraType(ExtraObjectHealth)
	ExtraObjectHealth *xHealth = (ExtraObjectHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraCellImageSpace *ExtraCellImageSpace::Create(TESImageSpace *_imgSpace)
{
	CreateExtraType(ExtraCellImageSpace)
	ExtraCellImageSpace *xCellIS = (ExtraCellImageSpace*)dataPtr;
	xCellIS->imageSpace = _imgSpace;
	return xCellIS;
}

ExtraRadius *ExtraRadius::Create(float _radius)
{
	CreateExtraType(ExtraRadius)
	ExtraRadius *xRadius = (ExtraRadius*)dataPtr;
	xRadius->radius = _radius;
	return xRadius;
}

ExtraPrimitive *ExtraPrimitive::Create(BGSPrimitive *_primitive)
{
	CreateExtraType(ExtraPrimitive)
	ExtraPrimitive *xPrimitive = (ExtraPrimitive*)dataPtr;
	xPrimitive->primitive = _primitive;
	return xPrimitive;
}

ExtraCellAcousticSpace *ExtraCellAcousticSpace::Create(BGSAcousticSpace *_acousticSpace)
{
	CreateExtraType(ExtraCellAcousticSpace)
	ExtraCellAcousticSpace *xCellAcoustic = (ExtraCellAcousticSpace*)dataPtr;
	xCellAcoustic->acousticSpace = _acousticSpace;
	return xCellAcoustic;
}

ExtraSpecialRenderFlags *ExtraSpecialRenderFlags::Create(UInt32 _flags)
{
	CreateExtraType(ExtraSpecialRenderFlags)
	ExtraSpecialRenderFlags *xRenderFlags = (ExtraSpecialRenderFlags*)dataPtr;
	xRenderFlags->flags = _flags;
	xRenderFlags->unk10 = 0;
	return xRenderFlags;
}

ExtraOriginalReference *ExtraOriginalReference::Create(TESObjectREFR *_originalRef)
{
	CreateExtraType(ExtraOriginalReference)
	ExtraOriginalReference *xOriginalRef = (ExtraOriginalReference*)dataPtr;
	xOriginalRef->originalRef = _originalRef;
	return xOriginalRef;
}

ExtraUses *ExtraUses::Create(UInt8 _uses)
{
	CreateExtraType(ExtraUses)
	ExtraUses *xUses = (ExtraUses*)dataPtr;
	xUses->uses = _uses;
	return xUses;
}

ExtraTimeLeft *ExtraTimeLeft::Create(float _timeLeft)
{
	CreateExtraType(ExtraTimeLeft)
	ExtraTimeLeft *xTimeLeft = (ExtraTimeLeft*)dataPtr;
	xTimeLeft->timeLeft = _timeLeft;
	return xTimeLeft;
}

ExtraCharge *ExtraCharge::Create(float _charge)
{
	CreateExtraType(ExtraCharge)
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
		extendData = Game_HeapAlloc<ContChangesExtraList>();
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
		call	Game_DoHeapAlloc
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

__declspec(naked) double ExtraContainerChanges::Data::GetInventoryWeight() const
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

__declspec(naked) SInt32 ContChangesEntry::GetExtendDataCount() const
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, [ecx]
		xor		edi, edi
		ALIGN 16
	iterHead:
		test	esi, esi
		jz		done
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		iterHead
		inc		edi
		push	kXData_ExtraCount
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		iterHead
		movsx	edx, word ptr [eax+0xC]
		lea		edi, [edi+edx-1]
		jmp		iterHead
		ALIGN 16
	done:
		mov		eax, 1
		cmp		eax, edi
		cmovl	eax, edi
		pop		edi
		pop		esi
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
		movq	xmm2, xmm0
		movss	xmm3, [eax+0xC]
		mov		ecx, esi
		call	ContChangesEntry::GetBaseHealth
		divss	xmm3, xmm0
		mulss	xmm3, xmm2
		minss	xmm2, xmm3
		movq	xmm0, xmm2
	done:
		pop		esi
		retn
	invalid:
		movss	xmm0, kFltMin1
		retn
		ALIGN 4
	kFltMin1:
		EMIT_DW(0xBF800000)
	}
}

bool __fastcall GetEntryDataHasModHook(ContChangesEntry *entry, int, UInt8 modType);

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
		fmul	dword ptr ds:0x1022004
	noMod:
		mov		ecx, [esp+8]
		cmp		s_patchInstallState.NPCPerks, 0
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
	}
}

ContChangesEntry *ContChangesEntry::Create(TESForm *item, SInt32 count, ExtraDataList *xData)
{
	ContChangesEntry *newEntry = Game_HeapAlloc<ContChangesEntry>();
	newEntry->type = item;
	newEntry->countDelta = count;
	if (xData)
	{
		newEntry->extendData = Game_HeapAlloc<ExtendDataList>();
		newEntry->extendData->Init(xData);
	}
	else newEntry->extendData = nullptr;
	return newEntry;
}

ContChangesEntry *ContChangesEntry::CreateCopy(ExtraDataList *xData)
{
	ContChangesEntry *pCopy = Game_HeapAlloc<ContChangesEntry>();
	pCopy->type = type;
	pCopy->countDelta = countDelta;
	if (xData)
	{
		pCopy->extendData = Game_HeapAlloc<ExtendDataList>();
		pCopy->extendData->Init(xData);
	}
	else pCopy->extendData = nullptr;
	return pCopy;
}

void ContChangesExtraList::Clear()
{
	auto xdlIter = Head();
	do
	{
		if (ExtraDataList *xData = xdlIter->data)
		{
			xData->RemoveAll(true);
			Game_HeapFree(xData);
		}
	}
	while (xdlIter = xdlIter->next);
	RemoveAll();
}

void ContChangesExtraList::CleanEmpty()
{
	tList<ExtraDataList>::Node *xdlIter = Head(), *prev = NULL;
	do
	{
		if (ExtraDataList *xDataList = xdlIter->data; xDataList && !xDataList->m_data)
		{
			Game_HeapFree(xDataList);
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

ExtraJIP *ExtraJIP::Create(UINT _key)
{
	CreateExtraType(ExtraTimeLeft)
	dataPtr[3] = _key;
	return (ExtraJIP*)dataPtr;
}

__declspec(noinline) UINT ExtraJIP::MakeKey()
{
	while (true)
		if (UINT key = GetRandomUInt(0xFFFFFFFF); key && !s_extraDataKeysMap->HasKey(key))
			return key;
}

__declspec(noinline) ExtraJIP *ExtraDataList::AddExtraJIP(UINT _key)
{
	return (ExtraJIP*)AddExtra(ExtraJIP::Create(_key));
}

void __fastcall ExtraJIP::SaveGame(BGSSaveFormBuffer *sgBuffer)
{
	UInt8 *buffer = sgBuffer->Reserve(5);
	buffer[4] = '|';
	if (ExtraJIPEntry *entry = s_extraDataKeysMap->GetPtr(key))
	{
		entry->refID = sgBuffer->GetRefID();
		*(UInt32*)buffer = key;
	}
	else *(UInt32*)buffer = 0;
}

void ExtraJIPData::ResolvedRefIDs()
{
	for (UInt32 idx = 0, maxIdx = GetMaxIndex(), mask = 1; idx < maxIdx; idx++, mask <<= 1)
		if ((typeBtf & mask) && !GetResolvedRefID(&values[idx].refID))
			initBtf ^= mask;
	typeBtf &= initBtf;
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

UInt32 ExtraJIPEntry::GetSaveSize()
{
	UInt32 size = dataMap.Size() * 5;
	for (auto iter = dataMap.Begin(); iter; ++iter)
		size += iter().GetSaveSize() + iter().strings[0].Size() + iter().strings[1].Size();
	return size;
}

void ExtraJIPData::Dump() const
{
	for (UInt32 i = 0, maxIdx = GetMaxIndex(); i < maxIdx; i++)
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
		for (auto iter = entry->dataMap.Begin(); iter; ++iter)
		{
			PrintDebug("ModIdx = %02X", iter.Key());
			Console_Print("ModIdx = %02X", iter.Key());
			iter().Dump();
		}
}