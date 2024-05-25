#pragma once

DEFINE_COMMAND_PLUGIN(SetPersistent, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(GetObjectDimensions, GetObjDim, 0, kParams_OneAxis_OneOptionalBoundObject);
DEFINE_COMMAND_PLUGIN(GetIsItem, 0, kParams_OneOptionalObjectID);
DEFINE_COMMAND_ALT_PLUGIN(SetLinkedReference, SetLinkedRef, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetEnableChildren, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetLinkedChildren, 1, nullptr);
DEFINE_COMMAND_PLUGIN(HasPrimitive, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetPrimitiveBound, 1, kParams_OneAxis);
DEFINE_COMMAND_PLUGIN(SetPrimitiveBound, 1, kParams_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(AddPrimitive, 1, kParams_OneInt_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetTeammates, 0, nullptr);
DEFINE_COMMAND_PLUGIN(MoveToCell, 1, kParams_OneForm_ThreeFloats);
DEFINE_COMMAND_PLUGIN(MoveToEditorPosition, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetCenterPos, 1, kParams_OneAxis);
DEFINE_COMMAND_PLUGIN(GetRefType, 0, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_ALT_PLUGIN(ToggleObjectCollision, TOC, 1, kParams_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(GetMaterialPropertyValue, GetMatrPropVal, 1, kParams_OneString_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetMaterialPropertyValue, SetMatrPropVal, 1, kParams_OneString_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetHasContact, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetHasContactBase, 1, kParams_OneForm);
DEFINE_CMD_COND_PLUGIN(GetHasContactType, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetContactRefs, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetHasPhantom, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetInteractionDisabled, 0, kParams_OneOptionalBoundObject);
DEFINE_COMMAND_PLUGIN(SetInteractionDisabled, 0, kParams_OneInt_OneOptionalBoundObject);
DEFINE_COMMAND_PLUGIN(GetInteractionDisabledType, 0, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetInteractionDisabledType, 0, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(AddRefMapMarker, 1, nullptr);
DEFINE_COMMAND_PLUGIN(RemoveRefMapMarker, 1, nullptr);
DEFINE_COMMAND_PLUGIN(RefHasMapMarker, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetPosEx, 1, kParams_ThreeFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(MoveToReticle, 1, kParams_FourOptionalFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetRefName, 1, kParams_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetAngleEx, 1, kParams_ThreeFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetTeleportDoor, 1, nullptr);
DEFINE_COMMAND_PLUGIN(SetOnCriticalHitEventHandler, 0, kParams_OneForm_OneInt_ThreeOptionalForms);
DEFINE_COMMAND_PLUGIN(MoveToFadeDelay, 1, kParams_OneObjectRef_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCrosshairWater, 0, nullptr);
DEFINE_COMMAND_PLUGIN(IsAnimPlayingEx, 1, kParams_OneInt_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(GetRigidBodyMass, 1, nullptr);
DEFINE_COMMAND_PLUGIN(PushObject, 1, kParams_FourFloats_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(MoveToContainer, 1, kParams_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockTranslation, GetNifTrans, 1, kParams_OneString_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(SetNifBlockTranslation, SetNifTrans, 1, kParams_OneString_ThreeFloats_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockRotation, GetNifRot, 1, kParams_OneString_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(SetNifBlockRotation, SetNifRot, 1, kParams_OneString_ThreeFloats_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockScale, GetNifScale, 1, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNifBlockScale, SetNifScale, 1, kParams_OneString_OneFloat_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockFlag, GetNifFlag, 1, kParams_OneString_OneInt_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(SetNifBlockFlag, SetNifFlag, 1, kParams_OneString_TwoInts_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetObjectVelocity, 1, kParams_OneOptionalAxis);
DEFINE_COMMAND_ALT_PLUGIN(GetAngularVelocity, GetAngVel, 1, kParams_OneString_OneOptionalAxis);
DEFINE_COMMAND_ALT_PLUGIN(SetAngularVelocity, SetAngVel, 1, kParams_OneString_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(PlaceAtCell, 0, kParams_OneForm_OneInt_OneForm_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetRayCastPos, 1, kParams_ThreeGlobals_OneOptFloat_OneOptInt_OneOptString_OneOptFloat);
DEFINE_COMMAND_ALT_PLUGIN(GetAnimSequenceFrequency, GetAnimSeqFreq, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(SetAnimSequenceFrequency, SetAnimSeqFreq, 1, kParams_OneString_OneFloat);
DEFINE_COMMAND_PLUGIN(MoveToNode, 1, kParams_OneObjectRef_OneString_ThreeOptionalFloats);
DEFINE_COMMAND_PLUGIN(GetPlayerPerks, 0, nullptr);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockParentNodes, GetNifParents, 1, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetCrosshairRefEx, 0, nullptr);
DEFINE_COMMAND_PLUGIN(IsMobile, 1, nullptr);
DEFINE_COMMAND_PLUGIN(IsGrabbable, 1, nullptr);
DEFINE_COMMAND_PLUGIN(AttachLight, 1, kParams_OneString_OneForm_ThreeOptionalFloats);
DEFINE_COMMAND_PLUGIN(RemoveLight, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetLinearVelocity, 1, kParams_OneString_ThreeFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(InsertNode, 0, kParams_OneForm_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(AttachModel, 0, kParams_OneForm_OneInt_OneFormatString);
DEFINE_COMMAND_ALT_PLUGIN(SynchronizePosition, SyncPos, 0, kParams_OneOptionalObjectRef_OneOptionalInt_OneOptionalFloat_OneOptionalString);
DEFINE_COMMAND_PLUGIN(ModelHasBlock, 0, kParams_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetRayCastRef, 1, kParams_OneOptionalInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetRayCastMaterial, 1, kParams_OneOptionalInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetCollisionNodes, 1, nullptr);
DEFINE_COMMAND_PLUGIN(GetChildBlocks, 1, kParams_OneOptionalString_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetBlockTextureSet, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetPosEx, 1, kParams_ThreeScriptVars);
DEFINE_COMMAND_PLUGIN(GetAngleEx, 1, kParams_ThreeScriptVars_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(SetTextureTransformKey, SetTexTransKey, 1, kParams_OneString_TwoInts_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(AttachExtraCamera, AttachCam, 1, kParams_OneString_OneInt_OneOptionalString_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(ProjectExtraCamera, ProjCam, 0, kParams_TwoStrings_OneFloat_ThreeOptionalInts);
DEFINE_COMMAND_PLUGIN(RenameNifBlock, 1, kParams_TwoStrings_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(RemoveNifBlock, 1, kParams_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(PlayAnimSequence, 1, kParams_OneString_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetTranslatedPos, 1, kParams_ThreeFloats_ThreeScriptVars);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockTranslationAlt, GetNifTransAlt, 1, kParams_OneString_ThreeScriptVars_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(GetNifBlockRotationAlt, GetNifRotAlt, 1, kParams_OneString_ThreeScriptVars_TwoOptionalInts);
DEFINE_COMMAND_ALT_PLUGIN(GetLinearVelocityAlt, GetLinVelAlt, 1, kParams_OneString_ThreeScriptVars_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(GetAngularVelocityAlt, GetAngVelAlt, 1, kParams_OneString_ThreeScriptVars_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(SetAngularVelocityEx, SetAngVelEx, 1, kParams_OneString_ThreeFloats_OneOptionalInt);
DEFINE_COMMAND_ALT_PLUGIN(GetCollisionObjProperty, GetCollProp, 1, kParams_OneString_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetCollisionObjProperty, SetCollProp, 1, kParams_OneString_OneInt_OneFloat);
DEFINE_COMMAND_ALT_PLUGIN(GetCollisionObjLayerType, GetCollLayer, 1, kParams_OneString);
DEFINE_COMMAND_ALT_PLUGIN(SetCollisionObjLayerType, SetCollLayer, 1, kParams_OneString_OneInt);
DEFINE_COMMAND_ALT_PLUGIN(SetRefrModelPath, SetRefModel, 1, kParams_OneOptionalString);
DEFINE_COMMAND_PLUGIN(AttachLine, 1, kParams_TwoStrings_FiveFloats);
DEFINE_COMMAND_PLUGIN(ToggleNoZPosReset, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(RotateAroundPoint, 1, kParams_SixFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(ToggleNodeCollision, 1, kParams_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(GetEditorPosition, 1, kParams_ThreeScriptVars_ThreeOptionalScriptVars);
DEFINE_COMMAND_PLUGIN(RefHasExtraData, 1, kParams_OneOptionalInt);
DEFINE_COMMAND_PLUGIN_EXP(GetRefExtraData, 1, kParams_NVSE_OneNum_OneOptionalNum);
DEFINE_COMMAND_PLUGIN_EXP(SetRefExtraData, 1, kParams_NVSE_OneOptionalNum_OneOptionalBasicType);
DEFINE_COMMAND_ALT_PLUGIN(TogglePurgeOnUnload, TPOU, 1, kParams_OneOptionalInt);

bool Cmd_SetPersistent_Execute(COMMAND_ARGS)
{
	UInt32 doSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &doSet) && (!doSet != !(thisObj->flags & TESObjectREFR::kFlags_Persistent)))
		ThisCall(0x565480, thisObj, doSet != 0);
	return true;
}

bool Cmd_GetObjectDimensions_Execute(COMMAND_ARGS)
{
	char axis;
	TESBoundObject *boundObj = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &boundObj)) return true;
	axis -= 'X';
	float scale = 1.0F;
	if (!boundObj)
	{
		if (!thisObj) return true;
		scale = thisObj->GetScale();
		if (BSBound *bounds = thisObj->GetBoundingBox())
		{
			*result = (bounds->centre[axis] + bounds->dimensions[axis]) * scale;
			return true;
		}
		boundObj = (TESBoundObject*)thisObj->baseForm;
	}
	*result = abs(float(boundObj->bounds[axis + 3] - boundObj->bounds[axis]) * scale);
	return true;
}

bool Cmd_GetIsItem_Execute(COMMAND_ARGS)
{
	TESForm *form = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj->baseForm;
	}
	*result = kInventoryType[form->typeID];
	return true;
}

bool Cmd_SetLinkedReference_Execute(COMMAND_ARGS)
{
	TESObjectREFR *linkRef = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &linkRef) && thisObj->SetLinkedRef(linkRef, scriptObj->GetOverridingModIdx()))
	{
		*result = 1;
		s_dataChangedFlags |= kChangedFlag_LinkedRefs;
	}
	return true;
}

bool Cmd_GetEnableChildren_Execute(COMMAND_ARGS)
{
	ExtraEnableStateChildren *xEnableChildren = GetExtraType(&thisObj->extraDataList, ExtraEnableStateChildren);
	if (!xEnableChildren) return true;
	TempElements *tmpElements = GetTempElements();
	auto iter = xEnableChildren->children.Head();
	do
	{
		if (iter->data) tmpElements->Append(iter->data);
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetLinkedChildren_Execute(COMMAND_ARGS)
{
	ExtraLinkedRefChildren *xLinkedChildren = GetExtraType(&thisObj->extraDataList, ExtraLinkedRefChildren);
	if (!xLinkedChildren) return true;
	TempElements *tmpElements = GetTempElements();
	auto iter = xLinkedChildren->children.Head();
	do
	{
		if (iter->data) tmpElements->Append(iter->data);
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_HasPrimitive_Execute(COMMAND_ARGS)
{
	if (auto xPrimitive = GetExtraType(&thisObj->extraDataList, ExtraPrimitive); xPrimitive && xPrimitive->primitive)
		*result = 1;
	return true;
}

bool Cmd_GetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
		if (ExtraPrimitive *xPrimitive = GetExtraType(&thisObj->extraDataList, ExtraPrimitive); xPrimitive && xPrimitive->primitive)
			*result = xPrimitive->primitive->bounds[axis - 'X'] * 2.0;
	return true;
}

bool Cmd_SetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis;
	float val;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &val))
		if (ExtraPrimitive *xPrimitive = GetExtraType(&thisObj->extraDataList, ExtraPrimitive); xPrimitive && xPrimitive->primitive)
		{
			axis -= 'X';
			val *= 0.5F;
			*result = xPrimitive->primitive->bounds[axis] * 2.0;
			if (xPrimitive->primitive->type == 2)
				xPrimitive->primitive->bounds = {val, val, val};
			else xPrimitive->primitive->bounds[axis] = val;
			thisObj->Update3D();
		}
	return true;
}

bool Cmd_AddPrimitive_Execute(COMMAND_ARGS)
{
	UInt32 type;
	NiVector3 bounds;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &bounds.x, &bounds.y, &bounds.z) && (type >= 1) && (type <= 3) &&
		((thisObj->baseForm->refID == 0x21) || (IS_ID(thisObj->baseForm, TESObjectACTI) && (type != 3))) &&
		!thisObj->extraDataList.HasType(kXData_ExtraPrimitive))
	{
		ExtraPrimitive *xPrimitive = ExtraPrimitive::Create();
		thisObj->extraDataList.AddExtra(xPrimitive);
		UInt32 size = (type == 1) ? 0x4C : 0x34;
		BGSPrimitive *primitive = (BGSPrimitive*)Game_HeapAlloc(size);
		ZERO_BYTES(primitive, size);
		*(UInt32*)primitive = (type == 1) ? kVtbl_BGSPrimitiveBox : ((type == 2) ? kVtbl_BGSPrimitiveSphere : kVtbl_BGSPrimitivePlane);
		primitive->type = type;
		bounds *= 0.5F;
		primitive->bounds.x = bounds.x;
		if (type == 2)
		{
			primitive->bounds.y = bounds.x;
			primitive->bounds.z = bounds.x;
		}
		else
		{
			primitive->bounds.y = (type == 1) ? bounds.y : 1.0F;
			primitive->bounds.z = bounds.z;
		}
		xPrimitive->primitive = primitive;
		thisObj->Update3D();
		*result = 1;
	}
	return true;
}

bool Cmd_GetTeammates_Execute(COMMAND_ARGS)
{
	TempElements *tmpElements = GetTempElements();
	auto iter = g_thePlayer->teammates.Head();
	do
	{
		if (iter->data) tmpElements->Append(iter->data);
	}
	while (iter = iter->next);
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_MoveToCell_Execute(COMMAND_ARGS)
{
	TESObjectCELL *cell;
	NiVector3 posVector;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &cell, &posVector.x, &posVector.y, &posVector.z))
	{
		if NOT_ID(cell, TESObjectCELL)
		{
			if NOT_ID(cell, TESWorldSpace)
				return true;
			cell = ((TESWorldSpace*)cell)->cell;
		}
		thisObj->MoveToCell(cell, posVector);
		*result = 1;
	}
	return true;
}

bool Cmd_MoveToEditorPosition_Execute(COMMAND_ARGS)
{
	UInt32 resetRot = 0;
	if (NUM_ARGS_EX)
		ExtractArgsEx(EXTRACT_ARGS_EX, &resetRot);
	TESObjectCELL *cell;
	NiVector3 *posVector;
	NiVector4 rotVector(_mm_setzero_ps());
	if (IS_ACTOR(thisObj))
	{
		Actor *actor = (Actor*)thisObj;
		cell = (TESObjectCELL*)actor->startingWorldOrCell;
		posVector = &actor->startingPos;
		if (resetRot)
			rotVector.z = actor->startingZRot * Flt180dPI;
	}
	else
	{
		ExtraStartingPosition *xStartingPos = GetExtraType(&thisObj->extraDataList, ExtraStartingPosition);
		if (!xStartingPos) return true;
		ExtraStartingWorldOrCell *xStartingCell = GetExtraType(&thisObj->extraDataList, ExtraStartingWorldOrCell);
		cell = xStartingCell ? (TESObjectCELL*)xStartingCell->worldOrCell : thisObj->GetParentCell();
		posVector = &xStartingPos->posVector;
		if (resetRot)
		{
			rotVector = xStartingPos->rotVector;
			rotVector *= Flt180dPI;
		}
	}
	if (!cell) return true;
	if NOT_ID(cell, TESObjectCELL)
	{
		if NOT_ID(cell, TESWorldSpace)
			return true;
		cell = ((TESWorldSpace*)cell)->cell;
	}
	thisObj->MoveToCell(cell, *posVector);
	if (resetRot)
		thisObj->SetAngle(rotVector * GET_PS(8), 0);
	*result = 1;
	return true;
}

bool Cmd_GetCenterPos_Execute(COMMAND_ARGS)
{
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
	{
		axis -= 'X';
		float cntPos = 0;
		if (BSBound *bounds = thisObj->GetBoundingBox())
			cntPos = bounds->centre[axis];
		else
		{
			TESBoundObject *object = (TESBoundObject*)thisObj->baseForm;
			if (object->IsBoundObject())
				cntPos = (object->bounds[axis] + object->bounds[axis + 3]) * 0.5F;
		}
		*result = thisObj->position[axis] + (cntPos * thisObj->GetScale());
	}
	return true;
}

bool Cmd_GetRefType_Execute(COMMAND_ARGS)
{
	TESObjectREFR *objRefr = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &objRefr) && (objRefr || (objRefr = thisObj)))
	{
		*result = objRefr->typeID;
		DoConsolePrint(result);
	}
	return true;
}

__declspec(naked) void __fastcall ToggleObjectCollision(TESObjectREFR *thisObj, bool enable)
{
	__asm
	{
		test	byte ptr [ecx+0x5F], kHookRefFlag5F_DisableCollision
		setz	al
		cmp		al, dl
		jz		done
		xor		byte ptr [ecx+0x5F], kHookRefFlag5F_DisableCollision
		jmp		TESObjectREFR::ToggleCollision
	done:
		retn
	}
}

bool Cmd_ToggleObjectCollision_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &enable))
		ToggleObjectCollision(thisObj, enable != 0);
	return true;
}

bool Cmd_GetMaterialPropertyValue_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 traitID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &traitID) && (traitID <= 8))
		if (NiAVObject *block = thisObj->GetNiBlock(blockName); block && IS_GEOMETRY(block))
			if (NiMaterialProperty *matProp = ((NiGeometry*)block)->materialProp)
				switch (traitID)
				{
					case 0:
					case 1:
					case 2:
						*result = matProp->specularRGB[traitID];
						break;
					case 3:
					case 4:
					case 5:
						*result = matProp->emissiveRGB[traitID - 3];
						break;
					case 6:
						*result = matProp->glossiness;
						break;
					case 7:
						*result = matProp->alpha;
						break;
					default:
						*result = matProp->emitMult;
				}
	return true;
}

bool Cmd_SetMaterialPropertyValue_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 traitID;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &traitID, &value) && (traitID <= 8) && (value >= 0))
		if (NiNode *niNode = thisObj->GetNiNode())
			if (*(UInt16*)blockName == '*')
				niNode->SetMaterialPropValueRecurse(traitID, value);
			else if (NiAVObject *block = niNode->GetBlock(blockName); block && IS_GEOMETRY(block))
				if (NiMaterialProperty *matProp = ((NiGeometry*)block)->materialProp)
					matProp->SetTraitValue(traitID, value);
	return true;
}

__declspec(noinline) void __fastcall PopulateFormSet(TESForm *form, TempFormList *tmpFormLst)
{
	if IS_ID(form, BGSListForm)
	{
		auto traverse = ((BGSListForm*)form)->list.Head();
		do
		{
			tmpFormLst->Insert(traverse->data);
		}
		while (traverse = traverse->next);
	}
	else tmpFormLst->Insert(form);
}

void __fastcall GetRefrContactObjects(TESObjectREFR *thisObj, ContactObjects &contactObjs)
{
	if (TESObjectREFR::RenderState *renderState = thisObj->renderState)
		if (NiNode *niNode = renderState->rootNode)
		{
			if (renderState->phantom && IS_TYPE(renderState->phantom, bhkSimpleShapePhantom))
				for (auto iter = ((hkpSimpleShapePhantom*)renderState->phantom->refObject)->cdBodies.Begin(); iter; ++iter)
					contactObjs.Append(iter->GetWorldObj());
			niNode->GetContactObjects(contactObjs);
		}
}

bool Cmd_GetHasContact_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		TempFormList *tmpFormLst = GetTempFormList();
		PopulateFormSet(form, tmpFormLst);
		hkpWorldObject **bodies;
		UInt32 count;
		TESObjectREFR *refr;
		if (IS_ACTOR(thisObj))
		{
			bhkCharacterController *charCtrl = thisObj->GetCharacterController();
			if (!charCtrl) return true;
			if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
			{
				refr = charCtrl->bodyUnderFeet->GetParentRef();
				if (refr && tmpFormLst->HasKey(refr))
				{
					*result = 1;
					return true;
				}
			}
			bodies = charCtrl->pointCollector.contactBodies.data;
			count = charCtrl->pointCollector.contactBodies.size;
		}
		else
		{
			ContactObjects contactObjs;
			GetRefrContactObjects(thisObj, contactObjs);
			bodies = contactObjs.Data();
			count = contactObjs.Size();
		}
		for (; count; count--, bodies++)
		{
			refr = (*bodies)->GetParentRef();
			if (refr && tmpFormLst->HasKey(refr))
			{
				*result = 1;
				return true;
			}
		}
	}
	return true;
}

bool Cmd_GetHasContactBase_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
	{
		TempFormList *tmpFormLst = GetTempFormList();
		PopulateFormSet(form, tmpFormLst);
		hkpWorldObject **bodies;
		UInt32 count;
		TESObjectREFR *refr;
		if (IS_ACTOR(thisObj))
		{
			bhkCharacterController *charCtrl = thisObj->GetCharacterController();
			if (!charCtrl) return true;
			if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
			{
				refr = charCtrl->bodyUnderFeet->GetParentRef();
				if (refr && tmpFormLst->HasKey(refr->baseForm))
				{
					*result = 1;
					return true;
				}
			}
			bodies = charCtrl->pointCollector.contactBodies.data;
			count = charCtrl->pointCollector.contactBodies.size;
		}
		else
		{
			ContactObjects contactObjs;
			GetRefrContactObjects(thisObj, contactObjs);
			bodies = contactObjs.Data();
			count = contactObjs.Size();
		}
		for (; count; count--, bodies++)
		{
			refr = (*bodies)->GetParentRef();
			if (refr && tmpFormLst->HasKey(refr->baseForm))
			{
				*result = 1;
				return true;
			}
		}
	}
	return true;
}

