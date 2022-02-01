@echo off

set GIT_PATH="C:\Program Files\Git\bin\git.exe"

@RD /S /Q "firmware/.dep"
@RD /S /Q "firmware/build"
del "firmware/pch/pch.h.gch" /s /f /q

@RD /S /Q "firmware/ChibiOS"
@RD /S /Q "firmware/ChibiOS-Contrib"
@RD /S /Q "unit_tests/googletest"
@RD /S /Q "hardware/rusefi_lib"
@RD /S /Q "hardware/InteractiveHtmlBom"
@RD /S /Q "firmware/ext/cmsis-svd"
@RD /S /Q "firmware/controllers/can/wideband_firmware"
@RD /S /Q "firmware/ext/uzlib"
@RD /S /Q "firmware/ext/lua"
@RD /S /Q "firmware/controllers/lua/luaaa"
@RD /S /Q "firmware/ext/openblt"
@RD /S /Q "misc/hex2dfu"
@RD /S /Q "java_console/luaformatter"

%GIT_PATH% submodule update --init

cd firmware
make clean -j8 PROJECT_BOARD=core8
make -j8 PROJECT_BOARD=core8 PROJECT_CPU=ARCH_STM32F4
bash.exe gen_config_board.sh core8 core8 rusefi_core8.ini




