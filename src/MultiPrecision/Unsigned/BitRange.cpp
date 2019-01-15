//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::BitRange::ConstIterator::ConstIterator(
	const decltype(Unsigned::digits)& digits,
	std::size_t digitPosition,
	DigitType mask) :
	digits(&digits),
	digitPosition(digitPosition),
	mask(mask)
{
}

Unsigned::BitRange::ConstIterator& Unsigned::BitRange::ConstIterator::operator++()
{
	if (mask == (DigitType(1) << (std::numeric_limits<DigitType>::digits - 1))) {
		++digitPosition;
		mask = 1;
	} else {
		mask <<= 1;
	}
	return *this;
}

bool Unsigned::BitRange::ConstIterator::operator*() const noexcept
{
	return (digits->at(digitPosition) & mask) != 0;
}

bool operator!=(const Unsigned::BitRange::ConstIterator& lhs, const Unsigned::BitRange::ConstIterator& rhs) noexcept
{
	return lhs.digitPosition != rhs.digitPosition || lhs.mask != rhs.mask || lhs.digits != rhs.digits;
}

} // namespace MultiPrecision
