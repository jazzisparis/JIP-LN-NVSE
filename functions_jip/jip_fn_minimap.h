#pragma once

DEFINE_COMMAND_PLUGIN(InitMiniMap, 0, nullptr);
DEFINE_COMMAND_PLUGIN(UpdateMiniMap, 0, kParams_OneInt_OneOptionalInt);

#define CACHED_TEXTURES_MAX 60
#define CACHED_TEXTURES_MIN 42

constexpr float kInteriorZoomMod = 0.25F;

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
		EMIT_PS_2(0x45000000)
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
	
	void __fastcall InitDimensions(TESWorldSpace *worldSpc);
	void __fastcall InitPosMods(TESWorldSpace *worldSpc);
}
s_rootWorldDimensions;

__declspec(naked) void __fastcall WorldDimensions::InitDimensions(TESWorldSpace *worldSpc)
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
		EMIT_DW(0x00010000) EMIT_DW(0x00000001) DUP_2(EMIT_DW(0x3F4C0000))
	}
}

__declspec(naked) void __fastcall WorldDimensions::InitPosMods(TESWorldSpace *worldSpc)
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
		EMIT_PS_2(0x3DD00000)
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
		EMIT_DW_4(0x00000000, 0x00000001, 0x00010000, 0x00010001)
		EMIT_DW_4(0x04060002, 0x0C0E080A, 0x80808080, 0x80808080)
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
		EMIT_DW_4(0xFFFFFFFF, 0xFFFF0000, 0xFFFF0001, 0x0000FFFF)
		EMIT_DW_4(0x00000001, 0x0001FFFF, 0x00010000, 0x00010001)
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
		mov		eax, [ecx+0x34]
		test	eax, eax
		jz		noBuffer
		and		dword ptr [eax+0x20], 0
	noBuffer:
		mov		esi, [ecx+0x28]
		add		esi, 0xC
		mov		edi, s_packedCellCoords[ebx*4]
		xor		eax, eax
		mov		bl, 0x10
		ALIGN 16
	iterHead:
		mov		edx, edi
		test	bl, 0x10
		setnz	al
		add		dx, ax
		movd	xmm0, edx
		mov		edx, ebx
		mov		ecx, ebp
		call	GetSectionSeenLevel
		mov		ecx, s_vertexAlphaLevel[eax*4]
		mov		[esi], ecx
		add		esi, 0x10
		dec		bl
		jns		iterHead
		mov		bl, 0x10
		inc		bh
		cmp		bh, bl
		jb		iterHead
		lea		edi, [edi+0x10000]
		jz		iterHead
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
		EMIT_PS_4(0x00000FFF)
	kGridSlice:
		DUP_2(EMIT_DW(0x000004FF) EMIT_DW(0x00000AFF))
	kUpdateMask:
		EMIT_W_8(0x001B, 0x0012, 0x0012, 0x0036, 0x0018, 0x0010, 0x0010, 0x0030)
		EMIT_W_8(0x0030, 0x0030, 0x0030, 0x0030, 0x00D8, 0x0090, 0x0090, 0x01B0)
	}
}

__declspec(naked) void __vectorcall DoSelectiveFOWUpdate(__m128 adjustedPos)
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
		call	Game_DoHeapAlloc
		mov		dword ptr [eax], kVtbl_ExtraSeenData
		mov		[eax+4], 5
		and		dword ptr [eax+8], 0
	createData:
		push	eax
		xorps	xmm0, xmm0
		test	byte ptr [esi+0x24], 1
		jnz		isInterior
		push	0x24
		call	Game_DoHeapAlloc
		mov		dword ptr [eax], kVtbl_SeenData
		jmp		doneData
	isInterior:
		push	0x2C
		call	Game_DoHeapAlloc
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
		call	Game_DoHeapAlloc
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
		DUP_2(EMIT_DW(0x00000800)) EMIT_DW(0x45C80000) EMIT_DW(0x80800002)
		EMIT_DW(0xFFFFFFFF) EMIT_DW(0xFFFF0000) EMIT_DW(0xFFFF0001)
		EMIT_DW(0x0000FFFF) EMIT_DW_0 EMIT_DW(0x00000001)
		EMIT_DW(0x0001FFFF) EMIT_DW(0x00010000) EMIT_DW(0x00010001)
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
	for (auto iter = g_loadedReferences->teleportDoors.Begin(); iter; ++iter)
		if (TESObjectREFR *refr = iter.Get(); refr && !(refr->flags & 0x860) && (refr->parentCell == cell))
			if (ExtraTeleport *xTeleport = GetExtraType(&refr->extraDataList, ExtraTeleport); xTeleport->data->linkedDoor)
				doorRefsList->Append(refr, xTeleport->data->linkedDoor->parentCell);
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
		EMIT_PS_2(0x42400000)
	}
}

