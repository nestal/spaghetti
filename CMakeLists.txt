#
#	Copyright © 2017 Wan Wai Ho <me@nestal.net>
#
#	This file is subject to the terms and conditions of the GNU General Public
#	License.  See the file COPYING in the main directory of the spaghetti
#	distribution for more details.
#

cmake_minimum_required(VERSION 3.6)
project(spaghetti VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS filesystem serialization REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(GTest)
find_package(Doxygen)
find_package(Threads)

# find libclang
find_library(LIBCLANG_LIBRARY NAMES clang libclang HINTS ${LIBCLANG_PREFIX})
find_path(LIBCLANG_HEADER clang-c/Index.h HINTS ${LIBCLANG_PREFIX})

if (NOT LIBCLANG_LIBRARY OR NOT LIBCLANG_HEADER)
	message(FATAL_ERROR "Cannot find libclang!")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# compiler specific flags
if (CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
elseif (MSVC)
	# C4530 and C4577 are from STL headers
	set(CMAKE_CXX_FLAGS "/W2 /WX /wd4530 /wd4577 /EHsc")
	add_definitions(-D_USE_MATH_DEFINES)
endif()
include_directories(${Boost_INCLUDE_DIRS} ${LIBCLANG_HEADER})

if (WIN32)
	set(GUI_TYPE WIN32)
	set(RC_FILE resources/spaghetti.rc)
endif()

# get build date
string(TIMESTAMP BUILD_DATE "%Y-%m-%d")

# get build information from git
execute_process(
	COMMAND git log -1 --format=%h
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GIT_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DVERSION="${PROJECT_VERSION}" -DBUILD_DATE="${BUILD_DATE}" -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")

file(GLOB UTIL_FILES       util/*.hh util/*.cc)
file(GLOB LIBCLANGXX_FILES libclx/*.hh libclx/*.cc)
file(GLOB CODEBASE_FILES   codebase/*.hh codebase/*.cc)
file(GLOB PROJECT_FILES    project/*.hh project/*.cc)
add_library(spag ${LIBCLANGXX_FILES} ${CODEBASE_FILES} ${PROJECT_FILES} ${UTIL_FILES})

# the classes in "project" namespace needs QtCore to serialize models to/from JSON files
target_link_libraries(spag ${LIBCLANG_LIBRARY} ${Boost_LIBRARIES} Qt5::Core)

file(GLOB GUI_FILES gui/*.hh gui/*.cc gui/*/*.cc gui/*/*.hh gui/spaghetti.qrc)
add_executable(spaghetti-gui ${GUI_TYPE} ${GUI_FILES} ${RC_FILE})
target_link_libraries(spaghetti-gui spag Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT})

if (GTEST_FOUND)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_definitions(-DSRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

	file(GLOB TEST_FILES */test/*.hh */test/*.cc)
	add_executable(unittest ${TEST_FILES})
	target_link_libraries(unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} spag)
endif(GTEST_FOUND)
