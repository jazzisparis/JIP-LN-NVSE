#pragma once

/*******************************************************
*
* BSTask
*	IOTask
*		QueuedFile
*			QueuedFileEntry
*				QueuedModel
*					QueuedDistantLOD
*					QueuedTreeModel
*				QueuedTexture
*					QueuedTreeBillboard
*				QueuedKF
*					QueuedAnimIdle
*				DistantLODLoaderTask
*				TerrainLODQuadLoadTask
*				SkyTask
*				LipTask
*				GrassLoadTask
*			QueuedReference
*				QueuedTree
*				QueuedActor
*					QueuedCharacter
*						QueuedPlayer
*					QueuedCreature
*			QueuedHead
*			QueuedHelmet
*			QueuedMagicItem
*		AttachDistant3DTask
*		ExteriorCellLoaderTask
*
* NiTArray< NiPointer<QueuedFile> >
*	QueuedChildren
*
*********************************************************/

class BackgroundCloneThread;
class BSFaceGenModel;
class QueuedReference;
class BSTaskManagerThread;

class RefNiRefObject;
class RefNiObject;
struct BSAData;

// 18
class BSTask
{
public:
	virtual void Destroy(bool doFree);
	virtual void Run(void);
	virtual void Unk_02(void);
	virtual void Unk_03(UInt32 arg0, UInt32 arg1);
	virtual bool GetDebugDescription(char *outDesc, void *arg1);
	
	BSTask		*unk04;		// 04
	UInt32		refCounter; // 08
	UInt32		unk0C;		// 0C	Semaphore/Status
	UInt32		unk10;		// 10	Paired : 10 and 14 for a 64 bit integer
	UInt32		unk14;		// 14

	static UInt32 *GetCounterSingleton();
};

// 18
class IOTask : public BSTask
{
public:
	virtual void Unk_05(void);
	virtual void Unk_06(void);				
	virtual void Unk_07(UInt32 arg0);	// most (all?) implementations appear to call IOManager::1202D98(this, arg0) eventually
};

// 20
class AttachDistant3DTask : public IOTask
{
public:
	TESObjectREFR	*refr;		// 18
	NiAVObject		*object1C;	// 1C
};

// 14
class QueuedChildren : public BSSimpleArray<NiPointer<QueuedFile>>
{
	UInt32	counter;
};

struct Model
{
	const char	*path;		// 00
	UInt32		counter1;	// 04
	UInt32		counter2;	// 08
	NiNode		*niNode;	// 0C

	void Destroy()
	{
		GameHeapFree((void*)path);
		if (niNode) NiReleaseObject(niNode);
		GameHeapFree(this);
	}
};

// 28
class QueuedFile : public IOTask
{
public:
	virtual void Unk_08(void);
	virtual void Unk_09(UInt32 arg0);
	virtual void Unk_0A(void);				

	// size?
	struct FileEntry
	{
		UInt32		unk00;
		UInt32		unk04;
		UInt32		size;
		UInt32		offset;
	};

	BSTask			*task18;				// 18	init to tlsData:2B4
	QueuedReference	*queuedRef;			// 1C	could be last QueuedRef
	QueuedChildren	*queuedChildren;	// 20
	UInt32			*unk24;				// 24	struct, 04 is a base, 08 is a length
};

// 40
class QueuedReference : public QueuedFile
{
public:
	virtual void Unk_0B(void);			// Initialize validBip01Names (and cretae the 3D model?)
	virtual void Unk_0C(void);
	virtual void Unk_0D(NiNode *arg0);
	virtual bool Unk_0E(void);
	virtual void Unk_0F(void);
	virtual void Unk_10(void);

	TESObjectREFR	*refr;		// 28
	BSTask			*task2C;	// 2C
	Model			*model;		// 30
	NiNode			*resultObj;	// 34
	BSTask			*task38;	// 38
	UInt32			unk3C;		// 3C	uninitialized
};

// 40
class QueuedActor : public QueuedReference
{
public:
};

// 40
class QueuedCreature : public QueuedActor
{
public:
};

// 48
class QueuedCharacter : public QueuedActor
{
public:
	typedef RefNiRefObject RefQueuedHead;

	RefQueuedHead	*refQueuedHead;	// 40
	RefNiRefObject	*unk44;			// 44
};

// 48
class QueuedPlayer : public QueuedCharacter
{
public:
};

