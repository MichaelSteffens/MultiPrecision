//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/UnsignedUnderflow.h"

namespace MultiPrecision {

UnsignedUnderflow::UnsignedUnderflow(const std::string what) : Error(what)
{
}

UnsignedUnderflow::~UnsignedUnderflow()
{
}

} // namespace MultiPrecision
