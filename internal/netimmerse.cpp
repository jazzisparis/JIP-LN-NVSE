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
		push	1
		push	0x64
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
		if (*iter && !StrCompareCI(iter->sequenceName, seqName))
			return *iter;
	return nullptr;
}

NiTransformController *NiTransformController::Create(NiObjectNET *pTarget, NiTransformInterpolator *pInterpolator)
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
		call	GetNiFixedString
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
	NiExtraData *xData;
	for (UInt32 iter = 0; iter < m_extraDataListLen; iter++)
		if (xData = m_extraDataList[iter])
			PrintDebug("(X) %08X\t%08X\t%s\t#%d", xData, *(UInt32*)xData, xData->GetType()->name, xData->m_uiRefCount);
}

__declspec(noinline) void __vectorcall NiMaterialProperty::SetTraitValue(UInt32 traitID, float value)
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

const NiUpdateData kNiUpdateData;

__declspec(naked) void NiAVObject::Update()
{
	__asm
	{
		push	ecx
		push	0
		push	offset kNiUpdateData
		mov		eax, [ecx]
		call	dword ptr [eax+0xA4]
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
		and		dword ptr [ecx+0x18], 0
		mov		[edx+0x18], eax
		call	NiReplaceObject
		mov		al, 1
		retn	4
		ALIGN 16
	done:
		xor		al, al
		retn	4
	}
}

__declspec(naked) void __fastcall NiGeometry::AddProperty(NiProperty *niProperty)
{
	__asm
	{
		lock inc dword ptr [edx+4]
		push	ecx
		push	edx
		CALL_EAX(0x43A010)
		pop		dword ptr [eax+8]
		pop		ecx
		inc		dword ptr [ecx+0x2C]
		mov		edx, [ecx+0x24]
		mov		[ecx+0x24], eax
		test	edx, edx
		jz		emptyList
		mov		[eax], edx
		mov		[edx+4], eax
		retn
	emptyList:
		mov		[ecx+0x28], eax
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
		cmp		dword ptr [eax], kVtbl_BSFadeNode
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

__declspec(naked) void NiAVObject::AssignGeometryProps()
{
	__asm
	{
		push	0
		push	0
		push	0
		push	ecx
		CALL_EAX(0xA5A040)
		CALL_EAX(0xB57BD0)
		add		esp, 0x10
		retn
	}
}

__declspec(naked) void NiAVObject::ExportToFile(const char *filePath)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x5C8
		push	esi
		mov		esi, ecx
		lea		ecx, [esp+4]
		CALL_EAX(0xA66150)
		push	esi
		mov		esi, eax
		mov		ecx, eax
		CALL_EAX(0xA66370)
		push	dword ptr [ebp+8]
		mov		ecx, esi
		CALL_EAX(0xA640B0)
		mov		ecx, esi
		CALL_EAX(0xA65300)
		pop		esi
		leave
		retn	4
	}
}

