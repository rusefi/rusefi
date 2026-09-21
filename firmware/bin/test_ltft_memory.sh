#!/usr/bin/env bash

# Requires the normal firmware build tools on PATH. Run board builds serially:
# configuration generation writes shared files even with separate build dirs.
set -eo pipefail
cd "$(dirname "$0")/.."

if [ "$#" -eq 0 ]; then
    set -- config/boards/microrusefi/meta-info-mre_f4.env \
        config/boards/microrusefi/meta-info-mre-legacy_f4.env \
        config/boards/hellen/alphax-8chan/meta-info-alphax-8chan.env \
        config/boards/nucleo_f429/meta-info.env
fi

for meta in "$@"; do
    (
        source config/boards/common_script_read_meta_env.inc "$meta"
        build="build/ltft_memory_${SHORT_BOARD_NAME}"
        # Keep the largest link command below Windows' command-line limit.
        if [ "$SHORT_BOARD_NAME" = stm32f429_nucleo ]; then
            build="build/ltft_f429"
        fi
        mkdir -p "$build"
        log="$build/build.log"
        make -r -j12 BUILDDIR="$build" config >"$log" 2>&1
        result=0
        make -r -j12 BUILDDIR="$build" "$build/rusefi.elf" >>"$log" 2>&1 || result=$?

        if [ "$result" -ne 0 ]; then
            tail -n 40 "$log"
            exit "$result"
        fi
        symbols=$(arm-none-eabi-nm -S -C "$build/rusefi.elf")
        io_address=$(echo "$symbols" | awk '/ltftIoState/ {print $1}')
        case "$io_address" in
            200*) ;;
            *) echo "$SHORT_BOARD_NAME: LTFT transfer buffer is not in DMA-accessible SRAM"; exit 1 ;;
        esac
        echo "$SHORT_BOARD_NAME: memory layout passed"
        echo "$symbols" | grep -E 'ltft(State|IoState)|__heap(_ccm)?_(base|end)__'
    )
done
