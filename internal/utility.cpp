#include "internal/utility.h"
#include "nvse/GameAPI.h"

alignas(16) const HexFloat kPackedValues[] =
{
	PS_DUP_4(0x7FFFFFFFUL),
	PS_DUP_1(0x7FFFFFFFUL),
	PS_DUP_4(0x80000000UL),
	PS_DUP_1(0x80000000UL),
	PS_DUP_3(0xFFFFFFFFUL),
	0xFFFFFFFFUL, 0x7FFFFFFFUL, 0xFFFFFFFFUL, 0x7FFFFFFFUL,
	0UL, 0x80000000UL, 0UL, 0x80000000UL,
	PS_DUP_4(FLT_EPSILON),
	PS_DUP_3(FltPId180),
	PS_DUP_3(Flt180dPI),
	PS_DUP_3(FltPId2),
	PS_DUP_3(FltPI),
	PS_DUP_3(FltPIx2),
	PS_DUP_3(0.5F),
	PS_DUP_3(1.0F),
	PS_DUP_3(0x40DFF8D6UL),
	0.001F, 0.01F, 0.1F, 0.25F,
	3.0F, 10.0F, 100.0F, 8192.0F
};

alignas(16) const char
kLwrCaseConverter[] =
{
	'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
	'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
	'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
	'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
	'\x40', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67', '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
	'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77', '\x78', '\x79', '\x7A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
	'\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67', '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
	'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77', '\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
	'\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
	'\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97', '\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
	'\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7', '\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
	'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7', '\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
	'\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
	'\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
	'\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7', '\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
	'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7', '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
},
kUprCaseConverter[] =
{
	'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
	'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
	'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
	'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
	'\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47', '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
	'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57', '\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
	'\x60', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47', '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
	'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57', '\x58', '\x59', '\x5A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
	'\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
	'\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97', '\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
	'\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7', '\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
	'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7', '\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
	'\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
	'\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
	'\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7', '\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
	'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7', '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};

memcpy_t MemCopy = memcpy, MemMove = memmove;

__declspec(naked) PrimitiveCS *PrimitiveCS::Enter()
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		call	GetCurrentThreadId
		cmp		[ebx], eax
		jnz		doSpin
	done:
		mov		eax, ebx
		pop		ebx
		retn
	doSpin:
		mov		ecx, eax
		xor		eax, eax
		lock cmpxchg [ebx], ecx
		test	eax, eax
		jz		done
		push	esi
		mov		esi, ecx
		ALIGN 16
	spinHead:
		push	0
		call	Sleep
		xor		eax, eax
		lock cmpxchg [ebx], esi
		test	eax, eax
		jnz		spinHead
		pop		esi
		mov		eax, ebx
		pop		ebx
		retn
	}
}

__declspec(naked) void LightCS::Enter()
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		call	GetCurrentThreadId
		cmp		[ebx], eax
		jnz		doSpin
		inc		dword ptr [ebx+4]
		pop		ebx
		retn
		ALIGN 16
	doSpin:
		mov		ecx, eax
		xor		eax, eax
		lock cmpxchg [ebx], ecx
		test	eax, eax
		jz		done
		push	esi
		mov		esi, ecx
		ALIGN 16
	spinHead:
		push	0
		call	Sleep
		xor		eax, eax
		lock cmpxchg [ebx], esi
		test	eax, eax
		jnz		spinHead
		pop		esi
	done:
		mov		dword ptr [ebx+4], 1
		pop		ebx
		retn
	}
}

__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds:[0x11C54C0]
		mov		eax, [esp+4]
		xor		edx, edx
		div		dword ptr [ecx+4]
		mov		eax, [ecx+8]
		mov		eax, [eax+edx*4]
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		ALIGN 16
	iterHead:
		cmp		[eax+4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		retn	4
	found:
		mov		eax, [eax+8]
	done:
		retn	4
	}
}

__declspec(naked) UInt32 __vectorcall cvtd2ui(double value)
{
	__asm
	{
		lea		eax, [esp-8]
		movq	qword ptr [eax], xmm0
		fld		qword ptr [eax]
		fisttp	qword ptr [eax]
		mov		eax, [eax]
		retn
	}
}

__declspec(naked) double __fastcall cvtui2d(UInt32 value)
{
	__asm
	{
		push	0
		push	ecx
		fild	qword ptr [esp]
		add		esp, 8
		retn
	}
}

__declspec(naked) void __fastcall cvtui2d(UInt32 value, double *result)
{
	__asm
	{
		movd	xmm0, ecx
		movq	qword ptr [edx], xmm0
		fild	qword ptr [edx]
		fstp	qword ptr [edx]
		retn
	}
}

__declspec(naked) int __vectorcall ifloor(float value)
{
	__asm
	{
		movmskps	eax, xmm0
		test	al, 1
		jnz		isNeg
		cvttss2si	eax, xmm0
		retn
	isNeg:
		push	0x3FA0
		ldmxcsr	[esp]
		cvtss2si	eax, xmm0
		mov		byte ptr [esp+1], 0x1F
		ldmxcsr	[esp]
		pop		ecx
		retn
	}
}

__declspec(naked) int __vectorcall iceil(float value)
{
	__asm
	{
		movmskps	eax, xmm0
		test	al, 1
		jnz		isNeg
		push	0x5FA0
		ldmxcsr	[esp]
		cvtss2si	eax, xmm0
		mov		byte ptr [esp+1], 0x1F
		ldmxcsr	[esp]
		pop		ecx
		retn
	isNeg:
		cvttss2si	eax, xmm0
		retn
	}
}

__declspec(naked) float __vectorcall fMod(float numer, float denom)
{
	__asm
	{
		movaps	xmm2, xmm0
		divss	xmm2, xmm1
		cvttps2dq	xmm2, xmm2
		cvtdq2ps	xmm2, xmm2
		mulss	xmm2, xmm1
		subss	xmm0, xmm2
		retn
	}
}

__declspec(naked) float __vectorcall Sin(float angle)
{
	__asm
	{
		movaps	xmm1, xmm0
		movss	xmm0, PS_V3_PId2
		subss	xmm0, xmm1
		jmp		Cos
	}
}

__declspec(naked) float __vectorcall Cos(float angle)
{
	__asm
	{
		andps	xmm0, PS_AbsMask0
		movss	xmm2, PS_V3_PIx2
		movss	xmm1, kFlt2dPI
		mulss	xmm1, xmm0
		cvttss2si	eax, xmm1
		cmp		eax, 3
		jle		perdOK
		mov		edx, eax
		and		eax, 3
		shr		edx, 2
		cvtsi2ss	xmm1, edx
		mulss	xmm1, xmm2
		subss	xmm0, xmm1
	perdOK:
		xorps	xmm3, xmm3
		comiss	xmm0, xmm3
		jbe		retn1
		test	al, al
		jz		doCalc
		jp		fourthQ
		movss	xmm2, PS_V3_PI
		movss	xmm3, PS_FlipSignMask0
	fourthQ:
		subss	xmm0, xmm2
	doCalc:
		mulss	xmm0, xmm0
		movaps	xmm1, xmm0
		mulss	xmm1, kCosConsts
		subss	xmm1, kCosConsts+4
		mulss	xmm1, xmm0
		addss	xmm1, kCosConsts+8
		mulss	xmm1, xmm0
		subss	xmm1, kCosConsts+0xC
		mulss	xmm0, xmm1
		addss	xmm0, kCosConsts+0x10
		xorps	xmm0, xmm3
		retn
	retn1:
		movss	xmm0, PS_V3_One
		retn
		ALIGN 16
	kFlt2dPI:
		EMIT_DW(3F, 22, F9, 83)
	kCosConsts:
		EMIT_DW(37, C2, 3A, B1)
		EMIT_DW(3A, B5, 95, 51)
		EMIT_DW(3D, 2A, A7, 6F)
		EMIT_DW(3E, FF, FF, E0)
		EMIT_DW(3F, 7F, FF, FF)
	}
}

