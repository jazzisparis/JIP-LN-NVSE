#pragma once

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx

#define DUP_2(a) a a
#define DUP_3(a) a a a
#define DUP_4(a) a a a a

// These are used for 10h aligning segments in ASM code (massive performance gain, particularly with loops).
#define EMIT(bt) __asm _emit 0x ## bt

#define NOP_0x1 EMIT(90)
//	"\x90"
#define NOP_0x2 EMIT(66) NOP_0x1
//	"\x66\x90"
#define NOP_0x3 EMIT(0F) EMIT(1F) EMIT(00)
//	"\x0F\x1F\x00"
#define NOP_0x4 EMIT(0F) EMIT(1F) EMIT(40) EMIT(00)
//	"\x0F\x1F\x40\x00"
#define NOP_0x5 EMIT(0F) EMIT(1F) EMIT(44) EMIT(00) EMIT(00)
//	"\x0F\x1F\x44\x00\x00"
#define NOP_0x6 EMIT(66) NOP_0x5
//	"\x66\x0F\x1F\x44\x00\x00"
#define NOP_0x7 EMIT(0F) EMIT(1F) EMIT(80) EMIT(00) EMIT(00) EMIT(00) EMIT(00)
//	"\x0F\x1F\x80\x00\x00\x00\x00"
#define NOP_0x8 EMIT(0F) EMIT(1F) EMIT(84) EMIT(00) EMIT(00) EMIT(00) EMIT(00) EMIT(00)
//	"\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0x9 EMIT(66) NOP_0x8
//	"\x66\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0xA EMIT(66) NOP_0x9
//	"\x66\x66\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0xB EMIT(66) NOP_0xA
//	"\x66\x66\x66\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0xC NOP_0x8 NOP_0x4
#define NOP_0xD NOP_0x8 NOP_0x5
#define NOP_0xE NOP_0x7 NOP_0x7
#define NOP_0xF NOP_0x8 NOP_0x7

#define GAME_HEAP 0x11F6238
#define GAME_HEAP_ALLOC __asm mov ecx, GAME_HEAP CALL_EAX(0xAA3E40)
#define GAME_HEAP_FREE  __asm mov ecx, GAME_HEAP CALL_EAX(0xAA4060)

