cmake_minimum_required(VERSION 3.6)
project(spaghetti)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED)

include_directories(${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

file(GLOB LIBCLANGXX_FILES libclx/*.hh libclx/*.cc)
file(GLOB CODEBASE_FILES codebase/*.hh codebase/*.cc)
add_library(clangxx ${LIBCLANGXX_FILES} ${CODEBASE_FILES})

set(SOURCE_FILES main.cpp)
add_executable(spaghetti ${SOURCE_FILES})

target_link_libraries(spaghetti clangxx clang)
