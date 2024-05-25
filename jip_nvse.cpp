#include "internal/jip_core.h"
#include "internal/hooks.h"
#include "internal/patches_cmd.h"
#include "internal/patches_game.h"
#include "internal/lutana.h"
#include "internal/serialization.h"

#include "functions_jip/jip_fn_actor.h"
#include "functions_jip/jip_fn_ammo.h"
#include "functions_jip/jip_fn_ammo_effect.h"
#include "functions_jip/jip_fn_armor.h"
#include "functions_jip/jip_fn_aux_variable.h"
#include "functions_jip/jip_fn_body_part_data.h"
#include "functions_jip/jip_fn_cell.h"
#include "functions_jip/jip_fn_class.h"
#include "functions_jip/jip_fn_climate.h"
#include "functions_jip/jip_fn_combat_style.h"
#include "functions_jip/jip_fn_destruction_data.h"
#include "functions_jip/jip_fn_effect.h"
#include "functions_jip/jip_fn_explosion.h"
#include "functions_jip/jip_fn_faction.h"
#include "functions_jip/jip_fn_global_var.h"
#include "functions_jip/jip_fn_grass.h"
#include "functions_jip/jip_fn_imagespace.h"
#include "functions_jip/jip_fn_impact_data.h"
#include "functions_jip/jip_fn_input.h"
#include "functions_jip/jip_fn_inventory.h"
#include "functions_jip/jip_fn_land_texture.h"
#include "functions_jip/jip_fn_light.h"
#include "functions_jip/jip_fn_minimap.h"
#include "functions_jip/jip_fn_misc_ref.h"
#include "functions_jip/jip_fn_miscellaneous.h"
#include "functions_jip/jip_fn_package.h"
#include "functions_jip/jip_fn_perk.h"
#include "functions_jip/jip_fn_projectile.h"
#include "functions_jip/jip_fn_quest.h"
#include "functions_jip/jip_fn_ref_map.h"
#include "functions_jip/jip_fn_script.h"
#include "functions_jip/jip_fn_sound.h"
#include "functions_jip/jip_fn_string.h"
#include "functions_jip/jip_fn_texture_set.h"
#include "functions_jip/jip_fn_ui.h"
#include "functions_jip/jip_fn_utility.h"
#include "functions_jip/jip_fn_water.h"
#include "functions_jip/jip_fn_weapon.h"
#include "functions_jip/jip_fn_weather.h"
#include "functions_jip/jip_fn_worldspace.h"
#include "functions_jip/jip_fn_ccc.h"

#include "functions_ln/ln_fn_activator.h"
#include "functions_ln/ln_fn_actor.h"
#include "functions_ln/ln_fn_casino.h"
#include "functions_ln/ln_fn_cell.h"
#include "functions_ln/ln_fn_challenge.h"
#include "functions_ln/ln_fn_game_data.h"
#include "functions_ln/ln_fn_gamepad.h"
#include "functions_ln/ln_fn_inventory.h"
#include "functions_ln/ln_fn_leveled_list.h"
#include "functions_ln/ln_fn_load_screen.h"
#include "functions_ln/ln_fn_map_marker.h"
#include "functions_ln/ln_fn_math.h"
#include "functions_ln/ln_fn_misc_ref.h"
#include "functions_ln/ln_fn_miscellaneous.h"
#include "functions_ln/ln_fn_perk.h"
#include "functions_ln/ln_fn_race.h"
#include "functions_ln/ln_fn_radio_ref.h"
#include "functions_ln/ln_fn_recipe.h"
#include "functions_ln/ln_fn_terminal.h"
#include "functions_ln/ln_fn_utility.h"
#include "functions_ln/ln_fn_weapon.h"
#if JIP_DEBUG
#include "internal/debug.h"
#endif

extern "C"
{
	BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		return TRUE;
	}
};

bool NVSEPlugin_Query(const NVSEInterface *nvse, PluginInfo *info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "JIP LN NVSE";
	info->version = JIP_LN_VERSION_INT;
	if (nvse->isEditor)
	{
		//s_log().Create("jip_ln_nvse_editor.log");
		return true;
	}
	s_log().Create("jip_ln_nvse.log");
	int version = nvse->nvseVersion;
	s_nvseVersion = (version >> 24) + (((version >> 16) & 0xFF) * 0.1) + (((version & 0xFF) >> 4) * 0.01);
	if (version < 0x6030040)
	{
		PrintLog("ERROR: NVSE version is outdated (v%.2f). This plugin requires v6.34 minimum.", s_nvseVersion);
		MessageBox(nullptr, "ERROR!\n\nxNVSE version is outdated.\n\nThis plugin requires v6.3.4 minimum.", "JIP LN NVSE Plugin", MB_OK | MB_ICONWARNING | MB_TOPMOST);
		return false;
	}
	PrintLog("NVSE version:\t%.2f\nJIP LN version:\t%.2f\n", s_nvseVersion, JIP_LN_VERSION);
	return true;
}

void NVSEMessageHandler(NVSEMessagingInterface::Message *nvseMsg);

