EESchema Schematic File Version 2  date Sun 08 Apr 2012 20:06:04 BST
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
LIBS:xyzebra
LIBS:stepper_4017-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "8 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	7400 4700 7400 4750
Connection ~ 9000 4800
Wire Wire Line
	9000 4700 9000 5100
Wire Wire Line
	9000 5100 8050 5100
Wire Wire Line
	8050 5100 8050 3900
Wire Wire Line
	8050 3900 7400 3900
Connection ~ 7950 3200
Wire Wire Line
	7400 3700 7400 3200
Wire Wire Line
	7400 3200 8050 3200
Wire Wire Line
	7950 3800 7950 3200
Wire Wire Line
	8650 3200 8450 3200
Wire Wire Line
	4550 3800 4550 3850
Wire Wire Line
	2500 3600 2250 3600
Wire Wire Line
	2500 3600 2500 3350
Wire Wire Line
	2500 3350 4250 3350
Wire Wire Line
	4250 3350 4250 3900
Wire Wire Line
	4250 3900 4100 3900
Connection ~ 2500 4200
Wire Wire Line
	2500 3700 2500 4550
Wire Wire Line
	2500 3700 2700 3700
Wire Wire Line
	2250 4150 2250 4250
Wire Wire Line
	4100 3700 4550 3700
Wire Wire Line
	5850 3900 6200 3900
Wire Wire Line
	6200 3700 5850 3700
Wire Wire Line
	5850 3800 6200 3800
Wire Wire Line
	5850 4000 6200 4000
Wire Wire Line
	5850 4100 6050 4100
Wire Wire Line
	6050 4100 6050 4950
Wire Wire Line
	6050 4950 4450 4950
Wire Wire Line
	4450 4950 4450 4000
Wire Wire Line
	4450 4000 4550 4000
Wire Wire Line
	2250 3600 2250 3650
Wire Wire Line
	2500 4550 4100 4550
Wire Wire Line
	2250 4200 2500 4200
Connection ~ 2250 4200
Connection ~ 4100 4100
Wire Wire Line
	4100 4550 4100 4100
Wire Wire Line
	2700 3950 2600 3950
Wire Wire Line
	2600 3950 2600 4650
Wire Wire Line
	8450 3200 8450 3800
Wire Wire Line
	8450 3800 8650 3800
Wire Wire Line
	9600 4100 9600 4300
Wire Wire Line
	9600 4300 9000 4300
Wire Wire Line
	9000 4300 9000 4100
Wire Wire Line
	9600 4800 9000 4800
Wire Wire Line
	7400 3800 8050 3800
Connection ~ 7950 3800
Wire Wire Line
	7400 4000 7800 4000
Wire Wire Line
	7800 4000 7800 5400
Wire Wire Line
	7800 5400 9600 5400
Wire Wire Line
	9600 5400 9600 4700
Connection ~ 9600 4800
$Comp
L GND #PWR?
U 1 1 4F81E18C
P 7400 4750
F 0 "#PWR?" H 7400 4750 30  0001 C CNN
F 1 "GND" H 7400 4680 30  0001 C CNN
	1    7400 4750
	1    0    0    -1  
$EndComp
$Comp
L DIODE D?
U 1 1 4F81E06C
P 9600 4500
F 0 "D?" H 9600 4600 40  0000 C CNN
F 1 "DIODE" H 9600 4400 40  0000 C CNN
	1    9600 4500
	0    1    1    0   
$EndComp
$Comp
L DIODE D?
U 1 1 4F81E06B
P 9000 4500
F 0 "D?" H 9000 4600 40  0000 C CNN
F 1 "DIODE" H 9000 4400 40  0000 C CNN
	1    9000 4500
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 4F81E06A
P 9300 4550
F 0 "R4" V 9380 4550 50  0000 C CNN
F 1 "6.8" V 9300 4550 50  0000 C CNN
	1    9300 4550
	-1   0    0    1   
