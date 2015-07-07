///////////////////////////////////////////////////////////////////////////////
//                                                                            /
// IAR ANSI C/C++ Compiler V6.50.3.4676/W32 for ARM     30/Jun/2015  23:10:06 /
// Copyright 1999-2013 IAR Systems AB.                                        /
//                                                                            /
//    Cpu mode        =  thumb                                                /
//    Endian          =  little                                               /
//    Source file     =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\serial /
//                       _over_usb\usbcfg.c                                   /
//    Command line    =  F:\stuff\rusefi_sourceforge\firmware\hw_layer\serial /
//                       _over_usb\usbcfg.c -lCN F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\Debug\List\ -lA                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                        -o F:\stuff\rusefi_sourceforge\firmware\iar\Debug\O /
//                       bj\ --no_cse --no_unroll --no_inline                 /
//                       --no_code_motion --no_tbaa --no_clustering           /
//                       --no_scheduling --debug --endian=little              /
//                       --cpu=Cortex-M4 -e --fpu=VFPv4_sp --dlib_config      /
//                       "C:\Program Files (x86)\IAR Systems\Embedded         /
//                       Workbench 6.5\arm\INC\c\DLib_Config_Normal.h"        /
//                       --misrac2004=1.2-1.4,2.3,3.4,4.2,6.5-7,8.2-8.3,9.3,  /
//                       12.3,13.6-13,15.5,17.3,19.17,20.7-20.9,20.11 -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\kernel\include\ -I F:\stuff\rusefi_sourceforge\fi /
//                       rmware\iar\..\chibios\os\ports\common\ARMCMx\ -I     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\common\ARMCMx\CMSIS\include\ -I             /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\ports\IAR\ARMCMx\ -I F:\stuff\rusefi_sourceforge\ /
//                       firmware\iar\..\chibios\os\ports\IAR\ARMCMx\STM32F4x /
//                       x\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ch /
//                       ibios\os\hal\include\ -I F:\stuff\rusefi_sourceforge /
//                       \firmware\iar\..\chibios\os\hal\platforms\STM32\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\DMAv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\GPIOv2\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\I2Cv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\SPIv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\TIMv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\RTCv2\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\USARTv1\ -I                   /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32\OTGv1\ -I                     /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\hal\platforms\STM32F4xx\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       os\various\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\chibios\os\various\devices_lib\accel\ -I      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\chibios\ /
//                       boards\ST_STM32F4_DISCOVERY\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\ /
//                       controllers\algo\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\controllers\core\ -I                    /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\system\ -I F:\stuff\rusefi_sourceforge\firmware\ /
//                       iar\..\controllers\sensors\ -I                       /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\controll /
//                       ers\math\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\controllers\trigger\ -I                         /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                        -I F:\stuff\rusefi_sourceforge\firmware\iar\..\conf /
//                       ig\ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\c /
//                       onfig\engines\ -I F:\stuff\rusefi_sourceforge\firmwa /
//                       re\iar\..\config\stm32f4ems\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\console\ /
//                       binary\ -I F:\stuff\rusefi_sourceforge\firmware\iar\ /
//                       ..\console_util\ -I F:\stuff\rusefi_sourceforge\firm /
//                       ware\iar\..\development\ -I                          /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\developm /
//                       ent\test\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \algo\ -I F:\stuff\rusefi_sourceforge\firmware\iar\. /
//                       .\hw_layer\lcd\ -I F:\stuff\rusefi_sourceforge\firmw /
//                       are\iar\..\hw_layer\stm32f4\ -I                      /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\hw_layer /
//                       \serial_over_usb\ -I F:\stuff\rusefi_sourceforge\fir /
//                       mware\iar\..\hw_layer\ -I                            /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext\ -I  /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\ext_algo /
//                       \ -I F:\stuff\rusefi_sourceforge\firmware\iar\..\dev /
//                       elopment\ -I F:\stuff\rusefi_sourceforge\firmware\ia /
//                       r\..\development\hw_layer\ -I                        /
//                       F:\stuff\rusefi_sourceforge\firmware\iar\..\util\    /
//                       -On                                                  /
//    MISRA C version =  2004                                                 /
//      Enabled       =  1.2-1.4 2.3 3.4 4.2 6.5 7 8.2 8.3 9.3 12.3 13.6      /
//                       13.7 15.5 17.3 19.17 20.7-20.9 20.11                 /
//      Checked       =  1.4 2.3 4.2 6.5 7 8.2 8.3 9.3 12.3 13.7 15.5         /
//                       20.7-20.9 20.11                                      /
//      Not checked   =  1.1-1.3 1.5-2.2 2.4-4.1 5-6.4 8.1 8.4-9.2 10-12.2    /
//                       12.4-13.6 14-15.4 16-20.6 20.10 20.12 21             /
//    List file       =  F:\stuff\rusefi_sourceforge\firmware\iar\Debug\List\ /
//                       usbcfg.s                                             /
//                                                                            /
//                                                                            /
///////////////////////////////////////////////////////////////////////////////

        NAME usbcfg

        RTMODEL "__SystemLibrary", "DLib"
        AAPCS BASE,INTERWORK,VFP
        PRESERVE8
        REQUIRE8

        #define SHT_PROGBITS 0x1

        EXTERN USBD1
        EXTERN dbg_check_lock_from_isr
        EXTERN dbg_check_unlock_from_isr
        EXTERN sduConfigureHookI
        EXTERN sduDataReceived
        EXTERN sduDataTransmitted
        EXTERN sduInterruptTransmitted
        EXTERN sduRequestsHook
        EXTERN usbInitEndpointI

        PUBLIC SDU1
        PUBLIC serusbcfg
        PUBLIC usbcfg
        
          CFI Names cfiNames0
          CFI StackFrame CFA R13 DATA
          CFI Resource R0:32, R1:32, R2:32, R3:32, R4:32, R5:32, R6:32, R7:32
          CFI Resource R8:32, R9:32, R10:32, R11:32, R12:32, R13:32, R14:32
          CFI Resource D0:64, D1:64, D2:64, D3:64, D4:64, D5:64, D6:64, D7:64
          CFI Resource D8:64, D9:64, D10:64, D11:64, D12:64, D13:64, D14:64
          CFI Resource D15:64
          CFI EndNames cfiNames0
        
          CFI Common cfiCommon0 Using cfiNames0
          CFI CodeAlign 2
          CFI DataAlign 4
          CFI ReturnAddress R14 CODE
          CFI CFA R13+0
          CFI R0 Undefined
          CFI R1 Undefined
          CFI R2 Undefined
          CFI R3 Undefined
          CFI R4 SameValue
          CFI R5 SameValue
          CFI R6 SameValue
          CFI R7 SameValue
          CFI R8 SameValue
          CFI R9 SameValue
          CFI R10 SameValue
          CFI R11 SameValue
          CFI R12 Undefined
          CFI R14 SameValue
          CFI D0 Undefined
          CFI D1 Undefined
          CFI D2 Undefined
          CFI D3 Undefined
          CFI D4 Undefined
          CFI D5 Undefined
          CFI D6 Undefined
          CFI D7 Undefined
          CFI D8 SameValue
          CFI D9 SameValue
          CFI D10 SameValue
          CFI D11 SameValue
          CFI D12 SameValue
          CFI D13 SameValue
          CFI D14 SameValue
          CFI D15 SameValue
          CFI EndCommon cfiCommon0
        
