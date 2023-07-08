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
	{
		if (iter->cmdPtr == JIPScriptRunner::RunScript)
		{
			((Script*)iter->thisObj)->Destroy(1);
			iter->bRemove = true;
		}
		else if (iter->flags & 8)
			iter->bRemove = true;
	}
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
	if (HOOK_SET(EquipItem, false))
	{
		HOOK_SET(ReEquipAll, false);
		HOOK_SET(WeaponSwitchSelect, false);
		HOOK_SET(WeaponSwitchUnequip, false);
		HOOK_SET(GetPreferedWeapon, false);
	}
	if (!s_forceDetectionValueMap->Empty())
	{
		s_forceDetectionValueMap->Clear();
		HOOK_SET(GetDetectionValue, false);
	}
	HOOK_SET(AddVATSTarget, false);

	UInt32 size = s_fireWeaponEventMap->Size() + s_fireWeaponEventScripts->Size();
	if (!size) HOOK_SET(RemoveAmmo, false);
	else s_hookInfos[kHook_RemoveAmmo].SetCount(size);

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
		HOOK_SET(ScriptRunner, false);
		HOOK_SET(EvalEventBlock, false);
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

	if (s_NPCWeaponMods)
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

	if (s_NPCPerks && (s_dataChangedFlags & kChangedFlag_NPCPerks))
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
		s_auxVariablesPerm->Clear();
	if (changedFlags & kChangedFlag_RefMaps)
		s_refMapArraysPerm->Clear();
	if (changedFlags & kChangedFlag_ExtraData)
		s_extraDataKeysMap->Clear();
	if (changedFlags & kChangedFlag_LinkedRefs)
		s_linkedRefModified->Clear();
	s_scriptVariablesBuffer->Clear();
	s_linkedRefsTemp->Clear();
	s_serializedVars.serializedFlags = 0;

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

struct LoadGameBuffer
{
	UInt8		*bufferPtr = nullptr;
	UInt32		bufferSize = 0x10000;

	UInt8* __fastcall Get(UInt32 length);
}
s_loadGameBuffer;

__declspec(noinline) UInt8* __fastcall LoadGameBuffer::Get(UInt32 length)
{
	if (bufferSize < length)
	{
		bufferSize = length;
		if (bufferPtr)
		{
			free(bufferPtr);
			bufferPtr = nullptr;
		}
	}
	if (!bufferPtr)
		bufferPtr = (UInt8*)malloc(bufferSize);
	ReadRecordData(bufferPtr, length);
	return bufferPtr;
}

