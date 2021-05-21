#pragma once

enum
{
	kChangedFlag_AuxVars =		1 << 0,
	kChangedFlag_RefMaps =		1 << 1,
	kChangedFlag_LinkedRefs =	1 << 2,

	kChangedFlag_All =			kChangedFlag_AuxVars | kChangedFlag_RefMaps | kChangedFlag_LinkedRefs,
};

void DoLoadGameCleanup()
{
	if (g_thePlayer->teammateCount)
	{
		ListNode<PerkRank> *perkIter = g_thePlayer->perkRanksTM.Head();
		PerkRank *perkRank;
		do
		{
			perkRank = perkIter->data;
			if (perkRank && perkRank->perk)
				ThisCall(0x5EB6A0, perkRank->perk, g_thePlayer, 0, perkRank->rank, 1);
		}
		while (perkIter = perkIter->next);
	}

	s_scriptVariablesBuffer.Clear();

	if (s_dataChangedFlags & kChangedFlag_AuxVars) s_auxVariablesPerm.Clear();
	if (s_dataChangedFlags & kChangedFlag_RefMaps) s_refMapArraysPerm.Clear();
	if (s_dataChangedFlags & kChangedFlag_LinkedRefs) s_linkedRefModified.Clear();
	s_linkedRefsTemp.Clear();

	if ((s_dataChangedFlags == kChangedFlag_All) && !s_resolvedGlobals.Empty())
	{
		for (auto globIter = s_resolvedGlobals.Begin(); globIter; ++globIter)
			globIter->jipFormFlags6 = 0;
		s_resolvedGlobals.Clear();
	}
	s_dataChangedFlags = 0;
	s_serializedFlags = 0;

	auto pcAprUndo = s_appearanceUndoMap.Find((TESNPC*)g_thePlayer->baseForm);
	if (pcAprUndo)
	{
		pcAprUndo->Destroy();
		pcAprUndo.Remove();
	}

	HOOK_SET(StartCombat, false);
	if (s_forceCombatTargetMap.Clear())
		HOOK_SET(SetCombatTarget, false);
	HOOK_SET(TeleportWithPC, false);
	if (HOOK_SET(EquipItem, false))
	{
		HOOK_SET(ReEquipAll, false);
		HOOK_SET(WeaponSwitchSelect, false);
		HOOK_SET(WeaponSwitchUnequip, false);
		HOOK_SET(GetPreferedWeapon, false);
	}
	if (s_forceDetectionValueMap.Clear())
		HOOK_SET(GetDetectionValue, false);
	HOOK_SET(AddVATSTarget, false);

	if (s_fireWeaponEventMap.Empty()) HOOK_SET(RemoveAmmo, false);
	else s_hookInfos[kHook_RemoveAmmo].userCount = s_fireWeaponEventMap.Size();

	TESForm *form;
	if (!s_eventInformedObjects.Empty())
	{
		for (auto userIter = s_eventInformedObjects.Begin(); userIter; ++userIter)
		{
			form = LookupFormByRefID(*userIter);
			if (form) form->jipFormFlags5 = 0;
		}
		s_eventInformedObjects.Clear();

		if (s_pcFastTravelInformed.Clear())
			HOOK_MOD(PCFastTravel, false);
		if (s_pcCellChangeInformed.Clear())
			HOOK_SET(PCCellChange, false);
	}
	
	if (s_refMapMarkersCount)
	{
		while (--s_refMapMarkersCount)
			s_refMapMarkersList.RemoveNth(0);
		s_refMapMarkersList.Init();
		HOOK_SET(CreateMapMarkers, false);
	}

	if (!s_scriptWaitInfoMap.Empty())
	{
		for (auto waitIter = s_scriptWaitInfoMap.Begin(); waitIter; ++waitIter)
		{
			form = LookupFormByRefID(waitIter.Key());
			if (form) form->jipFormFlags5 = 0;
		}
		s_scriptWaitInfoMap.Clear();
		HOOK_SET(ScriptRunner, false);
		HOOK_SET(EvalEventBlock, false);
		s_scriptWaitInfo = NULL;
	}

	for (auto flagsIter = s_jipFormFlagsMap.Begin(); flagsIter; ++flagsIter)
	{
		form = LookupFormByRefID(flagsIter.Key());
		if (form && *flagsIter) form->jipFormFlags6 = *flagsIter;
		else flagsIter.Remove();
	}

	if (!s_effectItemFree.Empty())
	{
		for (auto effIter = s_effectItemFree.Begin(); effIter; ++effIter)
			GameHeapFree(*effIter);
		s_effectItemFree.Clear();
	}

	for (auto lgtIter = s_activePtLights.Begin(); lgtIter; ++lgtIter)
		if ((lgtIter->extraFlags & 0x80) && lgtIter->m_parent)
			lgtIter->m_parent->RemoveObject(*lgtIter);

	if (!s_swapObjLODMap.Empty())
	{
		s_swapObjLODMap.Clear();
		HOOK_SET(MakeObjLODPath, false);
	}

	if (!s_extraCamerasMap.Empty())
	{
		for (auto camIter = s_extraCamerasMap.Begin(); camIter; ++camIter)
		{
			if (camIter->m_parent)
				camIter->m_parent->RemoveObject(*camIter);
			camIter->Destructor(true);
		}
		s_extraCamerasMap.Clear();
	}
}

