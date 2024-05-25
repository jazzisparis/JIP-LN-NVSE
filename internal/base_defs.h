#pragma once

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned long UInt32;
typedef unsigned long long UInt64;
typedef signed char SInt8;
typedef signed short SInt16;
typedef signed long SInt32;
typedef signed long long SInt64;

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx

#define DUP_2(a) a a
#define DUP_3(a) a a a
#define DUP_4(a) a a a a

// These are used for 10h aligning segments in ASM code (massive performance gain, particularly with loops).
#define EMIT(bt) __asm _emit bt

#define NOP_0x1 EMIT(0x90)
//	"\x90"
#define NOP_0x2 EMIT(0x66) NOP_0x1
//	"\x66\x90"
#define NOP_0x3 EMIT(0x0F) EMIT(0x1F) EMIT(0x00)
//	"\x0F\x1F\x00"
#define NOP_0x4 EMIT(0x0F) EMIT(0x1F) EMIT(0x40) EMIT(0x00)
//	"\x0F\x1F\x40\x00"
#define NOP_0x5 EMIT(0x0F) EMIT(0x1F) EMIT(0x44) EMIT(0x00) EMIT(0x00)
//	"\x0F\x1F\x44\x00\x00"
#define NOP_0x6 EMIT(0x66) NOP_0x5
//	"\x66\x0F\x1F\x44\x00\x00"
#define NOP_0x7 EMIT(0x0F) EMIT(0x1F) EMIT(0x80) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00)
//	"\x0F\x1F\x80\x00\x00\x00\x00"
#define NOP_0x8 EMIT(0x0F) EMIT(0x1F) EMIT(0x84) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00) EMIT(0x00)
//	"\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0x9 EMIT(0x66) NOP_0x8
//	"\x66\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0xA EMIT(0x66) NOP_0x9
//	"\x66\x66\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0xB EMIT(0x66) NOP_0xA
//	"\x66\x66\x66\x0F\x1F\x84\x00\x00\x00\x00\x00"
#define NOP_0xC NOP_0x8 NOP_0x4
#define NOP_0xD NOP_0x8 NOP_0x5
#define NOP_0xE NOP_0x7 NOP_0x7
#define NOP_0xF NOP_0x8 NOP_0x7

#define PS_DUP_1(a)	a, 0UL, 0UL, 0UL
#define PS_DUP_2(a)	a, a, 0UL, 0UL
#define PS_DUP_3(a)	a, a, a, 0UL
#define PS_DUP_4(a)	a, a, a, a

#define UBYT(a) *((UInt8*)&a)
#define USHT(a) *((UInt16*)&a)
#define ULNG(a) *((UInt32*)&a)

#define NBYTE(a, n) ((UInt8*)&a)[n]
#define NWORD(a, n) ((UInt16*)&a)[n]

#define GET_N_BYTE(a, n) ((a >> (n * 8)) & 0xFF)

#define EMIT_W(a) EMIT(GET_N_BYTE(a, 0)) EMIT(GET_N_BYTE(a, 1))
#define EMIT_DW(a) EMIT(GET_N_BYTE(a, 0)) EMIT(GET_N_BYTE(a, 1)) EMIT(GET_N_BYTE(a, 2)) EMIT(GET_N_BYTE(a, 3))
#define EMIT_DW_0 DUP_4(EMIT(0x00))
#define EMIT_PS_1(a) EMIT_DW(a) DUP_3(EMIT_DW_0)
#define EMIT_PS_2(a) DUP_2(EMIT_DW(a)) DUP_2(EMIT_DW_0)
#define EMIT_PS_3(a) DUP_3(EMIT_DW(a)) EMIT_DW_0
#define EMIT_PS_4(a) DUP_4(EMIT_DW(a))
#define EMIT_B_8(a0, a1, a2, a3, a4, a5, a6, a7) EMIT(a0) EMIT(a1) EMIT(a2) EMIT(a3) EMIT(a4) EMIT(a5) EMIT(a6) EMIT(a7)
#define EMIT_W_4(a0, a1, a2, a3) EMIT_W(a0) EMIT_W(a1) EMIT_W(a2) EMIT_W(a3)
#define EMIT_W_8(a0, a1, a2, a3, a4, a5, a6, a7) EMIT_W(a0) EMIT_W(a1) EMIT_W(a2) EMIT_W(a3) EMIT_W(a4) EMIT_W(a5) EMIT_W(a6) EMIT_W(a7)
#define EMIT_DW_4(a0, a1, a2, a3) EMIT_DW(a0) EMIT_DW(a1) EMIT_DW(a2) EMIT_DW(a3)

