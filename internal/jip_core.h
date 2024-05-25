#pragma once

#define WriteRecord g_serialization.WriteRecord
#define WriteRecordData g_serialization.WriteRecordData
#define GetNextRecordInfo g_serialization.GetNextRecordInfo
#define ReadRecordData g_serialization.ReadRecordData
#define WriteRecord8 g_serialization.WriteRecord8
#define WriteRecord16 g_serialization.WriteRecord16
#define WriteRecord32 g_serialization.WriteRecord32
#define WriteRecord64 g_serialization.WriteRecord64
#define ReadRecord8 g_serialization.ReadRecord8
#define ReadRecord16 g_serialization.ReadRecord16
#define ReadRecord32 g_serialization.ReadRecord32
#define ReadRecord64 g_serialization.ReadRecord64

#define GetCmdByOpcode g_commandTbl.GetByOpcode
#define GetPluginInfoByName g_commandTbl.GetPluginInfoByName

#define GetStringVar g_stringVar.GetString
#define AssignString g_stringVar.Assign

#define CreateArray g_arrayVar.CreateArray
#define CreateMap g_arrayVar.CreateMap
#define CreateStringMap g_arrayVar.CreateStringMap
#define SetElement g_arrayVar.SetElement
#define AppendElement g_arrayVar.AppendElement
#define GetArraySize g_arrayVar.GetArraySize
#define LookupArrayByID g_arrayVar.LookupArrayByID
#define GetElement g_arrayVar.GetElement
#define GetElements g_arrayVar.GetElements
#define GetContainerType g_arrayVar.GetContainerType
#define ArrayHasKey g_arrayVar.ArrayHasKey

#define ExtractArgsEx g_script.ExtractArgsEx
#define ExtractFormatStringArgs g_script.ExtractFormatStringArgs
#define CallFunction g_script.CallFunctionAlt
#define DecompileToBuffer g_script.pDecompileToBuffer

typedef void (*_CaptureLambdaVars)(Script* scriptLambda);
extern _CaptureLambdaVars CaptureLambdaVars;
typedef void (*_UncaptureLambdaVars)(Script* scriptLambda);
extern _UncaptureLambdaVars UncaptureLambdaVars;

typedef Tile* (__stdcall *_UIOInjectComponent)(Tile *parentTile, const char *dataStr);
extern _UIOInjectComponent UIOInjectComponent;

#define MSGBOX_ARGS 0, 0, ShowMessageBox_Callback, 0, 0x17, 0, 0, "OK", nullptr

namespace GameGlobals
{
	__forceinline const char **TerminalModelPtr() {return (const char**)0x11A0BB0;}
	__forceinline SpellItem *PipBoyLight() {return *(SpellItem**)0x11C358C;}
	__forceinline TESDescription **CurrentDescription() {return (TESDescription**)0x11C5490;}
	__forceinline String *CurrentDescriptionText() {return (String*)0x11C5498;}
	__forceinline NiTPtrMap<TESForm> *AllFormsMap() {return *(NiTPtrMap<TESForm>**)0x11C54C0;}
	__forceinline NiTStringPointerMap<TESForm> *EditorIDsMap() {return *(NiTStringPointerMap<TESForm>**)0x11C54C8;}
	__forceinline BSTCaseInsensitiveStringMap<void*> *IdleAnimsDirectoryMap() {return *(BSTCaseInsensitiveStringMap<void*>**)0x11CB6A0;}
	__forceinline BSSimpleArray<TESRecipeCategory*> *RecipeMenuCategories() {return (BSSimpleArray<TESRecipeCategory*>*)0x11D8F08;}
	__forceinline TESObjectWEAP *PlayerWeapon() {return *(TESObjectWEAP**)0x11D98D4;}
	__forceinline tList<VATSTarget> *VATSTargetList() {return (tList<VATSTarget>*)0x11DB150;}
	__forceinline NiNode *ObjectLODRoot() {return *(NiNode**)0x11DEA18;}
	__forceinline bool *GamePadRumble() {return (bool*)0x11E0854;}
	__forceinline UInt32 TickCount() {return *(UInt32*)0x11F63A8;}
	__forceinline tList<Archive> *ArchivesList() {return *(tList<Archive>**)0x11F8160;}
	__forceinline LightCS *SceneLightsLock() {return (LightCS*)SCENE_LIGHTS_CS;}
	__forceinline tList<ListBox<int>> *ActiveListBoxes() {return (tList<ListBox<int>>*)0x11D8B54;}
	__forceinline tList<GradualSetFloat> *QueuedGradualSetFloat() {return (tList<GradualSetFloat>*)0x11F3348;}
	__forceinline RadioEntry *PipboyRadio() {return *(RadioEntry**)0x11DD42C;}
	__forceinline const char *GetConfigDirectory() {return (const char*)0x1202FA0;}
};

