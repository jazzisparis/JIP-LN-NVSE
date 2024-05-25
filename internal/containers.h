#pragma once

#define MAP_DEFAULT_ALLOC			8UL
#define MAP_MIN_BUCKET_COUNT		4UL
#define MAP_MAX_BUCKET_COUNT		0x8000UL
#define MAP_DEFAULT_BUCKET_COUNT	8UL
#define VECTOR_DEFAULT_ALLOC		8UL

char* __fastcall CopyStringKey(const char *key);
__forceinline void FreeStringKey(char *key)
{
	UInt32 *bgnPtr = (UInt32*)(key - 4);
	Pool_CFree(bgnPtr, *bgnPtr);
}

__forceinline UInt32 *AllocBuckets(UInt32 numBuckets)
{
	UInt32 *buckets = Pool_CAlloc<UInt32>(numBuckets);
	__stosd(buckets, 0, numBuckets);
	return buckets;
}
UInt32 __fastcall AlignBucketCount(UInt32 count);
UInt32 __fastcall StrHashCS(const char *inKey);
UInt32 __fastcall StrHashCI(const char *inKey);

template <typename T_Data> class Stack
{
	using Data_Val = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&&>;

	struct alignas(16) Node
	{
		Node		*next;
		T_Data		data;
	};

	Node	*head = nullptr;

public:
	~Stack() {Clear();}

	bool Empty() const {return !head;}

	T_Data& Top()
	{
		return head->data;
	}

	T_Data *Push(Data_Val item)
	{
		Node *newNode = Pool_CAlloc<Node>();
		T_Data *data = &newNode->data;
		*data = std::move(item);
		newNode->next = head;
		head = newNode;
		return data;
	}

	template <typename ...Args>
	T_Data *Push(Args && ...args)
	{
		Node *newNode = Pool_CAlloc<Node>();
		T_Data *data = new (&newNode->data) T_Data(std::forward<Args>(args)...);
		newNode->next = head;
		head = newNode;
		return data;
	}

	T_Data *Pop()
	{
		if (!head) return nullptr;
		T_Data *frontItem = &head->data;
		Node *toRemove = head;
		head = head->next;
		toRemove->~Node();
		Pool_CFree<Node>(toRemove);
		return frontItem;
	}

	void Clear()
	{
		while (Node *pNode = head)
		{
			head = head->next;
			pNode->~Node();
			Pool_CFree<Node>(pNode);
		}
	}
};

template <typename T_Data> class BiLinkedList
{
	using Data_Arg = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, const T_Data&>;
	using Data_Val = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&&>;
	using Data_Res = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&>;
	using Init_List = std::initializer_list<T_Data>;
	Use_LinkedListUtils(BiLinkedList, T_Data)

	struct alignas(16) Node
	{
		Node		*next;
		Node		*prev;
		T_Data		data;

		Node *GetNth(UInt32 index)
		{
			Node *pNode = this;
			while (index--)
				if (!(pNode = pNode->next))
					break;
			return pNode;
		}
	};

	Node	*head;
	Node	*tail;

	Node *GetNthNode(UInt32 index) const
	{
		return head ? head->GetNth(index) : nullptr;
	}

	Node *PrependNew()
	{
		Node *newNode = Pool_CAlloc<Node>();
		newNode->next = head;
		newNode->prev = nullptr;
		if (head) head->prev = newNode;
		else tail = newNode;
		head = newNode;
		return newNode;
	}

	Node *AppendNew()
	{
		Node *newNode = Pool_CAlloc<Node>();
		newNode->next = nullptr;
		newNode->prev = tail;
		if (tail) tail->next = newNode;
		else head = newNode;
		tail = newNode;
		return newNode;
	}

	Node *InsertNew(UInt32 index)
	{
		Node *pNode = GetNthNode(index);
		if (!pNode) return AppendNew();
		Node *prev = pNode->prev, *newNode = Pool_CAlloc<Node>();
		newNode->next = pNode;
		newNode->prev = prev;
		pNode->prev = newNode;
		if (prev) prev->next = newNode;
		else head = newNode;
		return newNode;
	}

	Node *FindNode(Data_Arg item) const
	{
		if (head)
		{
			Node *pNode = head;
			do
			{
				if (pNode->data == item)
					return pNode;
			}
			while (pNode = pNode->next);
		}
		return nullptr;
	}

	template <class Matcher>
	Node *FindNode(const Matcher &matcher) const
	{
		if (head)
		{
			Node *pNode = head;
			do
			{
				if (matcher == pNode->data)
					return pNode;
			}
			while (pNode = pNode->next);
		}
		return nullptr;
	}

	void RemoveNode(Node *toRemove)
	{
		Node *next = toRemove->next, *prev = toRemove->prev;
		if (prev) prev->next = next;
		else head = next;
		if (next) next->prev = prev;
		else tail = prev;
		toRemove->~Node();
		Pool_CFree<Node>(toRemove);
	}

public:
	BiLinkedList() : head(nullptr), tail(nullptr) {}
	BiLinkedList(Init_List &&inList) : head(nullptr), tail(nullptr) {AppendList(std::forward<Init_List>(inList));}
	~BiLinkedList() {Clear();}

	bool Empty() const {return !head;}
	Node *Head() const {return head;}
	Node *Tail() const {return tail;}

	UInt32 Count() const
	{
		if (!head) return 0;
		UInt32 size = 1;
		Node *pNode = head;
		while (pNode = pNode->next)
			size++;
		return size;
	}

	T_Data *Prepend(Data_Val item)
	{
		Node *newNode = PrependNew();
		newNode->data = std::move(item);
		return &newNode->data;
	}

	template <typename ...Args>
	T_Data *Prepend(Args && ...args)
	{
		Node *newNode = PrependNew();
		return new (&newNode->data) T_Data(std::forward<Args>(args)...);
	}

	T_Data *Append(Data_Val item)
	{
		Node *newNode = AppendNew();
		newNode->data = std::move(item);
		return &newNode->data;
	}

	template <typename ...Args>
	T_Data *Append(Args && ...args)
	{
		Node *newNode = AppendNew();
		return new (&newNode->data) T_Data(std::forward<Args>(args)...);
	}

	void AppendList(Init_List &&inList)
	{
		for (auto iter = inList.begin(); iter != inList.end(); ++iter)
			Append(std::forward<T_Data>(*iter));
	}

	T_Data *Insert(UInt32 index, Data_Val item)
	{
		Node *newNode = InsertNew(index);
		newNode->data = std::move(item);
		return &newNode->data;
	}

	template <typename ...Args>
	T_Data *Insert(UInt32 index, Args && ...args)
	{
		Node *newNode = InsertNew(index);
		return new (&newNode->data) T_Data(std::forward<Args>(args)...);
	}

	T_Data *GetNth(UInt32 index) const
	{
		Node *pNode = GetNthNode(index);
		return pNode ? &pNode->data : nullptr;
	}

	T_Data *Front() const
	{
		return head ? &head->data : nullptr;
	}

	T_Data *Back() const
	{
		return tail ? &tail->data : nullptr;
	}

	SInt32 GetIndexOf(Data_Arg item) const
	{
		if (head)
		{
			Node *pNode = head;
			SInt32 index = 0;
			do
			{
				if (pNode->data == item)
					return index;
				index++;
			}
			while (pNode = pNode->next);
		}
		return -1;
	}

	template <class Matcher>
	SInt32 GetIndexOf(const Matcher &matcher) const
	{
		if (head)
		{
			Node *pNode = head;
			SInt32 index = 0;
			do
			{
				if (matcher == pNode->data)
					return index;
				index++;
			}
			while (pNode = pNode->next);
		}
		return -1;
	}

	bool IsInList(Data_Arg item) const
	{
		if (head)
		{
			Node *pNode = head;
			do
			{
				if (pNode->data == item)
					return true;
			}
			while (pNode = pNode->next);
		}
		return false;
	}

	template <class Matcher>
	bool IsInList(const Matcher &matcher) const
	{
		if (head)
		{
			Node *pNode = head;
			do
			{
				if (matcher == pNode->data)
					return true;
			}
			while (pNode = pNode->next);
		}
		return false;
	}

	T_Data *Remove(Data_Arg item)
	{
		if (Node *pNode = FindNode(item))
		{
			RemoveNode(pNode);
			return &pNode->data;
		}
		return nullptr;
	}

	template <class Matcher>
	T_Data *Remove(const Matcher &matcher)
	{
		if (Node *pNode = FindNode(matcher))
		{
			RemoveNode(pNode);
			return &pNode->data;
		}
		return nullptr;
	}

	T_Data *RemoveNth(UInt32 index)
	{
		if (Node *pNode = GetNthNode(index))
		{
			RemoveNode(pNode);
			return &pNode->data;
		}
		return nullptr;
	}

	T_Data *PopFront()
	{
		if (!head) return nullptr;
		T_Data *frontItem = &head->data;
		RemoveNode(head);
		return frontItem;
	}

	T_Data *PopBack()
	{
		if (!tail) return nullptr;
		T_Data *backItem = &tail->data;
		RemoveNode(tail);
		return backItem;
	}

	void Clear()
	{
		while (Node *pNode = head)
		{
			head = head->next;
			pNode->~Node();
			Pool_CFree<Node>(pNode);
		}
		tail = nullptr;
	}

	class Iterator
	{
	protected:
		Node		*pNode;

	public:
		explicit operator bool() const {return pNode != nullptr;}
		void operator++() {pNode = pNode->next;}
		void operator--() {pNode = pNode->prev;}

		Data_Res operator*() const {return pNode->data;}
		Data_Res operator->() const {return pNode->data;}
		Data_Res operator()() const {return pNode->data;}
		T_Data& Ref() {return pNode->data;}

		Iterator() : pNode(nullptr) {}
		Iterator(Node *_node) : pNode(_node) {}

		Iterator& operator=(const Iterator &other)
		{
			pNode = other.pNode;
			return *this;
		}

		void Remove(BiLinkedList &theList, bool frwrd = true)
		{
			Node *toRemove = pNode;
			pNode = frwrd ? pNode->next : pNode->prev;
			theList.RemoveNode(toRemove);
		}
	};

	T_Data *Remove(Iterator &iter)
	{
		if (Node *pNode = iter.pNode)
		{
			RemoveNode(pNode);
			return &pNode->data;
		}
		return nullptr;
	}

	Iterator Begin() {return Iterator(head);}

	Iterator BeginRv() {return Iterator(tail);}

	Iterator Find(Data_Arg item)
	{
		return Iterator(FindNode(item));
	}

	template <class Matcher>
	Iterator Find(const Matcher &matcher)
	{
		return Iterator(FindNode(matcher));
	}
};

