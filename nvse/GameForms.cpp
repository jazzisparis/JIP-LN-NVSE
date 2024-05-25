#include "nvse/GameForms.h"
#include "internal/jip_core.h"

__declspec(naked) TESForm *TESForm::GetBaseIfRef() const
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xF0], ADDR_ReturnTrue
		jz		isRef
		mov		eax, ecx
		retn
	isRef:
		jmp		TESObjectREFR::GetBaseForm
	}
}

const UInt32 kBaseComponentOffsets[] =
{
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, 0x00000006, 0x09000006, 0x09000006,
	0x00000006, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0600000E, 0x00000000, 0x00000000, 0x00000006, 0x00000006, 0x0F1A170C,
	0x0F1A170C, 0x0F1A170C, 0x1F530F0C, 0x0F271A0C, 0x00000000, 0x121F1A0F, 0x0F1A170C, 0x13251E0C, 0x0F251E0C, 0x0F211A0C, 0x0C000000,
	0x0C000000, 0x0CFE00FB, 0x0C000000, 0x0C000000, 0x0C150000, 0x00000000, 0x0F1A170C, 0x0F2D1A0C, 0x0F22270C, 0x37413D34, 0x37413D34,
	0x13000000, 0x13000000, 0x0F211A0C, 0x1329220C, 0x00000000, 0x0C000012, 0x00000000, 0x0F16000C, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006,
	0x00000000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x0000060C, 0x06000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x06000000, 0x00000006, 0x00000000, 0x00000000, 0x0F00000C, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, 0x06000000,
	0x0C000000, 0x00000006, 0x00000000, 0x06000000, 0x00000000, 0x00000000, 0x06000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0F231A0C, 0x00000006, 0x00000000, 0x00000006, 0x00000006, 0x0F20000C, 0x00000006,
	0x00000000, 0x00000006, 0x00000006, 0x00000B06, 0x00000006, 0x0F00200C, 0x0F00000C, 0x00000006, 0x00000000, 0x00000000, 0x00000000
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

__declspec(naked) const char *TESForm::GetModelPath() const
{
	__asm
	{
		movzx	edx, byte ptr [ecx+4]
		movsx	eax, byte ptr kBaseComponentOffsets[edx*4+3]
		test	eax, eax
		jz		done
		mov		eax, [ecx+eax*4+4]
		test	eax, eax
		jz		done
		cmp		[eax], 0
		jnz		done
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) TESContainer *TESForm::GetContainer() const
{
	__asm
	{
		test	ecx, ecx
		jz		retnNULL
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
		ExtraScript *xScript = GetExtraType(&((TESObjectREFR*)this)->extraDataList, ExtraScript);
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
		mov		al, 0xFF
		ALIGN 16
	iterHead:
		mov		edx, ecx
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		iterHead
		mov		ecx, [edx]
		test	ecx, ecx
		jz		done
		mov		al, [ecx+0x40C]
	done:
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

__declspec(naked) bool __fastcall TESContainer::ContainsForm(TESForm *form) const
{
	__asm
	{
		add		ecx, 4
		ALIGN 16
	iterHead:
		test	ecx, ecx
		jz		retn0
		mov		eax, [ecx]
		mov		ecx, [ecx+4]
		test	eax, eax
		jz		iterHead
		cmp		[eax+4], edx
		jnz		iterHead
		mov		al, 1
		retn
	retn0:
		xor		al, al
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

__declspec(naked) void __fastcall TESObjectCELL::ToggleNodes(UInt32 nodeBits)
{
	__asm
	{
		push	esi
		push	edi
		mov		ecx, [ecx+0xC4]
		mov		ecx, [ecx]
		mov		esi, [ecx+0xA0]
		mov		edi, 8
		ALIGN 16
	iterHead:
		mov		ecx, [esi+edi*4]
		bt		edx, edi
		setnc	al
		and		byte ptr [ecx+0x30], 0xFE
		or		[ecx+0x30], al
		dec		edi
		jns		iterHead
		pop		edi
		pop		esi
		retn
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
	auto iter = lVarOrObjectives.Head();
	do
	{
		if (BGSQuestObjective *objective = (BGSQuestObjective*)iter->data; objective && IS_TYPE(objective, BGSQuestObjective) && (objective->objectiveId == objectiveID))
			return objective;
	}
	while (iter = iter->next);
	return nullptr;
}

void TESActorBaseData::SetFactionRank(TESFaction *faction, char rank)
{
	tList<FactionListData>::Node *iter = factionList.Head(), *prev = nullptr;
	do
	{
		if (FactionListData *data = iter->data; data && (data->faction == faction))
		{
			if (rank >= 0) data->rank = rank;
			else
			{
				Game_HeapFree(data);
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
		FactionListData *data = Game_HeapAlloc<FactionListData>();
		data->faction = faction;
		data->rank = rank;
		factionList.Prepend(data);
	}
}

__declspec(naked) BGSTerrainManager::BGSTerrainNode *BGSTerrainManager::BGSTerrainNode::GetNodeByCoord(Coordinate coord) const
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
		call	BGSTerrainManager::BGSTerrainNode::GetNodeByCoord
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
	auto iter = magicItem.list.list.Head();
	do
	{
		if (!(effItem = iter->data)) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4))
			return false;
	}
	while (iter = iter->next);
	return effSetting != nullptr;
}

bool TESModelList::ModelListAction(char *path, char action)
{
	tList<char>::Node *iter = modelList.Head(), *prev = nullptr;
	do
	{
		if (char *nifPath = iter->data; nifPath && !StrCompareCI(nifPath, path))
		{
			if (action < 0)
			{
				Game_HeapFree(nifPath);
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
	tList<char>::Node *nextNode = modelList.Head(), *currNode = nextNode->next;
	Game_HeapFree(nextNode->data);
	nextNode->data = nullptr;
	nextNode->next = nullptr;
	while (currNode)
	{
		nextNode = currNode->next;
		Game_HeapFree(currNode->data);
		Game_HeapFree(currNode);
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
	tList<WeatherEntry>::Node *iter = weatherTypes.Head(), *prev = nullptr;
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
	entry = Game_HeapAlloc<WeatherEntry>();
	weatherTypes.Prepend(entry);
	return entry;
}

void TESRecipe::ComponentList::AddComponent(TESForm *form, UInt32 quantity)
{
	RecipeComponent *newEntry = Game_HeapAlloc<RecipeComponent>();
	newEntry->quantity = quantity;
	newEntry->item = form;
	Prepend(newEntry);
}

UInt32 TESRecipe::ComponentList::RemoveComponent(TESForm *form)
{
	Node *iter = Head(), *prev = nullptr;
	do
	{
		if (RecipeComponent *component = iter->data; component->item == form)
		{
			UInt32 result = component->quantity;
			Game_HeapFree(component);
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
	do
	{
		if (RecipeComponent *component = iter->data; component && (component->item == form))
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
	do
	{
		if (RecipeComponent *component = iter->data; component && (component->item == form))
			return component->quantity;
	}
	while (iter = iter->next);
	return 0;
}

void TESRecipe::ComponentList::SetQuantity(TESForm *form, UInt32 quantity)
{
	Node *iter = Head();
	do
	{
		if (RecipeComponent *component = iter->data; component && (component->item == form))
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
	auto iter = list.Head();
	do
	{
		if (!iter->data || (iter->data->level >= level)) break;
		index++;
	}
	while (iter = iter->next);
	ListData *newData = Game_HeapAlloc<ListData>();
	ContainerExtra *newExtra = Game_HeapAlloc<ContainerExtra>();
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
	tList<ListData>::Node *iter = list.Head(), *prev = nullptr;
	do
	{
		if (ListData *data = iter->data; data && (data->form == form))
		{
			Game_HeapFree(data);
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
	auto iter = list.Head();
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
	auto iter = list.Head();
	do
	{
		if (iter->data && (iter->data->form == form))
			return index;
		index++;
	}
	while (iter = iter->next);
	return -1;
}

UInt32 s_dumpLvlListIndent = 60;

void TESLeveledList::Dump()
{
	static const char kDumpLvlListIndentStr[] = "                                                            ";
	ListData *data;
	auto iter = list.Head();
	do
	{
		if (!(data = iter->data)) continue;
		TESForm *form = data->form;
		TESLeveledList *lvlList = form->GetLvlList();
		Console_Print("%s%s [%08X] Level: %d Count: %d Health: %.2f", kDumpLvlListIndentStr + s_dumpLvlListIndent,
			lvlList ? "(LeveledList)" : form->GetTheName(), form->refID, data->level, data->count, data->extra ? data->extra->health : 0);
		if (lvlList)
		{
			s_dumpLvlListIndent -= 3;
			lvlList->Dump();
			s_dumpLvlListIndent += 3;
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
	tList<EffectItem>::Node *iter = list.Head(), *prev = nullptr;
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
		Game_HeapFree(effItem);
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
			TESSaveLoadGame::GetSingleton()->AddCreatedForm(newForm);
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

char TESActorBaseData::GetFactionRank(TESFaction *faction) const
{
	auto facIter = factionList.Head();
	do
	{
		if (FactionListData	*data = facIter->data; data && (data->faction == faction))
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

TESObjectWEAP *g_rockItLauncher = nullptr;

__declspec(naked) TESAmmo *TESObjectWEAP::GetEquippedAmmo(Actor *actor) const
{
	__asm
	{
		cmp		ecx, g_rockItLauncher
		jz		retnNull
		mov		eax, [esp+4]
		test	eax, eax
		jz		baseWeap
		mov		eax, [eax+0x68]
		test	eax, eax
		jz		baseWeap
		cmp		byte ptr [eax+0x28], 1
		ja		baseWeap
		mov		edx, [eax+0x114]
		test	edx, edx
		jz		baseWeap
		cmp		ecx, [edx+8]
		jnz		baseWeap
		mov		edx, [eax+0x118]
		test	edx, edx
		jz		baseWeap
		mov		eax, [edx+8]
		test	eax, eax
		jz		baseWeap
		cmp		dword ptr [eax], kVtbl_TESAmmo
		jnz		baseWeap
		retn	4
	baseWeap:
		call	TESObjectWEAP::GetAmmo
		retn	4
	retnNull:
		xor		eax, eax
		retn	4
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

bool __fastcall ToggleDerivedActorValue(ActorValueCode specialID, ActorValueCode avID, bool toggle)
{
	if (ActorValueInfo *specialInfo = ActorValueInfo::Array()[specialID]; (specialInfo->avGroup == 0))
		if (UInt32 avGroup = ActorValueInfo::Array()[avID]->avGroup; (avGroup == 1) || (avGroup == 2) || (avGroup == 6))
		{
			SInt32 index = specialInfo->derivedStatIDs.GetIndexOf(avID);
			if (toggle)
			{
				if ((index < 0) && specialInfo->derivedStatIDs.Append(avID))
					return true;
			}
			else if (index >= 0)
			{
				specialInfo->derivedStatIDs.RemoveNth(index);
				return true;
			}
		}
	return false;
}

size_t TESQuest::DecompileResultScripts(FILE *pStream, char *pBuffer)
{
	size_t totalWritten = 0;
	char fmtBuffer[0x30];
	auto stgIter = stages.Head();
	do
	{
		if (auto stgInfo = stgIter->data)
		{
			int logIdx = 0;
			auto logIter = stgInfo->logEntries.Head();
			do
			{
				logIdx++;
				if (auto entry = logIter->data; entry && entry->resultScript.info.dataLength)
				{
					sprintf_s(fmtBuffer, "[Stage #%d :: Entry #%d]\n\n", stgInfo->stage, logIdx);
					if (pStream)
						fwrite(fmtBuffer, StrLen(fmtBuffer), 1, pStream);
					if (pBuffer)
						pBuffer = StrCopy(pBuffer, fmtBuffer);
					if (size_t numWritten = DecompileToBuffer(&entry->resultScript, pStream, pBuffer))
					{
						totalWritten += numWritten;
						if (pBuffer)
							pBuffer += numWritten;
					}
				}
			}
			while (logIter = logIter->next);
		}
	}
	while (stgIter = stgIter->next);
	return totalWritten;
}

size_t BGSTerminal::DecompileResultScripts(FILE *pStream, char *pBuffer)
{
	size_t totalWritten = 0;
	int entryIdx = 0;
	char fmtBuffer[0x30];
	auto entIter = menuEntries.Head();
	do
	{
		if (auto entry = entIter->data)
		{
			entryIdx++;
			if (entry->resultScript.info.dataLength)
			{
				sprintf_s(fmtBuffer, "[Entry #%d :: \"%s\"]\n\n", entryIdx, entry->entryText.CStr());
				if (pStream)
					fwrite(fmtBuffer, StrLen(fmtBuffer), 1, pStream);
				if (pBuffer)
					pBuffer = StrCopy(pBuffer, fmtBuffer);
				if (size_t numWritten = DecompileToBuffer(&entry->resultScript, pStream, pBuffer))
				{
					totalWritten += numWritten;
					if (pBuffer)
						pBuffer += numWritten;
				}
			}
		}
	}
	while (entIter = entIter->next);
	return totalWritten;
}

const char *kResScrTitle = "[OnBegin]\n\n\0\0\0\0\0[OnEnd]\n\n\0\0\0\0\0\0\0[OnChange]\n\n";

size_t TESTopicInfo::DecompileResultScripts(FILE *pStream, char *pBuffer)
{
	size_t totalWritten = 0;
	for (UInt32 i = 0; i < 2; i++)
		if (auto pScript = GetResultScript(i); pScript && pScript->info.dataLength)
		{
			auto title = kResScrTitle + (i << 4);
			if (pStream)
				fwrite(title, StrLen(title), 1, pStream);
			if (pBuffer)
				pBuffer = StrCopy(pBuffer, title);
			if (size_t numWritten = DecompileToBuffer(pScript, pStream, pBuffer))
			{
				totalWritten += numWritten;
				if (pBuffer)
					pBuffer += numWritten;
			}
		}
	return totalWritten;
}

size_t TESPackage::DecompileResultScripts(FILE *pStream, char *pBuffer)
{
	size_t totalWritten = 0;
	PackageEvent *pEvents = &onBegin;
	for (UInt32 i = 0; i < 3; i++)
		if (auto pScript = pEvents[i].resScript; pScript && pScript->info.dataLength)
		{
			auto title = kResScrTitle + (i << 4);
			if (pStream)
				fwrite(title, StrLen(title), 1, pStream);
			if (pBuffer)
				pBuffer = StrCopy(pBuffer, title);
			if (size_t numWritten = DecompileToBuffer(pScript, pStream, pBuffer))
			{
				totalWritten += numWritten;
				if (pBuffer)
					pBuffer += numWritten;
			}
		}
	return totalWritten;
}