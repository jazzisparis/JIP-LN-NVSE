#pragma once

DEFINE_COMMAND_PLUGIN(InitMiniMap, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(UpdateMiniMap, 0, 2, kParams_OneInt_OneOptionalInt);

#define CACHED_TEXTURES_MAX 60
#define CACHED_TEXTURES_MIN 42

#define TEXTURE_FMT_RGB D3DFMT_X8R8G8B8
#define TEXTURE_FMT_BW D3DFMT_L8

Tile::Value /**s_miniMapVisible, */*s_miniMapScale, *s_localMapZoom;
BSScissorTriShape *s_localMapShapes[9];
NiTriShapeData *s_localMapShapeDatas[9];
UInt32 s_texturePixelSize = 0x110, s_projectPixelSize = 0x100;

__declspec(naked) void UpdateTileScales()
{
	__asm
	{
		mov		eax, s_localMapZoom
		movss	xmm0, [eax+8]
		movss	xmm1, PS_V3_Half
		addss	xmm1, xmm0
		mov		eax, s_miniMapScale
		mulss	xmm1, [eax+8]
		mov		ecx, offset s_localMapShapes
		mov		edx, 8
		ALIGN 16
	iterHead:
		mov		eax, [ecx+edx*4]
		movss	[eax+0x64], xmm1
		dec		dl
		jns		iterHead
		mulss	xmm0, SS_10
		cvttss2si	eax, xmm0
		shl		eax, 4
		add		eax, 0x70
		mov		s_texturePixelSize, eax
		retn
	}
}

float s_cellNorthRotation = 0;
__m128 s_northRotationMods;

__declspec(naked) __m128* __fastcall GetNorthRotation(TESObjectCELL *cell)
{
	__asm
	{
		xor		eax, eax
		test	byte ptr [ecx+0x24], 1
		jz		noRotation
		push	kExtraData_NorthRotation
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		noRotation
		movss	xmm0, [eax+0xC]
		xorps	xmm0, PS_FlipSignMask0
		movss	s_cellNorthRotation, xmm0
		call	GetSinCos
		shufps	xmm0, xmm0, 0x14
		mov		eax, offset s_northRotationMods
		movaps	[eax], xmm0
		xor		byte ptr [eax+0xF], 0x80
		retn
		ALIGN 16
	noRotation:
		mov		s_cellNorthRotation, eax
		retn
	}
}

NiCamera *s_localMapCamera = nullptr;

__declspec(naked) void __fastcall SetCameraRotation(TESObjectCELL *cell)
{
	__asm
	{
		call	GetNorthRotation
		mov		ecx, s_localMapCamera
		mov		edx, offset kNoRotation
		test	eax, eax
		cmovz	eax, edx
		movaps	xmm0, [eax]
		movq	qword ptr [ecx+0x38], xmm0
		movhps	[ecx+0x44], xmm0
		retn
		ALIGN 16
	kNoRotation:
		EMIT_DW_1(00) DUP_2(EMIT_DW(3F, 80, 00, 00)) EMIT_DW_1(00)
	}
}

__declspec(naked) void __fastcall AdjustInteriorPos(const NiVector3 &inPos, NiPoint2 &outPos)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm0, xmm0
		mulps	xmm0, s_northRotationMods
		shufps	xmm0, xmm0, 0x27
		haddps	xmm0, xmm0
	noRotation:
		subps	xmm0, kIntrPosMods
		movq	qword ptr [edx], xmm0
		retn
		ALIGN 16
	kIntrPosMods:
		EMIT_PS_2(45, 00, 00, 00)
	}
}

struct WorldDimensions
{
	NiPoint2	min;		// 00
	NiPoint2	size;		// 08
	float		scale;		// 10
	NiPoint2	offset;		// 14
	NiPoint2	posMod;		// 1C
	int			mkRange;	// 24
	
	void __fastcall GetDimensions(TESWorldSpace *worldSpc);
	void __fastcall GetPosMods(TESWorldSpace *worldSpc);
};
alignas(16) WorldDimensions s_rootWorldDimensions;

__declspec(naked) void __fastcall WorldDimensions::GetDimensions(TESWorldSpace *worldSpc)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		add		edx, 0x80
		movsx	eax, word ptr [edx+8]
		movsx	ecx, word ptr [edx+0xC]
		inc		ecx
		sub		ecx, eax
		mov		[esi], eax
		mov		[esi+8], ecx
		movsx	eax, word ptr [edx+0xA]
		movsx	ecx, word ptr [edx+0xE]
		inc		eax
		sub		ecx, eax
		mov		[esi+4], eax
		mov		[esi+0xC], ecx
		movaps	xmm0, [esi]
		pslld	xmm0, 0xC
		cvtdq2ps	xmm0, xmm0
		movq	qword ptr [esi], xmm0
		movaps	xmm1, kSizeMults
		divps	xmm1, xmm0
		movhps	qword ptr [esi+8], xmm1
		pop		esi
		retn
		ALIGN 16
	kSizeMults:
		DUP_2(EMIT_DW_1(00)) DUP_2(EMIT_DW(3F, 4C, 00, 00))
	}
}

__declspec(naked) void __fastcall WorldDimensions::GetPosMods(TESWorldSpace *worldSpc)
{
	__asm
	{
		add		edx, 0x90
		movups	xmm0, [edx]
		movaps	[ecx+0x10], xmm0
		cmp		dword ptr [edx], 0x3F800000
		jz		done
		movss	xmm1, PS_V3_Half
		mulss	xmm0, xmm1
		subss	xmm1, xmm0
		unpcklps	xmm1, xmm1
		movq	xmm0, qword ptr [edx+0x10]
		movq	xmm2, qword ptr [edx+0x18]
		addps	xmm0, xmm2
		mulps	xmm0, xmm1
		movq	xmm1, qword ptr [edx+4]
		addps	xmm0, xmm1
		movq	qword ptr [ecx+0x1C], xmm0
	done:
		retn
	}
}

__declspec(naked) void __fastcall GetWorldMapPosMults(const NiVector3 &inPos, const WorldDimensions &worldDimensions, NiPoint2 &outMults, Coordinate *outCoord = nullptr)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		lea		eax, [edx+0x14]
		mov		ecx, [edx+0x10]
		cmp		ecx, 0x3F800000
		jz		noScale
		movd	xmm1, ecx
		unpcklps	xmm1, xmm1
		mulps	xmm0, xmm1
		add		eax, 8
	noScale:
		movq	xmm1, qword ptr [eax]
		addps	xmm0, xmm1
		movaps	xmm1, xmm0
		movq	xmm2, qword ptr [edx]
		subps	xmm0, xmm2
		movq	xmm2, qword ptr [edx+8]
		mulps	xmm0, xmm2
		addps	xmm0, kPosMultMods
		mov		ecx, [esp+4]
		movq	qword ptr [ecx], xmm0
		mov		eax, [esp+8]
		test	eax, eax
		jz		done
		cvtps2dq	xmm1, xmm1
		psrad	xmm1, 0xC
		sub		esp, 8
		movq	qword ptr [esp], xmm1
		pop		edx
		pop		dword ptr [eax]
		mov		[eax+2], dx
	done:
		retn	8
		ALIGN 16
	kPosMultMods:
		EMIT_PS_2(3D, D0, 00, 00)
	}
}

__declspec(naked) void __fastcall GetLocalMapPosMults(const NiPoint2 &inPos, const NiPoint2 &nwXY, NiPoint2 &outMults)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		movq	xmm1, qword ptr [edx]
		subps	xmm0, xmm1
		mulps	xmm0, kPosMults
		mov		eax, [esp+4]
		movq	qword ptr [eax], xmm0
		retn	4
		ALIGN 16
	kPosMults:
		EMIT_DW(38, AA, AA, AB) EMIT_DW(B8, AA, AA, AB)
		EMIT_DW_1(00) EMIT_DW_1(00)
	}
}

