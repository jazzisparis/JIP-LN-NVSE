#include "nvse/GameObjects.h"
#include "nvse/GameExtraData.h"
#include "nvse/GameTasks.h"

__declspec(naked) TESForm *TESObjectREFR::GetBaseForm() const
{
	__asm
	{
		mov		eax, [ecx+0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0xF], 0xFF
		jnz		done
		mov		edx, [eax]
		cmp		edx, kVtbl_BGSPlaceableWater
		jz		isWater
		cmp		dword ptr [edx+0xF8], ADDR_ReturnTrue
		jnz		done
		push	eax
		push	kXData_ExtraLeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		pop		ecx
		test	eax, eax
		cmovz	eax, ecx
		jz		done
		mov		eax, [eax+0xC]
		retn
	isWater:
		mov		eax, [eax+0x4C]
	done:
		retn
	}
}

__declspec(naked) TESForm *TESObjectREFR::GetBaseForm2() const
{
	__asm
	{
		mov		eax, [ecx+0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0xF], 0xFF
		jnz		done
		mov		edx, [eax]
		cmp		dword ptr [edx+0xF8], ADDR_ReturnTrue
		jnz		retnNULL
		push	kXData_ExtraLeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0x10]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

ScriptLocals *TESObjectREFR::GetEventList() const
{
	ExtraScript *xScript = GetExtraType(&extraDataList, ExtraScript);
	return xScript ? xScript->eventList : nullptr;
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
	TESObjectREFR *refr = ThisCall<TESObjectREFR*>(0x55A2F0, Game_HeapAlloc<TESObjectREFR>());
	if (bTemp) ThisCall(0x484490, refr);
	return refr;
}

__declspec(naked) bool TESObjectREFR::GetDisabled() const
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

__declspec(naked) ContChangesEntryList *TESObjectREFR::GetContainerChangesList() const
{
	__asm
	{
		push	kXData_ExtraContainerChanges
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

__declspec(naked) ContChangesEntry *TESObjectREFR::GetContainerChangesEntry(TESForm *itemForm) const
{
	__asm
	{
		push	kXData_ExtraContainerChanges
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

__declspec(naked) SInt32 __fastcall GetFormCount(TESContainer::FormCountList *formCountList, ContChangesEntryList *objList, TESForm *form)
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

__declspec(naked) SInt32 TESObjectREFR::GetItemCount(TESForm *form) const
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		mov		ecx, [ecx+0x20]
		call	TESForm::GetContainer
		test	eax, eax
		jz		done
		pop		ecx
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
		NOP_0x2
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

void __fastcall SetContainerItemsHealthHook(TESContainer *container, int, float healthPerc);

__declspec(naked) void TESObjectREFR::AddItemAlt(TESForm *form, UInt32 count, float condition, UInt32 doEquip, UInt32 noMessage)
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
		call	SetContainerItemsHealthHook
		push	dword ptr [ebp+0x18]
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
		mov		ecx, [ebp-0x14]
		call	ContChangesEntryList::FindForItem
		test	eax, eax
		jz		eqpIter
		push	dword ptr [ebp+0x18]
		push	0
		push	eax
		mov		ecx, [ebp-4]
		call	Actor::EquipItemAlt
		jmp		eqpIter
		ALIGN 16
	done:
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x481680)
		pop		esi
		leave
		retn	0x14
	}
}

void TESObjectREFR::RemoveItemTarget(TESForm *itemForm, TESObjectREFR *target, SInt32 quantity, bool keepOwner)
{
	if IS_ID(itemForm, BGSListForm)
	{
		auto lstIter = ((BGSListForm*)itemForm)->list.Head();
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
		if (ContChangesEntry *entry = GetContainerChangesEntry(itemForm); entry && entry->extendData)
		{
			ExtraDataList *xData;
			while ((total > 0) && entry->extendData && (xData = entry->extendData->GetFirstItem()))
			{
				SInt32 subCount = xData->GetCount();
				if (subCount < 1)
					subCount = 1;
				else if (subCount > total)
					subCount = total;
				RemoveItem(itemForm, xData, subCount, keepOwner, 0, target, 0, 0, 1, 0);
				total -= subCount;
			}
		}
		if (total > 0)
			RemoveItem(itemForm, nullptr, total, keepOwner, 0, target, 0, 0, 1, 0);
	}
}

void __fastcall ShowItemMessage(TESForm *item, const char *msgStr);

__declspec(naked) void Actor::EquipItemAlt(ContChangesEntry *entry, UInt32 noUnequip, UInt32 noMessage)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		mov		ecx, [ebp+8]
		mov		eax, [ecx+8]
		push	eax
		mov		ecx, 1
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
		push	1
		push	dword ptr [ebp+0xC]
		push	1
		mov		eax, [ebp+8]
		mov		edx, [eax]
		test	edx, edx
		jz		noExtra
		mov		edx, [edx]
	noExtra:
		push	edx
		mov		edx, [eax+4]
		test	ecx, ecx
		cmovz	ecx, edx
		push	ecx
		push	dword ptr [ebp-8]
		mov		ecx, [ebp-4]
		CALL_EAX(ADDR_EquipItem)
		cmp		byte ptr [ebp+0x10], 0
		jnz		done
		mov		edx, ds:0x11D2A10
		mov		ecx, [ebp-8]
		call	ShowItemMessage
	done:
		leave
		retn	0xC
	}
}

