cmake_minimum_required (VERSION 3.20)
# setup C++17 compiler
set(CMAKE_CXX_STANDARD 17)

project(ExternalSorting)

# add the library
# will build a static library as libBalancedSort.a
add_library(BalancedSort INTERFACE
        balanced_sort.tpp)
target_include_directories(BalancedSort INTERFACE .)
