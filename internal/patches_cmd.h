#pragma once

DEFINE_COMMAND_PLUGIN(GetNVSEVersionFull, , 0, 0, NULL);

bool Cmd_GetNVSEVersionFull_Execute(COMMAND_ARGS)
{
	*result = s_nvseVersion;
	return true;
}

bool Hook_MenuMode_Execute(COMMAND_ARGS)
{
	UInt32 menuID = 0;
	bool menuMode = false;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &menuID))
		menuMode = IsMenuMode(menuID);
	*result = menuMode;
	if (IsConsoleOpen()) Console_Print("MenuMode %d >> %d", menuID, menuMode);
	return true;
}

bool Hook_MenuMode_Eval(TESObjectREFR *thisObj, UInt32 menuID, UInt32 arg2, double *result)
{
	*result = IsMenuMode(menuID);
	return true;
}

bool Hook_GetItemCount_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		*result = thisObj->GetItemCount(form);
		DoConsolePrint(result);
	}
	else *result = 0;
	return true;
}

bool Hook_GetItemCount_Eval(TESObjectREFR *thisObj, TESForm *form, UInt32 arg2, double *result)
{
	*result = thisObj->GetItemCount(form);
	return true;
}

bool Hook_GetContainer_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (containingObj)
		REFR_RES = containingObj->refID;
	else
	{
		InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
		if (invRef && invRef->containerRef)
			REFR_RES = invRef->containerRef->refID;
	}
	return true;
}

bool Hook_IsInList_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSListForm *formList;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &formList))
	{
		if (formList->list.IsInList(thisObj->GetBaseForm()))
			*result = 1;
		DoConsolePrint(result);
	}
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
	if (thisObj->IsActor() && ((Actor*)thisObj)->baseProcess)
	{
		ActorHitData *hitData = ((Actor*)thisObj)->baseProcess->GetHitData();
		if (hitData) hitLoc = hitData->unk60;
	}
	*result = hitLoc;
	return true;
}

bool Hook_HasPerk_Execute(COMMAND_ARGS)		// Modifies HasPerk to allow detection of follower perks on followers.
{
	BGSPerk *perk;
	UInt32 useAlt = 0;
	UInt8 rank = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &useAlt))
	{
		if (thisObj == g_thePlayer)
			rank = g_thePlayer->GetPerkRank(perk, false);
		else if (thisObj->IsActor() && ((Actor*)thisObj)->isTeammate)
			rank = g_thePlayer->GetPerkRank(perk, true);
	}
	*result = rank ? 1 : 0;
	DoConsolePrint(result);
	return true;
}

bool Hook_HasPerk_Eval(Actor *thisObj, BGSPerk *perk, UInt32 useAlt, double *result)
{
	if (thisObj == g_thePlayer)
		*result = g_thePlayer->GetPerkRank(perk, false) ? 1 : 0;
	else if (thisObj->IsActor() && thisObj->isTeammate)
		*result = g_thePlayer->GetPerkRank(perk, true) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Hook_GetBaseObject_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *baseForm = thisObj->GetBaseForm();
	if (baseForm) REFR_RES = baseForm->refID;
	DoConsolePrint(baseForm);
	return true;
}

TESObjectWEAP* __fastcall ExtractWeapon(TESObjectWEAP *weapon, TESObjectREFR *thisObj)
{
	if (!weapon)
	{
		if (!thisObj) return NULL;
		weapon = (TESObjectWEAP*)thisObj->baseForm;
	}
	else if IS_ID(weapon, TESObjectREFR)
		weapon = (TESObjectWEAP*)((TESObjectREFR*)weapon)->baseForm;
	return IS_ID(weapon, TESObjectWEAP) ? weapon : NULL;
}

bool Hook_IsKeyPressed_Execute(COMMAND_ARGS)
{
	UInt32 keyID, flags = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &keyID, &flags))
		*result = g_DIHookCtrl->IsKeyPressed(keyID, flags);
	else *result = 0;
	return true;
}

bool Hook_IsKeyPressed_Eval(TESObjectREFR *thisObj, UInt32 keyID, UInt32 flags, double *result)
{
	*result = g_DIHookCtrl->IsKeyPressed(keyID, flags);
	return true;
}

bool __fastcall IsControlPressed(UInt32 ctrlID, UInt32 flags = 1)
{
	if (s_controllerReady)
		return GetXIControlPressed(g_inputGlobals->controllerBinds[ctrlID], flags);
	UInt32 keyID = g_inputGlobals->keyBinds[ctrlID];
	if ((keyID != 0xFF) && g_DIHookCtrl->IsKeyPressed(keyID, flags))
		return true;
	else
	{
		keyID = g_inputGlobals->mouseBinds[ctrlID];
		return (keyID != 0xFF) && g_DIHookCtrl->IsKeyPressed(keyID + 0x100, flags);
	}
}

