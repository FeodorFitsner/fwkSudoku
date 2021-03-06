
#pragma once

#include <Sudoku/Board.h>
#include <Sudoku/Location_Utilities.h>
#include <Sudoku/Options.h>
#include <Sudoku/Solver.h>
#include <gsl/gsl>

#include <string>

#include <iomanip> // setw(), setfill()
#include <sstream>
#include <utility>


namespace Sudoku
{
// Solver-function declarations:
template<int N>
void test_solver_unique(Board<Options<elem_size<N>>, N>&);
template<int N>
void test_solver_exclusive(Board<Options<elem_size<N>>, N>&);
template<int N>
Board<Value, N> getResult(Board<Options<elem_size<N>>, N> const&) noexcept;


//====--------------------------------------------------------------------====//
class Console
{
	struct delimiter
	{
		char space;
		char empty;
		std::string row_block;
		std::string col_block;
		std::string newl;
		std::string block_cross;
	};

	const delimiter space{' ', ' ', "", "", "", ""};
	const delimiter space2{' ', ' ', "", "", "\n", ""};
	const delimiter display{' ', ' ', "-", "|", "\n", "o"};
	// const delimiter csv;
	// const delimiter xml;

public:
	Console() noexcept;
	explicit Console(delimiter);
	//~Console() = default;

	template<int N>
	std::stringstream print_row(const Board<int, N>&, gsl::index row_id) const;
	template<int N, int E>
	std::stringstream
		print_row(const Board<Options<E>, N>&, gsl::index row_id) const;
	template<int N>
	std::stringstream print_board(const Board<int, N>&) const;
	template<int N, int E>
	std::stringstream print_board(const Board<Options<E>, N>&) const;

private:
	const delimiter d;
	// bool Format::find_option(const Board<std::set<int>>&,Location,int value);
	// format elem
	// format col-block section
	// format row-block = separator line
};

//====--------------------------------------------------------------------====//
// Member-functions:

inline Console::Console() noexcept : d(display)
{
	// empty constructor
}

inline Console::Console(delimiter del) : d(std::move(del))
{
	// empty constructor
}

namespace impl
{
	[[nodiscard]] constexpr int pow(int const base, int exp)
	{
		if (exp == 0)
		{
			return 1;
		}
		return base * pow(base, --exp);
	}

	[[nodiscard]] constexpr int charsize(int value, int length)
	{
		constexpr int decimal{10};
		if (value < pow(decimal, length))
		{
			return length;
		}
		++length;
		return charsize(value, length);
	}

