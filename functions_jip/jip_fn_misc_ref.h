#pragma once

DEFINE_COMMAND_PLUGIN(SetPersistent, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetObjectDimensions, , 0, 2, kParams_JIP_OneAxis_OneOptionalForm);
DEFINE_COMMAND_PLUGIN(GetIsItem, , 0, 1, kParams_OneOptionalObjectID);
DEFINE_COMMAND_PLUGIN(SetLinkedReference, , 1, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(GetEnableChildren, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetLinkedChildren, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(HasPrimitive, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetPrimitiveBound, , 1, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(SetPrimitiveBound, , 1, 2, kParams_JIP_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(AddPrimitive, , 1, 4, kParams_JIP_OneInt_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetTeammates, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(MoveToCell, , 1, 4, kParams_JIP_OneForm_ThreeFloats);
DEFINE_COMMAND_PLUGIN(MoveToEditorPosition, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetCenterPos, , 1, 1, kParams_Axis);
DEFINE_COMMAND_PLUGIN(GetRefType, , 0, 1, kParams_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(ToggleObjectCollision, , 1, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetMaterialPropertyValue, , 1, 2, kParams_JIP_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(SetMaterialPropertyValue, , 1, 3, kParams_JIP_OneString_OneInt_OneFloat);
DEFINE_COMMAND_PLUGIN(GetHasContact, , 1, 1, kParams_OneForm);
DEFINE_COMMAND_PLUGIN(GetHasContactBase, , 1, 1, kParams_OneForm);
DEFINE_CMD_ALT_COND_PLUGIN(GetHasContactType, , , 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(GetContactRefs, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetHasPhantom, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetInteractionDisabled, , 0, 1, kParams_OneOptionalObject);
DEFINE_COMMAND_PLUGIN(SetInteractionDisabled, , 0, 2, kParams_OneInt_OneOptionalObject);
DEFINE_COMMAND_PLUGIN(GetInteractionDisabledType, , 0, 1, kParams_OneInt);
DEFINE_COMMAND_PLUGIN(SetInteractionDisabledType, , 0, 2, kParams_TwoInts);
DEFINE_COMMAND_PLUGIN(AddRefMapMarker, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(RemoveRefMapMarker, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(RefHasMapMarker, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetPosEx, , 1, 3, kParams_JIP_ThreeFloats);
DEFINE_COMMAND_PLUGIN(MoveToReticle, , 1, 4, kParams_JIP_FourOptionalFloats);
DEFINE_COMMAND_PLUGIN(SetRefName, , 1, 1, kParams_JIP_OneOptionalString);
DEFINE_COMMAND_PLUGIN(SetAngleEx, , 1, 3, kParams_JIP_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetTeleportDoor, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetOnCriticalHitEventHandler, , 0, 5, kParams_JIP_OneForm_OneInt_ThreeOptionalForms);
DEFINE_COMMAND_PLUGIN(MoveToFadeDelay, , 1, 2, kParams_JIP_OneObjectRef_OneFloat);
DEFINE_COMMAND_PLUGIN(GetCrosshairWater, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(IsAnimPlayingEx, , 1, 3, kParams_JIP_OneInt_TwoOptionalInts);
DEFINE_COMMAND_PLUGIN(GetRigidBodyMass, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(PushObject, , 1, 5, kParams_JIP_FourFloats_OneOptionalObjectRef);
DEFINE_COMMAND_PLUGIN(MoveToContainer, , 1, 2, kParams_JIP_OneObjectRef_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetNifBlockTranslation, , 1, 2, kParams_JIP_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetNifBlockTranslation, , 1, 5, kParams_JIP_OneString_ThreeFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetNifBlockRotation, , 1, 2, kParams_JIP_OneString_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(SetNifBlockRotation, , 1, 5, kParams_JIP_OneString_ThreeFloats_OneOptionalInt);
DEFINE_COMMAND_PLUGIN(GetNifBlockScale, , 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetNifBlockScale, , 1, 2, kParams_JIP_OneString_OneFloat);
DEFINE_COMMAND_PLUGIN(GetNifBlockFlag, , 1, 2, kParams_JIP_OneString_OneInt);
DEFINE_COMMAND_PLUGIN(SetNifBlockFlag, , 1, 3, kParams_JIP_OneString_TwoInts);
DEFINE_COMMAND_PLUGIN(GetObjectVelocity, , 1, 1, kParams_JIP_OneOptionalAxis);
DEFINE_COMMAND_PLUGIN(GetAngularVelocity, , 1, 2, kParams_JIP_OneString_OneAxis);
DEFINE_COMMAND_PLUGIN(SetAngularVelocity, , 1, 3, kParams_JIP_OneString_OneAxis_OneFloat);
DEFINE_COMMAND_PLUGIN(PlaceAtCell, , 0, 6, kParams_JIP_OneForm_OneInt_OneForm_ThreeFloats);
DEFINE_COMMAND_PLUGIN(GetRayCastPos, , 1, 6, kParams_JIP_ThreeGlobals_OneOptionalFloat_OneOptionalInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetAnimSequenceFrequency, , 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(SetAnimSequenceFrequency, , 1, 2, kParams_JIP_OneString_OneFloat);
DEFINE_COMMAND_PLUGIN(MoveToNode, , 1, 5, kParams_JIP_OneObjectRef_OneString_ThreeOptionalFloats);
DEFINE_COMMAND_PLUGIN(GetPlayerPerks, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(GetNifBlockParentNodes, , 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetCrosshairRefEx, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(IsMobile, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(IsGrabbable, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(AttachLight, , 1, 5, kParams_JIP_OneString_OneForm_ThreeOptionalFloats);
DEFINE_COMMAND_PLUGIN(RemoveLight, , 1, 1, kParams_OneString);
DEFINE_COMMAND_PLUGIN(GetExtraFloat, , 1, 0, NULL);
DEFINE_COMMAND_PLUGIN(SetExtraFloat, , 1, 1, kParams_OneFloat);
DEFINE_COMMAND_PLUGIN(SetLinearVelocity, , 1, 4, kParams_JIP_OneString_ThreeFloats);
DEFINE_COMMAND_PLUGIN(InsertNode, , 0, 23, kParams_JIP_OneForm_OneInt_OneFormatString);
DEFINE_COMMAND_PLUGIN(AttachModel, , 0, 23, kParams_JIP_OneForm_OneInt_OneFormatString);
DEFINE_COMMAND_ALT_PLUGIN(SynchronizePosition, SyncPos, , 0, 4, kParams_JIP_OneOptionalObjectRef_OneOptionalInt_OneOptionalFloat_OneOptionalString);
DEFINE_COMMAND_PLUGIN(ModelHasBlock, , 0, 22, kParams_JIP_OneForm_OneFormatString);
DEFINE_COMMAND_PLUGIN(GetRayCastRef, , 1, 2, kParams_JIP_OneOptionalInt_OneOptionalString);
DEFINE_COMMAND_PLUGIN(GetRayCastMaterial, , 1, 2, kParams_JIP_OneOptionalInt_OneOptionalString);

bool Cmd_SetPersistent_Execute(COMMAND_ARGS)
{
	UInt32 flag;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &flag))
	{
		if (flag) thisObj->flags |= TESObjectREFR::kFlags_Persistent;
		else thisObj->flags &= ~TESObjectREFR::kFlags_Persistent;
	}
	return true;
}

bool Cmd_GetObjectDimensions_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis;
	TESForm *form = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &form)) return true;
	float scale;
	if (!form)
	{
		if (!thisObj) return true;
		scale = thisObj->scale;
		if (thisObj->IsActor())
		{
			Actor *actor = (Actor*)thisObj;
			if (actor->baseProcess && (actor->baseProcess->processLevel <= 1))
			{
				BSBound *bounds = ((MiddleHighProcess*)actor->baseProcess)->boundingBox;
				if (bounds) *result = bounds->dimensions[axis - 'X'] * 2 * scale;
			}
			return true;
		}
		form = thisObj->baseForm;
	}
	else scale = 1;
	TESBoundObject *object = DYNAMIC_CAST(form, TESForm, TESBoundObject);
	if (!object) return true;
	axis -= 'X';
	*result = abs(object->bounds[axis + 3] - object->bounds[axis]) * scale;
	return true;
}

bool Cmd_GetIsItem_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form = NULL;
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
	*result = 0;
	TESObjectREFR *linkRef = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &linkRef) && thisObj->SetLinkedRef(linkRef, scriptObj->GetOverridingModIdx()))
	{
		*result = 1;
		s_dataChangedFlags |= kChangedFlag_LinkedRefs;
	}
	return true;
}

bool Cmd_GetEnableChildren_Execute(COMMAND_ARGS)
{
	*result = 0;
	ExtraEnableStateChildren *xEnableChildren = GetExtraType(&thisObj->extraDataList, EnableStateChildren);
	if (!xEnableChildren) return true;
	s_tempElements.Clear();
	ListNode<TESObjectREFR> *iter = xEnableChildren->children.Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetLinkedChildren_Execute(COMMAND_ARGS)
{
	*result = 0;
	ExtraLinkedRefChildren *xLinkedChildren = GetExtraType(&thisObj->extraDataList, LinkedRefChildren);
	if (!xLinkedChildren) return true;
	s_tempElements.Clear();
	ListNode<TESObjectREFR> *iter = xLinkedChildren->children.Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_HasPrimitive_Execute(COMMAND_ARGS)
{
	ExtraPrimitive *xPrimitive = GetExtraType(&thisObj->extraDataList, Primitive);
	*result = (xPrimitive && xPrimitive->primitive) ? 1 : 0;
	return true;
}

bool Cmd_GetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
	{
		ExtraPrimitive *xPrimitive = GetExtraType(&thisObj->extraDataList, Primitive);
		if (xPrimitive && xPrimitive->primitive)
			*result = xPrimitive->primitive->bounds[axis - 'X'] * 2;
	}
	return true;
}

bool Cmd_SetPrimitiveBound_Execute(COMMAND_ARGS)
{
	*result = -1;
	char axis;
	float val;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &axis, &val)) return true;
	ExtraPrimitive *xPrimitive = GetExtraType(&thisObj->extraDataList, Primitive);
	if (!xPrimitive || !xPrimitive->primitive) return true;
	BGSPrimitive *primitive = xPrimitive->primitive;
	axis -= 'X';
	*result = primitive->bounds[axis] * 2;
	if (primitive->type == 2) primitive->bounds[2] = primitive->bounds[1] = primitive->bounds[0] = val / 2;
	else primitive->bounds[axis] = val / 2;
	thisObj->Update3D();
	return true;
}

bool Cmd_AddPrimitive_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 type;
	float boundX, boundY, boundZ;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &type, &boundX, &boundY, &boundZ) && (type >= 1) && (type <= 3) &&
		((thisObj->baseForm->refID == 0x21) || (IS_ID(thisObj->baseForm, TESObjectACTI) && (type != 3))) &&
		!thisObj->extraDataList.HasType(kExtraData_Primitive))
	{
		ExtraPrimitive *xPrimitive = ExtraPrimitive::Create();
		AddExtraData(&thisObj->extraDataList, xPrimitive);
		UInt8 size = (type == 1) ? 0x4C : 0x34;
		BGSPrimitive *primitive = (BGSPrimitive*)GameHeapAlloc(size);
		MemZero(primitive, size);
		*(UInt32*)primitive = (type == 1) ? kVtbl_BGSPrimitiveBox : ((type == 2) ? kVtbl_BGSPrimitiveSphere : kVtbl_BGSPrimitivePlane);
		primitive->type = type;
		primitive->bounds[0] = boundX / 2;
		if (type == 2) primitive->bounds[2] = primitive->bounds[1] = boundX / 2;
		else
		{
			primitive->bounds[1] = (type == 1) ? (boundY / 2) : 1;
			primitive->bounds[2] = boundZ / 2;
		}
		xPrimitive->primitive = primitive;
		thisObj->Update3D();
		*result = 1;
	}
	return true;
}

bool Cmd_GetTeammates_Execute(COMMAND_ARGS)
{
	*result = 0;
	s_tempElements.Clear();
	ListNode<TESObjectREFR> *iter = g_thePlayer->teammates.Head();
	do
	{
		if (iter->data) s_tempElements.Append(iter->data);
	}
	while (iter = iter->next);
	if (!s_tempElements.Empty())
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_MoveToCell_Execute(COMMAND_ARGS)
{
	TESForm *form;
	NiVector3 posVector;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &posVector.x, &posVector.y, &posVector.z) && thisObj->MoveToCell(form, &posVector))
		*result = 1;
	else *result = 0;
	return true;
}

bool Cmd_MoveToEditorPosition_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (thisObj->IsActor())
	{
		Actor *actor = (Actor*)thisObj;
		if (actor->MoveToCell(actor->startingWorldOrCell, &actor->startingPos)) *result = 1;
		return true;
	}
	ExtraStartingPosition *xStartingPos = GetExtraType(&thisObj->extraDataList, StartingPosition);
	if (!xStartingPos) return true;
	ExtraStartingWorldOrCell *xStartingCell = GetExtraType(&thisObj->extraDataList, StartingWorldOrCell);
	if (thisObj->MoveToCell(xStartingCell ? xStartingCell->worldOrCell : thisObj->GetParentCell(), &xStartingPos->posVector))
		*result = 1;
	return true;
}

bool Cmd_GetCenterPos_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
	{
		TESBoundObject *object = DYNAMIC_CAST(thisObj->baseForm, TESForm, TESBoundObject);
		if (object)
		{
			axis -= 'X';
			*result = ((float*)&thisObj->posX)[axis] + ((object->bounds[axis] + object->bounds[axis + 3]) * thisObj->scale / 2);
		}
	}
	return true;
}

bool Cmd_GetRefType_Execute(COMMAND_ARGS)
{
	TESObjectREFR *objRefr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &objRefr) && (objRefr || (objRefr = thisObj)))
	{
		*result = objRefr->typeID;
		DoConsolePrint(result);
	}
	else *result = 0;
	return true;
}

bool Cmd_ToggleObjectCollision_Execute(COMMAND_ARGS)
{
	UInt32 enable;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &enable) && !thisObj->IsActor() && !kInventoryType[thisObj->baseForm->typeID])
	{
		NiNode *rootNode = enable ? thisObj->GetNiNode() : thisObj->GetNiNodeCopyIfTemplate();
		if (rootNode)
			rootNode->ToggleCollision(enable != 0);
	}
	return true;
}

