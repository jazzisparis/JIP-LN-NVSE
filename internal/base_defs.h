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

#define PS_DUP_1(a)	a, 0UL, 0UL, 0UL
#define PS_DUP_2(a)	a, a, 0UL, 0UL
#define PS_DUP_3(a)	a, a, a, 0UL
#define PS_DUP_4(a)	a, a, a, a

#define UBYT(a) *((UInt8*)&a)
#define USHT(a) *((UInt16*)&a)
#define ULNG(a) *((UInt32*)&a)

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

#define HEX(a) std::bit_cast<UInt32>(a)

#define IS_REFERENCE(form) ((*(UInt32**)form)[0xF0 >> 2] == ADDR_ReturnTrue)
#define NOT_REFERENCE(form) ((*(UInt32**)form)[0xF0 >> 2] != ADDR_ReturnTrue)
#define IS_ACTOR(form) ((*(UInt32**)form)[0x100 >> 2] == ADDR_ReturnTrue)
#define NOT_ACTOR(form) ((*(UInt32**)form)[0x100 >> 2] != ADDR_ReturnTrue)
#define IS_PROJECTILE(form) ((*(UInt32**)form)[0x224 >> 2] == ADDR_ReturnTrue)
#define IS_NODE(object) ((*(UInt32**)object)[0xC >> 2] == ADDR_ReturnThis)
#define IS_GEOMETRY(object) ((*(UInt32**)object)[0x18 >> 2] == ADDR_ReturnThis2)

extern const bool kInventoryType[];
extern const UInt8 kMaterialConvert[];
extern const char kDaysPerMonth[], kMenuIDJumpTable[];
extern bool s_NPCWeaponMods, s_NPCPerks;