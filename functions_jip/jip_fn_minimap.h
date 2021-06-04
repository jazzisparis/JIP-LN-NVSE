#pragma once

DEFINE_COMMAND_PLUGIN(InitMiniMap, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(UpdateMiniMap, , 0, 2, kParams_OneInt_OneOptionalInt);

#define CACHED_TEXTURES_MAX 60
#define CACHED_TEXTURES_MIN 42

Tile::Value *s_miniMapScale, *s_localMapZoom;
BSScissorTriShape *s_localMapShapes[9];
NiTriShapeData *s_localMapShapeDatas[9];
UInt32 s_texturePixelSize = 0x110;

__declspec(naked) void UpdateTileScales()
{
	__asm
	{
		mov		eax, s_localMapZoom
		movss	xmm0, [eax+8]
		movss	xmm1, kFltHalf
		addss	xmm1, xmm0
		mov		eax, s_miniMapScale
		mulss	xmm1, [eax+8]
		mov		ecx, offset s_localMapShapes
		movd	edx, xmm1
		mov		eax, [ecx]
		mov		[eax+0x64], edx
		mov		eax, [ecx+4]
		mov		[eax+0x64], edx
		mov		eax, [ecx+8]
		mov		[eax+0x64], edx
		mov		eax, [ecx+0xC]
		mov		[eax+0x64], edx
		mov		eax, [ecx+0x10]
		mov		[eax+0x64], edx
		mov		eax, [ecx+0x14]
		mov		[eax+0x64], edx
		mov		eax, [ecx+0x18]
		mov		[eax+0x64], edx
		mov		eax, [ecx+0x1C]
		mov		[eax+0x64], edx
		mov		eax, [ecx+0x20]
		mov		[eax+0x64], edx
		mulss	xmm0, kFlt10
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
		fld		dword ptr [eax+0xC]
		fchs
		fst		s_cellNorthRotation
		mov		eax, offset s_northRotationMods
		fsincos
		fst		dword ptr [eax]
		fstp	dword ptr [eax+0xC]
		fst		dword ptr [eax+4]
		fchs
		fstp	dword ptr [eax+8]
		retn
		ALIGN 16
	noRotation:
		mov		s_cellNorthRotation, eax
		retn
	}
}

NiCamera *s_localMapCamera = NULL;

__declspec(naked) void __fastcall SetCameraRotation(TESObjectCELL *cell)
{
	__asm
	{
		call	GetNorthRotation
		mov		ecx, s_localMapCamera
		test	eax, eax
		jz		noRot
		mov		edx, [eax]
		mov		[ecx+0x3C], edx
		mov		[ecx+0x44], edx
		mov		edx, [eax+4]
		mov		[ecx+0x38], edx
		mov		edx, [eax+8]
		mov		[ecx+0x48], edx
		retn
		ALIGN 16
	noRot:
		mov		[ecx+0x38], eax
		mov		[ecx+0x48], eax
		mov		edx, 0x3F800000
		mov		[ecx+0x3C], edx
		mov		[ecx+0x44], edx
		retn
	}
}

__declspec(naked) void __fastcall AdjustInteriorPos(TESObjectREFR *refr, NiPoint2 *outPos)
{
	static const __m128 kIntrPosMods = {2048, 2048, 0, 0};
	__asm
	{
		movq	xmm0, qword ptr [ecx+0x30]
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm0, xmm0
		mulps	xmm0, s_northRotationMods
		pxor	xmm1, xmm1
		movhlps	xmm1, xmm0
		addps	xmm0, xmm1
	noRotation:
		subps	xmm0, kIntrPosMods
		movq	qword ptr [edx], xmm0
		retn
	}
}

struct WorldDimensions
{
	float		minX;		// 00
	float		minY;		// 04
	float		sizeX;		// 08
	float		sizeY;		// 0C
}
s_rootWorldDimensions;

__declspec(naked) void __fastcall GetWorldDimensions(TESWorldSpace *worldSpc)
{
	__asm
	{
		add		ecx, 0x80
		movsx	eax, word ptr [ecx+0xC]
		inc		eax
		movsx	edx, word ptr [ecx+8]
		sub		eax, edx
		shl		edx, 0xC
		movd	xmm0, edx
		cvtdq2ps	xmm0, xmm0
		shl		eax, 0xC
		movd	xmm1, eax
		cvtdq2ps	xmm1, xmm1
		mov		edx, 0x3F4C0000
		movd	xmm2, edx
		divss	xmm1, xmm2
		movsx	eax, word ptr [ecx+0xE]
		movsx	edx, word ptr [ecx+0xA]
		mov		ecx, offset s_rootWorldDimensions
		movss	[ecx], xmm0
		movss	[ecx+8], xmm1
		inc		edx
		sub		eax, edx
		shl		edx, 0xC
		movd	xmm0, edx
		cvtdq2ps	xmm0, xmm0
		shl		eax, 0xC
		movd	xmm1, eax
		cvtdq2ps	xmm1, xmm1
		divss	xmm1, xmm2
		movss	[ecx+4], xmm0
		movss	[ecx+0xC], xmm1
		retn
	}
}

struct WorldPosMods
{
	float		mapScale;	// 00
	float		mapCellX;	// 04
	float		mapCellY;	// 08
	float		posModX;	// 0C
	float		posModY;	// 10
}
s_currWorldPosMods;

__declspec(naked) void __fastcall GetWorldPosMods(TESWorldSpace *worldSpc)
{
	__asm
	{
		add		ecx, 0x90
		mov		eax, offset s_currWorldPosMods
		movups	xmm0, [ecx]
		movups	[eax], xmm0
		cmp		dword ptr [ecx], 0x3F800000
		jz		done
		movss	xmm0, kFltHalf
		movss	xmm1, [ecx]
		mulss	xmm1, xmm0
		subss	xmm0, xmm1
		movss	xmm1, [ecx+0x10]
		addss	xmm1, [ecx+0x18]
		mulss	xmm1, xmm0
		addss	xmm1, [ecx+4]
		movss	[eax+0xC], xmm1
		movss	xmm1, [ecx+0x14]
		addss	xmm1, [ecx+0x1C]
		mulss	xmm1, xmm0
		addss	xmm1, [ecx+8]
		movss	[eax+0x10], xmm1
	done:
		retn
	}
}

__declspec(naked) void __fastcall AdjustWorldPos(TESObjectREFR *refr, NiPoint2 *outMults)
{
	__asm
	{
		movq	xmm0, qword ptr [ecx+0x30]
		mov		eax, offset s_currWorldPosMods
		mov		ecx, [eax]
		cmp		ecx, 0x3F800000
		jz		noScale
		movd	xmm1, ecx
		movsldup	xmm1, xmm1
		mulps	xmm0, xmm1
		movq	xmm1, qword ptr [eax+0xC]
		jmp		done
	noScale:
		movq	xmm1, qword ptr [eax+4]
	done:
		addps	xmm0, xmm1
		movq	qword ptr [edx], xmm0
		retn
	}
}

__declspec(naked) void __fastcall GetWorldMapPosMults(NiPoint2 *outMults)
{
	static const __m128 kPosMultMods = {0.1015625, 0.1015625, 0, 0};
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		mov		eax, offset s_rootWorldDimensions
		movq	xmm1, qword ptr [eax]
		subps	xmm0, xmm1
		movq	xmm1, qword ptr [eax+8]
		movlhps	xmm1, xmm1
		divps	xmm0, xmm1
		addps	xmm0, kPosMultMods
		movq	qword ptr [ecx], xmm0
		retn
	}
}

