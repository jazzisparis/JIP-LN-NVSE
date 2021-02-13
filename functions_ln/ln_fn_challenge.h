#pragma once

DEFINE_COMMAND_PLUGIN(GetChallengeType, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetChallengeFlags, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeFlags, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeThreshold, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeThreshold, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeInterval, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeInterval, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeValue1, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeValue1, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeValue2, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeValue2, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeValue3, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeValue3, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetChallengeForm1, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeForm1, , 0, 2, kParams_JIP_TwoForms);
DEFINE_COMMAND_PLUGIN(GetChallengeForm2, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetChallengeForm2, , 0, 2, kParams_JIP_TwoForms);

bool Cmd_GetChallengeType_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = (int)challenge->data.type;
	else *result = -1;
	return true;
}

bool Cmd_GetChallengeFlags_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = (int)challenge->data.flags;
	else *result = 0;
	return true;
}

bool Cmd_SetChallengeFlags_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->data.flags = value;
	return true;
}

bool Cmd_GetChallengeThreshold_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = (int)challenge->data.threshold;
	else *result = 0;
	return true;
}

bool Cmd_SetChallengeThreshold_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->data.threshold = value;
	return true;
}

bool Cmd_GetChallengeInterval_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = (int)challenge->data.interval;
	else *result = 0;
	return true;
}

bool Cmd_SetChallengeInterval_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->data.interval = value;
	return true;
}

bool Cmd_GetChallengeValue1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = challenge->data.value1;
	else *result = 0;
	return true;
}

bool Cmd_SetChallengeValue1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->data.value1 = value;
	return true;
}

bool Cmd_GetChallengeValue2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = challenge->data.value2;
	else *result = 0;
	return true;
}

bool Cmd_SetChallengeValue2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->data.value2 = value;
	return true;
}

bool Cmd_GetChallengeValue3_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge))
		*result = (int)challenge->data.value3;
	else *result = 0;
	return true;
}

bool Cmd_SetChallengeValue3_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->data.value3 = value;
	return true;
}

bool Cmd_GetChallengeForm1_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge) && challenge->SNAM)
		REFR_RES = challenge->SNAM->refID;
	return true;
}

bool Cmd_SetChallengeForm1_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	TESForm *value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->SNAM = value;
	return true;
}

bool Cmd_GetChallengeForm2_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESChallenge *challenge;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge) && IS_ID(challenge, TESChallenge) && challenge->XNAM)
		REFR_RES = challenge->XNAM->refID;
	return true;
}

bool Cmd_SetChallengeForm2_Execute(COMMAND_ARGS)
{
	TESChallenge *challenge;
	TESForm *value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &challenge, &value) && IS_ID(challenge, TESChallenge))
		challenge->XNAM = value;
	return true;
}