__declspec(noinline) bool __stdcall GetHasContactType(TESObjectREFR *thisObj, UInt32 typeID)
{
	hkpWorldObject **bodies;
	UInt32 count;
	TESObjectREFR *refr;
	if (IS_ACTOR(thisObj))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (!charCtrl) return false;
		if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
		{
			refr = charCtrl->bodyUnderFeet->GetParentRef();
			if (refr && (refr->baseForm->typeID == typeID))
				return true;
		}
		bodies = charCtrl->pointCollector.contactBodies.data;
		count = charCtrl->pointCollector.contactBodies.size;
	}
	else
	{
		ContactObjects contactObjs;
		GetRefrContactObjects(thisObj, contactObjs);
		bodies = contactObjs.Data();
		count = contactObjs.Size();
	}
	for (; count; count--, bodies++)
	{
		refr = (*bodies)->GetParentRef();
		if (refr && (refr->baseForm->typeID == typeID))
			return true;
	}
	return false;
}

bool Cmd_GetHasContactType_Execute(COMMAND_ARGS)
{
	UInt32 typeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &typeID) && GetHasContactType(thisObj, typeID))
		*result = 1;
	return true;
}

bool Cmd_GetHasContactType_Eval(COMMAND_ARGS_EVAL)
{
	if (GetHasContactType(thisObj, (UInt32)arg1))
		*result = 1;
	return true;
}

