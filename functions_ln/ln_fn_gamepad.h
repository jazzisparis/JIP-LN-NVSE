#pragma once

DEFINE_COMMAND_PLUGIN(GetController, 0, nullptr);
DEFINE_CMD_COND_PLUGIN(IsButtonPressed, 0, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(GetPressedButtons, GetButtons, 0, nullptr);
DEFINE_CMD_ALT_COND_PLUGIN(GetLeftStickX, GetLSX, 0, nullptr);
DEFINE_CMD_ALT_COND_PLUGIN(GetLeftStickY, GetLSY, 0, nullptr);
DEFINE_CMD_ALT_COND_PLUGIN(GetRightStickX, GetRSX, 0, nullptr);
DEFINE_CMD_ALT_COND_PLUGIN(GetRightStickY, GetRSY, 0, nullptr);
DEFINE_CMD_ALT_COND_PLUGIN(GetLeftTrigger, GetLTr, 0, nullptr);
DEFINE_CMD_ALT_COND_PLUGIN(GetRightTrigger, GetRTr, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetDeadZoneLS, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetDeadZoneRS, 0, nullptr);
DEFINE_COMMAND_PLUGIN(SetDeadZoneLS, 0, kParams_OneDouble);
DEFINE_COMMAND_PLUGIN(SetDeadZoneRS, 0, kParams_OneDouble);
DEFINE_COMMAND_PLUGIN(EnableButton, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(DisableButton, 0, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsButtonDisabled, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(HoldButton, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ReleaseButton, 0, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsButtonHeld, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(EnableTrigger, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(DisableTrigger, 0, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsTriggerDisabled, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(HoldTrigger, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(ReleaseTrigger, 0, kParams_OneInt);
DEFINE_CMD_COND_PLUGIN(IsTriggerHeld, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(TapButton, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(TapTrigger, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetOnTriggerDownEventHandler, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetOnTriggerUpEventHandler, 0, kParams_OneForm_TwoInts);
DEFINE_CMD_COND_PLUGIN(IsStickDisabled, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetStickDisabled, 0, kParams_TwoInts);

bool Cmd_GetController_Execute(COMMAND_ARGS)
{
	*result = s_controllerReady;
	DoConsolePrint(result);
	return true;
}

bool Cmd_IsButtonPressed_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button) && (s_gamePad.wButtons & (UInt16)button))
		*result = 1;
	return true;
}

bool Cmd_IsButtonPressed_Eval(COMMAND_ARGS_EVAL)
{
	if (s_gamePad.wButtons & (UInt16)arg1)
		*result = 1;
	return true;
}

bool Cmd_GetPressedButtons_Execute(COMMAND_ARGS)
{
	*result = s_gamePad.wButtons;
	return true;
}

__declspec(naked) double __vectorcall GetLeftStick(SInt16 value)
{
	__asm
	{
		mov		eax, ecx
		cwde
		test	eax, eax
		jz		retnZero
		js		isNeg
		sub		eax, s_deadZoneLS
		jg		done
	retnZero:
		xorps	xmm0, xmm0
		retn
	isNeg:
		inc		eax
		add		eax, s_deadZoneLS
		jge		retnZero
	done:
		cvtsi2sd	xmm0, eax
		mulsd	xmm0, s_deadZoneLSd
		retn
	}
}

__declspec(naked) double __vectorcall GetRightStick(SInt16 value)
{
	__asm
	{
		mov		eax, ecx
		cwde
		test	eax, eax
		jz		retnZero
		js		isNeg
		sub		eax, s_deadZoneRS
		jg		done
	retnZero:
		xorps	xmm0, xmm0
		retn
	isNeg:
		inc		eax
		add		eax, s_deadZoneRS
		jge		retnZero
	done:
		cvtsi2sd	xmm0, eax
		mulsd	xmm0, s_deadZoneRSd
		retn
	}
}

bool Cmd_GetLeftStickX_Execute(COMMAND_ARGS)
{
	*result = GetLeftStick(s_gamePad.sThumbLX);
	return true;
}

bool Cmd_GetLeftStickX_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetLeftStick(s_gamePad.sThumbLX);
	return true;
}

bool Cmd_GetLeftStickY_Execute(COMMAND_ARGS)
{
	*result = GetLeftStick(s_gamePad.sThumbLY);
	return true;
}

bool Cmd_GetLeftStickY_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetLeftStick(s_gamePad.sThumbLY);
	return true;
}

bool Cmd_GetRightStickX_Execute(COMMAND_ARGS)
{
	*result = GetRightStick(s_gamePad.sThumbRX);
	return true;
}

bool Cmd_GetRightStickX_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetRightStick(s_gamePad.sThumbRX);
	return true;
}

bool Cmd_GetRightStickY_Execute(COMMAND_ARGS)
{
	*result = GetRightStick(s_gamePad.sThumbRY);
	return true;
}

bool Cmd_GetRightStickY_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetRightStick(s_gamePad.sThumbRY);
	return true;
}

bool Cmd_GetLeftTrigger_Execute(COMMAND_ARGS)
{
	*result = s_gamePad.bLeftTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetLeftTrigger_Eval(COMMAND_ARGS_EVAL)
{
	*result = s_gamePad.bLeftTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetRightTrigger_Execute(COMMAND_ARGS)
{
	*result = s_gamePad.bRightTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetRightTrigger_Eval(COMMAND_ARGS_EVAL)
{
	*result = s_gamePad.bRightTrigger * (1 / 255.0);
	return true;
}

bool Cmd_GetDeadZoneLS_Execute(COMMAND_ARGS)
{
	*result = s_deadZoneLSg * (1 / 32767.0);
	DoConsolePrint(result);
	return true;
}

bool Cmd_GetDeadZoneRS_Execute(COMMAND_ARGS)
{
	*result = s_deadZoneRSg * (1 / 32767.0);
	DoConsolePrint(result);
	return true;
}

bool Cmd_SetDeadZoneLS_Execute(COMMAND_ARGS)
{
	double deadzone;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &deadzone))
	{
		int value;
		if (deadzone <= 0.1)
			value = 3277;
		else if (deadzone >= 0.9)
			value = 29490;
		else
			value = deadzone * 32767;
		s_deadZoneLS = value;
		s_deadZoneLSg = value;
		s_deadZoneLSd = 1.0 / (32767 - value);
		for (UInt32 patchAddr : {0x717B90, 0x717BA5, 0x717C15, 0x717C2A, 0x755552, 0x755569, 0x7AED31, 0x7E7F82, 0x7ECA56, 0x941157, 0x941177, 0xA23E5E, 0xA23E87})
			SafeWrite32(patchAddr, value);
		value = -value;
		for (UInt32 patchAddr : {0x717BBE, 0x717BD3, 0x717BEA, 0x717BFE, 0xA23E73, 0xA23E9C})
			SafeWrite32(patchAddr, value);
	}
	return true;
}

bool Cmd_SetDeadZoneRS_Execute(COMMAND_ARGS)
{
	double deadzone;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &deadzone))
	{
		int value;
		if (deadzone <= 0.1)
			value = 3277;
		else if (deadzone >= 0.9)
			value = 29490;
		else
			value = deadzone * 32767;
		s_deadZoneRS = value;
		s_deadZoneRSg = value;
		s_deadZoneRSd = 1.0 / (32767 - value);
		for (UInt32 patchAddr : {0x7AE57A, 0x7AE598, 0x7AE666, 0x7AE687, 0x7AE699, 0x7AE6AE, 0x7AE6C0, 0x7D57C0, 0x7D57D3, 0x7E7FAD, 0x7ECAAC, 0x7ECB3A, 0x7ECB51, 0x941197, 0x9455F5, 0x945644, 0x9456F0, 0x945710})
			SafeWrite32(patchAddr, value);
		value = -value;
		SafeWrite32(0x7ECAEE, value);
	}
	return true;
}

bool Cmd_EnableButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonSkip |= button;
	return true;
}

