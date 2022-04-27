#include "nvse/GameObjects.h"
#include "nvse/GameExtraData.h"
#include "nvse/GameTasks.h"

__declspec(naked) float __vectorcall GetDistance3D(TESObjectREFR *ref1, TESObjectREFR *ref2)
{
	__asm
	{
		movups	xmm0, [ecx+0x2C]
		psrldq	xmm0, 4
		movups	xmm1, [edx+0x2C]
		psrldq	xmm1, 4
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) float __vectorcall GetDistance2D(TESObjectREFR *ref1, TESObjectREFR *ref2)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx+0x30]
		movq	xmm1, qword ptr [edx+0x30]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}

ScriptEventList *TESObjectREFR::GetEventList() const
{
	ExtraScript *xScript = GetExtraType(&extraDataList, Script);
	return xScript ? xScript->eventList : NULL;
}

__declspec(naked) TESContainer *TESObjectREFR::GetContainer()
{
	__asm
	{
		mov		eax, [ecx+0x20]
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESNPC
		jz		isActor
		cmp		dl, kFormType_TESCreature
		jz		isActor
		cmp		dl, kFormType_TESObjectCONT
		jnz		retnNULL
		add		eax, 0x30
		retn
	isActor:
		add		eax, 0x64
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

void __fastcall HidePointLights(NiNode *objNode);
extern ModelLoader *g_modelLoader;

__declspec(naked) void TESObjectREFR::Update3D()
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		cmp		dword ptr [ecx+0xC], 0x14
		jz		isPlayer
		push	ecx
		test	byte ptr [eax+0x33], 0x20
		jz		noLights
		mov		ecx, eax
		call	HidePointLights
		mov		ecx, [esp]
	noLights:
		push	1
		push	0
		CALL_EAX(0x5702E0)
		pop		ecx
		jmp		doQueue
	isPlayer:
		test	byte ptr [ecx+0x5F], 1
		jnz		done
	doQueue:
		or		byte ptr [ecx+0x5F], 1
		push	0
		push	1
		push	ecx
		mov		ecx, g_modelLoader
		CALL_EAX(0x444850)
	done:
		retn
	}
}

TESObjectREFR *TESObjectREFR::Create(bool bTemp)
{
	TESObjectREFR *refr = (TESObjectREFR*)GameHeapAlloc(sizeof(TESObjectREFR));
	ThisCall(0x55A2F0, refr);
	if (bTemp) ThisCall(0x484490, refr);
	return refr;
}

__declspec(naked) bool TESObjectREFR::GetDisabled()
{
	__asm
	{
		push	ecx
		test	byte ptr [ecx+9], 8
		jz		notPerm
		CALL_EAX(0x5AA680)
		xor		al, 1
		jnz		done
	notPerm:
		CALL_EAX(0x5AA630)
	done:
		pop		ecx
		retn
	}
}

__declspec(naked) ExtraContainerChanges::EntryDataList *TESObjectREFR::GetContainerChangesList()
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
		mov		eax, [eax]
	done:
		retn
	}
}

__declspec(naked) ContChangesEntry *TESObjectREFR::GetContainerChangesEntry(TESForm *itemForm)
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		mov		edx, [esp+4]
		ALIGN 16
	itemIter:
		test	ecx, ecx
		jz		retnNULL
		mov		eax, [ecx]
		mov		ecx, [ecx+4]
		test	eax, eax
		jz		itemIter
		cmp		[eax+8], edx
		jnz		itemIter
		retn	4
	retnNULL:
		xor		eax, eax
	done:
		retn	4
	}
}

__declspec(naked) SInt32 __fastcall GetFormCount(TESContainer::FormCountList *formCountList, ExtraContainerChanges::EntryDataList *objList, TESForm *form)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, [esp+0xC]
		xor		edi, edi
		ALIGN 16
	contIter:
		test	ecx, ecx
		jz		xtraIter
		mov		eax, [ecx]
		mov		ecx, [ecx+4]
		test	eax, eax
		jz		contIter
		cmp		[eax+4], esi
		jnz		contIter
		add		edi, [eax]
		jmp		contIter
		ALIGN 16
	xtraIter:
		test	edx, edx
		jz		done
		mov		ecx, [edx]
		mov		edx, [edx+4]
		test	ecx, ecx
		jz		xtraIter
		cmp		[ecx+8], esi
		jnz		xtraIter
		mov		esi, ecx
		test	edi, edi
		jz		noCont
		call	ContChangesEntry::HasExtraLeveledItem
		test	al, al
		jnz		noCont
		add		edi, [esi+4]
		js		retnZero
		jmp		done
		ALIGN 16
	noCont:
		mov		edi, [esi+4]
		test	edi, edi
		jge		done
	retnZero:
		xor		edi, edi
	done:
		mov		eax, edi
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) SInt32 TESObjectREFR::GetItemCount(TESForm *form)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		call	TESObjectREFR::GetContainer
		test	eax, eax
		jz		done
		add		eax, 4
		push	eax
		call	TESObjectREFR::GetContainerChangesList
		mov		edx, eax
		mov		eax, [ebp+8]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+4], kFormType_BGSListForm
		jz		itemList
		push	eax
		mov		ecx, [ebp-4]
		call	GetFormCount
	done:
		leave
		retn	4
		NOP_0x7
	itemList:
		push	edx
		push	esi
		push	edi
		lea		esi, [eax+0x18]
		xor		edi, edi
	iterHead:
		test	esi, esi
		jz		iterEnd
		mov		eax, [esi]
		mov		esi, [esi+4]
		test	eax, eax
		jz		iterHead
		push	eax
		mov		edx, [ebp-8]
		mov		ecx, [ebp-4]
		call	GetFormCount
		add		edi, eax
		jmp		iterHead
		ALIGN 16
	iterEnd:
		mov		eax, edi
		pop		edi
		pop		esi
		leave
		retn	4
	}
}

