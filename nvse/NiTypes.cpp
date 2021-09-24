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

void __fastcall NiMatrix33::ExtractAnglesLocal(NiVector3 *outAngles)
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

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrix(NiVector3 *rot)
{
	__asm
	{
		push	ecx
		fld		dword ptr [edx]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm1, [esp]
		fstp	dword ptr [esp]
		movss	xmm0, [esp]
		fld		dword ptr [edx+4]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm3, [esp]
		fstp	dword ptr [esp]
		movss	xmm2, [esp]
		fld		dword ptr [edx+8]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm5, [esp]
		fstp	dword ptr [esp]
		movss	xmm4, [esp]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss	[ecx], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss	[ecx+4], xmm6
		pxor	xmm6, xmm6
		subss	xmm6, xmm2
		movss	[ecx+8], xmm6
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
		movss	xmm6, xmm1
		mulss	xmm6, xmm3
		movss	[ecx+0x20], xmm6
		mov		eax, ecx
		pop		ecx
		retn
	}
}

__declspec(naked) NiMatrix33* __fastcall NiMatrix33::RotationMatrixLocal(NiVector3 *rot)
{
	__asm
	{
		push	ecx
		fld		dword ptr [edx]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm1, [esp]
		fstp	dword ptr [esp]
		movss	xmm0, [esp]
		fld		dword ptr [edx+4]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm3, [esp]
		fstp	dword ptr [esp]
		movss	xmm2, [esp]
		fld		dword ptr [edx+8]
		fsincos
		fstp	dword ptr [esp]
		movss	xmm5, [esp]
		fstp	dword ptr [esp]
		movss	xmm4, [esp]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movss	[ecx], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movss	[ecx+0xC], xmm6
		pxor	xmm6, xmm6
		subss	xmm6, xmm2
		movss	[ecx+0x18], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movss	[ecx+4], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movss	[ecx+0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movss	[ecx+0x1C], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movss	[ecx+8], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movss	[ecx+0x14], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm3
		movss	[ecx+0x20], xmm6
		mov		eax, ecx
		pop		ecx
		retn
	}
}

__declspec(naked) NiMatrix33 *NiMatrix33::MultiplyMatrices(NiMatrix33 *matA, NiMatrix33 *matB)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [esp+8]
		movss	xmm0, [eax]
		mulss	xmm0, [edx]
		movss	xmm1, [eax+4]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movss	[ecx], xmm0
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [edx]
		movss	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movss	[ecx+0xC], xmm0
		movss	xmm0, [eax+0x18]
		mulss	xmm0, [edx]
		movss	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movss	[ecx+0x18], xmm0
		movss	xmm0, [eax]
		mulss	xmm0, [edx+4]
		movss	xmm1, [eax+4]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movss	[ecx+4], xmm0
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [edx+4]
		movss	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movss	[ecx+0x10], xmm0
		movss	xmm0, [eax+0x18]
		mulss	xmm0, [edx+4]
		movss	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movss	[ecx+0x1C], xmm0
		movss	xmm0, [eax]
		mulss	xmm0, [edx+8]
		movss	xmm1, [eax+4]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movss	xmm1, [eax+8]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movss	[ecx+8], xmm0
		movss	xmm0, [eax+0xC]
		mulss	xmm0, [edx+8]
		movss	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movss	[ecx+0x14], xmm0
		movss	xmm0, [eax+0x18]
		mulss	xmm0, [edx+8]
		movss	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movss	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movss	[ecx+0x20], xmm0
		mov		eax, ecx
		retn	8
	}
}

