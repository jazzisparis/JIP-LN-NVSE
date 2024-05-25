#pragma once

DEFINE_COMMAND_PLUGIN(GetChallengeType, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(GetChallengeFlags, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeFlags, 0, kParams_OneChallenge_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeThreshold, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeThreshold, 0, kParams_OneChallenge_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeInterval, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeInterval, 0, kParams_OneChallenge_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeValue1, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeValue1, 0, kParams_OneChallenge_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeValue2, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeValue2, 0, kParams_OneChallenge_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeValue3, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeValue3, 0, kParams_OneChallenge_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeForm1, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeForm1, 0, kParams_OneChallenge_OneForm);
DEFINE_COMMAND_PLUGIN(GetChallengeForm2, 0, kParams_OneChallenge);
DEFINE_COMMAND_PLUGIN(SetChallengeForm2, 0, kParams_OneChallenge_OneForm);

bool Cmd_GetChallengeType_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	int type = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		type = (int)challenge->data.type;
	*result = type;
	return true;
}

bool Cmd_GetChallengeFlags_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		*result = (int)challenge->data.flags;
	return true;
}

bool Cmd_SetChallengeFlags_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->data.flags = value;
	return true;
}

bool Cmd_GetChallengeThreshold_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		*result = (int)challenge->data.threshold;
	return true;
}

bool Cmd_SetChallengeThreshold_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->data.threshold = value;
	return true;
}

bool Cmd_GetChallengeInterval_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		*result = (int)challenge->data.interval;
	return true;
}

bool Cmd_SetChallengeInterval_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->data.interval = value;
	return true;
}

bool Cmd_GetChallengeValue1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		*result = challenge->data.value1;
	return true;
}

bool Cmd_SetChallengeValue1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->data.value1 = value;
	return true;
}

bool Cmd_GetChallengeValue2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		*result = challenge->data.value2;
	return true;
}

bool Cmd_SetChallengeValue2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->data.value2 = value;
	return true;
}

bool Cmd_GetChallengeValue3_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge))
		*result = (int)challenge->data.value3;
	return true;
}

bool Cmd_SetChallengeValue3_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->data.value3 = value;
	return true;
}

bool Cmd_GetChallengeForm1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && challenge->SNAM)
		REFR_RES = challenge->SNAM->refID;
	return true;
}

bool Cmd_SetChallengeForm1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	TESForm *value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->SNAM = value;
	return true;
}

bool Cmd_GetChallengeForm2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && challenge->XNAM)
		REFR_RES = challenge->XNAM->refID;
	return true;
}

bool Cmd_SetChallengeForm2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	TESForm *value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value))
		challenge->XNAM = value;
	return true;
}