#define MARK_MODIFIED(form, flag) __asm push 0 __asm push flag __asm push form __asm mov ecx, g_BGSSaveLoadGame __asm mov eax, 0x84A690 __asm call eax

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret ThisCall(UInt32 _addr, void *_this, Args ...args)
{
	return ((T_Ret (__thiscall *)(void*, Args...))_addr)(_this, std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret StdCall(UInt32 _addr, Args ...args)
{
	return ((T_Ret (__stdcall *)(Args...))_addr)(std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret CdeclCall(UInt32 _addr, Args ...args)
{
	return ((T_Ret (__cdecl *)(Args...))_addr)(std::forward<Args>(args)...);
}

void* __stdcall Game_DoHeapAlloc(UInt32 size);
template <typename T = char> __forceinline T* Game_HeapAlloc(size_t count = 1)
{
	return (T*)Game_DoHeapAlloc(count * sizeof(T));
}
__forceinline void Game_HeapFree(void *ptr)
{
	ThisCall<void, void*>(0xAA4060, (void*)GAME_HEAP, ptr);
}

template <typename T = char> __forceinline T* Ni_Alloc(size_t count = 1)
{
	return CdeclCall<T*>(0xAA13E0, count * sizeof(T));
}
template <typename T = char> __forceinline void Ni_Free(T *ptr, size_t count = 1)
{
	CdeclCall(0xAA1460, ptr, count * sizeof(T));
}

#define GAME_RNG 0x11C4180
__forceinline UInt32 GetRandomUInt(UInt32 uBound)
{
	return ThisCall<UInt32, UInt32>(ADDR_GetRandomInt, (void*)GAME_RNG, uBound);
}
__forceinline SInt32 GetRandomIntInRange(SInt32 iMin, SInt32 iMax)
{
	return ThisCall<SInt32, SInt32>(ADDR_GetRandomInt, (void*)GAME_RNG, iMax - iMin) + iMin;
}

#define LOG_HOOKS 0

#define PS_DUP_1(a)	a, 0UL, 0UL, 0UL
#define PS_DUP_2(a)	a, a, 0UL, 0UL
#define PS_DUP_3(a)	a, a, a, 0UL
#define PS_DUP_4(a)	a, a, a, a

#define HEX(a) std::bit_cast<UInt32>(a)
#define UBYT(a) *((UInt8*)&a)
#define USHT(a) *((UInt16*)&a)
#define ULNG(a) *((UInt32*)&a)

extern const UInt32 kPackedValues[];
extern const char kLwrCaseConverter[];

#define GET_PS(i)	((const __m128*)kPackedValues)[i]
#define GET_SS(i)	((const float*)kPackedValues)[i << 2]

#define PS_AbsMask			kPackedValues
#define PS_AbsMask0			kPackedValues+0x10
#define PS_FlipSignMask		kPackedValues+0x20
#define PS_FlipSignMask0	kPackedValues+0x30
#define PS_XYZ0Mask			kPackedValues+0x40
#define PD_AbsMask			kPackedValues+0x50
#define PD_FlipSignMask		kPackedValues+0x60

#define PS_Epsilon			kPackedValues+0x70
#define PS_V3_PId180		kPackedValues+0x80
#define PS_V3_180dPI		kPackedValues+0x90
#define PS_V3_PId2			kPackedValues+0xA0
#define PS_V3_PI			kPackedValues+0xB0
#define PS_V3_PIx2			kPackedValues+0xC0
#define PS_V3_Half			kPackedValues+0xD0
#define PS_V3_One			kPackedValues+0xE0
#define PS_HKUnitCnvrt		kPackedValues+0xF0

#define SS_1d1K				kPackedValues+0x100
#define SS_1d100			kPackedValues+0x104
#define SS_1d10				kPackedValues+0x108
#define SS_1d4				kPackedValues+0x10C
#define SS_3				kPackedValues+0x110
#define SS_10				kPackedValues+0x114
#define SS_100				kPackedValues+0x118

#define FltPId2		1.570796371F
#define FltPI		3.141592741F
#define FltPIx2		6.283185482F
#define FltPId180	0.01745329238F
#define Flt180dPI	57.29578018F
#define DblPId180	0.017453292519943295
#define Dbl180dPI	57.29577951308232

#define EMIT_DW(b0, b1, b2, b3) EMIT(b3) EMIT(b2) EMIT(b1) EMIT(b0)
#define EMIT_DW_3(b0, b1, b2) EMIT_DW(00, b0, b1, b2)
#define EMIT_DW_2(b0, b1) EMIT_DW(00, 00, b0, b1)
#define EMIT_DW_1(b0) EMIT_DW(00, 00, 00, b0)
#define EMIT_PS_1(b0, b1, b2, b3) EMIT_DW(b0, b1, b2, b3) DUP_3(EMIT_DW_1(00))
#define EMIT_PS_2(b0, b1, b2, b3) DUP_2(EMIT_DW(b0, b1, b2, b3)) DUP_2(EMIT_DW_1(00))
#define EMIT_PS_3(b0, b1, b2, b3) DUP_3(EMIT_DW(b0, b1, b2, b3)) EMIT_DW_1(00)
#define EMIT_PS_4(b0, b1, b2, b3) DUP_4(EMIT_DW(b0, b1, b2, b3))
#define EMIT_8(b0, b1, b2, b3, b4, b5, b6, b7) EMIT(b0) EMIT(b1) EMIT(b2) EMIT(b3) EMIT(b4) EMIT(b5) EMIT(b6) EMIT(b7)
#define EMIT_4W(b0, b1, b2, b3, b4, b5, b6, b7) EMIT(b1) EMIT(b0) EMIT(b3) EMIT(b2) EMIT(b5) EMIT(b4) EMIT(b7) EMIT(b6)

typedef void* (__cdecl *memcpy_t)(void*, const void*, size_t);
extern memcpy_t MemCopy;

#define COPY_BYTES(dest, src, count) __movsb((UInt8*)(dest), (const UInt8*)(src), count)
#define ZERO_BYTES(addr, size) __stosb((UInt8*)(addr), 0, size)
#define CPY_RET_END(dest, src, length) ((char*)memcpy(dest, src, length + 1) + length)

#define NRGB(r, g, b) r / 255.0F, g / 255.0F, b / 255.0F

extern UInt32 g_TLSIndex;

//	Workaround used for:
//	* Preventing the compiler from generating _atexit d'tors for static objects.
//	* Bypassing the compiler calling the d'tor on function-scope objects.
template <typename T> class TempObject
{
	alignas(T) UInt8	objData[sizeof(T)];

public:
	TempObject() {Reset();}
	TempObject(const T &src) {memcpy((void*)this, (const void*)&src, sizeof(T));}

	template <typename ...Args>
	TempObject(Args&& ...args)
	{
		new (this) T(std::forward<Args>(args)...);
	}

	void Reset() {new (this) T();}

	void Destroy() {(*this)().~T();}

	T& operator()() {return *(reinterpret_cast<T*>(this));}
	T* operator*() {return reinterpret_cast<T*>(this);}
	T* operator->() {return reinterpret_cast<T*>(this);}

	inline operator T&() {return *(reinterpret_cast<T*>(this));}

	TempObject& operator=(const T &rhs)
	{
		memcpy((void*)this, (const void*)&rhs, sizeof(T));
		return *this;
	}
	TempObject& operator=(const TempObject &rhs)
	{
		memcpy((void*)this, (const void*)&rhs, sizeof(T));
		return *this;
	}
};

template <typename T> class StackObject
{
	alignas(T) UInt8	objData[sizeof(T)];

public:
	StackObject() {}

	T& operator()() {return *(reinterpret_cast<T*>(this));}
	T* operator*() {return reinterpret_cast<T*>(this);}
	T* operator->() {return reinterpret_cast<T*>(this);}
};

//	Swap lhs and rhs, bypassing operator=
template <typename T> __forceinline void RawSwap(T *lhs, T *rhs)
{
	alignas(T) UInt8	buffer[sizeof(T)];
	memcpy((void*)buffer, (const void*)lhs, sizeof(T));
	memcpy((void*)lhs, (const void*)rhs, sizeof(T));
	memcpy((void*)rhs, (const void*)buffer, sizeof(T));
}

class CriticalSection : public CRITICAL_SECTION
{
public:
	CriticalSection() {InitializeCriticalSection(this);}
	~CriticalSection() {DeleteCriticalSection(this);}

	void Enter() {EnterCriticalSection(this);}
	void Leave() {LeaveCriticalSection(this);}
	bool TryEnter() {return TryEnterCriticalSection(this) != 0;}
};

class PrimitiveCS
{
	UInt32		selfPtr = 0;

public:
	PrimitiveCS *Enter();
	__forceinline void Leave() {selfPtr &= 0;}
};

class LightCS
{
	UInt32	owningThread = 0;
	UInt32	enterCount = 0;

public:
	void Enter();
	__forceinline void Leave()
	{
		if (!--enterCount)
			owningThread &= 0;
	}
};

template <typename T_CS> class ScopedLock
{
	T_CS		*cs;

public:
	ScopedLock(T_CS *_cs) : cs(_cs) {cs->Enter();}
	~ScopedLock() {cs->Leave();}
};

typedef ScopedLock<CriticalSection> ScopedCS;
typedef ScopedLock<PrimitiveCS> ScopedPrimitiveCS;
typedef ScopedLock<LightCS> ScopedLightCS;

union FunctionArg
{
	void		*pVal;
	float		fVal;
	UInt32		uVal;
	SInt32		iVal;

	inline void operator=(void *other) {pVal = other;}
	inline void operator=(float other) {fVal = other;}
	inline void operator=(UInt32 other) {uVal = other;}
	inline void operator=(SInt32 other) {iVal = other;}
};

template <const size_t numBits> struct BitField
{
	static_assert((numBits == 8) || (numBits == 16) || (numBits == 32));
	using BITS = std::conditional_t<(numBits == 8), UInt8, std::conditional_t<(numBits == 16), UInt16, UInt32>>;
	
	BITS		bits;

	BitField(BITS _init = 0) : bits(_init) {}

	inline void operator=(BITS rhs) {bits = rhs;}

	inline bool operator()(BITS bitMask) const {return (bits & bitMask) != 0;}
	inline void operator|=(BITS bitMask) {bits |= bitMask;}
	inline void operator&=(BITS bitMask) {bits &= ~bitMask;}
	inline void operator^=(BITS bitMask) {bits ^= bitMask;}
	inline bool operator[](UInt8 bitIndex) const {return (bits & (1 << bitIndex)) != 0;}
	inline void operator+=(UInt8 bitIndex) {bits |= (1 << bitIndex);}
	inline void operator-=(UInt8 bitIndex) {bits &= ~(1 << bitIndex);}
};

template <typename T_Array> class ArrayUtils
{
	using T_Data = T_Array::Element;
	using Data_Arg = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, const T_Data&>;
	using Data_Val = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&&>;
	
	static bool __vectorcall CompareLT(Data_Arg lhs, Data_Arg rhs) {return lhs < rhs;}
	static bool __vectorcall CompareGT(Data_Arg lhs, Data_Arg rhs) {return rhs < lhs;}

	typedef bool (__vectorcall *SortComperator)(Data_Arg, Data_Arg);
	static void QuickSort(T_Array &array, UInt32 l, UInt32 h, SortComperator comperator)
	{
		UInt32 i = l;
		for (UInt32 j = l + 1; j < h; j++)
			if (!comperator(array[l], array[j]))
				RawSwap<T_Data>(&array[++i], &array[j]);
		RawSwap<T_Data>(&array[l], &array[i]);
		if (l < i)
			QuickSort(array, l, i, comperator);
		if (++i < h)
			QuickSort(array, i, h, comperator);
	}
	
public:
	static void Sort(T_Array &array, bool descending = false)
	{
		if (array.Size() > 1)
			QuickSort(array, 0, array.Size(), descending ? CompareGT : CompareLT);
	}

	static void Sort(T_Array &array, SortComperator comperator)
	{
		if (array.Size() > 1)
			QuickSort(array, 0, array.Size(), comperator);
	}

	__declspec(noinline) static UInt32 InsertSorted(T_Array &array, Data_Val item, SortComperator comperator)
	{
		UInt32 lBound = 0, uBound = array.Size();
		while (lBound != uBound)
		{
			UInt32 index = (lBound + uBound) >> 1;
			if (comperator(item, array[index]))
				uBound = index;
			else lBound = index + 1;
		}
		uBound = array.Size() - lBound;
		T_Data *pData = array.AllocateData();
		if (uBound)
		{
			pData = array.Data() + lBound;
			memmove(pData + 1, pData, sizeof(T_Data) * uBound);
		}
		*pData = std::move(item);
		return lBound;
	}
	
	static UInt32 InsertSorted(T_Array &array, Data_Val item, bool descending = false)
	{
		return InsertSorted(array, std::forward<T_Data>(item), descending ? CompareGT : CompareLT);
	}

	static void Shuffle(T_Array &array)
	{
		T_Data *pData = array.Data();
		for (UInt32 count = array.Size(); count > 1; count--, pData++)
			if (UInt32 rand = GetRandomUInt(count))
				RawSwap<T_Data>(pData, &pData[rand]);
	}

	static void Reverse(T_Array &array)
	{
		if (array.Size() > 1)
			for (UInt32 ftIdx = 0, bkIdx = array.Size() - 1; ftIdx < bkIdx; ftIdx++, bkIdx--)
				RawSwap<T_Data>(&array[ftIdx], &array[bkIdx]);
	}
};

template <typename T_List> class LinkedListUtils
{
	using T_Data = T_List::Element;
	using Node = T_List::Node;

	static void SwapData(Node *n1, Node *n2) {RawSwap<T_Data>(&n1->data, &n2->data);}
	
	static bool __fastcall CompareLT(const Node *n1, const Node *n2) {return n1->data < n2->data;}
	static bool __fastcall CompareGT(const Node *n1, const Node *n2) {return n2->data < n1->data;}

	typedef bool (__fastcall *SortComperator)(const Node*, const Node*);
	static void QuickSort(Node *lNode, UInt32 l, UInt32 h, SortComperator comperator)
	{
		UInt32 i = l;
		Node *iNode = lNode, *jNode = lNode->next;
		for (UInt32 j = l + 1; j < h; j++, jNode = jNode->next)
			if (!comperator(lNode, jNode))
			{
				i++;
				iNode = iNode->next;
				SwapData(jNode, iNode);
			}
		SwapData(lNode, iNode);
		if (l < i)
			QuickSort(lNode, l, i, comperator);
		if (++i < h)
			QuickSort(iNode->next, i, h, comperator);
	}
	
public:
	static void Sort(T_List &list, bool descending = false)
	{
		if (UInt32 count = list.Count(); count > 1)
			QuickSort(list.Head(), 0, count, descending ? CompareGT : CompareLT);
	}

	static void Sort(T_List &list, SortComperator comperator)
	{
		if (UInt32 count = list.Count(); count > 1)
			QuickSort(list.Head(), 0, count, comperator);
	}

	static void Shuffle(T_List &list)
	{
		Node *head = list.Head();
		for (UInt32 count = list.Count(); count > 1; count--, head = head->next)
			if (UInt32 rand = GetRandomUInt(count))
				SwapData(head, head->GetNth(rand));
	}

	static void Exchange(T_List &list, UInt32 idx1, UInt32 idx2)
	{
		if (list.Empty()) return;
		if (idx1 > idx2)
		{
			UInt32 tmp = idx1;
			idx1 = idx2;
			idx2 = tmp;
		}
		Node *iter = list.Head(), *node1 = nullptr;
		UInt32 idx = 0;
		do
		{
			if (!node1)
			{
				if (idx == idx1)
					node1 = iter;
			}
			else if (idx == idx2)
			{
				SwapData(node1, iter);
				break;
			}
			idx++;
		}
		while (iter = iter->next);
	}
};

void PrintLog(const char *fmt, ...);
void PrintDebug(const char *fmt, ...);

template <typename T_HashMap> class HashMapUtils
{
	using Entry = T_HashMap::Entry;
	using Bucket = T_HashMap::Bucket;
	
public:
	static void DumpLoads(const T_HashMap &map)
	{
		UInt32 loadsArray[0x40];
		ZERO_BYTES(loadsArray, sizeof(loadsArray));
		UInt32 maxLoad = 0;
		for (Bucket *pBucket = map.GetBuckets(), *pEnd = map.End(); pBucket != pEnd; pBucket++)
		{
			UInt32 entryCount = pBucket->Size();
			loadsArray[entryCount]++;
			if (maxLoad < entryCount)
				maxLoad = entryCount;
		}
		PrintDebug("Size = %d\nBuckets = %d\n----------------\n", map.Size(), map.BucketCount());
		for (UInt32 iter = 0; iter <= maxLoad; iter++)
			if (loadsArray[iter]) PrintDebug("%d:\t%05d (%.4f%%)", iter, loadsArray[iter], 100.0 * (double)loadsArray[iter] / map.Size());
	}
};

#define Use_ArrayUtils(cont_type, data_type)	\
	using Element = data_type;	\
	friend ArrayUtils<cont_type>;
#define Use_LinkedListUtils(cont_type, data_type)	\
	using Element = data_type;	\
	friend LinkedListUtils<cont_type>;
#define Use_HashMapUtils(cont_type)	\
	friend HashMapUtils<cont_type>;

struct CellCoord
{
	SInt32		x;
	SInt32		y;
};

union Coordinate
{
	UInt32		xy;
	struct
	{
		SInt16	y;
		SInt16	x;
	};

	Coordinate() {}
	Coordinate(SInt16 _x, SInt16 _y) : x(_x), y(_y) {}
	Coordinate(UInt32 _xy) : xy(_xy) {}
	Coordinate(const CellCoord &coord) {*this = coord;}

	inline void operator=(const Coordinate &rhs) {xy = rhs.xy;}
	inline void operator=(UInt32 rhs) {xy = rhs;}
	inline void operator=(__m128i rhs) {_mm_storeu_si32(this, rhs);}
	inline void operator=(const CellCoord &rhs) {_mm_storeu_si32(this, _mm_shufflelo_epi16(_mm_loadu_si64(&rhs), 2));}

	inline bool operator==(const Coordinate &rhs) {return xy == rhs.xy;}
	inline bool operator!=(const Coordinate &rhs) {return xy != rhs.xy;}

	inline __m128i operator+(const Coordinate &rhs)
	{
		return _mm_add_epi16(*this, rhs);
	}

	inline operator UInt32() const {return xy;}
	inline operator __m128i() const {return _mm_loadu_si32(this);}
};

template <typename T1, typename T2> __forceinline T1 GetMin(T1 value1, T2 value2)
{
	return (value1 < value2) ? value1 : value2;
}

template <typename T1, typename T2> __forceinline T1 GetMax(T1 value1, T2 value2)
{
	return (value1 > value2) ? value1 : value2;
}

template <typename T> __forceinline T sqr(T value)
{
	return value * value;
}

extern UInt32 s_CPUFeatures;
UInt32 GetCPUFeatures();

UInt32 __vectorcall cvtd2ul(double value);

double __vectorcall cvtul2d(UInt32 value);
void __fastcall cvtul2d(UInt32 value, double *result);

int __vectorcall ifloor(float value);

int __vectorcall iceil(float value);

__forceinline int iround(float value)
{
	return _mm_cvt_ss2si(_mm_load_ss(&value));
}

float __vectorcall fMod(float numer, float denom);

float __vectorcall Cos(float angle);
__forceinline float Sin(float angle)
{
	return Cos(FltPId2 - angle);
}
float __vectorcall Tan(float angle);

//	Takes:   xmm0 = {a, 0, 0, 0};
//	Returns: xmm0 = {sin(a), cos(a), 0, 0},
//			 xmm1 = {cos(a), 0, 0, 0}
__m128 __vectorcall GetSinCos(float angle);

//	Takes:   xmm0 = {x, y, z, 0};
//	Returns: xmm0 = {sin(x), sin(y), sin(z), 0},
//			 xmm1 = {cos(x), cos(y), cos(z), 0}
__m128 __vectorcall GetSinCos_V3(__m128 angles);

float __vectorcall ASin(float x);
float __vectorcall ACos(float x);
float __vectorcall ATan2(float y, float x);
__forceinline float ATan(float x)
{
	return ATan2(x, 1.0F);
}

float __vectorcall Length_V4(__m128 inPS);
__m128 __vectorcall Normalize_V4(__m128 inPS);
bool __vectorcall Equal_V3(__m128 v1, __m128 v2);
bool __vectorcall Equal_V4(__m128 v1, __m128 v2);

#define STR_BUFFER_SIZE 0x20000
char *GetStrArgBuffer();

void __fastcall NiReleaseObject(NiRefObject *toRelease);

NiRefObject** __stdcall NiReplaceObject(void *toRelease, NiRefObject *toAdd);

UInt32 __fastcall RGBHexToDec(UInt32 rgb);

UInt32 __fastcall RGBDecToHex(UInt32 rgb);

UInt32 __fastcall StrLen(const char *str);

bool __fastcall MemCmp(const void *ptr1, const void *ptr2, UInt32 bsize);

char* __fastcall StrCopy(char *dest, const char *src);

char* __fastcall StrNCopy(char *dest, const char *src, UInt32 length);

char* __fastcall StrCat(char *dest, const char *src);

char __fastcall StrCompareCS(const char *lstr, const char *rstr);

char __fastcall StrCompareCI(const char *lstr, const char *rstr);

char __fastcall StrBeginsCS(const char *lstr, const char *rstr);

char __fastcall StrBeginsCI(const char *lstr, const char *rstr);

void __fastcall StrToCase(char *str, UInt32 upper);

__forceinline void StrToLower(char *str)
{
	StrToCase(str, 0);
}
__forceinline void StrToUpper(char *str)
{
	StrToCase(str, 0x10);
}

void __fastcall ReplaceChr(char *str, char from, char to);

char* __fastcall FindChr(const char *str, char chr);

char* __fastcall FindChrR(const char *str, char chr);

const char* __fastcall SubStrCI(const char *srcStr, const char *subStr);

char* __fastcall SlashPos(const char *str);

char* __fastcall GetNextToken(char *str, char delim);

char* __fastcall GetNextToken(char *str, const char *delims);

char* __fastcall CopyString(const char *key);

char* __fastcall CopyCString(const char *src);

char* __fastcall IntToStr(char *str, int num);

char* __vectorcall FltToStr(char *str, double value);

int __fastcall StrToInt(const char *str);

UInt32 __fastcall StrToUInt(const char *str);

double __vectorcall StrToDbl(const char *str);

char* __fastcall UIntToHex(char *str, UInt32 num);

UInt32 __fastcall HexToUInt(const char *str);

template <size_t size> class ScopedString
{
	char		*theStr;

public:
	ScopedString(char *inStr) : theStr(inStr) {}
	~ScopedString() {Pool_CFree(theStr, size);}

	inline char *Get() const {return theStr;}
	inline operator char*() const {return theStr;}
};

class DString
{
	char		*str;
	UInt16		alloc;
	UInt16		length;

	DString(char *_str, UInt16 _length, UInt16 _alloc) : str(_str), alloc(_alloc), length(_length) {}

public:
	DString() : str(nullptr) {ULNG(alloc) = 0;}
	DString(const char *from);
	DString(const DString &from);
	DString(UInt16 _alloc);

	~DString()
	{
		if (str)
			Pool_CFree(str, alloc);
	}

	const char *CString() const {return str ? str : "";}
	char *Data() const {return str;}

	UInt32 Size() const {return length;}
	bool Empty() const {return !length;}
	UInt32 AllocSize() const {return alloc;}

	void Reserve(UInt16 size);

	void Clear()
	{
		if (length)
		{
			length = 0;
			*str = 0;
		}
	}

	char At(UInt16 index) const {return (index < length) ? str[index] : 0;}

	char& operator[](UInt16 index) const {return str[index];}

	DString& operator=(const char *other);
	DString& operator=(const DString &other);

	DString& operator+=(char chr);
	DString& operator+=(const char *other);
	DString& operator+=(const DString &other);

	bool operator==(const char *other) const;
	bool operator==(const DString &other) const;

	DString& Insert(UInt16 index, char chr);
	DString& Insert(UInt16 index, const char *other);
	DString& Insert(UInt16 index, const DString &other);

	DString& Erase(UInt16 index, UInt16 count);

	DString& Replace(UInt16 bgnIdx, const char *other);
	DString& Replace(UInt16 bgnIdx, const DString &other);

	DString SubString(UInt16 bgnIdx, UInt16 endIdx);

	DString ToLower();
	DString ToUpper();

	friend DString operator+(const DString &lStr, char rChr);
	friend DString operator+(const DString &lStr, const char *rStr);
	friend DString operator+(const char *lStr, const DString &rStr);
};

class XString
{
	UInt16		alloc;
	UInt16		length;
	char		*str;
	
public:
	inline void Reset()
	{
		ULNG(alloc) = 0;
		str = nullptr;
	}

	inline void Free() {if (str) Pool_CFree(str, alloc);}
	
	inline const char *CString() const {return str ? str : "";}
	inline char *Data() const {return str;}

	inline UInt32 Size() const {return length;}
	inline bool Empty() const {return !length;}
	
	void InitFromBuffer(const char *inStr, UInt32 len);

	inline void InitCopy(const XString &from)
	{
		if (from.length)
			InitFromBuffer(from.str, from.length);
		else Reset();
	}
	
	void operator=(const XString &other);
	void operator=(const char *other);
	
	bool operator==(const XString &other) const;
};

#define AUX_BUFFER_INIT_SIZE 0x8000

class AuxBuffer
{
	UInt8		*ptr = nullptr;
	UInt32		size = AUX_BUFFER_INIT_SIZE;

	static UInt8 *Alloc(UInt32 bufIdx, UInt32 reqSize);

public:
	template <typename T> static T *Get(UInt32 bufIdx, UInt32 numElements, bool clear = true)
	{
		UInt32 required = numElements * sizeof(T);
		T *resPtr = (T*)Alloc(bufIdx, required);
		if (clear)
			ZERO_BYTES(resPtr, required);
		return resPtr;
	}

	template <typename T> static T *Copy(UInt32 bufIdx, UInt32 numElements, T *data)
	{
		UInt32 required = numElements * sizeof(T);
		return (T*)memcpy(Alloc(bufIdx, required), data, required);
	}
};

bool __fastcall FileExists(const char *filePath);

class FileStream
{
	FILE		*theFile;

public:
	FileStream() : theFile(nullptr) {}
	FileStream(const char *filePath);
	FileStream(const char *filePath, UInt32 inOffset);
	~FileStream() {if (theFile) fclose(theFile);}

	bool Open(const char *filePath);
	bool OpenAt(const char *filePath, UInt32 inOffset);
	bool OpenWrite(char *filePath, bool append);
	void SetOffset(UInt32 inOffset);

	void Close()
	{
		fclose(theFile);
		theFile = nullptr;
	}

	explicit operator bool() const {return theFile != nullptr;}

	FILE *GetStream() const {return theFile;}
	UInt32 GetLength() const;
	char ReadChar();
	void ReadBuf(void *outData, UInt32 inLength);
	void WriteChar(char chr);
	void WriteStr(const char *inStr);
	void WriteBuf(const void *inData, UInt32 inLength);
	int WriteFmtStr(const char *fmt, ...);

	static void __fastcall MakeAllDirs(char *fullPath);
};

extern const char kIndentLevelStr[];

class DebugLog
{
	FILE			*theFile = nullptr;
	UInt32			indent = 0x28;

public:
	~DebugLog() {if (theFile) fclose(theFile);}

	bool Create(const char *filePath);
	FILE *GetStream() const {return theFile;}
	void Message(const char *msgStr);
	void FmtMessage(const char *fmt, va_list args);
	void Indent() {if (indent) indent--;}
	void Outdent() {if (indent < 40) indent++;}

	explicit operator bool() const {return theFile != nullptr;}
};

extern TempObject<DebugLog> s_log, s_debug;

class LineIterator
{
	char	*dataPtr;

public:
	LineIterator(const char *filePath, char *buffer);

	explicit operator bool() const {return *dataPtr != 3;}
	char* operator*() {return dataPtr;}
	void operator++()
	{
		dataPtr += StrLen(dataPtr);
		while (!*dataPtr)
			dataPtr++;
	}
};

class DirectoryIterator
{
	HANDLE				handle;
	WIN32_FIND_DATA		fndData;

public:
	DirectoryIterator(const char *path) : handle(FindFirstFile(path, &fndData)) {}
	~DirectoryIterator() {Close();}

	bool IsFile() const {return !(fndData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);}
	bool IsFolder() const
	{
		if (IsFile())
			return false;
		UInt32 prefix = *(UInt32*)fndData.cFileName;
		return ((prefix & 0xFFFF) != '.') && ((prefix & 0xFFFFFF) != '..');
	}
	void Close()
	{
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}

	explicit operator bool() const {return handle != INVALID_HANDLE_VALUE;}
	const char* operator*() const {return fndData.cFileName;}
	void operator++() {if (!FindNextFile(handle, &fndData)) Close();}
};

UInt32 __fastcall FileToBuffer(const char *filePath, char *buffer, UInt32 maxLen);

void __stdcall StoreOriginalData(UInt32 addr, UInt8 size);

void __stdcall SafeWrite8(UInt32 addr, UInt32 data);
void __stdcall SafeWrite16(UInt32 addr, UInt32 data);
void __stdcall SafeWrite32(UInt32 addr, UInt32 data);
void __stdcall SafeWriteBuf(UInt32 addr, const void *data, UInt32 len);

#define SAFE_WRITE_BUF(addr, data) SafeWriteBuf(addr, data, sizeof(data) - 1)

// 5 bytes
void __stdcall WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt);
void __stdcall WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt);

// 10 bytes
void __stdcall WritePushRetRelJump(UInt32 baseAddr, UInt32 retAddr, UInt32 jumpTgt);

void __fastcall GetTimeStamp(char *buffer);

UInt64 GetTimeMs64();

const char* __fastcall GetDXDescription(UInt32 keyID);

void __stdcall DumpMemImg(void *data, UInt32 size, UInt8 extra = 0);