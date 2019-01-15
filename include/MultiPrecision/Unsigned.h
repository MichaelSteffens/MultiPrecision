//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_Unsigned_INCLUDED
#define MultiPrecision_Unsigned_INCLUDED

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace MultiPrecision {

class Unsigned
{
public:
	using DigitType = unsigned int;
	static_assert(std::is_integral<DigitType>::value);
	static_assert(std::is_unsigned<DigitType>::value);

	class BitRange;
	class MinimalBitRange;
	class FullBitRange;
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
	Unsigned& operator+=(const Unsigned& other);
	Unsigned& operator++();
	Unsigned operator++(int);
	Unsigned& operator-=(const Unsigned& other);
	Unsigned& operator--();
	Unsigned operator--(int);
	Unsigned& operator*=(const Unsigned& other);
	DivisionResult dividedBy(const Unsigned& other) const;
	Unsigned& operator/=(const Unsigned& other);
	Unsigned& operator%=(const Unsigned& other);
	Unsigned& operator<<=(std::size_t bits);
	Unsigned& operator>>=(std::size_t bits);
	static void read(std::istream& in, Unsigned& out);
	static void readDecimal(std::istream& in, Unsigned& out);
	static void readHexadecimal(std::istream& in, Unsigned& out);
	static void readOctal(std::istream& in, Unsigned& out);
	static Unsigned fromDecimal(std::string::const_iterator first, std::string::const_iterator last);
	static Unsigned fromHexadecimal(std::string::const_iterator first, std::string::const_iterator last);
	static Unsigned fromOctal(std::string::const_iterator first, std::string::const_iterator last);
	std::string toDecimalString() const;
	std::string toHexadecimalString(bool uppercase = false) const;
	std::string toOctalString() const;
	bool isZero() const noexcept;
	MinimalBitRange minimalBitRange() const noexcept;
	FullBitRange fullBitRange() const noexcept;
	std::size_t mostSignificantBitPosition() const noexcept;

private:
	class AdditionOfUnsigned;
	class AdditionOfDigitType;
	class SubtractionOfUnsigned;
	class SubtractionOfDigitType;
	class MultiplicationWithUnsigned;
	class MultiplicationWithDigitType;
	class DivisionByUnsigned;
	class DivisionByDigitType;
	class LeftShift;
	class RightShift;
	class EqualComparison;
	class LessThanComparison;
	class LessThanOrEqualComparison;
	class StringReadBuffer;

	Unsigned& operator+=(DigitType other);
	Unsigned& operator-=(DigitType other);
	Unsigned& operator*=(DigitType other);
	Unsigned operator*(DigitType other) const;
	DivisionResult dividedBy(DigitType divisor) const;
	Unsigned& operator/=(DigitType divisor);
	Unsigned& operator%=(DigitType divisor);

	bool subtractAndTestNegative(const Unsigned& other);
	std::size_t mostSignificantDigitPosition() const noexcept;
	void normalize();

	friend bool operator==(const Unsigned& lhs, const Unsigned& rhs) noexcept;
	friend bool operator!=(const Unsigned& lhs, const Unsigned& rhs) noexcept;
	friend bool operator<(const Unsigned& lhs, const Unsigned& rhs) noexcept;
	friend bool operator<=(const Unsigned& lhs, const Unsigned& rhs) noexcept;
	friend bool operator>(const Unsigned& lhs, const Unsigned& rhs) noexcept;
	friend bool operator>=(const Unsigned& lhs, const Unsigned& rhs) noexcept;
	friend Unsigned operator+(const Unsigned& lhs, const Unsigned& rhs);
	friend Unsigned operator-(const Unsigned& lhs, const Unsigned& rhs);
	friend Unsigned operator*(const Unsigned& lhs, const Unsigned& rhs);
	friend Unsigned operator/(const Unsigned& lhs, const Unsigned& rhs);
	friend Unsigned operator%(const Unsigned& lhs, const Unsigned& rhs);
	friend Unsigned operator<<(const Unsigned& n, std::size_t bits);
	friend Unsigned operator>>(const Unsigned& n, std::size_t bits);
	friend std::istream& operator>>(std::istream& in, Unsigned& out);
	friend std::ostream& operator<<(std::ostream& out, const Unsigned& in);

	std::vector<DigitType> digits;
};

struct Unsigned::DivisionResult
{
	Unsigned quotient;
	Unsigned remainder;
};

class Unsigned::BitRange
{
public:
	class ConstIterator;

	virtual ~BitRange()
	{
	}
	virtual ConstIterator begin() const = 0;
	virtual ConstIterator end() const = 0;
};

class Unsigned::BitRange::ConstIterator
{
public:
	ConstIterator(const decltype(Unsigned::digits)& digits, std::size_t digitPosition, DigitType mask);
	ConstIterator& operator++();
	bool operator*() const noexcept;
	friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) noexcept;

private:
	const decltype(Unsigned::digits)* digits;
	std::size_t digitPosition;
	DigitType mask;
};

class Unsigned::FullBitRange : public Unsigned::BitRange
{
public:
	explicit FullBitRange(const Unsigned& number);
	~FullBitRange();
	virtual ConstIterator begin() const override;
	virtual ConstIterator end() const override;

private:
	const Unsigned* number;
};

class Unsigned::MinimalBitRange : public Unsigned::BitRange
{
public:
	explicit MinimalBitRange(const Unsigned& number);
	~MinimalBitRange();
	virtual ConstIterator begin() const override;
	virtual ConstIterator end() const override;

private:
	const Unsigned* number;
};

} // namespace MultiPrecision

#endif // MultiPrecision_Unsigned_INCLUDED