// F:\stuff\rusefi_sourceforge\firmware\hw_layer\serial_over_usb\usbcfg.c
//    1 /*
//    2     ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
//    3 
//    4     Licensed under the Apache License, Version 2.0 (the "License");
//    5     you may not use this file except in compliance with the License.
//    6     You may obtain a copy of the License at
//    7 
//    8         http://www.apache.org/licenses/LICENSE-2.0
//    9 
//   10     Unless required by applicable law or agreed to in writing, software
//   11     distributed under the License is distributed on an "AS IS" BASIS,
//   12     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   13     See the License for the specific language governing permissions and
//   14     limitations under the License.
//   15 */
//   16 
//   17 #include "main.h"
//   18 
//   19 #if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
//   20 
//   21 /*
//   22  * Endpoints to be used for USBD1.
//   23  */
//   24 #define USBD1_DATA_REQUEST_EP           1
//   25 #define USBD1_DATA_AVAILABLE_EP         1
//   26 #define USBD1_INTERRUPT_REQUEST_EP      2
//   27 
//   28 /*
//   29  * USB Device Descriptor.
//   30  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   31 static const uint8_t vcom_device_descriptor_data[18] = {
vcom_device_descriptor_data:
        DATA
        DC8 18, 1, 16, 1, 2, 0, 0, 64, 131, 4, 64, 87, 0, 2, 1, 2, 3, 1, 0, 0
