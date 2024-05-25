#pragma once

const char* __cdecl GetNiFixedString(const char *inStr);

class NiFixedString
{
	const char		*str;

	UInt32 *Meta() const {return (UInt32*)(str - 8);}

	void Set(const char *inStr)
	{
		str = inStr;
		if (str) InterlockedIncrement(Meta());
	}

	void Unset()
	{
		if (str)
		{
			InterlockedDecrement(Meta());
			str = nullptr;
		}
	}

public:
	NiFixedString() : str(nullptr) {}
	NiFixedString(const char *inStr) : str(GetNiFixedString(inStr))	{}
	NiFixedString(const NiFixedString &inStr) {Set(inStr.str);}
	~NiFixedString() {Unset();}

	const char *Get() const {return str ? str : "NULL";}

	UInt32 Length() const {return str ? Meta()[1] : 0;}

	explicit operator bool() const {return str != nullptr;}

	operator const char*() const {return str;}

	const char* operator*() const {return str;}

	inline void operator=(const char *inStr)
	{
		Unset();
		str = GetNiFixedString(inStr);
	}
	inline void operator=(const NiFixedString &inStr)
	{
		if (str != inStr.str)
			Set(inStr.str);
	}

	inline bool operator==(const NiFixedString &rhs) const {return str == rhs.str;}
	inline bool operator<(const NiFixedString &rhs) const {return str < rhs.str;}

	UInt32 RefCount() const {return str ? Meta()[0] : 0;}
};

template <typename T_Data>
class NiTFixedStringMap
{
public:
	struct Entry
	{
		Entry		*next;
		const char	*key;
		T_Data		*data;
	};

	virtual void	Destroy(bool doFree);
	virtual Entry	*AllocNewEntry();
	virtual void	FreeEntry(Entry *entry);
	
	UInt32		m_numBuckets;	// 04
	Entry		**m_buckets;	// 08
	UInt32		m_numItems;		// 0C
	
	bool HasKey(NiFixedString *key) const
	{
		const char *_key = *key;
		for (Entry *entry = m_buckets[(UInt32)_key % m_numBuckets]; entry; entry = entry->next)
			if (_key == entry->key) return true;
		return false;
	}
	
	T_Data *Lookup(NiFixedString *key) const
	{
		const char *_key = *key;
		for (Entry *entry = m_buckets[(UInt32)_key % m_numBuckets]; entry; entry = entry->next)
			if (_key == entry->key) return entry->data;
		return nullptr;
	}
	
	class Iterator
	{
		NiTFixedStringMap	*table;
		Entry				**bucket;
		Entry				*entry;

		void FindNonEmpty()
		{
			for (Entry **end = &table->m_buckets[table->m_numBuckets]; bucket != end; bucket++)
				if (entry = *bucket) break;
		}

	public:
		Iterator(NiTFixedStringMap &_table) : table(&_table), bucket(table->m_buckets), entry(nullptr) {FindNonEmpty();}

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
		T_Data *Get() const {return entry->data;}
		const char *Key() const {return entry->key;}
	};

	Iterator Begin() {return Iterator(*this);}
};

template <typename T_Data>
class NiSimpleArray
{
public:
	T_Data		*data;
	UInt32		capacity;
	UInt32		numItems;

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

		Iterator(NiSimpleArray &source) : pData(source.data), count(source.numItems) {}
	};

	Iterator Begin() {return Iterator(*this);}
};

class NiMemObject {};

// 08
class NiRefObject : public NiMemObject
{
public:
	/*00*/virtual void		Destructor(bool freeThis);
	/*04*/virtual void		Free(void);

	UInt32		m_uiRefCount;	// 04
};

class TempNiRefObject
{
	NiRefObject		*object;

public:
	TempNiRefObject(NiRefObject *_object) : object(_object) {if (object) InterlockedIncrement(&object->m_uiRefCount);}
	~TempNiRefObject() {if (object) NiReleaseObject(object);}
};

struct NiObjectCopyInfo
{
	NiTMap<NiObject*, NiObject*>	*map00;		// 00
	NiTMap<NiObject*, bool>			*map04;		// 04
	UInt32							unk08;		// 08	Init'd to 1
	UInt8							byte0C;		// 0C	Init'd to 0x24
	UInt8							pad0D[3];	// 0D
	NiVector3						scale;		// 10	Appears to be unused

	NiObjectCopyInfo(UInt32 bucketCount)
	{
		ThisCall(0x4AD0C0, this, bucketCount);
		scale = {1.0F, 1.0F, 1.0F};
	}
	//~NiObjectCopyInfo() {ThisCall(0x4AD1D0, this);}
};

// 0C	c'tor @ 0x43D410
struct NiUpdateData
{
	float		timePassed;			// 00
	bool		updateControllers;	// 04
	bool		isMultiThreaded;	// 05
	UInt8		byte06;				// 06
	bool		updateGeomorphs;	// 07
	bool		updateShadowScene;	// 08
	UInt8		pad09[3];			// 09

	NiUpdateData() {ZeroMemory(this, sizeof(NiUpdateData));}
};

extern const NiUpdateData kNiUpdateData;

typedef FixedTypeArray<hkpWorldObject*, 0x40> ContactObjects;

// 08
class NiObject : public NiRefObject
{
public:
	/*08*/virtual NiRTTI	*GetType();
	/*0C*/virtual NiNode	*GetNiNode();	// Returns this
	/*10*/virtual BSFadeNode	*GetFadeNode();	// Returns this
	/*14*/virtual BSMultiBoundNode	*GetMultiBoundNode();	// Returns this
	/*18*/virtual NiGeometry	*GetNiGeometry();	// Returns this
	/*1C*/virtual NiTriBasedGeom	*GetTriBasedGeom();	// Returns this
	/*20*/virtual NiTriStrips	*GetTriStrips();	// Returns this
	/*24*/virtual NiTriShape	*GetTriShape();	// Returns this
	/*28*/virtual BSSegmentedTriShape	*GetSegmentedTriShape();	// Returns this
	/*2C*/virtual BSResizableTriShape	*GetResizableTriShape();	// Returns this
	/*30*/virtual NiParticles	*GetNiParticles();	// Returns this
	/*34*/virtual NiLines	*GetNiLines();	// Returns this
	/*38*/virtual bhkNiCollisionObject	*GetCollisionObject();	// Returns this
	/*3C*/virtual bhkBlendCollisionObject	*GetBlendCollisionObject();	// Returns this
	/*40*/virtual bhkRigidBody	*GetRigidBody();	// Returns this
	/*44*/virtual bhkLimitedHingeConstraint	*GetLimitedHingeConstraint();	// Returns this
	/*48*/virtual NiObject	*Clone(NiObjectCopyInfo *copyInfo);
	/*4C*/virtual void		LoadBinary(NiStream *stream);
	/*50*/virtual void		LinkObject(NiStream *stream);
	/*54*/virtual void		RegisterStreamables(NiStream *stream);
	/*58*/virtual void		SaveBinary(NiStream *stream);
	/*5C*/virtual bool		Compare(NiObject *to);
	/*60*/virtual void		GetViewerStrings(NiTArray<char*> *strings);
	/*64*/virtual void		AddViewerStrings(NiTArray<char*> *strings);
	/*68*/virtual void		ProcessClone(NiObjectCopyInfo *copyInfo);
	/*6C*/virtual void		PostLinkObject(NiStream *stream);
	/*70*/virtual bool		StreamCanSkip();
	/*74*/virtual NiRTTI	*GetStreamableRTTI();
	/*78*/virtual void		SetWorldBound(NiBound *worldBound);
	/*7C*/virtual UInt32	GetBlockAllocationSize();
	/*80*/virtual void		Unk_20(void);
	/*84*/virtual void		Unk_21(UInt32 arg);
	/*88*/virtual NiControllerManager	*GetControllerManager();	// Returns this

	NiObject* __fastcall HasBaseType(const NiRTTI *baseType);
};

enum InterpKeyType
{
	kKeyType_Linear =	1,
	kKeyType_Quadratic,
	kKeyType_TBC,
	kKeyType_XYZ,
	kKeyType_Const
};

template <typename T_Data> struct InterpKey
{
	float			time;
	T_Data			value;
};

template <typename T_Data> struct QuadraticKey : public InterpKey<T_Data>
{
	T_Data			forward;
	T_Data			backward;
};

template <typename T_Data> struct TBCKey : public InterpKey<T_Data>
{
	NiVector3		TBC;
};

// 40
struct QuaternionKey
{
	float			time;			// 00
	NiQuaternion	value;			// 04
	NiVector3		TBC;			// 14
	NiQuaternion	quaternion20;	// 20
	NiQuaternion	quaternion30;	// 30
};
static_assert(sizeof(QuaternionKey) == 0x40);

// 2C
class NiTransformData : public NiObject
{
public:
	UInt16					numRotationKeys;	// 08
	UInt16					numTranslationKeys;	// 0A
	UInt16					numScaleKeys;		// 0C
	UInt16					pad0E;				// 0E
	UInt32					rotationKeyType;	// 10
	UInt32					translationKeyType;	// 14
	UInt32					scaleKeyType;		// 18
	UInt8					rotationKeySize;	// 1C
	UInt8					translationKeySize;	// 1D
	UInt8					scaleKeySize;		// 1E
	UInt8					pad1F;				// 1F
	InterpKey<NiQuaternion>	*rotationKeys;		// 20
	InterpKey<NiVector3>	*translationKeys;	// 24
	InterpKey<float>		*scaleKeys;			// 28
};
static_assert(sizeof(NiTransformData) == 0x2C);

// 18
class NiFloatData : public NiObject
{
public:
	UInt32				numKeys;	// 08
	InterpKey<float>	*data;		// 0C
	UInt32				keyType;	// 10
	UInt8				byte14;		// 14
	UInt8				pad15[3];	// 15
};

// 18
class NiColorData : public NiObject
{
public:
	UInt32					numKeys;	// 08
	InterpKey<NiColorAlpha>	*data;		// 0C
	UInt32					keyType;	// 10
	UInt32					unk14;		// 14
};

// 18
class NiMorphData : public NiObject
{
public:
	class MorphTarget : public NiMemObject
	{
    public:
        NiPoint3		*targetVerts;
        NiFixedString	name;
        float			legacyWeight;
        NiInterpolator	*legacyInterp;
    };

    UInt32			numTargets;			// 08
    UInt32			numVertsPerTarget;	// 0C
    MorphTarget		*morphTargets;		// 10
    bool			relativeTargets;	// 14
	UInt8			pad15[3];			// 15
};

// 0C
class NiInterpolator : public NiObject
{
public:
	/*8C*/virtual bool		Update(float time, NiAVObject *target, const NiQuatTransform &transform);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);
	/*B4*/virtual void		Unk_2D(void);
	/*B8*/virtual void		Unk_2E(void);
	/*BC*/virtual void		Unk_2F(void);
	/*C0*/virtual void		Unk_30(void);
	/*C4*/virtual void		Unk_31(float arg1, float arg2);
	/*C8*/virtual void		Unk_32(void);
	/*CC*/virtual void		Unk_33(void);
	/*D0*/virtual void		Unk_34(void);
	/*D4*/virtual void		Unk_35(void);
	/*D8*/virtual void		Unk_36(void);

	float		flt08;		// 08
};

// 0C
class NiKeyBasedInterpolator : public NiInterpolator
{
public:
	/*DC*/virtual void		Unk_37(void);
	/*E0*/virtual void		Unk_38(void);
	/*E4*/virtual void		Unk_39(void);
	/*E8*/virtual void		Unk_3A(void);
	/*EC*/virtual void		Unk_3B(void);
	/*F0*/virtual void		Unk_3C(void);
	/*F4*/virtual void		Unk_3D(void);
};

// 18
class NiFloatInterpolator : public NiKeyBasedInterpolator
{
public:
	float				value;		// 0C
	NiFloatData			*data;		// 10
	UInt32				unk14;		// 14
};

// 24
class NiColorInterpolator : public NiKeyBasedInterpolator
{
public:
	float				value[4];	// 0C
	NiColorData			*data;		// 1C
	UInt32				unk20;		// 20
};

// 48
class NiTransformInterpolator : public NiKeyBasedInterpolator
{
public:
	NiQuatTransform		transformValue;	// 0C
	NiTransformData		*transData;		// 2C
	UInt16				lastTransIdx;	// 30
	UInt16				lastRotIdx;		// 32
	UInt16				lastScaleIdx;	// 34
	UInt8				pad36[2];		// 36
	float				flt38[3];		// 38
	UInt8				byte44;			// 44
	UInt8				pad45[3];		// 45

	__forceinline static NiTransformInterpolator *Create() {return CdeclCall<NiTransformInterpolator*>(0xA403F0);}
};
static_assert(sizeof(NiTransformInterpolator) == 0x48);

// 30
class NiBlendInterpolator : public NiInterpolator
{
public:
	/*DC*/virtual void		Unk_37(void);
	/*E0*/virtual void		Unk_38(void);
	/*E4*/virtual void		Unk_39(void);
	/*E8*/virtual void		Unk_3A(void);

	// 18
	struct InterpArrayItem
	{
		NiTransformInterpolator	*interpolator;
		float					weight;
		float					normalizedWeight;
		UInt8					priority;
		UInt8					pad0D[3];
		float					easeSpinner;
		float					updateTime;
	};

	UInt8			flags;				// 0C
	UInt8			arraySize;			// 0D
	UInt8			interpCount;		// 0E
	UInt8			singleIdx;			// 0F
	UInt8			highPriority;		// 10
	UInt8			nextHighPriority;	// 11
	UInt8			pad12[2];			// 12
	InterpArrayItem	*interpArray;		// 14
	NiInterpolator	*singleInterp;		// 18
	float			weightThreshold;	// 1C
	float			singleTime;			// 20
	float			highSumOfWeights;	// 24
	float			nextHiSumOfWeights;	// 28
	float			highEaseSpinner;	// 2C
};
static_assert(sizeof(NiBlendInterpolator) == 0x30);

class NiBlendTransformInterpolator : public NiBlendInterpolator
{
public:
};

// 74
class NiControllerSequence : public NiObject
{
public:
	/*8C*/virtual bool		Deactivate(float fEaseOutTime, UInt8 bTransition);

	enum State
	{
		kState_Inactive,
		kState_Animating,
		kState_EaseIn,
		kState_EaseOut,
		kState_TransSource,
		kState_TransDest,
		kState_MorphSource
	};

	enum
	{
		kCycle_Loop,
		kCycle_Reverse,
		kCycle_Clamp
	};

	struct ControlledBlock
	{
		NiInterpolator						*interpolator;
		NiMultiTargetTransformController	*multiTargetCtrl;
		NiBlendInterpolator					*blendInterpolator;
		UInt8								blendIdx;
		UInt8								priority;
		UInt8								pad0E[2];
	};

	struct IDTag
	{
		NiFixedString	m_kAVObjectName;
		NiFixedString	m_kPropertyType;
		NiFixedString	m_kCtlrType;
		NiFixedString	m_kCtlrID;
		NiFixedString	m_kInterpolatorID;
	};

	const char				*sequenceName;			// 08
	UInt32					numControlledBlocks;	// 0C
	UInt32					arrayGrowBy;			// 10
	ControlledBlock			**controlledBlocks;		// 14
	IDTag					**idTags;				// 18
	float					weight;					// 1C
	NiTextKeyExtraData		*textKeyData;			// 20
	UInt32					cycleType;				// 24
	float					frequency;				// 28
	float					beginKeyTime;			// 2C
	float					endKeyTime;				// 30
	float					lastTime;				// 34
	float					weightedLastTime;		// 38
	float					lastScaledTime;			// 3C
	NiControllerManager		*manager;				// 40
	UInt32					state;					// 44
	float					offset;					// 48
	float					startTime;				// 4C
	float					endTime;				// 50
	float					destFrame;				// 54
	NiControllerSequence	*partnerSequence;		// 58
	const char				*accumRootName;			// 5C
	NiObject				*object60;				// 60
	UInt32					unk64;					// 64
	UInt16					word68;					// 68
	UInt16					word6A;					// 6A	Pad?
	UInt32					unk6C;					// 6C
	UInt16					word70;					// 70
	UInt8					byte72;					// 72
	UInt8					byte73;					// 73	Pad

	bool Play();
};
static_assert(sizeof(NiControllerSequence) == 0x74);

// 1C
class NiDefaultAVObjectPalette : public NiObject
{
public:
	/*8C*/virtual NiAVObject	*GetObjectByName(NiFixedString *objName);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		SetTarget(NiNode *_target);
	/*98*/virtual NiNode	*GetTarget();

	NiTFixedStringMap<NiAVObject*>	objectsMap;		// 08
	NiNode							*pTarget;		// 18
};

// 34
class NiTimeController : public NiObject
{
public:
	/*8C*/virtual void		Start(float fTime);
	/*90*/virtual void		Stop();
	/*94*/virtual void		Update(const NiUpdateData &updParams);
	/*98*/virtual void		SetTarget(NiObjectNET *pTarget);
	/*9C*/virtual bool		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
	/*A4*/virtual float		ComputeScaledTime(float fTime);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);

	enum TimeCtrlFlags
	{
		kCtrlFlag_AnimType_Pos =			0,
		kCtrlFlag_AnimType_Mask =			1,
		kCtrlFlag_CycleType_Pos =			1,
		kCtrlFlag_CycleType_Mask =			6,
		kCtrlFlag_Active_Mask =				8,
		kCtrlFlag_Direction_Mask =			0x10,
		kCtrlFlag_ManagerControlled_Mask =	0x20,
		kCtrlFlag_ComputeScaledTime_Mask =	0x40,
		kCtrlFlag_ForceUdpate_Mask =		0x80
	};

	UInt16				flags;				// 08
	UInt16				unk0A;				// 0A
	float				frequency;			// 0C
	float				phaseTime;			// 10
	float				lowKeyTime;			// 14
	float				highKeyTime;		// 18
	float				startTime;			// 1C
	float				lastTime;			// 20
	float				weightedLastTime;	// 24
	float				scaledTime;			// 28
	NiObjectNET			*target;			// 2C
	NiTimeController	*nextCtrl;			// 30
};

