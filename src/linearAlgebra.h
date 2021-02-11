#pragma once

#define COL_MAJOR

#include<src/MathObjects/Vector.h>
#include<src/MathObjects/Matrix.h>
//#include<src/glm/glm.hpp>
//#include<src/glm/ext.hpp>

namespace la
{
	constexpr double pi_d = 3.14159265358979323846;
	constexpr float pi_f = 3.14159265f;

	place float dot(const vec3& v1, const vec3& v2);
	place float dot2(const vec3& v);
	place float length(const vec3& v);
	template <class T>
	place float rad(T deg)
	{
		return static_cast<float>(deg) * pi_f / 180.f;
	}
	template<class T, size_t Dim>
	place std::pair<hide::Matrix<T, Dim, Dim>, hide::Matrix<T, Dim, Dim>>
		lu_decomposition(const hide::Matrix<T, Dim, Dim>& A)
	{
		using Matrix = hide::Matrix<T, Dim, Dim>;
		Matrix L(1.f), U(1.f);

		auto sum = [&] (auto bound_i, auto bound_j, auto count)
		{
			T sum = static_cast<T>(0);
			for (auto k = 0; k < count; ++k)
				sum += L[bound_i][k] * U[k][bound_j];

			return sum;
		};

		for (auto i = 0; i < Dim; ++i)
		{
			for (auto j = 0; j < Dim; ++j)
			{
				if (i <= j)
					U[i][j] = A[i][j] - sum(i, j, i);
				else
					L[i][j] = (A[i][j] - sum(i, j, j)) / U[j][j];
			}
		}
		return std::pair<Matrix, Matrix>(L, U);
	}
	template<class T, size_t Dim>
	place T det(const hide::Matrix<T, Dim, Dim>& mat) noexcept
	{
		auto mats = lu_decomposition(mat);
		T detL = static_cast<T>(1), detU = static_cast<T>(1);
		for (auto i = 0; i < Dim; ++i)
			detL *= mats.first[i][i];
		for (auto i = 0; i < Dim; ++i)
			detU *= mats.second[i][i];
		
		return detL * detU;
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
	place auto normalize(const hide::Vector<T, _Dim>& _Vec)->typename clean<decltype(_Vec)>::type
	{
		float rlen = 1.f / _Vec.lengthf();
		return _Vec * rlen;
	}

	template<class T, size_t M, size_t N>
	place hide::Matrix<T, N, M> transpose(const hide::Matrix<T, M, N>& mat)
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

	mat4 lock_at(const vec3& pos, const vec3& target, const vec3& up);

}