__declspec(naked) void __fastcall NiMatrix33::Rotate(NiVector3 *rot)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x48
		push	esi
		mov		esi, edx
		fld1
		cmp		dword ptr [esi+8], 0
		jz		doneZ
		lea		eax, [ebp-0x48]
		fld		dword ptr [esi+8]
		fchs
		fsincos
		fst		dword ptr [eax]
		fstp	dword ptr [eax+0x10]
		fst		dword ptr [eax+4]
		fchs
		fstp	dword ptr [eax+0xC]
		fst		dword ptr [eax+0x20]
		xor		edx, edx
		mov		[eax+8], edx
		mov		[eax+0x14], edx
		mov		[eax+0x18], edx
		mov		[eax+0x1C], edx
		push	eax
		lea		eax, [ebp-0x24]
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		movups	xmm0, [ecx+0x10]
		movups	[eax+0x10], xmm0
		mov		edx, [ecx+0x20]
		mov		[eax+0x20], edx
		push	eax
		call	NiMatrix33::MultiplyMatrices
	doneZ:
		cmp		dword ptr [esi+4], 0
		jz		doneY
		lea		eax, [ebp-0x48]
		fld		dword ptr [esi+4]
		fchs
		fsincos
		fst		dword ptr [eax]
		fstp	dword ptr [eax+0x20]
		fst		dword ptr [eax+0x18]
		fchs
		fstp	dword ptr [eax+8]
		fst		dword ptr [eax+0x10]
		xor		edx, edx
		mov		[eax+4], edx
		mov		[eax+0xC], edx
		mov		[eax+0x14], edx
		mov		[eax+0x1C], edx
		push	eax
		lea		eax, [ebp-0x24]
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		movups	xmm0, [ecx+0x10]
		movups	[eax+0x10], xmm0
		mov		edx, [ecx+0x20]
		mov		[eax+0x20], edx
		push	eax
		call	NiMatrix33::MultiplyMatrices
	doneY:
		cmp		dword ptr [esi], 0
		jz		doneX
		lea		eax, [ebp-0x48]
		fld		dword ptr [esi]
		fchs
		fsincos
		fst		dword ptr [eax+0x10]
		fstp	dword ptr [eax+0x20]
		fst		dword ptr [eax+0x14]
		fchs
		fstp	dword ptr [eax+0x1C]
		fst		dword ptr [eax]
		xor		edx, edx
		mov		[eax+4], edx
		mov		[eax+8], edx
		mov		[eax+0xC], edx
		mov		[eax+0x18], edx
		push	eax
		lea		eax, [ebp-0x24]
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		movups	xmm0, [ecx+0x10]
		movups	[eax+0x10], xmm0
		mov		edx, [ecx+0x20]
		mov		[eax+0x20], edx
		push	eax
		call	NiMatrix33::MultiplyMatrices
	doneX:
		fstp	st
		pop		esi
		leave
		retn
	}
}

__declspec(naked) void __fastcall NiMatrix33::Transpose(NiMatrix33 *resMat)
{
	__asm
	{
		test	edx, edx
		cmovz	edx, ecx
		movups	xmm0, [ecx]
		movups	xmm1, [ecx+0x10]
		shufps	xmm0, xmm0, 0x6C
		shufps	xmm1, xmm1, 0x6C
		movaps	xmm2, xmm1
		movhlps	xmm2, xmm0
		shufps	xmm0, xmm2, 0x64
		shufps	xmm1, xmm2, 0xC4
		movups	[edx], xmm0
		movups	[edx+0x10], xmm1
		mov		eax, [ecx+0x20]
		mov		[edx+0x20], eax
		retn
	}
}

