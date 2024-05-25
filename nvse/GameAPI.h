#pragma once

#define playerID	0x7
#define playerRefID 0x14

void Console_Print(const char * fmt, ...);

typedef bool (* _ExtractArgs)(ParamInfo * paramInfo, void * scriptData, UInt32 * arg2, TESObjectREFR * arg3, TESObjectREFR * arg4, Script * script, ScriptLocals * eventList, ...);
extern const _ExtractArgs ExtractArgs;

typedef TESForm * (* _CreateFormInstance)(UInt8 type);
extern const _CreateFormInstance CreateFormInstance;

bool GetConsoleEcho();
void SetConsoleEcho(bool doEcho);

typedef void (* _ShowMessageBox_Callback)(void);
extern const _ShowMessageBox_Callback ShowMessageBox_Callback;

// unk1 = 0
// unk2 = 0
// callback = may be NULL apparently
// unk4 = 0
// unk5 = 0x17 (why?)
// unk6 = 0
// unk7 = 0
// then buttons
// then NULL
typedef bool (* _ShowMessageBox)(const char * message, UInt32 unk1, UInt32 unk2, _ShowMessageBox_Callback callback, UInt32 unk4, UInt32 unk5, float unk6, float unk7, ...);
extern const _ShowMessageBox ShowMessageBox;

// set to scriptObj->refID after calling ShowMessageBox()
// GetButtonPressed checks this before returning a value, if it doesn't match it returns -1
typedef UInt32 * _ShowMessageBox_pScriptRefID;
extern const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID;
typedef UInt8 * _ShowMessageBox_button;
extern const _ShowMessageBox_button ShowMessageBox_button;

// unk1 = 0
// unk3 = 0, "UIVATSInsufficientAP" (sound?)
// duration = 2
// unk5 = 0
typedef bool (*_QueueUIMessage)(const char *msgText, UInt32 iconType, const char *iconPath, const char *soundPath, float displayTime, UInt8 unk5);
extern const _QueueUIMessage QueueUIMessage;

enum
{
	kMaxMessageLength = 0x4000
};

union VarData
{
	double			num;
	struct
	{
		UInt32		refID;
		UInt32		pad;
	};

	inline void operator=(double dVal) {num = dVal;}
	inline void operator=(UInt32 uVal) {refID = uVal;}
};

struct ScriptVar
{
	UInt32		id;
	UInt32		unk04;
	VarData		data;
	UInt8		type;
	UInt8		pad11[7];
};

struct ScriptLocals
{
	enum
	{
		kEvent_OnAdd =						1,
		kEvent_OnEquip =					2,
		kEvent_OnActorEquip =				kEvent_OnEquip,
		kEvent_OnDrop =						4,
		kEvent_OnUnequip =					8,
		kEvent_OnActorUnequip =				kEvent_OnUnequip,

		kEvent_OnDeath =					0x10,
		kEvent_OnMurder =					0x20,
		kEvent_OnCombatEnd =				0x40,
		kEvent_OnHit =						0x80,

		kEvent_OnHitWith =					0x100,
		kEvent_OnPackageStart =				0x200,
		kEvent_OnPackageDone =				0x400,
		kEvent_OnPackageChange =			0x800,

		kEvent_OnLoad =						0x1000,
		kEvent_OnMagicEffectHit =			0x2000,
		kEvent_OnSell =						0x4000,
		kEvent_OnStartCombat =				0x8000,

		kEvent_OnOpen =						0x10000,
		kEvent_OnClose =					0x20000,
		kEvent_SayToDone =					0x40000,
		kEvent_OnGrab =						0x80000,

		kEvent_OnRelease =					0x100000,
		kEvent_OnDestructionStageChange =	0x200000,
		kEvent_OnFire =						0x400000,

		kEvent_OnTrigger =					0x10000000,
		kEvent_OnTriggerEnter =				0x20000000,
		kEvent_OnTriggerLeave =				0x40000000,
		kEvent_OnReset =					0x80000000
	};

	struct Event
	{
		TESForm		*object;
		UInt32		eventMask;
	};

	struct EffectScriptFlags
	{
		bool	effectStart;
		bool	effectFinish;
		UInt8	unk03[2];
		float	secondsElapsed;
	};

	typedef tList<Event> EventList;
	typedef tList<ScriptVar> VarList;

