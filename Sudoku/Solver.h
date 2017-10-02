//===--	Sudoku/Solver.h													--===//
//
//	Solver function container-class
//	Collects and gives access to solver functions acting on a Board<Options>&
//===---------------------------------------------------------------------===//
#pragma once

#include "Options.h"
#include "Solvers_Appearance.h"

#include <array>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

// Forward declarations
#include "Board.fwd.h"
#include "Location.fwd.h"
#include "Solver.fwd.h"

/* experiment flags */
//! DO #undef at end of file!
// activate algorithms on removing option
#define DUAL_ON_REMOVE 1
#define MULTIPLE_ON_REMOVE true

namespace Sudoku
{
template<int N>
class Solver
{
	using Location = Location<N>;

	static constexpr int base_size = Location().base_size; // default 3
	static constexpr int elem_size = Location().elem_size; // default 9
	static constexpr int full_size = Location().full_size; // default 81

	using Options = Options<elem_size>;
	using Board   = Sudoku::Board<Options, base_size>;
	using Row     = typename Board::Row;
	using Col     = typename Board::Col;
	using Block   = typename Board::Block;

public:
	Solver(Board&);

	// Edit Board
	void setValue(Location, int);
	template<typename InItr_>
	void setValue(InItr_ begin, InItr_ end);

	// remove an option: triggers solvers single_option()
	int remove_option(Location, int value);
	template<typename SectionT, typename LocT, typename ValT>
	int remove_option_section(SectionT, LocT ignore, ValT value);
	template<typename InItr_>
	int remove_option_section(
		InItr_ begin, InItr_ end, Location ignore, int value);
	template<typename SectionT>
	int remove_option_section(
		SectionT section, const std::vector<Location>& ignore, int value);
	template<typename InItr_>
	int remove_option_section(
		InItr_ begin,
		InItr_ end,
		const std::vector<Location>& ignore,
		int value);
	template<typename InItr_>
	int remove_option_section(
		InItr_ begin,
		InItr_ end,
		const std::vector<Location>& ignore,
		const std::vector<int>& value);

	// Solvers
	int single_option(Location);
	int single_option(Location, int value);
	int dual_option(Location);
	int multi_option(Location, unsigned int = 0);

	template<typename SectionT>
	int unique_in_section(SectionT);
	template<typename InItr_>
	int unique_in_section(InItr_ begin, InItr_ end);
	template<typename InItr_>
	auto set_uniques(InItr_ begin, InItr_ end, const Options& worker);

	template<typename InItr_>
	int section_exclusive(InItr_ begin, InItr_ end);
	template<typename InItr_>
	int block_exclusive(InItr_ begin, InItr_ end);
	// TODO simplify calling
	int section_exclusive(Block);

private:
	Board& board_;

	template<typename InItr_>
	int set_section_locals(
		InItr_ begin, InItr_ end, int rep_count, const Options& worker);
	template<typename InItr_>
	int set_block_locals(
		InItr_ begin, InItr_ end, int rep_count, const Options& worker);

	// template<typename InItr_>
	// auto appearance_sets(InItr_ begin, InItr_ end) const;
	template<typename InItr_>
	auto appearance_sets(InItr_ begin, InItr_ end) const
	{
		return Solvers_::appearance_sets<N>(begin, end);
	}
	template<typename InItr_>
	auto find_locations(
		InItr_ begin, InItr_ end, int rep_count, int value) const;