/*
Controller	1
ExtraData	2
Collision	4
Properties	8
Transform	0x10
*/
void NiAVObject::Dump(UInt8 dumpFlags)
{
	char blkType = IS_NODE(this) ? 'N' : 'B';
	PrintDebug("(%c) %08X\t%s\t%s\t%08X\t#%d", blkType, this, GetType()->name, GetName(), m_flags, m_uiRefCount);
	s_debug().Indent();
	if (m_controller && (dumpFlags & 1))
		PrintDebug("(C) %08X\t%s\t#%d", m_controller, m_controller->GetType()->name, m_controller->m_uiRefCount);
	if (dumpFlags & 2)
		DumpExtraData();
	if (m_collisionObject && (dumpFlags & 4))
	{
		PrintDebug("(H) %08X\t%s\t%08X", m_collisionObject, m_collisionObject->GetType()->name, m_collisionObject->flags);
		if (bhkWorldObject *hWorldObj = m_collisionObject->worldObj)
			PrintDebug("\t(H1) %08X\t%s\t%08X", hWorldObj, hWorldObj->GetType()->name, hWorldObj->bodyFlags);
	}
	if (dumpFlags & 8)
	{
		if (NiGeometry *geom = GetNiGeometry())
		{
			if (NiGeometryData *geomData = geom->geometryData)
			{
				PrintDebug("(G) %08X\t%s\t#%d", geomData, geomData->GetType()->name, geomData->m_uiRefCount);
				//DumpMemImg(geomData, 0x58);
			}
			if IS_TYPE(this, NiParticleSystem)
			{
				s_debug().Indent();
				for (auto modIter = ((NiParticleSystem*)this)->modifiers.Begin(); modIter; ++modIter)
					if (*modIter) PrintDebug("(PM) %08X\t%s\t#%d", *modIter, modIter->GetType()->name, modIter->m_uiRefCount);
				s_debug().Outdent();
			}
		}
		NiProperty *niProp;
		for (auto propIter = m_propertyList.Begin(); propIter; ++propIter)
		{
			if (!(niProp = *propIter))
				continue;
			PrintDebug("(P) %08X\t%s\t#%d", niProp, niProp->GetType()->name, niProp->m_uiRefCount);
			s_debug().Indent();
			if (niProp->m_controller && (dumpFlags & 1))
				PrintDebug("(C) %08X\t%s\t#%d", niProp->m_controller, niProp->m_controller->GetType()->name, niProp->m_controller->m_uiRefCount);
			if (dumpFlags & 2)
				niProp->DumpExtraData();
			s_debug().Outdent();
		}
	}
	if (dumpFlags & 0x10)
	{
		/*if (m_kWorldBound)
			PrintDebug("(WB) (%.4f, %.4f, %.4f) R = %.4f", m_kWorldBound->x, m_kWorldBound->y, m_kWorldBound->z, m_kWorldBound->radius);*/
		/*PrintDebug("(LT) (%.4f, %.4f, %.4f) S %.4f", m_transformLocal.translate.x, m_transformLocal.translate.y, m_transformLocal.translate.z, m_transformLocal.scale);
		PrintDebug("(WT) (%.4f, %.4f, %.4f) S %.4f", m_transformWorld.translate.x, m_transformWorld.translate.y, m_transformWorld.translate.z, m_transformWorld.scale);*/

		/*PrintDebug("(LS) %.6f", m_transformLocal.scale);
		PrintDebug("(WS) %.6f", m_transformWorld.scale);*/

		s_debug().Indent();
		m_transformLocal.Dump();
		m_transformWorld.Dump();
		s_debug().Outdent();
	}
	if (blkType == 'N')
		for (auto iter = ((NiNode*)this)->m_children.Begin(); iter; ++iter)
			if (*iter) iter->Dump(dumpFlags);
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
	thread_local static NiObjectCopyInfo s_NiObjectCopyInfo(0x97);
	return &s_NiObjectCopyInfo;
}

__declspec(naked) NiAVObject *NiAVObject::CreateCopy()
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
		call	NiTMap<UInt32, UInt32>::FreeBuckets
		mov		ecx, [edi+4]
		call	NiTMap<UInt32, UInt32>::FreeBuckets
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
		call	GetNiFixedString
		pop		ecx
		pop		ecx
		lock dec dword ptr [eax-8]
		jz		retnNULL
		cmp		[ecx+8], eax
		jz		found
		mov		edx, eax
		jmp		NiNode::GetBlockByName
	found:
		mov		eax, ecx
		retn
	retnNULL:
		xor		eax, eax
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