NiCamera *s_localMapCamera;
TextureParams s_mmTextureParams(0x180, 0x180, D3DFMT_X8R8G8B8, kRndrMode_Normal, 0);

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
		mov		ecx, [ebp-0x14]
		mov		eax, [ecx+0x48]
		movq	xmm0, qword ptr [eax]
		pslld	xmm0, 0xC
		paddd	xmm0, kExtPosMod
		cvtdq2ps	xmm0, xmm0
		movss	xmm1, kExtPosMod+0x10
		mov		eax, [ecx+0x4C]
		test	eax, eax
		jz		doneZ
		mov		ecx, [eax+0x28]
		test	ecx, ecx
		jz		doneZ
		comiss	xmm1, [ecx+0x18]
		jb		doneZ
		movss	xmm1, [ecx+0x1C]
		addss	xmm1, kExtPosMod+0x14
	doneZ:
		unpcklpd	xmm0, xmm1
		mov		eax, s_localMapCamera
		movups	[eax+0x8C], xmm0
		push	dword ptr [ebp-0x18]
		push	offset s_mmTextureParams
		push	eax
		mov		byte ptr ds:0x11AD7B4, 0
		call	BSTextureManager::GenerateRenderedTexture
		mov		byte ptr ds:0x11AD7B4, 1
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
		EMIT_PS_2(0x00000800) EMIT_DW(0x47800000) EMIT_DW(0x46800000)
	}
}

__declspec(naked) void __vectorcall GenerateLocalMapInterior(__m128i coord, NiRenderedTexture **renderedTexture)
{
	__asm
	{
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
		movss	xmm1, kIntPosMod+0x10
		unpcklpd	xmm0, xmm1
		mov		eax, s_localMapCamera
		movups	[eax+0x8C], xmm0
		push	ecx
		push	offset s_mmTextureParams
		push	eax
		mov		byte ptr ds:0x11AD7B4, 0
		call	BSTextureManager::GenerateRenderedTexture
		mov		byte ptr ds:0x11AD7B4, 1
		retn
		ALIGN 16
	kIntPosMod:
		EMIT_PS_2(0x00000001) EMIT_DW(0x47000000)
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

	void SetInUse(bool bIU)
	{
		inUse = bIU;
		visible->SetBool(bIU);
	}
};

typedef Vector<MapMarkerTile*, 4> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles, 0x40, false> RenderedMapMarkers;
TempObject<RenderedMapMarkers> s_renderedMapMarkers;

void __fastcall FreeCellMapMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
		dtlIter->SetInUse(0);
	cmkIter.Remove();
}

bool s_discoveredLocation = false;

const char* __fastcall DiscoverLocationHook(MapMarkerData *mkData, PlayerCharacter::MapMarkerInfo *mkInfo)
{
	s_discoveredLocation = true;
	if (!s_renderedMapMarkers->Empty())
	{
		Coordinate coord;
		GetWorldMapPosMults(mkInfo->markerRef->position.PS2(), s_rootWorldDimensions, &coord);
		if (auto findRendered = s_renderedMapMarkers->Find(coord))
			FreeCellMapMarkers(findRendered);
	}
	const char *mkName = mkData->fullName.name.m_data;
	return mkName ? mkName : (const char*)0x1011584;
}

bool s_updateQuestTargets = false;

__declspec(naked) void SetQuestTargetsHook()
{
	__asm
	{
		add		ecx, 0x4C
		mov		[ebp-4], ecx
		mov		s_updateQuestTargets, 1
		JMP_EAX(0x60F145)
	}
}

__declspec(naked) void __fastcall UpdatePlacedMarkerHook(PlayerCharacter *thePlayer, int, TESObjectREFR *markerRef, ObjectiveTargetData *targetData, int)
{
	__asm
	{
		mov		s_updateQuestTargets, 1
		JMP_EAX(0x952D60)
	}
}

__declspec(naked) void RendererRecreateHook()
{
	__asm
	{
		and		s_mmTextureParams.d3dFormat, 0
		JMP_EAX(0xE736B0)
	}
}

UInt8 s_miniMapIndex = 0;
TileRect *s_worldMapRect, *s_mapMarkersRect, *s_doorMarkersRect, *s_lQuestMarkersRect, *s_wQuestMarkersRect;
TileImage *s_worldMapTile;
TileValue *s_miniMapMode = nullptr, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom;
TileShaderProperty *s_tileShaderProps[9];
float s_fLocalMapZoomCurr;
NiDirectionalLight *s_mmDirectionalLight;
BSFogProperty *s_mmFogProperty;
NiNode *s_fakeWaterPlanes, *s_shadowSceneNodes[5];
bool s_isInExterior = true;
TempObject<NiFixedString> s_BlackPlane01;

