#pragma once

DEFINE_COMMAND_PLUGIN(DisableNavMeshAlt, 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(EnableNavMeshAlt, 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetTerrainHeight, 0, 2, kParams_TwoFloats);
DEFINE_COMMAND_PLUGIN(GetFormDescription, 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetContainerRespawns, 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetContainerRespawns, 0, 2, kParams_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetExteriorCell, 0, 3, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetCellBuffered, 0, 1, kParams_OneOptionalForm);
DEFINE_CMD_COND_PLUGIN(GetGameDifficulty, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ToggleFirstPerson, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetFormDescription, 0, 22, kParams_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetPCFastTravelled, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetPCMovedCell, 0, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetPCDetectionState, 0, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetPipboyRadio, 0, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetPCUsingScope, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetIdleLoopTimes, 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetIdleLoopTimes, 0, 3, kParams_OneForm_TwoInts);
DEFINE_CMD_COND_PLUGIN(GetPCUsingIronSights, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetPCLastExteriorDoor, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SwapTextureEx, 0, 4, kParams_OneObjectRef_TwoStrings_OneOtionalInt);
DEFINE_COMMAND_PLUGIN(SetOnFastTravelEventHandler, 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetMoonTexture, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetMoonTexture, 0, 2, kParams_OneInt_OneString);
DEFINE_CMD_COND_PLUGIN(GetAlwaysRun, 0, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetAutoMove, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetOnPCTargetChangeEventHandler, 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(FreePlayer, GoodbyeWorldspace, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetLocalGravity, 0, 1, kParams_OneAxis);
DEFINE_COMMAND_PLUGIN(SetLocalGravityVector, 0, 3, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetReticlePos, 0, 2, kParams_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetReticleRange, 0, 2, kParams_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(SetOnDialogTopicEventHandler, 0, 3, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetGameDaysPassed, 0, 3, kParams_ThreeOptionalInts);
DEFINE_CMD_COND_PLUGIN(IsPCInCombat, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ToggleHardcoreTracking, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetGameDifficulty, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEnemyHealthTarget, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(MarkActivatorAshPile, 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetBufferedCells, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnLocationDiscoverEventHandler, 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnCraftingEventHandler, 0, 2, kParams_OneForm_OneInt);
DEFINE_CMD_COND_PLUGIN(IsInKillCam, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SwapObjectLOD, 0, 3, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWobblesRotation, 0, 3, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(SetGameHour, 0, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(StringToActorValue, 0, 1, kParams_OneString);
DEFINE_CMD_COND_PLUGIN(GetHardcoreTracking, 0, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetNoteRead, 0, 1, kParams_OneNote);
DEFINE_COMMAND_PLUGIN(SetOnNoteAddedEventHandler, 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetHardcoreStageThreshold, 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetHardcoreStageThreshold, 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetHardcoreStageEffect, 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetHardcoreStageEffect, 0, 2, kParams_OneForm_OneOptionalSpellItem);
DEFINE_COMMAND_PLUGIN(GetDebugModeState, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(FreezeTime, 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetConditionDamagePenalty, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetConditionDamagePenalty, 0, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(ToggleBipedSlotVisibility, 0, 2, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleImmortalMode, 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleCameraCollision, 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InitRockItLauncher, 0, 1, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(ToggleHitEffects, 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleNoMovementCombat, 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RewardXPExact, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ClearDeadActors, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetCameraMovement, 0, 2, kParams_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetReticleNode, 0, 2, kParams_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetInternalMarker, 0, 2, kParams_OneForm_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPointRayCastPos, 0, 10, kParams_FiveFloats_ThreeScriptVars_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(TogglePlayerSneaking, 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(PlaceModel, 0, 8, kParams_OneString_SixFloats_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(SetArmorConditionPenalty, 0, 1, kParams_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetReticlePosAlt, 0, 5, kParams_ThreeScriptVars_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetLightAmountAtPoint, 0, 3, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(TransformWorldToLocal, 0, 11, kParams_NineFloats_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetAnglesBetweenPoints, 0, 8, kParams_SixFloats_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetP2PRayCastRange, 0, 7, kParams_SixFloats_OneOptionalInt);

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
	NiPoint2 posXY;
	float tempRes = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posXY.x, &posXY.y))
		g_TES->GetTerrainHeight(&posXY, &tempRes);
	*result = tempRes;
	return true;
}

bool Cmd_GetFormDescription_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form)) resStr = form->GetDescriptionText();
	else resStr = nullptr;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_GetContainerRespawns_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCONT *container = nullptr;
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
	TESObjectCONT *container = nullptr;
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
	TESObjectCELL *cell = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
		return true;
	if (!cell)
	{
		if (!thisObj || !(cell = thisObj->parentCell))
			return true;
	}
	else if NOT_ID(cell, TESObjectCELL)
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
		if (bToggle == g_thePlayer->is3rdPerson)
			g_thePlayer->ToggleFirstPerson(bToggle);
	}
	return true;
}

bool Cmd_SetFormDescription_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	char *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, kCommandInfo_SetFormDescription.numParams, &form))
	{
		TESDescription *description = DYNAMIC_CAST(form, TESForm, TESDescription);
		if (description || (IS_ID(form, BGSNote) && (description = ((BGSNote*)form)->noteText)))
		{
			SetDescriptionAltText(description, buffer);
			*result = 1;
		}
	}
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
	if (s_pcFastTravelInformed().Empty())
		HOOK_MOD(PCFastTravel, true);
	if (s_pcFastTravelInformed().Insert(caller))
	{
		if (caller->jipFormFlags5 & kHookFormFlag5_FastTravelInformed) *result = 1;
		else
		{
			s_eventInformedObjects().Insert(caller->refID);
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
	if (s_pcCellChangeInformed().Empty())
		HOOK_SET(PCCellChange, true);
	if (s_pcCellChangeInformed().Insert(caller))
	{
		if (caller->jipFormFlags5 & kHookFormFlag5_CellChangeInformed) *result = 1;
		else
		{
			s_eventInformedObjects().Insert(caller->refID);
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
	RadioEntry *pipboyRadio = GameGlobals::PipboyRadio();
	if (pipboyRadio && pipboyRadio->radioRef) REFR_RES = pipboyRadio->radioRef->refID;
	else *result = 0;
	return true;
}

bool Cmd_GetPipboyRadio_Eval(COMMAND_ARGS_EVAL)
{
	RadioEntry *pipboyRadio = GameGlobals::PipboyRadio();
	*result = (pipboyRadio && (pipboyRadio->radioRef == thisObj)) ? 1 : 0;
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
		if (s_fastTravelEventScripts().Insert(script))
			HOOK_MOD(PCFastTravel, true);
	}
	else if (s_fastTravelEventScripts().Erase(script))
		HOOK_MOD(PCFastTravel, false);
	return true;
}

bool Cmd_GetMoonTexture_Execute(COMMAND_ARGS)
{
	UInt32 textureID;
	const char *texturePath;
	Sky *currSky = Sky::Get();
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &textureID) && (textureID <= 7) && currSky && currSky->masserMoon)
		texturePath = currSky->masserMoon->moonTexture[textureID].m_data;
	else texturePath = nullptr;
	AssignString(PASS_COMMAND_ARGS, texturePath);
	return true;
}

bool Cmd_SetMoonTexture_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 textureID;
	char path[0x80];
	Sky *currSky = Sky::Get();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &textureID, &path) || (textureID > 7) || !currSky) return true;
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
	currSky->RefreshMoon();
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
		if (s_targetChangeEventScripts().Insert(script))
			HOOK_MOD(SetPCTarget, true);
	}
	else if (s_targetChangeEventScripts().Erase(script))
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
		if (world) *result = world->gravity[axis - 'X'];
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

bool Cmd_GetReticlePos_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!g_thePlayer->parentCell)
		return true;
	int filter = -1;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS_EX;
	if (numArgs && !ExtractArgsEx(EXTRACT_ARGS_EX, &filter, &maxRange))
		return true;
	NiVector4 coords;
	if (coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, filter) || (numArgs >= 2))
	{
		ArrayElementL elements[3] = {coords.x, coords.y, coords.z};
		AssignCommandResult(CreateArray(elements, 3, scriptObj), result);
	}
	return true;
}

