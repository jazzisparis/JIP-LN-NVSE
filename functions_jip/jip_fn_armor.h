#pragma once

DEFINE_COMMAND_PLUGIN(GetArmorRCT, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetArmorRCT, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(SetBipedModelPathAlt, 0, kParams_OneObjectID_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetArmorAudioTemplate, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetArmorAudioTemplate, 0, kParams_OneObjectID_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(IsArmorAddon, 0, kParams_OneObjectID);
DEFINE_COMMAND_PLUGIN(SetArmorClass, 0, kParams_OneObjectID_OneInt);
DEFINE_COMMAND_PLUGIN(ClearModelAltTextures, 0, kParams_OneObjectID);

bool Cmd_GetArmorRCT_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESObjectARMO *armor;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &armor) && IS_TYPE(armor, TESObjectARMO))
		resStr = armor->bipedModel.modelRDT.nifPath.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetArmorRCT_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &armor, &path) && IS_TYPE(armor, TESObjectARMO))
		armor->bipedModel.modelRDT.nifPath.Set(path);
	return true;
}

bool Cmd_SetBipedModelPathAlt_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor;
	UInt32 whichPath;
	char buffer[0x80];
	if (ExtractFormatStringArgs(2, buffer, EXTRACT_ARGS_EX, kCommandInfo_SetBipedModelPathAlt.numParams, &armor, &whichPath) && 
		IS_TYPE(armor, TESObjectARMO) && (whichPath <= 3))
		armor->bipedModel.bipedModel[whichPath].nifPath.Set(buffer);
	return true;
}

bool Cmd_GetArmorAudioTemplate_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &armor) && IS_TYPE(armor, TESObjectARMO) && !armor->overrideSounds && armor->audioTemplate)
		REFR_RES = armor->audioTemplate->refID;
	return true;
}

bool Cmd_SetArmorAudioTemplate_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor, *audioTemp = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &armor, &audioTemp) && IS_TYPE(armor, TESObjectARMO) && !armor->overrideSounds && 
		(!audioTemp || (IS_TYPE(audioTemp, TESObjectARMO) && audioTemp->overrideSounds)))
		armor->audioTemplate = audioTemp;
	return true;
}

bool Cmd_IsArmorAddon_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && IS_TYPE(form, TESObjectARMA))
		*result = 1;
	return true;
}

bool Cmd_SetArmorClass_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor;
	UInt32 newClass;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &armor, &newClass) && IS_TYPE(armor, TESObjectARMO) && newClass && (newClass <= 3))
	{
		armor->bipedModel.bipedFlags &= ~0x88;
		if (newClass > 1)
			armor->bipedModel.bipedFlags |= (newClass == 2) ? 8 : 0x80;
	}
	return true;
}

bool Cmd_ClearModelAltTextures_Execute(COMMAND_ARGS)
{
	TESObjectARMO *armor;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &armor) && IS_TYPE(armor, TESObjectARMO))
	{
		TESModelTextureSwap *texSwap = armor->bipedModel.bipedModel;
		for (UInt32 index = 0; index < 4; index++)
			texSwap[index].textureList.RemoveAll();
	}
	return true;
}