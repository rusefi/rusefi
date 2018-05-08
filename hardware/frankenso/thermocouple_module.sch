EESchema Schematic File Version 2
LIBS:KICAD_Older_Version
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:176122-6
LIBS:art-electro-conn
LIBS:art-electro-conn_2
LIBS:art-electro-ic
LIBS:art-electro-max
LIBS:art-electro-power
LIBS:LMV324IDR
LIBS:logo
LIBS:logo_flipped
LIBS:MAX9926-9927
LIBS:max9939
LIBS:max31855
LIBS:stm32
LIBS:tc4427
LIBS:lm2596
LIBS:mos_p_numbered-pins
LIBS:rn-41
LIBS:skqucaa010_joystick
LIBS:itead_hc-0X_bluetooth
LIBS:joystick_SKRHAAE010
LIBS:STM32F407IGT6
LIBS:slc_oem
LIBS:mos_n_numbered-pins
LIBS:crystal(mc306)
LIBS:pololu_a4988
LIBS:Pololu_DRV8880
LIBS:fuse_BOM_only_no_pins
LIBS:frankenso-cache
EELAYER 25 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 11 15
Title "frankenso"
Date "2018-02-24"
Rev "0.5.3"
Comp "rusEFI.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
NoConn ~ 4075 4350
NoConn ~ 3875 4350
Text Notes 3900 4950 0    40   ~ 0
We want a big mass of copper in the\nTCPL joints, to dampen the cold junction\ntemperature and to make it more measurable\nwith this IC
Text Notes 3725 4450 0    40   ~ 0
Via for cable tie
Connection ~ 4800 4650
Wire Wire Line
	4800 4550 4800 4650
Wire Wire Line
	4700 4050 5150 4050
Wire Wire Line
	5150 4050 5150 4350
Wire Wire Line
	5150 4350 5400 4350
Wire Wire Line
	4700 4650 5150 4650
Wire Wire Line
	5150 4650 5150 4450
Wire Wire Line
	5150 4450 5400 4450
Wire Wire Line
	4800 4150 4800 4050
Connection ~ 4800 4050
$Comp
L C C1101
U 1 1 4E4C6B54
P 4800 4350
AR Path="/53BE1FC9/4E4C6B54" Ref="C1101"  Part="1" 
AR Path="/53BF9BE2/4E4C6B54" Ref="C1201"  Part="1" 
AR Path="/53BF9F6F/4E4C6B54" Ref="C1301"  Part="1" 
AR Path="/4E4C6B54" Ref="C?"  Part="1" 
AR Path="/53BFED1D/4E4C6B54" Ref="C1401"  Part="1" 
F 0 "C1101" H 4850 4450 50  0000 L CNN
F 1 "10nF" H 4850 4250 50  0000 L CNN
F 2 "SM0805-SM0603" V 1130 2600 60  0001 C CNN
F 3 "" H 4800 4350 60  0001 C CNN
F 4 "AVX,08051C103JAT2A" V 1130 2600 60  0001 C CNN "MFG,MFG#"
F 5 "DIGI,478-3745-6-ND" V 1130 2600 60  0001 C CNN "VEND1,VEND1#"
	1    4800 4350
	1    0    0    -1  
$EndComp
Text Notes 7250 4350 0    30   ~ 0
Care must be taken with the conector\nchosen for the TCPL to avoid inaccuracies.\n\nAlso, the connector must be as close as\npossible to the cold-juntion compensation.
Text Notes 5500 3650 0    40   ~ 0
MAX31855\nCold-Junction Compensated\nThermocouple-to-Digital Converter
Text Notes 5600 4900 0    30   ~ 0
Datasheet:\nhttp://datasheets.maxim-ic.com/en/ds/MAX31855.pdf
NoConn ~ 6500 4250
$Comp
L GND #PWR0180
U 1 1 4E13FFF9
P 7000 4000
AR Path="/53BE1FC9/4E13FFF9" Ref="#PWR0180"  Part="1" 
AR Path="/53BF9BE2/4E13FFF9" Ref="#PWR0182"  Part="1" 
AR Path="/53BF9F6F/4E13FFF9" Ref="#PWR0184"  Part="1" 
AR Path="/53BFED1D/4E13FFF9" Ref="#PWR0186"  Part="1" 
F 0 "#PWR0180" H 7000 4000 30  0001 C CNN
F 1 "GND" H 7000 3930 30  0001 C CNN
F 2 "" H 7000 4000 60  0001 C CNN
F 3 "" H 7000 4000 60  0001 C CNN
	1    7000 4000
	1    0    0    -1  
