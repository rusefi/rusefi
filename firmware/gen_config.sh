#!/bin/bash

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "The storage section of rusefi.ini is updated as well"

rm -f gen_config.log
rm -f gen_config_board.log

# todo: who is the consumer of this folder? shall we move that 'mkdir' command closer to usage?
mkdir build


# todo: we have a bit of code duplication with build-firmware.yaml here :(
config/boards/kinetis/config/gen_kinetis_config.sh
[ $? -eq 0 ] || { echo "ERROR generating board kinetis kin"; exit 1; }

config/boards/cypress/config/gen_cypress_config.sh
[ $? -eq 0 ] || { echo "ERROR generating board cypress cypress"; exit 1; }

config/boards/subaru_eg33/config/gen_subaru_config.sh
[ $? -eq 0 ] || { echo "ERROR generating board subaru_eg33 subaru_eg33_f7"; exit 1; }

#
# see also build-firmware where we compile all versions of firmware
#
#
for BOARD in \
   "hellen/alphax-2chan alphax-2chan" \
   "hellen/alphax-4chan alphax-4chan" \
   "hellen/harley81 harley81" \
   "hellen/hellen128 hellen128 rusefi_hellen128mercedes.ini" \
   "hellen/hellen121vag hellen121vag" \
   "hellen/hellen121nissan hellen121nissan" \
   "hellen/hellen154hyundai hellen154hyundai" \
   "hellen/hellen88bmw hellen88bmw" \
   "hellen/hellen72 hellen72" \
   "hellen/hellen81 hellen81" \
   "hellen/hellen-nb1 hellen-nb1" \
   "hellen/hellen-gm-e67 hellen-gm-e67" \
   "hellen/hellen64_miataNA6_94 hellenNA6" \
   "hellen/hellenNA8_96 hellenNA8_96" \
   "microrusefi mre_f7" \
   "microrusefi mre_f4" \
   "core8 core8" \
   "48way 48way" \
   "frankenso frankenso_na6" \
   "prometheus prometheus_469" \
   "prometheus prometheus_405" \
   "proteus proteus_f7" \
   "proteus proteus_f4" \
   "proteus proteus_h7" \
   "f407-discovery f407-discovery" \
   "f429-discovery f429-discovery" \
   "atlas atlas"\
   "tdg-pdm8 tdg-pdm8"\
   ; do
 BOARD_NAME=$(echo "$BOARD" | cut -d " " -f 1)
 BOARD_SHORT_NAME=$(echo "$BOARD" | cut -d " " -f 2)
 INI=$(echo "$BOARD" | cut -d " " -f 3)
 ./gen_config_board.sh $BOARD_NAME $BOARD_SHORT_NAME $INI
 [ $? -eq 0 ] || { echo "ERROR generating board $BOARD_NAME $BOARD_SHORT_NAME $INI"; exit 1; }
done

#
# TODO: it's time to kill the 'default' bundle concept and just live happily with explicit f407-discovery
# default config should be generated after normal custom boards so that it would be default
# firmware/controllers/generated/rusefi_generated.h file which would be pushed into VCS
./gen_config_default.sh
[ $? -eq 0 ] || { echo "ERROR generating default"; exit 1; }


exit 0
