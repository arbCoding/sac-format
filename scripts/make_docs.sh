#!/bin/bash
# Directory structure
scripts=$(pwd)
base=$(pwd)/..
cd "$base" || exit

# Minify html, js, and css files
min_web() {
    ls -1 | grep 'html\|js\|css' | parallel 'minify {} -o {}.new; mv {}.new {}'
}

# Compress pdf files
compress_pdf () {
  echo -e "\nCompressing $1"
  orig_size=$(ls -l | grep "$1" | awk '{print $5}')
  gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 -dNOPAUSE -dQUIET -dBATCH \
    -dPrinted=false -sOutputFile="$1".comp "$1"
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

# Clean up old files
echo "Cleaning up past build!"
rm -r "$base"/docs
mkdir "$base"/docs

echo -e "\nCopying screenshots"
cp -r "$base"/src/docs/screenshots "$base"/docs/screenshots
echo -e "\nOptimizing screenshots"
cd "$base"/docs/screenshots || exit
for dir in ./*; do
  if [ -d "$dir" ]; then
    cd "$dir" || exit
    optipng -quiet ./*.png
    cd "$base"/docs/screenshots || exit
  fi
done

# Make doxygen
echo -e "\nMaking Doxygen docs"
cd "$base" || exit
doxygen ./Doxyfile
# Minify doxygen files
echo -e "\nMinify-ing Doxygen html docs"
html_dox="$base"/docs/html
dir_list=("$html_dox" "$html_dox/search")
for dir in "${dir_list[@]}"; do
  cd "$dir" || exit
  echo -e "Minify html, js, and css for $dir\n"
  # Shrink all html, js, and css files
  min_web
  echo -e "Optimize all png for $dir\n"
  # Shrink all png files
  optipng -quiet ./*.png
  echo ""
done

echo -e "\nMaking Doxygen pdf docs"
cd "$base"/docs/latex || exit
make

# Prepare for deployment
mkdir "$base"/docs/pdf
# For some reason, attempting to shrink Doxygen pdf actually makes it larger!
#compress_pdf refman.pdf
mv refman.pdf "$base"/docs/pdf/sac-format_manual.pdf
cd "$base"/docs || exit
mv "$base"/docs/html/* "$base"/docs/
rm -rf "$base"/docs/html
rm -rf "$base"/docs/latex

cd "$scripts" || exit
