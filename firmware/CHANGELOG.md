<!---

How to use this file:
- When making a change to the firmware that users should know about, add it to the
	appropriate section (breaking/added/fixed) in the next release.
- If it's something interesting or breaking, make a post about it on the forum! We want users
	to find out about changes before they have to get help because their car won't start.

Release template (copy/paste this for new release):

## Month 202x Release - "Release Name"

### Breaking Changes
 -   

### Added
 -   

### Fixed
 -   

 -->

# Changelog

## Unreleased

### Added

### Fixed

### Removed

## August 2022 Release - "Day 170"

### Added
 - raw voltage gauges for Lua script aux analog inputs #4346
 - microRusEFI and Proteus F4 have CAN OpenBLT bootloader #4199 #4230
 - counter-noise measure for custom skipped trigger wheels #4375
 - Write hard faults to backup ram, print on next boot #4324
 - Many more options for Lua CAN rx filters/callbacks #4387
 - Password protection against tune access #4243
 - Additional CAN messages #4401
 - Option to invert VVT control (exhaust cams, etc) #4424
 - Raw Battery gauge
 - More accurate/stable VVT angle calculation #4433

### Fixed
 - Lua CAN reception fixed for 11-bit IDs where the frame would be received, but a corrupt ID was passed to the handler function. #4321
 - Many drop downs menues are now sorted #4339
 - rusEFI TS plugin launcher fixed
 - Console autoupdate error dialogs #4352
 - custom skipped wheel could be located on camshaft #4377
 - it's impossible to receive AcceleratorPedal sensor via CAN/Lua #4369
 - MRE stepper disables LS2 control #4381
 - VVT target tables allow negative values #4424

### Removed
 - ICU trigger input logic since it is unused in any current ECU #639
 - Idle PID "offset" field, as this role is filled more effectively by the various open loop parameters.

## July 2022 Release - "Day 130"

