#pragma once

#include <assert.h>
#include <initializer_list>
#include <functional>
#include <algorithm>

#include <src/linearAlgebra.h> //main

namespace la
{
	namespace hide
	{
		template<class T, size_t _Dim>
		class Vector
		{
			static_assert(_Dim > 0, "invalidate dimention!");
		public:
			friend Vector<T, _Dim - 1>;
			using value_type = typename T;
			using reference = T&;
			using pointer = T*;

			using iterator = T*;
			using const_iterator = const T*;

			using prev_dim_vector = typename Vector<T, _Dim - 1>;

			constexpr static size_t count() { return _Dim; }

			//constructos

			constexpr Vector() noexcept
			{
				for (auto i = 0; i < _Dim; i++) _Elems[i] = static_cast<T>(0);
			}
			template<class _Ty>
			constexpr Vector(_Ty _Value) noexcept
			{
				for (auto i = 0; i < _Dim; i++) _Elems[i] = static_cast<T>(_Value);
			}
			constexpr Vector(const Vector& _Other) noexcept
			{
				for (auto i = 0; i < _Dim; ++i)_Elems[i] = _Other._Elems[i];
			}
			template<class _Ty>
			constexpr Vector(const prev_dim_vector& _Vec, const _Ty _Value)
			{
				for (auto i = 0; i < prev_dim_vector::count(); ++i) _Elems[i] = _Vec[i];
				_Elems[_Dim - 1] = static_cast<T>(_Value);
			}
			Vector(const Vector&& _Other) noexcept
			{
				for (auto i = 0; i < _Dim; ++i)_Elems[i] = _Other._Elems[i];
			}
			constexpr Vector(const std::initializer_list<T>& _Init_List) noexcept
			{
				auto size = std::distance(_Init_List.begin(), _Init_List.end());
				assert(size <= _Dim);
				for (auto i = 0; i < size; i++)_Elems[i] = *(_Init_List.begin() + i);
			}
			template<class... Args>
			Vector(const Args... args) noexcept
			{
				_init_args(begin(), args...);
			}
			
			//constructors end
			//methods!!!

			float lengthf() const noexcept
			{
				float sum_sqrs = 0.f;
				for (auto it = begin(); it != end(); ++it) sum_sqrs += *it * *it;
				return std::sqrt(sum_sqrs);
			}
			
			double length() const noexcept
			{
				double sum_sqrs = 0.;
				for (auto it = begin(); it != end(); ++it)sum_sqrs += *it * *it;
				return std::sqrt(sum_sqrs);
			}

			constexpr iterator begin() const noexcept
			{
				return Vector<T, _Dim>::iterator(_Elems);
			}
			constexpr iterator end() const noexcept
			{
				return iterator(_Elems + _Dim);
			}
			constexpr const_iterator cbegin() const noexcept
			{
				return const_iterator(_Elems);
			}
			constexpr const_iterator cend() const noexcept
			{
				return const_iterator(_Elems + _Dim);
			}
			reference operator[](size_t _Index) noexcept
			{
				assert(_Index < _Dim);
				return _Elems[_Index];
			}
			value_type operator[](size_t _Index) const noexcept
			{
				assert(_Index < _Dim);
				return _Elems[_Index];
			}
			pointer data() noexcept
			{
				return _Elems;
			}
			const pointer data() const noexcept
			{
				return _Elems;
			}


			Vector& operator=(const Vector& _Right) noexcept
			{
				std::copy(_Right.begin(), _Right.end(), begin());
				return *this;
			}

			Vector& operator+() noexcept { return *this; }

			Vector& operator-()
			{
				std::transform(begin(), end(), begin(), [] (auto& elem) noexcept {
					return -elem;
					});
				return *this;
			}

		private:
			template<class Arg, class... Args>
			void _init_args(iterator _Iter, Arg _Value, Args... args)
			{
				assert(_Iter != end());
				*_Iter = static_cast<T>(_Value);
				_init_args(_Iter + 1, args...);
			}
			template<class Arg>
			void _init_args(iterator _Iter, Arg _Value)
			{
				assert(_Iter != end());
				*_Iter = static_cast<T>(_Value);
			}
			/*template<>
			void _init_args<prev_dim_vector, value_type>(iterator _Iter, prev_dim_vector _Vec, value_type _Value)
			{

			}*/
			T _Elems[_Dim];
		};

		template<class T, size_t _Dim>
		Vector<T, _Dim> operator+(const Vector<T, _Dim>& a, const Vector<T, _Dim>& b) noexcept
		{
			Vector<T, _Dim> result;
			std::transform(a.begin(), a.end(), b.begin(), result.begin(), [] (auto& e1, auto& e2) { return e1 + e2; });
			return result;
		}

		template<class T, size_t _Dim>
		Vector<T, _Dim> operator-(const Vector<T, _Dim>& a, const Vector<T, _Dim>& b) noexcept
		{
			Vector<T, _Dim> result;
			std::transform(a.begin(), a.end(), b.begin(), result.begin(), [] (auto& e1, auto& e2) { return e1 - e2; });
			return result;
		}

		template<class T, size_t _Dim>
		Vector<T, _Dim> operator*(const Vector<T, _Dim>& a, const typename Vector<T, _Dim>::value_type& k) noexcept
		{
			Vector<T, _Dim> result;
			std::transform(a.begin(), a.end(), result.begin(), [&] (auto& e) { return e * k; });
			return result;
		}

		template<class T, size_t _Dim>
		Vector<T, _Dim> operator/(const Vector<T, _Dim>& a, const typename Vector<T, _Dim>::value_type& k) noexcept
		{
			Vector<T, _Dim> result;
			std::transform(a.begin(), a.end(), result.begin(), [&] (auto& e) { return e / k; });
			return result;
		}
		
		template<class T, size_t _Dim>
		Vector<T, _Dim> operator*(const typename Vector<T, _Dim>::value_type& k, const Vector<T, _Dim>& a) noexcept
		{
			Vector<T, _Dim> result;
			std::transform(a.begin(), a.end(), result.begin(), [&] (auto& e) { return e * k; });
			return result;
		}

		template<class T, size_t _Dim>
		Vector<T, _Dim> operator/(const typename Vector<T, _Dim>::value_type& k, const Vector<T, _Dim>& a) noexcept
		{
			Vector<T, _Dim> result;
			std::transform(a.begin(), a.end(), result.begin(), [&] (auto& e) { return e / k; });
			return result;
		}

		template<class T>
		Vector<T, 3> operator*(const Vector<T, 3>& v1, const Vector<T, 3>& v2)
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