	template<typename InItr_>
	int remove_option_outside_block(
		InItr_ begin, InItr_ end, Location block, int value);
};


template<int N>
inline Solver<N>::Solver(Board& options) : board_(options)
{
	// empty constructor
}

//	IF valid, Make [value] the answer for [loc]
//??? and process
template<int N>
inline void Solver<N>::setValue(const Location loc, const int value)
{
	assert(is_valid_value<N>(value));
	if (!board_.at(loc).test(value))
	{
		throw std::logic_error{"Invalid Board"};
	}
	board_[loc].set_nocheck(value);

	//? process row / col / block
	//x single_option(loc, value);
}

//	set board_ using a transferable container of values
template<int N>
template<typename InItr_>
inline void Solver<N>::setValue(const InItr_ begin, const InItr_ end)
{
	// TODO check iterator type, at least: forward_iterator_tag
	assert(end - begin == full_size);

	int n{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		Location loc(n++); // start at 0!
		if (*itr > 0 && board_.at(loc).is_option(*itr))
		{
			setValue(loc, *itr);
			single_option(loc, *itr);
		}
		// check invalid value or conflict
		assert(*itr == 0 || board_.at(loc).is_answer(*itr));
	}
}

template<int N>
inline int Solver<N>::remove_option(const Location loc, const int value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	int changes{};
	auto& item{board_.at(loc)};

	if (item.is_option(value))
	{
		++changes;
		switch (const int count = item.remove_option(value).count())
		{
			// remaining options
		case 0: assert(false); break; // never trigger, removed last option
		case 1: changes += single_option(loc, item.get_answer()); break;
#if DUAL_ON_REMOVE == true
		case 2: changes += dual_option(loc); break;
#endif // dual
		default:
#if MULTIPLE_ON_REMOVE == true
			changes += multi_option(loc, count);
#endif // multiple
			break;
		}
	}
	return changes;
}

//	Check if only one option remaining
//	IF true: process answer
template<int N>
inline int Solver<N>::single_option(const Location loc)
{
	if (const int answer{board_[loc].get_answer()})
	{
		return single_option(loc, answer);
	}
	return 0;
}

//	Process answer from [loc], update board_ options
template<int N>
inline int Solver<N>::single_option(const Location loc, const int value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	assert(board_.at(loc).test(value));
	assert(board_[loc].count_all() == 1);

	if (!board_[loc].is_answer(value))
	{
		setValue(loc, value);
	}
	int changes{};
	changes += remove_option_section(board_.row(loc), loc, value);
	changes += remove_option_section(board_.col(loc), loc, value);
	changes += remove_option_section(board_.block(loc), loc, value);
	return changes;
}

// if 2 options in element:
// find exact pair in section:
// remove form other elements in section
template<int N>
inline int Solver<N>::dual_option(const Location loc)
{
	assert(is_valid(loc));
	assert(board_.at(loc).count() == 2);

	int changes{};
	const Options& item{board_[loc]};
	for (int i{}; i < full_size; ++i)
	{
		// find exact same in board
		if (board_[Location(i)] == item && Location(i) != loc)
		{
			// Remove values for rest of shared elements
			if (shared_row(loc, Location(i)))
			{
				changes += remove_option_section(
					board_.row(loc).begin(),
					board_.row(loc).end(),
					std::vector<Location>{loc, Location(i)},
					item.available());
			}
			else if (shared_col(loc, Location(i)))
			{
				changes += remove_option_section(
					board_.col(loc).begin(),
					board_.col(loc).end(),
					std::vector<Location>{loc, Location(i)},
					item.available());
			}
			if (shared_block(loc, Location(i)))
			{
				// NOTE this is slow
				changes += remove_option_section(
					board_.block(loc).begin(),
					board_.block(loc).end(),
					std::vector<Location>{loc, Location(i)},
					item.available());
			}
		}
	}
	return changes;
}

//	finds equal sets in section:
//	removes form others in section
template<int N>
inline int Solver<N>::multi_option(const Location loc, unsigned int count)
{
	assert(is_valid(loc));

	if (!count)
	{
		count = board_[loc].count();
	}
	constexpr auto specialize = 2; // use specialization below and including
	constexpr auto max_size   = elem_size / 2; //?? Assumption, Proof needed
	if (specialize < count && count <= max_size)
	{
		int changes{};                    // performance counter
		const Options& item{board_[loc]}; // input item, to find matches to
		std::vector<Location> list{};     // potential matches
		// traverse whole board
		for (int i{}; i < full_size; ++i)
		{
			const auto& other = board_[Location(i)];
			// find exact same in board
			// TODO rework to also pick-up cels containing [2,n) values
			if (other.count() > 0 && // include not processed answers
				other.count() <= base_size &&
				(other == item || (item & other).count() == other.count()))
			{
				list.push_back(Location(i));
			}
		}
		//
		//	This algorithm is supprizingly robust
		//	Example, showing operation if no specializations where applied
		//			start	end	/	start	end		/	start	end
		//	item =	1,2,3	3	/	1,2,3	1,2,3	/	1,2,3	1,2,3
		//	list1	1,2		1	/	1,2,3	1,2,3	/	1,2		1,2
		//	list2	2		2	/	1,2		1,2		/	1,2		1,2
		//	extern	1,2,3,4	4	/	1,2,3,4	4		/	1,2,3,4	4
		//
		//	item	1,2,3,4		1,2,3,4
		//	list1	1,2,3		1,2,3
		//	list2	1,2,3		1,2,3
		//	list3	1,2,3		1,2,3
		//	other	1,2,3,4,5	5
		//	Only the 2nd example could only be processed by this method
		//
		if (list.size() >= count)
		{
			// find if: count amount of items share an element
			auto in_row{shared_row(loc, list)};
			auto in_col{shared_col(loc, list)};
			auto in_block{shared_block(loc, list)};
			// Remove values for rest of shared elements
			if (in_row.size() == count)
			{
				changes += remove_option_section(
					board_.row(loc), in_row, item.available());
			}
			if (in_col.size() == count)
			{
				changes += remove_option_section(
					board_.col(loc), in_col, item.available());
			}
			if (in_block.size() == count)
			{
				// NOTE this is slow
				changes += remove_option_section(
					board_.block(loc), in_block, item.available());
			}
		}
		return changes;
	}
	return 0;
}

//	remove value(s) from other elements in section
template<int N>
template<typename SectionT, typename LocT, typename ValT>
inline int Solver<N>::remove_option_section(
	SectionT section, const LocT ignore, const ValT value)
{
	// referal function, creates iterators
	static_assert(std::is_base_of_v<Board::Section, SectionT>);

	using traits = std::iterator_traits<SectionT::iterator>;
	static_assert(std::is_object_v<traits::iterator_category>);
	static_assert(std::is_same_v<traits::value_type, Options>);

	return remove_option_section(section.begin(), section.end(), ignore, value);
}

//	remove [value] in [loc] from other elements in section
template<int N>
template<typename InItr_>
inline int Solver<N>::remove_option_section(
	const InItr_ begin,
	const InItr_ end,
	[[maybe_unused]] const Location loc, // not used in release mode
	const int value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));
	assert(board_.at(loc).is_answer(value));

	int changes{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		if (!(itr->is_answer()))
		{
			changes += remove_option(itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from element if not part of [block]
template<int N>
template<typename InItr_>
inline int Solver<N>::remove_option_outside_block(
	const InItr_ begin, const InItr_ end, const Location block, const int value)
{
	assert(is_valid(block));
	assert(is_valid_value<N>(value));

	int changes{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		if (!(itr->is_answer() || itr.location().block() == block.block()))
		{
			changes += remove_option(itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from set if not part of [loc]s
template<int N>
template<typename InItr_>
inline int Solver<N>::remove_option_section(
	const InItr_ begin,
	const InItr_ end,
	const std::vector<Location>& ignore,
	const int value)
{
	assert(is_valid_value<N>(value));

	int changes{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		// clang-format off
		if (itr->is_option(value) &&
			ignore.cend() == std::find_if(
				ignore.cbegin(),
				ignore.cend(),
				[itr](Location loc) { return itr == loc; }
			)) // <algorithm>
		// clang-format on
		// TODO replace find_if() with binary_search(), requires sorted data
		// cheaper, if sorting can be done by design
		{
			changes += remove_option(itr.location(), value);
		}
	}
	return changes;
}

// remove [value]s from [section] if not part of [loc]s
// the ignore Location vector must be sorted
template<int N>
template<typename InItr_>
inline int Solver<N>::remove_option_section(
	const InItr_ begin,
	const InItr_ end,
	const std::vector<Location>& ignore,
	const std::vector<int>& values)
{
	int changes{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		// clang-format off
		if (!(itr->is_answer()) &&
			ignore.cend() == std::find_if(
				ignore.cbegin(),
				ignore.cend(),
				[itr](Location loc) { return itr.location() == loc; }
			)) // <algorithm>
		// clang-format on
		// TODO replace find_if() with binary_search(), requires sorted data
		// cheaper, if sorting can be done by design
		{
			for (auto v : values)
			{
				changes += remove_option(itr.location(), v); // cascade
			}
		}
	}
	return changes;
}

//	Solver: Find and process options appearing only once in a section
template<int N>
template<typename SectionT>
inline int Solver<N>::unique_in_section(SectionT section)
{
	static_assert(std::is_base_of_v<Board::Section, SectionT>);

	const auto& worker = Solvers_::appearance_once<N>(section);
	return set_uniques(section.begin(), section.end(), worker);
}

//	Solver: Find and process options appearing only once a set
template<int N>
template<typename InItr_>
inline int Solver<N>::unique_in_section(const InItr_ begin, const InItr_ end)
{
	const auto& worker = Solvers_::appearance_once<elem_size>(begin, end);
	return set_uniques(begin, end, worker);
}

//	process unique values in section
template<int N>
template<typename InItr_>
inline auto
	Solver<N>::set_uniques(InItr_ begin, InItr_ end, const Options& worker)
{
	int changes{0};
	if (worker.count_all() > 0)
	{
		for (int value{1}; value < worker.size(); ++value)
		{
			if (worker[value])
			{
				const auto itr = std::find_if( // <algorithm>
					begin,
					end,
					[value](Options O) { return O.is_option(value); });
				assert(itr != end); // doesn't exist
				setValue(itr.location(), value);
				changes += single_option(itr.location(), value);
				++changes;
			}
		}
	}
	return changes;
}


//	Solver: find and process values appearing 2x or 3x in row/col:
//	IF all in same block -> remove from rest of block
template<int N>
template<typename InItr_>
inline int Solver<N>::section_exclusive(const InItr_ begin, const InItr_ end)
{
	int changes{}; // performance counter

	auto appearing = appearance_sets(begin, end);

	int i{2};
	while (i <= N) // won't run if condition fails
	{
		// unique specialization
		if (appearing[1].count_all() > 0)
		{
			changes += set_uniques(begin, end, appearing[1]);
			appearing = appearance_sets(begin, end);
		}
		else if (appearing[i].count_all() > 0)
		{
			changes += set_section_locals(begin, end, i, appearing[i]);
			appearing = appearance_sets(begin, end);
			++i;
		}
		else
		{
			++i;
		}
	}
	return changes;
}

template<int N>
template<typename InItr_>
inline int Solver<N>::block_exclusive(const InItr_ begin, const InItr_ end)
{
	// TODO ensure it's a block?
	// TODO type-checking on board_

	int changes{}; // performance counter

	int i{1};
	auto appearing = appearance_sets(begin, end);
	while (i <= N) // won't run if condition fails
	{
		// unique in block specialization
		if (appearing[1].count_all() > 0)
		{
			changes += set_uniques(begin, end, appearing[1]);
			appearing = appearance_sets(begin, end);
		}
		else if (appearing[i].count_all() > 0)
		{
			changes += set_block_locals(begin, end, i, appearing[i]);
			appearing = appearance_sets(begin, end);
			++i;
		}
		else
		{
			++i;
		}
	}
	return changes;
	// TODO can this be added/used?
	// 2 values only appear in 2 cells -> remove rest from cells
}

template<int N>
template<typename InItr_>
inline int Solver<N>::set_section_locals(
	const InItr_ begin,
	const InItr_ end,
	const int rep_count,
	const Options& worker)
{
	assert(worker.count_all() > 0); // should have been cought by caller

	int changes{0};
	for (size_t value{1}; value < worker.size(); ++value)
	{
		if (worker.test(value))
		{
			const auto locations = find_locations(begin, end, rep_count, value);
			assert(locations.size() <= N); // won't fit in single block-row/col
			assert(locations.size() > 1);  // use the set_uniques specialization

			// for each value check if appearing in same block
			if (locations.cend() ==
				std::find_if_not(
					locations.cbegin(), locations.cend(), [&locations](auto L) {
						return L.block() == locations[0].block();
					}))
			{
				changes += remove_option_section(
					board_.block(locations[0]).begin(),
					board_.block(locations[0]).end(),
					locations,
					value);
			}
		}
	}
	return changes;
}

template<int N>
template<typename InItr_>
inline int Solver<N>::set_block_locals(
	const InItr_ begin,
	const InItr_ end,
	const int rep_count,
	const Options& worker)
{
	assert(worker.count_all() > 0); // should have been cought by caller

	int changes{0};
	for (int value{1}; value < worker.size(); ++value)
	{
		if (worker.test(value))
		{
			const auto locations = find_locations(begin, end, rep_count, value);
			assert(locations.size() <= N); // won't fit in single block-row/col
			assert(locations.size() > 1);  // use the set_uniques specialization

			// for each value check if appearing in same row/col
			if (locations.cend() ==
				std::find_if_not(
					locations.cbegin(), locations.cend(), [&locations](auto L) {
						return L.row() == locations[0].row();
					}))
			{
				changes += remove_option_outside_block(
					board_.row(locations[0]).begin(),
					board_.row(locations[0]).end(),
					locations[0],
					value);
			}
			else if (
				locations.cend() ==
				std::find_if_not(
					locations.cbegin(), locations.cend(), [&locations](auto L) {
						return L.col() == locations[0].col();
					}))
			{
				changes += remove_option_outside_block(
					board_.col(locations[0]).begin(),
					board_.col(locations[0]).end(),
					locations[0],
					value);
			}
			else
			{
				continue;
			} // not in same row/col
		}
	}
	return changes;
}

template<int N>
template<typename InItr_>
inline auto Solver<N>::find_locations(
	const InItr_ begin,
	const InItr_ end,
	const int rep_count,
	const int value) const
{
	assert(is_valid_value<N>(value));
	assert(rep_count > 0 && rep_count <= board_.full_size);

	std::vector<Location> locations{};
	auto last = begin;
	for (int i{0}; i < rep_count; ++i)
	{
		last = std::find_if(
			last, end, [value](Options O) { return O.is_option(value); });
		assert(last != end); // incorrect rep_count
		locations.emplace_back(last.location());
		++last;
	}
	return locations;
}


// cell containing 2 options
//	find in board_	size() == 2
//		find exact same in rest board
//			IF pair in same row/col -> remove values from rest row/col
//			IF same block -> remove values from rest block
//		repeat until end of board_
//	repeat until end of board_


// cell containing 2 options
//	find in board_	size() == 2
//		find exact same in rest row
//			IF found:	remove values from rest row
//		find exact same in rest col
//		find exact same in rest block


/*	Remove local macros */
#undef DUAL_ON_REMOVE
#undef MULTIPLE_ON_REMOVE

} // namespace Sudoku
