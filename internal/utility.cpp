#include "internal/utility.h"
#include "nvse/GameAPI.h"

memcpy_t MemCopy = memcpy, MemMove = memmove;

void LightCS::Enter()
{
	UInt32 threadID = GetCurrentThreadId();
	if (owningThread == threadID)
	{
		enterCount++;
		return;
	}
	while (InterlockedCompareExchange(&owningThread, threadID, 0));
	enterCount = 1;
}

#define FAST_SLEEP_COUNT 10000UL

void LightCS::EnterSleep()
{
	UInt32 threadID = GetCurrentThreadId();
	if (owningThread == threadID)
	{
		enterCount++;
		return;
	}
	UInt32 fastIdx = FAST_SLEEP_COUNT;
	while (InterlockedCompareExchange(&owningThread, threadID, 0))
	{
		if (fastIdx)
		{
			fastIdx--;
			Sleep(0);
		}
		else Sleep(1);
	}
	enterCount = 1;
}

void LightCS::Leave()
{
	if (!--enterCount)
		owningThread = 0;
}

__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds:[0x11C54C0]
		mov		eax, [esp+4]
		xor		edx, edx
		div		dword ptr [ecx+4]
		mov		eax, [ecx+8]
		mov		eax, [eax+edx*4]
		test	eax, eax
		jz		done
		mov		edx, [esp+4]
		ALIGN 16
	iterHead:
		cmp		[eax+4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		retn	4
	found:
		mov		eax, [eax+8]
	done:
		retn	4
	}
}

alignas(16) static const double kValueBounds[] = {4294967296, -4294967296};

__declspec(naked) UInt32 __vectorcall cvtd2ui(double value)
{
	__asm
	{
		lea		eax, [esp-8]
		movq	qword ptr [eax], xmm0
		fld		qword ptr [eax]
		fisttp	qword ptr [eax]
		mov		eax, [eax]
		retn
	}
}

__declspec(naked) double __vectorcall cvtui2d(UInt32 value)
{
	__asm
	{
		cvtsi2sd	xmm0, ecx
		test	ecx, ecx
		jns		done
		addsd	xmm0, kValueBounds
	done:
		retn
	}
}

__declspec(naked) int __vectorcall ifloor(float value)
{
	__asm
	{
		movd	eax, xmm0
		test	eax, eax
		jns		isPos
		push	0x3FA0
		ldmxcsr	[esp]
		cvtss2si	eax, xmm0
		mov		dword ptr [esp], 0x1FA0
		ldmxcsr	[esp]
		pop		ecx
		retn
	isPos:
		cvttss2si	eax, xmm0
		retn
	}
}

__declspec(naked) int __vectorcall iceil(float value)
{
	__asm
	{
		movd	eax, xmm0
		test	eax, eax
		js		isNeg
		push	0x5FA0
		ldmxcsr	[esp]
		cvtss2si	eax, xmm0
		mov		dword ptr [esp], 0x1FA0
		ldmxcsr	[esp]
		pop		ecx
		retn
	isNeg:
		cvttss2si	eax, xmm0
		retn
	}
}

__declspec(naked) void __fastcall NiReleaseObject(NiRefObject *toRelease)
{
	__asm
	{
		lock dec dword ptr [ecx+4]
		jg		done
		mov		eax, [ecx]
		call	dword ptr [eax+4]
	done:
		retn
	}
}

__declspec(naked) NiRefObject** __stdcall NiReleaseAddRef(NiRefObject **toRelease, NiRefObject *toAdd)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		ecx, [eax]
		cmp		ecx, [esp+8]
		jz		done
		test	ecx, ecx
		jz		doAdd
		lock dec dword ptr [ecx+4]
		jg		doAdd
		mov		eax, [ecx]
		call	dword ptr [eax+4]
		mov		eax, [esp+4]
	doAdd:
		mov		ecx, [esp+8]
		mov		[eax], ecx
		test	ecx, ecx
		jz		done
		lock inc dword ptr [ecx+4]
	done:
		retn	8
	}
}

__declspec(naked) UInt32 __fastcall RGBHexToDec(UInt32 rgb)
{
	__asm
	{
		movzx	eax, cl
		imul	eax, 0xF4240
		movzx	edx, ch
		imul	edx, 0x3E8
		add		eax, edx
		shr		ecx, 0x10
		add		eax, ecx
		retn
	}
}