bool NVSEPlugin_Load(const NVSEInterface *nvse)
{
	auto RegisterCommand = nvse->RegisterCommand;
	auto RegisterTypedCommand = nvse->RegisterTypedCommand;

	//____________________FUNCTIONS_LN____________________
	nvse->SetOpcodeBase(0x2200);
	//	v1.00
	/*2200*/REG_CMD(GetLNVersion);
	/*2201*/REG_CMD(FileExists);
	//	v2.00
	/*2202*/REG_CMD_STR(GetTimeStamp);
	/*2203*/REG_CMD(GetController);
	/*2204*/REG_CMD(IsButtonPressed);
	/*2205*/REG_CMD(GetLeftStickX);
	/*2206*/REG_CMD(GetLeftStickY);
	/*2207*/REG_CMD(GetRightStickX);
	/*2208*/REG_CMD(GetRightStickY);
	/*2209*/REG_CMD(GetLeftTrigger);
	/*220A*/REG_CMD(GetRightTrigger);
	/*220B*/REG_CMD(GetDeadZoneLS);
	/*220C*/REG_CMD(GetDeadZoneRS);
	/*220D*/REG_CMD(SetDeadZoneLS);
	/*220E*/REG_CMD(SetDeadZoneRS);
	/*220F*/REG_CMD(GetPressedButtons);
	//	v3.00
	/*2210*/REG_CMD(GetNVSEVersionFull);
	/*2211*/REG_CMD(GetLoadedType);
	/*2212*/REG_CMD_ARR(ListToArray);
	/*2213*/REG_EMPTY;
	//	v4.00
	/*2214*/REG_CMD_FRM(GetCombatStyle);
	/*2215*/REG_EMPTY;
	/*2216*/REG_CMD_ARR(SaveHotkeys);
	/*2217*/REG_CMD(RestoreHotkeys);
	//	v5.00
	/*2218*/REG_CMD_FRM(GetBipedModelList);
	/*2219*/REG_CMD(SetBipedModelList);
	/*221A*/REG_CMD_STR(GetModName);
	/*221B*/REG_CMD(HasPerkRank);
	/*221C*/REG_CMD(SetGlobalValue);
	//	v6.00
	/*221D*/REG_CMD(GetSelfModIndex);
	/*221E*/REG_CMD(LeveledListAddForm);
	/*221F*/REG_CMD(LeveledListRemoveForm);
	/*2220*/REG_CMD(GetLevItemIndexByForm);
	/*2221*/REG_CMD(GetLevItemIndexByLevel);
	/*2222*/REG_CMD(GetChanceNone);
	/*2223*/REG_CMD(SetChanceNone);
	/*2224*/REG_CMD(GetNumLevItems);
	/*2225*/REG_CMD_FRM(GetNthLevItem);
	/*2226*/REG_CMD(GetNthLevItemLevel);
	/*2227*/REG_CMD(GetNthLevItemCount);
	/*2228*/REG_CMD(RemoveNthLevItem);
	/*2229*/REG_CMD(LeveledListClear);
	/*222A*/REG_CMD(DumpLevList);
	/*222B*/REG_CMD(GetChanceNoneGlobal);
	/*222C*/REG_CMD(ClearAllHotkeys);
	//	v7.00
	/*222D*/REG_CMD_FRM(GetMapMarkerRep);
	/*222E*/REG_CMD(SetMapMarkerRep);
	/*222F*/REG_CMD_STR(GetMapMarkerName);
	/*2230*/REG_CMD(SetMapMarkerName);
	/*2231*/REG_CMD(GetMapMarkerType);
	/*2232*/REG_CMD(SetMapMarkerType);
	/*2233*/REG_EMPTY;
	/*2234*/REG_CMD(SetMapMarkerVisible);
	/*2235*/REG_CMD(GetMapMarkerHidden);
	/*2236*/REG_CMD(SetMapMarkerHidden);
	/*2237*/REG_CMD(GetMapMarkerTravel);
	/*2238*/REG_CMD(SetMapMarkerTravel);
	/*2239*/REG_CMD_ARR(GetFormMods);
	/*223A*/REG_CMD(IsFormOverridden);
	/*223B*/REG_CMD(SetPerkRank);
	/*223C*/REG_CMD_FRM(GetMerchantContainer);
	//	v8.00
	/*223D*/REG_CMD(GetRadioBroadcastType);
	/*223E*/REG_CMD(SetRadioBroadcastType);
	/*223F*/REG_CMD(GetRadioRadius);
	/*2240*/REG_CMD(SetRadioRadius);
	/*2241*/REG_CMD(GetRadioStatic);
	/*2242*/REG_CMD(SetRadioStatic);
	/*2243*/REG_CMD(GetRadioPosRef);
	/*2244*/REG_CMD(SetRadioPosRef);
	/*2245*/REG_CMD(IsRadioRef);
	/*2246*/REG_CMD(Search);
	//	v9.00
	/*2247*/REG_CMD(LNGetAggroRadius);
	/*2248*/REG_CMD(LNSetAggroRadius);
	/*2249*/REG_CMD(IsMapMarker);
	/*224A*/REG_CMD_ARR(GetCellCoords);
	/*224B*/REG_CMD(GetLockedOut);
	/*224C*/REG_CMD(SetLockedOut);
	/*224D*/REG_CMD(GetCreatureType);
	//	v10.00
	/*224E*/REG_CMD_ARR(GetLoadedTypeArray);
	/*224F*/REG_CMD_FRM(GetPasswordNote);
	/*2250*/REG_CMD(SetPasswordNote);
	/*2251*/REG_CMD_ARR(GetFormRefs);
	/*2252*/REG_CMD_FRM(GetCellLightingTemplate);
	/*2253*/REG_CMD(SetCellLightingTemplate);
	/*2254*/REG_CMD_ARR(GetRecipeInputForms);
	/*2255*/REG_CMD_ARR(GetRecipeOutputForms);
	/*2256*/REG_CMD(GetRecipeInputCount);
	/*2257*/REG_CMD(SetRecipeInputCount);
	/*2258*/REG_CMD(GetRecipeOutputCount);
	/*2259*/REG_CMD(SetRecipeOutputCount);
	/*225A*/REG_CMD(GetRecipeRequiredSkill);
	/*225B*/REG_CMD(SetRecipeRequiredSkill);
	/*225C*/REG_CMD(GetRecipeRequiredSkillLevel);
	/*225D*/REG_CMD(SetRecipeRequiredSkillLevel);
	/*225E*/REG_CMD(ReplaceRecipeInputForm);
	/*225F*/REG_CMD(ReplaceRecipeOutputForm);
	//	v11.00
	/*2260*/REG_CMD_ARR(GetFormRecipes);
	/*2261*/REG_CMD_ARR(GetFormRecipeOutputs);
	/*2262*/REG_CMD_FRM(GetWorldspaceParentWorldspace);
	/*2263*/REG_CMD(GetTerminalLock);
	/*2264*/REG_CMD(GetArmorClass);
	/*2265*/REG_CMD(IsRaceInList);
	/*2266*/REG_CMD_FRM(GetFormFromMod);
	/*2267*/REG_CMD(GetPerkEntryCount);
	/*2268*/REG_CMD(GetNthPerkEntryType);
	/*2269*/REG_CMD(GetNthPerkEntryFunction);
	/*226A*/REG_CMD(SetNthPerkEntryFunction);
	/*226B*/REG_CMD_FRM(GetNthPerkEntryForm);
	/*226C*/REG_CMD(SetNthPerkEntryForm);
	/*226D*/REG_CMD(GetNthPerkEntryValue1);
	/*226E*/REG_CMD(SetNthPerkEntryValue1);
	/*226F*/REG_CMD(GetNthPerkEntryValue2);
	/*2270*/REG_CMD(SetNthPerkEntryValue2);
	/*2271*/REG_CMD_STR(GetNthPerkEntryString);
	/*2272*/REG_CMD(SetNthPerkEntryString);
	/*2273*/REG_CMD_FRM(GetActivatorRadioStation);
	/*2274*/REG_CMD(SetActivatorRadioStation);
	/*2275*/REG_CMD_STR(GetActivatorPrompt);
	/*2276*/REG_CMD(SetActivatorPrompt);
	/*2277*/REG_CMD_FRM(GetActivatorWaterType);
	/*2278*/REG_CMD(SetActivatorWaterType);
	/*2279*/REG_CMD_FRM(GetActiveQuest);
	/*227A*/REG_CMD(GetINIFloat);
	/*227B*/REG_CMD_STR(GetINIString);
	/*227C*/REG_CMD(SetINIFloat);
	/*227D*/REG_CMD(SetINIString);
	/*227E*/REG_CMD_ARR(GetINISection);
	/*227F*/REG_CMD(SetINISection);
	/*2280*/REG_CMD(IsParentActivateOnly);
	/*2281*/REG_CMD(SetLinkedReference);
	/*2282*/REG_CMD(SetBaseForm);
	/*2283*/REG_CMD_STR(GetStringSetting);
	/*2284*/REG_CMD(SetStringSetting);
	/*2285*/REG_CMD_ARR(GetINISectionNames);
	/*2286*/REG_CMD(GetRaceVoice);
	/*2287*/REG_CMD(SetRaceVoice);
	/*2288*/REG_CMD(GetRaceAgeRace);
	/*2289*/REG_CMD(SetRaceAgeRace);
	//	v12.00
	/*228A*/REG_CMD_ARR(GetFilesInFolder);
	/*228B*/REG_EMPTY;
	/*228C*/REG_EMPTY;
	/*228D*/REG_EMPTY;
	/*228E*/REG_CMD(GetLeveledListFlags);
	/*228F*/REG_CMD(SetLeveledListFlags);
	/*2290*/REG_CMD_FRM(GetActivatorSoundLooping);
	/*2291*/REG_CMD(SetActivatorSoundLooping);
	/*2292*/REG_CMD_FRM(GetActivatorSoundActivate);
	/*2293*/REG_CMD(SetActivatorSoundActivate);
	/*2294*/REG_CMD(EnableButton);
	/*2295*/REG_CMD(DisableButton);
	/*2296*/REG_CMD(IsButtonDisabled);
	/*2297*/REG_CMD(HoldButton);
	/*2298*/REG_CMD(ReleaseButton);
	/*2299*/REG_CMD(IsButtonHeld);
	/*229A*/REG_CMD(IsTrait);
	/*229B*/REG_CMD_STR(GetLoadScreenTexture);
	/*229C*/REG_CMD(SetLoadScreenTexture);
	/*229D*/REG_CMD_FRM(GetLoadScreenType);
	/*229E*/REG_CMD(SetLoadScreenType);
	/*229F*/REG_CMD_ARR(GetLoadScreenTypeTextRGB);
	/*22A0*/REG_CMD(SetLoadScreenTypeTextRGB);
	/*22A1*/REG_CMD(CopyFaceGenFrom);
	//	v13.00
	/*22A2*/REG_CMD(SetRace);
	/*22A3*/REG_CMD_ARR(GetFoldersInFolder);
	/*22A4*/REG_CMD(EnableTrigger);
	/*22A5*/REG_CMD(DisableTrigger);
	/*22A6*/REG_CMD(IsTriggerDisabled);
	/*22A7*/REG_CMD(HoldTrigger);
	/*22A8*/REG_CMD(ReleaseTrigger);
	/*22A9*/REG_CMD(IsTriggerHeld);
	/*22AA*/REG_CMD(GetPCCanUsePowerArmor);
	/*22AB*/REG_CMD_STR(GetIdleAnimPath);
	/*22AC*/REG_CMD(SetIdleAnimPath);
	/*22AD*/REG_CMD_STR(LNGetName);
	/*22AE*/REG_CMD(RemoveINIKey);
	/*22AF*/REG_CMD(RemoveINISection);
	//	v14.00
	/*22B0*/REG_CMD(GetGlobalTimeMultiplier);
	/*22B1*/REG_CMD(GetCasinoMaxWinnings);
	/*22B2*/REG_CMD(SetCasinoMaxWinnings);
	/*22B3*/REG_CMD(GetCasinoBJPayout);
	/*22B4*/REG_CMD(SetCasinoBJPayout);
	/*22B5*/REG_CMD(GetCasinoBJ17Stand);
	/*22B6*/REG_CMD(SetCasinoBJ17Stand);
	/*22B7*/REG_CMD(GetCasinoNumDecks);
	/*22B8*/REG_CMD(SetCasinoNumDecks);
	/*22B9*/REG_CMD(GetCasinoShufflePercent);
	/*22BA*/REG_CMD(SetCasinoShufflePercent);
	/*22BB*/REG_CMD(GetCasinoReelStops);
	/*22BC*/REG_CMD(SetCasinoReelStops);
	/*22BD*/REG_CMD(AddNoteNS);
	//	v15.00
	/*22BE*/REG_CMD_FRM(GetCellWaterForm);
	/*22BF*/REG_CMD_FRM(GetWaterFormEffect);
	/*22C0*/REG_CMD(ar_Cat);
	/*22C1*/REG_CMD(GetInventoryWeight);
	/*22C2*/REG_CMD(GetChallengeType);
	/*22C3*/REG_CMD(GetChallengeThreshold);
	/*22C4*/REG_CMD(SetChallengeThreshold);
	/*22C5*/REG_CMD(GetChallengeInterval);
	/*22C6*/REG_CMD(SetChallengeInterval);
	/*22C7*/REG_CMD(GetChallengeValue1);
	/*22C8*/REG_CMD(SetChallengeValue1);
	/*22C9*/REG_CMD(GetChallengeValue2);
	/*22CA*/REG_CMD(SetChallengeValue2);
	/*22CB*/REG_CMD(GetChallengeValue3);
	/*22CC*/REG_CMD(SetChallengeValue3);
	/*22CD*/REG_CMD_FRM(GetChallengeForm1);
	/*22CE*/REG_CMD(SetChallengeForm1);
	/*22CF*/REG_CMD_FRM(GetChallengeForm2);
	/*22D0*/REG_CMD(SetChallengeForm2);
	/*22D1*/REG_CMD_FRM(GetRecipeCategory);
	/*22D2*/REG_CMD(SetRecipeCategory);
	/*22D3*/REG_CMD_FRM(GetRecipeSubcategory);
	/*22D4*/REG_CMD(SetRecipeSubcategory);
	/*22D5*/REG_EMPTY;
	/*22D6*/REG_CMD(GetReferenceFlag);
	/*22D7*/REG_CMD(SetReferenceFlag);
	/*22D8*/REG_CMD(AddRecipeInputForm);
	/*22D9*/REG_CMD(AddRecipeOutputForm);
	/*22DA*/REG_CMD(RemoveRecipeInputForm);
	/*22DB*/REG_CMD(RemoveRecipeOutputForm);
	/*22DC*/REG_CMD(AddRecipeCondition);
	/*22DD*/REG_EMPTY;
	/*22DE*/REG_CMD(GetFormFlag);
	/*22DF*/REG_CMD(SetFormFlag);
	/*22E0*/REG_CMD(GetDistance2D);
	/*22E1*/REG_CMD(GetDistance3D);
	/*22E2*/REG_CMD(GetIngestibleFlag);
	/*22E3*/REG_CMD(SetIngestibleFlag);
	/*22E4*/REG_CMD(IsSkillMagazine);
	/*22E5*/REG_EMPTY;
	/*22E6*/REG_CMD(GetServiceFlag);
	/*22E7*/REG_CMD(SetServiceFlag);
	/*22E8*/REG_CMD(LeveledListReplaceForm);
	/*22E9*/REG_EMPTY;
	//	v16.00
	/*22EA*/REG_CMD_FRM(GetCellImageSpace);
	/*22EB*/REG_CMD(PlayerHasKey);
	/*22EC*/REG_CMD(GetCellFlag);
	/*22ED*/REG_CMD(SetCellFlag);
	/*22EE*/REG_EMPTY;	//	GetENBFloat
	/*22EF*/REG_EMPTY;	//	SetENBFloat
	/*22F0*/REG_CMD(fsqrt);
	/*22F1*/REG_CMD(GetCellWaterHeight);
	/*22F2*/REG_CMD(SetCellWaterHeight);
	/*22F3*/REG_CMD_FRM(GetAnimObjectIdle);
	/*22F4*/REG_CMD(SetAnimObjectIdle);
	/*22F5*/REG_CMD(GetActorTemplateFlag);
	/*22F6*/REG_CMD(SetActorTemplateFlag);
	/*22F7*/REG_CMD(BaseGetItemCount);
	/*22F8*/REG_CMD(BaseAddItem);
	/*22F9*/REG_CMD(BaseRemoveItem);
	/*22FA*/REG_CMD(LNSetActorTemplate);
	/*22FB*/REG_CMD(BaseAddItemHealth);
	/*22FC*/REG_CMD(LNSetName);
	//	v17.00
	/*22FD*/REG_CMD(fSin);
	/*22FE*/REG_CMD(fCos);
	/*22FF*/REG_CMD_ARR(SortFormsByType);
	/*2300*/REG_CMD(GetFormCountType);
	/*2301*/REG_CMD_FRM(GetPickupSound);
	/*2302*/REG_CMD(SetPickupSound);
	/*2303*/REG_CMD_FRM(GetPutdownSound);
	/*2304*/REG_CMD(SetPutdownSound);
	/*2305*/REG_CMD_FRM(GetContainerOpenSound);
	/*2306*/REG_CMD(SetContainerOpenSound);
	/*2307*/REG_CMD_FRM(GetContainerCloseSound);
	/*2308*/REG_CMD(SetContainerCloseSound);
	/*2309*/REG_EMPTY;
	/*230A*/REG_CMD_ARR(GetPlayerRegions);
	/*230B*/REG_CMD(fAtan);
	/*230C*/REG_CMD(GetPerkLevel);
	//	v18.00
	/*230D*/REG_CMD(GetZoneFlag);
	/*230E*/REG_CMD(SetZoneFlag);
	/*230F*/REG_CMD(fAsin);
	/*2310*/REG_CMD(fAcos);
	/*2311*/REG_CMD(SetNthLevItem);
	/*2312*/REG_CMD(SetNthLevItemLevel);
	/*2313*/REG_CMD(SetNthLevItemCount);
	/*2314*/REG_CMD(GetBaseFactionRank);
	/*2315*/REG_CMD(SetBaseFactionRank);
	/*2316*/REG_CMD_FRM(GetImpactDataSet);
	/*2317*/REG_CMD(SetImpactDataSet);
	/*2318*/REG_CMD(HasActionRef);
	/*2319*/REG_CMD(SetHotkey);
	/*231A*/REG_CMD(GetChallengeFlags);
	/*231B*/REG_CMD(SetChallengeFlags);
	/*231C*/REG_CMD(LNIsPlayable);
	/*231D*/REG_CMD(LNSetIsPlayable);
	/*231E*/REG_CMD(GetZone);
	/*231F*/REG_CMD(GetBaseKarma);
	/*2320*/REG_CMD(SetBaseKarma);
	/*2321*/REG_CMD(ActorHasEffect);
	/*2322*/REG_EMPTY;
	/*2323*/REG_CMD_ARR(GetEquippedData);
	/*2324*/REG_CMD(SetEquippedData);
	/*2325*/REG_CMD(EquipItemData);
	/*2326*/REG_CMD(Console);
	/*2327*/REG_CMD(GetDefaultMessageTime);
	/*2328*/REG_CMD(SetDefaultMessageTime);
	/*2329*/REG_EMPTY;
	/*232A*/REG_EMPTY;
	/*232B*/REG_CMD(GetWeaponKillImpulse);
	/*232C*/REG_CMD(SetWeaponKillImpulse);
	/*232D*/REG_CMD(GetWeaponImpulseDistance);
	/*232E*/REG_CMD(SetWeaponImpulseDistance);
	/*232F*/REG_CMD_FRM(GetWeaponVATSEffect);
	/*2330*/REG_CMD(SetWeaponVATSEffect);
	/*2331*/REG_CMD(GetWeaponCritFlags);
	/*2332*/REG_CMD(SetWeaponCritFlags);
	/*2333*/REG_CMD_FRM(GetBodyPartData);
	//	v19.00
	/*2334*/REG_CMD(GetRadius);
	/*2335*/REG_CMD(SetRadius);
	/*2336*/REG_CMD(CrosshairRefInList);
	/*2337*/REG_CMD_ARR(GetGameSettings);
	/*2338*/REG_CMD(GetShowQuestItems);
	/*2339*/REG_CMD(GetWaterTrait);
	/*233A*/REG_CMD(SetWaterTrait);

#if JIP_DEBUG
	RegisterDebugFunctions(nvse);
#endif

	//____________________FUNCTIONS_JIP____________________
	nvse->SetOpcodeBase(0x2600);
	//	v1.00
	/*2600*/REG_CMD(SetPersistent);
	/*2601*/REG_CMD_FRM(GetActorTemplate);
	/*2602*/REG_CMD_FRM(GetLeveledActorBase);
	/*2603*/REG_CMD(GetCreatureDamage);
	/*2604*/REG_CMD(SetCreatureDamage);
	/*2605*/REG_CMD(GetIsPoisoned);
	/*2606*/REG_CMD(AddItemAlt);
	/*2607*/REG_CMD(GetValueAlt);
	/*2608*/REG_CMD(IsComponentLoaded);
	/*2609*/REG_CMD(InjectUIXML);
	/*260A*/REG_CMD(InjectUIComponent);
	/*260B*/REG_CMD(GetAmmoTraitNumeric);
	/*260C*/REG_CMD(SetAmmoTraitNumeric);
	/*260D*/REG_CMD_FRM(GetAmmoProjectile);
	/*260E*/REG_CMD(SetAmmoProjectile);
	/*260F*/REG_CMD(GetNumAmmoEffects);
	/*2610*/REG_CMD_FRM(GetNthAmmoEffect);
	/*2611*/REG_CMD(AddAmmoEffect);
	/*2612*/REG_CMD(RemoveAmmoEffect);
	/*2613*/REG_CMD(GetAmmoEffectTraitNumeric);
	/*2614*/REG_CMD(SetAmmoEffectTraitNumeric);
	/*2615*/REG_CMD(GetProjectileTraitNumeric);
	/*2616*/REG_CMD(SetProjectileTraitNumeric);
	/*2617*/REG_CMD(GetProjectileFlag);
	/*2618*/REG_CMD(SetProjectileFlag);
	/*2619*/REG_CMD_FRM(GetProjectileExplosion);
	/*261A*/REG_CMD(SetProjectileExplosion);
	/*261B*/REG_CMD(GetExplosionTraitNumeric);
	/*261C*/REG_CMD(SetExplosionTraitNumeric);
	/*261D*/REG_CMD_FRM(GetExplosionTraitForm);
	/*261E*/REG_CMD(SetExplosionTraitForm);
	/*261F*/REG_CMD(GetExplosionFlag);
	/*2620*/REG_CMD(SetExplosionFlag);
	/*2621*/REG_CMD(AddScriptVariable);
	/*2622*/REG_CMD(GetDelayElapsed);
	/*2623*/REG_CMD(SetDelayElapsed);
	/*2624*/REG_CMD(GetPackageFlag);
	/*2625*/REG_CMD(SetPackageFlag);
	/*2626*/REG_CMD(GetGlobalVariable);
	/*2627*/REG_CMD(SetGlobalVariable);
	/*2628*/REG_CMD_FRM(GetFactionReputationType);
	/*2629*/REG_CMD(SetFactionReputationType);
	/*262A*/REG_CMD_STR(RefToString);
	/*262B*/REG_CMD_FRM(StringToRef);
	/*262C*/REG_CMD(GetObjectDimensions);
	/*262D*/REG_CMD(GetIsItem);
	/*262E*/REG_CMD(GetMinOf);
	/*262F*/REG_CMD(GetMaxOf);
	//	v2.00
	/*2630*/REG_CMD(GetCombatStyleTraitNumeric);
	/*2631*/REG_CMD(SetCombatStyleTraitNumeric);
	/*2632*/REG_CMD(GetCombatStyleFlag);
	/*2633*/REG_CMD(SetCombatStyleFlag);
	/*2634*/REG_CMD(SetLinkedReference);
	/*2635*/REG_CMD_ARR(GetEnableChildren);
	/*2636*/REG_CMD_ARR(GetLinkedChildren);
	/*2637*/REG_CMD_ARR(GetFollowers);
	/*2638*/REG_CMD(GetCursorPos);
	/*2639*/REG_CMD_STR(GetUIString);
	/*263A*/REG_CMD_ARR(ReadArrayFromFile);
	/*263B*/REG_CMD_STR(ReadStringFromFile);
	//	v3.00
	/*263C*/REG_CMD(WriteArrayToFile);
	/*263D*/REG_CMD(WriteStringToFile);
	/*263E*/REG_CMD(GetClassTraitNumeric);
	/*263F*/REG_CMD(SetClassTraitNumeric);
	/*2640*/REG_CMD(GetClassFlag);
	/*2641*/REG_CMD(SetClassFlag);
	/*2642*/REG_CMD(GetLightTraitNumeric);
	/*2643*/REG_CMD(SetLightTraitNumeric);
	/*2644*/REG_CMD(GetLightFlag);
	/*2645*/REG_CMD(SetLightFlag);
	/*2646*/REG_CMD(GetSoundTraitNumeric);
	/*2647*/REG_CMD(SetSoundTraitNumeric);
	/*2648*/REG_CMD(GetSoundFlag);
	/*2649*/REG_CMD(SetSoundFlag);
	//	v4.00
	/*264A*/REG_CMD(GetWeaponDetectionSoundLevel);
	/*264B*/REG_CMD(SetWeaponDetectionSoundLevel);
	/*264C*/REG_CMD(IsEquippedWeaponSilenced);
	/*264D*/REG_CMD(IsEquippedWeaponScoped);
	/*264E*/REG_CMD_FRM(GetWeaponSound);
	/*264F*/REG_CMD(SetWeaponSound);
	/*2650*/REG_CMD(SetWeaponItemMod);
	/*2651*/REG_CMD(SetWeaponItemModEffect);
	/*2652*/REG_CMD(SetWeaponItemModValue);
	/*2653*/REG_CMD_FRM(GetWeatherImageSpaceMod);
	/*2654*/REG_CMD(SetWeatherImageSpaceMod);
	/*2655*/REG_CMD_STR(GetWeatherTexture);
	/*2656*/REG_CMD(SetWeatherTexture);
	/*2657*/REG_CMD_STR(GetWeatherPrecipitationModel);
	/*2658*/REG_CMD(SetWeatherPrecipitationModel);
	/*2659*/REG_CMD(GetWeatherTraitNumeric);
	/*265A*/REG_CMD(SetWeatherTraitNumeric);
	/*265B*/REG_CMD(GetWeatherRGBColor);
	/*265C*/REG_CMD(SetWeatherRGBColor);
	/*265D*/REG_CMD_FRM(GetLandTextureTextureSet);
	/*265E*/REG_CMD(SetLandTextureTextureSet);
	/*265F*/REG_CMD(GetLandTextureTraitNumeric);
	/*2660*/REG_CMD(SetLandTextureTraitNumeric);
	/*2661*/REG_CMD(GetLandTextureNumGrasses);
	/*2662*/REG_CMD_FRM(GetLandTextureNthGrass);
	/*2663*/REG_CMD(LandTextureAddGrass);
	/*2664*/REG_CMD(LandTextureRemoveGrass);
	/*2665*/REG_CMD_STR(GetTextureSetTexture);
	/*2666*/REG_CMD(SetTextureSetTexture);
	/*2667*/REG_CMD(GetTextureSetTraitNumeric);
	/*2668*/REG_CMD(SetTextureSetTraitNumeric);
	/*2669*/REG_CMD(GetTextureSetFlag);
	/*266A*/REG_CMD(SetTextureSetFlag);
	/*266B*/REG_CMD_STR(GetClimateSunTexture);
	/*266C*/REG_CMD(SetClimateSunTexture);
	/*266D*/REG_CMD_STR(GetClimateSunGlareTexture);
	/*266E*/REG_CMD(SetClimateSunGlareTexture);
	/*266F*/REG_CMD_STR(GetClimateNightSkyModel);
	/*2670*/REG_CMD(SetClimateNightSkyModel);
	/*2671*/REG_CMD(GetClimateTraitNumeric);
	/*2672*/REG_CMD(SetClimateTraitNumeric);
	/*2673*/REG_CMD(GetClimateNumWeatherTypes);
	/*2674*/REG_CMD_FRM(GetClimateNthWeatherType);
	/*2675*/REG_CMD(GetClimateNthWeatherChance);
	/*2676*/REG_CMD_FRM(GetClimateNthWeatherGlobal);
	/*2677*/REG_CMD(ClimateAddWeatherType);
	/*2678*/REG_CMD(ClimateRemoveWeatherType);
	//	v5.00
	/*2679*/REG_CMD(GetPrimitiveBound);
	/*267A*/REG_CMD(SetPrimitiveBound);
	/*267B*/REG_CMD(GetNumEffects);
	/*267C*/REG_CMD_FRM(GetNthEffectBase);
	/*267D*/REG_CMD(GetNthEffectTraitNumeric);
	/*267E*/REG_CMD(SetNthEffectTraitNumeric);
	/*267F*/REG_CMD(GetNumActorEffects);
	/*2680*/REG_CMD(GetNthActorEffect);
	//	v6.00
	/*2681*/REG_CMD(GetActiveMenuMode);
	/*2682*/REG_CMD_STR(GetActiveUIComponentName);
	/*2683*/REG_CMD_STR(GetActiveUIComponentFullName);
	/*2684*/REG_CMD(GetActiveUIComponentID);
	/*2685*/REG_CMD_FRM(GetMenuTargetRef);
	/*2686*/REG_CMD(GetMenuItemFilter);
	/*2687*/REG_CMD(ClickMenuButton);
	//	v7.00
	/*2688*/REG_CMD(RemoveItemTarget);
	/*2689*/REG_CMD(GetActorLevelingData);
	/*268A*/REG_CMD(SetActorLevelingData);
	//	v8.00
	/*268B*/REG_CMD(SetNthEffectBase);
	/*268C*/REG_CMD(HasVariableAdded);
	/*268D*/REG_CMD(RemoveScriptVariable);
	/*268E*/REG_CMD(GetLoadOrderChanged);
	/*268F*/REG_CMD(ValidateModIndex);
	/*2690*/REG_CMD(RemoveAllAddedVariables);
	//	v9.00
	/*2691*/REG_CMD(AuxiliaryVariableGetSize);
	/*2692*/REG_CMD(AuxiliaryVariableGetType);
	/*2693*/REG_CMD(AuxiliaryVariableGetFloat);
	/*2694*/REG_CMD(AuxiliaryVariableSetFloat);
	/*2695*/REG_CMD_FRM(AuxiliaryVariableGetRef);
	/*2696*/REG_CMD(AuxiliaryVariableSetRef);
	/*2697*/REG_CMD_STR(AuxiliaryVariableGetString);
	/*2698*/REG_CMD(AuxiliaryVariableSetString);
	/*2699*/REG_CMD_ARR(AuxiliaryVariableGetAll);
	/*269A*/REG_CMD(AuxiliaryVariableErase);
	/*269B*/REG_CMD(AuxiliaryVariableEraseAll);
	/*269C*/REG_CMD(RefMapArrayGetSize);
	/*269D*/REG_CMD(RefMapArrayGetType);
	/*269E*/REG_CMD_ARR(RefMapArrayGetFirst);
	/*269F*/REG_CMD_ARR(RefMapArrayGetNext);
	/*26A0*/REG_CMD_ARR(RefMapArrayGetKeys);
	/*26A1*/REG_CMD_ARR(RefMapArrayGetAll);
	/*26A2*/REG_CMD(RefMapArrayErase);
	/*26A3*/REG_CMD(RefMapArrayDestroy);
	/*26A4*/REG_CMD(RefMapArrayGetFloat);
	/*26A5*/REG_CMD(RefMapArraySetFloat);
	/*26A6*/REG_CMD_FRM(RefMapArrayGetRef);
	/*26A7*/REG_CMD(RefMapArraySetRef);
	/*26A8*/REG_CMD_STR(RefMapArrayGetString);
	/*26A9*/REG_CMD(RefMapArraySetString);
	/*26AA*/REG_CMD(ClearJIPSavedData);
	//	v9.10
	/*26AB*/REG_CMD(SetValueAlt);
	//	v10.00
	/*26AC*/REG_CMD_STR(GetBodyPartDataSkeleton);
	/*26AD*/REG_CMD(SetBodyPartDataSkeleton);
	/*26AE*/REG_CMD_FRM(GetBodyPartDataRagdoll);
	/*26AF*/REG_CMD(SetBodyPartDataRagdoll);
	/*26B0*/REG_CMD(GetBodyPartDataHasPart);
	/*26B1*/REG_CMD(GetBodyPartTraitNumeric);
	/*26B2*/REG_CMD(SetBodyPartTraitNumeric);
	/*26B3*/REG_CMD_FRM(GetBodyPartTraitForm);
	/*26B4*/REG_CMD(SetBodyPartTraitForm);
	/*26B5*/REG_CMD_STR(GetBodyPartName);
	/*26B6*/REG_CMD(SetBodyPartName);
	/*26B7*/REG_CMD_STR(GetBodyPartReplacementModel);
	/*26B8*/REG_CMD(SetBodyPartReplacementModel);
	/*26B9*/REG_CMD(GetBodyPartFlag);
	/*26BA*/REG_CMD(SetBodyPartFlag);
	/*26BB*/REG_CMD(GetWeaponModReloadAnim);
	/*26BC*/REG_CMD(SetWeaponModReloadAnim);
	//	v11.00
	/*26BD*/REG_CMD_ARR(AuxiliaryVariableGetAsArray);
	/*26BE*/REG_CMD(AuxiliaryVariableSetFromArray);
	/*26BF*/REG_CMD_STR(GetSoundSourceFile);
	/*26C0*/REG_CMD(SetSoundSourceFile);
	/*26C1*/REG_CMD(GetDestructionDataHealth);
	/*26C2*/REG_CMD(SetDestructionDataHealth);
	/*26C3*/REG_CMD(GetDestructionDataTargetable);
	/*26C4*/REG_CMD(SetDestructionDataTargetable);
	/*26C5*/REG_CMD(GetDestructionDataNumStages);
	/*26C6*/REG_CMD(GetNthDestructionStageTrait);
	/*26C7*/REG_CMD(SetNthDestructionStageTrait);
	/*26C8*/REG_CMD_FRM(GetNthDestructionStageExplosion);
	/*26C9*/REG_CMD(SetNthDestructionStageExplosion);
	/*26CA*/REG_CMD_FRM(GetNthDestructionStageDebris);
	/*26CB*/REG_CMD(SetNthDestructionStageDebris);
	/*26CC*/REG_CMD_STR(GetNthDestructionStageReplacement);
	/*26CD*/REG_CMD(SetNthDestructionStageReplacement);
	//	v12.00
	/*26CE*/REG_CMD_ARR(GetTeammates);
	/*26CF*/REG_CMD_FRM(GetCurrentWeather);
	/*26D0*/REG_CMD_FRM(GetCurrentClimate);
	/*26D1*/REG_CMD(SetCurrentClimate);
	/*26D2*/REG_CMD(RefreshCurrentClimate);
	/*26D3*/REG_CMD_FRM(GetWorldspaceClimate);
	/*26D4*/REG_CMD(SetWorldspaceClimate);
	/*26D5*/REG_CMD_FRM(GetActorVoiceType);
	/*26D6*/REG_CMD(SetActorVoiceType);
	//	v13.00
	/*26D7*/REG_CMD(GetCreatureReach);
	/*26D8*/REG_CMD(GetIsImmobile);
	/*26D9*/REG_CMD(PickFromList);
	/*26DA*/REG_CMD(SetStageAlt);
	/*26DB*/REG_CMD(ToggleNoZPosReset);
	/*26DC*/REG_CMD_FRM(GetProjectileRefSource);
	/*26DD*/REG_CMD(SetProjectileRefSource);
	/*26DE*/REG_CMD_FRM(GetProjectileRefWeapon);
	/*26DF*/REG_CMD(SetProjectileRefWeapon);
	/*26E0*/REG_CMD(GetProjectileRefLifeTime);
	/*26E1*/REG_CMD(GetProjectileRefDistanceTraveled);
	/*26E2*/REG_CMD(GetProjectileRefDamage);
	/*26E3*/REG_CMD(SetProjectileRefDamage);
	/*26E4*/REG_CMD(GetProjectileRefSpeedMult);
	/*26E5*/REG_CMD(SetProjectileRefSpeedMult);
	/*26E6*/REG_EMPTY;
	/*26E7*/REG_CMD(GetWorldspaceFlag);
	/*26E8*/REG_CMD(SetWorldspaceFlag);
	/*26E9*/REG_CMD(GetIdleLoopTimes);
	/*26EA*/REG_CMD(ToggleCreatureModel);
	/*26EB*/REG_CMD(CreatureHasModel);
	/*26EC*/REG_CMD_ARR(GetCreatureModels);
	/*26ED*/REG_CMD(DisableNavMeshAlt);
	/*26EE*/REG_CMD(EnableNavMeshAlt);
	/*26EF*/REG_CMD(GetTerrainHeight);
	//	v14.00
	/*26F0*/REG_CMD(RefMapArrayValidate);
	/*26F1*/REG_CMD(HasPrimitive);
	/*26F2*/REG_CMD(AddPrimitive);
	/*26F3*/REG_CMD(AddDestructionStage);
	/*26F4*/REG_CMD(RemoveDestructionStage);
	/*26F5*/REG_CMD_STR(GetWeaponShellCasingModel);
	/*26F6*/REG_CMD(SetWeaponShellCasingModel);
	/*26F7*/REG_CMD(AddNewEffect);
	/*26F8*/REG_CMD(RemoveNthEffect);
	/*26F9*/REG_CMD(SetObjectEffect);
	/*26FA*/REG_CMD_FRM(GetActorUnarmedEffect);
	/*26FB*/REG_CMD(SetActorUnarmedEffect);
	//	v15.00
	/*26FC*/REG_CMD(IsPCInCombat);
	/*26FD*/REG_CMD(CCCOnLoad);
	/*26FE*/REG_CMD(CCCSetFloat);
	/*26FF*/REG_CMD(CCCSetString);
	/*2700*/REG_CMD(CCCSetTrait);
	/*2701*/REG_CMD(CCCGetDistance);
	/*2702*/REG_CMD(CCCTaskPackageFlags);
	/*2703*/REG_CMD(CCCSetEquipped);
	/*2704*/REG_CMD(CCCInFaction);
	/*2705*/REG_CMD(CCCSetNCCS);
	/*2706*/REG_CMD(GetEncumbranceRate);
	/*2707*/REG_CMD(CCCLoadNCCS);
	/*2708*/REG_CMD_FRM(CCCSavedForm);
	/*2709*/REG_CMD_STR(CCCLocationName);
	/*270A*/REG_CMD_FRM(CCCGetReputation);
	//	v16.00
	/*270B*/REG_CMD_FRM(GetSelectedItemRef);
	/*270C*/REG_CMD(GetWeaponRefModFlags);
	/*270D*/REG_CMD(SetWeaponRefModFlags);
	/*270E*/REG_CMD_ARR(GetBarterItems);
	/*270F*/REG_CMD(GetItemRefCurrentHealth);
	/*2710*/REG_CMD(SetItemRefCurrentHealth);
	/*2711*/REG_CMD(GetBarterGoldAlt);
	//	v17.00
	/*2712*/REG_CMD_FRM(GetRecipeMenuSelection);
	/*2713*/REG_CMD_FRM(GetRecipeMenuCategory);
	/*2714*/REG_CMD(UnlockRecipeMenuQuantity);
	//	v18.00
	/*2715*/REG_CMD(GetRecipeMenuNumSubcategories);
	//	v19.00
	/*2716*/REG_CMD(SetHotkeyItemRef);
	/*2717*/REG_CMD(EquipItemAlt);
	/*2718*/REG_CMD(UnequipItemAlt);
	/*2719*/REG_CMD(DropAlt);
	/*271A*/REG_CMD(DropMeAlt);
	/*271B*/REG_CMD_STR(GetFormDescription);
	/*271C*/REG_CMD_FRM(GetAddictionEffect);
	/*271D*/REG_CMD(SetAddictionEffect);
	/*271E*/REG_CMD(GetAddictionChance);
	/*271F*/REG_CMD(SetAddictionChance);
	/*2720*/REG_CMD(GetIsPoison);
	/*2721*/REG_CMD(GetContainerRespawns);
	/*2722*/REG_CMD(SetContainerRespawns);
	/*2723*/REG_CMD(ModBaseActorValue);
	/*2724*/REG_CMD(GetGrassTraitNumeric);
	/*2725*/REG_CMD(SetGrassTraitNumeric);
	/*2726*/REG_CMD_STR(GetGrassModel);
	/*2727*/REG_CMD(SetGrassModel);
	/*2728*/REG_CMD(GetWheelDisabled);
	/*2729*/REG_CMD(SetWheelDisabled);
	/*272A*/REG_CMD_FRM(GetInterruptPackage);
	/*272B*/REG_CMD(EnableImprovedRecipeMenu);
	/*272C*/REG_CMD(InitItemFilter);
	/*272D*/REG_CMD(SetItemFilter);
	/*272E*/REG_CMD(GetSessionTime);
	/*272F*/REG_CMD(GetQuestTargetsChanged);
	/*2730*/REG_CMD(GetWorldMapPosMults);
	/*2731*/REG_EMPTY;
	/*2732*/REG_EMPTY;
	//	v20.00
	/*2733*/REG_CMD_ARR(GetActiveEffects);
	/*2734*/REG_CMD(GetActorProcessingLevel);
	/*2735*/REG_CMD_ARR(GetActorsByProcessingLevel);
	/*2736*/REG_CMD_FRM(GetExteriorCell);
	/*2737*/REG_CMD(GetCellBuffered);
	/*2738*/REG_CMD(MessageExAlt);
	//	v21.00
	/*2739*/REG_CMD_ARR(GetTempEffects);
	/*273A*/REG_CMD(RemoveNthTempEffect);
	/*273B*/REG_CMD(GetActorLightAmount);
	/*273C*/REG_CMD(GetActorAlpha);
	/*273D*/REG_CMD(GetActorDiveBreath);
	/*273E*/REG_CMD(SetActorDiveBreath);
	/*273F*/REG_CMD(GetGameDifficulty);
	/*2740*/REG_CMD(ToggleFirstPerson);
	/*2741*/REG_CMD(StopIdle);
	/*2742*/REG_CMD_ARR(GetCombatTargets);
	/*2743*/REG_CMD_ARR(GetCombatAllies);
	/*2744*/REG_CMD_ARR(GetDetectedActors);
	/*2745*/REG_CMD_ARR(GetDetectingActors);
	/*2746*/REG_CMD(ClearActiveQuest);
	/*2747*/REG_CMD(SetFormDescription);
	/*2748*/REG_CMD(GetPCFastTravelled);
	/*2749*/REG_CMD(GetPCMovedCell);
	//	v22.00
	/*274A*/REG_CMD(GetPCDetectionState);
	/*274B*/REG_CMD_STR(GetFontFile);
	/*274C*/REG_CMD(SetFontFile);
	//	v23.00
	/*274D*/REG_CMD(ShowTextInputMenu);
	/*274E*/REG_EMPTY;
	/*274F*/REG_EMPTY;
	/*2750*/REG_EMPTY;
	/*2751*/REG_CMD_FRM(GetPipboyRadio);
	/*2752*/REG_CMD(CCCSayTo);
	/*2753*/REG_CMD(CCCRunResultScripts);
	//	v24.00
	/*2754*/REG_CMD(GetCombatDisabled);
	/*2755*/REG_CMD(SetCombatDisabled);
	/*2756*/REG_CMD(ToggleNoHealthReset);
	/*2757*/REG_CMD_FRM(GetCurrentStablePackage);
	/*2758*/REG_CMD(MoveToCell);
	/*2759*/REG_CMD(MoveToEditorPosition);
	/*275A*/REG_CMD(GetTeammateUsingAmmo);
	/*275B*/REG_CMD(SetTeammateUsingAmmo);
	/*275C*/REG_CMD(ToggleDetectionFix);
	/*275D*/REG_EMPTY;	//	ClearModNVSEVars
	/*275E*/REG_EMPTY;
	/*275F*/REG_CMD(ToggleIgnoreLockedDoors);
	/*2760*/REG_CMD(CCCSetFollowState);
	//	v25.00
	/*2761*/REG_CMD(IsInCombatWith);
	/*2762*/REG_CMD_STR(RefToPosStr);
	/*2763*/REG_CMD(MoveToPosStr);
	/*2764*/REG_CMD_ARR(RefMapArrayGetValue);
	/*2765*/REG_CMD(RefMapArraySetValue);
	/*2766*/REG_CMD_ARR(GetAllItems);
	/*2767*/REG_CMD_ARR(GetAllItemRefs);
	/*2768*/REG_CMD(LockEquipment);
	/*2769*/REG_CMD(IsAttacking);
	/*276A*/REG_CMD(GetPCUsingScope);
	/*276B*/REG_CMD(HolsterWeapon);
	/*276C*/REG_CMD(GetFallTimeElapsed);
	/*276D*/REG_CMD(GetFallTimeRemaining);
	/*276E*/REG_CMD(ResetFallTime);
	/*276F*/REG_CMD(GetCenterPos);
	//	v26.00
	/*2770*/REG_CMD(SetIdleLoopTimes);
	/*2771*/REG_CMD(CCCSMS);
	/*2772*/REG_CMD(GetDetonationTimer);
	/*2773*/REG_CMD(SetDetonationTimer);
	/*2774*/REG_CMD(GetMineArmed);
	/*2775*/REG_CMD(GetRefType);
	/*2776*/REG_CMD(GetEffectFlag);
	/*2777*/REG_CMD(SetEffectFlag);
	/*2778*/REG_CMD(GetBaseEffectFlag);
	/*2779*/REG_CMD(SetBaseEffectFlag);
	//	v27.00
	/*277A*/REG_CMD(GetPCUsingIronSights);
	/*277B*/REG_CMD_FRM(GetPCLastExteriorDoor);
	/*277C*/REG_CMD(GetRadiationLevelAlt);
	/*277D*/REG_CMD(IsInWater);
	/*277E*/REG_CMD(SetEmbeddedWeaponNode);
	/*277F*/REG_CMD(SetEmbeddedWeaponAV);
	/*2780*/REG_CMD_ARR(GetDroppedRefs);
	//	v28.00
	/*2781*/REG_CMD(MoveAwayFromPlayer);
	/*2782*/REG_CMD(FaceObject);
	/*2783*/REG_CMD(Turn);
	/*2784*/REG_CMD(SetMessageDisabled);
	/*2785*/REG_CMD(GetMessageDisabled);
	//	v29.00
	/*2786*/REG_CMD_FRM(GetAshPileSource);
	//	v30.00
	/*2787*/REG_CMD(GetMessageFlags);
	/*2788*/REG_CMD(SetMessageFlags);
	/*2789*/REG_CMD(ToggleObjectCollision);
	/*278A*/REG_CMD(IsSoundPlaying);
	/*278B*/REG_CMD(SwapTextureEx);
	/*278C*/REG_CMD(GetMaterialPropertyValue);
	/*278D*/REG_CMD(SetMaterialPropertyValue);
	/*278E*/REG_CMD_ARR(GetSoundPlayers);
	//	v31.00
	/*278F*/REG_CMD(SetMessageDisplayTime);
	/*2790*/REG_CMD_STR(GetImpactDataModel);
	/*2791*/REG_CMD(SetImpactDataModel);
	/*2792*/REG_CMD_AMB(GetImpactDataTrait);
	/*2793*/REG_CMD(SetImpactDataTraitNumeric);
	/*2794*/REG_CMD(SetImpactDataTraitForm);
	//	v32.00
	/*2795*/REG_CMD(sv_RegexMatch);
	/*2796*/REG_CMD_ARR(sv_RegexSearch);
	/*2797*/REG_CMD_STR(sv_RegexReplace);
	//	v33.00
	/*2798*/REG_CMD(SetOnMenuClickEventHandler);
	/*2799*/REG_CMD(SetOnFastTravelEventHandler);
	/*279A*/REG_CMD(SetOnMenuOpenEventHandler);
	/*279B*/REG_CMD(SetOnMenuCloseEventHandler);
	/*279C*/REG_CMD(SetOnAnimActionEventHandler);
	/*279D*/REG_CMD(SetOnPlayGroupEventHandler);
	/*279E*/REG_CMD(SetOnHealthDamageEventHandler);
	/*279F*/REG_CMD(SetOnCrippledLimbEventHandler);
	/*27A0*/REG_CMD(SetOnFireWeaponEventHandler);
	/*27A1*/REG_CMD_FRM(GetCurrentAmmo);
	/*27A2*/REG_CMD(GetCurrentAmmoRounds);
	//	v34.00
	/*27A3*/REG_CMD_STR(GetMoonTexture);
	/*27A4*/REG_CMD(SetMoonTexture);
	/*27A5*/REG_CMD(SetFullNameAlt);
	/*27A6*/REG_CMD(SetWeatherTransitionTimeOverride);
	//	v35.00
	/*27A7*/REG_CMD(GetDetectionValue);
	/*27A8*/REG_CMD(GetBaseActorValueAlt);
	/*27A9*/REG_CMD(SetOnMouseoverChangeEventHandler);
	/*27AA*/REG_CMD(GetAlwaysRun);
	/*27AB*/REG_CMD(GetAutoMove);
	/*27AC*/REG_CMD(SetSpeedMult);
	/*27AD*/REG_CMD(GetIsRagdolled);
	/*27AE*/REG_CMD(ForceActorDetectionValue);
	/*27AF*/REG_CMD(SetOnPCTargetChangeEventHandler);
	/*27B0*/REG_CMD(FreePlayer);
	//	v36.00
	/*27B1*/REG_CMD(AuxVarGetFltCond);
	/*27B2*/REG_CMD(GetLocalGravity);
	/*27B3*/REG_CMD(SetLocalGravityVector);
	/*27B4*/REG_CMD(GetActorVelocity);
	/*27B5*/REG_CMD(IsInAir);
	/*27B6*/REG_CMD(GetHasContact);
	/*27B7*/REG_CMD_ARR(GetContactRefs);
	//	v36.50
	/*27B8*/REG_CMD(GetHasContactBase);
	/*27B9*/REG_CMD(GetHasContactType);
	/*27BA*/REG_CMD_FRM(GetObjectUnderFeet);
	//	v40.00
	/*27BB*/REG_CMD(GetHasPhantom);
	/*27BC*/REG_CMD_STR(GetArmorRCT);
	/*27BD*/REG_CMD(SetArmorRCT);
	/*27BE*/REG_CMD(GetPerkFlag);
	/*27BF*/REG_CMD(SetPerkFlag);
	/*27C0*/REG_CMD(ModLogPrint);
	//	v41.00
	/*27C1*/REG_CMD_FRM(GetBaseEffectScript);
	/*27C2*/REG_CMD(SetBaseEffectScript);
	/*27C3*/REG_CMD_STR(GetWaterNoiseTexture);
	/*27C4*/REG_CMD(SetWaterNoiseTexture);
	/*27C5*/REG_CMD_FRM(SetWaterFormEffect);
	/*27C6*/REG_CMD_FRM(GetWaterSound);
	/*27C7*/REG_CMD(SetWaterSound);
	//	v42.00
	/*27C8*/REG_CMD(GetInteractionDisabled);
	/*27C9*/REG_CMD(SetInteractionDisabled);
	/*27CA*/REG_CMD(GetInteractionDisabledType);
	/*27CB*/REG_CMD(SetInteractionDisabledType);
	//	v43.00
	/*27CC*/REG_CMD(GetImageSpaceTrait);
	/*27CD*/REG_CMD(SetImageSpaceTrait);
	/*27CE*/REG_CMD(GetImageSpaceModTrait);
	/*27CF*/REG_CMD(SetImageSpaceModTrait);
	/*27D0*/REG_CMD(SetOnKeyDownEventHandler);
	/*27D1*/REG_CMD(SetOnKeyUpEventHandler);
	//	v43.50
	/*27D2*/REG_CMD(SetOnControlDownEventHandler);
	/*27D3*/REG_CMD(SetOnControlUpEventHandler);
	//	v44.00
	/*27D4*/REG_CMD(SetGameMainLoopCallback);
	/*27D5*/REG_CMD_FRM(GetGlobalRef);
	/*27D6*/REG_CMD(SetGlobalRef);
	/*27D7*/REG_CMD_STR(GetWorldspaceNoiseTexture);
	/*27D8*/REG_CMD(SetWorldspaceNoiseTexture);
	/*27D9*/REG_CMD_FRM(GetWorldspaceWaterType);
	/*27DA*/REG_CMD(SetWorldspaceWaterType);
	/*27DB*/REG_CMD_FRM(GetWorldspaceImagespace);
	/*27DC*/REG_CMD(SetWorldspaceImagespace);
	//	v45.00
	/*27DD*/REG_CMD(SetCellWaterForm);
	/*27DE*/REG_CMD_FRM(GetCellClimate);
	/*27DF*/REG_CMD(SetCellClimate);
	/*27E0*/REG_CMD_STR(GetCellNoiseTexture);
	/*27E1*/REG_CMD(SetCellNoiseTexture);
	//	v46.00
	/*27E2*/REG_CMD(RemoveMeIRAlt);
	/*27E3*/REG_CMD(GetScriptDisabled);
	/*27E4*/REG_CMD(SetScriptDisabled);
	/*27E5*/REG_CMD(GetScriptEventDisabled);
	/*27E6*/REG_CMD(SetScriptEventDisabled);
	/*27E7*/REG_CMD(FakeScriptEvent);
	/*27E8*/REG_CMD(RefreshItemsList);
	//	v46.50
	/*27E9*/REG_CMD(AddRefMapMarker);
	/*27EA*/REG_CMD(RemoveRefMapMarker);
	/*27EB*/REG_CMD(RefHasMapMarker);
	/*27EC*/REG_CMD(IsSpellTargetAlt);
	//	v47.00
	/*27ED*/REG_CMD(GetCalculatedWeaponDamage);
	/*27EE*/REG_CMD(GetOptionalPatch);
	/*27EF*/REG_CMD(SetOptionalPatch);
	//	v47.50
	/*27F0*/REG_CMD(SetPosEx);
	/*27F1*/REG_CMD_ARR(GetReticlePos);
	/*27F2*/REG_CMD(GetReticleRange);
	/*27F3*/REG_CMD(MoveToReticle);
	//	v48.00
	/*27F4*/REG_CMD(SetRefName);
	/*27F5*/REG_CMD(GetActorValueModifier);
	/*27F6*/REG_CMD(GetPerkModifier);
	/*27F7*/REG_CMD(SetOnQuestStageEventHandler);
	/*27F8*/REG_CMD(SetAngleEx);
	/*27F9*/REG_CMD_FRM(GetTeleportDoor);
	/*27FA*/REG_CMD(GetBarterPriceMult);
	/*27FB*/REG_CMD(SetBarterPriceMult);
	//	v48.80
	/*27FC*/REG_CMD(SetOnDialogTopicEventHandler);
	//	v49.00
	/*27FD*/REG_CMD(TapButton);
	/*27FE*/REG_CMD(TapTrigger);
	/*27FF*/REG_CMD(SetOnTriggerDownEventHandler);
	/*2800*/REG_CMD(SetOnTriggerUpEventHandler);
	//	v49.20
	/*2801*/REG_CMD(SetOnCriticalHitEventHandler);
	/*2802*/REG_CMD(GetHitHealthDamage);
	/*2803*/REG_CMD(GetHitLimbDamage);
	/*2804*/REG_CMD(CrippleLimb);
	//	v50.00
	/*2805*/REG_CMD(HoldControl);
	/*2806*/REG_CMD(ReleaseControl);
	/*2807*/REG_CMD(PlayIdleEx);
	/*2808*/REG_CMD(SetBipedModelPathAlt);
	/*2809*/REG_CMD(GetKillXP);
	/*280A*/REG_EMPTY;
	/*280B*/REG_CMD_FRM(GetKiller);
	/*280C*/REG_CMD(KillActorAlt);
	/*280D*/REG_CMD(ReloadEquippedModels);
	/*280E*/REG_CMD(MoveToFadeDelay);
	/*280F*/REG_CMD(RunScriptSnippet);
	/*2810*/REG_CMD(ScriptWait);
	/*2811*/REG_CMD(IsScriptWaiting);
	/*2812*/REG_CMD(StopScriptWaiting);
	/*2813*/REG_CMD_FRM(GetCrosshairWater);
	//	v50.50
	/*2814*/REG_CMD(GetScriptBlockDisabled);
	/*2815*/REG_CMD(SetScriptBlockDisabled);
	/*2816*/REG_CMD_FRM(GetPlayedIdle);
	/*2817*/REG_CMD(IsIdlePlayingEx);
	/*2818*/REG_CMD_ARR(GetObjectiveTargets);
	/*2819*/REG_CMD(SetObjectiveNthTarget);
	/*281A*/REG_CMD(GetObjectiveHasTarget);
	/*281B*/REG_CMD(AddObjectiveTarget);
	/*281C*/REG_CMD(RemoveObjectiveTarget);
	/*281D*/REG_CMD_STR(GetObjectiveText);
	/*281E*/REG_CMD(SetObjectiveText);
	//	v50.80
	/*281F*/REG_CMD_FRM(GetArmorAudioTemplate);
	/*2820*/REG_CMD(SetArmorAudioTemplate);
	/*2821*/REG_CMD(SetWeaponOut);
	//	v51.00
	/*2822*/REG_CMD(SetTerminalUIModel);
	/*2823*/REG_CMD(AddBaseEffectListEffect);
	/*2824*/REG_CMD(RemoveBaseEffectListEffect);
	/*2825*/REG_CMD(ShowQuantityMenu);
	//	v51.20
	/*2826*/REG_CMD(MessageBoxExAlt);
	/*2827*/REG_CMD_ARR(GetVATSTargets);
	/*2828*/REG_CMD(HasScriptBlock);
	//	v51.40
	/*2829*/REG_CMD(CastImmediate);
	/*282A*/REG_CMD(IsInCharGen);
	/*282B*/REG_CMD(DisableScriptedActivate);
	//	v51.60
	/*282C*/REG_CMD_FRM(GetHitAttacker);
	/*282D*/REG_CMD_FRM(GetHitProjectile);
	/*282E*/REG_CMD_FRM(GetHitWeapon);
	/*282F*/REG_CMD(GetGameDaysPassed);
	//	v51.80
	/*2830*/REG_CMD(GetWaterImmersionPerc);
	/*2831*/REG_CMD_STR(GetProjectileMuzzleFlash);
	/*2832*/REG_CMD(SetProjectileMuzzleFlash);
	/*2833*/REG_CMD(DismemberLimb);
	//	v52.00
	/*2834*/REG_CMD(RunBatchScript);
	//	v52.20
	/*2835*/REG_CMD(IsArmorAddon);
	//	v52.50
	/*2836*/REG_CMD(ToggleCraftingMessages);
	/*2837*/REG_CMD(ToggleHardcoreTracking);
	/*2838*/REG_CMD(SetGameDifficulty);
	/*2839*/REG_CMD_FRM(GetEquippedItemRef);
	/*283A*/REG_CMD(ExecuteScript);
	/*283B*/REG_EMPTY;	//	ReloadENB
	/*283C*/REG_CMD(IsFleeing);
	/*283D*/REG_CMD_FRM(GetEnemyHealthTarget);
	/*283E*/REG_CMD(SetCurrentAmmoRounds);
	/*283F*/REG_CMD(IsAnimPlayingEx);
	//	v52.60
	/*2840*/REG_CMD(AttachAshPileEx);
	/*2841*/REG_CMD(MarkActivatorAshPile);
	//	v52.70
	/*2842*/REG_CMD(SetActorVelocity);
	/*2843*/REG_CMD(GetRigidBodyMass);
	/*2844*/REG_CMD(PushObject);
	//	v53.00
	/*2845*/REG_CMD_STR(GetSkeletonModel);
	/*2846*/REG_CMD(SetNPCSkeletonModel);
	/*2847*/REG_CMD(SetCRESkeletonModel);
	/*2848*/REG_CMD(SetBodyPartData);
	/*2849*/REG_CMD(GetTargetUnreachable);
	/*284A*/REG_CMD(MoveToContainer);
	/*284B*/REG_CMD_ARR(GetNifBlockTranslation);
	/*284C*/REG_CMD(SetNifBlockTranslation);
	/*284D*/REG_CMD_ARR(GetNifBlockRotation);
	/*284E*/REG_CMD(SetNifBlockRotation);
	/*284F*/REG_CMD(GetNifBlockScale);
	/*2850*/REG_CMD(SetNifBlockScale);
	/*2851*/REG_CMD(GetNifBlockFlag);
	/*2852*/REG_CMD(SetNifBlockFlag);
	/*2853*/REG_CMD_ARR(GetBufferedCells);
	//	v53.20
	/*2854*/REG_CMD(SetVATSTargetable);
	/*2855*/REG_CMD(GetWindDirection);
	/*2856*/REG_CMD(SetWindDirection);
	/*2857*/REG_CMD_FRM(GetCreatureWeaponList);
	/*2858*/REG_CMD(SetWindSpeedMult);
	/*2859*/REG_CMD(TriggerLightningFX);
	/*285A*/REG_CMD(GetObjectVelocity);
	/*285B*/REG_CMD(GetAngularVelocity);
	/*285C*/REG_CMD(SetAngularVelocity);
	/*285D*/REG_CMD(PlaceAtCell);
	//	v53.40
	/*285E*/REG_CMD(GetRayCastPos);
	/*285F*/REG_CMD(GetAnimSequenceFrequency);
	/*2860*/REG_CMD(SetAnimSequenceFrequency);
	/*2861*/REG_CMD(StopSound);
	/*2862*/REG_CMD(IsMusicPlaying);
	/*2863*/REG_CMD(SetMusicState);
	//	v53.60
	/*2864*/REG_CMD(GetNoUnequip);
	/*2865*/REG_CMD(SetNoUnequip);
	/*2866*/REG_CMD(SetCursorPos);
	/*2867*/REG_CMD(MoveToNode);
	/*2868*/REG_CMD_FRM(GetDeathItem);
	/*2869*/REG_CMD(SetDeathItem);
	/*286A*/REG_CMD_FRM(GetActorLeveledList);
	/*286B*/REG_CMD_ARR(GetPlayerPerks);
	//	v53.80
	/*286C*/REG_CMD(UnloadUIComponent);
	/*286D*/REG_CMD_ARR(GetQuests);
	/*286E*/REG_CMD_ARR(GetQuestObjectives);
	/*286F*/REG_CMD_ARR(GetActiveObjectives);
	/*2870*/REG_CMD_ARR(GetObjectiveTeleportLinks);
	/*2871*/REG_CMD_ARR(GetNifBlockParentNodes);
	/*2872*/REG_CMD(ClearMessageQueue);
	/*2873*/REG_CMD(InitMiniMap);
	/*2874*/REG_CMD(UpdateMiniMap);
	//	v54.20
	/*2875*/REG_CMD_FRM(GetCrosshairRefEx);
	/*2876*/REG_CMD(IsMobile);
	/*2877*/REG_CMD(IsGrabbable);
	/*2878*/REG_CMD(SetOnLocationDiscoverEventHandler);
	//	v54.30
	/*2879*/REG_CMD(AttachLight);
	/*287A*/REG_CMD(RemoveLight);
	/*287B*/REG_CMD(SetOnCraftingEventHandler);
	/*287C*/REG_CMD(IsInKillCam);
	/*287D*/REG_CMD(SwapObjectLOD);
	/*287E*/REG_CMD(ResetClouds);
	//	v54.31
	/*287F*/REG_CMD(GetPluginHeaderVersion);
	//	v54.40
	/*2880*/REG_CMD(SetOnHitEventHandler);
	//	v54.45
	/*2881*/REG_CMD(SetSystemColor);
	/*2882*/REG_EMPTY;	//	SetWobblesRotation
	/*2883*/REG_CMD(GetArmourPenetrated);
	/*2884*/REG_CMD(GetImpactMaterialType);
	/*2885*/REG_CMD(SetImpactMaterialType);
	/*2886*/REG_CMD_FRM(GetImpactDataSetForm);
	/*2887*/REG_CMD(SetImpactDataSetForm);
	//	v54.50
	/*2888*/REG_CMD(SetQuestFlag);
	/*2889*/REG_CMD(GetQuestFlag);
	/*288A*/REG_CMD(PushActorAwayAlt);
	//	v54.60
	/*288B*/REG_CMD(SetGameHour);
	/*288C*/REG_CMD(GetGameVolume);
	/*288D*/REG_CMD(SetGameVolume);
	/*288E*/REG_CMD(MoveAwayFrom);
	/*288F*/REG_CMD_FRM(GetEquippedWeaponPoison);
	/*2890*/REG_CMD(SuppressQuestMessages);
	/*2891*/REG_CMD(TravelToRef);
	//	v54.65
	/*2892*/REG_CMD(GetIsLAA);
	/*2893*/REG_CMD(fTan);
	/*2894*/REG_CMD(fAtan2);
	//	v54.70
	/*2895*/REG_CMD(DonnerReedKuruParty);
	/*2896*/REG_CMD(SetArmorClass);
	/*2897*/REG_CMD_FRM(GetEquippedEx);
	/*2898*/REG_CMD(TestEquippedSlots);
	/*2899*/REG_CMD(GetWeaponOnHitSetting);
	/*289A*/REG_CMD(SetWeaponOnHitSetting);
	//	v54.75
	/*289B*/REG_CMD(GetWeaponSemiAutoFireDelay);
	/*289C*/REG_CMD(SetWeaponSemiAutoFireDelay);
	/*289D*/REG_CMD(IsStickDisabled);
	/*289E*/REG_CMD(SetStickDisabled);
	/*289F*/REG_EMPTY;	//	Sleep
	//	v54.80
	/*28A0*/REG_CMD_ARR(GetActiveIMODs);
	/*28A1*/REG_CMD(ToggleItemUnique);
	/*28A2*/REG_CMD(SetTextInputExtendedProps);
	/*28A3*/REG_CMD(SetTextInputString);
	/*28A4*/REG_CMD(ClickMenuTile);
	/*28A5*/REG_CMD(StringToActorValue);
	/*28A6*/REG_CMD_ARR(GetFactions);
	/*28A7*/REG_CMD_ARR(GetBaseItems);
	//	v54.85
	/*28A8*/REG_CMD_ARR(GetHit3DData);
	/*28A9*/REG_CMD_STR(GetWeaponModel);
	/*28AA*/REG_CMD(SetWeaponModel);
	/*28AB*/REG_CMD_FRM(GetOwnerOfCell);
	/*28AC*/REG_CMD(TogglePipBoyLight);
	/*28AD*/REG_CMD(GetHardcoreTracking);
	//	v54.90
	/*28AE*/REG_CMD(SetPerkLevel);
	/*28AF*/REG_CMD_FRM(GetProjectileRefImpactRef);
	/*28B0*/REG_CMD(SetOnProjectileImpactEventHandler);
	/*28B1*/REG_CMD(GetNoteRead);
	/*28B2*/REG_CMD(SetOnNoteAddedEventHandler);
	//	v54.95
	/*28B3*/REG_CMD(SetOnUseAidItemEventHandler);
	//	v55.00
	/*28B4*/REG_CMD(ToggleVanityWheel);
	/*28B5*/REG_CMD(ToggleHUDCursor);
	//	v55.05
	/*28B6*/REG_CMD(GetHardcoreStageThreshold);
	/*28B7*/REG_CMD(SetHardcoreStageThreshold);
	/*28B8*/REG_CMD_FRM(GetHardcoreStageEffect);
	/*28B9*/REG_CMD(SetHardcoreStageEffect);
	/*28BA*/REG_CMD(ClearModelAltTextures);
	/*28BB*/REG_CMD_FRM(GetCreatureSoundsTemplate);
	/*28BC*/REG_CMD(SetCreatureSoundsTemplate);
	//	v55.10
	/*28BD*/REG_CMD(GetDebugModeState);
	/*28BE*/REG_CMD(GetProjectileRefImpactMaterial);
	/*28BF*/REG_CMD(FreezeTime);
	/*28C0*/REG_CMD(GetConditionDamagePenalty);
	/*28C1*/REG_CMD(SetConditionDamagePenalty);
	//	v55.15
	/*28C2*/REG_EMPTY;	//	GetExtraFloat
	/*28C3*/REG_EMPTY;	//	SetExtraFloat
	/*28C4*/REG_CMD(ToggleBipedSlotVisibility);
	/*28C5*/REG_CMD(SetOnReloadWeaponEventHandler);
	//	v55.20
	/*28C6*/REG_CMD(AddTileFromTemplate);
	/*28C7*/REG_CMD(UpdatePlayer3D);
	/*28C8*/REG_CMD_AMB(GetArrayValue);
	//	v55.25
	/*28C9*/REG_CMD(SetOnRagdollEventHandler);
	/*28CA*/REG_CMD(PushActorNoRagdoll);
	//	v55.30
	/*28CB*/REG_CMD(SetUIFloatGradual);
	/*28CC*/REG_CMD(CloseActiveMenu);
	//	v55.35
	/*28CD*/REG_CMD(ReloadCloudTextures);
	//	v55.40
	/*28CE*/REG_CMD(ToggleImmortalMode);
	/*28CF*/REG_CMD_STR(GetBodyPartVATSTarget);
	/*28D0*/REG_CMD(SetLinearVelocity);
	//	v55.45
	/*28D1*/REG_CMD_ARR(GetEquippedArmorRefs);
	/*28D2*/REG_CMD_STR(GetHitNode);
	/*28D3*/REG_CMD(InsertNode);
	/*28D4*/REG_CMD(AttachModel);
	/*28D5*/REG_CMD(SynchronizePosition);
	/*28D6*/REG_CMD(ToggleCameraCollision);
	//	v55.50
	/*28D7*/REG_CMD(InitRockItLauncher);
	/*28D8*/REG_CMD(ForceClimate);
	/*28D9*/REG_CMD(GetInFactionList);
	/*28DA*/REG_CMD(LeveledListHasFormDeep);
	/*28DB*/REG_CMD(GetActorTiltAngle);
	/*28DC*/REG_CMD(SetActorTiltAngle);
	/*28DD*/REG_CMD(ReloadCharController);
	//	v55.55
	/*28DE*/REG_CMD(GetRandomInRange);
	/*28DF*/REG_CMD(ModelHasBlock);
	/*28E0*/REG_CMD(GetGroundMaterial);
	/*28E1*/REG_CMD(ShowLevelUpMenuEx);
	/*28E2*/REG_CMD_FRM(GetRayCastRef);
	/*28E3*/REG_CMD(GetRayCastMaterial);
	//	v55.65
	/*28E4*/REG_CMD(FireWeaponEx);
	//	v55.70
	/*28E5*/REG_CMD(GetArmorEffectiveDT);
	/*28E6*/REG_CMD(GetArmorEffectiveDR);
	//	v55.80
	/*28E7*/REG_CMD(GetActorGravityMult);
	/*28E8*/REG_CMD(SetActorGravityMult);
	//	v55.82
	/*28E9*/REG_CMD(ToggleHitEffects);
	/*28EA*/REG_CMD(ToggleNoMovementCombat);
	/*28EB*/REG_CMD(ToggleTeammateKillable);
	/*28EC*/REG_CMD(ToggleNoGunWobble);
	//	v56.00
	/*28ED*/REG_CMD(RewardXPExact);
	/*28EE*/REG_CMD(GetHitExtendedFlag);
	/*28EF*/REG_CMD(ClearDeadActors);
	/*28F0*/REG_CMD(GetCameraMovement);
	/*28F1*/REG_CMD_FRM(GetHotkeyItemRef);
	//	v56.02
	/*28F2*/REG_CMD_ARR(GetCollisionNodes);
	//	v56.05
	/*28F3*/REG_CMD_ARR(GetChildBlocks);
	/*28F4*/REG_CMD_ARR(GetBlockTextureSet);
	//	v56.08
	/*28F5*/REG_CMD_STR(GetReticleNode);
	/*28F6*/REG_EMPTY;
	/*28F7*/REG_CMD(EquippedWeaponHasModType);
	//	v56.10
	/*28F8*/REG_CMD(GetPosEx);
	/*28F9*/REG_CMD(GetAngleEx);
	//	v56.12
	/*28FA*/REG_CMD(ToggleMouseMovement);
	/*28FB*/REG_CMD(SetTextureTransformKey);
	//	v56.16
	/*28FC*/REG_CMD(AttachExtraCamera);
	/*28FD*/REG_CMD(ProjectExtraCamera);
	/*28FE*/REG_CMD(AttachUIXML);
	/*28FF*/REG_CMD(AttachUIComponent);
	//	v56.18
	/*2900*/REG_CMD(RenameNifBlock);
	//	v56.20
	/*2901*/REG_CMD(RemoveNifBlock);
	/*2902*/REG_CMD(SetInternalMarker);
	/*2903*/REG_CMD(FailQuest);
	//	v56.22
	/*2904*/REG_CMD(PlayAnimSequence);
	//	v56.26
	/*2905*/REG_CMD(RemoveAllPerks);
	//	v56.38
	/*2906*/REG_CMD(GetPointRayCastPos);
	/*2907*/REG_CMD(TogglePlayerSneaking);
	/*2908*/REG_CMD(GetActorMovementFlags);
	/*2909*/REG_CMD(GetTranslatedPos);
	/*290A*/REG_CMD(GetHitBaseWeaponDamage);
	/*290B*/REG_CMD(GetHitFatigueDamage);
	//	v56.42
	/*290C*/REG_CMD(RefreshAnimData);
	//	v56.44
	/*290D*/REG_CMD(GetNifBlockTranslationAlt);
	/*290E*/REG_CMD(GetNifBlockRotationAlt);
	/*290F*/REG_CMD(GetLinearVelocityAlt);
	/*2910*/REG_CMD(GetAngularVelocityAlt);
	/*2911*/REG_CMD(SetAngularVelocityEx);
	/*2912*/REG_CMD(GetActorVelocityAlt);
	//	v56.46
	/*2913*/REG_CMD(GetCollisionObjProperty);
	/*2914*/REG_CMD(SetCollisionObjProperty);
	/*2915*/REG_CMD(GetCollisionObjLayerType);
	/*2916*/REG_CMD(SetCollisionObjLayerType);
	//	v56.48
	/*2917*/REG_CMD(SetRefrModelPath);
	//	v56.52
	/*2918*/REG_EMPTY;	//	PlaceModel
	/*2919*/REG_CMD(AttachLine);
	//	v56.60
	/*291A*/REG_CMD(IsSpellTargetList);
	/*291B*/REG_CMD(GetExcludedCombatActions);
	/*291C*/REG_CMD(SetExcludedCombatActions);
	/*291D*/REG_CMD(SetArmorConditionPenalty);
	//	v56.65
	/*291E*/REG_CMD(PlaySound3DNode);
	/*291F*/REG_CMD(GetReticlePosAlt);
	/*2920*/REG_CMD(GetLightAmountAtPoint);
	/*2921*/REG_CMD(TransformWorldToLocal);
	/*2922*/REG_CMD(GetAnglesBetweenPoints);
	//	v56.68
	/*2923*/REG_CMD(GetP2PRayCastRange);
	/*2924*/REG_CMD(WeaponHasModType);
	//	v56.74
	/*2925*/REG_CMD(ProjectUITile);
	/*2926*/REG_CMD(RotateAroundPoint);
	/*2927*/REG_CMD(GetStringUIDimensions);
	//	v56.78
	/*2928*/REG_CMD(ToggleNodeCollision);
	/*2929*/REG_CMD_ARR(GetAllPerks);
	//	v56.80
	/*292A*/REG_CMD_FRM(GetEditorPosition);
	/*292B*/REG_CMD_FRM(GetWorldspacePersistentCell);
	//	v56.81
	/*292C*/REG_CMD(ScrollMouseWheel);
	//	v56.85
	/*292D*/REG_CMD(GetCellNorthRotation);
	/*292E*/REG_CMD(RefHasExtraData);
	/*292F*/REG_CMD_AMB(GetRefExtraData);
	/*2930*/REG_CMD(SetRefExtraData);
	/*2931*/REG_CMD(TogglePurgeOnUnload);
	//	v56.90
	/*2932*/REG_CMD_FRM(GetProjectileLight);
	/*2933*/REG_CMD(SetProjectileLight);
	/*2934*/REG_CMD(IsItemUnique);
	/*2935*/REG_CMD_ARR(GetMenuItemListRefs);
	//	v56.95
	/*2936*/REG_CMD_FRM(GetSelfAsInventoryRef);
	//	v57.05
	/*2937*/REG_CMD(SetDamageToArmorMaxPercent);
	//	v57.15
	/*2938*/REG_CMD(RegisterSRScript);
	/*2939*/REG_CMD(SetAmmoCasing);
	//	v57.20
	/*293A*/REG_CMD(AssignKeyword);
	/*293B*/REG_CMD(HasKeyword);
	/*293C*/REG_CMD_ARR(GetKeywordForms);
	/*293D*/REG_CMD(ToggleDepthClear);
	//	v57.30
	/*293E*/REG_CMD(HasKeywordCond);
	/*293F*/REG_CMD(GetStringHash);
	/*2940*/REG_CMD(CalculateShotsPerSec);
	/*2941*/REG_CMD(IsAttackQueued);
	/*2942*/REG_CMD(GetReticleTargetLimb);

	//===========================================================

	if (nvse->isEditor)
	{
		InitEditorPatches();
		return true;
	}

	nvse->InitExpressionEvaluatorUtils(&s_expEvalUtils);

	g_stringVar = *(NVSEStringVarInterface*)nvse->QueryInterface(kInterface_StringVar);
	g_arrayVar = *(NVSEArrayVarInterface*)nvse->QueryInterface(kInterface_ArrayVar);
	g_commandTbl = *(NVSECommandTableInterface*)nvse->QueryInterface(kInterface_CommandTable);
	g_script = *(NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
	g_serialization = *(NVSESerializationInterface*)nvse->QueryInterface(kInterface_Serialization);

	PluginHandle pluginHandle = nvse->GetPluginHandle();
	g_serialization.SetLoadCallback(pluginHandle, LoadGameCallback);
	g_serialization.SetSaveCallback(pluginHandle, SaveGameCallback);
	((NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging))->RegisterListener(pluginHandle, "NVSE", NVSEMessageHandler);

	NVSEDataInterface *nvseData = (NVSEDataInterface*)nvse->QueryInterface(kInterface_Data);
	g_DIHookCtrl = (DIHookControl*)nvseData->GetSingleton(NVSEDataInterface::kNVSEData_DIHookControl);
	InventoryRefCreate = (_InventoryRefCreate)nvseData->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceCreateEntry);
	InventoryRefGetForID = (_InventoryRefGetForID)nvseData->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceGetForRefID);
	CaptureLambdaVars = (_CaptureLambdaVars)nvseData->GetFunc(NVSEDataInterface::kNVSEData_LambdaSaveVariableList);
	UncaptureLambdaVars = (_UncaptureLambdaVars)nvseData->GetFunc(NVSEDataInterface::kNVSEData_LambdaUnsaveVariableList);

	return true;
}

