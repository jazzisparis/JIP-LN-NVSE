#pragma once

#define JIP_VARS_VERSION 10

enum JIPSerializationTags : UInt32
{
	kJIPTag_ScriptVars =		'VSPJ',
	kJIPTag_AuxVars =			'VAPJ',
	kJIPTag_RefMaps =			'MRPJ',
	kJIPTag_ExtraData =			'DEPJ',
	kJIPTag_LinkedRefs =		'RLPJ',
	kJIPTag_SerializedVars =	'FGPJ',
	kJIPTag_AppearanceUndo =	'UAPJ',
	kJIPTag_NPCPerks =			'PNPJ'
};

char s_lastLoadedPath[0x80] = {0};

void __fastcall RestoreLinkedRefs(UnorderedMap<UInt32, UInt32> *tempMap = nullptr)
{
	for (auto linkIter = s_linkedRefDefault->Begin(); linkIter;)
	{
		UInt32 key = linkIter.Key();
		++linkIter;
		if (!tempMap || !tempMap->HasKey(key))
			SetLinkedRefID(key);
	}
}

__declspec(noinline) void CleanMLCallbacks()
{
	for (auto iter = s_mainLoopCallbacks->Begin(); iter; ++iter)
		if (iter->cmdPtr == JIPScriptRunner::RunScript)
		{
			((Script*)iter->thisObj)->Destroy(1);
			iter->bRemove = true;
		}
		else if (iter->flags & 8)
			iter->bRemove = true;
}

void MiniMapLoadGame();

void DoPreLoadGameHousekeeping()
{
	HOOK_SET(StartCombat, false);
	if (!s_forceCombatTargetMap->Empty())
	{
		s_forceCombatTargetMap->Clear();
		HOOK_SET(SetCombatTarget, false);
	}
	HOOK_SET(TeleportWithPC, false);
	HOOK_SET(LockEquipment, false);
	if (!s_forceDetectionValueMap->Empty())
	{
		s_forceDetectionValueMap->Clear();
		HOOK_SET(GetDetectionValue, false);
	}
	HOOK_SET(AddVATSTarget, false);

	if (UInt32 size = s_fireWeaponEventMap->Size() + s_fireWeaponEventScripts->Size())
		s_hookInfos[kHook_RemoveAmmo].SetCount(size);
	else HOOK_SET(RemoveAmmo, false);

	if (!s_eventInformedObjects->Empty())
	{
		for (auto userIter = s_eventInformedObjects->Begin(); userIter; ++userIter)
			if (TESForm *form = LookupFormByRefID(*userIter))
				form->jipFormFlags5 &= ~kHookFormFlag5_ScriptInformed;
		s_eventInformedObjects->Clear();

		if (!s_pcFastTravelInformed->Empty())
		{
			s_pcFastTravelInformed->Clear();
			HOOK_DEC(PCFastTravel);
		}
		if (!s_pcCellChangeInformed->Empty())
		{
			s_pcCellChangeInformed->Clear();
			HOOK_SET(PCCellChange, false);
		}
	}

	if (!s_scriptWaitInfoMap->Empty())
	{
		for (auto waitIter = s_scriptWaitInfoMap->Begin(); waitIter; ++waitIter)
			if (TESForm *form = LookupFormByRefID(waitIter().refID))
				form->jipFormFlags5 &= ~kHookFormFlag5_ScriptOnWait;
		s_scriptWaitInfoMap->Clear();
		HOOK_SET(ScriptWait, false);
		s_scriptWaitInfo = nullptr;
	}

	for (auto lgtIter = s_activePtLights->Begin(); lgtIter; ++lgtIter)
		if ((lgtIter->extraFlags & 0x80) && lgtIter->m_parent)
			lgtIter->m_parent->RemoveObject(*lgtIter);

	if (!s_swapObjLODMap->Empty())
	{
		s_swapObjLODMap->Clear();
		HOOK_SET(MakeObjLODPath, false);
	}

	if (!s_extraCamerasMap->Empty())
	{
		for (auto camIter = s_extraCamerasMap->Begin(); camIter; ++camIter)
		{
			if (camIter->m_parent)
				camIter->m_parent->RemoveObject(*camIter);
			camIter->Destructor(true);
		}
		s_extraCamerasMap->Clear();
	}

	/*if (!s_refNamesMap->Empty())
	{
		for (auto nameIter = s_refNamesMap->Begin(); nameIter; ++nameIter)
			FreeStringKey(*nameIter);
		s_refNamesMap->Clear();
		HOOK_SET(GetRefName, false);
	}*/

	for (auto nameIter = s_refNamesMap->Begin(); nameIter; ++nameIter)
		nameIter.Key()->JIPRefFlags() |= kHookRefFlag5F_AltRefName;

	if (!s_refrModelPathMap->Empty())
	{
		for (auto pathIter = s_refrModelPathMap->Begin(); pathIter; ++pathIter)
			FreeStringKey(*pathIter);
		s_refrModelPathMap->Clear();
		HOOK_SET(GetModelPath, false);
	}

	if (s_refMapMarkersCount)
	{
		while (--s_refMapMarkersCount)
			s_refMapMarkersList.RemoveNth(0);
		s_refMapMarkersList.Init();
		HOOK_SET(CreateMapMarkers, false);
	}

	if (auto pcAprUndo = s_appearanceUndoMap->Find((TESNPC*)g_thePlayer->baseForm))
	{
		pcAprUndo->Destroy();
		pcAprUndo.Remove();
	}

	CleanMLCallbacks();
	s_gameLoadFlagLN = true;
	HOOK_SET(OnRagdoll, false);
	s_onRagdollEventScripts->Clear();
	s_excludedCombatActionsMap->Clear();
	MiniMapLoadGame();
	s_syncPositionRef = nullptr;
	g_thePlayer->killer = nullptr;
}

