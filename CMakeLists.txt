# @date 2026-05-12 (last modification)
cmake_minimum_required(VERSION 3.27)
project(merry_tools_tests)

set(CMAKE_CXX_STANDARD 20)

set( INCLUDE "inc" )
set( SOURCES "src" )

include_directories( "inc" )

add_executable( merry_tests
        #inc/
        "${INCLUDE}/ios_benders.h"
        "${INCLUDE}/lgc_boolean.h"
        "${INCLUDE}/mem_guard.h"
        "${INCLUDE}/mem_unique_val.h"
        "${INCLUDE}/mth_to.h"
        "${INCLUDE}/mth_fix_float.h"
        "${INCLUDE}/mth_vectors.h"
        "${INCLUDE}/rust_like.hpp"
        #src/
        "${SOURCES}/ios_benders.cpp"
        "${SOURCES}/lgc_boolean.cpp"
        #tests/
        "tests/main.cpp"
        "tests/testuniq/test_uniques.cpp"
        "tests/safecasttests/safecasttests.cpp"
)

inc/mth_to.hpp
