#pragma once

DEFINE_COMMAND_PLUGIN(DisableNavMeshAlt, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(EnableNavMeshAlt, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetTerrainHeight, 0, kParams_TwoFloats);
DEFINE_COMMAND_PLUGIN(GetFormDescription, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetContainerRespawns, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetContainerRespawns, 0, kParams_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetExteriorCell, 0, kParams_OneWorldspace_TwoInts);
DEFINE_COMMAND_PLUGIN(GetCellBuffered, 0, kParams_OneOptionalForm);
DEFINE_CMD_COND_PLUGIN(GetGameDifficulty, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ToggleFirstPerson, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetFormDescription, 0, kParams_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetPCFastTravelled, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetPCMovedCell, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetPCDetectionState, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetPipboyRadio, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetPCUsingScope, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetIdleLoopTimes, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetIdleLoopTimes, 0, kParams_OneForm_TwoInts);
DEFINE_CMD_COND_PLUGIN(GetPCUsingIronSights, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetPCLastExteriorDoor, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SwapTextureEx, 0, kParams_OneObjectRef_TwoStrings_OneOtionalInt);
DEFINE_COMMAND_PLUGIN(SetOnFastTravelEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetMoonTexture, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetMoonTexture, 0, kParams_OneInt_OneString);
DEFINE_CMD_COND_PLUGIN(GetAlwaysRun, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetAutoMove, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetOnPCTargetChangeEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(FreePlayer, GoodbyeWorldspace, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetLocalGravity, 0, kParams_OneAxis);
DEFINE_COMMAND_PLUGIN(SetLocalGravityVector, 0, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetReticlePos, 0, kParams_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetReticleRange, 0, kParams_OneOptionalInt_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetOnDialogTopicEventHandler, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetGameDaysPassed, 0, kParams_ThreeOptionalInts);
DEFINE_CMD_COND_PLUGIN(IsPCInCombat, 0, nullptr);
DEFINE_COMMAND_PLUGIN(ToggleHardcoreTracking, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetGameDifficulty, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetEnemyHealthTarget, 0, nullptr);
DEFINE_COMMAND_PLUGIN(MarkActivatorAshPile, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetBufferedCells, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnLocationDiscoverEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnCraftingEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_CMD_COND_PLUGIN(IsInKillCam, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SwapObjectLOD, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetWobblesRotation, 0, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(SetGameHour, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(StringToActorValue, 0, kParams_OneString);
DEFINE_CMD_COND_PLUGIN(GetHardcoreTracking, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(GetNoteRead, 0, kParams_OneNote);
DEFINE_COMMAND_PLUGIN(SetOnNoteAddedEventHandler, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetHardcoreStageThreshold, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetHardcoreStageThreshold, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetHardcoreStageEffect, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetHardcoreStageEffect, 0, kParams_OneForm_OneOptionalSpellItem);
DEFINE_COMMAND_PLUGIN(GetDebugModeState, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(FreezeTime, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetConditionDamagePenalty, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetConditionDamagePenalty, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(ToggleBipedSlotVisibility, 0, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(ToggleImmortalMode, TIMM, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleCameraCollision, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InitRockItLauncher, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(ToggleHitEffects, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleNoMovementCombat, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RewardXPExact, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ClearDeadActors, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetCameraMovement, 0, kParams_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetReticleNode, 0, kParams_OneOptionalFloat_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetInternalMarker, 0, kParams_OneForm_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetPointRayCastPos, 0, kParams_FiveFloats_ThreeScriptVars_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(TogglePlayerSneaking, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(PlaceModel, 0, kParams_OneString_SixFloats_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(SetArmorConditionPenalty, 0, kParams_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetReticlePosAlt, 0, kParams_ThreeScriptVars_OneOptionalInt_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(GetLightAmountAtPoint, 0, kParams_ThreeFloats);
DEFINE_COMMAND_PLUGIN(TransformWorldToLocal, 0, kParams_NineFloats_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetAnglesBetweenPoints, 0, kParams_SixFloats_TwoScriptVars);
DEFINE_COMMAND_PLUGIN(GetP2PRayCastRange, 0, kParams_SixFloats_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(SetDamageToArmorMaxPercent, DTAMP, 0, kParams_OneOptionalFloat);
DEFINE_COMMAND_PLUGIN(AssignKeyword, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(HasKeyword, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetKeywordForms, 0, kParams_OneString);
DEFINE_CMD_COND_ONLY(HasKeywordCond, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ToggleDepthClear, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetReticleTargetLimb, 0, kParams_OneOptionalFloat);

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
	TESObjectCONT *container = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &container)) return true;
	if (!container)
	{
		if (!thisObj) return true;
		container = (TESObjectCONT*)thisObj->baseForm;
	}
	if (IS_ID(container, TESObjectCONT) && (container->flags & 2))
		*result = 1;
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
	TESWorldSpace *wspc;
	SInt32 coordX, coordY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &wspc, &coordX, &coordY) && wspc->cellMap)
		if (TESObjectCELL *cell = wspc->cellMap->Lookup(Coordinate(coordX, coordY)))
			REFR_RES = cell->refID;
	return true;
}

bool Cmd_GetCellBuffered_Execute(COMMAND_ARGS)
{
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
		UInt32 maxVal = cell->worldSpace ? INIS_UINT(uExterior_Cell_Buffer_General) : INIS_UINT(uInterior_Cell_Buffer_General);
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
	TESForm *caller = scriptObj->quest;
	if (!caller)
	{
		if (!thisObj) return true;
		caller = thisObj;
	}
	if (s_pcFastTravelInformed->Empty())
		HOOK_INC(PCFastTravel);
	if (s_pcFastTravelInformed->Insert(caller))
		if (caller->jipFormFlags5 & kHookFormFlag5_FastTravelInformed)
			*result = 1;
		else
		{
			s_eventInformedObjects->Insert(caller->refID);
			caller->jipFormFlags5 |= kHookFormFlag5_FastTravelInformed;
		}
	return true;
}

bool Cmd_GetPCMovedCell_Execute(COMMAND_ARGS)
{
	TESForm *caller = scriptObj->quest;
	if (!caller)
	{
		if (!thisObj) return true;
		caller = thisObj;
	}
	if (s_pcCellChangeInformed->Empty())
		HOOK_SET(PCCellChange, true);
	if (s_pcCellChangeInformed->Insert(caller))
		if (caller->jipFormFlags5 & kHookFormFlag5_CellChangeInformed)
			*result = 1;
		else
		{
			s_eventInformedObjects->Insert(caller->refID);
			caller->jipFormFlags5 |= kHookFormFlag5_CellChangeInformed;
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
	if (RadioEntry *pipboyRadio = GameGlobals::PipboyRadio(); pipboyRadio && pipboyRadio->radioRef)
		REFR_RES = pipboyRadio->radioRef->refID;
	return true;
}

bool Cmd_GetPipboyRadio_Eval(COMMAND_ARGS_EVAL)
{
	if (RadioEntry *pipboyRadio = GameGlobals::PipboyRadio(); pipboyRadio && (pipboyRadio->radioRef == thisObj))
		*result = 1;
	return true;
}

bool Cmd_GetPCUsingScope_Eval(COMMAND_ARGS_EVAL)
{
	if (g_HUDMainMenu->isUsingScope)
		*result = 1;
	return true;
}

bool Cmd_GetPCUsingScope_Execute(COMMAND_ARGS)
{
	if (g_HUDMainMenu->isUsingScope)
		*result = 1;
	return true;
}

bool Cmd_GetIdleLoopTimes_Execute(COMMAND_ARGS)
{
	TESIdleForm *idle;
	UInt32 maxTimes;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idle, &maxTimes) && IS_ID(idle, TESIdleForm))
		*result = maxTimes ? idle->data.loopMax : idle->data.loopMin;
	return true;
}

bool Cmd_SetIdleLoopTimes_Execute(COMMAND_ARGS)
{
	TESIdleForm *idle;
	UInt32 loopMin, loopMax;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &idle, &loopMin, &loopMax) && IS_ID(idle, TESIdleForm))
	{
		if (loopMin > 255) loopMin = 255;
		if (loopMax < loopMin) loopMax = loopMin;
		idle->data.loopMin = loopMin;
		idle->data.loopMax = loopMax;
	}
	return true;
}

bool Cmd_GetPCUsingIronSights_Eval(COMMAND_ARGS_EVAL)
{
	if (g_thePlayer->ironSightNode && g_thePlayer->baseProcess->IsWeaponOut())
		*result = 1;
	return true;
}

bool Cmd_GetPCUsingIronSights_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->ironSightNode && g_thePlayer->baseProcess->IsWeaponOut())
		*result = 1;
	return true;
}

bool Cmd_GetPCLastExteriorDoor_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->lastExteriorDoor)
		REFR_RES = g_thePlayer->lastExteriorDoor->refID;
	return true;
}

bool Cmd_SwapTextureEx_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	char blockName[0x40], path[0x80];
	UInt32 texIdx = 0;
	char *pathBgn = CPY_RET_END(path, "Textures\\", 9);
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) && IS_ID(script, Script))
		if (addEvnt)
		{
			if (s_fastTravelEventScripts->Insert(script))
				HOOK_INC(PCFastTravel);
		}
		else if (s_fastTravelEventScripts->Erase(script))
			HOOK_DEC(PCFastTravel);
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

const char *kMoonTexturesDefault[] =
{
	"textures\\sky\\masser_full.dds",
	"textures\\sky\\masser_three_wan.dds",
	"textures\\sky\\masser_half_wan.dds",
	"textures\\sky\\masser_one_wan.dds",
	nullptr,
	"textures\\sky\\masser_one_wax.dds",
	"textures\\sky\\masser_half_wax.dds",
	"textures\\sky\\masser_three_wax.dds"
};
const char *s_moonTexturesOverride[8] = {nullptr};
const char *s_moonTexturesSet[8] = {nullptr};

__declspec(naked) void InitMoonHook()
{
	__asm
	{
		push	esi
		mov		esi, [ebp-0x18]
		mov		eax, offset s_moonTexturesSet
		push	dword ptr [eax+0x1C]
		push	dword ptr [eax+0x18]
		push	dword ptr [eax+0x14]
		push	dword ptr [eax+0x10]
		push	dword ptr [eax+0xC]
		push	dword ptr [eax+8]
		push	dword ptr [eax+4]
		push	dword ptr [eax]
		lea		ecx, [esi+0x18]
		call	String::Set
		lea		ecx, [esi+0x20]
		call	String::Set
		lea		ecx, [esi+0x28]
		call	String::Set
		lea		ecx, [esi+0x30]
		call	String::Set
		lea		ecx, [esi+0x38]
		call	String::Set
		lea		ecx, [esi+0x40]
		call	String::Set
		lea		ecx, [esi+0x48]
		call	String::Set
		lea		ecx, [esi+0x50]
		call	String::Set
		pop		esi
		JMP_EAX(0x634BF4)
	}
}

bool Cmd_SetMoonTexture_Execute(COMMAND_ARGS)
{
	UInt32 textureID;
	char path[0x80];
	Sky *currSky = Sky::Get();
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &textureID, &path) || (textureID > 7) || !currSky) return true;
	const char *newTexture = CopyString(path);
	for (UInt32 idx = 0; idx < 8; idx++)
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
	UInt8 original[5];
	*(UInt32*)original = *(UInt32*)0x634B30;
	original[4] = *(UInt8*)0x634B34;
	WriteRelJump(0x634B30, (UInt32)InitMoonHook);
	currSky->RefreshMoon();
	SafeWriteBuf(0x634B30, original, 5);
	*result = 1;
	return true;
}

