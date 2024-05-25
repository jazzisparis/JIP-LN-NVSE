#pragma once

DEFINE_COMMAND_PLUGIN(GetNVSEVersionFull, 0, nullptr);

bool Cmd_GetNVSEVersionFull_Execute(COMMAND_ARGS)
{
	*result = s_nvseVersion;
	return true;
}

bool Hook_MenuMode_Execute(COMMAND_ARGS)
{
	UInt32 menuID = 0;
	ExtractArgsEx(EXTRACT_ARGS_EX, &menuID);
	bool menuMode = IsMenuMode(menuID);
	if (menuMode)
		*result = 1;
	if (IsConsoleOpen())
		Console_Print("MenuMode %d >> %d", menuID, menuMode);
	return true;
}

bool Hook_MenuMode_Eval(TESObjectREFR *thisObj, UInt32 menuID, UInt32 arg2, double *result)
{
	if (IsMenuMode(menuID))
		*result = 1;
	return true;
}

bool Hook_GetItemCount_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		*result = thisObj->GetItemCount(form);
	DoConsolePrint(result);
	return true;
}

bool Hook_GetItemCount_Eval(TESObjectREFR *thisObj, TESForm *form, UInt32 arg2, double *result)
{
	*result = thisObj->GetItemCount(form);
	return true;
}

bool Hook_GetContainer_Execute(COMMAND_ARGS)
{
	if (containingObj)
		REFR_RES = containingObj->refID;
	else if (InventoryRef *invRef = InventoryRefGetForID(thisObj->refID); invRef && invRef->containerRef)
		REFR_RES = invRef->containerRef->refID;
	return true;
}

bool Hook_IsInList_Execute(COMMAND_ARGS)
{
	BGSListForm *formList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &formList) && formList->list.IsInList(thisObj->GetBaseForm()))
		*result = 1;
	DoConsolePrint(result);
	return true;
}

bool Hook_IsInList_Eval(TESObjectREFR *thisObj, BGSListForm *formList, void *unused, double *result)
{
	*result = formList->list.IsInList(thisObj->GetBaseForm());
	return true;
}

bool Hook_GetHitLocation_Execute(COMMAND_ARGS)
{
	SInt32 hitLoc = -1;
	if (IS_ACTOR(thisObj) && ((Actor*)thisObj)->baseProcess)
		if (ActorHitData *hitData = ((Actor*)thisObj)->baseProcess->GetHitData())
			hitLoc = hitData->unk60;
	*result = hitLoc;
	return true;
}

__declspec(noinline) UInt8 __fastcall DoGetPerkRank(Actor *actor, BGSPerk *perk, bool forTeammates)
{
	if IS_ACTOR(actor)
		if (s_patchInstallState.NPCPerks)
			return actor->GetPerkRank(perk, forTeammates);
		else if (actor->IsPlayer() || actor->isTeammate)
			return g_thePlayer->GetPerkRank(perk, forTeammates | actor->isTeammate);
	return 0;
}

bool Hook_HasPerk_Execute(COMMAND_ARGS)		// Modifies HasPerk to allow detection of follower perks on followers.
{
	BGSPerk *perk;
	UInt32 useAlt = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &useAlt))
	{
		UInt8 rank = DoGetPerkRank((Actor*)thisObj, perk, useAlt);
		if (rank) *result = 1;
		if (IsConsoleOpen())
			Console_Print((const char*)0x1035F1C, rank);
	}
	return true;
}

bool Hook_HasPerk_Eval(Actor *thisObj, BGSPerk *perk, UInt32 useAlt, double *result)
{
	if (DoGetPerkRank(thisObj, perk, useAlt))
		*result = 1;
	return true;
}

bool Hook_IsKeyPressed_Execute(COMMAND_ARGS)
{
	UInt32 keyID, flags = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &keyID, &flags))
		*result = g_DIHookCtrl->IsKeyPressed(keyID, flags);
	return true;
}

bool Hook_IsKeyPressed_Eval(TESObjectREFR *thisObj, UInt32 keyID, UInt32 flags, double *result)
{
	if (g_DIHookCtrl->IsKeyPressed(keyID, flags))
		*result = 1;
	return true;
}