void NiMatrix33::Dump()
{
	PrintDebug("%.4f\t%.4f\t%.4f\n%.4f\t%.4f\t%.4f\n%.4f\t%.4f\t%.4f\n", cr[0][0], cr[1][0], cr[2][0], cr[0][1], cr[1][1], cr[2][1], cr[0][2], cr[1][2], cr[2][2]);
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

__declspec(naked) void NiVector3::MultiplyMatrixVector(NiMatrix33 *mat, NiVector3 *vec)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [esp+8]
		movups	xmm0, [edx]
		andps	xmm0, kSSEDiscard4thPS
		movups	xmm1, [eax]
		mulps	xmm1, xmm0
		movhlps	xmm2, xmm1
		addss	xmm2, xmm1
		psrlq	xmm1, 0x20
		addss	xmm2, xmm1
		movss	[ecx], xmm2
		movups	xmm1, [eax+0xC]
		mulps	xmm1, xmm0
		movhlps	xmm2, xmm1
		addss	xmm2, xmm1
		psrlq	xmm1, 0x20
		addss	xmm2, xmm1
		movss	[ecx+4], xmm2
		movups	xmm1, [eax+0x18]
		mulps	xmm1, xmm0
		movhlps	xmm2, xmm1
		addss	xmm2, xmm1
		psrlq	xmm1, 0x20
		addss	xmm2, xmm1
		movss	[ecx+8], xmm2
		retn	8
	}
}

__declspec(naked) void NiVector3::ColumnMultiply(NiMatrix33 *rotMatrix, UInt32 whichColumn)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [esp+8]
		lea		eax, [eax+edx*4]
		movss	xmm0, [ecx]
		mulss	xmm0, [eax]
		movss	[ecx], xmm0
		movss	xmm0, [ecx+4]
		mulss	xmm0, [eax+0xC]
		movss	[ecx+4], xmm0
		movss	xmm0, [ecx+8]
		mulss	xmm0, [eax+0x18]
		movss	[ecx+8], xmm0
		retn	8
	}
}

__declspec(naked) void NiVector3::Normalize()
{
    __asm
    {
		movss	xmm0, [ecx+8]
		movlhps	xmm0, xmm0
		movlps	xmm0, [ecx]
		movaps	xmm1, xmm0
		mulps	xmm1, xmm1
		movhlps	xmm2, xmm1
		addss	xmm2, xmm1
		psrlq	xmm1, 0x20
		addss	xmm2, xmm1
		rsqrtss	xmm1, xmm2
		pxor	xmm2, xmm2
		comiss	xmm1, xmm2
        jz		zeroLen
		shufps	xmm1, xmm1, 0
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
		andps	xmm5, kSSERemoveSignMask
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

void NiQuaternion::RotationMatrix(NiMatrix33 &rotMatrix)
{
	float x2 = x * x, y2 = y * y, z2 = z * z;
	rotMatrix.cr[0][0] = 1 - 2 * (y2 + z2);
	rotMatrix.cr[0][1] = (2 * x * y) + (2 * z * w);
	rotMatrix.cr[0][2] = (2 * x * z) - (2 * y * w);
	rotMatrix.cr[1][0] = (2 * x * y) - (2 * z * w);
	rotMatrix.cr[1][1] = 1 - 2 * (x2 + z2);
	rotMatrix.cr[1][2] = (2 * y * z) + (2 * x * w);
	rotMatrix.cr[2][0] = (2 * x * z) + (2 * y * w);
	rotMatrix.cr[2][1] = (2 * y * z) - (2 * x * w);
	rotMatrix.cr[2][2] = 1 - 2 * (x2 + y2);
}

__declspec(naked) float __vectorcall Vector3Length(AlignedVector4 *inVec)
{
	__asm
	{
		movaps	xmm0, [ecx]
		mulps	xmm0, xmm0
		movhlps	xmm1, xmm0
		addss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
		retn
	}
}

__declspec(naked) float __vectorcall Vector3Distance(NiVector3 *vec1, NiVector3 *vec2)
{
	__asm
	{
		movss	xmm0, [ecx+8]
		movlhps	xmm0, xmm0
		movlps	xmm0, [ecx]
		movss	xmm1, [edx+8]
		movlhps	xmm1, xmm1
		movlps	xmm1, [edx]
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		movhlps	xmm1, xmm0
		addss	xmm1, xmm0
		psrlq	xmm0, 0x20
		addss	xmm1, xmm0
		sqrtss	xmm0, xmm1
		retn
	}
}