#pragma once

DEFINE_COMMAND_PLUGIN(GetSoundTraitNumeric, , 0, 2, kParams_JIP_OneSound_OneInt);
DEFINE_COMMAND_PLUGIN(SetSoundTraitNumeric, , 0, 3, kParams_JIP_OneSound_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetSoundFlag, , 0, 2, kParams_JIP_OneSound_OneInt);
DEFINE_COMMAND_PLUGIN(SetSoundFlag, , 0, 3, kParams_JIP_OneSound_TwoInts);
DEFINE_COMMAND_PLUGIN(GetSoundSourceFile, , 0, 1, kParams_JIP_OneSound);
DEFINE_COMMAND_PLUGIN(SetSoundSourceFile, , 0, 2, kParams_JIP_OneSound_OneString);
DEFINE_COMMAND_PLUGIN(IsSoundPlaying, , 0, 2, kParams_JIP_OneSound_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetSoundPlayers, , 0, 1, kParams_JIP_OneSound);
DEFINE_COMMAND_PLUGIN(StopSound, , 0, 1, kParams_JIP_OneSound);
DEFINE_COMMAND_PLUGIN(IsMusicPlaying, , 0, 1, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetMusicState, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetGameVolume, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetGameVolume, , 0, 2, kParams_OneInt_OneOptionalInt);

bool Cmd_GetSoundTraitNumeric_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *sound;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &traitID)) return true;
	switch (traitID)
	{
	case 0:
		*result = sound->minAttenuationDist * 5.0;
		break;
	case 1:
		*result = sound->maxAttenuationDist * 100.0;
		break;
	case 2:
		*result = sound->frequencyAdj;
		break;
	case 3:
		*result = sound->staticAttenuation / -100.0;
		break;
	case 4:
		*result = sound->endsAt * 5.62353;
		break;
	case 5:
		*result = sound->startsAt * 5.62353;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		*result = sound->attenuationCurve[traitID - 6];
		break;
	case 11:
		*result = sound->reverbAttenuation;
		break;
	case 12:
		*result = (int)sound->priority;
	}
	return true;
}

bool Cmd_SetSoundTraitNumeric_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	UInt32 traitID;
	float val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &traitID, &val)) return true;
	int intVal = (int)val;
	BSGameSound *gameSound;
	switch (traitID)
	{
	case 0:
		if (intVal > 1275)
			intVal = 255;
		else if (intVal < 0)
			intVal = 0;
		else intVal /= 5;
		sound->minAttenuationDist = intVal;
		break;
	case 1:
		if (intVal > 25500)
			intVal = 255;
		else if (intVal < 0)
			intVal = 0;
		else intVal /= 100;
		sound->maxAttenuationDist = intVal;
		break;
	case 2:
	{
		if (intVal > 100)
			intVal = 100;
		else if (intVal < -100)
			intVal = -100;
		sound->frequencyAdj = intVal;
		val = GetFrequencyModifier(sound);
		for (auto sndIter = g_audioManager->playingSounds.Begin(); sndIter; ++sndIter)
		{
			gameSound = sndIter.Get();
			if (gameSound && (gameSound->sourceSound == sound))
				gameSound->frequencyMod = val;
		}
		break;
	}
	case 3:
	{
		if (intVal < 0)
			intVal = -intVal;
		if (intVal > 100)
			intVal = 10000;
		else intVal *= 100;
		sound->staticAttenuation = intVal;
		for (auto sndIter = g_audioManager->playingSounds.Begin(); sndIter; ++sndIter)
		{
			gameSound = sndIter.Get();
			if (gameSound && (gameSound->sourceSound == sound))
				gameSound->staticAttenuation = intVal;
		}
		break;
	}
	case 4:
		if (intVal > 1434)
			intVal = 255;
		else if (intVal < 0)
			intVal = 0;
		else intVal = val / 5.62353;
		sound->endsAt = intVal;
		break;
	case 5:
		if (intVal > 1434)
			intVal = 255;
		else if (intVal < 0)
			intVal = 0;
		else intVal = val / 5.62353;
		sound->startsAt = intVal;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		if (intVal > 100)
			intVal = 100;
		else if (intVal < 0)
			intVal = 0;
		sound->attenuationCurve[traitID - 6] = intVal;
		break;
	case 11:
		if (intVal > 100)
			intVal = 100;
		else if (intVal < 0)
			intVal = 0;
		sound->reverbAttenuation = intVal;
		break;
	case 12:
		if (intVal > 255)
			intVal = 255;
		else if (intVal < 0)
			intVal = 0;
		sound->priority = intVal;
	}
	return true;
}

bool Cmd_GetSoundFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *sound;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &flagID) && (flagID <= 15))
		*result = (sound->soundFlags & (1 << flagID)) ? 1 : 0;
	return true;
}

