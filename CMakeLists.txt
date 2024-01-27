#===============================================================================
# Basic Setup
#===============================================================================
# CMake version
cmake_minimum_required (VERSION 3.25)
# Build type
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
#===============================================================================
# Fetch
#===============================================================================
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
# Random number generation
FetchContent_Declare(xoshiro
  GIT_REPOSITORY https://github.com/Reputeless/Xoshiro-cpp
  GIT_TAG v1.1
  SYSTEM)
FetchContent_MakeAvailable(xoshiro)
# Include Xoshiro since it doesn't have a CMakeLists.txt
include_directories(SYSTEM ${xoshiro_SOURCE_DIR})
# Provides location to search for Catch2 installation from source build
# build via:
# git clone https://github.com/catchorg/Catch2.git
# mkdir build && cd build
# cmake .. -DBUILD_TESTING=Off
# make && sudo make install
# Need to figure out how to get this working on Windows as well!
list(APPEND CMAKE_PREFIX_PATH
  "/usr/local/lib64/cmake" # Linux
  "/usr/local/lib/cmake" # macOS
  "C:/Program Files (x86)/Catch2" # Windows
)
find_package(Catch2 3 CONFIG REQUIRED)
#==============================================================================
# sac-format
#==============================================================================
project(sac-format
  LANGUAGES CXX
  DESCRIPTION "C++20 Single-Header SAC File library"
  HOMEPAGE_URL https://arbCoding.github.io/sac-format
  VERSION 0.6.0)

include_directories(${sac-format_SOURCE_DIR}/include)

add_library(sac-format STATIC
  src/sac_format.cpp)

set_target_properties(sac-format PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(sac-format PROPERTIES PUBLIC_HEADER
  include/sac-format/sac_format.hpp)
#===============================================================================
# Example programs
#===============================================================================
add_executable(list_sac src/examples/list_sac.cpp)
target_link_libraries(list_sac sac-format)
#===============================================================================
# Benchmarks
#===============================================================================
add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark
  PRIVATE Catch2::Catch2WithMain
  sac-format)
#===============================================================================
# Unit tests
#===============================================================================
set(basic_sources
  src/tests/binary_conversions.cpp
  src/tests/constants.cpp)
add_executable(basic_tests ${basic_sources})
target_link_libraries(basic_tests
  PRIVATE Catch2::Catch2WithMain
  sac-format)

add_executable(trace_tests
  src/tests/trace.cpp)
target_link_libraries(trace_tests
  PRIVATE Catch2::Catch2WithMain
  sac-format)

add_executable(geometry_tests
  src/tests/geometry.cpp)
target_link_libraries(geometry_tests
  PRIVATE Catch2::Catch2WithMain
  sac-format
)

add_executable(datetime_tests
  src/tests/datetime.cpp)
target_link_libraries(datetime_tests
  PRIVATE Catch2::Catch2WithMain
  sac-format
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

catch_discover_tests(basic_tests)
catch_discover_tests(trace_tests)
catch_discover_tests(geometry_tests)
catch_discover_tests(datetime_tests)
#===============================================================================
# Release preparation
#===============================================================================
if (UNIX AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(Exes basic_tests trace_tests benchmark list_sac)
  foreach(program ${Exes})
    message(STATUS "\n ${program} will be stripped to reduce filesize.\n")
    add_custom_command(TARGET ${program}
      COMMAND strip "$<TARGET_FILE:${program}>"
      VERBATIM)
  endforeach()
endif()
#===============================================================================
# CPack Installation
#===============================================================================
include(CPack)
# Setup components
install(TARGETS sac-format COMPONENT library)
install(TARGETS basic_tests trace_tests geometry_tests datetime_tests
  RUNTIME DESTINATION bin/tests COMPONENT tests)
install(TARGETS benchmark RUNTIME DESTINATION bin/tests COMPONENT benchmarks)
install(TARGETS list_sac RUNTIME DESTINATION bin COMPONENT list_sac)
if(CPACK_GENERATOR MATCHES "productbuild")
  # macOS
  set(CPACK_RESOURCE_FILE_README
    ${sac-format_SOURCE_DIR}/installers/macOS/Readme.txt)
  set(CPACK_RESOURCE_FILE_WELCOME
    ${sac-format_SOURCE_DIR}/installers/macOS/Welcome.txt)
  install(FILES ${sac-format_SOURCE_DIR}/License.txt DESTINATION .
    COMPONENT license)
  set(CPACK_RESOURCE_FILE_LICENSE ${sac-format_SOURCE_DIR}/License2.txt)
  set(CPACK_SET_DESTDIR "ON")
else()
  # Windows/Linux
  install(FILES ${sac-format_SOURCE_DIR}/LICENSE DESTINATION .
    COMPONENT license)
  set(CPACK_RESOURCE_FILE_LICENSE ${sac-format_SOURCE_DIR}/LICENSE)
endif()
# Group components
cpack_add_component(library DISPLAY_NAME "sac-format" GROUP development
  DESCRIPTION "sac-format library to use in your own programs.")
cpack_add_component(tests DISPLAY_NAME "Unit/Integration Testing" GROUP misc
  DESCRIPTION "Collection of command line unit and integration \
tests that ensure sac-format is working correctly.")
cpack_add_component(benchmarks DISPLAY_NAME "Benchmarks" GROUP misc
  DESCRIPTION "benchmarks.exe - Collection of command line benchmarks for \
determining sac-format performance.")
cpack_add_component(list_sac DISPLAY_NAME "list_sac" GROUP programs
  DESCRIPTION "list_sac.exe - Example command line program that lists all \
header information from an input SAC file.")
cpack_add_component(license DISPLAY_NAME "license"
  DESCRIPTION "sac-format license file.")
set(CPACK_COMPONENT_license_HIDDEN TRUE)
set(CPACK_COMPONENT_license_REQUIRED TRUE)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "sac-format")
set(CPACK_PACKAGE_VENDOR "Alexander R. Blanchette <arbCoding@gmail.com>")
# Allow separating components instead of monolithic
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
# Meta-data
set(CPACK_PACKAGE_NAME "sac-format")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDER "https://arbCoding.github.io/sac-format/")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://arbCoding.github.io/sac-format/")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "C++20 Single-Header SAC File library")
# Filename/installation directory
set(CPACK_PACKAGE_FILE_NAME "sac-format")
# Archive filename
set(CPACK_ARCHIVE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-\
${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
# Make checksum
set(CPACK_PACKAGE_CHECKSUM SHA512)
# Only specified components (exclude catch2)
set(CPACK_COMPONENTS_ALL library tests benchmarks list_sac license)
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)
# Windows Specific NSIS installer
set(CPACK_NSIS_HELP_LINK "https://arbCoding.github.io/sac-format/")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
# Debian Specific
# Installs into /usr (tests go into /usr/bin/tests)
# libsac-format.a goes into /usr/lib64
# sac-format.hpp goes into /usr/include
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alexander R. Blanchette \
<arbCoding@gmail.com>")
# RedHat Specific
# Installs into /usr (tests go into /usr/bin/tests)
# libsac-format.a goes into /usr/lib64
# sac-format.hpp goes into /usr/include
set(CPACK_RPM_COMPONENT_INSTALL ON)
# Don't strip binaries
set(CPACK_RPM_SPEC_MORE_DEFINE "%define __spec_install_post /bin/true")
