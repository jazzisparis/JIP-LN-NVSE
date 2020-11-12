#pragma once

DEFINE_COMMAND_PLUGIN(InitMiniMap, , 0, 0, NULL);
DEFINE_COMMAND_PLUGIN(UpdateMiniMap, , 0, 2, kParams_OneInt_OneOptionalInt);

#define CACHED_TEXTURES_MAX			60
#define CACHED_TEXTURES_PURGED		40
#define QUEST_TARGETS_POS_UPD_FREQ	5

__declspec(naked) NiTriangle *GenerateTriangles()
{
	__asm
	{
		push	0xC00
		mov		eax, 0xAA13E0
		call	eax
		pop		ecx
		mov		ecx, eax
		push	eax
		push	ebx
		xor		ah, ah
		xor		dx, dx
	iter1Head:
		xor		al, al
		mov		bl, ah
		and		bl, 1
	iter2Head:
		mov		bh, al
		and		bh, 1
		cmp		bl, bh
		mov		bx, dx
		jz		invDir
		mov		[ecx+2], bx
		inc		bx
		mov		[ecx+4], bx
		mov		[ecx+6], bx
		add		bx, 0x10
		mov		[ecx], bx
		mov		[ecx+0xA], bx
		inc		bx
		mov		[ecx+8], bx
		jmp		iter2Next
	invDir:
		mov		[ecx+4], bx
		mov		[ecx+6], bx
		inc		bx
		mov		[ecx+8], bx
		add		bx, 0x10
		mov		[ecx+2], bx
		inc		bx
		mov		[ecx], bx
		mov		[ecx+0xA], bx
	iter2Next:
		add		ecx, 0xC
		inc		dx
		inc		al
		cmp		al, 0x10
		jb		iter2Head
		inc		dx
		inc		ah
		cmp		ah, 0x10
		jb		iter1Head
		pop		ebx
		pop		eax
		retn
	}
}

Tile::Value *s_miniMapScale, *s_localMapZoom;
BSScissorTriShape *s_localMapShapes[9];
UInt32 s_texturePixelSize = 0x110;

__declspec(naked) void UpdateTileScales()
{
	__asm
	{
		mov		eax, s_localMapZoom
		movss	xmm0, [eax+8]
		addss	xmm0, kFltHalf
		mov		eax, s_miniMapScale
		mulss	xmm0, [eax+8]
		mov		edx, 9
	iterHead:
		dec		edx
		mov		eax, s_localMapShapes[edx*4]
		movss	[eax+0x64], xmm0
		jnz		iterHead
		mov		eax, s_localMapZoom
		movss	xmm0, [eax+8]
		mulss	xmm0, kFlt10
		cvttss2si	eax, xmm0
		imul	eax, 0x10
		add		eax, 0x70
		mov		s_texturePixelSize, eax
		retn
	}
}

float s_cellNorthRotation = 0, s_northRotationVector[] = {1, 0, 0, 1};

__declspec(naked) void __fastcall GetNorthRotation(TESObjectCELL *cell)
{
	__asm
	{
		test	byte ptr [ecx+0x24], 1
		jz		noRotation
		push	kExtraData_NorthRotation
		add		ecx, 0x28
		call	GetExtraData
		test	eax, eax
		jz		noRotation
		fld		dword ptr [eax+0xC]
		fchs
		fst		s_cellNorthRotation
		mov		eax, offset s_northRotationVector
		fsincos
		fst		dword ptr [eax]
		fstp	dword ptr [eax+0xC]
		fst		dword ptr [eax+4]
		fchs
		fstp	dword ptr [eax+8]
		retn
	noRotation:
		mov		s_cellNorthRotation, 0
		retn
	}
}

__declspec(naked) void __fastcall AdjustInteriorPos(CoordXY *inPos, CoordXY *outPos)
{
	__asm
	{
		cmp		s_cellNorthRotation, 0
		jz		noRotation
		mov		eax, offset s_northRotationVector
		movss	xmm0, [ecx]
		mulss	xmm0, [eax]
		movss	xmm1, [ecx+4]
		mulss	xmm1, [eax+8]
		addss	xmm0, xmm1
		movss	xmm1, [ecx]
		mulss	xmm1, [eax+4]
		movss	xmm2, [ecx+4]
		mulss	xmm2, [eax+0xC]
		addss	xmm1, xmm2
		jmp		done
	noRotation:
		movss	xmm0, [ecx]
		movss	xmm1, [ecx+4]
	done:
		subss	xmm0, kFlt2048
		subss	xmm1, kFlt2048
		movss	[edx], xmm0
		movss	[edx+4], xmm1
		retn
	}
}

