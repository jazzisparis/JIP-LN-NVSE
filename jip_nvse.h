#pragma once

#include "internal/decoding.h"
#include "internal/dinput.h"
#include "internal/xinput.h"

DebugLog s_log, s_debug, s_missingTextures;

bool (*WriteRecord)(UInt32 type, UInt32 version, const void *buffer, UInt32 length);
bool (*WriteRecordData)(const void *buffer, UInt32 length);
bool (*GetNextRecordInfo)(UInt32 *type, UInt32 *version, UInt32 *length);
UInt32 (*ReadRecordData)(void *buffer, UInt32 length);
bool (*ResolveRefID)(UInt32 refID, UInt32 *outRefID);
const char* (*GetSavePath)(void);
void (*WriteRecord8)(UInt8 inData);
void (*WriteRecord16)(UInt16 inData);
void (*WriteRecord32)(UInt32 inData);
void (*WriteRecord64)(const void *inData);
UInt8 (*ReadRecord8)();
UInt16 (*ReadRecord16)();
UInt32 (*ReadRecord32)();
void (*ReadRecord64)(void *outData);
void (*SkipNBytes)(UInt32 byteNum);
CommandInfo* (*GetCmdByOpcode)(UInt32 opcode);
const char* (*GetStringVar)(UInt32 stringID);
bool (*AssignString)(COMMAND_ARGS, const char *newValue);
NVSEArrayVar* (*CreateArray)(const NVSEArrayElement *data, UInt32 size, Script *callingScript);
NVSEArrayVar* (*CreateStringMap)(const char **keys, const NVSEArrayElement *values, UInt32 size, Script *callingScript);
bool (*AssignCommandResult)(NVSEArrayVar *arr, double *dest);
void (*SetElement)(NVSEArrayVar *arr, const NVSEArrayElement &key, const NVSEArrayElement &value);
void (*AppendElement)(NVSEArrayVar *arr, const NVSEArrayElement &value);
UInt32 (*GetArraySize)(NVSEArrayVar *arr);
NVSEArrayVar* (*LookupArrayByID)(UInt32 id);
bool (*GetElement)(NVSEArrayVar *arr, const NVSEArrayElement &key, NVSEArrayElement &outElement);
bool (*GetElements)(NVSEArrayVar *arr, NVSEArrayElement *elements, NVSEArrayElement *keys);
bool (*ExtractArgsEx)(COMMAND_ARGS_EX, ...);
bool (*ExtractFormatStringArgs)(UInt32 fmtStringPos, char *buffer, COMMAND_ARGS_EX, UInt32 maxParams, ...);
bool (*CallFunction)(Script *funcScript, TESObjectREFR *callingObj, TESObjectREFR *container, NVSEArrayElement *result, UInt8 numArgs, ...);

DIHookControl *g_DIHookCtrl = NULL;
void *g_NVSEArrayMap = NULL, *g_NVSEStringMap = NULL;
UInt8 *g_numPreloadMods = NULL;
void (*DelArrayVar)(void *varMap, UInt32 varID);
void (*DelStringVar)(void *varMap, UInt32 varID);

#define MSGBOX_ARGS 0, 0, ShowMessageBox_Callback, 0, 0x17, 0, 0, "OK", NULL

EffectArchTypeEntry *g_effectArchTypeArray = (EffectArchTypeEntry*)0x1183320;				// Array size = 0x25
CommandInfo *g_eventCmdInfos = (CommandInfo*)0x118E2F0;										// 0x26
CommandInfo *g_consoleCmdInfos = (CommandInfo*)0x118E8E0;									// 0xCD
CommandInfo *g_scriptCmdInfos = (CommandInfo*)0x1190910;									// 0x280
EntryPointConditionInfo *g_entryPointConditionInfo = (EntryPointConditionInfo*)0x1196EE0;	// 0x49
AnimGroupInfo *g_animGroupInfoArray = (AnimGroupInfo*)0x11977D8;							// 0xF5
PCMiscStat **g_miscStatData = (PCMiscStat**)0x11C6D50;										// 0x2B
TypeSignature *g_typeSignatures = (TypeSignature*)0x101C2AC;								// 0x79; order is reversed.
PlayingMusic *g_playingMusic = (PlayingMusic*)0x11DD0F0;

const char **g_terminalModelPtr = (const char**)0x11A0BB0;
TESDescription **g_currentDescription = (TESDescription**)0x11C5490;
String *g_currentDescriptionText = (String*)0x11C5498;
NiTPointerMap<TESForm> **g_allFormsMap = (NiTPointerMap<TESForm>**)0x11C54C0;
NiTStringPointerMap<TESForm> **g_formEditorIDs = (NiTStringPointerMap<TESForm>**)0x11C54C8;
LoadedReferenceMap **g_loadedRefrMaps = (LoadedReferenceMap**)0x11C95C8;
bhkWorldM **g_bhkWorldM = (bhkWorldM**)0x11CA0D8;
BSTCaseInsensitiveStringMap<void*> **g_idleAnimsDirectoryMap = (BSTCaseInsensitiveStringMap<void*>**)0x11CB6A0;
Sky **g_tempSky = (Sky**)0x11CCB78;
ActorValueInfo **g_actorValueInfoArray = (ActorValueInfo**)0x11D61C8;
TESRecipe **g_recipeMenuSelection = (TESRecipe**)0x11D8E94;
BSSimpleArray<TESRecipeCategory> *g_recipeMenuCategories = (BSSimpleArray<TESRecipeCategory>*)0x11D8F08;
TESObjectWEAP **g_playerWeapon = (TESObjectWEAP**)0x11D98D4;
tList<VATSTargetInfo> *g_VATSTargetList = (tList<VATSTargetInfo>*)0x11DB150;
RadioEntry **g_pipboyRadio = (RadioEntry**)0x11DD42C;
GameTimeGlobals *g_gameTimeGlobals = (GameTimeGlobals*)0x11DE7B8;
NiNode **g_objectLODRoot = (NiNode**)0x11DEA18;
Sky **g_currentSky = (Sky**)0x11DEA20;
bool *g_gamePadRumble = (bool*)0x11E0854;
ProcessManager *g_processManager = (ProcessManager*)0x11E0E80;
TESObjectREFR **g_VATSTargetRef = (TESObjectREFR**)0x11F21CC;
VATSCameraData *g_VATSCameraData = (VATSCameraData*)0x11F2250;
bool *g_menuVisibility = (bool*)0x11F308F;
UInt32 *g_tickCount = (UInt32*)0x11F63A8;
BSWin32Audio **g_BSWin32Audio = (BSWin32Audio**)0x11F6D98;
BSAudioManager *g_audioManager = (BSAudioManager*)0x11F6EF0;
tList<Archive> **g_archivesList = (tList<Archive>**)0x11F8160;
NiCamera *g_masterCamera = (NiCamera*)0x11E0C20;
NiNode **g_1stPersonCameraNode = (NiNode**)0x11E07D0;
NiNode **g_3rdPersonCameraNode = (NiNode**)0x11E07D4;
NiNode **g_1stPersonCameraBipedNode = (NiNode**)0x11E07D8;
tList<GradualSetFloat> *g_queuedGradualSetFloat = (tList<GradualSetFloat>*)0x11F3348;

RecipeMenu **g_recipeMenu = (RecipeMenu**)0x11D8E90;
BarterMenu **g_barterMenu = (BarterMenu**)0x11D8FA4;
CaravanMenu **g_caravanMenu = (CaravanMenu**)0x11D917C;
CompanionWheelMenu **g_companionWheelMenu = (CompanionWheelMenu**)0x11D92B8;
ComputersMenu **g_computersMenu = (ComputersMenu**)0x11D9334;
ContainerMenu **g_containerMenu = (ContainerMenu**)0x11D93F8;
DialogMenu **g_dialogMenu = (DialogMenu**)0x11D9510;
HackingMenu **g_hackingMenu = (HackingMenu**)0x11D95B8;
InventoryMenu **g_inventoryMenu = (InventoryMenu**)0x11D9EA4;
ItemModMenu **g_itemModMenu = (ItemModMenu**)0x11D9F54;
LockPickMenu **g_lockPickMenu = (LockPickMenu**)0x11DA204;
MapMenu **g_mapMenu = (MapMenu**)0x11DA368;
MessageMenu **g_messageMenu = (MessageMenu**)0x11DA4F0;
QuantityMenu **g_quantityMenu = (QuantityMenu**)0x11DA618;
RaceSexMenu **g_raceSexMenu = (RaceSexMenu**)0x11DA634;
RepairMenu **g_repairMenu = (RepairMenu**)0x11DA75C;
RepairServicesMenu **g_repairServicesMenu = (RepairServicesMenu**)0x11DA7F0;
SleepWaitMenu **g_sleepWaitMenu = (SleepWaitMenu**)0x11DA920;
StartMenu **g_startMenu = (StartMenu**)0x11DAAC0;
StatsMenu **g_statsMenu = (StatsMenu**)0x11DACE0;
TextEditMenu **g_textEditMenu = (TextEditMenu**)0x11DAEC4;
VATSMenu **g_VATSMenu = (VATSMenu**)0x11DB0D4;
TraitMenu **g_traitMenu = (TraitMenu**)0x11DAF74;
TraitSelectMenu **g_traitSelectMenu = (TraitSelectMenu**)0x11DB00C;

ContChangesEntry **g_barterMenuSelection = (ContChangesEntry**)0x11D8FA8;
ContChangesEntry **g_containerMenuSelection = (ContChangesEntry**)0x11D93FC;
ContChangesEntry **g_inventoryMenuSelection = (ContChangesEntry**)0x11D9EA8;
ContChangesEntry **g_modMenuTarget = (ContChangesEntry**)0x11D9F58;
ContChangesEntry **g_repairMenuTarget = (ContChangesEntry**)0x11DA760;

SpellItem *g_pipBoyLight = NULL;
ModelLoader *g_modelLoader = NULL;
DataHandler *g_dataHandler = NULL;
InterfaceManager *g_interfaceManager = NULL;
OSGlobals *g_OSGlobals = NULL;
TES *g_TES = NULL;
PlayerCharacter *g_thePlayer = NULL;
SceneGraph *g_sceneGraph = NULL;
void *g_scrapHeapQueue = NULL;
FontManager *g_fontManager = NULL;
OSInputGlobals *g_inputGlobals = NULL;
TileMenu **g_tileMenuArray = NULL;
HUDMainMenu *g_HUDMainMenu = NULL;
ConsoleManager *g_consoleManager = NULL;
SystemColorManager *g_sysColorManager = NULL;
UInt32 g_screenWidth = 0;
UInt32 g_screenHeight = 0;
const char *g_terminalModelDefault = NULL;
TESObjectWEAP *g_fistsWeapon = NULL;
TESObjectACTI *g_ashPileACTI = NULL;
TESObjectACTI *g_gooPileACTI = NULL;
TESObjectMISC *g_capsItem = NULL;
TESImageSpaceModifier *g_getHitIMOD = NULL;
TESImageSpaceModifier *g_explosionInFaceIMOD = NULL;
TESObjectREFR *s_tempPosMarker = NULL;
double *g_condDmgPenalty = NULL;
double s_condDmgPenalty = 0.67;

enum
{
	kAddr_AddExtraData =			0x40FF60,
	kAddr_RemoveExtraType =			0x410140,
	kAddr_GetExtraData =			0x410220,
	kAddr_LoadModel =				0x447080,
	kAddr_GetItemHealthPerc =		0x4BCDB0,
	kAddr_ApplyAmmoEffects =		0x59A030,
	kAddr_MoveToMarker =			0x5CCB20,
	kAddr_ApplyPerkModifiers =		0x5E58F0,
	kAddr_PurgeTerminalModel =		0x7FFE00,
	kAddr_EquipItem =				0x88C650,
	kAddr_UnequipItem =				0x88C790,
	kAddr_TileGetFloat =			0xA011B0,
	kAddr_TileSetFloat =			0xA012D0,
	kAddr_TileSetString =			0xA01350,
	kAddr_InitFontInfo =			0xA12020,
};

bool (*IsPipBoyActive)(UInt32 menuID) = (bool (*)(UInt32))0x702450;
bool (*IsMenuActive)(UInt32 menuID, UInt32 arg2) = (bool (*)(UInt32, UInt32))0x702680;
void (*MoveToMarker)(TESObjectREFR *refr, TESObjectREFR *target, float posX, float posY, float posZ) = (void (*)(TESObjectREFR*, TESObjectREFR*, float, float, float))0x5CCB20;
TESObjectREFR* (*PlaceAtMe)(TESObjectREFR *refr, TESForm *form, UInt32 count, UInt32 distance, UInt32 direction, float health) = (TESObjectREFR* (*)(TESObjectREFR*, TESForm*, UInt32, UInt32, UInt32, float))0x5C4B30;
NiNode* (*GetCdBodyNode)(hkCdBody *cdBody) = (NiNode* (*)(hkCdBody*))0xC7FA90;
TESObjectREFR* (*GetCdBodyRef)(hkCdBody *cdBody) = (TESObjectREFR* (*)(hkCdBody*))0x62B4E0;
void (*RefreshItemListBox)(void) = (void (*)(void))0x704AF0;
void (*RefreshContainerMenu)(void) = (void (*)(void))0x704BC0;
void (__thiscall *DoRefreshContainerMenu)(ContainerMenu *menu, TESForm *itemForm) = (void (__thiscall *)(ContainerMenu*, TESForm*))0x75C280;
void (*ApplyPerkModifiers)(UInt32 entryPointID, TESObjectREFR *perkOwner, void *arg3, ...) = (void (*)(UInt32, TESObjectREFR*, void*, ...))0x5E58F0;
float (*ApplyAmmoEffects)(UInt32 effType, tList<TESAmmoEffect> *effList, float baseValue) = (float (*)(UInt32, tList<TESAmmoEffect>*, float))0x59A030;
float (*GetWeaponDamage)(TESObjectWEAP *weapon, float condition, ContChangesEntry *entry, TESForm *ammo) = (float (*)(TESObjectWEAP*, float, ContChangesEntry*, TESForm*))0x6450F0;
float (*GetWeaponDPS)(ActorValueOwner *avOwner, TESObjectWEAP *weapon, float condition, UInt8 arg4, ContChangesEntry *entry, UInt8 arg6, UInt8 arg7, int arg8, float arg9, float arg10, UInt8 arg11, UInt8 arg12, TESForm *ammo) =
	(float (*)(ActorValueOwner*, TESObjectWEAP*, float, UInt8, ContChangesEntry*, UInt8, UInt8, int, float, float, UInt8, UInt8, TESForm*))0x645380;
TESTopicInfo* (*GetTopicInfo)(TESTopic *topic, bool *result, Actor *actor, Actor *target, bool arg5, UInt32 *arg6, UInt32 *arg7) = (TESTopicInfo* (*)(TESTopic*, bool*, Actor*, Actor*, bool, UInt32*, UInt32*))0x61A7D0;
void (*IncPCMiscStat)(UInt32 statID) = (void (*)(UInt32))0x4D5C60;
bool (*MergeScriptEvent)(TESForm *actionRef, ExtraDataList *xData, UInt32 bitMask) = (bool (*)(TESForm*, ExtraDataList*, UInt32))0x5AC750;
void* (*PurgeTerminalModel)(void) = (void* (*)(void))0x7FFE00;
TileMenu* (*ShowQuantityMenu)(int maxCount, void (*callback)(int), int defaultCount) = (TileMenu* (*)(int, void (*)(int), int))0x7ABA00;
BSXFlags* (*GetBSXFlags)(NiNode *niNode) = (BSXFlags* (*)(NiNode*))0xC43490;
void (*DoUpdateNode)(NiAVObject *object, bool doChildren) = (void (*)(NiAVObject*, bool))0xC6BD00;
void* (*NiAllocator)(UInt32 size) = (void* (*)(UInt32))0xAA13E0;
void* (*NiDeallocator)(void *blockPtr, UInt32 size) = (void* (*)(void*, UInt32))0xAA1460;
void* (*AllocateAligned)(UInt8 align, UInt32 size) = (void* (*)(UInt8, UInt32))0x553320;
UInt32 (*GetModelLoadType)(TESForm *form) = (UInt32 (*)(TESForm*))0x45C6B0;
Projectile* (*CreateProjectile)(BGSProjectile *baseProj, TESObjectREFR *sourceRef, void *arg03, TESObjectWEAP *weapon, float posX, float posY,
	float posZ, float rotS, float rotX, void *arg10, float arg11, bool arg12, bool arg13, float spread1, float spread2, UInt32 arg16) =
	(Projectile* (*)(BGSProjectile*, TESObjectREFR*, void*, TESObjectWEAP*, float, float, float, float, float, void*, float, bool, bool, float, float, UInt32))0x9BCA60;
void (*FormatString)(char *destStr, UInt32 size, const char *formatStr, ...) = (void (*)(char*, UInt32, const char*, ...))0x406D00;
BSFile* (*OpenStream)(const char *filePath, UInt32 openMode, UInt32 bufferSize) = (BSFile* (*)(const char*, UInt32, UInt32))0xAFDF00;
float (__thiscall *GetItemHealthPerc)(ContChangesEntry *entry, bool arg1) = (float (__thiscall *)(ContChangesEntry*, bool))0x4BCDB0;
NiNode* (__thiscall *LoadModel)(ModelLoader *modelLoader, const char *nifPath, UInt32 baseClass, bool flag3Cbit0, UInt32 unused, bool flag3Cbit5, bool dontIncCounter) =
	(NiNode* (__thiscall *)(ModelLoader*, const char*, UInt32, bool, UInt32, bool, bool))0x447080;
FontInfo* (__thiscall *InitFontInfo)(FontInfo *fontInfo, UInt32 fontID, const char *filePath, bool arg3) = (FontInfo* (__thiscall *)(FontInfo*, UInt32, const char*, bool))0xA12020;

Cmd_Execute SayTo, KillActor, AddNote, AttachAshPile, GetRefs;

alignas(16) char
s_strArgBuffer[0x4000],
s_strValBuffer[0x10000],
s_dataPathFull[0x100] = "Data\\",
s_configPathFull[0x100] = "Data\\Config\\",
s_scriptsPathFull[0x100] = "Data\\NVSE\\plugins\\scripts\\",
s_modLogPathFull[0x100] = "Mod Logs\\";
char *s_dataPath, *s_configPath, *s_scriptsPath, *s_modLogPath;

enum
{
	kSerializedFlag_NoHardcoreTracking =	1 << 0,
};

UInt32 s_serializedFlags = 0;

bool MainLoopHasCallback(void *cmdPtr, void *thisObj);
bool MainLoopRemoveCallback(void *cmdPtr, void *thisObj);
void MainLoopAddCallback(void *cmdPtr, void *thisObj);
void MainLoopAddCallbackArgs(void *cmdPtr, void *thisObj, UInt8 numArgs, ...);
void MainLoopAddCallbackEx(void *cmdPtr, void *thisObj, UInt32 callCount, UInt32 callDelay);
void MainLoopAddCallbackArgsEx(void *cmdPtr, void *thisObj, UInt32 callCount, UInt32 callDelay, UInt8 numArgs, ...);

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

UnorderedSet<TESForm*> s_tempFormList(0x40);

Vector<ArrayElementL> s_tempElements(0x100);

void Sky::RefreshMoon()
{
	if (masserMoon) masserMoon->Destroy(true);
	masserMoon = (Moon*)GameHeapAlloc(sizeof(Moon));
	ThisCall(0x634A70, masserMoon, (const char*)0x104EEB0, *(UInt32*)0x11CCCBC, *(UInt32*)0x11CCC98, *(UInt32*)0x11CCBA8, *(UInt32*)0x11CCC00, *(UInt32*)0x11CCC58, *(UInt32*)0x11CCC1C);
	masserMoon->Refresh(niNode008, (const char*)0x104EEB0);
}

__declspec(naked) bool Sky::GetIsRaining()
{
	__asm
	{
		mov		eax, [ecx+0x10]
		test	eax, eax
		jz		checkSecond
		cmp		byte ptr [eax+0xEB], 4
		jz		weatherPerc
	checkSecond:
		mov		eax, [ecx+0x14]
		test	eax, eax
		jz		retnFalse
		cmp		byte ptr [eax+0xEB], 4
		jnz		retnFalse
	weatherPerc:
		movss	xmm0, kFltOne
		comiss	xmm0, [ecx+0xF4]
		setbe	al
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

ExtraTerminalState *ExtraTerminalState::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraTerminalState));
	dataPtr[0] = kVtbl_ExtraTerminalState;
	dataPtr[1] = kExtraData_TerminalState;
	dataPtr[2] = 0;
	dataPtr[3] = 0;
	return (ExtraTerminalState*)dataPtr;
}

ExtraCellWaterType *ExtraCellWaterType::Create(TESWaterForm *_waterForm)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellWaterType));
	dataPtr[0] = kVtbl_ExtraCellWaterType;
	dataPtr[1] = kExtraData_CellWaterType;
	dataPtr[2] = 0;
	ExtraCellWaterType *xCellWater = (ExtraCellWaterType*)dataPtr;
	xCellWater->waterForm = _waterForm;
	return xCellWater;
}

ExtraCellMusicType *ExtraCellMusicType::Create(BGSMusicType *_musicType)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellMusicType));
	dataPtr[0] = kVtbl_ExtraCellMusicType;
	dataPtr[1] = kExtraData_CellMusicType;
	dataPtr[2] = 0;
	ExtraCellMusicType *xMusicType = (ExtraCellMusicType*)dataPtr;
	xMusicType->musicType = _musicType;
	return xMusicType;
}