bool Cmd_GetAlwaysRun_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->alwaysRun)
		*result = 1;
	return true;
}

bool Cmd_GetAlwaysRun_Eval(COMMAND_ARGS_EVAL)
{
	if (g_thePlayer->alwaysRun)
		*result = 1;
	return true;
}

bool Cmd_GetAutoMove_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->autoMove)
		*result = 1;
	return true;
}

bool Cmd_GetAutoMove_Eval(COMMAND_ARGS_EVAL)
{
	if (g_thePlayer->autoMove)
		*result = 1;
	return true;
}

bool Cmd_SetOnPCTargetChangeEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) && IS_ID(script, Script))
		if (addEvnt)
		{
			if (s_targetChangeEventScripts->Insert(script))
				HOOK_INC(SetPCTarget);
		}
		else if (s_targetChangeEventScripts->Erase(script))
			HOOK_DEC(SetPCTarget);
	return true;
}

bool Cmd_FreePlayer_Execute(COMMAND_ARGS)
{
	Game_HeapFree(g_thePlayer);
	return true;
}

bool Cmd_GetLocalGravity_Execute(COMMAND_ARGS)
{
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
		if (hkpWorld *world = GethkpWorld())
			*result = world->gravity[axis - 'X'];
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetLocalGravityVector_Execute(COMMAND_ARGS)
{
	if (hkpWorld *world = GethkpWorld())
		ExtractArgsEx(EXTRACT_ARGS_EX, &world->gravity.x, &world->gravity.y, &world->gravity.z);
	return true;
}

bool Cmd_GetReticlePos_Execute(COMMAND_ARGS)
{
	SInt32 layerType = -1;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS_EX;
	if (g_thePlayer->parentCell && (!numArgs || ExtractArgsEx(EXTRACT_ARGS_EX, &layerType, &maxRange)))
		if (NiVector4 coords; coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, layerType) || (numArgs >= 2))
		{
			ArrayElementL elements[3] = {coords.x, coords.y, coords.z};
			*result = (int)CreateArray(elements, 3, scriptObj);
		}
	return true;
}

