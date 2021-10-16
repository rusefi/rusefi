<div align="center">

<img src="https://raw.githubusercontent.com/wiki/rusefi/rusefi/Images/logo_new.png" alt="rusEFI" width="600" />

<b>GPL open-source DIY ECU</b>

[![Release](https://img.shields.io/github/v/release/rusefi/rusefi?style=flat)](https://github.com/rusefi/rusefi/releases/latest) ![Last Commit](https://img.shields.io/github/last-commit/rusefi/rusefi?style=flat)
[![Rusefi on Slack](https://img.shields.io/badge/slack-rusefi-orange.svg?style=flat&logo=slack)](https://rusefi.slack.com)
</div>


# rusEFI

Current binaries are always available on our [build server](http://rusefi.com/build_server/)

[![paypal](https://img.shields.io/badge/%E2%99%A5%EF%B8%8Fdonate-orange)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=6YSSN35GWYS3A)

[![Tindie](http://rusefi.com/wiki/images/1/1c/Tindie-smalls.png)](https://www.tindie.com/stores/russian/)

# Cloning the repository

Important note - we now use submodules:

`git submodule update --init`

# What do we have here?
 * [Firmware](/firmware) Source code for open source engine control unit for stm32 chips
 * [Hardware](/hardware) KiCAD files for our older PCBs
 * [rusEFI console](/java_console) rusEFI own naive tuning software
 * [Simulator](/simulator) win32 or posix version of firmware allows to explore without any hardware 
 * [Unit Tests](/unit_tests) Unit tests of firmware pieces
 * [Misc tools](/java_tools) Misc development utilities
 * [misc/Jenkins](/misc/jenkins) Jenkins Continues Integration scripts
 * [Software Development process](misc/SoftwareDevelopmentProcess.md)
 * [Work in progress status](work_in_progress.md)

# External Links

 * Forum http://rusefi.com/forum
 * Documentation https://github.com/rusefi/rusefi/wiki
 * Doxygen documentation is available at http://rusefi.com/docs/html
 * General source code Q&A is at http://rusefi.com/forum/viewtopic.php?f=5&t=10
 * Facebook https://www.facebook.com/rusEfiECU
 * YouTube: https://www.youtube.com/user/rusefi
 * Patreon https://www.patreon.com/rusefi

# Building the Code
See https://rusefi.com/forum/viewtopic.php?f=5&t=9


# Release Notes

| Release date  | Revision  | Details |
| ------------  | --------- | ------- |
| 10/28/2020    |           | [changelog has moved](firmware/CHANGELOG.md) |
| 07/26/2020    | r24635    | improvement #1637: DC motor idle air valve for late 90s German vehicles |
| 07/21/2020    |           | bugfix #1592 injectors could stay open on transition from cranking to running under certain conditions | 
| 06/17/2020    | r23656    | bugfix #1491 major performance/scheduling improvement |
| 05/21/2020    | r22961    | rusEFI console start-up time improvements |
| 04/18/2020    | r22231    | Renix 44-2-2 trigger support added |
| 04/02/2020    |           | Start button feature |
| 03/28/2020    |           | Critical error text is now displayed in TunerStudio |
| 03/26/2020    |           | Multi-spark feature |
| 09/05/2019    | r19849    | Electronic Throttle Body including idle control seems to work |
| 07/28/2019    | r19612    | improvement #809: software jump to DFU |
| 04/25/2019    | r17317    | bugfix #775: electrical noise reboot during settings change causes with full tune loss |
| 04/07/2019    | r17098    | improvement #714: TLE8888 Enable Outputs SPI integration |
| 02/27/2019    | r16886    | bugfix #698: concurrency defect with single timer executor initialization |
| 02/23/2019    | r16857    | improvement #688: better support for 144 and 176 pin packages |
| 02/05/2019    | r16713    | improvement #631: ChibiOS 18 |
| 01/11/2019    | r16346    | bugfix #663: SPI fixes for CJ125 for stm32 errata STM32f405/7/15/17 |
| 12/09/2018    | r16057    | electronic throttle body control is now BETA version |
| 08/19/2018    | r15811    | bugfix #604: no interpolation outside of the table |
| 01/29/2018    | r15514    | improvement #215: CJ125 wideband controller |
| 01/23/2018    | r15442    | improvement #463: ChibiOS 17 / 4 |
| 01/07/2018    | r14952    | usability & minor bugfix #532 idle defaults are broken |
| 12/17/2017    | r14892    | bugfix #513: console: erase on older chips does not work |
| 11/26/2017    | r14789    | bugfix #500: set_rpn_expression command is broken |
| 11/23/2017    | r14779    | bugfix #497: engine does not start with SD card |
| 11/19/2017    | r14766    | improvement #496: console flasher to support older boards |
| 11/19/2017    | r14760    | improvement #495: incompatible change - larger settings area |
| 11/06/2017    | r14733    | improvement #487: single-point injection mode |
| 10/26/2017    | r14700    | improvement #385: narrow band to wide band conversion table |
| 08/31/2017    |           | improvement #442: ADC_VCC should be configurable |
| 07/24/2017    | r14513    | bugfix #307: TS bench test blinks three times |
| 07/13/2017    | r14476    | bugfix #461: SAXParserException on console start-up due to damaged settings.xml file |
| 07/09/2017    | r14473    | improvement: IAC solenoid frequency changeable on the fly |
| 06/19/2017    | r14393    | bugfix: pinMode, milMode |
| 05/27/2017    | r14221    | improvement: ochGetCommand to support offset and count |
| 05/05/2017    | r13974    | bugfix #404: 36/1 FATAL error: angle range trgSync |
| 05/03/2017    | r13967    | improvement: ChibiOS 3.2 |
| 04/06/2017    | r13759    | major improvement #72: ChibiOS 3.1 |
| 03/26/2017    | r13330    | super annoying bug #336 fixed |
| 03/20/2017    | r13233    | improvements #375 & #376: hard FPU mode & migrating to fresh version of arm gcc |
| 03/19/2017    | r13223    | bugfix #374: persistent configuration starts to touch firmware - settings would need to be reloaded |
| 03/09/2017    | r13146    | bugfix #370: duty cycle vs error code 6050 |
| 03/06/2017    | r13123    | bugfix #363: 2/1 skipped wheel is a corner-case |
| 03/05/2017    | r13108    | bugfix #363: trigger front only processing |
| 02/22/2017    | r12980    | bugfix: false error message in case of single coil or simultaneous injection | 
| 02/22/2017    | r12973    | unused property 'custom Use Rise Edge' removed |
| 02/22/2017    | r12972    | protocol signature changed to 'v0.02' (this would happen from time to time to ensure version match between console and TS project | 
| 02/20/2017    | r12939    | https://github.com/rusefi/rusefi is now the official primary repository |
| 02/18/2017    | r11565    | improvement: level1 default brown out https://sourceforge.net/p/rusefi/tickets/354/ |
| 02/18/2017    | r11554    | bugfix: rusEfi console program/erase buttons fixed with ST-LINK 2.1 |
| 12/09/2016    | r10991    | bugfixes, bugfixes, bugfixes & release 1.0
| 02/15/2016	| r9600     | automatic warm-up fuel correction, performance improvements
| 08/31/2015	| r8725     | acceleration fuel correction, console improvements
| 07/06/2015	| r8554     | more CAN, better console, bugfixes, performance
| 04/26/2015	| r7938	    | vehicle speed sensor, stepper idle valve, lots of improvements
| 11/13/2014	| r5327	    | ChibiOS/RT 2.6.6, better cold start logic, bugfixes, bugfixes, bugfixes...
| 06/12/2014	| r3477	    | Precise event scheduling, protocol selection 
| 03/26/2014	| r2413	    | C++, refactoring & improvements
| 02/23/2013	| r1777	    | trigger is now configurable via TunerStudio
| 01/30/2014	| r1309	    | true trigger angles, VBatt signal
| 01/12/2014	| r1007	    | refactoring, refactoring & refactoring
| 12/19/2013	| r605	    | ignition control & a little bit of CAN bus
| 11/08/2013	| r100	    | Tuner Studio fuel map tuning, fuel pump control
| 10/14/2013	| r39	    | USB serial bug, missing IAR files, self-contained Makefile
| 10/13/2013	| r33	    | IAR project file
| 10/04/2013	| r26	    | Patched ChibiOS/RT 2.6.1 sources are now included
| 09/23/2013	| r20	    | Tuner Studio integration, configuration persistence
| 08/30/2013	| r14	    | initial documentation & refactoring. tunerstudio integration
| 08/03/2013	| r13	    | wideband O2 input, better idling algorithm, serial-over-USB
| 07/05/2013	| r10	    | Second CKP, sequential injection.
| 06/19/2013	| r9	    | Initial version - batch injection & ignition with advance table lookup.
