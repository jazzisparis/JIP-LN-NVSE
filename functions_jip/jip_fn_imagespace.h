#pragma once

DEFINE_COMMAND_PLUGIN(GetImageSpaceTrait, 0, kParams_OneImageSpace_OneInt);
DEFINE_COMMAND_PLUGIN(SetImageSpaceTrait, 0, kParams_OneImageSpace_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetImageSpaceModTrait, 0, kParams_OneIMOD_OneInt);
DEFINE_COMMAND_PLUGIN(SetImageSpaceModTrait, 0, kParams_OneIMOD_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetActiveIMODs, 0, nullptr);

bool Cmd_GetImageSpaceTrait_Execute(COMMAND_ARGS)
{
	TESImageSpace *imgSpace;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imgSpace, &traitID) && (traitID <= 32))
	{
		if ((traitID >= 21) && (traitID <= 31) && ((traitID <= 23) || (traitID >= 29)))
			*result = imgSpace->traitValues[traitID] * 255;
		else *result = imgSpace->traitValues[traitID];
	}
	return true;
}

bool Cmd_SetImageSpaceTrait_Execute(COMMAND_ARGS)
{
	TESImageSpace *imgSpace;
	UInt32 traitID;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imgSpace, &traitID, &value) && (traitID <= 32))
		if ((traitID >= 21) && (traitID <= 31) && ((traitID <= 23) || (traitID >= 29)))
			imgSpace->traitValues[traitID] = value * (1 / 255.0F);
		else imgSpace->traitValues[traitID] = value;
	return true;
}

bool Cmd_GetImageSpaceModTrait_Execute(COMMAND_ARGS)
{
	TESImageSpaceModifier *imod;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imod, &traitID) && (traitID <= 67))
	{
		if (traitID <= 2)
		{
			switch (traitID)
			{
			case 0:
				*result = imod->duration;
				break;
			case 1:
				*result = imod->radialBlurCentreX;
				break;
			case 2:
				*result = imod->radialBlurCentreY;
				break;
			}
		}
		else if (traitID > 63)
		{
			switch (traitID - 64)
			{
			case 0:
				*result = imod->animable;
				break;
			case 1:
				*result = imod->radBlurUseTarget;
				break;
			case 2:
				*result = imod->DoFUseTarget;
				break;
			case 3:
				*result = imod->DoFMode;
				break;
			}
		}
		else if (traitID <= 46)
			*result = imod->data654[traitID - 3]->value;
		else if (traitID <= 50)
			*result = imod->data704[0]->value[traitID - 47] * 255;
		else if (traitID <= 54)
			*result = imod->data704[1]->value[traitID - 51] * 255;
		else
			*result = imod->data70C[traitID - 55]->value;
	}
	return true;
}

bool Cmd_SetImageSpaceModTrait_Execute(COMMAND_ARGS)
{
	TESImageSpaceModifier *imod;
	UInt32 traitID;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imod, &traitID, &value) && (traitID <= 67))
		if (traitID <= 2)
		{
			switch (traitID)
			{
			case 0:
				imod->duration = value;
				break;
			case 1:
				imod->radialBlurCentreX = value;
				break;
			case 2:
				imod->radialBlurCentreY = value;
				break;
			}
		}
		else if (traitID > 63)
		{
			UInt8 iVal = UInt8(value) & 3;
			switch (traitID - 64)
			{
			case 0:
				imod->animable = iVal;
				break;
			case 1:
				imod->radBlurUseTarget = iVal;
				break;
			case 2:
				imod->DoFUseTarget = iVal;
				break;
			case 3:
				imod->DoFMode = iVal;
				break;
			}
		}
		else if (traitID <= 46)
		{
			traitID -= 3;
			imod->fltIntrpl1[traitID].value = value;
			imod->data654[traitID]->value = value;
		}
		else if (traitID <= 50)
		{
			traitID -= 47;
			value *= 1 / 255.0F;
			imod->clrIntrpl[0].value[traitID] = value;
			imod->data704[0]->value[traitID] = value;
		}
		else if (traitID <= 54)
		{
			traitID -= 51;
			value *= 1 / 255.0F;
			imod->clrIntrpl[1].value[traitID] = value;
			imod->data704[1]->value[traitID] = value;
		}
		else
		{
			traitID -= 55;
			imod->fltIntrpl2[traitID].value = value;
			imod->data70C[traitID]->value = value;
		}
	return true;
}

bool Cmd_GetActiveIMODs_Execute(COMMAND_ARGS)
{
	TempElements *tmpElements = GetTempElements();
	auto traverse = g_TES->activeIMODs.Head();
	do
	{
		if (ImageSpaceModifierInstance *imodInstance = traverse->data; imodInstance && IS_TYPE(imodInstance, ImageSpaceModifierInstanceForm) && !imodInstance->hidden)
			if (TESImageSpaceModifier *imod = ((ImageSpaceModifierInstanceForm*)imodInstance)->imageSpaceMod)
				tmpElements->Append(imod);
	}
	while (traverse = traverse->next);
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}