#pragma once

extern ExpressionEvaluatorUtils s_expEvalUtils;

typedef bool (*_WriteRecord)(UInt32 type, UInt32 version, const void *buffer, UInt32 length);
extern _WriteRecord WriteRecord;
typedef bool (*_WriteRecordData)(const void *buffer, UInt32 length);
extern _WriteRecordData WriteRecordData;
typedef bool (*_GetNextRecordInfo)(UInt32 *type, UInt32 *version, UInt32 *length);
extern _GetNextRecordInfo GetNextRecordInfo;
typedef UInt32 (*_ReadRecordData)(void *buffer, UInt32 length);
extern _ReadRecordData ReadRecordData;
typedef bool (*_ResolveRefID)(UInt32 refID, UInt32 *outRefID);
extern _ResolveRefID ResolveRefID;
typedef const char* (*_GetSavePath)(void);
extern _GetSavePath GetSavePath;
typedef void (*_WriteRecord8)(UInt8 inData);
extern _WriteRecord8 WriteRecord8;
typedef void (*_WriteRecord16)(UInt16 inData);
extern _WriteRecord16 WriteRecord16;
typedef void (*_WriteRecord32)(UInt32 inData);
extern _WriteRecord32 WriteRecord32;
typedef void (*_WriteRecord64)(const void *inData);
extern _WriteRecord64 WriteRecord64;
typedef UInt8 (*_ReadRecord8)();
extern _ReadRecord8 ReadRecord8;
typedef UInt16 (*_ReadRecord16)();
extern _ReadRecord16 ReadRecord16;
typedef UInt32 (*_ReadRecord32)();
extern _ReadRecord32 ReadRecord32;
typedef void (*_ReadRecord64)(void *outData);
extern _ReadRecord64 ReadRecord64;
typedef CommandInfo* (*_GetCmdByOpcode)(UInt32 opcode);
extern _GetCmdByOpcode GetCmdByOpcode;
typedef const char* (*_GetStringVar)(UInt32 stringID);
extern _GetStringVar GetStringVar;
typedef bool (*_AssignString)(COMMAND_ARGS, const char *newValue);
extern _AssignString AssignString;
typedef NVSEArrayVar* (*_CreateArray)(const NVSEArrayElement *data, UInt32 size, Script *callingScript);
extern _CreateArray CreateArray;
typedef NVSEArrayVar* (*_CreateStringMap)(const char **keys, const NVSEArrayElement *values, UInt32 size, Script *callingScript);
extern _CreateStringMap CreateStringMap;
typedef bool (*_AssignCommandResult)(NVSEArrayVar *arr, double *dest);
extern _AssignCommandResult AssignCommandResult;
typedef void (*_SetElement)(NVSEArrayVar *arr, const NVSEArrayElement &key, const NVSEArrayElement &value);
extern _SetElement SetElement;
typedef void (*_AppendElement)(NVSEArrayVar *arr, const NVSEArrayElement &value);
extern _AppendElement AppendElement;
typedef UInt32 (*_GetArraySize)(NVSEArrayVar *arr);
extern _GetArraySize GetArraySize;
typedef NVSEArrayVar* (*_LookupArrayByID)(UInt32 id);
extern _LookupArrayByID LookupArrayByID;
typedef bool (*_GetElement)(NVSEArrayVar *arr, const NVSEArrayElement &key, NVSEArrayElement &outElement);
extern _GetElement GetElement;
typedef bool (*_GetElements)(NVSEArrayVar *arr, NVSEArrayElement *elements, NVSEArrayElement *keys);
extern _GetElements GetElements;
typedef bool (*_ExtractArgsEx)(COMMAND_ARGS_EX, ...);
extern _ExtractArgsEx ExtractArgsEx;
typedef bool (*_ExtractFormatStringArgs)(UInt32 fmtStringPos, char *buffer, COMMAND_ARGS_EX, UInt32 maxParams, ...);
extern _ExtractFormatStringArgs ExtractFormatStringArgs;
typedef bool (*_CallFunction)(Script *funcScript, TESObjectREFR *callingObj, UInt8 numArgs, ...);
extern _CallFunction CallFunction;
typedef void (*_CaptureLambdaVars)(Script* scriptLambda);
extern _CaptureLambdaVars CaptureLambdaVars;
typedef void (*_UncaptureLambdaVars)(Script* scriptLambda);
extern _UncaptureLambdaVars UncaptureLambdaVars;