template <typename T_Key, typename T_Data> struct MappedPair
{
	T_Key		key;
	T_Data		value;
};

template <typename T_Key> class MapKey
{
	using Key_Arg = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Key_Res = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;

	T_Key		key;

public:
	__forceinline Key_Res Get() const {return key;}
	__forceinline char Compare(Key_Arg inKey) const
	{
		return (inKey < key) ? -1 : (key < inKey);
	}
	__forceinline void operator=(Key_Arg inKey) {key = inKey;}
	__forceinline bool operator==(const MapKey &other) const {return key == other.key;}
};

template <> class MapKey<const char*>
{
	const char	*key;

public:
	__forceinline const char *Get() const {return key;}
	__forceinline char Compare(const char *inKey) const {return StrCompareCI(inKey, key);}
	__forceinline void operator=(const char *inKey) {key = inKey;}
	__forceinline bool operator==(const MapKey &other) const {return !Compare(other.key);}
};

template <> class MapKey<char*>
{
	char		*key;

public:
	__forceinline char *Get() const {return key;}
	__forceinline char Compare(const char *inKey) const {return StrCompareCI(inKey, key);}
	__forceinline void operator=(const char *inKey) {key = CopyStringKey(inKey);}
	__forceinline bool operator==(const MapKey &other) const {return !Compare(other.key);}
	~MapKey() {FreeStringKey(key);}
};

template <> class MapKey<const SInt8*>
{
	const SInt8	*key;

public:
	__forceinline const SInt8 *Get() const {return key;}
	__forceinline char Compare(const SInt8 *inKey) const {return StrCompareCS((const char*)inKey, (const char*)key);}
	__forceinline void operator=(const SInt8 *inKey) {key = inKey;}
	__forceinline bool operator==(const MapKey &other) const {return !Compare(other.key);}
};

template <> class MapKey<SInt8*>
{
	SInt8		*key;

public:
	__forceinline SInt8 *Get() const {return key;}
	__forceinline char Compare(SInt8 *inKey) const {return StrCompareCS((const char*)inKey, (const char*)key);}
	__forceinline void operator=(SInt8 *inKey) {key = (SInt8*)CopyStringKey((const char*)inKey);}
	__forceinline bool operator==(const MapKey &other) const {return !Compare(other.key);}
	~MapKey() {FreeStringKey((char*)key);}
};

template <typename T_Data> class MapValue
{
	T_Data		value;

public:
	__forceinline T_Data *Init() {return &value;}
	__forceinline T_Data& Get() {return value;}
	__forceinline T_Data *Ptr() {return &value;}
	__forceinline bool operator==(const MapValue &other) const {return value == other.value;}
};

template <typename T_Data> class MapValue_p
{
	T_Data		*value;

public:
	__forceinline T_Data *Init()
	{
		value = Pool_Alloc<T_Data>();
		return value;
	}
	__forceinline T_Data& Get() {return *value;}
	__forceinline T_Data *Ptr() {return value;}
	__forceinline bool operator==(const MapValue_p &other) const {return *value == *other.value;}
	~MapValue_p()
	{
		value->~T_Data();
		Pool_Free<T_Data>(value);
	}
};

