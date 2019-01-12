//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"

namespace MultiPrecision {

class Unsigned::LessThanOrEqualComparison
{
public:
	LessThanOrEqualComparison(const decltype(Unsigned::digits)& lhs, const decltype(Unsigned::digits)& rhs) :
		lhs(lhs),
		rhs(rhs),
		lhsLength(lhs.size()),
		rhsLength(rhs.size())
	{
	}

	bool compareDigits() noexcept
	{
		if (lhsLength < rhsLength) {
			return true;
		} else if (lhsLength > rhsLength) {
			return false;
		}
		for (std::size_t i = lhsLength; i-- > 0;) {
			if (lhs[i] < rhs[i]) {
				return true;
			} else if (lhs[i] > rhs[i]) {
				return false;
			}
		}
		return true;
	}

private:
	const decltype(Unsigned::digits)& lhs;
	const decltype(Unsigned::digits)& rhs;
	const std::size_t lhsLength;
	const std::size_t rhsLength;
};

bool operator<=(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return Unsigned::LessThanOrEqualComparison(lhs.digits, rhs.digits).compareDigits();
}

bool operator>(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return !Unsigned::LessThanOrEqualComparison(lhs.digits, rhs.digits).compareDigits();
}

} // namespace MultiPrecision
