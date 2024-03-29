#pragma once

DEFINE_COMMAND_PLUGIN(GetCasinoMaxWinnings, 0, 1, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoMaxWinnings, 0, 2, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(GetCasinoBJPayout, 0, 1, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoBJPayout, 0, 2, kParams_OneCasino_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCasinoBJ17Stand, 0, 1, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoBJ17Stand, 0, 2, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(GetCasinoNumDecks, 0, 1, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoNumDecks, 0, 2, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(GetCasinoShufflePercent, 0, 1, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoShufflePercent, 0, 2, kParams_OneCasino_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCasinoReelStops, 0, 2, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(SetCasinoReelStops, 0, 3, kParams_OneForm_TwoInts);

bool Cmd_GetCasinoMaxWinnings_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino))
		*result = (int)casino->maxWinnings;
	else *result = 0;
	return true;
}

bool Cmd_SetCasinoMaxWinnings_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &value))
		casino->maxWinnings = value;
	return true;
}

bool Cmd_GetCasinoBJPayout_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino))
		*result = casino->blackjackPayout;
	else *result = 0;
	return true;
}

bool Cmd_SetCasinoBJPayout_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &value))
		casino->blackjackPayout = value;
	return true;
}

bool Cmd_GetCasinoBJ17Stand_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino))
		*result = (int)casino->casinoFlags;
	else *result = 0;
	return true;
}

bool Cmd_SetCasinoBJ17Stand_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &value))
		casino->casinoFlags = value != 0;
	return true;
}

bool Cmd_GetCasinoNumDecks_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino))
		*result = (int)casino->numDecks;
	else *result = 0;
	return true;
}

bool Cmd_SetCasinoNumDecks_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &value))
		casino->numDecks = value;
	return true;
}

bool Cmd_GetCasinoShufflePercent_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino))
		*result = casino->shufflePercent;
	else *result = 0;
	return true;
}

bool Cmd_SetCasinoShufflePercent_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &value))
		casino->shufflePercent = value;
	return true;
}

bool Cmd_GetCasinoReelStops_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	UInt32 slot;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &slot) && (slot > 0) && (slot < 8))
		*result = (int)casino->reelStops[slot - 1];
	else *result = -1;
	return true;
}

bool Cmd_SetCasinoReelStops_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	UInt32 slot, value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &slot, &value) && IS_ID(casino, TESCasino) && (slot > 0) && (slot < 8))
		casino->reelStops[slot - 1] = value;
	return true;
}