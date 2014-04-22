EESchema Schematic File Version 2
LIBS:project_specific_libs
LIBS:power
LIBS:device
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
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:contrib
LIBS:IO_protect_1x-cache
EELAYER 27 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "input IO protection"
Date "3 jan 2014"
Rev "A"
Comp "http://code.google.com/p/daecu/"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ -4675 3450
$Comp
L C C1
U 1 1 50FBE02E
P 8900 5300
F 0 "C1" H 8710 5400 50  0000 L CNN
F 1 "1uF" H 8650 5200 50  0000 L CNN
F 2 "SM0805" V -5770 4075 60  0001 C CNN
F 3 "" H 8900 5300 60  0001 C CNN
F 4 "YAGEO,CC0805KKX7R8BB105" V -5770 4075 60  0001 C CNN "mfg,mfg#"
F 5 "Seeedstudio OPL,CC0805KKX7R8BB105" V -5770 4075 60  0001 C CNN "vend1,vend1#"
	1    8900 5300
	0    1    1    0   
$EndComp
$Comp
L MOS_PRO Q1
U 1 1 50D6F30C
P 6550 6400
F 0 "Q1" H 6550 6590 60  0000 R CNN
F 1 "MOS_PRO" H 6550 6220 60  0001 R CNN
F 2 "SOT23" H 6550 6400 60  0001 C CNN
F 3 "" H 6550 6400 60  0001 C CNN
F 4 "onsemi,NTR1P02T1" H 6550 6400 60  0001 C CNN "mfg,mfg#"
F 5 "digi,NTR1P02T1GOSCT-ND" H 6550 6400 60  0001 C CNN "vend1,vend1#"
	1    6550 6400
	-1   0    0    1   
$EndComp
$Comp
L R R3
U 1 1 4AD9C75B
P 5950 6450
F 0 "R3" V 6030 6450 50  0000 C CNN
F 1 "10k" V 5950 6450 50  0000 C CNN
F 2 "SM0603" V -8220 7225 60  0001 C CNN
F 3 "" H 5950 6450 60  0001 C CNN
F 4 "Yageo,RC0603FR-0710KL" V -8220 7225 60  0001 C CNN "mfg,mfg#"
F 5 "seeed,0011240p1" V -8220 7225 60  0001 C CNN "vend1,vend1#"
	1    5950 6450
	-1   0    0    -1  
$EndComp
$Comp
L POT RV1
U 1 1 52877F58
P 7200 6400
F 0 "RV1" H 7200 6300 50  0000 C CNN
F 1 "500R" H 7200 6400 50  0000 C CNN
F 2 "POT_TC33X-2" H 7200 6400 60  0001 C CNN
F 3 "~" H 7200 6400 60  0000 C CNN
F 4 "Bourns,TC33X-2-501E" H 7200 6400 60  0001 C CNN "mfg,mfg#"
F 5 "newark,60H9655" H 7200 6400 60  0001 C CNN "vend1,vend1#"
	1    7200 6400
	0    -1   -1   0   
$EndComp
$Comp
L MOS_PRO Q2
U 1 1 52877F67
P 7500 6250
F 0 "Q2" H 7500 6440 60  0000 R CNN
F 1 "MOS_PRO" H 7500 6070 60  0001 R CNN
F 2 "SOT23" H 7500 6250 60  0001 C CNN
F 3 "" H 7500 6250 60  0001 C CNN
F 4 "onsemi,NTR1P02T1" H 7500 6250 60  0001 C CNN "mfg,mfg#"
F 5 "digi,NTR1P02T1GOSCT-ND" H 7500 6250 60  0001 C CNN "vend1,vend1#"
	1    7500 6250
	0    1    -1   0   
$EndComp
$Comp
L R R1
U 1 1 52877F97
P 4700 5950
F 0 "R1" V 4780 5950 50  0000 C CNN
F 1 "243k" V 4700 5950 50  0000 C CNN
F 2 "SM0805" V -9470 6725 60  0001 C CNN
F 3 "" H 4700 5950 60  0001 C CNN
F 4 "vishay,CRCW0805240KFKEA" V -9470 6725 60  0001 C CNN "mfg,mfg#"
F 5 "digi,541-240KCCT-ND" V -9470 6725 60  0001 C CNN "vend1,vend1#"
	1    4700 5950
	0    1    -1   0   
