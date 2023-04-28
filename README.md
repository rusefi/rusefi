<div align="center">

<b>GPL open-source ECU</b>

[![Release](https://img.shields.io/github/v/release/FOME-Tech/fome-fw?style=flat)](https://github.com/FOME-Tech/fome-fw/releases/latest)![Last Commit](https://img.shields.io/github/last-commit/FOME-Tech/fome-fw?style=flat)
[![Unit Tests](https://img.shields.io/github/actions/workflow/status/FOME-Tech/fome-fw/build-unit-tests.yaml?label=Unit%20Tests&branch=master)](https://github.com/FOME-Tech/fome-fw/actions/workflows/build-unit-tests.yaml)
![GitHub commits since latest release (by date)](https://img.shields.io/github/commits-since/FOME-Tech/fome-fw/latest?color=blueviolet&label=Commits%20Since%20Release)
[![FOME Discord](https://img.shields.io/discord/1060875162892898324?label=Discord&logo=Discord)](https://discord.gg/5kC7vvpk7z)

</div>

# [FOME: Free Open Motorsports ECU](https://www.fome.tech/)

Welcome to FOME, a project intended to provide OEM quality open source engine controls with a focus on user-friendly design, stability and ease of use.  

FOME intends to narrow the gap between the features and functions seen on modern OEM engine control systems and that expected from stand-alone engine control systems. At the same time FOME intends to improve the ease of working with these systems and help provide access to the learning resources needed to understand and tune engines effectively.  

FOME came about from a desire to enhance the user experience of rusEFI that was shared by a number of members of that project; over time it became apparent that the best way to achieve this goal was to begin working on a fork. Thus at the start of 2023 FOME was founded.  

The founding members would like to express gratitude towards rusEFI for its ground breaking work and the innovation it has brought to the ECU market.  

At present FOME shares a large amount of its core with the rusEFI project and most rusEFI boards are compatible with the FOME software; this is likely something that will continue to diverge over time, meaning full compatibility with older RE hardware cannot be guaranteed.

# User Documentation

Online and offline (PDF) versions of the user manual available here: https://wiki.fome.tech/

# What do we have here?
 * [Firmware](/firmware) Source code for open source engine control unit for stm32 chips
 * [FOME console](/java_console) ECU debugging/development software
 * [Simulator](/simulator) Windows or Linux version of firmware allows exploration without any hardware 
 * [Unit Tests](/unit_tests) Unit tests of firmware
 * [Misc tools](/java_tools) Misc development utilities
 * [misc/Jenkins](/misc/jenkins) Continuous integration scripts

# External Links

TODO

 <!-- * [Forum](http://rusefi.com/forum)
 * [Documentation](https://github.com/rusefi/rusefi/wiki)
 * [Doxygen documentation](http://rusefi.com/docs/html)
 * [General source code Q&A](http://rusefi.com/forum/viewtopic.php?f=5&t=10)
 * [Facebook](https://www.facebook.com/rusEfiECU)
 * [YouTube](https://www.youtube.com/user/rusefi)
 * [Patreon](https://www.patreon.com/rusefi) -->

# Contributors

This project exists thanks to all the people who contribute their time, expertise, and testing.

If you'd like to get involved, see [contributing](CONTRIBUTING.md).

## Cloning the repository

`git clone https://github.com/FOME-Tech/fome-fw.git`

`git submodule update --init`

## Building

See [firmware/check.c](firmware/check.c) for the recommended version of GCC.

Check out https://rusefi.com/forum/viewtopic.php?f=5&t=9

# Release Notes

See [the changelog](firmware/CHANGELOG.md)
