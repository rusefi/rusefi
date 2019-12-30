
set inputFile=../Common_Rail_MC33816.csv
set outputFile=MC33816_R0.3-bom.csv


echo "Translating from %inputFile% to %outputFile%"


echo Comment,Designator,Footprint,LCSC Part #> %outputFile%
                       
grep JLCPCB %inputFile%

grep JLCPCB %inputFile% > %outputFile%.temp

sed -f removedoublecomma.sed %outputFile%.temp > %outputFile%.all




rem  | gawk -F "," '{print $4}'


