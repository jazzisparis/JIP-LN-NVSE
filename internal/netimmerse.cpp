#include "internal/netimmerse.h"

__declspec(naked) NiObject* __fastcall NiObject::HasBaseType(const NiRTTI *baseType)
{
	__asm
	{
		mov		eax, [ecx]
		call	dword ptr [eax+8]
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		done
		cmp		eax, edx
		mov		eax, [eax+4]
		jnz		iterHead
		mov		eax, ecx
	done:
		retn
	}
}

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

NiControllerSequence *NiControllerManager::FindSequence(const char *seqName)
{
	for (auto iter = sequences.Begin(); iter; ++iter)
		if (!StrCompare(iter->sequenceName, seqName))
			return *iter;
	return nullptr;
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

__declspec(naked) NiExtraData* __fastcall NiObjectNET::GetExtraData(UInt32 vtbl) const
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
	s_debug().Indent();
	NiExtraData *xData;
	for (UInt32 iter = 0; iter < m_extraDataListLen; iter++)
	{
		xData = m_extraDataList[iter];
		if (xData)
			PrintDebug("(X) %08X\t%s\t#%d", xData, xData->GetType()->name, xData->m_uiRefCount);
	}
	s_debug().Outdent();
}

void __vectorcall NiMaterialProperty::SetTraitValue(UInt32 traitID, float value)
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

__declspec(naked) UInt32 NiAVObject::GetIndex() const
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
		sub		edx, 4
		ALIGN 16
	iterHead:
		cmp		[edx+eax*4], ecx
		jz		done
		dec		eax
		jnz		iterHead
	done:
		retn
	}
}

__declspec(naked) bool NiAVObject::ReplaceObject(NiAVObject *object)
{
	__asm
	{
		mov		edx, [ecx+0x18]
		test	edx, edx
		jz		done
		mov		eax, [edx+0xA0]
		movzx	edx, word ptr [edx+0xA6]
		ALIGN 16
	iterHead:
		dec		edx
		js		done
		cmp		[eax+edx*4], ecx
		jnz		iterHead
		lea		eax, [eax+edx*4]
		mov		edx, [esp+4]
		push	edx
		push	eax
		mov		eax, [ecx+0x18]
		mov		dword ptr [ecx+0x18], 0
		mov		[edx+0x18], eax
		call	NiReleaseAddRef
		mov		al, 1
		retn	4
		ALIGN 16
	done:
		xor		al, al
		retn	4
	}
}

__declspec(naked) NiProperty* __fastcall NiAVObject::GetProperty(UInt32 propID) const
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

__declspec(naked) TESObjectREFR *NiAVObject::GetParentRef() const
{
	__asm
	{
	iterHead:
		test	ecx, ecx
		jz		retnNULL
		mov		eax, ecx
		mov		ecx, [ecx+0x18]
		mov		edx, [eax]
		cmp		dword ptr [edx+0x10], ADDR_ReturnThis
		jnz		iterHead
		mov		eax, [eax+0xCC]
		test	eax, eax
		jz		iterHead
		retn
		ALIGN 16
	retnNULL:
		xor		eax, eax
		retn
	}
}

void NiAVObject::DumpProperties()
{
	s_debug().Indent();
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
	s_debug().Outdent();
}

void NiAVObject::DumpParents()
{
	if (m_parent)
		m_parent->DumpParents();
	PrintDebug("%08X\t%s\t%s", this, GetType()->name, GetName());
	//s_debug.Indent();
}

__declspec(naked) NiNode* __stdcall NiNode::Create(const char *nameStr)	//	str of NiFixedString
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
		movss	xmm0, PS_V3_One
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

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlockByName(const char *nameStr) const	//	str of NiFixedString
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
		cmp		dword ptr [ecx+0xC], ADDR_ReturnThis
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

__declspec(naked) NiAVObject* __fastcall NiNode::GetBlock(const char *blockName) const
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

__declspec(naked) NiNode* __fastcall NiNode::GetNode(const char *nodeName) const
{
	__asm
	{
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		xor		edx, edx
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0xC], ADDR_ReturnThis
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
		if (hWorldObj && ((hkpRigidBody*)hWorldObj->refObject)->IsMobile())
			return true;
	}
	for (auto iter = m_children.Begin(); iter; ++iter)
		if (*iter && IS_NODE(*iter) && ((NiNode*)*iter)->IsMovable())
			return true;
	return false;
}

