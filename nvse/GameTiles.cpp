#pragma once

#include "nvse/GameTiles.h"

typedef NiTMap<const char*, UInt32> TraitNameMap;
TraitNameMap *g_traitNameMap = (TraitNameMap*)0x11F32F4;

UInt32 __fastcall Tile::TraitNameToID(const char *traitName)
{
	return ((TraitNameMap*)0x11F32F4)->Lookup(traitName);
}

__declspec(naked) TileValue* __fastcall Tile::GetValue(UInt32 typeID) const
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, [ecx+0x14]
		xor		esi, esi
		mov		edi, [ecx+0x18]
		ALIGN 16
	iterHead:
		cmp		esi, edi
		jz		iterEnd
		lea		ecx, [esi+edi]
		shr		ecx, 1
		mov		eax, [ebx+ecx*4]
		cmp		[eax], edx
		jb		isLT
		jz		done
		mov		edi, ecx
		jmp		iterHead
		ALIGN 16
	isLT:
		lea		esi, [ecx+1]
		jmp		iterHead
	iterEnd:
		xor		eax, eax
	done:
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

TileValue* __fastcall Tile::GetValueName(const char *valueName) const
{
	return GetValue(TraitNameToID(valueName));
}

DList<Tile>::Node* __fastcall Tile::GetNthChild(UInt32 index) const
{
	auto result = children.Tail();
	while (result && index)
	{
		index--;
		result = result->prev;
	}
	return result;
}

void Tile::GetComponentFullName(char *resStr)
{
	Tile *parents[24], *pTile = this;
	UInt32 count = 0;
	while (true)
	{
		parents[count++] = pTile;
		if IS_TYPE(pTile, TileMenu) break;
		pTile = pTile->parent;
	}
	while (true)
	{
		pTile = parents[--count];
		char *pName = pTile->name.m_data;
		UInt32 length = pTile->name.m_dataLen;
		COPY_BYTES(resStr, pName, length);
		resStr += length;
		DList<Tile>::Node *node = pTile->parent->children.Tail();
		while (node->data != pTile)
			node = node->prev;
		int index = 0;
		while ((node = node->prev) && !StrCompareCS(node->data->name.m_data, pName))
			index++;
		if (index)
		{
			*resStr++ = ':';
			resStr = IntToStr(resStr, index);
		}
		if (!count) break;
		*resStr++ = '/';
	}
	*resStr = 0;
}

Menu *Tile::GetParentMenu() const
{
	auto tile = this;
	do
	{
		if IS_TYPE(tile, TileMenu)
			return ((TileMenu*)tile)->menu;
	}
	while (tile = tile->parent);
	return nullptr;
}

__declspec(naked) void __fastcall Tile::PokeValue(UInt32 valueID)
{
	__asm
	{
		call	Tile::GetValue
		test	eax, eax
		jz		done
		push	eax
		mov		dl, 1
		mov		ecx, eax
		call	TileValue::SetBool
		xor		dl, dl
		pop		ecx
		jmp		TileValue::SetBool
	done:
		retn
	}
}

__declspec(naked) void Tile::DestroyAllChildren()
{
	__asm
	{
		push	esi
		mov		esi, [ecx+4]
		ALIGN 16
	iterHead:
		test	esi, esi
		jz		done
		mov		ecx, [esi+8]
		mov		esi, [esi]
		test	ecx, ecx
		jz		iterHead
		push	1
		mov		eax, [ecx]
		call	dword ptr [eax]
		jmp		iterHead
	done:
		pop		esi
		retn
	}
}

Tile *Tile::GetChild(const char *childName)
{
	int childIndex = 0;
	char *colon = FindChr(childName, ':');
	if (colon)
	{
		if (colon == childName) return nullptr;
		*colon = 0;
		childIndex = StrToInt(colon + 1);
	}
	Tile *result = nullptr;
	bool wildcard = *childName == '*';
	for (auto node = children.Begin(); node; ++node)
	{
		if (*node && (wildcard || !StrCompareCI(node->name.m_data, childName)) && !childIndex--)
		{
			result = *node;
			break;
		}
	}
	if (colon) *colon = ':';
	return result;
}

Tile *Tile::GetComponent(const char *componentPath, const char **trait)
{
	Tile *parentTile = this;
	while (char *slashPos = SlashPos(componentPath))
	{
		*slashPos = 0;
		parentTile = parentTile->GetChild(componentPath);
		if (!parentTile) return nullptr;
		componentPath = slashPos + 1;
	}
	if (*componentPath)
	{
		Tile *result = parentTile->GetChild(componentPath);
		if (result) return result;
		*trait = componentPath;
	}
	return parentTile;
}

TileValue *Tile::GetComponentValue(const char *componentPath)
{
	const char *trait = nullptr;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && trait) ? tile->GetValueName(trait) : nullptr;
}

Tile *Tile::GetComponentTile(const char *componentPath)
{
	const char *trait = nullptr;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && !trait) ? tile : nullptr;
}

__declspec(naked) void __vectorcall Tile::Value::SetFloat(float value)
{
	__asm
	{
		comiss	xmm0, [ecx+8]
		jz		done
		movss	[ecx+8], xmm0
		push	1
		CALL_EAX(0xA09410)
	done:
		retn
	}
}