bool __fastcall IsControlPressedRaw(UInt32 ctrlID)
{
	if (s_controllerReady)
		return GetXIControlPressed(g_inputGlobals->controllerBinds[ctrlID], 2);
	UInt32 keyID = g_inputGlobals->keyBinds[ctrlID];
	if ((keyID != 0xFF) && g_DIHookCtrl->IsKeyPressedRaw(keyID))
		return true;
	else
	{
		keyID = g_inputGlobals->mouseBinds[ctrlID];
		return (keyID != 0xFF) && g_DIHookCtrl->IsKeyPressedRaw(keyID + 0x100);
	}
}

bool Hook_IsControlPressed_Execute(COMMAND_ARGS)
{
	UInt32 ctrlID, flags = 1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &ctrlID, &flags) && (ctrlID < kMaxControlBinds))
		*result = IsControlPressed(ctrlID, flags);
	else *result = 0;
	return true;
}

bool Hook_IsControlPressed_Eval(TESObjectREFR *thisObj, UInt32 ctrlID, UInt32 flags, double *result)
{
	if (ctrlID < kMaxControlBinds)
		*result = IsControlPressed(ctrlID, flags);
	else *result = 0;
	return true;
}

typedef UnorderedMap<const char*, Setting*> GameSettingMap;
GameSettingMap s_gameSettingsMap(0x1000), s_INISettingsMap(0x800);

void InitSettingMaps()
{
	GameSettingCollection *gameSettings = *(GameSettingCollection**)0x11C8048;
	Setting *setting;
	for (auto gstIter = gameSettings->settingMap.Begin(); !gstIter.End(); ++gstIter)
	{
		setting = gstIter.Get();
		if (setting && setting->name)
			s_gameSettingsMap[setting->name] = setting;
	}

	ListNode<Setting> *istIter = (*(IniSettingCollection**)0x11F96A0)->settings.Head();
	do
	{
		setting = istIter->data;
		if (setting && setting->ValidType())
			s_INISettingsMap[setting->name] = setting;
	}
	while (istIter = istIter->next);
	istIter = (*(IniSettingCollection**)0x11F35A0)->settings.Head();
	do
	{
		setting = istIter->data;
		if (setting && setting->ValidType())
			s_INISettingsMap[setting->name] = setting;
	}
	while (istIter = istIter->next);
	istIter = (*(IniSettingCollection**)0x11CC694)->settings.Head();
	do
	{
		setting = istIter->data;
		if (setting && setting->ValidType())
			s_INISettingsMap[setting->name] = setting;
	}
	while (istIter = istIter->next);
	istIter = (*(IniSettingCollection**)0x11F35A4)->settings.Head();
	do
	{
		setting = istIter->data;
		if (setting && setting->ValidType())
			s_INISettingsMap[setting->name] = setting;
	}
	while (istIter = istIter->next);
}

bool Hook_GetNumericGameSetting_Execute(COMMAND_ARGS)
{
	*result = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer) && ((s_strArgBuffer[0] | 0x20) != 's'))
	{
		Setting *setting = s_gameSettingsMap.Get(s_strArgBuffer);
		if (setting)
		{
			setting->Get(result);
			DoConsolePrint(result);
		}
		else if (IsConsoleOpen())
			Console_Print("GetNumericGameSetting >> SETTING NOT FOUND");
	}
	return true;
}

bool Hook_SetNumericGameSetting_Execute(COMMAND_ARGS)
{
	*result = 0;
	double newVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &newVal) && ((s_strArgBuffer[0] | 0x20) != 's'))
	{
		Setting *setting = s_gameSettingsMap.Get(s_strArgBuffer);
		if (setting)
		{
			setting->Set(newVal);
			*result = 1;
		}
		else if (IsConsoleOpen())
			Console_Print("SetNumericGameSetting >> NOT FOUND");
	}
	return true;
}

bool Hook_GetNumericINISetting_Execute(COMMAND_ARGS)
{
	*result = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer) && ((s_strArgBuffer[0] | 0x20) != 's'))
	{
		Setting *setting = s_INISettingsMap.Get(s_strArgBuffer);
		if (setting)
		{
			setting->Get(result);
			DoConsolePrint(result);
		}
		else if (IsConsoleOpen())
			Console_Print("GetNumericINISetting >> SETTING NOT FOUND");
	}
	return true;
}

