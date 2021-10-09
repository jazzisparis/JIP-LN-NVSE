#include "nvse/NiTypes.h"

void __fastcall NiMatrix33::ExtractAngles(NiVector3 *outAngles)
{
	float rotY = cr[0][2];
	if (abs(rotY) < 1.0F)
	{
		outAngles->x = -atan2f(-cr[1][2], cr[2][2]);
		outAngles->y = -asinf(rotY);
		outAngles->z = -atan2f(-cr[0][1], cr[0][0]);
	}
	else
	{
		float rotX = atan2f(cr[1][0], cr[1][1]);
		if (rotY > 0)
		{
			outAngles->x = rotX;
			outAngles->y = -kFltPId2;
		}
		else
		{
			outAngles->x = -rotX;
			outAngles->y = kFltPId2;
		}
		outAngles->z = 0;
	}
}

void __fastcall NiMatrix33::ExtractAnglesInv(NiVector3 *outAngles)
{
	float rotY = cr[2][0];
	if (abs(rotY) < 1.0F)
	{
		outAngles->x = -atan2f(-cr[2][1], cr[2][2]);
		outAngles->y = -asinf(rotY);
		outAngles->z = -atan2f(-cr[1][0], cr[0][0]);
	}
	else
	{
		float rotX = atan2f(cr[0][1], cr[1][1]);
		if (rotY > 0)
		{
			outAngles->x = rotX;
			outAngles->y = -kFltPId2;
		}
		else
		{
			outAngles->x = -rotX;
			outAngles->y = kFltPId2;
		}
		outAngles->z = 0;
	}
}

