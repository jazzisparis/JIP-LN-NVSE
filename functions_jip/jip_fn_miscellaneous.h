#pragma once

DEFINE_COMMAND_PLUGIN(DisableNavMeshAlt, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(EnableNavMeshAlt, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetTerrainHeight, , 0, 2, kParams_TwoFloats);
DEFINE_COMMAND_PLUGIN(GetFormDescription, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetContainerRespawns, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetContainerRespawns, , 0, 2, kParams_JIP_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetExteriorCell, , 0, 3, kParams_JIP_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetCellBuffered, , 0, 1, kParams_JIP_OneOptionalCell);
DEFINE_CMD_ALT_COND_PLUGIN(GetGameDifficulty, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(ToggleFirstPerson, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetFormDescription, , 0, 22, kParams_JIP_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetPCFastTravelled, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetPCMovedCell, , 0, 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetPCDetectionState, , , 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetPipboyRadio, , , 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetPCUsingScope, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(GetIdleLoopTimes, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetIdleLoopTimes, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_CMD_ALT_COND_PLUGIN(GetPCUsingIronSights, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(GetPCLastExteriorDoor, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SwapTextureEx, , 0, 4, kParams_JIP_OneObjectRef_TwoStrings_OneOtionalInt);
DEFINE_COMMAND_PLUGIN(SetOnFastTravelEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetMoonTexture, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetMoonTexture, , 0, 2, kParams_JIP_OneInt_OneString);
DEFINE_CMD_ALT_COND_PLUGIN(GetAlwaysRun, , , 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetAutoMove, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(SetOnPCTargetChangeEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(FreePlayer, GoodbyeWorldspace, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetLocalGravity, , 0, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(SetLocalGravityVector, , 0, 3, kParams_JIP_ThreeFloats);
DEFINE_COMMAND_PLUGIN(SetOnKeyDownEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnKeyUpEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnControlDownEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnControlUpEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetReticlePos, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetReticleRange, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnDialogTopicEventHandler, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetGameDaysPassed, , 0, 3, kParams_JIP_ThreeOptionalInts);
DEFINE_CMD_ALT_COND_PLUGIN(IsPCInCombat, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(ToggleHardcoreTracking, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetGameDifficulty, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEnemyHealthTarget, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(MarkActivatorAshPile, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetBufferedCells, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnLocationDiscoverEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnCraftingEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_CMD_ALT_COND_PLUGIN(IsInKillCam, , , 0, NULL);
DEFINE_COMMAND_PLUGIN(SwapObjectLOD, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWobblesRotation, , 0, 3, kParams_JIP_ThreeFloats);
DEFINE_COMMAND_PLUGIN(SetGameHour, , 0, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(StringToActorValue, , 0, 1, kParams_OneString);
DEFINE_CMD_ALT_COND_PLUGIN(GetHardcoreTracking, , , 0, NULL);
DEFINE_CMD_ALT_COND_PLUGIN(GetNoteRead, , , 0, kParams_JIP_OneNote);
DEFINE_COMMAND_PLUGIN(SetOnNoteAddedEventHandler, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetHardcoreStageThreshold, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetHardcoreStageThreshold, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetHardcoreStageEffect, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetHardcoreStageEffect, , 0, 2, kParams_JIP_OneForm_OneOptionalSpellItem);
DEFINE_COMMAND_PLUGIN(GetDebugModeState, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(FreezeTime, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetConditionDamagePenalty, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetConditionDamagePenalty, , 0, 1, kParams_JIP_OneDouble);
DEFINE_COMMAND_PLUGIN(ToggleBipedSlotVisibility, , 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleImmortalMode, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleCameraCollision, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InitRockItLauncher, , 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(ToggleHitEffects, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleNoMovementCombat, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RewardXPExact, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ClearDeadActors, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCameraMovement, , 0, 2, kParams_JIP_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetReticleNode, , 0, 2, kParams_JIP_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetInternalMarker, , 0, 2, kParams_JIP_OneForm_OneOptionalInt);

bool Cmd_DisableNavMeshAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IS_ID(form, NavMesh))
		ThisCall(0x6C1150, CdeclCall<void*>(0x6C0720), form->refID);
	return true;
}

bool Cmd_EnableNavMeshAlt_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IS_ID(form, NavMesh))
		ThisCall(0x6C1130, CdeclCall<void*>(0x6C0720), form->refID);
	return true;
}

bool Cmd_GetTerrainHeight_Execute(COMMAND_ARGS)
{
	float posXY[2], tempRes = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posXY[0], &posXY[1]))
		g_TES->GetTerrainHeight(posXY, &tempRes);
	*result = tempRes;
	return true;
}

bool Cmd_GetFormDescription_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form)) resStr = form->GetDescriptionText();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_GetContainerRespawns_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCONT *container = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &container)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if IS_ID(container, TESObjectCONT)
		*result = (container->flags & 2) ? 1 : 0;
	return true;
}

bool Cmd_SetContainerRespawns_Execute(COMMAND_ARGS)
{
	UInt32 respawn;
	TESObjectCONT *container = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &respawn, &container)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if IS_ID(container, TESObjectCONT)
	{
		if (respawn) container->flags |= 2;
		else container->flags &= 0xFD;
	}
	return true;
}

bool Cmd_GetExteriorCell_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWorldSpace *wspc;
	SInt32 coordX, coordY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc, &coordX, &coordY) && wspc->cellMap)
	{
		Coordinate coord;
		coord.x = coordX;
		coord.y = coordY;
		TESObjectCELL *cell = wspc->cellMap->Lookup(coord.xy);
		if (cell) REFR_RES = cell->refID;
	}
	return true;
}