ExtraCellClimate *ExtraCellClimate::Create(TESClimate *_climate)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellClimate));
	dataPtr[0] = kVtbl_ExtraCellClimate;
	dataPtr[1] = kExtraData_CellClimate;
	dataPtr[2] = 0;
	ExtraCellClimate *xCellClimate = (ExtraCellClimate*)dataPtr;
	xCellClimate->climate = _climate;
	return xCellClimate;
}

ExtraLinkedRef *ExtraLinkedRef::Create(TESObjectREFR *_linkedRef)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraLinkedRef));
	dataPtr[0] = kVtbl_ExtraLinkedRef;
	dataPtr[1] = kExtraData_LinkedRef;
	dataPtr[2] = 0;
	ExtraLinkedRef *xLinkedRef = (ExtraLinkedRef*)dataPtr;
	xLinkedRef->linkedRef = _linkedRef;
	return xLinkedRef;
}

ExtraObjectHealth *ExtraObjectHealth::Create(float _health)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraObjectHealth));
	dataPtr[0] = kVtbl_ExtraObjectHealth;
	dataPtr[1] = kExtraData_ObjectHealth;
	dataPtr[2] = 0;
	ExtraObjectHealth *xHealth = (ExtraObjectHealth*)dataPtr;
	xHealth->health = _health;
	return xHealth;
}

ExtraCellImageSpace *ExtraCellImageSpace::Create(TESImageSpace *_imgSpace)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellImageSpace));
	dataPtr[0] = kVtbl_ExtraCellImageSpace;
	dataPtr[1] = kExtraData_CellImageSpace;
	dataPtr[2] = 0;
	ExtraCellImageSpace *xCellIS = (ExtraCellImageSpace*)dataPtr;
	xCellIS->imageSpace = _imgSpace;
	return xCellIS;
}

ExtraRadius *ExtraRadius::Create(float _radius)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraRadius));
	dataPtr[0] = kVtbl_ExtraRadius;
	dataPtr[1] = kExtraData_Radius;
	dataPtr[2] = 0;
	ExtraRadius *xRadius = (ExtraRadius*)dataPtr;
	xRadius->radius = _radius;
	return xRadius;
}

ExtraPrimitive *ExtraPrimitive::Create(BGSPrimitive *_primitive)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraPrimitive));
	dataPtr[0] = kVtbl_ExtraPrimitive;
	dataPtr[1] = kExtraData_Primitive;
	dataPtr[2] = 0;
	ExtraPrimitive *xPrimitive = (ExtraPrimitive*)dataPtr;
	xPrimitive->primitive = _primitive;
	return xPrimitive;
}

ExtraCellAcousticSpace *ExtraCellAcousticSpace::Create(BGSAcousticSpace *_acousticSpace)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCellAcousticSpace));
	dataPtr[0] = kVtbl_ExtraCellAcousticSpace;
	dataPtr[1] = kExtraData_CellAcousticSpace;
	dataPtr[2] = 0;
	ExtraCellAcousticSpace *xCellAcoustic = (ExtraCellAcousticSpace*)dataPtr;
	xCellAcoustic->acousticSpace = _acousticSpace;
	return xCellAcoustic;
}

ExtraSpecialRenderFlags *ExtraSpecialRenderFlags::Create(UInt32 _flags)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraSpecialRenderFlags));
	dataPtr[0] = kVtbl_ExtraSpecialRenderFlags;
	dataPtr[1] = kExtraData_SpecialRenderFlags;
	dataPtr[2] = 0;
	ExtraSpecialRenderFlags *xRenderFlags = (ExtraSpecialRenderFlags*)dataPtr;
	xRenderFlags->flags = _flags;
	xRenderFlags->unk10 = 0;
	return xRenderFlags;
}

ExtraOriginalReference *ExtraOriginalReference::Create(TESObjectREFR *_originalRef)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraOriginalReference));
	dataPtr[0] = kVtbl_ExtraOriginalReference;
	dataPtr[1] = kExtraData_OriginalReference;
	dataPtr[2] = 0;
	ExtraOriginalReference *xOriginalRef = (ExtraOriginalReference*)dataPtr;
	xOriginalRef->originalRef = _originalRef;
	return xOriginalRef;
}

ExtraUses *ExtraUses::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraUses));
	dataPtr[0] = kVtbl_ExtraUses;
	dataPtr[1] = kExtraData_Uses;
	dataPtr[2] = 0;
	return (ExtraUses*)dataPtr;
}

ExtraTimeLeft *ExtraTimeLeft::Create()
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraTimeLeft));
	dataPtr[0] = kVtbl_ExtraTimeLeft;
	dataPtr[1] = kExtraData_TimeLeft;
	dataPtr[2] = 0;
	return (ExtraTimeLeft*)dataPtr;
}

ExtraCharge *ExtraCharge::Create(float _charge)
{
	UInt32 *dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCharge));
	dataPtr[0] = kVtbl_ExtraCharge;
	dataPtr[1] = kExtraData_Charge;
	dataPtr[2] = 0;
	ExtraCharge *xCharge = (ExtraCharge*)dataPtr;
	xCharge->charge = _charge;
	return xCharge;
}

const bool kInventoryType[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
};

bool TESForm::HasScript()
{
	TESScriptableForm *scriptable = DYNAMIC_CAST(this, TESForm, TESScriptableForm);
	return scriptable && scriptable->script;
}

bool TESForm::GetScriptAndEventList(Script **script, ScriptEventList **eventList)
{
	if (GetIsReference())
	{
		ExtraScript *xScript = GetExtraType(&((TESObjectREFR*)this)->extraDataList, Script);
		if (!xScript) return false;
		*script = xScript->script;
		*eventList = xScript->eventList;
	}
	else if (typeID == kFormType_TESQuest)
	{
		TESQuest *quest = (TESQuest*)this;
		*script = quest->scriptable.script;
		*eventList = quest->scriptEventList;
	}
	else return false;
	return *script && *eventList;
}

__declspec(naked) bool TESForm::IsItemPlayable()
{
	__asm
	{
		mov		al, [ecx+4]
		cmp		al, kFormType_TESObjectARMO
		jz		armor
		cmp		al, kFormType_TESObjectWEAP
		jz		weapon
		cmp		al, kFormType_TESAmmo
		jz		ammo
		mov		al, 1
		retn
	armor:
		test	[ecx+0x78], 0x40
		setz	al
		retn
	weapon:
		test	[ecx+0x100], 0x80
		setz	al
		retn
	ammo:
		test	[ecx+0xAC], 2
		setz	al
		retn
	}
}

UInt32 TESForm::GetItemValue()
{
	if (typeID == kFormType_AlchemyItem) return ((AlchemyItem*)this)->value;
	TESValueForm *valForm = DYNAMIC_CAST(this, TESForm, TESValueForm);
	return valForm ? valForm->value : 0;
}

UInt8 TESForm::GetOverridingModIdx()
{
	ModInfo *info = mods.GetLastItem();
	return info ? info->modIndex : 0xFF;
}

const char *TESForm::GetDescriptionText()
{
	if (typeID == kFormType_BGSNote)
	{
		BGSNote *note = (BGSNote*)this;
		return ((note->noteType == 1) && note->noteText) ? note->noteText->Get(this, 'MANT') : NULL;
	}
	if (typeID == kFormType_TESLoadScreen)
	{
		TESLoadScreen *loadScreen = (TESLoadScreen*)this;
		return loadScreen->screenText.m_dataLen ? loadScreen->screenText.m_data : NULL;
	}
	TESDescription *description = DYNAMIC_CAST(this, TESForm, TESDescription);
	return description ? description->Get(this, 'CSED') : NULL;
}

UnorderedMap<UInt32, const char*> s_refStrings;

const char *TESForm::RefToString()
{
	const char **findID;
	if (!s_refStrings.Insert(refID, &findID)) return *findID;
	const char *modName = g_dataHandler->GetNthModName(modIndex);
	UInt32 length = StrLen(modName);
	char *refStr = (char*)malloc(length + 8);
	if (length) memcpy(refStr, modName, length);
	refStr[length++] = ':';
	UIntToHex(refStr + length, refID & 0xFFFFFF);
	*findID = refStr;
	return refStr;
}

const char *TESForm::GetModelPath()
{
	TESModel *baseModel = DYNAMIC_CAST(this, TESForm, TESModel);
	if (baseModel)
	{
		const char *modelPath = baseModel->GetModelPath();
		if (modelPath && *modelPath)
			return modelPath;
	}
	return NULL;
}

void TESForm::UnloadModel()
{
	const char *modelPath = GetModelPath();
	if (modelPath)
	{
		Model *model = NULL;
		if (g_modelLoader->modelMap->Lookup(modelPath, &model))
		{
			g_modelLoader->modelMap->EraseKey(modelPath);
			model->Destroy();
		}
	}
}

TESLeveledList *TESForm::GetLvlList()
{
	if (IS_ID(this, TESLevCreature) || IS_ID(this, TESLevCharacter) || IS_ID(this, TESLevItem))
		return &((TESLevCreature*)this)->list;
	return NULL;
}

UnorderedMap<char*, UInt32> s_strRefs;

UInt32 __fastcall StringToRef(char *refStr)
{
	UInt32 *findStr;
	if (!s_strRefs.Insert(refStr, &findStr)) return *findStr;
	*findStr = 0;
	char *colon = strchr(refStr, ':');
	if (colon)
	{
		UInt8 modIdx;
		if (colon != refStr)
		{
			*colon = 0;
			modIdx = g_dataHandler->GetModIndex(refStr);
			if (modIdx == 0xFF) return 0;
		}
		else modIdx = 0xFF;
		*findStr = (modIdx << 24) | HexToUInt(colon + 1);
		return *findStr;
	}
	return ResolveRefID(HexToUInt(refStr), findStr) ? *findStr : 0;
}

__declspec(naked) TESForm *TESObjectREFR::GetBaseForm()
{
	__asm
	{
		mov		eax, [ecx+0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0xF], 0xFF
		jnz		done
		cmp		dword ptr [eax], kVtbl_BGSPlaceableWater
		jz		isWater
		push	eax
		push	kExtraData_LeveledCreature
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		pop		ecx
		test	eax, eax
		cmovz	eax, ecx
		jz		done
		mov		eax, [eax+0xC]
		retn
	isWater:
		mov		eax, [eax+0x4C]
	done:
		retn
	}
}

__declspec(naked) TESForm *TESObjectREFR::GetBaseForm2()
{
	__asm
	{
		mov		eax, [ecx+0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0xF], 0xFF
		jnz		done
		cmp		dword ptr [eax], kVtbl_BGSPlaceableWater
		jz		isWater
		push	eax
		push	kExtraData_LeveledCreature
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		pop		ecx
		test	eax, eax
		cmovz	eax, ecx
		jz		done
		mov		eax, [eax+0x10]
		retn
	isWater:
		mov		eax, [eax+0x4C]
	done:
		retn
	}
}

__declspec(naked) bool TESObjectREFR::GetDisabled()
{
	__asm
	{
		push	ecx
		test	dword ptr [ecx+8], 0x800
		jz		notPerm
		CALL_EAX(0x5AA680)
		xor		al, 1
		jnz		done
	notPerm:
		CALL_EAX(0x5AA630)
	done:
		pop		ecx
		retn
	}
}

__declspec(naked) ContChangesEntry* __fastcall ExtraContainerChanges::EntryDataList::FindForItem(TESForm *item)
{
	__asm
	{
	listIter:
		mov		eax, [ecx]
		test	eax, eax
		jz		listNext
		cmp		[eax+8], edx
		jz		done
	listNext:
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		listIter
		xor		eax, eax
	done:
		retn
	}
}

ExtraContainerChanges *TESObjectREFR::GetOrCreateContainerChanges()
{
	ExtraContainerChanges *xChanges = GetExtraType(&extraDataList, ContainerChanges);
	if (!xChanges)
	{
		xChanges = ExtraContainerChanges::Create();
		AddExtraData(&extraDataList, xChanges);
	}
	if (!xChanges->data) xChanges->data = ExtraContainerChanges::Data::Create(this);
	if (!xChanges->data->objList)
	{
		xChanges->data->objList = (ExtraContainerChanges::EntryDataList*)GameHeapAlloc(sizeof(ExtraContainerChanges::EntryDataList));
		xChanges->data->objList->Init();	
	}
	return xChanges;
}

__declspec(naked) ExtraContainerChanges::EntryDataList *TESObjectREFR::GetContainerChangesList()
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
		mov		eax, [eax]
	done:
		retn
	}
}

__declspec(naked) ContChangesEntry *TESObjectREFR::GetContainerChangesEntry(TESForm *itemForm)
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		edx, [esp+4]
		ALIGN 16
	itemIter:
		mov		eax, [ecx]
		test	eax, eax
		jz		itemNext
		cmp		[eax+8], edx
		jz		done
	itemNext:
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		itemIter
		xor		eax, eax
	done:
		retn	4
	}
}

__declspec(naked) SInt32 __fastcall TESContainer::GetCountForForm(TESForm *form)
{
	__asm
	{
		push	esi
		lea		esi, [ecx+4]
		xor		eax, eax
		ALIGN 16
	iterHead:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		iterNext
		cmp		[ecx+4], edx
		jnz		iterNext
		add		eax, [ecx]
		ALIGN 16
	iterNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		iterHead
		pop		esi
		retn
	}
}

__declspec(naked) SInt32 __fastcall GetFormCount(TESContainer::FormCountList *formCountList, ExtraContainerChanges::EntryDataList *objList, TESForm *form)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, [esp+0xC]
		xor		edi, edi
		ALIGN 16
	contIter:
		mov		eax, [ecx]
		test	eax, eax
		jz		contNext
		cmp		[eax+4], esi
		jnz		contNext
		add		edi, [eax]
		ALIGN 16
	contNext:
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		contIter
		jmp		doXtra
		ALIGN 16
	xtraIter:
		mov		ecx, [edx]
		test	ecx, ecx
		jz		xtraNext
		cmp		[ecx+8], esi
		jnz		xtraNext
		mov		esi, ecx
		test	edi, edi
		jz		noCont
		call	ExtraContainerChanges::EntryData::HasExtraLeveledItem
		test	al, al
		jnz		noCont
		mov		eax, [esi+4]
		add		eax, edi
		js		retnZero
		jmp		done
	noCont:
		mov		eax, [esi+4]
		test	eax, eax
		jge		done
	retnZero:
		xor		eax, eax
		jmp		done
		ALIGN 16
	xtraNext:
		mov		edx, [edx+4]
	doXtra:
		test	edx, edx
		jnz		xtraIter
		mov		eax, edi
	done:
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) SInt32 TESObjectREFR::GetItemCount(TESForm *form)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	ecx
		call	TESObjectREFR::GetContainer
		test	eax, eax
		jz		done
		mov		ecx, [ebp-4]
		add		eax, 4
		mov		[ebp-4], eax
		call	TESObjectREFR::GetContainerChangesList
		mov		edx, [ebp+8]
		cmp		byte ptr [edx+4], kFormType_BGSListForm
		jz		itemList
		push	edx
		mov		edx, eax
		mov		ecx, [ebp-4]
		call	GetFormCount
		jmp		done
	itemList:
		mov		[ebp-8], eax
		push	esi
		push	edi
		lea		esi, [edx+0x18]
		mov		edi, 0
		ALIGN 16
	iterHead:
		mov		eax, [esi]
		test	eax, eax
		jz		iterNext
		push	eax
		mov		edx, [ebp-8]
		mov		ecx, [ebp-4]
		call	GetFormCount
		add		edi, eax
	iterNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		iterHead
		mov		eax, edi
		pop		edi
		pop		esi
	done:
		leave
		retn	4
	}
}

__declspec(naked) void Actor::EquipItemAlt(TESForm *itemForm, ContChangesEntry *entry, UInt32 noUnequip, UInt32 noMessage)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		mov		ecx, 1
		mov		eax, [ebp+8]
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESObjectARMO
		jz		doneType
		cmp		dl, kFormType_TESObjectBOOK
		jz		doneType
		cmp		dl, kFormType_AlchemyItem
		jz		doneType
		cmp		dl, kFormType_TESAmmo
		jz		countMax
		cmp		dl, kFormType_TESObjectWEAP
		jnz		done
		cmp		byte ptr [eax+0xF4], 0xA
		jb		doneType
	countMax:
		xor		ecx, ecx
	doneType:
		push	dword ptr [ebp+0x14]
		push	dword ptr [ebp+0x10]
		push	1
		mov		eax, [ebp+0xC]
		test	eax, eax
		jz		noEntry
		mov		edx, [eax]
		test	edx, edx
		jz		noExtra
		mov		edx, [edx]
	noExtra:
		push	edx
		test	ecx, ecx
		cmovz	ecx, [eax+4]
		push	ecx
		jmp		doEquip
	noEntry:
		push	0
		push	1
	doEquip:
		push	dword ptr [ebp+8]
		mov		ecx, [ebp-4]
		CALL_EAX(kAddr_EquipItem)
	done:
		leave
		retn	0x10
	}
}

__declspec(naked) void TESObjectREFR::AddItemAlt(TESForm *form, UInt32 count, float condition, bool doEquip)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 0x10
		push	esi
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x481610)
		mov		ecx, [ebp+8]
		movzx	eax, byte ptr [ecx+4]
		cmp		al, kFormType_TESLevItem
		jz		lvlItem
		cmp		al, kFormType_BGSListForm
		jz		itemList
		cmp		kInventoryType[eax], 0
		jz		done
		push	0
		push	dword ptr [ebp+0xC]
		push	ecx
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x4818E0)
		jmp		doMove
	lvlItem:
		push	0
		mov		ecx, [ebp-4]
		CALL_EAX(0x567E10)
		push	0
		lea		ecx, [ebp-0x10]
		push	ecx
		push	dword ptr [ebp+0xC]
		push	eax
		mov		ecx, [ebp+8]
		add		ecx, 0x30
		CALL_EAX(0x487F70)
		jmp		doMove
	itemList:
		mov		esi, [ebp+8]
		add		esi, 0x18
	listIter:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		listNext
		movzx	eax, byte ptr [ecx+4]
		cmp		kInventoryType[eax], 0
		jz		listNext
		push	0
		push	dword ptr [ebp+0xC]
		push	ecx
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x4818E0)
	listNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		listIter
	doMove:
		push	dword ptr [ebp+0x10]
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x482090)
		push	1
		push	dword ptr [ebp-4]
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x4821A0)
		cmp		byte ptr [ebp+0x14], 0
		jz		done
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		done
		call	TESObjectREFR::GetContainerChangesList
		test	eax, eax
		jz		done
		mov		[ebp-0x14], eax
		lea		esi, [ebp-0xC]
	eqpIter:
		mov		eax, [esi]
		test	eax, eax
		jz		eqpNext
		push	1
		push	0
		mov		edx, [eax+4]
		mov		ecx, [ebp-0x14]
		call	ExtraContainerChanges::EntryDataList::FindForItem
		push	eax
		mov		eax, [esi]
		push	dword ptr [eax+4]
		mov		ecx, [ebp-4]
		call	Actor::EquipItemAlt
	eqpNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		eqpIter
	done:
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x481680)
		pop		esi
		leave
		retn	0x10
	}
}

void TESObjectREFR::RemoveItemTarget(TESForm *itemForm, TESObjectREFR *target, SInt32 quantity, bool keepOwner)
{
	if IS_ID(itemForm, BGSListForm)
	{
		ListNode<TESForm> *lstIter = ((BGSListForm*)itemForm)->list.Head();
		do
		{
			if (lstIter->data)
				RemoveItemTarget(lstIter->data, target, quantity, keepOwner);
		}
		while (lstIter = lstIter->next);
	}
	else if (kInventoryType[itemForm->typeID])
	{
		SInt32 total = GetItemCount(itemForm);
		if (total < 1) return;
		if ((quantity > 0) && (quantity < total))
			total = quantity;
		if (itemForm->HasScript())
		{
			ContChangesEntry *entry = GetContainerChangesEntry(itemForm);
			if (entry && entry->extendData)
			{
				ExtraDataList *xData;
				SInt32 subCount;
				while ((total > 0) && (xData = entry->extendData->GetFirstItem()))
				{
					subCount = xData->GetCount();
					if (subCount < 1)
						continue;
					if (subCount > total)
						subCount = total;
					RemoveItem(itemForm, xData, subCount, keepOwner, 0, target, 0, 0, 1, 0);
					total -= subCount;
				}
			}
		}
		if (total > 0)
			RemoveItem(itemForm, NULL, total, keepOwner, 0, target, 0, 0, 1, 0);
	}
}

struct InventoryItemData
{
	SInt32				count;
	ContChangesEntry	*entry;

	InventoryItemData(SInt32 _count, ContChangesEntry *_entry) : count(_count), entry(_entry) {}
};

typedef UnorderedMap<TESForm*, InventoryItemData> InventoryItemsMap;
InventoryItemsMap s_inventoryItemsMap(0x40);