__declspec(naked) SeenData* __fastcall GetSeenData(TESObjectCELL *cell)
{
	__asm
	{
		push	kExtraData_SeenData
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) IntSeenData* __fastcall GetSectionSeenData(IntSeenData *seenData, SInt16 section)
{
	__asm
	{
	iterHead:
		cmp		[ecx+0x24], dx
		jz		done
		mov		ecx, [ecx+0x28]
		test	ecx, ecx
		jnz		iterHead
	done:
		mov		eax, ecx
		retn
	}
}

GridCellArray *g_gridCellArray;

union SectionSeenInfo
{
	GridCellArray	*cellGrid;
	IntSeenData		*seenData;
};

__declspec(naked) UInt32 __fastcall GetSectionSeenLevel(SectionSeenInfo seenInfo, UInt32 packedBit, Coordinate cellCoord)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	0
		push	ebx
		push	esi
		push	edi
		mov		esi, ecx
		and		edx, 0xF0F
		movzx	ebx, dl
		shl		bl, 4
		or		bl, dh
		cmp		dh, 0xF
		setz	al
		shl		al, 1
		cmp		dl, 0xF
		setz	cl
		or		al, cl
		mov		[ebp-3], al
		mov		ecx, esi
		cmp		byte ptr [ecx], 0xE4
		jnz		isExterior
		mov		dl, [ebp+0xA]
		mov		dh, [ebp+8]
		call	GetSectionSeenData
		mov		edi, eax
		test	eax, eax
		jz		noData0Int
		mov		edx, ebx
		shr		dl, 5
		mov		ecx, [eax+edx*4+4]
		bt		ecx, ebx
		setc	al
		add		[ebp-4], al
	noData0Int:
		mov		ecx, edi
		mov		al, [ebp-3]
		test	al, al
		jz		case0Both
		cmp		al, 2
		jz		case2Int
		cmp		al, 1
		jnz		case3Int
		test	ecx, ecx
		jz		noData1Int
		lea		edx, [ebx+1]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
		add		[ebp-4], al
	noData1Int:
		mov		dl, [ebp+0xA]
		mov		dh, [ebp+8]
		inc		dh
		mov		ecx, esi
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x26], 0
		jnz		caseAdd2
		jmp		case1Both
		ALIGN 16
	case2Int:
		test	ecx, ecx
		jz		noData2Int
		lea		edx, [ebx+0x10]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
		add		[ebp-4], al
	noData2Int:
		mov		dl, [ebp+0xA]
		inc		dl
		mov		dh, [ebp+8]
		mov		ecx, esi
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x26], 0
		jnz		caseAdd2
		jmp		case2Both
		ALIGN 16
	case3Int:
		mov		dl, [ebp+0xA]
		mov		dh, [ebp+8]
		inc		dh
		mov		ecx, esi
		call	GetSectionSeenData
		test	eax, eax
		jz		noData3Int
		mov		edx, ebx
		and		dl, 0xF
		mov		ecx, [eax+4]
		bt		ecx, edx
		setc	al
		add		[ebp-4], al
	noData3Int:
		mov		dl, [ebp+0xA]
		inc		dl
		mov		dh, [ebp+8]
		mov		ecx, esi
		call	GetSectionSeenData
		test	eax, eax
		jz		noData4Int
		mov		edx, ebx
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [eax+edx*4+4]
		bt		eax, ecx
		setc	al
		add		[ebp-4], al
	noData4Int:
		mov		dl, [ebp+0xA]
		inc		dl
		mov		dh, [ebp+8]
		inc		dh
		mov		ecx, esi
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		mov		ecx, [eax+4]
		bt		ecx, 0
		setc	al
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	isExterior:
		push	dword ptr [ebp+8]
		call	GridCellArray::GetCell
		mov		edi, eax
		test	eax, eax
		jz		noDataExt
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		mov		[ebp-2], al
		jnz		modRes0
		call	GetSeenData
		mov		edi, eax
		test	eax, eax
		jz		noDataExt
		mov		edx, ebx
		shr		dl, 5
		mov		ecx, [eax+edx*4+4]
		bt		ecx, ebx
		setc	al
	modRes0:
		add		[ebp-4], al
	noDataExt:
		mov		al, [ebp-2]
		mov		ecx, edi
		mov		dl, [ebp-3]
		cmp		dl, 1
		jz		case1Ext
		cmp		dl, 2
		jz		case2Ext
		test	dl, dl
		jnz		case3Ext
		test	al, al
		jz		case0Both
		add		[ebp-4], 3
		jmp		done
		ALIGN 16
	case0Both:
		test	ecx, ecx
		jz		done
		lea		edx, [ebx+0x10]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
		add		[ebp-4], al
		lea		edx, [ebx+1]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [edi+edx*4+4]
		bt		ecx, eax
		setc	al
		add		[ebp-4], al
		lea		edx, [ebx+0x11]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [edi+edx*4+4]
		bt		ecx, eax
		setc	al
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	case1Ext:
		test	al, al
		jnz		modRes1
		test	ecx, ecx
		jz		getCell2
		lea		edx, [ebx+1]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
	modRes1:
		add		[ebp-4], al
	getCell2:
		mov		eax, [ebp+8]
		inc		ax
		push	eax
		mov		ecx, esi
		call	GridCellArray::GetCell
		test	eax, eax
		jz		done
		test	byte ptr [eax+0x25], 1
		jz		getData1
	caseAdd2:
		add		[ebp-4], 2
		jmp		done
		ALIGN 16
	getData1:
		mov		ecx, eax
		call	GetSeenData
		test	eax, eax
		jz		done
	case1Both:
		mov		edi, eax
		mov		edx, ebx
		and		dl, 0xF
		mov		ecx, [eax+4]
		bt		ecx, edx
		setc	al
		add		[ebp-4], al
		lea		edx, [ebx+1]
		and		dl, 0xF
		mov		ecx, [edi+4]
		bt		ecx, edx
		setc	al
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	case2Ext:
		test	al, al
		jnz		modRes2
		test	ecx, ecx
		jz		getCell3
		lea		edx, [ebx+0x10]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
	modRes2:
		add		[ebp-4], al
	getCell3:
		mov		eax, [ebp+8]
		add		eax, 0x10000
		push	eax
		mov		ecx, esi
		call	GridCellArray::GetCell
		test	eax, eax
		jz		done
		test	byte ptr [eax+0x25], 1
		jz		getData2
		add		[ebp-4], 2
		jmp		done
		ALIGN 16
	getData2:
		mov		ecx, eax
		call	GetSeenData
		test	eax, eax
		jz		done
	case2Both:
		mov		edi, eax
		mov		edx, ebx
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [eax+edx*4+4]
		bt		eax, ecx
		setc	al
		add		[ebp-4], al
		lea		edx, [ebx+0x10]
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [edi+edx*4+4]
		bt		eax, ecx
		setc	al
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	case3Ext:
		mov		eax, [ebp+8]
		inc		ax
		push	eax
		mov		ecx, esi
		call	GridCellArray::GetCell
		test	eax, eax
		jz		getCell4
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		jnz		modRes3
		call	GetSeenData
		test	eax, eax
		jz		getCell4
		mov		edx, ebx
		and		dl, 0xF
		mov		ecx, [eax+4]
		bt		ecx, edx
		setc	al
	modRes3:
		add		[ebp-4], al
	getCell4:
		mov		eax, [ebp+8]
		add		eax, 0x10000
		push	eax
		mov		ecx, esi
		call	GridCellArray::GetCell
		test	eax, eax
		jz		getCell5
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		jnz		modRes4
		call	GetSeenData
		test	eax, eax
		jz		getCell5
		mov		edx, ebx
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [eax+edx*4+4]
		bt		eax, ecx
		setc	al
	modRes4:
		add		[ebp-4], al
	getCell5:
		mov		eax, [ebp+8]
		inc		ax
		add		eax, 0x10000
		push	eax
		mov		ecx, esi
		call	GridCellArray::GetCell
		test	eax, eax
		jz		done
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		jnz		modRes5
		call	GetSeenData
		test	eax, eax
		jz		done
		mov		ecx, [eax+4]
		bt		ecx, 0
		setc	al
	modRes5:
		add		[ebp-4], al
	done:
		movzx	eax, byte ptr [ebp-4]
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn	4
	}
}

TESObjectCELL *s_pcCurrCell0, *s_pcCurrCell, *s_lastInterior;
Coordinate s_packedCellCoords[9] = {0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL};
float s_vertexAlphaLevel[] = {0, 0.25F, 0.5F, 0.75F, 1.0F};

__declspec(naked) void __fastcall CalcVtxAlphaBySeenData(UInt32 gridIdx)
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebx, ecx
		mov		ecx, s_pcCurrCell
		test	byte ptr [ecx+0x24], 1
		jz		isExterior
		call	GetSeenData
		test	eax, eax
		jz		done
		jmp		proceed
	isExterior:
		mov		eax, g_gridCellArray
	proceed:
		mov		ebp, eax
		mov		ecx, s_localMapShapeDatas[ebx*4]
		push	ecx
		mov		esi, [ecx+0x28]
		add		esi, 0xC
		mov		edi, s_packedCellCoords[ebx*4]
		inc		di
		mov		bl, 0x10
		ALIGN 16
	iterHead:
		push	edi
		mov		edx, ebx
		mov		ecx, ebp
		call	GetSectionSeenLevel
		mov		ecx, s_vertexAlphaLevel[eax*4]
		mov		[esi], ecx
		add		esi, 0x10
		mov		al, bl
		shr		al, 4
		sub		di, ax
		dec		bl
		jns		iterHead
		mov		bl, 0x10
		inc		di
		inc		bh
		cmp		bh, 0x10
		jb		iterHead
		ja		iterEnd
		add		edi, 0x10000
		jmp		iterHead
		ALIGN 16
	iterEnd:
		pop		ecx
		mov		ecx, [ecx+0x34]
		test	ecx, ecx
		jz		done
		mov		dword ptr [ecx+0x20], 0
	done:
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
	}
}

__declspec(naked) UInt32 __vectorcall GetFOWUpdateMask(__m128i inPos)
{
	__asm
	{
		movaps	xmm1, xmm0
		unpcklpd	xmm1, xmm0
		andps	xmm1, kTruncMask
		pcmpgtd	xmm1, kGridSlice
		psrld	xmm1, 0x1F
		pshufd	xmm2, xmm1, 0xE
		paddd	xmm1, xmm2
		movd	eax, xmm1
		pextrw	ecx, xmm1, 2
		lea		edx, [eax+eax*2]
		add		ecx, edx
		mov		eax, kUpdateMask[ecx*4]
		retn
		ALIGN 16
	kTruncMask:
		EMIT_PS_4(00, 00, 0F, FF)
	kGridSlice:
		DUP_2(EMIT_DW_2(04, FF)) DUP_2(EMIT_DW_2(0B, 00))
	kUpdateMask:
		EMIT_DW_1(1B) EMIT_DW_1(12) EMIT_DW_1(36)
		EMIT_DW_1(18) EMIT_DW_1(10) EMIT_DW_1(30)
		EMIT_DW_1(D8) EMIT_DW_1(90) EMIT_DW_2(01, B0)
	}
}

__declspec(naked) void __fastcall DoSelectiveFOWUpdate(const NiPoint2 &adjustedPos)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		cvttps2dq	xmm0, xmm0
		call	GetFOWUpdateMask
		push	esi
		push	edi
		mov		esi, eax
		mov		edi, 0xFFFFFFFF
		ALIGN 16
	iterHead:
		inc		edi
		shr		esi, 1
		ja		iterHead
		jnb		done
		mov		ecx, edi
		call	CalcVtxAlphaBySeenData
		jmp		iterHead
		ALIGN 16
	done:
		pop		edi
		pop		esi
		retn
	}
}

bool s_updateFogOfWar = false;

__declspec(naked) bool __fastcall UpdateSeenBits(SeenData *seenData, SInt32 *posPtr)
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		push	0
		push	0x02030304
		push	dword ptr [edx]
		push	0
		lea		ebp, [ecx+4]
		mov		esi, [edx+4]
		mov		bl, 0x10
		ALIGN 16
	iter1Head:
		dec		bl
		js		iter1End
		movzx	eax, bl
		add		eax, [esp+4]
		mov		edx, eax
		neg		eax
		cmovs	eax, edx
		cmp		al, 4
		ja		iter1Head
		mov		cl, [esp+eax+8]
		mov		bh, 0x10
		ALIGN 16
	iter2Head:
		dec		bh
		js		iter1Head
		movzx	eax, bh
		add		eax, esi
		mov		edx, eax
		neg		eax
		cmovs	eax, edx
		cmp		al, cl
		ja		iter2Head
		mov		dl, bh
		shl		dl, 4
		add		dl, bl
		mov		al, dl
		shr		al, 5
		lea		edi, [ebp+eax*4]
		mov		eax, [edi]
		bts		eax, edx
		jc		iter2Head
		mov		[edi], eax
		mov		[esp], 1
		jmp		iter2Head
		ALIGN 16
	iter1End:
		pop		eax
		or		s_updateFogOfWar, al
		pcmpeqd	xmm0, xmm0
		movups	xmm1, [ebp]
		pcmpeqd	xmm1, xmm0
		movmskps	eax, xmm1
		cmp		al, 0xF
		jnz		done
		movups	xmm1, [ebp+0x10]
		pcmpeqd	xmm1, xmm0
		movmskps	eax, xmm1
		cmp		al, 0xF
	done:
		setz	al
		lea		ecx, [ebp-4]
		add		esp, 0xC
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
	}
}

