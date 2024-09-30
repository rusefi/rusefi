#!/bin/bash

sudo dfu-util --alt 0 --download rusefi.bin -s 0x8000000:leave