__declspec(naked) NiNode *CreateFakeWaterPlanes()
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		xor		ebx, ebx
		CALL_EAX(0xA5F030)
		mov		esi, eax
		mov		dword ptr [eax+4], 2
		push	0x7C
		call	Game_DoHeapAlloc
		movq	xmm0, qword ptr kWaterPlanePos+0x10
		shufps	xmm0, xmm0, 0x61
		movups	[eax], xmm0
		shufps	xmm0, xmm0, 0x58
		movups	[eax+0x10], xmm0
		shufps	xmm0, xmm0, 0x49
		movups	[eax+0x20], xmm0
		movaps	xmm0, kWaterPlaneColor
		movups	[eax+0x30], xmm0
		movups	[eax+0x40], xmm0
		movups	[eax+0x50], xmm0
		movups	[eax+0x60], xmm0
		lea		ecx, [eax+0x70]
		mov		dword ptr [ecx], 0x10000
		mov		dword ptr [ecx+4], 0x20002
		mov		dword ptr [ecx+8], 0x30001
		push	ecx
		push	2
		push	ebx
		push	ebx
		push	ebx
		sub		ecx, 0x40
		push	ecx
		push	ebx
		push	eax
		push	4
		push	0x58
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA7B630)
		mov		edi, eax
		ALIGN 16
	iterHead:
		push	edi
		push	0xC4
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA74480)
		or		byte ptr [eax+0x30], 1
		movq	xmm0, qword ptr kWaterPlanePos[ebx*8]
		movlps	[eax+0x58], xmm0
		push	1
		push	eax
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0xDC]
		inc		ebx
		cmp		bl, 9
		jb		iterHead
		mov		ecx, esi
		call	NiNode::AddNoLightingPropertyRecurse
		push	s_alphaProperty
		mov		ecx, esi
		call	NiNode::AddPropertyRecurse
		mov		ecx, esi
		call	NiAVObject::AssignGeometryProps
		mov		eax, esi
		pop		edi
		pop		esi
		pop		ebx
		retn
		ALIGN 16
	kWaterPlaneColor:
		EMIT_DW_4(0x3DB8B8B9, 0x3E4CCCCD, 0x3E3CBCBD, 0x3F38B8B9)
	kWaterPlanePos:
		EMIT_DW_4(0x45000000, 0xC6200000, 0x45000000, 0xC5C00000)
		EMIT_DW_4(0x45000000, 0xC5000000, 0x45C00000, 0xC6200000)
		EMIT_DW_4(0x45C00000, 0xC5C00000, 0x45C00000, 0xC5000000)
		EMIT_DW_4(0x46200000, 0xC6200000, 0x46200000, 0xC5C00000)
		EMIT_DW(0x46200000) EMIT_DW(0xC5000000)
	}
}

struct MapMarkerInfo
{
	TESObjectREFR		*refr;
	MapMarkerData		*data;
	NiPoint2			pos;

	MapMarkerInfo(TESObjectREFR *_refr, MapMarkerData *_data, const NiPoint2 &posXY) :
		refr(_refr), data(_data), pos(posXY) {}

	const char *GetMarkerIconPath() const;
};

UInt32 kJGGetIconPathAddr = 0;

__declspec(naked) const char *MapMarkerInfo::GetMarkerIconPath() const
{
	__asm
	{
		mov		edx, [ecx+4]
		test	byte ptr [edx+0xC], 2
		jz		undiscovered
		movzx	edx, byte ptr [edx+0xE]
		mov		eax, kJGGetIconPathAddr
		test	eax, eax
		jnz		jgHook
		mov		eax, ds:0x11A0404[edx*4]
		retn
	undiscovered:
		mov		eax, 0x1075030
		retn
	jgHook:
		mov		ecx, [ecx]
		jmp		eax
	}
}

typedef Vector<MapMarkerInfo, 2> CellMapMarkers;
typedef UnorderedMap<UInt32, CellMapMarkers, 0x80, false> WorldMapMarkers;
TempObject<Map<TESWorldSpace*, WorldMapMarkers>> s_worldMapMarkers;

const __m128 kLocalMapPosMults = {1 / 12288.0F, 1 / -12288.0F, 0, 0}, kVertexAlphaMults = {0.25F, 0.5F, 0.75F, 1.0F};
const __m128i kNWCoordAdjust = _MM_SET_EPI32(-1, 2, 0, 0);

