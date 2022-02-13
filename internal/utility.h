#pragma once

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx

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

#define GAME_HEAP_ALLOC __asm mov ecx, 0x11F6238 CALL_EAX(0xAA3E40)
#define GAME_HEAP_FREE  __asm mov ecx, 0x11F6238 CALL_EAX(0xAA4060)

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

#define GameHeapAlloc(size) ThisCall<void*, UInt32>(0xAA3E40, (void*)0x11F6238, size)
#define GameHeapFree(ptr) ThisCall<void, void*>(0xAA4060, (void*)0x11F6238, ptr)

#define GetRandomInt(n) ThisCall<SInt32, SInt32>(0xAA5230, (void*)0x11C4180, n)
#define GetRandomIntInRange(iMin, iMax) (GetRandomInt(iMax - iMin) + iMin)

#define LOG_HOOKS 0

#define PS_DUP_1(a)	a, 0UL, 0UL, 0UL
#define PS_DUP_2(a)	a, a, 0UL, 0UL
#define PS_DUP_3(a)	a, a, a, 0UL
#define PS_DUP_4(a)	a, a, a, a

union HexFloat
{
	float	f;
	UInt32	h;

	constexpr HexFloat(const float _f) : f(_f) {}
	constexpr HexFloat(const UInt32 _h) : h(_h) {}
};

extern const HexFloat kPackedValues[];
extern const char kLwrCaseConverter[], kUprCaseConverter[];

#define GET_PS(i)	((const __m128*)kPackedValues)[i]

#define PS_AbsMask			kPackedValues
#define PS_AbsMask0			kPackedValues+0x10
#define PS_FlipSignMask		kPackedValues+0x20
#define PS_FlipSignMask0	kPackedValues+0x30
#define PS_Discard3rdMask	kPackedValues+0x40
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

#define SS_1d1K				kPackedValues+0xF0
#define SS_1d100			kPackedValues+0xF4
#define SS_1d10				kPackedValues+0xF8
#define SS_1d4				kPackedValues+0xFC
#define SS_3				kPackedValues+0x100
#define SS_10				kPackedValues+0x104
#define SS_100				kPackedValues+0x108

#define FltPId2		1.570796371F
#define FltPI		3.141592741F
#define FltPIx2		6.283185482F
#define Flt2dPI		0.6366197467F
#define FltPId180	0.01745329238F
#define Flt180dPI	57.29578018F
#define DblPId180	0.017453292519943295
#define Dbl180dPI	57.29577951308232

typedef void* (__cdecl *memcpy_t)(void*, const void*, size_t);
extern memcpy_t MemCopy, MemMove;

//	Workaround for bypassing the compiler calling the d'tor on function-scope objects.
template <typename T> class TempObject
{
	friend T;

	struct Buffer
	{
		UInt8	bytes[sizeof(T)];
	}
	objData;

public:
	TempObject() {Reset();}
	TempObject(const T &src) {memcpy((void*)&objData, (const void*)&src, sizeof(T));}

	void Reset() {new ((T*)&objData) T();}

	T& operator()() {return *(T*)&objData;}

	TempObject& operator=(const T &rhs)
	{
		memcpy((void*)&objData, (const void*)&rhs, sizeof(T));
		return *this;
	}
	TempObject& operator=(const TempObject &rhs)
	{
		memcpy((void*)&objData, (const void*)&rhs.objData, sizeof(T));
		return *this;
	}
};

//	Swap lhs and rhs, bypassing operator=
template <typename T> __forceinline void RawSwap(const T &lhs, const T &rhs)
{
	UInt8 buffer[sizeof(T)];
	memcpy((void*)buffer, (const void*)&lhs, sizeof(T));
	memcpy((void*)&lhs, (const void*)&rhs, sizeof(T));
	memcpy((void*)&rhs, (const void*)buffer, sizeof(T));
}

class CriticalSection
{
	CRITICAL_SECTION	critSection;

public:
	CriticalSection() {InitializeCriticalSection(&critSection);}
	~CriticalSection() {DeleteCriticalSection(&critSection);}

	void Enter() {EnterCriticalSection(&critSection);}
	void Leave() {LeaveCriticalSection(&critSection);}
	bool TryEnter() {return TryEnterCriticalSection(&critSection) != 0;}
};

class PrimitiveCS
{
	UInt32	owningThread;

public:
	PrimitiveCS() : owningThread(0) {}

	PrimitiveCS *Enter();
	__forceinline void Leave() {owningThread = 0;}
};

