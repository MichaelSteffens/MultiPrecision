//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/Underflow.h"
#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

class Unsigned::SubtractionOfDigitType
{
public:
	SubtractionOfDigitType(
		const decltype(Unsigned::digits)& minuend,
		DigitType subtrahend,
		decltype(Unsigned::digits)& difference) :
		minuend(minuend),
		subtrahend(subtrahend),
		difference(difference),
		minuendLength(minuend.size())
	{
	}

	bool subtractDigitAndTestNegative()
	{
		difference.resize(std::max(minuendLength, std::size_t(1)));
		DigitPairType borrow = 0;
		std::size_t i = 0;
		while (i < std::min(minuendLength, std::size_t(1))) {
			DigitPairType tmp = static_cast<DigitPairType>(minuend[0]) - subtrahend;
			difference[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < minuendLength) {
			DigitPairType tmp = minuend[i] - borrow;
			difference[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < 1) {
			DigitPairType tmp = 0 - ((i ? 0 : subtrahend) + borrow);
			difference[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		return borrow != 0;
	}

private:
	const decltype(Unsigned::digits)& minuend;
	const DigitType subtrahend;
	decltype(Unsigned::digits)& difference;
	const std::size_t minuendLength;
};

Unsigned& Unsigned::operator-=(DigitType other)
{
	if (SubtractionOfDigitType(digits, other, digits).subtractDigitAndTestNegative()) {
		throw Underflow("Unsigned::operator-=(DigitType): result is negative!");
	}
	return *this;
}

Unsigned& Unsigned::operator--()
{
	if (SubtractionOfDigitType(digits, 1, digits).subtractDigitAndTestNegative()) {
		throw Underflow("Unsigned::operator--(): result is negative!");
	}
	return *this;
}

Unsigned Unsigned::operator--(int)
{
	Unsigned result(*this);
	if (SubtractionOfDigitType(digits, 1, digits).subtractDigitAndTestNegative()) {
		throw Underflow("Unsigned::operator--(int): result is negative!");
	}
	return result;
}

} // namespace MultiPrecision