bool TESObjectREFR::GetInventoryItems(UInt8 typeID)
{
	TESContainer *container = GetContainer();
	if (!container) return false;
	ExtraContainerChanges::EntryDataList *entryList = GetContainerChangesList();
	if (!entryList) return false;
	s_inventoryItemsMap.Clear();

	TESContainer::FormCount *formCount;
	TESForm *item;
	SInt32 contCount, countDelta;
	ContChangesEntry *entry;

	ListNode<TESContainer::FormCount> *contIter = container->formCountList.Head();
	do
	{
		if (!(formCount = contIter->data)) continue;
		item = formCount->form;
		if ((typeID && (item->typeID != typeID)) || (item->typeID == kFormType_TESLevItem) || s_inventoryItemsMap.HasKey(item))
			continue;
		contCount = container->GetCountForForm(item);
		if (entry = entryList->FindForItem(item))
		{
			countDelta = entry->countDelta;
			if (entry->HasExtraLeveledItem())
				contCount = countDelta;
			else contCount += countDelta;
		}
		if (contCount > 0)
			s_inventoryItemsMap.Emplace(item, contCount, entry);
	}
	while (contIter = contIter->next);

	ListNode<ContChangesEntry> *xtraIter = entryList->Head();
	do
	{
		if (!(entry = xtraIter->data)) continue;
		item = entry->type;
		if ((typeID && (item->typeID != typeID)) || s_inventoryItemsMap.HasKey(item))
			continue;
		countDelta = entry->countDelta;
		if (countDelta > 0)
			s_inventoryItemsMap.Emplace(item, countDelta, entry);
	}
	while (xtraIter = xtraIter->next);

	return !s_inventoryItemsMap.Empty();
}

__declspec(naked) TESObjectCELL *TESObjectREFR::GetParentCell()
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		done
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) TESWorldSpace *TESObjectREFR::GetParentWorld()
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		getWorld
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	getWorld:
		mov		eax, [eax+0xC0]
	done:
		retn
	}
}

__declspec(naked) float __vectorcall GetDistance3D(TESObjectREFR *ref1, TESObjectREFR *ref2)
{
	__asm
	{
		movups	xmm0, [ecx+0x30]
		movups	xmm1, [edx+0x30]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		movhlps	xmm1, xmm0
		addss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
		retn
	}
}

__declspec(naked) float __vectorcall GetDistance2D(TESObjectREFR *ref1, TESObjectREFR *ref2)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx+0x30]
		movq	xmm1, qword ptr [edx+0x30]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		movss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
		retn
	}
}

__declspec(naked) bool __fastcall TESObjectREFR::GetInSameCellOrWorld(TESObjectREFR *target)
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		hasCell1
		push	edx
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		CALL_EAX(kAddr_GetExtraData)
		pop		edx
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	hasCell1:
		mov		ecx, [edx+0x40]
		test	ecx, ecx
		jnz		hasCell2
		push	eax
		push	kExtraData_PersistentCell
		lea		ecx, [edx+0x44]
		CALL_EAX(kAddr_GetExtraData)
		pop		edx
		test	eax, eax
		jz		done
		mov		ecx, [eax+0xC]
		mov		eax, edx
	hasCell2:
		cmp		eax, ecx
		jz		retnTrue
		mov		eax, [eax+0xC0]
		test	eax, eax
		jz		done
		cmp		eax, [ecx+0xC0]
	retnTrue:
		setz	al
	done:
		retn
	}
}

__declspec(naked) float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR *target)
{
	__asm
	{
		push	ecx
		push	edx
		call	TESObjectREFR::GetInSameCellOrWorld
		pop		edx
		pop		ecx
		test	al, al
		jz		fltMax
		jmp		GetDistance3D
	fltMax:
		movss	xmm0, kFltMax
		retn
	}
}

__declspec(naked) void __fastcall DoSetPos(TESObjectREFR *refr, int EDX, float posX, float posY, float posZ)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 0x10
		lea		eax, [ebp+8]
		push	eax
		CALL_EAX(0x575830)
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		noCharCtrl
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		noCharCtrl
		mov		eax, [ecx]
		call	dword ptr [eax+0x28C]
		test	eax, eax
		jz		noCharCtrl
		mov		[ebp-8], eax
		mov		ecx, eax
		CALL_EAX(0x5C0860)
		test	al, al
		jnz		noCharCtrl
		lea		eax, [ebp+8]
		push	eax
		mov		ecx, [ebp-8]
		CALL_EAX(0x5620E0)
	noCharCtrl:
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x1D0]
		test	eax, eax
		jz		done
		mov		[ebp-8], eax
		mov		ecx, eax
		lea		eax, [ebp+8]
		push	eax
		CALL_EAX(0x440460)
		push	1
		push	dword ptr [ebp-8]
		CALL_EAX(0xC6BD00)
		add		esp, 8
		lea		eax, [ebp-0x14]
		xor		edx, edx
		mov		[eax], edx
		mov		[eax+4], edx
		mov		[eax+8], edx
		push	eax
		mov		ecx, [ebp-8]
		CALL_EAX(0xA59C60)
	done:
		leave
		retn	0xC
	}
}

void TESObjectREFR::SetPos(NiVector3 *posVector)
{
	if (IsActor())
		DoSetPos(this, 0, posVector->x, posVector->y, posVector->z);
	else
	{
		QueuedCmdCall qCall(DoSetPos, refID, 3);
		UInt32 *ptr = (UInt32*)posVector;
		qCall.args[0] = ptr[0];
		qCall.args[1] = ptr[1];
		qCall.args[2] = ptr[2];
		AddQueuedCmdCall(qCall);
	}
}

__declspec(naked) void __fastcall DoSetAngle(TESObjectREFR *refr, int EDX, float angX, float angY, float angZ)
{
	__asm
	{
		movss	xmm0, kFltPId180
		movss	xmm1, [esp+4]
		mulss	xmm1, xmm0
		movss	[ecx+0x24], xmm1
		movss	xmm1, [esp+8]
		mulss	xmm1, xmm0
		movss	[ecx+0x28], xmm1
		mulss	xmm0, [esp+0xC]
		movss	[ecx+0x2C], xmm0
		push	0
		push	0
		push	ecx
		push	2
		mov		eax, [ecx]
		call	dword ptr [eax+0x48]
		CALL_EAX(0x5C0B10)
		add		esp, 0xC
		retn	0xC
	}
}

void TESObjectREFR::SetAngle(NiVector3 *rotVector)
{
	if (IsActor())
		DoSetAngle(this, 0, rotVector->x, rotVector->y, rotVector->z);
	else
	{
		QueuedCmdCall qCall(DoSetAngle, refID, 3);
		UInt32 *ptr = (UInt32*)rotVector;
		qCall.args[0] = ptr[0];
		qCall.args[1] = ptr[1];
		qCall.args[2] = ptr[2];
		AddQueuedCmdCall(qCall);
	}
}

__declspec(naked) void __fastcall DoMoveToCell(TESObjectREFR *refr, int EDX, TESObjectCELL *cell, float posX, float posY, float posZ)
{
	__asm
	{
		mov		eax, s_tempPosMarker
		mov		edx, [esp+4]
		mov		[eax+0x40], edx
		movq	xmm0, qword ptr [esp+8]
		movq	qword ptr [eax+0x30], xmm0
		mov		edx, [esp+0x10]
		mov		[eax+0x38], edx
		push	0
		push	0
		push	0
		push	eax
		push	ecx
		CALL_EAX(kAddr_MoveToMarker)
		add		esp, 0x14
		retn	0x10
	}
}

bool TESObjectREFR::MoveToCell(TESForm *worldOrCell, NiVector3 *posVector)
{
	if (!worldOrCell) return false;
	TESObjectCELL *cell = (TESObjectCELL*)worldOrCell;
	TESWorldSpace *world = NULL;
	if IS_ID(cell, TESObjectCELL)
		world = cell->worldSpace;
	else if IS_ID(cell, TESWorldSpace)
	{
		world = (TESWorldSpace*)cell;
		cell = world->cell;
	}
	else return false;
	TESObjectCELL *currCell = GetParentCell();
	if (!currCell) return false;
	if ((cell == currCell) || (world && (world == currCell->worldSpace)))
		SetPos(posVector);
	else
	{
		QueuedCmdCall qCall(DoMoveToCell, refID, 4);
		UInt32 *ptr = (UInt32*)posVector;
		qCall.args[0] = cell;
		qCall.args[1] = ptr[0];
		qCall.args[2] = ptr[1];
		qCall.args[3] = ptr[2];
		AddQueuedCmdCall(qCall);
	}
	return true;
}

__declspec(naked) bool TESObjectREFR::Disable()
{
	__asm
	{
		test	dword ptr [ecx+8], 0x4800
		jnz		retnFalse
		test	byte ptr [ecx+0x52], 0x80
		jnz		retnFalse
		push	0
		push	ecx
		CALL_EAX(0x5AA500)
		add		esp, 8
		mov		al, 1
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

__declspec(naked) void TESObjectREFR::DeleteReference()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	1
		mov		eax, [ecx]
		call	dword ptr [eax+0xC4]
		push	1
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0xC8]
		push	0
		push	0
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x1CC]
		mov		ecx, [ebp-4]
		cmp		byte ptr [ecx+0xF], 0xFF
		jnz		done
		lea		eax, [ebp-4]
		push	eax
		mov		ecx, 0x11CACB8
		CALL_EAX(0x5AE3D0)
	done:
		leave
		retn
	}
}

__declspec(naked) bhkCharacterController *TESObjectREFR::GetCharacterController()
{
	__asm
	{
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		retnNULL
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnNULL
		cmp		dword ptr [ecx+0x28], 1
		ja		retnNULL
		mov		eax, [ecx+0x138]
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

TESObjectREFR *TESObjectREFR::GetMerchantContainer()
{
	ExtraMerchantContainer *xMerchCont = GetExtraType(&extraDataList, MerchantContainer);
	return xMerchCont ? xMerchCont->containerRef : NULL;
}

__declspec(naked) float TESObjectREFR::GetWaterImmersionPerc()	// result >= 0.875 --> actor is diving
{
	__asm
	{
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		invalid
		cmp		byte ptr [ecx+0x14C], 0
		jz		invalid
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		invalid
		cmp		[eax+0x28], 0
		jnz		invalid
		push	dword ptr [ecx+0x40]
		push	dword ptr [ecx+0x38]
		CALL_EAX(0x885560)
		retn
	invalid:
		fldz
		retn
	}
}

bool TESObjectREFR::IsMobile()
{
	if (IsActor() || IsProjectile())
		return true;
	NiNode *objNode = GetNiNode();
	return objNode && objNode->IsMovable();
}

__declspec(naked) void TESObjectREFR::SwapTexture(const char *blockName, const char *filePath, UInt32 texIdx)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 8
		mov		eax, [ecx]
		call	dword ptr [eax+0x1D0]
		test	eax, eax
		jz		done
		mov		edx, [ebp+8]
		mov		ecx, eax
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		push	3
		mov		ecx, eax
		CALL_EAX(0xA59D30)
		test	eax, eax
		jz		done
		mov		[ebp-4], eax
		push	0
		push	0
		lea		ecx, [ebp-8]
		mov		dword ptr [ecx], 0
		push	ecx
		push	dword ptr [ebp+0xC]
		mov		ecx, g_TES
		CALL_EAX(0x4568C0)
		push	0x11F5AE0
		mov		ecx, [ebp-4]
		CALL_EAX(0x653290)
		test	eax, eax
		jz		noLighting
		mov		edx, [eax+0x1C]
		cmp		edx, 8
		jl		noLighting
		cmp		edx, 0xC
		jg		noLighting
		push	dword ptr [ebp-8]
		push	0
		push	dword ptr [ebp+0x10]
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xF0]
	noLighting:
		push	0x11FA05C
		mov		ecx, [ebp-4]
		CALL_EAX(0x653290)
		test	eax, eax
		jz		release
		push	dword ptr [ebp-8]
		mov		ecx, eax
		CALL_EAX(0x438230)
	release:
		mov		ecx, [ebp-8]
		test	ecx, ecx
		jz		done
		call	NiReleaseObject
	done:
		leave
		retn	0xC
	}
}

bool TESObjectREFR::IsGrabbable()
{
	if (IsActor())
		return *(bool*)0x11E0B20 || HasHealth(0);
	if (IsProjectile())
	{
		Projectile *projRefr = (Projectile*)this;
		return (projRefr->projFlags & 0x200) && (projRefr->GetProjectileType() == 3) && projRefr->IsProximityTriggered();
	}
	NiNode *objNode = GetNiNode();
	return objNode && objNode->IsMovable();
}

bool TESObjectREFR::ValidForHooks()
{
	if (IsActor()) return IsPersistent();
	return !kInventoryType[baseForm->typeID] && !IsProjectile();
}

__declspec(naked) NiAVObject* __fastcall TESObjectREFR::GetNiBlock(const char *blockName)
{
	__asm
	{
		push	edx
		mov		eax, [ecx]
		call	dword ptr [eax+0x1D0]
		pop		edx
		test	eax, eax
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
	done:
		retn
	}
}

__declspec(naked) NiNode* __fastcall TESObjectREFR::GetNode(const char *nodeName)
{
	__asm
	{
		push	edx
		mov		eax, [ecx]
		call	dword ptr [eax+0x1D0]
		pop		edx
		test	eax, eax
		jz		done
		cmp		[edx], 0
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
	done:
		retn
	}
}

NiNode *TESObjectREFR::GetNiNodeCopyIfTemplate()
{
	NiNode *rootNode = GetNiNode();
	if (rootNode)
	{
		const char *modelPath = GetBaseForm2()->GetModelPath();
		if (modelPath)
		{
			Model *model = NULL;
			if (g_modelLoader->modelMap->Lookup(modelPath, &model) && (model->niNode == rootNode))
				NiReleaseAddRef((NiRefObject**)&model->niNode, rootNode->CreateCopy());
		}
	}
	return rootNode;
}

hkpRigidBody *TESObjectREFR::GetRigidBody(const char *nodeName)
{
	NiNode *rootNode = GetNiNode();
	if (rootNode)
	{
		NiNode *targetNode = rootNode->GetNode(nodeName);
		if (targetNode && targetNode->m_collisionObject)
		{
			bhkWorldObject *hWorldObj = targetNode->m_collisionObject->worldObj;
			if (hWorldObj)
			{
				hkpRigidBody *rigidBody = (hkpRigidBody*)hWorldObj->refObject;
				UInt8 motionType = rigidBody->motion.type;
				if ((motionType == 2) || (motionType == 3) || (motionType == 6))
					return rigidBody;
			}
		}
	}
	return NULL;
}

__declspec(naked) NiAVObject* __fastcall _GetRayCastObject(void *rcData, void *args)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0xC
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		mov		eax, [edi+0xC]
		add		eax, [edi+0x14]
		mov		ecx, [edi+8]
		lea		edx, [ebp-0xC]
		movss	xmm0, [edi+0x10]
		movss	xmm1, [eax+0x18]
		mulss	xmm1, xmm0
		addss	xmm1, [ecx+8]
		movss	[edx+8], xmm1
		movss	xmm1, [eax+0xC]
		mulss	xmm1, xmm0
		addss	xmm1, [ecx+4]
		movss	[edx+4], xmm1
		mulss	xmm0, [eax]
		addss	xmm0, [ecx]
		movss	[edx], xmm0
		mov		ecx, esi
		CALL_EAX(0x4A3C20)
		lea		eax, [esi+0x24]
		push	eax
		mov		ecx, g_thePlayer
		CALL_EAX(0x931ED0)
		mov		edx, [eax]
		mov		dx, [edi+0x18]
		mov		[eax], edx
		push	dword ptr [edi+8]
		mov		ecx, esi
		CALL_EAX(0x4A3DA0)
		lea		eax, [ebp-0xC]
		push	eax
		mov		ecx, esi
		CALL_EAX(0x4A3EB0)
		push	esi
		mov		ecx, g_TES
		CALL_EAX(0x458420)
		pop		edi
		pop		esi
		leave
		retn
	}
}

__declspec(naked) NiAVObject* __stdcall GetRayCastObject(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xB0
		mov		edx, ebp
		mov		ecx, esp
		call	_GetRayCastObject
		leave
		retn	0x14
	}
}

__declspec(naked) bool NiVector3::RayCastCoords(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xC0
		mov		[esp], ecx
		mov		edx, ebp
		lea		ecx, [esp+0x10]
		call	_GetRayCastObject
		push	dword ptr [esp]
		lea		ecx, [esp+0x14]
		CALL_EAX(0x5DBE60)
		push	dword ptr [esp]
		mov		ecx, g_TES
		CALL_EAX(0x451110)
		leave
		retn	0x14
	}
}

alignas(16) static const UInt8 kMaterialConvert[] =
{
	kMaterial_Stone,
	kMaterial_Cloth,
	kMaterial_Dirt,
	kMaterial_Glass,
	kMaterial_Grass,
	kMaterial_Metal,
	kMaterial_Organic,
	kMaterial_Organic,
	kMaterial_Water,
	kMaterial_Wood,
	kMaterial_Stone,
	kMaterial_Metal,
	kMaterial_Wood,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_HollowMetal,
	kMaterial_HollowMetal,
	kMaterial_Dirt,
	kMaterial_Stone,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_HollowMetal,
	kMaterial_HollowMetal,
	kMaterial_Glass,
	kMaterial_HollowMetal,
	kMaterial_Metal,
	kMaterial_Metal,
	kMaterial_HollowMetal,
	kMaterial_HollowMetal,
	kMaterial_Wood,
	kMaterial_Wood
};

__declspec(naked) int __stdcall GetRayCastMaterial(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis, UInt16 filter)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0xC0
		mov		edx, ebp
		lea		ecx, [esp+0x10]
		call	_GetRayCastObject
		test	eax, eax
		jz		invalid
		mov		[esp+0xC], eax
		push	eax
		CALL_EAX(0x56F930)
		pop		ecx
		test	eax, eax
		jz		isTerrain
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		notActor
		mov		eax, [ecx]
		call	dword ptr [eax+0x218]
		mov		ecx, [ecx+0x20]
		test	al, al
		jz		notNPC
		mov		eax, [ecx+0x1E4]
		jmp		done
	notNPC:
		mov		eax, [ecx+0x148]
		jmp		done
	notActor:
		mov		ecx, [esp+0xC]
		mov		ecx, [ecx+0x1C]
		test	ecx, ecx
		jz		invalid
		mov		ecx, [ecx+0x10]
		test	ecx, ecx
		jz		invalid
		mov		ecx, [ecx+8]
		mov		ecx, [ecx+0x10]
		test	ecx, ecx
		jz		invalid
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		invalid
		push	0
		CALL_EAX(0xC84F10)
		jmp		convert
	isTerrain:
		push	esp
		lea		ecx, [esp+0x14]
		CALL_EAX(0x5DBE60)
		push	esp
		mov		ecx, g_TES
		CALL_EAX(0x451110)
		push	esp
		mov		ecx, g_TES
		CALL_EAX(0x457720)
		test	eax, eax
		jz		invalid
		movzx	eax, byte ptr [eax+0x1C]
	convert:
		cmp		eax, 0x1F
		ja		invalid
		mov		edx, eax
		movzx	eax, kMaterialConvert[edx]
		jmp		done
	invalid:
		mov		eax, 0xFFFFFFFF
	done:
		leave
		retn	0x14
	}
}

TESActorBase *Actor::GetActorBase()
{
	if (baseForm->modIndex == 0xFF)
	{
		ExtraLeveledCreature *xLvlCre = GetExtraType(&extraDataList, LeveledCreature);
		if (xLvlCre) return xLvlCre->actorBase;
	}
	return (TESActorBase*)baseForm;
}

bool Actor::GetLOS(Actor *target)
{
	if (baseProcess && !baseProcess->processLevel)
	{
		DetectionData *data = baseProcess->GetDetectionData(target, 0);
		if (data && data->inLOS) return true;
	}
	return false;
}

const char kMatchPackageType[] = {0, 1, 2, 3, 4, 13, 14, 15, 16, 17, 18, 19, 36, 37, 35, 34, 33, -1, 5, 20, 7, 8, 10, 11, 9, 12, 21, 24, 6, 28, 29, 32};

__declspec(naked) char Actor::GetCurrentAIPackage()
{
	__asm
	{
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnInvalid
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		retnInvalid
		movsx	ecx, byte ptr [eax+0x20]
		cmp		ecx, 0x20
		jnl		retnInvalid
		mov		al, kMatchPackageType[ecx]
		retn
	retnInvalid:
		mov		al, -1
		retn
	}
}

const char kMatchProcedureID[] =
{
	0, 7, 1, 2, 8, 10, 11, 12, 14, 13, 16, 18, 4, 3, 19, 15, 5, 20, 9, -1, 6, -1, 21, 22, 23, 24, -1, 27, 28, 
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 47, -1, 46, 41, -1, -1, 44, -1, 43, 42, 49, 50, -1, 17,
	45, 48, 8, 10, 7, 46, 4, 45, 21, 0, -1
};

__declspec(naked) char Actor::GetCurrentAIProcedure()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnInvalid
		push	ecx
		push	ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		retnInvalid
		mov		[ebp-8], eax
		cmp		byte ptr [eax+0x20], 0x12
		jnz		notCombat
		mov		ecx, eax
		CALL_EAX(0x981990)
		test	al, al
		jz		notCombat
		mov		al, 0x10
		jmp		done
	notCombat:
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x280]
		mov		edx, [ebp-8]
		mov		edx, [edx+0x18]
		mov		ecx, 0x11A3FF0
		mov		ecx, [ecx+edx*4]
		mov		ecx, [ecx+eax*4]
		cmp		ecx, 0x36
		ja		retnInvalid
		cmp		ecx, 0x2A
		jnz		getID
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x274]
		test	eax, eax
		jz		sandbox
		mov		ecx, [eax+4]
		cmp		ecx, 3
		jnz		classify
		mov		ecx, [eax+8]
		cmp		ecx, 6
		jnl		sandbox
		add		ecx, 0x38
		jmp		getID
	classify:
		test	ecx, ecx
		js		sandbox
		cmp		ecx, 4
		jnl		sandbox
		add		ecx, 0x3E
		jmp		getID
	sandbox:
		mov		al, 0x2D
		jmp		done
	getID:
		mov		al, kMatchProcedureID[ecx]
		jmp		done
	retnInvalid:
		mov		al, -1
	done:
		leave
		retn
	}
}

