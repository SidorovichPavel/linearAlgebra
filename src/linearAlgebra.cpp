#include <src/linearAlgebra.h>
#include <immintrin.h>
#include <assert.h>
#include <limits>
#include <cmath>

namespace la
{
	//calc
	place float dot(const vec3& v1, const vec3& v2)
	{
		float res = 0;
		for (auto it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end();)
		{
			res += *it1 * *it2;
			++it1;
			++it2;
		}
		return res;
	}

	place float dot2(const vec3& v)
	{
		return dot(v, v);
	}

	place float length(const vec3& v)
	{
		return std::sqrtf(dot2(v));
	}
	//calc end











	template<class T, size_t _Size, size_t __Aligned>
	struct buf
	{
		T* _Ptr;
		buf() : _Ptr((T*)_mm_malloc(_Size * sizeof(T), __Aligned)) {};
		~buf() { _mm_free(_Ptr); }
	};

	void micro_4x4(float* A, float* B, float* Res)
	{
		__m128 res0 = _mm_setzero_ps();
		__m128 res1 = _mm_setzero_ps();
		__m128 res2 = _mm_setzero_ps();
		__m128 res3 = _mm_setzero_ps();

		__m128 a0, b0;
		
		for (auto i = 0; i < 4; ++i, ++A)
		{
			b0 = _mm_loadu_ps(B);
			B += 4;
			a0 = _mm_set_ps1(A[0]);
			res0 = _mm_fmadd_ps(a0, b0, res0);
			a0 = _mm_set_ps1(A[4]);
			res1 = _mm_fmadd_ps(a0, b0, res1);
			a0 = _mm_set_ps1(A[8]);
			res2 = _mm_fmadd_ps(a0, b0, res2);
			a0 = _mm_set_ps1(A[12]);
			res3 = _mm_fmadd_ps(a0, b0, res3);
		}
		_mm_storeu_ps(Res + 0, res0);
		_mm_storeu_ps(Res + 4, res1);
		_mm_storeu_ps(Res + 8, res2);
		_mm_storeu_ps(Res + 12, res3);
	}

	namespace hide
	{
		Matrix<float, 4, 4> operator*(const Matrix<float, 4, 4>& A, const Matrix<float, 4, 4>& B)
		{
			Matrix<float, 4, 4> result;
			Matrix<float, 4, 4> cmp_res;
			#ifdef COL_MAJOR
				micro_4x4((float*)B.data(), (float*)A.data(), (float*)result.data());
			#else
				micro_4x4((float*)A.data(), (float*)B.data(), (float*)result.data());
			#endif
			return result;
		}


	}

	place mat4 translate(const mat4& M, const vec3& v)
	{
		mat4 result(M);
		#ifdef COL_MAJOR
		result[3] = M[0] * v[0] + M[1] * v[1] + M[2] * v[2] + M[3];
		#else
		//TODO
		#endif
		return result;
	}

	place mat4 scale(mat4& mat, const vec3& vec)
	{
		mat4 result;
		#ifdef COL_MAJOR
		std::transform(mat.begin(), mat.begin() + 3, vec.begin(), result.begin(), [] (auto& e1, auto& e2) { return e1 * e2; });
		result[3] = mat[3];
		#else
		//TODO
		#endif
		return result;
	}

	place mat4 rotate(mat4& mat, const vec3& vec, float angle)
	{
		float sin = std::sinf(angle);
		float cos = std::cosf(angle);
		
		vec3 axis = normalize(vec);
		vec3 temp(axis * (1.f - cos));

		mat4 result(1.f);
#ifdef COL_MAJOR
		mat3 rotate;

		rotate[0][0] = cos + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + sin * axis[2];
		rotate[0][2] = temp[0] * axis[2] - sin * axis[1];

		rotate[1][0] = temp[1] * axis[0] - sin * axis[2];
		rotate[1][1] = cos + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + sin * axis[0];

		rotate[2][0] = temp[2] * axis[0] + sin * axis[1];
		rotate[2][1] = temp[2] * axis[1] - sin * axis[0];
		rotate[2][2] = cos + temp[2] * axis[2];

		result[0] = mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2];
		result[1] = mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2];
		result[2] = mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2];
		result[3] = mat[3];
#else
		result[0][0] = cos + vec[0] * vec[0] * _cos;
		result[0][1] = vec[0] * vec[1] * _cos - vec[2] * sin;
		result[0][2] = vec[0] * vec[2] * _cos + vec[2] * sin;

		result[1][0] = vec[0] * vec[1] * _cos + vec[2] * sin;
		result[1][1] = cos + vec[1] * vec[1] * _cos;
		result[1][2] = vec[1] * vec[2] * _cos - vec[0] * sin;

		result[2][0] = vec[1] * vec[2] * _cos - vec[2] * sin;
		result[2][1] = vec[1] * vec[2] * _cos + vec[0] * sin;
		result[2][2] = cos + vec[2] * vec[2] * _cos;
#endif
		return result;
	}

	
	place mat4 perspeсtive(float zNear, float zFar, float aspect, float fov)
	{
		assert(std::abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));

		float const ctgHalfFovy = 1.f / std::tan(fov / 2.f);

		mat4 Result(0.f);
		Result[0][0] = ctgHalfFovy / aspect;
		Result[1][1] = ctgHalfFovy;
		Result[2][2] = - (zFar + zNear) / (zFar - zNear);
		Result[2][3] = - 1.f;
		Result[3][2] = - 2.f * zFar * zNear / (zFar - zNear);
		return Result;
	}

	place mat4 lock_at(const vec3& pos, const vec3& target, const vec3& up)
	{
		vec3 Dir = normalize(pos - target);
		vec3 Right = normalize(up * Dir);
		vec3 Up = Dir * Right;
		mat4 result(1.f);

		for (auto i = 0; i < 3; ++i)
		{
			result[i][0] = Right[i];
			result[i][1] = Up[i];
			result[i][2] = Dir[i];
		}
		result[3][0] = -dot(Right, pos);
		result[3][1] = -dot(Up, pos);
		result[3][2] = -dot(Dir, pos);

		return result;
	}

}