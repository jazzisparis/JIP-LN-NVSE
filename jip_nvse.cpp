#define JIP_DEBUG 0

#include "internal/param_info.h"
#include "jip_nvse.h"
#include "internal/lutana.h"
#include "internal/hooks.h"
#include "internal/patches_cmd.h"
#include "internal/patches_game.h"
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
#include "functions_jip/jip_fn_minimap.h"

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

#include "internal/debug.h"

bool NVSEPlugin_Query(const NVSEInterface *nvse, PluginInfo *info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "JIP LN NVSE";
	info->version = JIP_LN_VERSION_MAJ;
	if (nvse->isEditor)
	{
		//s_log.Create("jip_ln_nvse_editor.log");
		return nvse->editorVersion == CS_VERSION_1_4_0_518;
	}
	s_log.Create("jip_ln_nvse.log");
	if (nvse->runtimeVersion != RUNTIME_VERSION_1_4_0_525)
	{
		PrintLog("ERROR: Unsupported runtime version (%08X).", nvse->runtimeVersion);
		return false;
	}
	int version = nvse->nvseVersion;
	s_nvseVersion = (version >> 24) + (((version >> 16) & 0xFF) * 0.1) + (((version & 0xFF) >> 4) * 0.01);
	if (version < 0x5010040)
	{
		PrintLog("ERROR: NVSE version is outdated (v%.2f). This plugin requires v5.14 minimum.", s_nvseVersion);
		return false;
	}
	if (version >= 0x5020010)
		s_xNVSE = true;
	PrintLog("NVSE version:\t%.2f\nJIP LN version:\t%.2f\nBase address:\t%08X\n", s_nvseVersion, JIP_LN_VERSION, GetModuleHandle("jip_nvse.dll"));
	return true;
}

