#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "MultiPrecision/Unsigned.h"
#include <sstream>

TEST_CASE("Unsigned shift left", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("Shift by 1")
	{
		REQUIRE(testee.shiftedLeftBy(1) == "0x1fddb757c");
		REQUIRE("0xfeedbabe" == testee);
	}
	SECTION("Shift by 6")
	{
		REQUIRE(testee.shiftedLeftBy(6) == "0x3fbb6eaf80");
		REQUIRE(testee == "0xfeedbabe");
	}
	SECTION("Shift by 14")
	{
		REQUIRE(testee.shiftedLeftBy(14) == "0x3fbb6eaf8000");
		REQUIRE(testee == "0xfeedbabe");
	}
	SECTION("Shift by 16")
	{
		REQUIRE(testee.shiftedLeftBy(16) == "0xfeedbabe0000");
		REQUIRE(testee == "0xfeedbabe");
	}
}

TEST_CASE("Unsigned shift left in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("Shift by 1")
	{
		testee.shiftLeftBy(1);
		REQUIRE(testee == "0x1fddb757c");
	}
	SECTION("Shift by 6")
	{
		testee.shiftLeftBy(6);
		REQUIRE(testee == "0x3fbb6eaf80");
	}
	SECTION("Shift by 14")
	{
		testee.shiftLeftBy(14);
		REQUIRE(testee == "0x3fbb6eaf8000");
	}
	SECTION("Shift by 16")
	{
		testee.shiftLeftBy(16);
		REQUIRE(testee == "0xfeedbabe0000");
	}
}

TEST_CASE("Unsigned shift right", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe0000");
	SECTION("Shift by 2")
	{
		REQUIRE(testee.shiftedRightBy(2) == "0x3fbb6eaf8000");
		REQUIRE(testee == "0xfeedbabe0000");
	}
	SECTION("Shift by 10")
	{
		REQUIRE(testee.shiftedRightBy(10) == "0x3fbb6eaf80");
		REQUIRE(testee == "0xfeedbabe0000");
	}
	SECTION("Shift by 15")
	{
		REQUIRE(testee.shiftedRightBy(15) == "0x1fddb757c");
		REQUIRE(testee == "0xfeedbabe0000");
	}
	SECTION("Shift by 16")
	{
		REQUIRE(testee.shiftedRightBy(16) == "0xfeedbabe");
		REQUIRE(testee == "0xfeedbabe0000");
	}
}

TEST_CASE("Unsigned shift right in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe0000");
	SECTION("Shift by 2")
	{
		testee.shiftRightBy(2);
		REQUIRE(testee == "0x3fbb6eaf8000");
	}
	SECTION("Shift by 10")
	{
		testee.shiftRightBy(10);
		REQUIRE(testee == "0x3fbb6eaf80");
	}
	SECTION("Shift by 15")
	{
		testee.shiftRightBy(15);
		REQUIRE(testee == "0x1fddb757c");
	}
	SECTION("Shift by 16")
	{
		testee.shiftRightBy(16);
		REQUIRE(testee == "0xfeedbabe");
	}
}

TEST_CASE("Unsigned division", "[unsigned]")
{
	MultiPrecision::Unsigned dividend(
		"0xfeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabe");
	SECTION("Unsigned divisor")
	{
		auto result = dividend.dividedBy("0xfeedbabefeedbabefeedbabefeedbabefeed");
		REQUIRE(result.quotient == "0x1000000000000000000000000000000000000bb87e9207fa87f2165735c6d");
		REQUIRE(result.remainder == "0x2c51351a2c51bde6d550d6e7127cd28703d5");
	}
	SECTION("DigitType divisor")
	{
		auto result = dividend.dividedBy(MultiPrecision::Unsigned("0xfe"));
		std::cerr << result.quotient.toHexadecimalString() << std::endl;
		std::cerr << result.remainder.toHexadecimalString() << std::endl;
		REQUIRE(
			result.quotient == "0x100ef99f2e4b7291121301af4e8bf393161b11cf8f0cf5971e2b3210100ef99f2e4b7291121301af4e8bf393161b11c");
		REQUIRE(result.remainder == "0xf6");
	}
}

TEST_CASE("Unsigned division by divisor larger than dividend", "[unsigned]")
{
	MultiPrecision::Unsigned dividend("0x2");
	SECTION("Unsigned divisor")
	{
		auto result = dividend.dividedBy(MultiPrecision::Unsigned("0x10000000000000001"));
		REQUIRE(result.quotient == "0x0");
		REQUIRE(result.remainder == "0x2");
	}
	SECTION("DigitType divisor")
	{
		auto result = dividend.dividedBy(MultiPrecision::Unsigned("0x11"));
		REQUIRE(result.quotient == "0x0");
		REQUIRE(result.remainder == "0x2");
	}
}

TEST_CASE("Unsigned division need borrow", "[unsigned]")
{
	auto result = MultiPrecision::Unsigned("0x200000000000000000000000000000000")
					  .dividedBy(MultiPrecision::Unsigned("0x100000000000000000000000000000001"));
	REQUIRE(result.quotient == "0x1");
	REQUIRE(result.remainder == "0xffffffffffffffffffffffffffffffff");
}

TEST_CASE("Convert decimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		std::string zero("0");
		REQUIRE(MultiPrecision::Unsigned::fromDecimal(zero.begin(), zero.end()) == "0");
	}
	SECTION("Large number")
	{
		std::string number("12345678901234567890");
		REQUIRE(MultiPrecision::Unsigned::fromDecimal(number.begin(), number.end()) == "12345678901234567890");
	}
}

