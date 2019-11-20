
This configuration
1) builds bootloader from firmware/bootload just to validate that it builds (TODO: extract to separate job?)
1) compiles two versions of F4 version of firmware - with and without asserts, _fast_ and _debug_.
1) flashes _debug_ version of F4 into stm32f4discovery and runs a suite of tests against it. The source code
of tests is embedded into rusEfi console by the way (TODO: nicer UI for console to test F4 outside of Jenkins?)

```misc/jenkins/functional_test_and_build_bundle/run.bat```