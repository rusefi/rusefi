rem This script could be invoked from any folder - it would clone hardware projects into current folder
rem All outputs go into same "ibom" local directory


git clone https://github.com/rusefi/hw_modular.git
cd hw_modular
git pull
cd ..

git clone https://github.com/rusefi/hw_microRusEfi
cd hw_microRusEfi
git pull
cd ..

set kicad_path=c:\progra~1\KiCad
set ibom_cmd=%kicad_path%\bin\python.exe C:\stuff\InteractiveHtmlBom\InteractiveHtmlBom\generate_interactive_bom.py --no-browser --name-format %%f_latest --dest-dir ../ibom 
echo "ibom_cmd=%ibom_cmd%"
%ibom_cmd% --extra-fields "Part #","VEND#" hw_microRusEfi/microRusEfi.kicad_pcb --netlist-file hw_microRusEfi/microRusEfi.net

set ibom_cmd=%kicad_path%\bin\python.exe C:\stuff\InteractiveHtmlBom\InteractiveHtmlBom\generate_interactive_bom.py --no-browser --name-format %%f_latest --dest-dir ../../ibom 
echo "ibom_cmd=%ibom_cmd%"

%ibom_cmd% --extra-fields "Part #","VEND#" hw_modular/cj125_Module/cj125_Module.kicad_pcb
%ibom_cmd% --extra-fields "Part #","VEND#" hw_modular/TLE8888_Module/TLE8888_Module.kicad_pcb --netlist-file hw_modular/TLE8888_Module/TLE8888_Module.net
%ibom_cmd% --extra-fields "Part #","VEND#" hw_modular/TLE7209_or_MC33186_H-Bridge_Breakout/TLE7209_Module.kicad_pcb --netlist-file hw_modular/TLE7209_or_MC33186_H-Bridge_Breakout/TLE7209_Module.net 
%ibom_cmd% hw_modular/Analog_Input_Module/Analog_Input_Module.kicad_pcb
%ibom_cmd% hw_modular/TLE8110ED_breakout_board/TLE8110ED.kicad_pcb
%ibom_cmd% hw_modular/highside_module/hi-lo.kicad_pcb


echo Uploading IBOMs
ncftpput -R -v -u %RUSEFI_DOXYGEN_FTP_USER% -p %RUSEFI_DOXYGEN_FTP_PASS% %RUSEFI_FTP_SERVER% /ibom ibom/*
