#!/bin/bash

# Use this script for Proteus v0.1/0.2 (boards where vbat was on ADC3)

export PROTEUS_LEGACY=TRUE

bash ../common_make.sh config/boards/proteus/meta-info_f7.env