__declspec(naked) void NiNode::ResetCollision()
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		noColObj
		or      byte ptr [eax+0xC], 0x40
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
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		iterHead
		call	NiNode::ResetCollision
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void NiNode::RemoveCollision()
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		noColObj
		xor		edx, edx
		mov		[ecx+0x1C], edx
		lock dec dword ptr [eax+4]
		jg		noColObj
		push	ecx
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+4]
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
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
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

__declspec(naked) BSXFlags *NiNode::GetBSXFlags()
{
	__asm
	{
		push	esi
		mov		esi, [ecx+0x10]
		movzx	ecx, word ptr [ecx+0x14]
		mov		edx, kVtbl_BSXFlags
		ALIGN 16
	iterHead:
		dec		ecx
		js		done
		mov		eax, [esi+ecx*4]
		test	eax, eax
		jz		iterHead
		cmp		[eax], edx
		jnz		iterHead
		pop		esi
		retn
	done:
		xor		eax, eax
		pop		esi
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

void NiNode::GetContactObjects(ContactObjects &contactObjs)
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
					contactObjs.Append(contactsArr->contactBody);
			if (rigidBody->constraintInst.data)
				for (auto iter = rigidBody->constraintInst.Begin(); iter; ++iter)
					contactObjs.Append(iter->contactBody);
		}
		else if IS_TYPE(hWorldObj, hkpSimpleShapePhantom)
			for (auto iter = ((hkpSimpleShapePhantom*)hWorldObj)->cdBodies.Begin(); iter; ++iter)
				contactObjs.Append(iter->GetWorldObj());
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

__declspec(naked) float __vectorcall NiNode::GetBodyMass(float totalMass) const
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		doChildren
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		doChildren
		mov		eax, [eax+8]
		cmp		byte ptr [eax+0x28], 1
		jnz		doChildren
		mov		eax, [eax+0x1AC]
		test	eax, eax
		jz		doChildren
		movd	xmm1, eax
		rcpss	xmm1, xmm1
		addss	xmm0, xmm1
	doChildren:
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
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		iterHead
		call	NiNode::GetBodyMass
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void NiNode::ApplyForce(const NiVector4 &forceVector)
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		doChildren
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		doChildren
		mov		edx, [eax+8]
		cmp		byte ptr [edx+0x28], 1
		jnz		doChildren
		test	byte ptr [edx+0xE8], 2
		jz		doChildren
		push	ecx
		push	dword ptr [esp+8]
		mov		ecx, eax
		call	bhkWorldObject::ApplyForce
		pop		ecx
	doChildren:
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
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
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

__declspec(naked) bool __fastcall NiCamera::WorldToScreen(const NiVector3 &worldPos, float zeroTolerance, NiPoint2 &scrPos)
{
	__asm
	{
		add		ecx, 0x9C
		movups	xmm0, [ecx]
		movups	xmm1, [ecx+0x10]
		movups	xmm2, [ecx+0x30]
		movups	xmm3, [edx]
		andps	xmm3, PS_XYZ0Mask
		pshufd	xmm4, PS_V3_One, 0x3F
		orps	xmm3, xmm4
		xorps	xmm4, xmm4
		mulps	xmm0, xmm3
		haddps	xmm0, xmm4
		haddps	xmm0, xmm4
		mulps	xmm1, xmm3
		haddps	xmm1, xmm4
		haddps	xmm1, xmm4
		unpcklps	xmm0, xmm1
		mulps	xmm2, xmm3
		haddps	xmm2, xmm4
		haddps	xmm2, xmm4
		comiss	xmm2, xmm4
		setnz	al
		jz		done
		movss	xmm1, [esp+4]
		cmpnltss	xmm1, xmm2
		movmskps	eax, xmm1
		movss	xmm3, PS_FlipSignMask0
		andps	xmm1, xmm3
		xorps	xmm2, xmm1
		unpcklps	xmm2, xmm2
		divps	xmm0, xmm2
		movups	xmm1, [ecx+0x64]
		shufps	xmm1, xmm1, 0xE1
		pshufd	xmm2, PS_V3_Half, 0
		mulps	xmm1, xmm2
		movaps	xmm2, xmm1
		hsubps	xmm2, xmm4
		mulps	xmm0, xmm2
		shufps	xmm3, xmm3, 0x45
		xorps	xmm1, xmm3
		haddps	xmm1, xmm4
		addps	xmm0, xmm1
		shufps	xmm3, xmm3, 8
		xorps	xmm0, xmm3
		xor		al, 1
		jz		done
		unpcklpd	xmm0, xmm0
		movq	xmm1, qword ptr PS_V3_One
		shufps	xmm3, xmm3, 0x50
		xorps	xmm0, xmm3
		cmpnltps	xmm1, xmm0
		movmskps	eax, xmm1
		cmp		al, 0xF
		setz	al
	done:
		mov		edx, [esp+8]
		movq	qword ptr [edx], xmm0
		retn	8
	}
}

__declspec(naked) bool __fastcall NiPick::GetResults(NiCamera *camera)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 0x18
		mov		ecx, edx
		lea		edx, [ebp-0x1C]
		lea		eax, [ebp-0x10]
		push	0
		push	edx
		push	eax
		push	0
		push	edx
		push	eax
		mov		eax, g_interfaceManager
		cvttss2si	edx, [eax+0x40]
		push	edx
		cvttss2si	edx, [eax+0x38]
		push	edx
		CALL_EAX(0xA71080)
		mov		ecx, [ebp-4]
		CALL_EAX(0xE98E20)
		leave
		retn
	}
}

