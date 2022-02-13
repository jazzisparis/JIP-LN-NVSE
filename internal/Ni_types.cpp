#include "internal/Ni_types.h"

__declspec(naked) float __vectorcall NiVector3::Length() const
{
	__asm
	{
		movups	xmm0, [ecx-4]
		psrldq	xmm0, 4
		xorps	xmm1, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm1
		haddps	xmm0, xmm1
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) NiVector3& __fastcall NiVector3::MultiplyMatrix(const NiMatrix33 &mat)
{
	__asm
	{
		movups	xmm1, [ecx-4]
		psrldq	xmm1, 4
		xorps	xmm3, xmm3
		movups	xmm0, [edx]
		mulps	xmm0, xmm1
		haddps	xmm0, xmm3
		haddps	xmm0, xmm3
		movups	xmm2, [edx+0xC]
		mulps	xmm2, xmm1
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		unpcklps	xmm0, xmm2
		movq	qword ptr [ecx], xmm0
		movups	xmm2, [edx+0x18]
		mulps	xmm2, xmm1
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		movss	[ecx+8], xmm2
		shufps	xmm0, xmm2, 0x44
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3& __fastcall NiVector3::MultiplyMatrixInv(const NiMatrix33 &mat)
{
	__asm
	{
		movups	xmm3, [ecx-4]
		psrldq	xmm3, 4
		movups	xmm0, [edx]
		pshufd	xmm1, xmm3, 0xC0
		mulps	xmm0, xmm1
		movups	xmm1, [edx+0xC]
		pshufd	xmm2, xmm3, 0xD5
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		movups	xmm1, [edx+0x18]
		shufps	xmm3, xmm3, 0xEA
		mulps	xmm1, xmm3
		addps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0xFE
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3& __fastcall NiVector3::MultiplyMatrixRow(const NiMatrix33 &rotMatrix, UInt32 whichRow)
{
	__asm
	{
		mov		eax, [esp+4]
		movups	xmm0, [edx+eax*4]
		movss	xmm1, [edx+eax*4+0x18]
		shufps	xmm0, xmm0, 0xC
		shufps	xmm0, xmm1, 0x44
		movups	xmm1, [ecx]
		mulps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0xFE
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		mov		eax, ecx
		retn	4
	}
}

__declspec(naked) NiVector3& __fastcall NiVector3::MultiplyQuaternion(const NiQuaternion &qt)
{
	__asm
	{
		movups	xmm0, [ecx-4]
		psrldq	xmm0, 4
		movups	xmm1, [edx]
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
		pshufd	xmm1, xmm0, 0xFE
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3& NiVector3::Normalize()
{
    __asm
    {
		mov		eax, ecx
		movups	xmm1, [eax-4]
		psrldq	xmm1, 4
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
		pshufd	xmm2, xmm0, 0xFE
        movq	qword ptr [eax], xmm0
        movss	[eax+8], xmm2
        retn
    }
}

__declspec(naked) NiVector3& __fastcall NiVector3::CrossProduct(const NiVector3 &vB)
{
	__asm
	{
		movups	xmm1, [edx-4]
		psrldq	xmm1, 4
		movups	xmm2, [ecx]
		pshufd	xmm0, xmm1, 0xC9
		pshufd	xmm3, xmm2, 0x12
		mulps	xmm0, xmm3
		pshufd	xmm3, xmm1, 0xD2
		pshufd	xmm4, xmm2, 9
		mulps	xmm3, xmm4
		subps	xmm0, xmm3
		pshufd	xmm1, xmm0, 0xFE
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3& __fastcall NiVector3::Interpolate(const NiVector3 &vB, float t, const NiVector3 &res)
{
	__asm
	{
		movups	xmm0, [ecx-4]
		psrldq	xmm0, 4
		movups	xmm1, [edx-4]
		psrldq	xmm1, 4
		subps	xmm1, xmm0
		movss	xmm2, [esp+4]
		shufps	xmm2, xmm2, 0xC0
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0xFE
		mov		eax, [esp+8]
		movq	qword ptr [eax], xmm0
		movss	[eax+8], xmm1
		retn	8
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

void AlignedVector4::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\t%.6f\n", x, y, z, w);
}

__declspec(naked) AxisAngle& __fastcall AxisAngle::FromEulerYPR(const NiVector3 &ypr)
{
	__asm
	{
		mov		eax, offset PS_V3_PI
		movups	xmm0, [edx]
		movaps	xmm1, [eax]
		cmpltps	xmm1, xmm0
		andps	xmm1, [eax+0x10]
		subps	xmm0, xmm1
		mulps	xmm0, [eax+0x20]
		mov		eax, ecx
		call	GetSinCosV3
		mov		ecx, eax
		movaps	xmm4, xmm0
		unpcklps	xmm4, xmm1
		shufps	xmm4, xmm1, 0xF0
		pshufd	xmm5, xmm0, 0xF1
		shufps	xmm5, xmm1, 0x44
		shufps	xmm5, xmm5, 0x5C
		movaps	xmm6, xmm1
		pshufd	xmm6, xmm0, 0xFE
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

NiVector3& AxisAngle::ToEulerYPR(NiVector3 &ypr) const
{
	NiQuaternion qt = *this;
	return qt.ToEulerYPR(ypr);
}

void AxisAngle::Dump() const
{
	PrintDebug("(%.6f, %.6f, %.6f), %.6f\n", axis.x, axis.y, axis.z, theta);
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
		shufps	xmm4, xmm3, 0x44
		movups	[ecx+4], xmm4
		pshufd	xmm3, xmm2, 2
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm1, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		unpcklps	xmm4, xmm6
		unpcklps	xmm3, xmm7
		shufps	xmm4, xmm3, 0x44
		movups	[ecx+0x14], xmm4
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3& __fastcall NiMatrix33::ExtractAngles(NiVector3 &outAngles) const
{
	__asm
	{
		movss	xmm0, [ecx+8]
		xorps	xmm0, PS_FlipSignMask0
		call	ASin
		movaps	xmm7, xmm0
		movups	xmm0, [ecx+0x14]
		pshufd	xmm1, xmm0, 3
		call	ATan2
		unpcklps	xmm0, xmm7
		movq	qword ptr [edx], xmm0
		movq	xmm1, qword ptr [ecx]
		pshufd	xmm0, xmm1, 0xA9
		call	ATan2
		movss	[edx+8], xmm0
		mov		eax, edx
		retn
	}
}

__declspec(naked) NiVector3& __fastcall NiMatrix33::ExtractAnglesInv(NiVector3 &outAngles) const
{
	__asm
	{
		movss	xmm0, [ecx+0x18]
		xorps	xmm0, PS_FlipSignMask0
		call	ASin
		movaps	xmm7, xmm0
		movq	xmm0, qword ptr [ecx+0x1C]
		pshufd	xmm1, xmm0, 0xA9
		call	ATan2
		unpcklps	xmm0, xmm7
		movq	qword ptr [edx], xmm0
		movups	xmm1, [ecx]
		pshufd	xmm0, xmm1, 3
		call	ATan2
		movss	[edx+8], xmm0
		mov		eax, edx
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::RotationMatrix(const NiVector3 &rot)
{
	__asm
	{
		mov		eax, ecx
		movups	xmm0, [edx-4]
		psrldq	xmm0, 4
		call	GetSinCosV3
		pshufd	xmm4, xmm0, 0xFE
		pshufd	xmm5, xmm1, 0xFE
		unpcklps	xmm5, xmm4
		movshdup	xmm2, xmm0
		movshdup	xmm3, xmm1
		movaps	xmm6, xmm3
		mulps	xmm6, xmm5
		shufps	xmm6, xmm2, 0x44
		movups	[eax], xmm6
		xor		byte ptr [eax+0xB], 0x80
		movaps	xmm6, xmm0
		mulss	xmm6, xmm4
		movaps	xmm7, xmm1
		mulss	xmm7, xmm5
		shufps	xmm1, xmm7, 0x44
		mulss	xmm7, xmm2
		addss	xmm7, xmm6
		movaps	xmm6, xmm0
		mulss	xmm6, xmm3
		unpcklps	xmm6, xmm7
		shufps	xmm6, xmm1, 0xE4
		shufps	xmm6, xmm6, 0x4E
		movaps	xmm7, xmm2
		mulss	xmm7, xmm4
		shufps	xmm2, xmm7, 0x44
		mulss	xmm7, xmm0
		addss	xmm7, xmm6
		mulss	xmm0, xmm5
		mulss	xmm4, xmm1
		movaps	xmm5, xmm2
		mulss	xmm5, xmm0
		subss	xmm5, xmm4
		unpcklps	xmm5, xmm7
		shufps	xmm5, xmm6, 0xE4
		movups	[eax+0xC], xmm5
		mulss	xmm3, xmm1
		unpckhps	xmm2, xmm2
		mulss	xmm1, xmm2
		subss	xmm1, xmm0
		unpcklps	xmm1, xmm3
		movq	qword ptr [eax+0x1C], xmm1
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::RotationMatrixInv(const NiVector3 &rot)
{
	__asm
	{
		call	NiMatrix33::RotationMatrix
		movups	xmm0, [eax]
		movups	xmm1, [eax+0x10]
		pshufd	xmm2, xmm0, 9
		shufps	xmm2, xmm1, 0x94
		shufps	xmm0, xmm2, 0x3C
		shufps	xmm1, xmm2, 0x9C
		movups	[eax], xmm0
		movups	[eax+0x10], xmm1
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		movups	xmm7, [edx-4]
		psrldq	xmm7, 4
		mov		eax, ecx
		call	GetSinCos
		pshufd	xmm2, xmm0, 0x80
		movaps	xmm3, xmm1
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
		movss	[eax], xmm4
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
		shufps	xmm4, xmm3, 0x44
		movups	[eax+4], xmm4
		pshufd	xmm3, xmm1, 1
		movaps	xmm4, xmm3
		subss	xmm3, xmm2
		addss	xmm4, xmm2
		unpcklps	xmm4, xmm6
		unpcklps	xmm3, xmm7
		shufps	xmm4, xmm3, 0x44
		movups	[eax+0x14], xmm4
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
		pshufd	xmm1, xmm0, 0xFE
		movq	qword ptr [ecx+0x18], xmm0
		movss	[ecx+0x20], xmm1
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::MultiplyMatrices(const NiMatrix33 &matB)
{
	__asm
	{
		movups	xmm0, [edx-4]
		psrldq	xmm0, 4
		movups	xmm1, [edx+8]
		psrldq	xmm1, 4
		movups	xmm2, [edx+0x14]
		psrldq	xmm2, 4
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
		movq	qword ptr [ecx+0x18], xmm5
		pshufd	xmm6, xmm5, 0xFE
		movss	[ecx+0x20], xmm6
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33& __fastcall NiMatrix33::Rotate(const NiVector3 &rot)
{
	__asm
	{
		push	ecx
		sub		esp, 0x24
		mov		ecx, esp
		call	NiMatrix33::RotationMatrixInv
		mov		edx, eax
		mov		ecx, [esp+0x24]
		call	NiMatrix33::MultiplyMatrices
		add		esp, 0x28
		retn
	}
}

__declspec(naked) NiMatrix33& NiMatrix33::Transpose()
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [ecx+0x10]
		pshufd	xmm2, xmm0, 9
		shufps	xmm2, xmm1, 0x94
		shufps	xmm0, xmm2, 0x3C
		shufps	xmm1, xmm2, 0x9C
		movups	[ecx], xmm0
		movups	[ecx+0x10], xmm1
		mov		eax, ecx
		retn
	}
}

void NiMatrix33::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n", cr[0][0], cr[1][0], cr[2][0], cr[0][1], cr[1][1], cr[2][1], cr[0][2], cr[1][2], cr[2][2]);
}

__declspec(naked) bool __fastcall NiQuaternion::operator==(const NiQuaternion &rhs) const
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [edx]
		subps	xmm0, xmm1
		andps	xmm0, PS_AbsMask
		cmpltps	xmm0, PS_Epsilon
		movmskps	eax, xmm0
		cmp		al, 0xF
		setz	al
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
		movaps	xmm2, xmm1
		pshufd	xmm2, xmm0, 0xFE
		unpcklps	xmm1, xmm0
		pshufd	xmm1, xmm0, 0xFE
		pshufd	xmm0, xmm1, 0x8C
		subps	xmm0, xmm2
		movaps	xmm2, xmm0
		shufps	xmm0, xmm3, 1
		shufps	xmm0, xmm2, 0xE2
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
		movaps	xmm2, xmm0
		pshufd	xmm2, xmm1, 0xFE
		shufps	xmm2, xmm2, 0xD0
		unpcklps	xmm1, xmm0
		pshufd	xmm1, xmm0, 0xFE
		pshufd	xmm0, xmm1, 0xE0
		xorps	xmm2, xmm4
		addps	xmm0, xmm2
		shufps	xmm3, xmm0, 0x30
		shufps	xmm0, xmm3, 0x29
		jmp		done
	t_xz:
		comiss	xmm1, xmm2
		jbe		t_z
		addss	xmm3, xmm1
		subss	xmm3, xmm0
		subss	xmm3, xmm2
		movaps	xmm2, xmm1
		unpcklps	xmm2, xmm0
		pshufd	xmm2, xmm1, 0xFE
		shufps	xmm2, xmm2, 0x2C
		pshufd	xmm0, xmm1, 0xFE
		shufps	xmm0, xmm0, 0x9C
		xorps	xmm0, xmm4
		addps	xmm0, xmm2
		movaps	xmm2, xmm0
		shufps	xmm0, xmm3, 1
		shufps	xmm0, xmm2, 0xE8
		jmp		done
	t_z:
		subss	xmm3, xmm1
		subss	xmm3, xmm0
		addss	xmm3, xmm2
		movaps	xmm2, xmm1
		unpcklps	xmm2, xmm0
		pshufd	xmm2, xmm1, 0xFE
		shufps	xmm2, xmm2, 0xC8
		pshufd	xmm0, xmm1, 0xFE
		xorps	xmm0, xmm4
		addps	xmm0, xmm2
		shufps	xmm3, xmm0, 0x30
		shufps	xmm0, xmm3, 0x49
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

__declspec(naked) NiQuaternion& __fastcall NiQuaternion::FromEulerYPR(const NiVector3 &ypr)
{
	__asm
	{
		mov		eax, offset PS_V3_PI
		movups	xmm0, [edx]
		movaps	xmm1, [eax]
		cmpltps	xmm1, xmm0
		andps	xmm1, [eax+0x10]
		subps	xmm0, xmm1
		mulps	xmm0, [eax+0x20]
		mov		eax, ecx
		call	GetSinCosV3
		movaps	xmm2, xmm0
		shufps	xmm2, xmm1, 0x44
		shufps	xmm2, xmm2, 0xD7
		movaps	xmm3, xmm0
		pshufd	xmm3, xmm1, 0xFE
		shufps	xmm3, xmm3, 0x88
		movaps	xmm4, xmm1
		shufps	xmm4, xmm0, 0x44
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
		movups	[eax], xmm0
		retn
	}
}

__declspec(naked) NiQuaternion& __fastcall NiQuaternion::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		mulss	xmm0, PS_V3_Half
		movups	xmm7, [edx]
		mov		eax, ecx
		call	GetSinCos
		shufps	xmm0, xmm0, 2
		shufps	xmm7, xmm7, 0x90
		mulps	xmm0, xmm7
		movaps	xmm2, xmm0
		shufps	xmm0, xmm1, 1
		shufps	xmm0, xmm2, 0xE2
		movups	[eax], xmm0
		retn
	}
}

void NiQuaternion::operator=(const hkQuaternion &hkQt)
{
	_mm_storeu_si128((__m128i*)&w, _mm_shuffle_epi32(_mm_load_si128((__m128i*)&hkQt.x), 0x93));
}

__declspec(naked) NiQuaternion& __fastcall NiQuaternion::MultiplyQuaternion(const NiQuaternion &rhs)
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
		movaps	xmm0, xmm3
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
		movaps	xmm4, xmm3
		pshufd	xmm3, xmm1, 0x1B
		mulps	xmm3, xmm2
		pslldq	xmm5, 4
		xorps	xmm3, xmm5
		haddps	xmm3, xmm6
		haddps	xmm3, xmm6
		unpcklps	xmm4, xmm3
		shufps	xmm0, xmm4, 0x44
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiQuaternion& NiQuaternion::Normalize()
{
    __asm
    {
		mov		eax, ecx
		movups	xmm1, [eax]
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
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm1
	zeroLen:
		movups	[eax], xmm0
        retn
    }
}

__declspec(naked) NiVector3& __fastcall NiQuaternion::ToEulerYPR(NiVector3 &ypr) const
{
	__asm
	{
		movups	xmm7, [ecx]
		pshufd	xmm0, xmm7, 0x98
		pshufd	xmm1, xmm7, 0x9D
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, PS_FlipSignMask0
		addss	xmm1, PS_V3_One
		call	ATan2
		movss	[edx], xmm0
		pshufd	xmm0, xmm7, 0xC
		pshufd	xmm1, xmm7, 6
		mulps	xmm0, xmm1
		hsubps	xmm0, xmm0
		addss	xmm0, xmm0
		call	ASin
		movss	[edx+4], xmm0
		pshufd	xmm0, xmm7, 0xE4
		pshufd	xmm1, xmm7, 0xEB
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, PS_FlipSignMask0
		addss	xmm1, PS_V3_One
		call	ATan2
		movss	[edx+8], xmm0
		mov		eax, edx
		retn
	}
}

void __vectorcall NiQuaternion::lerp(const NiQuaternion &qb, float t)
{
	if (t <= 0) return;
	if (t >= 1.0F)
	{
		*this = qb;
		return;
	}
	float cosHalfTheta = DotProduct(qb);
	if (cosHalfTheta < 0)
		Negate();
	*this += (qb - *this) * t;
	Normalize();
}

void __vectorcall NiQuaternion::slerp(const NiQuaternion &qb, float t)
{
	if (t <= 0) return;
	if (t >= 1.0F)
	{
		*this = qb;
		return;
	}
	float cosHalfTheta = DotProduct(qb);
	if (abs(cosHalfTheta) >= 1.0F)
		return;
	if (cosHalfTheta < 0)
		Negate();
	float sinHalfTheta = 1.0F - cosHalfTheta * cosHalfTheta;
	if (sinHalfTheta < 0.001F)
	{
		*this += (qb - *this) * t;
		Normalize();
	}
	else
	{
		float halfTheta = ACos(cosHalfTheta);
		*this *= Cos(FltPId2 + t * halfTheta - halfTheta);
		*this += qb * Cos(FltPId2 - t * halfTheta);
		*this *= 1.0F / sqrtf(sinHalfTheta);
	}
}

void NiQuaternion::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\t%.6f\n", x, y, z, w);
}

__declspec(naked) NiVector3& __fastcall NiTransform::GetTranslatedPos(NiVector3 &posMods)
{
	__asm
	{
		xchg	ecx, edx
		call	NiVector3::MultiplyMatrix
		movups	xmm1, [edx+0x24]
		addps	xmm0, xmm1
		pshufd	xmm1, xmm0, 0xFE
		movq	qword ptr [eax], xmm0
		movss	[eax+8], xmm1
		retn
	}
}

__declspec(naked) float __vectorcall Point3Distance(const NiVector3 &pt1, const NiVector3 &pt2)
{
	__asm
	{
		movups	xmm0, [ecx-4]
		psrldq	xmm0, 4
		movups	xmm1, [edx-4]
		psrldq	xmm1, 4
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		xorps	xmm1, xmm1
		haddps	xmm0, xmm1
		haddps	xmm0, xmm1
		sqrtss	xmm0, xmm0
		retn
	}
}