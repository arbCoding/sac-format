#===============================================================================
# Basic Setup
#===============================================================================
# CMake version
cmake_minimum_required (VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Build type
set(CMAKE_BUILD_TYPE Release)
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
# Flags for thread-sanitizer
set(CMAKE_CXX_FLAGS_THREAD "-O0 -ggdb -Wextra -Wshadow -Werror -fsanitize=thread -pthread")
# Flags for Address sanitizer
set(CMAKE_CXX_FLAGS_ADDRESS "-O0 -ggdb -Wextra -Wshadow -Werror -fsanitize=address -fno-omit-frame-pointer")
# Flags for Memory sanitizer
#set(CMAKE_CXX_FLAGS_MEMORY "-O0 -ggdb -Wextra -Wshadow -Werror -fsanitize=memory -fPIE -pie -fno-omit-frame-pointer")
# Flags for release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
#==============================================================================
# sac-format
#==============================================================================
project(sac-format
  LANGUAGES CXX
  DESCRIPTION "C++20 Single-Header SAC File library"
  HOMEPAGE_URL https://arbCoding.github.io/sac-format
  VERSION 0.3.0)

include_directories(${sac-format_SOURCE_DIR}/src)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
