#pragma once

#include<src/MathObjects/vector.h>
#include<src/MathObjects/Matrix.h>
#include<src/MathObjects/array_wrapper.h>

namespace la
{
	template<class T, size_t M, size_t N>
	hide::Matrix<T, M, N> transpose(const hide::Matrix<T, M, N>& mat)
	{
		hide::Matrix<T, M, N> result;
		auto ptr = mat.data();
		for (auto i = 0; i < M; ++i)
			for (auto j = 0; j < N; ++j, ++ptr)
				result[j][i] = *ptr;

		return result;
	}
	template<class T, size_t M>
	void transpose(hide::Matrix<T, M, M>& mat)
	{
		for (auto i = 1; i < M; ++i)
			for (auto j = 0; j < i; ++j)
				std::swap(mat[i][j], mat[j][i]);
	}

	float dot(const vec3& v1, const vec3& v2);
	vec3 operator*(const vec3& v1, const vec3& v2);
	mat4 operator*(mat4& A, mat4& B);
	vec4 operator*(mat4& A, vec4& vec);


}