bool Cmd_GetReticleRange_Execute(COMMAND_ARGS)
{
	*result = -1;
	if (!g_thePlayer->parentCell)
		return true;
	int filter = -1;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS_EX;
	if (numArgs && !ExtractArgsEx(EXTRACT_ARGS_EX, &filter, &maxRange))
		return true;
	NiVector4 coords;
	if (coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, filter) || (numArgs >= 2))
		*result = Point3Distance(g_thePlayer->position, coords);
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
			if (s_dialogTopicEventMap().Insert(form, &callbacks))
				HOOK_MOD(RunResultScript, true);
			callbacks->Insert(script);
		}
		else
		{
			auto findTopic = s_dialogTopicEventMap().Find(form);
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
			auto iter = ((BGSListForm*)form)->list.Head();
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
		if (s_locationDiscoverEventScripts().Insert(script))
			HOOK_MOD(LocationDiscover, true);
	}
	else if (s_locationDiscoverEventScripts().Erase(script))
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
		if (s_itemCraftedEventScripts().Insert(script))
			HOOK_MOD(ItemCrafted, true);
	}
	else if (s_itemCraftedEventScripts().Erase(script))
		HOOK_MOD(ItemCrafted, false);
	return true;
}

bool Cmd_IsInKillCam_Execute(COMMAND_ARGS)
{
	*result = (VATSCameraData::Get()->mode == 4) && (g_thePlayer->killCamCooldown > 0);
	return true;
}

