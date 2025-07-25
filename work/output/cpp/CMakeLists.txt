﻿cmake_minimum_required(VERSION 3.8)
project(AntiAutistLeak)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/lib)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
        message(FATAL_ERROR "GCC version must be at least 6.0!")
    endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s -DNDEBUG /bigobj")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG build/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE build/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG build/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE build/lib)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP8 /wd4102 /wd4101 /EHsc /bigobj")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
else()
    message(AUTHOR_WARNING "You are using an unsupported compiler! Compilation has only been tested with MSVC, GNU, Clang and AppleClang.")
endif()

set(CLASS_FILES )
set(MAIN_FILES jni.h jni_md.h dllmain.cpp)
add_definitions(-DUSE_HOTSPOT=1)

add_library(AntiAutistLeak SHARED ${CLASS_FILES} ${MAIN_FILES} )