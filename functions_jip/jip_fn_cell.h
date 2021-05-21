#pragma once

DEFINE_COMMAND_PLUGIN(SetCellWaterForm, , 0, 2, kParams_JIP_OneCell_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellClimate, , 0, 1, kParams_JIP_OneCell);
DEFINE_COMMAND_PLUGIN(SetCellClimate, , 0, 2, kParams_JIP_OneCell_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellNoiseTexture, , 0, 1, kParams_JIP_OneCell);
DEFINE_COMMAND_PLUGIN(SetCellNoiseTexture, , 0, 2, kParams_JIP_OneCell_OneString);
DEFINE_COMMAND_PLUGIN(GetOwnerOfCell, , 0, 1, kParams_JIP_OneCell);

bool Cmd_SetCellWaterForm_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	TESWaterForm *water = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &water) && (!water || IS_ID(water, TESWaterForm)))
	{
		ExtraCellWaterType *xCellWater = GetExtraType(&cell->extraDataList, CellWaterType);
		if (xCellWater)
		{
			if (water) xCellWater->waterForm = water;
			else RemoveExtraData(&cell->extraDataList, xCellWater, true);
		}
		else if (water) AddExtraData(&cell->extraDataList, ExtraCellWaterType::Create(water));
	}
	return true;
}

bool Cmd_GetCellClimate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
	{
		ExtraCellClimate *xCellClimate = GetExtraType(&cell->extraDataList, CellClimate);
		if (xCellClimate && xCellClimate->climate) REFR_RES = xCellClimate->climate->refID;
	}
	return true;
}

bool Cmd_SetCellClimate_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	TESClimate *climate = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &climate) && (!climate || IS_ID(climate, TESClimate)))
	{
		ExtraCellClimate *xCellClimate = GetExtraType(&cell->extraDataList, CellClimate);
		if (xCellClimate)
		{
			if (climate) xCellClimate->climate = climate;
			else RemoveExtraData(&cell->extraDataList, xCellClimate, true);
		}
		else if (climate) AddExtraData(&cell->extraDataList, ExtraCellClimate::Create(climate));
	}
	return true;
}

bool Cmd_GetCellNoiseTexture_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
		resStr = cell->noiseTexture.ddsPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetCellNoiseTexture_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &path))
		cell->noiseTexture.ddsPath.Set(path);
	return true;
}

bool Cmd_GetOwnerOfCell_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
	{
		ExtraOwnership *xOwnership = GetExtraType(&cell->extraDataList, Ownership);
		if (xOwnership && xOwnership->owner)
			REFR_RES = xOwnership->owner->refID;
	}
	return true;
}