$EndComp
$Comp
L C C1102
U 1 1 4E13FFE2
P 7000 3800
AR Path="/53BE1FC9/4E13FFE2" Ref="C1102"  Part="1" 
AR Path="/53BF9BE2/4E13FFE2" Ref="C1202"  Part="1" 
AR Path="/53BF9F6F/4E13FFE2" Ref="C1302"  Part="1" 
AR Path="/4E13FFE2" Ref="C?"  Part="1" 
AR Path="/53BFED1D/4E13FFE2" Ref="C1402"  Part="1" 
F 0 "C1102" H 7050 3900 50  0000 L CNN
F 1 "100nF" H 7050 3700 50  0000 L CNN
F 2 "SM0805-SM0603" V 1130 2600 60  0001 C CNN
F 3 "" H 7000 3800 60  0001 C CNN
F 4 "AVX,08055C104KAT2A" V 1130 2600 60  0001 C CNN "MFG,MFG#"
F 5 "DIGI,478-1395-1-ND" V 1130 2600 60  0001 C CNN "VEND1,VEND1#"
	1    7000 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR0181
U 1 1 4E13FFDB
P 5400 4250
AR Path="/53BE1FC9/4E13FFDB" Ref="#PWR0181"  Part="1" 
AR Path="/53BF9BE2/4E13FFDB" Ref="#PWR0183"  Part="1" 
AR Path="/53BF9F6F/4E13FFDB" Ref="#PWR0185"  Part="1" 
AR Path="/53BFED1D/4E13FFDB" Ref="#PWR0187"  Part="1" 
F 0 "#PWR0181" H 5400 4250 30  0001 C CNN
F 1 "GND" H 5400 4180 30  0001 C CNN
F 2 "" H 5400 4250 60  0001 C CNN
F 3 "" H 5400 4250 60  0001 C CNN
	1    5400 4250
	1    0    0    -1  
$EndComp
$Comp
L MAX31855 U1101
U 1 1 4E13FF86
P 5950 4400
AR Path="/53BE1FC9/4E13FF86" Ref="U1101"  Part="1" 
AR Path="/53BF9BE2/4E13FF86" Ref="U1201"  Part="1" 
AR Path="/53BF9F6F/4E13FF86" Ref="U1301"  Part="1" 
AR Path="/4E13FF86" Ref="U?"  Part="1" 
AR Path="/53BFED1D/4E13FF86" Ref="U1401"  Part="1" 
F 0 "U1101" H 5950 4650 60  0000 C CNN
F 1 "MAX31855KASA" H 5950 4150 60  0000 C CNN
F 2 "SO8E" V 1130 2600 60  0001 C CNN
F 3 "" H 5950 4400 60  0001 C CNN
F 4 "DALLAS SEMI,MAX31855KASA+	" V 1130 2600 60  0001 C CNN "MFG,MFG#"
F 5 "DIGI,MAX31855KASA+-ND" V 1130 2600 60  0001 C CNN "VEND1,VEND1#"
	1    5950 4400
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1104
U 1 1 535A15DB
P 4350 4350
AR Path="/53BE1FC9/535A15DB" Ref="P1104"  Part="1" 
AR Path="/53BF9BE2/535A15DB" Ref="P1204"  Part="1" 
AR Path="/53BF9F6F/535A15DB" Ref="P1304"  Part="1" 
AR Path="/535A15DB" Ref="P?"  Part="1" 
AR Path="/53BFED1D/535A15DB" Ref="P1404"  Part="1" 
F 0 "P1104" V 4300 4350 40  0000 C CNN
F 1 "CONN_2" V 4400 4350 40  0000 C CNN
F 2 "rusEFI_LIB:tcpl_PCC-SMP" H 4350 4350 60  0001 C CNN
F 3 "" H 4350 4350 60  0000 C CNN
F 4 "omega,PCC-SMP-K" V 4350 4350 60  0001 C CNN "MFG,MFG#"
F 5 "DIGI,1471-1067-ND" V 4350 4350 60  0001 C CNN "VEND1,VEND1#"
	1    4350 4350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4700 4450 4700 4650
