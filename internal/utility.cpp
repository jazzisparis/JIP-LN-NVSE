#include "internal/utility.h"
#include "nvse/GameAPI.h"

const __m128i kPackedValues[] =
{
	_MM_SET_EPI32_4(0x7FFFFFFF),
	_MM_SET_EPI32_1(0x7FFFFFFF),
	_MM_SET_EPI32_4(0x80000000),
	_MM_SET_EPI32_1(0x80000000),
	_MM_SET_EPI32_3(0xFFFFFFFF),
	_MM_SET_EPI64_2(0x7FFFFFFFFFFFFFFF),
	_MM_SET_EPI64_2(0x8000000000000000),
	_MM_SET_EPI32_4(AS_I32(FLT_EPSILON)),
	_MM_SET_EPI32_3(AS_I32(FltPId180)),
	_MM_SET_EPI32_3(AS_I32(Flt180dPI)),
	_MM_SET_EPI32_3(AS_I32(FltPId2)),
	_MM_SET_EPI32_3(AS_I32(FltPI)),
	_MM_SET_EPI32_3(AS_I32(FltPIx2)),
	_MM_SET_EPI32_3(AS_I32(0.5F)),
	_MM_SET_EPI32_3(AS_I32(1.0F)),
	_MM_SET_EPI32_3(0x40DFF8D6),
	_MM_SET_EPI32(AS_I32(0.001F), AS_I32(0.01F), AS_I32(0.1F), AS_I32(0.25F)),
	_MM_SET_EPI32(AS_I32(3.0F), AS_I32(10.0F), AS_I32(100.0F), 0x00000000)
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
};

const __m128i kStringMasks[] =
{
	_MM_SET_EPI32_4(0xFFFFFFFF),
	_MM_SET_EPI32_4(0x00000000),
	_MM_SET_EPI32_4(0x41414141),
	_MM_SET_EPI32_4(0x61616161),
	_MM_SET_EPI32_4(0x7F7F7F7F),
	_MM_SET_EPI32_4(0x19191919),
	_MM_SET_EPI32_4(0x20202020)
};

UInt32 g_TLSIndex;

__declspec(naked) void* __fastcall MemCopy(void *dst, const void *src, size_t size)
{	
	__asm
	{
		push	esi
		push	edi
		push	ecx
		mov		esi, edx
		mov		edi, ecx
		mov		eax, ecx
		sub		eax, edx
		jz		CopyUpReturn
		mov		ecx, [esp+0x10]
		js		CopyUp
		cmp		eax, ecx
		jb		CopyDown
	CopyUp:
		cmp		ecx, 0x20
		jb		CopyUpByteMov
		cmp		ecx, 0x80
		jnb		CopyUpByteMov
		ALIGN 16
	XmmCopySmallLoop:
		movdqu	xmm0, xmmword ptr [esi]
		movdqu	xmm1, xmmword ptr [esi+0x10]
		movdqu	xmmword ptr [edi], xmm0
		movdqu	xmmword ptr [edi+0x10], xmm1
		lea		esi, [esi+0x20]
		lea		edi, [edi+0x20]
		sub		ecx, 0x20
		cmp		ecx, 0x20
		jnb		XmmCopySmallLoop
		test	ecx, ecx
		jz		CopyUpReturn
	CopyUpByteMov:
		rep movsb
	CopyUpReturn:
		pop		eax
		pop		edi
		pop		esi
		retn	4
		ALIGN 16
	CopyDown:
		lea		esi, [esi+ecx]
		lea		edi, [edi+ecx]
		cmp		ecx, 0x20
		jb		CopyDownDwordLoop
		test	edi, 0xF
		jz		XmmMovLargeLoop
		ALIGN 16
	XmmMovAlignLoop:
		dec		ecx
		dec		esi
		dec		edi
		mov		al, [esi]
		mov		[edi], al
		test	edi, 0xF
		jnz		XmmMovAlignLoop
		ALIGN 16
	XmmMovLargeLoop:
		cmp		ecx, 0x80
		jb		XmmMovSmallLoop
		sub		esi, 0x80
		sub		edi, 0x80
		movdqu	xmm0, xmmword ptr [esi]
		movdqu	xmm1, xmmword ptr [esi+0x10]
		movdqu	xmm2, xmmword ptr [esi+0x20]
		movdqu	xmm3, xmmword ptr [esi+0x30]
		movdqu	xmm4, xmmword ptr [esi+0x40]
		movdqu	xmm5, xmmword ptr [esi+0x50]
		movdqu	xmm6, xmmword ptr [esi+0x60]
		movdqu	xmm7, xmmword ptr [esi+0x70]
		movdqa	xmmword ptr [edi], xmm0
		movdqa	xmmword ptr [edi+0x10], xmm1
		movdqa	xmmword ptr [edi+0x20], xmm2
		movdqa	xmmword ptr [edi+0x30], xmm3
		movdqa	xmmword ptr [edi+0x40], xmm4
		movdqa	xmmword ptr [edi+0x50], xmm5
		movdqa	xmmword ptr [edi+0x60], xmm6
		movdqa	xmmword ptr [edi+0x70], xmm7
		sub		ecx, 0x80
		jmp		XmmMovLargeLoop
		ALIGN 16
	XmmMovSmallLoop:
		cmp		ecx, 0x20
		jb		CopyDownDwordLoop
		sub		esi, 0x20
		sub		edi, 0x20
		movdqu	xmm0, xmmword ptr [esi]
		movdqu	xmm1, xmmword ptr [esi+0x10]
		movdqa	xmmword ptr [edi], xmm0
		movdqa	xmmword ptr [edi+0x10], xmm1
		sub		ecx, 0x20
		jmp		XmmMovSmallLoop
		ALIGN 16
	CopyDownDwordLoop:
		cmp		ecx, 4
		jb		CopyDownByte
		sub		edi, 4
		sub		esi, 4
		mov		eax, [esi]
		mov		[edi], eax
		sub		ecx, 4
		jmp		CopyDownDwordLoop
	CopyDownByte:
		test	ecx, ecx
		jz		CopyDownReturn
		ALIGN 16
	CopyDownByteLoop:
		dec		edi
		dec		esi
		mov		al, [esi]
		mov		[edi], al
		dec		ecx
		jnz		CopyDownByteLoop
	CopyDownReturn:
		pop		eax
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) void* __stdcall Game_DoHeapAlloc(size_t size)
{
	__asm
	{
		mov		ecx, GAME_HEAP
		JMP_EAX(0xAA3E40)
	}
}

__declspec(naked) void __stdcall Game_HeapFree(void *ptr)
{
	__asm
	{
		mov		ecx, GAME_HEAP
		JMP_EAX(0xAA4060)
	}
}

__declspec(naked) PrimitiveCS *PrimitiveCS::Enter()
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, Sleep
		ALIGN 16
	spinHead:
		xor		eax, eax
		lock cmpxchg [ecx], esi
		jz		done
		push	0
		call	edi
		mov		ecx, esi
		jmp		spinHead
		ALIGN 16
	done:
		mov		eax, esi
		pop		edi
		pop		esi
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
		jz		incRefCnt
		push	esi
		push	edi
		mov		esi, eax
		mov		edi, Sleep
		ALIGN 16
	spinHead:
		xor		eax, eax
		lock cmpxchg [ebx], esi
		jz		done
		push	0
		call	edi
		jmp		spinHead
		ALIGN 16
	done:
		mov		dword ptr [ebx+4], 1
		pop		edi
		pop		esi
		pop		ebx
		retn
		ALIGN 16
	incRefCnt:
		inc		dword ptr [ebx+4]
		pop		ebx
		retn
	}
}

