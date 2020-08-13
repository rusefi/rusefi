#!/bin/bash

for file in $(find . -name "*.kicad_pcb"); do
../KiCad-Diff/kidiff_linux.py -w -s git -b $(git rev-parse --short HEAD~1) -a $(git rev-parse --short HEAD) -d :0 $file
done