void RestoreJIPFormFlags()
{
	for (auto flagsIter = s_jipFormFlagsMap->Begin(); flagsIter; ++flagsIter)
		if (TESForm *form = LookupFormByRefID(flagsIter.Key()); form && *flagsIter)
			form->jipFormFlags6 = *flagsIter;
		else flagsIter.Remove();
}

void DoLoadGameHousekeeping()
{
	RestoreJIPFormFlags();

	if (g_thePlayer->teammateCount)
	{
		auto perkIter = g_thePlayer->perkRanksTM.Head();
		do
		{
			if (PerkRank *perkRank = perkIter->data; perkRank && perkRank->perk)
				ThisCall(0x5EB6A0, perkRank->perk, g_thePlayer, 0, perkRank->rank, 1);
		}
		while (perkIter = perkIter->next);
	}

	if (s_patchInstallState.NPCWeaponMods)
	{
		g_thePlayer->actorFlags |= 0x10000000;
		auto actorIter = ProcessManager::Get()->highActors.Head();
		do
		{
			if (Actor *actor = actorIter->data; actor && !(actor->actorFlags & 0x10000000))
			{
				actor->actorFlags |= 0x10000000;
				if (ContChangesEntry *weaponInfo = ((HighProcess*)actor->baseProcess)->weaponInfo; weaponInfo && weaponInfo->extendData && !actor->isTeammate)
					DistributeWeaponMods(actor, weaponInfo);
			}
		}
		while (actorIter = actorIter->next);
	}

	if (s_patchInstallState.NPCPerks && (s_dataChangedFlags & kChangedFlag_NPCPerks))
	{
		auto actorIter = ProcessManager::Get()->highActors.Head();
		do
		{
			if (Actor *actor = actorIter->data)
				InitNPCPerks(actor);
		}
		while (actorIter = actorIter->next);
	}

	if (s_dataChangedFlags & kChangedFlag_LinkedRefs)
		RestoreLinkedRefs(*s_linkedRefsTemp);

	s_dataChangedFlags = 0;
}

