<div align="center">


<img src="https://raw.githubusercontent.com/wiki/rusefi/rusefi/Images/logo_new.png" alt="rusEFI" width="600" />

<b>GPL open-source DIY ECU</b>

[![Release](https://img.shields.io/github/v/release/rusefi/rusefi?style=flat)](https://github.com/rusefi/rusefi/releases/latest) ![Last Commit](https://img.shields.io/github/last-commit/rusefi/rusefi?style=flat)
[![Rusefi on Slack](https://img.shields.io/badge/slack-rusefi-orange.svg?style=flat&logo=slack)](https://rusefi.slack.com)
</div>

# rusEFI

Current binaries are always available on our [build server](http://rusefi.com/build_server/)

[![paypal](https://img.shields.io/badge/%E2%99%A5%EF%B8%8Fdonate-orange)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=6YSSN35GWYS3A)

[shop.rusEFI.com](https://www.shop.rusefi.com/)

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
 * [misc/Jenkins](/misc/jenkins) Jenkins Continuous Integration scripts
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

See [the changelog](firmware/CHANGELOG.md)