bool Cmd_GetContactRefs_Execute(COMMAND_ARGS)
{
	TempElements *tmpElements = GetTempElements();
	hkpWorldObject **bodies;
	UInt32 count;
	TESObjectREFR *refr;
	if (IS_ACTOR(thisObj))
	{
		bhkCharacterController *charCtrl = thisObj->GetCharacterController();
		if (!charCtrl) return true;
		if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
		{
			refr = charCtrl->bodyUnderFeet->GetParentRef();
			if (refr) tmpElements->Append(refr);
		}
		bodies = charCtrl->pointCollector.contactBodies.data;
		count = charCtrl->pointCollector.contactBodies.size;
	}
	else
	{
		ContactObjects contactObjs;
		GetRefrContactObjects(thisObj, contactObjs);
		bodies = contactObjs.Data();
		count = contactObjs.Size();
	}
	for (; count; count--, bodies++)
	{
		refr = (*bodies)->GetParentRef();
		if (refr) tmpElements->InsertUnique(refr);
	}
	if (!tmpElements->Empty())
		*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	return true;
}

bool Cmd_GetHasPhantom_Execute(COMMAND_ARGS)
{
	if (TESObjectREFR::RenderState *renderState = thisObj->renderState)
		if (NiNode *niNode = renderState->rootNode; niNode &&
			(IS_ACTOR(thisObj) || (renderState->phantom && IS_TYPE(renderState->phantom, bhkSimpleShapePhantom)) || niNode->HasPhantom()))
			*result = 1;
	return true;
}

bool Cmd_GetInteractionDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form = nullptr;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (form || (form = thisObj)) && (form->jipFormFlags6 & kHookFormFlag6_NoPCActivation))
		*result = 1;
	return true;
}

bool Cmd_SetInteractionDisabled_Execute(COMMAND_ARGS)
{
	UInt32 disable;
	TESForm *form = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &disable, &form)) return true;
	if (!form)
	{
		if (!thisObj) return true;
		form = thisObj;
	}
	bool add = (disable != 0);
	if (add == !(form->jipFormFlags6 & kHookFormFlag6_NoPCActivation))
	{
		form->SetJIPFlag(kHookFormFlag6_NoPCActivation, add);
		HOOK_MOD(InteractionDisabled, add);
	}
	return true;
}

bool Cmd_GetInteractionDisabledType_Execute(COMMAND_ARGS)
{
	UInt32 typeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &typeID) && (typeID < kFormType_Max) && kValidFilterForm[typeID] && s_activationDisabledTypes[typeID])
		*result = 1;
	return true;
}

bool Cmd_SetInteractionDisabledType_Execute(COMMAND_ARGS)
{
	UInt32 disable, typeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &disable, &typeID) && (typeID < kFormType_Max) && kValidFilterForm[typeID] && (!disable == s_activationDisabledTypes[typeID]))
	{
		bool add = (disable != 0);
		s_activationDisabledTypes[typeID] = add;
		HOOK_MOD(InteractionDisabled, add);
	}
	return true;
}

bool Cmd_AddRefMapMarker_Execute(COMMAND_ARGS)
{
	if (!thisObj->ValidForHooks()) return true;
	if (!s_refMapMarkersCount)
		s_refMapMarkersList.Head()->next = g_thePlayer->teammates.Head();
	else if (s_refMapMarkersList.IsInList(thisObj)) return true;
	s_refMapMarkersList.Prepend(thisObj);
	s_refMapMarkersCount++;
	HOOK_INC(CreateMapMarkers);
	return true;
}

bool Cmd_RemoveRefMapMarker_Execute(COMMAND_ARGS)
{
	SInt32 idx = s_refMapMarkersList.GetIndexOf(thisObj);
	if ((idx == -1) || (idx >= s_refMapMarkersCount)) return true;
	if (--s_refMapMarkersCount) s_refMapMarkersList.RemoveNth(idx);
	else s_refMapMarkersList.Init();
	HOOK_DEC(CreateMapMarkers);
	return true;
}

bool Cmd_RefHasMapMarker_Execute(COMMAND_ARGS)
{
	if (s_refMapMarkersCount)
	{
		SInt32 idx = s_refMapMarkersList.GetIndexOf(thisObj);
		if ((idx >= 0) && (idx < s_refMapMarkersCount))
		{
			*result = 1;
			return true;
		}
	}
	return true;
}

bool Cmd_SetPosEx_Execute(COMMAND_ARGS)
{
	NiVector3 posVector;
	UInt32 transform = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posVector.x, &posVector.y, &posVector.z, &transform))
		if (!transform)
			thisObj->SetPos(posVector);
		else if (transform == 1)
			thisObj->SetPos(posVector.SetPS(thisObj->GetTranslatedPos(posVector)));
		else if (transform == 2)
			thisObj->SetPos(posVector += thisObj->position.PS());
	return true;
}

bool Cmd_MoveToReticle_Execute(COMMAND_ARGS)
{
	float maxRange = 50000.0F;
	NiVector4 posMods(_mm_setzero_ps());
	UInt32 translate = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &maxRange, &posMods.x, &posMods.y, &posMods.z, &translate))
		if (TESObjectCELL *cell = g_thePlayer->parentCell)
			if (NiVector4 coords; coords.RayCastCoords(g_mainCamera->WorldTranslate(), g_mainCamera->WorldRotate(), maxRange, 6) || translate)
			{
				if (cell->worldSpace)
					cell = cell->worldSpace->cell;
				coords += posMods;
				thisObj->MoveToCell(cell, coords);
				*result = 1;
			}
	return true;
}

bool Cmd_SetRefName_Execute(COMMAND_ARGS)
{
	char name[0x80];
	name[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &name))
		if (name[0])
		{
			char **namePtr;
			if (s_refNamesMap->InsertKey(thisObj, &namePtr))
				HOOK_INC(GetRefName);
			else FreeStringKey(*namePtr);
			*namePtr = CopyStringKey(name);
			thisObj->JIPRefFlags() |= kHookRefFlag5F_AltRefName;
		}
		else
		{
			if (char *pName = s_refNamesMap->GetErase(thisObj))
			{
				FreeStringKey(pName);
				HOOK_DEC(GetRefName);
			}
			thisObj->JIPRefFlags() &= ~kHookRefFlag5F_AltRefName;
		}
	return true;
}

bool Cmd_GetAngleEx_Execute(COMMAND_ARGS)
{
	ResultVars outRot;
	UInt32 getLocal = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outRot.x, &outRot.y, &outRot.z, &getLocal))
	{
		__m128 angles;
		if (!getLocal)
			angles = thisObj->rotation.PS();
		else
		{
			NiNode *rootNode = thisObj->GetRefNiNode();
			if (rootNode && !thisObj->IsPlayer())
				angles = rootNode->WorldRotate().ToEulerPRYInv();
			else
			{
				NiMatrix33 rotMat(thisObj->rotation);
				angles = rotMat.ToEulerPRYInv();
			}
		}
		outRot.Set(angles, GET_PS(9));
	}
	return true;
}

bool Cmd_SetAngleEx_Execute(COMMAND_ARGS)
{
	NiVector3 rotVector;
	UInt32 transform = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &rotVector.x, &rotVector.y, &rotVector.z, &transform))
		switch (transform)
		{
			default:
			case 0:
			case 1:
				thisObj->SetAngle(rotVector * GET_PS(8), transform);
				break;
			case 2:
				thisObj->Rotate(rotVector * GET_PS(8));
				break;
			case 3:
			{
				NiMatrix33 rotMat, *pRotMat = &rotMat;
				NiNode *rootNode = thisObj->GetRefNiNode();
				if (rootNode && !thisObj->IsPlayer())
					pRotMat = &rootNode->WorldRotate();
				else
					rotMat = thisObj->rotation;
				thisObj->SetAngle((rotVector * GET_PS(8)) + pRotMat->ToEulerPRYInv(), 1);
				break;
			}
			case 4:
				thisObj->SetAngle(rotVector.PS(), 2);
				break;
		}
	return true;
}

bool Cmd_GetTeleportDoor_Execute(COMMAND_ARGS)
{
	if (thisObj->baseForm && IS_ID(thisObj->baseForm, TESObjectDOOR))
		if (ExtraTeleport *xTeleport = GetExtraType(&thisObj->extraDataList, ExtraTeleport); xTeleport && xTeleport->data && xTeleport->data->linkedDoor)
			REFR_RES = xTeleport->data->linkedDoor->refID;
	return true;
}

bool Cmd_SetOnCriticalHitEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	Actor *target = nullptr;
	TESObjectREFR *source = nullptr;
	TESObjectWEAP *weapon = nullptr;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &target, &source, &weapon) || NOT_ID(script, Script)) return true;
	if (target)
	{
		if (NOT_ACTOR(target))
		{
			if (!source) return true;
			target = nullptr;
		}
		if (source)
		{
			if NOT_REFERENCE(source)
			{
				if (!weapon) return true;
				source = nullptr;
			}
			if (weapon && NOT_ID(weapon, TESObjectWEAP))
				return true;
		}
	}
	CriticalHitEventData evntData(target, source, weapon, script);
	if (addEvnt && s_criticalHitEvents->Find(CriticalHitEventFind(evntData)))
		return true;
	s_criticalHitEvents->Remove(CriticalHitEventRemove(evntData));
	if (addEvnt)
		*s_criticalHitEvents->Append() = evntData;
	return true;
}