void ProcessDataChangedFlags(UInt8 changedFlags)
{
	if (changedFlags & kChangedFlag_AuxVars)
		s_auxVariables[0]->Clear();
	if (changedFlags & kChangedFlag_RefMaps)
		s_refMapArrays[0]->Clear();
	if (changedFlags & kChangedFlag_ExtraData)
		s_extraDataKeysMap->Clear();
	if (changedFlags & kChangedFlag_LinkedRefs)
		s_linkedRefModified->Clear();
	s_scriptVariablesBuffer->Clear();
	s_linkedRefsTemp->Clear();
	s_serializedVars.Reset();

	if (!s_NPCPerksInfoMap->Empty() && (changedFlags & kChangedFlag_NPCPerks))
	{
		UInt32 size = s_NPCPerksInfoMap->Size();
		for (auto refIter = s_NPCPerksInfoMap->Begin(); refIter; ++refIter)
		{
			if (Actor *actor = (Actor*)LookupFormByRefID(refIter.Key()))
				if IS_ACTOR(actor) actor->extraDataList.perksInfo = nullptr;
			if (!--size) break;
		}
		s_NPCPerksInfoMap->Clear();
	}
}

__declspec(naked) UInt8* __fastcall ReadRecordToBuffer(AuxBuffer<UInt8> &loadBuf, UInt32 length)
{
	__asm
	{
		push	edx
		push	ecx
		mov		ecx, edx
		call	AuxBufferArray::GetAvailable
		pop		ecx
		mov		[ecx], eax
		push	dword ptr [eax]
		call	ReadRecordData
		pop		eax
		pop		ecx
		retn
	}
}

