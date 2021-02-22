#include "Quaternion.h"
#include <src/linearAlgebra.h>

namespace la
{
	float dot(const vec3& v1, const vec3& v2);



	Quaternion::Quaternion()
		:
		a(0.f)
	{}

	Quaternion::Quaternion(const float _a, const vec3& _v)
		:
		a(_a),
		u(_v)
	{}

	Quaternion::Quaternion(const vec3& _v, const float _angle)
	{
		a = cosf(_angle / 2);
		u = normalize(_v) * sinf(_angle / 2);
	}

	float Quaternion::arg()
	{
		return acosf(a / mod(*this));
	}





	Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
	{
		Quaternion result;
		result.a = q1.a * q2.a - dot(q1.u, q2.u);
		result.u = (q1.a * q2.u) + (q2.a * q1.u) + (q1.u * q2.u);
		return result;
	}

	Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
	{
		Quaternion result;
		result.a = q1.a + q2.a;
		result.u = q1.u + q2.u;
		return result;
	}

	Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
	{
		Quaternion result;
		result.a = q1.a - q2.a;
		result.u = q1.u - q2.u;
		return result;
	}

	Quaternion operator*(const Quaternion& q, const float k)
	{
		return Quaternion(q.a * k, q.u * k);
	}

	Quaternion operator/(const Quaternion& q, const float k)
	{
		auto ik = 1.f / k;
		return q * ik;
	}

	float mod(const Quaternion& q)
	{
		float res = q.a * q.a;
		auto& v = q.u;
		res += v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		return sqrtf(res);
	}

	Quaternion get_conjugate(const Quaternion& q)
	{
		return Quaternion(q.a, -q.u);
	}

	Quaternion get_inverse(const Quaternion& q)
	{
		return get_conjugate(q) / (mod(q) * mod(q));
	}

	vec3 operator*(const vec3& v, const Quaternion& q)
	{
		Quaternion qv(0, v);
		Quaternion iq = get_inverse(q);
		Quaternion t;
		t = q * qv * iq;
		return t.u;
	}

	vec3 operator*(const Quaternion& q, const vec3& v)
	{
		return vec3();
	}





}