# @date 2024-10-31 (last modification)
cmake_minimum_required(VERSION 3.27)
project(merry_tools_tests)

set(CMAKE_CXX_STANDARD 17)

set( INCLUDE "inc" )
set( SOURCES "src" )

include_directories( "inc" )

add_executable( merry_tests
        #inc/
        "${INCLUDE}/ios_benders.h"
        "${INCLUDE}/mem_guard.h"
        "${INCLUDE}/mth_fix_float.h"
        "${INCLUDE}/mth_vectors.h"
        #src/
        "${SOURCES}/ios_benders.cpp"
        #tests/
        "tests/main.cpp"
        inc/mem_unique_val.h
        src/test_uniques.cpp
)
