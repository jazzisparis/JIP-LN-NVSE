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
	NiPoint2(float _x, float _y) : x(_x), y(_y) {}
	NiPoint2(const NiPoint2 &rhs) {*this = rhs;}

	inline void operator=(const NiPoint2 &rhs) {_mm_storel_pi((__m64*)this, rhs.PS());}
	inline void operator=(NiPoint2 &&rhs) {_mm_storel_pi((__m64*)this, rhs.PS());}

	inline operator float*() {return &x;}

	inline __m128 PS() const {return _mm_loadu_ps(&x);}

	void Dump() const;
};

#define SET_V3	_mm_storel_pi((__m64*)this, m);	\
	_mm_store_ss(&z, _mm_unpackhi_ps(m, m));

// 0C
struct NiVector3
{
	float	x, y, z;

	NiVector3() {}
	NiVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	NiVector3(const NiVector3 &rhs) {*this = rhs;}
	NiVector3(const NiVector4 &rhs) {*this = rhs;}
	NiVector3(const NiMatrix33 &rhs) {*this = rhs;}
	NiVector3(const NiQuaternion &rhs) {*this = rhs;}
	NiVector3(const __m128 rhs)
	{
		_mm_storel_pi((__m64*)this, rhs);
		_mm_store_ss(&z, _mm_unpackhi_ps(rhs, rhs));
	}

	inline void operator=(const NiVector3 &rhs)
	{
		__m128 m = _mm_loadu_ps((const float*)&rhs);
		SET_V3
	}
	inline void operator=(NiVector3 &&rhs)
	{
		__m128 m = _mm_loadu_ps((const float*)&rhs);
		SET_V3
	}
	inline void operator=(const NiVector4 &rhs)
	{
		__m128 m = _mm_loadu_ps((const float*)&rhs);
		SET_V3
	}
	inline void operator=(const NiPoint2 &rhs) {_mm_storel_pi((__m64*)this, rhs.PS());}

	void operator=(const NiMatrix33 &from);
	void operator=(const NiQuaternion &from);

	/*inline void __vectorcall operator=(const __m128 rhs)
	{
		_mm_storel_pi((__m64*)this, rhs);
		_mm_store_ss(&z, _mm_unpackhi_ps(rhs, rhs));
	}*/

	inline NiVector3& operator+=(const NiVector3 &rhs)
	{
		__m128 m = _mm_add_ps(PS(), rhs.PS());
		SET_V3
		return *this;
	}
	inline NiVector3& operator+=(const NiVector4 &rhs)
	{
		__m128 m = _mm_add_ps(PS(), _mm_loadu_ps((const float*)&rhs));
		SET_V3
		return *this;
	}
	inline NiVector3& operator+=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		m = _mm_add_ps(PS(), _mm_shuffle_ps(m, m, 0x40));
		SET_V3
		return *this;
	}
	inline NiVector3& operator-=(const NiVector3 &rhs)
	{
		__m128 m = _mm_sub_ps(PS(), rhs.PS());
		SET_V3
		return *this;
	}
	inline NiVector3& operator-=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		m = _mm_sub_ps(PS(), _mm_shuffle_ps(m, m, 0x40));
		SET_V3
		return *this;
	}
	inline NiVector3& operator*=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		m = _mm_mul_ps(PS(), _mm_shuffle_ps(m, m, 0x40));
		SET_V3
		return *this;
	}
	inline NiVector3& operator*=(const NiVector3 &rhs)
	{
		__m128 m = _mm_mul_ps(PS(), rhs.PS());
		SET_V3
		return *this;
	}
	inline NiVector3& __vectorcall operator*=(const __m128 packedPS)
	{
		__m128 m = _mm_mul_ps(PS(), packedPS);
		SET_V3
		return *this;
	}
	inline NiVector3& operator*=(const NiMatrix33 &mat) {return MultiplyMatrix(mat);}
	inline NiVector3& operator*=(const NiQuaternion &qt) {return MultiplyQuaternion(qt);}

	inline operator float*() {return &x;}
	inline operator NiPoint2&() const {return *(NiPoint2*)this;}

	inline __m128 PS() const {return _mm_loadu_ps(&x);}

	inline bool operator==(const NiVector3 &rhs) const {return Equal_V3(PS(), rhs.PS());}
	inline bool operator!=(const NiVector3 &rhs) const {return !(*this == rhs);}

	float __vectorcall Length() const;
	NiVector3& __fastcall MultiplyMatrix(const NiMatrix33 &mat);
	NiVector3& __fastcall MultiplyMatrixInv(const NiMatrix33 &mat);
	NiVector3& __fastcall MultiplyMatrixRow(const NiMatrix33 &rotMatrix, UInt32 whichRow);
	NiVector3& __fastcall MultiplyQuaternion(const NiQuaternion &qt);
	NiVector3& Normalize();
	__m128 __vectorcall CrossProduct(const NiVector3 &vB) const;
	__m128 __vectorcall Interpolate(const NiVector3 &vB, float t) const;
	__m128 __vectorcall GetAnglesBetweenPoints(const NiVector3 &pt2) const;

	void Dump() const;
};

