cmake_minimum_required(VERSION 3.15)

project(coffee)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/source)

find_package(GTest REQUIRED)

find_package(SDL3 REQUIRED)

add_executable(coffee source/main.cpp)

add_subdirectory(tests)

target_link_libraries(coffee SDL3)
