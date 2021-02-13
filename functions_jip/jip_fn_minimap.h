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
__m128 s_northRotationMods = {0, 1, -1, 0};

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
		cvtsi2ss	xmm0, edx
		shl		eax, 0xC
		cvtsi2ss	xmm1, eax
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
		cvtsi2ss	xmm0, edx
		shl		eax, 0xC
		cvtsi2ss	xmm1, eax
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
		movdqu	xmm0, xmmword ptr [ecx]
		movdqu	xmmword ptr [eax], xmm0
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
		CALL_EAX(kAddr_GetExtraData)
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
		mov		eax, [ecx]
		call	dword ptr [eax+0x48]
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
		mov		s_cellNorthRotation, 0
		test	byte ptr [esi+0x24], 1
		jz		skipPosDiff
		push	kExtraData_NorthRotation
		lea		ecx, [esi+0x28]
		CALL_EAX(kAddr_GetExtraData)
		test	eax, eax
		jz		skipPosDiff
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
		mov		eax, [ecx]
		call	dword ptr [eax+0x4C]
		push	2
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x48]
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
const __m128 kCaptureBounds = {-2048, 2048, 2048, -2048};

__declspec(naked) void ExtCaptureBoundsHook()
{
	static const float kFlt4096 = 4096.0F, kFlt12288 = 12288.0F;
	__asm
	{
		cmp		s_useAltFormat, 0
		jnz		altFormat
		mov		dword ptr [ebp-0x78], 0x880
		JMP_EAX(0x54EF77)
	altFormat:
		mov		dword ptr [ebp-0x78], 0x800
		mov		ecx, [ebp-0xD8]
		mov		ecx, [ecx+0x48]
		mov		eax, [ecx]
		shl		eax, 0xC
		add		eax, 0x800
		mov		[ebp-0xDC], eax
		cvtsi2ss	xmm0, eax
		movss	[ebp-0x3C], xmm0
		mov		eax, [ecx+4]
		shl		eax, 0xC
		add		eax, 0x800
		mov		[ebp-0xE0], eax
		cvtsi2ss	xmm0, eax
		movss	[ebp-0x38], xmm0
		movaps	xmm0, kCaptureBounds
		movups	[ebp-0x98], xmm0
		movss	xmm0, kFlt12288
		addss	xmm0, [ebp-0x40]
		movss	[ebp-0x34], xmm0
		addss	xmm0, kFlt4096
		subss	xmm0, [ebp-0x44]
		movss	[ebp-0x84], xmm0
		push	0x114
		CALL_EAX(0xAA13E0)
		pop		ecx
		mov		ecx, eax
		CALL_EAX(0xA712F0)
		mov		[ebp-0xC8], eax
		mov		dword ptr [eax+0x110], 0x3A83126F
		JMP_EAX(0x54F05B)
	}
}

__declspec(naked) void HideExtCellLandNodeHook()
{
	__asm
	{
		xor		eax, eax
		cmp		s_useAltFormat, 2
		jz		done
		mov		eax, [ecx+0xC4]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		test	eax, eax
		jz		done
		mov		eax, [eax+0xA0]
		mov		eax, [eax+8]
	done:
		retn	4
	}
}

