# Kinetis

rusEfi runs on MKE16F512 (alpha version as of July 2019)

rusEfi Kinetis implementation consists of three files/folders:
  firmware/config/boards/kinetis
  firmware/hw_layer/ports/kinetis
  firmware/hw_layer/trigger_input_comp.cpp
  

Since MKE16F512 is the first not stm32 rusEfi implementation, AndreiKA has a lot of fun!
Kinetis overrides are in
   firmware/config/boards/kinetis/rusefi_hw_enums.h overrides pins
   firmware/config/boards/kinetis/config/rusefi_config_kinetis.txt
   
todo:
have rusefi_stm32_hw_enums.h   
have rusefi_kinetis_hw_enums.h
move more generated files to 'generated' folder(s)
maybe generated_stm32?

todo: 
move firmware/config/boards/kinetis/config/!gen_enum_to_string.bat somewhere else? merge with stm32?

At the moment we use internal RC generator. Open question if it's good enough.

![pic](https://raw.githubusercontent.com/wiki/rusefi/rusefi_documentation/Hardware/Deucalion/Deucalion_0_1_half_assembled.jpg)

# How to program

Used to be NXP_Kinetis_Bootloader_2_0_0 KinetisFlashTool and it's not clear how to download it now :(

We happen to have https://github.com/rusefi/rusefi_external_utils/raw/master/Kinetis/FSL_Kinetis_Bootloader_2.0.0_repack.zip
