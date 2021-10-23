#include "nvse/NiTypes.h"

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
		comiss	xmm1, xmm2
		jz		zeroLen
		sqrtss	xmm2, xmm1
		movss	xmm1, kFltOne
		divss	xmm1, xmm2
		shufps	xmm1, xmm1, 0xC0
		mulps	xmm0, xmm1
		movhlps	xmm1, xmm0
		movq	qword ptr [ecx], xmm0
		movss	[ecx+8], xmm1
		retn
	zeroLen:
        movq	qword ptr [ecx], xmm2
        movss	[ecx+8], xmm2
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

__declspec(noinline) void AxisAngle::operator=(const NiVector3 &ypr)
{
	__m128 sc = GetSinCos(ypr.z * 0.5F);
	float sX = sc.m128_f32[0];
	float cX = sc.m128_f32[1];
	sc = GetSinCos(ypr.y * 0.5F);
	float sY = sc.m128_f32[0];
	float cY = sc.m128_f32[1];
	sc = GetSinCos(ypr.x * 0.5F);
	float sZ = sc.m128_f32[0];
	float cZ = sc.m128_f32[1];
	float sZcY = sZ * cY;
	float sYcX = sY * cX;
	float sXcZ = sX * cZ;
	theta = 2.0F * dACos(cX * cY * cZ + sX * sY * sZ);
	axis.x = (sZcY * cX) - (sXcZ * sY);
	axis.y = (sYcX * cZ) + (sZcY * sX);
	axis.z = (sXcZ * cY) - (sYcX * sZ);
	axis.Normalize();
}

void AxisAngle::operator=(const NiMatrix33 &rm)
{
	theta = dACos((rm[0] + rm[4] + rm[8] - 1.0F) * 0.5F);
	axis = {rm[5] - rm[7], rm[6] - rm[2], rm[1] - rm[3]};
	axis.Normalize();
}

void AxisAngle::operator=(const NiQuaternion &qt)
{
	theta = 2.0F * dACos(qt.w);
	axis = {qt.x, qt.y, qt.z};
	axis.Normalize();
}

void AxisAngle::ToEulerYPR(NiVector3 &ypr) const
{
	NiQuaternion qt = *this;
	qt.ToEulerYPR(ypr);
}

__declspec(naked) void __fastcall NiMatrix33::operator=(const NiQuaternion &qt)
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
		retn
	}
}

