#!/bin/bash

function gendiffs() {
  ../KiCad-Diff/kidiff_linux.py -w -s Git -b $(git rev-parse --short HEAD~1) -a $(git rev-parse --short HEAD) -d :0 $1
  if [ -d $(dirname "$1")/plots ] && [ -n "$RUSEFI_FTP_SERVER" ]; then
    echo "mkdir diffs/plots_$(basename "$1" .kicad_pcb)" | ncftp -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER"
    ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /diffs/plots_$(basename "$1" .kicad_pcb) $(dirname "$1")/plots
  fi
}

find . -name "*.kicad_pcb" -exec gendiffs {} \;
