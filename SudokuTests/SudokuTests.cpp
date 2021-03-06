// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/SudokuTests.cpp                                    ---===//
//
// custom main()
//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char** argv)
{
	// std::cout << "Running main() from SudokuTests.cpp\n";
	testing::InitGoogleTest(&argc, argv); // parse command-line flags
	return RUN_ALL_TESTS();
}
