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
	SubtractionOfDigitType(const decltype(Unsigned::digits)& lhs, DigitType& rhs, decltype(Unsigned::digits)& result) :
		lhs(lhs),
		rhs(rhs),
		result(result),
		lhsLength(lhs.size())
	{
	}

	bool subtractDigitAndTestNegative()
	{
		result.resize(std::max(lhsLength, std::size_t(1)));
		DigitPairType borrow = 0;
		std::size_t i = 0;
		while (i < std::min(lhsLength, std::size_t(1))) {
			DigitPairType tmp = static_cast<DigitPairType>(lhs[0]) - rhs;
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
		while (i < 1) {
			DigitPairType tmp = 0 - ((i ? 0 : rhs) + borrow);
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		return borrow != 0;
	}

private:
	const decltype(Unsigned::digits)& lhs;
	DigitType rhs;
	decltype(Unsigned::digits)& result;
	const std::size_t lhsLength;
};

Unsigned& Unsigned::operator-=(DigitType other)
{
	if (SubtractionOfDigitType(digits, other, digits).subtractDigitAndTestNegative()) {
		throw Underflow("Unsigned::operator-=(DigitType): result is negative!");
	}
	normalize();
	return *this;
}

} // namespace MultiPrecision
