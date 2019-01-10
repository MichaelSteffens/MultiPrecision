//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

class Unsigned::AdditionOfUnsigned
{
public:
	AdditionOfUnsigned(
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

	void addDigits()
	{
		result.resize(std::max(lhsLength, rhsLength));
		DigitPairType carry = 0;
		std::size_t i = 0;
		while (i < std::min(lhsLength, rhsLength)) {
			DigitPairType tmp = carry + lhs[i] + rhs[i];
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
		while (i < rhsLength) {
			DigitPairType tmp = carry + rhs[i];
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
	const decltype(Unsigned::digits)& rhs;
	decltype(Unsigned::digits)& result;
	const std::size_t lhsLength;
	const std::size_t rhsLength;
};

Unsigned& Unsigned::operator+=(const Unsigned& other)
{
	AdditionOfUnsigned(digits, other.digits, digits).addDigits();
	normalize();
	return *this;
}

Unsigned operator+(const Unsigned& lhs, const Unsigned& rhs)
{
	Unsigned result;
	Unsigned::AdditionOfUnsigned(lhs.digits, rhs.digits, result.digits).addDigits();
	result.normalize();
	return result;
}

} // namespace MultiPrecision
