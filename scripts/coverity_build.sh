#!/bin/dash
script=$(pwd)
base=$(pwd)/..
cd "$base" || exit
# Cleanup
if [ -e "$base/build/debug/gcc" ]; then
    rm -rf "$base/build/debug/gcc"
fi
# Prepare to build
cmake --preset gcc-debug
# Prepare coverity scan
cd ./build/debug/gcc || exit
# Make sure it only captures the sac_format.hpp/.cpp pair for the library
cov-configure --config cov-conf/cov.xml --compiler g++ --comptype g++ \
    --template --xml-option=skip_file:".*/_deps/.*" \
    --xml-option=skip_file:".*/tests/*.cpp" \
    --xml-option=skip_file:".*/benchmark.cpp" \
    --xml-option=skip_file:".*/list_sac.cpp"
# Capture the ninja build
cov-build --config cov-conf/cov.xml --dir cov-int ninja
# Package for submission
tar czf sac-format.tgz cov-int
# Move to base dir
mv ./sac-format.tgz "$base/sac-format.tgz"
# Return home
cd "$script" || exit
