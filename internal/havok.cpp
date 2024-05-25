#include "internal/havok.h"

__declspec(naked) hkQuaternion& __vectorcall hkQuaternion::FromEulerPRY(__m128 pry)
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
		shufps	xmm0, xmm0, 0x39
		movaps	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) hkQuaternion& __fastcall hkQuaternion::FromRotationMatrix(const NiMatrix33 &mat)
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
		shufps	xmm3, xmm0, 0x30
		shufps	xmm0, xmm3, 0x29
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
		movaps	xmm2, xmm0
		unpcklps	xmm0, xmm3
		shufps	xmm0, xmm2, 0xE6
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
		shufps	xmm0, xmm2, 0xE9
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
		shufps	xmm0, xmm3, 0x89
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

__declspec(naked) hkQuaternion& __fastcall hkQuaternion::FromAxisAngle(const AxisAngle &axisAngle)
{
	__asm
	{
		movss	xmm0, [edx+0xC]
		mulss	xmm0, PS_V3_Half
		movups	xmm5, [edx]
		call	GetSinCos
		shufps	xmm0, xmm0, 0x80
		mulps	xmm0, xmm5
		shufps	xmm1, xmm0, 0x20
		shufps	xmm0, xmm1, 0x24
		movaps	[ecx], xmm0
		mov		eax, ecx
		retn
	}
}

__declspec(naked) __m128 __vectorcall hkQuaternion::MultiplyVector(__m128 vec) const
{
	__asm
	{
		andps	xmm0, PS_XYZ0Mask
		pshufd	xmm1, [ecx], 0x93
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

__declspec(naked) __m128 __vectorcall hkQuaternion::MultiplyQuaternion(const hkQuaternion &rhs) const
{
	__asm
	{
		pshufd	xmm1, [ecx], 0x93
		pshufd	xmm2, [edx], 0x93
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
		unpcklpd	xmm0, xmm4
		shufps	xmm0, xmm0, 0x39
		retn
	}
}

__declspec(naked) __m128 __vectorcall hkQuaternion::ToEulerPRY() const
{
	__asm
	{
		movaps	xmm6, [ecx]
		shufps	xmm6, xmm6, 0x93
		jmp		NiQuaternion::ToEulerPRY+3
	}
}

__declspec(naked) __m128 __vectorcall hkQuaternion::ToEulerYPR() const
{
	__asm
	{
		movaps	xmm6, [ecx]
		shufps	xmm6, xmm6, 0x93
		jmp		NiQuaternion::ToEulerYPR+3
	}
}

void hkQuaternion::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\t%.6f\n", x, y, z, w);
}

__declspec(naked) void __fastcall hkMatrix3x4::operator=(const NiMatrix33 &inMatrix)
{
	__asm
	{
		movaps	xmm1, PS_XYZ0Mask
		movups	xmm0, [edx]
		andps	xmm0, xmm1
		movaps	[ecx], xmm0
		movups	xmm0, [edx+0xC]
		andps	xmm0, xmm1
		movaps	[ecx+0x10], xmm0
		movups	xmm0, [edx+0x18]
		andps	xmm0, xmm1
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
		subss	xmm0, PS_V3_One
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
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm1, 3
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x10], xmm3
		movss	[ecx+4], xmm4
		pshufd	xmm3, xmm2, 3
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm1, 2
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x20], xmm4
		movss	[ecx+8], xmm3
		pshufd	xmm3, xmm2, 2
		movaps	xmm4, xmm3
		pshufd	xmm5, xmm1, 1
		subss	xmm3, xmm5
		addss	xmm4, xmm5
		movss	[ecx+0x24], xmm3
		movss	[ecx+0x18], xmm4
		retn
	}
}

__declspec(naked) __m128 __vectorcall hkMatrix3x4::MultiplyVector(__m128 vec) const
{
	__asm
	{
		movaps	xmm1, xmm0
		andps	xmm1, PS_XYZ0Mask
		xorps	xmm3, xmm3
		movaps	xmm0, [ecx]
		mulps	xmm0, xmm1
		haddps	xmm0, xmm3
		haddps	xmm0, xmm3
		movaps	xmm2, [ecx+0x10]
		mulps	xmm2, xmm1
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		unpcklps	xmm0, xmm2
		movaps	xmm2, [ecx+0x20]
		mulps	xmm2, xmm1
		haddps	xmm2, xmm3
		haddps	xmm2, xmm3
		unpcklpd	xmm0, xmm2
		retn
	}
}

