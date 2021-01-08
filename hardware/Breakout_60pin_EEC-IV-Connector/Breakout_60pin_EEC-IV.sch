EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "Breakout_60pin_EEC-IV"
Date "2020-06-01"
Rev "R0.1"
Comp "rusEFI.com"
Comment1 "DAECU"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Breakout_60pin_EEC-IV-rescue:Logo_Open_Hardware_Small-Graphic-Breakout_60pin_EEC-IV-rescue LOGO1
U 1 1 5D21BC70
P 5975 7725
F 0 "LOGO1" H 5975 8000 50  0001 C CNN
F 1 "DNP" H 5975 7500 50  0001 C CNN
F 2 "Symbol:OSHW-Logo2_7.3x6mm_SilkScreen" H 5975 7725 50  0001 C CNN
F 3 "~" H 5975 7725 50  0001 C CNN
F 4 "DNP" H -5915 -2520 50  0001 C CNN "Part #"
F 5 "DNP" H -5915 -2520 50  0001 C CNN "VEND"
F 6 "DNP" H -5915 -2520 50  0001 C CNN "VEND#"
F 7 "DNP" H -5915 -2520 50  0001 C CNN "Manufacturer"
	1    5975 7725
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J10
U 1 1 5E3757F6
P 8175 3200
F 0 "J10" H 8175 3550 50  0000 C CNN
F 1 "Conn_01x10" H 8093 5226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 8175 3200 50  0001 C CNN
F 3 "~" H 8175 3200 50  0001 C CNN
	1    8175 3200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J4
U 1 1 5E3771EB
P 7875 3200
F 0 "J4" H 7975 3550 50  0000 C CNN
F 1 "Conn_01x10" H 7793 5226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 7875 3200 50  0001 C CNN
F 3 "~" H 7875 3200 50  0001 C CNN
	1    7875 3200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J11
U 1 1 5E37933C
P 8175 4200
F 0 "J11" H 8175 4550 50  0000 C CNN
F 1 "Conn_01x10" H 8093 6226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 8175 4200 50  0001 C CNN
F 3 "~" H 8175 4200 50  0001 C CNN
	1    8175 4200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J5
U 1 1 5E37A504
P 7875 4200
F 0 "J5" H 8000 4550 50  0000 C CNN
F 1 "Conn_01x10" H 7793 6226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 7875 4200 50  0001 C CNN
F 3 "~" H 7875 4200 50  0001 C CNN
	1    7875 4200
	-1   0    0    1   
$EndComp
$Comp
L Breakout_60pin_EEC-IV-rescue:GND-power-Breakout_60pin_EEC-IV-rescue #PWR01
U 1 1 5DD0B38F
P 1100 7725
F 0 "#PWR01" H 1100 7475 50  0001 C CNN
F 1 "GND" H 1105 7552 50  0000 C CNN
F 2 "" H 1100 7725 50  0001 C CNN
F 3 "" H 1100 7725 50  0001 C CNN
	1    1100 7725
	1    0    0    -1  
$EndComp
$Comp
L Breakout_60pin_EEC-IV-rescue:Conn_01x01-Connector_Generic-Breakout_60pin_EEC-IV-rescue J1
U 1 1 5DD1444D
P 1100 7525
F 0 "J1" V 1018 7437 50  0000 R CNN
F 1 "Conn_01x01" V 973 7437 50  0001 R CNN
F 2 "TestPoint:TestPoint_Keystone_5000-5004_Miniature" H 1100 7525 50  0001 C CNN
F 3 "~" H 1100 7525 50  0001 C CNN
	1    1100 7525
	0    -1   -1   0   
$EndComp
Text Label 1100 7725 0    50   ~ 0
GND
$Comp
L Breakout_60pin_EEC-IV-rescue:PWR_FLAG-power-Breakout_60pin_EEC-IV-rescue #FLG01
U 1 1 5DD43FA1
P 1100 7725
F 0 "#FLG01" H 1100 7800 50  0001 C CNN
F 1 "PWR_FLAG" V 1100 7852 50  0000 L CNN
F 2 "" H 1100 7725 50  0001 C CNN
F 3 "~" H 1100 7725 50  0001 C CNN
	1    1100 7725
	0    -1   -1   0   