__declspec(naked) NiAVObject* __fastcall NiNode::FindBlockOfType(UInt32 typeVtbl) const
{
	__asm
	{
		mov		eax, ecx
		cmp		[eax], edx
		jz		done
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
		jnz		notNode
		call	NiNode::FindBlockOfType
		test	eax, eax
		jz		iterHead
		pop		edi
		pop		esi
		retn
		ALIGN 16
	notNode:
		cmp		eax, edx
		jnz		iterHead
		mov		eax, ecx
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

__declspec(naked) bool NiNode::IsMovable() const
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		immovable
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		immovable
		mov		eax, [eax+8]
		test	byte ptr [eax+0xE8], 2
		setnz	al
		jz		immovable
		retn
		ALIGN 16
	immovable:
		push	esi
		push	edi
		mov		esi, [ecx+0xA0]
		movzx	edi, word ptr [ecx+0xA6]
		ALIGN 16
	iterHead:
		dec		edi
		js		done
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		edx, [ecx]
		cmp		dword ptr [edx+0xC], ADDR_ReturnThis
		jnz		iterHead
		call	NiNode::IsMovable
		test	al, al
		jz		iterHead
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall NiNode::ToggleCollision(UInt8 flag)
{
	__asm
	{
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		noColObj
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		noColObj
		push	ecx
		mov		ecx, [eax+8]
		and     byte ptr [ecx+0x2D], 0xBF
		or      [ecx+0x2D], dl
		push	edx
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xC4]
		pop		edx
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
		call	NiNode::ToggleCollision
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
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

__declspec(naked) void __vectorcall NiNode::SetAlphaRecurse(float alpha)
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		mov		edx, [ecx+0x20]
		test	edx, edx
		jz		done
		cmp		dword ptr [edx+0xC], 0
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
		jnz		notNode
		call	NiNode::SetAlphaRecurse
		jmp		iterHead
		ALIGN 16
	notNode:
		cmp		dword ptr [eax+0x18], ADDR_ReturnThis2
		jnz		iterHead
		mov		eax, [ecx+0xA8]
		test	eax, eax
		jz		iterHead
		movss	[eax+0x2C], xmm0
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void NiNode::ResetShaderRenderPass()
{
	__asm
	{
		movzx	eax, word ptr [ecx+0xA6]
		test	eax, eax
		jz		done
		mov		edx, [ecx+0x20]
		test	edx, edx
		jz		done
		cmp		dword ptr [edx+0xC], 0
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
		jnz		notNode
		call	NiNode::ResetShaderRenderPass
		jmp		iterHead
		ALIGN 16
	notNode:
		cmp		dword ptr [eax+0x18], ADDR_ReturnThis2
		jnz		iterHead
		mov		eax, [ecx+0xA8]
		test	eax, eax
		jz		iterHead
		and		dword ptr [eax+0x38], 0
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) UInt32 NiNode::GetBSXFlags() const
{
	__asm
	{
		push	esi
		movzx	edx, word ptr [ecx+0x14]
		mov		ecx, [ecx+0x10]
		mov		esi, kVtbl_BSXFlags
		ALIGN 16
	iterHead:
		dec		edx
		js		done
		mov		eax, [ecx+edx*4]
		cmp		[eax], esi
		jnz		iterHead
		mov		eax, [eax+0xC]
		pop		esi
		retn
	done:
		xor		eax, eax
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall BSFadeNode::SetVisible(bool visible)
{
	__asm
	{
		movzx	eax, dl
		neg		eax
		and		eax, 0x3F800000
		movd	xmm0, eax
		unpcklps	xmm0, xmm0
		shl		dl, 6
		and		byte ptr [ecx+0x31], 0xBF
		or		[ecx+0x31], dl
		movlps	[ecx+0xB4], xmm0
		jmp		NiNode::SetAlphaRecurse
	}
}

__declspec(naked) void NiNode::AddPropertyRecurse(NiProperty *niProperty)
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
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		notNode
		push	dword ptr [esp+0xC]
		call	NiNode::AddPropertyRecurse
		jmp		iterHead
		ALIGN 16
	notNode:
		cmp		dword ptr [eax+0x18], ADDR_ReturnThis2
		jnz		iterHead
		mov		edx, [esp+0xC]
		call	NiGeometry::AddProperty
		jmp		iterHead
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn	4
	}
}

__declspec(naked) void NiNode::AddNoLightingPropertyRecurse()
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
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		notNode
		call	NiNode::AddNoLightingPropertyRecurse
		jmp		iterHead
		ALIGN 16
	notNode:
		cmp		dword ptr [eax+0x18], ADDR_ReturnThis2
		jnz		iterHead
		push	ecx
		push	0x80
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xB6FC90)
		mov		edx, eax
		pop		ecx
		call	NiGeometry::AddProperty
		jmp		iterHead
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void __vectorcall NiNode::SetMaterialPropValueRecurse(UInt32 traitID, float value)
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
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		notNode
		call	NiNode::SetMaterialPropValueRecurse
		jmp		iterHead
		ALIGN 16
	notNode:
		cmp		dword ptr [eax+0x18], ADDR_ReturnThis2
		jnz		iterHead
		mov		ecx, [ecx+0xA4]
		test	ecx, ecx
		jz		iterHead
		push	edx
		call	NiMaterialProperty::SetTraitValue
		pop		edx
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		edi
		pop		esi
	done:
		retn
	}
}

__declspec(naked) void __fastcall NiNode::SetCollisionLayer(UInt32 layerType)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		eax, [ecx+0x1C]
		test	eax, eax
		jz		noColObj
		mov		ecx, [eax+0x10]
		test	ecx, ecx
		jz		noColObj
		mov		eax, [ecx+8]
		cmp		[eax+0x2C], dl
		jz		noColObj
		mov		[eax+0x2C], dl
		push	edx
		mov		eax, [ecx]
		call	dword ptr [eax+0xC4]
		pop		edx
	noColObj:
		movzx	edi, word ptr [esi+0xA6]
		test	edi, edi
		jz		done
		mov		esi, [esi+0xA0]
		ALIGN 16
	iterHead:
		dec		edi
		js		done
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		iterHead
		call	NiNode::SetCollisionLayer
		jmp		iterHead
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall NiNode::SetCollisionPropRecurse(UInt32 propID, FltAndInt value)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		ecx, [ecx+0x1C]
		test	ecx, ecx
		jz		noColObj
		mov		eax, [ecx+0x10]
		test	eax, eax
		jz		noColObj
		mov		ecx, [eax+8]
		cmp		byte ptr [ecx+0x28], 1
		jnz		noColObj
		push	edx
		push	dword ptr [esp+0x10]
		call	hkpRigidBody::SetCollisionProperty
		pop		edx
	noColObj:
		movzx	edi, word ptr [esi+0xA6]
		test	edi, edi
		jz		done
		mov		esi, [esi+0xA0]
		ALIGN 16
	iterHead:
		dec		edi
		js		done
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		iterHead
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xC], ADDR_ReturnThis
		jnz		iterHead
		push	dword ptr [esp+0xC]
		call	NiNode::SetCollisionPropRecurse
		jmp		iterHead
	done:
		pop		edi
		pop		esi
		retn	4
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
		mov		edx, [esp+8]
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

