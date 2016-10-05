/**	Unit tests for the templates in the namespace Sudoku::Solver
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// File under test
#include "Solver.h"
// helpers

// library
#include <vector>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(NS_Solver)
{
public:
	TEST_METHOD(T1_setValue)
	{
		using namespace Sudoku;

		static const std::vector<val_t> v1
		{
			// start	// after setValue
			0,2, 0,0,	// 1	2	3	4
			4,0, 0,0,	// 4	3	1,2	1,2
			0,1, 4,0,	// 2,3	1	4	2,3
			0,0, 0,0	// 2,3	4	1,2	1,2,
		};
		Board<Options<4>, 2> board;
		Assert::IsTrue(board.at(5) == Options<4>{}, L"incorrect instantiation", LINE_INFO());
		try { Solver::setValue(board, v1.cbegin(), v1.cend()); }
		catch (std::exception&) { Assert::Fail(L"setValue failed in copying from vector", LINE_INFO()); }
		Assert::IsTrue(
			board[0][1] == 2 &&
			board[1][0] == 4 &&
			board[2][1] == 1 && board[2][2] == 4,
			L"setValue() from vector didn't copy data", LINE_INFO()
		);
		Assert::IsTrue(
			board[0][3] == 4 &&
			board[3][1] == 4,
			L"setValue() from vector didn't process single option cells", LINE_INFO()
		);
		Assert::IsTrue(
			board[0][0] == 1 && board[0][2] == 3 &&
			board[1][1] == 3 &&
			board[3][1] == 4,
			L"setValue() from vector didn't cascade over single option cells", LINE_INFO()
		);
	}
	TEST_METHOD(T2_unique)
	{
		using namespace Sudoku;

		static const std::vector<val_t> v1
		{
			// start	// after setValue
			0,2, 0,0,	// 1	2	3	4
			4,0, 0,0,	// 4	3	1,2	1,2
			0,1, 4,0,	// 2,3	1	4	2,3
			0,0, 0,0	// 2,3	4	1,2	1,2,
		};
		Board<Options<4>, 2> B1;
		Solver::setValue(B1, v1.cbegin(), v1.cend());
		const Board<Options<4>, 2> cB1{ B1 };	// copy to compare with
		// single row
		Solver::unique_section(B1, B1.row(0).cbegin(), B1.row(0).cend());
		Assert::IsTrue(B1 == cB1, L"row 0 was completely fixed by setValue", LINE_INFO());

		static const std::vector<val_t> v2
		{
			//start		// after setValue
			3,2, 0,0,	// 3	2	14	14
			0,0, 0,0,	// 14	14	134	12.34
			0,0, 2,0,	// 14	134	2	134
			0,0, 0,0	// 12.4	134	134	134
		};
		Board<Options<4>, 2> B2{};
		Solver::setValue(B2, v2.cbegin(), v2.cend());
		const Board<Options<4>, 2> cB2{ B2 };
		// single row 0
		Solver::unique_section(B2, B2.row(0).cbegin(), B2.row(0).cend());
		Assert::IsTrue(B2 == cB2, L"row 0 was completely fixed by setValue", LINE_INFO());
		// single row 1
		Solver::unique_section(B2, B2.row(1).cbegin(), B2.row(1).cend());
		Assert::IsTrue(B2 != cB2, L"row 1 should have changed", LINE_INFO());
		// full board
		try
		{
			for (size_t i = 0; i < B2.elem_size; ++i)
			{
				Solver::unique_section(B2, B2.row(i).cbegin(), B2.row(i).cend());
			}
		}
		catch (std::exception&) { Assert::Fail(L"unique_section() threw an exception"); }
		Assert::IsTrue(
			B2[0][0] == 3 && B2[0][1] == 2 &&
			B2[2][2] == 2,
			L"input values are lost", LINE_INFO()
		);
		Assert::IsTrue(
			B2[1][3] == 2 && B2[3][0] == 2,
			L"not all uniques found in rows", LINE_INFO()
		);
	}
	TEST_METHOD(T3_solve_board)
	{
		using namespace Sudoku;
		/*	start board					answer board
		*	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
		*	|		| 		|   1 2 |	| 6 7 3	| 8 9 4	| 5 1 2 |
		*	|		|	3 5 |		|	| 9 1 2	| 7 3 5 | 4 8 6	|
		*	| _ _ _ | 6 _ _ | _ 7 _ |	|_8_4_5_|_6_1_2_|_9_7_3_|
		*	| 7		|		| 3		|	| 7 9 8	| 2 6 1	| 3 5 4 |
		*	|		| 4		| 8		|	| 5 2 6	| 4 7 3	| 8 9 1	|
		*	| 1 _ _ | _ _ _	| _ _ _ |	|_1_3_4_|_5_8_9_|_2_6_7_|
		*	| 		| 1 2	|		|	| 4 6 9	| 1 2 8 | 7 3 5	|
		*	|	8	|		|	4	|	| 2 8 7	| 3 5 6	| 1 4 9	|
		*	|_ _5_ _|_ _ _ _|_6_ _ _|	|_3_5_1_|_9_4_7_|_6_2_8_|
		*/
		static const std::vector<int> b1
		{
			0, 0, 0,	0, 0, 0,	0, 1, 2,
			0, 0, 0,	0, 3, 5,	0, 0, 0,
			0, 0, 0,	6, 0, 0,	0, 7, 0,
			7, 0, 0,	0, 0, 0,	3, 0, 0,
			0, 0, 0,	4, 0, 0,	8, 0, 0,
			1, 0, 0,	0, 0, 0,	0, 0, 0,
			0, 0, 0,	1, 2, 0,	0, 0, 0,
			0, 8, 0,	0, 0, 0,	0, 4, 0,
			0, 5, 0,	0, 0, 0,	6, 0, 0
		};
		static const std::vector<int> b1a
		{
			6, 7, 3,	8, 9, 4,	5, 1, 2,
			9, 1, 2,	7, 3, 5,	4, 8, 6,
			8, 4, 5,	6, 1, 2,	9, 7, 3,
			7, 9, 8,	2, 6, 1,	3, 5, 4,
			5, 2, 6,	4, 7, 3,	8, 9, 1,
			1, 3, 4,	5, 8, 9,	2, 6, 7,
			4, 6, 9,	1, 2, 8,	7, 3, 5,
			2, 8, 7,	3, 5, 6,	1, 4, 9,
			3, 5, 1,	9, 4, 7,	6, 2, 8
		};
		Board<int, 3> start;
		std::copy(b1.cbegin(), b1.cend(), start.begin());
		Board<int, 3> answer;
		std::copy(b1a.cbegin(), b1a.cend(), answer.begin());
		Board<Options<9>, 3> options{};
		Solver::setValue(options, b1.cbegin(), b1.cend());
		for (size_t i = 0; i < options.elem_size; ++i)
		{
			Solver::unique_section(options, options.row(i).cbegin(), options.row(i).cend());
		}
	}
/*	TEST_METHOD(block_exclusive)
	{
		static const std::vector<int> b1
		{
			0, 0, 0,	0, 0, 0,	0, 1, 2,
			0, 0, 0,	0, 3, 5,	0, 0, 0,
			0, 0, 0,	6, 0, 0,	0, 7, 0,
			7, 0, 0,	0, 0, 0,	3, 0, 0,
			0, 0, 0,	4, 0, 0,	8, 0, 0,
			1, 0, 0,	0, 0, 0,	0, 0, 0,
			0, 0, 0,	1, 2, 0,	0, 0, 0,
			0, 8, 0,	0, 0, 0,	0, 4, 0,
			0, 5, 0,	0, 0, 0,	6, 0, 0
		};
		std::set<int> chars{ 1,2,3,4,5,6,7,8,9 };
		Sudoku::Board<std::set<int>, 3> options(chars);
		for (size_t i = 0; i < options.elem_size; ++i)
		{
			Sudoku::Solver::block_exclusive(options, options.block(i).cbegin(), options.block(i).cend());
		}
	
	}*/
};
}	// namespace Sudoku_Test