$EndComp
$Comp
L JUMPER3 JP1
U 1 1 528790A1
P 9400 6600
F 0 "JP1" H 9450 6500 40  0000 L CNN
F 1 "JUMPER3" H 9400 6700 40  0001 C CNN
F 2 "SIL-3" H 9400 6600 60  0001 C CNN
F 3 "~" H 9400 6600 60  0000 C CNN
F 4 "-,-" H 9400 6600 60  0001 C CNN "mfg,mfg#"
F 5 "seeed,3460810p1" H 9400 6600 60  0001 C CNN "vend1,vend1#"
	1    9400 6600
	0    1    1    0   
$EndComp
$Comp
L VREF D2
U 1 1 52879EF4
P 7200 6750
F 0 "D2" H 7200 6850 40  0000 C CNN
F 1 "VREF" H 7200 6650 30  0000 C CNN
F 2 "SOT323" H 7200 6750 60  0001 C CNN
F 3 "~" H 7200 6750 60  0000 C CNN
F 4 "Analog,ADR5044BRTZ-R2" H 7200 6750 60  0001 C CNN "mfg,mfg#"
F 5 "digi,ADR5044BRTZ-R2" H 7200 6750 60  0001 C CNN "vend1,vend1#"
	1    7200 6750
	0    -1   -1   0   
$EndComp
$Comp
L TEST W1
U 1 1 5287A060
P 9200 6450
F 0 "W1" H 9200 6510 40  0000 C CNN
F 1 "JMP" H 9200 6380 40  0001 C CNN
F 2 "SIL-2" H 9200 6450 60  0001 C CNN
F 3 "" H 9200 6450 60  0001 C CNN
	1    9200 6450
	0    -1   -1   0   
$EndComp
$Comp
L MCP6G0_ U1
U 1 1 5287A5E4
P 9200 5950
F 0 "U1" H 9500 6200 70  0000 C CNN
F 1 "MCP6G0_" H 9600 5700 70  0000 C CNN
F 2 "MSOP_8" H 9200 5950 60  0001 C CNN
F 3 "" H 9200 5950 60  0000 C CNN
F 4 "microchip,MCP6G01T-E/OT" H 9200 5950 60  0001 C CNN "mfg,mfg#"
F 5 "digi,MCP6G01T-E/OT" H 9200 5950 60  0001 C CNN "vend1,vend1#"
	1    9200 5950
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P1
U 1 1 5287A625
P 9250 5300
F 0 "P1" H 9330 5300 40  0000 L CNN
F 1 "CONN_1" H 9250 5355 30  0001 C CNN
F 2 "SIL-1" H 9250 5300 60  0001 C CNN
F 3 "" H 9250 5300 60  0000 C CNN
	1    9250 5300
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P3
U 1 1 5287A62B
P 10450 6850
F 0 "P3" H 10530 6850 40  0000 L CNN
F 1 "CONN_1" H 10450 6905 30  0001 C CNN
F 2 "SIL-1" H 10450 6850 60  0001 C CNN
F 3 "" H 10450 6850 60  0000 C CNN
	1    10450 6850
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P2
U 1 1 5287A631
P 10450 5950
F 0 "P2" H 10530 5950 40  0000 L CNN
F 1 "CONN_1" H 10450 6005 30  0001 C CNN
F 2 "SIL-1" H 10450 5950 60  0001 C CNN
F 3 "" H 10450 5950 60  0000 C CNN
	1    10450 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 6850 9100 6250
Wire Wire Line
	9300 6250 9300 6600
Wire Wire Line
	5950 6850 10300 6850
$Comp
L GND #PWR01
U 1 1 5287A84F
P 9400 6900
F 0 "#PWR01" H 9400 6900 30  0001 C CNN
F 1 "GND" H 9400 6830 30  0001 C CNN
F 2 "" H 9400 6900 60  0000 C CNN
F 3 "" H 9400 6900 60  0000 C CNN
	1    9400 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 6850 9400 6900
Wire Wire Line
	5950 5950 5950 6200
Connection ~ 6450 5950
Wire Wire Line
	7200 6150 7300 6150
Wire Wire Line
	7500 6450 7500 6850
