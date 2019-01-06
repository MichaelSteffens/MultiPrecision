//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_Unsigned_INCLUDED
#define MultiPrecision_Unsigned_INCLUDED

#include <string>
#include <vector>

namespace MultiPrecision {

class Unsigned
{
public:
	using DigitType = unsigned int;
	static_assert(std::is_integral<DigitType>::value);
	static_assert(std::is_unsigned<DigitType>::value);

	struct DivisionResult;

	Unsigned();
	Unsigned(const Unsigned& other);
	Unsigned(Unsigned&& other);
	virtual ~Unsigned();
	Unsigned& operator=(const Unsigned& other);
	Unsigned& operator=(Unsigned&& other);
	explicit Unsigned(unsigned long long n);
	Unsigned(const char* number);
	Unsigned(const std::string& number);
	Unsigned(std::string::const_iterator first, std::string::const_iterator last);
	bool equal(const Unsigned& other) const noexcept;
	bool lessThan(const Unsigned& other) const noexcept;
	bool lessThanOrEqual(const Unsigned& other) const noexcept;
	Unsigned plus(const Unsigned& other) const;
	Unsigned& add(const Unsigned& other);
	Unsigned minus(const Unsigned& other) const;
	Unsigned& subtract(const Unsigned& other);
	Unsigned times(const Unsigned& other) const;
	DivisionResult dividedBy(const Unsigned& other) const;
	Unsigned& operator/=(const Unsigned& other);
	Unsigned& operator%=(const Unsigned& other);
	Unsigned shiftedLeftBy(std::size_t bits) const;
	Unsigned& shiftLeftBy(std::size_t bits);
	Unsigned shiftedRightBy(std::size_t bits) const;
	Unsigned& shiftRightBy(std::size_t bits);
	static Unsigned fromDecimal(std::string::const_iterator first, std::string::const_iterator last);
	static Unsigned fromHexadecimal(std::string::const_iterator first, std::string::const_iterator last);
	static Unsigned fromOctal(std::string::const_iterator first, std::string::const_iterator last);
	std::string toDecimalString() const;
	std::string toHexadecimalString() const;
	std::string toOctalString() const;
	operator bool() const noexcept;

private:
	class DivisionByUnsigned;
	class DivisionByDigitType;

	Unsigned& add(DigitType other);
	Unsigned& subtract(DigitType other);
	Unsigned times(DigitType factor) const;
	DivisionResult dividedBy(DigitType divisor) const;
	Unsigned& operator/=(DigitType divisor);
	Unsigned& operator%=(DigitType divisor);

	bool subtractAndTestNegative(const Unsigned& other);
	void normalize();

	std::vector<DigitType> digits;
};

struct Unsigned::DivisionResult
{
	Unsigned quotient;
	Unsigned remainder;
};

bool operator==(const Unsigned& lhs, const Unsigned& rhs) noexcept;
bool operator!=(const Unsigned& lhs, const Unsigned& rhs) noexcept;
bool operator<(const Unsigned& lhs, const Unsigned& rhs) noexcept;
bool operator<=(const Unsigned& lhs, const Unsigned& rhs) noexcept;
bool operator>(const Unsigned& lhs, const Unsigned& rhs) noexcept;
bool operator>=(const Unsigned& lhs, const Unsigned& rhs) noexcept;
Unsigned operator/(const Unsigned& lhs, const Unsigned& rhs);
Unsigned operator%(const Unsigned& lhs, const Unsigned& rhs);

} // namespace MultiPrecision

#endif // MultiPrecision_Unsigned_INCLUDED