#define AS_CHAR1(a) GET_N_BYTE(a, 0)
#define AS_CHAR2(a) GET_N_BYTE(a, 0), GET_N_BYTE(a, 1)
#define AS_CHAR4(a) GET_N_BYTE(a, 0), GET_N_BYTE(a, 1), GET_N_BYTE(a, 2), GET_N_BYTE(a, 3)
#define AS_CHAR8(a) GET_N_BYTE(a, 0), GET_N_BYTE(a, 1), GET_N_BYTE(a, 2), GET_N_BYTE(a, 3), \
	GET_N_BYTE(a, 4), GET_N_BYTE(a, 5), GET_N_BYTE(a, 6), GET_N_BYTE(a, 7)

#define _MM_SET_EPI8(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, aa, ab, ac, ad, ae, af) \
	{AS_CHAR1(a0), AS_CHAR1(a1), AS_CHAR1(a2), AS_CHAR1(a3), \
	 AS_CHAR1(a4), AS_CHAR1(a5), AS_CHAR1(a6), AS_CHAR1(a7), \
	 AS_CHAR1(a8), AS_CHAR1(a9), AS_CHAR1(aa), AS_CHAR1(ab), \
	 AS_CHAR1(ac), AS_CHAR1(ad), AS_CHAR1(ae), AS_CHAR1(af)}

#define _MM_SET_EPI16(a0, a1, a2, a3, a4, a5, a6, a7) \
	{AS_CHAR2(a0), AS_CHAR2(a1), AS_CHAR2(a2), AS_CHAR2(a3), \
	 AS_CHAR2(a4), AS_CHAR2(a5), AS_CHAR2(a6), AS_CHAR2(a7)}

#define _MM_SET_EPI32(a0, a1, a2, a3) \
	{AS_CHAR4(a0), AS_CHAR4(a1), AS_CHAR4(a2), AS_CHAR4(a3)}

#define _MM_SET_EPI64(a0, a1) {AS_CHAR8(a0), AS_CHAR8(a1)}

#define _MM_SET_EPI32_1(a) _MM_SET_EPI32(a, 0, 0, 0)
#define _MM_SET_EPI32_2(a) _MM_SET_EPI32(a, a, 0, 0)
#define _MM_SET_EPI32_3(a) _MM_SET_EPI32(a, a, a, 0)
#define _MM_SET_EPI32_4(a) _MM_SET_EPI32(a, a, a, a)

#define _MM_SET_EPI64_1(a) _MM_SET_EPI64(a, 0)
#define _MM_SET_EPI64_2(a) _MM_SET_EPI64(a, a)

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

void PrintDebug(const char *fmt, ...);

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

	void Dump()
	{
		char bitStr[numBits << 1];
		bitStr[(numBits << 1) - 1] = 0;
		for (UInt32 i = 0; i < (numBits << 1) - 1; i++)
			if (i & 1)
				bitStr[i] = ' ';
			else bitStr[i] = ((bits >> i) & 1) + '0';
		PrintDebug(bitStr);
	}
};

union Pointers
{
	void	*v;
	char	*c;
	UInt8	*b;
	UInt16	*s;
	UInt32	*l;

	Pointers() {}
	__forceinline Pointers(void *ptr) : v(ptr) {}
	__forceinline Pointers(Pointers &&other) : v(other.v) {}

	__forceinline void operator=(void *ptr) {v = ptr;}

	__forceinline void operator+=(size_t n) {c += n;}
};

