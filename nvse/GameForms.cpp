#include "nvse/GameForms.h"

static const ActorValueInfo** ActorValueInfoPointerArray = (const ActorValueInfo**)0x0011D61C8;		// See GetActorValueInfo
static const _GetActorValueInfo GetActorValueInfo = (_GetActorValueInfo)0x00066E920;	// See GetActorValueName
BGSDefaultObjectManager ** g_defaultObjectManager = (BGSDefaultObjectManager**)0x011CA80C;

TESForm *TESForm::TryGetREFRParent()
{
	if (!this) return NULL;
	if (GetIsReference())
	{
		TESObjectREFR *refr = (TESObjectREFR*)this;
		if (refr->baseForm) return refr->baseForm;
	}
	return this;
}

UInt8 TESForm::GetModIndex() const
{
	return modIndex;
}

TESFullName *TESForm::GetFullName()
{
	if (typeID == kFormType_TESObjectCELL)		// some exterior cells inherit name of parent worldspace
	{
		TESObjectCELL *cell = (TESObjectCELL*)this;
		TESFullName *fullName = &cell->fullName;
		if ((!fullName->name.m_data || !fullName->name.m_dataLen) && cell->worldSpace)
			return &cell->worldSpace->fullName;
		return fullName;
	}
	TESForm *baseForm = GetIsReference() ? ((TESObjectREFR*)this)->baseForm : this;
	return DYNAMIC_CAST(baseForm, TESForm, TESFullName);
}

const char *TESForm::GetTheName()
{
	TESFullName *fullName = GetFullName();
	return fullName ? fullName->name.CStr() : "";
}

bool TESForm::HasScript()
{
	TESScriptableForm *scriptable = DYNAMIC_CAST(this, TESForm, TESScriptableForm);
	return scriptable && scriptable->script;
}

bool TESForm::GetScriptAndEventList(Script **script, ScriptEventList **eventList)
{
	if IS_REFERENCE(this)
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

extern DataHandler *g_dataHandler;
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
		const char *modelPath = baseModel->nifPath.m_data;
		if (modelPath && *modelPath)
			return modelPath;
	}
	return NULL;
}

extern ModelLoader *g_modelLoader;

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

