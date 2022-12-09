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

Tile::Value /**s_miniMapVisible, */*s_miniMapScale, *s_localMapZoom;
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
		push	1
		push	ecx
		movss	[esp], xmm0
		mov		ecx, s_localMapZoom
		CALL_EAX(ADDR_TileValSetFloat)
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
		mov		eax, offset kNoRotation
		retn
		ALIGN 16
	kNoRotation:
		EMIT_DW_1(00) DUP_2(EMIT_DW(3F, 80, 00, 00)) EMIT_DW_1(00)
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
		movlps	[esi], xmm0
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
		movlps	[ecx+0x1C], xmm0
	done:
		retn
	}
}

__declspec(naked) __m128 __vectorcall GetWorldMapPosMults(const NiVector3 &inPos, const WorldDimensions &worldDimensions, Coordinate *outCoord = nullptr)
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
		movq	xmm1, xmm0
		movq	xmm2, qword ptr [edx]
		subps	xmm0, xmm2
		movq	xmm2, qword ptr [edx+8]
		mulps	xmm0, xmm2
		addps	xmm0, kPosMultMods
		mov		eax, [esp+4]
		test	eax, eax
		jz		done
		cvtps2dq	xmm1, xmm1
		psrad	xmm1, 0xC
		pshuflw	xmm1, xmm1, 2
		movd	[eax], xmm1
	done:
		retn	4
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
		push	kExtraData_SeenData
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
		xor		ebx, ebx
		and		edx, 0xF0F
		movzx	eax, dl
		shl		al, 4
		or		al, dh
		mov		esi, eax
		cmp		dh, 0xF
		setz	cl
		shl		cl, 1
		cmp		dl, 0xF
		setz	bh
		or		bh, cl
		pshufd	xmm1, xmm0, 0
		paddw	xmm1, kSeenLvlConsts
		mov		ecx, ebp
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
		push	kExtraData_SeenData
		add		ecx, 0x28
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
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
		DUP_2(EMIT_DW_2(03, FF) EMIT_DW_2(0B, FF))
	kUpdateMask:
		EMIT_DW(00, 12, 00, 1B) EMIT_DW(00, 36, 00, 12)
		EMIT_DW(00, 10, 00, 18) EMIT_DW(00, 30, 00, 10)
		EMIT_DW(00, 30, 00, 30) EMIT_DW(00, 30, 00, 30)
		EMIT_DW(00, 90, 00, D8) EMIT_DW(01, B0, 00, 90)
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
		movd	ebp, xmm0
		movsx	edx, bp
		sar		ebp, 0x10
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
		push	ebp
		push	ebx
		push	esi
		push	edi
		mov		ebx, g_thePlayer
		mov		esi, [ebx+0x40]
		test	esi, esi
		jz		doneExt
		movq	xmm7, qword ptr [ebx+0x30]
		mov		edi, 0x11CA208
		cmp		s_pcCurrCell0, esi
		jz		sameCell
		mov		s_pcCurrCell0, esi
		mov		ecx, esi
		call	GetNorthRotation
		jmp		skipPosDiff
	sameCell:
		movq	xmm0, qword ptr [edi]
		subps	xmm0, xmm7
		mulps	xmm0, xmm0
		haddps	xmm0, xmm0
		comiss	xmm0, kUpdSeenConsts+0x24
		jb		doneExt
	skipPosDiff:
		movq	xmm0, xmm7
		movlps	[edi], xmm0
		movq	xmm4, qword ptr kUpdSeenConsts
		movaps	xmm5, kUpdSeenConsts+0x10
		pshufd	xmm6, xmm5, 0xE
		test	byte ptr [esi+0x24], 1
		jnz		isInterior
		mov		ebp, g_gridCellArray
		cvtps2dq	xmm0, xmm0
		call	GetFOWUpdateMask
		mov		edi, eax
		mov		eax, [esi+0x48]
		movq	xmm3, qword ptr [eax]
		psubd	xmm3, xmm4
		pshuflw	xmm2, xmm3, 2
		pslld	xmm3, 0xC
		psubd	xmm3, xmm0
		psrad	xmm3, 8
		pshuflw	xmm3, xmm3, 8
		unpcklps	xmm2, xmm3
		movq	xmm3, xmm2
		xor		ebx, ebx
		ALIGN 16
	iterHeadExt:
		movzx	ecx, bl
		lea		eax, [ecx+ecx*2]
		add		al, bh
		bt		edi, eax
		jnc		iterNextExt
		movq	xmm0, xmm3
		mov		ecx, ebp
		call	GridCellArray::GetCellAtCoord
		test	eax, eax
		jz		iterNextExt
		test	byte ptr [eax+0x25], 1
		jnz		iterNextExt
		mov		esi, eax
		mov		ecx, eax
		call	AddExtraSeenData
		pshufd	xmm0, xmm3, 1
		mov		ecx, eax
		call	UpdateSeenBits
		test	al, al
		jz		iterNextExt
		push	kExtraData_SeenData
		lea		ecx, [esi+0x28]
		CALL_EAX(ADDR_RemoveExtraType)
		or		byte ptr [esi+0x25], 1
		push	0x80000000
		push	esi
		mov		ecx, g_BGSSaveLoadGame
		CALL_EAX(0x84A780)
		MARK_MODIFIED(esi, 2)
	iterNextExt:
		paddw	xmm3, xmm5
		inc		bh
		cmp		bh, 2
		jbe		iterHeadExt
		xor		bh, bh
		paddw	xmm2, xmm6
		movq	xmm3, xmm2
		inc		bl
		cmp		bl, 2
		jbe		iterHeadExt
	doneExt:
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
		ALIGN 16
	isInterior:
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm0, xmm0
		mulps	xmm0, s_northRotationMods
		pshufd	xmm1, xmm0, 0x27
		xorps	xmm0, xmm0
		haddps	xmm1, xmm0
		movq	xmm0, xmm1
	noRotation:
		cvtps2dq	xmm0, xmm0
		movq	xmm1, qword ptr kUpdSeenConsts+8
		psubd	xmm0, xmm1
		call	GetFOWUpdateMask
		mov		edi, eax
		movq	xmm3, xmm0
		psrad	xmm3, 0xC
		psubd	xmm3, xmm4
		movq	xmm2, xmm3
		pshufb	xmm2, kUpdSeenConsts+0x20
		movd	ebp, xmm2
		pslld	xmm3, 0xC
		psubd	xmm3, xmm0
		psrad	xmm3, 8
		pshuflw	xmm3, xmm3, 8
		unpcklps	xmm2, xmm3
		movq	xmm3, xmm2
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
		mov		edx, ebp
		add		dh, bh
		add		dl, bl
		mov		ecx, esi
		call	AddIntSeenData
		cmp		byte ptr [eax+0x26], 0
		jnz		iterNextInt
		pshufd	xmm0, xmm3, 1
		mov		ecx, eax
		call	UpdateSeenBits
		mov		[ecx+0x26], al
	iterNextInt:
		paddw	xmm3, xmm5
		inc		bh
		cmp		bh, 2
		jbe		iterHeadInt
		xor		bh, bh
		paddw	xmm2, xmm6
		movq	xmm3, xmm2
		inc		bl
		cmp		bl, 2
		jbe		iterHeadInt
		pop		edi
		pop		esi
		pop		ebx
		pop		ebp
		retn
		ALIGN 16
	kUpdSeenConsts:
		DUP_2(EMIT_DW(00, 00, 00, 01)) DUP_2(EMIT_DW(00, 00, 08, 00))
		EMIT_DW(00, 00, 00, 01) EMIT_DW(00, 10, 00, 00) EMIT_DW(00, 01, 00, 00) EMIT_DW(00, 00, 00, 10)
		EMIT_DW(80, 80, 04, 00) EMIT_DW(45, C8, 00, 00)
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
	if (!cell) return;
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

