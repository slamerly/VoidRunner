#pragma once
#include "Vector3.h"

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion()
	{
		*this = Quaternion::identity;
	}

	// This directly sets the quaternion components --
	// don't use for axis/angle
	explicit Quaternion(float inX, float inY, float inZ, float inW);

	// Construct the quaternion from an axis and angle
	// It is assumed that axis is already normalized,
	// and the angle is in radians
	explicit Quaternion(const Vector3& axis, float angle);

	void set(float inX, float inY, float inZ, float inW);
	void conjugate();
	void normalize();

	friend Quaternion operator*(const Quaternion& left, const Quaternion& right)
	{
		Quaternion res;
		res.x = left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y;
		res.y = left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x;
		res.z = left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w;
		res.w = left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z;
		return res;
	}

	float lengthSq() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	float length() const
	{
		return Maths::sqrt(lengthSq());
	}

	// Normalize the provided quaternion
	static Quaternion normalize(const Quaternion& q)
	{
		Quaternion retVal = q;
		retVal.normalize();
		return retVal;
	}

	// Calcul the inverse
	static Quaternion inverse(const Quaternion& q)
	{
		Quaternion conj = q;
		conj.conjugate();

		Quaternion retVal;
		retVal.x = conj.x / q.lengthSq();
		retVal.y = conj.y / q.lengthSq();
		retVal.z = conj.z / q.lengthSq();
		retVal.w = conj.w / q.lengthSq();

		return retVal;
	}

	// Linear interpolation
	static Quaternion lerp(const Quaternion& a, const Quaternion& b, float f)
	{
		Quaternion retVal;
		retVal.x = Maths::lerp(a.x, b.x, f);
		retVal.y = Maths::lerp(a.y, b.y, f);
		retVal.z = Maths::lerp(a.z, b.z, f);
		retVal.w = Maths::lerp(a.w, b.w, f);
		retVal.normalize();
		return retVal;
	}

	static float dot(const Quaternion& a, const Quaternion& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	// Spherical Linear Interpolation
	static Quaternion slerp(const Quaternion& a, const Quaternion& b, float f)
	{
		float rawCosm = Quaternion::dot(a, b);

		float cosom = -rawCosm;
		if (rawCosm >= 0.0f)
		{
			cosom = rawCosm;
		}

		float scale0, scale1;

		if (cosom < 0.9999f)
		{
			const float omega = Maths::acos(cosom);
			const float invSin = 1.f / Maths::sin(omega);
			scale0 = Maths::sin((1.f - f) * omega) * invSin;
			scale1 = Maths::sin(f * omega) * invSin;
		}
		else
		{
			// Use linear interpolation if the quaternions
			// are collinear
			scale0 = 1.0f - f;
			scale1 = f;
		}

		if (rawCosm < 0.0f)
		{
			scale1 = -scale1;
		}

		Quaternion retVal;
		retVal.x = scale0 * a.x + scale1 * b.x;
		retVal.y = scale0 * a.y + scale1 * b.y;
		retVal.z = scale0 * a.z + scale1 * b.z;
		retVal.w = scale0 * a.w + scale1 * b.w;
		retVal.normalize();
		return retVal;
	}

	// Concatenate
	// Rotate by q FOLLOWED BY p
	static Quaternion concatenate(const Quaternion& q, const Quaternion& p)
	{
		Quaternion retVal;

		// Vector component is:
		// ps * qv + qs * pv + pv x qv
		Vector3 qv(q.x, q.y, q.z);
		Vector3 pv(p.x, p.y, p.z);
		Vector3 newVec = p.w * qv + q.w * pv + Vector3::cross(pv, qv);
		retVal.x = newVec.x;
		retVal.y = newVec.y;
		retVal.z = newVec.z;

		// Scalar component is:
		// ps * qs - pv . qv
		retVal.w = p.w * q.w - Vector3::dot(pv, qv);

		return retVal;
	}

	Vector3 toEuler() const
	{
		Vector3 euler;

		// extract quaternion value
		float ysqr = y * y;

		// Calcul Pitch
		float t0 = +2.0f * (w * x + y * z);
		float t1 = +1.0f - 2.0f * (x * x + ysqr);
		euler.y = atan2(t0, t1);  // Pitch

		// Calcul Yaw
		float t2 = +2.0f * (w * y - z * x);
		t2 = Maths::clamp(t2, -1.0f, 1.0f);
		euler.z = asin(t2);  // Yaw

		// Calcul Roll
		float t3 = +2.0f * (w * z + x * y);
		float t4 = +1.0f - 2.0f * (ysqr + z * z);
		euler.x = atan2(t3, t4);  // Roll

		return euler;
	}

	static const Quaternion identity;
};

