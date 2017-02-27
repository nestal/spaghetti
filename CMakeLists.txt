#
#	Copyright © 2017 Wan Wai Ho <me@nestal.net>
#
#	This file is subject to the terms and conditions of the GNU General Public
#	License.  See the file COPYING in the main directory of the spaghetti
#	distribution for more details.
#

cmake_minimum_required(VERSION 3.6)
project(spaghetti VERSION 0.2)

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

# look like mingw doesn't need this either
if (MSVC)
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
install(TARGETS spaghetti-gui RUNTIME DESTINATION bin)

if (GTEST_FOUND)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_definitions(-DSRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

	file(GLOB TEST_FILES */test/*.hh */test/*.cc)
	add_executable(unittest ${TEST_FILES})
	target_link_libraries(unittest ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} spag)
endif(GTEST_FOUND)

if(WIN32)
	# NSIS specific settings
	set(CPACK_GENERATOR "NSIS")
	set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/fork.ico")
	set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/fork.ico")
	set(CPACK_NSIS_CONTACT "me@nestal.net")
	set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")

	# install dependent libraries
	find_file(LIBCLANG_DLL libclang.dll HINTS ${LIBCLANG_PREFIX}/bin)
	install(FILES ${LIBCLANG_DLL} DESTINATION bin)

	get_target_property(QtCore_location_Release     Qt5::Core    LOCATION_Release)
	get_filename_component(QT_BINARY_DIR "${QtCore_location_Release}" DIRECTORY)

	install(FILES
		${QT_BINARY_DIR}/Qt5Core.dll
		${QT_BINARY_DIR}/Qt5Widgets.dll
		${QT_BINARY_DIR}/Qt5Gui.dll
		${QT_BINARY_DIR}/d3dcompiler_47.dll
		${QT_BINARY_DIR}/libEGL.dll
		${QT_BINARY_DIR}/libGLESV2.dll
		${QT_BINARY_DIR}/opengl32sw.dll

		DESTINATION bin
	)
	set(QT_PLUGINS_DIR "${QT_BINARY_DIR}/../plugins")
	install(FILES
		${QT_PLUGINS_DIR}/iconengines/qsvgicon.dll
		DESTINATION bin/iconengines
	)
	install(FILES
		${QT_PLUGINS_DIR}/platforms/qwindows.dll
		DESTINATION bin/platforms
	)
	install(FILES
		${QT_PLUGINS_DIR}/imageformats/qdds.dll
		${QT_PLUGINS_DIR}/imageformats/qgif.dll
		${QT_PLUGINS_DIR}/imageformats/qicns.dll
		${QT_PLUGINS_DIR}/imageformats/qico.dll
		${QT_PLUGINS_DIR}/imageformats/qjpeg.dll
		${QT_PLUGINS_DIR}/imageformats/qsvg.dll
		${QT_PLUGINS_DIR}/imageformats/qtga.dll
		${QT_PLUGINS_DIR}/imageformats/qtiff.dll
		${QT_PLUGINS_DIR}/imageformats/qwbmp.dll
		${QT_PLUGINS_DIR}/imageformats/qwebp.dll
		DESTINATION bin/imageformats
	)
else()
	set(CPACK_GENERATOR "RPM")
endif()

set(CPACK_PACKAGE_EXECUTABLES "spaghetti-gui" "Spaghetti: source code analyzer")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Spaghetti: a tool to quickly understand spaghetti code")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_NAME "spaghetti-gui")
set(CPACK_PACKAGE_VENDOR "nestal.net")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")

include(CPack)
