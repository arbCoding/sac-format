#!/bin/dash
scripts=$(pwd)
base=$(pwd)/..
cd "$base" || exit
ct_cmd () {
    clang-tidy --checks="bugprone-*,performance-*,readability-*,portability-*,\
        clang-analyzer-*,cpp-coreguidelines-*,modernize-a*,modernize-c*,\
        modernize-d*,modernize-l*,modernize-m*,modernize-p*,modernize-r*,\
        modernize-s*,modernize-t*,modernize-un*,modernize-use-a*,\
        modernize-use-b*,modernize-use-c*,modernize-use-d*,modernize-use-e*,\
        modernize-use-n*,modernize-use-o*,modernize-use-s*,modernize-use-tran*,\
        modernize-use-u*" --extra-arg="-std=c++20" -p \
        "$base/compile_commands.json" "$1"
}

echo "Formatting files..."
echo "Formatting HPP files"
clang-format -style=file -i "$base/src/"*.hpp
# dash doesn't support echo -e "\nDone\n"
echo ""
echo "Done"
echo ""
echo "Formatting CPP files"
clang-format -style=file -i "$base/src/"*.cpp
echo ""
echo "Done"
echo ""
echo "Formatting Example CPP files"
clang-format -style=file -i "$base/src/examples/"*.cpp
echo ""
echo "Done"
echo ""
echo "Running clang-tidy on sac-format.?pp"
# Excluded checks:
#   modernize-use-trailing-return-type
#   Because I don't like that style
#       float my_function(int number) {}
#   is better than
#       auto my_function(int number) -> float {}
ct_cmd "$base/src/"sac_format.?pp
echo ""
echo "Done"
echo ""
echo "Running cpplint on sac-format.?pp"
cpplint "$base/src/"sac_format.?pp
echo ""
echo "Done"
echo ""
echo "Running clang-tidy on examples"
ct_cmd "$base/src/examples/"*.cpp
echo ""
echo "Done"
echo ""
echo "Running cpplint on examples"
cpplint "$base/src/examples/"*.cpp
echo ""
echo "Done"
echo ""
echo "Running shellcheck on *.sh"
shellcheck "$scripts/"*.sh
echo ""
echo "Done"
cd "$scripts" || exit