//   32   USB_DESC_DEVICE       (0x0110,        /* bcdUSB (1.1).                    */
//   33                          0x02,          /* bDeviceClass (CDC).              */
//   34                          0x00,          /* bDeviceSubClass.                 */
//   35                          0x00,          /* bDeviceProtocol.                 */
//   36                          0x40,          /* bMaxPacketSize.                  */
//   37                          0x0483,        /* idVendor (ST).                   */
//   38                          0x5740,        /* idProduct.                       */
//   39                          0x0200,        /* bcdDevice.                       */
//   40                          1,             /* iManufacturer.                   */
//   41                          2,             /* iProduct.                        */
//   42                          3,             /* iSerialNumber.                   */
//   43                          1)             /* bNumConfigurations.              */
//   44 };
//   45 
//   46 /*
//   47  * Device Descriptor wrapper.
//   48  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   49 static const USBDescriptor vcom_device_descriptor = {
vcom_device_descriptor:
        DATA
        DC32 18, vcom_device_descriptor_data
//   50   sizeof vcom_device_descriptor_data,
//   51   vcom_device_descriptor_data
//   52 };
//   53 
//   54 /* Configuration Descriptor tree for a CDC.*/

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//   55 static const uint8_t vcom_configuration_descriptor_data[67] = {
vcom_configuration_descriptor_data:
        DATA
        DC8 9, 2, 67, 0, 2, 1, 0, 192, 50, 9, 4, 0, 0, 1, 2, 2, 1, 0, 5, 36, 0
        DC8 16, 1, 5, 36, 1, 0, 1, 4, 36, 2, 2, 5, 36, 6, 0, 1, 7, 5, 130, 3, 8
        DC8 0, 255, 9, 4, 1, 0, 2, 10, 0, 0, 0, 7, 5, 1, 2, 64, 0, 0, 7, 5, 129
        DC8 2, 64, 0, 0, 0