__declspec(naked) TESForm *TESObjectREFR::GetBaseForm()
{
	__asm
	{
		mov		eax, [ecx+0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0xF], 0xFF
		jnz		done
		mov		edx, [eax]
		cmp		edx, kVtbl_BGSPlaceableWater
		jz		isWater
		cmp		dword ptr [edx+0xF8], kAddr_ReturnTrue
		jnz		done
		push	eax
		push	kExtraData_LeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
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
		mov		edx, [eax]
		cmp		dword ptr [edx+0xF8], kAddr_ReturnTrue
		jnz		retnNULL
		push	kExtraData_LeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0x10]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
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
		test	esi, esi
		jz		done
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		iterHead
		cmp		[ecx+4], edx
		jnz		iterHead
		add		eax, [ecx]
		jmp		iterHead
	done:
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall TESNPC::SetSex(UInt32 flags)
{
	__asm
	{
		test	byte ptr [ecx+0x34], 1
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
		jbe		retnNULL
		mov		ecx, [eax+0xA0]
		mov		eax, [ecx+edx*4]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

void TESObjectCELL::ToggleNodes(UInt32 nodeBits, UInt8 doHide)
{
	if (!renderData || !renderData->masterNode) return;
	for (auto childIter = renderData->masterNode->m_children.Begin(); childIter; ++childIter)
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

WeatherEntry *TESClimate::GetWeatherEntry(TESWeather *weather, bool remove)
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
	ListNode<ListData> *iter = list.Head();
	do
	{
		if (iter->data && (iter->data->level == level))
			return index;
		index++;
	}
	while (iter = iter->next);
	return -1;
}

SInt32 TESLeveledList::GetItemIndexByForm(TESForm *form)
{
	SInt32 index = 0;
	ListNode<ListData> *iter = list.Head();
	do
	{
		if (iter->data && (iter->data->form == form))
			return index;
		index++;
	}
	while (iter = iter->next);
	return -1;
}

UInt8 s_dumpLvlListIndent = 50;

void TESLeveledList::Dump()
{
	static const char kDumpLvlListIndentStr[] = "                                                  ";
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
			g_thePlayer->magicTarget.RemoveEffect(effItem);
			ProcessManager *procMngr = ProcessManager::Get();
			MobileObject **objArray = procMngr->objects.data, **arrEnd = objArray;
			objArray += procMngr->beginOffsets[0];
			arrEnd += procMngr->endOffsets[0];
			Actor *actor;
			for (; objArray != arrEnd; objArray++)
			{
				actor = (Actor*)*objArray;
				if (actor && IS_ACTOR(actor))
					actor->magicTarget.RemoveEffect(effItem);
			}
			objArray = procMngr->objects.data;
			arrEnd = objArray + procMngr->endOffsets[1];
			objArray += procMngr->beginOffsets[1];
			for (; objArray != arrEnd; objArray++)
			{
				actor = (Actor*)*objArray;
				if (actor && IS_ACTOR(actor))
					actor->magicTarget.RemoveEffect(effItem);
			}
		}
		if (prev) prev->RemoveNext();
		else iter->RemoveMe();
		GameHeapFree(effItem);
		return true;
	}
	while (iter = iter->next);
	return false;
}

void __fastcall MagicItem::UpdateEffectsAllActors(EffectItem *effItem, bool addNew)
{
	g_thePlayer->UpdateActiveEffects(this, effItem, addNew);
	ProcessManager *procMngr = ProcessManager::Get();
	MobileObject **objArray = procMngr->objects.data, **arrEnd = objArray;
	objArray += procMngr->beginOffsets[0];
	arrEnd += procMngr->endOffsets[0];
	Actor *actor;
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && actor->IsActor())
			actor->UpdateActiveEffects(this, effItem, addNew);
	}
	objArray = procMngr->objects.data;
	arrEnd = objArray + procMngr->endOffsets[1];
	objArray += procMngr->beginOffsets[1];
	for (; objArray != arrEnd; objArray++)
	{
		actor = (Actor*)*objArray;
		if (actor && actor->IsActor())
			actor->UpdateActiveEffects(this, effItem, addNew);
	}
}

float TESObjectWEAP::GetModBonuses(UInt8 modFlags, UInt32 effectID)
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if ((modFlags & (1 << idx)) && (effectMods[idx] == effectID))
			result += value1Mod[idx];
	return result;
}

void TESForm::DoAddForm(TESForm* newForm, bool persist, bool record) const
{
	CALL_MEMBER_FN(DataHandler::Get(), DoAddForm)(newForm);

	if(persist)
	{
		// Only some forms can be safely saved as SaveForm. ie TESPackage at the moment.
		bool canSave = false;
		TESPackage* package = DYNAMIC_CAST(newForm, TESForm, TESPackage);
		if (package)
			canSave = true;
		// ... more ?

		if (canSave)
			CALL_MEMBER_FN(TESSaveLoadGame::Get(), AddCreatedForm)(newForm);
	}
}

TESForm * TESForm::CloneForm(bool persist) const
{
	TESForm	* result = CreateFormInstance(typeID);
	if(result)
	{
		result->CopyFrom(this);
		// it looks like some fields are not copied, case in point: TESObjectCONT does not copy BoundObject information.
		TESBoundObject* boundObject = DYNAMIC_CAST(result, TESForm, TESBoundObject);
		if (boundObject)
		{
			TESBoundObject* boundSource = DYNAMIC_CAST(this, TESForm, TESBoundObject);
			if (boundSource)
			{
				for (UInt8 i=0; i<6; i++)
					boundObject->bounds[i] = boundSource->bounds[i];
			}
		}
		DoAddForm(result, persist);
	}

	return result;
}

bool TESForm::IsCloned() const
{
	return GetModIndex() == 0xff;
}

