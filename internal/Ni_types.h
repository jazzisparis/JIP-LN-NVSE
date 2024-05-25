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
	__forceinline NiPoint2(float _x, float _y) : x(_x), y(_y) {}
	__forceinline NiPoint2(const NiPoint2 &rhs) {*this = rhs;}
	__forceinline explicit NiPoint2(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiPoint2 &&rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	__forceinline void operator=(const NiPoint2 &rhs) {_mm_storeu_si64(this, _mm_loadu_si64(&rhs));}

	__forceinline NiPoint2& SetPS(const __m128 rhs)
	{
		_mm_storeu_si64(this, _mm_castps_si128(rhs));
		return *this;
	}

	inline operator float*() {return &x;}

	__forceinline __m128 PS() const {return _mm_castsi128_ps(_mm_loadu_si64(this));}

	void Dump() const;
};

// 0C
struct NiVector3
{
	float	x, y, z;

	NiVector3() {}
	__forceinline NiVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	__forceinline NiVector3(const NiVector3 &rhs) {*this = rhs;}
	__forceinline explicit NiVector3(const NiMatrix33 &rhs) {*this = rhs;}
	__forceinline explicit NiVector3(const NiQuaternion &rhs) {*this = rhs;}
	__forceinline explicit NiVector3(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiVector3 &&rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
	__forceinline void operator=(const NiVector3 &rhs)
	{
		_mm_storeu_si64(this, _mm_loadu_si64(&rhs));
		z = rhs.z;
	}
	__forceinline void operator=(const NiPoint2 &rhs) {_mm_storeu_si64(this, _mm_castps_si128(rhs.PS()));}

	void operator=(const NiMatrix33 &from);
	void operator=(const NiQuaternion &from);

	__forceinline NiVector3& SetPS(const __m128 rhs)
	{
		_mm_storeu_si64(this, _mm_castps_si128(rhs));
		_mm_store_ss(&z, _mm_unpackhi_ps(rhs, rhs));
		return *this;
	}

	__forceinline __m128 operator+(__m128 packedPS) const {return PS() + packedPS;}
	__forceinline __m128 operator-(__m128 packedPS) const {return PS() - packedPS;}
	__forceinline __m128 operator*(float s) const {return PS() * _mm_set_ps1(s);}
	__forceinline __m128 operator*(__m128 packedPS) const {return PS() * packedPS;}

	__forceinline NiVector3& operator+=(__m128 packedPS) {return SetPS(*this + packedPS);}
	__forceinline NiVector3& operator-=(__m128 packedPS) {return SetPS(*this - packedPS);}
	__forceinline NiVector3& operator*=(float s) {return SetPS(*this * s);}
	__forceinline NiVector3& operator*=(__m128 packedPS) {return SetPS(*this * packedPS);}

	inline operator float*() {return &x;}
	inline operator NiPoint2&() const {return *(NiPoint2*)this;}

	__forceinline __m128 PS() const {return _mm_loadu_ps(&x);}
	__forceinline __m128 PS2() const {return _mm_castsi128_ps(_mm_loadu_si64(this));}
	__forceinline __m128 PS3() const {return PS() & GET_PS(4);}

	inline bool operator==(const NiVector3 &rhs) const {return Equal_V3(PS(), rhs.PS());}
	inline bool operator!=(const NiVector3 &rhs) const {return !(*this == rhs);}

	inline float __vectorcall DotProduct(const NiVector3 &rhs) const
	{
		__m128 k = _mm_setzero_ps();
		return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(PS3() * rhs.PS(), k), k));
	}

	inline float __vectorcall Length() const {return Length_V4(PS3());}
	NiVector3& Normalize();
	__m128 __vectorcall CrossProduct(const NiVector3 &vB) const;
	__m128 __vectorcall Interpolate(const NiVector3 &vB, float t) const;
	__m128 __vectorcall GetTranslatedPos(const NiTransform &transfrm) const;
	__m128 __vectorcall GetRotatedPos(__m128 pry, const NiVector3 &origin) const;

	inline float __vectorcall GetAngleBetweenV3(const NiVector3 &vB) const
	{
		return ACos(DotProduct(vB));
	}

	void Dump() const;
};
typedef NiVector3 NiPoint3;

float __vectorcall Point2Distance(const NiVector3 &pt1, const NiVector3 &pt2);
float __vectorcall Point3Distance(const NiVector3 &pt1, const NiVector3 &pt2);
__m128 __vectorcall GetAnglesBetweenPoints(__m128 pt1, __m128 pt2);
__m128 __vectorcall TransformWorldToLocal(const NiVector3 &origin, const NiVector3 &target, __m128 rotation);

