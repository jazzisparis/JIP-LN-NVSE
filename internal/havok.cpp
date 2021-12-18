#include "internal/havok.h"

__declspec(naked) hkQuaternion* __fastcall hkQuaternion::FromEulerYPR(const NiVector3 &ypr)
{
	__asm
	{
		movups	xmm7, [edx]
		movaps	xmm0, kVcPI
		cmpps	xmm0, xmm7, 1
		andps	xmm0, kVcPIx2
		subps	xmm7, xmm0
		mulps	xmm7, kVcHalf
		pshufd	xmm0, xmm7, 0xFC
		call	GetSinCos
		movaps	xmm6, xmm0
		pshufd	xmm0, xmm7, 0xFD
		call	GetSinCos
		movaps	xmm5, xmm0
		pshufd	xmm0, xmm7, 0xFE
		call	GetSinCos
		pshufd	xmm1, xmm0, 0x11
		pshufd	xmm2, xmm5, 0x41
		mulps	xmm1, xmm2
		pshufd	xmm0, xmm1, 0xE0
		pshufd	xmm5, xmm6, 0x51
		mulps	xmm0, xmm5
		pshufd	xmm5, xmm1, 0xB5
		shufps	xmm6, xmm6, 4
		mulps	xmm5, xmm6
		xorps	xmm5, kSSEChangeSignMaskPD
		addps	xmm0, xmm5
		shufps	xmm0, xmm0, 0x39
		movaps	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) hkQuaternion* __fastcall hkQuaternion::FromRotationMatrix(const NiMatrix33 &mat)
{
	__asm
	{
		movss	xmm0, [edx]
		movss	xmm1, [edx+0x10]
		movss	xmm2, [edx+0x20]
		movss	xmm3, xmm0
		addss	xmm3, xmm1
		addss	xmm3, xmm2
		movss	xmm4, kFltOne
		pxor	xmm5, xmm5
		comiss	xmm3, xmm5
		jbe		t_xyz
		addss	xmm3, xmm4
		movaps	xmm2, xmm0
		movhlps	xmm2, xmm1
		unpcklps	xmm0, xmm1
		movhlps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0x8C
		subps	xmm1, xmm2
		movss	xmm1, xmm3
		shufps	xmm1, xmm1, 0x39
		jmp		done
	t_xyz:
		movss	xmm3, xmm4
		pshufd	xmm5, kSSEChangeSignMaskPS0, 0x51
		comiss	xmm0, xmm1
		jnb		t_xz
		comiss	xmm1, xmm2
		jb		t_z
		subss	xmm3, xmm0
		addss	xmm3, xmm1
		subss	xmm3, xmm2
		movaps	xmm2, xmm1
		movhlps	xmm2, xmm0
		shufps	xmm2, xmm2, 0xD0
		unpcklps	xmm0, xmm1
		movhlps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0xE0
		xorps	xmm2, xmm5
		addps	xmm1, xmm2
		movss	xmm1, xmm3
		shufps	xmm1, xmm1, 0x72
		jmp		done
	t_xz:
		comiss	xmm0, xmm2
		jb		t_z
		addss	xmm3, xmm0
		subss	xmm3, xmm1
		subss	xmm3, xmm2
		movaps	xmm2, xmm0
		unpcklps	xmm2, xmm1
		movhlps	xmm2, xmm0
		shufps	xmm2, xmm2, 0x2C
		movhlps	xmm1, xmm0
		shufps	xmm1, xmm1, 0x9C
		xorps	xmm1, xmm5
		addps	xmm1, xmm2
		movss	xmm1, xmm3
		pshufd	xmm1, xmm0, 0x78
		jmp		done
	t_z:
		subss	xmm3, xmm0
		subss	xmm3, xmm1
		addss	xmm3, xmm2
		movaps	xmm2, xmm0
		unpcklps	xmm2, xmm1
		movhlps	xmm2, xmm0
		shufps	xmm2, xmm2, 0xC8
		movhlps	xmm1, xmm0
		xorps	xmm1, xmm5
		addps	xmm1, xmm2
		movss	xmm1, xmm3
		pshufd	xmm1, xmm2, 0x4E
	done:
		andps	xmm3, kSSERemoveSignMaskPS0
		sqrtss	xmm0, xmm3
		addss	xmm0, xmm0
		divss	xmm4, xmm0
		shufps	xmm4, xmm4, 0
		mulps	xmm1, xmm4
		movaps	[ecx], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) hkQuaternion* __fastcall hkQuaternion::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		mulss	xmm0, kFltHalf
		movups	xmm7, [edx]
		call	GetSinCos
		shufps	xmm0, xmm0, 0x80
		mulps	xmm0, xmm7
		movaps	[ecx], xmm0
		movss	[ecx+0xC], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) void __fastcall hkQuaternion::operator*=(const hkQuaternion &rhs)
{
	__asm
	{
		pshufd	xmm0, [edx], 0x93
		pshufd	xmm1, [ecx], 0x93
		movss	xmm2, kSSEChangeSignMaskPS0
		movaps	xmm3, xmm1
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 1
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		movss	xmm5, xmm3
		pshufd	xmm3, xmm1, 0xB1
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 0x15
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		unpcklps	xmm5, xmm3
		pshufd	xmm3, xmm1, 0x4E
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 0x51
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		movss	xmm6, xmm3
		pshufd	xmm3, xmm1, 0x1B
		mulps	xmm3, xmm0
		pshufd	xmm4, xmm2, 0x45
		xorps	xmm3, xmm4
		haddps	xmm3, xmm3
		haddps	xmm3, xmm3
		unpcklps	xmm6, xmm3
		movlhps	xmm5, xmm6
		movaps	[ecx], xmm5
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
		comiss	xmm1, kEqEpsilon
		jb		zeroLen
		sqrtss	xmm1, xmm1
		movss	xmm2, kFltOne
		divss	xmm2, xmm1
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm0
	zeroLen:
		movaps	[eax], xmm2
        retn
    }
}

__declspec(naked) NiVector3* __fastcall hkQuaternion::ToEulerYPR(NiVector3 &ypr) const
{
	__asm
	{
		movaps	xmm7, [ecx]
		pshufd	xmm0, xmm7, 0x47
		pshufd	xmm1, xmm7, 0x48
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, kSSEChangeSignMaskPS0
		addss	xmm1, kFltOne
		call	ATan2
		movss	[edx], xmm0
		pshufd	xmm0, xmm7, 0xB
		pshufd	xmm1, xmm7, 1
		mulps	xmm0, xmm1
		hsubps	xmm0, xmm0
		addss	xmm0, xmm0
		call	ASin
		movss	[edx+4], xmm0
		pshufd	xmm0, xmm7, 0x93
		pshufd	xmm1, xmm7, 0x96
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, kSSEChangeSignMaskPS0
		addss	xmm1, kFltOne
		call	ATan2
		movss	[edx+8], xmm0
		mov		eax, edx
		retn
	}
}

void hkQuaternion::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\t%.6f\n\n", x, y, z, w);
}

