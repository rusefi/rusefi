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
LIBS:LMV324IDR
LIBS:logo
LIBS:logo_flipped
LIBS:project_specific_libs
LIBS:art-electro-conn
LIBS:art-electro-ic
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 8
Title ""
Date "8 feb 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_4X2 P51
U 1 1 52DB4402
P 6300 4700
F 0 "P51" H 6300 4950 50  0000 C CNN
F 1 "CONN_4X2" V 6300 4700 40  0000 C CNN
F 2 "" H 6300 4700 60  0000 C CNN
F 3 "" H 6300 4700 60  0000 C CNN
	1    6300 4700
	1    0    0    -1  
$EndComp
$Comp
L CONN_10X2 P52
U 1 1 52DB4411
P 2950 4400
F 0 "P52" H 2950 4950 60  0000 C CNN
F 1 "CONN_10X2" V 2950 4300 50  0000 C CNN
F 2 "" H 2950 4400 60  0000 C CNN
F 3 "" H 2950 4400 60  0000 C CNN
	1    2950 4400
	1    0    0    -1  
$EndComp
NoConn ~ 5900 4650
NoConn ~ 6700 4550
Text Label 6700 4850 0    60   ~ 0
GND
$Comp
L GND #PWR01
U 1 1 52DB443D
P 6850 5000
F 0 "#PWR01" H 6850 5000 30  0001 C CNN
F 1 "GND" H 6850 4930 30  0001 C CNN
F 2 "" H 6850 5000 60  0000 C CNN
F 3 "" H 6850 5000 60  0000 C CNN
	1    6850 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 4850 6850 4850
Wire Wire Line
	6850 4850 6850 5000
Text Label 5900 4850 2    60   ~ 0
GND
NoConn ~ 2550 4650
NoConn ~ 3350 4750
NoConn ~ 3350 4650
NoConn ~ 3350 4550
Text Label 3350 4850 0    60   ~ 0
GND
Text Label 2550 4850 2    60   ~ 0
GND
$Comp
L CONN_2 P53
U 1 1 52DB49B9
P 8200 3600
F 0 "P53" V 8150 3600 40  0000 C CNN
F 1 "CONN_2" V 8250 3600 40  0000 C CNN
F 2 "" H 8200 3600 60  0000 C CNN
F 3 "" H 8200 3600 60  0000 C CNN
	1    8200 3600
	1    0    0    -1  
$EndComp
NoConn ~ 7850 3500
NoConn ~ 7850 3700
$Comp
L CONN_5X2 P54
U 1 1 52DB49D4
P 2950 3650
F 0 "P54" H 2950 3950 60  0000 C CNN
F 1 "CONN_5X2" V 2950 3650 50  0000 C CNN
F 2 "" H 2950 3650 60  0000 C CNN
F 3 "" H 2950 3650 60  0000 C CNN
	1    2950 3650
	1    0    0    -1  
$EndComp
NoConn ~ 2550 3650
NoConn ~ 2550 3550
NoConn ~ 2550 3450
NoConn ~ 3350 3450
NoConn ~ 3350 3550
Wire Wire Line
	5150 4750 5900 4750
Wire Wire Line
	5900 4550 5550 4550
Text Label 3350 4750 0    60   ~ 0
NC
Text Label 3350 4650 0    60   ~ 0
PD14
Text Label 3350 4550 0    60   ~ 0
PD12
Text Label 3350 4450 0    60   ~ 0
PD10
NoConn ~ 2550 4750
Text Label 3350 4350 0    60   ~ 0
PD8
Text Label 3350 4250 0    60   ~ 0
PB14
Wire Wire Line
	3900 4150 3350 4150
Text Label 3600 4150 0    60   ~ 0
PB12
$Comp
L JUMPER JP52
U 1 1 52E33BA4
P 5250 4550
F 0 "JP52" H 5250 4700 60  0000 C CNN
F 1 "JUMPER" H 5250 4470 40  0000 C CNN
F 2 "~" H 5250 4550 60  0000 C CNN
F 3 "~" H 5250 4550 60  0000 C CNN
	1    5250 4550
	1    0    0    -1  
