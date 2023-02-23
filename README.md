<div align="center">

<b>GPL open-source DIY ECU</b>

[![Release](https://img.shields.io/github/v/release/FOME-Tech/fome-fw?style=flat)](https://github.com/FOME-Tech/fome-fw/releases/latest)![Last Commit](https://img.shields.io/github/last-commit/FOME-Tech/fome-fw?style=flat)
![GitHub commits since latest release (by date)](https://img.shields.io/github/commits-since/FOME-Tech/fome-fw/latest?color=blueviolet&label=Commits%20Since%20Release)
</div>

# [FOME: Free Open Motorsports ECU](https://www.fome.tech/)

# User Documentation

Online and offline (PDF) versions of the user manual available here: https://wiki.fome.tech/


# Cloning the repository

`git clone https://github.com/FOME-Tech/fome-fw.git`

`git submodule update --init`

# What do we have here?
 * [Firmware](/firmware) Source code for open source engine control unit for stm32 chips
 * [rusEFI console](/java_console) ECU debugging/development software
 * [Simulator](/simulator) win32 or posix version of firmware allows to explore without any hardware 
 * [Unit Tests](/unit_tests) Unit tests of firmware pieces
 * [Misc tools](/java_tools) Misc development utilities
 * [misc/Jenkins](/misc/jenkins) Jenkins Continuous Integration scripts

# External Links

TODO

 <!-- * [Forum](http://rusefi.com/forum)
 * [Documentation](https://github.com/rusefi/rusefi/wiki)
 * [Doxygen documentation](http://rusefi.com/docs/html)
 * [General source code Q&A](http://rusefi.com/forum/viewtopic.php?f=5&t=10)
 * [Facebook](https://www.facebook.com/rusEfiECU)
 * [YouTube](https://www.youtube.com/user/rusefi)
 * [Patreon](https://www.patreon.com/rusefi) -->

# Building the Code

See [check.c](https://github.com/FOME-Tech/fome-fw/blob/master/firmware/check.c) for recommended version of GCC.

Check out https://rusefi.com/forum/viewtopic.php?f=5&t=9

# Release Notes

See [the changelog](firmware/CHANGELOG.md)
