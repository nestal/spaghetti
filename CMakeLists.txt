cmake_minimum_required(VERSION 3.6)
project(StatStruct)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp DemoStat.hh)
add_executable(StatStruct ${SOURCE_FILES})
target_link_libraries(StatStruct clang)
