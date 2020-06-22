SCRIPT_NAME="common_make.bat"
echo "Entering $SCRIPT_NAME"

mkdir .dep
# todo: start using env variable for number of threads or for '-r'
make -j4 -r
[ -e build/rusefi.hex ] || (echo "FAILED to compile by $SCRIPT_NAME with $PROJECT_BOARD $DEBUG_LEVEL_OPT and $EXTRA_PARAMS"; exit -1)

mkdir deliver

rm -f deliver/rusefi.dfu
echo "$SCRIPT_NAME: invoking hex2dfu"
../misc/encedo_hex2dfu/hex2dfu -i build/rusefi.hex -o deliver/rusefi.dfu
cp build/rusefi.bin deliver/

echo "$SCRIPT_NAME: deliver folder"
ls -l deliver

sh config/boards/clean_env_variables.sh
