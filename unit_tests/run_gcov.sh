#!/bin/bash

rm -rf gcov_working_area

mkdir gcov_working_area
cd gcov_working_area
ls -l 


find     ..                          -name *.c* >  source_files.txt
find     ../../firmware/console/     -name *.c* >> source_files.txt
find     ../../firmware/controllers/ -name *.c* >> source_files.txt


xargs -L 1 -I {} cp {} . < source_files.txt

cp ../build/obj/* .

gcov *.c* > gcov.log 2>gcov.err

