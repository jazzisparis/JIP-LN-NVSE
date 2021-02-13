#pragma once

DEFINE_COMMAND_PLUGIN(GetImageSpaceTrait, , 0, 2, kParams_JIP_OneImageSpace_OneInt);
DEFINE_COMMAND_PLUGIN(SetImageSpaceTrait, , 0, 3, kParams_JIP_OneImageSpace_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetImageSpaceModTrait, , 0, 2, kParams_JIP_OneIMOD_OneInt);
DEFINE_COMMAND_PLUGIN(SetImageSpaceModTrait, , 0, 3, kParams_JIP_OneIMOD_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetActiveIMODs, , 0, 0, NULL);

bool Cmd_GetImageSpaceTrait_Execute(COMMAND_ARGS)
{
	TESImageSpace *imgSpace;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imgSpace, &traitID) && (traitID <= 32))
	{
		if (((traitID >= 21) && (traitID <= 23)) || ((traitID >= 29) && (traitID <= 31))) *result = imgSpace->traitValues[traitID] * 255;
		else *result = imgSpace->traitValues[traitID];
	}
	else *result = 0;
	return true;
}

bool Cmd_SetImageSpaceTrait_Execute(COMMAND_ARGS)
{
	TESImageSpace *imgSpace;
	UInt32 traitID;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imgSpace, &traitID, &value) && (traitID <= 32))
	{
		if (((traitID >= 21) && (traitID <= 23)) || ((traitID >= 29) && (traitID <= 31))) imgSpace->traitValues[traitID] = value / 255;
		else imgSpace->traitValues[traitID] = value;
	}
	return true;
}

bool Cmd_GetImageSpaceModTrait_Execute(COMMAND_ARGS)
{
	TESImageSpaceModifier *imod;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imod, &traitID) && (traitID <= 63))
	{
		if (!traitID)
			*result = imod->duration;
		else if (traitID == 1)
			*result = imod->radialBlurCentreX;
		else if (traitID == 2)
			*result = imod->radialBlurCentreY;
		else if (traitID <= 46)
			*result = imod->data654[traitID - 3]->value;
		else if (traitID <= 50)
			*result = imod->data704[0]->value[traitID - 47] * 255;
		else if (traitID <= 54)
			*result = imod->data704[1]->value[traitID - 51] * 255;
		else
			*result = imod->data70C[traitID - 55]->value;
	}
	else *result = 0;
	return true;
}

bool Cmd_SetImageSpaceModTrait_Execute(COMMAND_ARGS)
{
	TESImageSpaceModifier *imod;
	UInt32 traitID;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &imod, &traitID, &value) && (traitID <= 63))
	{
		if (!traitID)
			imod->duration = value;
		else if (traitID == 1)
			imod->radialBlurCentreX = value;
		else if (traitID == 2)
			imod->radialBlurCentreY = value;
		else if (traitID <= 46)
		{
			traitID -= 3;
			imod->fltIntrpl1[traitID].value = value;
			imod->data654[traitID]->value = value;
		}
		else if (traitID <= 50)
		{
			traitID -= 47;
			value /= 255;
			imod->clrIntrpl[0].value[traitID] = value;
			imod->data704[0]->value[traitID] = value;
		}
		else if (traitID <= 54)
		{
			traitID -= 51;
			value /= 255;
			imod->clrIntrpl[1].value[traitID] = value;
			imod->data704[1]->value[traitID] = value;
		}
		else
		{
			traitID -= 55;
			imod->fltIntrpl2[traitID].value = value;
			imod->data70C[traitID]->value = value;
		}
	}
	return true;
}

bool Cmd_GetActiveIMODs_Execute(COMMAND_ARGS)
{
	*result = 0;
	s_tempElements.Clear();
	ListNode<ImageSpaceModifierInstance> *traverse = g_TES->activeIMODs.Head();
	ImageSpaceModifierInstance *imodInstance;
	TESImageSpaceModifier *imod;
	do
	{
		imodInstance = traverse->data;
		if (!imodInstance || NOT_TYPE(imodInstance, ImageSpaceModifierInstanceForm) || imodInstance->hidden) continue;
		imod = ((ImageSpaceModifierInstanceForm*)imodInstance)->imageSpace;
		if (imod) s_tempElements.Append(imod);
	}
	while (traverse = traverse->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}