bool Hook_SetNumericINISetting_Execute(COMMAND_ARGS)
{
	*result = 0;
	double newVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &newVal) && ((s_strArgBuffer[0] | 0x20) != 's'))
	{
		Setting *setting = s_INISettingsMap.Get(s_strArgBuffer);
		if (setting)
		{
			setting->Set(newVal);
			*result = 1;
		}
		else if (IsConsoleOpen())
			Console_Print("SetNumericINISetting >> NOT FOUND");
	}
	return true;
}

SInt32 __fastcall IsRefInList(BGSListForm *listForm, TESForm *form)
{
	SInt32 index = listForm->list.GetIndexOf(form);
	if ((index < 0) && form->GetIsReference())
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
	thisObj->Update3D();
	return true;
}

bool Hook_ActorValueToStringC_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	UInt32 avCode, nameType = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &avCode, &nameType) && (avCode <= kAVCode_DamageThreshold))
	{
		ActorValueInfo *avInfo = g_actorValueInfoArray[avCode];
		switch (nameType)
		{
		case 0:
			resStr = avInfo->infoName;
			break;
		case 1:
			resStr = avInfo->fullName.name.m_data;
			break;
		case 2:
			resStr = avInfo->avName.m_data;
			break;
		}
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

Cmd_Execute IsPluginInstalled, GetPluginVersion;

bool __fastcall IsJIPAlias(const char *pluginName)
{
	return !StrCompare(pluginName, "JIP NVSE Plugin") || !StrCompare(pluginName, "lutana_nvse") || StrBeginsCI(pluginName, "JIP LN NVSE");
}

bool Hook_IsPluginInstalled_Execute(COMMAND_ARGS)
{
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		if (IsJIPAlias(s_strArgBuffer))
			*result = 1;
		else IsPluginInstalled(PASS_COMMAND_ARGS);
	}
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Hook_GetPluginVersion_Execute(COMMAND_ARGS)
{
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		if (IsJIPAlias(s_strArgBuffer))
			*result = JIP_LN_VERSION;
		else GetPluginVersion(PASS_COMMAND_ARGS);
	}
	else *result = 0;
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
	cmdInfo = GetCmdByOpcode(0x1403);
	cmdInfo->execute = Hook_GetBaseObject_Execute;
	cmdInfo = GetCmdByOpcode(0x1453);
	cmdInfo->execute = Hook_IsKeyPressed_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsKeyPressed_Eval;
	cmdInfo = GetCmdByOpcode(0x146B);
	cmdInfo->execute = Hook_IsControlPressed_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsControlPressed_Eval;
	cmdInfo = GetCmdByOpcode(0x149A);
	cmdInfo->execute = Hook_GetNumericGameSetting_Execute;
	cmdInfo = GetCmdByOpcode(0x149B);
	cmdInfo->execute = Hook_SetNumericGameSetting_Execute;
	cmdInfo->params = kParams_JIP_OneString_OneDouble;
	cmdInfo = GetCmdByOpcode(0x149C);
	cmdInfo->execute = Hook_GetNumericINISetting_Execute;
	cmdInfo = GetCmdByOpcode(0x149D);
	cmdInfo->execute = Hook_SetNumericINISetting_Execute;
	cmdInfo->params = kParams_JIP_OneString_OneDouble;
	cmdInfo = GetCmdByOpcode(0x1511);
	cmdInfo->execute = Hook_IsRefInList_Execute;
	cmdInfo->eval = (Cmd_Eval)Hook_IsRefInList_Eval;
	cmdInfo = GetCmdByOpcode(0x152D);
	cmdInfo->execute = Hook_Update3D_Execute;
	cmdInfo = GetCmdByOpcode(0x158D);
	cmdInfo->execute = Hook_ActorValueToStringC_Execute;
	cmdInfo = GetCmdByOpcode(0x15DF);
	IsPluginInstalled = cmdInfo->execute;
	cmdInfo->execute = Hook_IsPluginInstalled_Execute;
	cmdInfo = GetCmdByOpcode(0x15E0);
	GetPluginVersion = cmdInfo->execute;
	cmdInfo->execute = Hook_GetPluginVersion_Execute;

	SayTo = GetCmdByOpcode(0x1034)->execute;
	KillActor = GetCmdByOpcode(0x108B)->execute;
	AddNote = GetCmdByOpcode(0x117C)->execute;
	AttachAshPile = GetCmdByOpcode(0x1211)->execute;
	GetRefs = GetCmdByOpcode(0x15C7)->execute;

	PrintLog("> Command patches initialized successfully.\n");
}