################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../chibios/os/hal/src/adc.c \
../chibios/os/hal/src/can.c \
../chibios/os/hal/src/ext.c \
../chibios/os/hal/src/gpt.c \
../chibios/os/hal/src/hal.c \
../chibios/os/hal/src/i2c.c \
../chibios/os/hal/src/icu.c \
../chibios/os/hal/src/mac.c \
../chibios/os/hal/src/mmc_spi.c \
../chibios/os/hal/src/mmcsd.c \
../chibios/os/hal/src/pal.c \
../chibios/os/hal/src/pwm.c \
../chibios/os/hal/src/rtc.c \
../chibios/os/hal/src/sdc.c \
../chibios/os/hal/src/serial.c \
../chibios/os/hal/src/serial_usb.c \
../chibios/os/hal/src/spi.c \
../chibios/os/hal/src/tm.c \
../chibios/os/hal/src/uart.c \
../chibios/os/hal/src/usb.c 

OBJS += \
./chibios/os/hal/src/adc.o \
./chibios/os/hal/src/can.o \
./chibios/os/hal/src/ext.o \
./chibios/os/hal/src/gpt.o \
./chibios/os/hal/src/hal.o \
./chibios/os/hal/src/i2c.o \
./chibios/os/hal/src/icu.o \
./chibios/os/hal/src/mac.o \
./chibios/os/hal/src/mmc_spi.o \
./chibios/os/hal/src/mmcsd.o \
./chibios/os/hal/src/pal.o \
./chibios/os/hal/src/pwm.o \
./chibios/os/hal/src/rtc.o \
./chibios/os/hal/src/sdc.o \
./chibios/os/hal/src/serial.o \
./chibios/os/hal/src/serial_usb.o \
./chibios/os/hal/src/spi.o \
./chibios/os/hal/src/tm.o \
./chibios/os/hal/src/uart.o \
./chibios/os/hal/src/usb.o 

C_DEPS += \
./chibios/os/hal/src/adc.d \
./chibios/os/hal/src/can.d \
./chibios/os/hal/src/ext.d \
./chibios/os/hal/src/gpt.d \
./chibios/os/hal/src/hal.d \
./chibios/os/hal/src/i2c.d \
./chibios/os/hal/src/icu.d \
./chibios/os/hal/src/mac.d \
./chibios/os/hal/src/mmc_spi.d \
./chibios/os/hal/src/mmcsd.d \
./chibios/os/hal/src/pal.d \
./chibios/os/hal/src/pwm.d \
./chibios/os/hal/src/rtc.d \
./chibios/os/hal/src/sdc.d \
./chibios/os/hal/src/serial.d \
./chibios/os/hal/src/serial_usb.d \
./chibios/os/hal/src/spi.d \
./chibios/os/hal/src/tm.d \
./chibios/os/hal/src/uart.d \
./chibios/os/hal/src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
chibios/os/hal/src/%.o: ../chibios/os/hal/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DCORTEX_USE_FPU=TRUE -I".." -I"../hw_layer/stm32f4" -I../chibios/os/kernel/include -I"../ext" -I"../ext_algo" -I"../chibios/os/various" -I"../hw_layer/algo" -I"../util" -I"../console" -I"../console/binary" -I"../config" -I"../config/engines" -I"../config/boards" -I"../controllers" -I"../controllers/algo" -I"../controllers/core" -I"../controllers/trigger" -I"../controllers/sensors" -I"../controllers/system" -I"../controllers/math" -I"../console_util" -I"../development" -I"../development/test" -I"../development/hw_layer" -I"../hw_layer" -I"../hw_layer/flash" -I"../hw_layer/lcd" -I"../hw_layer/serial_over_usb" -I"../russianefi" -I"../russianefi/algo" -I"../russianefi/ckp" -I"../russianefi/adc" -I"../config/system" -I"../config/stm32f4ems" -I"../splib" -I"../chibios/os/kernel/include" -I"../chibios/boards/ST_STM32F4_DISCOVERY" -I"../chibios/os/hal/src" -I"../chibios/os/hal/include" -I"../chibios/os/hal/platforms/STM32/TIMv1" -I"../chibios/os/hal/platforms/STM32/I2Cv1" -I"../chibios/os/hal/platforms/STM32" -I"../chibios/os/hal/platforms/STM32/RTCv2" -I"../chibios/os/hal/platforms/STM32/OTGv1" -I"../chibios/os/hal/platforms/STM32/GPIOv2" -I"../chibios/os/hal/platforms/STM32F4xx" -I"../chibios/os/hal/platforms/STM32/SPIv1" -I"../chibios/os/hal/platforms/STM32/USARTv1" -I"../chibios/os/ports/GCC/ARMCMx" -I"../chibios/os/ports/GCC/ARMCMx/STM32F4xx" -I"../chibios/os/ports/common/ARMCMx" -I"../chibios/os/ports/common/ARMCMx/CMSIS/include" -std=gnu11 -fgnu89-inline -c -Werror=type-limits -Werror=uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


