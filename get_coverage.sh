#!/bin/dash
base=$(pwd)
# Cleanup
if [ -e "$base/coverage" ]; then
    rm -r "$base/coverage"
fi

if [ -e "$base/build/debug/gcc-coverage" ]; then
    rm -rf "$base/build/debug/gcc-coverage"
fi
## Build the preset
cmake --preset gcc-coverage
cmake --build build/debug/gcc-coverage
# Run my unit tests
cd ./build/debug/gcc-coverage/
ctest
# Locate the gcov output
cd ./CMakeFiles/sac-format.dir/
# Run lcov to generate coverage report
lcov -c -d . -o sac-format.lcov
# Remove standard library references (linux)
lcov -r sac-format.lcov "/usr*" -o sac-format.lcov
# Convert to html
genhtml --demangle-cpp sac-format.lcov -o coverage --num-spaces 4
cp sac-format.lcov $base/sac-format.lcov
# Move the coverage files
mv ./coverage "$base/coverage"
cd "$base"