struct NiVector4
{
	float	x, y, z, w;

	NiVector4() {}
	__forceinline NiVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	__forceinline NiVector4(const NiVector4 &rhs) {*this = rhs;}
	__forceinline NiVector4(const NiVector3 &rhs) {*this = rhs;}
	__forceinline explicit NiVector4(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiVector4 &&rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}
	__forceinline void operator=(const NiVector4 &rhs) {SetPS(rhs.PS());}
	__forceinline void operator=(const NiVector3 &rhs) {SetPS(rhs.PS3());}

	__forceinline NiVector4& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&x, rhs);
		return *this;
	}

	__forceinline __m128 operator+(const NiVector3 &rhs) const {return PS() + rhs.PS3();}
	__forceinline __m128 operator+(__m128 packedPS) const {return PS() + packedPS;}

	__forceinline __m128 operator-(const NiVector3 &rhs) const {return PS() - rhs.PS3();}
	__forceinline __m128 operator-(__m128 packedPS) const {return PS() * packedPS;}

	__forceinline __m128 operator*(float s) const {return PS() * _mm_set_ps1(s);}
	__forceinline __m128 operator*(const NiVector3 &rhs) const {return PS() * rhs.PS3();}
	__forceinline __m128 operator*(__m128 packedPS) const {return PS() * packedPS;}

	__forceinline NiVector4& operator+=(const NiVector3 &rhs) {return SetPS(*this + rhs);}
	__forceinline NiVector4& operator+=(__m128 packedPS) {return SetPS(*this + packedPS);}

	__forceinline NiVector4& operator-=(const NiVector3 &rhs) {return SetPS(*this - rhs);}
	__forceinline NiVector4& operator-=(__m128 packedPS) {return SetPS(*this - packedPS);}

	__forceinline NiVector4& operator*=(float s) {return SetPS(*this * s);}
	__forceinline NiVector4& operator*=(const NiVector3 &rhs) {return SetPS(*this * rhs);}
	__forceinline NiVector4& operator*=(__m128 packedPS) {return SetPS(*this * packedPS);}

	inline operator float*() {return &x;}
	inline operator NiVector3&() const {return *(NiVector3*)this;}

	__forceinline __m128 PS() const {return _mm_loadu_ps(&x);}

	bool RayCastCoords(const NiVector3 &posVector, float *rotMatRow, float maxRange, SInt32 layerType);

	void Dump() const;
};

struct alignas(16) AlignedVector4
{
	float	x, y, z, w;

	AlignedVector4() {}
	__forceinline AlignedVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	__forceinline AlignedVector4(const AlignedVector4 &rhs) {*this = rhs;}
	__forceinline AlignedVector4(const NiVector4 &rhs) {*this = rhs;}
	__forceinline explicit AlignedVector4(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(AlignedVector4 &&rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}
	__forceinline void operator=(const AlignedVector4 &rhs) {SetPS(rhs.PS());}
	__forceinline void operator=(const NiVector4 &rhs) {SetPS(rhs.PS());}

	__forceinline AlignedVector4& SetPS(const __m128 rhs)
	{
		_mm_store_ps(&x, rhs);
		return *this;
	}

	__forceinline __m128 operator+(__m128 packedPS) const {return PS() + packedPS;}
	__forceinline __m128 operator-(__m128 packedPS) const {return PS() - packedPS;}
	__forceinline __m128 operator*(float s) const {return PS() * _mm_set_ps1(s);}
	__forceinline __m128 operator*(__m128 packedPS) const {return PS() * packedPS;}

	__forceinline AlignedVector4& operator+=(__m128 packedPS) {return SetPS(*this + packedPS);}
	__forceinline AlignedVector4& operator-=(__m128 packedPS) {return SetPS(*this - packedPS);}
	__forceinline AlignedVector4& operator*=(float s) {return SetPS(*this * s);}
	__forceinline AlignedVector4& operator*=(__m128 packedPS) {return SetPS(*this * packedPS);}

	inline operator float*() {return &x;}
	inline operator NiVector3&() const {return *(NiVector3*)this;}

	__forceinline __m128 PS() const {return _mm_load_ps(&x);}
};

// 10
struct AxisAngle
{
	NiVector3	axis;
	float		theta;

