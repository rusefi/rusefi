@echo off

rm -f libtinyc.a
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard libc_stub.c 
arm-none-eabi-ar rcs libtinyc.a libc_stub.o abort.c.o memcmp.c.o memcpy.c.o memset.c.o strcmp.c.o strcpy.c.o strlen.c.o strncmp.c.o strncpy.c.o
