cd ../../..

export EXTRA_PARAMS="-DDUMMY \
 -DDEFAULT_ENGINE_TYPE=MIATA_NA6_VAF \
 -DFIRMWARE_ID=\"frankensoNA6\""

sh config/boards/common_make.sh

