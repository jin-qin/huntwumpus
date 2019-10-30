cmake_minimum_required(VERSION 3.0)

# Project name
project(HuntWumpus)

include(CTest)

set (CMAKE_CXX_STANDARD 11)

# The version number
set(HuntWumpus_VERSION_MAJOR 1)
set(HuntWumpus_VERSION_MINOR 0)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./json DIR_JSONCPP)

add_executable(huntwumpus ${DIR_SRCS} ${DIR_JSONCPP})

set (DIR_SRCS_NO_MAIN ${DIR_SRCS})
list(REMOVE_ITEM DIR_SRCS_NO_MAIN "./main.cpp")

add_executable(test_case_0 ${DIR_SRCS_NO_MAIN} ${DIR_JSONCPP} unittests/test_case_0.cpp)

add_test( test_case_0 test_case_0)