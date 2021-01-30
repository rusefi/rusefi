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

## XXX 2021 Release

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
