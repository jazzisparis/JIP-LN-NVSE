#pragma once

class NiMultiTargetTransformController;
class NiTextKeyExtraData;
class NiDefaultAVObjectPalette;
class NiLight;
class BSCubeMapCamera;
class NiFrustumPlanes;
class NiCullingProcess;
class NiSkinPartition;
class NiSkinInstance;
class NiSourceCubeMap;
class NiRenderedCubeMap;
class NiDepthStencilBuffer;
class NiRenderTargetGroup;
class RenderTarget;
class NiGeometryData;
class NiParticles;
class NiLines;
class IDirect3DDevice9;
class NiDX9RenderState;
class NiUnsharedGeometryGroup;
class BSPortalGraph;
class BSDismemberSkinInstance;

typedef FixedTypeArray<hkpWorldObject*, 0x40> ContactObjects;

class NiFixedString
{
	const char		*str;

	void Set(const char *inStr)
	{
		str = inStr;
		if (str) InterlockedIncrement((UInt32*)(str - 8));
	}

	void Unset()
	{
		if (str)
		{
			InterlockedDecrement((UInt32*)(str - 8));
			str = nullptr;
		}
	}

public:
	NiFixedString() : str(nullptr) {}
	NiFixedString(const char *inStr)
	{
		str = (inStr && *inStr) ? CdeclCall<const char*>(0xA5B690, inStr) : nullptr;
	}
	NiFixedString(const NiFixedString &inStr) {Set(inStr.str);}
	~NiFixedString() {Unset();}

	UInt32 Length() const {return str ? *(UInt32*)(str - 4) : 0;}

	explicit operator bool() const {return str != nullptr;}

	operator const char*() const {return str;}

	inline void operator=(const char *inStr)
	{
		Unset();
		if (inStr && *inStr)
			str = CdeclCall<const char*>(0xA5B690, inStr);
	}
	inline void operator=(const NiFixedString &inStr)
	{
		if (str != inStr.str)
			Set(inStr.str);
	}

	inline bool operator==(const NiFixedString &rhs) const {return str == rhs.str;}
	inline bool operator<(const NiFixedString &rhs) const {return str < rhs.str;}

	UInt32 RefCount() const {return str ? *(UInt32*)(str - 8) : 0;}
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
	NiTMapBase<NiObject*, NiObject*>	*map00;		// 00
	NiTMapBase<NiObject*, bool>			*map04;		// 04
	UInt32								unk08;		// 08	Init'd to 1
	UInt8								byte0C;		// 0C	Init'd to 0x24
	UInt8								pad0D[3];	// 0D
	NiVector3							scale;		// 10	Appears to be unused

	NiObjectCopyInfo(UInt32 bucketCount)
	{
		ThisCall(0x4AD0C0, this, bucketCount);
		scale = {1.0F, 1.0F, 1.0F};
	}
	//~NiObjectCopyInfo() {ThisCall(0x4AD1D0, this);}
};

struct UpdateParams
{
	float		timePassed;
	bool		updateControllers;
	UInt8		byte05;
	UInt8		byte06;
	UInt8		byte07;
	UInt8		byte08;
	UInt8		pad09[3];

	UpdateParams() {ZeroMemory(this, sizeof(UpdateParams));}
};

extern const UpdateParams kUpdateParams;

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
	/*4C*/virtual void		Load(UInt32 arg);
	/*50*/virtual void		Unk_14(UInt32 arg);
	/*54*/virtual void		Unk_15(UInt32 arg);
	/*58*/virtual void		Unk_16(UInt32 arg);
	/*5C*/virtual void		Unk_17(UInt32 arg);
	/*60*/virtual void		Unk_18(UInt32 arg);
	/*64*/virtual void		Unk_19(UInt32 arg);
	/*68*/virtual void		Unk_1A(NiObjectCopyInfo *copyInfo);
	/*6C*/virtual void		Unk_1B(UInt32 arg);
	/*70*/virtual void		Unk_1C(void);
	/*74*/virtual void		Unk_1D(void);
	/*78*/virtual void		SetWorldBound(NiSphere *worldBound);
	/*7C*/virtual UInt32	Unk_1F(void);
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
	void					*rotationKeys;		// 20
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

// 0C
class NiInterpolator : public NiObject
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
	/*F8*/virtual void		Unk_3E(void);

	float			flt0C;		// 0C
	float			flt10;		// 10
	float			flt14;		// 14
	float			flt18;		// 18
	float			flt1C;		// 1C
	float			flt20;		// 20
	float			flt24;		// 24
	float			flt28;		// 28
	NiTransformData	*transData;	// 2C
	UInt16			unk30;		// 30
	UInt16			unk32;		// 32
	UInt16			unk34;		// 34
	UInt8			pad36[2];	// 36
	float			flt38;		// 38
	float			flt3C;		// 3C
	float			flt40;		// 40
	UInt8			byte44;		// 44
	UInt8			pad45[3];	// 45

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

