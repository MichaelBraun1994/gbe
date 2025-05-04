cmake_minimum_required(VERSION 3.15)
project(GBE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendors)
add_subdirectory(src)