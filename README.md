*** test poke do not merge ***

<div align="center">

<img src="https://raw.githubusercontent.com/wiki/rusefi/rusefi/Images/logo_new.png" alt="rusEFI" width="600" />

<b>GPL open-source ECU</b>

[![Release](https://img.shields.io/github/v/release/rusefi/rusefi?style=flat)](https://github.com/rusefi/rusefi/releases/latest)![Last Commit](https://img.shields.io/github/last-commit/rusefi/rusefi?style=flat)
![GitHub commits since latest release (by date)](https://img.shields.io/github/commits-since/rusefi/rusefi/latest?color=blueviolet&label=Commits%20Since%20Release)
</div>

# rusEFI

[![paypal](https://img.shields.io/badge/%E2%99%A5%EF%B8%8Fdonate-orange)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=6YSSN35GWYS3A)

[shop.rusEFI.com](https://www.shop.rusefi.com/)

# Download

Current binaries are always available on our [build server](http://rusefi.com/build_server/)

# Release Notes

See [the changelog](firmware/CHANGELOG.md)

# What do we have here?
 * [Firmware](/firmware) Source code for open source engine control unit for stm32 chips including [implementation overview](https://github.com/rusefi/rusefi/blob/master/firmware/readme.md)
 * [Hardware](/hardware) KiCAD files for our older PCBs
 * [rusEFI console](/java_console) rusEFI own native tuning software
 * [Simulator](/simulator) win32 or posix version of firmware allows to explore without any hardware
 * [Unit Tests](/unit_tests) Unit tests of firmware pieces
 * [Misc tools](/java_tools) Misc development utilities
 * [Software Development process](misc/SoftwareDevelopmentProcess.md)

# External Links

 * [Forum](http://rusefi.com/forum)
 * [Documentation](https://wiki.rusefi.com)
 * [Doxygen documentation](http://rusefi.com/docs/html)
 * [General source code Q&A](http://rusefi.com/forum/viewtopic.php?f=5&t=10)
 * [Facebook](https://www.facebook.com/rusEfiECU)
 * [YouTube](https://www.youtube.com/user/rusefi)
 * [Patreon](https://www.patreon.com/rusefi)

# Cloning the repository
`git clone https://github.com/rusefi/rusefi.git`
Important note - we now use submodules:

`git submodule update --init`

# Building the Code

See [gcc_version_check.c](https://github.com/rusefi/rusefi/blob/master/firmware/gcc_version_check.c) for recommended version of GCC.

Check out https://rusefi.com/forum/viewtopic.php?f=5&t=9

