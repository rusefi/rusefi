#!/bin/bash

set -e

make clean
time make -j16 2>b > a