bool Cmd_DisableButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonSkip &= ~button;
	return true;
}

bool Cmd_IsButtonDisabled_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button) && !(s_XIStateMods.buttonSkip & button))
		*result = 1;
	return true;
}

bool Cmd_IsButtonDisabled_Eval(COMMAND_ARGS_EVAL)
{
	if (!(s_XIStateMods.buttonSkip & (UInt16)arg1))
		*result = 1;
	return true;
}

bool Cmd_HoldButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonHold |= button;
	return true;
}

bool Cmd_ReleaseButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonHold &= ~button;
	return true;
}

bool Cmd_IsButtonHeld_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button) && (s_XIStateMods.buttonHold & button))
		*result = 1;
	return true;
}

bool Cmd_IsButtonHeld_Eval(COMMAND_ARGS_EVAL)
{
	if (s_XIStateMods.buttonHold & (UInt16)arg1)
		*result = 1;
	return true;
}

bool Cmd_EnableTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger))
		s_XIStateMods.triggerMods &= (trigger ? 0xFD : 0xFE);
	return true;
}

bool Cmd_DisableTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger))
		s_XIStateMods.triggerMods |= (trigger ? 2 : 1);
	return true;
}

bool Cmd_IsTriggerDisabled_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (s_XIStateMods.triggerMods & (trigger ? 2 : 1)))
		*result = 1;
	return true;
}

