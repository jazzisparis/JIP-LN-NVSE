#pragma once

DEFINE_COMMAND_PLUGIN(IsParentActivateOnly, 0, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(SetBaseForm, 1, kParams_OneBoundObject);
DEFINE_COMMAND_ALT_PLUGIN(GetInventoryWeight, GetInvWeight, 1, kParams_OneOptionalInt);
DEFINE_CMD_ALT_COND_PLUGIN(GetReferenceFlag, GetRefFlag, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetReferenceFlag, SetRefFlag, 1, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(HasActionRef, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetRadius, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetRadius, 0, kParams_OneFloat_OneOptionalForm);

bool Cmd_IsParentActivateOnly_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr))
		if (refr || (refr = thisObj))
			if (auto xActivate = GetExtraType(&refr->extraDataList, ExtraActivateRef))
				*result = (int)xActivate->flags;
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
	if (auto xChanges = GetExtraType(&thisObj->extraDataList, ExtraContainerChanges); xChanges && xChanges->data)
		*result = xChanges->data->GetInventoryWeight();
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetReferenceFlag_Eval(COMMAND_ARGS_EVAL)
{
	if (thisObj->flags & (UInt32)arg1)
		*result = 1;
	return true;
}

bool Cmd_GetReferenceFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &flag) && (thisObj->flags & flag))
		*result = 1;
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
	if (thisObj->extraDataList.HasType(kXData_ExtraAction))
		*result = 1;
	return true;
}

bool Cmd_GetRadius_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &light))
		if (!light)
		{
			if (thisObj)
				if (auto xRadius = GetExtraType(&thisObj->extraDataList, ExtraRadius))
					*result = xRadius->radius;
		}
		else if IS_ID(light, TESObjectLIGH)
			*result = (int)light->radius;
	return true;
}

bool Cmd_SetRadius_Execute(COMMAND_ARGS)
{
	float value;
	TESObjectLIGH *light = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value, &light))
		if (!light)
		{
			if (thisObj)
				if (auto xRadius = GetExtraType(&thisObj->extraDataList, ExtraRadius))
					xRadius->radius = value;
				else thisObj->extraDataList.AddExtra(ExtraRadius::Create(value));
		}
		else if IS_ID(light, TESObjectLIGH)
			light->radius = (int)value;
	return true;
}