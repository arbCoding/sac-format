#!/bin/dash
base=$(pwd)
echo "Formatting files..."
echo "Formatting HPP files"
clang-format -style=file -i "$base/src/"*.hpp
echo "Done"
echo "Formatting CPP files"
clang-format -style=file -i "$base/src/"*.cpp
echo "Done"
echo "Formatting Example CPP files"
clang-format -style=file -i "$base/src/examples/"*.cpp
echo "Done"
echo "Running clang-tidy on sac-format.*"
# Excluded checks:
#   modernize-use-trailing-return-type
#   Because I don't like that style
#       float my_function(int number) {}
#   is better than
#       auto my_function(int number) -> float {}
clang-tidy --checks="bugprone-*,performance-*,readability-*,portability-*,\
    clang-analyzer-*,cpp-coreguidelines-*,modernize-a*,modernize-c*,\
    modernize-d*,modernize-l*,modernize-m*,modernize-p*,modernize-r*,\
    modernize-s*,modernize-t*,modernize-un*,modernize-use-a*,modernize-use-b*,\
    modernize-use-c*,modernize-use-d*,modernize-use-e*,modernize-use-n*,\
    modernize-use-o*,modernize-use-s*,modernize-use-tran*,modernize-use-u*"\
    --extra-arg="-std=c++20" -p "$base/compile_commands.json" \
    "$base/src/"sac_format.?pp
echo "Done"
