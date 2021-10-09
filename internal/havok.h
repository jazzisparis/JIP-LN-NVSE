#pragma once

typedef AlignedVector4 hkVector4;

struct hkMatrix3x4;

struct alignas(16) hkQuaternion
{
	float	x, y, z, w;

	hkQuaternion() {}
	hkQuaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	hkQuaternion(const NiVector3 &eulerYPR) {*this = eulerYPR;}
	hkQuaternion(const hkQuaternion &from) {*this = from;}
	hkQuaternion(const NiMatrix33 &rotMat) {*this = rotMat;}
	hkQuaternion(const hkMatrix3x4 &rotMat) {*this = rotMat;}

	void operator=(const NiVector3 &eulerYPR);
	inline void operator=(const hkQuaternion &from) {_mm_store_ps(&x, _mm_load_ps(&from.x));}
	void __fastcall operator=(const NiMatrix33 &mat);
	inline void operator=(const hkMatrix3x4 &rotMat) {ThisCall(0xCB26E0, this, &rotMat);}
	inline void operator=(const hkVector4 &from) {_mm_store_ps(&x, _mm_load_ps(&from.x));}

	inline void operator+=(const hkQuaternion &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
	}

	inline void operator-=(const hkQuaternion &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
	}

	inline void operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
	}

	void __fastcall operator*=(const hkQuaternion &rhs);

	inline hkQuaternion *Invert()
	{
		x = -x;
		y = -y;
		z = -z;
		return this;
	}

	hkQuaternion *Normalize();
	void EulerYPR(NiVector3 &ypr);
};

// 30
struct alignas(16) hkMatrix3x4
{
	float		rc[3][4];

	hkMatrix3x4() {}
	hkMatrix3x4(const NiMatrix33 &inMatrix) {*this = inMatrix;}
	hkMatrix3x4(const hkQuaternion &inQuaternion) {*this = inQuaternion;}
	hkMatrix3x4(const hkMatrix3x4 &inMatrix) {*this = inMatrix;}

	void __fastcall operator=(const NiMatrix33 &inMatrix);
	inline void operator=(const hkQuaternion &inQuaternion) {ThisCall(0xCB2D90, this, &inQuaternion);}
	inline void operator=(const hkMatrix3x4 &from)
	{
		_mm_store_ps(&rc[0][0], _mm_load_ps(&from.rc[0][0]));
		_mm_store_ps(&rc[1][0], _mm_load_ps(&from.rc[1][0]));
		_mm_store_ps(&rc[2][0], _mm_load_ps(&from.rc[2][0]));
	}
};

// 40
struct hkTransform
{
	hkMatrix3x4	rotation;		// 00
	hkVector4	translation;	// 30
};

// 50
struct hkSweptTransform
{
	hkVector4		centerOfMass0;	// 00 (40)
	hkVector4		centerOfMass1;	// 10 (50)
	hkQuaternion	rotation0;		// 20 (60)
	hkQuaternion	rotation1;		// 30 (70)
	hkVector4		centerOfMassLoc;// 40 (80)
};

// B0
struct hkMotionState
{
	hkTransform			transform;		// 00
	hkSweptTransform	sweptTransform;	// 40
	hkVector4			deltaAngle;		// 90
	float				objectRadius;	// A0
	float				linearDamping;	// A4
	float				angularDamping;	// A8
	UInt8				byteAC;			// AC
	UInt8				byteAD;			// AD
	UInt8				byteAE;			// AE
	UInt8				byteAF;			// AF
};
static_assert(sizeof(hkMotionState) == 0xB0);

// 0C
template <typename T_Data> class hkArray
{
public:
	enum
	{
		kFlag_NoDealloc =		0x80000000,	// data not owned by us
		kFlag_Locked =			0x40000000,	// will never be destroyed
	};

	T_Data		*data;				// 00
	UInt32		size;				// 04
	UInt32		capacityAndFlags;	// 08

	T_Data& operator[](UInt32 index) {return data[index];}

	class Iterator
	{
		friend hkArray;

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

		Iterator(hkArray &source) : pData(source.data), count(source.size) {}
	};

	Iterator Begin() {return Iterator(*this);}
};

class hkStatisticsCollector;
class hkpSimpleConstraintContactMgr;
class bhkWorldObject;
class NiStream;
class ahkpWorld;
class TESTrapListener;
class TESWaterListener;
class hkpContinuousSimulation;
class hkpDefaultWorldMaintenanceMgr;
class hkpBroadPhase;
class hkpBroadPhaseListener;
class hkpPhantomBroadPhaseListener;
class hkpEntityEntityBroadPhaseListener;
class hkpBroadPhaseBorderListener;
class hkpCollisionDispatcher;
class bhkCollisionFilter;
class hkpDefaultConvexListFilter;
class hkpEntityListener;
class hkpContactListener;
class bhkShape;
class hkpShape;
class hkpCachingShapePhantom;

// 04
class hkBaseObject
{
public:
	virtual void	Destroy(bool doFree);
};

// 08
class hkReferencedObject : public hkBaseObject
{
public:
	virtual void	CalcStatistics(hkStatisticsCollector *collector);
	virtual void	Unk_02(void);

