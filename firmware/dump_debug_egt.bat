arm-none-eabi-objdump -S debug_EGT2CAN/rusefi.elf > debug.dump

cd debug_EGT2CAN

java -jar ../../java_tools/gcc_map_reader.jar > ../rusefi_ram_report.txt