	AxisAngle() {}
	__forceinline AxisAngle(float x, float y, float z, float t) : axis(x, y, z), theta(t) {}
	__forceinline explicit AxisAngle(const NiVector3 &_axis, float t) : axis(_axis), theta(t) {}
	__forceinline AxisAngle(const AxisAngle &from) {*this = from;}
	__forceinline explicit AxisAngle(const NiVector3 &pry) {*this = pry;}
	__forceinline explicit AxisAngle(const NiMatrix33 &rotMat) {*this = rotMat;}
	__forceinline explicit AxisAngle(const NiQuaternion &qt) {*this = qt;}
	__forceinline explicit AxisAngle(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(AxisAngle &&from)
	{
		axis.x = from.axis.x;
		axis.y = from.axis.y;
		axis.z = from.axis.z;
		theta = from.theta;
	}
	__forceinline void operator=(const AxisAngle &from) {SetPS(from.PS());}
	__forceinline void operator=(const NiVector3 &pry) {FromEulerPRY(pry.PS());}
	__forceinline void operator=(const NiMatrix33 &rotMat) {FromRotationMatrix(rotMat);}
	__forceinline void operator=(const NiQuaternion &qt) {FromQuaternion(qt);}

	__forceinline AxisAngle& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&axis.x, rhs);
		return *this;
	}

	inline operator float*() {return &axis.x;}
	__forceinline __m128 PS() const {return _mm_loadu_ps(&axis.x);}

	__forceinline bool operator==(const AxisAngle &rhs) const {return Equal_V4(PS(), rhs.PS());}
	__forceinline bool operator!=(const AxisAngle &rhs) const {return !(*this == rhs);}

	AxisAngle& __vectorcall FromEulerPRY(__m128 pry);
	AxisAngle& __fastcall FromRotationMatrix(const NiMatrix33 &rotMat);
	AxisAngle& __fastcall FromQuaternion(const NiQuaternion &qt);

	__m128 __vectorcall ToEulerPRY() const;

	void Dump() const;
};

// 24
struct NiMatrix33
{
	float	cr[3][3];

	NiMatrix33() {}
	__forceinline NiMatrix33(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		cr[0][0] = m00; cr[0][1] = m10; cr[0][2] = m20;
		cr[0][3] = m01; cr[0][4] = m11; cr[0][5] = m21;
		cr[0][6] = m02; cr[0][7] = m12; cr[0][8] = m22;
	}
	__forceinline explicit NiMatrix33(const NiMatrix33 &from) {*this = from;}
	__forceinline explicit NiMatrix33(const NiVector3 &pry) {*this = pry;}
	__forceinline explicit NiMatrix33(__m128 pry) {*this = pry;}
	__forceinline explicit NiMatrix33(const NiQuaternion &qt) {*this = qt;}
	__forceinline explicit NiMatrix33(const AxisAngle &axisAngle) {*this = axisAngle;}
	__forceinline explicit NiMatrix33(const hkMatrix3x4 &inMat) {*this = inMat;}

	__forceinline void operator=(NiMatrix33 &&rhs)
	{
		cr[0][0] = rhs.cr[0][0]; cr[0][1] = rhs.cr[0][1]; cr[0][2] = rhs.cr[0][2];
		cr[0][3] = rhs.cr[0][3]; cr[0][4] = rhs.cr[0][4]; cr[0][5] = rhs.cr[0][5];
		cr[0][6] = rhs.cr[0][6]; cr[0][7] = rhs.cr[0][7]; cr[0][8] = rhs.cr[0][8];
	}
	__forceinline void operator=(const NiMatrix33 &rhs)
	{
		_mm_storeu_ps(&cr[0][0], _mm_loadu_ps(&rhs.cr[0][0]));
		_mm_storeu_ps(&cr[0][4], _mm_loadu_ps(&rhs.cr[0][4]));
		cr[0][8] = rhs.cr[0][8];
	}
	__forceinline void operator=(const NiVector3 &pry) {FromEulerPRY(pry.PS());}
	__forceinline void operator=(__m128 pry) {FromEulerPRY(pry);}
	__forceinline void operator=(const NiQuaternion &qt) {FromQuaternion(qt);}
	__forceinline void operator=(const AxisAngle &axisAngle) {FromAxisAngle(axisAngle);}
	void __fastcall operator=(const hkMatrix3x4 &inMat);

	__forceinline NiMatrix33& operator*=(const NiMatrix33 &rhs) {return MultiplyMatrices(rhs);}
	__forceinline __m128 operator*(__m128 vec) const {return MultiplyVector(vec);}

	inline operator float*() const {return (float*)this;}

	inline bool operator==(const NiMatrix33 &rhs) const
	{
		for (UInt32 i = 0; i < 9; i++)
			if (fabs((*this)[i] - rhs[i]) > 0.00001F)
				return false;
		return true;
	}