bool Cmd_GetCellBuffered_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &cell) || (!cell && (!thisObj || !(cell = thisObj->parentCell))))
		return true;
	TESObjectCELL **cellsBuffer = cell->worldSpace ? g_TES->exteriorsBuffer : g_TES->interiorsBuffer;
	if (cellsBuffer)
	{
		UInt32 maxVal = cell->worldSpace ? *(UInt32*)0x11C3C94 : *(UInt32*)0x11C3E3C;
		while (maxVal && *cellsBuffer)
		{
			if (*cellsBuffer == cell)
			{
				*result = 1;
				break;
			}
			cellsBuffer++;
			maxVal--;
		}
	}
	return true;
}

bool Cmd_GetGameDifficulty_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->gameDifficulty;
	return true;
}

bool Cmd_GetGameDifficulty_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_thePlayer->gameDifficulty;
	return true;
}

bool Cmd_ToggleFirstPerson_Execute(COMMAND_ARGS)
{
	UInt32 toggleON;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &toggleON))
	{
		bool bToggle = toggleON != 0;
		if (bToggle == g_thePlayer->bThirdPerson)
			g_thePlayer->ToggleFirstPerson(bToggle);
	}
	return true;
}

bool Cmd_SetFormDescription_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	char *buffer = GetStrArgBuffer();
	if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_SetFormDescription.numParams, &form))
		return true;
	TESDescription *description = DYNAMIC_CAST(form, TESForm, TESDescription);
	if (!description && (NOT_ID(form, BGSNote) || !(description = ((BGSNote*)form)->noteText))) return true;
	UInt16 newLen = StrLen(buffer);
	char **findDesc, *newDesc;
	if (!s_descriptionChanges.Insert(description, &findDesc))
	{
		newDesc = *findDesc;
		if (StrLen(newDesc) < newLen)
		{
			GameHeapFree(newDesc);
			newDesc = (char*)GameHeapAlloc(newLen + 1);
			*findDesc = newDesc;
		}
	}
	else *findDesc = newDesc = (char*)GameHeapAlloc(newLen + 1);
	StrCopy(newDesc, buffer);
	*g_currentDescription = NULL;
	HOOK_SET(GetDescription, true);
	*result = 1;
	return true;
}

bool Cmd_GetPCFastTravelled_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *caller = scriptObj->quest;
	if (!caller)
	{
		if (!thisObj) return true;
		caller = thisObj;
	}
	if (s_pcFastTravelInformed.Empty())
		HOOK_MOD(PCFastTravel, true);
	if (s_pcFastTravelInformed.Insert(caller))
	{
		if (caller->jipFormFlags5 & kHookFormFlag5_FastTravelInformed) *result = 1;
		else
		{
			s_eventInformedObjects.Insert(caller->refID);
			caller->jipFormFlags5 |= kHookFormFlag5_FastTravelInformed;
		}
	}
	return true;
}

bool Cmd_GetPCMovedCell_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *caller = scriptObj->quest;
	if (!caller)
	{
		if (!thisObj) return true;
		caller = thisObj;
	}
	if (s_pcCellChangeInformed.Empty())
		HOOK_SET(PCCellChange, true);
	if (s_pcCellChangeInformed.Insert(caller))
	{
		if (caller->jipFormFlags5 & kHookFormFlag5_CellChangeInformed) *result = 1;
		else
		{
			s_eventInformedObjects.Insert(caller->refID);
			caller->jipFormFlags5 |= kHookFormFlag5_CellChangeInformed;
		}
	}
	return true;
}

bool Cmd_GetPCDetectionState_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_thePlayer->GetDetectionState();
	return true;
}

