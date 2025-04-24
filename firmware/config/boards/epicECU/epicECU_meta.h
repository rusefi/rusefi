#pragma once
// 12V power input switched from ignition 12v
#define EPICECU_12v nan

// 12V power input switched from ignition BRV
#define EPICECU_BRV EFI_ADC_9

// 14v stable buck/boost output 14v
#define EPICECU_14v nan

// 14v stable buck/boost output 14v
#define EPICECU_14v nan

// regulated 5V output 5v
#define EPICECU_5v nan

// Analog input PPS1
#define EPICECU_PPS1 EFI_ADC_2

// Analog input PPS2
#define EPICECU_PPS2 EFI_ADC_7

// Analog input AINP1
#define EPICECU_AINP1 EFI_ADC_14

// Analog input AINP2
#define EPICECU_AINP2 EFI_ADC_8

// Analog input AINP3
#define EPICECU_AINP3 EFI_ADC_15

// Analog input AUXTemp1
#define EPICECU_AUXTemp1 EFI_ADC_10

// Analog input AUXTemp2
#define EPICECU_AUXTemp2 EFI_ADC_3

// CAN1RX CAN1RX
#define EPICECU_CAN1RX D0

// CAN1TX CAN1TX
#define EPICECU_CAN1TX D1

// CAN2RX CAN2RX
#define EPICECU_CAN2RX A8

// CAN2TX CAN2TX
#define EPICECU_CAN2TX A15

// dipswitch 10k pullup default, 10k pulldown DINP1
#define EPICECU_DINP1 C13

// dipswitch 10k pullup default, 10k pulldown DINP2
#define EPICECU_DINP2 E6

// dipswitch 10k pullup default, 10k pulldown DINP3
#define EPICECU_DINP3 E5

// dipswitch 10k pullup default, 10k pulldown DINP4
#define EPICECU_DINP4 E4

// dipswitch 10k pullup default, 10k pulldown DINP5
#define EPICECU_DINP5 E3

// dipswitch 10k pullup default, 10k pulldown DINP6
#define EPICECU_DINP6 E2

// dipswitch 10k pullup default, 10k pulldown DINP7
#define EPICECU_DINP7 E1

// dipswitch 10k pullup default, 10k pulldown DINP8
#define EPICECU_DINP8 E0

// Analog input CLT
#define EPICECU_CLT EFI_ADC_13

// FLEX sensor input FLEX
#define EPICECU_FLEX J15

// Hall crank input RPM1
#define EPICECU_RPM1 I13

// Hall cam 1 intake input RPM2
#define EPICECU_RPM2 I12

// Hall cam 2 exhaust input RPM3
#define EPICECU_RPM3 I11

// Hall cam 3 intake input RPM4
#define EPICECU_RPM4 I10

// Hall cam 4 exhaust input RPM5
#define EPICECU_RPM5 I9

// Hall 6 input RPM6
#define EPICECU_RPM6 I8

// Hall 7 input RPM7
#define EPICECU_RPM7 I7

// Hall 8 input RPM8
#define EPICECU_RPM8 I6

// High Side output 1 HS1
#define EPICECU_HS1 D2

// High Side output 2 HS2
#define EPICECU_HS2 D7

// High Side output 3 HS3
#define EPICECU_HS3 C11

// High Side output 4 HS4
#define EPICECU_HS4 C12

// 0-5V for smart coil IGN1
#define EPICECU_IGN1 J3

// 0-5V for smart coil IGN2
#define EPICECU_IGN2 J4

// 0-5V for smart coil IGN3
#define EPICECU_IGN3 I15

// 0-5V for smart coil IGN4
#define EPICECU_IGN4 J0

// 0-5V for smart coil IGN5
#define EPICECU_IGN5 H3

// 0-5V for smart coil IGN6
#define EPICECU_IGN6 H4

// 0-5V for smart coil IGN7
#define EPICECU_IGN7 J1

// 0-5V for smart coil IGN8
#define EPICECU_IGN8 J2

// 0-5V for smart coil IGN9
#define EPICECU_IGN9 H5