__declspec(naked) void __fastcall GetLocalMapPosMults(NiPoint2 *inPos, NiPoint2 *nwXY, NiPoint2 *outMults)
{
	static const __m128 kPosMultDivs = {12288, -12288, 1, 1};
	__asm
	{
		movq	xmm0, qword ptr [ecx]
		movq	xmm1, qword ptr [edx]
		subps	xmm0, xmm1
		divps	xmm0, kPosMultDivs
		mov		eax, [esp+4]
		movq	qword ptr [eax], xmm0
		retn	4
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

__declspec(naked) bool __fastcall GetBitSeen(SeenData *seenData, SInt16 colrow)
{
	__asm
	{
		xor		eax, eax
		mov		al, dh
		shr		al, 1
		mov		ecx, [ecx+eax*4+4]
		mov		al, dh
		and		al, 1
		shl		al, 4
		add		al, dl
		bt		ecx, eax
		setc	al
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

struct SectionSeenInfo
{
	union
	{
		CellPointerMap	*cellMap;
		IntSeenData		*seenData;
	};
	Coordinate			cellCoord;
	UInt8				row;
	UInt8				col;
};

__declspec(naked) UInt32 __fastcall GetSectionSeenLevel(SectionSeenInfo *seenInfo)
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
		mov		bl, [esi+8]
		cmp		bl, 0xF
		setz	al
		shl		al, 1
		mov		bh, [esi+9]
		cmp		bh, 0xF
		setz	dl
		or		al, dl
		mov		[ebp-3], al
		mov		ecx, [esi]
		cmp		byte ptr [ecx], 0xE4
		jnz		isExterior
		mov		dl, [esi+6]
		mov		dh, [esi+4]
		call	GetSectionSeenData
		mov		edi, eax
		test	eax, eax
		jz		noData0Int
		mov		edx, ebx
		mov		ecx, eax
		call	GetBitSeen
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
		mov		edx, ebx
		inc		dl
		call	GetBitSeen
		add		[ebp-4], al
	noData1Int:
		mov		dl, [esi+6]
		mov		dh, [esi+4]
		inc		dh
		mov		ecx, [esi]
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		jmp		case1Both
		ALIGN 16
	case2Int:
		test	ecx, ecx
		jz		noData2Int
		mov		edx, ebx
		inc		dh
		call	GetBitSeen
		add		[ebp-4], al
	noData2Int:
		mov		dl, [esi+6]
		inc		dl
		mov		dh, [esi+4]
		mov		ecx, [esi]
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		jmp		case2Both
		ALIGN 16
	case3Int:
		mov		dl, [esi+6]
		mov		dh, [esi+4]
		inc		dh
		mov		ecx, [esi]
		call	GetSectionSeenData
		test	eax, eax
		jz		noData3Int
		movzx	edx, bl
		mov		ecx, eax
		call	GetBitSeen
		add		[ebp-4], al
	noData3Int:
		mov		dl, [esi+6]
		inc		dl
		mov		dh, [esi+4]
		mov		ecx, [esi]
		call	GetSectionSeenData
		test	eax, eax
		jz		noData4Int
		mov		edx, ebx
		xor		dl, dl
		mov		ecx, eax
		call	GetBitSeen
		add		[ebp-4], al
	noData4Int:
		mov		dl, [esi+6]
		inc		dl
		mov		dh, [esi+4]
		inc		dh
		mov		ecx, [esi]
		call	GetSectionSeenData
		test	eax, eax
		jz		done
		xor		edx, edx
		mov		ecx, eax
		call	GetBitSeen
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	isExterior:
		push	dword ptr [esi+4]
		call	CellPointerMap::Lookup
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
		mov		ecx, eax
		call	GetBitSeen
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
		mov		edx, ebx
		inc		dh
		call	GetBitSeen
		add		[ebp-4], al
		mov		edx, ebx
		inc		dl
		mov		ecx, edi
		call	GetBitSeen
		add		[ebp-4], al
		mov		edx, ebx
		inc		dl
		inc		dh
		mov		ecx, edi
		call	GetBitSeen
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	case1Ext:
		test	al, al
		jnz		modRes1
		test	ecx, ecx
		jz		getCell2
		mov		edx, ebx
		inc		dl
		call	GetBitSeen
	modRes1:
		add		[ebp-4], al
	getCell2:
		mov		eax, [esi+4]
		inc		ax
		push	eax
		mov		ecx, [esi]
		call	CellPointerMap::Lookup
		test	eax, eax
		jz		done
		test	byte ptr [eax+0x25], 1
		jz		getData1
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
		movzx	edx, bl
		mov		ecx, eax
		call	GetBitSeen
		add		[ebp-4], al
		movzx	edx, bl
		inc		dl
		mov		ecx, edi
		call	GetBitSeen
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	case2Ext:
		test	al, al
		jnz		modRes2
		test	ecx, ecx
		jz		getCell3
		mov		edx, ebx
		inc		dh
		call	GetBitSeen
	modRes2:
		add		[ebp-4], al
	getCell3:
		mov		eax, [esi+4]
		add		eax, 0x10000
		push	eax
		mov		ecx, [esi]
		call	CellPointerMap::Lookup
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
		xor		dl, dl
		mov		ecx, eax
		call	GetBitSeen
		add		[ebp-4], al
		mov		edx, ebx
		xor		dl, dl
		inc		dh
		mov		ecx, edi
		call	GetBitSeen
		add		[ebp-4], al
		jmp		done
		ALIGN 16
	case3Ext:
		mov		eax, [esi+4]
		inc		ax
		push	eax
		mov		ecx, [esi]
		call	CellPointerMap::Lookup
		test	eax, eax
		jz		getCell4
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		jnz		modRes3
		call	GetSeenData
		test	eax, eax
		jz		getCell4
		movzx	edx, bl
		mov		ecx, eax
		call	GetBitSeen
	modRes3:
		add		[ebp-4], al
	getCell4:
		mov		eax, [esi+4]
		add		eax, 0x10000
		push	eax
		mov		ecx, [esi]
		call	CellPointerMap::Lookup
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
		xor		dl, dl
		mov		ecx, eax
		call	GetBitSeen
	modRes4:
		add		[ebp-4], al
	getCell5:
		mov		eax, [esi+4]
		inc		ax
		add		eax, 0x10000
		push	eax
		mov		ecx, [esi]
		call	CellPointerMap::Lookup
		test	eax, eax
		jz		done
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		jnz		modRes5
		call	GetSeenData
		test	eax, eax
		jz		done
		xor		edx, edx
		mov		ecx, eax
		call	GetBitSeen
	modRes5:
		add		[ebp-4], al
	done:
		movzx	eax, byte ptr [ebp-4]
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn
	}
}

Coordinate s_packedCellCoords[9];
float s_vertexAlphaLevel[] = {0, 0.25, 0.5, 0.75, 1};

__declspec(naked) void __stdcall CalcVtxAlphaBySeenData(UInt32 gridIdx)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x10
		push	ebx
		push	esi
		push	edi
		lea		edi, [ebp-0xC]
		mov		ecx, s_pcCurrCell
		test	byte ptr [ecx+0x24], 1
		jz		isExterior
		call	GetSeenData
		test	eax, eax
		jz		done
		jmp		proceed
	isExterior:
		mov		eax, [ecx+0xC0]
		mov		eax, [eax+0x30]
	proceed:
		mov		[edi], eax
		mov		eax, [ebp+8]
		mov		ecx, s_localMapShapeDatas[eax*4]
		mov		[ebp-0x10], ecx
		mov		esi, [ecx+0x28]
		add		esi, 0xC
		mov		edx, s_packedCellCoords[eax*4]
		mov		[edi+4], edx
		xor		bl, bl
		xor		al, al
		ALIGN 16
	iter1Head:
		mov		[edi+8], al
		mov		bh, 0x10
		mov		[edi+9], 0
		inc		word ptr [edi+4]
		ALIGN 16
	iter2Head:
		mov		ecx, edi
		call	GetSectionSeenLevel
		mov		ecx, s_vertexAlphaLevel[eax*4]
		mov		[esi], ecx
		add		esi, 0x10
		movzx	eax, bh
		shr		eax, 4
		sub		[edi+4], ax
		dec		bh
		mov		[edi+9], bh
		jns		iter2Head
		inc		bl
		cmp		bl, 0x10
		ja		iter1End
		mov		al, bl
		jb		iter1Head
		xor		al, al
		inc		word ptr [edi+6]
		jmp		iter1Head
	iter1End:
		mov		ecx, [ebp-0x10]
		mov		ecx, [ecx+0x34]
		test	ecx, ecx
		jz		done
		mov		dword ptr [ecx+0x20], 0
	done:
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn	4
	}
}

__declspec(naked) UInt32 __fastcall GetFOWUpdateMask(SInt32 posX, SInt32 posY)
{
	static const UInt32 kFOWUpdateMask[] = {0x1B, 0x12, 0x36, 0x18, 0x10, 0x30, 0xD8, 0x90, 0x1B0};
	__asm
	{
		push	esi
		mov		esi, ecx
		and		esi, 0xFFF
		xor		ecx, ecx
		cmp		esi, 0x500
		setnb	cl
		cmp		esi, 0xB00
		seta	al
		add		cl, al
		mov		esi, edx
		and		esi, 0xFFF
		xor		edx, edx
		cmp		esi, 0x500
		setnb	dl
		cmp		esi, 0xB00
		seta	al
		add		dl, al
		lea		esi, [ecx+ecx*2]
		add		edx, esi
		mov		eax, kFOWUpdateMask[edx*4]
		pop		esi
		retn
	}
}