//   56   /* Configuration Descriptor.*/
//   57   USB_DESC_CONFIGURATION(67,            /* wTotalLength.                    */
//   58                          0x02,          /* bNumInterfaces.                  */
//   59                          0x01,          /* bConfigurationValue.             */
//   60                          0,             /* iConfiguration.                  */
//   61                          0xC0,          /* bmAttributes (self powered).     */
//   62                          50),           /* bMaxPower (100mA).               */
//   63   /* Interface Descriptor.*/
//   64   USB_DESC_INTERFACE    (0x00,          /* bInterfaceNumber.                */
//   65                          0x00,          /* bAlternateSetting.               */
//   66                          0x01,          /* bNumEndpoints.                   */
//   67                          0x02,          /* bInterfaceClass (Communications
//   68                                            Interface Class, CDC section
//   69                                            4.2).                            */
//   70                          0x02,          /* bInterfaceSubClass (Abstract
//   71                                          Control Model, CDC section 4.3).   */
//   72                          0x01,          /* bInterfaceProtocol (AT commands,
//   73                                            CDC section 4.4).                */
//   74                          0),            /* iInterface.                      */
//   75   /* Header Functional Descriptor (CDC section 5.2.3).*/
//   76   USB_DESC_BYTE         (5),            /* bLength.                         */
//   77   USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
//   78   USB_DESC_BYTE         (0x00),         /* bDescriptorSubtype (Header
//   79                                            Functional Descriptor.           */
//   80   USB_DESC_BCD          (0x0110),       /* bcdCDC.                          */
//   81   /* Call Management Functional Descriptor. */
//   82   USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
//   83   USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
//   84   USB_DESC_BYTE         (0x01),         /* bDescriptorSubtype (Call Management
//   85                                            Functional Descriptor).          */
//   86   USB_DESC_BYTE         (0x00),         /* bmCapabilities (D0+D1).          */
//   87   USB_DESC_BYTE         (0x01),         /* bDataInterface.                  */
//   88   /* ACM Functional Descriptor.*/
//   89   USB_DESC_BYTE         (4),            /* bFunctionLength.                 */
//   90   USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
//   91   USB_DESC_BYTE         (0x02),         /* bDescriptorSubtype (Abstract
//   92                                            Control Management Descriptor).  */
//   93   USB_DESC_BYTE         (0x02),         /* bmCapabilities.                  */
//   94   /* Union Functional Descriptor.*/
//   95   USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
//   96   USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
//   97   USB_DESC_BYTE         (0x06),         /* bDescriptorSubtype (Union
//   98                                            Functional Descriptor).          */
//   99   USB_DESC_BYTE         (0x00),         /* bMasterInterface (Communication
//  100                                            Class Interface).                */
//  101   USB_DESC_BYTE         (0x01),         /* bSlaveInterface0 (Data Class
//  102                                            Interface).                      */
//  103   /* Endpoint 2 Descriptor.*/
//  104   USB_DESC_ENDPOINT     (USBD1_INTERRUPT_REQUEST_EP|0x80,
//  105                          0x03,          /* bmAttributes (Interrupt).        */
//  106                          0x0008,        /* wMaxPacketSize.                  */
//  107                          0xFF),         /* bInterval.                       */
//  108   /* Interface Descriptor.*/
//  109   USB_DESC_INTERFACE    (0x01,          /* bInterfaceNumber.                */
//  110                          0x00,          /* bAlternateSetting.               */
//  111                          0x02,          /* bNumEndpoints.                   */
//  112                          0x0A,          /* bInterfaceClass (Data Class
//  113                                            Interface, CDC section 4.5).     */
//  114                          0x00,          /* bInterfaceSubClass (CDC section
//  115                                            4.6).                            */
//  116                          0x00,          /* bInterfaceProtocol (CDC section
//  117                                            4.7).                            */
//  118                          0x00),         /* iInterface.                      */
//  119   /* Endpoint 3 Descriptor.*/
//  120   USB_DESC_ENDPOINT     (USBD1_DATA_AVAILABLE_EP,       /* bEndpointAddress.*/
//  121                          0x02,          /* bmAttributes (Bulk).             */
//  122                          0x0040,        /* wMaxPacketSize.                  */
//  123                          0x00),         /* bInterval.                       */
//  124   /* Endpoint 1 Descriptor.*/
//  125   USB_DESC_ENDPOINT     (USBD1_DATA_REQUEST_EP|0x80,    /* bEndpointAddress.*/
//  126                          0x02,          /* bmAttributes (Bulk).             */
//  127                          0x0040,        /* wMaxPacketSize.                  */
//  128                          0x00)          /* bInterval.                       */
//  129 };
//  130 
//  131 /*
//  132  * Configuration Descriptor wrapper.
//  133  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  134 static const USBDescriptor vcom_configuration_descriptor = {
vcom_configuration_descriptor:
        DATA
        DC32 67, vcom_configuration_descriptor_data
//  135   sizeof vcom_configuration_descriptor_data,
//  136   vcom_configuration_descriptor_data
//  137 };
//  138 
//  139 /*
//  140  * U.S. English language identifier.
//  141  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  142 static const uint8_t vcom_string0[] = {
vcom_string0:
        DATA
        DC8 4, 3, 9, 4
//  143   USB_DESC_BYTE(4),                     /* bLength.                         */
//  144   USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
//  145   USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
//  146 };
//  147 
//  148 /*
//  149  * Vendor string.
//  150  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  151 static const uint8_t vcom_string1[] = {
vcom_string1:
        DATA
        DC8 38, 3, 83, 0, 84, 0, 77, 0, 105, 0, 99, 0, 114, 0, 111, 0, 101, 0
        DC8 108, 0, 101, 0, 99, 0, 116, 0, 114, 0, 111, 0, 110, 0, 105, 0, 99
        DC8 0, 115, 0, 0, 0
