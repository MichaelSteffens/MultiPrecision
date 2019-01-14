#
# Copyright (C) 2018 Dr. Michael Steffens
#
# SPDX-License-Identifier:	 LGPL-2.1-or-later
#

add_executable(unsignedTest unsignedTest.cpp)
target_include_directories(unsignedTest PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(unsignedTest
        ${PROJECT_NAME}::Unsigned)

add_executable(mathTest mathTest.cpp)
target_include_directories(mathTest PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(mathTest
        ${PROJECT_NAME}::Unsigned)
