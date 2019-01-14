//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::BitRange::BitRange(const Unsigned& number) : number(number)
{
}

Unsigned::ConstBitIterator Unsigned::BitRange::begin() const
{
	std::size_t bitPosition = number.mostSignificantBitPosition();
	DigitType mask;
	if (bitPosition) {
		mask = 1;
	} else {
		mask = 0;
	}
	return ConstBitIterator(number.digits, 0, mask);
}

Unsigned::ConstBitIterator Unsigned::BitRange::end() const
{
	std::size_t bitPosition = number.mostSignificantBitPosition();
	std::size_t i;
	DigitType mask;
	if (bitPosition) {
		i = bitPosition / std::numeric_limits<DigitType>::digits;
		mask = DigitType(1) << (bitPosition % std::numeric_limits<DigitType>::digits);
	} else {
		i = 0;
		mask = 0;
	}
	return ConstBitIterator(number.digits, i, mask);
}

} // namespace MultiPrecision