extern UInt8 *g_numPreloadMods;

#define MSGBOX_ARGS 0, 0, ShowMessageBox_Callback, 0, 0x17, 0, 0, "OK", NULL

struct GameGlobals
{
	__forceinline static const char **TerminalModelPtr() {return (const char**)0x11A0BB0;}
	__forceinline static TESDescription **CurrentDescription() {return (TESDescription**)0x11C5490;}
	__forceinline static String *CurrentDescriptionText() {return (String*)0x11C5498;}
	__forceinline static NiTPointerMap<TESForm> *AllFormsMap() {return *(NiTPointerMap<TESForm>**)0x11C54C0;}
	__forceinline static NiTStringPointerMap<TESForm> *EditorIDsMap() {return *(NiTStringPointerMap<TESForm>**)0x11C54C8;}
	__forceinline static BSTCaseInsensitiveStringMap<void*> *IdleAnimsDirectoryMap() {return *(BSTCaseInsensitiveStringMap<void*>**)0x11CB6A0;}
	__forceinline static BSSimpleArray<TESRecipeCategory> *RecipeMenuCategories() {return (BSSimpleArray<TESRecipeCategory>*)0x11D8F08;}
	__forceinline static TESObjectWEAP *PlayerWeapon() {return *(TESObjectWEAP**)0x11D98D4;}
	__forceinline static tList<VATSTargetInfo> *VATSTargetList() {return (tList<VATSTargetInfo>*)0x11DB150;}
	__forceinline static NiNode *ObjectLODRoot() {return *(NiNode**)0x11DEA18;}
	__forceinline static bool *GamePadRumble() {return (bool*)0x11E0854;}
	__forceinline static UInt32 TickCount() {return *(UInt32*)0x11F63A8;}
	__forceinline static tList<Archive> *ArchivesList() {return *(tList<Archive>**)0x11F8160;}
	__forceinline static LightCS *SceneLightsLock() {return (LightCS*)0x11F9EA0;}
	__forceinline static tList<ListBox<int>> *ActiveListBoxes() {return (tList<ListBox<int>>*)0x11D8B54;}
	__forceinline static tList<GradualSetFloat> *QueuedGradualSetFloat() {return (tList<GradualSetFloat>*)0x11F3348;}
	__forceinline static RadioEntry *PipboyRadio() {return *(RadioEntry**)0x11DD42C;}
};

extern SpellItem *g_pipBoyLight;
extern void *g_scrapHeapQueue;
extern NiNode *g_cursorNode;
extern float g_screenResConvert, g_screenWidth, g_screenHeight;
extern const char *g_terminalModelDefault;
extern TESObjectWEAP *g_fistsWeapon;
extern TESObjectACTI *g_ashPileACTI, *g_gooPileACTI;
extern TESGlobal *g_gameYear, *g_gameMonth, *g_gameDay, *g_gameHour;
extern TESObjectMISC *g_capsItem;
extern TESImageSpaceModifier *g_getHitIMOD, *g_explosionInFaceIMOD;
extern double *g_condDmgPenalty;
extern double s_condDmgPenalty;
extern UInt32 s_mainThreadID, s_initialTickCount;

