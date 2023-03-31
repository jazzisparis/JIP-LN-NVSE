#pragma once

DEFINE_COMMAND_PLUGIN(InitMiniMap, 0, 0, nullptr);
DEFINE_COMMAND_PLUGIN(UpdateMiniMap, 0, 2, kParams_OneInt_OneOptionalInt);

#define CACHED_TEXTURES_MAX 60
#define CACHED_TEXTURES_MIN 42

#define TEXTURE_FMT_RGB D3DFMT_X8R8G8B8
#define TEXTURE_FMT_BW D3DFMT_L8

constexpr float kInteriorZoomMod = 0.3F;

struct TextureParams
{
	UInt32		width;
	UInt32		height;
	D3DFORMAT	d3dFormat;

	TextureParams(UInt32 _width, UInt32 _height, D3DFORMAT _d3dFmt) : width(_width), height(_height), d3dFormat(_d3dFmt) {}
}
s_mmTextureParams(0x180, 0x180, TEXTURE_FMT_RGB);

TileValue /**s_miniMapVisible, */*s_miniMapScale, *s_localMapZoom;
BSScissorTriShape *s_localMapShapes[9];

__declspec(naked) void __vectorcall UpdateTileScales(float fZoom)
{
	__asm
	{
		movss	xmm1, PS_V3_Half
		addss	xmm1, xmm0
		mov		eax, s_miniMapScale
		mulss	xmm1, [eax+8]
		movd	edx, xmm1
		mov		eax, 8
		ALIGN 16
	iterHead:
		mov		ecx, s_localMapShapes[eax*4]
		mov		[ecx+0x64], edx
		dec		eax
		jns		iterHead
		mov		ecx, s_localMapZoom
		jmp		TileValue::SetFloat
	}
}

float s_cellNorthRotation = 0;
__m128 s_northRotationMods;

__declspec(naked) __m128 __vectorcall GetNorthRotation(TESObjectCELL *cell)
{
	__asm
	{
		xor		eax, eax
		test	byte ptr [ecx+0x24], 1
		jz		noRotation
		push	kXData_ExtraNorthRotation
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		noRotation
		movss	xmm0, [eax+0xC]
		xorps	xmm0, PS_FlipSignMask0
		movss	s_cellNorthRotation, xmm0
		call	GetSinCos
		unpcklpd	xmm0, xmm0
		xorps	xmm0, PS_FlipSignMask0
		shufps	xmm0, xmm0, 0x16
		movaps	s_northRotationMods, xmm0
		retn
		ALIGN 16
	noRotation:
		mov		s_cellNorthRotation, eax
		pshufd	xmm0, PS_V3_One, 0xC3
		retn
	}
}

__declspec(naked) __m128 __vectorcall AdjustInteriorPos(__m128 inPos)
{
	__asm
	{
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm0, xmm0
		mulps	xmm0, s_northRotationMods
		shufps	xmm0, xmm0, 0x27
		haddps	xmm0, xmm0
	noRotation:
		subps	xmm0, kIntrPosMods
		retn
		ALIGN 16
	kIntrPosMods:
		EMIT_PS_2(45, 00, 00, 00)
	}
}

struct WorldDimensions
{
	NiPoint2		min;		// 00
	NiPoint2		size;		// 08
	float			scale;		// 10
	NiPoint2		offset;		// 14
	NiPoint2		posMod;		// 1C
	union						// 24
	{
		int			mkRange;
		UInt32		coord;
	};
	
	void __fastcall GetDimensions(TESWorldSpace *worldSpc);
	void __fastcall GetPosMods(TESWorldSpace *worldSpc);
}
s_rootWorldDimensions;

__declspec(naked) void __fastcall WorldDimensions::GetDimensions(TESWorldSpace *worldSpc)
{
	__asm
	{
		movq	xmm0, qword ptr [edx+0x88]
		paddw	xmm0, kSizeMults
		unpcklpd	xmm0, xmm0
		pshuflw	xmm0, xmm0, 0xFA
		pshufhw	xmm0, xmm0, 0x50
		psrad	xmm0, 0x10
		movaps	xmm1, xmm0
		psrldq	xmm1, 8
		psubd	xmm0, xmm1
		pslld	xmm0, 0xC
		cvtdq2ps	xmm0, xmm0
		pshufd	xmm1, xmm0, 0xE
		rcpps	xmm0, xmm0
		movq	xmm2, qword ptr kSizeMults+8
		mulps	xmm0, xmm2
		unpcklpd	xmm1, xmm0
		movups	[ecx], xmm1
		retn
		ALIGN 16
	kSizeMults:
		EMIT_DW(00, 01, 00, 00) EMIT_DW_1(01) DUP_2(EMIT_DW(3F, 4C, 00, 00))
	}
}

__declspec(naked) void __fastcall WorldDimensions::GetPosMods(TESWorldSpace *worldSpc)
{
	__asm
	{
		add		edx, 0x90
		movups	xmm0, [edx]
		movups	[ecx+0x10], xmm0
		cmp		dword ptr [edx], 0x3F800000
		jz		done
		movss	xmm1, PS_V3_Half
		mulss	xmm0, xmm1
		subss	xmm1, xmm0
		unpcklps	xmm1, xmm1
		movups	xmm0, [edx+0x10]
		shufps	xmm0, xmm0, 0xD8
		haddps	xmm0, xmm0
		mulps	xmm0, xmm1
		movq	xmm1, qword ptr [edx+4]
		addps	xmm0, xmm1
		movlps	[ecx+0x1C], xmm0
	done:
		retn
	}
}

