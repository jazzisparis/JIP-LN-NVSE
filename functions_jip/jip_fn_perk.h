#pragma once

DEFINE_COMMAND_PLUGIN(GetPerkFlag, , 0, 2, kParams_JIP_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetPerkFlag, , 0, 3, kParams_JIP_OnePerk_TwoInts);

bool Cmd_GetPerkFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	BGSPerk *perk;
	UInt32 flagID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &flagID)) return true;
	switch (flagID)
	{
		case 0:
			*result = perk->data.isTrait;
			break;
		case 1:
			*result = perk->data.isPlayable;
			break;
		case 2:
			*result = perk->data.isHidden;
	}
	return true;
}

bool Cmd_SetPerkFlag_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 flagID, value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &flagID, &value)) return true;
	switch (flagID)
	{
		case 0:
			perk->data.isTrait = (value != 0);
			break;
		case 1:
			perk->data.isPlayable = (value != 0);
			break;
		case 2:
			perk->data.isHidden = (value != 0);
	}
	return true;
}