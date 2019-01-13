#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "MultiPrecision/Unsigned.h"
#include <sstream>

TEST_CASE("Unsigned addition", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabefeedbabefeedbabe");
	SECTION("Small and large number")
	{
		REQUIRE(
			testee + "0xfffffffffffffffffffffffffffffffffeedbabefeedbabefeedbabefeedbabe" ==
			"0x100000000000000000000000000000000fddb757dfddb757dfddb757dfddb757c");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
	SECTION("Large and small number")
	{
		REQUIRE(testee + "0xfeedbabefeedbabe" == "0xfeedbabefeedbabffddb757dfddb757c");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
	SECTION("Equal size numbers")
	{
		REQUIRE(testee + "0xbabefeedbabefeedbabefeedbabefeed" == "0x1b9acb9acb9acb9acb9acb9acb9acb9ab");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
}

TEST_CASE("Unsigned addition in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabefeedbabefeedbabe");
	SECTION("Small and large number")
	{
		testee += "0xfffffffffffffffffffffffffffffffffeedbabefeedbabefeedbabefeedbabe";
		REQUIRE(testee == "0x100000000000000000000000000000000fddb757dfddb757dfddb757dfddb757c");
	}
	SECTION("Large and small number")
	{
		testee += "0xfeedbabefeedbabe";
		REQUIRE(testee == "0xfeedbabefeedbabffddb757dfddb757c");
	}
	SECTION("Equal size numbers")
	{
		testee += "0xbabefeedbabefeedbabefeedbabefeed";
		REQUIRE(testee == "0x1b9acb9acb9acb9acb9acb9acb9acb9ab");
	}
}

TEST_CASE("Unsigned increment", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabeffffffffffffffff");
	SECTION("Pre-increment")
	{
		REQUIRE(++testee == "0xfeedbabefeedbabf0000000000000000");
		REQUIRE(testee == "0xfeedbabefeedbabf0000000000000000");
	}
	SECTION("Post-increment")
	{
		REQUIRE(testee++ == "0xfeedbabefeedbabeffffffffffffffff");
		REQUIRE(testee == "0xfeedbabefeedbabf0000000000000000");
	}
}

TEST_CASE("Unsigned subtraction", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabefeedbabefeedbabe");
	SECTION("Negative result")
	{
		REQUIRE_THROWS(testee - "0xffffffffbabefeedbabefeedbabefeed");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
	SECTION("Large and small number")
	{
		REQUIRE(testee - "0xffffffffbabefeed" == "0xfeedbabefeedbabdfeedbabf442ebbd1");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
	SECTION("Equal size numbers")
	{
		REQUIRE(testee - "0xbabefeedbabefeedbabefeedbabefeed" == "0x442ebbd1442ebbd1442ebbd1442ebbd1");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
}

TEST_CASE("Unsigned subtraction in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabefeedbabefeedbabe");
	SECTION("Negative result")
	{
		REQUIRE_THROWS(testee -= "0xffffffffbabefeedbabefeedbabefeed");
	}
	SECTION("Large and small number")
	{
		testee -= "0xffffffffbabefeed";
		REQUIRE(testee == "0xfeedbabefeedbabdfeedbabf442ebbd1");
	}
	SECTION("Equal size numbers")
	{
		testee -= "0xbabefeedbabefeedbabefeedbabefeed";
		REQUIRE(testee == "0x442ebbd1442ebbd1442ebbd1442ebbd1");
	}
}

TEST_CASE("Unsigned decrement", "[unsigned]")
{
	MultiPrecision::Unsigned zero("0x0");
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabe0000000000000000");
	SECTION("Pre-decrement zero")
	{
		REQUIRE_THROWS(--zero);
	}
	SECTION("Pre-decrement")
	{
		REQUIRE(--testee == "0xfeedbabefeedbabdffffffffffffffff");
		REQUIRE(testee == "0xfeedbabefeedbabdffffffffffffffff");
	}
	SECTION("Post-decrement zero")
	{
		REQUIRE_THROWS(zero--);
	}
	SECTION("Post-decrement")
	{
		REQUIRE(testee-- == "0xfeedbabefeedbabe0000000000000000");
		REQUIRE(testee == "0xfeedbabefeedbabdffffffffffffffff");
	}
}

TEST_CASE("Unsigned multiplication", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabefeedbabefeedbabe");
	SECTION("Small and large number")
	{
		REQUIRE(
			testee * "0xbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeed" ==
			"0xb9f6ebfbfa503ddd3aa98fbe7b02e19f01654785016547850165478501654784476e5b89071509a7c6bbb7c6866265e6");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
	SECTION("Large and small number")
	{
		REQUIRE(testee * "0xbabefeedbabefeed" == "0xb9f6ebfbfa503ddc80b2a3c280b2a3c1c6bbb7c6866265e6");
		REQUIRE(testee == "0xfeedbabefeedbabefeedbabefeedbabe");
	}
}

TEST_CASE("Unsigned multiplication in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabefeedbabefeedbabe");
	SECTION("Small and large number")
	{
		testee *= "0xbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeed";
		REQUIRE(testee == "0xb9f6ebfbfa503ddd3aa98fbe7b02e19f01654785016547850165478501654784476e5b89071509a7c6bbb7c6866265e6");
	}
	SECTION("Large and small number")
	{
		testee *= "0xbabefeedbabefeed";
		REQUIRE(testee == "0xb9f6ebfbfa503ddc80b2a3c280b2a3c1c6bbb7c6866265e6");
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
		auto result = dividend.dividedBy("0xfe");
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

TEST_CASE("Unsigned division need reduction by one", "[unsigned]")
{
	SECTION("DigitType 8 bits")
	{
		auto result = MultiPrecision::Unsigned("0x10000").dividedBy(MultiPrecision::Unsigned("0x80ff"));
		REQUIRE(result.quotient == "0x1");
		REQUIRE(result.remainder == "0x7f01");
	}
	SECTION("DigitType 16 bits")
	{
		auto result = MultiPrecision::Unsigned("0x100000000").dividedBy(MultiPrecision::Unsigned("0x8000ffff"));
		REQUIRE(result.quotient == "0x1");
		REQUIRE(result.remainder == "0x7fff0001");
	}
	SECTION("DigitType 32 bits")
	{
		auto result = MultiPrecision::Unsigned("0x10000000000000000").dividedBy(MultiPrecision::Unsigned("0x80000000ffffffff"));
		REQUIRE(result.quotient == "0x1");
		REQUIRE(result.remainder == "0x7fffffff00000001");
	}
	SECTION("DigitType 64 bits")
	{
		auto result = MultiPrecision::Unsigned("0x100000000000000000000000000000000")
						  .dividedBy(MultiPrecision::Unsigned("0x8000000000000000ffffffffffffffff"));
		REQUIRE(result.quotient == "0x1");
		REQUIRE(result.remainder == "0x7fffffffffffffff0000000000000001");
	}
}

TEST_CASE("Unsigned division need reduction by two", "[unsigned]")
{
	SECTION("DigitType 8 bits")
	{
		auto result = MultiPrecision::Unsigned("0xff0000").dividedBy(MultiPrecision::Unsigned("0x80ff"));
		REQUIRE(result.quotient == "0x1fa");
		REQUIRE(result.remainder == "0x7fa");
	}
	SECTION("DigitType 16 bits")
	{
		auto result = MultiPrecision::Unsigned("0xffff00000000").dividedBy(MultiPrecision::Unsigned("0x8000ffff"));
		REQUIRE(result.quotient == "0x1fffa");
		REQUIRE(result.remainder == "0x7fffa");
	}
	SECTION("DigitType 32 bits")
	{
		auto result =
			MultiPrecision::Unsigned("0xffffffff0000000000000000").dividedBy(MultiPrecision::Unsigned("0x80000000ffffffff"));
		REQUIRE(result.quotient == "0x1fffffffa");
		REQUIRE(result.remainder == "0x7fffffffa");
	}
	SECTION("DigitType 64 bits")
	{
		auto result = MultiPrecision::Unsigned("0xffffffffffffffff00000000000000000000000000000000")
						  .dividedBy(MultiPrecision::Unsigned("0x8000000000000000ffffffffffffffff"));
		REQUIRE(result.quotient == "0x1fffffffffffffffa");
		REQUIRE(result.remainder == "0x7fffffffffffffffa");
	}
}

TEST_CASE("Unsigned division need borrow", "[unsigned]")
{
	auto result = MultiPrecision::Unsigned("0x200000000000000000000000000000000")
					  .dividedBy(MultiPrecision::Unsigned("0x100000000000000000000000000000001"));
	REQUIRE(result.quotient == "0x1");
	REQUIRE(result.remainder == "0xffffffffffffffffffffffffffffffff");
}

TEST_CASE("Unsigned division in place operator", "[unsigned]")
{
	MultiPrecision::Unsigned testee(
		"0xfeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabe");
	SECTION("Unsigned divisor")
	{
		testee /= "0xfeedbabefeedbabefeedbabefeedbabefeed";
		REQUIRE(testee == "0x1000000000000000000000000000000000000bb87e9207fa87f2165735c6d");
	}
	SECTION("DigitType divisor")
	{
		testee /= "0xfe";
		REQUIRE(testee == "0x100ef99f2e4b7291121301af4e8bf393161b11cf8f0cf5971e2b3210100ef99f2e4b7291121301af4e8bf393161b11c");
	}
}

TEST_CASE("Unsigned modulo in place operator", "[unsigned]")
{
	MultiPrecision::Unsigned testee(
		"0xfeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabefeedbabe");
	SECTION("Unsigned divisor")
	{
		testee %= "0xfeedbabefeedbabefeedbabefeedbabefeed";
		REQUIRE(testee == "0x2c51351a2c51bde6d550d6e7127cd28703d5");
	}
	SECTION("DigitType divisor")
	{
		testee %= "0xfe";
		REQUIRE(testee == "0xf6");
	}
}

TEST_CASE("Unsigned shift left", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("Shift by 1")
	{
		REQUIRE("0x1fddb757c" == testee << 1);
		REQUIRE(testee == "0xfeedbabe");
	}
	SECTION("Shift by 38")
	{
		REQUIRE("0x3fbb6eaf8000000000" == testee << 38);
		REQUIRE(testee == "0xfeedbabe");
	}
	SECTION("Shift by 78")
	{
		REQUIRE("0x3fbb6eaf80000000000000000000" == testee << 78);
		REQUIRE(testee == "0xfeedbabe");
	}
	SECTION("Shift by 112")
	{
		REQUIRE("0xfeedbabe0000000000000000000000000000" == testee << 112);
		REQUIRE(testee == "0xfeedbabe");
	}
}

TEST_CASE("Unsigned shift left in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe");
	SECTION("Shift by 1")
	{
		testee <<= 1;
		REQUIRE(testee == "0x1fddb757c");
	}
	SECTION("Shift by 38")
	{
		testee <<= 38;
		REQUIRE(testee == "0x3fbb6eaf8000000000");
	}
	SECTION("Shift by 78")
	{
		testee <<= 78;
		REQUIRE(testee == "0x3fbb6eaf80000000000000000000");
	}
	SECTION("Shift by 112")
	{
		testee <<= 112;
		REQUIRE(testee == "0xfeedbabe0000000000000000000000000000");
	}
}

TEST_CASE("Unsigned shift right", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe0000000000000000000000000000");
	SECTION("Shift by 2")
	{
		REQUIRE("0x3fbb6eaf8000000000000000000000000000" == testee >> 2);
		REQUIRE(testee == "0xfeedbabe0000000000000000000000000000");
	}
	SECTION("Shift by 42")
	{
		REQUIRE("0x3fbb6eaf800000000000000000" == testee >> 42);
		REQUIRE(testee == "0xfeedbabe0000000000000000000000000000");
	}
	SECTION("Shift by 79")
	{
		REQUIRE("0x1fddb757c00000000" == testee >> 79);
		REQUIRE(testee == "0xfeedbabe0000000000000000000000000000");
	}
	SECTION("Shift by 112")
	{
		REQUIRE("0xfeedbabe" == testee >> 112);
		REQUIRE(testee == "0xfeedbabe0000000000000000000000000000");
	}
}

TEST_CASE("Unsigned shift right in place", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabe0000000000000000000000000000");
	SECTION("Shift by 2")
	{
		testee >>= 2;
		REQUIRE(testee == "0x3fbb6eaf8000000000000000000000000000");
	}
	SECTION("Shift by 42")
	{
		testee >>= 42;
		REQUIRE(testee == "0x3fbb6eaf800000000000000000");
	}
	SECTION("Shift by 79")
	{
		testee >>= 79;
		REQUIRE(testee == "0x1fddb757c00000000");
	}
	SECTION("Shift by 112")
	{
		testee >>= 112;
		REQUIRE(testee == "0xfeedbabe");
	}
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
		std::string number("1234567890123456789012345678901234567890");
		REQUIRE(MultiPrecision::Unsigned::fromDecimal(number.begin(), number.end()) == "1234567890123456789012345678901234567890");
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
		std::string number("feedbabefeedbabefeedbabefeedbabe");
		REQUIRE(MultiPrecision::Unsigned::fromHexadecimal(number.begin(), number.end()) == "0xfeedbabefeedbabefeedbabefeedbabe");
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
		std::string number("12345670123456701234567012345670");
		REQUIRE(MultiPrecision::Unsigned::fromOctal(number.begin(), number.end()) == "012345670123456701234567012345670");
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
		testee = "1234567890123456789012345678901234567890";
		REQUIRE(testee.toDecimalString() == "1234567890123456789012345678901234567890");
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
		testee = "0xfeedbabefeedbabefeedbabefeedbabe";
		REQUIRE(testee.toHexadecimalString() == "feedbabefeedbabefeedbabefeedbabe");
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
		testee = "012345670123456701234567012345670";
		REQUIRE(testee.toOctalString() == "12345670123456701234567012345670");
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

TEST_CASE("Bit range", "[unsigned]")
{
	MultiPrecision::Unsigned testee("0xfeedbabefeedbabe");
	std::ostringstream out;
	for (auto i : testee.bitRange()) {
		out << i;
	}
	REQUIRE(out.str() == "0111110101011101101101110111111101111101010111011011011101111111");
}
