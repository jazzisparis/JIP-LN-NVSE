#pragma once

DEFINE_COMMAND_PLUGIN(GetLandTextureTextureSet, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLandTextureTextureSet, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetLandTextureTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLandTextureTraitNumeric, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetLandTextureNumGrasses, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetLandTextureNthGrass, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(LandTextureAddGrass, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(LandTextureRemoveGrass, , 0, 2, kParams_JIP_TwoForms);

bool Cmd_GetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESLandTexture *landTex;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex) && (landTex->typeID == kFormType_TESLandTexture) && landTex->textureSet)
		REFR_RES = landTex->textureSet->refID;
	return true;
}

bool Cmd_SetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	BGSTextureSet *texSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &texSet) && (landTex->typeID == kFormType_TESLandTexture) && (texSet->typeID == kFormType_BGSTextureSet))
		landTex->textureSet = texSet;
	return true;
}

bool Cmd_GetLandTextureTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESLandTexture *landTex;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &traitID) || (landTex->typeID != kFormType_TESLandTexture)) return true;
	switch (traitID)
	{
	case 0:
		*result = landTex->materialType;
		break;
	case 1:
		*result = landTex->friction;
		break;
	case 2:
		*result = landTex->restitution;
		break;
	case 3:
		*result = landTex->specularExponent;
	}
	return true;
}

bool Cmd_SetLandTextureTraitNumeric_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	UInt32 traitID, val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &traitID, &val) || (landTex->typeID != kFormType_TESLandTexture)) return true;
	switch (traitID)
	{
	case 0:
		if (val < 32) landTex->materialType = val;
		break;
	case 1:
		landTex->friction = val;
		break;
	case 2:
		landTex->restitution = val;
		break;
	case 3:
		landTex->specularExponent = val;
	}
	return true;
}

bool Cmd_GetLandTextureNumGrasses_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex) && (landTex->typeID == kFormType_TESLandTexture))
		*result = (int)landTex->grasses.Count();
	else *result = 0;
	return true;
}

bool Cmd_GetLandTextureNthGrass_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESLandTexture *landTex;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &idx) && (landTex->typeID == kFormType_TESLandTexture))
	{
		TESGrass *grass = landTex->grasses.GetNthItem(idx);
		if (grass) REFR_RES = grass->refID;
	}
	return true;
}

bool Cmd_LandTextureAddGrass_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &grass) && (landTex->typeID == kFormType_TESLandTexture) &&
		(grass->typeID == kFormType_TESGrass) && !landTex->grasses.IsInList(grass))
	{
		landTex->grasses.Prepend(grass);
		*result = 1;
	}
	else *result = 0;
	return true;
}

bool Cmd_LandTextureRemoveGrass_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &grass) && (landTex->typeID == kFormType_TESLandTexture) &&
		(grass->typeID == kFormType_TESGrass) && landTex->grasses.Remove(grass))
		*result = 1;
	else *result = 0;
	return true;
}