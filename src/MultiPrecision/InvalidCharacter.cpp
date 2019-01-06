//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/InvalidCharacter.h"

namespace MultiPrecision {

InvalidCharacter::InvalidCharacter(const std::string what) : Error(what)
{
}

InvalidCharacter::~InvalidCharacter()
{
}

} // namespace MultiPrecision
