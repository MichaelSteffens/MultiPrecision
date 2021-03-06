//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

class Unsigned::LeftShift
{
public:
	LeftShift(const decltype(Unsigned::digits)& digits, decltype(Unsigned::digits)& result, std::size_t bits) :
		digits(digits),
		result(result),
		digitShift(bits / std::numeric_limits<DigitType>::digits),
		bitShift(bits % std::numeric_limits<DigitType>::digits)
	{
	}

	void shiftBitsInDigits()
	{
		if (!digits.empty()) {
			result.resize(digits.size() + digitShift + 1);
			result[result.size() - 1] = leftBitsOf(digits[result.size() - digitShift - 2]);
			for (std::size_t i = result.size() - 2; i > digitShift; --i) {
				result[i] = rightBitsOf(digits[i - digitShift]) | leftBitsOf(digits[i - digitShift - 1]);
			}
			result[digitShift] = rightBitsOf(digits[0]);
			for (std::size_t i = 0; i < digitShift; ++i) {
				result[i] = 0;
			}
			if (!result.back()) {
				result.pop_back();
			}
		} else {
			result.clear();
		}
	}

private:
	DigitType leftBitsOf(DigitType digit)
	{
		return bitShift ? digit >> (std::numeric_limits<DigitType>::digits - bitShift) : 0;
	}

	DigitType rightBitsOf(DigitType digit)
	{
		return digit << bitShift;
	}

	const decltype(Unsigned::digits)& digits;
	decltype(Unsigned::digits)& result;
	const std::size_t digitShift;
	const std::size_t bitShift;
};

Unsigned& Unsigned::operator<<=(std::size_t bits)
{
	LeftShift(digits, digits, bits).shiftBitsInDigits();
	return *this;
}

Unsigned operator<<(const Unsigned& n, std::size_t bits)
{
	Unsigned result;
	Unsigned::LeftShift(n.digits, result.digits, bits).shiftBitsInDigits();
	return result;
}

} // namespace MultiPrecision