	UInt16			sizeAndFlags;		// 04
	UInt16			refCount;			// 06
};

// 10
struct hkCdBody
{
	hkpShape		*shape;			// 00
	UInt32			shapeKey;		// 04
	void			*motion;		// 08 - either hkTransform or hkMotionState
	hkCdBody		*parent;		// 0C

	hkpWorldObject *GetWorldObj() const {return (hkpWorldObject*)((UInt8*)this - 0x10);}
};

// 80
struct ContactData
{
	UInt16							unk00;
	UInt16							flags;
	UInt32							key;
	hkpSimpleConstraintContactMgr	*contactMgr;
	UInt32							unk0C;
	hkCdBody						*cdBody1;
	hkCdBody						*cdBody2;
	UInt32							unk18[26];
};

// 08
struct CdBodyLinker
{
	ContactData		*data;
	hkCdBody		*cdBody;
};

// 10
struct CdParentObj
{
	void		*unk00;
	UInt32		unk04;
	void		*object;
	UInt32		unk0C;
};

// 70
struct hkCdPoint
{
	NiVector4		point00;		// 00
	NiVector4		point10;		// 10
	hkCdBody		*cdBody1;		// 20
	UInt8			body1Key;		// 24
	UInt8			pad25[2];		// 25
	UInt8			obj1Flag;		// 27
	NiVector4		point28;		// 28
	NiVector4		point38;		// 38
	hkCdBody		*cdBody2;		// 48
	UInt8			body2Key;		// 4C
	UInt8			pad4D[2];		// 4D
	UInt8			obj2Flag;		// 4F
	NiVector4		point50;		// 50
	NiVector4		point60;		// 60
};

// 04
class hkpEntityListener
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(UInt32 arg);
	virtual void	Unk_02(hkpRigidBody *rigidBody);
	virtual void	Unk_03(UInt32 arg);
	virtual void	Unk_04(UInt32 arg);
	virtual void	Unk_05(UInt32 arg);
};

// 10
class bhkEntityListener : public hkpEntityListener
{
public:
	virtual void	Unk_06(UInt32 arg);
	virtual bool	Unk_07(hkCdPoint *cdPoint);
	virtual bool	Unk_08(hkCdPoint *cdPoint);
	virtual UInt32	Unk_09(UInt32 arg);

	UInt32			unk04;		// 04
	UInt8			byte08;		// 08
	UInt8			pad09[3];	// 09
	UInt32			unk0C;		// 0C
};

// 28
class BGSAcousticSpaceListener : public bhkEntityListener
{
public:
	virtual bool	Unk_0A(UInt32 arg1, UInt32 arg2);
	virtual bool	Unk_0B(UInt32 arg1, UInt32 arg2);

	UInt32								unk10;		// 10
	UInt8								byte14;		// 14
	UInt8								pad15[3];	// 15
	NiTMapBase<TESObjectREFR*, int>		refrMap;	// 18
};
static_assert(sizeof(BGSAcousticSpaceListener) == 0x28);

// 04
class hkpPhantomListener
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
};

// D0
class hkpCharacterProxy : public hkReferencedObject
{
public:
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);

	hkpEntityListener		entityListener;		// 08
	hkpPhantomListener		phantomListener;	// 0C

	hkVector4				velocity;			// 10
	UInt32					unk20[4];			// 20
	hkpCachingShapePhantom	*shapePhantom;		// 30
	UInt32					unk34[16];			// 34
	hkArray<hkCdPoint>		cdPoints;			// 74
	hkArray<UInt32>			arr80;				// 80
	hkArray<UInt32>			arr8C;				// 8C
	hkArray<UInt32>			arr98;				// 98
	UInt32					unkA4[11];			// A4
};
static_assert(sizeof(hkpCharacterProxy) == 0xD0);

// D0
class ahkpCharacterProxy : public hkpCharacterProxy
{
public:
};

// 8C
class hkpWorldObject : public hkReferencedObject
{
public:
	virtual void	Unk_03(void);
	virtual hkMotionState	*GetMotionState();

	hkpWorld				*pWorld;		// 08
	bhkWorldObject			*object;		// 0C
	hkCdBody				cdBody;			// 10
	UInt32					unk20[2];		// 20
	UInt8					collisionType;	// 28	Known: 1 - hkpRigidBody, 2 - hkpPhantom
	UInt8					unk29[3];		// 29
	UInt8					layerType;		// 2C
	UInt8					filterFlags;	// 2D
	UInt8					unk2E[2];		// 2E
	NiVector4				vector30;		// 30
	NiVector4				vector40;		// 40
	UInt32					unk50[3];		// 50
	float					flt5C;			// 5C
	hkArray<CdBodyLinker>	cdRgdBodies;	// 60
	UInt32					unk6C[3];		// 6C
	hkArray<CdParentObj>	parentObjs;		// 78
	UInt32					unk84[2];		// 84
};
static_assert(sizeof(hkpWorldObject) == 0x8C);

