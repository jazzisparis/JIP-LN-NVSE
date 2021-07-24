#pragma once

class BSAudioManagerThread;

// 254
class BSSoundInfo
{
public:
	UInt32			unk000[72];		// 000
	const char		*filePath;		// 120
	UInt32			unk124[76];		// 124
};

// 198
class BSGameSound
{
public:
	virtual BSGameSound	*Destroy(bool doFree);
	virtual bool	CheckStateFlagsBit5();
	virtual bool	CheckStateFlagsBit19();
	virtual bool	CheckStateFlagsBit6();
	virtual float	GetVolume();
	virtual void	SetIsLooping(bool doSet);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	CopyFrom(BSGameSound *source, bool arg2);
	virtual void	Unk_0C(void);
	virtual bool	Unk_0D(void);
	virtual bool	Unk_0E(void);
	virtual bool	SetVolume(float inVol);
	virtual void	Unk_10(void);
	virtual bool	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(float arg1, float arg2, float arg3);
	virtual void	Unk_14(float arg1, float arg2, float arg3);
	virtual void	Unk_15(NiVector3 &arg1);
	virtual void	Unk_16(void);
	virtual void	Unk_17(float arg1, float arg2);
	virtual void	Unk_18(UInt16 arg1, UInt16 arg2, UInt16 arg3, UInt16 arg4, UInt16 arg5);
	virtual bool	SetFrameFrequencyPerc(float freqPerc);
	virtual float	GetFrameFrequencyPerc();
	virtual void	Seek(UInt32 timePoint);

	enum SoundFlags
	{
		kFlag_2D =						1,
		kFlag_3D =						2,
		kFlag_IsVoice =					4,
		kFlag_IsFootsteps =				8,
		kFlag_Loop =					0x10,
		kFlag_NotDialogue =				0x20,
		kFlag_RandomFrequencyShift =	0x40,
		kFlag_UnkBit7 =					0x80,
		kFlag_UnkBit8 =					0x100,
		kFlag_UnkBit9 =					0x200,
		kFlag_UnkBit10 =				0x400,
		kFlag_IsMusic =					0x800,
		kFlag_MuteWhenSubmerged1 =		0x1000,
		kFlag_MaybeUnderwater =			0x2000,
		kFlag_DonotCache =				0x10000,
		kFlag_FirstPerson =				0x40000,
		kFlag_Modulated =				0x80000,
		kFlag_IsRadio =					0x100000,
		kFlag_UnkBit22 =				0x400000,
		kFlag_EnvelopeFast =			0x2000000,
		kFlag_EnvelopeSlow =			0x4000000,
		kFlag_2DRadius =				0x8000000
	};

	enum Flags00C
	{
		kFlag_MuteWhenSubmerged =		1,
		kFlag_StartAtRandomPosition =	2,
	};

	enum StateFlags
	{
		kState_Stopped =	0x10,
		kState_UnkBit5 =	0x20,
		kState_UnkBit6 =	0x40,
		kState_UnkBit9 =	0x200,
		kState_Muted =		0x400,
		kState_UnkBit15 =	0x8000,
		kState_UnkBit16 =	0x10000,
		kState_IsLooping =	0x20000,
		kState_UnkBit19 =	0x80000,
		kState_UnkBit20 =	0x100000,
	};

	UInt32			mapKey;					// 004
	UInt32			soundFlags;				// 008
	UInt32			flags00C;				// 00C
	UInt32			stateFlags;				// 010
	UInt32			duration;				// 014
	UInt16			staticAttenuation;		// 018	dB * -1000
	UInt16			unk01A;					// 01A
	UInt16			unk01C;					// 01C
	UInt16			unk01E;					// 01E
	UInt16			unk020;					// 020
	UInt16			unk022;					// 022
	float			volume;					// 024	0.0 to 1.0
	float			flt028;					// 028
	float			flt02C;					// 02C
	UInt32			unk030;					// 030
	UInt16			baseSamplingFreq;		// 034
	char			filePath[254];			// 036	Originally: filePath[260]
	TESSound		*sourceSound;			// 134	"Stolen" from filePath
	float			frequencyMod;			// 138	^
	float			maxAttenuationDist;		// 13C
	float			minAttenuationDist;		// 140
	UInt32			pathHashFile;			// 144
	UInt32			pathHashFolder;			// 148
	UInt32			unk14C;					// 14C
	float			flt150;					// 150
	UInt32			unk154;					// 154
	SInt8			randFrequencyShift;		// 158
	UInt8			byte159;				// 159
	UInt16			samplingFreq;			// 15A
	UInt32			unk15C;					// 15C
	UInt32			unk160;					// 160
	UInt32			unk164;					// 164
	UInt32			unk168;					// 168
	UInt32			unk16C;					// 16C
	UInt32			unk170;					// 170
	UInt32			unk174[5];				// 174
	UInt32			priority;				// 188
	UInt32			unk18C[3];				// 18C
};
STATIC_ASSERT(sizeof(BSGameSound) == 0x198);

