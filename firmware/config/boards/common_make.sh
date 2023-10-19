#!/bin/bash

#
# historically current directory for bundle builder was a sub-folder of 'config/boards/'
# this script is just a facade for more flexible script which assume invocation from firmware directory
#

PROJECT_BOARD=$1
PROJECT_CPU=$2

cd ../../..

bash config/boards/common_script.sh $PROJECT_BOARD $PROJECT_CPU