// 10
class hkpShape : public hkReferencedObject
{
public:
	virtual void	Unk_03(hkVector4 *arg1);
	virtual UInt32	Unk_04();
	virtual bool	IsConvex();
	virtual UInt32	Unk_06(UInt32 arg1, UInt32 arg2);
	virtual void	Unk_07(UInt32 arg1, UInt32 arg2, UInt32 arg3);
	virtual void	*Unk_08(void *arg1, hkVector4 *arg2, void *arg3);
	virtual void	Unk_09(void *arg1, void *arg2, void *arg3);
	virtual void	Unk_0A(hkVector4 *arg1, hkVector4 *arg2, void *arg3, hkVector4 *arg4);
	virtual UInt32	Unk_0B();

	bhkShape		*shape;		// 08
	UInt32			unk0C;		// 0C
};

// 0C
class bhkRefObject : public NiObject
{
public:
	virtual void	SetObject(hkReferencedObject *object);
	virtual void	UpdateRefCount(bool incRef);	// inc/dec ref, depending on arg

	hkReferencedObject	*refObject;		// 08
};

// 10
class bhkSerializable : public bhkRefObject
{
public:
	virtual void		Unk_25(UInt32 arg);
	virtual hkpWorld	*GetWorld(void);
	virtual bool		Unk_27(UInt32 arg);
	virtual bool		Unk_28(void);
	virtual void		FreeData(bool del);	// free hkData
	virtual UInt32		Unk_2A(void);
	virtual void		LoadHavokData(NiStream *stream);	// called from bhkSerializable::Load after primary load is done
	virtual void		Unk_2C(void);	// create object
	virtual void		*CreateHavokData(UInt8 *arg);	// create Cinfo, return hkData
	virtual void		Unk_2E(void);		// destroy object
	virtual void		Unk_2F(void);
	virtual void		Unk_30(void);

	void				*hkData;	// 0C - stores hkConstraintData (descriptor used to build hkObj)
};

// 14
class bhkWorldObject : public bhkSerializable
{
public:
	virtual void		Unk_31(void);
	virtual void		Unk_32(void);
	virtual void		Unk_33(void);
	virtual void		Unk_34(void);

	UInt32				bodyFlags;		// 10

	void ApplyForce(NiVector4 *forceVector);
};

// 18
class bhkPhantom : public bhkWorldObject
{
public:
	UInt32				unk14;		// 14
};

// 18
class bhkShapePhantom : public bhkPhantom
{
public:
};

// 18
class bhkSimpleShapePhantom : public bhkShapePhantom
{
public:
};

class bhkCachingShapePhantom : public bhkShapePhantom
{
public:
};

// 10
class hkpTransformShape : public hkpShape
{
public:
};

// 14
class bhkShape : public bhkSerializable
{
public:
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);

	UInt32			materialType;	// 10
};

// 14
class bhkTransformShape : public bhkShape
{
public:
};

// 10
class bhkConstraint : public bhkSerializable
{
public:
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);
	virtual void	Unk_35(void);
};

// 10
class bhkLimitedHingeConstraint : public bhkConstraint
{
public:
};

struct hkpCdBodyPair
{
	hkCdBody		*cdBody1;
	UInt32			unk04;
	hkCdBody		*cdBody2;
	UInt32			unk0C;
};

// 14
class hkpCdBodyPairCollector
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);

	UInt32					unk04;			// 04
	hkArray<hkpCdBodyPair>	cdPairs;		// 08
};

// 114
class hkpAllCdBodyPairCollector : public hkpCdBodyPairCollector
{
public:
	hkpCdBodyPair			cdPairsLoc[16];	// 14
};
static_assert(sizeof(hkpAllCdBodyPairCollector) == 0x114);

// A4
class hkpPhantom : public hkpWorldObject
{
public:
	virtual UInt8	PhantomType();
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);

	UInt32			unk8C[6];		// 8C
};
static_assert(sizeof(hkpPhantom) == 0xA4);

// E0
class hkpAabbPhantom : public hkpPhantom
{
public:
	UInt32					unkA4[11];	// A4
	hkArray<hkCdBody*>		cdBodies;	// D0
	UInt32					unkDC;		// DC
};
static_assert(sizeof(hkpAabbPhantom) == 0xE0);

// 160
class hkpShapePhantom : public hkpPhantom
{
public:
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(hkpCdBodyPairCollector *pairCollector, UInt32 arg2);
	virtual void	GetCdBodyData(hkpCdBodyPairCollector *pairCollector, UInt32 arg2);

	UInt32				unk0A4[47];		// 0A4
};
static_assert(sizeof(hkpShapePhantom) == 0x160);

// 170
class hkpSimpleShapePhantom : public hkpShapePhantom
{
public:
	hkArray<hkCdBody*>		cdBodies;	// 160
	UInt32					unk16C;		// 16C
};
static_assert(sizeof(hkpSimpleShapePhantom) == 0x170);

// 170
class hkpCachingShapePhantom : public hkpShapePhantom
{
public:
	hkArray<CdBodyLinker>	cdBodies;	// 160
	UInt32					unk16C;		// 16C
};
static_assert(sizeof(hkpCachingShapePhantom) == 0x170);

class hkpConstraintOwner : public hkReferencedObject
{
public:
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
};