// static
UInt32 TESBipedModelForm::MaskForSlot(UInt32 slot)
{
	switch(slot) {
		case ePart_Head:		return eSlot_Head;
		case ePart_Hair:		return eSlot_Hair;
		case ePart_UpperBody:	return eSlot_UpperBody;
		case ePart_LeftHand:	return eSlot_LeftHand;
		case ePart_RightHand:	return eSlot_RightHand;
		case ePart_Weapon:		return eSlot_Weapon;
		case ePart_PipBoy:		return eSlot_PipBoy;
		case ePart_Backpack:	return eSlot_Backpack;
		case ePart_Necklace:	return eSlot_Necklace;
		case ePart_Headband:	return eSlot_Headband;
		case ePart_Hat:			return eSlot_Hat;
		case ePart_Eyeglasses:	return eSlot_Eyeglasses;
		case ePart_Nosering:	return eSlot_Nosering;
		case ePart_Earrings:	return eSlot_Earrings;
		case ePart_Mask:		return eSlot_Mask;
		case ePart_Choker:		return eSlot_Choker;
		case ePart_MouthObject:	return eSlot_MouthObject;
		case ePart_BodyAddon1:	return eSlot_BodyAddon1;
		case ePart_BodyAddon2:	return eSlot_BodyAddon2;
		case ePart_BodyAddon3:	return eSlot_BodyAddon3;
		default:				return -1;
	}
}

UInt32 TESBipedModelForm::GetSlotsMask() const {
	return partMask;
}

void TESBipedModelForm::SetSlotsMask(UInt32 mask)
{
	partMask = (mask & ePartBitMask_Full);
}

UInt32 TESBipedModelForm::GetBipedMask() const {
	return bipedFlags;
}

void TESBipedModelForm::SetBipedMask(UInt32 mask)
{
	bipedFlags = mask & 0xFF;
}

void  TESBipedModelForm::SetPath(const char* newPath, UInt32 whichPath, bool bFemalePath)
{
	String* toSet = NULL;

	switch (whichPath)
	{
	case ePath_Biped:
		toSet = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		toSet = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		toSet = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	}

	if (toSet)
		toSet->Set(newPath);
}

const char* TESBipedModelForm::GetPath(UInt32 whichPath, bool bFemalePath)
{
	String* pathStr = NULL;

	switch (whichPath)
	{
	case ePath_Biped:
		pathStr = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		pathStr = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		pathStr = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	}

	if (pathStr)
		return pathStr->m_data;
	else
		return "";
}

char TESActorBaseData::GetFactionRank(TESFaction *faction)
{
	ListNode<FactionListData> *facIter = factionList.Head();
	FactionListData	*data;
	do
	{
		data = facIter->data;
		if (data && (data->faction == faction))
			return data->rank;
	}
	while (facIter = facIter->next);
	return -1;
}

static const UInt8 kHandGripTable[] =
{
	TESObjectWEAP::eHandGrip_Default,
	TESObjectWEAP::eHandGrip_1,
	TESObjectWEAP::eHandGrip_2,
	TESObjectWEAP::eHandGrip_3,
	TESObjectWEAP::eHandGrip_4,
	TESObjectWEAP::eHandGrip_5,
	TESObjectWEAP::eHandGrip_6,
};

UInt8 TESObjectWEAP::HandGrip() const
{
	for(UInt32 i = 0; i < sizeof(kHandGripTable) / sizeof(kHandGripTable[0]); i++)
		if(handGrip == kHandGripTable[i])
			return i;

	return 0;
}

void TESObjectWEAP::SetHandGrip(UInt8 _handGrip)
{
	if(_handGrip < sizeof(kHandGripTable) / sizeof(kHandGripTable[0]))
		handGrip = kHandGripTable[_handGrip];
}

UInt8 TESObjectWEAP::AttackAnimation() const
{
	switch (attackAnim)
	{
		case eAttackAnim_Default:		return 0;
		case eAttackAnim_Attack3:		return 1;
		case eAttackAnim_Attack4:		return 2;
		case eAttackAnim_Attack5:		return 3;
		case eAttackAnim_Attack6:		return 4;
		case eAttackAnim_Attack7:		return 5;
		case eAttackAnim_Attack8:		return 6;
		case eAttackAnim_AttackLeft:	return 7;
		case eAttackAnim_AttackLoop:	return 8;
		case eAttackAnim_AttackRight:	return 9;
		case eAttackAnim_AttackSpin:	return 10;
		case eAttackAnim_AttackSpin2:	return 11;
		case eAttackAnim_AttackThrow:	return 12;
		case eAttackAnim_AttackThrow2:	return 13;
		case eAttackAnim_AttackThrow3:	return 14;
		case eAttackAnim_AttackThrow4:	return 15;
		case eAttackAnim_AttackThrow5:	return 16;
		case eAttackAnim_PlaceMine:		return 17;
		case eAttackAnim_PlaceMine2:	return 18;
		case eAttackAnim_Attack9:		return 19;
		case eAttackAnim_AttackThrow6:	return 20;
		case eAttackAnim_AttackThrow7:	return 21;
		case eAttackAnim_AttackThrow8:	return 22;
		default:						return 255;
	}
}

