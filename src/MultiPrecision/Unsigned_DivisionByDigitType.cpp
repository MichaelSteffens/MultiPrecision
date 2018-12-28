//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/DivisionByZero.h"
#include "MultiPrecision/Unsigned.h"
#include "MultiPrecision/UnsignedUnderflow.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

/// Simplified algorithm D for n == 1 from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
class Unsigned::DivisionByDigitType
{
public:
	DivisionByDigitType(const Unsigned& dividend, DigitType divisor) : remainder(dividend), divisor(divisor)
	{
	}

	Unsigned::DivisionResult quotientAndRemainder()
	{
		if (remainder.digits.size() > 1 || (remainder.digits.size() == 1 && remainder.digits.front() >= divisor)) {
			loopOverQuotientDigits();
		}
		return normalizedResult();
	}

private:
	static constexpr DigitPairType radix = static_cast<DigitPairType>(1) << std::numeric_limits<DigitType>::digits;

	void loopOverQuotientDigits()
	{
		std::size_t quotientLength = remainder.digits.size();
		quotient.digits.assign(quotientLength, 0); // Reserve quotient result space, to be populated in reverse order below.
		for (std::size_t i = quotientLength; i--;) {
			calculateQuotientDigit(i);
			multiplyAndSubtract(i);
		};
	}

	void calculateQuotientDigit(std::size_t i)
	{
		remainder.digits.resize(i + 2);
		quotient.digits[i] = ((static_cast<DigitPairType>(remainder.digits[i + 1]) << std::numeric_limits<DigitType>::digits) +
							  remainder.digits[i]) /
							 divisor;
	}

	void multiplyAndSubtract(std::size_t i)
	{
		remainderFragment.digits.assign(remainder.digits.begin() + i, remainder.digits.end());
		remainderFragment.subtract(divisor * quotient.digits[i]);
		remainder.digits.resize(i);
		remainder.digits.insert(remainder.digits.end(), remainderFragment.digits.begin(), remainderFragment.digits.end());
	}

	Unsigned::DivisionResult normalizedResult()
	{
		quotient.normalize();
		remainder.normalize();
		return Unsigned::DivisionResult({std::move(quotient), std::move(remainder)});
	}

	Unsigned remainder;
	DigitType divisor;
	Unsigned quotient;
	Unsigned remainderFragment;
};

Unsigned::DivisionResult Unsigned::dividedBy(DigitType other) const
{
	if (other) {
		return DivisionByDigitType(*this, other).quotientAndRemainder();
	} else {
		throw DivisionByZero("Unsigned::dividedBy(DigitType): divisor is zero!");
	}
}

} // namespace MultiPrecision