__declspec(naked) bool __fastcall NiCamera::WorldToScreen(const NiVector3 &worldPos, NiPoint2 &scrPos)
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
		movss	xmm1, SS_1d1K
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
		pshufd	xmm2, xmm1, 0
		shufps	xmm3, xmm3, 0x50
		xorps	xmm0, xmm3
		xorps	xmm2, xmm3
		cmpltps	xmm1, xmm0
		movmskps	edx, xmm1
		test	dl, dl
		jz		done
		andps	xmm1, xmm2
		shufps	xmm1, xmm1, 0xD8
		haddps	xmm1, xmm4
		subps	xmm0, xmm1
		xor		al, al
	done:
		mov		edx, [esp+4]
		movlps	[edx], xmm0
		retn	4
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
		push	0
		push	esp
		push	ecx
		CALL_EAX(0xA61B90)
		pop		edx
		pop		eax
		mov		eax, [esp]
		test	eax, eax
		jnz		done
		mov		ecx, esp
		push	edx
		CALL_EAX(0x438170)
		push	0
		push	1
		push	0x11A9598
		push	eax
		CALL_EAX(0xA5FD70)
		add		esp, 0x10
		pop		ecx
		lock dec dword ptr [ecx-8]
		push	0
		push	eax
		CALL_EAX(0xA61C50)
		pop		eax
	done:
		pop		ecx
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
		call	Game_DoHeapAlloc
		mov		word ptr [eax], 1
		push	eax
		push	0
		push	0
		push	0
		push	0x20
		call	Game_DoHeapAlloc
		mov		ecx, [esp+0x18]
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		movups	[eax+0x10], xmm0
		push	eax
		push	0x18
		call	Game_DoHeapAlloc
		xorps	xmm0, xmm0
		movups	[eax], xmm0
		movss	xmm0, PS_V3_One
		movlps	[eax+0x10], xmm0
		push	eax
		push	2
		push	0xC4
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA746E0)
		push	ebx
		mov		ebx, eax
		mov		edx, s_alphaProperty
		mov		ecx, eax
		call	NiGeometry::AddProperty
		push	0x80
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xB6FC90)
		mov		edx, eax
		mov		ecx, ebx
		call	NiGeometry::AddProperty
		mov		ecx, ebx
		call	NiAVObject::AssignGeometryProps
		mov		edx, [esp+0x10]
		mov		ecx, ebx
		call	NiObjectNET::SetName
		mov		eax, [esp+8]
		mov		[ebx+0x64], eax
		mov		eax, ebx
		pop		ebx
		retn	0xC
	}
}

__declspec(naked) void NiGeometryData::FlipNormals()
{
	__asm
	{
		mov		eax, [ecx+0x24]
		test	eax, eax
		jz		done
		movzx	edx, word ptr [ecx+8]
		test	edx, edx
		jz		done
		lea		ecx, [edx+edx*2]
		shl		ecx, 2
		mov		edx, 0x80000000
		movaps	xmm1, PS_FlipSignMask
	Iter4:
		test	cl, 0xF
		jz		iter10
		xor		[eax+ecx-4], edx
		sub		ecx, 4
		jnz		Iter4
		retn
		ALIGN 16
	iter10:
		sub		ecx, 0x10
		movups	xmm0, [eax+ecx]
		xorps	xmm0, xmm1
		movups	[eax+ecx], xmm0
		jnz		Iter10
	done:
		retn
	}
}

