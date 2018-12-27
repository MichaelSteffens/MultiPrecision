#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "MultiPrecision/Unsigned.h"
#include <sstream>

TEST_CASE("Unsigned shift left", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabeULL);

	SECTION("Shift by 1")
	{
		REQUIRE(testee.shiftedLeftBy(1) == 0x1fddb757cULL);
		REQUIRE(testee == 0xfeedbabeULL);
	}
	SECTION("Shift by 6")
	{
		REQUIRE(testee.shiftedLeftBy(6) == 0x3fbb6eaf80ULL);
		REQUIRE(testee == 0xfeedbabeULL);
	}
	SECTION("Shift by 14")
	{
		REQUIRE(testee.shiftedLeftBy(14) == 0x3fbb6eaf8000ULL);
		REQUIRE(testee == 0xfeedbabeULL);
	}
	SECTION("Shift by 16")
	{
		REQUIRE(testee.shiftedLeftBy(16) == 0xfeedbabe0000ULL);
		REQUIRE(testee == 0xfeedbabeULL);
	}
}

TEST_CASE("Unsigned shift left in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabeULL);

	SECTION("Shift by 1")
	{
		testee.shiftLeftBy(1);
		REQUIRE(testee == 0x1fddb757cULL);
	}
	SECTION("Shift by 6")
	{
		testee.shiftLeftBy(6);
		REQUIRE(testee == 0x3fbb6eaf80ULL);
	}
	SECTION("Shift by 14")
	{
		testee.shiftLeftBy(14);
		REQUIRE(testee == 0x3fbb6eaf8000ULL);
	}
	SECTION("Shift by 16")
	{
		testee.shiftLeftBy(16);
		REQUIRE(testee == 0xfeedbabe0000ULL);
	}
}

TEST_CASE("Unsigned shift right", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabe0000ULL);

	SECTION("Shift by 2")
	{
		REQUIRE(testee.shiftedRightBy(2) == 0x3fbb6eaf8000ULL);
		REQUIRE(testee == 0xfeedbabe0000ULL);
	}
	SECTION("Shift by 10")
	{
		REQUIRE(testee.shiftedRightBy(10) == 0x3fbb6eaf80ULL);
		REQUIRE(testee == 0xfeedbabe0000ULL);
	}
	SECTION("Shift by 15")
	{
		REQUIRE(testee.shiftedRightBy(15) == 0x1fddb757cULL);
		REQUIRE(testee == 0xfeedbabe0000ULL);
	}
	SECTION("Shift by 16")
	{
		REQUIRE(testee.shiftedRightBy(16) == 0xfeedbabeULL);
		REQUIRE(testee == 0xfeedbabe0000ULL);
	}
}

TEST_CASE("Unsigned shift right in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee(0xfeedbabe0000ULL);

	SECTION("Shift by 2")
	{
		testee.shiftRightBy(2);
		REQUIRE(testee == 0x3fbb6eaf8000ULL);
	}
	SECTION("Shift by 10")
	{
		testee.shiftRightBy(10);
		REQUIRE(testee == 0x3fbb6eaf80ULL);
	}
	SECTION("Shift by 15")
	{
		testee.shiftRightBy(15);
		REQUIRE(testee == 0x1fddb757cULL);
	}
	SECTION("Shift by 16")
	{
		testee.shiftRightBy(16);
		REQUIRE(testee == 0xfeedbabeULL);
	}
}

TEST_CASE("Unsigned division need borrow", "[unsigned]")
{
	auto result = MultiPrecision::Unsigned(0x200000000ULL).dividedBy(MultiPrecision::Unsigned(0x100000001ULL));
	REQUIRE(result.quotient == 0x1ULL);
	REQUIRE(result.remainder == 0xffffffffULL);
}

TEST_CASE("Convert decimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		std::string zero("0");
		testee.fromDecimal(zero.begin(), zero.end());
		REQUIRE(testee == 0ULL);
	}
	SECTION("Large number")
	{
		std::string number("12345678901234567890");
		testee.fromDecimal(number.begin(), number.end());
		REQUIRE(testee == 12345678901234567890ULL);
	}
}

TEST_CASE("Convert hexadecimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		std::string zero("0");
		testee.fromHexadecimal(zero.begin(), zero.end());
		REQUIRE(testee == 0x0ULL);
	}
	SECTION("Large number")
	{
		std::string number("feedbabefeedbabe");
		testee.fromHexadecimal(number.begin(), number.end());
		REQUIRE(testee == 0xfeedbabefeedbabeULL);
	}
}

TEST_CASE("Convert octal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		std::string zero("0");
		testee.fromOctal(zero.begin(), zero.end());
		REQUIRE(testee == 0ULL);
	}
	SECTION("Large number")
	{
		std::string number("1234567012345670");
		testee.fromOctal(number.begin(), number.end());
		REQUIRE(testee == 01234567012345670ULL);
	}
}

TEST_CASE("Convert to decimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		testee = 0ULL;
		REQUIRE(testee.toDecimalString() == "0");
	}
	SECTION("Large number")
	{
		testee = 12345678901234567890ULL;
		REQUIRE(testee.toDecimalString() == "12345678901234567890");
	}
}

TEST_CASE("Convert to hexadecimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		testee = 0ULL;
		REQUIRE(testee.toHexadecimalString() == "0");
	}
	SECTION("Large number")
	{
		testee = 0xfeedbabefeedbabeULL;
		REQUIRE(testee.toHexadecimalString() == "feedbabefeedbabe");
	}
}

TEST_CASE("Convert to octal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		testee = 0ULL;
		REQUIRE(testee.toOctalString() == "0");
	}
	SECTION("Large number")
	{
		testee = 01234567012345670ULL;
		REQUIRE(testee.toOctalString() == "1234567012345670");
	}
}