// 7C
class NiControllerManager : public NiTimeController
{
public:
	/*B4*/virtual void		Unk_2D(void);

	NiTArray<NiControllerSequence*>				sequences;		// 34
	NiSimpleArray<NiControllerSequence*>		seqArray44;		// 44
	NiTStringPointerMap<NiControllerSequence*>	seqStrMap;		// 50	Case-sensitive!
	NiTArray<void*>								*arr64;			// 64	NiTPrimitiveArray<BSAnimNoteListener::BSAnimReceiverType*>
	UInt32										unk68;			// 68
	UInt32										unk6C;			// 6C
	UInt32										unk70;			// 70
	UInt32										unk74;			// 74
	NiDefaultAVObjectPalette					*defObjPlt;		// 78

	NiControllerSequence *FindSequence(const char *seqName);
};
static_assert(sizeof(NiControllerManager) == 0x7C);

// 34
class NiInterpController : public NiTimeController
{
public:
	/*B4*/virtual void		Unk_2D(void);
	/*B8*/virtual void		Unk_2E(void);
	/*BC*/virtual void		Unk_2F(void);
	/*C0*/virtual void		Unk_30(void);
	/*C4*/virtual NiInterpolator	*GetInterpolator(UInt32 arg1);
	/*C8*/virtual void		SetInterpolator(NiInterpolator *pInterpolator, UInt32 arg2);
	/*CC*/virtual void		Unk_33(void);
	/*D0*/virtual void		Unk_34(void);
	/*D4*/virtual void		Unk_35(void);
	/*D8*/virtual void		Unk_36(void);
	/*DC*/virtual void		Unk_37(void);
	/*E0*/virtual void		Unk_38(float arg1, float arg2);
	/*E4*/virtual void		Unk_39(void);
};

// 40
class NiMultiTargetTransformController : public NiInterpController
{
public:
	NiBlendTransformInterpolator	*blendInterp;	// 34
	NiAVObject						**targets;		// 38
	UInt16							numInterps;		// 3C
	UInt8							pad3E[2];		// 3E
};

// 38
class NiSingleInterpController : public NiInterpController
{
public:
	/*E8*/virtual void		Unk_3A(void);

	NiInterpolator		*interpolator;		// 34
};

// 38
class NiBoolInterpController : public NiSingleInterpController
{
public:
	/*EC*/virtual void		Unk_3B(void);
};

// 38
class NiVisController : public NiBoolInterpController
{
public:
};

// 38
class NiTransformController : public NiSingleInterpController
{
public:
	static NiTransformController* __stdcall Create(NiObjectNET *pTarget, NiTransformInterpolator *pInterpolator);
};
static_assert(sizeof(NiTransformController) == 0x38);

// 38
class NiFloatInterpController : public NiSingleInterpController
{
public:
	/*EC*/virtual void		Unk_3B(void);
};

// 38
class NiPoint3InterpController : public NiSingleInterpController
{
public:
	/*EC*/virtual void		Unk_3B(void);
};

// 38
class NiLightDimmerController : public NiFloatInterpController
{
public:
};

// 3C
class NiLightColorController : public NiPoint3InterpController
{
public:
	enum TargetColor
	{
		kColor_Diffuse,
		kColor_Ambient
	};

	UInt16			targetColor;	// 38
	UInt8			pad3A[2];		// 3A
};

// 50
class NiTextureTransformController : public NiFloatInterpController
{
public:
	/*F0*/virtual void		Unk_3C(void);

	enum TextureSlot
	{
		kTexSlot_Diffuse,
		kTexSlot_Dark,
		kTexSlot_Detail,
		kTexSlot_Gloss,
		kTexSlot_Glow,
		kTexSlot_Bump,
		kTexSlot_Normal,
		kTexSlot_Parallax,
		kTexSlot_Decal
	};

	enum OperationType
	{
		kOperation_Translate_U,
		kOperation_Translate_V,
		kOperation_Rotate,
		kOperation_Scale_U,
		kOperation_Scale_V
	};

	UInt32			unk38;			// 38
	UInt32			unk3C;			// 3C
	bool			shaderMap;		// 40
	UInt8			pad41[3];		// 41
	UInt32			textureSlot;	// 44
	UInt32			operationType;	// 48
	UInt32			unk4C;			// 4C
};

// 44
class NiGeomMorpherController : public NiInterpController
{
public:
	/*EC*/virtual void		Unk_3B(void);
	/*E8*/virtual void		Unk_3A(void);

	class InterpItem : public NiMemObject
	{
	public:
		NiInterpolator	*interp;
		float			weight;
	};

	UInt16			gmcFlags;			// 34
	UInt8			pad36[2];			// 36
	NiMorphData		*morphData;			// 38
	InterpItem		*interpItems;		// 3C
	bool			needPreDisplay;		// 40
	bool			ignoreBaseTarget;	// 41
	bool			alwaysUpdate;		// 42
	bool			boundCalculated;	// 43
};

// 5C
class bhkBlendController : public NiTimeController
{
public:
	UInt32						unk34;		// 34
	NiTLargeArray<NiVector3>	blendKeys;	// 38
	float						flt50;		// 50
	float						flt54;		// 54
	UInt32						unk5C;		// 58
};

// 0C
class NiExtraData : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);

	NiFixedString	name;		// 08
};

// 10
class BSXFlags : public NiExtraData
{
public:
	enum BSXFlag
	{
		kBSXFlag_Animated =					1,
		kBSXFlag_Havok =					2,
		kBSXFlag_Ragdoll =					4,
		kBSXFlag_Complex =					8,
		kBSXFlag_Addon =					0x10,
		kBSXFlag_EditorMarker =				0x20,
		kBSXFlag_Dynamic =					0x40,
		kBSXFlag_Articulated =				0x80,
		kBSXFlag_NeedsTransformUpdates =	0x100,
		kBSXFlag_ExternalEmit =				0x200
	};

	UInt32			flags;		// 0C

	__forceinline static BSXFlags *Create() {return CdeclCall<BSXFlags*>(0xC43190);}
};

// 24
class BSBound : public NiExtraData
{
public:
	NiVector3		centre;			// 0C
	NiVector3		dimensions;		// 18
};

// 14
class NiBinaryExtraData : public NiExtraData
{
public:
	UInt32		unk0C;		// 0C
	UInt32		unk10;		// 10
};

// 10
class NiIntegerExtraData : public NiExtraData
{
public:
	SInt32		intData;	// 0C
};

// 10
class NiFloatExtraData : public NiExtraData
{
public:
	float		fltData;	// 0C
};

// 14
class NiFloatsExtraData : public NiExtraData
{
public:
	UInt32		count;		// 0C
	float		*data;		// 10
};

// 10
class NiStringExtraData : public NiExtraData
{
public:
	NiFixedString	strData;	// 0C
};

// 1C
class NiVectorExtraData : public NiExtraData
{
public:
	NiVector4		vec4;		// 0C

	__forceinline static NiVectorExtraData *Create() {return CdeclCall<NiVectorExtraData*>(0xA91FE0);}
};

// 14
class NiTextKeyExtraData : public NiExtraData
{
public:
	struct TextKey
	{
		float			m_fTime;
		NiFixedString	m_kText;
	};

	UInt32		m_uiNumKeys;	// 0C
	TextKey		*m_pKeys;		// 10
};

// 14
class TileExtra : public NiExtraData
{
public:
	Tile		*parentTile;	// 0C
	NiNode		*parentNode;	// 10
};

// 1C0
class BSFaceGenAnimationData : public NiExtraData
{
public:
};

// 08
class BSTextureSet : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);

	enum TextureSlots
	{
		kSlot_Diffuse,
		kSlot_Normal,
		kSlot_Glow,
		kSlot_Parallax,
		kSlot_CubeMap,
		kSlot_EnvMask
	};
};

// 38
class BSShaderTextureSet : public BSTextureSet
{
public:
	String		textures[6];	// 08
};

// 0C
class BSMultiBoundShape : public NiObject
{
public:
	/*8C*/virtual UInt32	GetBoundType();
	/*90*/virtual float	GetRadius();
	/*94*/virtual int	CheckBSBound(BSMultiBound *targetBound);
	/*98*/virtual int	CheckBound(NiBound *targetBound);
	/*9C*/virtual bool	WithinFrustum(NiFrustumPlanes *planes);
	/*A0*/virtual bool	CompletelyWithinFrustum(NiFrustumPlanes *planes);
	/*A4*/virtual void	Unk_29(void);
	/*A8*/virtual void	Unk_2A(void);
	/*AC*/virtual void	Unk_2B(void);
	/*B0*/virtual void	Unk_2C(void);
	/*B4*/virtual bool	IsPointWithinRadius(const NiVector3 &inPos);
	/*B8*/virtual NiVector3	*GetShapeOrigin(NiVector3 *out);

	UInt32		cullResult;	// 08
};

// 1C
class BSMultiBoundSphere : public BSMultiBoundShape
{
public:
	NiBound		sphere;		// 0C
};

// 24
class BSMultiBoundAABB : public BSMultiBoundShape
{
public:
	NiVector3		diagonalPt0;// 0C
	NiVector3		diagonalPt1;// 18
};

// 18
class NiObjectNET : public NiObject
{
public:
	NiFixedString		m_blockName;				// 08
	NiTimeController	*m_controller;				// 0C
	NiExtraData			**m_extraDataList;			// 10
	UInt16				m_extraDataListLen;			// 14
	UInt16				m_extraDataListCapacity;	// 16

	const char *GetName() const {return m_blockName.Get();}
	void __fastcall SetName(const char *newName);
	NiExtraData* __fastcall GetExtraData(UInt32 vtbl) const;
	__forceinline bool AddExtraData(NiExtraData *xData)
	{
		return ThisCall<bool>(0xA5BA40, this, xData);
	}
	void DumpExtraData();
};

// 18
class NiProperty : public NiObjectNET
{
public:
	/*8C*/virtual UInt32	GetPropertyType();
	/*90*/virtual void		UpdateController(const NiUpdateData &updParams);

	enum PropertyType
	{
		kPropertyType_Alpha =			0,
		kPropertyType_Culling =			1,
		kPropertyType_Material =		2,
		kPropertyType_Shader =			3,
		kPropertyType_Stencil =			4,
		kPropertyType_Texturing =		5,
		kPropertyType_Dither =			8,
		kPropertyType_Specular =		9,
		kPropertyType_VertexColor =		10,
		kPropertyType_ZBuffer =			11,
		kPropertyType_Fog =				13,
	};
};

// 4C
class NiMaterialProperty : public NiProperty
{
public:
	UInt32				m_iIndex;		// 18
	NiColor				specularRGB;	// 1C
	NiColor				emissiveRGB;	// 28
	UInt32				isExternalEmit;	// 34
	float				glossiness;		// 38
	float				alpha;			// 3C
	float				emitMult;		// 40
	UInt32				m_uiRevID;		// 44
	UInt32				unk48;			// 48

	__forceinline static NiMaterialProperty *Create() {return CdeclCall<NiMaterialProperty*>(0xA756D0);}
	void __vectorcall SetTraitValue(UInt32 traitID, float value);
};

// 1C
class NiAlphaProperty : public NiProperty
{
public:
	enum AlphaFlag
	{
		kFlag_EnableBlending =		1 << 0,
		kFlag_SourceInverse =		1 << 1,
		kFlag_SourceSrcColor =		1 << 2,
		kFlag_SourceDestColor =		1 << 3,
		kFlag_SourceSrcAlpha =		kFlag_SourceSrcColor | kFlag_SourceDestColor,
		kFlag_SourceDestAlpha =		1 << 4,
		kFlag_SourceAlphaSaturate =	kFlag_SourceSrcColor | kFlag_SourceDestAlpha,
		kFlag_DestinInverse =		1 << 5,
		kFlag_DestinSrcColor =		1 << 6,
		kFlag_DestinDestColor =		1 << 7,
		kFlag_DestinSrcAlpha =		kFlag_DestinSrcColor | kFlag_DestinDestColor,
		kFlag_DestinDestAlpha =		1 << 8,
		kFlag_DestinAlphaSaturate =	kFlag_DestinSrcColor | kFlag_DestinDestAlpha,
		kFlag_EnableTesting =		1 << 9,
		kFlag_TestFuncLess =		1 << 10,
		kFlag_TestFuncEqual =		1 << 11,
		kFlag_TestFuncLessOrEq =	kFlag_TestFuncLess | kFlag_TestFuncEqual,
		kFlag_TestFuncGreater =		1 << 12,
		kFlag_TestFuncNotEqual =	kFlag_TestFuncLess | kFlag_TestFuncGreater,
		kFlag_TestFuncGrtOrEq =		kFlag_TestFuncEqual | kFlag_TestFuncGreater,
		kFlag_TestFuncNever =		kFlag_TestFuncLess | kFlag_TestFuncEqual | kFlag_TestFuncGreater,
		kFlag_NoSorter =			1 << 13,
	};

	UInt16				flags;		// 18
	UInt8				threshold;	// 1A
	UInt8				byte1B;		// 1B

	__forceinline static NiAlphaProperty *Create() {return CdeclCall<NiAlphaProperty*>(0xA5CEB0);}
};
extern NiAlphaProperty *s_alphaProperty;

// 48
struct TextureTransform
{
	NiPoint2		translation;	// 00
	float			rotation;		// 08
	NiPoint2		scale;			// 0C
	NiPoint2		centre;			// 14
	UInt8			byte1C;			// 1C
	UInt8			pad1D[3];		// 1D
	NiMatrix33		matrix20;		// 20
	UInt32			unk44;			// 44
};
static_assert(sizeof(TextureTransform) == 0x48);

// 30
class NiTexturingProperty : public NiProperty
{
public:
	class Map
	{
	public:
		virtual void	Destroy(bool doFree);
		virtual void	Unk_01(void);
		virtual void	Unk_02(void);
		virtual void	Unk_03(void);

		Map					*baseMap;		// 04
		NiSourceTexture		*srcTexture;	// 08
		TextureTransform	*transform;		// 0C
	};

	class ShaderMap : public Map
	{
	public:
		UInt32				mapID;		// 10
	};

	UInt16					flags;				// 18
	UInt8					pad1A[2];			// 1A
	NiTArray<Map*>			textures;			// 1C
	NiTArray<ShaderMap*>	*shaderTextures;	// 2C
};
static_assert(sizeof(NiTexturingProperty) == 0x30);

// 24
class NiStencilProperty : public NiProperty
{
public:
	UInt16				flags;		// 18
	UInt16				word1A;		// 1A
	UInt32				unk1C;		// 1C
	UInt32				mask;		// 20

	__forceinline static NiStencilProperty *Create() {return CdeclCall<NiStencilProperty*>(0xA6F410);}
};
static_assert(sizeof(NiStencilProperty) == 0x24);

// 1C
class NiCullingProperty : public NiProperty
{
public:
	UInt32				unk18;		// 18
};

class LightingData;

// 10
struct BSRenderPass
{
	void			*geometry;		// 00
	UInt16			passEnum;		// 04
	UInt8			accumHint;		// 06
	UInt8			bEnabled;		// 07
	UInt8			noFog;			// 08
	UInt8			numLights;		// 09
	UInt8			maxNumLights;	// 0A
	UInt8			currLandTexture;// 0B
	LightingData	**sceneLights;	// 0C
};

enum LightingPassFlags
{
	kPass_Ambient =			1,
	kPass_Diffuse =			2,
	kPass_Texture =			4,
	kPass_Specular =		8,
	kPass_Shadow =			0x10,
	kPass_CanopyShadow =	0x20,
	kPass_Opt =				0x100
};

// 14	vtbl 0x10B8480
class BSRenderPassList
{
public:
	virtual void	Destroy(bool doFree);
	
	BSRenderPass	**passes;	// 04
	UInt16			maxSize;	// 08
	UInt16			word0A;		// 0A
	UInt16			arraySize;	// 0C
	UInt16			growBy;		// 0E
	UInt32			passCount;	// 10
};