bool TESObjectREFR::ValidForHooks() const
{
	if IS_ACTOR(this) return IsPersistent();
	return !kInventoryType[baseForm->typeID] && !IS_PROJECTILE(this);
}

__declspec(naked) TESObjectCELL *TESObjectREFR::GetParentCell() const
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		done
		push	kXData_ExtraPersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) TESWorldSpace *TESObjectREFR::GetParentWorld() const
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		getWorld
		push	kXData_ExtraPersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
	getWorld:
		mov		eax, [eax+0xC0]
	done:
		retn
	}
}

__declspec(naked) bool __fastcall TESObjectREFR::GetInSameCellOrWorld(TESObjectREFR *target) const
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		hasCell1
		push	edx
		push	kXData_ExtraPersistentCell
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
		push	kXData_ExtraPersistentCell
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

__declspec(naked) float __vectorcall TESObjectREFR::GetDistance(TESObjectREFR *target) const
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
		add		ecx, 0x30
		add		edx, 0x30
		jmp		Point3Distance
	fltMax:
		mov		eax, 0x7F7FFFFF
		movd	xmm0, eax
		retn
	}
}

__declspec(naked) void TESObjectREFR::SetPos(const NiVector3 &posVector)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		push	dword ptr [esp+0xC]
		CALL_EAX(0x575830)
		mov		eax, [esi+0x64]
		test	eax, eax
		jz		done
		mov		ecx, [eax+0x14]
		test	ecx, ecx
		jz		done
		mov		edi, ecx
		movups	xmm0, [esi+0x30]
		movlps	[ecx+0x58], xmm0
		pshufd	xmm1, xmm0, 2
		movss	[ecx+0x60], xmm1
		mov		eax, [esi]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		resetClsn
		mov		eax, [esi+0x68]
		test	eax, eax
		jz		doUpdate
		cmp		byte ptr [eax+0x28], 1
		ja		doUpdate
		mov		eax, [eax+0x138]
		test	eax, eax
		jz		doUpdate
		cmp		dword ptr [eax+0x3F0], 4
		jz		doUpdate
		mov		ecx, [eax+0x594]
		test	ecx, ecx
		jz		doUpdate
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		doUpdate
		add		ecx, 0xB0
		mulps	xmm0, kUnitConv
		movaps	xmm1, [eax+0x570]
		pshufd	xmm2, xmm1, 0
		mulps	xmm2, [ecx]
		addps	xmm0, xmm2
		pshufd	xmm2, xmm1, 0x55
		mulps	xmm2, [ecx+0x10]
		addps	xmm0, xmm2
		pshufd	xmm2, xmm1, 0xAA
		mulps	xmm2, [ecx+0x20]
		addps	xmm0, xmm2
		mov		ecx, [eax+8]
		movq	xmm1, qword ptr [ecx+0x58]
		xorps	xmm2, xmm2
		haddps	xmm1, xmm2
		movaps	[ecx+0x20], xmm2
		mov		ecx, [ecx+0x30]
		lea		eax, [ecx+0xE0]
		movaps	[eax], xmm0
		push	ecx
		movss	[esp], xmm1
		push	eax
		CALL_EAX(0xC9E990)
		jmp		doUpdate
	resetClsn:
		call	NiNode::ResetCollision
	doUpdate:
		push	0
		push	offset kNiUpdateData
		mov		ecx, edi
		CALL_EAX(0xA5DD70)
	done:
		pop		edi
		pop		esi
		retn	4
		ALIGN 16
	kUnitConv:
		EMIT_PS_3(0x3E124DD2)
	}
}