// 6C
class hkpSimulationIsland : public hkpConstraintOwner
{
public:
	UInt32					unk08[4];		// 08
	ahkpWorld				*world;			// 18
	UInt32					unk1C[11];		// 1C
	hkArray<hkpRigidBody*>	rigidBodies;	// 48
	hkpRigidBody			*rgdBody54;		// 54
	UInt32					unk58;			// 58
	hkArray<ContactData*>	contactData;	// 5C
	ContactData				*data68;		// 68
};

// 1B8
class hkpWorld : public hkReferencedObject
{
public:
	hkpContinuousSimulation				*simulation;				// 008
	float								flt00C;						// 00C
	NiVector4							gravity;					// 010
	hkpSimulationIsland					*fixedIsland;				// 020
	hkpRigidBody						*fixedRigidBody;			// 024
	hkArray<hkpSimulationIsland*>		activeSimulationIslands;	// 028
	hkArray<hkpSimulationIsland*>		inactiveSimulationIslands;	// 034
	hkArray<hkpSimulationIsland*>		dirtySimulationIslands;		// 040
	hkpDefaultWorldMaintenanceMgr		*maintenanceMgr;			// 04C
	UInt32								unk050;						// 050
	UInt32								unk054;						// 054
	hkpBroadPhase						*broadPhase;				// 058
	UInt32								unk05C;						// 05C
	UInt32								unk060;						// 060
	hkpBroadPhaseListener				**broadPhaseListeners;		// 064
	hkpPhantomBroadPhaseListener		*phantomBroadPhaseListener;	// 068
	hkpEntityEntityBroadPhaseListener	*entityBroadPhaseListener;	// 06C
	hkpBroadPhaseBorderListener			*broadPhaseBorderListener;	// 070
	void								*ptr074;					// 074
	void								*ptr078;					// 078
	bhkCollisionFilter					*collisionFilter;			// 07C
	hkpCollisionDispatcher				*collisionDispatcher;		// 080
	hkpDefaultConvexListFilter			*convexListFilter;			// 084
	void								*ptr088;					// 088
	UInt32								unk08C[13];					// 08C
	void								*ptr0C0;					// 0C0
	UInt32								unk0C4[19];					// 0C4
	hkArray<hkpPhantom*>				phantoms;					// 110
	UInt32								unk11C[3];					// 11C
	hkArray<hkpEntityListener*>			entityListeners1;			// 128
	hkArray<hkpEntityListener*>			entityListeners2;			// 134
	UInt32								unk140[6];					// 140
	hkArray<hkpContactListener*>		contactListeners;			// 158
	UInt32								unk164[15];					// 164
	hkArray<hkpContactListener*>		collisionListeners;			// 1A0
	UInt32								unk1AC[3];					// 1AC
};
static_assert(sizeof(hkpWorld) == 0x1B8);

// 354
class ahkpWorld : public hkpWorld
{
public:
	UInt32			unk1B8[83];			// 1B8
	UInt8			byte304;			// 304
	UInt8			byte305;			// 305
	UInt8			byte306;			// 306
	UInt8			byte307;			// 307
	UInt32			unk308[19];			// 308
};
static_assert(sizeof(ahkpWorld) == 0x354);

// 94
class bhkWorld : public bhkSerializable
{
public:
	NiRefObject					*visDebug;				// 10
	UInt8						byte14;					// 14
	UInt8						bCollisionEnabled;		// 15
	UInt8						isDebugTCG;				// 16
	UInt8						byte17;					// 17
	UInt32						maybeFlags18;			// 18
	UInt32						unk1C;					// 1C
	TESTrapListener				*trapListener;			// 20
	TESWaterListener			*waterListener;			// 24
	BGSAcousticSpaceListener	*acousticSpaceListener;	// 28
	void						*ptr2C;					// 2C
	void						*ptr30;					// 30
	void						*ptr34;					// 34
	void						*ptr38;					// 38
	void						*ptr3C;					// 3C
	void						*ptr40;					// 40
	void						*ptr44;					// 44
	void						*ptr48;					// 48
	void						*ptr4C;					// 4C
	UInt32						unk50;					// 50
	UInt32						unk54;					// 54
	UInt32						unk58;					// 58
	UInt32						unk5C;					// 5C
	hkVector4					origin;					// 60
	bhkEntityListener			**listenerArray;		// 70
	UInt32						numListeners;			// 74
	UInt32						unk78;					// 78
	UInt32						unk7C;					// 7C
	UInt32						unk80;					// 80
	UInt32						unk84;					// 84
	UInt32						unk88;					// 88
	UInt32						unk8C;					// 8C
	UInt32						unk90;					// 90
	UInt32						unk94;					// 94
	UInt32						unk98;					// 98
	UInt32						unk9C;					// 9C
};
static_assert(sizeof(bhkWorld) == 0xA0);

// E0
class bhkWorldM : public bhkWorld
{
public:
	hkVector4		borderSize;		// A0
	hkVector4		worldTotalSize;	// B0
	hkVector4		broadPhaseSize;	// C0
};
static_assert(sizeof(bhkWorldM) == 0xD0);

extern bhkWorldM **g_bhkWorldM;

