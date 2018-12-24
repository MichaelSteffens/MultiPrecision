//
// Copyright (c) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_UnsignedUnderflow_INCLUDED
#define MultiPrecision_UnsignedUnderflow_INCLUDED

#include "MultiPrecision/Error.h"

namespace MultiPrecision {

class UnsignedUnderflow : Error
{
public:
	UnsignedUnderflow(const std::string what);
	virtual ~UnsignedUnderflow();
};

} // namespace MultiPrecision

#endif // MultiPrecision_UnsignedUnderflow_INCLUDED
