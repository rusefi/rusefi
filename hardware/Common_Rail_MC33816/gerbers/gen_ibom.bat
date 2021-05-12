
set version=0.3.2
set dest_dir=gerbers\%version%
mkdir %version%

set kicad_path=c:\progra~1\KiCad
set ibom_cmd=%kicad_path%\bin\python.exe ../../InteractiveHtmlBom\InteractiveHtmlBom\generate_interactive_bom.py --no-browser --name-format %%f_%version% --dest-dir %dest_dir%
%ibom_cmd% ../Common_Rail_MC33816.kicad_pcb