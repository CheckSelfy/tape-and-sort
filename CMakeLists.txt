cmake_minimum_required(VERSION 3.27)
project(tape_and_sort)

set(CMAKE_CXX_STANDARD 20) # using concepts
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources
        src/*.cpp
)

list(FILTER sources EXCLUDE REGEX "main.cpp")

add_executable(tape_and_sort
        ${sources}
        src/main.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB tests test/*.cpp)

add_executable(
        tape_test
        ${tests}
        ${sources}
)

target_link_libraries(
        tape_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tape_test)
