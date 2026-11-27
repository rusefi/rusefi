#!/bin/bash

#
# install-gcovr.sh - Install gcovr code coverage tool
#
# Installs python3-pip and gcovr, used for generating unit test coverage reports.
#

sudo apt-get install python3-pip

# local install would be
#   python3 -m venv venv && ./venv/bin/pip install gcovr
# and access
#   ./venv/bin/gcovr
# The CI image does not install libxml2/libxslt development headers.
pip install --break-system-packages --only-binary=lxml gcovr
