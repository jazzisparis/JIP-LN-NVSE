#include "internal/havok.h"

void hkQuaternion::operator=(const NiVector3 &eulerYPR)
{
	NiMatrix33 rotMat;
	rotMat.RotationMatrix((NiVector3*)&eulerYPR);
	*this = rotMat;
}

__declspec(naked) void __fastcall hkQuaternion::operator=(const NiMatrix33 &mat)
{
	__asm
	{
		movss	xmm0, [edx]
		movss	xmm1, [edx+0x10]
		movss	xmm2, [edx+0x20]
		movaps	xmm3, xmm0
		addss	xmm3, xmm1
		addss	xmm3, xmm2
		movss	xmm4, kFltOne
		movaps	xmm5, xmm4
		pxor	xmm6, xmm6
		comiss	xmm3, xmm6
		jbe		t_xyz
		addss	xmm5, xmm3
		movss	[ecx+0xC], xmm5
		movss	xmm0, [edx+0x1C]
		subss	xmm0, [edx+0x14]
		movss	[ecx], xmm0
		movss	xmm0, [edx+8]
		subss	xmm0, [edx+0x18]
		movss	[ecx+4], xmm0
		movss	xmm0, [edx+0xC]
		subss	xmm0, [edx+4]
		movss	[ecx+8], xmm0
		jmp		done
	t_xyz:
		comiss	xmm0, xmm1
		jnb		t_xz
		comiss	xmm1, xmm2
		jb		t_z
		subss	xmm5, xmm0
		addss	xmm5, xmm1
		subss	xmm5, xmm2
		movss	xmm0, [edx+8]
		subss	xmm0, [edx+0x18]
		movss	[ecx+0xC], xmm0
		movss	xmm0, [edx+0xC]
		addss	xmm0, [edx+4]
		movss	[ecx], xmm0
		movss	[ecx+4], xmm5
		movss	xmm0, [edx+0x1C]
		addss	xmm0, [edx+0x14]
		movss	[ecx+8], xmm0
		jmp		done
	t_xz:
		comiss	xmm0, xmm2
		jb		t_z
		addss	xmm5, xmm0
		subss	xmm5, xmm1
		subss	xmm5, xmm2
		movss	xmm0, [edx+0x1C]
		subss	xmm0, [edx+0x14]
		movss	[ecx+0xC], xmm0
		movss	[ecx], xmm5
		movss	xmm0, [edx+0xC]
		addss	xmm0, [edx+4]
		movss	[ecx+4], xmm0
		movss	xmm0, [edx+0x18]
		addss	xmm0, [edx+8]
		movss	[ecx+8], xmm0
		jmp		done
	t_z:
		subss	xmm5, xmm0
		subss	xmm5, xmm1
		addss	xmm5, xmm2
		movss	xmm0, [edx+0xC]
		subss	xmm0, [edx+4]
		movss	[ecx+0xC], xmm0
		movss	xmm0, [edx+0x18]
		addss	xmm0, [edx+8]
		movss	[ecx], xmm0
		movss	xmm0, [edx+0x1C]
		addss	xmm0, [edx+0x14]
		movss	[ecx+4], xmm0
		movss	[ecx+8], xmm5
	done:
		andps	xmm5, kSSERemoveSignMaskPS
		sqrtss	xmm0, xmm5
		addss	xmm0, xmm0
		divss	xmm4, xmm0
		shufps	xmm4, xmm4, 0
		movups	xmm0, [ecx]
		mulps	xmm0, xmm4
		movups	[ecx], xmm0
		retn
	}
}

__declspec(naked) void __fastcall hkQuaternion::operator*=(const hkQuaternion &rhs)
{
	__asm
	{
		pshufd	xmm0, [ecx], 0x93
		pshufd	xmm1, [edx], 0x93
		mov		edx, 0x80000000
		movd	xmm2, edx
		movaps	xmm3, xmm1
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 1
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		movss	[ecx], xmm3
		pshufd	xmm3, xmm1, 0xB1
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 0x15
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		movss	[ecx+4], xmm3
		pshufd	xmm3, xmm1, 0x4E
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 0x51
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		movss	[ecx+8], xmm3
		pshufd	xmm3, xmm1, 0x1B
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 0x45
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		movss	[ecx+0xC], xmm3
		retn
	}
}

__declspec(naked) hkQuaternion *hkQuaternion::Normalize()
{
    __asm
    {
		mov		eax, ecx
		movaps	xmm0, [eax]
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		haddps	xmm1, xmm1
		haddps	xmm1, xmm1
		pxor	xmm2, xmm2
		comiss	xmm1, xmm2
		jz		zeroLen
		rsqrtss	xmm2, xmm1
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm0
		movaps	[eax], xmm2
		retn
	zeroLen:
		movaps	[eax], xmm2
		mov		dword ptr [eax+0xC], 0x3F800000
        retn
    }
}

void hkQuaternion::EulerYPR(NiVector3 &ypr)
{
	ypr.x = atan2f(2.0F * (w * x + y * z), 1.0F - (2.0F * (x * x + y * y)));
	float sinp = 2.0F * (w * y - z * x);
	if (abs(sinp) < 1.0F)
		ypr.y = asinf(sinp);
	else
		ypr.y = (sinp > 0) ? kFltPId2 : -kFltPId2;
	ypr.z = atan2f(2.0F * (w * z + x * y), 1.0F - (2.0F * (y * y + z * z)));
}

__declspec(naked) void __fastcall hkMatrix3x4::operator=(const NiMatrix33 &inMatrix)
{
	__asm
	{
		mov		eax, [edx]
		mov		[ecx], eax
		mov		eax, [edx+0xC]
		mov		[ecx+4], eax
		mov		eax, [edx+0x18]
		mov		[ecx+8], eax
		mov		eax, [edx+4]
		mov		[ecx+0x10], eax
		mov		eax, [edx+0x10]
		mov		[ecx+0x14], eax
		mov		eax, [edx+0x1C]
		mov		[ecx+0x18], eax
		mov		eax, [edx+8]
		mov		[ecx+0x20], eax
		mov		eax, [edx+0x14]
		mov		[ecx+0x24], eax
		mov		eax, [edx+0x20]
		mov		[ecx+0x28], eax
		xor		eax, eax
		mov		[ecx+0xC], eax
		mov		[ecx+0x1C], eax
		mov		[ecx+0x2C], eax
		retn
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