#!/bin/bash

# Use this script for Proteus v0.1/0.2 (boards where vbat was on ADC3)

export PROTEUS_LEGACY=TRUE

bash ../common_make.sh proteus ARCH_STM32F7
