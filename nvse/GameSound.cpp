#include "nvse/GameSound.h"

void Sound::PlayEDID(const char *soundEDID, UInt32 flags, TESObjectREFR *refr)
{
	Sound sound;
	ThisCall(0xAD7550, BSWin32Audio::Get(), &sound, soundEDID, flags);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refr->posX, refr->posY, refr->posZ);
		sound.Play();
	}
}

void Sound::PlayFile(const char *filePath, UInt32 flags, TESObjectREFR *refr)
{
	NiNode *refrNode = refr->GetNiNode();
	if (!refrNode) return;
	Sound sound;
	ThisCall(0xAD7480, BSWin32Audio::Get(), &sound, filePath, flags, nullptr);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refrNode->m_worldTranslate.x, refrNode->m_worldTranslate.y, refrNode->m_worldTranslate.z);
		sound.SetNiNode(refrNode);
		sound.Play();
	}
}

void Sound::PlayTESSound(TESSound *gameSound, UInt32 flags, TESObjectREFR *refr)
{
	const char *filePath = gameSound->soundFile.path.m_data;
	if (!filePath) return;
	NiNode *refrNode = refr->GetNiNode();
	if (!refrNode) return;
	Sound sound;
	ThisCall(0xAD7480, BSWin32Audio::Get(), &sound, filePath, flags, gameSound);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refrNode->m_worldTranslate.x, refrNode->m_worldTranslate.y, refrNode->m_worldTranslate.z);
		sound.SetNiNode(refrNode);
		sound.Play();
	}
}