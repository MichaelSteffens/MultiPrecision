//
// Copyright (c) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_DivisionByZero_INCLUDED
#define MultiPrecision_DivisionByZero_INCLUDED

#include "MultiPrecision/Error.h"

namespace MultiPrecision {

class DivisionByZero : Error
{
public:
	DivisionByZero(const std::string what);
	virtual ~DivisionByZero();
};

} // namespace MultiPrecision

#endif // MultiPrecision_DivisionByZero_INCLUDED
