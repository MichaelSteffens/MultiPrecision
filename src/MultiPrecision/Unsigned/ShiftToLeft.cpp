//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

class Unsigned::ShiftToLeft
{
public:
	ShiftToLeft(const Unsigned& n, Unsigned& result, std::size_t bits) :
		digits(n.digits),
		result(result),
		shiftDigits(bits / std::numeric_limits<DigitType>::digits),
		shiftBits(bits % std::numeric_limits<DigitType>::digits)
	{
	}

	void run()
	{
		if (!digits.empty()) {
			result.digits.resize(digits.size() + shiftDigits + 1);
			result.digits[result.digits.size() - 1] = leftBitsOf(digits[result.digits.size() - shiftDigits - 2]);
			for (std::size_t i = result.digits.size() - 2; i > shiftDigits; --i) {
				result.digits[i] = rightBitsOf(digits[i - shiftDigits]) | leftBitsOf(digits[i - shiftDigits - 1]);
			}
			result.digits[shiftDigits] = rightBitsOf(digits[0]);
			for (std::size_t i = 0; i < shiftDigits; ++i) {
				result.digits[i] = 0;
			}
			result.normalize();
		} else {
			result.digits.clear();
		}
	}

private:
	DigitType leftBitsOf(DigitType digit)
	{
		return shiftBits ? digit >> (std::numeric_limits<DigitType>::digits - shiftBits) : 0;
	}

	DigitType rightBitsOf(DigitType digit)
	{
		return digit << shiftBits;
	}

	const decltype(Unsigned::digits)& digits;
	Unsigned& result;
	const std::size_t shiftDigits;
	const std::size_t shiftBits;
};

Unsigned& Unsigned::operator<<=(std::size_t bits)
{
	ShiftToLeft(*this, *this, bits).run();
	return *this;
}

Unsigned operator<<(const Unsigned& n, std::size_t bits)
{
	Unsigned result;
	Unsigned::ShiftToLeft(n, result, bits).run();
	return result;
}

} // namespace MultiPrecision