UInt32 s_CPUFeatures = 0;

__declspec(naked) UInt32 GetCPUFeatures()
{
	__asm
	{
		push	ebx
		xor		ecx, ecx
		mov		eax, 1
		cpuid
		xor		eax, eax
		bt		edx, 0x19
		setc	al
		bt		edx, 0x1A
		setc	dl
		shl		dl, 1
		or		al, dl
		xor		edx, edx
		test	ecx, 1
		setnz	dl
		shl		dl, 2
		or		eax, edx
		bt		ecx, 9
		setc	dl
		shl		dl, 3
		or		eax, edx
		bt		ecx, 0x13
		setc	dl
		shl		dl, 4
		or		eax, edx
		bt		ecx, 0x14
		setc	dl
		shl		dl, 5
		or		eax, edx
		bt		ecx, 0x1C
		setc	dl
		shl		dl, 6
		or		eax, edx
		push	eax
		xor		ecx, ecx
		mov		eax, 7
		cpuid
		xor		edx, edx
		bt		ebx, 5
		setc	dl
		shl		edx, 7
		mov		ecx, edx
		bt		ebx, 0x10
		setc	dl
		shl		edx, 8
		or		ecx, edx
		pop		eax
		or		eax, ecx
		pop		ebx
		retn
	}
}

__declspec(naked) void __vectorcall Coordinate::operator=(__m128 rhs)
{
	__asm
	{
		cvttps2dq	xmm0, xmm0
		psrad	xmm0, 0xC
		pshuflw	xmm1, xmm0, 2
		movd	[ecx], xmm1
		retn
	}
}

__declspec(naked) UInt32 __vectorcall cvtd2ul(double value)
{
	__asm
	{
		lea		eax, [esp-8]
		movlpd	[eax], xmm0
		fld		qword ptr [eax]
		fisttp	qword ptr [eax]
		mov		eax, [eax]
		retn
	}
}

__declspec(naked) double __vectorcall cvtul2d(UInt32 value)
{
	__asm
	{
		push	0
		push	ecx
		fild	qword ptr [esp]
		fstp	qword ptr [esp]
		movq	xmm0, qword ptr [esp]
		add		esp, 8
		retn
	}
}

__declspec(naked) void __fastcall cvtul2d(UInt32 value, double *result)
{
	__asm
	{
		mov		[edx], ecx
		and		dword ptr [edx+4], 0
		fild	qword ptr [edx]
		fstp	qword ptr [edx]
		retn
	}
}

__declspec(naked) int __vectorcall ifloor(float value)
{
	__asm
	{
		test	s_CPUFeatures, 0x10
		jz		noSSE4
		roundss	xmm0, xmm0, 1
		cvttss2si	eax, xmm0
		retn
	noSSE4:
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
		test	s_CPUFeatures, 0x10
		jz		noSSE4
		roundss	xmm0, xmm0, 2
		cvttss2si	eax, xmm0
		retn
	noSSE4:
		push	0x5FA0
		ldmxcsr	[esp]
		cvtss2si	eax, xmm0
		mov		byte ptr [esp+1], 0x1F
		ldmxcsr	[esp]
		pop		ecx
		retn
	}
}

__declspec(naked) float __vectorcall fMod(float numer, float denom)
{
	__asm
	{
		movq	xmm2, xmm0
		divss	xmm2, xmm1
		cvttps2dq	xmm2, xmm2
		cvtdq2ps	xmm2, xmm2
		mulss	xmm2, xmm1
		subss	xmm0, xmm2
		retn
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
		movq	xmm1, xmm0
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
		EMIT_DW(0x3F22F983)
	kCosConsts:
		EMIT_DW(0x37C23AB1)
		EMIT_DW(0x3AB59551)
		EMIT_DW(0x3D2AA76F)
		EMIT_DW(0x3EFFFFE0)
		EMIT_DW(0x3F7FFFFF)
	}
}

__declspec(naked) __m128 __vectorcall Cos_V3(__m128 angles)
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
		EMIT_PS_3(0x3F22F983)
	kQuadTest:
		EMIT_PS_3(0x00000003)
		EMIT_PS_3(0x00000002)
	kCosConsts:
		EMIT_PS_3(0x37C23AB1)
		EMIT_PS_3(0x3AB59551)
		EMIT_PS_3(0x3D2AA76F)
		EMIT_PS_3(0x3EFFFFE0)
		EMIT_PS_3(0x3F7FFFFF)
	}
}