### Added
 - verbose trigger sync should reuse engineSnifferRpmThreshold #4259
 - Subaru EZ30 variation of 36/2/2/2 trigger
 - Linux version of rusEFI simulator SocketCAN integration (#4312)

### Fixed
 - Improved logic used to disambiguate trigger sync using cam/VVT information. Engine now runs in wasted spark until cam sync is achieved, at which point it switches to fully sequential. #4099
 - visiting "live data" tab causes fatal error on ECU #4271
 - SD card logs are broken by double fuel: wall correction #4291
 - RPM-related tune fields can now hold larger values for higher rev limits, etc #4298

## June 2022 Release - "Day 98"

### Added
 - FasterEngineSpinUp should be enabled by default #4094
 - ETB low limit is now configurable #4119
 - Advanced Trigger option: require engine phase synchronization in sequential mode #4151
 - Maintainability: frequency sensors need a simple event counter in the logs #4173
 - TS bench test commands are now configurable #4192
 - microRusEFI can now use more pins for Cam position sensor #3032
 - hard requirement for console version match #4187

### Fixed
 - Make 36-2 trigger sync parameters more generous #4132
 - Synchronized Phase handling fixes #4099

### Removed
 - Various Honda 24/4/1 trigger modes (that didn't work very well), replaced with "12 tooth crank" mode.

## April 2022 Release - "Day 58"

All notable user-facing or behavior-altering changes will be documented in this file.

### Fixed
 - An attempt to make 'Trigger' dialog a bit less confusing #4021
 - Fixed knock threshold table, improved knock sensing status gauges

### Added
 - Mitsubishi 36-2-1-1 trigger wheel
 - Mitsubishi 3A92 VVT trigger
 - Toyota 4-1 VVT trigger
 - use extra RAM for lua if your microRusEFI unit has stm32f42x microcontroller 
 - VVT activation delay #3443
 - Allow setting injection phase by start or center of injection #4068
 - New 'View' menu in TunerStudio showing a lot of live data points #3614

## March 2022 Release - "Day 22"

### Added
 - Use board-specific output and digital input pin names in error messages #3886
 - Support LPS22 in addition to LPS25 baro sensors #3900
 - Proteus F7 flashing work-around 

### Fixed
 - SD card logging with SDIO hardware #3873 
 - rusEFI console stability improvement #3912
 - console updater does not work if folder name contains spaces #3927
 - SD card logs PPS as TPS #3943

## Removed
 - Miata NB1 trigger was merged with Miata NB2 trigger
 - FSIO

## January 2022 Release - "Green Juice"

### Added
 - Per-cylinder fuel trim tables
 - Per-cylinder ignition timing trim tables
 - SDRAM reference binary for stm32f429-discovery

### Fixed
 - rusefi_Console: Too long re-connection time #3305
 - Bosch Quick Start on B6 Passat is confused about phase #3812

## December 2021 Release - "Cookie Exchange"

### Added
 - Improved vehicle speed sensor configuration: now uses real physical constants about tires, gear ratio, sensor, etc.
 - Improved priming logic. Now includes a table of priming fuel mass vs. engine temperature, in addition to a delay before priming to allow fuel pressure to build. #3674
 - ISO-TP connector in firmware & ISO-TP to TCP/IP bridge in rusEFI console #3667
 - Lua: mcu_stop method for stm32 F4 and F7

### Fixed
 - Faster engine sync + startup on engines with crank-speed primary trigger

### Added
 - Lua 'Format' button

## November 2021 Release - "Loosen Up Lighten Up Day"

### Added
 - Higher Data Rate with TS 3.1.06.14 Beta or newer 
 - turbo speed sensor #2935
 - A TON of Lua progress
 - Overall performance
 - Knock retard #3396

### Fixed
 - A/C control bugfix
 - Better Windows build-in DFU #3338
 - Proteus now waits for VBatt to enable main relay output

## Removed
 - Remove engine load acceleration enrichment #3357
 - FSIO removed from user interface #2928

## October 2021 Release - "Pierogi Day"

### Added
 - Ford PIP trigger decoder https://github.com/rusefi/rusefi/wiki/Images/triggers/trigger_65.png
 - Dwell battery voltage correction table
 - rusEFI firmware feature should be honest about it's Windows-only limitation
 - Bosch MM5.10 accelerometer integration
 - CLT/IAT sensor calibration presets

### Fixed
 - Fix of 36/2/2/2 attributes to help rotary #3242
 - SD card doesn't work on H7
 - rusEFI console to validate current bundle against bundle already in ECU #3266
 - GPPWM usability: notes field #3270
 - overdwell protection #3220

## September 2021 Release - "Iguana Awareness Day"

### Added
 - RUSEFI MSD now contains bundle-specific URL #2848
 - Nissan 4 cylinder QR trigger wheel #3118
 - 1-6-2-4-3-5 firing order for all our 911 fans!
 - Add minimum MAP threshold for closed-loop boost control
 - rusEFI console Lua tab loads scripts from ECU on start

### Fixed
 - Composite Logger uses same engineSnifferRpmThreshold setting as engine sniffer #3161

## Removed
 -  TS file read feature #3004

## July 2021 Release "Ice Cream Day"

### Breaking Changes
 - vvtOffset field migrated to four vvtOffsets fields. Anyone using VVT would need to manually adjust their configuration.  

### Added
 - Injector nonlinearity (small pulse) correction - so far just polynomial, but table modes coming soon.
 - 1-4-3-6-2-5 firing order for VAG v6
 - vq30de trigger wheel decoder #2974
 - new hardware testing console commands #2978
 
### Fixed
 - rusEFI console start-up speed #2964
 - wrong handling of symmetrical crank wheels NB2 VQ35 Renix #2980   

## June 2021 Release "National Logistics Day"

### Fixed
 - Improved cranking defaults
 - Simplified/clarified idle and cranking TunerStudio pages
 - Minor change of Miata NA trigger shape #2855
 - Improved trigger sync for 60/2 36/1 36/22 triggers as part of VW warnings on cranking part #1 #2861   

### Added
 - Idle improvements! Open loop corrections are now applied in closed loop mode.
 - Electronic throttle rev limiter: close the throttle before hitting hard limit, OEM style.
 - "inhibit start until oil pressure" prevents starting the engine with no/low oil pressure #2799
 - Dual cooling fans, with new "enable fan with AC" option
 - Lua scripting: implement custom ECU behavior with a Lua script. https://github.com/rusefi/rusefi/wiki/Lua-Scripting

# 2021 May "Piercing Day"
### Fixed
 - LCD screen works again #2576
 - Support STM32F7 1MB devices
### Added
 - Per-bank closed loop fuel correction (each cylinder corrected by the sensor that sees its exhaust)

## April 2021 Release - "Malbec" 
### Added
 - mount SD card and drive containing TunerStudio ini at the same time
 - VAG MQB tachometer CAN integration
### Fixed
 - fix error/warning buffer overflow #2456
 - engine sniffer "show only interesting" fix #2528
 - SD card logs bugfix #2556 
 - Too many small logs are zero size #2553
 - Launcher splash screen is freezy #2560
 - NB2 trigger decoder #2533

## April 2021 Release "Be Kind to Lawyers Day"
### Added
 - mount SD card and drive containing TunerStudio ini at the same time
### Fixed
 - engine sniffer "show only interesting" fix #2528

## Potato Chip Day 2021 Release

### Added
 - VAG MQB tachometer CAN integration
    
### Fixed
 - fix error/warning buffer overflow #2456

## Life in the Cracks 2021 Release

### Added
 - USB Mass Storage: The connected SD card will be mounted over USB if rusEFI connected to a PC via USB.
 - GM 60/2/2/2 trigger https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers#gm
 - TriTach trigger https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers#tritach
 - Skoda Favorit trigger https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers#skoda-favorit
 - Add fallback logic handling failed MAP sensor.  In case of failed MAP, ses either a fixed MAP value, or a table that estimates MAP based on TPS and RPM.
 - STM32H7 is mostly working

### Fixed
 - Improved fuel/ignition bench testing precision - now accurate to ~1 microsecond #2440
 - Fix internal MCU temperature sensing on STM32F7-based ECUs #2392

### Breaking Changes
 - VVT target is now taken from VVT not FSIO table #2243

### 2021 Printing Ink Day

### Breaking Changes
 - Closed loop idle timing behavior changed to no longer fall off control as entering/leaving the idle region. It now sharply engages/disengages upon entering/leaving the idle area.
 - Idle phase logic uses the same idle detection thresholds as the main idle controller instead of its own thresholds.

### Added
 - Main Relay bench test button

### Fixed

### 2021 Orthodox Christmas Day

### Breaking Changes

### Added
 - Second wideband O2 processing as channels "Lambda 2" and "Air/Fuel Ratio 2".  Currently only supports CAN sensors, but analog will be supported too soon.
 - Basic fueling-only flex fuel implementation. Automatic adjustment of stoichiometric ratio based on ethanol content, compatible with the common GM/Continental 50-150hz flex fuel sensor 🎉 🎉 🎉

### Fixed
 - Simplify idle control dialog in TunerStudio
 - microRusEFI outputs are in trouble on configuration change
 - CLT gauge no longer erroneously shows "deg F" by default.

### December 2020 Bake Cookies Day

### Breaking Changes
 - GM x24 trigger now uses 0 offset (offset is now built in, previously needed 50 degrees offset)

### Added
 - DAIHATSU 3/4 cylinder trigger https://rusefi.com/images/triggers/trigger_6.png
 - Ford ST170 VVT cam trigger https://rusefi.com/images/triggers/trigger_42.png
 - Changed scheduling timer driver to improve scheduling accuracy #1983

### Fixed
 - microRusEFI GP3 & GP4 fixed

## December 2020 National Mutt Day

### Fixed
 -  minor bugfix - engine sniffer missing pin names #1995
 
## November 2020 Release - "Black Friday"

### Breaking Changes
 -   "AFR target" table is now "Lambda target" table.  It should convert itself correctly, but a backup of that table is recommended when updating firmware/ini.
 -   "Base cranking fuel" is now in milligrams, using the full injector model at all times now.  60mg per liter per cylinder is a good starting point

### Added
 -   High & low pressure fuel sensor channels
 -   Electronic throttle-style wastegate control found on some VW turbos (and VGTs)
 -   Improved TLE8888 driver on microRusEFI 
 -   Improved setting ECU presets/defaults from TunerStudio

### Fixed
 -   Improved TunerStudio protocol reliability - should see fewer CRC errors now #1943

## October 2020 Release - "Sausage Pizza Day"

### Added
 -   `iat` as FSIO variable to read intake air temperature
 -   [ALPHA] Software based knock detection on Proteus v0.4+.  Possible on microRusEFI and Frankenso too with hardware modification.
 -   [ALPHA] Early framing work for automatic transmission control!

### Fixed
 -   Fixed fuel pump logic
 -   Improve trigger input/decode and PWM performance
 -   Fixes to SD card logging


# Old Changelog

| Release date  | Revision  | Details |
| ------------  | --------- | ------- |
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
