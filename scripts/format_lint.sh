#!/bin/bash
#
# Required tools
#   clang-tidy          https://clang.llvm.org/extra/clang-tidy/
#   clang-format        https://clang.llvm.org/docs/ClangFormat.html
#   cpplint             https://github.com/cpplint/cpplint
#   Shellcheck          https://github.com/koalaman/shellcheck
#   markdownlint-cli2   https://github.com/DavidAnson/markdownlint-cli2
#   oclint              https://github.com/oclint/oclint
#   pmd                 https://github.com/pmd/pmd
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
    clang-tidy --checks="\
        bugprone-*,\
        performance-*,\
        readability-*,\
        portability-*,\
        clang-analyzer-*,\
        cpp-coreguidelines-*,\
        modernize-a*,\
        modernize-c*,\
        modernize-d*,\
        modernize-l*,\
        modernize-m*,\
        modernize-p*,\
        modernize-r*,\
        modernize-s*,\
        modernize-t*,\
        modernize-un*,\
        modernize-use-a*,\
        modernize-use-b*,\
        modernize-use-c*,\
        modernize-use-d*,\
        modernize-use-e*,\
        modernize-use-n*,\
        modernize-use-o*,\
        modernize-use-s*,\
        modernize-use-tran*,\
        modernize-use-u*,\
        clang-diagnostic-*,\
        alpha.clone.CloneChecker" \
        --extra-arg="-std=c++20" -p \
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

echo -e "\nLinting markdown"
cd "$base"/src/docs || exit
markdownlint-cli2 --fix ./*.md

# rule threshold issues are with Trace class
# Nothing likes all the parameters/methods, but I'm leaving
# it that way so that it can be close to the default usage
# inside SAC itself (more familiar to potential users of this
# library).
# I should break the reading/writing methods into smaller
# chunks (later).
echo "Running oclint"
oclint "$base"/include/sac-format/sac_format.hpp \
    "$base"/src/sac_format.cpp \
    "$base"/src/examples/list_sac.cpp \
    -disable-rule ConstantIfExpression \
    -rc NCSS_METHOD=300 \
    -rc LONG_METHOD=300 \
    -rc NPATH_COMPLEXITY=300 \
    -rc TOO_MANY_METHODS=300

echo "Running PMD CPD (Copy-Paste Detection)"
pmd cpd --minimum-tokens 45 \
    --ignore-sequences \
    --ignore-identifiers \
    --no-skip-blocks \
    --dir "$base"/src \
    --dir "$base"/include \
    --language cpp \
    --exclude "$base"/src/tests \
    --exclude "$base"/src/benchmark.cpp

cd "$scripts" || exit