// 74
class NiControllerSequence : public NiObject
{
public:
	/*8C*/virtual bool		Unk_23(float arg1, UInt8 arg2);

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
	UInt32					unk60;					// 60
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

// 34
class NiTimeController : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Update(UpdateParams &updParams);
	/*98*/virtual void		SetTarget(NiNode *pTarget);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(void);

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
	NiNode				*target;			// 2C
	NiTimeController	*nextCtrl;			// 30
};

// 7C
class NiControllerManager : public NiTimeController
{
public:
	/*B4*/virtual void		Unk_2D(void);

	NiTArray<NiControllerSequence*>				sequences;		// 34
	void										*ptr44;			// 44
	UInt32										unk48;			// 48
	UInt32										unk4C;			// 4C
	NiTStringPointerMap<NiControllerSequence*>	seqStrMap;		// 50	Case-sensitive!
	NiTArray<void*>								*arr64;			// 64
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

// 38
class NiSingleInterpController : public NiInterpController
{
public:
	/*E8*/virtual void		Unk_3A(void);

	NiInterpolator		*interpolator;		// 34
};

// 38
class NiTransformController : public NiSingleInterpController
{
public:
	static NiTransformController* __stdcall Create(NiNode *pTarget, NiTransformInterpolator *pInterpolator);
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
		kBSXFlag_Animated =					1 << 0,
		kBSXFlag_Havok =					1 << 1,
		kBSXFlag_Ragdoll =					1 << 2,
		kBSXFlag_Complex =					1 << 3,
		kBSXFlag_Addon =					1 << 4,
		kBSXFlag_EditorMarker =				1 << 5,
		kBSXFlag_Dynamic =					1 << 6,
		kBSXFlag_Articulated =				1 << 7,
		kBSXFlag_NeedsTransformUpdates =	1 << 8,
		kBSXFlag_ExternalEmit =				1 << 9,
	};

	UInt32			flags;		// 0C
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
};

// 38
class BSShaderTextureSet : public BSTextureSet
{
public:
	String		textures[6];	// 08
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

	const char *GetName() const {return m_blockName ? m_blockName : "NULL";}
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
	/*90*/virtual void		UpdateController(UpdateParams &updParams);

	enum PropertyType
	{
		kPropertyType_Alpha =			0,
		kPropertyType_Culling =			1,
		kPropertyType_Material =		2,
		kPropertyType_Shade =			3,
		kPropertyType_TileShader =		kPropertyType_Shade,
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
};
static_assert(sizeof(NiStencilProperty) == 0x24);

// 1C
class NiCullingProperty : public NiProperty
{
public:
	UInt32				unk18;		// 18
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

	UInt16			unk18;				// 18
	UInt16			unk1A;				// 1A
	UInt32			shaderType;			// 1C
	UInt32			flags1;				// 20
	UInt32			flags2;				// 24
	float			alpha;				// 28
	float			fadeAlpha;			// 2C
	float			envMapScale;		// 30
	float			flt34;				// 34
	UInt32			lastRenderPassState;// 38
	void			*renderPassList;	// 3C	Seen 010B8480
	UInt32			unk40;				// 40
	UInt32			unk44;				// 44
	UInt32			unk48;				// 48
	UInt32			unk4C;				// 4C
	UInt32			unk50;				// 50
	UInt32			unk54;				// 54
	UInt32			unk58;				// 58
	float			flt5C;				// 5C
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
	UInt32			unk64;					// 64
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

