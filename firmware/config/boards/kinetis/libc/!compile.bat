@echo off
set PATH=C:\Program Files\Java\jre\bin;G:\VStudio\Subversion\bin;G:\VStudio\arm-elf-gcc\bin;G:\VStudio\MinGW\msys\1.0\bin;G:\VStudio\Git\bin

rm -f libtinyc.a
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard libc_stub.c 
arm-none-eabi-ar rcs libtinyc.a libc_stub.o abort.c.o memcmp.c.o memcpy.c.o memset.c.o strcmp.c.o strcpy.c.o strlen.c.o strncmp.c.o strncpy.c.o