__declspec(naked) NiTriStripsData* __vectorcall NiTriStripsData::DrawConvex(float radius, UInt32 numEdges, const NiColorAlpha &color)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		movq	xmm7, xmm0
		movups	xmm6, [edx]
		lea		eax, [ecx+ecx+2]
		push	eax
		call	Game_DoHeapAlloc
		push	eax
		lea		ecx, [eax+ebx*2]
		mov		[ecx], bx
		push	ecx
		xor		ecx, ecx
		ALIGN 16
	pntIter:
		mov		[eax+ecx*2], cx
		inc		ecx
		cmp		ecx, ebx
		jb		pntIter
		push	1
		lea		eax, [ebx-2]
		push	eax
		push	0
		push	0
		push	0
		mov		eax, ebx
		shl		eax, 4
		push	eax
		call	Game_DoHeapAlloc
		push	eax
		mov		edx, ebx
		ALIGN 16
	colIter:
		movups	[eax], xmm6
		add		eax, 0x10
		dec		edx
		jnz		colIter
		push	0
		cvtsi2ss	xmm1, ebx
		movss	xmm0, PS_V3_PIx2
		divss	xmm0, xmm1
		call	GetSinCos
		movss	xmm5, PS_FlipSignMask0
		movq	xmm1, xmm0
		xorps	xmm0, xmm5
		shufps	xmm0, xmm1, 0x41
		lea		eax, [ebx+ebx*2]
		shl		eax, 2
		push	eax
		push	eax
		call	Game_DoHeapAlloc
		pop		ecx
		push	eax
		push	ebx
		pshufd	xmm1, xmm7, 0x51
		movups	[eax], xmm1
		xor		edx, edx
		test	bl, 1
		jnz		skipLast
		lea		ecx, [eax+ecx-0xC]
		movlps	[ecx], xmm1
		mov		[ecx+8], edx
		xor		byte ptr [ecx+7], 0x80
	skipLast:
		add		eax, 0xC
		dec		ebx
		shr		ebx, 1
		ALIGN 16
	vtxIter:
		pshufd	xmm2, xmm1, 0x44
		mulps	xmm2, xmm0
		haddps	xmm2, xmm2
		movq	xmm1, xmm2
		xorps	xmm2, xmm5
		movlps	[eax], xmm2
		mov		[eax+8], edx
		movlps	[eax+0xC], xmm1
		mov		[eax+0x14], edx
		add		eax, 0x18
		dec		ebx
		jnz		vtxIter
		push	0x50
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA75DC0)
		mov		ebx, eax
		mov		ecx, eax
		CALL_EAX(0xA8AB40)
		mov		eax, ebx
		pop		ebx
		retn
	}
}

