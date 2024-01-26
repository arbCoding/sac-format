# Build Instructions {#BuildInstructions}

## Dependencies

### Automatic (CMake)

[Xoshiro-cpp v1.12.0](https://github.com/Reputeless/Xoshiro-cpp) (testing and
benchmarking)

### Manual

[Catch2 v3.4.0](https://github.com/catchorg/Catch2) (testing and benchmarking)

## Building

Building is as easy as cloning the repository, running CMake for your preferred
build tool, and then building.

### GCC

```bash
git clone https://github.com/arbCoding/sac-format.git
cmake --preset gcc-hard-release
cmake --build ./build/release/gcc
```

### Clang

```bash
git clone https://github.com/arbCoding/sac-format.git
cmake --preset clang-hard-release
cmake --build ./build/release/clang
```
