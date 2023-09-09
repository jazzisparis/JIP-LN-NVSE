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

// 24
class BSArchiveHeader
{
public:
	UInt32		uiTag;					// 00
	UInt32		uiVersion;				// 04
	UInt32		uiHeaderSize;			// 08
	UInt32		uiFlag;					// 0C
	UInt32		uiDirectories;			// 10
	UInt32		uiFiles;				// 14
	UInt32		uiDirectoryNamesLength;	// 18
	UInt32		uiFileNamesLength;		// 1C
	UInt16		usArchiveType;			// 20
	UInt16		gap22;					// 22
};

// 70
class BSArchive : public BSArchiveHeader
{
public:
	UInt32				*pDirectories;				// 24
	FILETIME			archiveFileTime;			// 28
	UInt32				uiFileNameArrayOffset;		// 30
	UInt32				uiLastDirectoryIndex;		// 34
	UInt32				uiLastFileIndex;			// 38
	CRITICAL_SECTION	archiveCriticalSection;		// 3C
	UInt8				cArchiveFlags;				// 54
	UInt8				pad55[3];					// 55
	UInt32				pDirectoryStringArray;		// 58
	UInt32				*pDirectoryStringOffsets;	// 5C
	UInt32				pFileNameStringArray;		// 60
	UInt32				pFileNameStringOffsets;		// 64
	UInt32				uiID;						// 68
	UInt32				unk6C;						// 6C
};
static_assert(sizeof(BSArchive) == 0x70);

// 1D0
class Archive : public BSFile
{
public:
	NiRefObject			refObject;		// 158
	BSArchive			archive;		// 160
};
static_assert(sizeof(Archive) == 0x1D0);

// 160
class ArchiveFile : public BSFile
{
public:
	UInt32			unk158;		// 158
	UInt32			unk15C;		// 15C
};
static_assert(sizeof(ArchiveFile) == 0x160);

// 178
class CompressedArchiveFile : public ArchiveFile
{
public:
	void			*ptr160;		// 160
	void			*ptr164;		// 164
	UInt32			streamLength;	// 168
	UInt32			unk16C;			// 16C
	UInt32			streamOffset;	// 170
	UInt32			unk174;			// 174
};
static_assert(sizeof(CompressedArchiveFile) == 0x178);

// 5C8
class NiStream
{
public:
	virtual void	Destroy(bool doFree);
	virtual bool	Load(NiBinaryStream *pkIstr);
	virtual bool	Load1(char *pcBuffer, SInt32 *iBufferSize);
	virtual bool	Load2(const char *pcName);
	virtual bool	Save(NiBinaryStream *pkOstr);
	virtual bool	Save1(char **pcBuffer, SInt32 *iBufferSize);
	virtual bool	Save2(const char *pcName);
	virtual void	Unk_07();
	virtual void	RegisterFixedString();
	virtual bool	RegisterSaveObject(NiObject *pkObject);
	virtual void	ChangeObject(NiObject *pkNewObject);
	virtual UInt32	GetLinkIDFromObject(const NiObject *pkObject);
	virtual void	SaveLinkID(NiObject *pkObject);
	virtual bool	LoadHeader();
	virtual void	SaveHeader();
	virtual bool	LoadStream();
	virtual bool	SaveStream();
	virtual void	RegisterObjects();
	virtual void	LoadTopLevelObjects();
	virtual void	SaveTopLevelObjects();
	virtual bool	LoadObject();
	virtual UInt32	PreSaveObjectSizeTable();
	virtual bool	SaveObjectSizeTable(UInt32 uiStartOffset);
	virtual bool	LoadObjectSizeTable();

	enum ThreadStatus
	{
		eTS_IDLE,
		eTS_LOADING,
		eTS_CANCELLING,
		eTS_PAUSING,
		eTS_PAUSED
	};

	enum ErrorMessages
	{
		STREAM_OKAY,
		FILE_NOT_LOADED,
		NOT_NIF_FILE,
		OLDER_VERSION,
		LATER_VERSION,
		NO_CREATE_FUNCTION,
		ENDIAN_MISMATCH
	};

	struct BSStreamHeader
	{
		UInt32	NifBSVersion;
		char	author[0x40];
		char	processScript[0x40];
		char	exportScript[0x40];
	};

	struct NiObjectGroup
	{
		UInt32		size;
		void		*pBuffer;
		void		*pFree;
		UInt32		refCount;
	};

	class NiSearchPath
	{
		UInt32	nextPath;
		char	filePath[0x104];
		char	referencePath[0x104];
	};