bool Cmd_GetPCDetectionState_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->GetDetectionState();
	return true;
}

bool Cmd_GetPipboyRadio_Execute(COMMAND_ARGS)
{
	if (*g_pipboyRadio && (*g_pipboyRadio)->radioRef) REFR_RES = (*g_pipboyRadio)->radioRef->refID;
	else *result = 0;
	return true;
}

bool Cmd_GetPipboyRadio_Eval(COMMAND_ARGS_EVAL)
{
	*result = (*g_pipboyRadio && ((*g_pipboyRadio)->radioRef == thisObj)) ? 1 : 0;
	return true;
}

bool Cmd_GetPCUsingScope_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_HUDMainMenu->isUsingScope;
	return true;
}

bool Cmd_GetPCUsingScope_Execute(COMMAND_ARGS)
{
	*result = g_HUDMainMenu->isUsingScope;
	return true;
}

bool Cmd_GetIdleLoopTimes_Execute(COMMAND_ARGS)
{
	TESIdleForm *idle;
	UInt32 maxTimes;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idle, &maxTimes) && IS_ID(idle, TESIdleForm))
		*result = maxTimes ? idle->data.loopMax : idle->data.loopMin;
	else *result = 0;
	return true;
}

bool Cmd_SetIdleLoopTimes_Execute(COMMAND_ARGS)
{
	TESIdleForm *idle;
	UInt32 loopMin, loopMax;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &idle, &loopMin, &loopMax) || NOT_ID(idle, TESIdleForm)) return true;
	if (loopMin > 255) loopMin = 255;
	if (loopMax < loopMin) loopMax = loopMin;
	idle->data.loopMin = loopMin;
	idle->data.loopMax = loopMax;
	return true;
}

bool Cmd_GetPCUsingIronSights_Eval(COMMAND_ARGS_EVAL)
{
	*result = (g_thePlayer->ironSightNode && g_thePlayer->baseProcess->IsWeaponOut()) ? 1 : 0;
	return true;
}

bool Cmd_GetPCUsingIronSights_Execute(COMMAND_ARGS)
{
	*result = (g_thePlayer->ironSightNode && g_thePlayer->baseProcess->IsWeaponOut()) ? 1 : 0;
	return true;
}

bool Cmd_GetPCLastExteriorDoor_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->lastExteriorDoor) REFR_RES = g_thePlayer->lastExteriorDoor->refID;
	else *result = 0;
	return true;
}

bool Cmd_SwapTextureEx_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	char blockName[0x40], path[0x80];
	UInt32 texIdx = 0;
	char *pathBgn = StrLenCopy(path, "Textures\\", 9);
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr, &blockName, pathBgn, &texIdx))
	{
		StrCat(pathBgn, ".dds");
		refr->SwapTexture(blockName, path, texIdx);
	}
	return true;
}

bool Cmd_SetOnFastTravelEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_fastTravelEventScripts.Insert(script))
			HOOK_MOD(PCFastTravel, true);
	}
	else if (s_fastTravelEventScripts.Erase(script))
		HOOK_MOD(PCFastTravel, false);
	return true;
}

bool Cmd_GetMoonTexture_Execute(COMMAND_ARGS)
{
	UInt32 textureID;
	const char *texturePath;
	Sky *currSky = *g_currentSky;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &textureID) && (textureID <= 7) && currSky && currSky->masserMoon)
		texturePath = currSky->masserMoon->moonTexture[textureID].m_data;
	else texturePath = NULL;
	AssignString(PASS_COMMAND_ARGS, texturePath);
	return true;
}

bool Cmd_SetMoonTexture_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 textureID;
	char path[0x80];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &textureID, &path) || (textureID > 7) || !*g_currentSky) return true;
	const char *newTexture = CopyString(path);
	for (UInt8 idx = 0; idx < 8; idx++)
	{
		if (idx != textureID)
			s_moonTexturesSet[idx] = s_moonTexturesOverride[idx] ? s_moonTexturesOverride[idx] : kMoonTexturesDefault[idx];
		else
		{
			if (s_moonTexturesOverride[idx]) free(const_cast<char*>(s_moonTexturesOverride[idx]));
			s_moonTexturesOverride[idx] = newTexture;
			s_moonTexturesSet[idx] = newTexture;
		}
	}
	HOOK_SET(InitMoon, true);
	(*g_currentSky)->RefreshMoon();
	HOOK_SET(InitMoon, false);
	*result = 1;
	return true;
}

bool Cmd_GetAlwaysRun_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->alwaysRun;
	return true;
}

bool Cmd_GetAlwaysRun_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_thePlayer->alwaysRun;
	return true;
}

bool Cmd_GetAutoMove_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->autoMove;
	return true;
}

