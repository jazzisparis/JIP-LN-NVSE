#include "internal/netimmerse.h"

__declspec(naked) bool NiControllerSequence::Play()
{
	__asm
	{
		push	ecx
		push	0
		push	0
		push	0
		push	0x3F800000
		push	0
		push	0
		CALL_EAX(0xA34F20)
		pop		ecx
		test	al, al
		jz		done
		mov		dword ptr [ecx+0x48], 0xFF7FFFFF
		mov		edx, [ecx+0x40]
		test	edx, edx
		jz		done
		or		byte ptr [edx+8], 8
	done:
		retn
	}
}

NiTransformController *NiTransformController::Create(NiNode *pTarget, NiTransformInterpolator *pInterpolator)
{
	NiTransformController *controller = CdeclCall<NiTransformController*>(0xA36810);
	controller->SetTarget(pTarget);
	controller->SetInterpolator(pInterpolator, 0);
	return controller;
}

__declspec(naked) void __fastcall NiObjectNET::SetName(const char *newName)
{
	__asm
	{
		push	ecx
		push	edx
		CALL_EAX(0xA5B690)
		pop		ecx
		pop		ecx
		mov		edx, [ecx+8]
		cmp		eax, edx
		jz		decCount
		test	edx, edx
		jz		noCurrName
		lock dec dword ptr [edx-8]
	noCurrName:
		mov		[ecx+8], eax
		retn
	decCount:
		test	eax, eax
		jz		done
		lock dec dword ptr [eax-8]
	done:
		retn
	}
}

__declspec(naked) NiExtraData* __fastcall NiObjectNET::GetExtraData(UInt32 vtbl)
{
	__asm
	{
		push	esi
		mov		esi, [ecx+0x10]
		movzx	ecx, word ptr [ecx+0x14]
		ALIGN 16
	iterHead:
		dec		ecx
		js		retnNULL
		mov		eax, [esi+ecx*4]
		test	eax, eax
		jz		iterHead
		cmp		[eax], edx
		jnz		iterHead
		pop		esi
		retn
	retnNULL:
		xor		eax, eax
		pop		esi
		retn
	}
}

void NiObjectNET::DumpExtraData()
{
	s_debug.Indent();
	NiExtraData *xData;
	for (UInt32 iter = 0; iter < m_extraDataListLen; iter++)
	{
		xData = m_extraDataList[iter];
		if (xData)
			PrintDebug("(X) %08X\t%s\t#%d", xData, xData->GetType()->name, xData->m_uiRefCount);
	}
	s_debug.Outdent();
}

void NiMaterialProperty::SetTraitValue(UInt32 traitID, float value)
{
	switch (traitID)
	{
		case 0:
		case 1:
		case 2:
			specularRGB[traitID] = value;
			break;
		case 3:
		case 4:
		case 5:
			emissiveRGB[traitID - 3] = value;
			break;
		case 6:
			glossiness = value;
			break;
		case 7:
			alpha = value;
			break;
		default:
			emitMult = value;
	}
}

__declspec(naked) void NiAVObject::Update()
{
	__asm
	{
		push	ecx
		push	0
		push	0
		push	0
		mov		edx, esp
		push	0
		push	edx
		mov		eax, [ecx]
		call	dword ptr [eax+0xA4]
		add		esp, 0xC
		pop		ecx
		mov		ecx, [ecx+0x18]
		test	ecx, ecx
		jz		done
		mov		eax, [ecx]
		call	dword ptr [eax+0xFC]
	done:
		retn
	}
}

__declspec(naked) UInt32 NiAVObject::GetIndex()
{
	__asm
	{
		mov		eax, [ecx+0x18]
		test	eax, eax
		jz		done
		mov		edx, [eax+0xA0]
		movzx	eax, word ptr [eax+0xA6]
		test	eax, eax
		jz		done
		ALIGN 16
	iterHead:
		cmp		[edx+eax*4-4], ecx
		jz		done
		dec		eax
		jnz		iterHead
	done:
		retn
	}
}

__declspec(naked) bool __fastcall NiAVObject::ReplaceObject(NiAVObject *object)
{
	__asm
	{
		push	esi
		mov		eax, [ecx+0x18]
		test	eax, eax
		jz		done
		mov		esi, [eax+0xA0]
		movzx	eax, word ptr [eax+0xA6]
		test	eax, eax
		jz		done
		ALIGN 16
	iterHead:
		cmp		[esi], ecx
		jz		found
		add		esi, 4
		dec		eax
		jnz		iterHead
	done:
		pop		esi
		retn
	found:
		mov		eax, [ecx+0x18]
		mov		dword ptr [ecx+0x18], 0
		mov		[edx+0x18], eax
		push	edx
		push	esi
		call	NiReleaseAddRef
		mov		al, 1
		pop		esi
		retn
	}
}

