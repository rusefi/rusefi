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

### Added
 - Detected gear and wastegate position to CAN format
 - Y axis override for Idle VE table. Helps with idle quality on ITB setups. #94
 - Blend tables for boost open and closed loop control #73

### Fixed
 - Sensor checker wasn't checking sensors
 - Removed vestigial warning about unset ignition pins #79
 - Gear detection sometimes didn't work

### For older changelog entries prior to fork from rusEFI, see CHANGELOG.rusefi.md
