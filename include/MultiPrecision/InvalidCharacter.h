//
// Copyright (c) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_InvalidCharacter_INCLUDED
#define MultiPrecision_InvalidCharacter_INCLUDED

#include "MultiPrecision/Error.h"

namespace MultiPrecision {

class InvalidCharacter : Error
{
public:
	InvalidCharacter(const std::string what);
	virtual ~InvalidCharacter();
};

} // namespace MultiPrecision

#endif // MultiPrecision_InvalidCharacter_INCLUDED