bool Cmd_GetMaterialPropertyValue_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 traitID;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &traitID) || (traitID > 8)) return true;
	NiAVObject *block = thisObj->GetNiBlock(s_strArgBuffer);
	if (!block) return true;
	NiMaterialProperty *matProp = (NiMaterialProperty*)block->GetProperty(2);
	if (!matProp) return true;
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
	UInt32 traitID;
	float value;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &traitID, &value) && (traitID <= 8) && (value >= 0))
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (niNode)
		{
			if ((s_strArgBuffer[0] == '*') && !s_strArgBuffer[1])
				niNode->BulkSetMaterialPropertyTraitValue(traitID, value);
			else
			{
				NiAVObject *block = niNode->GetBlock(s_strArgBuffer);
				if (block)
				{
					NiMaterialProperty *matProp = (NiMaterialProperty*)block->GetProperty(2);
					if (matProp) matProp->SetTraitValue(traitID, value);
				}
			}
		}
	}
	return true;
}

void __fastcall PopulateFormSet(TESForm *form)
{
	s_tempFormList.Clear();
	if IS_ID(form, BGSListForm)
	{
		ListNode<TESForm> *traverse = ((BGSListForm*)form)->list.Head();
		do
		{
			s_tempFormList.Insert(traverse->data);
		}
		while (traverse = traverse->next);
	}
	else s_tempFormList.Insert(form);
}

