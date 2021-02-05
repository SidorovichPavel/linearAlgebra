#pragma once

#include <assert.h>
#include <array>
#include <src/MathObjects/array_wrapper.h>
#include <initializer_list>

namespace la
{
	namespace hide
	{
		template<class T, size_t N>
		class Vector : public std::array<T, N>
		{
			static_assert(
				std::is_same<T, int>::value ||
				std::is_same<T, float>::value ||
				std::is_same<T, unsigned>::value,
				"this vector is only for linear math");
			static_assert(N > 0, "dimention invalidate");

			using myBase = std::array<T, N>;
			using iterator = typename myBase::iterator;

		public:
			constexpr Vector(const T& _Vaule = 0) noexcept : myBase()
			{
				std::fill(myBase::begin(), myBase::end(), _Vaule);
			};
			constexpr Vector(const T(&_Array)[N]) noexcept : myBase()
			{
				std::copy(_Array, _Array + N, myBase::begin());
			}
			constexpr Vector(const std::initializer_list<T> _Init_List) noexcept : myBase()
			{
				assert(_Init_List.size() <= N);
				std::copy(_Init_List.begin(), _Init_List.end(), myBase::begin());
			}
			~Vector() {};
			constexpr static size_t count() noexcept
			{
				return N;
			}
		};
	}

	using vec2 = hide::Vector<float, 2>;
	using vec2i = hide::Vector<int, 2>;
	using vec2u = hide::Vector<unsigned, 2>;

	using vec3 = hide::Vector<float, 3>;
	using vec3i = hide::Vector<int, 3>;
	using vec3u = hide::Vector<unsigned, 3>;

	using vec4 = hide::Vector<float, 4>;
	using vec4u = hide::Vector<unsigned, 4>;
	using vec4i = hide::Vector<int, 4>;
}