	__m128 __vectorcall ToEulerPRY() const;
	__m128 __vectorcall ToEulerPRYInv() const;
	float __vectorcall ExtractPitch() const;
	float __vectorcall ExtractRoll() const;
	float __vectorcall ExtractYaw() const;
	NiMatrix33& __vectorcall FromEulerPRY(__m128 pry);
	NiMatrix33& __vectorcall FromEulerPRYInv(__m128 pry);
	NiMatrix33& __vectorcall FromEulerYPR(__m128 pry);
	float __vectorcall From2Points(__m128 pt1, __m128 pt2);
	NiMatrix33& __fastcall FromQuaternion(const NiQuaternion &qt);
	NiMatrix33& __fastcall FromAxisAngle(const AxisAngle &axisAngle);
	__m128 __vectorcall MultiplyVector(__m128 vec) const;
	__m128 __vectorcall MultiplyVectorInv(__m128 vec) const;
	__m128 __vectorcall MultiplyVectorRow(__m128 vec, UInt32 whichRow) const;
	NiMatrix33& __fastcall MultiplyMatrices(const NiMatrix33 &matB);
	NiMatrix33& __fastcall MultiplyMatricesInv(const NiMatrix33 &matB);
	NiMatrix33& __vectorcall Rotate(__m128 rot);
	NiMatrix33& __fastcall Transpose(NiMatrix33 &out);

	void Dump() const;
};

// 10
struct NiQuaternion
{
	float	w, x, y, z;

	NiQuaternion() {}
	__forceinline NiQuaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}
	__forceinline NiQuaternion(const NiQuaternion &from) {*this = from;}
	__forceinline explicit NiQuaternion(const NiMatrix33 &rotMat) {*this = rotMat;}
	__forceinline explicit NiQuaternion(const NiVector3 &pry) {*this = pry;}
	__forceinline explicit NiQuaternion(const AxisAngle &axisAngle) {*this = axisAngle;}
	__forceinline explicit NiQuaternion(const hkQuaternion &hkQt) {*this = hkQt;}
	__forceinline explicit NiQuaternion(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiQuaternion &&rhs)
	{
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
	__forceinline void operator=(const NiQuaternion &rhs) {SetPS(rhs.PS());}
	__forceinline void operator=(const NiMatrix33 &rotMat) {FromRotationMatrix(rotMat);}
	__forceinline void operator=(const NiVector3 &pry) {FromEulerPRY(pry.PS());}
	__forceinline void operator=(const AxisAngle &axisAngle) {FromAxisAngle(axisAngle);}
	void operator=(const hkQuaternion &hkQt);

	__forceinline NiQuaternion& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&w, rhs);
		return *this;
	}

	__forceinline __m128 operator+(const NiQuaternion &rhs) const {return PS() + rhs.PS();}

	__forceinline __m128 operator-(const NiQuaternion &rhs) const {return PS() - rhs.PS();}

	__forceinline __m128 operator*(float s) const {return PS() * _mm_set_ps1(s);}
	__forceinline __m128 operator*(const NiQuaternion &rhs) const {return MultiplyQuaternion(rhs);}
	__forceinline __m128 operator*(__m128 vec) const {return MultiplyVector(vec);}

	__forceinline NiQuaternion& operator+=(const NiQuaternion &rhs) {return SetPS(*this + rhs);}
	__forceinline NiQuaternion& operator-=(const NiQuaternion &rhs) {return SetPS(*this - rhs);}
	__forceinline NiQuaternion& operator*=(float s) {return SetPS(*this * s);}
	__forceinline NiQuaternion& operator*=(const NiQuaternion &rhs) {return SetPS(*this * rhs);}

	__forceinline bool operator==(const NiQuaternion &rhs) const {return Equal_V4(PS(), rhs.PS());}
	__forceinline bool operator!=(const NiQuaternion &rhs) const {return !(*this == rhs);}

	inline operator float*() {return &w;}
	__forceinline __m128 PS() const {return _mm_loadu_ps(&w);}

	NiQuaternion& __vectorcall FromEulerPRY(__m128 pry);
	NiQuaternion& __vectorcall FromEulerYPR(__m128 pry);
	NiQuaternion& __fastcall FromRotationMatrix(const NiMatrix33 &rotMat);
	float __vectorcall From2Points(__m128 pt1, __m128 pt2);
	NiQuaternion& __fastcall FromAxisAngle(const AxisAngle &axisAngle);

	__m128 __vectorcall MultiplyVector(__m128 vec) const;
	__m128 __vectorcall MultiplyQuaternion(const NiQuaternion &rhs) const;

	__forceinline __m128 Invert() const {return PS() ^ _mm_load_ps((const float*)0x10C8780);}

	__forceinline __m128 Negate() const {return PS() ^ GET_PS(2);}

	__forceinline float __vectorcall DotProduct(const NiQuaternion &rhs) const
	{
		__m128 k = _mm_setzero_ps();
		return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(PS() * rhs.PS(), k), k));
	}

	NiQuaternion& Normalize() {return SetPS(Normalize_V4(PS()));}

	__m128 __vectorcall ToEulerPRY() const;
	__m128 __vectorcall ToEulerYPR() const;

	NiQuaternion& Rotate(const NiQuaternion &rot) {return *this *= rot;}
	NiQuaternion& Rotate(const AxisAngle &rot) {return *this *= NiQuaternion(rot);}
	NiQuaternion& __vectorcall Rotate(__m128 pry);

	__m128 __vectorcall Lerp(const NiQuaternion &qb, float t) const;
	__m128 __vectorcall Slerp(const NiQuaternion &qb, float t) const;

	void Dump() const;
};