__declspec(naked) void TESObjectREFR::AddItemAlt(TESForm *form, UInt32 count, float condition, bool doEquip, UInt32 showMessage)
{
	__asm
	{
		push	ebp
		mov		ebp, esp

		push	ecx
		sub		esp, 0x10
		push	esi
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x481610)
		mov		ecx, [ebp+8]
		movzx	eax, byte ptr [ecx+4]
		cmp		al, kFormType_TESLevItem
		jz		lvlItem
		cmp		al, kFormType_BGSListForm
		jz		itemList
		cmp		kInventoryType[eax], 0
		jz		done
		push	0
		push	dword ptr [ebp+0xC]
		push	ecx
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x4818E0)
		jmp		doMove
	lvlItem:
		push	0
		mov		ecx, [ebp-4]
		CALL_EAX(0x567E10)
		push	0
		lea		ecx, [ebp-0x10]
		push	ecx
		push	dword ptr [ebp+0xC]
		push	eax
		mov		ecx, [ebp+8]
		add		ecx, 0x30
		CALL_EAX(0x487F70)
		jmp		doMove
	itemList:
		mov		esi, [ebp+8]
		add		esi, 0x18
		ALIGN 16
	listIter:
		test	esi, esi
		jz		doMove
		mov		ecx, [esi]
		mov		esi, [esi+4]
		test	ecx, ecx
		jz		listIter
		movzx	eax, byte ptr [ecx+4]
		cmp		kInventoryType[eax], 0
		jz		listIter
		push	0
		push	dword ptr [ebp+0xC]
		push	ecx
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x4818E0)
		jmp		listIter
		ALIGN 16
	doMove:
		push	dword ptr [ebp+0x10]
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x482090)
		push	1
		push	dword ptr [ebp-4]
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x4821A0)
		cmp		byte ptr [ebp+0x14], 0
		jz		done
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		done
		call	TESObjectREFR::GetContainerChangesList
		test	eax, eax
		jz		done
		mov		[ebp-0x14], eax
		lea		esi, [ebp-0xC]
		ALIGN 16
	eqpIter:
		test	esi, esi
		jz		done
		mov		eax, [esi]
		mov		esi, [esi+4]
		test	eax, eax
		jz		eqpIter
		mov		edx, [eax+4]
		push	edx
		mov		ecx, [ebp-0x14]
		call	ExtraContainerChanges::EntryDataList::FindForItem
		pop		edx

		push	[ebp+18] // showMessage
		push	0 //
		push	eax //
		push	edx //
		mov		ecx, [ebp-4]
		call	Actor::EquipItemAlt

		jmp		eqpIter
		ALIGN 16
	done:
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x481680)
		pop		esi
		leave
		retn	0x10
	}
}

void TESObjectREFR::RemoveItemTarget(TESForm *itemForm, TESObjectREFR *target, SInt32 quantity, bool keepOwner)
{
	if IS_ID(itemForm, BGSListForm)
	{
		ListNode<TESForm> *lstIter = ((BGSListForm*)itemForm)->list.Head();
		do
		{
			if (lstIter->data)
				RemoveItemTarget(lstIter->data, target, quantity, keepOwner);
		}
		while (lstIter = lstIter->next);
	}
	else if (kInventoryType[itemForm->typeID])
	{
		SInt32 total = GetItemCount(itemForm);
		if (total < 1) return;
		if ((quantity > 0) && (quantity < total))
			total = quantity;
		ContChangesEntry *entry = GetContainerChangesEntry(itemForm);
		if (entry && entry->extendData)
		{
			ExtraDataList *xData;
			SInt32 subCount;
			while ((total > 0) && (xData = entry->extendData->GetFirstItem()))
			{
				subCount = xData->GetCount();
				if (subCount < 1)
					subCount = 1;
				else if (subCount > total)
					subCount = total;
				RemoveItem(itemForm, xData, subCount, keepOwner, 0, target, 0, 0, 1, 0);
				total -= subCount;
			}
		}
		if (total > 0)
			RemoveItem(itemForm, NULL, total, keepOwner, 0, target, 0, 0, 1, 0);
	}
}

__declspec(naked) void Actor::EquipItemAlt(TESForm *itemForm, ContChangesEntry *entry, UInt32 noUnequip, UInt32 noMessage)
{
	__asm
	{
		push	ebp
		mov		ebp, esp

		push	ecx
		mov		ecx, 1
		mov		eax, [ebp+8]
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESObjectARMO
		jz		doneType
		cmp		dl, kFormType_TESObjectBOOK
		jz		doneType
		cmp		dl, kFormType_AlchemyItem
		jz		doneType
		cmp		dl, kFormType_TESAmmo
		jz		countMax
		cmp		dl, kFormType_TESObjectWEAP
		jnz		done
		cmp		byte ptr [eax+0xF4], 0xA
		jb		doneType
	countMax:
		xor		ecx, ecx
	doneType:
		push	dword ptr [ebp+0x14] // noMessage
		push	dword ptr [ebp+0x10] // noUnequip
		push	1
		mov		eax, [ebp+0xC] // entry
		test	eax, eax
		jz		noEntry
		mov		edx, [eax]
		test	edx, edx
		jz		noExtra
		mov		edx, [edx]
	noExtra:
		push	edx
		test	ecx, ecx
		cmovz	ecx, [eax+4]
		push	ecx
		jmp		doEquip
	noEntry:
		push	0
		push	1
	doEquip:
		push	dword ptr [ebp+8] // item
		mov		ecx, [ebp-4]	// actor (this)
		CALL_EAX(ADDR_EquipItem)

	done:
		leave
		retn	0x10
	}
}

bool TESObjectREFR::ValidForHooks()
{
	if IS_ACTOR(this) return IsPersistent();
	return !kInventoryType[baseForm->typeID] && !IsProjectile();
}

__declspec(naked) TESObjectCELL *TESObjectREFR::GetParentCell()
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		done
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) TESWorldSpace *TESObjectREFR::GetParentWorld()
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		getWorld
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	getWorld:
		mov		eax, [eax+0xC0]
	done:
		retn
	}
}

__declspec(naked) bool __fastcall TESObjectREFR::GetInSameCellOrWorld(TESObjectREFR *target)
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		hasCell1
		push	edx
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		pop		edx
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	hasCell1:
		mov		ecx, [edx+0x40]
		test	ecx, ecx
		jnz		hasCell2
		push	eax
		push	kExtraData_PersistentCell
		lea		ecx, [edx+0x44]
		call	BaseExtraList::GetByType
		pop		edx
		test	eax, eax
		jz		done
		mov		ecx, [eax+0xC]
		mov		eax, edx
	hasCell2:
		cmp		eax, ecx
		jz		retnTrue
		mov		eax, [eax+0xC0]
		test	eax, eax
		jz		done
		cmp		eax, [ecx+0xC0]
	retnTrue:
		setz	al
	done:
		retn
	}
}