bool Actor::IsFleeing()
{
	if (baseProcess)
	{
		TESPackage *package = baseProcess->GetCurrentPackage();
		if (package)
		{
			char pkgType = package->type;
			if (((pkgType == 0x12) && package->GetFleeCombat()) || (pkgType == 0x16) || (pkgType == 0xA))
				return true;
		}
	}
	return false;
}

__declspec(naked) ContChangesEntry *Actor::GetWeaponInfo()
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 1
		ja		retnNULL
		mov		eax, [eax+0x114]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) ContChangesEntry *Actor::GetAmmoInfo()
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 1
		ja		retnNULL
		mov		eax, [eax+0x118]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

TESObjectWEAP *Actor::GetEquippedWeapon()
{
	ContChangesEntry *weaponInfo = GetWeaponInfo();
	return weaponInfo ? (TESObjectWEAP*)weaponInfo->type : NULL;
}

bool Actor::IsItemEquipped(TESForm *item)
{
	if IS_ID(item, TESObjectWEAP)
		return item == GetEquippedWeapon();
	if (NOT_TYPE(item, TESObjectARMO) || (typeID == kFormType_Creature))
		return false;
	ContChangesEntry *entry = GetContainerChangesEntry(item);
	if (!entry || !entry->extendData)
		return false;
	ListNode<ExtraDataList> *node = entry->extendData->Head();
	ExtraDataList *xData;
	do
	{
		xData = node->data;
		if (xData && xData->HasType(kExtraData_Worn))
			return true;
	}
	while (node = node->next);
	return false;
}

UInt8 Actor::EquippedWeaponHasMod(UInt8 modID)
{
	ContChangesEntry *weaponInfo = GetWeaponInfo();
	if (!weaponInfo) return 0;
	TESObjectWEAP *weapon = (TESObjectWEAP*)weaponInfo->type;
	if (!weapon) return 0;
	if (modID == 14)
	{
		if (!(weapon->weaponFlags1 & 4))
			return 0;
		if (!(weapon->weaponFlags2 & 0x2000))
			return 1;
	}
	else if ((modID == 11) && (weapon->soundLevel == 2))
		return 1;
	ExtraDataList *xData = weaponInfo->extendData ? weaponInfo->extendData->GetFirstItem() : NULL;
	if (!xData) return 0;
	ExtraWeaponModFlags *xModFlags = GetExtraType(xData, WeaponModFlags);
	if (!xModFlags) return 0;
	UInt8 modFlags = xModFlags->flags, idx = 3;
	while (idx--) if ((modFlags & (1 << idx)) && (weapon->effectMods[idx] == modID)) return 2;
	return 0;
}

bool Actor::IsSneaking()
{
	return actorMover && ((actorMover->GetMovementFlags() & 0xC00) == 0x400);
}

void Actor::StopCombat()
{
	if (!isInCombat) return;
	CombatController *combatCtrl = GetCombatController();
	if (combatCtrl) combatCtrl->stopCombat = true;
}

__declspec(naked) bool __fastcall Actor::IsInCombatWith(Actor *target)
{
	__asm
	{
		mov		eax, [ecx+0x12C]
		test	eax, eax
		jz		done
		mov		ecx, [eax+4]
		mov		eax, [eax+8]
		test	eax, eax
		jz		done
		ALIGN 16
	iterHead:
		cmp		[ecx], edx
		jz		rtnTrue
		add		ecx, 4
		dec		eax
		jnz		iterHead
		retn
	rtnTrue:
		mov		al, 1
	done:
		retn
	}
}

int __fastcall Actor::GetDetectionValue(Actor *detected)
{
	if (baseProcess && !baseProcess->processLevel)
	{
		DetectionData *data = baseProcess->GetDetectionData(detected, 0);
		if (data) return data->detectionValue;
	}
	return -100;
}

TESPackage *Actor::GetStablePackage()
{
	if (!baseProcess) return NULL;
	TESPackage *package = baseProcess->currentPackage.package;
	if (!package) return NULL;
	if ((package->type < 18) || (package->type == 26) || (package->type == 30)) return package;
	ExtraPackage *xPackage = GetExtraType(&extraDataList, Package);
	return xPackage ? xPackage->package : NULL;
}

PackageInfo *Actor::GetPackageInfo()
{
	if (baseProcess)
	{
		if ((baseProcess->processLevel <= 1) && ((MiddleHighProcess*)baseProcess)->interruptPackage.package)
			return &((MiddleHighProcess*)baseProcess)->interruptPackage;
		if (baseProcess->currentPackage.package)
			return &baseProcess->currentPackage;
	}
	return NULL;
}

TESObjectREFR *Actor::GetPackageTarget()
{
	return baseProcess ? baseProcess->currentPackage.targetRef : NULL;
}

TESCombatStyle *Actor::GetCombatStyle()
{
	ExtraCombatStyle *xCmbStyle = GetExtraType(&extraDataList, CombatStyle);
	if (xCmbStyle && xCmbStyle->combatStyle) return xCmbStyle->combatStyle;
	return ((TESActorBase*)baseForm)->GetCombatStyle();
}

bool Actor::GetKnockedState()
{
	return baseProcess && (baseProcess->GetKnockedState() == 3);
}

bool Actor::IsWeaponOut()
{
	return baseProcess && baseProcess->IsWeaponOut();
}

bool Actor::GetIsGhost()
{
	if (baseProcess && baseProcess->unk2C)
		return (baseProcess->unk2C->flags & 0x10000000) ? true : false;
	return extraDataList.HasType(kExtraData_Ghost);
}

float Actor::GetRadiationLevel()
{
	float result = 0;
	if (inWater)
	{
		TESWaterForm *waterForm = NULL;
		if (renderState && renderState->waterRef)
		{
			waterForm = ((BGSPlaceableWater*)renderState->waterRef->baseForm)->water;
			if (waterForm && waterForm->waterForm) waterForm = waterForm->waterForm;
		}
		else waterForm = ThisCall<TESWaterForm*>(0x547770, parentCell);
		if (waterForm && waterForm->radiation) result = waterForm->radiation * (isSwimming ? *(float*)0x11D0464 : *(float*)0x11D1468);
	}
	if (*g_loadedRefrMaps)
	{
		TESObjectREFR *refr;
		ExtraRadius *xRadius;
		ExtraRadiation *xRadiation;
		float distance;
		for (auto iter = (*g_loadedRefrMaps)[6].Begin(); !iter.End(); ++iter)
		{
			if (!(refr = iter.Get())) continue;
			xRadius = GetExtraType(&refr->extraDataList, Radius);
			if (!xRadius) continue;
			distance = xRadius->radius - GetDistance(refr);
			if (distance <= 0) continue;
			xRadiation = GetExtraType(&refr->extraDataList, Radiation);
			if (xRadiation) result += xRadiation->radiation * distance / xRadius->radius;
		}
	}
	return result ? ((1.0 - (avOwner.GetActorValue(kAVCode_RadResist) / 100.0)) * result) : 0;
}

float s_moveAwayDistance = 300.0F;

__declspec(naked) BackUpPackage *Actor::AddBackUpPackage(TESObjectREFR *targetRef, TESObjectCELL *targetCell, UInt32 flags)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	ecx
		push	0
		mov		eax, [ecx+0x108]
		cmp		eax, 3
		jz		done
		cmp		eax, 5
		jz		done
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		done
		cmp		dword ptr [ecx+0x28], 0
		jnz		done
		mov		[ebp-8], ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		noPackage
		cmp		byte ptr [eax+0x20], 0x1A
		jz		done
	noPackage:
		push	0x8C
		GAME_HEAP_ALLOC
		mov		ecx, eax
		CALL_EAX(0x9ED030)
		mov		[ebp-0xC], eax
		push	dword ptr [ebp+8]
		push	dword ptr [ebp+0xC]
		push	dword ptr [ebp-4]
		mov		ecx, eax
		CALL_EAX(0x9ED0D0)
		mov		ecx, [ebp-0xC]
		cmp		dword ptr [ebp+0xC], 0
		jnz		haveCell
		mov		eax, [ebp+8]
		mov		edx, [eax+0x30]
		mov		[ecx+0x80], edx
		mov		edx, [eax+0x34]
		mov		[ecx+0x84], edx
		mov		edx, [eax+0x38]
		mov		[ecx+0x88], edx
	haveCell:
		mov		dword ptr [ecx+0x18], 0x30
		mov		eax, [ebp+0x10]
		or		dword ptr [ecx+0x1C], eax
		mov		ecx, [ebp-8]
		mov		eax, [ecx]
		call	dword ptr [eax+0x28]
		push	1
		push	1
		push	dword ptr [ebp-0xC]
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x2F4]
	done:
		mov		s_moveAwayDistance, 0x43960000
		mov		eax, [ebp-0xC]
		leave
		retn	0xC
	}
}

__declspec(naked) void __fastcall Actor::TurnToFaceObject(TESObjectREFR *target)
{
	__asm
	{
		push	0
		push	dword ptr [edx+0x38]
		push	dword ptr [edx+0x34]
		push	dword ptr [edx+0x30]
		CALL_EAX(0x8BB520)
		retn
	}
}

__declspec(naked) void Actor::TurnAngle(float angle)
{
	__asm
	{
		push	0
		movss	xmm0, [esp+8]
		mulss	xmm0, kFltPId180
		addss	xmm0, [ecx+0x2C]
		push	ecx
		movss	[esp], xmm0
		CALL_EAX(0x8BB5C0)
		retn	4
	}
}

__declspec(naked) void Actor::PlayIdle(TESIdleForm *idleAnim)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 8
		mov		eax, [ecx]
		call	dword ptr [eax+0x1E4]
		test	eax, eax
		jz		jmpAddr1
		mov		[ebp-8], eax
		mov		ecx, eax
		CALL_EAX(0x4985F0)
		test	al, al
		setz	dl
		mov		[ebp-9], dl
		mov		ecx, [ebp-8]
		mov		ecx, [ecx+0x128]
		test	ecx, ecx
		jz		jmpAddr2
		mov		ecx, [ecx+0x2C]
		cmp		ecx, [ebp+8]
		jz		jmpAddr1
	jmpAddr2:
		cmp		byte ptr [ebp-9], 0
		jz		jmpAddr4
		mov		ecx, [ebp-8]
		mov		ecx, [ecx+0x124]
		test	ecx, ecx
		jz		jmpAddr4
		mov		eax, [ecx+0xC]
		cmp		eax, 3
		jnz		jmpAddr3
		mov		ecx, [ecx+0x18]
		test	ecx, ecx
		jz		jmpAddr4
		mov		ecx, [ecx+0x24]
		test	ecx, ecx
		jnz		jmpAddr4
	jmpAddr3:
		mov		[ebp-9], 0
	jmpAddr4:
		cmp		byte ptr [ebp-9], 0
		jnz		jmpAddr1
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		jmpAddr1
		push	dword ptr [ebp+8]
		mov		edx, [ecx]
		mov		edx, [edx+0x71C]
		call	edx
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x68]
		push	0x80
		mov		eax, [ecx]
		call	dword ptr [eax+0x614]
	jmpAddr1:
		leave
		retn	4
	}
}

__declspec(naked) UInt32 Actor::GetLevel()
{
	__asm
	{
		mov		ecx, [ecx+0x20]
		add		ecx, 0x30
		CALL_EAX(0x47DED0)
		movzx	eax, ax
		retn
	}
}

__declspec(naked) float Actor::GetKillXP()
{
	__asm
	{
		push	ecx
		mov		ecx, [ecx+0x20]
		add		ecx, 0x30
		CALL_EAX(0x47DED0)
		pop		ecx
		movzx	edx, ax
		push	edx
		cmp		byte ptr [ecx+4], kFormType_Creature
		setnz	al
		movzx	edx, al
		push	edx
		CALL_EAX(0x6705B0)
		add		esp, 8
		cvtsi2ss	xmm0, eax
		mov		eax, g_thePlayer
		push	dword ptr [eax+0x7B8]
		push	ecx
		movss	[esp], xmm0
		CALL_EAX(0x648C80)
		pop		ecx
		fstp	dword ptr [esp]
		CALL_EAX(0x406CE0)
		pop		ecx
		retn
	}
}

__declspec(naked) void Actor::DismemberLimb(UInt32 bodyPartID, bool explode)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		cmp		dword ptr [ebp+8], 0xD
		ja		done
		push	ecx
		push	ecx
		push	dword ptr [ebp+8]
		CALL_EAX(0x573090)
		test	al, al
		jnz		done
		mov		ecx, [ebp-4]
		CALL_EAX(0x87F3D0)
		test	al, al
		jnz		done
		push	0
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x1A0]
		test	al, al
		jnz		done
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x20]
		mov		eax, [ecx]
		call	dword ptr [eax+0x180]
		test	eax, eax
		jz		done
		mov		edx, [ebp+8]
		mov		eax, [eax+edx*4+0x34]
		test	eax, eax
		jz		done
		test	byte ptr [eax+0x60], 9
		jz		done
		movzx	eax, byte ptr [eax+0x63]
		mov		[ebp-8], eax
		mov		ecx, [ebp-4]
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 0
		jnz		done
		mov		ecx, [eax+edx*4+0x1DC]
		test	ecx, ecx
		jz		done
		push	1
		CALL_EAX(0x456630)
		test	al, al
		jnz		done
		movzx	eax, [ebp+0xC]
		push	eax
		push	dword ptr [ebp+8]
		push	1
		push	0
		push	dword ptr [ebp+8]
		mov		ecx, [ebp-4]
		CALL_EAX(0x572FC0)
		push	0
		push	1
		push	1
		movzx	eax, [ebp+0xC]
		push	eax
		push	dword ptr [ebp+8]
		push	dword ptr [ebp-8]
		mov		ecx, [ebp-4]
		CALL_EAX(0x8B52A0)
	done:
		leave
		retn	8
	}
}

bool Actor::HasNoPath()
{
	return actorMover && actorMover->pathingRequest && !actorMover->pathingSolution;
}

__declspec(naked) bool Actor::CanBePushed()
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 0
		jnz		retn0
		cmp		dword ptr [eax+0x140], 0
		jnz		retn0
		mov		eax, [eax+0x138]
		test	eax, eax
		jz		done
		mov		eax, [ecx+0x108]
		cmp		eax, 1
		jz		retn0
		cmp		eax, 2
		//jz		retn0

		setnz	al
		retn

		/*mov		eax, [ecx+0x20]
		test	dword ptr [eax+0x34], 0xC000000
		jnz		retn0
		mov		eax, [ecx]
		call	dword ptr [eax+0x4B4]
		not		al
		retn*/
	retn0:
		xor		al, al
	done:
		retn
	}
}

__declspec(naked) float Actor::AdjustPushForce(float baseForce)
{
	__asm
	{
		push	0xA
		add		ecx, 0xA4
		CALL_EAX(0x66EF50)
		fmul	kFlt10
		fmul	dword ptr ds:[0x11CEA6C]
		fadd	dword ptr ds:[0x11CE664]
		fld		dword ptr [esp+4]
		fmul	dword ptr ds:[0x11CFA20]
		fld		dword ptr ds:[0x11CF9C0]
		test	dword ptr [esp+4], 0x80000000
		jz		posForce
		fchs
	posForce:
		faddp	st(1), st
		fmulp	st(1), st
		retn	4
	}
}

__declspec(naked) void Actor::PushActor(float force, float angle, TESObjectREFR *originRef, bool adjustForce)
{
	static const float kFltFive = 5.0F, kPushTime = 96.0F;
	__asm
	{
		push	esi
		mov		esi, ecx
		call	Actor::CanBePushed
		test	al, al
		jz		done
		cmp		dword ptr [esp+8], 0
		jnz		hasForce
		mov		esi, [esi+0x68]
		mov		esi, [esi+0x138]
		pxor	xmm0, xmm0
		movaps	[esi+0x500], xmm0
		mov		dword ptr [esi+0x524], 0
		jmp		done
	hasForce:
		mov		eax, [esp+0x10]
		test	eax, eax
		jnz		useRef
		fld		dword ptr [esp+0xC]
		fmul	kDblPId180
		fsincos
		fstp	dword ptr [esp+0xC]
		movss	xmm1, [esp+0xC]
		fstp	dword ptr [esp+0xC]
		movss	xmm0, [esp+0xC]
		jmp		doneAngle
	useRef:
		movss	xmm0, [esi+0x30]
		subss	xmm0, [eax+0x30]
		movss	xmm1, [esi+0x34]
		subss	xmm1, [eax+0x34]
		movss	xmm2, xmm0
		mulss	xmm2, xmm2
		movss	xmm3, xmm1
		mulss	xmm3, xmm3
		addss	xmm2, xmm3
		sqrtss	xmm2, xmm2
		pxor	xmm3, xmm3
		comiss	xmm2, xmm3
		jz		done
		divss	xmm0, xmm2
		divss	xmm1, xmm2
	doneAngle:
		cmp		[esp+0x14], 0
		jz		doneForce
		push	dword ptr [esp+8] 
		mov		ecx, esi
		call	Actor::AdjustPushForce
		fstp	dword ptr [esp+8]
	doneForce:
		mov		edx, [esp+8]
		movd	xmm2, edx
		mulss	xmm2, kFltFive
		mulss	xmm2, ds:[0x1267BCC]
		and		edx, 0x7FFFFFFF
		movd	xmm3, edx
		divss	xmm3, kPushTime
		mulss	xmm2, xmm3
		mulss	xmm0, xmm2
		mulss	xmm1, xmm2
		pxor	xmm2, xmm2
		mov		esi, [esi+0x68]
		mov		esi, [esi+0x138]
		movss	[esi+0x500], xmm0
		movss	[esi+0x504], xmm1
		movq	qword ptr [esi+0x508], xmm2
		movss	[esi+0x524], xmm3
		or		dword ptr [esi+0x414], 0x80000000
	done:
		pop		esi
		retn	0x10
	}
}

__declspec(naked) int Actor::GetGroundMaterial()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x50
		push	esi
		mov		esi, ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x100]
		test	al, al
		jz		invalid
		mov		eax, [esi+0x68]
		test	eax, eax
		jz		invalid
		cmp		dword ptr [eax+0x28], 1
		ja		invalid
		mov		eax, [eax+0x138]
		test	eax, eax
		jz		invalid
		test	dword ptr [eax+0x3F0], 2
		jnz		invalid
		mov		eax, [eax+0x434]
		cmp		eax, 0x24
		jnz		convert
		mov		eax, [ecx+0x40]
		test	eax, eax
		jz		invalid
		mov		ecx, eax
		CALL_EAX(0x546FB0)
		test	eax, eax
		jz		invalid
		mov		ecx, eax
		push	1
		lea		edx, [esi+0x30]
		push	edx
		lea		edx, [ebp-0x50]
		push	edx
		mov		esi, ecx
		CALL_EAX(0x53B550)
		movzx	edx, word ptr [ebp-0x14]
		push	edx
		movzx	edx, byte ptr [ebp-0x38]
		push	edx
		mov		ecx, esi
		CALL_EAX(0x53A630)
		test	eax, eax
		jz		invalid
		movzx	eax, byte ptr [eax+0x1C]
	convert:
		cmp		eax, 0x1F
		ja		invalid
		mov		edx, eax
		movzx	eax, kMaterialConvert[edx]
		jmp		done
	invalid:
		mov		eax, 0xFFFFFFFF
	done:
		pop		esi
		leave
		retn
	}
}

void Projectile::GetData(UInt32 dataType, double *result)
{
	*result = 0;
	if (!IsProjectile()) return;
	switch (dataType)
	{
		case 0:
			if (sourceRef)
				*(UInt32*)result = sourceRef->refID;
			break;
		case 1:
			if (sourceWeap)
				*(UInt32*)result = sourceWeap->refID;
			break;
		case 2:
			*result = elapsedTime;
			break;
		case 3:
			*result = distTravelled;
			break;
		case 4:
			*result = hitDamage;
			break;
		case 5:
			*result = speedMult2;
			break;
		case 6:
		{
			if (hasImpacted)
			{
				ListNode<ImpactData> *traverse = impactDataList.Head();
				ImpactData *impactData;
				do
				{
					impactData = traverse->data;
					if (impactData && impactData->refr)
					{
						*(UInt32*)result = impactData->refr->refID;
						break;
					}
				}
				while (traverse = traverse->next);
			}
			break;
		}
		case 7:
		{
			if (hasImpacted)
			{
				ImpactData *impactData = impactDataList.GetFirstItem();
				if (impactData && (impactData->materialType <= 31))
					*result = (impactData && (impactData->materialType <= 31)) ? kMaterialConvert[impactData->materialType] : -1;
			}
			break;
		}
		default: break;
	}
}

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

UnorderedMap<TESNPC*, AppearanceUndo*> s_appearanceUndoMap;

__declspec(naked) void __fastcall TESNPC::SetSex(UInt32 flags)
{
	__asm
	{
		test	dword ptr [ecx+0x34], 1
		setnz	al
		test	edx, 1
		setnz	dl
		cmp		al, dl
		jz		done
		push	1
		movzx	eax, dl
		push	eax
		push	1
		add		ecx, 0x30
		CALL_EAX(0x47DD50)
	done:
		retn
	}
}

