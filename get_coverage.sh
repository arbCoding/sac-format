#!/bin/dash
base=`pwd`
# Cleanup
if [ -e $base/coverage ]; then
    rm -r $base/coverage
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
lcov -c -d . -o sac-format.txt
# Remove standard library references (linux)
lcov -r sac-format.txt "/usr*" -o sac-format.txt
# Convert to html
genhtml --demangle-cpp sac-format.txt -o coverage --num-spaces 4
cp sac-format.txt $base/sac-format.txt
# Move the coverage files
mv ./coverage $base/coverage
cd $base