union FltAndInt
{
	float		f;
	int			i;
};

union FunctionArg
{
	void		*pVal;
	float		fVal;
	UInt32		uVal;
	SInt32		iVal;

	FunctionArg() {}
	__forceinline FunctionArg(void *_val) : pVal(_val) {}
	__forceinline FunctionArg(float _val) : fVal(_val) {}
	__forceinline FunctionArg(UInt32 _val) : uVal(_val) {}
	__forceinline FunctionArg(SInt32 _val) : iVal(_val) {}

	__forceinline void operator=(void *other) {pVal = other;}
	__forceinline void operator=(float other) {fVal = other;}
	__forceinline void operator=(UInt32 other) {uVal = other;}
	__forceinline void operator=(SInt32 other) {iVal = other;}
};

__forceinline __m128 __vectorcall operator+(__m128 a, __m128 b)
{
	return _mm_add_ps(a, b);
}
__forceinline __m128 __vectorcall operator-(__m128 a, __m128 b)
{
	return _mm_sub_ps(a, b);
}
__forceinline __m128 __vectorcall operator*(__m128 a, __m128 b)
{
	return _mm_mul_ps(a, b);
}
__forceinline __m128 __vectorcall operator&(__m128 a, __m128 b)
{
	return _mm_and_ps(a, b);
}
__forceinline __m128 __vectorcall operator|(__m128 a, __m128 b)
{
	return _mm_or_ps(a, b);
}
__forceinline __m128 __vectorcall operator^(__m128 a, __m128 b)
{
	return _mm_xor_ps(a, b);
}

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

#define COPY_BYTES(dest, src, count) __movsb((UInt8*)(dest), (const UInt8*)(src), count)
#define ZERO_BYTES(addr, size) __stosb((UInt8*)(addr), 0, size)
#define CPY_RET_END(dest, src, length) ((char*)memcpy(dest, src, length + 1) + length)

#define ADDR_AddExtraData		0x40FF60
#define ADDR_RemoveExtraType	0x410140
#define ADDR_LoadModel			0x447080
#define ADDR_ApplyAmmoEffects	0x59A030
#define ADDR_MoveToMarker		0x5CCB20
#define ADDR_ApplyPerkModifiers	0x5E58F0
#define ADDR_ReturnThis			0x6815C0
#define ADDR_PurgeTerminalModel	0x7FFE00
#define ADDR_EquipItem			0x88C650
#define ADDR_UnequipItem		0x88C790
#define ADDR_ReturnTrue			0x8D0360
#define ADDR_TileGetFloat		0xA011B0
#define ADDR_TileSetFloat		0xA012D0
#define ADDR_TileSetString		0xA01350
#define ADDR_InitFontInfo		0xA12020
#define ADDR_GetRandomInt		0xAA5230
#define ADDR_ReturnThis2		0xE68810

#define EXTRA_DATA_CS			0x11C3920
#define SCENE_LIGHTS_CS			0x11F9EA0

#define AS_I32(a) std::bit_cast<UInt32>(a)
#define AS_I64(a) std::bit_cast<UInt64>(a)

#define IS_REFERENCE(form) ((*(UInt32**)form)[0xF0 >> 2] == ADDR_ReturnTrue)
#define NOT_REFERENCE(form) ((*(UInt32**)form)[0xF0 >> 2] != ADDR_ReturnTrue)
#define IS_ACTOR(form) ((*(UInt32**)form)[0x100 >> 2] == ADDR_ReturnTrue)
#define NOT_ACTOR(form) ((*(UInt32**)form)[0x100 >> 2] != ADDR_ReturnTrue)
#define IS_PROJECTILE(form) ((*(UInt32**)form)[0x224 >> 2] == ADDR_ReturnTrue)
#define IS_NODE(object) ((*(UInt32**)object)[0xC >> 2] == ADDR_ReturnThis)
#define IS_GEOMETRY(object) ((*(UInt32**)object)[0x18 >> 2] == ADDR_ReturnThis2)

#define REFR_RES *(UInt32*)result