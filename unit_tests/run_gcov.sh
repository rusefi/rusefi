#!/bin/bash

rm -rf gcov_working_area

mkdir gcov_working_area
cd gcov_working_area


echo Looking for source code 
find     ..                          -name *.c* >  source_files.txt
find     ../../firmware/console/     -name *.c* >> source_files.txt
find     ../../firmware/controllers/ -name *.c* >> source_files.txt

wc -l source_files.txt

xargs -L 1 -I {} cp {} . < source_files.txt

cp ../build/obj/* .

gcov *.c* > gcov.log 2>gcov.err


lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory gcov 

# gcovr --html -o rusefi_unit_tests.html
# ls -l rusefi_unit_tests.html


ncftpput -m -R -v -u $RUSEFI_DOXYGEN_FTP_USER -p $RUSEFI_DOXYGEN_FTP_PASS $RUSEFI_FTP_SERVER /unit_tests_coverage gcov/*
 
