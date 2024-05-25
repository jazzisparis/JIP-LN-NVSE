#pragma once

DEFINE_COMMAND_PLUGIN(GetRaceVoice, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetRaceVoice, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(GetRaceAgeRace, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetRaceAgeRace, 0, kParams_OneForm_OneInt_OneForm);
DEFINE_COMMAND_PLUGIN(SetRace, 0, kParams_TwoForms);

bool Cmd_GetRaceVoice_Execute(COMMAND_ARGS)
{
	TESRace *race;
	UInt32 gender;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &race, &gender) && IS_ID(race, TESRace))
		if (BGSVoiceType *voice = race->voiceTypes[gender != 0])
			REFR_RES = voice->refID;
	return true;
}

bool Cmd_SetRaceVoice_Execute(COMMAND_ARGS)
{
	TESRace *race;
	UInt32 gender;
	BGSVoiceType *voice;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &race, &gender, &voice) && IS_ID(race, TESRace) && IS_ID(voice, BGSVoiceType))
		race->voiceTypes[gender != 0] = voice;
	return true;
}

bool Cmd_GetRaceAgeRace_Execute(COMMAND_ARGS)
{
	TESRace *race;
	UInt32 age;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &race, &age) && IS_ID(race, TESRace))
		if (race = race->ageRace[age != 0])
			REFR_RES = race->refID;
	return true;
}

bool Cmd_SetRaceAgeRace_Execute(COMMAND_ARGS)
{
	TESRace *race, *ageRace;
	UInt32 age;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &race, &age, &ageRace) && IS_ID(race, TESRace) && IS_ID(ageRace, TESRace))
		race->ageRace[age != 0] = ageRace;
	return true;
}

bool Cmd_SetRace_Execute(COMMAND_ARGS)
{
	TESNPC *npc;
	TESRace *race;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &npc, &race) && IS_ID(npc, TESNPC) && IS_ID(race, TESRace))
		npc->SetRace(race);
	return true;
}