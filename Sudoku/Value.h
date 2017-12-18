﻿//===--- Sudoku/Value.h													---===//
//
//	Value type
//===----------------------------------------------------------------------===//
// Strongly typed
// Unsigned to allow use as a bitmask
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Size.h"

#include <vector>
#include <algorithm>
#include <exception>


namespace Sudoku
{
class Value
{
	using unsigned_int = unsigned int;
	using u64_int      = unsigned long long int;

public:
	Value() noexcept {}
	explicit constexpr Value(unsigned int val) noexcept : value_(val) {}

	explicit constexpr operator unsigned_int() const noexcept
	{
		return value_;
	}
	explicit constexpr operator u64_int() const noexcept
	{
		return static_cast<unsigned long long int>(value_);
	}
	constexpr operator bool() const noexcept { return value_ != 0; }

	constexpr bool operator==(const Value&) const noexcept;
	constexpr bool operator<(const Value&) const noexcept;

private:
	unsigned int value_{0};
};

// using value_t = unsigned int;
using value_t = Value;
// allows use as a bitmask

//===---------------------------------------------------------------------===//
constexpr bool operator!=(const Value&, const Value&) noexcept;
constexpr bool operator>(const Value&, const Value&) noexcept;
constexpr bool operator<=(const Value&, const Value&) noexcept;
constexpr bool operator>=(const Value&, const Value&) noexcept;

auto to_Value(int val);
//===---------------------------------------------------------------------===//
template<int N>
constexpr bool is_valid(const Value&);
template<int N>
constexpr bool is_valid(const std::vector<Value>&);

//===---------------------------------------------------------------------===//

// Test input value
template<int N>
inline constexpr bool is_valid(const Value& value)
{
	return value > Value{0} && value <= static_cast<Value>(elem_size<N>);
}

// Test input values
template<int N>
constexpr bool is_valid(const std::vector<Value>& values)
{
	return (
		!values.empty() &&
		std::all_of(values.cbegin(), values.cend(), [](Value i) {
			return is_valid<N>(i);
		}));
}

//===---------------------------------------------------------------------===//
inline auto to_Value(int val)
{
	if (val >= 0)
	{
		return static_cast<Value>(unsigned(val));
	}
	else
		throw std::domain_error("negative Value");
};

//===---------------------------------------------------------------------===//
inline constexpr bool Value::operator==(const Value& right) const noexcept
{
	return value_ == right.value_;
}
inline constexpr bool operator!=(const Value& left, const Value& right) noexcept
{
	return not(left == right);
}
inline constexpr bool Value::operator<(const Value& right) const noexcept
{
	return value_ < right.value_;
}
inline constexpr bool operator<=(const Value& left, const Value& right) noexcept
{
	return not(right < left);
}
inline constexpr bool operator>(const Value& left, const Value& right) noexcept
{
	return right < left;
}
inline constexpr bool operator>=(const Value& left, const Value& right) noexcept
{
	return not(left < right);
}
static_assert(Value{7} == Value{7});
static_assert(Value{1} != Value{0});
static_assert(Value{8} > Value{2});
static_assert(Value{4} >= Value{1});
static_assert(Value{3} <= Value{9});
static_assert(Value{5} < Value{6});

} // namespace Sudoku