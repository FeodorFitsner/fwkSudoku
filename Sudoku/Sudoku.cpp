// Sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SolverBase.h"

#include <iostream>

int main()
{
	static const std::vector<val_t> b1
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
	static const std::vector<val_t> b1a
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

	Sudoku::Board<val_t, 3> start;
	std::copy(b1.cbegin(), b1.cend(), start.begin());
	Sudoku::Board<val_t, 3> answer;
	std::copy(b1a.cbegin(), b1a.cend(), answer.begin());

	// working object:
	Sudoku::SolverBase<3> other;
	// setup: generate options & find single options
	Sudoku::SolverBase<3> first(start);	//ERROR core-guidelines
	// find uniques (should solve this board)
	first.solver_unique();

	Sudoku::Board<val_t, 3> result = first.getResult();
	if (result == answer) { std::cout << " : ) Found the answer!\n"; }
	else { std::cout << ":{ bugs ...\n"; }

	return 0;
}
