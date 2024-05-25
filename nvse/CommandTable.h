#pragma once

struct ScriptLineBuffer;
struct ScriptBuffer;

enum TokenTypes
{
	kTokenType_Number,
	kTokenType_Boolean,
	kTokenType_String,
	kTokenType_Form,
	kTokenType_Ref,
	kTokenType_Global,
	kTokenType_Array,
	kTokenType_ArrayElement,
	kTokenType_Slice,
	kTokenType_Command,
	kTokenType_Variable,
	kTokenType_NumericVar,
	kTokenType_RefVar,
	kTokenType_StringVar,
	kTokenType_ArrayVar,
	kTokenType_Ambiguous,
	kTokenType_Operator,
	kTokenType_ForEachContext,

	// numeric literals can optionally be encoded as one of the following
	// all are converted to _Number on evaluation
	kTokenType_Byte,
	kTokenType_Short,		// 2 bytes
	kTokenType_Int,			// 4 bytes

	kTokenType_Pair,
	kTokenType_AssignableString,
	// xNVSE 6.1.0
	kTokenType_Lambda,

	kTokenType_Invalid,
	kTokenType_Max = kTokenType_Invalid,

	// sigil value, returned when an empty expression is parsed
	kTokenType_Empty = kTokenType_Max + 1,
};

enum ParamType : UInt32
{
	kParamType_String =					0x00,
	kParamType_Integer =				0x01,
	kParamType_Float =					0x02,
	kParamType_ObjectID =				0x03,	// GetItemCount				TESForm *, must pass IsInventoryObjectType and TESForm::Unk_3A
	kParamType_ObjectRef =				0x04,	// Activate					TESObjectREFR *, REFR-PFLA
	kParamType_ActorValue =				0x05,	// ModActorValue			UInt32 *, immediate UInt16
	kParamType_Actor =					0x06,	// ToggleAI					TESObjectREFR *, must pass IsActor (ACHR-ACRE)
	kParamType_SpellItem =				0x07,	// AddSpell					TESForm *, must be either SpellItem or book
	kParamType_Axis =					0x08,	// Rotate					char *, immediate char, X Y Z
	kParamType_Cell =					0x09,	// GetInCell				TESObjectCELL *, must be cell
	kParamType_AnimationGroup =			0x0A,	// PlayGroup				UInt32 *, immediate UInt16
	kParamType_MagicItem =				0x0B,	// Cast						MagicItem *
	kParamType_Sound =					0x0C,	// Sound					TESForm *, kFormType_Sound
	kParamType_Topic =					0x0D,	// Say						TESForm *, kFormType_Dialog
	kParamType_Quest =					0x0E,	// ShowQuestVars			TESForm *, kFormType_Quest
	kParamType_Race =					0x0F,	// GetIsRace				TESForm *, kFormType_Race
	kParamType_Class =					0x10,	// GetIsClass				TESForm *, kFormType_Class
	kParamType_Faction =				0x11,	// Faction					TESForm *, kFormType_Faction
	kParamType_Sex =					0x12,	// GetIsSex					UInt32 *, immediate UInt16
	kParamType_Global =					0x13,	// GetGlobalValue			TESForm *, kFormType_Global
	kParamType_Furniture =				0x14,	// IsCurrentFurnitureObj	TESForm *, kFormType_Furniture or kFormType_ListForm
	kParamType_BoundObject =			0x15,	// PlaceAtMe				TESObject *, must pass TESForm::Unk_3A
	kParamType_VariableName =			0x16,	// GetQuestVariable			only works in conditionals
	kParamType_QuestStage =				0x17,	// SetStage					handled like integer
	kParamType_MapMarker =				0x18,	// ShowMap					TESObjectREFR *, see ObjectRef
	kParamType_ActorBase =				0x19,	// SetEssential				TESActorBase * (NPC / creature)
	kParamType_Container =				0x1A,	// RemoveMe					TESObjectREFR *, see ObjectRef
	kParamType_WorldSpace =				0x1B,	// CenterOnWorld			TESWorldSpace *, kFormType_WorldSpace
	kParamType_CrimeType =				0x1C,	// GetCrimeKnown			UInt32 *, immediate UInt16
	kParamType_AIPackage =				0x1D,	// GetIsCurrentPackage		TESPackage *, kFormType_Package
	kParamType_CombatStyle =			0x1E,	// SetCombatStyle			TESCombatStyle *, kFormType_CombatStyle
	kParamType_MagicEffect =			0x1F,	// HasMagicEffect			EffectSetting *
	kParamType_FormType =				0x20,	// GetIsUsedItemType		UInt8 *, immediate UInt16
	kParamType_WeatherID =				0x21,	// GetIsCurrentWeather		TESForm *, kFormType_Weather
	kParamType_NPC =					0x22,	// unused					TESNPC *, kFormType_NPC
	kParamType_Owner =					0x23,	// IsOwner					TESForm *, kFormType_NPC or kFormType_Faction
	kParamType_EffectShader =			0x24,	// PlayMagicShaderVisuals	TESForm *, kFormType_EffectShader
	kParamType_FormList	=				0x25,	// IsInList					kFormType_ListForm
	kParamType_MenuIcon =				0x26,	// unused					kFormType_MenuIcon
	kParamType_Perk =					0x27,	// Add Perk					kFormType_Perk
	kParamType_Note =					0x28,	// Add Note					kFormType_Note
	kParamType_MiscellaneousStat =		0x29,	// ModPCMiscStat			UInt32 *, immediate UInt16
	kParamType_ImageSpaceModifier =		0x2A,	//							kFormType_ImageSpaceModifier
	kParamType_ImageSpace =				0x2B,	//							kFormType_ImageSpace
	kParamType_Double =					0x2C,	// 
	kParamType_ScriptVariable =			0x2D,	// 
	kParamType_VoiceType =				0x2E,	// 
	kParamType_EncounterZone =			0x2F,	//							kFormType_EncounterZone
	kParamType_IdleForm =				0x30,	//							kFormType_TESIdleForm
	kParamType_Message =				0x31,	//							kFormType_Message
	kParamType_InvObjOrFormList =		0x32,	// AddItem					IsInventoryObjectType or kFormType_ListForm
	kParamType_Alignment =				0x33,	// GetIsAlignment			UInt32 *, immediate UInt16
	kParamType_EquipType =				0x34,	// GetIsUsedEquipType		UInt32 *, immediate UInt16
	kParamType_NonFormList =			0x35,	// GetIsUsedItem			TESForm::Unk_3A and not kFormType_ListForm
	kParamType_SoundFile =				0x36,	// PlayMusic				kFormType_SoundFile
	kParamType_CriticalStage =			0x37,	// SetCriticalStage			UInt32 *, immediate UInt16

