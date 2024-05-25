#pragma once

DEFINE_COMMAND_PLUGIN(GetBodyPartDataSkeleton, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBodyPartDataSkeleton, 0, kParams_OneForm_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartDataRagdoll, 0, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(SetBodyPartDataRagdoll, 0, kParams_OneForm_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetBodyPartDataHasPart, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(GetBodyPartTraitNumeric, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartTraitNumeric, 0, kParams_OneForm_TwoInts_OneFloat);
DEFINE_COMMAND_PLUGIN(GetBodyPartTraitForm, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartTraitForm, 0, kParams_OneForm_TwoInts_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetBodyPartName, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBodyPartName, 0, kParams_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartReplacementModel, 0, kParams_OneForm_OneInt);
DEFINE_COMMAND_PLUGIN(SetBodyPartReplacementModel, 0, kParams_OneForm_OneInt_OneString);
DEFINE_COMMAND_PLUGIN(GetBodyPartFlag, 0, kParams_OneForm_TwoInts);
DEFINE_COMMAND_PLUGIN(SetBodyPartFlag, 0, kParams_OneForm_ThreeInts);

bool Cmd_GetBodyPartDataSkeleton_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	BGSBodyPartData *bpData;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData) && IS_ID(bpData, BGSBodyPartData))
		resStr = bpData->model.GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetBodyPartDataSkeleton_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &path) && IS_ID(bpData, BGSBodyPartData))
		bpData->model.SetModelPath(path);
	return true;
}

bool Cmd_GetBodyPartDataRagdoll_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData) && IS_ID(bpData, BGSBodyPartData) && bpData->ragDoll)
		REFR_RES = bpData->ragDoll->refID;
	return true;
}

bool Cmd_SetBodyPartDataRagdoll_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	BGSRagdoll *ragDoll = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &ragDoll) && IS_ID(bpData, BGSBodyPartData) && (!ragDoll || IS_ID(ragDoll, BGSRagdoll)))
		bpData->ragDoll = ragDoll;
	return true;
}

bool Cmd_GetBodyPartDataHasPart_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && bpData->bodyParts[partID])
		*result = 1;
	return true;
}

bool Cmd_GetBodyPartTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &traitID) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && (traitID <= 18))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			switch (traitID)
			{
				case 0:
					*result = bodyPart->actorValue;
					break;
				case 1:
					*result = bodyPart->damageMult;
					break;
				case 2:
					*result = bodyPart->healthPercent;
					break;
				case 3:
					*result = bodyPart->toHitChance;
					break;
				case 4:
					*result = bodyPart->trackingMaxAngle;
					break;
				case 5:
					*result = bodyPart->sevrDebrisCount;
					break;
				case 6:
					*result = bodyPart->sevrDebrisScale;
					break;
				case 7:
					*result = bodyPart->sevrDecalCount;
					break;
				case 8:
					*result = bodyPart->explChance;
					break;
				case 9:
					*result = bodyPart->limbRepScale;
					break;
				case 10:
					*result = bodyPart->explDebrisCount;
					break;
				case 11:
					*result = bodyPart->explDebrisScale;
					break;
				case 12:
					*result = bodyPart->explDecalCount;
					break;
				case 13:
				case 14:
				case 15:
					*result = bodyPart->goreEffTranslate[traitID - 13];
					break;
				default:
					*result = bodyPart->goreEffRotation[traitID - 16] * Dbl180dPI;
			}
	return true;
}

bool Cmd_SetBodyPartTraitNumeric_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	float val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &traitID, &val) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && (traitID <= 18) && (val >= 0))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			switch (traitID)
			{
				case 0:
					bodyPart->actorValue = val;
					break;
				case 1:
					bodyPart->damageMult = val;
					break;
				case 2:
					bodyPart->healthPercent = val;
					break;
				case 3:
					bodyPart->toHitChance = val;
					break;
				case 4:
					bodyPart->trackingMaxAngle = val;
					break;
				case 5:
					bodyPart->sevrDebrisCount = val;
					break;
				case 6:
					bodyPart->sevrDebrisScale = val;
					break;
				case 7:
					bodyPart->sevrDecalCount = val;
					break;
				case 8:
					bodyPart->explChance = val;
					break;
				case 9:
					bodyPart->limbRepScale = val;
					break;
				case 10:
					bodyPart->explDebrisCount = val;
					break;
				case 11:
					bodyPart->explDebrisScale = val;
					break;
				case 12:
					bodyPart->explDecalCount = val;
					break;
				case 13:
				case 14:
				case 15:
					bodyPart->goreEffTranslate[traitID - 13] = val;
					break;
				default:
					bodyPart->goreEffRotation[traitID - 16] = val * FltPId180;
			}
	return true;
}

