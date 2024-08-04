cmake_minimum_required (VERSION 3.20)
# setup C++17 compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ExternalSorting)

# add the library
# will build a static library as libBalancedSort.a
add_library(BalancedSort INTERFACE
        balanced_sort.tpp initial_distribution.tpp utils.hpp)
target_include_directories(BalancedSort INTERFACE .)

# add the library
# will build a static library as libPolyphasicSort.a
add_library(PolyphasicSort INTERFACE
        polyphasic_sort.tpp initial_distribution.tpp utils.hpp)
target_include_directories(PolyphasicSort INTERFACE .)