__declspec(naked) UInt32 __fastcall RGBDecToHex(UInt32 rgb)
{
	__asm
	{
		push	ebx
		mov		eax, ecx
		mov		ecx, 0xF4240
		xor		edx, edx
		div		ecx
		mov		ebx, eax
		mov		eax, edx
		mov		ecx, 0x3E8
		xor		edx, edx
		div		ecx
		shl		eax, 8
		add		eax, ebx
		shl		edx, 0x10
		add		eax, edx
		pop		ebx
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrLen(const char *str)
{
	__asm
	{
		test	ecx, ecx
		jz		nullPtr
		push	ecx
		test	ecx, 3
		jz		iter4
		ALIGN 16
	iter1:
		mov		al, [ecx]
		inc		ecx
		test	al, al
		jz		done1
		test	ecx, 3
		jnz		iter1
		ALIGN 16
	iter4:
		mov		eax, [ecx]
		mov		edx, 0x7EFEFEFF
		add		edx, eax
		not		eax
		xor		eax, edx
		add		ecx, 4
		test	eax, 0x81010100
		jz		iter4
		mov		eax, [ecx-4]
		test	al, al
		jz		done4
		test	ah, ah
		jz		done3
		test	eax, 0xFF0000
		jz		done2
		test	eax, 0xFF000000
		jnz		iter4
	done1:
		lea		eax, [ecx-1]
		pop		ecx
		sub		eax, ecx
		retn
	done2:
		lea		eax, [ecx-2]
		pop		ecx
		sub		eax, ecx
		retn
	done3:
		lea		eax, [ecx-3]
		pop		ecx
		sub		eax, ecx
		retn
	done4:
		lea		eax, [ecx-4]
		pop		ecx
		sub		eax, ecx
		retn
	nullPtr:
		xor		eax, eax
		retn
	}
}

__declspec(naked) void __fastcall MemZero(void *dest, UInt32 bsize)
{
	__asm
	{
		test	ecx, ecx
		jz		done
		shr		edx, 2
		jz		done
		push	edi
		mov		edi, ecx
		xor		eax, eax
		mov		ecx, edx
		rep stosd
		pop		edi
	done:
		retn
	}
}

__declspec(naked) char* __fastcall StrCopy(char *dest, const char *src)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		push	ecx
		mov		ecx, edx
		call	StrLen
		pop		edx
		push	eax
		inc		eax
		push	eax
		push	ecx
		push	edx
		call	MemMove
		add		esp, 0xC
		pop		ecx
		add		eax, ecx
		retn
	nullTerm:
		mov		[eax], 0
	done:
		retn
	}
}

__declspec(naked) char* __fastcall StrNCopy(char *dest, const char *src, UInt32 length)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		cmp		dword ptr [esp+4], 0
		jz		nullTerm
		push	esi
		mov		esi, ecx
		mov		ecx, edx
		call	StrLen
		mov		edx, [esp+8]
		cmp		edx, eax
		cmova	edx, eax
		push	edx
		push	ecx
		push	esi
		add		esi, edx
		call	MemMove
		add		esp, 0xC
		mov		eax, esi
		pop		esi
	nullTerm:
		mov		[eax], 0
	done:
		retn	4
	}
}

__declspec(naked) char* __fastcall StrLenCopy(char *dest, const char *src, UInt32 length)
{
	__asm
	{
		mov		eax, ecx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		mov		ecx, [esp+4]
		test	ecx, ecx
		jz		nullTerm
		push	ecx
		push	edx
		push	eax
		call	MemMove
		add		esp, 0xC
		add		eax, [esp+4]
	nullTerm:
		mov		[eax], 0
	done:
		retn	4
	}
}

__declspec(naked) char* __fastcall StrCat(char *dest, const char *src)
{
	__asm
	{
		test	ecx, ecx
		jz		nullPtr
		push	edx
		call	StrLen
		pop		edx
		add		ecx, eax
		jmp		StrCopy
	nullPtr:
		xor		eax, eax
		retn
	}
}