bool Cmd_GetReticleRange_Execute(COMMAND_ARGS)
{
	*result = -1;
	SInt32 layerType = -1;
	float maxRange = 50000.0F;
	UInt32 fromCam = 0;
	UInt8 numArgs = NUM_ARGS_EX;
	if (g_thePlayer->parentCell && (!numArgs || ExtractArgsEx(EXTRACT_ARGS_EX, &layerType, &maxRange, &fromCam)))
		if (NiVector4 coords; coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, layerType) || (numArgs >= 2))
			*result = Point3Distance(!fromCam ? g_thePlayer->position : g_mainCamera->WorldTranslate(), coords);
	return true;
}

bool Cmd_SetOnDialogTopicEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &form) && IS_ID(script, Script) && (IS_ID(form, TESTopic) || IS_ID(form, TESTopicInfo)))
		if (addEvnt)
		{
			EventCallbackScripts *callbacks;
			if (s_dialogTopicEventMap->Insert(form, &callbacks))
				HOOK_INC(RunResultScript);
			callbacks->Insert(script);
		}
		else if (auto findTopic = s_dialogTopicEventMap->Find(form); findTopic && findTopic().Erase(script) && findTopic().Empty())
		{
			findTopic.Remove();
			HOOK_DEC(RunResultScript);
		}
	return true;
}