//  152   USB_DESC_BYTE(38),                    /* bLength.                         */
//  153   USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
//  154   'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
//  155   'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
//  156   'c', 0, 's', 0
//  157 };
//  158 
//  159 /*
//  160  * Device Description string.
//  161  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  162 static const uint8_t vcom_string2[] = {
vcom_string2:
        DATA
        DC8 56, 3, 67, 0, 104, 0, 105, 0, 98, 0, 105, 0, 79, 0, 83, 0, 47, 0
        DC8 82, 0, 84, 0, 32, 0, 86, 0, 105, 0, 114, 0, 116, 0, 117, 0, 97, 0
        DC8 108, 0, 32, 0, 67, 0, 79, 0, 77, 0, 32, 0, 80, 0, 111, 0, 114, 0
        DC8 116, 0
//  163   USB_DESC_BYTE(56),                    /* bLength.                         */
//  164   USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
//  165   'C', 0, 'h', 0, 'i', 0, 'b', 0, 'i', 0, 'O', 0, 'S', 0, '/', 0,
//  166   'R', 0, 'T', 0, ' ', 0, 'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0,
//  167   'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0, 'M', 0, ' ', 0, 'P', 0,
//  168   'o', 0, 'r', 0, 't', 0
//  169 };
//  170 
//  171 /*
//  172  * Serial Number string.
//  173  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  174 static const uint8_t vcom_string3[] = {
vcom_string3:
        DATA
        DC8 8, 3, 50, 0, 54, 0, 56, 0
//  175   USB_DESC_BYTE(8),                     /* bLength.                         */
//  176   USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
//  177   '0' + CH_KERNEL_MAJOR, 0,
//  178   '0' + CH_KERNEL_MINOR, 0,
//  179   '0' + CH_KERNEL_PATCH, 0
//  180 };
//  181 
//  182 /*
//  183  * Strings wrappers array.
//  184  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  185 static const USBDescriptor vcom_strings[] = {
vcom_strings:
        DATA
        DC32 4, vcom_string0, 38, vcom_string1, 56, vcom_string2, 8
        DC32 vcom_string3
//  186   {sizeof vcom_string0, vcom_string0},
//  187   {sizeof vcom_string1, vcom_string1},
//  188   {sizeof vcom_string2, vcom_string2},
//  189   {sizeof vcom_string3, vcom_string3}
//  190 };
//  191 
//  192 /*
//  193  * Handles the GET_DESCRIPTOR callback. All required descriptors must be
//  194  * handled here.
//  195  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock0 Using cfiCommon0
          CFI Function get_descriptor
          CFI NoCalls
        THUMB
//  196 static const USBDescriptor *get_descriptor(USBDriver *usbp,
//  197                                            uint8_t dtype,
//  198                                            uint8_t dindex,
//  199                                            uint16_t lang) {
get_descriptor:
        PUSH     {R4}
          CFI R4 Frame(CFA, -4)
          CFI CFA R13+4
        MOVS     R4,R0
//  200 
//  201   (void)usbp;
//  202   (void)lang;
//  203   switch (dtype) {
        UXTB     R1,R1            ;; ZeroExt  R1,R1,#+24,#+24
        CMP      R1,#+1
        BEQ.N    ??get_descriptor_0
        BCC.N    ??get_descriptor_1
        CMP      R1,#+3
        BEQ.N    ??get_descriptor_2
        BCC.N    ??get_descriptor_3
        B.N      ??get_descriptor_1
//  204   case USB_DESCRIPTOR_DEVICE:
//  205     return &vcom_device_descriptor;
??get_descriptor_0:
        LDR.N    R0,??DataTable1
        B.N      ??get_descriptor_4
//  206   case USB_DESCRIPTOR_CONFIGURATION:
//  207     return &vcom_configuration_descriptor;
??get_descriptor_3:
        LDR.N    R0,??DataTable1_1
        B.N      ??get_descriptor_4
//  208   case USB_DESCRIPTOR_STRING:
//  209     if (dindex < 4)
??get_descriptor_2:
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        CMP      R2,#+4
        BGE.N    ??get_descriptor_1