__declspec(naked) void __fastcall DoSelectiveFOWUpdate(NiPoint2 *adjustedPos)
{
	__asm
	{
		push	esi
		push	edi
		mov		eax, ecx
		cvttss2si	edx, [eax+4]
		cvttss2si	ecx, [eax]
		call	GetFOWUpdateMask
		mov		esi, eax
		xor		edi, edi
		ALIGN 16
	iterHead:
		test	esi, 1
		jz		iterNext
		push	edi
		call	CalcVtxAlphaBySeenData
		ALIGN 16
	iterNext:
		inc		edi
		shr		esi, 1
		jnz		iterHead
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
		mov		ebp, esp
		push	ecx
		push	0
		push	ebx
		push	esi
		push	edi
		lea		esi, [ecx+4]
		mov		edi, edx
		xor		ecx, ecx
		mov		bl, 0xF
		ALIGN 16
	iter1Head:
		movzx	edx, bl
		add		edx, [edi]
		imul	edx, edx
		cmp		edx, 0x10
		ja		iter1Next
		mov		[ebp-4], edx
		mov		bh, 0xF
		ALIGN 16
	iter2Head:
		movzx	edx, bh
		add		edx, [edi+4]
		imul	edx, edx
		add		edx, [ebp-4]
		cmp		edx, 0x10
		ja		iter2Next
		mov		cl, bh
		shr		cl, 1
		lea		eax, [esi+ecx*4]
		mov		cl, bh
		and		cl, 1
		shl		cl, 4
		add		cl, bl
		mov		edx, [eax]
		bt		edx, ecx
		jc		iter2Next
		mov		edx, 1
		shl		edx, cl
		or		[eax], edx
		mov		[ebp-8], 1
	iter2Next:
		dec		bh
		jns		iter2Head
	iter1Next:
		dec		bl
		jns		iter1Head
		mov		dl, [ebp-8]
		or		s_updateFogOfWar, dl
		mov		edx, 0xFFFFFFFF
		cmp		[esi], edx
		jnz		done
		cmp		[esi+4], edx
		jnz		done
		cmp		[esi+8], edx
		jnz		done
		cmp		[esi+0xC], edx
		jnz		done
		cmp		[esi+0x10], edx
		jnz		done
		cmp		[esi+0x14], edx
		jnz		done
		cmp		[esi+0x18], edx
		jnz		done
		cmp		[esi+0x1C], edx
	done:
		setz	al
		lea		ecx, [esi-4]
		pop		edi
		pop		esi
		pop		ebx
		leave
		retn
	}
}

__declspec(naked) SeenData* __fastcall AddExtraSeenData(TESObjectCELL *cell)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		test	byte ptr [ecx+0x24], 1
		jnz		isInterior
		push	0x24
		GAME_HEAP_ALLOC
		mov		dword ptr [eax], kVtbl_SeenData
		jmp		doneData
	isInterior:
		push	0x2C
		GAME_HEAP_ALLOC
		mov		dword ptr [eax], kVtbl_IntSeenData
		xor		edx, edx
		mov		[eax+0x24], edx
		mov		[eax+0x28], edx
	doneData:
		pxor	xmm0, xmm0
		movups	[eax+4], xmm0
		movups	[eax+0x14], xmm0
		push	eax
		push	0x10
		GAME_HEAP_ALLOC
		mov		dword ptr [eax], kVtbl_ExtraSeenData
		mov		[eax+4], 5
		mov		dword ptr [eax+8], 0
		mov		ecx, [esp]
		mov		[eax+0xC], ecx
		push	eax
		lea		ecx, [esi+0x28]
		CALL_EAX(kAddr_AddExtraData)
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
		pxor	xmm0, xmm0
		movups	[eax+4], xmm0
		movups	[eax+0x14], xmm0
		mov		[eax+0x24], dx
		xor		edx, edx
		mov		[eax+0x26], dx
		mov		[eax+0x28], edx
		pop		ecx
		mov		[ecx+0x28], eax
	found:
		retn
	}
}

__declspec(naked) void UpdateCellsSeenBitsHook()
{
	static const UInt32 kFlt1d80 = 0x3C4CCCCD;
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
		movq	xmm0, qword ptr [ebx+0x30]
		mov		edi, 0x11CA208
		cmp		s_pcCurrCell0, esi
		jz		sameCell
		mov		s_pcCurrCell0, esi
		mov		ecx, esi
		call	GetNorthRotation
		jmp		skipPosDiff
	sameCell:
		movq	xmm1, qword ptr [edi]
		subps	xmm1, xmm0
		mulps	xmm1, xmm1
		movss	xmm2, xmm1
		psrlq	xmm1, 0x20
		addss	xmm1, xmm2
		rsqrtss	xmm1, xmm1
		comiss	xmm1, kFlt1d80
		ja		doneExt
	skipPosDiff:
		movq	qword ptr [edi], xmm0
		test	byte ptr [esi+0x24], 1
		jnz		isInterior
		mov		eax, [esi+0xC0]
		test	eax, eax
		jz		doneExt
		mov		eax, [eax+0x30]
		test	eax, eax
		jz		doneExt
		mov		[ebp-0x10], eax
		cvtps2dq	xmm0, xmm0
		movq	qword ptr [ebp-8], xmm0
		mov		edx, [ebp-4]
		mov		ecx, [ebp-8]
		call	GetFOWUpdateMask
		mov		edi, eax
		mov		eax, [esi+0x48]
		mov		ecx, [eax]
		dec		ecx
		mov		[ebp-0xA], cx
		mov		edx, [eax+4]
		dec		edx
		mov		[ebp-0xC], dx
		shl		ecx, 0xC
		sub		ecx, [ebp-8]
		sar		ecx, 8
		mov		[ebp-8], ecx
		shl		edx, 0xC
		sub		edx, [ebp-4]
		sar		edx, 8
		mov		[ebp-4], edx
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
		call	CellPointerMap::Lookup
		test	eax, eax
		jz		iterNextExt
		test	byte ptr [eax+0x25], 1
		jnz		iterNextExt
		mov		esi, eax
		mov		ecx, eax
		call	GetSeenData
		test	eax, eax
		jnz		hasDataExt
		mov		ecx, esi
		call	AddExtraSeenData
	hasDataExt:
		lea		edx, [ebp-8]
		mov		ecx, eax
		call	UpdateSeenBits
		test	al, al
		jz		iterNextExt
		push	kExtraData_SeenData
		lea		ecx, [esi+0x28]
		CALL_EAX(kAddr_RemoveExtraType)
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
		ALIGN 16
	isInterior:
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		unpcklps	xmm0, xmm0
		mulps	xmm0, s_northRotationMods
		pxor	xmm1, xmm1
		movhlps	xmm1, xmm0
		addps	xmm0, xmm1
	noRotation:
		cvtps2dq	xmm0, xmm0
		movq	qword ptr [ebp-8], xmm0
		mov		ecx, [ebp-8]
		sub		ecx, 0x800
		mov		eax, ecx
		sar		eax, 0xC
		dec		eax
		mov		bl, al
		shl		eax, 0xC
		sub		eax, ecx
		sar		eax, 8
		mov		[ebp-8], eax
		mov		edx, [ebp-4]
		sub		edx, 0x800
		mov		eax, edx
		sar		eax, 0xC
		dec		eax
		mov		bh, al
		shl		eax, 0xC
		sub		eax, edx
		sar		eax, 8
		mov		[ebp-4], eax
		mov		[ebp-0xC], bx
		call	GetFOWUpdateMask
		mov		edi, eax
		mov		ecx, esi
		call	GetSeenData
		test	eax, eax
		jnz		hasDataInt
		mov		ecx, esi
		call	AddExtraSeenData
		mov		cx, [ebp-0xC]
		mov		[eax+0x24], cx
	hasDataInt:
		mov		esi, eax
		xor		ebx, ebx
		ALIGN 16
	iterHeadInt:
		movzx	ecx, bl
		lea		eax, [ecx+ecx*2]
		add		al, bh
		bt		edi, eax
		jnc		iterNextInt
		mov		dx, [ebp-0xC]
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
	}
}

struct DoorRef
{
	TESObjectREFR	*doorRef;
	TESObjectCELL	*linkedCell;

	DoorRef(TESObjectREFR *_doorRef, TESObjectCELL *_linkedCell) : doorRef(_doorRef)
	{
		linkedCell = (_linkedCell && !_linkedCell->worldSpace) ? _linkedCell : NULL;
	}
};

typedef Vector<DoorRef> DoorRefsList;

void __fastcall GetTeleportDoors(TESObjectCELL *cell, DoorRefsList *doorRefsList)
{
	cell->RefLockEnter();
	auto refsIter = cell->objectList.Head();
	TESObjectREFR *refr;
	ExtraTeleport *xTeleport;
	do
	{
		if (!(refr = refsIter->data) || (refr->flags & 0x860) || NOT_ID(refr->baseForm, TESObjectDOOR))
			continue;
		xTeleport = GetExtraType(&refr->extraDataList, Teleport);
		if (xTeleport && xTeleport->data && xTeleport->data->linkedDoor)
			doorRefsList->Append(refr, xTeleport->data->linkedDoor->parentCell);
	}
	while (refsIter = refsIter->next);
	cell->RefLockLeave();
}

__declspec(naked) float* __fastcall GetVtxAlphaPtr(NiPoint2 *posMult)
{
	static const float kFlt48 = 48.0F;
	__asm
	{
		push	ebx
		movss	xmm0, kFlt48
		movss	xmm1, [ecx]
		mulss	xmm1, xmm0
		cvttss2si	eax, xmm1
		mulss	xmm0, [ecx+4]
		cvttss2si	ecx, xmm0
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
	}
}