bool __fastcall GetHasContact(TESObjectREFR *thisObj, TESForm *form)
{
	PopulateFormSet(form);
	hkpWorldObject **bodies;
	UInt32 count;
	TESObjectREFR *refr;
	if (thisObj->IsActor())
	{
		if (!((Actor*)thisObj)->baseProcess) return false;
		bhkCharacterController *charCtrl = ((Actor*)thisObj)->baseProcess->GetCharacterController();
		if (!charCtrl) return false;
		if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
		{
			refr = GetCdBodyRef(&charCtrl->bodyUnderFeet->cdBody);
			if (refr && s_tempFormList.HasKey(refr))
				return true;
		}
		bodies = charCtrl->pointCollector.contactBodies.data;
		count = charCtrl->pointCollector.contactBodies.size;
	}
	else
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (!niNode) return false;
		ContactObjects contactObjs;
		niNode->GetContactObjects(&contactObjs);
		bodies = contactObjs.Data();
		count = contactObjs.Size();
	}
	for (; count; count--, bodies++)
	{
		refr = GetCdBodyRef(&(*bodies)->cdBody);
		if (refr && s_tempFormList.HasKey(refr))
			return true;
	}
	return false;
}

bool Cmd_GetHasContact_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		*result = GetHasContact(thisObj, form);
	else *result = 0;
	return true;
}

bool __fastcall GetHasContactBase(TESObjectREFR *thisObj, TESForm *form)
{
	PopulateFormSet(form);
	hkpWorldObject **bodies;
	UInt32 count;
	TESObjectREFR *refr;
	if (thisObj->IsActor())
	{
		if (!((Actor*)thisObj)->baseProcess) return false;
		bhkCharacterController *charCtrl = ((Actor*)thisObj)->baseProcess->GetCharacterController();
		if (!charCtrl) return false;
		if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
		{
			refr = GetCdBodyRef(&charCtrl->bodyUnderFeet->cdBody);
			if (refr && s_tempFormList.HasKey(refr->baseForm))
				return true;
		}
		bodies = charCtrl->pointCollector.contactBodies.data;
		count = charCtrl->pointCollector.contactBodies.size;
	}
	else
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (!niNode) return false;
		ContactObjects contactObjs;
		niNode->GetContactObjects(&contactObjs);
		bodies = contactObjs.Data();
		count = contactObjs.Size();
	}
	for (; count; count--, bodies++)
	{
		refr = GetCdBodyRef(&(*bodies)->cdBody);
		if (refr && s_tempFormList.HasKey(refr->baseForm))
			return true;
	}
	return false;
}

bool Cmd_GetHasContactBase_Execute(COMMAND_ARGS)
{
	TESForm *form;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form))
		*result = GetHasContactBase(thisObj, form);
	else *result = 0;
	return true;
}

bool __fastcall GetHasContactType(TESObjectREFR *thisObj, UInt32 typeID)
{
	hkpWorldObject **bodies;
	UInt32 count;
	TESObjectREFR *refr;
	if (thisObj->IsActor())
	{
		if (!((Actor*)thisObj)->baseProcess) return false;
		bhkCharacterController *charCtrl = ((Actor*)thisObj)->baseProcess->GetCharacterController();
		if (!charCtrl) return false;
		if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
		{
			refr = GetCdBodyRef(&charCtrl->bodyUnderFeet->cdBody);
			if (refr && (refr->baseForm->typeID == typeID))
				return true;
		}
		bodies = charCtrl->pointCollector.contactBodies.data;
		count = charCtrl->pointCollector.contactBodies.size;
	}
	else
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (!niNode) return false;
		ContactObjects contactObjs;
		niNode->GetContactObjects(&contactObjs);
		bodies = contactObjs.Data();
		count = contactObjs.Size();
	}
	for (; count; count--, bodies++)
	{
		refr = GetCdBodyRef(&(*bodies)->cdBody);
		if (refr && (refr->baseForm->typeID == typeID))
			return true;
	}
	return false;
}

