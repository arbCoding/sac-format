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
orig_size=`ls -l | grep index | grep pdf | awk '{print $5}'`
gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 -dNOPAUSE -dQUIET -dBATCH -dPrinted=false -sOutputFile=index-compressed.pdf index.pdf 
mv index-compressed.pdf sac-format_manual.pdf
rm index.pdf
new_size=`ls -l | grep manual | grep pdf | awk '{print $5}'`

percent=`echo 'scale=1; ('"$new_size"'/'"$orig_size"') * 100' | bc`
orig_size=`echo 'scale=0; '"$orig_size"' / 1000.0' | bc`
new_size=`echo 'scale=0; '"$new_size"' / 1000.0' | bc`

echo "($orig_size kB, $new_size kB, $percent%)\n"
cd $base
