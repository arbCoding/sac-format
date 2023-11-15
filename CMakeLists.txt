#===============================================================================
# Basic Setup
#===============================================================================
# CMake version
cmake_minimum_required (VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Build type
set(CMAKE_BUILD_TYPE Debug)
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
#===============================================================================
# Flags
#===============================================================================
# Should set presets later on, but this is good enough for now
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors")
# Flags for debug
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -Wextra -Wshadow -Werror")
# Flags for release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
#==============================================================================
# sac-format
#==============================================================================
project(sac-format
  LANGUAGES CXX
  DESCRIPTION "Seismic Analaysis Code Binary File Library"
  HOMEPAGE_URL https://github.com/arbCoding/sac-format
  VERSION 0.9.0)
# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sac-format STATIC
  src/sac_io.hpp
  src/sac_io.cpp
  src/sac_stream.hpp
  src/sac_stream.cpp)

set_target_properties(sac-format PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(sac-format PROPERTIES PUBLIC_HEADER src/sac_stream.hpp)
#===============================================================================
# Unit tests
#===============================================================================
# Catch2 stuff
# Doesn't like Weffc++
add_executable(utests
  src/sac_stream.hpp
  src/utests.cpp)

target_link_libraries(utests
  PRIVATE Catch2::Catch2WithMain
  sac-format)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(utests)
