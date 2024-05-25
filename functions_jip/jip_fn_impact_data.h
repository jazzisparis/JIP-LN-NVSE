#pragma once

DEFINE_COMMAND_PLUGIN(GetImpactDataModel, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetImpactDataModel, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetImpactDataTrait, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetImpactDataTraitNumeric, 0, kParams_OneForm_OneInt_OneDouble);
DEFINE_COMMAND_PLUGIN(SetImpactDataTraitForm, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetImpactDataSetForm, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetImpactDataSetForm, 0, kParams_OneForm_OneInt_OneOptionalForm);

bool Cmd_GetImpactDataModel_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	BGSImpactData *impactData;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactData) && IS_ID(impactData, BGSImpactData))
		resStr = impactData->model.GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetImpactDataModel_Execute(COMMAND_ARGS)
{
	BGSImpactData *impactData;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactData, &path) && IS_ID(impactData, BGSImpactData))
		impactData->model.SetModelPath(path);
	return true;
}

bool Cmd_GetImpactDataTrait_Execute(COMMAND_ARGS)
{
	BGSImpactData *impactData;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactData, &traitID) && IS_ID(impactData, BGSImpactData))
		switch (traitID)
		{
			case 1:
				*result = impactData->effectOrientation;
				break;
			case 0:
			case 2:
			case 3:
				*result = ((float*)impactData)[12 + traitID];
				break;
			case 4:
				*result = impactData->soundLevel;
				break;
			case 5:
			case 6:
			case 7:
			{
				if (TESForm *form = ((TESForm**)impactData)[13 + traitID])
					REFR_RES = form->refID;
				break;
			}
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				*result = ((float*)impactData)[13 + traitID];
				break;
			case 15:
				*result = impactData->decalInfo.parallaxPasses;
				break;
			case 16:
				*result = (impactData->decalInfo.flags << 1) | impactData->noDecalData;
				break;
			case 17:
				cvtul2d(RGBHexToDec(impactData->decalInfo.color), result);
		}
	return true;
}

bool Cmd_SetImpactDataTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSImpactData *impactData;
	UInt32 traitID;
	double value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactData, &traitID, &value) && IS_ID(impactData, BGSImpactData))
	{
		UInt32 intVal = cvtd2ul(value);
		switch (traitID)
		{
			case 1:
				if (intVal < 3) impactData->effectOrientation = intVal;
				break;
			case 0:
			case 2:
			case 3:
				((float*)impactData)[12 + traitID] = value;
				break;
			case 4:
				if (intVal < 3) impactData->soundLevel = intVal;
				break;
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
				((float*)impactData)[16 + traitID] = value;
				break;
			case 12:
				impactData->decalInfo.parallaxPasses = intVal;
				break;
			case 13:
				impactData->noDecalData = intVal & 1;
				impactData->decalInfo.flags = intVal >> 1;
				break;
			case 14:
				if (intVal <= 255255255) impactData->decalInfo.color = RGBDecToHex(intVal);
		}
	}
	return true;
}

bool Cmd_SetImpactDataTraitForm_Execute(COMMAND_ARGS)
{
	BGSImpactData *impactData;
	UInt32 traitID;
	TESForm *object = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactData, &traitID, &object) && IS_ID(impactData, BGSImpactData))
		switch (traitID)
		{
			case 0:
				impactData->textureSet = (object && IS_ID(object, BGSTextureSet)) ? (BGSTextureSet*)object : nullptr;
				break;
			case 1:
				impactData->sound1 = (object && IS_ID(object, TESSound)) ? (TESSound*)object : nullptr;
				break;
			case 2:
				impactData->sound2 = (object && IS_ID(object, TESSound)) ? (TESSound*)object : nullptr;
		}
	return true;
}

bool Cmd_GetImpactDataSetForm_Execute(COMMAND_ARGS)
{
	BGSImpactDataSet *impactDataSet;
	UInt32 materialID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactDataSet, &materialID) && IS_ID(impactDataSet, BGSImpactDataSet) && (materialID <= 11) && impactDataSet->impactDatas[materialID])
		REFR_RES = impactDataSet->impactDatas[materialID]->refID;
	return true;
}

bool Cmd_SetImpactDataSetForm_Execute(COMMAND_ARGS)
{
	BGSImpactDataSet *impactDataSet;
	UInt32 materialID;
	BGSImpactData *impactData = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &impactDataSet, &materialID, &impactData) && IS_ID(impactDataSet, BGSImpactDataSet) && (materialID <= 11) && (!impactData || IS_ID(impactData, BGSImpactData)))
		impactDataSet->impactDatas[materialID] = impactData;
	return true;
}