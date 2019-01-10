//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

class Unsigned::AdditionOfDigitType
{
public:
	AdditionOfDigitType(const decltype(Unsigned::digits)& lhs, DigitType& rhs, decltype(Unsigned::digits)& result) :
		lhs(lhs),
		rhs(rhs),
		result(result),
		lhsLength(lhs.size())
	{
	}

	void addDigits()
	{
		result.resize(std::max(lhsLength + 1, std::size_t(1)));
		DigitPairType carry = 0;
		std::size_t i = 0;
		while (i < std::min(lhsLength, std::size_t(1))) {
			DigitPairType tmp = DigitPairType(lhs[i]) + rhs;
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < lhsLength) {
			DigitPairType tmp = carry + lhs[i];
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < 1) {
			DigitPairType tmp = carry + (i ? 0 : (i ? 0 : rhs));
			result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		if (carry) {
			result.push_back(carry & std::numeric_limits<Unsigned::DigitType>::max());
		}
	}

private:
	const decltype(Unsigned::digits)& lhs;
	DigitType rhs;
	decltype(Unsigned::digits)& result;
	const std::size_t lhsLength;
};

Unsigned& Unsigned::operator+=(DigitType other)
{
	AdditionOfDigitType(digits, other, digits).addDigits();
	normalize();
	return *this;
}

} // namespace MultiPrecision
