//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::FullBitRange::FullBitRange(const Unsigned& number) : number(number)
{
}

Unsigned::ConstBitIterator Unsigned::FullBitRange::begin() const
{
	DigitType mask;
	if (!number.digits.empty()) {
		mask = 1;
	} else {
		mask = 0;
	}
	return ConstBitIterator(number.digits, 0, mask);
}

Unsigned::ConstBitIterator Unsigned::FullBitRange::end() const
{
	std::size_t i;
	DigitType mask;
	if (!number.digits.empty()) {
		i = number.digits.size();
		mask = 1;
	} else {
		i = 0;
		mask = 0;
	}
	return ConstBitIterator(number.digits, i, mask);
}

} // namespace MultiPrecision
