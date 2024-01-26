# Quickstart {#Quickstart}

To use link to the library (`libsac-format.a` on Linux/macOS, `sac-format.lib`
on Windows) and include `sac_format.hpp`.

## Example Programs

### list_sac

`list_sac` is a command line program that takes a single SAC-file as its input
argument. It reads the SAC-file and outputs the header/footer information, as
well as the true size of the [`data1`](#%3Ddata1%3D) and [`data2`](#%3Ddata2%3D)
vectors.

## CMake Integration

To integrate sac-format into your CMake project, add it to your
`CMakeLists.txt`.

```cmake
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(sac-format
    GIT_REPOSITORY https://github.com/arbCoding/sac-format
    GIT_TAG vX.X.X)
FetchContent_MakeAvailable(sac-format)
include_directory(${sacformat_SOURCE_DIR/src})

project (your_project
    LANGUAGES CXX)

add_executable(your_executable
    your_sources
    sac_format.hpp)

target_link_libraries_library(your_executable
    PRIVATE sac-format)
```

## Example

### Reading and Writing

```cpp
#include <sac_format.hpp>
#include <filesystem>
#include <iostream>

using namespace sacfmt;
namespace fs = std::filesystem;

int main() {
    Trace trace1{};
    // Change header variable
    trace1.kstnm("Station1");
    fs::path file{"./test.SAC"};
    // Write
    trace1.write(file);
    // Read
    Trace trace2 = Trace(file);
    // Confirm equality
    std::cout << (trace1 == trace2) << '\n';
    fs::remove(file);
    return EXIT_SUCCESS;
}
```
