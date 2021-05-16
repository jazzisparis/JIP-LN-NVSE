#pragma once

DEFINE_COMMAND_PLUGIN(GetLightTraitNumeric, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightTraitNumeric, , 0, 3, kParams_JIP_OneForm_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetLightFlag, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetLightFlag, , 0, 3, kParams_JIP_OneForm_TwoInts);

bool Cmd_GetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH *light;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &light, &traitID) || NOT_ID(light, TESObjectLIGH)) return true;
	switch (traitID)
	{
	case 0:
		*result = (int)light->radius;
		break;
	case 1:
		*result = light->red;
		break;
	case 2:
		*result = light->green;
		break;
	case 3:
		*result = light->blue;
		break;
	case 4:
		*result = light->falloffExp;
		break;
	case 5:
		*result = light->FOV;
		break;
	case 6:
		*result = light->fadeValue;
	}
	return true;
}

__declspec(naked) void __fastcall MarkLightModified(TESObjectLIGH *lightForm)
{
	__asm
	{
		mov		eax, offset s_activePtLights
		mov		edx, [eax+4]
		test	edx, edx
		jz		done
		push	esi
		mov		esi, [eax]
		ALIGN 16
	iterHead:
		dec		edx
		js		iterEnd
		mov		eax, [esi]
		add		esi, 4
		cmp		[eax+0xE8], ecx
		jnz		iterHead
		mov		[eax+0x9E], 1
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		esi
	done:
		retn
	}
}

bool Cmd_SetLightTraitNumeric_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light;
	UInt32 traitID;
	float val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &light, &traitID, &val) || NOT_ID(light, TESObjectLIGH))
		return true;
	UInt32 intVal = (int)val;
	switch (traitID)
	{
		case 0:
			light->radius = intVal;
			break;
		case 1:
			light->red = intVal;
			break;
		case 2:
			light->green = intVal;
			break;
		case 3:
			light->blue = intVal;
			break;
		case 4:
			light->falloffExp = val;
			goto skipMarkModified;
		case 5:
			light->FOV = val;
			goto skipMarkModified;
		case 6:
			light->fadeValue = val;
			break;
		default:
			goto skipMarkModified;
	}
	MarkLightModified(light);
skipMarkModified:
	return true;
}

bool Cmd_GetLightFlag_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &light, &flagID) && IS_ID(light, TESObjectLIGH) && (flagID <= 10))
		*result = (light->lightFlags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetLightFlag_Execute(COMMAND_ARGS)
{
	TESObjectLIGH *light;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &light, &flagID, &val) && IS_ID(light, TESObjectLIGH) && (flagID <= 10))
	{
		flagID = 1 << flagID;
		if (val) light->lightFlags |= flagID;
		else light->lightFlags &= ~flagID;
		MarkLightModified(light);
	}
	return true;
}