bool Cmd_MoveToFadeDelay_Execute(COMMAND_ARGS)
{
	TESObjectREFR *destRef;
	float delayTime;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &destRef, &delayTime))
	{
		if IS_ACTOR(thisObj)
			if (HighProcess *hiProcess = (HighProcess*)((Actor*)thisObj)->baseProcess; hiProcess && !hiProcess->processLevel)
			{
				hiProcess->delayTime = delayTime;
				ThisCall(0x8FE9E0, hiProcess, thisObj, destRef, 0, 0, 0);
				return true;
			}
		CdeclCall(ADDR_MoveToMarker, thisObj, destRef, 0, 0, 0);
	}
	return true;
}

bool Cmd_GetCrosshairWater_Execute(COMMAND_ARGS)
{
	TESWaterForm *water = nullptr;
	TESObjectREFR *refr = g_interfaceManager->crosshairRef;
	if (refr && IS_ID(refr->baseForm, BGSPlaceableWater))
		water = ((BGSPlaceableWater*)refr->baseForm)->water;
	if (water) REFR_RES = water->refID;
	DoConsolePrint(water);
	return true;
}

bool Cmd_IsAnimPlayingEx_Execute(COMMAND_ARGS)
{
	UInt32 category, subType = 0, flags = 0, pcPOV = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &category, &subType, &flags, &pcPOV))
	{
		AnimData *animData = nullptr;
		if (pcPOV && thisObj->IsPlayer())
		{
			PlayerCharacter *thePlayer = (PlayerCharacter*)thisObj;
			if (!thePlayer->baseProcess || thePlayer->baseProcess->processLevel)
				return true;
			animData = (pcPOV == 1) ? thePlayer->animData1stPerson : ((HighProcess*)thePlayer->baseProcess)->animData;
		}
		else animData = thisObj->GetAnimData();
		if (animData)
		{
			for (UInt16 groupID : animData->animGroupIDs)
			{
				UInt32 animID = groupID & 0xFF;
				if (animID >= 245) continue;
				const AnimGroupClassify *classify = &kAnimGroupClassify[animID];
				if ((classify->category == category) && ((category >= 4) || ((!subType || (classify->subType == subType)) && (!flags || (classify->flags & flags)))))
				{
					*result = 1;
					break;
				}
			}
		}
	}
	return true;
}

bool Cmd_GetRigidBodyMass_Execute(COMMAND_ARGS)
{
	if (NOT_ACTOR(thisObj))
		if (NiNode *niNode = thisObj->GetRefNiNode())
			*result = niNode->GetBodyMass(0);
	return true;
}

bool Cmd_PushObject_Execute(COMMAND_ARGS)
{
	NiVector4 forceVector;
	TESObjectREFR *refr = nullptr;
	if (NOT_ACTOR(thisObj) && ExtractArgsEx(EXTRACT_ARGS_EX, &forceVector.x, &forceVector.y, &forceVector.z, &forceVector.w, &refr))
		if (NiNode *niNode = thisObj->GetRefNiNode())
		{
			if (!refr) refr = thisObj;
			forceVector += refr->position;
			niNode->ApplyForce(forceVector);
		}
	return true;
}

bool Cmd_MoveToContainer_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	UInt32 clrOwner = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &target, &clrOwner) && kInventoryType[thisObj->baseForm->typeID] && target->baseForm->GetContainer())
	{
		if (thisObj == g_HUDMainMenu->crosshairRef)
			CdeclCall(0x775A00, 0, 0, 0);
		if (clrOwner) thisObj->extraDataList.RemoveByType(kXData_ExtraOwnership);
		if (target->IsPlayer())
			ThisCall(0x953FF0, target, thisObj, 1, 0);
		else
		{
			ThisCall(0x572230, thisObj);
			ThisCall(0x574B30, target, thisObj, 1, 0, 0);
			if (!(thisObj->flags & 1) && thisObj->mods.Empty() && !ThisCall<bool>(0x41B3A0, &thisObj->extraDataList, 2))
				thisObj->Destroy(true);
		}
	}
	return true;
}

__declspec(naked) NiAVObject* __fastcall GetNifBlock(TESObjectREFR *thisObj, UInt32 pcNode, char *blockName)
{
	__asm
	{
		test	dl, dl
		jz		notPlayer
		cmp		dword ptr [ecx+0xC], 0x14
		jnz		notPlayer
		test	dl, 1
		jz		get1stP
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		jmp		gotRoot
	get1stP:
		mov		eax, [ecx+0x694]
		jmp		gotRoot
	notPlayer:
		call	TESObjectREFR::GetNiNode
	gotRoot:
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		cmp		[edx], 0
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
	done:
		retn	4
	}
}

bool Cmd_GetNifBlockTranslation_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 getWorld = 0, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &getWorld, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			NiVector3 &transltn = getWorld ? niBlock->WorldTranslate() : niBlock->LocalTranslate();
			ArrayElementL elements[3] = {transltn.x, transltn.y, transltn.z};
			*result = (int)CreateArray(elements, 3, scriptObj);
		}
	return true;
}

bool Cmd_SetNifBlockTranslation_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	NiVector3 transltn;
	UInt32 pcNode = 0, transform = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &transltn.x, &transltn.y, &transltn.z, &pcNode, &transform) && blockName[0])
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			if (!transform)
				niBlock->LocalTranslate() = transltn;
			else if (transform == 1)
				niBlock->LocalTranslate() += transltn.PS();
			else
				niBlock->LocalTranslate() += niBlock->WorldRotate().MultiplyVector(transltn.PS());
			if IS_NODE(niBlock)
			{
				if NOT_ACTOR(thisObj)
					((NiNode*)niBlock)->ResetCollision();
			}
			else if IS_TYPE(niBlock, NiPointLight)
				if (NiPointLight *ptLight = (NiPointLight*)niBlock; ptLight->extraFlags & 1)
					ptLight->vector100 = transltn;
			niBlock->UpdateDownwardPass(kNiUpdateData, 0);
		}
	return true;
}

bool Cmd_GetNifBlockRotation_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 getMode = 0, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &getMode, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			NiVector4 rot;
			__asm
			{
				mov		ecx, niBlock
				lea		eax, [ecx+0x68]
				add		ecx, 0x34
				test	getMode, 1
				cmovnz	ecx, eax
				mov		eax, NiMatrix33::ToEulerPRY
				mov		edx, NiMatrix33::ToEulerPRYInv
				test	getMode, 2
				cmovnz	eax, edx
				call	eax
				mulps	xmm0, PS_V3_180dPI
				movups	rot, xmm0
			}
			ArrayElementL elements[3] = {rot.x, rot.y, rot.z};
			*result = (int)CreateArray(elements, 3, scriptObj);
		}
	return true;
}

bool Cmd_SetNifBlockRotation_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	NiVector3 rot;
	UInt32 transform = 0, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &rot.x, &rot.y, &rot.z, &transform, &pcNode) && blockName[0])
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			switch (transform)
			{
				default:
				case 0:
					niBlock->LocalRotate().FromEulerPRY(rot * GET_PS(8));
					break;
				case 1:
					niBlock->LocalRotate().Rotate(rot * GET_PS(8));
					break;
				case 2:
				case 3:
					niBlock->LocalRotate().FromEulerPRYInv(rot * GET_PS(8));
					break;
				case 4:
					niBlock->LocalRotate().FromEulerPRYInv((rot * GET_PS(8)) + niBlock->WorldRotate().ToEulerPRYInv());
					niBlock->LocalRotate().MultiplyMatricesInv(niBlock->m_parent->WorldRotate());
					break;
				case 5:
					niBlock->LocalRotate().From2Points(niBlock->WorldTranslate().PS(), rot.PS());
					niBlock->LocalRotate().MultiplyMatricesInv(niBlock->m_parent->WorldRotate());
					break;
			}
			if (IS_NODE(niBlock) && NOT_ACTOR(thisObj))
				((NiNode*)niBlock)->ResetCollision();
			niBlock->UpdateDownwardPass(kNiUpdateData, 0);
		}
	return true;
}

bool Cmd_GetNifBlockScale_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
			*result = niBlock->m_transformLocal.scale;
	return true;
}

bool Cmd_SetNifBlockScale_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	float newScale;
	UInt32 pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &newScale, &pcNode) && blockName[0])
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			niBlock->m_transformLocal.scale = newScale;
			if (IS_NODE(niBlock) && NOT_ACTOR(thisObj))
				((NiNode*)niBlock)->ResetCollision();
			niBlock->UpdateDownwardPass(kNiUpdateData, 0);
		}
	return true;
}

bool Cmd_GetNifBlockFlag_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 flagID, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &flagID, &pcNode) && (flagID <= 31))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName); niBlock && (niBlock->m_flags & (1 << flagID)))
			*result = 1;
	return true;
}

bool Cmd_SetNifBlockFlag_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 flagID, doSet, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &flagID, &doSet, &pcNode) && (flagID <= 26))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			if (doSet) niBlock->m_flags |= (1 << flagID);
			else niBlock->m_flags &= ~(1 << flagID);
		}
	return true;
}

bool Cmd_GetObjectVelocity_Execute(COMMAND_ARGS)
{
	char axis = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(""); rigidBody && rigidBody->IsMobile())
			if (axis)
				*result = rigidBody->motion.linVelocity[axis - 'X'];
			else
				*result = Length_V4(rigidBody->motion.linVelocity.PS());
	return true;
}

bool Cmd_GetAngularVelocity_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	char axis = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &axis))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName); rigidBody && rigidBody->IsMobile())
		{
			__asm
			{
				mov		ecx, rigidBody
				movaps	xmm0, [ecx+0x1C0]
				andps	xmm0, PS_XYZ0Mask
				xorps	xmm1, xmm1
				xor		eax, eax
				mov		al, axis
				sub		al, 'X'
				js		getNet
				shl		al, 4
				mulps	xmm0, [ecx+eax+0xF0]
				haddps	xmm0, xmm1
				haddps	xmm0, xmm1
				jmp		done
			getNet:
				mulps	xmm0, xmm0
				haddps	xmm0, xmm1
				haddps	xmm0, xmm1
				sqrtss	xmm0, xmm0
			done:
				cvtps2pd	xmm0, xmm0
				mov		eax, result
				movlpd	[eax], xmm0
			}
		}
	return true;
}

bool Cmd_SetAngularVelocity_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	char axis;
	float velocity;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &axis, &velocity))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName); rigidBody && rigidBody->IsMobile())
		{
			__asm
			{
				movss	xmm0, velocity
				shufps	xmm0, xmm0, 0x40
				xor		eax, eax
				mov		al, axis
				sub		al, 'X'
				shl		al, 4
				mov		ecx, rigidBody
				mulps	xmm0, [ecx+eax+0xF0]
				movaps	[ecx+0x1C0], xmm0
				CALL_EAX(0xC9C1D0)
			}
		}
	return true;
}

