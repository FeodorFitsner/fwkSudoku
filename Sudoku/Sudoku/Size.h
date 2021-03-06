//====---- Sudoku/Size.h                                              ----====//
//
// The size definition that rules them all.
//====--------------------------------------------------------------------====//
#pragma once

#include <gsl/gsl>

#include <limits>

namespace Sudoku
{
template<int N>
struct Size
{
	using index                 = gsl::index;
	static constexpr index base = N;           // 3 for default 9*9 board
	static constexpr index elem = base * base; // 9 for default
	static constexpr index full = elem * elem; // 81 for default

	static_assert(
		1 < base && base < elem && base < full && elem < full,
		"struct Size: Board size out of bounds.");
	static_assert(
		N < std::numeric_limits<index>::max() / base &&
			N < std::numeric_limits<index>::max() / elem &&
			N < std::numeric_limits<index>::max() / elem / base &&
			N < std::numeric_limits<index>::max() / full &&
			N < std::numeric_limits<index>::max() / full / base,
		"struct Size: Board size too large for system integer size.");
	// fails at N > 35 when int less than 32-bits.
};

template<int N>
inline constexpr gsl::index base_size = Size<N>::base; // default 3
template<int N>
inline constexpr gsl::index elem_size = Size<N>::elem; // default 9
template<int N>
inline constexpr gsl::index full_size = Size<N>::full; // default 81

} // namespace Sudoku
