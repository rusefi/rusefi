In this folder we have a naive test suite.

I am not sure what is the best approach to tests in C (someone can educate me), but my approach is:

Unit tests are not aware of ChibiOS or ARM or else, they are just plain C/C++ which you build for your desktop, not your MCU.

Run 'make' to build desktop binary & run it on your PC/Mac, it's expected to say SUCCESS and not fail :)