void LoadGameCallback(void*)
{
	UInt8 changedFlags = s_dataChangedFlags;
	ProcessDataChangedFlags(changedFlags);

	UInt32 type, version, length, nRecs, nRefs, nVars, buffer4, refID;
	UInt8 buffer1, modIdx;
	UInt8 *bufPos, *namePos;

	while (GetNextRecordInfo(&type, &version, &length))
	{
		if (type == kJIPTag_ScriptVars)
		{
			bufPos = s_loadGameBuffer.Get(length);
			TESForm *form;
			Script *script;
			ScriptLocals *eventList;
			nRecs = *(UInt16*)bufPos;
			bufPos += 2;
			while (nRecs)
			{
				nRecs--;
				refID = *(UInt32*)bufPos;
				bufPos += 4;
				nVars = *(UInt16*)bufPos;
				bufPos += 2;
				if (GetResolvedRefID(&refID) && (form = LookupFormByRefID(refID)) && form->GetScriptAndEventList(&script, &eventList))
				{
					while (nVars)
					{
						nVars--;
						modIdx = *bufPos++;
						buffer1 = *bufPos++;
						namePos = bufPos;
						bufPos += buffer1;
						buffer1 = *bufPos;
						*bufPos = 0;
						VarData *varData = (VarData*)bufPos;
						bufPos += 8;
						if (!GetResolvedModIndex(&modIdx)) continue;
						if (ScriptVar *var = script->AddVariable((char*)namePos, eventList, refID, modIdx))
						{
							*(UInt8*)varData = buffer1;
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
						buffer1 = *++bufPos + 8;
						bufPos += buffer1;
					}
				}
			}
		}
		else if (type == kJIPTag_AuxVars)
		{
			if (!(changedFlags & kChangedFlag_AuxVars) || (version < JIP_VARS_VERSION))
				continue;
			bufPos = s_loadGameBuffer.Get(length);
			UInt16 nElems;
			nRecs = *(UInt16*)bufPos;
			bufPos += 2;
			while (nRecs)
			{
				nRecs--;
				modIdx = *bufPos;
				nRefs = *(UInt16*)(bufPos + 1);
				bufPos += 3;
				if ((modIdx > 5) && GetResolvedModIndex(&modIdx))
				{
					AuxVarOwnersMap *ownersMap = nullptr;
					while (nRefs)
					{
						refID = *(UInt32*)bufPos;
						nVars = *(UInt16*)(bufPos + 4);
						bufPos += 6;
						if (GetResolvedRefID(&refID) && (LookupFormByRefID(refID) || HasChangeData(refID)))
						{
							if (!ownersMap) ownersMap = s_auxVariablesPerm->Emplace(modIdx, AlignBucketCount(nRefs));
							AuxVarVarsMap *aVarsMap = ownersMap->Emplace(refID, AlignBucketCount(nVars));
							while (nVars)
							{
								buffer1 = *bufPos++;
								if (!buffer1)
									goto avSkipVars;
								namePos = bufPos;
								bufPos += buffer1;
								nElems = *(UInt16*)bufPos;
								*bufPos = 0;
								bufPos += 2;
								AuxVarValsArr *valsArr = aVarsMap->Emplace((char*)namePos, nElems);
								while (nElems)
								{
									buffer1 = *bufPos++;
									bufPos = valsArr->Append(buffer1)->ReadValData(bufPos);
									nElems--;
								}
								nVars--;
							}
						}
						else
						{
							while (nVars)
							{
								buffer1 = *bufPos++;
								bufPos += buffer1;
							avSkipVars:
								nElems = *(UInt16*)bufPos;
								bufPos += 2;
								while (nElems)
								{
									buffer1 = *bufPos++;
									if (buffer1 == 1)
										bufPos += 8;
									else if (buffer1 == 2)
										bufPos += 4;
									else bufPos += *(UInt16*)bufPos + 2;
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
						nVars = *(UInt16*)bufPos;
						bufPos += 2;
						while (nVars)
						{
							buffer1 = *bufPos++;
							bufPos += buffer1;
							nElems = *(UInt16*)bufPos;
							bufPos += 2;
							while (nElems)
							{
								buffer1 = *bufPos++;
								if (buffer1 == 1)
									bufPos += 8;
								else if (buffer1 == 2)
									bufPos += 4;
								else bufPos += *(UInt16*)bufPos + 2;
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
			bufPos = s_loadGameBuffer.Get(length);
			nRecs = *(UInt16*)bufPos;
			bufPos += 2;
			while (nRecs)
			{
				nRecs--;
				modIdx = *bufPos;
				nVars = *(UInt16*)(bufPos + 1);
				bufPos += 3;
				if ((modIdx > 5) && GetResolvedModIndex(&modIdx))
				{
					RefMapVarsMap *rVarsMap = nullptr;
					while (nVars)
					{
						buffer1 = *bufPos++;
						if (!buffer1)
							goto rmSkipVars;
						namePos = bufPos;
						bufPos += buffer1;
						nRefs = *(UInt16*)bufPos;
						*bufPos = 0;
						bufPos += 2;
						RefMapIDsMap *idsMap = nullptr;
						while (nRefs)
						{
							refID = *(UInt32*)bufPos;
							buffer1 = bufPos[4];
							bufPos += 5;
							if (GetResolvedRefID(&refID) && (LookupFormByRefID(refID) || HasChangeData(refID)))
							{
								if (!idsMap)
								{
									if (!rVarsMap) rVarsMap = s_refMapArraysPerm->Emplace(modIdx, AlignBucketCount(nVars));
									idsMap = rVarsMap->Emplace((char*)namePos, AlignBucketCount(nRefs));
								}
								bufPos = idsMap->Emplace(refID, buffer1)->ReadValData(bufPos);
							}
							else if (buffer1 == 1)
								bufPos += 8;
							else if (buffer1 == 2)
								bufPos += 4;
							else bufPos += *(UInt16*)bufPos + 2;
							nRefs--;
						}
						nVars--;
					}
				}
				else
				{
					while (nVars)
					{
						buffer1 = *bufPos++;
						bufPos += buffer1;
					rmSkipVars:
						nRefs = *(UInt16*)bufPos;
						bufPos += 2;
						while (nRefs)
						{
							buffer1 = bufPos[4];
							bufPos += 5;
							if (buffer1 == 1)
								bufPos += 8;
							else if (buffer1 == 2)
								bufPos += 4;
							else bufPos += *(UInt16*)bufPos + 2;
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
			bufPos = s_loadGameBuffer.Get(length);
			nRecs = *(UInt32*)bufPos;
			bufPos += 4;
			s_extraDataKeysMap->SetBucketCount(nRecs);
			while (nRecs)
			{
				nRecs--;
				UINT key = *(UInt32*)bufPos;
				refID = *(UInt32*)(bufPos + 4);
				nRefs = bufPos[8];
				UInt32 keySize = *(UInt32*)(bufPos + 9);
				bufPos += 13;
				if (GetResolvedRefID(&refID) && HasChangeData(refID))
				{
					ExtraJIPEntry *dataEntry = nullptr;
					while (nRefs)
					{
						nRefs--;
						modIdx = *bufPos;
						UInt32 entSize = *(UInt32*)(bufPos + 1);
						UInt32 strLen1 = *(UInt16*)(bufPos + 5);
						UInt32 strLen2 = *(UInt16*)(bufPos + 7);
						bufPos += 9;
						if (GetResolvedModIndex(&modIdx))
						{
							if (!dataEntry)
								dataEntry = s_extraDataKeysMap->Emplace(key, refID);
							ExtraJIPData *pData = &dataEntry->dataMap[modIdx];
							COPY_BYTES(pData, bufPos, entSize);
							pData->ResolvedRefIDs();
							if (strLen1)
								pData->strings[0].InitFromBuffer((char*)bufPos + entSize, strLen1);
							if (strLen2)
								pData->strings[1].InitFromBuffer((char*)bufPos + entSize + strLen1, strLen2);
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
			bufPos = s_loadGameBuffer.Get(length);
			nRecs = *(UInt16*)bufPos;
			bufPos += 2;
			while (nRecs)
			{
				nRecs--;
				refID = *(UInt32*)bufPos;
				UInt32 lnkID = *(UInt32*)(bufPos + 4);
				modIdx = bufPos[8];
				bufPos += 9;
				if (GetResolvedRefID(&refID) && GetResolvedRefID(&lnkID) && GetResolvedModIndex(&modIdx) && SetLinkedRefID(refID, lnkID, modIdx))
					s_linkedRefsTemp()[refID] = lnkID;
			}
		}
		else if (type == kJIPTag_SerializedVars)
		{
			if (version != 9)
				ReadRecordData(&s_serializedVars, length);
			else
				s_serializedVars.serializedFlags = ReadRecord32();
		}
		else if (type == kJIPTag_AppearanceUndo)
		{
			AppearanceUndo *aprUndo = (AppearanceUndo*)malloc(sizeof(AppearanceUndo));
			ReadRecordData(aprUndo->values0, 0x214);
			refID = ReadRecord32();
			if (!GetResolvedRefID(&refID) || !(aprUndo->race = (TESRace*)LookupFormByRefID(refID)))
				aprUndo->race = (TESRace*)LookupFormByRefID(0x19);
			refID = ReadRecord32();
			if (!GetResolvedRefID(&refID) || !(aprUndo->hair = (TESHair*)LookupFormByRefID(refID)))
			{
				refID = (aprUndo->flags & 1) ? 0x22E4E : 0x14B90;
				aprUndo->hair = (TESHair*)LookupFormByRefID(refID);
			}
			refID = ReadRecord32();
			if (!GetResolvedRefID(&refID) || !(aprUndo->eyes = (TESEyes*)LookupFormByRefID(refID)))
				aprUndo->eyes = (TESEyes*)LookupFormByRefID(0x4255);
			buffer1 = ReadRecord8();
			aprUndo->numParts = buffer1;
			if (buffer1)
			{
				BGSHeadPart **ptr = aprUndo->headParts = (BGSHeadPart**)malloc(buffer1 * 4);
				do
				{
					refID = ReadRecord32();
					if (GetResolvedRefID(&refID) && (*ptr = (BGSHeadPart*)LookupFormByRefID(refID)))
						ptr++;
					else aprUndo->numParts--;
				}
				while (--buffer1);
			}
			else aprUndo->headParts = nullptr;
			s_appearanceUndoMap()[(TESNPC*)g_thePlayer->baseForm] = aprUndo;
		}
		else if (type == kJIPTag_NPCPerks)
		{
			if (!s_NPCPerks || !(changedFlags & kChangedFlag_NPCPerks))
				continue;
			bufPos = s_loadGameBuffer.Get(length);
			nRecs = *(UInt16*)bufPos;
			bufPos += 2;
			s_NPCPerksInfoMap->SetBucketCount(nRecs);
			Actor *actor;
			BGSPerk *perk;
			while (nRecs)
			{
				nRecs--;
				refID = *(UInt32*)bufPos;
				bufPos += 4;
				buffer1 = *bufPos++;
				if (!GetResolvedRefID(&refID) || !(actor = (Actor*)LookupFormByRefID(refID)) || NOT_ACTOR(actor))
				{
					bufPos += buffer1 * 5;
					continue;
				}
				NPCPerksInfo *perksInfo = nullptr;
				while (buffer1)
				{
					buffer1--;
					buffer4 = *(UInt32*)bufPos;
					bufPos += 4;
					UInt8 rank = *bufPos++;
					if (!GetResolvedRefID(&buffer4) || !(perk = (BGSPerk*)LookupFormByRefID(buffer4)) || NOT_ID(perk, BGSPerk))
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
	UInt8 buffer1;
	UInt32 buffer2;

	if (buffer2 = s_scriptVariablesBuffer->Size())
	{
		WriteRecord(kJIPTag_ScriptVars, 9, &buffer2, 2);
		for (auto svOwnerIt = s_scriptVariablesBuffer->Begin(); svOwnerIt; ++svOwnerIt)
		{
			WriteRecord32(svOwnerIt.Key());
			WriteRecord16(svOwnerIt().Size());
			for (auto svVarIt = svOwnerIt().Begin(); svVarIt; ++svVarIt)
			{
				WriteRecord8(svVarIt().modIdx);
				buffer1 = StrLen(svVarIt.Key());
				WriteRecord8(buffer1);
				WriteRecordData(svVarIt.Key(), buffer1);
				WriteRecord64(&svVarIt().value->data);
			}
		}
	}
	if (buffer2 = s_auxVariablesPerm->Size())
	{
		WriteRecord(kJIPTag_AuxVars, JIP_VARS_VERSION, &buffer2, 2);
		for (auto avModIt = s_auxVariablesPerm->Begin(); avModIt; ++avModIt)
		{
			WriteRecord8(avModIt.Key());
			WriteRecord16(avModIt().Size());
			for (auto avOwnerIt = avModIt().Begin(); avOwnerIt; ++avOwnerIt)
			{
				WriteRecord32(avOwnerIt.Key());
				WriteRecord16(avOwnerIt().Size());
				for (auto avVarIt = avOwnerIt().Begin(); avVarIt; ++avVarIt)
				{
					buffer1 = StrLen(avVarIt.Key());
					WriteRecord8(buffer1);
					WriteRecordData(avVarIt.Key(), buffer1);
					WriteRecord16(avVarIt().Size());
					for (auto avValIt = avVarIt().Begin(); avValIt; ++avValIt)
						avValIt().WriteValData();
				}
			}
		}
	}
	if (buffer2 = s_refMapArraysPerm->Size())
	{
		WriteRecord(kJIPTag_RefMaps, JIP_VARS_VERSION, &buffer2, 2);
		for (auto rmModIt = s_refMapArraysPerm->Begin(); rmModIt; ++rmModIt)
		{
			WriteRecord8(rmModIt.Key());
			WriteRecord16(rmModIt().Size());
			for (auto rmVarIt = rmModIt().Begin(); rmVarIt; ++rmVarIt)
			{
				buffer1 = StrLen(rmVarIt.Key());
				WriteRecord8(buffer1);
				WriteRecordData(rmVarIt.Key(), buffer1);
				WriteRecord16(rmVarIt().Size());
				for (auto rmRefIt = rmVarIt().Begin(); rmRefIt; ++rmRefIt)
				{
					WriteRecord32(rmRefIt.Key());
					rmRefIt().WriteValData();
				}
			}
		}
	}
	if (buffer2 = s_extraDataKeysMap->Size())
	{
		WriteRecord(kJIPTag_ExtraData, ExtraJIP::kExtraJIP_Verion, &buffer2, 4);
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
	if (buffer2 = s_linkedRefModified->Size())
	{
		WriteRecord(kJIPTag_LinkedRefs, 9, &buffer2, 2);
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
		buffer1 = aprUndo->numParts;
		WriteRecord8(buffer1);
		if (buffer1)
		{
			BGSHeadPart **ptr = aprUndo->headParts;
			do
			{
				WriteRecord32((*ptr)->refID);
				ptr++;
			}
			while (--buffer1);
		}
	}
	if (buffer2 = s_NPCPerksInfoMap->Size())
	{
		Actor *actor;
		for (auto refIter = s_NPCPerksInfoMap->Begin(); refIter; ++refIter)
		{
			if ((actor = (Actor*)LookupFormByRefID(refIter.Key())) && IS_ACTOR(actor))
			{
				if (!refIter().perkRanks.Empty() && !actor->lifeState && (actor->isTeammate || !(((TESActorBase*)actor->baseForm)->baseData.flags & 8) || actor->GetRefNiNode()))
					goto isValid;
				actor->extraDataList.perksInfo = nullptr;
			}
			refIter.Remove();
		isValid:
			if (!--buffer2) break;
		}
		if (buffer2 = s_NPCPerksInfoMap->Size())
		{
			WriteRecord(kJIPTag_NPCPerks, 10, &buffer2, 2);
			for (auto refIter = s_NPCPerksInfoMap->Begin(); refIter; ++refIter)
			{
				WriteRecord32(refIter.Key());
				WriteRecord8(refIter().perkRanks.Size());
				for (auto perkIter = refIter().perkRanks.Begin(); perkIter; ++perkIter)
				{
					WriteRecord32(perkIter.Key()->refID);
					WriteRecord8(perkIter());
				}
				if (!--buffer2) break;
			}
		}
	}
}