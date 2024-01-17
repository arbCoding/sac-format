#!/bin/bash
# Clean up old files
scripts=$(pwd)
base=$(pwd)/..
cd "$base" || exit

# Minify html files
min_html () {
    ls -1 | grep html | parallel 'minify {} -o {}.new; mv {}.new {}'
}

# Compress pdf files

compress_pdf () {
  echo -e "\nCompressing $1"
  orig_size=$(ls -l | grep "$1" | awk '{print $5}')
  gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 -dNOPAUSE -dQUIET -dBATCH -dPrinted=false -sOutputFile="$1".comp "$1"
  mv "$1".comp "$1"
  new_size=$(ls -l | grep "$1" | awk '{print $5}')
  percent=$(echo 'scale=4; ('"$new_size"'/'"$orig_size"') * 100' | bc)
  percent=$(echo "$percent" | python -c "print(round(float(input()), 1))")
  orig_size=$(echo 'scale=4; '"$orig_size"' / 1000.0' | bc)
  orig_size=$(echo "$orig_size" | python -c "print(round(float(input()), 2))")
  new_size=$(echo 'scale=4; '"$new_size"' / 1000.0' | bc)
  new_size=$(echo "$new_size" | python -c "print(round(float(input()), 2))")
  echo -e "\n($orig_size kB, $new_size kB, $percent%)\n"
}

echo "Cleaning up past build!"
rm -r ./docs/*

# Build with emacs
echo -e "\nBuilding with emacs!"
emacs -Q --script "$scripts/make_docs.el"

# Time to shrink the files!
echo -e "\nMinify-ing files!"
cd "$base/docs/" || exit
min_html
cd "$base" || exit

# Cleanup
echo -e "\nCleaning up latex/pdf"
cd "$base/src/docs" || exit
rm ./*.tex ./*.pdf

# Compress and rename pdf
cd "$base/docs" || exit
compress_pdf index.pdf
mv index.pdf sac-format_manual.pdf

echo -e "($orig_size kB, $new_size kB, $percent%)\n"

# Make doxygen
echo -e "\nMaking Doxygen docs"
cd "$base" || exit
doxygen Doxyfile
# Minify doxygen files
echo -e "\nMinify-ing Doxygen html docs"
cd "$base"/docs/doxygen/html || exit
min_html
echo -e "\nMaking Doxygen pdf docs"
cd "$base"/docs/doxygen/latex || exit
make
#compress_pdf refman.pdf

cd "$scripts" || exit
