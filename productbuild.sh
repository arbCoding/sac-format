#!/bin/dash
# Build productbuild macOS package
base=$(pwd)
# Cleanup
if [ -e "$base/build/release/clang" ]; then
    rm -rf "$base/build/release/clang"
fi
# Prep the build
cmake -DCPACK_GENERATOR=productbuild \
    -DCMAKE_INSTALL_PREFIX=/opt/sac-format \
    --preset clang-release
# Build
cd "$base/build/release/clang/" || exit
cmake --build .
# Package
cpack -G productbuild
# Move the package
mv ./sac-format.pkg "$base/sac-format.pkg"
# Return home
cd "$base" || exit