	Script				*m_script;		// 00
	UInt8				m_flags;		// 04
	UInt8				pad05[3];		// 05
	EventList			*m_eventList;	// 08
	VarList				*m_vars;		// 0C
	EffectScriptFlags	*m_effScrFlags;	// 10

	ScriptVar* __fastcall GetVariable(UInt32 id) const;
	UInt32 ResetAllVariables();
	ScriptLocals *CreateCopy();

	__forceinline void Destroy() {ThisCall(0x41AF70, this, 1);}
};

// 914
class ConsoleManager
{
public:
	struct TextNode
	{
		TextNode	*next;
		TextNode	*prev;
		String		text;
	};

	struct TextList
	{
		TextNode	*first;
		TextNode	*last;
		UInt32		count;
	};

	void		*scriptContext;		// 000
	TextList	printedLines;		// 004
	TextList	inputHistory;		// 010
	UInt32		unk01C;				// 01C
	UInt32		unk020;				// 020
	UInt32		unk024;				// 024
	UInt32		unk028[571];		// 028

	static ConsoleManager *GetSingleton();

	void Clear();
};
static_assert(sizeof(ConsoleManager) == 0x914);

extern ConsoleManager *g_consoleManager;

class InteriorCellNewReferencesMap;
class ExteriorCellNewReferencesMap;
class NumericIDBufferMap;

class NiBinaryStream
{
public:
	virtual void	Destructor(bool freeMemory);		// 00
	virtual void	Unk_01(void);						// 04
	virtual void	SeekCur(SInt32 delta);				// 08
	virtual void	GetBufferSize(void);				// 0C
	virtual void	InitReadWriteProcs(bool useAlt);	// 10

//	void	** m_vtbl;		// 000
	UInt32	m_offset;		// 004
	void	* m_readProc;	// 008 - function pointer
	void	* m_writeProc;	// 00C - function pointer
};

class NiFile: public NiBinaryStream
{
public:
	virtual UInt32	SetOffset(UInt32 newOffset, UInt32 arg2);	// 14
	virtual UInt32	GetFilename(void);	// 18
	virtual UInt32	GetSize();			// 1C

	UInt32	m_bufSize;	// 010
	UInt32	m_unk014;	// 014 - Total read in buffer
	UInt32	m_unk018;	// 018 - Consumed from buffer
	UInt32	m_unk01C;	// 01C
	void*	m_buffer;	// 020
	FILE*	m_File;		// 024
};

// 158
class BSFile : public NiFile
{
public:
	virtual bool	Reset(bool arg1, bool arg2);	// 20
	virtual bool	Unk_09(UInt32 arg1);	// 24
	virtual UInt32	Unk_0A();	// 28
	virtual UInt32	Unk_0B(String *string, UInt32 arg2);	// 2C
	virtual UInt32	Unk_0C(void *ptr, UInt32 arg2);	// 30
	virtual UInt32	ReadBufDelim(void *bufferPtr, UInt32 bufferSize, short delim);		// 34
	virtual UInt32	Unk_0E(void *ptr, UInt8 arg2);	// 38
	virtual UInt32	Unk_0F(void *ptr, UInt8 arg2);	// 3C
	virtual bool	IsReadable();	// 40
	virtual UInt32	ReadBuf(void *bufferPtr, UInt32 numBytes);	// 44
	virtual UInt32	WriteBuf(void *bufferPtr, UInt32 numBytes);	// 48

	UInt32		m_modeReadWriteAppend;	// 028
	UInt8		m_good;					// 02C
	UInt8		pad02D[3];				// 02D
	UInt8		m_unk030;				// 030
	UInt8		pad031[3];				// 031
	UInt32		m_unk034;				// 034
	UInt32		m_unk038;				// 038 - init'd to FFFFFFFF
	UInt32		m_unk03C;				// 038
	UInt32		m_unk040;				// 038
	char		m_path[0x104];			// 044
	UInt32		m_unk148;				// 148
	UInt32		m_unk14C;				// 14C
	UInt32		m_fileSize;				// 150
	UInt32		m_unk154;				// 154
};

class BGSLoadGameBuffer
{
public:
	virtual UInt8			GetSaveFormVersion(void);	// replaced in descendant 
	virtual TESForm*		getForm(void);				// only implemented in descendants
	virtual TESObjectREFR*	getREFR(void);				// only implemented in descendants
	virtual Actor*			getActor(void);				// only implemented in descendants

