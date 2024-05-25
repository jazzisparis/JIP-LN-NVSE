#pragma once

DEFINE_COMMAND_PLUGIN(SetStageAlt, 0, kParams_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(ClearActiveQuest, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetObjectiveTargets, 0, kParams_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(SetObjectiveNthTarget, 0, kParams_OneQuest_TwoInts_OneObjectRef);
DEFINE_COMMAND_PLUGIN(GetObjectiveHasTarget, 0, kParams_OneQuest_OneInt_OneObjectRef);
DEFINE_COMMAND_PLUGIN(AddObjectiveTarget, 0, kParams_OneQuest_OneInt_OneObjectRef);
DEFINE_COMMAND_PLUGIN(RemoveObjectiveTarget, 0, kParams_OneQuest_OneInt_OneObjectRef);
DEFINE_COMMAND_PLUGIN(GetObjectiveText, 0, kParams_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(SetObjectiveText, 0, kParams_OneQuest_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetQuests, 0, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetQuestObjectives, 0, kParams_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(GetActiveObjectives, 0, nullptr);
DEFINE_COMMAND_PLUGIN(GetObjectiveTeleportLinks, 0, kParams_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(GetQuestFlag, 0, kParams_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(SetQuestFlag, 0, kParams_OneQuest_TwoInts);
DEFINE_COMMAND_PLUGIN(FailQuest, 0, kParams_OneQuest);
DEFINE_COMMAND_PLUGIN(GetQuestTargetsChanged, 0, nullptr);

bool Cmd_SetStageAlt_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 stageID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &stageID) && quest->SetStage(stageID))
	{
		quest->currentStage = stageID;
		*result = 1;
	}
	return true;
}

bool Cmd_ClearActiveQuest_Execute(COMMAND_ARGS)
{
	g_thePlayer->activeQuest = NULL;
	g_thePlayer->questTargetList.RemoveAll();
	return true;
}

bool Cmd_GetObjectiveTargets_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID); objective && ((objective->status & 3) == 1) && !objective->targets.Empty())
		{
			TempElements *tmpElements = GetTempElements();
			auto iter = objective->targets.Head();
			bool evalRes;
			do
			{
				if (ObjectiveTarget *target = iter->data; target && target->target && target->conditions.Evaluate(target->target, NULL, &evalRes, 0))
					tmpElements->Append(target->target);
			}
			while (iter = iter->next);
			if (!tmpElements->Empty())
				*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
		}
	return true;
}

bool Cmd_SetObjectiveNthTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID, index;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &index, &refr))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID))
			if (ObjectiveTarget *target = objective->targets.GetNthItem(index))
			{
				target->target = refr;
				if (quest == g_thePlayer->activeQuest)
					StdCall(0x952D60, target->target, &target->data, 1);
			}
	return true;
}

bool Cmd_GetObjectiveHasTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &refr))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID); objective && objective->targets.Find(ObjTargetFinder(refr)))
			*result = 1;
	return true;
}

bool Cmd_AddObjectiveTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &refr))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID); objective && !objective->targets.Find(ObjTargetFinder(refr)))
		{
			ObjectiveTarget *target = ThisCall<ObjectiveTarget*>(0x60FF70, Game_HeapAlloc<ObjectiveTarget>());
			target->target = refr;
			objective->targets.Prepend(target);
			if (quest == g_thePlayer->activeQuest)
				ThisCall(0x60F110, quest, &g_thePlayer->questTargetList, &g_thePlayer->questObjectiveList);
		}
	return true;
}

bool Cmd_RemoveObjectiveTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &refr))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID))
			if (ObjectiveTarget *target = objective->targets.RemoveIf(ObjTargetFinder(refr)))
			{
				ThisCall(0x5EC4D0, target, 1);
				if (quest == g_thePlayer->activeQuest)
					ThisCall(0x60F110, quest, &g_thePlayer->questTargetList, &g_thePlayer->questObjectiveList);
			}
	return true;
}

bool Cmd_GetObjectiveText_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	const char *resStr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID))
			resStr = objective->displayText.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetObjectiveText_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	char *buffer = GetStrArgBuffer();
	if (ExtractFormatStringArgs(2, buffer, EXTRACT_ARGS_EX, kCommandInfo_SetObjectiveText.numParams, &quest, &objectiveID))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID))
			objective->displayText.Set(buffer);
	return true;
}