	BSStreamHeader					streamHeader;		// 004
	NiTArray<NiObjectGroup*>		objGroups;			// 0C8
	UInt32							NifFileVersion;		// 0D8
	UInt32							userDefinedVersion;	// 0DC
	char							fileName[0x104];	// 0E0
	bool							saveLittleEndian;	// 1E4
	bool							sourceLittleEndian;	// 1E5
	UInt8							pad1E6[2];			// 1E6
	NiSearchPath					*pSearchPath;		// 1E8
	NiTLargeArray<NiObject*>		objects;			// 1EC
	NiTLargeArray<UInt32>			objectSizes;		// 204
	NiTLargeArray<NiObject*>		topObjects;			// 21C
	NiTLargeArray<NiFixedString>	fixedStrings;		// 234
	NiBinaryStream					*inputStream;		// 24C
	NiBinaryStream					*outputStream;		// 250
	UInt32							unk254[8];			// 254
	NiTMap<NiObject*, UInt32>		registerMap;		// 274
	UInt32							unk284[2];			// 284
	ThreadStatus					threadStatus;		// 28C
	bool							bgLoadExitStatus;	// 290
	UInt8							pad291[3];			// 291
	UInt32							unk294[3];			// 294
	void							*pThread;			// 2A0	NiThread
	void							*bgLoadProcedure;	// 2A4
	UInt32							threadPriority;		// 2A8
	UInt32							pcuAffinity[2];		// 2AC
	char							path2B4[0x104];		// 2B4
	ErrorMessages					lastError;			// 3B8
	char							lastErrorMsg[0x104];// 3BC
	char							referencePath[0x104];	// 4C0
	NiObject						*object5C4;			// 5C4

	void Init() {ThisCall(0xA66150, this);}
	void Clear() {ThisCall(0xA65300, this);}
	void InsertObject(NiObject *pObject) {ThisCall(0xA66370, this, pObject);}
	void ExportToFile(const char *filePath) {Save2(filePath);}
};
static_assert(sizeof(NiStream) == 0x5C8);

// 5D4 (?)
class BSStream : public NiStream
{
public:
	virtual void	Unk_18(void);

	UInt32			unk5C8[3];		// 5C8
};

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

	static UInt32 *GetCounterSingleton() {return (UInt32*)0x11C3B38;}
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
class QueuedChildren : public BSSimpleArray<QueuedFile*>
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
		Game_HeapFree((void*)path);
		if (niNode) NiReleaseObject(niNode);
		Game_HeapFree(this);
	}
};

// 28
class QueuedFile : public IOTask
{
public:
	virtual void DoRequestHandles();
	virtual void DoOnChildrenFinished(UInt32 arg0);
	virtual void DoOnTaskFinished();

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
	virtual void QueueModels();			// Initialize bipedAnims (and cretae the 3D model?)
	virtual void UseDistant3D();
	virtual void AttachDistant3D(NiNode *arg0);
	virtual bool BackgroundClone();
	virtual void DoAttach();
	virtual void FinishAttach();

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

// 60
class BGSTerrainChunkLoadTask : public QueuedFileEntry
{
public:
	UInt32			unk30[12];		// 30
};

// 58
class BGSDistantObjectBlockLoadTask : public QueuedFileEntry
{
public:
	virtual void	Unk_0C(void);

	UInt8			byte30;			// 30
	UInt8			byte31;			// 31
	UInt8			byte32;			// 32
	UInt8			byte33;			// 33
	int				cellX;			// 34
	int				cellY;			// 38
	UInt32			lodLevel;		// 3C
	void			*lodNode14;		// 40
	TESWorldSpace	*worldSpc;		// 44
	NiRefObject		*object48;		// 48
	NiRefObject		*object4C;		// 4C
	UInt8			byte50;			// 50
	UInt8			byte51;			// 51
	UInt8			byte52;			// 52
	UInt8			byte53;			// 53
	UInt32			unk54;			// 54
};
static_assert(sizeof(BGSDistantObjectBlockLoadTask) == 0x58);

// 48
class BGSDistantTreeBlockLoadTask : public QueuedFileEntry
{
public:
	UInt32			unk30[6];		// 30
};

// 14
struct KFModel
{
	const char			*path;					// 00
	BSAnimGroupSequence	*controllerSequence;	// 04
	TESAnimGroup		*animGroup;				// 08
	UInt32				counter1;				// 0C
	UInt32				counter2;				// 10

	__forceinline KFModel *Init(const char *kfPath, BSStream *stream)
	{
		return ThisCall<KFModel*>(0x43B640, this, kfPath, stream);
	}
};

// 38
class AnimIdle : public NiRefObject
{
public:
	UInt32					unk08;			// 08
	UInt32					unk0C;			// 0C
	UInt32					unk10;			// 10
	UInt32					sequenceID;		// 14
	BSAnimGroupSequence		*agSequence;	// 18
	NiObject				*object1C;		// 1C
	NiObject				*object20;		// 20
	NiObject				*object24;		// 24
	NiObject				*object28;		// 28
	TESIdleForm				*idleForm;		// 2C
	UInt32					unk30;			// 30
	Actor					*actor;			// 34
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

class InterfacedClass
{
public:
	/*00*/virtual void		Destroy(bool doFree);
	/*04*/virtual void		*AllocTLSValue(UInt32 arg);
};

// 40
template <typename T_Key, typename T_Data> class LockFreeMap : public InterfacedClass
{
	Use_HashMapUtils(LockFreeMap)

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

	void			*ptr04;			// 04
	UInt32			numBuckets;		// 08
	Bucket			*buckets;		// 0C
	UInt32			unk10;			// 10
	void			*ptr14;			// 14
	UInt32			numItems;		// 18
	UInt32			unk1C;			// 1C
	LightCS			semaphore;		// 20
	UInt32			unk28[6];		// 28

	Bucket *GetBuckets() const {return buckets;}
	Bucket *End() const {return buckets + numBuckets;}

public:
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

	UInt32 Size() const {return numItems;}
	bool Empty() const {return !numItems;}
	UInt32 BucketCount() const {return numBuckets;}

	class Iterator
	{
		LockFreeMap		*table;
		Bucket			*bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Bucket *end = table->End(); bucket != end; bucket++)
				if (entry = bucket->entries) return;
		}

