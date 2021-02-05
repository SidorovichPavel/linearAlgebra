#pragma once

#include <array>
#include <src/MathObjects/array_wrapper.h>
#include <functional>
#include <algorithm>

namespace la
{
	namespace hide
	{
		template <class T, size_t M, size_t N>
		class Matrix : public std::array<T, M * N>
		{
			static_assert(
				std::is_same<T, int>::value ||
				std::is_same<T, float>::value ||
				std::is_same<T, unsigned>::value,
				"this vector is only for linear math");

			static_assert(M != 0 || N != 0, "dimension invalidate");

			using myBase = std::array<T, M * N>;
			using iterator = typename myBase::iterator;

		public:
			constexpr Matrix(const T& _Value = 0) noexcept : myBase()
			{
				std::fill(myBase::begin(), myBase::end(), static_cast<T>(0));
				if (N == M)
					for (auto i = 0; i < M; ++i)myBase::_Elems[i * N + i] = _Value;
			}
			constexpr Matrix(const T(&_Array)[M * N]) noexcept : myBase()
			{
				std::copy(_Array, _Array + M * N, myBase::begin());
			}
			constexpr Matrix(const std::initializer_list<T> _Init_List) noexcept : myBase()
			{
				assert(_Init_List.size() <= N * M);
				std::copy(_Init_List.begin(), _Init_List.end(), myBase::begin());
			}
			std::array_wrapper<T, N> operator[](size_t _Index) noexcept
			{
				assert(_Index < M);
				return std::array_wrapper<T, N>(myBase::_Elems + _Index * N);
			}
			const std::array_wrapper<T, N> operator[](size_t _Index) const
			{
				assert(_Index < M);
				return std::array_wrapper<T, N>(myBase::_Elems + _Index * N);
			}

		};
	}

	using mat4 = hide::Matrix<float, 4, 4>;
	using mat4u = hide::Matrix<unsigned, 4, 4>;
	using mat4i = hide::Matrix<int, 4, 4>;

	using mat3 = hide::Matrix<float, 3, 3>;
	using mat3u = hide::Matrix<unsigned, 3, 3>;
	using mat3i = hide::Matrix<int, 3, 3>;

	using mat2 = hide::Matrix<float, 2, 2>;
	using mat2u = hide::Matrix<unsigned, 2, 2>;
	using mat2i = hide::Matrix<int, 2, 2>;
}