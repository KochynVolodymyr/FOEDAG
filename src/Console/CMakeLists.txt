# -*- mode:cmake -*-

# Copyright 2021 The Foedag team

# GPL License

# Copyright (c) 2021 The Open-Source FPGA Foundation

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.15)

project(console LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
else()
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")
endif()

include (../../cmake/cmake_qt.txt)
include (../../cmake/cmake_tcl.txt)

include_directories(${PROJECT_SOURCE_DIR}/../../src ${PROJECT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/../../include/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../lib)

set (SRC_CPP_LIST
  TclConsoleWidget.cpp
  ConsoleInterface.cpp
  TclConsole.cpp
  TclWorker.cpp
  StreamBuffer.cpp
  FileInfo.cpp
  SearchWidget.cpp
  TclConsoleBuilder.cpp
  ConsoleDefines.cpp
  OutputFormatter.cpp
  DummyParser.cpp
  ConsolePlugin.cpp
)

set (SRC_H_LIST
  TclConsoleWidget.h
  ConsoleInterface.h
  TclConsole.h
  TclWorker.h
  StreamBuffer.h
  FileInfo.h
  SearchWidget.h
  TclConsoleBuilder.h
  ConsoleDefines.h
  OutputFormatter.h
  DummyParser.h
  ConsolePlugin.h
)

set (SRC_UI_LIST
)

add_library(console STATIC
  ${SRC_CPP_LIST}
  ${SRC_H_LIST}
  ${SRC_UI_LIST}
)

target_link_libraries(console PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui)
target_compile_definitions(console PRIVATE CONSOLE_LIBRARY)

install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/../../lib/$<TARGET_FILE_NAME:console>
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/foedag)

install(
   FILES ${PROJECT_SOURCE_DIR}/../Console/TclConsoleWidget.h
         ${PROJECT_SOURCE_DIR}/../Console/StreamBuffer.h
         ${PROJECT_SOURCE_DIR}/../Console/ConsoleInterface.h
         ${PROJECT_SOURCE_DIR}/../Console/TclConsole.h
         ${PROJECT_SOURCE_DIR}/../Console/TclConsoleBuilder.h
         ${PROJECT_SOURCE_DIR}/../Console/OutputFormatter.h
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/foedag/Console)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../bin)

set(CMAKE_CXX_FLAGS_DEBUG
"${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} -Werror -Wall -O0 -g ${MSYS_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS} ${MEM_SANITIZER_FLAGS}")

add_library(console_testing_lib STATIC
  ${PROJECT_SOURCE_DIR}/../Console/Test/ConsoleTestUtils.h
  ${PROJECT_SOURCE_DIR}/../Console/Test/ConsoleTestUtils.cpp
  ${PROJECT_SOURCE_DIR}/../Console/Test/TestingUtils.h
  ${PROJECT_SOURCE_DIR}/../Console/Test/TestingUtils.cpp
)
target_link_libraries(console_testing_lib foedag foedagcore compiler tcl_stubb #[[tcl_static]] console)

#add_library(zlib SHARED IMPORTED)
#set_target_properties(zlib_shared PROPERTIES
#  IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/lib/libz_shared.so)

#add_library(tcl_shared SHARED IMPORTED)
#set_target_properties(tcl_shared PROPERTIES
#  IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/../../lib/libtcl9.0.so)

add_library(console-plugin SHARED
  ${SRC_CPP_LIST}
  ${SRC_H_LIST}
  ${PROJECT_SOURCE_DIR}/../Console/ConsolePlugin.cpp)
#target_link_directories(console-plugin PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../lib)
#target_link_libraries(console-plugin -ltcl9.0)
target_link_libraries(console-plugin tcl_stubb tcl_shared zlib)
target_link_libraries(console-plugin foedag foedagcore Qt5::Widgets Qt5::Core Qt5::Gui QConsole)
add_definitions(-DCONSOLEPLUGIN_LIBRARY)
set_target_properties(console-plugin PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../plugins)
target_link_directories(console-plugin PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../lib)


add_executable(console_test
  ${PROJECT_SOURCE_DIR}/../Console/Test/console_main.cpp
  ${PROJECT_SOURCE_DIR}/../Console/Test/console_test.cpp)
target_link_libraries(console_test console_testing_lib foedag foedagcore compiler tcl_stubb #[[tcl_static]] console)
set_target_properties(console_test PROPERTIES OUTPUT_NAME console_test)

target_link_directories(console_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../lib)
add_executable(console_debug
  ${PROJECT_SOURCE_DIR}/../Console/Test/console_standalone.cpp)
target_link_libraries(console_debug foedag foedagcore compiler tcl_stubb #[[tcl_static]] console -ltcl9.0)
set_target_properties(console_debug PROPERTIES OUTPUT_NAME console)
#target_link_directories(console_debug PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
#target_link_directories(console_test PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_directories(console_debug PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../../lib)