template <typename T_Key, typename T_Data, const UInt32 _default_alloc = MAP_DEFAULT_ALLOC> class Map
{
	using M_Key = MapKey<T_Key>;
	using M_Value = std::conditional_t<(sizeof(T_Data) <= 8) || (sizeof(T_Data) <= alignof(T_Key)), MapValue<T_Data>, MapValue_p<T_Data>>;
	using Key_Arg = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Key_Res = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Data_Arg = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, const T_Data&>;
	using Data_Res = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&>;
	using M_Pair = MappedPair<T_Key, T_Data>;
	using Init_List = std::initializer_list<M_Pair>;

	struct Entry
	{
		M_Key		key;
		M_Value		value;

		bool operator==(const Entry &other) const {return (key == other.key) && (value == other.value);}
	};

	Entry		*entries;		// 00
	UInt32		numEntries;		// 04
	UInt32		numAlloc;		// 08

	UInt32 GetIndex(Key_Arg key) const
	{
		UInt32 lBound = 0, uBound = numEntries;
		while (lBound != uBound)
		{
			UInt32 index = (lBound + uBound) >> 1;
			char cmpr = entries[index].key.Compare(key);
			if (cmpr < 0)
				uBound = index;
			else if (cmpr > 0)
				lBound = index + 1;
			else
				return index;
		}
		return lBound | 0x80000000;
	}

	Entry *End() const {return entries + numEntries;}

public:
	Map(UInt32 _alloc = _default_alloc) : entries(nullptr), numEntries(0), numAlloc(_alloc) {}
	Map(Init_List &&inList) : entries(nullptr), numEntries(0), numAlloc(inList.size()) {InsertList(std::forward<Init_List>(inList));}
	~Map()
	{
		if (entries)
		{
			Clear();
			Pool_CFree<Entry>(entries, numAlloc);
		}
	}

	void Destroy()
	{
		this->~Map();
		entries = nullptr;
	}

	UInt32 Size() const {return numEntries;}
	bool Empty() const {return !numEntries;}

	void operator=(Map &&rhs)
	{
		this->~Map();
		entries = rhs.entries;
		numEntries = rhs.numEntries;
		numAlloc = rhs.numAlloc;
		rhs.entries = nullptr;
		rhs.numEntries = 0;
		rhs.numAlloc = MAP_DEFAULT_ALLOC;
	}

	void operator=(const Map &rhs)
	{
		Destroy();
		if (numEntries = rhs.numEntries)
		{
			numAlloc = AlignNumAlloc<Entry>(numEntries);
			entries = Pool_CAlloc<Entry>(numAlloc);
			Entry *pEntry = entries, *pSource = rhs.entries, *pEnd = rhs.End();
			do
			{
				pEntry->key = pSource->key.Get();
				*(pEntry->value.Init()) = pSource->value.Get();
				pEntry++;
			}
			while (++pSource != pEnd);
		}
	}

	bool InsertKey(Key_Arg key, T_Data **outData)
	{
		UInt32 index = GetIndex(key);
		if (SInt32(index) >= 0)
		{
			*outData = entries[index].value.Ptr();
			return false;
		}
		index &= 0x7FFFFFFF;
		if (!entries)
		{
			numAlloc = AlignNumAlloc<Entry>(numAlloc);
			entries = Pool_CAlloc<Entry>(numAlloc);
		}
		else if (numAlloc <= numEntries)
		{
			UInt32 newAlloc = numAlloc << 1;
			entries = Pool_CRealloc<Entry>(entries, numAlloc, newAlloc);
			numAlloc = newAlloc;
		}
		Entry *pEntry = entries + index;
		if (UInt32 movCnt = numEntries - index)
			MemCopy(pEntry + 1, pEntry, sizeof(Entry) * movCnt);
		numEntries++;
		pEntry->key = key;
		*outData = pEntry->value.Init();
		return true;
	}

	bool Insert(Key_Arg key, T_Data **outData)
	{
		if (InsertKey(key, outData))
		{
			new (*outData) T_Data();
			return true;
		}
		return false;
	}

	T_Data& operator[](Key_Arg key)
	{
		T_Data *outData;
		if (InsertKey(key, &outData))
			new (outData) T_Data();
		return *outData;
	}

	template <typename ...Args>
	T_Data* Emplace(Key_Arg key, Args&& ...args)
	{
		T_Data *outData;
		if (InsertKey(key, &outData))
			new (outData) T_Data(std::forward<Args>(args)...);
		return outData;
	}

	void InsertList(Init_List &&inList)
	{
		T_Data *outData;
		for (auto iter = inList.begin(); iter != inList.end(); ++iter)
		{
			InsertKey(iter->key, &outData);
			*outData = std::move(iter->value);
		}
	}

	bool HasKey(Key_Arg key) const
	{
		return SInt32(GetIndex(key)) >= 0;
	}

	T_Data Get(Key_Arg key) const
	{
		static_assert(std::is_scalar_v<T_Data>);
		if (UInt32 index = GetIndex(key); SInt32(index) >= 0)
			return entries[index].value.Get();
		return NULL;
	}

	T_Data* GetPtr(Key_Arg key) const
	{
		if (UInt32 index = GetIndex(key); SInt32(index) >= 0)
			return entries[index].value.Ptr();
		return nullptr;
	}

	bool Erase(Key_Arg key)
	{
		if (UInt32 index = GetIndex(key); SInt32(index) >= 0)
		{
			Entry *pEntry = entries + index;
			pEntry->~Entry();
			numEntries--;
			if (UInt32 cpyCnt = numEntries - index)
				MemCopy(pEntry, pEntry + 1, sizeof(Entry) * cpyCnt);
			return true;
		}
		return false;
	}

	T_Data GetErase(Key_Arg key)
	{
		static_assert(std::is_scalar_v<T_Data>);
		if (UInt32 index = GetIndex(key); SInt32(index) >= 0)
		{
			Entry *pEntry = entries + index;
			T_Data outVal = pEntry->value.Get();
			pEntry->~Entry();
			numEntries--;
			if (UInt32 cpyCnt = numEntries - index)
				MemCopy(pEntry, pEntry + 1, sizeof(Entry) * cpyCnt);
			return outVal;
		}
		return NULL;
	}

	void Clear()
	{
		for (Entry *pEntry = entries, *pEnd = End(); pEntry != pEnd; pEntry++)
			pEntry->~Entry();
		numEntries &= 0;
	}

	bool operator==(const Map &other) const
	{
		if (numEntries != other.numEntries)
			return false;
		for (UInt32 i = 0; i < numEntries; i++)
			if (!(entries[i] == other.entries[i]))
				return false;
		return true;
	}

	class Iterator
	{
	protected:
		Entry		*pEntry;
		UInt32		count;

		Iterator() {}

	public:
		Key_Res Key() const {return pEntry->key.Get();}
		Data_Res operator()() const {return pEntry->value.Get();}
		Data_Res operator*() const {return pEntry->value.Get();}
		Data_Res operator->() const {return pEntry->value.Get();}
		T_Data& Ref() {return pEntry->value.Get();}

		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pEntry++;
			count--;
		}

		void Find(Map &source, Key_Arg key)
		{
			if (UInt32 index = source.GetIndex(key); SInt32(index) >= 0)
			{
				pEntry = source.entries + index;
				count = source.numEntries - index;
			}
			else count = 0;
		}

		Iterator(Map &source) : pEntry(source.entries), count(source.numEntries) {}
		Iterator(Map &source, Key_Arg key) {Find(source, key);}
	};

	class OpIterator : public Iterator
	{
		using Iterator::pEntry, Iterator::count, Iterator::Find;
	public:
		void operator--()
		{
			pEntry--;
			count--;
		}

		void Remove(Map &source, bool frwrd = true)
		{
			pEntry->~Entry();
			UInt32 size = frwrd ? (count - 1) : (source.numEntries - count);
			source.numEntries--;
			if (size)
				MemCopy(pEntry, pEntry + 1, size * sizeof(Entry));
			if (frwrd) pEntry--;
		}

		OpIterator(Map &source)
		{
			pEntry = source.entries;
			count = source.numEntries;
		}
		OpIterator(Map &source, Key_Arg key) {Find(source, key);}
		OpIterator(Map &source, Key_Arg key, bool frwrd)
		{
			if (source.numEntries)
			{
				UInt32 index = source.GetIndex(key);
				bool match = SInt32(index) >= 0;
				index &= 0x7FFFFFFF;
				if (frwrd)
				{
					pEntry = source.entries + index;
					count = source.numEntries - index;
				}
				else
				{
					pEntry = source.entries + (index - !match);
					count = index + match;
				}
			}
			else count = 0;
		}
	};

	class CpIterator : public Iterator
	{
		using Iterator::pEntry, Iterator::count;

		AuxBuffer<Entry>	buffer;

	public:
		CpIterator(Map &source)
		{
			count = source.numEntries;
			if (count > 1)
				pEntry = buffer.Copy(count, source.entries);
			else
				pEntry = source.entries;
		}
	};

	Iterator Begin() {return Iterator(*this);}
	Iterator Find(Key_Arg key) {return Iterator(*this, key);}

	OpIterator BeginOp() {return OpIterator(*this);}
	OpIterator FindOp(Key_Arg key) {return OpIterator(*this, key);}
	OpIterator FindOpDir(Key_Arg key, bool frwrd) {return OpIterator(*this, key, frwrd);}

	CpIterator BeginCp() {return CpIterator(*this);}
};