Connection ~ 9100 6850
$Comp
L R R4
U 1 1 5287A8C9
P 9850 5950
F 0 "R4" V 9930 5950 50  0000 C CNN
F 1 "150R" V 9850 5950 50  0000 C CNN
F 2 "SM0805" V -4320 6725 60  0001 C CNN
F 3 "" H 9850 5950 60  0001 C CNN
F 4 "bourns,CR0805-FX-1500ELF" V -4320 6725 60  0001 C CNN "mfg,mfg#"
F 5 "digi,CR0805-FX-1500ELFCT-ND" V -4320 6725 60  0001 C CNN "vend1,vend1#"
	1    9850 5950
	0    1    -1   0   
$EndComp
Connection ~ 7500 6850
Connection ~ 7200 6850
Connection ~ 6450 6850
Wire Wire Line
	6450 6600 6450 6850
Wire Wire Line
	5950 6700 5950 6850
Connection ~ 5950 5950
Wire Wire Line
	6450 5950 6450 6200
Wire Wire Line
	6750 6400 7050 6400
Connection ~ 9100 5300
Connection ~ 9100 5650
$Comp
L GND #PWR02
U 1 1 5287AA37
P 8700 5700
F 0 "#PWR02" H 8700 5700 30  0001 C CNN
F 1 "GND" H 8700 5630 30  0001 C CNN
F 2 "" H 8700 5700 60  0000 C CNN
F 3 "" H 8700 5700 60  0000 C CNN
	1    8700 5700
	1    0    0    -1  
$EndComp
NoConn ~ 9200 5650
NoConn ~ 9300 5650
Text Label 9100 5300 2    60   ~ 0
V+
Text Label 7800 6150 2    60   ~ 0
V+
Text Label 9400 6350 0    60   ~ 0
V+
Text Label 9200 6750 1    60   ~ 0
V+
Wire Wire Line
	7700 6150 7800 6150
Text Label 10100 5950 0    60   ~ 0
Vmcu
Wire Wire Line
	10100 5950 10300 5950
Text Label 10300 6850 2    60   ~ 0
GND
$Comp
L DIODE D1
U 1 1 5287AEA9
P 6800 6200
F 0 "D1" H 6800 6300 40  0000 C CNN
F 1 "DIODE" H 6800 6100 40  0001 C CNN
F 2 "SOD-123" H 6800 6200 60  0001 C CNN
F 3 "SOD-123" H 6800 6200 60  0001 C CNN
F 4 "ON-SEMI,BAT54T1G" H 6800 6200 60  0001 C CNN "mfg,mfg#"
F 5 "Seeed,SS0520-7-F" H 6800 6200 60  0001 C CNN "vend1,vend1#"
F 6 "digi,BAT54T1GOSCT-ND" H 6800 6200 60  0001 C CNN "vend2,vend2"
	1    6800 6200
	0    1    1    0   
$EndComp
Connection ~ 6800 6400
Wire Wire Line
	6800 6000 6800 5950
Connection ~ 6800 5950
Wire Wire Line
	9200 6650 9200 6750
Text Label 7100 5950 0    60   ~ 0
signal
Text Notes 9550 6550 0    60   ~ 0
GAIN 50\nGAIN 1 (no jumper)\nGAIN 10
Wire Notes Line
	6350 5800 6350 6900
Wire Notes Line
	6350 6900 8950 6900
Wire Notes Line
	7850 4850 7850 6900
Wire Notes Line
	7850 5800 6350 5800
Text Notes 6400 7000 0    60   ~ 0
Passed up to 5V shunts \nto GND to keep under \n5.3V max.
Connection ~ 9400 6850
Wire Wire Line
	9100 5300 9100 5650
Text Notes 4500 5500 0    60   ~ 0
Front end divider.\nWhen connected to term 3\na 500V input is limited 5V \nat the next stage\n\n
Text Notes 8800 4850 0    60   ~ 0
Selectable Gain Amplifier SGA\ndigitally controlled gain of 50X, 10X or 1X
$Comp
L CONN_3 K1
U 1 1 5287F15B
P 3000 6050
F 0 "K1" V 2950 6050 50  0000 C CNN
F 1 "CONN_3" V 3050 6050 40  0000 C CNN
F 2 "bornier3" H 3000 6050 60  0001 C CNN
F 3 "" H 3000 6050 60  0000 C CNN
F 4 "Weidmuller,1760500000" V 3000 6050 60  0001 C CNN "mfg,mfg#"
F 5 "seeed,347008p1" V 3000 6050 60  0001 C CNN "vend1,vend1#"
	1    3000 6050
	-1   0    0    1   