Wire Wire Line
	4700 4250 4700 4050
Connection ~ 5400 4250
$Comp
L CONN_2 P1103
U 1 1 535A1B1D
P 4350 3650
AR Path="/53BE1FC9/535A1B1D" Ref="P1103"  Part="1" 
AR Path="/53BF9BE2/535A1B1D" Ref="P1203"  Part="1" 
AR Path="/53BF9F6F/535A1B1D" Ref="P1303"  Part="1" 
AR Path="/535A1B1D" Ref="P?"  Part="1" 
AR Path="/53BFED1D/535A1B1D" Ref="P1403"  Part="1" 
F 0 "P1103" V 4300 3650 40  0000 C CNN
F 1 "CONN_2" V 4400 3650 40  0000 C CNN
F 2 "SIL-2" H 4350 3650 60  0001 C CNN
F 3 "" H 4350 3650 60  0000 C CNN
F 4 "on-shore,OSTVN02A150" V 4350 3650 60  0001 C CNN "MFG,MFG#"
F 5 "DIGI,ED10561-ND" V 4350 3650 60  0001 C CNN "VEND1,VEND1#"
	1    4350 3650
	-1   0    0    1   
$EndComp
Text Label 4700 4250 1    40   ~ 0
TCPL-
Text Label 4700 4650 1    40   ~ 0
TCPL+
Text Label 4700 3550 0    40   ~ 0
TCPL-
Text Label 4700 3750 0    40   ~ 0
TCPL+
$Comp
L CONN_1 P1101
U 1 1 53BE36C5
P 3875 4200
AR Path="/53BE1FC9/53BE36C5" Ref="P1101"  Part="1" 
AR Path="/53BF9BE2/53BE36C5" Ref="P1201"  Part="1" 
AR Path="/53BF9F6F/53BE36C5" Ref="P1301"  Part="1" 
AR Path="/53BE36C5" Ref="P?"  Part="1" 
AR Path="/53BFED1D/53BE36C5" Ref="P1401"  Part="1" 
F 0 "P1101" H 3955 4200 40  0000 L CNN
F 1 "CONN_1" H 3875 4255 30  0001 C CNN
F 2 "1pin" V 1130 2600 60  0001 C CNN
F 3 "" H 3875 4200 60  0001 C CNN
F 4 "DNP,DNP" V 1130 2600 60  0001 C CNN "MFG,MFG#"
F 5 "DNP,DNP" V 1130 2600 60  0001 C CNN "VEND1,VEND1#"
	1    3875 4200
	0    -1   -1   0   
$EndComp
$Comp
L CONN_1 P1102
U 1 1 53BE36F6
P 4075 4200
AR Path="/53BE1FC9/53BE36F6" Ref="P1102"  Part="1" 
AR Path="/53BF9BE2/53BE36F6" Ref="P1202"  Part="1" 
AR Path="/53BF9F6F/53BE36F6" Ref="P1302"  Part="1" 
AR Path="/53BE36F6" Ref="P?"  Part="1" 
AR Path="/53BFED1D/53BE36F6" Ref="P1402"  Part="1" 
F 0 "P1102" H 4155 4200 40  0000 L CNN
F 1 "CONN_1" H 4075 4255 30  0001 C CNN
F 2 "1pin" V 1130 2600 60  0001 C CNN
F 3 "" H 4075 4200 60  0001 C CNN
F 4 "DNP,DNP" V 1130 2600 60  0001 C CNN "MFG,MFG#"
F 5 "DNP,DNP" V 1130 2600 60  0001 C CNN "VEND1,VEND1#"
	1    4075 4200
	0    -1   -1   0   
$EndComp
Text Label 5400 4550 0    40   ~ 0
3.3v
Text HLabel 7000 3600 0    40   Input ~ 0
3.3v
Text HLabel 6500 4350 2    40   Input ~ 0
MISO
Text HLabel 6500 4450 2    40   Input ~ 0
/CS
Text HLabel 6500 4550 2    40   Input ~ 0
SCK
Text HLabel 5400 4250 1    40   Input ~ 0
GND
$EndSCHEMATC
