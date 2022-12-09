#include "nvse/GameForms.h"

const UInt32 kBaseComponentOffsets[] =
{
	0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000006, 0x000006, 0x000006, 0x000006,
	0x000006, 0x000006, 0x000000, 0x000000, 0x000000, 0x00000E, 0x000000, 0x000000, 0x000006, 0x000006, 0x1A170C,
	0x1A170C, 0x1A170C, 0x530F0C, 0x271A0C, 0x000000, 0x1F1A0F, 0x1A170C, 0x251E0C, 0x251E0C, 0x211A0C, 0x000000,
	0x000000, 0xFE00FB, 0x000000, 0x000000, 0x150000, 0x000000, 0x1A170C, 0x2D1A0C, 0x22270C, 0x413D34, 0x413D34,
	0x000000, 0x000000, 0x211A0C, 0x29220C, 0x000000, 0x000012, 0x000000, 0x16000C, 0x000000, 0x000000, 0x000000,
	0x000000, 0x000000, 0x000006, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000006,
	0x000000, 0x000000, 0x000000, 0x000006, 0x000000, 0x00060C, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
	0x000000, 0x000006, 0x000000, 0x000000, 0x00000C, 0x000000, 0x000000, 0x000000, 0x000000, 0x000006, 0x000000,
	0x000000, 0x000006, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000006,
	0x000000, 0x000000, 0x000000, 0x000000, 0x231A0C, 0x000006, 0x000000, 0x000006, 0x000006, 0x20000C, 0x000006,
	0x000000, 0x000006, 0x000006, 0x000B06, 0x000006, 0x00200C, 0x00000C, 0x000006, 0x000000, 0x000000, 0x000000
};

__declspec(naked) TESFullName *TESForm::GetFullName() const
{
	__asm
	{
		mov		eax, [ecx+0x20]
		mov		edx, [ecx]
		cmp		dword ptr [edx+0xF0], ADDR_ReturnTrue
		cmovz	ecx, eax
		movzx	edx, byte ptr [ecx+4]
		cmp		dl, kFormType_TESObjectCELL
		jz		isCell
		movsx	eax, byte ptr kBaseComponentOffsets[edx*4]
		test	eax, eax
		jz		done
		lea		eax, [ecx+eax*4]
		retn
	isCell:
		lea		eax, [ecx+0x18]
		cmp		word ptr [eax+8], 0
		jg		done
		mov		eax, [ecx+0xC0]
		test	eax, eax
		jz		done
		add		eax, 0x18
	done:
		retn
	}
}

__declspec(naked) const char *TESForm::GetTheName() const
{
	__asm
	{
		call	TESForm::GetFullName
		test	eax, eax
		jz		nullStr
		cmp		word ptr [eax+8], 0
		jle		nullStr
		mov		eax, [eax+4]
		retn
	nullStr:
		mov		eax, 0x1011584
		retn
	}
}

__declspec(naked) TESScriptableForm *TESForm::GetScriptableForm() const
{
	__asm
	{
		movzx	edx, byte ptr [ecx+4]
		movsx	eax, byte ptr kBaseComponentOffsets[edx*4+1]
		test	eax, eax
		jz		done
		lea		eax, [ecx+eax*4]
	done:
		retn
	}
}

__declspec(naked) BGSDestructibleObjectForm *TESForm::GetDestructibleForm() const
{
	__asm
	{
		movzx	edx, byte ptr [ecx+4]
		movsx	eax, byte ptr kBaseComponentOffsets[edx*4+2]
		test	eax, eax
		jz		done
		lea		eax, [ecx+eax*4]
	done:
		retn
	}
}

