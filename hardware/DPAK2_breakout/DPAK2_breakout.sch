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
LIBS:DDPAK_breakout-cache
EELAYER 27 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "DDPAK breakout"
Date "23 dec 2014"
Rev "R0.1"
Comp "rusEFI"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R R2
U 1 1 54499C42
P 5900 4350
F 0 "R2" V 5980 4350 40  0000 C CNN
F 1 "100K" V 5907 4351 40  0000 C CNN
F 2 "SM2010" V 5830 4350 30  0001 C CNN
F 3 "~" H 5900 4350 30  0000 C CNN
	1    5900 4350
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 54499D02
P 5600 4100
F 0 "R1" V 5680 4100 40  0000 C CNN
F 1 "1K" V 5607 4101 40  0000 C CNN
F 2 "SM0805_jumper" V 5530 4100 30  0001 C CNN
F 3 "~" H 5600 4100 30  0000 C CNN
	1    5600 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5850 4100 6450 4100
Connection ~ 5900 4100
Wire Wire Line
	6750 4300 6750 4600
Connection ~ 6750 4600
$Comp
L CONN_2 P2
U 1 1 5449A1BA
P 7250 3950
F 0 "P2" V 7200 3950 40  0000 C CNN
F 1 "CONN_2" V 7300 3950 40  0001 C CNN
F 2 "AK300-2" H 7250 3950 60  0001 C CNN
F 3 "" H 7250 3950 60  0000 C CNN
	1    7250 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4600 6900 4050
Wire Wire Line
	5350 4600 6900 4600
Connection ~ 5900 4600
Wire Wire Line
	6900 3850 6750 3850
$Comp
L CONN_1 P1
U 1 1 544A1CBF
P 5200 4100
F 0 "P1" H 5280 4100 40  0000 L CNN
F 1 "CONN_1" H 5200 4155 30  0001 C CNN
F 2 "SIL-1" H 5200 4100 60  0001 C CNN
F 3 "" H 5200 4100 60  0000 C CNN
	1    5200 4100
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 P3
U 1 1 544A1CD8
P 5200 4600
F 0 "P3" H 5280 4600 40  0000 L CNN
F 1 "CONN_1" H 5200 4655 30  0001 C CNN
F 2 "SIL-1" H 5200 4600 60  0001 C CNN
F 3 "" H 5200 4600 60  0000 C CNN
	1    5200 4600
	-1   0    0    1   
$EndComp
Text Label 5350 4100 1    60   ~ 0
SIG
Text Label 6100 4600 0    60   ~ 0
GND
Text Label 6750 3850 0    60   ~ 0
HV
Text Label 6000 4100 0    60   ~ 0
SIG2
$Comp
L TIP121 Q1
U 1 1 5498B4BA
P 6650 4100
F 0 "Q1" H 6600 3950 40  0000 R CNN
F 1 "IRGS14C40L" H 6650 4250 40  0001 R CNN
F 2 "DPAK2" H 6530 4205 29  0001 C CNN
F 3 "~" H 6650 4100 60  0000 C CNN
	1    6650 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3850 6750 3900
$EndSCHEMATC
