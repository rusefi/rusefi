# rusEFI firmware build Docker image

ARG UBUNTU_VERSION=22.04
FROM rusefi/build-base:ubuntu-${UBUNTU_VERSION}

ARG RUSEFI_DIR=/rusefi
ARG PROJECT_BOARD
ENV PROJECT_BOARD=${PROJECT_BOARD:-}
ARG PROJECT_CPU
ENV PROJECT_CPU=${PROJECT_CPU:-}
ARG COMPILE_ARGS="PROJECT_BOARD=${PROJECT_BOARD} PROJECT_CPU=${PROJECT_CPU}"
ENV COMPILE_ARGS=${COMPILE_ARGS:-}

WORKDIR ${RUSEFI_DIR}

WORKDIR ./rusefi/firmware
ENV PATH="$PATH:$RUSEFI_DIR/gcc-arm-none-eabi/bin"
ENTRYPOINT ["sh", "-c", "./gen_live_documentation.sh && \
   ./gen_enum_to_string.sh && \
   ./gen_config_default.sh && \
   ([ \"${PROJECT_BOARD}\" = \"kinetis\" ] && ./config/boards/kinetis/config/gen_kinetis_config.sh) || \
   ([ \"${PROJECT_BOARD}\" = \"cypress\" ] && ./config/boards/cypress/config/gen_cypress_config.sh) || \
   ([ \"${PROJECT_BOARD}\" = \"subaru_eg33_f7\" ] && ./config/boards/subaru_eg33/config/gen_subaru_config.sh) || \
   (./gen_config_board.sh **/${PROJECT_BOARD} ${PROJECT_BOARD} firmware/tunerstudio/generated/rusefi_${PROJECT_BOARD}.ini) && \
   ./clean_compile_two_versions.sh ${COMPILE_ARGS}"]
# TODO(nms): support specifying project board to configure
