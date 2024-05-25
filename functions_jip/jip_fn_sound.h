#pragma once

DEFINE_COMMAND_PLUGIN(GetSoundTraitNumeric, 0, kParams_OneSound_OneInt);
DEFINE_COMMAND_PLUGIN(SetSoundTraitNumeric, 0, kParams_OneSound_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetSoundFlag, 0, kParams_OneSound_OneInt);
DEFINE_COMMAND_PLUGIN(SetSoundFlag, 0, kParams_OneSound_TwoInts);
DEFINE_COMMAND_PLUGIN(GetSoundSourceFile, 0, kParams_OneSound);
DEFINE_COMMAND_PLUGIN(SetSoundSourceFile, 0, kParams_OneSound_OneString);
DEFINE_COMMAND_PLUGIN(IsSoundPlaying, 0, kParams_OneSound_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetSoundPlayers, 0, kParams_OneSound);
DEFINE_COMMAND_PLUGIN(StopSound, 0, kParams_OneSound_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(IsMusicPlaying, 0, kParams_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(SetMusicState, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetGameVolume, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetGameVolume, 0, kParams_OneInt_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(PlaySound3DNode, 1, kParams_OneSound_OneString);

bool Cmd_GetSoundTraitNumeric_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &traitID))
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
				*result = sound->staticAttenuation * -0.01;
				break;
			case 4:
				*result = sound->endsAt * (1434 / 255.0F);
				break;
			case 5:
				*result = sound->startsAt * (1434 / 255.0F);
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
			for (auto sndIter = BSAudioManager::Get()->playingSounds.Begin(); sndIter; ++sndIter)
				if (BSGameSound *gameSound = sndIter.Get(); gameSound && (gameSound->sourceSound == sound))
					gameSound->frequencyMod = val;
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
			for (auto sndIter = BSAudioManager::Get()->playingSounds.Begin(); sndIter; ++sndIter)
				if (BSGameSound *gameSound = sndIter.Get(); gameSound && (gameSound->sourceSound == sound))
					gameSound->staticAttenuation = intVal;
			break;
		}
		case 4:
			if (intVal > 1434)
				intVal = 255;
			else if (intVal < 0)
				intVal = 0;
			else intVal = val * (255 / 1434.0F);
			sound->endsAt = intVal;
			break;
		case 5:
			if (intVal > 1434)
				intVal = 255;
			else if (intVal < 0)
				intVal = 0;
			else intVal = val * (255 / 1434.0F);
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
	TESSound *sound;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &flagID) && (flagID <= 31) && (sound->soundFlags & (1 << flagID)))
		*result = 1;
	return true;
}

bool Cmd_SetSoundFlag_Execute(COMMAND_ARGS)
{
	TESSound *sound;
	UInt32 flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound, &flagID, &val) && (flagID <= 31))
		sound->SetFlag(1 << flagID, val != 0);
	return true;
}

bool Cmd_GetSoundSourceFile_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESSound *sound;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sound))
		resStr = sound->soundFile.path.m_data;
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

bool __fastcall PlayingSoundsIterator(TESSound *soundForm, bool doStop, TESObjectREFR *refr)
{
	BSAudioManager *audioMngr = BSAudioManager::Get();
	auto sndIter = audioMngr->playingSounds.Begin();
	BSGameSound *gameSound;
	if (refr)
	{
		if (!refr->GetRefNiNode())
			return false;
		auto playingObjMap = &audioMngr->soundPlayingObjects;
		NiAVObject *soundObj;
		for (; sndIter; ++sndIter)
		{
			if (!(gameSound = sndIter.Get()) || (gameSound->sourceSound != soundForm) || !(soundObj = playingObjMap->Lookup(sndIter.Key())) || (soundObj->GetParentRef() != refr))
				continue;
			if (!doStop) return true;
			gameSound->stateFlags &= 0xFFFFFF0F;
			gameSound->stateFlags |= 0x10;
		}
	}
	else
	{
		for (; sndIter; ++sndIter)
		{
			if (!(gameSound = sndIter.Get()) || (gameSound->sourceSound != soundForm))
				continue;
			if (!doStop) return true;
			gameSound->stateFlags &= 0xFFFFFF0F;
			gameSound->stateFlags |= 0x10;
		}
	}
	return false;
}

bool Cmd_IsSoundPlaying_Execute(COMMAND_ARGS)
{
	TESSound *soundForm;
	TESObjectREFR *refr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm, &refr) && PlayingSoundsIterator(soundForm, false, refr))
		*result = 1;
	return true;
}

bool Cmd_GetSoundPlayers_Execute(COMMAND_ARGS)
{
	TESSound *soundForm;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm))
	{
		TempElements *tmpElements = GetTempElements();
		BSAudioManager *audioMngr = BSAudioManager::Get();
		auto playingObjMap = &audioMngr->soundPlayingObjects;
		BSGameSound *gameSound;
		NiAVObject *object;
		TESObjectREFR *refr;
		for (auto sndIter = audioMngr->playingSounds.Begin(); sndIter; ++sndIter)
			if ((gameSound = sndIter.Get()) && (gameSound->sourceSound == soundForm) && (object = playingObjMap->Lookup(gameSound->mapKey)) && (refr = object->GetParentRef()))
				tmpElements->Append(refr);
		if (!tmpElements->Empty())
			*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_StopSound_Execute(COMMAND_ARGS)
{
	TESSound *soundForm;
	TESObjectREFR *refr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm, &refr))
		PlayingSoundsIterator(soundForm, true, refr);
	return true;
}

bool Cmd_IsMusicPlaying_Execute(COMMAND_ARGS)
{
	UInt8 playState = 0;
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		PlayingMusic *playingMus = PlayingMusic::Get();
		UInt8 flags = playingMus->track1Active ? playingMus->track1Flags : playingMus->track2Flags;
		if (flags & kMusicState_Play)
		{
			if (!form || (form == playingMus->medLocCtrl))
				playState = (flags & kMusicState_Pause) ? 1 : 2;
			else if IS_ID(form, BGSMusicType)
			{
				String *filePath = &((BGSMusicType*)form)->soundFile.path;
				if (filePath->m_dataLen && StrBeginsCI(playingMus->track1Active ? playingMus->track1Path : playingMus->track2Path, filePath->m_data))
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
		PlayingMusic *playingMus = PlayingMusic::Get();
		UInt8 *flagsPtr = playingMus->track1Active ? &playingMus->track1Flags : &playingMus->track2Flags;
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
		*result = BSAudioManager::Get()->volumes[volType] * 100;
	return true;
}

bool Cmd_SetGameVolume_Execute(COMMAND_ARGS)
{
	UInt32 volType;
	int volLevel = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &volType, &volLevel) && (volType <= 5) && (volLevel <= 100))
		BSAudioManager::Get()->volumes[volType] = (volLevel < 0) ? *(float*)(0x11F6E18 + volType * 0xC) : (volLevel * 0.01F);
	return true;
}

bool Cmd_PlaySound3DNode_Execute(COMMAND_ARGS)
{
	TESSound *soundForm;
	char nodeName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &soundForm, &nodeName))
		if (NiNode *targetNode = thisObj->GetNode2(nodeName))
		{
			Sound sound;
			BSAudioManager::Get()->InitSoundForm(sound, soundForm->refID, 0x102);
			if (sound.soundKey != 0xFFFFFFFF)
			{
				sound.SetPos(targetNode->WorldTranslate());
				sound.SetNiNode(targetNode);
				sound.Play();
			}
		}
	return true;
}