const UInt8 kAttackAnims[] = {255, 38, 44, 50, 56, 62, 68, 26, 74, 32, 80, 86, 114, 120, 126, 132, 138, 102, 108, 144, 150, 156, 162};

void TESObjectWEAP::SetAttackAnimation(UInt32 _attackAnim)
{
	attackAnim = kAttackAnims[_attackAnim];
}

TESObjectIMOD* TESObjectWEAP::GetItemMod(UInt8 which)
{
	if ((which < 1) || (which > 3)) return NULL;
	return itemMod[which - 1];
}

__declspec(naked) TESAmmo *TESObjectWEAP::GetAmmo()
{
	__asm
	{
		mov		eax, [ecx+0xA8]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+4], kFormType_BGSListForm
		jnz		done
		lea		ecx, [eax+0x18]
		mov		edx, [eax+0x20]
		ALIGN 16
	iterHead:
		dec		edx
		js		iterEnd
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		iterHead
		mov		eax, [eax+0x18]
		retn
	iterEnd:
		mov		eax, [ecx]
	done:
		retn
	}
}

class FindByForm {
	TESForm* m_pForm;
public:
	FindByForm(TESForm* pForm) : m_pForm(pForm) {}
	bool Accept(TESForm* pForm) const {
		return pForm && (pForm->refID == m_pForm->refID) ? true : false;
	}
};

SInt32 BGSListForm::GetIndexOf(TESForm* pForm)
{
	return list.GetIndexOf(FindByForm(pForm));
}

SInt32 BGSListForm::RemoveForm(TESForm* pForm)
{
	SInt32 index = GetIndexOf(pForm);
	if (index >= 0) {
		RemoveNthForm(index);
	}
	return index;
}

SInt32 BGSListForm::ReplaceForm(TESForm* pForm, TESForm* pReplaceWith)
{
	SInt32 index = GetIndexOf(pForm);
	if (index >= 0) {
		list.ReplaceNth(index, pReplaceWith);
	}
	return index;
}

bool TESForm::IsInventoryObject() const
{
	typedef bool (*_IsInventoryObjectType)(UInt32 formType);
	static _IsInventoryObjectType IsInventoryObjectType = (_IsInventoryObjectType)0x481F30;
	return IsInventoryObjectType(typeID);
}

const char* TESPackage::TargetData::StringForTargetCode(UInt8 targetCode)
{
	switch (targetCode) {
		case TESPackage::kTargetType_Refr:
			return "Reference";
		case TESPackage::kTargetType_BaseObject:
			return "Object";
		case TESPackage::kTargetType_TypeCode:
			return "ObjectType";
		default:
			return NULL;
	}
}

UInt8 TESPackage::TargetData::TargetCodeForString(const char* targetStr)
{
	if (!_stricmp(targetStr, "REFERENCE"))
		return TESPackage::kTargetType_Refr;
	else if (!_stricmp(targetStr, "OBJECT"))
		return TESPackage::kTargetType_BaseObject;
	else if (!_stricmp(targetStr, "OBJECTTYPE"))
		return TESPackage::kTargetType_TypeCode;
	else
		return 0xFF;
}

TESPackage::TargetData* TESPackage::TargetData::Create()
{
	TargetData* data = (TargetData*)GameHeapAlloc(sizeof(TargetData));

	// fill out with same defaults as editor uses
	data->count = 0;
	data->target.objectCode = TESPackage::kObjectType_Activators;
	data->targetType = TESPackage::kTargetType_TypeCode;

	return data;
}

TESPackage::TargetData* TESPackage::GetTargetData()
{
	if (!target)
		target = TargetData::Create();

	return target;
}

