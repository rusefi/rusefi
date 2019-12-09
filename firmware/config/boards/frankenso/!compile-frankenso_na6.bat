
cd ../../..

set EXTRA_PARAMS=-DDUMMY ^
 -DDEFAULT_ENGINE_TYPE=MIATA_NA6_VAF ^
 -DFIRMWARE_ID=\"frankensoNA6\"

call config/boards/common_make.bat

