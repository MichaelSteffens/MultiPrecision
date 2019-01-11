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
	MultiplicationWithDigitType(const Unsigned& lhs, DigitType rhs) : lhs(lhs), rhs(rhs), lhsLength(lhs.digits.size())
	{
	}

	Unsigned getProduct()
	{
		product.digits.assign(lhsLength + 1, 0);
		DigitPairType carry = 0;
		for (std::size_t i = 0; i < lhsLength; ++i) {
			DigitPairType tmp = DigitPairType(lhs.digits[i]) * rhs + product.digits[i] + carry;
			product.digits[i] = tmp & std::numeric_limits<DigitType>::max();
			carry = tmp >> std::numeric_limits<DigitType>::digits;
		}
		product.digits[lhsLength] = carry;
		product.normalize();
		return std::move(product);
	}

private:
	const Unsigned& lhs;
	const DigitType rhs;
	const std::size_t lhsLength;
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
