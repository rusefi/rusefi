#!/bin/bash

#
# we use this script to reduce github actions load by executing some tasks less frequent than always
# param: THRESHOLD, rate of SKIPPING task, from 0 to 100. No skipping if zero, always skipping if 100.
#

THRESHOLD=$1
if [ -z $THRESHOLD ] ; then
  echo "Throttle threshold not specified, allowing always"
  exit 0
fi

RANDOM100=$(($RANDOM % 100))
msg="Random ${RANDOM100} while threshold $THRESHOLD:"

if (( ${RANDOM100} > $THRESHOLD )); then
  echo "$msg Allowing"
  exit 0
else
  echo "$msg Not this time, maybe next time"
  exit -1
fi
