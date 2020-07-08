EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 8
Title "FT81x TFT040 Driver Board"
Date "2020-07-06"
Rev "2007D"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L driver-board-rescue:FT813-driver-board-driver-board-rescue U6
U 1 1 5F0268C1
P 5900 2250
AR Path="/5F0268C1" Ref="U6"  Part="1" 
AR Path="/5F022188/5F0268C1" Ref="U6"  Part="1" 
F 0 "U6" H 5850 2400 50  0000 L CNN
F 1 "FT813" H 5800 2250 50  0000 L CNN
F 2 "Package_DFN_QFN:QFN-56-1EP_8x8mm_P0.5mm_EP5.6x5.6mm_HandSolder" H 5900 2250 50  0001 C CNN
F 3 "" H 5900 2250 50  0001 C CNN
	1    5900 2250
	1    0    0    -1  
$EndComp
Text GLabel 4750 1800 0    50   Output ~ 0
AUDIO
Text GLabel 4750 2000 0    50   Input ~ 0
SCK_3V
Text GLabel 4750 2100 0    50   Output ~ 0
SDI_BUFFER
Text GLabel 4750 2200 0    50   Input ~ 0
SDO_3V
Text GLabel 4750 2300 0    50   Input ~ 0
CS1_3V
$Comp
L power:GND #PWR034
U 1 1 5F02B6C5
P 4750 1900
F 0 "#PWR034" H 4750 1650 50  0001 C CNN
F 1 "GND" V 4750 1800 50  0000 R CNN
F 2 "" H 4750 1900 50  0001 C CNN
F 3 "" H 4750 1900 50  0001 C CNN
	1    4750 1900
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR038
U 1 1 5F02BC5F
P 5700 3100
F 0 "#PWR038" H 5700 2950 50  0001 C CNN
F 1 "+3.3V" V 5700 3300 50  0000 C CNN
F 2 "" H 5700 3100 50  0001 C CNN
F 3 "" H 5700 3100 50  0001 C CNN
	1    5700 3100
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR039
U 1 1 5F02BEAA
P 5900 3100
F 0 "#PWR039" H 5900 2950 50  0001 C CNN
F 1 "+3.3V" V 5900 3300 50  0000 C CNN
F 2 "" H 5900 3100 50  0001 C CNN
F 3 "" H 5900 3100 50  0001 C CNN
	1    5900 3100
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR041
U 1 1 5F02C6DE
P 6500 3100
F 0 "#PWR041" H 6500 2850 50  0001 C CNN
F 1 "GND" V 6500 2900 50  0000 C CNN
F 2 "" H 6500 3100 50  0001 C CNN
F 3 "" H 6500 3100 50  0001 C CNN
	1    6500 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR033
U 1 1 5F02CB8C
P 7100 1600
F 0 "#PWR033" H 7100 1350 50  0001 C CNN
F 1 "GND" V 7100 1500 50  0000 R CNN
F 2 "" H 7100 1600 50  0001 C CNN
F 3 "" H 7100 1600 50  0001 C CNN
	1    7100 1600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4750 2900 4150 2900
$Comp
L Device:R R3
U 1 1 5F02D8F9
P 3900 2900
F 0 "R3" V 3800 2900 50  0000 C CNN
F 1 "47k" V 3900 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3830 2900 50  0001 C CNN
F 3 "~" H 3900 2900 50  0001 C CNN
F 4 "C23178" H 3900 2900 50  0001 C CNN "JLCPCB"
	1    3900 2900
	0    1    1    0   
$EndComp
$Comp
L Device:C C10
U 1 1 5F02E8F5
P 4150 3050
F 0 "C10" H 4000 2950 50  0000 L CNN
F 1 "100nF" H 4200 2950 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4188 2900 50  0001 C CNN
F 3 "~" H 4150 3050 50  0001 C CNN
F 4 "C14663" H 4150 3050 50  0001 C CNN "JLCPCB"
	1    4150 3050
	1    0    0    -1  
$EndComp
Connection ~ 4150 2900
Wire Wire Line
	4150 2900 4050 2900
$Comp
L power:GND #PWR042
U 1 1 5F02F445
P 4150 3200
F 0 "#PWR042" H 4150 2950 50  0001 C CNN
F 1 "GND" H 4155 3027 50  0000 C CNN
F 2 "" H 4150 3200 50  0001 C CNN
F 3 "" H 4150 3200 50  0001 C CNN
	1    4150 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 5F032943
