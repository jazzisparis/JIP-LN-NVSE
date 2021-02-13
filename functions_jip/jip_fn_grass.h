#pragma once

DEFINE_COMMAND_PLUGIN(GetGrassTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetGrassTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetGrassModel, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetGrassModel, , 0, 2, kParams_JIP_OneForm_OneString);

bool Cmd_GetGrassTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESGrass *grass;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &grass, &traitID) || NOT_ID(grass, TESGrass)) return true;
	switch (traitID)
	{
	case 0:
		*result = grass->density;
		break;
	case 1:
		*result = grass->minSlope;
		break;
	case 2:
		*result = grass->maxSlope;
		break;
	case 3:
		*result = grass->unitFromWaterAmount;
		break;
	case 4:
		*result = grass->unitFromWaterType;
		break;
	case 5:
		*result = grass->positionRange;
		break;
	case 6:
		*result = grass->heightRange;
		break;
	case 7:
		*result = grass->colorRange;
		break;
	case 8:
		*result = grass->wavePeriod;
		break;
	case 9:
		*result = grass->flags;
	}
	return true;
}

bool Cmd_SetGrassTraitNumeric_Execute(COMMAND_ARGS)
{
	TESGrass *grass;
	UInt32 traitID;
	float fVal;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &grass, &traitID, &fVal) || NOT_ID(grass, TESGrass) || (fVal < 0)) return true;
	int iVal = fVal;
	switch (traitID)
	{
	case 0:
		grass->density = (iVal > 100) ? 100 : iVal;
		break;
	case 1:
		grass->minSlope = (iVal > grass->maxSlope) ? grass->maxSlope : iVal;
		break;
	case 2:
		grass->maxSlope = GetMax(grass->minSlope, GetMin(iVal, 90));
		break;
	case 3:
		grass->unitFromWaterAmount = (iVal > 0xFFFF) ? 0xFFFF : iVal;
		break;
	case 4:
		grass->unitFromWaterType = (iVal > 7) ? 7 : iVal;
		break;
	case 5:
		grass->positionRange = fVal;
		break;
	case 6:
		grass->heightRange = fVal;
		break;
	case 7:
		grass->colorRange = fVal;
		break;
	case 8:
		grass->wavePeriod = fVal;
		break;
	case 9:
		grass->flags = (iVal > 7) ? 7 : iVal;
	}
	return true;
}

bool Cmd_GetGrassModel_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &grass) && IS_ID(grass, TESGrass))
		resStr = grass->model.GetModelPath();
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetGrassModel_Execute(COMMAND_ARGS)
{
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &grass, &s_strArgBuffer) && IS_ID(grass, TESGrass))
		grass->model.SetModelPath(s_strArgBuffer);
	return true;
}