__declspec(naked) __m128 __vectorcall Cos_PS(__m128 angles)
{
	__asm
	{
		andps	xmm0, PS_AbsMask
		movaps	xmm1, xmm0
		movaps	xmm2, PS_V3_PIx2
		mulps	xmm1, kPS2dPI
		cvttps2dq	xmm1, xmm1
		movaps	xmm3, xmm1
		pcmpgtd	xmm3, kQuadTest
		movmskps	edx, xmm3
		test	dl, dl
		jz		perdOK
		movaps	xmm3, xmm1
		andps	xmm1, kQuadTest
		psrld	xmm3, 2
		cvtdq2ps	xmm3, xmm3
		mulps	xmm3, xmm2
		subps	xmm0, xmm3
	perdOK:
		movaps	xmm3, xmm1
		pcmpgtd	xmm1, kQuadTest+0x10
		andps	xmm2, xmm1
		andnps	xmm1, xmm3
		xorps	xmm3, xmm3
		pcmpgtd	xmm1, xmm3
		movaps	xmm3, xmm1
		pslld	xmm3, 0x1F
		andps	xmm1, PS_V3_PI
		orps	xmm1, xmm2
		subps	xmm0, xmm1
		mulps	xmm0, xmm0
		movaps	xmm1, kCosConsts
		mulps	xmm1, xmm0
		subps	xmm1, kCosConsts+0x10
		mulps	xmm1, xmm0
		addps	xmm1, kCosConsts+0x20
		mulps	xmm1, xmm0
		subps	xmm1, kCosConsts+0x30
		mulps	xmm0, xmm1
		addps	xmm0, kCosConsts+0x40
		xorps	xmm0, xmm3
		retn
		ALIGN 16
	kPS2dPI:
		EMIT_PS_3(3F, 22, F9, 83)
	kQuadTest:
		EMIT_PS_3(00, 00, 00, 03)
		EMIT_PS_3(00, 00, 00, 02)
	kCosConsts:
		EMIT_PS_3(37, C2, 3A, B1)
		EMIT_PS_3(3A, B5, 95, 51)
		EMIT_PS_3(3D, 2A, A7, 6F)
		EMIT_PS_3(3E, FF, FF, E0)
		EMIT_PS_3(3F, 7F, FF, FF)
	}
}

__declspec(naked) __m128 __vectorcall GetSinCos(float angle)
{
	__asm
	{
		movaps	xmm1, xmm0
		movss	xmm0, PS_V3_PId2
		subss	xmm0, xmm1
		unpcklps	xmm0, xmm1
		call	Cos_PS
		movshdup	xmm1, xmm0
		retn
	}
}

__declspec(naked) __m128 __vectorcall GetSinCosV3(__m128 angles)
{
	__asm
	{
		movaps	xmm5, xmm0
		call	Cos_PS
		movaps	xmm4, xmm0
		movaps	xmm0, PS_V3_PId2
		subps	xmm0, xmm5
		call	Cos_PS
		movaps	xmm1, xmm4
		retn
	}
}

__declspec(naked) float __vectorcall Tan(float angle)
{
	_asm
	{
		call	GetSinCos
		movaps	xmm2, xmm1
		andps	xmm2, PS_AbsMask0
		comiss	xmm2, PS_Epsilon
		jb		ooRange
		divss	xmm0, xmm1
		retn
	ooRange:
		xorps	xmm0, xmm0
		retn
	}
}

alignas(16) const UInt32 kASinConsts[] =
{
	0xBC996E30, 0x3D981627, 0x3E593484, 0x3FC90FDB, 0x34000000, 0x3F800000, 0x40490FDB, 0x03800000,
	0x37202A00, 0x37E24000, 0x38333600, 0x386E4C00, 0x38913200, 0x38A84000, 0x38BC5200, 0x38CDAC00,
	0x38DC5000, 0x38E8C000, 0x38F2C800, 0x38FAA800, 0x39005600, 0x39029000, 0x3903C800, 0x39046400,
	0x39042C00, 0x39036800, 0x39022800, 0x39003400, 0x38FB7000, 0x38F60800, 0x38EFD800, 0x38E8B800,
	0x38E15800, 0x38D96000, 0x38D0C000, 0x38C83000, 0x38BF2000, 0x38B59800, 0x38AC5800, 0x38A2E000,
	0x38998000, 0x388FD000, 0x38866000, 0x387AA000, 0x38678000, 0x3855E000, 0x3844A000, 0x38338000,
	0x38234000, 0x3812E000, 0x38042000, 0x37EB4000, 0x37CF4000, 0x37B64000, 0x379D4000, 0x37874000,
	0x37658000, 0x373F8000, 0x371A8000, 0x36F90000, 0x36C30000, 0x36900000, 0x364E0000, 0x360E0000,
	0x35B40000, 0x35500000, 0x34D00000, 0x33C00000, 0x34000000, 0x34800000, 0x35400000, 0x35A80000,
	0x36040000, 0x36400000, 0x367C0000, 0x36A60000, 0x36D60000, 0x37040000, 0x371D0000, 0x37390000,
	0x37560000, 0x37750000, 0x378B0000, 0x379C0000, 0x37AC8000, 0x37BE8000, 0x37D08000, 0x37E30000,
	0x37F58000, 0x3803C000, 0x380D0000, 0x3815C000, 0x381EC000, 0x38274000, 0x382F8000, 0x38380000,
	0x383F8000, 0x38470000, 0x384E8000, 0x3854C000, 0x385B0000, 0x38604000, 0x38658000, 0x3869C000,
	0x386D4000, 0x38708000, 0x3872C000, 0x38744000, 0x38750000, 0x38750000, 0x38740000, 0x38724000,
	0x38700000, 0x386C0000, 0x3867C000, 0x3862C000, 0x385C8000, 0x38558000, 0x384D8000, 0x38450000,
	0x383B0000, 0x38318000, 0x38260000, 0x381A0000, 0x380D8000, 0x38010000, 0x37E70000, 0x37CB0000,
	0x37AF0000, 0x37930000, 0x376C0000, 0x37340000, 0x37020000, 0x36A00000, 0x36180000, 0x35000000
};

__declspec(naked) float __vectorcall ASin(float x)
{
	__asm
	{
		xorps	xmm4, xmm4
		comiss	xmm0, xmm4
		jz		done
		movaps	xmm4, xmm0
		andps	xmm4, PS_FlipSignMask0
		xorps	xmm0, xmm4
		movss	xmm3, kASinConsts+0x14
		comiss	xmm0, xmm3
		jnb		ooRange
		movss	xmm1, kASinConsts+0x1C
		paddd	xmm1, xmm0
		movaps	xmm2, kASinConsts
		cvttss2si	eax, xmm1
		movaps	xmm1, xmm2
		mulss	xmm1, xmm0
		psrldq	xmm2, 4
		addss	xmm1, xmm2
		mulss	xmm1, xmm0
		psrldq	xmm2, 4
		subss	xmm1, xmm2
		mulss	xmm1, xmm0
		subss	xmm3, xmm0
		movshdup	xmm0, xmm2
		addss	xmm1, xmm0
		sqrtss	xmm2, xmm3
		mulss	xmm2, xmm1
		subss	xmm0, xmm2
		addss	xmm0, kASinConsts[eax*4+0x20]
		xorps	xmm0, xmm4
	done:
		retn
	ooRange:
		movss	xmm0, kASinConsts+0xC
		xorps	xmm0, xmm4
		retn
	}
}

