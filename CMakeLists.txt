#===============================================================================
# Basic Setup
#===============================================================================
# CMake version
cmake_minimum_required (VERSION 3.20)
# Build type
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
#===============================================================================
# Fetch
#===============================================================================
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
# Unit testing
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG v3.4.0)
# Random number generation
FetchContent_Declare(xoshiro
  GIT_REPOSITORY https://github.com/Reputeless/Xoshiro-cpp
  GIT_TAG v1.1)
FetchContent_MakeAvailable(Catch2 xoshiro)
# Include Xoshiro since it doesn't have a CMakeLists.txt
include_directories(${xoshiro_SOURCE_DIR})
#==============================================================================
# sac-format
#==============================================================================
project(sac-format
  LANGUAGES CXX
  DESCRIPTION "C++20 Single-Header SAC File library"
  HOMEPAGE_URL https://arbCoding.github.io/sac-format
  VERSION 0.4.0)

include_directories(${sac-format_SOURCE_DIR}/src)

add_library(sac-format STATIC
  src/sac_format.hpp
  src/sac_format.cpp)

set_target_properties(sac-format PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(sac-format PROPERTIES PUBLIC_HEADER src/sac_format.hpp)
#===============================================================================
# Example programs
#===============================================================================
add_executable(list_sac
  src/sac_format.hpp
  src/examples/list_sac.cpp)

target_link_libraries(list_sac
  sac-format)
#===============================================================================
# Benchmarks
#===============================================================================
add_executable(benchmark
  src/sac_format.hpp
  src/util.hpp
  src/benchmark.cpp)

target_link_libraries(benchmark
  PRIVATE Catch2::Catch2WithMain
  sac-format)
#===============================================================================
# Unit tests
#===============================================================================
add_executable(utests
  src/sac_format.hpp
  src/util.hpp
  src/utests.cpp)

target_link_libraries(utests
  PRIVATE Catch2::Catch2WithMain
  sac-format)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(utests)
#===============================================================================
# CPack Installation
#===============================================================================
# Setup components
install(TARGETS sac-format COMPONENT library)
install(TARGETS utests RUNTIME DESTINATION bin/tests COMPONENT tests)
install(TARGETS benchmark RUNTIME DESTINATION bin/tests COMPONENT benchmarks)
install(TARGETS list_sac RUNTIME DESTINATION bin COMPONENT list_sac)
if(CPACK_GENERATOR MATCHES "productbuild")
  # macOS
  set(CPACK_RESOURCE_FILE_README ${sac-format_SOURCE_DIR}/installers/macOS/Readme.txt)
  set(CPACK_RESOURCE_FILE_WELCOME ${sac-format_SOURCE_DIR}/installers/macOS/Welcome.txt)
  install(FILES ${sac-format_SOURCE_DIR}/License.txt DESTINATION . COMPONENT license)
  set(CPACK_RESOURCE_FILE_LICENSE ${sac-format_SOURCE_DIR}/License2.txt)
  set(CPACK_SET_DESTDIR "ON")
else()
  # Windows/Linux
  install(FILES ${sac-format_SOURCE_DIR}/LICENSE DESTINATION . COMPONENT license)
  set(CPACK_RESOURCE_FILE_LICENSE ${sac-format_SOURCE_DIR}/LICENSE)
endif()

if(CPACK_GENERATOR MATCHES "NSIS")
  set(CPACK_RESOURCE_FILE_README ${sac-format_SOURCE_DIR}/installers/Windows/Readme.txt)
  set(CPACK_RESOURCE_FILE_WELCOME ${sac-format_SOURCE_DIR}/installers/Windows/Welcome.txt)
endif()
# Group components
cpack_add_component(library DISPLAY_NAME "sac-format" GROUP development
  DESCRIPTION "sac-format library to use in your own programs.")
cpack_add_component(tests DISPLAY_NAME "Unit/Integration Testing" GROUP misc
  DESCRIPTION "utests.exe - Collection of command line unit and integration \
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
set(CPACK_PACKAGE_VENDOR "Alexander R. Blanchette")
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
include(CPack)
