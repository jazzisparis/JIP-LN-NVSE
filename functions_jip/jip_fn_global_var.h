#pragma once

DEFINE_COMMAND_PLUGIN(GetGlobalVariable, 0, kParams_OneGlobal);
DEFINE_COMMAND_PLUGIN(SetGlobalVariable, 0, kParams_OneGlobal_OneFloat);
DEFINE_COMMAND_PLUGIN(GetGlobalRef, 0, kParams_OneGlobal);
DEFINE_COMMAND_PLUGIN(SetGlobalRef, 0, kParams_OneGlobal_OneOptionalForm);

bool Cmd_GetGlobalVariable_Execute(COMMAND_ARGS)
{
	TESGlobal *global;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global))
		*result = global->data;
	return true;
}

bool Cmd_SetGlobalVariable_Execute(COMMAND_ARGS)
{
	TESGlobal *global;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global, &value))
		global->data = value;
	return true;
}

bool Cmd_GetGlobalRef_Execute(COMMAND_ARGS)
{
	TESGlobal *global;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global))
	{
		if (!(global->flags & 0x200))
		{
			global->flags |= 0x200;
			GetResolvedRefID(&global->uRefID);
		}
		REFR_RES = global->uRefID;
	}
	return true;
}

bool Cmd_SetGlobalRef_Execute(COMMAND_ARGS)
{
	TESGlobal *global;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global, &form))
	{
		global->uRefID = form ? form->refID : 0;
		global->flags |= 0x200;
	}
	return true;
}