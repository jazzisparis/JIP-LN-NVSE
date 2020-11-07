#pragma once

DEFINE_COMMAND_PLUGIN(GetRaceVoice, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetRaceVoice, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetRaceAgeRace, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetRaceAgeRace, , 0, 3, kParams_JIP_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetRace, , 0, 2, kParams_JIP_TwoForms);

bool Cmd_GetRaceVoice_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRace *race;
	UInt32 gender;
	if (ExtractArgs(EXTRACT_ARGS, &race, &gender) && IS_TYPE(race, TESRace))
	{
		BGSVoiceType *voice = race->voiceTypes[gender != 0];
		if (voice) REFR_RES = voice->refID;
	}
	return true;
}

bool Cmd_SetRaceVoice_Execute(COMMAND_ARGS)
{
	TESRace *race;
	UInt32 gender;
	BGSVoiceType *voice;
	if (ExtractArgs(EXTRACT_ARGS, &race, &gender, &voice) && IS_TYPE(race, TESRace) && IS_TYPE(voice, BGSVoiceType))
		race->voiceTypes[gender != 0] = voice;
	return true;
}

bool Cmd_GetRaceAgeRace_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESRace *race;
	UInt32 age;
	if (ExtractArgs(EXTRACT_ARGS, &race, &age) && IS_TYPE(race, TESRace))
	{
		race = race->ageRace[age != 0];
		if (race) REFR_RES = race->refID;
	}
	return true;
}

bool Cmd_SetRaceAgeRace_Execute(COMMAND_ARGS)
{
	TESRace *race, *ageRace;
	UInt32 age;
	if (ExtractArgs(EXTRACT_ARGS, &race, &age, &ageRace) && IS_TYPE(race, TESRace) && IS_TYPE(ageRace, TESRace))
		race->ageRace[age != 0] = ageRace;
	return true;
}

bool Cmd_SetRace_Execute(COMMAND_ARGS)
{
	TESNPC *npc;
	TESRace *race;
	if (ExtractArgs(EXTRACT_ARGS, &npc, &race) && IS_TYPE(npc, TESNPC) && IS_TYPE(race, TESRace))
		npc->SetRace(race);
	return true;
}