// 0C
class NiCollisionObject : public NiObject
{
public:
	/*08C*/virtual void		Attach(NiAVObject *obj);
	/*090*/virtual void		Unk_24(UInt32 arg);
	/*094*/virtual void		Unk_25(void);
	/*098*/virtual void		LoadBoundingVolume(UInt32 arg);
	/*09C*/virtual void		Unk_27(UInt32 version, UInt32 arg1);

	NiNode			*linkedNode;	// 08
};

// 14
class bhkNiCollisionObject : public NiCollisionObject
{
public:
	/*0A0*/virtual void		Unk_28(void);
	/*0A4*/virtual void		Unk_29(void);
	/*0A8*/virtual void		Unk_2A(void);
	/*0AC*/virtual void		Unk_2B(void);
	/*0B0*/virtual void		Unk_2C(UInt32 moType, UInt32 arg2, UInt32 arg3);	//	arg3 unused?
	/*0B4*/virtual bool		IsMovable();
	/*0B8*/virtual void		ProcessCollision(UInt32 arg);
	/*0BC*/virtual bool		Unk_2F();
	/*0C0*/virtual void		Unk_30(UInt32 arg);

	enum Flags
	{
		kFlag_Active =			1,
		kFlag_Notify =			4,
		kFlag_SetLocal =		8,
		kFlag_DebugDisplay =	0x10,
		kFlag_UseVelocity =		0x20,
		kFlag_Reset =			0x40,
		kFlag_SyncOnUpdate =	0x80,
		kFlag_AnimTargeted =	0x400,
		kFlag_DismemberLimb =	0x800
	};

	UInt16			flags;			// 0C	0x40 is Update? Callbacks array @ 0x11AFE88
	UInt16			word0E;			// 0E
	bhkWorldObject	*worldObj;		// 10
};

// 14
class bhkCollisionObject : public bhkNiCollisionObject
{
public:
};

// 14
class bhkPCollisionObject : public bhkNiCollisionObject
{
public:
};

// 14
class bhkSPCollisionObject : public bhkPCollisionObject
{
public:
};

// 2C
class bhkBlendCollisionObject : public bhkCollisionObject
{
public:
	UInt8		byte14;		// 14
	UInt8		pad15[3];	// 15
	float		flt18;		// 18
	float		flt1C;		// 1C
	UInt32		unk20;		// 20
	UInt32		unk24;		// 24
	UInt32		unk28;		// 28
};

// 08
class hkpCdPointCollector
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);

	float			earlyOutDistance;	// 04
};

// 3A0
class hkpAllCdPointCollector : public hkpCdPointCollector
{
public:
	virtual void	Unk_03(void);

	UInt32					unk008[2];		// 008
	hkArray<hkCdPoint>		cdPoints;		// 010
	UInt32					unk01C;			// 01C
	hkCdPoint				cdPointsLoc[8];	// 020
};
static_assert(sizeof(hkpAllCdPointCollector) == 0x3A0);

// 3C8
class bhkCharacterPointCollector : public hkpAllCdPointCollector
{
public:
	UInt32						unk3A0;			// 3A0
	hkArray<hkpWorldObject*>	contactBodies;	// 3A4
	hkArray<UInt32>				arr3B0;			// 3B0
	hkArray<float>				arr3BC;			// 3BC
};
static_assert(sizeof(bhkCharacterPointCollector) == 0x3C8);

// 3E0
class bhkCharacterProxy : public bhkSerializable
{
public:
	bhkCharacterPointCollector	pointCollector;		// 10
	UInt32						unk3D8[2];
};
static_assert(sizeof(bhkCharacterProxy) == 0x3E0);

// 08
class hkpCharacterState : public hkReferencedObject
{
public:
	enum StateType
	{
		kState_OnGround,
		kState_Jumping,
		kState_InAir,
		kState_Climbing,
		kState_Flying,
		kState_Swimming,
		kState_Projectile
	};

	virtual UInt32	GetStateType();
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	UpdateVelocity(bhkCharacterController *charCtrl);
};

class bhkCharacterState : public hkpCharacterState
{
public:
};

class bhkCharacterStateOnGround : public bhkCharacterState
{
public:
};

class bhkCharacterStateJumping : public bhkCharacterState
{
public:
};

class bhkCharacterStateInAir : public bhkCharacterState
{
public:
};

class bhkCharacterStateClimbing : public bhkCharacterState
{
public:
};

class bhkCharacterStateFlying : public bhkCharacterState
{
public:
};

class bhkCharacterStateSwimming : public bhkCharacterState
{
public:
};

class bhkCharacterStateProjectile : public bhkCharacterState
{
public:
};

// 34
class hkpCharacterStateManager : public hkReferencedObject
{
public:
	bhkCharacterStateOnGround		*stateOnGround;		// 08
	bhkCharacterStateJumping		*stateJumping;		// 0C
	bhkCharacterStateInAir			*stateInAir;		// 10
	bhkCharacterStateClimbing		*stateClimbing;		// 14
	bhkCharacterStateFlying			*stateFlying;		// 18
	bhkCharacterStateSwimming		*stateSwimming;		// 1C
	bhkCharacterStateProjectile		*stateProjectile;	// 20
	UInt32							unk24[4];			// 24
};