template <typename T_Key, const UInt32 _default_alloc = MAP_DEFAULT_ALLOC> class Set
{
	using M_Key = MapKey<T_Key>;
	using Key_Arg = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Key_Res = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Init_List = std::initializer_list<T_Key>;

	M_Key		*keys;		// 00
	UInt32		numKeys;	// 04
	UInt32		numAlloc;	// 08

	UInt32 GetIndex(Key_Arg key) const
	{
		UInt32 lBound = 0, uBound = numKeys;
		while (lBound != uBound)
		{
			UInt32 index = (lBound + uBound) >> 1;
			char cmpr = keys[index].Compare(key);
			if (cmpr < 0)
				uBound = index;
			else if (cmpr > 0)
				lBound = index + 1;
			else
				return index;
		}
		return lBound | 0x80000000;
	}

	M_Key *End() const {return keys + numKeys;}

public:
	Set(UInt32 _alloc = _default_alloc) : keys(nullptr), numKeys(0), numAlloc(_alloc) {}
	Set(Init_List &&inList) : keys(nullptr), numKeys(0), numAlloc(inList.size()) {InsertList(std::forward<Init_List>(inList));}
	~Set()
	{
		if (keys)
		{
			Clear();
			Pool_CFree<M_Key>(keys, numAlloc);
		}
	}

	void Destroy()
	{
		this->~Set();
		keys = nullptr;
	}

	UInt32 Size() const {return numKeys;}
	bool Empty() const {return !numKeys;}
	T_Key *Keys() {return reinterpret_cast<T_Key*>(keys);}

	void operator=(Set &&rhs)
	{
		this->~Set();
		keys = rhs.keys;
		numKeys = rhs.numKeys;
		numAlloc = rhs.numAlloc;
		rhs.keys = nullptr;
		rhs.numKeys = 0;
		rhs.numAlloc = MAP_DEFAULT_ALLOC;
	}

	void operator=(const Set &rhs)
	{
		Clear();
		if (numKeys = rhs.numKeys)
		{
			if (numAlloc < numKeys)
			{
				if (keys)
				{
					Pool_CFree<M_Key>(keys, numAlloc);
					keys = nullptr;
				}
				numAlloc = AlignNumAlloc<M_Key>(numKeys);
			}
			if (!keys)
				keys = Pool_CAlloc<M_Key>(numAlloc);
			MemCopy(keys, rhs.keys, sizeof(M_Key) * numKeys);
		}
	}

	const T_Key& operator[](UInt32 index) const {return keys[index];}

	bool Insert(Key_Arg key)
	{
		if (UInt32 index = GetIndex(key); SInt32(index) < 0)
		{
			index &= 0x7FFFFFFF;
			if (!keys)
			{
				numAlloc = AlignNumAlloc<M_Key>(numAlloc);
				keys = Pool_CAlloc<M_Key>(numAlloc);
			}
			else if (numAlloc <= numKeys)
			{
				UInt32 newAlloc = numAlloc << 1;
				keys = Pool_CRealloc<M_Key>(keys, numAlloc, newAlloc);
				numAlloc = newAlloc;
			}
			M_Key *pKey = keys + index;
			if (UInt32 movCnt = numKeys - index)
				MemCopy(pKey + 1, pKey, sizeof(M_Key) * movCnt);
			numKeys++;
			*pKey = key;
			return true;
		}
		return false;
	}

	void InsertList(Init_List &&inList)
	{
		for (auto iter = inList.begin(); iter != inList.end(); ++iter)
			Insert(*iter);
	}

	bool HasKey(Key_Arg key) const
	{
		return SInt32(GetIndex(key)) >= 0;
	}

	bool Erase(Key_Arg key)
	{
		if (UInt32 index = GetIndex(key); SInt32(index) >= 0)
		{
			M_Key *pKey = keys + index;
			pKey->~M_Key();
			numKeys--;
			if (UInt32 cpyCnt = numKeys - index)
				MemCopy(pKey, pKey + 1, sizeof(M_Key) * cpyCnt);
			return true;
		}
		return false;
	}

	void Clear()
	{
		for (M_Key *pKey = keys, *pEnd = End(); pKey != pEnd; pKey++)
			pKey->~M_Key();
		numKeys &= 0;
	}

	inline bool operator==(const Set &rhs) const
	{
		return (numKeys == rhs.numKeys) && (!numKeys || MemCmp(keys, rhs.keys, sizeof(M_Key) * numKeys));
	}
	inline bool operator!=(const Set &rhs) const {return !(*this == rhs);}

	class Iterator
	{
	protected:
		M_Key		*pKey;
		UInt32		count;

		Iterator() {}

	public:
		Key_Res operator()() const {return pKey->Get();}
		Key_Res operator*() const {return pKey->Get();}
		Key_Res operator->() const {return pKey->Get();}

		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pKey++;
			count--;
		}

		Iterator(Set &source) : pKey(source.keys), count(source.numKeys) {}
		Iterator(Set &source, Key_Arg key)
		{
			if (UInt32 index = source.GetIndex(key); SInt32(index) >= 0)
			{
				pKey = source.keys + index;
				count = source.numKeys - index;
			}
			else count = 0;
		}
	};

	class CpIterator : public Iterator
	{
		using Iterator::pKey, Iterator::count;

		AuxBuffer<M_Key>	buffer;

	public:
		CpIterator(Set &source)
		{
			count = source.numKeys;
			if (count > 1)
				pKey = buffer.Copy(count, source.keys);
			else
				pKey = source.keys;
		}
	};

	void Remove(Iterator &iter)
	{
		iter.pKey->~M_Key();
		numKeys--;
		if (iter.count > 1)
		{
			MemCopy(iter.pKey, iter.pKey + 1, (iter.count - 1) * sizeof(M_Key));
			iter.pKey--;
		}
	}

	Iterator Begin() {return Iterator(*this);}
	Iterator Find(Key_Arg key) {return Iterator(*this, key);}

	CpIterator BeginCp() {return CpIterator(*this);}
};

template <typename T_Key> __forceinline UInt32 HashKey(T_Key inKey)
{
	union
	{
		T_Key		raw;
		UInt8		u8;
		UInt16		u16;
		UInt32		u32;
		const char	*str;
	}
	key = {inKey};
	if (std::is_same_v<T_Key, UINT>)
		return key.u32;
	if (std::is_same_v<T_Key, char*> || std::is_same_v<T_Key, const char*>)
		return StrHashCI(key.str);
	if (std::is_same_v<T_Key, SInt8*> || std::is_same_v<T_Key, const SInt8*>)
		return StrHashCS(key.str);
	UInt32 uKey;
	if (sizeof(T_Key) == 1)
		uKey = key.u8;
	else if (sizeof(T_Key) == 2)
		uKey = key.u16;
	else
	{
		uKey = key.u32;
		if (sizeof(T_Key) > 4)
		{
			uKey += ULNG(inKey + 1) ^ 0x7ED55D16;
			if (sizeof(T_Key) > 8)
			{
				uKey += ULNG(inKey + 2) ^ 0x165667B1;
				if (sizeof(T_Key) > 0xC)
					uKey += ULNG(inKey + 3) ^ 0xFD7046C5;
			}
		}
	}
	return (uKey * 0xD) ^ (uKey >> 0xF);
}

template <typename T_Key> class HashedKey
{
	using Key_Arg = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Key_Res = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;

	T_Key		key;

public:
	__forceinline bool Equal(Key_Arg inKey, UInt32) const {return key == inKey;}
	__forceinline Key_Res Get() const {return key;}
	__forceinline void Set(Key_Arg inKey, UInt32) {key = inKey;}
	__forceinline UInt32 GetHash() const {return HashKey<T_Key>(key);}
};

template <> class HashedKey<const char*>
{
	UInt32		hashVal;

public:
	__forceinline bool Equal(const char*, UInt32 inHash) const {return hashVal == inHash;}
	__forceinline const char *Get() const {return "";}
	__forceinline void Set(const char*, UInt32 inHash) {hashVal = inHash;}
	__forceinline UInt32 GetHash() const {return hashVal;}
};

template <> class HashedKey<char*>
{
	UInt32		hashVal;
	char		*key;

public:
	__forceinline bool Equal(char*, UInt32 inHash) const {return hashVal == inHash;}
	__forceinline char *Get() const {return key;}
	__forceinline void Set(const char *inKey, UInt32 inHash)
	{
		hashVal = inHash;
		key = CopyStringKey(inKey);
	}
	__forceinline UInt32 GetHash() const {return hashVal;}
	~HashedKey() {FreeStringKey(key);}
};

template <> class HashedKey<const SInt8*>
{
	UInt32		hashVal;

public:
	__forceinline bool Equal(const SInt8*, UInt32 inHash) const {return hashVal == inHash;}
	__forceinline const SInt8 *Get() const {return (const SInt8*)"";}
	__forceinline void Set(const SInt8*, UInt32 inHash) {hashVal = inHash;}
	__forceinline UInt32 GetHash() const {return hashVal;}
};

template <> class HashedKey<SInt8*>
{
	UInt32		hashVal;
	SInt8		*key;

public:
	__forceinline bool Equal(SInt8*, UInt32 inHash) const {return hashVal == inHash;}
	__forceinline SInt8 *Get() const {return key;}
	__forceinline void Set(SInt8 *inKey, UInt32 inHash)
	{
		hashVal = inHash;
		key = (SInt8*)CopyStringKey((const char*)inKey);
	}
	__forceinline UInt32 GetHash() const {return hashVal;}
	~HashedKey() {FreeStringKey((char*)key);}
};

