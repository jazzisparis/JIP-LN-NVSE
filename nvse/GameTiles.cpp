#pragma once

#include "nvse/GameTiles.h"

typedef NiTMapBase<const char*, int> TraitNameMap;
TraitNameMap *g_traitNameMap = (TraitNameMap*)0x11F32F4;
const _TraitNameToID TraitNameToID = (_TraitNameToID)0xA01860;
UInt32 (*TraitNameToIDAdd)(const char*, UInt32) = (UInt32 (*)(const char*, UInt32))0xA00940;

UInt32 Tile::TraitNameToID(const char *traitName)
{
	return ::TraitNameToID(traitName);
}

UInt32 Tile::TraitNameToIDAdd(const char *traitName)
{
	return ::TraitNameToIDAdd(traitName, 0xFFFFFFFF);
}

__declspec(naked) Tile::Value* __fastcall Tile::GetValue(UInt32 typeID)
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
		jz		done
		jb		isLT
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

Tile::Value *Tile::GetValueName(const char *valueName)
{
	return GetValue(::TraitNameToID(valueName));
}

DListNode<Tile> *Tile::GetNthChild(UInt32 index)
{
	return children.Tail()->Regress(index);
}

void Tile::GetComponentFullName(char *resStr)
{
	Tile *parents[24], *pTile = this;
	UInt32 count = 0, length;
	while (true)
	{
		parents[count++] = pTile;
		if IS_TYPE(pTile, TileMenu) break;
		pTile = pTile->parent;
	}
	char *pName;
	DListNode<Tile> *node;
	int index;
	while (true)
	{
		pTile = parents[--count];
		pName = pTile->name.m_data;
		length = pTile->name.m_dataLen;
		COPY_BYTES(resStr, pName, length);
		resStr += length;
		node = pTile->parent->children.Tail();
		while (node->data != pTile)
			node = node->prev;
		index = 0;
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

Menu *Tile::GetParentMenu()
{
	Tile *tile = this;
	do
	{
		if IS_TYPE(tile, TileMenu)
			return ((TileMenu*)tile)->menu;
	}
	while (tile = tile->parent);
	return NULL;
}

__declspec(naked) void __fastcall Tile::PokeValue(UInt32 valueID)
{
	__asm
	{
		call	Tile::GetValue
		test	eax, eax
		jz		done
		push	eax
		push	1
		push	0x3F800000
		mov		ecx, eax
		CALL_EAX(ADDR_TileValSetFloat)
		pop		ecx
		push	1
		push	0
		CALL_EAX(ADDR_TileValSetFloat)
	done:
		retn
	}
}

__declspec(naked) void Tile::FakeClick()
{
	__asm
	{
		mov		edx, kTileValue_clicked
		call	Tile::GetValue
		test	eax, eax
		jz		done
		push	eax
		push	1
		push	0x3F800000
		mov		ecx, eax
		CALL_EAX(ADDR_TileValSetFloat)
		pop		ecx
		push	1
		push	0
		CALL_EAX(ADDR_TileValSetFloat)
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
		if (colon == childName) return NULL;
		*colon = 0;
		childIndex = StrToInt(colon + 1);
	}
	Tile *result = NULL;
	bool wildcard = *childName == '*';
	for (DListNode<Tile> *node = children.Head(); node; node = node->next)
	{
		if (node->data && (wildcard || !StrCompareCI(node->data->name.m_data, childName)) && !childIndex--)
		{
			result = node->data;
			break;
		}
	}
	if (colon) *colon = ':';
	return result;
}

Tile *Tile::GetComponent(const char *componentPath, const char **trait)
{
	Tile *parentTile = this;
	char *slashPos;
	while (slashPos = SlashPos(componentPath))
	{
		*slashPos = 0;
		parentTile = parentTile->GetChild(componentPath);
		if (!parentTile) return NULL;
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

Tile::Value *Tile::GetComponentValue(const char *componentPath)
{
	const char *trait = NULL;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && trait) ? tile->GetValueName(trait) : NULL;
}

Tile *Tile::GetComponentTile(const char *componentPath)
{
	const char *trait = NULL;
	Tile *tile = GetComponent(componentPath, &trait);
	return (tile && !trait) ? tile : NULL;
}

Tile* __fastcall GetTargetComponent(const char *componentPath, Tile::Value **value)
{
	char *slashPos = SlashPos(componentPath);
	if (!slashPos)
		return GetMenuTile(componentPath);
	*slashPos = 0;
	Tile *component = GetMenuTile(componentPath);
	if (!component)
		return NULL;
	const char *trait = NULL;
	component = component->GetComponent(slashPos + 1, &trait);
	if (!component)
		return NULL;
	if (trait)
	{
		if (!value || !(*value = component->GetValueName(trait)))
			return NULL;
	}
	else if (value)
		return NULL;
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
		call	NiReleaseAddRef
	done:
		retn
	}
}

void Tile::Dump()
{
	PrintDebug("%08X\t%s", this, name.m_data);
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

	for (DListNode<Tile> *traverse = children.Tail(); traverse; traverse = traverse->prev)
		if (traverse->data) traverse->data->Dump();

	s_debug().Outdent();
}

// not a one-way mapping, so we just return the first
// also this is slow and sucks
const char *TraitIDToName(int id)
{
	for (auto iter = g_traitNameMap->Begin(); iter; ++iter)
		if (iter.Get() == id) return iter.Key();
	return NULL;
}
