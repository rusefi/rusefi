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
        mkdir -p "$build"
        log="$build/build.log"
        make -r -j12 BUILDDIR="$build" config >"$log" 2>&1
        result=0
        make -r -j12 BUILDDIR="$build" "$build/rusefi.elf" >>"$log" 2>&1 || result=$?

        # Reproduce the current CCM overflow before changing placement.
        case "$SHORT_BOARD_NAME" in
            mre_f4|mre-legacy_f4) overflow='from 10010510 to 10010000' ;;
            alphax-8chan) overflow='from 100103e0 to 10010000' ;;
            *) overflow='' ;;
        esac
        if [ -n "$overflow" ]; then
            if [ "$result" -eq 0 ] || ! grep -Fq "$overflow" "$log"; then
                tail -n 40 "$log"
                exit 1
            fi
            echo "$SHORT_BOARD_NAME: reproduced CCM overflow ($overflow)"
        elif [ "$result" -ne 0 ]; then
            tail -n 40 "$log"
            exit "$result"
        else
            arm-none-eabi-nm -S -C "$build/rusefi.elf" | grep -E 'ltft(State|IoState)|__heap(_ccm)?_(base|end)__'
        fi
    )
done
