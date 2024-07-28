cmake_minimum_required (VERSION 3.20)
# setup C++17 compiler
set(CMAKE_CXX_STANDARD 17)

project (ExternalSorting)

add_subdirectory(lib)

add_executable(TestsBalancedSort tests/TestBalancedSort.cpp)
# link against library lib
target_link_libraries(TestsBalancedSort PUBLIC BalancedSort)
# point to the header files in lib
target_include_directories(TestsBalancedSort PUBLIC "lib")

add_executable(TestPolyphasicSort tests/TestPolyphasicSort.cpp)
# link against library lib
target_link_libraries(TestPolyphasicSort PUBLIC PolyphasicSort)
# point to the header files in lib
target_include_directories(TestPolyphasicSort PUBLIC "lib")
