rm -f build/rusefi_simulator.exe
make -j$(nproc) -r
[ $? -eq 0 ] || { echo "Simulator compilation failed"; exit 1; }
file build/rusefi_simulator
echo "TIMESTAMP $(date "+%D %T.%2N") just compiled rusefi simulator"
