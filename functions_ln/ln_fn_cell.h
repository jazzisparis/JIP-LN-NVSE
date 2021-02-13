#pragma once

DEFINE_COMMAND_PLUGIN(GetCellRefs, , 0, 4, kParams_JIP_OneInt_TwoOptionalInts_OneOptionalCell);
DEFINE_COMMAND_PLUGIN(GetCellWaterHeight, , 0, 1, kParams_JIP_OneOptionalCell);
DEFINE_COMMAND_PLUGIN(SetCellWaterHeight, , 0, 2, kParams_JIP_OneFloat_OneOptionalCell);
DEFINE_COMMAND_PLUGIN(GetCellWaterForm, , 0, 1, kParams_JIP_OneOptionalCell);
DEFINE_COMMAND_PLUGIN(GetCellImageSpace, , 0, 1, kParams_JIP_OneCell);
DEFINE_COMMAND_PLUGIN(GetCellLightingTemplate, , 0, 1, kParams_JIP_OneCell);
DEFINE_COMMAND_PLUGIN(SetCellLightingTemplate, , 0, 2, kParams_JIP_OneCell_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetCellFlag, , 0, 2, kParams_JIP_OneCell_OneInt);
DEFINE_COMMAND_PLUGIN(SetCellFlag, , 0, 3, kParams_JIP_OneCell_TwoInts);

bool Cmd_GetCellRefs_Execute(COMMAND_ARGS)		// Deprecated
{
	return GetRefs(PASS_COMMAND_ARGS);
}

bool Cmd_GetCellWaterHeight_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && (cell || (cell = g_thePlayer->parentCell)))
		*result = cell->waterHeight;
	else *result = 0;
	return true;
}

bool Cmd_SetCellWaterHeight_Execute(COMMAND_ARGS)
{
	float height;
	TESObjectCELL *cell = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &height, &cell) && (cell || (cell = g_thePlayer->parentCell)))
		cell->waterHeight = height;
	return true;
}

bool Cmd_GetCellWaterForm_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && (cell || (cell = g_thePlayer->parentCell)))
	{
		ExtraCellWaterType *xCellWater = GetExtraType(&cell->extraDataList, CellWaterType);
		if (xCellWater && xCellWater->waterForm)
			REFR_RES = xCellWater->waterForm->refID;
		else if (cell->worldSpace && cell->worldSpace->waterFormFirst)
			REFR_RES = cell->worldSpace->waterFormFirst->refID;
	}
	return true;
}

bool Cmd_GetCellImageSpace_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell))
	{
		ExtraCellImageSpace *xCellIS = GetExtraType(&cell->extraDataList, CellImageSpace);
		if (xCellIS && xCellIS->imageSpace) REFR_RES = xCellIS->imageSpace->refID;
	}
	return true;
}

bool Cmd_GetCellLightingTemplate_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectCELL *cell;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell) && cell->lightingTemplate)
		REFR_RES = cell->lightingTemplate->refID;
	return true;
}

bool Cmd_SetCellLightingTemplate_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	BGSLightingTemplate *lgtTemp = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &lgtTemp) && (!lgtTemp || IS_ID(lgtTemp, BGSLightingTemplate)))
		cell->lightingTemplate = lgtTemp;
	return true;
}

bool Cmd_GetCellFlag_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &flag) && (flag <= 16))
	{
		if (flag <= 7) *result = (cell->cellFlags & (1 << flag)) ? 1 : 0;
		else *result = (cell->inheritFlags & (1 << (flag - 8))) ? 1 : 0;
	}
	else *result = 0;
	return true;
}

bool Cmd_SetCellFlag_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	UInt32 flag, value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &flag, &value) && (flag <= 16))
	{
		if (flag <= 7)
		{
			if (value) cell->cellFlags |= (1 << flag);
			else cell->cellFlags &= ~(1 << flag);
		}
		else if (value) cell->inheritFlags |= (1 << (flag - 8));
		else cell->inheritFlags &= ~(1 << (flag - 8));
	}
	return true;
}