__declspec(naked) UInt32 __fastcall GetInteriorCoord(CoordXY *adjustedPos)
{
	__asm
	{
		cvttss2si	eax, [ecx]
		sar		eax, 0xC
		shl		eax, 0x10
		cvttss2si	ecx, [ecx+4]
		sar		ecx, 0xC
		mov		ax, cx
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
	static const float kSizeDivisor = 0.796875F;
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
		divss	xmm1, kSizeDivisor
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
		divss	xmm1, kSizeDivisor
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

__declspec(naked) void __fastcall AdjustWorldPos(CoordXY *inPos, CoordXY *outMults)
{
	__asm
	{
		mov		eax, offset s_currWorldPosMods
		cmp		dword ptr [eax], 0x3F800000
		jz		noScale
		movss	xmm0, [ecx]
		mulss	xmm0, [eax]
		addss	xmm0, [eax+0xC]
		movss	xmm1, [ecx+4]
		mulss	xmm1, [eax]
		addss	xmm1, [eax+0x10]
		jmp		done
	noScale:
		movss	xmm0, [ecx]
		addss	xmm0, [eax+4]
		movss	xmm1, [ecx+4]
		addss	xmm1, [eax+8]
	done:
		movss	[edx], xmm0
		movss	[edx+4], xmm1
		retn
	}
}

__declspec(naked) void __fastcall GetWorldMapPosMults(CoordXY *outMults)
{
	static const float kPosMultMod = 0.1015625F;
	__asm
	{
		mov		eax, offset s_rootWorldDimensions
		movss	xmm0, [ecx]
		subss	xmm0, [eax]
		divss	xmm0, [eax+8]
		addss	xmm0, kPosMultMod
		movss	[ecx], xmm0
		movss	xmm0, [ecx+4]
		subss	xmm0, [eax+4]
		divss	xmm0, [eax+0xC]
		addss	xmm0, kPosMultMod
		movss	[ecx+4], xmm0
		retn
	}
}

__declspec(naked) UInt32 __fastcall GetPackedCoords(SInt32 *coords)
{
	__asm
	{
		mov		ax, [ecx]
		shl		eax, 0x10
		mov		ax, [ecx+4]
		retn
	}
}

__declspec(naked) SeenData* __fastcall GetSeenData(TESObjectCELL *cell)
{
	__asm
	{
		push	kExtraData_SeenData
		add		ecx, 0x28
		call	GetExtraData
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) bool SeenData::GetBitSeen(UInt32 column, UInt32 row)
{
	__asm
	{
		lea		eax, [ecx+4]
		mov		ecx, [esp+8]
		and		ecx, 1
		shl		ecx, 4
		add		ecx, [esp+4]
		mov		edx, 1
		shl		edx, cl
		mov		ecx, [esp+8]
		shr		ecx, 1
		test	[eax+ecx*4], edx
		setnz	al
		retn	8
	}
}

__declspec(naked) IntSeenData *IntSeenData::GetSectionSeenData(int sectionX, int sectionY)
{
	__asm
	{
		push	esi
		mov		eax, ecx
		mov		ecx, [esp+8]
		mov		edx, [esp+0xC]
		jmp		iterHead
		lea		esp, [esp]
	iterHead:
		movsx	esi, byte ptr [eax+0x24]
		cmp		esi, ecx
		jnz		iterNext
		movsx	esi, byte ptr [eax+0x25]
		cmp		esi, edx
		jz		done
	iterNext:
		mov		eax, [eax+0x28]
		test	eax, eax
		jnz		iterHead
	done:
		pop		esi
		retn	8
	}
}

__declspec(naked) UInt32 __fastcall GetSectionSeenLevel(void *dataPtr)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 8
		push	ebx
		push	esi
		push	edi
		mov		ebx, ecx
		mov		[ebp-4], 0
		mov		esi, [ebx+0xC]
		cmp		esi, 0xF
		setz	al
		shl		al, 1
		mov		[ebp-3], al
		mov		edi, [ebx+0x10]
		cmp		edi, 0xF
		setz	al
		add		[ebp-3], al
		mov		ecx, [ebx]
		cmp		dword ptr [ecx], kVtbl_IntSeenData
		jnz		isExterior
		push	dword ptr [ebx+8]
		push	dword ptr [ebx+4]
		call	IntSeenData::GetSectionSeenData
		mov		[ebp-8], eax
		test	eax, eax
		jz		noData0Int
		push	edi
		push	esi
		mov		ecx, eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
	noData0Int:
		mov		ecx, [ebp-8]
		mov		al, [ebp-3]
		test	al, al
		jz		case0Both
		cmp		al, 1
		jz		case1Int
		cmp		al, 2
		jz		case2Int
		jmp		case3Int
	case1Int:
		test	ecx, ecx
		jz		noData1Int
		push	edi
		lea		eax, [esi+1]
		push	eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
	noData1Int:
		mov		eax, [ebx+8]
		inc		eax
		push	eax
		push	dword ptr [ebx+4]
		mov		ecx, [ebx]
		call	IntSeenData::GetSectionSeenData
		test	eax, eax
		jz		done
		jmp		case1Both
	case2Int:
		test	ecx, ecx
		jz		noData2Int
		lea		eax, [edi+1]
		push	eax
		push	esi
		call	SeenData::GetBitSeen
		add		[ebp-4], al
	noData2Int:
		push	dword ptr [ebx+8]
		mov		eax, [ebx+4]
		inc		eax
		push	eax
		mov		ecx, [ebx]
		call	IntSeenData::GetSectionSeenData
		test	eax, eax
		jz		done
		jmp		case2Both
	case3Int:
		mov		eax, [ebx+8]
		inc		eax
		push	eax
		push	dword ptr [ebx+4]
		mov		ecx, [ebx]
		call	IntSeenData::GetSectionSeenData
		test	eax, eax
		jz		noData3Int
		push	0
		push	esi
		mov		ecx, eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
	noData3Int:
		push	dword ptr [ebx+8]
		mov		eax, [ebx+4]
		inc		eax
		push	eax
		mov		ecx, [ebx]
		call	IntSeenData::GetSectionSeenData
		test	eax, eax
		jz		noData4Int
		push	edi
		push	0
		mov		ecx, eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
	noData4Int:
		mov		eax, [ebx+8]
		inc		eax
		push	eax
		mov		eax, [ebx+4]
		inc		eax
		push	eax
		mov		ecx, [ebx]
		call	IntSeenData::GetSectionSeenData
		test	eax, eax
		jz		done
		push	0
		push	0
		mov		ecx, eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		jmp		done
	isExterior:
		mov		ax, [ebx+4]
		shl		eax, 0x10
		mov		ax, [ebx+8]
		push	eax
		call	CellPointerMap::Lookup
		mov		[ebp-8], eax
		test	eax, eax
		jz		noDataExt
		mov		ecx, eax
		test	byte ptr [eax+0x25], 1
		setnz	al
		mov		[ebp-2], al
		jnz		modRes0
		call	GetSeenData
		mov		[ebp-8], eax
		test	eax, eax
		jz		noDataExt
		push	edi
		push	esi
		mov		ecx, eax
		call	SeenData::GetBitSeen
	modRes0:
		add		[ebp-4], al
	noDataExt:
		mov		al, [ebp-2]
		mov		ecx, [ebp-8]
		mov		dl, [ebp-3]
		test	dl, dl
		jz		case0Ext
		cmp		dl, 1
		jz		case1Ext
		cmp		dl, 2
		jz		case2Ext
		jmp		case3Ext
	case0Ext:
		test	al, al
		jz		case0Both
		add		[ebp-4], 3
		jmp		done
	case0Both:
		test	ecx, ecx
		jz		done
		lea		eax, [edi+1]
		push	eax
		push	esi
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		push	edi
		lea		eax, [esi+1]
		push	eax
		mov		ecx, [ebp-8]
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		lea		eax, [edi+1]
		push	eax
		lea		eax, [esi+1]
		push	eax
		mov		ecx, [ebp-8]
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		jmp		done
	case1Ext:
		test	al, al
		jnz		modRes1
		test	ecx, ecx
		jz		getCell2
		push	edi
		lea		eax, [esi+1]
		push	eax
		call	SeenData::GetBitSeen
	modRes1:
		add		[ebp-4], al
	getCell2:
		mov		ax, [ebx+4]
		shl		eax, 0x10
		mov		ax, [ebx+8]
		inc		ax
		push	eax
		mov		ecx, [ebx]
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
		mov		[ebp-8], eax
		push	0
		push	esi
		mov		ecx, eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		push	0
		lea		eax, [esi+1]
		push	eax
		mov		ecx, [ebp-8]
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		jmp		done
	case2Ext:
		test	al, al
		jnz		modRes2
		test	ecx, ecx
		jz		getCell3
		lea		eax, [edi+1]
		push	eax
		push	esi
		call	SeenData::GetBitSeen
	modRes2:
		add		[ebp-4], al
	getCell3:
		mov		ax, [ebx+4]
		inc		ax
		shl		eax, 0x10
		mov		ax, [ebx+8]
		push	eax
		mov		ecx, [ebx]
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
		mov		[ebp-8], eax
		push	edi
		push	0
		mov		ecx, eax
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		lea		eax, [edi+1]
		push	eax
		push	0
		mov		ecx, [ebp-8]
		call	SeenData::GetBitSeen
		add		[ebp-4], al
		jmp		done
	case3Ext:
		mov		ax, [ebx+4]
		shl		eax, 0x10
		mov		ax, [ebx+8]
		inc		ax
		push	eax
		mov		ecx, [ebx]
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
		push	0
		push	esi
		mov		ecx, eax
		call	SeenData::GetBitSeen
	modRes3:
		add		[ebp-4], al
	getCell4:
		mov		ax, [ebx+4]
		inc		ax
		shl		eax, 0x10
		mov		ax, [ebx+8]
		push	eax
		mov		ecx, [ebx]
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
		push	edi
		push	0
		mov		ecx, eax
		call	SeenData::GetBitSeen
	modRes4:
		add		[ebp-4], al
	getCell5:
		mov		ax, [ebx+4]
		inc		ax
		shl		eax, 0x10
		mov		ax, [ebx+8]
		inc		ax
		push	eax
		mov		ecx, [ebx]
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
		push	0
		push	0
		mov		ecx, eax
		call	SeenData::GetBitSeen
	modRes5:
		add		[ebp-4], al
	done:
		movzx	eax, byte ptr [ebp-4]
		pop		edi
		pop		esi
		pop		ebx
		mov		esp, ebp
		pop		ebp
		retn
	}
}

UInt32 s_packedCellCoords[9];
float s_vertexAlphaLevel[] = {0, 0.25, 0.5, 0.75, 1};

__declspec(naked) void __stdcall CalcVtxAlphaBySeenData(UInt32 gridIdx)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x18
		push	ebx
		push	esi
		push	edi
		mov		ecx, s_pcCurrCell
		test	byte ptr [ecx+0x24], 1
		jz		isExterior
		call	GetSeenData
		test	eax, eax
		jz		done
		jmp		proceed
	isExterior:
		mov		eax, [ecx+0xC0]
		test	eax, eax
		jz		done
		mov		eax, [eax+0x30]
		test	eax, eax
		jz		done
	proceed:
		mov		[ebp-0x14], eax
		mov		eax, [ebp+8]
		mov		ecx, s_localMapShapes[eax*4]
		mov		ecx, [ecx+0xB8]
		mov		[ebp-0x18], ecx
		mov		ebx, [ecx+0x28]
		add		ebx, 0xC
		lea		ecx, s_packedCellCoords[eax*4]
		movsx	edx, word ptr [ecx+2]
		mov		[ebp-0x10], edx
		movsx	edx, word ptr [ecx]
		mov		[ebp-0xC], edx
		xor		esi, esi
		mov		eax, esi
	iter1Head:
		mov		[ebp-8], eax
		mov		edi, 0x10
		mov		dword ptr [ebp-4], 0
		inc		dword ptr [ebp-0xC]
		jmp		iter2Head
		and		esp, 0xEFFFFFFF
		lea		esp, [esp]
		fnop
	iter2Head:
		lea		ecx, [ebp-0x14]
		call	GetSectionSeenLevel
		mov		ecx, s_vertexAlphaLevel[eax*4]
		mov		[ebx], ecx
		add		ebx, 0x10
		test	edi, 0x10
		jz		iter2Next
		dec		dword ptr [ebp-0xC]
	iter2Next:
		dec		edi
		mov		[ebp-4], edi
		jns		iter2Head
		inc		esi
		cmp		esi, 0x10
		ja		iter1End
		mov		eax, esi
		jb		iter1Head
		xor		eax, eax
		inc		dword ptr [ebp-0x10]
		jmp		iter1Head
	iter1End:
		mov		ecx, [ebp-0x18]
		mov		ecx, [ecx+0x34]
		test	ecx, ecx
		jz		done
		mov		dword ptr [ecx+0x20], 0
	done:
		pop		edi
		pop		esi
		pop		ebx
		mov		esp, ebp
		pop		ebp
		retn	4
	}
}

