cmake_minimum_required (VERSION 3.20)
# setup C++17 compiler
set(CMAKE_CXX_STANDARD 17)

project (ExternalSorting)

add_subdirectory(lib)

add_executable(Tests tests/TestBalancedSort.cpp)
# link against library lib
target_link_libraries(Tests PUBLIC BalancedSort)
# point to the header files in lib
target_include_directories(Tests PUBLIC "lib")