bool Cmd_GetHasContactType_Execute(COMMAND_ARGS)
{
	UInt32 typeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &typeID))
		*result = GetHasContactType(thisObj, typeID);
	else *result = 0;
	return true;
}

bool Cmd_GetHasContactType_Eval(COMMAND_ARGS_EVAL)
{
	*result = GetHasContactType(thisObj, (UInt32)arg1);
	return true;
}

bool Cmd_GetContactRefs_Execute(COMMAND_ARGS)
{
	*result = 0;
	s_tempFormList.Clear();
	hkpWorldObject **bodies;
	UInt32 count;
	TESObjectREFR *refr;
	if (thisObj->IsActor())
	{
		if (!((Actor*)thisObj)->baseProcess) return true;
		bhkCharacterController *charCtrl = ((Actor*)thisObj)->baseProcess->GetCharacterController();
		if (!charCtrl) return true;
		if (!charCtrl->byte608 && charCtrl->bodyUnderFeet)
		{
			refr = GetCdBodyRef(&charCtrl->bodyUnderFeet->cdBody);
			if (refr) s_tempFormList.Insert(refr);
		}
		bodies = charCtrl->pointCollector.contactBodies.data;
		count = charCtrl->pointCollector.contactBodies.size;
	}
	else
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (!niNode) return true;
		ContactObjects contactObjs;
		niNode->GetContactObjects(&contactObjs);
		bodies = contactObjs.Data();
		count = contactObjs.Size();
	}
	for (; count; count--, bodies++)
	{
		refr = GetCdBodyRef(&(*bodies)->cdBody);
		if (refr) s_tempFormList.Insert(refr);
	}
	if (!s_tempFormList.Empty())
	{
		s_tempElements.Clear();
		for (auto refIter = s_tempFormList.Begin(); refIter; ++refIter)
			s_tempElements.Append(*refIter);
		AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	}
	return true;
}

bool Cmd_GetHasPhantom_Execute(COMMAND_ARGS)
{
	if (thisObj->IsActor())
	{
		*result = 1;
		return true;
	}
	NiNode *niNode = thisObj->GetNiNode();
	*result = (niNode && niNode->HasPhantom()) ? 1 : 0;
	return true;
}

bool Cmd_GetInteractionDisabled_Execute(COMMAND_ARGS)
{
	TESForm *form = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form) && (form || (form = thisObj)))
		*result = (form->jipFormFlags6 & kHookFormFlag6_NoPCActivation) ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetInteractionDisabled_Execute(COMMAND_ARGS)
{
	UInt32 disable;
	TESForm *form = NULL;
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
		HOOK_MOD(PCActivateRef, add);
		HOOK_MOD(SetRolloverText, add);
	}
	return true;
}

bool Cmd_GetInteractionDisabledType_Execute(COMMAND_ARGS)
{
	UInt32 typeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &typeID) && (typeID < kFormType_Max) && kValidFilterForm[typeID])
		*result = s_activationDisabledTypes[typeID] ? 1 : 0;
	else *result = 0;
	return true;
}

bool Cmd_SetInteractionDisabledType_Execute(COMMAND_ARGS)
{
	UInt32 disable, typeID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &disable, &typeID) && (typeID < kFormType_Max) && kValidFilterForm[typeID] && (!disable == s_activationDisabledTypes[typeID]))
	{
		bool add = (disable != 0);
		s_activationDisabledTypes[typeID] = add;
		HOOK_MOD(PCActivateRef, add);
		HOOK_MOD(SetRolloverText, add);
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
	HOOK_MOD(CreateMapMarkers, true);
	return true;
}

bool Cmd_RemoveRefMapMarker_Execute(COMMAND_ARGS)
{
	SInt32 idx = s_refMapMarkersList.GetIndexOf(thisObj);
	if ((idx == -1) || (idx >= s_refMapMarkersCount)) return true;
	if (--s_refMapMarkersCount) s_refMapMarkersList.RemoveNth(idx);
	else s_refMapMarkersList.Init();
	HOOK_MOD(CreateMapMarkers, false);
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
	*result = 0;
	return true;
}

bool Cmd_SetPosEx_Execute(COMMAND_ARGS)
{
	NiVector3 posVector;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &posVector.x, &posVector.y, &posVector.z))
		thisObj->SetPos(&posVector);
	return true;
}

bool Cmd_MoveToReticle_Execute(COMMAND_ARGS)
{
	*result = 0;
	float maxRange = 10000.0F, posXmod = 0, posYmod = 0, posZmod = 0;
	UInt8 numArgs = NUM_ARGS;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &maxRange, &posXmod, &posYmod, &posZmod))
	{
		NiVector3 coords;
		if (coords.RayCastCoords(&g_thePlayer->cameraPos, &g_sceneGraph->camera->m_worldRotate, maxRange))
		{
			if (numArgs > 1)
			{
				coords.x += posXmod;
				coords.y += posYmod;
				coords.z += posZmod;
			}
			*result = thisObj->MoveToCell(g_thePlayer->parentCell, &coords);
		}
	}
	return true;
}

bool Cmd_SetRefName_Execute(COMMAND_ARGS)
{
	UInt8 numArgs = NUM_ARGS;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer) || !thisObj->ValidForHooks()) return true;
	if (numArgs)
	{
		char **namePtr;
		if (s_refNamesMap.Insert(thisObj, &namePtr))
		{
			thisObj->SetJIPFlag(kHookFormFlag6_AltRefName, true);
			HOOK_MOD(GetRefName, true);
		}
		else free(*namePtr);
		*namePtr = CopyString(s_strArgBuffer);
	}
	else
	{
		char *refName = s_refNamesMap.GetErase(thisObj);
		if (refName)
		{
			thisObj->SetJIPFlag(kHookFormFlag6_AltRefName, false);
			HOOK_MOD(GetRefName, false);
			free(refName);
		}
	}
	return true;
}

bool Cmd_SetAngleEx_Execute(COMMAND_ARGS)
{
	NiVector3 rotVector;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &rotVector.x, &rotVector.y, &rotVector.z))
		thisObj->SetAngle(&rotVector);
	return true;
}

bool Cmd_GetTeleportDoor_Execute(COMMAND_ARGS)
{
	*result = 0;
	if IS_ID(thisObj->baseForm, TESObjectDOOR)
	{
		ExtraTeleport *xTeleport = GetExtraType(&thisObj->extraDataList, Teleport);
		if (xTeleport && xTeleport->data && xTeleport->data->linkedDoor)
			REFR_RES = xTeleport->data->linkedDoor->refID;
	}
	return true;
}