	UInt32			unk60[3];		// 60
	float			flt6C;			// 6C
	UInt32			unk70;			// 70
	UInt8			byte74;			// 74
	UInt8			pad75[3];		// 75
	UInt32			unk78;			// 78
};
static_assert(sizeof(BSShaderLightingProperty) == 0x7C);

// 104
class BSShaderPPLightingProperty : public BSShaderLightingProperty
{
public:
	/*0CC*/virtual void		Unk_33(void);
	/*0D0*/virtual void		Unk_34(void);
	/*0D4*/virtual void		Unk_35(void);
	/*0D8*/virtual void		Unk_36(void);
	/*0DC*/virtual void		Unk_37(void);
	/*0E0*/virtual void		Unk_38(void);
	/*0E4*/virtual void		Unk_39(void);
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
	float				flt084[8];				// 084
	BSShaderTextureSet	*textureSet;			// 0A4
	UInt16				word0A8;				// 0A8
	UInt16				word0AA;				// 0AA
	NiSourceTexture		**srcTextures[6];		// 0AC
	void				*ptr0C4;				// 0C4
	UInt32				unk0C8;					// 0C8
	void				*ptr0CC;				// 0CC
	NiObject			*object0D0;				// 0D0
	UInt32				unk0D4;					// 0D4
	UInt32				unk0D8;					// 0D8
	NiObject			*object0DC;				// 0DC
	float				refractionStrength;		// 0E0
	SInt32				refractionFirePeriod;	// 0E4
	float				parallaxMaxPasses;		// 0E8
	float				parallaxScale;			// 0EC
	UInt32				unk0F0[4];				// 0F0
	void				*ptr100;				// 100
};
static_assert(sizeof(BSShaderPPLightingProperty) == 0x104);

// 150
class WaterShaderProperty : public BSShaderProperty
{
public:
	UInt8				byte060;			// 060
	UInt8				byte061;			// 061
	UInt8				byte062;			// 062
	UInt8				bDepth;				// 063
	UInt32				unk064;				// 064
	UInt32				unk068;				// 068
	float				flt06C;				// 06C
	float				flt070;				// 070
	float				fogDistanceFar;		// 074
	float				fogDistanceNear;	// 078
	UInt8				byte07C;			// 07C
	UInt8				byte07D;			// 07D
	UInt8				byte07E;			// 07E
	UInt8				useDefaultWater;	// 07F
	UInt8				bReflections;		// 080
	UInt8				bRefractions;		// 081
	UInt8				bTextureCoords;		// 082
	UInt8				byte083;			// 083
	UInt32				unk084;				// 084
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
	float				fresnel;			// 118
	float				noiseScale;			// 11C
	float				fog;				// 120
	float				flt124;				// 124
	UInt32				unk128[3];			// 128
	NiSourceTexture		*srcTexture;		// 134
	RenderTarget		*noDepth;			// 138
	RenderTarget		*reflections;		// 13C
	RenderTarget		*refractions;		// 140
	RenderTarget		*depth;				// 144
	UInt32				unk148;				// 148
	UInt32				unk14C;				// 14C
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

// 9C
class NiAVObject : public NiObjectNET
{
public:
	/*8C*/virtual void		UpdateControllers(const UpdateParams &updParams);
	/*90*/virtual void		Unk_24(NiMatrix33 *arg1, NiVector3 *arg2, bool arg3);
	/*94*/virtual void		Unk_25(UInt32 arg1);
	/*98*/virtual void		Unk_26(UInt32 arg1);
	/*9C*/virtual NiAVObject	*GetObjectByName(NiFixedString *objName);
	/*A0*/virtual void		SetSelectiveUpdateFlags(UInt8 *bSelectiveUpdate, UInt32 bSelectiveUpdateTransform, UInt8 *bRigid);
	/*A4*/virtual void		UpdateDownwardPass(const UpdateParams &updParams, UInt32 flags);
	/*A8*/virtual void		UpdateSelectedDownwardPass(const UpdateParams &updParams, UInt32 flags);
	/*AC*/virtual void		UpdateRigidDownwardPass(const UpdateParams &updParams, UInt32 flags);
	/*B0*/virtual void		Unk_2C(UInt32 arg1);
	/*B4*/virtual void		Unk_2D(UInt32 arg1);
	/*B8*/virtual void		UpdateTransform(const UpdateParams &updParams);
	/*BC*/virtual void		UpdateWorldBound();
	/*C0*/virtual void		UpdateBounds(const UpdateParams &updParams);
	/*C4*/virtual void		PreAttachUpdate(NiNode *parent, const UpdateParams &updParams);
	/*C8*/virtual void		Unk_32(NiNode *parent);
	/*CC*/virtual void		Unk_33(UInt32 arg1);
	/*D0*/virtual void		PostAttachUpdate();
	/*D4*/virtual void		UpdateFadeAmount(void *arg1);
	/*D8*/virtual void		Unk_36(UInt32 arg1);

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
		kNiFlag_NoDismember =				0x00100000,
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
	NiSphere				*m_kWorldBound;			// 20
	DList<NiProperty>		m_propertyList;			// 24
	UInt32					m_flags;				// 30
	NiTransform				m_transformLocal;		// 34
	NiTransform				m_transformWorld;		// 68

	inline NiMatrix33& LocalRotate() {return m_transformLocal.rotate;}
	inline NiMatrix33& WorldRotate() {return m_transformWorld.rotate;}
	inline NiVector3& LocalTranslate() {return m_transformLocal.translate;}
	inline NiVector3& WorldTranslate() {return m_transformWorld.translate;}

	NiAVObject *CreateCopy();
	void Update();
	UInt32 GetIndex() const;
	bool ReplaceObject(NiAVObject *object);
	NiProperty* __fastcall GetProperty(UInt32 propID) const;
	__forceinline void AddProperty(NiProperty *niProperty) {ThisCall(0x439410, this, niProperty);}
	TESObjectREFR *GetParentRef() const;
	__forceinline void RenderGeometryAndShader() {CdeclCall(0xB57E30, this, 0, 0);}

	void DumpObject();
	void DumpParents();
};

// AC
class NiNode : public NiAVObject
{
public:
	/*DC*/virtual void		AddObject(NiAVObject *object, bool arg2);
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
	bool IsMovable();
	void ResetCollision();
	void RemoveCollision();
	BSXFlags *GetBSXFlags();
	void BulkSetMaterialPropertyTraitValue(UInt32 traitID, float value);
	void GetContactObjects(ContactObjects &contactObjs);
	bool HasPhantom();
	float __vectorcall GetBodyMass(float totalMass) const;
	void ApplyForce(const NiVector4 &forceVector);
	void Dump();
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
};
static_assert(sizeof(BSFadeNode) == 0xE4);

// B4
class BSMultiBoundNode : public NiNode
{
public:
	/*100*/virtual void		Unk_40(UInt32 arg1, UInt32 arg2);
	/*104*/virtual void		Unk_41(void);
	/*108*/virtual void		Unk_42(UInt32 arg1);
	/*10C*/virtual void		Unk_43(UInt32 arg1);
	/*110*/virtual void		Unk_44(UInt32 arg1);

