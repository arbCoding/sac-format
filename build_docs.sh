#!/bin/dash
# Clean up old files
base=`pwd`
echo "Cleaning up past build!"
rm -r ./docs/*

# Build with emacs
echo "Building with emacs!"
emacs -Q --script ./build_docs.el

# Time to shrink the files!
echo "Minify-ing files!"
cd $base/docs/
ls -1 | grep html | parallel 'minify {} -o {}.new; mv {}.new {}'
cd $base

# Cleanup
cd $base/src/docs
rm *.tex *.pdf

cd $base/docs
mv index.pdf sac-format_manual.pdf
cd $base