void TESPackage::SetTarget(TESObjectREFR* refr)
{
	TargetData* tdata = GetTargetData();
	tdata->targetType = kTargetType_Refr;
	tdata->target.refr = refr;
	tdata->count = 150;	//DefaultDistance
}

void TESPackage::SetCount(UInt32 aCount)
{
	if (target) {
		TargetData* tdata = GetTargetData();
		tdata->count = aCount;
	}
}

void TESPackage::SetTarget(TESForm* baseForm, UInt32 count)
{
	TargetData* tdata = GetTargetData();
	tdata->targetType = kTargetType_BaseObject;
	tdata->count = count;
	tdata->target.form = baseForm;
}

void TESPackage::SetTarget(UInt8 typeCode, UInt32 count)
{
	if (typeCode > 0 && typeCode < kObjectType_Max)
	{
		TargetData* tdata = GetTargetData();
		tdata->targetType = kTargetType_TypeCode;
		tdata->target.objectCode = typeCode;
		tdata->count= count;
	}
}

TESPackage::LocationData* TESPackage::LocationData::Create()
{
	LocationData* data = (LocationData*)GameHeapAlloc(sizeof(LocationData));

	data->locationType = kPackLocation_CurrentLocation;
	data->object.form = NULL;
	data->radius = 0;

	return data;
}

TESPackage::LocationData* TESPackage::GetLocationData()
{
	if (!location)
		location = LocationData::Create();

	return location;
}

bool TESPackage::IsFlagSet(UInt32 flag)
{
	return (packageFlags & flag) == flag;
}

void TESPackage::SetFlag(UInt32 flag, bool bSet)
{
	if (bSet)
		packageFlags |= flag;
	else
		packageFlags &= ~flag;

	// handle either-or flags
	switch (flag)
	{
	case kPackageFlag_LockDoorsAtStart:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtStart) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtStart, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtStart:
		if (IsFlagSet(kPackageFlag_LockDoorsAtStart) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtStart, !bSet);
		break;
	case kPackageFlag_LockDoorsAtEnd:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtEnd) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtEnd, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtEnd:
		if (IsFlagSet(kPackageFlag_LockDoorsAtEnd) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtEnd, !bSet);
		break;
	case kPackageFlag_LockDoorsAtLocation:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtLocation) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtLocation, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtLocation:
		if (IsFlagSet(kPackageFlag_LockDoorsAtLocation) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtLocation, !bSet);
		break;
	}
}

static const char* TESPackage_ObjectTypeStrings[TESPackage::kObjectType_Max] =
{
	"NONE", "Activators", "Armors", "Books", "Clothing", "Containers", "Doors", "Ingredients", "Lights", "Miscellaneous", "Flora", "Furniture",
	"Weapons: Any", "Ammo", "NPCs", "Creatures", "Keys", "Alchemy", "Food", "All: Combat Wearable", "All: Wearable", "Weapons: Ranged", "Weapons: Melee",
	"Weapons: NONE", "Actor Effects: Any", "Actor Effects: Range Target", "Actor Effects: Range Touch", "Actor Effects: Range Self"
};

// add 1 to code before indexing
static const char* TESPackage_DayStrings[] = {
	"Any", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Weekdays", "Weekends", "MWF", "TT"
};

// add 1
static const char* TESPackage_MonthString[] = {
	"Any", "January", "February", "March", "April", "May", "June", "July", "August", "September",
	"October", "November", "December", "Spring", "Summer", "Autumn", "Winter"
};

static const char* TESPackage_LocationStrings[] = {
	"Reference", "Cell", "Current", "Editor", "Object", "ObjectType"
};

static const char* TESPackage_TypeStrings[] = {
	"Explore", "Follow", "Escort", "Eat", "Sleep", "Wander", "Travel", "Accompany", "UseItemAt", "Ambush",
	"FleeNotCombat", "CastMagic", "Sandbox", "Patrol", "Guard", "Dialogue", "UseWeapon", "Find", "Combat", "CombatLow",
	"Activate", "Alarm", "Flee", "Trespass", "Spectator", "ReactToDead", "GetUp", "DoNothing", "InGameDialogue",
	"Surface", "SearchForAttacker", "AvoidRadiation", "ReactToDestroyedObject", "ReactToGrenadeOrMine", "StealWarning",
	"PickpocketWarning", "MovementBlocked", "Sandman", "Cannibal"
};

