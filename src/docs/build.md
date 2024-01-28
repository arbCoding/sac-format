# Build Instructions {#BuildInstructions}

## Dependencies

### Automatic (CMake)

[Xoshiro-cpp v1.12.0](https://github.com/Reputeless/Xoshiro-cpp) (testing and
benchmarking).

### Manual

[Catch2 v3.4.0](https://github.com/catchorg/Catch2) (testing and benchmarking).
Note that this is automatic on Windows (not Linux nor macOS).

#### macOS and Linux

```bash
git clone https://github.com/catchorg/Catch2.git
cd Catch2
git checkout v3.5.2
cmake -Bbuild -S. -DBUILD_TESTING=OFF
sudo cmake --build ./build/ --target install
```

## Building

Building is as easy as cloning the repository, running CMake for your preferred
build tool, and then building.

### GCC

```bash
git clone https://github.com/arbCoding/sac-format.git
cd sac-format
cmake --preset gcc-hard-release
cmake --build ./build/hard/release/gcc
```

### Clang

```bash
git clone https://github.com/arbCoding/sac-format.git
cd sac-format
cmake --preset clang-hard-release
cmake --build ./build/hard/release/clang
```

### MSVC
```
git clone https://github.com/arbCoding/sac-format.git
cd sac-format
cmake -B ./build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON -DCMAKE_CXX_EXTENSIONS=OFF -DCMAKE_CXX_FLAGS="/O2 /EHsc /Gs /guard:cf"
```
