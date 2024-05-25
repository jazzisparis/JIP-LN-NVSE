#pragma once

DEFINE_COMMAND_PLUGIN(GetTerminalLock, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetPasswordNote, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetPasswordNote, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetLockedOut, 0, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(SetLockedOut, 0, kParams_OneInt_OneOptionalObjectRef);

bool Cmd_GetTerminalLock_Execute(COMMAND_ARGS)
{
	BGSTerminal *terminal = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &terminal))
	{
		if (!terminal)
		{
			if (!thisObj) return true;
			terminal = (BGSTerminal*)thisObj->baseForm;
		}
		if IS_ID(terminal, BGSTerminal)
			* result = (terminal->data.terminalFlags & BGSTerminal::kTerminalFlagUnlocked) ? -1 : terminal->data.difficulty;
	}
	return true;
}

bool Cmd_GetPasswordNote_Execute(COMMAND_ARGS)
{
	BGSTerminal *terminal = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &terminal))
	{
		if (!terminal)
		{
			if (!thisObj) return true;
			terminal = (BGSTerminal*)thisObj->baseForm;
		}
		if (IS_ID(terminal, BGSTerminal) && terminal->password)
			REFR_RES = terminal->password->refID;
	}
	return true;
}

bool Cmd_SetPasswordNote_Execute(COMMAND_ARGS)
{
	BGSNote *note;
	BGSTerminal *terminal = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &note, &terminal) && IS_ID(note, BGSNote))
	{
		if (!terminal)
		{
			if (!thisObj) return true;
			terminal = (BGSTerminal*)thisObj->baseForm;
		}
		if IS_ID(terminal, BGSTerminal)
			terminal->password = note;
	}
	return true;
}

bool Cmd_GetLockedOut_Execute(COMMAND_ARGS)
{
	TESObjectREFR *refr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &refr))
	{
		if (!refr)
		{
			if (!thisObj) return true;
			refr = thisObj;
		}
		if (refr->flags & 0x100)
			*result = 3;
		else if IS_ID(refr->baseForm, BGSTerminal)
		{
			if (auto xTermState = GetExtraType(&refr->extraDataList, ExtraTerminalState))
				*result = xTermState->lockedOut & 0xF;
		}
		else if (auto xLock = GetExtraType(&refr->extraDataList, ExtraLock); xLock && xLock->data)
			*result = (int)(xLock->data->unk0C & 0xF);
		else if IS_ID(refr->baseForm, TESObjectDOOR)
			if (auto xTeleport = GetExtraType(&refr->extraDataList, ExtraTeleport); xTeleport && xTeleport->data && xTeleport->data->linkedDoor)
				if (xLock = GetExtraType(&xTeleport->data->linkedDoor->extraDataList, ExtraLock); xLock && xLock->data)
					*result = (int)(xLock->data->unk0C & 0xF);
	}
	return true;
}

bool Cmd_SetLockedOut_Execute(COMMAND_ARGS)
{
	UInt32 state;
	TESObjectREFR *refr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &state, &refr) && (state <= 2))
	{
		if (!refr)
		{
			if (!thisObj) return true;
			refr = thisObj;
		}
		if IS_ID(refr->baseForm, BGSTerminal)
		{
			auto xTermState = GetExtraType(&refr->extraDataList, ExtraTerminalState);
			if (!xTermState)
			{
				if (!state) return true;
				xTermState = (ExtraTerminalState*)refr->extraDataList.AddExtra(ExtraTerminalState::Create());
				xTermState->lockLevel = ((BGSTerminal*)refr->baseForm)->data.difficulty;
			}
			xTermState->lockedOut = state;
			refr->MarkModified(0x80000000);
		}
		else if (auto xLock = GetExtraType(&refr->extraDataList, ExtraLock); xLock && xLock->data)
		{
			xLock->data->unk0C = state;
			refr->MarkModified(0x1000);
		}
		else
		{
			if IS_ID(refr->baseForm, TESObjectDOOR)
				if (auto xTeleport = GetExtraType(&refr->extraDataList, ExtraTeleport); xTeleport && xTeleport->data && xTeleport->data->linkedDoor)
				{
					refr = xTeleport->data->linkedDoor;
					if (xLock = GetExtraType(&refr->extraDataList, ExtraLock); xLock && xLock->data)
					{
						xLock->data->unk0C = state;
						refr->MarkModified(0x1000);
						return true;
					}
				}
			if (state)
			{
				xLock = (ExtraLock*)refr->extraDataList.AddExtra(ExtraLock::Create());
				xLock->data->unk0C = state;
				refr->MarkModified(0x1000);
			}
		}
	}
	return true;
}