bool Cmd_SetOnCriticalHitEventHandler_Execute(COMMAND_ARGS)
{
	Script *script;
	UInt32 addEvnt;
	Actor *target = NULL;
	TESObjectREFR *source = NULL;
	TESObjectWEAP *weapon = NULL;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &script, &addEvnt, &target, &source, &weapon) || NOT_ID(script, Script)) return true;
	if (target)
	{
		if (!target->IsActor())
		{
			if (!source) return true;
			target = NULL;
		}
		if (source)
		{
			if (!source->GetIsReference())
			{
				if (!weapon) return true;
				source = NULL;
			}
			if (weapon && NOT_ID(weapon, TESObjectWEAP))
				return true;
		}
	}
	CriticalHitEventData evntData(target, source, weapon, script);
	if (addEvnt && s_criticalHitEvents.Find(CriticalHitEventFind(evntData))) return true;
	s_criticalHitEvents.Remove(CriticalHitEventRemove(evntData));
	if (addEvnt) s_criticalHitEvents.Append(evntData);
	return true;
}

bool Cmd_MoveToFadeDelay_Execute(COMMAND_ARGS)
{
	UInt8 *data = (UInt8*)scriptData + *opcodeOffsetPtr;
	*(UInt8*)data = 1;
	Actor *actor = (Actor*)thisObj;
	if (actor->IsActor() && actor->baseProcess && !actor->baseProcess->processLevel)
		((HighProcess*)actor->baseProcess)->delayTime = *(double*)(data + 6);
	static const CommandInfo *cmdInfo = NULL;
	if (!cmdInfo) cmdInfo = GetCmdByOpcode(0x124F);
	paramInfo = cmdInfo->params;
	cmdInfo->execute(PASS_COMMAND_ARGS);
	return true;
}

bool Cmd_GetCrosshairWater_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESWaterForm *water = NULL;
	TESObjectREFR *refr = g_interfaceManager->crosshairRef;
	if (refr && IS_ID(refr->baseForm, BGSPlaceableWater))
		water = ((BGSPlaceableWater*)refr->baseForm)->water;
	if (water) REFR_RES = water->refID;
	DoConsolePrint(water);
	return true;
}

bool Cmd_IsAnimPlayingEx_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 category, subType = 0, flags = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &category, &subType, &flags))
	{
		AnimData *animData = thisObj->GetAnimData();
		if (animData)
		{
			UInt32 animID;
			const AnimGroupClassify *classify;
			for (UInt16 groupID : animData->animGroupIDs)
			{
				animID = groupID & 0xFF;
				if (animID >= 245) continue;
				classify = &s_animGroupClassify[animID];
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
	float totalMass = 0;
	if (!thisObj->IsActor())
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (niNode) niNode->GetBodyMass(&totalMass);
	}
	*result = totalMass;
	return true;
}

bool Cmd_PushObject_Execute(COMMAND_ARGS)
{
	hkVector4 forceVector;
	TESObjectREFR *refr = NULL;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &forceVector.x, &forceVector.y, &forceVector.z, &forceVector.w, &refr))
	{
		NiNode *niNode = thisObj->GetNiNode();
		if (niNode)
		{
			if (!refr) refr = thisObj;
			forceVector.x += refr->posX;
			forceVector.y += refr->posY;
			forceVector.z += refr->posZ;
			niNode->ApplyForce(&forceVector);
		}
	}
	return true;
}

bool Cmd_MoveToContainer_Execute(COMMAND_ARGS)
{
	TESObjectREFR *target;
	UInt32 clrOwner = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &target, &clrOwner) && kInventoryType[thisObj->baseForm->typeID] && target->GetContainer())
	{
		SInt32 quantity = 1;
		ExtraDataList *xDataList = NULL;
		if (thisObj->extraDataList.m_data)
		{
			ExtraCount *xCount = GetExtraType(&thisObj->extraDataList, Count);
			if (xCount) quantity = xCount->count;
			if (clrOwner) RemoveExtraType(&thisObj->extraDataList, kExtraData_Ownership);
			xDataList = thisObj->extraDataList.CreateCopy();
			ClearExtraDataList(&thisObj->extraDataList, true);
		}
		target->AddItem(thisObj->baseForm, xDataList, quantity);
		thisObj->DeleteReference();
	}
	return true;
}

bool Cmd_GetNifBlockTranslation_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 getWorld = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &getWorld))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock)
		{
			NiVector3 &transltn = getWorld ? niBlock->m_worldTranslate : niBlock->m_localTranslate;
			ArrayElementL elements[3] = {transltn.x, transltn.y, transltn.z};
			AssignCommandResult(CreateArray(elements, 3, scriptObj), result);
		}
	}
	return true;
}

bool Cmd_SetNifBlockTranslation_Execute(COMMAND_ARGS)
{
	NiVector3 transltn;
	UInt32 transform = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &transltn.x, &transltn.y, &transltn.z, &transform))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock)
		{
			niBlock->m_localTranslate = transltn;
			if (thisObj->IsActor()) niBlock->UpdateTransform(0);
			else niBlock->UpdateBounds(0);
		}
	}
	return true;
}

bool Cmd_GetNifBlockRotation_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 getWorld = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &getWorld))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock)
		{
			float rotX, rotY, rotZ;
			NiMatrix33 &rotMat = getWorld ? niBlock->m_worldRotate : niBlock->m_localRotate;
			rotMat.ExtractAngles(&rotX, &rotY, &rotZ);
			ArrayElementL elements[3] = {rotX / kDblPId180, rotY / kDblPId180, rotZ / kDblPId180};
			AssignCommandResult(CreateArray(elements, 3, scriptObj), result);
		}
	}
	return true;
}

bool Cmd_SetNifBlockRotation_Execute(COMMAND_ARGS)
{
	float rotX, rotY, rotZ;
	UInt32 transform = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &rotX, &rotY, &rotZ, &transform))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock)
		{
			if (transform) niBlock->m_localRotate.Rotate(rotX * kFltPId180, rotY * kFltPId180, rotZ * kFltPId180);
			else niBlock->m_localRotate.RotationMatrix(rotX * kFltPId180, rotY * kFltPId180, rotZ * kFltPId180);
			if (thisObj->IsActor()) niBlock->UpdateTransform(0);
			else niBlock->UpdateBounds(0);
		}
	}
	return true;
}

bool Cmd_GetNifBlockScale_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock) *result = niBlock->m_localScale;
	}
	return true;
}

bool Cmd_SetNifBlockScale_Execute(COMMAND_ARGS)
{
	float newScale;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &newScale))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock)
		{
			niBlock->m_localScale = newScale;
			if (thisObj->IsActor()) niBlock->UpdateTransform(0);
			else niBlock->UpdateBounds(0);
		}
	}
	return true;
}