float __vectorcall Point2Distance(const NiVector3 &pt1, const NiVector3 &pt2);
float __vectorcall Point3Distance(const NiVector3 &pt1, const NiVector3 &pt2);
__m128 __vectorcall TransformWorldToLocal(const NiVector3 &origin, const NiVector3 &target, __m128 rotation);

struct NiVector4
{
	float	x, y, z, w;

	NiVector4() {}
	NiVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	NiVector4(const NiVector4 &rhs) {*this = rhs;}
	NiVector4(const NiVector3 &rhs) {*this = rhs;}
	NiVector4(const __m128 rhs) {*this = rhs;}

	inline void operator=(NiVector4 &&rhs) {_mm_storeu_ps(&x, rhs.PS());}
	inline void operator=(const NiVector4 &rhs) {_mm_storeu_ps(&x, rhs.PS());}
	inline void operator=(const NiVector3 &rhs)
	{
		_mm_storeu_ps(&x, _mm_and_ps(rhs.PS(), GET_PS(4)));
	}
	inline void __vectorcall operator=(const __m128 rhs) {_mm_storeu_ps(&x, rhs);}

	inline NiVector4& operator+=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		_mm_storeu_ps(&x, _mm_add_ps(PS(), _mm_shuffle_ps(m, m, 0)));
		return *this;
	}
	inline NiVector4& operator+=(const NiVector3 &rhs)
	{
		__m128 m = _mm_and_ps(rhs.PS(), GET_PS(4));
		_mm_storeu_ps(&x, _mm_add_ps(m, PS()));
		return *this;
	}
	inline NiVector4& operator+=(const NiVector4 &rhs)
	{
		_mm_storeu_ps(&x, _mm_add_ps(PS(), rhs.PS()));
		return *this;
	}
	inline NiVector4& __vectorcall operator+=(const __m128 packedPS)
	{
		_mm_storeu_ps(&x, _mm_add_ps(PS(), packedPS));
		return *this;
	}

	inline NiVector4& operator-=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		_mm_storeu_ps(&x, _mm_sub_ps(PS(), _mm_shuffle_ps(m, m, 0)));
		return *this;
	}
	inline NiVector4& operator-=(const NiVector3 &rhs)
	{
		__m128 m = _mm_and_ps(rhs.PS(), GET_PS(4));
		_mm_storeu_ps(&x, _mm_sub_ps(m, PS()));
		return *this;
	}
	inline NiVector4& operator-=(const NiVector4 &rhs)
	{
		_mm_storeu_ps(&x, _mm_sub_ps(PS(), rhs.PS()));
		return *this;
	}
	inline NiVector4& __vectorcall operator-=(const __m128 packedPS)
	{
		_mm_storeu_ps(&x, _mm_sub_ps(PS(), packedPS));
		return *this;
	}

	inline NiVector4& operator*=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		_mm_storeu_ps(&x, _mm_mul_ps(PS(), _mm_shuffle_ps(m, m, 0)));
		return *this;
	}
	inline NiVector4& operator*=(const NiVector3 &rhs)
	{
		__m128 m = _mm_and_ps(rhs.PS(), GET_PS(4));
		_mm_storeu_ps(&x, _mm_mul_ps(m, PS()));
		return *this;
	}
	inline NiVector4& operator*=(const NiVector4 &rhs)
	{
		_mm_storeu_ps(&x, _mm_mul_ps(PS(), rhs.PS()));
		return *this;
	}
	inline NiVector4& __vectorcall operator*=(const __m128 packedPS)
	{
		_mm_storeu_ps(&x, _mm_mul_ps(PS(), packedPS));
		return *this;
	}

	inline __m128 __vectorcall operator*(const __m128 packedPS) const
	{
		return _mm_mul_ps(PS(), packedPS);
	}

	inline operator float*() {return &x;}
	inline operator NiVector3&() const {return *(NiVector3*)this;}

	inline __m128 PS() const {return _mm_loadu_ps(&x);}

	float __vectorcall Length() const;

	__m128 __vectorcall GetTranslatedPos(const NiTransform &transfrm) const;
	bool RayCastCoords(const NiVector3 &posVector, float *rotMatRow, float maxRange, UInt32 filter);

	void Dump() const;
};