__declspec(naked) bool __fastcall GetLinkedCellVisited(TESObjectREFR *doorRef)
{
	__asm
	{
		push	kExtraData_Teleport
		add		ecx, 0x44
		call	GetExtraData
		test	eax, eax
		jz		retn1
		mov		ecx, [eax+0xC]
		test	ecx, ecx
		jz		retn1
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		retn1
		mov		ecx, [ecx+0x40]
		test	ecx, ecx
		jz		retn1
		test	byte ptr [ecx+0x31], 8
		setnz	al
		retn
	retn1:
		mov		al, 1
		retn
	}
}

typedef Vector<TESObjectREFR*> DoorRefsList;
TESObjectDOOR *s_defaultDoor;

void __fastcall GetTeleportDoors(TESObjectCELL *cell, DoorRefsList *doorRefsList)
{
	ThisCall(0x541AC0, cell);
	ListNode<TESObjectREFR> *refsIter = cell->objectList.Head();
	TESObjectREFR *refr;
	TESObjectDOOR *baseDoor;
	do
	{
		refr = refsIter->data;
		if (!refr || (refr->flags & 0x860)) continue;
		baseDoor = (TESObjectDOOR*)refr->baseForm;
		if (IS_ID(baseDoor, TESObjectDOOR) && (baseDoor != s_defaultDoor) && (!baseDoor->list88.Empty() || GetExtraType(&refr->extraDataList, Teleport)))
			doorRefsList->Append(refr);
	}
	while (refsIter = refsIter->next);
	ThisCall(0x541AE0, cell);
}

TESForm *s_blackPlaneBase;
Vector<NiNode*> s_hiddenNodes(0x20);

void __fastcall GetBlackPlaneNodes(TESObjectCELL *cell)
{
	ThisCall(0x541AC0, cell);
	ListNode<TESObjectREFR> *refsIter = cell->objectList.Head();
	TESObjectREFR *refr;
	NiNode *node;
	do
	{
		refr = refsIter->data;
		if (refr && (refr->baseForm == s_blackPlaneBase) && (node = refr->GetNiNode()))
			s_hiddenNodes.Append(node);
	}
	while (refsIter = refsIter->next);
	ThisCall(0x541AE0, cell);
}

UInt8 s_useAltFormat = 0;
alignas(16) const UInt32 kCaptureBounds[] = {0xC5000000, 0x45000000, 0x45000000, 0xC5000000};

__declspec(naked) void ExtCaptureBoundsHook()
{
	__asm
	{
		cmp		s_useAltFormat, 0
		jnz		altFormat
		mov		dword ptr [ebp-0x78], 0x880
		mov		eax, 0x54EF77
		jmp		eax
	altFormat:
		mov		dword ptr [ebp-0x78], 0x800
		mov		ecx, [ebp-0xD8]
		mov		ecx, [ecx+0x48]
		mov		eax, [ecx]
		shl		eax, 0xC
		add		eax, 0x800
		mov		[ebp-0xDC], eax
		fild	dword ptr [ebp-0xDC]
		fstp	dword ptr [ebp-0x3C]
		mov		eax, [ecx+4]
		shl		eax, 0xC
		add		eax, 0x800
		mov		[ebp-0xE0], eax
		fild	dword ptr [ebp-0xE0]
		fstp	dword ptr [ebp-0x38]
		movdqa	xmm0, xmmword ptr ds:[kCaptureBounds]
		movdqu	xmmword ptr [ebp-0x98], xmm0
		fld		kFlt12288
		fadd	dword ptr [ebp-0x40]
		fst		dword ptr [ebp-0x34]
		fadd	kFlt4096
		fsub	dword ptr [ebp-0x44]
		fstp	dword ptr [ebp-0x84]
		push	0x114
		mov		eax, 0xAA13E0
		call	eax
		pop		ecx
		mov		ecx, eax
		mov		eax, 0xA712F0
		call	eax
		mov		[ebp-0xC8], eax
		mov		dword ptr [eax+0x110], 0x3A83126F
		mov		eax, 0x54F05B
		jmp		eax
	}
}

/*__declspec(naked) void HideExtCellLandNodeHook()
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
}*/

