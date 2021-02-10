#!/usr/bin/env bash

DIR=$(cd -P -- "$(dirname -- "$0")" && pwd -P)"/"
sed -e "/###CSS###/{r ${DIR}style.css" -e 'd}' -e "/###JS###/{r ${DIR}script.js" -e 'd}' ${DIR}pinout.html | sed -e "/\/\/\/DATA\/\/\//{a \ \`" -e "r $1"  -e "a \ \`,\n///DATA///" -e "d}"