bool Cmd_GetGameDaysPassed_Execute(COMMAND_ARGS)
{
	int bgnYear = 2281, bgnMonth = 10, bgnDay = 13;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bgnYear, &bgnMonth, &bgnDay) && (bgnMonth <= 12))
		*result = GetDaysPassed(bgnYear, bgnMonth - 1, bgnDay);
	DoConsolePrint(result);
	return true;
}

bool Cmd_IsPCInCombat_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->pcInCombat && !g_thePlayer->pcUnseen)
		*result = 1;
	return true;
}

bool Cmd_IsPCInCombat_Eval(COMMAND_ARGS_EVAL)
{
	if (g_thePlayer->pcInCombat && !g_thePlayer->pcUnseen)
		*result = 1;
	return true;
}

bool Cmd_ToggleHardcoreTracking_Execute(COMMAND_ARGS)
{
	UInt32 toggleON;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &toggleON))
		if (toggleON) s_serializedVars.serializedFlags &= ~kSerializedFlag_NoHardcoreTracking;
		else s_serializedVars.serializedFlags |= kSerializedFlag_NoHardcoreTracking;
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
				if (form = iter->data; form && IS_ID(form, TESObjectACTI))
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
	UInt32 interiors;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &interiors))
	{
		TempElements *tmpElements = GetTempElements();
		TESObjectCELL **cellsBuffer = interiors ? g_TES->interiorsBuffer : g_TES->exteriorsBuffer, *cell;
		if (cellsBuffer)
		{
			UInt32 maxVal = interiors ? INIS_UINT(uInterior_Cell_Buffer_General) : INIS_UINT(uExterior_Cell_Buffer_General);
			while (maxVal && (cell = *cellsBuffer))
			{
				tmpElements->Append(cell);
				cellsBuffer++;
				maxVal--;
			}
		}
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_SetOnLocationDiscoverEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) && IS_ID(script, Script))
		if (addEvnt)
		{
			if (s_locationDiscoverEventScripts->Insert(script))
				HOOK_INC(LocationDiscover);
		}
		else if (s_locationDiscoverEventScripts->Erase(script))
			HOOK_DEC(LocationDiscover);
	return true;
}

