#pragma once

DEFINE_COMMAND_PLUGIN(GetLandTextureTextureSet, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetLandTextureTextureSet, 0, kParams_TwoForms);
DEFINE_COMMAND_PLUGIN(GetLandTextureTraitNumeric, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLandTextureTraitNumeric, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(GetLandTextureNumGrasses, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetLandTextureNthGrass, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(LandTextureAddGrass, 0, kParams_TwoForms);
DEFINE_COMMAND_PLUGIN(LandTextureRemoveGrass, 0, kParams_TwoForms);

bool Cmd_GetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex) && IS_ID(landTex, TESLandTexture) && landTex->textureSet)
		REFR_RES = landTex->textureSet->refID;
	return true;
}

bool Cmd_SetLandTextureTextureSet_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	BGSTextureSet *texSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &texSet) && IS_ID(landTex, TESLandTexture) && IS_ID(texSet, BGSTextureSet))
		landTex->textureSet = texSet;
	return true;
}

bool Cmd_GetLandTextureTraitNumeric_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &traitID) && IS_ID(landTex, TESLandTexture))
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &traitID, &val) && IS_ID(landTex, TESLandTexture))
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
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex) && IS_ID(landTex, TESLandTexture))
		*result = (int)landTex->grasses.Count();
	return true;
}

bool Cmd_GetLandTextureNthGrass_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &idx) && IS_ID(landTex, TESLandTexture))
		if (TESGrass *grass = landTex->grasses.GetNthItem(idx))
			REFR_RES = grass->refID;
	return true;
}

bool Cmd_LandTextureAddGrass_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &grass) && IS_ID(landTex, TESLandTexture) && IS_ID(grass, TESGrass) && !landTex->grasses.IsInList(grass))
	{
		landTex->grasses.Prepend(grass);
		*result = 1;
	}
	return true;
}

bool Cmd_LandTextureRemoveGrass_Execute(COMMAND_ARGS)
{
	TESLandTexture *landTex;
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &landTex, &grass) && IS_ID(landTex, TESLandTexture) && IS_ID(grass, TESGrass) && landTex->grasses.Remove(grass))
		*result = 1;
	return true;
}