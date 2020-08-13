#!/bin/bash

for file in $(find . -name "*.kicad_pcb"); do
  ../KiCad-Diff/kidiff_linux.py -w -s Git -b $(git rev-parse --short HEAD~1) -a $(git rev-parse --short HEAD) -d :0 $file
  if [ -d $(dirname "$file")/plots ] && [ -n "$RUSEFI_FTP_SERVER" ]; then
    echo "mkdir diffs/plots_$(basename "$file")" | ncftp -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER"
    ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /diffs/plots_$(basename "$file") $(dirname "$file")/plots
  fi
done
