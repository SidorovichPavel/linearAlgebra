#pragma once

#include <assert.h>
#include <initializer_list>
#include <algorithm>

#ifdef __location__
#define place __device__ __host__
#else
#define place
#endif

namespace la
{
	namespace hide
	{
		template<class T, uint8_t Dim>
		class Vector
		{
			static_assert(Dim > 0, "linear Algebra : [Vector] : Invalidete dimention");
		public:
			using value_type = T;
			using reference = T&;
			using creference = const T&;
			using pointer = T*;

			using iterator = pointer;
			using citerator = T const*;

			using next_dim_vector = Vector<T, Dim + 1>;
			using prev_dim_vector = Vector<T, Dim - 1>;

			constexpr static uint8_t count() { return Dim; }

			constexpr Vector() noexcept {
				for (auto it = begin(); it != end(); ++it) {
					*it = static_cast<T>(0);
				}
			}
			constexpr Vector(const T& _Value) noexcept {
				for (auto it = begin(); it != end(); ++it) {
					*it = static_cast<T>(_Value);
				}
			}
			constexpr Vector(const next_dim_vector& _Other) noexcept {
				auto src = _Other.begin();
				auto dest = begin();
				for (; dest != end();) {
					*dest = *src;
					++dest;
					++src;
				}
			}
			constexpr Vector(const prev_dim_vector& _Other, const T& _Value) noexcept {
				auto src = _Other.begin();
				auto dest = begin();
				for (; src != _Other.end();) {
					*dest = *src;
					++dest;
					++src;
				}
				*dest = _Value;
			}
			constexpr Vector(std::initializer_list<T> _Init_List) noexcept {
				size_t size = _Init_List.size();
				assert(size <= static_cast<size_t>(Dim));

				auto src = _Init_List.begin();
				auto dest = begin();
				for (auto i = 0; i < size; ++i) {
					*dest = *src;
					++dest;
					++src;
				}
				for (; dest != end(); ++dest) {
					*dest = static_cast<T>(0);
				}
			}

			Vector(const Vector&) = default;
			Vector& operator=(const Vector&) = default;
			Vector(Vector&&) = default;
			Vector& operator=(Vector&&) = default;

			pointer data() noexcept {
				return mData;
			}

			const pointer data() const noexcept {
				return mData;
			}

			iterator begin() noexcept {
				return static_cast<iterator>(mData);
			}
			iterator end() noexcept {
				return static_cast<iterator>(mData + Dim);
			}
			citerator begin() const noexcept {
				return const_cast<T* const>(mData);
			}
			citerator end() const noexcept {
				return const_cast<T* const>(mData + Dim);
			}

			reference operator[](uint8_t _Index) noexcept {
				assert(_Index < Dim);
				return *(mData + _Index);
			}
			creference operator[](uint8_t _Index) const noexcept {
				assert(_Index < Dim);
				return *(mData + _Index);
			}

			Vector& operator+() const noexcept { return *this; }

			Vector& operator-() noexcept {
				std::transform(begin(), end(), begin(), [] (auto& e) {return-e; });
				return *this;
			}

			Vector operator-() const noexcept {
				Vector result;
				std::transform(begin(), end(), result.begin(), [] (auto& e) {return-e; });
				return result;
			}

			Vector& operator+=(const Vector& _Right) noexcept {
				std::transform(begin(), end(), _Right.begin(), begin(), [] (auto& a, auto& b) {return a + b; });
				return *this;
			}

			Vector& operator-=(const Vector& _Right) noexcept {
				std::transform(begin(), end(), _Right.begin(), begin(), [] (auto& a, auto& b) {return a - b; });
				return *this;
			}

			Vector& operator*=(const T& _Right) noexcept {
				std::transform(begin(), end(), begin(), [&] (auto& a) {return a * _Right; });
				return *this;
			}

			Vector& operator/=(const T& _Right) noexcept {
				std::transform(begin(), end(), begin(), [&] (auto& a) {return a / _Right; });
				return *this;
			}

			float lengthf() const noexcept {
				float result = 0;
				for (auto it = begin(); it != end(); ++it) {
					result += *it * *it;
				}
				return sqrtf(result);
			}

		private:
			T mData[Dim];
		};

		template<class T, size_t _Dim>
		place Vector<T, _Dim> operator+(const Vector<T, _Dim>& a, const Vector<T, _Dim>& b) noexcept
		{
			Vector<T, _Dim> result;
			auto aIt = a.begin();
			auto bIt = b.begin();
			auto rIt = result.begin();
			for (; aIt != a.end(); ++aIt, ++bIt, ++rIt)
				*rIt = *aIt + *bIt;
			return result;
		}

		template<class T, size_t _Dim>
		place Vector<T, _Dim> operator-(const Vector<T, _Dim>& a, const Vector<T, _Dim>& b) noexcept
		{
			Vector<T, _Dim> result;
			auto aIt = a.begin();
			auto bIt = b.begin();
			auto rIt = result.begin();
			for (; aIt != a.end(); ++aIt, ++bIt, ++rIt)
				*rIt = *aIt - *bIt;
			return result;
		}

		template<class T, size_t _Dim>
		place Vector<T, _Dim> operator*(const Vector<T, _Dim>& a, const typename Vector<T, _Dim>::value_type& k) noexcept
		{
			Vector<T, _Dim> result;
			auto aIt = a.begin();
			auto rIt = result.begin();
			for (; aIt != a.end(); ++aIt, ++rIt)
				*rIt = *aIt * k;
			return result;
		}

		template<class T, size_t _Dim>
		place Vector<T, _Dim> operator/(const Vector<T, _Dim>& a, const typename Vector<T, _Dim>::value_type& k) noexcept
		{
			Vector<T, _Dim> result;
			auto aIt = a.begin();
			auto rIt = result.begin();
			for (; aIt != a.end(); ++aIt, ++rIt)
				*rIt = *aIt / k;
			return result;
		}

		template<class T, size_t _Dim>
		place Vector<T, _Dim> operator*(const typename Vector<T, _Dim>::value_type& k, const Vector<T, _Dim>& a) noexcept
		{
			Vector<T, _Dim> result;
			auto aIt = a.begin();
			auto rIt = result.begin();
			for (; aIt != a.end(); ++aIt, ++rIt)
				*rIt = *aIt * k;
			return result;
		}

		template<class T, size_t _Dim>
		place Vector<T, _Dim> operator/(const typename Vector<T, _Dim>::value_type& k, const Vector<T, _Dim>& a) noexcept
		{
			Vector<T, _Dim> result;
			auto aIt = a.begin();
			auto rIt = result.begin();
			for (; aIt != a.end(); ++aIt, ++rIt)
				*rIt = *aIt / k;
			return result;
		}

		template<class T>
		place Vector<T, 3> operator*(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
		{
			return Vector<T, 3>{ v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] };
		}
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