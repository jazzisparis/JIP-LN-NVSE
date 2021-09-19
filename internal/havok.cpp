#include "internal/havok.h"

__declspec(naked) void hkMatrix3x4::operator=(const NiMatrix33 &inMatrix)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [eax]
		mov		[ecx], edx
		mov		edx, [eax+0xC]
		mov		[ecx+4], edx
		mov		edx, [eax+0x18]
		mov		[ecx+8], edx
		mov		edx, [eax+4]
		mov		[ecx+0x10], edx
		mov		edx, [eax+0x10]
		mov		[ecx+0x14], edx
		mov		edx, [eax+0x1C]
		mov		[ecx+0x18], edx
		mov		edx, [eax+8]
		mov		[ecx+0x20], edx
		mov		edx, [eax+0x14]
		mov		[ecx+0x24], edx
		mov		edx, [eax+0x20]
		mov		[ecx+0x28], edx
		xor		edx, edx
		mov		[ecx+0xC], edx
		mov		[ecx+0x1C], edx
		mov		[ecx+0x2C], edx
		retn	4
	}
}

bhkWorldM **g_bhkWorldM = (bhkWorldM**)0x11CA0D8;

__declspec(naked) void bhkWorldObject::ApplyForce(NiVector4 *forceVector)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0x1C
		push	ecx
		lea		eax, [esp+0x10]
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xF0]
		push	eax
		lea		eax, [esp+8]
		push	eax
		CALL_EAX(0x458620)
		add		esp, 8
		mov		eax, [ebp+8]
		lea		ecx, [esp+4]
		movss	xmm0, [ecx]
		subss	xmm0, [eax]
		movss	[ecx], xmm0
		movss	xmm0, [ecx+4]
		subss	xmm0, [eax+4]
		movss	[ecx+4], xmm0
		movss	xmm0, [ecx+8]
		subss	xmm0, [eax+8]
		movss	[ecx+8], xmm0
		call	NiVector3::Normalize
		mov		ecx, [esp]
		mov		ecx, [ecx+8]
		add		ecx, 0xE0
		CALL_EAX(0xC86430)
		fstp	dword ptr [esp+0x1C]
		mov		eax, [ebp+8]
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [esp+0x1C]
		lea		eax, [esp+4]
		movss	xmm1, [eax]
		mulss	xmm1, xmm0
		movss	[eax], xmm1
		movss	xmm1, [eax+4]
		mulss	xmm1, xmm0
		movss	[eax+4], xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, xmm0
		movss	[eax+8], xmm1
		push	eax
		mov		ecx, [esp+4]
		CALL_EAX(0x9B0BF0)
		leave
		retn	4
	}
}

__declspec(naked) void hkpRigidBody::SetAngularVelocity(UInt8 axis, float angVel)
{
	__asm
	{
		movss	xmm0, [esp+8]
		shufps	xmm0, xmm0, 0xC0
		movzx	edx, byte ptr [esp+4]
		shl		edx, 4
		mulps	xmm0, [ecx+edx+0xF0]
		movaps	[ecx+0x1C0], xmm0
		retn	8
	}
}