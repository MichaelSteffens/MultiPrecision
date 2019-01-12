//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/InvalidCharacter.h"
#include "MultiPrecision/Unsigned.h"
#include <cstring>
#include <istream>
#include <streambuf>
#include <string>

namespace MultiPrecision {

class Unsigned::StringReadBuffer : public std::streambuf
{
public:
	StringReadBuffer(const char* s)
	{
		char* inputSequence = const_cast<char*>(s);
		setg(inputSequence, inputSequence, inputSequence + std::strlen(s));
	}

	StringReadBuffer(const std::string& s)
	{
		char* inputSequence = const_cast<char*>(s.c_str());
		setg(inputSequence, inputSequence, inputSequence + s.size());
	}

	StringReadBuffer(std::string::const_iterator first, std::string::const_iterator last)
	{
		char* inputSequence = const_cast<char*>(&(*first));
		setg(inputSequence, inputSequence, inputSequence + (last - first));
	}
};

Unsigned::Unsigned(const char* number)
{
	StringReadBuffer buffer(number);
	std::istream in(&buffer);
	read(in, *this);
	if (in) {
		throw InvalidCharacter("Unsigned::Unsigned(const char*): invalid input!");
	}
}

Unsigned::Unsigned(const std::string& number)
{
	StringReadBuffer buffer(number);
	std::istream in(&buffer);
	read(in, *this);
	if (in) {
		throw InvalidCharacter("Unsigned::Unsigned(const std::string&): invalid input!");
	}
}

Unsigned Unsigned::fromDecimal(std::string::const_iterator first, std::string::const_iterator last)
{
	StringReadBuffer buffer(first, last);
	std::istream in(&buffer);
	Unsigned result;
	readDecimal(in, result);
	if (in) {
		throw InvalidCharacter("Unsigned::fromDecimal(std::string::const_iterator, std::string::const_iterator): invalid input!");
	}
	return result;
}

Unsigned Unsigned::fromHexadecimal(std::string::const_iterator first, std::string::const_iterator last)
{
	StringReadBuffer buffer(first, last);
	std::istream in(&buffer);
	Unsigned result;
	readHexadecimal(in, result);
	if (in) {
		throw InvalidCharacter(
			"Unsigned::fromHexadecimal(std::string::const_iterator, std::string::const_iterator): invalid input!");
	}
	return result;
}

Unsigned Unsigned::fromOctal(std::string::const_iterator first, std::string::const_iterator last)
{
	StringReadBuffer buffer(first, last);
	std::istream in(&buffer);
	Unsigned result;
	readOctal(in, result);
	if (in) {
		throw InvalidCharacter("Unsigned::fromOctal(std::string::const_iterator, std::string::const_iterator): invalid input!");
	}
	return result;
}


} // namespace MultiPrecision