bool Cmd_SetSoundFlag_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &flagID, &val) && (flagID <= 15))
		sound->SetFlag(1 << flagID, val != 0);
	return true;
}

bool Cmd_GetSoundSourceFile_Execute(COMMAND_ARGS)
{
	const char *resStr;
	TESSound *sound;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound))
		resStr = sound->soundFile.path.m_data;
	else resStr = NULL;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetSoundSourceFile_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &path))
		sound->soundFile.Set(path);
	return true;
}

bool Cmd_IsSoundPlaying_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *soundForm;
	TESObjectREFR *refr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm, &refr))
	{
		auto sndIter = g_audioManager->playingSounds.Begin();
		BSGameSound *gameSound;
		if (refr)
		{
			NiAVObject *refNode = refr->GetNiNode();
			if (!refNode) return true;
			for (; sndIter; ++sndIter)
			{
				gameSound = sndIter.Get();
				if (!gameSound || (gameSound->sourceSound != soundForm) || (refNode != g_audioManager->soundPlayingObjects.Lookup(sndIter.Key())))
					continue;
				*result = 1;
				break;
			}
		}
		else
		{
			for (; sndIter; ++sndIter)
			{
				gameSound = sndIter.Get();
				if (!gameSound || (gameSound->sourceSound != soundForm))
					continue;
				*result = 1;
				break;
			}
		}
	}
	return true;
}

bool Cmd_GetSoundPlayers_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESSound *soundForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm))
	{
		s_tempElements.Clear();
		BSGameSound *gameSound;
		BSFadeNode *fadeNode;
		for (auto sndIter = g_audioManager->playingSounds.Begin(); sndIter; ++sndIter)
		{
			gameSound = sndIter.Get();
			if (!gameSound || (gameSound->sourceSound != soundForm))
				continue;
			fadeNode = (BSFadeNode*)g_audioManager->soundPlayingObjects.Lookup(gameSound->mapKey);
			if (fadeNode && fadeNode->GetFadeNode() && fadeNode->linkedObj)
				s_tempElements.Append(fadeNode->linkedObj);
		}
		if (!s_tempElements.Empty())
			AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_StopSound_Execute(COMMAND_ARGS)
{
	TESSound *soundForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm))
	{
		BSGameSound *gameSound;
		for (auto sndIter = g_audioManager->playingSounds.Begin(); sndIter; ++sndIter)
		{
			gameSound = sndIter.Get();
			if (!gameSound || (gameSound->sourceSound != soundForm))
				continue;
			gameSound->stateFlags &= 0xFFFFFF0F;
			gameSound->stateFlags |= 0x10;
		}
	}
	return true;
}

bool Cmd_IsMusicPlaying_Execute(COMMAND_ARGS)
{
	UInt8 playState = 0;
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		UInt8 flags = g_playingMusic->track1Active ? g_playingMusic->track1Flags : g_playingMusic->track2Flags;
		if (flags & kMusicState_Play)
		{
			if (!form || (form == g_playingMusic->medLocCtrl))
				playState = (flags & kMusicState_Pause) ? 1 : 2;
			else if IS_ID(form, BGSMusicType)
			{
				String *filePath = &((BGSMusicType*)form)->soundFile.path;
				if (filePath->m_dataLen && StrBeginsCI(g_playingMusic->track1Active ? g_playingMusic->track1Path : g_playingMusic->track2Path, filePath->m_data))
					playState = (flags & kMusicState_Pause) ? 1 : 2;
			}
		}
	}
	*result = playState;
	return true;
}

bool Cmd_SetMusicState_Execute(COMMAND_ARGS)
{
	UInt32 state;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &state))
	{
		UInt8 *flagsPtr = g_playingMusic->track1Active ? &g_playingMusic->track1Flags : &g_playingMusic->track2Flags;
		if (*flagsPtr & kMusicState_Play)
		{
			switch (state)
			{
			case 0:
				*flagsPtr = kMusicState_Stop | kMusicState_Play;
				break;
			case 1:
				*flagsPtr |= kMusicState_Pause;
				break;
			case 2:
				*flagsPtr &= ~kMusicState_Pause;
				break;
			}
		}
	}
	return true;
}

bool Cmd_GetGameVolume_Execute(COMMAND_ARGS)
{
	UInt32 volType;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &volType) && (volType <= 5))
		*result = g_audioManager->volumes[volType] * 100;
	else *result = 0;
	return true;
}

bool Cmd_SetGameVolume_Execute(COMMAND_ARGS)
{
	UInt32 volType;
	int volLevel = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &volType, &volLevel) && (volType <= 5) && (volLevel <= 100))
		g_audioManager->volumes[volType] = (volLevel < 0) ? *(float*)(0x11F6E18 + volType * 0xC) : (volLevel / 100.0);
	return true;
}