// 230
class BSWin32GameSound : public BSGameSound
{
public:
	UInt32			unk198;					// 198
	UInt32			*ptr19C;				// 19C	DirectSound func table
	UInt32			unk1A0[5];				// 1A0
	UInt32			*ptr1B4;				// 1B4	DirectSound func table
	UInt32			unk1B8[5];				// 1B8
	UInt32			*ptr1CC;				// 1CC	DirectSound func table
	UInt32			unk1D0;					// 1D0
	UInt32			*ptr1D4;				// 1D4	DirectSound func table
	UInt32			unk1D8[13];				// 1D8
	UInt16			word20C;				// 20C
	UInt16			word20E;				// 20E
	UInt16			word210;				// 210
	UInt16			word212;				// 212
	UInt16			word214;				// 214
	UInt16			word216;				// 216
	NiVector3		originWorldPos;			// 218
	UInt32			unk224[3];				// 224
};
STATIC_ASSERT(sizeof(BSWin32GameSound) == 0x230);

enum AudioRequestTypes
{
	kRequestType_Stop =											3,
	kRequestType_Stop2 =										4,
	kRequestType_Delay =										5,
	kRequestType_StopSoundFadeOutTime =							9,
	kRequestType_DialogueFadeInOrOut =							0xD,
	kRequestType_RecalculateVolumesForChangesToMasterVolume =	0xE,
	kRequestType_Volume =										0x10,
	kRequestType_FrequencyAdjustment =							0x14,
	kRequestType_FadeAllSFX =									0x18,
	kRequestType_SetState400ForSomeSounds =						0x1A,
	kRequestType_ClearState400ForSomeSounds =					0x1B,
	kRequestType_SetState400ForSomeSounds2 =					0x1C,
	kRequestType_ClearState400ForSomeSounds2 =					0x1D,
	kRequestType_SoundAtPos =									0x1E,
	kRequestType_Attenuation =									0x21,
	kRequestType_ReverbAttenuation =							0x22,
	kRequestType_PlayWithMinMaxDistance =						0x23,
	kRequestType_SetsPlayingSoundClearStateBit200 =				0x26,
	kRequestType_PlayAtNode =									0x27,
	kRequestType_NiNodeSound =									0x29,
	kRequestType_StopAllSoundsWithFlags =						0x2A,
	kRequestType_SetAudioListenerPosition =						0x38,
	kRequestType_HasCompletionCallback =						0x3A,
	kRequestType_HasSomeCallback =								0x3B,
	kRequestType_StartsAtEndsAt =								0x3E,
	kRequestType_MultiThreaded =								0x35,
	kRequestType_ToggleFirstPersonForPlayingSound =				0x40,
	kRequestType_Priority =										0x41,
	kRequestType_Speed =										0x42
};

// 20
struct AudioRequestData
{
	UInt32				type;		// 00
	UInt32				soundKey;	// 04
	FunctionArg			value1;		// 08
	FunctionArg			value2;		// 0C
	NiNode				*niNode;	// 10
	NiVector3			pos;		// 14
};

// 188
class BSAudioManager
{
public:
	virtual void				Destroy(bool doFree);

	// 10
	struct AudioRequest
	{
		UInt32				count;			// 00
		UInt8				byte04;			// 04
		UInt8				pad05[3];		// 05
		AudioRequestData	*requestData;	// 08
		UInt32				tickCount;		// 0C
	};

	UInt32						unk004[8];			// 004
	AudioRequest				request024;			// 024
	AudioRequest				request034;			// 034
	AudioRequest				request044;			// 044
	NiTPointerMap<BSGameSound>	playingSounds;		// 054
	NiTPointerMap<BSSoundInfo>	playingSoundInfos1;	// 064
	NiTPointerMap<BSSoundInfo>	playingSoundInfos2;	// 074
	NiTPointerMap<NiAVObject>	soundPlayingObjects;// 084
	DList<BSGameSound>			cachedSounds;		// 094
	UInt32						unk0A0;				// 0A0
	UInt32						unk0A4;				// 0A4
	float						flt0A8;				// 0A8
	LPCRITICAL_SECTION			cs0AC;				// 0AC
	UInt32						unk0B0[5];			// 0B0
	LPCRITICAL_SECTION			cs0C4;				// 0C4
	UInt32						unk0C8[5];			// 0C8
	LPCRITICAL_SECTION			cs0DC;				// 0DC
	UInt32						unk0E0[5];			// 0E0
	LPCRITICAL_SECTION			cs0F4;				// 0F4
	UInt32						unk0F8[5];			// 0F8
	LPCRITICAL_SECTION			cs10C;				// 10C
	UInt32						unk110[5];			// 110
	DList<void>					list124;			// 124
	UInt32						lastTickCount;		// 130
	UInt8						byte134;			// 134
	UInt8						byte135;			// 135
	UInt8						pad136[2];			// 136
	UInt32						unk138;				// 138
	BSAudioManagerThread		*audioMgrThread;	// 13C
	float						volumes[12];		// 140
	//	0	Master
	//	1	Foot
	//	2	Voice
	//	3	Effects
	//	4	Music
	//	5	Radio
	UInt32						unk170;				// 170
	UInt8						byte174;			// 174
	UInt8						byte175;			// 175
	UInt8						pad176[2];			// 176
	UInt32						unk178;				// 178
	UInt32						unk17C;				// 17C
	UInt32						nextMapKey;			// 180
	UInt8						byte184;			// 184
	UInt8						pad185[3];			// 185