extern NiNode *s_pc1stPersonNode, *g_cursorNode;
extern float g_screenWidth, g_screenHeight;
extern const char *g_terminalModelDefault;
extern TESObjectWEAP *g_fistsWeapon;
extern TESObjectACTI *g_ashPileACTI, *g_gooPileACTI;
extern TESGlobal *g_gameYear, *g_gameMonth, *g_gameDay, *g_gameHour, *g_timeScale;
extern TESObjectMISC *g_capsItem;
extern TESImageSpaceModifier *g_getHitIMOD, *g_explosionInFaceIMOD;
extern UInt32 s_mainThreadID, s_initialTickCount;

extern const char kDaysPerMonth[], kMenuIDJumpTable[];

__forceinline TESObjectREFR *PlaceAtMe(TESObjectREFR *refr, TESForm *form, UInt32 count, UInt32 distance, UInt32 direction, float health)
{
	return CdeclCall<TESObjectREFR*>(0x5C4B30, refr, form, count, distance, direction, health);
}
__forceinline NiNode *GetCdBodyNode(hkpWorldObject *object)
{
	return CdeclCall<NiNode*>(0xC7FA90, &object->cdBody);
}
__forceinline TESObjectREFR *GetCdBodyRef(hkpWorldObject *object)
{
	return CdeclCall<TESObjectREFR*>(0x62B4E0, &object->cdBody);
}
__forceinline float ApplyAmmoEffects(AmmoEffectID effType, AmmoEffectList *effList, float baseValue)
{
	return CdeclCall<float>(0x59A030, effType, effList, baseValue);
}
__forceinline void ApplyPerkModifiers(PerkEntryPointID entryPointID, TESObjectREFR *perkOwner, float *baseValue)
{
	CdeclCall(0x5E58F0, entryPointID, perkOwner, baseValue);
}
__forceinline void ApplyPerkModifiers(PerkEntryPointID entryPointID, TESObjectREFR *perkOwner, TESForm *filter1, float *baseValue)
{
	CdeclCall(0x5E58F0, entryPointID, perkOwner, filter1, baseValue);
}
__forceinline void ApplyPerkModifiers(PerkEntryPointID entryPointID, TESObjectREFR *perkOwner, TESForm *filter1, TESForm *filter2, float *baseValue)
{
	CdeclCall(0x5E58F0, entryPointID, perkOwner, filter1, filter2, baseValue);
}
__forceinline TESTopicInfo *GetTopicInfo(TESTopic *topic, void *result, Actor *actor, Actor *target)
{
	return CdeclCall<TESTopicInfo*>(0x61A7D0, topic, result, actor, target, true, nullptr, nullptr);
}
__forceinline ImageSpaceModifierInstanceForm *ApplyIMOD(TESImageSpaceModifier *imod, float percent, NiObject *obj10)
{
	return CdeclCall<ImageSpaceModifierInstanceForm*>(0x5299A0, imod, percent, obj10);
}
__forceinline void *PurgeTerminalModel()
{
	return CdeclCall<void*>(ADDR_PurgeTerminalModel);
}

TESForm* __stdcall LookupFormByRefID(UInt32 refID);
TESForm* __fastcall LookupFormByEDID(const char *edidStr);
UInt32 __stdcall HasChangeData(UInt32 refID);
bool __fastcall GetResolvedModIndex(UInt8 *pModIdx);
UInt32 __fastcall GetResolvedRefID(UInt32 *refID);

struct PatchInstallState
{
	bool	bigGunsSkill = false;
	bool	impactDmgFix = false;
	bool	hardcoreNeedsFix = false;
	bool	failedScriptLocks = false;
	bool	dblPrecision = false;
	bool	FO3WpnDegrade = false;
	bool	localizedDTDR = false;
	bool	sneakBoundsFix = false;
	bool	NVACAlerts = false;
	bool	NPCWeaponMods = false;
	bool	NPCPerks = false;
	bool	creSpreadFix = false;
};
extern PatchInstallState s_patchInstallState;