class LightCS
{
	UInt32	owningThread;
	UInt32	enterCount;

public:
	LightCS() : owningThread(0), enterCount(0) {}

	void Enter();
	__forceinline void Leave()
	{
		if (!--enterCount)
			owningThread = 0;
	}
};

template <typename T_CS> class ScopedLock
{
	T_CS		*cs;

public:
	ScopedLock(T_CS *_cs) : cs(_cs) {cs->Enter();}
	~ScopedLock() {cs->Leave();}
};

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

TESForm* __stdcall LookupFormByRefID(UInt32 refID);

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
	Coordinate(SInt32 *_x) : x(_x[0]), y(_x[1]) {}

	inline void operator=(const Coordinate &rhs) {xy = rhs.xy;}
	inline void operator=(UInt32 rhs) {xy = rhs;}

	inline bool operator==(const Coordinate &rhs) {return xy == rhs.xy;}
	inline bool operator!=(const Coordinate &rhs) {return xy != rhs.xy;}

	inline Coordinate operator+(const Coordinate &rhs)
	{
		return Coordinate(x + rhs.x, y + rhs.y);
	}

	inline operator UInt32() const {return xy;}
};

template <typename T1, typename T2> inline T1 GetMin(T1 value1, T2 value2)
{
	return (value1 < value2) ? value1 : value2;
}

template <typename T1, typename T2> inline T1 GetMax(T1 value1, T2 value2)
{
	return (value1 > value2) ? value1 : value2;
}

template <typename T> inline T sqr(T value)
{
	return value * value;
}

UInt32 __vectorcall cvtd2ui(double value);

double __fastcall cvtui2d(UInt32 value);
void __fastcall cvtui2d(UInt32 value, double *result);

int __vectorcall ifloor(float value);

int __vectorcall iceil(float value);

__forceinline int iround(float value)
{
	return _mm_cvt_ss2si(_mm_load_ss(&value));
}

float __vectorcall fMod(float numer, float denom);

float __vectorcall Sin(float angle);
float __vectorcall Cos(float angle);
float __vectorcall Tan(float angle);

//	Takes:   xmm0 = {a, 0, 0, 0};
//	Returns: xmm0 = {sin(a), cos(a), 0, 0},
//			 xmm1 = {cos(a), 0, 0, 0}
__m128 __vectorcall GetSinCos(float angle);

//	Takes:   xmm0 = {x, y, z, 0};
//	Returns: xmm0 = {sin(x), sin(y), sin(z), 0},
//			 xmm1 = {cos(x), cos(y), cos(z), 0}
__m128 __vectorcall GetSinCosV3(__m128 angles);

float __vectorcall ASin(float x);
float __vectorcall ACos(float x);
float __vectorcall ATan(float x);
float __vectorcall ATan2(float y, float x);

char *GetStrArgBuffer();

void __fastcall NiReleaseObject(NiRefObject *toRelease);

NiRefObject** __stdcall NiReleaseAddRef(void *toRelease, NiRefObject *toAdd);

UInt32 __fastcall RGBHexToDec(UInt32 rgb);

UInt32 __fastcall RGBDecToHex(UInt32 rgb);

UInt32 __fastcall StrLen(const char *str);

bool __fastcall MemCmp(const void *ptr1, const void *ptr2, UInt32 bsize);

void __fastcall MemZero(void *dest, UInt32 bsize);

char* __fastcall StrCopy(char *dest, const char *src);

char* __fastcall StrNCopy(char *dest, const char *src, UInt32 length);

char* __fastcall StrLenCopy(char *dest, const char *src, UInt32 length);

char* __fastcall StrCat(char *dest, const char *src);

char __fastcall StrCompare(const char *lstr, const char *rstr);

char __fastcall StrBeginsCS(const char *lstr, const char *rstr);

char __fastcall StrBeginsCI(const char *lstr, const char *rstr);

void __fastcall FixPath(char *str);

void __fastcall StrToLower(char *str);

void __fastcall StrToUpper(char *str);

void __fastcall ReplaceChr(char *str, char from, char to);

char* __fastcall FindChr(const char *str, char chr);

char* __fastcall FindChrR(const char *str, UInt32 length, char chr);

char* __fastcall SubStrCI(const char *srcStr, const char *subStr);

char* __fastcall SlashPos(const char *str);

char* __fastcall SlashPosR(const char *str);

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

class TempCString
{
	char		*str;

