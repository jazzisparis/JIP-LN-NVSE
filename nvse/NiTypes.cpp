#include "nvse/NiTypes.h"

const __m128 kHalfAngles = {0.5F, 0.5F, 0.5F, 0};

__declspec(naked) NiVector3* __fastcall NiVector3::MultiplyMatrix(const NiMatrix33 &mat)
{
	__asm
	{
		movups	xmm0, [ecx]
		andps	xmm0, kSSEDiscard4thPS
		movups	xmm1, [edx]
		mulps	xmm1, xmm0
		haddps	xmm1, xmm1
		haddps	xmm1, xmm1
		movss	[ecx], xmm1
		movups	xmm1, [edx+0xC]
		mulps	xmm1, xmm0
		haddps	xmm1, xmm1
		haddps	xmm1, xmm1
		movss	[ecx+4], xmm1
		movups	xmm1, [edx+0x18]
		mulps	xmm1, xmm0
		haddps	xmm1, xmm1
		haddps	xmm1, xmm1
		movss	[ecx+8], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3* __fastcall NiVector3::MultiplyMatrixRow(const NiMatrix33 &rotMatrix, UInt32 whichRow)
{
	__asm
	{
		mov		eax, [esp+4]
		movups	xmm0, [edx+eax*4]
		movss	xmm1, [edx+eax*4+0x18]
		shufps	xmm0, xmm0, 0xC
		movlhps	xmm0, xmm1
		movups	xmm1, [ecx]
		mulps	xmm0, xmm1
		movhlps	xmm1, xmm0
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		mov		eax, ecx
		retn	4
	}
}

__declspec(naked) NiVector3* __fastcall NiVector3::MultiplyQuaternion(const NiQuaternion &qt)
{
	__asm
	{
		movups	xmm0, [edx]
		movups	xmm1, [ecx]
		movaps	xmm5, kSSEDiscard4thPS
		andps	xmm1, xmm5
		pshufd	xmm2, xmm0, 1
		pshufd	xmm3, xmm1, 0x90
		mulps	xmm2, xmm3
		pshufd	xmm3, xmm0, 0x7A
		pshufd	xmm4, xmm1, 0x49
		mulps	xmm3, xmm4
		xorps	xmm3, kSSEChangeSignMaskPS0
		subps	xmm2, xmm3
		pshufd	xmm3, xmm0, 0x9F
		pshufd	xmm4, xmm1, 0x26
		mulps	xmm3, xmm4
		addps	xmm2, xmm3
		pshufd	xmm1, xmm2, 0x39
		andps	xmm1, xmm5
		pshufd	xmm3, xmm0, 0
		mulps	xmm1, xmm3
		pshufd	xmm3, xmm2, 0
		andps	xmm3, xmm5
		pshufd	xmm4, xmm0, 0x39
		mulps	xmm3, xmm4
		addps	xmm1, xmm3
		pshufd	xmm3, xmm2, 0x1E
		andps	xmm3, xmm5
		pshufd	xmm4, xmm0, 0x27
		mulps	xmm3, xmm4
		addps	xmm1, xmm3
		pshufd	xmm3, xmm2, 0x27
		andps	xmm3, xmm5
		pshufd	xmm4, xmm0, 0x1E
		mulps	xmm3, xmm4
		subps	xmm1, xmm3
		movhlps	xmm0, xmm1
		movq	qword ptr [ecx], xmm1
		movss	[ecx+8], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3 *NiVector3::Normalize()
{
    __asm
    {
		mov		eax, ecx
		movups	xmm0, [ecx]
		andps	xmm0, kSSEDiscard4thPS
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		haddps	xmm1, xmm1
		haddps	xmm1, xmm1
		pxor	xmm2, xmm2
		comiss	xmm1, kFlt1d10K
		jb		zeroLen
		sqrtss	xmm1, xmm1
		movss	xmm2, kFltOne
		divss	xmm2, xmm1
		shufps	xmm2, xmm2, 0xC0
		mulps	xmm2, xmm0
	zeroLen:
		movhlps	xmm1, xmm2
        movq	qword ptr [ecx], xmm2
        movss	[ecx+8], xmm1
        retn
    }
}

__declspec(naked) NiVector3* __fastcall NiVector3::CrossProduct(const NiVector3 &vB)
{
	__asm
	{
		movups	xmm0, [edx]
		andps	xmm0, kSSEDiscard4thPS
		movups	xmm1, [ecx]
		pshufd	xmm2, xmm0, 0xC9
		pshufd	xmm3, xmm1, 0x12
		mulps	xmm2, xmm3
		pshufd	xmm3, xmm0, 0xD2
		pshufd	xmm4, xmm1, 9
		mulps	xmm3, xmm4
		subps	xmm2, xmm3
		movhlps	xmm0, xmm2
		movq	qword ptr [ecx], xmm2
		movss	[ecx+8], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) AxisAngle* __fastcall AxisAngle::FromEulerYPR(const NiVector3 &ypr)
{
	__asm
	{
		movups	xmm7, [edx]
		mulps	xmm7, kHalfAngles
		pshufd	xmm0, xmm7, 0xFE
		call	GetSinCos
		movaps	xmm5, xmm0
		pshufd	xmm0, xmm7, 0xFD
		call	GetSinCos
		movaps	xmm6, xmm0
		pshufd	xmm0, xmm7, 0xFC
		call	GetSinCos
		movaps	xmm7, xmm0
		movlhps	xmm0, xmm6
		shufps	xmm0, xmm0, 8
		shufps	xmm0, xmm5, 0x84
		movq	xmm1, xmm6
		movlhps	xmm1, xmm5
		shufps	xmm1, xmm1, 0xD
		shufps	xmm1, xmm7, 0x94
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		shufps	xmm0, xmm0, 0xC9
		shufps	xmm1, xmm1, 0xC9
		pshufd	xmm3, xmm2, 0xD2
		mulps	xmm2, xmm1
		mulps	xmm3, xmm0
		pshufd	xmm0, kSSEChangeSignMaskPS0, 0x44
		xorps	xmm3, xmm0
		addps	xmm2, xmm3
		movups	[ecx], xmm2
		movaps	xmm0, xmm5
		mulps	xmm0, xmm6
		mulps	xmm0, xmm7
		haddps	xmm0, xmm0
		call	ACos
		addss	xmm0, xmm0
		movss	[ecx+0xC], xmm0
		jmp		NiVector3::Normalize
	}
}

__declspec(naked) AxisAngle* __fastcall AxisAngle::FromRotationMatrix(const NiMatrix33 &rotMat)
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
		subss	xmm0, kFltOne
		mulss	xmm0, kFltHalf
		call	ACos
		movss	[ecx+0xC], xmm0
		jmp		NiVector3::Normalize
	}
}

__declspec(naked) AxisAngle* __fastcall AxisAngle::FromQuaternion(const NiQuaternion &qt)
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

NiVector3 *AxisAngle::ToEulerYPR(NiVector3 &ypr) const
{
	NiQuaternion qt = *this;
	return qt.ToEulerYPR(ypr);
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::FromQuaternion(const NiQuaternion &qt)
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
		subss	xmm0, kFltOne
		pshufd	xmm3, xmm0, 1
		addss	xmm3, xmm0
		movss	[ecx], xmm3
		pshufd	xmm3, xmm0, 2
		addss	xmm3, xmm0
		movss	[ecx+0x10], xmm3
		pshufd	xmm3, xmm0, 3
		addss	xmm3, xmm0
		movss	[ecx+0x20], xmm3
		pshufd	xmm3, xmm2, 1
		movss	xmm4, xmm3
		pshufd	xmm5, xmm1, 3
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0xC], xmm3
		movss	[ecx+4], xmm4
		pshufd	xmm3, xmm2, 3
		movss	xmm4, xmm3
		pshufd	xmm5, xmm1, 2
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x18], xmm4
		movss	[ecx+8], xmm3
		pshufd	xmm3, xmm2, 2
		movss	xmm4, xmm3
		pshufd	xmm5, xmm1, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x1C], xmm3
		movss	[ecx+0x14], xmm4
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiVector3* __fastcall NiMatrix33::ExtractAngles(NiVector3 &outAngles) const
{
	__asm
	{
		movss	xmm0, [ecx+8]
		movss	xmm1, xmm0
		andps	xmm1, kSSERemoveSignMaskPS
		comiss	xmm1, kFltOne
		jnb		zeroZ
		call	ASin
		movss	[edx+4], xmm0
		xor		byte ptr [edx+7], 0x80
		movss	xmm0, [ecx+0x14]
		movss	xmm1, [ecx+0x20]
		call	ATan2
		movss	[edx], xmm0
		movss	xmm0, [ecx+4]
		movss	xmm1, [ecx]
		call	ATan2
		movss	[edx+8], xmm0
		mov		eax, edx
		retn
	zeroZ:
		movss	xmm0, [ecx+0xC]
		movss	xmm1, [ecx+0x10]
		call	ATan2
		movss	[edx], xmm0
		mov		dword ptr [edx+4], 0x3FC90FDB
		pxor	xmm1, xmm1
		movss	[edx+8], xmm1
		mov		eax, 3
		mov		ecx, 7
		comiss	xmm0, xmm1
		cmova	eax, ecx
		xor		byte ptr [edx+eax], 0x80
		mov		eax, edx
		retn
	}
}

__declspec(naked) NiVector3* __fastcall NiMatrix33::ExtractAnglesInv(NiVector3 &outAngles) const
{
	__asm
	{
		movss	xmm0, [ecx+0x18]
		movss	xmm1, xmm0
		andps	xmm1, kSSERemoveSignMaskPS
		comiss	xmm1, kFltOne
		jnb		zeroZ
		call	ASin
		movss	[edx+4], xmm0
		xor		byte ptr [edx+7], 0x80
		movss	xmm0, [ecx+0x1C]
		movss	xmm1, [ecx+0x20]
		call	ATan2
		movss	[edx], xmm0
		movss	xmm0, [ecx+0xC]
		movss	xmm1, [ecx]
		call	ATan2
		movss	[edx+8], xmm0
		mov		eax, edx
		retn
	zeroZ:
		movss	xmm0, [ecx+4]
		movss	xmm1, [ecx+0x10]
		call	ATan2
		movss	[edx], xmm0
		mov		dword ptr [edx+4], 0x3FC90FDB
		pxor	xmm1, xmm1
		movss	[edx+8], xmm1
		mov		eax, 3
		mov		ecx, 7
		comiss	xmm0, xmm1
		cmova	eax, ecx
		xor		byte ptr [edx+eax], 0x80
		mov		eax, edx
		retn
	}
}

__declspec(naked) void RotationMatrixX()
{
	__asm
	{
		movss	xmm0, xmm7
		call	GetSinCos
		movq	qword ptr [ecx+0x1C], xmm0
		shufps	xmm0, xmm0, 1
		movq	qword ptr [ecx+0x10], xmm0
		xor		byte ptr [ecx+0x1F], 0x80
		mov		eax, ecx
		retn
	}
}
__declspec(naked) void RotationMatrixY()
{
	__asm
	{
		pshufd	xmm0, xmm7, 0xFD
		call	GetSinCos
		movss	[ecx+8], xmm0
		xor		byte ptr [ecx+0xB], 0x80
		movss	[ecx+0x18], xmm0
		shufps	xmm0, xmm0, 1
		movss	[ecx], xmm0
		movss	[ecx+0x20], xmm0
		mov		eax, ecx
		retn
	}
}
__declspec(naked) void RotationMatrixXY()
{
	__asm
	{
		movss	xmm0, xmm7
		call	GetSinCos
		movss	[ecx+0x1C], xmm0
		shufps	xmm0, xmm0, 1
		movss	[ecx+0x10], xmm0
		pshufd	xmm0, xmm7, 0xFD
		call	GetSinCos
		movss	[ecx+8], xmm0
		movss	xmm2, xmm0
		pshufd	xmm3, xmm0, 0xA9
		movss	[ecx], xmm3
		movss	xmm0, [ecx+0x1C]
		movss	xmm1, [ecx+0x10]
		xor		byte ptr [ecx+0xB], 0x80
		movss	xmm4, xmm0
		mulss	xmm4, xmm2
		movss	[ecx+0xC], xmm4
		mulss	xmm0, xmm3
		movss	[ecx+0x14], xmm0
		movss	xmm4, xmm1
		mulss	xmm4, xmm2
		movss	[ecx+0x18], xmm4
		xor		byte ptr [ecx+0x1F], 0x80
		mulss	xmm1, xmm3
		movss	[ecx+0x20], xmm1
		mov		eax, ecx
		retn
	}
}
__declspec(naked) void RotationMatrixZ()
{
	__asm
	{
		pshufd	xmm0, xmm7, 0xFE
		call	GetSinCos
		movq	qword ptr [ecx+0xC], xmm0
		shufps	xmm0, xmm0, 1
		movq	qword ptr [ecx], xmm0
		xor		byte ptr [ecx+0xF], 0x80
		mov		eax, ecx
		retn
	}
}
__declspec(naked) void RotationMatrixXZ()
{
	__asm
	{
		movss	xmm0, xmm7
		call	GetSinCos
		movss	[ecx+0x14], xmm0
		shufps	xmm0, xmm0, 1
		movss	[ecx+0x20], xmm0
		pshufd	xmm0, xmm7, 0xFE
		call	GetSinCos
		movss	xmm2, xmm0
		pshufd	xmm3, xmm0, 0xA1
		movq	qword ptr [ecx], xmm3
		movss	xmm0, [ecx+0x14]
		movss	xmm1, [ecx+0x20]
		movss	xmm4, xmm1
		mulss	xmm4, xmm2
		movss	[ecx+0xC], xmm4
		xor		byte ptr [ecx+0xF], 0x80
		mulss	xmm1, xmm3
		movss	[ecx+0x10], xmm1
		movss	xmm4, xmm0
		mulss	xmm4, xmm2
		movss	[ecx+0x18], xmm4
		mulss	xmm0, xmm3
		movss	[ecx+0x1C], xmm0
		xor		byte ptr [ecx+0x1F], 0x80
		mov		eax, ecx
		retn
	}
}
__declspec(naked) void RotationMatrixYZ()
{
	__asm
	{
		pshufd	xmm0, xmm7, 0xFD
		call	GetSinCos
		movss	[ecx+8], xmm0
		shufps	xmm0, xmm0, 1
		movss	[ecx+0x20], xmm0
		pshufd	xmm0, xmm7, 0xFE
		call	GetSinCos
		movq	qword ptr [ecx+0xC], xmm0
		movss	xmm2, xmm0
		pshufd	xmm3, xmm0, 0xA9
		movss	xmm0, [ecx+8]
		movss	xmm1, [ecx+0x20]
		movss	xmm4, xmm1
		mulss	xmm4, xmm3
		movss	[ecx], xmm4
		mulss	xmm1, xmm2
		movss	[ecx+4], xmm1
		xor		byte ptr [ecx+0xB], 0x80
		xor		byte ptr [ecx+0xF], 0x80
		movss	xmm4, xmm0
		mulss	xmm4, xmm3
		movss	[ecx+0x18], xmm4
		mulss	xmm0, xmm2
		movss	[ecx+0x1C], xmm0
		mov		eax, ecx
		retn
	}
}
__declspec(naked) void RotationMatrixXYZ()
{
	__asm
	{
		movss	xmm0, xmm7
		call	GetSinCos
		movq	qword ptr [ecx], xmm0
		pshufd	xmm0, xmm7, 0xFD
		call	GetSinCos
		movq	qword ptr [ecx+8], xmm0
		pshufd	xmm0, xmm7, 0xFE
		call	GetSinCos
		movss	xmm4, xmm0
		pshufd	xmm5, xmm0, 0xA9
		movss	xmm0, [ecx]
		movss	xmm1, [ecx+4]
		movss	xmm2, [ecx+8]
		movss	xmm3, [ecx+0xC]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss	[ecx], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss	[ecx+4], xmm6
		xor		byte ptr [ecx+0xB], 0x80
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movss	[ecx+0xC], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movss	[ecx+0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movss	[ecx+0x14], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movss	[ecx+0x18], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movss	[ecx+0x1C], xmm6
		mulss	xmm1, xmm3
		movss	[ecx+0x20], xmm1
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrix(const NiVector3 &rot)
{
	static const void *kRotationMatrixJmpTable[] = {RotationMatrixX, RotationMatrixY, RotationMatrixXY, RotationMatrixZ, RotationMatrixXZ, RotationMatrixYZ, RotationMatrixXYZ};
	__asm
	{
		mov		eax, 0x3F800000
		movd	xmm0, eax
		movups	[ecx], xmm0
		movups	[ecx+0x10], xmm0
		mov		[ecx+0x20], eax
		movups	xmm7, [edx]
		andps	xmm7, kSSEDiscard4thPS
		pxor	xmm1, xmm1
		cmpps	xmm1, xmm7, 4
		movmskps	eax, xmm1
		test	al, al
		jz		noRotation
		jmp		kRotationMatrixJmpTable[eax*4-4]
	noRotation:
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrixInv(const NiVector3 &rot)
{
	__asm
	{
		call	NiMatrix33::RotationMatrix
		movups	xmm0, [eax]
		movups	xmm1, [eax+0x10]
		shufps	xmm0, xmm0, 0x6C
		shufps	xmm1, xmm1, 0x6C
		movaps	xmm2, xmm1
		movhlps	xmm2, xmm0
		shufps	xmm0, xmm2, 0x64
		shufps	xmm1, xmm2, 0xC4
		movups	[eax], xmm0
		movups	[eax+0x10], xmm1
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		movups	xmm7, [edx]
		andps	xmm7, kSSEDiscard4thPS
		call	GetSinCos
		pshufd	xmm2, xmm0, 0x80
		pshufd	xmm3, xmm0, 0xA9
		movss	xmm4, kFltOne
		subss	xmm4, xmm3
		shufps	xmm4, xmm4, 0x40
		mulps	xmm2, xmm7
		mulps	xmm4, xmm7
		pshufd	xmm1, xmm7, 0xC9
		mulps	xmm1, xmm4
		mulps	xmm7, xmm4
		movss	xmm4, xmm7
		addss	xmm4, xmm3
		movss	[ecx], xmm4
		pshufd	xmm4, xmm7, 1
		addss	xmm4, xmm3
		movss	[ecx+0x10], xmm4
		pshufd	xmm4, xmm7, 2
		addss	xmm4, xmm3
		movss	[ecx+0x20], xmm4
		movss	xmm3, xmm1
		movss	xmm4, xmm3
		pshufd	xmm5, xmm2, 2
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0xC], xmm3
		movss	[ecx+4], xmm4
		pshufd	xmm3, xmm1, 2
		movss	xmm4, xmm3
		pshufd	xmm5, xmm2, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+8], xmm3
		movss	[ecx+0x18], xmm4
		pshufd	xmm3, xmm1, 1
		movss	xmm4, xmm3
		subss	xmm3, xmm2
		addss	xmm4, xmm2
		movss	[ecx+0x1C], xmm3
		movss	[ecx+0x14], xmm4
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::MultiplyMatrices(const NiMatrix33 &matB)
{
	__asm
	{
		movups	xmm0, [edx]
		movups	xmm1, [edx+0xC]
		movups	xmm2, [edx+0x18]
		movups	xmm3, [ecx]
		movups	xmm4, [ecx+0x10]
		movaps	xmm5, kSSEDiscard4thPS
		andps	xmm0, xmm5
		andps	xmm1, xmm5
		andps	xmm2, xmm5
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
		movhlps	xmm6, xmm5
		movss	[ecx+0x20], xmm6
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::Rotate(const NiVector3 &rot)
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

__declspec(naked) NiMatrix33 *NiMatrix33::Transpose()
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [ecx+0x10]
		shufps	xmm0, xmm0, 0x6C
		shufps	xmm1, xmm1, 0x6C
		movaps	xmm2, xmm1
		movhlps	xmm2, xmm0
		shufps	xmm0, xmm2, 0x64
		shufps	xmm1, xmm2, 0xC4
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
	static const __m128 kEqEpsilon = {0.0001F, 0.0001F, 0.0001F, 0.0001F};
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [edx]
		subps	xmm0, xmm1
		andps	xmm0, kSSERemoveSignMaskPS
		cmpps	xmm0, kEqEpsilon, 1
		movmskps	eax, xmm0
		xor		al, 0xF
		setz	al
		retn
	}
}

__declspec(naked) NiQuaternion* __fastcall NiQuaternion::FromRotationMatrix(const NiMatrix33 &rotMat)
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
		movss	xmm5, xmm4
		pxor	xmm6, xmm6
		comiss	xmm3, xmm6
		jbe		t_xyz
		addss	xmm5, xmm3
		movss	[ecx], xmm5
		movss	xmm0, [edx+0x14]
		subss	xmm0, [edx+0x1C]
		movss	[ecx+4], xmm0
		movss	xmm0, [edx+0x18]
		subss	xmm0, [edx+8]
		movss	[ecx+8], xmm0
		movss	xmm0, [edx+4]
		subss	xmm0, [edx+0xC]
		movss	[ecx+0xC], xmm0
		jmp		done
	t_xyz:
		comiss	xmm0, xmm1
		jnb		t_xz
		comiss	xmm1, xmm2
		jb		t_z
		subss	xmm5, xmm0
		addss	xmm5, xmm1
		subss	xmm5, xmm2
		movss	xmm0, [edx+0x18]
		subss	xmm0, [edx+8]
		movss	[ecx], xmm0
		movss	xmm0, [edx+4]
		addss	xmm0, [edx+0xC]
		movss	[ecx+4], xmm0
		movss	[ecx+8], xmm5
		movss	xmm0, [edx+0x14]
		addss	xmm0, [edx+0x1C]
		movss	[ecx+0xC], xmm0
		jmp		done
	t_xz:
		comiss	xmm0, xmm2
		jb		t_z
		addss	xmm5, xmm0
		subss	xmm5, xmm1
		subss	xmm5, xmm2
		movss	xmm0, [edx+0x14]
		subss	xmm0, [edx+0x1C]
		movss	[ecx], xmm0
		movss	[ecx+4], xmm5
		movss	xmm0, [edx+4]
		addss	xmm0, [edx+0xC]
		movss	[ecx+8], xmm0
		movss	xmm0, [edx+8]
		addss	xmm0, [edx+0x18]
		movss	[ecx+0xC], xmm0
		jmp		done
	t_z:
		subss	xmm5, xmm0
		subss	xmm5, xmm1
		addss	xmm5, xmm2
		movss	xmm0, [edx+4]
		subss	xmm0, [edx+0xC]
		movss	[ecx], xmm0
		movss	xmm0, [edx+8]
		addss	xmm0, [edx+0x18]
		movss	[ecx+4], xmm0
		movss	xmm0, [edx+0x14]
		addss	xmm0, [edx+0x1C]
		movss	[ecx+8], xmm0
		movss	[ecx+0xC], xmm5
	done:
		andps	xmm5, kSSERemoveSignMaskPS
		sqrtss	xmm0, xmm5
		addss	xmm0, xmm0
		divss	xmm4, xmm0
		shufps	xmm4, xmm4, 0
		movups	xmm0, [ecx]
		mulps	xmm0, xmm4
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiQuaternion* __fastcall NiQuaternion::FromEulerYPR(const NiVector3 &ypr)
{
	__asm
	{
		movups	xmm7, [edx]
		mulps	xmm7, kHalfAngles
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
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) NiQuaternion* __fastcall NiQuaternion::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		mulss	xmm0, kFltHalf
		movups	xmm7, [edx]
		call	GetSinCos
		pshufd	xmm1, xmm0, 1
		shufps	xmm0, xmm0, 2
		shufps	xmm7, xmm7, 0x90
		mulps	xmm0, xmm7
		movss	xmm0, xmm1
		movups	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) void __fastcall NiQuaternion::operator*=(const NiQuaternion &rhs)
{
	__asm
	{
		movups	xmm0, [edx]
		movups	xmm1, [ecx]
		movss	xmm2, kSSEChangeSignMaskPS0
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

__declspec(naked) NiQuaternion *NiQuaternion::Normalize()
{
    __asm
    {
		mov		eax, ecx
		movups	xmm0, [eax]
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		haddps	xmm1, xmm1
		haddps	xmm1, xmm1
		pxor	xmm2, xmm2
		comiss	xmm1, kFlt1d10K
		jb		zeroLen
		sqrtss	xmm1, xmm1
		movss	xmm2, kFltOne
		divss	xmm2, xmm1
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm0
	zeroLen:
		movups	[eax], xmm2
        retn
    }
}

__declspec(naked) NiVector3* __fastcall NiQuaternion::ToEulerYPR(NiVector3 &ypr) const
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
		xorps	xmm1, kSSEChangeSignMaskPS0
		addss	xmm1, kFltOne
		call	ATan2
		movss	[edx], xmm0
		pshufd	xmm0, xmm7, 0xC
		pshufd	xmm1, xmm7, 6
		mulps	xmm0, xmm1
		hsubps	xmm0, xmm0
		addss	xmm0, xmm0
		movss	xmm1, xmm0
		andps	xmm1, kSSERemoveSignMaskPS
		comiss	xmm1, kFltOne
		jnb		invSinP
		call	ASin
		jmp		doneY
	invSinP:
		movss	xmm1, xmm0
		andps	xmm1, kSSEChangeSignMaskPS0
		movss	xmm0, kFltPId2
		xorps	xmm0, xmm1
	doneY:
		movss	[edx+4], xmm0
		pshufd	xmm0, xmm7, 0xE4
		pshufd	xmm1, xmm7, 0xEB
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
		*this *= Sin(halfTheta - t * halfTheta);
		*this += qb * Sin(t * halfTheta);
		*this *= 1.0F / sqrtf(sinHalfTheta);
	}
}

__declspec(naked) float __vectorcall Vector3Length(AlignedVector4 *inVec)
{
	__asm
	{
		movaps	xmm0, [ecx]
		andps	xmm0, kSSEDiscard4thPS
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}

__declspec(naked) float __vectorcall Point3Distance(NiVector3 *pt1, NiVector3 *pt2)
{
	__asm
	{
		movaps	xmm2, kSSEDiscard4thPS
		movups	xmm0, [ecx]
		andps	xmm0, xmm2
		movups	xmm1, [edx]
		andps	xmm1, xmm2
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		haddps	xmm0, xmm0
		sqrtss	xmm0, xmm0
		retn
	}
}