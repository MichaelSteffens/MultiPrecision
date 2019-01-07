//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Underflow.h"

namespace MultiPrecision {

Underflow::Underflow(const std::string what) : Error(what)
{
}

Underflow::~Underflow()
{
}

} // namespace MultiPrecision