__declspec(naked) float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR *target)
{
	__asm
	{
		push	ecx
		push	edx
		call	TESObjectREFR::GetInSameCellOrWorld
		pop		edx
		pop		ecx
		test	al, al
		jz		fltMax
		jmp		GetDistance3D
	fltMax:
		mov		eax, 0x7F7FFFFF
		movd	xmm0, eax
		retn
	}
}

__declspec(naked) void TESObjectREFR::SetPos(NiVector4 *posVector)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		push	dword ptr [esp+8]
		CALL_EAX(0x575830)
		mov		eax, [esi]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		noCharCtrl
		mov		ecx, [esi+0x68]
		test	ecx, ecx
		jz		noCharCtrl
		cmp		dword ptr [ecx+0x28], 1
		ja		noCharCtrl
		mov		ecx, [ecx+0x138]
		test	ecx, ecx
		jz		noCharCtrl
		cmp		dword ptr [ecx+0x3F0], 4
		jz		noCharCtrl
		push	dword ptr [esp+8]
		CALL_EAX(0x5620E0)
	noCharCtrl:
		mov		ecx, esi
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		mov		ecx, [esp+8]
		movq	xmm0, qword ptr [ecx]
		movq	qword ptr [eax+0x58], xmm0
		mov		edx, [ecx+8]
		mov		[eax+0x60], edx
		push	1
		push	eax
		CALL_EAX(0xC6BD00)
		pop		ecx
		pop		edx
		call	NiAVObject::Update
	done:
		pop		esi
		retn	4
	}
}

__declspec(naked) void TESObjectREFR::SetAngle(NiVector4 *rotVector, bool setLocal)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		mov		edi, eax
		lea		ecx, [eax+0x34]
		mov		edx, [esp+0xC]
		movups	xmm0, [edx]
		mulps	xmm0, PS_V3_PId180
		cmp		byte ptr [esp+0x10], 0
		jnz		localRot
		lea		edx, [esi+0x24]
		mov		eax, [edx+0xC]
		movups	[edx], xmm0
		mov		[edx+0xC], eax
		call	NiMatrix33::RotationMatrix
		jmp		doneRot
	localRot:
		movups	[edx], xmm0
		call	NiMatrix33::RotationMatrixInv
		lea		edx, [esi+0x24]
		mov		ecx, eax
		call	NiMatrix33::ExtractAngles
	doneRot:
		push	2
		mov		ecx, esi
		CALL_EAX(0x484B60)
		push	1
		push	edi
		CALL_EAX(0xC6BD00)
		add		esp, 8
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x1E4]
		test	eax, eax
		jnz		done
		mov		ecx, edi
		call	NiAVObject::Update
	done:
		pop		edi
		pop		esi
		retn	8
	}
}

__declspec(naked) void TESObjectREFR::MoveToCell(TESObjectCELL *cell, NiVector3 *posVector)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		call	TESObjectREFR::GetParentCell
		mov		ecx, [esp+8]
		test	eax, eax
		jz		doMove
		mov		edx, [esi+0x64]
		test	edx, edx
		jz		doMove
		cmp		dword ptr [edx+0x14], 0
		jz		doMove
		cmp		eax, ecx
		jz		doSetPos
		mov		edx, [ecx+0xC0]
		test	edx, edx
		jz		doMove
		cmp		[eax+0xC0], edx
		jz		doSetPos
	doMove:
		mov		eax, s_tempPosMarker
		mov		[eax+0x40], ecx
		mov		ecx, [esp+0xC]
		movq	xmm0, qword ptr [ecx]
		movq	qword ptr [eax+0x30], xmm0
		mov		edx, [ecx+8]
		mov		[eax+0x38], edx
		push	0
		push	0
		push	0
		push	eax
		push	esi
		CALL_EAX(ADDR_MoveToMarker)
		add		esp, 0x14
		pop		esi
		retn	8
		ALIGN 16
	doSetPos:
		push	dword ptr [esp+0xC]
		mov		ecx, esi
		call	TESObjectREFR::SetPos
		pop		esi
		retn	8
	}
}

__declspec(naked) bool __fastcall TESObjectREFR::GetTranslatedPos(NiVector4 *posMods)
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		mov		ecx, edx
		lea		edx, [eax+0x68]
		call	NiVector3::MultiplyMatrix
		movups	xmm1, [edx+0x24]
		addps	xmm0, xmm1
		movups	[eax], xmm0
		mov		al, 1
	done:
		retn
	}
}

__declspec(naked) void __fastcall TESObjectREFR::Rotate(NiVector4 *rotVector)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		mov		edi, eax
		movups	xmm0, [edx]
		mulps	xmm0, PS_V3_PId180
		movups	[edx], xmm0
		lea		ecx, [edi+0x34]
		call	NiMatrix33::Rotate
		lea		edx, [esi+0x24]
		mov		ecx, eax
		call	NiMatrix33::ExtractAngles
		push	2
		mov		ecx, esi
		CALL_EAX(0x484B60)
		push	1
		push	edi
		CALL_EAX(0xC6BD00)
		add		esp, 8
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x1E4]
		test	eax, eax
		jnz		done
		mov		ecx, edi
		call	NiAVObject::Update
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) bool TESObjectREFR::Disable()
{
	__asm
	{
		test	byte ptr [ecx+9], 0x48
		jnz		retnFalse
		test	byte ptr [ecx+0x52], 0x80
		jnz		retnFalse
		push	0
		push	ecx
		CALL_EAX(0x5AA500)
		add		esp, 8
		mov		al, 1
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

__declspec(naked) void TESObjectREFR::DeleteReference()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	1
		mov		eax, [ecx]
		call	dword ptr [eax+0xC4]
		push	1
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0xC8]
		push	0
		push	0
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x1CC]
		mov		ecx, [ebp-4]
		cmp		byte ptr [ecx+0xF], 0xFF
		jnz		done
		lea		eax, [ebp-4]
		push	eax
		mov		ecx, 0x11CACB8
		CALL_EAX(0x5AE3D0)
	done:
		leave
		retn
	}
}