$EndComp
Text Label 3350 5950 0    60   ~ 0
input_500Vmax
Text Label 3350 6050 0    60   ~ 0
input_250Vmax
Text Label 3350 6150 0    60   ~ 0
input_5Vmax
Wire Wire Line
	3350 5950 3950 5950
Wire Wire Line
	3350 6050 4950 6050
Wire Wire Line
	3350 6150 5950 6150
Connection ~ 5950 6150
Text Notes 9100 7050 0    60   ~ 0
Install W1 jumper \nshorting to +V to \ndisable op-amp.
Text Notes 750  7400 0    60   ~ 0
If 500V input is on pin 3 this will results in 5V on the signal node (100:1). If U1 is set to 1X gain, output = 5V.\nIf 5V on pin 3 results in .05V on the signal node. If U1 is set to 50X gain, output = 2.5V.\nThis can be handy if you are sniffing an automotive ignition signal, that can spike up to 400 to 500V. \n\nIf 250V input is on pin 2, this will result in 5V on the signal node (50:1). If U1 is set to 1X gain, output = 5V. \nIf 50V input is on pin 2, this will result in 1V on the signal node (50:1). If U1 is set to 1X gain, output = 1V. \nIf 25V input is on pin 2, this will result in 1/2V on the signal node (50:1). If U1 is set to 10X gain, output = 5V.\nIf 5V input is on pin 2, this will result in 1/10V on the signal node (50:1). If U1 is set to 50X gain, output = 5V.  \nThis is good for sniffing fuel injectors that might spike to 40 to 70V. \n\nIf 5V input is on pin 1, this will result in 5V on the signal node (1:1). If U1 is set to 1X gain, output = 5V. \nIf 1V input is on pin 1, this will result in 5V on the signal node (1:1). If U1 is set to 10X gain, output = 5V. \nIf .1V input is on pin 1, this will result in 5V on the signal node (1:1). If U1 is set to 50X gain, output = 5V. \nThis is good for signals you know will be 5V or less, including very small signals. \n\nSallenkey LPF 10kHz\nhttp://sim.okawa-denshi.jp/en/OPstool.php
$Comp
L C C2
U 1 1 5288D9F3
P 8900 5650
F 0 "C2" H 8710 5750 50  0000 L CNN
F 1 "1uF" H 8650 5550 50  0000 L CNN
F 2 "SM0805" V -5770 4425 60  0001 C CNN
F 3 "" H 8900 5650 60  0001 C CNN
F 4 "YAGEO,CC0805KKX7R8BB105" V -5770 4425 60  0001 C CNN "mfg,mfg#"
F 5 "Seeedstudio OPL,CC0805KKX7R8BB105" V -5770 4425 60  0001 C CNN "vend1,vend1#"
	1    8900 5650
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 52908526
P 5200 5950
F 0 "R2" V 5280 5950 50  0000 C CNN
F 1 "243k" V 5200 5950 50  0000 C CNN
F 2 "SM0805" V -8970 6725 60  0001 C CNN
F 3 "" H 5200 5950 60  0001 C CNN
F 4 "vishay,CRCW0805240KFKEA" V -8970 6725 60  0001 C CNN "mfg,mfg#"
F 5 "digi,541-240KCCT-ND" V -8970 6725 60  0001 C CNN "vend1,vend1#"
	1    5200 5950
	0    1    -1   0   
$EndComp
$Comp
L R R6
U 1 1 52908557
P 5700 5950
F 0 "R6" V 5780 5950 50  0000 C CNN
F 1 "243k" V 5700 5950 50  0000 C CNN
F 2 "SM0805" V -8470 6725 60  0001 C CNN
F 3 "" H 5700 5950 60  0001 C CNN
F 4 "vishay,CRCW0805240KFKEA" V -8470 6725 60  0001 C CNN "mfg,mfg#"
F 5 "digi,541-240KCCT-ND" V -8470 6725 60  0001 C CNN "vend1,vend1#"
	1    5700 5950
	0    1    -1   0   
$EndComp
$Comp
L R R5
U 1 1 5290855F
P 4200 5950
F 0 "R5" V 4280 5950 50  0000 C CNN
F 1 "243k" V 4200 5950 50  0000 C CNN
F 2 "SM0805" V -9970 6725 60  0001 C CNN
F 3 "" H 4200 5950 60  0001 C CNN
F 4 "vishay,CRCW0805240KFKEA" V -9970 6725 60  0001 C CNN "mfg,mfg#"
F 5 "digi,541-240KCCT-ND" V -9970 6725 60  0001 C CNN "vend1,vend1#"
	1    4200 5950
	0    1    -1   0   
