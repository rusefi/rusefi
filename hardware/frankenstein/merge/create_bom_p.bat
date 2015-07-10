cd ..
java -jar ../../java_tools/pcb_sync.jar bom_builder frankenstein.cmp frankenstein.kicad_pcb bom_dictionary_digikey.csv digikey_parts printPadCount printReference printUserComment > bom_builder.log