__declspec(naked) void IntCaptureBoundsHook()
{
	__asm
	{
		cmp		s_useAltFormat, 0
		jnz		altFormat
		mov		dword ptr [ebp-0x78], 0x880
		mov		eax, 0x54F68F
		jmp		eax
	altFormat:
		mov		dword ptr [ebp-0x78], 0x800
		lea		ecx, [ebp-0x38]
		mov		eax, [ebp+8]
		shl		eax, 0xC
		add		eax, 0x1000
		mov		[ebp-0x12C], eax
		fild	dword ptr [ebp-0x12C]
		fstp	dword ptr [ecx]
		mov		eax, [ebp+0xC]
		shl		eax, 0xC
		add		eax, 0x1000
		mov		[ebp-0x130], eax
		fild	dword ptr [ebp-0x130]
		fstp	dword ptr [ecx+4]
		fld		dword ptr [ebp-0x3C]
		fadd	kFlt40000
		fstp	dword ptr [ecx+8]
		cmp		s_cellNorthRotation, 0
		jz		noRot
		mov		eax, offset s_northRotationVector
		fld		dword ptr [ecx]
		fmul	dword ptr [eax]
		fld		dword ptr [ecx+4]
		fmul	dword ptr [eax+4]
		faddp	st(1), st
		fld		dword ptr [ecx]
		fmul	dword ptr [eax+8]
		fld		dword ptr [ecx+4]
		fmul	dword ptr [eax+0xC]
		faddp	st(1), st
		fstp	dword ptr [ecx+4]
		fstp	dword ptr [ecx]
	noRot:
		movdqa	xmm0, xmmword ptr ds:[kCaptureBounds]
		movdqu	xmmword ptr [ebp-0xA8], xmm0
		fld		dword ptr [ecx+8]
		fsub	dword ptr [ebp-0x40]
		fadd	kFlt10000
		fstp	dword ptr [ebp-0x94]
		mov		eax, 0x54F727
		jmp		eax
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
		mov		eax, 0x54E835
		jmp		eax
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
		mov		eax, 0x404EB0
		call	eax
		push	dword ptr ds:[0x11F95EC]
		lea		ecx, [ebp-0xF8]
		mov		eax, 0x633C90
		call	eax
		push	0
		lea		ecx, [ebp-0xDC]
		mov		eax, 0x4A0EB0
		call	eax
		mov		dword ptr [ebp-0x10], 0
		push	0
		push	0
		push	0
		push	0x1E
		mov		eax, ds:[0x11F9508]
		mov		[ebp-0xE8], eax
		push	eax
		mov		ecx, ds:[0x11F91A8]
		mov		eax, 0xB6E110
		call	eax
		test	eax, eax
		jz		done
		push	eax
		lea		ecx, [ebp-0x10]
		mov		eax, 0x66B0D0
		call	eax
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
		mov		eax, 0x54EE20
		call	eax
		mov		[ebp-0xE0], al
		push	0
		mov		eax, 0x54EE60
		call	eax
		pop		ecx
		mov		ecx, [ebp-0x10]
		cmp		[ebp-0xDF], 0
		jz		bgnScnAlt
		mov		eax, 0xB6B260
		call	eax
		push	eax
		push	7
		mov		eax, 0xB6B8D0
		call	eax
		jmp		doneBgnScn
	bgnScnAlt:
		push	7
		push	ecx
		mov		eax, 0x54EDE0
		call	eax
	doneBgnScn:
		add		esp, 8
		push	0x280
		mov		eax, 0xAA13E0
		call	eax
		pop		ecx
		push	0x2F7
		push	1
		push	0x63
		mov		ecx, eax
		mov		eax, 0xB660D0
		call	eax
		push	eax
		lea		ecx, [ebp-0xF4]
		mov		eax, 0x633C90
		call	eax
		mov		eax, [ebp-0xF0]
		mov		ecx, [ebp-0xF4]
		mov		[ecx+0x194], eax
		push	ecx
		lea		ecx, [ebp-0x18]
		mov		eax, 0x66B0D0
		call	eax
		mov		ecx, [ebp-0xF4]
		xor		eax, eax
		mov		edx, 0xC
		cmp		byte ptr [ebp-0xDD], 0
		cmovnz	eax, edx
		mov		[ecx+0x19C], eax
		push	3
		lea		ecx, [ebp-0xDC]
		mov		eax, 0xC4F270
		call	eax
		lea		edx, [ebp-0xDC]
		push	edx
		push	dword ptr [ebp-0xF0]
		push	dword ptr [ebp+8]
		mov		eax, 0xB6BEE0
		call	eax
		add		esp, 0xC
		lea		ecx, [ebp-0xDC]
		mov		eax, 0xC4F2D0
		call	eax
		push	6
		mov		ecx, [ebp-0x100]
		mov		eax, 0x456FC0
		call	eax
		mov		[ebp-0x104], eax
		test	eax, eax
		jz		noNode6
		push	1
		lea		ecx, [ebp-0xDC]
		mov		eax, 0xC4F270
		call	eax
		lea		edx, [ebp-0xDC]
		push	edx
		push	dword ptr [ebp-0x104]
		push	dword ptr [ebp+8]
		mov		eax, 0xB6BEE0
		call	eax
		add		esp, 0xC
		lea		ecx, [ebp-0xDC]
		mov		eax, 0xC4F2D0
		call	eax
	noNode6:
		push	0
		push	dword ptr [ebp-0xF4]
		push	dword ptr [ebp+8]
		mov		eax, 0xB6C0D0
		call	eax
		add		esp, 0xC
		push	0
		lea		ecx, [ebp-0x18]
		mov		eax, 0x66B0D0
		call	eax
		push	0
		lea		ecx, [ebp-0xF4]
		mov		eax, 0x66B0D0
		call	eax
		mov		eax, 0xB6B790
		mov		ecx, 0xB6B840
		cmp		byte ptr [ebp-0xDF], 0
		cmovz	eax, ecx
		call	eax
		movzx	eax, byte ptr [ebp-0xE0]
		push	eax
		mov		eax, 0x54EE60
		call	eax
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
		mov		eax, 0x45CEC0
		call	eax
	done:
		lea		eax, [ebp-0x10]
		push	eax
		mov		ecx, [ebp+0xC]
		mov		eax, 0x6E5CC0
		call	eax
		lea		ecx, [ebp-0x10]
		mov		eax, 0x45CEC0
		call	eax
		lea		ecx, [ebp-0xDC]
		mov		eax, 0x4A0F60
		call	eax
		lea		ecx, [ebp-0xE4]
		mov		eax, 0x404EE0
		call	eax
		mov		esp, ebp
		pop		ebp
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

bool s_updateMapMarkers = false;

__declspec(naked) void DiscoverLocationHook()
{
	__asm
	{
		mov		s_updateMapMarkers, 1
		mov		eax, 0x11D0368
		mov		edx, 0x779571
		jmp		edx
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
		mov		edx, 0x60F145
		jmp		edx
	}
}

bool s_updateFogOfWar = false;

__declspec(naked) bool __stdcall UpdateSeenBitsHook(NiVector3 *posVector1, NiVector3 *posVector2)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x10
		push	ebx
		push	esi
		push	edi
		lea		ebx, [ecx+4]
		mov		eax, [ebp+8]
		mov		ecx, [ebp+0xC]
		fld		dword ptr [ecx]
		fsub	dword ptr [eax]
		fistp	dword ptr [ebp-8]
		fld		dword ptr [ecx+4]
		fsub	dword ptr [eax+4]
		fistp	dword ptr [ebp-0xC]
		xor		esi, esi
		mov		[ebp-4], esi
	iter1Head:
		xor		edi, edi
		imul	eax, esi, 0x100
		add		eax, [ebp-0xC]
		imul	eax, eax
		mov		[ebp-0x10], eax
		jmp		iter2Head
		and		esp, 0xEFFFFFFF
	iter2Head:
		mov		ecx, esi
		and		ecx, 1
		shl		ecx, 4
		add		ecx, edi
		mov		edx, 1
		shl		edx, cl
		mov		eax, esi
		shr		eax, 1
		lea		ecx, [ebx+eax*4]
		test	dword ptr [ecx], edx
		jnz		doInc
		imul	eax, edi, 0x100
		add		eax, [ebp-8]
		imul	eax, eax
		add		eax, [ebp-0x10]
		cmp		eax, 0x100000
		ja		iter2Next
		or		dword ptr [ecx], edx
		mov		s_updateFogOfWar, 1
	doInc:
		inc		dword ptr [ebp-4]
	iter2Next:
		inc		edi
		cmp		edi, 0x10
		jb		iter2Head
		inc		esi
		cmp		esi, 0x10
		jb		iter1Head
		cmp		dword ptr [ebp-4], 0xF8
		setnl	al
		pop		edi
		pop		esi
		pop		ebx
		mov		esp, ebp
		pop		ebp
		retn	8
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
		mov		edx, 0xE7D144
		jmp		edx
	}
}

