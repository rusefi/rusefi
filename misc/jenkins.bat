
zip -j rusefi_simulator.zip win32_functional_tests/build/rusefi_simulator.exe firmware/tunerstudio/rusefi.ini java_console_binary/rusefi_console.jar

echo user u71977750-build> ftpcmd.dat
echo XXX >> ftpcmd.dat
echo bin>> ftpcmd.dat
echo put rusefi_simulator.zip separate_files/rusefi_simulator.zip >> ftpcmd.dat
echo put temp/rusefi_bundle.zip rusefi_bundle.zip >> ftpcmd.dat
echo quit>> ftpcmd.dat

ftp -n -s:ftpcmd.dat rusefi.com
