//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <iostream>

int main(int argc, char* argv[])
{
	unsigned long x;
	unsigned long y;
	for (x = 0x10000; x <= 0xffffffffff; ++x) {
		for (y = 0x80ff; y <= 0x80ff; ++y) {
			MultiPrecision::Unsigned u(x);
			MultiPrecision::Unsigned v(y);
			auto result = u.dividedBy(v);
			if (result.quotient != MultiPrecision::Unsigned(x / y) || result.remainder != MultiPrecision::Unsigned(x % y)) {
				std::cout << "Failed: " << x << " / " << y << " == " << x / y << " != " << result.quotient.toDecimalString() << "  || " << x << " % "
						  << y << " == " << x % y << " != " << result.remainder.toDecimalString() << std::endl;
			}
		}
	}
	std::cout << "Finished: "
			  << "x=" << x << " y=" << y << std::endl;
	return 0;
}