__declspec(naked) void __fastcall TESNPC::SetRace(TESRace *pRace)
{
	__asm
	{
		cmp		[ecx+0x110], edx
		jz		done
		mov		eax, g_thePlayer
		cmp		[eax+0x20], ecx
		jnz		notPlayer
		push	0
		push	edx
		CALL_EAX(0x60B240)
		retn
	notPlayer:
		mov		[ecx+0x110], edx
	done:
		retn
	}
}

char PlayerCharacter::GetDetectionState()
{
	if (!parentCell) return -1;
	if (pcUnseen || byte5F8) return 1;	// CAUTION
	if (pcInCombat) return 3;			// DANGER
	if (g_processManager->GetTotalDetectionValue(this) <= 0)
		return 0;						// HIDDEN
	return 2;							// DETECTED
}

bool s_queuedPlayerLock = false;

__declspec(naked) void PlayerCharacter::UpdatePlayer3D()
{
	//	s_queuedPlayerLock is reset by DoQueuedPlayerHook()
	__asm
	{
		cmp		s_queuedPlayerLock, 0
		jnz		done
		mov		s_queuedPlayerLock, 1
		push	0
		push	1
		push	ecx
		push	1
		push	1
		push	dword ptr ds:[0x11CDD7C]
		mov		ecx, g_modelLoader
		CALL_EAX(0x445300)
		mov		ecx, g_modelLoader
		CALL_EAX(0x444850)
	done:
		retn
	}
}

__declspec(naked) NiNode* __fastcall TESObjectCELL::Get3DNode(UInt32 index)
{
	__asm
	{
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		cmp		[eax+0xA6], dx
		ja		getChild
		xor		eax, eax
		retn
	getChild:
		mov		ecx, [eax+0xA0]
		mov		eax, [ecx+edx*4]
	done:
		retn
	}
}

void TESObjectCELL::ToggleNodes(UInt32 nodeBits, UInt8 doHide)
{
	if (!renderData || !renderData->masterNode) return;
	for (auto childIter = renderData->masterNode->m_children.Begin(); !childIter.End(); ++childIter)
	{
		if (!*childIter) break;
		if ((nodeBits & 1) && ((childIter->m_flags & 1) != doHide))
			childIter->m_flags ^= 1;
		if (!(nodeBits >>= 1)) break;
	}
}

__declspec(naked) TESWorldSpace *TESWorldSpace::GetRootMapWorld()
{
	__asm
	{
	iterHead:
		mov		eax, ecx
		mov		ecx, [ecx+0x70]
		test	ecx, ecx
		jz		done
		test	byte ptr [eax+0x4E], 4
		jnz		iterHead
	done:
		retn
	}
}

BGSQuestObjective *TESQuest::GetObjective(UInt32 objectiveID)
{
	ListNode<void> *iter = lVarOrObjectives.Head();
	BGSQuestObjective *objective;
	do
	{
		objective = (BGSQuestObjective*)iter->data;
		if (objective && IS_TYPE(objective, BGSQuestObjective) && (objective->objectiveId == objectiveID))
			return objective;
	}
	while (iter = iter->next);
	return NULL;
}

SInt32 BGSQuestObjective::GetTargetIndex(TESObjectREFR *refr)
{
	SInt32 index = 0;
	ListNode<Target> *iter = targets.Head();
	do
	{
		if (iter->data && (iter->data->target == refr))
			return index;
		index++;
	}
	while (iter = iter->next);
	return -1;
}

void TESActorBaseData::SetFactionRank(TESFaction *faction, char rank)
{
	ListNode<FactionListData> *iter = factionList.Head(), *prev = NULL;
	FactionListData *data;
	do
	{
		data = iter->data;
		if (data && (data->faction == faction))
		{
			if (rank >= 0) data->rank = rank;
			else
			{
				GameHeapFree(data);
				if (prev) prev->RemoveNext();
				else iter->RemoveMe();
			}
			return;
		}
		prev = iter;
	}
	while (iter = iter->next);
	if (rank >= 0)
	{
		data = (FactionListData*)GameHeapAlloc(sizeof(FactionListData));
		data->faction = faction;
		data->rank = rank;
		factionList.Prepend(data);
	}
}

UnorderedSet<TESGlobal*> s_resolvedGlobals;

__declspec(naked) UInt32 TESGlobal::ResolveRefValue()
{
	__asm
	{
		mov		word ptr [ecx+6], 1
		push	esi
		lea		esi, [ecx+0x24]
		cmp		byte ptr [esi], 0xFF
		jz		doLookup
		push	esi
		push	dword ptr [esi]
		call	ResolveRefID
		add		esp, 8
		movzx	eax, al
		test	eax, eax
		jz		invalid
	doLookup:
		push	dword ptr [esi]
		call	LookupFormByRefID
		test	eax, eax
		jz		invalid
		mov		eax, [esi]
		pop		esi
		retn
	invalid:
		mov		[esi], eax
		pop		esi
		retn
	}
}

__declspec(naked) NiExtraData* __fastcall NiObjectNET::GetExtraData(UInt32 vtbl)
{
	__asm
	{
		push	esi
		mov		esi, [ecx+0x10]
		movzx	ecx, word ptr [ecx+0x14]
		ALIGN 16
	iterHead:
		dec		ecx
		js		retnNULL
		mov		eax, [esi+ecx*4]
		test	eax, eax
		jz		iterHead
		cmp		[eax], edx
		jnz		iterHead
		pop		esi
		retn
	retnNULL:
		xor		eax, eax
		pop		esi
		retn
	}
}

__declspec(naked) NiNode* __stdcall NiNode::Create(const char *nodeName)
{
	__asm
	{
		push	0xAC
		CALL_EAX(0xAA13E0)
		pop		ecx
		push	0
		mov		ecx, eax
		CALL_EAX(0xA5ECB0)
		push	eax
		push	dword ptr [esp+8]
		CALL_EAX(0xA5B690)
		pop		ecx
		pop		ecx
		mov		[ecx+8], eax
		mov		eax, ecx
		retn	4
	}
}

NiObjectCopyInfo s_NiObjectCopyInfo;
void NiTMapBase<int, int>::FreeBuckets();

__declspec(naked) NiNode *NiNode::CreateCopy()
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		eax, offset s_NiObjectCopyInfo
		push	eax
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x48]
		mov		ecx, esi
		mov		esi, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x68]
		mov		ecx, ds:[s_NiObjectCopyInfo.map00]
		call	NiTMapBase<int, int>::FreeBuckets
		mov		ecx, ds:[s_NiObjectCopyInfo.map04]
		call	NiTMapBase<int, int>::FreeBuckets
		and		byte ptr [esi+0x30], 0xFE
		mov		edx, 0x3F800000
		movd	xmm0, edx
		movups	[esi+0x34], xmm0
		movups	[esi+0x44], xmm0
		movups	[esi+0x54], xmm0
		mov		[esi+0x64], edx
		mov		eax, esi
		pop		esi
		retn
	}
}

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlock(const char *blockName)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		push	edx
		CALL_EAX(0xA5B690)
		pop		ecx
		push	eax
		push	esp
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x9C]
		mov		esi, eax
		push	esp
		CALL_EAX(0x4381D0)
		add		esp, 8
		mov		eax, esi
		pop		esi
		retn
	}
}

__declspec(naked) NiNode* __fastcall NiNode::GetNode(const char *nodeName)
{
	__asm
	{
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) void NiMatrix33::RotationMatrix(float rotX, float rotY, float rotZ)
{
	__asm
	{
		push	ecx
		fld		dword ptr [esp+8]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm1, [esp]
		fstp	dword ptr [esp]
		movss	xmm0, [esp]
		fld		dword ptr [esp+0xC]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm3, [esp]
		fstp	dword ptr [esp]
		movss	xmm2, [esp]
		fld		dword ptr [esp+0x10]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm5, [esp]
		fstp	dword ptr [esp]
		movss	xmm4, [esp]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss	[ecx], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss	[ecx+4], xmm6
		pxor	xmm6, xmm6
		subss	xmm6, xmm2
		movss	[ecx+8], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movss	[ecx+0xC], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movss	[ecx+0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movss	[ecx+0x14], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movss	[ecx+0x18], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movss	[ecx+0x1C], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm3
		movss	[ecx+0x20], xmm6
		pop		ecx
		retn	0xC
	}
}

__declspec(naked) void NiMatrix33::Rotate(float rotX, float rotY, float rotZ)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 0x48
		lea		eax, [ebp-0x28]
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		movups	xmm0, [ecx+0x10]
		movups	[eax+0x10], xmm0
		mov		edx, [ecx+0x20]
		mov		[eax+0x20], edx
		push	dword ptr [ebp+0x10]
		push	dword ptr [ebp+0xC]
		push	dword ptr [ebp+8]
		lea		ecx, [ebp-0x4C]
		call	NiMatrix33::RotationMatrix
		lea		eax, [ebp-0x4C]
		push	eax
		lea		eax, [ebp-0x28]
		push	eax
		mov		ecx, [ebp-4]
		call	NiMatrix33::MultiplyMatrices
		leave
		retn	0xC
	}
}

__declspec(naked) void NiMatrix33::MultiplyMatrices(NiMatrix33 &matA, NiMatrix33 &matB)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [esp+8]
		movss	xmm0, [eax]
		mulss	xmm0, [edx]
		movss	xmm1, [eax+4]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movss	[ecx], xmm0
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [edx]
		movss	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movss	[ecx+0xC], xmm0
		movss	xmm0, [eax+0x18]
		mulss	xmm0, [edx]
		movss	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movss	[ecx+0x18], xmm0
		movss	xmm0, [eax]
		mulss	xmm0, [edx+4]
		movss	xmm1, [eax+4]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movss	[ecx+4], xmm0
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [edx+4]
		movss	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movss	[ecx+0x10], xmm0
		movss	xmm0, [eax+0x18]
		mulss	xmm0, [edx+4]
		movss	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movss	[ecx+0x1C], xmm0
		movss	xmm0, [eax]
		mulss	xmm0, [edx+8]
		movss	xmm1, [eax+4]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movss	[ecx+8], xmm0
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [edx+8]
		movss	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movss	[ecx+0x14], xmm0
		movss	xmm0, [eax+0x18]
		mulss	xmm0, [edx+8]
		movss	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movss	[ecx+0x20], xmm0
		retn	8
	}
}

void NiVector3::ToQuaternion(NiQuaternion &quaternion)
{
	double hlf = (double)z * 0.5;
	double cY = cos(hlf);
	double sY = sin(hlf);
	hlf = (double)y * 0.5;
	double cP = cos(hlf);
	double sP = sin(hlf);
	hlf = (double)x * 0.5;
	double cR = cos(hlf);
	double sR = sin(hlf);
	quaternion.w = cY * cP * cR + sY * sP * sR;
	quaternion.x = cY * cP * sR - sY * sP * cR;
	quaternion.y = cY * sP * cR + sY * cP * sR;
	quaternion.z = sY * cP * cR - cY * sP * sR;
}

__declspec(naked) void NiVector3::MultiplyMatrixVector(NiMatrix33 &mat, NiVector3 &vec)
{
	__asm
    {
        mov		eax, [esp+4]
        mov		edx, [esp+8]
		movss	xmm0, [edx]
		movss	xmm1, [edx+4]
		movss	xmm2, [edx+8]
		movss	xmm3, [eax]
        mulss	xmm3, xmm0
		movss	xmm4, [eax+4]
        mulss	xmm4, xmm1
        addss	xmm3, xmm4
		movss	xmm4, [eax+8]
        mulss	xmm4, xmm2
        addss	xmm3, xmm4
		movss	[ecx], xmm3
		movss	xmm3, [eax+0xC]
        mulss	xmm3, xmm0
		movss	xmm4, [eax+0x10]
        mulss	xmm4, xmm1
        addss	xmm3, xmm4
		movss	xmm4, [eax+0x14]
        mulss	xmm4, xmm2
        addss	xmm3, xmm4
		movss	[ecx+4], xmm3
		movss	xmm3, [eax+0x18]
        mulss	xmm3, xmm0
		movss	xmm4, [eax+0x1C]
        mulss	xmm4, xmm1
        addss	xmm3, xmm4
		movss	xmm4, [eax+0x20]
        mulss	xmm4, xmm2
        addss	xmm3, xmm4
		movss	[ecx+8], xmm3
        retn	8
    }
}

__declspec(naked) float __vectorcall Vector3Length(AlignedVector4 *inVec)
{
    __asm
    {
		movaps	xmm0, [ecx]
		mulps	xmm0, xmm0
		movhlps	xmm1, xmm0
		addss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
        retn
    }
}

__declspec(naked) float __vectorcall Vector3Distance(NiVector3 *vec1, NiVector3 *vec2)
{
	__asm
	{
		movss	xmm0, [ecx+8]
		movlhps	xmm0, xmm0
		movlps	xmm0, [ecx]
		movss	xmm1, [edx+8]
		movlhps	xmm1, xmm1
		movlps	xmm1, [edx]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		movhlps	xmm1, xmm0
		addss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
		retn
	}
}

__declspec(naked) void NiVector3::Normalize()
{
    __asm
    {
		movss	xmm0, [ecx+8]
		movlhps	xmm0, xmm0
		movlps	xmm0, [ecx]
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		movhlps	xmm2, xmm1
		addss	xmm2, xmm1
		psrlq	xmm1, 0x20
		addss	xmm2, xmm1
		sqrtss	xmm1, xmm2
		pxor	xmm2, xmm2
		comiss	xmm1, xmm2
        jz		zeroLen
		shufps	xmm1, xmm1, 0
		divps	xmm0, xmm1
		movhlps	xmm1, xmm0
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		retn
	zeroLen:
        movq	qword ptr [ecx], xmm2
        movss	[ecx+8], xmm2
        retn
    }
}

void NiQuaternion::EulerYPR(NiVector3 &ypr)
{
	ypr.x = atan2(2.0 * (w * x + y * z), 1.0 - (2.0 * (x * x + y * y)));
	double sinp = 2.0 * (w * y - z * x);
	if (fabs(sinp) < 1)
		ypr.y = asin(sinp);
	else
		ypr.y = (sinp > 0) ? kDblPId2 : -kDblPId2;
	ypr.z = atan2(2.0 * (w * z + x * y), 1.0 - (2.0 * (y * y + z * z)));
}

void NiQuaternion::RotationMatrix(NiMatrix33 &rotMatrix)
{
	float x2 = x * x, y2 = y * y, z2 = z * z;
	rotMatrix.cr[0][0] = 1 - 2 * (y2 + z2);
	rotMatrix.cr[0][1] = (2 * x * y) + (2 * z * w);
	rotMatrix.cr[0][2] = (2 * x * z) - (2 * y * w);
	rotMatrix.cr[1][0] = (2 * x * y) - (2 * z * w);
	rotMatrix.cr[1][1] = 1 - 2 * (x2 + z2);
	rotMatrix.cr[1][2] = (2 * y * z) + (2 * x * w);
	rotMatrix.cr[2][0] = (2 * x * z) + (2 * y * w);
	rotMatrix.cr[2][1] = (2 * y * z) - (2 * x * w);
	rotMatrix.cr[2][2] = 1 - 2 * (x2 + y2);
}

bool NiNode::IsMovable()
{
	if (m_collisionObject)
	{
		bhkWorldObject *hWorldObj = m_collisionObject->worldObj;
		if (hWorldObj)
		{
			UInt8 motionType = ((hkpRigidBody*)hWorldObj->refObject)->motion.type;
			if ((motionType == 2) || (motionType == 3) || (motionType == 6))
				return true;
		}
	}
	for (auto iter = m_children.Begin(); !iter.End(); ++iter)
		if (*iter && iter->GetNiNode() && ((NiNode*)*iter)->IsMovable())
			return true;
	return false;
}

__declspec(naked) void NiNode::ToggleCollision(bool enable)
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		noColObj
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		noColObj
		mov		eax, [eax+8]
		test	eax, eax
		jz		noColObj
		cmp		[esp+4], 0
		jz		doDisable
		and		byte ptr [eax+0x2D], 0xBF
		jmp		noColObj
	doDisable:
		or		byte ptr [eax+0x2D], 0x40
	noColObj:
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		mov		ecx, [esi]
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		test	eax, eax
		jz		iterNext
		push	dword ptr [esp+0xC]
		mov		ecx, eax
		call	NiNode::ToggleCollision
		ALIGN 16
	iterNext:
		add		esi, 4
		dec		edi
		jnz		iterHead
		pop		edi
		pop		esi
	done:
		retn	4
	}
}

__declspec(naked) void NiNode::RemoveCollision()
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		noColObj
		push	ecx
		mov		ecx, eax
		call	NiReleaseObject
		pop		ecx
		mov		dword ptr [ecx+0x1C], 0
	noColObj:
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		mov		ecx, [esi]
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		test	eax, eax
		jz		iterNext
		mov		ecx, eax
		call	NiNode::RemoveCollision
		ALIGN 16
	iterNext:
		add		esi, 4
		dec		edi
		jnz		iterHead
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) UInt32 NiAVObject::GetIndex()
{
	__asm
	{
		push	esi
		mov		esi, ecx
		xor		eax, eax
		mov		ecx, [ecx+0x18]
		test	ecx, ecx
		jz		done
		movzx	edx, word ptr [ecx+0xA6]
		test	edx, edx
		jz		done
		mov		ecx, [ecx+0xA0]
		ALIGN 16
	iterHead:
		inc		eax
		cmp		[ecx], esi
		jz		done
		add		ecx, 4
		dec		edx
		jnz		iterHead
		xor		eax, eax
	done:
		pop		esi
		retn
	}
}

NiProperty *NiAVObject::GetProperty(UInt32 propID)
{
	NiProperty *niProp;
	for (DListNode<NiProperty> *propNode = m_propertyList.Head(); propNode; propNode = propNode->next)
	{
		niProp = propNode->data;
		if (niProp && (niProp->GetPropertyType() == propID))
			return niProp;
	}
	return NULL;
}

__declspec(naked) void __fastcall NiAVObject::SetName(const char *newName)
{
	__asm
	{
		push	ecx
		push	edx
		CALL_EAX(0xA5B690)
		pop		ecx
		pop		ecx
		push	eax
		push	esp
		CALL_EAX(0xA5B950)
		pop		ecx
		retn
	}
}

void NiMaterialProperty::SetTraitValue(UInt32 traitID, float value)
{
	switch (traitID)
	{
		case 0:
		case 1:
		case 2:
			specularRGB[traitID] = value;
			break;
		case 3:
		case 4:
		case 5:
			emissiveRGB[traitID - 3] = value;
			break;
		case 6:
			glossiness = value;
			break;
		case 7:
			alpha = value;
			break;
		default:
			emitMult = value;
	}
}

void NiNode::BulkSetMaterialPropertyTraitValue(UInt32 traitID, float value)
{
	NiAVObject *block;
	NiMaterialProperty *matProp;
	for (auto iter = m_children.Begin(); !iter.End(); ++iter)
	{
		if (!(block = *iter)) continue;
		if (block->GetNiNode())
			((NiNode*)block)->BulkSetMaterialPropertyTraitValue(traitID, value);
		else if (matProp = (NiMaterialProperty*)block->GetProperty(2))
			matProp->SetTraitValue(traitID, value);
	}
}

void NiNode::GetContactObjects(ContactObjects *contactObjs)
{
	if (m_collisionObject && m_collisionObject->worldObj)
	{
		hkpWorldObject *hWorldObj = (hkpWorldObject*)m_collisionObject->worldObj->refObject;
		if (hWorldObj->collisionType == 1)
		{
			hkpRigidBody *rigidBody = (hkpRigidBody*)hWorldObj;
			ConstraintContact *contactsArr = rigidBody->contactsArr;
			if (contactsArr)
				for (UInt16 count = rigidBody->contactsSize; count; count--, contactsArr++)
					contactObjs->Append(contactsArr->contactBody);
			if (rigidBody->constraintInst.data)
				for (auto iter = rigidBody->constraintInst.Begin(); !iter.End(); ++iter)
					contactObjs->Append(iter->contactBody);
		}
		else if IS_TYPE(hWorldObj, hkpSimpleShapePhantom)
			for (auto iter = ((hkpSimpleShapePhantom*)hWorldObj)->cdBodies.Begin(); !iter.End(); ++iter)
				contactObjs->Append(iter->GetWorldObj());
	}
	for (auto iter = m_children.Begin(); !iter.End(); ++iter)
		if (*iter && iter->GetNiNode()) ((NiNode*)*iter)->GetContactObjects(contactObjs);
}

bool NiNode::HasPhantom()
{
	if (m_collisionObject && m_collisionObject->worldObj && (((hkpWorldObject*)m_collisionObject->worldObj->refObject)->collisionType == 2))
		return true;
	for (auto iter = m_children.Begin(); !iter.End(); ++iter)
		if (*iter && iter->GetNiNode() && ((NiNode*)*iter)->HasPhantom())
			return true;
	return false;
}

NiTransformInterpolator *NiTransformInterpolator::Create()
{
	NiTransformInterpolator *interpolator = (NiTransformInterpolator*)NiAllocator(sizeof(NiTransformInterpolator));
	ThisCall(0xA3FA30, interpolator, 0);
	return interpolator;
}

