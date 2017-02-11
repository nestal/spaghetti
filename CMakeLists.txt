#
#	Copyright © 2017 Wan Wai Ho <me@nestal.net>
#
#	This file is subject to the terms and conditions of the GNU General Public
#	License.  See the file COPYING in the main directory of the spaghetti
#	distribution for more details.
#

cmake_minimum_required(VERSION 3.6)
project(spaghetti)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(GTest)
find_package(Doxygen)
find_package(Threads)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB LIBCLANGXX_FILES libclx/*.hh libclx/*.cc)
file(GLOB CODEBASE_FILES   codebase/*.hh codebase/*.cc)
file(GLOB PROJECT_FILES    project/*.hh project/*.cc project/*.h project/*.cpp)
add_library(spag ${LIBCLANGXX_FILES} ${CODEBASE_FILES} ${PROJECT_FILES})
target_link_libraries(spag clang ${Boost_LIBRARIES})

file(GLOB GUI_FILES gui/*.hh gui/*.cc gui/*/*.cc gui/*/*.hh)
add_executable(spaghetti-gui ${GUI_FILES})
target_link_libraries(spaghetti-gui spag Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT})

if (GTEST_FOUND)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_definitions(-DSRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

	file(GLOB TEST_FILES */test/*.hh */test/*.cc)
	add_executable(unittest ${TEST_FILES})
	target_link_libraries(unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} spag)
endif(GTEST_FOUND)
