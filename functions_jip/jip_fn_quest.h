#pragma once

DEFINE_COMMAND_PLUGIN(SetStageAlt, , 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(ClearActiveQuest, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetObjectiveTargets, , 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(SetObjectiveNthTarget, , 0, 4, kParams_JIP_OneQuest_TwoInts_OneObjectRef);
DEFINE_COMMAND_PLUGIN(GetObjectiveHasTarget, , 0, 3, kParams_JIP_OneQuest_OneInt_OneObjectRef);
DEFINE_COMMAND_PLUGIN(AddObjectiveTarget, , 0, 3, kParams_JIP_OneQuest_OneInt_OneObjectRef);
DEFINE_COMMAND_PLUGIN(RemoveObjectiveTarget, , 0, 3, kParams_JIP_OneQuest_OneInt_OneObjectRef);
DEFINE_COMMAND_PLUGIN(GetObjectiveText, , 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(SetObjectiveText, , 0, 23, kParams_JIP_OneQuest_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetQuests, , 0, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetQuestObjectives, , 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(GetActiveObjectives, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetObjectiveTeleportLinks, , 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(GetQuestFlag, , 0, 2, kParams_JIP_OneQuest_OneInt);
DEFINE_COMMAND_PLUGIN(SetQuestFlag, , 0, 3, kParams_JIP_OneQuest_TwoInts);

bool Cmd_SetStageAlt_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 stageID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &stageID) && quest->SetStage(stageID))
	{
		quest->currentStage = stageID;
		*result = 1;
	}
	else *result = 0;
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
	*result = 0;
	TESQuest *quest;
	UInt32 objectiveID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID)) return true;
	BGSQuestObjective *objective = quest->GetObjective(objectiveID);
	if (!objective || objective->targets.Empty()) return true;
	s_tempElements.Clear();
	ListNode<BGSQuestObjective::Target> *iter = objective->targets.Head();
	BGSQuestObjective::Target *target;
	do
	{
		target = iter->data;
		if (target && target->target)
			s_tempElements.Append(target->target);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_SetObjectiveNthTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID, index;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &index, &refr))
	{
		BGSQuestObjective *objective = quest->GetObjective(objectiveID);
		if (objective)
		{
			BGSQuestObjective::Target *target = objective->targets.GetNthItem(index);
			if (target) target->target = refr;
		}
	}
	return true;
}

bool Cmd_GetObjectiveHasTarget_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESQuest *quest;
	UInt32 objectiveID;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &refr))
	{
		BGSQuestObjective *objective = quest->GetObjective(objectiveID);
		if (objective && (objective->GetTargetIndex(refr) >= 0))
			*result = 1;
	}
	return true;
}

bool Cmd_AddObjectiveTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &refr))
	{
		BGSQuestObjective *objective = quest->GetObjective(objectiveID);
		if (objective && (objective->GetTargetIndex(refr) < 0))
		{
			BGSQuestObjective::Target *target = (BGSQuestObjective::Target*)GameHeapAlloc(sizeof(BGSQuestObjective::Target));
			ThisCall(0x60FF70, target);
			target->target = refr;
			objective->targets.Prepend(target);
		}
	}
	return true;
}

bool Cmd_RemoveObjectiveTarget_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	TESObjectREFR *refr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID, &refr))
	{
		BGSQuestObjective *objective = quest->GetObjective(objectiveID);
		SInt32 index = objective ? objective->GetTargetIndex(refr) : -1;
		if (index >= 0)
		{
			BGSQuestObjective::Target *target = objective->targets.RemoveNth(index);
			ThisCall(0x5EC4D0, target, 1);
		}
	}
	return true;
}

bool Cmd_GetObjectiveText_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	const char *resStr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID))
	{
		BGSQuestObjective *objective = quest->GetObjective(objectiveID);
		if (objective) resStr = objective->displayText.m_data;
	}
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetObjectiveText_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 objectiveID;
	if (ExtractFormatStringArgs(2, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_SetObjectiveText.numParams, &quest, &objectiveID))
	{
		BGSQuestObjective *objective = quest->GetObjective(objectiveID);
		if (objective) objective->displayText.Set(s_strArgBuffer);
	}
	return true;
}

bool Cmd_GetQuests_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 completed = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &completed))
		return true;
	s_tempFormList.Clear();
	ListNode<BGSQuestObjective> *iter = g_thePlayer->questObjectiveList.Head();
	BGSQuestObjective *objective;
	TESQuest *quest;
	bool bCompl = completed != 0;
	do
	{
		objective = iter->data;
		if (!objective || !(objective->status & 1)) continue;
		quest = objective->quest;
		if (bCompl != !(quest->flags & 2))
			s_tempFormList.Insert(quest);
	}
	while (iter = iter->next);
	if (!s_tempFormList.Empty())
	{
		s_tempElements.Clear();
		for (auto refIter = s_tempFormList.Begin(); refIter; ++refIter)
			s_tempElements.Append(*refIter);
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_GetQuestObjectives_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESQuest *quest;
	UInt32 completed;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &completed)) return true;
	s_tempElements.Clear();
	ListNode<void> *iter = quest->lVarOrObjectives.Head();
	BGSQuestObjective *objective;
	bool bCompl = completed != 0;
	do
	{
		objective = (BGSQuestObjective*)iter->data;
		if (objective && IS_TYPE(objective, BGSQuestObjective) && (objective->status & 1) && (bCompl != !(objective->status & 2)))
			s_tempElements.Append((int)objective->objectiveId);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetActiveObjectives_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (!g_thePlayer->activeQuest) return true;
	s_tempElements.Clear();
	ListNode<void> *iter = g_thePlayer->activeQuest->lVarOrObjectives.Head();
	BGSQuestObjective *objective;
	do
	{
		objective = (BGSQuestObjective*)iter->data;
		if (objective && IS_TYPE(objective, BGSQuestObjective) && ((objective->status & 3) == 1))
			s_tempElements.Append((int)objective->objectiveId);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetObjectiveTeleportLinks_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESQuest *quest;
	UInt32 objectiveID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &objectiveID)) return true;
	BGSQuestObjective *objective = quest->GetObjective(objectiveID);
	if (!objective || ((objective->status & 3) != 1)) return true;
	NVSEArrayVar *linksArr = CreateArray(NULL, 0, scriptObj);
	ListNode<BGSQuestObjective::Target> *trgIter = objective->targets.Head();
	BGSQuestObjective::Target *target;
	do
	{
		if (!(target = trgIter->data)) continue;
		if (quest != g_thePlayer->activeQuest)
			ThisCall(0x952D60, g_thePlayer, target->target, &target->data, 1);
		s_tempElements.Clear();
		for (auto lnkIter = target->data.teleportLinks.Begin(); !lnkIter.End(); ++lnkIter)
			s_tempElements.Append(lnkIter.Get().door);
		s_tempElements.Append(target->target);
		AppendElement(linksArr, ArrayElementL(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj)));
	}
	while (trgIter = trgIter->next);
	if (GetArraySize(linksArr)) AssignCommandResult(linksArr, result);
	return true;
}

bool Cmd_GetQuestFlag_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &flagID) && (flagID <= 3))
		*result = (quest->flags & (1 << flagID)) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetQuestFlag_Execute(COMMAND_ARGS)
{
	TESQuest *quest;
	UInt32 flagID, doSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &quest, &flagID, &doSet) && (flagID <= 3))
	{
		if (doSet) quest->flags |= (1 << flagID);
		else quest->flags &= ~(1 << flagID);
	};
	return true;
}