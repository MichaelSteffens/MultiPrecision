//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::FullBitRange::FullBitRange(const Unsigned& number) : number(&number)
{
}

Unsigned::FullBitRange::~FullBitRange()
{
}

Unsigned::BitRange::ConstIterator Unsigned::FullBitRange::begin() const
{
	DigitType mask;
	if (!number->digits.empty()) {
		mask = 1;
	} else {
		mask = 0;
	}
	return ConstIterator(number->digits, 0, mask);
}

Unsigned::BitRange::ConstIterator Unsigned::FullBitRange::end() const
{
	std::size_t digitPosition;
	DigitType mask;
	if (!number->digits.empty()) {
		digitPosition = number->digits.size();
		mask = 1;
	} else {
		digitPosition = 0;
		mask = 0;
	}
	return ConstIterator(number->digits, digitPosition, mask);
}

} // namespace MultiPrecision