__declspec(naked) __m128 __vectorcall GetSinCos(float angle)
{
	__asm
	{
		movq	xmm1, xmm0
		movss	xmm0, PS_V3_PId2
		subss	xmm0, xmm1
		unpcklps	xmm0, xmm1
		call	Cos_V3
		movshdup	xmm1, xmm0
		retn
	}
}

__declspec(naked) __m128 __vectorcall GetSinCos_V3(__m128 angles)
{
	__asm
	{
		movaps	xmm5, xmm0
		call	Cos_V3
		movaps	xmm4, xmm0
		movaps	xmm0, PS_V3_PId2
		subps	xmm0, xmm5
		call	Cos_V3
		movaps	xmm1, xmm4
		retn
	}
}

__declspec(naked) float __vectorcall Tan(float angle)
{
	_asm
	{
		call	GetSinCos
		movq	xmm2, xmm1
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
		movq	xmm4, xmm0
		andps	xmm4, PS_FlipSignMask0
		xorps	xmm0, xmm4
		movss	xmm3, kASinConsts+0x14
		comiss	xmm0, xmm3
		jnb		ooRange
		movss	xmm1, kASinConsts+0x1C
		paddd	xmm1, xmm0
		movaps	xmm2, kASinConsts
		cvttss2si	eax, xmm1
		movq	xmm1, xmm2
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
		movq	xmm4, xmm0
		andps	xmm4, PS_FlipSignMask0
		xorps	xmm0, xmm4
		movss	xmm3, kASinConsts+0x14
		comiss	xmm0, xmm3
		jnb		ooRange
		movss	xmm1, kASinConsts+0x1C
		paddd	xmm1, xmm0
		movaps	xmm2, kASinConsts
		cvttss2si	eax, xmm1
		movq	xmm1, xmm2
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

__declspec(naked) float __vectorcall ATan2(float y, float x)
{
	__asm
	{
		xorps	xmm2, xmm2
		comiss	xmm0, xmm2
		jz		zeroY
		comiss	xmm1, xmm2
		jz		zeroX
		movq	xmm2, xmm0
		unpcklpd	xmm2, xmm1
		andps	xmm2, PS_AbsMask
		movaps	xmm3, xmm2
		pshufd	xmm4, xmm2, 0xFE
		maxss	xmm3, xmm4
		minss	xmm4, xmm2
		divss	xmm4, xmm3
		movq	xmm3, xmm4
		mulss	xmm3, xmm4
		movq	xmm5, xmm3
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
		EMIT_DW(0xBC5CDD30)
		EMIT_DW(0x3D6B6D55)
		EMIT_DW(0x3DF84C31)
		EMIT_DW(0x3E4854C9)
		EMIT_DW(0x3EAA7E45)
		EMIT_DW(0x3F7FFFB7)
	}
}

__declspec(naked) float __vectorcall Length_V4(__m128 inPS)
{
	__asm
	{
		xorps	xmm1, xmm1
		mulps	xmm0, xmm0
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

__declspec(naked) __m128 __vectorcall Normalize_V4(__m128 inPS)
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
		shufps	xmm0, xmm0, 0
		mulps	xmm0, xmm1
	zeroLen:
        retn
    }
}

__declspec(naked) bool __vectorcall Equal_V3(__m128 v1, __m128 v2)
{
	__asm
	{
		subps	xmm0, xmm1
		pshufd	xmm1, PS_AbsMask0, 0x40
		andps	xmm0, xmm1
		cmpltps	xmm0, PS_Epsilon
		movmskps	eax, xmm0
		cmp		al, 0xF
		setz	al
		retn
	}
}

__declspec(naked) bool __vectorcall Equal_V4(__m128 v1, __m128 v2)
{
	__asm
	{
		subps	xmm0, xmm1
		andps	xmm0, PS_AbsMask
		cmpltps	xmm0, PS_Epsilon
		movmskps	eax, xmm0
		cmp		al, 0xF
		setz	al
		retn
	}
}

__declspec(noinline) char *GetStrArgBuffer()
{
	thread_local static char *s_strBuffer = nullptr;
	if (!s_strBuffer)
		s_strBuffer = (char*)(((UInt32)malloc(STR_BUFFER_SIZE + 0x40) + 0x20) & 0xFFFFFFF0);
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

__declspec(naked) NiRefObject** __stdcall NiReplaceObject(void *toRelease, NiRefObject *toAdd)
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
		xorps	xmm7, xmm7
		mov		edx, ecx
		and		ecx, 0xF
		sub		edx, ecx
		or		eax, 0xFFFFFFFF
		shl		eax, cl
		ALIGN 16
	iterHead:
		movaps	xmm6, [edx]
		pcmpeqb	xmm6, xmm7
		pmovmskb	ecx, xmm6
		and		ecx, eax
		jnz		foundEnd
		or		eax, 0xFFFFFFFF
		add		edx, 0x10
		jmp		iterHead
	foundEnd:
		bsf		eax, ecx
		add		eax, edx
		mov		edx, eax
		pop		ecx
		sub		eax, ecx
		retn
	nullPtr:
		xor		edx, edx
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
		jz		cmpEnd
		repe cmpsd
		jnz		done
	cmpEnd:
		and		dword ptr [esp+0xC], 3
		jz		done
		or		eax, 0xFFFFFFFF
		mov		ecx, [esp+0xC]
		shl		ecx, 3
		shl		eax, cl
		mov		ecx, [esi]
		or		ecx, eax
		or		eax, [edi]
		xor		eax, ecx
	done:
		setz	al
		pop		edi
		pop		esi
		retn	4
	}
}

__declspec(naked) char* __fastcall StrCopy(char *dest, const char *src)
{
	__asm
	{
		mov		eax, ecx
		test	eax, eax
		jz		done
		test	edx, edx
		jz		nullTerm
		ALIGN 16
	iterHead:
		movups	xmm7, [edx]
		xorps	xmm6, xmm6
		pcmpeqb	xmm6, xmm7
		pmovmskb	ecx, xmm6
		test	ecx, ecx
		jnz		foundNull
		movups	[eax], xmm7
		add		eax, 0x10
		add		edx, 0x10
		jmp		iterHead
		ALIGN 16
	foundNull:
		bsf		ecx, ecx
		jz		nullTerm
		push	esi
		push	edi
		mov		esi, edx
		mov		edi, eax
		rep movsb
		mov		eax, edi
		pop		edi
		pop		esi
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
		push	esi
		push	edi
		mov		esi, edx
		mov		edi, ecx
		mov		ecx, [esp+0xC]
		rep movsb
		mov		eax, edi
		pop		edi
		pop		esi
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
		push	edx
		call	StrLen
		mov		ecx, edx
		pop		edx
		jmp		StrCopy
	}
}

__declspec(naked) char __fastcall StrCompareCS(const char *lstr, const char *rstr)
{
	__asm
	{
		test	ecx, ecx
		jz		retnLT
		push	ebx
		push	esi
		push	edi
		lea		esi, [ecx-0x10]
		lea		edi, [edx-0x10]
		and		ecx, 0xF
		sub		esi, ecx
		sub		edi, ecx
		or		ebx, 0xFFFFFFFF
		shl		ebx, cl
		ALIGN 16
	iterHead:
		add		esi, 0x10
		add		edi, 0x10
		movaps	xmm0, [esi]
		movups	xmm1, [edi]
		xorps	xmm2, xmm2
		pcmpeqb	xmm2, xmm0
		pmovmskb	ecx, xmm2
		xorps	xmm2, xmm2
		pcmpeqb	xmm2, xmm1
		pmovmskb	edx, xmm2
		movaps	xmm2, xmm0
		pcmpeqb	xmm2, xmm1
		pmovmskb	eax, xmm2
		not		ax
		and		eax, ebx
		or		edx, ecx
		and		edx, ebx
		jnz		foundEnd
		mov		edx, ebx
		or		ebx, 0xFFFFFFFF
		test	eax, eax
		jz		iterHead
		bsf		ecx, eax
		pcmpgtb	xmm0, xmm1
		pmovmskb	eax, xmm0
		and		eax, edx
		bsf		eax, eax
		cmp		al, cl
		setz	al
		cmovnz	eax, ebx
		add		ecx, edi
		pop		edi
		pop		esi
		pop		ebx
		retn
	retnLT:
		mov		al, 0xFF
		retn
		ALIGN 16
	foundEnd:
		test	eax, eax
		jz		done
		bsf		ecx, eax
		bsf		edx, edx
		xor		eax, eax
		cmp		cl, dl
		ja		done
		pcmpgtb	xmm0, xmm1
		pmovmskb	edx, xmm0
		and		edx, ebx
		bsf		edx, edx
		cmp		cl, dl
		setz	al
		lea		eax, [eax+eax-1]
		add		ecx, edi
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) char __fastcall StrCompareCI(const char *lstr, const char *rstr)
{
	__asm
	{
		test	ecx, ecx
		jz		retnLT
		push	ebx
		push	esi
		push	edi
		lea		esi, [ecx-0x10]
		lea		edi, [edx-0x10]
		and		ecx, 0xF
		sub		esi, ecx
		sub		edi, ecx
		or		ebx, 0xFFFFFFFF
		shl		ebx, cl
		movaps	xmm3, kStringMasks+0x20
		movaps	xmm4, kStringMasks+0x40
		movaps	xmm5, kStringMasks+0x50
		movaps	xmm6, kStringMasks+0x60
		ALIGN 16
	iterHead:
		add		esi, 0x10
		add		edi, 0x10
		movaps	xmm0, [esi]
		movups	xmm1, [edi]
		xorps	xmm2, xmm2
		pcmpeqb	xmm2, xmm0
		pmovmskb	ecx, xmm2
		xorps	xmm2, xmm2
		pcmpeqb	xmm2, xmm1
		pmovmskb	edx, xmm2
		movaps	xmm2, xmm0
		psubb	xmm2, xmm3
		pand	xmm2, xmm4
		pcmpgtb	xmm2, xmm5
		pandn	xmm2, xmm6
		pxor	xmm0, xmm2
		movaps	xmm2, xmm1
		psubb	xmm2, xmm3
		pand	xmm2, xmm4
		pcmpgtb	xmm2, xmm5
		pandn	xmm2, xmm6
		pxor	xmm1, xmm2
		movaps	xmm2, xmm0
		pcmpeqb	xmm2, xmm1
		pmovmskb	eax, xmm2
		not		ax
		and		eax, ebx
		or		edx, ecx
		and		edx, ebx
		jnz		foundEnd
		mov		edx, ebx
		or		ebx, 0xFFFFFFFF
		test	eax, eax
		jz		iterHead
		bsf		ecx, eax
		pcmpgtb	xmm0, xmm1
		pmovmskb	eax, xmm0
		and		eax, edx
		bsf		eax, eax
		cmp		al, cl
		setz	al
		cmovnz	eax, ebx
		add		ecx, edi
		pop		edi
		pop		esi
		pop		ebx
		retn
	retnLT:
		mov		al, 0xFF
		retn
		ALIGN 16
	foundEnd:
		test	eax, eax
		jz		done
		bsf		ecx, eax
		bsf		edx, edx
		xor		eax, eax
		cmp		cl, dl
		ja		done
		pcmpgtb	xmm0, xmm1
		pmovmskb	edx, xmm0
		and		edx, ebx
		bsf		edx, edx
		cmp		cl, dl
		setz	al
		lea		eax, [eax+eax-1]
		add		ecx, edi
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) char __fastcall StrBeginsCS(const char *lstr, const char *rstr)
{
	__asm
	{
		call	StrCompareCS
		test	al, al
		js		retn0
		jz		retn2
		cmp		[ecx], 0
		setz	al
		retn
	retn0:
		xor		al, al
		retn
	retn2:
		mov		al, 2
		retn
	}
}

__declspec(naked) char __fastcall StrBeginsCI(const char *lstr, const char *rstr)
{
	__asm
	{
		call	StrCompareCI
		test	al, al
		js		retn0
		jz		retn2
		cmp		[ecx], 0
		setz	al
		retn
	retn0:
		xor		al, al
		retn
	retn2:
		mov		al, 2
		retn
	}
}

__declspec(naked) void __fastcall StrToCase(char *str, UInt32 upper)
{
	__asm
	{
		mov		eax, offset kStringMasks+0x10
		movaps	xmm2, [eax+edx+0x10]
		movaps	xmm3, [eax+0x30]
		movaps	xmm4, [eax+0x40]
		movaps	xmm5, [eax+0x50]
		ALIGN 16
	iterHead:
		movups	xmm0, [ecx]
		xorps	xmm1, xmm1
		pcmpeqb	xmm1, xmm0
		pmovmskb	edx, xmm1
		movaps	xmm1, xmm0
		psubb	xmm1, xmm2
		pand	xmm1, xmm3
		pcmpgtb	xmm1, xmm4
		pandn	xmm1, xmm5
		test	edx, edx
		jnz		foundEnd
		pxor	xmm0, xmm1
		movups	[ecx], xmm0
		add		ecx, 0x10
		jmp		iterHead
		ALIGN 16
	foundEnd:
		bsf		edx, edx
		sub		eax, edx
		movups	xmm2, [eax]
		pand	xmm1, xmm2
		pxor	xmm0, xmm1
		movups	[ecx], xmm0
		retn
	}
}

__declspec(naked) void __fastcall ReplaceChr(char *str, char from, char to)
{
	__asm
	{
		mov		dh, dl
		movd	xmm7, edx
		pshuflw	xmm7, xmm7, 0
		unpcklpd	xmm7, xmm7
		mov		al, [esp+4]
		mov		ah, al
		movd	xmm6, eax
		pshuflw	xmm6, xmm6, 0
		unpcklpd	xmm6, xmm6
		mov		eax, ecx
		ALIGN 16
	iterHead:
		movups	xmm5, [eax]
		movaps	xmm3, xmm5
		pcmpeqb	xmm3, xmm7
		pmovmskb	ecx, xmm3
		xorps	xmm4, xmm4
		pcmpeqb	xmm4, xmm5
		pmovmskb	edx, xmm4
		test	edx, edx
		jnz		foundEnd
		add		eax, 0x10
		test	ecx, ecx
		jz		iterHead
		movaps	xmm4, xmm3
		pandn	xmm4, xmm5
		pand	xmm3, xmm6
		por		xmm3, xmm4
		movups	[eax-0x10], xmm3
		jmp		iterHead
		ALIGN 16
	foundEnd:
		test	ecx, ecx
		jz		done
		bsf		edx, edx
		mov		ecx, offset kStringMasks+0x10
		sub		ecx, edx
		movups	xmm4, [ecx]
		pand	xmm3, xmm4
		movaps	xmm4, xmm3
		pandn	xmm4, xmm5
		pand	xmm3, xmm6
		por		xmm3, xmm4
		movups	[eax], xmm3
	done:
		retn	4
	}
}

__declspec(naked) char* __fastcall FindChr(const char *str, char chr)
{
	__asm
	{
		mov		dh, dl
		movd	xmm7, edx
		pshuflw	xmm7, xmm7, 0
		unpcklps	xmm7, xmm7
		mov		edx, ecx
		and		ecx, 0xF
		sub		edx, ecx
		or		eax, 0xFFFFFFFF
		shl		eax, cl
		ALIGN 16
	iterHead:
		movaps	xmm6, [edx]
		xorps	xmm5, xmm5
		pcmpeqb	xmm5, xmm6
		pcmpeqb	xmm6, xmm7
		por		xmm5, xmm6
		pmovmskb	ecx, xmm5
		and		ecx, eax
		jnz		foundEnd
		or		eax, 0xFFFFFFFF
		add		edx, 0x10
		jmp		iterHead
	foundEnd:
		bsf		eax, ecx
		add		eax, edx
		xor		ecx, ecx
		cmp		[eax], 0
		cmovz	eax, ecx
		retn
	}
}

__declspec(naked) char* __fastcall FindChrR(const char *str, char chr)
{
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		dh, dl
		movd	xmm7, edx
		pshuflw	xmm7, xmm7, 0
		unpcklpd	xmm7, xmm7
		and		ecx, 0xF
		sub		esi, ecx
		or		edi, 0xFFFFFFFF
		shl		edi, cl
		xor		eax, eax
		ALIGN 16
	iterHead:
		movaps	xmm6, [esi]
		xorps	xmm5, xmm5
		pcmpeqb	xmm5, xmm6
		pmovmskb	ecx, xmm5
		pcmpeqb	xmm6, xmm7
		pmovmskb	edx, xmm6
		and		edx, edi
		and		ecx, edi
		mov		edi, 0xFFFFFFFF
		jnz		foundEnd
		add		esi, 0x10
		bsr		ecx, edx
		jz		iterHead
		lea		eax, [esi+ecx-0x10]
		jmp		iterHead
	foundEnd:
		bsf		ecx, ecx
		shl		edi, cl
		not		edi
		and		edx, edi
		jz		done
		bsr		eax, edx
		add		eax, esi
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) const char* __fastcall SubStrCI(const char *srcStr, const char *subStr)
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		call	StrLen
		test	eax, eax
		jz		retnNull
		mov		ebx, eax
		mov		ecx, edi
		call	StrLen
		test	eax, eax
		jz		retnNull
		sub		ebx, eax
		js		retnNull
		xor		eax, eax
		xor		edx, edx
		ALIGN 16
	iterHead:
		mov		dl, [eax+edi]
		mov		cl, kLwrCaseConverter[edx]
		test	cl, cl
		jz		found
		mov		dl, [eax+esi]
		inc		eax
		cmp		cl, kLwrCaseConverter[edx]
		jz		iterHead
		inc		esi
		xor		eax, eax
		dec		ebx
		jns		iterHead
		ALIGN 16
	retnNull:
		xor		eax, eax
		pop		edi
		pop		esi
		pop		ebx
		retn
		ALIGN 16
	found:
		mov		eax, esi
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

__declspec(naked) char* __fastcall SlashPos(const char *str)
{
	__asm
	{
		movaps	xmm7, kBFSlash
		pshufd	xmm6, xmm7, 0xAA
		unpcklpd	xmm7, xmm7
		mov		edx, ecx
		and		ecx, 0xF
		sub		edx, ecx
		or		eax, 0xFFFFFFFF
		shl		eax, cl
		ALIGN 16
	iterHead:
		movaps	xmm5, [edx]
		xorps	xmm4, xmm4
		movaps	xmm3, xmm5
		pcmpeqb	xmm4, xmm5
		pcmpeqb	xmm3, xmm7
		pcmpeqb	xmm5, xmm6
		por		xmm4, xmm3
		por		xmm4, xmm5
		pmovmskb	ecx, xmm4
		and		ecx, eax
		jnz		foundEnd
		or		eax, 0xFFFFFFFF
		add		edx, 0x10
		jmp		iterHead
	foundEnd:
		bsf		eax, ecx
		add		eax, edx
		xor		ecx, ecx
		cmp		[eax], 0
		cmovz	eax, ecx
		retn
		ALIGN 16
	kBFSlash:
		DUP_2(EMIT_DW(0x2F2F2F2F)) DUP_2(EMIT_DW(0x5C5C5C5C))
	}
}

__declspec(naked) char* __fastcall GetNextToken(char *str, char delim)
{
	__asm
	{
		mov		dh, dl
		movd	xmm7, edx
		pshuflw	xmm7, xmm7, 0
		unpcklpd	xmm7, xmm7
		ALIGN 16
	lookup:
		movups	xmm6, [ecx]
		add		ecx, 0x10
		xorps	xmm5, xmm5
		pcmpeqb	xmm5, xmm6
		pcmpeqb	xmm6, xmm7
		por		xmm5, xmm6
		pmovmskb	eax, xmm5
		bsf		eax, eax
		jz		lookup
		lea		eax, [eax+ecx-0x10]
		ALIGN 16
	nextNon:
		cmp		[eax], dl
		jnz		done
		mov		[eax], 0
		inc		eax
		jmp		nextNon
	done:
		retn
	}
}

__declspec(noinline) char* __fastcall GetNextToken(char *str, const char *delims)
{
	bool table[0x80];
	ZERO_BYTES(table, sizeof(table));
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
		push	esi
		mov		esi, ecx
		call	StrLen
		inc		eax
		push	eax
		call	malloc
		pop		ecx
		push	edi
		mov		edi, eax
		rep movsb
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) char* __fastcall CopyCString(const char *src)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		call	StrLen
		inc		eax
		push	eax
		push	eax
		call	Game_DoHeapAlloc
		pop		ecx
		push	edi
		mov		edi, eax
		rep movsb
		pop		edi
		pop		esi
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
		movlpd	[eax], xmm0
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
		movlpd	[eax], xmm2
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
		ALIGN 8
	kDbl1Mil:
		EMIT_DW_0 EMIT_DW(0x412E8480)
	kIntDivisors:
		EMIT_DW_4(0x0000000A, 0x00000064, 0x000003E8, 0x00002710) EMIT_DW(0x000186A0)
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
		movq	xmm0, qword ptr kStrToDblConsts[eax*8]
		pop		edi
		pop		esi
		retn
		ALIGN 8
	kStrToDblConsts:
		EMIT_DW_4(0x00000000, 0x41F00000, 0x00000000, 0xC1F00000)
		EMIT_DW_4(0xE826D695, 0x3E112E0B, 0xE2308C3A, 0x3E45798E)
		EMIT_DW_4(0x9ABCAF48, 0x3E7AD7F2, 0xA0B5ED8D, 0x3EB0C6F7)
		EMIT_DW_4(0x88E368F1, 0x3EE4F8B5, 0xEB1C432D, 0x3F1A36E2)
		EMIT_DW_4(0xD2F1A9FC, 0x3F50624D, 0x47AE147B, 0x3F847AE1)
		EMIT_DW(0x9999999A) EMIT_DW(0x3FB99999)
	}
}

__declspec(naked) char* __fastcall UIntToHex(char *str, UInt32 num)
{
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
		mov		cl, byte ptr kCharAtlas[eax]
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
		ALIGN 4
	kCharAtlas:
		EMIT_B_8('0', '1', '2', '3', '4', '5', '6', '7')
		EMIT_B_8('8', '9', 'A', 'B', 'C', 'D', 'E', 'F')
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
		ALIGN 16
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

DString::DString(const char *from)
{
	*this = from;
}

DString::DString(const DString &from)
{
	*this = from;
}

DString::DString(UInt16 _alloc) : length(0)
{
	if (_alloc)
	{
		alloc = (_alloc + 0x11) & 0xFFF0;
		str = Pool_CAlloc(alloc);
		*str = 0;
	}
	else
	{
		str = nullptr;
		alloc = 0;
	}
}

void DString::Reserve(UInt16 size)
{
	if (alloc > size) return;
	UInt16 newAlloc = (size + 0x11) & 0xFFF0;
	char *newStr = Pool_CAlloc(newAlloc);
	if (str)
	{
		if (length) COPY_BYTES(newStr, str, length);
		Pool_CFree(str, alloc);
	}
	str = newStr;
	str[length] = 0;
	alloc = newAlloc;
}

DString& DString::operator=(const char *other)
{
	if (length = StrLen(other))
	{
		if (alloc <= length)
		{
			if (str) Pool_CFree(str, alloc);
			alloc = (length + 0x11) & 0xFFF0;
			str = Pool_CAlloc(alloc);
		}
		COPY_BYTES(str, other, length + 1);
	}
	else if (str)
		*str = 0;
	return *this;
}

DString& DString::operator=(const DString &other)
{
	if (length = other.length)
	{
		if (alloc <= length)
		{
			if (str) Pool_CFree(str, alloc);
			alloc = (length + 0x11) & 0xFFF0;
			str = Pool_CAlloc(alloc);
		}
		COPY_BYTES(str, other.str, length + 1);
	}
	else if (str)
		*str = 0;
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
	if (UInt16 otherLen = StrLen(other))
	{
		UInt16 newLen = length + otherLen;
		Reserve(newLen);
		COPY_BYTES(str + length, other, otherLen + 1);
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
		COPY_BYTES(str + length, other.str, other.length + 1);
		length = newLen;
	}
	return *this;
}

bool DString::operator==(const char *other) const
{
	return !StrCompareCS(str, other);
}

bool DString::operator==(const DString &other) const
{
	return (length == other.length) && (!length || !StrCompareCS(str, other.str));
}

DString& DString::Insert(UInt16 index, char chr)
{
	if (index >= length)
		return this->operator+=(chr);
	Reserve(length + 1);
	MemCopy(str + index + 1, str + index, length - index + 1);
	str[index] = chr;
	length++;
	return *this;
}

DString& DString::Insert(UInt16 index, const char *other)
{
	if (index >= length)
		return this->operator+=(other);
	if (UInt16 otherLen = StrLen(other))
	{
		UInt16 newLen = length + otherLen;
		Reserve(newLen);
		MemCopy(str + index + otherLen, str + index, length - index + 1);
		COPY_BYTES(str + index, other, otherLen);
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
		MemCopy(str + index + other.length, str + index, length - index + 1);
		COPY_BYTES(str + index, other.str, other.length);
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
			COPY_BYTES(str + index, str + endIdx, length - endIdx + 1);
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
	if (UInt16 otLen = StrLen(other))
	{
		UInt16 endIdx = bgnIdx + otLen;
		if (endIdx > length)
		{
			Reserve(endIdx);
			length = endIdx;
		}
		COPY_BYTES(str + bgnIdx, other, otLen);
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
		COPY_BYTES(str + bgnIdx, other.str, other.length);
	}
	else length = bgnIdx;
	str[length] = 0;
	return *this;
}

DString DString::SubString(UInt16 bgnIdx, UInt16 endIdx)
{
	char *resStr = nullptr;
	UInt16 resLen = 0, resAlloc = 0;
	if (bgnIdx < length)
	{
		if (endIdx > length)
			endIdx = length;
		resLen = endIdx - bgnIdx;
		if (resLen)
		{
			resAlloc = (resLen + 0x11) & 0xFFF0;
			resStr = Pool_CAlloc(resAlloc);
			COPY_BYTES(resStr, str + bgnIdx, resLen);
			resStr[resLen] = 0;
		}
	}
	return DString(resStr, resLen, resAlloc);
}

DString DString::ToLower()
{
	if (!length) return DString();
	UInt16 resAlloc = (length + 0x11) & 0xFFF0;
	char *resStr = Pool_CAlloc(resAlloc);
	COPY_BYTES(resStr, str, length + 1);
	StrToLower(resStr);
	return DString(resStr, length, resAlloc);
}

DString DString::ToUpper()
{
	if (!length) return DString();
	UInt16 resAlloc = (length + 0x11) & 0xFFF0;
	char *resStr = Pool_CAlloc(resAlloc);
	COPY_BYTES(resStr, str, length + 1);
	StrToUpper(resStr);
	return DString(resStr, length, resAlloc);
}

DString operator+(const DString &lStr, char rChr)
{
	UInt16 resLen = lStr.length + 1, resAlloc = (resLen + 0x11) & 0xFFF0;
	char *resStr = Pool_CAlloc(resAlloc);
	if (lStr.length) COPY_BYTES(resStr, lStr.str, lStr.length);
	*(UInt16*)(resStr + lStr.length) = rChr;
	return DString(resStr, resLen, resAlloc);
}

DString operator+(const DString &lStr, const char *rStr)
{
	char *resStr = nullptr;
	UInt16 rLen = StrLen(rStr), resLen = lStr.length + rLen, resAlloc = 0;
	if (resLen)
	{
		resAlloc = (resLen + 0x11) & 0xFFF0;
		resStr = Pool_CAlloc(resAlloc);
		if (lStr.length) COPY_BYTES(resStr, lStr.str, lStr.length);
		if (rLen) COPY_BYTES(resStr + lStr.length, rStr, rLen);
		resStr[resLen] = 0;
	}
	return DString(resStr, resLen, resAlloc);
}

DString operator+(const char *lStr, const DString &rStr)
{
	char *resStr = nullptr;
	UInt16 lLen = StrLen(lStr), resLen = lLen + rStr.length, resAlloc = 0;
	if (resLen)
	{
		resAlloc = (resLen + 0x11) & 0xFFF0;
		resStr = Pool_CAlloc(resAlloc);
		if (lLen) COPY_BYTES(resStr, lStr, lLen);
		if (rStr.length) COPY_BYTES(resStr + lLen, rStr.str, rStr.length);
		resStr[resLen] = 0;
	}
	return DString(resStr, resLen, resAlloc);
}

__declspec(noinline) void XString::InitFromBuffer(const char *inStr, UInt32 len)
{
	length = len;
	alloc = (length + 0x11) & 0xFFF0;
	str = Pool_CAlloc(alloc);
	COPY_BYTES(str, inStr, length);
	str[length] = 0;
}

void XString::operator=(const XString &other)
{
	if (length = other.length)
	{
		if (alloc <= length)
		{
			if (str) Pool_CFree(str, alloc);
			alloc = (length + 0x11) & 0xFFF0;
			str = Pool_CAlloc(alloc);
		}
		COPY_BYTES(str, other.str, length + 1);
	}
	else if (str)
		*str = 0;
}

void XString::operator=(const char *other)
{
	if (length = StrLen(other))
	{
		if (alloc <= length)
		{
			if (str) Pool_CFree(str, alloc);
			alloc = (length + 0x11) & 0xFFF0;
			str = Pool_CAlloc(alloc);
		}
		COPY_BYTES(str, other, length + 1);
	}
	else if (str)
		*str = 0;
}

bool XString::operator==(const XString &other) const
{
	return (length == other.length) && (!length || !StrCompareCS(str, other.str));
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
	if (theFile = fopen(filePath, "rb"))
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
	return theFile != nullptr;
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
	return theFile != nullptr;
}

bool FileStream::OpenWriteEx(char *filePath, char *buffer, size_t buffSize)
{
	if (theFile) fclose(theFile);
	MakeAllDirs(filePath);
	theFile = fopen(filePath, "wb");
	if (theFile)
	{
		setvbuf(theFile, buffer, _IOFBF, buffSize);
		return true;
	}
	return false;
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
		push	ecx
		ALIGN 16
	iterHead:
		mov		dl, '\\'
		call	FindChr
		test	eax, eax
		jz		done
		mov		esi, eax
		mov		[eax], 0
		call	_mkdir
		mov		[esi], '\\'
		lea		ecx, [esi+1]
		jmp		iterHead
	done:
		pop		ecx
		pop		esi
		retn
	}
}

bool DebugLog::Create(const char *filePath)
{
	theFile = _fsopen(filePath, "wb", _SH_DENYWR);
	return theFile != nullptr;
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
	s_log->FmtMessage(fmt, args);
	va_end(args);
}

void PrintDebug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
#if JIP_DEBUG
	s_debug->FmtMessage(fmt, args);
#else
	s_log->FmtMessage(fmt, args);
#endif
	va_end(args);
}

__declspec(naked) LineIterator::LineIterator(const char *filePath, char *buffer)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		mov		eax, [esp+0xC]
		mov		[eax], 3
		mov		[ebx], eax
		push	'br'
		push	esp
		push	dword ptr [esp+0x10]
		call	fopen
		add		esp, 0xC
		test	eax, eax
		jz		openFail
		push	SEEK_END
		push	0
		push	eax
		call	fseek
		call	ftell
		mov		[esp+8], eax
		call	rewind
		push	1
		push	dword ptr [esp+0xC]
		push	dword ptr [ebx]
		call	fread
		add		esp, 0xC
		call	fclose
		add		esp, 8
		pop		eax
		mov		ecx, [ebx]
		mov		word ptr [eax+ecx], 0x300
		push	'\n'
		mov		dl, '\r'
		call	ReplaceChr
		push	0
		mov		dl, '\n'
		mov		ecx, [ebx]
		call	ReplaceChr
		mov		eax, [ebx]
		ALIGN 16
	findBgn:
		cmp		[eax], 0
		jnz		done
		inc		eax
		jmp		findBgn
	done:
		mov		[ebx], eax
	openFail:
		pop		ebx
		retn	8
	}
}

