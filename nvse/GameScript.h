#pragma once

void SuppressConsoleOutput();

// 54
class Script : public TESForm
{
public:
	struct RefVariable
	{
		String		name;		// 00 variable name/editorID (not used at run-time)
		TESForm		*form;		// 08
		UInt32		varIdx;		// 0C always zero in editor

		void Resolve(ScriptLocals *eventList);
	};

	struct RefVarList : tList<RefVariable>
	{
		RefVariable *GetRefVariableByName(const char *name) const;
		UInt32 GetIndex(RefVariable *refVar) const;
	};

	enum
	{
		eVarType_Float = 0,			//ref is also zero
		eVarType_Integer,

		// NVSE, return values only
		eVarType_String,
		eVarType_Array,
		eVarType_Ref,

		eVarType_Invalid
	};

	typedef tList<VariableInfo> VarInfoList;

	// 14
	struct ScriptInfo
	{
		UInt32	lastID;			// 00 (18)
		UInt32	numRefs;		// 04 (1C)
		UInt32	dataLength;		// 08 (20)
		UInt32	varCount;		// 0C (24)
		bool	isQuestScr;		// 10 (28)
		bool	isEffectScr;	// 11 (29)
		bool	isCompiled;		// 12 (2A)
		UInt8	byte13;			// 13 (2B)
	};

	ScriptInfo		info;					// 18
	char			*text;					// 2C
	UInt8			*data;					// 30
	float			profilerTimer;			// 34
	float			questDelayTimeCounter;	// 38      - init'd to fQuestDelayTime, decremented by frametime each frame
	float			secondsPassed;			// 3C      - only if you've modified fQuestDelayTime
	TESQuest		*quest;					// 40
	RefVarList		refList;				// 44
	VarInfoList		varList;				// 4C

	RefVariable *GetVariable(UInt32 reqIdx) const;
	VariableInfo *GetVariableInfo(UInt32 idx) const;

	UInt32 AddVariable(TESForm *form);
	void CleanupVariables() {refList.RemoveAll();}

	bool IsObjectScript() const {return !USHT(info.isQuestScr);}
	bool IsQuestScript() const {return info.isQuestScr;}
	bool IsMagicScript() const {return info.isEffectScr;}

	VariableInfo *GetVariableByName(const char *varName) const;
	ScriptVar *AddVariable(char *varName, ScriptLocals *eventList, UInt32 ownerID, UInt8 modIdx);
	UInt32 GetDataLength() const;

	/*Script() {Constructor();}
	~Script() {Destructor();}*/

	__forceinline Script *Constructor()
	{
		return ThisCall<Script*>(0x5AA0F0, this);
	}
	__forceinline void Destructor()
	{
		ThisCall(0x5AA1A0, this);
	}

	__forceinline bool Execute(TESObjectREFR *thisObj = nullptr, ScriptLocals *eventList = nullptr, TESObjectREFR *containingObj = nullptr, bool arg3 = true)
	{
		return ThisCall<bool>(0x5AC1E0, this, thisObj, eventList, containingObj, arg3);
	}

	__forceinline bool Run(void *scriptContext, TESObjectREFR *object)
	{
		return ThisCall<bool>(0x5AC400, this, scriptContext, true, object);
	}

	bool Init(char *scrText, const char *scrName);
	static Script *Create(char *scrText, const char *scrName);
	bool Compile(const char *scrName);

	__forceinline ScriptLocals *CreateEventList()
	{
		return ThisCall<ScriptLocals*>(0x5ABF60, this);
	}
};
static_assert(sizeof(Script) == 0x54);

struct ScriptRunner
{
	enum IfFlag
	{
		kIfFlag_IF =		1,
		kIfFlag_ELSEIF =	2
	};