__declspec(naked) TESContainer *TESForm::GetContainer() const
{
	__asm
	{
		cmp		byte ptr [ecx+4], kFormType_TESObjectCONT
		jz		isContainer
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xF8], ADDR_ReturnTrue
		jnz		retnNULL
		lea		eax, [ecx+0x64]
		retn
	isContainer:
		lea		eax, [ecx+0x30]
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

bool TESForm::HasScript() const
{
	TESScriptableForm *scriptable = GetScriptableForm();
	return scriptable && scriptable->script;
}

bool TESForm::GetScriptAndEventList(Script **script, ScriptLocals **eventList) const
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

__declspec(naked) bool TESForm::IsItemPlayable() const
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

UInt32 TESForm::GetItemValue() const
{
	if (typeID == kFormType_AlchemyItem) return ((AlchemyItem*)this)->value;
	TESValueForm *valForm = DYNAMIC_CAST(this, TESForm, TESValueForm);
	return valForm ? valForm->value : 0;
}

__declspec(naked) UInt8 TESForm::GetOverridingModIdx() const
{
	__asm
	{
		add		ecx, 0x10
		ALIGN 16
	iterHead:
		mov		eax, [ecx]
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		iterHead
		test	eax, eax
		jz		noInfo
		mov		al, [eax+0x40C]
		retn
	noInfo:
		mov		al, 0xFF
		retn
	}
}

const char *TESForm::GetDescriptionText()
{
	if (typeID == kFormType_BGSNote)
	{
		BGSNote *note = (BGSNote*)this;
		return ((note->noteType == 1) && note->noteText) ? note->noteText->Get(this, 'MANT') : nullptr;
	}
	if (typeID == kFormType_TESLoadScreen)
	{
		TESLoadScreen *loadScreen = (TESLoadScreen*)this;
		return loadScreen->screenText.m_dataLen ? loadScreen->screenText.m_data : nullptr;
	}
	TESDescription *description = DYNAMIC_CAST(this, TESForm, TESDescription);
	return description ? description->Get(this, 'CSED') : nullptr;
}

extern DataHandler *g_dataHandler;
TempObject<UnorderedMap<UInt32, const char*>> s_refStrings;

const char *TESForm::RefToString()
{
	const char **findID;
	if (!s_refStrings().InsertKey(refID, &findID))
		return *findID;
	const char *modName = g_dataHandler->GetNthModName(modIndex);
	UInt32 length = StrLen(modName);
	char *refStr = Pool_CAlloc((length + 0x18) & 0xFFFFFFF0);
	if (length) COPY_BYTES(refStr, modName, length);
	refStr[length++] = ':';
	UIntToHex(refStr + length, refID & 0xFFFFFF);
	*findID = refStr;
	return refStr;
}

const char *TESForm::GetModelPath() const
{
	TESModel *baseModel = DYNAMIC_CAST(this, TESForm, TESModel);
	if (baseModel)
	{
		const char *modelPath = baseModel->nifPath.m_data;
		if (modelPath && *modelPath)
			return modelPath;
	}
	return nullptr;
}

extern ModelLoader *g_modelLoader;

void TESForm::UnloadModel()
{
	const char *modelPath = GetModelPath();
	if (modelPath)
	{
		Model *model = nullptr;
		if (g_modelLoader->modelMap->Lookup(modelPath, &model))
		{
			g_modelLoader->modelMap->EraseKey(modelPath);
			model->Destroy();
		}
	}
}

__declspec(naked) TESLeveledList *TESForm::GetLvlList() const
{
	__asm
	{
		mov		al, [ecx+4]
		cmp		al, kFormType_TESLevCreature
		jz		getList
		cmp		al, kFormType_TESLevCharacter
		jz		getList
		cmp		al, kFormType_TESLevItem
		jz		getList
		xor		eax, eax
		retn
	getList:
		lea		eax, [ecx+0x30]
		retn
	}
}

__declspec(naked) MagicItem *TESForm::GetMagicItem() const
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xEC], ADDR_ReturnTrue
		jnz		retnNULL
		lea		eax, [ecx+0x18]
		lea		edx, [ecx+0x30]
		cmp		byte ptr [ecx+4], kFormType_SpellItem
		cmovg	eax, edx
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) SInt32 __fastcall TESContainer::GetCountForForm(TESForm *form) const
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

