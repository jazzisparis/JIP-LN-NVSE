#pragma once

DEFINE_COMMAND_PLUGIN(GetCellWaterHeight, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetCellWaterHeight, 0, kParams_OneFloat_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellWaterForm, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellImageSpace, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetCellLightingTemplate, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetCellLightingTemplate, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellFlag, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetCellFlag, 0, kParams_OneForm_TwoInts);

bool Cmd_GetCellWaterHeight_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
	{
		if (!cell)
			cell = g_thePlayer->parentCell;
		if (cell && IS_ID(cell, TESObjectCELL))
			*result = cell->waterHeight;
	}
	return true;
}

bool Cmd_SetCellWaterHeight_Execute(COMMAND_ARGS)
{
	float height;
	TESObjectCELL *cell = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &height, &cell))
	{
		if (!cell)
			cell = g_thePlayer->parentCell;
		if (cell && IS_ID(cell, TESObjectCELL))
			cell->waterHeight = height;
	}
	return true;
}

bool Cmd_GetCellWaterForm_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
	{
		if (!cell)
			cell = g_thePlayer->parentCell;
		if (cell && IS_ID(cell, TESObjectCELL))
			if (auto xCellWater = GetExtraType(&cell->extraDataList, ExtraCellWaterType); xCellWater && xCellWater->waterForm)
				REFR_RES = xCellWater->waterForm->refID;
			else if (cell->worldSpace && cell->worldSpace->waterFormFirst)
				REFR_RES = cell->worldSpace->waterFormFirst->refID;
	}
	return true;
}

bool Cmd_GetCellImageSpace_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && IS_ID(cell, TESObjectCELL))
		if (auto xCellIS = GetExtraType(&cell->extraDataList, ExtraCellImageSpace); xCellIS && xCellIS->imageSpace)
			REFR_RES = xCellIS->imageSpace->refID;
	return true;
}

bool Cmd_GetCellLightingTemplate_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && IS_ID(cell, TESObjectCELL) && cell->lightingTemplate)
		REFR_RES = cell->lightingTemplate->refID;
	return true;
}

bool Cmd_SetCellLightingTemplate_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	BGSLightingTemplate *lgtTemp = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &lgtTemp) && IS_ID(cell, TESObjectCELL) && (!lgtTemp || IS_ID(lgtTemp, BGSLightingTemplate)))
		cell->lightingTemplate = lgtTemp;
	return true;
}

bool Cmd_GetCellFlag_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &flag) && IS_ID(cell, TESObjectCELL) && (flag <= 16))
		if (flag <= 7)
		{
			if (cell->cellFlags & (1 << flag))
				*result = 1;
		}
		else if (cell->inheritFlags & (1 << (flag - 8)))
			*result = 1;
	return true;
}

bool Cmd_SetCellFlag_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	UInt32 flag, value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &flag, &value) && IS_ID(cell, TESObjectCELL) && (flag <= 16))
		if (flag <= 7)
		{
			if (value) cell->cellFlags |= (1 << flag);
			else cell->cellFlags &= ~(1 << flag);
		}
		else if (value) cell->inheritFlags |= (1 << (flag - 8));
		else cell->inheritFlags &= ~(1 << (flag - 8));
	return true;
}