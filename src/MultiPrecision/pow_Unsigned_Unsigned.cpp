//
// Copyright (c) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:      LGPL-2.1-or-later
//


#include "MultiPrecision/math.h"
#include <iostream>

namespace MultiPrecision {

Unsigned pow(const Unsigned& base, const Unsigned& exponent)
{
	Unsigned power(1);
	Unsigned multiplier(base);
	for (auto i : exponent.bitRange()) {
		std::cerr << "pow(const Unsigned&, const Unsigned&): i=" << i << std::endl;
		if (i) {
			power *= multiplier;
		}
		multiplier *= multiplier;
	}
	return power;
}

} // namespace MultiPrecision
