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
  VERSION 0.3.0)

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