// 60
class BSShaderProperty : public NiProperty
{
public:
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(UInt32 arg1);
	/*9C*/virtual void		Unk_27(UInt32 arg1);
	/*A0*/virtual void		Unk_28(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	/*A4*/virtual void		Unk_29(UInt32 arg1);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(UInt32 arg1);
	/*B0*/virtual void		Unk_2C(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	/*B4*/virtual void		Unk_2D(void);
	/*B8*/virtual void		Unk_2E(UInt32 arg1);
	/*BC*/virtual void		Unk_2F(UInt32 arg1, UInt32 arg2);
	/*C0*/virtual void		Unk_30(void);

	enum ShaderType
	{
		kType_Lighting =			1,
		kType_DistantLOD =			2,
		kType_GeometryDecal =		3,
		kType_TallGrass =			4,
		kType_SpeedTreeLeaf =		6,
		kType_PPLighting =			8,
		kType_Hair =				9,
		kType_SpeedTreeBranch =		0xA,
		kType_SpeedTreeBillboard =	0xB,
		kType_Lighting30 =			0xC,
		kType_Sky =					0xD,
		kType_Water =				0xE,
		kType_Bolt =				0xF,
		kType_Particle =			0x11,
		kType_Precipitation =		0x12,
		kType_Tile =				0x13,
		kType_NoLighting =			0x15,
		kType_VolumetricFog =		0x16,
		kType_BloodSplatter =		0x17,
		kType_DistantTree =			0x18
	};

	enum ShaderFlag
	{
		kFlag1_Specular =					1,
		kFlag1_Skinned =					2,
		kFlag1_LowDetail =					4,
		kFlag1_VertexAlpha =				8,
		kFlag1_MotionBlur =					0x10,
		kFlag1_SinglePass =					0x20,
		kFlag1_Empty =						0x40,
		kFlag1_EnvironmentMapping =			0x80,
		kFlag1_AlphaTexture =				0x100,
		kFlag1_ZPrepass =					0x200,
		kFlag1_Facegen =					0x400,
		kFlag1_Parallax =					0x800,
		kFlag1_ModelSpaceNormals =			0x1000,
		kFlag1_NonProjectiveShadows =		0x2000,
		kFlag1_Landscape =					0x4000,
		kFlag1_Refraction =					0x8000,
		kFlag1_FireRefraction =				0x10000,
		kFlag1_EyeEnvironmentMapping =		0x20000,
		kFlag1_Hair =						0x40000,
		kFlag1_DynamicAlpha =				0x80000,
		kFlag1_LocalMapHideSecret =			0x100000,
		kFlag1_WindowEnvironmentMapping =	0x200000,
		kFlag1_TreeBillboard =				0x400000,
		kFlag1_ShadowFrustum =				0x800000,
		kFlag1_MultipleTextures =			0x1000000,
		kFlag1_RemappableTextures =			0x2000000,
		kFlag1_Decal =						0x4000000,
		kFlag1_DynamicDecal =				0x8000000,
		kFlag1_ParallaxOcclusion =			0x10000000,
		kFlag1_ExternalEmittance =			0x20000000,
		kFlag1_Shadowmap =					0x40000000,
		kFlag1_ZbufferTest =				0x80000000,
		
		kFlag2_ZbufferWrite =				1,
		kFlag2_LODLandscape =				2,
		kFlag2_LODBuilding =				4,
		kFlag2_NoFade =						8,
		kFlag2_RefractionTint =				0x10,
		kFlag2_VertexColors =				0x20,
		kFlag2_1stPerson =					0x40,
		kFlag2_1stLightIsPointLight =		0x80,
		kFlag2_2ndLight =					0x100,
		kFlag2_3rdLight =					0x200,
		kFlag2_VertexLighting =				0x400,
		kFlag2_UniformScale =				0x800,
		kFlag2_FitSlope =					0x1000,
		kFlag2_Billboard =					0x2000,
		kFlag2_NoLODLandBlend =				0x4000,
		kFlag2_EnvmapLightFade =			0x8000,
		kFlag2_Wireframe =					0x10000,
		kFlag2_VatsSelection =				0x20000,
		kFlag2_ShowInLocalMap =				0x40000,
		kFlag2_PremultAlpha =				0x80000,
		kFlag2_SkipNormalMaps =				0x100000,
		kFlag2_AlphaDecal =					0x200000,
		kFlag2_NoTransparencyMultisampling =0x400000,
		kFlag2_StingerProp =				0x800000
	};

	UInt16				unk18;				// 18
	UInt16				unk1A;				// 1A
	UInt32				shaderType;			// 1C
	UInt32				flags1;				// 20
	UInt32				flags2;				// 24
	float				alpha;				// 28
	float				fadeAlpha;			// 2C
	float				envMapScale;		// 30
	float				LODFade;			// 34
	UInt32				lastRenderPassState;// 38
	BSRenderPassList	*renderPassLists[7];// 3C
	UInt32				shaderIndex;		// 58
	float				depthBias;			// 5C
};
static_assert(sizeof(BSShaderProperty) == 0x60);

// 80
class BSShaderNoLightingProperty : public BSShaderProperty
{
public:
	/*C4*/virtual void		Unk_31(void);
	/*C8*/virtual void		Unk_32(void);
	/*CC*/virtual void		Unk_33(void);
	/*D0*/virtual void		Unk_34(void);

	NiTexture		*srcTexture;			// 60
	const char		*texturePath;			// 64
	UInt16			word68;					// 68
	UInt16			word6A;					// 6A
	UInt32			unk6C;					// 6C
	float			falloffStartAngle;		// 70
	float			falloffStopAngle;		// 74
	float			falloffStartOpacity;	// 78
	float			falloffStopOpacity;		// 7C

	__forceinline static BSShaderNoLightingProperty *Create() {return ThisCall<BSShaderNoLightingProperty*>(0xB6FC90, CdeclCall<void*>(0xAA13E0, sizeof(BSShaderNoLightingProperty)));}
};
static_assert(sizeof(BSShaderNoLightingProperty) == 0x80);

// 7C
class BSShaderLightingProperty : public BSShaderProperty
{
public:
	/*C4*/virtual void		Unk_31(void);
	/*C8*/virtual void		Unk_32(void);

	DList<LightingData>			illuminatingLights;	// 60
	float						flt6C;				// 6C
	UInt32						unk70;				// 70
	bool						lightListChanged;	// 74
	UInt8						pad75[3];			// 75
	DList<LightingData>::Node	*lastLight;			// 78
};
static_assert(sizeof(BSShaderLightingProperty) == 0x7C);

// 104
class BSShaderPPLightingProperty : public BSShaderLightingProperty
{
public:
	/*0CC*/virtual void		SetupGeometry(void*);
	/*0D0*/virtual void		CopyProperties(BSShaderPPLightingProperty *from);
	/*0D4*/virtual void		Unk_35(void);
	/*0D8*/virtual void		Unk_36(void);
	/*0DC*/virtual void		PickClampMode(UInt32 mode);
	/*0E0*/virtual UInt32	GetClampMode();
	/*0E4*/virtual void		SetClampMode(UInt32 mode);
	/*0E8*/virtual void		Unk_3A(void);
	/*0EC*/virtual void		Unk_3B(void);
	/*0F0*/virtual void		Unk_3C(void);
	/*0F4*/virtual void		Unk_3D(void);
	/*0F8*/virtual void		Unk_3E(void);
	/*0FC*/virtual void		Unk_3F(void);
	/*100*/virtual void		Unk_40(void);
	/*104*/virtual void		Unk_41(void);
	/*108*/virtual void		Unk_42(void);
	/*10C*/virtual void		Unk_43(void);
	/*110*/virtual void		Unk_44(void);
	/*114*/virtual void		Unk_45(void);
	/*118*/virtual void		Unk_46(void);
	/*11C*/virtual void		Unk_47(void);

	UInt32				unk07C;					// 07C
	UInt32				unk080;					// 080
	NiColorAlpha		hairTint;				// 084
	NiColorAlpha		color094;				// 094
	BSShaderTextureSet	*textureSet;			// 0A4
	UInt16				word0A8;				// 0A8
	UInt16				word0AA;				// 0AA
	NiTexture			**srcTextures[6];		// 0AC
	void				*ptr0C4;				// 0C4
	UInt32				unk0C8;					// 0C8
	void				*ptr0CC;				// 0CC
	NiObject			*object0D0;				// 0D0
	UInt32				unk0D4;					// 0D4
	UInt32				clampMode;				// 0D8
	NiObject			*object0DC;				// 0DC
	float				refractionStrength;		// 0E0
	SInt32				refractionFirePeriod;	// 0E4
	float				parallaxMaxPasses;		// 0E8
	float				parallaxScale;			// 0EC
	NiColorAlpha		emmisiveColor;			// 0F0
	BSRenderPass		*depthPass;				// 100
};
static_assert(sizeof(BSShaderPPLightingProperty) == 0x104);

// 150
class WaterShaderProperty : public BSShaderProperty
{
public:
	bool				isWading;			// 060
	UInt8				byte061;			// 061
	bool				renderAtWaterLevel;	// 062
	bool				bDepth;				// 063
	UInt32				unk064;				// 064
	UInt32				unk068;				// 068
	NiPoint2			blendRadius;		// 06C
	float				fogDistanceFar;		// 074
	float				fogDistanceNear;	// 078
	UInt8				byte07C;			// 07C
	UInt8				byte07D;			// 07D
	bool				isUnderwater;		// 07E
	UInt8				useDefaultWater;	// 07F
	UInt8				bReflections;		// 080
	UInt8				bRefractions;		// 081
	UInt8				bTextureCoords;		// 082
	UInt8				byte083;			// 083
	UInt32				stencilMask;		// 084
	NiColorAlpha		shallowColor;		// 088
	NiColorAlpha		deepColor;			// 098
	NiColorAlpha		reflectionColor;	// 0A8
	float				sunPower;			// 0B8
	float				reflectivity;		// 0BC
	float				opacity;			// 0C0
	float				distortion;			// 0C4
	float				flt0C8;				// 0C8
	float				flt0CC;				// 0CC
	float				lightRadius;		// 0D0
	float				lightBrightness;	// 0D4
	float				depthFalloffStart;	// 0D8
	float				depthFalloffEnd;	// 0DC
	float				flt0E0;				// 0E0
	float				flt0E4;				// 0E4
	float				flt0E8;				// 0E8
	float				flt0EC;				// 0EC
	float				flt0F0;				// 0F0
	float				flt0F4;				// 0F4
	float				flt0F8;				// 0F8
	float				flt0FC;				// 0FC
	float				shininess;			// 100
	float				reflectionHDRMult;	// 104
	float				flt108;				// 108
	float				flt10C;				// 10C
	float				flt110;				// 110
	float				flt114;				// 114
	float				waterFresnelTerm;	// 118
	float				noiseScale;			// 11C
	float				fog;				// 120
	float				texScale;			// 124
	UInt32				unk128[3];			// 128
	NiSourceTexture		*noiseTexture;		// 134
	BSRenderedTexture	*noDepth;			// 138
	BSRenderedTexture	*reflections;		// 13C
	BSRenderedTexture	*refractions;		// 140
	BSRenderedTexture	*depth;				// 144
	BSRenderedTexture	*wading;			// 148
	BSRenderPass		*stencilPass;		// 14C
};
static_assert(sizeof(WaterShaderProperty) == 0x150);

// 90
class SkyShaderProperty : public BSShaderProperty
{
public:
	/*C4*/virtual void		Unk_31(void);
	/*C8*/virtual void		Unk_32(void);
	/*CC*/virtual void		Unk_33(void);
	/*D0*/virtual void		Unk_34(void);

	NiColorAlpha		color;			// 60
	NiSourceTexture		*srcTexture70;	// 70
	String				filePath;		// 74
	UInt32				unk7C;			// 7C
	NiSourceTexture		*srcTexture80;	// 80
	float				flt84;			// 84	Maybe texture % for animation?
	UInt32				unk88;			// 88
	UInt32				unk8C;			// 8C
};
static_assert(sizeof(SkyShaderProperty) == 0x90);

// B0
class TileShaderProperty : public BSShaderProperty
{
public:
	NiTexture			*srcTexture;	// 60
	NiTexture			*alphaTexture;	// 64
	NiColorAlpha		overlayColor;	// 68
	float				alpha;			// 78
	NiPoint2			textureOrigin;	// 7C	X/Y: [0.0, 1.0)
	NiPoint2			texturePercent;	// 84
	UInt32				unk8C;			// 8C
	UInt8				byte90;			// 90
	bool				textureMorph;	// 91	Applies textureOrigin & texturePercent
	bool				hasVtxColors;	// 92
	UInt8				byte93;			// 93
	void				*ptr94;			// 94
	UInt16				word98;			// 98
	UInt16				word9A;			// 9A
	int					minX;			// 9C	min/max values used for clipping
	int					minY;			// A0
	int					maxX;			// A4
	int					maxY;			// A8
	UInt8				byteAC;			// AC
	UInt8				padAD[3];		// AD
};
static_assert(sizeof(TileShaderProperty) == 0xB0);

// 1C
struct GeometryProperties
{
	NiAlphaProperty			*alphaProp;		// 00
	NiCullingProperty		*cullingProp;	// 04
	NiMaterialProperty		*materialProp;	// 08
	BSShaderProperty		*shaderProp;	// 0C
	NiStencilProperty		*stencilProp;	// 10
	NiTexturingProperty		*texturingProp;	// 14
	UInt32					unk18;			// 18
};

// 9C
class NiAVObject : public NiObjectNET
{
public:
	/*8C*/virtual void		UpdateControllers(const NiUpdateData &updParams);
	/*90*/virtual void		ApplyTransform(NiMatrix33 *arg1, NiVector3 *arg2, bool arg3);
	/*94*/virtual void		Unk_25(UInt32 arg);
	/*98*/virtual void		Unk_26(UInt32 arg);
	/*9C*/virtual NiAVObject	*GetObjectByName(NiFixedString *objName);
	/*A0*/virtual void		SetSelectiveUpdateFlags(UInt8 *bSelectiveUpdate, UInt32 bSelectiveUpdateTransform, UInt8 *bRigid);
	/*A4*/virtual void		UpdateDownwardPass(const NiUpdateData &updParams, UInt32 flags);
	/*A8*/virtual void		UpdateSelectedDownwardPass(const NiUpdateData &updParams, UInt32 flags);
	/*AC*/virtual void		UpdateRigidDownwardPass(const NiUpdateData &updParams, UInt32 flags);
	/*B0*/virtual void		Unk_2C(const GeometryProperties &properties);
	/*B4*/virtual void		Unk_2D(UInt32 arg);
	/*B8*/virtual void		UpdateWorldData(const NiUpdateData &updParams);
	/*BC*/virtual void		UpdateWorldBound();
	/*C0*/virtual void		UpdateTransformAndBounds(const NiUpdateData &updParams);
	/*C4*/virtual void		PreAttachUpdate(NiNode *newParent, const NiUpdateData &updParams);
	/*C8*/virtual void		PreAttachUpdateProperties(NiNode *newParent);
	/*CC*/virtual void		DetachParent(UInt32 arg);
	/*D0*/virtual void		UpdateUpwardPassParent();
	/*D4*/virtual void		OnVisible(NiCullingProcess *culling);
	/*D8*/virtual void		PurgeRendererData(NiDX9Renderer *renderer);

	enum NiFlags
	{
		kNiFlag_Hidden =					0x00000001,
		kNiFlag_SelectiveUpdate =			0x00000002,
		kNiFlag_SelUpdTransforms =			0x00000004,
		kNiFlag_SelUpdController =			0x00000008,
		kNiFlag_SelUpdRigid =				0x00000010,
		kNiFlag_DisplayObject =				0x00000020,
		kNiFlag_DisableSorting =			0x00000040,
		kNiFlag_SelUpdTransformsOverride =	0x00000080,
		kNiFlag_UnkBit8 =					0x00000100,
		kNiFlag_SaveExternalGeomData =		0x00000200,
		kNiFlag_NoDecals =					0x00000400,
		kNiFlag_AlwaysDraw =				0x00000800,
		kNiFlag_MeshLOD =					0x00001000,
		kNiFlag_FixedBound =				0x00002000,
		kNiFlag_TopFadeNode =				0x00004000,
		kNiFlag_IgnoreFade =				0x00008000,
		kNiFlag_NoAnimSyncX =				0x00010000,
		kNiFlag_NoAnimSyncY =				0x00020000,
		kNiFlag_NoAnimSyncZ =				0x00040000,
		kNiFlag_NoAnimSyncS =				0x00080000,
		kNiFlag_Culled =					0x00100000,
		kNiFlag_NoDismemberValidity =		0x00200000,
		kNiFlag_RenderUse =					0x00400000,
		kNiFlag_MaterialsApplied =			0x00800000,
		kNiFlag_HighDetail =				0x01000000,
		kNiFlag_ForceUpdate =				0x02000000,
		kNiFlag_PreProcessedNode =			0x04000000,
		kNiFlag_UnkBit27 =					0x08000000,
		kNiFlag_UnkBit28 =					0x10000000,
		kNiFlag_IsPointLight =				0x20000000,	//	JIP only
		kNiFlag_DoneInitLights =			0x40000000,	//	JIP only
		kNiFlag_IsInserted =				0x80000000	//	JIP only
	};

	NiNode					*m_parent;				// 18
	bhkNiCollisionObject	*m_collisionObject;		// 1C
	NiBound					*m_kWorldBound;			// 20
	DList<NiProperty>		m_propertyList;			// 24
	UInt32					m_flags;				// 30
	NiTransform				m_transformLocal;		// 34
	NiTransform				m_transformWorld;		// 68

	inline NiMatrix33& LocalRotate() {return m_transformLocal.rotate;}
	inline NiMatrix33& WorldRotate() {return m_transformWorld.rotate;}
	inline NiVector3& LocalTranslate() {return m_transformLocal.translate;}
	inline NiVector3& WorldTranslate() {return m_transformWorld.translate;}

	inline void SetHidden(bool hide)
	{
		UBYT(m_flags) &= ~1;
		UBYT(m_flags) |= (UInt8)hide;
	}
	inline void Hide() {UBYT(m_flags) |= 1;}
	inline void Show() {UBYT(m_flags) &= ~1;}

	NiAVObject *CreateCopy();
	void Update();
	UInt32 GetIndex() const;
	bool ReplaceObject(NiAVObject *object);
	TESObjectREFR *GetParentRef() const;
	void AssignGeometryProps();

	void ExportToFile(const char *filePath);

	void Dump(UInt8 dumpFlags = 0xF);
	void DumpParents();
};

// AC
class NiNode : public NiAVObject
{
public:
	/*DC*/virtual void		AddObject(NiAVObject *object, bool firstFree);
	/*E0*/virtual void		AddObjectAt(UInt32 index, NiAVObject *object);
	/*E4*/virtual void		RemoveObject2(NiAVObject *toRemove, NiAVObject **arg2);
	/*E8*/virtual void		RemoveObject(NiAVObject *toRemove);		//	Calls RemoveObject2 with arg2 as ptr to NULL
	/*EC*/virtual void		RemoveNthObject2(UInt32 index, NiAVObject **arg2);
	/*F0*/virtual void		RemoveNthObject(UInt32 index);			//	Calls RemoveNthObject2 with arg2 as ptr to NULL
	/*F4*/virtual void		ReplaceNthObject2(UInt32 index, NiAVObject *replaceWith, NiAVObject **arg3);
	/*F8*/virtual void		ReplaceNthObject(UInt32 index, NiAVObject *replaceWith);	//	Calls ReplaceNthObject2 with arg3 as ptr to NULL
	/*FC*/virtual void		UpdateUpwardPass();

	NiTArray<NiAVObject*>	m_children;		// 9C

	static NiNode* __stdcall Create(const char *nameStr);		//	str of NiFixedString
	NiAVObject* __fastcall GetBlockByName(const char *nameStr) const;	//	str of NiFixedString
	NiAVObject* __fastcall GetBlock(const char *blockName) const;
	NiNode* __fastcall GetNode(const char *nodeName) const;
	NiAVObject* __fastcall FindBlockOfType(UInt32 typeVtbl) const;
	bool IsMovable() const;
	void __fastcall ToggleCollision(UInt8 flag);
	void ResetCollision();
	void RemoveCollision();
	void __vectorcall SetAlphaRecurse(float alpha);
	void ResetShaderRenderPass();
	UInt32 GetBSXFlags() const;
	void AddPropertyRecurse(NiProperty *niProperty);
	void AddNoLightingPropertyRecurse();
	void __vectorcall SetMaterialPropValueRecurse(UInt32 traitID, float value);
	void __fastcall SetCollisionPropRecurse(UInt32 propID, FltAndInt value);
	void __fastcall SetCollisionLayer(UInt32 layerType);
	void GetContactObjects(ContactObjects &contactObjs);
	bool HasPhantom();
	float __vectorcall GetBodyMass(float totalMass) const;
	void ApplyForce(const NiVector4 &forceVector);
};
static_assert(sizeof(NiNode) == 0xAC);

// E4
class BSFadeNode : public NiNode
{
public:
	enum FadeType
	{
		kFade_Object =			1,
		kFade_Item =			2,
		kFade_Actor =			3,
		kFade_Unknown6 =		6,
		kFade_Unknown7 =		7,
		kFade_Unknown8 =		8,
		kFade_LODFadeOutMax =	0xA,
	};

	float			nearDistSqr;	// AC
	float			farDistSqr;		// B0
	float			lastFade;		// B4
	float			currentFade;	// B8	[0.0, 1.0]; Used for fade-in/out
	float			boundRadius;	// BC
	float			timeSinceUpdate;// C0
	UInt32			fadeType;		// C4
	UInt32			frameCounter;	// C8
	TESObjectREFR	*linkedObj;		// CC
	UInt32			unkD0[5];		// D0

	__forceinline static BSFadeNode *Create() {return ThisCall<BSFadeNode*>(0xB4E150, CdeclCall<void*>(0xAA13E0, sizeof(BSFadeNode)));}

	void __fastcall SetVisible(bool visible);
};
static_assert(sizeof(BSFadeNode) == 0xE4);

// F8
class BSTreeNode : public BSFadeNode
{
public:
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual void	Unk_4A(void);
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);

	UInt32			unkE8[5];		// E8
};
static_assert(sizeof(BSTreeNode) == 0xF8);

// 10
class BSMultiBound : public NiObject
{
public:
	/*8C*/virtual void	Unk_23(void);
	/*90*/virtual void	Unk_24(void);

