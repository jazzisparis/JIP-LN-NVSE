#pragma once

DEFINE_COMMAND_PLUGIN(GetDestructionDataHealth, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetDestructionDataHealth, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataTargetable, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetDestructionDataTargetable, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetDestructionDataNumStages, , 0, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageTrait, , 0, 3, kParams_JIP_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageTrait, , 0, 4, kParams_JIP_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageExplosion, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageExplosion, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageDebris, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageDebris, , 0, 3, kParams_JIP_OneForm_OneInt_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetNthDestructionStageReplacement, , 0, 2, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetNthDestructionStageReplacement, , 0, 3, kParams_JIP_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(AddDestructionStage, , 0, 4, kParams_JIP_OneForm_ThreeInts);
DEFINE_COMMAND_PLUGIN(RemoveDestructionStage, , 0, 2, kParams_OneForm_OneInt);

bool Cmd_GetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (destructible && destructible->data) *result = (int)destructible->data->health;
	return true;
}

bool Cmd_SetDestructionDataHealth_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &value)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (destructible && destructible->data)
		destructible->data->health = value;
	return true;
}

bool Cmd_GetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (destructible && destructible->data) *result = destructible->data->targetable ? 1 : 0;
	return true;
}

bool Cmd_SetDestructionDataTargetable_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 value;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &value)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (destructible && destructible->data)
		destructible->data->targetable = value ? true : false;
	return true;
}

bool Cmd_GetDestructionDataNumStages_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (destructible && destructible->data) *result = destructible->data->stageCount;
	return true;
}

bool Cmd_GetNthDestructionStageTrait_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object;
	UInt32 idx, traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &traitID)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (!destructible || !destructible->data || !destructible->data->stages || (idx >= destructible->data->stageCount)) return true;
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
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &traitID, &value)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (!destructible || !destructible->data || !destructible->data->stages || (idx >= destructible->data->stageCount)) return true;
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
	*result = 0;
	TESForm *object;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx))
	{
		BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data && destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->explosion)
			REFR_RES = destructible->data->stages[idx]->explosion->refID;
	}
	return true;
}

bool Cmd_SetNthDestructionStageExplosion_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	BGSExplosion *explForm = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &explForm))
	{
		BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			destructible->data->stages[idx]->explosion = (explForm && IS_ID(explForm, BGSExplosion)) ? explForm : NULL;
	}
	return true;
}

bool Cmd_GetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx))
	{
		BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data && destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->debris)
			REFR_RES = destructible->data->stages[idx]->debris->refID;
	}
	return true;
}

bool Cmd_SetNthDestructionStageDebris_Execute(COMMAND_ARGS)
{
	TESForm *object;
	UInt32 idx;
	BGSDebris *dbrForm = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &dbrForm))
	{
		BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data && destructible->data->stages && (idx < destructible->data->stageCount))
			destructible->data->stages[idx]->debris = (dbrForm && IS_ID(dbrForm, BGSDebris)) ? dbrForm : NULL;
	}
	return true;
}

bool Cmd_GetNthDestructionStageReplacement_Execute(COMMAND_ARGS)
{
	const char *resStr = NULL;
	TESForm *object;
	UInt32 idx;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx))
	{
		BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data && destructible->data->stages && (idx < destructible->data->stageCount) && destructible->data->stages[idx]->replacement)
			resStr = destructible->data->stages[idx]->replacement->GetModelPath();
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetNthDestructionStageReplacement_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *object;
	UInt32 idx;
	char path[0x80];
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &path)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (!destructible || !destructible->data || !destructible->data->stages || (idx >= destructible->data->stageCount)) return true;
	TESModelTextureSwap *rplc = destructible->data->stages[idx]->replacement;
	if (!rplc)
	{
		destructible->data->stages[idx]->replacement = rplc = (TESModelTextureSwap*)GameHeapAlloc(sizeof(TESModelTextureSwap));
		MemZero(rplc, sizeof(TESModelTextureSwap));
		*(UInt32*)rplc = kVtbl_TESModelTextureSwap;
	}
	rplc->SetModelPath(path);
	*result = 1;
	return true;
}

bool Cmd_AddDestructionStage_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *object;
	SInt32 idx;
	UInt32 health, dmgStg;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx, &health, &dmgStg)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (!destructible) return true;
	SInt32 count = (destructible->data && destructible->data->stages) ? destructible->data->stageCount : 0;
	if (idx > count) return true;
	if (idx < 0) idx = count;
	*result = idx;
	DestructionStage *destrStage = (DestructionStage*)GameHeapAlloc(sizeof(DestructionStage));
	MemZero(destrStage, sizeof(DestructionStage));
	destrStage->healthPrc = health;
	destrStage->dmgStage = dmgStg;
	if (!destructible->data)
	{
		destructible->data = (DestructibleData*)GameHeapAlloc(sizeof(DestructibleData));
		destructible->data->health = 0;
		*(UInt32*)&destructible->data->stageCount = 0x160D0000;
		destructible->data->stages = NULL;
	}
	if (destructible->data->stages)
	{
		DestructionStage **newStages = (DestructionStage**)GameHeapAlloc((destructible->data->stageCount + 1) * 4);
		memcpy(newStages, destructible->data->stages, idx * 4);
		memcpy(newStages + idx + 1, destructible->data->stages + idx, (destructible->data->stageCount - idx) * 4);
		GameHeapFree(destructible->data->stages);
		destructible->data->stages = newStages;
		newStages[idx] = destrStage;
	}
	else
	{
		destructible->data->stages = (DestructionStage**)GameHeapAlloc(4);
		destructible->data->stages[0] = destrStage;
	}
	destructible->data->stageCount++;
	return true;
}

bool Cmd_RemoveDestructionStage_Execute(COMMAND_ARGS)
{
	*result = -1;
	TESForm *object;
	UInt32 idx;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &object, &idx)) return true;
	BGSDestructibleObjectForm *destructible = DYNAMIC_CAST(object, TESForm, BGSDestructibleObjectForm);
	if (!destructible) return true;
	DestructibleData *data = destructible->data;
	if (!data || (idx >= data->stageCount)) return true;
	DestructionStage **stages = data->stages;
	if (!stages) return true;
	*result = (int)idx;
	GameHeapFree(stages[idx]);
	if (--data->stageCount)
	{
		memmove(stages + idx, stages + idx + 1, (data->stageCount - idx) * 4);
		stages[data->stageCount] = NULL;
	}
	else
	{
		GameHeapFree(stages);
		data->stages = NULL;
	}
	return true;
}