// 0-5V for smart coil IGN10
#define EPICECU_IGN10 B2

// 0-5V for smart coil IGN11
#define EPICECU_IGN11 I14

// 0-5V for smart coil IGN12
#define EPICECU_IGN12 H2

// Injector 1 INJ1
#define EPICECU_INJ1 G9

// Injector 2 INJ2
#define EPICECU_INJ2 J14

// Injector 3 INJ3
#define EPICECU_INJ3 I1

// Injector 4 INJ4
#define EPICECU_INJ4 I2

// Injector 5, INJ5
#define EPICECU_INJ5 C10

// Injector 6 INJ6
#define EPICECU_INJ6 I3

// Injector 7 INJ7
#define EPICECU_INJ7 H15

// Injector 8 INJ8
#define EPICECU_INJ8 I0

// Injector 9 INJ9
#define EPICECU_INJ9 H13

// Injector 10 INJ10
#define EPICECU_INJ10 H14

// Injector 11 INJ11
#define EPICECU_INJ11 A9

// Injector 12 INJ12
#define EPICECU_INJ12 A10

// Analog input IAT
#define EPICECU_IAT EFI_ADC_11

// Knock sensor 1 input Knock1
#define EPICECU_Knock1 F0

// Knock sensor 2 input Knock2
#define EPICECU_Knock2 F1

// Low Side output 1 LS1
#define EPICECU_LS1 G13

// Low Side output 2 LS2
#define EPICECU_LS2 G12

// Low Side output 3 LS3
#define EPICECU_LS3 G11

// Low Side output 4 LS4
#define EPICECU_LS4 G10

// Low Side output 5 LS5
#define EPICECU_LS5 C8

// Low Side output 6 LS6
#define EPICECU_LS6 C6

// Low Side output 7 LS7
#define EPICECU_LS7 G5

// Low Side output 8 LS8
#define EPICECU_LS8 G6

// Low Side output 9 LS9
#define EPICECU_LS9 C7

// Low Side output 10 LS10
#define EPICECU_LS10 G7

// Low Side output 11 LS11
#define EPICECU_LS11 G8

// Low Side output 12 LS12
#define EPICECU_LS12 K0

// Low Side output 13 LS13
#define EPICECU_LS13 K1

// Low Side output 14 LS14
#define EPICECU_LS14 G3

// Low Side output 15 LS15
#define EPICECU_LS15 G4

// Low Side output 16 LS16
#define EPICECU_LS16 G2

// Low Side output 17 LS17
#define EPICECU_LS17 K2

// Low Side output 18 LS18
#define EPICECU_LS18 J13

// Low Side output 19 LS19
#define EPICECU_LS19 C9

// Analog input MAP
#define EPICECU_MAP EFI_ADC_6

// Analog input OT
#define EPICECU_OT EFI_ADC_12

// Analog input O2
#define EPICECU_O2 EFI_ADC_5

// Analog input O2_2
#define EPICECU_O2_2 EFI_ADC_4

// egt+ EGT1_CS
#define EPICECU_EGT1_CS B8

// egt+ EGT2_CS
#define EPICECU_EGT2_CS G15

// egt+ EGT3_CS
#define EPICECU_EGT3_CS K7

// egt+ EGT4_CS
#define EPICECU_EGT4_CS K6

// egt+ EGT5_CS
#define EPICECU_EGT5_CS K5

// egt+ EGT6_CS
#define EPICECU_EGT6_CS K4

// egt+ EGT7_CS
#define EPICECU_EGT7_CS K3

// egt+ EGT8_CS
#define EPICECU_EGT8_CS G14

// Low Side output 20,Tacho, pullup dipswitch to 12v (def) or 5v LS20
#define EPICECU_LS20 J12

// Analog input TPS1
#define EPICECU_TPS1 EFI_ADC_1

// Analog input TPS2
#define EPICECU_TPS2 EFI_ADC_0

// VR1+ RPM9
#define EPICECU_RPM9 I5

// VR2+ RPM10
#define EPICECU_RPM10 I4