	UInt32			unkAC[2];		// AC
};

// B8
class BSParticleSystemManager : public NiNode
{
public:
	/*100*/virtual void		Unk_40(void);

	UInt32			unkAC[3];		// AC
};

// B4
class NiBillboardNode : public NiNode
{
public:
	/*100*/virtual void		Unk_40(void);

	UInt32			unkAC[2];		// AC
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
class BSTempNodeManager : public NiNode
{
public:
	UInt32			unkAC;		// AC
};

// 64
class BSFogProperty : public NiProperty
{
public:
	UInt16				flags;		// 18
	UInt16				unk1A;		// 1A
	float				flt1C;		// 1C
	NiColor				color;		// 20
	float				distNear;	// 2C
	float				distFar;	// 30
	UInt32				unk34;		// 34
	UInt32				unk38;		// 38
	float				flt3C;		// 3C
	float				flt40;		// 40
	float				flt44;		// 44
	float				flt48;		// 48
	UInt32				unk4C;		// 4C
	UInt32				unk50;		// 50
	float				flt54;		// 54
	float				flt58;		// 58
	float				flt5C;		// 5C
	float				power;		// 60
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

// 250
class LightingData : public NiRefObject
{
public:
	UInt32					unk008;			// 008
	float					flt00C[53];		// 00C
	DList<NiTriStrips>		lgtList0E0;		// 0E0
	UInt8					byte0EC;		// 0EC
	UInt8					byte0ED;		// 0ED
	UInt8					pad0EE[2];		// 0EE
	NiRefObject				*obj0F0;		// 0F0
	UInt8					isPointLight;	// 0F4
	UInt8					isAmbientLight;	// 0F5
	UInt8					pad0F6[2];		// 0F6
	NiLight					*light;			// 0F8
	UInt8					isDynamic;		// 0FC
	UInt8					pad0FD[3];		// 0FD
	NiVector3				centrePos;		// 100
	NiRefObject				*obj10C;		// 10C
	UInt16					state;			// 110	0xFF = disabled (distance too far), anything else = enabled
	UInt16					word112;		// 112
	NiRefObject				*obj114;		// 114
	UInt8					byte118;		// 118
	UInt8					pad119[3];		// 119
	float					flt11C;			// 11C
	float					flt120;			// 120
	UInt8					byte124;		// 124
	UInt8					pad125[3];		// 125
	NiRefObject				*obj128;		// 128
	UInt32					unk12C[3];		// 12C
	NiRefObject				*obj138;		// 138
	NiRefObject				*obj13C;		// 13C
	NiRefObject				*obj140;		// 140
	UInt32					unk144[53];		// 144
	UInt8					byte208;		// 208
	UInt8					pad209[3];		// 209
	NiRefObject				*obj20C;		// 20C
	UInt32					unk210[4];		// 210
	BSSimpleArray<NiNode*>	array230;		// 230
	BSPortalGraph			*portalGraph;	// 240
	UInt32					unk244[3];		// 244
};
static_assert(sizeof(LightingData) == 0x250);

// 200
class ShadowSceneNode : public NiNode
{
public:
	UInt32							unk0AC[2];		// 0AC
	DList<LightingData>				lgtList0B4;		// 0B4
	DList<LightingData>				lgtList0C0;		// 0C0
	UInt32							unk0CC;			// 0CC
	DListNode<LightingData>			*node0D0;		// 0D0
	DListNode<LightingData>			*node0D4;		// 0D4
	LightingData					*data0D8;		// 0D8
	LightingData					*data0DC;		// 0DC
	LightingData					*data0E0;		// 0E0
	DList<LightingData>				lgtList0E4;		// 0E4
	DList<LightingData>				list0F0;		// 0F0
	DList<LightingData>				list0FC;		// 0FC
	DList<void>						list108;		// 108
	DList<void>						list114;		// 114
	UInt32							unk120;			// 120
	UInt32							unk124;			// 124
	BSCubeMapCamera					*cubeMapCam;	// 128
	UInt32							unk12C;			// 12C
	UInt8							byte130;		// 130
	UInt8							byte131;		// 131
	UInt8							pad132[2];		// 132
	BSFogProperty					*fogProperty;	// 134
	UInt32							unk138;			// 138
	BSSimpleArray<NiFrustumPlanes*>	array13C;		// 13C
	BSSimpleArray<void*>			array14C;		// 14C	010C1E9C
	UInt32							unk15C[3];		// 15C
	NiVector4						unk168;			// 168
	NiVector4						unk178;			// 178
	NiVector4						unk188;			// 188
	NiVector4						unk198;			// 198
	NiVector4						unk1A8;			// 1A8
	NiVector4						unk1B8;			// 1B8
	UInt32							lightingPasses;	// 1C8
	float							flt1CC[3];		// 1CC
	UInt32							unk1D8;			// 1D8
	UInt8							byte1DC;		// 1DC
	UInt8							pad1DD[3];		// 1DD
	BSPortalGraph					*portalGraph;	// 1E0
	UInt32							unk1E4[3];		// 1E4
	NiVector3						cameraPos;		// 1F0
	UInt8							byte1FC;		// 1FC
	UInt8							pad1FD[3];		// 1FD
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
	UInt32			unkA0;			// A0
	UInt32			unkA4;			// A4
	UInt32			unkA8;			// A8
	DList<void>		listAC;			// AC
	DList<void>		listB8;			// B8
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
};

// 15C
class NiTextureEffect : public NiDynamicEffect
{
public:
	NiMatrix33		matrix0C4;		// 0C4
	float			flt0E8;			// 0E8
	float			flt0EC;			// 0EC
	float			flt0F0;			// 0F0
	NiMatrix33		matrix0F4;		// 0F4
	float			flt118;			// 118
	float			flt11C;			// 11C
	float			flt120;			// 120
	UInt32			unk124[5];		// 124
	UInt8			byte138;		// 138
	UInt8			pad139[3];		// 139
	float			flt13C[4];		// 13C
	float			flt14C[4];		// 14C
};

class BSSceneGraph : public NiNode
{
public:
	/*100*/virtual void		Unk_40(void);
	/*104*/virtual void		Unk_41(void);
};

// C0
class SceneGraph : public BSSceneGraph
{
public:
	NiCamera			*camera;			// AC
	UInt32				unkB0;				// B0
	NiCullingProcess	*cullingProc;		// B4
	UInt32				isMinFarPlaneDist;	// B8 The farplane is set to 20480.0 when the flag is true. Probably used for interiors.
	float				cameraFOV;			// BC
};

extern SceneGraph *g_sceneGraph;

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
	UInt8			groupIndex;		// 10
	UInt8			pad11[3];		// 11
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

// 14
class Ni2DBuffer : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);