	UInt32				unk00;				// 00
	TESForm				*baseForm;			// 04
	ScriptLocals		*eventList;			// 08
	UInt32				unk0C;				// 0C
	UInt32				unk10;				// 10
	Script				*script;			// 14
	UInt32				unk18;				// 18	Set to 6 after a failed expression evaluation
	UInt32				unk1C;				// 1C	Set to Expression::errorCode
	UInt32				ifStackDepth;		// 20
	UInt32				ifStackFlags[10];	// 24
	UInt32				ifStack2Depth;		// 4C
	UInt32				ifStack2Flags[10];	// 50
	UInt32				ifStack3Flags[10];	// 78
	UInt8				invalidRefs;		// A0	Set when the dot operator fails to resolve a reference (inside the error message handler)
	UInt8				byteA1;				// A1	Set when the executing CommandInfo's 2nd flag bit (+0x25) is set
	UInt8				padA2[2];			// A2
};
static_assert(sizeof(ScriptRunner) == 0xA4);

// 74
struct QuestLogEntry
{
	UInt32			unk00;			// 00
	ConditionList	conditions;		// 04
	Script			resultScript;	// 0C
	UInt32			unk60;			// 60	disk offset to log text records? consistent within a single quest
	UInt8			index;			// 64
	bool			hasLogText;		// 65
	UInt8			pad66[2];		// 66
	UInt32			*date;			// 68	ptr to 4 byte struct storing day/month/year
	TESQuest		*owningQuest;	// 6C
	UInt32			unk70;			// 70
};
static_assert(sizeof(QuestLogEntry) == 0x74);

// 78
struct TerminalEntry
{
	String			entryText;		// 00
	String			resultText;		// 08
	Script			resultScript;	// 10
	ConditionList	conditions;		// 64
	BGSNote			*displayNote;	// 6C
	BGSTerminal		*subMenu;		// 70
	UInt8			byte74;			// 74
	UInt8			pad75[3];		// 75
};
static_assert(sizeof(TerminalEntry) == 0x78);

// 41C
struct ScriptLineBuffer
{
	static const UInt32	kBufferSize = 0x200;

	UInt32				lineNumber;			// 000 counts blank lines too
	char				paramText[0x200];	// 004 portion of line text following command
	UInt32				paramTextLen;		// 204
	UInt32				lineOffset;			// 208
	UInt8				dataBuf[0x200];		// 20C
	UInt32				dataOffset;			// 40C
	UInt32				cmdOpcode;			// 410 not initialized. Opcode of command being parsed
	UInt32				callingRefIndex;	// 414 not initialized. Zero if cmd not invoked with dot syntax
	UInt32				errorCode;			// 418

	// these write data and update dataOffset
	bool Write(const void* buf, UInt32 bufsize);
	bool WriteFloat(double buf);
	bool WriteString(const char* buf);
	bool Write32(UInt32 buf);
	bool Write16(UInt16 buf);
	bool WriteByte(UInt8 buf);
};

// size 0x58? Nothing initialized beyond 0x50.
struct ScriptBuffer
{
	ScriptBuffer() {ThisCall(0x5AE490, this);}
	~ScriptBuffer() {ThisCall(0x5AE5C0, this);}

	char					*scriptText;		// 00
	UInt32					textOffset;			// 04 
	UInt32					runtimeMode;		// 08	0 - Editor; 1 - GameConsole
	String					scriptName;			// 0C
	UInt32					errorCode;			// 14
	bool					partialScript;		// 18
	UInt8					pad19[3];			// 19
	UInt32					curLineNumber;		// 1C 
	UInt8					*scriptData;		// 20	Ptr to 0x4000-byte array
	UInt32					dataOffset;			// 24
	Script::ScriptInfo		info;				// 28
	Script::VarInfoList		vars;				// 3C
	Script::RefVarList		refVars;			// 44
	Script					*currScript;		// 4C
	tList<ScriptLineBuffer>	lines;				// 50
	// nothing else initialized

	UInt32 GetRefIdx(Script::RefVariable *refVar) {return refVars.GetIndex(refVar);}
};