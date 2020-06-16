#!/bin/bash

rm -rf gcov_working_area

mkdir gcov_working_area
cd gcov_working_area

echo "Looking for source code"
find     ..                          -name *.c* >  source_files.txt
find     ../../firmware/console/     -name *.c* >> source_files.txt
find     ../../firmware/controllers/ -name *.c* >> source_files.txt

wc -l source_files.txt

xargs -L 1 -I {} cp {} . < source_files.txt

cp ../build/obj/* .

echo "Generating coverage"
gcov *.c* > gcov.log 2>gcov.err

echo "Collecting coverage"
lcov --capture --directory . --output-file coverage.info

echo "Generating HTML"
genhtml coverage.info --output-directory gcov

echo "Uploading HTML"
if [ $1 ] && [ $2 ] && [ $3 ]; then
 ncftpput -m -R -v -u "$1" -p "$2" "$3" /unit_tests_coverage gcov/*
else
 echo "Error: FTP server details seem to be missing"
 exit 1
fi