$EndComp
Text Label 8375 6600 0    50   ~ 0
60
Text Label 8375 6100 0    50   ~ 0
55
Text Label 8375 6000 0    50   ~ 0
54
Text Label 8375 5900 0    50   ~ 0
53
Text Label 8375 5600 0    50   ~ 0
50
Text Label 8375 5100 0    50   ~ 0
45
Text Label 8375 4700 0    50   ~ 0
41
Text Label 8375 2700 0    50   ~ 0
421
Text Label 8375 3100 0    50   ~ 0
25
Text Label 8375 3600 0    50   ~ 0
30
Text Label 8375 3700 0    50   ~ 0
31
Text Label 8375 4100 0    50   ~ 0
35
Text Label 8375 4600 0    50   ~ 0
40
$Comp
L eec-iv:EEC-IV J14
U 1 1 5EDC31E9
P 8875 1600
F 0 "J14" H 9055 -408 50  0000 L CNN
F 1 "EEC-IV" H 9055 -499 50  0000 L CNN
F 2 "rusEFI:eeciv" H 8875 1600 50  0001 C CNN
F 3 "" H 8875 1600 50  0001 C CNN
	1    8875 1600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J12
U 1 1 5EE52D7D
P 8175 5200
F 0 "J12" H 8175 5550 50  0000 C CNN
F 1 "Conn_01x10" H 8093 7226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 8175 5200 50  0001 C CNN
F 3 "~" H 8175 5200 50  0001 C CNN
	1    8175 5200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J6
U 1 1 5EE53A3D
P 7875 5200
F 0 "J6" H 8000 5550 50  0000 C CNN
F 1 "Conn_01x10" H 7793 7226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 7875 5200 50  0001 C CNN
F 3 "~" H 7875 5200 50  0001 C CNN
	1    7875 5200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J13
U 1 1 5EE5401D
P 8175 6200
F 0 "J13" H 8175 6550 50  0000 C CNN
F 1 "Conn_01x10" H 8093 8226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 8175 6200 50  0001 C CNN
F 3 "~" H 8175 6200 50  0001 C CNN
	1    8175 6200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J7
U 1 1 5EE55E51
P 7875 6200
F 0 "J7" H 8000 6550 50  0000 C CNN
F 1 "Conn_01x10" H 7793 8226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 7875 6200 50  0001 C CNN
F 3 "~" H 7875 6200 50  0001 C CNN
	1    7875 6200
	-1   0    0    1   
$EndComp
Text Label 8375 2600 0    50   ~ 0
420
Text Label 8375 2100 0    50   ~ 0
15
Text Label 8375 1700 0    50   ~ 0
11
Text Label 8375 1600 0    50   ~ 0
10
Text Label 8375 1100 0    50   ~ 0
5
Text Label 8375 700  0    50   ~ 0
1
$Comp
L logo:LOGO G2
U 1 1 5F1903D8
P 5525 7725
F 0 "G2" H 5525 7622 60  0001 C CNN
F 1 "LOGO" H 5525 7828 60  0001 C CNN
F 2 "rusEFI:LOGO" H 5525 7725 50  0001 C CNN
F 3 "" H 5525 7725 50  0001 C CNN
	1    5525 7725
	1    0    0    -1  
$EndComp
$Comp
L Off_Road_Disclaimer:Disclaimer G1
U 1 1 5F1910EA
P 9775 6800
F 0 "G1" H 9775 7150 60  0001 C CNN
F 1 "Disclaimer" H 9775 7050 60  0001 C CNN
F 2 "rusEFI:Off_Road_Disclaimer" H 9775 6800 50  0001 C CNN
F 3 "" H 9775 6800 50  0001 C CNN
	1    9775 6800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J8
U 1 1 5E370115
P 8175 1200
F 0 "J8" H 8175 1550 50  0000 C CNN
F 1 "Conn_01x10" H 8093 3226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 8175 1200 50  0001 C CNN
F 3 "~" H 8175 1200 50  0001 C CNN
	1    8175 1200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J2
U 1 1 5DD70BCF
P 7875 1200
F 0 "J2" H 8000 1550 50  0000 C CNN
F 1 "Conn_01x10" H 7793 3226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 7875 1200 50  0001 C CNN
F 3 "~" H 7875 1200 50  0001 C CNN
	1    7875 1200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J9
