#pragma once

DEFINE_COMMAND_PLUGIN(GetTerminalLock, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetPasswordNote, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetPasswordNote, , 0, 2, kParams_JIP_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetLockedOut, , 0, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(SetLockedOut, , 0, 2, kParams_JIP_OneInt_OneOptionalObjectRef);

bool Cmd_GetTerminalLock_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSTerminal *terminal = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &terminal)) return true;
	if (!terminal)
	{
		if (!thisObj) return true;
		terminal = (BGSTerminal*)thisObj->baseForm;
	}
	if IS_ID(terminal, BGSTerminal)
		*result = (terminal->data.terminalFlags & BGSTerminal::kTerminalFlagUnlocked) ? -1 : terminal->data.difficulty;
	return true;
}

bool Cmd_GetPasswordNote_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSTerminal *terminal = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &terminal)) return true;
	if (!terminal)
	{
		if (!thisObj) return true;
		terminal = (BGSTerminal*)thisObj->baseForm;
	}
	if (IS_ID(terminal, BGSTerminal) && terminal->password)
		REFR_RES = terminal->password->refID;
	return true;
}

bool Cmd_SetPasswordNote_Execute(COMMAND_ARGS)
{
	BGSNote *note;
	BGSTerminal *terminal = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &note, &terminal) || NOT_ID(note, BGSNote)) return true;
	if (!terminal)
	{
		if (!thisObj) return true;
		terminal = (BGSTerminal*)thisObj->baseForm;
	}
	if IS_ID(terminal, BGSTerminal)
		terminal->password = note;
	return true;
}

bool Cmd_GetLockedOut_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *refr = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &refr)) return true;
	if (!refr)
	{
		if (!thisObj) return true;
		refr = thisObj;
	}
	if (refr->flags & 0x100) *result = 3;
	else
	{
		ExtraLock *xLock = GetExtraType(&refr->extraDataList, Lock);
		if (xLock && xLock->data) *result = (int)(xLock->data->unk0C & 0xF);
		else
		{
			ExtraTerminalState *xTerm = GetExtraType(&refr->extraDataList, TerminalState);
			if (xTerm) *result = xTerm->lockedOut & 0xF;
		}
	}
	return true;
}

bool Cmd_SetLockedOut_Execute(COMMAND_ARGS)
{
	UInt32 state;
	TESObjectREFR *refr = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &state, &refr) || (state > 2)) return true;
	if (!refr)
	{
		if (!thisObj) return true;
		refr = thisObj;
	}
	if IS_ID(refr->baseForm, BGSTerminal)
	{
		ExtraTerminalState *xTerm = GetExtraType(&refr->extraDataList, TerminalState);
		if (!xTerm)
		{
			xTerm = ExtraTerminalState::Create();
			refr->extraDataList.AddExtra(xTerm);
		}
		xTerm->lockedOut = state;
	}
	else
	{
		ExtraLock *xLock = GetExtraType(&refr->extraDataList, Lock);
		if (!xLock)
		{
			xLock = ExtraLock::Create();
			refr->extraDataList.AddExtra(xLock);
		}
		xLock->data->unk0C = state;
	}
	return true;
}