__declspec(noinline) UInt32 __fastcall FileToBuffer(const char *filePath, char *buffer, UInt32 maxLen)
{
	if (FileStream srcFile(filePath); srcFile)
		if (UInt32 length = srcFile.GetLength())
		{
			if (length > maxLen)
				length = maxLen;
			srcFile.ReadBuf(buffer, length);
			buffer[length] = 0;
			return length;
		}
	return 0;
}

char* __fastcall FileToBuffer(const char *filePath)
{
	if (FileStream srcFile(filePath); srcFile)
		if (UInt32 length = srcFile.GetLength())
		{
			char *buffer = (char*)malloc(length + 1);
			srcFile.ReadBuf(buffer, length);
			buffer[length] = 0;
			return buffer;
		}
	return nullptr;
}

__declspec(naked) void __stdcall SafeWrite8(UInt32 addr, UInt32 data)
{
	__asm
	{
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
		push	4
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWrite16(UInt32 addr, UInt32 data)
{
	__asm
	{
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
		push	4
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWrite32(UInt32 addr, UInt32 data)
{
	__asm
	{
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
		push	4
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWriteBuf(UInt32 addr, const void *data, UInt32 len)
{
	__asm
	{
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	dword ptr [esp+0x18]
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		push	esi
		push	edi
		mov		esi, [esp+0x14]
		mov		edi, [esp+0x10]
		mov		ecx, [esp+0x18]
		rep movsb
		pop		edi
		pop		esi
		mov		edx, [esp]
		push	esp
		push	edx
		push	dword ptr [esp+0x18]
		push	eax
		call	VirtualProtect
		push	dword ptr [esp+0x10]
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	0xC
	}
}

__declspec(naked) void __stdcall WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt)
{
	__asm
	{
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
		push	5
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	__asm
	{
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
		push	5
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall WritePushRetRelJump(UInt32 baseAddr, UInt32 retAddr, UInt32 jumpTgt)
{
	__asm
	{
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
		push	0xA
		push	dword ptr [esp+0xC]
		push	0xFFFFFFFF
		call	FlushInstructionCache
		pop		ecx
		retn	0xC
	}
}

void __fastcall GetTimeStamp(char *buffer)
{
	time_t rawTime = time(nullptr);
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

__declspec(naked) void __fastcall DumpCallStack(UInt32 *EBP, UInt32 depth)
{
	static const char kAddrFmt[] = "%08X\t";
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, edx
		mov		eax, dword ptr s_debug
		mov		ecx, dword ptr s_log
		test	eax, eax
		cmovz	eax, ecx
		push	eax
		push	offset kAddrFmt
		push	eax
		ALIGN 16
	iterHead:
		mov		eax, [esi+4]
		mov		esi, [esi]
		mov		[esp+8], eax
		call	fprintf
		dec		edi
		jnz		iterHead
		push	'\n'
		call	fputc
		pop		ecx
		call	fflush
		add		esp, 0xC
		pop		edi
		pop		esi
		retn
	}
}

void __stdcall DumpMemImg(void *data, UInt32 size, UInt8 extra)
{
	UInt32 *ptr = (UInt32*)data;
	//Console_Print("Output");
	PrintDebug("\nDumping  %08X\n", ptr);
	for (UInt32 iter = 0; iter < size; iter += 4, ptr++)
	{
		if (!extra) PrintDebug("%03X\t\t%08X\t", iter, *ptr);
		else if (extra == 1) PrintDebug("%03X\t\t%08X\t[%08X]\t", iter, *ptr, _byteswap_ulong(*ptr));
		else if (extra == 2) PrintDebug("%04X\t\t%08X\t%f", iter, *ptr, *(float*)ptr);
		else if (extra == 3) PrintDebug("%03X\t\t%08X\t[%08X]\t%f", iter, *ptr, _byteswap_ulong(*ptr), *(float*)ptr);
	}
}