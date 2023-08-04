#pragma once

// 0C
class Setting
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);

	enum EType
	{
		kSetting_Bool = 0,
		kSetting_c,
		kSetting_h,
		kSetting_Integer,
		kSetting_Unsigned,
		kSetting_Float,
		kSetting_String,
		kSetting_r,
		kSetting_a,
		kSetting_Other
	};

	union Info
	{
		bool		b;
		UInt32		uint;
		int			i;
		float		f;
		char		*str;
	};

	Info		data;	// 04
	const char	*name;	// 08

	UInt32 GetType();
	bool ValidType();
	void Get(double *out);
	void Set(double newVal);
	void Set(const char *str, bool doFree);
};

template<class T> class SettingCollection 
{
	UInt8	byt0004;
	UInt8	fil0005[3];
	UInt32	arr0008[0x0100];
	UInt32	unk0108;
};

// 0014
template<class M, class T> class NiTStringTemplateMap: M
{
	UInt8	byt0010;	// 0010
	UInt8	fil0011[3];	// 0011
};

template<class T> class NiTStringMap : NiTStringTemplateMap<NiTMap<char const *, T>, T>
{
};

template<class T> class BSTCaseInsensitiveStringMap: NiTStringMap<T>
{
};

template<class T> class SettingCollectionMap
{
	SettingCollection<T>					coll000;
	const BSTCaseInsensitiveStringMap<T *>	map010C;
};

template<class T> class SettingCollectionList
{
	SettingCollection<T>	coll000;
	tList<UInt32>			lst010C;
};

// 120
class GameSettingCollection			// Contains SettingCollectionMap<Setting> and SettingCollection<Setting>
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);

	// BSTCaseInsensitiveStringMap <Setting *>
	typedef NiTMap<const char*, Setting*> SettingMap;

	// void		** vtbl							// 000
	UInt32		unk004[(0x10C - 0x004) >> 2];	// 004
	SettingMap	settingMap;						// 10C BSTCaseInsensitiveStringMap
	UInt8		unk11C;							// 11C
	UInt8		pad11D[3];

	bool GetGameSetting(char* settingName, Setting** out);
	static GameSettingCollection * GetSingleton();
};
static_assert(sizeof(GameSettingCollection) == 0x120);

// 114
// actually 2 classes used: IniSettingCollection and IniPrefSettingCollection. Layouts are the same
class IniSettingCollection
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	AddSetting(Setting *setting);
	virtual void	RemoveSetting(Setting *setting);
	virtual void	GetViewerStrings(void *arg);
	virtual bool    WriteSetting(Setting *setting);
    virtual bool    ReadSetting(Setting *setting);
    virtual bool    Open(UInt32 unused);
    virtual bool    Close();
    virtual bool    WriteAllSettings();
    virtual bool    ReadAllSettings();

	char				iniPath[0x104];		// 004
	UInt32				handle;				// 108
	tList<Setting>		settings;			// 10C

	static IniSettingCollection * GetIniSettings();
	static IniSettingCollection * GetIniPrefs();
};
static_assert(sizeof(IniSettingCollection) == 0x114);

bool GetNumericGameSetting(char *settingName, double *result);
bool GetINISetting(const char *settingName, Setting **out);
bool GetNumericINISetting(char *settingName, double *result);