__declspec(naked) void __fastcall hkMatrix3x4::operator=(const NiMatrix33 &inMatrix)
{
	__asm
	{
		movups	xmm0, [edx-4]
		psrldq	xmm0, 4
		movaps	[ecx], xmm0
		movups	xmm0, [edx+8]
		psrldq	xmm0, 4
		movaps	[ecx+0x10], xmm0
		movups	xmm0, [edx+0x14]
		psrldq	xmm0, 4
		movaps	[ecx+0x20], xmm0
		retn
	}
}

__declspec(naked) void __fastcall hkMatrix3x4::operator=(const hkQuaternion &inQuaternion)
{
	__asm
	{
		pshufd	xmm0, [edx], 0x93
		movaps	xmm3, xmm0
		addps	xmm3, xmm0
		pshufd	xmm1, xmm0, 0
		mulps	xmm1, xmm3
		pshufd	xmm2, xmm0, 0x78
		mulps	xmm2, xmm3
		mulps	xmm0, xmm3
		subss	xmm0, kFltOne
		pshufd	xmm3, xmm0, 1
		addss	xmm3, xmm0
		movss	[ecx], xmm3
		pshufd	xmm3, xmm0, 2
		addss	xmm3, xmm0
		movss	[ecx+0x14], xmm3
		pshufd	xmm3, xmm0, 3
		addss	xmm3, xmm0
		movss	[ecx+0x28], xmm3
		pshufd	xmm3, xmm2, 1
		movss	xmm4, xmm3
		pshufd	xmm5, xmm1, 3
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x10], xmm3
		movss	[ecx+4], xmm4
		pshufd	xmm3, xmm2, 3
		movss	xmm4, xmm3
		pshufd	xmm5, xmm1, 2
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x20], xmm4
		movss	[ecx+8], xmm3
		pshufd	xmm3, xmm2, 2
		movss	xmm4, xmm3
		pshufd	xmm5, xmm1, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x24], xmm3
		movss	[ecx+0x18], xmm4
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

__declspec(naked) void __vectorcall hkpRigidBody::SetAngularVelocity(UInt32 axis, float angVel)
{
	__asm
	{
		shufps	xmm0, xmm0, 0xC0
		shl		edx, 4
		mulps	xmm0, [ecx+edx+0xF0]
		movaps	[ecx+0x1C0], xmm0
		retn
	}
}