bool Cmd_IsInKillCam_Eval(COMMAND_ARGS_EVAL)
{
	*result = (VATSCameraData::Get()->mode == 4) && (g_thePlayer->killCamCooldown > 0);
	return true;
}

bool Cmd_SwapObjectLOD_Execute(COMMAND_ARGS)
{
	TESWorldSpace *worldSpc;
	SInt32 cellX, cellY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &worldSpc, &cellX, &cellY) && IS_ID(worldSpc, TESWorldSpace))
	{
		Coordinate cellXY(cellX, cellY);
		if (s_swapObjLODMap()[worldSpc].Insert(cellXY.xy))
			HOOK_SET(MakeObjLODPath, true);
	}
	return true;
}

bool Cmd_SetWobblesRotation_Execute(COMMAND_ARGS)
{
	/*static BSFadeNode **wobbleAnimations = nullptr;
	static QuaternionKey *wobbleAnimRotations[9] = {};
	NiVector3 rotPRY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &rotPRY.x, &rotPRY.y, &rotPRY.z))
	{
		if (!wobbleAnimations)
		{
			wobbleAnimations = (BSFadeNode**)0x11E0110;
			NiTransformData *transData;
			for (UInt32 index = 0; index < 9; index++)
			{
				if (!wobbleAnimations[index]) continue;
				transData = ((NiTransformInterpolator*)((NiTransformController*)wobbleAnimations[index]->m_controller)->interpolator)->transData;
				if ((transData->rotationKeyType == 3) && (transData->numRotationKeys == 3))
					wobbleAnimRotations[index] = (QuaternionKey*)transData->rotationKeys;
			}
		}
		QuaternionKey *rotationKeys = nullptr;
		UInt32 startIdx = 0;
		do
		{
			rotationKeys = wobbleAnimRotations[startIdx];
			if (rotationKeys) break;
		}
		while (++startIdx < 9);
		if (rotationKeys)
		{
			rotPRY *= GET_PS(8);
			NiQuaternion quaternion = rotPRY;
			rotationKeys[1].value = quaternion;
			rotationKeys[1].quaternion20 = quaternion;
			rotationKeys[1].quaternion30 = quaternion;
			rotationKeys[2].value = quaternion;
			rotationKeys[2].quaternion20 = quaternion;
			rotationKeys[2].quaternion30 = quaternion;
			rotPRY *= -0.5;
			quaternion = rotPRY;
			rotationKeys[0].quaternion20 = quaternion;
			rotationKeys[0].quaternion30 = quaternion;
			while (++startIdx < 8)
				if (wobbleAnimRotations[startIdx])
					memmove(wobbleAnimRotations[startIdx], rotationKeys, sizeof(QuaternionKey) * 3);
		}
	}*/
	return true;
}