enum
{
	kChangedFlag_AuxVars =		1,
	kChangedFlag_RefMaps =		2,
	kChangedFlag_ExtraData =	4,
	kChangedFlag_LinkedRefs =	8,
	kChangedFlag_NPCPerks =		0x10,

	kChangedFlag_All =			kChangedFlag_AuxVars | kChangedFlag_RefMaps | kChangedFlag_ExtraData | kChangedFlag_LinkedRefs | kChangedFlag_NPCPerks,

	kSerializedFlag_NoHardcoreTracking =	1 << 0,
};

struct JIPSerializedVars
{
	enum
	{
		kSzVars_Version =	1
	};

	UInt32		serializedFlags;
	float		dmgArmorMaxPercent;

	void Reset()
	{
		serializedFlags = 0;
		dmgArmorMaxPercent = 1.0F;
	}
};
extern JIPSerializedVars s_serializedVars;

struct QueuedCmdCall
{
	UInt32			opcode;		// 00
	void			*cmdAddr;	// 04
	UInt32			thisObj;	// 08	refID
	UInt32			numArgs;	// 0C
	FunctionArg		args[4];	// 10

	QueuedCmdCall(void *_cmdAddr, UInt32 _thisObj) : opcode(0x2B), cmdAddr(_cmdAddr), thisObj(_thisObj), numArgs(0) {}

	void PushArg(FunctionArg arg)
	{
		if (numArgs < 4)
			args[numArgs++] = arg;
	}

	bool QueueCall() {return ThisCall<bool>(0x9054A0, g_scrapHeapQueue->taskQueue, this);}
};

class LambdaVarContext
{
	Script		*scriptLambda;

	void Set(Script *script)
	{
		scriptLambda = script;
		CaptureLambdaVars(script);
	}

public:
	LambdaVarContext() : scriptLambda(nullptr) {}
	LambdaVarContext(Script *script) {Set(script);}
	LambdaVarContext(const LambdaVarContext &other) {Set(other.scriptLambda);}

	~LambdaVarContext() {UncaptureLambdaVars(scriptLambda);}

	inline LambdaVarContext& operator=(const LambdaVarContext &other) {Set(other.scriptLambda); return *this;}

	inline Script* operator()() const {return scriptLambda;}

	inline operator Script*() const {return scriptLambda;}

	inline bool operator==(const LambdaVarContext &rhs) const {return scriptLambda == rhs.scriptLambda;}
	inline bool operator!=(const LambdaVarContext &rhs) const {return scriptLambda != rhs.scriptLambda;}
	inline bool operator<(const LambdaVarContext &rhs) const {return scriptLambda < rhs.scriptLambda;}
};

struct ResultVars
{
	ScriptVar	*x;
	ScriptVar	*y;
	ScriptVar	*z;

	ResultVars() {}

	void __vectorcall Set(__m128 values);
	void __vectorcall Set(__m128 values, const __m128 modifier);
	inline void Set(double _x, double _y, double _z)
	{
		x->data = _x;
		y->data = _y;
		z->data = _z;
	}
};

typedef Set<TESForm*, 0x40> TempFormList;
TempFormList *GetTempFormList();

typedef Vector<ArrayElementL, 0x100> TempElements;
TempElements *GetTempElements();

UInt32 __fastcall StringToRef(char *refStr);

struct InventoryItemData
{
	SInt32				count;
	ContChangesEntry	*entry;

	InventoryItemData(SInt32 _count, ContChangesEntry *_entry) : count(_count), entry(_entry) {}
};

typedef Map<TESForm*, InventoryItemData, 0x40> InventoryItemsMap;
InventoryItemsMap *GetInventoryItemsMap();

bool GetInventoryItems(TESObjectREFR *refr, UInt8 typeID, InventoryItemsMap *invItemsMap);

void __fastcall ShowItemMessage(TESForm *item, const char *msgStr);

NiAVObject* __stdcall GetRayCastObject(const NiVector3 &posVector, float *rotMatRow, float maxRange, SInt32 layerType);