__declspec(naked) NiTriStripsData* __vectorcall NiTriStripsData::DrawCylinder(float radius, float height, UInt32 numEdges, const NiColorAlpha &color)
{
	__asm
	{
		push	ebx
		push	esi
		lea		ebx, [ecx+ecx+2]
		movups	xmm5, [edx]
		pshufd	xmm6, xmm0, 0x51
		movq	xmm7, xmm1
		lea		eax, [ebx+ebx+4]
		push	eax
		call	Game_DoHeapAlloc
		push	eax
		mov		esi, eax
		lea		ecx, [eax+ebx*2]
		push	ecx
		mov		[ecx], bx
		mov		dword ptr [ecx-4], 0x10000
		sub		ebx, 2
		push	1
		push	ebx
		push	0
		push	0
		push	0
		mov		eax, ebx
		shl		eax, 4
		push	eax
		push	eax
		call	Game_DoHeapAlloc
		pop		edx
		push	eax
		push	0
		xor		ecx, ecx
		ALIGN 16
	pntIter:
		mov		[esi+ecx*2], cx
		inc		ecx
		sub		edx, 0x10
		movups	[eax+edx], xmm5
		jnz		pntIter
		mov		eax, ebx
		shr		eax, 1
		cvtsi2ss	xmm1, eax
		movss	xmm0, PS_V3_PIx2
		divss	xmm0, xmm1
		call	GetSinCos
		movss	xmm5, PS_FlipSignMask0
		movq	xmm1, xmm0
		xorps	xmm0, xmm5
		shufps	xmm0, xmm1, 0x41
		lea		esi, [ebx+ebx*2]
		shl		esi, 2
		push	esi
		call	Game_DoHeapAlloc
		push	eax
		push	ebx
		movq	xmm1, xmm6
		movups	[eax], xmm1
		movlps	[eax+0xC], xmm1
		movss	[eax+0x14], xmm7
		lea		ecx, [eax+esi-0x18]
		xor		edx, edx
		test	bl, 2
		jnz		skipLast
		shr		esi, 1
		add		esi, eax
		movlps	[esi], xmm1
		mov		[esi+8], edx
		xor		byte ptr [esi+7], 0x80
		movlps	[esi+0xC], xmm1
		movss	[esi+0x14], xmm7
		xor		byte ptr [esi+0x13], 0x80
	skipLast:
		add		eax, 0x18
		dec		ebx
		shr		ebx, 2
		ALIGN 16
	vtxIter:
		pshufd	xmm2, xmm1, 0x44
		mulps	xmm2, xmm0
		haddps	xmm2, xmm2
		movq	xmm1, xmm2
		xorps	xmm2, xmm5
		movlps	[eax], xmm2
		mov		[eax+8], edx
		movlps	[eax+0xC], xmm2
		movss	[eax+0x14], xmm7
		movlps	[ecx], xmm1
		mov		[ecx+8], edx
		movlps	[ecx+0xC], xmm1
		movss	[ecx+0x14], xmm7
		add		eax, 0x18
		sub		ecx, 0x18
		dec		ebx
		jnz		vtxIter
		push	0x50
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA75DC0)
		mov		ebx, eax
		mov		ecx, eax
		CALL_EAX(0xA8AB40)
		mov		eax, ebx
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) NiTriStripsData* __vectorcall NiTriStripsData::DrawPrism(float radius, float height, UInt32 numEdges, const NiColorAlpha &color)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		push	esi
		lea		esi, [ecx*4]
		push	edi
		movups	xmm5, [edx]
		pshufd	xmm6, xmm0, 0x51
		movq	xmm7, xmm1
		lea		eax, [ecx*8+0xC]
		push	eax
		call	Game_DoHeapAlloc
		push	eax
		mov		esi, eax
		lea		ecx, [eax+ebx*8+4]
		push	ecx
		lea		edx, [ebx+ebx+2]
		mov		[ecx], bx
		mov		[ecx+2], bx
		mov		[ecx+4], dx
		push	3
		lea		edi, [ebx*4]
		lea		ecx, [edi-4]
		push	ecx
		push	0
		push	0
		push	0
		shl		edi, 4
		push	edi
		call	Game_DoHeapAlloc
		push	eax
		push	0
		xor		ecx, ecx
		ALIGN 16
	pntIter:
		mov		[esi+ecx*2], cx
		inc		ecx
		sub		edi, 0x10
		movups	[eax+edi], xmm5
		jnz		pntIter
		mov		edx, [esi+ebx*4]
		mov		[esi+ecx*2], edx
		cvtsi2ss	xmm1, ebx
		movss	xmm0, PS_V3_PIx2
		divss	xmm0, xmm1
		call	GetSinCos
		movss	xmm5, PS_FlipSignMask0
		movq	xmm1, xmm0
		xorps	xmm0, xmm5
		shufps	xmm0, xmm1, 0x41
		lea		edi, [ebx+ebx*2]
		shl		edi, 4
		push	edi
		call	Game_DoHeapAlloc
		push	eax
		lea		ecx, [ebx*4]
		push	ecx
		shr		edi, 2
		lea		edx, [eax+edi]
		lea		esi, [edx+edi]
		movq	xmm1, xmm6
		movq	xmm2, xmm1
		unpcklpd	xmm2, xmm7
		movups	[eax], xmm1
		movups	[edx], xmm2
		movups	[esi], xmm1
		movups	[esi+0xC], xmm2
		xor		ecx, ecx
		test	bl, 1
		jnz		skipLast
		pshufd	xmm3, xmm5, 0x51
		xorps	xmm2, xmm3
		movlps	[edx-0xC], xmm2
		mov		[edx-4], ecx
		movlps	[esi-0xC], xmm2
		movss	[esi-4], xmm7
		movlps	[esi+edi], xmm2
		mov		[esi+edi+8], ecx
		movlps	[esi+edi+0xC], xmm2
		movss	[esi+edi+0x14], xmm7
	skipLast:
		add		eax, 0xC
		add		edx, 0xC
		lea		edi, [esi+edi*2-0x18]
		add		esi, 0x18
		dec		ebx
		shr		ebx, 1
		xorps	xmm3, xmm3
		ALIGN 16
	vtxIter:
		pshufd	xmm2, xmm1, 0x44
		mulps	xmm2, xmm0
		haddps	xmm2, xmm3
		movq	xmm1, xmm2
		xorps	xmm2, xmm5
		movlps	[eax], xmm2
		mov		[eax+8], ecx
		movlps	[edx+0xC], xmm2
		movss	[edx+0x14], xmm7
		movlps	[esi], xmm2
		mov		[esi+8], ecx
		movlps	[esi+0xC], xmm2
		movss	[esi+0x14], xmm7
		movlps	[eax+0xC], xmm1
		mov		[eax+0x14], ecx
		movlps	[edx], xmm1
		movss	[edx+8], xmm7
		movlps	[edi], xmm1
		mov		[edi+8], ecx
		movlps	[edi+0xC], xmm1
		movss	[edi+0x14], xmm7
		add		eax, 0x18
		add		edx, 0x18
		add		esi, 0x18
		sub		edi, 0x18
		dec		ebx
		jnz		vtxIter
		push	0x50
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA75DC0)
		mov		ebx, eax
		mov		ecx, eax
		CALL_EAX(0xA8AB40)
		mov		eax, ebx
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) NiPSysModifier* __fastcall NiParticleSystem::FindModifier(UInt32 typeVtbl) const
{
	__asm
	{
		mov		ecx, [ecx+0xC8]
	iterHead:
		test	ecx, ecx
		jz		done
		mov		eax, [ecx+8]
		mov		ecx, [ecx]
		test	eax, eax
		jz		iterHead
		cmp		[eax], edx
		jnz		iterHead
		retn
	done:
		xor		eax, eax
		retn
	}
}

