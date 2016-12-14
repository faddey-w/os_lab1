cmake_minimum_required(VERSION 3.6)
project(os_lab1)

set(CMAKE_CXX_STANDARD 11)


set(SRC_LIST
        src/bmp_io.cpp
        src/bmp_io.h)
set(TESTS_LIST
        tests/TestBMPStructs.h)

set(ALL_CODE ${TESTS_LIST} ${SRC_LIST})

add_executable(os_lab1 src/main.cpp ${SOURCE_FILES})

find_package(CxxTest)
set(CXXTEST_TESTGEN_INTERPRETER bash)
include_directories(${CXXTEST_INCLUDE_DIR})
enable_testing()

FUNCTION(PREPEND var prefix)
    SET(listVar "")
    FOREACH(f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH(f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

PREPEND(PATHS_FOR_CXXTEST ${CMAKE_CURRENT_SOURCE_DIR} ${ALL_CODE})
CXXTEST_ADD_TEST(tests testrunner.cpp ${PATHS_FOR_CXXTEST})
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
