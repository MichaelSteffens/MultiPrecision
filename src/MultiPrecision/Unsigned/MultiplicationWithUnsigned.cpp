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
	MultiplicationWithUnsigned(const Unsigned& lhs, const Unsigned& rhs) :
		lhs(lhs),
		rhs(rhs),
		lhsLength(lhs.digits.size()),
		rhsLength(rhs.digits.size())
	{
	}

	Unsigned getProduct()
	{
		product.digits.assign(lhsLength + rhsLength, 0);
		for (std::size_t j = 0; j < rhsLength; ++j) {
			DigitPairType carry = 0;
			for (std::size_t i = 0; i < lhsLength; ++i) {
				DigitPairType tmp = DigitPairType(lhs.digits[i]) * rhs.digits[j] + product.digits[i + j] + carry;
				product.digits[i + j] = tmp & std::numeric_limits<DigitType>::max();
				carry = tmp >> std::numeric_limits<DigitType>::digits;
			}
			product.digits[lhsLength + j] = carry;
		}
		product.normalize();
		return std::move(product);
	}

private:
	const Unsigned& lhs;
	const Unsigned& rhs;
	const std::size_t lhsLength;
	const std::size_t rhsLength;
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
