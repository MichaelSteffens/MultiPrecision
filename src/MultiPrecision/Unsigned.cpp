//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Unsigned.h"
#include "MultiPrecision/DivisionByZero.h"
#include "MultiPrecision/UnsignedUnderflow.h"
#include <iostream>
#include <limits>
#include <type_traits>

namespace MultiPrecision {

using DigitPairType = unsigned int;

static_assert(std::is_integral<DigitPairType>::value);
static_assert(std::is_unsigned<DigitPairType>::value);
static_assert(std::numeric_limits<DigitPairType>::max() >= 2 * std::numeric_limits<Unsigned::DigitType>::max());

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

namespace {

void addDigits(
	const std::vector<Unsigned::DigitType>& lhs,
	const std::vector<Unsigned::DigitType>& rhs,
	std::vector<Unsigned::DigitType>& result)
{
	std::size_t lhsLength = lhs.size();
	std::size_t rhsLength = rhs.size();
	result.resize(std::max(lhsLength, rhsLength));
	DigitPairType carry = 0;
	std::size_t i = 0;
	while (i < std::min(lhsLength, rhsLength)) {
		DigitPairType tmp = carry + lhs[i] + rhs[i];
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < lhsLength) {
		DigitPairType tmp = carry + lhs[i];
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < rhsLength) {
		DigitPairType tmp = carry + rhs[i];
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	if (carry) {
		result.push_back(carry & std::numeric_limits<Unsigned::DigitType>::max());
	}
}

void addDigits(const std::vector<Unsigned::DigitType>& lhs, Unsigned::DigitType rhs, std::vector<Unsigned::DigitType>& result)
{
	std::size_t lhsLength = lhs.size();
	result.resize(std::max(lhsLength + 1, std::size_t(1)));
	DigitPairType carry = 0;
	std::size_t i = 0;
	while (i < std::min(lhsLength, std::size_t(1))) {
		DigitPairType tmp = static_cast<DigitPairType>(lhs[i]) + rhs;
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < lhsLength) {
		DigitPairType tmp = carry + lhs[i];
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < 1) {
		DigitPairType tmp = carry + (i ? 0 : (i ? 0 : rhs));
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		carry = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	if (carry) {
		result.push_back(carry & std::numeric_limits<Unsigned::DigitType>::max());
	}
}

} // namespace

Unsigned Unsigned::plus(const Unsigned& other) const
{
	Unsigned result;
	addDigits(digits, other.digits, result.digits);
	result.normalize();
	return result;
}

Unsigned& Unsigned::add(const Unsigned& other)
{
	addDigits(digits, other.digits, digits);
	normalize();
	return *this;
}

Unsigned& Unsigned::add(DigitType other)
{
	addDigits(digits, other, digits);
	normalize();
	return *this;
}

namespace {

bool subtractDigitsAndTestNegative(
	const std::vector<Unsigned::DigitType>& lhs,
	const std::vector<Unsigned::DigitType>& rhs,
	std::vector<Unsigned::DigitType>& result)
{
	std::size_t lhsLength = lhs.size();
	std::size_t rhsLength = rhs.size();
	result.resize(std::max(lhsLength, rhsLength));
	DigitPairType borrow = 0;
	std::size_t i = 0;
	while (i < std::min(lhsLength, rhsLength)) {
		DigitPairType tmp = lhs[i] - (rhs[i] + borrow);
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < lhsLength) {
		DigitPairType tmp = lhs[i] - borrow;
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < rhsLength) {
		DigitPairType tmp = 0 - (rhs[i] + borrow);
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	return borrow != 0;
}

bool subtractDigitsAndTestNegative(
	const std::vector<Unsigned::DigitType>& lhs,
	Unsigned::DigitType rhs,
	std::vector<Unsigned::DigitType>& result)
{
	std::size_t lhsLength = lhs.size();
	result.resize(std::max(lhsLength, std::size_t(1)));
	DigitPairType borrow = 0;
	std::size_t i = 0;
	while (i < std::min(lhsLength, std::size_t(1))) {
		DigitPairType tmp = static_cast<DigitPairType>(lhs[0]) - rhs;
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < lhsLength) {
		DigitPairType tmp = lhs[i] - borrow;
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	while (i < 1) {
		DigitPairType tmp = 0 - ((i ? 0 : rhs) + borrow);
		result[i] = tmp & std::numeric_limits<Unsigned::DigitType>::max();
		borrow = tmp >> std::numeric_limits<Unsigned::DigitType>::digits ? 1 : 0;
		++i;
	}
	return borrow != 0;
}

} // namespace

Unsigned Unsigned::minus(const Unsigned& other) const
{
	Unsigned result;
	if (subtractDigitsAndTestNegative(digits, other.digits, result.digits)) {
		throw UnsignedUnderflow("Unsigned::minus(const Unsigned&): result is negative!");
	}
	result.normalize();
	return result;
}

Unsigned& Unsigned::subtract(const Unsigned& other)
{
	if (subtractDigitsAndTestNegative(digits, other.digits, digits)) {
		throw UnsignedUnderflow("Unsigned::subtract(const Unsigned&): result is negative!");
	}
	normalize();
	return *this;
}

Unsigned& Unsigned::subtract(DigitType other)
{
	if (subtractDigitsAndTestNegative(digits, other, digits)) {
		throw UnsignedUnderflow("Unsigned::subtract(DigitType): result is negative!");
	}
	normalize();
	return *this;
}

Unsigned Unsigned::times(const Unsigned& other) const
{
	std::size_t length = digits.size();
	std::size_t otherLength = other.digits.size();
	Unsigned result;
	result.digits.assign(length + otherLength, 0);
	for (std::size_t j = 0; j < otherLength; ++j) {
		DigitPairType carry = 0;
		for (std::size_t i = 0; i < length; ++i) {
			DigitPairType tmp =
				static_cast<DigitPairType>(digits[i]) * static_cast<DigitPairType>(other.digits[j]) + result.digits[i + j] + carry;
			result.digits[i + j] = tmp & std::numeric_limits<DigitType>::max();
			carry = tmp >> std::numeric_limits<DigitType>::digits;
		}
		result.digits[length + j] = carry;
	}
	result.normalize();
	return result;
}

Unsigned Unsigned::times(DigitType other) const
{
	std::size_t length = digits.size();
	Unsigned result;
	result.digits.assign(length + 1, 0);
	DigitPairType carry = 0;
	for (std::size_t i = 0; i < length; ++i) {
		DigitPairType tmp = static_cast<DigitPairType>(digits[i]) * static_cast<DigitPairType>(other) + result.digits[i] + carry;
		result.digits[i] = tmp & std::numeric_limits<DigitType>::max();
		carry = tmp >> std::numeric_limits<DigitType>::digits;
	}
	result.digits[length] = carry;
	result.normalize();
	return result;
}

/// Algorithm D from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
class Unsigned::DivisionByUnsigned
{
public:
	DivisionByUnsigned(const Unsigned& dividend, const Unsigned& divisor) :
		remainder(dividend),
		divisor(divisor),
		divisorLength(divisor.digits.size())
	{
	}

	Unsigned::DivisionResult operator()()
	{
		expandFraction();
		loopOverQuotientDigits();
		reduceRemainder();
		return normalizedResult();
	}

private:
	static constexpr DigitPairType radix = static_cast<DigitPairType>(1) << std::numeric_limits<DigitType>::digits;

	// Expand remainder (initialized by dividend) and divisor by a common factor power of 2, ensuring that the divisor most
	// significant word is >= radix / 2.
	void expandFraction()
	{
		expansionShift = 0;
		for (DigitType mask = 1 << (std::numeric_limits<DigitType>::digits - 1); !(divisor.digits.back() & mask); mask >>= 1) {
			++expansionShift;
		}
		remainder.shiftLeftBy(expansionShift);
		divisor.shiftLeftBy(expansionShift);
	}

	void loopOverQuotientDigits()
	{
		std::size_t quotientLength = remainder.digits.size() - divisorLength + 1;
		quotient.digits.assign(quotientLength, 0); // Reserve quotient result space, to be populated in reverse order below.
		for (std::size_t i = quotientLength; i--;) {
			calculateQuotientDigit(i);
			multiplyAndSubtract(i);
		};
	}

	void calculateQuotientDigit(std::size_t i)
	{
		remainder.digits.resize(i + divisorLength + 1);
		DigitPairType trialDividend =
			(static_cast<DigitPairType>(remainder.digits[i + divisorLength]) << std::numeric_limits<DigitType>::digits) +
			remainder.digits[i + divisorLength - 1];
		trialQuotient = trialDividend / divisor.digits[divisorLength - 1];
		trialRemainder = trialDividend % divisor.digits[divisorLength - 1];
		testQuotientDigitTooLarge(i);
		if (trialRemainder < radix) {
			testQuotientDigitTooLarge(i);
		}
		quotient.digits[i] = trialQuotient;
	}

	void testQuotientDigitTooLarge(std::size_t i)
	{
		if (trialQuotient == radix ||
			(trialQuotient * divisor.digits[divisorLength - 2]) >
				((trialRemainder << std::numeric_limits<DigitType>::digits) + remainder.digits[i + divisorLength - 2])) {
			--trialQuotient;
			trialRemainder += divisor.digits[divisorLength - 1];
		}
	}

	void multiplyAndSubtract(std::size_t i)
	{
		remainderFragment.digits.assign(remainder.digits.begin() + i, remainder.digits.end());
		if (remainderFragment.subtractAndTestNegative(divisor.times(trialQuotient))) {
			addBack(i);
		}
		remainder.digits.resize(i);
		remainder.digits.insert(remainder.digits.end(), remainderFragment.digits.begin(), remainderFragment.digits.end());
	}

	void addBack(std::size_t i)
	{
		--quotient.digits[i];
		remainderFragment.add(divisor);
		remainderFragment.digits.pop_back();
	}

	// Revert fraction expansion
	void reduceRemainder()
	{
		remainder.shiftRightBy(expansionShift);
	}

	Unsigned::DivisionResult normalizedResult()
	{
		quotient.normalize();
		remainder.normalize();
		return Unsigned::DivisionResult({std::move(quotient), std::move(remainder)});
	}

	Unsigned remainder;
	Unsigned divisor;
	Unsigned quotient;
	std::size_t divisorLength;
	std::size_t expansionShift;
	DigitPairType trialQuotient;
	DigitPairType trialRemainder;
	Unsigned remainderFragment;
};

/// Simplified algorithm D for n == 1 from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
class Unsigned::DivisionByDigitType
{
public:
	DivisionByDigitType(const Unsigned& dividend, DigitType divisor) : remainder(dividend), divisor(divisor)
	{
	}

	Unsigned::DivisionResult operator()()
	{
		loopOverQuotientDigits();
		return normalizedResult();
	}

private:
	static constexpr DigitPairType radix = static_cast<DigitPairType>(1) << std::numeric_limits<DigitType>::digits;

	void loopOverQuotientDigits()
	{
		std::size_t quotientLength = remainder.digits.size();
		quotient.digits.assign(quotientLength, 0); // Reserve quotient result space, to be populated in reverse order below.
		for (std::size_t i = quotientLength; i--;) {
			calculateQuotientDigit(i);
			multiplyAndSubtract(i);
		};
	}

	void calculateQuotientDigit(std::size_t i)
	{
		remainder.digits.resize(i + 2);
		quotient.digits[i] = ((static_cast<DigitPairType>(remainder.digits[i + 1]) << std::numeric_limits<DigitType>::digits) +
							  remainder.digits[i]) /
							 divisor;
	}

	void multiplyAndSubtract(std::size_t i)
	{
		remainderFragment.digits.assign(remainder.digits.begin() + i, remainder.digits.end());
		remainderFragment.subtract(divisor * quotient.digits[i]);
		remainder.digits.resize(i);
		remainder.digits.insert(remainder.digits.end(), remainderFragment.digits.begin(), remainderFragment.digits.end());
	}

	Unsigned::DivisionResult normalizedResult()
	{
		quotient.normalize();
		remainder.normalize();
		return Unsigned::DivisionResult({std::move(quotient), std::move(remainder)});
	}

	Unsigned remainder;
	DigitType divisor;
	Unsigned quotient;
	Unsigned remainderFragment;
};

Unsigned::DivisionResult Unsigned::dividedBy(const Unsigned& other) const
{
	Unsigned::DivisionResult result;
	if (other.digits.empty()) {
		throw DivisionByZero("Unsigned::dividedBy(const Unsigned&): divisor is zero!");
	} else if (other.digits.size() == 1) {
		result = DivisionByDigitType(*this, other.digits[0])();
	} else if (!lessThan(other)) {
		result = DivisionByUnsigned(*this, other)();
	} else {
		result.remainder.digits = digits;
	}
	return result;
}

/// Algorithm D from Donald E. Knuth, "The Art of Computer Programming", 2nd vol, 3rd ed, pp 272.
Unsigned::DivisionResult Unsigned::dividedBy(DigitType other) const
{
	Unsigned::DivisionResult result;
	if (!other) {
		throw DivisionByZero("Unsigned::dividedBy(DigitType): divisor is zero!");
	} else if (digits.size() > 1 || !(digits.empty() || digits[0] < other)) {
		result = DivisionByDigitType(*this, other)();
	} else {
		result.remainder.digits = digits;
	}
	return result;
}

namespace {

void shiftDigitsLeft(const std::vector<Unsigned::DigitType>& digits, std::size_t bits, std::vector<Unsigned::DigitType>& result)
{
	if (!digits.empty()) {
		std::size_t shiftDigits = bits / std::numeric_limits<Unsigned::DigitType>::digits;
		std::size_t shiftBits = bits % std::numeric_limits<Unsigned::DigitType>::digits;
		result.resize(digits.size() + shiftDigits + 1);
		result[result.size() - 1] =
			digits[result.size() - shiftDigits - 2] >> (std::numeric_limits<Unsigned::DigitType>::digits - shiftBits);
		for (std::size_t i = result.size() - 2; i > shiftDigits; --i) {
			result[i] = (digits[i - shiftDigits] << shiftBits) |
						(digits[i - shiftDigits - 1] >> (std::numeric_limits<Unsigned::DigitType>::digits - shiftBits));
		}
		result[shiftDigits] = digits[0] << shiftBits;
		for (std::size_t i = 0; i < shiftDigits; ++i) {
			result[i] = 0;
		}
	} else {
		result.clear();
	}
}

} // namespace

Unsigned& Unsigned::shiftLeftBy(std::size_t bits)
{
	shiftDigitsLeft(digits, bits, digits);
	normalize();
	return *this;
}

Unsigned Unsigned::shiftedLeftBy(std::size_t bits) const
{
	Unsigned result;
	shiftDigitsLeft(digits, bits, result.digits);
	result.normalize();
	return result;
}

namespace {

template<bool resultIsDigits>
void shiftDigitsRight(const std::vector<Unsigned::DigitType>& digits, std::size_t bits, std::vector<Unsigned::DigitType>& result)
{
	std::size_t shiftDigits = bits / std::numeric_limits<Unsigned::DigitType>::digits;
	std::size_t shiftBits = bits % std::numeric_limits<Unsigned::DigitType>::digits;
	if (shiftDigits < digits.size()) {
		if (!resultIsDigits) {
			result.resize(digits.size() - shiftDigits);
		}
		for (std::size_t i = 0; i < digits.size() - shiftDigits - 1; ++i) {
			result[i] = (digits[i + shiftDigits] >> shiftBits) |
						(digits[i + shiftDigits + 1] << (std::numeric_limits<Unsigned::DigitType>::digits - shiftBits));
		}
		result[digits.size() - shiftDigits - 1] = digits[digits.size() - 1] >> shiftBits;
		if (resultIsDigits) {
			result.resize(digits.size() - shiftDigits);
		}
	} else {
		result.clear();
	}
}

} // namespace

Unsigned& Unsigned::shiftRightBy(std::size_t bits)
{
	shiftDigitsRight<true>(digits, bits, digits);
	normalize();
	return *this;
}

Unsigned Unsigned::shiftedRightBy(std::size_t bits) const
{
	Unsigned result;
	shiftDigitsRight<false>(digits, bits, result.digits);
	result.normalize();
	return result;
}

Unsigned Unsigned::fromDecimal(std::string::const_iterator first, std::string::const_iterator last)
{
	Unsigned result;
	for (std::string::const_iterator i = first; i != last; ++i) {
		if (*i >= '0' && *i <= '9') {
			result = result.times(10).add(*i - '0');
		} else {
			throw Error("Unsigned::fromDecimal(std::string::const_iterator, std::string::const_iterator): invalid character");
		}
	}
	return result;
}

Unsigned Unsigned::fromHexadecimal(std::string::const_iterator first, std::string::const_iterator last)
{
	Unsigned result;
	for (std::string::const_iterator i = first; i != last; ++i) {
		if (*i >= '0' && *i <= '9') {
			result.shiftLeftBy(4).add(*i - '0');
		} else if (*i >= 'a' && *i <= 'f') {
			result.shiftLeftBy(4).add(*i - 'a' + 10);
		} else {
			throw Error("Unsigned::fromHexadecimal(std::string::const_iterator, std::string::const_iterator): invalid character");
		}
	}
	return result;
}

Unsigned Unsigned::fromOctal(std::string::const_iterator first, std::string::const_iterator last)
{
	Unsigned result;
	for (std::string::const_iterator i = first; i != last; ++i) {
		if (*i >= '0' && *i <= '7') {
			result.shiftLeftBy(3).add(*i - '0');
		} else {
			throw Error("Unsigned::fromOctal(std::string::const_iterator, std::string::const_iterator): invalid character");
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
	Unsigned quotient = shiftedRightBy(4);
	buffer.push_back(remainder < 10 ? '0' + remainder : 'a' + remainder - 10);
	while (!quotient.digits.empty()) {
		remainder = quotient.digits.empty() ? 0 : quotient.digits.front() & 0xf;
		quotient.shiftRightBy(4);
		buffer.push_back(remainder < 10 ? '0' + remainder : 'a' + remainder - 10);
	}
	return std::string(buffer.rbegin(), buffer.rend());
}

std::string Unsigned::toOctalString() const
{
	std::vector<char> buffer;
	DigitType remainder = digits.empty() ? 0 : digits.front() & 0x7;
	Unsigned quotient = shiftedRightBy(3);
	buffer.push_back('0' + remainder);
	while (!quotient.digits.empty()) {
		remainder = quotient.digits.empty() ? 0 : quotient.digits.front() & 0x7;
		quotient.shiftRightBy(3);
		buffer.push_back('0' + remainder);
	}
	return std::string(buffer.rbegin(), buffer.rend());
}

Unsigned::operator bool() const noexcept
{
	return !digits.empty();
}

bool Unsigned::subtractAndTestNegative(const Unsigned& other)
{
	return subtractDigitsAndTestNegative(digits, other.digits, digits);
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