	UInt8		*chunk;			// 04
	UInt32		chunkSize;		// 08
	UInt32		chunkConsumed;	// 0C

	__forceinline void Read(void *buffer, size_t size)
	{
		ThisCall(0x864820, this, buffer, size);
	}

	inline UInt32 ReadFormID()
	{
		UInt32 outID = 0;
		Read(&outID, 3);
		return ThisCall<UInt32>(0x853500, &outID);
	}

	inline void SkipBytes(UInt32 size)
	{
		chunkConsumed += size + 1;
	}

	inline UInt8 Read8()
	{
		UInt8 res = chunk[chunkConsumed];
		chunkConsumed += 2;
		return res;
	}

	inline UInt16 Read16()
	{
		UInt16 res = *(UInt16*)(chunk + chunkConsumed);
		chunkConsumed += 3;
		return res;
	}

	inline UInt32 Read32()
	{
		UInt32 res = *(UInt32*)(chunk + chunkConsumed);
		chunkConsumed += 5;
		return res;
	}

	inline void ReadBuff(void *buff, UInt32 size)
	{
		MemCopy(buff, chunk + chunkConsumed, size);
		chunkConsumed += size + 1;
	}

	__forceinline static void DecodeRefID(UInt32 *pRefID)
	{
		*pRefID = ThisCall<UInt32>(0x853500, pRefID);
	}
};

struct BGSFormChange
{
	UInt32	changeFlags;
	UInt32	unk004;			// Pointer to the changed record or the save record ?
};

struct FormBufferHeader
{
	UInt32		encodeID;			// 00
	UInt32		changeFlags;		// 03
	UInt8		codedTypeAndLength;	// 07
	UInt8		formVersion;		// 08
	UInt8		pad009[2];			// 09

	__forceinline UInt32 GetRefID()
	{
		return ThisCall<UInt32>(0x853500, this);
	}
};
static_assert(sizeof(FormBufferHeader) == 0xC);

// 030
class BGSLoadFormBuffer : public BGSLoadGameBuffer
{
public:
	UInt32				refID;				// 10
	FormBufferHeader	header;				// 14
	UInt32				bufferSize;			// 20
	TESForm				*form;				// 24
	UInt32				flg28;				// 28	bit1 form invalid
	BGSFormChange		*currentFormChange;	// 2C

	UInt32 GetRefID()
	{
		return form ? form->refID : header.GetRefID();
	}
};
static_assert(sizeof(BGSLoadFormBuffer) == 0x30);

// 14
class BGSSaveGameBuffer
{
public:
	virtual TESForm	*GetForm(void);
	virtual TESObjectREFR	*GetRefr(void);
	virtual Actor	*GetActor(void);

	UInt8		*chunk;			// 04
	UInt32		chunkSize;		// 08
	UInt32		chunkConsumed;	// 0C
	UInt32		count10;		// 10

	__forceinline void Write(void *source, size_t size)
	{
		ThisCall(0x865CE0, this, source, size, chunkConsumed, 0);
	}

	__forceinline static void EncodeRefID(UInt32 *pRefID)
	{
		ThisCall(0x853570, pRefID, *pRefID);
	}

	inline void __fastcall WriteRefID(UInt32 refID)
	{
		UInt32 encoded = 0;
		ThisCall(0x853570, &encoded, refID);
		ThisCall(0x865CE0, this, &encoded, 4, chunkConsumed, 0);
	}

	UInt8* Reserve(size_t size)
	{
		UInt32 required = chunkConsumed + size;
		if (required > chunkSize)
			ThisCall(0x8660B0, this);
		count10++;
		UInt8 *pos = chunk + chunkConsumed;
		chunkConsumed = required;
		return pos;
	}
};

// 24
class BGSSaveFormBuffer : public BGSSaveGameBuffer
{
public:
	FormBufferHeader	header;		// 14
	TESForm				*form;		// 20

	UInt32 GetRefID()
	{
		return form ? form->refID : header.GetRefID();
	}
};
static_assert(sizeof(BGSSaveFormBuffer) == 0x24);

typedef NiTPtrMap<BGSFormChange> BGSSaveLoadChangesMap;
BGSFormChange *BGSSaveLoadChangesMap::Lookup(UInt32 key) const;