__declspec(naked) bhkCharacterController *TESObjectREFR::GetCharacterController()
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		retnNULL
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnNULL
		cmp		byte ptr [ecx+0x28], 1
		ja		retnNULL
		mov		eax, [ecx+0x138]
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) double TESObjectREFR::GetWaterImmersionPerc()	// result >= 0.875 --> actor is diving
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		invalid
		cmp		byte ptr [ecx+0x14C], 0
		jz		invalid
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		invalid
		cmp		[eax+0x28], 0
		jnz		invalid
		push	dword ptr [ecx+0x40]
		push	dword ptr [ecx+0x38]
		CALL_EAX(0x885560)
		retn
	invalid:
		fldz
		retn
	}
}

bool TESObjectREFR::IsMobile()
{
	if (IS_ACTOR(this) || IsProjectile())
		return true;
	NiNode *objNode = GetNiNode();
	return objNode && objNode->IsMovable();
}

__declspec(naked) void TESObjectREFR::SwapTexture(const char *blockName, const char *filePath, UInt32 texIdx)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 8
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		mov		edx, [ebp+8]
		mov		ecx, eax
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		push	3
		mov		ecx, eax
		CALL_EAX(0xA59D30)
		test	eax, eax
		jz		done
		mov		[ebp-4], eax
		push	0
		push	0
		lea		ecx, [ebp-8]
		mov		dword ptr [ecx], 0
		push	ecx
		push	dword ptr [ebp+0xC]
		mov		ecx, g_TES
		CALL_EAX(0x4568C0)
		push	0x11F5AE0
		mov		ecx, [ebp-4]
		CALL_EAX(0x653290)
		test	eax, eax
		jz		noLighting
		mov		edx, [eax+0x1C]
		cmp		edx, 8
		jl		noLighting
		cmp		edx, 0xC
		jg		noLighting
		push	dword ptr [ebp-8]
		push	0
		push	dword ptr [ebp+0x10]
		mov		ecx, eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xF0]
	noLighting:
		push	0x11FA05C
		mov		ecx, [ebp-4]
		CALL_EAX(0x653290)
		test	eax, eax
		jz		release
		push	dword ptr [ebp-8]
		mov		ecx, eax
		CALL_EAX(0x438230)
	release:
		mov		ecx, [ebp-8]
		test	ecx, ecx
		jz		done
		call	NiReleaseObject
	done:
		leave
		retn	0xC
	}
}

__declspec(naked) NiNode *TESObjectREFR::GetNiNode()
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		cmp		dword ptr [ecx+0xC], 0x14
		jnz		done
		cmp		byte ptr [ecx+0x64A], 0
		jnz		done
		mov		eax, [ecx+0x694]
	done:
		retn
	}
}

__declspec(naked) NiAVObject* __fastcall TESObjectREFR::GetNiBlock(const char *blockName)
{
	__asm
	{
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		cmp		[edx], 0
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
	done:
		retn
	}
}

__declspec(naked) NiNode* __fastcall TESObjectREFR::GetNode(const char *nodeName)
{
	__asm
	{
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		cmp		[edx], 0
		jz		done
		mov		ecx, eax
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

__declspec(naked) hkpRigidBody* __fastcall TESObjectREFR::GetRigidBody(const char *blockName)
{
	__asm
	{
		call	TESObjectREFR::GetNiBlock
		test	eax, eax
		jz		done
		mov		eax, [eax+0x1C]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		done
		mov		eax, [eax+8]
		mov		dl, [eax+0xE8]
		cmp		dl, 3
		jbe		done
		cmp		dl, 6
		jz		done
		xor		eax, eax
	done:
		retn
	}
}

bool TESObjectREFR::IsGrabbable()
{
	if IS_ACTOR(this)
		return *(bool*)0x11E0B20 || HasHealth(0);
	if (IsProjectile())
	{
		Projectile *projRefr = (Projectile*)this;
		return (projRefr->projFlags & 0x200) && (projRefr->GetProjectileType() == 3) && projRefr->IsProximityTriggered();
	}
	NiNode *objNode = GetNiNode();
	return objNode && objNode->IsMovable();
}

const char kMatchPackageType[] = {0, 1, 2, 3, 4, 13, 14, 15, 16, 17, 18, 19, 36, 37, 35, 34, 33, -1, 5, 20, 7, 8, 10, 11, 9, 12, 21, 24, 6, 28, 29, 32};

__declspec(naked) char Actor::GetCurrentAIPackage()
{
	__asm
	{
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnInvalid
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		retnInvalid
		movsx	ecx, byte ptr [eax+0x20]
		cmp		ecx, 0x20
		jnl		retnInvalid
		mov		al, kMatchPackageType[ecx]
		retn
	retnInvalid:
		mov		al, -1
		retn
	}
}

const char kMatchProcedureID[] =
{
	0, 7, 1, 2, 8, 10, 11, 12, 14, 13, 16, 18, 4, 3, 19, 15, 5, 20, 9, -1, 6, -1, 21, 22, 23, 24, -1, 27, 28, 
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 47, -1, 46, 41, -1, -1, 44, -1, 43, 42, 49, 50, -1, 17,
	45, 48, 8, 10, 7, 46, 4, 45, 21, 0, -1
};

__declspec(naked) char Actor::GetCurrentAIProcedure()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnInvalid
		push	ecx
		push	ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		retnInvalid
		mov		[ebp-8], eax
		cmp		byte ptr [eax+0x20], 0x12
		jnz		notCombat
		mov		ecx, eax
		CALL_EAX(0x981990)
		test	al, al
		jz		notCombat
		mov		al, 0x10
		jmp		done
	notCombat:
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x280]
		mov		edx, [ebp-8]
		mov		edx, [edx+0x18]
		mov		ecx, 0x11A3FF0
		mov		ecx, [ecx+edx*4]
		mov		ecx, [ecx+eax*4]
		cmp		ecx, 0x36
		ja		retnInvalid
		cmp		ecx, 0x2A
		jnz		getID
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x274]
		test	eax, eax
		jz		sandbox
		mov		ecx, [eax+4]
		cmp		ecx, 3
		jnz		classify
		mov		ecx, [eax+8]
		cmp		ecx, 6
		jnl		sandbox
		add		ecx, 0x38
		jmp		getID
	classify:
		test	ecx, ecx
		js		sandbox
		cmp		ecx, 4
		jnl		sandbox
		add		ecx, 0x3E
		jmp		getID
	sandbox:
		mov		al, 0x2D
		jmp		done
	getID:
		mov		al, kMatchProcedureID[ecx]
		jmp		done
	retnInvalid:
		mov		al, -1
	done:
		leave
		retn
	}
}

