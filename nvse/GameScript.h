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
		RefVariable *GetRefVariableByName(const char *name);
		UInt32 GetIndex(RefVariable *refVar);
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
		UInt32	unk0;			// 00 (18)
		UInt32	numRefs;		// 04 (1C)
		UInt32	dataLength;		// 08 (20)
		UInt32	varCount;		// 0C (24)
		UInt16	type;			// 10 (28)
		UInt16	unk12;			// 12 (2A)
	};

	enum
	{
		eType_Object =	0,
		eType_Quest =	1,
		eType_Magic =	0x100,
		eType_Unk =		0x10000,
	};

	ScriptInfo		info;					// 18
	char			*text;					// 2C
	UInt8			*data;					// 30
	float			unk34;					// 34
	float			questDelayTimeCounter;	// 38      - init'd to fQuestDelayTime, decremented by frametime each frame
	float			secondsPassed;			// 3C      - only if you've modified fQuestDelayTime
	TESQuest		*quest;					// 40
	RefVarList		refList;				// 44
	VarInfoList		varList;				// 4C

	RefVariable *GetVariable(UInt32 reqIdx);
	VariableInfo *GetVariableInfo(UInt32 idx);

	UInt32 AddVariable(TESForm *form);
	void CleanupVariables() {refList.RemoveAll();}

	UInt32 Type() const {return info.type;}
	bool IsObjectScript() const {return info.type == eType_Object;}
	bool IsQuestScript() const {return info.type == eType_Quest;}
	bool IsMagicScript() const {return info.type == eType_Magic;}
	bool IsUnkScript() const {return info.type == eType_Unk;}

	VariableInfo *GetVariableByName(const char *varName);
	ScriptVar *AddVariable(char *varName, ScriptLocals *eventList, UInt32 ownerID, UInt8 modIdx);
	UInt32 GetDataLength();

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

	bool Execute(TESObjectREFR *thisObj = nullptr, ScriptLocals *eventList = nullptr, TESObjectREFR *containingObj = nullptr, bool arg3 = true);

	__forceinline bool Run(void *scriptContext, TESObjectREFR *object)
	{
		return ThisCall<bool>(0x5AC400, this, scriptContext, true, object);
	}

	bool Init(char *scrText);
	static Script *Create(char *scrText);
	bool Compile();

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

struct ConditionEntry
{
	struct Data
	{
		union Param
		{
			float	number;
			TESForm	* form;
		};

		// ### TODO: this
		UInt32		operatorAndFlags;	// 00
		float		comparisonValue;	// 04
		UInt16		functionIndex;		// 08 is opcode & 0x0FFF
		UInt16		unk0A;
		Param		param1;				// 0C
		Param		param2;				// 10
		UInt32		unk14;
	};

	Data			* data;
	ConditionEntry	* next;
};

// 70
struct QuestStageItem
{
	UInt32			unk00;			// 00
	ConditionEntry	conditionList;	// 04
	Script			resultScript;	// 0C
	UInt32			unk60;			// 60 disk offset to log text records? consistent within a single quest
	UInt8			index;			// 64 sequential
	bool			hasLogText;		// 65
	UInt8			pad66[2];		// 66 pad?
	UInt32			logDate;		// 68
	TESQuest		*owningQuest;	// 6C;
};
static_assert(sizeof(QuestStageItem) == 0x70);

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