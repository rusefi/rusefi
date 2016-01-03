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
LIBS:PWR_5V_linear-cache
EELAYER 27 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "Adjustable Linear Regulator"
Date "2 jan 2016"
Rev ".02"
Comp "rusEFI.com"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_1 P1002
U 1 1 5611BCCA
P 8750 6250
F 0 "P1002" H 8830 6250 40  0000 L CNN
F 1 "CONN_1" H 8750 6305 30  0001 C CNN
F 2 "SIL-1" V 2130 2150 60  0001 C CNN
F 3 "" H 8750 6250 60  0001 C CNN
F 4 "DO NOT POPULATE,DNP" V 2130 2150 60  0001 C CNN "mfg,mfg#"
F 5 "DO NOT POPULATE,DNP" V 2130 2150 60  0001 C CNN "vend1,vend1#"
	1    8750 6250
	0    1    -1   0   
$EndComp
$Comp
L CP1 C1001
U 1 1 5611BCCB
P 7200 6600
F 0 "C1001" V 7050 6350 50  0000 L CNN
F 1 "0.1uF 50V" V 7400 6350 50  0000 L CNN
F 2 "C1" H 7200 6600 60  0001 C CNN
F 3 "" H 7200 6600 60  0001 C CNN
F 4 "blah,blah" H 7200 6600 60  0001 C CNN "MFG,MFG#"
F 5 "blah,blah" H 7200 6600 60  0001 C CNN "VEND1,VEND1#"
F 6 "sky-macau,6175330450" V 7200 6600 60  0001 C CNN "vend2,vend2#"
	1    7200 6600
	-1   0    0    -1  
$EndComp
$Comp
L R R1002
U 1 1 5611BCCC
P 9050 6600
F 0 "R1002" V 9130 6600 50  0000 C CNN
F 1 "1k" V 9050 6600 50  0000 C CNN
F 2 "SM0805" V 1530 2600 60  0001 C CNN
F 3 "" H 9050 6600 60  0001 C CNN
F 4 "Panasonic,ERA-6AEB102V" V 1530 2600 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,P1.0KDACT-ND" V 1530 2600 60  0001 C CNN "vend1,vend1#"
F 6 "sky-macau,7768409796" V 9050 6600 60  0001 C CNN "vend2,vend2#"
	1    9050 6600
	0    1    -1   0   
$EndComp
$Comp
L LED D1005
U 1 1 5611BCCD
P 9000 6750
F 0 "D1005" H 9000 6600 50  0000 C CNN
F 1 "LED-grn" H 9000 6925 50  0001 C CNN
F 2 "LED-0805" H 9000 6750 60  0001 C CNN
F 3 "" H 9000 6750 60  0001 C CNN
F 4 "lite-on,LTST-C171GKT" H 9000 6750 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,160-1423-1-ND" H 9000 6750 60  0001 C CNN "VEND1,VEND1#"
F 6 "sky-macau,1562597916" H 9000 6750 60  0001 C CNN "vend2,vend2#"
	1    9000 6750
	1    0    0    -1  
$EndComp
Text Label 7200 6400 0    60   ~ 0
VBAT
$Comp
L GND #PWR01
U 1 1 5611BCCE
P 7850 6900
F 0 "#PWR01" H 7850 6900 30  0001 C CNN
F 1 "GND" H 7850 6830 30  0001 C CNN
F 2 "" H 7850 6900 60  0000 C CNN
F 3 "" H 7850 6900 60  0000 C CNN
	1    7850 6900
	1    0    0    -1  