__declspec(naked) float __vectorcall ACos(float x)
{
	_asm
	{
		xorps	xmm4, xmm4
		comiss	xmm0, xmm4
		jz		isZero
		movaps	xmm4, xmm0
		andps	xmm4, PS_FlipSignMask0
		xorps	xmm0, xmm4
		movss	xmm3, kASinConsts+0x14
		comiss	xmm0, xmm3
		jnb		ooRange
		movss	xmm1, kASinConsts+0x1C
		paddd	xmm1, xmm0
		movaps	xmm2, kASinConsts
		cvttss2si	eax, xmm1
		movaps	xmm1, xmm2
		mulss	xmm1, xmm0
		psrldq	xmm2, 4
		addss	xmm1, xmm2
		mulss	xmm1, xmm0
		psrldq	xmm2, 4
		subss	xmm1, xmm2
		mulss	xmm1, xmm0
		psrldq	xmm2, 4
		addss	xmm1, xmm2
		subss	xmm3, xmm0
		sqrtss	xmm0, xmm3
		mulss	xmm0, xmm1
		subss	xmm0, kASinConsts[eax*4+0x20]
		movmskps	eax, xmm4
		test	al, al
		jz		done
		xorps	xmm0, xmm4
		addss	xmm0, kASinConsts+0x18
	done:
		retn
	isZero:
		movss	xmm0, kASinConsts+0xC
		retn
	ooRange:
		movss	xmm0, kASinConsts+0x18
		psrad	xmm4, 0x1F
		andps	xmm0, xmm4
		retn
	}
}

__declspec(naked) float __vectorcall ATan(float x)
{
	_asm
	{
		movss	xmm1, PS_V3_One
		jmp		ATan2
	}
}

__declspec(naked) float __vectorcall ATan2(float y, float x)
{
	__asm
	{
		xorps	xmm2, xmm2
		comiss	xmm0, xmm2
		jz		zeroY
		comiss	xmm1, xmm2
		jz		zeroX
		movaps	xmm2, xmm0
		unpcklpd	xmm2, xmm1
		andps	xmm2, PS_AbsMask
		movaps	xmm3, xmm2
		pshufd	xmm4, xmm2, 0xFE
		maxss	xmm3, xmm4
		minss	xmm4, xmm2
		divss	xmm4, xmm3
		movaps	xmm3, xmm4
		mulss	xmm3, xmm4
		movaps	xmm5, xmm3
		mulss	xmm5, kATanConsts
		addss	xmm5, kATanConsts+4
		mulss	xmm5, xmm3
		subss	xmm5, kATanConsts+8
		mulss	xmm5, xmm3
		addss	xmm5, kATanConsts+0xC
		mulss	xmm5, xmm3
		subss	xmm5, kATanConsts+0x10
		mulss	xmm5, xmm3
		addss	xmm5, kATanConsts+0x14
		mulss	xmm4, xmm5
		movss	xmm3, PS_FlipSignMask0
		pshufd	xmm5, xmm2, 0xFE
		comiss	xmm2, xmm5
		jbe		doneCmp1
		xorps	xmm4, xmm3
		addss	xmm4, PS_V3_PId2
	doneCmp1:
		xorps	xmm2, xmm2
		comiss	xmm1, xmm2
		jnb		doneCmp2
		xorps	xmm4, xmm3
		addss	xmm4, PS_V3_PI
	doneCmp2:
		andps	xmm0, xmm3
		xorps	xmm0, xmm4
		retn
	zeroY:
		comiss	xmm1, xmm2
		jnb		done
		movss	xmm0, PS_V3_PI
		retn
	zeroX:
		movss	xmm1, PS_V3_PId2
		andps	xmm0, PS_FlipSignMask0
		orps	xmm0, xmm1
	done:
		retn
		ALIGN 16
	kATanConsts:
		EMIT_DW(BC, 5C, DD, 30)
		EMIT_DW(3D, 6B, 6D, 55)
		EMIT_DW(3D, F8, 4C, 31)
		EMIT_DW(3E, 48, 54, C9)
		EMIT_DW(3E, AA, 7E, 45)
		EMIT_DW(3F, 7F, FF, B7)
	}
}

__declspec(naked) __m128 __vectorcall NormalizePS(__m128 inPS)
{
	__asm
    {
		movaps	xmm1, xmm0
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
        retn
    }
}

__declspec(noinline) char *GetStrArgBuffer()
{
	thread_local char *s_strBuffer = NULL;
	if (!s_strBuffer)
		s_strBuffer = (char*)_aligned_malloc(0x20000, 0x10);
	return s_strBuffer;
}

__declspec(naked) void __fastcall NiReleaseObject(NiRefObject *toRelease)
{
	__asm
	{
		lock dec dword ptr [ecx+4]
		jg		done
		mov		eax, [ecx]
		call	dword ptr [eax+4]
	done:
		retn
	}
}

__declspec(naked) NiRefObject** __stdcall NiReleaseAddRef(void *toRelease, NiRefObject *toAdd)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		ecx, [eax]
		cmp		ecx, [esp+8]
		jz		done
		test	ecx, ecx
		jz		doAdd
		lock dec dword ptr [ecx+4]
		jg		doAdd
		mov		eax, [ecx]
		call	dword ptr [eax+4]
		mov		eax, [esp+4]
	doAdd:
		mov		ecx, [esp+8]
		mov		[eax], ecx
		test	ecx, ecx
		jz		done
		lock inc dword ptr [ecx+4]
	done:
		retn	8
	}
}

__declspec(naked) UInt32 __fastcall RGBHexToDec(UInt32 rgb)
{
	__asm
	{
		movzx	eax, cl
		imul	eax, 0xF4240
		movzx	edx, ch
		imul	edx, 0x3E8
		add		eax, edx
		shr		ecx, 0x10
		add		eax, ecx
		retn
	}
}

