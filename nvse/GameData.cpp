#include "nvse/GameData.h"
#include "internal/jip_core.h"

__declspec(naked) ModInfo* __fastcall DataHandler::LookupModByName(const char *modName) const
{
	_asm
	{
		push	ebx
		lea		ebx, [ecx+0x21C]
		mov		ecx, edx
		call	StrHashCI
		mov		ecx, eax
		mov		edx, [ebx-4]
		ALIGN 16
	iterHead:
		mov		eax, [ebx]
		cmp		[eax+0x120], ecx
		jz		done
		add		ebx, 4
		dec		edx
		jnz		iterHead
		xor		eax, eax
	done:
		pop		ebx
		retn
	}
}

char kDecompilePath[0x100] = "DecompiledScripts (JIP)\\";

void DataHandler::DecompileModScripts(UInt8 modIdx, UInt8 typeMask)
{
	FileStream outFile;
	char *wBuffer = GetStrArgBuffer(), *mainEnd = StrCopy(kDecompilePath + 24, GetNthModName(modIdx)), *typeEnd;
	*mainEnd++ = '\\';

	if (typeMask & 1)
	{
		typeEnd = StrCopy(mainEnd, "Script\\");
		auto scrIter = scriptList.Head();
		do
		{
			if (auto pScript = scrIter->data; pScript && pScript->info.dataLength && (pScript->GetOverridingModIdx() == modIdx))
			{
				char *fileEnd = StrCopy(typeEnd, pScript->GetEditorID());
				*(UInt32*)fileEnd = 'txt.';
				fileEnd[4] = 0;
				if (outFile.OpenWriteEx(kDecompilePath, wBuffer, 0x10000))
				{
					size_t numWritten = DecompileToBuffer(pScript, outFile, nullptr);
					outFile.Close();
					if (!numWritten)
						remove(kDecompilePath);
				}
			}
		}
		while (scrIter = scrIter->next);
	}

	if (typeMask & 2)
	{
		typeEnd = StrCopy(mainEnd, "Quest\\");
		auto qstIter = questList.Head();
		do
		{
			if (auto pQuest = qstIter->data; pQuest && (pQuest->GetOverridingModIdx() == modIdx))
			{
				char *fileEnd = StrCopy(typeEnd, pQuest->GetEditorID());
				*(UInt32*)fileEnd = 'txt.';
				fileEnd[4] = 0;
				if (outFile.OpenWriteEx(kDecompilePath, wBuffer, 0x10000))
				{
					size_t numWritten = pQuest->DecompileResultScripts(outFile, nullptr);
					outFile.Close();
					if (!numWritten)
						remove(kDecompilePath);
				}
			}
		}
		while (qstIter = qstIter->next);
	}

	if (typeMask & 4)
	{
		typeEnd = StrCopy(mainEnd, "Package\\");
		auto pkgIter = packageList.Head();
		do
		{
			if (auto pPackage = pkgIter->data; pPackage && (pPackage->GetOverridingModIdx() == modIdx))
			{
				char *fileEnd = StrCopy(typeEnd, pPackage->GetEditorID());
				*(UInt32*)fileEnd = 'txt.';
				fileEnd[4] = 0;
				if (outFile.OpenWriteEx(kDecompilePath, wBuffer, 0x10000))
				{
					size_t numWritten = pPackage->DecompileResultScripts(outFile, nullptr);
					outFile.Close();
					if (!numWritten)
						remove(kDecompilePath);
				}
			}
		}
		while (pkgIter = pkgIter->next);
	}

	if (typeMask & 8)
	{
		typeEnd = StrCopy(mainEnd, "Dialogue\\");
		auto dlgIter = topicInfoList.Head();
		do
		{
			if (auto pTopic = dlgIter->data; pTopic && (pTopic->GetOverridingModIdx() == modIdx) && pTopic->parentTopic)
			{
				sprintf_s(typeEnd, 0x80, "%06X [%s].txt", pTopic->refID & 0xFFFFFF, pTopic->parentTopic->editorIDstr.CStr());
				if (outFile.OpenWriteEx(kDecompilePath, wBuffer, 0x10000))
				{
					size_t numWritten = pTopic->DecompileResultScripts(outFile, nullptr);
					outFile.Close();
					if (!numWritten)
						remove(kDecompilePath);
				}
			}
		}
		while (dlgIter = dlgIter->next);
	}

	if (typeMask & 0x10)
	{
		typeEnd = StrCopy(mainEnd, "Terminal\\");
		for (auto bndIter = boundObjectList->first; bndIter; bndIter = bndIter->next)
			if (IS_ID(bndIter, BGSTerminal) && (bndIter->GetOverridingModIdx() == modIdx))
				if (auto terminal = (BGSTerminal*)bndIter; !terminal->menuEntries.Empty())
				{
					char *fileEnd = StrCopy(typeEnd, terminal->GetEditorID());
					*(UInt32*)fileEnd = 'txt.';
					fileEnd[4] = 0;
					if (outFile.OpenWriteEx(kDecompilePath, wBuffer, 0x10000))
					{
						size_t numWritten = terminal->DecompileResultScripts(outFile, nullptr);
						outFile.Close();
						if (!numWritten)
							remove(kDecompilePath);
					}
				}
	}
}

