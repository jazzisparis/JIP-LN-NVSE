#pragma once

DEFINE_COMMAND_PLUGIN(GetDestructionDataHealth, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetDestructionDataHealth, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataTargetable, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetDestructionDataTargetable, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataNumStages, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageTrait, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageTrait, 0, kParams_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageExplosion, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageExplosion, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageDebris, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageDebris, 0, kParams_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageReplacement, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageReplacement, 0, kParams_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(AddDestructionStage, 0, kParams_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(RemoveDestructionStage, 0, kParams_OneForm_OneInt);

bool Cmd_GetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	TESForm *object;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data)
			*result = (int)destructible->data->health;
	return true;
}

bool Cmd_SetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &value))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data)
			destructible->data->health = value;
	return true;
}

bool Cmd_GetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	TESForm *object;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data && destructible->data->targetable)
			*result = 1;
	return true;
}

bool Cmd_SetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &value))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data)
			destructible->data->targetable = value != 0;
	return true;
}

bool Cmd_GetDestructionDataNumStages_Execute(COMMAND_ARGS)
{
	TESForm *object;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data)
			*result = destructible->data->stageCount;
	return true;
}

bool Cmd_GetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx, traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &traitID))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && 
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			switch (traitID)
			{
				case 0:
					*result = destructible->data->stages[idx]->healthPrc;
					break;
				case 1:
					*result = (int)destructible->data->stages[idx]->selfDmgSec;
					break;
				case 2:
					*result = destructible->data->stages[idx]->dmgStage;
					break;
				case 3:
					*result = destructible->data->stages[idx]->flags;
					break;
				case 4:
					*result = (int)destructible->data->stages[idx]->debrisCount;
			}
	return true;
}

bool Cmd_SetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx, traitID, value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &traitID, &value))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			switch (traitID)
			{
				case 0:
					destructible->data->stages[idx]->healthPrc = value % 101;
					break;
				case 1:
					destructible->data->stages[idx]->selfDmgSec = value;
					break;
				case 2:
					destructible->data->stages[idx]->dmgStage = value % 9;
					break;
				case 3:
					destructible->data->stages[idx]->flags = value & 7;
					break;
				case 4:
					destructible->data->stages[idx]->debrisCount = value;
			}
	return true;
}

bool Cmd_GetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data && 
			destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->explosion)
			REFR_RES = destructible->data->stages[idx]->explosion->refID;
	return true;
}

bool Cmd_SetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	BGSExplosion *explForm = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &explForm))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			destructible->data->stages[idx]->explosion = (explForm && IS_ID(explForm, BGSExplosion)) ? explForm : nullptr;
	return true;
}

bool Cmd_GetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible && destructible->data && 
			destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->debris)
			REFR_RES = destructible->data->stages[idx]->debris->refID;
	return true;
}

bool Cmd_SetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	BGSDebris *dbrForm = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &dbrForm))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			destructible->data->stages[idx]->debris = (dbrForm && IS_ID(dbrForm, BGSDebris)) ? dbrForm : nullptr;
	return true;
}

bool Cmd_GetNthDestructionStageReplacement_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	TESForm *object;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx))
		if (BGSDestructibleObjectForm* destructible = object->GetDestructibleForm(); destructible && destructible->data && 
			destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->replacement)
			resStr = destructible->data->stages[idx]->replacement->GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetNthDestructionStageReplacement_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &path))
		if (BGSDestructibleObjectForm *destructible = object->GetDestructibleForm(); destructible &&
			destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
		{
			TESModelTextureSwap *rplc = destructible->data->stages[idx]->replacement;
			if (!rplc)
			{
				destructible->data->stages[idx]->replacement = rplc = Game_HeapAlloc<TESModelTextureSwap>();
				ZeroMemory(rplc, sizeof(TESModelTextureSwap));
				*(UInt32*)rplc = kVtbl_TESModelTextureSwap;
			}
			rplc->SetModelPath(path);
			*result = 1;
		}
	return true;
}

bool Cmd_AddDestructionStage_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *object;
	SInt32 idx;
	UInt32 health, dmgStg;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &health, &dmgStg)) return true;
	BGSDestructibleObjectForm *destructible = object->GetDestructibleForm();
	if (!destructible) return true;
	SInt32 count = (destructible->data && destructible->data->stages) ? destructible->data->stageCount : 0;
	if (idx > count) return true;
	if (idx < 0) idx = count;
	*result = idx;
	DestructionStage *destrStage = Game_HeapAlloc<DestructionStage>();
	ZeroMemory(destrStage, sizeof(DestructionStage));
	destrStage->healthPrc = health;
	destrStage->dmgStage = dmgStg;
	if (!destructible->data)
	{
		destructible->data = Game_HeapAlloc<DestructibleData>();
		destructible->data->health = 0;
		ULNG(destructible->data->stageCount) = 0x160D0000;
		destructible->data->stages = NULL;
		destructible->data->unk0C = 0;
		destructible->data->unk10 = 0;
	}
	if (destructible->data->stages)
	{
		DestructionStage **newStages = Game_HeapAlloc<DestructionStage*>(destructible->data->stageCount + 1);
		MemCopy(newStages, destructible->data->stages, idx * 4);
		MemCopy(newStages + idx + 1, destructible->data->stages + idx, (destructible->data->stageCount - idx) * 4);
		Game_HeapFree(destructible->data->stages);
		destructible->data->stages = newStages;
		newStages[idx] = destrStage;
	}
	else
	{
		destructible->data->stages = Game_HeapAlloc<DestructionStage*>();
		destructible->data->stages[0] = destrStage;
	}
	destructible->data->stageCount++;
	object->flags |= TESForm::kFormFlag_Destructible;
	return true;
}

bool Cmd_RemoveDestructionStage_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *object;
	UInt32 idx;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx)) return true;
	BGSDestructibleObjectForm *destructible = object->GetDestructibleForm();
	if (!destructible) return true;
	DestructibleData *data = destructible->data;
	if (!data || (idx >= data->stageCount)) return true;
	DestructionStage **stages = data->stages;
	if (!stages) return true;
	*result = (int)idx;
	Game_HeapFree(stages[idx]);
	if (--data->stageCount)
	{
		MemCopy(stages + idx, stages + idx + 1, (data->stageCount - idx) * 4);
		stages[data->stageCount] = NULL;
	}
	else
	{
		Game_HeapFree(stages);
		data->stages = NULL;
		object->flags &= ~TESForm::kFormFlag_Destructible;
	}
	return true;
}