bool Cmd_GetAutoMove_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_thePlayer->autoMove;
	return true;
}

bool Cmd_SetOnPCTargetChangeEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_targetChangeEventScripts.Insert(script))
			HOOK_MOD(SetPCTarget, true);
	}
	else if (s_targetChangeEventScripts.Erase(script))
		HOOK_MOD(SetPCTarget, false);
	return true;
}

bool Cmd_FreePlayer_Execute(COMMAND_ARGS)
{
	GameHeapFree(g_thePlayer);
	return true;
}

bool Cmd_GetLocalGravity_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
	{
		hkpWorld *world = GethkpWorld();
		if (world) *result = ((float*)&world->gravity)[axis - 'X'];
	}
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetLocalGravityVector_Execute(COMMAND_ARGS)
{
	hkpWorld *world = GethkpWorld();
	if (world) ExtractArgsEx(EXTRACT_ARGS_EX, &world->gravity.x, &world->gravity.y, &world->gravity.z);
	return true;
}

UInt32 s_onKeyEventMask = 0;

bool SetOnKeyEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	UInt32 keyID = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &keyID) && IS_ID(script, Script))
		SetInputEventHandler(s_onKeyEventMask, script, keyID, addEvnt != 0);
	return true;
}

__declspec(naked) bool Cmd_SetOnKeyDownEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onKeyEventMask, kLNEventMask_OnKeyDown
		jmp		SetOnKeyEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnKeyUpEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onKeyEventMask, kLNEventMask_OnKeyUp
		jmp		SetOnKeyEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnControlDownEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onKeyEventMask, kLNEventMask_OnControlDown
		jmp		SetOnKeyEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnControlUpEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_onKeyEventMask, kLNEventMask_OnControlUp
		jmp		SetOnKeyEventHandler_Execute
	}
}

bool Cmd_GetReticlePos_Execute(COMMAND_ARGS)
{
	UInt32 filter = 6;
	if (NUM_ARGS)
	{
		ExtractArgsEx(EXTRACT_ARGS_EX, &filter);
		filter &= 0x3F;
	}
	NiVector3 coords;
	if (coords.RayCastCoords(&g_thePlayer->cameraPos, &g_sceneGraph->camera->m_worldRotate, 50000.0F, 0, filter))
	{
		ArrayElementL elements[3] = {coords.x, coords.y, coords.z};
		AssignCommandResult(CreateArray(elements, 3, scriptObj), result);
	}
	else *result = 0;
	return true;
}

bool Cmd_GetReticleRange_Execute(COMMAND_ARGS)
{
	UInt32 filter = 6;
	if (NUM_ARGS)
	{
		ExtractArgsEx(EXTRACT_ARGS_EX, &filter);
		filter &= 0x3F;
	}
	NiVector3 coords;
	if (coords.RayCastCoords(&g_thePlayer->cameraPos, &g_sceneGraph->camera->m_worldRotate, 50000.0F, 0, filter))
		*result = Vector3Distance(&coords, &g_thePlayer->cameraPos);
	else *result = -1;
	return true;
}

bool Cmd_SetOnDialogTopicEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &form) && IS_ID(script, Script) && (IS_ID(form, TESTopic) || IS_ID(form, TESTopicInfo)))
	{
		if (addEvnt)
		{
			EventCallbackScripts *callbacks;
			if (s_dialogTopicEventMap.Insert(form, &callbacks))
				HOOK_MOD(RunResultScript, true);
			callbacks->Insert(script);
		}
		else
		{
			auto findTopic = s_dialogTopicEventMap.Find(form);
			if (findTopic && findTopic().Erase(script) && findTopic().Empty())
			{
				findTopic.Remove();
				HOOK_MOD(RunResultScript, false);
			}
		}
	}
	return true;
}

bool Cmd_GetGameDaysPassed_Execute(COMMAND_ARGS)
{
	int bgnYear = 2281, bgnMonth = 10, bgnDay = 13;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bgnYear, &bgnMonth, &bgnDay) && (bgnMonth <= 12))
		*result = GetDaysPassed(bgnYear, bgnMonth - 1, bgnDay);
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_IsPCInCombat_Execute(COMMAND_ARGS)
{
	*result = g_thePlayer->pcInCombat && !g_thePlayer->pcUnseen;
	return true;
}

bool Cmd_IsPCInCombat_Eval(COMMAND_ARGS_EVAL)
{
	*result = g_thePlayer->pcInCombat && !g_thePlayer->pcUnseen;
	return true;
}