$EndComp
$Comp
L MOS_PRO Q1001
U 1 1 5611BCD0
P 5350 6500
F 0 "Q1001" H 5350 6690 60  0000 R CNN
F 1 "20V" V 5550 6550 60  0000 R CNN
F 2 "SOT23" H 5350 6500 60  0001 C CNN
F 3 "~" H 5350 6500 60  0000 C CNN
F 4 "vishay,SI2323DS-T1-E3" H 5350 6500 60  0001 C CNN "mfg,mfg"
F 5 "DIGI,SI2323DS-T1-E3CT-ND" H 5350 6500 60  0001 C CNN "vend1,vend1"
F 6 "sky-macau.com,4824167646" H 5350 6500 60  0001 C CNN "vend2,vend2#"
	1    5350 6500
	0    -1   -1   0   
$EndComp
$Comp
L R R1001
U 1 1 5611BCD1
P 5900 6700
F 0 "R1001" V 5980 6700 50  0000 C CNN
F 1 "1k" V 5900 6700 50  0000 C CNN
F 2 "SM0805" V 1530 2600 60  0001 C CNN
F 3 "" H 5900 6700 60  0001 C CNN
F 4 "Panasonic,ERA-6AEB102V" V 1530 2600 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,P1.0KDACT-ND" V 1530 2600 60  0001 C CNN "vend1,vend1#"
F 6 "sky-macau,7768409796" V 5900 6700 60  0001 C CNN "vend2,vend2#"
	1    5900 6700
	0    -1   -1   0   
$EndComp
$Comp
L ZENERSMALL D1001
U 1 1 5611BCD2
P 5650 6600
F 0 "D1001" H 5650 6700 40  0000 C CNN
F 1 "7.5V" H 5650 6500 30  0000 C CNN
F 2 "SM1206" H 5650 6600 60  0001 C CNN
F 3 "~" H 5650 6600 60  0000 C CNN
F 4 "diodesinc,BZT52C7V5-7-F" H 5650 6600 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,BZT52C7V5-FDICT-ND" H 5650 6600 60  0001 C CNN "vend1,vend1#"
F 6 "sky-macau.com,5633100308" H 5650 6600 60  0001 C CNN "vend2,vend2#"
	1    5650 6600
	0    -1   -1   0   
$EndComp
$Comp
L CP1 C1002
U 1 1 5611BCD4
P 9450 6650
F 0 "C1002" V 9300 6550 50  0000 L CNN
F 1 "1uF 16V" H 9000 6500 50  0000 L CNN
F 2 "SM0805" H 9450 6650 60  0001 C CNN
F 3 "" H 9450 6650 60  0001 C CNN
F 4 "blah,blah" H 9450 6650 60  0001 C CNN "MFG,MFG#"
F 5 "blah,blah" H 9450 6650 60  0001 C CNN "VEND1,VEND1#"
F 6 "sky-macau.com,5761275017" H 9450 6650 60  0001 C CNN "vend2,vend2#"
	1    9450 6650
	-1   0    0    -1  
$EndComp
$Comp
L CONN_1 P902
U 1 1 5611BCD7
P 5650 6250
F 0 "P902" H 5730 6250 40  0000 L CNN
F 1 "CONN_1" H 5650 6305 30  0001 C CNN
F 2 "SIL-1" H 5650 6250 60  0001 C CNN
F 3 "" H 5650 6250 60  0000 C CNN
F 4 "dnp,dnp" H 5650 6250 60  0001 C CNN "Field4"
F 5 "dnp,dnp" H 5650 6250 60  0001 C CNN "Field5"
	1    5650 6250
	0    -1   -1   0   
$EndComp
$Comp
L LM317MDT U1001
U 1 1 5611BCD8
P 7850 6450
F 0 "U1001" H 7650 6650 40  0000 C CNN
F 1 "LM317M" H 7850 6650 40  0000 L CNN
F 2 "DPAK2" H 7850 6550 30  0001 C CIN
F 3 "~" H 7850 6450 60  0000 C CNN
F 4 "blah,LM317M" H 7850 6450 60  0001 C CNN "mfg,mfg#"
F 5 "blah,blah" H 7850 6450 60  0001 C CNN "vend1,vend1#"
F 6 "sky-macau,9372473499" H 7850 6450 60  0001 C CNN "vend2,vend2#"
	1    7850 6450
	1    0    0    -1  
