//
// Copyright (c) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:      LGPL-2.1-or-later
//


#include "MultiPrecision/math.h"

namespace MultiPrecision {

Unsigned pow(const Unsigned& base, const Unsigned& exponent, const Unsigned& modulus)
{
	Unsigned power(1);
	Unsigned multiplier(base);
	for (auto i : exponent.bitRange()) {
		if (i) {
			power = power * multiplier % modulus;
		}
		multiplier = multiplier * multiplier % modulus;
	}
	return power;
}

} // namespace MultiPrecision