__declspec(naked) __m128 __vectorcall hkMatrix3x4::MultiplyVectorInv(__m128 vec) const
{
	__asm
	{
		movaps	xmm3, xmm0
		andps	xmm3, PS_XYZ0Mask
		movaps	xmm0, [ecx]
		pshufd	xmm1, xmm3, 0xC0
		mulps	xmm0, xmm1
		movaps	xmm1, [ecx+0x10]
		pshufd	xmm2, xmm3, 0xD5
		mulps	xmm1, xmm2
		addps	xmm0, xmm1
		movaps	xmm1, [ecx+0x20]
		shufps	xmm3, xmm3, 0xEA
		mulps	xmm1, xmm3
		addps	xmm0, xmm1
		retn
	}
}

void hkMatrix3x4::Dump() const
{
	PrintDebug("%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n%.6f\t%.6f\t%.6f\n", cr[0][0], cr[1][0], cr[2][0], cr[0][1], cr[1][1], cr[2][1], cr[0][2], cr[1][2], cr[2][2]);
}

__declspec(naked) NiNode *hkpWorldObject::GetParentNode()
{
	__asm
	{
		mov		edx, [ecx+0x7C]
		mov		ecx, [ecx+0x78]
		ALIGN 16
	iterHead:
		dec		edx
		js		retnNULL
		mov		eax, [ecx]
		cmp		eax, 0x1267B70
		jz		foundCol
		cmp		eax, 0x11F4280
		jz		done
		add		ecx, 0x10
		jmp		iterHead
	foundCol:
		mov		ecx, [ecx+8]
		test	ecx, ecx
		jz		retnNULL
	done:
		mov		eax, [ecx+8]
		retn
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) TESObjectREFR *hkpWorldObject::GetParentRef()
{
	__asm
	{
		call	hkpWorldObject::GetParentNode
		mov		edx, kVtbl_BSFadeNode
		ALIGN 16
	iterHead:
		test	eax, eax
		jz		done
		mov		ecx, eax
		mov		eax, [eax+0x18]
		cmp		[ecx], edx
		jnz		iterHead
		mov		ecx, [ecx+0xCC]
		test	ecx, ecx
		jz		iterHead
		mov		eax, ecx
	done:
		retn
	}
}

__declspec(naked) void __fastcall bhkWorldObject::ApplyForce(const NiVector4 &forceVector)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF0
		sub		esp, 0x10
		mov		eax, esp
		push	ecx
		push	edx
		push	eax
		mov		eax, [ecx]
		call	dword ptr [eax+0xF0]
		movss	xmm0, kUnitConv
		shufps	xmm0, xmm0, 0x40
		mulps	xmm0, [eax]
		pop		edx
		movups	xmm1, [edx]
		andps	xmm1, PS_XYZ0Mask
		subps	xmm0, xmm1
		call	Normalize_V4
		movss	xmm1, [edx+0xC]
		mulss	xmm1, kUnitConv+4
		shufps	xmm1, xmm1, 0x40
		mulps	xmm0, xmm1
		pop		eax
		mov		ecx, [eax+8]
		lea		eax, [ecx+0x1B0]
		addps	xmm0, [eax]
		movaps	[eax], xmm0
		leave
		JMP_EAX(0xC9C1D0)
		ALIGN 8
	kUnitConv:
		EMIT_DW(0x40DFF8D6) EMIT_DW(0x3E124DD2)
	}
}

__declspec(naked) void __vectorcall hkpRigidBody::SetVelocity(__m128 vel, UInt32 flags)
{
	__asm
	{
		test	dl, 1
		jnz		doneCalc
		add		ecx, 0xF0
		call	hkMatrix3x4::MultiplyVectorInv
		sub		ecx, 0xF0
	doneCalc:
		mov		eax, edx
		and		al, 0x10
		lea		eax, [eax+ecx+0x1B0]
		test	dl, 2
		jz		doSet
		addps	xmm0, [eax]
	doSet:
		movaps	[eax], xmm0
		JMP_EAX(0xC9C1D0)
	}
}

void __fastcall hkpRigidBody::SetCollisionProperty(UInt32 propID, FltAndInt value)
{
	switch (propID)
	{
		case 0:
			friction = value.f;
			break;
		case 1:
			restitution = value.f;
			break;
		case 2:
			motion.motionState.linearDamping = value.f;
			break;
		case 3:
			motion.motionState.angularDamping = value.f;
			break;
		case 4:
		case 5:
		case 6:
			motion.inertia[propID - 4] = value.f;
			break;
		case 7:
			motion.bodyMassInv = value.f;
			break;
		case 8:
			if (motion.type != value.i)
				((bhkRigidBody*)object)->SetMotionType(value.i);
			break;
	}
}