$EndComp
Text Label 10050 6450 1    60   ~ 0
5V-REG
$Comp
L PWR_FLAG #FLG02
U 1 1 5611BCD9
P 9450 6450
F 0 "#FLG02" H 9450 6545 30  0001 C CNN
F 1 "PWR_FLAG" H 9450 6630 30  0000 C CNN
F 2 "" H 9450 6450 60  0000 C CNN
F 3 "" H 9450 6450 60  0000 C CNN
	1    9450 6450
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG03
U 1 1 5611BCDA
P 7200 6850
F 0 "#FLG03" H 7200 6945 30  0001 C CNN
F 1 "PWR_FLAG" H 7200 7030 30  0000 C CNN
F 2 "" H 7200 6850 60  0000 C CNN
F 3 "" H 7200 6850 60  0000 C CNN
	1    7200 6850
	-1   0    0    1   
$EndComp
$Comp
L POT RV1001
U 1 1 5611BCDB
P 8500 6700
F 0 "RV1001" H 8500 6600 50  0000 C CNN
F 1 "1k" H 8500 6700 50  0000 C CNN
F 2 "TO220_VERT" H 8500 6700 60  0001 C CNN
F 3 "~" H 8500 6700 60  0000 C CNN
F 4 " ," H 8500 6700 60  0001 C CNN "mfg,mfg#"
F 5 " ," H 8500 6700 60  0001 C CNN "vend1,vend2#"
F 6 "sky-macau,2633110656" H 8500 6700 60  0001 C CNN "vend2,vend2#"
	1    8500 6700
	-1   0    0    1   
$EndComp
$Comp
L R R1006
U 1 1 5611BCDC
P 8500 6500
F 0 "R1006" V 8450 6775 50  0000 C CNN
F 1 "240R" V 8500 6500 50  0000 C CNN
F 2 "SM0805" V 980 2500 60  0001 C CNN
F 3 "" H 8500 6500 60  0001 C CNN
F 4 "," V 980 2500 60  0001 C CNN "mfg,mfg#"
F 5 "," V 980 2500 60  0001 C CNN "vend1,vend1#"
F 6 "sky-macau,5622933558" V 8500 6500 60  0001 C CNN "vend2,vend2#"
	1    8500 6500
	0    -1   1    0   
$EndComp
$Comp
L CONN_1 P1004
U 1 1 5611BCDE
P 9200 7000
F 0 "P1004" H 9280 7000 40  0000 L CNN
F 1 "CONN_1" H 9200 7055 30  0001 C CNN
F 2 "SIL-1" V 2580 2900 60  0001 C CNN
F 3 "" H 9200 7000 60  0001 C CNN
F 4 "DO NOT POPULATE,DNP" V 2580 2900 60  0001 C CNN "mfg,mfg#"
F 5 "DO NOT POPULATE,DNP" V 2580 2900 60  0001 C CNN "vend1,vend1#"
	1    9200 7000
	0    -1   1    0   
$EndComp
$Comp
L FUSE F1001
U 1 1 5611BCEB
P 3850 6400
F 0 "F1001" H 3950 6450 40  0000 C CNN
F 1 "0.5A" H 3750 6350 40  0000 C CNN
F 2 "SIL-2" H 3850 6400 60  0001 C CNN
F 3 "~" H 3850 6400 60  0000 C CNN
F 4 "0R,jumperwire" H 3850 6400 60  0001 C CNN "mfg,mfg#"
F 5 "0R,jumperwire" H 3850 6400 60  0001 C CNN "vend1,vend1#"
	1    3850 6400
	1    0    0    -1  
