cmake_minimum_required(VERSION 3.14)
project(ExternalSorting)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Add tests
add_executable(TestsBalancedSort TestBalancedSort.cpp)
add_executable(TestsPolyphasicSort TestPolyphasicSort.cpp)

# Point to the header files in lib
target_include_directories(TestsBalancedSort PUBLIC "${CMAKE_SOURCE_DIR}/lib")
target_include_directories(TestsPolyphasicSort PUBLIC "${CMAKE_SOURCE_DIR}/lib")

# Link against library lib and GoogleTest
target_link_libraries(TestsBalancedSort
        PUBLIC BalancedSort
        GTest::gtest_main
)
target_link_libraries(TestsPolyphasicSort
        PUBLIC PolyphasicSort
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestsBalancedSort)
gtest_discover_tests(TestsPolyphasicSort)
