//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/DivisionByZero.h"

namespace MultiPrecision {

DivisionByZero::DivisionByZero(const std::string what) : Error(what)
{
}

DivisionByZero::~DivisionByZero()
{
}

} // namespace MultiPrecision