__declspec(naked) void __vectorcall TESObjectREFR::SetAngle(__m128 rotVector, UInt8 setMode)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		sub		esp, 0x24
		mov		ecx, esp
		cmp		dl, 1
		jnb		transMode
		movlps	[esi+0x24], xmm0
		pshufd	xmm1, xmm0, 2
		movss	[esi+0x2C], xmm1
		call	NiMatrix33::FromEulerPRY
		jmp		doneRot
	transMode:
		jnz		toPoint
		call	NiMatrix33::FromEulerPRYInv
		jmp		getEuler
	toPoint:
		movaps	xmm1, xmm0
		movups	xmm0, [esi+0x30]
		call	NiMatrix33::From2Points
	getEuler:
		call	NiMatrix33::ToEulerPRY
		movlps	[esi+0x24], xmm0
		movss	[esi+0x2C], xmm6
	doneRot:
		mov		eax, [esi]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jz		done
		mov		eax, [esi+0x64]
		test	eax, eax
		jz		done
		mov		ecx, [eax+0x14]
		test	ecx, ecx
		jz		done
		movups	xmm0, [esp]
		movups	[ecx+0x34], xmm0
		movups	xmm0, [esp+0x10]
		movups	[ecx+0x44], xmm0
		mov		edx, [esp+0x20]
		mov		[ecx+0x54], edx
		push	ecx
		call	NiNode::ResetCollision
		pop		ecx
		push	0
		push	offset kNiUpdateData
		CALL_EAX(0xA5DD70)
	done:
		MARK_MODIFIED(esi, 2)
		add		esp, 0x24
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall TESObjectREFR::MoveToCell(TESObjectCELL *cell, const NiVector3 &posVector)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		eax, [esi+0x40]
		test	eax, eax
		jz		doMove
		mov		ecx, [esi+0x64]
		test	ecx, ecx
		jz		doMove
		cmp		dword ptr [ecx+0x14], 0
		jz		doMove
		cmp		eax, edx
		jz		doSetPos
		mov		ecx, [edx+0xC0]
		test	ecx, ecx
		jz		doMove
		cmp		[eax+0xC0], ecx
		jz		doSetPos
	doMove:
		mov		eax, s_tempPosMarker
		mov		[eax+0x40], edx
		movups	xmm0, [esi+0x24]
		movups	[eax+0x24], xmm0
		mov		ecx, [esp+8]
		movups	xmm0, [ecx]
		movups	[eax+0x30], xmm0
		push	0
		push	0
		push	0
		push	eax
		push	esi
		CALL_EAX(ADDR_MoveToMarker)
		add		esp, 0x14
		pop		esi
		retn	4
		ALIGN 16
	doSetPos:
		mov		ecx, esi
		pop		esi
		jmp		TESObjectREFR::SetPos
	}
}

__declspec(naked) __m128 __vectorcall TESObjectREFR::GetTranslatedPos(const NiVector3 &posMods) const
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		noNode
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		noNode
		movups	xmm0, [edx]
		lea		ecx, [eax+0x68]
		call	NiMatrix33::MultiplyVector
		movups	xmm1, [ecx+0x24]
		addps	xmm0, xmm1
		retn
	noNode:
		sub		esp, 0x24
		push	ecx
		push	edx
		movups	xmm0, [ecx+0x24]
		lea		ecx, [esp+8]
		call	NiMatrix33::FromEulerPRY
		pop		edx
		movups	xmm0, [edx]
		mov		ecx, eax
		call	NiMatrix33::MultiplyVector
		pop		ecx
		movups	xmm1, [ecx+0x30]
		addps	xmm0, xmm1
		add		esp, 0x24
		retn
	}
}

__declspec(naked) void __vectorcall TESObjectREFR::Rotate(__m128 pry)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		sub		esp, 0x24
		mov		ecx, esp
		call	NiMatrix33::FromEulerPRYInv
		MARK_MODIFIED(esi, 2)
		cmp		dword ptr [esi+0xC], 0x14
		jz		noNode
		mov		eax, [esi+0x64]
		test	eax, eax
		jz		noNode
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		noNode
		mov		edx, esp
		lea		ecx, [eax+0x34]
		push	eax
		call	NiMatrix33::MultiplyMatrices
		mov		ecx, eax
		call	NiMatrix33::ToEulerPRY
		movlps	[esi+0x24], xmm0
		movss	[esi+0x2C], xmm6
		mov		eax, [esi]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jz		done
		mov		ecx, [esp]
		call	NiNode::ResetCollision
		mov		ecx, [esp]
		push	0
		push	offset kNiUpdateData
		CALL_EAX(0xA5DD70)
	done:
		add		esp, 0x28
		pop		esi
		retn
		ALIGN 16
	noNode:
		sub		esp, 0x24
		movups	xmm0, [esi+0x24]
		mov		ecx, esp
		call	NiMatrix33::FromEulerPRY
		lea		edx, [esp+0x24]
		mov		ecx, eax
		call	NiMatrix33::MultiplyMatrices
		mov		ecx, eax
		call	NiMatrix33::ToEulerPRY
		movlps	[esi+0x24], xmm0
		movss	[esi+0x2C], xmm6
		add		esp, 0x48
		pop		esi
		retn
	}
}