//  210       return &vcom_strings[dindex];
        UXTB     R2,R2            ;; ZeroExt  R2,R2,#+24,#+24
        LDR.N    R0,??DataTable1_2
        ADDS     R0,R0,R2, LSL #+3
        B.N      ??get_descriptor_4
//  211   }
//  212   return NULL;
??get_descriptor_1:
        MOVS     R0,#+0
??get_descriptor_4:
        POP      {R4}
          CFI R4 SameValue
          CFI CFA R13+0
        BX       LR               ;; return
          CFI EndBlock cfiBlock0
//  213 }
//  214 
//  215 /**
//  216  * @brief   IN EP1 state.
//  217  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  218 static USBInEndpointState ep1instate;
ep1instate:
        DS8 20
//  219 
//  220 /**
//  221  * @brief   OUT EP1 state.
//  222  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  223 static USBOutEndpointState ep1outstate;
ep1outstate:
        DS8 20
//  224 
//  225 /**
//  226  * @brief   EP1 initialization structure (both IN and OUT).
//  227  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  228 static const USBEndpointConfig ep1config = {
ep1config:
        DATA
        DC32 2, 0H, sduDataTransmitted, sduDataReceived
        DC16 64, 64
        DC32 ep1instate, ep1outstate
        DC16 2
        DC8 0, 0
        DC32 0H
//  229   USB_EP_MODE_TYPE_BULK,
//  230   NULL,
//  231   sduDataTransmitted,
//  232   sduDataReceived,
//  233   0x0040,
//  234   0x0040,
//  235   &ep1instate,
//  236   &ep1outstate,
//  237   2,
//  238   NULL
//  239 };
//  240 
//  241 /**
//  242  * @brief   IN EP2 state.
//  243  */

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  244 static USBInEndpointState ep2instate;
ep2instate:
        DS8 20
//  245 
//  246 /**
//  247  * @brief   EP2 initialization structure (IN only).
//  248  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  249 static const USBEndpointConfig ep2config = {
ep2config:
        DATA
        DC32 3, 0H, sduInterruptTransmitted, 0H
        DC16 16, 0
        DC32 ep2instate, 0H
        DC16 1
        DC8 0, 0
        DC32 0H
//  250   USB_EP_MODE_TYPE_INTR,
//  251   NULL,
//  252   sduInterruptTransmitted,
//  253   NULL,
//  254   0x0010,
//  255   0x0000,
//  256   &ep2instate,
//  257   NULL,
//  258   1,
//  259   NULL
//  260 };
//  261 
//  262 /*
//  263  * Handles the USB driver global events.
//  264  */

        SECTION `.text`:CODE:NOROOT(1)
          CFI Block cfiBlock1 Using cfiCommon0
          CFI Function usb_event
        THUMB
//  265 static void usb_event(USBDriver *usbp, usbevent_t event) {
usb_event:
        PUSH     {R3-R5,LR}
          CFI R14 Frame(CFA, -4)
          CFI R5 Frame(CFA, -8)
          CFI R4 Frame(CFA, -12)
          CFI CFA R13+16
        MOVS     R4,R0
        MOVS     R5,R1
//  266   extern SerialUSBDriver SDU1;
//  267 
//  268   switch (event) {
        UXTB     R5,R5            ;; ZeroExt  R5,R5,#+24,#+24
        CMP      R5,#+0
        BEQ.N    ??usb_event_0
        CMP      R5,#+2
        BEQ.N    ??usb_event_1
        BCC.N    ??usb_event_2
        CMP      R5,#+4
        BEQ.N    ??usb_event_3
        BCC.N    ??usb_event_4
        CMP      R5,#+5
        BEQ.N    ??usb_event_5
        B.N      ??usb_event_6
//  269   case USB_EVENT_RESET:
//  270     return;
??usb_event_0:
        B.N      ??usb_event_7
//  271   case USB_EVENT_ADDRESS:
//  272     return;
??usb_event_2:
        B.N      ??usb_event_7
