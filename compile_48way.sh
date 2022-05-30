@echo off
cd firmware
make -j8 clean
cd ..
bash "misc/jenkins/compile_other_versions/compile.sh" 48way 48way rusefi_48way.ini
cd firmware
bash gen_config_board.sh 48way 48way rusefi_48way.ini
cd ..
