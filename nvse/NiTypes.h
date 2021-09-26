#pragma once

// 08
struct NiRTTI
{
	const char		*name;
	NiRTTI			*parent;
};

// 08
struct NiPoint2
{
	float	x, y;

	NiPoint2() {}
	NiPoint2(float _x, float _y) : x(_x), y(_y) {}
	NiPoint2(const NiPoint2 &rhs) {*this = rhs;}

	inline void operator=(const NiPoint2 &rhs) {_mm_storeu_si64(this, _mm_loadu_si64(&rhs));}
};

struct NiMatrix33;
struct NiQuaternion;

// 0C
struct NiVector3
{
	float	x, y, z;

	NiVector3() {}
	NiVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	NiVector3(const NiVector3 &rhs) {*this = rhs;}

	inline float& operator[](char axis)
	{
		return ((float*)&x)[axis];
	}

	inline void operator=(const NiVector3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}

	inline void operator+=(const NiVector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	inline void operator+=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
	inline void operator-=(const NiVector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	inline void operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;
	}
	inline void operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
	}

	void ToQuaternion(NiQuaternion &quaternion);
	void MultiplyMatrixVector(NiMatrix33 *mat, NiVector3 *vec);
	void ColumnMultiply(NiMatrix33 *rotMatrix, UInt32 whichColumn);
	void Normalize();
	bool RayCastCoords(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis = 0, UInt16 filter = 6);
};

float __vectorcall Vector3Distance(NiVector3 *vec1, NiVector3 *vec2);

struct NiVector4
{
	float	x, y, z, w;

	NiVector4() {}
	NiVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	NiVector4(const NiVector4 &rhs) {*this = rhs;}

	inline void operator=(const NiVector4 &rhs) {_mm_storeu_ps(&x, _mm_loadu_ps(&rhs.x));}

	inline float& operator[](char axis)
	{
		return ((float*)&x)[axis];
	}

	inline void operator+=(const NiVector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}

	inline void operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
	}
};

struct alignas(16) AlignedVector4
{
	float	x, y, z, w;

	AlignedVector4() {}
	AlignedVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	AlignedVector4(const AlignedVector4 &from) {*this = from;}
	AlignedVector4(const NiVector4 &from) {*this = from;}

	inline void operator=(const AlignedVector4 &from) {_mm_store_ps(&x, _mm_load_ps(&from.x));}
	inline void operator=(const NiVector3 &from) {_mm_store_ps(&x, _mm_loadu_ps(&from.x));}
	inline void operator=(const NiVector4 &from) {_mm_store_ps(&x, _mm_loadu_ps(&from.x));}
	inline void operator=(float *valPtr) {_mm_store_ps(&x, _mm_loadu_ps(valPtr));}

	inline float& operator[](char axis)
	{
		return ((float*)&x)[axis];
	}

	inline void operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;
	}
};

float __vectorcall Vector3Length(AlignedVector4 *inVec);

// 24
struct NiMatrix33
{
	float	cr[3][3];

	inline void operator=(const NiMatrix33 &rhs)
	{
		_mm_storeu_ps(&cr[0][0], _mm_loadu_ps(&rhs.cr[0][0]));
		_mm_storeu_ps(&cr[1][1], _mm_loadu_ps(&rhs.cr[1][1]));
		cr[2][2] = rhs.cr[2][2];
	}

	void __fastcall ExtractAngles(NiVector3 *outAngles);
	void __fastcall ExtractAnglesInv(NiVector3 *outAngles);
	NiMatrix33* __fastcall RotationMatrix(NiVector3 *rot);
	NiMatrix33* __fastcall RotationMatrixInv(NiVector3 *rot);
	NiMatrix33* __fastcall MultiplyMatrices(NiMatrix33 *matB);
	NiMatrix33* __fastcall Rotate(NiVector3 *rot);
	NiMatrix33 *Transpose();
	__forceinline NiMatrix33 *RotationMatrixFromAxisAndAngle(const NiVector3 &normalizedAxes, float angle)
	{
		return ThisCall<NiMatrix33*>(0x4168A0, this, angle, normalizedAxes.x, normalizedAxes.y, normalizedAxes.z);	//	Returns this
	}
	void Dump();
};

// 10
struct NiQuaternion
{
	float	w, x, y, z;

	NiQuaternion() {}
	NiQuaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}
	NiQuaternion(const NiQuaternion &from) {*this = from;}
	NiQuaternion(const NiMatrix33 &rotMat) {*this = rotMat;}

	inline void operator=(const NiQuaternion &rhs) {_mm_storeu_ps(&w, _mm_loadu_ps(&rhs.w));}

	void __fastcall operator=(const NiMatrix33 &mat);

	inline void operator+=(const NiQuaternion &rhs)
	{
		w += rhs.w;
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	inline void operator-=(const NiQuaternion &rhs)
	{
		w -= rhs.w;
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}
	inline void operator*=(const NiQuaternion &rhs)
	{
		float tw = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		float tx = w * rhs.x + x * rhs.w - y * rhs.z + z * rhs.y;
		float ty = w * rhs.y + x * rhs.z + y * rhs.w - z * rhs.x;
		float tz = w * rhs.z - x * rhs.y + y * rhs.x + z * rhs.w;
		w = tw;
		x = tx;
		y = ty;
		z = tz;
	}
	inline void operator*=(float s)
	{
		w *= s;
		x *= s;
		y *= s;
		z *= s;
	}

	void EulerYPR(NiVector3 &ypr);
	void RotationMatrix(NiMatrix33 &rotMatrix);
};