__declspec(naked) SeenData* __fastcall AddExtraSeenData(TESObjectCELL *cell)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		push	kExtraData_SeenData
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		createExtra
		mov		ecx, [eax+0xC]
		test	ecx, ecx
		jz		createData
		mov		eax, ecx
		pop		esi
		retn
	createExtra:
		push	0x10
		GAME_HEAP_ALLOC
		mov		dword ptr [eax], kVtbl_ExtraSeenData
		mov		[eax+4], 5
		mov		dword ptr [eax+8], 0
	createData:
		push	eax
		xorps	xmm0, xmm0
		test	byte ptr [esi+0x24], 1
		jnz		isInterior
		push	0x24
		GAME_HEAP_ALLOC
		mov		dword ptr [eax], kVtbl_SeenData
		jmp		doneData
	isInterior:
		push	0x2C
		GAME_HEAP_ALLOC
		mov		dword ptr [eax], kVtbl_IntSeenData
		movq	qword ptr [eax+0x24], xmm0
		mov		[eax+0x24], bx
	doneData:
		movups	[eax+4], xmm0
		movups	[eax+0x14], xmm0
		pop		ecx
		mov		[ecx+0xC], eax
		push	eax
		push	ecx
		lea		ecx, [esi+0x28]
		CALL_EAX(ADDR_AddExtraData)
		push	0x80000000
		mov		ecx, esi
		CALL_EAX(0x484B60)
		pop		eax
		pop		esi
		retn
	}
}

__declspec(naked) IntSeenData* __fastcall AddIntSeenData(IntSeenData *seenData, SInt16 section)
{
	__asm
	{
	iterHead:
		mov		eax, ecx
		cmp		[ecx+0x24], dx
		jz		found
		mov		ecx, [ecx+0x28]
		test	ecx, ecx
		jnz		iterHead
		push	eax
		push	edx
		push	0x2C
		GAME_HEAP_ALLOC
		pop		edx
		mov		dword ptr [eax], kVtbl_IntSeenData
		xorps	xmm0, xmm0
		movups	[eax+4], xmm0
		movups	[eax+0x14], xmm0
		movq	qword ptr [eax+0x24], xmm0
		mov		[eax+0x24], dx
		pop		ecx
		mov		[ecx+0x28], eax
	found:
		retn
	}
}

__declspec(naked) void UpdateCellsSeenBitsHook()
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x10
		push	ebx
		push	esi
		push	edi
		mov		ebx, g_thePlayer
		mov		esi, [ebx+0x40]
		test	esi, esi
		jz		doneExt
		mov		edi, 0x11CA208
		cmp		s_pcCurrCell0, esi
		jz		sameCell
		mov		s_pcCurrCell0, esi
		mov		ecx, esi
		call	GetNorthRotation
		movq	xmm0, qword ptr [ebx+0x30]
		jmp		skipPosDiff
	sameCell:
		movq	xmm0, qword ptr [ebx+0x30]
		movq	xmm1, qword ptr [edi]
		subps	xmm1, xmm0
		mulps	xmm1, xmm1
		haddps	xmm1, xmm1
		comiss	xmm1, kFlt6400
		jb		doneExt
	skipPosDiff:
		movq	qword ptr [edi], xmm0
		test	byte ptr [esi+0x24], 1
		jnz		isInterior
		mov		eax, g_gridCellArray
		mov		[ebp-0x10], eax
		cvtps2dq	xmm0, xmm0
		call	GetFOWUpdateMask
		mov		edi, eax
		mov		eax, [esi+0x48]
		movq	xmm1, qword ptr [eax]
		psubd	xmm1, kExtPosMod
		movd	eax, xmm1
		mov		[ebp-0xA], ax
		pextrw	eax, xmm1, 2
		mov		[ebp-0xC], ax
		pslld	xmm1, 0xC
		psubd	xmm1, xmm0
		psrad	xmm1, 8
		movq	qword ptr [ebp-8], xmm1
		xor		ebx, ebx
		ALIGN 16
	iterHeadExt:
		movzx	ecx, bl
		lea		eax, [ecx+ecx*2]
		add		al, bh
		bt		edi, eax
		jnc		iterNextExt
		push	dword ptr [ebp-0xC]
		mov		ecx, [ebp-0x10]
		call	GridCellArray::GetCell
		test	eax, eax
		jz		iterNextExt
		test	byte ptr [eax+0x25], 1
		jnz		iterNextExt
		mov		esi, eax
		mov		ecx, eax
		call	AddExtraSeenData
		lea		edx, [ebp-8]
		mov		ecx, eax
		call	UpdateSeenBits
		test	al, al
		jz		iterNextExt
		push	kExtraData_SeenData
		lea		ecx, [esi+0x28]
		CALL_EAX(ADDR_RemoveExtraType)
		or		byte ptr [esi+0x25], 1
		push	0x80000000
		mov		ecx, esi
		CALL_EAX(0x484BC0)
		push	2
		mov		ecx, esi
		CALL_EAX(0x484B60)
	iterNextExt:
		inc		word ptr [ebp-0xC]
		add		dword ptr [ebp-4], 0x10
		inc		bh
		cmp		bh, 2
		jbe		iterHeadExt
		xor		bh, bh
		inc		word ptr [ebp-0xA]
		add		dword ptr [ebp-8], 0x10
		sub		word ptr [ebp-0xC], 3
		sub		dword ptr [ebp-4], 0x30
		inc		bl
		cmp		bl, 2
		jbe		iterHeadExt
	doneExt:
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn
	isInterior:
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm0, xmm0
		mulps	xmm0, s_northRotationMods
		pshufd	xmm1, xmm0, 0x27
		xorps	xmm0, xmm0
		haddps	xmm1, xmm0
		movaps	xmm0, xmm1
	noRotation:
		cvtps2dq	xmm0, xmm0
		psubd	xmm0, kIntPosMod
		call	GetFOWUpdateMask
		mov		edi, eax
		movaps	xmm1, xmm0
		psrad	xmm1, 0xC
		psubd	xmm1, kExtPosMod
		movd	ebx, xmm1
		pextrw	eax, xmm1, 2
		mov		bh, al
		pslld	xmm1, 0xC
		psubd	xmm1, xmm0
		psrad	xmm1, 8
		movq	qword ptr [ebp-8], xmm1
		mov		[ebp-0xC], ebx
		mov		ecx, esi
		call	AddExtraSeenData
		mov		esi, eax
		xor		ebx, ebx
		ALIGN 16
	iterHeadInt:
		movzx	ecx, bl
		lea		eax, [ecx+ecx*2]
		add		al, bh
		bt		edi, eax
		jnc		iterNextInt
		mov		edx, [ebp-0xC]
		add		dh, bh
		add		dl, bl
		mov		ecx, esi
		call	AddIntSeenData
		cmp		byte ptr [eax+0x26], 0
		jnz		iterNextInt
		lea		edx, [ebp-8]
		mov		ecx, eax
		call	UpdateSeenBits
		mov		[ecx+0x26], al
	iterNextInt:
		add		dword ptr [ebp-4], 0x10
		inc		bh
		cmp		bh, 2
		jbe		iterHeadInt
		xor		bh, bh
		add		dword ptr [ebp-8], 0x10
		sub		dword ptr [ebp-4], 0x30
		inc		bl
		cmp		bl, 2
		jbe		iterHeadInt
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn
		ALIGN 16
	kExtPosMod:
		EMIT_PS_2(00, 00, 00, 01)
	kIntPosMod:
		EMIT_PS_2(00, 00, 08, 00)
	kFlt6400:
		EMIT_DW(45, C8, 00, 00)
	}
}

struct DoorRef
{
	TESObjectREFR	*doorRef;
	TESObjectCELL	*linkedCell;

	DoorRef(TESObjectREFR *_doorRef, TESObjectCELL *_linkedCell) : doorRef(_doorRef)
	{
		linkedCell = (_linkedCell && !_linkedCell->worldSpace) ? _linkedCell : nullptr;
	}
};

typedef Vector<DoorRef> DoorRefsList;

void __fastcall GetTeleportDoors(TESObjectCELL *cell, DoorRefsList *doorRefsList)
{
	for (auto iter = g_loadedReferences->teleportDoors.Begin(); iter; ++iter)
	{
		TESObjectREFR *refr = iter.Get();
		if (!refr || (refr->flags & 0x860) || (refr->parentCell != cell))
			continue;
		ExtraTeleport *xTeleport = GetExtraType(&refr->extraDataList, Teleport);
		if (xTeleport && xTeleport->data && xTeleport->data->linkedDoor)
			doorRefsList->Append(refr, xTeleport->data->linkedDoor->parentCell);
	}
}

__declspec(naked) float* __fastcall GetVtxAlphaPtr(const NiPoint2 &posMult)
{
	__asm
	{
		push	ebx
		movq	xmm0, qword ptr [ecx]
		mulps	xmm0, kGridPosMult
		cvttps2dq	xmm0, xmm0
		movshdup	xmm1, xmm0
		movd	eax, xmm0
		movd	ecx, xmm1
		mov		ebx, eax
		shr		ebx, 4
		lea		edx, [ebx+ebx*2]
		mov		ebx, ecx
		shr		ebx, 4
		sub		ebx, 2
		sub		edx, ebx
		and		eax, 0xF
		mov		ebx, eax
		shl		eax, 4
		add		eax, ebx
		and		ecx, 0xF
		add		ecx, eax
		shl		ecx, 4
		mov		ebx, s_localMapShapeDatas[edx*4]
		mov		edx, [ebx+0x28]
		lea		eax, [ecx+edx+0xC]
		pop		ebx
		retn
		ALIGN 16
	kGridPosMult:
		EMIT_PS_2(42, 40, 00, 00)
	}
}

