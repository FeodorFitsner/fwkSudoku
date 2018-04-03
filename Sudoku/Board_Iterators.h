﻿//====---- Sudoku/Board_Iterators.h                                   ----====//
//
//	Location aware iterator implementation for Board
//====--------------------------------------------------------------------====//
//
// The iterator can return a Location object.
// included by Board.h
//
//====--------------------------------------------------------------------====//
#pragma once

#include "Location.h"
#include <gsl/gsl>
#include <iterator>
#include <type_traits>

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku
{
template<typename T, int N, bool is_const = false, bool is_reverse = false>
class Board_iterator
{
	using self_type = Board_iterator;
	using owner_type =
		std::conditional_t<is_const, Board<T, N> const, Board<T, N>>;
	using Location = ::Sudoku::Location<N>;

public:
	// member types
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t<is_const, T const*, T*>;
	using reference         = std::conditional_t<is_const, T const&, T&>;
	using iterator_category = std::random_access_iterator_tag;

	using _Unchecked_type = pointer; // MSVC STL implementation specific

	// Constructors
	constexpr Board_iterator() noexcept = default;
	explicit constexpr Board_iterator(gsl::not_null<owner_type*> owner) noexcept
		: board_(owner)
	{
	}
	constexpr Board_iterator(
		gsl::not_null<owner_type*> owner, Location loc) noexcept
		: board_(owner), elem_(loc.element())
	{
	}
	// Assignment
	constexpr Board_iterator& operator=(Location loc) noexcept
	{
		assert(is_valid(loc));
		elem_ = loc.element();
	}

	//====----------------------------------------------------------------====//
	[[nodiscard]] constexpr Location location() const noexcept
	{
		return Location{gsl::narrow_cast<int>(elem_)};
	}
	// Allows for implicit conversion to Location
	explicit constexpr operator Location() const noexcept { return location(); }

	//====----------------------------------------------------------------====//
	[[nodiscard]] reference operator*() const
	{
		assert(board_ != nullptr && dereferenceable_location());
		return board_->operator[](location());
	}
	[[nodiscard]] pointer operator->() const
	{ // member access; equivalent to (*p).member
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	//====----------------------------------------------------------------====//
	self_type& operator++()
	{ // pre-increment
		assert(board_ != nullptr);
		if constexpr (is_reverse)
		{
			assert(elem_ >= 0);
			--elem_;
		}
		else
		{
			assert(elem_ < full_size<N>);
			++elem_;
		}

		return (*this);
	}
	self_type operator++(int)
	{ // post-increment
		const self_type pre{*this};
		operator++();
		return pre;
	}
	self_type& operator--()
	{ // pre-decrement
		assert(board_ != nullptr);
		if constexpr (is_reverse)
		{
			assert(elem_ < full_size<N> - 1);
			++elem_;
		}
		else
		{
			assert(elem_ > 0);
			--elem_;
		}
		return (*this);
	}
	self_type operator--(int)
	{ // post-decrement
		const self_type pre{*this};
		operator--();
		return pre;
	}

	//====----------------------------------------------------------------====//
	self_type& operator+=(const difference_type offset)
	{
		assert(offset == 0 || board_ != nullptr);
		if constexpr (is_reverse)
		{
			elem_ -= offset;
			assert(elem_ >= -1);
			assert(elem_ < full_size<N>);
		}
		else
		{
			elem_ += offset;
			assert(elem_ >= 0);
			assert(elem_ <= full_size<N>);
		}

		return (*this);
	}
	[[nodiscard]] self_type operator+(const difference_type offset) const
	{
		self_type tmp{*this};
		return (tmp += offset);
	}
	self_type& operator-=(const difference_type offset)
	{
		return operator+=(-offset);
	}
	[[nodiscard]] self_type operator-(const difference_type offset) const
	{
		self_type tmp{*this};
		return (tmp += -offset);
	}

	[[nodiscard]] difference_type operator-(const self_type& other) const
	{ // difference
		assert(is_same_address(other));
		if constexpr (is_reverse)
			return other.elem_ - elem_;
		else
			return elem_ - other.elem_;
	}

	[[nodiscard]] reference operator[](const difference_type offset) const
	{
		return (*(*this + offset));
	}

	//====----------------------------------------------------------------====//
	[[nodiscard]] bool operator==(const self_type& other) const
	{
		assert(is_same_address(other));
		return is_same_address(other) && elem_ == other.elem_;
	}
	[[nodiscard]] bool operator!=(const self_type& other) const
	{
		return (!operator==(other));
	}
	[[nodiscard]] bool operator<(const self_type& other) const
	{
		assert(is_same_address(other));
		if constexpr (is_reverse)
			return elem_ > other.elem_;
		else
			return elem_ < other.elem_;
	}
	[[nodiscard]] bool operator>(const self_type& other) const
	{
		return (other < *this);
	}
	[[nodiscard]] bool operator<=(const self_type& other) const
	{
		return (!(other < *this));
	}
	[[nodiscard]] bool operator>=(const self_type& other) const
	{
		return (!(*this < other));
	}

private:
	owner_type* board_{nullptr};
	difference_type elem_{0};

	constexpr bool is_same_address(const self_type& other) const noexcept
	{ // compare address of:
		return board_ == other.board_;
	}

	// Dereferenceable locations
	static constexpr bool
		dereferenceable_location(const difference_type x) noexcept
	{
		return !(x < 0 || x >= full_size<N>);
	}
	constexpr bool dereferenceable_location() const noexcept
	{
		return dereferenceable_location(elem_);
	}
};

template<typename T, int N>
using const_Board_iterator = Board_iterator<T, N, true, false>;

template<typename T, int N>
using reverse_Board_iterator = Board_iterator<T, N, false, true>;

template<typename T, int N>
using const_reverse_Board_iterator = Board_iterator<T, N, true, true>;


template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline Board_iterator<T, N, is_const, is_reverse> operator+(
	typename Board_iterator<T, N, is_const>::difference_type const offset,
	Board_iterator<T, N, is_const, is_reverse> itr)
{
	return (itr += offset);
}

} // namespace Sudoku
