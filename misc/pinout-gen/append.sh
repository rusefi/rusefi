#!/usr/bin/env bash

DIR=$(cd -P -- "$(dirname -- "$0")" && pwd -P)"/"
TEXT=$(sed -e "/\/\/\/DATA\/\/\//{a \ \`" -e "r $1"  -e "a \ \`,\n///DATA///" -e "d}" $2)
echo "$TEXT" > $2