__declspec(naked) void __fastcall GenerateRenderedTexture(TESObjectCELL *cell, D3DFORMAT texFormat, NiCamera *camera, NiTexture **pTexture)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 0xE8
		test	ecx, ecx
		jz		noCell
		mov		cl, [ecx+0x24]
		and		cl, 1
	noCell:
		mov		[ebp-0x1D], cl
		mov		eax, fs:[0x2C]
		mov		ecx, ds:0x126FD98
		mov		ecx, [eax+ecx*4]
		add		ecx, 0x2B4
		mov		eax, [ecx]
		mov		[ebp-0x10], eax
		mov		dword ptr [ecx], 0xE
		mov		byte ptr ds:0x11AD7B4, 0
		push	0
		push	0
		push	0
		push	0x1E
		push	0
		push	edx
		push	0
		cmp		dword ptr [ebp-4], 0
		mov		edx, s_texturePixelSize
		cmovz	edx, s_projectPixelSize
		push	edx
		push	edx
		mov		eax, ds:0x11F9508
		mov		[ebp-0x14], eax
		push	eax
		mov		ecx, ds:0x11F91A8
		CALL_EAX(0xB6D5E0)
		test	eax, eax
		jz		done
		mov		[ebp-0xC], eax
		mov		ecx, [ebp-0x14]
		mov		eax, [ecx+0x5E0]
		mov		[ebp-0x18], eax
		mov		dword ptr [ecx+0x5E0], 0
		push	0
		lea		ecx, [ebp-0xEC]
		CALL_EAX(0x4A0EB0)
		mov		ecx, g_shadowSceneNode
		mov		[ebp-8], ecx
		mov		dl, [ecx+0x130]
		mov		[ebp-0x1E], dl
		mov		[ecx+0x130], 1
		mov		edx, [ebp-0xC]
		mov		edx, [edx+8]
		xor		eax, eax
		mov		ecx, ds:0x11F4748
		cmp		dword ptr [ecx+0x200], 0
		setnz	al
		mov		[ebp-0x20], al
		mov		ecx, 7
		cmovnz	edx, ecx
		cmovnz	ecx, [ebp-0xC]
		push	edx
		push	ecx
		call	kScnCallbacks[eax*4]
		add		esp, 8
		push	0x280
		CALL_EAX(0xAA13E0)
		pop		ecx
		push	0x2F7
		push	1
		push	0x63
		mov		ecx, eax
		CALL_EAX(0xB660D0)
		mov		[ebp-0x28], eax
		mov		byte ptr [eax+0x16C], 0
		mov		edx, [ebp-8]
		mov		[eax+0x194], edx
		mov		dl, [ebp-0x1D]
		neg		dl
		and		edx, 0xC
		mov		[eax+0x19C], edx
		mov		dword ptr [ebp-0x5C], 3
		lea		edx, [ebp-0xEC]
		push	edx
		push	dword ptr [ebp-8]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
		mov		ecx, g_TES
		mov		eax, [ecx+0x34]
		test	eax, eax
		jz		skipPortals
		mov		dword ptr [ebp-0x5C], 1
		lea		edx, [ebp-0xEC]
		push	edx
		mov		ecx, [eax+0xC4]
		mov		eax, [ecx]
		mov		ecx, [eax+0xA0]
		push	dword ptr [ecx+0x18]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
	skipPortals:
		push	dword ptr [ebp-0x28]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6C0D0)
		add		esp, 8
		movzx	eax, byte ptr [ebp-0x20]
		call	kScnCallbacks[eax*4+8]
		mov		al, [ebp-0x1E]
		mov		ecx, [ebp-8]
		mov		[ecx+0x130], al
		mov		eax, ds:0x11F95EC
		mov		[eax+0x86], 1
		mov		ecx, [ebp-0x14]
		mov		eax, [ebp-0x18]
		mov		[ecx+0x5E0], eax
		mov		ecx, [ebp-0xC]
		mov		eax, [ecx+0x30]
		test	eax, eax
		jz		doRelease
		push	eax
		push	dword ptr [ebp+0xC]
		call	NiReleaseAddRef
		mov		ecx, [ebp-0xC]
	doRelease:
		call	NiReleaseObject
	done:
		mov		byte ptr ds:0x11AD7B4, 1
		mov		eax, fs:[0x2C]
		mov		ecx, ds:0x126FD98
		mov		edx, [eax+ecx*4]
		mov		eax, [ebp-0x10]
		mov		[edx+0x2B4], eax
		leave
		retn	8
		ALIGN 16
	kScnCallbacks:
		EMIT_DW_3(B6, B8, D0) EMIT_DW_3(54, ED, E0)
		EMIT_DW_3(B6, B7, 90) EMIT_DW_3(B6, B8, 40)
	}
}

__declspec(naked) void __stdcall GenerateLocalMapExterior(TESObjectCELL *cell, D3DFORMAT texFormat, NiRenderedTexture **renderedTexture)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		mov		ecx, [ebp+8]
		mov		edx, [ecx+0xC4]
		test	edx, edx
		jz		done
		mov		eax, [ecx+0x48]
		mov		ecx, [edx]
		test	ecx, ecx
		jz		done
		sub		esp, 0x28
		movq	xmm0, qword ptr [eax]
		pslld	xmm0, 0xC
		paddd	xmm0, kExtPosMod
		cvtdq2ps	xmm0, xmm0
		mov		eax, [ecx+0xA0]
		mov		ecx, [eax]
		mov		[ebp-4], ecx
		or		byte ptr [ecx+0x30], 1
		mov		ecx, [eax+4]
		mov		[ebp-8], ecx
		or		byte ptr [ecx+0x30], 1
		xorps	xmm1, xmm1
		movups	[ebp-0x18], xmm1
		mov		ecx, [eax+8]
		mov		eax, [ecx+0xA0]
		mov		edx, 4
		ALIGN 16
	iterHead:
		dec		dl
		js		iterEnd
		mov		ecx, [eax+edx*4]
		mov		ecx, [ecx+0xA0]
		mov		ecx, [ecx]
		mov		ecx, [ecx+0x24]
		mov		ecx, [ecx+8]
		test	byte ptr [ecx+0x25], 0x40
		jnz		iterHead
		cmp		dword ptr [ecx+0x38], 0
		jz		iterHead
		mov		[ebp+edx*4-0x18], ecx
		or		byte ptr [ecx+0x25], 0x40
		mov		dword ptr [ecx+0x38], 0
		jmp		iterHead
		ALIGN 16
	iterEnd:
		mov		eax, ds:0x11CCB78
		mov		eax, [eax+4]
		mov		[ebp-0x1C], eax
		or		byte ptr [eax+0x30], 1
		mov		eax, ds:0x11DEDA4
		mov		[ebp-0x20], eax
		or		byte ptr [eax+0x30], 1
		mov		eax, ds:0x11CA438
		mov		[ebp-0x24], eax
		test	eax, eax
		jz		noGrass0
		or		byte ptr [eax+0x30], 1
	noGrass0:
		mov		eax, ds:0x11D5C48
		mov		[ebp-0x28], eax
		mov		byte ptr [eax+0x1B], 1
		push	dword ptr [ebp+0x10]
		mov		ecx, s_localMapCamera
		movq	qword ptr [ecx+0x58], xmm0
		push	ecx
		push	0
		push	offset kUpdateParams
		CALL_EAX(0xA59F90)
		mov		edx, [ebp+0xC]
		mov		ecx, [ebp+8]
		call	GenerateRenderedTexture
		mov		dl, 0xFE
		mov		ecx, [ebp-4]
		and		[ecx+0x30], dl
		mov		ecx, [ebp-8]
		and		[ecx+0x30], dl
		mov		ecx, [ebp-0x1C]
		and		[ecx+0x30], dl
		mov		ecx, [ebp-0x20]
		and		[ecx+0x30], dl
		mov		ecx, [ebp-0x24]
		test	ecx, ecx
		jz		noGrass1
		and		[ecx+0x30], dl
	noGrass1:
		xor		eax, eax
		mov		ecx, [ebp-0x28]
		mov		[ecx+0x1B], al
		mov		edx, 4
		ALIGN 16
	undoIter:
		dec		dl
		js		done
		mov		ecx, [ebp+edx*4-0x18]
		test	ecx, ecx
		jz		undoIter
		and		byte ptr [ecx+0x25], 0xBF
		mov		[ecx+0x38], eax
		jmp		undoIter
	done:
		leave
		retn	0xC
		ALIGN 16
	kExtPosMod:
		EMIT_PS_2(00, 00, 08, 00)
	}
}

__declspec(naked) void __stdcall GenerateLocalMapInterior(TESObjectCELL *cell, Coordinate coord, D3DFORMAT texFormat, NiRenderedTexture **renderedTexture)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		mov		ecx, [ebp+8]
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		test	ecx, ecx
		jz		done
		movsx	edx, word ptr [ebp+0xC]
		inc		edx
		push	edx
		movsx	edx, word ptr [ebp+0xE]
		inc		edx
		push	edx
		movq	xmm0, qword ptr [esp]
		pslld	xmm0, 0xC
		cvtdq2ps	xmm0, xmm0
		cmp		s_cellNorthRotation, 0
		jz		noRot
		shufps	xmm0, xmm0, 0x11
		mulps	xmm0, s_northRotationMods
		haddps	xmm0, xmm0
	noRot:
		mov		eax, [ecx+0xA0]
		mov		ecx, [eax]
		mov		[ebp-4], ecx
		or		byte ptr [ecx+0x30], 1
		mov		ecx, [eax+4]
		mov		[ebp-8], ecx
		or		byte ptr [ecx+0x30], 1
		push	dword ptr [ebp+0x14]
		mov		ecx, s_localMapCamera
		movq	qword ptr [ecx+0x58], xmm0
		push	ecx
		push	0
		push	offset kUpdateParams
		CALL_EAX(0xA59F90)
		mov		edx, [ebp+0x10]
		mov		ecx, [ebp+8]
		call	GenerateRenderedTexture
		mov		ecx, [ebp-4]
		and		byte ptr [ecx+0x30], 0xFE
		mov		ecx, [ebp-8]
		and		byte ptr [ecx+0x30], 0xFE
	done:
		leave
		retn	0x10
	}
}

__declspec(naked) void AttachRefToCellHook()
{
	__asm
	{
		cmp		al, kFormType_BGSProjectile
		jz		contRetn
		cmp		al, kFormType_BGSExplosion
		jz		contRetn
		cmp		byte ptr [ecx+0xF], 0xFF
		jz		contRetn
		mov		ecx, [ebp+8]
		mov		eax, [ecx+0x64]
		mov		ecx, [eax+0x14]
		call	NiNode::GetBSXFlags
		test	eax, eax
		jz		skipRetn
		mov		dl, [eax+0xC]
		and		dl, 0x21
		cmp		dl, 0x21
		jz		contRetn
	skipRetn:
		JMP_EAX(0x549179)
	contRetn:
		JMP_EAX(0x549164)
	}
}