NiTransformController *NiTransformController::Create(NiNode *pTarget, NiTransformInterpolator *pInterpolator)
{
	NiTransformController *controller = (NiTransformController*)NiAllocator(sizeof(NiTransformController));
	ThisCall(0xA36560, controller);
	controller->SetTarget(pTarget);
	controller->SetInterpolator(pInterpolator, 0);
	return controller;
}

void NiNode::GetBodyMass(float *totalMass)
{
	if (m_collisionObject && m_collisionObject->worldObj)
		*totalMass += ((hkpRigidBody*)m_collisionObject->worldObj->refObject)->motion.GetBodyMass();
	for (auto iter = m_children.Begin(); !iter.End(); ++iter)
		if (*iter && iter->GetNiNode()) ((NiNode*)*iter)->GetBodyMass(totalMass);
}

__declspec(naked) void NiNode::ApplyForce(hkVector4 *forceVector)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		doneCol
		mov		ecx, [eax+0x10]
		test	ecx, ecx
		jz		doneCol
		mov		eax, [ecx+8]
		mov		al, [eax+0xE8]
		cmp		al, 2
		jz		doApply
		cmp		al, 3
		jz		doApply
		cmp		al, 6
		jnz		doneCol
	doApply:
		push	dword ptr [esp+8]
		call	bhkWorldObject::ApplyForce
	doneCol:
		movzx	edx, word ptr [ebx+0xA6]
		test	edx, edx
		jz		done
		push	esi
		push	edi
		mov		esi, [ebx+0xA0]
		mov		edi, edx
		ALIGN 16
	iterHead:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		iterNext
		mov		eax, [ecx]
		call	dword ptr [eax+0xC]
		test	eax, eax
		jz		iterNext
		push	dword ptr [esp+0x10]
		mov		ecx, eax
		call	NiNode::ApplyForce
		ALIGN 16
	iterNext:
		add		esi, 4
		dec		edi
		jnz		iterHead
		pop		edi
		pop		esi
	done:
		pop		ebx
		retn	4
	}
}

__declspec(naked) void bhkWorldObject::ApplyForce(hkVector4 *forceVector)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0x20
		mov		[esp], ecx
		lea		eax, [esp+0x10]
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xF0]
		push	eax
		lea		eax, [esp+8]
		push	eax
		CALL_EAX(0x458620)
		add		esp, 8
		mov		eax, [ebp+8]
		lea		ecx, [esp+4]
		movss	xmm0, [ecx]
		subss	xmm0, [eax]
		movss	[ecx], xmm0
		movss	xmm0, [ecx+4]
		subss	xmm0, [eax+4]
		movss	[ecx+4], xmm0
		movss	xmm0, [ecx+8]
		subss	xmm0, [eax+8]
		movss	[ecx+8], xmm0
		call	NiVector3::Normalize
		mov		ecx, [esp]
		mov		ecx, [ecx+8]
		add		ecx, 0xE0
		CALL_EAX(0xC86430)
		fstp	dword ptr [esp+0x1C]
		mov		eax, [ebp+8]
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [esp+0x1C]
		lea		eax, [esp+4]
		movss	xmm1, [eax]
		mulss	xmm1, xmm0
		movss	[eax], xmm1
		movss	xmm1, [eax+4]
		mulss	xmm1, xmm0
		movss	[eax+4], xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, xmm0
		movss	[eax+8], xmm1
		push	eax
		mov		ecx, [esp+4]
		CALL_EAX(0x9B0BF0)
		leave
		retn	4
	}
}

hkpWorld *GethkpWorld()
{
	if (!g_thePlayer) return NULL;
	bhkWorld *hkWorld = NULL;
	TESObjectCELL *cell = g_thePlayer->parentCell;
	if (cell)
	{
		ExtraHavok *xHavok = GetExtraType(&cell->extraDataList, Havok);
		if (xHavok) hkWorld = xHavok->world;
	}
	if (!hkWorld) hkWorld = *g_bhkWorldM;
	return hkWorld ? hkWorld->GetWorld() : NULL;
}

__declspec(naked) LODdata::LODNode *LODdata::LODNode::GetNodeByCoord(UInt32 coord)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 8
		xor		edx, edx
		mov		[ebp-8], edx
		mov		eax, [ecx]
		mov		eax, [eax+0x24]
		cmp		[ecx+4], eax
		jb		done
		mov		[ebp-0xC], edx
		jnz		iterHead
		mov		eax, [ecx+8]
		cmp		[ebp+8], eax
		jnz		iterHead
		mov		[ebp-8], ecx
		jmp		done
		ALIGN 16
	iterHead:
		mov		ecx, [ecx+edx*4+0x20]
		test	ecx, ecx
		jz		iterNext
		push	dword ptr [ebp+8]
		call	LODdata::LODNode::GetNodeByCoord
		mov		[ebp-8], eax
		test	eax, eax
		jnz		done
		mov		edx, [ebp-0xC]
	iterNext:
		mov		ecx, [ebp-4]
		inc		edx
		mov		[ebp-0xC], edx
		cmp		dl, 4
		jb		iterHead
	done:
		mov		eax, [ebp-8]
		leave
		retn	4
	}
}

ExtraDataList *ExtraDataList::CreateCopy()
{
	ExtraDataList *xDataList = Create();
	CopyExtraDataList(xDataList, this);
	return xDataList;
}

__declspec(naked) float ExtraContainerChanges::Data::GetInventoryWeight()
{
	__asm
	{
		mov		dword ptr [ecx+8], 0xBF800000
		mov		eax, g_thePlayer
		movzx	eax, byte ptr [eax+0x7BC]
		push	eax
		mov		byte ptr ds:[0x11E0898], 1
		CALL_EAX(0x4D0900)
		mov		byte ptr ds:[0x11E0898], 0
		retn
	}
}

__declspec(naked) bool __fastcall ExtraContainerChanges::EntryData::HasExtraType(UInt8 xType)
{
	__asm
	{
		mov		eax, [ecx]
		test	eax, eax
		jz		done
		push	esi
		mov		esi, eax
		mov		cl, dl
		movzx	eax, cl
		shr		al, 3
		add		al, 8
		and		cl, 7
		mov		dl, 1
		shl		dl, cl
		ALIGN 16
	iterHead:
		mov		ecx, [esi]
		test	ecx, ecx
		jz		iterNext
		test	byte ptr [ecx+eax], dl
		jnz		retnTrue
	iterNext:
		mov		esi, [esi+4]
		test	esi, esi
		jnz		iterHead
		xor		al, al
		pop		esi
		retn
	retnTrue:
		mov		al, 1
		pop		esi
	done:
		retn
	}
}

__declspec(naked) bool ExtraContainerChanges::EntryData::HasExtraLeveledItem()
{
	__asm
	{
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		retnFalse
		ALIGN 16
	iterHead:
		mov		edx, [ecx]
		test	edx, edx
		jz		iterNext
		test	byte ptr [edx+0xD], 0x80
		jnz		retnTrue
	iterNext:
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		iterHead
	retnFalse:
		xor		al, al
		retn
	retnTrue:
		mov		al, 1
		retn
	}
}

void ExtraContainerChanges::EntryData::RemoveCannotWear()
{
	if (!extendData) return;
	ListNode<ExtraDataList> *xdlIter = extendData->Head(), *prev = NULL;
	ExtraDataList *xData;
	do
	{
		xData = xdlIter->data;
		if (xData)
		{
			RemoveExtraType(xData, kExtraData_CannotWear);
			if (!xData->m_data)
			{
				GameHeapFree(xData);
				xdlIter = prev ? prev->RemoveNext() : xdlIter->RemoveMe();
				continue;
			}
		}
		prev = xdlIter;
		xdlIter = xdlIter->next;
	}
	while (xdlIter);
}

ExtraDataList *ExtraContainerChanges::EntryData::GetEquippedExtra()
{
	if (extendData)
	{
		ExtraDataList *xData;
		ListNode<ExtraDataList> *xdlIter = extendData->Head();
		do
		{
			xData = xdlIter->data;
			if (xData && xData->HasType(kExtraData_Worn))
				return xData;
		}
		while (xdlIter = xdlIter->next);
	}
	return NULL;
}

__declspec(naked) float ExtraContainerChanges::EntryData::CalculateWeaponDamage(Actor *owner, float condition, TESForm *ammo)
{
	static const double kSplitBeamMult = 1.3;
	__asm
	{
		push	esi
		mov		esi, ecx
		cmp		dword ptr [esp+0x10], 0
		setnz	dl
		movzx	edx, dl
		push	edx
		push	ecx
		push	0
		push	0
		push	0x3F800000
		push	dword ptr [esp+0x20]
		push	dword ptr [ecx+8]
		mov		ecx, [esp+0x24]
		add		ecx, 0xA4
		push	ecx
		CALL_EAX(0x644CE0)
		add		esp, 0x20
		mov		ecx, [esp+0x10]
		test	ecx, ecx
		jz		noAmmo
		add		ecx, 0xD4
		cmp		dword ptr [ecx], 0
		jz		noAmmo
		push	ecx
		fstp	dword ptr [esp]
		push	ecx
		push	kAmmoEffect_DamageMod
		CALL_EAX(0x59A030)
		add		esp, 0xC
	noAmmo:
		push	0xC
		mov		ecx, esi
		CALL_EAX(0x4BDA70)
		test	al, al
		jz		noMod
		fmul	kSplitBeamMult
	noMod:
		mov		ecx, [esp+8]
		cmp		dword ptr [ecx+0xC], 0x14
		jz		perkMod
		cmp		byte ptr [ecx+0x18D], 0
		jz		done
	perkMod:
		push	ecx
		fstp	dword ptr [esp]
		push	esp
		push	ecx
		push	dword ptr [esi+8]
		push	ecx
		push	kPerkEntry_CalculateWeaponDamage
		CALL_EAX(0x5E58F0)
		add		esp, 0x14
		fld		dword ptr [esp]
		pop		ecx
	done:
		pop		esi
		retn	0xC
	}
}

void ExtraContainerChanges::ExtendDataList::Clear()
{
	ListNode<ExtraDataList> *xdlIter = Head();
	ExtraDataList *xData;
	do
	{
		xData = xdlIter->data;
		if (xData)
		{
			ClearExtraDataList(xData, true);
			GameHeapFree(xData);
		}
	}
	while (xdlIter = xdlIter->next);
	RemoveAll();
}

ExtraDataList *ExtraContainerChanges::ExtendDataList::RemoveByType(ExtraDataList *xDataList, UInt32 type)
{
	ListNode<ExtraDataList> *xdlIter = Head(), *prev = NULL;
	do
	{
		if (xdlIter->data == xDataList)
		{
			RemoveExtraType(xDataList, type);
			if (xDataList->m_data) return xDataList;
			GameHeapFree(xDataList);
			prev ? prev->RemoveNext() : xdlIter->RemoveMe();
			break;
		}
		prev = xdlIter;
	}
	while (xdlIter = xdlIter->next);
	return NULL;
}

void ExtraContainerChanges::ExtendDataList::CleanEmpty()
{
	ListNode<ExtraDataList> *xdlIter = Head(), *prev = NULL;
	ExtraDataList *xDataList;
	do
	{
		xDataList = xdlIter->data;
		if (xDataList && !xDataList->m_data)
		{
			GameHeapFree(xDataList);
			xdlIter = prev ? prev->RemoveNext() : xdlIter->RemoveMe();
		}
		else
		{
			prev = xdlIter;
			xdlIter = xdlIter->next;
		}
	}
	while (xdlIter);
}

void MagicTarget::RemoveEffect(EffectItem *effItem)
{
	ActiveEffectList *effList = GetEffectList();
	if (!effList) return;
	ListNode<ActiveEffect> *iter = effList->Head(), *prev = NULL;
	ActiveEffect *activeEff;
	do
	{
		activeEff = iter->data;
		if (activeEff && (activeEff->effectItem == effItem))
		{
			activeEff->Remove(true);
			if (prev) iter = prev->RemoveNext();
			else iter->RemoveMe();
		}
		else
		{
			prev = iter;
			iter = iter->next;
		}
	}
	while (iter);
}

UnorderedSet<void*> s_effectItemFree;

bool EffectItemList::RemoveNthEffect(UInt32 index)
{
	if (!list.GetNthItem(1)) return false;
	ListNode<EffectItem> *iter = list.Head(), *prev = NULL;
	do
	{
		if (index)
		{
			index--;
			prev = iter;
			continue;
		}
		EffectItem *effItem = iter->data;
		if (effItem)
		{
			s_effectItemFree.Insert(effItem);
			g_thePlayer->magicTarget.RemoveEffect(effItem);
			MobileObject **objArray = g_processManager->objects.data, **arrEnd = objArray;
			objArray += g_processManager->beginOffsets[0];
			arrEnd += g_processManager->endOffsets[0];
			Actor *actor;
			for (; objArray != arrEnd; objArray++)
			{
				actor = (Actor*)*objArray;
				if (actor && actor->IsActor())
					actor->magicTarget.RemoveEffect(effItem);
			}
			objArray = g_processManager->objects.data;
			arrEnd = objArray + g_processManager->endOffsets[1];
			objArray += g_processManager->beginOffsets[1];
			for (; objArray != arrEnd; objArray++)
			{
				actor = (Actor*)*objArray;
				if (actor && actor->IsActor())
					actor->magicTarget.RemoveEffect(effItem);
			}
		}
		if (prev) prev->RemoveNext();
		else iter->RemoveMe();
		return true;
	}
	while (iter = iter->next);
	return false;
}

void Actor::UpdateActiveEffects(MagicItem *magicItem, EffectItem *effItem, ActiveEffectCreate callback, bool addNew)
{
	ActiveEffectList *effList = magicTarget.GetEffectList();
	if (!effList) return;
	ActiveEffect *activeEff, *newEff;
	ListNode<ActiveEffect> *iter = effList->Head();
	do
	{
		activeEff = iter->data;
		if (activeEff && (activeEff->magicItem == magicItem) && (addNew || (activeEff->effectItem == effItem)))
		{
			newEff = callback(activeEff->caster, magicItem, effItem);
			if (newEff)
			{
				newEff->target = activeEff->target;
				if (addNew) effList->Prepend(newEff);
				else
				{
					activeEff->Remove(true);
					iter->data = newEff;
				}
			}
			break;
		}
	}
	while (iter = iter->next);
}

void __fastcall UpdateEffectsAllActors(MagicItem *magicItem, EffectItem *effItem, bool addNew = false)
{
	ActiveEffectCreate callback = g_effectArchTypeArray[effItem->setting->archtype].callback;
	if (!callback) return;
	g_thePlayer->UpdateActiveEffects(magicItem, effItem, callback, addNew);
	MobileObject **objArray = g_processManager->objects.data, **arrEnd = objArray;
	objArray += g_processManager->beginOffsets[0];
	arrEnd += g_processManager->endOffsets[0];
	Actor *actor;
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && actor->IsActor())
			actor->UpdateActiveEffects(magicItem, effItem, callback, addNew);
	}
	objArray = g_processManager->objects.data;
	arrEnd = objArray + g_processManager->endOffsets[1];
	objArray += g_processManager->beginOffsets[1];
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && actor->IsActor())
			actor->UpdateActiveEffects(magicItem, effItem, callback, addNew);
	}
}

bool AlchemyItem::IsPoison()
{
	EffectItem *effItem;
	EffectSetting *effSetting = NULL;
	ListNode<EffectItem> *iter = magicItem.list.list.Head();
	do
	{
		if (!(effItem = iter->data)) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	}
	while (iter = iter->next);
	return effSetting != NULL;
}

bool TESModelList::ModelListAction(char *path, char action)
{
	ListNode<char> *iter = modelList.Head(), *prev = NULL;
	char *nifPath;
	do
	{
		nifPath = iter->data;
		if (!StrCompare(path, nifPath))
		{
			if (action < 0)
			{
				GameHeapFree(nifPath);
				if (prev) prev->RemoveNext();
				else iter->RemoveMe();
			}
			return action <= 0;
		}
		prev = iter;
	}
	while (iter = iter->next);
	if (action <= 0) return false;
	UInt32 length = StrLen(path) + 1;
	nifPath = (char*)GameHeapAlloc(length);
	memcpy(nifPath, path, length);
	modelList.Prepend(nifPath);
	return true;
}

void TESModelList::CopyFrom(TESModelList *source)
{
	count = source->count;
	unk10 = source->unk10;
	ListNode<char> *nextNode = modelList.Head(), *currNode = nextNode->next;
	GameHeapFree(nextNode->data);
	nextNode->data = NULL;
	nextNode->next = NULL;
	while (currNode)
	{
		nextNode = currNode->next;
		GameHeapFree(currNode->data);
		GameHeapFree(currNode);
		currNode = nextNode;
	}
	currNode = source->modelList.Head();
	char *nifPath;
	do
	{
		nifPath = currNode->data;
		if (nifPath && *nifPath)
			modelList.Prepend(CopyCString(nifPath));
	}
	while (currNode = currNode->next);
}

WeatherEntry *TESClimate::GetWeatherEntry(TESWeather *weather, bool remove = false)
{
	ListNode<WeatherEntry> *iter = weatherTypes.Head(), *prev = NULL;
	WeatherEntry *entry;
	do
	{
		entry = iter->data;
		if (entry && (entry->weather == weather))
		{
			if (remove)
			{
				if (prev) prev->RemoveNext();
				else iter->RemoveMe();
			}
			return entry;
		}
		prev = iter;
	}
	while (iter = iter->next);
	if (remove) return NULL;
	entry = (WeatherEntry*)GameHeapAlloc(sizeof(WeatherEntry));
	weatherTypes.Prepend(entry);
	return entry;
}

void *TESRecipe::ComponentList::GetComponents(Script *scriptObj)
{
	s_tempElements.Clear();
	Node *iter = Head();
	RecipeComponent *component;
	do
	{
		if (component = iter->data)
			s_tempElements.Append(component->item);
	}
	while (iter = iter->next);
	NVSEArrayVar *outArray = CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj);
	return outArray;
}

void TESRecipe::ComponentList::AddComponent(TESForm *form, UInt32 quantity)
{
	RecipeComponent *newEntry = (RecipeComponent*)GameHeapAlloc(sizeof(RecipeComponent));
	newEntry->quantity = quantity;
	newEntry->item = form;
	Prepend(newEntry);
}

UInt32 TESRecipe::ComponentList::RemoveComponent(TESForm *form)
{
	Node *iter = Head(), *prev = NULL;
	RecipeComponent *component;
	do
	{
		component = iter->data;
		if (component->item == form)
		{
			UInt32 result = component->quantity;
			GameHeapFree(component);
			if (prev) prev->RemoveNext();
			else iter->RemoveMe();
			return result;
		}
		prev = iter;
	}
	while (iter = iter->next);
	return 0;
}

void TESRecipe::ComponentList::ReplaceComponent(TESForm *form, TESForm *replace)
{
	Node *iter = Head();
	RecipeComponent *component;
	do
	{
		if ((component = iter->data) && (component->item == form))
		{
			component->item = replace;
			break;
		}
	}
	while (iter = iter->next);
}

UInt32 TESRecipe::ComponentList::GetQuantity(TESForm *form)
{
	Node *iter = Head();
	RecipeComponent *component;
	do
	{
		if ((component = iter->data) && (component->item == form))
			return component->quantity;
	}
	while (iter = iter->next);
	return 0;
}

void TESRecipe::ComponentList::SetQuantity(TESForm *form, UInt32 quantity)
{
	Node *iter = Head();
	RecipeComponent *component;
	do
	{
		if ((component = iter->data) && (component->item == form))
		{
			component->quantity = quantity;
			break;
		}
	}
	while (iter = iter->next);
}

void TESLeveledList::AddItem(TESForm *form, UInt16 level, UInt16 count, float health)
{
	SInt32 index = 0;
	ListNode<ListData> *iter = list.Head();
	do
	{
		if (!iter->data || (iter->data->level >= level)) break;
		index++;
	}
	while (iter = iter->next);
	ListData *newData = (ListData*)GameHeapAlloc(sizeof(ListData));
	LvlListExtra *newExtra = (LvlListExtra*)GameHeapAlloc(sizeof(LvlListExtra));
	newData->form = form;
	newData->level = level;
	newData->count = count;
	newExtra->health = health;
	newData->extra = newExtra;
	list.Insert(newData, index);
}

UInt32 TESLeveledList::RemoveItem(TESForm *form)
{
	UInt32 numRemoved = 0;
	ListNode<ListData> *iter = list.Head(), *prev = NULL;
	ListData *data;
	do
	{
		data = iter->data;
		if (data && (data->form == form))
		{
			GameHeapFree(data);
			if (prev) iter = prev->RemoveNext();
			else iter->RemoveMe();
			numRemoved++;
		}
		else
		{
			prev = iter;
			iter = iter->next;
		}
	}
	while (iter);
	return numRemoved;
}

SInt32 TESLeveledList::GetItemIndexByLevel(UInt32 level)
{
	SInt32 index = 0;
	bool found = false;
	ListNode<ListData> *iter = list.Head();
	do
	{
		if (iter->data && (iter->data->level == level))
		{
			found = true;
			break;
		}
		index++;
	}
	while (iter = iter->next);
	return found ? index : -1;
}

SInt32 TESLeveledList::GetItemIndexByForm(TESForm *form)
{
	SInt32 index = 0;
	bool found = false;
	ListNode<ListData> *iter = list.Head();
	do
	{
		if (iter->data && (iter->data->form == form))
		{
			found = true;
			break;
		}
		index++;
	}
	while (iter = iter->next);
	return found ? index : -1;
}

const char kDumpLvlListIndentStr[] = "                                                  ";
UInt8 s_dumpLvlListIndent = 50;

