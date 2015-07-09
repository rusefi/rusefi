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
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "6 dec 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_4 P1
U 1 1 54830CC1
P 3500 3450
F 0 "P1" V 3450 3450 50  0000 C CNN
F 1 "CONN_4" V 3550 3450 50  0000 C CNN
F 2 "" H 3500 3450 60  0000 C CNN
F 3 "" H 3500 3450 60  0000 C CNN
	1    3500 3450
	-1   0    0    -1  
$EndComp
$Comp
L CONN_4 P2
U 1 1 54830CCE
P 6000 3200
F 0 "P2" V 5950 3200 50  0000 C CNN
F 1 "CONN_4" V 6050 3200 50  0000 C CNN
F 2 "" H 6000 3200 60  0000 C CNN
F 3 "" H 6000 3200 60  0000 C CNN
	1    6000 3200
	1    0    0    1   
$EndComp
Wire Wire Line
	5250 3400 5650 3400
Wire Wire Line
	5650 3400 5650 3350
Wire Wire Line
	5650 3250 5200 3250
Wire Wire Line
	5200 3250 5200 3200
Wire Wire Line
	3850 3400 4400 3400
Wire Wire Line
	3850 3300 4400 3300
Wire Wire Line
	4400 3300 4400 3150
Wire Wire Line
	3850 3500 4150 3500
Wire Wire Line
	4150 3500 4150 3700
Wire Wire Line
	4150 3700 4400 3700
$Comp
L XTR116U U1
U 1 1 54830CB2
P 4800 3350
F 0 "U1" H 4600 3800 50  0000 C CNN
F 1 "XTR116U" H 4700 2900 50  0000 C CNN
F 2 "" H 4800 3350 60  0000 C CNN
F 3 "" H 4800 3350 60  0000 C CNN
	1    4800 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