bool Cmd_GetNifBlockFlag_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 flagID;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &flagID) && (flagID <= 31))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock) *result = (niBlock->m_flags & (1 << flagID)) != 0;
	}
	return true;
}

bool Cmd_SetNifBlockFlag_Execute(COMMAND_ARGS)
{
	UInt32 flagID, doSet;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &flagID, &doSet) && (flagID <= 31))
	{
		NiAVObject *niBlock = thisObj->GetNiBlock(s_strArgBuffer);
		if (niBlock)
		{
			if (doSet) niBlock->m_flags |= (1 << flagID);
			else niBlock->m_flags &= ~(1 << flagID);
		}
	}
	return true;
}

bool Cmd_GetObjectVelocity_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &axis))
	{
		NiNode *objNode = thisObj->GetNiNode();
		if (objNode && objNode->m_collisionObject)
		{
			bhkWorldObject *hWorldObj = objNode->m_collisionObject->worldObj;
			if (hWorldObj && hWorldObj->refObject)
			{
				hkpRigidBody *rigidBody = (hkpRigidBody*)hWorldObj->refObject;
				if ((rigidBody->motion.type <= 3) || (rigidBody->motion.type == 6))
				{
					float *velocity = (float*)&rigidBody->motion.linVelocity;
					if (axis) *result = rigidBody->motion.linVelocity[axis - 'X'];
					else *result = Vector3Length(&rigidBody->motion.linVelocity);
				}
			}
		}
	}
	return true;
}

bool Cmd_GetAngularVelocity_Execute(COMMAND_ARGS)
{
	*result = 0;
	char axis;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &axis))
	{
		hkpRigidBody *rigidBody = thisObj->GetRigidBody(s_strArgBuffer);
		if (rigidBody) *result = rigidBody->motion.angVelocity[axis - 'X'];
	}
	return true;
}

bool Cmd_SetAngularVelocity_Execute(COMMAND_ARGS)
{
	char axis;
	float velocity;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &axis, &velocity))
	{
		hkpRigidBody *rigidBody = thisObj->GetRigidBody(s_strArgBuffer);
		if (rigidBody)
		{
			rigidBody->motion.angVelocity[axis - 'X'] = velocity;
			rigidBody->UpdateMotion();
		}
	}
	return true;
}

bool Cmd_PlaceAtCell_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 count;
	TESObjectCELL *worldOrCell;
	TESObjectREFR *tempPosMarker = s_tempPosMarker;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &form, &count, &worldOrCell, &tempPosMarker->posX, &tempPosMarker->posY, &tempPosMarker->posZ))
	{
		if NOT_ID(worldOrCell, TESObjectCELL)
		{
			if NOT_ID(worldOrCell, TESWorldSpace)
				return true;
			worldOrCell = ((TESWorldSpace*)worldOrCell)->cell;
		}
		tempPosMarker->parentCell = worldOrCell;
		TESObjectREFR *placedRef = PlaceAtMe(tempPosMarker, form, count, 0, 0, 1);
		if (placedRef) REFR_RES = placedRef->refID;
	}
	return true;
}

bool Cmd_GetRayCastPos_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESGlobal *outX, *outY, *outZ;
	float posZmod = 0;
	UInt32 filter = 6;
	s_strArgBuffer[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &outX, &outY, &outZ, &posZmod, &filter, &s_strArgBuffer))
	{
		NiNode *objNode = thisObj->GetNode(s_strArgBuffer);
		if (objNode)
		{
			NiVector3 coords, posVector = objNode->m_worldTranslate;
			posVector.z += posZmod;
			filter &= 0x3F;
			if (coords.RayCastCoords(&posVector, &objNode->m_worldRotate, 50000.0F, 4, filter))
			{
				outX->data = coords.x;
				outY->data = coords.y;
				outZ->data = coords.z;
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_GetAnimSequenceFrequency_Execute(COMMAND_ARGS)
{
	*result = -1;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		NiNode *rootNode = thisObj->GetNiNode();
		if (rootNode)
		{
			NiControllerManager *ctrlMgr = (NiControllerManager*)rootNode->m_controller;
			if (ctrlMgr && IS_TYPE(ctrlMgr, NiControllerManager))
			{
				NiControllerSequence *sequence = ctrlMgr->seqStrMap.Lookup(s_strArgBuffer);
				if (sequence) *result = sequence->frequency;
			}
		}
	}
	return true;
}

bool Cmd_SetAnimSequenceFrequency_Execute(COMMAND_ARGS)
{
	float frequency;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &frequency))
	{
		NiNode *rootNode = thisObj->GetNiNode();
		if (rootNode)
		{
			NiControllerManager *ctrlMgr = (NiControllerManager*)rootNode->m_controller;
			if (ctrlMgr && IS_TYPE(ctrlMgr, NiControllerManager))
			{
				if (s_strArgBuffer[0] == '*')
					for (auto iter = ctrlMgr->sequences.Begin(); !iter.End(); ++iter)
						iter->frequency = frequency;
				else
				{
					NiControllerSequence *sequence = ctrlMgr->seqStrMap.Lookup(s_strArgBuffer);
					if (sequence) sequence->frequency = frequency;
				}
			}
		}
	}
	return true;
}

bool Cmd_MoveToNode_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *targetRef;
	NiVector3 posMods(0, 0, 0);
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &targetRef, &s_strArgBuffer, &posMods.x, &posMods.y, &posMods.z))
	{
		NiNode *targetNode = targetRef->GetNode(s_strArgBuffer);
		if (targetNode)
		{
			posMods.x += targetNode->m_worldTranslate.x;
			posMods.y += targetNode->m_worldTranslate.y;
			posMods.z += targetNode->m_worldTranslate.z;
			*result = thisObj->MoveToCell(targetRef->GetParentCell(), &posMods);
		}
	}
	return true;
}

bool Cmd_GetPlayerPerks_Execute(COMMAND_ARGS)
{
	*result = 0;
	s_tempElements.Clear();
	ListNode<PerkRank> *iter = g_thePlayer->perkRanksPC.Head();
	BGSPerk *perk;
	do
	{
		if (iter->data)
		{
			perk = iter->data->perk;
			if (perk && !perk->data.isHidden)
				s_tempElements.Append(perk);
		}
	}
	while (iter = iter->next);
	AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
	return true;
}

