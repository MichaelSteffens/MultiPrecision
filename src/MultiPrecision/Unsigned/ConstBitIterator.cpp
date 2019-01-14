//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::ConstBitIterator::ConstBitIterator(const decltype(Unsigned::digits)& digits, std::size_t i, DigitType mask) :
	digits(digits),
	i(i),
	mask(mask)
{
}

Unsigned::ConstBitIterator& Unsigned::ConstBitIterator::operator++()
{
	if (mask == (DigitType(1) << (std::numeric_limits<DigitType>::digits - 1))) {
		++i;
		mask = 1;
	} else {
		mask <<= 1;
	}
	return *this;
}

bool Unsigned::ConstBitIterator::operator*() const noexcept
{
	return (digits[i] & mask) != 0;
}

bool operator!=(const Unsigned::ConstBitIterator& lhs, const Unsigned::ConstBitIterator& rhs) noexcept
{
	return lhs.i != rhs.i || lhs.mask != rhs.mask || &lhs.digits != &rhs.digits;
}

} // namespace MultiPrecision
