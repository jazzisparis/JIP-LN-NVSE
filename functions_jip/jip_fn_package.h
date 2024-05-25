#pragma once

DEFINE_COMMAND_PLUGIN(GetPackageFlag, 0, kParams_OneAIPackage_OneInt);
DEFINE_COMMAND_PLUGIN(SetPackageFlag, 0, kParams_OneAIPackage_TwoInts);

bool Cmd_GetPackageFlag_Execute(COMMAND_ARGS)
{
	TESPackage *package;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &package, &flagID) && (flagID <= 31) && (package->packageFlags & (1 << flagID)))
		*result = 1;
	return true;
}

bool Cmd_SetPackageFlag_Execute(COMMAND_ARGS)
{
	TESPackage *package;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &package, &flagID, &val) && (flagID <= 31))
	{
		flagID = 1 << flagID;
		if (val) package->packageFlags |= flagID;
		else package->packageFlags &= ~flagID;
	}
	return true;
}