bool Actor::IsFleeing()
{
	if (baseProcess)
	{
		TESPackage *package = baseProcess->GetCurrentPackage();
		if (package)
		{
			char pkgType = package->type;
			if (((pkgType == 0x12) && package->GetFleeCombat()) || (pkgType == 0x16) || (pkgType == 0xA))
				return true;
		}
	}
	return false;
}

__declspec(naked) ContChangesEntry *Actor::GetWeaponInfo()
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 1
		ja		retnNULL
		mov		eax, [eax+0x114]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) ContChangesEntry *Actor::GetAmmoInfo()
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 1
		ja		retnNULL
		mov		eax, [eax+0x118]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

TESObjectWEAP *Actor::GetEquippedWeapon()
{
	ContChangesEntry *weaponInfo = GetWeaponInfo();
	return weaponInfo ? (TESObjectWEAP*)weaponInfo->type : NULL;
}

bool Actor::IsItemEquipped(TESForm *item)
{
	if IS_ID(item, TESObjectWEAP)
		return item == GetEquippedWeapon();
	if (NOT_TYPE(item, TESObjectARMO) || (typeID == kFormType_Creature))
		return false;
	ContChangesEntry *entry = GetContainerChangesEntry(item);
	if (!entry || !entry->extendData)
		return false;
	ListNode<ExtraDataList> *node = entry->extendData->Head();
	ExtraDataList *xData;
	do
	{
		xData = node->data;
		if (xData && xData->HasType(kExtraData_Worn))
			return true;
	}
	while (node = node->next);
	return false;
}

bool __fastcall GetEntryDataHasModHook(ContChangesEntry *entry, int EDX, UInt8 modType);

UInt8 Actor::EquippedWeaponHasMod(UInt32 modType)
{
	ContChangesEntry *weaponInfo = GetWeaponInfo();
	if (!weaponInfo) return 0;
	TESObjectWEAP *weapon = (TESObjectWEAP*)weaponInfo->type;
	if (modType == 14)
	{
		if (!(weapon->weaponFlags1 & 4))
			return 0;
		if (!(weapon->weaponFlags2 & 0x2000))
			return 1;
	}
	else if ((modType == 11) && (weapon->soundLevel == 2))
		return 1;
	return GetEntryDataHasModHook(weaponInfo, 0, modType) ? 2 : 0;
}

bool Actor::IsSneaking()
{
	return actorMover && ((actorMover->GetMovementFlags() & 0xC00) == 0x400);
}

__declspec(naked) bool __fastcall Actor::IsInCombatWith(Actor *target)
{
	__asm
	{
		mov		eax, [ecx+0x12C]
		test	eax, eax
		jz		done
		mov		ecx, [eax+4]
		mov		eax, [eax+8]
		test	eax, eax
		jz		done
		ALIGN 16
	iterHead:
		cmp		[ecx], edx
		jz		rtnTrue
		add		ecx, 4
		dec		eax
		jnz		iterHead
		retn
	rtnTrue:
		mov		al, 1
	done:
		retn
	}
}

void Actor::StopCombat()
{
	if (!isInCombat) return;
	CombatController *combatCtrl = GetCombatController();
	if (combatCtrl) combatCtrl->stopCombat = true;
}

void Actor::UpdateActiveEffects(MagicItem *magicItem, EffectItem *effItem, bool addNew)
{
	ActiveEffectList *effList = magicTarget.GetEffectList();
	if (!effList) return;
	ActiveEffect *activeEff;
	ListNode<ActiveEffect> *iter = effList->Head();
	do
	{
		if (!(activeEff = iter->data) || (activeEff->magicItem != magicItem))
			continue;
		if (!addNew)
		{
			if (activeEff->effectItem != effItem) continue;
			if (activeEff->bTerminated) break;
		}
		MagicCaster *magCaster = activeEff->caster;
		ActiveEffect *newEff = magCaster->CreateActiveEffect(magicItem, effItem, activeEff->enchantObject);
		if (newEff)
		{
			MagicTarget *magTarget = activeEff->target;
			if (!addNew)
			{
				if (activeEff->timeElapsed >= newEff->duration)
				{
					newEff->Destroy(true);
					activeEff->Remove(true);
					break;
				}
				newEff->timeElapsed = activeEff->timeElapsed;
				newEff->bActive = activeEff->bActive;
				newEff->flags = activeEff->flags;
				activeEff->Remove(true);
			}
			newEff->target = magTarget;
			magTarget->ApplyEffect(magCaster, magicItem, newEff, false);
		}
		break;
	}
	while (iter = iter->next);
}

PackageInfo *Actor::GetPackageInfo()
{
	if (baseProcess)
	{
		if ((baseProcess->processLevel <= 1) && ((MiddleHighProcess*)baseProcess)->interruptPackage.package)
			return &((MiddleHighProcess*)baseProcess)->interruptPackage;
		if (baseProcess->currentPackage.package)
			return &baseProcess->currentPackage;
	}
	return NULL;
}

TESObjectREFR *Actor::GetPackageTarget()
{
	return baseProcess ? baseProcess->currentPackage.targetRef : NULL;
}

TESCombatStyle *Actor::GetCombatStyle()
{
	ExtraCombatStyle *xCmbStyle = GetExtraType(&extraDataList, CombatStyle);
	if (xCmbStyle && xCmbStyle->combatStyle) return xCmbStyle->combatStyle;
	return ((TESActorBase*)baseForm)->GetCombatStyle();
}

bool Actor::GetKnockedState()
{
	return baseProcess && (baseProcess->GetKnockedState() == 3);
}

bool Actor::IsWeaponOut()
{
	return baseProcess && baseProcess->IsWeaponOut();
}

bool Actor::GetIsGhost()
{
	if (baseProcess && baseProcess->cachedValues)
		return (baseProcess->cachedValues->flags & 0x10000000) != 0;
	return extraDataList.HasType(kExtraData_Ghost);
}