	UInt32		unk08[3];		// 08
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

	UInt8			byte08;			// 08
	UInt8			pad09[3];		// 09
	Ni2DBuffer		*buffers[4];	// 0C
	UInt32			unk1C;			// 1C
	Ni2DBuffer		*stencilBuffer;	// 20
	void			*ptr24;			// 24
};

// 210
class NiRenderer : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);

	UInt32			unk008[126];	// 008
	UInt32			sceneState;		// 200
	UInt32			unk204;			// 204
	UInt32			unk208;			// 208
	UInt32			unk20C;			// 20C
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

class NiDX9Renderer : public NiRenderer
{
public:
	enum ClearFlags
	{
		kClear_NONE =			0,
		kClear_BACKBUFFER =		1,
		kClear_STENCIL =		2,
		kClear_ZBUFFER =		4,
		kClear_ALL =			kClear_BACKBUFFER | kClear_STENCIL | kClear_ZBUFFER
	};

	virtual void		Unk_24(void);
	virtual void		Unk_25(void);
	virtual void		Unk_26(void);
	virtual void		Unk_27(void);
	virtual void		Unk_28(void);
	virtual void		Unk_29(void);
	virtual void		Unk_2A(void);
	virtual void		SetBackgroundColor(NiVector4 *inARGB);
	virtual void		Unk_2C(void);
	virtual void		GetBackgroundColor(NiVector4 *outARGB);
	virtual void		Unk_2E(void);
	virtual void		Unk_2F(void);
	virtual void		Unk_30(void);
	virtual void		Unk_31(void);
	virtual NiRenderTargetGroup *GetDefaultRT();	// get back buffer rt
	virtual NiRenderTargetGroup *GetCurrentRT();	// get currentRTGroup
	virtual void		Unk_34(void);
	virtual void		Unk_35(void);
	virtual void		Unk_36(void);
	virtual void		Unk_37(void);
	virtual void 		Unk_38(void);
	virtual void 		Unk_39(void);
	virtual void		Unk_3A(void);
	virtual void		Unk_3B(void);
	virtual void		PurgeGeometry(NiGeometryData *geo);
	virtual void		PurgeMaterial(NiMaterialProperty *material);
	virtual void		PurgeEffect(NiDynamicEffect *effect);
	virtual void		PurgeScreenTexture();
	virtual void		PurgeSkinPartition(NiSkinPartition *skinPartition);
	virtual void		PurgeSkinInstance(NiSkinInstance *skinInstance);
	virtual void		Unk_42(void);
	virtual bool		Unk_43(void);
	virtual void		Unk_44(void);
	virtual bool		FastCopy(void *src, void *dst, RECT *srcRect, SInt32 xOffset, SInt32 yOffset);
	virtual bool		Copy(void *src, void *dst, RECT *srcRect, RECT *dstRect, UInt32 filterMode);
	virtual void		Unk_47(void);
	virtual bool		Unk_48(void *arg);
	virtual void		Unk_49(void);
	virtual void		Unk_4A(float arg);
	virtual void 		Unk_4B(UInt32 size);
	virtual void		Unk_4C(UInt32 arg0, UInt32 arg1);
	virtual void		Unk_4D(UInt32 arg0, UInt32 arg1);
	virtual void		Unk_4E(void *buf);
	virtual void		CreateSourceTexture(NiSourceTexture *texture);
	virtual bool		CreateRenderedTexture(NiRenderedTexture *arg);
	virtual bool		CreateSourceCubeMap(NiSourceCubeMap *arg);
	virtual bool		CreateRenderedCubeMap(NiRenderedCubeMap *arg);
	virtual bool		CreateDynamicTexture(void *arg);
	virtual void		Unk_54(void);
	virtual bool		CreateDepthStencil(NiDepthStencilBuffer *arg, void *textureFormat);
	virtual void		Unk_56(void);
	virtual void		Unk_57(void);
	virtual void		Unk_58(void);
	virtual void		Unk_59(void);
	virtual void		Unk_5A(void);
	virtual void		Unk_5B(void);
	virtual void		Unk_5C(void);
	virtual void		Unk_5D(void);
	virtual void		Unk_5E(void);
	virtual bool		BeginScene();
	virtual bool		EndScene();
	virtual void		DisplayScene();
	virtual void		Clear(float *rect, UInt32 flags);
	virtual void		SetupCamera(NiVector3 * pos, NiVector3 * at, NiVector3 * up, NiVector3 * right, NiFrustum * frustum, float * viewport);
	virtual void		SetupScreenSpaceCamera(float* viewport);
	virtual bool		BeginUsingRenderTargetGroup(NiRenderTargetGroup* renderTarget, ClearFlags clearFlags);
	virtual bool		EndUsingRenderTargetGroup();
	virtual void		BeginBatch(UInt32 arg0, UInt32 arg1);
	virtual void		EndBatch();
	virtual void		BatchRenderShape(void *arg);
	virtual void		BatchRenderStrips(void *arg);
	virtual void		RenderTriShape(NiTriShape *obj);
	virtual void		RenderTriStrips(NiTriStrips *obj);
	virtual void		RenderTriShape2(NiTriShape *obj);
	virtual void		RenderTriStrips2(NiTriStrips *obj);
	virtual void		RenderParticles(NiParticles *obj);
	virtual void		RenderLines(NiLines *obj);
	virtual void		RenderScreenTexture();

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
	NiTPointerMap<PrePackObject*>		prePackObjects;				// 610 - NiTPointerMap <NiVBBlock *, NiDX9Renderer::PrePackObject *>
	UInt32								unk620[153];				// 620
	NiRenderTargetGroup					*defaultRTGroup;			// 884 - back buffer
	NiRenderTargetGroup					*currentRTGroup;			// 888
	NiRenderTargetGroup					*currentscreenRTGroup;		// 88C
	NiTPointerMap<NiRenderTargetGroup*>	screenRTGroups;				// 890 - NiTPointerMap <HWND *, NiPointer <NiRenderTargetGroup> >
	UInt32								unk8A0[6];					// 8A0
	NiDX9RenderState					*renderState;				// 8B8
	UInt32								unk8BC[3];					// 8BC
	NiTMapBase<NiLight*, void*>			*lightsMap;					// 8C8
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
	UInt32								unkAD0[44];					// AD0
};
static_assert(sizeof(NiDX9Renderer) == 0xB80);

class IDirect3DBaseTexture9;

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
	UInt32					unk0C[6];		// 0C
	UInt32					unk24;			// 24
	UInt32					unk28[14];		// 28
	NiDX9Renderer			*renderer;		// 60
	IDirect3DBaseTexture9	*d3dBaseTexture;// 64
	UInt32					unk68;			// 68
	NiObject				*object6C;		// 6C
};

