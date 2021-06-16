#!/usr/bin/env bash

DIR=$(cd -P -- "$(dirname -- "$0")" && pwd -P)"/"
TEXT=$(sed -e "/###CSS###/{r ${DIR}style.css" -e 'd}' -e "/###JS###/{r ${DIR}script.js" -e 'd}' ${DIR}pinout.html | sed -e "s/\/\/\/DATA\/\/\//\`$(echo ${1//\//\\/} | tr -d '\n')\`,\n\/\/\/DATA\/\/\//")
echo "$TEXT" > $2