__declspec(naked) float* __vectorcall GetVtxAlphaPtr(__m128 posMult)
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

__declspec(naked) void __fastcall GenerateRenderedTexture(TESObjectCELL *cell, const TextureParams &texParams, NiCamera *camera, NiTexture **pTexture)
{
	static BSCullingProcess *s_cullingProcess = nullptr;
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x1C
		test	ecx, ecx
		setz	[ebp-0x19]
		jz		noCell
		test	byte ptr [ecx+0x24], 1
	noCell:
		setnz	[ebp-0x1A]
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
		mov		eax, ds:0x11F9508
		push	eax
		add		eax, 0x5E0
		mov		[ebp-4], eax
		mov		ecx, ds:0x11F91A8
		CALL_EAX(0xB6D5E0)
		test	eax, eax
		jz		done
		mov		[ebp-8], eax
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
		mov		ecx, g_shadowSceneNode
		mov		[eax+0x194], ecx
		push	eax
		push	0
		lea		ecx, [eax+0x280]
		CALL_EAX(0x4A0EB0)
		pop		dword ptr [eax+0xC4]
		mov		s_cullingProcess, eax
	hasCulling:
		mov		[ebp-0x14], eax
		mov		ecx, [eax+0xC4]
		mov		[ebp-0x18], ecx
		mov		dl, [ebp-0x19]
		mov		[ecx+0x16C], dl
		mov		eax, [ecx+0x194]
		mov		[ebp-0x10], eax
		mov		dl, [eax+0x130]
		mov		[ebp-0x1B], dl
		mov		[eax+0x130], 1
		xor		eax, eax
		mov		ecx, [ebp-4]
		mov		edx, [ecx]
		mov		[ebp-0xC], edx
		mov		[ecx], eax
		mov		byte ptr ds:0x11AD7B4, 0
		mov		edx, [ebp-8]
		mov		edx, [edx+8]
		mov		ecx, ds:0x11F4748
		cmp		dword ptr [ecx+0x200], 0
		setnz	al
		mov		[ebp-0x1C], al
		mov		ecx, 7
		cmovnz	edx, ecx
		cmovnz	ecx, [ebp-8]
		push	edx
		push	ecx
		call	kScnCallbacks[eax*4]
		add		esp, 8
		mov		dl, [ebp-0x1A]
		neg		dl
		and		edx, 0xC
		mov		eax, [ebp-0x18]
		mov		[eax+0x19C], edx
		mov		eax, [ebp-0x14]
		mov		dword ptr [eax+0x90], 3
		push	eax
		push	dword ptr [ebp-0x10]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
		mov		ecx, g_TES
		mov		eax, [ecx+0x34]
		test	eax, eax
		jz		skipPortals
		mov		ecx, [ebp-0x14]
		mov		dword ptr [ecx+0x90], 1
		push	ecx
		mov		ecx, [eax+0xC4]
		mov		eax, [ecx]
		mov		ecx, [eax+0xA0]
		push	dword ptr [ecx+0x18]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
	skipPortals:
		push	dword ptr [ebp-0x18]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6C0D0)
		add		esp, 8
		movzx	eax, byte ptr [ebp-0x1C]
		call	kScnCallbacks[eax*4+8]
		mov		byte ptr ds:0x11AD7B4, 1
		mov		al, [ebp-0x1B]
		mov		ecx, [ebp-0x10]
		mov		[ecx+0x130], al
		mov		ecx, [ebp-4]
		mov		eax, [ebp-0xC]
		mov		[ecx], eax
		mov		ecx, [ebp-8]
		mov		eax, [ecx+0x30]
		test	eax, eax
		jz		doRelease
		push	eax
		push	dword ptr [ebp+0xC]
		call	NiReleaseAddRef
		mov		ecx, [ebp-8]
	doRelease:
		call	NiReleaseObject
	done:
		leave
		retn	8
		ALIGN 16
	kScnCallbacks:
		EMIT_DW_3(B6, B8, D0) EMIT_DW_3(54, ED, E0)
		EMIT_DW_3(B6, B7, 90) EMIT_DW_3(B6, B8, 40)
	}
}