void __fastcall RestoreLinkedRefs(UnorderedMap<UInt32, UInt32> *tempMap = NULL)
{
	if (s_linkedRefDefault.Empty()) return;
	UInt32 key;
	auto linkIter = s_linkedRefDefault.Begin();
	while (linkIter)
	{
		key = linkIter.Key();
		++linkIter;
		if (!tempMap || !tempMap->HasKey(key))
			SetLinkedRefID(key);
	}
}

char s_lastLoadedPath[MAX_PATH];

void LoadGameCallback(void*)
{
	const char *currentPath = GetSavePath();
	if (strcmp(s_lastLoadedPath, currentPath))
	{
		StrCopy(s_lastLoadedPath, currentPath);
		s_dataChangedFlags = kChangedFlag_All;
	}
	UInt8 changedFlags = s_dataChangedFlags;
	DoLoadGameCleanup();

	UInt32 type, length, buffer4, refID, skipSize;
	UInt8 buffer1, modIdx;
	UInt16 nRecs, nRefs, nVars;
	char varName[0x50];

	while (GetNextRecordInfo(&type, &s_serializedVersion, &length))
	{
		switch (type)
		{
			case 'VSPJ':
			{
				TESForm *form;
				Script *script;
				ScriptEventList *eventList;
				ScriptVar *var;
				VarData varData;
				nRecs = ReadRecord16();
				while (nRecs)
				{
					nRecs--;
					refID = ReadRecord32();
					if (!ResolveRefID(refID, &refID) || !(form = LookupFormByRefID(refID)) || !form->GetScriptAndEventList(&script, &eventList)) continue;
					nVars = ReadRecord16();
					while (nVars)
					{
						nVars--;
						modIdx = ReadRecord8();
						buffer1 = ReadRecord8();
						ReadRecordData(varName, buffer1);
						varName[buffer1] = 0;
						ReadRecord64(&varData);
						if (!ResolveRefID(modIdx << 24, &buffer4)) continue;
						if (var = script->AddVariable(varName, eventList, refID, buffer4 >> 24))
						{
							if (varData.refID && !varData.pad && !ResolveRefID(varData.refID, &varData.refID))
								varData.refID = 0;
							var->data.num = varData.num;
						}
					}
				}
				break;
			}
			case 'VAPJ':
			{
				if (!(changedFlags & kChangedFlag_AuxVars)) continue;
				AuxVarOwnersMap *ownersMap;
				AuxVarVarsMap *aVarsMap;
				AuxVarValsArr *valsArr;
				UInt16 nElems;
				skipSize = (s_serializedVersion < 10) ? 4 : 8;
				nRecs = ReadRecord16();
				while (nRecs)
				{
					nRecs--;
					buffer1 = ReadRecord8();
					if (!ResolveRefID(buffer1 << 24, &buffer4)) continue;
					ownersMap = NULL;
					modIdx = buffer4 >> 24;
					nRefs = ReadRecord16();
					while (nRefs)
					{
						refID = ReadRecord32();
						nVars = ReadRecord16();
						if (ResolveRefID(refID, &refID) && LookupFormByRefID(refID))
						{
							if (!ownersMap) ownersMap = s_auxVariablesPerm.Emplace(modIdx, nRefs);
							aVarsMap = ownersMap->Emplace(refID, nVars);
						}
						else aVarsMap = NULL;
						while (nVars)
						{
							valsArr = NULL;
							buffer1 = ReadRecord8();
							ReadRecordData(varName, buffer1);
							varName[buffer1] = 0;
							nElems = ReadRecord16();
							while (nElems)
							{
								buffer1 = ReadRecord8();
								if (aVarsMap)
								{
									if (!valsArr) valsArr = aVarsMap->Emplace(varName, nElems);
									valsArr->Append(buffer1);
								}
								else if (buffer1 == 1)
									SkipNBytes(skipSize);
								else if (buffer1 == 2)
									SkipNBytes(4);
								else SkipNBytes(ReadRecord16());
								nElems--;
							}
							nVars--;
						}
						nRefs--;
					}
				}
				break;
			}
			case 'MRPJ':
			{
				if (!(changedFlags & kChangedFlag_RefMaps)) continue;
				RefMapVarsMap *rVarsMap;
				RefMapIDsMap *idsMap;
				skipSize = (s_serializedVersion < 10) ? 4 : 8;
				nRecs = ReadRecord16();
				while (nRecs)
				{
					nRecs--;
					buffer1 = ReadRecord8();
					if (!ResolveRefID(buffer1 << 24, &buffer4)) continue;
					rVarsMap = NULL;
					modIdx = buffer4 >> 24;
					nVars = ReadRecord16();
					while (nVars)
					{
						idsMap = NULL;
						buffer1 = ReadRecord8();
						ReadRecordData(varName, buffer1);
						varName[buffer1] = 0;
						nRefs = ReadRecord16();
						while (nRefs)
						{
							refID = ReadRecord32();
							buffer1 = ReadRecord8();
							if (ResolveRefID(refID, &refID))
							{
								if (!idsMap)
								{
									if (!rVarsMap) rVarsMap = s_refMapArraysPerm.Emplace(modIdx, nVars);
									idsMap = rVarsMap->Emplace(varName, nRefs);
								}
								idsMap->Emplace(refID, buffer1);
							}
							else if (buffer1 == 1)
								SkipNBytes(skipSize);
							else if (buffer1 == 2)
								SkipNBytes(4);
							else SkipNBytes(ReadRecord16());
							nRefs--;
						}
						nVars--;
					}
				}
				break;
			}
			case 'RLPJ':
			{
				if (!(changedFlags & kChangedFlag_LinkedRefs)) continue;
				UInt32 lnkID;
				nRecs = ReadRecord16();
				while (nRecs)
				{
					nRecs--;
					refID = ReadRecord32();
					lnkID = ReadRecord32();
					buffer1 = ReadRecord8();
					if (ResolveRefID(refID, &refID) && ResolveRefID(lnkID, &lnkID) &&
						ResolveRefID(buffer1 << 24, &buffer4) && SetLinkedRefID(refID, lnkID, buffer4 >> 24))
						s_linkedRefsTemp[refID] = lnkID;
				}
				break;
			}
			case 'FGPJ':
			{
				s_serializedFlags = ReadRecord32();
				break;
			}
			case 'UAPJ':
			{
				AppearanceUndo *aprUndo = (AppearanceUndo*)malloc(sizeof(AppearanceUndo));
				ReadRecordData(aprUndo->values0, 0x214);
				refID = ReadRecord32();
				if (!ResolveRefID(refID, &refID) || !(aprUndo->race = (TESRace*)LookupFormByRefID(refID)))
					aprUndo->race = (TESRace*)LookupFormByRefID(0x19);
				refID = ReadRecord32();
				if (!ResolveRefID(refID, &refID) || !(aprUndo->hair = (TESHair*)LookupFormByRefID(refID)))
				{
					refID = (aprUndo->flags & 1) ? 0x22E4E : 0x14B90;
					aprUndo->hair = (TESHair*)LookupFormByRefID(refID);
				}
				refID = ReadRecord32();
				if (!ResolveRefID(refID, &refID) || !(aprUndo->eyes = (TESEyes*)LookupFormByRefID(refID)))
					aprUndo->eyes = (TESEyes*)LookupFormByRefID(0x4255);
				buffer1 = ReadRecord8();
				aprUndo->numParts = buffer1;
				if (buffer1)
				{
					BGSHeadPart **ptr = aprUndo->headParts = (BGSHeadPart**)malloc(buffer1 * 4);
					do
					{
						refID = ReadRecord32();
						if (ResolveRefID(refID, &refID) && (*ptr = (BGSHeadPart*)LookupFormByRefID(refID)))
							ptr++;
						else aprUndo->numParts--;
					}
					while (--buffer1);
				}
				else aprUndo->headParts = NULL;
				s_appearanceUndoMap[(TESNPC*)g_thePlayer->baseForm] = aprUndo;
				break;
			}
			default:
				break;
		}
	}
	if (changedFlags & kChangedFlag_LinkedRefs)
		RestoreLinkedRefs(&s_linkedRefsTemp);
	if (s_NPCWeaponMods && !(g_thePlayer->actorFlags & 0x10000000))
	{
		g_thePlayer->actorFlags |= 0x10000000;
		auto actorIter = g_processManager->highActors.Head();
		Actor *actor;
		do
		{
			if (!(actor = actorIter->data) || (actor->actorFlags & 0x10000000))
				continue;
			actor->actorFlags |= 0x10000000;
			if (!actor->isTeammate)
				DistributeWeaponMods(actor);
		}
		while (actorIter = actorIter->next);
	}
}

