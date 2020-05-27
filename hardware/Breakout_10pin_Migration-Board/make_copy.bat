mkdir clone_src
mkdir clone

cp pin_board_1.* clone_src/

java -jar ../../java_tools/pcb_sync.jar rename rename.txt clone_src clone

#cp pin_board_1.kicad_pcb pin_board_1_copy.kicad_pcb
java -jar ../../java_tools/pcb_sync.jar pcb_move clone/pin_board_1.kicad_pcb clone/pin_board_1.kicad_pcb 200 0


java -jar ../../java_tools/pcb_sync.jar pcb_merge pin_board_1.kicad_pcb pin_board_1_x2.kicad_pcb merge.txt