__declspec(naked) void __fastcall SaveLocalMapTexture(TESForm *worldOrCell, NiRenderedTexture *texture, Coordinate coord)
{
	static const char kFileFmt[] = "local_maps\\%s.(%d,%d).dds";
	__asm
	{
		mov		eax, [edx+0x24]
		test	eax, eax
		jz		done
		mov		edx, [eax+0x64]
		test	edx, edx
		jz		done
		push	ebp
		mov		ebp, esp
		sub		esp, 0x80
		push	0
		push	edx
		push	D3DXIFF_DDS
		mov		eax, [ecx]
		call	dword ptr [eax+0x130]
		lea		ecx, [ebp-0x80]
		push	ecx
		movsx	edx, word ptr [ebp+8]
		push	edx
		movsx	edx, word ptr [ebp+0xA]
		push	edx
		push	eax
		push	offset kFileFmt
		push	0x80
		push	ecx
		call	sprintf_s
		add		esp, 0x18
		lea		ecx, [ebp-0x80]
		call	FileStream::MakeAllDirs
		CALL_EAX(0xEE6DC2)
		leave
	done:
		retn	4
	}
}

struct MapMarkerTile
{
	Tile::Value		*visible;
	Tile::Value		*x;
	Tile::Value		*y;
	Tile::Value		*filename;
	bool			inUse;

	MapMarkerTile(Tile *markerTile);

	void SetInUse(bool bInUse)
	{
		inUse = bInUse;
		visible->SetFloat(inUse);
	}
};

TempObject<Vector<MapMarkerTile>> s_mapMarkerTiles(0x40);

MapMarkerTile::MapMarkerTile(Tile *markerTile)
{
	visible = markerTile->GetValue(kTileValue_visible);
	x = markerTile->GetValue(kTileValue_user0);
	y = markerTile->GetValue(kTileValue_user1);
	filename = ThisCall<Tile::Value*>(0xA01000, markerTile, kTileValue_filename);
	markerTile->SetFloat(kTileValue_depth, int((s_mapMarkerTiles().Size() % 10) + 18));
	inUse = true;
}

typedef Vector<MapMarkerTile*, 4> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles> RenderedMapMarkers;
TempObject<RenderedMapMarkers> s_renderedMapMarkers(0x40);

void __fastcall FreeCellMapMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
		dtlIter->SetInUse(false);
	cmkIter.Remove();
}

bool s_discoveredLocation = false;

void __fastcall HandleDiscoverLocation(TESObjectCELL *markerCell)
{
	auto findRendered = s_renderedMapMarkers().Find(Coordinate(&markerCell->coords.exterior->x));
	if (findRendered)
	{
		FreeCellMapMarkers(findRendered);
		s_discoveredLocation = true;
	}
}

__declspec(naked) void DiscoverLocationHook()
{
	__asm
	{
		push	dword ptr ds:0x11D0368
		CALL_EAX(0x8D5100)
		mov		[ebp-0x8F], 1
		mov		ecx, [ebp-0x8C]
		mov		eax, [ecx+4]
		mov		ecx, [eax+0x40]
		test	ecx, ecx
		jz		done
		call	HandleDiscoverLocation
	done:
		retn
	}
}

bool s_updateQuestTargets = false;

__declspec(naked) void SetQuestTargetsHook()
{
	__asm
	{
		add		ecx, 0x4C
		mov		[ebp-4], ecx
		mov		s_updateQuestTargets, 1
		retn
	}
}

__declspec(naked) void __fastcall UpdatePlacedMarkerHook(PlayerCharacter *thePlayer, int EDX, TESObjectREFR *markerRef, ObjectiveTargetData *targetData, int unused)
{
	__asm
	{
		mov		s_updateQuestTargets, 1
		JMP_EAX(0x952D60)
	}
}

bool s_regenTextures = false;

__declspec(naked) void RendererRecreateHook()
{
	__asm
	{
		mov		edx, [esi+8]
		mov		[edx+8], ecx
		mov		[edx+0xC], eax
		mov		s_regenTextures, 1
		retn
	}
}

UInt8 s_miniMapIndex = 0;
TileRect *s_localMapRect, *s_worldMapRect, *s_mapMarkersRect, *s_doorMarkersRect, *s_lQuestMarkersRect, *s_wQuestMarkersRect;
TileImage *s_worldMapTile;
Tile::Value *s_miniMapMode, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom;
TileShaderProperty *s_tileShaderProps[9];
BSFadeNode *s_fakeWaterPlanes;
bool s_defaultGridSize;
NiColor *g_directionalLightColor, *g_shadowFogColor;
BSParticleSystemManager *g_particleSysMngr;

struct MapMarkerInfo
{
	TESObjectREFR		*refr;
	MapMarkerData		*data;
	NiPoint2			pos;

	MapMarkerInfo(TESObjectREFR *_refr, MapMarkerData *_data, const NiPoint2 &posXY) :
		refr(_refr), data(_data), pos(posXY) {}
};
typedef Vector<MapMarkerInfo, 2> CellMapMarkers;
typedef UnorderedMap<UInt32, CellMapMarkers> WorldMapMarkers;
TempObject<Map<TESWorldSpace*, WorldMapMarkers>> s_worldMapMarkers;

const __m128 kWaterPlaneColor = {23 / 255.0F, 51/ 255.0F, 47 / 255.0F, 0.75F};
const NiPoint2 kWaterPlanePos[] =
{
	{2048.0F, -10240.0F}, {2048.0F, -6144.0F}, {2048.0F, -2048.0F},
	{6144.0F, -10240.0F}, {6144.0F, -6144.0F}, {6144.0F, -2048.0F},
	{10240.0F, -10240.0F}, {10240.0F, -6144.0F}, {10240.0F, -2048.0F}
};

bool Cmd_InitMiniMap_Execute(COMMAND_ARGS)
{
	if (s_miniMapIndex == scriptObj->modIndex)
	{
		UInt8 *codePtr = scriptData + 0x2FE5;
		if (*(UInt32*)codePtr == 0x173031)
		{
			*codePtr = 0x33;
			scriptObj->quest->questDelayTime = 1 / 60.0F;
		}
	}
	return true;
}