TileRect *s_localMapRect = NULL, *s_worldMapRect, *s_mapMarkersRect, *s_doorMarkersRect, *s_lQuestMarkersRect, *s_wQuestMarkersRect;
TileImage *s_worldMapTile;
Tile::Value *s_miniMapMode, *s_pcMarkerRotate, *s_miniMapPosX, *s_miniMapPosY, *s_worldMapZoom;
TileShaderProperty *s_tileShaderProps[9];
bool s_defaultGridSize;
ShadowSceneNode *g_shadowSceneNode;
BSFogProperty *g_shadowFogProperty;
BSParticleSystemManager *g_particleSysMngr;

struct MapMarkerInfo
{
	TESObjectREFR				*refr;
	ExtraMapMarker::MarkerData	*data;
	float						posX;
	float						posY;

	MapMarkerInfo(TESObjectREFR *_refr, ExtraMapMarker::MarkerData *_data, CoordXY &posXY) :
		refr(_refr), data(_data), posX(posXY.x), posY(posXY.y) {}
	MapMarkerInfo(const MapMarkerInfo &copyFrom) {*this = copyFrom;}
};
typedef Vector<MapMarkerInfo> CellMapMarkers;
typedef UnorderedMap<UInt32, CellMapMarkers> WorldMapMarkers;
UnorderedMap<TESWorldSpace*, WorldMapMarkers> s_worldMapMarkers;

bool Cmd_InitMiniMap_Execute(COMMAND_ARGS)
{
	if (s_localMapRect || (s_miniMapIndex == 0xFF) || (scriptObj->modIndex != s_miniMapIndex))
		return true;

	Tile *tile = g_HUDMainMenu->tile->GetComponentTile("JIPMiniMap");
	if (!tile) return true;

	if (tile->GetValueFloat(kTileValue_user0) < 1.4F)
	{
		*ShowMessageBox_pScriptRefID = scriptObj->refID;
		*ShowMessageBox_button = 0xFF;
		ShowMessageBox("Please be advised:\n\nJIP MiniMap versions older than 1.4 will NOT work with JIP LN NVSE.\n\nYou MUST update JIP MiniMap to the latest version.", MSGBOX_ARGS);
		return true;
	}

	s_miniMapScale = tile->GetValue(kTileValue_user1);
	DListNode<Tile> *node = tile->children.Tail();
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
	UVCoord *shapeUVCoords = (UVCoord*)GameHeapAlloc(sizeof(UVCoord) * 289);
	NiVector3 vertex(0, 0, 0), normal(0, 1, 0);
	UVCoord uvCoord(0, 0);
	UInt32 index;
	for (int iterX = 0; iterX < 17; iterX++)
	{
		index = iterX;
		for (int iterZ = 0; iterZ < 17; iterZ++, index += 17)
		{
			shapeVertices[index] = vertex;
			vertex.x += 8;
			shapeNormals[index] = normal;
			shapeUVCoords[index] = uvCoord;
			uvCoord.x += 0.0625;
		}
		vertex.x = 0;
		vertex.z -= 8;
		uvCoord.x = 0;
		uvCoord.y += 0.0625;
	}

	NiTriangle *shapeTriangles = GenerateTriangles();

	NiAlphaProperty *alphaProp = (NiAlphaProperty*)NiAllocator(sizeof(NiAlphaProperty));
	ThisCall(0x4391C0, alphaProp);
	alphaProp->flags = 0x8D;

	TileImage *localTile;
	BSScissorTriShape *sciTriShp;
	NiTriShapeData *shapeData;
	for (index = 0; index < 9; index++)
	{
		localTile = (TileImage*)node->data;
		node = node->prev;
		sciTriShp = (BSScissorTriShape*)localTile->node->m_children.data[0];
		s_localMapShapes[index] = sciTriShp;
		shapeData = (NiTriShapeData*)sciTriShp->geometryData;
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

		NiReleaseThenAdd(&sciTriShp->m_propertyList.Head()->data, alphaProp);
		sciTriShp->alphaProp = alphaProp;
	}
	UpdateTileScales();

	s_doorMarkersRect = (TileRect*)node->data;
	s_lQuestMarkersRect = (TileRect*)node->prev->data;
	node = s_worldMapRect->children.Tail();
	s_worldMapTile = (TileImage*)node->data;
	s_mapMarkersRect = (TileRect*)node->prev->data;
	s_wQuestMarkersRect = (TileRect*)node->prev->prev->data;

	TESForm *markerBase = LookupFormByRefID(0x10);
	ListNode<TESWorldSpace> *worldIter = g_dataHandler->worldSpaceList.Head();
	ListNode<TESObjectREFR> *refrIter;
	TESWorldSpace *worldSpc, *rootWorld, *lastRoot = NULL;
	WorldMapMarkers *worldMarkers;
	TESObjectREFR *markerRef;
	ExtraMapMarker *xMarker;
	CoordXY posXY;
	Coordinate coord;
	do
	{
		worldSpc = worldIter->data;
		if (!worldSpc || !worldSpc->cell) continue;
		GetWorldPosMods(worldSpc);
		rootWorld = worldSpc->GetRootMapWorld();
		if (lastRoot != rootWorld)
		{
			lastRoot = rootWorld;
			GetWorldDimensions(rootWorld);
			worldMarkers = NULL;
		}
		refrIter = worldSpc->cell->objectList.Head();
		do
		{
			markerRef = refrIter->data;
			if (!markerRef || (markerRef->baseForm != markerBase)) continue;
			xMarker = GetExtraType(&markerRef->extraDataList, MapMarker);
			if (!xMarker || !xMarker->data) continue;
			AdjustWorldPos(markerRef->PosXY(), &posXY);
			coord.x = lfloor(posXY.x / 4096);
			coord.y = lfloor(posXY.y / 4096);
			GetWorldMapPosMults(&posXY);
			if (!worldMarkers)
				worldMarkers = &s_worldMapMarkers[rootWorld];
			(*worldMarkers)[coord.xy].Append(markerRef, xMarker->data, posXY);
		}
		while (refrIter = refrIter->next);
	}
	while (worldIter = worldIter->next);

	s_defaultDoor = *(TESObjectDOOR**)0x11CA258;
	s_blackPlaneBase = LookupFormByRefID(0x15A1F2);
	s_defaultGridSize = *(UInt8*)0x11C63D0 <= 5;
	g_shadowSceneNode = *(ShadowSceneNode**)0x11F91C8;
	g_shadowFogProperty = *(BSFogProperty**)0x11DEB00;
	g_particleSysMngr = *(BSParticleSystemManager**)0x11DED58;
	SafeWrite16(0x452736, 0x7705);
	WriteRelJump(0x54EF70, (UInt32)ExtCaptureBoundsHook);
	//WriteRelCall(0x54F257, (UInt32)HideExtCellLandNodeHook);
	WriteRelJump(0x54F688, (UInt32)IntCaptureBoundsHook);
	WriteRelJump(0x54E830, (UInt32)GenerateRenderedTextureHook);
	SafeWrite32(0xB6D0C0, (UInt32)D3DFormatHook);
	WriteRelJump(0x779567, (UInt32)DiscoverLocationHook);
	WriteRelJump(0x60F13D, (UInt32)SetQuestTargetsHook);
	WriteRelJump(0x879AD0, (UInt32)UpdateSeenBitsHook);
	WriteRelJump(0xE7D13A, (UInt32)RendererRecreateHook);
	return true;
}