__declspec(naked) __m128 __vectorcall GetWorldMapPosMults(__m128 inPos, const WorldDimensions &worldDimensions, Coordinate *outCoord = nullptr)
{
	__asm
	{
		lea		eax, [ecx+0x14]
		cmp		dword ptr [ecx+0x10], 0x3F800000
		jz		noScale
		movss	xmm1, [ecx+0x10]
		unpcklps	xmm1, xmm1
		mulps	xmm0, xmm1
		add		eax, 8
	noScale:
		movq	xmm1, qword ptr [eax]
		addps	xmm0, xmm1
		movq	xmm1, xmm0
		movq	xmm2, qword ptr [ecx]
		subps	xmm0, xmm2
		movq	xmm2, qword ptr [ecx+8]
		mulps	xmm0, xmm2
		addps	xmm0, kPosMultMods
		test	edx, edx
		jz		done
		cvtps2dq	xmm1, xmm1
		psrad	xmm1, 0xC
		pshuflw	xmm1, xmm1, 2
		movd	[edx], xmm1
	done:
		retn
		ALIGN 16
	kPosMultMods:
		EMIT_PS_2(3D, D0, 00, 00)
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

__declspec(naked) UInt32 __vectorcall GetGridCellSeenData(GridCellArray *gridArray, __m128i cellXY)
{
	__asm
	{
		push	ebx
		mov		ebx, ecx
		movd	eax, xmm0
		movsx	edx, ax
		sar		eax, 0x10
		mov		ecx, [ebx+0xC]
		shr		ecx, 1
		sub		eax, [ebx+4]
		add		eax, ecx
		cmp		eax, [ebx+0xC]
		jnb		retnNull
		sub		edx, [ebx+8]
		add		edx, ecx
		cmp		edx, [ebx+0xC]
		jnb		retnNull
		add		edx, eax
		shl		eax, cl
		add		edx, eax
		mov		eax, [ebx+0x10]
		mov		ecx, [eax+edx*4]
		test	ecx, ecx
		jz		retnNull
		mov		eax, 1
		test	byte ptr [ecx+0x25], 1
		jnz		done
		push	kXData_ExtraSeenData
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		pop		ebx
		retn
	retnNull:
		xor		eax, eax
		pop		ebx
		retn
	}
}

union SectionSeenInfo
{
	GridCellArray	*cellGrid;
	IntSeenData		*seenData;
};

__declspec(naked) UInt32 __vectorcall GetSectionSeenLevel(SectionSeenInfo seenInfo, UInt32 packedBit, __m128i cellCoord)
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebp, ecx
		and		edx, 0xF0F
		movzx	eax, dl
		shl		al, 4
		or		al, dh
		mov		esi, eax
		cmp		dh, 0xF
		setz	bl
		shl		bl, 1
		cmp		dl, 0xF
		setz	bh
		or		bh, bl
		xor		bl, bl
		pshufd	xmm1, xmm0, 0
		paddw	xmm1, kSeenLvlConsts
		cmp		byte ptr [ecx], 0xE4
		jnz		isExterior
		pshufb	xmm1, kSeenLvlConsts+0x10
		pextrw	edx, xmm1, 0
		call	GetSectionSeenData
		mov		edi, eax
		test	eax, eax
		jz		noData0Int
		mov		edx, esi
		shr		dl, 5
		mov		ecx, [eax+edx*4+4]
		bt		ecx, esi
		setc	bl
	noData0Int:
		mov		ecx, edi
		test	bh, bh
		jz		case0Both
		cmp		bh, 2
		jz		case2Int
		cmp		bh, 1
		jnz		case3Int
		test	ecx, ecx
		jz		noData1Int
		lea		edx, [esi+1]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
		add		bl, al
	noData1Int:
		pextrw	edx, xmm1, 1
		mov		ecx, ebp
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
		lea		edx, [esi+0x10]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
		add		bl, al
	noData2Int:
		pextrw	edx, xmm1, 2
		mov		ecx, ebp
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		cmp		byte ptr [eax+0x26], 0
		jnz		caseAdd2
		jmp		case2Both
		ALIGN 16
	case3Int:
		pextrw	edx, xmm1, 1
		mov		ecx, ebp
		call	GetSectionSeenData
		test	eax, eax
		jz		noData3Int
		mov		edx, esi
		and		dl, 0xF
		mov		ecx, [eax+4]
		bt		ecx, edx
		setc	al
		add		bl, al
	noData3Int:
		pextrw	edx, xmm1, 2
		mov		ecx, ebp
		call	GetSectionSeenData
		test	eax, eax
		jz		noData4Int
		mov		edx, esi
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [eax+edx*4+4]
		bt		eax, ecx
		setc	al
		add		bl, al
	noData4Int:
		pextrw	edx, xmm1, 3
		mov		ecx, ebp
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		mov		ecx, [eax+4]
		bt		ecx, 0
		setc	al
		add		bl, al
		jmp		done
		ALIGN 16
	isExterior:
		movq	xmm0, xmm1
		call	GetGridCellSeenData
		xor		edi, edi
		test	eax, eax
		jz		noDataExt
		cmp		eax, 1
		cmovnz	edi, eax
		setz	bl
		jz		noDataExt
		mov		edx, esi
		shr		dl, 5
		mov		ecx, [eax+edx*4+4]
		bt		ecx, esi
		setc	bl
		xor		al, al
	noDataExt:
		mov		ecx, edi
		cmp		bh, 1
		jz		case1Ext
		cmp		bh, 2
		jz		case2Ext
		test	bh, bh
		jnz		case3Ext
		test	al, al
		jz		case0Both
		add		bl, 3
		jmp		done
		ALIGN 16
	case0Both:
		test	ecx, ecx
		jz		done
		lea		edx, [esi+0x10]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
		add		bl, al
		lea		edx, [esi+1]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [edi+edx*4+4]
		bt		ecx, eax
		setc	al
		add		bl, al
		lea		edx, [esi+0x11]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [edi+edx*4+4]
		bt		ecx, eax
		setc	al
		add		bl, al
		jmp		done
		ALIGN 16
	case1Ext:
		test	al, al
		jnz		modRes1
		test	ecx, ecx
		jz		getCell2
		lea		edx, [esi+1]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
	modRes1:
		add		bl, al
	getCell2:
		pshufd	xmm0, xmm1, 1
		mov		ecx, ebp
		call	GetGridCellSeenData
		test	eax, eax
		jz		done
		cmp		eax, 1
		jnz		case1Both
	caseAdd2:
		add		bl, 2
		jmp		done
		ALIGN 16
	case1Both:
		mov		edi, eax
		mov		edx, esi
		and		dl, 0xF
		mov		ecx, [eax+4]
		bt		ecx, edx
		setc	al
		add		bl, al
		lea		edx, [esi+1]
		and		dl, 0xF
		mov		ecx, [edi+4]
		bt		ecx, edx
		setc	al
		add		bl, al
		jmp		done
		ALIGN 16
	case2Ext:
		test	al, al
		jnz		modRes2
		test	ecx, ecx
		jz		getCell3
		lea		edx, [esi+0x10]
		mov		eax, edx
		shr		dl, 5
		mov		ecx, [ecx+edx*4+4]
		bt		ecx, eax
		setc	al
	modRes2:
		add		bl, al
	getCell3:
		pshufd	xmm0, xmm1, 2
		mov		ecx, ebp
		call	GetGridCellSeenData
		test	eax, eax
		jz		done
		cmp		eax, 1
		jnz		case2Both
		add		bl, 2
		jmp		done
		ALIGN 16
	case2Both:
		mov		edi, eax
		mov		edx, esi
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [eax+edx*4+4]
		bt		eax, ecx
		setc	al
		add		bl, al
		lea		edx, [esi+0x10]
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [edi+edx*4+4]
		bt		eax, ecx
		setc	al
		add		bl, al
		jmp		done
		ALIGN 16
	case3Ext:
		pshufd	xmm0, xmm1, 1
		mov		ecx, ebp
		call	GetGridCellSeenData
		test	eax, eax
		jz		getCell4
		cmp		eax, 1
		jz		modRes3
		mov		edx, esi
		and		dl, 0xF
		mov		ecx, [eax+4]
		bt		ecx, edx
		setc	al
	modRes3:
		add		bl, al
	getCell4:
		pshufd	xmm0, xmm1, 2
		mov		ecx, ebp
		call	GetGridCellSeenData
		test	eax, eax
		jz		getCell5
		cmp		eax, 1
		jz		modRes4
		mov		edx, esi
		and		dl, 0xF0
		mov		ecx, edx
		shr		dl, 5
		mov		eax, [eax+edx*4+4]
		bt		eax, ecx
		setc	al
	modRes4:
		add		bl, al
	getCell5:
		pshufd	xmm0, xmm1, 3
		mov		ecx, ebp
		call	GetGridCellSeenData
		test	eax, eax
		jz		done
		cmp		eax, 1
		jz		modRes5
		mov		ecx, [eax+4]
		bt		ecx, 0
		setc	al
	modRes5:
		add		bl, al
	done:
		movzx	eax, bl
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
		ALIGN 16
	kSeenLvlConsts:
		EMIT_DW_1(00) EMIT_DW(00, 00, 00, 01) EMIT_DW(00, 01, 00, 00) EMIT_DW(00, 01, 00, 01)
		EMIT_DW(04, 06, 00, 02) EMIT_DW(0C, 0E, 08, 0A) DUP_2(EMIT_DW(80, 80, 80, 80))
	}
}

NiTriShapeData *s_localMapShapeDatas[9];
TESObjectCELL *s_pcCurrCell, *s_lastInterior;
Coordinate s_packedCellCoords[9];
float s_vertexAlphaLevel[] = {0, 0.25F, 0.5F, 0.75F, 1.0F};

__declspec(naked) void __fastcall PackCurrGridCoords(UInt32 currCoords)
{
	__asm
	{
		movd	xmm0, ecx
		shufps	xmm0, xmm0, 0
		movaps	xmm1, xmm0
		paddw	xmm0, kGridAdjustCoord
		paddw	xmm1, kGridAdjustCoord+0x10
		mov		eax, offset s_packedCellCoords
		movups	[eax], xmm0
		mov		[eax+0x10], ecx
		movups	[eax+0x14], xmm1
		retn
		ALIGN 16
	kGridAdjustCoord:
		EMIT_DW(FF, FF, FF, FF) EMIT_DW(FF, FF, 00, 00) EMIT_DW(FF, FF, 00, 01) EMIT_DW(00, 00, FF, FF)
		EMIT_DW(00, 00, 00, 01) EMIT_DW(00, 01, FF, FF) EMIT_DW(00, 01, 00, 00) EMIT_DW(00, 01, 00, 01)
	}
}

__declspec(naked) void __fastcall CalcVtxAlphaBySeenData(UInt32 gridIdx)
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebp, g_gridCellArray
		mov		ebx, ecx
		mov		ecx, s_pcCurrCell
		test	byte ptr [ecx+0x24], 1
		jz		isExterior
		push	kXData_ExtraSeenData
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		ebp, [eax+0xC]
		test	ebp, ebp
		jz		done
	isExterior:
		mov		ecx, s_localMapShapeDatas[ebx*4]
		push	ecx
		mov		esi, [ecx+0x28]
		add		esi, 0xC
		mov		edi, s_packedCellCoords[ebx*4]
		inc		di
		mov		bl, 0x10
		ALIGN 16
	iterHead:
		movd	xmm0, edi
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
		cmp		bh, bl
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
		and		dword ptr [ecx+0x20], 0
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
		pshufd	xmm1, xmm0, 5
		andps	xmm1, kTruncMask
		pcmpgtd	xmm1, kGridSlice
		movmskps	ecx, xmm1
		movzx	eax, word ptr kUpdateMask[ecx*2]
		retn
		ALIGN 16
	kTruncMask:
		EMIT_PS_4(00, 00, 0F, FF)
	kGridSlice:
		DUP_2(EMIT_DW_2(04, FF) EMIT_DW_2(0A, FF))
	kUpdateMask:
		EMIT_4W(00, 1B, 00, 12, 00, 12, 00, 36) EMIT_4W(00, 18, 00, 10, 00, 10, 00, 30)
		EMIT_4W(00, 30, 00, 30, 00, 30, 00, 30) EMIT_4W(00, D8, 00, 90, 00, 90, 01, B0)
	}
}