bool Cmd_PlaceAtCell_Execute(COMMAND_ARGS)
{
	TESForm *form;
	UInt32 count;
	TESObjectCELL *worldOrCell;
	TESObjectREFR *tempPosMarker = s_tempPosMarker;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &worldOrCell, &tempPosMarker->position.x, &tempPosMarker->position.y, &tempPosMarker->position.z))
	{
		if NOT_ID(worldOrCell, TESObjectCELL)
		{
			if NOT_ID(worldOrCell, TESWorldSpace)
				return true;
			worldOrCell = ((TESWorldSpace*)worldOrCell)->cell;
		}
		tempPosMarker->parentCell = worldOrCell;
		if (TESObjectREFR *placedRef = PlaceAtMe(tempPosMarker, form, count, 0, 0, 1))
			REFR_RES = placedRef->refID;
	}
	return true;
}

bool Cmd_GetRayCastPos_Execute(COMMAND_ARGS)
{
	TESGlobal *outX, *outY, *outZ;
	float posZmod, maxRange = 50000.0F;
	SInt32 layerType = 6;
	char nodeName[0x40];
	nodeName[0] = 0;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY, &outZ, &posZmod, &layerType, &nodeName, &maxRange))
		if (NiNode *objNode = thisObj->GetNode(nodeName))
		{
			NiVector4 coords;
			if (coords.RayCastCoords(objNode->WorldTranslate(), objNode->WorldRotate() + 1, maxRange, layerType) || (numArgs >= 7))
			{
				outX->data = coords.x;
				outY->data = coords.y;
				outZ->data = coords.z;
				*result = 1;
			}
		}
	return true;
}

bool Cmd_GetAnimSequenceFrequency_Execute(COMMAND_ARGS)
{
	*result = -1;
	char seqName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &seqName))
		if (NiNode *rootNode = thisObj->GetNiNode(); rootNode && (NOT_ACTOR(thisObj) || (((Actor*)thisObj)->lifeState != 1)))
			if (NiControllerManager *ctrlMgr = (NiControllerManager*)rootNode->m_controller; ctrlMgr && IS_TYPE(ctrlMgr, NiControllerManager))
				if (NiControllerSequence *sequence = ctrlMgr->FindSequence(seqName))
					*result = sequence->frequency;
	return true;
}

bool Cmd_SetAnimSequenceFrequency_Execute(COMMAND_ARGS)
{
	char seqName[0x40];
	float frequency;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &seqName, &frequency))
		if (NiNode *rootNode = thisObj->GetNiNode(); rootNode && (NOT_ACTOR(thisObj) || (((Actor*)thisObj)->lifeState != 1)))
			if (NiControllerManager *ctrlMgr = (NiControllerManager*)rootNode->m_controller; ctrlMgr && IS_TYPE(ctrlMgr, NiControllerManager))
				if (*(UInt16*)seqName == '*')
					for (auto iter = ctrlMgr->sequences.Begin(); iter; ++iter)
						iter->frequency = frequency;
				else if (NiControllerSequence *sequence = ctrlMgr->FindSequence(seqName))
					sequence->frequency = frequency;
	return true;
}

bool Cmd_MoveToNode_Execute(COMMAND_ARGS)
{
	TESObjectREFR *targetRef;
	char nodeName[0x40];
	NiVector4 posMods(_mm_setzero_ps());
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &targetRef, &nodeName, &posMods.x, &posMods.y, &posMods.z))
		if (TESObjectCELL *cell = targetRef->GetParentCell())
			if (NiNode *targetNode = targetRef->GetNode(nodeName))
			{
				posMods += targetNode->WorldTranslate();
				thisObj->MoveToCell(cell, posMods);
				*result = 1;
			}
	return true;
}

bool Cmd_GetPlayerPerks_Execute(COMMAND_ARGS)
{
	*result = (int)GetAllPerks(g_thePlayer, 0, scriptObj, 0);
	return true;
}

bool Cmd_GetNifBlockParentNodes_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &pcNode))
	{
		NiNode *rootNode = thisObj->GetRefNiNode();
		if (thisObj->IsPlayer() && !(pcNode & 1) && !((PlayerCharacter*)thisObj)->is3rdPerson)
			rootNode = ((PlayerCharacter*)thisObj)->node1stPerson;
		if (rootNode)
			if (NiAVObject *niBlock = rootNode->GetBlock(blockName))
			{
				TempElements *tmpElements = GetTempElements();
				while ((niBlock != rootNode) && (niBlock = niBlock->m_parent))
					tmpElements->Append(niBlock->GetName());
				*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
			}
	}
	return true;
}

__declspec(naked) TESObjectREFR *GetCrosshairRef()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x10
		mov		ecx, g_thePlayer
		movups	xmm0, [ecx+0xDD4]
		cmp		byte ptr [ecx+0x64B], 0
		jz		firstPerson
		movups	xmm1, [ecx+0xD58]
		addps	xmm0, xmm1
	firstPerson:
		lea		edx, [ebp-0x10]
		movups	[edx], xmm0
		mov		eax, [ecx+0x64]
		mov		ecx, [eax+0x14]
		push	dword ptr [ecx+0x84]
		push	dword ptr [ecx+0x78]
		push	dword ptr [ecx+0x6C]
		lea		ecx, [ebp-0x1C]
		lea		eax, [ebp-4]
		push	eax
		push	eax
		push	0x46400000
		push	ecx
		push	edx
		mov		ecx, g_interfaceManager
		mov		ecx, [ecx+0x13C]
		CALL_EAX(0x631D60)
		leave
		retn
	}
}

bool Cmd_GetCrosshairRefEx_Execute(COMMAND_ARGS)
{
	TESObjectREFR *resRefr = g_interfaceManager->crosshairRef;
	if (!resRefr)
	{
		resRefr = GetCrosshairRef();
		if (!resRefr) return true;
	}
	REFR_RES = resRefr->refID;

	/*if (s_crosshairRefEx)
		REFR_RES = s_crosshairRefEx->refID;*/
	return true;
}

bool Cmd_IsMobile_Execute(COMMAND_ARGS)
{
	if (thisObj->IsMobile())
		*result = 1;
	return true;
}

bool Cmd_IsGrabbable_Execute(COMMAND_ARGS)
{
	if (thisObj->IsGrabbable())
		*result = 1;
	return true;
}

bool Cmd_AttachLight_Execute(COMMAND_ARGS)
{
	char nodeName[0x40];
	TESObjectLIGH *lightForm;
	NiVector4 offsetMod(_mm_setzero_ps());
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName, &lightForm, &offsetMod.x, &offsetMod.y, &offsetMod.z) && IS_ID(lightForm, TESObjectLIGH))
		if (NiNode *objNode = thisObj->GetNode2(nodeName))
		{
			NiPointLight *pointLight = CreatePointLight(lightForm, objNode);
			pointLight->LocalTranslate() = offsetMod;
			pointLight->extraFlags |= 0x80;
			*result = 1;
		}
	return true;
}

bool Cmd_RemoveLight_Execute(COMMAND_ARGS)
{
	char nodeName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName))
		if (NiNode *objNode = thisObj->GetNode2(nodeName))
		{
			NiPointLight *pointLight;
			for (auto iter = objNode->m_children.Begin(); iter; ++iter)
			{
				if (!(pointLight = (NiPointLight*)*iter) || NOT_TYPE(pointLight, NiPointLight) || !(pointLight->extraFlags & 0x80))
					continue;
				objNode->RemoveObject(pointLight);
				break;
			}
		}
	return true;
}

bool Cmd_SetLinearVelocity_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	NiVector4 velocity(_mm_setzero_ps());
	UInt32 flags = 0;	//	1 - set local; 2 - add value
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &velocity.x, &velocity.y, &velocity.z, &flags))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName); rigidBody && rigidBody->IsMobile())
		{
			rigidBody->SetVelocity(velocity.PS(), flags ^ 1);
			*result = 1;
		}
	return true;
}

bool __fastcall RegisterInsertObject(char *inData)
{
	static char meshesPath[0x100] = "data\\meshes\\";
	ScopedString<0x100> scopedStr(inData);
	TESForm *form = *(TESForm**)inData;
	TESObjectREFR *refr = IS_REFERENCE(form) ? (TESObjectREFR*)form : nullptr;
	NiNode *rootNode = refr ? refr->GetRefNiNode() : nullptr;
	bool modifyMap = true;
	char doInsert = inData[4];
	if (refr)
	{
		if (refr->IsCreated() || kInventoryType[refr->baseForm->typeID])
		{
			if ((doInsert != 3) || !rootNode)
				return false;
			modifyMap = false;
		}
	}
	else if (!form->IsBoundObject())
		return false;

	char *pInData = inData + 8, *nodeName = nullptr, *objectName = FindChr(pInData, '|'), *suffix;
	if (objectName)
	{
		*objectName++ = 0;
		nodeName = pInData;
	}
	else objectName = pInData;

	if (!*objectName) return false;

	UInt8 flag = ((UInt8*)inData)[5];
	bool insertNode = flag == kHookFormFlag6_InsertNode;
	auto formsMap = insertNode ? *s_insertNodeMap : *s_attachModelMap;

	if (doInsert & 1)
	{
		if (!insertNode)
		{
			suffix = objectName;
			if (*objectName == '*')
			{
				if (!(suffix = FindChr(objectName + 2, '*')))
					return false;
				suffix++;
			}
			StrCopy(meshesPath + 12, suffix);
			if (!FileExistsEx(meshesPath, false))
				return false;
		}

		NiFixedString blockName(nodeName), dataStr, *pDataStr;
		if (modifyMap)
		{
			NodeNamesMap *namesMap;
			if (formsMap->Insert(form, &namesMap))
				form->SetJIPFlag(flag, true);
			if (!(*namesMap)[blockName].Insert(objectName, &pDataStr))
				return false;
		}
		else pDataStr = &dataStr;

		if (insertNode)
			*pDataStr = objectName + (*objectName == '^');

		if (rootNode && (doInsert & 2))
		{
			bool useRoot = !blockName || (rootNode->m_blockName == blockName);
			NiAVObject *targetObj = useRoot ? rootNode : rootNode->GetBlockByName(blockName);
			if (targetObj)
				if (insertNode)
					DoInsertNode(targetObj, objectName, *pDataStr, rootNode);
				else if ((rootNode = DoAttachModel(targetObj, objectName, pDataStr, rootNode)) && (rootNode->m_flags & 0x20000000))
					AddPointLights(rootNode);
			if (refr->IsPlayer() && (rootNode = s_pc1stPersonNode))
				if (targetObj = useRoot ? rootNode : rootNode->GetBlockByName(blockName))
					if (insertNode)
						DoInsertNode(targetObj, objectName, *pDataStr, rootNode);
					else DoAttachModel(targetObj, objectName, pDataStr, rootNode);
		}
	}
	else
	{
		auto findForm = formsMap->Find(form);
		if (!findForm) return false;
		auto findNode = findForm().FindOp(NiFixedString(nodeName));
		if (!findNode) return false;
		auto findData = findNode().FindOp(objectName);
		if (!findData) return false;
		if (rootNode && (doInsert & 2) && findData() && (!insertNode || (*objectName != '^')))
		{
			NiAVObject *object = rootNode->GetBlockByName(findData());
			if (object)
				object->m_parent->RemoveObject(object);
			if (refr->IsPlayer() && (rootNode = s_pc1stPersonNode))
			{
				object = rootNode->GetBlockByName(findData());
				if (object)
					object->m_parent->RemoveObject(object);
			}
		}
		findData.Remove(findNode());
		if (findNode().Empty())
		{
			findNode.Remove(findForm());
			if (findForm().Empty())
			{
				findForm.Remove();
				form->SetJIPFlag(flag, false);
			}
		}
	}
	s_insertObjects = !s_insertNodeMap->Empty() || !s_attachModelMap->Empty();
	return true;
}

