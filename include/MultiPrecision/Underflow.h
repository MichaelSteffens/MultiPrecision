//
// Copyright (c) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_Underflow_INCLUDED
#define MultiPrecision_Underflow_INCLUDED

#include "MultiPrecision/Error.h"

namespace MultiPrecision {

class Underflow : Error
{
public:
	Underflow(const std::string what);
	virtual ~Underflow();
};

} // namespace MultiPrecision

#endif // MultiPrecision_Underflow_INCLUDED
