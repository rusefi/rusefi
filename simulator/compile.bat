rem this is about CygWin colon issue, .dep files are invalid because of that
rem rm -rf .dep
rm -f build/rusefi_simulator.exe
make -j4 -r
echo "TIMESTAMP %date% %time% just compiled rusefi simulator"