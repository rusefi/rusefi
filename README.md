# rusEFI
A GPL open-source DIY ECU

current binaries are always available at http://rusefi.com/build_server/

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=6YSSN35GWYS3A)


[![Tindie](http://rusefi.com/wiki/images/1/1c/Tindie-smalls.png)](https://www.tindie.com/stores/russian/)

# Releases

| Release date  | Revision  | Details |
| ------------  | --------- | ------- |
| 01/29/2018    | r15514    | improvement #215: CJ125 wideband contoller |
| 01/23/2018    | r15442    | improvement #463: ChibiOS 17 / 4 |
| 01/07/2018    | r14952    | usability & minor bugfix #532 idle defaults are broken |
| 12/17/2017    | r14892    | bugfix #513: console: erase on older chips does not work |
| 11/26/2017    | r14789    | bugfix #500: set_rpn_expression command is broken |
| 11/23/2017    | r14779    | bugfix #497: engine does not start with SD card |
| 11/19/2017    | r14766    | improvement #496: console flasher to support older boards |
| 11/19/2017    | r14760    | improvement #495: incomppatible change - larger settings area |
| 11/06/2017    | r14733    | improvement #487: single-point injection mode |
| 10/26/2017    | r14700    | improvement #385: narrow band to wide band conversion table |
| 08/31/2017    |           | improvement #442: ADC_VCC should be configurable |
| 07/24/2017    | r14513    | bugfix #307: TS bench test blinks three times |
| 07/13/2017    | r14476    | bugfix #461: SAXParserException on console start-up due to damaged settings.xml file |
| 07/09/2017    | r14473    | improvement: IAC solenoid frequencty changeable on the fly |
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
| 12/09/2016    | r10991    | bugfixes, bugfixes, bugfixes & releave 1.0
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
| 10/14/2013	| r39	    | USB serial bug, missing IAR files, self-containted Makefile
| 10/13/2013	| r33	    | IAR project file
| 10/04/2013	| r26	    | Patched ChibiOS/RT 2.6.1 sources are now included
| 09/23/2013	| r20	    | Tuner Studio integration, configuraton persistence
| 08/30/2013	| r14	    | initial documentation & refactoring. tunerstudio integration
| 08/03/2013	| r13	    | wideband O2 input, better idling algorithm, serial-over-USB
| 07/05/2013	| r10	    | Second CKP, sequential injection.
| 06/19/2013	| r9	    | Initial version - batch injection & ignition with advance table lookup.
	
# Links

 * Wiki: http://rusefi.com
 * Forum http://rusefi.com/forum
 * Doxygen documentation is available at http://rusefi.com/docs/html
 * General source code Q&A is at http://rusefi.com/forum/viewtopic.php?f=5&t=10
 * Facebook https://www.facebook.com/rusEfiECU
 * YouTube: https://www.youtube.com/user/rusefi

# Building the Code

## Cloning the repository

We now use submodules:

`git submodule update --init`
