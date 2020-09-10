@echo off
set PATH=G:\VStudio\arm-elf-gcc\bin;G:\VStudio\msys2\usr\bin;C:\Program Files\Java\jre\bin;G:\VStudio\Git\bin

echo "Compiling for Cypress FM4 S6E2CxAH"
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
set EXTRA_PARAMS=-DDUMMY -D__USE_CMSIS^
 -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE^
 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS

set BUILDDIR=build_cypress
set PROJECT_BOARD=hellen/cypress
set PROJECT_CPU=cypress
set USE_FATFS=no
set USE_BOOTLOADER=no
set DEBUG_LEVEL_OPT="-O2"

rem make -r clean
rem make -r
rem 2> err.log
rem rm -f build/rusefi.dfu
rem dir build
rem ..\misc\hex2dfu\HEX2DFU.exe build/rusefi.hex   -out build/rusefi.dfu

cd ../../../..

rm -f build_cypress/rusefi.hex

call config/boards/common_make.bat