// 30
class NiTexture : public NiObjectNET
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);

	enum
	{
		kPxlLayout_Palette8BPP =	0,
		kPxlLayout_Raw16BPP,
		kPxlLayout_Raw32BPP,
		kPxlLayout_Compressed,
		kPxlLayout_Bumpmap,
		kPxlLayout_Palette4BPP,
		kPxlLayout_Default,

		kAlphaFmt_None =			0,
		kAlphaFmt_Binary1BPP,
		kAlphaFmt_Smooth8BPP,
		kAlphaFmt_Default,

		kMipMapFmt_Disabled =		0,
		kMipMapFmt_Enabled,
		kMipMapFmt_Default,
	};

	UInt32				pixelLayout;	// 18
	UInt32				alphaFormat;	// 1C
	UInt32				mipmapFormat;	// 20
	NiDX9TextureData	*textureData;	// 24
	NiTexture			*prev;			// 28
	NiTexture			*next;			// 2C
};

// 48
class NiSourceTexture : public NiTexture
{
public:
	/*A4*/virtual void		Unk_29(void);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);

	NiFixedString	ddsPath1;		// 30
	NiFixedString	ddsPath2;		// 34
	UInt32			unk38;			// 38
	UInt32			unk3C;			// 3C
	UInt8			byte40;			// 40
	UInt8			byte41;			// 41
	UInt8			byte42;			// 42
	UInt8			byte43;			// 43
	UInt32			unk44;			// 44

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
	/*A4*/virtual void		Unk_29(void);

	Ni2DBuffer			*buffer;	// 30
	UInt32				unk34;		// 34
	UInt32				unk38;		// 38
	UInt32				unk3C;		// 3C
	UInt8				byte40;		// 40
	UInt8				byte41;		// 41
	UInt8				pad42[2];	// 42
	UInt32				unk44;		// 44

	void __fastcall SaveToFile(UInt32 fileFmt, char *filePath);
};