__declspec(naked) void RotationMatrixX()
{
	__asm
	{
		fld		dword ptr [edx]
		fsincos
		fst		dword ptr [eax+0x10]
		fstp	dword ptr [eax+0x20]
		fst		dword ptr [eax+0x14]
		fchs
		fstp	dword ptr [eax+0x1C]
		retn
	}
}
__declspec(naked) void RotationMatrixY()
{
	__asm
	{
		fld		dword ptr [edx+4]
		fsincos
		fst		dword ptr [eax]
		fstp	dword ptr [eax+0x20]
		fst		dword ptr [eax+0x18]
		fchs
		fstp	dword ptr [eax+8]
		retn
	}
}
__declspec(naked) void RotationMatrixXY()
{
	__asm
	{
		fld		dword ptr [edx]
		fsincos
		fstp	dword ptr [eax+0x10]
		fstp	dword ptr [eax+0x1C]
		fld		dword ptr [edx+4]
		fsincos
		fstp	dword ptr [eax]
		fstp	dword ptr [eax+8]
		movss	xmm0, [eax+0x1C]
		movss	xmm1, [eax+0x10]
		movss	xmm2, [eax+8]
		movss	xmm3, [eax]
		xor		byte ptr [eax+0xB], 0x80
		movss	xmm4, xmm0
		mulss	xmm4, xmm2
		movss	[eax+0xC], xmm4
		mulss	xmm0, xmm3
		movss	[eax+0x14], xmm0
		movss	xmm4, xmm1
		mulss	xmm4, xmm2
		movss	[eax+0x18], xmm4
		xor		byte ptr [eax+0x1F], 0x80
		mulss	xmm1, xmm3
		movss	[eax+0x20], xmm1
		retn
	}
}
__declspec(naked) void RotationMatrixZ()
{
	__asm
	{
		fld		dword ptr [edx+8]
		fsincos
		fst		dword ptr [eax]
		fstp	dword ptr [eax+0x10]
		fst		dword ptr [eax+4]
		fchs
		fstp	dword ptr [eax+0xC]
		retn
	}
}
__declspec(naked) void RotationMatrixXZ()
{
	__asm
	{
		fld		dword ptr [edx]
		fsincos
		fstp	dword ptr [eax+0x20]
		fstp	dword ptr [eax+0x14]
		fld		dword ptr [edx+8]
		fsincos
		fstp	dword ptr [eax]
		fstp	dword ptr [eax+4]
		movss	xmm0, [eax+0x14]
		movss	xmm1, [eax+0x20]
		movss	xmm2, [eax+4]
		movss	xmm3, [eax]
		movss	xmm4, xmm1
		mulss	xmm4, xmm2
		movss	[eax+0xC], xmm4
		xor		byte ptr [eax+0xF], 0x80
		mulss	xmm1, xmm3
		movss	[eax+0x10], xmm1
		movss	xmm4, xmm0
		mulss	xmm4, xmm2
		movss	[eax+0x18], xmm4
		mulss	xmm0, xmm3
		movss	[eax+0x1C], xmm0
		xor		byte ptr [eax+0x1F], 0x80
		retn
	}
}
__declspec(naked) void RotationMatrixYZ()
{
	__asm
	{
		fld		dword ptr [edx+4]
		fsincos
		fstp	dword ptr [eax+0x20]
		fstp	dword ptr [eax+8]
		fld		dword ptr [edx+8]
		fsincos
		fstp	dword ptr [eax+0x10]
		fstp	dword ptr [eax+0xC]
		movss	xmm0, [eax+8]
		movss	xmm1, [eax+0x20]
		movss	xmm2, [eax+0xC]
		movss	xmm3, [eax+0x10]
		movss	xmm4, xmm1
		mulss	xmm4, xmm3
		movss	[eax], xmm4
		mulss	xmm1, xmm2
		movss	[eax+4], xmm1
		xor		byte ptr [eax+0xB], 0x80
		xor		byte ptr [eax+0xF], 0x80
		movss	xmm4, xmm0
		mulss	xmm4, xmm3
		movss	[eax+0x18], xmm4
		mulss	xmm0, xmm2
		movss	[eax+0x1C], xmm0
		retn
	}
}
__declspec(naked) void RotationMatrixXYZ()
{
	__asm
	{
		fld		dword ptr [edx]
		fsincos
		fstp	dword ptr [eax+4]
		fstp	dword ptr [eax]
		fld		dword ptr [edx+4]
		fsincos
		fstp	dword ptr [eax+0xC]
		fstp	dword ptr [eax+8]
		fld		dword ptr [edx+8]
		fsincos
		fstp	dword ptr [eax+0x14]
		fstp	dword ptr [eax+0x10]
		movss	xmm0, [eax]
		movss	xmm1, [eax+4]
		movss	xmm2, [eax+8]
		movss	xmm3, [eax+0xC]
		movss	xmm4, [eax+0x10]
		movss	xmm5, [eax+0x14]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss	[eax], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss	[eax+4], xmm6
		xor		byte ptr [eax+0xB], 0x80
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movss	[eax+0xC], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movss	[eax+0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movss	[eax+0x14], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movss	[eax+0x18], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movss	[eax+0x1C], xmm6
		mulss	xmm1, xmm3
		movss	[eax+0x20], xmm1
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrix(NiVector3 *rot)
{
	static alignas(16) const UInt32 kNEQZeroMask[] = {0x80000000, 0x80, 0x80, 0};
	static const void *kRotationMatrixJmpTable[] = {RotationMatrixX, RotationMatrixY, RotationMatrixXY, RotationMatrixZ, RotationMatrixXZ, RotationMatrixYZ, RotationMatrixXYZ};
	__asm
	{
		mov		eax, ecx
		mov		ecx, 0x3F800000
		movd	xmm0, ecx
		movups	[eax], xmm0
		movups	[eax+0x10], xmm0
		mov		[eax+0x20], ecx
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
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrixInv(NiVector3 *rot)
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

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::MultiplyMatrices(NiMatrix33 *matB)
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

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::Rotate(NiVector3 *rot)
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

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::FromQuaternion(NiQuaternion *qt)
{
	__asm
	{
		mov		eax, ecx
		movups	xmm0, [edx]
		movaps	xmm4, xmm0
		addps	xmm4, xmm0
		pshufd	xmm1, xmm0, 0x55
		mulps	xmm1, xmm4
		pshufd	xmm2, xmm0, 0xAA
		mulps	xmm2, xmm4
		pshufd	xmm3, xmm0, 3
		mulss	xmm3, xmm4
		mulps	xmm0, xmm4
		subss	xmm0, kFltOne
		pshufd	xmm4, xmm0, 1
		addss	xmm4, xmm0
		movss	[eax], xmm4
		pshufd	xmm4, xmm0, 2
		addss	xmm4, xmm0
		movss	[eax+0x10], xmm4
		pshufd	xmm4, xmm0, 3
		addss	xmm4, xmm0
		movss	[eax+0x20], xmm4
		pshufd	xmm4, xmm1, 2
		movss	xmm5, xmm4
		subss	xmm4, xmm3
		addss	xmm5, xmm3
		movss	[eax+4], xmm4
		movss	[eax+0xC], xmm5
		pshufd	xmm4, xmm1, 3
		movss	xmm5, xmm4
		subss	xmm4, xmm2
		addss	xmm5, xmm2
		movss	[eax+8], xmm5
		movss	[eax+0x18], xmm4
		pshufd	xmm4, xmm2, 3
		movss	xmm5, xmm4
		subss	xmm4, xmm1
		addss	xmm5, xmm1
		movss	[eax+0x14], xmm4
		movss	[eax+0x1C], xmm5
		retn
	}
}

void NiMatrix33::Dump()
{
	PrintDebug("%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n", cr[0][0], cr[1][0], cr[2][0], cr[0][1], cr[1][1], cr[2][1], cr[0][2], cr[1][2], cr[2][2]);
}

void NiVector3::ToQuaternion(NiQuaternion &quaternion)
{
	float hlf = z * 0.5F;
	float cY = cosf(hlf);
	float sY = sinf(hlf);
	hlf = y * 0.5F;
	float cP = cosf(hlf);
	float sP = sinf(hlf);
	hlf = x * 0.5F;
	float cR = cosf(hlf);
	float sR = sinf(hlf);
	quaternion.w = cY * cP * cR + sY * sP * sR;
	quaternion.x = cY * cP * sR - sY * sP * cR;
	quaternion.y = cY * sP * cR + sY * cP * sR;
	quaternion.z = sY * cP * cR - cY * sP * sR;
}

__declspec(naked) NiVector3* __fastcall NiVector3::MultiplyMatrix(NiMatrix33 *mat)
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

__declspec(naked) NiVector3* __fastcall NiVector3::MultiplyMatrixRow(NiMatrix33 *rotMatrix, UInt32 whichRow)
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
		rsqrtss	xmm1, xmm1
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

__declspec(naked) NiVector3* __fastcall NiVector3::CrossProduct(NiVector3 *vB)
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

__declspec(naked) void __fastcall NiQuaternion::operator=(const NiMatrix33 &mat)
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
		movss	[ecx], xmm5
		movss	xmm0, [edx+0x1C]
		subss	xmm0, [edx+0x14]
		movss	[ecx+4], xmm0
		movss	xmm0, [edx+8]
		subss	xmm0, [edx+0x18]
		movss	[ecx+8], xmm0
		movss	xmm0, [edx+0xC]
		subss	xmm0, [edx+4]
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
		movss	xmm0, [edx+8]
		subss	xmm0, [edx+0x18]
		movss	[ecx], xmm0
		movss	xmm0, [edx+0xC]
		addss	xmm0, [edx+4]
		movss	[ecx+4], xmm0
		movss	[ecx+8], xmm5
		movss	xmm0, [edx+0x1C]
		addss	xmm0, [edx+0x14]
		movss	[ecx+0xC], xmm0
		jmp		done
	t_xz:
		comiss	xmm0, xmm2
		jb		t_z
		addss	xmm5, xmm0
		subss	xmm5, xmm1
		subss	xmm5, xmm2
		movss	xmm0, [edx+0x1C]
		subss	xmm0, [edx+0x14]
		movss	[ecx], xmm0
		movss	[ecx+4], xmm5
		movss	xmm0, [edx+0xC]
		addss	xmm0, [edx+4]
		movss	[ecx+8], xmm0
		movss	xmm0, [edx+0x18]
		addss	xmm0, [edx+8]
		movss	[ecx+0xC], xmm0
		jmp		done
	t_z:
		subss	xmm5, xmm0
		subss	xmm5, xmm1
		addss	xmm5, xmm2
		movss	xmm0, [edx+0xC]
		subss	xmm0, [edx+4]
		movss	[ecx], xmm0
		movss	xmm0, [edx+0x18]
		addss	xmm0, [edx+8]
		movss	[ecx+4], xmm0
		movss	xmm0, [edx+0x1C]
		addss	xmm0, [edx+0x14]
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

__declspec(naked) void __fastcall NiQuaternion::operator*=(const NiQuaternion &rhs)
{
	__asm
	{
		movups	xmm0, [ecx]
		movups	xmm1, [edx]
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
		rsqrtss	xmm2, xmm1
		shufps	xmm2, xmm2, 0
		mulps	xmm2, xmm0
		movups	[eax], xmm2
		retn
	zeroLen:
		movups	[eax], xmm2
		mov		dword ptr [eax], 0x3F800000
        retn
    }
}

void NiQuaternion::EulerYPR(NiVector3 &ypr)
{
	ypr.x = atan2f(2.0F * (w * x + y * z), 1.0F - (2.0F * (x * x + y * y)));
	float sinp = 2.0F * (w * y - z * x);
	if (abs(sinp) < 1.0F)
		ypr.y = asinf(sinp);
	else
		ypr.y = (sinp > 0) ? kFltPId2 : -kFltPId2;
	ypr.z = atan2f(2.0F * (w * z + x * y), 1.0F - (2.0F * (y * y + z * z)));
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