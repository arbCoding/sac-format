#!/bin/dash
script=$(pwd)
base=$(pwd)/..
cd "$base" || exit
# Cleanup
if [ -e "$base/coverage" ]; then
    rm -r "$base/coverage"
fi

if [ -e "$base/build/debug/gcc-coverage" ]; then
    rm -rf "$base/build/debug/gcc-coverage"
fi
# Build the preset
cmake --preset gcc-coverage
cmake --build build/debug/gcc-coverage
# Run my unit tests
cd ./build/debug/gcc-coverage/ || exit
ctest
# Locate the gcov output
cd ./CMakeFiles/sac-format.dir/ || exit
# Run lcov to generate coverage report
lcov -c -d . -o ./sac-format.lcov
# Remove standard library references (linux)
lcov -r ./sac-format.lcov "/usr*" -o ./sac-format.lcov
# Convert to html
genhtml --demangle-cpp sac-format.lcov -o coverage --num-spaces 4
# Move the coverage files
mv ./coverage "$base/coverage"
cd "$script" || exit
