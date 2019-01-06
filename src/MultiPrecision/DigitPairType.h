//
// Copyright (C) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_DigitPairType_INCLUDED
#define MultiPrecision_DigitPairType_INCLUDED

#include "MultiPrecision/Unsigned.h"
#include <limits>
#include <type_traits>

namespace MultiPrecision {

using DigitPairType = unsigned long;

static_assert(std::is_integral<DigitPairType>::value);
static_assert(std::is_unsigned<DigitPairType>::value);
static_assert(std::numeric_limits<DigitPairType>::digits >= 2 * std::numeric_limits<Unsigned::DigitType>::digits);

} // namespace MultiPrecision

#endif // MultiPrecision_DigitPairType_INCLUDED