__declspec(noinline) void InitContainers()
{
	s_fileExtToType->InsertList({{"nif", 1}, {"egm", 1}, {"egt", 1}, {"kf", 1}, {"psa", 1}, {"tri", 1}, {"dds", 2},
		{"fnt", 2}, {"psd", 2}, {"tai", 2}, {"tex", 2}, {"wav", 8}, {"lip", 0x10}, {"ogg", 0x10}, {"spt", 0x40},
		{"ctl", 0x100}, {"dat", 0x100}, {"dlodsettings", 0x100}, {"xml", 0x100}});

	s_internalMarkerIDs->InsertList({1, 2, 3, 4, 5, 6, 0x10, 0x12, 0x15, 0x23, 0x24, 0x32, 0x33, 0x34, 0x3B, 0x64, 0x65, 0x66,
		0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77});

	s_eventMasks->InsertList({{"OnActivate", 0}, {"OnAdd", 1}, {"OnEquip", 2}, {"OnActorEquip", 2}, {"OnDrop", 4}, {"OnUnequip", 8},
		{"OnActorUnequip", 8}, {"OnDeath", 0x10}, {"OnMurder", 0x20}, {"OnCombatEnd", 0x40}, {"OnHit", 0x80}, {"OnHitWith", 0x100},
		{"OnPackageStart", 0x200}, {"OnPackageDone", 0x400}, {"OnPackageChange", 0x800}, {"OnLoad", 0x1000}, {"OnMagicEffectHit", 0x2000},
		{"OnSell", 0x4000}, {"OnStartCombat", 0x8000}, {"OnOpen", 0x10000}, {"OnClose", 0x20000}, {"SayToDone", 0x40000}, {"OnGrab", 0x80000},
		{"OnRelease", 0x100000}, {"OnDestructionStageChange", 0x200000}, {"OnFire", 0x400000}, {"OnTrigger", 0x10000000},
		{"OnTriggerEnter", 0x20000000}, {"OnTriggerLeave", 0x40000000}, {"OnReset", 0x80000000}});

	s_optionalHacks->InsertList({{"bIgnoreDTDRFix", 1}, {"bEnableFO3Repair", 2}, {"bEnableBigGunsSkill", 3}, {"bProjImpactDmgFix", 4}, {"bGameDaysPassedFix", 5},
		{"bHardcoreNeedsFix", 6}, {"bNoFailedScriptLocks", 7}, {"bDoublePrecision", 8}, {"bQttSelectShortKeys", 9}, {"bFO3WpnDegradation", 11},
		{"bLocalizedDTDR", 12}, {"bVoiceModulationFix", 13}, {"bSneakBoundingBoxFix", 14}, {"bEnableNVACAlerts", 15}, {"bLoadScreenFix", 16},
		{"bNPCWeaponMods", 17}, {"uNPCPerks", 18}, {"bCreatureSpreadFix", 19}, {"uWMChancePerLevel", 20}, {"uWMChanceMin", 21}, {"uWMChanceMax", 22}});

	s_LNEventNames->InsertList({{"OnCellEnter", kLNEventMask_OnCellEnter}, {"OnCellExit", kLNEventMask_OnCellExit}, {"OnPlayerGrab", kLNEventMask_OnPlayerGrab},
		{"OnPlayerRelease", kLNEventMask_OnPlayerRelease}, {"OnCrosshairOn", kLNEventMask_OnCrosshairOn}, {"OnCrosshairOff", kLNEventMask_OnCrosshairOff},
		{"OnButtonDown", kLNEventMask_OnButtonDown}, {"OnButtonUp", kLNEventMask_OnButtonUp}, {"OnKeyDown", kLNEventMask_OnKeyDown},
		{"OnKeyUp", kLNEventMask_OnKeyUp}, {"OnControlDown", kLNEventMask_OnControlDown}, {"OnControlUp", kLNEventMask_OnControlUp}});

	s_menuNameToID->InsertList({{"MessageMenu", kMenuType_Message}, {"InventoryMenu", kMenuType_Inventory}, {"StatsMenu", kMenuType_Stats},
		{"HUDMainMenu", kMenuType_HUDMain}, {"LoadingMenu", kMenuType_Loading}, {"ContainerMenu", kMenuType_Container}, {"DialogMenu", kMenuType_Dialog},
		{"SleepWaitMenu", kMenuType_SleepWait}, {"StartMenu", kMenuType_Start}, {"LockpickMenu", kMenuType_LockPick}, {"QuantityMenu", kMenuType_Quantity},
		{"MapMenu", kMenuType_Map}, {"BookMenu", kMenuType_Book}, {"LevelUpMenu", kMenuType_LevelUp}, {"RepairMenu", kMenuType_Repair},
		{"RaceSexMenu", kMenuType_RaceSex}, {"CharGenMenu", kMenuType_CharGen}, {"TextEditMenu", kMenuType_TextEdit}, {"BarterMenu", kMenuType_Barter},
		{"SurgeryMenu", kMenuType_Surgery}, {"HackingMenu", kMenuType_Hacking}, {"VATSMenu", kMenuType_VATS}, {"ComputersMenu", kMenuType_Computers},
		{"RepairServicesMenu", kMenuType_RepairServices}, {"TutorialMenu", kMenuType_Tutorial}, {"SpecialBookMenu", kMenuType_SpecialBook},
		{"ItemModMenu", kMenuType_ItemMod}, {"LoveTesterMenu", kMenuType_LoveTester}, {"CompanionWheelMenu", kMenuType_CompanionWheel},
		{"TraitSelectMenu", kMenuType_TraitSelect}, {"RecipeMenu", kMenuType_Recipe}, {"SlotMachineMenu", kMenuType_SlotMachine},
		{"BlackjackMenu", kMenuType_Blackjack}, {"RouletteMenu", kMenuType_Roulette}, {"CaravanMenu", kMenuType_Caravan}, {"TraitMenu", kMenuType_Trait}});
}

