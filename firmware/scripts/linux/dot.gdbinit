# copy this file to firmware directory and rename to '.gdbinit' to make gdb automaticly connect to OpenOCD and upload firmware on start
# see gdb.sh

target extended-remote localhost:3333
file build/rusefi.elf

load
set breakpoint auto-hw on
set remote hardware-breakpoint-limit 8
set remote hardware-watchpoint-limit 4