bool Cmd_SetGameHour_Execute(COMMAND_ARGS)
{
	float newHour;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &newHour) && (newHour >= 0))
	{
		if (g_gameHour->data <= newHour)
			g_gameHour->data = newHour;
		else
			g_gameHour->data = 24.0F + newHour;
		s_forceHCNeedsUpdate = g_thePlayer->isHardcore && !(s_serializedFlags & kSerializedFlag_NoHardcoreTracking);
	}
	return true;
}

TempObject<UnorderedMap<const char*, UInt32>> s_actorValueIDsMap(0x80);

bool Cmd_StringToActorValue_Execute(COMMAND_ARGS)
{
	*result = -1;
	char avStr[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &avStr))
	{
		if (s_actorValueIDsMap().Empty())
		{
			ActorValueInfo *avInfo;
			for (UInt32 avCode = 0; avCode < 77; avCode++)
			{
				avInfo = ActorValueInfo::Array()[avCode];
				s_actorValueIDsMap()[avInfo->infoName] = avCode;
			}
		}
		UInt32 *idPtr = s_actorValueIDsMap().GetPtr(avStr);
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
		if (s_noteAddedEventScripts().Insert(script))
			HOOK_MOD(AddNote, true);
	}
	else if (s_noteAddedEventScripts().Erase(script))
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
	SpellItem *effect = nullptr;
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
	*result = s_condDmgPenalty;
	return true;
}

bool Cmd_SetConditionDamagePenalty_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		s_condDmgPenalty = value;
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

TESObjectWEAP *g_rockItLauncher = nullptr;

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
		cmp		byte ptr [ecx+0x28], 1
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

__declspec(naked) BGSProjectile* __fastcall GetWeaponProjectileHook(TESObjectWEAP *weapon, int EDX, Actor *actor)
{
	__asm
	{
		push	ecx
		push	dword ptr [esp+8]
		call	GetWeaponAmmoHook
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
		mov		ecx, ds:0x11D98D4
		cmp		ecx, g_rockItLauncher
		retn
	}
}

bool Cmd_InitRockItLauncher_Execute(COMMAND_ARGS)
{
	if (!g_rockItLauncher && ExtractArgsEx(EXTRACT_ARGS_EX, &g_rockItLauncher))
	{
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
	ProcessManager *procMngr = ProcessManager::Get();
	UInt32 count = procMngr->beginOffsets[0];
	MobileObject **objArray = procMngr->objects.data + count;
	count = procMngr->endOffsets[0] - count;
	Actor *actor;
	HighProcess *hiProcess;
	while (count)
	{
		actor = (Actor*)objArray[--count];
		if (!actor || NOT_ACTOR(actor) || (actor->lifeState != 2) || (actor->flags & 0x200000) || (actor->baseForm->flags & 0x400))
			continue;
		hiProcess = (HighProcess*)actor->baseProcess;
		if (hiProcess && !hiProcess->processLevel && !hiProcess->fadeType && (hiProcess->dyingTimer < 0) &&
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
				fMovX *= 1 / 1500.0;
				fMovY *= -1 / 1500.0;
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
		NiAVObject *rtclObject = GetRayCastObject(g_thePlayer->cameraPos, g_mainCamera->WorldRotate(), maxRange, filter & 0x3F);
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
			*result = s_internalMarkerIDs().HasKey(form->refID);
		else if (toggle)
			s_internalMarkerIDs().Insert(form->refID);
		else s_internalMarkerIDs().Erase(form->refID);
	}
	return true;
}

bool Cmd_GetPointRayCastPos_Execute(COMMAND_ARGS)
{
	*result = 0;
	NiVector3 pos;
	NiVector4 rot(_mm_setzero_ps());
	ResultVars outPos;
	UInt32 filter = 6;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &pos.x, &pos.y, &pos.z, &rot.x, &rot.z, &outPos.x, &outPos.y, &outPos.z, &filter, &maxRange))
	{
		NiMatrix33 rotMat;
		rotMat.FromEulerPRYInv(rot * GET_PS(8));
		if (rot.RayCastCoords(pos, rotMat + 1, maxRange, filter) || (numArgs >= 10))
		{
			outPos.Set(rot.PS());
			*result = 1;
		}
	}
	return true;
}