int __stdcall GetRayCastMaterial(const NiVector3 &posVector, float *rotMatRow, float maxRange, SInt32 layerType);

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
		auto partIter = npc->headPart.Head();
		do
		{
			if (partIter->data)
				numParts++;
		}
		while (partIter = partIter->next);
		if (numParts)
		{
			BGSHeadPart **ptr = (BGSHeadPart**)malloc(numParts * 4);
			headParts = ptr;
			partIter = npc->headPart.Head();
			do
			{
				if (*ptr = partIter->data)
					ptr++;
			}
			while (partIter = partIter->next);
		}
		else headParts = nullptr;
	}

	void Undo(TESNPC *npc)
	{
		npc->copyFrom = nullptr;
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
		for (UInt32 idx = 0; idx < numParts; idx++)
			npc->headPart.Prepend(headParts[idx]);
	}

	void Destroy()
	{
		if (headParts) free(headParts);
		free(this);
	}
};

extern TempObject<UnorderedMap<TESNPC*, AppearanceUndo*>> s_appearanceUndoMap;

hkpWorld *GethkpWorld();

bool LeveledListHasFormDeep(TESLeveledList *pLvlList, TESForm *form, TempFormList *tmpFormLst);

float GetDaysPassed(int bgnYear = 2281, int bgnMonth = 9, int bgnDay = 13);

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
extern TempObject<UnorderedMap<UInt32, ScriptVariablesMap>> s_scriptVariablesBuffer;
typedef UnorderedSet<const char*> VariableNames;

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
extern TempObject<UnorderedMap<UInt32, LinkedRefEntry>> s_linkedRefModified;
extern TempObject<UnorderedMap<UInt32, UInt32>> s_linkedRefDefault, s_linkedRefsTemp;

bool SetLinkedRefID(UInt32 thisID, UInt32 linkID = 0, UInt8 modIdx = 0xFF);

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
			Pool_CFree(str, alloc);
			alloc = 0;
		}
	}