__forceinline TESObjectREFR *PlaceAtMe(TESObjectREFR *refr, TESForm *form, UInt32 count, UInt32 distance, UInt32 direction, float health)
{
	return CdeclCall<TESObjectREFR*>(0x5C4B30, refr, form, count, distance, direction, health);
}
__forceinline NiNode *GetCdBodyNode(hkCdBody *cdBody)
{
	return CdeclCall<NiNode*>(0xC7FA90, cdBody);
}
__forceinline TESObjectREFR *GetCdBodyRef(hkCdBody *cdBody)
{
	return CdeclCall<TESObjectREFR*>(0x62B4E0, cdBody);
}
__forceinline void RefreshItemListBox()
{
	CdeclCall(0x704AF0);
}
__forceinline float ApplyAmmoEffects(UInt32 effType, tList<TESAmmoEffect> *effList, float baseValue)
{
	return CdeclCall<float>(0x59A030, effType, effList, baseValue);
}
__forceinline void ApplyPerkModifiers(UInt32 entryPointID, TESObjectREFR *perkOwner, float *baseValue)
{
	CdeclCall(0x5E58F0, entryPointID, perkOwner, baseValue);
}
__forceinline void ApplyPerkModifiers(UInt32 entryPointID, TESObjectREFR *perkOwner, TESForm *filter1, float *baseValue)
{
	CdeclCall(0x5E58F0, entryPointID, perkOwner, filter1, baseValue);
}
__forceinline void ApplyPerkModifiers(UInt32 entryPointID, TESObjectREFR *perkOwner, TESForm *filter1, TESForm *filter2, float *baseValue)
{
	CdeclCall(0x5E58F0, entryPointID, perkOwner, filter1, filter2, baseValue);
}
__forceinline TESTopicInfo *GetTopicInfo(TESTopic *topic, bool *result, Actor *actor, Actor *target)
{
	return CdeclCall<TESTopicInfo*>(0x61A7D0, topic, result, actor, target, true, NULL, NULL);
}
__forceinline ImageSpaceModifierInstanceForm *ApplyIMOD(TESImageSpaceModifier *imod, float percent, NiObject *obj10)
{
	return CdeclCall<ImageSpaceModifierInstanceForm*>(0x5299A0, imod, percent, obj10);
}
__forceinline void *PurgeTerminalModel()
{
	return CdeclCall<void*>(0x7FFE00);
}
__forceinline TileMenu *ShowQuantityMenu(int maxCount, void (*callback)(int), int defaultCount)
{
	return CdeclCall<TileMenu*>(0x7ABA00, maxCount, callback, defaultCount);
}
__forceinline void *NiAllocator(UInt32 size)
{
	return CdeclCall<void*>(0xAA13E0, size);
}
__forceinline void *NiDeallocator(void *blockPtr, UInt32 size)
{
	return CdeclCall<void*>(0xAA1460, blockPtr, size);
}

extern Cmd_Execute SayTo, KillActor, AddNote, AttachAshPile, MoveToFade, GetRefs;

enum
{
	kChangedFlag_AuxVars =		1 << 0,
	kChangedFlag_RefMaps =		1 << 1,
	kChangedFlag_LinkedRefs =	1 << 2,
	kChangedFlag_NPCPerks =		1 << 3,

	kChangedFlag_All =			kChangedFlag_AuxVars | kChangedFlag_RefMaps | kChangedFlag_LinkedRefs | kChangedFlag_NPCPerks,

	kSerializedFlag_NoHardcoreTracking =	1 << 0,
};

extern UInt32 s_serializedFlags;

struct QueuedCmdCall
{
	UInt32			opcode;		// 00
	void			*cmdAddr;	// 04
	UInt32			thisObj;	// 08	refID
	UInt32			numArgs;	// 0C
	FunctionArg		args[4];	// 10

	QueuedCmdCall(void *_cmdAddr, UInt32 _thisObj, UInt8 _numArgs) : opcode(0x2B), cmdAddr(_cmdAddr), thisObj(_thisObj), numArgs(_numArgs) {}
};

#define AddQueuedCmdCall(qCall) ThisCall(0x87D160, g_scrapHeapQueue, &qCall)

class LambdaVariableContext
{
	Script* scriptLambda;
public:

	LambdaVariableContext(const LambdaVariableContext& other) = delete;
	LambdaVariableContext& operator=(const LambdaVariableContext& other) = delete;

	explicit LambdaVariableContext::LambdaVariableContext(Script* scriptLambda) : scriptLambda(scriptLambda)
	{
		if (scriptLambda)
			CaptureLambdaVars(scriptLambda);
	}

	LambdaVariableContext::LambdaVariableContext(LambdaVariableContext&& other) noexcept : scriptLambda(other.scriptLambda)
	{
		other.scriptLambda = nullptr;
	}

	LambdaVariableContext& LambdaVariableContext::operator=(LambdaVariableContext&& other) noexcept
	{
		if (this == &other)
			return *this;
		if (this->scriptLambda)
			UncaptureLambdaVars(this->scriptLambda);
		scriptLambda = other.scriptLambda;
		other.scriptLambda = nullptr;
		return *this;
	}

	LambdaVariableContext::~LambdaVariableContext()
	{
		if (this->scriptLambda)
			UncaptureLambdaVars(this->scriptLambda);
	}
};