	UInt32				unk08;		// 08
	BSMultiBoundShape	*mbShape;	// 0C
};

// B4
class BSMultiBoundNode : public NiNode
{
public:
	/*100*/virtual void		Unk_40(UInt32 arg1, UInt32 arg2);
	/*104*/virtual void		Unk_41(void);
	/*108*/virtual void		Unk_42(UInt32 arg1);
	/*10C*/virtual void		Unk_43(UInt32 arg1);
	/*110*/virtual void		Unk_44(UInt32 arg1);

	BSMultiBound	*multiBound;	// AC
	UInt32			cullingMode;	// B0
};

// B8
class BSParticleSystemManager : public NiNode
{
public:
	/*100*/virtual void		Unk_40(void);

	UInt32			*pcmArray;		// AC
	UInt32			arraySize;		// B0
	void			*ptrB4;			// B4

	__forceinline static BSParticleSystemManager *GetSingleton() {return *(BSParticleSystemManager**)0x11DED58;}
};

// B4
class NiBillboardNode : public NiNode
{
public:
	/*100*/virtual void		Unk_40(void);

	enum BillboardMode
	{
		ALWAYS_FACE_CAMERA,
		ROTATE_ABOUT_UP,
		RIGID_FACE_CAMERA,
		ALWAYS_FACE_CENTER,
		RIGID_FACE_CENTER,
		BSROTATE_ABOUT_UP
	};

	UInt16			billboardMode;	// AC
	UInt16			wordAE;			// AE
	float			savedTime;		// B0
	
	__forceinline void RotateToCamera(NiCamera *camera) {ThisCall(0xA7DE40, this, camera);}
};

// F0
class BSFaceGenNiNode : public NiNode
{
public:
	/*100*/virtual BSFaceGenAnimationData	*GetAnimData();
	/*104*/virtual void		SetAnimData(BSFaceGenAnimationData *inAnimData);
	/*108*/virtual NiMatrix33	*GetRotation(NiMatrix33 *outMat);
	/*10C*/virtual void		SetRotation(NiMatrix33 *inMat);
	/*110*/virtual bool		GetAnimationUpdate();
	/*114*/virtual void		SetAnimationUpdate(bool arg);
	/*118*/virtual bool		GetApplyRotToParent();
	/*11C*/virtual void		SetApplyRotToParent(bool doSet);
	/*120*/virtual bool		GetFixedNormals();
	/*124*/virtual void		SetFixedNormals(bool doSet);
	/*128*/virtual void		Unk_4A(void *arg);

	BSFaceGenAnimationData	*animData;			// AC
	NiMatrix33				rotation;			// B0
	UInt8					forceBaseMorph;		// D4
	UInt8					fixedNormals;		// D5
	UInt8					animationUpdate;	// D6
	UInt8					rotatedLastUpdate;	// D7
	UInt8					applyRotToParent;	// D8
	UInt8					padD9[3];			// D9
	float					lastTime;			// DC
	UInt8					usingLoResHead;		// E0
	UInt8					isThePlayer;		// E1
	UInt8					isInDialouge;		// E2
	UInt8					padE3;				// E3
	UInt32					unkE4;				// E4
	UInt32					unkE8;				// E8
	UInt8					byteEC;				// EC
	UInt8					padED[3];			// ED
};
static_assert(sizeof(BSFaceGenNiNode) == 0xF0);

// B0
class BSTempNode : public NiNode
{
public:
	float			secondsLeft;// AC
};

// B0
class BSTempNodeManager : public NiNode
{
public:
	float			lastTime;	// AC
};

// B8
class BSClearZNode : public NiNode
{
public:
	UInt8			byteAC;		// AC
	UInt8			byteAD;		// AD
	UInt8			padAE[2];	// AE
	float			fltB0;		// B0
	float			fltB4;		// B4

	__forceinline static BSClearZNode *GetSingleton() {return *(BSClearZNode**)0x11DEA14;}
};

// 2C
class NiFogProperty : public NiProperty
{
public:
	UInt16			flags;			// 18
	UInt16			unk1A;			// 1A
	float			depth;			// 1C
	NiColor			color;			// 20
};

// 64
class BSFogProperty : public NiFogProperty
{
public:
	float			startDistance;	// 2C
	float			endDistance;	// 30
	float			waterStartDist;	// 34
	float			waterEndDist;	// 38
	NiPlane			plane3C;		// 3C
	float			falloff;		// 4C
	float			height;			// 50
	NiColor			waterColor;		// 54
	float			power;			// 60
};
static_assert(sizeof(BSFogProperty) == 0x64);

// 14
class NiShader : public NiRefObject
{
public:
	/*08*/virtual void		Unk_02(void);
	/*0C*/virtual void		Unk_03(void);
	/*10*/virtual void		Unk_04(void);
	/*14*/virtual void		Unk_05(void);
	/*18*/virtual void		Unk_06(void);
	/*1C*/virtual void		Unk_07(void);
	/*20*/virtual void		Unk_08(void);
	/*24*/virtual void		Unk_09(void);
	/*28*/virtual void		Unk_0A(void);
	/*2C*/virtual void		Unk_0B(void);
	/*30*/virtual void		Unk_0C(void);
	/*34*/virtual void		Unk_0D(void);
	/*38*/virtual void		Unk_0E(void);
	/*3C*/virtual void		Unk_0F(void);
	/*40*/virtual void		Unk_10(void);
	/*44*/virtual void		Unk_11(void);
	/*48*/virtual void		Unk_12(void);
	/*4C*/virtual void		Unk_13(void);
	/*50*/virtual void		Unk_14(void);
	/*54*/virtual void		Unk_15(void);
	/*58*/virtual void		Unk_16(void);
	/*5C*/virtual void		Unk_17(void);
	/*60*/virtual void		Unk_18(void);
	/*64*/virtual void		Unk_19(void);
	/*68*/virtual void		Unk_1A(void);
	/*6C*/virtual void		Unk_1B(void);
	/*70*/virtual void		Unk_1C(void);
	/*74*/virtual void		Unk_1D(void);
	/*78*/virtual void		Unk_1E(void);
	/*7C*/virtual void		Unk_1F(void);
	/*80*/virtual void		Unk_20(void);
	/*84*/virtual void		Unk_21(void);
	/*88*/virtual void		Unk_22(void);
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);
	/*B4*/virtual void		Unk_2D(void);
	/*B8*/virtual void		Unk_2E(void);

	UInt32			unk08;		// 08
	UInt32			unk0C;		// 0C
	UInt32			unk10;		// 10
};

// 90
class TileShader : public NiShader
{
public:
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);
	virtual void	Unk_39(void);
	virtual void	Unk_3A(void);
	virtual void	Unk_3B(void);
	virtual void	Unk_3C(void);
	virtual void	Unk_3D(void);
	virtual void	Unk_3E(void);
	virtual void	Unk_3F(void);
	virtual void	Unk_40(void);
	virtual void	Unk_41(void);
	virtual void	Unk_42(void);
	virtual void	Unk_43(void);
	virtual void	Unk_44(void);
	virtual void	Unk_45(void);
	virtual void	Unk_46(void);
	virtual void	Unk_47(void);
	virtual void	Unk_48(void);
	virtual void	Unk_49(void);
	virtual void	Unk_4A(void);
	virtual void	Unk_4B(void);
	virtual void	Unk_4C(void);
	virtual void	Unk_4D(void);
	virtual void	Unk_4E(void);
	virtual void	Unk_4F(void);
	virtual void	Unk_50(void);
	virtual void	Unk_51(void);
	virtual void	Unk_52(void);
	virtual void	Unk_53(void);

	UInt32			unk14[31];		// 14
};
static_assert(sizeof(TileShader) == 0x90);

// 78
class BSPortalGraph : public NiRefObject
{
public:
	UInt32					unk08[10];	// 08
	void					*ptr30;		// 30
	void					*ptr34;		// 34
	UInt32					unk38;		// 38
	NiTArray<NiAVObject*>	array3C;	// 3C
	NiNode					*node4C;	// 4C
	UInt32					unk50[6];	// 50
	BSSimpleArray<NiNode*>	array68;	// 68
};
static_assert(sizeof(BSPortalGraph) == 0x78);

class BSPortal;

// 250
class LightingData : public NiRefObject
{
public:
	UInt32								unk008;				// 008
	float								luminance;			// 00C
	float								matrix4x4[4][4];	// 010
	float								flt050[32];			// 050
	float								lodDimmer;			// 0D0
	float								alpha;				// 0D4
	float								shadowFadeTime0D8;	// 0D8
	float								shadowFadeTime0DC;	// 0DC
	DList<NiGeometry>					geometryList;		// 0E0
	bool								isShadowCasting;	// 0EC
	UInt8								byte0ED;			// 0ED
	UInt8								pad0EE[2];			// 0EE
	NiRefObject							*obj0F0;			// 0F0
	bool								isPointLight;		// 0F4
	bool								isAmbientLight;		// 0F5
	UInt8								pad0F6[2];			// 0F6
	NiLight								*light;				// 0F8
	bool								isDynamic;			// 0FC
	UInt8								pad0FD[3];			// 0FD
	NiVector3							centrePos;			// 100
	BSRenderedTexture					*shadowRenderTarget;// 10C
	UInt16								frustumCull;		// 110	0xFF = disabled (distance too far), anything else = enabled
	UInt16								word112;			// 112
	NiRefObject							*obj114;			// 114
	UInt8								byte118;			// 118
	UInt8								pad119[3];			// 119
	float								flt11C;				// 11C
	float								flt120;				// 120
	bool								showDebugTexture;	// 124
	UInt8								pad125[3];			// 125
	NiNode								*shadowCaster;		// 128
	DList<void>							list12C;			// 12C
	DList<NiGeometry>::Node				*fenceGeomList;		// 138
	NiTriShape							*fenceObject;		// 13C
	NiCamera							*shadowMapCamera;	// 140
	NiFrustumPlanes						frustumPlanes;		// 144
	float								clipPlanes[24];		// 1A8
	UInt8								byte208;			// 208
	UInt8								pad209[3];			// 209
	BSShaderAccumulator					*shaderAccum;		// 20C
	BSSimpleArray<BSMultiBoundNode*>	multiboundRooms;	// 210
	BSSimpleArray<BSPortal*>			array220;			// 220
	BSSimpleArray<NiNode*>				processedNodes;		// 230
	BSPortalGraph						*portalGraph;		// 240
	UInt32								unk244[3];			// 244
};
static_assert(sizeof(LightingData) == 0x250);

// A4
struct BSCompoundFrustum
{
	bool							preThreaded;		// 00
	UInt8							pad01[3];			// 01
	BSSimpleArray<NiFrustumPlanes*>	planesArr;			// 04
	BSSimpleArray<int>				functionOperators;	// 14
	int								iFreePlane;			// 24
	int								iFreeOp;			// 28
	int								iFirstOp;			// 2C
	NiFrustumPlanes					viewFrustum;		// 30
	NiPoint3						viewPosition;		// 94
	bool							skipViewFrustum;	// A0
	UInt8							padA1[3];			// A1
};
static_assert(sizeof(BSCompoundFrustum) == 0xA4);

// 200
class ShadowSceneNode : public NiNode
{
public:
	UInt32								unk0AC[2];			// 0AC
	DList<LightingData>					sceneLights;		// 0B4
	DList<LightingData>					actorShadowCasters;	// 0C0
	DList<LightingData>::Node			*node0CC;			// 0CC
	DList<LightingData>::Node			*node0D0;			// 0D0
	DList<LightingData>::Node			*node0D4;			// 0D4
	LightingData						*sceneLight1;		// 0D8
	LightingData						*sceneLight2;		// 0DC
	LightingData						*pSunLight;			// 0E0
	DList<LightingData>					lgtList0E4;			// 0E4
	DList<LightingData>					list0F0;			// 0F0
	DList<LightingData>					list0FC;			// 0FC
	DList<void>							list108;			// 108
	DList<void>							list114;			// 114
	UInt32								unk120;				// 120
	UInt32								unk124;				// 124
	BSCubeMapCamera						*cubeMapCam;		// 128
	UInt32								unk12C;				// 12C
	bool								noLightProcessing;	// 130
	bool								isWireFrameMode;	// 131
	UInt8								pad132[2];			// 132
	BSFogProperty						*fogProperty;		// 134
	BSCompoundFrustum					compoundFrustum;	// 138
	UInt8								byte1DC;			// 1DC
	UInt8								pad1DD[3];			// 1DD
	BSPortalGraph						*portalGraph;		// 1E0
	NiPoint3							lightingOffset;		// 1E4
	NiPoint3							cameraPos;			// 1F0
	bool								allowLightRemoveQueues;	// 1FC
	UInt8								pad1FD[3];			// 1FD
};
static_assert(sizeof(ShadowSceneNode) == 0x200);

extern ShadowSceneNode *g_shadowSceneNode;

// 114
class NiCamera : public NiAVObject
{
public:
	float			worldToCam[4][4];	// 09C
	NiFrustum		frustum;			// 0DC
	float			minNearPlaneDist;	// 0F8
	float			maxFarNearRatio;	// 0FC
	NiViewport		viewPort;			// 100
	float			LODAdjust;			// 110

	__forceinline static NiCamera *Create() {return CdeclCall<NiCamera*>(0xA71430);}

	bool __fastcall WorldToScreen(const NiVector3 &worldPos, NiPoint2 &scrPos);
};
static_assert(sizeof(NiCamera) == 0x114);

extern NiCamera *g_mainCamera;

// C4
class NiDynamicEffect : public NiAVObject
{
public:
	enum EffectType
	{
		kEffect_AmbientLight =		0,
		kEffect_PointLight =		2,
		kEffect_DirectionalLight =	3,
		kEffect_SpotLight =			4,
		kEffect_TextureEffect =		5
	};

	bool			switchState;	// 9C
	UInt8			effectType;		// 9D
	bool			resetTraits;	// 9E	JIP only
	UInt8			extraFlags;		// 9F	JIP only
	SInt32			iIndex;			// A0
	UInt32			pushCount;		// A4
	UInt32			revID;			// A8
	DList<NiNode>	shadowGenerator;// AC
	DList<NiNode>	unaffectedNodes;// B8
};

// F0
class NiLight : public NiDynamicEffect
{
public:
	float			fadeValue;			// C4
	NiColor			ambientColor;		// C8
	NiColor			diffuseColor;		// D4
};