__declspec(align(16)) typedef NiVector4 AlignedVector4;

// 10
struct AxisAngle
{
	NiVector3	axis;
	float		theta;

	AxisAngle() {}
	AxisAngle(float x, float y, float z, float t) : axis(x, y, z), theta(t) {}
	AxisAngle(const NiVector3 &_axis, float t) : axis(_axis), theta(t) {}
	AxisAngle(const AxisAngle &from) {*this = from;}
	AxisAngle(const NiVector3 &pry) {*this = pry;}
	AxisAngle(const NiMatrix33 &rotMat) {*this = rotMat;}
	AxisAngle(const NiQuaternion &qt) {*this = qt;}
	AxisAngle(const __m128 rhs) {*this = rhs;}

	inline void operator=(const AxisAngle &from) {_mm_storeu_ps(&axis.x, from.PS());}
	inline void operator=(AxisAngle &&from) {_mm_storeu_ps(&axis.x, from.PS());}
	inline void operator=(const NiVector3 &pry) {FromEulerPRY(pry.PS());}
	inline void operator=(const NiMatrix33 &rotMat) {FromRotationMatrix(rotMat);}
	inline void operator=(const NiQuaternion &qt) {FromQuaternion(qt);}
	inline void __vectorcall operator=(const __m128 rhs) {_mm_storeu_ps(&axis.x, rhs);}

	inline operator float*() {return &axis.x;}

	inline __m128 PS() const {return _mm_loadu_ps(&axis.x);}

	inline bool operator==(const AxisAngle &rhs) const {return Equal_V4(PS(), rhs.PS());}
	inline bool operator!=(const AxisAngle &rhs) const {return !(*this == rhs);}

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
	NiMatrix33(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		cr[0][0] = m00;
		cr[0][1] = m10;
		cr[0][2] = m20;
		cr[1][0] = m01;
		cr[1][1] = m11;
		cr[1][2] = m21;
		cr[2][0] = m02;
		cr[2][1] = m12;
		cr[2][2] = m22;
	}
	NiMatrix33(const NiMatrix33 &from) {*this = from;}
	NiMatrix33(const NiVector3 &pry) {*this = pry;}
	NiMatrix33(__m128 pry) {*this = pry;}
	NiMatrix33(const NiQuaternion &qt) {*this = qt;}
	NiMatrix33(const AxisAngle &axisAngle) {*this = axisAngle;}
	NiMatrix33(const hkMatrix3x4 &inMat) {*this = inMat;}

