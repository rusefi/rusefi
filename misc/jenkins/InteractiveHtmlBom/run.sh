#!/bin/bash

INTERACTIVE_HTML_BOM_NO_DISPLAY="true"
IBOM_CMD="python3 hardware/InteractiveHtmlBom/InteractiveHtmlBom/generate_interactive_bom.py --no-browser --name-format \"%f_latest\" --dest-dir ../ibom"
echo "IBOM_CMD=$IBOM_CMD"

pwd
[ -e hardware/frankenso/frankenso.kicad_pcb ] || { echo "hardware/frankenso/frankenso.kicad_pcb not found. Was this invoked from wrong folder?"; exit -1; }

$IBOM_CMD hardware/Common_Rail_MC33816/Common_Rail_MC33816.kicad_pcb
$IBOM_CMD hardware/brain_board/brain_board_STM32F407.kicad_pcb
$IBOM_CMD hardware/brain_board_176-pin/176-pin_board.kicad_pcb
$IBOM_CMD --extra-fields "mfg,mfg#,vend1,vend1#" hardware/CJ125_board/O2_input_CJ125.kicad_pcb --netlist-file hardware/CJ125_board/O2_input_CJ125.net
$IBOM_CMD --extra-fields "MFG,MFG#,VEND1,VEND1#" hardware/frankenso/frankenso.kicad_pcb --netlist-file hardware/frankenso/frankenso.net
$IBOM_CMD --extra-fields "mfg,mfg#,vend1,vend1#" hardware/frankenstein/frankenstein.kicad_pcb --netlist-file hardware/frankenstein/frankenstein.net
$IBOM_CMD hardware/mini48-stm32/mini48-stm32.kicad_pcb
$IBOM_CMD hardware/HighSideSwitch/VN750PS_E.kicad_pcb
$IBOM_CMD hardware/can_board/can_brd_1.kicad_pcb