void SaveGameCallback(void*)
{
	UInt8 buffer1;
	UInt16 buffer2;

	StrCopy(s_lastLoadedPath, GetSavePath());
	s_dataChangedFlags = 0;

	if (buffer2 = s_scriptVariablesBuffer.Size())
	{
		WriteRecord('VSPJ', 9, &buffer2, 2);
		for (auto svOwnerIt = s_scriptVariablesBuffer.Begin(); svOwnerIt; ++svOwnerIt)
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
	if (buffer2 = s_auxVariablesPerm.Size())
	{
		WriteRecord('VAPJ', 10, &buffer2, 2);
		for (auto avModIt = s_auxVariablesPerm.Begin(); avModIt; ++avModIt)
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
	if (buffer2 = s_refMapArraysPerm.Size())
	{
		WriteRecord('MRPJ', 10, &buffer2, 2);
		for (auto rmModIt = s_refMapArraysPerm.Begin(); rmModIt; ++rmModIt)
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
	if (buffer2 = s_linkedRefModified.Size())
	{
		WriteRecord('RLPJ', 9, &buffer2, 2);
		for (auto lrRefIt = s_linkedRefModified.Begin(); lrRefIt; ++lrRefIt)
		{
			WriteRecord32(lrRefIt.Key());
			WriteRecord32(lrRefIt().linkID);
			WriteRecord8(lrRefIt().modIdx);
		}
	}
	if (s_serializedFlags)
		WriteRecord('FGPJ', 9, &s_serializedFlags, 4);
	AppearanceUndo *aprUndo = s_appearanceUndoMap.Get((TESNPC*)g_thePlayer->baseForm);
	if (aprUndo)
	{
		WriteRecord('UAPJ', 9, aprUndo->values0, 0x214);
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
}

void NewGameCallback(void*)
{
	s_dataChangedFlags = kChangedFlag_All;
	DoLoadGameCleanup();
	RestoreLinkedRefs();
	s_lastLoadedPath[0] = 0;
}

void CleanMLCallbacks()
{
	for (auto iter = s_mainLoopCallbacks.Begin(); iter; ++iter)
		if (iter->flags & 8) iter->bRemove = true;
}

void NVSEMessageHandler(NVSEMessagingInterface::Message *nvseMsg)
{
	switch (nvseMsg->type)
	{
		case NVSEMessagingInterface::kMessage_PostLoad:
			MemCopy = memcpy;
			MemMove = memmove;

			InitJIPHooks();
			InitLNHooks();
			InitGamePatches();
			InitCmdPatches();
			MainLoopAddCallback(DeferredInit);
			break;
		case NVSEMessagingInterface::kMessage_ExitGame:
			JIPScriptRunner::RunScriptFiles('xg');
			break;
		case NVSEMessagingInterface::kMessage_ExitToMainMenu:
			CleanMLCallbacks();
			JIPScriptRunner::RunScriptFiles('mx');
			break;
		case NVSEMessagingInterface::kMessage_LoadGame:
			JIPScriptRunner::RunScriptFiles('lg');
			break;
		case NVSEMessagingInterface::kMessage_SaveGame:
			JIPScriptRunner::RunScriptFiles('sg');
			break;
		case NVSEMessagingInterface::kMessage_Precompile:
			break;
		case NVSEMessagingInterface::kMessage_PreLoadGame:
			CleanMLCallbacks();
			s_gameLoadFlagLN = true;
			HOOK_SET(OnRagdoll, false);
			s_onRagdollEventScripts.Clear();
			s_pcCurrCell0 = NULL;
			s_pcCurrCell = NULL;
			s_lastInterior = NULL;
			s_pcRootWorld = NULL;
			HOOK_SET(SynchronizePosition, false);
			s_syncPositionRef = NULL;
			break;
		case NVSEMessagingInterface::kMessage_ExitGame_Console:
			JIPScriptRunner::RunScriptFiles('xg');
			break;
		case NVSEMessagingInterface::kMessage_PostLoadGame:
			break;
		case NVSEMessagingInterface::kMessage_PostPostLoad:
			break;
		case NVSEMessagingInterface::kMessage_RuntimeScriptError:
			break;
		case NVSEMessagingInterface::kMessage_DeleteGame:
			break;
		case NVSEMessagingInterface::kMessage_RenameGame:
			break;
		case NVSEMessagingInterface::kMessage_RenameNewGame:
			break;
		case NVSEMessagingInterface::kMessage_NewGame:
			JIPScriptRunner::RunScriptFiles('ng');
			break;
	}
}