bool Cmd_InsertNode_Execute(COMMAND_ARGS)
{
	char *dataStr = Pool_CAlloc(0x100);
	if (ExtractFormatStringArgs(2, dataStr + 8, EXTRACT_ARGS_EX, kCommandInfo_InsertNode.numParams, dataStr, dataStr + 4))
	{
		((UInt8*)dataStr)[5] = kHookFormFlag6_InsertNode;
		if (!(dataStr[4] & 2) || IsInMainThread())
		{
			if (RegisterInsertObject(dataStr))
				*result = 1;
		}
		else MainLoopAddCallback(RegisterInsertObject, dataStr);
	}
	else Pool_CFree(dataStr, 0x100);
	return true;
}

bool Cmd_AttachModel_Execute(COMMAND_ARGS)
{
	char *dataStr = Pool_CAlloc(0x100);
	if (ExtractFormatStringArgs(2, dataStr + 8, EXTRACT_ARGS_EX, kCommandInfo_AttachModel.numParams, dataStr, dataStr + 4))
	{
		((UInt8*)dataStr)[5] = kHookFormFlag6_AttachModel;
		if (!(dataStr[4] & 2) || IsInMainThread())
		{
			if (RegisterInsertObject(dataStr))
				*result = 1;
		}
		else MainLoopAddCallback(RegisterInsertObject, dataStr);
	}
	else Pool_CFree(dataStr, 0x100);
	return true;
}

bool Cmd_SynchronizePosition_Execute(COMMAND_ARGS)
{
	TESObjectREFR *targetRef = nullptr;
	UInt32 syncRot = 0;
	s_syncPositionMods.z = 0;
	char nodeName[0x40];
	nodeName[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &targetRef, &syncRot, &s_syncPositionMods.z, &nodeName))
	{
		TESObjectCELL *cell;
		if (targetRef)
		{
			if (g_thePlayer->GetDistance(targetRef) > 1024.0F)
			{
				cell = targetRef->GetParentCell();
				if (cell) g_thePlayer->MoveToCell(cell, targetRef->position);
			}
			s_syncPositionFlags = (syncRot != 0);
			s_syncPositionPos.SetPS(targetRef->position.PS3());
			s_syncPositionNode() = nodeName;
			s_syncPositionRef = targetRef;
		}
		else if (targetRef = s_syncPositionRef)
		{
			s_syncPositionRef = nullptr;
			cell = targetRef->GetParentCell();
			if (cell) g_thePlayer->MoveToCell(cell, s_syncPositionPos);
		}
	}
	return true;
}

bool Cmd_ModelHasBlock_Execute(COMMAND_ARGS)
{
	char buffer[0x80];
	buffer[0] = '^';
	TESForm *form;
	if (ExtractFormatStringArgs(1, buffer + 1, EXTRACT_ARGS_EX, kCommandInfo_ModelHasBlock.numParams, &form))
	{
		TESObjectREFR *refr = IS_REFERENCE(form) ? (TESObjectREFR*)form : nullptr;
		NiNode *rootNode = refr ? refr->GetNiNode() : nullptr;
		if (rootNode && rootNode->GetBlock(buffer + 1))
			goto Retn1;
		NodeNamesMap *namesMap = s_insertNodeMap->GetPtr(form);
		if (namesMap)
		{
			for (auto iter = namesMap->Begin(); iter; ++iter)
				if (iter().HasKey(buffer + 1) || iter().HasKey(buffer))
					goto Retn1;
		}
		if (refr)
		{
			form = refr->GetBaseForm2();
			if (!form) goto Retn0;
			namesMap = s_insertNodeMap->GetPtr(form);
			if (namesMap)
			{
				for (auto iter = namesMap->Begin(); iter; ++iter)
					if (iter().HasKey(buffer + 1) || iter().HasKey(buffer))
						goto Retn1;
			}
		}
		if (!rootNode && form->IsBoundObject())
		{
			const char *modelPath = form->GetModelPath();
			if (modelPath)
			{
				rootNode = g_modelLoader->LoadModel(modelPath, 0, 1, 0, 1);
				if (rootNode && rootNode->GetBlock(buffer + 1))
					goto Retn1;
			}
		}
	}
Retn0:
	return true;
Retn1:
	*result = 1;
	return true;
}

bool Cmd_GetRayCastRef_Execute(COMMAND_ARGS)
{
	SInt32 layerType = 6;
	char nodeName[0x40];
	nodeName[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &layerType, &nodeName))
		if (NiNode *objNode = thisObj->GetNode(nodeName))
			if (NiAVObject *rcObject = GetRayCastObject(objNode->WorldTranslate(), objNode->WorldRotate() + 1, 50000.0F, layerType))
				if (TESObjectREFR *resRefr = rcObject->GetParentRef())
					REFR_RES = resRefr->refID;
	return true;
}

bool Cmd_GetRayCastMaterial_Execute(COMMAND_ARGS)
{
	SInt32 layerType = 6;
	char nodeName[0x40];
	nodeName[0] = 0;
	int material = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &layerType, &nodeName))
		if (NiNode *objNode = thisObj->GetNode(nodeName))
			material = GetRayCastMaterial(objNode->WorldTranslate(), objNode->WorldRotate() + 1, 50000.0F, layerType);
	*result = material;
	return true;
}

void GetCollisionNodes(NiNode *node, TempElements *tmpElements)
{
	if (node->m_collisionObject && node->m_collisionObject->worldObj)
		tmpElements->Append(node->GetName());
	for (auto iter = node->m_children.Begin(); iter; ++iter)
		if (*iter && IS_NODE(*iter))
			GetCollisionNodes((NiNode*)*iter, tmpElements);
}

bool Cmd_GetCollisionNodes_Execute(COMMAND_ARGS)
{
	if (NiNode *rootNode = thisObj->GetRefNiNode())
	{
		TempElements *tmpElements = GetTempElements();
		GetCollisionNodes(rootNode, tmpElements);
		if (!tmpElements->Empty())
			*result = (int)CreateArray(tmpElements->Data(), tmpElements->Size(), scriptObj);
	}
	return true;
}

void GetChildBlocks(NiNode *node, TempElements *tmpElements)
{
	tmpElements->Append(node->GetName());
	for (auto iter = node->m_children.Begin(); iter; ++iter)
		if (*iter)
			if IS_NODE(*iter)
				GetChildBlocks((NiNode*)*iter, tmpElements);
			else tmpElements->Append(iter->GetName());
}

bool Cmd_GetChildBlocks_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	blockName[0] = 0;
	UInt32 pcNode = 0, noRecourse = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &pcNode, &noRecourse))
		if (NiNode *objNode = (NiNode*)GetNifBlock(thisObj, pcNode, blockName); objNode && IS_NODE(objNode))
		{
			TempElements *tmpElements = GetTempElements();
			if (noRecourse)
			{
				tmpElements->Append(0.0);
				for (auto iter = objNode->m_children.Begin(); iter; ++iter)
					if (*iter) tmpElements->Append(iter->GetName());
			}
			else GetChildBlocks(objNode, tmpElements);
			if (tmpElements->Size() > 1)
				*result = (int)CreateArray(tmpElements->Data() + 1, tmpElements->Size() - 1, scriptObj);
		}
	return true;
}

bool Cmd_GetBlockTextureSet_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName))
		if (NiAVObject *block = thisObj->GetNiBlock(blockName); block && IS_GEOMETRY(block))
			if (BSShaderProperty *shaderProp = ((NiGeometry*)block)->shaderProp)
			{
				ArrayElementL elements[6];
				const char *filePath;
				bool createArr = false;
				if (shaderProp->shaderType == BSShaderProperty::kType_PPLighting)
				{
					BSShaderPPLightingProperty *lightingProp = (BSShaderPPLightingProperty*)shaderProp;
					if (lightingProp->textureSet)
					{
						createArr = true;
						String *textures = lightingProp->textureSet->textures;
						UInt32 index = 0;
						do
						{
							filePath = textures[index].CStr();
							if (*filePath && StrBeginsCI(filePath, "data\\textures\\"))
								filePath += 14;
							elements[index] = filePath;
						}
						while (++index < 6);
					}
				}
				else if (shaderProp->shaderType == BSShaderProperty::kType_NoLighting)
					if (BSShaderNoLightingProperty *noLightingProp = (BSShaderNoLightingProperty*)shaderProp;
						noLightingProp->srcTexture && IS_TYPE(noLightingProp->srcTexture, NiSourceTexture))
						if (filePath = ((NiSourceTexture*)noLightingProp->srcTexture)->ddsPath1)
						{
							createArr = true;
							if (StrBeginsCI(filePath, "data\\textures\\"))
								filePath += 14;
							UInt32 index = 0;
							do
							{
								elements[index] = filePath;
								filePath = "";
							}
							while (++index < 6);
						}
				if (createArr)
					*result = (int)CreateArray(elements, 6, scriptObj);
			}
	return true;
}

bool Cmd_GetPosEx_Execute(COMMAND_ARGS)
{
	ResultVars outPos;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outPos.x, &outPos.y, &outPos.z))
		outPos.Set(thisObj->position.PS());
	return true;
}

bool Cmd_SetTextureTransformKey_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 ctrlIndex, keyIndex;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &ctrlIndex, &keyIndex, &value))
		if (NiAVObject *block = thisObj->GetNiBlock(blockName); block && IS_GEOMETRY(block))
			if (NiTexturingProperty *texProp = ((NiGeometry*)block)->texturingProp)
			{
				NiTextureTransformController *ctrlr = (NiTextureTransformController*)texProp->m_controller;
				while (ctrlr && ctrlIndex)
				{
					ctrlIndex--;
					ctrlr = (NiTextureTransformController*)ctrlr->nextCtrl;
				}
				if (ctrlr && IS_TYPE(ctrlr, NiTextureTransformController))
					if (NiFloatInterpolator *intrpl = (NiFloatInterpolator*)ctrlr->interpolator; intrpl && IS_TYPE(intrpl, NiFloatInterpolator))
						if (NiFloatData *fltData = intrpl->data; fltData && IS_TYPE(fltData, NiFloatData) && (keyIndex < fltData->numKeys))
						{
							fltData->data[keyIndex].value = value;
							ctrlr->Update(kNiUpdateData);
						}
			}
	return true;
}

