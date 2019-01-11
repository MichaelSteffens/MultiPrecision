//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/DivisionByZero.h"
#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

/// Simplified algorithm D for n == 1 from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
class Unsigned::DivisionByDigitType
{
public:
	DivisionByDigitType(const Unsigned& dividend, DigitType divisor) : remainder(dividend), divisor(divisor)
	{
	}

	Unsigned::DivisionResult getQuotientAndRemainder()
	{
		if (remainder.digits.size() > 1 || (remainder.digits.size() == 1 && remainder.digits.front() >= divisor)) {
			loopOverQuotientDigits();
		}
		quotient.normalize();
		remainder.normalize();
		return Unsigned::DivisionResult({std::move(quotient), std::move(remainder)});
	}

	Unsigned getQuotient()
	{
		if (remainder.digits.size() > 1 || (remainder.digits.size() == 1 && remainder.digits.front() >= divisor)) {
			loopOverQuotientDigits();
		}
		quotient.normalize();
		return std::move(quotient);
	}

	Unsigned getRemainder()
	{
		if (remainder.digits.size() > 1 || (remainder.digits.size() == 1 && remainder.digits.front() >= divisor)) {
			loopOverQuotientDigits();
		}
		remainder.normalize();
		return std::move(remainder);
	}

private:
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
		quotient.digits[i] =
			((DigitPairType(remainder.digits[i + 1]) << std::numeric_limits<DigitType>::digits) + remainder.digits[i]) / divisor;
	}

	void multiplyAndSubtract(std::size_t i)
	{
		remainderFragment.digits.assign(remainder.digits.begin() + i, remainder.digits.end());
		remainderFragment -= Unsigned(DigitPairType(divisor) * quotient.digits[i]);
		remainder.digits.resize(i);
		remainder.digits.insert(remainder.digits.end(), remainderFragment.digits.begin(), remainderFragment.digits.end());
	}

	Unsigned remainder;
	DigitType divisor;
	Unsigned quotient;
	Unsigned remainderFragment;
};

Unsigned::DivisionResult Unsigned::dividedBy(DigitType divisor) const
{
	if (divisor) {
		return DivisionByDigitType(*this, divisor).getQuotientAndRemainder();
	} else {
		throw DivisionByZero("Unsigned::dividedBy(DigitType): divisor is zero!");
	}
}

Unsigned& Unsigned::operator/=(DigitType divisor)
{
	if (divisor) {
		*this = DivisionByDigitType(*this, divisor).getQuotient();
		return *this;
	} else {
		throw DivisionByZero("Unsigned::operator/=(DigitType): divisor is zero!");
	}
}

Unsigned& Unsigned::operator%=(DigitType divisor)
{
	if (divisor) {
		*this = DivisionByDigitType(*this, divisor).getRemainder();
		return *this;
	} else {
		throw DivisionByZero("Unsigned::operator%=(DigitType): divisor is zero!");
	}
}

} // namespace MultiPrecision