// 34
struct NiTransform
{
	NiMatrix33	rotate;		// 00
	NiVector3	translate;	// 24
	float		scale;		// 30

	NiTransform() : scale(0) {}
	NiTransform(const NiTransform &from) {*this = from;}
	NiTransform(const NiMatrix33 &rot, const NiVector3 &trnsl)
	{
		rotate = rot;
		_mm_storeu_ps(translate, trnsl.PS());
		scale = 0;
	}
	NiTransform(const NiVector3 &pry, const NiVector3 &trnsl)
	{
		rotate.FromEulerPRYInv(pry.PS());
		_mm_storeu_ps(translate, trnsl.PS());
		scale = 0;
	}

	inline void operator=(const NiTransform &rhs)
	{
		float *d = rotate, *s = rhs.rotate;
		_mm_storeu_ps(d, _mm_loadu_ps(s));
		_mm_storeu_ps(d + 4, _mm_loadu_ps(s + 4));
		_mm_storeu_ps(d + 8, _mm_loadu_ps(s + 8));
	}

	NiTransform& __fastcall RotateOrigin(const NiTransform &origin);
	NiTransform& __fastcall GetInverse(NiTransform &out);
	void Dump() const;
};

// 20
struct NiQuatTransform
{
	NiQuaternion	rotate;		// 00
	NiVector3		translate;	// 10
	float			scale;		// 1C
};

// 10
struct NiPlane
{
	NiVector3	nrm;
	float		offset;

	enum Sides
	{
		kSide_None,
		kSide_Positive,
		kSide_Negative
	};

	NiPlane() {}
	__forceinline NiPlane(float nX, float nY, float nZ, float offs) : nrm(nX, nY, nZ), offset(offs) {}
	__forceinline NiPlane(const NiPlane &rhs) {*this = rhs;}
	__forceinline explicit NiPlane(const NiVector3 &_nrm, const NiVector3 &point) : nrm(_nrm), offset(_nrm.DotProduct(point)) {}
	__forceinline explicit NiPlane(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiPlane &&rhs)
	{
		nrm.x = rhs.nrm.x;
		nrm.y = rhs.nrm.y;
		nrm.z = rhs.nrm.z;
		offset = rhs.offset;
	}
	__forceinline void operator=(const NiPlane &rhs) {SetPS(rhs.PS());}

	__forceinline NiPlane& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&nrm.x, rhs);
		return *this;
	}
	
	inline operator float*() {return &nrm.x;}
	__forceinline __m128 PS() const {return _mm_loadu_ps(&nrm.x);}

	UInt8 __fastcall CalculateSide(const NiVector3 &point) const;
};

// 10
struct NiBound
{
	NiVector3	center;
	float		radius;

	NiBound() {}
	__forceinline NiBound(float cX, float cY, float cZ, float rad) : center(cX, cY, cZ), radius(rad) {}
	__forceinline NiBound(const NiBound &rhs) {*this = rhs;}
	__forceinline explicit NiBound(const __m128 rhs) {SetPS(rhs);}
	
	__forceinline void operator=(NiBound &&rhs)
	{
		center.x = rhs.center.x;
		center.y = rhs.center.y;
		center.z = rhs.center.z;
		radius = rhs.radius;
	}
	__forceinline void operator=(const NiBound &rhs) {SetPS(rhs.PS());}

