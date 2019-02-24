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
LIBS:art-electro-conn
LIBS:art-electro-conn_2
LIBS:logo
LIBS:logo_flipped
LIBS:NUCLEO
LIBS:NUCLEO144_wing-cache
EELAYER 25 0
EELAYER END
$Descr B 17000 11000
encoding utf-8
Sheet 1 1
Title "nucleo144_wing"
Date "2019-01-13"
Rev "0.1"
Comp "rusEFI.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L NUCLEO144-F429ZI U1
U 1 1 5C728F9A
P 5000 4800
F 0 "U1" H 4100 8525 50  0000 R CNN
F 1 "NUCLEO144-F429ZI" H 4100 8450 50  0000 R CNN
F 2 "rusEFI_LIB:ST_Morpho_Connector_144_STLink" H 5850 1150 50  0001 L CNN
F 3 "" H 4100 5100 50  0001 C CNN
	1    5000 4800
	1    0    0    -1  
$EndComp
$Comp
L NUCLEO144-F429ZI U2
U 1 1 5C729630
P 9700 5000
F 0 "U2" H 8800 8725 50  0000 R CNN
F 1 "NUCLEO144-F429ZI" H 8800 8650 50  0000 R CNN
F 2 "rusEFI_LIB:ST_Morpho_Connector_144_STLink" H 10550 1350 50  0001 L CNN
F 3 "" H 8800 5300 50  0001 C CNN
	1    9700 5000
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x40_Odd_Even J2
U 1 1 5C729B67
P 13950 4500
F 0 "J2" H 14000 6500 50  0000 C CNN
F 1 "Conn_02x40_Odd_Even" H 14000 2400 50  0000 C CNN
F 2 "" H 13950 4500 50  0001 C CNN
F 3 "" H 13950 4500 50  0001 C CNN
	1    13950 4500
	1    0    0    -1  
$EndComp
$Comp
L Conn_02x36_Top_Bottom J1
U 1 1 5C729C2B
P 12100 4050
F 0 "J1" H 12150 5850 50  0000 C CNN
F 1 "Conn_02x36_Top_Bottom" H 12150 2150 50  0000 C CNN
F 2 "" H 12100 4050 50  0001 C CNN
F 3 "" H 12100 4050 50  0001 C CNN
	1    12100 4050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