// 30
class hkpCharacterContext : public hkReferencedObject
{
public:
	UInt32						unk08;			// 08
	hkpCharacterStateManager	*stateMngr;		// 0C
	UInt32						hkState;		// 10
	UInt32						unk14[7];		// 14
};

// 04
class hkpCharacterProxyListener
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
};

// 70
class bhkCharacterListener : public hkpCharacterProxyListener
{
public:
	enum
	{
		kJumping =			0x400,
		kIsSceneComplex =	0x100000,
		kTiltFrontBack =	0x2000000,
		kTiltLeftRight =	0x4000000,
		kIsUsingFurniture =	0x8000000,
	};

	UInt32			flags;			// 04 (414)
	UInt32			unk08[6];		// 08
	float			flt20;			// 20 (430)
	UInt32			surfaceMaterial;// 24 (434)
	UInt32			unk28;			// 28 (438)
	UInt32			unk2C[5];		// 2C
	hkVector4		vector40;		// 40 (450)
	UInt32			unk50[8];		// 50
};

class hkpSphereRepShape : public hkpShape
{
public:
};

class hkpConvexShape : public hkpSphereRepShape
{
public:
};

// 20
class hkpSphereShape : public hkpConvexShape
{
public:
	UInt32			unk10[4];		// 10
};

// 30
class hkpBoxShape : public hkpConvexShape
{
public:
	virtual void	Unk_0C(void *arg1);
	virtual void	Unk_0D(hkVector4 *vec);
	virtual void	GetSupportingVertex(hkVector4 *arg1, hkVector4 *outVertex);
	virtual void	ConvertVertexIdsToVertices(UInt16 *vtxIDsArr, UInt32 numVertices, hkVector4 *outVertices);
	virtual UInt32	Unk_10(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6, UInt32 arg7);
	virtual void	Unk_11(hkVector4 *vec);

	hkVector4		vector10;	// 10
	hkVector4		vector20;	// 20
};
static_assert(sizeof(hkpBoxShape) == 0x30);

struct VerticesBlock
{
	hkVector4	rowX;
	hkVector4	rowY;
	hkVector4	rowZ;
};

// 70
class hkpConvexVerticesShape : public hkpConvexShape
{
public:
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);

	float			radius;			// 10
	float			flt14;			// 14
	UInt32			unk18;			// 18
	float			flt1C;			// 1C
	float			flt20;			// 20
	float			flt24;			// 24
	float			flt28;			// 28
	UInt32			unk2C;			// 2C
	UInt32			unk30;			// 30
	UInt32			unk34;			// 34
	UInt32			unk38;			// 38
	UInt32			unk3C;			// 3C
	VerticesBlock	*verticesArray;	// 40
	UInt32			numBlocks;		// 44
	UInt32			unk48;			// 48
	UInt32			numVertices;	// 4C
	UInt32			unk50;			// 50
	NiVector4		*normalsArray;	// 54
	UInt32			numNormals;		// 58
	UInt32			unk5C;			// 5C
	UInt32			unk60;			// 60
	float			flt64;			// 64
	UInt32			unk68;			// 68
	float			flt6C;			// 6C
};
static_assert(sizeof(hkpConvexVerticesShape) == 0x70);

// 70
class hkCharControllerShape : public hkpConvexVerticesShape
{
public:
	virtual void	Unk_12(void);
};

// 40
class hkpCapsuleShape : public hkpConvexShape
{
public:
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);

	float			radius;		// 10
	UInt32			unk14;		// 14
	UInt32			unk18;		// 18
	UInt32			unk1C;		// 1C
	NiVector3		point1;		// 20
	float			radius1;	// 2C
	NiVector3		point2;		// 30
	float			radius2;	// 3C
};
static_assert(sizeof(hkpCapsuleShape) == 0x40);

// 18
class hkpShapeContainer
{
public:
	virtual void	Destroy(bool doFree);
	virtual UInt32	GetSize();
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);

	// 10
	struct ShapeItem
	{
		hkpShape	*shape;		// 00
		UInt32		unk04[3];	// 04
	};

	UInt32				unk04;		// 04
	hkArray<ShapeItem>	shapes;		// 08
	UInt16				word14;		// 14
	UInt16				sizeMod;	// 16
};
static_assert(sizeof(hkpShapeContainer) == 0x18);

// 70
class hkpShapeCollection : public hkpShape
{
public:
	hkpShapeContainer	shapeList;	// 10
	UInt32				unk28[2];	// 28
	float				flt30[8];	// 30
	UInt32				unk50[8];	// 50
};
static_assert(sizeof(hkpShapeCollection) == 0x70);

// 70
class hkpListShape : public hkpShapeCollection
{
public:
};

// 14
class bhkSphereRepShape : public bhkShape
{
public:
};

// 14
class bhkConvexShape : public bhkSphereRepShape
{
public:
};

// 14
class bhkCapsuleShape : public bhkConvexShape
{
public:
};

// 14
class bhkConvexVerticesShape : public bhkConvexShape
{
public:
};

// 14
class bhkCharControllerShape : public bhkConvexVerticesShape
{
public:
};

class bhkShapeCollection : public bhkShape
{
public:
};

class bhkListShape : public bhkShapeCollection
{
public:
};