	__forceinline NiBound& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&center.x, rhs);
		return *this;
	}
	
	inline operator float*() {return &center.x;}
	__forceinline __m128 PS() const {return _mm_loadu_ps(&center.x);}
	
	UInt8 __fastcall CalculateSide(const NiPlane &plane) const;
	void Merge(const NiBound *other) {ThisCall(0xA7F3F0, this, other);}
};

// 10
struct NiViewport
{
	float	l, r, t, b;

	NiViewport() {}
	__forceinline NiViewport(float _l, float _r, float _t, float _b) : l(_l), r(_r), t(_t), b(_b) {}
	__forceinline explicit NiViewport(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiViewport &&rhs)
	{
		l = rhs.l;
		r = rhs.r;
		t = rhs.t;
		b = rhs.b;
	}

	__forceinline NiViewport& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&l, rhs);
		return *this;
	}

	__forceinline __m128 PS() const {return _mm_loadu_ps(&l);}

	void __vectorcall SetFOV(float fov);
};

// 1C
struct NiFrustum
{
	NiViewport	viewPort;	// 00
	float		dNear;		// 10
	float		dFar;		// 14
	bool		ortho;		// 18
	UInt8		pad19[3];	// 19
};

// C
struct NiColor
{
	float	r, g, b;

	NiColor() {}
	__forceinline NiColor(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	__forceinline NiColor(const NiColor &rhs) {*this = rhs;}
	__forceinline explicit NiColor(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiColor &&rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
	}
	__forceinline void operator=(const NiColor &rhs)
	{
		_mm_storeu_si64(this, _mm_loadu_si64(&rhs));
		b = rhs.b;
	}

	__forceinline NiColor& SetPS(const __m128 rhs)
	{
		_mm_storeu_si64(this, _mm_castps_si128(rhs));
		_mm_store_ss(&r, _mm_unpackhi_ps(rhs, rhs));
		return *this;
	}

	inline operator float*() {return &r;}
	__forceinline __m128 PS() const {return _mm_loadu_ps(&r);}

	void Dump() const;
};

// 10
struct NiColorAlpha
{
	float	r, g, b, a;

	NiColorAlpha() {}
	__forceinline NiColorAlpha(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	__forceinline NiColorAlpha(const NiColorAlpha &rhs) {*this = rhs;}
	__forceinline explicit NiColorAlpha(const __m128 rhs) {SetPS(rhs);}

	__forceinline void operator=(NiColorAlpha &&rgba)
	{
		r = rgba.r;
		g = rgba.g;
		b = rgba.b;
		a = rgba.a;
	}
	__forceinline void operator=(const NiColorAlpha &rhs) {SetPS(rhs.PS());}

	__forceinline NiColorAlpha& SetPS(const __m128 rhs)
	{
		_mm_storeu_ps(&r, rhs);
		return *this;
	}

	__forceinline NiColorAlpha& operator*=(float value) {return SetPS(_mm_mul_ps(PS(), _mm_set_ps1(value)));}

	inline operator float*() {return &r;}
	__forceinline __m128 PS() const {return _mm_loadu_ps(&r);}
};

// 64
struct NiFrustumPlanes
{
	enum PlaneIndex
	{
		kPlaneIdx_Near,
		kPlaneIdx_Far,
		kPlaneIdx_Left,
		kPlaneIdx_Right,
		kPlaneIdx_Top,
		kPlaneIdx_Bottom
	};
	
	enum PlaneBit
	{
		kPlaneBit_Near =	1,
		kPlaneBit_Far =		2,
		kPlaneBit_Left =	4,
		kPlaneBit_Right =	8,
		kPlaneBit_Top =		0x10,
		kPlaneBit_Bottom =	0x20
	};

	NiPlane		cullingPlanes[6];
	UInt32		activePlanes;	//	Bitmask

	void Set(NiCamera *camera);
};

// 06
struct NiTriangle
{
	UInt16		point1;
	UInt16		point2;
	UInt16		point3;

	NiTriangle(UInt16 pt1, UInt16 pt2, UInt16 pt3) : point1(pt1), point2(pt2), point3(pt3) {}

