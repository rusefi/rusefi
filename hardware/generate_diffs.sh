#####################################################################################
#                                 generate_diffs.sh                                 #
#                                    08/13/2020                                     #
#                             Written By David Holdeman                             #
#   Checks for changed .kicad_pcb files, generates visual diffs, and uploads them   #
#                                      to FTP.                                      #
#####################################################################################
# The FTP location this uploads to is /diffs/plot_<name of kicad_pcb file, without the extension>
# In that directory are subdirectorys containing generated SVGs, as well as a 'web' subdirectory,
# which contains index.html. Open that in a web browser and you get a nice visual diff of each layer.
#!/bin/bash

function gendiffs() {
  ../KiCad-Diff/kidiff_linux.py -w -s Git -b $(git rev-parse --short HEAD~1) -a $(git rev-parse --short HEAD) -d :0 $1
  if [ -d $(dirname "$1")/plots ] && [ -n "$RUSEFI_FTP_SERVER" ]; then
    echo "mkdir diffs/plots_$(basename "$1" .kicad_pcb)" | ncftp -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER"
    ncftpput -R -v -u "$RUSEFI_DOXYGEN_FTP_USER" -p "$RUSEFI_DOXYGEN_FTP_PASS" "$RUSEFI_FTP_SERVER" /diffs/plots_$(basename "$1" .kicad_pcb) $(dirname "$1")/plots
  fi
}
export -f gendiffs

find . -name "*.kicad_pcb" -exec bash -c 'gendiffs "$0"' {} \;
