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

All notable user-facing or behavior-altering changes will be documented in this file.

## Month 202x Release - "Release Name"

### Added
 - Ford PIP trigger decoder https://github.com/rusefi/rusefi/wiki/Images/triggers/trigger_65.png
 - Dwell battery voltage correction table

### Fixed
 - Fix of 36/2/2/2 attributes to help rotary #3242
 - SD card doesn't work on H7
 - rusEFI console to validate current bundle against bundle already in ECU #3266
 - GPPWM usability: notes field #3270

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