	public:
		Iterator(LockFreeMap &_table) : table(&_table), bucket(table->buckets), entry(nullptr) {FindNonEmpty();}

		explicit operator bool() const {return entry != nullptr;}
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
static_assert(sizeof(LockFreeMap<int, int>) == 0x40);

class Animation;
class QueuedReplacementKFList;
class QueuedHelmet;
class BSFileEntry;
class LoadedFile;

// 40
template <typename T_Data> class LockFreeQueue : public InterfacedClass
{
public:
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
static_assert(sizeof(LockFreeQueue<int>) == 0x40);

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

	__forceinline static ModelLoader *GetSingleton() {return *(ModelLoader**)0x11C3B3C;}

	__forceinline void QueueReference(TESObjectREFR *refr)
	{
		ThisCall(0x444850, this, refr, 1, false);
	}
	__forceinline NiNode *LoadModel(const char *nifPath, UInt32 baseClass, bool flag3Cbit0, bool flag3Cbit5, bool dontIncCounter)
	{
		return ThisCall<NiNode*>(0x447080, this, nifPath, baseClass, flag3Cbit0, 0, flag3Cbit5, dontIncCounter);
	}
	__forceinline KFModel *LoadKFModel(const char *kfPath)
	{
		return ThisCall<KFModel*>(0x4471C0, this, kfPath);
	}
};

extern ModelLoader *g_modelLoader;

class ExteriorCellLoaderTask;

// 40
class ExteriorCellLoader : public LockFreeMap<UInt32, ExteriorCellLoaderTask*>
{
public:
	__forceinline static ExteriorCellLoader *GetSingleton() {return *(ExteriorCellLoader**)0x11C9618;}
};

// 20
class ExteriorCellLoaderTask : public IOTask
{
public:
	struct TaskData
	{
		SInt32			cellX;
		SInt32			cellY;
		TESWorldSpace	*worldspace;
		UInt32			unk0C;
		UInt8			byte10;
		UInt8			pad11[3];
	};

	ExteriorCellLoader	*extCellLoader;	// 18
	TaskData			*tastData;		// 1C
};

// A0
class IOManager : public LockFreeMap<__int64, IOTask*>
{
public:
	/*48*/virtual void Unk_12(void);
	/*4C*/virtual void Unk_13(void);
	/*50*/virtual void Unk_14(void);
	/*54*/virtual void Unk_15(void);
	/*58*/virtual void Unk_16(void);

	UInt32							unk40[4];	// 40
	void							*ptr50;		// 50
	void							*ptr54;		// 54
	UInt32							unk58[3];	// 58
	LockFreePriorityQueue<IOTask*>	*taskQueue;	// 64
	UInt32							unk68[14];	// 68

	__forceinline static IOManager *GetSingleton() {return *(IOManager**)0x1202D98;}
};
static_assert(sizeof(IOManager) == 0xA0);

// 50
class BSTreeModel : public NiRefObject
{
public:
	/*08*/virtual void	Unk_02(void);
	/*0C*/virtual void	Unk_03(void);
	/*10*/virtual void	Unk_04(void);
	/*14*/virtual void	Unk_05(void);
	/*18*/virtual void	Unk_06(void);
	
	UInt32			unk08[18];
};
static_assert(sizeof(BSTreeModel) == 0x50);

// 20
struct BSTreeManager
{
	NiTMap<TESObjectTREE*, BSTreeModel*>		*treeModelsMap;	// 00
	NiSourceTexture								*canopyShadow;	// 04
	NiMaterialProperty							*materialProp;	// 08
	NiAlphaProperty								*alphaProp;		// 0C
	BSXFlags									*bsxFlags;		// 10
	float										flt14;			// 14
	bool										treesEnabled;	// 18
	bool										leavesEnabled;	// 19
	bool										byte1A;			// 1A
	bool										forceFullLOD;	// 1B
	LockFreeMap<TESObjectREFR*, BSTreeNode*>	*treeNodesMap;	// 1C
	
	__forceinline static BSTreeManager *GetSingleton() {return *(BSTreeManager**)0x11D5C48;}
};
static_assert(sizeof(BSTreeManager) == 0x20);