typedef BGSSaveLoadChangesMap ChangesMap;

// 1C8 - only explicitly marked things are verified
class TESSaveLoadGame
{
public:
	struct CreatedObject {
		UInt32			refID;
		CreatedObject	* next;
	};

	ChangesMap					* changesMap;		// 000
	UInt32						unk004;				// 004
	InteriorCellNewReferencesMap	* intRefMap;	// 008
	ExteriorCellNewReferencesMap	* extRefMap00C;	// 00C
	ExteriorCellNewReferencesMap	* extRefMap010;	// 010
	void						* saveLoadBuffer;	// 014
	UInt32						unk018;				// 018
	UInt8						unk01C;				// 01C
	UInt8						pad01D[3];
	NiTArray<TESObjectREFR*>	* arr020;			// 020
	UInt32						unk024;				// 024
	UInt32						unk028;				// 028
	CreatedObject				createdObjectList;	// 02C data is formID - verified
	NiTPtrMap<void*>			* map034;			// 034
	UInt32				unk034[(0x58-0x44) >> 2];	// 044
	NumericIDBufferMap			* idMap058;			// 058
	NumericIDBufferMap			* idMap05C;			// 05C
	NumericIDBufferMap			* idMap060;			// 060
	NumericIDBufferMap			* idMap064;			// 064
	UInt32						unk068;				// 068
	UInt32						unk06C;				// 06C
	UInt32						unk070;				// 070
	UInt8						unk074;				// 074
	UInt8						unk075;				//     init to 0x7D
	UInt8						pad076[2];
	NiTArray<UInt32>			* array078;			// 078 NiTLargePrimitiveArray<?>
	NiTArray<UInt32>			* array07C;			// 07C NiTLargePrimitiveArray<?>	
	UInt8						unk080;				// 080 version of save?
	UInt8						unk081;
	UInt8						pad082[2];
	UInt32				unk084[(0xAC-0x84) >> 2];	// 084
	UInt8						unk0AC;				// 0AC
	UInt8						unk0AD;				// 0AD
	UInt8						unk0AE;				// 0AE
	UInt8						unk0AF;				// 0AF
	UInt32				unk0B0[(0x1C8-0x0B0) >> 2];	// 0B0

	__forceinline static TESSaveLoadGame *GetSingleton() {return *(TESSaveLoadGame**)0x11DE45C;}

	__forceinline void AddCreatedForm(TESForm *pForm)
	{
		return ThisCall(0x861780, this, pForm);
	}
};

const UInt32 _SaveGameManager_ConstructSavegameFilename = 0x0084FF90;
const UInt32 _SaveGameManager_ConstructSavegamePath		= 0x0084FF30;

class BGSCellNumericIDArrayMap;
class BGSLoadGameSubBuffer;
class BGSReconstructFormsInFileMap;
class BGSReconstructFormsInAllFilesMap;

class BGSSaveLoadGame	// 0x011DDF38
{
public:
	typedef UInt32	RefID;
	typedef UInt32	IndexRefID;
	struct RefIDIndexMapping	// reversible map between refID and loaded form index
	{
		NiTMap<RefID, IndexRefID>	map000;	// 000
		NiTMap<IndexRefID, RefID>	map010;	// 010
		UInt32			            countRefID;	// 020
	};

	struct SaveChapters	// 06E	chapter table in save
	{
		struct RefIDArray	// List of all refID referenced in save for tranlation in RefIDIndexMapping
		{
			UInt32	count;	// 000
			RefID	IDs[1];	// 004
		};

		RefIDArray	*arr000;	// 000
		RefIDArray	*arr004;	// 004

	};

	struct BGSSaveLoadReferencesMap
	{
		NiTPtrMap<UInt32>						* map000;	// 000
		BGSCellNumericIDArrayMap				* map010;	// 010
		NiTPtrMap<BGSCellNumericIDArrayMap*>	* map020;	// 020
	};