static const char* TESPackage_ProcedureStrings[] = {
	"TRAVEL", "ACTIVATE", "ACQUIRE", "WAIT", "DIALOGUE", "GREET", "GREET DEAD", "WANDER", "SLEEP", 
	"OBSERVE COMBAT", "EAT", "FOLLOW", "ESCORT", "COMBAT", "ALARM", "PURSUE", "FLEE", "DONE", "YELD", 
	"TRAVEL TARGET", "CREATE FOLLOW", "GET UP", "MOUNT HORSE", "DISMOUNT HORSE", "DO NOTHING", "UNKNOWN 019", "UNKNOWN 01B",
	"ACCOMPANY", "USE ITEM AT", "AIM", "NOTIFY", "SANDMAN", "WAIT AMBUSH", "SURFACE", "WAIT FOR SPELL", "CHOOSE CAST", 
	"FLEE NON COMBAT", "REMOVE WORN ITEMS", "SEARCH", "CLEAR MOUNT POSITION", "SUMMON CREATURE DEFEND", "AVOID AREA", 
	"UNEQUIP ARMOR", "PATROL", "USE WEAPON", "DIALOGUE ACTIVATE", "GUARD", "SANDBOX", "USE IDLE MARKER", "TAKE BACK ITEM", 
	"SITTING", "MOVEMENT BLOCKED", "CANIBAL FEED", 
};

const char* TESPackage::StringForPackageType(UInt32 pkgType)
{
	if (pkgType < kPackType_MAX) {
		return TESPackage_TypeStrings[pkgType];
	}
	else {
		return "";
	}
}

const char* TESPackage::StringForObjectCode(UInt8 objCode)
{
	if (objCode < kObjectType_Max)
		return TESPackage_ObjectTypeStrings[objCode];

	return "";
}

UInt8 TESPackage::ObjectCodeForString(const char* objString)
{
	for (UInt32 i = 0; i < kObjectType_Max; i++) {
		if (!_stricmp(objString, TESPackage_ObjectTypeStrings[i]))
			return i;
	}

	return kObjectType_Max;
}

static const char** s_procNames = (const char**)0x011A3CC0;

const char* TESPackage::StringForProcedureCode(eProcedure proc)
{
	if (proc < kProcedure_MAX)
		return TESPackage_ProcedureStrings[proc];

	return "";
}

//const char* TESPackage::StringForProcedureCode(eProcedure proc, bool bRemovePrefix)
//{
//	static size_t prefixLen = strlen("PROCEDURE_");
//
//	const char* name = NULL;
//	// special-case "AQUIRE" (sic) to fix typo in game executable
//	if (proc == TESPackage::kProcedure_ACQUIRE) {
//		name = "PROCEDURE_ACQUIRE";
//	}
//	else {
//		name = (proc <= TESPackage::kProcedure_MAX) ? s_procNames[proc] : NULL;
//	}
//
//	if (name && bRemovePrefix) {
//		name += prefixLen;
//	}
//
//	return name;
//}

const char* TESPackage::PackageTime::DayForCode(UInt8 dayCode)
{
	dayCode += 1;
	if (dayCode >= sizeof(TESPackage_DayStrings))
		return "";
	return TESPackage_DayStrings[dayCode];
}

const char* TESPackage::PackageTime::MonthForCode(UInt8 monthCode)
{
	monthCode += 1;
	if (monthCode >= sizeof(TESPackage_MonthString))
		return "";
	return TESPackage_MonthString[monthCode];
}

UInt8 TESPackage::PackageTime::CodeForDay(const char* dayStr)
{
	for (UInt8 i = 0; i < sizeof(TESPackage_DayStrings); i++) {
		if (!_stricmp(dayStr, TESPackage_DayStrings[i])) {
			return i-1;
		}
	}

	return kWeekday_Any;
}

UInt8 TESPackage::PackageTime::CodeForMonth(const char* monthStr)
{
	for (UInt8 i = 0; i < sizeof(TESPackage_MonthString); i++) {
		if (!_stricmp(monthStr, TESPackage_MonthString[i])) {
			return i-1;
		}
	}

	return kMonth_Any;
}

