﻿//===--- SudokuTests/Size.cpp                                           ---===//
//
//	Unit tests for the template class Sudoku::Size
//===----------------------------------------------------------------------===//
//	Implemented with GoogleTest
//
//	Notes:
//	gTest is limited for use with multiple template parameters.
//	These expressions need to be implemented between extra parentheses
//	- test elements implementing this are flagged with [gTest]
//	- not implemented tests are flagged as NEEDTEST [gTest]
//	gTest tries to print iterators if they use inheritance,
//		if used in *_EQ/NE etc.
//		use an explicit test like EXPECT_TRUE(.. == ..).
//
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Size.h>

// Helpers

// additional


namespace SudokuTests::SizeTest
{
using ::Sudoku::Size;

namespace compiletime
{ // Type properties
	using typeT = Size<3>;
	// http://howardhinnant.github.io/TypeHiearchy.pdf
	// Composite Type Categories
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_object_v<typeT>);
	static_assert(std::is_compound_v<typeT>);

	static_assert(not std::is_scalar_v<typeT>);
	static_assert(not std::is_array_v<typeT>);
	static_assert(not std::is_union_v<typeT>);
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_reference_v<typeT>);
	static_assert(not std::is_function_v<typeT>);
	// Type Properties
	static_assert(not std::is_const_v<typeT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(not std::has_unique_object_representations_v<typeT>);
	static_assert(std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(std::is_aggregate_v<typeT>);

	// default constructor: typeT()
	static_assert(std::is_default_constructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_default_constructible<typeT>::value);   // ++
	static_assert(std::is_trivially_default_constructible<typeT>::value); // ++
	// ++ nothing virtual

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_copy_constructible<typeT>::value);   // ++
	static_assert(std::is_trivially_copy_constructible<typeT>::value); // +

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_move_constructible<typeT>::value);   // ++
	static_assert(std::is_trivially_move_constructible<typeT>::value); // ++

	// copy assignment
	static_assert(std::is_copy_assignable<typeT>::value);           // ++
	static_assert(std::is_nothrow_copy_assignable<typeT>::value);   // ++
	static_assert(std::is_trivially_copy_assignable<typeT>::value); // ++

	static_assert(std::is_move_assignable<typeT>::value);           // ++
	static_assert(std::is_nothrow_move_assignable<typeT>::value);   // ++
	static_assert(std::is_trivially_move_assignable<typeT>::value); // ++

	static_assert(std::is_destructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_destructible<typeT>::value);   // ++
	static_assert(std::is_trivially_destructible<typeT>::value); // ++
	static_assert(!std::has_virtual_destructor<typeT>::value);   // --

	static_assert(std::is_swappable<typeT>::value);         // C++17
	static_assert(std::is_nothrow_swappable<typeT>::value); // C++17

	static_assert(!std::is_swappable_with<typeT, int>::value);          // C++17
	static_assert(!std::is_swappable_with<typeT, unsigned int>::value); // C++17

	static_assert(Size<2>::base == 2);
	static_assert(Size<3>::base == 3);
	static_assert(Size<3>::elem == 9);
	static_assert(Size<3>::full == 81);
	static_assert(Size<3>::base == ::Sudoku::base_size<3>);
	static_assert(Size<3>::elem == ::Sudoku::elem_size<3>);
	static_assert(Size<3>::full == ::Sudoku::full_size<3>);

	static_assert(std::is_scalar_v<decltype(Size<3>::base)>);
	static_assert(std::is_scalar_v<decltype(Size<3>::elem)>);
	static_assert(std::is_scalar_v<decltype(Size<3>::full)>);
	static_assert(std::is_signed_v<decltype(Size<3>::base)>);

} // namespace compiletime

} // namespace SudokuTests::SizeTest
