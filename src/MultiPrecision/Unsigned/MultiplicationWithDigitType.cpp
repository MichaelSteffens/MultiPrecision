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

class Unsigned::MultiplicationWithDigitType
{
public:
	MultiplicationWithDigitType(const Unsigned& multiplicand, DigitType multiplier) :
		multiplicand(multiplicand),
		multiplier(multiplier),
		multiplicandLength(multiplicand.digits.size())
	{
	}

	Unsigned getProduct()
	{
		product.digits.assign(multiplicandLength + 1, 0);
		DigitPairType carry = 0;
		for (std::size_t i = 0; i < multiplicandLength; ++i) {
			DigitPairType tmp = DigitPairType(multiplicand.digits[i]) * multiplier + product.digits[i] + carry;
			product.digits[i] = tmp & std::numeric_limits<DigitType>::max();
			carry = tmp >> std::numeric_limits<DigitType>::digits;
		}
		product.digits[multiplicandLength] = carry;
		product.normalize();
		return std::move(product);
	}

private:
	const Unsigned& multiplicand;
	const DigitType multiplier;
	const std::size_t multiplicandLength;
	Unsigned product;
};

Unsigned& Unsigned::operator*=(const DigitType other)
{
	*this = MultiplicationWithDigitType(*this, other).getProduct();
	return *this;
}

Unsigned Unsigned::operator*(const DigitType other) const
{
	return MultiplicationWithDigitType(*this, other).getProduct();
}

} // namespace MultiPrecision