U 1 1 5E374575
P 8175 2200
F 0 "J9" H 8175 2550 50  0000 C CNN
F 1 "Conn_01x10" H 8093 4226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 8175 2200 50  0001 C CNN
F 3 "~" H 8175 2200 50  0001 C CNN
	1    8175 2200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J3
U 1 1 5E376794
P 7875 2200
F 0 "J3" H 8000 2550 50  0000 C CNN
F 1 "Conn_01x10" H 7793 4226 50  0001 C CNN
F 2 "Connector_Phoenix_MC:PhoenixContact_MCV_1,5_10-G-3.81_1x10_P3.81mm_Vertical" H 7875 2200 50  0001 C CNN
F 3 "~" H 7875 2200 50  0001 C CNN
	1    7875 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	8075 6500 8675 6500
Wire Wire Line
	8075 6400 8675 6400
Wire Wire Line
	8075 6300 8675 6300
Wire Wire Line
	8075 6200 8675 6200
Wire Wire Line
	8075 6100 8675 6100
Wire Wire Line
	8075 6000 8675 6000
Wire Wire Line
	8075 5900 8675 5900
Wire Wire Line
	8075 5800 8675 5800
Wire Wire Line
	8075 5700 8675 5700
Wire Wire Line
	8075 5500 8675 5500
Wire Wire Line
	8075 5400 8675 5400
Wire Wire Line
	8075 5300 8675 5300
Wire Wire Line
	8075 5200 8675 5200
Wire Wire Line
	8075 5100 8675 5100
Wire Wire Line
	8075 5000 8675 5000
Wire Wire Line
	8075 4900 8675 4900
Wire Wire Line
	8075 4800 8675 4800
Wire Wire Line
	8075 4700 8675 4700
Wire Wire Line
	8075 4500 8675 4500
Wire Wire Line
	8075 4300 8675 4300
Wire Wire Line
	8075 4400 8675 4400
Wire Wire Line
	8075 4100 8675 4100
Wire Wire Line
	8075 4200 8675 4200
Wire Wire Line
	8075 3900 8675 3900
Wire Wire Line
	8075 4000 8675 4000
Wire Wire Line
	8075 3800 8675 3800
Wire Wire Line
	8075 3700 8675 3700
Wire Wire Line
	8075 3400 8675 3400
Wire Wire Line
	8075 3500 8675 3500
Wire Wire Line
	8075 3200 8675 3200
Wire Wire Line
	8075 3300 8675 3300
Wire Wire Line
	8075 3000 8675 3000
Wire Wire Line
	8075 3100 8675 3100
Wire Wire Line
	8075 2900 8675 2900
Wire Wire Line
	8075 2800 8675 2800
Wire Wire Line
	8075 2700 8675 2700
Wire Wire Line
	8075 700  8675 700 
Wire Wire Line
	8075 900  8675 900 
Wire Wire Line
	8075 800  8675 800 
Wire Wire Line
	8075 1100 8675 1100
Wire Wire Line
	8075 1000 8675 1000
Wire Wire Line
	8075 1200 8675 1200
Wire Wire Line
	8075 1400 8675 1400
Wire Wire Line
	8075 1300 8675 1300
Wire Wire Line
	8075 1500 8675 1500
Wire Wire Line
	8075 1700 8675 1700
Wire Wire Line
	8075 1800 8675 1800
Wire Wire Line
	8075 1900 8675 1900
Wire Wire Line
	8075 2000 8675 2000
Wire Wire Line
	8075 2100 8675 2100
Wire Wire Line
	8075 2200 8675 2200
Wire Wire Line
	8075 2300 8675 2300
Wire Wire Line
	8075 2400 8675 2400
Wire Wire Line
	8075 2500 8675 2500
Wire Wire Line
	8075 1600 8675 1600
Wire Wire Line
	8075 2600 8675 2600
Wire Wire Line
	8075 3600 8675 3600
Wire Wire Line
	8075 4600 8675 4600
Wire Wire Line
	8075 5600 8675 5600
Wire Wire Line
	8075 6600 8675 6600
$EndSCHEMATC
