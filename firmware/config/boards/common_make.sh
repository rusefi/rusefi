#!/bin/bash

#
# historically current directory for bundle builder was a sub-folder of 'config/boards/'
# this script is just a facade for more flexible script which assume invocation from firmware directory
#

BOARD_META_PATH=$1

cd ../../..

bash config/boards/common_script.sh $BOARD_META_PATH