__declspec(noinline) BSCullingProcess *GetGRTCullingProcess()
{
	thread_local static BSCullingProcess *cullingProcess = nullptr;
	if (!cullingProcess)
	{
		auto cullingProc = ThisCall<BSCullingProcess*>(0x4A0EB0, Ni_Alloc(sizeof(BSCullingProcess) + sizeof(BSShaderAccumulator)), 0);
		auto shaderAccum = ThisCall<BSShaderAccumulator*>(0xB660D0, (UInt8*)cullingProc + sizeof(BSCullingProcess), 0x63, 1, 0x2F7);
		shaderAccum->m_uiRefCount = 2;
		shaderAccum->shadowScene = g_shadowSceneNode;
		cullingProc->shaderAccum = shaderAccum;
		cullingProc->topCullMode = kCull_IgnoreMultiBounds;
		cullingProcess = cullingProc;
	}
	return cullingProcess;
}

__declspec(naked) void __stdcall BSTextureManager::GenerateRenderedTexture(NiCamera *camera, const TextureParams &texParams, NiTexture **pTexture)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		xor		eax, eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		mov		ecx, [ebp+0xC]
		push	dword ptr [ecx+8]
		push	dword ptr [ecx+4]
		push	dword ptr [ecx]
		push	eax
		mov		ecx, BSTEXTUREMANAGER
		CALL_EAX(0xB6D170)
		lock inc dword ptr [eax+4]
		push	eax
		call	GetGRTCullingProcess
		push	eax
		lea		ecx, [eax+0xC8]
		push	ecx
		mov		edx, [ecx+0x194]
		push	edx
		mov		[edx+0x130], 1
		mov		eax, [ebp+0xC]
		mov		edx, [eax+0xC]
		mov		[ecx+0x19C], edx
		mov		edx, [eax+0x10]
		mov		eax, NIDX9RENDERER
		add		eax, 0x5E0
		push	dword ptr [eax]
		push	eax
		and		[eax], edx
		setnz	[ecx+0x16C]
		mov		eax, [ebp-4]
		push	dword ptr [eax+8]
		push	kClrFlag_All
		CALL_EAX(0xB6B8D0)
		push	dword ptr [ebp-8]
		push	dword ptr [ebp-0x10]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		mov		eax, [ebp-0xC]
		mov		[esp+4], eax
		CALL_EAX(0xB6C0D0)
		add		esp, 0x14
		CALL_EAX(0xB6B790)
		mov		ecx, [ebp+0xC]
		cmp		dword ptr [ecx+0x14], IS_SHADER_NOISE_NORMAL_MAP
		ja		noISEff
		xor		eax, eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	dword ptr [ecx+8]
		push	dword ptr [ecx+4]
		push	dword ptr [ecx]
		push	eax
		mov		ecx, BSTEXTUREMANAGER
		CALL_EAX(0xB6D170)
		lock inc dword ptr [eax+4]
		push	eax
		push	dword ptr [eax+8]
		push	kClrFlag_All
		CALL_EAX(0xB6B8D0)
		add		esp, 8
		push	1
		push	0
		push	dword ptr [ebp-0x1C]
		push	dword ptr [ebp-4]
		push	dword ptr NIDX9RENDERER
		mov		ecx, IMAGESPACEMANAGER
		mov		eax, [ebp+0xC]
		mov		edx, [eax+0x14]
		mov		eax, [ecx+8]
		push	dword ptr [eax+edx*4]
		CALL_EAX(0xB8C830)
		mov		ecx, [ebp-4]
		call	NiReleaseObject
		pop		dword ptr [ebp-4]
	noISEff:
		mov		eax, [ebp-0x10]
		mov		[eax+0x130], 0
		pop		eax
		pop		dword ptr [eax]
		mov		eax, [ebp-4]
		push	dword ptr [eax+0x30]
		push	dword ptr [ebp+0x10]
		call	NiReplaceObject
		mov		ecx, [ebp-4]
		call	NiReleaseObject
		leave
		retn	0xC
	}
}

