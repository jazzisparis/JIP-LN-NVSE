#pragma once

DEFINE_COMMAND_PLUGIN(GetFactionReputationType, , 0, 1, kParams_JIP_OneFaction);
DEFINE_COMMAND_PLUGIN(SetFactionReputationType, , 0, 2, kParams_JIP_OneFaction_OneOptionalReputation);

bool Cmd_GetFactionReputationType_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESFaction *faction;
	if (ExtractArgs(EXTRACT_ARGS, &faction) && faction->reputation) REFR_RES = faction->reputation->refID;
	return true;
}

bool Cmd_SetFactionReputationType_Execute(COMMAND_ARGS)
{
	TESFaction *faction;
	TESReputation *reputation = NULL;
	if (ExtractArgs(EXTRACT_ARGS, &faction, &reputation))
		faction->reputation = reputation;
	return true;
}