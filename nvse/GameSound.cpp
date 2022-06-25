#include "nvse/GameSound.h"

void Sound::PlayEDID(const char *soundEDID, UInt32 flags, TESObjectREFR *refr)
{
	Sound sound;
	BSAudioManager::Get()->InitSoundEDID(sound, soundEDID, flags);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refr->position);
		sound.Play();
	}
}

void Sound::PlayFile(const char *filePath, UInt32 flags, TESObjectREFR *refr)
{
	NiNode *refrNode = refr->GetRefNiNode();
	if (!refrNode) return;
	Sound sound;
	BSAudioManager::Get()->InitSoundPath(sound, filePath, flags);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refrNode->WorldTranslate());
		sound.SetNiNode(refrNode);
		sound.Play();
	}
}

void Sound::PlayTESSound(TESSound *soundForm, UInt32 flags, TESObjectREFR *refr)
{
	NiNode *refrNode = refr->GetRefNiNode();
	if (!refrNode) return;
	Sound sound;
	BSAudioManager::Get()->InitSoundForm(sound, soundForm->refID, flags);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refrNode->WorldTranslate());
		sound.SetNiNode(refrNode);
		sound.Play();
	}
}