#pragma once

DEFINE_COMMAND_PLUGIN(SetCellWaterForm, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellClimate, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetCellClimate, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellNoiseTexture, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetCellNoiseTexture, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetOwnerOfCell, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetCellNorthRotation, 0, kParams_OneForm);

bool Cmd_SetCellWaterForm_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	TESWaterForm *water = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &water) && IS_ID(cell, TESObjectCELL) && (!water || IS_ID(water, TESWaterForm)))
		if (auto xCellWater = GetExtraType(&cell->extraDataList, ExtraCellWaterType))
		{
			if (water) xCellWater->waterForm = water;
			else cell->extraDataList.RemoveByType(kXData_ExtraCellWaterType);
		}
		else if (water)
			cell->extraDataList.AddExtra(ExtraCellWaterType::Create(water));
	return true;
}

bool Cmd_GetCellClimate_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && IS_ID(cell, TESObjectCELL))
		if (auto xCellClimate = GetExtraType(&cell->extraDataList, ExtraCellClimate); xCellClimate && xCellClimate->climate)
			REFR_RES = xCellClimate->climate->refID;
	return true;
}

bool Cmd_SetCellClimate_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	TESClimate *climate = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &climate) && IS_ID(cell, TESObjectCELL) && (!climate || IS_ID(climate, TESClimate)))
		if (auto xCellClimate = GetExtraType(&cell->extraDataList, ExtraCellClimate))
		{
			if (climate) xCellClimate->climate = climate;
			else cell->extraDataList.RemoveByType(kXData_ExtraCellClimate);
		}
		else if (climate)
			cell->extraDataList.AddExtra(ExtraCellClimate::Create(climate));
	return true;
}

bool Cmd_GetCellNoiseTexture_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && IS_ID(cell, TESObjectCELL))
		resStr = cell->noiseTexture.ddsPath.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetCellNoiseTexture_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &path) && IS_ID(cell, TESObjectCELL))
		cell->noiseTexture.ddsPath.Set(path);
	return true;
}

bool Cmd_GetOwnerOfCell_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && IS_ID(cell, TESObjectCELL))
		if (TESForm *owner = ThisCall<TESForm*>(0x546A40, cell))
			REFR_RES = owner->refID;
	return true;
}

bool Cmd_GetCellNorthRotation_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && IS_ID(cell, TESObjectCELL) && cell->IsInterior())
		if (ExtraNorthRotation *xNorthRot = GetExtraType(&cell->extraDataList, ExtraNorthRotation))
			*result = xNorthRot->rotation * Dbl180dPI;
	return true;
}