// 40
class RenderTarget : public NiObject	//	0x10AE500
{
public:
	NiRenderTargetGroup		*targetGroup;	// 08
	NiObject				*object0C[5];	// 0C
	NiObject				*object20;		// 20
	NiObject				*object24;		// 24
	NiObject				*object28;		// 28
	UInt32					unk2C;			// 2C
	NiRenderedTexture		*textures[4];	// 30
};

class NiAccumulator : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(void);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(UInt32 arg);
	/*9C*/virtual void		Unk_27(void);
	/*A0*/virtual void		Unk_28(void);
};

class NiBackToFrontAccumulator : public NiAccumulator
{
public:
	/*A4*/virtual void		Unk_29(void);
};

// 34
class NiAlphaAccumulator : public NiBackToFrontAccumulator
{
public:
	UInt32			unk08[11];		// 08
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

// 280
class BSShaderAccumulator : public NiAlphaAccumulator
{
public:
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(void);
	/*B0*/virtual void		Unk_2C(UInt32 arg1, UInt32 arg2);

	struct AccumStruct	//	Temp name
	{
		void		*_vtbl;	// 0x10B7DC0
		UInt32		unk04;
		UInt32		unk08;
		UInt32		unk0C;
		UInt32		unk10;
	};

	UInt32				unk034[4];		// 034
	float				flt044;			// 044
	UInt32				unk048[21];		// 048
	AccumStruct			accum09C;		// 09C
	AccumStruct			accum0B0;		// 0B0
	AccumStruct			accum0C4;		// 0C4
	AccumStruct			accum0D8;		// 0D8
	AccumStruct			accum0EC;		// 0EC
	AccumStruct			accum100;		// 100
	AccumStruct			accum114;		// 114
	AccumStruct			accum128;		// 128
	UInt32				unk13C[6];		// 13C
	float				flt154;			// 154
	float				flt158;			// 158
	float				flt15C;			// 15C
	float				flt160;			// 160
	UInt32				unk164[4];		// 164
	BSBatchRenderer		*batchRenderer;	// 174
	UInt32				unk178[7];		// 178
	ShadowSceneNode		*shadowScene;	// 194
	UInt32				unk198;			// 198
	UInt32				unk19C;			// 19C
	UInt32				unk1A0[56];		// 1A0
};
static_assert(sizeof(BSShaderAccumulator) == 0x280);

// 54
class NiGeometryBufferData
{
public:
	/*00*/virtual void		Destructor(bool doFree);
	/*04*/virtual bool		Unk_01(UInt32 arg1);

	UInt32						flags;			// 04
	NiUnsharedGeometryGroup		*unsharedGeom;	// 08
	UInt32						unk0C;			// 0C
	void						*ptr10;			// 10
	UInt32						unk14;			// 14
	UInt32						unk18;			// 18	Vertices/Normals count
	UInt32						unk1C;			// 1C		"			"
	UInt32						finished;		// 20
	void						*ptr24;			// 24
	void						*ptr28;			// 28
	UInt32						trianglePoints;	// 2C
	UInt32						unk30;			// 30	Byte size of triangles array
	void						*ptr34;			// 34
	UInt32						unk38;			// 38
	UInt32						unk3C;			// 3C
	UInt32						unk40;			// 40	Triangle count
	UInt32						unk44;			// 44		"
	UInt32						unk48;			// 48
	UInt32						unk4C;			// 4C
	NiTriangle					*triangles;		// 50	Same ptr as in NiTriShapeData
};
static_assert(sizeof(NiGeometryBufferData) == 0x54);

// 40
class NiGeometryData : public NiObject
{
public:
	/*8C*/virtual void		Unk_23(UInt32 arg);
	/*90*/virtual void		Unk_24(void);
	/*94*/virtual void		Unk_25(void);
	/*98*/virtual void		Unk_26(void);
	/*9C*/virtual bool		Unk_27(UInt32 arg);
	/*A0*/virtual void		Unk_28(void);