	void Clear()
	{
		if (!str) return;
		free(str);
		str = NULL;
	}

public:
	TempCString() : str(NULL) {}
	TempCString(UInt32 initSize)
	{
		str = (char*)malloc(initSize + 1);
		*str = 0;
	}
	TempCString(const char *src)
	{
		str = src ? CopyString(src) : NULL;
	}
	TempCString(const TempCString &src)
	{
		str = src.str ? CopyString(src.str) : NULL;
	}

	~TempCString() {Clear();}

	const char *CString() const {return str ? str : "";}
	char *Data() {return str;}

	bool operator==(const TempCString &rhs) {return !StrCompare(str, rhs.str);}
	bool operator<(const TempCString &rhs) {return StrCompare(str, rhs.str) < 0;}
	bool operator>(const TempCString &rhs) {return StrCompare(str, rhs.str) > 0;}

	char& operator[](UInt32 index) {return str[index];}

	TempCString& operator=(const TempCString &rhs)
	{
		if (this != &rhs)
		{
			Clear();
			if (rhs.str)
				str = CopyString(rhs.str);
		}
		return *this;
	}
	TempCString& operator=(const char *srcStr)
	{
		if (str != srcStr)
		{
			Clear();
			if (srcStr)
				str = CopyString(srcStr);
		}
		return *this;
	}
};

class DString
{
	char		*str;
	UInt16		alloc;
	UInt16		length;

	DString(char *_str, UInt16 _length, UInt16 _alloc) : str(_str), alloc(_alloc), length(_length) {}

public:
	DString() : str(NULL) {*(UInt32*)&alloc = 0;}
	DString(const char *from);
	DString(const DString &from);
	DString(UInt16 _alloc);

	~DString()
	{
		if (str)
		{
			Pool_Free(str, alloc);
			str = NULL;
			*(UInt32*)&alloc = 0;
		}
	}

	const char *CString() const {return str ? str : "";}
	char *Data() {return str;}

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

	bool operator==(const char *other);
	bool operator==(const DString &other);

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

#define AUX_BUFFER_INIT_SIZE 0x8000

class AuxBuffer
{
	UInt8		*ptr;
	UInt32		size;

public:
	AuxBuffer() : ptr(NULL), size(AUX_BUFFER_INIT_SIZE) {}

	static UInt8 *Get(UInt32 bufIdx, UInt32 reqSize);
};

bool __fastcall FileExists(const char *filePath);

class FileStream
{
	FILE		*theFile;

public:
	FileStream() : theFile(NULL) {}
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
		theFile = NULL;
	}

	explicit operator bool() const {return theFile != NULL;}

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
	FILE			*theFile;
	UInt32			indent;

public:
	DebugLog() : theFile(NULL), indent(40) {}
	~DebugLog() {if (theFile) fclose(theFile);}

	bool Create(const char *filePath);
	void Message(const char *msgStr);
	void FmtMessage(const char *fmt, va_list args);
	void Indent() {if (indent) indent--;}
	void Outdent() {if (indent < 40) indent++;}
};

extern DebugLog s_log, s_debug;

void PrintLog(const char *fmt, ...);
void PrintDebug(const char *fmt, ...);

class LineIterator
{
	char	*dataPtr;

public:
	LineIterator(const char *filePath, char *buffer);

	explicit operator bool() const {return *dataPtr != 3;}
	char* operator*() {return dataPtr;}
	void operator++()
	{
		while (*dataPtr)
			dataPtr++;
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
		if (fndData.cFileName[0] != '.')
			return true;
		if (fndData.cFileName[1] != '.')
			return fndData.cFileName[1] != 0;
		return fndData.cFileName[2] != 0;
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

UInt32 __fastcall FileToBuffer(const char *filePath, char *buffer);

void __stdcall StoreOriginalData(UInt32 addr, UInt8 size);

void __stdcall SafeWrite8(UInt32 addr, UInt32 data);
void __stdcall SafeWrite16(UInt32 addr, UInt32 data);
void __stdcall SafeWrite32(UInt32 addr, UInt32 data);
void __stdcall SafeWriteBuf(UInt32 addr, void * data, UInt32 len);

#define SAFE_WRITE_BUF(addr, data) SafeWriteBuf(addr, data, sizeof(data) - 1)

// 5 bytes
void __stdcall WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt);
void __stdcall WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt);

// 10 bytes
void __stdcall WritePushRetRelJump(UInt32 baseAddr, UInt32 retAddr, UInt32 jumpTgt);

void __fastcall GetTimeStamp(char *buffer);

UInt64 GetTimeMs64();

const char* __fastcall GetDXDescription(UInt32 keyID);

UInt32 __fastcall ByteSwap(UInt32 dword);

void DumpMemImg(void *data, UInt32 size, UInt8 extra = 0);