__declspec(naked) void IntCaptureBoundsHook()
{
	static const float kFlt10000 = 10000.0F, kFlt40000 = 40000.0F;
	__asm
	{
		cmp		s_useAltFormat, 0
		jnz		altFormat
		mov		dword ptr [ebp-0x78], 0x880
		JMP_EAX(0x54F68F)
	altFormat:
		mov		dword ptr [ebp-0x78], 0x800
		lea		ecx, [ebp-0x38]
		mov		eax, [ebp+8]
		shl		eax, 0xC
		add		eax, 0x1000
		mov		[ebp-0x12C], eax
		cvtsi2ss	xmm0, eax
		movss	[ecx], xmm0
		mov		eax, [ebp+0xC]
		shl		eax, 0xC
		add		eax, 0x1000
		mov		[ebp-0x130], eax
		cvtsi2ss	xmm0, eax
		movss	[ecx+4], xmm0
		movss	xmm0, [ebp-0x3C]
		addss	xmm0, kFlt40000
		movss	[ecx+8], xmm0
		cmp		s_cellNorthRotation, 0
		jz		noRot
		movq	xmm0, qword ptr [ecx]
		unpcklps	xmm0, xmm0
		pshufd	xmm1, s_northRotationMods, 0xD8
		mulps	xmm0, xmm1
		movhlps	xmm1, xmm0
		addps	xmm0, xmm1
		movq	qword ptr [ecx], xmm0
	noRot:
		movaps	xmm0, kCaptureBounds
		movups	[ebp-0xA8], xmm0
		movss	xmm0, [ecx+8]
		subss	xmm0, [ebp-0x40]
		addss	xmm0, kFlt10000
		movss	[ebp-0x94], xmm0
		JMP_EAX(0x54F727)
	}
}

