

# CORE 48 v2.1

![image](https://github.com/opelpanfan/CORE48/assets/12942077/5bdb0bb8-2ab7-4919-9121-8c81560b6320)

## Features

- 8 Injectors
- 8 Coils
- 8 Low Side Switch
- 4 VR / 8 Hall Triggers
- 4 Digital Inputs
- 8 Analogue 0-5v inputs (all can be used as temperature inputs via 2.49k pull up resistor SW10 and SW11)
- DBW
- 2 Wideband (14point7)
- 2 Canbus
- 2 EGT
- 4bar map
- Onboard Barometer
- Onboard SD card/Removable SD Card
- Bluetooth

## Firmware

<div align="center">

<a href="https://github.com/rusefi/rusefi" target="_blank">
  <img src="https://raw.githubusercontent.com/wiki/rusefi/rusefi/Images/logo_new.png" alt="rusEFI" width="600" />
</a>
<br/>
<b>GPL open-source ECU</b>
<br/>
</div>

## Interactive Pinout Manual 

https://opelpanfan.github.io/CORE48/core48_21.html

### Black Connector

<div align="center">
<a href="https://opelpanfan.github.io/CORE48/core48_21.html" target="_blank">
  <img src="https://github.com/opelpanfan/CORE48/assets/12942077/cb34225e-63c1-4ae5-84aa-97a96f62b942" alt="black" width="100%" />
</a>
</div>

| Pin Number | STM32 Pin Number | TS Name  | Typical Function | Alternative Function #1	 | Alternative Function #2 |
| ------ | ------ | ------ | ------ | ------ | ------ |
| A1 | PG3  | Low Side 12 (G3)	| FUEL PUMP	            |                           |               |
| A2 | PG4  | Low Side 13 (G4)	| FAN RELAY		        |                           |               |
| A3 | PG5  | Low Side 14 (G5)	| BOOST	                |                           |               |
| A4 | PG6  | Low Side 15 (G6)	| IDLE	                |                           |               |
| A5 |      | 	                | LSU HEATER-		    | LSU 4.9 CONNECTOR PIN 3	| WHITE WIRE    |
| A6 |      | 	                | LSU IA			    | LSU 4.9 CONNECTOR PIN 5	| GREEN WIRE    |
| A7 |      | 	                | LSU VGND			    | LSU 4.9 CONNECTOR PIN 2	| YELLOW WIRE   |
| A8 |      | 	                | LSU HEATER+		    | LSU 4.9 CONNECTOR PIN 4	| GREY WIRE     |
| B1 |      | Switch SW5		| DBW+		            | Low Side 11 (G2)	        |               |
| B2 |      | Switch SW4		| CAN LOW		        | Digital Input 1 (D5)	    | USB D+        |
| B3 |      | Switch SW3		| CAN HIGH		        | Digital Input 2 (D4)	    | USB D-        |
| B4 |      | Switch SW2	 	| Digital Input 4 (D2)  | Analogue Input 8 (A4)	    | Knock 1 (C0)  |
| B5 |      | Switch SW1		| Digital Input 3 (D3)	| Analogue Input 9 (A1)	    | Knock 2 (C1)  |
| B6 |      | 	                | LSU NERMEST	        | LSU 4.9 CONNECTOR PIN 6	| BLACK WIRE    |
| B7 |      | 	                | LSU IP	            | LSU 4.9 CONNECTOR PIN 1	| RED WIRE      |
| B8 |      | +12V from Battery	| +12V Battery	        |                           |               |
| C1 |      | Switch SW6		| DBW-		            | Low Side 10 (D15)	        |               |
| C2 | PE2  | VR1+		        | VR1+		            | HALL 1 (PG1)	            |               |
| C3 |      | VR1-		        | VR1-		            | HALL 2 (PE7)	            |               |
| C4 |      | VR2-		        | VR2-		            | HALL 4 (PE8)	            |               |
| C5 | PE3  | VR2+		        | VR2+		            | HALL 3 (PE9)	            |               |
| C6 | PG7  | Low Side 16 (G7)	| VVTI	                |                           |               |
| C7 |      | Sensor +5v Supply	| Sensor +5v Supply		|                           |               |
| C8 |      | Ground	        | Ground	            |                           |               |

### Grey Connector


<div align="center">
<a href="https://opelpanfan.github.io/CORE48/core48_21.html" target="_blank">
  <img src="https://github.com/opelpanfan/CORE48/assets/12942077/ebb802ea-ebaf-4b2f-a6b5-6568010a384e" alt="grey" width="100%" />
</a>
</div>

| Pin Number | STM32 Pin Number | TS Name  | Typical Function | Alternative Function #1	 | 
| ------ | ------ | ------ | ------ | ------ | 
| A1 | PD6  | Low Side 1 (D6)		 |INJECTOR 1		|                                           |
| A2 | PD7  | Low Side 2 (D7)		 |INJECTOR 2		|                                           |
| A3 | PD8  | Low Side 3 (D8)		 |INJECTOR 3		|                                           |
| A4 | PD9  | Low Side 4 (D9)		 |INJECTOR 4		|                                           |
| A5 | PD10 | Low Side 5 (D10)		 |INJECTOR 5		|                                           |
| A6 | PD11 | Low Side 6 (D11)		 |INJECTOR 6		|                                           |
| A7 | PD12 | Low Side 7 (D12)		 |INJECTOR 7		|                                           |
| A8 | PD13 | Low Side 8 (D13)		 |INJECTOR 8		|                                           |
| B1 | PG15 | High Side 1 (G15)		 |COIL 1			| +5v or +12v signal depends on selection	|
| B2 | PG8  | High Side 2 (G8)		 |COIL 2			| +5v or +12v signal depends on selection   |
| B3 | PE10 | High Side 3 (E10)		 |COIL 3		    | +5v or +12v signal depends on selection	|
| B4 | PE11 | High Side 4 (E11)	 	 |COIL 4			| +5v or +12v signal depends on selection	|
| B5 | PE12 | High Side 5 (E12)		 |COIL 5			| +5v or +12v signal depends on selection	|
| B6 | PE13 | High Side 6 (E13)		 |COIL 6		    | +5v or +12v signal depends on selection	|
| B7 | PE14 | High Side 7 (E14)		 |COIL 7		    | +5v or +12v signal depends on selection	|
| B8 | PE15 | High Side 8 (E15)		 |COIL 8		    |                                           |
| C1 | PC5  | Analog Inputs 1 (C5)	 |TPS	            |                                           |
| C2 | PB0  | Analog Inputs 2 (B0)	 |CLT		        | turn SW10 B0 ON to enable 2.49k PULL UP	|
| C3 | PA7  | Analog Inputs 3 (A7)	 |IAT			    | turn SW10 A7 ON to enable 2.49k PULL UP	|
| C4 | PC4  | Analog Inputs 4 (C4)	 |OIL PRESSURE		| HALL 4 (PE8)	                            |
| C5 | PA5  | Analog Inputs 5 (A5)	 |FUEL PRESSURE		| HALL 3 (PE9)	                            |
| C6 | PA6  | Analog Inputs 6 (A6)	 |SPARE ANALOG INPUT|                                           |
| C7 | PA3  | Analog Inputs 7 (A3)	 |SPARE ANALOG INPUT|                                           |
| C8 | PD14 | High Side 9 (D14)		 |TACHO		        | PULLUP +5v or +12v depends on selection   |


## PCB Layout

### :warning: **Please check the connector pinout table as some silk prints on the PCB are wrong.**

![image](https://github.com/opelpanfan/CORE48/assets/12942077/3469a74a-7623-40d8-a7de-25a0a55c2823)

## Solder jumper configuration

| Switch ID | Link to Connector | Image  | RED | BLUE	 | GREEN |
| ------ | ------ | ------ | ------ | ------ |  ------ |
| SW1   | B_B5  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/1757576a-7584-4ea2-9035-57acbbaf3b19)  | Knock 2 (C1)  | Analog Inputs 9 (A1) | Digital Input 3 (D3) |
| SW2   | B_C6  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/1b31a344-36ec-4790-a561-0a1140de30eb)  | Digital Input 4 (D2)   | Analog Inputs 8 (A4) | Knock 1 (C0) |
| SW3   | B_B3  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/711358c0-e6ea-4739-a8e7-d2e205c731dd)  | USB D+  | Digital Input 1 (D5) | CAN LOW |
| SW4   | B_B2  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/13c2c3d5-92b9-4165-abd7-87adfbc2b485)  | CAN HIGH  | Digital Input 2 (D4) | USB D- |
| SW5   | B_B1  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/f82bcf90-6858-464f-9b6d-c5ac517da40f)  | Low Side 11 (G2)  | DBW+ |  |
| SW6   | B_C1  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/f0aa1e6a-c734-4d53-98a2-01fd3d8e0ee6)  | Low Side 10 (D15)  | DBW- |  |
| SW7   | B_C8  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/4ac00500-5d3b-4104-9123-b3110029daaf)  | TACHO PULL UP to +12v  | TACHO PULL UP to +5v |  |
| SW8   | COIL Signal  | ![image](https://github.com/opelpanfan/CORE48/assets/12942077/ce27c313-928a-4be8-b1f0-466bbbb1c55b) | COIL Trigger +12v  | COIL Trigger +5v |  |

### VR Trigger Configuration

![image](https://github.com/opelpanfan/CORE48/assets/12942077/76f164eb-7be7-466e-9e0a-0189acd6b181) 

### HALL Trigger Configuration

![image](https://github.com/opelpanfan/CORE48/assets/12942077/5c0ed98f-dc9c-4850-972a-7a3a368713a3)

### SW10 and SW11: Analogue input PULLUP resistors (2.49k) to +5V for Temperature Sensor

>Analogue input pull up settings to use for temperature inputs:
>Most of Analogue inputs can be used as temperature inputs with 2.49Kohm pull up to +5v resistors.
>These resistors can be enabled by flipping toggle switch to ON position.

![image](https://github.com/opelpanfan/CORE48/assets/12942077/feb964ff-f6d2-4a99-856c-991d7fd84f30)

### SW12 Digital Input PULLUP resistors (680k) to +5v

>Digital input 680kohm pull up resistors to +5v.
>If your digital input requires pull up resistor taht can be enabled by flipping toggle switch to ON possition.

![image](https://github.com/opelpanfan/CORE48/assets/12942077/7b24b38e-0779-4887-ad61-4abda1b18171)

### SW9 CAN bus terminator Resistors and Knock Audio Filters

>1. toggle switch is to ON possition to enable 120ohm terminating resistor on CAN1
>2. toggle switch is to ON possition to enable 120ohm terminating resistor on CAN2
>3. toggle switch is to ON possition to enable 3.3nF filter capacitor on KNOCK 1 Input
>4. toggle switch is to ON possition to enable 3.3nF filter capacitor on KNOCK 2 Input

![image](https://github.com/opelpanfan/CORE48/assets/12942077/d3dc0053-40e4-4aa0-af15-56401b117058)

## Additional Connectors

![image](https://github.com/opelpanfan/CORE48/assets/12942077/232973ae-c908-4171-bbfc-a493dfd78635)


### CAN Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/1f331c42-2952-4920-9695-7dd1e4caabf2)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | CAN 1 LOW |
| 2 |  | CAN 1 HIGH |
| 3 |  | CAN 2 LOW |
| 4 |  | CAN 2 HIGH |

### USB Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/d67afd6f-4710-4ef0-bb53-0fb0c96deeb1)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | USB +5V |
| 2 |  | USB D- |
| 3 |  | USB D+ |
| 4 |  | GND |

### Stepper Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/c8108ecd-139e-4c5d-8e20-70fbb49c9b4d)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | STEP 1B |
| 2 |  | STEP 1A |
| 3 |  | STEP 2A |
| 4 |  | STEP 2B |

### VR3/VR4 Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/c49c41fe-d15f-4377-80a0-b8a9ff652205)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 | PE4/PF13  | VR3+/HALL5 (F13) |
| 2 | PF14 | VR3-/HALL6 (F14) |
| 3 | PF15 | VR4-/HALL8 (F15) |
| 4 | PE5/PG0 | VR4+/HALL7 (G0) |

### EGT Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/1dcf5bb6-c38a-41e1-8293-b87aee4d508b)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | EGT1 T- |
| 2 |  | EGT1 T+ |
| 3 |  | EGT2 T- |
| 4 |  | EGT2 T+ |

### Knock/Serial Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/24d01a55-a5b8-459b-b840-11c136f379a0)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | UART6 RX |
| 2 |  | UART6 TX |
| 3 |  | KNOCK 2 Audio |
| 4 |  | KNOCK 1 Audio |

### DBW Connector
![image](https://github.com/opelpanfan/CORE48/assets/12942077/c65efae5-5758-409d-acdc-784bcb1f960b)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | GND |
| 2 |  | +5V |
| 3 |  | ET- |
| 4 |  | ET+ |
| 5 |  | Analog Inputs 8 (A4) |
| 6 |  | Analog Inputs 9 (A1) |

### LSU Connector

![image](https://github.com/opelpanfan/CORE48/assets/12942077/63d5e15a-5c4e-44d1-8edd-1df03da7e6d4)
![image](https://github.com/opelpanfan/CORE48/assets/12942077/05f5cd9a-93ac-4810-8be9-47d00f090758)

| Pin   | STM32 PIN | Function  |
| ------ | ------ | ------ |
| 1 |  | LSU Connector 4 |
| 2 |  | LSU Connector 3 |
| 3 |  | LSU Connector 2 |
| 4 |  | LSU Connector 1 |
| 5 |  | LSU Connector 6 |
| 6 |  | LSU Connector 5 |




