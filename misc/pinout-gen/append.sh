#!/usr/bin/env bash

DIR=$(cd -P -- "$(dirname -- "$0")" && pwd -P)"/"
TEXT=$(sed -e "/\/\/\/DATA\/\/\//{a \ \`$1\ \`,\n///DATA///" -e "d}" $2)
echo "$TEXT" > $2
