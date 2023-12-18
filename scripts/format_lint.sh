#!/bin/bash
scripts=$(pwd)
base=$(pwd)/..
cd "$base" || exit
# Excluded checks:
#   modernize-use-trailing-return-type
#   Because I don't like that style
#       float my_function(int number) {}
#   is better than
#       auto my_function(int number) -> float {}
ct_cmd () {
    clang-tidy --checks="bugprone-*,performance-*,readability-*,portability-*,\
        clang-analyzer-*,cpp-coreguidelines-*,modernize-a*,modernize-c*,\
        modernize-d*,modernize-l*,modernize-m*,modernize-p*,modernize-r*,\
        modernize-s*,modernize-t*,modernize-un*,modernize-use-a*,\
        modernize-use-b*,modernize-use-c*,modernize-use-d*,modernize-use-e*,\
        modernize-use-n*,modernize-use-o*,modernize-use-s*,modernize-use-tran*,\
        modernize-use-u*" --extra-arg="-std=c++20" -p \
        "$base/compile_commands.json" "$1"
    echo ""
}

cf_cmd() {
    clang-format -style=file -i "$1"
    echo ""
}

cl_cmd() {
    cpplint "$1"
    echo ""
}

format_lint_dir() {
    for file in "$1"/*.?pp; do
        echo "Formatting file: $file"
        cf_cmd "$file"
        echo "clang-tidy: $file"
        ct_cmd "$file"
        echo "cpplint: $file"
        cl_cmd "$file"
        echo ""
    done
}

dir_list=("$base/src" "$base/src/examples" "$base/src/tests" \
    "$base/include/sac-format")

for dir in "${dir_list[@]}"; do
    echo -e "Scanning $dir\n"
    format_lint_dir "$dir"
done

echo "Running shellcheck on *.sh"
shellcheck "$scripts/"*.sh
cd "$scripts" || exit