NiCamera *s_localMapCamera = nullptr;

__declspec(naked) void __stdcall GenerateLocalMapExterior(TESObjectCELL *cell, NiRenderedTexture **renderedTexture)
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
		dec		edx
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
		push	dword ptr [ebp+0xC]
		mov		ecx, s_localMapCamera
		movlps	[ecx+0x58], xmm0
		push	ecx
		push	0
		push	offset kUpdateParams
		CALL_EAX(0xA59F90)
		mov		edx, offset s_mmTextureParams
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
		dec		edx
		js		done
		mov		ecx, [ebp+edx*4-0x18]
		test	ecx, ecx
		jz		undoIter
		and		byte ptr [ecx+0x25], 0xBF
		mov		[ecx+0x38], eax
		jmp		undoIter
	done:
		leave
		retn	8
		ALIGN 16
	kExtPosMod:
		EMIT_PS_2(00, 00, 08, 00)
	}
}

__declspec(naked) void __stdcall GenerateLocalMapInterior(TESObjectCELL *cell, Coordinate coord, NiRenderedTexture **renderedTexture)
{
	__asm
	{
		mov		ecx, [esp+4]
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		mov		ecx, [eax]
		test	ecx, ecx
		jz		done
		movss	xmm0, [esp+8]
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
		mov		eax, [ecx+0xA0]
		mov		ecx, [eax]
		push	ecx
		or		byte ptr [ecx+0x30], 1
		mov		ecx, [eax+4]
		push	ecx
		or		byte ptr [ecx+0x30], 1
		push	dword ptr [esp+0x14]
		mov		ecx, s_localMapCamera
		movlps	[ecx+0x58], xmm0
		push	ecx
		push	0
		push	offset kUpdateParams
		CALL_EAX(0xA59F90)
		mov		edx, offset s_mmTextureParams
		mov		ecx, [esp+0x14]
		call	GenerateRenderedTexture
		pop		ecx
		and		byte ptr [ecx+0x30], 0xFE
		pop		ecx
		and		byte ptr [ecx+0x30], 0xFE
	done:
		retn	0xC
		ALIGN 16
	kIntPosMod:
		EMIT_PS_2(00, 00, 00, 01)
	}
}