// FC (JIP: 110)
class NiPointLight : public NiLight
{
public:
	float			radius;			// 0E0
	float			radius0E4;		// 0E4
	TESObjectLIGH	*baseLight;		// 0E8	JIP only
	NiObject		*obj0EC;		// 0EC
	NiVector3		vector0F0;		// 0F0	Used for animated lights
	//	JIP Only
	float			flt0FC;			// 0FC
	NiVector4		vector100;		// 100

	__forceinline static NiPointLight *Create() {return CdeclCall<NiPointLight*>(0xA7D6E0);}
};
static_assert(sizeof(NiPointLight) == 0x110);

// 114
class NiSpotLight : public NiLight
{
public:
	float			radius;			// 0E0
	float			radius0E4;		// 0E4
	TESObjectLIGH	*baseLight;		// 0E8	JIP only
	NiObject		*obj0EC;		// 0EC
	NiVector3		vector0F0;		// 0F0	Used for animated lights
	NiVector3		direction;		// 0FC
	float			outerSpotAngle;	// 108
	float			innerSpotAngle;	// 10C
	float			spotExponent;	// 110
};
static_assert(sizeof(NiSpotLight) == 0x114);

// FC
class NiDirectionalLight : public NiLight
{
public:
	NiColor			fogColor;		// E0
	NiObject		*objEC;			// EC
	NiVector3		direction;		// F0

	__forceinline static NiDirectionalLight *Create() {return CdeclCall<NiDirectionalLight*>(0xA75D50);}
};

// 15C
class NiTextureEffect : public NiDynamicEffect
{
public:
	NiMatrix33		modelProjMat;	// 0C4
	NiVector3		modelProjTrans;	// 0E8
	NiMatrix33		worldProjMat;	// 0F4
	NiVector3		worldProjTrans;	// 118
	NiTexture		*texture;		// 124
	UInt32			filterMode;		// 128
	UInt32			clampMode;		// 12C
	UInt32			textureMode;	// 130
	UInt32			coordMode;		// 134
	bool			planesEnable;	// 138
	UInt8			pad139[3];		// 139
	NiPlane			modelPlane;		// 13C
	NiPlane			worldPlane;		// 14C
};

class BSSceneGraph : public NiNode
{
public:
	/*100*/virtual double	CalculateNoLODFarDist();
	/*104*/virtual void		SetViewDistanceBasedOnFrameRate(float afTime);
};

// C0
class SceneGraph : public BSSceneGraph
{
public:
	NiCamera			*camera;			// AC
	void				*pVisArray;			// B0
	NiCullingProcess	*cullingProc;		// B4
	bool				isMenuSceneGraph;	// B8
	UInt8				padB9[3];			// B9
	float				cameraFOV;			// BC

	__forceinline void SetCameraFOV(float fFOV, bool forceFOV, NiCamera *pCamera, bool bSetLODAdjust)
	{
		ThisCall(0xC52020, this, fFOV, forceFOV, pCamera, bSetLODAdjust);
	}
};

// 3C
class TESAnimGroup : public NiRefObject
{
public:
	struct AnimGroupSound
	{
		float		flt00;
		UInt8		soundID;
		UInt8		pad05[3];
		UInt32		unk08;
		TESSound	*sound;
	};

	UInt32			unk08[2];		// 08
	UInt16			groupIndex;		// 10
	UInt8			pad12[2];		// 12
	UInt32			numKeys;		// 14
	float			*keyTimes;		// 18
	NiVector3		moveVector;		// 1C
	UInt8			leftOrRightFoot;// 28
	UInt8			blend;			// 29
	UInt8			blendIn;		// 2A
	UInt8			blendOut;		// 2B
	UInt8			decal;			// 2C
	UInt8			pad2D[3];		// 2D
	char			*parentRootNode;// 30
	UInt32			unk34;			// 34
	AnimGroupSound	*animSound;		// 38
};
static_assert(sizeof(TESAnimGroup) == 0x3C);

// 78
class BSAnimGroupSequence : public NiControllerSequence
{
public:
	enum AnimSequenceType
	{
		kSequence_Idle,
		kSequence_Movement,
		kSequence_LeftArm,
		kSequence_LeftHand,
		kSequence_Weapon,
		kSequence_WeaponUp,
		kSequence_WeaponDown,
		kSequence_SpecialIdle,
		kSequence_Death = 0x14
	};

	TESAnimGroup	*animGroup;		// 74
};

enum TexturePixelLayout : UInt32
{
	kPixelLayout_Palettized8,
	kPixelLayout_HighColor16,
	kPixelLayout_TrueColor32,
	kPixelLayout_Compressed,
	kPixelLayout_Bumpmap,
	kPixelLayout_Palettized4,
	kPixelLayout_PixDefault,
	kPixelLayout_SingleColor8,
	kPixelLayout_SingleColor16,
	kPixelLayout_SingleColor32,
	kPixelLayout_DoubleColor32,
	kPixelLayout_DoubleColor64,
	kPixelLayout_FloatColor32,
	kPixelLayout_FloatColor64,
	kPixelLayout_FloatColor128,
	kPixelLayout_SingleColor4
};

enum TextureAlphaFormat : UInt32
{
	kAlphaFormat_None,
	kAlphaFormat_Binary,
	kAlphaFormat_Smooth,
	kAlphaFormat_Default
};

enum TextureMipMapFlag : UInt32
{
	kMipMapFlag_Disabled,
	kMipMapFlag_Enabled,
	kMipMapFlag_Default
};

struct TextureFormatPrefs
{
	TexturePixelLayout	pixelLayout;
	TextureAlphaFormat	alpha;
	TextureMipMapFlag	mipMaps;
};

// 0C
struct NiComponentSpec
{
	int			m_eComponent;
	int			m_eRepresentation;
	UInt8		m_ucBitsPerComponent;
	bool		m_bSigned;
	UInt8		pad0A[2];
};

// 44
struct NiPixelFormat
{
	UInt8			m_ucBitsPerPixel;
	bool			m_bSRGBSpace;
	UInt8			pad02[2];
	int				m_eFormat;
	int				m_eTiling;
	UInt32			m_uiRendererHint;
	UInt32			m_uiExtraData;
	NiComponentSpec	m_akComponents[4];
};
static_assert(sizeof(NiPixelFormat) == 0x44);

// 74
class NiPixelData : public NiObject
{
public:
	NiPixelFormat	m_kPixelFormat;		// 08
	void			*m_spPalette;		// 4C
	UInt8			*m_pucPixels;		// 50
	UInt32			*m_puiWidth;		// 54
	UInt32			*m_puiHeight;		// 58
	UInt32			*m_puiOffsetInBytes;// 5C
	UInt32			m_uiMipmapLevels;	// 60
	UInt32			m_uiPixelStride;	// 64
	UInt32			m_uiRevID;			// 68
	UInt32			m_uiFaces;			// 6C
	UInt32			unk70;				// 70
};
static_assert(sizeof(NiPixelData) == 0x74);

// 24
class NiDX92DBufferData : public NiRefObject
{
public:
	Ni2DBuffer				*buffer;		// 08
	void					*ptr0C;			// 0C
	UInt32					unk10;			// 10
	void					*ptr14;			// 14
	UInt32					unk18;			// 18
	UInt32					unk1C;			// 1C
	IDirect3DBaseTexture9	*d3dBaseTexture;// 20
};

// 14
class Ni2DBuffer : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);

	UInt32				width;			// 08
	UInt32				height;			// 0C
	NiDX92DBufferData	*bufferData;	// 10
};

// 28
class NiRenderTargetGroup : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);
	/*B4*/virtual void		Unk_2D(void);
	/*B8*/virtual void		Unk_2E(void);
	/*BC*/virtual void		Unk_2F(void);
	/*C0*/virtual void		Unk_30(void);
	/*C4*/virtual void		Unk_31(void);
	/*C8*/virtual void		Unk_32(void);
	/*CC*/virtual void		Unk_33(void);

	class RendererData;

	UInt8					byte08;			// 08
	UInt8					pad09[3];		// 09
	Ni2DBuffer				*buffers[4];	// 0C
	UInt32					numBuffers;		// 1C
	NiDepthStencilBuffer	*stencilBuffer;	// 20
	RendererData			*rendererData;	// 24
};

enum RendererClearFlag
{
	kClrFlag_BackBuffer =	1,
	kClrFlag_Stencil =		2,
	kClrFlag_ZBuffer =		4,
	kClrFlag_All =			kClrFlag_BackBuffer | kClrFlag_Stencil | kClrFlag_ZBuffer
};

// 210
class NiRenderer : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);

	enum FrameState : UInt32
	{
		kFrmState_OutsideFrame,
		kFrmState_InsideFrame,
		kFrmState_InsideOffscreenFrame,
		kFrmState_waitingForDisplay,
		kFrmState_InternalFrame
	};

	BSShaderAccumulator	*shaderAccum;		// 008
	UInt32				unk00C[124];		// 00C
	FrameState			savdFrameState;		// 1FC
	FrameState			frameState;			// 200
	UInt32				frameID;			// 204
	bool				renderTargetGroup;	// 208
	bool				batchRendering;		// 209
	UInt8				pad20A[2];			// 20A
	UInt32				unk20C;				// 20C
};
static_assert(sizeof(NiRenderer) == 0x210);

enum D3DXIMAGE_FILEFORMAT
{
	D3DXIFF_BMP,
	D3DXIFF_JPG,
	D3DXIFF_TGA,
	D3DXIFF_PNG,
	D3DXIFF_DDS,
	D3DXIFF_PPM,
	D3DXIFF_DIB,
	D3DXIFF_HDR,
	D3DXIFF_PFM
};

enum D3DFORMAT : UInt32
{
	D3DFMT_INVALID =				0,
	D3DFMT_R8G8B8 =					0x14,
	D3DFMT_A8R8G8B8 =				0x15,
	D3DFMT_X8R8G8B8 =				0x16,
	D3DFMT_R5G6B5 =					0x17,
	D3DFMT_X1R5G5B5 =				0x18,
	D3DFMT_A1R5G5B5 =				0x19,
	D3DFMT_A4R4G4B4 =				0x1A,
	D3DFMT_R3G3B2 =					0x1B,
	D3DFMT_A8 =						0x1C,
	D3DFMT_A8R3G3B2 =				0x1D,
	D3DFMT_X4R4G4B4 =				0x1E,
	D3DFMT_A2B10G10R10 =			0x1F,
	D3DFMT_A8B8G8R8 =				0x20,
	D3DFMT_X8B8G8R8 =				0x21,
	D3DFMT_G16R16 =					0x22,
	D3DFMT_A2R10G10B10 =			0x23,
	D3DFMT_A16B16G16R16 =			0x24,
	D3DFMT_A8P8 =					0x28,
	D3DFMT_P8 =						0x29,
	D3DFMT_L8 =						0x32,
	D3DFMT_A8L8 =					0x33,
	D3DFMT_A4L4 =					0x34,
	D3DFMT_V8U8 =					0x3C,
	D3DFMT_L6V5U5 =					0x3D,
	D3DFMT_X8L8V8U8 =				0x3E,
	D3DFMT_Q8W8V8U8 =				0x3F,
	D3DFMT_V16U16 =					0x40,
	D3DFMT_A2W10V10U10 =			0x43,
	D3DFMT_D16_LOCKABLE =			0x46,
	D3DFMT_D32 =					0x47,
	D3DFMT_D15S1 =					0x49,
	D3DFMT_D24S8 =					0x4B,
	D3DFMT_D24X8 =					0x4D,
	D3DFMT_D24X4S4 =				0x4F,
	D3DFMT_D16 =					0x50,
	D3DFMT_L16 =					0x51,
	D3DFMT_D32F_LOCKABLE =			0x52,
	D3DFMT_D24FS8 =					0x53,
	D3DFMT_VERTEXDATA =				0x64,
	D3DFMT_INDEX16 =				0x65,
	D3DFMT_INDEX32 =				0x66,
	D3DFMT_Q16W16V16U16 =			0x6E,
	D3DFMT_R16F =					0x6F,
	D3DFMT_G16R16F =				0x70,
	D3DFMT_A16B16G16R16F =			0x71,
	D3DFMT_R32F =					0x72,
	D3DFMT_G32R32F =				0x73,
	D3DFMT_A32B32G32R32F =			0x74,
	D3DFMT_CxV8U8 =					0x75,
	D3DFMT_A1 =						0x76,
	D3DFMT_A2B10G10R10_XR_BIAS =	0x77,
	D3DFMT_BINARYBUFFER =			0xC7,
	D3DFMT_DXT1 =					0x31545844,
	D3DFMT_DXT2 =					0x32545844,
	D3DFMT_DXT3 =					0x33545844,
	D3DFMT_DXT4 =					0x34545844,
	D3DFMT_DXT5 =					0x35545844
};

enum D3DPRIMITIVETYPE : UInt32
{
    D3DPT_POINTLIST =		1,
    D3DPT_LINELIST =		2,
    D3DPT_LINESTRIP =		3,
    D3DPT_TRIANGLELIST =	4,
    D3DPT_TRIANGLESTRIP =	5,
    D3DPT_TRIANGLEFAN =		6
};

// ???
class IDirect3DBaseTexture9
{
public:
	void			**vtbl;		// 00
	void			*ptr04;		// 04
	D3DFORMAT		texFormat;	// 08
	//	And unknown more.
};

class NiDX9Renderer : public NiRenderer
{
public:
	/*090*/virtual void		SetDefaultProgramCache();
	/*094*/virtual void		Unk_25(void);
	/*098*/virtual void		Unk_26(void);
	/*09C*/virtual void		Unk_27(void);
	/*0A0*/virtual void		Unk_28(void);
	/*0A4*/virtual void		Unk_29(void);
	/*0A8*/virtual void		Unk_2A(void);
	/*0AC*/virtual void		SetBackgroundColor(NiVector4* inARGB);
	/*0B0*/virtual void		Unk_2C(void);
	/*0B4*/virtual void		GetBackgroundColor(NiVector4* outARGB);
	/*0B8*/virtual void		Unk_2E(void);
	/*0BC*/virtual void		Unk_2F(void);
	/*0C0*/virtual void		Unk_30(void);
	/*0C4*/virtual void		Unk_31(void);
	/*0C8*/virtual NiRenderTargetGroup* GetDefaultRT();	// get back buffer rt
	/*0CC*/virtual NiRenderTargetGroup* GetCurrentRT();	// get currentRTGroup
	/*0D0*/virtual void		Unk_34(void);
	/*0D4*/virtual void		Unk_35(void);
	/*0D8*/virtual void		Unk_36(void);
	/*0DC*/virtual void		Unk_37(void);
	/*0E0*/virtual bool		PrecacheTexture(NiTexture *pkIm, bool bForceLoad, bool bLocked);
	/*0E4*/virtual void 	Unk_39(void);
	/*0E8*/virtual void		Unk_3A(void);
	/*0EC*/virtual bool		PrecacheGeometry(NiGeometry *pkGeometry, UInt32 uiBonesPerPartition, UInt32 uiBonesPerVertex);
	/*0F0*/virtual void		PurgeGeometry(NiGeometryData* geo);
	/*0F4*/virtual void		PurgeMaterial(NiMaterialProperty* material);
	/*0F8*/virtual void		PurgeEffect(NiDynamicEffect* effect);
	/*0FC*/virtual void		PurgeScreenTexture();
	/*100*/virtual void		PurgeSkinPartition(NiSkinPartition* skinPartition);
	/*104*/virtual void		PurgeSkinInstance(NiSkinInstance* skinInstance);
	/*108*/virtual void		Unk_42(void);
	/*10C*/virtual bool		PurgeAllTextures();
	/*110*/virtual NiPixelData	*TakeScreenShot(const NiRect<UInt32> *pkScreenRect, const NiRenderTargetGroup *pkTarget);
	/*114*/virtual bool		FastCopy(const Ni2DBuffer *pkSrc, Ni2DBuffer *pkDest, const NiRect<UInt32> *pkSrcRect, UInt32 uiDestX, UInt32 uiDestY);
	/*118*/virtual bool		Copy(const Ni2DBuffer *pkSrc, Ni2DBuffer *pkDest, NiRect<UInt32> *pkSrcRect, NiRect<UInt32> *pkDestRect, UInt32 copyFilter);
	/*11C*/virtual void		Unk_47(void);
	/*120*/virtual bool		Unk_48(void* arg);
	/*124*/virtual void		Unk_49(void);
	/*128*/virtual void		Unk_4A(float arg);
	/*12C*/virtual void 	Unk_4B(UInt32 size);
	/*130*/virtual void		Unk_4C(UInt32 arg0, UInt32 arg1);
	/*134*/virtual void		Unk_4D(UInt32 arg0, UInt32 arg1);
	/*138*/virtual void		Unk_4E(void* buf);
	/*13C*/virtual void		CreateSourceTexture(NiSourceTexture* texture);
	/*140*/virtual bool		CreateRenderedTexture(NiRenderedTexture* arg);
	/*144*/virtual bool		CreateSourceCubeMap(NiSourceCubeMap* arg);
	/*148*/virtual bool		CreateRenderedCubeMap(NiRenderedCubeMap* arg);
	/*14C*/virtual bool		CreateDynamicTexture(void* arg);
	/*150*/virtual void		Unk_54(void);
	/*154*/virtual bool		CreateDepthStencil(NiDepthStencilBuffer* arg, void* textureFormat);
	/*158*/virtual void		Unk_56(void);
	/*15C*/virtual void		Unk_57(void);
	/*160*/virtual void		Unk_58(void);
	/*164*/virtual void		Unk_59(void);
	/*168*/virtual void		Unk_5A(void);
	/*16C*/virtual void		Unk_5B(void);
	/*170*/virtual void		Unk_5C(void);
	/*174*/virtual void		Unk_5D(void);
	/*178*/virtual void		Unk_5E(void);
	/*17C*/virtual bool		BeginScene();
	/*180*/virtual bool		EndScene();
	/*184*/virtual void		DisplayScene();
	/*188*/virtual void		Clear(float* rect, UInt32 flags);
	/*18C*/virtual void		SetupCamera(NiVector3* pos, NiVector3* at, NiVector3* up, NiVector3* right, NiFrustum* frustum, float* viewport);
	/*190*/virtual void		SetupScreenSpaceCamera(float* viewport);
	/*194*/virtual bool		BeginUsingRenderTargetGroup(NiRenderTargetGroup* renderTarget, RendererClearFlag clearFlags);
	/*198*/virtual bool		EndUsingRenderTargetGroup();
	/*19C*/virtual void		BeginBatch(UInt32 arg0, UInt32 arg1);
	/*1A0*/virtual void		EndBatch();
	/*1A4*/virtual void		BatchRenderShape(void* arg);
	/*1A8*/virtual void		BatchRenderStrips(void* arg);
	/*1AC*/virtual void		RenderTriShape(NiTriShape* obj);
	/*1B0*/virtual void		RenderTriStrips(NiTriStrips* obj);
	/*1B4*/virtual void		RenderTriShape2(NiTriShape* obj);
	/*1B8*/virtual void		RenderTriStrips2(NiTriStrips* obj);
	/*1BC*/virtual void		RenderParticles(NiParticles* obj);
	/*1C0*/virtual void		RenderLines(NiLines* obj);
	/*1C4*/virtual void		RenderScreenTexture();

