//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"

namespace MultiPrecision {

class Unsigned::LessThanComparison
{
public:
	LessThanComparison(const decltype(Unsigned::digits)& lhs, const decltype(Unsigned::digits)& rhs) :
		lhs(lhs),
		rhs(rhs),
		lhsLength(lhs.size()),
		rhsLength(rhs.size()),
		minLength(std::min(lhsLength, rhsLength))
	{
	}

	bool compareDigits() noexcept
	{
		for (std::size_t i = lhsLength; i-- > minLength;) {
			if (lhs[i]) {
				return false;
			}
		}
		for (std::size_t i = rhsLength; i-- > minLength;) {
			if (rhs[i]) {
				return true;
			}
		}
		for (std::size_t i = minLength; i-- > 0;) {
			if (lhs[i] < rhs[i]) {
				return true;
			} else if (lhs[i] > rhs[i]) {
				return false;
			}
		}
		return false;
	}

private:
	const decltype(Unsigned::digits)& lhs;
	const decltype(Unsigned::digits)& rhs;
	const std::size_t lhsLength;
	const std::size_t rhsLength;
	const std::size_t minLength;
};

bool operator<(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return Unsigned::LessThanComparison(lhs.digits, rhs.digits).compareDigits();
}

bool operator>=(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return !Unsigned::LessThanComparison(lhs.digits, rhs.digits).compareDigits();
}

} // namespace MultiPrecision