__declspec(naked) void __fastcall DoSelectiveFOWUpdate(__m128 adjustedPos)
{
	__asm
	{
		cvttps2dq	xmm0, xmm0
		call	GetFOWUpdateMask
		push	esi
		push	edi
		mov		esi, eax
		or		edi, 0xFFFFFFFF
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

__declspec(naked) bool __vectorcall UpdateSeenBits(SeenData *seenData, __m128i relPos)
{
	__asm
	{
		push	ebp
		push	ebx
		push	esi
		push	edi
		xor		eax, eax
		push	eax
		push	0x02030304
		movd	edx, xmm0
		movsx	ebp, dx
		sar		edx, 0x10
		push	edx
		push	eax
		lea		esi, [ecx+4]
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
		add		eax, ebp
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
		lea		edi, [esi+eax*4]
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
		movups	xmm0, [esi]
		movups	xmm1, [esi+0x10]
		pand	xmm0, xmm1
		pcmpeqd	xmm1, xmm1
		pcmpeqd	xmm0, xmm1
		movmskps	eax, xmm0
		cmp		al, 0xF
		setz	al
		lea		ecx, [esi-4]
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
		push	kXData_ExtraSeenData
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
		and		dword ptr [eax+8], 0
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
		movlps	[eax+0x24], xmm0
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
		MARK_MODIFIED(esi, 0x80000000)
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
		movlps	[eax+0x24], xmm0
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
		push	ebx
		push	esi
		push	edi
		mov		ebx, g_thePlayer
		mov		esi, [ebx+0x40]
		test	esi, esi
		jz		done
		movq	xmm4, qword ptr [ebx+0x30]
		mov		edi, 0x11CA208
		cmp		ds:0x11E0BB0, esi
		jz		sameCell
		mov		ecx, esi
		call	GetNorthRotation
		jmp		skipPosDiff
	sameCell:
		movq	xmm0, qword ptr [edi]
		subps	xmm0, xmm4
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		comiss	xmm0, kUpdSeenConsts+8
		jb		done
	skipPosDiff:
		movlps	[edi], xmm4
		mov		ebx, 0xFFFFFFFF
		test	byte ptr [esi+0x24], 1
		jnz		isInterior
		cvtps2dq	xmm0, xmm4
		call	GetFOWUpdateMask
		mov		edi, eax
		mov		eax, g_TES
		movq	xmm4, qword ptr [eax+0x2C]
		pshuflw	xmm2, xmm4, 2
		pslld	xmm4, 0xC
		psubd	xmm4, xmm0
		psrad	xmm4, 8
		pshuflw	xmm3, xmm4, 2
		ALIGN 16
	iterHeadExt:
		inc		ebx
		shr		edi, 1
		ja		iterHeadExt
		jnb		done
		movd	xmm4, kUpdSeenConsts[ebx*4+0x10]
		movq	xmm0, xmm4
		paddw	xmm0, xmm2
		mov		ecx, g_gridCellArray
		call	GridCellArray::GetCellAtCoord
		test	eax, eax
		jz		iterHeadExt
		test	byte ptr [eax+0x25], 1
		jnz		iterHeadExt
		mov		esi, eax
		mov		ecx, eax
		call	AddExtraSeenData
		movq	xmm0, xmm4
		psllw	xmm0, 4
		paddw	xmm0, xmm3
		mov		ecx, eax
		call	UpdateSeenBits
		test	al, al
		jz		iterHeadExt
		push	kXData_ExtraSeenData
		lea		ecx, [esi+0x28]
		CALL_EAX(ADDR_RemoveExtraType)
		or		byte ptr [esi+0x25], 1
		mov		eax, [esi+0xC]
		push	0x80000000
		push	eax
		push	2
		push	eax
		mov		eax, g_BGSSaveLoadGame
		mov		ecx, [eax]
		mov		esi, ecx
		CALL_EAX(0x8457B0)
		mov		ecx, esi
		CALL_EAX(0x845A80)
		jmp		iterHeadExt
		ALIGN 16
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn
		ALIGN 16
	isInterior:
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm4, xmm4
		mulps	xmm4, s_northRotationMods
		shufps	xmm4, xmm4, 0x27
		haddps	xmm4, xmm4
	noRotation:
		cvtps2dq	xmm0, xmm4
		psubd	xmm0, kUpdSeenConsts
		call	GetFOWUpdateMask
		mov		edi, eax
		movq	xmm4, xmm0
		psrad	xmm4, 0xC
		pshuflw	xmm2, xmm4, 2
		pslld	xmm4, 0xC
		psubd	xmm4, xmm0
		psrad	xmm4, 8
		pshuflw	xmm3, xmm4, 2
		movd	xmm4, kUpdSeenConsts+0xC
		mov		ecx, esi
		call	AddExtraSeenData
		mov		esi, eax
		ALIGN 16
	iterHeadInt:
		inc		ebx
		shr		edi, 1
		ja		iterHeadInt
		jnb		done
		movd	xmm5, kUpdSeenConsts[ebx*4+0x10]
		movq	xmm0, xmm5
		paddw	xmm0, xmm2
		pshufb	xmm0, xmm4
		movd	edx, xmm0
		mov		ecx, esi
		call	AddIntSeenData
		cmp		[eax+0x26], 0
		jnz		iterHeadInt
		movq	xmm0, xmm5
		psllw	xmm0, 4
		paddw	xmm0, xmm3
		mov		ecx, eax
		call	UpdateSeenBits
		or		[ecx+0x26], al
		jmp		iterHeadInt
		ALIGN 16
	kUpdSeenConsts:
		DUP_2(EMIT_DW(00, 00, 08, 00)) EMIT_DW(45, C8, 00, 00) EMIT_DW(80, 80, 00, 02)
		EMIT_DW(FF, FF, FF, FF) EMIT_DW(FF, FF, 00, 00) EMIT_DW(FF, FF, 00, 01)
		EMIT_DW(00, 00, FF, FF) EMIT_DW(00, 00, 00, 00) EMIT_DW(00, 00, 00, 01)
		EMIT_DW(00, 01, FF, FF) EMIT_DW(00, 01, 00, 00) EMIT_DW(00, 01, 00, 01)
	}
}

struct DoorRef
{
	TESObjectREFR	*doorRef;
	TESObjectCELL	*linkedCell;

	DoorRef(TESObjectREFR *_doorRef, TESObjectCELL *_linkedCell) : doorRef(_doorRef), linkedCell((_linkedCell && !_linkedCell->worldSpace) ? _linkedCell : nullptr) {}
};

typedef Vector<DoorRef> DoorRefsList;

void __fastcall GetTeleportDoors(TESObjectCELL *cell, DoorRefsList *doorRefsList)
{
	TESObjectREFR *refr;
	for (auto iter = g_loadedReferences->teleportDoors.Begin(); iter; ++iter)
	{
		if (!(refr = iter.Get()) || (refr->flags & 0x860) || (refr->parentCell != cell))
			continue;
		ExtraTeleport *xTeleport = GetExtraType(&refr->extraDataList, ExtraTeleport);
		if (xTeleport->data->linkedDoor)
			doorRefsList->Append(refr, xTeleport->data->linkedDoor->parentCell);
	}
}

__declspec(naked) UInt32* __vectorcall GetVtxAlphaPtr(__m128 posMult)
{
	__asm
	{
		push	ebx
		mulps	xmm0, kGridPosMult
		cvttps2dq	xmm0, xmm0
		pextrw	eax, xmm0, 0
		pextrw	ecx, xmm0, 2
		mov		ebx, eax
		shr		ebx, 4
		lea		edx, [ebx+ebx*2]
		mov		ebx, ecx
		shr		ebx, 4
		neg		ebx
		lea		edx, [edx+ebx+2]
		and		eax, 0xF
		lea		ebx, [eax+eax]
		lea		eax, [eax+ebx*8]
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

__declspec(naked) void __fastcall GenerateRenderedTexture(TESObjectCELL *cell, const TextureParams &texParams, NiCamera *camera, NiTexture **pTexture)
{
	static BSCullingProcess *s_cullingProcess = nullptr;
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		test	ecx, ecx
		setz	[ebp-1]
		jz		noCell
		test	byte ptr [ecx+0x24], 1
	noCell:
		setnz	[ebp-2]
		mov		ecx, ds:0x11F9508
		lea		eax, [ecx+0x5E0]
		push	eax
		xor		eax, eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	eax
		push	dword ptr [edx+8]
		push	eax
		push	dword ptr [edx+4]
		push	dword ptr [edx]
		push	ecx
		mov		ecx, ds:0x11F91A8
		CALL_EAX(0xB6D5E0)
		test	eax, eax
		jz		done
		push	eax
		push	g_shadowSceneNode
		mov		eax, s_cullingProcess
		test	eax, eax
		jnz		hasCulling
		push	0x350
		CALL_EAX(0xAA13E0)
		pop		ecx
		push	0x2F7
		push	1
		push	0x63
		mov		ecx, eax
		CALL_EAX(0xB660D0)
		mov		dword ptr [eax+4], 2
		mov		ecx, [ebp-0x10]
		mov		[eax+0x194], ecx
		push	eax
		push	0
		lea		ecx, [eax+0x280]
		CALL_EAX(0x4A0EB0)
		mov		dword ptr [eax+0x90], 3
		pop		dword ptr [eax+0xC4]
		mov		s_cullingProcess, eax
	hasCulling:
		push	eax
		mov		ecx, [eax+0xC4]
		push	ecx
		mov		dl, [ebp-1]
		mov		[ecx+0x16C], dl
		mov		ecx, [ebp-8]
		push	dword ptr [ecx]
		xor		eax, eax
		mov		[ecx], eax
		mov		ecx, [ebp-0x10]
		mov		[ecx+0x130], 1
		mov		byte ptr ds:0x11AD7B4, 0
		mov		edx, [ebp-0xC]
		mov		edx, [edx+8]
		mov		ecx, ds:0x11F4748
		cmp		dword ptr [ecx+0x200], 0
		setnz	al
		mov		[ebp-3], al
		mov		ecx, 7
		cmovnz	edx, ecx
		cmovnz	ecx, [ebp-0xC]
		push	edx
		push	ecx
		call	kScnCallbacks[eax*4]
		add		esp, 8
		mov		dl, [ebp-2]
		neg		dl
		and		edx, 0xC
		mov		eax, [ebp-0x18]
		mov		[eax+0x19C], edx
		push	dword ptr [ebp-0x14]
		push	dword ptr [ebp-0x10]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		mov		eax, [ebp-0x18]
		mov		[esp+4], eax
		CALL_EAX(0xB6C0D0)
		add		esp, 0xC
		movzx	eax, byte ptr [ebp-3]
		call	kScnCallbacks[eax*4+8]
		mov		byte ptr ds:0x11AD7B4, 1
		mov		ecx, [ebp-0x10]
		mov		[ecx+0x130], 0
		mov		ecx, [ebp-8]
		pop		dword ptr [ecx]
		mov		ecx, [ebp-0xC]
		push	dword ptr [ecx+0x30]
		push	dword ptr [ebp+0xC]
		call	NiReleaseAddRef
		mov		ecx, [ebp-0xC]
		call	NiReleaseObject
	done:
		leave
		retn	8
		ALIGN 4
	kScnCallbacks:
		EMIT_DW_3(B6, B8, D0) EMIT_DW_3(54, ED, E0)
		EMIT_DW_3(B6, B7, 90) EMIT_DW_3(B6, B8, 40)
	}
}

NiCamera *s_localMapCamera;

__declspec(naked) void __fastcall GenerateLocalMapExterior(TESObjectCELL *cell, NiRenderedTexture **renderedTexture)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x10
		push	ecx
		push	edx
		xorps	xmm0, xmm0
		movups	[ebp-0x10], xmm0
		mov		edx, 0xA0
		mov		eax, [ecx+0xC4]
		mov		ecx, [eax]
		mov		eax, [ecx+edx]
		mov		ecx, [eax]
		push	ecx
		or		byte ptr [ecx+0x30], 1
		mov		ecx, [eax+8]
		mov		eax, [ecx+edx]
		mov		dl, 4
		ALIGN 16
	iterHead:
		dec		edx
		js		iterEnd
		mov		ecx, [eax+edx*4]
		mov		ecx, [ecx+0xA0]
		mov		ecx, [ecx]
		mov		ecx, [ecx+0xA8]
		test	byte ptr [ecx+0x25], 0x40
		jnz		iterHead
		cmp		dword ptr [ecx+0x38], 0
		jz		iterHead
		mov		[ebp+edx*4-0x10], ecx
		or		byte ptr [ecx+0x25], 0x40
		and		dword ptr [ecx+0x38], 0
		jmp		iterHead
		ALIGN 16
	iterEnd:
		mov		eax, ds:0x11D5C48
		push	eax
		mov		[eax+0x1B], 1
		mov		ecx, [ebp-0x14]
		mov		eax, [ecx+0x48]
		movq	xmm0, qword ptr [eax]
		pslld	xmm0, 0xC
		paddd	xmm0, kExtPosMod
		cvtdq2ps	xmm0, xmm0
		mov		eax, s_localMapCamera
		movlps	[eax+0x8C], xmm0
		push	dword ptr [ebp-0x18]
		push	eax
		mov		edx, offset s_mmTextureParams
		call	GenerateRenderedTexture
		pop		eax
		mov		[eax+0x1B], 0
		pop		eax
		and		byte ptr [eax+0x30], 0xFE
		mov		edx, 4
		ALIGN 16
	undoIter:
		dec		edx
		js		done
		mov		ecx, [ebp+edx*4-0x10]
		test	ecx, ecx
		jz		undoIter
		and		byte ptr [ecx+0x25], 0xBF
		and		dword ptr [ecx+0x38], 0
		jmp		undoIter
	done:
		pop		ecx
		mov		eax, [ecx]
		pop		ecx
		mov		ecx, [ecx+0xC]
		mov		[eax+0x34], ecx
		leave
		retn
		ALIGN 16
	kExtPosMod:
		EMIT_PS_2(00, 00, 08, 00)
	}
}

__declspec(naked) void __vectorcall GenerateLocalMapInterior(TESObjectCELL *cell, __m128i coord, NiRenderedTexture **renderedTexture)
{
	__asm
	{
		push	edx
		mov		eax, s_localMapCamera
		push	eax
		pshuflw	xmm0, xmm0, 0x26
		psrad	xmm0, 0x10
		paddd	xmm0, kIntPosMod
		pslld	xmm0, 0xC
		cvtdq2ps	xmm0, xmm0
		cmp		s_cellNorthRotation, 0
		jz		noRot
		shufps	xmm0, xmm0, 0x11
		mulps	xmm0, s_northRotationMods
		haddps	xmm0, xmm0
	noRot:
		movlps	[eax+0x8C], xmm0
		mov		edx, offset s_mmTextureParams
		call	GenerateRenderedTexture
		retn
		ALIGN 16
	kIntPosMod:
		EMIT_PS_2(00, 00, 00, 01)
	}
}

__declspec(naked) void AttachToCellNodeHook()
{
	__asm
	{
		push	1
		push	eax
		push	0x54972C
		mov		ecx, [ebp+0xC]
		mov		eax, [eax+8]
		cmp		eax, s_BlackPlane01
		jnz		notBkPln
		mov		eax, [ecx+0x18]
		mov		ecx, [eax+0xA0]
		mov		eax, [ecx+0x10]
		mov		[ebp+0xC], eax
		mov		ecx, [ecx+0x20]
	notBkPln:
		JMP_EAX(0xA5ED10)
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

struct MapMarkerTile;
TempObject<Vector<MapMarkerTile>> s_mapMarkerTiles(0x40);

struct MapMarkerTile
{
	TileValue		*visible;
	TileValue		*x;
	TileValue		*y;
	TileValue		*filename;
	bool			inUse;

	MapMarkerTile(Tile *markerTile)
	{
		visible = markerTile->GetValue(kTileValue_visible);
		x = markerTile->GetValue(kTileValue_user0);
		y = markerTile->GetValue(kTileValue_user1);
		filename = markerTile->AddValue(kTileValue_filename);
		markerTile->SetFloat(kTileValue_depth, int((s_mapMarkerTiles->Size() & 0xF) + 18));
		inUse = true;
	}
};

typedef Vector<MapMarkerTile*, 4> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles, 0x40, false> RenderedMapMarkers;
TempObject<RenderedMapMarkers> s_renderedMapMarkers;

void __fastcall FreeCellMapMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
	{
		dtlIter->inUse = 0;
		dtlIter->visible->SetFloat(0);
	}
	cmkIter.Remove();
}

bool s_discoveredLocation = false;

void __fastcall HandleDiscoverLocation(TESObjectCELL *markerCell)
{
	if (!markerCell) return;
	if (auto findRendered = s_renderedMapMarkers->Find(Coordinate(*markerCell->exteriorCoords)))
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
		jmp		HandleDiscoverLocation
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

UInt8 s_miniMapIndex = 0;
TileRect *s_localMapRect, *s_worldMapRect, *s_mapMarkersRect, *s_doorMarkersRect, *s_lQuestMarkersRect, *s_wQuestMarkersRect;
TileImage *s_worldMapTile;
TileValue *s_miniMapMode = nullptr, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom;
TileShaderProperty *s_tileShaderProps[9];
float s_fLocalMapZoomCurr;
BSFadeNode *s_fakeWaterPlanes;
bool s_isInInterior = false;
NiNode *s_shadowSceneNodes[5];
NiColor *g_directionalLightColor, *g_shadowFogColor;

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

const NiPoint2 kWaterPlanePos[] =
{
	{2048.0F, -10240.0F}, {2048.0F, -6144.0F}, {2048.0F, -2048.0F},
	{6144.0F, -10240.0F}, {6144.0F, -6144.0F}, {6144.0F, -2048.0F},
	{10240.0F, -10240.0F}, {10240.0F, -6144.0F}, {10240.0F, -2048.0F}
};

bool Cmd_InitMiniMap_Execute(COMMAND_ARGS)
{
	if (s_miniMapMode || (scriptObj->modIndex != s_miniMapIndex))
		return true;

	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPMiniMap");
	if (!tile) return true;

	UInt8 *codePtr = scriptData + 0x2FE5;
	if (*(UInt32*)codePtr == 0x173031)
	{
		*codePtr = 0x33;
		scriptObj->quest->questDelayTime = 1 / 60.0F;
	}

	//s_miniMapVisible = tile->GetValue(kTileValue_visible);
	s_miniMapScale = tile->GetValue(kTileValue_user1);
	auto node = tile->children.Tail()->prev;
	s_pcMarkerRotate = node->data->GetValue(kTileValue_rotateangle);
	node->data->SetFloat(kTileValue_depth, 34.0F);
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

	NiColorAlpha *vertexColors = (NiColorAlpha*)GameHeapAlloc(sizeof(NiColorAlpha) * 0xA29);
	ZERO_BYTES(vertexColors, sizeof(NiColorAlpha) * 0xA29);

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
		shapeData->vertexColors = vertexColors;
		vertexColors += 0x121;

		GameHeapFree(shapeData->uvCoords);
		shapeData->uvCoords = shapeUVCoords;

		GameHeapFree(shapeData->triangles);
		shapeData->triangles = shapeTriangles;

		NiReleaseAddRef(&sciTriShp->m_propertyList.Head()->data, alphaProp);
		NiReleaseAddRef(&sciTriShp->alphaProp, alphaProp);
	}
	while (++index < 9);

	s_doorMarkersRect = (TileRect*)node->data;
	s_lQuestMarkersRect = (TileRect*)node->prev->data;
	node = s_worldMapRect->children.Tail();
	s_worldMapTile = (TileImage*)node->data;
	s_mapMarkersRect = (TileRect*)node->prev->data;
	s_wQuestMarkersRect = (TileRect*)node->prev->prev->data;

	s_fLocalMapZoomCurr = s_localMapZoom->num;
	if (g_TES->currentInterior)
	{
		s_isInInterior = true;
		s_fLocalMapZoomCurr += kInteriorZoomMod;
	}
	UpdateTileScales(s_fLocalMapZoomCurr);

	NiCamera *lmCamera = NiCamera::Create();
	lmCamera->m_uiRefCount = 2;
	lmCamera->LocalRotate()[0] = 0;
	lmCamera->LocalRotate()[2] = 1.0F;
	lmCamera->LocalRotate()[6] = -1.0F;;
	lmCamera->LocalRotate()[8] = 0;
	lmCamera->LocalTranslate().z = 65536.0F;
	lmCamera->m_transformWorld = lmCamera->m_transformLocal;
	lmCamera->frustum.viewPort = {-2048.0F, 2048.0F, 2048.0F, -2048.0F};
	lmCamera->frustum.n = 1.0F;
	lmCamera->frustum.f = 131072.0F;
	lmCamera->frustum.o = 1;
	lmCamera->LODAdjust = 0.001F;
	g_shadowSceneNode->AddObject(lmCamera, 1);
	s_localMapCamera = lmCamera;

	BSFadeNode *waterParent = BSFadeNode::Create();
	waterParent->m_uiRefCount = 2;

	NiVector4 *waterVertices = (NiVector4*)GameHeapAlloc(sizeof(NiVector3) << 2);
	waterVertices[0] = __m128{-2048.0F, 2048.0F, 0, -2048.0F};
	waterVertices[1] = __m128{-2048.0F, 0, 2048.0F, 2048.0F};
	waterVertices[2] = __m128{0, 2048.0F, -2048.0F, 0};

	vertexColors = (NiColorAlpha*)GameHeapAlloc(sizeof(NiColorAlpha) << 2);
	__m128 kWaterPlaneColor = {23 / 255.0F, 51/ 255.0F, 47 / 255.0F, 0.75F};
	vertexColors[0] = kWaterPlaneColor;
	vertexColors[1] = kWaterPlaneColor;
	vertexColors[2] = kWaterPlaneColor;
	vertexColors[3] = kWaterPlaneColor;

	shapeTriangles = (NiTriangle*)GameHeapAlloc(sizeof(NiTriangle) << 1);
	shapeTriangles[0] = {0, 1, 2};
	shapeTriangles[1] = {2, 1, 3};

	NiTriShapeData *shapeData = ThisCall<NiTriShapeData*>(0xA7B630, NiAllocator(sizeof(NiTriShapeData)), 4, waterVertices, nullptr, vertexColors, nullptr, 0, 0, 2, shapeTriangles);

	alphaProp = NiAlphaProperty::Create();
	alphaProp->flags = 0x10ED;

	index = 0;
	do
	{
		NiTriShape *waterPlane = ThisCall<NiTriShape*>(0xA74480, NiAllocator(sizeof(NiTriShape)), shapeData);
		waterPlane->AddProperty(alphaProp);
		waterPlane->AddProperty(BSShaderNoLightingProperty::Create());
		waterPlane->Hide();
		waterPlane->LocalTranslate() = kWaterPlanePos[index];
		waterParent->AddObject(waterPlane, 1);
	}
	while (++index < 9);

	waterParent->RenderGeometryAndShader();
	GameGlobals::ObjectLODRoot()->AddObject(waterParent, 1);
	s_fakeWaterPlanes = waterParent;

	auto worldIter = g_dataHandler->worldSpaceList.Head();
	TESWorldSpace *worldSpc, *rootWorld = nullptr;
	TESObjectCELL *rootCell;
	WorldDimensions worldDimensions;
	WorldMapMarkers *worldMarkers;
	TESObjectREFR *markerRef;
	MapMarkerData *markerData;
	NiPoint2 posXY;
	do
	{
		if (!(worldSpc = worldIter->data) || (!worldSpc->parent && !worldSpc->mapData.usableDimensions.X) || !(rootCell = worldSpc->cell) || rootCell->objectList.Empty())
			continue;
		worldDimensions.GetPosMods(worldSpc);
		worldSpc = worldSpc->GetRootMapWorld();
		if (rootWorld != worldSpc)
		{
			rootWorld = worldSpc;
			worldDimensions.GetDimensions(rootWorld);
			worldMarkers = &s_worldMapMarkers()[rootWorld];
		}
		auto refrIter = rootCell->objectList.Head();
		do
		{
			if (!(markerRef = refrIter->data) || !(markerData = markerRef->GetMapMarkerData()))
				continue;
			__asm
			{
				lea		ecx, worldDimensions
				lea		edx, [ecx+0x24]
				mov		eax, markerRef
				movq	xmm0, qword ptr [eax+0x30]
				call	GetWorldMapPosMults
				movlps	posXY, xmm0
			}
			(*worldMarkers)[worldDimensions.coord].Append(markerRef, markerData, posXY);
		}
		while (refrIter = refrIter->next);
	}
	while (worldIter = worldIter->next);

	s_shadowSceneNodes[0] = *(NiNode**)0x11DEB34;	// Sky
	s_shadowSceneNodes[1] = *(NiNode**)0x11DEDA4;	// Weather
	s_shadowSceneNodes[2] = *(NiNode**)0x11D86A8;	// TerrainLOD
	s_shadowSceneNodes[3] = *(NiNode**)0x11DED58;	// ParticleSysMngr
	s_shadowSceneNodes[4] = *(NiNode**)0x11CA438;	// Grass
	g_directionalLightColor = &g_TES->directionalLight->ambientColor;
	g_shadowFogColor = &g_shadowSceneNode->fogProperty->color;
	SafeWrite16(0x452736, 0x7705);
	SafeWrite8(0x555C20, 0xC3);
	WriteRelCall(0x9438F6, (UInt32)UpdateCellsSeenBitsHook);
	WritePushRetRelJump(0x779567, 0x7795E8, (UInt32)DiscoverLocationHook);
	WritePushRetRelJump(0x60F13A, 0x60F145, (UInt32)SetQuestTargetsHook);
	WriteRelCall(0x952C69, (UInt32)UpdatePlacedMarkerHook);
	WriteRelCall(0x952F6B, (UInt32)UpdatePlacedMarkerHook);
	return true;
}

const __m128 kLocalMapPosMults = {1 / 12288.0F, 1 / -12288.0F, 0, 0};
const __m128 kVertexAlphaMults = {0.25F, 0.5F, 0.75F, 1.0F};
alignas(16) const NiColor kDirectionalLightValues[] = {{1.0F, 1.0F, 239 / 255.0F}, {0, 0, 0}};
alignas(16) const float kFogPropertyValues[] = {23 / 255.0F, 51 / 255.0F, 47 / 255.0F, FLT_MAX, FLT_MAX};
alignas(16) const int kNWCoordAdjust[] = {-1, 2, 0, 0};

struct RenderedEntry
{
	NiRenderedTexture	*texture;
	UInt8				regenFlags;

	RenderedEntry() : texture(nullptr), regenFlags(0) {}
	~RenderedEntry() {if (texture) ThisCall(0xA7FD30, texture, true);}
};

struct DoorMarkerTile;
TempObject<Vector<DoorMarkerTile>> s_doorMarkerTiles(0x40);

struct DoorMarkerTile
{
	TileValue		*visible;
	TileValue		*x;
	TileValue		*y;
	TileValue		*visited;
	UInt32			*vtxAlpha;
	bool			inUse;

	DoorMarkerTile(Tile *markerTile)
	{
		visible = markerTile->GetValue(kTileValue_visible);
		x = markerTile->GetValue(kTileValue_user0);
		y = markerTile->GetValue(kTileValue_user1);
		visited = markerTile->AddValue(kTileValue_user2);
		markerTile->SetFloat(kTileValue_depth, int((s_doorMarkerTiles->Size() & 0xF) + 18));
	}
};

struct CulledObject
{
	NiAVObject		*object;

	CulledObject(NiAVObject *_obj) : object(_obj) {object->Hide();}
	~CulledObject() {object->Show();}
};

struct QuestMarkerTile
{
	UInt32			pos;
	TileValue		*x;
	TileValue		*y;
};

UInt32 s_currentMapMode = 0;
TESWorldSpace *s_pcCurrWorld, *s_pcRootWorld;
Coordinate s_currWorldCoords(0x7FFF, 0x7FFF), s_currLocalCoords(0x7FFF, 0x7FFF), s_cellGridCenter;
WorldMapMarkers *s_currWorldMarkers;
TempObject<Set<UInt32>> s_currMarkerCells(0x40);
typedef UnorderedMap<UInt32, RenderedEntry, 0x40, false> LocalTexturesMap;
TempObject<LocalTexturesMap> s_renderedExteriors, s_renderedInterior;
TempObject<UnorderedMap<UInt32, DoorRefsList, 0x20, false>> s_exteriorDoorRefs;
TempObject<DoorRefsList> s_doorRefsList(0x40);
TempObject<Vector<CulledObject>> s_hiddenObjects(0x40);
TESQuest *s_activeQuest = nullptr;
TESObjectREFR *s_placedMarker = nullptr;
TempObject<Map<TESObjectREFR*, QuestMarkerTile>> s_questMarkers(0x10);
TESObjectCELL *s_currCellGrid[9];
bool s_useFogOfWar = false;

#define LIGHTING_PASSES *(UInt8*)0x11F91D8
#define TEXTURE_LIST_BGN *(NiTexture**)0x11F4500
#define TEXTURE_LIST_END *(NiTexture**)0x11F4504
#define TEXTURE_LIST_CS (CriticalSection*)0x11F4580

NiRenderedTexture *s_mmFirstTexture = nullptr;

__declspec(naked) NiRenderedTexture* __fastcall FindNextMMTexture(NiTexture *texture)
{
	__asm
	{
		xor		eax, eax
		mov		edx, kVtbl_NiRenderedTexture
		ALIGN 16
	iterHead:
		mov		ecx, [ecx+0x2C]
		test	ecx, ecx
		jz		done
		cmp		[ecx], edx
		jnz		iterHead
		cmp		[ecx+0x34], eax
		jz		iterHead
		mov		eax, ecx
	done:
		retn
	}
}

void MiniMapLoadGame()
{
	s_pcCurrCell = nullptr;
	s_pcCurrWorld = nullptr;
	s_pcRootWorld = nullptr;
	s_lastInterior = nullptr;
}

template <typename T> __forceinline T PickFrom(T (&&values)[], size_t index)
{
	return values[index];
}

bool Cmd_UpdateMiniMap_Execute(COMMAND_ARGS)
{
	if (!s_miniMapMode || (scriptObj->modIndex != s_miniMapIndex))
		return true;

	UInt32 updateType, showFlags = 0x100;
	ExtractArgsEx(EXTRACT_ARGS_EX, &updateType, &showFlags);

	if (updateType == 3)
	{
		__asm
		{
			mov		eax, s_fLocalMapZoomCurr
			mov		ecx, s_localMapZoom
			mov		ecx, [ecx+8]
			cmp		eax, ecx
			cmovnz	eax, ecx
			movd	xmm0, eax
			jz		sameZoom
			cmp		s_isInInterior, 0
			jz		notInterior
			addss	xmm0, kInteriorZoomMod
		notInterior:
			movss	s_fLocalMapZoomCurr, xmm0
		sameZoom:
			call	UpdateTileScales
		}
		if (showFlags == 0x100)
			return true;
	}

	PlayerCharacter *thePlayer = g_thePlayer;
	TESObjectCELL *parentCell = thePlayer->parentCell;
	if (!parentCell) return true;
	TESWorldSpace *parentWorld = parentCell->worldSpace;

	bool updateTiles;
	__asm
	{
		mov		ecx, parentCell
		cmp		s_pcCurrCell, ecx
		setnz	updateTiles
		jz		noChange
		mov		s_pcCurrCell, ecx
		call	GetNorthRotation
		mov		ecx, s_localMapCamera
		movlps	[ecx+0x38], xmm0
		movhps	[ecx+0x44], xmm0
		movlps	[ecx+0x6C], xmm0
		movhps	[ecx+0x78], xmm0
		cmp		parentWorld, 0
		setz	al
		cmp		s_isInInterior, al
		jz		noChange
		mov		s_isInInterior, al
		movzx	eax, al
		movss	xmm0, kInteriorZoomMod
		movss	xmm1, PS_FlipSignMask0[eax*4]
		xorps	xmm0, xmm1
		addss	xmm0, s_fLocalMapZoomCurr
		movss	s_fLocalMapZoomCurr, xmm0
		call	UpdateTileScales
	noChange:
	}

	bool updQuestTargets = s_updateQuestTargets;
	s_updateQuestTargets = false;

	if (s_currentMapMode != updateType)
	{
		if (updateType <= 1)
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
		for (auto shaderProp : s_tileShaderProps)
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
		else if (!(parentWorld = g_TES->currentWrldspc) || !(objectRef = thePlayer->lastExteriorDoor))
			return true;

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
				s_currWorldMarkers = s_worldMapMarkers->GetPtr(rootWorld);
				s_rootWorldDimensions.mkRange = iceil(72.0F / rootWorld->mapData.usableDimensions.X *
					(rootWorld->mapData.cellSECoordinates.X - rootWorld->mapData.cellNWCoordinates.X +
					rootWorld->mapData.cellNWCoordinates.Y - rootWorld->mapData.cellSECoordinates.Y));
				if (showFlags & 1)
				{
					for (auto tileIter = s_mapMarkerTiles->Begin(); tileIter; ++tileIter)
						if (tileIter().inUse)
						{
							tileIter().inUse = 0;
							tileIter().visible->SetFloat(0);
						}
					s_renderedMapMarkers->Clear();
				}
			}
			updateTiles = true;
		}

		posMult = GetWorldMapPosMults(objectRef->position.PS2(), s_rootWorldDimensions, &coord);
		updateTiles |= s_currWorldCoords != coord;
		s_currWorldCoords = coord;

		if (showFlags & 1)
		{
			if (updateTiles || s_discoveredLocation)
			{
				s_discoveredLocation = false;
				s_currMarkerCells->Clear();
				if (s_currWorldMarkers && !s_currWorldMarkers->Empty())
				{
					int range = iceil(s_rootWorldDimensions.mkRange / s_worldMapZoom->num);
					coord.x -= range;
					coord.y -= range;
					range = (range * 2) + 1;
					auto tileIter = s_mapMarkerTiles->Begin();
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
							s_currMarkerCells->Insert(coord);
							if (!s_renderedMapMarkers->Insert(coord, &dynamicTiles))
								goto iterYnext;
							for (auto mkIter = mapMarkers->Begin(); mkIter; ++mkIter)
							{
								if (!(mkIter().data->flags & 1) || (mkIter().refr->flags & 0x800))
									continue;
								while (true)
								{
									if (!tileIter)
									{
										tileData = s_mapMarkerTiles->Append(hudMain->AddTileFromTemplate(s_mapMarkersRect, "MiniMapWorldMarkerTemplate"));
										break;
									}
									tileData = &tileIter();
									++tileIter;
									if (tileData->inUse) continue;
									tileData->inUse = 1;
									tileData->visible->SetFloat(1.0F);
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
				for (auto cmkIter = s_renderedMapMarkers->Begin(); cmkIter; ++cmkIter)
					if (!s_currMarkerCells->HasKey(cmkIter.Key()))
						FreeCellMapMarkers(cmkIter);
			}
		}
		else if (!s_renderedMapMarkers->Empty())
		{
			s_mapMarkerTiles->Clear();
			s_renderedMapMarkers->Clear();
			s_mapMarkersRect->DestroyAllChildren();
		}
	}
	else
	{
		bool updateFogOfWar = useFogOfWar && s_updateFogOfWar/*, saveToFile = (showFlags & 0x20) != 0*/;
		s_updateFogOfWar = false;

		D3DFORMAT d3dFormat = (showFlags & 8) ? TEXTURE_FMT_RGB : TEXTURE_FMT_BW;
		if (s_mmTextureParams.d3dFormat != d3dFormat)
		{
			s_mmTextureParams.d3dFormat = d3dFormat;
			s_renderedExteriors->Clear();
			s_mmFirstTexture = nullptr;
			s_lastInterior = nullptr;
			updateTiles = true;
		}

		TESObjectCELL *cell;
		UInt32 gridIdx;
		NiNode *hideNode;
		RenderedEntry *textureEntry;
		UInt8 lightingPasses = LIGHTING_PASSES;

		if (parentWorld)
		{
			Coordinate gridCenter;
			__asm
			{
				mov		eax, g_TES
				movq	xmm0, qword ptr [eax+0x2C]
				pshuflw	xmm1, xmm0, 2
				movd	coord, xmm1
				paddd	xmm0, kNWCoordAdjust
				pslld	xmm0, 0xC
				cvtdq2ps	xmm1, xmm0
				movlps	nwXY, xmm1
				mov		edx, thePlayer
				movq	xmm0, qword ptr [edx+0x30]
				subps	xmm0, xmm1
				mulps	xmm0, kLocalMapPosMults
				movlps	posMult, xmm0
				movq	xmm0, qword ptr [eax+0x24]
				pshuflw	xmm1, xmm0, 2
				movd	gridCenter, xmm1
			}

			if (updateTiles |= (s_currLocalCoords != coord))
			{
				s_currLocalCoords = coord;
				PackCurrGridCoords(coord);
				gridIdx = 0;
				do
				{
					s_currCellGrid[gridIdx] = g_gridCellArray->GetCellAtCoord(s_packedCellCoords[gridIdx]);
					if (useFogOfWar)
						CalcVtxAlphaBySeenData(gridIdx);
				}
				while (++gridIdx < 9);
				if (showFlags & 2)
				{
					for (auto drlIter = s_exteriorDoorRefs->Begin(); drlIter; ++drlIter)
					{
						bool inGrid;
						for (auto pCell : s_currCellGrid)
							if (inGrid = pCell && (drlIter.Key() == pCell->refID))
								break;
						if (!inGrid) drlIter.Remove();
					}
					s_doorRefsList->Clear();
					DoorRefsList *listPtr;
					for (auto pCell : s_currCellGrid)
					{
						if (!pCell) continue;
						if (s_exteriorDoorRefs->Insert(pCell->refID, &listPtr))
							GetTeleportDoors(pCell, listPtr);
						s_doorRefsList->Concatenate(*listPtr);
					}
				}
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(thePlayer->position.PS2());

			if (updateTiles || (s_cellGridCenter != gridCenter))
			{
				s_cellGridCenter = gridCenter;
				memcpy(g_directionalLightColor, kDirectionalLightValues, sizeof(kDirectionalLightValues));
				memcpy(g_shadowFogColor, kFogPropertyValues, sizeof(kFogPropertyValues));
				*(bool*)0x11FF104 = 1;
				s_shadowSceneNodes[0]->Hide();
				s_shadowSceneNodes[1]->Hide();
				s_shadowSceneNodes[2]->Hide();
				s_shadowSceneNodes[3]->Hide();
				s_shadowSceneNodes[4]->Hide();
				bool updateWater = false;
				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]))
						continue;
					s_hiddenObjects->Append(cell->Get3DNode(4));
					for (auto anmIter = cell->renderData->animatedRefs.Begin(); anmIter; ++anmIter)
						if ((hideNode = anmIter.Get()) && ((hideNode->GetBSXFlags() & 0x23) == 0x21))
							s_hiddenObjects->Append(hideNode);
					if (!(cell->cellFlags & 2))
						continue;
					objectRef = nullptr;
					auto awtIter = cell->renderData->placeableWaterList.Head();
					do
					{
						if (!awtIter->data || !awtIter->data->IsCreated() || !(hideNode = awtIter->data->GetRefNiNode()))
							continue;
						s_hiddenObjects->Append(hideNode);
						objectRef = awtIter->data;
					}
					while (awtIter = awtIter->next);
					if (!objectRef) continue;
					NiAVObject *pWaterPlane = s_fakeWaterPlanes->m_children[gridIdx];
					pWaterPlane->Show();
					if (updateWater = updateTiles)
						pWaterPlane->LocalTranslate().z = objectRef->position.z;
				}
				while (++gridIdx < 9);
				if (!updateTiles)
					s_hiddenObjects->Append(*(NiNode**)0x11D8690);
				else if (updateWater)
				{
					s_fakeWaterPlanes->LocalTranslate() = nwXY;
					ThisCall(0xA5DD70, s_fakeWaterPlanes, &kUpdateParams, 0);
				}
				NiLight *light;
				GameGlobals::SceneLightsLock()->Enter();
				for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
					if (lgtNode->data->isPointLight && (light = lgtNode->data->light) && !(light->m_flags & 1))
						s_hiddenObjects->Append(light);
				
				LIGHTING_PASSES = 0x3C;
				const UInt8 kSelectUpdate[] = {0x8, 0xA, 0x2, 0xC, 0xF, 0x3, 0x4, 0x5, 0x1};
				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]))
						continue;
					const UInt8 updateMask = kSelectUpdate[gridIdx];
					if (s_renderedExteriors->Insert(cell->refID, &textureEntry))
					{
						GenerateLocalMapExterior(cell, &textureEntry->texture);
						if (s_currLocalCoords == gridCenter)
							textureEntry->regenFlags = updateMask;
						if (!s_mmFirstTexture)
							s_mmFirstTexture = textureEntry->texture;
						/*if (saveToFile)
							SaveLocalMapTexture(parentWorld, textureEntry->texture, s_packedCellCoords[gridIdx]);*/
					}
					else if (updateTiles)
					{
						NiRenderedTexture *texture = textureEntry->texture;
						if (texture != TEXTURE_LIST_END)
						{
							ScopedCS cs(TEXTURE_LIST_CS);
							if (texture == s_mmFirstTexture)
								s_mmFirstTexture = FindNextMMTexture(texture);
							NiTexture *prev = texture->prev, *next = texture->next;
							if (prev) prev->next = next;
							else TEXTURE_LIST_BGN = next;
							if (next) next->prev = prev;
							prev = TEXTURE_LIST_END;
							prev->next = texture;
							texture->prev = prev;
							texture->next = nullptr;
							TEXTURE_LIST_END = texture;
						}
					}
					else if ((textureEntry->regenFlags & updateMask) != updateMask)
					{
						textureEntry->regenFlags |= updateMask;
						GenerateLocalMapExterior(cell, &textureEntry->texture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentWorld, textureEntry->texture, s_packedCellCoords[gridIdx]);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = textureEntry->texture;
				}
				while (++gridIdx < 9);

				s_hiddenObjects->Clear();
				GameGlobals::SceneLightsLock()->Leave();
				*(bool*)0x11FF104 = 0;
				s_shadowSceneNodes[0]->Show();
				s_shadowSceneNodes[1]->Show();
				s_shadowSceneNodes[2]->Show();
				s_shadowSceneNodes[3]->Show();
				s_shadowSceneNodes[4]->Show();

				for (auto plnIter = s_fakeWaterPlanes->m_children.Begin(); plnIter; ++plnIter)
					plnIter->Hide();

				if (s_renderedExteriors->Size() > CACHED_TEXTURES_MAX)
				{
					UInt32 purgeCount = s_renderedExteriors->Size() - CACHED_TEXTURES_MIN;
					NiRenderedTexture *curr = s_mmFirstTexture;
					ScopedCS cs(TEXTURE_LIST_CS);
					do
					{
						NiRenderedTexture *next = FindNextMMTexture(curr);
						purgeCount -= s_renderedExteriors->Erase(curr->unk34);
						curr = next;
					}
					while (purgeCount && curr);
					s_mmFirstTexture = curr;
				}
			}
		}
		else
		{
			__asm
			{
				mov		ecx, thePlayer
				movq	xmm0, qword ptr [ecx+0x30]
				call	AdjustInteriorPos
				movlps	adjustedPos, xmm0
				cvttps2dq	xmm1, xmm0
				psrad	xmm1, 0xC
				pshuflw	xmm2, xmm1, 2
				movd	coord, xmm2
				paddd	xmm1, kNWCoordAdjust
				pslld	xmm1, 0xC
				cvtdq2ps	xmm1, xmm1
				movlps	nwXY, xmm1
				subps	xmm0, xmm1
				mulps	xmm0, kLocalMapPosMults
				movlps	posMult, xmm0
			}

			if (s_lastInterior != parentCell)
			{
				s_lastInterior = parentCell;
				s_renderedInterior->Clear();
				updateTiles = true;
			}

			if ((showFlags & 2) && updateTiles)
			{
				s_doorRefsList->Clear();
				GetTeleportDoors(parentCell, *s_doorRefsList);
			}

			if (updateTiles |= (s_currLocalCoords != coord))
			{
				s_currLocalCoords = coord;
				PackCurrGridCoords(coord);
				NiNode **cellNodes = parentCell->Get3DNodes();
				cellNodes[0]->Hide();
				cellNodes[4]->Hide();
				cellNodes[8]->Hide();
				s_shadowSceneNodes[3]->Hide();
				LIGHTING_PASSES = 0x34;

				gridIdx = 0;
				do
				{
					coord = s_packedCellCoords[gridIdx];
					if (s_renderedInterior->Insert(coord, &textureEntry))
					{
						GenerateLocalMapInterior(parentCell, coord, &textureEntry->texture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentCell, textureEntry->texture, coord);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = textureEntry->texture;
					if (useFogOfWar)
						CalcVtxAlphaBySeenData(gridIdx);
				}
				while (++gridIdx < 9);

				cellNodes[0]->Show();
				cellNodes[4]->Show();
				cellNodes[8]->Show();
				s_shadowSceneNodes[3]->Show();
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(adjustedPos.PS());
		}
		LIGHTING_PASSES = lightingPasses;

		if (showFlags & 2)
		{
			if (updateTiles)
			{
				auto markerIter = s_doorMarkerTiles->Begin();
				DoorMarkerTile *tileData;
				for (auto doorIter = s_doorRefsList->Begin(); doorIter; ++doorIter)
				{
					objectRef = doorIter().doorRef;
					bool offBounds;
					__asm
					{
						mov		ecx, objectRef
						movq	xmm0, qword ptr [ecx+0x30]
						cmp		parentWorld, 0
						jnz		getMults
						call	AdjustInteriorPos
					getMults:
						movq	xmm1, qword ptr nwXY
						subps	xmm0, xmm1
						mulps	xmm0, kLocalMapPosMults
						movlps	adjustedPos, xmm0
						unpcklpd	xmm0, xmm0
						movq	xmm1, qword ptr PS_V3_One
						cmpnltps	xmm0, xmm1
						movmskps	eax, xmm0
						xor		al, 0xC
						setnz	offBounds
					}
					if (offBounds) continue;
					if (markerIter)
					{
						tileData = &markerIter();
						++markerIter;
					}
					else tileData = s_doorMarkerTiles->Append(hudMain->AddTileFromTemplate(s_doorMarkersRect, "MiniMapDoorMarkerTemplate"));
					tileData->x->SetFloat(adjustedPos.x);
					tileData->y->SetFloat(adjustedPos.y);
					cell = doorIter().linkedCell;
					tileData->visited->SetFloat(!cell || (cell == parentCell) || cell->extraDataList.HasType(kXData_ExtraDetachTime));
					tileData->inUse = true;
					if (useFogOfWar)
						tileData->vtxAlpha = GetVtxAlphaPtr(adjustedPos.PS());
					tileData->visible->SetFloat(!useFogOfWar || *tileData->vtxAlpha);
				}

				for (; markerIter; ++markerIter)
				{
					if (!markerIter().inUse) continue;
					markerIter().inUse = false;
					markerIter().visible->SetFloat(0);
				}
			}
			else if (updateFogOfWar)
				for (auto markerIter = s_doorMarkerTiles->Begin(); markerIter; ++markerIter)
					if (markerIter().inUse)
						markerIter().visible->SetFloat(*markerIter().vtxAlpha != 0);
		}
		else if (!s_doorMarkerTiles->Empty())
		{
			s_doorMarkerTiles->Clear();
			s_doorMarkersRect->DestroyAllChildren();
		}
	}

	if (showFlags & 4)
	{
		QuestMarkerTile *markerData;
		if (updQuestTargets || (s_activeQuest != thePlayer->activeQuest) || (s_placedMarker != thePlayer->placedMarkerRef))
		{
			s_activeQuest = thePlayer->activeQuest;
			s_placedMarker = thePlayer->placedMarkerRef;
			if (!s_questMarkers->Empty())
			{
				s_questMarkers->Clear();
				s_lQuestMarkersRect->DestroyAllChildren();
				s_wQuestMarkersRect->DestroyAllChildren();
			}
			if (!worldMap || parentCell->worldSpace)
			{
				auto trgIter = thePlayer->questTargetList.Head();
				ObjectiveTarget *targetData;
				TileRect *targetsRect = worldMap ? s_wQuestMarkersRect : s_lQuestMarkersRect;
				int depth = 35;
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
					if (s_questMarkers->Insert(objectRef, &markerData))
					{
						markerTile = hudMain->AddTileFromTemplate(targetsRect, "MiniMapQuestMarkerTemplate");
						markerData->pos = 0x7F7FFFFF;
						markerData->x = markerTile->GetValue(kTileValue_user0);
						markerData->y = markerTile->GetValue(kTileValue_user1);
						markerTile->SetFloat(kTileValue_depth, depth++);
					}
					if (donePlaced)
					{
						markerTile = markerData->x->parent;
						markerTile->SetFloat(kTileValue_red, 191.0F);
						markerTile->SetFloat(kTileValue_green, 127.0F);
						markerTile->SetFloat(kTileValue_blue, 255.0F);
						break;
					}
				}
			}
		}

		for (auto mkrIter = s_questMarkers->Begin(); mkrIter; ++mkrIter)
		{
			objectRef = mkrIter.Key();
			markerData = &mkrIter();
			UInt32 currX = ULNG(objectRef->position);
			if (markerData->pos != currX)
				markerData->pos = currX;
			else if (!updateTiles) continue;
			if (worldMap)
				adjustedPos = GetWorldMapPosMults(objectRef->position.PS2(), s_rootWorldDimensions);
			else if (parentWorld)
				adjustedPos = _mm_mul_ps(_mm_sub_ps(objectRef->position.PS2(), nwXY.PS()), kLocalMapPosMults);
			else
				adjustedPos = _mm_mul_ps(_mm_sub_ps(AdjustInteriorPos(objectRef->position.PS2()), nwXY.PS()), kLocalMapPosMults);
			markerData->x->SetFloat(adjustedPos.x);
			markerData->y->SetFloat(adjustedPos.y);
		}
	}
	else if (!s_questMarkers->Empty())
	{
		s_questMarkers->Clear();
		s_lQuestMarkersRect->DestroyAllChildren();
		s_wQuestMarkersRect->DestroyAllChildren();
	}

	s_pcMarkerRotate->SetFloat(s_cellNorthRotation - thePlayer->rotation.z);
	s_miniMapPosX->SetFloat(posMult.x);
	s_miniMapPosY->SetFloat(posMult.y);
	return true;
}