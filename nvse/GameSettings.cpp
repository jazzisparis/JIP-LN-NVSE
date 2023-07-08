#include "nvse/GameSettings.h"

UInt32 Setting::GetType()
{
	if (name)
	{
		switch (*name | 0x20)
		{
		case 'b':
			return kSetting_Bool;
		case 'c':
			return kSetting_c;
		case 'i':
			return kSetting_Integer;
		case 'u':
			return kSetting_Unsigned;
		case 'f':
			return kSetting_Float;
		case 's':
			return kSetting_String;
		case 'r':
			return kSetting_r;
		case 'a':
			return kSetting_a;
		default:
			break;
		}
	}
	return kSetting_Other;
}

bool Setting::ValidType()
{
	switch (*name | 0x20)
	{
		case 'b':
		case 'f':
		case 'i':
		case 's':
		case 'u':
			return true;
		default:
			return false;
	}
}

void Setting::Get(double *out)
{
	switch (*name | 0x20)
	{
		case 'b':
			*out = data.b;
			break;
		case 'u':
			*out = data.uint;
			break;
		case 'f':
			*out = data.f;
			break;
		case 'i':
			*out = data.i;
			break;
		default:
			break;
	}
}

void Setting::Set(double newVal)
{
	switch (*name | 0x20)
	{
		case 'b':
			data.b = newVal != 0;
			break;
		case 'f':
			data.f = newVal;
			break;
		case 'i':
			data.i = newVal;
			break;
		case 'u':
			data.uint = cvtd2ul(newVal);
			break;
		default:
			break;
	}
}

void Setting::Set(const char *strVal, bool doFree)
{
	if (doFree) Game_HeapFree(data.str);
	data.str = CopyCString(strVal);
}

__declspec(naked) bool GameSettingCollection::GetGameSetting(char *settingName, Setting **out)
{
	__asm
	{
		add		ecx, 0x10C
		JMP_EAX(0x853130)
	}
}

GameSettingCollection *GameSettingCollection::GetSingleton()
{
	return *(GameSettingCollection**)0x11C8048;
}

IniSettingCollection *IniSettingCollection::GetIniSettings()
{
	return *(IniSettingCollection**)0x11F96A0;
}

IniSettingCollection *IniSettingCollection::GetIniPrefs()
{
	return *(IniSettingCollection**)0x11F35A0;
}