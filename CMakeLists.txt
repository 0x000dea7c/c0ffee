cmake_minimum_required(VERSION 3.15)

project(coffee)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(SOURCE_DIRECTORY ${CMAKE_SOURCE_DIR}/source)

find_package(SDL3 REQUIRED)

add_executable(coffee source/main.cpp)

target_link_libraries(coffee SDL3 SDL3_ttf)
