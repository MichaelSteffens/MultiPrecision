//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#include "MultiPrecision/Error.h"

namespace MultiPrecision {

Error::Error(const std::string what) : std::runtime_error(what)
{
}

Error::~Error()
{
}

} // namespace MultiPrecision
