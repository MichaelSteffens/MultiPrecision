//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

class Unsigned::RightShift
{
public:
	RightShift(const decltype(Unsigned::digits)& digits, decltype(Unsigned::digits)& result, std::size_t bits) :
		digits(digits),
		result(result),
		digitShift(bits / std::numeric_limits<DigitType>::digits),
		bitShift(bits % std::numeric_limits<DigitType>::digits)
	{
	}

	template<bool resultIsDigits>
	void shiftBitsInDigits()
	{
		if (digitShift < digits.size()) {
			if (!resultIsDigits) {
				result.resize(digits.size() - digitShift);
			}
			for (std::size_t i = 0; i < digits.size() - digitShift - 1; ++i) {
				result[i] = leftBitsOf(digits[i + digitShift]) | rightBitsOf(digits[i + digitShift + 1]);
			}
			result[digits.size() - digitShift - 1] = leftBitsOf(digits[digits.size() - 1]);
			if (resultIsDigits) {
				result.resize(digits.size() - digitShift);
			}
		} else {
			result.clear();
		}
	}

private:
	DigitType rightBitsOf(DigitType digit)
	{
		return bitShift ? digit << (std::numeric_limits<DigitType>::digits - bitShift) : 0;
	}

	DigitType leftBitsOf(DigitType digit)
	{
		return digit >> bitShift;
	}

	const decltype(Unsigned::digits)& digits;
	decltype(Unsigned::digits)& result;
	const std::size_t digitShift;
	const std::size_t bitShift;
};

Unsigned& Unsigned::operator>>=(std::size_t bits)
{
	RightShift(digits, digits, bits).shiftBitsInDigits<true>();
	normalize();
	return *this;
}

Unsigned operator>>(const Unsigned& n, std::size_t bits)
{
	Unsigned result;
	Unsigned::RightShift(n.digits, result.digits, bits).shiftBitsInDigits<false>();
	result.normalize();
	return result;
}

} // namespace MultiPrecision
