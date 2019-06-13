
cd ../../..

set EXTRA_PARAMS=-DDUMMY ^
 -DDEFAULT_ENGINE_TYPE=MIATA_NA6_VAF

call config/boards/common_make.bat

call config/boards/clean_env_variables.bat