__declspec(naked) UInt32 __fastcall RGBDecToHex(UInt32 rgb)
{
	__asm
	{
		push	ebx
		mov		eax, ecx
		mov		ecx, 0xF4240
		xor		edx, edx
		div		ecx
		mov		ebx, eax
		mov		eax, edx
		mov		ecx, 0x3E8
		xor		edx, edx
		div		ecx
		shl		eax, 8
		add		eax, ebx
		shl		edx, 0x10
		add		eax, edx
		pop		ebx
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrLen(const char *str)
{
	__asm
	{
		test	ecx, ecx
		jz		nullPtr
		push	ecx
		test	ecx, 3
		jz		iter4
		ALIGN 16
	iter1:
		mov		al, [ecx]
		inc		ecx
		test	al, al
		jz		done1
		test	ecx, 3
		jnz		iter1
		ALIGN 16
	iter4:
		mov		eax, [ecx]
		mov		edx, 0x7EFEFEFF
		add		edx, eax
		not		eax
		xor		eax, edx
		add		ecx, 4
		test	eax, 0x81010100
		jz		iter4
		mov		eax, [ecx-4]
		test	al, al
		jz		done4
		test	ah, ah
		jz		done3
		test	eax, 0xFF0000
		jz		done2
		test	eax, 0xFF000000
		jnz		iter4
	done1:
		lea		eax, [ecx-1]
		pop		ecx
		sub		eax, ecx
		retn
	done2:
		lea		eax, [ecx-2]
		pop		ecx
		sub		eax, ecx
		retn
	done3:
		lea		eax, [ecx-3]
		pop		ecx
		sub		eax, ecx
		retn
	done4:
		lea		eax, [ecx-4]
		pop		ecx
		sub		eax, ecx
		retn
	nullPtr:
		xor		eax, eax
		retn
	}
}

__declspec(naked) bool __fastcall MemCmp(const void *ptr1, const void *ptr2, UInt32 bsize)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		mov		ecx, [esp+0xC]
		shr		ecx, 2
		repe cmpsd
		setz	al
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) void __fastcall MemZero(void *dest, UInt32 bsize)
{
	__asm
	{
		test	ecx, ecx
		jz		done
		shr		edx, 2
		jz		done
		push	edi
		mov		edi, ecx
		xor		eax, eax
		mov		ecx, edx
		rep stosd
		pop		edi
	done:
		retn
	}
}

__declspec(naked) char* __fastcall StrCopy(char *dest, const char *src)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		push	ecx
		mov		ecx, edx
		call	StrLen
		pop		edx
		push	eax
		inc		eax
		push	eax
		push	ecx
		push	edx
		call	MemMove
		add		esp, 0xC
		pop		ecx
		add		eax, ecx
		retn
	nullTerm:
		mov		[eax], 0
	done:
		retn
	}
}

__declspec(naked) char* __fastcall StrNCopy(char *dest, const char *src, UInt32 length)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		cmp		dword ptr [esp+4], 0
		jz		nullTerm
		push	esi
		mov		esi, ecx
		mov		ecx, edx
		call	StrLen
		mov		edx, [esp+8]
		cmp		edx, eax
		cmova	edx, eax
		push	edx
		push	ecx
		push	esi
		add		esi, edx
		call	MemMove
		add		esp, 0xC
		mov		eax, esi
		pop		esi
	nullTerm:
		mov		[eax], 0
	done:
		retn	4
	}
}

__declspec(naked) char* __fastcall StrLenCopy(char *dest, const char *src, UInt32 length)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		mov		ecx, [esp+4]
		test	ecx, ecx
		jz		nullTerm
		push	ecx
		push	edx
		push	eax
		call	MemMove
		add		esp, 0xC
		add		eax, [esp+4]
	nullTerm:
		mov		[eax], 0
	done:
		retn	4
	}
}

__declspec(naked) char* __fastcall StrCat(char *dest, const char *src)
{
	__asm
	{
		test	ecx, ecx
		jz		nullPtr
		push	edx
		call	StrLen
		pop		edx
		add		ecx, eax
		jmp		StrCopy
	nullPtr:
		xor		eax, eax
		retn
	}
}

__declspec(noinline) char __fastcall StrCompare(const char *lstr, const char *rstr)
{
	if (!lstr) return rstr ? -1 : 0;
	if (!rstr) return 1;
	UInt8 lchr, rchr;
	while (*lstr)
	{
		lchr = kLwrCaseConverter[*(UInt8*)lstr];
		rchr = kLwrCaseConverter[*(UInt8*)rstr];
		if (lchr == rchr)
		{
			lstr++;
			rstr++;
			continue;
		}
		return (lchr < rchr) ? -1 : 1;
	}
	return *rstr ? -1 : 0;
}

__declspec(noinline) char __fastcall StrBeginsCS(const char *lstr, const char *rstr)
{
	if (!lstr || !rstr) return 0;
	UInt32 length = StrLen(rstr);
	while (length >= 4)
	{
		if (*(UInt32*)lstr != *(UInt32*)rstr)
			return 0;
		lstr += 4;
		rstr += 4;
		length -= 4;
	}
	while (length)
	{
		if (*lstr != *rstr)
			return 0;
		lstr++;
		rstr++;
		length--;
	}
	return *lstr ? 1 : 2;
}

__declspec(noinline) char __fastcall StrBeginsCI(const char *lstr, const char *rstr)
{
	if (!lstr || !rstr) return 0;
	UInt32 length = StrLen(rstr);
	while (length)
	{
		if (kLwrCaseConverter[*(UInt8*)lstr] != kLwrCaseConverter[*(UInt8*)rstr])
			return 0;
		lstr++;
		rstr++;
		length--;
	}
	return *lstr ? 1 : 2;
}

__declspec(noinline) void __fastcall FixPath(char *str)
{
	UInt8 curr;
	while (curr = *str)
	{
		if (curr == '\\')
			*str = '/';
		else *str = kLwrCaseConverter[curr];
		str++;
	}
}

__declspec(noinline) void __fastcall StrToLower(char *str)
{
	UInt8 curr;
	while (curr = *str)
	{
		*str = kLwrCaseConverter[curr];
		str++;
	}
}

__declspec(noinline) void __fastcall StrToUpper(char *str)
{
	UInt8 curr;
	while (curr = *str)
	{
		*str = kUprCaseConverter[curr];
		str++;
	}
}

__declspec(naked) void __fastcall ReplaceChr(char *str, char from, char to)
{
	__asm
	{
	iterHead:
		mov		al, [ecx]
		test	al, al
		jz		done
		inc		ecx
		cmp		al, dl
		jnz		iterHead
		mov		al, [esp+4]
		mov		[ecx-1], al
		jmp		iterHead
		ALIGN 16
	done:
		retn	4
	}
}