__declspec(naked) NiProperty* __fastcall NiAVObject::GetProperty(UInt32 propID)
{
	__asm
	{
		push	esi
		mov		esi, [ecx+0x24]
	iterHead:
		test	esi, esi
		jz		retnNULL
		mov		ecx, [esi+8]
		mov		esi, [esi]
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		call	dword ptr [eax+0x8C]
		cmp		eax, edx
		jnz		iterHead
		mov		eax, ecx
		pop		esi
		retn
	retnNULL:
		xor		eax, eax
		pop		esi
		retn
	}
}

void NiAVObject::DumpProperties()
{
	s_debug.Indent();
	NiProperty *niProp;
	for (DListNode<NiProperty> *traverse = m_propertyList.Head(); traverse; traverse = traverse->next)
	{
		niProp = traverse->data;
		if (niProp)
		{
			PrintDebug("(P) %08X\t%s\t#%d", niProp, niProp->GetType()->name, niProp->m_uiRefCount);
			if (niProp->m_controller)
				PrintDebug("\t(C) %08X\t%s\t#%d", niProp->m_controller, niProp->m_controller->GetType()->name, niProp->m_controller->m_uiRefCount);
			niProp->DumpExtraData();
		}
	}
	s_debug.Outdent();
}

void NiAVObject::DumpParents()
{
	if (m_parent)
		m_parent->DumpParents();
	PrintDebug("%08X\t%s\t%s", this, GetType()->name, GetName());
	//s_debug.Indent();
}

__declspec(naked) NiNode* __stdcall NiNode::Create(const char *nameStr)	//	str of NiString
{
	__asm
	{
		CALL_EAX(0xA5F030)
		mov		ecx, [esp+4]
		test	ecx, ecx
		jz		done
		lock inc dword ptr [ecx-8]
		mov		[eax+8], ecx
	done:
		retn	4
	}
}

__declspec(noinline) NiObjectCopyInfo *GetNiObjectCopyInfo()
{
	thread_local NiObjectCopyInfo s_NiObjectCopyInfo(0x97);
	return &s_NiObjectCopyInfo;
}

void NiTMapBase<int, int>::FreeBuckets();

__declspec(naked) NiNode *NiNode::CreateCopy()
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		call	GetNiObjectCopyInfo
		mov		edi, eax
		push	eax
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x48]
		push	edi
		mov		ecx, esi
		mov		esi, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0x68]
		and		byte ptr [esi+0x30], 0xFE
		mov		edx, 0x3F800000
		movd	xmm0, edx
		movups	[esi+0x34], xmm0
		movups	[esi+0x44], xmm0
		movups	[esi+0x54], xmm0
		mov		ecx, [edi]
		call	NiTMapBase<int, int>::FreeBuckets
		mov		ecx, [edi+4]
		call	NiTMapBase<int, int>::FreeBuckets
		mov		eax, esi
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlockByName(const char *nameStr)	//	str of NiString
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		eax, [esi]
		add		esi, 4
		test	eax, eax
		jz		iterHead
		cmp		[eax+8], edx
		jz		found
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0xC], kAddr_ReturnThis
		jnz		iterHead
		mov		ecx, eax
		call	NiNode::GetBlockByName
		test	eax, eax
		jz		iterHead
	found:
		pop		edi
		pop		esi
		retn
		ALIGN 16
	iterEnd:
		xor		eax, eax
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlock(const char *blockName)
{
	__asm
	{
		cmp		[edx], 0
		jz		retnNULL
		push	ecx
		push	edx
		CALL_EAX(0xA5B690)
		pop		ecx
		pop		ecx
		test	eax, eax
		jz		done
		lock dec dword ptr [eax-8]
		jz		retnNULL
		cmp		[ecx+8], eax
		jz		found
		mov		edx, eax
		call	NiNode::GetBlockByName
		retn
	found:
		mov		eax, ecx
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) NiNode* __fastcall NiNode::GetNode(const char *nodeName)
{
	__asm
	{
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		xor		edx, edx
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0xC], kAddr_ReturnThis
		cmovnz	eax, edx
	done:
		retn
	}
}

bool NiNode::IsMovable()
{
	if (m_collisionObject)
	{
		bhkWorldObject *hWorldObj = m_collisionObject->worldObj;
		if (hWorldObj)
		{
			UInt8 motionType = ((hkpRigidBody*)hWorldObj->refObject)->motion.type;
			if ((motionType == 2) || (motionType == 3) || (motionType == 6))
				return true;
		}
	}
	for (auto iter = m_children.Begin(); iter; ++iter)
		if (*iter && IS_NODE(*iter) && ((NiNode*)*iter)->IsMovable())
			return true;
	return false;
}

