#!/bin/sh

cd ../../..

export PROJECT_BOARD=proteus
export PROJECT_CPU=ARCH_STM32F4

sh config/boards/common_make.sh