	void Dump() const;
};

template <typename T> struct NiRect
{
	T	left, right, top, bottom;

	NiRect() {}
	NiRect(T _left, T _right, T _top, T _bottom) : left(_left), right(_right), top(_top), bottom(_bottom) {}
};

// 10
// NiTArrays are slightly weird: they can be sparse
// this implies that they can only be used with types that can be NULL?
// not sure on the above, but some code only works if this is true
// this can obviously lead to fragmentation, but the accessors don't seem to care
// weird stuff
template <typename T_Data> class NiTArray
{
	Use_ArrayUtils(NiTArray, T_Data)
public:
	virtual void	*Destroy(UInt32 doFree);

	T_Data		*data;			// 04
	UInt16		capacity;		// 08 - init'd to size of preallocation
	UInt16		firstFreeEntry;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16		numObjs;		// 0C - init'd to 0
	UInt16		growSize;		// 0E - init'd to size of preallocation

	T_Data operator[](UInt32 idx) {return data[idx];}

	UInt16 Size() const {return firstFreeEntry;}
	bool Empty() const {return !firstFreeEntry;}
	T_Data *Data() const {return const_cast<T_Data*>(data);}

	__forceinline int Append(T_Data *item)
	{
		return ThisCall<int>(0xA5EB20, this, item);
	}
	__forceinline void AddAtIndex(UInt32 index, T_Data *item)
	{
		ThisCall(0x4B0310, this, index, item);
	}
	__forceinline void SetCapacity(UInt16 newCapacity)
	{
		ThisCall(0x8696E0, this, newCapacity);
	}

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

		Iterator(NiTArray &source) : pData(source.data), count(source.firstFreeEntry) {}
	};

	Iterator Begin() {return Iterator(*this);}
};

// 18
// an NiTArray that can go above 0xFFFF, probably with all the same weirdness
// this implies that they make fragmentable arrays with 0x10000 elements, wtf
template <typename T_Data> class NiTLargeArray
{
	Use_ArrayUtils(NiTLargeArray, T_Data)
public:
	virtual void	*Destroy(UInt32 doFree);

	T_Data	*data;			// 04
	UInt32	capacity;		// 08 - init'd to size of preallocation
	UInt32	firstFreeEntry;	// 0C - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt32	numObjs;		// 10 - init'd to 0
	UInt32	growSize;		// 14 - init'd to size of preallocation

	T_Data operator[](UInt32 idx) {return data[idx];}

	UInt32 Size() const {return firstFreeEntry;}
	bool Empty() const {return !firstFreeEntry;}
	T_Data *Data() const {return const_cast<T_Data*>(data);}

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

		Iterator(NiTLargeArray &source) : pData(source.data), count(source.firstFreeEntry) {}
	};

	Iterator Begin() {return Iterator(*this);}
};

// 10
template <typename T_Data> class NiTPtrMap
{
	Use_HashMapUtils(NiTPtrMap)

	struct Entry
	{
		Entry		*next;
		UInt32		key;
		T_Data		*data;
	};

	struct Bucket
	{
		Entry		*entries;

		UInt32 Size() const
		{
			UInt32 size = 0;
			for (Entry *pEntry = entries; pEntry; pEntry = pEntry->next, size++);
			return size;
		}
	};

	UInt32		m_numBuckets;	// 04
	Bucket		*m_buckets;		// 08
	UInt32		m_numItems;		// 0C

	Bucket *GetBuckets() const {return m_buckets;}
	Bucket *End() const {return m_buckets + m_numBuckets;}

public:
	virtual void	Destroy(bool doFree);
	virtual UInt32	CalculateBucket(UInt32 key);
	virtual bool	CompareKey(UInt32 lhs, UInt32 rhs);
	virtual void	FillEntry(Entry *entry, UInt32 key, T_Data data);
	virtual void	FreeKey(Entry *entry);
	virtual Entry	*AllocNewEntry();
	virtual void	FreeEntry(Entry *entry);

	UInt32 Size() const {return m_numItems;}
	bool Empty() const {return !m_numItems;}
	UInt32 BucketCount() const {return m_numBuckets;}

	bool HasKey(UInt32 key) const
	{
		for (Entry *entry = m_buckets[key % m_numBuckets]; entry; entry = entry->next)
			if (key == entry->key) return true;
		return false;
	}

	T_Data *Lookup(UInt32 key) const;
	void Insert(UInt32 key, T_Data value);

	class Iterator
	{
		NiTPtrMap		*table;
		Bucket			*bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Bucket *end = table->End(); bucket != end; bucket++)
				if (entry = bucket->entries) break;
		}

	public:
		Iterator(NiTPtrMap &_table) : table(&_table), bucket(table->m_buckets), entry(nullptr) {FindNonEmpty();}

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
		UInt32 Key() const {return entry->key;}
	};

	Iterator Begin() {return Iterator(*this);}
};

template <typename T_Data>
__declspec(naked) T_Data *NiTPtrMap<T_Data>::Lookup(UInt32 key) const
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

UInt32 *NiTPtrMap<UInt32>::Lookup(UInt32 key) const;