// 40
struct hkpSurfaceInfo
{
	UInt32			supportedState;		// 00
	UInt32			unk04;				// 04
	UInt32			unk08;				// 08
	UInt32			unk0C;				// 0C
	hkVector4		surfaceNormal;		// 10
	hkVector4		surfaceVelocity;	// 20
	float			flt30;				// 30
	UInt32			unk34[3];			// 34
};
static_assert(sizeof(hkpSurfaceInfo) == 0x40);

// 10
struct hkStepInfo
{
	float		startTime;		// 00
	float		endTime;		// 04
	float		deltaTime;		// 08
	float		invDeltaTime;	// 0C
};

// 650
class bhkCharacterController : public bhkCharacterProxy
{
public:
	virtual void	Unk_31(void);
	virtual void	Unk_32(void);
	virtual void	Unk_33(void);
	virtual void	Unk_34(void);

	hkpCharacterContext		chrContext;			// 3E0
	bhkCharacterListener	chrListener;		// 410

	hkpSurfaceInfo			surfaceInfo;		// 480
	hkVector4				upVec;				// 4C0
	hkVector4				forwardVec;			// 4D0
	hkStepInfo				stepInfo;			// 4E0
	hkVector4				velocity;			// 4F0
	hkVector4				throwbackVelocity;	// 500
	hkVector4				direction;			// 510
	UInt32					wantState;			// 520
	float					throwbackTimer;		// 524
	float					rotMod;				// 528
	float					rotModTime;			// 52C
	float					calculatePitchTimer;// 530
	float					acrobatics;			// 534	Always 100.0
	float					center;				// 538
	float					waterHeight;		// 53C
	float					jumpHeight;			// 540
	float					startingHeight;		// 544
	float					fallTimeElapsed;	// 548
	float					gravityMult;		// 54C
	float					tiltAngleX;			// 550
	float					tiltAngleY;			// 554
	float					pitchMult;			// 558
	float					scale;				// 55C
	float					swimFloatHeight;	// 560	Always 1.6
	float					actorHeight;		// 564	ControllerShape total length (z)
	float					speedPct;			// 568
	float					flt56C;				// 56C
	hkVector4				rotCenter;			// 570
	float					flt580;				// 580
	float					flt584;				// 584
	float					flt588;				// 588
	float					flt58C;				// 58C
	float					flt590;				// 590
	bhkCachingShapePhantom	*chrPhantom;		// 594
	UInt32					unk598;				// 598
	UInt32					unk59C;				// 59C
	UInt32					unk5A0;				// 5A0
	bhkCapsuleShape			*capsuleShape;		// 5A4
	bhkCharControllerShape	*charCtrlShape;		// 5A8
	float					unk5AC[17];			// 5AC
	float					shapeDefaultRadius;	// 5F0
	float					shapeHeight;		// 5F4
	float					shapeRadius;		// 5F8
	float					flt5FC;				// 5FC
	UInt32					unk600[2];			// 600
	UInt8					byte608;			// 608
	UInt8					pad609[3];			// 609
	hkpRigidBody			*bodyUnderFeet;		// 60C
	UInt8					byte610;			// 610
	UInt8					pad611[3];			// 611
	UInt32					unk614[3];			// 614
	hkVector4				vector620;			// 620
	void					*ptr630;			// 630
	float					flt634;				// 634
	void					*ptr638;			// 638
	hkArray<hkCdPoint>		arr63C;				// 63C
	void					*ptr648;			// 648
	void					*ptr64C;			// 64C
};
static_assert(sizeof(bhkCharacterController) == 0x650);

class bhkCharacterListenerSpell : public bhkCharacterController
{
public:
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);

	UInt32			unk650;		// 650
};

class bhkCharacterListenerArrow : public bhkCharacterController
{
public:
	virtual void	Unk_35(void);
	virtual void	Unk_36(void);
	virtual void	Unk_37(void);
	virtual void	Unk_38(void);

	UInt32			unk650;		// 650
};

class ProjectileListener : public bhkCharacterController
{
public:
	virtual void	Unk_35(void);

	UInt32			unk650;		// 650
};

class hkpEntity : public hkpWorldObject
{
public:
	virtual void	Unk_05(void);
};

class hkpConstraintData : public hkReferencedObject
{
public:
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
};

// 38
class hkpConstraintInstance : public hkReferencedObject
{
public:
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);

	hkpSimulationIsland		*simIsle;			// 08
	hkpConstraintData		*constraintData;	// 0C
	UInt32					unk10;				// 10
	hkpRigidBody			*contactBody;		// 14
	hkpRigidBody			*parentBody;		// 18
	UInt32					unk1C[5];			// 1C
	void					*ptr30;				// 30
	UInt32					unk34;				// 34
};

