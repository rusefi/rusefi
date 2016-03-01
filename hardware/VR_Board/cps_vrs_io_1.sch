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
EELAYER 25 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 1 1
Title "cps_vrs_io_1"
Date "28 feb 2014"
Rev ""
Comp "based on http://code.google.com/p/daecu/"
Comment1 "cps_vrs_io_1"
Comment2 "Art_Electro"
Comment3 "Art_Electro"
Comment4 "Art_Electro"
$EndDescr
Wire Wire Line
	4450 1325 3975 1325
Wire Wire Line
	2700 1325 3175 1325
Wire Wire Line
	2700 1425 3175 1425
Wire Wire Line
	4450 1625 3975 1625
Wire Wire Line
	4450 1725 3975 1725
Wire Wire Line
	4450 1525 3975 1525
Wire Wire Line
	4450 1425 3975 1425
Text Notes 2700 1125 0    60   ~ 0
FIRMWARE PINOUT
Text Label 2950 1325 2    60   ~ 0
CAM
Text Label 2775 1425 0    60   ~ 0
CRANK
Entry Wire Line
	4450 1425 4550 1525
Entry Wire Line
	4450 1525 4550 1625
Entry Wire Line
	4450 1725 4550 1825
Entry Wire Line
	4450 1625 4550 1725
Entry Wire Line
	2600 1625 2700 1525
Entry Wire Line
	2600 1525 2700 1425
Entry Wire Line
	4450 1325 4550 1425
Entry Wire Line
	2600 1425 2700 1325
$Comp
L CONN_5X2 P103
U 1 1 5288F8BF
P 3575 1525
F 0 "P103" H 3575 1825 60  0000 C CNN
F 1 "CONN_5X2" V 3575 1525 50  0000 C CNN
F 2 "PIN_ARRAY_5x2" H 3575 1525 60  0001 C CNN
F 3 "" H 3575 1525 60  0000 C CNN
	1    3575 1525
	1    0    0    -1  
$EndComp
Text Label 4025 1525 0    60   ~ 0
knock1+
Text Label 4025 1625 0    60   ~ 0
knock1-
Text Label 3125 1525 2    60   ~ 0
CS-KN1
Text Label 4150 1725 2    40   ~ 0
SCK
Text Label 2900 1725 2    40   ~ 0
MISO
$Comp
L CONN_4 P104
U 1 1 5288FF50
P 9400 3375
F 0 "P104" V 9350 3375 50  0000 C CNN
F 1 "CONN_4" V 9450 3375 50  0000 C CNN
F 2 "PIN_ARRAY_4x1" H 9400 3375 60  0001 C CNN
F 3 "" H 9400 3375 60  0000 C CNN
	1    9400 3375
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5289002F
P 3125 1625
F 0 "#PWR06" H 3125 1625 30  0001 C CNN
F 1 "GND" H 3125 1555 30  0001 C CNN
F 2 "" H 3125 1625 60  0001 C CNN
F 3 "" H 3125 1625 60  0001 C CNN
	1    3125 1625
	0    1    1    0   
$EndComp
Entry Wire Line
	2600 1825 2700 1725
Wire Wire Line
	2700 1725 3175 1725
Wire Wire Line
	2700 1525 3175 1525
Text Label 4025 1425 0    60   ~ 0
5v stab
Text Label 4125 1325 2    60   ~ 0
5V
Wire Wire Line
	3175 1625 3125 1625
Wire Bus Line
	4550 850  4550 1825
Wire Bus Line
	2600 850  2600 1825
Wire Bus Line
	2600 850  2975 850 
$EndSCHEMATC
