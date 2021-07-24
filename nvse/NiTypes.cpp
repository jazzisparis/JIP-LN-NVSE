#include "nvse/NiTypes.h"

void __fastcall NiMatrix33::ExtractAngles(NiVector3 *outAngles)
{
	float rotY = cr[0][2];
	if (abs(rotY) < 1.0F)
	{
		outAngles->x = -atan2(-cr[1][2], cr[2][2]);
		outAngles->y = -asin(rotY);
		outAngles->z = -atan2(-cr[0][1], cr[0][0]);
	}
	else
	{
		float rotX = atan2(cr[1][0], cr[1][1]);
		if (rotY > 0)
		{
			outAngles->x = rotX;
			outAngles->y = -1.5707963F;
		}
		else
		{
			outAngles->x = -rotX;
			outAngles->y = 1.5707963F;
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

__declspec(naked) void __fastcall NiMatrix33::Inverse(NiMatrix33 *mat)
{
	__asm
	{
		test	edx, edx
		cmovz	edx, ecx
		sub		esp, 0x20
		movups	xmm0, [edx]
		movups	[esp], xmm0
		movups	xmm0, [edx+0x10]
		movups	[esp+0x10], xmm0
		movss	xmm2, [edx+0x20]
		movss	xmm0, [esp+0x10]
		mulss	xmm0, xmm2
		movss	xmm1, [esp+0x14]
		mulss	xmm1, [esp+0x1C]
		subss	xmm0, xmm1
		movss	[ecx], xmm0
		movss	xmm0, [esp+8]
		mulss	xmm0, [esp+0x1C]
		movss	xmm1, [esp+4]
		mulss	xmm1, xmm2
		subss	xmm0, xmm1
		movss	[ecx+4], xmm0
		movss	xmm0, [esp+4]
		mulss	xmm0, [esp+0x14]
		movss	xmm1, [esp+8]
		mulss	xmm1, [esp+0x10]
		subss	xmm0, xmm1
		movss	[ecx+8], xmm0
		movss	xmm0, [esp+0x14]
		mulss	xmm0, [esp+0x18]
		movss	xmm1, [esp+0xC]
		mulss	xmm1, xmm2
		subss	xmm0, xmm1
		movss	[ecx+0xC], xmm0
		movss	xmm0, [esp]
		mulss	xmm0, xmm2
		movss	xmm1, [esp+8]
		mulss	xmm1, [esp+0x18]
		subss	xmm0, xmm1
		movss	[ecx+0x10], xmm0
		movss	xmm0, [esp+8]
		mulss	xmm0, [esp+0xC]
		movss	xmm1, [esp]
		mulss	xmm1, [esp+0x14]
		subss	xmm0, xmm1
		movss	[ecx+0x14], xmm0
		movss	xmm0, [esp+0xC]
		mulss	xmm0, [esp+0x1C]
		movss	xmm1, [esp+0x10]
		mulss	xmm1, [esp+0x18]
		subss	xmm0, xmm1
		movss	[ecx+0x18], xmm0
		movss	xmm0, [esp+4]
		mulss	xmm0, [esp+0x18]
		movss	xmm1, [esp]
		mulss	xmm1, [esp+0x1C]
		subss	xmm0, xmm1
		movss	[ecx+0x1C], xmm0
		movss	xmm0, [esp]
		mulss	xmm0, [esp+0x10]
		movss	xmm1, [esp+4]
		mulss	xmm1, [esp+0xC]
		subss	xmm0, xmm1
		movss	[ecx+0x20], xmm0
		add		esp, 0x20
		retn
	}
}

void NiVector3::ToQuaternion(NiQuaternion &quaternion)
{
	double hlf = (double)z * 0.5;
	double cY = cos(hlf);
	double sY = sin(hlf);
	hlf = (double)y * 0.5;
	double cP = cos(hlf);
	double sP = sin(hlf);
	hlf = (double)x * 0.5;
	double cR = cos(hlf);
	double sR = sin(hlf);
	quaternion.w = cY * cP * cR + sY * sP * sR;
	quaternion.x = cY * cP * sR - sY * sP * cR;
	quaternion.y = cY * sP * cR + sY * cP * sR;
	quaternion.z = sY * cP * cR - cY * sP * sR;
}

__declspec(naked) void NiVector3::MultiplyMatrixVector(NiMatrix33 &mat, NiVector3 &vec)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [esp+8]
		movss	xmm0, [edx]
		movss	xmm1, [edx+4]
		movss	xmm2, [edx+8]
		movss	xmm3, [eax]
		mulss	xmm3, xmm0
		movss	xmm4, [eax+4]
		mulss	xmm4, xmm1
		addss	xmm3, xmm4
		movss	xmm4, [eax+8]
		mulss	xmm4, xmm2
		addss	xmm3, xmm4
		movss	[ecx], xmm3
		movss	xmm3, [eax+0xC]
		mulss	xmm3, xmm0
		movss	xmm4, [eax+0x10]
		mulss	xmm4, xmm1
		addss	xmm3, xmm4
		movss	xmm4, [eax+0x14]
		mulss	xmm4, xmm2
		addss	xmm3, xmm4
		movss	[ecx+4], xmm3
		movss	xmm3, [eax+0x18]
		mulss	xmm3, xmm0
		movss	xmm4, [eax+0x1C]
		mulss	xmm4, xmm1
		addss	xmm3, xmm4
		movss	xmm4, [eax+0x20]
		mulss	xmm4, xmm2
		addss	xmm3, xmm4
		movss	[ecx+8], xmm3
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

void NiQuaternion::EulerYPR(NiVector3 &ypr)
{
	ypr.x = atan2(2.0 * (w * x + y * z), 1.0 - (2.0 * (x * x + y * y)));
	double sinp = 2.0 * (w * y - z * x);
	if (fabs(sinp) < 1)
		ypr.y = asin(sinp);
	else
		ypr.y = (sinp > 0) ? kDblPId2 : -kDblPId2;
	ypr.z = atan2(2.0 * (w * z + x * y), 1.0 - (2.0 * (y * y + z * z)));
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