float Actor::GetRadiationLevel()
{
	float result = 0;
	if (inWater)
	{
		TESWaterForm *waterForm = NULL;
		if (renderState && renderState->waterRef)
		{
			waterForm = ((BGSPlaceableWater*)renderState->waterRef->baseForm)->water;
			if (waterForm && waterForm->waterForm) waterForm = waterForm->waterForm;
		}
		else waterForm = ThisCall<TESWaterForm*>(0x547770, parentCell);
		if (waterForm && waterForm->radiation) result = waterForm->radiation * (isSwimming ? *(float*)0x11D0464 : *(float*)0x11D1468);
	}
	TESObjectREFR *refr;
	ExtraRadius *xRadius;
	ExtraRadiation *xRadiation;
	float distance;
	for (auto iter = g_loadedReferences->radiationEmitters.Begin(); iter; ++iter)
	{
		if (!(refr = iter.Get())) continue;
		xRadius = GetExtraType(&refr->extraDataList, Radius);
		if (!xRadius) continue;
		distance = xRadius->radius - GetDistance(refr);
		if (distance <= 0) continue;
		xRadiation = GetExtraType(&refr->extraDataList, Radiation);
		if (xRadiation) result += xRadiation->radiation * distance / xRadius->radius;
	}
	return result ? ((1.0 - (avOwner.GetActorValue(kAVCode_RadResist) * 0.01)) * result) : 0;
}

__declspec(naked) void __fastcall Actor::TurnToFaceObject(TESObjectREFR *target)
{
	__asm
	{
		push	0
		push	dword ptr [edx+0x38]
		push	dword ptr [edx+0x34]
		push	dword ptr [edx+0x30]
		CALL_EAX(0x8BB520)
		retn
	}
}

__declspec(naked) void __vectorcall Actor::TurnAngle(float angle)
{
	__asm
	{
		push	0
		mulss	xmm0, PS_V3_PId180
		addss	xmm0, [ecx+0x2C]
		push	ecx
		movss	[esp], xmm0
		CALL_EAX(0x8BB5C0)
		retn
	}
}

__declspec(naked) void Actor::PlayAnimGroup(UInt32 animGroupID)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x1E4]
		test	eax, eax
		jz		done
		mov		edi, eax
		push	0x10000000
		mov		ecx, esi
		CALL_EAX(0x484B60)
		mov		edx, [esp+0xC]
		push	edi
		push	0
		push	0
		push	edx
		mov		ecx, esi
		CALL_EAX(0x897910)
		push	0xFFFFFFFF
		push	0xFFFFFFFF
		push	1
		movzx	edx, ax
		push	edx
		mov		ecx, edi
		CALL_EAX(0x494740)
		push	dword ptr [edi+0xE4]
		push	0xE
		mov		ecx, esi
		CALL_EAX(0x8A73E0)
		push	1
		mov		ecx, esi
		CALL_EAX(0x55D420)
	done:
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) UInt32 Actor::GetLevel()
{
	__asm
	{
		mov		ecx, [ecx+0x20]
		add		ecx, 0x30
		CALL_EAX(0x47DED0)
		movzx	eax, ax
		retn
	}
}

__declspec(naked) double Actor::GetKillXP()
{
	__asm
	{
		push	ecx
		mov		ecx, [ecx+0x20]
		add		ecx, 0x30
		CALL_EAX(0x47DED0)
		pop		ecx
		movzx	edx, ax
		push	edx
		cmp		byte ptr [ecx+4], kFormType_Creature
		setnz	al
		movzx	edx, al
		push	edx
		CALL_EAX(0x6705B0)
		add		esp, 8
		cvtsi2ss	xmm0, eax
		mov		eax, g_thePlayer
		push	dword ptr [eax+0x7B8]
		push	ecx
		movss	[esp], xmm0
		CALL_EAX(0x648C80)
		pop		ecx
		fstp	dword ptr [esp]
		CALL_EAX(0x406CE0)
		pop		ecx
		retn
	}
}

__declspec(naked) double __fastcall AdjustDmgByDifficulty(ActorHitData *hitData)
{
	__asm
	{
		mov		edx, g_thePlayer
		mov		eax, 0x119B310
		cmp		dword ptr [ecx+4], edx
		jz		isPlayer
		add		eax, 0x14
	isPlayer:
		mov		edx, [edx+0x7B8]
		mov		eax, [eax+edx*4]
		fld		dword ptr [ecx+0x14]
		fmul	dword ptr [eax+4]
		retn
	}
}

void Actor::GetHitDataValue(UInt32 valueType, double *result)
{
	*result = 0;
	if (NOT_ACTOR(this) || !baseProcess || (baseProcess->processLevel > 1))
		return;
	ActorHitData *hitData = ((MiddleHighProcess*)baseProcess)->hitData240;
	if (!hitData) return;
	switch (valueType)
	{
		case 0:
			*result = AdjustDmgByDifficulty(hitData);
			break;
		case 1:
			*result = hitData->limbDmg;
			break;
		case 2:
			if (hitData->source)
				*(UInt32*)result = hitData->source->refID;
			break;
		case 3:
			if (hitData->projectile)
				*(UInt32*)result = hitData->projectile->refID;
			break;
		case 4:
			if (hitData->weapon)
				*(UInt32*)result = hitData->weapon->refID;
			break;
		case 5:
			if (hitData->flags & 0x80000000)
				*result = 1;
			break;
		case 6:
			*result = hitData->wpnBaseDmg;
			break;
		case 7:
			*result = hitData->fatigueDmg;
			break;
		case 8:
			*result = hitData->armorDmg;
			break;
	}
}

__declspec(naked) void Actor::DismemberLimb(UInt32 bodyPartID, bool explode)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		cmp		dword ptr [ebp+8], 0xD
		ja		done
		push	ecx
		push	ecx
		push	dword ptr [ebp+8]
		CALL_EAX(0x573090)
		test	al, al
		jnz		done
		mov		ecx, [ebp-4]
		CALL_EAX(0x87F3D0)
		test	al, al
		jnz		done
		push	0
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x1A0]
		test	al, al
		jnz		done
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x20]
		mov		eax, [ecx]
		call	dword ptr [eax+0x180]
		test	eax, eax
		jz		done
		mov		edx, [ebp+8]
		mov		eax, [eax+edx*4+0x34]
		test	eax, eax
		jz		done
		test	byte ptr [eax+0x60], 9
		jz		done
		movzx	eax, byte ptr [eax+0x63]
		mov		[ebp-8], eax
		mov		ecx, [ebp-4]
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 0
		jnz		done
		mov		ecx, [eax+edx*4+0x1DC]
		test	ecx, ecx
		jz		done
		push	1
		CALL_EAX(0x456630)
		test	al, al
		jnz		done
		movzx	eax, [ebp+0xC]
		push	eax
		push	dword ptr [ebp+8]
		push	1
		push	0
		push	dword ptr [ebp+8]
		mov		ecx, [ebp-4]
		CALL_EAX(0x572FC0)
		push	0
		push	1
		push	1
		movzx	eax, [ebp+0xC]
		push	eax
		push	dword ptr [ebp+8]
		push	dword ptr [ebp-8]
		mov		ecx, [ebp-4]
		CALL_EAX(0x8B52A0)
	done:
		leave
		retn	8
	}
}