bool Cmd_IsTriggerDisabled_Eval(COMMAND_ARGS_EVAL)
{
	if (s_XIStateMods.triggerMods & (arg1 ? 2 : 1))
		*result = 1;
	return true;
}

bool Cmd_HoldTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger))
		s_XIStateMods.triggerMods |= (trigger ? 8 : 4);
	return true;
}

bool Cmd_ReleaseTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger))
		s_XIStateMods.triggerMods &= (trigger ? 0xF7 : 0xFB);
	return true;
}

bool Cmd_IsTriggerHeld_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger) && (s_XIStateMods.triggerMods & (trigger ? 8 : 4)))
		*result = 1;
	return true;
}

bool Cmd_IsTriggerHeld_Eval(COMMAND_ARGS_EVAL)
{
	if (s_XIStateMods.triggerMods & (arg1 ? 8 : 4))
		*result = 1;
	return true;
}

bool Cmd_TapButton_Execute(COMMAND_ARGS)
{
	UInt32 button;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &button))
		s_XIStateMods.buttonTap |= button;
	return true;
}

bool Cmd_TapTrigger_Execute(COMMAND_ARGS)
{
	UInt32 trigger;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &trigger))
		s_XIStateMods.triggerMods |= (trigger ? 0x20 : 0x10);
	return true;
}

bool SetOnTriggerEventHandler_Execute(COMMAND_ARGS)
{
	CAPTURE_ECX(eventType)
	Script *script;
	UInt32 addEvnt, rTrigger;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &rTrigger) || NOT_ID(script, Script) || !s_controllerReady)
		return true;
	rTrigger = ((rTrigger & 1) << 1) + eventType;
	UInt32 eventMask = eventType ? kLNEventMask_OnTriggerDown : kLNEventMask_OnTriggerUp;
	if (addEvnt)
	{
		if (s_LNOnTriggerEvents[rTrigger]().Insert(script))
			s_LNEventFlags |= eventMask;
	}
	else if (s_LNOnTriggerEvents[rTrigger]().Erase(script) && s_LNOnTriggerEvents[eventType]().Empty() && s_LNOnTriggerEvents[eventType + 2]().Empty())
		s_LNEventFlags &= ~eventMask;
	return true;
}

__declspec(naked) bool Cmd_SetOnTriggerDownEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		ecx, 1
		jmp		SetOnTriggerEventHandler_Execute
	}
}

__declspec(naked) bool Cmd_SetOnTriggerUpEventHandler_Execute(COMMAND_ARGS)
{
	__asm
	{
		xor		ecx, ecx
		jmp		SetOnTriggerEventHandler_Execute
	}
}

bool Cmd_IsStickDisabled_Execute(COMMAND_ARGS)
{
	UInt32 stick;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &stick) && (s_XIStateMods.stickSkip & (stick ? 0xF0 : 0xF)))
		*result = 1;
	return true;
}

bool Cmd_IsStickDisabled_Eval(COMMAND_ARGS_EVAL)
{
	if (s_XIStateMods.stickSkip & (arg1 ? 0xF0 : 0xF))
		*result = 1;
	return true;
}

bool Cmd_SetStickDisabled_Execute(COMMAND_ARGS)
{
	UInt32 stick, doSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &stick, &doSet))
	{
		UInt8 flag = stick ? 0xF0 : 0xF;
		if (doSet) s_XIStateMods.stickSkip |= flag;
		else s_XIStateMods.stickSkip &= ~flag;
	}
	return true;
}