__declspec(naked) void __fastcall Tile::Value::SetBool(bool value)
{
	__asm
	{
		and		edx, 1
		neg		edx
		and		edx, 0x3F800000
		cmp		[ecx+8], edx
		jz		done
		mov		[ecx+8], edx
		push	1
		CALL_EAX(0xA09410)
	done:
		retn
	}
}

__declspec(naked) void Tile::Value::SetString(const char *strVal)
{
	__asm
	{
		push	esi
		mov		esi, ecx
		mov		ecx, [ecx+0xC]
		mov		edx, [esp+8]
		test	edx, edx
		jz		noSource
		test	ecx, ecx
		jz		doCopy
		call	StrCompareCS
		test	al, al
		jnz		doFree
		pop		esi
		retn	4
	noSource:
		test	ecx, ecx
		jz		done
	doFree:
		push	dword ptr [esi+0xC]
		call	Game_HeapFree
		and		dword ptr [esi+0xC], 0
		mov		edx, [esp+8]
		test	edx, edx
		jz		doRefresh
	doCopy:
		mov		ecx, edx
		call	CopyCString
		mov		[esi+0xC], eax
	doRefresh:
		push	1
		mov		ecx, esi
		CALL_EAX(0xA09410)
	done:
		pop		esi
		retn	4
	}
}

__declspec(naked) float __vectorcall Tile::GetValueFloat(UInt32 id) const
{
	__asm
	{
		call	Tile::GetValue
		test	eax, eax
		jz		done
		movss	xmm0, [eax+8]
		retn
	done:
		xorps	xmm0, xmm0
		retn
	}
}

__declspec(naked) void __vectorcall Tile::SetFloat(UInt32 id, float fltVal)
{
	__asm
	{
		call	Tile::GetValue
		test	eax, eax
		jz		done
		comiss	xmm0, [eax+8]
		jz		done
		movss	[eax+8], xmm0
		push	1
		mov		ecx, eax
		CALL_EAX(0xA09410)
	done:
		retn
	}
}

__declspec(naked) void __fastcall Tile::SetBool(UInt32 id, bool value)
{
	__asm
	{
		call	Tile::GetValue
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		neg		edx
		and		edx, 0x3F800000
		cmp		[eax+8], edx
		jz		done
		mov		[eax+8], edx
		mov		[esp+4], 1
		mov		ecx, eax
		JMP_EAX(0xA09410)
	done:
		retn	4
	}
}

__declspec(naked) void __fastcall Tile::SetString(UInt32 id, const char *strVal)
{
	__asm
	{
		call	Tile::GetValue
		test	eax, eax
		jz		done
		mov		ecx, eax
		jmp		Tile::Value::SetString
	done:
		retn	4
	}
}

Tile* __fastcall GetTargetComponent(const char *componentPath, TileValue **value)
{
	char *slashPos = SlashPos(componentPath);
	if (!slashPos)
		return GetMenuTile(componentPath);
	*slashPos = 0;
	Tile *component = GetMenuTile(componentPath);
	if (!component)
		return nullptr;
	const char *trait = nullptr;
	component = component->GetComponent(slashPos + 1, &trait);
	if (!component)
		return nullptr;
	if (trait)
	{
		if (!value || !(*value = component->GetValueName(trait)))
			return nullptr;
	}
	else if (value)
		return nullptr;
	return component;
}

__declspec(naked) void __fastcall TileImage::SetAlphaTexture(const char *ddsPath)
{
	__asm
	{
		mov		ecx, [ecx+0x40]
		test	ecx, ecx
		jz		done
		push	ecx
		mov		ecx, edx
		call	NiSourceTexture::Create
		pop		ecx
		push	eax
		add		ecx, 0x64
		push	ecx
		call	NiReplaceObject
	done:
		retn
	}
}

void Tile::Dump()
{
	PrintDebug("%08X\t%08X\t%08X\t%s", this, GetType(), node, name.m_data);
	s_debug().Indent();

	PrintDebug("Values:");

	s_debug().Indent();
	
	Value *value;
	const char *traitName;
	char traitID[9];
	for (auto iter = values.Begin(); iter; ++iter)
	{
		value = *iter;
		traitName = TraitIDToName(value->id);

		if (!traitName)
		{
			UIntToHex(traitID, value->id);
			traitName = traitID;
		}
		if (value->str)
			PrintDebug("%d  %s: %s", value->id, traitName, value->str);
		/*else if (value->action)
			PrintDebug("%d  %s: Action %08X", value->id, traitName, value->action);*/
		else
			PrintDebug("%d  %s: %.4f", value->id, traitName, value->num);
	}

	s_debug().Outdent();

	for (auto traverse = children.RBegin(); traverse; --traverse)
		if (*traverse) traverse->Dump();

	s_debug().Outdent();
}

// not a one-way mapping, so we just return the first
// also this is slow and sucks
const char *TraitIDToName(UInt32 id)
{
	for (auto iter = g_traitNameMap->Begin(); iter; ++iter)
		if (iter.Get() == id) return iter.Key();
	return nullptr;
}