	class PrePackObject;

	enum FrameBufferFormat
	{
		FBFMT_UNKNOWN = 0,
		FBFMT_R8G8B8,
		FBFMT_A8R8G8B8,
		FBFMT_X8R8G8B8,
		FBFMT_R5G6B5,
		FBFMT_X1R5G5B5,
		FBFMT_A1R5G5B5,
		FBFMT_A4R4G4B4,
		FBFMT_R3G3B2,
		FBFMT_A8,
		FBFMT_A8R3G3B2,
		FBFMT_X4R4G4B4,
		FBFMT_R16F,
		FBFMT_G16R16F,
		FBFMT_A16B16G16R16F,
		FBFMT_R32F,
		FBFMT_G32R32F,
		FBFMT_A32B32G32R32F,
		FBFMT_NUM
	};

	enum DepthStencilFormat
	{
		DSFMT_UNKNOWN = 0,
		DSFMT_D16_LOCKABLE = 70,
		DSFMT_D32 = 71,
		DSFMT_D15S1 = 73,
		DSFMT_D24S8 = 75,
		DSFMT_D16 = 80,
		DSFMT_D24X8 = 77,
		DSFMT_D24X4S4 = 79,
	};

	enum PresentationInterval
	{
		PRESENT_INTERVAL_IMMEDIATE = 0,
		PRESENT_INTERVAL_ONE = 1,
		PRESENT_INTERVAL_TWO = 2,
		PRESENT_INTERVAL_THREE = 3,
		PRESENT_INTERVAL_FOUR = 4,
		PRESENT_INTERVAL_NUM
	};

	enum SwapEffect
	{
		SWAPEFFECT_DEFAULT,
		SWAPEFFECT_DISCARD,
		SWAPEFFECT_FLIP,
		SWAPEFFECT_COPY,
		SWAPEFFECT_NUM
	};

	enum FrameBufferMode
	{
		FBMODE_DEFAULT,
		FBMODE_LOCKABLE,
		FBMODE_MULTISAMPLES_2 = 0x00010000,
		FBMODE_MULTISAMPLES_3 = 0x00020000,
		FBMODE_MULTISAMPLES_4 = 0x00030000,
		FBMODE_MULTISAMPLES_5 = 0x00040000,
		FBMODE_MULTISAMPLES_6 = 0x00050000,
		FBMODE_MULTISAMPLES_7 = 0x00060000,
		FBMODE_MULTISAMPLES_8 = 0x00070000,
		FBMODE_MULTISAMPLES_9 = 0x00080000,
		FBMODE_MULTISAMPLES_10 = 0x00090000,
		FBMODE_MULTISAMPLES_11 = 0x000a0000,
		FBMODE_MULTISAMPLES_12 = 0x000b0000,
		FBMODE_MULTISAMPLES_13 = 0x000c0000,
		FBMODE_MULTISAMPLES_14 = 0x000d0000,
		FBMODE_MULTISAMPLES_15 = 0x000e0000,
		FBMODE_MULTISAMPLES_16 = 0x000f0000,
		FBMODE_MULTISAMPLES_NONMASKABLE = 0x80000000,
		FBMODE_QUALITY_MASK = 0x0000FFFF,
		FBMODE_NUM = 18
	};

	enum RefreshRate
	{
		REFRESHRATE_DEFAULT = 0
	};

	UInt32								unk210[30];					// 210
	IDirect3DDevice9					*device;					// 288
	UInt32								unk28C[76];					// 28C
	HANDLE								deviceWindow;				// 3BC
	HANDLE								focusWindow;				// 3C0
	char								rendererInfo[0x200];		// 3C4
	UInt32								adapterIdx;					// 5C4
	UInt32								d3dDevType;					// 5C8 - D3DDEVTYPE
	UInt32								d3dDevFlags;				// 5CC - D3DCREATE
	UInt8								softwareVertexProcessing;	// 5D0 - !D3DCREATE_HARDWARE_VERTEXPROCESSING
	UInt8								mixedVertexProcessing;		// 5D1 - D3DCREATE_MIXED_VERTEXPROCESSING
	UInt8								pad5D2[2];					// 5D2
	UInt32								unk5D4[3];					// 5D4
	UInt32								backgroundColor;			// 5E0	ARGB
	UInt32								unk5E4[11];					// 5E4
	NiTPtrMap<PrePackObject*>			prePackObjects;				// 610 - NiTPointerMap <NiVBBlock *, NiDX9Renderer::PrePackObject *>
	UInt32								unk620[153];				// 620
	NiRenderTargetGroup					*defaultRTGroup;			// 884 - back buffer
	NiRenderTargetGroup					*currentRTGroup;			// 888
	NiRenderTargetGroup					*currentscreenRTGroup;		// 88C
	NiTPtrMap<NiRenderTargetGroup*>		screenRTGroups;				// 890 - NiTPointerMap <HWND *, NiPointer <NiRenderTargetGroup> >
	UInt32								unk8A0[6];					// 8A0
	NiDX9RenderState					*renderState;				// 8B8
	UInt32								unk8BC[3];					// 8BC
	NiTMap<NiLight*, void*>				*lightsMap;					// 8C8
	UInt32								unk8CC[115];				// 8CC
	UInt32								width;						// A98
	UInt32								height;						// A9C
	UInt32								flags;						// AA0
	UInt32								windowDevice;				// AA4
	UInt32								windowFocus;				// AA8
	UInt32								adapterType;				// AAC
	UInt32								deviceType;					// AB0
	FrameBufferFormat					frameBufferFormat;			// AB4
	DepthStencilFormat					depthStencilFormat;			// AB8
	PresentationInterval				presentationInterval;		// ABC
	SwapEffect							swapEffect;					// AC0
	FrameBufferMode						frameBufferMode;			// AC4
	UInt32								backBufferCount;			// AC8
	RefreshRate							refreshRate;				// ACC
	UInt32								unkAD0[17];					// AD0
	NiTMap<D3DFORMAT, NiPixelFormat*>	formatMatchMao;				// B14
	UInt32								unkB24[23];					// B24

	__forceinline static NiDX9Renderer *GetSingleton() {return *(NiDX9Renderer**)0x11F4748;}
};
static_assert(sizeof(NiDX9Renderer) == 0xB80);

#define NIDX9RENDERER ds:0x11F4748

// 70
class NiDX9TextureData : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual IDirect3DBaseTexture9	*GetD3DBaseTexture();
	/*A0*/virtual void		SetD3DBaseTexture(IDirect3DBaseTexture9 *baseTexture);
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);

	NiTexture				*owningTexture;	// 08
	UInt32					width;			// 0C
	UInt32					height;			// 10
	NiPixelFormat			pxlFormat;		// 14
	UInt8					byte58;			// 58
	UInt8					byte59;			// 59
	UInt8					byte5A;			// 5A
	UInt8					state;			// 5B
	UInt32					unk5C;			// 5C
	NiDX9Renderer			*renderer;		// 60
	IDirect3DBaseTexture9	*d3dBaseTexture;// 64
	UInt32					levels;			// 68
	NiObject				*object6C;		// 6C
};
static_assert(sizeof(NiDX9TextureData) == 0x70);

// 30
class NiTexture : public NiObjectNET
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual UInt32	GetRenderedWidth();
	/*98*/virtual UInt32	GetRenderedHeight();
	/*9C*/virtual NiFixedString		*GetPath();
	/*A0*/virtual UInt32	GetPixelCount();

	TextureFormatPrefs	formatPrefs;	// 18
	NiDX9TextureData	*textureData;	// 24
	NiTexture			*prev;			// 28
	NiTexture			*next;			// 2C
};
static_assert(sizeof(NiTexture) == 0x30);

// 48
class NiSourceTexture : public NiTexture
{
public:
	/*A4*/virtual void		LoadPixelDataFromFile();
	/*A8*/virtual void		DestroyAppPixelData();
	/*AC*/virtual void		CreateRendererData();

	NiFixedString	ddsPath1;		// 30
	NiFixedString	ddsPath2;		// 34
	void			*ptr38;			// 38
	NiPixelData		*pixelData;		// 3C
	bool			loadDirectToRender;	// 40
	bool			persistRenderData;	// 41
	bool			srcRendererDataIsPersistent;			// 42
	UInt8			byte43;			// 43
	NiFile			*pFile;			// 44

	static NiSourceTexture* __fastcall Create(const char *ddsPath);
};

// 4C
class NiSourceCubeMap : public NiSourceTexture
{
public:
	UInt32			unk48;	// 48
};

// 48
class NiRenderedTexture : public NiTexture
{
public:
	/*A4*/virtual Ni2DBuffer	*Get2DBuffer();

	Ni2DBuffer			*buffer;		// 30
	UInt32				cellRefID;		// 34	Used by MiniMap; otherwise appears unused
	UInt32				unk38;			// 38
	UInt32				multiSampleLvl;	// 3C
	bool				isRenderTarget;	// 40
	bool				useCustomFormat;// 41
	UInt8				pad42[2];		// 42
	D3DFORMAT			customFormat;	// 44

	void __fastcall SaveToFile(UInt32 fileFmt, char *filePath);
};

enum RenderTargetTypes : UInt32
{
	/*00*/eRTT_HDR_DOWNSAMPLE,
	/*01*/eRTT_HDR_DOWNSAMPLE_ANISOINTERMEDIATE,
	/*02*/eRTT_HDR_BLURSWAP,
	/*03*/eRTT_IMAGESPACE_SWAP,
	/*04*/eRTT_MAIN_IMAGESPACE,
	/*05*/eRTT_MAIN_FIRSTPERSON,
	/*06*/eRTT_MAIN_MENUBG,
	/*07*/eRTT_TESWATER_FFT,
	/*08*/eRTT_TESWATER_GREY,
	/*09*/eRTT_TESWATER_REFLECTION,
	/*0A*/eRTT_TESWATER_LOD_REFLECTION,
	/*0B*/eRTT_TESWATER_REFRACTION,
	/*0C*/eRTT_TESWATER_RAIN,
	/*0D*/eRTT_TESWATER_SCRAMBLEMAP,
	/*0E*/eRTT_TESWATER_IFFT_BUTTERFLYMAP,
	/*0F*/eRTT_TESWATER_HIGH_W,
	/*10*/eRTT_TESWATER_HIGH_AMPLITUDE,
	/*11*/eRTT_TESWATER_DEPTH,
	/*12*/eRTT_TESWATER_NOISE_HEIGHTMAP,
	/*13*/eRTT_TESWATER_NOISE_NORMALMAP,
	/*14*/eRTT_TESWATER_DISPLACEMENT,
	/*15*/eRTT_TESWATER_DEBUG_DISPLAY,
	/*16*/eRTT_TESWATER_BLUR,
	/*17*/eRTT_TREES_CANOPYSHADOWMASK,
	/*18*/eRTT_BLURFULL_BUFFER,
	/*19*/eRTT_BLURFULL_SWAP,
	/*1A*/eRTT_BLUR20_BUFFER,
	/*1B*/eRTT_GETHIT_BUFFER,
	/*1C*/eRTT_LOCALMAP_FINAL,
	/*1D*/eRTT_LOCALMAP_COLOR,
	/*1E*/eRTT_LOCALMAP_NORMALS,
	/*1F*/eRTT_REFRACTION_NORMALS,
	/*20*/eRTT_VFOG,
	/*21*/eRTT_SCENE_DEPTH_BUFFER,
	/*22*/eRTT_VFOG_SWAP,
	/*23*/eRTT_VELOCITY,
	/*24*/eRTT_VELOCITY_SWAP,
	/*25*/eRTT_DOF_BLUR,
	/*26*/eRTT_DOF_BLUR_SWAP,
	/*27*/eRTT_DOF_MASK,
	/*28*/eRTT_DEPTH_RADIALBLUR_BLUR,
	/*29*/eRTT_CUBEMAP_CAMERA,
	/*2A*/eRTT_SHADOWS_SHADOWMAP,
	/*2B*/eRTT_SHADOWS_SHADOWMAPFRUSTUM,
	/*2C*/eRTT_ENVMAP_WINDOWS,
	/*2D*/eRTT_INTERFACE_RENDEREDMENU,
	/*2E*/eRTT_INTERFACE_RENDEREDMENU_SWAP,
	/*2F*/eRTT_INTERFACE_VATS,
	/*30*/eRTT_INTERFACE_TEMP,
	/*31*/eRTT_NOISE_BUFFER,
	/*32*/eRTT_UNK_32,
	/*33*/eRTT_DECAL_OCCLUSION
};

enum TextureCreationFlags
{
	eTCF_NONE =				0,
	eTCF_UNK_1 =			1,
	eTCF_UNK_2 =			2,
	eTCF_NO_STENCIL =		4,
	eTCF_NO_RENDER_TARGET =	8,
	eTCF_CUBE_MAP =			0x10,
	eTCF_UNK_20 =			0x20,
	eTCF_USE_MSAA =			0x40,
	eTCF_NO_DEPTH_DISCARD =	0x80,
	eTCF_NO_DEPTH =			0x100,
	eTCF_UNK_200 =			0x200,
	eTCF_UNK_400 =			0x400
};

// 40
class BSRenderedTexture : public NiObject	//	0x10AE500
{
public:
	NiRenderTargetGroup		*targetGroups[6];	// 08
	NiRenderTargetGroup		*targetGroup20;		// 20
	NiObject				*object24;			// 24
	NiObject				*object28;			// 28
	RenderTargetTypes		rtType;				// 2C
	NiRenderedTexture		*textures[4];		// 30
};

// 0C
class NiAccumulator : public NiObject
{
public:
	/*8C*/virtual void		SetCamera(NiCamera *_camera);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(UInt32 arg);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);

	NiCamera		*camera;		// 08
};

// 2C
class NiBackToFrontAccumulator : public NiAccumulator
{
public:
	/*A4*/virtual void		Unk_29(void);

	DList<void>		list0C;			// 0C
	UInt32			unk18[5];		// 18
};

// 34
class NiAlphaAccumulator : public NiBackToFrontAccumulator
{
public:
	UInt32			unk2C;			// 2C
	UInt8			byte30;			// 30
	UInt8			byte31;			// 31
	UInt8			byte32;			// 32
	UInt8			pad33;			// 33
};

// 98
class BSBatchRenderer : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);

	UInt32			unk08[36];		// 08
};
static_assert(sizeof(BSBatchRenderer) == 0x98);

enum RenderMode : UInt32
{
	kRndrMode_Normal,		//	Renders everything
	kRndrMode_Unk1,
	kRndrMode_Unk2,
	kRndrMode_Unk3,
	kRndrMode_Unk4,
	kRndrMode_Unk5,
	kRndrMode_Unk6,
	kRndrMode_DepthMap,
	kRndrMode_ConstAlpha,	//	Everything is green
	kRndrMode_LocalMap,		//	Renders vertex normals, skips landscape
	kRndrMode_Tiles,		//	Renders only UI tiles
	kRndrMode_UnkB,
	kRndrMode_NoWaterPass,	//	Skips water rendering
	kRndrMode_UnkD,
	kRndrMode_WaterDepth,
	kRndrMode_Silhouette	//	Everything is black except for grass, sky and emissives
};

// 280	vtbl @ 0x10ADFF8
class BSShaderAccumulator : public NiAlphaAccumulator
{
public:
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(UInt32 arg1, UInt32 arg2);

	// 14
	struct Struct48
	{
		UInt32		unk00;
		UInt8		byte04;
		UInt8		pad05[3];
		float		flt08;
		UInt32		unk0C;
		UInt32		unk10;
	};

	struct AccumStruct	//	Temp name
	{
		void		*_vtbl;	// 0x10B7DC0
		UInt32		unk04;
		UInt32		unk08;
		UInt32		unk0C;
		UInt32		unk10;
	};