typedef Set<TESForm*> TempFormList;
TempFormList *GetTempFormList();

typedef Vector<ArrayElementL> TempElements;
TempElements *GetTempElements();

UInt32 __fastcall StringToRef(char *refStr);

struct InventoryItemData
{
	SInt32				count;
	ContChangesEntry	*entry;

	InventoryItemData(SInt32 _count, ContChangesEntry *_entry) : count(_count), entry(_entry) {}
};

typedef UnorderedMap<TESForm*, InventoryItemData> InventoryItemsMap;
InventoryItemsMap *GetInventoryItemsMap();

bool GetInventoryItems(TESObjectREFR *refr, UInt8 typeID, InventoryItemsMap *invItemsMap);

NiAVObject* __stdcall GetRayCastObject(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter);

int __stdcall GetRayCastMaterial(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter);

struct AppearanceUndo
{
	float					values0[50];
	float					values1[30];
	float					values2[50];
	UInt32					flags;
	float					hairLength;
	UInt32					hairColor;
	TESRace					*race;
	TESHair					*hair;
	TESEyes					*eyes;
	BGSHeadPart				**headParts;
	UInt8					numParts;

	AppearanceUndo(TESNPC *npc) : flags(npc->baseData.flags), hairLength(npc->hairLength), hairColor(npc->hairColor), race(npc->race.race), hair(npc->hair), eyes(npc->eyes), numParts(0)
	{
		memcpy(values0, npc->faceGenData[0].values, sizeof(values0));
		memcpy(values1, npc->faceGenData[1].values, sizeof(values1));
		memcpy(values2, npc->faceGenData[2].values, sizeof(values2));
		ListNode<BGSHeadPart> *partIter = npc->headPart.Head();
		do
		{
			if (partIter->data)
				numParts++;
		}
		while (partIter = partIter->next);
		if (numParts)
		{
			BGSHeadPart **ptr = headParts = (BGSHeadPart**)malloc(numParts * 4);
			partIter = npc->headPart.Head();
			do
			{
				if (*ptr = partIter->data)
					ptr++;
			}
			while (partIter = partIter->next);
		}
		else headParts = NULL;
	}

	void Undo(TESNPC *npc)
	{
		npc->copyFrom = NULL;
		npc->SetSex(flags);
		npc->SetRace(race);
		memcpy(npc->faceGenData[0].values, values0, sizeof(values0));
		memcpy(npc->faceGenData[1].values, values1, sizeof(values1));
		memcpy(npc->faceGenData[2].values, values2, sizeof(values2));
		npc->hairLength = hairLength;
		npc->hairColor = hairColor;
		npc->hair = hair;
		npc->eyes = eyes;
		npc->headPart.RemoveAll();
		if (numParts)
		{
			BGSHeadPart **ptr = headParts;
			UInt8 idx = numParts;
			do
			{
				npc->headPart.Prepend(*ptr);
				ptr++;
			}
			while (--idx);
		}
	}

	void Destroy()
	{
		if (headParts) free(headParts);
		free(this);
	}
};

extern UnorderedMap<TESNPC*, AppearanceUndo*> s_appearanceUndoMap;

extern UnorderedSet<TESGlobal*> s_resolvedGlobals;

hkpWorld *GethkpWorld();

bool LeveledListHasFormDeep(TESLeveledList *pLvlList, TESForm *form, TempFormList *tmpFormLst);

float GetDaysPassed(int bgnYear = 2281, int bgnMonth = 9, int bgnDay = 13);

void PlayGameSound(const char *soundEDID);

struct ScriptVariableEntry
{
	ScriptVar	*value;
	UInt8		modIdx;

	void Set(ScriptVar *_value, UInt8 _modIdx)
	{
		value = _value;
		modIdx = _modIdx;
	}
};
typedef UnorderedMap<char*, ScriptVariableEntry, 4> ScriptVariablesMap;
extern UnorderedMap<UInt32, ScriptVariablesMap> s_scriptVariablesBuffer;
typedef UnorderedSet<const char*> VariableNames;
extern UnorderedMap<UInt32, VariableNames> s_addedVariables;

bool __fastcall GetVariableAdded(UInt32 ownerID, char *varName);

struct LinkedRefEntry
{
	UInt32		linkID;
	UInt8		modIdx;

