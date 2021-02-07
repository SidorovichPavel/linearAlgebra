#pragma once

#define COL_MAJOR

#include<src/MathObjects/Vector.h>
#include<src/MathObjects/Matrix.h>
#include<src/glm/glm.hpp>
#include<src/glm/ext.hpp>

namespace la
{
	constexpr double pi_d = 3.14159265358979323846;
	constexpr float pi_f = 3.14159265f;

	float dot(const vec3& v1, const vec3& v2);
	float dot2(const vec3& v);
	float length(const vec3& v);
	template <class T>
	float rad(T deg)
	{
		return static_cast<float>(deg) * pi_f / 180.f;
	}

	template<class _Ty>
	struct clean
	{
		using type = _Ty;
	};
	template<class _Ty>
	struct clean<const _Ty>
	{
		using type = _Ty;
	};
	template<class _Ty>
	struct clean<const _Ty&>
	{
		using type = _Ty;
	};
	template<class _Ty>
	struct clean<_Ty&>
	{
		using type = _Ty;
	};

	template<class T, size_t _Dim>//сука это я хотел упростить вывод типа функции)) ага. упростил.
	auto normalize(const hide::Vector<T, _Dim>& _Vec)->typename clean<decltype(_Vec)>::type
	{
		float rlen = 1.f / _Vec.lengthf();
		return _Vec * rlen;
	}

	template<class T, size_t M, size_t N>
	hide::Matrix<T, N, M> transpose(const hide::Matrix<T, M, N>& mat)
	{
		hide::Matrix<T, N, M> result;
		auto ptr = (T*)mat.data();
		for (auto i = 0; i < M; ++i)
			for (auto j = 0; j < N; ++j, ++ptr)
				result[j][i] = *ptr;

		return result;
	}

	mat4 translate(const mat4& M, const vec3& v);

	mat4 scale(mat4& mat, const vec3& vec);
	
	mat4 rotate(mat4& mat, const vec3& vec, float angle);
	
	mat4 perspeсtive(float near, float far, float aspect, float fov);
}