bool Cmd_SetOnCraftingEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) && IS_ID(script, Script))
		if (addEvnt)
		{
			if (s_itemCraftedEventScripts->Insert(script))
				HOOK_INC(ItemCrafted);
		}
		else if (s_itemCraftedEventScripts->Erase(script))
			HOOK_DEC(ItemCrafted);
	return true;
}

bool Cmd_IsInKillCam_Execute(COMMAND_ARGS)
{
	if ((VATSCameraData::Get()->mode == 4) && (g_thePlayer->killCamTimer > 0))
		*result = 1;
	return true;
}

bool Cmd_IsInKillCam_Eval(COMMAND_ARGS_EVAL)
{
	if ((VATSCameraData::Get()->mode == 4) && (g_thePlayer->killCamTimer > 0))
		*result = 1;
	return true;
}

bool Cmd_SwapObjectLOD_Execute(COMMAND_ARGS)
{
	TESWorldSpace *worldSpc;
	SInt32 cellX, cellY;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &worldSpc, &cellX, &cellY) && IS_ID(worldSpc, TESWorldSpace) && s_swapObjLODMap()[worldSpc].Insert(Coordinate(cellX, cellY)))
		HOOK_SET(MakeObjLODPath, true);
	return true;
}

bool Cmd_SetWobblesRotation_Execute(COMMAND_ARGS)
{
	static BSFadeNode **wobbleAnimations = nullptr;
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
			NiQuaternion quaternion(rotPRY);
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
					MemCopy(wobbleAnimRotations[startIdx], rotationKeys, sizeof(QuaternionKey) * 3);
		}
	}
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
		s_forceHCNeedsUpdate = g_thePlayer->isHardcore && !(s_serializedVars.serializedFlags & kSerializedFlag_NoHardcoreTracking);
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
		if (s_actorValueIDsMap->Empty())
			for (UInt32 avCode = 0; avCode < 77; avCode++)
			{
				ActorValueInfo *avInfo = ActorValueInfo::Array()[avCode];
				s_actorValueIDsMap()[avInfo->infoName] = avCode;
			}
		if (UInt32 *idPtr = s_actorValueIDsMap->GetPtr(avStr))
			*result = (int)*idPtr;
	}
	return true;
}

bool Cmd_GetHardcoreTracking_Execute(COMMAND_ARGS)
{
	if (s_serializedVars.serializedFlags & kSerializedFlag_NoHardcoreTracking)
		*result = 1;
	return true;
}

bool Cmd_GetHardcoreTracking_Eval(COMMAND_ARGS_EVAL)
{
	if (s_serializedVars.serializedFlags & kSerializedFlag_NoHardcoreTracking)
		*result = 1;
	return true;
}

bool Cmd_GetNoteRead_Execute(COMMAND_ARGS)
{
	BGSNote *note;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &note) && note->read)
		*result = 1;
	return true;
}

bool Cmd_GetNoteRead_Eval(COMMAND_ARGS_EVAL)
{
	if (((BGSNote*)arg1)->read)
		*result = 1;
	return true;
}