	// added for dlc (1.1)
	kParamType_LeveledOrBaseChar =		0x38,	// AddNPCToLeveledList		NPC / LeveledCharacter
	kParamType_LeveledOrBaseCreature =	0x39,	// AddCreatureToLeveledList	Creature / LeveledCreature
	kParamType_LeveledChar =			0x3A,	// AddNPCToLeveledList		kFormType_LeveledCharacter
	kParamType_LeveledCreature =		0x3B,	// AddCreatureToLeveledList	kFormType_LeveledCreature
	kParamType_LeveledItem =			0x3C,	// AddItemToLeveledList		kFormType_LeveledItem
	kParamType_AnyForm =				0x3D,	// AddFormToFormList		any form

	// new vegas
	kParamType_Reputation =				0x3E,	//							kFormType_Reputation
	kParamType_Casino =					0x3F,	//							kFormType_Casino
	kParamType_CasinoChip =				0x40,	//							kFormType_CasinoChip
	kParamType_Challenge =				0x41,	//							kFormType_Challenge
	kParamType_CaravanMoney =			0x42,	//							kFormType_CaravanMoney
	kParamType_CaravanCard =			0x43,	//							kFormType_CaravanCard
	kParamType_CaravanDeck =			0x44,	//							kFormType_CaravanDeck
	kParamType_Region =					0x45,	//							kFormType_Region

	// custom NVSE types
	kParamType_StringVar =				0x01,
	kParamType_Array =					0x100,	// only usable with compiler override; StandardCompile() will report unrecognized param type

