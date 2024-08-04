cmake_minimum_required (VERSION 3.20)
# setup C++17 compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (ExternalSorting)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main
        BalancedSort
        PolyphasicSort
)
target_include_directories(main PUBLIC lib)