	void Set(UInt32 _linkID, UInt8 _modIdx)
	{
		linkID = _linkID;
		modIdx = _modIdx;
	}
};
extern UnorderedMap<UInt32, LinkedRefEntry> s_linkedRefModified;
extern UnorderedMap<UInt32, UInt32> s_linkedRefDefault, s_linkedRefsTemp;

bool SetLinkedRefID(UInt32 thisID, UInt32 linkID = 0, UInt8 modIdx = 0xFF);

extern UInt32 s_serializedVersion;

class AuxVariableValue
{
	UInt8		type;
	UInt8		pad01[3];
	UInt16		alloc;
	UInt16		length;
	union
	{
		double	num;
		UInt32	refID;
		char	*str;
	};

	void Clear()
	{
		if (alloc)
		{
			Pool_Free(str, alloc);
			alloc = 0;
		}
	}

public:
	AuxVariableValue() : alloc(0) {}
	AuxVariableValue(UInt8 _type) : type(_type), alloc(0) {}
	AuxVariableValue(NVSEArrayElement &elem) : alloc(0) {SetElem(elem);}

	~AuxVariableValue() {Clear();}

	UInt8 GetType() const {return type;}
	double GetFlt() const {return (type == 1) ? num : 0;}
	UInt32 GetRef() const {return (type == 2) ? refID : 0;}
	const char *GetStr() const {return alloc ? str : NULL;}

	void SetFlt(double value)
	{
		Clear();
		type = 1;
		num = value;
	}

	void SetRef(TESForm *value)
	{
		Clear();
		type = 2;
		refID = value ? value->refID : 0;
	}

	void SetStr(const char *value)
	{
		type = 4;
		length = StrLen(value);
		if (length)
		{
			UInt16 size = length + 1;
			if (alloc < size)
			{
				if (alloc) Pool_Free(str, alloc);
				alloc = AlignNumAlloc<char>(size);
				str = (char*)Pool_Alloc(alloc);
			}
			memcpy(str, value, size);
		}
		else if (alloc)
			*str = 0;
	}

	void SetElem(NVSEArrayElement &elem)
	{
		if (elem.GetType() == 2) SetRef(elem.form);
		else if (elem.GetType() == 3) SetStr(elem.str);
		else SetFlt(elem.num);
	}

	ArrayElementL GetAsElement() const
	{
		if (type == 2) return ArrayElementL(LookupFormByRefID(refID));
		if (type == 4) return ArrayElementL(GetStr());
		return ArrayElementL(num);
	}

	UInt8 *ReadValData(UInt8 *bufPos)
	{
		if (type == 1)
		{
			if (s_serializedVersion < 10)
			{
				num = *(float*)bufPos;
				bufPos += 4;
			}
			else
			{
				num = *(double*)bufPos;
				bufPos += 8;
			}
		}
		else if (type == 2)
		{
			refID = *(UInt32*)bufPos;
			bufPos += 4;
			ResolveRefID(refID, &refID);
		}
		else
		{
			length = *(UInt16*)bufPos;
			bufPos += 2;
			if (length)
			{
				alloc = AlignNumAlloc<char>(length + 1);
				str = (char*)Pool_Alloc(alloc);
				memcpy(str, bufPos, length);
				bufPos += length;
				str[length] = 0;
			}
		}
		return bufPos;
	}

	void WriteValData() const
	{
		WriteRecord8(type);
		if (type == 1)
			WriteRecord64(&num);
		else if (type == 2)
			WriteRecord32(refID);
		else
		{
			WriteRecord16(length);
			if (length) WriteRecordData(str, length);
		}
	}
};
STATIC_ASSERT(sizeof(AuxVariableValue) == 0x10);

typedef Vector<AuxVariableValue, 2> AuxVarValsArr;
typedef UnorderedMap<char*, AuxVarValsArr> AuxVarVarsMap;
typedef UnorderedMap<UInt32, AuxVarVarsMap> AuxVarOwnersMap;
typedef UnorderedMap<UInt32, AuxVarOwnersMap> AuxVarModsMap;
extern AuxVarModsMap s_auxVariablesPerm, s_auxVariablesTemp;

typedef UnorderedMap<UInt32, AuxVariableValue> RefMapIDsMap;
typedef UnorderedMap<char*, RefMapIDsMap> RefMapVarsMap;
typedef UnorderedMap<UInt32, RefMapVarsMap> RefMapModsMap;
extern RefMapModsMap s_refMapArraysPerm, s_refMapArraysTemp;

