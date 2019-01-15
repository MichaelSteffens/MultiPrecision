//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/Unsigned.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

class Unsigned::MultiplicationWithUnsigned
{
public:
	MultiplicationWithUnsigned(const Unsigned& multiplicand, const Unsigned& multiplier) :
		multiplicand(multiplicand),
		multiplier(multiplier),
		multiplicandLength(multiplicand.digits.size()),
		multiplierLength(multiplier.digits.size())
	{
	}

	Unsigned getProduct()
	{
		product.digits.resize(multiplicandLength + multiplierLength);
		for (std::size_t j = 0; j < multiplierLength; ++j) {
			DigitPairType carry = 0;
			for (std::size_t i = 0; i < multiplicandLength; ++i) {
				DigitPairType tmp = DigitPairType(multiplicand.digits[i]) * multiplier.digits[j] + product.digits[i + j] + carry;
				product.digits[i + j] = tmp & std::numeric_limits<DigitType>::max();
				carry = tmp >> std::numeric_limits<DigitType>::digits;
			}
			product.digits[multiplicandLength + j] = carry;
		}
		product.trimMostSignificantDigit();
		return std::move(product);
	}

private:
	const Unsigned& multiplicand;
	const Unsigned& multiplier;
	const std::size_t multiplicandLength;
	const std::size_t multiplierLength;
	Unsigned product;
};

Unsigned& Unsigned::operator*=(const Unsigned& other)
{
	*this = MultiplicationWithUnsigned(*this, other).getProduct();
	return *this;
}

Unsigned operator*(const Unsigned& lhs, const Unsigned& rhs)
{
	return Unsigned::MultiplicationWithUnsigned(lhs, rhs).getProduct();
}

} // namespace MultiPrecision