TempObject<NiFixedString> s_AutoWaterFadeNode;

__declspec(naked) void __fastcall InitAutoWaterHook(BSFadeNode *node, int EDX, bool doSet)
{
	__asm
	{
		or		byte ptr [ecx+0x31], 0x80
		mov		eax, s_AutoWaterFadeNode
		lock inc dword ptr [eax-8]
		mov		[ecx+8], eax
		retn	4
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
	markerTile->SetFloat(kTileValue_depth, int((s_mapMarkerTiles->Size() % 10) + 18));
	inUse = true;
}

typedef Vector<MapMarkerTile*, 4> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles, 0x40, false> RenderedMapMarkers;
TempObject<RenderedMapMarkers> s_renderedMapMarkers;

void __fastcall FreeCellMapMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
		dtlIter->SetInUse(false);
	cmkIter.Remove();
}

bool s_discoveredLocation = false;

void __fastcall HandleDiscoverLocation(TESObjectCELL *markerCell)
{
	auto findRendered = s_renderedMapMarkers->Find(Coordinate(&markerCell->exteriorCoords->x));
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

UInt8 s_miniMapIndex = 0;
bool s_initMiniMap = false;
TileRect *s_localMapRect, *s_worldMapRect, *s_mapMarkersRect, *s_doorMarkersRect, *s_lQuestMarkersRect, *s_wQuestMarkersRect;
TileImage *s_worldMapTile;
Tile::Value *s_miniMapMode, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom;
TileShaderProperty *s_tileShaderProps[9];
float s_fLocalMapZoomCurr;
BSFadeNode *s_fakeWaterPlanes;
bool s_isInInterior = false, s_defaultGridSize;
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

const NiPoint2 kWaterPlanePos[] =
{
	{2048.0F, -10240.0F}, {2048.0F, -6144.0F}, {2048.0F, -2048.0F},
	{6144.0F, -10240.0F}, {6144.0F, -6144.0F}, {6144.0F, -2048.0F},
	{10240.0F, -10240.0F}, {10240.0F, -6144.0F}, {10240.0F, -2048.0F}
};

bool Cmd_InitMiniMap_Execute(COMMAND_ARGS)
{
	if (s_initMiniMap || (scriptObj->modIndex != s_miniMapIndex))
		return true;

	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPMiniMap");
	if (!tile) return true;

	UInt8 *codePtr = scriptData + 0x2FE5;
	if (*(UInt32*)codePtr == 0x173031)
	{
		*codePtr = 0x33;
		scriptObj->quest->questDelayTime = 1 / 60.0F;
	}

	s_initMiniMap = true;
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
		ZERO_BYTES(shapeData->vertexColors, sizeof(NiColorAlpha) * 0x121);

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

	NiVector4 *waterVertices = (NiVector4*)GameHeapAlloc(sizeof(NiVector3) << 2);
	waterVertices[0] = __m128{-2048.0F, 2048.0F, 0, -2048.0F};
	waterVertices[1] = __m128{-2048.0F, 0, 2048.0F, 2048.0F};
	waterVertices[2] = __m128{0, 2048.0F, -2048.0F, 0};

	NiColorAlpha *vertexColors = (NiColorAlpha*)GameHeapAlloc(sizeof(NiColorAlpha) << 2);
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
			posXY = GetWorldMapPosMults(markerRef->position, s_rootWorldDimensions, &coord);
			if (!worldMarkers)
				worldMarkers = &s_worldMapMarkers()[rootWorld];
			(*worldMarkers)[coord].Append(markerRef, markerData, posXY);
		}
		while (refrIter = refrIter->next);
	}
	while (worldIter = worldIter->next);

	s_defaultGridSize = *(UInt8*)0x11C63D0 <= 5;
	g_directionalLightColor = &g_TES->directionalLight->ambientColor;
	g_shadowFogColor = &(*(BSFogProperty**)0x11DEB00)->color;
	g_particleSysMngr = BSParticleSystemManager::GetSingleton();
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