UInt32 __fastcall GetSubjectID(TESForm *form, TESObjectREFR *thisObj);

struct AuxVarInfo
{
	UInt32		ownerID;
	UInt32		modIndex;
	char		*varName;
	bool		isPerm;

	AuxVarInfo(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj, char *pVarName)
	{
		if (!pVarName[0])
		{
			ownerID = 0;
			return;
		}
		ownerID = GetSubjectID(form, thisObj);
		if (ownerID)
		{
			varName = pVarName;
			isPerm = (varName[0] != '*');
			modIndex = (varName[!isPerm] == '_') ? 0xFF : scriptObj->GetOverridingModIdx();
		}
	}

	AuxVarInfo(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj, UInt8 type)
	{
		ownerID = GetSubjectID(form, thisObj);
		if (ownerID)
		{
			isPerm = !(type & 1);
			modIndex = (type > 1) ? 0xFF : scriptObj->GetOverridingModIdx();
		}
	}

	AuxVarModsMap& ModsMap() {return isPerm ? s_auxVariablesPerm : s_auxVariablesTemp;}
};

struct RefMapInfo
{
	UInt32		modIndex;
	bool		isPerm;

	RefMapInfo(Script *scriptObj, char *varName)
	{
		isPerm = (varName[0] != '*');
		modIndex = (varName[!isPerm] == '_') ? 0xFF : scriptObj->GetOverridingModIdx();
	}

	RefMapInfo(Script *scriptObj, UInt8 type)
	{
		isPerm = !(type & 1);
		modIndex = (type > 1) ? 0xFF : scriptObj->GetOverridingModIdx();
	}

	RefMapModsMap& ModsMap() {return isPerm ? s_refMapArraysPerm : s_refMapArraysTemp;}
};

extern UInt8 s_dataChangedFlags;

struct EventCallbackScripts : Set<Script*, 4>
{
	void InvokeEvents(UInt32 arg);
	void InvokeEvents2(UInt32 arg1, UInt32 arg2);
	void InvokeEventsThis(TESObjectREFR *thisObj);
	void InvokeEventsThis1(TESObjectREFR *thisObj, UInt32 arg);
	void InvokeEventsThis2(TESObjectREFR *thisObj, UInt32 arg1, UInt32 arg2);
};

typedef Map<char*, EventCallbackScripts, 2> MenuClickFiltersMap;
typedef UnorderedMap<int, EventCallbackScripts> MenuClickIDsMap;

struct QuestStageEventCallback
{
	UInt8			stageID;
	UInt8			flags;
	UInt8			pad02[2];
	Script			*callback;

	QuestStageEventCallback(UInt8 _stageID, UInt8 _flags, Script *_callback) : stageID(_stageID), flags(_flags), callback(_callback) {}
	QuestStageEventCallback(const QuestStageEventCallback &copyFrom) {*this = copyFrom;}
};
typedef Vector<QuestStageEventCallback, 2> QuestStageCallbacks;

class QuestStageEventFinder
{
	QuestStageEventCallback	*m_callback;

public:
	QuestStageEventFinder(QuestStageEventCallback &_callback) : m_callback(&_callback) {}

	bool operator()(const QuestStageEventCallback &rhs)
	{
		return (m_callback->stageID == rhs.stageID) && (m_callback->callback == rhs.callback);
	}
};

struct AnimEventCallbacks : UnorderedMap<UInt32, EventCallbackScripts> {};

struct CriticalHitEventData
{
	Actor				*target;	// 00
	TESObjectREFR		*source;	// 04
	TESObjectWEAP		*weapon;	// 08
	Script				*callback;	// 0C

	CriticalHitEventData(Actor *_target, TESObjectREFR *_source, TESObjectWEAP *_weapon, Script *_callback) :
		target(_target), source(_source), weapon(_weapon), callback(_callback) {}
	CriticalHitEventData(const CriticalHitEventData &copyFrom) {*this = copyFrom;}
};

typedef Vector<CriticalHitEventData> CriticalHitEventCallbacks;
extern CriticalHitEventCallbacks s_criticalHitEvents;

class CriticalHitEventFind
{
	CriticalHitEventData	*data;

public:
	CriticalHitEventFind(CriticalHitEventData &_data) : data(&_data) {}

