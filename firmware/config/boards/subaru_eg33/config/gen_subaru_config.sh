#/!bin/sh
# This batch files reads rusefi_config.txt and produces firmware persistent configuration headers
# the storage section of rusefi.ini is updated as well

BOARDNAME=subaru_eg33
SHORT_BOARD_NAME=${BOARDNAME}_f7
BOARD_DIR=config/boards/${BOARDNAME}















  INI="rusefi_${SHORT_BOARD_NAME}.ini"




bash gen_signature.sh ${SHORT_BOARD_NAME}











source gen_config_common.sh
echo "Using COMMON_GEN_CONFIG [$COMMON_GEN_CONFIG]"

JAVA_REMOTE_DEBUG="-agentlib:jdwp=transport=dt_socket,server=y,suspend=y,address=*:5005"



java \
 $COMMON_GEN_CONFIG_PREFIX \
  -tool ${BOARD_DIR}/config/gen_subaru_config.sh \
 $COMMON_GEN_CONFIG \
  -enumInputFile ${BOARD_DIR}/rusefi_hw_enums.h \
  \
 -c_defines controllers/generated/rusefi_generated_subaru_eg33_f7.h \
 -c_destination controllers/generated/engine_configuration_generated_structures_subaru_eg33_f7.h \

[ $? -eq 0 ] || { echo "ERROR generating TunerStudio config for ${BOARDNAME}"; exit 1; }

# EG33 does not get fancy mass storage device since it does not have create_ini_image.sh etc invocations like gen_config_board.sh does
echo "Happy ${SHORT_BOARD_NAME}!"
exit 0
