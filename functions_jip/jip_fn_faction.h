#pragma once

DEFINE_COMMAND_PLUGIN(GetFactionReputationType, 0, kParams_OneFaction);
DEFINE_COMMAND_PLUGIN(SetFactionReputationType, 0, kParams_OneFaction_OneOptionalReputation);

bool Cmd_GetFactionReputationType_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &faction) && faction->reputation)
		REFR_RES = faction->reputation->refID;
	return true;
}

bool Cmd_SetFactionReputationType_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	TESReputation *reputation = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &faction, &reputation))
		faction->reputation = reputation;
	return true;
}