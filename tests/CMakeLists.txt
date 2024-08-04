cmake_minimum_required(VERSION 3.14)
project(ExternalSorting)

# GoogleTest requires at least C++17
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

# Add test library / mock
add_library(RandomFixtures STATIC RandomDataFixture.cpp)

# Add tests
add_executable(TestBalancedSort TestBalancedSort.cpp)
add_executable(TestPolyphasicSort TestPolyphasicSort.cpp)
add_executable(TestInitialDistribution TestInitialDistribution.cpp)

# Point to the header files in lib
target_include_directories(TestBalancedSort PUBLIC "${CMAKE_SOURCE_DIR}/lib")
target_include_directories(TestPolyphasicSort PUBLIC "${CMAKE_SOURCE_DIR}/lib")
target_include_directories(TestInitialDistribution PUBLIC "${CMAKE_SOURCE_DIR}/lib")

# Link against library lib and GoogleTest
target_link_libraries(TestBalancedSort
        PUBLIC BalancedSort
        PUBLIC RandomFixtures
        GTest::gtest_main
)
target_link_libraries(TestPolyphasicSort
        PUBLIC PolyphasicSort
        PUBLIC RandomFixtures
        GTest::gtest_main
)
target_link_libraries(TestInitialDistribution
        PUBLIC PolyphasicSort  # so initial_distribution gets pulled in
        GTest::gtest_main
)

add_test(TestBalancedSort TestBalancedSort)
add_test(TestPolyphasicSort TestPolyphasicSort)
add_test(TestInitialDistribution TestInitialDistribution)
