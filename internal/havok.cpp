#include "internal/havok.h"

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