template <typename T_Data>
__declspec(naked) void NiTPtrMap<T_Data>::Insert(UInt32 key, T_Data value)
{
	__asm
	{
		mov		eax, [esp+4]
		xor		edx, edx
		div		dword ptr [ecx+4]
		mov		eax, [ecx+8]
		lea		eax, [eax+edx*4]
		push	eax
		inc		dword ptr [ecx+0xC]
		CALL_EAX(0x43A010)
		pop		ecx
		mov		edx, [ecx]
		mov		[eax], edx
		mov		edx, [esp+4]
		mov		[eax+4], edx
		mov		edx, [esp+8]
		mov		[eax+8], edx
		mov		[ecx], eax
		retn	8
	}
}

// 10
// todo: NiTPointerMap should derive from this
// cleaning that up now could cause problems, so it will wait

template <typename T_Key, typename T_Data> class NiTMap
{
	Use_HashMapUtils(NiTMap)

	struct Entry
	{
		Entry		*next;
		T_Key		key;
		T_Data		data;
	};

	struct Bucket
	{
		Entry		*entries;

		UInt32 Size() const
		{
			UInt32 size = 0;
			for (Entry *pEntry = entries; pEntry; pEntry = pEntry->next, size++);
			return size;
		}
	};

	UInt32		numBuckets;	// 04
	Bucket		*buckets;	// 08
	UInt32		numItems;	// 0C

	Bucket *GetBuckets() const {return buckets;}
	Bucket *End() const {return buckets + numBuckets;}

public:
	/*00*/virtual void		Destroy(bool doFree);
	/*04*/virtual UInt32	CalculateBucket(T_Key key);
	/*08*/virtual bool		Equal(T_Key key1, T_Key key2);
	/*0C*/virtual void		FillEntry(Entry *entry, T_Key key, T_Data data);
	/*10*/virtual void		FreeKey(Entry *entry);
	/*14*/virtual Entry		*AllocNewEntry();
	/*18*/virtual void		FreeEntry(Entry *entry);

	UInt32 Size() const {return numItems;}
	bool Empty() const {return !numItems;}
	UInt32 BucketCount() const {return numBuckets;}

	T_Data __fastcall Lookup(T_Key key) const;

	void FreeBuckets();

	class Iterator
	{
		NiTMap			*table;
		Bucket			*bucket;
		Entry			*entry;

		void FindNonEmpty()
		{
			for (Bucket *end = table->End(); bucket != end; bucket++)
				if (entry = bucket->entries) break;
		}

	public:
		Iterator(NiTMap &_table) : table(&_table), bucket(table->buckets), entry(nullptr) {FindNonEmpty();}

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
		Entry *GetEntry() {return entry;}
	};

	Iterator Begin() {return Iterator(*this);}
};

template <typename T_Key, typename T_Data>
__declspec(naked) T_Data __fastcall NiTMap<T_Key, T_Data>::Lookup(T_Key key) const
{
	__asm
	{
		push	ebx
		push	esi
		push	edi
		mov		ebx, ecx
		mov		edi, edx
		push	edx
		mov		esi, [ecx]
		call	dword ptr [esi+4]
		mov		edx, [ebx+8]
		mov		ebx, [esi+8]
		mov		esi, [edx+eax*4]
		ALIGN 16
	iterHead:
		test	esi, esi
		jz		retnNull
		push	dword ptr [esi+4]
		push	edi
		call	ebx
		mov		ecx, esi
		mov		esi, [esi]
		test	al, al
		jz		iterHead
		mov		eax, [ecx+8]
		pop		edi
		pop		esi
		pop		ebx
		retn
	retnNull:
		xor		eax, eax
		pop		edi
		pop		esi
		pop		ebx
		retn
	}
}

template <typename T_Key, typename T_Data>
__declspec(naked) void NiTMap<T_Key, T_Data>::FreeBuckets()
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
		and		dword ptr [ebx+0xC], 0
		mov		ebx, 0x11C5F58
		xorps	xmm0, xmm0
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
		movlps	[eax+4], xmm0
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
		and		dword ptr [ecx], 0
	inUse:
		pop		edi
		pop		esi
		pop		ebx
	done:
		retn
	}
}

UInt32 NiTMap<UInt32, UInt32>::Lookup(UInt32 key) const;
void NiTMap<UInt32, UInt32>::FreeBuckets();

// 14
template <typename T_Data>
class NiTStringPointerMap : public NiTMap<const char*, T_Data*>
{
public:
	UInt8		byte10;
	UInt8		pad11[3];
};