P 5800 3550
F 0 "C11" H 5650 3450 50  0000 L CNN
F 1 "4.7µF" H 5850 3450 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5838 3400 50  0001 C CNN
F 3 "~" H 5800 3550 50  0001 C CNN
	1    5800 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3100 5800 3400
$Comp
L power:GND #PWR043
U 1 1 5F033AAC
P 5800 3700
F 0 "#PWR043" H 5800 3450 50  0001 C CNN
F 1 "GND" H 5805 3527 50  0000 C CNN
F 2 "" H 5800 3700 50  0001 C CNN
F 3 "" H 5800 3700 50  0001 C CNN
	1    5800 3700
	1    0    0    -1  
$EndComp
Text GLabel 7100 2900 2    50   Output ~ 0
DE
Text GLabel 7100 2800 2    50   Output ~ 0
VSYNC
Text GLabel 7100 2700 2    50   Output ~ 0
HSYNC
Text GLabel 7100 2500 2    50   Output ~ 0
PCLK
$Comp
L driver-board:YSX321SL X1
U 1 1 5F0375EF
P 5050 4000
F 0 "X1" H 5050 4365 50  0000 C CNN
F 1 "YSX321SL" H 5050 4274 50  0000 C CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 5000 4000 50  0001 C CNN
F 3 "" H 5000 4000 50  0001 C CNN
F 4 "C9002" H 5050 4183 50  0000 C CNN "JLCPCB"
	1    5050 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR044
U 1 1 5F03AD9F
P 5000 4300
F 0 "#PWR044" H 5000 4050 50  0001 C CNN
F 1 "GND" V 5000 4100 50  0000 C CNN
F 2 "" H 5000 4300 50  0001 C CNN
F 3 "" H 5000 4300 50  0001 C CNN
	1    5000 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR045
U 1 1 5F03AF97
P 5100 4300
F 0 "#PWR045" H 5100 4050 50  0001 C CNN
F 1 "GND" V 5100 4100 50  0000 C CNN
F 2 "" H 5100 4300 50  0001 C CNN
F 3 "" H 5100 4300 50  0001 C CNN
	1    5100 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3100 5500 3950
Wire Wire Line
	5500 3950 5300 3950
Wire Wire Line
	5400 3100 5400 3550
Wire Wire Line
	5400 3550 4750 3550
Wire Wire Line
	4750 3550 4750 3950
Wire Wire Line
	4750 3950 4800 3950
$Comp
L power:+3V0 #PWR036
U 1 1 5F0403D2
P 3750 2900
F 0 "#PWR036" H 3750 2750 50  0001 C CNN
F 1 "+3V0" V 3750 3000 50  0000 L CNN
F 2 "" H 3750 2900 50  0001 C CNN
F 3 "" H 3750 2900 50  0001 C CNN
	1    3750 2900
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V0 #PWR035
U 1 1 5F040E18
P 4750 2600
F 0 "#PWR035" H 4750 2450 50  0001 C CNN
F 1 "+3V0" V 4750 2700 50  0000 L CNN
F 2 "" H 4750 2600 50  0001 C CNN
F 3 "" H 4750 2600 50  0001 C CNN
	1    4750 2600
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V0 #PWR040
U 1 1 5F041F56
P 6000 3100
F 0 "#PWR040" H 6000 2950 50  0001 C CNN
F 1 "+3V0" V 6000 3300 50  0000 C CNN
F 2 "" H 6000 3100 50  0001 C CNN
F 3 "" H 6000 3100 50  0001 C CNN
	1    6000 3100
	-1   0    0    1   
$EndComp
NoConn ~ 6100 3100
NoConn ~ 6200 3100
NoConn ~ 6300 3100
NoConn ~ 6400 3100
NoConn ~ 6600 3100
NoConn ~ 7100 2600
NoConn ~ 5300 3100
NoConn ~ 4750 2700
NoConn ~ 4750 2500
NoConn ~ 4750 2400
$Comp
L power:GND #PWR037
U 1 1 5F095769
P 5600 3100
F 0 "#PWR037" H 5600 2850 50  0001 C CNN
F 1 "GND" V 5600 2900 50  0000 C CNN
F 2 "" H 5600 3100 50  0001 C CNN
F 3 "" H 5600 3100 50  0001 C CNN
	1    5600 3100
	1    0    0    -1  