$EndComp
Text Label 5900 4550 2    60   ~ 0
PA8
Text Label 7500 4750 0    60   ~ 0
H_IN5
Text Label 7500 4650 0    60   ~ 0
H_IN6
Text Label 2550 4550 2    60   ~ 0
PD11
Text Label 2550 4450 2    60   ~ 0
PD9
Text Label 2550 4350 2    60   ~ 0
PB15
Text Label 2550 4250 2    60   ~ 0
PB13
Text Label 2550 4150 2    60   ~ 0
PB11
Text Label 2550 4050 2    60   ~ 0
PE15
Text Label 2550 3950 2    60   ~ 0
PE13
Text Label 3350 4050 0    60   ~ 0
PB10
$Comp
L CONN_6 P56
U 1 1 52E349DA
P 1200 4600
F 0 "P56" V 1150 4600 60  0000 C CNN
F 1 "CONN_6" V 1250 4600 60  0000 C CNN
F 2 "" H 1200 4600 60  0000 C CNN
F 3 "" H 1200 4600 60  0000 C CNN
	1    1200 4600
	-1   0    0    1   
$EndComp
$Comp
L CONN_6 P57
U 1 1 52E349FD
P 1200 4000
F 0 "P57" V 1150 4000 60  0000 C CNN
F 1 "CONN_6" V 1250 4000 60  0000 C CNN
F 2 "" H 1200 4000 60  0000 C CNN
F 3 "" H 1200 4000 60  0000 C CNN
	1    1200 4000
	-1   0    0    1   
$EndComp
Text Label 1550 4850 0    60   ~ 0
GND
Text Label 1550 4750 0    60   ~ 0
PD10
Wire Wire Line
	2550 4550 1550 4550
Wire Wire Line
	2550 4450 1550 4450
Wire Wire Line
	2550 4350 1550 4350
Wire Wire Line
	2550 4250 1550 4250
Wire Wire Line
	2550 4150 1550 4150
Wire Wire Line
	2550 4050 1550 4050
Wire Wire Line
	2550 3950 1550 3950
Text Label 1550 4650 0    60   ~ 0
PD8
Text Label 1550 5200 0    60   ~ 0
PB14
Text Label 1550 5300 0    60   ~ 0
PB10
Text Label 2550 3850 2    60   ~ 0
PE11
Text Label 2550 3750 2    60   ~ 0
PE9
Text Label 2550 3650 2    60   ~ 0
PE7
$Comp
L CONN_6 P58
U 1 1 52E3E69E
P 1200 5450
F 0 "P58" V 1150 5450 60  0000 C CNN
F 1 "CONN_6" V 1250 5450 60  0000 C CNN
F 2 "" H 1200 5450 60  0000 C CNN
F 3 "" H 1200 5450 60  0000 C CNN
	1    1200 5450
	-1   0    0    1   
$EndComp
Wire Wire Line
	2550 3850 1550 3850
Wire Wire Line
	1550 3750 2550 3750
Text Label 4950 4550 2    60   ~ 0
CRANK
Text Label 5650 4750 2    60   ~ 0
PC6
Text Label 4500 4150 0    60   ~ 0
CAN_RX
$Comp
L CONN_6 P_VOID1
U 1 1 52E444C3
P 4400 6150
F 0 "P_VOID1" V 4350 6150 60  0000 C CNN
F 1 "CONN_6" V 4450 6150 60  0000 C CNN
F 2 "" H 4400 6150 60  0000 C CNN
F 3 "" H 4400 6150 60  0000 C CNN
	1    4400 6150
	-1   0    0    1   
$EndComp
Text Label 4550 4750 2    60   ~ 0
CAM
Text Label 4750 5900 0    60   ~ 0
CAM
NoConn ~ 4750 6400
NoConn ~ 4750 6300
$Comp
L JUMPER JP53
U 1 1 52E445CA
P 4850 4750
F 0 "JP53" H 4850 4900 60  0000 C CNN
F 1 "JUMPER" H 4850 4670 40  0000 C CNN
F 2 "~" H 4850 4750 60  0000 C CNN
F 3 "~" H 4850 4750 60  0000 C CNN
	1    4850 4750
	1    0    0    -1  
