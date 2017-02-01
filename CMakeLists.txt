cmake_minimum_required(VERSION 3.6)
project(spaghetti)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB LIBCLANGXX_FILES libclx/*.hh libclx/*.cc)
file(GLOB CODEBASE_FILES codebase/*.hh codebase/*.cc)
add_library(clangxx ${LIBCLANGXX_FILES} ${CODEBASE_FILES})

file(GLOB GUI_FILES gui/*.hh gui/*.cc)
add_executable(spaghetti-gui ${GUI_FILES})
target_link_libraries(spaghetti-gui clangxx clang Qt5::Widgets)
