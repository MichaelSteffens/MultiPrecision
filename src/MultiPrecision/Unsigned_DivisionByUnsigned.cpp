//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include "MultiPrecision/DivisionByZero.h"
#include "MultiPrecision/UnsignedUnderflow.h"
#include "MultiPrecision/DigitPairType.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

/// Algorithm D from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
class Unsigned::DivisionByUnsigned
{
public:
	DivisionByUnsigned(const Unsigned& dividend, const Unsigned& divisor) :
		remainder(dividend),
		divisor(divisor),
		divisorLength(divisor.digits.size())
	{
	}

	Unsigned::DivisionResult quotientAndRemainder()
	{
		if (remainder >= divisor) {
			expandFraction();
			loopOverQuotientDigits();
			reduceRemainder();
		}
		return normalizedResult();
	}

private:
	static constexpr DigitPairType radix = static_cast<DigitPairType>(1) << std::numeric_limits<DigitType>::digits;

	// Expand remainder (initialized by dividend) and divisor by a common factor power of 2, ensuring that the divisor most
	// significant word is >= radix / 2.
	void expandFraction()
	{
		expansionShift = 0;
		for (DigitType mask = 1 << (std::numeric_limits<DigitType>::digits - 1); !(divisor.digits.back() & mask); mask >>= 1) {
			++expansionShift;
		}
		remainder.shiftLeftBy(expansionShift);
		divisor.shiftLeftBy(expansionShift);
	}

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
		testQuotientDigitTooLarge(i);
		if (trialRemainder < radix) {
			testQuotientDigitTooLarge(i);
		}
		quotient.digits[i] = trialQuotient;
	}

	void testQuotientDigitTooLarge(std::size_t i)
	{
		if (trialQuotient == radix ||
			(trialQuotient * divisor.digits[divisorLength - 2]) >
				((trialRemainder << std::numeric_limits<DigitType>::digits) + remainder.digits[i + divisorLength - 2])) {
			--trialQuotient;
			trialRemainder += divisor.digits[divisorLength - 1];
		}
	}

	void multiplyAndSubtract(std::size_t i)
	{
		remainderFragment.digits.assign(remainder.digits.begin() + i, remainder.digits.end());
		if (remainderFragment.subtractAndTestNegative(divisor.times(trialQuotient))) {
			addBack(i);
		}
		remainder.digits.resize(i);
		remainder.digits.insert(remainder.digits.end(), remainderFragment.digits.begin(), remainderFragment.digits.end());
	}

	void addBack(std::size_t i)
	{
		--quotient.digits[i];
		remainderFragment.add(divisor);
		remainderFragment.digits.pop_back();
	}

	// Revert fraction expansion
	void reduceRemainder()
	{
		remainder.shiftRightBy(expansionShift);
	}

	Unsigned::DivisionResult normalizedResult()
	{
		quotient.normalize();
		remainder.normalize();
		return Unsigned::DivisionResult({std::move(quotient), std::move(remainder)});
	}

	Unsigned remainder;
	Unsigned divisor;
	Unsigned quotient;
	std::size_t divisorLength;
	std::size_t expansionShift;
	DigitPairType trialQuotient;
	DigitPairType trialRemainder;
	Unsigned remainderFragment;
};

Unsigned::DivisionResult Unsigned::dividedBy(const Unsigned& other) const
{
	Unsigned::DivisionResult result;
	if (other.digits.empty()) {
		throw DivisionByZero("Unsigned::dividedBy(const Unsigned&): divisor is zero!");
	} else if (other.digits.size() == 1) {
		result = dividedBy(other.digits[0]);
	} else {
		result = DivisionByUnsigned(*this, other).quotientAndRemainder();
	}
	return result;
}

} // namespace MultiPrecision
