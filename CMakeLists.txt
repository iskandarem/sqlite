cmake_minimum_required(VERSION 3.10)
project(sqlite)

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0135 NEW)

# Core library
file(GLOB CORE_SOURCES "src/*.cpp")
add_library(sqlite_core ${CORE_SOURCES})
target_include_directories(sqlite_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sqlite_core)

# Enable GoogleTest
enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Tests
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(sqlite_tests ${TEST_SOURCES})
target_link_libraries(sqlite_tests
    PRIVATE sqlite_core GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sqlite_tests)