bool Cmd_SetOnNoteAddedEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt) && IS_ID(script, Script))
		if (addEvnt)
		{
			if (s_noteAddedEventScripts->Insert(script))
				HOOK_INC(AddNote);
		}
		else if (s_noteAddedEventScripts->Erase(script))
			HOOK_DEC(AddNote);
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modeID) && (modeID <= 4) && *(UInt8*)kDebugModeBytes[modeID])
		*result = 1;
	return true;
}

bool Cmd_FreezeTime_Execute(COMMAND_ARGS)
{
	if (g_OSGlobals->freezeTime)
		*result = 1;
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
	if (cameraCollision)
		*result = 1;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (!toggle == cameraCollision))
	{
		cameraCollision = !cameraCollision;
		SafeWrite8(0x94A34E, toggle ? 0x74 : 0xEB);
	}
	return true;
}

__declspec(naked) BGSProjectile* __fastcall GetWeaponProjectileHook(TESObjectWEAP *weapon, int, Actor *actor)
{
	__asm
	{
		push	ecx
		push	dword ptr [esp+8]
		call	TESObjectWEAP::GetEquippedAmmo
		pop		ecx
		test    eax, eax
		jz		weapProj
		cmp		byte ptr [eax+4], kFormType_TESAmmo
		jnz		weapProj
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
		xor		edx, edx
		cmp		ecx, g_rockItLauncher
		cmovz	eax, edx
		JMP_EDX(0x946326)
	}
}

__declspec(naked) void PCAmmoSwitchHook()
{
	__asm
	{
		mov		ecx, ds:0x11D98D4
		cmp		ecx, g_rockItLauncher
		JMP_EAX(0x9409B2)
	}
}

bool Cmd_InitRockItLauncher_Execute(COMMAND_ARGS)
{
	if (!g_rockItLauncher && ExtractArgsEx(EXTRACT_ARGS_EX, &g_rockItLauncher))
	{
		__asm
		{
			push	TESObjectWEAP::GetEquippedAmmo
			push	0x525980
			call	WriteRelJump
		}
		WriteRelJump(0x525A90, (UInt32)GetWeaponProjectileHook);
		WriteRelJump(0x946310, (UInt32)RILAmmoFixHook);
		if (*(UInt8*)0x781EE6 != 0xEB)
			WriteRelJump(0x940998, (UInt32)PCAmmoSwitchHook);
	}
	return true;
}

bool Cmd_ToggleHitEffects_Execute(COMMAND_ARGS)
{
	if (!s_disableHitEffects)
		*result = 1;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle))
		if (toggle) s_disableHitEffects &= 2;
		else s_disableHitEffects |= 1;
	return true;
}

bool Cmd_ToggleNoMovementCombat_Execute(COMMAND_ARGS)
{
	if (s_noMovementCombat)
		*result = 1;
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
		g_thePlayer->ModActorValueInt(kAVCode_XP, xpAmount, 0);
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
	while (count)
	{
		if (Actor *actor = (Actor*)objArray[--count]; actor && IS_ACTOR(actor) && (actor->lifeState == 2) && !(actor->flags & 0x200000) && !(actor->baseForm->flags & 0x400))
			if (HighProcess *hiProcess = (HighProcess*)actor->baseProcess; hiProcess && !hiProcess->processLevel && !hiProcess->fadeType && (hiProcess->dyingTimer < 0) &&
				!actor->extraDataList.HasType(kXData_ExtraEnableStateChildren) && !ThisCall<bool>(0x577DE0, actor))
				ThisCall(0x8FEB60, hiProcess, actor);
	}
	return true;
}

bool Cmd_GetCameraMovement_Execute(COMMAND_ARGS)
{
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
			if (iMovY && INIS_BOOL(bInvertYValues_Controls))
				iMovY = -iMovY;
			double mouseSensitivity = INIS_FLT(fMouseSensitivity_Controls);
			double fMovX = iMovX * mouseSensitivity;
			double fMovY = iMovY * mouseSensitivity;
			if (controller)
			{
				fMovX *= 1 / 1500.0;
				fMovY *= -1 / 1500.0;
			}
			outX->data = fMovX;
			outY->data = fMovY;
			*result = 1;
		}
		else
		{
			outX->data = 0.0;
			outY->data = 0.0;
		}
	}
	return true;
}