#define NUM_BUCKETS (_allow_resize ? numBuckets : _default_bucket_count)

template <typename T_Key, typename T_Data, const UInt32 _default_bucket_count = MAP_DEFAULT_BUCKET_COUNT, const bool _allow_resize = true> class UnorderedMap
{
	static_assert(std::has_single_bit(_default_bucket_count));

	using H_Key = HashedKey<T_Key>;
	using Key_Arg = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Key_Res = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Data_Arg = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, const T_Data&>;
	using Data_Res = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&>;
	using M_Pair = MappedPair<T_Key, T_Data>;
	using Init_List = std::initializer_list<M_Pair>;
	Use_HashMapUtils(UnorderedMap)

	struct alignas(16) Entry
	{
		Entry		*next;
		H_Key		key;
		T_Data		value;
	};

	struct Bucket
	{
		Entry	*entries;

		void Insert(Entry *entry)
		{
			entry->next = entries;
			entries = entry;
		}

		Entry *Remove(Entry *entry)
		{
			Entry *prev = nullptr;
			for (Entry *curr = entries; curr != entry; curr = curr->next)
				prev = curr;
			if (prev) prev->next = entry->next;
			else entries = entry->next;
			entry->~Entry();
			Pool_CFree<Entry>(entry);
			return prev;
		}

		~Bucket()
		{
			while (Entry *pEntry = entries)
			{
				entries = entries->next;
				pEntry->~Entry();
				Pool_CFree<Entry>(pEntry);
			}
		}

		UInt32 Size() const
		{
			UInt32 size = 0;
			for (Entry *pEntry = entries; pEntry; pEntry = pEntry->next, size++);
			return size;
		}
	};

	Bucket		*buckets;		// 00
	UInt32		numBuckets;		// 04
	UInt32		numEntries;		// 08

	Bucket *GetBuckets() const {return buckets;}
	Bucket *End() const {return buckets + NUM_BUCKETS;}

	__declspec(noinline) void __fastcall ResizeTable(UInt32 newCount)
	{
		Bucket *pBucket = buckets, *pEnd = End(), *newBuckets = (Bucket*)AllocBuckets(newCount);
		newCount--;
		do
		{
			Entry *pEntry = pBucket->entries;
			while (Entry *pTemp = pEntry)
			{
				pEntry = pEntry->next;
				newBuckets[pTemp->key.GetHash() & newCount].Insert(pTemp);
			}
		}
		while (++pBucket != pEnd);
		Pool_CFree<Bucket>(buckets, numBuckets);
		buckets = newBuckets;
		numBuckets = newCount + 1;
	}

	Entry *FindEntry(Key_Arg key) const
	{
		if (numEntries)
		{
			UInt32 hashVal = HashKey<T_Key>(key);
			for (Entry *pEntry = buckets[hashVal & (NUM_BUCKETS - 1)].entries; pEntry; pEntry = pEntry->next)
				if (pEntry->key.Equal(key, hashVal)) return pEntry;
		}
		return nullptr;
	}

public:
	UnorderedMap(UInt32 _numBuckets = _default_bucket_count) : buckets(nullptr), numBuckets(_allow_resize ? _numBuckets : _default_bucket_count), numEntries(0) {}
	UnorderedMap(Init_List &&inList) : buckets(nullptr), numBuckets(_allow_resize ? AlignBucketCount(inList.size()) : _default_bucket_count), numEntries(0)
	{
		InsertList(std::forward<Init_List>(inList));
	}
	~UnorderedMap()
	{
		if (buckets)
		{
			Clear();
			Pool_CFree<Bucket>(buckets, NUM_BUCKETS);
		}
	}

	void Destroy()
	{
		this->~UnorderedMap();
		buckets = nullptr;
	}

	UInt32 Size() const {return numEntries;}
	bool Empty() const {return !numEntries;}
	UInt32 BucketCount() const {return numBuckets;}

	void operator=(const UnorderedMap &rhs) = delete;

	void operator=(UnorderedMap &&rhs)
	{
		this->~UnorderedMap();
		buckets = rhs.buckets;
		numBuckets = rhs.numBuckets;
		numEntries = rhs.numEntries;
		rhs.buckets = nullptr;
		rhs.numBuckets = MAP_DEFAULT_BUCKET_COUNT;
		rhs.numEntries = 0;
	}

	__declspec(noinline) void SetBucketCount(UInt32 newCount)
	{
		if (!_allow_resize) return;
		newCount = AlignBucketCount(newCount);
		if (numBuckets == newCount)
			return;
		if (!numEntries)
		{
			if (buckets)
			{
				Pool_CFree<Bucket>(buckets, numBuckets);
				buckets = nullptr;
			}
			numBuckets = newCount;
		}
		else if (numEntries <= newCount)
			ResizeTable(newCount);
	}

	float LoadFactor() const {return (float)numEntries / (float)numBuckets;}

	bool InsertKey(Key_Arg key, T_Data **outData)
	{
		if (!buckets)
			buckets = (Bucket*)AllocBuckets(NUM_BUCKETS);
		else if (_allow_resize)
		{
			if ((numEntries > numBuckets) && (numBuckets < MAP_MAX_BUCKET_COUNT))
				ResizeTable(numBuckets << 1);
		}
		UInt32 hashVal = HashKey<T_Key>(key);
		Bucket *pBucket = &buckets[hashVal & (NUM_BUCKETS - 1)];
		for (Entry *pEntry = pBucket->entries; pEntry; pEntry = pEntry->next)
		{
			if (!pEntry->key.Equal(key, hashVal))
				continue;
			*outData = &pEntry->value;
			return false;
		}
		numEntries++;
		Entry *newEntry = Pool_CAlloc<Entry>();
		newEntry->key.Set(key, hashVal);
		pBucket->Insert(newEntry);
		*outData = &newEntry->value;
		return true;
	}

	bool Insert(Key_Arg key, T_Data **outData)
	{
		if (InsertKey(key, outData))
		{
			new (*outData) T_Data();
			return true;
		}
		return false;
	}

	T_Data& operator[](Key_Arg key)
	{
		T_Data *outData;
		if (InsertKey(key, &outData))
			new (outData) T_Data();
		return *outData;
	}

	template <typename ...Args>
	T_Data* Emplace(Key_Arg key, Args&& ...args)
	{
		T_Data *outData;
		if (InsertKey(key, &outData))
			new (outData) T_Data(std::forward<Args>(args)...);
		return outData;
	}

	void InsertList(Init_List &&inList)
	{
		T_Data *outData;
		for (auto iter = inList.begin(); iter != inList.end(); ++iter)
		{
			InsertKey(iter->key, &outData);
			*outData = std::move(iter->value);
		}
	}

	bool HasKey(Key_Arg key) const {return FindEntry(key) != nullptr;}

	T_Data Get(Key_Arg key) const
	{
		static_assert(std::is_scalar_v<T_Data>);
		Entry *pEntry = FindEntry(key);
		return pEntry ? pEntry->value : NULL;
	}

	T_Data* GetPtr(Key_Arg key) const
	{
		Entry *pEntry = FindEntry(key);
		return pEntry ? &pEntry->value : nullptr;
	}

	bool Erase(Key_Arg key)
	{
		if (numEntries)
		{
			UInt32 hashVal = HashKey<T_Key>(key);
			Bucket *pBucket = &buckets[hashVal & (NUM_BUCKETS - 1)];
			for (Entry *pEntry = pBucket->entries; pEntry; pEntry = pEntry->next)
			{
				if (!pEntry->key.Equal(key, hashVal))
					continue;
				numEntries--;
				pBucket->Remove(pEntry);
				return true;
			}
		}
		return false;
	}

	T_Data GetErase(Key_Arg key)
	{
		static_assert(std::is_scalar_v<T_Data>);
		if (numEntries)
		{
			UInt32 hashVal = HashKey<T_Key>(key);
			Bucket *pBucket = &buckets[hashVal & (NUM_BUCKETS - 1)];
			for (Entry *pEntry = pBucket->entries; pEntry; pEntry = pEntry->next)
			{
				if (!pEntry->key.Equal(key, hashVal))
					continue;
				T_Data outVal = pEntry->value;
				numEntries--;
				pBucket->Remove(pEntry);
				return outVal;
			}
		}
		return NULL;
	}

	void Clear()
	{
		if (numEntries)
		{
			Bucket *pBucket = buckets, *pEnd = End();
			do
			{
				pBucket->~Bucket();
			}
			while (++pBucket != pEnd);
		}
		numEntries &= 0;
	}

	class Iterator
	{
	protected:
		UnorderedMap	*table;
		Bucket			*bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Bucket *pEnd = table->End(); bucket != pEnd; bucket++)
				if (entry = bucket->entries) return;
		}

	public:
		void Init(UnorderedMap &_table)
		{
			table = &_table;
			entry = nullptr;
			if (table->numEntries)
			{
				bucket = table->buckets;
				FindNonEmpty();
			}
		}

		void Find(Key_Arg key)
		{
			if (table->numEntries)
			{
				UInt32 hashVal = HashKey<T_Key>(key);
				bucket = &table->buckets[hashVal & (table->numBuckets - 1)];
				for (entry = bucket->entries; entry; entry = entry->next)
					if (entry->key.Equal(key, hashVal)) break;
			}
			else entry = nullptr;
		}

		UnorderedMap* Table() const {return table;}
		Key_Res Key() const {return entry->key.Get();}
		Data_Res operator()() const {return entry->value;}
		T_Data& Ref() {return entry->value;}
		Data_Res operator*() const {return entry->value;}
		Data_Res operator->() const {return entry->value;}

		explicit operator bool() const {return entry != nullptr;}
		void operator++()
		{
			if (entry)
				entry = entry->next;
			else entry = bucket->entries;
			if (!entry)
			{
				bucket++;
				FindNonEmpty();
			}
		}

		bool IsValid()
		{
			if (entry)
			{
				for (Entry *temp = bucket->entries; temp; temp = temp->next)
					if (temp == entry) return true;
				entry = nullptr;
			}
			return false;
		}

		void Remove()
		{
			table->numEntries--;
			entry = bucket->Remove(entry);
		}

		Iterator(UnorderedMap *_table = nullptr) : table(_table), entry(nullptr) {}
		Iterator(UnorderedMap &_table) {Init(_table);}
		Iterator(UnorderedMap &_table, Key_Arg key) : table(&_table) {Find(key);}
	};

	Iterator Begin() {return Iterator(*this);}
	Iterator Find(Key_Arg key) {return Iterator(*this, key);}
};