bool Cmd_GetNifBlockParentNodes_Execute(COMMAND_ARGS)
{
	*result = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		NiNode *rootNode = thisObj->GetNiNode();
		if (rootNode)
		{
			NiAVObject *niBlock = rootNode->GetBlock(s_strArgBuffer);
			if (niBlock)
			{
				s_tempElements.Clear();
				while ((niBlock != rootNode) && (niBlock = niBlock->m_parent))
					s_tempElements.Append(niBlock->m_blockName);
				AssignCommandResult(CreateArray(s_tempElements.Data(), s_tempElements.Size(), scriptObj), result);
			}
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
		sub		esp, 0x60
		mov		eax, g_thePlayer
		add		eax, 0xDD4
		lea		ecx, [ebp-0xC]
		mov		edx, [eax]
		mov		[ecx], edx
		mov		edx, [eax+4]
		mov		[ecx+4], edx
		mov		edx, [eax+8]
		mov		[ecx+8], edx
		mov		ecx, g_thePlayer
		cmp		byte ptr [ecx+0x64B], 0
		jz		firstPerson
		lea		eax, [ebp-0x18]
		push	eax
		CALL_EAX(0x70C410)
		push	eax
		lea		ecx, [ebp-0xC]
		CALL_EAX(0x63C8A0)
		mov		ecx, g_thePlayer
	firstPerson:
		push	0
		CALL_EAX(0x953F20)
		fstp	dword ptr [ebp-0x18]
		push	dword ptr [ebp-0x18]
		lea		ecx, [ebp-0x3C]
		CALL_EAX(0x4A0C90)
		mov		eax, g_thePlayer
		push	dword ptr [eax+0x24]
		lea		ecx, [ebp-0x60]
		CALL_EAX(0x524AC0)
		lea		eax, [ebp-0x60]
		push	eax
		lea		ecx, [ebp-0x3C]
		push	ecx
		CALL_EAX(0x43F8D0)
		lea		eax, [ebp-0x18]
		push	eax
		push	1
		lea		ecx, [ebp-0x3C]
		CALL_EAX(0x439F50)
		lea		eax, [ebp-0x60]
		push	eax
		lea		eax, [ebp-0x3C]
		push	eax
		push	0x45FE0000
		lea		eax, [ebp-0x18]
		push	eax
		lea		eax, [ebp-0xC]
		push	eax
		mov		ecx, g_interfaceManager
		mov		ecx, [ecx+0x13C]
		CALL_EAX(0x631D60)
		leave
		retn
	}
}

bool Cmd_GetCrosshairRefEx_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectREFR *resRefr = g_interfaceManager->crosshairRef;
	if (!resRefr)
	{
		resRefr = GetCrosshairRef();
		if (!resRefr) return true;
	}
	REFR_RES = resRefr->refID;
	return true;
}

bool Cmd_IsMobile_Execute(COMMAND_ARGS)
{
	*result = thisObj->IsMobile();
	return true;
}

bool Cmd_IsGrabbable_Execute(COMMAND_ARGS)
{
	*result = thisObj->IsGrabbable();
	return true;
}

bool Cmd_AttachLight_Execute(COMMAND_ARGS)
{
	*result = 0;
	TESObjectLIGH *lightForm;
	NiVector3 offsetMod(0, 0, 0);
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &lightForm, &offsetMod.x, &offsetMod.y, &offsetMod.z) && IS_ID(lightForm, TESObjectLIGH))
	{
		NiNode *objNode = thisObj->GetNode(s_strArgBuffer);
		if (objNode)
		{
			NiPointLight *pointLight = NULL;
			for (auto iter = objNode->m_children.Begin(); !iter.End(); ++iter)
			{
				if NOT_TYPE(*iter, NiPointLight) continue;
				pointLight = (NiPointLight*)*iter;
				break;
			}
			if (pointLight)
				pointLight->directionalColor = {lightForm->red / 255.0F, lightForm->green / 255.0F, lightForm->blue / 255.0F};
			else pointLight = lightForm->CreatePointLight(thisObj, objNode, 0);
			if (pointLight)
			{
				pointLight->unkA8 = 0x12E;
				pointLight->ambientColor = {1, 1, 1};
				float radius = (int)lightForm->radius;
				pointLight->radius = radius;
				pointLight->radiusE4 = radius;
				pointLight->radiusE8 = radius;
				pointLight->attenuation2 = 1;
				pointLight->attenuation3 = 0;
				pointLight->m_localTranslate = offsetMod;
				AttachedLights *attachedLights;
				if (s_attachedLightsMap.Insert(thisObj->refID, &attachedLights))
					attachedLights->refrPtr = thisObj;
				attachedLights->nodeNames.Insert(s_strArgBuffer);
				*result = 1;
			}
		}
	}
	return true;
}

bool Cmd_RemoveLight_Execute(COMMAND_ARGS)
{
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer))
	{
		auto findID = s_attachedLightsMap.Find(thisObj->refID);
		if (!findID) return true;
		if (LookupFormByRefID(thisObj->refID) != findID().refrPtr)
			findID().nodeNames.Clear();
		else if (findID().nodeNames.Erase(s_strArgBuffer))
			RemoveAttachedLight(findID().refrPtr, s_strArgBuffer);
		if (findID().nodeNames.Empty())
			findID.Remove();
	}
	return true;
}

bool Cmd_GetExtraFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	ExtraDataList *xData = NULL;
	InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
	if (invRef)
		xData = invRef->xData;
	else xData = &thisObj->extraDataList;
	if (xData)
	{
		ExtraCharge *xCharge = GetExtraType(xData, Charge);
		if (xCharge)
			*result = xCharge->charge;
	}
	return true;
}

bool Cmd_SetExtraFloat_Execute(COMMAND_ARGS)
{
	float fltVal;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &fltVal))
	{
		ExtraDataList *xData = NULL;
		InventoryRef *invRef = InventoryRefGetForID(thisObj->refID);
		if (invRef)
		{
			xData = invRef->xData;
			if (!xData)
			{
				if (xData = invRef->CreateExtraData())
				{
					SInt32 count = invRef->GetCount();
					if (count > 1)
						AddExtraData(xData, ExtraCount::Create(count));
					AddExtraData(xData, ExtraCharge::Create(fltVal));
				}
				return true;
			}
		}
		else xData = &thisObj->extraDataList;
		ExtraCharge *xCharge = GetExtraType(xData, Charge);
		if (xCharge)
			xCharge->charge = fltVal;
		else AddExtraData(xData, ExtraCharge::Create(fltVal));
	}
	return true;
}

bool Cmd_SetLinearVelocity_Execute(COMMAND_ARGS)
{
	AlignedVector4 velocity;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &s_strArgBuffer, &velocity.x, &velocity.y, &velocity.z))
	{
		hkpRigidBody *rigidBody = thisObj->GetRigidBody(s_strArgBuffer);
		if (rigidBody)
		{
			velocity.w = 0;
			rigidBody->motion.SetLinearVelocity(&velocity);
			rigidBody->UpdateMotion();
		}
	}
	return true;
}