__declspec(noinline) bool __fastcall IsControlPressed(UInt32 ctrlID, UInt32 flags = 1)
{
	if (!s_controllerReady)
	{
		UInt32 keyID = KEYBOARD_BIND(ctrlID);
		if ((keyID != 0xFF) && g_DIHookCtrl->IsKeyPressed(keyID, flags))
			return true;
		else
		{
			keyID = MOUSE_BIND(ctrlID);
			return (keyID != 0xFF) && g_DIHookCtrl->IsKeyPressed(keyID + 0x100, flags);
		}
	}
	return GetXIControlPressed((flags & 2) ? &s_gamePad : (XINPUT_GAMEPAD_EX*)0x11F35A8, ctrlID);
}

bool __fastcall IsControlPressedRaw(UInt32 ctrlID)
{
	if (!s_controllerReady)
	{
		UInt32 keyID = KEYBOARD_BIND(ctrlID);
		if ((keyID != 0xFF) && g_DIHookCtrl->IsKeyPressedRaw(keyID))
			return true;
		else
		{
			keyID = MOUSE_BIND(ctrlID);
			return (keyID != 0xFF) && g_DIHookCtrl->IsKeyPressedRaw(keyID + 0x100);
		}
	}
	return GetXIControlPressed(&s_gamePad, ctrlID);
}

void __fastcall SetControlDisabled(UInt32 ctrlID, bool bDisable)
{
	if (!s_controllerReady)
	{
		UInt32 keyID = KEYBOARD_BIND(ctrlID);
		if (keyID != 0xFF) g_DIHookCtrl->SetKeyDisableState(keyID, bDisable);
		keyID = MOUSE_BIND(ctrlID);
		if (keyID != 0xFF) g_DIHookCtrl->SetKeyDisableState(keyID + 0x100, bDisable);
	}
	else SetXIControlDisabled(ctrlID, bDisable);
}

bool Hook_DisableControl_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID, mask;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ctrlID, &mask) && (ctrlID < MAX_CONTROL_BIND))
		SetControlDisabled(ctrlID, true);
	return true;
}

bool Hook_EnableControl_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID, mask;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ctrlID, &mask) && (ctrlID < MAX_CONTROL_BIND))
		SetControlDisabled(ctrlID, false);
	return true;
}

bool Hook_TapControl_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ctrlID) && (ctrlID < MAX_CONTROL_BIND))
		if (!s_controllerReady)
		{
			UInt32 keyID = KEYBOARD_BIND(ctrlID);
			if (keyID != 0xFF)
			{
				g_DIHookCtrl->TapKey(keyID);
				*result = 1;
			}
			else
			{
				keyID = MOUSE_BIND(ctrlID);
				if (keyID != 0xFF)
				{
					g_DIHookCtrl->TapKey(keyID + 0x100);
					*result = 1;
				}
			}
		}
		else if (TapXIControl(ctrlID))
			*result = 1;
	return true;
}

bool Hook_IsControlDisabled_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ctrlID) && (ctrlID < MAX_CONTROL_BIND))
		if (!s_controllerReady)
		{
			UInt32 keyID = KEYBOARD_BIND(ctrlID), btnID = MOUSE_BIND(ctrlID);
			if (keyID != 0xFF)
			{
				if (g_DIHookCtrl->IsKeyDisabled(keyID) && ((btnID == 0xFF) || g_DIHookCtrl->IsKeyDisabled(btnID + 0x100)))
					*result = 1;
			}
			else if ((btnID != 0xFF) && g_DIHookCtrl->IsKeyDisabled(btnID + 0x100))
				*result = 1;
		}
		else if (GetXIControlDisabled(ctrlID))
			*result = 1;
	return true;
}

bool Hook_IsControlPressed_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID, flags = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ctrlID, &flags) && (ctrlID < MAX_CONTROL_BIND) && IsControlPressed(ctrlID, flags))
		*result = 1;
	return true;
}

bool Hook_IsControlPressed_Eval(TESObjectREFR *thisObj, UInt32 ctrlID, UInt32 flags, double *result)
{
	if ((ctrlID < MAX_CONTROL_BIND) && IsControlPressed(ctrlID, flags))
		*result = 1;
	return true;
}

TempObject<UnorderedMap<const char*, Setting*, 0x1000, false>> s_gameSettingsMap;

