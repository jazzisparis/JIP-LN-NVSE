#pragma once

DEFINE_COMMAND_PLUGIN(GetGlobalVariable, , 0, 1, kParams_JIP_OneGlobal);
DEFINE_COMMAND_PLUGIN(SetGlobalVariable, , 0, 2, kParams_JIP_OneGlobal_OneFloat);
DEFINE_COMMAND_PLUGIN(GetGlobalRef, , 0, 1, kParams_JIP_OneGlobal);
DEFINE_COMMAND_PLUGIN(SetGlobalRef, , 0, 2, kParams_JIP_OneGlobal_OneOptionalForm);

bool Cmd_GetGlobalVariable_Execute(COMMAND_ARGS)
{
	TESGlobal *global;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global)) *result = global->data;
	else *result = 0;
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
	*result = 0;
	TESGlobal *global;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global) && global->uRefID)
	{
		if (global->jipFormFlags6)
			REFR_RES = global->uRefID;
		else
		{
			s_resolvedGlobals.Insert(global);
			REFR_RES = global->ResolveRefValue();
		}
	}
	return true;
}

bool Cmd_SetGlobalRef_Execute(COMMAND_ARGS)
{
	TESGlobal *global;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &global, &form))
		global->uRefID = form ? form->refID : (thisObj ? thisObj->refID : 0);
	return true;
}