bool Cmd_GetBodyPartTraitForm_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &traitID) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && (traitID <= 5))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			switch (traitID)
			{
				case 0:
					if (bodyPart->sevrExplosion)
						REFR_RES = bodyPart->sevrExplosion->refID;
					break;
				case 1:
					if (bodyPart->explExplosion)
						REFR_RES = bodyPart->explExplosion->refID;
					break;
				case 2:
					if (bodyPart->sevrDebris)
						REFR_RES = bodyPart->sevrDebris->refID;
					break;
				case 3:
					if (bodyPart->explDebris)
						REFR_RES = bodyPart->explDebris->refID;
					break;
				case 4:
					if (bodyPart->sevrImpactDS)
						REFR_RES = bodyPart->sevrImpactDS->refID;
					break;
				default:
					if (bodyPart->explImpactDS)
						REFR_RES = bodyPart->explImpactDS->refID;
			}
	return true;
}

bool Cmd_SetBodyPartTraitForm_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, traitID;
	TESForm *object = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &traitID, &object) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && (traitID <= 5))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			switch (traitID)
			{
				case 0:
					bodyPart->sevrExplosion = (object && IS_ID(object, BGSExplosion)) ? (BGSExplosion*)object : nullptr;
					break;
				case 1:
					bodyPart->explExplosion = (object && IS_ID(object, BGSExplosion)) ? (BGSExplosion*)object : nullptr;
					break;
				case 2:
					bodyPart->sevrDebris = (object && IS_ID(object, BGSDebris)) ? (BGSDebris*)object : nullptr;
					break;
				case 3:
					bodyPart->explDebris = (object && IS_ID(object, BGSDebris)) ? (BGSDebris*)object : nullptr;
					break;
				case 4:
					bodyPart->sevrImpactDS = (object && IS_ID(object, BGSImpactDataSet)) ? (BGSImpactDataSet*)object : nullptr;
					break;
				default:
					bodyPart->explImpactDS = (object && IS_ID(object, BGSImpactDataSet)) ? (BGSImpactDataSet*)object : nullptr;
			}
	return true;
}

bool Cmd_GetBodyPartName_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID]; bodyPart && bodyPart->partName.m_dataLen)
			resStr = bodyPart->partName.m_data;
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetBodyPartName_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID;
	char nameStr[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &nameStr) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			bodyPart->partName.Set(nameStr);
	return true;
}

bool Cmd_GetBodyPartReplacementModel_Execute(COMMAND_ARGS)
{
	const char *resStr = nullptr;
	BGSBodyPartData *bpData;
	UInt32 partID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			resStr = bodyPart->limbReplacement.GetModelPath();
	AssignString(PASS_COMMAND_ARGS, resStr);
	return true;
}

bool Cmd_SetBodyPartReplacementModel_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID;
	char path[0x80];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &path) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			bodyPart->limbReplacement.SetModelPath(path);
	return true;
}

bool Cmd_GetBodyPartFlag_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &flagID) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && (flagID <= 7))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID]; bodyPart && (bodyPart->flags & (1 << flagID)))
			*result = 1;
	return true;
}

bool Cmd_SetBodyPartFlag_Execute(COMMAND_ARGS)
{
	BGSBodyPartData *bpData;
	UInt32 partID, flagID, val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &bpData, &partID, &flagID, &val) && IS_ID(bpData, BGSBodyPartData) && (partID <= 14) && (flagID <= 7))
		if (BGSBodyPart *bodyPart = bpData->bodyParts[partID])
			bodyPart->SetFlag(1 << flagID, val != 0);
	return true;
}