UInt8 s_useAltFormat = 0;

__declspec(naked) void __fastcall GenerateRenderedTextureHook(TESObjectCELL *cell, int EDX, NiCamera *camera, RenderTarget **outTexture)
{
	__asm
	{
		cmp		s_useAltFormat, 2
		jnb		altFormat
		JMP_EAX(0x54E830)
	altFormat:
		push	ebp
		mov		ebp, esp
		push	ecx
		sub		esp, 0xE8
		test	ecx, ecx
		jz		noCell_0
		test	byte ptr [ecx+0x24], 1
		setnz	cl
	noCell_0:
		mov		[ebp-0x1D], cl
		mov		eax, fs:[0x2C]
		mov		ecx, ds:[0x126FD98]
		mov		edx, [eax+ecx*4]
		add		edx, 0x2B4
		mov		eax, [edx]
		mov		[ebp-0x10], eax
		mov		dword ptr [edx], 0xE
		push	0
		lea		ecx, [ebp-0xEC]
		CALL_EAX(0x4A0EB0)
		push	0
		push	0
		push	0
		push	0x1E
		push	0
		push	0x16
		push	0
		cmp		dword ptr [ebp-4], 0
		mov		edx, s_texturePixelSize
		cmovz	edx, s_projectPixelSize
		push	edx
		push	edx
		mov		eax, ds:[0x11F9508]
		mov		[ebp-0x14], eax
		push	eax
		mov		ecx, ds:[0x11F91A8]
		CALL_EAX(0xB6D5E0)
		test	eax, eax
		jz		done
		mov		[ebp-0xC], eax
		mov		edx, [ebp+0xC]
		mov		[edx], eax
		mov		ecx, [ebp-0x14]
		mov		eax, [ecx+0x5E0]
		mov		[ebp-0x18], eax
		cmp		dword ptr [ebp-4], 0
		jz		noCell_1
		movzx	eax, byte ptr [ebp-0x1D]
		dec		eax
		and		eax, 0x1F2F3F
		mov		[ecx+0x5E0], eax
	noCell_1:
		mov		ecx, g_shadowSceneNode
		mov		[ebp-8], ecx
		mov		dl, [ecx+0x130]
		mov		[ebp-0x1E], dl
		mov		[ecx+0x130], 1
		mov		eax, ds:[0x11D5C48]
		mov		[ebp-0x1C], eax
		mov		dl, [eax+0x18]
		mov		[ebp-0x1F], dl
		mov		[eax+0x18], 0
		mov		ecx, [ebp-0xC]
		mov		eax, ds:[0x11F4748]
		cmp		dword ptr [eax+0x200], 0
		setz	al
		mov		[ebp-0x20], al
		jnz		bgnScnAlt
		push	dword ptr [ecx+8]
		push	7
		mov		eax, 0xB6B8D0
		jmp		doneBgnScn
	bgnScnAlt:
		push	7
		push	ecx
		mov		eax, 0x54EDE0
	doneBgnScn:
		call	eax
		add		esp, 8
		push	0x280
		CALL_EAX(0xAA13E0)
		pop		ecx
		push	0x2F7
		push	1
		push	0x63
		mov		ecx, eax
		CALL_EAX(0xB660D0)
		lock inc dword ptr [eax+4]
		mov		[ebp-0x28], eax
		mov		edx, [ebp-8]
		mov		[eax+0x194], edx
		movzx	edx, byte ptr [ebp-0x1D]
		neg		edx
		and		edx, 0xC
		mov		[eax+0x19C], edx
		mov		eax, [ebp-0x30]
		mov		ecx, [ebp-0x5C]
		mov		[ebp+eax*4-0x58], ecx
		inc		dword ptr [ebp-0x30]
		mov		dword ptr [ebp-0x5C], 3
		lea		edx, [ebp-0xEC]
		push	edx
		push	dword ptr [ebp-8]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
		mov		ecx, [ebp-4]
		test	ecx, ecx
		jz		skipCull
		mov		edx, 6
		call	TESObjectCELL::Get3DNode
		test	eax, eax
		jz		skipCull
		mov		dword ptr [ebp-0x5C], 1
		lea		edx, [ebp-0xEC]
		push	edx
		push	eax
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
	skipCull:
		mov		eax, [ebp-0x30]
		dec		eax
		mov		[ebp-0x30], eax
		mov		ecx, [ebp+eax*4-0x58]
		mov		[ebp-0x5C], ecx
		push	dword ptr [ebp-0x28]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6C0D0)
		add		esp, 8
		mov		ecx, [ebp-0x28]
		call	NiReleaseObject
		mov		eax, 0xB6B790
		mov		ecx, 0xB6B840
		cmp		[ebp-0x20], 0
		cmovz	eax, ecx
		call	eax
		mov		eax, [ebp-0x1C]
		mov		dl, [ebp-0x1F]
		mov		[eax+0x18], dl
		mov		al, [ebp-0x1E]
		mov		ecx, [ebp-8]
		mov		[ecx+0x130], al
		mov		eax, ds:[0x11F95EC]
		mov		[eax+0x86], 1
		mov		ecx, [ebp-0x14]
		mov		eax, [ebp-0x18]
		mov		[ecx+0x5E0], eax
	done:
		mov		eax, fs:[0x2C]
		mov		ecx, ds:[0x126FD98]
		mov		edx, [eax+ecx*4]
		mov		eax, [ebp-0x10]
		mov		[edx+0x2B4], eax
		leave
		retn	8
	}
}

__declspec(naked) void D3DFormatHook()
{
	__asm
	{
		cmp		s_useAltFormat, 0
		mov		eax, 0x16
		mov		ecx, 0x17
		cmovnz	ecx, eax
		mov		edx, 0x80
		cmovnz	edx, s_texturePixelSize
		mov		eax, [esp+0x2C]
		mov		[eax], ecx
		mov		[eax+8], edx
		mov		[eax+0xC], edx
		xor		ecx, ecx
		mov		[ebx], ecx
		mov		[esi], ecx
		pop		edi
		pop		esi
		pop		ebp
		pop		ebx
		add		esp, 8
		retn	0x20
	}
}

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
		mov		edx, [eax]
		shl		edx, 0xC
		add		edx, 0x800
		mov		[ebp-8], edx
		mov		edx, [eax+4]
		shl		edx, 0xC
		add		edx, 0x800
		mov		[ebp-4], edx
		movq	xmm0, qword ptr [ebp-8]
		cvtdq2ps	xmm0, xmm0
		mov		eax, [ecx+0xA0]
		mov		ecx, [eax]
		mov		[ebp-4], ecx
		or		byte ptr [ecx+0x30], 1
		mov		ecx, [eax+4]
		mov		[ebp-8], ecx
		or		byte ptr [ecx+0x30], 1
		pxor	xmm1, xmm1
		movups	[ebp-0x18], xmm1
		mov		ecx, [eax+8]
		mov		eax, [ecx+0xA0]
		movzx	edx, word ptr [ecx+0xA6]
		ALIGN 16
	iterHead:
		dec		dl
		js		iterEnd
		mov		ecx, [eax+edx*4]
		test	ecx, ecx
		jz		iterHead
		mov		ecx, [ecx+0xA0]
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		iterHead
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
		mov		eax, ds:[0x11CCB78]
		mov		eax, [eax+4]
		mov		[ebp-0x1C], eax
		or		byte ptr [eax+0x30], 1
		mov		eax, ds:[0x11DEDA4]
		mov		[ebp-0x20], eax
		or		byte ptr [eax+0x30], 1
		mov		eax, ds:[0x11CA438]
		mov		[ebp-0x24], eax
		test	eax, eax
		jz		noGrass0
		or		byte ptr [eax+0x30], 1
	noGrass0:
		mov		byte ptr ds:[0x11AD7B4], 0
		mov		eax, ds:[0x11D5C48]
		mov		[ebp-0x28], eax
		mov		byte ptr [eax+0x1B], 1
		push	0
		push	esp
		mov		ecx, s_localMapCamera
		movq	qword ptr [ecx+0x58], xmm0
		push	ecx
		call	NiAVObject::Update
		mov		ecx, [ebp+8]
		call	GenerateRenderedTextureHook
		mov		byte ptr ds:[0x11AD7B4], 1
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
		xor		edx, edx
		mov		eax, [ebp-0x28]
		mov		[eax+0x1B], dl
		mov		al, 0xBF
		mov		ecx, [ebp-0x18]
		test	ecx, ecx
		jz		noBlock0
		and		[ecx+0x25], al
		mov		[ecx+0x38], edx
	noBlock0:
		mov		ecx, [ebp-0x14]
		test	ecx, ecx
		jz		noBlock1
		and		[ecx+0x25], al
		mov		[ecx+0x38], edx
	noBlock1:
		mov		ecx, [ebp-0x10]
		test	ecx, ecx
		jz		noBlock2
		and		[ecx+0x25], al
		mov		[ecx+0x38], edx
	noBlock2:
		mov		ecx, [ebp-0xC]
		test	ecx, ecx
		jz		noBlock3
		and		[ecx+0x25], al
		mov		[ecx+0x38], edx
	noBlock3:
		mov		ecx, [ebp-0x2C]
		test	ecx, ecx
		jz		done
		mov		eax, [ecx+0x30]
		test	eax, eax
		jz		doRelease
		push	eax
		push	dword ptr [ebp+0xC]
		call	NiReleaseAddRef
		mov		ecx, [ebp-0x2C]
	doRelease:
		call	NiReleaseObject
	done:
		leave
		retn	8
	}
}