bool Cmd_GetReticleNode_Execute(COMMAND_ARGS)
{
	const char *nodeName = "";
	float maxRange = 50000.0F;
	SInt32 layerType = 6;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &maxRange, &layerType))
		if (NiAVObject *rtclObject = GetRayCastObject(g_thePlayer->cameraPos, g_mainCamera->WorldRotate(), maxRange, layerType))
			nodeName = rtclObject->GetName();
	AssignString(PASS_COMMAND_ARGS, nodeName);
	return true;
}

bool Cmd_SetInternalMarker_Execute(COMMAND_ARGS)
{
	TESForm *form;
	int toggle = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &toggle))
	{
		if IS_REFERENCE(form)
			form = ((TESObjectREFR*)form)->baseForm;
		if (toggle < 0)
		{
			if (s_internalMarkerIDs->HasKey(form->refID))
				*result = 1;
		}
		else if (toggle)
			s_internalMarkerIDs->Insert(form->refID);
		else s_internalMarkerIDs->Erase(form->refID);
	}
	return true;
}

bool Cmd_GetPointRayCastPos_Execute(COMMAND_ARGS)
{
	NiVector3 pos;
	NiVector4 rot(_mm_setzero_ps());
	ResultVars outPos;
	SInt32 layerType = 6;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &pos.x, &pos.y, &pos.z, &rot.x, &rot.z, &outPos.x, &outPos.y, &outPos.z, &layerType, &maxRange))
		if (NiMatrix33 rotMat; rotMat.FromEulerPRYInv(rot * GET_PS(8)) && (rot.RayCastCoords(pos, rotMat + 1, maxRange, layerType) || (numArgs >= 10)))
		{
			outPos.Set(rot.PS());
			*result = 1;
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
	char modelPath[0x80];
	NiVector3 pos;
	NiVector4 rot;
	float scale = 1.0F;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modelPath, &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &scale))
		if (TESObjectCELL *parentCell = g_thePlayer->parentCell; parentCell && (!parentCell->worldSpace || (parentCell = g_gridCellArray->GetCellAtPos(g_thePlayer->position.PS()))))
			if (NiNode *objNode = LoadModelCopy(modelPath))
			{
				objNode->RemoveCollision();
				parentCell->Get3DNode(3)->AddObject(objNode, 1);
				objNode->LocalRotate().FromEulerYPR(rot * GET_PS(8));
				objNode->LocalTranslate() = pos;
				objNode->m_transformLocal.scale = scale;
				objNode->UpdateDownwardPass(kNiUpdateData, 0);
				if (objNode->m_flags & 0x20000000)
					AddPointLights(objNode);
				*result = 1;
			}
	return true;
}

bool Cmd_SetArmorConditionPenalty_Execute(COMMAND_ARGS)
{
	*result = s_condDRDTPenalty;
	if (NUM_ARGS)
		ExtractArgsEx(EXTRACT_ARGS_EX, &s_condDRDTPenalty);
	return true;
}

bool Cmd_GetReticlePosAlt_Execute(COMMAND_ARGS)
{
	ResultVars outPos;
	SInt32 layerType = 6;
	float maxRange = 50000.0F;
	UInt8 numArgs = NUM_ARGS;
	if (g_thePlayer->parentCell && ExtractArgsEx(EXTRACT_ARGS_EX, &outPos.x, &outPos.y, &outPos.z, &layerType, &maxRange))
		if (NiVector4 coords; coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, layerType) || (numArgs >= 5))
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
	return true;
}

bool Cmd_TransformWorldToLocal_Execute(COMMAND_ARGS)
{
	NiVector3 origin, target, rotation;
	ResultVars outAngles;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &origin.x, &origin.y, &origin.z, &target.x, &target.y, &target.z, &rotation.x, &rotation.y, &rotation.z, &outAngles.x, &outAngles.z))
	{
		outAngles.y = outAngles.z;
		__m128 angles = TransformWorldToLocal(origin, target, rotation.PS() * GET_PS(8));
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
		__m128 angles = GetAnglesBetweenPoints(point1.PS(), point2.PS());
		outAngles.Set(angles, GET_PS(9));
	}
	return true;
}