// 34
struct NiTransform
{
	NiMatrix33	rotate;		// 00
	NiVector3	translate;	// 24
	float		scale;		// 30
};

// 10
struct NiSphere
{
	float	x, y, z, radius;
};

// 10
struct NiViewport
{
	float	l;
	float	r;
	float	t;
	float	b;
};

// 1C
struct NiFrustum
{
	NiViewport	viewPort;	// 00
	float		n;			// 10
	float		f;			// 14
	UInt8		o;			// 18
	UInt8		pad19[3];	// 19
};

// C
struct NiColor
{
	float	r, g, b;

	inline float& operator[](UInt32 which)
	{
		return ((float*)&r)[which];
	}
};

// 10
struct NiColorAlpha
{
	float	r, g, b, a;

	inline float& operator[](UInt32 which)
	{
		return ((float*)&r)[which];
	}
};

// 10
struct NiPlane
{
	NiVector3	nrm;
	float		offset;
};

// 10
// NiTArrays are slightly weird: they can be sparse
// this implies that they can only be used with types that can be NULL?
// not sure on the above, but some code only works if this is true
// this can obviously lead to fragmentation, but the accessors don't seem to care
// weird stuff
template <typename T_Data>
struct NiTArray
{
	virtual void	*Destroy(UInt32 doFree);

	T_Data		*data;			// 04
	UInt16		capacity;		// 08 - init'd to size of preallocation
	UInt16		firstFreeEntry;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16		numObjs;		// 0C - init'd to 0
	UInt16		growSize;		// 0E - init'd to size of preallocation

	T_Data operator[](UInt32 idx) {return data[idx];}

	UInt16 Length() const {return firstFreeEntry;}

	__forceinline void AddAtIndex(UInt32 index, T_Data *item)
	{
		ThisCall(0x869640, this, index, item);
	}
	__forceinline void SetCapacity(UInt16 newCapacity)
	{
		ThisCall(0x8696E0, this, newCapacity);
	}

	class Iterator
	{
		friend NiTArray;

		T_Data		*pData;
		UInt32		count;

	public:
		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pData++;
			count--;
		}

		T_Data& operator*() const {return *pData;}
		T_Data& operator->() const {return *pData;}
		T_Data& Get() const {return *pData;}

		Iterator(NiTArray &source) : pData(source.data), count(source.firstFreeEntry) {}
	};

	Iterator Begin() {return Iterator(*this);}
};

// 18
// an NiTArray that can go above 0xFFFF, probably with all the same weirdness
// this implies that they make fragmentable arrays with 0x10000 elements, wtf
template <typename T_Data>
class NiTLargeArray
{
public:
	virtual void	*Destroy(UInt32 doFree);

	T_Data	*data;			// 04
	UInt32	capacity;		// 08 - init'd to size of preallocation
	UInt32	firstFreeEntry;	// 0C - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt32	numObjs;		// 10 - init'd to 0
	UInt32	growSize;		// 14 - init'd to size of preallocation

	T_Data operator[](UInt32 idx) {return data[idx];}

	UInt32 Length() const {return firstFreeEntry;}

	class Iterator
	{
		friend NiTLargeArray;

		T_Data		*pData;
		UInt32		count;

	public:
		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pData++;
			count--;
		}

		T_Data& operator*() const {return *pData;}
		T_Data& operator->() const {return *pData;}
		T_Data& Get() const {return *pData;}

		Iterator(NiTLargeArray &source) : pData(source.data), count(source.firstFreeEntry) {}
	};

	Iterator Begin() {return Iterator(*this);}
};

// 10
// this is a NiTPointerMap <UInt32, T_Data>
// todo: generalize key
template <typename T_Data>
class NiTPointerMap
{
public:
	struct Entry
	{
		Entry		*next;
		UInt32		key;
		T_Data		*data;
	};

	virtual void	Destroy(bool doFree);
	virtual UInt32	CalculateBucket(UInt32 key);
	virtual bool	CompareKey(UInt32 lhs, UInt32 rhs);
	virtual void	FillEntry(Entry *entry, UInt32 key, T_Data data);
	virtual void	FreeKey(Entry *entry);
	virtual Entry	*AllocNewEntry();
	virtual void	FreeEntry(Entry *entry);

	UInt32		m_numBuckets;	// 04
	Entry		**m_buckets;	// 08
	UInt32		m_numItems;		// 0C

	T_Data *Lookup(UInt32 key) const;

