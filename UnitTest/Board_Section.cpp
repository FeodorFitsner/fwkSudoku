/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Board.h"
// aditional

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(Class_Board_Section)
{
public:
	TEST_METHOD(properties)
	{
		Sudoku::Board<int, 3> A;
		Assert::IsTrue(A.row(0).id() == 0 &&
					   A.row(3).id() == 3 &&
					   A.row(8).id() == 8, L"Row::id()", LINE_INFO());
		Assert::IsTrue(A.col(0).id() == 0 &&
					   A.col(3).id() == 3 &&
					   A.col(8).id() == 8, L"Col::id()", LINE_INFO());
		Assert::IsTrue(A.block(0).id() == 0 &&
					   A.block(3).id() == 3 &&
					   A.block(8).id() == 8, L"Block::id()", LINE_INFO());
		Assert::IsTrue(A.row(0).size() == 9 &&
					   A.col(7).size() == 9 &&
					   A.block(8).size() == 9, L"Section::size()", LINE_INFO());
		Sudoku::Board<int, 2> B;
		Assert::IsTrue(B.row(1).id() == 1 &&
					   B.col(3).id() == 3 &&
					   B.block(3).id() == 3, L"Section::id()", LINE_INFO());
		Assert::IsTrue(B.col(2).size() == 4, L"Section.size() 2", LINE_INFO());
	}
	TEST_METHOD(section_element_access)
	{
		Sudoku::Board<int, 3> B;

		Assert::IsTrue(B.row(0).size() == 9, L"...");
		Assert::IsTrue(B.row(0).at(0) == 0, L"");
		try { B.row(5).at(3) = 8; }
		catch (const std::exception&)
		{
			Assert::Fail(L"Write to row with at() failed", LINE_INFO());
		}
		try { B.row(8)[8] = 2; }
		catch (const std::exception&)
		{
			Assert::Fail(L"Write to row section with [] failed", LINE_INFO());
		}
		Assert::IsTrue(B[5][3] == 8, L"Write using section failed 1", LINE_INFO());
		Assert::IsTrue(B[8][8] == 2, L"Write using section failed 2", LINE_INFO());
		Assert::IsTrue(B.row(8).at(8) == 2, L"Reading from row section with at() failed", LINE_INFO());
		Assert::IsTrue(B.row(5)[3] == 8, L"Reading from row section with [] failed", LINE_INFO());

		Assert::IsTrue(B.col(3)[5] == 8, L"Col section access failed", LINE_INFO());
		Assert::IsTrue(B.block(8)[8] == 2, L"Block section access failed", LINE_INFO());

		auto block_unit = B.block(4);
		try
		{
			for (size_t i = 0; i < 9; ++i)
			{
				block_unit[i] = 9 - i;
			}
			for (size_t i = 1; i < 4; ++i)
			{
				Assert::IsTrue(B.col(i+2)[5] == 4-i, L"loop value different", LINE_INFO());
			}
		}
		catch (const std::exception&)
		{
			Assert::Fail(L"Loop over block failed", LINE_INFO());
		}


		//try
		//{
		//	size_t total{};
		//	const Sudoku::Board<int, 3>::Row& vs = B.row(5);
		//	//TODO working range-for loop on sections
		//	//p79
		//	//for (auto& s : BBoard.row(5) )
		//	//{
		//		//total += s;
		//	//}
		//}
		//catch (const std::exception&)
		//{
		//	Assert::Fail(L"range-for loop on Section::Row", LINE_INFO());
		//}
	}
private:

};
} // namespace Sudoku_Test