__declspec(noinline) void InitMiniMap(UInt8 modIdx)
{
	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPMiniMap");
	if (!tile) return;

	s_miniMapIndex = modIdx;
	//s_miniMapVisible = tile->GetValue(kTileValue_visible);
	s_miniMapScale = tile->GetValue(kTileValue_user1);
	auto node = tile->children.Tail()->prev;
	s_pcMarkerRotate = node->data->GetValue(kTileValue_rotateangle);
	tile = node->prev->data;
	s_miniMapMode = tile->GetValue(kTileValue_user0);
	s_miniMapPosX = tile->GetValue(kTileValue_user1);
	s_miniMapPosY = tile->GetValue(kTileValue_user2);
	node = tile->children.Tail()->prev;
	s_localMapRect = (TileRect*)node->data;
	s_localMapZoom = s_localMapRect->GetValue(kTileValue_zoom);
	s_worldMapRect = (TileRect*)node->prev->data;
	s_worldMapZoom = s_worldMapRect->GetValue(kTileValue_zoom);
	node = s_localMapRect->children.Tail();

	NiVector3 *shapeVertices = (NiVector3*)GameHeapAlloc(sizeof(NiVector3) * 0x121);
	NiPoint2 *shapeUVCoords = (NiPoint2*)GameHeapAlloc(sizeof(NiPoint2) * 0x121);
	NiVector3 vertex = {0, 0, 0};
	NiPoint2 uvCoord = {0, 0};
	UInt32 index = 0x11, iterZ = 0x11;
	while (true)
	{
		*shapeVertices++ = vertex;
		*shapeUVCoords++ = uvCoord;
		if (--iterZ)
		{
			vertex.z -= 8.0F;
			uvCoord.y += 0.0625F;
			continue;
		}
		if (!--index) break;
		vertex.x += 8.0F;
		vertex.z = 0;
		uvCoord.x += 0.0625F;
		uvCoord.y = 0;
		iterZ = 0x11;
	}
	shapeVertices -= 0x121;
	shapeUVCoords -= 0x121;

	NiTriangle *shapeTriangles = (NiTriangle*)GameHeapAlloc(sizeof(NiTriangle) << 9);
	UInt32 vtx1 = 0, vtx2 = 0x11;
	do
	{
		index = 0x10;
		do
		{
			shapeTriangles->point1 = vtx2;
			shapeTriangles->point2 = vtx1++;
			shapeTriangles->point3 = vtx1;
			shapeTriangles++;
			shapeTriangles->point1 = vtx1;
			shapeTriangles->point3 = vtx2++;
			shapeTriangles->point2 = vtx2;
			shapeTriangles++;
		}
		while (--index);
		vtx1++;
		vtx2++;
	}
	while (vtx2 < 0x120);
	shapeTriangles -= 0x200;

	NiAlphaProperty *alphaProp = NiAlphaProperty::Create();
	alphaProp->flags = 0x8D;

	index = 0;
	do
	{
		TileImage *localTile = (TileImage*)node->data;
		node = node->prev;
		BSScissorTriShape *sciTriShp = (BSScissorTriShape*)localTile->node->m_children[0];
		s_localMapShapes[index] = sciTriShp;
		NiTriShapeData *shapeData = (NiTriShapeData*)sciTriShp->geometryData;
		s_localMapShapeDatas[index] = shapeData;
		s_tileShaderProps[index] = localTile->shaderProp;
		NiReleaseAddRef(&localTile->shaderProp->srcTexture, nullptr);

		shapeData->numVertices = 0x121;
		shapeData->numTriangles = 0x200;
		shapeData->trianglePoints = 0x600;

		GameHeapFree(shapeData->vertices);
		shapeData->vertices = shapeVertices;

		GameHeapFree(shapeData->normals);
		shapeData->normals = nullptr;

		GameHeapFree(shapeData->vertexColors);
		shapeData->vertexColors = (NiColorAlpha*)GameHeapAlloc(sizeof(NiColorAlpha) * 0x121);
		MemZero(shapeData->vertexColors, sizeof(NiColorAlpha) * 0x121);

		GameHeapFree(shapeData->uvCoords);
		shapeData->uvCoords = shapeUVCoords;

		GameHeapFree(shapeData->triangles);
		shapeData->triangles = shapeTriangles;

		NiReleaseAddRef(&sciTriShp->m_propertyList.Head()->data, alphaProp);
		NiReleaseAddRef(&sciTriShp->alphaProp, alphaProp);
	}
	while (++index < 9);
	UpdateTileScales();

	s_doorMarkersRect = (TileRect*)node->data;
	s_lQuestMarkersRect = (TileRect*)node->prev->data;
	node = s_worldMapRect->children.Tail();
	s_worldMapTile = (TileImage*)node->data;
	s_mapMarkersRect = (TileRect*)node->prev->data;
	s_wQuestMarkersRect = (TileRect*)node->prev->prev->data;

	NiCamera *lmCamera = NiCamera::Create();
	s_localMapCamera = lmCamera;
	lmCamera->LocalRotate() = {0, 0, 1.0F, 0, 1.0F, 0, -1.0F, 0, 0};
	lmCamera->LocalTranslate().z = 65536.0F;
	lmCamera->frustum.viewPort = {-2048.0F, 2048.0F, 2048.0F, -2048.0F};
	lmCamera->frustum.n = 100.0F;
	lmCamera->frustum.f = 131072.0F;
	lmCamera->frustum.o = 1;
	lmCamera->LODAdjust = 0.001F;
	g_shadowSceneNode->AddObject(lmCamera, 1);

	BSFadeNode *waterParent = BSFadeNode::Create();

	shapeVertices = (NiVector3*)GameHeapAlloc(sizeof(NiVector3) << 2);
	shapeVertices[0] = {-2048.0F, 2048.0F, 0};
	shapeVertices[1] = {-2048.0F, -2048.0F, 0};
	shapeVertices[2] = {2048.0F, 2048.0F, 0};
	shapeVertices[3] = {2048.0F, -2048.0F, 0};

	NiColorAlpha *vertexColors = (NiColorAlpha*)GameHeapAlloc(sizeof(NiColorAlpha) << 2);
	vertexColors[0] = kWaterPlaneColor;
	vertexColors[1] = kWaterPlaneColor;
	vertexColors[2] = kWaterPlaneColor;
	vertexColors[3] = kWaterPlaneColor;

	shapeTriangles = (NiTriangle*)GameHeapAlloc(sizeof(NiTriangle) << 1);
	shapeTriangles[0] = {0, 1, 2};
	shapeTriangles[1] = {2, 1, 3};

	NiTriShapeData *shapeData = ThisCall<NiTriShapeData*>(0xA7B630, NiAllocator(sizeof(NiTriShapeData)), 4, shapeVertices, nullptr, vertexColors, nullptr, 0, 0, 2, shapeTriangles);

	alphaProp = NiAlphaProperty::Create();
	alphaProp->flags = 0x10ED;

	index = 0;
	do
	{
		NiTriShape *waterPlane = ThisCall<NiTriShape*>(0xA74480, NiAllocator(sizeof(NiTriShape)), shapeData);
		waterPlane->AddProperty(alphaProp);
		waterPlane->AddProperty(BSShaderNoLightingProperty::Create());
		waterPlane->m_flags |= 1;
		waterPlane->LocalTranslate() = kWaterPlanePos[index];
		waterParent->AddObject(waterPlane, 1);
	}
	while (++index < 9);

	waterParent->RenderGeometryAndShader();
	g_shadowSceneNode->AddObject(waterParent, 1);
	s_fakeWaterPlanes = waterParent;

	auto worldIter = g_dataHandler->worldSpaceList.Head();
	TESWorldSpace *worldSpc, *rootWorld = nullptr;
	TESObjectCELL *rootCell;
	WorldMapMarkers *worldMarkers;
	TESObjectREFR *markerRef;
	MapMarkerData *markerData;
	NiPoint2 posXY;
	Coordinate coord;
	do
	{
		if (!(worldSpc = worldIter->data) || !(rootCell = worldSpc->cell))
			continue;
		s_rootWorldDimensions.GetPosMods(worldSpc);
		worldSpc = worldSpc->GetRootMapWorld();
		if (rootWorld != worldSpc)
		{
			rootWorld = worldSpc;
			s_rootWorldDimensions.GetDimensions(rootWorld);
			worldMarkers = nullptr;
		}
		auto refrIter = rootCell->objectList.Head();
		do
		{
			if (!(markerRef = refrIter->data) || !(markerData = markerRef->GetMapMarkerData()))
				continue;
			GetWorldMapPosMults(markerRef->position, s_rootWorldDimensions, posXY, &coord);
			if (!worldMarkers)
				worldMarkers = &s_worldMapMarkers()[rootWorld];
			(*worldMarkers)[coord].Append(markerRef, markerData, posXY);
		}
		while (refrIter = refrIter->next);
	}
	while (worldIter = worldIter->next);

	g_gridCellArray = g_TES->gridCellArray;
	s_defaultGridSize = *(UInt8*)0x11C63D0 <= 5;
	g_directionalLightColor = &g_TES->directionalLight->ambientColor;
	g_shadowFogColor = &(*(BSFogProperty**)0x11DEB00)->color;
	g_particleSysMngr = BSParticleSystemManager::GetSingleton();
	SafeWrite16(0x452736, 0x7705);
	SafeWrite8(0x555C20, 0xC3);
	WriteRelCall(0x9438F6, (UInt32)UpdateCellsSeenBitsHook);
	WriteRelJump(0x54914D, (UInt32)AttachRefToCellHook);
	WritePushRetRelJump(0x779567, 0x7795E8, (UInt32)DiscoverLocationHook);
	WritePushRetRelJump(0x60F13A, 0x60F145, (UInt32)SetQuestTargetsHook);
	WriteRelCall(0x952C69, (UInt32)UpdatePlacedMarkerHook);
	WriteRelCall(0x952F6B, (UInt32)UpdatePlacedMarkerHook);
	WritePushRetRelJump(0xE7D13A, 0xE7D144, (UInt32)RendererRecreateHook);

	TESModel *baseModel = DYNAMIC_CAST(LookupFormByRefID(0x15A1F2), TESForm, TESModel);
	if (baseModel)
		baseModel->SetPath("Clutter\\BlackRefBlock256.NIF");
}

const __m128 kVertexAlphaMults = {0.25, 0.5, 0.75, 1};
alignas(16) const NiColor kDirectionalLightValues[] = {{1.0F, 1.0F, 239 / 255.0F}, {0, 0, 0}};
alignas(16) const float kFogPropertyValues[] = {23 / 255.0F, 51 / 255.0F, 47 / 255.0F, FLT_MAX, FLT_MAX};
const UInt8 kSelectImgUpdate[][9] =
{
	{8, 2, 0, 4, 1, 0, 0, 0, 0},
	{0, 0, 0, 8, 2, 0, 4, 1, 0},
	{0, 8, 2, 0, 4, 1, 0, 0, 0},
	{0, 0, 0, 0, 8, 2, 0, 4, 1}
};
const Coordinate kGridAdjustCoord[] =
{
	{-1, -1}, {-1, 0}, {-1, 1},
	{0, -1}, {0, 0}, {0, 1},
	{1, -1}, {1, 0}, {1, 1}
};
alignas(16) const int kNWCoordAdjust[] = {-1, 2, 0, 0};

struct ExteriorEntry
{
	NiRenderedTexture	*texture;
	UInt8				regenFlags;

	ExteriorEntry() : texture(nullptr), regenFlags(0) {}
};

struct DoorMarkerTile
{
	Tile::Value		*visible;
	Tile::Value		*x;
	Tile::Value		*y;
	Tile::Value		*visited;
	float			*vtxAlpha;
	bool			inUse;

	DoorMarkerTile(Tile *markerTile);
};

TempObject<Vector<DoorMarkerTile>> s_doorMarkerTiles(0x40);

DoorMarkerTile::DoorMarkerTile(Tile *markerTile)
{
	visible = markerTile->GetValue(kTileValue_visible);
	x = markerTile->GetValue(kTileValue_user0);
	y = markerTile->GetValue(kTileValue_user1);
	visited = ThisCall<Tile::Value*>(0xA01000, markerTile, kTileValue_user2);
	markerTile->SetFloat(kTileValue_depth, int((s_doorMarkerTiles().Size() % 10) + 18));
}

struct QuestMarkerTile
{
	UInt32			pos;
	Tile::Value		*x;
	Tile::Value		*y;
};

UInt32 s_currentMapMode = 0;
TESWorldSpace *s_pcCurrWorld, *s_pcRootWorld;
Coordinate s_currWorldCoords(0x7FFF, 0x7FFF), s_currLocalCoords(0x7FFF, 0x7FFF);
WorldMapMarkers *s_currWorldMarkers;
TempObject<Set<UInt32>> s_currCellsSet(0xC), s_currMarkerCells(0x40);
TempObject<UnorderedMap<UInt32, ExteriorEntry>> s_renderedExteriors(0x80);
TempObject<Vector<UInt32>> s_exteriorKeys(0x60);
TempObject<UnorderedMap<UInt32, NiRenderedTexture*>> s_renderedInterior(0x20);
TempObject<UnorderedMap<UInt32, DoorRefsList>> s_exteriorDoorRefs(0x20);
TempObject<DoorRefsList> s_doorRefsList(0x40);
TESQuest *s_activeQuest = nullptr;
TESObjectREFR *s_placedMarker = nullptr;
TempObject<Map<TESObjectREFR*, QuestMarkerTile>> s_questMarkers(0x10);
TESObjectCELL *s_currCellGrid[9] = {};
UInt8 s_currCellQuad = 0;
bool s_useFogOfWar = false;

void MiniMapLoadGame()
{
	s_pcCurrCell0 = nullptr;
	s_pcCurrCell = nullptr;
	s_pcCurrWorld = nullptr;
	s_pcRootWorld = nullptr;
	s_lastInterior = nullptr;
}

