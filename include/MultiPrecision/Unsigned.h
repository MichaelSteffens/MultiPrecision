//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_Unsigned_INCLUDED
#define MultiPrecision_Unsigned_INCLUDED

#include <vector>

namespace MultiPrecision {

class Unsigned
{
public:
	using DigitType = unsigned short;
	static_assert(std::is_integral<DigitType>::value);
	static_assert(std::is_unsigned<DigitType>::value);

	struct DivisionResult;

	Unsigned();
	Unsigned(const Unsigned& other);
	Unsigned(Unsigned&& other);
	virtual ~Unsigned();
	Unsigned& operator=(const Unsigned& other);
	Unsigned& operator=(Unsigned&& other);
	Unsigned(unsigned long long i);
	operator unsigned long long() const;
	// operator bool();
	bool equal(const Unsigned& other) const noexcept;
	bool lessThan(const Unsigned& other) const noexcept;
	bool lessThanOrEqual(const Unsigned& other) const noexcept;
	Unsigned plus(const Unsigned& other) const;
	Unsigned& add(const Unsigned& other);
	Unsigned& add(DigitType other);
	Unsigned minus(const Unsigned& other) const;
	Unsigned& subtract(const Unsigned& other);
	Unsigned& subtract(DigitType other);
	Unsigned times(const Unsigned& other) const;
	Unsigned times(DigitType factor) const;
	DivisionResult dividedBy(const Unsigned& other) const;
	DivisionResult dividedBy(DigitType other) const;
	Unsigned shiftedLeftBy(std::size_t bits);
	Unsigned& shiftLeftBy(std::size_t bits);
	Unsigned shiftedRightBy(std::size_t bits);
	Unsigned& shiftRightBy(std::size_t bits);

private:
	class DivisionByUnsigned;
	class DivisionByDigitType;

	void assign(std::vector<DigitType>::const_iterator start, std::vector<DigitType>::const_iterator end);
	bool subtractAndTestNegative(const Unsigned& other);
	void normalize();

	std::vector<DigitType> digits;
};

struct Unsigned::DivisionResult
{
	Unsigned quotient;
	Unsigned remainder;
};

} // namespace MultiPrecision

#endif // MultiPrecision_Unsigned_INCLUDED