$EndComp
Text Label 4100 6400 0    60   ~ 0
Vf
Text HLabel 3500 6400 0    60   Input ~ 0
Vin
Text HLabel 3500 6600 0    60   Input ~ 0
GND
$Comp
L CONN_1 P901
U 1 1 5611BCEF
P 3450 6250
F 0 "P901" H 3530 6250 40  0000 L CNN
F 1 "CONN_1" H 3450 6305 30  0001 C CNN
F 2 "SIL-1" H 3450 6250 60  0001 C CNN
F 3 "" H 3450 6250 60  0000 C CNN
F 4 "dnp,dnp" H 3450 6250 60  0001 C CNN "Field4"
F 5 "dnp,dnp" H 3450 6250 60  0001 C CNN "Field5"
	1    3450 6250
	-1   0    0    1   
$EndComp
$Comp
L MOS_NRO Q1002
U 1 1 5611BCF1
P 4800 6500
F 0 "Q1002" H 4800 6350 60  0000 R CNN
F 1 "200V" V 5000 6600 60  0000 R CNN
F 2 "m-pad-2.1-TO-263AB" H 4800 6500 60  0001 C CNN
F 3 "~" H 4800 6500 60  0000 C CNN
F 4 "IXYS,IXTA6N50D2" H 4800 6500 60  0001 C CNN "mfg,mfg#"
F 5 "digikey,IXTA6N50D2-ND" H 4800 6500 60  0001 C CNN "vend1,vend1#"
	1    4800 6500
	0    -1   -1   0   
$EndComp
$Comp
L ZENERSMALL D1002
U 1 1 5611BCF2
P 4600 6750
F 0 "D1002" H 4600 6850 40  0000 C CNN
F 1 "15V" H 4600 6650 30  0000 C CNN
F 2 "D3" H 4600 6750 60  0001 C CNN
F 3 "~" H 4600 6750 60  0000 C CNN
F 4 "FAIRCHILD,1N5245B" H 4600 6750 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,1N5245BFS-ND" H 4600 6750 60  0001 C CNN "vend1,vend1#"
F 6 "sky-macau.com,9197428750" H 4600 6750 60  0001 C CNN "vend2,vend2#"
	1    4600 6750
	0    -1   -1   0   
$EndComp
$Comp
L R R1003
U 1 1 5611BCF3
P 4350 6600
F 0 "R1003" V 4430 6600 50  0000 C CNN
F 1 "100k" V 4350 6600 50  0000 C CNN
F 2 "SM0805" V -20 2500 60  0001 C CNN
F 3 "" H 4350 6600 60  0001 C CNN
F 4 "ECG,ERA6AEB104V" V -20 2500 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,P100KDACT-ND" V -20 2500 60  0001 C CNN "vend1,vend1#"
	1    4350 6600
	0    -1   -1   0   
$EndComp
Text Label 5000 6400 0    60   ~ 0
Vs2
Text Label 4700 6600 2    60   ~ 0
Vs1
Text Notes 550  5350 0    60   ~ 0
Brief overview\n-- Q1002, R1003, D1002 preform an active transient protection. It will suppress voltages up to 150V to 200V. This limits to 15V(diode) + 4.5V(MOSFET gate) 19.5V down stream.\n-- Q1001, R1001, D1001 preform a reverse polarity protection. If the input signal is the wrong polarity, the gate will not conduct which will prevent current from flowing.  Above 19.5V is below 20Vmax of Q1001.\n-- D1003 is a second transient suppressor, it would catch faster transients allowing a brief amount of time for Q1002 to preform it's duty. \n-- C1001 is a bulk cap, it simply stores energy locally such that the regulator can draw large currents in short periods of time. \n-- U1001 and the components to the right, are a buck style linear regulator, that will pull the 5V line up to 5V. It will now pull it down from 5V if there is an external voltage. 
$Comp
L ZENERSMALL D1003
U 1 1 5687F3A8
P 6750 6750
F 0 "D1003" H 6750 6850 40  0000 C CNN
F 1 "24V" H 6750 6650 30  0000 C CNN
F 2 "D3" H 6750 6750 60  0001 C CNN
F 3 "~" H 6750 6750 60  0000 C CNN
F 4 "fairchild,BZX79C20" H 6750 6750 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,BZX79C20-ND" H 6750 6750 60  0001 C CNN "vend1,vend1#"
	1    6750 6750
	0    -1   -1   0   