bool Cmd_UpdateMiniMap_Execute(COMMAND_ARGS)
{
	UInt32 updateType, showFlags = 0x100;
	if ((scriptObj->modIndex != s_miniMapIndex) || !ExtractArgsEx(EXTRACT_ARGS_EX, &updateType, &showFlags))
		return true;

	if (updateType == 3)
	{
		UpdateTileScales();
		s_regenTextures = true;
		if (showFlags == 0x100)
			return true;
	}

	PlayerCharacter *thePlayer = g_thePlayer;
	TESObjectCELL *parentCell = thePlayer->parentCell;
	if (!parentCell) return true;
	TESWorldSpace *parentWorld = parentCell->worldSpace;

	bool updateTiles = s_pcCurrCell != parentCell;
	if (updateTiles)
	{
		s_pcCurrCell = parentCell;
		SetCameraRotation(parentCell);
	}

	bool updQuestTargets = s_updateQuestTargets;
	s_updateQuestTargets = false;

	if (s_currentMapMode != updateType)
	{
		if (updateType >= 2)
			s_regenTextures = true;
		else
		{
			s_currentMapMode = updateType;
			s_miniMapMode->SetFloat((int)updateType);
		}
		updateTiles = true;
		updQuestTargets = true;
	}

	bool worldMap = s_currentMapMode != 0;
	NiPoint2 nwXY, posMult, adjustedPos;
	Coordinate coord;
	Tile *markerTile;
	TESObjectREFR *objectRef;
	HUDMainMenu *hudMain = g_HUDMainMenu;

	bool useFogOfWar = (showFlags & 0x10) != 0;
	if (s_useFogOfWar != useFogOfWar)
	{
		s_useFogOfWar = useFogOfWar;
		updateTiles = true;
		for (TileShaderProperty *shaderProp : s_tileShaderProps)
			shaderProp->hasVtxColors = useFogOfWar;
		if (useFogOfWar)
		{
			__asm
			{
				mov		eax, s_tileShaderProps
				movss	xmm0, [eax+0x78]
				shufps	xmm0, xmm0, 0
				mulps	xmm0, kVertexAlphaMults
				movups	s_vertexAlphaLevel+4, xmm0
			}
		}
	}

	if (worldMap)
	{
		if (parentWorld)
			objectRef = thePlayer;
		else
		{
			parentWorld = g_TES->currentWrldspc;
			if (!parentWorld) return true;
			objectRef = thePlayer->lastExteriorDoor;
			if (!objectRef) return true;
		}

		if (s_pcCurrWorld != parentWorld)
		{
			s_pcCurrWorld = parentWorld;
			s_rootWorldDimensions.GetPosMods(parentWorld);
			TESWorldSpace *rootWorld = parentWorld->GetRootMapWorld();
			if (s_pcRootWorld != rootWorld)
			{
				s_pcRootWorld = rootWorld;
				s_rootWorldDimensions.GetDimensions(rootWorld);
				s_worldMapTile->SetString(kTileValue_filename, rootWorld->worldMap.ddsPath.m_dataLen ? rootWorld->worldMap.ddsPath.m_data : "jazzisparis\\minimap\\blanktile.dds");
				s_worldMapRect->SetFloat(kTileValue_user1, rootWorld->mapData.usableDimensions.X);
				s_worldMapRect->SetFloat(kTileValue_user2, rootWorld->mapData.usableDimensions.Y);
				s_currWorldMarkers = s_worldMapMarkers().GetPtr(rootWorld);
				s_rootWorldDimensions.mkRange = iceil(72.0F / rootWorld->mapData.usableDimensions.X *
					(rootWorld->mapData.cellSECoordinates.X - rootWorld->mapData.cellNWCoordinates.X +
					rootWorld->mapData.cellNWCoordinates.Y - rootWorld->mapData.cellSECoordinates.Y));
				if (showFlags & 1)
				{
					for (auto tileIter = s_mapMarkerTiles().Begin(); tileIter; ++tileIter)
						if (tileIter().inUse) tileIter().SetInUse(false);
					s_renderedMapMarkers().Clear();
				}
			}
			updateTiles = true;
		}

		GetWorldMapPosMults(objectRef->position, s_rootWorldDimensions, posMult, &coord);

		if (s_currWorldCoords != coord)
		{
			s_currWorldCoords = coord;
			updateTiles = true;
		}

		if (showFlags & 1)
		{
			if (updateTiles || s_discoveredLocation)
			{
				s_discoveredLocation = false;
				s_currMarkerCells().Clear();
				if (s_currWorldMarkers)
				{
					int range = iceil(s_rootWorldDimensions.mkRange / s_worldMapZoom->num);
					coord.x -= range;
					coord.y -= range;
					range = (range * 2) + 1;
					auto tileIter = s_mapMarkerTiles().Begin();
					CellMapMarkers *mapMarkers;
					DynamicTiles *dynamicTiles;
					MapMarkerTile *tileData;
					UInt32 iterX = range, iterY;
					do
					{
						iterY = range;
						do
						{
							if (!(mapMarkers = s_currWorldMarkers->GetPtr(coord)))
								goto iterYnext;
							s_currMarkerCells().Insert(coord);
							if (!s_renderedMapMarkers().Insert(coord, &dynamicTiles))
								goto iterYnext;
							for (auto mkIter = mapMarkers->Begin(); mkIter; ++mkIter)
							{
								if (!(mkIter().data->flags & 1) || (mkIter().refr->flags & 0x800))
									continue;
								while (true)
								{
									if (!tileIter)
									{
										tileData = s_mapMarkerTiles().Append(hudMain->AddTileFromTemplate(s_mapMarkersRect, "MiniMapWorldMarkerTemplate"));
										break;
									}
									tileData = &tileIter();
									++tileIter;
									if (tileData->inUse) continue;
									tileData->SetInUse(true);
									break;
								}
								dynamicTiles->Append(tileData);
								tileData->x->SetFloat(mkIter().pos.x);
								tileData->y->SetFloat(mkIter().pos.y);
								tileData->filename->SetString((mkIter().data->flags & 2) ? ((const char**)0x11A0404)[mkIter().data->type] : (const char*)0x1075030);
							}
						iterYnext:
							coord.y++;
						}
						while (--iterY);
						coord.x++;
						coord.y -= range;
					}
					while (--iterX);
				}
				for (auto cmkIter = s_renderedMapMarkers().Begin(); cmkIter; ++cmkIter)
					if (!s_currMarkerCells().HasKey(cmkIter.Key())) FreeCellMapMarkers(cmkIter);
			}
		}
		else if (!s_renderedMapMarkers().Empty())
		{
			s_mapMarkerTiles().Clear();
			s_renderedMapMarkers().Clear();
			s_mapMarkersRect->DestroyAllChildren();
		}
	}
	else
	{
		TESObjectCELL *cell;
		NiNode *intDynamicNode;
		UInt32 gridIdx, lightingPasses;
		D3DFORMAT texFormat = (showFlags & 8) ? TEXTURE_FMT_RGB : TEXTURE_FMT_BW;
		NiPointLight *pntLight;
		bool restore = false, showDoors = (showFlags & 2) != 0, updateFogOfWar = useFogOfWar && s_updateFogOfWar/*, saveToFile = (showFlags & 0x20) != 0*/;
		s_updateFogOfWar = false;

		if (s_regenTextures)
		{
			s_regenTextures = false;
			if (!s_exteriorKeys().Empty())
			{
				for (auto prgIter = s_renderedExteriors().Begin(); prgIter; ++prgIter)
					if (prgIter().texture) ThisCall(0xA7FD30, prgIter().texture, true);
				s_renderedExteriors().Clear();
				s_exteriorKeys().Clear();
			}
			s_lastInterior = nullptr;
			updateTiles = true;
		}

		if (parentWorld)
		{
			UInt8 quadrant;
			__asm
			{
				mov		eax, g_TES
				movq	xmm0, qword ptr [eax+0x2C]
				movd	eax, xmm0
				shl		eax, 0x10
				pextrw	edx, xmm0, 2
				or		eax, edx
				mov		coord, eax
				paddd	xmm0, kNWCoordAdjust
				pslld	xmm0, 0xC
				cvtdq2ps	xmm0, xmm0
				lea		edx, nwXY
				movq	qword ptr [edx], xmm0
				lea		ecx, posMult
				push	ecx
				mov		ecx, thePlayer
				add		ecx, 0x30
				call	GetLocalMapPosMults
				cmpnltps	xmm0, PS_V3_Half
				movmskps	eax, xmm0
				xor		al, 0xA
				mov		quadrant, al
			}

			if (s_currLocalCoords != coord)
			{
				s_currLocalCoords = coord;
				updateTiles = true;
			}

			if (updateTiles)
			{
				s_currCellsSet().Clear();
				gridIdx = 0;
				do
				{
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					cell = g_gridCellArray->GetCell(coord);
					s_currCellGrid[gridIdx] = cell;
					if (!cell) continue;
					s_currCellsSet().Insert(cell->refID);
					if (useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				while (++gridIdx < 9);
				if (showDoors)
				{
					DoorRefsList *listPtr;
					for (auto clsIter = s_currCellsSet().Begin(); clsIter; ++clsIter)
						if (s_exteriorDoorRefs().Insert(*clsIter, &listPtr))
							GetTeleportDoors((TESObjectCELL*)LookupFormByRefID(*clsIter), listPtr);
					s_doorRefsList().Clear();
					for (auto drlIter = s_exteriorDoorRefs().Begin(); drlIter; ++drlIter)
						if (s_currCellsSet().HasKey(drlIter.Key()))
							s_doorRefsList().Concatenate(drlIter());
						else drlIter.Remove();
				}
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(thePlayer->position);

			if (updateTiles || (s_defaultGridSize && (s_currCellQuad != quadrant)))
			{
				s_currCellQuad = quadrant;
				restore = true;
				GameGlobals::SceneLightsLock()->Enter();
				for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
				{
					if ((pntLight = (NiPointLight*)lgtNode->data->light) && (pntLight->effectType == 2))
					{
						pntLight->radius0E4 = pntLight->radius;
						pntLight->radius = 0;
					}
				}
				GameGlobals::SceneLightsLock()->Leave();
				memcpy(g_directionalLightColor, kDirectionalLightValues, sizeof(kDirectionalLightValues));
				memcpy(g_shadowFogColor, kFogPropertyValues, sizeof(kFogPropertyValues));
				*(UInt8*)0x11FF104 = 1;
				g_particleSysMngr->m_flags |= 1;

				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]))
						continue;
					cell->Get3DNode(4)->m_flags |= 1;
					if (!(cell->cellFlags & 2))
						continue;
					NiAVObject *waterPlane = s_fakeWaterPlanes->m_children[gridIdx];
					waterPlane->m_flags &= ~1;
					if (updateTiles)
						waterPlane->LocalTranslate().z = (cell->waterHeight == FLT_MAX) ? parentWorld->defaultWaterHeight : cell->waterHeight;
				}
				while (++gridIdx < 9);
				if (updateTiles)
				{
					s_fakeWaterPlanes->LocalTranslate() = nwXY;
					ThisCall(0xA5DD70, s_fakeWaterPlanes, &kUpdateParams, 0);
				}

				const UInt8 *updateList = kSelectImgUpdate[quadrant];
				ExteriorEntry *exteriorEntry;
				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]) || !(updateTiles || (quadrant = updateList[gridIdx])))
						continue;

					if (s_renderedExteriors().Insert(cell->refID, &exteriorEntry))
					{
						GenerateLocalMapExterior(cell, texFormat, &exteriorEntry->texture);
						s_exteriorKeys().Append(cell->refID);
						/*if (saveToFile)
							SaveLocalMapTexture(parentWorld, exteriorEntry->texture, s_currLocalCoords + kGridAdjustCoord[gridIdx]);*/
					}
					else if (updateTiles)
						s_exteriorKeys().MoveToEnd(cell->refID);
					else if (!(exteriorEntry->regenFlags & quadrant))
					{
						if (gridIdx == 4) exteriorEntry->regenFlags = 0xF;
						else exteriorEntry->regenFlags |= quadrant;
						GenerateLocalMapExterior(cell, texFormat, &exteriorEntry->texture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentWorld, exteriorEntry->texture, s_currLocalCoords + kGridAdjustCoord[gridIdx]);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = exteriorEntry->texture;
				}
				while (++gridIdx < 9);

				if (s_exteriorKeys().Size() > CACHED_TEXTURES_MAX)
				{
					UInt32 size = s_exteriorKeys().Size() - CACHED_TEXTURES_MIN;
					gridIdx = size;
					UnorderedMap<UInt32, ExteriorEntry>::Iterator findTex(*s_renderedExteriors);
					do
					{
						gridIdx--;
						findTex.Find(s_exteriorKeys()[gridIdx]);
						if (!findTex) continue;
						if (findTex().texture)
							ThisCall(0xA7FD30, findTex().texture, true);
						findTex.Remove();
					}
					while (gridIdx);
					s_exteriorKeys().RemoveRange(0, size);
				}
			}
		}
		else
		{
			__asm
			{
				lea		edx, adjustedPos
				mov		ecx, thePlayer
				add		ecx, 0x30
				call	AdjustInteriorPos
				cvttps2dq	xmm0, xmm0
				psrad	xmm0, 0xC
				movd	eax, xmm0
				shl		eax, 0x10
				pextrw	ecx, xmm0, 2
				or		eax, ecx
				mov		coord, eax
				mov		ecx, edx
				paddd	xmm0, kNWCoordAdjust
				pslld	xmm0, 0xC
				cvtdq2ps	xmm0, xmm0
				lea		edx, nwXY
				movq	qword ptr [edx], xmm0
				lea		eax, posMult
				push	eax
				call	GetLocalMapPosMults
			}

			if (s_lastInterior != parentCell)
			{
				s_lastInterior = parentCell;
				if (!s_renderedInterior().Empty())
				{
					for (auto clrIter = s_renderedInterior().Begin(); clrIter; ++clrIter)
						if (*clrIter) ThisCall(0xA7FD30, *clrIter, true);
					s_renderedInterior().Clear();
				}
				updateTiles = true;
			}

			if (showDoors && updateTiles)
			{
				s_doorRefsList().Clear();
				GetTeleportDoors(parentCell, *s_doorRefsList);
			}

			if (s_currLocalCoords != coord)
			{
				s_currLocalCoords = coord;
				updateTiles = true;
			}

			if (updateTiles)
			{
				restore = true;
				intDynamicNode = parentCell->Get3DNode(4);
				intDynamicNode->m_flags |= 1;
				g_particleSysMngr->m_flags |= 1;
				lightingPasses = *GameGlobals::LightingPasses();
				*GameGlobals::LightingPasses() = 0x34;

				NiRenderedTexture **renderedTexture;
				gridIdx = 0;
				do
				{
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					if (s_renderedInterior().Insert(coord, &renderedTexture))
					{
						*renderedTexture = nullptr;
						GenerateLocalMapInterior(parentCell, coord, texFormat, renderedTexture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentCell, *renderedTexture, coord);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = *renderedTexture;
					if (useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				while (++gridIdx < 9);
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(adjustedPos);
		}

		if (restore)
		{
			if (parentWorld)
			{
				for (auto cellIter : s_currCellGrid)
					if (cellIter) cellIter->Get3DNode(4)->m_flags &= ~1;

				GameGlobals::SceneLightsLock()->Enter();
				for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
					if ((pntLight = (NiPointLight*)lgtNode->data->light) && (pntLight->effectType == 2))
						pntLight->radius = pntLight->radius0E4;
				GameGlobals::SceneLightsLock()->Leave();

				for (auto plnIter = s_fakeWaterPlanes->m_children.Begin(); plnIter; ++plnIter)
					plnIter->m_flags |= 1;
			}
			else
			{
				intDynamicNode->m_flags &= ~1;
				*GameGlobals::LightingPasses() = lightingPasses;
			}
			g_particleSysMngr->m_flags &= ~1;
		}

		if (showDoors)
		{
			if (updateTiles)
			{
				UInt8 offBounds;
				auto markerIter = s_doorMarkerTiles().Begin();
				DoorMarkerTile *tileData;
				for (auto doorIter = s_doorRefsList().Begin(); doorIter; ++doorIter)
				{
					objectRef = doorIter().doorRef;
					__asm
					{
						lea		edx, adjustedPos
						push	edx
						mov		ecx, objectRef
						add		ecx, 0x30
						cmp		parentWorld, 0
						jnz		getMults
						call	AdjustInteriorPos
						mov		ecx, edx
					getMults:
						lea		edx, nwXY
						call	GetLocalMapPosMults
						unpcklpd	xmm0, xmm0
						movq	xmm1, qword ptr PS_V3_One
						cmpnltps	xmm0, xmm1
						movmskps	eax, xmm0
						xor		al, 0xC
						mov		offBounds, al
					}
					if (offBounds) continue;
					if (markerIter)
					{
						tileData = &markerIter();
						++markerIter;
					}
					else tileData = s_doorMarkerTiles().Append(hudMain->AddTileFromTemplate(s_doorMarkersRect, "MiniMapDoorMarkerTemplate"));
					tileData->x->SetFloat(adjustedPos.x);
					tileData->y->SetFloat(adjustedPos.y);
					cell = doorIter().linkedCell;
					tileData->visited->SetFloat(!cell || (cell == parentCell) || cell->extraDataList.HasType(kExtraData_DetachTime));
					if (useFogOfWar)
						tileData->vtxAlpha = GetVtxAlphaPtr(adjustedPos);
					else
					{
						tileData->vtxAlpha = nullptr;
						tileData->visible->SetFloat(1.0F);
					}
					tileData->inUse = true;
				}

				for (; markerIter; ++markerIter)
				{
					if (!markerIter().inUse) continue;
					markerIter().inUse = false;
					markerIter().visible->SetFloat(0);
				}
			}

			if (updateTiles || updateFogOfWar)
			{
				for (auto markerIter = s_doorMarkerTiles().Begin(); markerIter; ++markerIter)
					if (markerIter().inUse && markerIter().vtxAlpha)
						markerIter().visible->SetFloat(*markerIter().vtxAlpha > 0);
			}
		}
		else if (!s_doorMarkerTiles().Empty())
		{
			s_doorMarkerTiles().Clear();
			s_doorMarkersRect->DestroyAllChildren();
		}
	}

	if (showFlags & 4)
	{
		if (s_activeQuest != thePlayer->activeQuest)
		{
			s_activeQuest = thePlayer->activeQuest;
			updQuestTargets = true;
		}
		if (s_placedMarker != thePlayer->placedMarkerRef)
		{
			s_placedMarker = thePlayer->placedMarkerRef;
			updQuestTargets = true;
		}
		QuestMarkerTile *markerData;
		if (updQuestTargets)
		{
			if (!s_questMarkers().Empty())
			{
				s_questMarkers().Clear();
				s_lQuestMarkersRect->DestroyAllChildren();
				s_wQuestMarkersRect->DestroyAllChildren();
			}
			if (!worldMap || parentCell->worldSpace)
			{
				auto trgIter = thePlayer->questTargetList.Head();
				ObjectiveTarget *targetData;
				TileRect *targetsRect = worldMap ? s_wQuestMarkersRect : s_lQuestMarkersRect;
				int depth = 29;
				bool donePlaced = false;
				while (true)
				{
					if (trgIter)
					{
						targetData = trgIter->data;
						trgIter = trgIter->next;
						if (!targetData) continue;
					}
					else if (!donePlaced)
					{
						donePlaced = true;
						targetData = (ObjectiveTarget*)&thePlayer->unk6E8;
					}
					else break;
					if (!(objectRef = targetData->target))
						continue;
					if (targetData->data.teleportLinks.size)
					{
						if (!(objectRef = targetData->data.teleportLinks[0].door))
							continue;
					}
					else if ((parentCell != objectRef->parentCell) && (!parentWorld || (parentWorld != objectRef->GetParentWorld())))
						continue;
					if (s_questMarkers().Insert(objectRef, &markerData))
					{
						markerTile = hudMain->AddTileFromTemplate(targetsRect, "MiniMapQuestMarkerTemplate");
						markerData->pos = 0x7F7FFFFF;
						markerData->x = markerTile->GetValue(kTileValue_user0);
						markerData->y = markerTile->GetValue(kTileValue_user1);
						markerTile->SetFloat(kTileValue_depth, depth++);
					}
					if (donePlaced)
					{
						Tile *mkTile = markerData->x->parent;
						mkTile->SetFloat(kTileValue_red, 191.0F);
						mkTile->SetFloat(kTileValue_green, 127.0F);
						mkTile->SetFloat(kTileValue_blue, 255.0F);
						break;
					}
				}
			}
		}

		if (!s_questMarkers().Empty())
		{
			for (auto mkrIter = s_questMarkers().Begin(); mkrIter; ++mkrIter)
			{
				objectRef = mkrIter.Key();
				markerData = &mkrIter();
				UInt32 currX = *(UInt32*)&objectRef->position;
				if (markerData->pos != currX)
					markerData->pos = currX;
				else if (!updateTiles) continue;
				if (worldMap)
					GetWorldMapPosMults(objectRef->position, s_rootWorldDimensions, adjustedPos);
				else if (parentWorld)
					GetLocalMapPosMults(objectRef->position, nwXY, adjustedPos);
				else
				{
					AdjustInteriorPos(objectRef->position, adjustedPos);
					GetLocalMapPosMults(adjustedPos, nwXY, adjustedPos);
				}
				markerData->x->SetFloat(adjustedPos.x);
				markerData->y->SetFloat(adjustedPos.y);
			}
		}
	}
	else if (!s_questMarkers().Empty())
	{
		s_questMarkers().Clear();
		s_lQuestMarkersRect->DestroyAllChildren();
		s_wQuestMarkersRect->DestroyAllChildren();
	}

	s_pcMarkerRotate->SetFloat(s_cellNorthRotation - thePlayer->rotation.z);
	s_miniMapPosX->SetFloat(posMult.x);
	s_miniMapPosY->SetFloat(posMult.y);
	return true;
}