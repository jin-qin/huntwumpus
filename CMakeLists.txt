cmake_minimum_required(VERSION 3.0)

# Project name
project(HuntWumpus)

include(CTest)

set (CMAKE_CXX_STANDARD 11)

# The version number
set(HuntWumpus_VERSION_MAJOR 1)
set(HuntWumpus_VERSION_MINOR 0)

file(GLOB INCS "*.h")
file(GLOB SRCS "*.cpp")
file(GLOB INCS_JSON "./json/*.h")

add_executable(huntwumpus ${INCS} ${SRCS} ${INCS_JSON})

# Test cases
add_test(test_case_0 huntwumpus "../test_data/maps/map_0.json")
add_test(test_case_1 huntwumpus "../test_data/maps/map_1.json")
add_test(test_case_2 huntwumpus "../test_data/maps/map_2.json")