$EndComp
Text GLabel 4750 1700 0    50   Output ~ 0
B0
Text GLabel 4750 1600 0    50   Output ~ 0
B1
Text GLabel 5300 1400 1    50   Output ~ 0
B2
Text GLabel 5400 1400 1    50   Output ~ 0
B3
Text GLabel 5500 1400 1    50   Output ~ 0
B4
Text GLabel 5600 1400 1    50   Output ~ 0
B5
Text GLabel 5700 1400 1    50   Output ~ 0
B6
Text GLabel 5800 1400 1    50   Output ~ 0
B7
Text GLabel 5900 1400 1    50   Output ~ 0
G0
Text GLabel 6000 1400 1    50   Output ~ 0
G1
Text GLabel 6100 1400 1    50   Output ~ 0
G2
Text GLabel 6200 1400 1    50   Output ~ 0
G3
Text GLabel 6300 1400 1    50   Output ~ 0
G4
Text GLabel 6400 1400 1    50   Output ~ 0
G5
Text GLabel 6500 1400 1    50   Output ~ 0
G6
Text GLabel 6600 1400 1    50   Output ~ 0
G7
Text GLabel 7100 1700 2    50   Output ~ 0
R0
Text GLabel 7100 1800 2    50   Output ~ 0
R1
Text GLabel 7100 1900 2    50   Output ~ 0
R2
Text GLabel 7100 2000 2    50   Output ~ 0
R3
Text GLabel 7100 2100 2    50   Output ~ 0
R4
Text GLabel 7100 2200 2    50   Output ~ 0
R5
Text GLabel 7100 2300 2    50   Output ~ 0
R6
Text GLabel 7100 2400 2    50   Output ~ 0
R7
$Comp
L Device:C C12
U 1 1 5F0BABCA
P 6350 5050
F 0 "C12" H 6465 5096 50  0000 L CNN
F 1 "10µF" H 6465 5005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6388 4900 50  0001 C CNN
F 3 "~" H 6350 5050 50  0001 C CNN
F 4 "C19702" H 6350 5050 50  0001 C CNN "JLCPCB"
	1    6350 5050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR046
U 1 1 5F0BB418
P 6350 4900
F 0 "#PWR046" H 6350 4750 50  0001 C CNN
F 1 "+3.3V" H 6365 5073 50  0000 C CNN
F 2 "" H 6350 4900 50  0001 C CNN
F 3 "" H 6350 4900 50  0001 C CNN
	1    6350 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR048
U 1 1 5F0BBD7E
P 6350 5200
F 0 "#PWR048" H 6350 4950 50  0001 C CNN
F 1 "GND" H 6355 5027 50  0000 C CNN
F 2 "" H 6350 5200 50  0001 C CNN
F 3 "" H 6350 5200 50  0001 C CNN
	1    6350 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 5F0CB172
P 7000 5050
F 0 "C13" H 7115 5096 50  0000 L CNN
F 1 "100nF" H 7115 5005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7038 4900 50  0001 C CNN
F 3 "~" H 7000 5050 50  0001 C CNN
F 4 "C14663" H 7000 5050 50  0001 C CNN "JLCPCB"
	1    7000 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR049
U 1 1 5F0CC6D1
P 7000 5200
F 0 "#PWR049" H 7000 4950 50  0001 C CNN
F 1 "GND" H 7005 5027 50  0000 C CNN
F 2 "" H 7000 5200 50  0001 C CNN
F 3 "" H 7000 5200 50  0001 C CNN
	1    7000 5200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR047
U 1 1 5F0CC952
P 7000 4900
F 0 "#PWR047" H 7000 4750 50  0001 C CNN
F 1 "+3V0" H 7015 5073 50  0000 C CNN
F 2 "" H 7000 4900 50  0001 C CNN
F 3 "" H 7000 4900 50  0001 C CNN
	1    7000 4900
	1    0    0    -1  
$EndComp
Text GLabel 4150 2900 1    50   Input ~ 0
FTRES
Text GLabel 4750 2800 0    50   Output ~ 0
IRQ
$EndSCHEMATC