bool Actor::HasNoPath()
{
	return actorMover && actorMover->pathingRequest && !actorMover->pathingSolution;
}

__declspec(naked) bool Actor::CanBePushed()
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		dword ptr [eax+0x28], 0
		jnz		retn0
		cmp		dword ptr [eax+0x140], 0
		jnz		retn0
		mov		eax, [eax+0x138]
		test	eax, eax
		jz		done
		mov		eax, [ecx+0x108]
		cmp		eax, 1
		jz		retn0
		cmp		eax, 2
		//jz		retn0

		setnz	al
		retn

		/*mov		eax, [ecx+0x20]
		test	dword ptr [eax+0x34], 0xC000000
		jnz		retn0
		mov		eax, [ecx]
		call	dword ptr [eax+0x4B4]
		not		al
		retn*/
	retn0:
		xor		al, al
	done:
		retn
	}
}

__declspec(naked) float Actor::AdjustPushForce(float baseForce)
{
	__asm
	{
		push	0xA
		add		ecx, 0xA4
		CALL_EAX(0x66EF50)
		fmul	dword ptr SS_10
		fmul	dword ptr ds:[0x11CEA6C]
		fadd	dword ptr ds:[0x11CE664]
		fld		dword ptr [esp+4]
		fmul	dword ptr ds:[0x11CFA20]
		fld		dword ptr ds:[0x11CF9C0]
		test	byte ptr [esp+7], 0x80
		jz		posForce
		fchs
	posForce:
		faddp	st(1), st
		fmulp	st(1), st
		retn	4
	}
}

__declspec(naked) void Actor::PushActor(float force, float angle, TESObjectREFR *originRef, bool adjustForce)
{
	static const float kPushActor[] = {5 / 6.999125481F, 1 / 96.0F};
	__asm
	{
		push	esi
		mov		esi, ecx
		call	Actor::CanBePushed
		test	al, al
		jz		done
		cmp		dword ptr [esp+8], 0
		jnz		hasForce
		mov		esi, [esi+0x68]
		mov		esi, [esi+0x138]
		xorps	xmm0, xmm0
		movaps	[esi+0x500], xmm0
		mov		dword ptr [esi+0x524], 0
		jmp		done
	hasForce:
		mov		eax, [esp+0x10]
		test	eax, eax
		jnz		useRef
		movss	xmm0, [esp+0xC]
		mulss	xmm0, PS_V3_PId180
		call	GetSinCos
		jmp		doneAngle
	useRef:
		movq	xmm0, qword ptr [esi+0x30]
		movq	xmm1, qword ptr [eax+0x30]
		subps	xmm0, xmm1
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		haddps	xmm1, xmm1
		comiss	xmm1, PS_Epsilon
		jb		done
		rsqrtss	xmm1, xmm1
		unpcklps	xmm1, xmm1
		mulps	xmm0, xmm1
	doneAngle:
		cmp		[esp+0x14], 0
		jz		doneForce
		push	dword ptr [esp+8] 
		mov		ecx, esi
		call	Actor::AdjustPushForce
		fstp	dword ptr [esp+8]
	doneForce:
		mov		edx, [esp+8]
		movd	xmm2, edx
		mulss	xmm2, kPushActor
		and		edx, 0x7FFFFFFF
		movd	xmm3, edx
		mulss	xmm3, kPushActor+4
		mulss	xmm2, xmm3
		unpcklps	xmm2, xmm2
		mulps	xmm0, xmm2
		mov		esi, [esi+0x68]
		mov		esi, [esi+0x138]
		movaps	[esi+0x500], xmm0
		movss	[esi+0x524], xmm3
		or		byte ptr [esi+0x417], 0x80
	done:
		pop		esi
		retn	0x10
	}
}

__declspec(naked) int Actor::GetGroundMaterial()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x50
		push	esi
		mov		esi, ecx
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		invalid
		mov		eax, [esi+0x68]
		test	eax, eax
		jz		invalid
		cmp		dword ptr [eax+0x28], 1
		ja		invalid
		mov		eax, [eax+0x138]
		test	eax, eax
		jz		invalid
		test	byte ptr [eax+0x3F0], 2
		jnz		invalid
		mov		eax, [eax+0x434]
		cmp		eax, 0x24
		jnz		convert
		mov		eax, [ecx+0x40]
		test	eax, eax
		jz		invalid
		mov		ecx, eax
		CALL_EAX(0x546FB0)
		test	eax, eax
		jz		invalid
		mov		ecx, eax
		push	1
		lea		edx, [esi+0x30]
		push	edx
		lea		edx, [ebp-0x50]
		push	edx
		mov		esi, ecx
		CALL_EAX(0x53B550)
		movzx	edx, word ptr [ebp-0x14]
		push	edx
		movzx	edx, byte ptr [ebp-0x38]
		push	edx
		mov		ecx, esi
		CALL_EAX(0x53A630)
		test	eax, eax
		jz		invalid
		movzx	eax, byte ptr [eax+0x1C]
	convert:
		cmp		eax, 0x1F
		ja		invalid
		mov		edx, eax
		movzx	eax, kMaterialConvert[edx]
		jmp		done
	invalid:
		mov		eax, 0xFFFFFFFF
	done:
		pop		esi
		leave
		retn
	}
}

void MagicTarget::RemoveEffect(EffectItem *effItem)
{
	ActiveEffectList *effList = GetEffectList();
	if (!effList) return;
	ListNode<ActiveEffect> *iter = effList->Head(), *prev = NULL;
	ActiveEffect *activeEff;
	do
	{
		activeEff = iter->data;
		if (activeEff && (activeEff->effectItem == effItem))
		{
			activeEff->Remove(true);
			if (prev) iter = prev->RemoveNext();
			else iter->RemoveMe();
		}
		else
		{
			prev = iter;
			iter = iter->next;
		}
	}
	while (iter);
}

