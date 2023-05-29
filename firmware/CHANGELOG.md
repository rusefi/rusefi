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

### Removed
 - 

 -->

# Changelog

## Unreleased

### Fixed
 - Improved bench test resolution (more usable for testing injectors, dwell, etc)

## May 2023 Release

### Added
 - Detected gear and wastegate position to CAN format
 - Y axis override for Idle VE table. Helps with idle quality on ITB setups. #94
 - Blend tables for boost open and closed loop control #73
 - Log channels to make MegaLogViewer VE Analyze work #82
 - Enable knock sensing on F407 Discovery build, hardcoded to PA3 #105
 - Support gauge and differential fuel pressure sensors (in addition to absolute) #32
 - Snapshot builds have a date code in the folder name #91

### Fixed
 - Sensor checker wasn't checking sensors
 - Removed vestigial warning about unset ignition pins #79
 - Gear detection sometimes didn't work #86
 - Restore built-in INI storage on STM32F7-based ECUs
 - Improve log field names, easier to find the channel you're looking for #98

### For older changelog entries prior to fork from rusEFI, see CHANGELOG.rusefi.md