UInt16 s_insertObjectFlag = 0;

bool RegisterInsertObject(COMMAND_ARGS)
{
	*result = 0;
	TESForm *form;
	UInt32 doInsert;
	if (ExtractFormatStringArgs(2, s_strArgBuffer, EXTRACT_ARGS_EX, kCommandInfo_AttachModel.numParams, &form, &doInsert) && (form->GetIsReference() || form->IsBoundObject()))
	{
		UnorderedMap<TESForm*, NodeNamesMap> &formsMap = (s_insertObjectFlag == kHookFormFlag6_InsertNode) ? s_insertNodeMap : s_attachModelMap;

		char *nodeName = "", *objectName = strrchr(s_strArgBuffer, '|');
		if (objectName)
		{
			*objectName++ = 0;
			nodeName = s_strArgBuffer;
		}
		else objectName = s_strArgBuffer;

		if (doInsert)
		{
			NodeNamesMap *namesMap;
			if (formsMap.Insert(form, &namesMap))
			{
				form->SetJIPFlag(s_insertObjectFlag, true);
				HOOK_MOD(InsertObject, true);
			}
			if (!(*namesMap)[nodeName].Insert(objectName))
				return true;
		}
		else
		{
			auto findForm = formsMap.Find(form);
			if (!findForm) return true;
			auto findNode = findForm().FindOp(nodeName);
			if (!findNode || !findNode().Erase(objectName))
				return true;
			if (findNode().Empty())
			{
				findNode.Remove(findForm());
				if (findForm().Empty())
				{
					findForm.Remove();
					form->SetJIPFlag(s_insertObjectFlag, false);
					HOOK_MOD(InsertObject, false);
				}
			}
		}
		*result = 1;
	}
	return true;
}

__declspec(naked) bool Cmd_InsertNode_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_insertObjectFlag, kHookFormFlag6_InsertNode
		jmp		RegisterInsertObject
	}
}

__declspec(naked) bool Cmd_AttachModel_Execute(COMMAND_ARGS)
{
	__asm
	{
		mov		s_insertObjectFlag, kHookFormFlag6_AttachModel
		jmp		RegisterInsertObject
	}
}

bool Cmd_SynchronizePosition_Execute(COMMAND_ARGS)
{
	TESObjectREFR *targetRef = NULL;
	UInt32 syncRot = 0;
	s_syncPositionMods.z = 0;
	if (NUM_ARGS) s_syncPositionNode[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &targetRef, &syncRot, &s_syncPositionMods.z, &s_syncPositionNode))
	{
		if (targetRef)
		{
			if (g_thePlayer->GetDistance(targetRef) > 1024.0F)
				g_thePlayer->MoveToCell(targetRef->GetParentCell(), targetRef->PosVector());
			s_syncPositionRef = targetRef;
			s_syncPositionFlags = (syncRot != 0);
			_mm_store_ps(&s_syncPositionPos.x, _mm_loadu_ps(&targetRef->posX));
			HOOK_SET(SynchronizePosition, true);
		}
		else
		{
			HOOK_SET(SynchronizePosition, false);
			if (targetRef = s_syncPositionRef)
			{
				s_syncPositionRef = NULL;
				g_thePlayer->MoveToCell(targetRef->GetParentCell(), (NiVector3*)&s_syncPositionPos);
			}
		}
	}
	return true;
}

bool Cmd_ModelHasBlock_Execute(COMMAND_ARGS)
{
	char *blockName = s_strArgBuffer;
	*blockName = '^';
	TESForm *form;
	if (ExtractFormatStringArgs(1, blockName + 1, EXTRACT_ARGS_EX, kCommandInfo_ModelHasBlock.numParams, &form))
	{
		TESObjectREFR *refr = form->GetIsReference() ? (TESObjectREFR*)form : NULL;
		NiNode *rootNode = refr ? refr->GetNiNode() : NULL;
		if (rootNode && rootNode->GetBlock(blockName + 1))
			goto Retn1;
		NodeNamesMap *namesMap = s_insertNodeMap.GetPtr(form);
		if (namesMap)
		{
			for (auto iter = namesMap->Begin(); iter; ++iter)
				if (iter().HasKey(blockName + 1) || iter().HasKey(blockName))
					goto Retn1;
		}
		if (refr)
		{
			form = refr->GetBaseForm2();
			if (!form) goto Retn0;
			namesMap = s_insertNodeMap.GetPtr(form);
			if (namesMap)
			{
				for (auto iter = namesMap->Begin(); iter; ++iter)
					if (iter().HasKey(blockName + 1) || iter().HasKey(blockName))
						goto Retn1;
			}
		}
		if (!rootNode && form->IsBoundObject())
		{
			const char *modelPath = form->GetModelPath();
			if (modelPath)
			{
				rootNode = LoadModel(g_modelLoader, modelPath, 0, 1, 0, 0, 1);
				if (rootNode && rootNode->GetBlock(blockName + 1))
					goto Retn1;
			}
		}
	}
Retn0:
	*result = 0;
	return true;
Retn1:
	*result = 1;
	return true;
}

bool Cmd_GetRayCastRef_Execute(COMMAND_ARGS)
{
	*result = 0;
	UInt32 filter = 6;
	s_strArgBuffer[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &filter, &s_strArgBuffer))
	{
		NiNode *objNode = thisObj->GetNode(s_strArgBuffer);
		if (objNode)
		{
			filter &= 0x3F;
			NiAVObject *rcObject = GetRayCastObject(&objNode->m_worldTranslate, &objNode->m_worldRotate, 50000.0F, 4, filter);
			if (rcObject)
			{
				TESObjectREFR *resRefr = CdeclCall<TESObjectREFR*>(0x56F930, rcObject);
				if (resRefr) REFR_RES = resRefr->refID;
			}
		}
	}
	return true;
}

bool Cmd_GetRayCastMaterial_Execute(COMMAND_ARGS)
{
	*result = -1;
	UInt32 filter = 6;
	s_strArgBuffer[0] = 0;
	if (ExtractArgsEx(EXTRACT_ARGS_EX, &filter, &s_strArgBuffer))
	{
		NiNode *objNode = thisObj->GetNode(s_strArgBuffer);
		if (objNode)
		{
			filter &= 0x3F;
			*result = GetRayCastMaterial(&objNode->m_worldTranslate, &objNode->m_worldRotate, 50000.0F, 4, filter);
		}
	}
	return true;
}