void TESLeveledList::Dump()
{
	ListData *data;
	TESForm *form;
	TESLeveledList *lvlList;
	ListNode<ListData> *iter = list.Head();
	do
	{
		if (!(data = iter->data)) continue;
		form = data->form;
		lvlList = form->GetLvlList();
		Console_Print("%s%s [%08X] Level: %d Count: %d Health: %.2f", kDumpLvlListIndentStr + s_dumpLvlListIndent,
			lvlList ? "(LeveledList)" : form->GetTheName(), form->refID, data->level, data->count, data->extra ? data->extra->health : 0);
		if (lvlList)
		{
			s_dumpLvlListIndent -= 5;
			lvlList->Dump();
			s_dumpLvlListIndent += 5;
		}
	}
	while (iter = iter->next);
}

bool TESLeveledList::HasFormDeep(TESForm *form)
{
	ListNode<ListData> *iter = list.Head();
	ListData *data;
	TESLeveledList *lvlList;
	do
	{
		if (!(data = iter->data)) continue;
		if (data->form == form)
			return true;
		lvlList = data->form->GetLvlList();
		if (lvlList && s_tempFormList.Insert(data->form) && lvlList->HasFormDeep(form))
			return true;
	}
	while (iter = iter->next);
	return false;
}

TESIdleForm *TESIdleForm::FindIdle(Actor *animActor)
{
	bool eval;
	if (!conditions.Evaluate(animActor, 0, &eval, 0)) return NULL;
	if (!children) return this;
	TESIdleForm **idles = children->data, *result = NULL;
	for (UInt32 count = children->size; count && !result; count--, idles++)
		result = (*idles)->FindIdle(animActor);
	return result;
}

UInt32 InterfaceManager::GetTopVisibleMenuID()
{
	if (currentMode < 2) return 0;
	if (activeMenu) return activeMenu->id;
	UInt32 *mnStack = &menuStack[1];
	while (*mnStack) mnStack++;
	UInt32 menuID = *(mnStack - 1);
	if (menuID != 1) return menuID;
	if (g_menuVisibility[kMenuType_Inventory])
		return kMenuType_Inventory;
	if (g_menuVisibility[kMenuType_Map])
		return kMenuType_Map;
	if (g_menuVisibility[kMenuType_Stats])
		return kMenuType_Stats;
	if (g_menuVisibility[kMenuType_Repair])
		return kMenuType_Repair;
	if (g_menuVisibility[kMenuType_ItemMod])
		return kMenuType_ItemMod;
	return 0;
}

__declspec(naked) TESObjectREFR *InterfaceManager::GetCursorPick()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x4C
		push	esi
		push	edi
		mov		esi, ecx
		lea     edi, [ebp-0x34]
		push	8
		push	0
		mov     ecx, edi
		CALL_EAX(0xE98F20)
		mov     ecx, edi
		mov		dword ptr [ecx], 1
		mov		byte ptr [ecx+0x11], 1
		mov		eax, g_TES
		push	dword ptr [eax+0xC]
		CALL_EAX(0x705FC0)
		push	0
		push	0
		push	0
		mov		eax, g_thePlayer
		push	dword ptr [eax+0x670]
		mov		ecx, ds:[0x11DEB7C]
		CALL_EAX(0xC52020)
		push	0
		lea     edx, [ebp-0x4C]
		push	edx
		lea     edx, [ebp-0x40]
		push	edx
		cvttss2si	edx, [esi+0x40]
		push	edx
		cvttss2si	edx, [esi+0x38]
		push	edx
		mov		ecx, ds:[0x11DEB7C]
		mov		ecx, [ecx+0xAC]
		CALL_EAX(0xA71080)
		push	0
		lea     edx, [ebp-0x4C]
		push	edx
		lea     edx, [ebp-0x40]
		push	edx
		mov     ecx, edi
		CALL_EAX(0xE98E20)
		xor		esi, esi
		test	al, al
		jz		done
		cmp		dword ptr [edi+0x28], 0
		jz		done
		mov		eax, [edi+0x1C]
		mov		eax, [eax]
		push	dword ptr [eax]
		CALL_EAX(0x56F930)
		pop		ecx
		mov		esi, eax
	done:
		mov     ecx, edi
		CALL_EAX(0xE98FA0)
		mov		eax, esi
		pop		edi
		pop		esi
		leave
		retn
	}
}

Tile *InterfaceManager::GetActiveTile()
{
	return activeTile ? activeTile : activeTileAlt;
}

void InterfaceManager::ClearHighlights()
{
	highlightMain.Clear();
	while (numHighlighted)
	{
		numHighlighted--;
		highlightedRefs[numHighlighted].Clear();
	}
	unk1EC = 0xFFFFFFFF;
	highlightState = 0;
}

void InterfaceManager::AddHighlightedRef(TESObjectREFR *refr)
{
	if (numHighlighted < 32)
	{
		highlightedRefs[numHighlighted].Set(refr);
		if (!numHighlighted)
			highlightMain.Set(refr);
		numHighlighted++;
		highlightState = 1;
		byte490 = 0;
	}
}

const char kDaysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

float GameTimeGlobals::GetDaysPassed(int bgnYear, int bgnMonth, int bgnDay)
{
	int totalDays = 0, iter;
	int iYear = year->data, iMonth = month->data, iDay = day->data;
	iYear -= bgnYear;
	if (iYear > 0)
	{
		totalDays = kDaysPerMonth[bgnMonth] - bgnDay;
		for (iter = bgnMonth + 1; iter < 12; iter++)
			totalDays += kDaysPerMonth[iter];
		iYear--;
		if (iYear) totalDays += (365 * iYear);
		for (iter = 0; iter < iMonth; iter++)
			totalDays += kDaysPerMonth[iter];
		totalDays += iDay - 1;
	}
	else if (bgnMonth < iMonth)
	{
		totalDays = kDaysPerMonth[bgnMonth] - bgnDay;
		for (iter = bgnMonth + 1; iter < iMonth; iter++)
			totalDays += kDaysPerMonth[iter];
		totalDays += iDay - 1;
	}
	else totalDays = iDay - bgnDay;
	return (float)totalDays + (hour->data / 24);
}

void PlayGameSound(const char *soundEDID)
{
	Sound sound(soundEDID, 0x121);
	sound.Play();
}

ArrayElementL s_callRes;

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
typedef UnorderedMap<char*, ScriptVariableEntry> ScriptVariablesMap;
UnorderedMap<UInt32, ScriptVariablesMap> s_scriptVariablesBuffer;
typedef UnorderedSet<const char*> VariableNames;
UnorderedMap<UInt32, VariableNames> s_addedVariables;

bool __fastcall GetVariableAdded(UInt32 ownerID)
{
	VariableNames *findOwner = s_addedVariables.GetPtr(ownerID);
	if (!findOwner) return false;
	return findOwner->HasKey(s_strArgBuffer);
}

ScriptVar *Script::AddVariable(ScriptEventList *eventList, UInt32 ownerID, UInt8 modIdx)
{
	VariableInfo *varInfo = GetVariableByName(s_strArgBuffer);
	if (!varInfo)
	{
		varInfo = (VariableInfo*)GameHeapAlloc(sizeof(VariableInfo));
		MemZero(varInfo, sizeof(VariableInfo));
		varInfo->idx = ++info.varCount;
		varInfo->name.Set(s_strArgBuffer);
		varList.Append(varInfo);
		s_addedVariables[refID].Insert(s_strArgBuffer);
	}
	else if (!GetVariableAdded(refID)) return NULL;

	ScriptVar *var = eventList->GetVariable(varInfo->idx);
	if (!var)
	{
		var = (ScriptVar*)GameHeapAlloc(sizeof(ScriptVar));
		var->id = varInfo->idx;
		var->next = NULL;
		var->data.num = 0;
		eventList->m_vars->Append(var);
	}

	if (s_strArgBuffer[0] != '*')
		s_scriptVariablesBuffer[ownerID][s_strArgBuffer].Set(var, modIdx);
	return var;
}

UInt32 Script::GetDataLength()
{
	UInt8 *dataPtr = data;
	if (*(UInt32*)dataPtr != 0x1D) return 0;
	dataPtr += 4;
	while (*(UInt16*)dataPtr == 0x10)
	{
		dataPtr += *(UInt16*)(dataPtr + 2) + *(UInt16*)(dataPtr + 6);
		if (*(UInt32*)dataPtr != 0x11) return 0;
		dataPtr += 4;
	}
	return dataPtr - data;
}

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
UnorderedMap<UInt32, LinkedRefEntry> s_linkedRefModified;
UnorderedMap<UInt32, UInt32> s_linkedRefDefault, s_linkedRefsTemp;

bool TESObjectREFR::SetLinkedRef(TESObjectREFR *linkObj = NULL, UInt8 modIdx = 0xFF)
{
	ExtraLinkedRef *xLinkedRef = GetExtraType(&extraDataList, LinkedRef);
	if (!linkObj)
	{
		auto findDefID = s_linkedRefDefault.Find(refID);
		if (findDefID)
		{
			if (xLinkedRef)
			{
				if (*findDefID)
				{
					TESForm *form = LookupFormByRefID(*findDefID);
					if (form && form->GetIsReference()) xLinkedRef->linkedRef = (TESObjectREFR*)form;
				}
				else RemoveExtraData(&extraDataList, xLinkedRef, true);
			}
			findDefID.Remove();
		}
		s_linkedRefModified.Erase(refID);
		return true;
	}
	if (!xLinkedRef)
	{
		AddExtraData(&extraDataList, ExtraLinkedRef::Create(linkObj));
		s_linkedRefDefault[refID] = 0;
	}
	else
	{
		if (!xLinkedRef->linkedRef) return false;
		s_linkedRefDefault[refID] = xLinkedRef->linkedRef->refID;
		xLinkedRef->linkedRef = linkObj;
	}
	s_linkedRefModified[refID].Set(linkObj->refID, modIdx);
	return true;
}

bool SetLinkedRefID(UInt32 thisID, UInt32 linkID = 0, UInt8 modIdx = 0xFF)
{
	TESObjectREFR *thisObj = (TESObjectREFR*)LookupFormByRefID(thisID);
	if (!thisObj || !thisObj->GetIsReference()) return false;
	if (!linkID) return thisObj->SetLinkedRef();
	TESObjectREFR *linkObj = (TESObjectREFR*)LookupFormByRefID(linkID);
	return linkObj && linkObj->GetIsReference() && thisObj->SetLinkedRef(linkObj, modIdx);
}

class AuxVariableValue
{
	UInt8		type;
	UInt8		byte01;
	UInt16		alloc;
	union
	{
		float	flt;
		UInt32	ref;
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

	void ReadValData()
	{
		if (type == 4)
		{
			UInt16 size = ReadRecord16();
			if (size)
			{
				alloc = AlignNumAlloc<char>(size + 1);
				str = (char*)Pool_Alloc(alloc);
				ReadRecordData(str, size);
				str[size] = 0;
			}
		}
		else
		{
			ref = ReadRecord32();
			if (type == 2) ResolveRefID(ref, &ref);
		}
	}

public:
	AuxVariableValue() : alloc(0) {}
	AuxVariableValue(UInt8 _type) : type(_type), alloc(0) {ReadValData();}
	AuxVariableValue(NVSEArrayElement &elem) : alloc(0) {SetElem(elem);}

	~AuxVariableValue() {Clear();}

	UInt8 GetType() const {return type;}
	float GetFlt() const {return (type == 1) ? flt : 0;}
	UInt32 GetRef() const {return (type == 2) ? ref : 0;}
	const char *GetStr() const {return alloc ? str : NULL;}

	void SetFlt(float value)
	{
		Clear();
		type = 1;
		flt = value;
	}

	void SetRef(TESForm *value)
	{
		Clear();
		type = 2;
		ref = value ? value->refID : 0;
	}

	void SetStr(const char *value)
	{
		type = 4;
		UInt16 size = StrLen(value);
		if (size)
		{
			size++;
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
		if (type == 2) return ArrayElementL(LookupFormByRefID(ref));
		if (type == 4) return ArrayElementL(GetStr());
		return ArrayElementL(flt);
	}

	void WriteValData() const
	{
		WriteRecord8(type);
		if (type == 4)
		{
			UInt16 size = alloc ? StrLen(str) : 0;
			WriteRecord16(size);
			if (size) WriteRecordData(str, size);
		}
		else WriteRecord32(ref);
	}
};

struct AuxVarValsArr : Vector<AuxVariableValue>
{
	AuxVarValsArr(UInt32 _alloc = 2) : Vector<AuxVariableValue>(_alloc) {}
};
typedef UnorderedMap<char*, AuxVarValsArr> AuxVarVarsMap;
typedef UnorderedMap<UInt32, AuxVarVarsMap> AuxVarOwnersMap;
typedef UnorderedMap<UInt32, AuxVarOwnersMap> AuxVarModsMap;
AuxVarModsMap s_auxVariablesPerm, s_auxVariablesTemp;

typedef UnorderedMap<UInt32, AuxVariableValue> RefMapIDsMap;
typedef UnorderedMap<char*, RefMapIDsMap> RefMapVarsMap;
typedef UnorderedMap<UInt32, RefMapVarsMap> RefMapModsMap;
RefMapModsMap s_refMapArraysPerm, s_refMapArraysTemp;

bool s_avIsPerm = false;
UInt32 s_avModIdx = 0xFF;
UInt8 s_dataChangedFlags = 0;

void __fastcall GetBaseParams(Script *scriptObj)
{
	s_avIsPerm = (s_strArgBuffer[0] != '*');
	s_avModIdx = (s_strArgBuffer[!s_avIsPerm] == '_') ? 0xFF : scriptObj->GetOverridingModIdx();
}

void __fastcall GetBaseParams(Script *scriptObj, UInt8 type)
{
	s_avIsPerm = !(type & 1);
	s_avModIdx = (type > 1) ? 0xFF : scriptObj->GetOverridingModIdx();
}

UInt32 __fastcall GetSubjectID(TESForm *form, TESObjectREFR *thisObj)
{
	if (form) return form->GetIsReference() ? ((TESObjectREFR*)form)->baseForm->refID : form->refID;
	if (thisObj) return thisObj->refID;
	return 0;
}

struct AttachedLights
{
	TESObjectREFR			*refrPtr;
	UnorderedSet<char*>		nodeNames;
};

UnorderedMap<UInt32, AttachedLights> s_attachedLightsMap;

void __fastcall RemoveAttachedLight(TESObjectREFR *refr, char *nodeName)
{
	NiNode *objNode = refr->GetNode(nodeName);
	if (!objNode) return;
	for (auto iter = objNode->m_children.Begin(); !iter.End(); ++iter)
	{
		if NOT_TYPE(*iter, NiPointLight) continue;
		((NiPointLight*)*iter)->radius = 0;
		break;
	}
}

struct EventCallbackScripts : Set<Script*>
{
	EventCallbackScripts(UInt32 _alloc = 4) : Set<Script*>(_alloc) {}

	void InvokeEvents(UInt32 arg);
	void InvokeEvents2(UInt32 arg1, UInt32 arg2);
	void InvokeEventsThis(TESObjectREFR *thisObj);
	void InvokeEventsThis1(TESObjectREFR *thisObj, UInt32 arg);
	void InvokeEventsThis2(TESObjectREFR *thisObj, UInt32 arg1, UInt32 arg2);
};

void EventCallbackScripts::InvokeEvents(UInt32 arg)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, NULL, NULL, &s_callRes, 1, arg);
}

void EventCallbackScripts::InvokeEvents2(UInt32 arg1, UInt32 arg2)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, NULL, NULL, &s_callRes, 2, arg1, arg2);
}

void EventCallbackScripts::InvokeEventsThis(TESObjectREFR *thisObj)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, thisObj, NULL, &s_callRes, 0);
}

void EventCallbackScripts::InvokeEventsThis1(TESObjectREFR *thisObj, UInt32 arg)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, thisObj, NULL, &s_callRes, 1, arg);
}

void EventCallbackScripts::InvokeEventsThis2(TESObjectREFR *thisObj, UInt32 arg1, UInt32 arg2)
{
	for (auto script = BeginCp(); script; ++script)
		CallFunction(*script, thisObj, NULL, &s_callRes, 2, arg1, arg2);
}

typedef Map<char*, EventCallbackScripts> MenuClickFiltersMap;
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
typedef Vector<QuestStageEventCallback> QuestStageCallbacks;

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
CriticalHitEventCallbacks s_criticalHitEvents;

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

typedef Vector<DisabledBlockInfo> DisabledScriptBlocks;
UnorderedMap<Script*, DisabledScriptBlocks> s_disabledScriptBlocksMap;

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
	bool End() const {return !length;}

	void Next()
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

ArrayElementR* __fastcall GetArrayData(NVSEArrayVar *srcArr, UInt32 *size)
{
	*size = GetArraySize(srcArr);
	if (!*size) return NULL;
	ArrayElementR *data = (ArrayElementR*)GetAuxBuffer(s_auxBuffers[2], *size * sizeof(ArrayElementR));
	MemZero(data, *size * sizeof(ArrayElementR));
	GetElements(srcArr, data, NULL);
	return data;
}

UnorderedSet<ContChangesEntry*> s_tempContChangesEntries(0x40);

void DoDeferredFreeEntries()
{
	for (auto iter = s_tempContChangesEntries.Begin(); iter; ++iter)
	{
		if (iter->extendData)
			GameHeapFree(iter->extendData);
		GameHeapFree(*iter);
	}
	s_tempContChangesEntries.Clear();
}

struct InventoryRefData
{
	TESForm				*type;
	ContChangesEntry	*entry;
	ExtraDataList		*xData;

	InventoryRefData(TESForm *_type, ContChangesEntry *_entry, ExtraDataList *_xData) : type(_type), entry(_entry), xData(_xData) {}
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

	SInt32 GetCount();
	ExtraDataList *CreateExtraData();
};

SInt32 InventoryRef::GetCount()
{
	if (s_tempContChangesEntries.HasKey(entry))
		return entry->countDelta;
	return xData ? xData->GetCount() : 1;
}

ExtraDataList *InventoryRef::CreateExtraData()
{
	ExtraContainerChanges::EntryDataList *entryList = containerRef->GetContainerChangesList();
	if (!entryList) return NULL;
	ContChangesEntry *pEntry = entryList->FindForItem(type);
	if (!pEntry)
	{
		pEntry = (ContChangesEntry*)GameHeapAlloc(sizeof(ContChangesEntry));
		pEntry->extendData = NULL;
		pEntry->countDelta = 0;
		pEntry->type = type;
		entryList->Prepend(pEntry);
	}
	xData = ExtraDataList::Create();
	if (pEntry->extendData)
		pEntry->extendData->Prepend(xData);
	else
	{
		pEntry->extendData = (ExtraContainerChanges::ExtendDataList*)GameHeapAlloc(8);
		pEntry->extendData->Init(xData);
	}
	containerRef->MarkAsModified(0x20);
	return xData;
}

InventoryRef* (*InventoryRefCreate)(TESObjectREFR *container, const InventoryRefData &data, bool bValidate);
InventoryRef* (*InventoryRefGetForID)(UInt32 refID);

TESObjectREFR* __fastcall CreateInventoryRef(TESObjectREFR *container, TESForm *itemForm, SInt32 countDelta, ExtraDataList *xData)
{
	ContChangesEntry *entry = (ContChangesEntry*)GameHeapAlloc(sizeof(ContChangesEntry));
	if (xData)
	{
		entry->extendData = (ExtraContainerChanges::ExtendDataList*)GameHeapAlloc(8);
		entry->extendData->Init(xData);
	}
	else entry->extendData = NULL;
	entry->countDelta = countDelta;
	entry->type = itemForm;
	s_tempContChangesEntries.Insert(entry);
	InventoryRef *invRef = InventoryRefCreate(container, InventoryRefData(itemForm, entry, xData), false);
	return invRef->tempRef;
}

TESObjectREFR* __fastcall CreateRefForStack(TESObjectREFR *container, ContChangesEntry *menuEntry)
{
	if (!container || !menuEntry)
		return NULL;
	ExtraDataList *xData = menuEntry->extendData ? menuEntry->extendData->GetFirstItem() : NULL;
	return CreateInventoryRef(container, menuEntry->type, menuEntry->countDelta, xData);
}

ExtraDataList* __fastcall SplitFromStack(ContChangesEntry *entry, ExtraDataList *xDataIn)
{
	ExtraCount *xCount = GetExtraType(xDataIn, Count);
	if (!xCount) return xDataIn;
	ExtraDataList *xDataOut = xDataIn->CreateCopy();
	RemoveExtraType(xDataOut, kExtraData_Count);
	RemoveExtraType(xDataOut, kExtraData_Hotkey);
	if (--xCount->count < 2)
	{
		RemoveExtraData(xDataIn, xCount, true);
		if (!xDataIn->m_data)
		{
			entry->extendData->Remove(xDataIn);
			GameHeapFree(xDataIn);
		}
	}
	entry->extendData->Prepend(xDataOut);
	return xDataOut;
}