bool Cmd_ToggleHardcoreTracking_Execute(COMMAND_ARGS)
{
	UInt32 toggleON;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &toggleON))
	{
		if (toggleON) s_serializedFlags &= ~kSerializedFlag_NoHardcoreTracking;
		else s_serializedFlags |= kSerializedFlag_NoHardcoreTracking;
	}
	return true;
}

bool Cmd_SetGameDifficulty_Execute(COMMAND_ARGS)
{
	UInt32 difficulty;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &difficulty) && (difficulty <= 4))
		g_thePlayer->gameDifficulty = difficulty;
	return true;
}

bool Cmd_GetEnemyHealthTarget_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (g_HUDMainMenu->healthTarget)
		REFR_RES = g_HUDMainMenu->healthTarget->refID;
	return true;
}

bool Cmd_MarkActivatorAshPile_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		if IS_ID(form, TESObjectACTI)
		{
			form->SetJIPFlag(kHookFormFlag6_IsAshPile, true);
			form->flags |= 1;
		}
		else if IS_ID(form, BGSListForm)
		{
			ListNode<TESForm> *iter = ((BGSListForm*)form)->list.Head();
			do
			{
				form = iter->data;
				if (form && IS_ID(form, TESObjectACTI))
				{
					form->SetJIPFlag(kHookFormFlag6_IsAshPile, true);
					form->flags |= 1;
				}
			}
			while (iter = iter->next);
		}
	}
	return true;
}

bool Cmd_GetBufferedCells_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 interiors;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &interiors))
	{
		TempElements *tmpElements = GetTempElements();
		tmpElements->Clear();
		TESObjectCELL **cellsBuffer = interiors ? g_TES->interiorsBuffer : g_TES->exteriorsBuffer, *cell;
		if (cellsBuffer)
		{
			UInt32 maxVal = interiors ? *(UInt32*)0x11C3E3C : *(UInt32*)0x11C3C94;
			while (maxVal && (cell = *cellsBuffer))
			{
				
				tmpElements->Append(cell);
				cellsBuffer++;
				maxVal--;
			}
		}
		AssignCommandResult(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_SetOnLocationDiscoverEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_locationDiscoverEventScripts.Insert(script))
			HOOK_MOD(LocationDiscover, true);
	}
	else if (s_locationDiscoverEventScripts.Erase(script))
		HOOK_MOD(LocationDiscover, false);
	return true;
}

bool Cmd_SetOnCraftingEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_itemCraftedEventScripts.Insert(script))
			HOOK_MOD(ItemCrafted, true);
	}
	else if (s_itemCraftedEventScripts.Erase(script))
		HOOK_MOD(ItemCrafted, false);
	return true;
}

bool Cmd_IsInKillCam_Execute(COMMAND_ARGS)
{
	*result = (g_VATSCameraData->mode == 4) && (g_thePlayer->killCamCooldown > 0);
	return true;
}

bool Cmd_IsInKillCam_Eval(COMMAND_ARGS_EVAL)
{
	*result = (g_VATSCameraData->mode == 4) && (g_thePlayer->killCamCooldown > 0);
	return true;
}

bool Cmd_SwapObjectLOD_Execute(COMMAND_ARGS)
{
	TESWorldSpace *worldSpc;
	SInt32 cellX, cellY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &worldSpc, &cellX, &cellY) && IS_ID(worldSpc, TESWorldSpace))
	{
		Coordinate cellXY(cellX, cellY);
		if (s_swapObjLODMap[(UInt32)worldSpc].Insert(cellXY.xy))
			HOOK_SET(MakeObjLODPath, true);
	}
	return true;
}

BSFadeNode **g_wobbleAnimations = NULL;
QuaternionKey *g_wobbleAnimRotations[9] = {};