struct RenderedEntry
{
	NiRenderedTexture	*texture;
	UInt8				regenFlags;

	RenderedEntry() : texture(nullptr), regenFlags(0) {}
	~RenderedEntry() {if (texture) ThisCall(0xA7FD30, texture, true);}
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
	markerTile->SetFloat(kTileValue_depth, int((s_doorMarkerTiles->Size() % 10) + 18));
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
typedef UnorderedMap<UInt32, RenderedEntry, 0x40, false> LocalTexturesMap;
TempObject<LocalTexturesMap> s_renderedExteriors, s_renderedInterior;
TempObject<Vector<UInt32>> s_exteriorKeys(0x48);
TempObject<UnorderedMap<UInt32, DoorRefsList, 0x20, false>> s_exteriorDoorRefs;
TempObject<DoorRefsList> s_doorRefsList(0x40);
TempObject<Vector<NiNode*>> s_hiddenNodes(0x40);
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
	if (!s_initMiniMap || (scriptObj->modIndex != s_miniMapIndex) || !ExtractArgsEx(EXTRACT_ARGS_EX, &updateType, &showFlags))
		return true;

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
		movaps	xmm0, [eax]
		movlps	[ecx+0x38], xmm0
		movhps	[ecx+0x44], xmm0
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
				s_currWorldMarkers = s_worldMapMarkers->GetPtr(rootWorld);
				s_rootWorldDimensions.mkRange = iceil(72.0F / rootWorld->mapData.usableDimensions.X *
					(rootWorld->mapData.cellSECoordinates.X - rootWorld->mapData.cellNWCoordinates.X +
					rootWorld->mapData.cellNWCoordinates.Y - rootWorld->mapData.cellSECoordinates.Y));
				if (showFlags & 1)
				{
					for (auto tileIter = s_mapMarkerTiles->Begin(); tileIter; ++tileIter)
						if (tileIter().inUse) tileIter.Ref().SetInUse(false);
					s_renderedMapMarkers->Clear();
				}
			}
			updateTiles = true;
		}

		posMult = GetWorldMapPosMults(objectRef->position, s_rootWorldDimensions, &coord);

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
				s_currMarkerCells->Clear();
				if (s_currWorldMarkers)
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
									tileData = &tileIter.Ref();
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
				for (auto cmkIter = s_renderedMapMarkers->Begin(); cmkIter; ++cmkIter)
					if (!s_currMarkerCells->HasKey(cmkIter.Key())) FreeCellMapMarkers(cmkIter);
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
		bool restore = false, showDoors = (showFlags & 2) != 0, updateFogOfWar = useFogOfWar && s_updateFogOfWar/*, saveToFile = (showFlags & 0x20) != 0*/;
		s_updateFogOfWar = false;

		D3DFORMAT d3dFormat = (showFlags & 8) ? TEXTURE_FMT_RGB : TEXTURE_FMT_BW;
		if (s_mmTextureParams.d3dFormat != d3dFormat)
		{
			s_mmTextureParams.d3dFormat = d3dFormat;
			s_renderedExteriors->Clear();
			s_exteriorKeys->Clear();
			s_lastInterior = nullptr;
			updateTiles = true;
		}

		TESObjectCELL *cell;
		UInt32 gridIdx, lightingPasses;
		NiPointLight *pntLight;
		RenderedEntry *textureEntry;

		if (parentWorld)
		{
			UInt8 quadrant;
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
				mov		ecx, thePlayer
				movq	xmm0, qword ptr [ecx+0x30]
				subps	xmm0, xmm1
				mulps	xmm0, kLocalMapPosMults
				movlps	posMult, xmm0
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
				s_currCellsSet->Clear();
				gridIdx = 0;
				do
				{
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					cell = g_gridCellArray->GetCellAtCoord(coord);
					s_currCellGrid[gridIdx] = cell;
					if (!cell) continue;
					s_currCellsSet->Insert(cell->refID);
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
					for (auto clsIter = s_currCellsSet->Begin(); clsIter; ++clsIter)
						if (s_exteriorDoorRefs->Insert(*clsIter, &listPtr))
							GetTeleportDoors((TESObjectCELL*)LookupFormByRefID(*clsIter), listPtr);
					s_doorRefsList->Clear();
					for (auto drlIter = s_exteriorDoorRefs->Begin(); drlIter; ++drlIter)
						if (s_currCellsSet->HasKey(drlIter.Key()))
							s_doorRefsList->Concatenate(drlIter());
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
					auto lgtData = lgtNode->data;
					if ((lgtData->state != 0xFF) && lgtData->isPointLight && (pntLight = (NiPointLight*)lgtData->light))
					{
						pntLight->radius0E4 = pntLight->radius;
						pntLight->radius = 0;
					}
				}
				GameGlobals::SceneLightsLock()->Leave();
				memcpy(g_directionalLightColor, kDirectionalLightValues, sizeof(kDirectionalLightValues));
				memcpy(g_shadowFogColor, kFogPropertyValues, sizeof(kFogPropertyValues));
				*(UInt8*)0x11FF104 = 1;
				g_particleSysMngr->Hide();

				s_hiddenNodes->Clear();
				NiNode *hideNode;
				BSXFlags *bsxFlags;
				NiAVObject **pWaterPlanes = s_fakeWaterPlanes->m_children.data;
				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]))
						continue;
					hideNode = cell->Get3DNode(4);
					hideNode->Hide();
					s_hiddenNodes->Append(hideNode);
					for (auto chlIter = cell->Get3DNode(3)->m_children.Begin(); chlIter; ++chlIter)
					{
						if ((hideNode = (NiNode*)*chlIter) && ((hideNode->m_blockName == s_AutoWaterFadeNode) ||
							((bsxFlags = hideNode->GetBSXFlags()) && ((bsxFlags->flags & 0x23) == 0x21))))
						{
							hideNode->Hide();
							s_hiddenNodes->Append(hideNode);
						}
					}
					if (!(cell->cellFlags & 2))
						continue;
					pWaterPlanes[gridIdx]->Show();
					if (updateTiles)
						pWaterPlanes[gridIdx]->LocalTranslate().z = (cell->waterHeight == FLT_MAX) ? parentWorld->defaultWaterHeight : cell->waterHeight;
				}
				while (++gridIdx < 9);
				if (updateTiles)
				{
					s_fakeWaterPlanes->LocalTranslate() = nwXY;
					ThisCall(0xA5DD70, s_fakeWaterPlanes, &kUpdateParams, 0);
				}

				const UInt8 *updateList = kSelectImgUpdate[quadrant];
				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]) || !(updateTiles || (quadrant = updateList[gridIdx])))
						continue;

					if (s_renderedExteriors->Insert(cell->refID, &textureEntry))
					{
						GenerateLocalMapExterior(cell, &textureEntry->texture);
						s_exteriorKeys->Append(cell->refID);
						/*if (saveToFile)
							SaveLocalMapTexture(parentWorld, textureEntry->texture, s_currLocalCoords + kGridAdjustCoord[gridIdx]);*/
					}
					else if (updateTiles)
						s_exteriorKeys->MoveToEnd(cell->refID);
					else if (!(textureEntry->regenFlags & quadrant))
					{
						if (gridIdx == 4) quadrant = 0xF;
						textureEntry->regenFlags |= quadrant;
						GenerateLocalMapExterior(cell, &textureEntry->texture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentWorld, textureEntry->texture, s_currLocalCoords + kGridAdjustCoord[gridIdx]);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = textureEntry->texture;
				}
				while (++gridIdx < 9);

				if (s_exteriorKeys->Size() > CACHED_TEXTURES_MAX)
				{
					UInt32 purgeCount = s_exteriorKeys->Size() - CACHED_TEXTURES_MIN;
					gridIdx = purgeCount;
					do {
						s_renderedExteriors->Erase(s_exteriorKeys()[--purgeCount]);
					} while (purgeCount);
					s_exteriorKeys->RemoveRange(0, gridIdx);
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

			if (showDoors && updateTiles)
			{
				s_doorRefsList->Clear();
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
				parentCell->Get3DNode(4)->Hide();
				g_particleSysMngr->Hide();
				lightingPasses = *GameGlobals::LightingPasses();
				*GameGlobals::LightingPasses() = 0x34;

				gridIdx = 0;
				do
				{
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					if (s_renderedInterior->Insert(coord, &textureEntry))
					{
						GenerateLocalMapInterior(parentCell, coord, &textureEntry->texture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentCell, textureEntry->texture, coord);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = textureEntry->texture;
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
				for (auto hdnIter = s_hiddenNodes->Begin(); hdnIter; ++hdnIter)
					hdnIter->Show();

				GameGlobals::SceneLightsLock()->Enter();
				for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
				{
					auto lgtData = lgtNode->data;
					if ((lgtData->state != 0xFF) && lgtData->isPointLight && (pntLight = (NiPointLight*)lgtData->light))
						pntLight->radius = pntLight->radius0E4;
				}
				GameGlobals::SceneLightsLock()->Leave();

				for (auto plnIter = s_fakeWaterPlanes->m_children.Begin(); plnIter; ++plnIter)
					plnIter->Hide();
			}
			else
			{
				parentCell->Get3DNode(4)->Show();
				*GameGlobals::LightingPasses() = lightingPasses;
			}
			g_particleSysMngr->Show();
		}

		if (showDoors)
		{
			if (updateTiles)
			{
				UInt8 offBounds;
				auto markerIter = s_doorMarkerTiles->Begin();
				DoorMarkerTile *tileData;
				for (auto doorIter = s_doorRefsList->Begin(); doorIter; ++doorIter)
				{
					objectRef = doorIter().doorRef;
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
						mov		offBounds, al
					}
					if (offBounds) continue;
					if (markerIter)
					{
						tileData = &markerIter.Ref();
						++markerIter;
					}
					else tileData = s_doorMarkerTiles->Append(hudMain->AddTileFromTemplate(s_doorMarkersRect, "MiniMapDoorMarkerTemplate"));
					tileData->x->SetFloat(adjustedPos.x);
					tileData->y->SetFloat(adjustedPos.y);
					cell = doorIter().linkedCell;
					tileData->visited->SetFloat(!cell || (cell == parentCell) || cell->extraDataList.HasType(kExtraData_DetachTime));
					if (useFogOfWar)
						tileData->vtxAlpha = GetVtxAlphaPtr(adjustedPos.PS());
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
					markerIter.Ref().inUse = false;
					markerIter().visible->SetFloat(0);
				}
			}

			if (updateTiles || updateFogOfWar)
			{
				for (auto markerIter = s_doorMarkerTiles->Begin(); markerIter; ++markerIter)
					if (markerIter().inUse && markerIter().vtxAlpha)
						markerIter().visible->SetFloat(*markerIter().vtxAlpha > 0);
			}
		}
		else if (!s_doorMarkerTiles->Empty())
		{
			s_doorMarkerTiles->Clear();
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
						Tile *mkTile = markerData->x->parent;
						mkTile->SetFloat(kTileValue_red, 191.0F);
						mkTile->SetFloat(kTileValue_green, 127.0F);
						mkTile->SetFloat(kTileValue_blue, 255.0F);
						break;
					}
				}
			}
		}

		if (!s_questMarkers->Empty())
		{
			for (auto mkrIter = s_questMarkers->Begin(); mkrIter; ++mkrIter)
			{
				objectRef = mkrIter.Key();
				markerData = &mkrIter();
				UInt32 currX = *(UInt32*)&objectRef->position;
				if (markerData->pos != currX)
					markerData->pos = currX;
				else if (!updateTiles) continue;
				if (worldMap)
					adjustedPos = GetWorldMapPosMults(objectRef->position, s_rootWorldDimensions);
				else if (parentWorld)
					adjustedPos = _mm_mul_ps(_mm_sub_ps(objectRef->position.PS2(), nwXY.PS()), kLocalMapPosMults);
				else
					adjustedPos = _mm_mul_ps(_mm_sub_ps(AdjustInteriorPos(objectRef->position.PS2()), nwXY.PS()), kLocalMapPosMults);
				markerData->x->SetFloat(adjustedPos.x);
				markerData->y->SetFloat(adjustedPos.y);
			}
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