void Sky::RefreshMoon()
{
	if (masserMoon) masserMoon->Destroy(true);
	masserMoon = ThisCall<Moon*>(0x634A70, Game_HeapAlloc<Moon>(), (const char*)0x104EEB0, GMST_FLT(fMasserAngleFadeStart), GMST_FLT(fMasserAngleFadeEnd),
		GMST_FLT(fMasserAngleShadowEarlyFade), GMST_FLT(fMasserSpeed), GMST_FLT(fMasserZOffset), GMST_INT(iMasserSize));
	masserMoon->Refresh(niNode008, (const char*)0x104EEB0);
}

__declspec(naked) bool Sky::GetIsRaining() const
{
	__asm
	{
		mov		eax, [ecx+0x10]
		test	eax, eax
		jz		checkSecond
		cmp		byte ptr [eax+0xEB], 4
		jz		weatherPerc
	checkSecond:
		mov		eax, [ecx+0x14]
		test	eax, eax
		jz		retnFalse
		cmp		byte ptr [eax+0xEB], 4
		jnz		retnFalse
	weatherPerc:
		movss	xmm0, PS_V3_One
		comiss	xmm0, [ecx+0xF4]
		setbe	al
		retn
	retnFalse:
		xor		al, al
		retn
	}
}

void __fastcall WaterSurfaceManager::Update(NiCamera *camera)
{
	if (!waterGroups.Empty())
	{
		*(bool*)0x11F91C5 = 1;
		ThisCall(0x4E21B0, this, camera, 0);
		*(bool*)0x11F91C5 = 0;
	}
}

__declspec(naked) void __fastcall WaterSurfaceManager::UpdateEx(NiCamera *camera)
{
	__asm
	{
		cmp		dword ptr [ecx+0x3C], 0
		jz		done
		mov		eax, 0x11C7B70
		push	dword ptr [eax]
		and		dword ptr [eax], 0
		push	dword ptr [eax+0x50]
		and		dword ptr [eax+0x50], 0
		push	eax
		push	0
		push	edx
		mov		byte ptr ds:0x11F91C5, 1
		CALL_EAX(0x4E21B0)
		mov		byte ptr ds:0x11F91C5, 0
		pop		eax
		pop		dword ptr [eax+0x50]
		pop		dword ptr [eax]
	done:
		retn
	}
}

GridCellArray *g_gridCellArray;

__declspec(naked) TESObjectCELL* __vectorcall GridCellArray::GetCellAtPos(__m128 pos) const
{
	__asm
	{
		pxor	xmm3, xmm3
		unpcklpd	xmm0, xmm3
		cvttps2dq	xmm0, xmm0
		psrad	xmm0, 0xC
		movq	xmm1, qword ptr [ecx+4]
		psubd	xmm0, xmm1
		movd	xmm1, [ecx+0xC]
		unpcklps	xmm1, xmm1
		movaps	xmm2, xmm1
		psrld	xmm2, 1
		paddd	xmm0, xmm2
		pcmpgtd	xmm1, xmm0
		movmskps	eax, xmm1
		cmp		al, 3
		jnz		retnNull
		movaps	xmm1, xmm0
		phaddd	xmm1, xmm3
		psrldq	xmm2, 4
		pslld	xmm0, xmm2
		paddd	xmm0, xmm1
		pextrw	edx, xmm0, 0
		mov		ecx, [ecx+0x10]
		mov		eax, [ecx+edx*4]
		retn
	retnNull:
		xor		eax, eax
		retn
	}
}

__declspec(naked) TESObjectCELL* __vectorcall GridCellArray::GetCellAtCoord(__m128i cellXY) const
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
		mov		ecx, [ebx+0x10]
		mov		eax, [ecx+edx*4]
		pop		ebx
		retn
	retnNull:
		xor		eax, eax
		pop		ebx
		retn
	}
}

void TES::UnloadBufferedExterior(TESObjectCELL *cell)
{
	UInt32 bufferSize = INIS_UINT(uExterior_Cell_Buffer_General);
	for (UInt32 i = 0; i < bufferSize; i++)
		if (!exteriorsBuffer[i])
		{
			bufferSize = i;
			break;
		}			
	for (UInt32 i = 0; i < bufferSize; i++)
	{
		if (exteriorsBuffer[i] != cell)
			continue;
		MemCopy(exteriorsBuffer + i, exteriorsBuffer + i + 1, (bufferSize - i - 1) << 2);
		ThisCall(0x462290, g_dataHandler, cell);
		break;
	}
}