bool Cmd_SetWobblesRotation_Execute(COMMAND_ARGS)
{
	NiVector3 rotYPR;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &rotYPR.x, &rotYPR.y, &rotYPR.z))
	{
		if (!g_wobbleAnimations)
		{
			g_wobbleAnimations = (BSFadeNode**)0x11E0110;
			NiTransformData *transData;
			for (UInt32 index = 0; index < 9; index++)
			{
				if (!g_wobbleAnimations[index]) continue;
				transData = ((NiTransformInterpolator*)((NiTransformController*)g_wobbleAnimations[index]->m_controller)->interpolator)->transData;
				if ((transData->rotationKeyType == 3) && (transData->numRotationKeys == 3))
					g_wobbleAnimRotations[index] = (QuaternionKey*)transData->rotationKeys;
			}
		}
		QuaternionKey *rotationKeys = NULL;
		UInt32 startIdx = 0;
		do
		{
			rotationKeys = g_wobbleAnimRotations[startIdx];
			if (rotationKeys) break;
		}
		while (++startIdx < 9);
		if (rotationKeys)
		{
			NiQuaternion quaternion;
			rotYPR.x *= kFltPId180;
			rotYPR.y *= kFltPId180;
			rotYPR.z *= kFltPId180;
			rotYPR.ToQuaternion(quaternion);
			rotationKeys[1].value = quaternion;
			rotationKeys[1].quaternion20 = quaternion;
			rotationKeys[1].quaternion30 = quaternion;
			rotationKeys[2].value = quaternion;
			rotationKeys[2].quaternion20 = quaternion;
			rotationKeys[2].quaternion30 = quaternion;
			rotYPR.x *= -0.5;
			rotYPR.y *= -0.5;
			rotYPR.z *= -0.5;
			rotYPR.ToQuaternion(quaternion);
			rotationKeys[0].quaternion20 = quaternion;
			rotationKeys[0].quaternion30 = quaternion;
			while (++startIdx < 8)
				if (g_wobbleAnimRotations[startIdx])
					memmove(g_wobbleAnimRotations[startIdx], rotationKeys, sizeof(QuaternionKey) * 3);
		}
	}
	return true;
}

bool Cmd_SetGameHour_Execute(COMMAND_ARGS)
{
	float newHour;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &newHour))
	{
		if (g_gameHour->data <= newHour)
			g_gameHour->data = newHour;
		else if (newHour >= 0)
			g_gameHour->data = 24.0F + newHour;
	}
	return true;
}

UnorderedMap<const char*, UInt32> s_actorValueIDsMap(0x80);

bool Cmd_StringToActorValue_Execute(COMMAND_ARGS)
{
	*result = -1;
	char avStr[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &avStr))
	{
		if (s_actorValueIDsMap.Empty())
		{
			ActorValueInfo *avInfo;
			for (UInt32 avCode = 0; avCode < 77; avCode++)
			{
				avInfo = g_actorValueInfoArray[avCode];
				s_actorValueIDsMap[avInfo->infoName] = avCode;
			}
		}
		UInt32 *idPtr = s_actorValueIDsMap.GetPtr(avStr);
		if (idPtr) *result = (int)*idPtr;
	}
	return true;
}

bool Cmd_GetHardcoreTracking_Execute(COMMAND_ARGS)
{
	*result = (s_serializedFlags & kSerializedFlag_NoHardcoreTracking) ? 0 : 1;
	return true;
}

bool Cmd_GetHardcoreTracking_Eval(COMMAND_ARGS_EVAL)
{
	*result = (s_serializedFlags & kSerializedFlag_NoHardcoreTracking) ? 0 : 1;
	return true;
}

bool Cmd_GetNoteRead_Execute(COMMAND_ARGS)
{
	BGSNote *note;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &note))
		*result = note->read;
	else *result = 0;
	return true;
}

bool Cmd_GetNoteRead_Eval(COMMAND_ARGS_EVAL)
{
	*result = ((BGSNote*)arg1)->read;
	return true;
}

bool Cmd_SetOnNoteAddedEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) || NOT_ID(script, Script)) return true;
	if (addEvnt)
	{
		if (s_noteAddedEventScripts.Insert(script))
			HOOK_MOD(AddNote, true);
	}
	else if (s_noteAddedEventScripts.Erase(script))
		HOOK_MOD(AddNote, false);
	return true;
}

bool __fastcall IsHardcoreStage(TESForm *form)
{
	return (form->typeID == kFormType_BGSRadiationStage) || (form->typeID >= kFormType_BGSDehydrationStage);
}

bool Cmd_GetHardcoreStageThreshold_Execute(COMMAND_ARGS)
{
	BGSRadiationStage *hcStage;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hcStage) && IsHardcoreStage(hcStage))
		*result = (int)hcStage->threshold;
	else *result = 0;
	return true;
}

bool Cmd_SetHardcoreStageThreshold_Execute(COMMAND_ARGS)
{
	BGSRadiationStage *hcStage;
	UInt32 threshold;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hcStage, &threshold) && IsHardcoreStage(hcStage))
		hcStage->threshold = threshold;
	return true;
}

bool Cmd_GetHardcoreStageEffect_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSRadiationStage *hcStage;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hcStage) && IsHardcoreStage(hcStage) && hcStage->effect)
		REFR_RES = hcStage->effect->refID;
	return true;
}

bool Cmd_SetHardcoreStageEffect_Execute(COMMAND_ARGS)
{
	BGSRadiationStage *hcStage;
	SpellItem *effect = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &hcStage, &effect) && IsHardcoreStage(hcStage))
		hcStage->effect = effect;
	return true;
}