__declspec(naked) void __fastcall GenerateRenderedTextureHook(TESObjectCELL *cell, int EDX, NiCamera *camera, NiRenderedTexture **outTexture)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		cmp		s_useAltFormat, 2
		jz		altFormat
		push	0xFFFFFFFF
		JMP_EAX(0x54E835)
	altFormat:
		sub		esp, 0x104
		mov		[ebp-0x100], ecx
		test	byte ptr [ecx+0x24], 1
		setnz	al
		mov		[ebp-0xDD], al
		push	0x2251
		push	0x102ED68
		push	1
		push	0xE
		lea		ecx, [ebp-0xE4]
		CALL_EAX(0x404EB0)
		push	dword ptr ds:[0x11F95EC]
		lea		ecx, [ebp-0xF8]
		CALL_EAX(0x633C90)
		push	0
		lea		ecx, [ebp-0xDC]
		CALL_EAX(0x4A0EB0)
		mov		dword ptr [ebp-0x10], 0
		push	0
		push	0
		push	0
		push	0x1E
		mov		eax, ds:[0x11F9508]
		mov		[ebp-0xE8], eax
		push	eax
		mov		ecx, ds:[0x11F91A8]
		CALL_EAX(0xB6E110)
		test	eax, eax
		jz		done
		push	eax
		lea		ecx, [ebp-0x10]
		push	ecx
		call	NiReleaseAddRef
		mov		ecx, [ebp-0xE8]
		mov		eax, [ecx+0x5E0]
		mov		[ebp-0xEC], eax
		mov		eax, 0xFF1F2F3F
		xor		edx, edx
		cmp		byte ptr [ebp-0xDD], 0
		cmovnz	eax, edx
		mov		[ecx+0x5E0], eax
		mov		ecx, ds:[0x11F91C8]
		mov		[ebp-0xF0], ecx
		mov		dl, [ecx+0x130]
		mov		[ebp-0xDE], dl
		mov		byte ptr [ecx+0x130], 1
		mov		eax, ds:[0x11F4748]
		cmp		dword ptr [eax+0x200], 0
		setz	al
		mov		[ebp-0xDF], al
		mov		dword ptr [ebp-0xFC], 0
		mov		eax, ds:[0x11C7C28]
		test	eax, eax
		jz		skipHide
		test	dword ptr [eax+0x30], 1
		jnz		skipHide
		mov		[ebp-0xFC], eax
		or		dword ptr [eax+0x30], 1
	skipHide:
		CALL_EAX(0x54EE20)
		mov		[ebp-0xE0], al
		push	0
		CALL_EAX(0x54EE60)
		pop		ecx
		mov		ecx, [ebp-0x10]
		cmp		[ebp-0xDF], 0
		jz		bgnScnAlt
		CALL_EAX(0xB6B260)
		push	eax
		push	7
		CALL_EAX(0xB6B8D0)
		jmp		doneBgnScn
	bgnScnAlt:
		push	7
		push	ecx
		CALL_EAX(0x54EDE0)
	doneBgnScn:
		add		esp, 8
		push	0x280
		CALL_EAX(0xAA13E0)
		pop		ecx
		push	0x2F7
		push	1
		push	0x63
		mov		ecx, eax
		CALL_EAX(0xB660D0)
		push	eax
		lea		ecx, [ebp-0xF4]
		CALL_EAX(0x633C90)
		mov		eax, [ebp-0xF0]
		mov		ecx, [ebp-0xF4]
		mov		[ecx+0x194], eax
		push	ecx
		lea		ecx, [ebp-0x18]
		push	ecx
		call	NiReleaseAddRef
		mov		ecx, [ebp-0xF4]
		xor		eax, eax
		mov		edx, 0xC
		cmp		byte ptr [ebp-0xDD], 0
		cmovnz	eax, edx
		mov		[ecx+0x19C], eax
		push	3
		lea		ecx, [ebp-0xDC]
		CALL_EAX(0xC4F270)
		lea		edx, [ebp-0xDC]
		push	edx
		push	dword ptr [ebp-0xF0]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
		lea		ecx, [ebp-0xDC]
		CALL_EAX(0xC4F2D0)
		push	6
		mov		ecx, [ebp-0x100]
		CALL_EAX(0x456FC0)
		mov		[ebp-0x104], eax
		test	eax, eax
		jz		noNode6
		push	1
		lea		ecx, [ebp-0xDC]
		CALL_EAX(0xC4F270)
		lea		edx, [ebp-0xDC]
		push	edx
		push	dword ptr [ebp-0x104]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6BEE0)
		add		esp, 0xC
		lea		ecx, [ebp-0xDC]
		CALL_EAX(0xC4F2D0)
	noNode6:
		push	0
		push	dword ptr [ebp-0xF4]
		push	dword ptr [ebp+8]
		CALL_EAX(0xB6C0D0)
		add		esp, 0xC
		push	0
		lea		ecx, [ebp-0x18]
		push	ecx
		call	NiReleaseAddRef
		push	0
		lea		ecx, [ebp-0xF4]
		push	ecx
		call	NiReleaseAddRef
		mov		eax, 0xB6B790
		mov		ecx, 0xB6B840
		cmp		byte ptr [ebp-0xDF], 0
		cmovz	eax, ecx
		call	eax
		movzx	eax, byte ptr [ebp-0xE0]
		push	eax
		CALL_EAX(0x54EE60)
		pop		ecx
		mov		eax, [ebp-0xFC]
		test	eax, eax
		jz		skipUnhide
		xor		dword ptr [eax+0x30], 1
	skipUnhide:
		mov		al, [ebp-0xDE]
		mov		ecx, [ebp-0xF0]
		mov		[ecx+0x130], al
		mov		eax, ds:[0x11F95EC]
		mov		byte ptr [eax+0x86], 1
		mov		ecx, [ebp-0xE8]
		mov		eax, [ebp-0xEC]
		mov		[ecx+0x5E0], eax
		lea		ecx, [ebp-0xF4]
		CALL_EAX(0x45CEC0)
	done:
		lea		eax, [ebp-0x10]
		push	eax
		mov		ecx, [ebp+0xC]
		CALL_EAX(0x6E5CC0)
		lea		ecx, [ebp-0x10]
		CALL_EAX(0x45CEC0)
		lea		ecx, [ebp-0xDC]
		CALL_EAX(0x4A0F60)
		lea		ecx, [ebp-0xE4]
		CALL_EAX(0x404EE0)
		leave
		retn	8
	}
}