	__forceinline static BSAudioManager *Get() {return (BSAudioManager*)0x11F6EF0;}
};
STATIC_ASSERT(sizeof(BSAudioManager) == 0x188);

class BSAudioListener
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	SetOriginWorldPos(NiVector3 *pos);
	virtual void	Unk_03(void);
	virtual void	UpdatePositionAndOrientation();
	virtual void	SetFrontAndTopOrientation(NiVector3 *front, NiVector3 *top);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	SetVelocity(NiVector3 *pVelocity);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	SetRolloffFactor(float factor);
};

class IDirectSound3DListener;

// 64
class BSWin32AudioListener : public BSAudioListener
{
public:
	NiVector3				originWorldPos;		// 04
	UInt32					unk10[6];			// 10
	NiVector3				velocity;			// 28
	UInt32					sysTime;			// 34
	float					flt38;				// 38
	float					flt3C;				// 3C
	float					flt40;				// 40
	UInt32					unk44;				// 44
	IDirectSound3DListener	**ptr48;			// 48
	NiVector3				topOrientation;		// 4C
	NiVector3				frontOrientation;	// 58
};
STATIC_ASSERT(sizeof(BSWin32AudioListener) == 0x64);

class IDirectSound8;
class IDirectSoundBuffer;

// A4
class BSWin32Audio
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	CreateAudioListener(HWND _window);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);	// Does nothing
	virtual void	Unk_04(void);	// Does nothing
	virtual BSGameSound	*CreateGameSound(const char *filePath);
	virtual void	InsertPathPrefix(char *filePath);	// Prefixes path with data\\sound\\ if fx\\ or song\\.
	virtual void	Unk_07(void);	// Does nothing

	UInt8					byte04;			// 04
	UInt8					byte05;			// 05
	UInt8					byte06;			// 06
	UInt8					pad07;			// 07
	UInt32					unk08;			// 08
	UInt8					byte0C;			// 0C
	UInt8					pad0D[3];		// 0D
	BSWin32AudioListener	*listener;		// 10
	UInt32					unk14[3];		// 14
	bool					(*GetSoundDataFromRefID)(UInt32 refID, char *outFilePath, UInt32 *outFlags, TESSound **outSound);	// 20	0x82D150
	bool					(*GetSoundDataFromEDID)(const char *EDIDstr, char *outFilePath, UInt32 *outFlags, TESSound **outSound);	// 24	0x82D280
	bool					(*PickSoundFileFromFolder)(char *outFilePath);	// 28	0x5E3630
	UInt32					(*FillGameSoundProps)(UInt32 *mapKey, TESSound *soundForm, UInt32 *outFlags0C);	// 2C	0x82D400
	void					(*sub_832C40)(void);	// 30
	void					(*sub_832C80)(void);	// 34
	IDirectSound8 			**ptr38;				// 38
	IDirectSoundBuffer		*ptr3C;					// 3C
	UInt32					unk40[24];				// 40
	HWND					window;					// A0

	__forceinline static BSWin32Audio *Get() {return *(BSWin32Audio**)0x11F6D98;}
};
STATIC_ASSERT(sizeof(BSWin32Audio) == 0xA4);

// 30
class BSThread
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);

	LPCRITICAL_SECTION	cs;			// 04
	UInt32				unk08[10];	// 08
};

// 9C
class FAMThread : public BSThread
{
public:
	UInt32				unk30[27];	// 30
};

enum
{
	kMusicState_Pause =		1 << 2,
	kMusicState_Stop =		1 << 3,
	kMusicState_Play =		1 << 4,
	kMusicState_Loop =		1 << 5,
};

// 288
struct PlayingMusic
{
	char					track1Path[MAX_PATH];	// 000
	FAMThread				*famThread;				// 104
	char					track2Path[MAX_PATH];	// 108
	UInt32					unk20C;					// 20C
	float					flt210;					// 210
	float					flt214;					// 214
	float					flt218;					// 218
	float					flt21C;					// 21C
	UInt8					track1Flags;			// 220
	UInt8					track2Flags;			// 221
	UInt8					pad222[2];				// 222
	UInt32					unk224;					// 224
	UInt32					unk228[8];				// 228
	float					flt248;					// 248
	float					flt24C;					// 24C
	float					flt250;					// 250
	float					flt254;					// 254
	UInt32					unk258;					// 258
	UInt32					unk25C[8];				// 25C
	UInt32					track1Active;			// 27C
	UInt32					unk280;					// 280
	MediaLocationController	*medLocCtrl;			// 284

	__forceinline static PlayingMusic *Get() {return (PlayingMusic*)0x11DD0F0;}
};
STATIC_ASSERT(sizeof(PlayingMusic) == 0x288);