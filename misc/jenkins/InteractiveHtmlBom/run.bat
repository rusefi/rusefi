set kicad_path=c:\progra~1\KiCad
set ibom_cmd=%kicad_path%\bin\python.exe C:\stuff\InteractiveHtmlBom\InteractiveHtmlBom\generate_interactive_bom.py --no-browser --name-format %%f_latest --dest-dir ../ibom 
echo "ibom_cmd=%ibom_cmd%"
%ibom_cmd% hardware/frankenso/frankenso.kicad_pcb
%ibom_cmd% hardware/CJ125_board/O2_input_CJ125.kicad_pcb