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
#include <sstream>

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

std::istream& operator>>(std::istream& in, Unsigned& out)
{
	std::ios_base::fmtflags formatFlags = in.flags();
	if (formatFlags & std::ios::oct) {
		Unsigned::readOctal(in, out);
	} else if (formatFlags & std::ios::dec) {
		Unsigned::readDecimal(in, out);
	} else if (formatFlags & std::ios::hex) {
		Unsigned::readHexadecimal(in, out);
	} else {
		Unsigned::read(in, out);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Unsigned& in)
{
	std::ios_base::fmtflags formatFlags = out.flags();
	if (formatFlags & std::ios::oct) {
		out << in.toOctalString();
	} else if (formatFlags & std::ios::hex) {
		out << in.toHexadecimalString(formatFlags & std::ios::uppercase);
	} else {
		out << in.toDecimalString();
	}
	return out;
}

void Unsigned::read(std::istream& in, Unsigned& out)
{
	char c;
	if (in.get(c)) {
		if (c != '0') {
			in.unget();
			readDecimal(in, out);
		} else if (in.get(c)) {
			if (c != 'x') {
				in.unget();
				readOctal(in, out);
			} else {
				readHexadecimal(in, out);
			}
		}
	}
}

void Unsigned::readDecimal(std::istream& in, Unsigned& out)
{
	out.digits.clear();
	char c;
	while (in.get(c)) {
		if (c >= '0' && c <= '9') {
			out *= 10;
			out += c - '0';
		} else {
			in.unget();
			break;
		}
	}
}

void Unsigned::readHexadecimal(std::istream& in, Unsigned& out)
{
	out.digits.clear();
	char c;
	while (in.get(c)) {
		if (c >= '0' && c <= '9') {
			out <<= 4;
			out += c - '0';
		} else if (c >= 'a' && c <= 'f') {
			out <<= 4;
			out += c - 'a' + 10;
		} else {
			in.unget();
			break;
		}
	}
}

void Unsigned::readOctal(std::istream& in, Unsigned& out)
{
	out.digits.clear();
	char c;
	while (in.get(c)) {
		if (c >= '0' && c <= '7') {
			out <<= 3;
			out += c - '0';
		} else {
			in.unget();
			break;
		}
	}
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

std::string Unsigned::toHexadecimalString(bool uppercase) const
{
	std::vector<char> buffer;
	const char a = uppercase ? 'A' : 'a';
	DigitType remainder = digits.empty() ? 0 : digits.front() & 0xf;
	Unsigned quotient = *this >> 4;
	buffer.push_back(remainder < 10 ? '0' + remainder : a + remainder - 10);
	while (!quotient.digits.empty()) {
		remainder = quotient.digits.empty() ? 0 : quotient.digits.front() & 0xf;
		quotient >>= 4;
		buffer.push_back(remainder < 10 ? '0' + remainder : a + remainder - 10);
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

Unsigned::MinimalBitRange Unsigned::minimalBitRange() const noexcept
{
	return MinimalBitRange(*this);
}

Unsigned::FullBitRange Unsigned::fullBitRange() const noexcept
{
	return FullBitRange(*this);
}

std::size_t Unsigned::mostSignificantBitPosition() const noexcept
{
	std::size_t digitPosition = mostSignificantDigitPosition();
	std::size_t bitPosition = digitPosition * std::numeric_limits<DigitType>::digits;
	if (digitPosition) {
		for (auto mask = DigitType(1) << (std::numeric_limits<DigitType>::digits - 1);
			 mask && (digits[digitPosition - 1] & mask) == 0;
			 mask >>= 1) {
			--bitPosition;
		}
	}
	return bitPosition;
}

void Unsigned::normalize()
{
	while (!digits.empty() && !digits.back()) {
		digits.pop_back();
	}
}

std::size_t Unsigned::mostSignificantDigitPosition() const noexcept
{
	std::size_t digitPosition = digits.size();
	for (auto i = digits.rbegin(); i != digits.rend() && *i == 0; ++i) {
		--digitPosition;
	}
	return digitPosition;
}

} // namespace MultiPrecision