$EndComp
$Comp
L JUMPER JP51
U 1 1 52E445DC
P 4200 4150
F 0 "JP51" H 4200 4300 60  0000 C CNN
F 1 "JUMPER" H 4200 4070 40  0000 C CNN
F 2 "~" H 4200 4150 60  0000 C CNN
F 3 "~" H 4200 4150 60  0000 C CNN
	1    4200 4150
	1    0    0    -1  
$EndComp
Text Label 4750 6000 0    60   ~ 0
CAN_RX
Text Notes 3400 5550 0    60   ~ 0
The purpose of this VOID connector is to trick KiCad\ninto assigning proper net name to the jumper pad\nwhich would not happen if the pad is unconnected
Text Label 4750 6100 0    60   ~ 0
CRANK
$Comp
L CONN_6 P55
U 1 1 52E474FE
P 7750 5550
F 0 "P55" V 7700 5550 60  0000 C CNN
F 1 "CONN_6" V 7800 5550 60  0000 C CNN
F 2 "" H 7750 5550 60  0000 C CNN
F 3 "" H 7750 5550 60  0000 C CNN
	1    7750 5550
	-1   0    0    1   
$EndComp
Text Label 8100 5500 0    60   ~ 0
GND
Text Label 3800 3750 0    60   ~ 0
H_IN1
Text Label 3800 3850 0    60   ~ 0
H_IN2
Text Label 8100 5400 0    60   ~ 0
GND
$Comp
L CONN_1 P610
U 1 1 52E831CD
P 5650 6200
F 0 "P610" H 5730 6200 40  0000 L CNN
F 1 "CONN_1" H 5650 6255 30  0001 C CNN
F 2 "" H 5650 6200 60  0000 C CNN
F 3 "" H 5650 6200 60  0000 C CNN
	1    5650 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 6200 4750 6200
Text Label 5200 6200 0    60   ~ 0
VP
Text Notes 6700 4750 0    60   ~ 0
PC7
Text Notes 6700 4650 0    60   ~ 0
PC9
Wire Wire Line
	7500 4650 6700 4650
Wire Wire Line
	7500 4750 6700 4750
Text Notes 1800 4450 0    60   ~ 0
PD9
Text Notes 1800 4350 0    60   ~ 0
PB15
Text Notes 1800 4250 0    60   ~ 0
PB13
Text Notes 1800 4150 0    60   ~ 0
PB11
Wire Wire Line
	3900 3950 3350 3950
Text Label 3800 3950 0    60   ~ 0
H_IN4
Text Label 3350 3550 0    60   ~ 0
PB2
Wire Wire Line
	3900 3850 3350 3850
Wire Wire Line
	3900 3750 3350 3750
Wire Wire Line
	3900 3650 3350 3650
Text Label 3800 3650 0    60   ~ 0
H_IN3
Text Label 8100 5700 0    60   ~ 0
H_IN5
Text Label 8100 5600 0    60   ~ 0
H_IN6
Text Notes 3350 3650 0    60   ~ 0
PE8
Text Notes 3350 3750 0    60   ~ 0
PE10
Text Notes 3350 3850 0    60   ~ 0
PE12
Text Notes 3350 3950 0    60   ~ 0
PE14
$Comp
L CONN_6 P_VOID2
U 1 1 52EB01FA
P 4400 6900
F 0 "P_VOID2" V 4350 6900 60  0000 C CNN
F 1 "CONN_6" V 4450 6900 60  0000 C CNN
F 2 "" H 4400 6900 60  0000 C CNN
F 3 "" H 4400 6900 60  0000 C CNN
	1    4400 6900
	-1   0    0    1   
$EndComp
NoConn ~ 4750 7150
NoConn ~ 4750 7050
Text Label 4750 6650 0    60   ~ 0
H_IN1
Text Label 4750 6750 0    60   ~ 0
H_IN2
Text Label 4750 6850 0    60   ~ 0
H_IN4
Text Label 4750 6950 0    60   ~ 0
H_IN3
Text Label 8100 5800 0    60   ~ 0
GND
Text Label 8100 5300 0    60   ~ 0
GND
Text Label 1550 5400 0    60   ~ 0
GND
Text Label 1550 5500 0    60   ~ 0
GND
Text Label 1550 5600 0    60   ~ 0
GND
Text Label 1550 5700 0    60   ~ 0
GND
$EndSCHEMATC