TESObjectREFR* __fastcall GetEquippedItemRef(Actor *actor, UInt32 slotIndex)
{
	ExtraContainerChanges::EntryDataList *entryList = actor->GetContainerChangesList();
	if (!entryList) return NULL;
	UInt32 partMask = 1 << slotIndex;
	TESForm *item;
	ContChangesEntry *entry;
	ExtraDataList *xData;
	if (actor->GetNiNode())
	{
		if (slotIndex == 5)
		{
			entry = actor->GetWeaponInfo();
			if (entry && entry->extendData)
				return CreateInventoryRef(actor, entry->type, entry->countDelta, entry->extendData->GetFirstItem());
		}
		else
		{
			ValidBip01Names *equipment = actor->GetValidBip01Names();
			if (equipment)
			{
				ValidBip01Names::Data *slotData = equipment->slotData;
				for (UInt8 count = 20; count; count--, slotData++)
				{
					item = slotData->item;
					if (!item || NOT_TYPE(item, TESObjectARMO) || !(((TESObjectARMO*)item)->bipedModel.partMask & partMask))
						continue;
					if (!(entry = entryList->FindForItem(item)) || !(xData = entry->GetEquippedExtra()))
						break;
					return CreateInventoryRef(actor, item, entry->countDelta, xData);
				}
			}
		}
	}
	else
	{
		ListNode<ContChangesEntry> *listIter = entryList->Head();
		do
		{
			entry = listIter->data;
			if (!entry || !entry->extendData)
				continue;
			item = entry->type;
			if (slotIndex == 5)
			{
				if NOT_ID(item, TESObjectWEAP)
					continue;
			}
			else if (NOT_TYPE(item, TESObjectARMO) || !(((TESObjectARMO*)item)->bipedModel.partMask & partMask))
				continue;
			if (xData = entry->GetEquippedExtra())
				return CreateInventoryRef(actor, item, entry->countDelta, xData);
		}
		while (listIter = listIter->next);
	}
	return NULL;
}

ContChangesEntry* __fastcall GetHotkeyItemEntry(UInt8 index, ExtraDataList **outXData)
{
	ExtraContainerChanges::EntryDataList *entryList = g_thePlayer->GetContainerChangesList();
	if (!entryList) return NULL;
	ListNode<ContChangesEntry> *entryIter = entryList->Head();
	ContChangesEntry *entry;
	UInt8 type;
	ListNode<ExtraDataList> *xdlIter;
	ExtraDataList *xData;
	ExtraHotkey *xHotkey;
	do
	{
		if (!(entry = entryIter->data) || !entry->extendData)
			continue;
		type = entry->type->typeID;
		if ((type != kFormType_TESObjectARMO) && (type != kFormType_TESObjectWEAP) && (type != kFormType_AlchemyItem))
			continue;
		xdlIter = entry->extendData->Head();
		do
		{
			if (!(xData = xdlIter->data)) continue;
			xHotkey = GetExtraType(xData, Hotkey);
			if (!xHotkey || (xHotkey->index != index))
				continue;
			*outXData = xData;
			return entry;
		}
		while (xdlIter = xdlIter->next);
	}
	while (entryIter = entryIter->next);
	return NULL;
}

bool __fastcall ClearHotkey(UInt8 index)
{
	ExtraDataList *xData;
	ContChangesEntry *entry = GetHotkeyItemEntry(index, &xData);
	if (entry)
	{
		RemoveExtraType(xData, kExtraData_Hotkey);
		if (!xData->m_data)
		{
			entry->extendData->Remove(xData);
			GameHeapFree(xData);
		}
		return true;
	}
	return false;
}

float TESObjectWEAP::GetModBonuses(UInt8 modFlags, UInt32 effectID)
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if ((modFlags & (1 << idx)) && (effectMods[idx] == effectID))
			result += value1Mod[idx];
	return result;
}

float __fastcall GetModBonuses(TESObjectREFR *wpnRef, UInt32 effectID)
{
	TESObjectWEAP *weapon = (TESObjectWEAP*)wpnRef->baseForm;
	if NOT_ID(weapon, TESObjectWEAP) return 0;
	ExtraWeaponModFlags *xModFlags = GetExtraType(&wpnRef->extraDataList, WeaponModFlags);
	if (!xModFlags) return 0;
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if ((xModFlags->flags & (1 << idx)) && (weapon->effectMods[idx] == effectID))
			result += weapon->value1Mod[idx];
	return result;
}

float __fastcall GetModBonuses(InventoryRef *invRef, UInt32 effectID)
{
	TESObjectWEAP *weapon = (TESObjectWEAP*)invRef->type;
	if NOT_ID(weapon, TESObjectWEAP) return 0;
	ExtraWeaponModFlags *xModFlags = GetExtraType(invRef->xData, WeaponModFlags);
	if (!xModFlags) return 0;
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if ((xModFlags->flags & (1 << idx)) && (weapon->effectMods[idx] == effectID))
			result += weapon->value1Mod[idx];
	return result;
}

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
}
s_jipScriptRunner;

void JIPScriptRunner::Init()
{
	s_jipScriptRunner.script = (Script*)malloc(sizeof(Script));
	MemZero(s_jipScriptRunner.script, sizeof(Script));
	s_jipScriptRunner.scrContext = g_consoleManager->scriptContext;
	memcpy(s_scriptsPath, "*.txt", 6);
	char *fileName;
	for (DirectoryIterator iter(s_scriptsPathFull); !iter.End(); iter.Next())
	{
		if (!iter.IsFile()) continue;
		fileName = (char*)iter.Get();
		if (fileName[2] != '_') continue;
		switch (*(UInt16*)fileName |= 0x2020)
		{
			case 'rg':
				RunFile(fileName);
				break;
			case 'lg':
			case 'sg':
			case 'mx':
			case 'xg':
				s_jipScriptRunner.sourceFiles.Insert(fileName);
				break;
			default:
				break;
		}
	}
}

__declspec(naked) bool __stdcall JIPScriptRunner::RunScript(const char *scriptText, TESObjectREFR *thisObj)
{
	__asm
	{
		push	ebx
		push	esi
		mov		ebx, offset s_jipScriptRunner
		mov		esi, [ebx]
		mov		ecx, esi
		CALL_EAX(0x5AA0F0)
		mov		ecx, esi
		CALL_EAX(0x484490)
		push	dword ptr [esp+0xC]
		mov		ecx, esi
		CALL_EAX(0x5ABE50)
		push	dword ptr [esp+0x10]
		push	1
		push	dword ptr [ebx+4]
		mov		ecx, esi
		CALL_EAX(0x5AC400)
		mov		ecx, esi
		cmp		dword ptr [ecx+0x20], 0
		setnz	bl
		CALL_EAX(0x5AA1A0)
		mov		al, bl
		pop		esi
		pop		ebx
		retn	8
	}
}

void __stdcall JIPScriptRunner::RunFile(const char *fileName)
{
	StrCopy(s_scriptsPath, fileName);
	bool success = FileToBuffer(s_scriptsPathFull, s_strValBuffer) && RunScript(s_strValBuffer);
	PrintLog("Run Script : %s >> %s", fileName, success ? "SUCCESS" : "FAILED");
}

void __stdcall JIPScriptRunner::RunScriptFiles(UInt16 type)
{
	bool found = false;
	for (auto iter = s_jipScriptRunner.sourceFiles.Begin(); iter; ++iter)
	{
		if (*(UInt16*)*iter == type)
		{
			found = true;
			RunFile(*iter);
		}
		else if (found) break;
	}
}

bool TESObjectREFR::RunScriptSource(const char *sourceStr)
{
	ExtraScript *xScript = GetExtraType(&extraDataList, Script);
	ScriptEventList *eventList = xScript ? xScript->eventList : NULL;
	if (eventList) xScript->eventList = NULL;
	bool success = JIPScriptRunner::RunScript(sourceStr, this);
	if (!success)
	{
		if (StrLen(sourceStr) > 0x80) const_cast<char*>(sourceStr)[0x80] = 0;
		Console_Print("ERROR: Failed to execute script source:\n%s (...)", sourceStr);
	}
	if (sourceStr != s_strValBuffer) free(const_cast<char*>(sourceStr));
	if (eventList) xScript->eventList = eventList;
	return success;
}

__declspec(naked) void __fastcall DeleteArrayVar(UInt32 varID)
{
	__asm
	{
		push	ecx
		push	g_NVSEArrayMap
		call	DelArrayVar
		add		esp, 8
		retn
	}
}

__declspec(naked) void __fastcall DeleteStringVar(UInt32 varID)
{
	__asm
	{
		push	ecx
		push	g_NVSEStringMap
		call	DelStringVar
		add		esp, 8
		retn
	}
}

typedef Set<UInt32> NVSEVarsSet;

class NVSEVarsCollector
{
public:
	struct MapNode
	{
		MapNode		*lower;
		MapNode		*parent;
		MapNode		*higher;
		UInt32		key;
		void		*value;
	};

	bool			getArrays;
	UInt8			ownerIdx;
	UInt8			idxOffset;
	UInt8			pad03;
	void			(__fastcall *DelVar)(UInt32);
	MapNode			*root;
	NVSEVarsSet		varsSet;

	void CollectVars(MapNode *mapNode);

public:
	void Init(bool _getArrays, UInt8 _ownerIdx = 0xFF);
	NVSEVarsSet *GetVars() {return &varsSet;}
	void RemoveToKeep(UInt32 arrID);
	void ClearVars();

	NVSEVarsCollector(bool _getArrays, UInt8 _ownerIdx = 0xFF) {Init(_getArrays, _ownerIdx);}
};

void NVSEVarsCollector::Init(bool _getArrays, UInt8 _ownerIdx)
{
	getArrays = _getArrays;
	ownerIdx = _ownerIdx;
	UInt32 *dataPtr;
	if (getArrays)
	{
		dataPtr = (UInt32*)g_NVSEArrayMap;
		idxOffset = 0x24;
		DelVar = DeleteArrayVar;
	}
	else
	{
		dataPtr = (UInt32*)g_NVSEStringMap;
		idxOffset = 0x1C;
		DelVar = DeleteStringVar;
	}
	varsSet.Clear();
	if (!*dataPtr) return;
	dataPtr = (UInt32*)(((UInt32*)*dataPtr)[6]);
	if (dataPtr == (UInt32*)dataPtr[1]) return;
	root = (MapNode*)dataPtr;
	CollectVars((MapNode*)(dataPtr[1]));
}

void NVSEVarsCollector::CollectVars(MapNode *node)
{
	UInt8 varOwner = ((UInt8*)node->value)[idxOffset];
	if ((varOwner == ownerIdx) || (varOwner == 0xFF))
		varsSet.Insert(node->key);
	if (node->lower != root) CollectVars(node->lower);
	if (node->higher != root) CollectVars(node->higher);
}

void NVSEVarsCollector::RemoveToKeep(UInt32 arrID)
{
	NVSEArrayVar *pArray = arrID ? LookupArrayByID(arrID) : NULL;
	if (!pArray) return;
	UInt32 size;
	ArrayElementR *elements = GetArrayData(pArray, &size);
	if (!elements) return;
	while (size--)
	{
		pArray = elements[size].Array();
		if (pArray) varsSet.Erase((UInt32)pArray);
	}
}

void NVSEVarsCollector::ClearVars()
{
	for (auto varIter = varsSet.Begin(); varIter; ++varIter)
		DelVar(*varIter);
}

bool s_HUDCursorMode = false;

bool GetMenuMode()
{
	return (g_interfaceManager->currentMode > 1) || s_HUDCursorMode;
}

bool __fastcall IsMenuMode(UInt32 menuID)
{
	switch (menuID)
	{
		case 0:
			return g_interfaceManager->currentMode != 1;
		case 1:
			if (*(bool*)0x11CAB24)
				return g_menuVisibility[kMenuType_Inventory] || g_menuVisibility[kMenuType_Stats] || g_menuVisibility[kMenuType_Map];
			return g_interfaceManager->pipBoyMode == 3;
		case 2:
		{
			for (UInt32 pMenuID : g_interfaceManager->menuStack)
			{
				if (pMenuID >= kMenuType_Min) return true;
				if (!pMenuID) break;
			}
			return false;
		}
		case 3:
			return *(bool*)0x11DEA2E;
		case 4:
			return g_menuVisibility[kMenuType_Loading] && g_menuVisibility[kMenuType_Start] && g_menuVisibility[kMenuType_HUDMain];
		default:
			if ((menuID >= kMenuType_Min) && (menuID <= kMenuType_Max))
				return g_menuVisibility[menuID];
			return false;
	}
}

__declspec(naked) bool IsConsoleOpen()
{
	__asm
	{
		mov		al, byte ptr ds:[0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, ds:[0x126FD98]
		mov		edx, fs:[0x2C]
		mov		eax, [edx+eax*4]
		test	eax, eax
		jz		done
		mov		al, [eax+0x268]
	done:
		retn
	}
}

__declspec(naked) void __fastcall DoConsolePrint(double *result)
{
	__asm
	{
		call	IsConsoleOpen
		test	al, al
		jz		done
		mov		edx, [ebp]
		mov		edx, [edx-0x30]
		test	edx, edx
		jz		done
		mov		edx, [edx]
		movq	xmm0, qword ptr [ecx]
		push	ebp
		mov		ebp, esp
		sub		esp, 0x50
		lea		ecx, [ebp-0x50]
		call	StrCopy
		mov		ecx, eax
		mov		dword ptr [ecx], ' >> '
		add		ecx, 4
		call	FltToStr
		lea		eax, [ebp-0x50]
		push	eax
		push	eax
		mov		ecx, g_consoleManager
		CALL_EAX(0x71D0A0)
		leave
	done:
		retn
	}
}

__declspec(naked) void __fastcall DoConsolePrint(TESForm *result)
{
	__asm
	{
		call	IsConsoleOpen
		test	al, al
		jz		done
		mov		edx, [ebp]
		mov		edx, [edx-0x30]
		test	edx, edx
		jz		done
		mov		edx, [edx]
		push	ebp
		mov		ebp, esp
		sub		esp, 0x60
		push	esi
		mov		esi, ecx
		lea		ecx, [ebp-0x60]
		call	StrCopy
		mov		ecx, eax
		mov		dword ptr [ecx], ' >> '
		add		ecx, 4
		test	esi, esi
		jnz		haveID
		mov		word ptr [ecx], '0'
		jmp		noEDID
	haveID:
		mov		edx, [esi+0xC]
		call	UIntToHex
		mov		ecx, esi
		mov		esi, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x130]
		test	eax, eax
		jz		noEDID
		cmp		[eax], 0
		jz		noEDID
		mov		ecx, esi
		mov		word ptr [ecx], '\" '
		add		ecx, 2
		mov		edx, eax
		call	StrCopy
		mov		word ptr [eax], '\"'
	noEDID:
		pop		esi
		lea		eax, [ebp-0x60]
		push	eax
		push	eax
		mov		ecx, g_consoleManager
		CALL_EAX(0x71D0A0)
		leave
	done:
		retn
	}
}

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

__declspec(naked) TLSData *GetTLSData()
{
	__asm
	{
		mov		eax, ds:[0x126FD98]
		mov		edx, fs:[0x2C]
		mov		eax, [edx+eax*4]
		retn
	}
}

UnorderedMap<const char*, UInt32> s_fileExtToType({{"nif", 1}, {"egm", 1}, {"egt", 1}, {"kf", 1}, {"psa", 1}, {"tri", 1}, {"dds", 2}, {"fnt", 2}, {"psd", 2},
	{"tai", 2}, {"tex", 2}, {"wav", 8}, {"lip", 0x10}, {"ogg", 0x10}, {"spt", 0x40}, {"ctl", 0x100}, {"dat", 0x100}, {"dlodsettings", 0x100}, {"xml", 0x100}});

__declspec(naked) bool __fastcall GetFileArchived(const char *filePath)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x1C
		push	esi
		push	edi
		mov		esi, ecx
		call	StrLen
		push	'.'
		mov		edx, eax
		call	FindChrR
		test	eax, eax
		jz		retnFalse
		inc		eax
		push	eax
		mov		ecx, offset s_fileExtToType
		call	UnorderedMap<const char*, UInt32>::Get
		test	eax, eax
		jz		retnFalse
		mov		[ebp-4], ax
		mov		edx, 0x10A4828
		mov		ecx, esi
		call	StrBeginsCI
		test	al, al
		jz		noSubDir
		add		esi, 5
	noSubDir:
		lea		eax, [ebp-0x14]
		push	eax
		lea		eax, [ebp-0xC]
		push	eax
		push	esi
		CALL_EAX(0xAFD270)
		add		esp, 0xC
		mov		edi, ds:[0x11F8160]
		ALIGN 16
	iterHead:
		test	edi, edi
		jz		retnFalse
		mov		ecx, [edi]
		test	ecx, ecx
		jz		iterNext
		mov		ax, word ptr [ecx+0x180]
		and		ax, word ptr [ebp-4]
		jz		iterNext
		push	esi
		lea		eax, [ebp-0x1C]
		push	eax
		lea		eax, [ebp-0x18]
		push	eax
		lea		eax, [ebp-0x14]
		push	eax
		lea		eax, [ebp-0xC]
		push	eax
		CALL_EAX(0xAF9BF0)
		test	al, al
		jnz		done
	iterNext:
		mov		edi, [edi+4]
		jmp		iterHead
	retnFalse:
		xor		al, al
	done:
		pop		edi
		pop		esi
		leave
		retn
	}
}

int s_isLAA = -1;

int GetIsLAA()
{
	if (s_isLAA == -1)
	{
		HMODULE gameHandle = GetModuleHandle(NULL);
		UInt8 *dataPtr = (UInt8*)gameHandle + *(UInt32*)((UInt8*)gameHandle + 0x3C) + 0x16;
		s_isLAA = (*dataPtr & 0x20) ? 1 : 0;
		if (s_isLAA)
		{
			void *blockPtrs[20], *block;
			MemZero(blockPtrs, 0x50);
			int idx = 0;
			do
			{
				blockPtrs[idx++] = block = malloc(0x6400000);
				if (!block) break;
				if ((UInt32)block >= 0x80000000)
				{
					s_isLAA = 2;
					break;
				}
			}
			while (idx < 20);
			do
			{
				free(blockPtrs[--idx]);
			}
			while (idx);
		}
	}
	return s_isLAA;
}

TESObjectCELL *s_pcCurrCell0 = NULL, *s_pcCurrCell = NULL;
TESWorldSpace *s_pcRootWorld = NULL;

const char kMenuIDJumpTable[] =
{
	0, 1, 2, 3, -1, -1, 4, 5, 6, -1, -1, 7, 8, 9, -1, 10, -1, -1, -1, -1, -1, -1, 11, -1, -1, 12, 13, -1, -1, -1, 
	-1, -1, -1, -1, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, 17, -1, 18, 19, 20, 21, 22, 
	23, 24, 25, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, 28, 29, 30, -1, -1, 31, 32, 33, 34, 35
};

UnorderedMap<const char*, UInt32> s_menuNameToID({{"MessageMenu", kMenuType_Message}, {"InventoryMenu", kMenuType_Inventory}, {"StatsMenu", kMenuType_Stats},
	{"HUDMainMenu", kMenuType_HUDMain}, {"LoadingMenu", kMenuType_Loading}, {"ContainerMenu", kMenuType_Container}, {"DialogMenu", kMenuType_Dialog},
	{"SleepWaitMenu", kMenuType_SleepWait}, {"StartMenu", kMenuType_Start}, {"LockpickMenu", kMenuType_LockPick}, {"QuantityMenu", kMenuType_Quantity},
	{"MapMenu", kMenuType_Map}, {"BookMenu", kMenuType_Book}, {"LevelUpMenu", kMenuType_LevelUp}, {"RepairMenu", kMenuType_Repair},
	{"RaceSexMenu", kMenuType_RaceSex}, {"CharGenMenu", kMenuType_CharGen}, {"TextEditMenu", kMenuType_TextEdit}, {"BarterMenu", kMenuType_Barter},
	{"SurgeryMenu", kMenuType_Surgery}, {"HackingMenu", kMenuType_Hacking}, {"VATSMenu", kMenuType_VATS}, {"ComputersMenu", kMenuType_Computers},
	{"RepairServicesMenu", kMenuType_RepairServices}, {"TutorialMenu", kMenuType_Tutorial}, {"SpecialBookMenu", kMenuType_SpecialBook},
	{"ItemModMenu", kMenuType_ItemMod}, {"LoveTesterMenu", kMenuType_LoveTester}, {"CompanionWheelMenu", kMenuType_CompanionWheel},
	{"TraitSelectMenu", kMenuType_TraitSelect}, {"RecipeMenu", kMenuType_Recipe}, {"SlotMachineMenu", kMenuType_SlotMachine},
	{"BlackjackMenu", kMenuType_Blackjack}, {"RouletteMenu", kMenuType_Roulette}, {"CaravanMenu", kMenuType_Caravan}, {"TraitMenu", kMenuType_Trait}});

double s_nvseVersion = 0;

#define REG_CMD(name) nvse->RegisterCommand(&kCommandInfo_##name)
#define REG_CMD_STR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_String)
#define REG_CMD_ARR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_Array)

#define REFR_RES *(UInt32*)result
#define NUM_ARGS scriptData[*opcodeOffsetPtr]

DEFINE_COMMAND_PLUGIN(EmptyCommand, , 0, 0, NULL);
bool Cmd_EmptyCommand_Execute(COMMAND_ARGS)
{
	*result = 0;
	return true;
}

#if LOG_HOOKS
Map<UInt32, UInt8*> s_hookOriginalData(0x200);

void __stdcall StoreOriginalData(UInt32 addr, UInt8 size)
{
	UInt8 **dataPtr;
	if (s_hookOriginalData.Insert(addr, &dataPtr))
	{
		UInt8 *data = (UInt8*)malloc(0x10);
		*dataPtr = data;
		*data++ = size;
		memcpy(data, (void*)addr, size);
	}
}
#endif