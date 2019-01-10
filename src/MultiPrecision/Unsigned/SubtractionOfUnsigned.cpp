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
		const decltype(Unsigned::digits)& lhs,
		const decltype(Unsigned::digits)& rhs,
		decltype(Unsigned::digits)& result) :
		lhs(lhs),
		rhs(rhs),
		result(result),
		lhsLength(lhs.size()),
		rhsLength(rhs.size())
	{
	}

	bool subtractDigitsAndTestNegative()
	{
		result.resize(std::max(lhsLength, rhsLength));
		DigitPairType borrow = 0;
		std::size_t i = 0;
		while (i < std::min(lhsLength, rhsLength)) {
			DigitPairType tmp = lhs[i] - (rhs[i] + borrow);
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < lhsLength) {
			DigitPairType tmp = lhs[i] - borrow;
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < rhsLength) {
			DigitPairType tmp = 0 - (rhs[i] + borrow);
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		return borrow != 0;
	}

private:
	const decltype(Unsigned::digits)& lhs;
	const decltype(Unsigned::digits)& rhs;
	decltype(Unsigned::digits)& result;
	const std::size_t lhsLength;
	const std::size_t rhsLength;
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