	UInt32					unk034;				// 034
	bool					wireFrameRelated;	// 038
	UInt8					byte039;			// 039
	bool					renderDOF;			// 03A
	UInt8					byte03B;			// 03B
	UInt32					sunPixelCount;		// 03C
	bool					waitingForSunQuery;	// 040
	UInt8					pad041[3];			// 041
	float					percentSunOccludedStored;	// 044
	Struct48				unk048;				// 048
	Struct48				unk05C;				// 05C
	Struct48				unk070;				// 070
	bool					processLights;		// 084
	UInt8					byte085;			// 085
	UInt8					byte086;			// 086
	UInt8					byte087;			// 087
	UInt16					word088;			// 088
	UInt16					word08A;			// 08A
	void					*ptr08C;			// 08C	Array of 200 DWORDS, or pointers
	UInt32					unk090[3];			// 090
	AccumStruct				accum09C;			// 09C
	AccumStruct				accum0B0;			// 0B0
	AccumStruct				accum0C4;			// 0C4
	AccumStruct				accum0D8;			// 0D8
	AccumStruct				accum0EC;			// 0EC
	AccumStruct				accum100;			// 100
	AccumStruct				accum114;			// 114
	AccumStruct				accum128;			// 128
	UInt32					unk13C[5];			// 13C
	UInt8					byte150;			// 150
	UInt8					pad151[3];			// 151
	NiColorAlpha			ambientColor;		// 154
	bool					ignoreWater;		// 164
	bool					renderWaterDepth;	// 165
	UInt8					byte166;			// 166
	bool					isUnderwater;		// 167
	UInt32					waterHeight;		// 168
	bool					renderDecals;		// 16C
	UInt8					pad16D[3];			// 16D
	void					*ptr170;			// 170
	BSBatchRenderer			*renderer174;		// 174
	BSBatchRenderer			*renderer178;		// 178
	UInt32					*passes;			// 17C
	void					*ptr180;			// 180
	void					*ptr184;			// 184
	void					*ptr188;			// 188
	UInt32					batchRendererCount;	// 18C
	UInt32					maxPassCount;		// 190
	ShadowSceneNode			*shadowScene;		// 194
	UInt32					unk198;				// 198
	RenderMode				renderMode;			// 19C	Index of a callback in 2 arrays @ 0x11F9F40 & 0x11F9F80
	LightingData			*depthLight;		// 1A0
	BSRenderPass			*tileRenderPass;	// 1A4
	UInt16					renderPassType;		// 1A8
	UInt8					pad1AA[2];			// 1AA
	UInt32					unk1A0[2];			// 1AC
	NiTPtrMap<void*>		map1B4;				// 1B4
	UInt32					unk1C4[47];			// 1C4
};
static_assert(sizeof(BSShaderAccumulator) == 0x280);

struct NiVBDynamicSet;
struct NiVBChip;

// 0C
class NiGeometryGroup
{
public:
	/*00*/virtual void	Purge();
	/*04*/virtual void	Unk_01(void *arg);
	/*08*/virtual void	Unk_02(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	/*0C*/virtual void	Unk_03(UInt32 arg);
	/*10*/virtual void	Unk_04(UInt32 arg);
	/*14*/virtual void	Unk_05(UInt32 arg);
	/*18*/virtual void	Unk_06(UInt32 arg1, UInt32 arg2);
	/*1C*/virtual void	Unk_07(UInt32 arg1, UInt32 arg2);
	/*20*/virtual bool	IsDynamic();
	/*24*/virtual void	Destroy(bool doFree);
	
	UInt32			m_uiRefCount;		// 04
	void			*pD3DDevice;		// 08
};

// 1C
class NiStaticGeometryGroup : public NiGeometryGroup
{
public:
	struct NiVBSet;
	
	NiTPtrMap<NiVBSet>		map0C;		// 0C
};

// 0C
class NiUnsharedGeometryGroup : public NiGeometryGroup
{
public:
};

// 50
class NiDynamicGeometryGroup : public NiGeometryGroup
{
public:
	NiTPtrMap<NiVBDynamicSet>		map0C;		// 0C
	NiTPtrMap<NiVBChip>				map1C;		// 1C
	NiTArray<NiVBDynamicSet*>		array2C;	// 2C
	NiTArray<NiVBChip*>				array3C;	// 3C
	UInt32							unk4C;		// 4C
};
static_assert(sizeof(NiDynamicGeometryGroup) == 0x50);

// 2C
class NiAdditionalGeometryData : public NiObject
{
public:
	// 10
	class NiAGDDataBlock
	{
	public:
		virtual void	Destructor(bool doFree);
		virtual void	*AllocBlock(UInt32 size);
		virtual void	Unk_02(UInt32, UInt32);
		virtual void	Unk_03(UInt32, UInt32);
		virtual void	FreeBlock(void *pBlock);

		UInt32			unk04;		// 04
		void			*block08;	// 08
		UInt8			byte0C;		// 0C
		UInt8			pad0D[3];	// 0D
	};

	/*8C*/virtual bool		IsPacked();
	/*90*/virtual NiAGDDataBlock	*AllocDataBlock();

	UInt32						unk08;		// 08
	UInt16						word0C;		// 0C
	UInt8						pad0E[2];	// 0E
	UInt32						unk10;		// 10
	void						*ptr14;		// 14
	UInt32						unk18;		// 18
	NiTArray<NiAGDDataBlock*>	dataBlocks;	// 1C
};
static_assert(sizeof(NiAdditionalGeometryData) == 0x2C);

// 54	vtbl @ 0x010F017C
class NiGeometryBufferData
{
public:
	/*00*/virtual void		Destructor(bool doFree);
	/*04*/virtual bool		Unk_01(UInt32 arg1);

	UInt32					flags;			// 04
	NiGeometryGroup			*geomGroup;		// 08
	UInt32					fvf;			// 0C
	void					*declaration;	// 10
	bool					softwareVP;		// 14
	UInt8					pad15[3];		// 15
	UInt32					vertexCount;	// 18
	UInt32					maxVertCount;	// 1C
	UInt32					streamCount;	// 20
	void					*vertexStride;	// 24
	NiVBChip				**pVBChip;		// 28
	UInt32					trianglePoints;	// 2C
	UInt32					trigPtSize;		// 30	Byte size of triangles array
	void					*ptr34;			// 34
	UInt32					baseVertexIdx;	// 38
	D3DPRIMITIVETYPE		primitiveType;	// 3C
	UInt32					triangleCount;	// 40
	UInt32					maxTriCount;	// 44
	UInt32					numArrays;		// 48
	UInt16					*indexArray;	// 4C
	NiTriangle				*triangles;		// 50	Same ptr as in NiTriShapeData
};
static_assert(sizeof(NiGeometryBufferData) == 0x54);

// 40
class NiGeometryData : public NiObject
{
public:
	/*8C*/virtual void		SetVertexCount(UInt16 vtxCount);
	/*90*/virtual UInt16	GetVertexCount();
	/*94*/virtual NiTriStripsData	*GetStripsData();
	/*98*/virtual NiTriShapeData	*GetShapeData();
	/*9C*/virtual bool		ContainsDataType(UInt32 dataType);
	/*A0*/virtual void		CalculateNormals();

	enum KeepFlag
	{
		eKeep_Vertices =	1,
        eKeep_Normals =		2,
        eKeep_VertexColor =	4,
        eKeep_UVCoords =	8,
        eKeep_Indices =		0x10,
        eKeep_BoneData =	0x20,
		eKeep_All =			eKeep_Vertices | eKeep_Normals | eKeep_VertexColor | eKeep_UVCoords | eKeep_Indices | eKeep_BoneData
	};

	enum CompressFlag
	{
		eCompress_Normals =		1,
		eCompress_Color =		2,
		eCompress_UV =			4,
		eCompress_Weight =		8,
		eCompress_Position =	0x10,
		eCompress_All =			0x1F
	};

	UInt16						numVertices;	// 08
	UInt16						id;				// 0A
	UInt16						dataFlags;		// 0C	NormalBinormalTangent
	UInt16						dirtyFlags;		// 0E
	NiBound						bounds;			// 10
	NiVector3					*vertices;		// 20
	NiVector3					*normals;		// 24
	NiColorAlpha				*vertexColors;	// 28
	NiPoint2					*uvCoords;		// 2C
	NiAdditionalGeometryData	*additionalData;// 30
	NiGeometryBufferData		*bufferData;	// 34
	UInt8						keepFlags;		// 38
	UInt8						compressFlags;	// 39
	UInt8						byte3A;			// 3A
	UInt8						byte3B;			// 3B
	bool						canSave;		// 3C
	UInt8						pad3D[3];		// 3D

	void FlipNormals();
};
static_assert(sizeof(NiGeometryData) == 0x40);

// 44
class NiTriBasedGeomData : public NiGeometryData
{
public:
	/*A4*/virtual void		Unk_29(UInt32 arg);
	/*A8*/virtual UInt16	GetNumTriangles();
	/*AC*/virtual void		Unk_2B(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	/*B0*/virtual void		Unk_2C(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);

	UInt16			numTriangles;		// 40
	UInt16			activeTriangles;	// 42
};

// 58
class NiTriShapeData : public NiTriBasedGeomData
{
public:
	struct SharedNormalArray
	{
		UInt16		numSharedNormals;
		UInt16		pad02;
		UInt16		*sharedNormalIndexArray;
	};

	struct SharedNormArrBlock
	{
		UInt16				*pBlock;
		UInt16				*pFreeBlock;
		UInt32				blockSize;
		UInt32				freeBlockSize;
		SharedNormArrBlock	*pNext;
	};

	UInt32				trianglePoints;		// 44
	NiTriangle			*triangles;			// 48
	SharedNormalArray	*sharedNormals;		// 4C
	UInt16				sharedNormArrSize;	// 50
	UInt16				pad52;				// 52
	SharedNormArrBlock	*snArrBlock;		// 54

	static NiTriShapeData *Create(UInt32 numVertices, NiPoint3 *vertices, NiPoint3 *normals, NiColorAlpha *vertexColor,
		NiPoint2 *uvCoords, UInt32 numTriangles, NiTriangle *triangles)
	{
		return ThisCall<NiTriShapeData*>(0xA7B630, Ni_Alloc<NiTriShapeData>(), numVertices, vertices, normals, vertexColor, uvCoords, 0, 0, numTriangles, triangles);
	}
};
static_assert(sizeof(NiTriShapeData) == 0x58);

// 50
class NiTriStripsData : public NiTriBasedGeomData
{
public:
	UInt16			numStrips;		// 44
	UInt16			unk46;			// 46
	UInt16			*stripLengths;	// 48
	UInt16			*strips;		// 4C

	static NiTriStripsData *Create(UInt32 numVertices, NiPoint3 *vertices, NiPoint3 *normals, NiColorAlpha *vertexColor,
		NiPoint2 *uvCoords, UInt32 numTriangles, UInt32 numStrips, UInt16 *stripLengths, UInt16 *strips)
	{
		return ThisCall<NiTriStripsData*>(0xA75DC0, Ni_Alloc<NiTriStripsData>(), numVertices, vertices, normals, vertexColor,
			uvCoords, 0, 0, numTriangles, numStrips, stripLengths, strips);
	}

	static NiTriStripsData* __vectorcall DrawConvex(float radius, UInt32 numEdges, const NiColorAlpha &color);
	static NiTriStripsData* __vectorcall DrawCylinder(float radius, float height, UInt32 numEdges, const NiColorAlpha &color);
	static NiTriStripsData* __vectorcall DrawPrism(float radius, float height, UInt32 numEdges, const NiColorAlpha &color);
};

// 44
class NiLinesData : public NiGeometryData
{
public:
	UInt8			*vertexConnectFlags;	// 40
};

class NiSkinData : public NiObject
{
public:
};

// 10
class NiSkinPartition : public NiObject
{
public:

};

// 34
class NiSkinInstance : public NiObject
{
public:
	NiSkinData		*skinData;		// 08
	NiSkinPartition	*skinPartition;	// 0C
	NiNode			*actorRoot;		// 10
	NiAVObject		**pBones;		// 14
	UInt32			frameID;		// 18
	UInt32			numMatrices;	// 1C
	UInt32			numRegisters;	// 20
	UInt32			allocatedSize;	// 24
	void			*pBoneMatrices;	// 28
	void			*ptr2C;			// 2C
	void			*skinToWorldToSkinMatrix;	// 30
};

// C4
class NiGeometry : public NiAVObject
{
public:
	/*DC*/virtual void		RenderImmediate(NiRenderer *pRenderer);
	/*E0*/virtual void		RenderImmediateAlt(NiRenderer *pRenderer);
	/*E4*/virtual void		SetGeometryData(NiGeometryData *pkModelData);
	/*E8*/virtual void		CalculateNormals();
	/*EC*/virtual void		CalculateConsistency(UInt8 arg1);

	NiAlphaProperty			*alphaProp;		// 9C
	NiCullingProperty		*cullingProp;	// A0
	NiMaterialProperty		*materialProp;	// A4
	BSShaderProperty		*shaderProp;	// A8
	NiStencilProperty		*stencilProp;	// AC
	NiTexturingProperty		*texturingProp;	// B0
	UInt32					unkB4;			// B4
	NiGeometryData			*geometryData;	// B8
	NiSkinInstance			*skinInstance;	// BC
	NiShader				*shader;		// C0

	GeometryProperties *GetProperties() const {return (GeometryProperties*)&alphaProp;}

	NiProperty *GetProperty(UInt32 propID) const
	{
		return ((NiProperty**)&alphaProp)[propID];
	}

	void __fastcall AddProperty(NiProperty *niProperty);
};
static_assert(sizeof(NiGeometry) == 0xC4);

// C4
class NiTriBasedGeom : public NiGeometry
{
public:
	/*F0*/virtual void		Unk_3C(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
};

// C4
class NiTriShape : public NiTriBasedGeom
{
public:
	static NiTriShape *Create(NiTriShapeData *pShapeData) {return ThisCall<NiTriShape*>(0xA74480, Ni_Alloc<NiTriShape>(), pShapeData);}
};

// C4
class NiTriStrips : public NiTriBasedGeom
{
public:
	static NiTriStrips *Create(NiTriStripsData *pShapeData) {return ThisCall<NiTriStrips*>(0xA71CC0, Ni_Alloc<NiTriStrips>(), pShapeData);}
};

// D4
class BSScissorTriShape : public NiTriShape
{
public:
	RECT		scissorRect;	// C4
};
static_assert(sizeof(BSScissorTriShape) == 0xD4);

// C4
class NiScreenElements : public NiTriShape
{
public:
};

// C4
class NiLines : public NiGeometry
{
public:
	static NiLines* __stdcall Create(float length, const NiColorAlpha &color, const char *objName);
};

// D8
class BSResizableTriShape : public NiTriShape
{
public:
	/*F4*/virtual void		Unk_3D();
	/*F8*/virtual void		Unk_3E();

	UInt32		unkC4[5];		// C4
};

// D0
class BSSegmentedTriShape : public NiTriShape
{
public:
	// 14
	struct Segment
	{
		UInt32		startIndex;
		UInt32		numPrimitives;
		UInt32		unk08;
		UInt32		unk0C;
		UInt32		unk10;
	};

	Segment			*segments;		// C4
	UInt32			numSegments;	// C8
	UInt8			byteCC;			// CC
	UInt8			padCD[3];		// CD
};

// 18
class NiPSysModifier : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);

	NiFixedString		name;			// 08
	UInt32				unk0C;			// 0C
	NiParticleSystem	*particleSys;	// 10
	UInt8				byte14;			// 14
	UInt8				pad15[3];		// 15
};
static_assert(sizeof(NiPSysModifier) == 0x18);

// 54
class NiPSysEmitter : public NiPSysModifier
{
public:
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);

	float			speed;					// 18
	float			speedVariation;			// 1C
	float			declination;			// 20
	float			declinationVariation;	// 24
	float			planarAngle;			// 28
	float			planarAngleVariation;	// 2C
	NiColorAlpha	initialColor;			// 30
	float			initialRadius;			// 40
	float			radiusVariation;		// 44
	float			lifespan;				// 48
	float			lifespanVariation;		// 4C
	float			flt50;					// 50
};
static_assert(sizeof(NiPSysEmitter) == 0x54);

// 58
class NiPSysVolumeEmitter : public NiPSysEmitter
{
public:
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);

	NiNode			*emitterObj;	// 54
};

// 64
class NiPSysBoxEmitter : public NiPSysVolumeEmitter
{
public:
	NiVector3		dimensions;	// 58	width, height, depth
};

// 5C
class NiPSysSphereEmitter : public NiPSysVolumeEmitter
{
public:
	float			radius;		// 58
};

// 60
class NiPSysCylinderEmitter : public NiPSysVolumeEmitter
{
public:
	float		flt58;		// 58
	float		flt5C;		// 5C
};

// 88
class NiPSysMeshEmitter : public NiPSysEmitter
{
public:
	// 0C
	class NiSkinnedEmitterData : public NiRefObject
	{
	public:
		UInt32			unk08;
	};

	NiTArray<NiGeometry*>			geometryArr;	// 54
	NiTArray<NiSkinnedEmitterData*>	skinnedDataArr;	// 64
	UInt32							unk74;			// 74
	UInt32							unk78;			// 78
	NiVector3						vec7C;			// 7C
};

// C4
class NiParticles : public NiGeometry
{
public:
	/*F0*/virtual void		Unk_3C(void);
};

// 110
class NiParticleSystem : public NiParticles
{
public:
	/*F4*/virtual void		Unk_3D(void);

	UInt8					byte0C4;	// 0C4
	UInt8					pad0C5[3];	// 0C5
	DList<NiPSysModifier>	modifiers;	// 0C8
	float					flt0D4;		// 0D4
	UInt8					byte0D8;	// 0D8
	UInt8					byte0D9;	// 0D9
	UInt8					pad0DA[2];	// 0DA
	UInt32					unk0DC[13];	// 0DC

	__forceinline static NiParticleSystem *Create() {return CdeclCall<NiParticleSystem*>(0xC1B7F0);}

	NiPSysModifier* __fastcall FindModifier(UInt32 typeVtbl) const;
};
static_assert(sizeof(NiParticleSystem) == 0x110);