__declspec(naked) NiNode* __fastcall TESObjectCELL::Get3DNode(UInt32 index) const
{
	__asm
	{
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		mov		ecx, [eax+0xA0]
		mov		eax, [ecx+edx*4]
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

TESObjectCELL *CellPointerMap::Lookup(UInt32 key) const;

__declspec(naked) TESObjectCELL* __vectorcall TESWorldSpace::GetCellAtPos(__m128 pos) const
{
	__asm
	{
		cvttps2dq	xmm0, xmm0
		psrad	xmm0, 0xC
		pshuflw	xmm0, xmm0, 2
		push	ecx
		movss	[esp], xmm0
		mov		ecx, [ecx+0x30]
		call	CellPointerMap::Lookup
		retn
	}
}

__declspec(naked) TESWorldSpace *TESWorldSpace::GetRootMapWorld() const
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

BGSQuestObjective *TESQuest::GetObjective(UInt32 objectiveID) const
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
	return nullptr;
}

void TESActorBaseData::SetFactionRank(TESFaction *faction, char rank)
{
	ListNode<FactionListData> *iter = factionList.Head(), *prev = nullptr;
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

__declspec(naked) LODdata::LODNode *LODdata::LODNode::GetNodeByCoord(Coordinate coord) const
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

bool AlchemyItem::IsPoison() const
{
	EffectItem *effItem;
	EffectSetting *effSetting = nullptr;
	ListNode<EffectItem> *iter = magicItem.list.list.Head();
	do
	{
		if (!(effItem = iter->data)) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	}
	while (iter = iter->next);
	return effSetting != nullptr;
}

bool TESModelList::ModelListAction(char *path, char action)
{
	ListNode<char> *iter = modelList.Head(), *prev = nullptr;
	char *nifPath;
	do
	{
		nifPath = iter->data;
		if (nifPath && !StrCompareCI(nifPath, path))
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
	modelList.Prepend(CopyCString(path));
	return true;
}

void TESModelList::CopyFrom(TESModelList *source)
{
	count = source->count;
	unk10 = source->unk10;
	ListNode<char> *nextNode = modelList.Head(), *currNode = nextNode->next;
	GameHeapFree(nextNode->data);
	nextNode->data = nullptr;
	nextNode->next = nullptr;
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
	ListNode<WeatherEntry> *iter = weatherTypes.Head(), *prev = nullptr;
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
	if (remove) return nullptr;
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
	Node *iter = Head(), *prev = nullptr;
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
	ContainerExtra *newExtra = (ContainerExtra*)GameHeapAlloc(sizeof(ContainerExtra));
	newExtra->ownerFaction = nullptr;
	newExtra->globalVar = nullptr;
	newExtra->health = health;
	newData->form = form;
	newData->level = level;
	newData->count = count;
	newData->extra = newExtra;
	list.Insert(newData, index);
}

UInt32 TESLeveledList::RemoveItem(TESForm *form)
{
	UInt32 numRemoved = 0;
	ListNode<ListData> *iter = list.Head(), *prev = nullptr;
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
	if (!conditions.Evaluate(animActor, 0, &eval, 0)) return nullptr;
	if (!children) return this;
	TESIdleForm **idles = children->data, *result = nullptr;
	for (UInt32 count = children->size; count && !result; count--, idles++)
		result = (*idles)->FindIdle(animActor);
	return result;
}

bool EffectItemList::RemoveNthEffect(UInt32 index)
{
	if (!list.GetNthItem(1)) return false;
	ListNode<EffectItem> *iter = list.Head(), *prev = nullptr;
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

float TESObjectWEAP::GetModBonuses(UInt8 modFlags, UInt32 effectID) const
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if ((modFlags & (1 << idx)) && (effectMods[idx] == effectID))
			result += value1Mod[idx];
	return result;
}

void TESForm::DoAddForm(TESForm *newForm, bool persist, bool record) const
{
	DataHandler::GetSingleton()->DoAddForm(newForm);

	if (persist)
	{
		// Only some forms can be safely saved as SaveForm. ie TESPackage at the moment.
		bool canSave = false;
		TESPackage* package = DYNAMIC_CAST(newForm, TESForm, TESPackage);
		if (package)
			canSave = true;
		// ... more ?

		if (canSave)
			TESSaveLoadGame::Get()->AddCreatedForm(newForm);
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
	String* toSet = nullptr;

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
	String* pathStr = nullptr;

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

char TESActorBaseData::GetFactionRank(TESFaction *faction) const
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

__declspec(naked) TESAmmo *TESObjectWEAP::GetAmmo() const
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

SInt32 BGSListForm::GetIndexOf(TESForm* pForm)
{
	return list.GetIndexOf(pForm);
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
	return list.Replace(pForm, pReplaceWith);
}

BGSDefaultObjectManager* BGSDefaultObjectManager::GetSingleton()
{
	return *(BGSDefaultObjectManager**)0x11CA80C;
}