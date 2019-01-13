//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

Unsigned::BitRange::BitRange(const decltype(Unsigned::digits)& digits) : digits(digits)
{
}

Unsigned::BitRange::ConstIterator Unsigned::BitRange::begin() const
{
	DigitType mask;
	if (!digits.empty()) {
		mask = 1;
	} else {
		mask = 0;
	}
	return ConstIterator(digits, 0, mask);
}

Unsigned::BitRange::ConstIterator Unsigned::BitRange::end() const
{
	std::size_t i;
	DigitType mask;
	if (!digits.empty()) {
		i = digits.size();
		mask = 1;
	} else {
		i = 0;
		mask = 0;
	}
	return ConstIterator(digits, i, mask);
}

Unsigned::BitRange::ConstIterator::ConstIterator(const decltype(Unsigned::digits)& digits, std::size_t i, DigitType mask) :
	digits(digits),
	i(i),
	mask(mask)
{
}

Unsigned::BitRange::ConstIterator& Unsigned::BitRange::ConstIterator::operator++()
{
	if (mask == (DigitType(1) << (std::numeric_limits<DigitType>::digits - 1))) {
		++i;
		mask = 1;
	} else {
		mask <<= 1;
	}
	return *this;
}

bool Unsigned::BitRange::ConstIterator::operator*() const noexcept
{
	return (digits[i] & mask) != 0;
}

bool operator!=(const Unsigned::BitRange::ConstIterator& lhs, const Unsigned::BitRange::ConstIterator& rhs) noexcept
{
	return lhs.i != rhs.i || lhs.mask != rhs.mask || &lhs.digits != &rhs.digits;
}

} // namespace MultiPrecision