bool Cmd_GetP2PRayCastRange_Execute(COMMAND_ARGS)
{
	NiVector3 point1, point2;
	SInt32 layerType = 6;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &point1.x, &point1.y, &point1.z, &point2.x, &point2.y, &point2.z, &layerType))
	{
		NiMatrix33 rotMat;
		float length = rotMat.From2Points(point1.PS(), point2.PS());
		NiVector4 rcPos;
		if (rcPos.RayCastCoords(point1, rotMat + 1, length, layerType))
			*result = Point3Distance(rcPos, point2);
	}
	return true;
}

bool Cmd_SetDamageToArmorMaxPercent_Execute(COMMAND_ARGS)
{
	*result = s_serializedVars.dmgArmorMaxPercent;
	if (NUM_ARGS)
	{
		float value;
		ExtractArgsEx(EXTRACT_ARGS_EX, &value);
		if ((value >= 0) && (value <= 1.0F))
			s_serializedVars.dmgArmorMaxPercent = value;
	}
	return true;
}

TempObject<UnorderedMap<UINT, Set<TESForm*>>> s_keywordFormsMap;

void AssignKeywordRecourse(Set<TESForm*> *formsSet, tList<TESForm> &list)
{
	auto iter = list.Head();
	do
	{
		if (TESForm *form = iter->data)
			if NOT_ID(form, BGSListForm)
				formsSet->Insert(form->GetBaseIfRef());
			else AssignKeywordRecourse(formsSet, ((BGSListForm*)form)->list);
	}
	while (iter = iter->next);
}

bool Cmd_AssignKeyword_Execute(COMMAND_ARGS)
{
	TESForm *form;
	char keyword[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &keyword))
	{
		auto formsSet = &s_keywordFormsMap()[StrHashCI(keyword)];
		if NOT_ID(form, BGSListForm)
			formsSet->Insert(form->GetBaseIfRef());
		else AssignKeywordRecourse(formsSet, ((BGSListForm*)form)->list);
	}
	return true;
}

bool Cmd_HasKeyword_Execute(COMMAND_ARGS)
{
	TESForm *form;
	char keyword[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &keyword))
		if (auto formsSet = s_keywordFormsMap->GetPtr(StrHashCI(keyword)); formsSet && formsSet->HasKey(form->GetBaseIfRef()))
			*result = 1;
	return true;
}

bool Cmd_GetKeywordForms_Execute(COMMAND_ARGS)
{
	NVSEArrayVar *outArray = CreateArray(nullptr, 0, scriptObj);
	*result = (int)outArray;
	char keyword[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &keyword))
		if (auto formsSet = s_keywordFormsMap->GetPtr(StrHashCI(keyword)))
			for (auto frmIter = formsSet->Begin(); frmIter; ++frmIter)
				AppendElement(outArray, ArrayElementL(*frmIter));
	return true;
}

bool Cmd_HasKeywordCond_Eval(COMMAND_ARGS_EVAL)
{
	if (auto formsSet = s_keywordFormsMap->GetPtr((UINT)arg1); formsSet && formsSet->HasKey(thisObj->GetBaseForm()))
		*result = 1;
	return true;
}

bool Cmd_ToggleDepthClear_Execute(COMMAND_ARGS)
{
	ExtractArgsEx(EXTRACT_ARGS_EX, &s_clearDepthBuffer);
	return true;
}

bool Cmd_GetReticleTargetLimb_Execute(COMMAND_ARGS)
{
	*result = -1;
	float maxRange = 50000.0F;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &maxRange))
		if (NiAVObject *rtclObject = GetRayCastObject(g_thePlayer->cameraPos, g_mainCamera->WorldRotate(), maxRange, 54))
			if (TESObjectREFR *resRefr = rtclObject->GetParentRef(); resRefr && IS_ACTOR(resRefr))
				// Credits to lStewieAl for pointing out this address!
				*result = ThisCall<int>(0x8B3EF0, (Actor*)resRefr, rtclObject);	// Actor::GetHitLocationForNode
	return true;
}