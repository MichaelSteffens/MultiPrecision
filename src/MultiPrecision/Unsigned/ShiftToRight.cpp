//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

class Unsigned::ShiftToRight
{
public:
	ShiftToRight(const Unsigned& n, Unsigned& result, std::size_t bits) :
		digits(n.digits),
		result(result),
		shiftDigits(bits / std::numeric_limits<DigitType>::digits),
		shiftBits(bits % std::numeric_limits<DigitType>::digits)
	{
	}

	template<bool resultIsDigits>
	void run()
	{
		if (shiftDigits < digits.size()) {
			if (!resultIsDigits) {
				result.digits.resize(digits.size() - shiftDigits);
			}
			for (std::size_t i = 0; i < digits.size() - shiftDigits - 1; ++i) {
				result.digits[i] = leftBitsOf(digits[i + shiftDigits]) | rightBitsOf(digits[i + shiftDigits + 1]);
			}
			result.digits[digits.size() - shiftDigits - 1] = leftBitsOf(digits[digits.size() - 1]);
			if (resultIsDigits) {
				result.digits.resize(digits.size() - shiftDigits);
			}
			result.normalize();
		} else {
			result.digits.clear();
		}
	}

private:
	DigitType rightBitsOf(DigitType digit)
	{
		return shiftBits ? digit << (std::numeric_limits<DigitType>::digits - shiftBits) : 0;
	}

	DigitType leftBitsOf(DigitType digit)
	{
		return digit >> shiftBits;
	}

	const decltype(Unsigned::digits)& digits;
	Unsigned& result;
	const std::size_t shiftDigits;
	const std::size_t shiftBits;
};

Unsigned& Unsigned::operator>>=(std::size_t bits)
{
	ShiftToRight(*this, *this, bits).run<true>();
	return *this;
}

Unsigned operator>>(const Unsigned& n, std::size_t bits)
{
	Unsigned result;
	Unsigned::ShiftToRight(n, result, bits).run<false>();
	return result;
}

} // namespace MultiPrecision
