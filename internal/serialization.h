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

	if (!s_attachedLightsMap.Empty())
	{
		for (auto lghtIter = s_attachedLightsMap.Begin(); lghtIter; ++lghtIter)
		{
			if (LookupFormByRefID(lghtIter.Key()) != lghtIter().refrPtr) continue;
			for (auto nodeIter = lghtIter().nodeNames.Begin(); nodeIter; ++nodeIter)
				RemoveAttachedLight(lghtIter().refrPtr, *nodeIter);
		}
		s_attachedLightsMap.Clear();
	}

	if (!s_swapObjLODMap.Empty())
	{
		s_swapObjLODMap.Clear();
		HOOK_SET(MakeObjLODPath, false);
	}
}

void __fastcall ReadDataDump(UInt16 type)
{
	if (type == 4) ReadRecordData(&type, 2);
	else type = 4;
	ReadRecordData(s_strValBuffer, type);
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

	UInt32 type, version, length, buffer4, refID;
	UInt8 buffer1, modIdx;
	UInt16 nRecs, nRefs, nVars;

	while (GetNextRecordInfo(&type, &version, &length))
	{
		switch (type)
		{
			case 'VSPJ':
			{
				TESForm *form;
				Script *script;
				ScriptEventList *eventList;
				UInt32 value[2], *pVal;
				ScriptVar *var;
				ReadRecordData(&nRecs, 2);
				while (nRecs)
				{
					nRecs--;
					ReadRecordData(&buffer4, 4);
					if (!ResolveRefID(buffer4, &refID) || !(form = LookupFormByRefID(refID)) || !form->GetScriptAndEventList(&script, &eventList)) continue;
					ReadRecordData(&nVars, 2);
					while (nVars)
					{
						nVars--;
						ReadRecordData(&modIdx, 1);
						ReadRecordData(&buffer1, 1);
						ReadRecordData(s_strArgBuffer, buffer1);
						s_strArgBuffer[buffer1] = 0;
						ReadRecordData(value, 8);
						if (!ResolveRefID(modIdx << 24, &buffer4)) continue;
						if (var = script->AddVariable(eventList, refID, buffer4 >> 24))
						{
							if (*value && !value[1])
							{
								if (ResolveRefID(*value, value))
								{
									pVal = (UInt32*)&var->data;
									*pVal = *value;
									pVal[1] = 0;
									continue;
								}
								*value = 0;
							}
							var->data = *(double*)value;
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
				ReadRecordData(&nRecs, 2);
				while (nRecs)
				{
					nRecs--;
					ReadRecordData(&buffer1, 1);
					if (!ResolveRefID(buffer1 << 24, &buffer4)) continue;
					ownersMap = NULL;
					modIdx = buffer4 >> 24;
					ReadRecordData(&nRefs, 2);
					while (nRefs)
					{
						ReadRecordData(&refID, 4);
						ReadRecordData(&nVars, 2);
						if (ResolveRefID(refID, &refID) && LookupFormByRefID(refID))
						{
							if (!ownersMap) ownersMap = s_auxVariablesPerm.Emplace(modIdx, nRefs);
							aVarsMap = ownersMap->Emplace(refID, nVars);
						}
						else aVarsMap = NULL;
						while (nVars)
						{
							valsArr = NULL;
							ReadRecordData(&buffer1, 1);
							ReadRecordData(s_strArgBuffer, buffer1);
							s_strArgBuffer[buffer1] = 0;
							ReadRecordData(&nElems, 2);
							while (nElems)
							{
								ReadRecordData(&buffer1, 1);
								if (aVarsMap)
								{
									if (!valsArr) valsArr = aVarsMap->Emplace(s_strArgBuffer, nElems);
									valsArr->Append(buffer1);
								}
								else ReadDataDump(buffer1);
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
				ReadRecordData(&nRecs, 2);
				while (nRecs)
				{
					nRecs--;
					ReadRecordData(&buffer1, 1);
					if (!ResolveRefID(buffer1 << 24, &buffer4)) continue;
					rVarsMap = NULL;
					modIdx = buffer4 >> 24;
					ReadRecordData(&nVars, 2);
					while (nVars)
					{
						idsMap = NULL;
						ReadRecordData(&buffer1, 1);
						ReadRecordData(s_strArgBuffer, buffer1);
						s_strArgBuffer[buffer1] = 0;
						ReadRecordData(&nRefs, 2);
						while (nRefs)
						{
							ReadRecordData(&refID, 4);
							ReadRecordData(&buffer1, 1);
							if (ResolveRefID(refID, &refID))
							{
								if (!idsMap)
								{
									if (!rVarsMap) rVarsMap = s_refMapArraysPerm.Emplace(modIdx, nVars);
									idsMap = rVarsMap->Emplace(s_strArgBuffer, nRefs);
								}
								idsMap->Emplace(refID, buffer1);
							}
							else ReadDataDump(buffer1);
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
				ReadRecordData(&nRecs, 2);
				while (nRecs)
				{
					nRecs--;
					ReadRecordData(&refID, 4);
					ReadRecordData(&lnkID, 4);
					ReadRecordData(&buffer1, 1);
					if (ResolveRefID(refID, &refID) && ResolveRefID(lnkID, &lnkID) &&
						ResolveRefID(buffer1 << 24, &buffer4) && SetLinkedRefID(refID, lnkID, buffer4 >> 24))
						s_linkedRefsTemp[refID] = lnkID;
				}
				break;
			}
			case 'FGPJ':
			{
				ReadRecordData(&s_serializedFlags, 4);
				break;
			}
			case 'UAPJ':
			{
				AppearanceUndo *aprUndo = (AppearanceUndo*)malloc(sizeof(AppearanceUndo));
				ReadRecordData(aprUndo->values0, 0x214);
				ReadRecordData(&refID, 4);
				if (!ResolveRefID(refID, &refID) || !(aprUndo->race = (TESRace*)LookupFormByRefID(refID)))
					aprUndo->race = (TESRace*)LookupFormByRefID(0x19);
				ReadRecordData(&refID, 4);
				if (!ResolveRefID(refID, &refID) || !(aprUndo->hair = (TESHair*)LookupFormByRefID(refID)))
				{
					refID = (aprUndo->flags & 1) ? 0x22E4E : 0x14B90;
					aprUndo->hair = (TESHair*)LookupFormByRefID(refID);
				}
				ReadRecordData(&refID, 4);
				if (!ResolveRefID(refID, &refID) || !(aprUndo->eyes = (TESEyes*)LookupFormByRefID(refID)))
					aprUndo->eyes = (TESEyes*)LookupFormByRefID(0x4255);
				ReadRecordData(&buffer1, 1);
				aprUndo->numParts = buffer1;
				if (buffer1)
				{
					BGSHeadPart **ptr = aprUndo->headParts = (BGSHeadPart**)malloc(buffer1 * 4);
					do
					{
						ReadRecordData(&refID, 4);
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
		DoDistributeWeaponModsLoadGame();
}

void SaveGameCallback(void*)
{
	UInt8 buffer1;
	UInt16 buffer2;
	UInt32 numKey;

	StrCopy(s_lastLoadedPath, GetSavePath());
	s_dataChangedFlags = 0;

	if (buffer2 = s_scriptVariablesBuffer.Size())
	{
		WriteRecord('VSPJ', 9, &buffer2, 2);
		for (auto svOwnerIt = s_scriptVariablesBuffer.Begin(); svOwnerIt; ++svOwnerIt)
		{
			numKey = svOwnerIt.Key();
			WriteRecordData(&numKey, 4);
			buffer2 = svOwnerIt().Size();
			WriteRecordData(&buffer2, 2);
			for (auto svVarIt = svOwnerIt().Begin(); svVarIt; ++svVarIt)
			{
				WriteRecordData(&svVarIt().modIdx, 1);
				buffer1 = StrLen(svVarIt.Key());
				WriteRecordData(&buffer1, 1);
				WriteRecordData(svVarIt.Key(), buffer1);
				WriteRecordData(&svVarIt().value->data, 8);
			}
		}
	}
	if (buffer2 = s_auxVariablesPerm.Size())
	{
		WriteRecord('VAPJ', 9, &buffer2, 2);
		for (auto avModIt = s_auxVariablesPerm.Begin(); avModIt; ++avModIt)
		{
			buffer1 = avModIt.Key();
			WriteRecordData(&buffer1, 1);
			buffer2 = avModIt().Size();
			WriteRecordData(&buffer2, 2);
			for (auto avOwnerIt = avModIt().Begin(); avOwnerIt; ++avOwnerIt)
			{
				numKey = avOwnerIt.Key();
				WriteRecordData(&numKey, 4);
				buffer2 = avOwnerIt().Size();
				WriteRecordData(&buffer2, 2);
				for (auto avVarIt = avOwnerIt().Begin(); avVarIt; ++avVarIt)
				{
					buffer1 = StrLen(avVarIt.Key());
					WriteRecordData(&buffer1, 1);
					WriteRecordData(avVarIt.Key(), buffer1);
					buffer2 = avVarIt().Size();
					WriteRecordData(&buffer2, 2);
					for (auto avValIt = avVarIt().Begin(); avValIt; ++avValIt)
						avValIt().WriteValData();
				}
			}
		}
	}
	if (buffer2 = s_refMapArraysPerm.Size())
	{
		WriteRecord('MRPJ', 9, &buffer2, 2);
		for (auto rmModIt = s_refMapArraysPerm.Begin(); rmModIt; ++rmModIt)
		{
			buffer1 = rmModIt.Key();
			WriteRecordData(&buffer1, 1);
			buffer2 = rmModIt().Size();
			WriteRecordData(&buffer2, 2);
			for (auto rmVarIt = rmModIt().Begin(); rmVarIt; ++rmVarIt)
			{
				buffer1 = StrLen(rmVarIt.Key());
				WriteRecordData(&buffer1, 1);
				WriteRecordData(rmVarIt.Key(), buffer1);
				buffer2 = rmVarIt().Size();
				WriteRecordData(&buffer2, 2);
				for (auto rmRefIt = rmVarIt().Begin(); rmRefIt; ++rmRefIt)
				{
					numKey = rmRefIt.Key();
					WriteRecordData(&numKey, 4);
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
			numKey = lrRefIt.Key();
			WriteRecordData(&numKey, 4);
			WriteRecordData(&lrRefIt(), 5);
		}
	}
	if (s_serializedFlags)
		WriteRecord('FGPJ', 9, &s_serializedFlags, 4);
	AppearanceUndo *aprUndo = s_appearanceUndoMap.Get((TESNPC*)g_thePlayer->baseForm);
	if (aprUndo)
	{
		WriteRecord('UAPJ', 9, aprUndo->values0, 0x214);
		WriteRecordData(&aprUndo->race->refID, 4);
		WriteRecordData(&aprUndo->hair->refID, 4);
		WriteRecordData(&aprUndo->eyes->refID, 4);
		buffer1 = aprUndo->numParts;
		WriteRecordData(&buffer1, 1);
		if (buffer1)
		{
			BGSHeadPart **ptr = aprUndo->headParts;
			do
			{
				WriteRecordData(&(*ptr)->refID, 4);
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
			s_pcCurrCell0 = NULL;
			s_pcCurrCell = NULL;
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