struct RenderedEntry
{
	NiRenderedTexture	*texture = nullptr;
	UInt8				regenFlags = 0;

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
		visited = markerTile->GetValue(kTileValue_user2);
		markerTile->SetFloat(kTileValue_depth, int((s_doorMarkerTiles->Size() & 0xF) + 18));
	}

	void SetInUse(bool bIU)
	{
		inUse = bIU;
		visible->SetBool(bIU);
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
	UInt32			pos = 0x7F7FFFFF;
	TileValue		*x;
	TileValue		*y;
};

UInt32 s_currentMapMode = 0, s_showFlags = 0;
TESWorldSpace *s_pcCurrWorld, *s_pcRootWorld;
Coordinate s_currWorldCoords = 0x7FFF7FFF, s_currLocalCoords = 0x7FFF7FFF, s_cellGridCenter;
WorldMapMarkers *s_currWorldMarkers;
TempObject<Set<UInt32>> s_currMarkerCells(0x40);
typedef UnorderedMap<UInt32, RenderedEntry, 0x40, false> LocalTexturesMap;
TempObject<LocalTexturesMap> s_renderedExteriors, s_renderedInterior;
TempObject<UnorderedMap<UInt32, DoorRefsList, 0x20, false>> s_exteriorDoorRefs;
TempObject<DoorRefsList> s_doorRefsList(0x40);
TempObject<Vector<CulledObject>> s_hiddenObjectsExt(0x40);
TempObject<Vector<NiAVObject*>> s_hiddenObjectsInt(0x40);
TESQuest *s_activeQuest = nullptr;
TESObjectREFR *s_placedMarker = nullptr;
TempObject<Map<TESObjectREFR*, QuestMarkerTile>> s_questMarkers(0x10);
TESObjectCELL *s_currCellGrid[9];

#define LIGHTING_PASSES *(UInt8*)0x11F91D8
#define TEXTURE_LIST_BGN *(NiTexture**)0x11F4500
#define TEXTURE_LIST_END *(NiTexture**)0x11F4504
#define TEXTURE_LIST_CS (CriticalSection*)0x11F4580

NiRenderedTexture *s_mmFirstTexture = nullptr;

