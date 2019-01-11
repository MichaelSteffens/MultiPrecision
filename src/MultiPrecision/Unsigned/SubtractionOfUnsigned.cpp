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

class Unsigned::SubtractionOfUnsigned
{
public:
	SubtractionOfUnsigned(
		const decltype(Unsigned::digits)& minuend,
		const decltype(Unsigned::digits)& subtrahend,
		decltype(Unsigned::digits)& difference) :
		minuend(minuend),
		subtrahend(subtrahend),
		difference(difference),
		minuendLength(minuend.size()),
		subtrahendLength(subtrahend.size())
	{
	}

	bool subtractDigitsAndTestNegative()
	{
		difference.resize(std::max(minuendLength, subtrahendLength));
		DigitPairType borrow = 0;
		std::size_t i = 0;
		while (i < std::min(minuendLength, subtrahendLength)) {
			DigitPairType tmp = minuend[i] - (subtrahend[i] + borrow);
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
		while (i < subtrahendLength) {
			DigitPairType tmp = 0 - (subtrahend[i] + borrow);
			difference[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		return borrow != 0;
	}

private:
	const decltype(Unsigned::digits)& minuend;
	const decltype(Unsigned::digits)& subtrahend;
	decltype(Unsigned::digits)& difference;
	const std::size_t minuendLength;
	const std::size_t subtrahendLength;
};

Unsigned& Unsigned::operator-=(const Unsigned& other)
{
	if (SubtractionOfUnsigned(digits, other.digits, digits).subtractDigitsAndTestNegative()) {
		throw Underflow("Unsigned::operator-=(const Unsigned&): result is negative!");
	}
	normalize();
	return *this;
}

Unsigned operator-(const Unsigned& lhs, const Unsigned& rhs)
{
	Unsigned result;
	if (Unsigned::SubtractionOfUnsigned(lhs.digits, rhs.digits, result.digits).subtractDigitsAndTestNegative()) {
		throw Underflow("operator+(const Unsigned&, const Unsigned& rhs): : result is negative!");
	}
	result.normalize();
	return result;
}

bool Unsigned::subtractAndTestNegative(const Unsigned& other)
{
	return SubtractionOfUnsigned(digits, other.digits, digits).subtractDigitsAndTestNegative();
}

} // namespace MultiPrecision