alignas(16) const UInt8 kLwrCaseConverter[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};
alignas(16) const UInt8 kUprCaseConverter[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

__declspec(noinline) char __fastcall StrCompare(const char *lstr, const char *rstr)
{
	if (!lstr) return rstr ? -1 : 0;
	if (!rstr) return 1;
	UInt8 lchr, rchr;
	while (*lstr)
	{
		lchr = kLwrCaseConverter[*(UInt8*)lstr];
		rchr = kLwrCaseConverter[*(UInt8*)rstr];
		if (lchr == rchr)
		{
			lstr++;
			rstr++;
			continue;
		}
		return (lchr < rchr) ? -1 : 1;
	}
	return *rstr ? -1 : 0;
}

__declspec(noinline) char __fastcall StrBeginsCS(const char *lstr, const char *rstr)
{
	if (!lstr || !rstr) return 0;
	UInt32 length = StrLen(rstr);
	while (length >= 4)
	{
		if (*(UInt32*)lstr != *(UInt32*)rstr)
			return 0;
		lstr += 4;
		rstr += 4;
		length -= 4;
	}
	while (length)
	{
		if (*lstr != *rstr)
			return 0;
		lstr++;
		rstr++;
		length--;
	}
	return *lstr ? 1 : 2;
}

__declspec(noinline) char __fastcall StrBeginsCI(const char *lstr, const char *rstr)
{
	if (!lstr || !rstr) return 0;
	UInt32 length = StrLen(rstr);
	while (length)
	{
		if (kLwrCaseConverter[*(UInt8*)lstr] != kLwrCaseConverter[*(UInt8*)rstr])
			return 0;
		lstr++;
		rstr++;
		length--;
	}
	return *lstr ? 1 : 2;
}

__declspec(noinline) void __fastcall FixPath(char *str)
{
	if (!str) return;
	UInt8 curr;
	while (curr = *str)
	{
		if (curr == '\\')
			*str = '/';
		else *str = kLwrCaseConverter[curr];
		str++;
	}
}

__declspec(noinline) void __fastcall StrToLower(char *str)
{
	if (!str) return;
	UInt8 curr;
	while (curr = *str)
	{
		*str = kLwrCaseConverter[curr];
		str++;
	}
}

__declspec(noinline) void __fastcall StrToUpper(char *str)
{
	if (!str) return;
	UInt8 curr;
	while (curr = *str)
	{
		*str = kUprCaseConverter[curr];
		str++;
	}
}

__declspec(noinline) void __fastcall ReplaceChr(char *str, char from, char to)
{
	if (!str) return;
	char curr;
	while (curr = *str)
	{
		if (curr == from)
			*str = to;
		str++;
	}
}

__declspec(noinline) char* __fastcall FindChrR(const char *str, UInt32 length, char chr)
{
	if (!str) return NULL;
	char *strEnd = const_cast<char*>(str) + length;
	while (strEnd > str)
	{
		strEnd--;
		if (*strEnd == chr)
			return strEnd;
	}
	return NULL;
}

__declspec(noinline) char* __fastcall SubStrCI(const char *srcStr, const char *subStr)
{
	int srcLen = StrLen(srcStr);
	if (!srcLen) return NULL;
	int subLen = StrLen(subStr);
	if (!subLen) return NULL;
	srcLen -= subLen;
	if (srcLen < 0) return NULL;
	int index;
	do
	{
		index = 0;
		while (true)
		{
			if (kLwrCaseConverter[*(UInt8*)(srcStr + index)] != kLwrCaseConverter[*(UInt8*)(subStr + index)])
				break;
			if (++index == subLen)
				return const_cast<char*>(srcStr);
		}
		srcStr++;
	}
	while (--srcLen >= 0);
	return NULL;
}

__declspec(noinline) char* __fastcall SlashPos(const char *str)
{
	if (!str) return NULL;
	char curr;
	while (curr = *str)
	{
		if ((curr == '/') || (curr == '\\'))
			return const_cast<char*>(str);
		str++;
	}
	return NULL;
}

__declspec(noinline) char* __fastcall SlashPosR(const char *str)
{
	if (!str) return NULL;
	char *strEnd = const_cast<char*>(str) + StrLen(str);
	while (strEnd > str)
	{
		strEnd--;
		if ((*strEnd == '/') || (*strEnd == '\\'))
			return strEnd;
	}
	return NULL;
}

__declspec(noinline) char* __fastcall GetNextToken(char *str, char delim)
{
	if (!str) return NULL;
	bool found = false;
	char chr;
	while (chr = *str)
	{
		if (chr == delim)
		{
			*str = 0;
			found = true;
		}
		else if (found)
			break;
		str++;
	}
	return str;
}

__declspec(noinline) char* __fastcall GetNextToken(char *str, const char *delims)
{
	if (!str) return NULL;
	bool table[0x100];
	MemZero(table, 0x100);
	UInt8 curr;
	while (curr = *delims)
	{
		table[curr] = true;
		delims++;
	}
	bool found = false;
	while (curr = *str)
	{
		if (table[curr])
		{
			*str = 0;
			found = true;
		}
		else if (found)
			break;
		str++;
	}
	return str;
}

__declspec(naked) char* __fastcall CopyString(const char *key)
{
	__asm
	{
		call	StrLen
		inc		eax
		push	eax
		push	ecx
		push	eax
		call	malloc
		pop		ecx
		push	eax
		call	MemCopy
		add		esp, 0xC
		retn
	}
}

__declspec(naked) char* __fastcall CopyCString(const char *src)
{
	__asm
	{
		call	StrLen
		test	eax, eax
		jz		done
		inc		eax
		push	eax
		push	ecx
		push	eax
		GAME_HEAP_ALLOC
		push	eax
		call	MemCopy
		add		esp, 0xC
	done:
		retn
	}
}

__declspec(naked) char* __fastcall IntToStr(char *str, int num)
{
	__asm
	{
		push	esi
		push	edi
		test	edx, edx
		jns		skipNeg
		neg		edx
		mov		[ecx], '-'
		inc		ecx
	skipNeg:
		mov		esi, ecx
		mov		edi, ecx
		mov		eax, edx
		mov		ecx, 0xA
		ALIGN 16
	workIter:
		xor		edx, edx
		div		ecx
		add		dl, '0'
		mov		[esi], dl
		inc		esi
		test	eax, eax
		jnz		workIter
		mov		[esi], 0
		mov		eax, esi
		ALIGN 16
	swapIter:
		dec		esi
		cmp		esi, edi
		jbe		done
		mov		dl, [esi]
		mov		cl, [edi]
		mov		[esi], cl
		mov		[edi], dl
		inc		edi
		jmp		swapIter
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) char* __vectorcall FltToStr(char *str, double value)
{
	static const double kDbl1Mil = 1000000.0;
	static const int kIntDivisors[] = {10, 100, 1000, 10000, 100000};
	__asm
	{
		pxor	xmm1, xmm1
		comisd	xmm0, xmm1
		jnz		nonZero
		mov		word ptr [ecx], '0'
		lea		eax, [ecx+1]
		retn
	nonZero:
		ja		isPos
		pcmpeqd	xmm2, xmm2
		psrlq	xmm2, 1
		andpd	xmm0, xmm2
		mov		[ecx], '-'
		inc		ecx
	isPos:
		push	esi
		push	edi
		lea		eax, [esp-8]
		movq	qword ptr [eax], xmm0
		fld		qword ptr [eax]
		fld		st
		fisttp	qword ptr [eax]
		mov		esi, [eax]
		fild	qword ptr [eax]
		fsubp	st(1), st
		fstp	qword ptr [eax]
		movq	xmm0, qword ptr [eax]
		/*roundsd	xmm2, xmm0, 3
		lea		eax, [esp-8]
		movq	qword ptr [eax], xmm2
		fld		qword ptr [eax]
		fisttp	qword ptr [eax]
		mov		esi, [eax]
		test	esi, esi
		jz		noInt
		subsd	xmm0, xmm2
	noInt:*/
		xor		edi, edi
		comisd	xmm0, xmm1
		jz		noFrac
		mulsd	xmm0, kDbl1Mil
		cvtsd2si	edi, xmm0
		cmp		edi, 0xF4240
		jnz		noFrac
		xor		edi, edi
		inc		esi
	noFrac:
		test	esi, esi
		jz		zeroInt
		mov		edx, esi
		call	IntToStr
		mov		ecx, eax
		jmp		doneInt
	zeroInt:
		mov		[ecx], '0'
		inc		ecx
	doneInt:
		test	edi, edi
		jz		done
		mov		[ecx], '.'
		inc		ecx
		mov		edx, edi
		mov		esi, offset kIntDivisors
		mov		edi, 4
		ALIGN 16
	fracIter:
		mov		eax, edx
		xor		edx, edx
		div		dword ptr [esi+edi*4]
		add		al, '0'
		mov		[ecx], al
		inc		ecx
		test	edx, edx
		jz		done
		dec		edi
		jns		fracIter
		add		dl, '0'
		mov		[ecx], dl
		inc		ecx
	done:
		mov		[ecx], 0
		mov		eax, ecx
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) int __fastcall StrToInt(const char *str)
{
	__asm
	{
		push	esi
		push	edi
		xor		eax, eax
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		ecx, ecx
		xor		edi, edi
		cmp		[esi], '-'
		setz	dl
		jnz		charIter
		inc		esi
		ALIGN 16
	charIter:
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		iterEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		cmp		eax, edi
		jl		overflow
		mov		edi, eax
		inc		esi
		jmp		charIter
		ALIGN 16
	iterEnd:
		test	dl, dl
		jz		done
		neg		eax
	done:
		pop		edi
		pop		esi
		retn
	overflow:
		movzx	eax, dl
		add		eax, 0x7FFFFFFF
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall StrToUInt(const char *str)
{
	__asm
	{
		push	esi
		push	edi
		xor		eax, eax
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		ecx, ecx
		xor		edi, edi
		cmp		[esi], '-'
		setz	dl
		jnz		charIter
		inc		esi
		ALIGN 16
	charIter:
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		iterEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		cmp		eax, edi
		jb		overflow
		mov		edi, eax
		inc		esi
		jmp		charIter
		ALIGN 16
	iterEnd:
		test	dl, dl
		jz		done
		neg		eax
	done:
		pop		edi
		pop		esi
		retn
	overflow:
		movzx	eax, dl
		dec		eax
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) double __vectorcall StrToDbl(const char *str)
{
	static const double kFactor10Div[] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10};
	__asm
	{
		push	esi
		push	edi
		pxor	xmm0, xmm0
		test	ecx, ecx
		jz		done
		mov		esi, ecx
		xor		eax, eax
		xor		ecx, ecx
		xor		edi, edi
		cmp		[esi], '-'
		setz	dl
		jnz		intIter
		inc		esi
		ALIGN 16
	intIter:
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		intEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		cmp		eax, edi
		jb		overflow
		mov		edi, eax
		inc		esi
		jmp		intIter
		ALIGN 16
	intEnd:
		test	eax, eax
		jz		noInt
		cvtsi2sd	xmm0, eax
		jns		noOverflow
		addsd	xmm0, kValueBounds
	noOverflow:
		shl		dl, 1
		xor		eax, eax
	noInt:
		cmp		cl, 0xFE
		jnz		addSign
		mov		dh, 9
		ALIGN 16
	fracIter:
		inc		esi
		mov		cl, [esi]
		sub		cl, '0'
		cmp		cl, 9
		ja		fracEnd
		lea		eax, [eax+eax*4]
		lea		eax, [ecx+eax*2]
		dec		dh
		jnz		fracIter
	fracEnd:
		test	eax, eax
		jz		addSign
		cvtsi2sd	xmm1, eax
		shl		dl, 1
		mov		cl, dh
		divsd	xmm1, kFactor10Div[ecx*8]
		addsd	xmm0, xmm1
	addSign:
		test	dl, 6
		jz		done
		pcmpeqd	xmm1, xmm1
		psllq	xmm1, 0x3F
		orpd	xmm0, xmm1
	done:
		pop		edi
		pop		esi
		retn
	overflow:
		movzx	eax, dl
		movq	xmm0, kValueBounds[eax*8]
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) char* __fastcall UIntToHex(char *str, UInt32 num)
{
	static const char kCharAtlas[] = "0123456789ABCDEF";
	__asm
	{
		push	esi
		push	edi
		mov		esi, ecx
		mov		edi, ecx
		xor		eax, eax
		ALIGN 16
	workIter:
		mov		al, dl
		and		al, 0xF
		mov		cl, kCharAtlas[eax]
		mov		[esi], cl
		inc		esi
		shr		edx, 4
		jnz		workIter
		mov		[esi], 0
		mov		eax, esi
		ALIGN 16
	swapIter:
		dec		esi
		cmp		esi, edi
		jbe		done
		mov		dl, [esi]
		mov		cl, [edi]
		mov		[esi], cl
		mov		[edi], dl
		inc		edi
		jmp		swapIter
	done:
		pop		edi
		pop		esi
		retn
	}
}

__declspec(naked) UInt32 __fastcall HexToUInt(const char *str)
{
	__asm
	{
		push	esi
		call	StrLen
		test	eax, eax
		jz		done
		lea		esi, [ecx+eax]
		mov		ch, al
		xor		eax, eax
		xor		cl, cl
	hexToInt:
		dec		esi
		movsx	edx, byte ptr [esi]
		sub		dl, '0'
		js		done
		cmp		dl, 9
		jle		doAdd
		or		dl, 0x20
		cmp		dl, '1'
		jl		done
		cmp		dl, '6'
		jg		done
		sub		dl, 0x27
	doAdd:
		shl		edx, cl
		add		eax, edx
		add		cl, 4
		dec		ch
		jnz		hexToInt
	done:
		pop		esi
		retn
	}
}

AuxBuffer s_auxBuffers[3];

__declspec(naked) UInt8* __fastcall GetAuxBuffer(AuxBuffer &buffer, UInt32 reqSize)
{
	__asm
	{
		mov		eax, [ecx]
		cmp		[ecx+4], edx
		jb		doRealloc
		test	eax, eax
		jz		doInit
		retn
	doInit:
		push	ecx
		push	0x10
		push	dword ptr [ecx+4]
		jmp		doAlloc
	doRealloc:
		mov		[ecx+4], edx
		push	ecx
		push	0x10
		push	edx
		test	eax, eax
		jz		doAlloc
		push	eax
		call	_aligned_free
		pop		ecx
	doAlloc:
		call	_aligned_malloc
		add		esp, 8
		pop		ecx
		mov		[ecx], eax
		retn
	}
}

DString::DString(const char *from)
{
	length = StrLen(from);
	if (length)
	{
		alloc = AlignNumAlloc<char>(length + 1);
		str = (char*)Pool_Alloc(alloc);
		memcpy(str, from, length + 1);
	}
	else
	{
		str = NULL;
		alloc = 0;
	}
}

DString::DString(const DString &from)
{
	length = from.length;
	if (length)
	{
		alloc = AlignNumAlloc<char>(length + 1);
		str = (char*)Pool_Alloc(alloc);
		memcpy(str, from.str, length + 1);
	}
	else
	{
		str = NULL;
		alloc = 0;
	}
}

DString::DString(UInt16 _alloc) : length(0)
{
	if (_alloc)
	{
		alloc = AlignNumAlloc<char>(_alloc + 1);
		str = (char*)Pool_Alloc(alloc);
		*str = 0;
	}
	else
	{
		str = NULL;
		alloc = 0;
	}
}

void DString::Reserve(UInt16 size)
{
	if (alloc > size) return;
	UInt16 newAlloc = AlignNumAlloc<char>(size + 1);
	char *newStr = (char*)Pool_Alloc(newAlloc);
	if (str)
	{
		if (length) memcpy(newStr, str, length);
		Pool_Free(str, alloc);
	}
	str = newStr;
	str[length] = 0;
	alloc = newAlloc;
}

DString& DString::operator=(const char *other)
{
	if (str != other)
	{
		length = StrLen(other);
		if (length)
		{
			if (alloc <= length)
			{
				if (str) Pool_Free(str, alloc);
				alloc = AlignNumAlloc<char>(length + 1);
				str = (char*)Pool_Alloc(alloc);
			}
			memcpy(str, other, length + 1);
		}
		else if (str)
			*str = 0;
	}
	return *this;
}

DString& DString::operator=(const DString &other)
{
	if (this != &other)
	{
		length = other.length;
		if (length)
		{
			if (alloc <= length)
			{
				if (str) Pool_Free(str, alloc);
				alloc = AlignNumAlloc<char>(length + 1);
				str = (char*)Pool_Alloc(alloc);
			}
			memcpy(str, other.str, length + 1);
		}
		else if (str)
			*str = 0;
	}
	return *this;
}

DString& DString::operator+=(char chr)
{
	Reserve(length + 1);
	*(UInt16*)(str + length) = chr;
	length++;
	return *this;
}

DString& DString::operator+=(const char *other)
{
	UInt16 otherLen = StrLen(other);
	if (otherLen)
	{
		UInt16 newLen = length + otherLen;
		Reserve(newLen);
		memcpy(str + length, other, otherLen + 1);
		length = newLen;
	}
	return *this;
}

DString& DString::operator+=(const DString &other)
{
	if (other.length)
	{
		UInt16 newLen = length + other.length;
		Reserve(newLen);
		memcpy(str + length, other.str, other.length + 1);
		length = newLen;
	}
	return *this;
}

bool DString::operator==(const char *other)
{
	return !strcmp(str, other);
}

bool DString::operator==(const DString &other)
{
	return !strcmp(str, other.str);
}

DString& DString::Insert(UInt16 index, char chr)
{
	if (index >= length)
		return this->operator+=(chr);
	Reserve(length + 1);
	memmove(str + index + 1, str + index, length - index + 1);
	str[index] = chr;
	length++;
	return *this;
}

DString& DString::Insert(UInt16 index, const char *other)
{
	if (index >= length)
		return this->operator+=(other);
	UInt16 otherLen = StrLen(other);
	if (otherLen)
	{
		UInt16 newLen = length + otherLen;
		Reserve(newLen);
		memmove(str + index + otherLen, str + index, length - index + 1);
		memcpy(str + index, other, otherLen);
		length = newLen;
	}
	return *this;
}

DString& DString::Insert(UInt16 index, const DString &other)
{
	if (index >= length)
		return this->operator+=(other);
	if (other.length)
	{
		UInt16 newLen = length + other.length;
		Reserve(newLen);
		memmove(str + index + other.length, str + index, length - index + 1);
		memcpy(str + index, other.str, other.length);
		length = newLen;
	}
	return *this;
}

DString& DString::Erase(UInt16 index, UInt16 count)
{
	if (index < length)
	{
		UInt16 endIdx = index + count;
		if (endIdx < length)
		{
			memmove(str + index, str + endIdx, length - endIdx + 1);
			length -= count;
		}
		else
		{
			str[index] = 0;
			length = index;
		}
	}
	return *this;
}

DString& DString::Replace(UInt16 bgnIdx, const char *other)
{
	if (bgnIdx >= length)
		return this->operator+=(other);
	UInt16 otLen = StrLen(other);
	if (otLen)
	{
		UInt16 endIdx = bgnIdx + otLen;
		if (endIdx > length)
		{
			Reserve(endIdx);
			length = endIdx;
		}
		memcpy(str + bgnIdx, other, otLen);
	}
	else length = bgnIdx;
	str[length] = 0;
	return *this;
}

DString& DString::Replace(UInt16 bgnIdx, const DString &other)
{
	if (bgnIdx >= length)
		return this->operator+=(other);
	if (other.length)
	{
		UInt16 endIdx = bgnIdx + other.length;
		if (endIdx > length)
		{
			Reserve(endIdx);
			length = endIdx;
		}
		memcpy(str + bgnIdx, other.str, other.length);
	}
	else length = bgnIdx;
	str[length] = 0;
	return *this;
}

DString DString::SubString(UInt16 bgnIdx, UInt16 endIdx)
{
	char *resStr = NULL;
	UInt16 resLen = 0, resAlloc = 0;
	if (bgnIdx < length)
	{
		if (endIdx > length)
			endIdx = length;
		resLen = endIdx - bgnIdx;
		if (resLen)
		{
			resAlloc = AlignNumAlloc<char>(resLen + 1);
			resStr = (char*)Pool_Alloc(resAlloc);
			memcpy(resStr, str + bgnIdx, resLen);
			resStr[resLen] = 0;
		}
	}
	return DString(resStr, resLen, resAlloc);
}

DString DString::ToLower()
{
	if (!length) return DString();
	UInt16 resAlloc = AlignNumAlloc<char>(length + 1);
	char *resStr = (char*)Pool_Alloc(resAlloc);
	memcpy(resStr, str, length + 1);
	StrToLower(resStr);
	return DString(resStr, length, resAlloc);
}

DString DString::ToUpper()
{
	if (!length) return DString();
	UInt16 resAlloc = AlignNumAlloc<char>(length + 1);
	char *resStr = (char*)Pool_Alloc(resAlloc);
	memcpy(resStr, str, length + 1);
	StrToUpper(resStr);
	return DString(resStr, length, resAlloc);
}

DString operator+(const DString &lStr, char rChr)
{
	UInt16 resLen = lStr.length + 1, resAlloc = AlignNumAlloc<char>(resLen + 1);
	char *resStr = (char*)Pool_Alloc(resAlloc);
	if (lStr.length) memcpy(resStr, lStr.str, lStr.length);
	*(UInt16*)(resStr + lStr.length) = rChr;
	return DString(resStr, resLen, resAlloc);
}

DString operator+(const DString &lStr, const char *rStr)
{
	char *resStr = NULL;
	UInt16 rLen = StrLen(rStr), resLen = lStr.length + rLen, resAlloc = 0;
	if (resLen)
	{
		resAlloc = AlignNumAlloc<char>(resLen + 1);
		resStr = (char*)Pool_Alloc(resAlloc);
		if (lStr.length) memcpy(resStr, lStr.str, lStr.length);
		if (rLen) memcpy(resStr + lStr.length, rStr, rLen);
		resStr[resLen] = 0;
	}
	return DString(resStr, resLen, resAlloc);
}

DString operator+(const char *lStr, const DString &rStr)
{
	char *resStr = NULL;
	UInt16 lLen = StrLen(lStr), resLen = lLen + rStr.length, resAlloc = 0;
	if (resLen)
	{
		resAlloc = AlignNumAlloc<char>(resLen + 1);
		resStr = (char*)Pool_Alloc(resAlloc);
		if (lLen) memcpy(resStr, lStr, lLen);
		if (rStr.length) memcpy(resStr + lLen, rStr.str, rStr.length);
		resStr[resLen] = 0;
	}
	return DString(resStr, resLen, resAlloc);
}

bool __fastcall FileExists(const char *filePath)
{
	UInt32 attr = GetFileAttributes(filePath);
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileStream::Open(const char *filePath)
{
	if (theFile) fclose(theFile);
	theFile = _fsopen(filePath, "rb", 0x20);
	return theFile ? true : false;
}

bool FileStream::OpenAt(const char *filePath, UInt32 inOffset)
{
	if (theFile) fclose(theFile);
	theFile = _fsopen(filePath, "rb", 0x20);
	if (!theFile) return false;
	fseek(theFile, 0, SEEK_END);
	if (ftell(theFile) < inOffset)
	{
		Close();
		return false;
	}
	fseek(theFile, inOffset, SEEK_SET);
	return true;
}

bool FileStream::OpenWrite(char *filePath, bool append)
{
	if (theFile) fclose(theFile);
	if (FileExists(filePath))
	{
		if (append)
		{
			theFile = _fsopen(filePath, "ab", 0x20);
			if (!theFile) return false;
			fputc('\n', theFile);
			fflush(theFile);
			return true;
		}
	}
	else MakeAllDirs(filePath);
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile ? true : false;
}

bool FileStream::Create(const char *filePath)
{
	if (theFile) fclose(theFile);
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile ? true : false;
}

UInt32 FileStream::GetLength()
{
	fseek(theFile, 0, SEEK_END);
	UInt32 result = ftell(theFile);
	rewind(theFile);
	return result;
}

void FileStream::SetOffset(UInt32 inOffset)
{
	fseek(theFile, 0, SEEK_END);
	if (ftell(theFile) > inOffset)
		fseek(theFile, inOffset, SEEK_SET);
}

char FileStream::ReadChar()
{
	return (char)fgetc(theFile);
}

void FileStream::ReadBuf(void *outData, UInt32 inLength)
{
	fread(outData, inLength, 1, theFile);
}

void FileStream::WriteChar(char chr)
{
	fputc(chr, theFile);
	fflush(theFile);
}

void FileStream::WriteStr(const char *inStr)
{
	fputs(inStr, theFile);
	fflush(theFile);
}

void FileStream::WriteBuf(const void *inData, UInt32 inLength)
{
	fwrite(inData, inLength, 1, theFile);
	fflush(theFile);
}

int FileStream::WriteFmtStr(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int iWritten = vfprintf(theFile, fmt, args);
	va_end(args);
	fflush(theFile);
	return iWritten;
}

void FileStream::MakeAllDirs(char *fullPath)
{
	char *traverse = fullPath, curr;
	while (curr = *traverse)
	{
		if ((curr == '\\') || (curr == '/'))
		{
			*traverse = 0;
			CreateDirectory(fullPath, NULL);
			*traverse = '\\';
		}
		traverse++;
	}
}

bool DebugLog::Create(const char *filePath)
{
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile ? true : false;
}

const char kIndentLevelStr[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

void DebugLog::Message(const char *msgStr)
{
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	fputs(msgStr, theFile);
	fputc('\n', theFile);
	fflush(theFile);
}

void DebugLog::FmtMessage(const char *fmt, va_list args)
{
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	vfprintf(theFile, fmt, args);
	fputc('\n', theFile);
	fflush(theFile);
}

void PrintLog(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	s_log.FmtMessage(fmt, args);
	va_end(args);
}

void PrintDebug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	s_debug.FmtMessage(fmt, args);
	va_end(args);
}

LineIterator::LineIterator(const char *filePath, char *buffer)
{
	dataPtr = buffer;
	FileStream sourceFile;
	if (!sourceFile.Open(filePath))
	{
		*buffer = 3;
		return;
	}
	UInt32 length = sourceFile.GetLength();
	sourceFile.ReadBuf(buffer, length);
	*(UInt16*)(buffer + length) = 0x300;
	while (length)
	{
		length--;
		if ((*buffer == '\n') || (*buffer == '\r'))
			*buffer = 0;
		buffer++;
	}
	while (!*dataPtr)
		dataPtr++;
}

void LineIterator::Next()
{
	while (*dataPtr)
		dataPtr++;
	while (!*dataPtr)
		dataPtr++;
}

bool __fastcall FileToBuffer(const char *filePath, char *buffer)
{
	FileStream srcFile;
	if (!srcFile.Open(filePath)) return false;
	UInt32 length = srcFile.GetLength();
	if (!length) return false;
	if (length > kMaxMessageLength)
		length = kMaxMessageLength;
	srcFile.ReadBuf(buffer, length);
	buffer[length] = 0;
	return true;
}

extern char s_strArgBuffer[0x4000];

void ClearFolder(char *pathEndPtr)
{
	DirectoryIterator dirIter(s_strArgBuffer);
	while (!dirIter.End())
	{
		if (dirIter.IsFolder())
			ClearFolder(StrCopy(StrCopy(pathEndPtr - 1, dirIter.Get()), "\\*"));
		else
		{
			StrCopy(pathEndPtr - 1, dirIter.Get());
			remove(s_strArgBuffer);
		}
		dirIter.Next();
	}
	dirIter.Close();
	*(pathEndPtr - 1) = 0;
	RemoveDirectory(s_strArgBuffer);
}

__declspec(naked) void __stdcall SafeWrite8(UInt32 addr, UInt32 data)
{
	__asm
	{
#if LOG_HOOKS
		push	1
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	4
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		[eax], dl
		mov		edx, [esp]
		push	esp
		push	edx
		push	4
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWrite16(UInt32 addr, UInt32 data)
{
	__asm
	{
#if LOG_HOOKS
		push	2
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	4
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		[eax], dx
		mov		edx, [esp]
		push	esp
		push	edx
		push	4
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWrite32(UInt32 addr, UInt32 data)
{
	__asm
	{
#if LOG_HOOKS
		push	4
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	4
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		[eax], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	4
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall SafeWriteBuf(UInt32 addr, void *data, UInt32 len)
{
	__asm
	{
#if LOG_HOOKS
		push	dword ptr [esp+0xC]
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	dword ptr [esp+0x18]
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		push	dword ptr [esp+0x10]
		push	dword ptr [esp+0x10]
		push	dword ptr [esp+0x10]
		call	MemCopy
		add		esp, 0xC
		mov		edx, [esp]
		push	esp
		push	edx
		push	dword ptr [esp+0x18]
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	0xC
	}
}

__declspec(naked) void __stdcall WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt)
{
	__asm
	{
#if LOG_HOOKS
		push	5
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	5
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		sub		edx, eax
		sub		edx, 5
		mov		byte ptr [eax], 0xE9
		mov		[eax+1], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	5
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	__asm
	{
#if LOG_HOOKS
		push	5
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	5
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		sub		edx, eax
		sub		edx, 5
		mov		byte ptr [eax], 0xE8
		mov		[eax+1], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	5
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	8
	}
}

__declspec(naked) void __stdcall WritePushRetRelJump(UInt32 baseAddr, UInt32 retAddr, UInt32 jumpTgt)
{
	__asm
	{
#if LOG_HOOKS
		push	0xA
		push	dword ptr [esp+8]
		call	StoreOriginalData
#endif
		push	ecx
		push	esp
		push	PAGE_EXECUTE_READWRITE
		push	0xA
		push	dword ptr [esp+0x14]
		call	VirtualProtect
		mov		eax, [esp+8]
		mov		edx, [esp+0xC]
		mov		byte ptr [eax], 0x68
		mov		[eax+1], edx
		mov		edx, [esp+0x10]
		sub		edx, eax
		sub		edx, 0xA
		mov		byte ptr [eax+5], 0xE9
		mov		[eax+6], edx
		mov		edx, [esp]
		push	esp
		push	edx
		push	0xA
		push	eax
		call	VirtualProtect
		pop		ecx
		retn	0xC
	}
}

void __fastcall GetTimeStamp(char *buffer)
{
	time_t rawTime = time(NULL);
	tm timeInfo;
	localtime_s(&timeInfo, &rawTime);
	sprintf_s(buffer, 0x10, "%02d-%02d-%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
}

UInt64 GetTimeMs64()
{
	FILETIME ft;
	LARGE_INTEGER li;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	UInt64 ret = li.QuadPart;
	ret -= 116444736000000000LL;
	ret /= 10000;
	return ret;
}

struct ControlName
{
	UInt32		unk00;
	const char	*name;
	UInt32		unk0C;
};

Setting **g_keyNames = (Setting**)0x11D52F0;
Setting **g_mouseButtonNames = (Setting**)0x11D5240;
Setting **g_joystickNames = (Setting**)0x11D51B0;

const char* __fastcall GetDXDescription(UInt32 keyID)
{
	if (keyID <= 220)
	{
		if (g_keyNames[keyID])
			return g_keyNames[keyID]->data.str;
	}
	else if (keyID <= 255);
	else if (keyID <= 263)
	{
		if (g_mouseButtonNames[keyID - 256])
			return g_mouseButtonNames[keyID - 256]->data.str;
	}
	else if (keyID == 264)
		return "WheelUp";
	else if (keyID == 265)
		return "WheelDown";

	return "<no key>";
}

__declspec(naked) UInt32 __fastcall ByteSwap(UInt32 dword)
{
	__asm
	{
		mov		eax, ecx
		bswap	eax
		retn
	}
}

void DumpMemImg(void *data, UInt32 size, UInt8 extra)
{
	UInt32 *ptr = (UInt32*)data;
	//Console_Print("Output");
	PrintDebug("\nDumping  %08X\n", ptr);
	for (UInt32 iter = 0; iter < size; iter += 4, ptr++)
	{
		if (!extra) PrintDebug("%03X\t\t%08X\t", iter, *ptr);
		else if (extra == 1) PrintDebug("%03X\t\t%08X\t[%08X]\t", iter, *ptr, ByteSwap(*ptr));
		else if (extra == 2) PrintDebug("%03X\t\t%08X\t%f", iter, *ptr, *(float*)ptr);
		else if (extra == 3) PrintDebug("%03X\t\t%08X\t[%08X]\t%f", iter, *ptr, ByteSwap(*ptr), *(float*)ptr);
		/*else
		{
			UInt32 addr = *ptr;
			if (!(addr & 3) && (addr > 0x08000000) && (addr < 0x34000000))
				PrintDebug("%03X\t\t%08X\t%08X\t", iter, *ptr, *(UInt32*)addr);
			else PrintDebug("%03X\t\t%08X\t", iter, *ptr);
		}*/
	}
}