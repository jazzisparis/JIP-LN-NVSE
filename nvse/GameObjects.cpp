#include "nvse/GameObjects.h"
#include "nvse/GameExtraData.h"
#include "nvse/GameTasks.h"

extern PlayerCharacter *g_thePlayer;

ScriptEventList *TESObjectREFR::GetEventList() const
{
	ExtraScript *xScript = GetExtraType(&extraDataList, Script);
	return xScript ? xScript->eventList : NULL;
}

PlayerCharacter *PlayerCharacter::GetSingleton()
{
	return g_thePlayer;
}

__declspec(naked) TESContainer *TESObjectREFR::GetContainer()
{
	__asm
	{
		mov		eax, [ecx]
		mov		eax, [eax+0x100]
		call	eax
		test	al, al
		mov		eax, [ecx+0x20]
		jz		notActor
		add		eax, 0x64
		retn
	notActor:
		cmp		dword ptr [eax], kVtbl_TESObjectCONT
		jnz		notCONT
		add		eax, 0x30
		retn
	notCONT:
		xor		eax, eax
		retn
	}
}

bool TESObjectREFR::IsMapMarker()
{
	return baseForm->refID == 0x10;
}

bool PlayerCharacter::SetSkeletonPath(const char* newPath)
{
	if (!bThirdPerson) {
		// ###TODO: enable in first person
		return false;
	}

	//// store parent of current niNode
	//NiNode* niParent = (NiNode*)(renderState->niNode->m_parent);

	//// set niNode to NULL via BASE CLASS Set3D() method
	//ThisCall(s_TESObjectREFR_Set3D, this, NULL);

	//// modify model path
	//if (newPath) {
	//	TESNPC* base = DYNAMIC_CAST(baseForm, TESForm, TESNPC);
	//	base->model.SetPath(newPath);
	//}

	//// create new NiNode, add to parent
	//*(g_bUpdatePlayerModel) = 1;
	//NiNode* newNode = (NiNode*)ThisCall(s_PlayerCharacter_GenerateNiNode, this);

	//niParent->AddObject(newNode, 1);
	//*(g_bUpdatePlayerModel) = 0;
	//newNode->SetName("Player");

	//// get and store camera node
	//// ### TODO: pretty this up
	//UInt32 vtbl = *((UInt32*)newNode);
	//UInt32 vfunc = *((UInt32*)(vtbl + 0x58));
	//NiObject* cameraNode = (NiObject*)ThisCall(vfunc, newNode, "Camera01");
	//*g_3rdPersonCameraNode = cameraNode;

	//cameraNode = (NiObject*)ThisCall(vfunc, (NiNode*)this->firstPersonNiNode, "Camera01");
	//*g_1stPersonCameraNode = cameraNode;

	//Unk_52();

	return true;
}

extern ModelLoader *g_modelLoader;

void TESObjectREFR::Update3D()
{
	if (this == g_thePlayer)
	{
		ThisCall(kUpdateAppearanceAddr, this);
	}
	else
	{
		Set3D(NULL, true);
		g_modelLoader->QueueReference(this, 1, 0);
	}
}

TESObjectREFR *TESObjectREFR::Create(bool bTemp)
{
	TESObjectREFR *refr = (TESObjectREFR*)GameHeapAlloc(sizeof(TESObjectREFR));
	ThisCall(s_TESObject_REFR_init, refr);
	if (bTemp) ThisCall(0x484490, refr);
	return refr;
}