	[[nodiscard]] constexpr int charsize(int value)
	{
		constexpr int decimal{10};
		if (value < decimal)
		{
			return 1;
		}
		return charsize(value, 2);
	}
} // namespace impl

template<int N>
std::stringstream
	Console::print_row(const Board<int, N>& input, gsl::index row_id) const
{
	std::stringstream stream;
	constexpr int chars = impl::charsize(elem_size<N>) + 1;

	stream << d.col_block << std::setfill(d.space);
	for (gsl::index i = 0; i < elem_size<N>; ++i)
	{
		if (input[row_id][i] == 0) // no value
		{
			stream << std::setw(chars) << d.space;
		}
		else
		{
			stream << std::setw(chars) << input[row_id][i];
		}
		if ((i + 1) % base_size<N> == 0)
		{
			stream << std::setw(2) << d.col_block;
		}
	}
	return stream;
}

template<int N>
std::stringstream Console::print_board(const Board<int, N>& input) const
{
	std::stringstream stream;
	std::stringstream temp;
	constexpr int chars = impl::charsize(elem_size<N>) + 1;

	// opening bar
	temp << d.block_cross;
	for (gsl::index j = 0; j < base_size<N>; ++j)
	{
		temp << std::setfill(d.row_block[0])
			 << std::setw(chars * base_size<N> + 2) << d.block_cross;
	}
	std::string bar;
	temp >> bar;
	stream << bar << '\n';

	// loop rows
	for (gsl::index i = 0; i < elem_size<N>; ++i)
	{
		stream << print_row(input, i).str() << d.newl;
		if ((i + 1) % base_size<N> == 0)
		{
			stream << bar << '\n';
		}
	}
	return stream;
}

template<int N, int E>
std::stringstream Console::print_board(const Board<Options<E>, N>& input) const
{
	static_assert(E == N * N);
	constexpr gsl::index block_size = elem_size<N> + base_size<N> + 2;
	constexpr gsl::index row_length = base_size<N> * block_size;
	/*
	9   9   9
	o-----------------------------------------o
	| 123 123 123 | 123 123 123 | 123 123 123 |
	| 456 456 456 | 456 456 456 | 456 456 456 | 9
	| 789 789
	|
	| 123 123
	| ...
	o-----------------------------------------o
	|
	*/
	std::stringstream stream;
	std::stringstream n0; // horizontal block separator
	std::stringstream n4; // vertical element separator
	n0 << std::setfill(d.row_block[0]) << d.block_cross << std::setw(row_length)
	   << d.block_cross << d.newl;
	n4 << std::setfill(' ') << std::setw(block_size) << d.col_block;

	stream << std::setfill(d.empty) << std::setw(base_size<N>);

	stream << '\n' << n0.str();

	for (gsl::index row{0}; row < elem_size<N>; ++row)
	{
		stream << print_row(input, row).str();

		if ((row + 1) % base_size<N> == 0)
		{
			stream << n0.str();
		}
		else
		{
			stream << '|';
			for (gsl::index index{}; index < base_size<N>; ++index)
			{
				stream << n4.str();
			}
			stream << '\n';
		}
	}
	return stream;
}

template<int N, int E>
std::stringstream Console::print_row(
	const Board<Options<E>, N>& input, gsl::index row_id) const
{
	std::stringstream stream;

	gsl::index X{1};
	for (gsl::index k{0}; k < base_size<N>; ++k)
	{
		stream << d.col_block << d.space;
		for (gsl::index col{0}; col < elem_size<N>; ++col)
		{
			for (gsl::index i{X}; i < X + base_size<N>; ++i)
			{
				if (input[row_id][col].test(to_Value<N>(i)))
				{
					stream << i;
				}
				else
				{
					stream << d.empty;
				}
			}
			if ((col + 1) % base_size<N> == 0)
			{
				stream << std::setfill(d.space) << std::setw(2) << d.col_block
					   << d.space;
			}
			else
			{
				stream << d.space;
			}
		}
		stream << d.newl;
		X += base_size<N>;
	}
	return stream;
}

//====--------------------------------------------------------------------====//
// Solver function applications:
template<int N> // NOLINTNEXTLINE(runtime/references)
void test_solver_unique(Board<Options<elem_size<N>>, N>& board)
{
	int found{1};
	while (found > 0)
	{
		found = 0;
		for (gsl::index i = 0; i < elem_size<N>; ++i)
		{
			found += unique_in_section(board, board.row(i));
		}
		for (gsl::index i = 0; i < elem_size<N>; ++i)
		{
			found += unique_in_section(board, board.col(i));
		}
		for (gsl::index i = 0; i < elem_size<N>; ++i)
		{
			found += unique_in_section(board, board.block(i));
		}
	}
}

template<int N> // NOLINTNEXTLINE(runtime/references)
void test_solver_exclusive(Board<Options<elem_size<N>>, N>& board)
{
	int found{1};
	while (found > 0)
	{
		found = 0;
		for (gsl::index i = 0; i < elem_size<N>; ++i)
		{
			found += section_exclusive(board, board.row(i));
		}
		for (gsl::index i = 0; i < elem_size<N>; ++i)
		{
			found += section_exclusive(board, board.col(i));
		}
		for (gsl::index i = 0; i < elem_size<N>; ++i)
		{
			found += section_exclusive(board, board.block(i));
		}
	}
}

template<int N>
Board<Value, N>
	getResult(const Board<Options<elem_size<N>>, N>& options) noexcept
{
	Board<Value, N> result{};
	for (gsl::index i = 0; i < full_size<N>; ++i)
	{
		if (options[Location<N>(i)].is_answer())
		{
			result[Location<N>(i)] = get_answer(options[Location<N>(i)]);
		}
	}
	return result;
}


} // namespace Sudoku