__declspec(naked) void __fastcall NiMatrix33::ExtractAngles(NiVector3 *outAngles) const
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		movss	xmm0, [esi+8]
		movss	xmm1, xmm0
		andps	xmm1, kSSERemoveSignMaskPS
		comiss	xmm1, kFltOne
		jnb		zeroZ
		call	dASin
		movss	[edi+4], xmm0
		xor		byte ptr [edi+7], 0x80
		movss	xmm0, [esi+0x14]
		movss	xmm1, [esi+0x20]
		call	dATan2
		movss	[edi], xmm0
		movss	xmm0, [esi+4]
		movss	xmm1, [esi]
		call	dATan2
		movss	[edi+8], xmm0
		pop		edi
		pop		esi
		retn
	zeroZ:
		movss	xmm0, [esi+0xC]
		movss	xmm1, [esi+0x10]
		call	dATan2
		movss	[edi], xmm0
		mov		dword ptr [edi+4], 0x3FC90FDB
		pxor	xmm1, xmm1
		movss	[edi+8], xmm1
		mov		edx, 7
		mov		ecx, 3
		comiss	xmm0, xmm1
		cmova	ecx, edx
		xor		byte ptr [edi+ecx], 0x80
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall NiMatrix33::ExtractAnglesInv(NiVector3 *outAngles) const
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		movss	xmm0, [esi+0x18]
		movss	xmm1, xmm0
		andps	xmm1, kSSERemoveSignMaskPS
		comiss	xmm1, kFltOne
		jnb		zeroZ
		call	dASin
		movss	[edi+4], xmm0
		xor		byte ptr [edi+7], 0x80
		movss	xmm0, [esi+0x1C]
		movss	xmm1, [esi+0x20]
		call	dATan2
		movss	[edi], xmm0
		movss	xmm0, [esi+0xC]
		movss	xmm1, [esi]
		call	dATan2
		movss	[edi+8], xmm0
		pop		edi
		pop		esi
		retn
	zeroZ:
		movss	xmm0, [esi+4]
		movss	xmm1, [esi+0x10]
		call	dATan2
		movss	[edi], xmm0
		mov		dword ptr [edi+4], 0x3FC90FDB
		pxor	xmm1, xmm1
		movss	[edi+8], xmm1
		mov		edx, 7
		mov		ecx, 3
		comiss	xmm0, xmm1
		cmova	ecx, edx
		xor		byte ptr [edi+ecx], 0x80
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) void RotationMatrixX()
{
	__asm
	{
		movss	xmm0, [edx]
		call	GetSinCos
		movss	[esi+0x14], xmm0
		movss	[esi+0x1C], xmm0
		xor		byte ptr [esi+0x1F], 0x80
		shufps	xmm0, xmm0, 1
		movss	[esi+0x10], xmm0
		movss	[esi+0x20], xmm0
		mov		eax, esi
		pop		esi
		retn
	}
}
__declspec(naked) void RotationMatrixY()
{
	__asm
	{
		movss	xmm0, [edx+4]
		call	GetSinCos
		movss	[esi+8], xmm0
		xor		byte ptr [esi+0xB], 0x80
		movss	[esi+0x18], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi], xmm0
		movss	[esi+0x20], xmm0
		mov		eax, esi
		pop		esi
		retn
	}
}
__declspec(naked) void RotationMatrixXY()
{
	__asm
	{
		movq	xmm0, qword ptr [edx]
		movq	qword ptr [esi+8], xmm0
		call	GetSinCos
		movss	[esi+0x1C], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+0x10], xmm0
		movss	xmm0, [esi+0xC]
		call	GetSinCos
		movss	[esi+8], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi], xmm0
		movss	xmm0, [esi+0x1C]
		movss	xmm1, [esi+0x10]
		movss	xmm2, [esi+8]
		movss	xmm3, [esi]
		xor		byte ptr [esi+0xB], 0x80
		movss	xmm4, xmm0
		mulss	xmm4, xmm2
		movss	[esi+0xC], xmm4
		mulss	xmm0, xmm3
		movss	[esi+0x14], xmm0
		movss	xmm4, xmm1
		mulss	xmm4, xmm2
		movss	[esi+0x18], xmm4
		xor		byte ptr [esi+0x1F], 0x80
		mulss	xmm1, xmm3
		movss	[esi+0x20], xmm1
		mov		eax, esi
		pop		esi
		retn
	}
}
__declspec(naked) void RotationMatrixZ()
{
	__asm
	{
		movss	xmm0, [edx+8]
		call	GetSinCos
		movss	[esi+4], xmm0
		movss	[esi+0xC], xmm0
		xor		byte ptr [esi+0xF], 0x80
		shufps	xmm0, xmm0, 1
		movss	[esi], xmm0
		movss	[esi+0x10], xmm0
		mov		eax, esi
		pop		esi
		retn
	}
}
__declspec(naked) void RotationMatrixXZ()
{
	__asm
	{
		movups	xmm0, [edx]
		movups	[esi+0x10], xmm0
		call	GetSinCos
		movss	[esi+0x14], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+0x20], xmm0
		movss	xmm0, [esi+0x18]
		call	GetSinCos
		movss	[esi+4], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi], xmm0
		movss	xmm0, [esi+0x14]
		movss	xmm1, [esi+0x20]
		movss	xmm2, [esi+4]
		movss	xmm3, [esi]
		movss	xmm4, xmm1
		mulss	xmm4, xmm2
		movss	[esi+0xC], xmm4
		xor		byte ptr [esi+0xF], 0x80
		mulss	xmm1, xmm3
		movss	[esi+0x10], xmm1
		movss	xmm4, xmm0
		mulss	xmm4, xmm2
		movss	[esi+0x18], xmm4
		mulss	xmm0, xmm3
		movss	[esi+0x1C], xmm0
		xor		byte ptr [esi+0x1F], 0x80
		mov		eax, esi
		pop		esi
		retn
	}
}
__declspec(naked) void RotationMatrixYZ()
{
	__asm
	{
		movq	xmm0, qword ptr [edx+4]
		movq	qword ptr [esi+8], xmm0
		call	GetSinCos
		movss	[esi+8], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+0x20], xmm0
		movss	xmm0, [esi+0xC]
		call	GetSinCos
		movss	[esi+0xC], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+0x10], xmm0
		movss	xmm0, [esi+8]
		movss	xmm1, [esi+0x20]
		movss	xmm2, [esi+0xC]
		movss	xmm3, [esi+0x10]
		movss	xmm4, xmm1
		mulss	xmm4, xmm3
		movss	[esi], xmm4
		mulss	xmm1, xmm2
		movss	[esi+4], xmm1
		xor		byte ptr [esi+0xB], 0x80
		xor		byte ptr [esi+0xF], 0x80
		movss	xmm4, xmm0
		mulss	xmm4, xmm3
		movss	[esi+0x18], xmm4
		mulss	xmm0, xmm2
		movss	[esi+0x1C], xmm0
		mov		eax, esi
		pop		esi
		retn
	}
}
__declspec(naked) void RotationMatrixXYZ()
{
	__asm
	{
		movups	xmm0, [edx]
		movups	[esi+0x10], xmm0
		call	GetSinCos
		movss	[esi], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+4], xmm0
		movss	xmm0, [esi+0x14]
		call	GetSinCos
		movss	[esi+8], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+0xC], xmm0
		movss	xmm0, [esi+0x18]
		call	GetSinCos
		movss	[esi+0x10], xmm0
		shufps	xmm0, xmm0, 1
		movss	[esi+0x14], xmm0
		movss	xmm0, [esi]
		movss	xmm1, [esi+4]
		movss	xmm2, [esi+8]
		movss	xmm3, [esi+0xC]
		movss	xmm4, [esi+0x10]
		movss	xmm5, [esi+0x14]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss	[esi], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss	[esi+4], xmm6
		xor		byte ptr [esi+0xB], 0x80
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movss	[esi+0xC], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movss	[esi+0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movss	[esi+0x14], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movss	[esi+0x18], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movss	[esi+0x1C], xmm6
		mulss	xmm1, xmm3
		movss	[esi+0x20], xmm1
		mov		eax, esi
		pop		esi
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrix(const NiVector3 &rot)
{
	static alignas(16) const UInt32 kNEQZeroMask[] = {0x80000000, 0x80, 0x80, 0};
	static const void *kRotationMatrixJmpTable[] = {RotationMatrixX, RotationMatrixY, RotationMatrixXY, RotationMatrixZ, RotationMatrixXZ, RotationMatrixYZ, RotationMatrixXYZ};
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		ecx, 0x3F800000
		movd	xmm0, ecx
		movups	[esi], xmm0
		movups	[esi+0x10], xmm0
		mov		[esi+0x20], ecx
		movups	xmm0, [edx]
		pxor	xmm1, xmm1
		pcmpeqd	xmm0, xmm1
		andnps	xmm0, kNEQZeroMask
		pmovmskb	ecx, xmm0
		shl		cl, 3
		shr		ecx, 4
		jz		noRotation
		jmp		kRotationMatrixJmpTable[ecx-4]
	noRotation:
		mov		eax, esi
		pop		esi
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
		push	esi
		mov		esi, ecx
		movss	xmm0, [edx+0xC]
		call	GetSinCos
		pshufd	xmm2, xmm0, 0x80
		pshufd	xmm3, xmm0, 0xA9
		movss	xmm4, kFltOne
		subss	xmm4, xmm3
		shufps	xmm4, xmm4, 0x40
		movups	xmm0, [edx]
		andps	xmm0, kSSEDiscard4thPS
		mulps	xmm2, xmm0
		mulps	xmm4, xmm0
		pshufd	xmm1, xmm0, 0xC9
		mulps	xmm1, xmm4
		mulps	xmm0, xmm4
		movss	xmm4, xmm0
		addss	xmm4, xmm3
		movss	[esi], xmm4
		pshufd	xmm4, xmm0, 1
		addss	xmm4, xmm3
		movss	[esi+0x10], xmm4
		pshufd	xmm4, xmm0, 2
		addss	xmm4, xmm3
		movss	[esi+0x20], xmm4
		movss	xmm3, xmm1
		movss	xmm4, xmm3
		pshufd	xmm5, xmm2, 2
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[esi+0xC], xmm3
		movss	[esi+4], xmm4
		pshufd	xmm3, xmm1, 2
		movss	xmm4, xmm3
		pshufd	xmm5, xmm2, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[esi+8], xmm3
		movss	[esi+0x18], xmm4
		pshufd	xmm3, xmm1, 1
		movss	xmm4, xmm3
		subss	xmm3, xmm2
		addss	xmm4, xmm2
		movss	[esi+0x1C], xmm3
		movss	[esi+0x14], xmm4
		mov		eax, esi
		pop		esi
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
		movss	xmm3, [ecx]
		shufps	xmm3, xmm3, 0xC0
		mulps	xmm3, xmm0
		movss	xmm4, [ecx+4]
		shufps	xmm4, xmm4, 0xC0
		mulps	xmm4, xmm1
		addps	xmm3, xmm4
		movss	xmm4, [ecx+8]
		shufps	xmm4, xmm4, 0xC0
		mulps	xmm4, xmm2
		addps	xmm4, xmm3
		movss	xmm3, [ecx+0xC]
		movups	[ecx], xmm4
		shufps	xmm3, xmm3, 0xC0
		mulps	xmm3, xmm0
		movss	xmm4, [ecx+0x10]
		shufps	xmm4, xmm4, 0xC0
		mulps	xmm4, xmm1
		addps	xmm3, xmm4
		movss	xmm4, [ecx+0x14]
		shufps	xmm4, xmm4, 0xC0
		mulps	xmm4, xmm2
		addps	xmm4, xmm3
		movss	xmm3, [ecx+0x18]
		movups	[ecx+0xC], xmm4
		shufps	xmm3, xmm3, 0xC0
		mulps	xmm0, xmm3
		movss	xmm3, [ecx+0x1C]
		shufps	xmm3, xmm3, 0xC0
		mulps	xmm1, xmm3
		movss	xmm3, [ecx+0x20]
		shufps	xmm3, xmm3, 0xC0
		mulps	xmm2, xmm3
		addps	xmm0, xmm1
		addps	xmm0, xmm2
		movq	qword ptr [ecx+0x18], xmm0
		movhlps	xmm1, xmm0
		movss	[ecx+0x20], xmm1
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

void NiMatrix33::Dump()
{
	PrintDebug("%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n", cr[0][0], cr[1][0], cr[2][0], cr[0][1], cr[1][1], cr[2][1], cr[0][2], cr[1][2], cr[2][2]);
}

__declspec(naked) void __fastcall NiQuaternion::operator=(const NiMatrix33 &mat)
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
		retn
	}
}

