@echo off

set inputFile=../Common_Rail_MC33816.csv
set outputFile=MC33816_R0.3-bom.csv

if not exist %inputFile% echo Not found %inputFile%
if not exist %inputFile% exit -1

echo "Translating from %inputFile% to %outputFile%"

echo Comment,Designator,Footprint,LCSC Part #> %outputFile%
                      

grep JLCPCB %inputFile% > %outputFile%.temp

sed -f removedoublecomma.sed %outputFile%.temp | gawk -F "," '{print $3 "," $1 "," $4 "," $10}' >> %outputFile%

rm -f %outputFile%.temp
