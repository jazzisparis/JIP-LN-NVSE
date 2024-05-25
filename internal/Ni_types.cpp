#include "internal/Ni_types.h"

void NiPoint2::Dump() const
{
	PrintDebug("%.6f\t%.6f\n", x, y);
}

void NiVector3::operator=(const NiMatrix33 &from)
{
	SetPS(from.ToEulerPRY());
}

void NiVector3::operator=(const NiQuaternion &from)
{
	SetPS(from.ToEulerPRY());
}

__declspec(naked) NiVector3& NiVector3::Normalize()
{
    __asm
    {
		mov		eax, ecx
		movups	xmm1, [eax]
		andps	xmm1, PS_XYZ0Mask
		movaps	xmm2, xmm1
		mulps	xmm2, xmm2
		xorps	xmm0, xmm0
		haddps	xmm2, xmm0
		haddps	xmm2, xmm0
		comiss	xmm2, PS_Epsilon
		jb		zeroLen
		rsqrtss	xmm3, xmm2
		movss	xmm0, SS_3
		mulss	xmm2, xmm3
		mulss	xmm2, xmm3
		subss	xmm0, xmm2
		mulss	xmm0, xmm3
		mulss	xmm0, PS_V3_Half
		shufps	xmm0, xmm0, 0xC0
		mulps	xmm0, xmm1
	zeroLen:
		pshufd	xmm2, xmm0, 2
        movlps	[eax], xmm0
        movss	[eax+8], xmm2
        retn
    }
}

__declspec(naked) __m128 __vectorcall NiVector3::CrossProduct(const NiVector3 &vB) const
{
	__asm
	{
		movups	xmm1, [edx]
		andps	xmm1, PS_XYZ0Mask
		movups	xmm2, [ecx]
		pshufd	xmm0, xmm1, 0xC9
		pshufd	xmm3, xmm2, 0x12
		mulps	xmm0, xmm3
		pshufd	xmm3, xmm1, 0xD2
		pshufd	xmm4, xmm2, 9
		mulps	xmm3, xmm4
		subps	xmm0, xmm3
		jmp		Normalize_V4
	}
}

__declspec(naked) __m128 __vectorcall NiVector3::Interpolate(const NiVector3 &vB, float t) const
{
	__asm
	{
		pshufd	xmm2, xmm0, 0x40
		movups	xmm0, [ecx]
		andps	xmm0, PS_XYZ0Mask
		movups	xmm1, [edx]
		subps	xmm1, xmm0
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiVector3::GetTranslatedPos(const NiTransform &transfrm) const
{
	__asm
	{
		movups	xmm0, [ecx]
		mov		ecx, edx
		call	NiMatrix33::MultiplyVector
		movups	xmm1, [ecx+0x24]
		addps	xmm0, xmm1
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiVector3::GetRotatedPos(__m128 pry, const NiVector3 &origin) const
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	edx
		sub		esp, 0x24
		mov		ecx, esp
		call	NiMatrix33::FromEulerYPR
		mov		ecx, [ebp-4]
		movups	xmm0, [ecx]
		mov		edx, [ebp-8]
		movups	xmm1, [edx]
		subps	xmm0, xmm1
		mov		ecx, eax
		call	NiMatrix33::MultiplyVector
		mov		eax, [ebp-8]
		movups	xmm1, [eax]
		addps	xmm0, xmm1
		andps	xmm0, PS_XYZ0Mask
		leave
		retn
	}
}

void NiVector3::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\n", x, y, z);
}

void NiVector4::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\t%.6f\n", x, y, z, w);
}

__declspec(naked) AxisAngle& __vectorcall AxisAngle::FromEulerPRY(__m128 pry)
{
	__asm
	{
		movaps	xmm1, PS_V3_PI
		cmpltps	xmm1, xmm0
		andps	xmm1, PS_V3_PIx2
		subps	xmm0, xmm1
		mulps	xmm0, PS_V3_Half
		call	GetSinCos_V3
		movaps	xmm4, xmm0
		unpcklps	xmm4, xmm1
		shufps	xmm4, xmm1, 0xF0
		pshufd	xmm5, xmm0, 0xF1
		unpcklpd	xmm5, xmm1
		shufps	xmm5, xmm5, 0x5C
		movaps	xmm6, xmm0
		unpckhpd	xmm6, xmm1
		shufps	xmm6, xmm6, 0x58		
		shufps	xmm1, xmm1, 0xC9
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		shufps	xmm0, xmm0, 0xC9
		shufps	xmm1, xmm1, 0xC9
		pshufd	xmm3, xmm2, 0xD2
		mulps	xmm2, xmm1
		mulps	xmm3, xmm0
		pshufd	xmm0, PS_FlipSignMask0, 0x44
		xorps	xmm3, xmm0
		addps	xmm2, xmm3
		movups	[ecx], xmm2
		movaps	xmm0, xmm4
		mulps	xmm0, xmm5
		mulps	xmm0, xmm6
		haddps	xmm0, xmm0
		call	ACos
		addss	xmm0, xmm0
		movss	[ecx+0xC], xmm0
		jmp		NiVector3::Normalize
	}
}

__declspec(naked) AxisAngle& __fastcall AxisAngle::FromRotationMatrix(const NiMatrix33 &rotMat)
{
	__asm
	{
		movq	xmm0, qword ptr [edx+0x14]
		movhps	xmm0, [edx+4]
		movss	xmm1, [edx+0x1C]
		movhps	xmm1, [edx+8]
		shufps	xmm1, xmm1, 0x38
		subps	xmm0, xmm1
		movups	[ecx], xmm0
		movss	xmm0, [edx]
		addss	xmm0, [edx+0x10]
		addss	xmm0, [edx+0x20]
		subss	xmm0, PS_V3_One
		mulss	xmm0, PS_V3_Half
		call	ACos
		movss	[ecx+0xC], xmm0
		jmp		NiVector3::Normalize
	}
}