bool Cmd_TogglePlayerSneaking_Execute(COMMAND_ARGS)
{
	UInt32 toggle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &toggle))
		g_thePlayer->ToggleSneak(toggle != 0);
	return true;
}

bool Cmd_PlaceModel_Execute(COMMAND_ARGS)
{
	*result = 0;
	char modelPath[0x80];
	NiVector3 pos;
	NiVector4 rot;
	float scale = 1.0F;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modelPath, &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &scale))
	{
		PlayerCharacter *thePlayer = g_thePlayer;
		TESObjectCELL *parentCell = thePlayer->parentCell;
		if (parentCell && (!parentCell->worldSpace || (parentCell = parentCell->worldSpace->GetCellAtPos(thePlayer->position.PS()))))
		{
			NiNode *objParent = parentCell->Get3DNode(3);
			if (objParent)
			{
				NiNode *objNode = LoadModelCopy(modelPath);
				if (objNode)
				{
					objNode->RemoveCollision();
					objParent->AddObject(objNode, 1);
					objNode->LocalRotate().FromEulerPRYInv(rot * GET_PS(8));
					objNode->LocalTranslate() = pos;
					objNode->m_transformLocal.scale = scale;
					objNode->UpdateDownwardPass(kUpdateParams, 0);
					if (objNode->m_flags & 0x20000000)
						AddPointLights(objNode);
					*result = 1;
				}
			}
		}
	}
	return true;
}

bool Cmd_SetArmorConditionPenalty_Execute(COMMAND_ARGS)
{
	*result = s_condDRDTPenalty;
	float value;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		s_condDRDTPenalty = value;
	return true;
}

bool Cmd_GetReticlePosAlt_Execute(COMMAND_ARGS)
{
	*result = 0;
	ResultVars outPos;
	int filter = 6;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS;
	if (!g_thePlayer->parentCell || !ExtractArgsEx(EXTRACT_ARGS_EX, &outPos.x, &outPos.y, &outPos.z, &filter, &maxRange))
		return true;
	NiVector4 coords;
	if (coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, filter) || (numArgs >= 5))
	{
		outPos.Set(coords.PS());
		*result = 1;
	}
	return true;
}

bool Cmd_GetLightAmountAtPoint_Execute(COMMAND_ARGS)
{
	NiVector3 pos;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &pos.x, &pos.y, &pos.z))
		*result = GetLightAmountAtPoint(pos);
	else *result = 0;
	return true;
}

bool Cmd_TransformWorldToLocal_Execute(COMMAND_ARGS)
{
	NiVector3 origin, target;
	NiVector4 rotation;
	ResultVars outAngles;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &origin.x, &origin.y, &origin.z, &target.x, &target.y, &target.z, &rotation.x, &rotation.y, &rotation.z, &outAngles.x, &outAngles.z))
	{
		outAngles.y = outAngles.z;
		__m128 angles = _mm_mul_ps(rotation.PS(), GET_PS(8));
		angles = TransformWorldToLocal(origin, target, angles);
		outAngles.Set(angles, GET_PS(9));
	}
	return true;
}

bool Cmd_GetAnglesBetweenPoints_Execute(COMMAND_ARGS)
{
	NiVector3 point1, point2;
	ResultVars outAngles;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &point1.x, &point1.y, &point1.z, &point2.x, &point2.y, &point2.z, &outAngles.x, &outAngles.z))
	{
		outAngles.y = outAngles.z;
		__m128 angles = point1.GetAnglesBetweenPoints(point2);
		outAngles.Set(angles, GET_PS(9));
	}
	return true;
}

bool Cmd_GetP2PRayCastRange_Execute(COMMAND_ARGS)
{
	*result = 0;
	NiVector3 point1, point2;
	int filter = 6;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &point1.x, &point1.y, &point1.z, &point2.x, &point2.y, &point2.z, &filter))
	{
		NiMatrix33 rotMat;
		float length = rotMat.From2Points(point1, point2);
		NiVector4 rcPos;
		if (rcPos.RayCastCoords(point1, rotMat + 1, length, filter))
			*result = Point3Distance(rcPos, point2);
	}
	return true;
}