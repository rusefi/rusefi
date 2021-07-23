TL, DR: just follow [tests](tests) folder as examples

In this folder we have rusEFI unit tests using https://github.com/google/googletest

Unit tests are not aware of ChibiOS or ARM or else, they are just plain C/C++ which you build for your desktop, not your MCU.

1. Run 'make' to build desktop binary.
1. Execute rusefi_test binary on your PC/Mac, it's expected to say SUCCESS and not fail :) Googletest will also print results summary.

[Code Coverage Report](https://rusefi.com/docs/unit_tests_coverage/)

See also [https://github.com/rusefi/rusefi/wiki/Build-Server-and-Automation](https://github.com/rusefi/rusefi/wiki/Build-Server-and-Automation)