public:
	AuxVariableValue() : alloc(0) {}
	__forceinline AuxVariableValue(UInt8 _type) : type(_type), alloc(0) {}
	explicit AuxVariableValue(const NVSEArrayElement &elem) : alloc(0) {*this = elem;}

	~AuxVariableValue() {Clear();}

	UInt8 GetType() const {return type;}
	double GetFlt() const {return (type == 1) ? num : 0;}
	UInt32 GetRef() const {return (type == 2) ? refID : 0;}
	const char *GetStr() const {return alloc ? str : nullptr;}

	inline void operator=(double value)
	{
		Clear();
		type = 1;
		num = value;
	}

	inline void operator=(TESForm *value)
	{
		Clear();
		type = 2;
		refID = value ? value->refID : 0;
	}

	__declspec(noinline) void operator=(const char *value)
	{
		type = 4;
		if (length = StrLen(value))
		{
			if (length > 0x1FFF)
				length = 0x1FFF;
			if (alloc <= length)
			{
				if (alloc) Pool_CFree(str, alloc);
				alloc = (length + 0x11) & 0xFFF0;
				str = Pool_CAlloc(alloc);
			}
			COPY_BYTES(str, value, length);
			str[length] = 0;
		}
		else if (alloc)
			*str = 0;
	}

	inline void operator=(const NVSEArrayElement &value)
	{
		if (value.GetType() == 2)
			*this = value.form;
		else if (value.GetType() == 3)
			*this = value.str;
		else *this = value.num;
	}

	inline bool operator==(const AuxVariableValue &rhs) const
	{
		if (type != rhs.type)
			return false;
		if (type == 1)
			return num == rhs.num;
		if (type == 2)
			return refID == rhs.refID;
		if (!alloc || !length)
			return !rhs.alloc || !rhs.length;
		return rhs.alloc && (length == rhs.length) && !StrCompareCS(str, rhs.str);
	}

	inline bool operator==(const NVSEArrayElement &rhs) const
	{
		switch (rhs.GetType())
		{
			case 1:
				return (type == 1) && (num == rhs.num);
			case 2:
				return (type == 2) && rhs.form && (refID == rhs.form->refID);
			case 3:
				return (type == 4) && alloc && rhs.str && !StrCompareCI(str, rhs.str);
			default:
				return false;
		}
	}

	ArrayElementL GetAsElement() const
	{
		if (type == 2) return ArrayElementL(LookupFormByRefID(refID));
		if (type == 4) return ArrayElementL(GetStr());
		return ArrayElementL(num);
	}

	__declspec(noinline) UInt8 *ReadValData(UInt8 *bufPos)
	{
		if (type == 1)
		{
			num = *(double*)bufPos;
			return bufPos + 8;
		}
		if (type == 2)
		{
			refID = GetResolvedRefID((UInt32*)bufPos);
			return bufPos + 4;
		}
		length = *(UInt16*)bufPos;
		bufPos += 2;
		if (length)
		{
			alloc = (length + 0x11) & 0xFFF0;
			str = Pool_CAlloc(alloc);
			COPY_BYTES(str, bufPos, length);
			str[length] = 0;
			bufPos += length;
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
static_assert(sizeof(AuxVariableValue) == 0x10);

typedef Vector<AuxVariableValue, 2> AuxVarValsArr;
typedef UnorderedMap<char*, AuxVarValsArr, 4> AuxVarVarsMap;
typedef UnorderedMap<UInt32, AuxVarVarsMap> AuxVarOwnersMap;
typedef UnorderedMap<UInt32, AuxVarOwnersMap> AuxVarModsMap;
extern TempObject<AuxVarModsMap> s_auxVariables[2];

typedef UnorderedMap<UInt32, AuxVariableValue, 4> RefMapIDsMap;
typedef UnorderedMap<char*, RefMapIDsMap, 4> RefMapVarsMap;
typedef UnorderedMap<UInt32, RefMapVarsMap> RefMapModsMap;
extern TempObject<RefMapModsMap> s_refMapArrays[2];

extern PrimitiveCS s_auxVarCS, s_refMapCS;

UInt32 __fastcall GetSubjectID(TESForm *form, TESObjectREFR *thisObj);

struct AuxVarInfo
{
	UInt32		ownerID;
	UInt32		modIndex;
	char		*varName;
	bool		isTemp;

	AuxVarInfo(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj, char *pVarName)
	{
		if (*pVarName)
		{
			if (ownerID = GetSubjectID(form, thisObj))
			{
				varName = pVarName;
				isTemp = (*pVarName == '*');
				modIndex = (pVarName[isTemp] == '_') ? 0xFF : scriptObj->GetOverridingModIdx();
			}
		}
		else ownerID = 0;
	}

	AuxVarInfo(TESForm *form, TESObjectREFR *thisObj, Script *scriptObj, UInt32 type)
	{
		if (ownerID = GetSubjectID(form, thisObj))
		{
			isTemp = type & 1;
			modIndex = (type > 1) ? 0xFF : scriptObj->GetOverridingModIdx();
		}
	}

	inline AuxVarModsMap& operator()() const {return s_auxVariables[isTemp];}

	AuxVarValsArr* __fastcall GetArray(bool addArr = false);
	AuxVariableValue* __fastcall GetValue(SInt32 idx, bool addVal = false);

	explicit operator bool() const {return ownerID != 0;}
};

struct RefMapInfo
{
	UInt32		modIndex;
	bool		isTemp;

	RefMapInfo(Script *scriptObj, char *varName)
	{
		isTemp = (*varName == '*');
		modIndex = (varName[isTemp] == '_') ? 0xFF : scriptObj->GetOverridingModIdx();
	}

	RefMapInfo(Script *scriptObj, UInt32 type)
	{
		isTemp = type & 1;
		modIndex = (type > 1) ? 0xFF : scriptObj->GetOverridingModIdx();
	}

	inline RefMapModsMap& operator()() const {return s_refMapArrays[isTemp];}
};

extern UInt8 s_dataChangedFlags;

struct EventCallbackScripts : Set<LambdaVarContext, 4>
{
	void InvokeEvents(UInt32 arg);
	void InvokeEvents2(UInt32 arg1, UInt32 arg2);
	void InvokeEventsThis(TESObjectREFR *thisObj);
	void InvokeEventsThis1(TESObjectREFR *thisObj, UInt32 arg);
	void InvokeEventsThis2(TESObjectREFR *thisObj, UInt32 arg1, UInt32 arg2);
};

typedef TempObject<Map<char*, EventCallbackScripts, 2>> MenuClickFiltersMap;
typedef TempObject<UnorderedMap<int, EventCallbackScripts>> MenuClickIDsMap;

struct QuestStageEventCallback
{
	UInt8				stageID;
	UInt8				flags;
	UInt8				pad02[2];
	LambdaVarContext	callback;

	QuestStageEventCallback() {}
	QuestStageEventCallback(UInt8 _stageID, UInt8 _flags, Script *_callback) : stageID(_stageID), flags(_flags), callback(_callback) {}
};
typedef Vector<QuestStageEventCallback, 2> QuestStageCallbacks;

class QuestStageEventFinder
{
	QuestStageEventCallback	*m_callback;

public:
	QuestStageEventFinder(QuestStageEventCallback &_callback) : m_callback(&_callback) {}

	bool operator==(const QuestStageEventCallback &rhs) const
	{
		return (m_callback->stageID == rhs.stageID) && (m_callback->callback == rhs.callback);
	}
};

typedef UnorderedMap<UInt32, EventCallbackScripts> AnimEventCallbacks;

struct CriticalHitEventData
{
	Actor				*target;	// 00
	TESObjectREFR		*source;	// 04
	TESObjectWEAP		*weapon;	// 08
	LambdaVarContext	callback;	// 0C

	CriticalHitEventData() {}
	CriticalHitEventData(Actor *_target, TESObjectREFR *_source, TESObjectWEAP *_weapon, Script *_callback) :
		target(_target), source(_source), weapon(_weapon), callback(_callback) {}
};

typedef Vector<CriticalHitEventData> CriticalHitEventCallbacks;
extern TempObject<CriticalHitEventCallbacks> s_criticalHitEvents;

class CriticalHitEventFind
{
	CriticalHitEventData	*data;

public:
	CriticalHitEventFind(CriticalHitEventData &_data) : data(&_data) {}

	bool operator==(const CriticalHitEventData &rhs) const
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

	bool operator==(const CriticalHitEventData &rhs) const
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
extern TempObject<UnorderedMap<Script*, DisabledScriptBlocks>> s_disabledScriptBlocksMap;

class DisabledBlockFinder
{
	UInt8		blockType;

public:
	DisabledBlockFinder(UInt8 _blockType) : blockType(_blockType) {}

	bool operator==(const DisabledBlockInfo &rhs) const {return rhs.blockType == blockType;}
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
			typePtr = nullptr;
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
			typePtr = nullptr;
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
		if (size = GetArraySize(srcArr))
		{
			elements = (ArrayElementR*)calloc(size, sizeof(ArrayElementR));
			GetElements(srcArr, elements, nullptr);
		}
		else elements = nullptr;
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

	ArrayElementR& operator[](UInt32 index) const {return elements[index];}
};

struct ArrayData
{
	UInt32						size;
	AuxBuffer<ArrayElementR>	vals;
	ArrayElementR				*keys;

	ArrayData(NVSEArrayVar *srcArr, bool isPacked)
	{
		if (size = GetArraySize(srcArr))
		{
			vals = isPacked ? size : (size << 1);
			keys = isPacked ? nullptr : (vals + size);
			if (!GetElements(srcArr, vals, keys))
				size = 0;
		}
	}

	~ArrayData()
	{
		if (size)
		{
			UInt32 count = keys ? (size * 2) : size;
			ArrayElementR *elems = vals;
			do
			{
				elems->~ElementR();
				elems++;
			}
			while (--count);
		}
	}
};

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

	SInt32 GetCount() const {return entry->countDelta;}
	ExtraDataList *CreateExtraData();
	ExtraDataList* __fastcall SplitFromStack(SInt32 maxStack = 1);
};

typedef TESObjectREFR* (__stdcall *_InventoryRefCreate)(TESObjectREFR *container, TESForm *itemForm, SInt32 countDelta, ExtraDataList *xData);
extern _InventoryRefCreate InventoryRefCreate;
typedef InventoryRef* (*_InventoryRefGetForID)(UInt32 refID);
extern _InventoryRefGetForID InventoryRefGetForID;

TESObjectREFR* __fastcall CreateRefForStack(TESObjectREFR *container, ContChangesEntry *menuEntry);

TESObjectREFR* __fastcall GetEquippedItemRef(Actor *actor, UInt32 slotIndex);

ContChangesEntry* __fastcall GetHotkeyItemEntry(UInt8 index, ExtraDataList **outXData);

bool __fastcall ClearHotkey(UInt8 index);

float __vectorcall GetLightAmountAtPoint(const NiVector3 &pos);

struct AnimGroupClassify
{
	UInt8	category;	// 00
	UInt8	subType;	// 01
	UInt8	flags;		// 02
	UInt8	byte03;		// 03
};
extern const AnimGroupClassify kAnimGroupClassify[];

namespace JIPScriptRunner
{
	enum ScriptRunOn : UInt16
	{
		kRunOn_Invalid =		0,
		kRunOn_LoadGame =		'lg',
		kRunOn_ExitToMainMenu =	'mx',
		kRunOn_NewGame =		'ng',
		kRunOn_LoadOrNewGame =	'nl',
		kRunOn_RestartGame =	'rg',
		kRunOn_SaveGame =		'sg',
		kRunOn_ExitGame =		'xg'
	};

	struct CachedSRScript
	{
		LambdaVarContext	script;
		ScriptRunOn			runOn;

		CachedSRScript(Script *_script, ScriptRunOn _runOn = kRunOn_Invalid) : script(_script), runOn(_runOn) {}
	};

	extern UInt8 initInProgress;

	void Init();
	bool RegisterScript(char *relPath);
	void RunScripts(ScriptRunOn runOn1, ScriptRunOn runOn2 = kRunOn_Invalid);

	void __fastcall RunScript(Script *script, int, TESObjectREFR *callingRef);
	bool __fastcall RunScriptSource(char *scrSource, const char *scrName, bool capture = false);

	void __fastcall LogCompileError(String &errorStr);
};

extern TempObject<UnorderedMap<const char*, JIPScriptRunner::CachedSRScript>> s_cachedScripts;

extern TempObject<UnorderedMap<const char*, NiCamera*>> s_extraCamerasMap;

extern bool s_HUDCursorMode;

bool GetMenuMode();

extern bool s_mapMenuSkipSetXY;

void RefreshItemListBox();

bool IsConsoleOpen();

void __fastcall DoConsolePrint(double *result);

void __fastcall DoConsolePrint(TESForm *result);

bool IsInMainThread();

struct TLSData
{
	UInt32			unk000;				// 000
	UInt32			unk004;				// 004
	ExtraDataList	*lastXtraList;		// 008
	UInt32			unk00C;				// 00C
	BSExtraData		*xDatas[kXData_ExtraMax];	// 010
	UInt8			byte25C;			// 25C
	UInt8			pad25D[3];			// 25D
	NiNode			*lastNiNode;		// 260
	TESObjectREFR	*lastNiNodeREFR;	// 264
	UInt8			consoleMode;		// 268
	UInt8			pad269[3];			// 269
	UInt32			unk26C[4];			// 26C
	TESForm			*lastRefVar;		// 27C
	UInt32			lastVarIndex;		// 280
	ScriptLocals	*lastEventList;		// 284
	Script			*lastScript;		// 288
	UInt32			activateRecursionDepth;	// 28C
	UInt32			unk290;				// 290
	UInt32			flags294;			// 294
	UInt32			unk298[7];			// 298
	UInt32			heapIndex;			// 2B4
	UInt32			unk2B8;				// 2B8
	UInt32			unk2BC;				// 2BC
										// 25C is used as do not head track the player , 2B8 is used to init QueudFile::unk0018
};
static_assert(sizeof(TLSData) == 0x2C0);

TLSData *GetTLSData();

extern TempObject<UnorderedMap<const char*, UInt32, 0x20, false>> s_fileExtToType;

bool __fastcall GetFileArchived(const char *filePath);

int __stdcall FileExistsEx(char *filePath, bool isFolder);

int GetIsLAA();

extern double s_nvseVersion;

void InitMemUsageDisplay(UInt32 callDelay);

#define REG_CMD(name) RegisterCommand(&kCommandInfo_##name)
#define REG_CMD_FRM(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Form)
#define REG_CMD_STR(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_String)
#define REG_CMD_ARR(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Array)
#define REG_CMD_AMB(name) RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Ambiguous)
extern CommandInfo kEmptyCommand;
#define REG_EMPTY RegisterCommand(&kEmptyCommand)

#define OPCODE_ID (*(UInt16*)(scriptData + *opcodeOffsetPtr - 4))
#define NUM_ARGS scriptData[*opcodeOffsetPtr]
#define NUM_ARGS_EX (scriptData[*opcodeOffsetPtr - 2] ? scriptData[*opcodeOffsetPtr] : 0)
#define CAPTURE_CL(varName) UInt8 varName; __asm mov varName, cl
#define CAPTURE_ECX(varName) UInt32 varName; __asm mov varName, ecx