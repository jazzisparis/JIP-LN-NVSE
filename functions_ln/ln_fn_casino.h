#pragma once

DEFINE_COMMAND_PLUGIN(GetCasinoMaxWinnings, 0, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoMaxWinnings, 0, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(GetCasinoBJPayout, 0, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoBJPayout, 0, kParams_OneCasino_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCasinoBJ17Stand, 0, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoBJ17Stand, 0, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(GetCasinoNumDecks, 0, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoNumDecks, 0, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(GetCasinoShufflePercent, 0, kParams_OneCasino);
DEFINE_COMMAND_PLUGIN(SetCasinoShufflePercent, 0, kParams_OneCasino_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCasinoReelStops, 0, kParams_OneCasino_OneInt);
DEFINE_COMMAND_PLUGIN(SetCasinoReelStops, 0, kParams_OneForm_TwoInts);

bool Cmd_GetCasinoMaxWinnings_Execute(COMMAND_ARGS)
{
	TESCasino *casino;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino))
		*result = (int)casino->maxWinnings;
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
	int reelStops = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &casino, &slot) && (slot > 0) && (slot < 8))
		reelStops = (int)casino->reelStops[slot - 1];
	*result = reelStops;
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