/*
        *** THIS IS A FILE FOR DOXYGEN AND NOT A C HEADER FILE ***
*/

/**
 *******************************************************************************
 ** \mainpage 
 ** \{
 *******************************************************************************
 ** \image html cypress_logo.png
 ** 
 ** <h1>Copyright</h1>
 ** 
 ** Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
 ** You may use this file only in accordance with the license, terms, conditions,
 ** disclaimers, and limitations in the end user license agreement accompanying
 ** the software package with which this file was provided.
 ** CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
 ** OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
 ** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 ** MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 ** PURPOSE.
 **
 ** <h1>Overview</h1>
 **
 ** Cypress provides the Peripheral Driver Library (PDL) to simplify software 
 ** development for the FM0+, and FM4 MCU families. The PDL reduces the need to
 ** understand register usage and bit structures, thus easing software development 
 ** for the extensive set of peripherals in the FM families. You configure the 
 ** library for the particular device, and then use API calls to initialize and
 ** use a peripheral. Using the PDL makes it easier to port code from one family
 ** to the other, because the same code supports all FM families.<br>
 ** This documentation provides technical information on each supported peripheral 
 ** driver, including:<br>
 ** * Configuration considerations
 ** * Macros
 ** * Functions
 ** * Global variables
 ** * Data structures
 ** * Enumerated types
 **
 ** For a general introduction to the PDL, read 
 ** Peripheral Driver Library Quick Start Guide.
 **
 ** <h1>PDL Example Code</h1>
 **
 ** The PDL installation includes code examples with IDE project files configured
 ** for particular FM portfolio starter kits. You find the code examples in the 
 ** <i>/examples</i> folder.<br>
 ** Each example demonstrates how to configure, initialize, and use a peripheral.
 ** Some peripherals have multiple examples.<br>
 ** Each code example project uses two files that implement the example: main.c 
 ** and pdl_user.h. The main.c file has code that implements the example. The 
 ** pdl_user.h file enables or disables the PDL capabilities required for the 
 ** example. Each example also has a readme file that explains the example.
 **
 ** To use an example, open the project file for your preferred IDE. See the 
 ** <i>PDL Quick Start Guide</i> section “Build and Run a PDL Project” for details.
 **
 ** <table>
 ** <caption id="multi_row">Example Code Support</caption>
 ** <tr><th rowspan="2">Code example     <th rowspan="2">Associated Peripheral <th colspan="2">Development Kit
 ** <tr>                                                                        <th>SK_S6E1B8    <th>SK_S6E2GM
 ** <tr><td>adc_scan_dma             <td>ADC     <td>-    <td>+
 ** <tr><td>adc_scan_multich_polling_sw   <td>ADC     <td>+    <td>+
 ** <tr><td>adc_scan_onech_polling_sw     <td>ADC     <td>+    <td>+
 ** <tr><td>bt_ppg                   <td>BT      <td>+    <td>+
 ** <tr><td>bt_pwc                   <td>BT      <td>+    <td>+
 ** <tr><td>bt_pwm                   <td>BT      <td>+    <td>+
 ** <tr><td>bt_rt                    <td>BT      <td>+    <td>+
 ** <tr><td>can_simple               <td>CAN     <td>-    <td>+
 ** <tr><td>clk_gating               <td>CLK     <td>+    <td>+
 ** <tr><td>clk_init                 <td>CLK     <td>+    <td>+
 ** <tr><td>cr_trimming              <td>CRC     <td>+    <td>+
 ** <tr><td>crc_16_32                <td>CRC     <td>+    <td>+
 ** <tr><td>csv_reset                <td>CSV     <td>+    <td>+
 ** <tr><td>fcs_int                  <td>CSV     <td>+    <td>+
 ** <tr><td>fcs_reset                <td>CSV     <td>+    <td>+
 ** <tr><td>dma_software             <td>DMA     <td>-    <td>+
 ** <tr><td>dstc_hw_transfer         <td>DSTC    <td>+    <td>+
 ** <tr><td>dstc_sw_transfer         <td>DSTC    <td>+    <td>+
 ** <tr><td>dt_unuse_int             <td>DT      <td>+    <td>+
 ** <tr><td>dt_use_int               <td>DT      <td>+    <td>+
 ** <tr><td>exint_simple             <td>EXINT   <td>+    <td>+
 ** <tr><td>toggling_led             <td>GPIO    <td>+    <td>+
 ** <tr><td>lpm_deep_standby_rtc_mode <td>LPM    <td>+    <td>+
 ** <tr><td>lpm_rtc_mode             <td>LPM     <td>+    <td>+
 ** <tr><td>lpm_sleep_mode           <td>LPM     <td>+    <td>+
 ** <tr><td>lvd_interrupt            <td>LVD     <td>+    <td>+
 ** <tr><td>lvd_polling              <td>LVD     <td>+    <td>+
 ** <tr><td>csio_interrupt           <td>MFS     <td>+    <td>+
 ** <tr><td>csio_polling             <td>MFS     <td>+    <td>+
 ** <tr><td>csio_using_cs            <td>MFS     <td>+    <td>+
 ** <tr><td>i2c_master_polling       <td>MFS     <td>+    <td>+
 ** <tr><td>i2c_slave_polling        <td>MFS     <td>+    <td>+
 ** <tr><td>uart_dma                 <td>MFS     <td>-    <td>+
 ** <tr><td>uart_fifo_interrupt      <td>MFS     <td>+    <td>+
 ** <tr><td>uart_interrupt           <td>MFS     <td>+    <td>+
 ** <tr><td>uart_polling             <td>MFS     <td>+    <td>+
 ** <tr><td>uart_printf              <td>MFS     <td>+    <td>+
 ** <tr><td>frt_interrupt            <td>MFT     <td>+    <td>+
 ** <tr><td>icu_interrupt            <td>MFT     <td>+    <td>+
 ** <tr><td>wfg_rt_ppg_mode          <td>MFT     <td>+    <td>+
 ** <tr><td>rtc_calibration          <td>RTC     <td>+    <td>+
 ** <tr><td>rtc_count                <td>RTC     <td>+    <td>+
 ** <tr><td>rtc_timer                <td>RTC     <td>+    <td>+
 ** <tr><td>uid_read                 <td>UID     <td>+    <td>+
 ** <tr><td>vbat_gpio                <td>VBAT    <td>+    <td>-
 ** <tr><td>hwwdg                    <td>WDG     <td>+    <td>+
 ** <tr><td>swwdg_normal             <td>WDG     <td>+    <td>+
 ** <tr><td>swwdg_window_mode        <td>WDG     <td>+    <td>+
 ** </table>
 ** 
 ** <h1>Utility</h1>
 ** The PDL provides various utility files such as redirecting standard I/O to 
 ** serial terminal and interfacing to different on-board devices.
 **
 ** <table>
 ** <tr><th>Name<th>Details
 ** <tr><td> EEPROM         <td> I2C interface EEPROM                          
 ** <tr><td> HYPER_FLASH    <td> Hyper Bus Flash                               
 ** <tr><td> I2S_CODEC      <td> I2S codec                                     
 ** <tr><td> NAND_FLASH     <td> Access Nand Flash via External Bus Interface  
 ** <tr><td> PRINTF         <td> Printf via UART                               
 ** <tr><td> QSPI_FLASH     <td> Access Quad-SPI Flash via High speed Quad-SPI 
 ** <tr><td> SD_CARD        <td> Access SD card via SDIO interface             
 ** <tr><td> SDRAM          <td> Access SDRAM via External Bus Interface       
 ** <tr><td> SEG_LCD        <td> Control Segment LCD via LCD controller        
 ** </table>
 **
 **
 ** <h1>Mapping Peripheral Signals to Physical I/O Pins</h1>
 **
 **
 ** Most peripherals can be connected to two or more device I/O ports. You 
 ** control which port to use (called “pin relocation”) by software.
 **
 ** After device reset each I/O is configured to its default function as shown 
 ** in the following table.
 **
 ** <table>
 ** <caption id="multi_row">Initially Selected Functions for Each I/O Port after Reset Is Released</caption>
 ** <tr><th>Pin name        <th>Initial pin function
 ** <tr><td>SWCLK, SWDIO    <td>Serial Wire Debug (SWD) pin is selected. Pull-up is enabled.
 ** <tr><td>ANxx            <td>Can be used as an analog input pin. Digital input is cut off and ‘0’ is input.
 ** <tr><td>X0,X1,X0A, X1A  <td>Can be used as an oscillation pin. Digital input is cut off and ‘0’ is input.
 ** <tr><td>All GPIO pins other than the above pins   <td>Digital input. Output is Hi-Z.
 ** </table>
 ** 
 ** The supported pin functions for each I/O vary among products and packages. 
 ** Consult the “Pin Assignment” and “List of Pin Functions” sections of the 
 ** device-specific datasheet to determine which pin can be assigned to which
 ** peripheral signal.
 ** 
 ** For example, the following figure shows an excerpt from the LQFP-80 package 
 ** view in the S6E1B8 Series device datasheet. For each I/O there is a list of 
 ** pin names provided. The number after the underscore ("_") in pin names such 
 ** as XXX_0 and XXX_1 indicates the relocated port number. These signals can be
 ** connected to several device pins.
 **
 ** \image html pin_relocation.png
 ** 
 ** To determine the pin functions for a specific pin search for the pin name in 
 ** the “List of Pin Functions” table of the device datasheet. For example, pin 
 ** 60 can be used as:
 ** * P21 – General-purpose I/O port 2[pin 1]
 ** * AN18 – ADC input channel 18
 ** * SIN0_0 – MFS channel 0 input pin
 ** * INT06_1 – external interrupt request 6 input pin
 ** * WKUP2 – deep standby mode return signal input pin 
 ** * SEG11 – LCD controller segment 11 output pin
 **
 ** To assign a pin function use the appropriate macro. The file gpio_<package>.h 
 ** defines macro names, in the form: SetPinFunc_<pin_name>. 
 ** 
 ** So, to assign pin 60 as SIN0_0, the macro name is SetPinFunc_SIN0_0, and the 
 ** code would be <code>SetPinFunc_SIN0_0();</code>
 **
 ** The gpio_<package>.h file is device-specific.
 ** 
 ** For FM0+ portfolio the files are located: <i>/devices/fm0p/<package>/common/</i>
 **
 ** For FM4 portfolio the location is: </i>/devices/fm4/<package>/common/</i>
 ** <h1>Revision History</h1>
 **
 ** The following table shows the overall high-level updates. Refer to the PDL Release Notes for more details.
 ** <table>
 ** <tr><th>Version<th>Description
 ** <tr><td> 2.1.0     <td>
 **                        * Code Examples - PDL now includes ready to use project file for 
 **                          each IDE as a part of every code example, no need to reconfigure.<br>
 **                        * Low-level programming - Preconfigured project file for each IDE provided
 **                          for every device package.<br>
 **                        * Added support for iSYSTEM winIDEA v9.12, Atollic TrueSTUDIO v5.5, and Makefile
 **                          support with GCC from ARM Embedded.<br>
 **                        * Peripherals - various enhancements and corrections.<br>
 **                        * Device support – Now CMSIS v4.5 compliant dedicated device header file and startup code
 **                          for every device package.
 ** </table>
 ** 
 ** <h1>More Information</h1>
 **
 ** For more information on the pin relocation, refer to:<br> 
 ** <a href="http://www.cypress.com/file/223051/download">FM0+ PERIPHERAL MANUAL - Core Subsystem</a><br>
 ** <a href="http://www.cypress.com/file/222996/download">FM4 PERIPHERAL MANUAL - Core Subsystem</a><br>
 ** The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
 ** <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
 ** FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
 ** <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
 ** FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
 ** \}
 ******************************************************************************/