void InitSettingMaps()
{
	for (auto gstIter = (*(GameSettingCollection**)0x11C8048)->settingMap.Begin(); gstIter; ++gstIter)
		if (Setting *setting = gstIter.Get(); setting && setting->name)
			s_gameSettingsMap()[setting->name] = setting;

	for (UInt32 addr : {0x11F96A0, 0x11F35A0, 0x11CC694, 0x11F35A4})
	{
		auto istIter = (*(IniSettingCollection**)addr)->settings.Head();
		do
		{
			if (Setting *setting = istIter->data; setting && setting->ValidType())
				s_gameSettingsMap()[setting->name] = setting;
		}
		while (istIter = istIter->next);
	}
}

bool Hook_GetNumericGameSetting_Execute(COMMAND_ARGS)
{
	*result = -1;
	char settingName[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &settingName) && ((settingName[0] | 0x20) != 's'))
		if (Setting *setting = s_gameSettingsMap->Get(settingName))
		{
			setting->Get(result);
			DoConsolePrint(result);
		}
		else if (IsConsoleOpen())
			Console_Print("SETTING NOT FOUND");
	return true;
}

bool Hook_SetNumericGameSetting_Execute(COMMAND_ARGS)
{
	char settingName[0x80];
	double newVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &settingName, &newVal) && ((settingName[0] | 0x20) != 's'))
		if (Setting *setting = s_gameSettingsMap->Get(settingName))
		{
			setting->Set(newVal);
			*result = 1;
		}
		else if (IsConsoleOpen())
			Console_Print("SETTING NOT FOUND");
	return true;
}

bool Hook_GetNumericINISetting_Execute(COMMAND_ARGS)
{
	*result = -1;
	char settingName[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &settingName) && ((settingName[0] | 0x20) != 's'))
		if (Setting *setting = s_gameSettingsMap->Get(settingName))
		{
			setting->Get(result);
			DoConsolePrint(result);
		}
		else if (IsConsoleOpen())
			Console_Print("SETTING NOT FOUND");
	return true;
}

bool Hook_SetNumericINISetting_Execute(COMMAND_ARGS)
{
	char settingName[0x80];
	double newVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &settingName, &newVal) && ((settingName[0] | 0x20) != 's'))
		if (Setting *setting = s_gameSettingsMap->Get(settingName))
		{
			setting->Set(newVal);
			*result = 1;
		}
		else if (IsConsoleOpen())
			Console_Print("SETTING NOT FOUND");
	return true;
}

__declspec(noinline) SInt32 __fastcall IsRefInList(BGSListForm *listForm, TESForm *form)
{
	SInt32 index = listForm->list.GetIndexOf(form);
	if ((index < 0) && IS_REFERENCE(form))
		index = listForm->list.GetIndexOf(((TESObjectREFR*)form)->GetBaseForm());
	return index;
}

bool Hook_IsRefInList_Execute(COMMAND_ARGS)
{
	BGSListForm *listForm;
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &listForm, &form))
		*result = IsRefInList(listForm, form);
	else *result = -1;
	DoConsolePrint(result);
	return true;
}

bool Hook_IsRefInList_Eval(TESObjectREFR *thisObj, BGSListForm *listForm, TESForm *form, double *result)
{
	*result = IsRefInList(listForm, form);
	return true;
}

bool Hook_Update3D_Execute(COMMAND_ARGS)
{
	if (thisObj->IsPlayer())
		ThisCall(0x8D3FA0, thisObj);
	else
		thisObj->Update3D();
	return true;
}

//	Added support for models set per-refr, via SetRefrModelPath
bool Hook_GetModelPath_Execute(COMMAND_ARGS)
{
	TESForm *pForm = nullptr;
	const char *pathStr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &pForm) && (pForm || thisObj))
	{
		TESObjectREFR *pRefr;
		if (!pForm)
		{
			pRefr = thisObj;
			pForm = pRefr->baseForm;
		}
		else
		{
			pRefr = IS_REFERENCE(pForm) ? (TESObjectREFR*)pForm : nullptr;
			if (pRefr) pForm = pRefr->baseForm;
		}
		if (pForm && (!pRefr || !(pRefr->JIPRefFlags() & kHookRefFlag5F_RefrModelPath) || !(pathStr = s_refrModelPathMap->Get(pRefr))))
			pathStr = pForm->GetModelPath();
	}
	AssignString(PASS_COMMAND_ARGS, pathStr);
	return true;
}

