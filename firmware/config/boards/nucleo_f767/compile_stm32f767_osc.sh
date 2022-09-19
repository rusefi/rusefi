#!/bin/bash

# STM32F767 version of the firmware for F767ZI chip NOT on Nucleo board, with a real oscilattor

export EXTRA_PARAMS="-DDUMMY -DSTM32F767xx \
 -DEFI_INJECTOR_PIN3=Gpio::Unassigned \
 -DFIRMWARE_ID=\\\"board767\\\" \
 -DEFI_COMMUNICATION_PIN=Gpio::B7 \
 -DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::B14 \
 -DEFI_ENABLE_ASSERTS=FALSE \
 -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE"

# Do not forget to comment out following line if looking to debug!
export DEBUG_LEVEL_OPT="-O2"

bash ../common_make.sh nucleo_f767 ARCH_STM32F7

