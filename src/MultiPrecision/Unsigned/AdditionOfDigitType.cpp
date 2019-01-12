//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

class Unsigned::AdditionOfDigitType
{
public:
	AdditionOfDigitType(const decltype(Unsigned::digits)& augend, DigitType addend, decltype(Unsigned::digits)& sum) :
		augend(augend),
		addend(addend),
		sum(sum),
		augendLength(augend.size())
	{
	}

	void addDigit()
	{
		sum.resize(std::max(augendLength + 1, std::size_t(1)));
		DigitPairType carry = 0;
		std::size_t i = 0;
		while (i < std::min(augendLength, std::size_t(1))) {
			DigitPairType tmp = DigitPairType(augend[i]) + addend;
			sum[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < augendLength) {
			DigitPairType tmp = carry + augend[i];
			sum[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		while (i < 1) {
			DigitPairType tmp = carry + (i ? 0 : (i ? 0 : addend));
			sum[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
			carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
			++i;
		}
		if (carry) {
			sum.push_back(carry & std::numeric_limits<Unsigned::DigitType>::max());
		}
	}

private:
	const decltype(Unsigned::digits)& augend;
	const DigitType addend;
	decltype(Unsigned::digits)& sum;
	const std::size_t augendLength;
};

Unsigned& Unsigned::operator+=(DigitType other)
{
	AdditionOfDigitType(digits, other, digits).addDigit();
	normalize();
	return *this;
}

Unsigned& Unsigned::operator++()
{
	AdditionOfDigitType(digits, 1, digits).addDigit();
	normalize();
	return *this;
}

Unsigned Unsigned::operator++(int)
{
	Unsigned result(*this);
	AdditionOfDigitType(digits, 1, digits).addDigit();
	normalize();
	return result;
}

} // namespace MultiPrecision