__declspec(naked) void __vectorcall TESObjectREFR::RotateAroundPoint(__m128 pry, const NiVector3 &origin, UInt32 skipAngles)
{
	__asm
	{
		cmp		[esp+4], 0
		jnz		noAngles
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		push	ebp
		mov		ebp, esp
		push	ecx
		push	eax
		sub		esp, 0x34
		mov		ecx, esp
		movups	xmm1, [edx]
		movups	[ecx+0x24], xmm1
		call	NiMatrix33::FromEulerYPR
		mov		edx, eax
		mov		ecx, [ebp-8]
		add		ecx, 0x34
		call	NiTransform::RotateOrigin
		call	NiMatrix33::ToEulerPRY
		mov		ecx, [ebp-4]
		movups	[ecx+0x24], xmm0
		mov		eax, [ebp-8]
		add		eax, 0x58
		mov		[ebp+8], eax
		leave
		jmp		TESObjectREFR::SetPos
	done:
		retn	4
	noAngles:
		push	ecx
		add		ecx, 0x30
		call	NiVector3::GetRotatedPos
		pop		ecx
		movlps	[eax], xmm0
		unpckhpd	xmm0, xmm0
		movss	[eax+8], xmm0
		mov		[esp+4], eax
		jmp		TESObjectREFR::SetPos
	}
}

__declspec(naked) float __vectorcall TESObjectREFR::GetHeadingAngle(TESObjectREFR *to) const
{
	__asm
	{
		movq	xmm0, qword ptr [edx+0x30]
		movq	xmm1, qword ptr [ecx+0x30]
		subps	xmm0, xmm1
		movshdup	xmm1, xmm0
		call	ATan2
		subss	xmm0, [ecx+0x2C]
		movq	xmm2, xmm0
		andps	xmm2, PS_FlipSignMask0
		movq	xmm1, xmm0
		xorps	xmm1, xmm2
		cmpnless	xmm1, PS_V3_PI
		andps	xmm1, PS_V3_PIx2
		xorps	xmm1, xmm2
		subss	xmm0, xmm1
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

__declspec(naked) bhkCharacterController *TESObjectREFR::GetCharacterController() const
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

__declspec(naked) double TESObjectREFR::GetWaterImmersionPerc() const	// result >= 0.875 --> actor is diving
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

bool TESObjectREFR::IsMobile() const
{
	if (IS_ACTOR(this) || IS_PROJECTILE(this))
		return true;
	NiNode *objNode = GetRefNiNode();
	return objNode && objNode->IsMovable();
}

__declspec(naked) NiTexture** __fastcall TESObjectREFR::GetTexturePtr(const char *blockName) const
{
	__asm
	{
		cmp		[edx], 0
		jz		retnNull
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		mov		ecx, eax
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0x1C], ADDR_ReturnThis2
		jnz		retnNull
		mov		eax, [eax+0xA8]
		test	eax, eax
		jz		done
		mov		dl, [eax+0x1C]
		cmp		dl, 8
		jz		ppLighting
		cmp		dl, 0x15
		jnz		retnNull
		add		eax, 0x60
		retn
	ppLighting:
		mov		eax, [eax+0xAC]
		retn
	retnNull:
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) void TESObjectREFR::SwapTexture(const char *blockName, const char *filePath, UInt32 texIdx)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	0
		call	TESObjectREFR::GetNiNode
		test	eax, eax
		jz		done
		mov		edx, [ebp+8]
		mov		ecx, eax
		call	NiNode::GetBlock
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		cmp		dword ptr [ecx+0x1C], ADDR_ReturnThis2
		jnz		done
		mov		eax, [eax+0xA8]
		test	eax, eax
		jz		done
		mov		ecx, 0x200100
		mov		edx, [eax+0x1C]
		bt		ecx, edx
		jnc		done
		push	eax
		push	0
		push	0
		lea		eax, [ebp-4]
		push	eax
		push	dword ptr [ebp+0xC]
		CALL_EAX(0x4568C0)
		mov		eax, [ebp-4]
		test	eax, eax
		jz		done
		pop		ecx
		push	eax
		cmp		byte ptr [ecx+0x1C], 8
		jnz		noLighting
		mov		edx, [ebp+0x10]
		mov		eax, [ecx+edx*4+0xAC]
		jmp		doSet
	noLighting:
		and		dword ptr [ecx+0x38], 0
		lea		eax, [ecx+0x60]
	doSet:
		push	eax
		call	NiReplaceObject
		pop		ecx
		call	NiReleaseObject
	done:
		leave
		retn	0xC
	}
}

