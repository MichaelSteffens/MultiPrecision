//
// Copyright (c) 2019 Dr. Michael Steffens
//
// SPDX-License-Identifier:	 LGPL-2.1-or-later
//


#ifndef MultiPrecision_Error_INCLUDED
#define MultiPrecision_Error_INCLUDED

#include <stdexcept>

namespace MultiPrecision {

class Error : public std::runtime_error
{
public:
	Error(const std::string what);
	virtual ~Error();
};

} // namespace MultiPrecision

#endif // MultiPrecision_Error_INCLUDED