__declspec(naked) void __fastcall NiQuaternion::operator=(const NiVector3 &vec)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		movups	xmm0, [edx]
		movss	xmm1, kFltHalf
		shufps	xmm1, xmm1, 0x40
		mulps	xmm0, xmm1
		movups	[esi], xmm0
		sub		esp, 0x10
		call	GetSinCos
		movq	qword ptr [esp], xmm0
		movss	xmm0, [esi+4]
		call	GetSinCos
		movq	qword ptr [esp+8], xmm0
		movss	xmm0, [esi+8]
		call	GetSinCos
		movq	xmm1, qword ptr [esp+8]
		movq	xmm2, qword ptr [esp]
		add		esp, 0x10
		pshufd	xmm3, xmm0, 0x11
		pshufd	xmm4, xmm1, 0x41
		mulps	xmm3, xmm4
		pshufd	xmm0, xmm3, 0xE0
		pshufd	xmm1, xmm2, 0x51
		mulps	xmm0, xmm1
		pshufd	xmm1, xmm3, 0xB5
		shufps	xmm2, xmm2, 4
		mulps	xmm1, xmm2
		xorps	xmm1, kSSEChangeSignMaskPD
		addps	xmm0, xmm1
		movups	[esi], xmm0
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall NiQuaternion::operator=(const AxisAngle &axisAngle)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		movss	xmm0, [edx+0xC]
		mulss	xmm0, kFltHalf
		call	GetSinCos
		pshufd	xmm1, xmm0, 1
		shufps	xmm0, xmm0, 2
		movups	xmm2, [edx]
		shufps	xmm2, xmm2, 0x90
		mulps	xmm0, xmm2
		movss	xmm0, xmm1
		movups	[esi], xmm0
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall NiQuaternion::operator*=(const NiQuaternion &rhs)
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [edx]
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
		comiss	xmm1, xmm2
		jz		zeroLen
		sqrtss	xmm2, xmm1
		movss	xmm1, kFltOne
		divss	xmm1, xmm2
		shufps	xmm1, xmm1, 0
		mulps	xmm0, xmm1
		movups	[eax], xmm0
		retn
	zeroLen:
		movups	[eax], xmm2
		mov		dword ptr [eax], 0x3F800000
        retn
    }
}