//  273   case USB_EVENT_CONFIGURED:
//  274     chSysLockFromIsr();
??usb_event_1:
        MOVS     R0,#+32
        MSR      BASEPRI,R0
          CFI FunCall dbg_check_lock_from_isr
        BL       dbg_check_lock_from_isr
//  275 
//  276     /* Enables the endpoints specified into the configuration.
//  277        Note, this callback is invoked from an ISR so I-Class functions
//  278        must be used.*/
//  279     usbInitEndpointI(usbp, USBD1_DATA_REQUEST_EP, &ep1config);
        LDR.N    R2,??DataTable1_3
        MOVS     R1,#+1
        MOVS     R0,R4
          CFI FunCall usbInitEndpointI
        BL       usbInitEndpointI
//  280     usbInitEndpointI(usbp, USBD1_INTERRUPT_REQUEST_EP, &ep2config);
        LDR.N    R2,??DataTable1_4
        MOVS     R1,#+2
        MOVS     R0,R4
          CFI FunCall usbInitEndpointI
        BL       usbInitEndpointI
//  281 
//  282     /* Resetting the state of the CDC subsystem.*/
//  283     sduConfigureHookI(&SDU1);
        LDR.N    R0,??DataTable1_5
          CFI FunCall sduConfigureHookI
        BL       sduConfigureHookI
//  284 
//  285     chSysUnlockFromIsr();
          CFI FunCall dbg_check_unlock_from_isr
        BL       dbg_check_unlock_from_isr
        MOVS     R0,#+0
        MSR      BASEPRI,R0
//  286     return;
        B.N      ??usb_event_7
//  287   case USB_EVENT_SUSPEND:
//  288     return;
??usb_event_4:
        B.N      ??usb_event_7
//  289   case USB_EVENT_WAKEUP:
//  290     return;
??usb_event_3:
        B.N      ??usb_event_7
//  291   case USB_EVENT_STALLED:
//  292     return;
??usb_event_5:
        B.N      ??usb_event_7
//  293   }
//  294   return;
??usb_event_6:
??usb_event_7:
        POP      {R0,R4,R5,PC}    ;; return
          CFI EndBlock cfiBlock1
//  295 }

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1:
        DC32     vcom_device_descriptor

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_1:
        DC32     vcom_configuration_descriptor

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_2:
        DC32     vcom_strings

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_3:
        DC32     ep1config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_4:
        DC32     ep2config

        SECTION `.text`:CODE:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
??DataTable1_5:
        DC32     SDU1
//  296 
//  297 /*
//  298  * USB driver configuration.
//  299  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  300 const USBConfig usbcfg = {
usbcfg:
        DATA
        DC32 usb_event, get_descriptor, sduRequestsHook, 0H
//  301   usb_event,
//  302   get_descriptor,
//  303   sduRequestsHook,
//  304   NULL
//  305 };
//  306 
//  307 /*
//  308  * Serial over USB driver configuration.
//  309  */

        SECTION `.rodata`:CONST:REORDER:NOROOT(2)
//  310 const SerialUSBConfig serusbcfg = {
serusbcfg:
        DATA
        DC32 USBD1
        DC8 1, 1, 2, 0
//  311   &USBD1,
//  312   USBD1_DATA_REQUEST_EP,
//  313   USBD1_DATA_AVAILABLE_EP,
//  314   USBD1_INTERRUPT_REQUEST_EP
//  315 };
//  316 
//  317 /* Virtual serial port over USB.*/

        SECTION `.bss`:DATA:REORDER:NOROOT(2)
//  318 SerialUSBDriver SDU1;
SDU1:
        DS8 600

        SECTION `.iar_vfe_header`:DATA:REORDER:NOALLOC:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, 0
        DATA
        DC32 0

        SECTION __DLIB_PERTHREAD:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        SECTION __DLIB_PERTHREAD_init:DATA:REORDER:NOROOT(0)
        SECTION_TYPE SHT_PROGBITS, 0

        END
//  319 #endif
// 
// 660 bytes in section .bss
// 340 bytes in section .rodata
// 164 bytes in section .text
// 
// 164 bytes of CODE  memory
// 340 bytes of CONST memory
// 660 bytes of DATA  memory
//
//Errors: none
//Warnings: none