	inline void operator=(const NiMatrix33 &rhs)
	{
		_mm_storeu_ps(&cr[0][0], _mm_loadu_ps(&rhs.cr[0][0]));
		_mm_storeu_ps(&cr[1][1], _mm_loadu_ps(&rhs.cr[1][1]));
		cr[2][2] = rhs.cr[2][2];
	}
	inline void operator=(NiMatrix33 &&rhs)
	{
		_mm_storeu_ps(&cr[0][0], _mm_loadu_ps(&rhs.cr[0][0]));
		_mm_storeu_ps(&cr[1][1], _mm_loadu_ps(&rhs.cr[1][1]));
		cr[2][2] = rhs.cr[2][2];
	}
	inline void operator=(const NiVector3 &pry) {FromEulerPRY(pry.PS());}
	inline void __vectorcall operator=(__m128 pry) {FromEulerPRY(pry);}
	inline void operator=(const NiQuaternion &qt) {FromQuaternion(qt);}
	inline void operator=(const AxisAngle &axisAngle) {FromAxisAngle(axisAngle);}
	void __fastcall operator=(const hkMatrix3x4 &inMat);

	inline NiMatrix33& operator*=(const NiMatrix33 &rhs) {return MultiplyMatrices(rhs);}

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
	float __vectorcall ExtractPitch(UInt32 invert) const;
	float __vectorcall ExtractRoll(UInt32 invert) const;
	float __vectorcall ExtractYaw(UInt32 invert) const;
	NiMatrix33& __vectorcall FromEulerPRY(__m128 pry);
	NiMatrix33& __vectorcall FromEulerPRYInv(__m128 pry);
	NiMatrix33& __fastcall FromQuaternion(const NiQuaternion &qt);
	NiMatrix33& __fastcall FromAxisAngle(const AxisAngle &axisAngle);
	NiMatrix33& __fastcall MultiplyMatrices(const NiMatrix33 &matB);
	NiMatrix33& __vectorcall Rotate(__m128 rot);
	NiMatrix33& Transpose();
	float __vectorcall From2Points(const NiVector3 &pt1, const NiVector3 &pt2);

	void Dump() const;
};

// 10
struct NiQuaternion
{
	float	w, x, y, z;