	BGSSaveLoadChangesMap					*changesMap;			// 000
	BGSSaveLoadChangesMap					*previousChangeMap;		// 004
	RefIDIndexMapping						*refIDmapping;			// 008
	RefIDIndexMapping						*visitedWorldspaces;	// 00C
	BGSSaveLoadReferencesMap				*referencesMap;			// 010
	NiTMap<TESForm*, BGSLoadGameSubBuffer>	*maps014;				// 014
	NiTMap<UInt32, UInt32>					*map018;				// 018
	BSSimpleArray<char*>					*strings;				// 01C
	BGSReconstructFormsInAllFilesMap		*rfiafMap;				// 020
	BSSimpleArray<BGSLoadFormBuffer*>		changedForms;			// 024
	NiTPtrMap<Actor*>						map03C;					// 034 Either dead or not dead actors
	UInt8									saveMods[255];			// 044
	UInt8									loadedMods[255];		// 143
	UInt16									pad242;					// 242
	UInt32									flags;					// 244 bit 6 block updating player position/rotation from save, bit 2 set during save
	UInt8									formVersion;			// 248
	UInt8									pad249[3];				// 249

	__forceinline static BGSSaveLoadGame *GetSingleton() {return *(BGSSaveLoadGame**)0x11DDF38;}

	inline bool IsLoading() const {return (flags & 2) != 0;}

	UInt32 __fastcall EncodeRefID(UInt32 *pRefID);
	UInt32 __fastcall DecodeRefID(UInt32 *pRefID);
};
static_assert(sizeof(BGSSaveLoadGame) == 0x24C);

class SaveGameManager
{
public:
	static SaveGameManager* GetSingleton();

	struct SaveGameData
	{
		const char	* name;		// 00
		UInt32		unk04;		// 04
		UInt32		saveNumber;	// 08 index?
		const char	* pcName;	// 0C
		const char	* pcTitle;	// 10
		const char	* location;	// 14
		const char	* time;		// 18
	};

	tList<SaveGameData>		* saveList;		// 00
	UInt32					numSaves;		// 04
	UInt32					unk08;			// 08
	UInt8					byte0C;			// 0C	flag for either opened or writable or useSeparator (|)
	UInt8					byte0D;
	UInt8					byte0E;
	UInt8					byte0F;
	UInt8					byte10;
	UInt8					byte11;
	UInt8					pad12[2];
	UInt32					unk14;
	UInt32					unk18;
	UInt8					byte1C;
	UInt8					pad1D[3];
	UInt32					unk20;
	UInt8					byte24;
	UInt8					byte25;
	UInt8					byte26;
	UInt8					pad27;
	UInt32					unk28;
	UInt32					unk2C;
	String					saveName;
};

enum Coords
{
	kCoords_X = 0,	// 00
	kCoords_Y,		// 01
	kCoords_Z,		// 02
	kCoords_Max		// 03
};

struct NavMeshVertex
{
	float coords[kCoords_Max];	// 000
};	// 00C

enum Vertices
{
	kVertices_0 = 0,	// 00
	kVertices_1,		// 01
	kVertices_2,		// 02
	kVertices_Max		// 03
};

enum Sides
{
	kSides_0_1 = 0,	// 00
	kSides_1_2,		// 01
	kSides_2_0,		// 02
	kSides_Max		// 03
};

struct NavMeshTriangle
{
	SInt16	verticesIndex[kVertices_Max];	// 000
	SInt16	sides[kSides_Max];				// 006
	UInt32	flags;							// 00C
};	// Alloc'd by 0x10

struct NavMeshInfo;

struct EdgeExtraInfo
{
	struct Connection
	{
		NavMeshInfo*	navMeshInfo;
		SInt16			triangle;
	};

	UInt32	unk000;			// 00
	Connection connectTo;	// 04
};	// Alloc'd by 0x0C

struct NavMeshTriangleDoorPortal
{
	TESObjectREFR	* door;	// 00
	UInt16			unk004;	// 04
	UInt16			pad006;	// 06
};	// Alloc'd to 0x08

struct NavMeshCloseDoorInfo
{
	UInt32	unk000;	// 00
	UInt32	unk004;	// 04
};	// Alloc'd to 0x08

struct NavMeshPOVData;
class ObstacleData;
struct ObstacleUndoData;

struct NavMeshStaticAvoidNode
{
	UInt32	unk000;	// 00
	UInt32	unk004;	// 04
	UInt32	unk008;	// 08
	UInt32	unk00C;	// 0C
	UInt32	unk010;	// 10
	UInt32	unk014;	// 14
	UInt32	unk018;	// 18
	UInt32	unk01C;	// 1C
	UInt32	unk020;	// 20
	UInt32	unk024;	// 24
};	// Alloc'd to 0x28