const UInt32 kDebugModeBytes[] =
{
	0x11E07BA,	// TGM
	0x11F1220,	// TAI
	0x11A4880,	// TCAI
	0x11C3C0D,	// TCL
	0x11CAE44	// TB
};

bool Cmd_GetDebugModeState_Execute(COMMAND_ARGS)
{
	UInt32 modeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modeID) && (modeID <= 4))
		*result = *(UInt8*)kDebugModeBytes[modeID];
	else *result = 0;
	return true;
}

bool Cmd_FreezeTime_Execute(COMMAND_ARGS)
{
	*result = g_OSGlobals->freezeTime;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (g_OSGlobals->freezeTime == !toggle))
		g_OSGlobals->freezeTime = toggle != 0;
	return true;
}

bool Cmd_GetConditionDamagePenalty_Execute(COMMAND_ARGS)
{
	*result = *g_condDmgPenalty;
	return true;
}

bool Cmd_SetConditionDamagePenalty_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*g_condDmgPenalty = value;
	return true;
}

bool Cmd_ToggleBipedSlotVisibility_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt8 numArgs = NUM_ARGS;
	UInt32 slotID, toggle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &slotID, &toggle) && (slotID <= 19))
	{
		slotID = 1 << slotID;
		if (numArgs > 1)
		{
			if (toggle) s_bipedSlotVisibility |= slotID;
			else s_bipedSlotVisibility &= ~slotID;
		}
		if (s_bipedSlotVisibility & slotID)
			*result = 1;
	}
	return true;
}

bool Cmd_ToggleImmortalMode_Execute(COMMAND_ARGS)
{
	*result = s_playerMinHPMode;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle))
	{
		s_playerMinHPMode = toggle;
		HOOK_SET(PlayerMinHealth, toggle == 1);
		if (toggle == 2) s_disableHitEffects |= 2;
		else s_disableHitEffects &= 1;
	}
	return true;
}

bool Cmd_ToggleCameraCollision_Execute(COMMAND_ARGS)
{
	static bool cameraCollision = true;
	*result = cameraCollision;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (!toggle == cameraCollision))
	{
		cameraCollision = !cameraCollision;
		SafeWrite8(0x94A34E, toggle ? 0x74 : 0xEB);
	}
	return true;
}

TESObjectWEAP *g_rockItLauncher = NULL;

__declspec(naked) BGSProjectile* __fastcall GetWeaponProjectileHook(TESObjectWEAP *weapon, int EDX, Actor *actor)
{
	__asm
	{
		push	ecx
		push	dword ptr [esp+8]
		CALL_EAX(0x525980)
		pop		ecx
		test    eax, eax
		jz		weapProj
		cmp		byte ptr [eax+4], kFormType_TESObjectMISC
		jz		weapProj
		mov		eax, [eax+0xB4]
		test	eax, eax
		jnz		done
	weapProj:
		mov		eax, [ecx+0x118]
	done:
		retn	4
	}
}

__declspec(naked) TESForm* __fastcall GetWeaponAmmoHook(TESObjectWEAP *weapon, int EDX, Actor *actor)
{
	__asm
	{
		push	ecx
		cmp		ecx, g_rockItLauncher
		jz		retnNull
		mov		ecx, [esp+8]
		test	ecx, ecx
		jz		baseWeap
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		baseWeap
		cmp		dword ptr [ecx+0x28], 1
		ja		baseWeap
		xor		edx, edx
		mov		eax, [ecx+0x114]
		test	eax, eax
		jz		nullWeap
		mov		edx, [eax+8]
		cmp		edx, g_rockItLauncher
		jz		retnNull
	nullWeap:
		mov		eax, [ecx+0x118]
		test	eax, eax
		jz		baseWeap
		mov		eax, [eax+8]
		cmp		byte ptr [eax+4], kFormType_TESAmmo
		jz		done
		cmp		eax, edx
		jnz		done
	baseWeap:
		pop		ecx
		add		ecx, 0xA4
		CALL_EAX(0x474920)
		retn	4
	retnNull:
		xor		eax, eax
	done:
		pop		ecx
		retn	4
	}
}

