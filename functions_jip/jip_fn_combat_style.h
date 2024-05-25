#pragma once

DEFINE_COMMAND_PLUGIN(GetCombatStyleTraitNumeric, 0, kParams_OneCombatStyle_OneInt);
DEFINE_COMMAND_PLUGIN(SetCombatStyleTraitNumeric, 0, kParams_OneCombatStyle_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCombatStyleFlag, 0, kParams_OneCombatStyle_OneInt);
DEFINE_COMMAND_PLUGIN(SetCombatStyleFlag, 0, kParams_OneCombatStyle_TwoInts);

bool Cmd_GetCombatStyleTraitNumeric_Execute(COMMAND_ARGS)
{
	TESCombatStyle *cStyle;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cStyle, &traitID) && (traitID <= 63))
		switch (traitID)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				*result = ((float*)cStyle)[6 + traitID];
				break;
			case 9:
				*result = cStyle->weaponRestrictions;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				*result = ((float*)cStyle)[7 + traitID];
				break;
			case 15:
			case 16:
				*result = ((UInt8*)cStyle)[0x49 + traitID];
				break;
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
				*result = ((float*)cStyle)[6 + traitID];
				break;
			case 25:
			case 26:
				*result = ((UInt8*)cStyle)[0x63 + traitID];
				break;
			case 27:
			case 28:
			case 29:
				*result = ((float*)cStyle)[5 + traitID];
				break;
			case 30:
				*result = cStyle->powerAttackChance;
				break;
			case 31:
			case 32:
				*result = ((float*)cStyle)[5 + traitID];
				break;
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
				*result = ((UInt8*)cStyle)[0x77 + traitID];
				break;
			case 38:
			case 39:
				*result = ((float*)cStyle)[2 + traitID];
				break;
			case 40:
			case 41:
				*result = ((UInt8*)cStyle)[0x84 + traitID];
				break;
			default:
				*result = ((float*)cStyle)[2 + traitID];
				break;
		}
	return true;
}

bool Cmd_SetCombatStyleTraitNumeric_Execute(COMMAND_ARGS)
{
	TESCombatStyle *cStyle;
	UInt32 traitID;
	float fVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cStyle, &traitID, &fVal) && (traitID <= 63))
	{
		UInt8 bVal = (int)fVal;
		switch (traitID)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				((float*)cStyle)[6 + traitID] = fVal;
				break;
			case 9:
				if (bVal && (bVal <= 2)) cStyle->weaponRestrictions = bVal;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
				((float*)cStyle)[7 + traitID] = fVal;
				break;
			case 15:
			case 16:
				((UInt8*)cStyle)[0x49 + traitID] = bVal;
				break;
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
				((float*)cStyle)[6 + traitID] = fVal;
				break;
			case 25:
			case 26:
				((UInt8*)cStyle)[0x63 + traitID] = bVal;
				break;
			case 27:
			case 28:
			case 29:
				((float*)cStyle)[5 + traitID] = fVal;
				break;
			case 30:
				cStyle->powerAttackChance = bVal;
				break;
			case 31:
			case 32:
				((float*)cStyle)[5 + traitID] = fVal;
				break;
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
				((UInt8*)cStyle)[0x77 + traitID] = bVal;
				break;
			case 38:
			case 39:
				((float*)cStyle)[2 + traitID] = fVal;
				break;
			case 40:
			case 41:
				((UInt8*)cStyle)[0x84 + traitID] = bVal;
				break;
			default:
				((float*)cStyle)[2 + traitID] = fVal;
				break;
		}
	}
	return true;
}

bool Cmd_GetCombatStyleFlag_Execute(COMMAND_ARGS)
{
	TESCombatStyle *cStyle;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cStyle, &flagID) && (flagID <= 15) && (cStyle->csFlags & (1 << flagID)))
		*result = 1;
	return true;
}

bool Cmd_SetCombatStyleFlag_Execute(COMMAND_ARGS)
{
	TESCombatStyle *cStyle;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cStyle, &flagID, &val) && (flagID <= 15))
		cStyle->SetFlag(1 << flagID, val != 0);
	return true;
}