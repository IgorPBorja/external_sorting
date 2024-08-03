cmake_minimum_required (VERSION 3.20)
# setup C++17 compiler
set(CMAKE_CXX_STANDARD 17)

project (ExternalSorting)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)