$EndComp
Wire Wire Line
	4950 6050 4950 5950
$Comp
L R R8
U 1 1 52909041
P 8550 5950
F 0 "R8" V 8630 5950 50  0000 C CNN
F 1 "4.7k" V 8550 5950 50  0000 C CNN
F 2 "SM0603" V -5620 6725 60  0001 C CNN
F 3 "" H 8550 5950 60  0001 C CNN
F 4 "Yageo,RC0603JR-074K7L" V -5620 6725 60  0001 C CNN "mfg,mfg#"
F 5 "seeed,0010030p1" V -5620 6725 60  0001 C CNN "vend1,vend1#"
	1    8550 5950
	0    1    -1   0   
$EndComp
$Comp
L R R7
U 1 1 529090DB
P 8050 5950
F 0 "R7" V 8130 5950 50  0000 C CNN
F 1 "4.7k" V 8050 5950 50  0000 C CNN
F 2 "SM0603" V -6120 6725 60  0001 C CNN
F 3 "" H 8050 5950 60  0001 C CNN
F 4 "Yageo,RC0603JR-074K7L" V -6120 6725 60  0001 C CNN "mfg,mfg#"
F 5 "seeed,0010030p1" V -6120 6725 60  0001 C CNN "vend1,vend1#"
	1    8050 5950
	0    1    -1   0   
$EndComp
Wire Wire Line
	8700 5300 8700 5700
Connection ~ 8700 5650
$Comp
L C C4
U 1 1 52909177
P 8800 6150
F 0 "C4" H 8610 6250 50  0000 L CNN
F 1 "4.7nF" H 8550 6050 50  0000 L CNN
F 2 "SM0603" V -5870 4925 60  0001 C CNN
F 3 "" H 8800 6150 60  0001 C CNN
F 4 "Yageo,CC0603KRX7R9BB472" V -5870 4925 60  0001 C CNN "mfg,mfg#"
F 5 "Seeedstudio OPL,0100850p1" V -5870 4925 60  0001 C CNN "vend1,vend1#"
	1    8800 6150
	1    0    0    1   
$EndComp
$Comp
L C C3
U 1 1 529091EA
P 8500 5000
F 0 "C3" H 8310 5100 50  0000 L CNN
F 1 "4.7nF" H 8250 4900 50  0000 L CNN
F 2 "SM0603" V -6170 3775 60  0001 C CNN
F 3 "" H 8500 5000 60  0001 C CNN
F 4 "Yageo,CC0603KRX7R9BB472" V -6170 3775 60  0001 C CNN "mfg,mfg#"
F 5 "Seeedstudio OPL,0100850p1" V -6170 3775 60  0001 C CNN "vend1,vend1#"
	1    8500 5000
	0    1    -1   0   
$EndComp
Wire Wire Line
	8700 5000 9600 5000
Wire Wire Line
	9600 5000 9600 5950
Wire Wire Line
	8300 5000 8300 5950
Wire Wire Line
	8800 6350 8800 6850
Connection ~ 8800 6850
Wire Notes Line
	8950 6900 8950 5800
Wire Notes Line
	8950 5800 8550 5800
Wire Notes Line
	8550 5800 8550 5250
Wire Notes Line
	8550 5250 8700 5250
Wire Notes Line
	8700 5250 8700 4850
Wire Notes Line
	8700 4850 7850 4850
Text Notes 7900 7000 0    60   ~ 0
Sallenkey 10kHz
$Comp
L FUSE F1
U 1 1 52932969
P 6200 5950
F 0 "F1" H 6300 6000 40  0000 C CNN
F 1 ".5A" H 6100 5900 40  0000 C CNN
F 2 "SM1206" H 6200 5950 60  0001 C CNN
F 3 "~" H 6200 5950 60  0000 C CNN
F 4 "-,-" H 6200 5950 60  0001 C CNN "mfg,mfg#"
F 5 "Seeed,090030p1" H 6200 5950 60  0001 C CNN "Vend1,Vend1#"
	1    6200 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 5950 7800 5950
$EndSCHEMATC