	UInt16					numVertices;	// 08
	UInt16					word0A;			// 0A
	UInt16					word0C;			// 0C
	UInt16					word0E;			// 0E
	NiSphere				bounds;			// 10
	NiVector3				*vertices;		// 20
	NiVector3				*normals;		// 24
	NiColorAlpha			*vertexColors;	// 28
	NiPoint2				*uvCoords;		// 2C
	UInt32					unk30;			// 30
	NiGeometryBufferData	*bufferData;	// 34
	UInt8					byte38;			// 38
	UInt8					byte39;			// 39
	UInt8					byte3A;			// 3A
	UInt8					byte3B;			// 3B
	UInt8					byte3C;			// 3C
	UInt8					pad3D[3];		// 3D
};
static_assert(sizeof(NiGeometryData) == 0x40);

// 44
class NiTriBasedGeomData : public NiGeometryData
{
public:
	/*A4*/virtual void		Unk_29(UInt32 arg);
	/*A8*/virtual void		Unk_2A(void);
	/*AC*/virtual void		Unk_2B(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	/*B0*/virtual void		Unk_2C(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);

	UInt16			numTriangles;		// 40
	UInt8			pad42[2];			// 42
};

// 58
class NiTriShapeData : public NiTriBasedGeomData
{
public:
	UInt32			trianglePoints;	// 44
	NiTriangle		*triangles;		// 48
	UInt16			*points;		// 4C
	UInt32			unk50;			// 50
	UInt32			unk54;			// 54
};
static_assert(sizeof(NiTriShapeData) == 0x58);

// 50
class NiTriStripsData : public NiTriBasedGeomData
{
public:
	UInt16			unk44;			// 44
	UInt16			unk46;			// 46
	UInt32			unk48;			// 48
	UInt32			unk4C;			// 4C
};

// 44
class NiLinesData : public NiGeometryData
{
public:
	UInt8			*unkArray;		// 44
};

// C4
class NiGeometry : public NiAVObject
{
public:
	/*DC*/virtual void		Unk_37(UInt32 arg1);
	/*E0*/virtual void		Unk_38(UInt32 arg1);
	/*E4*/virtual void		Unk_39(void);
	/*E8*/virtual void		Unk_3A(void);
	/*EC*/virtual void		Unk_3B(UInt32 arg1);

	NiAlphaProperty			*alphaProp;		// 9C
	NiCullingProperty		*cullingProp;	// A0
	NiMaterialProperty		*materialProp;	// A4
	BSShaderProperty		*shaderProp;	// A8
	NiStencilProperty		*stencilProp;	// AC
	NiTexturingProperty		*texturingProp;	// B0
	UInt32					unkB4;			// B4
	NiGeometryData			*geometryData;	// B8
	BSDismemberSkinInstance	*skinInstance;	// BC
	NiShader				*shader;		// C0
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
};

// C4
class NiTriStrips : public NiTriBasedGeom
{
public:
};

// D4
class BSScissorTriShape : public NiTriShape
{
public:
	UInt32			unkC4;			// C4
	UInt32			unkC8;			// C8
	UInt32			width;			// CC
	UInt32			height;			// D0
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
	UInt32		unkC4[3];		// C4
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

	NiFixedString	name;		// 08
	UInt32			unk0C;		// 0C
	UInt32			unk10;		// 10
	UInt8			byte14;		// 14
	UInt8			pad15[3];	// 15
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
};
static_assert(sizeof(NiParticleSystem) == 0x110);

// 14C
class ParticleShaderProperty : public BSShaderProperty
{
public:
	UInt32			unk060[59];		// 060
};
static_assert(sizeof(ParticleShaderProperty) == 0x14C);

struct NiCulledGeoList
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
	/*00*/virtual void		Unk_00(void);
	/*04*/virtual void		Unk_01(void);
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
	/*40*/virtual void		Destructor(bool freeMemory);
	/*44*/virtual void		Unk_11(void *arg);
	/*48*/virtual void		Cull(NiCamera *camera, ShadowSceneNode *scene, NiCulledGeoList *culledGeo);
	/*4C*/virtual void		AddGeo(NiGeometry *arg);

	UInt8				m_useAddGeoFn;	// 04 - call AddGeo when true, else just add to the list
	UInt8				pad05[3];		// 05
	NiCulledGeoList		*m_culledGeo;	// 08
	UInt32				unk0C[33];		// 0C
};
static_assert(sizeof(NiCullingProcess) == 0x90);

// CC
class BSCullingProcess : public NiCullingProcess
{
public:
	UInt32					unk90;			// 90
	UInt32					unk94[10];		// 94
	UInt32					unkBC;			// BC
	UInt32					unkC0;			// C0
	BSShaderAccumulator		*shaderAccum;	// C4
	UInt32					unkC8;			// C8
};
static_assert(sizeof(BSCullingProcess) == 0xCC);

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
		NiAVObject	*result;	// 00
		NiObject	*object04;	// 04
		NiVector3	pos;		// 08
		float		flt14;		// 14
		UInt32		unk18[7];	// 18
		float		flt34;		// 34
		float		flt38;		// 38
		float		flt3C;		// 3C
		float		flt40;		// 40
	};

	UInt32				unk00;		// 00	If non-zero, returns only one result
	UInt32				unk04;		// 04
	UInt32				unk08;		// 08
	UInt32				unk0C;		// 0C
	UInt8				byte10;		// 10
	UInt8				byte11;		// 11
	UInt8				pad12[2];	// 12
	NiAVObject			*originObj;	// 14
	NiTArray<Result*>	results;	// 18
	UInt32				numResults;	// 28
	UInt32				unk2C;		// 2C
	UInt8				byte30;		// 30
	UInt8				byte31;		// 31
	UInt8				byte32;		// 32
	UInt8				byte33;		// 33

	bool __fastcall GetResults(NiCamera *camera);
};