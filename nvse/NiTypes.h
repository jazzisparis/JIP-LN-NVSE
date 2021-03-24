#pragma once

#include "nvse/Utilities.h"

// 08
struct NiRTTI
{
	const char		*name;
	NiRTTI			*parent;
};

// 08
struct NiPoint2
{
	float	x;
	float	y;

	NiPoint2() {}
	NiPoint2(float _x, float _y) : x(_x), y(_y) {}
	NiPoint2(const NiPoint2 &rhs) {*(double*)this = *(double*)&rhs;}

	inline NiPoint2& operator=(const NiPoint2 &rhs)
	{
		*(double*)this = *(double*)&rhs;
		return *this;
	}
};

// 24
struct NiMatrix33
{
	float	cr[3][3];

	__forceinline void ExtractAngles(float *rotX, float *rotY, float *rotZ)
	{
		ThisCall(0xA592C0, this, rotX, rotY, rotZ);
	}
	void RotationMatrix(float rotX, float rotY, float rotZ);
	void Rotate(float rotX, float rotY, float rotZ);
	void MultiplyMatrices(NiMatrix33 &matA, NiMatrix33 &matB);
	void Dump(const char *title = NULL);
};

struct NiQuaternion;

// 0C
struct NiVector3
{
	float	x, y, z;

	NiVector3() {}
	NiVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	float& operator[](char axis)
	{
		return ((float*)&x)[axis];
	}

	void operator+=(const NiVector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}
	void operator-=(const NiVector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	}

	void ToQuaternion(NiQuaternion &quaternion);
	void MultiplyMatrixVector(NiMatrix33 &mat, NiVector3 &vec);
	void Normalize();
	bool RayCastCoords(NiVector3 *posVector, NiMatrix33 *rotMatrix, float maxRange, UInt32 axis = 0, UInt16 filter = 6);
};

// 10 - always aligned?
struct NiVector4
{
	float	x, y, z, w;

	NiVector4() {}
	NiVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	float& operator[](char axis)
	{
		return ((float*)&x)[axis];
	}
};

// 10 - always aligned?
struct NiQuaternion
{
	float	w, x, y, z;

	NiQuaternion() {}
	NiQuaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

	void EulerYPR(NiVector3 &ypr);
	void RotationMatrix(NiMatrix33 &rotMatrix);
	void Dump();
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

// 1C
struct NiFrustum
{
	float	l;			// 00
	float	r;			// 04
	float	t;			// 08
	float	b;			// 0C
	float	n;			// 10
	float	f;			// 14
	UInt8	o;			// 18
	UInt8	pad19[3];	// 19
};

// 10
struct NiViewport
{
	float	l;
	float	r;
	float	t;
	float	b;
};

// C
struct NiColor
{
	float	r;
	float	g;
	float	b;

	float& operator[](UInt32 which)
	{
		return ((float*)&r)[which];
	}
};

// 10
struct NiColorAlpha
{
	float	r;
	float	g;
	float	b;
	float	a;

	float& operator[](UInt32 which)
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

	T_Data operator[](UInt32 idx)
	{
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T_Data Get(UInt32 idx) {return data[idx];}

	UInt16 Length() {return firstFreeEntry;}
	void AddAtIndex(UInt32 index, T_Data *item);	// no bounds checking
	void SetCapacity(UInt16 newCapacity);	// grow and copy data if needed

	class Iterator
	{
		friend NiTArray;

		T_Data		*pData;
		UInt16		count;

	public:
		bool End() const {return !count;}
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

template <typename T> void NiTArray<T>::AddAtIndex(UInt32 index, T* item)
{
	ThisCall(0x00869640, this, index, item);
}

template <typename T> void NiTArray<T>::SetCapacity(UInt16 newCapacity)
{
	ThisCall(0x008696E0, this, newCapacity);
}

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

	T_Data operator[](UInt32 idx) {
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T_Data Get(UInt32 idx) { return data[idx]; }

	UInt32 Length(void) { return firstFreeEntry; }

	class Iterator
	{
		friend NiTLargeArray;

		T_Data		*pData;
		UInt32		count;

	public:
		bool End() const {return !count;}
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

// 8
template <typename T>
struct NiTSet
{
	T		* data;		// 00
	UInt16	capacity;	// 04
	UInt16	length;		// 06
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
				if (entry = *bucket) return;
		}

	public:
		Iterator(NiTPointerMap &_table) : table(&_table), bucket(table->m_buckets), entry(NULL) {FindNonEmpty();}

		bool End() const {return !entry;}
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

	virtual void	Destroy(bool doFree);
	virtual UInt32	CalculateBucket(T_Key key);
	virtual bool	Equal(T_Key key1, T_Key key2);
	virtual void	FillEntry(Entry *entry, T_Key key, T_Data data);
	virtual	void	FreeKey(Entry *entry);
	virtual	Entry	*AllocNewEntry();
	virtual	void	FreeEntry(Entry *entry);

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
				if (entry = *bucket) return;
		}

	public:
		Iterator(NiTMapBase &_table) : table(&_table), bucket(table->buckets), entry(NULL) {FindNonEmpty();}

		bool End() const {return !entry;}
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
		push	esi
		push	edi
		mov		ebx, ecx
		mov		esi, [ecx+8]
		mov		edi, [ecx+4]
		ALIGN 16
	bucketIter:
		dec		edi
		js		bucketEnd
		mov		eax, [esi]
		add		esi, 4
		test	eax, eax
		jz		bucketIter
		ALIGN 16
	entryIter:
		push	dword ptr [eax]
		push	eax
		push	eax
		mov		ecx, ebx
		mov		eax, [ecx]
		call	dword ptr [eax+0x10]
		mov		ecx, ebx
		mov		eax, [ecx]
		call	dword ptr [eax+0x18]
		pop		eax
		test	eax, eax
		jnz		entryIter
		mov		[esi-4], eax
		jmp		bucketIter
		ALIGN 16
	bucketEnd:
		mov		dword ptr [ebx+0xC], 0
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

// not sure how much of this is in NiTListBase and how much is in NiTPointerListBase
// 10
template <typename T>
class NiTListBase
{
public:
	struct Node
	{
		Node	* next;
		Node	* prev;
		T		* data;
	};

	virtual void	Destructor(void);
	virtual Node *	AllocateNode(void);
	virtual void	FreeNode(Node * node);

	Node	* start;	// 004
	Node	* end;		// 008
	UInt32	numItems;	// 00C
};

// 10
template <typename T>
class NiTPointerListBase : public NiTListBase <T>
{
public:
};

// 10
template <typename T>
class NiTPointerList : public NiTPointerListBase <T>
{
public:
};

// 4
template <typename T>
class NiPointer
{
public:
	NiPointer(T *init) : data(init)		{	}

	T	* data;

	const T&	operator *() const { return *data; }
	T&			operator *() { return *data; }

	operator const T*() const { return data; }
	operator T*() { return data; }
};

// 14
template <typename T>
class BSTPersistentList
{
public:
	virtual void	Destroy(bool destroy);

	UInt32	unk04;		// 04
	UInt32	unk08;		// 08
	UInt32	unk0C;		// 0C
	UInt32	unk10;		// 10
};
