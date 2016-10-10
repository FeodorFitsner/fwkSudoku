/**	Unit tests for the template class Sudoku::Options
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Options.h"
// additional
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sudoku;

namespace Sudoku_Test
{
TEST_CLASS(Class_Options)
{
public:
	TEST_METHOD(T0_initialize)
	{
		static_assert(std::is_class<Options<9>>(), "should be a class, hiding datarepresentation");
		static_assert(std::is_standard_layout<Options<9>>(), "should have standard layout");
		static_assert(std::is_trivially_copyable<Options<9>>(), "should be trivially copyable");
		static_assert(!std::is_empty<Options<9>>(), "memberdata missing");
		static_assert(std::is_standard_layout<Options<9>>(), "should have standard layout");
		//C++17 static_assert(std::has_unique_object_representations<Options<9>>(), "");
		static_assert(std::is_default_constructible<Options<9>>(), "default constructor");
		//static_assert(std::is_trivially_default_constructible<Options<9>>(), "not using the default constructor");
		static_assert(std::is_nothrow_default_constructible<Options<9>>(), "notrow default constructor");
		static_assert(std::is_copy_constructible<Options<9>>(), "copy constructor");
		static_assert(std::is_trivially_copy_constructible<Options<9>>(), "trivially copy constructor");
		static_assert(std::is_nothrow_copy_constructible<Options<9>>(), "notrow copy constructor");
		static_assert(std::is_move_constructible<Options<9>>(), "move constructor");
		static_assert(std::is_trivially_move_constructible<Options<9>>(), "trivially move constructor");
		static_assert(std::is_nothrow_move_constructible<Options<9>>(), "nothrow move constructor");
		static_assert(std::is_copy_assignable<Options<9>>(), "copy assignable");
		static_assert(std::is_trivially_copy_assignable<Options<9>>(), "trivially copy assignable");
		static_assert(std::is_nothrow_copy_assignable<Options<9>>(), "notrow copy assignable");
		static_assert(std::is_move_assignable<Options<9>>(), "move assignable");
		static_assert(std::is_trivially_move_assignable<Options<9>>(), "trivially move assignable");
		static_assert(std::is_nothrow_move_assignable<Options<9>>(), "move assignable");
		static_assert(std::is_destructible<Options<9>>(), "destructable");
		static_assert(std::is_trivially_destructible<Options<9>>(), "trivially destructable");
		static_assert(std::is_nothrow_destructible<Options<9>>(), "nothrow destructable");
		//C++17 static_assert(std::is_swappable<Options<9>>(), "swappable");
		//C++17 static_assert(std::is_nothrow_swappable<Options<9>>(), "swappable");
		//C++17 static_assert(std::is_swappable_with<Options<9>, std::bitset<10>>(), "");
		//C++17 static_assert(std::is_nothrow_swappable_with<Options<9>, std::bitset<10>>(), "");

		/// type construction
		static_assert(std::is_constructible<Options<3>, const std::bitset<4>&>(), "construct from const std::bitset&");
		static_assert(!std::is_constructible<Options<3>, const std::bitset<3>&>(), "shouldn't accept non matching dimensions_1");
		static_assert(!std::is_constructible<Options<3>, const std::bitset<2>&>(), "shouldn't accept non matching dimensions_2");
		static_assert(std::is_assignable<Options<3>, std::bitset<4>>(), "assign from std::bitset");
		static_assert(!std::is_assignable<Options<3>, std::bitset<3>>(), "shouldn't accept non matching dimensions_3");
		static_assert(!std::is_assignable<Options<3>, std::bitset<2>>(), "shouldn't accept non matching dimensions_4");
		static_assert(std::is_constructible<Options<3>, val_t>(), "construct from val_t");
		static_assert(std::is_constructible<Options<3>, unsigned int>(), "construct from unsigned int");
		static_assert(std::is_assignable<Options<3>, val_t>(), "assign from val_t");

		Sudoku::Options<9> two{};
		Sudoku::Options<4> O_3{std::bitset<5>()};
		Sudoku::Options<4> O_4{ 2 };
	}
	TEST_METHOD(T1_memberfunctions)
	{
		// _test-data
		Sudoku::Options<9> O_1{};
		Sudoku::Options<9> O_2{};
		const Sudoku::Options<4> O_3{ std::bitset<5>{"00100"} };	// answer = 2
		const Sudoku::Options<4> O_4{ std::bitset<5>{"01011"} };	// 2 options 1 & 3
		const Sudoku::Options<4> E_1{ std::bitset<5>{"00000"} };	// empty
		const Sudoku::Options<4> E_2{ std::bitset<5>{"00001"} };	// empty, no answer

		static_assert(noexcept(O_1.count()), "count() should be noexcept");
		Assert::IsTrue(O_1.count() == 9, L"count() failed", LINE_INFO());
		Assert::IsTrue(O_3.count() == 0, L"count() failed on answer", LINE_INFO());
		Assert::IsTrue(O_4.count() == 2, L"count() failed_2", LINE_INFO());
		Assert::IsTrue(E_1.count() == 0, L"count() failed on empty", LINE_INFO());
		Assert::IsTrue(E_2.count() == 0, L"count() failed on empty_2", LINE_INFO());

		static_assert(noexcept(O_1.count_all()), "count_all() should be noexcept");
		Assert::IsTrue(O_1.count_all() == 9, L"count_all() failed", LINE_INFO());
		Assert::IsTrue(O_3.count_all() == 1, L"count_all() failed on answer", LINE_INFO());
		Assert::IsTrue(O_4.count_all() == 2, L"count_all() failed_2", LINE_INFO());
		Assert::IsTrue(E_1.count_all() == 0, L"count_all() failed on empty", LINE_INFO());
		Assert::IsTrue(E_2.count_all() == 0, L"count_all() failed on empty_2", LINE_INFO());

		static_assert(!noexcept(O_1.test(2)), "test() is NOT noexcept");
		Assert::IsTrue(O_1.test(2), L"test(value) failed", LINE_INFO());
		Assert::IsTrue(O_3.test(2), L"test(value) failed_2", LINE_INFO());
		Assert::IsFalse(O_3.test(1), L"test(value) inverse failed", LINE_INFO());
		try { if (O_1.test(15)) { Assert::Fail(L"out-of-bounds expected"); } }
		catch (const std::out_of_range&) {} // caught
		catch (const std::exception&) { Assert::Fail(L"test() unexpected exception"); }

		static_assert(noexcept(O_1.is_answer()), "is_answer() should be noexcept");
		Assert::IsTrue(O_3.is_answer(), L"is_answer() failed", LINE_INFO());
		Assert::IsFalse(O_1.is_answer(), L"is_answer() inverse failed", LINE_INFO());
		Assert::IsFalse(O_4.is_answer(), L"is_answer() inverse failed_2", LINE_INFO());
		Assert::IsFalse(E_2.is_answer(), L"is_answer() inverse failed_4", LINE_INFO());
		static_assert(!noexcept(O_1.is_answer(1)), "is_answer(val_t) is NOT noexcept");
		Assert::IsTrue(O_3.is_answer(2), L"is_answer(answer) failed", LINE_INFO());
		Assert::IsFalse(O_3.is_answer(1), L"is_answer(val_t) failed", LINE_INFO());
		Assert::IsFalse(O_1.is_answer(2), L"is_answer(random) failed", LINE_INFO());

		// empty with answer flag
		Assert::IsTrue(E_1.is_answer(), L"is_answer() on 0 answer", LINE_INFO());
		Assert::IsFalse(E_1.is_answer(0), L"is_answer(0) returned true", LINE_INFO());

		static_assert(!noexcept(O_1.is_option(2)), "is_option(val_t) is NOT noexept");
		Assert::IsTrue(O_1.is_option(1), L"is_option(val_t) failed", LINE_INFO());
		Assert::IsFalse(O_3.is_option(1), L"is_option(val_t) inverse failed", LINE_INFO());
		Assert::IsFalse(O_3.is_option(2), L"is_option(val_t) inverse failed_2", LINE_INFO());

		//constexpr bool is_empty() const noexcept;
		static_assert(noexcept(O_1.is_empty()), "is_empty() should be noexcept");
		Assert::IsTrue(E_1.is_empty(), L"is_empty() failed", LINE_INFO());
		Assert::IsFalse(O_1.is_empty(), L"is_empty() inverse failed", LINE_INFO());
		Assert::IsTrue(E_2.is_empty(), L"is_empty() failed on special case", LINE_INFO());

		static_assert(noexcept(O_1.clear()), "clear() should be noexcept");
		O_2.clear();
		Assert::IsTrue(O_2.is_empty(), L"clear() failed", LINE_INFO());
		Assert::IsTrue(O_2.count() == 0, L"count() failed on empty", LINE_INFO());
		//Assert::IsFalse(O_2.is_answer(), L"is_answer() failed on empty", LINE_INFO());
		Assert::IsFalse(O_2.is_answer(2), L"is_answer(val_t) failed on empty", LINE_INFO());
		Assert::IsFalse(O_2.is_option(2), L"is_option(val_t) failed on empty", LINE_INFO());

		static_assert(noexcept(O_1.reset()), "reset() should be noexcept");
		O_2.reset();
		Assert::IsFalse(O_2.is_empty(), L"reset() failed", LINE_INFO());
		Assert::IsTrue(O_2.count() == 9, L"count() failed after reset()", LINE_INFO());
		Assert::IsFalse(O_2.is_answer(), L"is_answer() failed after reset()", LINE_INFO());
		Assert::IsFalse(O_2.is_answer(2), L"is_answer(val_t) failed after reset", LINE_INFO());
		Assert::IsTrue(O_2.is_option(2), L"is_option(val_t) failed after reset", LINE_INFO());

		static_assert(noexcept(O_1.flip()), "flip() should be noexcept");
		O_2.flip();
		Assert::IsTrue(O_2.is_empty(), L"flip() failed", LINE_INFO());

		//bool remove(val_t value);			// remove single option, return if needed
		static_assert(!noexcept(O_1.remove(3)), "remove(val_t) should be noexcept");
		Assert::IsTrue(O_1.remove(3), L"remove(val_t) failed", LINE_INFO());
		Assert::IsTrue(O_1.count() == 8, L"remove(val_t) failed_2", LINE_INFO());
		Assert::IsFalse(O_1.remove(3), L"remove(val_t) on false should return false", LINE_INFO());

		//Options& add(val_t value);			// add single option
		static_assert(!noexcept(O_1.add(4)), "add(val_t) should NOT be noexcept");
		try { O_1.add(4); }
		catch (std::exception&) { Assert::Fail(L"add(val_t) failed"); }
		try { O_1.add(12); }
		catch (const std::out_of_range&) {} // caught
		catch (const std::exception&) { Assert::Fail(L"add(high val) exception wasn't caught"); }

		//Options& set(val_t value);			// set to answer
		static_assert(!noexcept(O_1.set(4)), "set(val_t) should NOT be noexcept");
		Assert::IsTrue(O_1.set(4).is_answer(), L"set(val_t) failed", LINE_INFO());
		Assert::IsTrue(O_1.is_answer(4), L"set(val_t) failed_2", LINE_INFO());
		Assert::IsTrue(O_1.count() == 0, L"count() failed after set(val_t)", LINE_INFO());
		Assert::IsTrue(O_1.set(1).is_answer(1), L"set(val_t) failed when changing", LINE_INFO());
		Assert::IsFalse(O_1.is_answer(4), L"set(val_t) failed to remove value", LINE_INFO());
		Assert::IsTrue(O_1.set(0).is_empty(), L"set(0) should remove all values", LINE_INFO());

		//val_t answer() const noexcept;		// return answer or 0
		static_assert(noexcept(O_1.answer()), "answer() should be noexcept");
		Assert::IsTrue(O_1.set(1).answer() == 1, L"answer() failed", LINE_INFO());
		Assert::IsTrue(O_2.answer() == 0, L"answer() failed_2", LINE_INFO());
		Assert::IsTrue(O_2.set(0).answer() == 0, L"answer() when empty with 0th bit set", LINE_INFO());

		//std::vector<val_t> available() const;	// return available options
		static_assert(!noexcept(O_1.available()), "available() should NOT be noexcept");
		std::vector<val_t> result{};
		try { result = O_4.available(); }
		catch (const std::exception&) { Assert::Fail(L"available() failed", LINE_INFO()); }
		Assert::IsTrue(result.size() == 2, L"available() failed_2", LINE_INFO());
		Assert::IsTrue(result[0] == 1, L"available() failed_3", LINE_INFO());
		Assert::IsTrue(result[1] == 3, L"available() failed_4", LINE_INFO());
		try { result = E_1.available(); }
		catch (const std::exception&) { Assert::Fail(L"available() on empty failed", LINE_INFO()); }
		Assert::IsTrue(result.size() == 0, L"available() when none available", LINE_INFO());
		try { result = E_2.available(); }
		catch (const std::exception&) { Assert::Fail(L"available() on empty failed_2", LINE_INFO()); }
		Assert::IsTrue(result.size() == 0, L"available() when none available_2", LINE_INFO());
	}
	TEST_METHOD(T3_operators)
	{
		// _test-data
		Sudoku::Options<4> O_1{};							// all options
		Sudoku::Options<4> O_2{ std::bitset<5>{"11111"} };	// all options
		Sudoku::Options<4> O_3{ std::bitset<5>{"00101"} };	// single option 2
		Sudoku::Options<4> E_1{ 0 };						// empty answer "00001"
		Sudoku::Options<4> E_2{ std::bitset<5>{"00000"} };	// empty
		Sudoku::Options<4> E_3{ std::bitset<5>{"00001"} };	// empty option
		Sudoku::Options<4> A_1{ 1 };						// answer 1
		Sudoku::Options<4> A_2{ std::bitset<5>{"00100"} };	// answer 2

		//bool operator==(const Options<E>&) const noexcept;
		static_assert(noexcept(O_1 == O_2), "operator== should be noexcept");
		Assert::IsTrue(O_1 == O_2, L"operator== failed", LINE_INFO());
		Assert::IsFalse(O_1 == E_1, L"operator== inverse failed", LINE_INFO());
		//bool operator!=(const Options<E>&) const noexcept;
		static_assert(noexcept(O_1 != O_2), "operator!= should be noexcept");
		Assert::IsTrue(O_1 != E_1, L"operator!= failed", LINE_INFO());
		Assert::IsFalse(O_1 != O_2, L"operator!= failed", LINE_INFO());
		// operator==(val_t)
		Assert::IsTrue(A_1 == 1, L"operator==(val_t) should behave like is_answer(val_t)", LINE_INFO());
		Assert::IsTrue(A_2 == 2, L"operator==(val_t) should behave like is_answer(val_t)_2", LINE_INFO());

		// constructors
		Assert::IsTrue(E_1 == E_3, L"Options{ 0 } the 0th bit is true", LINE_INFO());

		// copy-assign
		static_assert(noexcept(O_1.operator=(O_2)), "operator= should be noexcept");
		Sudoku::Options<4> O_4 = O_3;
		Assert::IsTrue(O_4 == O_3, L"copy-assign failed", LINE_INFO());
		Sudoku::Options<4> O_5 = 2;
		Assert::IsTrue(O_5 == A_2, L"copy-assign operator(val_t) failed", LINE_INFO());
		// move-assign
		static_assert(noexcept(O_1.operator=(std::bitset<5>())), "operator= should be noexcept_2");
		Sudoku::Options<4> O_6 = std::bitset<5>{};	// "00000"
		Assert::IsTrue(O_6 == E_2, L"copy-assign failed", LINE_INFO());

		//constexpr bool operator[](const val_t value) const noexcept;
		static_assert(noexcept(O_1[2]), "operator[val_t] should be no except for reading");
		static_assert(noexcept(O_1[14]), "operator[out of range] should not throw an exception");
		Assert::IsTrue(O_1[2], L"reading with operator[val_t] failed", LINE_INFO());
		Assert::IsTrue(A_2[2], L"reading with operator[val_t] failed_2", LINE_INFO());
		Assert::IsTrue(A_2[1] == false, L"reading with operator[val_t] failed_3", LINE_INFO());

		//Options& operator+=(const Options&) noexcept;
		static_assert(noexcept(O_1 += O_2), "operator+= should be noexcept");
		O_1 += O_2;
		Assert::IsTrue(O_1 == O_2, L"operator+= failed", LINE_INFO());
			// others tested by using operator+()

		//Options operator+(const Options&) const noexcept;
		static_assert(noexcept(O_1 + O_2), "operator+ should be noexcept");
		Assert::IsTrue(O_1 + O_2 == O_1, L"operation+ failed", LINE_INFO());
		Assert::IsTrue(E_1 + O_1 == O_1, L"operation+ failed", LINE_INFO());
		Assert::IsTrue(A_2 + E_3 == O_3, L"operation+= ans+=other failed", LINE_INFO());
		Assert::IsFalse(E_1 + A_2 == O_3, L"operation+= other+=ans failed", LINE_INFO());

		//Options operator-(const Options&) const noexcept;
		static_assert(noexcept(O_1 - O_2), "operator- should be noexcept");

		//Options operator&(const Options&) const noexcept;)
		static_assert(noexcept(O_1 & O_2), "operator& should be noexcept");
		Assert::IsTrue((O_2 & O_3) == O_3, L"operation& failed", LINE_INFO());
	}
};
}	//namespace Sudoku_Test