__declspec(naked) NiSourceTexture* __fastcall NiSourceTexture::Create(const char *ddsPath)
{
	__asm
	{
		push	eax
		push	ecx
		lea		ecx, [esp+4]
		CALL_EAX(0x438170)
		push	0
		push	1
		push	0x11A9598
		push	eax
		CALL_EAX(0xA5FD70)
		add		esp, 0x10
		pop		ecx
		test	ecx, ecx
		jz		done
		lock dec dword ptr [ecx-8]
	done:
		retn
	}
}

__declspec(naked) void __fastcall NiRenderedTexture::SaveToFile(UInt32 fileFmt, char *filePath)
{
	__asm
	{
		mov		eax, [ecx+0x24]
		test	eax, eax
		jz		done
		mov		ecx, [eax+0x64]
		test	ecx, ecx
		jz		done
		mov		eax, [esp+4]
		push	0
		push	ecx
		push	edx
		push	eax
		CALL_EAX(0xEE6DC2)
	done:
		retn	4
	}
}

__declspec(naked) NiLines* __stdcall NiLines::Create(float length, const NiColorAlpha &color, const char *objName)
{
	__asm
	{
		push	2
		GAME_HEAP_ALLOC
		mov		word ptr [eax], 1
		push	eax
		push	0
		push	0
		push	0
		push	0x20
		GAME_HEAP_ALLOC
		mov		edx, [esp+0x18]
		movups	xmm0, [edx]
		movups	[eax], xmm0
		movups	[eax+0x10], xmm0
		push	eax
		push	0x18
		GAME_HEAP_ALLOC
		xorps	xmm0, xmm0
		movups	[eax], xmm0
		movq	qword ptr [eax+0x10], xmm0
		mov		dword ptr [eax+0xC], 0x3F800000
		push	eax
		push	2
		push	0xC4
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA746E0)
		push	0
		push	0
		push	eax
		push	eax
		push	eax
		push	0x80
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xB6FC90)
		pop		ecx
		push	eax
		CALL_EAX(0x439410)
		CALL_EAX(0xA5CEB0)
		mov		word ptr [eax+0x18], 0x10ED
		pop		ecx
		push	eax
		CALL_EAX(0x439410)
		CALL_EAX(0xB57E30)
		mov		edx, [esp+0x18]
		mov		ecx, [esp]
		call	NiObjectNET::SetName
		pop		eax
		add		esp, 8
		mov		edx, [esp+4]
		mov		[eax+0x64], edx
		retn	0xC
	}
}

void NiNode::Dump()
{
	PrintDebug("(N) %08X\t%s\t%s\t%08X\t#%d", this, GetType()->name, GetName(), m_flags, m_uiRefCount);
	if (m_controller)
		PrintDebug("\t(C) %08X\t%s\t#%d", m_controller, m_controller->GetType()->name, m_controller->m_uiRefCount);
	DumpExtraData();
	DumpProperties();
	s_debug().Indent();
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
	s_debug().Outdent();
}

