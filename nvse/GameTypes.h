#pragma once

// 8
class String
{
public:
	String();
	~String();

	char		*m_data;
	UInt16		m_dataLen;
	UInt16		m_bufLen;

	void Init(UInt32 bufSize);
	bool Set(const char *src);
	bool Append(const char *toAppend);
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

template <typename T_Data> struct ListNode
{
	T_Data		*data;
	ListNode	*next;

	ListNode() : data(NULL), next(NULL) {}
	ListNode(T_Data *_data) : data(_data), next(NULL) {}
	ListNode(const ListNode &rhs) : data(rhs.data), next(rhs.next) {}

	inline void operator=(T_Data *_data)
	{
		data = _data;
		next = NULL;
	}
	inline void operator=(const ListNode &rhs)
	{
		data = rhs.data;
		next = rhs.next;
	}

	T_Data *Data() const {return data;}
	ListNode *Next() const {return next;}

	ListNode *RemoveMe()
	{
		if (next)
		{
			ListNode *pNext = next;
			data = next->data;
			next = next->next;
			GameHeapFree(pNext);
			return this;
		}
		data = NULL;
		return NULL;
	}

	ListNode *RemoveNext()
	{
		ListNode *pNext = next;
		next = next->next;
		GameHeapFree(pNext);
		return next;
	}

	ListNode *Append(T_Data *_data)
	{
		ListNode *newNode = (ListNode*)GameHeapAlloc(sizeof(ListNode));
		newNode->data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}

	ListNode *Insert(T_Data *_data)
	{
		ListNode *newNode = (ListNode*)GameHeapAlloc(sizeof(ListNode));
		newNode->data = data;
		data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}
};

template <class Item> class tList
{
public:
	typedef ListNode<Item> Node;

	Node	m_listHead;

	tList() {}
	tList(Item *item) {m_listHead = item;}
	tList(const tList &rhs) {m_listHead = rhs.m_listHead;}

	template <class Op>
	UInt32 FreeNodes(Node *node, Op &compareOp) const
	{
		static UInt32 nodeCount = 0, numFreed = 0, lastNumFreed = 0;
		if (node->next)
		{
			nodeCount++;
			FreeNodes(node->next, compareOp);
			nodeCount--;
		}
		if (compareOp.Accept(node->data))
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

		Item *Get() const {return m_curr->data;}
		Item* operator->() const {return m_curr->data;}
		Item*& operator*() const {return m_curr->data;}

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
		if (index == eListEnd)
			return GetLastNode()->data;
		Node *node = GetNthNode(index);
		return node ? node->data : NULL;
	}

	SInt32 Insert(Item *item, SInt32 index)
	{
		if (!item) return eListInvalid;
		Node *node;
		if (!index)
		{
			if (m_listHead.data) m_listHead.Insert(item);
			else m_listHead.data = item;
		}
		else if (index == eListEnd)
		{
			node = GetLastNode(&index);
			if (node->data) node->Append(item);
			else node->data = item;
		}
		else
		{
			node = GetNthNode(index);
			if (!node) return eListInvalid;
			node->Insert(item);
		}
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
			Node *newNode = (Node*)GameHeapAlloc(sizeof(Node));
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

	template <class Op>
	void Visit(Op &op, Node *prev = NULL) const
	{
		Node *curr = prev ? prev->next : Head();
		while (curr)
		{
			if (!curr->data || !op.Accept(curr->data)) break;
			curr = curr->next;
		}
	}

	template <class Op>
	Item *Find(Op &op) const
	{
		Node *curr = Head();
		Item *pItem;
		do
		{
			pItem = curr->data;
			if (pItem && op.Accept(pItem))
				return pItem;
		}
		while (curr = curr->next);
		return NULL;
	}

	template <class Op>
	Iterator Find(Op &op, Iterator &prev) const
	{
		Iterator curIt = prev.End() ? Begin() : ++prev;
		while (!curIt.End())
		{
			if (*curIt && op.Accept(*curIt)) break;
			++curIt;
		}
		return curIt;
	}

	const Node *FindString(char *str, Iterator prev) const
	{
		return Find(StringFinder_CI(str), prev);
	}

	template <class Op>
	UInt32 CountIf(Op &op) const
	{
		UInt32 count = 0;
		Node *curr = Head();
		do
		{
			if (curr->data && op.Accept(curr->data))
				count++;
		}
		while (curr = curr->next);
		return count;
	}

	class AcceptAll
	{
	public:
		bool Accept(Item *item) {return true;}
	};

	void RemoveAll() const
	{
		Node *nextNode = Head(), *currNode = nextNode->next;
		nextNode->data = NULL;
		nextNode->next = NULL;
		while (currNode)
		{
			nextNode = currNode->next;
			GameHeapFree(currNode);
			currNode = nextNode;
		}
	}

	void DeleteAll() const
	{
		Node *nextNode = Head(), *currNode = nextNode->next;
		GameHeapFree(nextNode->data);
		nextNode->data = NULL;
		nextNode->next = NULL;
		while (currNode)
		{
			nextNode = currNode->next;
			GameHeapFree(currNode->data);
			GameHeapFree(currNode);
			currNode = nextNode;
		}
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
		Node *curr = Head(), *prev = NULL;
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

	template <class Op>
	bool RemoveIf(Op &op)
	{
		Node *curr = Head(), *prev = NULL;
		do
		{
			if (curr->data && op.Accept(curr->data))
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

	Item *ReplaceNth(SInt32 index, Item *item)
	{
		Item *replaced = NULL;
		if (item)
		{
			Node *node;
			if (eListEnd == index)
				node = GetLastNode();
			else
			{
				node = GetNthNode(index);
				if (!node) return NULL;
			}
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

	template <class Op>
	SInt32 GetIndexOf(Op &op)
	{
		SInt32 idx = 0;
		Node *curr = Head();
		do
		{
			if (curr->data && op.Accept(curr->data))
				return idx;
			idx++;
		}
		while (curr = curr->next);
		return -1;
	}
};
static_assert(sizeof(tList<void *>) == 0x8);

template <typename T_Data> struct DListNode
{
	DListNode	*next;
	DListNode	*prev;
	T_Data		*data;

	DListNode *Advance(UInt32 times)
	{
		DListNode *result = this;
		while (result && times)
		{
			times--;
			result = result->next;
		}
		return result;
	}

	DListNode *Regress(UInt32 times)
	{
		DListNode *result = this;
		while (result && times)
		{
			times--;
			result = result->prev;
		}
		return result;
	}
};

template <class Item> class DList
{
public:
	typedef DListNode<Item> Node;

private:
	Node		*first;
	Node		*last;
	UInt32		count;

public:
	bool Empty() const {return !first;}
	Node *Head() {return first;}
	Node *Tail() {return last;}
	UInt32 Size() const {return count;}
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

template <typename T_Data>
struct BSSimpleArray
{
	virtual void	Destroy(bool doFree);
	virtual T_Data	*Allocate(UInt32 size);

	T_Data		*data;		// 04
	UInt32		size;		// 08
	UInt32		alloc;		// 0C

	T_Data& operator[](UInt32 idx) {return data[idx];}

	class Iterator
	{
		friend BSSimpleArray;

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