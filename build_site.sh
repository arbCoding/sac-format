#!/bin/dash
# Clean up old files
echo "Cleaning up past build!"
rm -r ./docs/*

# Build with emacs
echo "Building with emacs!"
emacs -Q --script ./build_site.el

# Time to shrink the files!
echo "Minify-ing files!"
cd ./docs/
ls -1 | grep html | parallel 'minify {} -o {}.new; mv {}.new {}'
cd ../