__declspec(naked) void NiNode::RemoveCollision()
{
	__asm
	{
		lea		eax, [ecx+0x1C]
		cmp		dword ptr [eax], 0
		jz		noColObj
		push	ecx
		push	0
		push	eax
		call	NiReleaseAddRef
		pop		ecx
	noColObj:
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		iterHead
		call	NiNode::RemoveCollision
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

void NiNode::BulkSetMaterialPropertyTraitValue(UInt32 traitID, float value)
{
	NiAVObject *block;
	NiMaterialProperty *matProp;
	for (auto iter = m_children.Begin(); iter; ++iter)
	{
		if (!(block = *iter)) continue;
		if IS_NODE(block)
			((NiNode*)block)->BulkSetMaterialPropertyTraitValue(traitID, value);
		else if (matProp = (NiMaterialProperty*)block->GetProperty(2))
			matProp->SetTraitValue(traitID, value);
	}
}

void NiNode::GetContactObjects(ContactObjects *contactObjs)
{
	if (m_collisionObject && m_collisionObject->worldObj)
	{
		hkpWorldObject *hWorldObj = (hkpWorldObject*)m_collisionObject->worldObj->refObject;
		if (hWorldObj->collisionType == 1)
		{
			hkpRigidBody *rigidBody = (hkpRigidBody*)hWorldObj;
			ConstraintContact *contactsArr = rigidBody->contactsArr;
			if (contactsArr)
				for (UInt16 count = rigidBody->contactsSize; count; count--, contactsArr++)
					contactObjs->Append(contactsArr->contactBody);
			if (rigidBody->constraintInst.data)
				for (auto iter = rigidBody->constraintInst.Begin(); iter; ++iter)
					contactObjs->Append(iter->contactBody);
		}
		else if IS_TYPE(hWorldObj, hkpSimpleShapePhantom)
			for (auto iter = ((hkpSimpleShapePhantom*)hWorldObj)->cdBodies.Begin(); iter; ++iter)
				contactObjs->Append(iter->GetWorldObj());
	}
	for (auto iter = m_children.Begin(); iter; ++iter)
		if (*iter && IS_NODE(*iter)) ((NiNode*)*iter)->GetContactObjects(contactObjs);
}

bool NiNode::HasPhantom()
{
	if (m_collisionObject && m_collisionObject->worldObj && (((hkpWorldObject*)m_collisionObject->worldObj->refObject)->collisionType == 2))
		return true;
	for (auto iter = m_children.Begin(); iter; ++iter)
		if (*iter && IS_NODE(*iter) && ((NiNode*)*iter)->HasPhantom())
			return true;
	return false;
}

void NiNode::GetBodyMass(float *totalMass)
{
	if (m_collisionObject && m_collisionObject->worldObj)
		*totalMass += ((hkpRigidBody*)m_collisionObject->worldObj->refObject)->motion.GetBodyMass();
	for (auto iter = m_children.Begin(); iter; ++iter)
		if (*iter && IS_NODE(*iter)) ((NiNode*)*iter)->GetBodyMass(totalMass);
}

__declspec(naked) void NiNode::ApplyForce(NiVector4 *forceVector)
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		doneCol
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		doneCol
		mov		edx, [eax+8]
		mov		dl, [edx+0xE8]
		cmp		dl, 2
		jz		doApply
		cmp		dl, 3
		jz		doApply
		cmp		dl, 6
		jnz		doneCol
	doApply:
		push	ecx
		push	dword ptr [esp+8]
		mov		ecx, eax
		call	bhkWorldObject::ApplyForce
		pop		ecx
	doneCol:
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		mov		edi, eax
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], kAddr_ReturnThis
		jnz		iterHead
		push	dword ptr [esp+0xC]
		call	NiNode::ApplyForce
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn	4
	}
}

void NiNode::Dump()
{
	PrintDebug("(N) %08X\t%s\t%s\t%08X\t#%d", this, GetType()->name, GetName(), m_flags, m_uiRefCount);
	if (m_controller)
		PrintDebug("\t(C) %08X\t%s\t#%d", m_controller, m_controller->GetType()->name, m_controller->m_uiRefCount);
	DumpExtraData();
	DumpProperties();
	s_debug.Indent();
	if (m_collisionObject)
	{
		PrintDebug("(H) %08X\t%s\t%08X", m_collisionObject, m_collisionObject->GetType()->name, m_collisionObject->flags);
		bhkWorldObject *hWorldObj = m_collisionObject->worldObj;
		if (hWorldObj)
			PrintDebug("\t(H1) %08X\t%s\t%08X", hWorldObj, hWorldObj->GetType()->name, hWorldObj->bodyFlags);
	}

	NiAVObject *block;
	for (NiTArray<NiAVObject*>::Iterator iter(m_children); iter; ++iter)
	{
		block = *iter;
		if (!block) continue;
		if IS_NODE(block) ((NiNode*)block)->Dump();
		else
		{
			PrintDebug("(B) %08X\t%s\t%s\t%08X\t#%d", block, block->GetType()->name, block->GetName(), block->m_flags, block->m_uiRefCount);
			if (block->m_controller)
				PrintDebug("\t(C) %08X\t%s\t#%d", block->m_controller, block->m_controller->GetType()->name, block->m_controller->m_uiRefCount);
			block->DumpExtraData();
			block->DumpProperties();
		}
	}
	s_debug.Outdent();
}

