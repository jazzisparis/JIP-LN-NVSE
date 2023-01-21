#pragma once

DEFINE_COMMAND_PLUGIN(IsParentActivateOnly, 0, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(SetBaseForm, 1, 1, kParams_OneBoundObject);
DEFINE_COMMAND_ALT_PLUGIN(GetInventoryWeight, GetInvWeight, 1, 1, kParams_OneOptionalInt);
DEFINE_CMD_ALT_COND_PLUGIN(GetReferenceFlag, GetRefFlag, 1, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetReferenceFlag, SetRefFlag, 1, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(HasActionRef, 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadius, 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetRadius, 0, 2, kParams_OneFloat_OneOptionalForm);

bool Cmd_IsParentActivateOnly_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *refr = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &refr)) return true;
	if (!refr)
	{
		if (!thisObj) return true;
		refr = thisObj;
	}
	ExtraActivateRef *xActivate = GetExtraType(&refr->extraDataList, ExtraActivateRef);
	if (xActivate) *result = (int)xActivate->flags;
	return true;
}

bool Cmd_SetBaseForm_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (!containingObj && ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		thisObj->baseForm = form;
		thisObj->Update3D();
	}
	return true;
}

bool Cmd_GetInventoryWeight_Execute(COMMAND_ARGS)
{
	ExtraContainerChanges *xChanges = GetExtraType(&thisObj->extraDataList, ExtraContainerChanges);
	*result = (xChanges && xChanges->data) ? xChanges->data->GetInventoryWeight() : 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetReferenceFlag_Eval(COMMAND_ARGS_EVAL)
{
	*result = (thisObj->flags & (UInt32)arg1) != 0;
	return true;
}

bool Cmd_GetReferenceFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &flag) && (thisObj->flags & flag))
		*result = 1;
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetReferenceFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag, inval;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &flag, &inval))
	{
		if (inval) thisObj->flags |= flag;
		else thisObj->flags &= ~flag;
		//ThisCall(0x84A690, *(void**)0x11DDF38, thisObj, 1, 0);
	}
	return true;
}

bool Cmd_HasActionRef_Execute(COMMAND_ARGS)
{
	*result = thisObj->extraDataList.HasType(kXData_ExtraAction);
	return true;
}

bool Cmd_GetRadius_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH *light = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &light)) return true;
	if (!light)
	{
		if (thisObj)
		{
			ExtraRadius *xRadius = GetExtraType(&thisObj->extraDataList, ExtraRadius);
			if (xRadius) *result = xRadius->radius;
		}
		return true;
	}
	if IS_ID(light, TESObjectLIGH)
		*result = (int)light->radius;
	return true;
}

bool Cmd_SetRadius_Execute(COMMAND_ARGS)
{
	float value;
	TESObjectLIGH *light = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &value, &light)) return true;
	if (!light)
	{
		if (thisObj)
		{
			ExtraRadius *xRadius = GetExtraType(&thisObj->extraDataList, ExtraRadius);
			if (xRadius) xRadius->radius = value;
			else thisObj->extraDataList.AddExtra(ExtraRadius::Create(value));
		}
		return true;
	}
	if IS_ID(light, TESObjectLIGH)
		light->radius = (int)value;
	return true;
}