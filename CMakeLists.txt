cmake_minimum_required(VERSION 3.6)
project(StatStruct)

set(CMAKE_CXX_STANDARD 14)

file(GLOB LIBCLANGXX_FILES libclangxx/*.hh libclangxx/*.cc)
add_library(clangxx ${LIBCLANGXX_FILES})

set(SOURCE_FILES main.cpp DemoStat.hh)
add_executable(StatStruct ${SOURCE_FILES})

target_link_libraries(StatStruct clangxx clang)
