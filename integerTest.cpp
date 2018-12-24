//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <iostream>

int main(int argc, char* argv[])
{
	unsigned long x;
	unsigned long y;
	for (x = 0; x <= 0x1ffffffffffff; ++x) {
		for (y = 1; y <= 0x1ffffffff; ++y) {
			MultiPrecision::Unsigned u = x;
			MultiPrecision::Unsigned v = y;
			auto result = u.dividedBy(v);
			if (result.quotient != x / y) {
				std::cout << "Failed: " << x << " / " << y << " == " << x / y << " != " << result.quotient << "  || " << x << " % "
						  << y << " == " << x % y << " != " << result.remainder << std::endl;
			}
		}
	}
	std::cout << "Finished: "
			  << "x=" << x << " y=" << y << std::endl;
	return 0;
}