	void DumpLoads()
	{
		int loadsArray[0x80];
		MemZero(loadsArray, sizeof(loadsArray));
		Entry **pBucket = m_buckets, *entry;
		UInt32 maxLoad = 0, entryCount;
		for (Entry **pEnd = m_buckets + m_numBuckets; pBucket != pEnd; pBucket++)
		{
			entryCount = 0;
			entry = *pBucket;
			while (entry)
			{
				entryCount++;
				entry = entry->next;
			}
			loadsArray[entryCount]++;
			if (maxLoad < entryCount)
				maxLoad = entryCount;
		}
		PrintDebug("Size = %d\nBuckets = %d\n----------------\n", m_numItems, m_numBuckets);
		for (UInt32 iter = 0; iter <= maxLoad; iter++)
			PrintDebug("%d:\t%05d (%.4f%%)", iter, loadsArray[iter], 100.0 * (double)loadsArray[iter] / m_numItems);
	}

	class Iterator
	{
		friend NiTPointerMap;

		NiTPointerMap	*table;
		Entry			**bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Entry **end = &table->m_buckets[table->m_numBuckets]; bucket != end; bucket++)
				if (entry = *bucket) break;
		}

	public:
		Iterator(NiTPointerMap &_table) : table(&_table), bucket(table->m_buckets), entry(NULL) {FindNonEmpty();}

		explicit operator bool() const {return entry != NULL;}
		void operator++()
		{
			entry = entry->next;
			if (!entry)
			{
				bucket++;
				FindNonEmpty();
			}
		}
		T_Data *Get() const {return entry->data;}
		UInt32 Key() const {return entry->key;}
	};

	Iterator Begin() {return Iterator(*this);}
};

template <typename T_Data>
__declspec(naked) T_Data *NiTPointerMap<T_Data>::Lookup(UInt32 key) const
{
	__asm
	{
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

// 10
// todo: NiTPointerMap should derive from this
// cleaning that up now could cause problems, so it will wait

template <typename T_Key, typename T_Data>
class NiTMapBase
{
public:
	struct Entry
	{
		Entry		*next;
		T_Key		key;
		T_Data		data;
	};

	/*00*/virtual void		Destroy(bool doFree);
	/*04*/virtual UInt32	CalculateBucket(T_Key key);
	/*08*/virtual bool		Equal(T_Key key1, T_Key key2);
	/*0C*/virtual void		FillEntry(Entry *entry, T_Key key, T_Data data);
	/*10*/virtual void		FreeKey(Entry *entry);
	/*14*/virtual Entry		*AllocNewEntry();
	/*18*/virtual void		FreeEntry(Entry *entry);

	UInt32		numBuckets;	// 04
	Entry		**buckets;	// 08
	UInt32		numItems;	// 0C

	T_Data Lookup(T_Key key)
	{
		for (Entry *entry = buckets[CalculateBucket(key)]; entry; entry = entry->next)
			if (Equal(key, entry->key)) return entry->data;
		return NULL;
	}

	void FreeBuckets();

	class Iterator
	{
		friend NiTMapBase;

		NiTMapBase		*table;
		Entry			**bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Entry **end = &table->buckets[table->numBuckets]; bucket != end; bucket++)
				if (entry = *bucket) break;
		}

	public:
		Iterator(NiTMapBase &_table) : table(&_table), bucket(table->buckets), entry(NULL) {FindNonEmpty();}

		explicit operator bool() const {return entry != NULL;}
		void operator++()
		{
			entry = entry->next;
			if (!entry)
			{
				bucket++;
				FindNonEmpty();
			}
		}
		T_Data Get() const {return entry->data;}
		T_Key Key() const {return entry->key;}
	};

	Iterator Begin() {return Iterator(*this);}
};

template <typename T_Key, typename T_Data>
__declspec(naked) void NiTMapBase<T_Key, T_Data>::FreeBuckets()
{
	__asm
	{
		cmp		dword ptr [ecx+0xC], 0
		jz		done
		push	ebx
		mov		ebx, ecx
		mov		ecx, 0x11C5F80
		call	LightCS::Enter
		push	esi
		push	edi
		mov		esi, [ebx+8]
		mov		edi, [ebx+4]
		mov		dword ptr [ebx+0xC], 0
		mov		ebx, 0x11C5F58
		pxor	xmm0, xmm0
		ALIGN 16
	bucketIter:
		dec		edi
		js		bucketEnd
		mov		ecx, [esi]
		add		esi, 4
		test	ecx, ecx
		jz		bucketIter
		ALIGN 16
	entryIter:
		mov		eax, ecx
		mov		ecx, [ecx]
		movq	qword ptr [eax+4], xmm0
		mov		edx, [ebx]
		mov		[eax], edx
		mov		[ebx], eax
		test	ecx, ecx
		jnz		entryIter
		mov		[esi-4], ecx
		jmp		bucketIter
		ALIGN 16
	bucketEnd:
		mov		ecx, 0x11C5F80
		dec		dword ptr [ecx+4]
		jnz		inUse
		mov		dword ptr [ecx], 0
	inUse:
		pop		edi
		pop		esi
		pop		ebx
	done:
		retn
	}
}

// 14
template <typename T_Data>
class NiTStringPointerMap : public NiTMapBase<const char*, T_Data>
{
public:
	UInt8		byte10;
	UInt8		pad11[3];
};