	bool operator()(const CriticalHitEventData &rhs)
	{
		return (data->callback == rhs.callback) && (!rhs.target || (data->target == rhs.target)) && 
			(!rhs.source || (data->source == rhs.source)) && (!rhs.weapon || (data->weapon == rhs.weapon));
	}
};

class CriticalHitEventRemove
{
	CriticalHitEventData	*data;

public:
	CriticalHitEventRemove(CriticalHitEventData &_data) : data(&_data) {}

	bool operator()(const CriticalHitEventData &rhs)
	{
		return (data->callback == rhs.callback) && (!data->target || (data->target == rhs.target)) && 
			(!data->source || (data->source == rhs.source)) && (!data->weapon || (data->weapon == rhs.weapon));
	}
};

struct DisabledBlockInfo
{
	UInt8		blockType;
	UInt8		pad01[3];
	UInt8		*typeOffset;

	DisabledBlockInfo(UInt8 *_typeOffset) : blockType(*_typeOffset), typeOffset(_typeOffset) {*typeOffset = 0xE;}
	~DisabledBlockInfo() {*typeOffset = blockType;}
};

typedef Vector<DisabledBlockInfo, 4> DisabledScriptBlocks;
extern UnorderedMap<Script*, DisabledScriptBlocks> s_disabledScriptBlocksMap;

class DisabledBlockFinder
{
	UInt8		blockType;

public:
	DisabledBlockFinder(UInt8 _blockType) : blockType(_blockType) {}

	bool operator()(const DisabledBlockInfo &rhs) {return rhs.blockType == blockType;}
};

class ScriptBlockIterator
{
	UInt8		*dataPtr;
	UInt32		length;
	UInt32		currOffset;
	UInt8		*typePtr;
	UInt32		nextOpOffset;

	bool EvalBlock()
	{
		if (*(UInt16*)(dataPtr + currOffset) != 0x10) return false;
		currOffset += 2;
		UInt16 opSize = *(UInt16*)(dataPtr + currOffset);
		currOffset += 2;
		typePtr = dataPtr + currOffset;
		nextOpOffset = currOffset + opSize;
		currOffset += 2;
		currOffset += *(UInt16*)(dataPtr + currOffset) + opSize - 6;
		if (*(UInt32*)(dataPtr + currOffset) == 0x11)
		{
			currOffset += 4;
			return true;
		}
		return false;
	}

public:
	explicit operator bool() const {return length != 0;}
	void operator++()
	{
		if (currOffset < length)
		{
			if (EvalBlock()) return;
			typePtr = NULL;
		}
		length = 0;
	}

	UInt8 *TypePtr() const {return typePtr;}
	UInt32 NextOpOffset() const {return nextOpOffset;}
	UInt32 NextBlockOffset() const {return currOffset;}

	ScriptBlockIterator(UInt8 *_dataPtr, UInt32 _length) : dataPtr(_dataPtr), length(_length), currOffset(4)
	{
		if (!dataPtr || (*(UInt32*)dataPtr != 0x1D) || !EvalBlock())
		{
			length = 0;
			typePtr = NULL;
		}
	}
};

struct TempArrayElements
{
	UInt32			size;
	ArrayElementR	*elements;
	bool			doFree;

	TempArrayElements(NVSEArrayVar *srcArr)
	{
		doFree = true;
		size = GetArraySize(srcArr);
		if (size)
		{
			elements = (ArrayElementR*)calloc(size, sizeof(ArrayElementR));
			GetElements(srcArr, elements, NULL);
		}
		else elements = NULL;
	}
	TempArrayElements(ArrayElementR *element) : size(1), elements(element), doFree(false) {}
	~TempArrayElements()
	{
		if (!doFree || !elements) return;
		ArrayElementR *dataPtr = elements;
		do
		{
			dataPtr->~ElementR();
			dataPtr++;
		}
		while (--size);
		free(elements);
	}
};

ArrayElementR* __fastcall GetArrayData(NVSEArrayVar *srcArr, UInt32 *size);

// 30
struct InventoryRef
{
	TESForm				*type;			// 00
	ContChangesEntry	*entry;			// 04
	ExtraDataList		*xData;			// 08
	TESObjectREFR		*containerRef;	// 0C
	TESObjectREFR		*tempRef;		// 10
	UInt8				pad14[24];		// 14
	bool				doValidation;	// 2C
	bool				removed;		// 2D
	UInt8				pad2E[2];		// 2E

