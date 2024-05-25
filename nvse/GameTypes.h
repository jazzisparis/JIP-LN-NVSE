#pragma once

// 8
class String
{
public:
	char		*m_data;
	UInt16		m_dataLen;
	UInt16		m_bufLen;

	String() : m_data(nullptr), m_dataLen(0), m_bufLen(0) {}
	~String() {if (m_data) Game_HeapFree(m_data);}

	void Init(UInt32 bufSize);
	void Set(const char *src);
	void Append(const char *toAppend);
	void AppendChar(char toAppend);
	void InsertChar(char toInsert, UInt32 index);
	void EraseAt(UInt32 index);
	const char *CStr();
};

enum
{
	eListInvalid =	-1,
	eListEnd =		-2,
	eListCount =	-3,
};

template <class Item> class tList
{
	Use_LinkedListUtils(tList, Item*)
public:
	struct Node
	{
		Item		*data;
		Node		*next;

		Node() : data(NULL), next(nullptr) {}
		Node(Item *_data) : data(_data), next(nullptr) {}
		Node(const Node &rhs) : data(rhs.data), next(rhs.next) {}

		Node *GetNth(UInt32 index)
		{
			Node *pNode = this;
			while (index--)
				if (!(pNode = pNode->next))
					break;
			return pNode;
		}

		inline void operator=(Item *_data)
		{
			data = _data;
			next = nullptr;
		}
		inline void operator=(const Node &rhs)
		{
			data = rhs.data;
			next = rhs.next;
		}

		Node *RemoveMe()
		{
			if (next)
			{
				Node *pNext = next;
				data = next->data;
				next = next->next;
				Game_HeapFree(pNext);
				return this;
			}
			data = NULL;
			return NULL;
		}

		Node *RemoveNext()
		{
			Node *pNext = next;
			next = next->next;
			Game_HeapFree(pNext);
			return next;
		}

		Node *Append(Item *_data)
		{
			Node *newNode = Game_HeapAlloc<Node>();
			newNode->data = _data;
			newNode->next = next;
			next = newNode;
			return newNode;
		}

		Node *Insert(Item *_data)
		{
			Node *newNode = Game_HeapAlloc<Node>();
			newNode->data = data;
			data = _data;
			newNode->next = next;
			next = newNode;
			return newNode;
		}
	};

	Node	m_listHead;

	tList() {}
	tList(Item *item) {m_listHead = item;}
	tList(const tList &rhs) {m_listHead = rhs.m_listHead;}

	inline void operator=(const tList &rhs) {m_listHead = rhs.m_listHead;}

	template <class Matcher>
	UInt32 FreeNodes(Node *node, const Matcher &matcher) const
	{
		static UInt32 nodeCount = 0, numFreed = 0, lastNumFreed = 0;
		if (node->next)
		{
			nodeCount++;
			FreeNodes(node->next, matcher);
			nodeCount--;
		}
		if (matcher == node->data)
		{
			node->RemoveMe();
			numFreed++;
		}
		if (!nodeCount)
		{
			lastNumFreed = numFreed;
			numFreed = 0;
		}
		return lastNumFreed;
	}

	Node *GetLastNode(SInt32 *outIdx = NULL) const
	{
		SInt32 index = 0;
		Node *node = Head();
		while (node->next)
		{
			node = node->next;
			index++;
		}
		if (outIdx) *outIdx = index;
		return node;
	}

	Node *GetNthNode(SInt32 index) const
	{
		if (index >= 0)
		{
			Node *node = Head();
			do
			{
				if (!index) return node;
				index--;
			}
			while (node = node->next);
		}
		return NULL;
	}

	void Init(Item *item = NULL) {m_listHead = item;}

	Node *Head() const {return const_cast<Node*>(&m_listHead);}

	bool Empty() const {return !m_listHead.data;}

	class Iterator
	{
		Node	*m_curr;

	public:
		explicit operator bool() const {return m_curr != NULL;}
		void operator++() {m_curr = m_curr->next;}

		Item* Get() const {return m_curr->data;}
		Item* operator->() const {return m_curr->data;}
		Item* operator*() const {return m_curr->data;}

		const Iterator& operator=(const Iterator &rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}

		Iterator(Node *node = NULL) : m_curr(node) {}
	};
	
	const Iterator Begin() const {return Iterator(Head());}

	UInt32 Count() const
	{
		if (!m_listHead.data) return 0;
		Node *node = Head();
		UInt32 count = 1;
		while (node = node->next) count++;
		return count;
	};

	bool IsInList(Item *item) const
	{
		Node *node = Head();
		do
		{
			if (node->data == item)
				return true;
		}
		while (node = node->next);
		return false;
	}

	Item *GetFirstItem() const
	{
		return m_listHead.data;
	}

	Item *GetLastItem() const
	{
		return GetLastNode()->data;
	}

	Item *GetNthItem(SInt32 index) const
	{
		Node *node = GetNthNode(index);
		return node ? node->data : NULL;
	}

	SInt32 Insert(Item *item, SInt32 index)
	{
		if (!item) return eListInvalid;
		if (!index)
		{
			if (m_listHead.data)
				m_listHead.Insert(item);
			else m_listHead.data = item;
		}
		else if (Node *node = GetNthNode(index))
			node->Insert(item);
		else return eListInvalid;
		return index;
	}

	SInt32 Append(Item *item)
	{
		SInt32 index = eListInvalid;
		if (item)
		{
			Node *node = GetLastNode(&index);
			if (node->data) node->Append(item);
			else node->data = item;
		}
		return index;
	}

	void Prepend(Item *item)
	{
		if (item)
		{
			if (m_listHead.data) m_listHead.Insert(item);
			else m_listHead.data = item;
		}
	}

	bool AppendNotIn(Item *item)
	{
		Node *curr = Head(), *prev;
		do
		{
			if (curr->data == item)
				return false;
			prev = curr;
		}
		while (curr = curr->next);
		if (prev->data)
		{
			Node *newNode = Game_HeapAlloc<Node>();
			newNode->data = item;
			newNode->next = NULL;
			prev->next = newNode;
		}
		else prev->data = item;
		return true;
	}

	void CopyFrom(tList &sourceList)
	{
		Node *target = Head(), *source = sourceList.Head();
		RemoveAll();
		if (!source->data) return;
		target->data = source->data;
		while (source = source->next)
			target = target->Append(source->data);
	}

	template <class Matcher>
	void Visit(const Matcher &matcher, Node *prev = NULL) const
	{
		Node *curr = prev ? prev->next : Head();
		while (curr)
		{
			if (!curr->data || !(matcher == curr->data))
				break;
			curr = curr->next;
		}
	}

	template <class Matcher>
	Item *Find(const Matcher &matcher) const
	{
		Node *curr = Head();
		do
		{
			if (Item *pItem = curr->data; pItem && (matcher == pItem))
				return pItem;
		}
		while (curr = curr->next);
		return nullptr;
	}

	template <class Matcher>
	Iterator Find(const Matcher &matcher, Iterator &prev) const
	{
		Iterator curIt = prev.End() ? Begin() : ++prev;
		while (!curIt.End())
		{
			if (*curIt && (matcher == *curIt)) break;
			++curIt;
		}
		return curIt;
	}

	template <class Matcher>
	UInt32 CountIf(const Matcher &matcher) const
	{
		UInt32 count = 0;
		Node *curr = Head();
		do
		{
			if (curr->data && (matcher == curr->data))
				count++;
		}
		while (curr = curr->next);
		return count;
	}

	void RemoveAll()
	{
		auto topNode = Head();
		while (auto iter = topNode->next)
		{
			topNode->next = iter->next;
			Game_HeapFree(iter);
		}
		topNode->data = nullptr;
	}

	void DeleteAll()
	{
		auto topNode = Head();
		while (auto iter = topNode->next)
		{
			Game_HeapFree(iter->data);
			topNode->next = iter->next;
			Game_HeapFree(iter);
		}
		Game_HeapFree(topNode->data);
		topNode->data = nullptr;
	}

	Item *RemoveNth(SInt32 idx)
	{
		Item *removed = NULL;
		if (idx <= 0)
		{
			removed = m_listHead.data;
			m_listHead.RemoveMe();
		}
		else
		{
			Node *node = Head();
			while (node->next && --idx)
				node = node->next;
			if (!idx)
			{
				removed = node->next->data;
				node->RemoveNext();
			}
		}
		return removed;
	};

	bool Remove(Item *item)
	{
		Node *curr = Head(), *prev = nullptr;
		do
		{
			if (curr->data == item)
			{
				if (prev) prev->RemoveNext();
				else curr->RemoveMe();
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
		while (curr);
		return false;
	}

	template <class Matcher>
	Item *RemoveIf(const Matcher &matcher)
	{
		Node *curr = Head(), *prev = nullptr;
		do
		{
			if (Item *item = curr->data; item && (matcher == item))
			{
				if (prev) prev->RemoveNext();
				else curr->RemoveMe();
				return item;
			}
			prev = curr;
			curr = curr->next;
		}
		while (curr);
		return nullptr;
	}

	Item *ReplaceNth(SInt32 index, Item *item)
	{
		Item *replaced = nullptr;
		if (item)
		{
			Node *node = GetNthNode(index);
			if (!node) return nullptr;
			replaced = node->data;
			node->data = item;
		}
		return replaced;
	}

	SInt32 Replace(Item *item, Item *replace)
	{
		SInt32 index = 0;
		Node *curr = Head();
		do
		{
			if (curr->data == item)
			{
				curr->data = replace;
				return index;
			}
			index++;
		}
		while (curr = curr->next);
		return -1;
	}

	SInt32 GetIndexOf(Item *item)
	{
		SInt32 idx = 0;
		Node *curr = Head();
		do
		{
			if (curr->data == item)
				return idx;
			idx++;
		}
		while (curr = curr->next);
		return -1;
	}

	template <class Matcher>
	SInt32 GetIndexOf(const Matcher &matcher)
	{
		SInt32 idx = 0;
		Node *curr = Head();
		do
		{
			if (curr->data && (matcher == curr->data))
				return idx;
			idx++;
		}
		while (curr = curr->next);
		return -1;
	}
};

template <class Item> class DList
{
	Use_LinkedListUtils(DList, Item*)
public:
	struct Node
	{
		Node		*next;
		Node		*prev;
		Item		*data;

		Node *GetNth(UInt32 index)
		{
			Node *pNode = this;
			while (index--)
				if (!(pNode = pNode->next))
					break;
			return pNode;
		}
	};

private:
	Node		*first;
	Node		*last;
	UInt32		count;

	Node *GetNthNode(UInt32 index) const
	{
		return first ? first->GetNth(index) : nullptr;
	}

	static Node *NewNode() {return CdeclCall<Node*>(0x43A010);}

	void ReleaseNode(Node *pNode)
	{
		if (pNode->prev)
			pNode->prev->next = pNode->next;
		else first = pNode->next;
		if (pNode->next)
			pNode->next->prev = pNode->prev;
		else last = pNode->prev;
		StdCall(0x45CEE0, pNode);
		count--;
	}

	Node *FindNode(Item *pItem) const
	{
		for (auto iter = first; iter; iter = iter->next)
			if (iter->data == pItem)
				return iter;
		return nullptr;
	}

	template <class Matcher>
	Node *FindNode(const Matcher &matcher) const
	{
		for (auto iter = first; iter; iter = iter->next)
			if (matcher == iter->data)
				return iter;
		return nullptr;
	}

public:
	DList() : first(nullptr), last(nullptr), count(0) {}
	DList(Node *_first, Node *_last, UInt32 _count) : first(_first), last(_last), count(_count) {}
	DList(const DList &rhs) {*this = rhs;}

	void operator=(const DList &rhs)
	{
		first = rhs.first;
		last = rhs.last;
		count = rhs.count;
	}

	bool Empty() const {return !first;}
	Node *Head() const {return first;}
	Node *Tail() const {return last;}
	UInt32 Count() const {return count;}

	void RemoveAll() {ThisCall(0x4ED900, this);}

	void Prepend(Item *pData)
	{
		Node *newNode = NewNode();
		newNode->next = first;
		newNode->data = pData;
		if (first)
			first->prev = newNode;
		first = newNode;
		if (!last)
			last = newNode;
		count++;
	}

	void Append(Item *pData)
	{
		Node *newNode = NewNode();
		newNode->prev = last;
		newNode->data = pData;
		if (last)
			last->next = newNode;
		last = newNode;
		if (!first)
			first = newNode;
		count++;
	}

	Item *GetNthItem(UInt32 index) const
	{
		Node *node = GetNthNode(index);
		return node ? node->data : NULL;
	}

	bool Remove(Item *pData)
	{
		if (Node *pNode = FindNode(pData))
		{
			ReleaseNode(pNode);
			return true;
		}
		return false;
	}

	template <class Matcher>
	bool Remove(const Matcher &matcher)
	{
		if (Node *pNode = FindNode(matcher))
		{
			ReleaseNode(pNode);
			return true;
		}
		return false;
	}

	SInt32 GetIndexOf(Item *pItem) const
	{
		SInt32 resIdx = 0;
		for (auto iter = first; iter; iter = iter->next)
		{
			if (iter->data == pItem)
				return resIdx;
			resIdx++;
		}
		return -1;
	}

	template <class Matcher>
	SInt32 GetIndexOf(const Matcher &matcher) const
	{
		SInt32 resIdx = 0;
		for (auto iter = first; iter; iter = iter->next)
		{
			if (matcher == iter->data)
				return resIdx;
			resIdx++;
		}
		return -1;
	}

	class Iterator
	{
		Node	*pNode;

	public:
		Iterator() : pNode(nullptr) {}
		Iterator(Node *bgn) : pNode(bgn) {}

		explicit operator bool() const {return pNode != nullptr;}
		void operator++() {pNode = pNode->next;}
		void operator--() {pNode = pNode->prev;}

		Node *GetNode() const {return pNode;}

		Item* operator*() const {return pNode->data;}
		Item* operator->() const {return pNode->data;}
		Node* operator()() const {return pNode;}
	};

	Iterator Begin() {return Iterator(first);}
	Iterator RBegin() {return Iterator(last);}

	Iterator Find(Item *pItem) {return Iterator(FindNode(pItem));}

	template <class Matcher>
	Iterator Find(const Matcher &matcher) {return Iterator(FindNode(matcher));}

	bool Remove(Iterator &iter, bool forward = true)
	{
		Node *pNode = iter();
		if (forward) ++iter;
		else --iter;
		ReleaseNode(pNode);
		return iter() != nullptr;
	}
};

// 010
template <class T>
class BSSimpleList
{
public:
	virtual void	Unk_00(UInt32 arg);
	virtual void	Unk_01(void);
	virtual void	Unk_02(UInt32 arg);
	virtual void	Unk_03(UInt32 arg);
	virtual void	Unk_04(void);
	virtual UInt32	Unk_05(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_06(UInt32 arg);
	virtual void	Unk_07(void);
	virtual void	Unk_08(UInt32 arg);

	tList<T>	list;
};
static_assert(sizeof(BSSimpleList<void *>) == 0xC);

template <typename T_Data> class BSSimpleArray
{
	Use_ArrayUtils(BSSimpleArray, T_Data)
public:
	virtual void	Destroy(bool doFree);
	virtual T_Data	*Allocate(UInt32 size);

	T_Data		*data;		// 04
	UInt32		size;		// 08
	UInt32		alloc;		// 0C

	UInt32 Size() const {return size;}
	bool Empty() const {return !size;}
	T_Data *Data() const {return const_cast<T_Data*>(data);}

	T_Data& operator[](UInt32 idx) {return data[idx];}

	class Iterator
	{
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

		Iterator(BSSimpleArray &source) : pData(source.data), count(source.size) {}
	};

	Iterator Begin() {return Iterator(*this);}

	__forceinline void Append(T_Data *item)
	{
		ThisCall(0x7CB2E0, this, item);
	}
};

struct TimePair
{
	float	start;
	float	cooldown;
};