__declspec(naked) void __stdcall GenerateLocalMapInterior(TESObjectCELL *cell, Coordinate coord, NiRenderedTexture **renderedTexture)
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
		sub		esp, 8
		movsx	edx, word ptr [ebp+0xE]
		shl		edx, 0xC
		add		edx, 0x1000
		mov		[ebp-8], edx
		movsx	edx, word ptr [ebp+0xC]
		shl		edx, 0xC
		add		edx, 0x1000
		mov		[ebp-4], edx
		movq	xmm0, qword ptr [ebp-8]
		cvtdq2ps	xmm0, xmm0
		cmp		s_cellNorthRotation, 0
		jz		noRot
		unpcklps	xmm0, xmm0
		pshufd	xmm1, s_northRotationMods, 0xD8
		mulps	xmm0, xmm1
		movhlps	xmm1, xmm0
		addps	xmm0, xmm1
	noRot:
		mov		eax, [ecx+0xA0]
		mov		ecx, [eax]
		mov		[ebp-4], ecx
		or		byte ptr [ecx+0x30], 1
		mov		ecx, [eax+4]
		mov		[ebp-8], ecx
		or		byte ptr [ecx+0x30], 1
		mov		byte ptr ds:[0x11AD7B4], 0
		push	0
		push	esp
		mov		ecx, s_localMapCamera
		movq	qword ptr [ecx+0x58], xmm0
		push	ecx
		call	NiAVObject::Update
		mov		ecx, [ebp+8]
		call	GenerateRenderedTextureHook
		mov		byte ptr ds:[0x11AD7B4], 1
		mov		ecx, [ebp-4]
		and		byte ptr [ecx+0x30], 0xFE
		mov		ecx, [ebp-8]
		and		byte ptr [ecx+0x30], 0xFE
		mov		ecx, [ebp-0xC]
		test	ecx, ecx
		jz		done
		mov		eax, [ecx+0x30]
		test	eax, eax
		jz		doRelease
		push	eax
		push	dword ptr [ebp+0x10]
		call	NiReleaseAddRef
		mov		ecx, [ebp-0xC]
	doRelease:
		call	NiReleaseObject
	done:
		leave
		retn	0xC
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

Vector<MapMarkerTile> s_mapMarkerTiles(0x40);

MapMarkerTile::MapMarkerTile(Tile *markerTile)
{
	visible = markerTile->GetValue(kTileValue_visible);
	x = markerTile->GetValue(kTileValue_user0);
	y = markerTile->GetValue(kTileValue_user1);
	filename = ThisCall<Tile::Value*>(0xA01000, markerTile, kTileValue_filename);
	markerTile->SetFloat(kTileValue_depth, int((s_mapMarkerTiles.Size() % 10) + 18));
	inUse = true;
}

typedef Vector<MapMarkerTile*> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles> RenderedMapMarkers;
RenderedMapMarkers s_renderedMapMarkers(0x40);

void __fastcall FreeCellMapMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
		dtlIter->SetInUse(false);
	cmkIter.Remove();
}

bool s_discoveredLocation = false;