__declspec(naked) void __fastcall NiQuaternion::ToEulerYPR(NiVector3 &ypr) const
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		movups	xmm7, [esi]
		pshufd	xmm0, xmm7, 0x98
		pshufd	xmm1, xmm7, 0x9D
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, kSSEChangeSignMaskPS0
		addss	xmm1, kFltOne
		call	dATan2
		movss	[edi], xmm0
		pshufd	xmm0, xmm7, 0xC
		pshufd	xmm1, xmm7, 6
		mulps	xmm0, xmm1
		hsubps	xmm0, xmm0
		addss	xmm0, xmm0
		movss	xmm1, xmm0
		andps	xmm1, kSSERemoveSignMaskPS
		comiss	xmm1, kFltOne
		jnb		invSinP
		call	dASin
		jmp		doneY
	invSinP:
		pxor	xmm2, xmm2
		comiss	xmm0, xmm2
		movss	xmm0, kFltPId2
		ja		doneY
		xorps	xmm0, kSSEChangeSignMaskPS0
	doneY:
		movss	[edi+4], xmm0
		pshufd	xmm0, xmm7, 0xE4
		pshufd	xmm1, xmm7, 0xEB
		mulps	xmm0, xmm1
		haddps	xmm0, xmm0
		addps	xmm0, xmm0
		pshufd	xmm1, xmm0, 1
		xorps	xmm1, kSSEChangeSignMaskPS0
		addss	xmm1, kFltOne
		call	dATan2
		movss	[edi+8], xmm0
		pop		edi
		pop		esi
		retn
	}
}

void NiQuaternion::lerp(const NiQuaternion &qb, float t)
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

void NiQuaternion::slerp(const NiQuaternion &qb, float t)
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
	float sinHalfTheta = sqrtf(1.0F - cosHalfTheta * cosHalfTheta);
	if (sinHalfTheta < 0.001F)
	{
		*this += qb;
		*this *= 0.5F;
		return;
	}
	float halfTheta = dACos(cosHalfTheta);
	sinHalfTheta = 1.0F / sinHalfTheta;
	*this *= dSin((1.0F - t) * halfTheta) * sinHalfTheta;
	*this += qb * (dSin(t * halfTheta) * sinHalfTheta);
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