void LoadGameCallback(void*)
{
	UInt8 changedFlags = s_dataChangedFlags;
	ProcessDataChangedFlags(changedFlags);

	UInt32 type, version, length, nRecs, nRefs, nVars, refID;
	Pointers bufPos;
	UInt8 auxByte, modIdx;

	while (GetNextRecordInfo(&type, &version, &length))
	{
		if ((type & 0xFFFF) != 'PJ')
			continue;
		AuxBuffer<UInt8> loadBuf;
		if (type == kJIPTag_ScriptVars)
		{
			bufPos = ReadRecordToBuffer(loadBuf, length);
			TESForm *form;
			Script *script;
			ScriptLocals *eventList;
			nRecs = *bufPos.s++;
			while (nRecs)
			{
				nRecs--;
				refID = *bufPos.l;
				nVars = bufPos.s[2];
				bufPos += 6;
				if (GetResolvedRefID(&refID) && (form = LookupFormByRefID(refID)) && form->GetScriptAndEventList(&script, &eventList))
				{
					while (nVars)
					{
						nVars--;
						modIdx = *bufPos.b;
						auxByte = bufPos.b[1];
						char *namePos = bufPos.c + 2;
						bufPos += auxByte + 2;
						auxByte = *bufPos.b;
						*bufPos.b = 0;
						VarData *varData = (VarData*)bufPos.v;
						bufPos += 8;
						if (!GetResolvedModIndex(&modIdx)) continue;
						if (ScriptVar *var = script->AddVariable(namePos, eventList, refID, modIdx))
						{
							*(UInt8*)varData = auxByte;
							if (varData->refID && !varData->pad)
								GetResolvedRefID(&varData->refID);
							var->data = varData->num;
						}
					}
				}
				else
				{
					while (nVars)
					{
						nVars--;
						bufPos += bufPos.b[1] + 9;
					}
				}
			}
		}
		else if (type == kJIPTag_AuxVars)
		{
			if (!(changedFlags & kChangedFlag_AuxVars) || (version < JIP_VARS_VERSION))
				continue;
			bufPos = ReadRecordToBuffer(loadBuf, length);
			UInt16 nElems;
			nRecs = *bufPos.s++;
			while (nRecs)
			{
				nRecs--;
				modIdx = *bufPos.b++;
				nRefs = *bufPos.s++;
				if ((modIdx > 5) && GetResolvedModIndex(&modIdx))
				{
					AuxVarOwnersMap *ownersMap = nullptr;
					while (nRefs)
					{
						refID = *bufPos.l;
						nVars = bufPos.s[2];
						bufPos += 6;
						if (GetResolvedRefID(&refID) && (LookupFormByRefID(refID) || HasChangeData(refID)))
						{
							if (!ownersMap) ownersMap = s_auxVariables[0]->Emplace(modIdx, AlignBucketCount(nRefs));
							AuxVarVarsMap *aVarsMap = ownersMap->Emplace(refID, AlignBucketCount(nVars));
							while (nVars)
							{
								auxByte = *bufPos.b++;
								if (!auxByte)
									goto avSkipVars;
								char *namePos = bufPos.c;
								bufPos += auxByte;
								nElems = *bufPos.s;
								*bufPos.b = 0;
								bufPos += 2;
								AuxVarValsArr *valsArr = aVarsMap->Emplace(namePos, nElems);
								while (nElems)
								{
									bufPos = valsArr->Append(*bufPos.b)->ReadValData(bufPos.b + 1);
									nElems--;
								}
								nVars--;
							}
						}
						else
						{
							while (nVars)
							{
								bufPos += *bufPos.b + 1;
							avSkipVars:
								nElems = *bufPos.s++;
								while (nElems)
								{
									auxByte = *bufPos.b++;
									if (auxByte == 1)
										bufPos += 8;
									else if (auxByte == 2)
										bufPos += 4;
									else bufPos += *bufPos.s + 2;
									nElems--;
								}
								nVars--;
							}
						}
						nRefs--;
					}
				}
				else
				{
					while (nRefs)
					{
						bufPos += 4;
						nVars = *bufPos.s++;
						while (nVars)
						{
							bufPos += *bufPos.b + 1;
							nElems = *bufPos.s++;
							while (nElems)
							{
								auxByte = *bufPos.b++;
								if (auxByte == 1)
									bufPos += 8;
								else if (auxByte == 2)
									bufPos += 4;
								else bufPos += *bufPos.s + 2;
								nElems--;
							}
							nVars--;
						}
						nRefs--;
					}
				}
			}
		}
		else if (type == kJIPTag_RefMaps)
		{
			if (!(changedFlags & kChangedFlag_RefMaps) || (version < JIP_VARS_VERSION))
				continue;
			bufPos = ReadRecordToBuffer(loadBuf, length);
			nRecs = *bufPos.s++;
			while (nRecs)
			{
				nRecs--;
				modIdx = *bufPos.b++;
				nVars = *bufPos.s++;
				if ((modIdx > 5) && GetResolvedModIndex(&modIdx))
				{
					RefMapVarsMap *rVarsMap = nullptr;
					while (nVars)
					{
						auxByte = *bufPos.b++;
						if (!auxByte)
							goto rmSkipVars;
						char *namePos = bufPos.c;
						bufPos += auxByte;
						nRefs = *bufPos.s;
						*bufPos.b = 0;
						bufPos += 2;
						RefMapIDsMap *idsMap = nullptr;
						while (nRefs)
						{
							refID = *bufPos.l;
							auxByte = bufPos.b[4];
							bufPos += 5;
							if (GetResolvedRefID(&refID) && (LookupFormByRefID(refID) || HasChangeData(refID)))
							{
								if (!idsMap)
								{
									if (!rVarsMap) rVarsMap = s_refMapArrays[0]->Emplace(modIdx, AlignBucketCount(nVars));
									idsMap = rVarsMap->Emplace(namePos, AlignBucketCount(nRefs));
								}
								bufPos = idsMap->Emplace(refID, auxByte)->ReadValData(bufPos.b);
							}
							else if (auxByte == 1)
								bufPos += 8;
							else if (auxByte == 2)
								bufPos += 4;
							else bufPos += *bufPos.s + 2;
							nRefs--;
						}
						nVars--;
					}
				}
				else
				{
					while (nVars)
					{
						bufPos += *bufPos.b + 1;
					rmSkipVars:
						nRefs = *bufPos.s++;
						while (nRefs)
						{
							auxByte = bufPos.b[4];
							bufPos += 5;
							if (auxByte == 1)
								bufPos += 8;
							else if (auxByte == 2)
								bufPos += 4;
							else bufPos += *bufPos.s + 2;
							nRefs--;
						}
						nVars--;
					}
				}
			}
		}
		else if (type == kJIPTag_ExtraData)
		{
			if (!(changedFlags & kChangedFlag_ExtraData) || (version > ExtraJIP::kExtraJIP_Verion))
				continue;
			bufPos = ReadRecordToBuffer(loadBuf, length);
			nRecs = *bufPos.l++;
			s_extraDataKeysMap->SetBucketCount(nRecs);
			while (nRecs)
			{
				nRecs--;
				UINT key = *bufPos.l;
				refID = bufPos.l[1];
				nRefs = bufPos.b[8];
				bufPos += 9;
				UInt32 keySize = *bufPos.l++;
				if (GetResolvedRefID(&refID) && HasChangeData(refID))
				{
					ExtraJIPEntry *dataEntry = nullptr;
					while (nRefs)
					{
						nRefs--;
						modIdx = *bufPos.b++;
						UInt32 entSize = *bufPos.l;
						UInt32 strLen1 = bufPos.s[2];
						UInt32 strLen2 = bufPos.s[3];
						bufPos += 8;
						if (GetResolvedModIndex(&modIdx))
						{
							if (!dataEntry)
								dataEntry = s_extraDataKeysMap->Emplace(key, refID);
							ExtraJIPData *pData = &dataEntry->dataMap[modIdx];
							COPY_BYTES(pData, bufPos.b, entSize);
							pData->ResolvedRefIDs();
							if (strLen1)
								pData->strings[0].InitFromBuffer(bufPos.c + entSize, strLen1);
							if (strLen2)
								pData->strings[1].InitFromBuffer(bufPos.c + entSize + strLen1, strLen2);
						}
						bufPos += entSize + strLen1 + strLen2;
					}
				}
				else bufPos += keySize;
			}
		}
		else if (type == kJIPTag_LinkedRefs)
		{
			if (!(changedFlags & kChangedFlag_LinkedRefs))
				continue;
			bufPos = ReadRecordToBuffer(loadBuf, length);
			nRecs = *bufPos.s++;
			while (nRecs)
			{
				nRecs--;
				refID = *bufPos.l;
				UInt32 lnkID = bufPos.l[1];
				modIdx = bufPos.b[8];
				bufPos += 9;
				if (GetResolvedRefID(&refID) && GetResolvedRefID(&lnkID) && GetResolvedModIndex(&modIdx) && SetLinkedRefID(refID, lnkID, modIdx))
					s_linkedRefsTemp()[refID] = lnkID;
			}
		}
		else if (type == kJIPTag_SerializedVars)
			ReadRecordData(&s_serializedVars, length);
		else if (type == kJIPTag_AppearanceUndo)
		{
			AppearanceUndo *aprUndo = (AppearanceUndo*)malloc(sizeof(AppearanceUndo));
			ReadRecordData(aprUndo, 0x214);
			bufPos = ReadRecordToBuffer(loadBuf, length - 0x214);
			if (!GetResolvedRefID(bufPos.l) || !(aprUndo->race = (TESRace*)LookupFormByRefID(*bufPos.l)))
				aprUndo->race = (TESRace*)LookupFormByRefID(0x19);
			if (!GetResolvedRefID(bufPos.l + 1) || !(aprUndo->hair = (TESHair*)LookupFormByRefID(bufPos.l[1])))
				aprUndo->hair = (TESHair*)LookupFormByRefID((aprUndo->flags & 1) ? 0x22E4E : 0x14B90);
			if (!GetResolvedRefID(bufPos.l + 2) || !(aprUndo->eyes = (TESEyes*)LookupFormByRefID(bufPos.l[2])))
				aprUndo->eyes = (TESEyes*)LookupFormByRefID(0x4255);
			auxByte = bufPos.b[12];
			aprUndo->numParts = auxByte;
			bufPos += 13;
			if (auxByte)
			{
				BGSHeadPart **ptr = (BGSHeadPart**)malloc(auxByte * 4);
				aprUndo->headParts = ptr;
				do
				{
					if (GetResolvedRefID(bufPos.l) && (*ptr = (BGSHeadPart*)LookupFormByRefID(*bufPos.l)))
						ptr++;
					else aprUndo->numParts--;
					bufPos.l++;
				}
				while (--auxByte);
			}
			else aprUndo->headParts = nullptr;
			s_appearanceUndoMap()[(TESNPC*)g_thePlayer->baseForm] = aprUndo;
		}
		else if (type == kJIPTag_NPCPerks)
		{
			if (!s_patchInstallState.NPCPerks || !(changedFlags & kChangedFlag_NPCPerks))
				continue;
			bufPos = ReadRecordToBuffer(loadBuf, length);
			nRecs = *bufPos.s++;
			s_NPCPerksInfoMap->SetBucketCount(nRecs);
			Actor *actor;
			BGSPerk *perk;
			while (nRecs)
			{
				nRecs--;
				refID = *bufPos.l;
				auxByte = bufPos.b[4];
				bufPos += 5;
				if (!GetResolvedRefID(&refID) || !(actor = (Actor*)LookupFormByRefID(refID)) || NOT_ACTOR(actor))
				{
					bufPos += auxByte * 5;
					continue;
				}
				NPCPerksInfo *perksInfo = nullptr;
				while (auxByte)
				{
					auxByte--;
					UInt32 perkID = *bufPos.l;
					UInt8 rank = bufPos.b[4];
					bufPos += 5;
					if (!GetResolvedRefID(&perkID) || !(perk = (BGSPerk*)LookupFormByRefID(perkID)) || NOT_ID(perk, BGSPerk))
						continue;
					if (rank > perk->data.numRanks)
						rank = perk->data.numRanks;
					if (!perksInfo)
						perksInfo = &s_NPCPerksInfoMap()[refID];
					perksInfo->perkRanks[perk] = rank;
				}
				actor->extraDataList.perksInfo = perksInfo;
			}
		}
	}
}