template <typename T_Key, const UInt32 _default_bucket_count = MAP_DEFAULT_BUCKET_COUNT, const bool _allow_resize = true> class UnorderedSet
{
	static_assert(std::has_single_bit(_default_bucket_count));

	using H_Key = HashedKey<T_Key>;
	using Key_Arg = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Key_Res = std::conditional_t<std::is_scalar_v<T_Key>, T_Key, const T_Key&>;
	using Init_List = std::initializer_list<T_Key>;
	Use_HashMapUtils(UnorderedSet)

	struct alignas(16) Entry
	{
		Entry		*next;
		H_Key		key;
	};

	struct Bucket
	{
		Entry	*entries;

		void Insert(Entry *entry)
		{
			entry->next = entries;
			entries = entry;
		}

		Entry *Remove(Entry *entry)
		{
			Entry *prev = nullptr;
			for (Entry *curr = entries; curr != entry; curr = curr->next)
				prev = curr;
			if (prev) prev->next = entry->next;
			else entries = entry->next;
			entry->~Entry();
			Pool_CFree<Entry>(entry);
			return prev;
		}

		~Bucket()
		{
			while (Entry *pEntry = entries)
			{
				entries = entries->next;
				pEntry->~Entry();
				Pool_CFree<Entry>(pEntry);
			}
		}

		UInt32 Size() const
		{
			UInt32 size = 0;
			for (Entry *pEntry = entries; pEntry; pEntry = pEntry->next, size++);
			return size;
		}
	};

	Bucket		*buckets;		// 00
	UInt32		numBuckets;		// 04
	UInt32		numEntries;		// 08

	Bucket *GetBuckets() const {return buckets;}
	Bucket *End() const {return buckets + NUM_BUCKETS;}

	__declspec(noinline) void __fastcall ResizeTable(UInt32 newCount)
	{
		Bucket *pBucket = buckets, *pEnd = End(), *newBuckets = (Bucket*)AllocBuckets(newCount);
		newCount--;
		do
		{
			Entry *pEntry = pBucket->entries;
			while (Entry *pTemp = pEntry)
			{
				pEntry = pEntry->next;
				newBuckets[pTemp->key.GetHash() & newCount].Insert(pTemp);
			}
		}
		while (++pBucket != pEnd);
		Pool_CFree<Bucket>(buckets, numBuckets);
		buckets = newBuckets;
		numBuckets = newCount + 1;
	}

public:
	UnorderedSet(UInt32 _numBuckets = _default_bucket_count) : buckets(nullptr), numBuckets(_allow_resize ? _numBuckets : _default_bucket_count), numEntries(0) {}
	UnorderedSet(Init_List &&inList) : buckets(nullptr), numBuckets(_allow_resize ? AlignBucketCount(inList.size()) : _default_bucket_count), numEntries(0)
	{
		InsertList(std::forward<Init_List>(inList));
	}
	~UnorderedSet()
	{
		if (buckets)
		{
			Clear();
			Pool_CFree<Bucket>(buckets, NUM_BUCKETS);
		}
	}

	void Destroy()
	{
		this->~UnorderedSet();
		buckets = nullptr;
	}

	UInt32 Size() const {return numEntries;}
	bool Empty() const {return !numEntries;}
	UInt32 BucketCount() const {return numBuckets;}

	void operator=(const UnorderedSet &rhs) = delete;

	void operator=(UnorderedSet &&rhs)
	{
		this->~UnorderedSet();
		buckets = rhs.buckets;
		numBuckets = rhs.numBuckets;
		numEntries = rhs.numEntries;
		rhs.buckets = nullptr;
		rhs.numBuckets = MAP_DEFAULT_BUCKET_COUNT;
		rhs.numEntries = 0;
	}

	__declspec(noinline) void SetBucketCount(UInt32 newCount)
	{
		if (!_allow_resize) return;
		newCount = AlignBucketCount(newCount);
		if (numBuckets == newCount)
			return;
		if (!numEntries)
		{
			if (buckets)
			{
				Pool_CFree<Bucket>(buckets, numBuckets);
				buckets = nullptr;
			}
			numBuckets = newCount;
		}
		else if (numEntries <= newCount)
			ResizeTable(newCount);
	}

	float LoadFactor() const {return (float)numEntries / (float)numBuckets;}

	bool Insert(Key_Arg key)
	{
		if (!buckets)
			buckets = (Bucket*)AllocBuckets(NUM_BUCKETS);
		else if (_allow_resize)
		{
			if ((numEntries > numBuckets) && (numBuckets < MAP_MAX_BUCKET_COUNT))
				ResizeTable(numBuckets << 1);
		}
		UInt32 hashVal = HashKey<T_Key>(key);
		Bucket *pBucket = &buckets[hashVal & (NUM_BUCKETS - 1)];
		for (Entry *pEntry = pBucket->entries; pEntry; pEntry = pEntry->next)
			if (pEntry->key.Equal(key, hashVal)) return false;
		numEntries++;
		Entry *newEntry = Pool_CAlloc<Entry>();
		newEntry->key.Set(key, hashVal);
		pBucket->Insert(newEntry);
		return true;
	}

	void InsertList(Init_List &&inList)
	{
		for (auto iter = inList.begin(); iter != inList.end(); ++iter)
			Insert(*iter);
	}

	bool HasKey(Key_Arg key) const
	{
		if (numEntries)
		{
			UInt32 hashVal = HashKey<T_Key>(key);
			for (Entry *pEntry = buckets[hashVal & (NUM_BUCKETS - 1)].entries; pEntry; pEntry = pEntry->next)
				if (pEntry->key.Equal(key, hashVal)) return true;
		}
		return false;
	}

	bool Erase(Key_Arg key)
	{
		if (numEntries)
		{
			UInt32 hashVal = HashKey<T_Key>(key);
			Bucket *pBucket = &buckets[hashVal & (NUM_BUCKETS - 1)];
			for (Entry *pEntry = pBucket->entries; pEntry; pEntry = pEntry->next)
			{
				if (!pEntry->key.Equal(key, hashVal))
					continue;
				numEntries--;
				pBucket->Remove(pEntry);
				return true;
			}
		}
		return false;
	}

	void Clear()
	{
		if (numEntries)
		{
			Bucket *pBucket = buckets, *pEnd = End();
			do
			{
				pBucket->~Bucket();
			}
			while (++pBucket != pEnd);
		}
		numEntries &= 0;
	}

	class Iterator
	{
	protected:
		UnorderedSet	*table;
		Bucket			*bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Bucket *pEnd = table->End(); bucket != pEnd; bucket++)
				if (entry = bucket->entries) return;
		}

	public:
		void Init(UnorderedSet &_table)
		{
			table = &_table;
			entry = nullptr;
			if (table->numEntries)
			{
				bucket = table->buckets;
				FindNonEmpty();
			}
		}

		Key_Res operator*() const {return entry->key.Get();}
		Key_Res operator->() const {return entry->key.Get();}

		explicit operator bool() const {return entry != nullptr;}
		void operator++()
		{
			if (entry = entry->next)
				return;
			bucket++;
			FindNonEmpty();
		}

		Iterator() : table(nullptr), entry(nullptr) {}
		Iterator(UnorderedSet &_table) {Init(_table);}
	};

	Iterator Begin() {return Iterator(*this);}
};