bool Cmd_AttachExtraCamera_Execute(COMMAND_ARGS)
{
	char camName[0x40], nodeName[0x40];
	UInt32 doAttach;
	nodeName[0] = 0;
	UInt32 pcNode = 0;
	NiCamera *xCamera;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &camName, &doAttach, &nodeName, &pcNode) && camName[0])
		if (doAttach)
		{
			if (NiNode *targetNode = (NiNode*)GetNifBlock(thisObj, pcNode, nodeName); targetNode && IS_NODE(targetNode))
			{
				NiCamera **pCamera;
				if (s_extraCamerasMap->InsertKey(camName, &pCamera))
				{
					*pCamera = xCamera = NiCamera::Create();
					xCamera->m_uiRefCount = 2;
					xCamera->SetName(camName);
					xCamera->m_transformLocal.scale = 0;
					xCamera->frustum.dNear = 5.0F;
					xCamera->frustum.dFar = 353840.0F;
					xCamera->minNearPlaneDist = 1.0F;
					xCamera->maxFarNearRatio = 70768.0F;
					xCamera->LODAdjust = 1.0F;
				}
				else xCamera = *pCamera;
				if (xCamera->m_parent != targetNode)
					targetNode->AddObject(xCamera, 1);
				*result = 1;
			}
		}
		else if (xCamera = s_extraCamerasMap->GetErase(camName))
		{
			if (xCamera->m_parent)
				xCamera->m_parent->RemoveObject(xCamera);
			xCamera->Destructor(true);
			*result = 1;
		}
	return true;
}

bool Cmd_ProjectExtraCamera_Execute(COMMAND_ARGS)
{
	char camName[0x40], blockName[0x80];
	float fov;
	UInt32 pixelSize = 0x100, flags = 0/*Bit0: Grayscale; Bit1: Run actor culler*/;
	ImageSpaceEffectEffectID isEffect = IS_EFFECT_NONE;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &camName, &blockName, &fov, &pixelSize, &flags, &isEffect))
		if (NiCamera *xCamera = s_extraCamerasMap->Get(camName); xCamera && xCamera->m_parent && (pixelSize &= 0xFF0))
		{
			D3DFORMAT d3dFormat = D3DFMT_X8R8G8B8;
			NiTexture **pTexture = nullptr;
			if (blockName[0] == '*')
			{
				if (TileImage *targetTile = (TileImage*)GetTargetComponent(blockName + 1); targetTile && IS_TYPE(targetTile, TileImage) && targetTile->shaderProp)
				{
					pTexture = &targetTile->shaderProp->srcTexture;
					if (flags & 1)
						d3dFormat = D3DFMT_L8;
				}
			}
			else if (thisObj)
			{
				pTexture = thisObj->GetTexturePtr(blockName);
				if (*(bool*)0x11F941E)
					d3dFormat = D3DFMT_A16B16G16R16F;
			}
			if (pTexture)
			{
				if (ULNG(xCamera->m_transformLocal.scale) != ULNG(fov))
				{
					xCamera->m_transformLocal.scale = fov;
					xCamera->frustum.viewPort.SetFOV(fov * FltPId180);
					xCamera->LODAdjust = fov * 0.08F;
				}
				ProcessManager::Get()->UnCullHighActors();
				g_TES->waterManager->UpdateEx(xCamera);
				BSTextureManager::GenerateRenderedTexture(xCamera, TextureParams(pixelSize, pixelSize, d3dFormat, kRndrMode_Normal, 0xFFFFFFFF/*, isEffect*/), pTexture);
				*result = 1;
			}
		}
	return true;
}

bool Cmd_RenameNifBlock_Execute(COMMAND_ARGS)
{
	char blockName[0x40], newName[0x40];
	UInt32 pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &newName, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			niBlock->SetName(newName);
			*result = 1;
		}
	return true;
}

bool Cmd_RemoveNifBlock_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			niBlock->m_parent->RemoveObject(niBlock);
			*result = 1;
		}
	return true;
}

bool Cmd_PlayAnimSequence_Execute(COMMAND_ARGS)
{
	char sequenceName[0x60], nodeName[0x40];
	nodeName[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &sequenceName, &nodeName))
		if (NiNode *targetNode = thisObj->GetNode(nodeName); targetNode && (NOT_ACTOR(thisObj) || (((Actor*)thisObj)->lifeState != 1)))
			if (NiControllerManager *ctrlMgr = (NiControllerManager*)targetNode->m_controller; ctrlMgr && IS_TYPE(ctrlMgr, NiControllerManager))
				if (NiControllerSequence *sequence = ctrlMgr->FindSequence(sequenceName))
				{
					for (auto iter = ctrlMgr->sequences.Begin(); iter; ++iter)
						if (*iter && iter->sequenceName && (*(UInt32*)iter->sequenceName != 'eldI'))
							iter->Deactivate(0, 0);
					if (sequence->Play())
					{
						//thisObj->Unk_32(1);
						thisObj->MarkModified(0x10000000);
						*result = 1;
					}
				}
	return true;
}

bool Cmd_GetTranslatedPos_Execute(COMMAND_ARGS)
{
	NiVector3 posMods;
	ResultVars outPos;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posMods.x, &posMods.y, &posMods.z, &outPos.x, &outPos.y, &outPos.z))
	{
		outPos.Set(thisObj->GetTranslatedPos(posMods));
		*result = 1;
	}
	return true;
}

bool Cmd_GetNifBlockTranslationAlt_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	ResultVars outPos;
	UInt32 getWorld = 0, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &outPos.x, &outPos.y, &outPos.z, &getWorld, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			outPos.Set(getWorld ? niBlock->WorldTranslate().PS() : niBlock->LocalTranslate().PS());
			*result = 1;
		}
	return true;
}

bool Cmd_GetNifBlockRotationAlt_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	ResultVars outRot;
	UInt32 getMode = 0, pcNode = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &outRot.x, &outRot.y, &outRot.z, &getMode, &pcNode))
		if (NiAVObject *niBlock = GetNifBlock(thisObj, pcNode, blockName))
		{
			NiMatrix33 &rotMat = (getMode & 1) ? niBlock->WorldRotate() : niBlock->LocalRotate();
			__m128 angles = (getMode & 2) ? rotMat.ToEulerPRYInv() : rotMat.ToEulerPRY();
			outRot.Set(angles, GET_PS(9));
			*result = 1;
		}
	return true;
}

bool Cmd_GetLinearVelocityAlt_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	ResultVars outVel;
	UInt32 getLocal = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &outVel.x, &outVel.y, &outVel.z, &getLocal))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName); rigidBody && rigidBody->IsMobile())
		{
			if (!getLocal)
				outVel.Set(rigidBody->motion.linVelocity.PS());
			else
				outVel.Set(rigidBody->motion.motionState.transform.rotation.MultiplyVector(_mm_load_ps(rigidBody->motion.linVelocity)));
			*result = 1;
		}
	return true;
}

bool Cmd_GetAngularVelocityAlt_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	ResultVars outVel;
	UInt32 getGlobal = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &outVel.x, &outVel.y, &outVel.z, &getGlobal))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName); rigidBody && rigidBody->IsMobile())
		{
			__m128 velocity = rigidBody->motion.angVelocity.PS();
			if (!getGlobal)
				velocity = rigidBody->motion.motionState.transform.rotation.MultiplyVector(velocity);
			outVel.Set(velocity);
			*result = 1;
		}
	return true;
}

bool Cmd_SetAngularVelocityEx_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	NiVector4 velocity(_mm_setzero_ps());
	UInt32 flags = 0;	//	1 - set global; 2 - add value
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &velocity.x, &velocity.y, &velocity.z, &flags))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName); rigidBody && rigidBody->IsMobile())
		{
			rigidBody->SetVelocity(velocity.PS(), flags | 0x10);
			*result = 1;
		}
	return true;
}

bool Cmd_GetCollisionObjProperty_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 propID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &propID) && (propID <= 8))
		if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName))
		{
			switch (propID)
			{
				case 0:
					*result = rigidBody->friction;
					break;
				case 1:
					*result = rigidBody->restitution;
					break;
				case 2:
					*result = rigidBody->motion.motionState.linearDamping;
					break;
				case 3:
					*result = rigidBody->motion.motionState.angularDamping;
					break;
				case 4:
				case 5:
				case 6:
					*result = rigidBody->motion.inertia[propID - 4];
					break;
				case 7:
					if (rigidBody->motion.bodyMassInv > 0)
						*result = 1.0 / rigidBody->motion.bodyMassInv;
					break;
				case 8:
					*result = rigidBody->motion.type;
					break;
			}
		}
	return true;
}

bool Cmd_SetCollisionObjProperty_Execute(COMMAND_ARGS)
{
	char blockName[0x40];
	UInt32 propID;
	FltAndInt value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &blockName, &propID, &value) && (propID <= 8))
	{
		if (propID == 7)
		{
			if (value.f > 0)
				value.f = 1.0F / value.f;
			else value.i = 0;
		}
		else if (propID == 8)
			value.i = int(value.f);
		if (*(UInt16*)blockName == '*')
		{
			if (NiNode *rootNode = thisObj->GetRefNiNode())
				rootNode->SetCollisionPropRecurse(propID, value);
		}
		else if (hkpRigidBody *rigidBody = thisObj->GetRigidBody(blockName))
			rigidBody->SetCollisionProperty(propID, value);
	}
	return true;
}

bool Cmd_GetCollisionObjLayerType_Execute(COMMAND_ARGS)
{
	char nodeName[0x40];
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName))
		if (NiNode *targetNode = thisObj->GetNode2(nodeName); targetNode && targetNode->m_collisionObject)
			if (bhkWorldObject *worldObj = targetNode->m_collisionObject->worldObj)
				*result = ((hkpWorldObject*)worldObj->refObject)->layerType;
	return true;
}

bool Cmd_SetCollisionObjLayerType_Execute(COMMAND_ARGS)
{
	char nodeName[0x40];
	UInt32 layerType;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName, &layerType) && layerType && (layerType < LAYER_MAX))
		if (NiNode *rootNode = thisObj->GetRefNiNode())
			if (*(UInt16*)nodeName == '*')
				rootNode->SetCollisionLayer(layerType);
			else if ((rootNode = rootNode->GetNode(nodeName)) && rootNode->m_collisionObject)
				if (bhkWorldObject *worldObj = rootNode->m_collisionObject->worldObj; worldObj && (((hkpWorldObject*)worldObj->refObject)->layerType != layerType))
				{
					((hkpWorldObject*)worldObj->refObject)->layerType = layerType;
					worldObj->UpdateCollisionFilter();
				}
	return true;
}