__declspec(naked) NiNode *TESObjectREFR::GetNiNode() const
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

__declspec(naked) NiAVObject* __fastcall TESObjectREFR::GetNiBlock(const char *blockName) const
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

__declspec(naked) NiAVObject* __fastcall TESObjectREFR::GetNiBlock2(const char *blockName) const
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
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

__declspec(naked) NiNode* __fastcall TESObjectREFR::GetNode(const char *nodeName) const
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

__declspec(naked) NiNode* __fastcall TESObjectREFR::GetNode2(const char *nodeName) const
{
	__asm
	{
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
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

__declspec(naked) hkpRigidBody* __fastcall TESObjectREFR::GetRigidBody(const char *blockName) const
{
	__asm
	{
		call	TESObjectREFR::GetNiBlock2
		test	eax, eax
		jz		done
		mov		eax, [eax+0x1C]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x10]
		test	eax, eax
		jz		done
		mov		eax, [eax+8]
		xor		edx, edx
		cmp		byte ptr [eax+0x28], 1
		cmovnz	eax, edx
	done:
		retn
	}
}

__declspec(naked) void __fastcall TESObjectREFR::ToggleCollision(bool toggle)
{
	__asm
	{
		dec		dl
		and		dl, 0x40
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		notActor
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		notActor
		cmp		byte ptr [eax+0x28], 1
		ja		notActor
		mov		eax, [eax+0x138]
		test	eax, eax
		jz		notActor
		mov		eax, [eax+0x594]
		test	eax, eax
		jz		notActor
		mov		eax, [eax+8]
		test	eax, eax
		jz		notActor
		and     byte ptr [eax+0x2D], 0xBF
		or      [eax+0x2D], dl
	notActor:
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		ecx, [eax+0x14]
		test	ecx, ecx
		jz		done
		jmp		NiNode::ToggleCollision
	done:
		retn
	}
}

__declspec(naked) BSBound *TESObjectREFR::GetBoundingBox() const
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0xFC], ADDR_ReturnTrue
		jnz		getFromNode
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		getFromNode
		cmp		[eax+0x28], 1
		ja		getFromNode
		mov		eax, [eax+0x224]
		retn
	getFromNode:
		mov		eax, [ecx+0x64]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x14]
		test	eax, eax
		jz		done
		mov		edx, kVtbl_BSBound
		mov		ecx, eax
		jmp		NiObjectNET::GetExtraData
	done:
		retn
	}
}

__declspec(naked) MapMarkerData *TESObjectREFR::GetMapMarkerData() const
{
	__asm
	{
		xor		eax, eax
		mov		edx, [ecx+0x20]
		test	edx, edx
		jz		done
		cmp		dword ptr [edx+0xC], 0x10
		jnz		done
		push	kXData_ExtraMapMarker
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

bool TESObjectREFR::IsGrabbable() const
{
	if IS_ACTOR(this)
		return INIS_BOOL(bAllowHavokGrabTheLiving_GamePlay) || ((Actor*)this)->GetDead();
	if IS_PROJECTILE(this)
	{
		Projectile *projRefr = (Projectile*)this;
		return (projRefr->projFlags & 0x200) && (projRefr->GetProjectileType() == 3) && projRefr->IsProximityTriggered();
	}
	NiNode *objNode = GetRefNiNode();
	return objNode && objNode->IsMovable();
}

__declspec(naked) char Actor::GetCurrentAIPackage() const
{
	static const char kMatchPackageType[] = {0, 1, 2, 3, 4, 13, 14, 15, 16, 17, 18, 19, 36, 37, 35, 34, 33, -1, 5, 20, 7, 8, 10, 11, 9, 12, 21, 24, 6, 28, 29, 32};
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

__declspec(naked) char Actor::GetCurrentAIProcedure() const
{
	static const char kMatchProcedureID[] =
	{
		0, 7, 1, 2, 8, 10, 11, 12, 14, 13, 16, 18, 4, 3, 19, 15, 5, 20, 9, -1, 6, -1, 21, 22, 23, 24, -1, 27, 28,
		29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 47, -1, 46, 41, -1, -1, 44, -1, 43, 42, 49, 50, -1, 17,
		45, 48, 8, 10, 7, 46, 4, 45, 21, 0, -1
	};
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

__declspec(naked) bool Actor::IsFleeing() const
{
	__asm
	{
		mov		eax, [ecx]
		cmp		dword ptr [eax+0x100], ADDR_ReturnTrue
		jnz		retnFalse
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		retnFalse
		mov		eax, [ecx]
		call	dword ptr [eax+0x27C]
		test	eax, eax
		jz		retnFalse
		mov		dl, [eax+0x20]
		cmp		dl, 0xA
		jz		retnTrue
		cmp		dl, 0x16
		jz		retnTrue
		cmp		dl, 0x12
		jnz		retnFalse
		mov		ecx, eax
		JMP_EAX(0x981990)
	retnFalse:
		xor		al, al
		retn
	retnTrue:
		mov		al, 1
		retn
	}
}

__declspec(naked) ContChangesEntry *Actor::GetWeaponInfo() const
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x28], 1
		ja		retnNULL
		mov		eax, [eax+0x114]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

__declspec(naked) ContChangesEntry *Actor::GetAmmoInfo() const
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x28], 1
		ja		retnNULL
		mov		eax, [eax+0x118]
		retn
	retnNULL:
		xor		eax, eax
	done:
		retn
	}
}