template <typename T_Data, const UInt32 _default_alloc = VECTOR_DEFAULT_ALLOC> class Vector
{
protected:
	using Data_Arg = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, const T_Data&>;
	using Data_Val = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&&>;
	using Data_Res = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&>;
	using Init_List = std::initializer_list<T_Data>;
	Use_ArrayUtils(Vector, T_Data)

	T_Data		*data;		// 00
	UInt32		numItems;	// 04
	UInt32		numAlloc;	// 08

	__declspec(noinline) T_Data *AllocateData()
	{
		if (!data)
		{
			numAlloc = AlignNumAlloc<T_Data>(numAlloc);
			data = Pool_CAlloc<T_Data>(numAlloc);
		}
		else if (numAlloc <= numItems)
		{
			UInt32 newAlloc = numAlloc << 1;
			data = Pool_CRealloc<T_Data>(data, numAlloc, newAlloc);
			numAlloc = newAlloc;
		}
		return data + numItems++;
	}

	T_Data *End() const {return data + numItems;}

public:
	Vector(UInt32 _alloc = _default_alloc) : data(nullptr), numItems(0), numAlloc(_alloc) {}
	Vector(Init_List &&inList) : data(nullptr), numItems(0), numAlloc(inList.size()) {AppendList(std::forward<Init_List>(inList));}
	~Vector()
	{
		if (data)
		{
			Clear();
			Pool_CFree<T_Data>(data, numAlloc);
		}
	}

	void Destroy()
	{
		this->~Vector();
		data = nullptr;
	}

	UInt32 Size() const {return numItems;}
	bool Empty() const {return !numItems;}

	T_Data* Data() const {return const_cast<T_Data*>(data);}

	void operator=(Vector &&rhs)
	{
		this->~Vector();
		data = rhs.data;
		numItems = rhs.numItems;
		numAlloc = rhs.numAlloc;
		rhs.data = nullptr;
		rhs.numItems = 0;
		rhs.numAlloc = VECTOR_DEFAULT_ALLOC;
	}

	void operator=(const Vector &rhs)
	{
		Clear();
		if (numItems = rhs.numItems)
		{
			if (numAlloc < numItems)
			{
				if (data)
				{
					Pool_CFree<T_Data>(data, numAlloc);
					data = nullptr;
				}
				numAlloc = AlignNumAlloc<T_Data>(numItems);
			}
			if (!data)
				data = Pool_CAlloc<T_Data>(numAlloc);
			MemCopy(data, rhs.data, sizeof(T_Data) * numItems);
		}
	}

	T_Data& operator[](UInt32 index) const {return data[index];}

	T_Data *GetPtr(UInt32 index) const {return (index < numItems) ? (data + index) : nullptr;}

	Data_Res Top() const {return data[numItems - 1];}

	T_Data* Append(Data_Val item)
	{
		T_Data *pData = AllocateData();
		*pData = std::move(item);
		return pData;
	}

	template <typename ...Args>
	T_Data* Append(Args&& ...args)
	{
		return new (AllocateData()) T_Data(std::forward<Args>(args)...);
	}

	void AppendList(Init_List &&inList)
	{
		UInt32 count = inList.size();
		T_Data *pData = Reserve(count);
		numItems += count;
		for (auto iter = inList.begin(); iter != inList.end(); ++iter)
		{
			new (pData) T_Data(std::move(*iter));
			pData++;
		}
	}

	T_Data* Insert(UInt32 index, Data_Val item)
	{
		if (index > numItems)
			index = numItems;
		UInt32 size = numItems - index;
		T_Data *pData = AllocateData();
		if (size)
		{
			pData = data + index;
			MemCopy(pData + 1, pData, sizeof(T_Data) * size);
		}
		*pData = std::move(item);
		return pData;
	}

	template <typename ...Args>
	T_Data* Insert(UInt32 index, Args&& ...args)
	{
		if (index > numItems)
			index = numItems;
		UInt32 size = numItems - index;
		T_Data *pData = AllocateData();
		if (size)
		{
			pData = data + index;
			MemCopy(pData + 1, pData, sizeof(T_Data) * size);
		}
		return new (pData) T_Data(std::forward<Args>(args)...);
	}

	void Concatenate(const Vector &source)
	{
		if (!source.numItems) return;
		UInt32 newCount = numItems + source.numItems;
		if (!data)
		{
			if (numAlloc < newCount) numAlloc = newCount;
			numAlloc = AlignNumAlloc<T_Data>(numAlloc);
			data = Pool_CAlloc<T_Data>(numAlloc);
		}
		else if (numAlloc < newCount)
		{
			UInt32 newAlloc = AlignNumAlloc<T_Data>(newCount);
			data = Pool_CRealloc<T_Data>(data, numAlloc, newAlloc);
			numAlloc = newAlloc;
		}
		MemCopy(data + numItems, source.data, sizeof(T_Data) * source.numItems);
		numItems = newCount;
	}

	bool InsertUnique(Data_Val item)
	{
		UInt32 lBound = 0, uBound = numItems;
		while (lBound != uBound)
		{
			UInt32 index = (lBound + uBound) >> 1;
			if (item < data[index])
				uBound = index;
			else if (data[index] < item)
				lBound = index + 1;
			else return false;
		}
		uBound = numItems - lBound;
		T_Data *pData = AllocateData();
		if (uBound)
		{
			pData = data + lBound;
			MemCopy(pData + 1, pData, sizeof(T_Data) * uBound);
		}
		*pData = std::move(item);
		return true;
	}

	SInt32 GetIndexOf(Data_Arg item) const
	{
		if (numItems)
		{
			T_Data *pData = data, *pEnd = End();
			do
			{
				if (*pData == item)
					return pData - data;
			}
			while (++pData != pEnd);
		}
		return -1;
	}

	template <class Matcher>
	SInt32 GetIndexOf(const Matcher &matcher) const
	{
		if (numItems)
		{
			T_Data *pData = data, *pEnd = End();
			do
			{
				if (matcher == *pData)
					return pData - data;
			}
			while (++pData != pEnd);
		}
		return -1;
	}

	template <class Matcher>
	T_Data* Find(const Matcher &matcher) const
	{
		if (numItems)
		{
			T_Data *pData = data, *pEnd = End();
			do
			{
				if (matcher == *pData)
					return pData;
			}
			while (++pData != pEnd);
		}
		return nullptr;
	}

	bool RemoveNth(UInt32 index)
	{
		if (index < numItems)
		{
			T_Data *pData = data + index;
			pData->~T_Data();
			numItems--;
			if (UInt32 size = numItems - index)
				MemCopy(pData, pData + 1, sizeof(T_Data) * size);
			return true;
		}
		return false;
	}

	bool Remove(Data_Arg item)
	{
		if (numItems)
		{
			T_Data *pData = End(), *pEnd = data;
			do
			{
				pData--;
				if (*pData != item) continue;
				numItems--;
				pData->~T_Data();
				if (UInt32 size = (UInt32)End() - (UInt32)pData)
					MemCopy(pData, pData + 1, size);
				return true;
			}
			while (pData != pEnd);
		}
		return false;
	}

	template <class Matcher>
	UInt32 Remove(const Matcher &matcher)
	{
		if (numItems)
		{
			T_Data *pData = End(), *pEnd = data;
			UInt32 removed = 0;
			do
			{
				pData--;
				if (!(matcher == *pData)) continue;
				numItems--;
				pData->~T_Data();
				if (UInt32 size = (UInt32)End() - (UInt32)pData)
					MemCopy(pData, pData + 1, size);
				removed++;
			}
			while (pData != pEnd);
			return removed;
		}
		return 0;
	}

	void RemoveUnordered(Data_Arg item)
	{
		for (UInt32 index = 0; index < numItems; index++)
		{
			if (data[index] != item) continue;
			numItems--;
			if (index < numItems)
				memcpy(data + index, data + numItems, sizeof(T_Data));
			break;
		}
	}

	void RemoveRange(UInt32 beginIdx, UInt32 count)
	{
		if (!count || (beginIdx >= numItems)) return;
		if (count > (numItems - beginIdx))
			count = numItems - beginIdx;
		T_Data *pBgn = data + beginIdx, *pEnd = pBgn + count, *pData = pBgn;
		do
		{
			pData->~T_Data();
		}
		while (++pData != pEnd);
		UInt32 size = (UInt32)End() - (UInt32)pEnd;
		if (size) MemCopy(pBgn, pEnd, size);
		numItems -= count;
	}

	T_Data* Reserve(UInt32 howMany)
	{
		UInt32 newSize = numItems + howMany;
		if (!data)
		{
			numAlloc = AlignNumAlloc<T_Data>(newSize);
			data = Pool_CAlloc<T_Data>(numAlloc);
		}
		else if (numAlloc < newSize)
		{
			UInt32 newAlloc = AlignNumAlloc<T_Data>(newSize);
			data = Pool_CRealloc<T_Data>(data, numAlloc, newAlloc);
			numAlloc = newAlloc;
		}
		return data + numItems;
	}

	void Resize(UInt32 newSize)
	{
		if (numItems == newSize)
			return;
		T_Data *pData, *pEnd;
		if (numItems < newSize)
		{
			if (!data)
			{
				numAlloc = AlignNumAlloc<T_Data>(newSize);
				data = Pool_CAlloc<T_Data>(numAlloc);
			}
			else if (numAlloc < newSize)
			{
				UInt32 newAlloc = AlignNumAlloc<T_Data>(newSize);
				data = Pool_CRealloc<T_Data>(data, numAlloc, newAlloc);
				numAlloc = newAlloc;
			}
			pData = data + numItems;
			pEnd = data + newSize;
			do
			{
				new (pData) T_Data();
			}
			while (++pData != pEnd);
		}
		else
		{
			pData = data + newSize;
			pEnd = End();
			do
			{
				pData->~T_Data();
			}
			while (++pData != pEnd);
		}
		numItems = newSize;
	}

	T_Data Pop()
	{
		static_assert(std::is_scalar_v<T_Data>);
		return numItems ? data[--numItems] : NULL;
	}

	inline bool operator==(const Vector &rhs) const
	{
		if (numItems != rhs.numItems)
			return false;
		if (!numItems)
			return true;
		if (std::is_pod_v<T_Data>)
			return MemCmp(data, rhs.data, sizeof(T_Data) * numItems);
		UInt32 count = numItems;
		T_Data *pData1 = data, *pData2 = rhs.data;
		while (true)
		{
			if (!(*pData1 == *pData2))
				return false;
			if (!--count) break;
			pData1++;
			pData2++;
		}
		return true;
	}
	inline bool operator!=(const Vector &rhs) const {return !(*this == rhs);}

	void Clear()
	{
		for (T_Data *pData = data, *pEnd = End(); pData != pEnd; pData++)
			pData->~T_Data();
		numItems &= 0;
	}

	class Iterator
	{
	protected:
		T_Data		*pData;
		UInt32		count;

		Iterator() {}

	public:
		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pData++;
			count--;
		}
		void operator+=(UInt32 _count)
		{
			if (_count >= count)
				count = 0;
			else
			{
				pData += _count;
				count -= _count;
			}
		}

		void Init(Vector *source) {pData = source->data; count = source->numItems;}

		Data_Res operator*() const {return *pData;}
		Data_Res operator->() const {return *pData;}
		Data_Res operator()() const {return *pData;}
		T_Data& Ref() {return *pData;}

		Iterator(Vector &source) : pData(source.data), count(source.numItems) {}
		Iterator(Vector &source, UInt32 index) : count(source.numItems)
		{
			if (count <= index)
				count = 0;
			else
			{
				pData = source.data + index;
				count -= index;
			}
		}
	};

	class RvIterator : public Iterator
	{
		using Iterator::pData, Iterator::count;
	public:
		void operator--()
		{
			pData--;
			count--;
		}
		void operator-=(UInt32 _count)
		{
			if (_count >= count)
				count = 0;
			else
			{
				pData -= _count;
				count -= _count;
			}
		}

		void Remove(Vector &source)
		{
			pData->~T_Data();
			if (UInt32 size = source.numItems - count)
				MemCopy(pData, pData + 1, size * sizeof(T_Data));
			source.numItems--;
		}

		RvIterator(Vector &source)
		{
			if (count = source.numItems)
				pData = source.data + (count - 1);
		}
	};

	class CpIterator : public Iterator
	{
		using Iterator::pData, Iterator::count;

		AuxBuffer<T_Data>	buffer;

	public:
		CpIterator(Vector &source)
		{
			count = source.numItems;
			if (count > 1)
				pData = buffer.Copy(count, source.data);
			else
				pData = source.data;
		}
	};

	Iterator Begin() {return Iterator(*this);}
	Iterator BeginAt(UInt32 index) {return Iterator(*this, index);}

	RvIterator BeginRv() {return RvIterator(*this);}

	CpIterator BeginCp() {return CpIterator(*this);}
};

