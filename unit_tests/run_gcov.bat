
rm -rf gcov_working_area

mkdir gcov_working_area
cd gcov_working_area
ls -l 


find_cyg ..                          -name *.c* >  source_files.txt
find_cyg ../../firmware/console/     -name *.c* >> source_files.txt
find_cyg ../../firmware/controllers/ -name *.c* >> source_files.txt


xargs -L 1 -I {} cp {} . < source_files.txt

cp ../build/obj/* .

gcov *.c* > gcov.log 2>gcov.err

rem TODO find a windows solution, I was not able to find a working lcov for Windows :(