extern float g_screenHeight;
__declspec(naked) void __stdcall BSTextureManager::GenerateRenderedUITexture(NiNode *tileNode, const NiVector4 &scrArea, NiTexture **pTexture)
{
	static BSCullingProcess *cullingProcessUI = nullptr;
	__asm
	{
		push	ebp
		mov		ebp, esp
		xor		eax, eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	D3DFMT_A8R8G8B8
		mov		ecx, [ebp+0xC]
		movq	xmm0, qword ptr [ecx+8]
		cvtps2dq	xmm0, xmm0
		sub		esp, 8
		movlps	[esp], xmm0
		push	eax
		mov		ecx, BSTEXTUREMANAGER
		CALL_EAX(0xB6D170)
		lock inc dword ptr [eax+4]
		push	dword ptr NIDX9RENDERER
		push	eax
		mov		eax, cullingProcessUI
		test	eax, eax
		jnz		hasCulling
		push	0x348
		CALL_EAX(0xAA13E0)
		pop		ecx
		push	0x2F7
		push	1
		push	0x64
		mov		ecx, eax
		CALL_EAX(0xB660D0)
		mov		dword ptr [eax+4], 2
		mov		dword ptr [eax+0x19C], kRndrMode_Tiles
		push	eax
		push	0
		lea		ecx, [eax+0x280]
		CALL_EAX(0x4A0EB0)
		pop		dword ptr [eax+0xC4]
		mov		dword ptr [eax+0x90], kCull_AllPass
		mov		cullingProcessUI, eax
	hasCulling:
		push	eax
		push	dword ptr [eax+0xC4]
		mov		ecx, g_interfaceManager
		mov		ecx, [ecx+4]
		push	dword ptr [ecx+0xAC]
		mov		ecx, [ebp-4]
		push	dword ptr [ecx+0x5E0]
		and		dword ptr [ecx+0x5E0], 0
		cmp		dword ptr [ecx+0x200], 0
		jnz		scnActive
		mov		ecx, [ebp-8]
		push	dword ptr [ecx+8]
		push	kClrFlag_BackBuffer
		CALL_EAX(0xB6B8D0)
		add		esp, 8
	scnActive:
		sub		esp, 0x14
		mov		ecx, [ebp-0x14]
		movups	xmm0, [ecx+0xDC]
		movups	[ebp-0x28], xmm0
		mov		eax, [ebp+0xC]
		movq	xmm0, qword ptr [eax]
		movq	xmm1, qword ptr [eax+8]
		pshufd	xmm2, PS_FlipSignMask0, 0x51
		xorps	xmm0, xmm2
		xorps	xmm1, xmm2
		movss	xmm2, g_screenHeight
		addss	xmm2, xmm2
		pslldq	xmm2, 4
		addps	xmm0, xmm2
		addps	xmm1, xmm0
		unpcklps	xmm0, xmm1
		movups	[ecx+0xDC], xmm0
		mov		eax, [ebp+8]
		test	byte ptr [eax+0x30], 1
		setnz	[ebp-0x29]
		and		byte ptr [eax+0x30], 0xFE
		push	dword ptr [ebp-0xC]
		push	eax
		push	ecx
		CALL_EAX(0xB6BEE0)
		mov		eax, [ebp-0x10]
		mov		[esp+4], eax
		CALL_EAX(0xB6C0D0)
		add		esp, 0xC
		CALL_EAX(0xB6B790)
		mov		eax, [ebp+8]
		mov		dl, [ebp-0x29]
		or		[eax+0x30], dl
		mov		eax, [ebp-4]
		mov		edx, [ebp-0x18]
		mov		[eax+0x5E0], edx
		mov		ecx, [ebp-0x14]
		movups	xmm0, [ebp-0x28]
		movups	[ecx+0xDC], xmm0
		mov		ecx, [ebp-8]
		push	dword ptr [ecx+0x30]
		push	dword ptr [ebp+0x10]
		call	NiReplaceObject
		mov		ecx, [ebp-8]
		call	NiReleaseObject
		leave
		retn	0xC
	}
}

NiAlphaProperty *s_alphaProperty;