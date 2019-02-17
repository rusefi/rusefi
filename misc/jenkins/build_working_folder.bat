
echo Working folder: %folder%
mkdir %folder%

cp java_console_binary/rusefi_console.jar %folder%
cp simulator/build/rusefi_simulator.exe %folder%
cp firmware/tunerstudio/rusefi.ini %folder%

cp firmware/svnversion.h %folder%
cp -r misc/install/openocd %folder% 
cp java_console/rusefi.xml %folder%
rem 407 has two versions
cp firmware/deliver/rusefi_release.* %folder%
cp firmware/deliver/rusefi_debug.* %folder%
rem 746 builds one version at the moment
cp firmware/build/rusefi.* %folder%
cp misc/console_launcher/rusefi_console.exe %folder%


cd temp
echo "Please copy find.exe to findcyg.exe in cygwin folder"
findcyg . -name '.svn' > folders_to_delete.txt
echo "Deleting .svn"
xargs rm  -rf < folders_to_delete.txt
echo "Deleted .svn"
rm -rf folders_to_delete.txt

echo "Building bundle"
pwd
zip -r rusefi_bundle.zip *
echo "Bundle ready"
cd ..
echo "We are back in root directory"
pwd