	kNVSEParamType_Number =				(1 << kTokenType_Number) | (1 << kTokenType_Ambiguous),
	kNVSEParamType_Boolean =			(1 << kTokenType_Boolean) | (1 << kTokenType_Ambiguous),
	kNVSEParamType_String =				(1 << kTokenType_String) | (1 << kTokenType_Ambiguous),
	kNVSEParamType_Form =				(1 << kTokenType_Form) | (1 << kTokenType_Ambiguous),
	kNVSEParamType_Array =				(1 << kTokenType_Array) | (1 << kTokenType_Ambiguous),
	kNVSEParamType_ArrayElement =		(1 << kTokenType_ArrayElement) | (1 << kTokenType_Ambiguous),
	kNVSEParamType_Slice =				1 << kTokenType_Slice,
	kNVSEParamType_Command =			1 << kTokenType_Command,
	kNVSEParamType_Variable =			1 << kTokenType_Variable,
	kNVSEParamType_NumericVar =			1 << kTokenType_NumericVar,
	kNVSEParamType_RefVar =				1 << kTokenType_RefVar,
	kNVSEParamType_StringVar =			1 << kTokenType_StringVar,
	kNVSEParamType_ArrayVar =			1 << kTokenType_ArrayVar,
	kNVSEParamType_ForEachContext =		1 << kTokenType_ForEachContext,

	kNVSEParamType_Collection =			kNVSEParamType_Array | kNVSEParamType_String,
	kNVSEParamType_ArrayVarOrElement =	kNVSEParamType_ArrayVar | kNVSEParamType_ArrayElement,
	kNVSEParamType_ArrayIndex =			kNVSEParamType_String | kNVSEParamType_Number,
	kNVSEParamType_BasicType =			kNVSEParamType_Array | kNVSEParamType_String | kNVSEParamType_Number | kNVSEParamType_Form,
	kNVSEParamType_NoTypeCheck =		0,

	kNVSEParamType_FormOrNumber =		kNVSEParamType_Form | kNVSEParamType_Number,
	kNVSEParamType_StringOrNumber =		kNVSEParamType_String | kNVSEParamType_Number,
	kNVSEParamType_Pair	=				1 << kTokenType_Pair
};

enum CommandReturnType
{
	kRetnType_Default,
	kRetnType_Form,
	kRetnType_String,
	kRetnType_Array,
	kRetnType_ArrayIndex,
	kRetnType_Ambiguous,
	kRetnType_Max
};

struct ParamInfo
{
	const char	*typeStr;
	ParamType	typeID;
	UInt32		isOptional;

	constexpr ParamInfo(ParamType _typeID, UInt32 _isOptional = 0) : typeStr(""), typeID(_typeID), isOptional(_isOptional) {}
	constexpr ParamInfo(const char *_typeStr, ParamType _typeID, UInt32 _isOptional = 0) : typeStr(_typeStr), typeID(_typeID), isOptional(_isOptional) {}
};

#define COMMAND_ARGS		ParamInfo *paramInfo, UInt8 *scriptData, TESObjectREFR *thisObj, TESObjectREFR *containingObj, Script *scriptObj, ScriptLocals *eventList, double *result, UInt32 *opcodeOffsetPtr
#define PASS_COMMAND_ARGS	paramInfo, scriptData, thisObj, containingObj, scriptObj, eventList, result, opcodeOffsetPtr
#define EXTRACT_ARGS		paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, scriptObj, eventList
#define COMMAND_ARGS_EVAL	TESObjectREFR *thisObj, void *arg1, void *arg2, double *result
#define PASS_CMD_ARGS_EVAL	thisObj, arg1, arg2, result
#define COMMAND_ARGS_EX		ParamInfo *paramInfo, UInt8 *scriptData, UInt32 *opcodeOffsetPtr, Script *scriptObj, ScriptLocals *eventList
#define EXTRACT_ARGS_EX		paramInfo, scriptData, opcodeOffsetPtr, scriptObj, eventList

typedef bool (*Cmd_Execute)(COMMAND_ARGS);
bool Cmd_Default_Execute(COMMAND_ARGS);

typedef bool (*Cmd_Parse)(UInt32 numParams, ParamInfo *paramInfo, ScriptLineBuffer *lineBuf, ScriptBuffer *scriptBuf);
bool Cmd_Default_Parse(UInt32 numParams, ParamInfo *paramInfo, ScriptLineBuffer *lineBuf, ScriptBuffer *scriptBuf);
const Cmd_Parse Cmd_Expression_Plugin_Parse = (Cmd_Parse)0x08000000;

typedef bool (*Cmd_Eval)(COMMAND_ARGS_EVAL);
bool Cmd_Default_Eval(COMMAND_ARGS_EVAL);

