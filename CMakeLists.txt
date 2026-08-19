#   SiddiqSoft CMakeLists 
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file
#
#   When building from our CI server, the following options must be set
#   BUILD_TESTS ON
#   CI_BUILDID must be set to the gitversion
# 
cmake_minimum_required(VERSION 3.29)

# _________________
# Version detection; must preceed the declaration of the actual project()
# Figures out the value ${CURRENT_PROJECT_VERSION} and therefore the ${PROJECT_VERSION}
# Brings in the CPM module into pack/CPM.make
if(WIN32)
    if(DEFINED ENV{LOCALAPPDATA})
        set(CPM_SOURCE_CACHE "$ENV{LOCALAPPDATA}/CPM/.cpmcache")
    elseif(DEFINED ENV{USERPROFILE})
        set(CPM_SOURCE_CACHE "$ENV{USERPROFILE}/AppData/Local/CPM/.cpmcache")
    else()
        set(CPM_SOURCE_CACHE "${CMAKE_BINARY_DIR}/.cpmcache")
    endif()
else()
    set(CPM_SOURCE_CACHE "$ENV{HOME}/.cache/.cpmcache")
endif()
set(CPM_SOURCE_CACHE ${CPM_SOURCE_CACHE} CACHE PATH "CPM source cache")
include(pack/CMakeCommonHelpers.cmake)

# ______________________
# Project Metadata block
project(AzureCppUtils
        VERSION         "${CURRENT_PROJECT_VERSION}"
        LANGUAGES       CXX
        HOMEPAGE_URL    "%%myurl%%"
        DESCRIPTION     "Azure REST API Helpers for Modern C++. Featuring helper functions for date-time, encryption tokens for Azure REST API as a single header-only package. Using OpenSSL on UNIX and native Win32 on Windows.")

# Set C++ standard globally
set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(${PROJECT_NAME}_BUILD_TESTS "Build tests for ${PROJECT_NAME}" OFF)
option(CI_BUILDID "Build ID for CI builds" "0.0.0")

# ____________________________________
#  Library Definition
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                            INTERFACE
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_options(${PROJECT_NAME} INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

# ____________________________________
# Dependencies
find_package(OpenSSL 3.5 REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} INTERFACE OpenSSL::SSL OpenSSL::Crypto)

# Install natvis file for Visual Studio debugger visualization
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/pack/SiddiqSoft.AzureCppUtils.natvis
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
        COMPONENT natvis
        OPTIONAL)

CPMAddPackage("gh:siddiqsoft/RunOnEnd#1.4.5")
target_link_libraries(${PROJECT_NAME} INTERFACE RunOnEnd::RunOnEnd)
CPMAddPackage("gh:siddiqsoft/StringHelpers#1.2.2")
target_link_libraries(${PROJECT_NAME} INTERFACE StringHelpers::StringHelpers)

# ____________________________________
# Testing
# Available only when building in our repo; controlled by the switch: ${PROJECT_NAME}_BUILD_TESTS = ON
if(${${PROJECT_NAME}_BUILD_TESTS} AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests" )
    message(STATUS "${PROJECT_NAME} - Asked to build tests.. ${PROJECT_NAME}_BUILD_TESTS = ${${PROJECT_NAME}_BUILD_TESTS}")
    enable_testing()
    add_subdirectory(tests)
endif()
