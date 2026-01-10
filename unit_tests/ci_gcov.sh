#!/bin/bash

#
# this script is used by github actions
#

rm -rf gcov_working_area

mkdir gcov_working_area
cd gcov_working_area
mkdir gcov

echo -e "\nGenerating rusEFI unit test coverage"

# for debug use --html-details --html-single-page --verbose  to generate a single html
gcovr --exclude-throw-branches --exclude-unreachable-branches --decisions --merge-mode-functions=separate \
      --exclude '/.*/googletest/' \
      -j4  -r ../.. --html-nested gcov/index.html