// 030
class QueuedFileEntry : public QueuedFile
{
public:
	virtual bool Unk_0B(void) = 0;

	char	* name;		// 028
	BSAData	* bsaData;	// 02C
};

// 48
class QueuedModel : public QueuedFileEntry
{
public:
	virtual void Unk_0C(UInt32 arg0);

	Model		*model;			// 30
	TESModel	*tesModel;		// 34
	UInt32		baseFormClass;	// 38	table at offset : 0x045C708. Pickable, invisible, unpickable ? 6 is VisibleWhenDistant or internal
	UInt8		flags;			// 3C	bit 0 and bit 1 init'd by parms, bit 2 set after textureSwap, bit 3 is model set, bit 4 is file found.
	UInt8		pad3D[3];		// 3D
	float		flt40;			// 40
	UInt32		unk44;			// 44

	// There are at least 3 Create/Initiator
};

// 30
class QueuedTexture : public QueuedFileEntry
{
public:
	void	* niTexture;	// 030
};

// 014
class KFModel
{
	const char			* path;					// 000
	BSAnimGroupSequence	* controllerSequence;	// 004
	TESAnimGroup		* animGroup;			// 008
	UInt32				unk0C;					// 00C
	UInt32				unk10;					// 010
};

// 30
class QueuedKF : public QueuedFileEntry
{
public:
	KFModel		* kf;		// 030
	UInt8		unk034;		// 034
	UInt8		pad035[3];	// 035
};

// 040
class QueuedAnimIdle : public QueuedKF
{
public:
	ModelLoader	* modelLoader;	// 038	Init"d by arg2
	RefNiObject	* unk03C;		// 03C	Init"d by arg1
};

// 38
class QueuedHead : public QueuedFile
{
public:
	TESNPC			* npc;				// 028
	BSFaceGenNiNode * faceNiNodes[2];	// 02C OBSE presumably male and female
	UInt32			unk034;				// 034
};

/*
// 38
class QueuedHelmet : public QueuedFile
{
public:
	QueuedHelmet();
	~QueuedHelmet();

	QueuedCharacter		* queuedCharacter;		// 18
	QueuedChildren		* queuedChildren;		// 1C
	void				* unk20;				// 20
	QueuedModel			* queuedModel;			// 24
	BSFaceGenModel		* faceGenModel;			// 28
	NiNode				* niNode;				// 2C
	Character			* character;			// 30
	UInt32				unk34;					// 34
};
*/

// 40
template <typename T_Key, typename T_Data> class LockFreeMap
{
public:
	struct Entry
	{
		T_Key		key;
		T_Data		data;
		Entry		*next;
	};

	struct Bucket
	{
		Entry		*entries;
	};

	/*00*/virtual void		Destroy(bool doFree);
	/*04*/virtual void		*Unk_01(UInt32 arg);
	/*08*/virtual bool		Lookup(T_Key key, T_Data *result);
	/*0C*/virtual bool		Unk_03(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt8 arg4);
	/*10*/virtual bool		Insert(T_Key key, T_Data *dataPtr, UInt8 arg3);
	/*14*/virtual bool		EraseKey(T_Key key);
	/*18*/virtual bool		Unk_06(UInt32 arg1, UInt32 arg2);
	/*1C*/virtual bool		Unk_07(UInt32 arg);
	/*20*/virtual bool		Unk_08(UInt32 arg1, UInt32 arg2);
	/*24*/virtual UInt32	CalcBucketIndex(T_Key key);
	/*28*/virtual void		FreeKey(T_Key key);
	/*2C*/virtual T_Key		GenerateKey(T_Key src);
	/*30*/virtual void		CopyKeyTo(T_Key src, T_Key *destPtr);
	/*34*/virtual bool		LKeyGreaterOrEqual(T_Key lkey, T_Key rkey);
	/*38*/virtual bool		KeysEqual(T_Key lkey, T_Key rkey);
	/*3C*/virtual UInt32	IncNumItems();
	/*40*/virtual UInt32	DecNumItems();
	/*44*/virtual UInt32	GetNumItems();

	void			*ptr04;			// 04
	UInt32			numBuckets;		// 08
	Bucket			*buckets;		// 0C
	UInt32			unk10;			// 10
	void			*ptr14;			// 14
	UInt32			numItems;		// 18
	UInt32			unk1C;			// 1C
	LightCS			semaphore;		// 20
	UInt32			unk28[6];		// 28

	class Iterator
	{
		friend LockFreeMap;

		LockFreeMap		*table;
		Bucket			*bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Bucket *end = &table->buckets[table->numBuckets]; bucket != end; bucket++)
				if (entry = bucket->entries) return;
		}

	public:
		Iterator(LockFreeMap &_table) : table(&_table), bucket(table->buckets), entry(NULL) {FindNonEmpty();}

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
STATIC_ASSERT(sizeof(LockFreeMap<int, int>) == 0x40);