void __fastcall HandleDiscoverLocation(TESObjectCELL *markerCell)
{
	auto findRendered = s_renderedMapMarkers.Find(Coordinate(&markerCell->coords.exterior->x));
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
		push	dword ptr ds:[0x11D0368]
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
Tile::Value *s_miniMapMode, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom/*, *s_parentCellName*/;
TileShaderProperty *s_tileShaderProps[9];
bool s_defaultGridSize;
BSFogProperty *g_shadowFogProperty;
BSParticleSystemManager *g_particleSysMngr;
TESForm *g_blackPlaneBase;

struct MapMarkerInfo
{
	TESObjectREFR				*refr;
	ExtraMapMarker::MarkerData	*data;
	NiPoint2					pos;

	MapMarkerInfo(TESObjectREFR *_refr, ExtraMapMarker::MarkerData *_data, NiPoint2 &posXY) :
		refr(_refr), data(_data), pos(posXY) {}
};
typedef Vector<MapMarkerInfo> CellMapMarkers;
typedef UnorderedMap<UInt32, CellMapMarkers> WorldMapMarkers;
UnorderedMap<TESWorldSpace*, WorldMapMarkers> s_worldMapMarkers;

bool Cmd_InitMiniMap_Execute(COMMAND_ARGS)
{
	if (s_miniMapIndex) return true;

	UInt8 modIndex = g_dataHandler->GetModIndex("JIP MiniMap.esp");
	if ((modIndex == 0xFF) || (modIndex != scriptObj->modIndex))
		return true;

	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPMiniMap");
	if (!tile) return true;

	if (tile->GetValueFloat(kTileValue_user0) < 1.4F)
	{
		ShowMessageBox("Please be advised:\n\nJIP MiniMap versions older than 1.4 will NOT work with JIP LN NVSE.\n\nYou MUST update JIP MiniMap to the latest version.", MSGBOX_ARGS);
		return true;
	}
	s_miniMapIndex = modIndex;

	s_miniMapScale = tile->GetValue(kTileValue_user1);
	//s_parentCellName = tile->children.Head()->data->GetValue(kTileValue_string);
	auto node = tile->children.Tail();
	node->data->SetFloat(kTileValue_depth, 39);
	node = node->prev;
	node->data->SetFloat(kTileValue_depth, 28);
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

	NiVector3 *shapeVertices = (NiVector3*)GameHeapAlloc(sizeof(NiVector3) * 289);
	NiVector3 *shapeNormals = (NiVector3*)GameHeapAlloc(sizeof(NiVector3) * 289);
	MemZero(shapeNormals, sizeof(NiVector3) * 289);
	UVCoord *shapeUVCoords = (UVCoord*)GameHeapAlloc(sizeof(UVCoord) * 289);
	NiVector3 vertex(0, 0, 0);
	UVCoord uvCoord(0, 0);
	UInt32 index = 17, iterZ = 17;
	while (true)
	{
		*shapeVertices++ = vertex;
		*shapeUVCoords++ = uvCoord;
		shapeNormals++->y = 1;
		if (--iterZ)
		{
			vertex.z -= 8;
			uvCoord.y += 0.0625F;
			continue;
		}
		if (!--index) break;
		vertex.x += 8;
		vertex.z = 0;
		uvCoord.x += 0.0625F;
		uvCoord.y = 0;
		iterZ = 17;
	}
	shapeVertices -= 289;
	shapeUVCoords -= 289;
	shapeNormals -= 289;

	NiTriangle *shapeTriangles = (NiTriangle*)GameHeapAlloc(sizeof(NiTriangle) * 512);
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
	shapeTriangles -= 512;

	NiAlphaProperty *alphaProp = CdeclCall<NiAlphaProperty*>(0xA5CEB0);
	alphaProp->flags = 0x8D;

	TileImage *localTile;
	BSScissorTriShape *sciTriShp;
	NiTriShapeData *shapeData;
	index = 0;
	do
	{
		localTile = (TileImage*)node->data;
		node = node->prev;
		sciTriShp = (BSScissorTriShape*)localTile->node->m_children.data[0];
		s_localMapShapes[index] = sciTriShp;
		shapeData = (NiTriShapeData*)sciTriShp->geometryData;
		s_localMapShapeDatas[index] = shapeData;
		s_tileShaderProps[index] = localTile->shaderProp;
		NiReleaseAddRef((NiRefObject**)&localTile->shaderProp->srcTexture, NULL);

		shapeData->numVertices = 289;
		shapeData->numTriangles = 0x200;
		shapeData->trianglePoints = 0x600;

		GameHeapFree(shapeData->vertices);
		shapeData->vertices = shapeVertices;

		GameHeapFree(shapeData->normals);
		shapeData->normals = shapeNormals;

		GameHeapFree(shapeData->vertexColors);
		shapeData->vertexColors = (NiColorAlpha*)GameHeapAlloc(sizeof(NiColorAlpha) * 289);
		MemZero(shapeData->vertexColors, sizeof(NiColorAlpha) * 289);

		GameHeapFree(shapeData->uvCoords);
		shapeData->uvCoords = shapeUVCoords;

		GameHeapFree(shapeData->triangles);
		shapeData->triangles = shapeTriangles;

		NiReleaseAddRef((NiRefObject**)&sciTriShp->m_propertyList.Head()->data, alphaProp);
		NiReleaseAddRef((NiRefObject**)&sciTriShp->alphaProp, alphaProp);
	}
	while (++index < 9);
	UpdateTileScales();

	s_doorMarkersRect = (TileRect*)node->data;
	s_lQuestMarkersRect = (TileRect*)node->prev->data;
	node = s_worldMapRect->children.Tail();
	s_worldMapTile = (TileImage*)node->data;
	s_mapMarkersRect = (TileRect*)node->prev->data;
	s_wQuestMarkersRect = (TileRect*)node->prev->prev->data;

	NiCamera *lmCamera = ThisCall<NiCamera*>(0xA712F0, NiAllocator(sizeof(NiCamera)));
	s_localMapCamera = lmCamera;
	lmCamera->m_localRotate = {0, 0, 1.0F, 0, 1.0F, 0, -1.0F, 0, 0};
	lmCamera->m_localTranslate.z = 65536.0F;
	lmCamera->frustum.viewPort = {-2048.0F, 2048.0F, 2048.0F, -2048.0F};
	lmCamera->frustum.n = 100.0F;
	lmCamera->frustum.f = 131072.0F;
	lmCamera->frustum.o = 1;
	lmCamera->LODAdjust = 0.001F;
	g_shadowSceneNode->AddObject(lmCamera, 1);

	TESForm *markerBase = *(TESForm**)0x11CA224;
	auto worldIter = g_dataHandler->worldSpaceList.Head();
	TESWorldSpace *worldSpc, *rootWorld, *lastRoot = NULL;
	TESObjectCELL *rootCell;
	WorldMapMarkers *worldMarkers;
	TESObjectREFR *markerRef;
	ExtraMapMarker *xMarker;
	NiPoint2 posXY;
	Coordinate coord;
	do
	{
		if (!(worldSpc = worldIter->data) || !(rootCell = worldSpc->cell))
			continue;
		GetWorldPosMods(worldSpc);
		rootWorld = worldSpc->GetRootMapWorld();
		if (lastRoot != rootWorld)
		{
			lastRoot = rootWorld;
			GetWorldDimensions(rootWorld);
			worldMarkers = NULL;
		}
		rootCell->RefLockEnter();
		auto refrIter = rootCell->objectList.Head();
		do
		{
			if (!(markerRef = refrIter->data) || (markerRef->baseForm != markerBase))
				continue;
			xMarker = GetExtraType(&markerRef->extraDataList, MapMarker);
			if (!xMarker || !xMarker->data)
				continue;
			AdjustWorldPos(markerRef, &posXY);
			coord.x = ifloor(posXY.x / 4096.0F);
			coord.y = ifloor(posXY.y / 4096.0F);
			GetWorldMapPosMults(&posXY);
			if (!worldMarkers)
				worldMarkers = &s_worldMapMarkers[rootWorld];
			(*worldMarkers)[coord].Append(markerRef, xMarker->data, posXY);
		}
		while (refrIter = refrIter->next);
		rootCell->RefLockLeave();
	}
	while (worldIter = worldIter->next);

	s_defaultGridSize = *(UInt8*)0x11C63D0 <= 5;
	g_shadowFogProperty = *(BSFogProperty**)0x11DEB00;
	g_particleSysMngr = *(BSParticleSystemManager**)0x11DED58;
	g_blackPlaneBase = LookupFormByRefID(0x15A1F2);
	SafeWrite16(0x452736, 0x7705);
	SAFE_WRITE_BUF(0x87A12A, "\x31\xD2\x66\x89\x50\x26\x89\x50\x28\x90");
	SafeWrite8(0x555C20, 0xC3);
	WriteRelCall(0x9438F6, (UInt32)UpdateCellsSeenBitsHook);
	WriteRelCall(0x54F362, (UInt32)GenerateRenderedTextureHook);
	WriteRelCall(0x54FA46, (UInt32)GenerateRenderedTextureHook);
	SafeWrite32(0xB6D0C0, (UInt32)D3DFormatHook);
	WritePushRetRelJump(0x779567, 0x7795E8, (UInt32)DiscoverLocationHook);
	WritePushRetRelJump(0x60F13A, 0x60F145, (UInt32)SetQuestTargetsHook);
	WritePushRetRelJump(0xE7D13A, 0xE7D144, (UInt32)RendererRecreateHook);
	return true;
}

const __m128 kVertexAlphaMults = {0.25, 0.5, 0.75, 1};
alignas(16) const UInt32
kDirectionalLightValues[] = {0x3F59D9DA, 0x3F59D9DA, 0x3F59D9DA, 0x3E4CCCCD, 0x3E4CCCCD, 0x3E4CCCCD, 0, 0, 0, 0, 0xBFC90FDB, 0, 0},
kFogPropertyValues[] = {0x3DF8F8F9, 0x3E3CBCBD, 0x3E7CFCFD, 0x48F42400, 0x48F42400};
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

struct ExteriorEntry
{
	NiRenderedTexture	*texture;
	UInt8				regenFlags;

	ExteriorEntry() : texture(NULL), regenFlags(0) {}
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

Vector<DoorMarkerTile> s_doorMarkerTiles(0x40);

DoorMarkerTile::DoorMarkerTile(Tile *markerTile)
{
	visible = markerTile->GetValue(kTileValue_visible);
	x = markerTile->GetValue(kTileValue_user0);
	y = markerTile->GetValue(kTileValue_user1);
	visited = ThisCall<Tile::Value*>(0xA01000, markerTile, kTileValue_user2);
	markerTile->SetFloat(kTileValue_depth, int((s_doorMarkerTiles.Size() % 10) + 18));
}

struct QuestMarkerPos
{
	UInt32			pos;
	Tile::Value		*x;
	Tile::Value		*y;
};

UInt32 *g_lightingPasses = (UInt32*)0x11F91D8, s_currentMapMode = 0;
TESWorldSpace *s_pcCurrWorld = NULL;
Coordinate s_currLocalCoords(0x7FFF, 0x7FFF), s_currWorldCoords(0x7FFF, 0x7FFF);
const char **g_mapMarkerIcons = (const char**)0x11A0404;
Set<UInt32> s_currCellsSet(0xC), s_currMarkerCells(0x40);
UnorderedMap<UInt32, ExteriorEntry> s_renderedExteriors(0x80);
Vector<UInt32> s_exteriorKeys(0x60);
UnorderedMap<UInt32, NiRenderedTexture*> s_renderedInterior(0x20);
UnorderedMap<UInt32, DoorRefsList> s_exteriorDoorRefs(0x20);
DoorRefsList s_doorRefsList(0x40);
Vector<NiNode*> s_hiddenNodes(0x20);
TESQuest *s_activeQuest = NULL;
UnorderedMap<TESObjectREFR*, QuestMarkerPos> s_questMarkers(0x10);
TESObjectCELL *s_currCellGrid[9] = {};
UInt8 s_currCellQuad = 0;
bool s_useFogOfWar = false;
int s_markerRange = 0;

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

		/*if (showFlags & 0x20)
		{
			if (parentWorld)
			{
				char nameBuf[0x80], *endPtr;
				endPtr = StrLenCopy(nameBuf, parentWorld->fullName.name.m_data, parentWorld->fullName.name.m_dataLen);
				*(UInt16*)endPtr = '( ';
				endPtr += 2;
				endPtr = IntToStr(endPtr, parentCell->coords.exterior->x);
				*(UInt16*)endPtr = ' ,';
				endPtr += 2;
				endPtr = IntToStr(endPtr, parentCell->coords.exterior->y);
				*(UInt16*)endPtr = ')';
				s_parentCellName->SetString(nameBuf);
			}
			else s_parentCellName->SetString(parentCell->fullName.name.CStr());
		}*/
	}

	if (s_currentMapMode != updateType)
	{
		if (updateType >= 2)
			s_regenTextures = true;
		else
		{
			s_currentMapMode = updateType;
			s_miniMapMode->SetFloat((int)updateType);
		}
		s_updateQuestTargets = true;
		updateTiles = true;
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
				movups	ds:[s_vertexAlphaLevel+4], xmm0
			}
		}
	}

	if (worldMap)
	{
		if (parentWorld)
			objectRef = thePlayer;
		else
		{
			objectRef = thePlayer->lastExteriorDoor;
			if (!objectRef) return true;
			parentWorld = objectRef->GetParentWorld();
			if (!parentWorld) return true;
		}

		const char *ddsPath;
		TESWorldSpace *rootWorld = parentWorld->GetRootMapWorld();
		if (s_pcRootWorld != rootWorld)
		{
			s_pcRootWorld = rootWorld;
			GetWorldDimensions(rootWorld);
			ddsPath = rootWorld->worldMap.ddsPath.m_data;
			if (!ddsPath) ddsPath = "jazzisparis\\minimap\\blanktile.dds";
			s_worldMapTile->SetString(kTileValue_filename, ddsPath);
			s_worldMapRect->SetFloat(kTileValue_user1, rootWorld->mapData.usableDimensions.X);
			s_worldMapRect->SetFloat(kTileValue_user2, rootWorld->mapData.usableDimensions.Y);
			s_markerRange = iceil(60.0F / rootWorld->mapData.usableDimensions.X *
				(rootWorld->mapData.cellSECoordinates.X - rootWorld->mapData.cellNWCoordinates.X +
				rootWorld->mapData.cellNWCoordinates.Y - rootWorld->mapData.cellSECoordinates.Y));
			updateTiles = true;
			if (showFlags & 1)
			{
				for (auto tileIter = s_mapMarkerTiles.Begin(); tileIter; ++tileIter)
					if (tileIter().inUse) tileIter().SetInUse(false);
				s_renderedMapMarkers.Clear();
			}
		}

		if (s_pcCurrWorld != parentWorld)
		{
			s_pcCurrWorld = parentWorld;
			GetWorldPosMods(parentWorld);
		}

		AdjustWorldPos(objectRef, &posMult);
		coord.x = ifloor(posMult.x / 4096.0F);
		coord.y = ifloor(posMult.y / 4096.0F);
		GetWorldMapPosMults(&posMult);

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
				s_currMarkerCells.Clear();
				WorldMapMarkers *worldMarkers = s_worldMapMarkers.GetPtr(rootWorld);
				if (worldMarkers)
				{
					int range = iceil(s_markerRange / s_worldMapZoom->num);
					coord.x -= range;
					coord.y -= range;
					range = (range * 2) + 1;
					auto tileIter = s_mapMarkerTiles.Begin();
					CellMapMarkers *mapMarkers;
					DynamicTiles *dynamicTiles;
					MapMarkerTile *tileData;
					UInt32 iterX = range, iterY;
					do
					{
						iterY = range;
						do
						{
							if (!(mapMarkers = worldMarkers->GetPtr(coord)))
								goto iterYnext;
							s_currMarkerCells.Insert(coord);
							if (!s_renderedMapMarkers.Insert(coord, &dynamicTiles))
								goto iterYnext;
							for (auto mkIter = mapMarkers->Begin(); mkIter; ++mkIter)
							{
								if (!(mkIter().data->flags & 1) || (mkIter().refr->flags & 0x800))
									continue;
								while (true)
								{
									if (!tileIter)
									{
										tileData = s_mapMarkerTiles.Append(hudMain->AddTileFromTemplate(s_mapMarkersRect, "MiniMapWorldMarkerTemplate"));
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
								ddsPath = (mkIter().data->flags & 2) ? g_mapMarkerIcons[mkIter().data->type] : (const char*)0x1075030;
								tileData->filename->SetString(ddsPath);
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
				for (auto cmkIter = s_renderedMapMarkers.Begin(); cmkIter; ++cmkIter)
					if (!s_currMarkerCells.HasKey(cmkIter.Key())) FreeCellMapMarkers(cmkIter);
			}
		}
		else if (!s_renderedMapMarkers.Empty())
		{
			s_mapMarkerTiles.Clear();
			s_renderedMapMarkers.Clear();
			s_mapMarkersRect->DestroyAllChildren();
		}
	}
	else
	{
		TESObjectCELL *cell;
		UInt32 gridIdx, lightingPasses;
		NiNode *hideNode;
		NiPointLight *pntLight;
		bool showDoors = (showFlags & 2) != 0, updateFogOfWar = useFogOfWar && s_updateFogOfWar;
		s_updateFogOfWar = false;

		if (s_regenTextures)
		{
			s_regenTextures = false;
			if (!s_exteriorKeys.Empty())
			{
				for (auto prgIter = s_renderedExteriors.Begin(); prgIter; ++prgIter)
					if (prgIter().texture) ThisCall(0xA7FD30, prgIter().texture, true);
				s_renderedExteriors.Clear();
				s_exteriorKeys.Clear();
			}
			s_lastInterior = NULL;
			updateTiles = true;
		}

		if (parentWorld)
		{
			__asm
			{
				mov		eax, parentCell
				mov		eax, [eax+0x48]
				mov		ecx, [eax]
				mov		coord.x, cx
				dec		ecx
				shl		ecx, 0xC
				movd	xmm0, ecx
				cvtdq2ps	xmm0, xmm0
				movss	nwXY.x, xmm0
				mov		edx, [eax+4]
				mov		coord.y, dx
				add		edx, 2
				shl		edx, 0xC
				movd	xmm0, edx
				cvtdq2ps	xmm0, xmm0
				movss	nwXY.y, xmm0
			}
			GetLocalMapPosMults(thePlayer->PosXY(), &nwXY, &posMult);

			UInt8 quadrant = posMult.x >= 0.5F;
			if (posMult.y < 0.5F) quadrant |= 2;

			if (s_currLocalCoords != coord)
			{
				s_currLocalCoords = coord;
				updateTiles = true;
			}

			if (updateTiles)
			{
				s_currCellsSet.Clear();
				s_hiddenNodes.Clear();
				gridIdx = 9;
				do
				{
					gridIdx--;
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					cell = parentWorld->cellMap->Lookup(coord);
					s_currCellGrid[gridIdx] = cell;
					if (!cell) continue;
					s_currCellsSet.Insert(cell->refID);
					if (hideNode = cell->Get3DNode(4))
						s_hiddenNodes.Append(hideNode);
					if (useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				while (gridIdx);
				if (showDoors)
				{
					DoorRefsList *listPtr;
					for (auto clsIter = s_currCellsSet.Begin(); clsIter; ++clsIter)
						if (s_exteriorDoorRefs.Insert(*clsIter, &listPtr))
							GetTeleportDoors((TESObjectCELL*)LookupFormByRefID(*clsIter), listPtr);
					s_doorRefsList.Clear();
					for (auto drlIter = s_exteriorDoorRefs.Begin(); drlIter; ++drlIter)
						if (s_currCellsSet.HasKey(drlIter.Key()))
							s_doorRefsList.Concatenate(drlIter());
						else drlIter.Remove();
				}
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(thePlayer->PosXY());

			if (updateTiles || (s_defaultGridSize && (s_currCellQuad != quadrant)))
			{
				s_currCellQuad = quadrant;
				if (showFlags & 8)
				{
					s_useAltFormat = 2;
					g_sceneLightsLock->Enter();
					for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
					{
						if ((pntLight = (NiPointLight*)lgtNode->data->light) && (pntLight->effectType == 2))
						{
							pntLight->radiusE4 = pntLight->radius;
							pntLight->radius = 0;
						}
					}
					g_sceneLightsLock->Leave();
					memcpy(&g_TES->directionalLight->ambientColor, kDirectionalLightValues, sizeof(kDirectionalLightValues));
					memcpy(&g_shadowFogProperty->color, kFogPropertyValues, sizeof(kFogPropertyValues));
					g_shadowFogProperty->power = 1;
					*(UInt8*)0x11FF104 = 1;
					g_particleSysMngr->m_flags |= 1;
					for (auto hdnIter = s_hiddenNodes.Begin(); hdnIter; ++hdnIter)
						hdnIter->m_flags |= 1;
				}
				else s_useAltFormat = 1;

				const UInt8 *updateList = kSelectImgUpdate[quadrant];
				ExteriorEntry *exteriorEntry;
				gridIdx = 9;
				do
				{
					gridIdx--;
					cell = s_currCellGrid[gridIdx];
					if (!cell || !(updateTiles || (quadrant = updateList[gridIdx])))
						continue;

					if (s_renderedExteriors.Insert(cell->refID, &exteriorEntry))
					{
						GenerateLocalMapExterior(cell, &exteriorEntry->texture);
						s_exteriorKeys.Append(cell->refID);
					}
					else if (updateTiles)
						s_exteriorKeys.MoveToEnd(cell->refID);
					else if (!(exteriorEntry->regenFlags & quadrant))
					{
						if (gridIdx == 4) exteriorEntry->regenFlags = 0xF;
						else exteriorEntry->regenFlags |= quadrant;
						GenerateLocalMapExterior(cell, &exteriorEntry->texture);
					}
					s_tileShaderProps[gridIdx]->srcTexture = exteriorEntry->texture;
				}
				while (gridIdx);

				if (s_exteriorKeys.Size() > CACHED_TEXTURES_MAX)
				{
					UInt32 size = s_exteriorKeys.Size() - CACHED_TEXTURES_MIN;
					gridIdx = size;
					UnorderedMap<UInt32, ExteriorEntry>::Iterator findTex(&s_renderedExteriors);
					do
					{
						gridIdx--;
						findTex.Find(s_exteriorKeys[gridIdx]);
						if (!findTex) continue;
						if (findTex().texture)
							ThisCall(0xA7FD30, findTex().texture, true);
						findTex.Remove();
					}
					while (gridIdx);
					s_exteriorKeys.RemoveRange(0, size);
				}
			}
		}
		else
		{
			AdjustInteriorPos(thePlayer, &adjustedPos);
			__asm
			{
				cvttss2si	ecx, adjustedPos.x
				sar		ecx, 0xC
				mov		coord.x, cx
				dec		ecx
				shl		ecx, 0xC
				movd	xmm0, ecx
				cvtdq2ps	xmm0, xmm0
				movss	nwXY.x, xmm0
				cvttss2si	edx, adjustedPos.y
				sar		edx, 0xC
				mov		coord.y, dx
				add		edx, 2
				shl		edx, 0xC
				movd	xmm0, edx
				cvtdq2ps	xmm0, xmm0
				movss	nwXY.y, xmm0
			}
			GetLocalMapPosMults(&adjustedPos, &nwXY, &posMult);

			if (s_lastInterior != parentCell)
			{
				s_lastInterior = parentCell;
				if (!s_renderedInterior.Empty())
				{
					for (auto clrIter = s_renderedInterior.Begin(); clrIter; ++clrIter)
						if (*clrIter) ThisCall(0xA7FD30, *clrIter, true);
					s_renderedInterior.Clear();
				}
				updateTiles = true;
			}

			if (updateTiles)
			{
				s_hiddenNodes.Clear();
				if (hideNode = parentCell->Get3DNode(4))
					s_hiddenNodes.Append(hideNode);
				parentCell->RefLockEnter();
				auto refsIter = parentCell->objectList.Head();
				do
				{
					if ((objectRef = refsIter->data) && (objectRef->baseForm == g_blackPlaneBase) && !(objectRef->flags & 0x800) && (hideNode = objectRef->GetNiNode()))
						s_hiddenNodes.Append(hideNode);
				}
				while (refsIter = refsIter->next);
				parentCell->RefLockLeave();
				if (showDoors)
				{
					s_doorRefsList.Clear();
					GetTeleportDoors(parentCell, &s_doorRefsList);
				}
			}

			if (s_currLocalCoords != coord)
			{
				s_currLocalCoords = coord;
				updateTiles = true;
			}

			if (updateTiles)
			{
				if (showFlags & 8)
				{
					s_useAltFormat = 2;
					lightingPasses = *g_lightingPasses;
					*g_lightingPasses = 0x34;
					g_particleSysMngr->m_flags |= 1;
					for (auto hdnIter = s_hiddenNodes.Begin(); hdnIter; ++hdnIter)
						hdnIter->m_flags |= 1;
				}
				else s_useAltFormat = 1;

				NiRenderedTexture **renderedTexture;
				gridIdx = 9;
				do
				{
					gridIdx--;
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					if (s_renderedInterior.Insert(coord, &renderedTexture))
					{
						*renderedTexture = NULL;
						GenerateLocalMapInterior(parentCell, coord, renderedTexture);
					}
					s_tileShaderProps[gridIdx]->srcTexture = *renderedTexture;
					if (useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				while (gridIdx);
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(&adjustedPos);
		}

		if (s_useAltFormat == 2)
		{
			if (parentWorld)
			{
				g_sceneLightsLock->Enter();
				for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
					if ((pntLight = (NiPointLight*)lgtNode->data->light) && (pntLight->effectType == 2))
						pntLight->radius = pntLight->radiusE4;
				g_sceneLightsLock->Leave();
			}
			else *g_lightingPasses = lightingPasses;
			g_particleSysMngr->m_flags &= 0xFFFFFFFE;
			for (auto hdnIter = s_hiddenNodes.Begin(); hdnIter; ++hdnIter)
				hdnIter->m_flags &= 0xFFFFFFFE;
		}
		s_useAltFormat = 0;

		if (showDoors)
		{
			if (updateTiles)
			{
				auto markerIter = s_doorMarkerTiles.Begin();
				DoorMarkerTile *tileData;
				for (auto doorIter = s_doorRefsList.Begin(); doorIter; ++doorIter)
				{
					if (parentWorld)
						GetLocalMapPosMults(doorIter().doorRef->PosXY(), &nwXY, &adjustedPos);
					else
					{
						AdjustInteriorPos(doorIter().doorRef, &adjustedPos);
						GetLocalMapPosMults(&adjustedPos, &nwXY, &adjustedPos);
					}
					if ((adjustedPos.x < 0) || (adjustedPos.x >= 1.0F) || (adjustedPos.y < 0) || (adjustedPos.y >= 1.0F))
						continue;
					if (markerIter)
					{
						tileData = &markerIter();
						++markerIter;
					}
					else tileData = s_doorMarkerTiles.Append(hudMain->AddTileFromTemplate(s_doorMarkersRect, "MiniMapDoorMarkerTemplate"));
					tileData->x->SetFloat(adjustedPos.x);
					tileData->y->SetFloat(adjustedPos.y);
					cell = doorIter().linkedCell;
					tileData->visited->SetFloat(!cell || (cell == parentCell) || cell->extraDataList.HasType(kExtraData_DetachTime));
					if (useFogOfWar)
						tileData->vtxAlpha = GetVtxAlphaPtr(&adjustedPos);
					else
					{
						tileData->vtxAlpha = NULL;
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
				for (auto markerIter = s_doorMarkerTiles.Begin(); markerIter; ++markerIter)
					if (markerIter().inUse && markerIter().vtxAlpha)
						markerIter().visible->SetFloat(*markerIter().vtxAlpha > 0);
			}
		}
		else if (!s_doorMarkerTiles.Empty())
		{
			s_doorMarkerTiles.Clear();
			s_doorMarkersRect->DestroyAllChildren();
		}
	}

	if (showFlags & 4)
	{
		QuestMarkerPos *markerPos;
		if (s_updateQuestTargets || (s_activeQuest != thePlayer->activeQuest))
		{
			s_updateQuestTargets = false;
			s_activeQuest = thePlayer->activeQuest;
			if (!s_questMarkers.Empty())
			{
				s_questMarkers.Clear();
				s_lQuestMarkersRect->DestroyAllChildren();
				s_wQuestMarkersRect->DestroyAllChildren();
			}
			if (!worldMap || parentCell->worldSpace)
			{
				int depth = 29;
				auto trgIter = thePlayer->questTargetList.Head();
				BGSQuestObjective::Target *targetData;
				TileRect *targetsRect = worldMap ? s_wQuestMarkersRect : s_lQuestMarkersRect;
				do
				{
					if (!(targetData = trgIter->data)) continue;
					if (targetData->data.teleportLinks.size)
					{
						if (!(objectRef = targetData->data.teleportLinks.data[0].door))
							continue;
					}
					else if (!(objectRef = targetData->target))
						continue;
					else if ((parentCell != objectRef->parentCell) && (!parentWorld || (parentWorld != objectRef->GetParentWorld())))
						continue;
					if (!s_questMarkers.Insert(objectRef, &markerPos)) continue;
					markerPos->pos = 0;
					markerTile = hudMain->AddTileFromTemplate(targetsRect, "MiniMapQuestMarkerTemplate");
					markerPos->x = markerTile->GetValue(kTileValue_user0);
					markerPos->y = markerTile->GetValue(kTileValue_user1);
					markerTile->SetFloat(kTileValue_depth, depth);
					if (depth >= 38) depth = 29;
					else depth++;
				}
				while (trgIter = trgIter->next);
			}
		}

		if (!s_questMarkers.Empty())
		{
			UInt32 currX;
			for (auto mkrIter = s_questMarkers.Begin(); mkrIter; ++mkrIter)
			{
				objectRef = mkrIter.Key();
				markerPos = &mkrIter();
				currX = *(UInt32*)&objectRef->posX;
				if (markerPos->pos != currX)
					markerPos->pos = currX;
				else if (!updateTiles) continue;
				if (worldMap)
				{
					AdjustWorldPos(objectRef, &adjustedPos);
					GetWorldMapPosMults(&adjustedPos);
				}
				else if (parentWorld)
					GetLocalMapPosMults(objectRef->PosXY(), &nwXY, &adjustedPos);
				else
				{
					AdjustInteriorPos(objectRef, &adjustedPos);
					GetLocalMapPosMults(&adjustedPos, &nwXY, &adjustedPos);
				}
				markerPos->x->SetFloat(adjustedPos.x);
				markerPos->y->SetFloat(adjustedPos.y);
			}
		}
	}
	else if (!s_questMarkers.Empty())
	{
		s_questMarkers.Clear();
		s_lQuestMarkersRect->DestroyAllChildren();
		s_wQuestMarkersRect->DestroyAllChildren();
	}

	s_pcMarkerRotate->SetFloat(s_cellNorthRotation - thePlayer->rotZ);
	s_miniMapPosX->SetFloat(posMult.x);
	s_miniMapPosY->SetFloat(posMult.y);
	return true;
}