bool Cmd_GetQuests_Execute(COMMAND_ARGS)
{
	UInt32 completed = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &completed))
	{
		TempElements *tmpElements = GetTempElements();
		auto iter = g_thePlayer->questObjectiveList.Head();
		if (completed) completed = TESQuest::kFlag_Completed;
		do
		{
			if (BGSQuestObjective *objective = iter->data; objective && (objective->status & 1))
				if (TESQuest *quest = objective->quest; (completed == (quest->questFlags & TESQuest::kFlag_Completed)))
					tmpElements->InsertUnique(quest);
		}
		while (iter = iter->next);
		if (!tmpElements->Empty())
			*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_GetQuestObjectives_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 completed;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &completed))
	{
		TempElements *tmpElements = GetTempElements();
		auto iter = quest->lVarOrObjectives.Head();
		if (completed) completed = 2;
		completed++;
		do
		{
			if (BGSQuestObjective *objective = (BGSQuestObjective*)iter->data; objective && IS_TYPE(objective, BGSQuestObjective) && (completed == (objective->status & 3)))
				tmpElements->Append((int)objective->objectiveId);
		}
		while (iter = iter->next);
		if (!tmpElements->Empty())
			*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_GetActiveObjectives_Execute(COMMAND_ARGS)
{
	if (g_thePlayer->activeQuest)
	{
		TempElements *tmpElements = GetTempElements();
		auto iter = g_thePlayer->activeQuest->lVarOrObjectives.Head();
		do
		{
			if (BGSQuestObjective *objective = (BGSQuestObjective*)iter->data; objective && IS_TYPE(objective, BGSQuestObjective) && ((objective->status & 3) == 1))
				tmpElements->Append((int)objective->objectiveId);
		}
		while (iter = iter->next);
		if (!tmpElements->Empty())
			*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

bool Cmd_GetObjectiveTeleportLinks_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID))
		if (BGSQuestObjective *objective = quest->GetObjective(objectiveID); objective && ((objective->status & 3) == 1))
		{
			NVSEArrayVar *linksArr = CreateArray(NULL, 0, scriptObj);
			TempElements *tmpElements = GetTempElements();
			auto trgIter = objective->targets.Head();
			TESQuest *activeQuest = g_thePlayer->activeQuest;
			bool evalRes;
			do
			{
				if (ObjectiveTarget *target = trgIter->data; target && target->target && target->conditions.Evaluate(target->target, NULL, &evalRes, 0))
				{
					if (quest != activeQuest)
						StdCall(0x952D60, target->target, &target->data, 1);
					for (auto lnkIter = target->data.teleportLinks.Begin(); lnkIter; ++lnkIter)
						tmpElements->Append(lnkIter.Get().door);
					tmpElements->Append(target->target);
					AppendElement(linksArr, ArrayElementL(CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj)));
					tmpElements->Clear();
				}
			}
			while (trgIter = trgIter->next);
			*result = (int)linksArr;
		}
	return true;
}

bool Cmd_GetQuestFlag_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &flagID) && (flagID <= 7) && (quest->questFlags & (1 << flagID)))
		*result = 1;
	return true;
}

bool Cmd_SetQuestFlag_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 flagID, doSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &flagID, &doSet) && (flagID <= 7))
		if (doSet) quest->questFlags |= (1 << flagID);
		else quest->questFlags &= ~(1 << flagID);
	return true;
}

bool Cmd_FailQuest_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest))
		ThisCall(0x60CAF0, quest, 1);
	return true;
}

TempObject<TempFormList> s_lastQuestTargets;

bool Cmd_GetQuestTargetsChanged_Execute(COMMAND_ARGS)
{
	TempFormList *tmpFormLst = GetTempFormList();
	auto objIter = g_thePlayer->questObjectiveList.Head();
	bool evalRes;
	do
	{
		if (BGSQuestObjective *objective = objIter->data; objective && !(objective->quest->questFlags & TESQuest::kFlag_Completed) &&
			((objective->status & 3) == 1) && !objective->targets.Empty())
		{
			auto trgIter = objective->targets.Head();
			do
			{
				if (ObjectiveTarget *target = trgIter->data; target && target->target && target->conditions.Evaluate(target->target, NULL, &evalRes, 0))
					tmpFormLst->Insert(target->target);
			}
			while (trgIter = trgIter->next);
		}
	}
	while (objIter = objIter->next);
	if (s_lastQuestTargets() != *tmpFormLst)
	{
		s_lastQuestTargets = *tmpFormLst;
		*result = 1;
	}
	return true;
}