void NVSEMessageHandler(NVSEMessagingInterface::Message *nvseMsg)
{
	switch (nvseMsg->type)
	{
		case NVSEMessagingInterface::kMessage_PostLoad:
		{
			WriteRelCall(0x86B0F4, (UInt32)GetSingletonsHook);
			SAFE_WRITE_BUF(0x86B1EE, "\x0F\x1F\x44\x00\x00");

			s_CPUFeatures = GetCPUFeatures();

			InitContainers();
			InitJIPHooks();
			InitGamePatches();
			InitCmdPatches();

			if (HMODULE hUIO = GetModuleHandle("ui_organizer"))
				UIOInjectComponent = (_UIOInjectComponent)GetProcAddress(hUIO, (LPCSTR)0xA);

			*s_inventoryIterator;

			break;
		}
		case NVSEMessagingInterface::kMessage_ExitGame_Console:
		case NVSEMessagingInterface::kMessage_ExitGame:
			JIPScriptRunner::RunScripts(JIPScriptRunner::kRunOn_ExitGame);
			PrintLog("> JIP MemoryPool session total allocations: %d KB", MemoryPool::GetTotalAllocSize() >> 0xA);
			break;
		case NVSEMessagingInterface::kMessage_ExitToMainMenu:
			ProcessDataChangedFlags(kChangedFlag_All);
			DoPreLoadGameHousekeeping();
			RestoreLinkedRefs();
			s_lastLoadedPath[0] = 0;
			JIPScriptRunner::RunScripts(JIPScriptRunner::kRunOn_ExitToMainMenu);
			break;
		case NVSEMessagingInterface::kMessage_LoadGame:
			break;
		case NVSEMessagingInterface::kMessage_SaveGame:
			COPY_BYTES(s_lastLoadedPath, nvseMsg->fosPath, nvseMsg->dataLen + 1);
			s_dataChangedFlags = 0;
			JIPScriptRunner::RunScripts(JIPScriptRunner::kRunOn_SaveGame);
			break;
		case NVSEMessagingInterface::kMessage_Precompile:
			break;
		case NVSEMessagingInterface::kMessage_PreLoadGame:
			if (StrCompareCS(nvseMsg->fosPath, s_lastLoadedPath))
			{
				COPY_BYTES(s_lastLoadedPath, nvseMsg->fosPath, nvseMsg->dataLen + 1);
				s_dataChangedFlags = kChangedFlag_All;
			}
			DoPreLoadGameHousekeeping();
			break;
		case NVSEMessagingInterface::kMessage_PostLoadGame:
			if (nvseMsg->fosLoaded)
			{
				DoLoadGameHousekeeping();
				JIPScriptRunner::RunScripts(JIPScriptRunner::kRunOn_LoadGame, JIPScriptRunner::kRunOn_LoadOrNewGame);
			}
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
			s_serializedVars.Reset();
			RestoreJIPFormFlags();
			JIPScriptRunner::RunScripts(JIPScriptRunner::kRunOn_NewGame, JIPScriptRunner::kRunOn_LoadOrNewGame);
			break;
		case NVSEMessagingInterface::kMessage_DeleteGameName:
			break;
		case NVSEMessagingInterface::kMessage_RenameGameName:
			break;
		case NVSEMessagingInterface::kMessage_RenameNewGameName:
			break;
		case NVSEMessagingInterface::kMessage_DeferredInit:
			DeferredInit();
			break;
		case NVSEMessagingInterface::kMessage_ClearScriptDataCache:
			break;
		case NVSEMessagingInterface::kMessage_MainGameLoop:
		{
			if (!s_mainLoopCallbacks->Empty())
				CycleMainLoopCallbacks(*s_mainLoopCallbacks);
			if (s_LNEventFlags)
				LN_ProcessEvents();
			if (s_HUDCursorMode && (g_interfaceManager->currentMode > 1))
			{
				s_HUDCursorMode = 0;
				g_DIHookCtrl->SetLMBDisabled(false);
			}
			if (s_doRefreshItemListBox)
			{
				s_doRefreshItemListBox = false;
				RefreshItemListBox();
			}
			break;
		}
		case NVSEMessagingInterface::kMessage_ScriptCompile:
			break;
		case NVSEMessagingInterface::kMessage_EventListDestroyed:
			break;
		case NVSEMessagingInterface::kMessage_PostQueryPlugins:
			break;
	}
}

double GetJIPLNVersion()
{
	return JIP_LN_VERSION;
}