bool __fastcall IsJIPAlias(const char *pluginName)
{
	return !StrCompareCI(pluginName, "JIP NVSE Plugin") || !StrCompareCI(pluginName, "lutana_nvse") || StrBeginsCI(pluginName, "JIP LN NVSE");
}

bool Hook_IsPluginInstalled_Execute(COMMAND_ARGS)
{
	char pluginName[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &pluginName) && (GetPluginInfoByName(pluginName) || IsJIPAlias(pluginName)))
		*result = 1;
	DoConsolePrint(result);
	return true;
}

bool Hook_GetPluginVersion_Execute(COMMAND_ARGS)
{
	*result = -1;
	char pluginName[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &pluginName))
		if (IsJIPAlias(pluginName))
			*result = JIP_LN_VERSION;
		else if (auto pluginInfo = GetPluginInfoByName(pluginName))
			*result = (int)pluginInfo->version;
	DoConsolePrint(result);
	return true;
}

void InitCmdPatches()
{
	CommandInfo *cmdInfo = GetCmdByOpcode(0x1024);
	cmdInfo->execute = Hook_MenuMode_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_MenuMode_Eval;
	cmdInfo = GetCmdByOpcode(0x102F);
	cmdInfo->execute = Hook_GetItemCount_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_GetItemCount_Eval;
	cmdInfo = GetCmdByOpcode(0x10CF);
	cmdInfo->execute = Hook_GetContainer_Execute;
	cmdInfo = GetCmdByOpcode(0x1174);
	cmdInfo->execute = Hook_IsInList_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsInList_Eval;
	cmdInfo = GetCmdByOpcode(0x1187);
	cmdInfo->execute = Hook_GetHitLocation_Execute;
	cmdInfo = GetCmdByOpcode(0x11C1);
	cmdInfo->execute = Hook_HasPerk_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_HasPerk_Eval;
	cmdInfo = GetCmdByOpcode(0x127D);
	cmdInfo->execute = Cmd_GetNVSEVersionFull_Execute;
	cmdInfo = GetCmdByOpcode(0x1453);
	cmdInfo->execute = Hook_IsKeyPressed_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsKeyPressed_Eval;
	cmdInfo = GetCmdByOpcode(0x1462);
	cmdInfo->execute = Hook_DisableControl_Execute;
	cmdInfo = GetCmdByOpcode(0x1463);
	cmdInfo->execute = Hook_EnableControl_Execute;
	cmdInfo = GetCmdByOpcode(0x1464);
	cmdInfo->execute = Hook_TapControl_Execute;
	cmdInfo = GetCmdByOpcode(0x146A);
	cmdInfo->execute = Hook_IsControlDisabled_Execute;
	cmdInfo = GetCmdByOpcode(0x146B);
	cmdInfo->execute = Hook_IsControlPressed_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsControlPressed_Eval;
	cmdInfo = GetCmdByOpcode(0x149A);
	cmdInfo->execute = Hook_GetNumericGameSetting_Execute;
	cmdInfo = GetCmdByOpcode(0x149B);
	cmdInfo->execute = Hook_SetNumericGameSetting_Execute;
	cmdInfo->params = kParams_OneString_OneDouble;
	cmdInfo = GetCmdByOpcode(0x149C);
	cmdInfo->execute = Hook_GetNumericINISetting_Execute;
	cmdInfo = GetCmdByOpcode(0x149D);
	cmdInfo->execute = Hook_SetNumericINISetting_Execute;
	cmdInfo->params = kParams_OneString_OneDouble;
	cmdInfo = GetCmdByOpcode(0x1511);
	cmdInfo->execute = Hook_IsRefInList_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsRefInList_Eval;
	cmdInfo = GetCmdByOpcode(0x152D);
	cmdInfo->execute = Hook_Update3D_Execute;
	cmdInfo = GetCmdByOpcode(0x158E);
	cmdInfo->execute = Hook_GetModelPath_Execute;
	cmdInfo = GetCmdByOpcode(0x15DF);
	cmdInfo->execute = Hook_IsPluginInstalled_Execute;
	cmdInfo = GetCmdByOpcode(0x15E0);
	cmdInfo->execute = Hook_GetPluginVersion_Execute;
}