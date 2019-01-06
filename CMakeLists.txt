#
# Copyright (C) 2019 Dr. Michael Steffens
#
# SPDX-License-Identifier:	 LGPL-2.1-or-later
#

cmake_minimum_required(VERSION 3.0)
project(MultiPrecision VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

file(GLOB Unsigned_SRCS_G src/MultiPrecision/*.cpp)
add_library(Unsigned ${Unsigned_SRCS_G})
add_library(${PROJECT_NAME}::Unsigned ALIAS Unsigned)
set_target_properties(Unsigned PROPERTIES OUTPUT_NAME unsigned DEBUG_POSTFIX d)

target_include_directories(Unsigned PUBLIC
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>)

install(TARGETS Unsigned EXPORT MultiPrecisionTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
install(EXPORT MultiPrecisionTargets NAMESPACE ${PROJECT_NAME}:: DESTINATION share/MultiPrecision/cmake)
configure_file(cmake/MultiPrecisionConfig.cmake.in MultiPrecisionConfig.cmake @ONLY)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/MultiPrecisionConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/MultiPrecisionConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/MultiPrecisionConfigVersion.cmake
	DESTINATION share/MultiPrecision/cmake)

add_executable(integerTest integerTest.cpp)
target_link_libraries(integerTest
        ${PROJECT_NAME}::Unsigned)

enable_testing()

# On demand build of tests. See https://cmake.org/Wiki/CMakeEmulateMakeCheck
if(NOT "${CMAKE_GENERATOR}" MATCHES "Visual Studio")
	add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process)
else()
	add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process -C $(Configuration))
endif()

add_subdirectory(test EXCLUDE_FROM_ALL)

add_test(NAME UnsignedTest COMMAND unsignedTest)
add_dependencies(check unsignedTest)
