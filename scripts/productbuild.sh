#!/bin/dash
# Build productbuild macOS package
script=$(pwd)
base=$(pwd)/..
cd "$base" || exit
# Cleanup
if [ -e "$base/build/release/clang" ]; then
    rm -rf "$base/build/release/clang"
fi
cp "$base/LICENSE" "$base/License2.txt"
fold -s "$base/LICENSE" > "$base/License.txt"
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
# Cleanup
rm "$base/License.txt" "$base/License2.txt"
# Return home
cd "$script" || exit