typedef Vector<Tile*> DynamicTiles;
typedef UnorderedMap<UInt32, DynamicTiles> RenderedMapMarkers;

void __fastcall DestroyCellMarkers(RenderedMapMarkers::Iterator &cmkIter)
{
	for (auto dtlIter = cmkIter().Begin(); dtlIter; ++dtlIter)
		dtlIter->Destroy(true);
	cmkIter.Remove();
}

alignas(16) const UInt32
kDirectionalLightValues[] = {0x3F59D9DA, 0x3F59D9DA, 0x3F59D9DA, 0x3E4CCCCD, 0x3E4CCCCD, 0x3E4CCCCD, 0, 0, 0, 0, 0xBFC90FDB, 0, 0},
kFogPropertyValues[] = {0x3DF8F8F9, 0x3E3CBCBD, 0x3E7CFCFD, 0x48F42400, 0x48F42400};
const UInt8
kSelectImgUpdate[4][9] =
{
	{8, 2, 0, 4, 1, 0, 0, 0, 0},
	{0, 0, 0, 8, 2, 0, 4, 1, 0},
	{0, 8, 2, 0, 4, 1, 0, 0, 0},
	{0, 0, 0, 0, 8, 2, 0, 4, 1}
},
kSelectFOWUpdate[4][4] =
{
	{0, 1, 3, 4}, {3, 4, 6, 7},
	{1, 2, 4, 5}, {4, 5, 7, 8}
};
const char kGridAdjustCoord[9][2] =
{
	{-1, -1}, {-1, 0}, {-1, 1},
	{0, -1}, {0, 0}, {0, 1},
	{1, -1}, {1, 0}, {1, 1}
};

struct ExteriorEntry
{
	NiRenderedTexture	*texture;
	UInt32				regenFlags;

	ExteriorEntry() : texture(NULL), regenFlags(0) {}
};

struct QuestMarkerPos
{
	Tile::Value		*x;
	Tile::Value		*y;
};

UInt32 *g_lightingPasses = (UInt32*)0x11F91D8, s_currentMapMode = 0, s_targetsUpdDelay = 0;
TESWorldSpace *s_pcCurrWorld = NULL;
Coordinate s_currLocalCoords(0x7FFF, 0x7FFF), s_currWorldCoords(0x7FFF, 0x7FFF);
const char **g_mapMarkerIcons = (const char**)0x11A0404;
RenderedMapMarkers s_renderedMapMarkers(0x40);
UnorderedSet<UInt32> s_currMarkerCells(0x40);
UnorderedMap<UInt32, ExteriorEntry> s_renderedExteriors(0x80);
Vector<UInt32> s_exteriorKeys(0x60);
UnorderedMap<UInt32, NiRenderedTexture*> s_renderedInterior(0x20);
UnorderedMap<TESObjectCELL*, DoorRefsList> s_exteriorDoorRefs(0x20);
DoorRefsList s_doorRefsList(0x20);
Set<TESObjectCELL*> s_currCellsSet(9);
TESQuest *s_activeQuest = NULL;
UnorderedMap<TESObjectREFR*, QuestMarkerPos> s_questMarkers(0x10);
TESObjectCELL *s_currCellGrid[9] = {}, *s_lastInterior = NULL;
UInt8 s_useFogOfWar = 0, s_currCellQuad = 0;
int s_markerRange = 0;