__declspec(naked) char* __fastcall FindChr(const char *str, char chr)
{
	__asm
	{
	iterHead:
		mov		al, [ecx]
		test	al, al
		jz		retnNULL
		cmp		al, dl
		jz		found
		inc		ecx
		jmp		iterHead
		ALIGN 16
	found:
		mov		eax, ecx
		retn
		ALIGN 16
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) char* __fastcall FindChrR(const char *str, UInt32 length, char chr)
{
	__asm
	{
		lea		eax, [ecx+edx]
		mov		dl, [esp+4]
		ALIGN 16
	iterHead:
		cmp		eax, ecx
		jz		retnNULL
		dec		eax
		mov		dh, [eax]
		cmp		dl, dh
		jnz		iterHead
		retn	4
		ALIGN 16
	retnNULL:
		xor		eax, eax
		retn	4
	}
}

__declspec(noinline) char* __fastcall SubStrCI(const char *srcStr, const char *subStr)
{
	int srcLen = StrLen(srcStr);
	if (!srcLen) return NULL;
	int subLen = StrLen(subStr);
	if (!subLen) return NULL;
	srcLen -= subLen;
	if (srcLen < 0) return NULL;
	int index;
	do
	{
		index = 0;
		while (true)
		{
			if (kLwrCaseConverter[*(UInt8*)(srcStr + index)] != kLwrCaseConverter[*(UInt8*)(subStr + index)])
				break;
			if (++index == subLen)
				return const_cast<char*>(srcStr);
		}
		srcStr++;
	}
	while (--srcLen >= 0);
	return NULL;
}

__declspec(naked) char* __fastcall SlashPos(const char *str)
{
	__asm
	{
	iterHead:
		mov		dl, [ecx]
		test	dl, dl
		jz		retnNULL
		cmp		dl, '\\'
		jz		found
		cmp		dl, '/'
		jz		found
		inc		ecx
		jmp		iterHead
		ALIGN 16
	found:
		mov		eax, ecx
		retn
		ALIGN 16
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(naked) char* __fastcall SlashPosR(const char *str)
{
	__asm
	{
		call	StrLen
		add		eax, ecx
		ALIGN 16
	iterHead:
		cmp		eax, ecx
		jz		retnNULL
		dec		eax
		mov		dl, [eax]
		cmp		dl, '\\'
		jz		found
		cmp		dl, '/'
		jnz		iterHead
	found:
		retn
		ALIGN 16
	retnNULL:
		xor		eax, eax
		retn
	}
}

__declspec(noinline) char* __fastcall GetNextToken(char *str, char delim)
{
	if (!str) return NULL;
	bool found = false;
	char chr;
	while (chr = *str)
	{
		if (chr == delim)
		{
			*str = 0;
			found = true;
		}
		else if (found)
			break;
		str++;
	}
	return str;
}

__declspec(noinline) char* __fastcall GetNextToken(char *str, const char *delims)
{
	if (!str) return NULL;
	bool table[0x100];
	MemZero(table, 0x100);
	UInt8 curr;
	while (curr = *delims)
	{
		table[curr] = true;
		delims++;
	}
	bool found = false;
	while (curr = *str)
	{
		if (table[curr])
		{
			*str = 0;
			found = true;
		}
		else if (found)
			break;
		str++;
	}
	return str;
}

__declspec(naked) char* __fastcall CopyString(const char *key)
{
	__asm
	{
		call	StrLen
		inc		eax
		push	eax
		push	ecx
		push	eax
		call	malloc
		pop		ecx
		push	eax
		call	MemCopy
		add		esp, 0xC
		retn
	}
}

__declspec(naked) char* __fastcall CopyCString(const char *src)
{
	__asm
	{
		call	StrLen
		test	eax, eax
		jz		done
		inc		eax
		push	eax
		push	ecx
		push	eax
		GAME_HEAP_ALLOC
		push	eax
		call	MemCopy
		add		esp, 0xC
	done:
		retn
	}
}

__declspec(naked) char* __fastcall IntToStr(char *str, int num)
{
	__asm
	{
		push	esi
		push	edi
		test	edx, edx
		jns		skipNeg
		neg		edx
		mov		[ecx], '-'
		inc		ecx
	skipNeg:
		mov		esi, ecx
		mov		edi, ecx
		mov		eax, edx
		mov		ecx, 0xA
		ALIGN 16
	workIter:
		xor		edx, edx
		div		ecx
		add		dl, '0'
		mov		[esi], dl
		inc		esi
		test	eax, eax
		jnz		workIter
		mov		[esi], 0
		mov		eax, esi
		ALIGN 16
	swapIter:
		dec		esi
		cmp		esi, edi
		jbe		done
		mov		dl, [esi]
		mov		cl, [edi]
		mov		[esi], cl
		mov		[edi], dl
		inc		edi
		jmp		swapIter
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) char* __vectorcall FltToStr(char *str, double value)
{
	static const double kDbl1Mil = 1000000.0;
	static const int kIntDivisors[] = {10, 100, 1000, 10000, 100000};
	__asm
	{
		xorps	xmm1, xmm1
		comisd	xmm0, xmm1
		jz		isZero
		ja		isPos
		andpd	xmm0, PD_AbsMask
		mov		[ecx], '-'
		inc		ecx
	isPos:
		push	esi
		push	edi
		lea		eax, [esp-8]
		movq	qword ptr [eax], xmm0
		fld		qword ptr [eax]
		fld		st
		fisttp	qword ptr [eax]
		mov		esi, [eax]
		fild	qword ptr [eax]
		fsubp	st(1), st
		fstp	qword ptr [eax]
		movq	xmm0, qword ptr [eax]
		/*roundsd	xmm2, xmm0, 3
		lea		eax, [esp-8]
		movq	qword ptr [eax], xmm2
		fld		qword ptr [eax]
		fisttp	qword ptr [eax]
		mov		esi, [eax]
		test	esi, esi
		jz		noInt
		subsd	xmm0, xmm2
	noInt:*/
		xor		edi, edi
		comisd	xmm0, xmm1
		jz		noFrac
		mulsd	xmm0, kDbl1Mil
		cvtsd2si	edi, xmm0
		cmp		edi, 0xF4240
		jnz		noFrac
		xor		edi, edi
		inc		esi
	noFrac:
		test	esi, esi
		jz		zeroInt
		mov		edx, esi
		call	IntToStr
		mov		ecx, eax
		jmp		doneInt
	zeroInt:
		mov		[ecx], '0'
		inc		ecx
	doneInt:
		test	edi, edi
		jz		done
		mov		[ecx], '.'
		inc		ecx
		mov		edx, edi
		mov		esi, offset kIntDivisors
		mov		edi, 4
		ALIGN 16
	fracIter:
		mov		eax, edx
		xor		edx, edx
		div		dword ptr [esi+edi*4]
		add		al, '0'
		mov		[ecx], al
		inc		ecx
		test	edx, edx
		jz		done
		dec		edi
		jns		fracIter
		add		dl, '0'
		mov		[ecx], dl
		inc		ecx
	done:
		mov		[ecx], 0
		mov		eax, ecx
		pop		edi
		pop		esi
		retn
	isZero:
		mov		word ptr [ecx], '0'
		lea		eax, [ecx+1]
		retn
	}
}

__declspec(naked) int __fastcall StrToInt(const char *str)
{
	__asm
	{
		push	esi
		push	edi
		xor		eax, eax
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		ecx, ecx
		xor		edi, edi
		cmp		[esi], '-'
		setz	dl
		jnz		charIter
		inc		esi
		ALIGN 16
	charIter:
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		iterEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		cmp		eax, edi
		jl		overflow
		mov		edi, eax
		inc		esi
		jmp		charIter
		ALIGN 16
	iterEnd:
		test	dl, dl
		jz		done
		neg		eax
	done:
		pop		edi
		pop		esi
		retn
	overflow:
		movzx	eax, dl
		add		eax, 0x7FFFFFFF
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrToUInt(const char *str)
{
	__asm
	{
		push	esi
		push	edi
		xor		eax, eax
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		ecx, ecx
		xor		edi, edi
		cmp		[esi], '-'
		setz	dl
		jnz		charIter
		inc		esi
		ALIGN 16
	charIter:
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		iterEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		cmp		eax, edi
		jb		overflow
		mov		edi, eax
		inc		esi
		jmp		charIter
		ALIGN 16
	iterEnd:
		test	dl, dl
		jz		done
		neg		eax
	done:
		pop		edi
		pop		esi
		retn
	overflow:
		movzx	eax, dl
		dec		eax
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) double __vectorcall StrToDbl(const char *str)
{
	alignas(16) static const double kStrToDblConsts[] = {4294967296, -4294967296, 1.0e-09, 1.0e-08, 1.0e-07, 1.0e-06, 1.0e-05, 0.0001, 0.001, 0.01, 0.1};
	__asm
	{
		push	esi
		push	edi
		xorps	xmm0, xmm0
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		eax, eax
		xor		ecx, ecx
		xor		edi, edi
		cmp		[esi], '-'
		setz	dl
		jnz		intIter
		inc		esi
		ALIGN 16
	intIter:
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		intEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		cmp		eax, edi
		jb		overflow
		mov		edi, eax
		inc		esi
		jmp		intIter
		ALIGN 16
	intEnd:
		test	eax, eax
		jz		noInt
		cvtsi2sd	xmm0, eax
		jns		noOverflow
		addsd	xmm0, kStrToDblConsts
	noOverflow:
		shl		dl, 1
		xor		eax, eax
	noInt:
		cmp		cl, 0xFE
		jnz		addSign
		mov		dh, 9
		ALIGN 16
	fracIter:
		inc		esi
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		fracEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		dec		dh
		jnz		fracIter
	fracEnd:
		test	eax, eax
		jz		addSign
		cvtsi2sd	xmm1, eax
		shl		dl, 1
		mov		cl, dh
		mulsd	xmm1, kStrToDblConsts[ecx*8+0x10]
		addsd	xmm0, xmm1
	addSign:
		test	dl, 6
		jz		done
		orpd	xmm0, PD_FlipSignMask
	done:
		pop		edi
		pop		esi
		retn
	overflow:
		movzx	eax, dl
		movq	xmm0, kStrToDblConsts[eax*8]
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) char* __fastcall UIntToHex(char *str, UInt32 num)
{
	static const char kCharAtlas[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	__asm
	{
		bsr		eax, edx
		jz		isZero
		push	esi
		shr		eax, 2
		lea		esi, [ecx+eax+1]
		push	esi
		xor		eax, eax
		ALIGN 16
	workIter:
		mov		al, dl
		and		al, 0xF
		mov		cl, kCharAtlas[eax]
		dec		esi
		mov		[esi], cl
		shr		edx, 4
		jnz		workIter
		pop		eax
		mov		[eax], 0
		pop		esi
		retn
		ALIGN 16
	isZero:
		mov		word ptr [ecx], '0'
		lea		eax, [ecx+1]
		retn
	}
}

__declspec(naked) UInt32 __fastcall HexToUInt(const char *str)
{
	__asm
	{
		push	esi
		call	StrLen
		test	eax, eax
		jz		done
		lea		esi, [ecx+eax]
		mov		ch, al
		xor		eax, eax
		xor		cl, cl
	hexToInt:
		dec		esi
		movsx	edx, byte ptr [esi]
		sub		dl, '0'
		js		done
		cmp		dl, 9
		jle		doAdd
		or		dl, 0x20
		cmp		dl, '1'
		jl		done
		cmp		dl, '6'
		jg		done
		sub		dl, 0x27
	doAdd:
		shl		edx, cl
		add		eax, edx
		add		cl, 4
		dec		ch
		jnz		hexToInt
	done:
		pop		esi
		retn
	}
}

__declspec(noinline) UInt8 *AuxBuffer::Get(UInt32 bufIdx, UInt32 reqSize)
{
	thread_local AuxBuffer s_auxBuffers[3];
	AuxBuffer *auxBuf = &s_auxBuffers[bufIdx];
	if (auxBuf->size < reqSize)
	{
		auxBuf->size = reqSize;
		if (auxBuf->ptr)
			_aligned_free(auxBuf->ptr);
		auxBuf->ptr = (UInt8*)_aligned_malloc(reqSize, 0x10);
	}
	else if (!auxBuf->ptr)
		auxBuf->ptr = (UInt8*)_aligned_malloc(auxBuf->size, 0x10);
	return auxBuf->ptr;
}

DString::DString(const char *from)
{
	length = StrLen(from);
	if (length)
	{
		alloc = AlignNumAlloc<char>(length + 1);
		str = (char*)Pool_Alloc(alloc);
		memcpy(str, from, length + 1);
	}
	else
	{
		str = NULL;
		alloc = 0;
	}
}

DString::DString(const DString &from)
{
	length = from.length;
	if (length)
	{
		alloc = AlignNumAlloc<char>(length + 1);
		str = (char*)Pool_Alloc(alloc);
		memcpy(str, from.str, length + 1);
	}
	else
	{
		str = NULL;
		alloc = 0;
	}
}

DString::DString(UInt16 _alloc) : length(0)
{
	if (_alloc)
	{
		alloc = AlignNumAlloc<char>(_alloc + 1);
		str = (char*)Pool_Alloc(alloc);
		*str = 0;
	}
	else
	{
		str = NULL;
		alloc = 0;
	}
}

void DString::Reserve(UInt16 size)
{
	if (alloc > size) return;
	UInt16 newAlloc = AlignNumAlloc<char>(size + 1);
	char *newStr = (char*)Pool_Alloc(newAlloc);
	if (str)
	{
		if (length) memcpy(newStr, str, length);
		Pool_Free(str, alloc);
	}
	str = newStr;
	str[length] = 0;
	alloc = newAlloc;
}

DString& DString::operator=(const char *other)
{
	if (str != other)
	{
		length = StrLen(other);
		if (length)
		{
			if (alloc <= length)
			{
				if (str) Pool_Free(str, alloc);
				alloc = AlignNumAlloc<char>(length + 1);
				str = (char*)Pool_Alloc(alloc);
			}
			memcpy(str, other, length + 1);
		}
		else if (str)
			*str = 0;
	}
	return *this;
}

DString& DString::operator=(const DString &other)
{
	if (this != &other)
	{
		length = other.length;
		if (length)
		{
			if (alloc <= length)
			{
				if (str) Pool_Free(str, alloc);
				alloc = AlignNumAlloc<char>(length + 1);
				str = (char*)Pool_Alloc(alloc);
			}
			memcpy(str, other.str, length + 1);
		}
		else if (str)
			*str = 0;
	}
	return *this;
}

DString& DString::operator+=(char chr)
{
	Reserve(length + 1);
	*(UInt16*)(str + length) = chr;
	length++;
	return *this;
}

DString& DString::operator+=(const char *other)
{
	UInt16 otherLen = StrLen(other);
	if (otherLen)
	{
		UInt16 newLen = length + otherLen;
		Reserve(newLen);
		memcpy(str + length, other, otherLen + 1);
		length = newLen;
	}
	return *this;
}

DString& DString::operator+=(const DString &other)
{
	if (other.length)
	{
		UInt16 newLen = length + other.length;
		Reserve(newLen);
		memcpy(str + length, other.str, other.length + 1);
		length = newLen;
	}
	return *this;
}

bool DString::operator==(const char *other)
{
	return !strcmp(str, other);
}

bool DString::operator==(const DString &other)
{
	return !strcmp(str, other.str);
}

DString& DString::Insert(UInt16 index, char chr)
{
	if (index >= length)
		return this->operator+=(chr);
	Reserve(length + 1);
	memmove(str + index + 1, str + index, length - index + 1);
	str[index] = chr;
	length++;
	return *this;
}

DString& DString::Insert(UInt16 index, const char *other)
{
	if (index >= length)
		return this->operator+=(other);
	UInt16 otherLen = StrLen(other);
	if (otherLen)
	{
		UInt16 newLen = length + otherLen;
		Reserve(newLen);
		memmove(str + index + otherLen, str + index, length - index + 1);
		memcpy(str + index, other, otherLen);
		length = newLen;
	}
	return *this;
}

DString& DString::Insert(UInt16 index, const DString &other)
{
	if (index >= length)
		return this->operator+=(other);
	if (other.length)
	{
		UInt16 newLen = length + other.length;
		Reserve(newLen);
		memmove(str + index + other.length, str + index, length - index + 1);
		memcpy(str + index, other.str, other.length);
		length = newLen;
	}
	return *this;
}

DString& DString::Erase(UInt16 index, UInt16 count)
{
	if (index < length)
	{
		UInt16 endIdx = index + count;
		if (endIdx < length)
		{
			memmove(str + index, str + endIdx, length - endIdx + 1);
			length -= count;
		}
		else
		{
			str[index] = 0;
			length = index;
		}
	}
	return *this;
}

DString& DString::Replace(UInt16 bgnIdx, const char *other)
{
	if (bgnIdx >= length)
		return this->operator+=(other);
	UInt16 otLen = StrLen(other);
	if (otLen)
	{
		UInt16 endIdx = bgnIdx + otLen;
		if (endIdx > length)
		{
			Reserve(endIdx);
			length = endIdx;
		}
		memcpy(str + bgnIdx, other, otLen);
	}
	else length = bgnIdx;
	str[length] = 0;
	return *this;
}

DString& DString::Replace(UInt16 bgnIdx, const DString &other)
{
	if (bgnIdx >= length)
		return this->operator+=(other);
	if (other.length)
	{
		UInt16 endIdx = bgnIdx + other.length;
		if (endIdx > length)
		{
			Reserve(endIdx);
			length = endIdx;
		}
		memcpy(str + bgnIdx, other.str, other.length);
	}
	else length = bgnIdx;
	str[length] = 0;
	return *this;
}

DString DString::SubString(UInt16 bgnIdx, UInt16 endIdx)
{
	char *resStr = NULL;
	UInt16 resLen = 0, resAlloc = 0;
	if (bgnIdx < length)
	{
		if (endIdx > length)
			endIdx = length;
		resLen = endIdx - bgnIdx;
		if (resLen)
		{
			resAlloc = AlignNumAlloc<char>(resLen + 1);
			resStr = (char*)Pool_Alloc(resAlloc);
			memcpy(resStr, str + bgnIdx, resLen);
			resStr[resLen] = 0;
		}
	}
	return DString(resStr, resLen, resAlloc);
}

DString DString::ToLower()
{
	if (!length) return DString();
	UInt16 resAlloc = AlignNumAlloc<char>(length + 1);
	char *resStr = (char*)Pool_Alloc(resAlloc);
	memcpy(resStr, str, length + 1);
	StrToLower(resStr);
	return DString(resStr, length, resAlloc);
}

DString DString::ToUpper()
{
	if (!length) return DString();
	UInt16 resAlloc = AlignNumAlloc<char>(length + 1);
	char *resStr = (char*)Pool_Alloc(resAlloc);
	memcpy(resStr, str, length + 1);
	StrToUpper(resStr);
	return DString(resStr, length, resAlloc);
}

DString operator+(const DString &lStr, char rChr)
{
	UInt16 resLen = lStr.length + 1, resAlloc = AlignNumAlloc<char>(resLen + 1);
	char *resStr = (char*)Pool_Alloc(resAlloc);
	if (lStr.length) memcpy(resStr, lStr.str, lStr.length);
	*(UInt16*)(resStr + lStr.length) = rChr;
	return DString(resStr, resLen, resAlloc);
}

DString operator+(const DString &lStr, const char *rStr)
{
	char *resStr = NULL;
	UInt16 rLen = StrLen(rStr), resLen = lStr.length + rLen, resAlloc = 0;
	if (resLen)
	{
		resAlloc = AlignNumAlloc<char>(resLen + 1);
		resStr = (char*)Pool_Alloc(resAlloc);
		if (lStr.length) memcpy(resStr, lStr.str, lStr.length);
		if (rLen) memcpy(resStr + lStr.length, rStr, rLen);
		resStr[resLen] = 0;
	}
	return DString(resStr, resLen, resAlloc);
}

DString operator+(const char *lStr, const DString &rStr)
{
	char *resStr = NULL;
	UInt16 lLen = StrLen(lStr), resLen = lLen + rStr.length, resAlloc = 0;
	if (resLen)
	{
		resAlloc = AlignNumAlloc<char>(resLen + 1);
		resStr = (char*)Pool_Alloc(resAlloc);
		if (lLen) memcpy(resStr, lStr, lLen);
		if (rStr.length) memcpy(resStr + lLen, rStr.str, rStr.length);
		resStr[resLen] = 0;
	}
	return DString(resStr, resLen, resAlloc);
}

bool __fastcall FileExists(const char *filePath)
{
	UInt32 attr = GetFileAttributes(filePath);
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

FileStream::FileStream(const char *filePath)
{
	theFile = fopen(filePath, "rb");
}

FileStream::FileStream(const char *filePath, UInt32 inOffset)
{
	theFile = fopen(filePath, "rb");
	if (theFile)
	{
		fseek(theFile, 0, SEEK_END);
		if (ftell(theFile) < inOffset)
			Close();
		else fseek(theFile, inOffset, SEEK_SET);
	}
}

bool FileStream::Open(const char *filePath)
{
	if (theFile) fclose(theFile);
	theFile = fopen(filePath, "rb");
	return theFile != NULL;
}

bool FileStream::OpenAt(const char *filePath, UInt32 inOffset)
{
	if (theFile) fclose(theFile);
	theFile = fopen(filePath, "rb");
	if (!theFile) return false;
	fseek(theFile, 0, SEEK_END);
	if (ftell(theFile) < inOffset)
	{
		Close();
		return false;
	}
	fseek(theFile, inOffset, SEEK_SET);
	return true;
}

bool FileStream::OpenWrite(char *filePath, bool append)
{
	if (theFile) fclose(theFile);
	if (FileExists(filePath))
	{
		if (append)
		{
			theFile = fopen(filePath, "ab");
			if (!theFile) return false;
			fputc('\n', theFile);
			//fflush(theFile);
			return true;
		}
	}
	else MakeAllDirs(filePath);
	theFile = fopen(filePath, "wb");
	return theFile != NULL;
}

UInt32 FileStream::GetLength() const
{
	fseek(theFile, 0, SEEK_END);
	UInt32 result = ftell(theFile);
	rewind(theFile);
	return result;
}

void FileStream::SetOffset(UInt32 inOffset)
{
	fseek(theFile, 0, SEEK_END);
	if (ftell(theFile) > inOffset)
		fseek(theFile, inOffset, SEEK_SET);
}

char FileStream::ReadChar()
{
	return (char)fgetc(theFile);
}

void FileStream::ReadBuf(void *outData, UInt32 inLength)
{
	fread(outData, inLength, 1, theFile);
}

void FileStream::WriteChar(char chr)
{
	fputc(chr, theFile);
	//fflush(theFile);
}

void FileStream::WriteStr(const char *inStr)
{
	fputs(inStr, theFile);
	//fflush(theFile);
}

void FileStream::WriteBuf(const void *inData, UInt32 inLength)
{
	fwrite(inData, inLength, 1, theFile);
	//fflush(theFile);
}

int FileStream::WriteFmtStr(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int iWritten = vfprintf(theFile, fmt, args);
	va_end(args);
	//fflush(theFile);
	return iWritten;
}

__declspec(naked) void __fastcall FileStream::MakeAllDirs(char *fullPath)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		lea		edi, [ecx-1]
		ALIGN 16
	iterHead:
		inc		edi
		mov		al, [edi]
		test	al, al
		jz		done
		cmp		al, '/'
		jz		slash
		cmp		al, '\\'
		jnz		iterHead
	slash:
		mov		[edi], 0
		push	0
		push	esi
		call	CreateDirectoryA
		mov		[edi], '\\'
		jmp		iterHead
	done:
		pop		edi
		pop		esi
		retn
	}
}

bool DebugLog::Create(const char *filePath)
{
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile != NULL;
}

const char kIndentLevelStr[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

void DebugLog::Message(const char *msgStr)
{
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	fputs(msgStr, theFile);
	fputc('\n', theFile);
	fflush(theFile);
}

void DebugLog::FmtMessage(const char *fmt, va_list args)
{
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	vfprintf(theFile, fmt, args);
	fputc('\n', theFile);
	fflush(theFile);
}

TempObject<DebugLog> s_log, s_debug;

void PrintLog(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	s_log().FmtMessage(fmt, args);
	va_end(args);
}

void PrintDebug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	s_debug().FmtMessage(fmt, args);
	va_end(args);
}

__declspec(naked) LineIterator::LineIterator(const char *filePath, char *buffer)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		mov		edx, [esp+0xC]
		mov		[ebx], edx
		push	'br'
		push	esp
		push	dword ptr [esp+0x10]
		call	fopen
		add		esp, 0xC
		test	eax, eax
		jz		openFail
		push	esi
		push	edi
		push	SEEK_END
		push	0
		push	eax
		call	fseek
		call	ftell
		mov		edi, eax
		call	rewind
		push	1
		push	edi
		push	dword ptr [ebx]
		call	fread
		add		esp, 0xC
		call	fclose
		add		esp, 0xC
		mov		esi, [ebx]
		mov		word ptr [esi+edi], 0x300
		dec		esi
		ALIGN 16
	iterHead:
		dec		edi
		js		iterEnd
		inc		esi
		mov		al, [esi]
		cmp		al, '\r'
		ja		iterHead
		jz		nullTerm
		cmp		al, '\n'
		jnz		iterHead
	nullTerm:
		mov		[esi], 0
		jmp		iterHead
		NOP_0x9
	iterEnd:
		mov		esi, [ebx]
	findBgn:
		cmp		[esi], 0
		jnz		done
		inc		esi
		jmp		findBgn
		ALIGN 16
	done:
		mov		[ebx], esi
		pop		edi
		pop		esi
		pop		ebx
		retn	8
		ALIGN 16
	openFail:
		mov		edx, [ebx]
		mov		[edx], 3
		pop		ebx
		retn	8
	}
}

UInt32 __fastcall FileToBuffer(const char *filePath, char *buffer)
{
	FileStream srcFile(filePath);
	if (!srcFile) return 0;
	UInt32 length = srcFile.GetLength();
	if (length)
	{
		if (length > kMaxMessageLength)
			length = kMaxMessageLength;
		srcFile.ReadBuf(buffer, length);
		buffer[length] = 0;
	}
	return length;
}

__declspec(naked) void __stdcall SafeWrite8(UInt32 addr, UInt32 data)
{
	__asm
	{
#if LOG_HOOKS
		push	1
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	4
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		[eax], dl
		mov		edx, [esp]
		push	esp
		push	edx
		push	4
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWrite16(UInt32 addr, UInt32 data)
{
	__asm
	{
#if LOG_HOOKS
		push	2
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	4
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		[eax], dx
		mov		edx, [esp]
		push	esp
		push	edx
		push	4
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWrite32(UInt32 addr, UInt32 data)
{
	__asm
	{
#if LOG_HOOKS
		push	4
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	4
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		[eax], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	4
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWriteBuf(UInt32 addr, void *data, UInt32 len)
{
	__asm
	{
#if LOG_HOOKS
		push	dword ptr [esp+0xC]
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	dword ptr [esp+0x18]
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		push	dword ptr [esp+0x10]
		push	dword ptr [esp+0x10]
		push	dword ptr [esp+0x10]
		call	MemCopy
		add		esp, 0xC
		mov		edx, [esp]
		push	esp
		push	edx
		push	dword ptr [esp+0x18]
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	0xC
	}
}

__declspec(naked) void __stdcall WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt)
{
	__asm
	{
#if LOG_HOOKS
		push	5
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	5
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		sub		edx, eax
		sub		edx, 5
		mov		byte ptr [eax], 0xE9
		mov		[eax+1], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	5
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	__asm
	{
#if LOG_HOOKS
		push	5
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	5
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		sub		edx, eax
		sub		edx, 5
		mov		byte ptr [eax], 0xE8
		mov		[eax+1], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	5
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall WritePushRetRelJump(UInt32 baseAddr, UInt32 retAddr, UInt32 jumpTgt)
{
	__asm
	{
#if LOG_HOOKS
		push	0xA
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	0xA
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		byte ptr [eax], 0x68
		mov		[eax+1], edx
		mov		edx, [esp+0x10]
		sub		edx, eax
		sub		edx, 0xA
		mov		byte ptr [eax+5], 0xE9
		mov		[eax+6], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	0xA
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	0xC
	}
}

void __fastcall GetTimeStamp(char *buffer)
{
	time_t rawTime = time(NULL);
	tm timeInfo;
	localtime_s(&timeInfo, &rawTime);
	sprintf_s(buffer, 0x10, "%02d-%02d-%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
}

UInt64 GetTimeMs64()
{
	FILETIME ft;
	LARGE_INTEGER li;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	UInt64 ret = li.QuadPart;
	ret -= 116444736000000000LL;
	ret /= 10000;
	return ret;
}

struct ControlName
{
	UInt32		unk00;
	const char	*name;
	UInt32		unk0C;
};

Setting **g_keyNames = (Setting**)0x11D52F0;
Setting **g_mouseButtonNames = (Setting**)0x11D5240;
Setting **g_joystickNames = (Setting**)0x11D51B0;

const char* __fastcall GetDXDescription(UInt32 keyID)
{
	if (keyID <= 220)
	{
		if (g_keyNames[keyID])
			return g_keyNames[keyID]->data.str;
	}
	else if (keyID <= 255);
	else if (keyID <= 263)
	{
		if (g_mouseButtonNames[keyID - 256])
			return g_mouseButtonNames[keyID - 256]->data.str;
	}
	else if (keyID == 264)
		return "WheelUp";
	else if (keyID == 265)
		return "WheelDown";

	return "<no key>";
}

__declspec(naked) UInt32 __fastcall ByteSwap(UInt32 dword)
{
	__asm
	{
		mov		eax, ecx
		bswap	eax
		retn
	}
}

void DumpMemImg(void *data, UInt32 size, UInt8 extra)
{
	UInt32 *ptr = (UInt32*)data;
	//Console_Print("Output");
	PrintDebug("\nDumping  %08X\n", ptr);
	for (UInt32 iter = 0; iter < size; iter += 4, ptr++)
	{
		if (!extra) PrintDebug("%03X\t\t%08X\t", iter, *ptr);
		else if (extra == 1) PrintDebug("%03X\t\t%08X\t[%08X]\t", iter, *ptr, ByteSwap(*ptr));
		else if (extra == 2) PrintDebug("%04X\t\t%08X\t%f", iter, *ptr, *(float*)ptr);
		else if (extra == 3) PrintDebug("%03X\t\t%08X\t[%08X]\t%f", iter, *ptr, ByteSwap(*ptr), *(float*)ptr);
	}
}