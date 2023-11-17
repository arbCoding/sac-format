#!/bin/dash
# Clean up old files
base=`pwd`
echo "Cleaning up past build!"
rm -r ./docs/*

# Build with emacs
echo "\nBuilding with emacs!"
emacs -Q --script ./build_docs.el

# Time to shrink the files!
echo "\nMinify-ing files!"
cd $base/docs/
ls -1 | grep html | parallel 'minify {} -o {}.new; mv {}.new {}'
cd $base

# Cleanup
echo "\nCleaning up latex/pdf"
cd $base/src/docs
rm *.tex *.pdf

# Compress and rename pdf
cd $base/docs
echo "\nCompressing pdf"
gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 -dNOPAUSE -dQUIET -dBATCH -dPrinted=false -sOutputFile=index-compressed.pdf index.pdf 
mv index-compressed.pdf sac-format_manual.pdf
rm index.pdf
cd $base
