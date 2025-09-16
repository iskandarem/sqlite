cmake_minimum_required(VERSION 3.10)
project(sqlite_project)


set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
# Collect all .cpp files in src/
file(GLOB SOURCES "src/*.cpp")
add_executable(sqlite ${SOURCES}) # Add all .cpp files to the executable
