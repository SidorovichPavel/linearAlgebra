#include <src/linearAlgebra.h>

#include <immintrin.h>

namespace la
{
	template<class T, size_t _Size, size_t __Aligned>
	struct buf
	{
		T* _Ptr;
		buf() : _Ptr((T*)_mm_malloc(_Size * sizeof(T), __Aligned)) {};
		~buf() { _mm_free(_Ptr); }
	};

	float dot(const vec3& v1, const vec3& v2)
	{
		float res = 0;
		for (auto _Ptr1 = v1.begin(), _Ptr2 = v2.begin(); _Ptr1 != v1.end(); ++_Ptr1, ++_Ptr2)
			res += *_Ptr1 * *_Ptr2;
		return res;
	}
	float dot2(const vec3& v)
	{
		return dot(v, v);
	}
	float length(const vec3& v)
	{
		return std::sqrtf(dot2(v));
	}
	vec3 operator*(const vec3& v1, const vec3& v2)
	{
		return vec3{ v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] };
	}
	void micro_4x4(float* A, float* B, float* Res)
	{
		__m128 res0 = _mm_setzero_ps();
		__m128 res1 = _mm_setzero_ps();
		__m128 res2 = _mm_setzero_ps();
		__m128 res3 = _mm_setzero_ps();
		
		__m128 a0, b0;

		for (auto i = 0; i < 4; ++i)
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

			++A;
		}
		_mm_storeu_ps(Res + 0, res0);
		_mm_storeu_ps(Res + 4, res1);
		_mm_storeu_ps(Res + 8, res2);
		_mm_storeu_ps(Res + 12, res3);
	}
	mat4 operator*(mat4& A, mat4& B)
	{
		mat4 res;
		micro_4x4(A.data(), B.data(), res.data());
		return res;
	}
	vec4 operator*(mat4& A, vec4& vec)
	{
		vec4 res;
		__m128 result = _mm_setzero_ps();

		buf<float, 16, 16> matT;
		auto ptr = matT._Ptr;
		for (auto i = 0; i < 4; ++i)
			for (auto j = 0; j < 4; ++j, ++ptr)
				*ptr = A[j][i];

		ptr = matT._Ptr;
		auto v_ptr = vec.data();
		__m128 v, a;
		for (auto i = 0; i < 4; ++i, ptr += 4, ++v_ptr)
		{
			v = _mm_set_ps1(*v_ptr);
			a = _mm_load_ps(ptr);
			result = _mm_fmadd_ps(v, a, result);
		}

		_mm_storeu_ps(res.data(), result);
		return res;
	}


}