#!/bin/bash

set -euo pipefail

echo "This script reads rusefi_config.txt and produces firmware persistent configuration headers"
echo "The storage section of fome.ini is updated as well"

rm -f gen_config.log
rm -f gen_config_board.log

# todo: who is the consumer of this folder? shall we move that 'mkdir' command closer to usage?
mkdir build

#
# see also build-firmware where we compile all versions of firmware
#
#
for BOARD in \
   "config/boards/hellen/alphax-2chan alphax-2chan" \
   "config/boards/hellen/alphax-4chan alphax-4chan" \
   "config/boards/hellen/alphax-8chan alphax-8chan" \
   "config/boards/hellen/harley81 harley81" \
   "config/boards/hellen/hellen128 hellen128" \
   "config/boards/hellen/hellen121vag hellen121vag" \
   "config/boards/hellen/hellen121nissan hellen121nissan" \
   "config/boards/hellen/hellen-honda-k hellen-honda-k" \
   "config/boards/hellen/hellen154hyundai hellen154hyundai" \
   "config/boards/hellen/hellen88bmw hellen88bmw" \
   "config/boards/hellen/hellen72 hellen72" \
   "config/boards/hellen/hellen81 hellen81" \
   "config/boards/hellen/hellen-nb1 hellen-nb1" \
   "config/boards/hellen/hellen-gm-e67 hellen-gm-e67" \
   "config/boards/hellen/hellen64_miataNA6_94 hellenNA6" \
   "config/boards/hellen/hellenNA8_96 hellenNA8_96" \
   "config/boards/hellen/small-can-board small-can-board" \
   "config/boards/microrusefi mre_f7" \
   "config/boards/microrusefi mre_f4" \
   "config/boards/core8 core8" \
   "config/boards/core48 core48" \
   "config/boards/frankenso frankenso_na6" \
   "config/boards/prometheus prometheus_469" \
   "config/boards/prometheus prometheus_405" \
   "config/boards/proteus proteus_f7" \
   "config/boards/proteus proteus_f4" \
   "config/boards/proteus proteus_h7" \
   "config/boards/f407-discovery f407-discovery" \
   "config/boards/f429-discovery f429-discovery" \
   "config/boards/atlas atlas"\
   "config/boards/tdg-pdm8 tdg-pdm8"\
   ; do
 BOARD_NAME=$(echo "$BOARD" | cut -d " " -f 1)
 BOARD_SHORT_NAME=$(echo "$BOARD" | cut -d " " -f 2)
 INI=$(echo "$BOARD" | cut -d " " -f 3)
 ./gen_config_board.sh $BOARD_NAME $BOARD_SHORT_NAME $INI
 [ $? -eq 0 ] || { echo "ERROR generating board $BOARD_NAME $BOARD_SHORT_NAME $INI"; exit 1; }
done

#
# TODO: it's time to kill the 'default' bundle concept and just live happily with explicit f407-discovery
#
# problem statement: it's desired that plain 'make' invocation compiles discovery binary
# reality: while we have per-target signature*.h and rusefi*.ini we do not have matching per-target 'engine_configuration_generated_structures.h'
# so for plain 'make' to produce f4 discovery we rely of file system engine_configuration_generated_structures.h being f4 discovery version
#
#
# default config should be generated after normal custom boards so that it would be default
# firmware/controllers/generated/rusefi_generated.h file which would be pushed into VCS
./gen_config_default.sh
[ $? -eq 0 ] || { echo "ERROR generating default"; exit 1; }


exit 0
