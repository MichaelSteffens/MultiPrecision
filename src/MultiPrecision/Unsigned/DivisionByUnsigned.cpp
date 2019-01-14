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

/// Algorithm D from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
class Unsigned::DivisionByUnsigned
{
public:
	// Expand remainder (initialized by dividend) and divisor by a common factor power of 2, ensuring that the divisor most
	// significant digit is >= RADIX / 2.
	DivisionByUnsigned(const Unsigned& dividend, const Unsigned& divisor) :
		divisorLength(divisor.digits.size()),
		expansionShift(divisorLength * std::numeric_limits<DigitType>::digits - divisor.mostSignificantBitPosition()),
		divisor(divisor << expansionShift)
	{
		// Sufficient space for expansion shift result, including an extra digit appended in calculateQuotientDigit() below.
		remainder.digits.reserve(dividend.digits.size() + expansionShift / std::numeric_limits<DigitType>::digits + 2);
		remainder.digits.assign(dividend.digits.begin(), dividend.digits.end());
		remainder <<= expansionShift;
	}

	Unsigned::DivisionResult getQuotientAndRemainder()
	{
		if (remainder >= divisor) {
			loopOverQuotientDigits();
		}
		reduceRemainder();
		quotient.normalize();
		remainder.normalize();
		return Unsigned::DivisionResult({std::move(quotient), std::move(remainder)});
	}

	Unsigned getQuotient()
	{
		if (remainder >= divisor) {
			loopOverQuotientDigits();
		}
		quotient.normalize();
		return std::move(quotient);
	}

	Unsigned getRemainder()
	{
		if (remainder >= divisor) {
			loopOverQuotientDigits();
		}
		reduceRemainder();
		remainder.normalize();
		return std::move(remainder);
	}

private:
	static constexpr DigitPairType RADIX = static_cast<DigitPairType>(1) << std::numeric_limits<DigitType>::digits;

	void loopOverQuotientDigits()
	{
		std::size_t quotientLength = remainder.digits.size() - divisorLength + 1;
		quotient.digits.assign(quotientLength, 0); // Reserve quotient result space, to be populated in reverse order below.
		for (std::size_t i = quotientLength; i--;) {
			calculateQuotientDigit(i);
			multiplyAndSubtract(i);
		};
	}

	void calculateQuotientDigit(std::size_t i)
	{
		remainder.digits.resize(i + divisorLength + 1);
		DigitPairType trialDividend =
			(static_cast<DigitPairType>(remainder.digits[i + divisorLength]) << std::numeric_limits<DigitType>::digits) +
			remainder.digits[i + divisorLength - 1];
		trialQuotient = trialDividend / divisor.digits[divisorLength - 1];
		trialRemainder = trialDividend % divisor.digits[divisorLength - 1];
		adjustIfQuotientDigitTooLarge(i);
		if (trialRemainder < RADIX) {
			adjustIfQuotientDigitTooLarge(i);
		}
		quotient.digits[i] = trialQuotient;
	}

	void adjustIfQuotientDigitTooLarge(std::size_t i)
	{
		if (trialQuotient == RADIX ||
			(trialQuotient * divisor.digits[divisorLength - 2]) >
				((trialRemainder << std::numeric_limits<DigitType>::digits) + remainder.digits[i + divisorLength - 2])) {
			--trialQuotient;
			trialRemainder += divisor.digits[divisorLength - 1];
		}
	}

	void multiplyAndSubtract(std::size_t i)
	{
		remainderFragment.digits.assign(remainder.digits.begin() + i, remainder.digits.end());
		if (remainderFragment.subtractAndTestNegative(divisor * trialQuotient)) {
			addBack(i);
		}
		remainder.digits.resize(i);
		remainder.digits.insert(remainder.digits.end(), remainderFragment.digits.begin(), remainderFragment.digits.end());
	}

	void addBack(std::size_t i)
	{
		--quotient.digits[i];
		remainderFragment += divisor;
		remainderFragment.digits.pop_back();
	}

	// Revert fraction expansion
	void reduceRemainder()
	{
		remainder >>= expansionShift;
	}

	std::size_t divisorLength;
	std::size_t expansionShift;
	Unsigned divisor;
	Unsigned remainder;
	Unsigned quotient;
	DigitPairType trialQuotient;
	DigitPairType trialRemainder;
	Unsigned remainderFragment;
};

Unsigned::DivisionResult Unsigned::dividedBy(const Unsigned& other) const
{
	if (other.digits.empty()) {
		throw DivisionByZero("Unsigned::dividedBy(const Unsigned&): divisor is zero!");
	} else {
		Unsigned::DivisionResult result;
		if (other.digits.size() == 1) {
			result = dividedBy(other.digits.front());
		} else {
			result = DivisionByUnsigned(*this, other).getQuotientAndRemainder();
		}
		return result;
	}
}

Unsigned& Unsigned::operator/=(const Unsigned& other)
{
	if (other.isZero()) {
		throw DivisionByZero("Unsigned::operator/=(const Unsigned&): divisor is zero!");
	} else {
		if (other.digits.size() == 1) {
			*this /= other.digits.front();
		} else {
			*this = DivisionByUnsigned(*this, other).getQuotient();
		}
		return *this;
	}
}

Unsigned& Unsigned::operator%=(const Unsigned& other)
{
	if (other.isZero()) {
		throw DivisionByZero("Unsigned::operator%=(const Unsigned&): divisor is zero!");
	} else {
		if (other.digits.size() == 1) {
			*this %= other.digits.front();
		} else {
			*this = DivisionByUnsigned(*this, other).getRemainder();
		}
		return *this;
	}
}

Unsigned operator/(const Unsigned& lhs, const Unsigned& rhs)
{
	if (rhs.isZero()) {
		throw DivisionByZero("operator/(const Unsigned&, const Unsigned&): divisor is zero!");
	} else {
		return Unsigned::DivisionByUnsigned(lhs, rhs).getQuotient();
	}
}

Unsigned operator%(const Unsigned& lhs, const Unsigned& rhs)
{
	if (rhs.isZero()) {
		throw DivisionByZero("operator%(const Unsigned&, const Unsigned&): divisor is zero!");
	} else {
		return Unsigned::DivisionByUnsigned(lhs, rhs).getRemainder();
	}
}

} // namespace MultiPrecision