const char* TESPackage::LocationData::StringForLocationCode(UInt8 locCode)
{
	if (locCode < kPackLocation_Max)
		return TESPackage_LocationStrings[locCode];
	return "";
}

const char* TESPackage::LocationData::StringForLocationCodeAndData(void)
{
#define resultSize 256
	static char result[resultSize];
	if (locationType < kPackLocation_Max) {
		switch (locationType) { 
			case kPackLocation_NearReference:
			case kPackLocation_InCell:
			case kPackLocation_ObjectID:
				if (object.form)
					sprintf_s(result, resultSize, "%s \"%s\" [%08X] with a radius of %u", TESPackage_LocationStrings[locationType], object.form->GetTheName(), 
						object.form->refID, radius);
				else
					sprintf_s(result, resultSize, "%s \"\" [%08X] with a radius of %u", TESPackage_LocationStrings[locationType], 0, radius);
				break;
			case kPackLocation_ObjectType:
				sprintf_s(result, resultSize, "%s \"%s\" [%04X] with a radius of %u", TESPackage_LocationStrings[locationType], StringForObjectCode(object.objectCode),
					object.objectCode, radius);
				break;
			default:
				sprintf_s(result, resultSize, "%s with a radius of %u", TESPackage_LocationStrings[locationType], radius);
				break;
		}
		return result;
	}
	return "";
}

const char* TESPackage::TargetData::StringForTargetCodeAndData(void)
{
#define resultSize 256
	static char result[resultSize];
	if (targetType < kTargetType_Max) {
		switch (targetType) { 
			case kTargetType_Refr:
				if (target.refr)
					sprintf_s(result, resultSize, "%s \"%s\" [%08X] with a distance of %u", StringForTargetCode(targetType), target.refr->GetTheName(),
						target.refr->refID, count);
				else
					sprintf_s(result, resultSize, "%s [%08X] with a distance of %u", StringForTargetCode(targetType), 0, count);
				break;
			case kTargetType_BaseObject:
				if (target.form)
					sprintf_s(result, resultSize, "%s \"%s\" [%08X] with a count of %u", StringForTargetCode(targetType), target.form->GetTheName(), target.form->refID, count);
				else
					sprintf_s(result, resultSize, "%s [%08X] with a count of %u", StringForTargetCode(targetType), 0, count);
				break;
			case kTargetType_TypeCode:
				sprintf_s(result, resultSize, "%s \"%s\" [%04X] with a radius of %u", StringForTargetCode(targetType), StringForObjectCode(target.objectCode),
						target.objectCode, count);
				break;
			default:
				sprintf_s(result, resultSize, "%s with a radius of %u", StringForTargetCode(targetType), count);
				break;
		}
		return result;
	}
	return "";
}

UInt8 TESPackage::LocationData::LocationCodeForString(const char* locStr)
{
	for (UInt32 i = 0; i < kPackLocation_Max; i++)
		if (!_stricmp(locStr, TESPackage_LocationStrings[i]))
			return i;
	return kPackLocation_Max;
}

const char* TESFaction::GetNthRankName(UInt32 whichRank, bool bFemale)
{
	TESFaction::Rank* rank = ranks.GetNthItem(whichRank);
	if (!rank)
		return NULL;
	else
		return bFemale ? rank->femaleName.CStr() : rank->name.CStr();
}

void TESFaction::SetNthRankName(const char* newName, UInt32 whichRank, bool bFemale)
{
	TESFaction::Rank* rank = ranks.GetNthItem(whichRank);
	if (rank)
	{
		if (bFemale)
			rank->femaleName.Set(newName);
		else
			rank->name.Set(newName);
	}
}

#if 0
UInt32 EffectItemList::CountItems() const
{
	return list.Count();
}

EffectItem* EffectItemList::ItemAt(UInt32 whichItem)
{
	return list.GetNthItem(whichItem);
}

const char* EffectItemList::GetNthEIName(UInt32 whichEffect) const
{
	EffectItem* effItem = list.GetNthItem(whichEffect);
	if (effItem->setting)
		return GetFullName(effItem->setting);
	else
		return "<no name>";
}
#endif

BGSDefaultObjectManager* BGSDefaultObjectManager::GetSingleton()
{
	return *g_defaultObjectManager;
}
