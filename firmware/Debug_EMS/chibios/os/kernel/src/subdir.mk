################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../chibios/os/kernel/src/chcond.c \
../chibios/os/kernel/src/chdebug.c \
../chibios/os/kernel/src/chdynamic.c \
../chibios/os/kernel/src/chevents.c \
../chibios/os/kernel/src/chheap.c \
../chibios/os/kernel/src/chlists.c \
../chibios/os/kernel/src/chmboxes.c \
../chibios/os/kernel/src/chmemcore.c \
../chibios/os/kernel/src/chmempools.c \
../chibios/os/kernel/src/chmsg.c \
../chibios/os/kernel/src/chmtx.c \
../chibios/os/kernel/src/chqueues.c \
../chibios/os/kernel/src/chregistry.c \
../chibios/os/kernel/src/chschd.c \
../chibios/os/kernel/src/chsem.c \
../chibios/os/kernel/src/chsys.c \
../chibios/os/kernel/src/chthreads.c \
../chibios/os/kernel/src/chvt.c 

OBJS += \
./chibios/os/kernel/src/chcond.o \
./chibios/os/kernel/src/chdebug.o \
./chibios/os/kernel/src/chdynamic.o \
./chibios/os/kernel/src/chevents.o \
./chibios/os/kernel/src/chheap.o \
./chibios/os/kernel/src/chlists.o \
./chibios/os/kernel/src/chmboxes.o \
./chibios/os/kernel/src/chmemcore.o \
./chibios/os/kernel/src/chmempools.o \
./chibios/os/kernel/src/chmsg.o \
./chibios/os/kernel/src/chmtx.o \
./chibios/os/kernel/src/chqueues.o \
./chibios/os/kernel/src/chregistry.o \
./chibios/os/kernel/src/chschd.o \
./chibios/os/kernel/src/chsem.o \
./chibios/os/kernel/src/chsys.o \
./chibios/os/kernel/src/chthreads.o \
./chibios/os/kernel/src/chvt.o 

C_DEPS += \
./chibios/os/kernel/src/chcond.d \
./chibios/os/kernel/src/chdebug.d \
./chibios/os/kernel/src/chdynamic.d \
./chibios/os/kernel/src/chevents.d \
./chibios/os/kernel/src/chheap.d \
./chibios/os/kernel/src/chlists.d \
./chibios/os/kernel/src/chmboxes.d \
./chibios/os/kernel/src/chmemcore.d \
./chibios/os/kernel/src/chmempools.d \
./chibios/os/kernel/src/chmsg.d \
./chibios/os/kernel/src/chmtx.d \
./chibios/os/kernel/src/chqueues.d \
./chibios/os/kernel/src/chregistry.d \
./chibios/os/kernel/src/chschd.d \
./chibios/os/kernel/src/chsem.d \
./chibios/os/kernel/src/chsys.d \
./chibios/os/kernel/src/chthreads.d \
./chibios/os/kernel/src/chvt.d 


# Each subdirectory must supply rules for building sources it contributes
chibios/os/kernel/src/%.o: ../chibios/os/kernel/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I../chibios/os/kernel/include -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I"../chibios/os/ports/common/ARMCMx/CMSIS/include" -std=gnu11 -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