// 140
class hkpMotion : public hkReferencedObject
{
public:
	virtual void	SetBodyMass(float _bodyMass);
	virtual void	SetBodyMassInv(float _bodyMassInv);
	virtual void	GetInertiaLocal(hkMatrix3x4 *outInertia);
	virtual void	GetInertiaWorld(hkMatrix3x4 *outInertia);
	virtual void	SetInertiaLocal(hkMatrix3x4 *inInertia);
	virtual void	SetInertiaInvLocal(hkMatrix3x4 *inInertia);
	virtual void	GetInertiaInvLocal(hkMatrix3x4 *outInertia);
	virtual void	GetInertiaInvWorld(hkMatrix3x4 *outInertia);
	virtual void	SetCenterOfMassInLocal(hkVector4 *centerOfMass);
	virtual void	SetPosition(hkVector4 *position);
	virtual void	SetRotation(hkQuaternion *rotation);
	virtual void	SetPositionAndRotation(hkVector4 *position, hkQuaternion *rotation);
	virtual void	SetTransform(hkTransform *transform);
	virtual void	SetLinearVelocity(hkVector4 *velocity);
	virtual void	SetAngularVelocity(hkVector4 *velocity);
	virtual void	GetProjectedPointVelocity(hkVector4 *point, hkVector4 *normal, float *outVel, float *outInvVirtMass);
	virtual void	ApplyLinearImpulse(hkVector4 *impulse);
	virtual void	ApplyPointImpulse(hkVector4 *impulse, hkVector4 *point);
	virtual void	ApplyAngularImpulse(hkVector4 *impulse);
	virtual void	ApplyPointForce(float deltaTime, hkVector4 *force, hkVector4 *point);
	virtual void	ApplyForce(float deltaTime, hkVector4 *force);
	virtual void	ApplyTorque(float deltaTime, hkVector4 *torque);
	virtual void	GetMotionStateAndVelocitiesAndDeactivationType(hkpMotion *outMotion);

	enum MotionType
	{
		kMotionType_Sphere = 2,
		kMotionType_Box,
		kMotionType_KeyframedRigid,
		kMotionType_FixedRigid,
		kMotionType_ThinBox,
		kMotionType_Character,
	};

	UInt8			type;				// 008
	UInt8			deactivationType;	// 009
	UInt16			word00A;			// 00A
	UInt16			word00C;			// 00C
	UInt8			pad00E[2];			// 00E
	hkMotionState	motionState;		// 010
	NiVector3		inertia;			// 0C0
	float			bodyMassInv;		// 0CC
	hkVector4		linVelocity;		// 0D0
	hkVector4		angVelocity;		// 0E0
	hkVector4		deactivationPos[2];	// 0F0
	UInt32			unk110[12];			// 110

	__forceinline float GetBodyMass()
	{
		return ThisCall<float>(0xC86430, this);
	}
};
static_assert(sizeof(hkpMotion) == 0x140);

class hkpSphereMotion : public hkpMotion
{
public:
};

class hkpBoxMotion : public hkpMotion
{
public:
};

class hkpThinBoxMotion : public hkpBoxMotion
{
public:
};

class hkpKeyframedRigidMotion : public hkpMotion
{
public:
	virtual void	Unk_1A(UInt32 arg1, UInt32 arg2);	// Null sub
	virtual void	SetStoredMotion(hkpMotion *savedMotion);
};

class hkpMaxSizeMotion : public hkpKeyframedRigidMotion
{
public:
};

class hkpFixedRigidMotion : public hkpKeyframedRigidMotion
{
public:
	virtual void	GetPositionAndVelocities(hkpMotion *outMotion);
};

// 30
struct ConstraintContact
{
	hkpConstraintInstance	*instance;		// 00
	hkpRigidBody			*parentBody;	// 04
	hkpRigidBody			*contactBody;	// 08
	void					*ptr0C;			// 0C
	UInt32					unk10[5];		// 10
	float					flt24[3];		// 24
};

// 220
class hkpRigidBody : public hkpEntity
{
public:
	virtual void	Unk_06(void);

	UInt8							byte8C;			// 8C
	UInt8							pad8D[3];		// 8D
	float							friction;		// 90
	float							restitution;	// 94
	UInt32							unk98[5];		// 98
	ConstraintContact				*contactsArr;	// AC
	UInt16							contactsSize;	// B0
	UInt16							contactsCap;	// B2
	hkArray<hkpConstraintInstance*>	constraintInst;	// B4
	UInt32							unkC0[3];		// C0
	hkpSimulationIsland				*simIsle;		// CC
	UInt32							unkD0[4];		// D0
	hkpMotion						motion;			// E0

	__forceinline void UpdateMotion()
	{
		ThisCall(0xC9C1D0, this);
	}

	void SetAngularVelocity(UInt8 axis, float angVel);
};

// 14
class bhkEntity : public bhkWorldObject
{
public:
};

// 1C
class bhkRigidBody : public bhkEntity
{
public:
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

	UInt32			unk14[2];		// 14
};

// 50
class bhkRigidBodyT : public bhkRigidBody
{
public:
	UInt32			unk1C[13];		// 1C
};

// ?
class hkaRaycastInterface
{
public:
	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
};

// 2C0
class bhkRagdollController : public hkaRaycastInterface
{
public:
	UInt32				unk004[21];		// 004
	NiNode				*bip01Node;		// 058
	UInt32				unk05C[140];	// 05C
	UInt8				byte28C;		// 28C
	UInt8				byte28D;		// 28D
	UInt8				byte28E;		// 28E
	UInt8				byte28F;		// 28F
	UInt32				unk290[12];		// 290
};
static_assert(sizeof(bhkRagdollController) == 0x2C0);