TESObjectWEAP *Actor::GetEquippedWeapon() const
{
	ContChangesEntry *weaponInfo = GetWeaponInfo();
	return weaponInfo ? weaponInfo->weapon : nullptr;
}

bool Actor::IsItemEquipped(TESForm *item) const
{
	if IS_ID(item, TESObjectWEAP)
		return item == GetEquippedWeapon();
	if (NOT_TYPE(item, TESObjectARMO) || (typeID == kFormType_Creature))
		return false;
	if (ContChangesEntry *entry = GetContainerChangesEntry(item); entry && entry->extendData)
	{
		auto node = entry->extendData->Head();
		do
		{
			if (ExtraDataList *xData = node->data; xData && xData->HasType(kXData_ExtraWorn))
				return true;
		}
		while (node = node->next);
	}
	return false;
}

bool __fastcall GetEntryDataHasModHook(ContChangesEntry *entry, int, UInt8 modType);

__declspec(noinline) bool Actor::EquippedWeaponHasMod(UInt32 modType) const
{
	if (ContChangesEntry *weaponInfo = GetWeaponInfo(); weaponInfo && GetEntryDataHasModHook(weaponInfo, 0, modType))
		return true;
	return false;
}

__declspec(noinline) UInt8 Actor::EquippedWeaponHasScope() const
{
	if (ContChangesEntry *weaponInfo = GetWeaponInfo(); weaponInfo && (weaponInfo->weapon->weaponFlags1 & 4))
		if (!(weaponInfo->weapon->weaponFlags2 & 0x2000))
			return 1;
		else if (GetEntryDataHasModHook(weaponInfo, 0, 14))
			return 2;
	return 0;
}

__declspec(noinline) UInt8 Actor::EquippedWeaponSilenced() const
{
	if (ContChangesEntry *weaponInfo = GetWeaponInfo())
		if (weaponInfo->weapon->soundLevel == 2)
			return 1;
		else if (GetEntryDataHasModHook(weaponInfo, 0, 11))
			return 2;
	return 0;
}

bool Actor::IsSneaking() const
{
	return actorMover && ((actorMover->GetMovementFlags() & 0xC00) == 0x400);
}

__declspec(naked) bool __fastcall Actor::IsInCombatWith(Actor *target) const
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
	if (isInCombat)
		if (CombatController *combatCtrl = GetCombatController())
			combatCtrl->stopCombat = true;
}

