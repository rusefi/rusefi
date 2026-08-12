End users should be able to use pre-built firmware. They should not need to build or modify the source code.
See https://wiki.rusefi.com/Download

See also https://wiki.rusefi.com/Dev-Quick-Start

[Doxygen](https://rusefi.com/docs/html/) <<< landing page has best implementation introduction.

[Q&A on source code](https://rusefi.com/forum/viewtopic.php?f=5&t=10)

This directory contains the source code for the rusEFI embedded firmware.

TL;DR

``make``

# Environment

Embedded firmware is build on top of https://www.chibios.org/ with plain Makefile gcc version 12 (See https://github.com/rusefi/rusefi/blob/master/.github/workflows/hardware-ci.yaml to confirm current GCC version)

Windows development is fully supported with Cygwin, WSL or Linux is recommended due to poor NTFS performance.

See also [../simulator](../simulator)

See also [../unit_tests](../unit_tests)