class AnimIdle;
class Animation;
class QueuedReplacementKFList;
class QueuedHelmet;
class BSFileEntry;
class LoadedFile;

// 40
template <typename T_Data> class LockFreeQueue
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	*Unk_01(UInt32 arg);
	virtual UInt32	IncNumItems();
	virtual UInt32	DecNumItems();
	virtual UInt32	GetNumItems();

	void			*ptr04;		// 04
	void			*ptr08;		// 08
	UInt32			unk0C;		// 0C
	void			*ptr10;		// 10
	void			*ptr14;		// 14
	UInt32			numItems;	// 18
	UInt32			unk1C;		// 1C
	LightCS			semaphore;	// 20
	UInt32			unk28[6];	// 28
};
STATIC_ASSERT(sizeof(LockFreeQueue<int>) == 0x40);

// 10
template <typename T_Data> class LockFreePriorityQueue
{
public:
	virtual void Destroy(bool doFree);
	virtual void Unk_01(void);
	virtual void Unk_02(void);
	virtual void Unk_03(void);

	LockFreeQueue<T_Data>	**queues;	// 04
	UInt32					numQueues;	// 08
	UInt32					unk0C;		// 0C
};

// 30
// C'tor: 0x442650
struct ModelLoader
{
	LockFreeMap<const char*, Model*>					*modelMap;			// 00
	LockFreeMap<const char*, KFModel*>					*kfMap;				// 04
	LockFreeMap<TESObjectREFR*, QueuedReference*>		*refMap1;			// 08
	LockFreeMap<TESObjectREFR*, QueuedReference*>		*refMap2;			// 0C
	LockFreeMap<AnimIdle*, QueuedAnimIdle*>				*idleMap;			// 10
	LockFreeMap<Animation*, QueuedReplacementKFList*>	*animMap;			// 14
	LockFreeMap<TESObjectREFR*, QueuedHelmet*>			*helmetMap;			// 18
	LockFreeQueue<AttachDistant3DTask*>					*attachQueue;		// 1C
	LockFreeMap<BSFileEntry*, QueuedTexture*>			*textureMap;		// 20
	LockFreeMap<const char*, LoadedFile*>				*fileMap;			// 24
	BackgroundCloneThread								*bgCloneThread;		// 28
	UInt8												byte2C;				// 2C
	UInt8												pad2D[3];			// 2D

	static ModelLoader *GetSingleton();
	__forceinline void QueueReference(TESObjectREFR *refr)
	{
		ThisCall(0x444850, this, refr, 1, false);
	}
};

// A0
class IOManager
{
public:
	virtual void Destroy(bool doFree);
	virtual void Unk_01(void);
	virtual void Unk_02(void);
	virtual void Unk_03(void);
	virtual void Unk_04(void);
	virtual void Unk_05(void);
	virtual void Unk_06(void);
	virtual void Unk_07(void);
	virtual void Unk_08(void);
	virtual void Unk_09(void);
	virtual void Unk_0A(void);
	virtual void Unk_0B(void);
	virtual void Unk_0C(void);
	virtual void Unk_0D(void);
	virtual void Unk_0E(void);
	virtual void Unk_0F(void);
	virtual void Unk_10(void);
	virtual void Unk_11(void);
	virtual void Unk_12(void);
	virtual void Unk_13(void);
	virtual void Unk_14(void);
	virtual void Unk_15(void);
	virtual void Unk_16(void);

	void							*ptr04;		// 04
	UInt32							unk08;		// 08
	void							*ptr0C;		// 0C
	UInt32							unk10;		// 10
	void							*ptr14;		// 14
	UInt32							unk18[14];	// 18
	void							*ptr50;		// 50
	void							*ptr54;		// 54
	UInt32							unk58[3];	// 58
	LockFreePriorityQueue<IOTask*>	*taskQueue;	// 64
	UInt32							unk68[14];	// 68
};
STATIC_ASSERT(sizeof(IOManager) == 0xA0);