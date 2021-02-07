#pragma once

#include <initializer_list>
#include <src/MathObjects/Vector.h>
#include <algorithm>

namespace la
{
	namespace hide
	{
		template <class T, size_t M, size_t N>
		class Matrix
		{
			static_assert(M != 0 || N != 0, "dimension invalidate");
		public:
			using value_type = Vector<T, N>;
			using reference = value_type&;
			using pointer = value_type*;

			using col_type = Vector<T, N>;
			using iterator = col_type*;
			using const_iterator = col_type;

			using prev_dim_matrix = typename Matrix<T, M - 1, N - 1>;

			constexpr static size_t count() { return M; }

			constexpr Matrix() noexcept
			{}

			template<class _Ty>
			constexpr Matrix(_Ty _Value = static_cast<_Ty>(1)) noexcept
			{
				if constexpr (M == N)
					for (auto i = 0; i < M; ++i)_Elems[i][i] = static_cast<T>(_Value);
			}

			constexpr Matrix(const Matrix& _Other) noexcept
			{
				std::copy(_Other.begin(), _Other.end(), begin());
			}

			constexpr Matrix(Matrix&& _Other) noexcept
			{
				std::copy(_Other.begin(), _Other.end(), begin());
			}

			constexpr Matrix(const prev_dim_matrix& _Mat) noexcept
			{
				for (auto i = 0; i < prev_dim_matrix::count(); ++i) _Elems[i] = col_type(_Mat[i], static_cast<T>(0));
				_Elems[M - 1][N - 1] = static_cast<T>(1);
			}

			constexpr Matrix(const std::initializer_list<col_type>& _Init_List) noexcept
			{
				auto size = std::distance(_Init_List.begin(), _Init_List.end());
				assert(size <= N * M);
				for (auto i = 0; i < size; ++i) _Elems[i] = *(_Init_List.begin() + i);
			}

			constexpr iterator begin() const noexcept
			{
				return iterator(_Elems);
			}

			constexpr iterator end() const noexcept
			{
				return iterator(_Elems + M);
			}

			constexpr const_iterator cbegin() const noexcept
			{
				return const_iterator(_Elems);
			}

			constexpr const_iterator cend() const noexcept
			{
				return const_iterator(_Elems + M);
			}

			reference operator[](unsigned _Index) noexcept
			{
				assert(_Index <= M);
				return _Elems[_Index];
			}
			
			value_type operator[](unsigned _Index) const noexcept
			{
				assert(_Index <= M);
				return _Elems[_Index];
			}

			pointer data() noexcept
			{
				return _Elems;
			}

			const pointer data() const noexcept
			{
				return (const pointer)_Elems;
			}

			Matrix& operator=(const Matrix& _Right)
			{
				std::copy(_Right.begin(), _Right.end(), begin());
				return *this;
			}

			Matrix& operator+() { return*this; }

			Matrix& operator-()
			{
				std::transform(begin(), end(), begin(), [] (auto& e) {return -e; });
				return *this;
			}

		private:
			col_type _Elems[M];
		};

		template<class T, size_t M, size_t N>
		Matrix<T, M, N> operator+(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) noexcept
		{
			Matrix<T, M, N> result;
			std::transform(m1.begin(), m1.end(), m2.begin(), result.begin(), [] (auto& e1, auto& e2) { return e1 + e2; });
			return result;
		}
		
		template<class T, size_t M, size_t N>
		Matrix<T, M, N> operator-(const Matrix<T, M, N>& m1, const Matrix<T, M, N>& m2) noexcept
		{
			Matrix<T, M, N> result;
			std::transform(m1.begin(), m1.end(), m2.begin(), result.begin(), [] (auto& e1, auto& e2) { return e1 - e2; });
			return result;
		}

		template<class T, size_t M, size_t N>
		Matrix<T, M, N> operator*(const Matrix<T, M, N>& m1, const T& k) noexcept
		{
			Matrix<T, M, N> result;
			std::transform(m1.begin(), m1.end(), result.begin(), [&] (auto& e1) { return e1 * k; });
			return result;
		}
		template<class T, size_t M, size_t N>
		Matrix<T, M, N> operator/(const Matrix<T, M, N>& m1, const T& k) noexcept
		{
			Matrix<T, M, N> result;
			std::transform(m1.begin(), m1.end(), result.begin(), [&] (auto& e1) { return e1 / k; });
			return result;
		}
		template<class T, size_t M, size_t N>
		Matrix<T, M, N> operator*(const T& k, const Matrix<T, M, N>& m1) noexcept
		{
			Matrix<T, M, N> result;
			std::transform(m1.begin(), m1.end(), result.begin(), [&] (auto& e1) { return e1 * k; });
			return result;
		}
		template<class T, size_t M, size_t N>
		Matrix<T, M, N> operator/(const T& k, const Matrix<T, M, N>& m1) noexcept
		{
			Matrix<T, M, N> result;
			std::transform(m1.begin(), m1.end(), result.begin(), [&] (auto& e1) { return e1 / k; });
			return result;
		}

		template<class T, size_t M, size_t K, size_t N>
		Matrix<T, M, N> operator*(const Matrix<T, M, K>& A, const Matrix<T, K, N>& B)
		{
			return Matrix<T, M, N>;
		}

		Matrix<float, 4, 4> operator*(const Matrix<float, 4, 4>& A, const Matrix<float, 4, 4>& B);

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