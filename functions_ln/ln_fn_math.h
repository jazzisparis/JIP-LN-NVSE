#pragma once

DEFINE_COMMAND_PLUGIN(fsqrt, , 0, 1, kParams_JIP_OneDouble);
DEFINE_CMD_ALT_COND_PLUGIN(GetDistance2D, , , 1, kParams_JIP_OneObjectRef);
DEFINE_CMD_ALT_COND_PLUGIN(GetDistance3D, , , 1, kParams_JIP_OneObjectRef);
DEFINE_COMMAND_PLUGIN(fSin, , 0, 2, kParams_JIP_OneDouble_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(fCos, , 0, 2, kParams_JIP_OneDouble_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(fTan, , 0, 1, kParams_JIP_OneDouble);
DEFINE_COMMAND_PLUGIN(fAsin, , 0, 1, kParams_JIP_OneDouble);
DEFINE_COMMAND_PLUGIN(fAcos, , 0, 1, kParams_JIP_OneDouble);
DEFINE_COMMAND_PLUGIN(fAtan, , 0, 1, kParams_JIP_OneDouble);
DEFINE_COMMAND_PLUGIN(fAtan2, , 0, 2, kParams_JIP_TwoDoubles);

bool Cmd_fsqrt_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgs(EXTRACT_ARGS, &value))
	{
		__asm
		{
			fld		value
			fsqrt
			mov		eax, result
			fstp	qword ptr [eax]
		}
	}
	else *result = 0;
	return true;
}

bool Cmd_GetDistance2D_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	if (ExtractArgs(EXTRACT_ARGS, &refr) && thisObj->GetInSameCellOrWorld(refr))
		*result = GetDistance2D(thisObj, refr);
	else *result = kFltMax;
	return true;
}

bool Cmd_GetDistance2D_Eval(COMMAND_ARGS_EVAL)
{
	TESObjectREFR *refr = (TESObjectREFR*)arg1;
	*result = thisObj->GetInSameCellOrWorld(refr) ? GetDistance2D(thisObj, refr) : kFltMax;
	return true;
}

bool Cmd_GetDistance3D_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	if (ExtractArgs(EXTRACT_ARGS, &refr))
		*result = thisObj->GetDistance(refr);
	else *result = kFltMax;
	return true;
}

bool Cmd_GetDistance3D_Eval(COMMAND_ARGS_EVAL)
{
	*result = thisObj->GetDistance((TESObjectREFR*)arg1);
	return true;
}

bool Cmd_fSin_Execute(COMMAND_ARGS)
{
	double value;
	UInt32 precision;
	if (ExtractArgs(EXTRACT_ARGS, &value, &precision))
		*result = sin(value * kDblPId180);
	else *result = 0;
	return true;
}

bool Cmd_fCos_Execute(COMMAND_ARGS)
{
	double value;
	UInt32 precision;
	if (ExtractArgs(EXTRACT_ARGS, &value, &precision))
		*result = cos(value * kDblPId180);
	else *result = 0;
	return true;
}

bool Cmd_fTan_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgs(EXTRACT_ARGS, &value))
		*result = tan(value * kDblPId180);
	else *result = 0;
	return true;
}

bool Cmd_fAsin_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgs(EXTRACT_ARGS, &value))
		*result = asin(value) / kDblPId180;
	else *result = 0;
	return true;
}

bool Cmd_fAcos_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgs(EXTRACT_ARGS, &value))
		*result = acos(value) / kDblPId180;
	else *result = 0;
	return true;
}

bool Cmd_fAtan_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgs(EXTRACT_ARGS, &value))
		*result = atan(value) / kDblPId180;
	else *result = 0;
	return true;
}

bool Cmd_fAtan2_Execute(COMMAND_ARGS)
{
	double y, x;
	if (ExtractArgs(EXTRACT_ARGS, &y, &x))
		*result = atan2(y, x) / kDblPId180;
	else *result = 0;
	return true;
}