__declspec(naked) void D3DFormatHook()
{
	__asm
	{
		mov		eax, [esp+0x2C]
		cmp		s_useAltFormat, 0
		jz		defaultFmt
		mov		dword ptr [eax], 0x16
		mov		ecx, s_texturePixelSize
		jmp		done
	defaultFmt:
		mov		dword ptr [eax], 0x17
		mov		ecx, 0x80
	done:
		mov		[eax+8], ecx
		mov		[eax+0xC], ecx
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

typedef Vector<Tile*> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles> RenderedMapMarkers;
RenderedMapMarkers s_renderedMapMarkers(0x40);

void __fastcall DestroyCellMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
		dtlIter->Destroy(true);
	cmkIter.Remove();
}

bool s_discoveredLocation = false;

void __fastcall HandleDiscoverLocation(TESObjectCELL *markerCell)
{
	if (markerCell)
	{
		auto findRendered = s_renderedMapMarkers.Find(Coordinate(&markerCell->coords.exterior->x));
		if (findRendered)
		{
			DestroyCellMarkers(findRendered);
			s_discoveredLocation = true;
		}
	}
	StdCall(0x8D5100, *(int*)0x11D0368);
}

__declspec(naked) void DiscoverLocationHook()
{
	__asm
	{
		mov		ecx, [ebp-0x8C]
		mov		ecx, [ecx+4]
		mov		ecx, [ecx+0x40]
		push	0x77958A
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
		JMP_EDX(0x60F145)
	}
}

__declspec(naked) void WaterToLandClipHook()
{
	__asm
	{
		mov		eax, [ebp-0x10]
		mov		eax, [eax+0x28]
		test	eax, eax
		jz		noData
		movss	xmm0, [eax+0xA0]
		subss	xmm0, kFltOne
		mov		al, 1
		jmp		done
	noData:
		pxor	xmm0, xmm0
	done:
		movss	[ebp-0x20], xmm0
		mov		[ebp-0x31], al
		JMP_EAX(0x49CA94)
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
		JMP_EDX(0xE7D144)
	}
}

UInt8 s_miniMapIndex = 0;
TileRect *s_localMapRect, *s_worldMapRect, *s_mapMarkersRect, *s_doorMarkersRect, *s_lQuestMarkersRect, *s_wQuestMarkersRect;
TileImage *s_worldMapTile;
Tile::Value *s_miniMapMode, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom;
TileShaderProperty *s_tileShaderProps[9];
bool s_defaultGridSize;
ShadowSceneNode *g_shadowSceneNode;
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
		sciTriShp->alphaProp = alphaProp;
	}
	while (++index < 9);
	UpdateTileScales();

	s_doorMarkersRect = (TileRect*)node->data;
	s_lQuestMarkersRect = (TileRect*)node->prev->data;
	node = s_worldMapRect->children.Tail();
	s_worldMapTile = (TileImage*)node->data;
	s_mapMarkersRect = (TileRect*)node->prev->data;
	s_wQuestMarkersRect = (TileRect*)node->prev->prev->data;

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
	g_shadowSceneNode = *(ShadowSceneNode**)0x11F91C8;
	g_shadowFogProperty = *(BSFogProperty**)0x11DEB00;
	g_particleSysMngr = *(BSParticleSystemManager**)0x11DED58;
	g_blackPlaneBase = LookupFormByRefID(0x15A1F2);
	SafeWrite16(0x452736, 0x7705);
	SafeWriteBuf(0x87A12A, "\x31\xD2\x66\x89\x50\x26\x89\x50\x28\x90", 10);
	WriteRelCall(0x54B72B, (UInt32)UpdateCellsSeenBitsHook);
	WriteRelCall(0x9438F6, (UInt32)UpdateCellsSeenBitsHook);
	WriteRelJump(0x54EF70, (UInt32)ExtCaptureBoundsHook);
	//WriteRelCall(0x54F257, (UInt32)HideExtCellLandNodeHook);
	WriteRelJump(0x54F688, (UInt32)IntCaptureBoundsHook);
	WriteRelJump(0x54E830, (UInt32)GenerateRenderedTextureHook);
	SafeWrite32(0xB6D0C0, (UInt32)D3DFormatHook);
	WriteRelJump(0x779567, (UInt32)DiscoverLocationHook);
	WriteRelJump(0x60F13D, (UInt32)SetQuestTargetsHook);
	WriteRelJump(0x49CA5A, (UInt32)WaterToLandClipHook);
	WriteRelJump(0xE7D13A, (UInt32)RendererRecreateHook);
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