__declspec(naked) AxisAngle& __fastcall AxisAngle::FromQuaternion(const NiQuaternion &qt)
{
	__asm
	{
		movups	xmm0, [edx]
		pshufd	xmm1, xmm0, 0x39
		movups	[ecx], xmm1
		call	ACos
		addss	xmm0, xmm0
		movss	[ecx+0xC], xmm0
		jmp		NiVector3::Normalize
	}
}

__declspec(naked) __m128 __vectorcall AxisAngle::ToEulerPRY() const
{
	__asm
	{
		mov		edx, ecx
		lea		ecx, [esp-0x14]
		call	NiQuaternion::FromAxisAngle
		mov		ecx, eax
		jmp		NiQuaternion::ToEulerPRY
	}
}

void AxisAngle::Dump() const
{
	PrintDebug("(%.6f, %.6f, %.6f), %.6f\n", axis.x, axis.y, axis.z, theta);
}

__declspec(naked) __m128 __vectorcall NiMatrix33::ToEulerPRY() const
{
	__asm
	{
		movq	xmm1, qword ptr [ecx]
		pshufd	xmm0, xmm1, 0xA9
		call	ATan2
		movq	xmm6, xmm0
		movss	xmm0, [ecx+8]
		xorps	xmm0, PS_FlipSignMask0
		call	ASin
		movq	xmm7, xmm0
		movups	xmm0, [ecx+0x14]
		pshufd	xmm1, xmm0, 3
		call	ATan2
		unpcklps	xmm0, xmm7
		unpcklpd	xmm0, xmm6
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiMatrix33::ToEulerPRYInv() const
{
	__asm
	{
		movups	xmm1, [ecx]
		pshufd	xmm0, xmm1, 3
		call	ATan2
		movq	xmm6, xmm0
		movss	xmm0, [ecx+0x18]
		xorps	xmm0, PS_FlipSignMask0
		call	ASin
		movq	xmm7, xmm0
		movq	xmm0, qword ptr [ecx+0x1C]
		pshufd	xmm1, xmm0, 0xA9
		call	ATan2
		unpcklps	xmm0, xmm7
		unpcklpd	xmm0, xmm6
		retn
	}
}

__declspec(naked) float __vectorcall NiMatrix33::ExtractPitch() const
{
	__asm
	{
		movss	xmm0, [ecx+0x1C]
		xorps	xmm0, PS_FlipSignMask0
		jmp		ASin
	}
}

__declspec(naked) float __vectorcall NiMatrix33::ExtractRoll() const
{
	__asm
	{
		movups	xmm0, [ecx+0x18]
		pshufd	xmm1, xmm0, 2
		jmp		ATan2
	}
}

__declspec(naked) float __vectorcall NiMatrix33::ExtractYaw() const
{
	__asm
	{
		movups	xmm0, [ecx+4]
		pshufd	xmm1, xmm0, 3
		jmp		ATan2
	}
}

__declspec(naked) NiMatrix33& __vectorcall NiMatrix33::FromEulerPRY(__m128 pry)
{
	__asm
	{
		andps	xmm0, PS_XYZ0Mask
		call	GetSinCos_V3
		pshufd	xmm2, xmm0, 0x55
		pshufd	xmm3, xmm1, 0x55
		pshufd	xmm4, xmm0, 0xA
		shufps	xmm4, xmm1, 0xA0
		shufps	xmm4, xmm4, 0x22
		shufps	xmm0, xmm1, 0
		shufps	xmm1, xmm0, 0
		pshufd	xmm5, xmm0, 8
		unpcklpd	xmm5, xmm4
		mulps	xmm3, xmm5
		mulps	xmm0, xmm2
		mulps	xmm0, xmm4
		shufps	xmm4, xmm4, 0x11
		mulps	xmm1, xmm4
		pshufd	xmm4, PS_FlipSignMask0, 0x14
		xorps	xmm1, xmm4
		addps	xmm0, xmm1
		xorps	xmm2, xmm4
		movhpd	[ecx], xmm3
		movss	[ecx+8], xmm2
		movlpd	[ecx+0xC], xmm0
		shufps	xmm0, xmm3, 0x4E
		shufps	xmm0, xmm0, 0xD2
		movups	[ecx+0x14], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33& __vectorcall NiMatrix33::FromEulerPRYInv(__m128 pry)
{
	__asm
	{
		andps	xmm0, PS_XYZ0Mask
		call	GetSinCos_V3
		pshufd	xmm2, xmm0, 0x55
		pshufd	xmm3, xmm1, 0x55
		pshufd	xmm4, xmm1, 0xA
		shufps	xmm4, xmm0, 0xA0
		unpcklps	xmm0, xmm1
		unpcklpd	xmm0, xmm0
		pshufd	xmm1, xmm0, 0x11
		pshufd	xmm5, xmm4, 8
		unpcklpd	xmm5, xmm0
		mulps	xmm3, xmm5
		mulps	xmm0, xmm2
		mulps	xmm0, xmm4
		shufps	xmm4, xmm4, 0xA
		mulps	xmm1, xmm4
		pshufd	xmm4, PS_FlipSignMask0, 0x14
		xorps	xmm1, xmm4
		addps	xmm0, xmm1
		xorps	xmm2, xmm4
		movhpd	[ecx+0x10], xmm0
		movss	[ecx+0x18], xmm2
		movhpd	[ecx+0x1C], xmm3
		unpcklpd	xmm0, xmm3
		shufps	xmm0, xmm0, 0xD2
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33& __vectorcall NiMatrix33::FromEulerYPR(__m128 pry)
{
	__asm
	{
		andps	xmm0, PS_XYZ0Mask
		call	GetSinCos_V3
		pshufd	xmm2, xmm0, 5
		shufps	xmm2, xmm1, 0x50
		shufps	xmm2, xmm2, 0x88
		pshufd	xmm3, xmm2, 0x11
		pshufd	xmm4, xmm0, 0xA
		shufps	xmm4, xmm1, 0xA0
		shufps	xmm0, xmm0, 0
		shufps	xmm1, xmm1, 0
		pshufd	xmm5, xmm4, 8
		unpcklpd	xmm5, xmm2
		mulps	xmm1, xmm5
		mulps	xmm2, xmm0
		mulps	xmm2, xmm4
		shufps	xmm4, xmm4, 0xA
		mulps	xmm3, xmm4
		pshufd	xmm4, PS_FlipSignMask0, 0x14
		xorps	xmm3, xmm4
		subps	xmm2, xmm3
		xorps	xmm0, xmm4
		pshufd	xmm4, xmm1, 3
		unpcklps	xmm0, xmm4
		movlps	[ecx+0x1C], xmm0
		movaps	xmm4, xmm2
		unpcklps	xmm4, xmm1
		movups	[ecx], xmm4
		shufps	xmm2, xmm1, 0x9E
		shufps	xmm2, xmm2, 0xD8
		movups	[ecx+0xC], xmm2
		mov		eax, ecx
		retn
	}
}

__declspec(naked) float __vectorcall NiMatrix33::From2Points(__m128 pt1, __m128 pt2)
{
	__asm
	{
		subps	xmm0, xmm1
		andps	xmm0, PS_XYZ0Mask
		xorps	xmm6, xmm6
		movq	xmm7, xmm6
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		movaps	xmm2, xmm1
		haddps	xmm2, xmm7
		haddps	xmm2, xmm7
		comiss	xmm2, xmm7
		jz		allZero
		pshufd	xmm3, xmm0, 0xFE
		xorps	xmm3, PS_FlipSignMask0
		unpcklpd	xmm1, xmm7
		haddps	xmm1, xmm7
		comiss	xmm1, xmm7
		jz		zZero
		unpcklpd	xmm2, xmm1
		movaps	xmm4, xmm2
		rsqrtps	xmm6, xmm2
		mulps	xmm2, xmm6
		mulps	xmm2, xmm6
		movss	xmm5, SS_3
		unpcklpd	xmm5, xmm5
		subps	xmm5, xmm2
		mulps	xmm5, xmm6
		mulps	xmm5, PS_V3_Half
		movaps	xmm2, xmm5
		mulps	xmm4, xmm2
		movaps	xmm6, xmm4
		movsldup	xmm2, xmm2
		unpckhpd	xmm4, xmm4
		unpcklps	xmm3, xmm4
		unpcklpd	xmm3, xmm0
		cmpeqps	xmm0, xmm7
		movmskps	eax, xmm0
		and		al, 3
		cmp		al, 2
		jz		getSC
		pshufd	xmm0, PS_FlipSignMask0, 0x15
		xorps	xmm3, xmm0
	getSC:
		mulps	xmm3, xmm2
		jmp		makeMat
	zZero:
		movaps	xmm6, xmm3
		pshufd	xmm0, PS_V3_One, 0x3C
		andps	xmm3, PS_FlipSignMask0
		xorps	xmm6, xmm3
		xorps	xmm3, xmm0
		jmp		makeMat
	allZero:
		pshufd	xmm3, PS_V3_One, 0x33
	makeMat:
		pshufd	xmm0, xmm3, 0xA
		pshufd	xmm1, xmm3, 0xF
		movss	[ecx], xmm1
		movss	[ecx+0xC], xmm0
		movss	[ecx+0x18], xmm7
		movlps	[ecx+0x1C], xmm3
		shufps	xmm3, xmm7, 1
		mulps	xmm0, xmm3
		movlps	[ecx+4], xmm0
		xor		byte ptr [ecx+7], 0x80
		mulps	xmm3, xmm1
		movlps	[ecx+0x10], xmm3
		xor		byte ptr [ecx+0x17], 0x80
		movq	xmm0, xmm6
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::FromQuaternion(const NiQuaternion &qt)
{
	__asm
	{
		movups	xmm0, [edx]
		movaps	xmm3, xmm0
		addps	xmm3, xmm0
		pshufd	xmm1, xmm0, 0
		mulps	xmm1, xmm3
		pshufd	xmm2, xmm0, 0x78
		mulps	xmm2, xmm3
		mulps	xmm0, xmm3
		subss	xmm0, PS_V3_One
		pshufd	xmm3, xmm0, 1
		addss	xmm3, xmm0
		movss	[ecx], xmm3
		pshufd	xmm6, xmm0, 2
		addss	xmm6, xmm0
		pshufd	xmm7, xmm0, 3
		addss	xmm7, xmm0
		pshufd	xmm3, xmm2, 1
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm1, 3
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		unpcklps	xmm3, xmm6
		pshufd	xmm0, xmm2, 3
		movaps	xmm6, xmm0
		pshufd	xmm5, xmm1, 2
		subss	xmm0, xmm5
		addss	xmm6, xmm5
		unpcklps	xmm4, xmm0
		unpcklpd	xmm4, xmm3
		movups	[ecx+4], xmm4
		pshufd	xmm3, xmm2, 2
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm1, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		unpcklps	xmm4, xmm6
		unpcklps	xmm3, xmm7
		unpcklpd	xmm4, xmm3
		movups	[ecx+0x14], xmm4
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		movups	xmm7, [edx]
		andps	xmm7, PS_XYZ0Mask
		call	GetSinCos
		pshufd	xmm2, xmm0, 0x80
		movq	xmm3, xmm1
		movss	xmm4, PS_V3_One
		subss	xmm4, xmm3
		shufps	xmm4, xmm4, 0x40
		mulps	xmm2, xmm7
		mulps	xmm4, xmm7
		pshufd	xmm1, xmm7, 0xC9
		mulps	xmm1, xmm4
		mulps	xmm7, xmm4
		movaps	xmm4, xmm7
		addss	xmm4, xmm3
		movss	[ecx], xmm4
		pshufd	xmm6, xmm7, 1
		addss	xmm6, xmm3
		shufps	xmm7, xmm7, 2
		addss	xmm7, xmm3
		movaps	xmm3, xmm1
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm2, 2
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		unpcklps	xmm3, xmm6
		pshufd	xmm0, xmm1, 2
		movaps	xmm6, xmm0
		pshufd	xmm5, xmm2, 1
		subss	xmm0, xmm5
		addss	xmm6, xmm5
		unpcklps	xmm4, xmm0
		unpcklpd	xmm4, xmm3
		movups	[ecx+4], xmm4
		pshufd	xmm3, xmm1, 1
		movaps	xmm4, xmm3
		subss	xmm3, xmm2
		addss	xmm4, xmm2
		unpcklps	xmm4, xmm6
		unpcklps	xmm3, xmm7
		unpcklpd	xmm4, xmm3
		movups	[ecx+0x14], xmm4
		mov		eax, ecx
		retn
	}
}

__declspec(naked) void __fastcall NiMatrix33::operator=(const hkMatrix3x4 &inMatrix)
{
	__asm
	{
		movaps	xmm0, [edx]
		movups	[ecx], xmm0
		movaps	xmm0, [edx+0x10]
		movups	[ecx+0xC], xmm0
		movaps	xmm0, [edx+0x20]
		pshufd	xmm1, xmm0, 2
		movlps	[ecx+0x18], xmm0
		movss	[ecx+0x20], xmm1
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiMatrix33::MultiplyVector(__m128 vec) const
{
	__asm
	{
		movaps	xmm1, xmm0
		andps	xmm1, PS_XYZ0Mask
		movups	xmm0, [ecx]
		mulps	xmm0, xmm1
		xorps	xmm3, xmm3
		haddps	xmm0, xmm3
		haddps	xmm0, xmm3
		movups	xmm2, [ecx+0xC]
		mulps	xmm2, xmm1
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		unpcklps	xmm0, xmm2
		movups	xmm2, [ecx+0x18]
		mulps	xmm2, xmm1
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		unpcklpd	xmm0, xmm2
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiMatrix33::MultiplyVectorInv(__m128 vec) const
{
	__asm
	{
		movaps	xmm3, xmm0
		andps	xmm3, PS_XYZ0Mask
		movups	xmm0, [ecx]
		pshufd	xmm1, xmm3, 0xC0
		mulps	xmm0, xmm1
		movups	xmm1, [ecx+0xC]
		pshufd	xmm2, xmm3, 0xD5
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		movups	xmm1, [ecx+0x18]
		shufps	xmm3, xmm3, 0xEA
		mulps	xmm1, xmm3
		addps	xmm0, xmm1
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiMatrix33::MultiplyVectorRow(__m128 vec, UInt32 whichRow) const
{
	__asm
	{
		movups	xmm1, [ecx+edx*4]
		shufps	xmm1, xmm1, 0xC
		movss	xmm2, [ecx+edx*4+0x18]
		unpcklpd	xmm1, xmm2
		mulps	xmm0, xmm1
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::MultiplyMatrices(const NiMatrix33 &matB)
{
	__asm
	{
		movups	xmm0, [edx]
		movups	xmm1, [edx+0xC]
		movups	xmm2, [edx+0x18]
		movups	xmm3, [ecx]
		movups	xmm4, [ecx+0x10]
		pshufd	xmm5, xmm3, 0
		mulps	xmm5, xmm0
		pshufd	xmm6, xmm3, 0x15
		mulps	xmm6, xmm1
		addps	xmm5, xmm6
		pshufd	xmm6, xmm3, 0x2A
		mulps	xmm6, xmm2
		addps	xmm5, xmm6
		movups	[ecx], xmm5
		pshufd	xmm5, xmm3, 0x3F
		mulps	xmm5, xmm0
		pshufd	xmm6, xmm4, 0
		mulps	xmm6, xmm1
		addps	xmm5, xmm6
		pshufd	xmm6, xmm4, 0x15
		mulps	xmm6, xmm2
		addps	xmm5, xmm6
		movups	[ecx+0xC], xmm5
		pshufd	xmm5, xmm4, 0x2A
		mulps	xmm5, xmm0
		pshufd	xmm6, xmm4, 0x3F
		mulps	xmm6, xmm1
		addps	xmm5, xmm6
		movss	xmm3, [ecx+0x20]
		shufps	xmm3, xmm3, 0x40
		mulps	xmm3, xmm2
		addps	xmm5, xmm3
		movlps	[ecx+0x18], xmm5
		unpckhpd	xmm5, xmm5
		movss	[ecx+0x20], xmm5
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::MultiplyMatricesInv(const NiMatrix33 &matB)
{
	__asm
	{
		movups	xmm0, [edx]
		movups	xmm1, [ecx]
		pshufd	xmm2, xmm0, 0
		mulps	xmm2, xmm1
		pshufd	xmm3, xmm0, 0x55
		mulps	xmm3, xmm1
		pshufd	xmm4, xmm0, 0xAA
		mulps	xmm4, xmm1
		movups	xmm0, [edx+0xC]
		movups	xmm1, [ecx+0xC]
		pshufd	xmm5, xmm0, 0
		mulps	xmm5, xmm1
		pshufd	xmm6, xmm0, 0x55
		mulps	xmm6, xmm1
		pshufd	xmm7, xmm0, 0xAA
		mulps	xmm7, xmm1
		addps	xmm2, xmm5
		addps	xmm3, xmm6
		addps	xmm4, xmm7
		movups	xmm0, [edx+0x18]
		movups	xmm1, [ecx+0x18]
		pshufd	xmm5, xmm0, 0
		mulps	xmm5, xmm1
		pshufd	xmm6, xmm0, 0x55
		mulps	xmm6, xmm1
		pshufd	xmm7, xmm0, 0xAA
		mulps	xmm7, xmm1
		addps	xmm2, xmm5
		addps	xmm3, xmm6
		addps	xmm4, xmm7
		movups	[ecx], xmm2
		movups	[ecx+0xC], xmm3
		movlps	[ecx+0x18], xmm4
		unpckhpd	xmm4, xmm4
		movss	[ecx+0x20], xmm4
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33& __vectorcall NiMatrix33::Rotate(__m128 rot)
{
	__asm
	{
		push	ecx
		lea		ecx, [esp-0x28]
		call	NiMatrix33::FromEulerPRYInv
		mov		edx, eax
		pop		ecx
		jmp		NiMatrix33::MultiplyMatrices
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::Transpose(NiMatrix33 &out)
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [ecx+0x10]
		pshufd	xmm2, xmm0, 9
		shufps	xmm2, xmm1, 0x94
		shufps	xmm0, xmm2, 0x3C
		shufps	xmm1, xmm2, 0x9C
		movups	[edx], xmm0
		movups	[edx+0x10], xmm1
		mov		eax, [ecx+0x20]
		mov		[edx+0x20], eax
		mov		eax, edx
		retn
	}
}

void NiMatrix33::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n", cr[0][0], cr[1][0], cr[2][0], cr[0][1], cr[1][1], cr[2][1], cr[0][2], cr[1][2], cr[2][2]);
}

__declspec(naked) NiQuaternion& __vectorcall NiQuaternion::FromEulerPRY(__m128 pry)
{
	__asm
	{
		movaps	xmm1, PS_V3_PI
		cmpltps	xmm1, xmm0
		andps	xmm1, PS_V3_PIx2
		subps	xmm0, xmm1
		mulps	xmm0, PS_V3_Half
		call	GetSinCos_V3
		movaps	xmm2, xmm0
		unpcklpd	xmm2, xmm1
		shufps	xmm2, xmm2, 0xD7
		movaps	xmm3, xmm1
		unpckhpd	xmm3, xmm0
		shufps	xmm3, xmm3, 0x88
		movaps	xmm4, xmm1
		unpcklpd	xmm4, xmm0
		pshufd	xmm0, xmm4, 0xA2
		shufps	xmm4, xmm4, 8
		movaps	xmm5, xmm3
		mulps	xmm5, xmm2
		pshufd	xmm3, xmm5, 0xE0
		mulps	xmm3, xmm4
		pshufd	xmm6, xmm5, 0xB5
		mulps	xmm0, xmm6
		xorps	xmm0, PD_FlipSignMask
		addps	xmm0, xmm3
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiQuaternion& __vectorcall NiQuaternion::FromEulerYPR(__m128 pry)
{
	__asm
	{
		movaps	xmm1, PS_V3_PI
		cmpltps	xmm1, xmm0
		andps	xmm1, PS_V3_PIx2
		subps	xmm0, xmm1
		mulps	xmm0, PS_V3_Half
		call	GetSinCos_V3
		pshufd	xmm2, xmm0, 0x55
		pshufd	xmm3, xmm1, 0x55
		unpcklpd	xmm2, xmm3
		unpcklpd	xmm3, xmm2
		shufps	xmm2, xmm2, 0x20
		shufps	xmm3, xmm3, 0x20
		pshufd	xmm4, xmm0, 0xAA
		pshufd	xmm5, xmm1, 0xAA
		unpcklpd	xmm4, xmm5
		unpcklpd	xmm5, xmm4
		shufps	xmm4, xmm4, 2
		shufps	xmm5, xmm5, 2
		unpcklpd	xmm0, xmm1
		unpcklpd	xmm1, xmm0
		shufps	xmm0, xmm0, 0x2A
		shufps	xmm1, xmm1, 0x2A
		mulps	xmm0, xmm3
		mulps	xmm0, xmm5
		mulps	xmm1, xmm2
		mulps	xmm1, xmm4
		addsubps	xmm0, xmm1
		shufps	xmm0, xmm0, 0x2D
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiQuaternion& __fastcall NiQuaternion::FromRotationMatrix(const NiMatrix33 &rotMat)
{
	__asm
	{
		movups	xmm1, [edx]
		movups	xmm0, [edx+0x10]
		movss	xmm2, [edx+0x20]
		movaps	xmm3, xmm1
		addss	xmm3, xmm0
		addss	xmm3, xmm2
		xorps	xmm4, xmm4
		comiss	xmm3, xmm4
		jbe		t_xyz
		addss	xmm3, PS_V3_One
		movaps	xmm2, xmm0
		unpckhpd	xmm2, xmm1
		unpcklps	xmm1, xmm0
		unpckhpd	xmm1, xmm0
		shufps	xmm1, xmm1, 0x4E
		pshufd	xmm0, xmm1, 0x8C
		subps	xmm0, xmm2
		movaps	xmm2, xmm0
		unpcklps	xmm0, xmm3
		shufps	xmm0, xmm2, 0xE9
		jmp		done
	t_xyz:
		movss	xmm3, PS_V3_One
		pshufd	xmm4, PS_FlipSignMask0, 0x51
		comiss	xmm1, xmm0
		ja		t_xz
		comiss	xmm0, xmm2
		jbe		t_z
		subss	xmm3, xmm1
		addss	xmm3, xmm0
		subss	xmm3, xmm2
		movaps	xmm2, xmm1
		unpckhps	xmm2, xmm0
		shufps	xmm2, xmm2, 0xE0
		unpcklps	xmm1, xmm0
		shufps	xmm0, xmm1, 0xEA
		xorps	xmm2, xmm4
		addps	xmm0, xmm2
		shufps	xmm3, xmm0, 0x30
		shufps	xmm0, xmm3, 0x89
		jmp		done
	t_xz:
		comiss	xmm1, xmm2
		jbe		t_z
		addss	xmm3, xmm1
		subss	xmm3, xmm0
		subss	xmm3, xmm2
		movaps	xmm2, xmm1
		unpcklps	xmm2, xmm0
		unpckhpd	xmm2, xmm1
		shufps	xmm2, xmm2, 0x86
		unpckhpd	xmm0, xmm1
		shufps	xmm0, xmm0, 0x36
		xorps	xmm0, xmm4
		addps	xmm0, xmm2
		movaps	xmm2, xmm0
		unpcklps	xmm0, xmm3
		shufps	xmm0, xmm2, 0xE6
		jmp		done
	t_z:
		subss	xmm3, xmm1
		subss	xmm3, xmm0
		addss	xmm3, xmm2
		movaps	xmm2, xmm1
		unpcklps	xmm2, xmm0
		unpckhpd	xmm2, xmm1
		shufps	xmm2, xmm2, 0x62
		unpckhpd	xmm0, xmm1
		shufps	xmm0, xmm0, 0x4E
		xorps	xmm0, xmm4
		addps	xmm0, xmm2
		shufps	xmm3, xmm0, 0x30
		shufps	xmm0, xmm3, 0x29
	done:
		andps	xmm3, PS_AbsMask0
		rsqrtss	xmm1, xmm3
		mulss	xmm3, xmm1
		mulss	xmm3, xmm1
		movss	xmm2, SS_3
		subss	xmm2, xmm3
		mulss	xmm2, xmm1
		mulss	xmm2, SS_1d4
		shufps	xmm2, xmm2, 0
		mulps	xmm0, xmm2
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) float __vectorcall NiQuaternion::From2Points(__m128 pt1, __m128 pt2)
{
	__asm
	{
		mov		edx, ecx
		lea		ecx, [esp-0x28]
		call	NiMatrix33::From2Points
		xchg	ecx, edx
		call	NiQuaternion::FromRotationMatrix
		movq	xmm0, xmm6
		retn
	}
}

__declspec(naked) NiQuaternion& __fastcall NiQuaternion::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		mulss	xmm0, PS_V3_Half
		movups	xmm5, [edx]
		call	GetSinCos
		shufps	xmm0, xmm0, 2
		shufps	xmm5, xmm5, 0x90
		mulps	xmm0, xmm5
		movaps	xmm2, xmm0
		shufps	xmm0, xmm1, 1
		shufps	xmm0, xmm2, 0xE2
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

void NiQuaternion::operator=(const hkQuaternion &hkQt)
{
	__m128 m = hkQt.PS();
	SetPS(_mm_shuffle_ps(m, m, 0x93));
}

__declspec(naked) __m128 __vectorcall NiQuaternion::MultiplyVector(__m128 vec) const
{
	__asm
	{
		andps	xmm0, PS_XYZ0Mask
		movups	xmm1, [ecx]
		pshufd	xmm2, xmm1, 0
		psrldq	xmm1, 4
		movaps	xmm3, xmm1
		pshufd	xmm4, xmm0, 0xC9
		pshufd	xmm5, xmm3, 0x12
		mulps	xmm4, xmm5
		pshufd	xmm5, xmm0, 0xD2
		pshufd	xmm6, xmm3, 9
		mulps	xmm5, xmm6
		subps	xmm4, xmm5
		addps	xmm4, xmm4
		mulps	xmm2, xmm4
		addps	xmm2, xmm0
		pshufd	xmm0, xmm4, 0xC9
		pshufd	xmm5, xmm1, 0x12
		mulps	xmm0, xmm5
		pshufd	xmm5, xmm4, 0xD2
		pshufd	xmm6, xmm1, 9
		mulps	xmm5, xmm6
		subps	xmm0, xmm5
		addps	xmm0, xmm2
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiQuaternion::MultiplyQuaternion(const NiQuaternion &rhs) const
{
	__asm
	{
		movups	xmm1, [ecx]
		movups	xmm2, [edx]
		xorps	xmm6, xmm6
		movaps	xmm3, xmm1
		mulps	xmm3, xmm2
		pshufd	xmm5, PS_FlipSignMask0, 1
		xorps	xmm3, xmm5
		haddps	xmm3, xmm6
		haddps	xmm3, xmm6
		movq	xmm0, xmm3
		pshufd	xmm3, xmm1, 0xB1
		mulps	xmm3, xmm2
		pslldq	xmm5, 8
		xorps	xmm3, xmm5
		haddps	xmm3, xmm6
		haddps	xmm3, xmm6
		unpcklps	xmm0, xmm3
		pshufd	xmm3, xmm1, 0x4E
		mulps	xmm3, xmm2
		psrldq	xmm5, 8
		xorps	xmm3, xmm5
		haddps	xmm3, xmm6
		haddps	xmm3, xmm6
		movq	xmm4, xmm3
		pshufd	xmm3, xmm1, 0x1B
		mulps	xmm3, xmm2
		pslldq	xmm5, 4
		xorps	xmm3, xmm5
		haddps	xmm3, xmm6
		haddps	xmm3, xmm6
		unpcklps	xmm4, xmm3
		unpcklpd	xmm0, xmm4
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiQuaternion::ToEulerPRY() const
{
	__asm
	{
		movups	xmm6, [ecx]
		pshufd	xmm0, xmm6, 0x98
		pshufd	xmm1, xmm6, 0x9D
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, PS_FlipSignMask0
		addss	xmm1, PS_V3_One
		call	ATan2
		movq	xmm7, xmm0
		pshufd	xmm0, xmm6, 0xC
		pshufd	xmm1, xmm6, 6
		mulps	xmm0, xmm1
		hsubps	xmm0, xmm0
		addss	xmm0, xmm0
		call	ASin
		unpcklps	xmm7, xmm0
		pshufd	xmm0, xmm6, 0xE4
		pshufd	xmm1, xmm6, 0xEB
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, PS_FlipSignMask0
		addss	xmm1, PS_V3_One
		call	ATan2
		unpcklpd	xmm7, xmm0
		movaps	xmm0, xmm7
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiQuaternion::ToEulerYPR() const
{
	__asm
	{
		movups	xmm6, [ecx]
		pshufd	xmm0, xmm6, 8
		pshufd	xmm1, xmm6, 0xD
		mulps	xmm0, xmm1
		hsubps	xmm0, xmm0
		addss	xmm0, xmm0
		call	ASin
		movq	xmm7, xmm0
		pshufd	xmm0, xmm6, 0x94
		pshufd	xmm1, xmm6, 0x9E
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, PS_FlipSignMask0
		addss	xmm1, PS_V3_One
		call	ATan2
		unpcklps	xmm7, xmm0
		pshufd	xmm0, xmm6, 0xD4
		pshufd	xmm1, xmm6, 0xDB
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, PS_FlipSignMask0
		addss	xmm1, PS_V3_One
		call	ATan2
		unpcklpd	xmm7, xmm0
		movaps	xmm0, xmm7
		retn
	}
}

__declspec(naked) NiQuaternion& __vectorcall NiQuaternion::Rotate(__m128 pry)
{
	__asm
	{
		push	ecx
		lea		ecx, [esp-0x14]
		call	NiQuaternion::FromEulerYPR
		mov		edx, eax
		pop		ecx
		jmp		NiQuaternion::MultiplyQuaternion
	}
}

__declspec(naked) __m128 __vectorcall NiQuaternion::Lerp(const NiQuaternion &qb, float t) const
{
	__asm
	{
		xorps	xmm4, xmm4
		comiss	xmm0, xmm4
		jbe		done
		movups	xmm2, [edx]
		comiss	xmm0, PS_V3_One
		jnb		outRange
		movups	xmm1, [ecx]
		movaps	xmm3, xmm1
		mulps	xmm3, xmm2
		haddps	xmm3, xmm4
		haddps	xmm3, xmm4
		comiss	xmm3, xmm4
		jnb		proceed
		xorps	xmm1, PS_FlipSignMask
	proceed:
		subps	xmm2, xmm1
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm2
		addps	xmm0, xmm1
		jmp		Normalize_V4
	outRange:
		movaps	xmm0, xmm2
	done:
		retn
	}
}

__declspec(naked) __m128 __vectorcall NiQuaternion::Slerp(const NiQuaternion &qb, float t) const
{
	__asm
	{
		xorps	xmm7, xmm7
		comiss	xmm0, xmm7
		jbe		done
		movss	xmm6, PS_V3_One
		movups	xmm2, [edx]
		comiss	xmm0, xmm6
		jnb		outRange
		movups	xmm1, [ecx]
		movaps	xmm4, xmm1
		mulps	xmm4, xmm2
		haddps	xmm4, xmm7
		haddps	xmm4, xmm7
		movq	xmm3, xmm4
		andps	xmm3, PS_AbsMask0
		comiss	xmm3, xmm6
		jnb		done
		comiss	xmm3, xmm7
		jnb		proceed
		xorps	xmm1, PS_FlipSignMask
	proceed:
		movq	xmm5, xmm4
		mulss	xmm5, xmm4
		subss	xmm6, xmm5
		comiss	xmm6, SS_1d1K
		jb		case0
		sub		esp, 8
		unpcklps	xmm0, xmm6
		movlps	[esp], xmm0
		movaps	xmm7, xmm1
		movaps	xmm6, xmm2
		movq	xmm5, xmm4
		movq	xmm0, xmm4
		call	ACos
		movq	xmm4, xmm0
		movss	xmm0, [esp]
		subss	xmm0, PS_V3_One
		mulss	xmm0, xmm4
		addss	xmm0, PS_V3_PId2
		call	Cos
		shufps	xmm0, xmm0, 0
		mulps	xmm7, xmm0
		movss	xmm0, PS_V3_PId2
		mulss	xmm4, [esp]
		subss	xmm0, xmm4
		call	Cos
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm6
		addps	xmm0, xmm7
		movss	xmm1, [esp+4]
		movq	xmm2, xmm1
		rsqrtss	xmm3, xmm2
		mulss	xmm2, xmm3
		mulss	xmm2, xmm3
		movss	xmm4, SS_3
		subss	xmm4, xmm2
		mulss	xmm4, xmm3
		mulss	xmm4, PS_V3_Half
		shufps	xmm4, xmm4, 0
		mulps	xmm0, xmm4
		add		esp, 8
		retn
	case0:
		subps	xmm2, xmm1
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm2
		addps	xmm0, xmm1
		jmp		Normalize_V4
	outRange:
		movaps	xmm0, xmm2
	done:
		retn
	}
}

void NiQuaternion::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\t%.6f\n", x, y, z, w);
}

__declspec(naked) NiTransform& __fastcall NiTransform::RotateOrigin(const NiTransform &origin)
{
	__asm
	{
		movups	xmm0, [ecx+0x24]
		movups	xmm7, [edx+0x24]
		subps	xmm0, xmm7
		xchg	ecx, edx
		call	NiMatrix33::MultiplyVector
		xchg	ecx, edx
		addps	xmm0, xmm7
		movlps	[ecx+0x24], xmm0
		unpckhps	xmm0, xmm0
		movss	[ecx+0x2C], xmm0
		jmp		NiMatrix33::MultiplyMatrices
	}
}

__declspec(naked) NiTransform& __fastcall NiTransform::GetInverse(NiTransform &out)
{
    __asm
    {
		call	NiMatrix33::Transpose
		movups  xmm0, [ecx+0x24]
		xorps   xmm0, PS_FlipSignMask
		andps   xmm0, PS_XYZ0Mask
		xchg	ecx, edx
		call	NiMatrix33::MultiplyVector
		movss	xmm1, PS_V3_One
		movss	xmm2, [edx+0x30]
		comiss	xmm1, xmm2
		jz		done
		divss	xmm1, xmm2
		shufps  xmm1, xmm1, 0x40
        mulps   xmm0, xmm1
	done:
        movups  [ecx+0x24], xmm0
        movss   [ecx+0x30], xmm1
		mov		eax, ecx
		retn
	}
}

void NiTransform::Dump() const
{
	//rotate.Dump();
	NiVector4 angles(rotate.ToEulerPRY() * GET_PS(9));
	PrintDebug("T (%.4f, %.4f, %.4f) R (%.4f, %.4f, %.4f) S %.4f", translate.x, translate.y, translate.z, angles.x, angles.y, angles.z, scale);
}

__declspec(naked) UInt8 __fastcall NiPlane::CalculateSide(const NiVector3 &point) const
{
	__asm
	{
		movups	xmm0, [ecx]
		andps	xmm0, PS_XYZ0Mask
		movups	xmm1, [edx]
		mulps	xmm0, xmm1
		xorps	xmm1, xmm1
		haddps	xmm0, xmm1
		haddps	xmm0, xmm1
		subss	xmm0, [ecx+0xC]
		pshufd	xmm1, xmm0, 0x51
		cmpnleps	xmm0, xmm1
		movmskps	eax, xmm0
		and		al, 3
		retn
	}
}

__declspec(naked) UInt8 __fastcall NiBound::CalculateSide(const NiPlane &plane) const
{
	__asm
	{
		movups	xmm0, [ecx]
		andps	xmm0, PS_XYZ0Mask
		movups	xmm1, [edx]
		mulps	xmm0, xmm1
		xorps	xmm1, xmm1
		haddps	xmm0, xmm1
		haddps	xmm0, xmm1
		subss	xmm0, [edx+0xC]
		movss	xmm1, [ecx+0xC]
		unpcklps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0xA1
		pshufd	xmm2, PS_FlipSignMask0, 0x51
		xorps	xmm0, xmm2
		cmpnltps	xmm0, xmm1
		movmskps	eax, xmm0
		and		al, 3
		retn
	}
}

__declspec(naked) void __vectorcall NiViewport::SetFOV(float fov)
{
	__asm
	{
		call	Tan
		mulss	xmm0, kFlt2d3
		movsldup	xmm0, xmm0
		xorps	xmm0, PS_FlipSignMask0
		shufps	xmm0, xmm0, 0x14
		movups	[ecx], xmm0
		retn
		ALIGN 4
	kFlt2d3:
		EMIT_DW(0x3F2AAAAB)
	}
}

void NiFrustumPlanes::Set(NiCamera *camera)
{
	ThisCall(0xA74E10, this, &camera->frustum, &camera->m_transformWorld);
}

void NiTriangle::Dump() const
{
	PrintDebug("%d\t%d\t%d\n", point1, point2, point3);
}

void NiColor::Dump() const
{
	PrintDebug("R %.0f, G %.0f, B %.0f\n", r * 255.0, g * 255.0, b * 255.0);
	Console_Print("R %.0f, G %.0f, B %.0f\n", r * 255.0, g * 255.0, b * 255.0);
}

__declspec(naked) float __vectorcall Point2Distance(const NiVector3 &pt1, const NiVector3 &pt2)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		movq	xmm1, qword ptr [edx]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		xorps	xmm1, xmm1
		haddps	xmm0, xmm1
		comiss	xmm0, xmm1
		jz		done
		movq	xmm1, xmm0
		rsqrtss	xmm2, xmm0
		mulss	xmm1, xmm2
		mulss	xmm1, xmm2
		movss	xmm3, SS_3
		subss	xmm3, xmm1
		mulss	xmm3, xmm2
		mulss	xmm3, PS_V3_Half
		mulss	xmm0, xmm3
	done:
		retn
	}
}

__declspec(naked) float __vectorcall Point3Distance(const NiVector3 &pt1, const NiVector3 &pt2)
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [edx]
		subps	xmm0, xmm1
		andps	xmm0, PS_XYZ0Mask
		mulps	xmm0, xmm0
		xorps	xmm1, xmm1
		haddps	xmm0, xmm1
		haddps	xmm0, xmm1
		comiss	xmm0, xmm1
		jz		done
		movq	xmm1, xmm0
		rsqrtss	xmm2, xmm0
		mulss	xmm1, xmm2
		mulss	xmm1, xmm2
		movss	xmm3, SS_3
		subss	xmm3, xmm1
		mulss	xmm3, xmm2
		mulss	xmm3, PS_V3_Half
		mulss	xmm0, xmm3
	done:
		retn
	}
}

__declspec(naked) __m128 __vectorcall GetAnglesBetweenPoints(__m128 pt1, __m128 pt2)
{
	__asm
	{
		subps	xmm0, xmm1
		andps	xmm0, PS_XYZ0Mask
		pshufd	xmm1, xmm0, 0xFD
		xorps	xmm1, PS_FlipSignMask0
		movaps	xmm2, xmm0
		xorps	xmm6, xmm6
		unpcklpd	xmm2, xmm6
		mulps	xmm2, xmm2
		haddps	xmm2, xmm6
		sqrtss	xmm6, xmm2
		pshufd	xmm7, xmm0, 0xFE
		call	ATan2
		movq	xmm1, xmm6
		movq	xmm6, xmm0
		movq	xmm0, xmm7
		call	ATan2
		xorps	xmm0, PS_FlipSignMask0
		unpcklpd	xmm0, xmm6
		retn
	}
}

__declspec(naked) __m128 __vectorcall TransformWorldToLocal(const NiVector3 &origin, const NiVector3 &target, __m128 rotation)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x24
		push	ecx
		push	edx
		lea		ecx, [ebp-0x24]
		call	NiMatrix33::FromEulerPRYInv
		pop		edx
		pop		ecx
		movups	xmm4, [edx]
		movups	xmm1, [ecx]
		subps	xmm4, xmm1
		andps	xmm4, PS_XYZ0Mask
		xorps	xmm5, xmm5
		movups	xmm0, [eax]
		mulps	xmm0, xmm4
		haddps	xmm0, xmm5
		haddps	xmm0, xmm5
		movups	xmm1, [eax+0xC]
		mulps	xmm1, xmm4
		haddps	xmm1, xmm5
		haddps	xmm1, xmm5
		movups	xmm2, [eax+0x18]
		mulps	xmm2, xmm4
		haddps	xmm2, xmm5
		haddps	xmm2, xmm5
		movq	xmm7, xmm0
		unpcklps	xmm7, xmm1
		unpcklpd	xmm7, xmm5
		mulps	xmm7, xmm7
		haddps	xmm7, xmm5
		sqrtss	xmm7, xmm7
		unpcklpd	xmm7, xmm2
		call	ATan2
		movq	xmm6, xmm0
		pshufd	xmm0, xmm7, 0xFE
		pshufd	xmm1, xmm7, 0xFC
		call	ATan2
		xorps	xmm0, PS_FlipSignMask0
		unpcklpd	xmm0, xmm6
		leave
		retn
	}
}