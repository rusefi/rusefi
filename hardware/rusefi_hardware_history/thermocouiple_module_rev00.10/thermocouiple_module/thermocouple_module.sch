EESchema Schematic File Version 2
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
LIBS:special
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
LIBS:max31855
LIBS:thermocouple_module-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Electronic Industrial Temperature Interface (EITI)"
Date "28 may 2014"
Rev ".001"
Comp "Mrk Industries"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
NoConn ~ 4075 4350
NoConn ~ 3875 4350
$Comp
L VIA V802
U 1 1 4F629521
P 3875 4350
F 0 "V802" V 3900 4450 20  0000 C CNN
F 1 "VIA" H 3875 4550 60  0001 C CNN
F 2 "1pin" H 3875 4650 60  0001 C CNN
F 3 "" H 3875 4350 60  0001 C CNN
	1    3875 4350
	0    1    1    0   
$EndComp
Text Notes 3900 4950 0    40   ~ 0
We want a big mass of copper in the\nTCPL joints, to dampen the cold junction\ntemperature and to make it more measurable\nwith this IC
Text Notes 3725 4450 0    40   ~ 0
Via for cable tie
$Comp
L VIA V801
U 1 1 4F627FCA
P 4075 4350
F 0 "V801" V 4100 4450 20  0000 C CNN
F 1 "VIA" H 4075 4550 60  0001 C CNN
F 2 "1pin" H 4075 4350 60  0001 C CNN
F 3 "" H 4075 4350 60  0001 C CNN
	1    4075 4350
	0    1    1    0   
$EndComp
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
L C C21
U 1 1 4E4C6B54
P 4800 4350
F 0 "C21" H 4850 4450 50  0000 L CNN
F 1 "10nF" H 4850 4250 50  0000 L CNN
F 2 "SM0805-SM0603" H 4850 4350 50  0001 C CNN
F 3 "" H 4800 4350 60  0001 C CNN
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
L GND #PWR01
U 1 1 4E13FFF9
P 7000 4000
F 0 "#PWR01" H 7000 4000 30  0001 C CNN
F 1 "GND" H 7000 3930 30  0001 C CNN
F 2 "" H 7000 4000 60  0001 C CNN
F 3 "" H 7000 4000 60  0001 C CNN
	1    7000 4000
	1    0    0    -1  
$EndComp
Text GLabel 7000 3600 0    40   Input ~ 0
3.3v
$Comp
L C C8
U 1 1 4E13FFE2
P 7000 3800
F 0 "C8" H 7050 3900 50  0000 L CNN
F 1 "100nF" H 7050 3700 50  0000 L CNN
F 2 "SM0805-SM0603" H 7050 3800 50  0001 C CNN
F 3 "" H 7000 3800 60  0001 C CNN
	1    7000 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 4E13FFDB
P 5400 4250
F 0 "#PWR02" H 5400 4250 30  0001 C CNN
F 1 "GND" H 5400 4180 30  0001 C CNN
F 2 "" H 5400 4250 60  0001 C CNN
F 3 "" H 5400 4250 60  0001 C CNN
	1    5400 4250
	1    0    0    -1  
$EndComp
Text GLabel 5400 4550 0    40   Input ~ 0
3.3v
$Comp
L MAX31855 U5
U 1 1 4E13FF86
P 5950 4400
F 0 "U5" H 5950 4650 60  0000 C CNN
F 1 "MAX31855KASA" H 5950 4150 60  0000 C CNN
F 2 "SO8E" H 5950 4200 60  0001 C CNN
F 3 "" H 5950 4400 60  0001 C CNN
F 4 "MAX31855KASA+" H 5950 4400 60  0001 C CNN "mfg#"
	1    5950 4400
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P1
U 1 1 535A15DB
P 4350 4350
F 0 "P1" V 4300 4350 40  0000 C CNN
F 1 "CONN_2" V 4400 4350 40  0000 C CNN
F 2 "tcpl_PCC-SMP" H 4350 4350 60  0001 C CNN
F 3 "" H 4350 4350 60  0000 C CNN
	1    4350 4350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4700 4450 4700 4650
Wire Wire Line
	4700 4250 4700 4050
$Comp
L CONN_4 P3
U 1 1 535A166C
P 7100 4550
F 0 "P3" V 7050 4550 50  0000 C CNN
F 1 "CONN_4" V 7150 4550 50  0000 C CNN
F 2 "SIL-4" H 7100 4550 60  0001 C CNN
F 3 "" H 7100 4550 60  0000 C CNN
	1    7100 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4350 6500 4400
Wire Wire Line
	6500 4400 6750 4400
Wire Wire Line
	6500 4450 6500 4500
Wire Wire Line
	6500 4500 6750 4500
Wire Wire Line
	6500 4550 6500 4600
Wire Wire Line
	6500 4600 6750 4600
Wire Wire Line
	5400 4550 5400 4700
Wire Wire Line
	5400 4700 6750 4700
$Comp
L CONN_1 P2
U 1 1 535A1737
P 5400 4100
F 0 "P2" H 5480 4100 40  0000 L CNN
F 1 "CONN_1" H 5400 4155 30  0001 C CNN
F 2 "SIL-1" H 5400 4100 60  0001 C CNN
F 3 "" H 5400 4100 60  0000 C CNN
	1    5400 4100
	0    -1   -1   0   
$EndComp
Connection ~ 5400 4250
$Comp
L CONN_2 P4
U 1 1 535A1B1D
P 4350 3650
F 0 "P4" V 4300 3650 40  0000 C CNN
F 1 "CONN_2" V 4400 3650 40  0000 C CNN
F 2 "SIL-2" H 4350 3650 60  0001 C CNN
F 3 "" H 4350 3650 60  0000 C CNN
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
Text Label 6500 4350 2    40   ~ 0
MISO
Text Label 6500 4450 2    40   ~ 0
/CS
Text Label 6500 4550 2    40   ~ 0
SCK
$EndSCHEMATC
