#pragma once

DEFINE_COMMAND_PLUGIN(SetLinkedRef, , 1, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(IsParentActivateOnly, , 0, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(SetBaseForm, , 1, 1, kParams_OneForm);
DEFINE_COMMAND_ALT_PLUGIN(GetInventoryWeight, GetInvWeight, , 1, 1, kParams_OneOptionalInt);
DEFINE_CMD_ALT_COND_PLUGIN(GetReferenceFlag, GetRefFlag, , 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetReferenceFlag, SetRefFlag, , 1, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(HasActionRef, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetRadius, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetRadius, , 0, 2, kParams_JIP_OneFloat_OneOptionalForm);

bool Cmd_SetLinkedRef_Execute(COMMAND_ARGS)
{
	return Cmd_SetLinkedReference_Execute(PASS_COMMAND_ARGS);
}

bool Cmd_IsParentActivateOnly_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *refr = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &refr)) return true;
	if (!refr)
	{
		if (!thisObj) return true;
		refr = thisObj;
	}
	ExtraActivateRef *xActivate = GetExtraType(&refr->extraDataList, ActivateRef);
	if (xActivate) *result = (int)xActivate->flags;
	return true;
}

bool Cmd_SetBaseForm_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (!containingObj && !thisObj->IsActor() && ExtractArgs(EXTRACT_ARGS, &form))
	{
		thisObj->baseForm = form;
		if (thisObj->GetNiNode()) thisObj->Update3D();
	}
	return true;
}

bool Cmd_GetInventoryWeight_Execute(COMMAND_ARGS)
{
	ExtraContainerChanges *xChanges = GetExtraType(&thisObj->extraDataList, ContainerChanges);
	*result = (xChanges && xChanges->data) ? xChanges->data->GetInventoryWeight() : 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetReferenceFlag_Eval(COMMAND_ARGS_EVAL)
{
	*result = (thisObj->flags & (UInt32)arg1) ? 1 : 0;
	return true;
}

bool Cmd_GetReferenceFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag;
	if (ExtractArgs(EXTRACT_ARGS, &flag))
		*result = (thisObj->flags & flag) ? 1 : 0;
	else *result = 0;
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetReferenceFlag_Execute(COMMAND_ARGS)
{
	UInt32 flag, inval;
	if (ExtractArgs(EXTRACT_ARGS, &flag, &inval))
	{
		if (inval) thisObj->flags |= flag;
		else thisObj->flags &= ~flag;
	}
	return true;
}

bool Cmd_HasActionRef_Execute(COMMAND_ARGS)
{
	*result = thisObj->extraDataList.HasType(kExtraData_Action);
	return true;
}

bool Cmd_GetRadius_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH *light = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &light)) return true;
	if (!light)
	{
		if (thisObj)
		{
			ExtraRadius *xRadius = GetExtraType(&thisObj->extraDataList, Radius);
			if (xRadius) *result = xRadius->radius;
		}
		return true;
	}
	if IS_TYPE(light, TESObjectLIGH)
		*result = (int)light->radius;
	return true;
}

bool Cmd_SetRadius_Execute(COMMAND_ARGS)
{
	float value;
	TESObjectLIGH *light = NULL;
	if (!ExtractArgs(EXTRACT_ARGS, &value, &light)) return true;
	if (!light)
	{
		if (thisObj)
		{
			ExtraRadius *xRadius = GetExtraType(&thisObj->extraDataList, Radius);
			if (xRadius) xRadius->radius = value;
			else
			{
				xRadius = ExtraRadius::Create(value);
				AddExtraData(&thisObj->extraDataList, xRadius);
			}
		}
		return true;
	}
	if IS_TYPE(light, TESObjectLIGH)
		light->radius = (int)value;
	return true;
}