// 14C
class ParticleShaderProperty : public BSShaderProperty
{
public:
	void					*ptr060;	// 060
	UInt32					unk064[2];	// 064
	UInt8					byte06C;	// 06C
	UInt8					pad06D[3];	// 06D
	UInt32					unk070;		// 070
	float					flt074[7];	// 074
	UInt32					unk090[6];	// 090
	float					flt0A8[4];	// 0A8
	UInt32					unk0B8[3];	// 0B8
	float					flt0C4[12];	// 0C4
	UInt32					unk0F4[4];	// 0F4
	float					flt104;		// 104
	UInt32					unk108[5];	// 108
	NiTArray<NiAVObject*>	array11C;	// 11C
	UInt32					unk12C;		// 12C
	float					flt130[3];	// 130
	UInt32					unk13C[3];	// 13C
	float					flt148;		// 148
};
static_assert(sizeof(ParticleShaderProperty) == 0x14C);

struct NiVisibleArray
{
	NiGeometry		**m_geo;		// 00
	UInt32			m_numItems;		// 04
	UInt32			m_bufLen;		// 08
	UInt32			m_bufGrowSize;	// 0C
};

// 90
class NiCullingProcess
{
public:
	/*00*/virtual NiRTTI*	GetRTTI();
	/*04*/virtual void		Unk_01(void);	//	Unk_01 through Unk_0F are unused
	/*08*/virtual void		Unk_02(void);
	/*0C*/virtual void		Unk_03(void);
	/*10*/virtual void		Unk_04(void);
	/*14*/virtual void		Unk_05(void);
	/*18*/virtual void		Unk_06(void);
	/*1C*/virtual void		Unk_07(void);
	/*20*/virtual void		Unk_08(void);
	/*24*/virtual void		Unk_09(void);
	/*28*/virtual void		Unk_0A(void);
	/*2C*/virtual void		Unk_0B(void);
	/*30*/virtual void		Unk_0C(void);
	/*34*/virtual void		Unk_0D(void);
	/*38*/virtual void		Unk_0E(void);
	/*3C*/virtual void		Unk_0F(void);
	/*40*/virtual void		Destructor(bool doFree);
	/*44*/virtual void		Process(NiAVObject *pObject);
	/*48*/virtual void		ProcessAlt(NiCamera *camera, NiAVObject *pScene, NiVisibleArray *pVisibleArr);
	/*4C*/virtual void		Append(NiGeometry *arg);

	UInt8				useAppendFn;	// 04 - call AddGeo when true, else just add to the list
	UInt8				pad05[3];		// 05
	NiVisibleArray		*visibleArr;	// 08
	NiCamera			*camera;		// 0C
	NiFrustum			frustum;		// 10
	NiFrustumPlanes		planes;			// 2C
};
static_assert(sizeof(NiCullingProcess) == 0x90);

enum CullingMode : UInt32
{
	kCull_Normal =						0,
	kCull_AllPass =						1,
	kCull_AllFail =						2,
	kCull_IgnoreMultiBounds =			3,
	kCull_ForceMultiBoundsNoUpdate =	4
};

// C8	vtbl @ 0x101E2EC
class BSCullingProcess : public NiCullingProcess
{
public:
	CullingMode				topCullMode;		// 90
	CullingMode				cullModeStack[10];	// 94
	UInt32					stackSize;			// BC
	BSCompoundFrustum		*compFrustum;		// C0
	BSShaderAccumulator		*shaderAccum;		// C4
};
static_assert(sizeof(BSCullingProcess) == 0xC8);

// 90
class BSFadeNodeCuller : public NiCullingProcess
{
public:
};

class ImageSpaceEffect
{
public:
	virtual void Destroy(bool doFree);
	virtual void RenderShader(NiGeometry *apScreenShape, NiDX9Renderer *pkRenderer, ImageSpaceEffectParam *apParam, bool abEndFrame);
	virtual void Setup(ImageSpaceManager *pISManager, ImageSpaceEffectParam *apParam);
	virtual void Shutdown();
	virtual void BorrowTextures(ImageSpaceEffectParam *apParam);
	virtual void ReturnTextures();
	virtual bool IsActive();
	virtual bool UpdateParams(ImageSpaceEffectParam *apParam);

	bool							bIsActive;
	bool							bParamsChanged;
	UInt8							pad02[2];
	NiTArray<ImageSpaceEffect>		pEffectList;
	NiTArray<ImageSpaceShaderParam>	pEffectParamList;
	NiTArray<ImageSpaceTexture>		pTextures;
	NiTArray<int>					pEffectInputs;
	NiTArray<int>					pEffectOutput;
};

enum ImageSpaceEffectEffectID : SInt32
{
	IS_EFFECT_NONE = -1,
	/*00*/IS_EFFECT_BLOOM,
	/*01*/IS_EFFECT_HDR,
	/*02*/IS_EFFECT_REFRACTION,
	/*03*/IS_EFFECT_CINEMATIC,
	/*04*/IS_EFFECT_DEPTH_OF_FIELD,
	/*05*/IS_EFFECT_DEPTH_RADIAL_BLUR,
	/*06*/IS_EFFECT_RADIAL_BLUR,
	/*07*/IS_EFFECT_FULLSCREEN_BLUR,
	/*08*/IS_EFFECT_GET_HIT,
	/*09*/IS_EFFECT_SUNBEAMS,
	/*0A*/IS_EFFECT_INTERFACE,
	/*0B*/IS_EFFECT_VATS_SCAN,
	/*0C*/IS_EFFECT_MOTION_BLUR,
	/*0D*/IS_EFFECT_VOLUMETRIC_FOG,
	/*0E*/IS_EFFECT_MAP,
	/*0F*/IS_EFFECT_MENU_BG,
	/*10*/IS_EFFECT_BLUR_3,
	/*11*/IS_EFFECT_BLUR_5,
	/*12*/IS_EFFECT_BLUR_7,
	/*13*/IS_EFFECT_BLUR_9,
	/*14*/IS_EFFECT_BLUR_11,
	/*15*/IS_EFFECT_BLUR_13,
	/*16*/IS_EFFECT_BLUR_15,
	/*17*/IS_EFFECT_BRIGHTPASS_BLUR_3,
	/*18*/IS_EFFECT_BRIGHTPASS_BLUR_5,
	/*19*/IS_EFFECT_BRIGHTPASS_BLUR_7,
	/*1A*/IS_EFFECT_BRIGHTPASS_BLUR_9,
	/*1B*/IS_EFFECT_BRIGHTPASS_BLUR_11,
	/*1C*/IS_EFFECT_BRIGHTPASS_BLUR_13,
	/*1D*/IS_EFFECT_BRIGHTPASS_BLUR_15,
	/*1E*/IS_EFFECT_TV,
	/*1F*/IS_EFFECT_WATER_FFT,
	/*20*/IS_EFFECT_WATER_DISPLACEMENT,
	/*21*/IS_EFFECT_NOISE,
	/*22*/IS_SHADER_COPY,
	/*23*/IS_SHADER_COPY_RENDERTARGET1,
	/*24*/IS_SHADER_RESTORE_EDRAM,
	/*25*/IS_SHADER_NULL,
	/*26*/IS_SHADER_COPY_ALPHA,
	/*27*/IS_SHADER_COPY_STENCIL,
	/*28*/IS_SHADER_COPY_STENCIL_NO_TEXTURE_NO_DEPTH,
	/*29*/IS_SHADER_REFRACTION,
	/*2A*/IS_SHADER_VOLUMETRIC_FOG,
	/*2B*/IS_SHADER_BLUR,
	/*2C*/IS_SHADER_DOUBLE_VISION,
	/*2D*/IS_SHADER_ALPHA_MULT,
	/*2E*/IS_SHADER_BLEND,
	/*2F*/IS_SHADER_TEXTURE_MASK,
	/*30*/IS_SHADER_MAP,
	/*31*/IS_SHADER_MENU_BG,
	/*32*/IS_SHADER_CINEMATIC,
	/*33*/IS_SHADER_DEPTH_OF_FIELD,
	/*34*/IS_SHADER_DEPTH_OF_FIELD_MASK,
	/*35*/IS_SHADER_MOTION_BLUR,
	/*36*/IS_SHADER_RADIAL_BLUR,
	/*37*/IS_SHADER_RADIAL_BLUR_MEDIUM,
	/*38*/IS_SHADER_RADIAL_BLUR_HIGH,
	/*39*/IS_SHADER_SUNBEAMS,
	/*3A*/IS_SHADER_HDR_BLEND,
	/*3B*/IS_SHADER_HDR_BLEND_CINEMATIC,
	/*3C*/IS_SHADER_HDR_BLEND_CINEMATIC_ALPHA_MASK,
	/*3D*/IS_SHADER_HDR_BLUR,
	/*3E*/IS_SHADER_BRIGHTPASS_FILTER,
	/*3F*/IS_SHADER_DOWNSAMPLE_16,
	/*40*/IS_SHADER_DOWNSAMPLE_9,
	/*41*/IS_SHADER_DOWNSAMPLE_4,
	/*42*/IS_SHADER_DOWNSAMPLE_4_LUM_CLAMP,
	/*43*/IS_SHADER_DOWNSAMPLE_4_LIGHT_ADAPT,
	/*44*/IS_SHADER_DOWNSAMPLE_16_LUM_CLAMP,
	/*45*/IS_SHADER_DOWNSAMPLE_16_LIGHT_ADAPT,
	/*46*/IS_SHADER_LIGHT_ADAPT,
	/*47*/IS_SHADER_LUM_CLAMP,
	/*48*/IS_SHADER_SCANLINES_BLEND,
	/*49*/IS_SHADER_SCANLINES_BLEND_1,
	/*4A*/IS_SHADER_UNK_4A,
	/*4B*/IS_SHADER_VATS_SCAN,
	/*4C*/IS_SHADER_MENU_UNPACK,
	/*4D*/IS_SHADER_TV,
	/*4E*/IS_SHADER_BLUR_3,
	/*4F*/IS_SHADER_BLUR_5,
	/*50*/IS_SHADER_BLUR_7,
	/*51*/IS_SHADER_BLUR_9,
	/*52*/IS_SHADER_BLUR_11,
	/*53*/IS_SHADER_BLUR_13,
	/*54*/IS_SHADER_BLUR_15,
	/*55*/IS_SHADER_BRIGHTPASS_BLUR_3,
	/*56*/IS_SHADER_BRIGHTPASS_BLUR_5,
	/*57*/IS_SHADER_BRIGHTPASS_BLUR_7,
	/*58*/IS_SHADER_BRIGHTPASS_BLUR_9,
	/*59*/IS_SHADER_BRIGHTPASS_BLUR_11,
	/*5A*/IS_SHADER_BRIGHTPASS_BLUR_13,
	/*5B*/IS_SHADER_BRIGHTPASS_BLUR_15,
	/*5C*/IS_SHADER_UNK_5C,
	/*5D*/IS_SHADER_WATER_FFT_WATER_SPECTRUM,
	/*5E*/IS_SHADER_WATER_FFT_HORIZONTAL_BUTTERFLY,
	/*5F*/IS_SHADER_WATER_FFT_VERTICAL_BUTTERFLY,
	/*60*/IS_SHADER_WATER_FFT_HORIZONTAL_SCRAMBLE,
	/*61*/IS_SHADER_WATER_FFT_VERTICAL_SCRAMBLE,
	/*62*/IS_SHADER_WATER_FFT_NORMALS,
	/*63*/IS_SHADER_WATER_FFT_FILTER,
	/*64*/IS_SHADER_WATER_FFT_DISPLAY_NORMALS,
	/*65*/IS_SHADER_WATER_FFT_DISPLAY_HIGHW,
	/*66*/IS_SHADER_WATER_FFT_DISPLAY_AMPLITUTE,
	/*67*/IS_SHADER_WATER_FFT_WATER_SPECTRUM_1,
	/*68*/IS_SHADER_WATER_FFT_DISPLAY_WATER_HEIGHT,
	/*69*/kWaterDisplaceClearSimulation,
	/*6A*/kWaterDisplaceTexOffset,
	/*6B*/kWaterDisplacementWadingRipple,
	/*6C*/kWaterDisplacementRainRipple,
	/*6D*/kWaterWadingHeightmap,
	/*6E*/kWaterRainHeightmap,
	/*6F*/kWaterBlendHeightmaps,
	/*70*/kWaterSmoothHeightmap,
	/*71*/kWaterDisplacementNormals,
	/*72*/IS_SHADER_NOISE_SCROLL_AND_BLEND,
	/*73*/IS_SHADER_NOISE_NORMAL_MAP
};

// 31C
class ImageSpaceManager
{
public:
	BSRenderedTexture			*pMainTarget;
	NiTArray<ImageSpaceEffect*>	effectList;
	UInt32						unk14[194];

	__forceinline static ImageSpaceManager *GetSingleton() {return *(ImageSpaceManager**)0x11F91AC;}
};
static_assert(sizeof(ImageSpaceManager) == 0x31C);

#define IMAGESPACEMANAGER ds:0x11F91AC

struct TextureParams
{
	UInt32						width;		// 00
	UInt32						height;		// 04
	D3DFORMAT					d3dFormat;	// 08
	RenderMode					renderMode;	// 0C
	UInt32						bgColorMask;// 10
	ImageSpaceEffectEffectID	isEffect;	// 14

	TextureParams(UInt32 _width, UInt32 _height, D3DFORMAT _d3dFmt, RenderMode _renderMode = kRndrMode_Normal, UInt32 _bgColorMask = 0xFFFFFFFF, ImageSpaceEffectEffectID _isEffect = IS_EFFECT_NONE) :
		width(_width), height(_height), d3dFormat(_d3dFmt), renderMode(_renderMode), bgColorMask(_bgColorMask), isEffect(_isEffect) {}
};

// B0
class BSTextureManager
{
public:
	struct TextureHandle
	{
		BSRenderedTexture	*texture;
		D3DFORMAT			d3dFormat;
		TextureHandle		*next;
		UInt32				uiFlags;
		bool				byte10;
		UInt8				pad11[3];
		RenderTargetTypes	renderTargetType;
	};

	DList<void>					unkLists[5];			// 00
	DList<BSRenderedTexture>	shadowFrustumTextures;	// 3C
	DList<BSRenderedTexture>	waterDepthTextures;		// 48
	DList<BSRenderedTexture>	waterReflectionTextures;// 54
	DList<BSRenderedTexture>	waterBlurTextures;		// 60
	DList<BSRenderedTexture>	waterNoiseTextures;		// 6C
	DList<TextureHandle>		textureLists[2];		// 78
	DList<BSRenderedTexture>	shadowMapLists[2];		// 90
	NiDepthStencilBuffer		*depthStencilBuffer;	// A8
	NiObject					*objAC;					// AC

	__forceinline static BSTextureManager *GetSingleton() {return *(BSTextureManager**)0x11F91A8;}

	static void __stdcall GenerateRenderedTexture(NiCamera *camera, const TextureParams &texParams, NiTexture **pTexture);
	static void __stdcall GenerateRenderedUITexture(NiNode *tileNode, const NiVector4 &scrArea, NiTexture **pTexture);
};
static_assert(sizeof(BSTextureManager) == 0xB0);

#define BSTEXTUREMANAGER ds:0x11F91A8

// 08
class MobIterOperator
{
public:
	virtual void	Operate(Actor *actor);

	UInt32			unk04;		// 04
};

// A4
class HighActorCuller : public MobIterOperator
{
public:
	BSFadeNodeCuller	fadeNodeCuller;		// 08
	BSMultiBoundNode	*mbNode;			// 98
	BSMultiBound		*multiBound;		// 9C
	BSMultiBoundSphere	*mbSphere;			// A0

	__forceinline static HighActorCuller *GetSingleton() {return (HighActorCuller*)0x11DEF10;}

	__forceinline static void Run(NiCamera *camera) {ThisCall(0x8D6F50, GetSingleton(), camera);}
};
static_assert(sizeof(HighActorCuller) == 0xA4);

// 34
class NiPick
{
public:
	NiPick(NiAVObject *origin)
	{
		ThisCall(0xE98F20, this, 0, 8);
		InterlockedIncrement(&origin->m_uiRefCount);
		originObj = origin;
	}
	~NiPick() {ThisCall(0xE98FA0, this);}

	// 44
	struct Result
	{
		NiAVObject		*result;	// 00
		NiObject		*proxyParent;	// 04
		NiVector3		intersect;	// 08
		float			distance;	// 14
		UInt16			triangleIdx;// 18
		UInt16			vertexIdx;	// 1E
		NiPoint2		uvCoords;	// 20
		NiVector3		normal;		// 28
		NiColorAlpha	colour;		// 34
	};

	enum PickType : UInt32
	{
		PICK_ALL,
		PICK_FIRST
	};
	enum SortType : UInt32
	{
		SORT_YES,
		SORT_NO
	};
	enum IntersectType : UInt32
	{
		INTERSECT_BOUND,
		INTERSECT_TRIANGLE
	};
	enum CoordinateType : UInt32
	{
		COORDS_MODEL,
		COORDS_WORLD
	};

	PickType			pickType;		// 00
	SortType			sortType;		// 04
	IntersectType		intersectType;	// 08
	CoordinateType		coordType;		// 0C
	UInt8				bFrontOnly;		// 10
	UInt8				observeCull;	// 11
	UInt8				pad12[2];		// 12
	NiAVObject			*originObj;		// 14
	NiTArray<Result*>	results;		// 18
	UInt32				numResults;		// 28
	UInt32				lastAddedRecord;// 2C
	UInt8				retnTexture;	// 30
	UInt8				retnNormal;		// 31
	UInt8				retnSmoothNormal;	// 32
	UInt8				retnColour;		// 33

	bool __fastcall GetResults(NiCamera *camera);
};