
cd ..

cp ../power_supply_kb1gtt/PWR_buck_12V_switcher.sch .

cp ../hip9011/DD_HIP9011.sch .


rem Let's copy Analog Input schematic 
cp ../adc_amp_divider.vertical12/adc_amp_divider.sch .

rem Same thing for 6 channel board
cp ../1A_injector_12-channels/inj_12ch.sch .

rem MMC/TTL module
cp ../usb_mmc_spi/mmc_usb_1.sch .

rem VR input
cp -r ../knock_VR_Art_ELectro/cps_vrs_io_1.sch .

rem Hi-Low driver

cp -r ../hi-lo_driver/hi-lo.sch .

rem CAN module

cp ../can_board/can_brd_1.sch .


rem Now let's merge all compoennt mappings automatically. Magic!
java -jar ../../java_tools/pcb_sync.jar cmp_merge frankenso.cmp ../hi-lo_driver/hi-lo.cmp ../can_board/can_brd_1.cmp ../1A_injector_12-channels/inj_12ch.cmp ../adc_amp_divider.vertical12/adc_amp_divider.cmp ../usb_mmc_spi/mmc_usb_1.cmp ../knock_VR_Art_ELectro/cps_vrs_io_1.cmp

mkdir temp


java -jar ../../java_tools/pcb_sync.jar pcb_merge ../usb_mmc_spi/mmc_usb_1.kicad_pcb temp/mmc_usb_1.kicad_pcb merge/mmc_ttl_prepare_changes.txt

java -jar ../../java_tools/pcb_sync.jar pcb_merge ../knock_VR_Art_ELectro/cps_vrs_io_1.kicad_pcb temp/cps_vrs_io_1.kicad_pcb merge/vr_changes.txt




rem Let's merge these PCBs into the frankenso!
rem all the pre-processed PCBs would be added by pcb_merge_changes.txt script
java -jar ../../java_tools/pcb_sync.jar pcb_merge ../frankenso_foundation/frankenso_foundation.kicad_pcb frankenso.kicad_pcb merge/pcb_merge_changes.txt


java -jar ../../java_tools/pcb_sync.jar bom_builder frankenstein.cmp digikey_parts.csv
