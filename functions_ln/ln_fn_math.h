#pragma once

DEFINE_COMMAND_PLUGIN(fsqrt, 0, kParams_OneDouble);
DEFINE_CMD_COND_PLUGIN(GetDistance2D, 1, kParams_OneObjectRef);
DEFINE_CMD_COND_PLUGIN(GetDistance3D, 1, kParams_OneObjectRef);
DEFINE_COMMAND_PLUGIN(fSin, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(fCos, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(fTan, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(fAsin, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(fAcos, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(fAtan, 0, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(fAtan2, 0, kParams_TwoFloats);

bool Cmd_fsqrt_Execute(COMMAND_ARGS)
{
	double value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
	{
		__asm
		{
			xorps	xmm1, xmm1
			maxsd	xmm1, value
			sqrtsd	xmm0, xmm1
			mov		eax, result
			movlpd	[eax], xmm0
		}
	}
	return true;
}

bool Cmd_GetDistance2D_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr) && thisObj->GetInSameCellOrWorld(refr))
		*result = Point2Distance(thisObj->position, refr->position);
	else *result = FLT_MAX;
	return true;
}

bool Cmd_GetDistance2D_Eval(COMMAND_ARGS_EVAL)
{
	TESObjectREFR *refr = (TESObjectREFR*)arg1;
	*result = thisObj->GetInSameCellOrWorld(refr) ? Point2Distance(thisObj->position, refr->position) : FLT_MAX;
	return true;
}

bool Cmd_GetDistance3D_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr))
		*result = thisObj->GetDistance(refr);
	else *result = FLT_MAX;
	return true;
}

bool Cmd_GetDistance3D_Eval(COMMAND_ARGS_EVAL)
{
	*result = thisObj->GetDistance((TESObjectREFR*)arg1);
	return true;
}

bool Cmd_fSin_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*result = Sin(value * FltPId180);
	return true;
}

bool Cmd_fCos_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*result = Cos(value * FltPId180);
	return true;
}

bool Cmd_fTan_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*result = Tan(value * FltPId180);
	return true;
}

bool Cmd_fAsin_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*result = ASin(value) * Dbl180dPI;
	return true;
}

bool Cmd_fAcos_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*result = ACos(value) * Dbl180dPI;
	return true;
}

bool Cmd_fAtan_Execute(COMMAND_ARGS)
{
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &value))
		*result = ATan(value) * Dbl180dPI;
	return true;
}

bool Cmd_fAtan2_Execute(COMMAND_ARGS)
{
	float y, x;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &y, &x))
		*result = ATan2(y, x) * Dbl180dPI;
	return true;
}