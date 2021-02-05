#pragma once

#include <array>
#include <stdexcept>

_STD_BEGIN

template<class _Ty, size_t _Size>
class array_wrapper
{
public:
    using value_type = _Ty;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = _Ty*;
    using const_pointer = const _Ty*;
    using reference = _Ty&;
    using const_reference = const _Ty&;

    using iterator = _Array_iterator<_Ty, _Size>;
    using const_iterator = _Array_const_iterator<_Ty, _Size>;

    using reverse_iterator = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

#if _HAS_TR1_NAMESPACE
    _DEPRECATE_TR1_NAMESPACE void assign(const _Ty& _Value) {
        _STD fill_n(_Elems, _Size, _Value);
    }
#endif // _HAS_TR1_NAMESPACE

    _CONSTEXPR17 array_wrapper() = delete;

    _CONSTEXPR17 array_wrapper(const std::array<_Ty, _Size>& _Right) noexcept {
        static_assert(_Right._Elems, "empty pointer: 100% UB");
        _Elems = _Right._Elems;
    }

    _CONSTEXPR17 array_wrapper(const array_wrapper<_Ty, _Size>& _Right) noexcept {
        static_assert(_Right._Elems, "empty pointer: 100% UB");
        _Elems = _Right._Elems;
    }

    _CONSTEXPR20 array_wrapper(_Ty* _Ptr) {
        if (_Ptr)
            _Elems = _Ptr;
        else
            throw std::runtime_error("null pointer");
    }

    _CONSTEXPR20 array_wrapper(const _Ty* _Ptr) {
        if (_Ptr)
            _Elems = (_Ty*)_Ptr;
        else
            throw std::runtime_error("null pointer");
    }

    _CONSTEXPR20 void fill(const _Ty& _Value) {
        _STD fill_n(_Elems, _Size, _Value);
    }

    _CONSTEXPR20 void swap(array_wrapper& _Other) noexcept(_Is_nothrow_swappable<_Ty>::value) {
        std::swap(_Elems, _Other._Elems);
    }

    _NODISCARD _CONSTEXPR17 iterator begin() noexcept {
        return iterator(_Elems, 0);
    }

    _NODISCARD _CONSTEXPR17 const_iterator begin() const noexcept {
        return const_iterator(_Elems, 0);
    }

    _NODISCARD _CONSTEXPR17 iterator end() noexcept {
        return iterator(_Elems, _Size);
    }

    _NODISCARD _CONSTEXPR17 const_iterator end() const noexcept {
        return const_iterator(_Elems, _Size);
    }

    _NODISCARD _CONSTEXPR17 reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR17 const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR17 reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR17 const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR17 const_iterator cbegin() const noexcept {
        return begin();
    }

    _NODISCARD _CONSTEXPR17 const_iterator cend() const noexcept {
        return end();
    }

    _NODISCARD _CONSTEXPR17 const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    _NODISCARD _CONSTEXPR17 const_reverse_iterator crend() const noexcept {
        return rend();
    }

    _CONSTEXPR17 _Ty* _Unchecked_begin() noexcept {
        return _Elems;
    }

    _CONSTEXPR17 const _Ty* _Unchecked_begin() const noexcept {
        return _Elems;
    }

    _CONSTEXPR17 _Ty* _Unchecked_end() noexcept {
        return _Elems + _Size;
    }

    _CONSTEXPR17 const _Ty* _Unchecked_end() const noexcept {
        return _Elems + _Size;
    }

    _NODISCARD constexpr size_type size() const noexcept {
        return _Size;
    }

    _NODISCARD constexpr size_type max_size() const noexcept {
        return _Size;
    }

    _NODISCARD constexpr bool empty() const noexcept {
        return false;
    }

    _NODISCARD _CONSTEXPR17 reference at(size_type _Pos) {
        if (_Size <= _Pos) {
            _Xran();
        }

        return _Elems[_Pos];
    }

    _NODISCARD constexpr const_reference at(size_type _Pos) const {
        if (_Size <= _Pos) {
            _Xran();
        }

        return _Elems[_Pos];
    }

    _NODISCARD _CONSTEXPR17 reference operator[](_In_range_(0, _Size - 1) size_type _Pos) noexcept /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Pos < _Size, "array subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        return _Elems[_Pos];
    }

    _NODISCARD constexpr const_reference operator[](_In_range_(0, _Size - 1) size_type _Pos) const noexcept
        /* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Pos < _Size, "array subscript out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        return _Elems[_Pos];
    }

    _NODISCARD _CONSTEXPR17 reference front() noexcept /* strengthened */ {
        return _Elems[0];
    }

    _NODISCARD constexpr const_reference front() const noexcept /* strengthened */ {
        return _Elems[0];
    }

    _NODISCARD _CONSTEXPR17 reference back() noexcept /* strengthened */ {
        return _Elems[_Size - 1];
    }

    _NODISCARD constexpr const_reference back() const noexcept /* strengthened */ {
        return _Elems[_Size - 1];
    }

    _NODISCARD _CONSTEXPR17 _Ty* data() noexcept {
        return _Elems;
    }

    _NODISCARD _CONSTEXPR17 const _Ty* data() const noexcept {
        return _Elems;
    }

    [[noreturn]] void _Xran() const {
        _Xout_of_range("invalid array<T, N> subscript");
    }

    _Ty* _Elems;
};

_STD_END