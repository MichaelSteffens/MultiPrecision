#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "MultiPrecision/Unsigned.h"
#include <sstream>

TEST_CASE("Unsigned shift left", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabeUL);

	SECTION("Shift by 1")
	{
		REQUIRE(testee.shiftedLeftBy(1) == 0x1fddb757cUL);
		REQUIRE(testee == 0xfeedbabeUL);
	}
	SECTION("Shift by 6")
	{
		REQUIRE(testee.shiftedLeftBy(6) == 0x3fbb6eaf80UL);
		REQUIRE(testee == 0xfeedbabeUL);
	}
	SECTION("Shift by 14")
	{
		REQUIRE(testee.shiftedLeftBy(14) == 0x3fbb6eaf8000UL);
		REQUIRE(testee == 0xfeedbabeUL);
	}
	SECTION("Shift by 16")
	{
		REQUIRE(testee.shiftedLeftBy(16) == 0xfeedbabe0000UL);
		REQUIRE(testee == 0xfeedbabeUL);
	}
}

TEST_CASE("Unsigned shift left in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabeUL);

	SECTION("Shift by 1")
	{
		testee.shiftLeftBy(1);
		REQUIRE(testee == 0x1fddb757cUL);
	}
	SECTION("Shift by 6")
	{
		testee.shiftLeftBy(6);
		REQUIRE(testee == 0x3fbb6eaf80UL);
	}
	SECTION("Shift by 14")
	{
		testee.shiftLeftBy(14);
		REQUIRE(testee == 0x3fbb6eaf8000UL);
	}
	SECTION("Shift by 16")
	{
		testee.shiftLeftBy(16);
		REQUIRE(testee == 0xfeedbabe0000UL);
	}
}

TEST_CASE("Unsigned shift right", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabe0000UL);

	SECTION("Shift by 2")
	{
		REQUIRE(testee.shiftedRightBy(2) == 0x3fbb6eaf8000UL);
		REQUIRE(testee == 0xfeedbabe0000UL);
	}
	SECTION("Shift by 10")
	{
		REQUIRE(testee.shiftedRightBy(10) == 0x3fbb6eaf80UL);
		REQUIRE(testee == 0xfeedbabe0000UL);
	}
	SECTION("Shift by 15")
	{
		REQUIRE(testee.shiftedRightBy(15) == 0x1fddb757cUL);
		REQUIRE(testee == 0xfeedbabe0000UL);
	}
	SECTION("Shift by 16")
	{
		REQUIRE(testee.shiftedRightBy(16) == 0xfeedbabeUL);
		REQUIRE(testee == 0xfeedbabe0000UL);
	}
}

TEST_CASE("Unsigned shift right in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabe0000UL);

	SECTION("Shift by 2")
	{
		testee.shiftRightBy(2);
		REQUIRE(testee == 0x3fbb6eaf8000UL);
	}
	SECTION("Shift by 10")
	{
		testee.shiftRightBy(10);
		REQUIRE(testee == 0x3fbb6eaf80UL);
	}
	SECTION("Shift by 15")
	{
		testee.shiftRightBy(15);
		REQUIRE(testee == 0x1fddb757cUL);
	}
	SECTION("Shift by 16")
	{
		testee.shiftRightBy(16);
		REQUIRE(testee == 0xfeedbabeUL);
	}
}

TEST_CASE("Unsigned division need borrow", "[unsigned]")
{
	auto result = MultiPrecision::Unsigned(0x200000000UL).dividedBy(MultiPrecision::Unsigned(0x100000001UL));
	REQUIRE(result.quotient == 0x1UL);
	REQUIRE(result.remainder == 0xffffffffUL);
}
