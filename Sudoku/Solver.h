/**	Container class for solver functions
 */
#pragma once
#include "Board.h"
#include "Location.h"
#include "Options.h"

#include <cassert>
#include <bitset>

namespace Sudoku
{
template<size_t N>
class Solver
{
	static const size_t base_size = N;
	static const size_t elem_size = N*N;

	using Location = Location<N>;
	using Options = Options<elem_size>;
	using Board = Sudoku::Board<Options, base_size>;
public:
	static void setValue(Board& board, const Location, const val_t);
	template<typename InItr_>
	static void setValue(Board& board, InItr_ begin, InItr_ end);

	template<typename InItr_>
	static void remove_option_section(Board& Board, InItr_ begin, const InItr_ end, const Location loc, const val_t value);

	template<typename InItr_>
	static size_t unique_section(Board& board, const InItr_ begin, const InItr_ end);

private:
	static void single_option(Board& options, const Location loc, const val_t value);

	//Options section_options(Board&, const InItr_ begin, const InItr_ end);
};

///	Make [value] the answer for [loc] and process
template<size_t N> inline
void Solver<N>::setValue(
	Board& board,
	const Location loc,
	const val_t value)
{
	assert(value <= board.elem_size);
	assert(board.at(loc)[value] == true);	// check if available option, if not invalid board
	board.at(loc).set(value);

	// process row / col / block
	single_option(board, loc, value);
}

///	set board using a vector of values
template<size_t N>
template<typename InItr_> inline
void Solver<N>::setValue(
	Board& board,
	InItr_ begin,
	InItr_ end)
{
	assert(end - begin == board.full_size);
	size_t n{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		Location loc(n++);
		if (*itr > 0)
		{
			if (board.at(loc).count() > 1) { setValue(board, loc, *itr); }
			else
			{
				assert(board.at(loc).is_answer(*itr));
			}
		}
	}
}

///	Process answer from [loc], update board options
template<size_t N> inline
void Solver<N>::single_option(Board& options, const Location loc, const val_t value)
{
	assert(loc.element() < loc.full_size);	// loc inside board
	assert(options.at(loc).is_answer());		// contains anwer
	assert(options.at(loc).is_answer(value));	// answer and correct value
	remove_option_section(
		options,
		options.row(loc).begin(),
		options.row(loc).end(),
		loc,
		value
	);
	remove_option_section(
		options,
		options.col(loc).begin(),
		options.col(loc).end(),
		loc,
		value
	);
	remove_option_section(
		options,
		options.block(loc).begin(),
		options.block(loc).end(),
		loc,
		value
	);
}

/// remove [value] in [loc] from other elements in section
template<size_t N>
template<typename InItr_> inline
void Solver<N>::remove_option_section(
	Board& Board,
	InItr_ begin,
	const InItr_ end,
	const Location loc,
	const val_t value)	// ingnore loc
{
	assert(Board[loc].is_answer(value));

	for (auto itr = begin; itr != end; ++itr)
	{
		if ( !(itr->is_answer()) )
		{
			if (itr->remove(value)) // true if applied
			{
				if (itr->count() == 1)	// new answer found=>cascade
				{
					Solver::setValue(Board, itr.location(), itr->answer());
				}
			}
		}
	}
}

/*
template<typename _InItr, size_t N> inline
void remove_option_section(
	Board<std::set<int>, N>& Board,
	_InItr begin,
	const _InItr end,
	const std::vector<Location<N>> loc,	// ignore elements in loc
	const int value)
{
	for (auto itr = begin; itr != end; ++itr)
	{
		if (loc.cend() == std::find_if(
			loc.cbegin(),
			loc.cend(),
			[itr](Location<N> loc) { return itr != loc; }))
		{
			if (itr->erase(value))
			{
				if (itr->size() == 1)	// cascade
				{
					Solver::single_option(Board, itr.location());
				}
			}
		}
	}
}
*/

template<size_t N>
template<typename InItr_> inline
size_t Solver<N>::unique_section(Board& board, const InItr_ begin, const InItr_ end)
{
	Options sum(0);		// helper all used
	Options worker(0);	// multiple uses OR answer
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr->is_answer())
		{
			worker = *itr + worker;
		}
		else
		{
			worker += (sum & *itr);
			sum += *itr;
		}
	}
	worker.flip();	// multiple uses -> single-use
	worker.add(0);	// set answer flag for correct behaviour count()

	// process uniques
	const size_t count = worker.count();
	if (count > 0)
	{
		std::vector<val_t> uniques{};
		uniques.reserve(count);
		uniques = worker.available();
		for (auto value : uniques)
		{
			Location loc = std::find_if(
				begin, end,
				[value](Options O) { return O.is_option(value); }
			).location();
			setValue(board, loc, value);
		}
	}
	return count;
}

/*
// 2x in block + zelfde block-row/col
// => remove from rest row/col
template<typename InItr_, size_t N> inline
void block_exclusive(Board<std::set<int>,N> board, InItr_ begin, InItr_ end)
{
	std::multiset<int> cache{ section_options(board, begin, end) };

	for (
		std::multiset<int>::const_iterator itr = cache.lower_bound(1);
		itr != cache.cend();
		++itr)
	{
		if (cache.count(*itr) == 1)	// = unique
		{
			unique(board, begin, end, itr);
		}
		else if (cache.count(*itr) <= N)
		{
			auto lambda = [itr](std::set<int> elem) {return (elem.count(*itr) == 1); };
			// check if all in same row or col
			std::vector<Location<N>> items{};
			auto found = std::find_if(begin, end, lambda);
			items.push_back(found.location());
			for (size_t i = 1; i < cache.count(*itr); ++i)
			{
				found = std::find_if(found, end, lambda);
				items.push_back(found.location());
			}
			bool is_row{ true };
			bool is_col{ true };
			for (Location<N>& loc : items)
			{
				if (loc.row() != items[1].row()) { is_row = false; }
				if (loc.col() != items[1].col()) { is_col = false; }
			}
			assert( !(is_row && is_col) );	// can't both be true
			if (is_row)
			{
				//setValue(board, items);
				remove_option_section(
					board,
					board.row(items[0]).begin(),
					board.row(items[0]).end(),
					items, *itr); 
			}
			// if is_col
		}
	}
}
*/
}	// namespace Sudoku