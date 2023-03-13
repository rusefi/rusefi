#!/bin/bash

INTERACTIVE_HTML_BOM_NO_DISPLAY="true"
ibom() {
  python3 hardware/InteractiveHtmlBom/InteractiveHtmlBom/generate_interactive_bom.py --no-browser --name-format "%f_latest" --dest-dir ../ibom --include-nets $@
}

echo "Current directory: $(pwd)"
[ $(basename $(pwd)) = 'rusefi' ] || { echo "Not in 'rusefi' directory. Was this invoked from the wrong directory?"; exit 1; }

for f in $(ls hardware/*/*.kicad_pcb); do
  if ls $(dirname $f)/$(basename $f .kicad_pcb).net 2>/dev/null; then
    ibom --netlist-file $(ls $(dirname $f)/$(basename $f .kicad_pcb).net) $f
  else
    ibom $f
  fi
done