struct CommandInfo
{
	const char		*longName;		// 00
	const char		*shortName;		// 04
	UInt32			opcode;			// 08
	const char		*helpText;		// 0C
	UInt16			needsParent;	// 10
	UInt16			numParams;		// 12
	const ParamInfo	*params;		// 14

	// handlers
	Cmd_Execute		execute;		// 18
	Cmd_Parse		parse;			// 1C
	Cmd_Eval		eval;			// 20

	UInt32			flags;			// 24		might be more than one field (reference to 25 as a byte)
};

#define DEFINE_COMMAND_PLUGIN(name, refRequired, paramInfo) \
	extern bool Cmd_##name##_Execute(COMMAND_ARGS); \
	static CommandInfo kCommandInfo_##name = { \
	#name, \
	nullptr, \
	0, \
	nullptr, \
	refRequired, \
	sizeof(paramInfo) / sizeof(ParamInfo), \
	paramInfo, \
	Cmd_##name##_Execute, \
	nullptr, \
	nullptr, \
	0 \
	};

#define DEFINE_COMMAND_ALT_PLUGIN(name, altName, refRequired, paramInfo) \
	extern bool Cmd_##name##_Execute(COMMAND_ARGS); \
	static CommandInfo kCommandInfo_##name = { \
	#name, \
	#altName, \
	0, \
	nullptr, \
	refRequired, \
	sizeof(paramInfo) / sizeof(ParamInfo), \
	paramInfo, \
	Cmd_##name##_Execute, \
	nullptr, \
	nullptr, \
	0 \
	};

#define DEFINE_COMMAND_PLUGIN_EXP(name, refRequired, paramInfo) \
	extern bool Cmd_##name##_Execute(COMMAND_ARGS); \
	static CommandInfo kCommandInfo_##name = { \
	#name, \
	nullptr, \
	0, \
	nullptr, \
	refRequired, \
	sizeof(paramInfo) / sizeof(ParamInfo), \
	paramInfo, \
	Cmd_##name##_Execute, \
	Cmd_Expression_Plugin_Parse, \
	nullptr, \
	0 \
	};

#define DEFINE_COMMAND_ALT_PLUGIN_EXP(name, altName, refRequired, paramInfo) \
	extern bool Cmd_##name##_Execute(COMMAND_ARGS); \
	static CommandInfo kCommandInfo_##name = { \
	#name, \
	#altName, \
	0, \
	nullptr, \
	refRequired, \
	sizeof(paramInfo) / sizeof(ParamInfo), \
	paramInfo, \
	Cmd_##name##_Execute, \
	Cmd_Expression_Plugin_Parse, \
	nullptr, \
	0 \
	};

#define DEFINE_CMD_COND_PLUGIN(name, refRequired, paramInfo) \
	extern bool Cmd_##name##_Execute(COMMAND_ARGS); \
	extern bool Cmd_##name##_Eval(COMMAND_ARGS_EVAL); \
	static CommandInfo kCommandInfo_##name = { \
	#name,	\
	nullptr,	\
	0,		\
	nullptr,	\
	refRequired,	\
	sizeof(paramInfo) / sizeof(ParamInfo),	\
	paramInfo,	\
	Cmd_##name##_Execute,	\
	nullptr,	\
	Cmd_##name##_Eval,	\
	1	\
	};

#define DEFINE_CMD_ALT_COND_PLUGIN(name, altName, refRequired, paramInfo) \
	extern bool Cmd_##name##_Execute(COMMAND_ARGS); \
	extern bool Cmd_##name##_Eval(COMMAND_ARGS_EVAL); \
	static CommandInfo kCommandInfo_##name = { \
	#name,	\
	#altName,		\
	0,		\
	nullptr,	\
	refRequired,	\
	sizeof(paramInfo) / sizeof(ParamInfo),	\
	paramInfo,	\
	Cmd_##name##_Execute,	\
	nullptr,	\
	Cmd_##name##_Eval,	\
	1	\
	};

#define DEFINE_CMD_COND_ONLY(name, paramInfo) \
	extern bool Cmd_##name##_Eval(COMMAND_ARGS_EVAL); \
	static CommandInfo kCommandInfo_##name = { \
	#name,	\
	nullptr,	\
	0,		\
	nullptr,	\
	1,		\
	sizeof(paramInfo) / sizeof(ParamInfo),	\
	paramInfo,	\
	nullptr,	\
	nullptr,	\
	Cmd_##name##_Eval,	\
	1	\
	};