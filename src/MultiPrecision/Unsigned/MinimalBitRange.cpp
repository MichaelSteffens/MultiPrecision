//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::MinimalBitRange::MinimalBitRange(const Unsigned& number) : number(&number)
{
}

Unsigned::MinimalBitRange::~MinimalBitRange()
{
}

Unsigned::BitRange::ConstIterator Unsigned::MinimalBitRange::begin() const
{
	std::size_t bitPosition = number->mostSignificantBitPosition();
	DigitType mask;
	if (bitPosition) {
		mask = 1;
	} else {
		mask = 0;
	}
	return ConstIterator(number->digits, 0, mask);
}

Unsigned::BitRange::ConstIterator Unsigned::MinimalBitRange::end() const
{
	std::size_t bitPosition = number->mostSignificantBitPosition();
	std::size_t digitPosition;
	DigitType mask;
	if (bitPosition) {
		digitPosition = bitPosition / std::numeric_limits<DigitType>::digits;
		mask = DigitType(1) << (bitPosition % std::numeric_limits<DigitType>::digits);
	} else {
		digitPosition = 0;
		mask = 0;
	}
	return ConstIterator(number->digits, digitPosition, mask);
}

} // namespace MultiPrecision