$EndComp
Text Notes 5850 7200 0    60   ~ 0
For 5V, install 715 ohm between pins 3 and 2 (or 1) on RV1001\n1.25v * (1 + (715 / 240)) + 50uA * 715 = 5.01
Connection ~ -4675 3450
Connection ~ 9450 6850
Connection ~ 7850 6850
Wire Wire Line
	7200 6850 7200 6800
Connection ~ 7200 6850
Wire Wire Line
	9200 6850 9200 6750
Connection ~ 9200 6850
Wire Wire Line
	8800 6750 8800 6600
Wire Wire Line
	5550 6400 7450 6400
Connection ~ 7200 6400
Wire Wire Line
	10400 6850 10400 6650
Connection ~ 10050 6450
Connection ~ 6750 6400
Wire Wire Line
	3500 6850 10400 6850
Connection ~ 6750 6850
Wire Wire Line
	6150 6850 6150 6700
Connection ~ 6150 6850
Wire Wire Line
	5650 6700 5350 6700
Wire Wire Line
	5650 6500 5650 6400
Connection ~ 5650 6400
Wire Wire Line
	8250 6400 8750 6400
Wire Wire Line
	9300 6600 9300 6450
Connection ~ 9300 6450
Wire Wire Line
	8750 6400 8750 6500
Wire Wire Line
	8750 6450 10400 6450
Connection ~ 8750 6450
Connection ~ 8750 6400
Wire Wire Line
	7850 6700 8250 6700
Wire Wire Line
	8250 6700 8250 6500
Wire Wire Line
	8750 6700 8750 6850
Connection ~ 8750 6850
Connection ~ 8500 6850
Connection ~ 8250 6700
Wire Wire Line
	7850 6850 7850 6900
Connection ~ 9450 6450
Wire Wire Line
	3500 6850 3500 6600
Wire Wire Line
	3500 6400 3600 6400
Wire Wire Line
	3600 6400 3600 6250
Wire Wire Line
	4100 6400 4600 6400
Wire Wire Line
	5000 6400 5150 6400
Wire Wire Line
	4100 6600 4100 6400
Connection ~ 4600 6850
Wire Wire Line
	4600 6600 4600 6650
Wire Wire Line
	4700 6700 4800 6700
Wire Wire Line
	4700 6600 4700 6700
Wire Wire Line
	4700 6600 4600 6600
Connection ~ 4600 6600
Wire Wire Line
	6750 6650 6750 6400
Wire Notes Line
	8100 7100 8300 6900
Wire Notes Line
	8300 6900 8250 6900
Wire Notes Line
	8250 6900 8300 6950
Wire Notes Line
	8300 6950 8300 6900
$Comp
L ZENERSMALL D1004
U 1 1 5687F710
P 7800 6050
F 0 "D1004" H 7800 6150 40  0000 C CNN
F 1 "24V" H 7800 5950 30  0000 C CNN
F 2 "D3" H 7800 6050 60  0001 C CNN
F 3 "~" H 7800 6050 60  0000 C CNN
F 4 "fairchild,BZX79C20" H 7800 6050 60  0001 C CNN "mfg,mfg#"
F 5 "DIGI,BZX79C20-ND" H 7800 6050 60  0001 C CNN "vend1,vend1#"
	1    7800 6050
	-1   0    0    1   
$EndComp
Wire Wire Line
	7450 6400 7450 6050
Wire Wire Line
	7450 6050 7700 6050
Wire Wire Line
	7900 6050 8250 6050
Wire Wire Line
	8250 6050 8250 6400
$EndSCHEMATC