TESObjectREFR *TESObjectREFR::GetMerchantContainer()
{
	ExtraMerchantContainer *xMerchCont = GetExtraType(&extraDataList, MerchantContainer);
	return xMerchCont ? xMerchCont->containerRef : NULL;
}

TESActorBase *Actor::GetActorBase()
{
	if (baseForm->modIndex == 0xFF)
	{
		ExtraLeveledCreature *xLvlCre = GetExtraType(&extraDataList, LeveledCreature);
		if (xLvlCre) return xLvlCre->actorBase;
	}
	return (TESActorBase*)baseForm;
}

TESPackage *Actor::GetStablePackage()
{
	if (!baseProcess) return NULL;
	TESPackage *package = baseProcess->currentPackage.package;
	if (!package) return NULL;
	if ((package->type < 18) || (package->type == 26) || (package->type == 30)) return package;
	ExtraPackage *xPackage = GetExtraType(&extraDataList, Package);
	return xPackage ? xPackage->package : NULL;
}

bool Actor::GetLOS(Actor *target)
{
	if (baseProcess && !baseProcess->processLevel)
	{
		DetectionData *data = baseProcess->GetDetectionData(target, 0);
		if (data && data->inLOS) return true;
	}
	return false;
}

int __fastcall Actor::GetDetectionValue(Actor *detected)
{
	if (baseProcess && !baseProcess->processLevel)
	{
		DetectionData *data = baseProcess->GetDetectionData(detected, 0);
		if (data) return data->detectionValue;
	}
	return -100;
}

float s_moveAwayDistance = 300.0F;

__declspec(naked) BackUpPackage *Actor::AddBackUpPackage(TESObjectREFR *targetRef, TESObjectCELL *targetCell, UInt32 flags)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	ecx
		push	0
		mov		eax, [ecx+0x108]
		cmp		eax, 3
		jz		done
		cmp		eax, 5
		jz		done
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		done
		cmp		byte ptr [ecx+0x28], 0
		jnz		done
		mov		[ebp-8], ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		noPackage
		cmp		byte ptr [eax+0x20], 0x1A
		jz		done
	noPackage:
		push	0x8C
		GAME_HEAP_ALLOC
		mov		ecx, eax
		CALL_EAX(0x9ED030)
		mov		[ebp-0xC], eax
		push	dword ptr [ebp+8]
		push	dword ptr [ebp+0xC]
		push	dword ptr [ebp-4]
		mov		ecx, eax
		CALL_EAX(0x9ED0D0)
		mov		ecx, [ebp-0xC]
		cmp		dword ptr [ebp+0xC], 0
		jnz		haveCell
		mov		eax, [ebp+8]
		mov		edx, [eax+0x30]
		mov		[ecx+0x80], edx
		mov		edx, [eax+0x34]
		mov		[ecx+0x84], edx
		mov		edx, [eax+0x38]
		mov		[ecx+0x88], edx
	haveCell:
		mov		dword ptr [ecx+0x18], 0x30
		mov		eax, [ebp+0x10]
		or		dword ptr [ecx+0x1C], eax
		mov		ecx, [ebp-8]
		mov		eax, [ecx]
		call	dword ptr [eax+0x28]
		push	1
		push	1
		push	dword ptr [ebp-0xC]
		mov		ecx, [ebp-4]
		mov		eax, [ecx]
		call	dword ptr [eax+0x2F4]
	done:
		mov		s_moveAwayDistance, 0x43960000
		mov		eax, [ebp-0xC]
		leave
		retn	0xC
	}
}

int PlayerCharacter::GetDetectionState()
{
	if (!parentCell)
		return -1;
	if (pcUnseen)
		return 1;		// CAUTION
	if (pcInCombat)
		return 3;		// DANGER
	if (ProcessManager::Get()->GetTotalDetectionValue(this) <= 0)
		return 0;		// HIDDEN
	return 2;			// DETECTED
}

void PlayerCharacter::ToggleSneak(bool toggle)
{
	HighProcess *hiProcess = (HighProcess*)baseProcess;
	if (!hiProcess || hiProcess->processLevel || !actorMover)
		return;
	PlayerMover *pcMover = (PlayerMover*)actorMover;
	UInt32 movementFlags = pcMover->pcMovementFlags;
	bool isSneaking = (movementFlags & 0x400) != 0;
	if ((isSneaking == toggle) || lifeState || sitSleepState || (pcControlFlags & 0x40) ||
		hiProcess->knockedState || ((bool*)0x94423D)[hiProcess->currentAction] || GetIsParalyzed())
		return;
	//Sound::PlayEDID(toggle ? (const char*)0x108B050 : (const char*)0x108B064, 0x40102, this);
	if (toggle) movementFlags |= 0x400;
	else movementFlags &= ~0x400;
	pcMover->SetMovementFlags(movementFlags);
	ThisCall(0x93AB30, this, 0xA + isSneaking);
	if (!isUsingScope)
	{
		ThisCall(0x8BB650, this, 0, 0, 0);
		ThisCall(0x894CC0, this, 0);
	}
	if (hiProcess->cachedValues)
		hiProcess->cachedValues->flags &= ~0x3000;
}

void Projectile::GetData(UInt32 dataType, double *result)
{
	*result = 0;
	if (!IsProjectile()) return;
	switch (dataType)
	{
		case 0:
			if (sourceRef)
				*(UInt32*)result = sourceRef->refID;
			break;
		case 1:
			if (sourceWeap)
				*(UInt32*)result = sourceWeap->refID;
			break;
		case 2:
			*result = elapsedTime;
			break;
		case 3:
			*result = distTravelled;
			break;
		case 4:
			*result = hitDamage;
			break;
		case 5:
			*result = speedMult2;
			break;
		case 6:
		{
			if (hasImpacted)
			{
				ListNode<ImpactData> *traverse = impactDataList.Head();
				ImpactData *impactData;
				do
				{
					impactData = traverse->data;
					if (impactData && impactData->refr)
					{
						*(UInt32*)result = impactData->refr->refID;
						break;
					}
				}
				while (traverse = traverse->next);
			}
			break;
		}
		case 7:
		{
			if (hasImpacted)
			{
				ImpactData *impactData = impactDataList.GetFirstItem();
				if (impactData && (impactData->materialType <= 31))
					*result = (impactData && (impactData->materialType <= 31)) ? kMaterialConvert[impactData->materialType] : -1;
			}
			break;
		}
		default: break;
	}
}