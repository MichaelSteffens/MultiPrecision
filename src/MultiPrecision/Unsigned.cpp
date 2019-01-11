//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include "MultiPrecision/DigitPairType.h"
#include "MultiPrecision/InvalidCharacter.h"
#include <iostream>
#include <limits>

namespace MultiPrecision {

Unsigned::Unsigned()
{
}

Unsigned::Unsigned(const Unsigned& other) : digits(other.digits)
{
}

Unsigned::Unsigned(Unsigned&& other) : digits(std::move(other.digits))
{
}

Unsigned::~Unsigned()
{
}

Unsigned& Unsigned::operator=(const Unsigned& other)
{
	digits = other.digits;
	return *this;
}

Unsigned& Unsigned::operator=(Unsigned&& other)
{
	digits = std::move(other.digits);
	return *this;
}

Unsigned::Unsigned(unsigned long long n)
{
	while (n) {
		DigitType digit = n & std::numeric_limits<DigitType>::max();
		digits.push_back(digit);
		n >>= std::numeric_limits<DigitType>::digits;
	}
}

Unsigned::Unsigned(const char* number) : Unsigned(std::string(number))
{
}

Unsigned::Unsigned(const std::string& number) : Unsigned(number.begin(), number.end())
{
}

Unsigned::Unsigned(std::string::const_iterator first, std::string::const_iterator last)
{
	if (first != last) {
		if (*first != '0') {
			*this = fromDecimal(first, last);
		} else if (++first != last) {
			if (*first != 'x') {
				*this = fromOctal(first, last);
			} else if (++first != last) {
				*this = fromHexadecimal(first, last);
			}
		}
	}
}

bool Unsigned::equal(const Unsigned& other) const noexcept
{
	if (digits.size() != other.digits.size()) {
		return false;
	}
	for (std::size_t i = digits.size(); i > 0; --i) {
		if (digits[i - 1] != other.digits[i - 1]) {
			return false;
		}
	}
	return true;
}

bool Unsigned::lessThan(const Unsigned& other) const noexcept
{
	if (digits.size() < other.digits.size()) {
		return true;
	} else if (digits.size() > other.digits.size()) {
		return false;
	}
	for (std::size_t i = digits.size(); i > 0; --i) {
		if (digits[i - 1] < other.digits[i - 1]) {
			return true;
		} else if (digits[i - 1] > other.digits[i - 1]) {
			return false;
		}
	}
	return false;
}

bool Unsigned::lessThanOrEqual(const Unsigned& other) const noexcept
{
	if (digits.size() < other.digits.size()) {
		return true;
	} else if (digits.size() > other.digits.size()) {
		return false;
	}
	for (std::size_t i = digits.size(); i > 0; --i) {
		if (digits[i - 1] < other.digits[i - 1]) {
			return true;
		} else if (digits[i - 1] > other.digits[i - 1]) {
			return false;
		}
	}
	return true;
}

Unsigned Unsigned::fromDecimal(std::string::const_iterator first, std::string::const_iterator last)
{
	Unsigned result;
	for (std::string::const_iterator i = first; i != last; ++i) {
		if (*i >= '0' && *i <= '9') {
			result *= 10;
			result += *i - '0';
		} else {
			throw InvalidCharacter(
				"Unsigned::fromDecimal(std::string::const_iterator, std::string::const_iterator): invalid character");
		}
	}
	return result;
}

Unsigned Unsigned::fromHexadecimal(std::string::const_iterator first, std::string::const_iterator last)
{
	Unsigned result;
	for (std::string::const_iterator i = first; i != last; ++i) {
		if (*i >= '0' && *i <= '9') {
			result <<= 4;
			result += *i - '0';
		} else if (*i >= 'a' && *i <= 'f') {
			result <<= 4;
			result += *i - 'a' + 10;
		} else {
			throw InvalidCharacter(
				"Unsigned::fromHexadecimal(std::string::const_iterator, std::string::const_iterator): invalid character");
		}
	}
	return result;
}

Unsigned Unsigned::fromOctal(std::string::const_iterator first, std::string::const_iterator last)
{
	Unsigned result;
	for (std::string::const_iterator i = first; i != last; ++i) {
		if (*i >= '0' && *i <= '7') {
			result <<= 3;
			result += *i - '0';
		} else {
			throw InvalidCharacter(
				"Unsigned::fromOctal(std::string::const_iterator, std::string::const_iterator): invalid character");
		}
	}
	return result;
}

std::string Unsigned::toDecimalString() const
{
	std::vector<char> buffer;
	DivisionResult divisionResult = dividedBy(10);
	buffer.push_back(divisionResult.remainder.digits.empty() ? '0' : divisionResult.remainder.digits.front() + '0');
	while (!divisionResult.quotient.digits.empty()) {
		divisionResult = divisionResult.quotient.dividedBy(10);
		buffer.push_back(divisionResult.remainder.digits.empty() ? '0' : divisionResult.remainder.digits.front() + '0');
	}
	return std::string(buffer.rbegin(), buffer.rend());
}

std::string Unsigned::toHexadecimalString() const
{
	std::vector<char> buffer;
	DigitType remainder = digits.empty() ? 0 : digits.front() & 0xf;
	Unsigned quotient = *this >> 4;
	buffer.push_back(remainder < 10 ? '0' + remainder : 'a' + remainder - 10);
	while (!quotient.digits.empty()) {
		remainder = quotient.digits.empty() ? 0 : quotient.digits.front() & 0xf;
		quotient >>= 4;
		buffer.push_back(remainder < 10 ? '0' + remainder : 'a' + remainder - 10);
	}
	return std::string(buffer.rbegin(), buffer.rend());
}

std::string Unsigned::toOctalString() const
{
	std::vector<char> buffer;
	DigitType remainder = digits.empty() ? 0 : digits.front() & 0x7;
	Unsigned quotient = *this >> 3;
	buffer.push_back('0' + remainder);
	while (!quotient.digits.empty()) {
		remainder = quotient.digits.empty() ? 0 : quotient.digits.front() & 0x7;
		quotient >>= 3;
		buffer.push_back('0' + remainder);
	}
	return std::string(buffer.rbegin(), buffer.rend());
}

bool Unsigned::isZero() const noexcept
{
	return digits.empty();
}

void Unsigned::normalize()
{
	while (!digits.empty() && !digits.back()) {
		digits.pop_back();
	}
}

bool operator==(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return lhs.equal(rhs);
}

bool operator!=(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return !lhs.equal(rhs);
}

bool operator<(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return lhs.lessThan(rhs);
}

bool operator<=(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return lhs.lessThanOrEqual(rhs);
}

bool operator>(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return !lhs.lessThanOrEqual(rhs);
}

bool operator>=(const Unsigned& lhs, const Unsigned& rhs) noexcept
{
	return !lhs.lessThan(rhs);
}

} // namespace MultiPrecision
