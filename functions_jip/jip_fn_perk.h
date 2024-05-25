#pragma once

DEFINE_COMMAND_PLUGIN(GetPerkFlag, 0, kParams_OnePerk_OneInt);
DEFINE_COMMAND_PLUGIN(SetPerkFlag, 0, kParams_OnePerk_TwoInts);

bool Cmd_GetPerkFlag_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &flagID))
	{
		bool isSet = false;
		switch (flagID)
		{
			case 0:
				isSet = perk->data.isTrait;
				break;
			case 1:
				isSet = perk->data.isPlayable;
				break;
			case 2:
				isSet = perk->data.isHidden;
		}
		if (isSet)
			*result = 1;
	}
	return true;
}

bool Cmd_SetPerkFlag_Execute(COMMAND_ARGS)
{
	BGSPerk *perk;
	UInt32 flagID, value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &perk, &flagID, &value))
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