struct DoorMarkerVisibility
{
	Tile::Value		*visibility;
	float			*alphaPtr;

	DoorMarkerVisibility(Tile::Value *_visibility, float *_alphaPtr) : visibility(_visibility), alphaPtr(_alphaPtr) {}
};

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
DoorRefsList s_doorRefsList(0x20);
Vector<DoorMarkerVisibility> s_doorMarkerList(0x20);
Vector<NiNode*> s_hiddenNodes(0x20);
TESQuest *s_activeQuest = NULL;
UnorderedMap<TESObjectREFR*, QuestMarkerPos> s_questMarkers(0x10);
TESObjectCELL *s_currCellGrid[9] = {}, *s_lastInterior = NULL;
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

	bool updateTiles = s_pcCurrCell != parentCell;
	s_pcCurrCell = parentCell;
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
	TESWorldSpace *parentWorld = parentCell->worldSpace;
	NiPoint2 nwXY, posMult, adjustedPos;
	Coordinate coord;
	Tile *markerTile;
	TESObjectREFR *objectRef;
	HUDMainMenu *hudMain = g_HUDMainMenu;
	int depth = 18;

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
				s_mapMarkersRect->DestroyAllChildren();
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
					CellMapMarkers *mapMarkers;
					DynamicTiles *dynamicTiles;
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
								markerTile = hudMain->AddTileFromTemplate(s_mapMarkersRect, "MiniMapWorldMarkerTemplate");
								dynamicTiles->Append(markerTile);
								ddsPath = (mkIter().data->flags & 2) ? g_mapMarkerIcons[mkIter().data->type] : (const char*)0x1075030;
								markerTile->SetString(kTileValue_filename, ddsPath);
								markerTile->SetFloat(kTileValue_user0, mkIter().pos.x);
								markerTile->SetFloat(kTileValue_user1, mkIter().pos.y);
								markerTile->SetFloat(kTileValue_depth, depth);
								if (depth >= 27) depth = 18;
								else depth++;
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
					if (!s_currMarkerCells.HasKey(cmkIter.Key())) DestroyCellMarkers(cmkIter);
			}
		}
		else if (!s_renderedMapMarkers.Empty())
		{
			s_mapMarkersRect->DestroyAllChildren();
			s_renderedMapMarkers.Clear();
		}
	}
	else
	{
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
				cvtsi2ss	xmm0, ecx
				movss	nwXY.x, xmm0
				mov		edx, [eax+4]
				mov		coord.y, dx
				add		edx, 2
				shl		edx, 0xC
				cvtsi2ss	xmm0, edx
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
					parentCell = parentWorld->cellMap->Lookup(coord);
					s_currCellGrid[gridIdx] = parentCell;
					if (!parentCell) continue;
					s_currCellsSet.Insert(parentCell->refID);
					if (hideNode = parentCell->Get3DNode(4))
						s_hiddenNodes.Append(hideNode);
					if (useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				while (gridIdx);
				s_doorMarkersRect->DestroyAllChildren();
				s_doorMarkerList.Clear();
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
					for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
					{
						if ((pntLight = (NiPointLight*)lgtNode->data->light) && (pntLight->effectType == 2) && !pntLight->unkEC)
						{
							pntLight->unkEC = pntLight->radius;
							pntLight->radius = 0;
						}
					}
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
					parentCell = s_currCellGrid[gridIdx];
					if (!parentCell || !(updateTiles || (quadrant = updateList[gridIdx])))
						continue;

					if (s_renderedExteriors.Insert(parentCell->refID, &exteriorEntry))
					{
						ThisCall(0x54E640, parentCell, exteriorEntry);
						s_exteriorKeys.Append(parentCell->refID);
					}
					else if (updateTiles)
					{
						s_exteriorKeys.Remove(parentCell->refID);
						s_exteriorKeys.Append(parentCell->refID);
					}
					else if (!(exteriorEntry->regenFlags & quadrant))
					{
						if (gridIdx == 4) exteriorEntry->regenFlags = 0xF;
						else exteriorEntry->regenFlags |= quadrant;
						ThisCall(0x54E640, parentCell, exteriorEntry);
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
				cvtsi2ss	xmm0, ecx
				movss	nwXY.x, xmm0
				cvttss2si	edx, adjustedPos.y
				sar		edx, 0xC
				mov		coord.y, dx
				add		edx, 2
				shl		edx, 0xC
				cvtsi2ss	xmm0, edx
				movss	nwXY.y, xmm0
			}
			GetLocalMapPosMults(&adjustedPos, &nwXY, &posMult);

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
						ThisCall(0x54E750, parentCell, (SInt32)coord.x, (SInt32)coord.y, renderedTexture);
					}
					s_tileShaderProps[gridIdx]->srcTexture = *renderedTexture;
					if (useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				while (gridIdx);

				s_doorMarkersRect->DestroyAllChildren();
				s_doorMarkerList.Clear();
			}
			else if (updateFogOfWar)
				DoSelectiveFOWUpdate(&adjustedPos);
		}

		if (s_useAltFormat == 2)
		{
			if (parentWorld)
			{
				for (auto lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
				{
					if ((pntLight = (NiPointLight*)lgtNode->data->light) && (pntLight->effectType == 2) && !pntLight->radius)
					{
						pntLight->radius = pntLight->unkEC;
						pntLight->unkEC = 0;
					}
				}
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
				for (auto doorIter = s_doorRefsList.Begin(); doorIter; ++doorIter)
				{
					if (parentWorld)
						GetLocalMapPosMults(doorIter().doorRef->PosXY(), &nwXY, &adjustedPos);
					else
					{
						AdjustInteriorPos(doorIter().doorRef, &adjustedPos);
						GetLocalMapPosMults(&adjustedPos, &nwXY, &adjustedPos);
						if ((adjustedPos.x < 0) || (adjustedPos.x >= 1.0F) || (adjustedPos.y < 0) || (adjustedPos.y >= 1.0F))
							continue;
					}
					markerTile = hudMain->AddTileFromTemplate(s_doorMarkersRect, "MiniMapDoorMarkerTemplate");
					markerTile->SetFloat(kTileValue_user0, adjustedPos.x);
					markerTile->SetFloat(kTileValue_user1, adjustedPos.y);
					markerTile->SetFloat(kTileValue_depth, depth);
					if (!doorIter().linkedCell || doorIter().linkedCell->extraDataList.HasType(kExtraData_DetachTime))
						markerTile->SetFloat(kTileValue_user2, 1.0F);
					if (depth >= 27) depth = 18;
					else depth++;
					if (useFogOfWar)
						s_doorMarkerList.Append(markerTile->GetValue(kTileValue_visible), GetVtxAlphaPtr(&adjustedPos));
				}
			}

			if (!s_doorMarkerList.Empty() && (updateTiles || updateFogOfWar))
				for (auto mkrIter = s_doorMarkerList.Begin(); mkrIter; ++mkrIter)
					mkrIter().visibility->SetFloat(*mkrIter().alphaPtr > 0);
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
			parentCell = s_pcCurrCell;
			if (!worldMap || parentCell->worldSpace)
			{
				depth = 29;
				auto trgIter = thePlayer->questTargetList.Head();
				BGSQuestObjective::Target *targetData;
				TileRect *targetsRect = worldMap ? s_wQuestMarkersRect : s_lQuestMarkersRect;
				do
				{
					if (!(targetData = trgIter->data)) continue;
					if (targetData->data.teleportLinks.size)
						objectRef = targetData->data.teleportLinks.data[0].door;
					else if (!(objectRef = targetData->target) || ((parentCell != objectRef->parentCell) && (!parentWorld || (parentWorld != objectRef->GetParentWorld()))))
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