bool Cmd_SetRefrModelPath_Execute(COMMAND_ARGS)
{
	char modelPath[0x80];
	modelPath[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modelPath))
	{
		if (modelPath[0])
		{
			char **pPath;
			if (s_refrModelPathMap->InsertKey(thisObj, &pPath))
				HOOK_INC(GetModelPath);
			else FreeStringKey(*pPath);
			*pPath = CopyStringKey(modelPath);
			thisObj->JIPRefFlags() |= kHookRefFlag5F_RefrModelPath;
		}
		else
		{
			if (char *pPath = s_refrModelPathMap->GetErase(thisObj))
			{
				FreeStringKey(pPath);
				HOOK_DEC(GetModelPath);
			}
			thisObj->JIPRefFlags() &= ~kHookRefFlag5F_RefrModelPath;
		}
	}
	return true;
}

bool Cmd_AttachLine_Execute(COMMAND_ARGS)
{
	char nodeName[0x40], lineName[0x40];
	float length;
	NiColorAlpha color;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName, &lineName, &length, &color.r, &color.g, &color.b, &color.a))
		if (NiNode *targetNode = thisObj->GetNode2(nodeName))
		{
			color *= 1 / 255.0F;
			NiLines *lines = NiLines::Create(length, color, lineName);
			targetNode->AddObject(lines, 1);
			if (thisObj->IsPlayer() && (targetNode = s_pc1stPersonNode->GetNode(nodeName)))
				targetNode->AddObject(lines->CreateCopy(), 1);
			*result = 1;
		}
	return true;
}

bool Cmd_ToggleNoZPosReset_Execute(COMMAND_ARGS)
{
	UInt32 toggle;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &toggle))
	{
		if (toggle)
		{
			if (*(UInt32*)0x629006 != 0x0C74C085)
			{
				SAFE_WRITE_BUF(0x629006, "\x85\xC0\x74\x0C\xF6\x40\x5F\x10\x0F\x85\x28\x05\x00\x00\x66\x90");					//	kHookRefFlag5F_NoZPosReset
				SAFE_WRITE_BUF(0x6295E9, "\x8B\x4D\x08\xF6\x41\x5F\x10\x75\x56\x8D\x45\xF4\x50\x8B\x11\x0F\x1F\x44\x00\x00");	//	kHookRefFlag5F_NoZPosReset
			}
			thisObj->JIPRefFlags() |= kHookRefFlag5F_NoZPosReset;
		}
		else thisObj->JIPRefFlags() &= ~kHookRefFlag5F_NoZPosReset;
	}
	return true;
}

bool Cmd_RotateAroundPoint_Execute(COMMAND_ARGS)
{
	NiVector3 origin, rotation;
	UInt32 skipAngles = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &origin.x, &origin.y, &origin.z, &rotation.x, &rotation.y, &rotation.z, &skipAngles))
		thisObj->RotateAroundPoint(rotation * GET_PS(8), origin, skipAngles);
	return true;
}

bool Cmd_ToggleNodeCollision_Execute(COMMAND_ARGS)
{
	char nodeName[0x40];
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &nodeName, &enable))
		if (NiNode *targetNode = thisObj->GetNode(nodeName); targetNode && targetNode->m_collisionObject)
			if (bhkWorldObject *worldObj = targetNode->m_collisionObject->worldObj)
			{
				hkpWorldObject *refObj = (hkpWorldObject*)worldObj->refObject;
				refObj->filterFlags &= 0xBF;
				refObj->filterFlags |= (!enable) << 6;
			}
	return true;
}

bool Cmd_GetEditorPosition_Execute(COMMAND_ARGS)
{
	ResultVars outPos, outRot;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outPos.x, &outPos.y, &outPos.z, &outRot.x, &outRot.y, &outRot.z))
	{
		TESForm *worldOrCell = nullptr;
		NiVector3 *pPosVec = nullptr, *pRotVec = nullptr;
		if (IS_ACTOR(thisObj))
		{
			Actor *actor = (Actor*)thisObj;
			worldOrCell = actor->startingWorldOrCell;
			pPosVec = &actor->startingPos;
			NiVector3 rotVec(0, 0, actor->startingZRot);
			pRotVec = &rotVec;
		}
		else if (ExtraStartingWorldOrCell *xStartingCell = GetExtraType(&thisObj->extraDataList, ExtraStartingWorldOrCell))
		{
			worldOrCell = xStartingCell->worldOrCell;
			if (ExtraStartingPosition *xStartingPos = GetExtraType(&thisObj->extraDataList, ExtraStartingPosition))
			{
				pPosVec = &xStartingPos->posVector;
				pRotVec = &xStartingPos->rotVector;
			}
		}
		if (worldOrCell && (IS_ID(worldOrCell, TESObjectCELL) || (IS_ID(worldOrCell, TESWorldSpace) && (worldOrCell = ((TESWorldSpace*)worldOrCell)->cell))) && pPosVec && pRotVec)
		{
			outPos.Set(pPosVec->PS());
			if (numArgs == 6)
				outRot.Set(pRotVec->PS(), GET_PS(9));
			REFR_RES = worldOrCell->refID;
		}		
	}
	return true;
}

ExtraJIPData* __fastcall GetExtraJIPData(TESObjectREFR *thisObj, UInt32 modIdx)
{
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (ExtraDataList *xData = invRef ? invRef->xData : &thisObj->extraDataList)
		if (ExtraJIP *xJIP = GetExtraType(xData, ExtraJIP))
			if (ExtraJIPEntry *entry = s_extraDataKeysMap->GetPtr(xJIP->key))
				return entry->dataMap.GetPtr(modIdx);
	return nullptr;
}

bool Cmd_RefHasExtraData_Execute(COMMAND_ARGS)
{
	UInt32 modIdx = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &modIdx))
	{
		if (!modIdx)
			modIdx = scriptObj->GetOverridingModIdx();
		if (modIdx < 0xFF)
		{
			XDATA_CS
			if (GetExtraJIPData(thisObj, modIdx))
				*result = 1;
		}
	}
	return true;
}

bool Cmd_GetRefExtraData_Execute(COMMAND_ARGS)
{
	double fVarIdx, fModIdx = 0;
	if (PluginExpressionEvaluator eval(PASS_COMMAND_ARGS); eval.ExtractArgsVA(nullptr, &fVarIdx, &fModIdx))
	{
		UInt32 varIdx = (int)fVarIdx;
		if (varIdx > 19) return true;
		UInt32 modIdx = (int)fModIdx;
		if (!modIdx)
			modIdx = scriptObj->GetOverridingModIdx();
		if (modIdx >= 0xFF) return true;
		XDATA_CS
		if (ExtraJIPData *pData = GetExtraJIPData(thisObj, modIdx))
			if (varIdx >= 18)
			{
				AssignString(PASS_COMMAND_ARGS, pData->strings[varIdx - 18].Data());
				eval.SetExpectedReturnType(kRetnType_String);
			}
			else if (pData->IsRef(varIdx))
			{
				REFR_RES = pData->values[varIdx];
				eval.SetExpectedReturnType(kRetnType_Form);
			}
			else *result = pData->values[varIdx];
	}
	return true;
}

bool Cmd_SetRefExtraData_Execute(COMMAND_ARGS)
{
	UInt32 modIdx = scriptObj->GetOverridingModIdx();
	if (modIdx == 0xFF) return true;
	if (PluginExpressionEvaluator eval(PASS_COMMAND_ARGS); eval.ExtractArgs())
	{
		UInt32 varIdx;
		PluginScriptToken *tIndex = eval.GetNthArg(0), *tValue;
		if (tIndex)
		{
			varIdx = tIndex->GetInt();
			if ((varIdx > 19) || !(tValue = eval.GetNthArg(1)))
				return true;
		}
		
		XDATA_CS

		TESObjectREFR *refr = thisObj;
		ExtraJIP *xJIP;
		if (InventoryRef *invRef = InventoryRefGetForID(thisObj->refID))
		{
			refr = invRef->containerRef;
			ExtraDataList *xData = invRef->xData;
			if (tIndex)
			{
				if (!xData)
				{
					if (!(xData = invRef->CreateExtraData()))
						return true;
					if (invRef->GetCount() > 1)
						xData->AddExtraCount(invRef->GetCount());
					xJIP = xData->AddExtraJIP();
				}
				else if (!(xJIP = GetExtraType(xData, ExtraJIP)))
					xJIP = xData->AddExtraJIP();
			}
			else
			{
				if (xData && (xJIP = GetExtraType(xData, ExtraJIP)))
					if (ExtraJIPEntry *dataEntry = s_extraDataKeysMap->GetPtr(xJIP->key); dataEntry && dataEntry->dataMap.Erase(modIdx))
					{
						s_dataChangedFlags |= kChangedFlag_ExtraData;
						if (dataEntry->dataMap.Empty())
						{
							dataEntry->refID = 0;
							xData->RemoveByType(kXData_ExtraJIP);
							if (!xData->m_data || ((xData->m_data->type == kXData_ExtraCount) && !xData->m_data->next))
								if (ContChangesEntry *entry = refr->GetContainerChangesEntry(invRef->type))
								{
									entry->extendData->Remove(xData);
									xData->Destroy(1);
								}
						}
					}
				return true;
			}
		}
		else
		{
			xJIP = GetExtraType(&thisObj->extraDataList, ExtraJIP);
			if (tIndex)
			{
				if (!xJIP)
					xJIP = thisObj->extraDataList.AddExtraJIP();
			}
			else
			{
				if (xJIP)
					if (ExtraJIPEntry *dataEntry = s_extraDataKeysMap->GetPtr(xJIP->key); dataEntry && dataEntry->dataMap.Erase(modIdx))
					{
						s_dataChangedFlags |= kChangedFlag_ExtraData;
						if (dataEntry->dataMap.Empty())
						{
							dataEntry->refID = 0;
							thisObj->extraDataList.RemoveByType(kXData_ExtraJIP);
						}
					}
				return true;
			}
		}
		if (!xJIP->key)
		{
			xJIP->key = ExtraJIP::MakeKey();
			refr->MarkModified(0x400);
		}
		ExtraJIPData *pData = &s_extraDataKeysMap()[xJIP->key].dataMap[modIdx];
		if (varIdx < 18)
		{
			if (tValue->CanConvertTo(kTokenType_Form))
				pData->Set(varIdx, tValue->GetFormID());
			else
				pData->Set(varIdx, tValue->GetFloat());
		}
		else if (tValue->CanConvertTo(kTokenType_String))
			pData->Set(varIdx - 18, tValue->GetString());
		s_dataChangedFlags |= kChangedFlag_ExtraData;
	}
	return true;
}

bool Cmd_TogglePurgeOnUnload_Execute(COMMAND_ARGS)
{
	BOOL currState = _bittest((SInt32*)&thisObj->flags, 0x16);
	*result = currState;
	UInt32 toggle;
	if (NUM_ARGS && ExtractArgsEx(EXTRACT_ARGS_EX, &toggle) && (currState != toggle) && thisObj->IsCreated() && !thisObj->IsPersistent())
	{
		thisObj->flags ^= 0x400000;
		thisObj->MarkModified(1);
	}
	return true;
}