TEST_CASE("Convert hexadecimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		std::string zero("0");
		REQUIRE(MultiPrecision::Unsigned::fromHexadecimal(zero.begin(), zero.end()) == "0x0");
	}
	SECTION("Large number")
	{
		std::string number("feedbabefeedbabe");
		REQUIRE(MultiPrecision::Unsigned::fromHexadecimal(number.begin(), number.end()) == "0xfeedbabefeedbabe");
	}
}

TEST_CASE("Convert octal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		std::string zero("0");
		REQUIRE(MultiPrecision::Unsigned::fromOctal(zero.begin(), zero.end()) == "0");
	}
	SECTION("Large number")
	{
		std::string number("1234567012345670");
		REQUIRE(MultiPrecision::Unsigned::fromOctal(number.begin(), number.end()) == "01234567012345670");
	}
}

TEST_CASE("Convert to decimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		testee = "0";
		REQUIRE(testee.toDecimalString() == "0");
	}
	SECTION("Large number")
	{
		testee = "12345678901234567890";
		REQUIRE(testee.toDecimalString() == "12345678901234567890");
	}
}

TEST_CASE("Convert to hexadecimal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		testee = "0";
		REQUIRE(testee.toHexadecimalString() == "0");
	}
	SECTION("Large number")
	{
		testee = "0xfeedbabefeedbabe";
		REQUIRE(testee.toHexadecimalString() == "feedbabefeedbabe");
	}
}

TEST_CASE("Convert to octal string", "[unsigned]")
{
	MultiPrecision::Unsigned testee;
	SECTION("Zero")
	{
		testee = "0";
		REQUIRE(testee.toOctalString() == "0");
	}
	SECTION("Large number")
	{
		testee = "01234567012345670";
		REQUIRE(testee.toOctalString() == "1234567012345670");
	}
}

TEST_CASE("Operator ==", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("LHS equal")
	{
		REQUIRE(testee == "0xfeedbabe");
	}
	SECTION("LHS not equal LHS")
	{
		REQUIRE_FALSE(testee == "0xbabefeed");
	}
	SECTION("Equal RHS")
	{
		REQUIRE("0xfeedbabe" == testee);
	}
	SECTION("Not equal RHS")
	{
		REQUIRE_FALSE("0xbabefeed" == testee);
	}
}

TEST_CASE("Operator !=", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("LHS equal")
	{
		REQUIRE_FALSE(testee != "0xfeedbabe");
	}
	SECTION("LHS not equal")
	{
		REQUIRE(testee != "0xbabefeed");
	}
	SECTION("Equal RHS")
	{
		REQUIRE_FALSE("0xfeedbabe" != testee);
	}
	SECTION("Not equal RHS")
	{
		REQUIRE("0xbabefeed" != testee);
	}
}

TEST_CASE("Operator <", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("LHS less than")
	{
		REQUIRE(testee < "0xfeedbabf");
	}
	SECTION("LHS equal")
	{
		REQUIRE_FALSE(testee < "0xfeedbabe");
	}
	SECTION("LHS greater than")
	{
		REQUIRE_FALSE(testee < "0xfeedbabd");
	}
	SECTION("Less than RHS")
	{
		REQUIRE("0xfeedbabd" < testee);
	}
	SECTION("Equal RHS")
	{
		REQUIRE_FALSE("0xfeedbabe" < testee);
	}
	SECTION("Greater than RHS")
	{
		REQUIRE_FALSE("0xfeedbabf" < testee);
	}
}

TEST_CASE("Operator <=", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("LHS less than")
	{
		REQUIRE(testee <= "0xfeedbabf");
	}
	SECTION("LHS equal")
	{
		REQUIRE(testee <= "0xfeedbabe");
	}
	SECTION("LHS greater than")
	{
		REQUIRE_FALSE(testee <= "0xfeedbabd");
	}
	SECTION("Less than RHS")
	{
		REQUIRE("0xfeedbabd" <= testee);
	}
	SECTION("Equal RHS")
	{
		REQUIRE("0xfeedbabe" <= testee);
	}
	SECTION("Greater than RHS")
	{
		REQUIRE_FALSE("0xfeedbabf" <= testee);
	}
}

TEST_CASE("Operator >", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("LHS less than")
	{
		REQUIRE_FALSE(testee > "0xfeedbabf");
	}
	SECTION("LHS equal")
	{
		REQUIRE_FALSE(testee > "0xfeedbabe");
	}
	SECTION("LHS greater than")
	{
		REQUIRE(testee > "0xfeedbabd");
	}
	SECTION("Less than RHS")
	{
		REQUIRE_FALSE("0xfeedbabd" > testee);
	}
	SECTION("Equal RHS")
	{
		REQUIRE_FALSE("0xfeedbabe" > testee);
	}
	SECTION("Greater than RHS")
	{
		REQUIRE("0xfeedbabf" > testee);
	}
}

TEST_CASE("Operator >=", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("LHS less than")
	{
		REQUIRE_FALSE(testee >= "0xfeedbabf");
	}
	SECTION("LHS equal")
	{
		REQUIRE(testee >= "0xfeedbabe");
	}
	SECTION("LHS greater than")
	{
		REQUIRE(testee >= "0xfeedbabd");
	}
	SECTION("Less than RHS")
	{
		REQUIRE_FALSE("0xfeedbabd" >= testee);
	}
	SECTION("Equal RHS")
	{
		REQUIRE("0xfeedbabe" >= testee);
	}
	SECTION("Greater than RHS")
	{
		REQUIRE("0xfeedbabf" >= testee);
	}
}