__declspec(naked) NiRenderedTexture* __fastcall FindNextMMTexture(NiTexture *texture)
{
	__asm
	{
		mov		eax, ecx
		mov		ecx, kVtbl_NiRenderedTexture
		xor		edx, edx
		ALIGN 16
	iterHead:
		mov		eax, [eax+0x2C]
		test	eax, eax
		jz		done
		cmp		[eax], ecx
		jnz		iterHead
		cmp		[eax+0x34], edx
		jz		iterHead
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

bool Cmd_InitMiniMap_Execute(COMMAND_ARGS)
{
	if (s_miniMapMode) return true;

	if (!s_miniMapIndex)
	{
		UInt8 modIdx = g_dataHandler->GetModIndex("JIP MiniMap.esp");
		if (modIdx != 0xFF)
			s_miniMapIndex = modIdx;
	}
	if (scriptObj->modIndex != s_miniMapIndex)
		return true;

	auto tile = g_HUDMainMenu->tile->GetComponentTile("JIPMiniMap");
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
	tile = node->data;
	s_localMapZoom = tile->GetValue(kTileValue_zoom);
	s_worldMapRect = (TileRect*)node->prev->data;
	s_worldMapZoom = s_worldMapRect->GetValue(kTileValue_zoom);
	node = tile->children.Tail();

	NiVector3 *shapeVertices = Game_HeapAlloc<NiVector3>(0x121), *pVertices = shapeVertices, vertex(0, 0, 0);
	NiPoint2 *shapeUVCoords = Game_HeapAlloc<NiPoint2>(0x121), *pUVCoords = shapeUVCoords, uvCoord(0, 0);
	UInt32 index = 0x11, iterZ = 0x11;
	while (true)
	{
		*pVertices++ = vertex;
		*pUVCoords++ = uvCoord;
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

	NiTriangle *shapeTriangles = Game_HeapAlloc<NiTriangle>(0x200), *pTriangles = shapeTriangles;
	UInt32 vtx1 = 0, vtx2 = 0x11;
	do
	{
		index = 0x10;
		do
		{
			pTriangles[0].point1 = vtx2;
			pTriangles[0].point2 = vtx1++;
			pTriangles[0].point3 = vtx1;
			pTriangles[1].point1 = vtx1;
			pTriangles[1].point3 = vtx2++;
			pTriangles[1].point2 = vtx2;
			pTriangles += 2;
		}
		while (--index);
		vtx1++;
		vtx2++;
	}
	while (vtx2 < 0x120);

	auto vertexColors = Game_HeapAlloc<NiColorAlpha>(0xA29);
	ZERO_BYTES(vertexColors, sizeof(NiColorAlpha) * 0xA29);

	auto alphaProp = NiAlphaProperty::Create();
	alphaProp->flags = 0x8D;

	index = 0;
	do
	{
		auto localTile = (TileImage*)node->data;
		node = node->prev;
		auto sciTriShp = (BSScissorTriShape*)localTile->node->m_children[0];
		s_localMapShapes[index] = sciTriShp;
		auto shapeData = (NiTriShapeData*)sciTriShp->geometryData;
		s_localMapShapeDatas[index] = shapeData;
		s_tileShaderProps[index] = localTile->shaderProp;
		NiReplaceObject(&localTile->shaderProp->srcTexture, nullptr);

		shapeData->numVertices = 0x121;
		shapeData->numTriangles = 0x200;
		shapeData->trianglePoints = 0x600;

		Game_HeapFree(shapeData->vertices);
		shapeData->vertices = shapeVertices;

		Game_HeapFree(shapeData->normals);
		shapeData->normals = nullptr;

		Game_HeapFree(shapeData->vertexColors);
		shapeData->vertexColors = vertexColors;
		vertexColors += 0x121;

		Game_HeapFree(shapeData->uvCoords);
		shapeData->uvCoords = shapeUVCoords;

		Game_HeapFree(shapeData->triangles);
		shapeData->triangles = shapeTriangles;

		NiReplaceObject(&sciTriShp->m_propertyList.Head()->data, alphaProp);
		NiReplaceObject(&sciTriShp->alphaProp, alphaProp);
	}
	while (++index < 9);

	s_doorMarkersRect = (TileRect*)node->data;
	s_lQuestMarkersRect = (TileRect*)node->prev->data;
	node = s_worldMapRect->children.Tail();
	s_worldMapTile = (TileImage*)node->data;
	node = node->prev;
	s_mapMarkersRect = (TileRect*)node->data;
	node = node->prev;
	s_wQuestMarkersRect = (TileRect*)node->data;

	s_fLocalMapZoomCurr = s_localMapZoom->num;
	if (g_TES->currentInterior)
	{
		s_isInExterior = false;
		s_fLocalMapZoomCurr += kInteriorZoomMod;
	}
	UpdateTileScales(s_fLocalMapZoomCurr);

	auto lmCamera = NiCamera::Create();
	lmCamera->m_uiRefCount = 2;
	lmCamera->WorldRotate()[0] = 0;
	lmCamera->WorldRotate()[2] = 1.0F;
	lmCamera->WorldRotate()[6] = -1.0F;;
	lmCamera->WorldRotate()[8] = 0;
	lmCamera->frustum.viewPort = {-2048.0F, 2048.0F, 2048.0F, -2048.0F};
	lmCamera->frustum.dNear = 1.0F;
	lmCamera->frustum.dFar = 131072.0F;
	lmCamera->frustum.ortho = 1;
	lmCamera->minNearPlaneDist = 1.0F;
	lmCamera->maxFarNearRatio = 70768.0F;
	lmCamera->LODAdjust = 0.001F;
	g_shadowSceneNode->AddObject(lmCamera, 1);
	s_localMapCamera = lmCamera;

	auto dirLight = NiDirectionalLight::Create();
	dirLight->m_uiRefCount = 2;
	dirLight->ambientColor = {NRGB(175, 175, 175)};
	dirLight->diffuseColor = {NRGB(169, 167, 140)};
	dirLight->fogColor = {0, 0, 0};
	dirLight->direction = {0.6154797F, -0.5235988F, 0.6154797F};
	s_mmDirectionalLight = dirLight;

	auto fogProperty = (BSFogProperty*)memcpy(Ni_Alloc<BSFogProperty>(), *(void**)0x11DEB00, sizeof(BSFogProperty));
	fogProperty->color = {NRGB(23, 51, 47)};
	fogProperty->startDistance = FLT_MAX;
	fogProperty->endDistance = FLT_MAX;
	s_mmFogProperty = fogProperty;

	s_fakeWaterPlanes = CreateFakeWaterPlanes();
	GameGlobals::ObjectLODRoot()->AddObject(s_fakeWaterPlanes, 1);

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
		worldDimensions.InitPosMods(worldSpc);
		worldSpc = worldSpc->GetRootMapWorld();
		if (rootWorld != worldSpc)
		{
			rootWorld = worldSpc;
			worldDimensions.InitDimensions(rootWorld);
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

	if (*(UInt32*)0x79D32F == 0xE89090FF)
	{
		UInt32 procAddr = *(UInt32*)0x79D333 + 0x79D337;
		if ((*(UInt32*)procAddr == 0x4D8BD08B) && (*(UInt16*)(procAddr + 4) == 0xE9DC))
			kJGGetIconPathAddr = *(UInt32*)(procAddr + 6) + procAddr + 0xA;
	}

	s_shadowSceneNodes[0] = *(NiNode**)0x11DEB34;	// Sky
	s_shadowSceneNodes[1] = *(NiNode**)0x11DEDA4;	// Weather
	s_shadowSceneNodes[2] = *(NiNode**)0x11D86A8;	// TerrainLOD
	s_shadowSceneNodes[3] = *(NiNode**)0x11DED58;	// ParticleSysMngr
	s_shadowSceneNodes[4] = *(NiNode**)0x11CA438;	// Grass
	s_BlackPlane01() = "BlackPlane01";
	SafeWrite16(0x452736, 0x7705);
	SafeWrite8(0x555C20, 0xC3);
	WriteRelCall(0x9438F6, (UInt32)UpdateCellsSeenBitsHook);
	WriteRelCall(0x77951E, (UInt32)DiscoverLocationHook);
	WriteRelJump(0x60F13A, (UInt32)SetQuestTargetsHook);
	WriteRelCall(0x952C69, (UInt32)UpdatePlacedMarkerHook);
	WriteRelCall(0x952F6B, (UInt32)UpdatePlacedMarkerHook);
	WriteRelJump(0xE74C3C, (UInt32)RendererRecreateHook);
	return true;
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
			cmp		s_isInExterior, 0
			jnz		notInterior
			addss	xmm0, kInteriorZoomMod
		notInterior:
			movss	s_fLocalMapZoomCurr, xmm0
		sameZoom:
			call	UpdateTileScales
		}
		if (showFlags == 0x100)
			return true;
	}

	auto thePlayer = g_thePlayer;
	auto parentCell = thePlayer->parentCell;
	if (!parentCell) return true;
	auto parentWorld = parentCell->worldSpace;

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
		movlps	[ecx+0x6C], xmm0
		movhps	[ecx+0x78], xmm0
		cmp		parentWorld, 0
		setnz	al
		cmp		s_isInExterior, al
		jz		noChange
		mov		s_isInExterior, al
		shl		eax, 0x1F
		or		eax, kInteriorZoomMod
		movd	xmm0, eax
		addss	xmm0, s_fLocalMapZoomCurr
		movss	s_fLocalMapZoomCurr, xmm0
		call	UpdateTileScales
	noChange:
	}

	UInt32 showFlagsChange = s_showFlags ^ showFlags;
	s_showFlags = showFlags;

	bool updQuestTargets = s_updateQuestTargets;
	s_updateQuestTargets = false;

	if (s_currentMapMode != updateType)
	{
		if (updateType <= 1)
		{
			s_currentMapMode = updateType;
			s_miniMapMode->SetBool(updateType == 1);
		}
		updateTiles = true;
		updQuestTargets = true;
	}

	bool worldMap = s_currentMapMode != 0;
	NiPoint2 nwXY, posMult, adjustedPos;
	Coordinate coord;
	Tile *markerTile;
	TESObjectREFR *objectRef;
	auto hudMain = g_HUDMainMenu;

	bool useFogOfWar = (showFlags & 0x10) != 0;
	if (showFlagsChange & 0x10)
	{
		updateTiles = true;
		for (auto shaderProp : s_tileShaderProps)
			shaderProp->hasVtxColors = useFogOfWar;
		if (useFogOfWar)
			__asm
			{
				mov		eax, s_tileShaderProps
				movss	xmm0, [eax+0x78]
				shufps	xmm0, xmm0, 0
				mulps	xmm0, kVertexAlphaMults
				movups	s_vertexAlphaLevel+4, xmm0
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
			s_rootWorldDimensions.InitPosMods(parentWorld);
			auto rootWorld = parentWorld->GetRootMapWorld();
			if (s_pcRootWorld != rootWorld)
			{
				s_pcRootWorld = rootWorld;
				s_rootWorldDimensions.InitDimensions(rootWorld);
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
						if (tileIter().inUse) tileIter().SetInUse(0);
					s_renderedMapMarkers->Clear();
				}
			}
			updateTiles = true;
		}

		posMult.SetPS(GetWorldMapPosMults(objectRef->position.PS2(), s_rootWorldDimensions, &coord));
		updateTiles |= s_currWorldCoords != coord;
		s_currWorldCoords = coord;

		if (showFlagsChange & 1)
			s_mapMarkersRect->SetBool(kTileValue_visible, (showFlags & 1) != 0);
		if ((showFlags & 1) && (updateTiles || s_discoveredLocation))
		{
			s_discoveredLocation = false;
			s_currMarkerCells->Clear();
			if (s_currWorldMarkers && !s_currWorldMarkers->Empty())
			{
				int range = iceil(s_rootWorldDimensions.mkRange / s_worldMapZoom->num);
				coord.x -= range;
				coord.y -= range;
				range = (range << 1) + 1;
				auto tileIter = s_mapMarkerTiles->Begin();
				UInt32 iterX = range;
				do
				{
					UInt32 iterY = range;
					do
					{
						CellMapMarkers *mapMarkers = s_currWorldMarkers->GetPtr(coord);
						if (!mapMarkers)
							goto iterYnext;
						s_currMarkerCells->Insert(coord);
						DynamicTiles *dynamicTiles;
						if (!s_renderedMapMarkers->Insert(coord, &dynamicTiles))
							goto iterYnext;
						for (auto mkIter = mapMarkers->Begin(); mkIter; ++mkIter)
						{
							if (!(mkIter().data->flags & 1) || (mkIter().refr->flags & 0x800))
								continue;
							MapMarkerTile *tileData;
							while (true)
							{
								if (!tileIter)
								{
									tileData = s_mapMarkerTiles->Append(hudMain->AddTileFromTemplate(s_mapMarkersRect, "MiniMapWorldMarkerTemplate"));
									break;
								}
								tileData = &tileIter();
								++tileIter;
								if (!tileData->inUse)
								{
									tileData->SetInUse(1);
									break;
								}
							}
							dynamicTiles->Append(tileData);
							tileData->x->SetFloat(mkIter().pos.x);
							tileData->y->SetFloat(mkIter().pos.y);
							tileData->filename->SetString(mkIter().GetMarkerIconPath());
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
	else
	{
		bool updateFogOfWar = useFogOfWar && s_updateFogOfWar/*, saveToFile = (showFlags & 0x20) != 0*/;
		s_updateFogOfWar = false;

		D3DFORMAT d3dFormat = (showFlags & 8) ? D3DFMT_X8R8G8B8 : D3DFMT_L8;
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
					s_currCellGrid[gridIdx] = g_gridCellArray->GetCellAtCoord(s_packedCellCoords[gridIdx].PS());
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
					s_hiddenObjectsExt->Append(cell->Get3DNode(4));
					for (auto anmIter = cell->renderData->animatedRefs.Begin(); anmIter; ++anmIter)
						if ((hideNode = anmIter.Get()) && ((hideNode->GetBSXFlags() & 0x23) == 0x21))
							s_hiddenObjectsExt->Append(hideNode);
					if (!(cell->cellFlags & 2))
						continue;
					objectRef = nullptr;
					auto awtIter = cell->renderData->placeableWaterList.Head();
					do
					{
						if (!awtIter->data || !awtIter->data->IsCreated() || !(hideNode = awtIter->data->GetRefNiNode()))
							continue;
						s_hiddenObjectsExt->Append(hideNode);
						objectRef = awtIter->data;
					}
					while (awtIter = awtIter->next);
					if (!objectRef) continue;
					auto pWaterPlane = s_fakeWaterPlanes->m_children[gridIdx];
					pWaterPlane->Show();
					if (updateWater = updateTiles)
						pWaterPlane->LocalTranslate().z = objectRef->position.z;
				}
				while (++gridIdx < 9);
				if (!updateTiles)
					s_hiddenObjectsExt->Append(*(NiNode**)0x11D8690);
				else if (updateWater)
				{
					s_fakeWaterPlanes->LocalTranslate() = nwXY;
					ThisCall(0xA5DD70, s_fakeWaterPlanes, &kNiUpdateData, 0);
				}
				GameGlobals::SceneLightsLock()->Enter();
				for (auto lgtNode = g_shadowSceneNode->sceneLights.Begin(); lgtNode; ++lgtNode)
					if (lgtNode->isPointLight && lgtNode->light && !(lgtNode->light->m_flags & 1))
						s_hiddenObjectsExt->Append(lgtNode->light);
				g_shadowSceneNode->pSunLight->light = s_mmDirectionalLight;
				g_shadowSceneNode->fogProperty = s_mmFogProperty;
				
				bool underwaterFog = *(bool*)0x11FF104;
				*(bool*)0x11FF104 = 1;
				LIGHTING_PASSES = 0x3C;
				s_mmTextureParams.renderMode = kRndrMode_Normal;
				const UInt8 kSelectUpdate[] = {0x8, 0xA, 0x2, 0xC, 0xF, 0x3, 0x4, 0x5, 0x1};
				gridIdx = 0;
				do
				{
					if (!(cell = s_currCellGrid[gridIdx]))
						continue;
					auto updateMask = kSelectUpdate[gridIdx];
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
						auto texture = textureEntry->texture;
						if (texture != TEXTURE_LIST_END)
						{
							ScopedCS cs(TEXTURE_LIST_CS);
							if (texture == s_mmFirstTexture)
								s_mmFirstTexture = FindNextMMTexture(texture);
							auto prev = texture->prev, next = texture->next;
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

				*(bool*)0x11FF104 = underwaterFog;
				s_hiddenObjectsExt->Clear();
				g_shadowSceneNode->pSunLight->light = g_TES->directionalLight;
				g_shadowSceneNode->fogProperty = *(BSFogProperty**)0x11DEB00;
				GameGlobals::SceneLightsLock()->Leave();
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
					auto curr = s_mmFirstTexture;
					ScopedCS cs(TEXTURE_LIST_CS);
					do
					{
						auto next = FindNextMMTexture(curr);
						purgeCount -= s_renderedExteriors->Erase(curr->cellRefID);
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

			if (updateTiles)
			{
				if (s_lastInterior != parentCell)
				{
					s_lastInterior = parentCell;
					s_renderedInterior->Clear();
				}
				if (showFlags & 2)
				{
					s_doorRefsList->Clear();
					GetTeleportDoors(parentCell, *s_doorRefsList);
				}
				s_hiddenObjectsInt->Clear();
				auto cellNodes = parentCell->Get3DNodes();
				s_hiddenObjectsInt->Append(cellNodes[0]);
				s_hiddenObjectsInt->Append(cellNodes[4]);
				for (auto objIter = cellNodes[3]->m_children.Begin(); objIter; ++objIter)
					if (*objIter && (objIter->m_blockName == s_BlackPlane01))
						s_hiddenObjectsInt->Append(*objIter);
			}

			if (updateTiles |= (s_currLocalCoords != coord))
			{
				s_currLocalCoords = coord;
				PackCurrGridCoords(coord);
				for (auto objIter = s_hiddenObjectsInt->Begin(); objIter; ++objIter)
					objIter->Hide();
				s_shadowSceneNodes[3]->Hide();
				LIGHTING_PASSES = 0x34;

				s_mmTextureParams.renderMode = kRndrMode_NoWaterPass;
				gridIdx = 0;
				do
				{
					coord = s_packedCellCoords[gridIdx];
					if (s_renderedInterior->Insert(coord, &textureEntry))
					{
						GenerateLocalMapInterior(coord.PS(), &textureEntry->texture);
						/*if (saveToFile)
							SaveLocalMapTexture(parentCell, textureEntry->texture, coord);*/
					}
					s_tileShaderProps[gridIdx]->srcTexture = textureEntry->texture;
					if (useFogOfWar)
						CalcVtxAlphaBySeenData(gridIdx);
				}
				while (++gridIdx < 9);

				for (auto objIter = s_hiddenObjectsInt->Begin(); objIter; ++objIter)
					objIter->Show();
				s_shadowSceneNodes[3]->Show();
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(adjustedPos.PS());
		}
		LIGHTING_PASSES = lightingPasses;

		if (showFlagsChange & 2)
			s_doorMarkersRect->SetBool(kTileValue_visible, (showFlags & 2) != 0);
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
					tileData->visited->SetBool(!cell || (cell == parentCell) || cell->extraDataList.HasType(kXData_ExtraDetachTime));
					tileData->inUse = true;
					if (useFogOfWar)
						tileData->vtxAlpha = GetVtxAlphaPtr(adjustedPos.PS());
					tileData->visible->SetBool(!useFogOfWar || *tileData->vtxAlpha);
				}

				for (; markerIter; ++markerIter)
					if (markerIter().inUse) markerIter().SetInUse(0);
			}
			else if (updateFogOfWar)
				for (auto markerIter = s_doorMarkerTiles->Begin(); markerIter; ++markerIter)
					if (markerIter().inUse)
						markerIter().visible->SetBool(*markerIter().vtxAlpha != 0);
		}
	}

	if (showFlagsChange & 4)
	{
		s_lQuestMarkersRect->SetBool(kTileValue_visible, (showFlags & 4) != 0);
		s_wQuestMarkersRect->SetBool(kTileValue_visible, (showFlags & 4) != 0);
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
				auto targetsRect = worldMap ? s_wQuestMarkersRect : s_lQuestMarkersRect;
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
				adjustedPos.SetPS(GetWorldMapPosMults(objectRef->position.PS2(), s_rootWorldDimensions));
			else if (parentWorld)
				adjustedPos.SetPS((objectRef->position.PS2() - nwXY.PS()) * kLocalMapPosMults);
			else
				adjustedPos.SetPS((AdjustInteriorPos(objectRef->position.PS2()) - nwXY.PS()) * kLocalMapPosMults);
			markerData->x->SetFloat(adjustedPos.x);
			markerData->y->SetFloat(adjustedPos.y);
		}
	}

	s_pcMarkerRotate->SetFloat(s_cellNorthRotation - thePlayer->rotation.z);
	s_miniMapPosX->SetFloat(posMult.x);
	s_miniMapPosY->SetFloat(posMult.y);
	return true;
}