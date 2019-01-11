//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/Unsigned.h"
#include <limits>

namespace MultiPrecision {

class Unsigned::AdditionOfUnsigned
{
public:
	AdditionOfUnsigned(
		const decltype(Unsigned::digits)& augend,
		const decltype(Unsigned::digits)& addend,
		decltype(Unsigned::digits)& sum) :
		augend(augend),
		addend(addend),
		sum(sum),
		augendLength(augend.size()),
		addendLength(addend.size())
	{
	}

	void addDigits()
	{
		sum.resize(std::max(augendLength, addendLength));
		DigitPairType carry = 0;
		std::size_t i = 0;
		while (i < std::min(augendLength, addendLength)) {
			DigitPairType tmp = carry + augend[i] + addend[i];
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
		while (i < addendLength) {
			DigitPairType tmp = carry + addend[i];
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
	const decltype(Unsigned::digits)& addend;
	decltype(Unsigned::digits)& sum;
	const std::size_t augendLength;
	const std::size_t addendLength;
};

Unsigned& Unsigned::operator+=(const Unsigned& other)
{
	AdditionOfUnsigned(digits, other.digits, digits).addDigits();
	normalize();
	return *this;
}

Unsigned operator+(const Unsigned& lhs, const Unsigned& rhs)
{
	Unsigned result;
	Unsigned::AdditionOfUnsigned(lhs.digits, rhs.digits, result.digits).addDigits();
	result.normalize();
	return result;
}

} // namespace MultiPrecision