bool Cmd_UpdateMiniMap_Execute(COMMAND_ARGS)
{
	UInt32 updateType, showFlags = 0x100;
	if (!ExtractArgs(EXTRACT_ARGS, &updateType, &showFlags) || !s_localMapRect || (scriptObj->modIndex != s_miniMapIndex))
		return true;

	if (updateType == 3)
	{
		UpdateTileScales();
		s_regenTextures = true;
		if (showFlags == 0x100)
			return true;
	}

	TESObjectCELL *parentCell = g_thePlayer->parentCell;
	if (!parentCell) return true;
	TESWorldSpace *parentWorld = parentCell->worldSpace;
	Tile *markerTile;
	TESObjectREFR *objectRef;
	CoordXY posMult, adjustedPos;
	Coordinate coord;
	int swX, swY, depth = 18;

	if (s_currentMapMode != updateType)
	{
		if (updateType >= 2)
			s_regenTextures = true;
		else
		{
			s_currentMapMode = updateType;
			s_miniMapMode->SetFloat(updateType);
		}
		s_updateQuestTargets = true;
		s_pcCurrCell = NULL;
	}

	bool updateTiles = s_pcCurrCell != parentCell;
	if (updateTiles)
	{
		s_pcCurrCell = parentCell;
		GetNorthRotation(parentCell);
	}

	if (s_useFogOfWar != (showFlags >> 4))
	{
		s_useFogOfWar = !s_useFogOfWar;
		for (TileShaderProperty *shaderProp : s_tileShaderProps)
			shaderProp->hasVtxColors = s_useFogOfWar;
		if (s_useFogOfWar)
		{
			float alpha = s_tileShaderProps[0]->alpha;
			s_vertexAlphaLevel[1] = alpha * 0.25;
			s_vertexAlphaLevel[2] = alpha * 0.5;
			s_vertexAlphaLevel[3] = alpha * 0.75;
			s_vertexAlphaLevel[4] = alpha;
			updateTiles = true;
		}
	}

	if (s_currentMapMode)
	{
		objectRef = g_thePlayer;
		if (!parentWorld)
		{
			objectRef = g_thePlayer->lastExteriorDoor;
			if (!objectRef) return true;
			parentWorld = objectRef->GetParentWorld();
			if (!parentWorld) return true;
		}

		TESWorldSpace *rootWorld = parentWorld->GetRootMapWorld();
		if (s_pcRootWorld != rootWorld)
		{
			s_pcRootWorld = rootWorld;
			GetWorldDimensions(rootWorld);
			const char *ddsPath = rootWorld->texture.ddsPath.m_data;
			if (!ddsPath) ddsPath = "jazzisparis\\minimap\\blanktile.dds";
			s_worldMapTile->SetString(kTileValue_filename, ddsPath);
			s_worldMapRect->SetFloat(kTileValue_user1, rootWorld->mapData.usableDimensions.X);
			s_worldMapRect->SetFloat(kTileValue_user2, rootWorld->mapData.usableDimensions.Y);
			s_markerRange = lceil(60.0 / rootWorld->mapData.usableDimensions.X *
				(rootWorld->mapData.cellSECoordinates.X - rootWorld->mapData.cellNWCoordinates.X +
				rootWorld->mapData.cellNWCoordinates.Y - rootWorld->mapData.cellSECoordinates.Y));
			updateTiles = true;
			s_updateMapMarkers = true;
		}

		if (s_pcCurrWorld != parentWorld)
		{
			s_pcCurrWorld = parentWorld;
			GetWorldPosMods(parentWorld);
		}

		AdjustWorldPos(objectRef->PosXY(), &posMult);
		coord.x = lfloor(posMult.x / 4096);
		coord.y = lfloor(posMult.y / 4096);
		GetWorldMapPosMults(&posMult);

		if (s_currWorldCoords != coord)
		{
			s_currWorldCoords = coord;
			updateTiles = true;
		}

		if (!(showFlags & 1))
		{
			if (!s_renderedMapMarkers.Empty())
			{
				s_mapMarkersRect->DestroyAllChildren();
				s_renderedMapMarkers.Clear();
			}
		}
		else if (updateTiles || s_updateMapMarkers)
		{
			if (s_updateMapMarkers)
			{
				s_updateMapMarkers = false;
				s_mapMarkersRect->DestroyAllChildren();
				s_renderedMapMarkers.Clear();
			}
			s_currMarkerCells.Clear();
			WorldMapMarkers *worldMarkers = s_worldMapMarkers.GetPtr(rootWorld);
			if (worldMarkers)
			{
				int range = lceil(s_markerRange / s_worldMapZoom->num);
				CellMapMarkers *mapMarkers;
				DynamicTiles *dynamicTiles;
				coord.x -= range;
				for (int iterX = -range; iterX <= range; iterX++, coord.x++)
				{
					coord.y = s_currWorldCoords.y - range;
					for (int iterY = -range; iterY <= range; iterY++, coord.y++)
					{
						mapMarkers = worldMarkers->GetPtr(coord.xy);
						if (!mapMarkers) continue;
						s_currMarkerCells.Insert(coord.xy);
						if (!s_renderedMapMarkers.Insert(coord.xy, &dynamicTiles)) continue;
						for (auto mkIter = mapMarkers->Begin(); mkIter; ++mkIter)
						{
							if (!(mkIter().data->flags & 1) || mkIter().refr->GetDisabled()) continue;
							markerTile = g_HUDMainMenu->AddTileFromTemplate(s_mapMarkersRect, "MiniMapWorldMarkerTemplate", 0);
							dynamicTiles->Append(markerTile);
							if (mkIter().data->flags & 2)
								markerTile->SetString(kTileValue_filename, g_mapMarkerIcons[mkIter().data->type]);
							else
								markerTile->SetString(kTileValue_filename, (const char*)0x1075030);
							markerTile->SetFloat(kTileValue_user0, mkIter().posX);
							markerTile->SetFloat(kTileValue_user1, mkIter().posY);
							markerTile->SetFloat(kTileValue_depth, depth);
							if (depth >= 27) depth = 18;
							else depth++;
						}
					}
				}
			}
			for (auto cmkIter = s_renderedMapMarkers.Begin(); cmkIter; ++cmkIter)
				if (!s_currMarkerCells.HasKey(cmkIter.Key())) DestroyCellMarkers(cmkIter);
		}
	}
	else
	{
		if (s_regenTextures)
		{
			s_regenTextures = false;
			if (!s_exteriorKeys.Empty())
			{
				for (auto prgIter = s_renderedExteriors.Begin(); prgIter; ++prgIter)
					if (prgIter().texture) ThisCall(0x401970, prgIter().texture);
				s_renderedExteriors.Clear();
				s_exteriorKeys.Clear();
			}
			s_lastInterior = NULL;
			updateTiles = true;
		}

		const UInt8 *updateList;
		UInt8 quadrant = 0;
		UInt32 gridIdx, lightingPasses;
		NiNode *hideNode;
		NiPointLight *pntLight;

		if (parentWorld)
		{
			coord = GetPackedCoords(&g_TES->extCoordX);
			swX = (coord.x - 1) << 12;
			swY = (coord.y - 1) << 12;
			posMult.x = (g_thePlayer->posX - swX) / 12288.0;
			posMult.y = 1.0 - ((g_thePlayer->posY - swY) / 12288.0);

			if (posMult.x >= 0.5F) quadrant = 1;
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
				for (gridIdx = 0; gridIdx < 9; gridIdx++)
				{
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					s_packedCellCoords[gridIdx] = coord.xy;
					parentCell = parentWorld->cellMap->Lookup(coord.xy);
					s_currCellGrid[gridIdx] = parentCell;
					if (!parentCell) continue;
					s_currCellsSet.Insert(parentCell);
					if (hideNode = parentCell->Get3DNode(4))
						s_hiddenNodes.Append(hideNode);
					if (s_useFogOfWar) CalcVtxAlphaBySeenData(gridIdx);
				}
				s_doorMarkersRect->DestroyAllChildren();
				if (showFlags & 2)
				{
					DoorRefsList *listPtr;
					for (auto clsIter = s_currCellsSet.Begin(); clsIter; ++clsIter)
						if (s_exteriorDoorRefs.Insert(*clsIter, &listPtr))
							GetTeleportDoors(*clsIter, listPtr);
					s_doorRefsList.Clear();
					for (auto drlIter = s_exteriorDoorRefs.Begin(); drlIter; ++drlIter)
						if (s_currCellsSet.HasKey(drlIter.Key()))
							s_doorRefsList.Concatenate(drlIter());
						else drlIter.Remove();
				}
			}
			else if (s_useFogOfWar && s_updateFogOfWar)
			{
				updateList = kSelectFOWUpdate[quadrant];
				gridIdx = 4;
				do {
					CalcVtxAlphaBySeenData(*updateList++);
				} while (--gridIdx);
			}

			if (updateTiles || (s_defaultGridSize && (s_currCellQuad != quadrant)))
			{
				s_currCellQuad = quadrant;
				if (showFlags & 8)
				{
					s_useAltFormat = 2;
					for (DListNode<LightingData> *lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
					{
						pntLight = (NiPointLight*)lgtNode->data->light;
						if (pntLight && IS_TYPE(pntLight, NiPointLight))
						{
							pntLight->fltE4 = pntLight->radius;
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

				updateList = kSelectImgUpdate[quadrant];
				ExteriorEntry *exteriorEntry;
				for (gridIdx = 0; gridIdx < 9; gridIdx++)
				{
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

				if (s_exteriorKeys.Size() > CACHED_TEXTURES_MAX)
				{
					UInt32 count = s_exteriorKeys.Size() - CACHED_TEXTURES_PURGED, size = count;
					auto findTex = s_renderedExteriors.Begin();
					while (count--)
					{
						findTex.Find(s_exteriorKeys[count]);
						if (!findTex) continue;
						if (findTex().texture)
							ThisCall(0x401970, findTex().texture);
						findTex.Remove();
					}
					s_exteriorKeys.RemoveRange(0, size);
				}
			}
		}
		else
		{
			AdjustInteriorPos(g_thePlayer->PosXY(), &adjustedPos);
			coord = GetInteriorCoord(&adjustedPos);
			swX = (coord.x - 1) << 12;
			swY = (coord.y - 1) << 12;
			posMult.x = (adjustedPos.x - swX) / 12288.0;
			posMult.y = 1.0 - ((adjustedPos.y - swY) / 12288.0);

			if (updateTiles)
			{
				s_hiddenNodes.Clear();
				if (hideNode = parentCell->Get3DNode(4))
					s_hiddenNodes.Append(hideNode);
				GetBlackPlaneNodes(parentCell);
				if (showFlags & 2)
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
						if (*clrIter) ThisCall(0x401970, *clrIter);
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
				for (gridIdx = 0; gridIdx < 9; gridIdx++)
				{
					coord = s_currLocalCoords + kGridAdjustCoord[gridIdx];
					if (s_renderedInterior.Insert(coord.xy, &renderedTexture))
					{
						*renderedTexture = NULL;
						ThisCall(0x54E750, parentCell, (SInt32)coord.x, (SInt32)coord.y, renderedTexture);
					}
					s_tileShaderProps[gridIdx]->srcTexture = *renderedTexture;
					if (s_useFogOfWar)
					{
						s_packedCellCoords[gridIdx] = coord.xy;
						CalcVtxAlphaBySeenData(gridIdx);
					}
				}
				s_doorMarkersRect->DestroyAllChildren();
			}
			else if (s_useFogOfWar && s_updateFogOfWar)
			{
				if (posMult.x >= 0.5F) quadrant = 1;
				if (posMult.y < 0.5F) quadrant |= 2;
				updateList = kSelectFOWUpdate[quadrant];
				gridIdx = 4;
				do {
					CalcVtxAlphaBySeenData(*updateList++);
				} while (--gridIdx);
			}
		}

		if (s_useAltFormat == 2)
		{
			if (parentWorld)
			{
				for (DListNode<LightingData> *lgtNode = g_shadowSceneNode->lgtList0B4.Head(); lgtNode; lgtNode = lgtNode->next)
				{
					pntLight = (NiPointLight*)lgtNode->data->light;
					if (pntLight && IS_TYPE(pntLight, NiPointLight))
						pntLight->radius = pntLight->fltE4;
				}
			}
			else *g_lightingPasses = lightingPasses;
			g_particleSysMngr->m_flags &= 0xFFFFFFFE;
			for (auto hdnIter = s_hiddenNodes.Begin(); hdnIter; ++hdnIter)
				hdnIter->m_flags &= 0xFFFFFFFE;
		}
		s_useAltFormat = 0;
		s_updateFogOfWar = false;

		if (updateTiles && (showFlags & 2))
		{
			for (auto doorIter = s_doorRefsList.Begin(); doorIter; ++doorIter)
			{
				objectRef = *doorIter;
				if (parentWorld) adjustedPos = *objectRef->PosXY();
				else AdjustInteriorPos(objectRef->PosXY(), &adjustedPos);
				markerTile = g_HUDMainMenu->AddTileFromTemplate(s_doorMarkersRect, "MiniMapDoorMarkerTemplate", 0);
				markerTile->SetFloat(kTileValue_user0, (adjustedPos.x - swX) / 12288.0);
				markerTile->SetFloat(kTileValue_user1, 1.0 - ((adjustedPos.y - swY) / 12288.0));
				markerTile->SetFloat(kTileValue_depth, depth);
				/*if (!GetLinkedCellVisited(objectRef))
					markerTile->SetFloat(kTileValue_systemcolor, 1.0F);*/
				if (depth >= 27) depth = 18;
				else depth++;
			}
		}
	}

	if (showFlags & 4)
	{
		if (s_updateQuestTargets || (s_activeQuest != g_thePlayer->activeQuest))
		{
			s_updateQuestTargets = false;
			s_activeQuest = g_thePlayer->activeQuest;
			s_targetsUpdDelay = 0;
			s_lQuestMarkersRect->DestroyAllChildren();
			s_wQuestMarkersRect->DestroyAllChildren();
			s_questMarkers.Clear();
			depth = 29;

			ListNode<BGSQuestObjective::Target> *trgIter = g_thePlayer->questTargetList.Head();
			BGSQuestObjective::Target *targetData;
			QuestMarkerPos *markerPos;
			TileRect *targetsRect = s_currentMapMode ? s_wQuestMarkersRect : s_lQuestMarkersRect;
			do
			{
				targetData = trgIter->data;
				if (!targetData) continue;
				if (targetData->data.teleportLinks.size)
					objectRef = targetData->data.teleportLinks.data[0].door;
				else
				{
					objectRef = targetData->target;
					if (!objectRef || ((objectRef->parentCell != s_pcCurrCell) && (!parentWorld || (parentWorld != objectRef->GetParentWorld()))))
						continue;
				}
				if (!s_questMarkers.Insert(objectRef, &markerPos)) continue;
				markerTile = g_HUDMainMenu->AddTileFromTemplate(targetsRect, "MiniMapQuestMarkerTemplate", 0);
				markerPos->x = markerTile->GetValue(kTileValue_user0);
				markerPos->y = markerTile->GetValue(kTileValue_user1);
				markerTile->SetFloat(kTileValue_depth, depth);
				if (depth >= 38) depth = 29;
				else depth++;
			}
			while (trgIter = trgIter->next);
		}

		if (!s_targetsUpdDelay || updateTiles)
		{
			s_targetsUpdDelay = QUEST_TARGETS_POS_UPD_FREQ;
			for (auto mkrIter = s_questMarkers.Begin(); mkrIter; ++mkrIter)
			{
				objectRef = mkrIter.Key();
				if (s_currentMapMode)
				{
					AdjustWorldPos(objectRef->PosXY(), &adjustedPos);
					GetWorldMapPosMults(&adjustedPos);
				}
				else
				{
					if (parentWorld) adjustedPos = *objectRef->PosXY();
					else AdjustInteriorPos(objectRef->PosXY(), &adjustedPos);
					adjustedPos.x = (adjustedPos.x - swX) / 12288.0;
					adjustedPos.y = 1.0 - ((adjustedPos.y - swY) / 12288.0);
				}
				mkrIter().x->SetFloat(adjustedPos.x);
				mkrIter().y->SetFloat(adjustedPos.y);
			}
		}
		else s_targetsUpdDelay--;
	}
	else if (!s_questMarkers.Empty())
	{
		s_questMarkers.Clear();
		s_lQuestMarkersRect->DestroyAllChildren();
		s_wQuestMarkersRect->DestroyAllChildren();
	}

	s_pcMarkerRotate->SetFloat(s_cellNorthRotation - g_thePlayer->rotZ);
	s_miniMapPosX->SetFloat(posMult.x);
	s_miniMapPosY->SetFloat(posMult.y);
	return true;
}