	SInt32 GetCount() {return entry->countDelta;}
	ExtraDataList *CreateExtraData();
};

typedef TESObjectREFR* (__stdcall *_InventoryRefCreate)(TESObjectREFR *container, TESForm *itemForm, SInt32 countDelta, ExtraDataList *xData);
extern _InventoryRefCreate InventoryRefCreate;
typedef InventoryRef* (*_InventoryRefGetForID)(UInt32 refID);
extern _InventoryRefGetForID InventoryRefGetForID;

TESObjectREFR* __fastcall CreateRefForStack(TESObjectREFR *container, ContChangesEntry *menuEntry);

ExtraDataList* __fastcall SplitFromStack(ContChangesEntry *entry, ExtraDataList *xDataIn);

TESObjectREFR* __fastcall GetEquippedItemRef(Actor *actor, UInt32 slotIndex);

ContChangesEntry* __fastcall GetHotkeyItemEntry(UInt8 index, ExtraDataList **outXData);

bool __fastcall ClearHotkey(UInt8 index);

float __fastcall GetModBonuses(TESObjectREFR *wpnRef, UInt32 effectID);

struct AnimGroupClassify
{
	UInt8	category;	// 00
	UInt8	subType;	// 01
	UInt8	flags;		// 02
	UInt8	byte03;		// 03
};
extern const AnimGroupClassify kAnimGroupClassify[];

class JIPScriptRunner
{
	Script					*script;
	void					*scrContext;
	UnorderedSet<char*>		sourceFiles;

public:
	JIPScriptRunner() : script(NULL) {}
	~JIPScriptRunner() {free(script);}

	static void Init();
	static bool __stdcall RunScript(const char *scriptText, TESObjectREFR *thisObj = NULL);
	static void __stdcall RunFile(const char *fileName);
	static void __stdcall RunScriptFiles(UInt16 type);
};

extern UnorderedMap<const char*, NiCamera*> s_extraCamerasMap;

extern bool s_HUDCursorMode;

bool GetMenuMode();

bool IsConsoleOpen();

void __fastcall DoConsolePrint(double *result);

void __fastcall DoConsolePrint(TESForm *result);

struct TLSData
{
	UInt32			unk000;				// 000
	UInt32			unk004;				// 004
	ExtraDataList	*lastXtraList;		// 008
	UInt32			unk00C;				// 00C
	BSExtraData		*xDatas[kExtraData_Max];	// 010
	UInt32			unk25C;				// 25C
	NiNode			*lastNiNode;		// 260
	TESObjectREFR	*lastNiNodeREFR;	// 264
	UInt8			consoleMode;		// 268
	UInt8			pad269[3];			// 269
	UInt32			unk26C[4];			// 26C
	TESForm			*lastRefVar;		// 27C
	UInt32			lastVarIndex;		// 280
	ScriptEventList	*lastEventList;		// 284
	Script			*lastScript;		// 288
	UInt32			unk28C;				// 28C
	UInt32			unk290;				// 290
	UInt32			flags294;			// 294
	UInt32			unk298[7];			// 298
	UInt32			heapIndex;			// 2B4
	UInt32			unk2B8;				// 2B8
	UInt32			unk2BC;				// 2BC
										// 25C is used as do not head track the player , 2B8 is used to init QueudFile::unk0018
};
STATIC_ASSERT(sizeof(TLSData) == 0x2C0);

TLSData *GetTLSData();

bool __fastcall GetFileArchived(const char *filePath);

int __stdcall FileExistsEx(char *filePath, bool isFolder);

int GetIsLAA();

extern double s_nvseVersion;

#define REG_CMD(name) RegisterCommand(&kCommandInfo_##name)
#define REG_CMD_FRM(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Form)
#define REG_CMD_STR(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_String)
#define REG_CMD_ARR(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Array)
#define REG_CMD_AMB(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Ambiguous)

#define REFR_RES *(UInt32*)result
#define NUM_ARGS scriptData[*opcodeOffsetPtr]

DEFINE_COMMAND_PLUGIN(EmptyCommand, , 0, 0, NULL);

void __stdcall StoreOriginalData(UInt32 addr, UInt8 size);
