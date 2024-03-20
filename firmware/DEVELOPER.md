See also [../unit_tests](../unit_tests)

This directory contains the source code for the RusEFI firmware.

The ideal is that typical end users should be able to use pre-built
firmware.  They should not need to modify or even rebuild from the
source code for basic use, but building from the source code provides
the opportunity for optimization, supporting unexpected engine
configurations, and specialized enhancements.


TL;DR

``make PROJECT_BOARD=microrusefi PROJECT_CPU=ARCH_STM32F4``

# Environment

Building from source code requires this firmware, and a platform that
supports 'make' based builds. The correct compiler is now included for
both 64-bit Linux (Intel/AMD) and macOS (64-bit Intel and ARM) platforms.

Linux and MacOS systems should have the software development tools,
primarily 'make', pre-installed or readily installed.  MS-Windows
requires selecting and installing a Unix-compatible environment.

## Setup Instructions

## Editor/IDE

The supported IDE is [Visual Studio Code](https://code.visualstudio.com/).
Others will work, but if you need help from the maintainers, you might not
get much if you're using something else.

### Linux

1. Make sure you have the latest version of git installed: `sudo apt install git`
1. Clone the repo. `git clone https://github.com/<github username>/fome-fw.git`
1. Install additional dependencies: `cd fome-fw/firmware && ./setup_linux_environment.sh`
1. Open vscode: `code fome-fw/`

### macOS

macOS setup is largely the same as Linux, except the setup script will not work.
On recent versions of macOS, simply attempting to run `make` from the terminal
will prompt installation of Xcode command line developer tools, which is the only
prerequisite. Then, do steps 2 and 4 from the Linux setup instructions above.

### Windows

While technically possible to build on Windows, the preferred method is to first install
the [Windows Subsystem for Linux (WSL)](), then follow the [instructions for Linux](#linux)
above.

## Note

Note that the developers are volunteers, with varied motivations.
These motivations often include using leading-edge language and build
system concepts, requiring recent versions of tools.  Should you
encounter build problems, review the latest version of this document.
If you're still having trouble, reach out on Discord in the #firmware channel.
