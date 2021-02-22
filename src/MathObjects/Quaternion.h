#pragma once

#include <src/MathObjects/Vector.h>

namespace la
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(const float _a, const vec3& _v);
		Quaternion(const vec3& _v, const float _angle);

		float arg();

		friend Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
		friend Quaternion operator*(const Quaternion& q, const float k);
		friend Quaternion operator/(const Quaternion& q, const float k);

		friend vec3 operator*(const vec3& v, const Quaternion& q);

		friend float mod(const Quaternion& q);
		friend Quaternion get_conjugate(const Quaternion& q);
		friend Quaternion get_inverse(const Quaternion& q);

	private:
		float a;
		vec3 u;
	};

	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
	//Quaternion operator/(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& q, const float k);
	Quaternion operator/(const Quaternion& q, const float k);

	float mod(const Quaternion& q);
	Quaternion get_conjugate(const Quaternion& q);
	Quaternion get_inverse(const Quaternion& q);

	vec3 operator*(const vec3& v, const Quaternion& q);
	vec3 operator*(const Quaternion& q, const vec3& v);


}