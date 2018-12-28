rem
rem This script is measuring compilation time
rem 

cd ../firmware

echo Are you using the right -jX value? > benchmark.txt
echo Startting clean for benchmark outside of time measurment >> benchmark.txt
make clean
echo Startting build for benchmark >> benchmark.txt
powershell get-date -format "{dd-MMM-yyyy HH:mm:ss}" >> benchmark.txt
make -j4
echo Done! >> benchmark.txt
powershell get-date -format "{dd-MMM-yyyy HH:mm:ss}" >> benchmark.txt
