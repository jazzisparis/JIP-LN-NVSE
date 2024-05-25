#pragma once

DEFINE_COMMAND_PLUGIN(GetGrassTraitNumeric, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetGrassTraitNumeric, 0, kParams_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetGrassModel, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetGrassModel, 0, kParams_OneForm_OneString);

bool Cmd_GetGrassTraitNumeric_Execute(COMMAND_ARGS)
{
	TESGrass *grass;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &grass, &traitID) && IS_ID(grass, TESGrass))
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
				*result = grass->grassFlags;
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
			grass->density = GetMin(iVal, 100);
			break;
		case 1:
			grass->minSlope = GetMin(grass->maxSlope, iVal);
			break;
		case 2:
			grass->maxSlope = GetMax(grass->minSlope, GetMin(iVal, 90));
			break;
		case 3:
			grass->unitFromWaterAmount = GetMin(iVal, 0xFFFF);
			break;
		case 4:
			grass->unitFromWaterType = GetMin(iVal, 7);
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
			grass->grassFlags = GetMin(iVal, 7);
	}
	return true;
}

bool Cmd_GetGrassModel_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESGrass *grass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &grass) && IS_ID(grass, TESGrass))
		resStr = grass->model.GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetGrassModel_Execute(COMMAND_ARGS)
{
	TESGrass *grass;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &grass, &path) && IS_ID(grass, TESGrass))
		grass->model.SetModelPath(path);
	return true;
}