$EndComp
$Comp
L INDUCTOR L?
U 1 1 4F81E069
P 9300 4100
F 0 "L?" V 9250 4100 40  0000 C CNN
F 1 "STEPPER" V 9400 4100 40  0000 C CNN
	1    9300 4100
	0    -1   -1   0   
$EndComp
$Comp
L INDUCTOR L?
U 1 1 4F81E041
P 8650 3500
F 0 "L?" V 8600 3500 40  0000 C CNN
F 1 "STEPPER" V 8750 3500 40  0000 C CNN
	1    8650 3500
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 4F81E027
P 8200 3500
F 0 "R3" V 8280 3500 50  0000 C CNN
F 1 "6.8" V 8200 3500 50  0000 C CNN
	1    8200 3500
	0    -1   -1   0   
$EndComp
$Comp
L DIODE D?
U 1 1 4F81DE36
P 8250 3200
F 0 "D?" H 8250 3300 40  0000 C CNN
F 1 "DIODE" H 8250 3100 40  0000 C CNN
	1    8250 3200
	-1   0    0    1   
$EndComp
$Comp
L DIODE D?
U 1 1 4F81DE2E
P 8250 3800
F 0 "D?" H 8250 3900 40  0000 C CNN
F 1 "DIODE" H 8250 3700 40  0000 C CNN
	1    8250 3800
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR?
U 1 1 4F81D8D1
P 2250 3100
F 0 "#PWR?" H 2250 3190 20  0001 C CNN
F 1 "+5V" H 2250 3190 30  0000 C CNN
	1    2250 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F81D8A3
P 4550 3850
F 0 "#PWR?" H 4550 3850 30  0001 C CNN
F 1 "GND" H 4550 3780 30  0001 C CNN
	1    4550 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F81D85C
P 2600 5050
F 0 "#PWR?" H 2600 5050 30  0001 C CNN
F 1 "GND" H 2600 4980 30  0001 C CNN
	1    2600 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F81D855
P 2250 4650
F 0 "#PWR?" H 2250 4650 30  0001 C CNN
F 1 "GND" H 2250 4580 30  0001 C CNN
	1    2250 4650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 4F81D81A
P 2700 4200
F 0 "#PWR?" H 2700 4290 20  0001 C CNN
F 1 "+5V" H 2700 4290 30  0000 C CNN
	1    2700 4200
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4F81D7C0
P 2600 4850
F 0 "C2" H 2650 4950 50  0000 L CNN
F 1 "10n" H 2650 4750 50  0000 L CNN
	1    2600 4850
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4F81D716
P 2250 4450
F 0 "C1" H 2300 4550 50  0000 L CNN
F 1 "1u" H 2300 4350 50  0000 L CNN
	1    2250 4450
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 4F81D5E2
P 2250 3900
F 0 "R2" V 2330 3900 50  0000 C CNN
F 1 "1K" V 2250 3900 50  0000 C CNN
	1    2250 3900
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 4F81D5DC
P 2250 3350
F 0 "R1" V 2330 3350 50  0000 C CNN
F 1 "10K" V 2250 3350 50  0000 C CNN
	1    2250 3350
	1    0    0    -1  
$EndComp
$Comp
L ULN2803A U?
U 1 1 4F81D485
P 6800 4200
F 0 "U?" H 6900 4775 60  0000 L BNN
F 1 "ULN2803A" H 6600 3625 60  0000 L TNN
	1    6800 4200
	1    0    0    -1  
$EndComp
$Comp
L LM555N U?
U 1 1 4F86BC76
P 3400 3900
F 0 "U?" H 3400 4000 70  0000 C CNN
F 1 "LM555N" H 3400 3800 70  0000 C CNN
	1    3400 3900
	1    0    0    -1  
$EndComp
$Comp
L 4017 U?
U 1 1 4F86BC6E
P 5200 4250
F 0 "U?" H 5100 4100 60  0000 C CNN
F 1 "4017" H 5100 3950 60  0000 C CNN
	1    5200 4250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