bool NVSEPlugin_Load(const NVSEInterface *nvse)
{
	//____________________FUNCTIONS_LN____________________
	nvse->SetOpcodeBase(0x2200);
	//	v1.00
	REG_CMD(GetLNVersion)
	REG_CMD(FileExists)
	//	v2.00
	REG_CMD_STR(GetTimeStamp)
	REG_CMD(GetController)
	REG_CMD(IsButtonPressed)
	REG_CMD(GetLeftStickX)
	REG_CMD(GetLeftStickY)
	REG_CMD(GetRightStickX)
	REG_CMD(GetRightStickY)
	REG_CMD(GetLeftTrigger)
	REG_CMD(GetRightTrigger)
	REG_CMD(GetDeadZoneLS)
	REG_CMD(GetDeadZoneRS)
	REG_CMD(SetDeadZoneLS)
	REG_CMD(SetDeadZoneRS)
	REG_CMD(GetPressedButtons)
	//	v3.00
	REG_CMD(GetNVSEVersionFull)
	REG_CMD(GetLoadedType)
	REG_CMD_ARR(ListToArray)
	REG_CMD(EmptyCommand)
	//	v4.00
	REG_CMD(GetCombatStyle)
	REG_CMD_ARR(GetCellRefs)
	REG_CMD_ARR(SaveHotkeys)
	REG_CMD(RestoreHotkeys)
	//	v5.00
	REG_CMD(GetBipedModelList)
	REG_CMD(SetBipedModelList)
	REG_CMD_STR(GetModName)
	REG_CMD(HasPerkRank)
	REG_CMD(SetGlobalValue)
	//	v6.00
	REG_CMD(GetSelfModIndex)
	REG_CMD(LeveledListAddForm)
	REG_CMD(LeveledListRemoveForm)
	REG_CMD(GetLevItemIndexByForm)
	REG_CMD(GetLevItemIndexByLevel)
	REG_CMD(GetChanceNone)
	REG_CMD(SetChanceNone)
	REG_CMD(GetNumLevItems)
	REG_CMD(GetNthLevItem)
	REG_CMD(GetNthLevItemLevel)
	REG_CMD(GetNthLevItemCount)
	REG_CMD(RemoveNthLevItem)
	REG_CMD(LeveledListClear)
	REG_CMD(DumpLevList)
	REG_CMD(GetChanceNoneGlobal)
	REG_CMD(ClearAllHotkeys)
	//	v7.00
	REG_CMD(GetMapMarkerRep)
	REG_CMD(SetMapMarkerRep)
	REG_CMD_STR(GetMapMarkerName)
	REG_CMD(SetMapMarkerName)
	REG_CMD(GetMapMarkerType)
	REG_CMD(SetMapMarkerType)
	REG_CMD(EmptyCommand)
	REG_CMD(SetMapMarkerVisible)
	REG_CMD(GetMapMarkerHidden)
	REG_CMD(SetMapMarkerHidden)
	REG_CMD(GetMapMarkerTravel)
	REG_CMD(SetMapMarkerTravel)
	REG_CMD_ARR(GetFormMods)
	REG_CMD(IsFormOverridden)
	REG_CMD(SetPerkRank)
	REG_CMD(GetMerchantContainer)
	//	v8.00
	REG_CMD(GetRadioBroadcastType)
	REG_CMD(SetRadioBroadcastType)
	REG_CMD(GetRadioRadius)
	REG_CMD(SetRadioRadius)
	REG_CMD(GetRadioStatic)
	REG_CMD(SetRadioStatic)
	REG_CMD(GetRadioPosRef)
	REG_CMD(SetRadioPosRef)
	REG_CMD(IsRadioRef)
	REG_CMD(Search)
	//	v9.00
	REG_CMD(LNGetAggroRadius)
	REG_CMD(LNSetAggroRadius)
	REG_CMD(IsMapMarker)
	REG_CMD_ARR(GetCellCoords)
	REG_CMD(GetLockedOut)
	REG_CMD(SetLockedOut)
	REG_CMD(GetCreatureType)
	//	v10.00
	REG_CMD_ARR(GetLoadedTypeArray)
	REG_CMD(GetPasswordNote)
	REG_CMD(SetPasswordNote)
	REG_CMD_ARR(GetFormRefs)
	REG_CMD(GetCellLightingTemplate)
	REG_CMD(SetCellLightingTemplate)
	REG_CMD_ARR(GetRecipeInputForms)
	REG_CMD_ARR(GetRecipeOutputForms)
	REG_CMD(GetRecipeInputCount)
	REG_CMD(SetRecipeInputCount)
	REG_CMD(GetRecipeOutputCount)
	REG_CMD(SetRecipeOutputCount)
	REG_CMD(GetRecipeRequiredSkill)
	REG_CMD(SetRecipeRequiredSkill)
	REG_CMD(GetRecipeRequiredSkillLevel)
	REG_CMD(SetRecipeRequiredSkillLevel)
	REG_CMD(ReplaceRecipeInputForm)
	REG_CMD(ReplaceRecipeOutputForm)
	//	v11.00
	REG_CMD_ARR(GetFormRecipes)
	REG_CMD_ARR(GetFormRecipeOutputs)
	REG_CMD(GetWorldspaceParentWorldspace)
	REG_CMD(GetTerminalLock)
	REG_CMD(GetArmorClass)
	REG_CMD(IsRaceInList)
	REG_CMD(GetFormFromMod)
	REG_CMD(GetPerkEntryCount)
	REG_CMD(GetNthPerkEntryType)
	REG_CMD(GetNthPerkEntryFunction)
	REG_CMD(SetNthPerkEntryFunction)
	REG_CMD(GetNthPerkEntryForm)
	REG_CMD(SetNthPerkEntryForm)
	REG_CMD(GetNthPerkEntryValue1)
	REG_CMD(SetNthPerkEntryValue1)
	REG_CMD(GetNthPerkEntryValue2)
	REG_CMD(SetNthPerkEntryValue2)
	REG_CMD_STR(GetNthPerkEntryString)
	REG_CMD(SetNthPerkEntryString)
	REG_CMD(GetActivatorRadioStation)
	REG_CMD(SetActivatorRadioStation)
	REG_CMD_STR(GetActivatorPrompt)
	REG_CMD(SetActivatorPrompt)
	REG_CMD(GetActivatorWaterType)
	REG_CMD(SetActivatorWaterType)
	REG_CMD(GetActiveQuest)
	REG_CMD(GetINIFloat)
	REG_CMD_STR(GetINIString)
	REG_CMD(SetINIFloat)
	REG_CMD(SetINIString)
	REG_CMD_ARR(GetINISection)
	REG_CMD(SetINISection)
	REG_CMD(IsParentActivateOnly)
	REG_CMD(SetLinkedRef)
	REG_CMD(SetBaseForm)
	REG_CMD_STR(GetStringSetting)
	REG_CMD(SetStringSetting)
	REG_CMD_ARR(GetINISectionNames)
	REG_CMD(GetRaceVoice)
	REG_CMD(SetRaceVoice)
	REG_CMD(GetRaceAgeRace)
	REG_CMD(SetRaceAgeRace)
	//	v12.00
	REG_CMD_ARR(GetFilesInFolder)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(GetLeveledListFlags)
	REG_CMD(SetLeveledListFlags)
	REG_CMD(GetActivatorSoundLooping)
	REG_CMD(SetActivatorSoundLooping)
	REG_CMD(GetActivatorSoundActivate)
	REG_CMD(SetActivatorSoundActivate)
	REG_CMD(EnableButton)
	REG_CMD(DisableButton)
	REG_CMD(IsButtonDisabled)
	REG_CMD(HoldButton)
	REG_CMD(ReleaseButton)
	REG_CMD(IsButtonHeld)
	REG_CMD(IsTrait)
	REG_CMD_STR(GetLoadScreenTexture)
	REG_CMD(SetLoadScreenTexture)
	REG_CMD(GetLoadScreenType)
	REG_CMD(SetLoadScreenType)
	REG_CMD_ARR(GetLoadScreenTypeTextRGB)
	REG_CMD(SetLoadScreenTypeTextRGB)
	REG_CMD(CopyFaceGenFrom)
	//	v13.00
	REG_CMD(SetRace)
	REG_CMD_ARR(GetFoldersInFolder)
	REG_CMD(EnableTrigger)
	REG_CMD(DisableTrigger)
	REG_CMD(IsTriggerDisabled)
	REG_CMD(HoldTrigger)
	REG_CMD(ReleaseTrigger)
	REG_CMD(IsTriggerHeld)
	REG_CMD(GetPCCanUsePowerArmor)
	REG_CMD_STR(GetIdleAnimPath)
	REG_CMD(SetIdleAnimPath)
	REG_CMD_STR(LNGetName)
	REG_CMD(RemoveINIKey)
	REG_CMD(RemoveINISection)
	//	v14.00
	REG_CMD(GetGlobalTimeMultiplier)
	REG_CMD(GetCasinoMaxWinnings)
	REG_CMD(SetCasinoMaxWinnings)
	REG_CMD(GetCasinoBJPayout)
	REG_CMD(SetCasinoBJPayout)
	REG_CMD(GetCasinoBJ17Stand)
	REG_CMD(SetCasinoBJ17Stand)
	REG_CMD(GetCasinoNumDecks)
	REG_CMD(SetCasinoNumDecks)
	REG_CMD(GetCasinoShufflePercent)
	REG_CMD(SetCasinoShufflePercent)
	REG_CMD(GetCasinoReelStops)
	REG_CMD(SetCasinoReelStops)
	REG_CMD(AddNoteNS)
	//	v15.00
	REG_CMD(GetCellWaterForm)
	REG_CMD(GetWaterFormEffect)
	REG_CMD(ar_Cat)
	REG_CMD(GetInventoryWeight)
	REG_CMD(GetChallengeType)
	REG_CMD(GetChallengeThreshold)
	REG_CMD(SetChallengeThreshold)
	REG_CMD(GetChallengeInterval)
	REG_CMD(SetChallengeInterval)
	REG_CMD(GetChallengeValue1)
	REG_CMD(SetChallengeValue1)
	REG_CMD(GetChallengeValue2)
	REG_CMD(SetChallengeValue2)
	REG_CMD(GetChallengeValue3)
	REG_CMD(SetChallengeValue3)
	REG_CMD(GetChallengeForm1)
	REG_CMD(SetChallengeForm1)
	REG_CMD(GetChallengeForm2)
	REG_CMD(SetChallengeForm2)
	REG_CMD(GetRecipeCategory)
	REG_CMD(SetRecipeCategory)
	REG_CMD(GetRecipeSubcategory)
	REG_CMD(SetRecipeSubcategory)
	REG_CMD(EmptyCommand)
	REG_CMD(GetReferenceFlag)
	REG_CMD(SetReferenceFlag)
	REG_CMD(AddRecipeInputForm)
	REG_CMD(AddRecipeOutputForm)
	REG_CMD(RemoveRecipeInputForm)
	REG_CMD(RemoveRecipeOutputForm)
	REG_CMD(AddRecipeCondition)
	REG_CMD(EmptyCommand)
	REG_CMD(GetFormFlag)
	REG_CMD(SetFormFlag)
	REG_CMD(GetDistance2D)
	REG_CMD(GetDistance3D)
	REG_CMD(GetIngestibleFlag)
	REG_CMD(SetIngestibleFlag)
	REG_CMD(IsSkillMagazine)
	REG_CMD(EmptyCommand)
	REG_CMD(GetServiceFlag)
	REG_CMD(SetServiceFlag)
	REG_CMD(LeveledListReplaceForm)
	REG_CMD(EmptyCommand)
	//	v16.00
	REG_CMD(GetCellImageSpace)
	REG_CMD(PlayerHasKey)
	REG_CMD(GetCellFlag)
	REG_CMD(SetCellFlag)
	REG_CMD(GetENBFloat)
	REG_CMD(SetENBFloat)
	REG_CMD(fsqrt)
	REG_CMD(GetCellWaterHeight)
	REG_CMD(SetCellWaterHeight)
	REG_CMD(GetAnimObjectIdle)
	REG_CMD(SetAnimObjectIdle)
	REG_CMD(GetActorTemplateFlag)
	REG_CMD(SetActorTemplateFlag)
	REG_CMD(BaseGetItemCount)
	REG_CMD(BaseAddItem)
	REG_CMD(BaseRemoveItem)
	REG_CMD(LNSetActorTemplate)
	REG_CMD(BaseAddItemHealth)
	REG_CMD(LNSetName)
	//	v17.00
	REG_CMD(fSin)
	REG_CMD(fCos)
	REG_CMD_ARR(SortFormsByType)
	REG_CMD(GetFormCountType)
	REG_CMD(GetPickupSound)
	REG_CMD(SetPickupSound)
	REG_CMD(GetPutdownSound)
	REG_CMD(SetPutdownSound)
	REG_CMD(GetContainerOpenSound)
	REG_CMD(SetContainerOpenSound)
	REG_CMD(GetContainerCloseSound)
	REG_CMD(SetContainerCloseSound)
	REG_CMD(EmptyCommand)
	REG_CMD_ARR(GetPlayerRegions)
	REG_CMD(fAtan)
	REG_CMD(GetPerkLevel)
	//	v18.00
	REG_CMD(GetZoneFlag)
	REG_CMD(SetZoneFlag)
	REG_CMD(fAsin)
	REG_CMD(fAcos)
	REG_CMD(SetNthLevItem)
	REG_CMD(SetNthLevItemLevel)
	REG_CMD(SetNthLevItemCount)
	REG_CMD(GetBaseFactionRank)
	REG_CMD(SetBaseFactionRank)
	REG_CMD(GetImpactDataSet)
	REG_CMD(SetImpactDataSet)
	REG_CMD(HasActionRef)
	REG_CMD(SetHotkey)
	REG_CMD(GetChallengeFlags)
	REG_CMD(SetChallengeFlags)
	REG_CMD(LNIsPlayable)
	REG_CMD(LNSetIsPlayable)
	REG_CMD(GetZone)
	REG_CMD(GetBaseKarma)
	REG_CMD(SetBaseKarma)
	REG_CMD(ActorHasEffect)
	REG_CMD(EmptyCommand)
	REG_CMD_ARR(GetEquippedData)
	REG_CMD(SetEquippedData)
	REG_CMD(EquipItemData)
	REG_CMD(Console)
	REG_CMD(GetDefaultMessageTime)
	REG_CMD(SetDefaultMessageTime)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(GetWeaponKillImpulse)
	REG_CMD(SetWeaponKillImpulse)
	REG_CMD(GetWeaponImpulseDistance)
	REG_CMD(SetWeaponImpulseDistance)
	REG_CMD(GetWeaponVATSEffect)
	REG_CMD(SetWeaponVATSEffect)
	REG_CMD(GetWeaponCritFlags)
	REG_CMD(SetWeaponCritFlags)
	REG_CMD(GetBodyPartData)
	//	v19.00
	REG_CMD(GetRadius)
	REG_CMD(SetRadius)
	REG_CMD(CrosshairRefInList)
	REG_CMD_ARR(GetGameSettings)
	REG_CMD(GetShowQuestItems)
	REG_CMD(GetWaterTrait)
	REG_CMD(SetWaterTrait)

#if JIP_DEBUG
	RegisterDebugFunctions(nvse);
#endif

	//____________________FUNCTIONS_JIP____________________
	nvse->SetOpcodeBase(0x2600);
	//	v1.00
	REG_CMD(SetPersistent)
	REG_CMD(GetActorTemplate)
	REG_CMD(GetLeveledActorBase)
	REG_CMD(GetCreatureDamage)
	REG_CMD(SetCreatureDamage)
	REG_CMD(GetIsPoisoned)
	REG_CMD(AddItemAlt)
	REG_CMD(GetValueAlt)
	REG_CMD(IsComponentLoaded)
	REG_CMD(InjectUIXML)
	REG_CMD(InjectUIComponent)
	REG_CMD(GetAmmoTraitNumeric)
	REG_CMD(SetAmmoTraitNumeric)
	REG_CMD(GetAmmoProjectile)
	REG_CMD(SetAmmoProjectile)
	REG_CMD(GetNumAmmoEffects)
	REG_CMD(GetNthAmmoEffect)
	REG_CMD(AddAmmoEffect)
	REG_CMD(RemoveAmmoEffect)
	REG_CMD(GetAmmoEffectTraitNumeric)
	REG_CMD(SetAmmoEffectTraitNumeric)
	REG_CMD(GetProjectileTraitNumeric)
	REG_CMD(SetProjectileTraitNumeric)
	REG_CMD(GetProjectileFlag)
	REG_CMD(SetProjectileFlag)
	REG_CMD(GetProjectileExplosion)
	REG_CMD(SetProjectileExplosion)
	REG_CMD(GetExplosionTraitNumeric)
	REG_CMD(SetExplosionTraitNumeric)
	REG_CMD(GetExplosionTraitForm)
	REG_CMD(SetExplosionTraitForm)
	REG_CMD(GetExplosionFlag)
	REG_CMD(SetExplosionFlag)
	REG_CMD(AddScriptVariable)
	REG_CMD(GetDelayElapsed)
	REG_CMD(SetDelayElapsed)
	REG_CMD(GetPackageFlag)
	REG_CMD(SetPackageFlag)
	REG_CMD(GetGlobalVariable)
	REG_CMD(SetGlobalVariable)
	REG_CMD(GetFactionReputationType)
	REG_CMD(SetFactionReputationType)
	REG_CMD_STR(RefToString)
	REG_CMD(StringToRef)
	REG_CMD(GetObjectDimensions)
	REG_CMD(GetIsItem)
	REG_CMD(GetMinOf)
	REG_CMD(GetMaxOf)
	//	v2.00
	REG_CMD(GetCombatStyleTraitNumeric)
	REG_CMD(SetCombatStyleTraitNumeric)
	REG_CMD(GetCombatStyleFlag)
	REG_CMD(SetCombatStyleFlag)
	REG_CMD(SetLinkedReference)
	REG_CMD_ARR(GetEnableChildren)
	REG_CMD_ARR(GetLinkedChildren)
	REG_CMD_ARR(GetFollowers)
	REG_CMD(GetCursorPos)
	REG_CMD_STR(GetUIString)
	REG_CMD_ARR(ReadArrayFromFile)
	REG_CMD_STR(ReadStringFromFile)
	//	v3.00
	REG_CMD(WriteArrayToFile)
	REG_CMD(WriteStringToFile)
	REG_CMD(GetClassTraitNumeric)
	REG_CMD(SetClassTraitNumeric)
	REG_CMD(GetClassFlag)
	REG_CMD(SetClassFlag)
	REG_CMD(GetLightTraitNumeric)
	REG_CMD(SetLightTraitNumeric)
	REG_CMD(GetLightFlag)
	REG_CMD(SetLightFlag)
	REG_CMD(GetSoundTraitNumeric)
	REG_CMD(SetSoundTraitNumeric)
	REG_CMD(GetSoundFlag)
	REG_CMD(SetSoundFlag)
	//	v4.00
	REG_CMD(GetWeaponDetectionSoundLevel)
	REG_CMD(SetWeaponDetectionSoundLevel)
	REG_CMD(IsEquippedWeaponSilenced)
	REG_CMD(IsEquippedWeaponScoped)
	REG_CMD(GetWeaponSound)
	REG_CMD(SetWeaponSound)
	REG_CMD(SetWeaponItemMod)
	REG_CMD(SetWeaponItemModEffect)
	REG_CMD(SetWeaponItemModValue)
	REG_CMD(GetWeatherImageSpaceMod)
	REG_CMD(SetWeatherImageSpaceMod)
	REG_CMD_STR(GetWeatherTexture)
	REG_CMD(SetWeatherTexture)
	REG_CMD_STR(GetWeatherPrecipitationModel)
	REG_CMD(SetWeatherPrecipitationModel)
	REG_CMD(GetWeatherTraitNumeric)
	REG_CMD(SetWeatherTraitNumeric)
	REG_CMD(GetWeatherRGBColor)
	REG_CMD(SetWeatherRGBColor)
	REG_CMD(GetLandTextureTextureSet)
	REG_CMD(SetLandTextureTextureSet)
	REG_CMD(GetLandTextureTraitNumeric)
	REG_CMD(SetLandTextureTraitNumeric)
	REG_CMD(GetLandTextureNumGrasses)
	REG_CMD(GetLandTextureNthGrass)
	REG_CMD(LandTextureAddGrass)
	REG_CMD(LandTextureRemoveGrass)
	REG_CMD_STR(GetTextureSetTexture)
	REG_CMD(SetTextureSetTexture)
	REG_CMD(GetTextureSetTraitNumeric)
	REG_CMD(SetTextureSetTraitNumeric)
	REG_CMD(GetTextureSetFlag)
	REG_CMD(SetTextureSetFlag)
	REG_CMD_STR(GetClimateSunTexture)
	REG_CMD(SetClimateSunTexture)
	REG_CMD_STR(GetClimateSunGlareTexture)
	REG_CMD(SetClimateSunGlareTexture)
	REG_CMD_STR(GetClimateNightSkyModel)
	REG_CMD(SetClimateNightSkyModel)
	REG_CMD(GetClimateTraitNumeric)
	REG_CMD(SetClimateTraitNumeric)
	REG_CMD(GetClimateNumWeatherTypes)
	REG_CMD(GetClimateNthWeatherType)
	REG_CMD(GetClimateNthWeatherChance)
	REG_CMD(GetClimateNthWeatherGlobal)
	REG_CMD(ClimateAddWeatherType)
	REG_CMD(ClimateRemoveWeatherType)
	//	v5.00
	REG_CMD(GetPrimitiveBound)
	REG_CMD(SetPrimitiveBound)
	REG_CMD(GetNumEffects)
	REG_CMD(GetNthEffectBase)
	REG_CMD(GetNthEffectTraitNumeric)
	REG_CMD(SetNthEffectTraitNumeric)
	REG_CMD(GetNumActorEffects)
	REG_CMD(GetNthActorEffect)
	//	v6.00
	REG_CMD(GetActiveMenuMode)
	REG_CMD_STR(GetActiveUIComponentName)
	REG_CMD_STR(GetActiveUIComponentFullName)
	REG_CMD(GetActiveUIComponentID)
	REG_CMD(GetMenuTargetRef)
	REG_CMD(GetMenuItemFilter)
	REG_CMD(ClickMenuButton)
	//	v7.00
	REG_CMD(RemoveItemTarget)
	REG_CMD(GetActorLevelingData)
	REG_CMD(SetActorLevelingData)
	//	v8.00
	REG_CMD(SetNthEffectBase)
	REG_CMD(HasVariableAdded)
	REG_CMD(RemoveScriptVariable)
	REG_CMD(GetLoadOrderChanged)
	REG_CMD(ValidateModIndex)
	REG_CMD(RemoveAllAddedVariables)
	//	v9.00
	REG_CMD(AuxiliaryVariableGetSize)
	REG_CMD(AuxiliaryVariableGetType)
	REG_CMD(AuxiliaryVariableGetFloat)
	REG_CMD(AuxiliaryVariableSetFloat)
	REG_CMD(AuxiliaryVariableGetRef)
	REG_CMD(AuxiliaryVariableSetRef)
	REG_CMD_STR(AuxiliaryVariableGetString)
	REG_CMD(AuxiliaryVariableSetString)
	REG_CMD_ARR(AuxiliaryVariableGetAll)
	REG_CMD(AuxiliaryVariableErase)
	REG_CMD(AuxiliaryVariableEraseAll)
	REG_CMD(RefMapArrayGetSize)
	REG_CMD(RefMapArrayGetType)
	REG_CMD_ARR(RefMapArrayGetFirst)
	REG_CMD_ARR(RefMapArrayGetNext)
	REG_CMD_ARR(RefMapArrayGetKeys)
	REG_CMD_ARR(RefMapArrayGetAll)
	REG_CMD(RefMapArrayErase)
	REG_CMD(RefMapArrayDestroy)
	REG_CMD(RefMapArrayGetFloat)
	REG_CMD(RefMapArraySetFloat)
	REG_CMD(RefMapArrayGetRef)
	REG_CMD(RefMapArraySetRef)
	REG_CMD_STR(RefMapArrayGetString)
	REG_CMD(RefMapArraySetString)
	REG_CMD(ClearJIPSavedData)
	//	v9.10
	REG_CMD(SetValueAlt)
	//	v10.00
	REG_CMD_STR(GetBodyPartDataSkeleton)
	REG_CMD(SetBodyPartDataSkeleton)
	REG_CMD(GetBodyPartDataRagdoll)
	REG_CMD(SetBodyPartDataRagdoll)
	REG_CMD(GetBodyPartDataHasPart)
	REG_CMD(GetBodyPartTraitNumeric)
	REG_CMD(SetBodyPartTraitNumeric)
	REG_CMD(GetBodyPartTraitForm)
	REG_CMD(SetBodyPartTraitForm)
	REG_CMD_STR(GetBodyPartName)
	REG_CMD(SetBodyPartName)
	REG_CMD_STR(GetBodyPartReplacementModel)
	REG_CMD(SetBodyPartReplacementModel)
	REG_CMD(GetBodyPartFlag)
	REG_CMD(SetBodyPartFlag)
	REG_CMD(GetWeaponModReloadAnim)
	REG_CMD(SetWeaponModReloadAnim)
	//	v11.00
	REG_CMD_ARR(AuxiliaryVariableGetAsArray)
	REG_CMD(AuxiliaryVariableSetFromArray)
	REG_CMD_STR(GetSoundSourceFile)
	REG_CMD(SetSoundSourceFile)
	REG_CMD(GetDestructionDataHealth)
	REG_CMD(SetDestructionDataHealth)
	REG_CMD(GetDestructionDataTargetable)
	REG_CMD(SetDestructionDataTargetable)
	REG_CMD(GetDestructionDataNumStages)
	REG_CMD(GetNthDestructionStageTrait)
	REG_CMD(SetNthDestructionStageTrait)
	REG_CMD(GetNthDestructionStageExplosion)
	REG_CMD(SetNthDestructionStageExplosion)
	REG_CMD(GetNthDestructionStageDebris)
	REG_CMD(SetNthDestructionStageDebris)
	REG_CMD_STR(GetNthDestructionStageReplacement)
	REG_CMD(SetNthDestructionStageReplacement)
	//	v12.00
	REG_CMD_ARR(GetTeammates)
	REG_CMD(GetCurrentWeather)
	REG_CMD(GetCurrentClimate)
	REG_CMD(SetCurrentClimate)
	REG_CMD(RefreshCurrentClimate)
	REG_CMD(GetWorldspaceClimate)
	REG_CMD(SetWorldspaceClimate)
	REG_CMD(GetActorVoiceType)
	REG_CMD(SetActorVoiceType)
	//	v13.00
	REG_CMD(GetCreatureReach)
	REG_CMD(GetIsImmobile)
	REG_CMD(PickFromList)
	REG_CMD(SetStageAlt)
	REG_CMD(EmptyCommand)
	REG_CMD(GetProjectileRefSource)
	REG_CMD(SetProjectileRefSource)
	REG_CMD(GetProjectileRefWeapon)
	REG_CMD(SetProjectileRefWeapon)
	REG_CMD(GetProjectileRefLifeTime)
	REG_CMD(GetProjectileRefDistanceTraveled)
	REG_CMD(GetProjectileRefDamage)
	REG_CMD(SetProjectileRefDamage)
	REG_CMD(GetProjectileRefSpeedMult)
	REG_CMD(SetProjectileRefSpeedMult)
	REG_CMD(EmptyCommand)
	REG_CMD(GetWorldspaceFlag)
	REG_CMD(SetWorldspaceFlag)
	REG_CMD(GetIdleLoopTimes)
	REG_CMD(ToggleCreatureModel)
	REG_CMD(CreatureHasModel)
	REG_CMD_ARR(GetCreatureModels)
	REG_CMD(DisableNavMeshAlt)
	REG_CMD(EnableNavMeshAlt)
	REG_CMD(GetTerrainHeight)
	//	v14.00
	REG_CMD(RefMapArrayValidate)
	REG_CMD(HasPrimitive)
	REG_CMD(AddPrimitive)
	REG_CMD(AddDestructionStage)
	REG_CMD(RemoveDestructionStage)
	REG_CMD_STR(GetWeaponShellCasingModel)
	REG_CMD(SetWeaponShellCasingModel)
	REG_CMD(AddNewEffect)
	REG_CMD(RemoveNthEffect)
	REG_CMD(SetObjectEffect)
	REG_CMD(GetActorUnarmedEffect)
	REG_CMD(SetActorUnarmedEffect)
	//	v15.00
	REG_CMD(IsPCInCombat)
	REG_CMD(CCCOnLoad)
	REG_CMD(CCCSetFloat)
	REG_CMD(CCCSetString)
	REG_CMD(CCCSetTrait)
	REG_CMD(CCCGetDistance)
	REG_CMD(CCCTaskPackageFlags)
	REG_CMD(CCCSetEquipped)
	REG_CMD(CCCInFaction)
	REG_CMD(CCCSetNCCS)
	REG_CMD(GetEncumbranceRate)
	REG_CMD(CCCLoadNCCS)
	REG_CMD(CCCSavedForm)
	REG_CMD_STR(CCCLocationName)
	REG_CMD(CCCGetReputation)
	//	v16.00
	REG_CMD(GetSelectedItemRef)
	REG_CMD(GetWeaponRefModFlags)
	REG_CMD(SetWeaponRefModFlags)
	REG_CMD_ARR(GetBarterItems)
	REG_CMD(GetItemRefCurrentHealth)
	REG_CMD(SetItemRefCurrentHealth)
	REG_CMD(GetBarterGoldAlt)
	//	v17.00
	REG_CMD(GetRecipeMenuSelection)
	REG_CMD(GetRecipeMenuCategory)
	REG_CMD(UnlockRecipeMenuQuantity)
	//	v18.00
	REG_CMD(GetRecipeMenuNumSubcategories)
	//	v19.00
	REG_CMD(SetHotkeyItemRef)
	REG_CMD(EquipItemAlt)
	REG_CMD(UnequipItemAlt)
	REG_CMD(DropAlt)
	REG_CMD(DropMeAlt)
	REG_CMD_STR(GetFormDescription)
	REG_CMD(GetAddictionEffect)
	REG_CMD(SetAddictionEffect)
	REG_CMD(GetAddictionChance)
	REG_CMD(SetAddictionChance)
	REG_CMD(GetIsPoison)
	REG_CMD(GetContainerRespawns)
	REG_CMD(SetContainerRespawns)
	REG_CMD(ModBaseActorValue)
	REG_CMD(GetGrassTraitNumeric)
	REG_CMD(SetGrassTraitNumeric)
	REG_CMD_STR(GetGrassModel)
	REG_CMD(SetGrassModel)
	REG_CMD(GetWheelDisabled)
	REG_CMD(SetWheelDisabled)
	REG_CMD(GetInterruptPackage)
	REG_CMD(EnableImprovedRecipeMenu)
	REG_CMD(InitItemFilter)
	REG_CMD(SetItemFilter)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	//	v20.00
	REG_CMD_ARR(GetActiveEffects)
	REG_CMD(GetActorProcessingLevel)
	REG_CMD_ARR(GetActorsByProcessingLevel)
	REG_CMD(GetExteriorCell)
	REG_CMD(GetCellBuffered)
	REG_CMD(MessageExAlt)
	//	v21.00
	REG_CMD_ARR(GetTempEffects)
	REG_CMD(RemoveNthTempEffect)
	REG_CMD(GetActorLightAmount)
	REG_CMD(GetActorAlpha)
	REG_CMD(GetActorDiveBreath)
	REG_CMD(SetActorDiveBreath)
	REG_CMD(GetGameDifficulty)
	REG_CMD(ToggleFirstPerson)
	REG_CMD(StopIdle)
	REG_CMD_ARR(GetCombatTargets)
	REG_CMD_ARR(GetCombatAllies)
	REG_CMD_ARR(GetDetectedActors)
	REG_CMD_ARR(GetDetectingActors)
	REG_CMD(ClearActiveQuest)
	REG_CMD(SetFormDescription)
	REG_CMD(GetPCFastTravelled)
	REG_CMD(GetPCMovedCell)
	//	v22.00
	REG_CMD(GetPCDetectionState)
	REG_CMD_STR(GetFontFile)
	REG_CMD(SetFontFile)
	//	v23.00
	REG_CMD(ShowTextInputMenu)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(EmptyCommand)
	REG_CMD(GetPipboyRadio)
	REG_CMD(CCCSayTo)
	REG_CMD(CCCRunResultScripts)
	//	v24.00
	REG_CMD(GetCombatDisabled)
	REG_CMD(SetCombatDisabled)
	REG_CMD(ToggleNoHealthReset)
	REG_CMD(GetCurrentStablePackage)
	REG_CMD(MoveToCell)
	REG_CMD(MoveToEditorPosition)
	REG_CMD(GetTeammateUsingAmmo)
	REG_CMD(SetTeammateUsingAmmo)
	REG_CMD(ToggleDetectionFix)
	REG_CMD(ClearModNVSEVars)
	REG_CMD(EmptyCommand)
	REG_CMD(ToggleIgnoreLockedDoors)
	REG_CMD(CCCSetFollowState)
	//	v25.00
	REG_CMD(IsInCombatWith)
	REG_CMD_STR(RefToPosStr)
	REG_CMD(MoveToPosStr)
	REG_CMD_ARR(RefMapArrayGetValue)
	REG_CMD(RefMapArraySetValue)
	REG_CMD_ARR(GetAllItems)
	REG_CMD_ARR(GetAllItemRefs)
	REG_CMD(LockEquipment)
	REG_CMD(IsAttacking)
	REG_CMD(GetPCUsingScope)
	REG_CMD(HolsterWeapon)
	REG_CMD(GetFallTimeElapsed)
	REG_CMD(GetFallTimeRemaining)
	REG_CMD(ResetFallTime)
	REG_CMD(GetCenterPos)
	//	v26.00
	REG_CMD(SetIdleLoopTimes)
	REG_CMD(CCCSMS)
	REG_CMD(GetDetonationTimer)
	REG_CMD(SetDetonationTimer)
	REG_CMD(GetMineArmed)
	REG_CMD(GetRefType)
	REG_CMD(GetEffectFlag)
	REG_CMD(SetEffectFlag)
	REG_CMD(GetBaseEffectFlag)
	REG_CMD(SetBaseEffectFlag)
	//	v27.00
	REG_CMD(GetPCUsingIronSights)
	REG_CMD(GetPCLastExteriorDoor)
	REG_CMD(GetRadiationLevelAlt)
	REG_CMD(IsInWater)
	REG_CMD(SetEmbeddedWeaponNode)
	REG_CMD(SetEmbeddedWeaponAV)
	REG_CMD_ARR(GetDroppedRefs)
	//	v28.00
	REG_CMD(MoveAwayFromPlayer)
	REG_CMD(FaceObject)
	REG_CMD(Turn)
	REG_CMD(SetMessageDisabled)
	REG_CMD(GetMessageDisabled)
	//	v29.00
	REG_CMD(GetAshPileSource)
	//	v30.00
	REG_CMD(GetMessageFlags)
	REG_CMD(SetMessageFlags)
	REG_CMD(ToggleObjectCollision)
	REG_CMD(IsSoundPlaying)
	REG_CMD(SwapTextureEx)
	REG_CMD(GetMaterialPropertyValue)
	REG_CMD(SetMaterialPropertyValue)
	REG_CMD_ARR(GetSoundPlayers)
	//	v31.00
	REG_CMD(SetMessageDisplayTime)
	REG_CMD_STR(GetImpactDataModel)
	REG_CMD(SetImpactDataModel)
	REG_CMD(GetImpactDataTrait)
	REG_CMD(SetImpactDataTraitNumeric)
	REG_CMD(SetImpactDataTraitForm)
	//	v32.00
	REG_CMD(sv_RegexMatch)
	REG_CMD_ARR(sv_RegexSearch)
	REG_CMD_STR(sv_RegexReplace)
	//	v33.00
	REG_CMD(SetOnMenuClickEventHandler)
	REG_CMD(SetOnFastTravelEventHandler)
	REG_CMD(SetOnMenuOpenEventHandler)
	REG_CMD(SetOnMenuCloseEventHandler)
	REG_CMD(SetOnAnimActionEventHandler)
	REG_CMD(SetOnPlayGroupEventHandler)
	REG_CMD(SetOnHealthDamageEventHandler)
	REG_CMD(SetOnCrippledLimbEventHandler)
	REG_CMD(SetOnFireWeaponEventHandler)
	REG_CMD(GetCurrentAmmo)
	REG_CMD(GetCurrentAmmoRounds)
	//	v34.00
	REG_CMD_STR(GetMoonTexture)
	REG_CMD(SetMoonTexture)
	REG_CMD(SetFullNameAlt)
	REG_CMD(SetWeatherTransitionTimeOverride)
	//	v35.00
	REG_CMD(GetDetectionValue)
	REG_CMD(GetBaseActorValueAlt)
	REG_CMD(SetOnMouseoverChangeEventHandler)
	REG_CMD(GetAlwaysRun)
	REG_CMD(GetAutoMove)
	REG_CMD(SetSpeedMult)
	REG_CMD(GetIsRagdolled)
	REG_CMD(ForceActorDetectionValue)
	REG_CMD(SetOnPCTargetChangeEventHandler)
	REG_CMD(FreePlayer)
	//	v36.00
	REG_CMD(AuxVarGetFltCond)
	REG_CMD(GetLocalGravity)
	REG_CMD(SetLocalGravityVector)
	REG_CMD(GetActorVelocity)
	REG_CMD(IsInAir)
	REG_CMD(GetHasContact)
	REG_CMD_ARR(GetContactRefs)
	//	v36.50
	REG_CMD(GetHasContactBase)
	REG_CMD(GetHasContactType)
	REG_CMD(GetObjectUnderFeet)
	//	v40.00
	REG_CMD(GetHasPhantom)
	REG_CMD_STR(GetArmorRCT)
	REG_CMD(SetArmorRCT)
	REG_CMD(GetPerkFlag)
	REG_CMD(SetPerkFlag)
	REG_CMD(ModLogPrint)
	//	v41.00
	REG_CMD(GetBaseEffectScript)
	REG_CMD(SetBaseEffectScript)
	REG_CMD_STR(GetWaterNoiseTexture)
	REG_CMD(SetWaterNoiseTexture)
	REG_CMD(SetWaterFormEffect)
	REG_CMD(GetWaterSound)
	REG_CMD(SetWaterSound)
	//	v42.00
	REG_CMD(GetInteractionDisabled)
	REG_CMD(SetInteractionDisabled)
	REG_CMD(GetInteractionDisabledType)
	REG_CMD(SetInteractionDisabledType)
	//	v43.00
	REG_CMD(GetImageSpaceTrait)
	REG_CMD(SetImageSpaceTrait)
	REG_CMD(GetImageSpaceModTrait)
	REG_CMD(SetImageSpaceModTrait)
	REG_CMD(SetOnKeyDownEventHandler)
	REG_CMD(SetOnKeyUpEventHandler)
	//	v43.50
	REG_CMD(SetOnControlDownEventHandler)
	REG_CMD(SetOnControlUpEventHandler)
	//	v44.00
	REG_CMD(SetGameMainLoopCallback)
	REG_CMD(GetGlobalRef)
	REG_CMD(SetGlobalRef)
	REG_CMD_STR(GetWorldspaceNoiseTexture)
	REG_CMD(SetWorldspaceNoiseTexture)
	REG_CMD(GetWorldspaceWaterType)
	REG_CMD(SetWorldspaceWaterType)
	REG_CMD(GetWorldspaceImagespace)
	REG_CMD(SetWorldspaceImagespace)
	//	v45.00
	REG_CMD(SetCellWaterForm)
	REG_CMD(GetCellClimate)
	REG_CMD(SetCellClimate)
	REG_CMD_STR(GetCellNoiseTexture)
	REG_CMD(SetCellNoiseTexture)
	//	v46.00
	REG_CMD(RemoveMeIRAlt)
	REG_CMD(GetScriptDisabled)
	REG_CMD(SetScriptDisabled)
	REG_CMD(GetScriptEventDisabled)
	REG_CMD(SetScriptEventDisabled)
	REG_CMD(FakeScriptEvent)
	REG_CMD(RefreshItemsList)
	//	v46.50
	REG_CMD(AddRefMapMarker)
	REG_CMD(RemoveRefMapMarker)
	REG_CMD(RefHasMapMarker)
	REG_CMD(IsSpellTargetAlt)
	//	v47.00
	REG_CMD(GetCalculatedWeaponDamage)
	REG_CMD(GetOptionalPatch)
	REG_CMD(SetOptionalPatch)
	//	v47.50
	REG_CMD(SetPosEx)
	REG_CMD_ARR(GetReticlePos)
	REG_CMD(GetReticleRange)
	REG_CMD(MoveToReticle)
	//	v48.00
	REG_CMD(SetRefName)
	REG_CMD(GetActorValueModifier)
	REG_CMD(GetPerkModifier)
	REG_CMD(SetOnQuestStageEventHandler)
	REG_CMD(SetAngleEx)
	REG_CMD(GetTeleportDoor)
	REG_CMD(GetBarterPriceMult)
	REG_CMD(SetBarterPriceMult)
	//	v48.80
	REG_CMD(SetOnDialogTopicEventHandler)
	//	v49.00
	REG_CMD(TapButton)
	REG_CMD(TapTrigger)
	REG_CMD(SetOnTriggerDownEventHandler)
	REG_CMD(SetOnTriggerUpEventHandler)
	//	v49.20
	REG_CMD(SetOnCriticalHitEventHandler)
	REG_CMD(GetHitHealthDamage)
	REG_CMD(GetHitLimbDamage)
	REG_CMD(CrippleLimb)
	//	v50.00
	REG_CMD(HoldControl)
	REG_CMD(ReleaseControl)
	REG_CMD(PlayIdleEx)
	REG_CMD(SetBipedModelPathAlt)
	REG_CMD(GetKillXP)
	REG_CMD(sv_DestructAlt)
	REG_CMD(GetKiller)
	REG_CMD(KillActorAlt)
	REG_CMD(ReloadEquippedModels)
	REG_CMD(MoveToFadeDelay)
	REG_CMD(RunScriptSnippet)
	REG_CMD(ScriptWait)
	REG_CMD(IsScriptWaiting)
	REG_CMD(StopScriptWaiting)
	REG_CMD(GetCrosshairWater)
	//	v50.50
	REG_CMD(GetScriptBlockDisabled)
	REG_CMD(SetScriptBlockDisabled)
	REG_CMD(GetPlayedIdle)
	REG_CMD(IsIdlePlayingEx)
	REG_CMD_ARR(GetObjectiveTargets)
	REG_CMD(SetObjectiveNthTarget)
	REG_CMD(GetObjectiveHasTarget)
	REG_CMD(AddObjectiveTarget)
	REG_CMD(RemoveObjectiveTarget)
	REG_CMD_STR(GetObjectiveText)
	REG_CMD(SetObjectiveText)
	//	v50.80
	REG_CMD(GetArmorAudioTemplate)
	REG_CMD(SetArmorAudioTemplate)
	REG_CMD(SetWeaponOut)
	//	v51.00
	REG_CMD(SetTerminalUIModel)
	REG_CMD(AddBaseEffectListEffect)
	REG_CMD(RemoveBaseEffectListEffect)
	REG_CMD(ShowQuantityMenu)
	//	v51.20
	REG_CMD(MessageBoxExAlt)
	REG_CMD_ARR(GetVATSTargets)
	REG_CMD(HasScriptBlock)
	//	v51.40
	REG_CMD(CastImmediate)
	REG_CMD(IsInCharGen)
	REG_CMD(DisableScriptedActivate)
	//	v51.60
	REG_CMD(GetHitAttacker)
	REG_CMD(GetHitProjectile)
	REG_CMD(GetHitWeapon)
	REG_CMD(GetGameDaysPassed)
	//	v51.80
	REG_CMD(GetWaterImmersionPerc)
	REG_CMD_STR(GetProjectileMuzzleFlash)
	REG_CMD(SetProjectileMuzzleFlash)
	REG_CMD(DismemberLimb)
	//	v52.00
	REG_CMD(RunBatchScript)
	//	v52.20
	REG_CMD(IsArmorAddon)
	//	v52.50
	REG_CMD(ToggleCraftingMessages)
	REG_CMD(ToggleHardcoreTracking)
	REG_CMD(SetGameDifficulty)
	REG_CMD(GetEquippedItemRef)
	REG_CMD(ExecuteScript)
	REG_CMD(ReloadENB)
	REG_CMD(IsFleeing)
	REG_CMD(GetEnemyHealthTarget)
	REG_CMD(SetCurrentAmmoRounds)
	REG_CMD(IsAnimPlayingEx)
	//	v52.60
	REG_CMD(AttachAshPileEx)
	REG_CMD(MarkActivatorAshPile)
	//	v52.70
	REG_CMD(SetActorVelocity)
	REG_CMD(GetRigidBodyMass)
	REG_CMD(PushObject)
	//	v53.00
	REG_CMD_STR(GetSkeletonModel)
	REG_CMD(SetNPCSkeletonModel)
	REG_CMD(SetCRESkeletonModel)
	REG_CMD(SetBodyPartData)
	REG_CMD(GetTargetUnreachable)
	REG_CMD(MoveToContainer)
	REG_CMD_ARR(GetNifBlockTranslation)
	REG_CMD(SetNifBlockTranslation)
	REG_CMD_ARR(GetNifBlockRotation)
	REG_CMD(SetNifBlockRotation)
	REG_CMD(GetNifBlockScale)
	REG_CMD(SetNifBlockScale)
	REG_CMD(GetNifBlockFlag)
	REG_CMD(SetNifBlockFlag)
	REG_CMD_ARR(GetBufferedCells)
	//	v53.20
	REG_CMD(SetVATSTargetable)
	REG_CMD(GetWindDirection)
	REG_CMD(SetWindDirection)
	REG_CMD(GetCreatureWeaponList)
	REG_CMD(SetWindSpeedMult)
	REG_CMD(TriggerLightningFX)
	REG_CMD(GetObjectVelocity)
	REG_CMD(GetAngularVelocity)
	REG_CMD(SetAngularVelocity)
	REG_CMD(PlaceAtCell)
	//	v53.40
	REG_CMD(GetRayCastPos)
	REG_CMD(GetAnimSequenceFrequency)
	REG_CMD(SetAnimSequenceFrequency)
	REG_CMD(StopSound)
	REG_CMD(IsMusicPlaying)
	REG_CMD(SetMusicState)
	//	v53.60
	REG_CMD(GetNoUnequip)
	REG_CMD(SetNoUnequip)
	REG_CMD(SetCursorPos)
	REG_CMD(MoveToNode)
	REG_CMD(GetDeathItem)
	REG_CMD(SetDeathItem)
	REG_CMD(GetActorLeveledList)
	REG_CMD_ARR(GetPlayerPerks)
	//	v53.80
	REG_CMD(UnloadUIComponent)
	REG_CMD_ARR(GetQuests)
	REG_CMD_ARR(GetQuestObjectives)
	REG_CMD_ARR(GetActiveObjectives)
	REG_CMD_ARR(GetObjectiveTeleportLinks)
	REG_CMD_ARR(GetNifBlockParentNodes)
	REG_CMD(ClearMessageQueue)
	REG_CMD(InitMiniMap)
	REG_CMD(UpdateMiniMap)
	//	v54.20
	REG_CMD(GetCrosshairRefEx)
	REG_CMD(IsMobile)
	REG_CMD(IsGrabbable)
	REG_CMD(SetOnLocationDiscoverEventHandler)
	//	v54.30
	REG_CMD(AttachLight)
	REG_CMD(RemoveLight)
	REG_CMD(SetOnCraftingEventHandler)
	REG_CMD(IsInKillCam)
	REG_CMD(SwapObjectLOD)
	REG_CMD(ResetClouds)
	//	v54.31
	REG_CMD(GetPluginHeaderVersion)
	//	v54.40
	REG_CMD(SetOnHitEventHandler)
	//	v54.45
	REG_CMD(SetSystemColor)
	REG_CMD(SetWobblesRotation)
	REG_CMD(GetArmourPenetrated)
	REG_CMD(GetImpactMaterialType)
	REG_CMD(SetImpactMaterialType)
	REG_CMD(GetImpactDataSetForm)
	REG_CMD(SetImpactDataSetForm)
	//	v54.50
	REG_CMD(SetQuestFlag)
	REG_CMD(GetQuestFlag)
	REG_CMD(PushActorAwayAlt)
	//	v54.60
	REG_CMD(SetGameHour)
	REG_CMD(GetGameVolume)
	REG_CMD(SetGameVolume)
	REG_CMD(MoveAwayFrom)
	REG_CMD(GetEquippedWeaponPoison)
	REG_CMD(SuppressQuestMessages)
	REG_CMD(TravelToRef)
	//	v54.65
	REG_CMD(GetIsLAA)
	REG_CMD(fTan)
	REG_CMD(fAtan2)
	//	v54.70
	REG_CMD(DonnerReedKuruParty)
	REG_CMD(SetArmorClass)
	REG_CMD(GetEquippedEx)
	REG_CMD(TestEquippedSlots)
	REG_CMD(GetWeaponOnHitSetting)
	REG_CMD(SetWeaponOnHitSetting)
	//	v54.75
	REG_CMD(GetWeaponSemiAutoFireDelay)
	REG_CMD(SetWeaponSemiAutoFireDelay)
	REG_CMD(IsStickDisabled)
	REG_CMD(SetStickDisabled)
	REG_CMD(Sleep)
	//	v54.80
	REG_CMD_ARR(GetActiveIMODs)
	REG_CMD(ToggleItemUnique)
	REG_CMD(SetTextInputExtendedProps)
	REG_CMD(SetTextInputString)
	REG_CMD(ClickMenuTile)
	REG_CMD(StringToActorValue)
	REG_CMD_ARR(GetFactions)
	REG_CMD_ARR(GetBaseItems)
	//	v54.85
	REG_CMD_ARR(GetHit3DData)
	REG_CMD_STR(GetWeaponModel)
	REG_CMD(SetWeaponModel)
	REG_CMD(GetOwnerOfCell)
	REG_CMD(TogglePipBoyLight)
	REG_CMD(GetHardcoreTracking)
	//	v54.90
	REG_CMD(SetPerkLevel)
	REG_CMD(GetProjectileRefImpactRef)
	REG_CMD(SetOnProjectileImpactEventHandler)
	REG_CMD(GetNoteRead)
	REG_CMD(SetOnNoteAddedEventHandler)
	//	v54.95
	REG_CMD(SetOnUseAidItemEventHandler)
	//	v55.00
	REG_CMD(ToggleVanityWheel)
	REG_CMD(ToggleHUDCursor)
	//	v55.05
	REG_CMD(GetHardcoreStageThreshold)
	REG_CMD(SetHardcoreStageThreshold)
	REG_CMD(GetHardcoreStageEffect)
	REG_CMD(SetHardcoreStageEffect)
	REG_CMD(ClearModelAltTextures)
	REG_CMD(GetCreatureSoundsTemplate)
	REG_CMD(SetCreatureSoundsTemplate)
	//	v55.10
	REG_CMD(GetDebugModeState)
	REG_CMD(GetProjectileRefImpactMaterial)
	REG_CMD(FreezeTime)
	REG_CMD(GetConditionDamagePenalty)
	REG_CMD(SetConditionDamagePenalty)
	//	v55.15
	REG_CMD(GetExtraFloat)
	REG_CMD(SetExtraFloat)
	REG_CMD(ToggleBipedSlotVisibility)
	REG_CMD(SetOnReloadWeaponEventHandler)
	//	v55.20
	REG_CMD(AddTileFromTemplate)
	REG_CMD(UpdatePlayer3D)
	REG_CMD(GetArrayValue)
	//	v55.25
	REG_CMD(SetOnRagdollEventHandler)
	REG_CMD(PushActorNoRagdoll)
	//	v55.30
	REG_CMD(SetUIFloatGradual)
	REG_CMD(CloseActiveMenu)
	//	v55.35
	REG_CMD(ReloadCloudTextures)
	//	v55.40
	REG_CMD(ToggleImmortalMode)
	REG_CMD_STR(GetBodyPartVATSTarget)
	REG_CMD(SetLinearVelocity)
	//	v55.45
	REG_CMD_ARR(GetEquippedArmorRefs)
	REG_CMD(TF_DebugDump)
	REG_CMD(InsertNode)
	REG_CMD(AttachModel)
	REG_CMD(SynchronizePosition)
	REG_CMD(ToggleCameraCollision)
	//	v55.50
	REG_CMD(InitRockItLauncher)
	REG_CMD(ForceClimate)
	REG_CMD(GetInFactionList)
	REG_CMD(LeveledListHasFormDeep)
	REG_CMD(GetActorTiltAngle)
	REG_CMD(SetActorTiltAngle)
	REG_CMD(ReloadCharController)
	//	v55.55
	REG_CMD(GetRandomInRange)
	REG_CMD(ModelHasBlock)
	REG_CMD(GetGroundMaterial)
	REG_CMD(ShowLevelUpMenuEx)
	REG_CMD(GetRayCastRef)
	REG_CMD(GetRayCastMaterial)
	//	v55.65
	REG_CMD(FireWeaponEx)
	//	v55.70
	REG_CMD(GetArmorEffectiveDT)
	REG_CMD(GetArmorEffectiveDR)

	//===========================================================

	NVSECommandTableInterface *cmdTable = (NVSECommandTableInterface*)nvse->QueryInterface(kInterface_CommandTable);
	GetCmdByOpcode = cmdTable->GetByOpcode;
	CommandInfo *cmdInfo = GetCmdByOpcode(0x146B);
	cmdInfo->numParams = 2;
	cmdInfo->params = kParams_OneInt_OneOptionalInt;
	cmdInfo = GetCmdByOpcode(0x158D);
	cmdInfo->numParams = 2;
	cmdInfo->params = kParams_OneInt_OneOptionalInt;

	if (nvse->isEditor)
	{
		InitEditorPatches();
		return true;
	}

	PluginHandle pluginHandle = nvse->GetPluginHandle();
	NVSESerializationInterface *serialization = (NVSESerializationInterface*)nvse->QueryInterface(kInterface_Serialization);
	WriteRecord = serialization->WriteRecord;
	WriteRecordData = serialization->WriteRecordData;
	GetNextRecordInfo = serialization->GetNextRecordInfo;
	ReadRecordData = serialization->ReadRecordData;
	ResolveRefID = serialization->ResolveRefID;
	GetSavePath = serialization->GetSavePath;
	serialization->SetLoadCallback(pluginHandle, LoadGameCallback);
	serialization->SetSaveCallback(pluginHandle, SaveGameCallback);
	serialization->SetNewGameCallback(pluginHandle, NewGameCallback);
	NVSEStringVarInterface *strInterface = (NVSEStringVarInterface*)nvse->QueryInterface(kInterface_StringVar);
	GetStringVar = strInterface->GetString;
	AssignString = strInterface->Assign;
	NVSEArrayVarInterface *arrInterface = (NVSEArrayVarInterface*)nvse->QueryInterface(kInterface_ArrayVar);
	CreateArray = arrInterface->CreateArray;
	CreateStringMap = arrInterface->CreateStringMap;
	AssignCommandResult = arrInterface->AssignCommandResult;
	SetElement = arrInterface->SetElement;
	AppendElement = arrInterface->AppendElement;
	GetArraySize = arrInterface->GetArraySize;
	LookupArrayByID = arrInterface->LookupArrayByID;
	GetElement = arrInterface->GetElement;
	GetElements = arrInterface->GetElements;
	NVSEScriptInterface *scrInterface = (NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
	ExtractFormatStringArgs = scrInterface->ExtractFormatStringArgs;
	CallFunction = scrInterface->CallFunction;

	NVSEDataInterface *nvseData = (NVSEDataInterface*)nvse->QueryInterface(kInterface_Data);
	g_DIHookCtrl = (DIHookControl*)nvseData->GetSingleton(NVSEDataInterface::kNVSEData_DIHookControl);
	InventoryRefCreate = (InventoryRef* (*)(TESObjectREFR*, const InventoryRefData&, bool))nvseData->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceCreate);
	InventoryRefGetForID = (InventoryRef* (*)(UInt32))nvseData->GetFunc(NVSEDataInterface::kNVSEData_InventoryReferenceGetForRefID);
	g_NVSEArrayMap = nvseData->GetSingleton(NVSEDataInterface::kNVSEData_ArrayMap);
	g_NVSEStringMap = nvseData->GetSingleton(NVSEDataInterface::kNVSEData_StringMap);
	DelArrayVar = (void (*)(void*, UInt32))nvseData->GetFunc(NVSEDataInterface::kNVSEData_ArrayVarMapDeleteBySelf);
	DelStringVar = (void (*)(void*, UInt32))nvseData->GetFunc(NVSEDataInterface::kNVSEData_StringVarMapDeleteBySelf);
	g_numPreloadMods = (UInt8*)nvseData->GetData(NVSEDataInterface::kNVSEData_NumPreloadMods);

	HMODULE handle = GetModuleHandle("nvse_1_4.dll");
	if (((UInt32)g_NVSEArrayMap - (UInt32)handle) == 0xE3EF0)
		s_releaseFast = false;

	((NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging))->RegisterListener(pluginHandle, "NVSE", NVSEMessageHandler);

	s_dataPath = s_dataPathFull + 5;
	s_configPath = s_configPathFull + 12;
	s_scriptsPath = s_scriptsPathFull + 26;
	s_modLogPath = s_modLogPathFull + 9;

	return true;
}