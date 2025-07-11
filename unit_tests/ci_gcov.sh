#!/bin/bash

#
# this script is used by github actions
#

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

echo -e "\nGenerating rusEFI unit test coverage"
gcov *.c* > gcov.log 2>gcov.err

echo -e "\nCollecting rusEFI unit test coverage"
#FIXME: we have some problem related to google test macro "TEST" and the ouput code on lcov
# todo gtest vs gcov https://github.com/rusefi/rusefi/issues/8129
# Currently we cannot obtain coverage on the tests themselves, but on the tests towards the code.
lcov --ignore-errors mismatch --capture --directory . --output-file coverage.info

echo -e "\nGenerating rusEFI unit test HTML"
genhtml coverage.info --output-directory gcov
echo -e "\nGenerating rusEFI unit test HTML"