	NiQuaternion() {}
	NiQuaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}
	NiQuaternion(const NiQuaternion &from) {*this = from;}
	NiQuaternion(const NiMatrix33 &rotMat) {*this = rotMat;}
	NiQuaternion(const NiVector3 &pry) {*this = pry;}
	NiQuaternion(const AxisAngle &axisAngle) {*this = axisAngle;}
	NiQuaternion(const hkQuaternion &hkQt) {*this = hkQt;}
	NiQuaternion(const __m128 rhs) {*this = rhs;}

	inline void operator=(const NiQuaternion &rhs) {_mm_storeu_ps(&w, rhs.PS());}
	inline void operator=(NiQuaternion &&rhs) {_mm_storeu_ps(&w, rhs.PS());}
	inline void operator=(const NiMatrix33 &rotMat) {FromRotationMatrix(rotMat);}
	inline void operator=(const NiVector3 &pry) {FromEulerPRY(pry.PS());}
	inline void operator=(const AxisAngle &axisAngle) {FromAxisAngle(axisAngle);}
	void operator=(const hkQuaternion &hkQt);
	inline void __vectorcall operator=(const __m128 rhs) {_mm_storeu_ps(&w, rhs);}

	inline NiQuaternion& operator+=(const NiQuaternion &rhs)
	{
		*this = _mm_add_ps(PS(), rhs.PS());
		return *this;
	}

	inline NiQuaternion& operator-=(const NiQuaternion &rhs)
	{
		*this = _mm_sub_ps(PS(), rhs.PS());
		return *this;
	}

	inline NiQuaternion& operator*=(const NiQuaternion &rhs) {return MultiplyQuaternion(rhs);}

	inline NiQuaternion& operator*=(float s)
	{
		__m128 m = _mm_load_ss(&s);
		*this = _mm_mul_ps(PS(), _mm_shuffle_ps(m, m, 0));
		return *this;
	}

	inline NiQuaternion operator+(const NiQuaternion &rhs) const
	{
		NiQuaternion res = *this;
		res += rhs;
		return res;
	}

	inline NiQuaternion operator-(const NiQuaternion &rhs) const
	{
		NiQuaternion res = *this;
		res -= rhs;
		return res;
	}

	inline NiQuaternion operator*(float s) const
	{
		NiQuaternion res = *this;
		res *= s;
		return res;
	}

	inline NiQuaternion operator*(const NiQuaternion &rhs) const
	{
		NiQuaternion res = *this;
		res *= rhs;
		return res;
	}

	inline bool operator==(const NiQuaternion &rhs) const {return Equal_V4(PS(), rhs.PS());}
	inline bool operator!=(const NiQuaternion &rhs) const {return !(*this == rhs);}

	inline __m128 PS() const {return _mm_loadu_ps(&w);}

	NiQuaternion& __fastcall FromRotationMatrix(const NiMatrix33 &rotMat);
	NiQuaternion& __vectorcall FromEulerPRY(__m128 pry);
	NiQuaternion& __fastcall FromAxisAngle(const AxisAngle &axisAngle);

	NiQuaternion& __fastcall MultiplyQuaternion(const NiQuaternion &rhs);

	inline NiQuaternion& Invert()
	{
		*this = _mm_xor_ps(PS(), _mm_load_ps((const float*)0x10C8780));
		return *this;
	}

	inline NiQuaternion& Negate()
	{
		*this = _mm_xor_ps(PS(), GET_PS(2));
		return *this;
	}

	inline float __vectorcall DotProduct(const NiQuaternion &rhs) const
	{
		__m128 m = _mm_mul_ps(PS(), rhs.PS());
		return _mm_hadd_ps(_mm_hadd_ps(m, m), m).m128_f32[0];
	}

	NiQuaternion& Normalize()
	{
		*this = Normalize_V4(PS());
		return *this;
	}

	__m128 __vectorcall ToEulerPRY() const;

	NiQuaternion& Rotate(const NiQuaternion &rot)
	{
		NiQuaternion q = rot;
		*this *= q.Invert();
		return *this;
	}

	NiQuaternion& Rotate(const NiVector3 &rot)
	{
		NiQuaternion q = rot;
		*this *= q.Invert();
		return *this;
	}

	NiQuaternion& Rotate(const AxisAngle &rot)
	{
		NiQuaternion q = rot;
		*this *= q.Invert();
		return *this;
	}

	void __vectorcall lerp(const NiQuaternion &qb, float t);
	void __vectorcall slerp(const NiQuaternion &qb, float t);

	void Dump() const;
};

// 34
struct NiTransform
{
	NiMatrix33	rotate;		// 00
	NiVector3	translate;	// 24
	float		scale;		// 30

	void Dump() const;
};

// 10
struct NiSphere
{
	float	x, y, z, radius;
};

// 10
struct NiViewport
{
	float	l, r, t, b;

	void __vectorcall SetFOV(float fov);
};

// 1C
struct NiFrustum
{
	NiViewport	viewPort;	// 00
	float		n;			// 10
	float		f;			// 14
	UInt8		o;			// 18
	UInt8		pad19[3];	// 19
};

// C
struct NiColor
{
	float	r, g, b;