void Actor::UpdateActiveEffects(MagicItem *magicItem, EffectItem *effItem, bool addNew)
{
	ActiveEffectList *effList = magicTarget.GetEffectList();
	if (!effList) return;
	ActiveEffect *activeEff;
	auto iter = effList->Head();
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

PackageInfo *Actor::GetPackageInfo() const
{
	if (baseProcess)
	{
		if ((baseProcess->processLevel <= 1) && ((MiddleHighProcess*)baseProcess)->interruptPackage.package)
			return &((MiddleHighProcess*)baseProcess)->interruptPackage;
		if (baseProcess->currentPackage.package)
			return &baseProcess->currentPackage;
	}
	return nullptr;
}

TESObjectREFR *Actor::GetPackageTarget() const
{
	return baseProcess ? baseProcess->currentPackage.targetRef : nullptr;
}

TESCombatStyle *Actor::GetCombatStyle() const
{
	if (ExtraCombatStyle *xCmbStyle = GetExtraType(&extraDataList, ExtraCombatStyle); xCmbStyle && xCmbStyle->combatStyle)
		return xCmbStyle->combatStyle;
	return ((TESActorBase*)baseForm)->GetCombatStyle();
}

SInt8 Actor::GetKnockedState() const
{
	return (baseProcess && (baseProcess->processLevel <= 1)) ? ((MiddleHighProcess*)baseProcess)->knockedState : -1;
}

bool Actor::IsWeaponOut() const
{
	return baseProcess && baseProcess->IsWeaponOut();
}

bool Actor::GetIsGhost() const
{
	if (baseProcess && baseProcess->cachedValues)
		return (baseProcess->cachedValues->flags & 0x10000000) != 0;
	return extraDataList.HasType(kXData_ExtraGhost);
}

float Actor::GetRadiationLevel() const
{
	float result = 0;
	if (inWater)
	{
		TESWaterForm *waterForm = nullptr;
		if (renderState && renderState->currWaterRef)
		{
			waterForm = ((BGSPlaceableWater*)renderState->currWaterRef->baseForm)->water;
			if (waterForm && waterForm->waterForm)
				waterForm = waterForm->waterForm;
		}
		else waterForm = ThisCall<TESWaterForm*>(0x547770, parentCell);
		if (waterForm && waterForm->radiation)
			result = waterForm->radiation * (isSwimming ? GMST_FLT(fSwimRadiationDamageMult) : GMST_FLT(fWadeRadiationDamageMult));
	}
	for (auto iter = g_loadedReferences->radiationEmitters.Begin(); iter; ++iter)
		if (TESObjectREFR *refr = iter.Get())
			if (auto xRadius = GetExtraType(&refr->extraDataList, ExtraRadius))
				if (float distance = xRadius->radius - GetDistance(refr); distance > 0)
					if (auto xRadiation = GetExtraType(&refr->extraDataList, ExtraRadiation))
						result += xRadiation->radiation * distance / xRadius->radius;
	if (result)
		result *= 1.0F - (avOwner.GetActorValue(kAVCode_RadResist) * 0.01F);
	return result;
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
		MARK_MODIFIED(esi, 0x10000000)
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

__declspec(naked) UInt32 Actor::GetLevel() const
{
	__asm
	{
		mov		ecx, [ecx+0x20]
		add		ecx, 0x30
		CALL_EAX(0x47DED0)
		cwde
		retn
	}
}

__declspec(naked) double Actor::GetKillXP() const
{
	__asm
	{
		push	ecx
		mov		ecx, [ecx+0x20]
		add		ecx, 0x30
		CALL_EAX(0x47DED0)
		pop		ecx
		cwde
		push	eax
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

__declspec(noinline) void __fastcall Actor::GetHitDataValue(UInt32 valueType, double *result) const
{
	if (IS_ACTOR(this) && baseProcess && (baseProcess->processLevel <= 1))
		if (ActorHitData *hitData = ((MiddleHighProcess*)baseProcess)->lastHitData)
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
						REFR_RES = hitData->source->refID;
					break;
				case 3:
					if (hitData->projectile)
						REFR_RES = hitData->projectile->refID;
					break;
				case 4:
					if (hitData->weapon)
						REFR_RES = hitData->weapon->refID;
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
		cmp		byte ptr [eax+0x28], 0
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

bool Actor::HasNoPath() const
{
	return actorMover && actorMover->pathingRequest && !actorMover->pathingSolution;
}

__declspec(naked) bool Actor::CanBePushed() const
{
	__asm
	{
		mov		eax, [ecx+0x68]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x28], 0
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

__declspec(naked) float __vectorcall Actor::AdjustPushForce(float baseForce)
{
	__asm
	{
		push	0xA
		add		ecx, 0xA4
		CALL_EAX(0x66EF50)
		fstp	dword ptr [esp-4]
		movss	xmm1, [esp-4]
		mulss	xmm1, SS_10
		mulss	xmm1, ds:0x11CEA6C
		addss	xmm1, ds:0x11CE664
		movq	xmm2, xmm0
		andps	xmm2, PS_FlipSignMask0
		xorps	xmm2, ds:0x11CF9C0
		mulss	xmm0, ds:0x11CFA20
		addss	xmm0, xmm2
		mulss	xmm0, xmm1
		retn
	}
}

__declspec(naked) void Actor::PushActor(float force, float angle, TESObjectREFR *originRef)
{
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
		and		dword ptr [esi+0x524], 0
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
		movq	xmm1, xmm0
		mulps	xmm1, xmm1
		haddps	xmm1, xmm1
		comiss	xmm1, PS_Epsilon
		jb		done
		rsqrtss	xmm1, xmm1
		unpcklps	xmm1, xmm1
		mulps	xmm0, xmm1
	doneAngle:
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
		retn	0xC
		ALIGN 8
	kPushActor:
		EMIT_DW(0x3F36E147) EMIT_DW(0x3C2AAAAB)
	}
}

__declspec(naked) int Actor::GetGroundMaterial() const
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
		cmp		byte ptr [eax+0x28], 1
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
		or		eax, 0xFFFFFFFF
	done:
		pop		esi
		leave
		retn
	}
}

__declspec(naked) void Actor::RefreshAnimData()
{
	__asm
	{
		push	esi
		mov		esi, [ecx+0x68]
		test	esi, esi
		jz		done
		cmp		dword ptr [esi+0x28], 0
		jnz		done
		mov		eax, [esi+0x1C0]
		test	eax, eax
		jz		done
		push	ecx
		cmp		byte ptr [esi+0x135], 0
		jz		skipBlend1
		cmp		dword ptr [eax+0xF0], 0
		jz		skipBlend1
		push	0
		push	4
		mov		ecx, eax
		CALL_EAX(0x4994F0)
		mov		eax, [esi+0x1C0]
	skipBlend1:
		push	0
		mov		ecx, eax
		CALL_EAX(0x499240)
		pop		ecx
		cmp		dword ptr [ecx+0xC], 0x14
		jnz		done
		cmp		byte ptr [esi+0x135], 0
		mov		esi, [ecx+0x690]
		jz		skipBlend2
		cmp		dword ptr [esi+0xF0], 0
		jz		skipBlend2
		push	0
		push	4
		mov		ecx, esi
		CALL_EAX(0x4994F0)
	skipBlend2:
		push	0
		mov		ecx, esi
		CALL_EAX(0x499240)
	done:
		pop		esi
		retn
	}
}

double Actor::GetPathingDistance(TESObjectREFR *target)
{
	PathingLocation from(this), to(target);
	PathingRequest::ActorData actorData(this);
	return CdeclCall<double>(0x6D4EB0, &from, &to, 0, &actorData, 0);
}

void MagicTarget::RemoveEffect(EffectItem *effItem)
{
	if (ActiveEffectList *effList = GetEffectList())
	{
		tList<ActiveEffect>::Node *iter = effList->Head(), *prev = nullptr;
		do
		{
			if (ActiveEffect *activeEff = iter->data; activeEff && (activeEff->effectItem == effItem))
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
}

TESObjectREFR *TESObjectREFR::GetMerchantContainer() const
{
	if (auto xMerchCont = GetExtraType(&extraDataList, ExtraMerchantContainer))
		return xMerchCont->containerRef;
	return nullptr;
}

__declspec(naked) TESActorBase *Actor::GetActorBase() const
{
	__asm
	{
		mov		eax, [ecx+0x20]
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0xF], 0xFF
		jnz		done
		push	eax
		push	kXData_ExtraLeveledCreature
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		mov		ecx, eax
		pop		eax
		test	ecx, ecx
		jz		done
		mov		ecx, [ecx+0x10]
		test	ecx, ecx
		cmovnz	eax, ecx
	done:
		retn
	}
}

TESPackage *Actor::GetStablePackage() const
{
	if (baseProcess)
		if (TESPackage *package = baseProcess->currentPackage.package)
			if ((package->type < 18) || (package->type == 26) || (package->type == 30))
				return package;
			else if (auto xPackage = GetExtraType(&extraDataList, ExtraPackage))
				return xPackage->package;
	return nullptr;
}

bool Actor::GetLOS(Actor *target) const
{
	if (baseProcess && !baseProcess->processLevel)
		if (DetectionData *data = baseProcess->GetDetectionData(target, 0); data && data->inLOS)
			return true;
	return false;
}

int __fastcall Actor::GetDetectionValue(Actor *detected) const
{
	if (baseProcess && !baseProcess->processLevel)
		if (DetectionData *data = baseProcess->GetDetectionData(detected, 0))
			return data->detectionValue;
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
		call	Game_DoHeapAlloc
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

__declspec(noinline) int PlayerCharacter::GetDetectionState()
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

void __fastcall Projectile::GetData(UInt32 dataType, double *result) const
{
	if IS_PROJECTILE(this)
		switch (dataType)
		{
			case 0:
				if (sourceRef)
					REFR_RES = sourceRef->refID;
				break;
			case 1:
				if (sourceWeap)
					REFR_RES = sourceWeap->refID;
				break;
			case 2:
				*result = lifeTime;
				break;
			case 3:
				*result = distTravelled;
				break;
			case 4:
				*result = hitDamage;
				break;
			case 5:
				*result = speedMult;
				break;
			case 6:
			{
				if (hasImpacted)
				{
					auto traverse = impactDataList.Head();
					do
					{
						if (ImpactData *impactData = traverse->data; impactData && impactData->refr)
						{
							REFR_RES = impactData->refr->refID;
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
					if (ImpactData *impactData = impactDataList.GetFirstItem(); impactData && (impactData->materialType <= 31))
					{
						*result = kMaterialConvert[impactData->materialType];
						break;
					}
				*result = -1;
				break;
			}
			default: break;
		}
}