void SaveGameCallback(void*)
{
	UInt8 auxByte;
	UInt32 auxLong;

	if (auxLong = s_scriptVariablesBuffer->Size())
	{
		WriteRecord(kJIPTag_ScriptVars, 9, &auxLong, 2);
		for (auto svOwnerIt = s_scriptVariablesBuffer->Begin(); svOwnerIt; ++svOwnerIt)
		{
			WriteRecord32(svOwnerIt.Key());
			WriteRecord16(svOwnerIt().Size());
			for (auto svVarIt = svOwnerIt().Begin(); svVarIt; ++svVarIt)
			{
				WriteRecord8(svVarIt().modIdx);
				auxByte = StrLen(svVarIt.Key());
				WriteRecord8(auxByte);
				WriteRecordData(svVarIt.Key(), auxByte);
				WriteRecord64(&svVarIt().value->data);
			}
		}
	}
	if (auxLong = s_auxVariables[0]->Size())
	{
		WriteRecord(kJIPTag_AuxVars, JIP_VARS_VERSION, &auxLong, 2);
		for (auto avModIt = s_auxVariables[0]->Begin(); avModIt; ++avModIt)
		{
			WriteRecord8(avModIt.Key());
			WriteRecord16(avModIt().Size());
			for (auto avOwnerIt = avModIt().Begin(); avOwnerIt; ++avOwnerIt)
			{
				WriteRecord32(avOwnerIt.Key());
				WriteRecord16(avOwnerIt().Size());
				for (auto avVarIt = avOwnerIt().Begin(); avVarIt; ++avVarIt)
				{
					auxByte = StrLen(avVarIt.Key());
					WriteRecord8(auxByte);
					WriteRecordData(avVarIt.Key(), auxByte);
					WriteRecord16(avVarIt().Size());
					for (auto avValIt = avVarIt().Begin(); avValIt; ++avValIt)
						avValIt().WriteValData();
				}
			}
		}
	}
	if (auxLong = s_refMapArrays[0]->Size())
	{
		WriteRecord(kJIPTag_RefMaps, JIP_VARS_VERSION, &auxLong, 2);
		for (auto rmModIt = s_refMapArrays[0]->Begin(); rmModIt; ++rmModIt)
		{
			WriteRecord8(rmModIt.Key());
			WriteRecord16(rmModIt().Size());
			for (auto rmVarIt = rmModIt().Begin(); rmVarIt; ++rmVarIt)
			{
				auxByte = StrLen(rmVarIt.Key());
				WriteRecord8(auxByte);
				WriteRecordData(rmVarIt.Key(), auxByte);
				WriteRecord16(rmVarIt().Size());
				for (auto rmRefIt = rmVarIt().Begin(); rmRefIt; ++rmRefIt)
				{
					WriteRecord32(rmRefIt.Key());
					rmRefIt().WriteValData();
				}
			}
		}
	}
	if (auxLong = s_extraDataKeysMap->Size())
	{
		WriteRecord(kJIPTag_ExtraData, ExtraJIP::kExtraJIP_Verion, &auxLong, 4);
		for (auto edKeyIt = s_extraDataKeysMap->Begin(); edKeyIt; ++edKeyIt)
		{
			WriteRecord32(edKeyIt.Key());
			WriteRecord32(edKeyIt().refID);
			WriteRecord8(edKeyIt().dataMap.Size());
			WriteRecord32(edKeyIt().GetSaveSize());
			for (auto edModIt = edKeyIt().dataMap.Begin(); edModIt; ++edModIt)
			{
				WriteRecord8(edModIt.Key());
				UInt32 saveSize = edModIt().GetSaveSize();
				WriteRecord32(saveSize);
				WriteRecord16(edModIt().strings[0].Size());
				WriteRecord16(edModIt().strings[1].Size());
				WriteRecordData(&edModIt(), saveSize);
				if (!edModIt().strings[0].Empty())
					WriteRecordData(edModIt().strings[0].Data(), edModIt().strings[0].Size());
				if (!edModIt().strings[1].Empty())
					WriteRecordData(edModIt().strings[1].Data(), edModIt().strings[1].Size());
			}
		}
	}
	if (auxLong = s_linkedRefModified->Size())
	{
		WriteRecord(kJIPTag_LinkedRefs, 9, &auxLong, 2);
		for (auto lrRefIt = s_linkedRefModified->Begin(); lrRefIt; ++lrRefIt)
		{
			WriteRecord32(lrRefIt.Key());
			WriteRecord32(lrRefIt().linkID);
			WriteRecord8(lrRefIt().modIdx);
		}
	}
	WriteRecord(kJIPTag_SerializedVars, JIPSerializedVars::kSzVars_Version, &s_serializedVars, sizeof(JIPSerializedVars));
	if (AppearanceUndo *aprUndo = s_appearanceUndoMap->Get((TESNPC*)g_thePlayer->baseForm))
	{
		WriteRecord(kJIPTag_AppearanceUndo, 9, aprUndo->values0, 0x214);
		WriteRecord32(aprUndo->race->refID);
		WriteRecord32(aprUndo->hair->refID);
		WriteRecord32(aprUndo->eyes->refID);
		WriteRecord8(aprUndo->numParts);
		for (UInt32 idx = 0; idx < aprUndo->numParts; idx++)
			WriteRecord32(aprUndo->headParts[idx]->refID);
	}
	if (auxLong = s_NPCPerksInfoMap->Size())
	{
		for (auto refIter = s_NPCPerksInfoMap->Begin(); refIter; ++refIter)
		{
			if (Actor *actor = (Actor*)LookupFormByRefID(refIter.Key()); actor && IS_ACTOR(actor))
			{
				if (!refIter().perkRanks.Empty() && !actor->lifeState && (actor->isTeammate || !(((TESActorBase*)actor->baseForm)->baseData.flags & 8) || actor->GetRefNiNode()))
					goto isValid;
				actor->extraDataList.perksInfo = nullptr;
			}
			refIter.Remove();
		isValid:
			if (!--auxLong) break;
		}
		if (auxLong = s_NPCPerksInfoMap->Size())
		{
			WriteRecord(kJIPTag_NPCPerks, 10, &auxLong, 2);
			for (auto refIter = s_NPCPerksInfoMap->Begin(); refIter; ++refIter)
			{
				WriteRecord32(refIter.Key());
				WriteRecord8(refIter().perkRanks.Size());
				for (auto perkIter = refIter().perkRanks.Begin(); perkIter; ++perkIter)
				{
					WriteRecord32(perkIter.Key()->refID);
					WriteRecord8(perkIter());
				}
				if (!--auxLong) break;
			}
		}
	}
}