	NiColor() {}
	NiColor(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	NiColor(const NiColor &rhs) {*this = rhs;}

	inline void operator=(NiColor &&rhs)
	{
		__m128 m = _mm_loadu_ps((const float*)&rhs);
		_mm_storel_pi((__m64*)this, m);
		_mm_store_ss(&b, _mm_unpackhi_ps(m, m));
	}
	inline void operator=(const NiColor &rhs)
	{
		__m128 m = _mm_loadu_ps((const float*)&rhs);
		_mm_storel_pi((__m64*)this, m);
		_mm_store_ss(&b, _mm_unpackhi_ps(m, m));
	}
	inline void operator=(const NiVector3 &rhs)
	{
		__m128 m = _mm_loadu_ps((const float*)&rhs);
		_mm_storel_pi((__m64*)this, m);
		_mm_store_ss(&b, _mm_unpackhi_ps(m, m));
	}

	inline operator float*() {return &r;}
	inline operator NiVector3() const {return *(NiVector3*)this;}
	inline __m128 PS() const {return _mm_loadu_ps(&r);}
};

// 10
struct NiColorAlpha
{
	float	r, g, b, a;

	NiColorAlpha() {}
	NiColorAlpha(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	NiColorAlpha(const NiColorAlpha &rhs) {*this = rhs;}
	explicit NiColorAlpha(const __m128 rhs) {*this = rhs;}

	inline void operator=(NiColorAlpha &&rhs) {_mm_storeu_ps(&r, rhs);}
	inline void operator=(const NiColorAlpha &rhs) {_mm_storeu_ps(&r, rhs);}
	inline void operator=(const __m128 rhs) {_mm_storeu_ps(&r, rhs);}

	inline NiColorAlpha& operator*=(float value)
	{
		__m128 m = _mm_load_ss(&value);
		_mm_storeu_ps(&r, _mm_mul_ps(_mm_loadu_ps(&r), _mm_shuffle_ps(m, m, 0)));
		return *this;
	}

	inline operator float*() {return &r;}
	inline operator NiVector4&() const {return *(NiVector4*)this;}
	inline operator __m128() const {return _mm_loadu_ps(&r);}
};

// 10
struct NiPlane
{
	NiVector3	nrm;
	float		offset;
};

// 06
struct NiTriangle
{
	UInt16		point1;
	UInt16		point2;
	UInt16		point3;

	void Dump() const;
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

	T_Data operator[](UInt32 idx) {return data[idx];}

	UInt16 Length() const {return firstFreeEntry;}

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
		friend NiTArray;

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

	T_Data operator[](UInt32 idx) {return data[idx];}

	UInt32 Length() const {return firstFreeEntry;}

	class Iterator
	{
		friend NiTLargeArray;

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
				if (entry = *bucket) break;
		}

	public:
		Iterator(NiTPointerMap &_table) : table(&_table), bucket(table->m_buckets), entry(nullptr) {FindNonEmpty();}

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

	/*00*/virtual void		Destroy(bool doFree);
	/*04*/virtual UInt32	CalculateBucket(T_Key key);
	/*08*/virtual bool		Equal(T_Key key1, T_Key key2);
	/*0C*/virtual void		FillEntry(Entry *entry, T_Key key, T_Data data);
	/*10*/virtual void		FreeKey(Entry *entry);
	/*14*/virtual Entry		*AllocNewEntry();
	/*18*/virtual void		FreeEntry(Entry *entry);

	UInt32		numBuckets;	// 04
	Entry		**buckets;	// 08
	UInt32		numItems;	// 0C

	T_Data Lookup(T_Key key)
	{
		for (Entry *entry = buckets[CalculateBucket(key)]; entry; entry = entry->next)
			if (Equal(key, entry->key)) return entry->data;
		return nullptr;
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
				if (entry = *bucket) break;
		}

	public:
		Iterator(NiTMapBase &_table) : table(&_table), bucket(table->buckets), entry(nullptr) {FindNonEmpty();}

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

template <typename T_Key, typename T_Data>
__declspec(naked) void NiTMapBase<T_Key, T_Data>::FreeBuckets()
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
		mov		dword ptr [ebx+0xC], 0
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
		movq	qword ptr [eax+4], xmm0
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
		mov		dword ptr [ecx], 0
	inUse:
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