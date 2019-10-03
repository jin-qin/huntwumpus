cmake_minimum_required(VERSION 3.0)

# Project name
project(HuntWumpus)

set (CMAKE_CXX_STANDARD 11)

# The version number
set(HuntWumpus_VERSION_MAJOR 1)
set(HuntWumpus_VERSION_MINOR 0)

aux_source_directory(. DIR_SRCS)

add_executable(huntwumpus ${DIR_SRCS})