template <typename T_Data, const UInt32 size> class FixedTypeArray
{
	using Data_Arg = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&&>;
	using Data_Res = std::conditional_t<std::is_scalar_v<T_Data>, T_Data, T_Data&>;
	Use_ArrayUtils(FixedTypeArray, T_Data)

	UInt32		numItems;
	T_Data		data[size];

public:
	FixedTypeArray() : numItems(0) {}

	UInt32 Size() const {return numItems;}
	bool Empty() const {return !numItems;}
	bool Full() const {return numItems == size;}
	T_Data *Data() const {return const_cast<T_Data*>(&data[0]);}

	T_Data& operator[](UInt32 index) const {return data[index];}

	bool Append(Data_Arg item)
	{
		if (numItems < size)
		{
			data[numItems++] = std::move(item);
			return true;
		}
		return false;
	}

	SInt32 GetIndexOf(Data_Arg item) const
	{
		for (SInt32 idx = 0; idx < numItems; idx++)
			if (data[idx] == item)
				return idx;
		return -1;
	}
	
	bool RemoveNth(UInt32 index)
	{
		if (index < numItems)
		{	
			T_Data *pData = data + index;
			pData->~T_Data();
			numItems--;
			if (UInt32 count = numItems - index)
				MemCopy(pData, pData + 1, sizeof(T_Data) * count);
			return true;
		}
		return false;
	}

	T_Data *PopBack()
	{
		return numItems ? &data[--numItems] : nullptr;
	}

	void Clear() {numItems = 0;}

	class Iterator
	{
	protected:
		T_Data		*pData;
		UInt32		count;

	public:
		explicit operator bool() const {return count != 0;}
		void operator++()
		{
			pData++;
			count--;
		}

		Data_Res operator*() const {return *pData;}
		Data_Res operator->() const {return *pData;}
		Data_Res operator()() const {return *pData;}
		T_Data& Ref() {return *pData;}

		Iterator(FixedTypeArray &source) : pData(source.data), count(source.numItems) {}
	};

	Iterator Begin() {return Iterator(*this);}
};