__declspec(naked) void RILAmmoFixHook()
{
	__asm
	{
		mov		ecx, [eax+8]
		cmp		ecx, g_rockItLauncher
		jnz		done
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) void PCAmmoSwitchHook()
{
	__asm
	{
		mov		ecx, ds:[0x11D98D4]
		cmp		ecx, g_rockItLauncher
		retn
	}
}

bool Cmd_InitRockItLauncher_Execute(COMMAND_ARGS)
{
	static bool installed = false;
	if (!installed && ExtractArgsEx(EXTRACT_ARGS_EX, &g_rockItLauncher))
	{
		installed = true;
		WriteRelJump(0x525980, (UInt32)GetWeaponAmmoHook);
		WriteRelJump(0x525A90, (UInt32)GetWeaponProjectileHook);
		WritePushRetRelJump(0x946310, 0x946326, (UInt32)RILAmmoFixHook);
		if (*(UInt8*)0x781EE6 != 0xEB)
			WritePushRetRelJump(0x940998, 0x9409B2, (UInt32)PCAmmoSwitchHook);
	}
	return true;
}

bool Cmd_ToggleHitEffects_Execute(COMMAND_ARGS)
{
	*result = !s_disableHitEffects;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle))
	{
		if (toggle) s_disableHitEffects &= 2;
		else s_disableHitEffects |= 1;
	}
	return true;
}

bool Cmd_ToggleNoMovementCombat_Execute(COMMAND_ARGS)
{
	*result = s_noMovementCombat;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle))
		s_noMovementCombat = toggle != 0;
	return true;
}

bool Cmd_RewardXPExact_Execute(COMMAND_ARGS)
{
	int xpAmount;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &xpAmount))
	{
		g_thePlayer->ModActorValue(kAVCode_XP, xpAmount, 0);
		ThisCall(0xAD7FA0, *(BSWin32Audio**)0x11F6D98, (const char*)0x106F354, 0x121);
	}
	return true;
}

bool Cmd_ClearDeadActors_Execute(COMMAND_ARGS)
{
	UInt32 count = g_processManager->beginOffsets[0];
	MobileObject **objArray = g_processManager->objects.data + count;
	count = g_processManager->endOffsets[0] - count;
	Actor *actor;
	HighProcess *hiProcess;
	while (count)
	{
		actor = (Actor*)objArray[--count];
		if (!actor || NOT_ACTOR(actor) || (actor->lifeState != 2) || (actor->flags & 0x200000) || (actor->baseForm->flags & 0x400))
			continue;
		hiProcess = (HighProcess*)actor->baseProcess;
		if (hiProcess && !hiProcess->processLevel && !hiProcess->fadeType && (hiProcess->flt330 < 0) && 
			!actor->extraDataList.HasType(kExtraData_EnableStateChildren) && !ThisCall<bool>(0x577DE0, actor))
			ThisCall(0x8FEB60, hiProcess, actor);
	}
	return true;
}

bool Cmd_GetCameraMovement_Execute(COMMAND_ARGS)
{
	*result = 0;
	ScriptVar *outX, *outY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY))
	{
		bool controller = s_controllerReady;
		int iMovX, iMovY;
		if (controller)
		{
			iMovX = *(short*)0x11F35B4;
			if ((iMovX >= -s_deadZoneRS) && (iMovX <= s_deadZoneRS))
				iMovX = 0;
			iMovY = *(short*)0x11F35B6;
			if ((iMovY >= -s_deadZoneRS) && (iMovY <= s_deadZoneRS))
				iMovY = 0;
		}
		else
		{
			iMovX = g_inputGlobals->currMouseMovementX;
			iMovY = g_inputGlobals->currMouseMovementY;
		}
		if (iMovX || iMovY)
		{
			if (iMovY && *(bool*)0x11E0A60)
				iMovY = -iMovY;
			double mouseSensitivity = *(float*)0x11E0A70;
			double fMovX = iMovX * mouseSensitivity;
			double fMovY = iMovY * mouseSensitivity;
			if (controller)
			{
				fMovX /= 1500;
				fMovY /= -1500;
			}
			outX->data.num = fMovX;
			outY->data.num = fMovY;
			*result = 1;
		}
		else
		{
			outX->data.num = 0;
			outY->data.num = 0;
		}
	}
	return true;
}

bool Cmd_GetReticleNode_Execute(COMMAND_ARGS)
{
	const char *nodeName = "";
	float maxRange = 50000.0F;
	UInt32 filter = 6;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &maxRange, &filter))
	{
		filter &= 0x3F;
		NiAVObject *rtclObject = GetRayCastObject(&g_thePlayer->cameraPos, &g_sceneGraph->camera->m_worldRotate, maxRange, 0, filter);
		if (rtclObject) nodeName = rtclObject->GetName();
	}
	AssignString(PASS_COMMAND_ARGS, nodeName);
	return true;
}

bool Cmd_SetInternalMarker_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	int toggle = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &toggle))
	{
		if IS_REFERENCE(form)
			form = ((TESObjectREFR*)form)->baseForm;
		if (toggle < 0)
			*result = s_internalMarkerIDs.HasKey(form->refID);
		else if (toggle)
			s_internalMarkerIDs.Insert(form->refID);
		else s_internalMarkerIDs.Erase(form->refID);
	}
	return true;
}